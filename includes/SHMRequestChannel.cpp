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

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

SHMRequestChannel::SHMRequestChannel(const std::string _name, const Side _side) :
RequestChannel(_name, _side)
{
	if (_side == ::SERVER_SIDE) {
	}
	else {
	}
}

SHMRequestChannel::~SHMRequestChannel() {
}

const int MAX_MESSAGE = 255;

string SHMRequestChannel::cread() {
}

int SHMRequestChannel::cwrite(string msg) {
}

std::string SHMRequestChannel::name() {
	return my_name;
}
