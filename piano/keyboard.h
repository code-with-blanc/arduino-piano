#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "keyboardHardware.h"

class Keyboard {
  public:
    Keyboard();
    void scanBank(byte bankNum);
    void forEachPressed(void(*)(int keyNum));

  private:
    byte _keys[NUM_BANKS*NUM_KEYS];
    byte _lastBank = 255;
    byte _lastKey = 255;
    unsigned long _lastDetection = 0;
};

#endif
