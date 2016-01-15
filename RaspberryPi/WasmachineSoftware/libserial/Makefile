CPP = g++
CPPFLAGS = -std=c++11 -O3 -c -Wall -Wextra -pedantic -Werror

PROGRAMS=main
all: $(PROGRAMS)

clean:
	rm *.o $(PROGRAMS) &> /dev/null || true
	
main: main.o libserial.o UART.o
	$(CPP) -o $@ libserial.o UART.o main.o

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $<

libserial.o: libserial.h libserial.cpp
UART.o: UART.h UART.cpp
main.o: UART.h UART.cpp libserial.h libserial.cpp
