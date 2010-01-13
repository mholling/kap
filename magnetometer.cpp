#include "app.h"
#include "magnetometer.h"
#include "serial.h"

void Magnetometer::show_bearing() {
  while (vector.pending()) ;
  vector.read();
  while (vector.pending()) ;
  App::app().serial.debug("bearing", vector.bearing());
}

void Magnetometer::show_status() {
  while (status.pending()) ;
  status.read();
  while (status.pending()) ;
  App::app().serial.debug_b("status", status.value);
}





