/**
 **************************************************
 *
 * @file        EasyC-board-test-SOLDERED.cpp
 * @brief       Functions for testing attiny board via easyC.
 *
 *
 * @copyright   GNU General Public License v3.0
 * @authors     Karlo Leksic for soldered.com
 ***************************************************/


#include "EasyC-board-test-SOLDERED.h"

/**
 * @brief                   Constructor for the EasyC board test.
 */
EasyCBoardTest::EasyCBoardTest()
{
    native = 0;
}

/**
 * @brief                   Overloaded function for virtual in base class to initialize sensor specific.
 */
void EasyCBoardTest::initializeNative()
{
}

/**
 * @brief                   This function is for setting a pin to one of 4 modes described below
 *
 * @param pin               The number of pin to configure as numbers or ports
 *
 * @param mode              Which of the mode the pin will be
 *
 * @return                  None
 */
void EasyCBoardTest::set(int pin, byte mode)
{
    registers[0] = pin;       // registers[0] represents the number of the pin
    registers[0] |= (1 << 7); // Set MSB to 1 to tell breakout to write pin configuration

    byte mask = 0b00000011; // Mask for writing mods in the register in the last 2 bits
    registers[1] = 0;       // Register for the pin mode and digitalWrite value

    // Two LSB bits are pin mode:
    // 00 - input mode
    // 01 - output mode
    // 10 - input pullup mode
    // 11 - pwm mode

    if (mode == INPUT)
    {
        registers[1] |= (mask & 0); // Write 00 to the LSB without changing other bits in the register
    }

    if (mode == OUTPUT)
    {
        registers[1] |= (mask & 1); // Write 01 to the LSB without changing other bits in the register
    }

    if (mode == INPUT_PULLUP)
    {
        registers[1] |= (mask & 2); // Write 10 to the LSB without changing other bits in the register
    }

    if (mode == PWM)
    {
        registers[1] |= (mask & 3); // Write 11 to the LSB without changing other bits in the register
    }

    registers[2] = 0; // Register for PWM value is empty

    sendBytes(); // Send those 3 bytes to the slave device
    delay(10);
}

/**
 * @brief                      This function writes the value to the pin given in the function. It can be for digital
 * writing or PWM
 *
 * @param pin                  The number of pins to configure as numbers or ports
 *
 * @param value                The value to set on the pin
 *
 * @return                     None
 */
void EasyCBoardTest::pinWrite(int pin, byte value)
{
    registers[0] = (byte)pin; // registers[0] represents the number of the pin

    // 00000X00 - X bit in register[1] is for the digital state of the pin
    if (value == 0)
    {
        registers[1] |= (0 << 2); // Writing X bit to 0
    }
    else // If the value is different from 0, it means digital write 1
    {
        registers[1] |= (1 << 2); // Writing X bit to 1
    }

    registers[2] = value; // Also set the complete value to the PWM register

    sendBytes(); // Send those 3 bytes to the slave device
    delay(10);
}

/**
 * @brief                      Function to read the value from pin
 *
 * @param pin                  The pin from which the value is read
 *
 * @return                     The read value
 */
byte EasyCBoardTest::readPin(byte pin)
{
    // Send the pin from which we want to read
    Wire.beginTransmission(0x30);
    Wire.write(pin);
    Wire.endTransmission();

    delay(10);

    // It sends back the read value
    Wire.requestFrom(0x30, 1);
    delay(20);

    byte readValue = 0;
    if (Wire.available())
    {
        readValue = Wire.read();
    }

    return readValue;
}

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

/**
 * @brief                      Function to send bytes to the slave
 *
 * @return                     The error code of the end transmission
 */
byte EasyCBoardTest::sendBytes()
{
    // da vidimo kaj se salje
    printBinary(registers[0]);
    printBinary(registers[1]);
    printBinary(registers[2]);

    Wire.beginTransmission(0x30);
    Wire.write(registers, 3);
    byte err = Wire.endTransmission();
    return err;
}
