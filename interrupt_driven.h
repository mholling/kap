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
    
    void wait() const { do { } while (const_cast<volatile Queable<T>*>(Queable<T>::next)); }
    
  protected:
    virtual void after_enqueue() { }
    virtual void before_dequeue() { }
  
  private:
    bool enqueue() {
      CriticalSection cs;
      if (!Queable<T>::enqueue()) return false;
      after_enqueue();
      if (Queable<T>::at_head()) static_cast<T&>(*this).initiate();
      return true;
    }

    void dequeue() {
      before_dequeue();
      Queable<T>::dequeue();
      if (Queable<T>::any()) Queable<T>::head().initiate();
    }
  };
};

#endif
