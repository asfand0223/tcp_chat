CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: tcp_chat

tcp_chat: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o tcp_chat

clean:
	rm -f tcp_chat
