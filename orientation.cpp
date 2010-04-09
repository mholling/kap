#include "orientation.h"
#include "matrix.h"

void Orientation::init() {
  angles.init();
}

void Orientation::adjust(Vector& vector) const {
  vector.rotate_90<2>(angles.yaw);
  vector.rotate_90<1>(angles.pitch);
  vector.rotate_90<0>(angles.roll);
}

Vector Orientation::variance_from_sd(float sdx, float sdy, float sdz) const {
  Vector result(sdx, sdy, sdz);
  adjust(result);
  for (int n = 0; n < 3; n++) result[n] *= result[n];
  return result;
}

