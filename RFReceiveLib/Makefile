MCU   = attiny85

## Main source
MAIN_SOURCE = example.c

## Other sources seperated by a SPACE
OTHER_SOURCES = RFReceiveLib.c 

## Defined programs / locations
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AVRSIZE = avr-size
AVRDUDE = avrdude

CFLAGS= -mmcu=$(MCU) -Os  -Wall -std=gnu99

## All the source files
SRC = $(MAIN_SOURCE) $(OTHER_SOURCES)

OBJS = $(SRC:.c=.o)

TARGET = $(strip $(basename $(MAIN_SOURCE)))

## List of all header files
HEADERS = ../../RFReceiveLib.h ../../serial.h
##HEADERS = $(SRC:.c=.h) 

## For every .c file, compile an .o object file
OBJS = $(SRC:.c=.o) 
HEXS = $(MAIN_SOURCE:.c=.hex)

$(TARGET).hex: $(TARGET).elf

%.hex: %.elf
	$(OBJCOPY) -R .eeprom -O ihex $< $@
#	rm -f *.elf *.o

%.elf: $(SRC)
	$(CC) $(CFLAGS) $(SRC) --output $@ 

$(OBJS): $HEADERS

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).obj \
	$(TARGET).o $(TARGET).d $(TARGET).eep $(TARGET).lst \
	$(TARGET).lss $(TARGET).sym $(TARGET).map $(TARGET)~ \
	$(TARGET).eeprom

squeaky_clean:
	rm -f *.elf *.hex *.obj *.o *.d *.eep *.lst *.lss *.sym *.map *~
