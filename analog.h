#ifndef __ADC_H_
#define __ADC_H_

#include "interrupt_driven.h"
#include "critical_section.h"

class Analog : public InterruptDriven {
public:
  Analog();
    
  class Channel : public Driven<Channel> {
    friend class Driven<Channel>;
    friend class Analog;
    
  protected:
    const unsigned int number;
    unsigned int data;

    void initiate();
    bool process();
    void terminate();
    
  public:
    Channel(unsigned int number) : number(number) { }
    
    void measure(bool block = false) { Driven<Channel>::operator ()(block); }    
    float operator ()() const { CriticalSection cs; return static_cast<float>(data) / 1024; }
  };
  
  void interrupt() { Channel::head().interrupt(); }
};

#endif
