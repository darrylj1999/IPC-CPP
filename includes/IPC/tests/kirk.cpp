#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include "SharedMemory.h"

#define MAX_SIZE 300

int main(void)
{
    SharedMemory a("shm_test", 256, 10);
    printf("Enter lines of text, ^D to quit:\n");
    std::string input("");
    
    while( std::getline(std::cin, input)) {
        a.write(input);
        if (input == "quit" ) break;
    }

    return 0;
}