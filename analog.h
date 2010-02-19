#ifndef __ADC_H_
#define __ADC_H_

#include "interrupt_driven.h"
#include "critical_section.h"

class Analog : public InterruptDriven<Analog> {
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
    
    void convert(bool block = false) { Item::operator ()(block); }    
    float operator ()() const { CriticalSection cs; return static_cast<float>(data) / 1024; }
  };
};

#endif
