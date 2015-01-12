rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Declaration of variables
CC = g++
CFLAGS = -lmraa  -I. -std=c++0x
LIBS = -lmraa

# File names
EXEC = run
SOURCES = $(filter-out ./test_main.cpp, $(filter-out $(call rwildcard, ./,*test.cpp), $(call rwildcard, ./,*.cpp)))
OBJECTS = $(SOURCES:.cpp=.o)

EXEC_TEST = test_run
SOURCES_TEST = $(filter-out ./main.cpp, $(call rwildcard, ./,*.cpp))
OBJECTS_TEST = $(SOURCES_TEST:.cpp=.o)

#File names for tests


# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(CFLAGS) $(LIBS)

# To obtain object files
%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) 

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)

#For tests
test: $(OBJECTS_TEST)
	$(CC) $(OBJECTS_TEST) -o $(EXEC_TEST) $(CFLAGS) $(LIBS)
test_clean:
	rm -f $(EXEC_TEST) $(OBJECTS_TEST)
