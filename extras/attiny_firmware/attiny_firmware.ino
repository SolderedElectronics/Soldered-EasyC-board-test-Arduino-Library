/**
 **************************************************
 *
 * @file        Template for attiny_firmware
 * @brief       Fill in sensor specific code.
 *

 *
 * @authors     @ soldered.com
 ***************************************************/

#include "easyC.h"
#include <Wire.h>

#define PWM 0x3 // The code for the PWM mode

int addr = DEFAULT_ADDRESS;

byte registers[3];  // Registers for configuration
byte pinConfig[12]; // Array where is stored configuration for each pin. The index represents the pin and the value
                    // represents the mode (1 - 4)
bool received;      // The flag to know when the data is received
bool reading;       // The flag to know when is received a pin from which read
byte readDigital;   // The flag to know when to read the data

void setup()
{
    initDefault();
    addr = getI2CAddress();

    //Serial.begin(115200);
    //Serial.println("Bok, ja glumim atiny");

    Wire.begin(0x30); //malo je problem kad je tu addr, onda nece, ali mozda je zato kad sam testirao na dasduinu
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    memset(pinConfig, 9, sizeof(pinConfig)); // The default value of the pin configuration register is 9 and represents
                                             // non - configured pins because there are only defined 4 modes
}

void loop()
{
    // If data are received and stored in the registers, we can set the pins depending on registers values
    if (received == 1)
    {
        // Just to print the bits we send
        //printBinary(registers[0]);
        //printBinary(registers[1]);
        //printBinary(registers[2]);
        if ((registers[0] & 0b10000000) != 0) // If the MSB is set to 1 it means we need to configure the pins
        {
            byte mode;
            byte modeMask = 0b00000011;
            mode = (modeMask & registers[1]); // Get mode from the register[1] - last 2 LSB bits represents mode
            registers[0] &= 0b01111111;       // Reset the MSB to get the number of the pin in this register
            pinConfig[registers[0]] = mode;   // Write the pin mode for this pin in registers[0]
            switch (mode)
            {
            case 0: // 0 represents INPUT
                pinMode(registers[0], INPUT);
                break;
            case 1: // 1 represents OUTPUT
                pinMode(registers[0], OUTPUT);
                break;
            case 2: // 2 represents INPUT_PULLUP
                pinMode(registers[0], INPUT_PULLUP);
                break;
            case 3: // 3 represents PWM
                pinMode(registers[0], OUTPUT);
                break;
            }
        }
        else // We need to set the value on the pin
        {
            registers[0] &= 0b01111111;       // Reset the MSB to get the number of the pin in this register
            if (pinConfig[registers[0]] == 1) // If the pin configured as an output
            {
                // Write the output depend on the registers[2] - the same as the 5th bit in registers[1]
                if (registers[2] != 0)
                {
                    digitalWrite(registers[0], HIGH);
                }
                else
                {
                    digitalWrite(registers[0], LOW);
                }
            }

            if (pinConfig[registers[0]] == 3) // If the pin is configured as PWM, write PWM to the pin
            {
                analogWrite(registers[0], registers[2]);
            }
        }
        received = 0;
        //print();
    }

    if (reading == 1) // When is known which pin needs to be read, read it
    {
        // Serial.println("moramo nesto procitati!"); //Just for debugging
        readDigital = digitalRead(registers[0]);
        reading = 0; // Set the flag to 0 and not execute this until receiving a new pin for reading
    }
}


void receiveEvent(int howMany)
{
    if (Wire.available() == 3) // If received 3 bytes, this is for the configuration or to set the output
    {
        byte err = Wire.readBytes(registers, 3);
        if (err == 0)
        {
            Serial.println("Podaci nisu valjani");
        }
        else
        {
            received = 1;
        }
    }

    if (Wire.available() == 1) // If received 1 byte, this is the pin from which we want to read analog or digital
    {
        // Save this pin to the registers[0] and set the flag to 1 to know the pin for reading is received
        registers[0] = Wire.read();
        reading = 1;
    }
}

void requestEvent()
{
    Wire.write(readDigital); // Send the master the read value
}

/*
// Just to see bytes as bits for debugging purposes
void printBinary(byte inByte)
{
    for (int b = 7; b >= 0; b--)
    {
        Serial.print(bitRead(inByte, b));
        // if(b == 4) Serial.print(" "); // Uncomment for separate bytes in print -> 0000 0000
    }
    Serial.println();
}

// Just to print the pin configuration array for debugging purposes
void print()
{
    for (int i = 0; i < 12; ++i)
    {
        Serial.print(pinConfig[i]);
        Serial.print(" ");
    }
    Serial.println();
}
*/
