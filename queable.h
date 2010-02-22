#ifndef __QUEABLE_H_
#define __QUEABLE_H_

template <typename T>
class Queable {
private:
  Queable(const Queable&);
  Queable& operator =(const Queable&);
  
protected:
  Queable* prev; // TODO: make these private members and define inline pre-casted next(), etc.?
  Queable* next;
  static Queable* first;
  
  bool enqueue() {
    if (pending()) return false;
    if (first) insert_before(first);
    else prev = next = first = this;
    return true;
  }
  
  void dequeue() {
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

  bool pending() { return next != 0; }
  bool at_head() { return first == this; }

  static T& head() { return *static_cast<T*>(first); }

  static bool empty() { return first == 0; }
  static bool any() { return first != 0; }
};

template <typename T> Queable<T>* Queable<T>::first(0);

#endif
