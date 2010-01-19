#ifndef __ADC_H_
#define __ADC_H_

#include "resource.h"
#include "queable.h"

class Analog : private Resource {
public:
  Analog(App* app);
  inline void init() { }
  const void interrupt();
  
  void start_conversions();
    
  class Channel : public Queable<Channel> {
  protected:
    const unsigned int channel;
    volatile unsigned int data;

    void dequeue();
    
    void start();
    
  public:
    Channel(unsigned int channel) : channel(channel) { }
    
    void interrupt();
    
    void enqueue();
    inline unsigned int operator ()() { return data; };
  };
  
  Channel yaw;
  Channel pitch;
  Channel roll;
  Channel ref;
};

#endif
