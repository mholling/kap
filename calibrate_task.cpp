#include "calibrate_task.h"

void CalibrateTask::State::defaults() {
  P0[0] = P1[1] = P2[2] = P3[3] = 5000000000.0f; // TODO: value?
  P1[0] = P2[0] = P2[1] = P3[0] = P3[1] = P3[2] = w3 = h2 = b2 = bias[0] = bias[1] = bias[2] = 0.0;
}

void CalibrateTask::run() {
  if (state.b2 > 0.5 * state.h2 || (previous - measured).sqabs() > 0.4 * state.h2) { // TODO: adjust these fractions...
    previous = measured;
      
    float Px[4];
    
    Px[0] = 2.0 * (state.P0[0] * measured[0] + state.P1[0] * measured[1] + state.P2[0] * measured[2]) + state.P3[0];
    Px[1] = 2.0 * (state.P1[0] * measured[0] + state.P1[1] * measured[1] + state.P2[1] * measured[2]) + state.P3[1];
    Px[2] = 2.0 * (state.P2[0] * measured[0] + state.P2[1] * measured[1] + state.P2[2] * measured[2]) + state.P3[2];
    Px[3] = 2.0 * (state.P3[0] * measured[0] + state.P3[1] * measured[1] + state.P3[2] * measured[2]) + state.P3[3];
    
    float d = lambda + 2.0 * (measured[0] * Px[0] + measured[1] * Px[1] + measured[2] * Px[2]) + Px[3];
      
    float k[4];
    for (int n = 0; n < 4; n++) k[n] = Px[n] / d;
    
    float e = measured.sqabs() - 2.0 * measured.dot(state.bias) - state.w3;
    
    for (int n = 0; n < 3; n++) state.bias[n] += k[n] * e;
    state.w3 += k[3] * e;
    
    for (int n = 0; n < 4; n++) state.P3[n] = (state.P3[n] - Px[3] * k[n]) / lambda;
    for (int n = 0; n < 3; n++) state.P2[n] = (state.P2[n] - Px[2] * k[n]) / lambda;
    for (int n = 0; n < 2; n++) state.P1[n] = (state.P1[n] - Px[1] * k[n]) / lambda;
    for (int n = 0; n < 1; n++) state.P0[n] = (state.P0[n] - Px[0] * k[n]) / lambda;
    
    state.b2 = state.bias.sqabs();
    state.h2 = state.w3 + state.b2;
  }
  
  // TODO: periodical store state to eeprom
  
  vector = measured - state.bias;
}
