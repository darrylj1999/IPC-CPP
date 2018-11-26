CC=g++
FLAGS=-g -Wall

all: kirk spock

KernelSemaphore.o: KernelSemaphore.cpp KernelSemaphore.h
	$(CC) $(FLAGS) -c KernelSemaphore.cpp

SharedMemory.o: SharedMemory.cpp SharedMemory.h
	$(CC) $(FLAGS) -c SharedMemory.cpp

kirk: KernelSemaphore.o SharedMemory.o kirk.cpp
	$(CC) $(FLAGS) kirk.cpp SharedMemory.o KernelSemaphore.o -o kirk

spock: KernelSemaphore.o SharedMemory.o spock.cpp
	$(CC) $(FLAGS) spock.cpp SharedMemory.o KernelSemaphore.o -o spock
