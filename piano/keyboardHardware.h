#ifndef _KEYBOARD_HARDWARE_H_
#define _KEYBOARD_HARDWARE_H_

#define NUM_BANKS 9
#define NUM_KEYS 6

class KeyboardHardware {
  public:
    static void setupPins();
    static void setActiveBank(byte bankNum);
    static byte readKey(byte keyNum);

  private:
    static void configMux(byte c_val, byte ab_val);
};

#endif