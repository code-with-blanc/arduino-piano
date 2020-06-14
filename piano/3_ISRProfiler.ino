#include "ISRProfiler.h"

ISRProfiler::ISRProfiler(unsigned long isr_millist_total) {
  isr_micros_total = isr_millis_total * 1000L;
  last_isr_start = 0;

  isr_micros_used = 0;
  isr_micros_used_max = 0;

  isr_ongoing = false;
  report_reentrance_error = false;
}

void ISRProfiler::printReport() {
  const bool report_reentrance = this->report_reentrance_error; 
  this->report_reentrance_error = false;

  short pctg_last = 100.0 * ((float) isr_micros_used)/((float) isr_micros_total);
  short pctg_max  = 100.0 * ((float) isr_micros_used_max)/((float) isr_micros_total);
  isr_micros_used_max = 0;

  Serial.print("[");
  Serial.print(millis());
  Serial.print("][ISR usage] Last: ");
  Serial.print(pctg_last);
  Serial.print("%   Max: ");
  Serial.print(pctg_max);
  Serial.print("%");

  if(report_reentrance) {
    Serial.print("  [ISR reentered before completing]");
  }

  Serial.print("\n");
}

void ISRProfiler::onISREnter() {
  last_isr_start = micros();

  if(isr_ongoing) {
    report_reentrance_error = true;
  }

  isr_ongoing = true;
}

void ISRProfiler::onISRExit() {
  isr_micros_used = micros() - last_isr_start;
  if(isr_micros_used > isr_micros_used_max) {
    isr_micros_used_max = isr_micros_used;
  }

  isr_ongoing = false;
}
