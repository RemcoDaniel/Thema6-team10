#-----------------------------------------------------------------------------------------
# Makefile for GCC & gmake
#-----------------------------------------------------------------------------------------
# Copyright (c) 2014 Marten Wensink
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
#     1. The origin of this software must not be misrepresented; you must not
#     claim that you wrote the original software. If you use this software
#     in a product, an acknowledgment in the product documentation would be
#     appreciated but is not required.
#
#     2. Altered source versions must be plainly marked as such, and must not be
#     misrepresented as being the original software.
#
#     3. This notice may not be removed or altered from any source
#     distribution.
#-----------------------------------------------------------------------------------------

# Compiler settings
CPP = g++
#CPPFLAGS = -std=c++11 -W -O3 -c -I./source
CPPFLAGS = -std=c++11 -W -O3 -c -Wall -Wextra -pedantic -Werror -I./source
LFLAGS =
LIBS =

# Object files for the test program
TEST_OBJS = test.o

# RTOS object files
RTOS_OBJS = libfiber.o pRTOS.o

all: test

clean:
	rm test *.o $(RTOS_OBJS)

test: $(TEST_OBJS) $(RTOS_OBJS)
	$(CPP) $(LFLAGS) -o $@ $(TEST_OBJS) $(RTOS_OBJS)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $<

%.o: ./source/%.cpp
	$(CPP) $(CPPFLAGS) $<

# Dependencies
test.o: test.cpp ./source/pRTOS.h ./source/libfiber.h
pRTOS.o: ./source/pRTOS.cpp ./source/pRTOS.h ./source/libfiber.cpp ./source/libfiber.h
