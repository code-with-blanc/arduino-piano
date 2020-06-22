#include "pins.h"
#include "keyboard.h"

Keyboard::Keyboard() {
  for(byte i = 0; i < sizeof(_keyCounter); i++) {
    _keyCounter[i] = -KB_KEY_COUNTER_MAX;
  }
  for(int i = 0; i < KB_EVENT_QUEUE_SIZE; i++) {
    _eventQueue[i].type = EMPTY;
  }
}

void Keyboard::scanBank(byte bankNum) {
  KeyboardHardware::setActiveBank(bankNum);
  delayMicroseconds(10);
  
  // scan keys  
  for(int k = 0; k < NUM_KEYS; k++) {
    const int key = bankNum*NUM_KEYS + k;
    byte newValue = KeyboardHardware::readKeyPin(k);
    byte wasPressed = _keyCounter[key] >= 0;

    if(newValue != wasPressed) {
      // key value change, reset count value
      if(newValue) {
        _keyCounter[key] = 0;
      } else {
        _keyCounter[key] = -1;
      }
    } else {
      // key in same state as last reading

      // increase counting
      if(wasPressed) {
        _keyCounter[key]++;
        if(_keyCounter[key] > KB_KEY_COUNTER_MAX) {
          _keyCounter[key] = KB_KEY_COUNTER_MAX;
        }
      } else {
        _keyCounter[key]--;
        if(_keyCounter[key] < -KB_KEY_COUNTER_MAX) {
          _keyCounter[key] = -KB_KEY_COUNTER_MAX;
        }
      }

      // register event if passed debouncing threshold
      if(_keyCounter[key] == KB_KEY_COUNTER_THRESHOLD) {
        pushEvent(KEY_PRESS, key);
      }
      if(_keyCounter[key] == -KB_KEY_COUNTER_THRESHOLD) {
        pushEvent(KEY_RELEASE, key);
      }
    }
  }
}

bool Keyboard::hasEvent() {
  return (_eventQueuePosition > 0);
}

KeyboardEvent Keyboard::popEvent() {
  _eventQueuePosition--;
  if(_eventQueuePosition < -1) {
    _eventQueuePosition = -1;
  }

  if(_eventQueuePosition < 0) {
    KeyboardEvent empty;
    empty.type = EMPTY;
    return empty;
  }

  return _eventQueue[_eventQueuePosition + 1];
}

void Keyboard::pushEvent(KeyboardEventType type, byte key) {
  _eventQueuePosition++;
  if(_eventQueuePosition == KB_EVENT_QUEUE_SIZE-1) {
    _eventQueue[_eventQueuePosition].type = EVENT_QUEUE_OVERFLOW;
    _eventQueuePosition--;
    return;
  }

  _eventQueue[_eventQueuePosition].type = type;
  _eventQueue[_eventQueuePosition].key = key;
}

