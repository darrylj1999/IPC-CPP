# makefile

INCLUDE_DIR=./includes
CHANNEL_OBJS=FIFORequestChannel.o MQRequestChannel.o SHMRequestChannel.o

all: dataserver client

FIFORequestChannel.o: ${INCLUDE_DIR}/FIFORequestChannel.h ${INCLUDE_DIR}/FIFORequestChannel.cpp
	g++ -g -w -Wall -O1 -std=c++11 -c ${INCLUDE_DIR}/FIFORequestChannel.cpp

MQRequestChannel.o: ${INCLUDE_DIR}/MQRequestChannel.h ${INCLUDE_DIR}/MQRequestChannel.cpp
	g++ -g -w -Wall -O1 -std=c++11 -c ${INCLUDE_DIR}/MQRequestChannel.cpp

SHMRequestChannel.o: ${INCLUDE_DIR}/SHMRequestChannel.h ${INCLUDE_DIR}/SHMRequestChannel.cpp
	g++ -g -w -Wall -O1 -std=c++11 -c ${INCLUDE_DIR}/SHMRequestChannel.cpp

BoundedBuffer.o: BoundedBuffer.h BoundedBuffer.cpp
	g++ -g -w -Wall -O1 -std=c++11 -c BoundedBuffer.cpp

Histogram.o: Histogram.h Histogram.cpp
	g++ -g -w -Wall -O1 -std=c++11 -c Histogram.cpp


dataserver: dataserver.cpp ${CHANNEL_OBJS} 
	g++ -g -w -Wall -O1 -std=c++11 -o dataserver dataserver.cpp ${CHANNEL_OBJS} -lpthread #-lrt

client: client.cpp ${CHANNEL_OBJS} BoundedBuffer.o Histogram.o
	g++ -g -w -Wall -O1 -std=c++11 -o client client.cpp ${CHANNEL_OBJS} BoundedBuffer.o Histogram.o -lpthread #-lrt

bufferTest: testBuffer.cpp BoundedBuffer.o
	g++ -g -w -Wall -std=c++11 -o bufferTest testBuffer.cpp BoundedBuffer.o -lpthread

clean:
	rm -rf *.o fifo* dataserver client *.dSYM/ bufferTest
