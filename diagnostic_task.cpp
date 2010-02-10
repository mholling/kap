#include "diagnostic_task.h"
#include "app.h"

void DiagnosticTask::run() {
  app.serial.debug(app.magnetometer.vector + app.magnetometer.calibrate.bias);
  app.serial.debug(app.magnetometer.calibrate.bias);
  app.serial.line();
  app.serial.line();
}

