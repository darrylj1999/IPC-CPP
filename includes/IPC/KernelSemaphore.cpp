#include "KernelSemaphore.h"

#define DEFAULT_KERNEL_SEMAPHORE_SEED 20

inline void ERROR(const char* func) {
    perror(func);
    exit(-1);
}

KernelSemaphore::KernelSemaphore(std::string t_filename, int t_val):
    val(t_val),
    filename(t_filename) {
    // File does not exist, create it
    if( access( filename.c_str(), F_OK ) == -1 ) {    
        int fid = creat( filename.c_str(), 0666 );
        if (fid < 0) ERROR("creat (creating key file)");
        close(fid);
    }

    // Get one semaphore
    key_t semkey = ftok(filename.c_str(), DEFAULT_KERNEL_SEMAPHORE_SEED);
    semid = semget( semkey, 1, 0666 | IPC_CREAT );
    if (semid < 0) ERROR("semget (getting semid)");

    // Give it an initial value
    union semun semopts;
    semopts.val = val;
    int status = semctl( semid, 0, SETVAL, semopts );
    if (status < 0) ERROR("semctl (setting initial value)");
}

KernelSemaphore::~KernelSemaphore() {
    remove(filename.c_str());
    int status = semctl( semid, 0, IPC_RMID, NULL );
    // if (status < 0) ERROR("semctl (removing semaphore)"); // Sometimes the semaphore has already been deallocated
}

// Acquire lock (decrement)
void KernelSemaphore::P() {
    struct sembuf sop;
    sop.sem_num = 0;
    sop.sem_op = -1;
    sop.sem_flg = 0;
    int status = semop( semid, &sop, 1 );
    /* if (status < 0) ERROR("semop (P)"); */
    val--;
}

// Release lock (increment)
void KernelSemaphore::V() {
    struct sembuf sop;
    sop.sem_num = 0;
    sop.sem_op = 1;
    sop.sem_flg = IPC_NOWAIT;
    int status = semop( semid, &sop, 1 );
    /* if (status < 0) ERROR("semop (V)"); */
    val++;
}

int KernelSemaphore::getval() {
    return val;
}