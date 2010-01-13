#ifndef __APP_H_
#define __APP_H_

#include "singleton.h"
#include "serial.h"
#include "i2c.h"
#include "magnetometer.h"

void flash();
void long_flash();
void delay(int t);

class App {
private:
  App& operator=(const App&);
  
public:
  App() { sei(); }
  static App& app() { return Singleton<App>::instance(); }
  
  Serial serial;
  I2C i2c;
  Magnetometer magnetometer;
  
  void run();
};

#endif
