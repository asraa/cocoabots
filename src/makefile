rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

#python things
PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
# location of the Boost Python include files and library
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib
CEXTRAFLAGS= -shared -Wl,--export-dynamic -L$(BOOST_LIB) -lboost_python -L/usr  

# Declaration of variables
CC = g++
CFLAGS = -I. -std=c++11 -fPIC
LIBS = -lmraa -I /usr/local/include/eigen3 -I/usr/local/include/opencv -I/usr/local/include/opencv2 -std=c++11 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video -lopencv_features2d 
CDEBUGFLAGS = -g

OBJPYTHONLIBRARIES = -I$(PYTHON_INCLUDE) -I$(BOOST_INC)


# File names
EXEC = creepybot.so
SOURCES =  $(filter-out ./main.cpp, $(filter-out ./test_main.cpp, $(filter-out $(call rwildcard, ./,*test.cpp), $(call rwildcard, ./,*.cpp))))
OBJECTS = $(SOURCES:.cpp=.o)

EXEC_TEST = test_run
SOURCES_TEST = $(filter-out ./main.cpp, $(call rwildcard, ./,*.cpp))
OBJECTS_TEST = $(SOURCES_TEST:.cpp=.o)

ACTUATORS_SOURCES = $(call rwildcard, ./actuators/,*.cpp)
ACTUATORS_OBJECTS = $(ACTUATORS_SOURCES:.cpp=.o)

SENSORS_SOURCES = $(call rwildcard, ./sensors/,*.cpp)
SENSORS_OBJECTS = $(SENSORS_SOURCES:.cpp=.o)

IMAGES_SOURCES = $(call rwildcard, ./imageProcessing/,*.cpp)
IMAGES_OBJECTS = $(IMAGES_SOURCES:.cpp=.o)

#File names for tests






# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(CFLAGS) $(CEXTRAFLAGS) $(LIBS)

# To obtain object files
%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(OBJPYTHONLIBRARIES)

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)

#For tests
test: $(OBJECTS_TEST)
	$(CC) $(OBJECTS_TEST) -o $(EXEC_TEST) $(CFLAGS) $(LIBS)
test_clean:
	rm -f $(EXEC_TEST) $(OBJECTS_TEST)

clean_actuators:
	rm -f $(EXEC_TEST) $(EXEC) $(ACTUATORS_OBJECTS)

clean_sensors:
	rm -f $(EXEC_TEST) $(EXEC) $(SENSORS_OBJECTS)

clean_images:
	rm -f $(EXEC_TEST) $(EXEC) $(IMAGES_OBJECTS)

debug: CFLAGS+= -g
debug: $(EXEC)

debug_test: CFLAGS+= -g
debug_test: test

warnings: CFLAGS+= -Wall -Wno-reorder 2>>makeFileLog.txt
warnings: $(EXEC)

clean_log: 
	rm -f makeFileLog.txt
