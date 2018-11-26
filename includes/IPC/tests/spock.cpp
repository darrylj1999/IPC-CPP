#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "SharedMemory.h"

int main(void)
{
    SharedMemory a("shm_test", 256, 10);
    
    printf("spock: ready to receive messages, captain.\n");
    std::string last("");

    for(;;) { /* Spock never quits! */
        std::string output = a.read();
        if ( last != output && !output.empty() ) {
            std::cout << "spock: \"" << output << '"' << std::endl;
            last = output;
        }
        if (last == "quit") break;
    }

    return 0;
}