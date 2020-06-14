# Arduino Piano

Turning a defunct electronic keyboard into a MIDI controller

It is a hobby project but let's put it in github because reasons.

# Overview

The arduino is connected directly to the keys in an electronic piano.

An [ISR](https://en.wikipedia.org/wiki/Interrupt_handler) reads the pressed keys and stores the state in memory. Based on key changes (press, release) events are registered in an event queue.

The main loop polls the event queue and sends corresponding MIDI messages to the computer via the Arduino serial.

On the computer a software (e.g. the [Hairless MIDI Bridge](https://projectgus.github.io/hairless-midiserial/) ) presents the serial connection as a virtual MIDI device that can be connected to a soft synth or DAW.

## Code organization
```
/piano/piano.ino:
  - main file

KeyboardHardware class:
  - key reading and MUX manipulation
  - all hardware-specific code should be contained in this class

Keyboard class:
  - actual keyboard logic, like registering in memory 
    which keys are pressed and handling the event queue

ISRProfiler class:
  - Utility logic to debounce ISR timing and usage

The Arduino Media Library (https://github.com/FortySevenEffects/arduino_midi_library) is replicated in the project source because Arduino doesn't have a covenient package manage system. This allows the sketch to be run without installing additional dependencies in the Arduino IDE.
```

## Electric circuit

The keyboard keys are connected in banks. When one bank pin (pins BX, BY, ...) is energized, the pins K0 - K6 will present the electric levels corresponding to the key states (pressed, not pressed) in that bank.

![Key Banks](/images/key_banks_diagram.jpg)


There are 9 Bank Pins (B0-B9) and 6 key pins (K0-K5)

![Pinout](/images/ribbon_pinout.jpg)


The banks are energized one at a time. To ease the number of pins, two MUX'es (CD 4053B) are used to connect the bank pins to the arduino. The Key pins are directly connected.

![Circuit](/images/circuit_protoboard.jpg)
