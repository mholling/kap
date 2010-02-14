#ifndef __ADC_H_
#define __ADC_H_

#include "interrupt_driven.h"

class Analog : public InterruptDriven {
public:
  Analog();
    
  class Channel : public Item {
  protected:
    const unsigned int number;
    unsigned int data;

    void initiate();
    bool process();
    void terminate();
    
  public:
    Channel(unsigned int number) : number(number) { }
    
    inline void convert(bool block = false) volatile { wait(); enqueue(block); }    
    inline float operator ()() volatile { return static_cast<float>(data) / 1024; }
  };
  
  volatile Channel yaw;
  volatile Channel pitch;
  volatile Channel roll;
  volatile Channel ref;
};

#endif
