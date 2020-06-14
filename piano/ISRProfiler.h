#ifndef _ISRPROFILER_H_
#define _ISRPROFILER_H_

class ISRProfiler {
  public:
    ISRProfiler(unsigned long isr_millist_total);
    void printReport();
    void onISREnter();
    void onISRExit();

  private:
    unsigned long isr_micros_total;
    unsigned long last_isr_start;
    unsigned long isr_micros_used;
    unsigned long isr_micros_used_max;

    bool isr_ongoing;
    bool report_reentrance_error;
};

#endif
