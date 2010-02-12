#ifndef __PRIORITISED_QUEABLE_H_
#define __PRIORITISED_QUEABLE_H_

#include "queable.h"

template <typename T>
class PrioritisedQueable : public Queable<T> {
protected:
  bool enqueue() {
    if (this->pending()) return false;
    if (this->empty())
      this->prev = this->next = this->first = this;
    else {
      PrioritisedQueable* t = static_cast<PrioritisedQueable*>(this->first);
      for (; t->level >= level && t->next != this->first; t = static_cast<PrioritisedQueable*>(t->next)) ;
      if (t->level >= level) t = static_cast<PrioritisedQueable*>(t->next);
      this->insert_before(t);
      if (static_cast<PrioritisedQueable*>(this->first)->level < level) this->first = this;
    }
    return true;
  }
  
public:
  const unsigned int level;
  PrioritisedQueable(unsigned int level) : level(level) { }
};

#endif
