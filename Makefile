LIBS = -lrt -lpthread -lm -ldl
#CFLAGS += $(LIBS) -fpermissive
CFLAGS := $(CFLAGS) -pthread
LDFLAGS=$(LIBS) 
CPPFLAGS = $(CFLAGS) -std=c++11 -pthread

TARGET := enc-server
CXX = g++
CC = gcc
 

OBJECTS = tcp-util.o circular-buffer.o


all: tcp-server tcp-client


tcp-client: $(OBJECTS) tcp-client.o
tcp-server: $(OBJECTS) tcp-server.o


%.o: %.cpp
	$(CXX) -c $<



clean:
	rm -rf *.o *.gch


