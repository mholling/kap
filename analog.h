#ifndef __ADC_H_
#define __ADC_H_

#include "queable.h"

class Analog {
public:
  Analog();
  inline void init() { }
  const void interrupt();
  
  void start_conversions();
    
  class Channel : protected Queable<Channel> {
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

    inline bool pending() { return Queable<Channel>::pending(); }; // TODO: add critical section here and in corresponding others?
    inline static Channel& head() { return Queable<Channel>::head(); };
  };
  
  Channel yaw;
  Channel pitch;
  Channel roll;
  Channel ref;
};

#endif
