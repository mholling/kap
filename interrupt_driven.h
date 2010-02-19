#ifndef __INTERRUPT_DRIVEN_H_
#define __INTERRUPT_DRIVEN_H_

#include "queable.h"
#include "critical_section.h"

template <typename T>
class InterruptDriven {
public:
  InterruptDriven() { }
  inline void init() { }
  
  class Item : protected Queable<Item> {
  public:
    Item() { }
    
    bool operator ()(bool block = false) {
      if (!enqueue()) return false;
      if (block) wait();
      return true;
    }

    void interrupt() {
      if (process()) {
        terminate();
        dequeue();
      }
    }
    
    void wait() const { do { } while (const_cast<volatile Queable<Item>*>(Queable<Item>::next)); }
    
  protected:
    virtual void initiate()  = 0;
    virtual bool process()   = 0;  // return true if interrupt sequence for the item is complete
    virtual void terminate() = 0;
    virtual void after_enqueue() { }
  
  private:
    bool enqueue() {
      CriticalSection cs;
      if (!Queable<Item>::enqueue()) return false;
      after_enqueue();
      if (Queable<Item>::at_head()) initiate();
      return true;
    }

    void dequeue() {
      Queable<Item>::dequeue();
      if (Queable<Item>::any()) Queable<Item>::head().initiate();
    }
  };
  
  void interrupt() {
    Queable<Item>::head().interrupt();
  }
};

#endif
