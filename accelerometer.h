#ifndef __ACCELEROMETER_H_
#define __ACCELEROMETER_H_

#include "i2c.h"
#include "scheduler.h"
#include <math.h>

class Accelerometer {
private:
  enum { i2c_address = 0x1d, thresh_tap_reg = 0x1d, ofsx_reg, ofsy_reg, ofsz_reg, dur_reg, latent_reg, window_reg, thresh_act_reg, thresh_inact_reg, time_inact_reg, time_inact_ctl_reg, act_inact_ctl_reg, thresh_ff_reg, time_ff_reg, tap_exes_reg, act_tap_status_reg, bw_rate_reg, power_ctl_reg, int_enable_reg, int_map_reg, int_source_reg, data_format_reg, datax0_reg, datax1_reg, datay0_reg, datay1_reg, dataz0_reg, dataz1_reg, fifo_ctl_reg, fifo_status_reg };
  
public:
  Accelerometer() { }
  void init() { }

  class Task : public Scheduler::Task {
  public:
    Task() : Scheduler::Task(20) { }
    
    void run();
  } task;
};

#endif
