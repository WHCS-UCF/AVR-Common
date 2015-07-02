SOURCES=Radio.cpp \
	Timer.cpp \
	timing.cpp
LIBNAME=common
INCLUDE=../lib/WHCS_RF24/WHCS_RF24/ \
	../lib/MEGA32A_SPI_LIBRARY/MEGA32A_SPI_LIBRARY/


include ../atmega.mk
