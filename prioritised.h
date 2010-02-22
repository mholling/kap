#ifndef __PRIORITISED_H_
#define __PRIORITISED_H_

#include "queable.h"

template <typename T>
class Prioritised : public Queable<T> {
protected:
  bool enqueue() {
    if (this->pending ()) return false;
    if (T::empty()) this->prev = this->next = T::first = static_cast<T*>(this);
    else {
      T* t = T::first;
      for (; t->level >= level && t->next != T::first; t = t->next) ;
      if (t->level >= level) t = t->next;
      insert_before(t);
      if (T::first->level < level) T::first = static_cast<T*>(this);
    }
    return true;
  }
  
public:
  const unsigned int level;
  Prioritised(unsigned int level) : level(level) { }
};

#endif
