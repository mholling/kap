#ifndef __ADC_H_
#define __ADC_H_

#include "queable.h"
#include "safe.h"

class Analog {
public:
  Analog();
  inline void init() volatile { }
    
  class Channel : public Queable<Channel> {
  protected:
    const unsigned int number;
    unsigned int data;

    void dequeue();
    void start();
    
  public:
    Channel(unsigned int number) : number(number) { }
    
    void interrupt();
    inline void convert() volatile { Safe<Channel>(this)().convert(); }
    void convert();
    
    inline float operator ()() volatile { return static_cast<float>(data) / 1024; }

    inline void wait() volatile { do { } while (next != 0); }
  };
  
  volatile Channel yaw;
  volatile Channel pitch;
  volatile Channel roll;
  volatile Channel ref;
};

#endif
