#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "keyboardHardware.h"

#define EVENT_QUEUE_SIZE 32
#define OFF_COUNT_MAX 200

enum KeyboardEventType : byte {
  KEY_PRESS = 1,
  KEY_RELEASE = 0,
  EVENT_QUEUE_OVERFLOW = 111,  // The event queue was full upon attempting to 
                               // add an event. Events may have been missed since.
  EMPTY = 100  // Object represents an empty slot in queue
};

struct KeyboardEvent {
  KeyboardEventType type;
  byte key;
};

class Keyboard {
  public:
    Keyboard();
    void scanBank(byte bankNum);

    void forEachPressed(void(*callback)(int keyNum));

    bool hasEvent();
    KeyboardEvent popEvent();

  private:
    void pushEvent(KeyboardEventType type, byte key);

    byte _keys[NUM_BANKS*NUM_KEYS];
    byte _offCount[NUM_BANKS*NUM_KEYS];
    KeyboardEvent _eventQueue[EVENT_QUEUE_SIZE];
    int _eventQueuePosition = 0;
};

#endif
