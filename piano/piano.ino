#include "pins.h"
#include "keyboard.h"
#include "ISRProfiler.h"

#include "arduino_midi_library-5.0.2/src/MIDI.h"

#define MIDI_CH 1
#define MIDI_VEL 127

//// Fn declarations
void setupISR();

//// Global scope
MIDI_CREATE_DEFAULT_INSTANCE();
Keyboard keyboard;

const float isr_millis_total = 0.2;
ISRProfiler isrProfiler(isr_millis_total);

////
void setup() {
  KeyboardHardware::setupPins();
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  setupISR();

  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
}

int isrCount = 0;
ISR(TIMER1_COMPA_vect) {
  isrProfiler.onISREnter();

  keyboard.scanBank(isrCount % NUM_BANKS);
  isrCount++;

  isrProfiler.onISRExit();
}

void loop() {
  while(keyboard.hasEvent()) {
    const KeyboardEvent event = keyboard.popEvent();
    if (event.type == KEY_PRESS) {
      MIDI.sendNoteOn(keyToMidiNote(event.key), MIDI_VEL, MIDI_CH);
    } else if(event.type == KEY_RELEASE) {
      MIDI.sendNoteOff(keyToMidiNote(event.key), MIDI_VEL, MIDI_CH);
    }

    delayMicroseconds(50);
  }

  // isrProfiler.printReport();
}

void setupISR() {
  cli(); //stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0; //initialize counter value to 0
  // set compare match register
  // Formula is:
  // OCR1A = (T_int [ms] * 1/1000 * (16*10^6)/(1024)) - 1      [must be <65536]
  OCR1A = (isr_millis_total * 15.625) - 1;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei(); //allow interrupts
}
