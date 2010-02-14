#ifndef __INTERRUPT_DRIVEN_H_
#define __INTERRUPT_DRIVEN_H_

#include "queable.h"

class InterruptDriven {
public:
  InterruptDriven() { }
  inline void init() volatile { }
  
  class Item : public Queable<Item> {
  public:
    Item() { }
    void enqueue(bool block = false) volatile;
    void interrupt();
    
    inline void wait() volatile { do { } while (next); }
    
  protected:
    virtual void initiate()  = 0;
    virtual bool process()   = 0;  // return true if interrupt sequence for the item is complete
    virtual void terminate() = 0;
  
  private:
    bool enqueue();
    void dequeue();
  };
};

#endif
