#ifndef BoundedBuffer_h
#define BoundedBuffer_h

#include <stdio.h>
#include <queue>
#include <string>
#include <iomanip>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
using namespace std;

class BoundedBuffer {
private:
    int capacity;
	queue<string> q;
    pthread_mutex_t buffer_lock, size_lock;
    pthread_cond_t buffer_is_empty, buffer_is_full;
public:
    BoundedBuffer(int);
	~BoundedBuffer();
	int size();
    void push (string);
    string pop();
};

#endif /* BoundedBuffer_ */
