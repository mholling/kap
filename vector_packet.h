#ifndef __VECTOR_PACKET_H_
#define __VECTOR_PACKET_H_

#include "i2c.h"
#include "vector.h"

class VectorPacketBase : public I2C::ReadPacket {
protected:
  unsigned char data[6];
    
public:
  VectorPacketBase(unsigned char i2c_address, unsigned char i2c_registers) : I2C::ReadPacket(data, i2c_address, i2c_registers, 6) { }
  Vector vector;
};

template <typename T>
class VectorPacket : public VectorPacketBase {
protected:
  typedef VectorPacket<T> Base;
  virtual void before_dequeue() {
    vector[0] = static_cast<float>(static_cast<T&>(*this).x());
    vector[1] = static_cast<float>(static_cast<T&>(*this).y());
    vector[2] = static_cast<float>(static_cast<T&>(*this).z());
  }
  
public:
  VectorPacket(unsigned char i2c_address, unsigned char i2c_registers) : VectorPacketBase(i2c_address, i2c_registers) { }
};

#endif
