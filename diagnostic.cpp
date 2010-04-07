#include "diagnostic.h"
#include "timer.h"
#include "app.h"
#include "persistent.h"

void Diagnostic::run() {
  if (count++ % (Timer::frequency / 10) == 0) {

#define xxCOMPAREALL

#ifdef COMPAREALL
    app.serial.debug(app.attitude.measure.yaw());
    app.serial.debug(app.attitude.measure.pitch());
    app.serial.debug(app.attitude.measure.roll());
    app.serial.debug(app.attitude.estimate.yaw());
    app.serial.debug(app.attitude.estimate.pitch());
    app.serial.debug(app.attitude.estimate.roll());
#endif

#ifdef CALIBRATION
    app.serial.debug(app.magnetometer.measure.vector);
    app.serial.debug(app.magnetometer.calibrate.state.bias);
#endif
#ifdef VECTORS
    app.serial.debug(app.accelerometer.measure.vector.normalised());
    app.serial.debug(app.accelerometer.measure.vector.normalised().cross(app.magnetometer.calibrate.vector).normalised());
#endif

#ifdef ATTITUDE    
    app.serial.debug(app.attitude.measure.yaw(), 8);
    app.serial.debug(app.attitude.measure.pitch(), 8);
    app.serial.debug(app.attitude.measure.roll(), 8);
#endif
#ifdef RATES
    app.serial.debug(app.gyros.yaw.rate(), 8);
    app.serial.debug(app.gyros.pitch.rate(), 8);
    app.serial.debug(app.gyros.roll.rate(), 8);
#endif
#ifdef ESTIMATES
    app.serial.debug(app.gyros.yaw.estimate.angle());
    app.serial.debug(app.gyros.pitch.estimate.angle());
    app.serial.debug(app.gyros.roll.estimate.angle());
#endif

    app.serial.line();
  }
}

