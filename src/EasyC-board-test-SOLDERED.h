/**
 **************************************************
 *
 * @file        EasyC-board-test-SOLDERED.h
 * @brief       Header file for EasyC board test.
 *
 *
 * @copyright   GNU General Public License v3.0
 * @authors     Karlo Leksic for soldered.com
 ***************************************************/

#ifndef __EASYC_BOARD_TEST__
#define __EASYC_BOARD_TEST__

#define PWM 0x3 // The code for the PWM mode

// IO port bank A defines
#define PA0 11
#define PA1 8
#define PA2 9
#define PA3 10
#define PA4 0
#define PA5 1
#define PA6 2
#define PA7 3 // USED BY DEFAULT

// IO port bank B defines
#define PB0 7
#define PB1 6
#define PB2 5 // USED BY DEFAULT
#define PB3 4 // USED BY DEFAULT

#include "Arduino.h"
#include "libs/Generic-easyC/easyC.hpp"

class EasyCBoardTest : public EasyC
{
  public:
    EasyCBoardTest();
    void set(int pin, byte mode);
    void pinWrite(int pin, byte value);
    byte readPin(byte pin);

  protected:
    void initializeNative();

  private:
    byte sendBytes();

    // Registers where is stored data for pin, mode, and value
    byte registers[3];
    // First byte is pin which we want configure
    // Second byte use last 3 bits, 2 last to represent functionality
    // The last byte is for PWM value
};

#endif
