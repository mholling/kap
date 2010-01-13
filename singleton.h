#ifndef __SINGLETON_H_
#define __SINGLETON_H_

template <typename T>
class Singleton : public T {
private:
  static T* t;
  Singleton();
  Singleton(const Singleton<T>&);
  Singleton<T>& operator = (const Singleton<T>&);
public:
  static T& instance() {
    if (t) return *t;
    static T singleton;
    t = &singleton;
    return singleton;
  }
};

template <typename T> T* Singleton<T>::t(0);

#endif
