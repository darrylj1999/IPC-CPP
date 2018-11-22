/*
    Based on original assignment by: Dr. R. Bettati, PhD
    Department of Computer Science
    Texas A&M University
    Date  : 2013/01/31
 */


#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>

#include <sys/time.h>
#include <cassert>
#include <assert.h>

#include <cmath>
#include <numeric>
#include <algorithm>

#include <list>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "./includes/FIFORequestChannel.h"
#include "./includes/MQRequestChannel.h"
#include "./includes/SHMRequestChannel.h"
#include "BoundedBuffer.h"
#include "Histogram.h"
using namespace std;

#define RequestChannel FIFORequestChannel

// Wrapper around request arguments
struct Request {
    Request(std::string t_name, int t_num, BoundedBuffer* t_buf):
        name(t_name),
        num_requests(t_num),
        buf_ptr(t_buf)
    {};

    ~Request() {}

    std::string name;
    int num_requests;
    BoundedBuffer* buf_ptr;
};

struct Worker {
    Worker ( RequestChannel* t_chan, 
        BoundedBuffer* t_buf, 
        BoundedBuffer* t_stat1,
        BoundedBuffer* t_stat2,
        BoundedBuffer* t_stat3 ):
        chan(t_chan),
        buf_ptr(t_buf) {
        pthread_mutex_init(&channel_lock, NULL);
        res_ptr[0] = t_stat1;
        res_ptr[1] = t_stat2;
        res_ptr[2] = t_stat3;
    };

    ~Worker() {
        pthread_mutex_destroy(&channel_lock);
    };

    // Since R/W might access FIFO file at same time as other threads, channel creation must be made thread safe
    std::string channel_create() {
        pthread_mutex_lock(&channel_lock);
        chan -> cwrite("newchannel");
        std::string s = chan -> cread();
        pthread_mutex_unlock(&channel_lock);
        return s;
    }

    // Since only one instance of a worker argument struct is created, I can get away with setting up the lock in here
    pthread_mutex_t channel_lock;
    RequestChannel* chan;
    BoundedBuffer* buf_ptr;
    BoundedBuffer* res_ptr[3];
};

struct Stat {
    Stat ( std::string t_name, Histogram* t_hist, BoundedBuffer* t_stat ):
        name(t_name),
        hist(t_hist),
        stat_ptr(t_stat)
    {};

    ~Stat() {};

    std::string name;
    Histogram* hist;
    BoundedBuffer* stat_ptr;
};

void* request_thread_function(void* arg) {
	/*
		Fill in this function.

		The loop body should require only a single line of code.
		The loop conditions should be somewhat intuitive.

		In both thread functions, the arg parameter
		will be used to pass parameters to the function.
		One of the parameters for the request thread
		function MUST be the name of the "patient" for whom
		the data requests are being pushed: you MAY NOT
		create 3 copies of this function, one for each "patient".
	 */
    Request* req = (Request*) arg;
	for(int i = 0; i < req -> num_requests; i++) {
        req -> buf_ptr -> push(req -> name);
	}
}

void* worker_thread_function(void* arg) {
    /*
		Fill in this function. 

		Make sure it terminates only when, and not before,
		all the requests have been processed.

		Each thread must have its own dedicated
		RequestChannel. Make sure that if you
		construct a RequestChannel (or any object)
		using "new" that you "delete" it properly,
		and that you send a "quit" request for every
		RequestChannel you construct regardless of
		whether you used "new" for it.
     */

    Worker* req = (Worker*) arg;
    std::string s = req -> channel_create();
    RequestChannel* workerChannel = new RequestChannel(s, ::CLIENT_SIDE);

    // Reading until thread hits 'quit'
    while(true) {
        string request = req -> buf_ptr -> pop();
        workerChannel->cwrite(request);

        if(request == "quit") {
            delete workerChannel;
            break;
        }else{
            int idx = -1;

            string response = workerChannel->cread();
	    // TODO change this to a function
            if ( request == "data John Smith" )
                idx = 0;
            else if ( request == "data Jane Smith" )
                idx = 1;
            else if ( request == "data Joe Smith" )
                idx = 2;
            if ( idx == -1 ) std::cout << "BAD " << response << std::endl;
            req -> res_ptr[idx] -> push (response);
        }
    }
}

void* stat_thread_function(void* arg) {
    /*
		Fill in this function. 

		There should 1 such thread for each person. Each stat thread 
        must consume from the respective statistics buffer and update
        the histogram. Since a thread only works on its own part of 
        histogram, does the Histogram class need to be thread-safe????

     */
    Stat* req = (Stat*) arg;
    while (true) {
        string response = req -> stat_ptr -> pop();

        if(response == "quit")
            break;
        else {
            req -> hist -> update (req->name, response);
        }
    }
}


/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
    int n = 100; //default number of requests per "patient"
    int w = 1; //default number of worker threads
    int b = 3 * n; // default capacity of the request buffer, you should change this default
    int opt = 0;
    while ((opt = getopt(argc, argv, "n:w:b:")) != -1) {
        switch (opt) {
            case 'n':
                n = atoi(optarg);
                break;
            case 'w':
                w = atoi(optarg); //This won't do a whole lot until you fill in the worker thread function
                break;
            case 'b':
                b = atoi (optarg);
                break;
		}
    }

    int pid = fork();
	if (pid == 0){
		execl("dataserver", (char*) NULL);
	}
	else {

        cout << "n == " << n << endl;
        cout << "w == " << w << endl;
        cout << "b == " << b << endl;

        RequestChannel* chan = new RequestChannel("control", ::CLIENT_SIDE);
        BoundedBuffer request_buffer(b),
            john_stat_buffer(b/3),
            jane_stat_buffer(b/3),
            joe_stat_buffer(b/3);
		Histogram hist;

        // 3 Request threads, w Worker threads and 3 statistics threads
        pthread_t john_request_thread, 
            jane_request_thread,
            joe_request_thread,
            worker_thread_pool[w],
            john_stat_thread,
            jane_stat_thread,
            joe_stat_thread;

        Request john_request("data John Smith", n, &request_buffer);
        Request jane_request("data Jane Smith", n, &request_buffer);
        Request joe_request("data Joe Smith", n, &request_buffer);

        void* john_request_arg = (void*) &john_request;
        void* jane_request_arg = (void*) &jane_request;
        void* joe_request_arg = (void*) &joe_request;

        Worker worker_request(chan, &request_buffer, &john_stat_buffer, &jane_stat_buffer, &joe_stat_buffer);

        void* worker_request_arg = (void*) &worker_request;

        Stat john_stat("data John Smith", &hist, &john_stat_buffer);
        Stat jane_stat("data Jane Smith", &hist, &jane_stat_buffer);
        Stat joe_stat("data Joe Smith", &hist, &joe_stat_buffer);

        void* john_stat_arg = (void*) &john_stat;
        void* jane_stat_arg = (void*) &jane_stat;
        void* joe_stat_arg = (void*) &joe_stat;

        std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

        // Starting all threads at the same time
        pthread_create(&john_request_thread, NULL, request_thread_function, john_request_arg);
        pthread_create(&jane_request_thread, NULL, request_thread_function, jane_request_arg);
        pthread_create(&joe_request_thread, NULL, request_thread_function, joe_request_arg);
        for ( int i = 0; i < w; i++ ) pthread_create(&worker_thread_pool[i], NULL, worker_thread_function, worker_request_arg);
        pthread_create(&john_stat_thread, NULL, stat_thread_function, john_stat_arg);
        pthread_create(&jane_stat_thread, NULL, stat_thread_function, jane_stat_arg);
        pthread_create(&joe_stat_thread, NULL, stat_thread_function, joe_stat_arg);

        pthread_join(john_request_thread, NULL);
        pthread_join(jane_request_thread, NULL);
        pthread_join(joe_request_thread, NULL);

        std::cout << "Requests pushed.." << std::endl;

        // Pushing quits to workers once requests have all been pushed
        for ( int i = 0; i < w; ++i ) {
            request_buffer.push("quit");
        }

        for ( int i = 0; i < w; i++ ) pthread_join(worker_thread_pool[i], NULL) ;

        std::cout << "Workers done..." << std::endl;

        // Pushing quits to stat buffers once workers are done
        john_stat_buffer.push("quit");
        jane_stat_buffer.push("quit");
        joe_stat_buffer.push("quit");

        pthread_join(john_stat_thread, NULL);
        pthread_join(jane_stat_thread, NULL);
        pthread_join(joe_stat_thread, NULL);

        std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end_time - start_time;

        std::cout << "Stats gathered...." << std::endl;

        chan->cwrite ("quit");
        delete chan;
        cout << "All Done!!!" << endl; 

		hist.print ();

        std::cout << std::setprecision( std::numeric_limits<double>::max_digits10 );
        std::cout << "Runtime (s) = " << diff.count() << std::endl;
    }
}
