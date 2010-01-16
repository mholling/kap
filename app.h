#ifndef __APP_H_
#define __APP_H_

#include "singleton.h"
#include "scheduler.h"
#include "serial.h"
#include "i2c.h"
#include "magnetometer.h"
#include "timer.h"
#include "analog.h"
#include "pwm.h"

class App {
  friend class Singleton<App>;
  
private:
  App& operator=(const App&);
  
protected:
  App() { }
  
public:
  inline static App& app() { return Singleton<App>::instance(); }
  
  Scheduler scheduler;
  Serial serial;
  I2C i2c;
  Magnetometer magnetometer;
  Timer timer;
  Analog analog;
  Pwm pwm;
  
  void run();
};




void toggle_led();
// void delay(int t);

extern "C" void __cxa_pure_virtual(void);
extern "C" void atexit(void);

#endif
