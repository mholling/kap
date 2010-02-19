#include "diagnostic_task.h"
#include "app.h"

void DiagnosticTask::run() {
  app.serial.debug(app.accelerometer.measure.vector);
  app.serial.debug(app.accelerometer.measure.vector);    
  
  app.serial.debug(app.magnetometer.measure.vector);
  app.serial.debug(app.magnetometer.calibrate.state.bias);
  
  app.serial.debug(app.gyros.yaw());
  app.serial.debug(app.gyros.pitch());
  app.serial.debug(app.gyros.roll());
    
  app.serial.line();
}

