# Makefile for a simple C++ project

CXX = g++
CXXFLAGS = -Iinclude -Wall -Wextra -std=c++11
SOURCES = src/main.cpp
TARGET = build/server

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

