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

#include "SHMRequestChannel.h"

#define DEFAULT_SHM_MAX_SIZE 256
#define DEFAULT_SHM_SEED 40

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

SHMRequestChannel::SHMRequestChannel(const std::string _name, const Side _side) :
RequestChannel(_name, _side), 
server_shm(_name + "server", DEFAULT_SHM_MAX_SIZE, DEFAULT_SHM_SEED),
client_shm(_name + "client", DEFAULT_SHM_MAX_SIZE, DEFAULT_SHM_SEED)
{}

SHMRequestChannel::~SHMRequestChannel() {}

string SHMRequestChannel::cread() {
    std::string result;
    // Server reads from server
    // Client reads from client
    if (my_side == ::SERVER_SIDE) {
        result = server_shm.pop();
    }
    else {
        result = client_shm.pop();
    }
    return result;
}

int SHMRequestChannel::cwrite(string msg) {
    if (msg.length() > DEFAULT_SHM_MAX_SIZE) return -1;
    // Server writes to client
    // Client writes to server
    if (my_side == ::SERVER_SIDE) {
        client_shm.push(msg);
    }
    else {
        server_shm.push(msg);
    }
    return msg.length() + 1;
}

std::string SHMRequestChannel::name() {
	return my_name;
}
