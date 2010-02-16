#ifndef __QUEABLE_H_
#define __QUEABLE_H_

#include "safe.h"

template <typename T>
class Queable {
private:
  Queable& operator=(const Queable&);
  
protected:
  Queable* prev;
  Queable* next;
  static Queable* first;
  
  bool enqueue() {
    if (pending()) return false;
    if (first) insert_before(first);
    else prev = next = first = this;
    return true;
  }
  
  virtual void dequeue() {
    if (next) {
      next->prev = prev;
      prev->next = next;
      if (first == this) first = next;
      if (first == this) first = 0;
      next = prev = 0;
    }
  }

  void insert_before(Queable* t) {
    prev = t->prev;
    next = t;
    prev->next = next->prev = this;
  }
  
public:
  Queable() : prev(0), next(0) { }

  inline bool pending() { return next != 0; }
  inline bool at_head() { return first == this; }

  inline static T& head() { return *static_cast<T*>(first); } // TODO: static methods?

  inline static bool empty() { return first == 0; } // TODO: static methods?
  inline static bool any() { return first != 0; } // TODO: static methods?
};

template <typename T> Queable<T>* Queable<T>::first(0);

#endif
