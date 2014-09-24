#
# Brief: Makefile for firmware of simple tool to check 12V Pb-Acid battery
# (C) 2014 Sergey Shcherbakov <shchers@gmail.com>
#

GCC_MCU := attiny13
DUDE_MCU := t13
DUDE_BAUD := 200

# SPM enabled, dW enabled and BOD=2.7V
HFUSE := 0xe3

# SPI, Preserver EEPROM and 9.6MHz
LFUSE := 0x32

# Frequency of CPU in Hz
F_CPU := 9600000

# Firmware name
BIN := avr_bat

CFLAGS := -DF_CPU=$(F_CPU)UL
# Compiler options
CFLAGS += -mmcu=$(GCC_MCU) -Os -gdwarf-2 -std=gnu99
# C Dialect options
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
# Warning options
CFLAGS += -Wall -Wstrict-prototypes
# Includes
CFLAGS += -I ./

LDFLAGS := -lm

ifeq ($(shell which avr-gcc),)
$(error Please install avr-gcc package)
endif

ifeq ($(shell which avrdude),)
$(error Please install avrdude package)
endif


.PHONY: all firmware flash fuses clean

all: firmware

clean:
	@rm -v *.o $(BIN).elf $(BIN).hex $(BIN).eep $(BIN).map

fuses:
	@avrdude -p $(DUDE_MCU) -c dragon_isp -P usb -B $(DUDE_BAUD) -U hfuse:w:$(HFUSE):m -U lfuse:w:$(LFUSE):m

firmware:
# Compiling sources
	@avr-gcc -c $(CFLAGS) main.c -o main.o
	@avr-gcc $(CFLAGS) main.o --output $(BIN).elf -Wl,-Map=$(BIN).map,--cref $(LDFLAGS)
# Creating load file for Flash
	@avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $(BIN).elf $(BIN).hex
# Creating load file for EEPROM
	@avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex $(BIN).elf $(BIN).eep
# Just print size
	@avr-size -C $(BIN).elf

flash: firmware
	@avrdude -p $(DUDE_MCU) -c dragon_isp -P usb -B $(DUDE_BAUD) -U flash:w:$(BIN).hex

