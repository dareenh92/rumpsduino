CMSIS       = ${PROGRAM}/c_library/cmsis
LPC11XX     = ${PROGRAM}/c_library/lpc11xx
TYPE      	= ${PROGRAM}/uv1/lib/type
MEMORY      = ${PROGRAM}/uv1/lib/memory
DEBUG_REG   = ${PROGRAM}/uv1/lib/debug_reg
NOC         = ${PROGRAM}/uv1/lib/noc
GPIO        = ${PROGRAM}/uv1/lib/gpio
AES         = ${PROGRAM}/uv1/lib/aes
PP          = ${PROGRAM}/uv1/lib/parallel_port
MUX         = ${PROGRAM}/uv1/lib/mux_ctrl
FLASH       = ${PROGRAM}/uv1/lib/flash
TIMER       = ${PROGRAM}/uv1/lib/timer
UART        = ${PROGRAM}/uv1/lib/uart
UART_HD   	= ${PROGRAM}/uv1/lib/uart_hd
MAC					= ${PROGRAM}/uv1/lib/mac
GLCD        = ${PROGRAM}/uv1/lib/glcd
SEVEN_SEG   = ${PROGRAM}/uv1/lib/seven_seg
MATRIX      = ${PROGRAM}/uv1/lib/matrix
SPI         = ${PROGRAM}/uv1/lib/spi
SPI_DISK    = ${PROGRAM}/uv1/lib/spi_disk
FF    			= ${PROGRAM}/uv1/lib/ff
RTC   			= ${PROGRAM}/uv1/lib/rtc
XPRINTF		  = ${PROGRAM}/uv1/lib/xprintf


MAIN0     = ${PROGRAM}/uv1/${program}/main0
MAIN1     = ${PROGRAM}/uv1/${program}/main1
MAIN2     = ${PROGRAM}/uv1/${program}/main2
MAIN3     = ${PROGRAM}/uv1/${program}/main3
ISR       = ${PROGRAM}/uv1/${program}/isr
INCLUDE  	= ${PROGRAM}/uv1/include
LINKER   	= ${PROGRAM}/uv1/linker/cm0.ld

OPTION0 =   -I ${MAIN0} -I ${INCLUDE} -I ${CMSIS}  -I ${LPC11XX} -I ${TYPE} -I ${MEMORY} -I ${DEBUG_REG} -I ${NOC} \
            -I ${GPIO} -I ${AES} -I ${PP} -I ${MUX} -I ${FLASH} -I ${TIMER} -I ${UART} -I ${UART_HD} -I ${GLCD} \
            -I ${SEVEN_SEG} -I ${MATRIX} -I ${SPI} -I ${SPI_DISK} -I ${FF} -I ${MAC} -I ${ISR} \
            ${MAIN0}/main.c ${CMSIS}/core_cm0.c ${MEMORY}/memory.c ${DEBUG_REG}/debug_reg.c ${NOC}/noc.c \
            ${GPIO}/gpio.c ${AES}/aes.c ${PP}/parallel_port.c ${MUX}/mux_ctrl.c ${FLASH}/flash.c ${TIMER}/timer.c \
           	${UART}/uart.c ${UART_HD}/uart_hd.c ${MAC}/mac.c ${GLCD}/glcd.c ${SEVEN_SEG}/seven_seg.c ${MATRIX}/matrix.c \
           	${SPI}/spi.c ${ISR}/isr0.c -T ${LINKER}

#OPTION0 =   -I ${MAIN0} -I ${INCLUDE} -I ${CMSIS}  -I ${LPC11XX} -I ${TYPE} -I ${MEMORY} -I ${NOC} \
            -I ${GPIO} -I ${MUX} -I ${FLASH} -I ${UART_HD} \
            -I ${ISR} \
            ${MAIN0}/main.c ${CMSIS}/core_cm0.c ${MEMORY}/memory.c ${NOC}/noc.c \
            ${GPIO}/gpio.c ${MUX}/mux_ctrl.c ${FLASH}/flash.c \
           	${UART_HD}/uart_hd.c \
           	${ISR}/isr0.c -T ${LINKER}

#OPTION0 =   -I ${MAIN0} -I ${INCLUDE} -I ${CMSIS}  -I ${LPC11XX} -I ${TYPE} -I ${MEMORY} \
            -I ${GPIO} -I ${MUX} -I ${FLASH} -I ${TIMER} -I ${UART_HD} \
            -I ${SPI} -I ${SPI_DISK} -I ${FF} -I ${RTC} -I ${XPRINTF} -I ${ISR} \
            ${MAIN0}/main.c ${CMSIS}/core_cm0.c ${MEMORY}/memory.c \
            ${GPIO}/gpio.c ${MUX}/mux_ctrl.c ${FLASH}/flash.c ${TIMER}/timer.c \
           	 ${UART_HD}/uart_hd.c \
           	${SPI}/spi.c ${SPI_DISK}/spi_disk.c ${FF}/ff.c ${RTC}/rtc.c ${XPRINTF}/xprintf.c ${ISR}/isr0.c -T ${LINKER}

#OPTION0 =   -I ${MAIN0} -I ${INCLUDE} -I ${CMSIS}  -I ${LPC11XX} -I ${TYPE} -I ${MEMORY} \
            -I ${GPIO} -I ${MUX} -I ${FLASH} -I ${TIMER} -I ${UART_HD} \
            -I ${SPI} -I ${SPI_DISK} -I ${FF} -I ${ISR} \
            ${MAIN0}/main.c ${CMSIS}/core_cm0.c ${MEMORY}/memory.c \
            ${GPIO}/gpio.c ${MUX}/mux_ctrl.c ${FLASH}/flash.c ${TIMER}/timer.c \
           	 ${UART_HD}/uart_hd.c \
           	${SPI}/spi.c ${SPI_DISK}/spi_disk.c ${ISR}/isr0.c -T ${LINKER}

OPTION1 =   -I $(MAIN1) -I $(INCLUDE) -I $(CMSIS)  -I $(LPC11XX) -I $(TYPE) -I $(MEMORY) -I $(DEBUG_REG) -I $(NOC) \
            -I $(GPIO) -I $(AES) -I $(PP) -I $(MUX) -I $(FLASH) -I $(TIMER) -I $(UART) -I $(UART_HD) -I $(GLCD) \
            -I $(SEVEN_SEG) -I $(MATRIX) -I $(SPI) -I $(SPI_DISK) -I $(FF) -I $(MAC) -I $(ISR) \
            $(MAIN1)/main.c $(CMSIS)/core_cm0.c $(MEMORY)/memory.c $(DEBUG_REG)/debug_reg.c $(NOC)/noc.c \
            $(GPIO)/gpio.c $(AES)/aes.c $(PP)/parallel_port.c $(MUX)/mux_ctrl.c $(FLASH)/flash.c $(TIMER)/timer.c \
           	$(UART)/uart.c $(UART_HD)/uart_hd.c $(MAC)/mac.c $(GLCD)/glcd.c $(SEVEN_SEG)/seven_seg.c $(MATRIX)/matrix.c \
           	$(SPI)/spi.c $(ISR)/isr1.c -T $(LINKER)

OPTION2 =   -I ${MAIN2} -I ${INCLUDE} -I ${CMSIS}  -I ${LPC11XX} -I ${TYPE} -I ${MEMORY} -I ${DEBUG_REG} -I ${NOC} \
            -I ${GPIO} -I ${AES} -I ${PP} -I ${MUX} -I ${FLASH} -I ${TIMER} -I ${UART} -I ${UART_HD} -I ${GLCD} \
            -I ${SEVEN_SEG} -I ${MATRIX} -I ${SPI} -I ${SPI_DISK} -I ${FF} -I ${MAC} -I ${ISR} \
            ${MAIN2}/main.c ${CMSIS}/core_cm0.c ${MEMORY}/memory.c ${DEBUG_REG}/debug_reg.c ${NOC}/noc.c \
            ${GPIO}/gpio.c ${AES}/aes.c ${PP}/parallel_port.c ${MUX}/mux_ctrl.c ${FLASH}/flash.c ${TIMER}/timer.c \
           	${UART}/uart.c ${UART_HD}/uart_hd.c ${MAC}/mac.c ${GLCD}/glcd.c ${SEVEN_SEG}/seven_seg.c ${MATRIX}/matrix.c \
           	${SPI}/spi.c ${ISR}/isr2.c -T ${LINKER}

OPTION3 =   -I ${MAIN3} -I ${INCLUDE} -I ${CMSIS}  -I ${LPC11XX} -I ${TYPE} -I ${MEMORY} -I ${DEBUG_REG} -I ${NOC} \
            -I ${GPIO} -I ${AES} -I ${PP} -I ${MUX} -I ${FLASH} -I ${TIMER} -I ${UART} -I ${UART_HD} -I ${GLCD} \
            -I ${SEVEN_SEG} -I ${MATRIX} -I ${SPI} -I ${SPI_DISK} -I ${FF} -I ${MAC} -I ${ISR} \
            ${MAIN3}/main.c ${CMSIS}/core_cm0.c ${MEMORY}/memory.c ${DEBUG_REG}/debug_reg.c ${NOC}/noc.c \
            ${GPIO}/gpio.c ${AES}/aes.c ${PP}/parallel_port.c ${MUX}/mux_ctrl.c ${FLASH}/flash.c ${TIMER}/timer.c \
           	${UART}/uart.c ${UART_HD}/uart_hd.c ${MAC}/mac.c ${GLCD}/glcd.c ${SEVEN_SEG}/seven_seg.c ${MATRIX}/matrix.c \
           	${SPI}/spi.c ${ISR}/isr3.c -T ${LINKER}

all: bin0 bin1 bin2 bin3
#all: bin0
  
bin0:
	arm-none-eabi-gcc -g -O3 -mthumb -mcpu=cortex-m0 -mfloat-abi=softfp -std=gnu99 ${OPTION0} -lm -o main0.out
	arm-none-eabi-objcopy -O binary main0.out main0.bin
  
bin1:
	arm-none-eabi-gcc -g -O3 -mthumb -mcpu=cortex-m0 -mfloat-abi=softfp -std=gnu99 ${OPTION1} -lm -o main1.out
	arm-none-eabi-objcopy -O binary main1.out main1.bin
  
bin2:
	arm-none-eabi-gcc -g -O3 -mthumb -mcpu=cortex-m0 -mfloat-abi=softfp -std=gnu99 ${OPTION2} -lm -o main2.out
	arm-none-eabi-objcopy -O binary main2.out main2.bin
  
bin3:
	arm-none-eabi-gcc -g -O3 -mthumb -mcpu=cortex-m0 -mfloat-abi=softfp -std=gnu99 ${OPTION3} -lm -o main3.out
	arm-none-eabi-objcopy -O binary main3.out main3.bin

clean:
	rm -f *.hex *.bin *.lst *.out *~ 
