# Arduino Piano

Turning a defunct electronic keyboard into a MIDI controller

It is a hobby project but let's put it in github because reasons.

# Overview

The arduino is connected directly to the keys in an electronic piano.

The code in an ISR reads the pressed keys and stores the state in memory. The main loop polls the registered key states and sends it as MIDI messages to the computer via the serial connection. On the computer a software (e.g. the [Hairless MIDI Bridge](https://projectgus.github.io/hairless-midiserial/) ) translates the serial messages to a MIDI device.

## Code organization
```
/piano/piano.ino:
  - main file

KeyboardHardware class:
  - key reading and MUX manipulation
  - all hardware-specific code should be 
    contained in this class

Keyboard class:
  - actual keyboard logic, like registering in memory 
    which keys are pressed
```

## Electric circuit

The keyboard keys are connected in banks. When one bank pin (pins BX, BY, ...) is energized, the pins K0 - K6 will present the electric levels corresponding to the key states (pressed, not pressed) in that bank.

![Key Banks](/images/key_banks_diagram.jpg)


There are 9 Bank Pins (B0-B9) and 6 key pins (K0-K5)

![Pinout](/images/ribbon_pinout.jpg)


The banks are energized one at a time. To ease the number of pins, two MUX'es (CD 4053B) are used to connect the bank pins to the arduino. The Key pins are directly connected.

![Circuit](/images/circuit_protoboard.jpg)
