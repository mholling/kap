#ifndef __VECTOR_PACKET_H_
#define __VECTOR_PACKET_H_

#include "i2c.h"
#include "vector.h"

class VectorPacket : public I2C::ReadPacket {
protected:
  unsigned char data[6];
  
public:
  VectorPacket(unsigned char i2c_address, unsigned char i2c_registers) : I2C::ReadPacket(data, i2c_address, i2c_registers, 6) { }
  Vector vector;
};

#endif
