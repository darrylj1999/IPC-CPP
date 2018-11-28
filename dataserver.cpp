#include <cassert>
#include <cstring>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include "./includes/FIFORequestChannel.h"
#include "./includes/MQRequestChannel.h"
#include "./includes/SHMRequestChannel.h"
#include <pthread.h>
using namespace std;

// Convention (type of channel)
// 0 -> FIFO
// 1 -> MQ
// 2 -> SHM
int type_of_channel = 0;
// Helper function to get new channel based on runtime necessity
inline RequestChannel* get_new_channel(int TYPE, std::string _name, Side _side) {
    if ( TYPE == 0 ) return new FIFORequestChannel(_name, _side);
    else if ( TYPE == 1 ) return new MQRequestChannel(_name, _side);
    else if ( TYPE == 2)  return new SHMRequestChannel(_name, _side);
    else return NULL;
}

int nchannels = 0;
pthread_mutex_t newchannel_lock;
void* handle_process_loop (void* _channel);

void process_newchannel(RequestChannel* _channel) {
	nchannels ++;
	string new_channel_name = "data" + to_string(nchannels) + "_";
	_channel->cwrite(new_channel_name);
	// RequestChannel * data_channel = new RequestChannel(new_channel_name, SERVER_SIDE);
	RequestChannel* data_channel = get_new_channel( type_of_channel, new_channel_name, SERVER_SIDE );
	pthread_t thread_id;
	if (pthread_create(& thread_id, NULL, handle_process_loop, data_channel) < 0 ) {
		perror("pthread_create");
		exit(-1);
	}
	
}

void process_request(RequestChannel* _channel, string _request) {

	if (_request.compare(0, 5, "hello") == 0) {
		_channel->cwrite("hello to you too");
	}
	else if (_request.compare(0, 4, "data") == 0) {
		usleep(1000 + (rand() % 5000));
		_channel->cwrite(to_string(rand() % 100));	
	}
	else if (_request.compare(0, 10, "newchannel") == 0) {
		process_newchannel(_channel);
	}
	else {
		// _channel->cwrite("UNREQ " + _request); // ("unknown request");
	}
}

void* handle_process_loop (void* _channel) {
	RequestChannel* channel = (RequestChannel *) _channel;
	for(;;) {
		string request = channel->cread();
		// std::cout << "Dataserver recieved " << request << std::endl;
		if ( ( request.compare("quit") == 0 ) || request.empty() || (request == "\n") ) {
			break;                  // break out of the loop;
		}
		process_request(channel, request);
	}
}


/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/


int main(int argc, char * argv[]) {
	newchannel_lock = PTHREAD_MUTEX_INITIALIZER;
	// RequestChannel control_channel("control", SERVER_SIDE);
	RequestChannel* control_channel = get_new_channel( type_of_channel, "control", SERVER_SIDE );
	handle_process_loop (control_channel);	
}

