#include "pins.h"
#include "keyboard.h"

Keyboard::Keyboard() {
  for(byte i = 0; i < sizeof(_keys); i++) {
    _keys[i] = 0;
    _offCount[i] = OFF_COUNT_MAX;
  }
  for(int i = 0; i < EVENT_QUEUE_SIZE; i++) {
    _eventQueue[i].type = EMPTY;
  }
}

void Keyboard::scanBank(byte bankNum) {
  KeyboardHardware::setActiveBank(bankNum);
  delayMicroseconds(10);
  
  // scan keys
  for(int k = 0; k < NUM_KEYS; k++) {
    byte newValue = KeyboardHardware::readKeyPin(k);
    const int key = bankNum*NUM_KEYS + k;
    bool oldValue = _keys[key];

    if(newValue) {
      _offCount[key] = 0;

      if(newValue != oldValue) {
        _keys[key] = newValue;
        pushEvent(KEY_PRESS, key);
      }
    }

    if(!newValue) {
      _offCount[key]++;
      if(_offCount[key] > OFF_COUNT_MAX)
        _offCount[key] = OFF_COUNT_MAX;

      // Regitster KEY_RELEASE after some off readings to prevent bouncing
      if(_offCount[key] == 10) {
        _keys[key] = newValue;
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
  if(_eventQueuePosition == EVENT_QUEUE_SIZE-1) {
    _eventQueue[_eventQueuePosition].type = EVENT_QUEUE_OVERFLOW;
    _eventQueuePosition--;
    return;
  }

  _eventQueue[_eventQueuePosition].type = type;
  _eventQueue[_eventQueuePosition].key = key;
}

