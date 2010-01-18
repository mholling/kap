#ifndef __PWM_H_
#define __PWM_H_

class Pwm {
public:
  class Channel {
  private:
    volatile unsigned int& reg;
  public:
    Channel(volatile unsigned int& reg) : reg(reg) { }
    void set_duty_cycle(float value);
  };
  
  enum { frequency = 4000, resolution = F_CPU / frequency / 2 };
  
  Pwm();
  void init();
  
  Channel channel_a;
  Channel channel_b;
};

#endif
