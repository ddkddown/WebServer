#Makefile

CXX = g++
EXEC_TARGET = WebServer
SOURCE_CPP = main.cpp webserver.cpp csapp.cpp start.cpp
OBJECT_TEST = main.o

LDFLAGS_COMMON = -std=c++11
LPATH_COMMON = 
LLIB_COMMON = 

build:
	$(CXX) $(SOURCE_CPP) $(LPATH_COMMON) $(LLIB_COMMON) -o $(EXEC_TARGET) $(LDFLAGS_COMMON)
clean:
	rm -f $(EXEC_TARGET) *.o
