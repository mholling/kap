#include "diagnostic.h"
#include "timer.h"
#include "app.h"
#include "persistent.h"

void Diagnostic::run() {
  
  // if (dt > static_cast<float>(Timer::ocr2a + 1) * 1024 / F_CPU)
  //   app.serial.send("!!!");
    

  if (count++ % (Timer::frequency / 45) == 0) {
    // app.serial.debug(static_cast<int>(app.attitude.estimate.yaw()   * 1800.0 / M_PI));
    // app.serial.debug(static_cast<int>(app.attitude.estimate.pitch() * 1800.0 / M_PI));
    // app.serial.debug(static_cast<int>(app.attitude.estimate.roll()  * 1800.0 / M_PI));
    // app.serial.line();
  }
}

