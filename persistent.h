#ifndef __PERSISTENT_H_
#define __PERSISTENT_H_

#include "eeprom.h"

template <typename T>
class Persistent : public T {
private:  
  volatile Eeprom::ReadPacket read;
  volatile Eeprom::WritePacket write;
  
public:
  Persistent(unsigned int address) :
    read( address, reinterpret_cast<char *>(static_cast<T*>(this)), sizeof(T)),
    write(address, reinterpret_cast<char *>(static_cast<T*>(this)), sizeof(T)) { }
  
  void init() { if (!restore()) T::defaults(); }
  
  bool restore() {
    if (!read()) return false;
    read >> static_cast<T&>(*this);
    return true;
  }
  
  void store() {
    write << static_cast<T&>(*this);
    write();
  }
};

#endif
