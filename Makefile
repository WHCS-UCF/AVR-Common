SOURCES=Radio.cpp \
	Timer.cpp \
	timing.cpp \
	ADC.cpp
LIBNAME=common
LIBRARIES=../lib/WHCS_RF24/WHCS_RF24/libRF24.a \
	../lib/MEGA32A_SPI_LIBRARY/MEGA32A_SPI_LIBRARY/libSPI.a

INCLUDE=../


include ../atmega.mk
