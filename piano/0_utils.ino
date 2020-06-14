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

byte keyToMidiNote(byte keyNum) {
  const byte middleCKey = 24;
  const byte middleCMidi = 60;
  return keyNum + (middleCMidi - middleCKey);
}
