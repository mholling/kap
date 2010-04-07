#include "orientation.h"
#include "matrix.h"

void Orientation::init() {
  angles.init();
  rotation = Matrix<3, 3>::rotation(0, static_cast<float>(angles.roll)  * M_PI / 180.0) *
             Matrix<3, 3>::rotation(1, static_cast<float>(angles.pitch) * M_PI / 180.0) *
             Matrix<3, 3>::rotation(2, static_cast<float>(angles.yaw)   * M_PI / 180.0);
}

const Vector Orientation::adjust_vector(const Vector& vector) const {
  return rotation * vector;
}

const Vector Orientation::adjust_variance(float vx, float vy, float vz) const {
  Matrix<3, 3> covariance;
  covariance(0,0) = vx;
  covariance(1,1) = vy;
  covariance(2,2) = vz;
  covariance = rotation * covariance * rotation.t();
  Vector result;
  result[0] = covariance(0,0);
  result[1] = covariance(1,1);
  result[2] = covariance(2,2);
  return result;
}

