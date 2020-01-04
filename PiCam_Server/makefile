CXX  = g++
CXXFLAGS = -Wall -g -pthread -I../Server
PIGFLAGS = -lpigpio -lpigpiod_if2 -lrt
CVFLAGS = `pkg-config --cflags --libs opencv4`

#CXXFLAGS: -Wall generates warnings, -g turns on debugging (clearer output)
# and -I tells make to look in current directory for header files

# Object file = compiled source file, generated using -c
# Execuable file = linked object file, generated using -o
# 2-line format for each target (i.e. executable or object file)
# Line 1: dependencies for each target
# Line 2: target generation

servdir = ../Server
main: main.o $(servdir)/Server.o
	$(CXX) $(CXXFLAGS) -o main main.o $(servdir)/Server.o $(PIGFLAGS) $(CVFLAGS)

main.o: main.cpp $(servdir)/Server.h
	$(CXX) $(CXXFLAGS) -c main.cpp $(PIGFLAGS) $(CVFLAGS)
