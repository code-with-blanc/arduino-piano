#include "pins.h"
#include "keyboard.h"

#include "arduino_midi_library-5.0.2/src/MIDI.h"

MIDI_CREATE_DEFAULT_INSTANCE();

float isr_millis_total = 0.5;
unsigned long isr_micros_total = isr_millis_total * 1000L;
unsigned long isr_micros_used = 0;

Keyboard keyboard;

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
  unsigned long ms_start = micros();

  keyboard.scanBank(isrCount % NUM_BANKS);
  
  isrCount++;

  isr_micros_used = micros() - ms_start;
}

void sendMidi(int keyNum) {
  MIDI.sendNoteOn(keyToMidiNote(keyNum), 127, 1);
}

void loop() {
  while(keyboard.hasEvent()) {
    const KeyboardEvent event = keyboard.popEvent();
    Serial.print(millis());
    Serial.print(": key ");
    Serial.print(event.key);
    Serial.print("   type ");
    Serial.print(event.type);
    Serial.print("\n");
  }

  // reportISRTiming();
  delay(5);
}

void reportISRTiming() {
  // serial implementation
  int pctg = 100.0 * ((float) isr_micros_used)/((float) isr_micros_total);
  Serial.print(millis());
  Serial.print(": ISR usage  ");
  Serial.print(pctg);
  Serial.print("%\n");
  
//  // LED implementation
//  int t = 200;
//  int t_on = 1000L * isr_micros_used/isr_micros_total;
//  int t_off = t - t_on;
//  
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(t_on);
//  digitalWrite(LED_BUILTIN, LOW);
//  delay(t_off);
}

