#ifndef __INTERRUPT_DRIVEN_H_
#define __INTERRUPT_DRIVEN_H_

#include "queable.h"

class InterruptDriven {
public:
  InterruptDriven() { }
  inline void init() { }
  
  class Item : public Queable<Item> {
  public:
    Item() { }
    void interrupt();
    
    inline void wait() const { do { } while (const_cast<volatile Queable<Item>*>(next)); }
    
  protected:
    bool enqueue(bool block = false);

    virtual void initiate()  = 0;
    virtual bool process()   = 0;  // return true if interrupt sequence for the item is complete
    virtual void terminate() = 0;
    virtual void after_enqueue() { }
  
  private:
    void dequeue(); // TODO: needed?
  };
  
  void interrupt();
};

#endif
