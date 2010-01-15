#include "app.h"
#include "magnetometer.h"
#include "serial.h"

void Magnetometer::show_bearing() {
  App::app().serial.debug("bearing", vector.bearing());
  vector();
}

