#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "keyboardHardware.h"

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
    #define KB_EVENT_QUEUE_SIZE 64
    KeyboardEvent _eventQueue[KB_EVENT_QUEUE_SIZE];
    int _eventQueuePosition = 0;
    void pushEvent(KeyboardEventType type, byte key);

    // This counter increases when the key is pressed and decreases
    // when released. When the threshold is reached, an event is generated
    int8_t _keyCounter[NUM_BANKS*NUM_KEYS];
    #define KB_KEY_COUNTER_MAX (int8_t) 100
    #define KB_KEY_COUNTER_THRESHOLD 3   // >= 0 and <= 126
};

#endif
