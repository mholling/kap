#ifndef __MOTOR_H_
#define __MOTOR_H_

#include <avr/io.h>

class Motors {
  enum { standby_shift_register_bit = 1, yaw_polarity = +1, pitch_polarity = +1 };
  
public:
  Motors();
  inline void init() volatile { disable(); }
  
  void disable() volatile;
  void enable() volatile;
  
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
    void set(float value);
  };
  
  Motor yaw;
  Motor pitch;
};

#endif
