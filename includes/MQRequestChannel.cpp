/* 
    File: requestchannel.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2012/07/11

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

#include "MQRequestChannel.h"

#define MAX_MESSAGE 255
#define DEFAULT_SEED 10

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

MQRequestChannel::MQRequestChannel(const std::string _name, const Side _side) :
RequestChannel(_name, _side), mq(_name, MAX_MESSAGE, DEFAULT_SEED)
{
	/* if (_side == ::SERVER_SIDE) {
	}
	else {
	} */ // I may not need to distinguish between server and client side
}

MQRequestChannel::~MQRequestChannel() {
}

string MQRequestChannel::cread() {
    string result("");
    if (my_side == ::SERVER_SIDE)
        result = mq.recieve(2);
    else 
        result = mq.recieve(1);
    // std::cout << side_name << ' ' << my_name << ' ' << mq.msqid << " read " << result << std::endl;
    return result;
}

int MQRequestChannel::cwrite(string msg) {
    if ( msg.length() > MAX_MESSAGE ) {
        ::EXITONERROR("cwrite " + msg + " failed");
        return -1;
    }
    if (my_side == ::SERVER_SIDE)
        mq.send(1, msg);
    else 
        mq.send(2, msg);
    // std::cout << side_name << ' ' <<  my_name << ' ' << mq.msqid << " wrote " << msg << std::endl;
    return msg.length() + 1;
}

std::string MQRequestChannel::name() {
	return my_name;
}
