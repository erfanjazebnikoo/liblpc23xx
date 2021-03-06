
#
# Makefile for liblpc23xx.a
#

NAME            := liblpc23xx

##
# this is the default, 'make LPC23XX_PART=-DLPC2378' will over-ride
#LPC23XX_PART    ?= -DLPC2378
LPC23XX_PART   ?= -DGFE_LPC2368

##
# If you are using port B the LPC2378 uncomment out the next line.
# this is the default, make LPC2378_PORT= will overwrite
#LPC2378_PORT    = -DLPC2378_PORTB

CROSSCMP        := /opt/cross
CROSSNAME       ?= arm-elf

CC              := $(CROSSCMP)/bin/$(CROSSNAME)-gcc
LD              := $(CROSSCMP)/bin/$(CROSSNAME)-ld
AR              := $(CROSSCMP)/bin/$(CROSSNAME)-ar
AS              := $(CROSSCMP)/bin/$(CROSSNAME)-as
CP              := $(CROSSCMP)/bin/$(CROSSNAME)-objcopy
OD              := $(CROSSCMP)/bin/$(CROSSNAME)-objdump

GCC_VERSION_GE_46 := $(shell $(CC) -dumpversion | gawk '{print $$1>=4.6?"1":"0"}')

#DEBUG           ?= -DDEBUG_ADC
#DEBUG           = -DDEBUG_UTIL
 
INCLUDE         := -I.\
                   -I./include\
                   -I./lpc23xx-info/include\
                   -I./lpc23xx-adc/include\
                   -I./lpc23xx-i2c/include\
                   -I./lpc23xx-mam/include\
                   -I./lpc23xx-pll/include\
                   -I./lpc23xx-spi/include\
                   -I./lpc23xx-uart/include\
                   -I./lpc23xx-util/include\
                   -I./lpc23xx-vic/include\
                   -I./lpc23xx-binsem/include\
                   -I./lpc23xx-timer/include\
                   -I./lpc23xx-usb/include\
                   -I./lpc23xx-power/include\
                   -I./external-devices/include

HS              :=  $(wildcard ./include/*.h)\
                    $(wildcard ./lpc23xx-*/include/*.h)

TOPLIB          = $(NAME).a

TESTS           = ./lpc23xx-pll/test/lpc23xx-pll-test.hex\
	          ./lpc23xx-binsem/test/lpc23xx-binsem-test.hex\
	          ./lpc23xx-adc/adc-test/adc-test.hex\
	          ./lpc23xx-util/util-test/util-test.hex\
	          ./lpc23xx-spi/spi-test/spi-test.hex\
	          ./lpc23xx-usb/serial-test/serial-test.hex\
	          ./lpc23xx-usb/datapath-test/datapath-test.hex\
	          ./lpc23xx-uart/test/lpc23xx-uart-test.hex\
	          ./lpc23xx-timer/timer-test/timer-test.hex

ASRCS           := $(wildcard lpc23xx-*/*.s)

CSRCS           := $(wildcard lpc23xx-*/*.c)

COBJS           = $(CSRCS:.c=.o)

AOBJS           = $(ASRCS:.s=.o)
                  
ifeq ($(GCC_VERSION_GE_46),1)
CFLAGS          = $(INCLUDE) $(DEBUG) $(LPC2378_PORT) $(LPC23XX_PART) -c \
                  -Wall -Werror -Wno-unused-but-set-variable -g3 \
                  -mfloat-abi=softfp -fno-common -O3 -mcpu=arm7tdmi-s
else
CFLAGS          = $(INCLUDE) $(DEBUG) $(LPC2378_PORT) $(LPC23XX_PART) -c \
                  -Wall -Werror -g3 \
                  -mfloat-abi=softfp -fno-common -O3 -mcpu=arm7tdmi-s
endif

ARCHIVEFLAGS    = rs

ASFLAGS         = -g -ahls -mfloat-abi=softfp $(INCLUDE) 
 
.PHONY: clean allclean rebuild

.SUFFIXES : .c .cpp .s

.c.o :
	@echo "-------- COMPILING $@ "
	@$(CC) $(CFLAGS) -o $(<:.c=.o) $<

.s.o :
	@echo "-------- COMPILING $@ "
	@$(AS) $(ASFLAGS) -o $@ $< > $*.lst
        
all: $(TOPLIB) Makefile

tests: $(TESTS)

$(COBJS): $(HS)

$(TOPLIB): $(AOBJS) $(COBJS)
	@echo "--------- Making Library $@ ------------------------"
	@$(AR) $(ARCHIVEFLAGS) $@ $(AOBJS) $(COBJS)

$(TESTS): $(LIB)
	@echo "------- Recursive make: $(@D) ------------------------"
	@$(MAKE) -s -C $(@D) $(@F)

clean:
	@$(RM)  $(LIB) $(AOBJS) $(COBJS) $(COBJS) $(USBLIB)\
	lpc23xx*/*.lst *.map *.hex *.bin *.lst *~ ./include/*~ a.out 

allclean: clean
	$(MAKE) -s -C lpc23xx-pll/test clean
	$(MAKE) -s -C lpc23xx-uart/test clean
	$(MAKE) -s -C lpc23xx-binsem/test clean
	$(MAKE) -s -C lpc23xx-usb/serial-test clean
	$(MAKE) -s -C lpc23xx-usb/datapath-test clean
	$(MAKE) -s -C lpc23xx-adc/adc-test clean
	$(MAKE) -s -C lpc23xx-spi/spi-test clean
	$(MAKE) -s -C lpc23xx-timer/timer-test clean
	$(MAKE) -s -C lpc23xx-util/util-test clean
	
rebuild: allclean
	$(MAKE)

