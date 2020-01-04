CXX  = g++
CXXFLAGS = -Wall -g -pthread -I../Server

servdir = ../Server
Client.o: Client.cpp $(servdir)/Server.h Client.h
	$(CXX) $(CXXFLAGS) -c Client.cpp
