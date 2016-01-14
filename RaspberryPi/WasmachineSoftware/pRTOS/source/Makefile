CPP = g++
CPPFLAGS = -std=c++11 -O3 -c -Wall -Wextra -pedantic -Werror

PROGRAMS=example
all: $(PROGRAMS)

clean:
	rm *.o $(PROGRAMS) &> /dev/null || true
	
example: example.o libfiber.o
	$(CPP) -o $@ libfiber.o example.o

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $<

libfiber.o: libfiber.h
example.o: libfiber.h libfiber.cpp
