CC=g++
FLAGS=-Wall -std=c++11 -g

all: msgtest

MessageQueue.o: MessageQueue.h MessageQueue.cpp
	$(CC) $(FLAGS) -c MessageQueue.cpp -o MessageQueue.o

msgtest: MessageQueue.o kirk.cpp spock.cpp
	$(CC) $(FLAGS) kirk.cpp MessageQueue.o -o kirk
	$(CC) $(FLAGS) spock.cpp MessageQueue.o -o spock
clean:
	rm -rf kirk spock *.dSYM/ MessageQueue.o
