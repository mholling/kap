#include "app.h"
#include "magnetometer.h"
#include "serial.h"

void Magnetometer::show_bearing() {
  // App::app().serial.debug_b("status", vector_status.value());
  // App::app().serial.debug("      x", vector_status.x());
  // App::app().serial.debug("      y", vector_status.y());
  // App::app().serial.debug("      z", vector_status.z());
  App::app().serial.debug("bearing", vector_status.bearing());
  vector_status();
}

