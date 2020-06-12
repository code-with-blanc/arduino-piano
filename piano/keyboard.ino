#include "constants.h"
#include "keyboard.h"

#define NONE (unsigned char) 255

#define X 0
#define Y 1

Keyboard::Keyboard() {
  for(byte i = 0; i < sizeof(_keys); i++) {
    _keys[i] = 0;
  }
}

void Keyboard::setupPins() {
  pinMode(PIN_K0, INPUT);
  pinMode(PIN_K1, INPUT);
  pinMode(PIN_K2, INPUT);
  pinMode(PIN_K3, INPUT);
  pinMode(PIN_K4, INPUT);
  pinMode(PIN_K5, INPUT);

  pinMode(PIN_B0, OUTPUT);
  pinMode(PIN_MUX1, OUTPUT);
  pinMode(PIN_MUX2, OUTPUT);
  pinMode(PIN_MUX_C, OUTPUT);
  pinMode(PIN_MUX_AB, OUTPUT);
}

void Keyboard::forEachPressed(void(*callback)(int keyNum)) {
  for(int i = 0; i<sizeof(_keys); i++) {
    if(_keys[i]){
      callback(i);
    }
  }
}

byte Keyboard::getLastBank() {
  return _lastBank;
}

byte Keyboard::getLastKey() {
  return _lastKey;
}

int Keyboard::getNumKeysPressed() {
  int count = 0;
  for(int i = 0; i < NUM_BANKS*NUM_KEYS; i++) {
    count += (_keys[i] > 0);
  }
  return count;
}

void Keyboard::scanBank(byte bankNum) {
  setActiveBank(bankNum);
  delayMicroseconds(10);
  
  // scan keys
  for(int k = 0; k < NUM_KEYS; k++) {
    byte newValue = keyRead(k);
    bool oldValue = _keys[bankNum*NUM_KEYS + k];

    if(newValue != oldValue) {
      _keys[bankNum*NUM_KEYS + k] = newValue;
      _lastDetection = millis();
      _lastBank = bankNum;
      _lastKey = k;
    }
  }
}

void Keyboard::setActiveBank(byte bankNum) {
  //Clear bank outputs
  digitalWrite(PIN_B0, LOW);
  digitalWrite(PIN_MUX1, LOW);
  digitalWrite(PIN_MUX2, LOW);

  //setup mux
  switch(bankNum) {
    case 1:
    case 5:
      configMux(X, Y);
      break;
    case 2:
    case 6:
      configMux(X, X);
      break;
    case 3:
    case 7:
      configMux(Y, Y);
      break;
    case 4:
    case 8:
      configMux(Y, X);
      break;
  }

  //activate bank output
  if(bankNum == 0) {
    digitalWrite(PIN_B0, HIGH); 
  } else if(bankNum <= 4) {
    digitalWrite(PIN_MUX1, HIGH); 
  } else if(bankNum <= 8) {
    digitalWrite(PIN_MUX2, HIGH);
  }
}

void Keyboard::configMux(byte c_value, byte ab_value) {  
  if(ab_value == X) {
    digitalWrite(PIN_MUX_AB, LOW);
  } else {
    digitalWrite(PIN_MUX_AB, HIGH);
  }
  
  if(c_value == X) {
    digitalWrite(PIN_MUX_C, LOW);
  } else {
    digitalWrite(PIN_MUX_C, HIGH);
  }
}

byte Keyboard::keyRead(byte keyNum) {
  byte key = 255;
  switch(keyNum) {
    case 0: key = PIN_K0; break; 
    case 1: key = PIN_K1; break; 
    case 2: key = PIN_K2; break; 
    case 3: key = PIN_K3; break; 
    case 4: key = PIN_K4; break; 
    case 5: key = PIN_K5; break; 
  }

  if(key != 255) {
    return digitalRead(key);
  }

  return false;
}
