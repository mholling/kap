#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "shift_register.h"
#include "safe.h"
#include <avr/io.h>

class Motors {
  enum { standby_shift_register_pin = 1, yaw_polarity = +1, pitch_polarity = +1 };
  
public:
  Motors();
  inline void init() { disable(true); }
  
  volatile ShiftRegister::Bit disable;
  volatile ShiftRegister::Bit enable;
  
  class Motor {
  private:
    volatile unsigned int& pwm_reg;
    volatile unsigned char& in1_reg;
    const unsigned char in1_mask;
    volatile unsigned char& in2_reg;
    const unsigned char in2_mask;
    const int polarity;
    
  public:
    Motor(volatile unsigned int& pwm_reg, volatile unsigned char& in1_reg, unsigned char in1_pin, volatile unsigned char& in2_reg, unsigned char in2_pin, bool reversed) : pwm_reg(pwm_reg), in1_reg(in1_reg), in1_mask(_BV(in1_pin)), in2_reg(in2_reg), in2_mask(_BV(in2_pin)), polarity(reversed ? -1 : +1) { }
    
    void set(float value) volatile { Safe<Motor>(this)->set(value); }
    void set(float value);
  };
  
  volatile Motor yaw;
  volatile Motor pitch;
};

#endif
