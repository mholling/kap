#ifndef __SPI_H_
#define __SPI_H_

#include "queable.h"

class Spi {  
public:
  Spi();
  void init();
  const void interrupt();
  
  class Message : public Queable<Message> {
  public:
    Message(const unsigned char *tx_buffer, volatile unsigned char *rx_buffer, unsigned int tx_length, unsigned int rx_length) : tx_buffer(tx_buffer), rx_buffer(rx_buffer), tx_length(tx_length), rx_length(rx_length) { }
    
    inline void operator()() { enqueue(); }
    void next();

  private:
    const unsigned char * const tx_buffer;
    volatile unsigned char * const rx_buffer;
    const unsigned int tx_length;
    const unsigned int rx_length;
    unsigned int index;
    virtual const void toggle_select() = 0;
    
  protected:
    void enqueue();
    void start();    
  };
};

#endif
