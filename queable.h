#ifndef __QUEABLE_H_
#define __QUEABLE_H_

#include "critical_section.h"

template <typename T>
class Queable {
private:
  Queable& operator=(const Queable&);
  
protected:
  Queable* prev;
  Queable* next;
  static Queable* first; // TODO: these should probably all be volatile?
  
  void insert_before(Queable* t) {
    prev = t->prev;
    next = t;
    prev->next = next->prev = this;
  }
  
public:
  Queable() : prev(0), next(0) { }
  
  void enqueue() {
    CriticalSection cs; // TODO: remove these and put them in I2C::Packet methods instead ??
    if (pending()) return;
    if (first) insert_before(first);
    else prev = next = first = this;
  }
  
  void dequeue() {
    CriticalSection cs; // TODO: remove these and put them in I2C::Packet methods instead ??
    // TODO: prevent dequeing of head item?
    if (next) {
      next->prev = prev;
      prev->next = next;
      if (first == this) first = next;
      if (first == this) first = 0;
      next = prev = 0;
    }
  }

  inline bool pending() { return next != 0; }
  inline bool at_head() { return first == this; }

  inline static T& head() { return *static_cast<T*>(first); }

  static bool empty() { return first == 0; }
  static bool any() { return first != 0; }
  static bool just_one() { return first && first->next == first; }
};

template <typename T> Queable<T>* Queable<T>::first(0);

#endif
