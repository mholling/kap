#ifndef __SPI_H_
#define __SPI_H_

#include "interrupt_driven.h"

class Spi : public InterruptDriven {
public:
  Spi();
  
  class Packet : public Item {
  protected:
    void initiate();
    bool process();
    void terminate();

  public:
    Packet(const unsigned char *tx_buffer, unsigned int tx_length, unsigned char *rx_buffer, unsigned int rx_length) : tx_buffer(tx_buffer), tx_length(tx_length), rx_buffer(rx_buffer), rx_length(rx_length) { }
    
    bool operator ()(bool block = false) { return enqueue(block); }

  private:
    const unsigned char * const tx_buffer;
    const unsigned int tx_length;
    unsigned char * const rx_buffer;
    const unsigned int rx_length;
    unsigned int index;
    virtual void toggle_select() = 0;    
  };
  
  class ReadPacket : public Packet {
  public:
    ReadPacket(unsigned char *rx_buffer, unsigned int rx_length) : Packet(0, 0, rx_buffer, rx_length) { }
  };
  
  class WritePacket : public Packet {
  public:
    WritePacket(const unsigned char *tx_buffer, unsigned int tx_length) : Packet(tx_buffer, tx_length, 0, 0) { }
  };
};

#endif
