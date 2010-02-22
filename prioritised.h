#ifndef __PRIORITISED_H_
#define __PRIORITISED_H_

template <typename T>
class Prioritised : public T {
protected:
  bool enqueue() {
    if (T::pending ()) return false;
    if (T::empty())
      T::prev = T::next = T::first = this;
    else {
      Prioritised* t = static_cast<Prioritised*>(T::first);
      for (; t->level >= level && t->next != T::first; t = static_cast<Prioritised*>(t->next)) ;
      if (t->level >= level) t = static_cast<Prioritised*>(t->next);
      insert_before(t);
      if (static_cast<Prioritised*>(T::first)->level < level) T::first = this;
    }
    return true;
  }
  
public:
  const unsigned int level;
  Prioritised(unsigned int level) : level(level) { }
};

#endif
