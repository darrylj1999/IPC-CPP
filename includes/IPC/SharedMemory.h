#ifndef __SHARED_MEMORY_H__
#define __SHARED_MEMORY_H__

#include "string"
#include "fcntl.h"
#include "unistd.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <iostream>
#include "KernelSemaphore.h"

// Get (or create) Shared Memory segment
// key = ftok(char* filename, int id)
// size = size (in bytes) of shared memory segment
// shmflag = 0666 | IPC_CREAT = Create if doesn't exist and rw-rw-rw-
// int shmid = shmget(key_t key, size_t size, int shmflg);

// Attach Shared Memory segment
// shmid = shared memory ID you got from the call to shmget() 
// shmaddr = used to tell shmat() which specific address to use but you should just set it to 0 and let the OS choose the address for you 
// shmflg = can be set to SHM_RDONLY if you only want to read from it, 0 otherwise.
// void* data = shmat(int shmid, void *shmaddr, int shmflg);

// Detach Shared Memory segment
// int shmdt(data);

// Remove Shared Memory segment
// shmctl(shmid, IPC_RMID, NULL);

class SharedMemory {
    // Custom types
    public:
        typedef std::string DATA_T;

    // Data Members
    private:
        char* shmptr;
        int seed, shmid, max_size;
        std::string filename;
        KernelSemaphore empty_sem, full_sem;

    // Methods
    public:
        SharedMemory(std::string t_filename, int t_max_size, int t_seed);
        ~SharedMemory();

        // Read from shared memory
        DATA_T pop();
        // Write to shared memory
        void push(DATA_T input);
};

#endif