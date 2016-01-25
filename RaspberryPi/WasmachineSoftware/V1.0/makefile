CC =g++
CFLAGS=-std=gnu++11 -c -w -Wall -MMD -MP
LD_FRONT_FLAGS=-pthread 
LD_END_FLAGS=-lcrypto -lssl
SOURCES = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDS = $(SOURCES:.cpp=.d)
INCLUDE = $(addprefix -I, $(wildcard */))
EXECUTABLE = main

.PHONY: all clean doc

all: $(EXECUTABLE)
	@echo "make all"

run: $(EXECUTABLE)
	./$<
	@echo "make run"

$(EXECUTABLE): $(OBJECTS)
	@echo "linking"
	$(CC) $(LD_FRONT_FLAGS) $(INCLUDE) -o $@ $^ $(LD_END_FLAGS)

%.o: %.cpp
	@echo ".cpp -> .o"
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@    

clean:
	@echo "make clean"
	-rm -rf *.o *.d
	-rm -rf */*.o */*.d
	-rm -rf ./Doc
	-rm -rf $(EXECUTABLE)

doc:
	@echo "Making documentation"
	doxygen Doxyfile

-include $(DEPENDS)
