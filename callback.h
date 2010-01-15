// #ifndef __CALLBACK_H_
// #define __CALLBACK_H
// 
// class CallbackBase {
// public:
//   virtual void operator()() const { }
//   virtual ~CallbackBase() = 0;
// };
// 
// CallbackBase::~CallbackBase() { }
// 
// template <typename T>
// class Callback : public CallbackBase {
// public:
//   typedef void (T::*F)();
//   Callback(T* t, F f) : t(t), f(f) { }
//   void operator()() const { (t->*f)(); }
// private:
//   T* t;
//   F  f;
// };
// 
// #endif
