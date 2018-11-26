# makefile

FLAGS=-g -w -Wall -O1 -std=c++11
INCLUDE_DIR=./includes
CHANNEL_OBJS=FIFORequestChannel.o MQRequestChannel.o SHMRequestChannel.o
IPC_OBJS=MessageQueue.o# KernelSemaphore.o SharedMemory.o

all: dataserver client

MessageQueue.o: $(INCLUDE_DIR)/IPC/MessageQueue.h $(INCLUDE_DIR)/IPC/MessageQueue.cpp
	g++ $(FLAGS) -c $(INCLUDE_DIR)/IPC/MessageQueue.cpp

FIFORequestChannel.o: ${INCLUDE_DIR}/FIFORequestChannel.h ${INCLUDE_DIR}/FIFORequestChannel.cpp
	g++ $(FLAGS) -c ${INCLUDE_DIR}/FIFORequestChannel.cpp

MQRequestChannel.o: ${INCLUDE_DIR}/MQRequestChannel.h ${INCLUDE_DIR}/MQRequestChannel.cpp
	g++ $(FLAGS) -c ${INCLUDE_DIR}/MQRequestChannel.cpp

SHMRequestChannel.o: ${INCLUDE_DIR}/SHMRequestChannel.h ${INCLUDE_DIR}/SHMRequestChannel.cpp
	g++ $(FLAGS) -c ${INCLUDE_DIR}/SHMRequestChannel.cpp

BoundedBuffer.o: BoundedBuffer.h BoundedBuffer.cpp
	g++ $(FLAGS) -c BoundedBuffer.cpp

Histogram.o: Histogram.h Histogram.cpp
	g++ $(FLAGS) -c Histogram.cpp


dataserver: dataserver.cpp ${CHANNEL_OBJS} ${IPC_OBJS}
	g++ $(FLAGS) -o dataserver dataserver.cpp ${CHANNEL_OBJS} ${IPC_OBJS} -lpthread #-lrt

client: client.cpp ${CHANNEL_OBJS} ${IPC_OBJS} BoundedBuffer.o Histogram.o
	g++ $(FLAGS) -o client client.cpp ${CHANNEL_OBJS} ${IPC_OBJS} BoundedBuffer.o Histogram.o -lpthread #-lrt

bufferTest: testBuffer.cpp BoundedBuffer.o
	g++ -g -w -Wall -std=c++11 -o bufferTest testBuffer.cpp BoundedBuffer.o -lpthread

clean:
	rm -rf *.o fifo* dataserver client *.dSYM/ bufferTest
