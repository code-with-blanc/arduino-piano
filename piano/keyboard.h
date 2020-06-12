#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define NUM_BANKS 9
#define NUM_KEYS 6

class Keyboard {
  private:
    byte _keys[NUM_BANKS*NUM_KEYS];
    byte _lastBank = 255;
    byte _lastKey = 255;
    unsigned long _lastDetection = 0;

    void setActiveBank(byte bankNum);
    void configMux(byte c_val, byte ab_val);
    byte keyRead(byte keyNum);

  public:
    Keyboard();
    static void setupPins();
    void scanBank(byte bankNum);
    byte getLastBank();
    byte getLastKey();
    int getNumKeysPressed();
    void forEachPressed(void(*)(int keyNum));
};

#endif
