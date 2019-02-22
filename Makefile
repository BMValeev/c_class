
SRC := $(shell pwd)
all:
	$(MAKE)  charge.o boardmodule.o connmodule.o mcu.o screenlight.o GPIOSysfs.o

charge.o:	
	${CXX} I2C.cpp crc.cpp stc3115_Driver.cpp charge.cpp chargeinterface.cpp  -std=c++11 -o charge.o -D C_CLASS_DEBUG

boardmodule.o: 
	${CXX} I2C.cpp BoardModule.cpp crc.cpp -std=c++11 -o boardmodule.o -D C_CLASS_DEBUG

connmodule.o: 
	${CXX} I2C.cpp ConnModule.cpp crc.cpp -std=c++11 -o connmodule.o -D C_CLASS_DEBUG
mcu.o:		SPI.cpp MCU.cpp crc.cpp
	${CXX} SPI.cpp MCU.cpp crc.cpp -std=c++11 -o screenlight.o -D C_CLASS_DEBUG
screenlight.o: 
	${CXX} screenlight.cpp  backlight_driver.cpp backlight_pwm.cpp backlightinterface.cpp -std=c++11 -o screenlight.o -D C_CLASS_DEBUG

GPIOSysfs.o: 
	${CXX} GPIOSysfs.cpp gpiointerface.cpp -std=c++11 -o GPIOSysfs.o -D C_CLASS_DEBUG

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c

