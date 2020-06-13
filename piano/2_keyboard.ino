#include "pins.h"
#include "keyboard.h"

#define NONE (unsigned char) 255


Keyboard::Keyboard() {
  for(byte i = 0; i < sizeof(_keys); i++) {
    _keys[i] = 0;
  }
}

void Keyboard::scanBank(byte bankNum) {
  KeyboardHardware::setActiveBank(bankNum);
  delayMicroseconds(10);
  
  // scan keys
  for(int k = 0; k < NUM_KEYS; k++) {
    byte newValue = KeyboardHardware::readKey(k);
    bool oldValue = _keys[bankNum*NUM_KEYS + k];

    if(newValue != oldValue) {
      _keys[bankNum*NUM_KEYS + k] = newValue;
      _lastDetection = millis();
      _lastBank = bankNum;
      _lastKey = k;
    }
  }
}

void Keyboard::forEachPressed(void(*callback)(int keyNum)) {
  for(int i = 0; i<sizeof(_keys); i++) {
    if(_keys[i]){
      callback(i);
    }
  }
}

