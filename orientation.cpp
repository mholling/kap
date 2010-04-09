#include "orientation.h"
#include "matrix.h"

void Orientation::init() {
  angles.init();
}

Vector Orientation::adjust(const Vector& vector) const {
  Vector result = vector;
  result.rotate_90<2>(angles.yaw);
  result.rotate_90<1>(angles.pitch);
  result.rotate_90<0>(angles.roll);
  return result;
}

Vector Orientation::variance_from_sd(float sdx, float sdy, float sdz) const {
  Vector result = adjust(Vector(sdx, sdy, sdz));
  for (int n = 0; n < 3; n++) result[n] *= result[n];
  return result;
}

