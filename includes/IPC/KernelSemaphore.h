#ifndef __KERNEL_SEMAPHORE_H__
#define __KERNEL_SEMAPHORE_H__

#include "string"
#include "fcntl.h"
#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include "sys/sem.h"
#include <sys/types.h>

// Get a set of 'nsems' ( between 500 and 2000 ) semaphores
// Key can be generated with ftok
// key_t key = ftok(filename, seed)
// 'semflg' to create set if does not exist : 0666 | IPC_CREAT
// int semid = semget(key_t key, int nsems, int semflg);

// Atomic operations on semaphores
// 'semnum' is id of semaphore you want to manipulate (index starting from 0)
// 'cmd' -> "what you wish to do with the semaphore set in question -> IPC_RMID, IPC_STAT, SETVAL, GETVAL, SETALL, GETALL
// int semctl(int semid, int semnum, int cmd, *argv);


/* struct sembuf {
    ushort sem_num; -> number of semaphore you wish to interact with
    short sem_op; -> '''
        Zero = Process waits until semaphore becomes zero
        Positive = Release resources. The value of sem_op is added to the semaphore's value.
        Negative = Allocate resources. Block the calling process until the value of the semaphore is 
                   greater than or equal to the absolute value of sem_op. (That is, wait until enough 
                   resources have been freed by other processes for this one to allocate.) Then add 
                   (effectively subtract, since it's negative) the value of sem_op to the semaphore's value.
    '''
    short sem_flg;
}; */
// 'sops' can be an array of sembufs. You can indicate the number of items in this array with 'nsops'
// int semop(int semid, struct sembuf *sops, unsigned int nsops);

class KernelSemaphore {
    private:
        /* INTERNAL DATA STRUCTURES */
        int semid, val;
        std::string filename;
    public:
        /* CONSTRUCTOR/DESTRUCTOR */
        KernelSemaphore(std::string, int);
        ~KernelSemaphore(); // make sure to remove all allocated resources
        void P(); /* Acquire Lock*/
        void V(); /* Release Lock */
        int getval(); /* Get current value of semaphore */
};

#endif