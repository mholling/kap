#ifndef __SPI_H_
#define __SPI_H_

#include "interrupt_driven.h"

class Spi : public InterruptDriven {
public:
  Spi();
  
  class Packet : public Driven<Packet> {
    friend class Driven<Packet>;
    friend class Spi;
    
  protected:
    void initiate();
    bool process();
    void terminate();

  public:
    Packet(volatile unsigned char& select_reg, int select_pin, const unsigned char *tx_buffer, unsigned int tx_length, unsigned char *rx_buffer, unsigned int rx_length) : select_reg(select_reg), select_mask(_BV(select_pin)), tx_buffer(tx_buffer), tx_length(tx_length), rx_buffer(rx_buffer), rx_length(rx_length) { }

  private:
    volatile unsigned char& select_reg;
    const unsigned char select_mask;
    const unsigned char * const tx_buffer;
    const unsigned int tx_length;
    unsigned char * const rx_buffer;
    const unsigned int rx_length;
    unsigned int index;
  };
  
  class ReadPacket : public Packet {
  public:
    ReadPacket(volatile unsigned char& select_reg, int select_pin, unsigned char *rx_buffer, unsigned int rx_length) : Packet(select_reg, select_pin, 0, 0, rx_buffer, rx_length) { }
  };
  
  class WritePacket : public Packet {
  public:
    WritePacket(volatile unsigned char& select_reg, int select_pin, const unsigned char *tx_buffer, unsigned int tx_length) : Packet(select_reg, select_pin, tx_buffer, tx_length, 0, 0) { }
  };
  
  void interrupt() { Packet::head().interrupt(); }
};

#endif
