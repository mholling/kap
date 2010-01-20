#include "app.h"
#include "magnetometer.h"
#include "serial.h"

void Magnetometer::Task::run() {
  app.magnetometer.get_vector_status();
  for (bool done = false; done; done = !app.magnetometer.get_vector_status.pending()) { }
  // app.serial.debug_b(" status", app.magnetometer.get_vector_status.status());
  // app.serial.debug("      x", vector_status.x());
  // app.serial.debug("      y", vector_status.y());
  // app.serial.debug("      z", vector_status.z());
  app.serial.debug("bearing", app.magnetometer.get_vector_status.bearing());
}
