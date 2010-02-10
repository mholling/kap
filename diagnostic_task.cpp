#include "diagnostic_task.h"
#include "app.h"

void DiagnosticTask::run() {
  app.serial.debug(app.magnetometer.measure.vector);
  app.serial.debug(app.magnetometer.calibrate.bias);
  app.serial.line();
}

