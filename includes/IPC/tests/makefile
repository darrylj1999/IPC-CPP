FLAGS=-g -std=c++11 -Wall

all: kirk spock

MessageQueue.o: MessageQueue.h MessageQueue.cpp
	g++ $(FLAGS) -c MessageQueue.cpp

kirk: kirk.cpp MessageQueue.o
	g++ $(FLAGS) kirk.cpp MessageQueue.o -o kirk

spock: spock.cpp MessageQueue.o
	g++ $(FLAGS) spock.cpp MessageQueue.o -o spock

clean:
	rm -rf kirk spock MessageQueue.o
