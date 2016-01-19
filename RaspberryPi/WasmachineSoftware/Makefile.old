#-----------------------------------------------------------------------------------------
# Makefile for GCC & gmake
#-----------------------------------------------------------------------------------------
#
# 
#
#
#
#
#
#-----------------------------------------------------------------------------------------

# Compiler settings
CPP = g++
#CPPFLAGS = -std=c++11 -W -O3 -c -I./source
CPPFLAGS = -std=c++11 -W -O3 -c -Wall -Wextra -pedantic -Werror -I./pRTOS/source -I./pRTOS -I./libserial
LFLAGS =
LIBS =

# Wasmachine object files
WASMACHINE_OBJS = main.o 

# RTOS object files
RTOS_OBJS = libfiber.o pRTOS.o

# Libserial object files
LIBSERIAL_OBJS = libserial.o

all: main

clean:
	rm main *.o $(RTOS_OBJS) $(LIBSERIAL_OBJS)

main: $(WASMACHINE_OBJS) $(RTOS_OBJS) $(LIBSERIAL_OBJS)
	$(CPP) $(LFLAGS) -o $@ $(WASMACHINE_OBJS) $(RTOS_OBJS) &(LIBSERIAL_OBJS)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $<

%.o: ./pRTOS/source/%.cpp
	$(CPP) $(CPPFLAGS) $<

%.o: ./libserial/%.cpp
	$(CPP) $(CPPFLAGS) $<

# Dependencies
main.o: main.cpp ./pRTOS/source/pRTOS.h ./pRTOS/source/libfiber.h ./Protocol.h
pRTOS.o: ./pRTOS/source/pRTOS.cpp ./pRTOS/source/pRTOS.h ./pRTOS/source/libfiber.cpp ./pRTOS/source/libfiber.h
libserial.o: ./libserial/libserial.cpp ./libserial/libserial.h
