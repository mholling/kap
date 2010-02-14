#include "diagnostic_task.h"
#include "app.h"

void DiagnosticTask::run() {
  // app.serial.debug(const_cast<Vector&>(app.accelerometer.measure.vector));
  // app.serial.debug(const_cast<Vector&>(app.accelerometer.measure.vector));    
  
  // app.serial.debug(const_cast<Vector&>(app.magnetometer.measure.vector));
  // app.serial.debug(const_cast<Vector&>(app.magnetometer.calibrate.bias));
  
  // app.serial.debug(app.gyros.yaw());
  // app.serial.debug(app.gyros.pitch());
  // app.serial.debug(app.gyros.roll());
  // app.serial.debug(app.analog.ref());
    
  app.serial.line();
}

