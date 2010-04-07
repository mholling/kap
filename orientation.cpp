#include "orientation.h"
#include "matrix.h"

void Orientation::init() {
  angles.init();
}

Vector Orientation::adjust_vector(const Vector& vector) const {
  Vector result = vector;
  result.rotate_90<2>(angles.yaw);
  result.rotate_90<1>(angles.pitch);
  result.rotate_90<0>(angles.roll);
  return result;
}

Vector Orientation::adjust_variance(float vx, float vy, float vz) const {
  Vector result(vx, vy, vz);
  result.rotate_90<2>(angles.yaw);
  result.rotate_90<1>(angles.pitch);
  result.rotate_90<0>(angles.roll);
  for (int n = 0; n < 3; n++)
    if (result[n] < 0.0) result[n] = -result[n];
  return result;
}

