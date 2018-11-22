#include "BoundedBuffer.h"
#include <string>
#include <queue>
using namespace std;

BoundedBuffer::BoundedBuffer(int _cap): capacity(_cap) {
	pthread_mutex_init(&buffer_lock, NULL);
	pthread_mutex_init(&size_lock, NULL);
	pthread_cond_init(&buffer_is_empty, NULL);
	pthread_cond_init(&buffer_is_full, NULL);
}

BoundedBuffer::~BoundedBuffer() {
	pthread_mutex_destroy(&buffer_lock);
	pthread_mutex_destroy(&size_lock);
	pthread_cond_destroy(&buffer_is_empty);
	pthread_cond_destroy(&buffer_is_full);
}

int BoundedBuffer::size() {
	pthread_mutex_lock(&size_lock);
	int s = q.size();
	pthread_mutex_unlock(&size_lock);
	return s;
}

void BoundedBuffer::push(string str) {
	/*
	Is this function thread-safe??? Does this automatically wait for the pop() to make room 
	when the buffer if full to capacity???
	*/
	pthread_mutex_lock(&buffer_lock);
	while( q.size() == capacity ) pthread_cond_wait(&buffer_is_full, &buffer_lock);
	q.push (str);
	pthread_cond_signal(&buffer_is_empty);
	// std::cout << str << " pushed successfully! Current Size: " << q.size() << std::endl << std::flush;
	pthread_mutex_unlock(&buffer_lock);
}

string BoundedBuffer::pop() {
	/*
	Is this function thread-safe??? Does this automatically wait for the push() to make data available???
	*/
	pthread_mutex_lock(&buffer_lock);
	while ( q.empty() ) pthread_cond_wait(&buffer_is_empty, &buffer_lock);
	string s = q.front();
	q.pop();
	pthread_cond_signal(&buffer_is_full);
	// std::cout << s << " popped successfully! Current Size: " << q.size() << std::endl << std::flush;
	pthread_mutex_unlock(&buffer_lock);
	return s;
}
