#include "calibrate_task.h"
#include "app.h"
#include "safe.h"

// TODO: have the settings persist to EEPROM.

CalibrateTask::CalibrateTask(const Vector& measured, float variance, float lambda) : Scheduler::Task(20), measured(measured), lambda(lambda) {
  P0[0] = P1[1] = P2[2] = P3[3] = 400.0 * variance;
  P1[0] = P2[0] = P2[1] = P3[0] = P3[1] = P3[2] = 0.0;
  w3 = h2 = b2 = 0.0;
}

void CalibrateTask::run() volatile {
  app.magnetometer.measure.wait(); // TODO: no good, needs to be independent of measured vector source!
  
  Vector& _measured = const_cast<Vector&>(measured);
  Vector& _bias     = const_cast<Vector&>(bias);
  Vector& _previous = const_cast<Vector&>(previous);
  
  if (b2 > 0.5 * h2 || (_previous - _measured).sqabs() > 0.4 * h2) { // TODO: adjust these fractions...
    _previous = _measured;
      
    float Px[4];
    
    Px[0] = 2.0 * (P0[0] * _measured[0] + P1[0] * _measured[1] + P2[0] * _measured[2]) + P3[0];
    Px[1] = 2.0 * (P1[0] * _measured[0] + P1[1] * _measured[1] + P2[1] * _measured[2]) + P3[1];
    Px[2] = 2.0 * (P2[0] * _measured[0] + P2[1] * _measured[1] + P2[2] * _measured[2]) + P3[2];
    Px[3] = 2.0 * (P3[0] * _measured[0] + P3[1] * _measured[1] + P3[2] * _measured[2]) + P3[3];
    
    float d = lambda + 2.0 * (_measured[0] * Px[0] + _measured[1] * Px[1] + _measured[2] * Px[2]) + Px[3];
      
    float k[4];
    for (int n = 0; n < 4; n++) k[n] = Px[n] / d;
    
    float e = _measured.sqabs() - 2.0 * _measured.dot(_bias) - w3;
    
    for (int n = 0; n < 3; n++) _bias[n] += k[n] * e;
    w3 += k[3] * e;
    
    for (int n = 0; n < 4; n++) P3[n] = (P3[n] - Px[3] * k[n]) / lambda;
    for (int n = 0; n < 3; n++) P2[n] = (P2[n] - Px[2] * k[n]) / lambda;
    for (int n = 0; n < 2; n++) P1[n] = (P1[n] - Px[1] * k[n]) / lambda;
    for (int n = 0; n < 1; n++) P0[n] = (P0[n] - Px[0] * k[n]) / lambda;
    
    b2 = _bias.sqabs();
    h2 = w3 + b2;
  }
  
  const_cast<Vector&>(vector) = _measured - _bias;
}
