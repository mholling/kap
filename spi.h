#ifndef __SPI_H_
#define __SPI_H_

#include "queable.h"

class Spi {
public:
  Spi();
  inline void init() { }
  const void interrupt();
  
  class Packet : public Queable<Packet> {
  protected:
    void enqueue();    
    void dequeue();    

    void start();    

  public:
    Packet(const unsigned char *tx_buffer, unsigned int tx_length, volatile unsigned char *rx_buffer, unsigned int rx_length) : tx_buffer(tx_buffer), tx_length(tx_length), rx_buffer(rx_buffer), rx_length(rx_length) { }
    
    void interrupt();

    inline void operator()() { enqueue(); }

  private:
    const unsigned char * const tx_buffer;
    const unsigned int tx_length;
    volatile unsigned char * const rx_buffer;
    const unsigned int rx_length;
    unsigned int index;
    virtual const void toggle_select() = 0;    
  };
  
  class ReadPacket : public Packet {
  public:
    ReadPacket(volatile unsigned char *rx_buffer, unsigned int rx_length) : Packet(0, 0, rx_buffer, rx_length) { }
  };
  
  class WritePacket : public Packet {
  public:
    WritePacket(const unsigned char *tx_buffer, unsigned int tx_length) : Packet(tx_buffer, tx_length, 0, 0) { }
  };
};

#endif
