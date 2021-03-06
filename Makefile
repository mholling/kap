# Name: Makefile
# Author: <insert your name here>
# Copyright: <insert your copyright message here>
# License: <insert your license reference here>

# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.

DEVICE     = atmega328p
CLOCK      = 8000000
PROGRAMMER = -c arduino -P /dev/tty.usbserial-A800csBR -b 57600
OBJECTS    = main.o app.o task.o serial.o timer.o eeprom.o orientation.o i2c.o analog.o spi.o shift_register.o pwm.o motors.o magnetometer.o accelerometer.o gyros.o trajectory.o attitude.o pid.o diagnostic.o

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -mcall-prologues

# symbolic targets:
all:	clear main.hex

increment:
	ruby increment.rb

size:	all
    # avr-size $(OBJECTS) main.hex
	avr-size -C --mcu=$(DEVICE) main.elf

clear:
	clear; clear

debug:
	screen /dev/tty.usbserial-A800csBR 38400
	
.c.o:
	$(COMPILE) -c $< -o $@

.cpp.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all size increment
	$(AVRDUDE) -U flash:w:main.hex:i


fd:	flash debug

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS) -lm
#	$(COMPILE) -o main.elf $(OBJECTS) -Wl -u vfprintf -lprintf_flt -lm

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.cpp
