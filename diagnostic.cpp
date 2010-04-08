#include "diagnostic.h"
#include "timer.h"
#include "app.h"
#include "persistent.h"

void Diagnostic::run() {
  if (count++ % (Timer::frequency / 10) == 0) {
    app.serial.debug(static_cast<int>(app.attitude.estimate.yaw()   * 1800.0 / M_PI));
    app.serial.debug(static_cast<int>(app.attitude.estimate.pitch() * 1800.0 / M_PI));
    app.serial.debug(static_cast<int>(app.attitude.estimate.roll()  * 1800.0 / M_PI));
    app.serial.line();
  }
}

