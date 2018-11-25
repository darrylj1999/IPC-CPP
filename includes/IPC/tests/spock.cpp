#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "MessageQueue.h"

int main(void)
{
    MessageQueue a("hello_world.txt", 300, 10);
    
    printf("spock: ready to receive messages, captain.\n");

    for(;;) { /* Spock never quits! */
        std::string output = a.recieve(0);
        std::cout << "spock: \"" << output << '"' << std::endl;
    }

    return 0;
}