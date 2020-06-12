bool getBit(byte reg, byte bytePos) {
  return (reg & (1 << bytePos)) > 0;
}

byte setBit(byte reg, byte bytePos, byte value) {
  const byte mask = (1 << bytePos);

  if(value)
    return reg | mask;
  else
    return reg & ~mask;
}

byte countBits(byte reg) {
  int count = 0;
  byte mask = 1;
  for(int i = 0; i < 8; i++) {
    count += ((reg & mask) > 0);
    mask = mask << 1;
  }

  return count;
}

byte keyToMidiNote(byte keyNum) {
  const byte middleCKey = 24;
  const byte middleCMidi = 60;
  return keyNum + (middleCMidi - middleCKey);
}

char bankToChar(byte bankNum) {
  if(bankNum > 10) {
    return '_';
  }

  return '0' + bankNum % 10;
}

char keyToChar(byte keyNum) {
  if(keyNum > 6) {
    return '_';
  }

  return 'A' + keyNum % 6;
}
