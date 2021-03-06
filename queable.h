#ifndef __QUEABLE_H_
#define __QUEABLE_H_

template <typename T>
class Queable {
private:
  Queable(const Queable&);
  Queable& operator =(const Queable&);
  
protected:
  T* prev;
  T* next;
  static T* first;
  
  bool enqueue() {
    if (pending()) return false;
    if (first) insert_before(first);
    else prev = next = first = static_cast<T*>(this);
    return true;
  }
  
  void dequeue() {
    if (next) {
      next->prev = prev;
      prev->next = next;
      if (first == static_cast<T*>(this)) first = next;
      if (first == static_cast<T*>(this)) first = 0;
      next = prev = 0;
    }
  }

  void insert_before(T* t) {
    prev = t->prev;
    next = t;
    prev->next = next->prev = static_cast<T*>(this);
  }
  
public:
  Queable() : prev(0), next(0) { }

  bool pending() { return next != 0; }
  bool at_head() { return first == static_cast<T*>(this); }

  static T& head() { return *first; }
  
  static bool empty() { return first == 0; }
  static bool any() { return first != 0; }
};

template <typename T> T* Queable<T>::first(0);

#endif
