#ifndef __INTERRUPT_DRIVEN_H_
#define __INTERRUPT_DRIVEN_H_

#include "queable.h"
#include "critical_section.h"

class InterruptDriven {
public:
  InterruptDriven() { }
  void init() { }
  
  // TODO: better name than Item?
  template <typename T>
  class Item : protected Queable<T> {
    friend class Queable<T>; // TODO: needed?
    // friend class InterruptDriven;
    
  public:
    Item() { }
    
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
    void after_enqueue() { }
  
  private:
    bool enqueue() {
      CriticalSection cs;
      if (!Queable<T>::enqueue()) return false;
      static_cast<T&>(*this).after_enqueue();
      if (Queable<T>::at_head()) static_cast<T&>(*this).initiate();
      return true;
    }

    void dequeue() {
      Queable<T>::dequeue();
      // if (Queable<T>::any()) Queable<T>::head().initiate(); // TODO: which one?
      if (Queable<T>::any()) static_cast<T&>(*Queable<T>::first).initiate();
    }
  };
};

#endif
