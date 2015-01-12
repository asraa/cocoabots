rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Declaration of variables
CC = g++
CFLAGS = -lmraa  -I. -std=c++0x
LIBS = -lmraa

# File names
EXEC = run
#SOURCES = $(call rwildcard, ./,*.cpp))
SOURCES = $(filter-out $(call rwildcard, ./,*test.cpp), $(call rwildcard, ./,*.cpp))
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(CFLAGS) $(LIBS)

# To obtain object files
%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) 

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
