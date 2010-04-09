#ifndef __PERSISTENT_H_
#define __PERSISTENT_H_

#include "eeprom.h"
#include "timer.h"

template <typename T>
class Persistent : public T {
private:  
   Eeprom::ReadPacket read;
   Eeprom::WritePacket write;
  
public:
  Persistent(unsigned int address) :
    read(address, reinterpret_cast<char *>(static_cast<T*>(this)), sizeof(T)),
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
    stored();
  }

  Timer::Stamp stored;
};

#endif
