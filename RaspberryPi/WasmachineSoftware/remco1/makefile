CC =g++
CFLAGS=-std=gnu++11 -c -Wall -MMD -MP
LD_FRONT_FLAGS=-pthread 
LD_END_FLAGS=-lcrypto -lssl
SOURCES = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDS = $(SOURCES:.cpp=.d)
EXECUTABLE = Main

.PHONY: all clean

all: $(EXECUTABLE)
	@echo "make all"

run: $(EXECUTABLE)
	./$<
	@echo "make run"

$(EXECUTABLE): $(OBJECTS)
	@echo "linking"
	$(CC) $(LD_FRONT_FLAGS) -o $@ $^ $(LD_END_FLAGS)

%.o: %.cpp
	@echo ".cpp -> .o"
	$(CC) $(CFLAGS) -c $< -o $@    

clean:
	@echo "make clean"
	rm -rf *.o *.d
	rm -rf */*.o */*.d
	rm $(EXECUTABLE)

-include $(DEPENDS)
