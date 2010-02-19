#ifndef __PERSISTENT_H_
#define __PERSISTENT_H_

#include "eeprom.h"
#include "safe.h"
#include "timer.h"

template <typename T>
class Persistent : public T {
private:  
  volatile Eeprom::ReadPacket read;
  volatile Eeprom::WritePacket write;
  
public:
  Persistent(unsigned int address) :
    read( address, reinterpret_cast<char *>(static_cast<T*>(this)), sizeof(T)),
    write(address, reinterpret_cast<char *>(static_cast<T*>(this)), sizeof(T)) { }
  
  void init() volatile { if (!restore()) Safe<T>(this)->defaults(); }

  bool restore() volatile {
    if (!read()) return false;
    read >> static_cast<volatile T&>(*this);
    return true;
  }
    
  void store() volatile {
    write << static_cast<volatile T&>(*this);
    write();
    stored();
  }

  Timer::Stamp stored;
};

#endif
