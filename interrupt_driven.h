#ifndef __INTERRUPT_DRIVEN_H_
#define __INTERRUPT_DRIVEN_H_

#include "queable.h"
#include "critical_section.h"

class InterruptDriven {
public:
  InterruptDriven() { }
  void init() { }
  
  template <typename T>
  class Driven : protected Queable<T> {
    typedef Queable<T> Base;
    
  public:
    Driven() { }
    
    bool operator ()(bool block = false) {
      if (!enqueue()) return false;
      if (block) wait();
      return true;
    }

    void interrupt() {
      if (static_cast<T&>(*this).process()) {
        static_cast<T&>(*this).terminate();
        dequeue();
      }
    }
    
    // void wait() const { for (volatile bool waiting = true; waiting; waiting = const_cast<T * volatile>(Base::next) != 0) ; }
    void wait() const { const_cast<volatile Driven&>(*this).wait(); }
    void wait() const volatile { while (Base::next != 0) ; } 
    
    
  protected:
    virtual void after_enqueue() { }
    virtual void before_dequeue() { }
  
  private:
    bool enqueue() {
      CriticalSection cs;
      if (!Base::enqueue()) return false;
      after_enqueue();
      if (Base::at_head()) static_cast<T&>(*this).initiate();
      return true;
    }

    void dequeue() {
      before_dequeue();
      Base::dequeue();
      if (Base::any()) Base::head().initiate();
    }
  };
};

#endif
