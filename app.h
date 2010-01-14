#ifndef __APP_H_
#define __APP_H_

#include "singleton.h"
#include "scheduler.h"
#include "serial.h"
#include "i2c.h"
#include "magnetometer.h"

class App {
  friend class Singleton<App>;
  
private:
  App& operator=(const App&);
  
protected:
  App() { sei(); }
  
public:
  static App& app() { return Singleton<App>::instance(); }
  
  Scheduler scheduler;
  Serial serial;
  I2C i2c;
  Magnetometer magnetometer;
  
  void run();
};




void flash();
void long_flash();
void delay(int t);

extern "C" void __cxa_pure_virtual(void);
extern "C" void atexit(void);

#endif
