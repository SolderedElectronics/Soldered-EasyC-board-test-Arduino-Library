/**
 **************************************************
 *
 * @file        digitalWriteEasyC.ino
 * @brief       Example to control the LED on the breakout with another Arduino
 *
 *  For this example you need Dasduino, LED and some sensor with easyC and attiny404.
 *  Upload attiny firmware to breakout, this example to the Dasduino and connect Dasduino and breakout with easyC.
 *
 * @authors     Karlo Leksic for soldered.com
 ***************************************************/

#include "EasyC-board-test-SOLDERED.h"

#define TEST_PIN PA7 // Define the pin on which LED is connected

EasyCBoardTest test;

void setup()
{
    test.begin();
    test.set(TEST_PIN, OUTPUT); // Set pin mode to the OUTPUT
}

void loop()
{
    test.pinWrite(TEST_PIN, HIGH); // Turn on the LED on the TEST_PIN
    delay(1000);
    test.pinWrite(TEST_PIN, LOW); // Turn off the LED on the TEST_PIN
    delay(1000);
}
