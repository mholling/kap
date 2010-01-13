#include "app.h"
#include <avr/interrupt.h>
#include "i2c.h"

ISR(TWI_vect) {
  App::app().i2c.interrupt();
}
