#ifndef __PRIORITISED_H_
#define __PRIORITISED_H_

#include "queable.h"

template <typename T>
class Prioritised : public Queable<T> {
protected:
  bool enqueue() {
    if (Queable<T>::pending ()) return false;
    if (Queable<T>::empty()) Queable<T>::enqueue();
    else {
      T* t = T::first;
      for (; static_cast<Prioritised*>(t)->level >= level && t->next != T::first; t = t->next) ;
      if (t->level >= level) t = t->next;
      insert_before(t);
      if (static_cast<Prioritised*>(T::first)->level < level) T::first = static_cast<T*>(this);

      // Prioritised* t = static_cast<Prioritised*>(T::first);
      // for (; t->level >= level && t->next != T::first; t = static_cast<Prioritised*>(t->next)) ;
      // if (t->level >= level) t = static_cast<Prioritised*>(t->next);
      // insert_before(static_cast<T*>(t));
      // if (static_cast<Prioritised*>(T::first)->level < level) T::first = static_cast<T*>(this);
    }
    return true;
  }
  
public:
  const unsigned int level;
  Prioritised(unsigned int level) : level(level) { }
};

#endif
