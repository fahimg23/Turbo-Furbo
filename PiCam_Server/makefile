CXX  = g++
CXXFLAGS = -Wall -g -pthread
PIGFLAGS = -lpigpio -lpigpiod_if2 -lrt
CVFLAGS = `pkg-config --cflags --libs opencv4`

#CXXFLAGS: -Wall generates warnings, -g turns on debugging (clearer output)
# and -I tells make to look in current directory for header files

# Object file = compiled source file, generated using -c
# Execuable file = linked object file, generated using -o
# 2-line format for each target (i.e. executable or object file)
# Line 1: dependencies for each target
# Line 2: target generation

main: main.o Server.o Client.o
	$(CXX) $(CXXFLAGS) -o main main.o Server.o Client.o $(PIGFLAGS) $(CVFLAGS)

main.o: main.cpp Server.h Client.h
	$(CXX) $(CXXFLAGS) -c main.cpp $(PIGFLAGS) $(CVFLAGS)

Server.o: Server.cpp Server.h
	$(CXX) $(CXXFLAGS) -c Server.cpp

Client.o: Client.cpp Server.h Client.h
		$(CXX) $(CXXFLAGS) -c Client.cpp
