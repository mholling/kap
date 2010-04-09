#include "accelerometer.h"
#include "app.h"

unsigned char Accelerometer::InterruptConfigPacket::data[2] = { 0x08, 0x08 }; // send interrupts for DATA_READY to INT2
unsigned char Accelerometer::DataFormatPacket::data[1] = { 0x20 }; // active-low interrupts

Vector Accelerometer::vector() const {
  Vector result = measure.vector;
  app.orientation.adjust(result);
  return result;
}
