#ifndef __SINGLETON_H_
#define __SINGLETON_H_

template <typename T>
class Singleton : public T {
private:
  Singleton();
  Singleton(const Singleton&);
  Singleton& operator = (const Singleton&);
public:
  static T& instance() {
    static T singleton;
    return singleton;
  }
};

#endif
