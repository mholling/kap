#include "app.h"
#include "magnetometer.h"
#include "serial.h"

void Magnetometer::show_bearing() {
  if (vector_status.pending())
    app.serial.string("no go!\r\n");
  else {
    app.serial.debug_b("status", vector_status.status());
    // app.serial.debug("      x", vector_status.x());
    // app.serial.debug("      y", vector_status.y());
    // app.serial.debug("      z", vector_status.z());
    app.serial.debug("bearing", vector_status.bearing());
  }
  vector_status();
}

