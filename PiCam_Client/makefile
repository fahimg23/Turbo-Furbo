CXX  = g++
CXXFLAGS = -std=c++11 -Wall -g -pthread -I../Client_Server
PIGFLAGS = -lpigpio -lpigpiod_if2 -lrt
CVFLAGS = `pkg-config --cflags --libs opencv4`

#CXXFLAGS: -Wall generates warnings, -g turns on debugging (clearer output)
# and -I tells make to look in current directory for header files

# Object file = compiled source file, generated using -c
# Execuable file = linked object file, generated using -o
# 2-line format for each target (i.e. executable or object file)
# Line 1: dependencies for each target
# Line 2: target generation

socketdir = ../Client_Server

main: main.o Server.o Client.o Camera_Client.o
	$(CXX) $(CXXFLAGS) -o main main.o Server.o Client.o Camera_Client.o $(PIGFLAGS) $(CVFLAGS)

main.o: main.cpp $(socketdir)/Server.h $(socketdir)/Client.h
	$(CXX) $(CXXFLAGS) -c main.cpp $(PIGFLAGS) $(CVFLAGS)

Server.o: $(socketdir)/Server.cpp $(socketdir)/Server.h
	$(CXX) $(CXXFLAGS) -c $(socketdir)/Server.cpp

Client.o: $(socketdir)/Client.cpp $(socketdir)/Server.h $(socketdir)/Client.h
		$(CXX) $(CXXFLAGS) -c $(socketdir)/Client.cpp

Camera_Client.o: $(socketdir)/Camera_Client.cpp $(socketdir)/Server.h $(socketdir)/Client.h $(socketdir)/Camera_Client.h
		$(CXX) $(CXXFLAGS) -c $(socketdir)/Camera_Client.cpp
