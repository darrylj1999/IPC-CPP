
#ifndef _FIFO_reqchannel_H_                   
#define _FIFO_reqchannel_H_

#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include "../reqchannel.h"
using namespace std;

class FIFORequestChannel : public RequestChannel {
private:
	/*  The current implementation uses named pipes. */ 

	int wfd;
	int rfd;

	string pipe_name(Mode _mode);
	void create_pipe (string _pipe_name);
	void open_read_pipe(string _pipe_name);
	void open_write_pipe(string _pipe_name);
	
	
public:

	/* -- CONSTRUCTOR/DESTRUCTOR */

	FIFORequestChannel(const string _name, const Side _side);
	/* Creates a "local copy" of the channel specified by the given name. 
	 If the channel does not exist, the associated IPC mechanisms are 
	 created. If the channel exists already, this object is associated with the channel.
	 The channel has two ends, which are conveniently called "SERVER_SIDE" and "CLIENT_SIDE".
	 If two processes connect through a channel, one has to connect on the server side 
	 and the other on the client side. Otherwise the results are unpredictable.

	 NOTE: If the creation of the request channel fails (typically happens when too many
	 request channels are being created) and error message is displayed, and the program
	 unceremoniously exits.

	 NOTE: It is easy to open too many request channels in parallel. In most systems,
	 limits on the number of open files per process limit the number of established
	 request channels to 125.
	*/

	~FIFORequestChannel();
	/* Destructor of the local copy of the bus. By default, the Server Side deletes any IPC 
	 mechanisms associated with the channel. */

	string cread();
	/* Blocking read of data from the channel. Returns a string of characters
	 read from the channel. Returns NULL if read failed. */

	int cwrite(string _msg);
	/* Write the data to the channel. The function returns the number of characters written
	 to the channel. */

	string name();
	/* Returns the name of the request channel. */

	int read_fd();
	/* Returns the file descriptor used to read from the channel. */

	int write_fd();
	/* Returns the file descriptor used to write to the channel. */
};


#endif


