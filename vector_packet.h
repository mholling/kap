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

template <typename T>
class OrientedVectorPacket : public VectorPacket {
protected:
  virtual void before_dequeue() {
    vector[0] = static_cast<float>(static_cast<T&>(*this).x());
    vector[1] = static_cast<float>(static_cast<T&>(*this).y());
    vector[2] = static_cast<float>(static_cast<T&>(*this).z());
  }
  
public:
  OrientedVectorPacket(unsigned char i2c_address, unsigned char i2c_registers) : VectorPacket(i2c_address, i2c_registers) { }
};

#endif
