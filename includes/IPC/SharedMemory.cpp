#include "SharedMemory.h"

inline void ERROR(const char* func) {
    perror(func);
    exit(-1);
}
inline void ERROR(std::string func) { ERROR(func.c_str()); }

inline struct shmid_ds get_shm_info(int shmid) {
    struct shmid_ds shm_info;
    int status = shmctl( shmid, IPC_STAT, &shm_info );
    if ( status < 0 ) ERROR("shmctl (information)");
    return shm_info;
}

SharedMemory::SharedMemory(std::string t_filename, int t_max_size, int t_seed):
    seed(t_seed),
    max_size(t_max_size),
    filename(t_filename),
    empty_sem("sem_empty_" + t_filename, 0),
    full_sem("sem_full_" + t_filename, 1)
    {
    // Creating file if does not exist
    int fid = creat( filename.c_str(), 0666 );
    if (fid < 0) ERROR("creat (" + filename + ")");
    close(fid);

    // Generating key from file
    key_t shmkey = ftok( filename.c_str(), seed );
    if (shmkey < 0) ERROR("ftok (" + filename + ")");

    // Getting a shared memory segment of 'max_size' bytes
    shmid = shmget( shmkey, max_size, 0666 | IPC_CREAT );
    if (shmid < 0) ERROR("shmget (" + filename + ", " + std::to_string(max_size) + ")");

    // Getting pointer to attached segment
    shmptr = (char*) shmat( shmid, (void*) 0, 0 );
    if (shmptr == (char*) -1) ERROR("shmat (" + filename + ")");
}

SharedMemory::~SharedMemory() {
    // Deleting file
    remove(filename.c_str());
    // Detaching shared memory
    int status = shmdt( (void*) shmptr );
    if (status < 0) ERROR("shmdt (" + filename + ")");
    // Deleting if no processes are attached
    struct shmid_ds shm_info = get_shm_info(shmid);
    if ( shm_info.shm_nattch == 0 ) {
        status = shmctl( shmid, IPC_RMID, &shm_info );
        // if (status < 0) ERROR("shmctl (removal)");
    }
}

SharedMemory::DATA_T SharedMemory::pop() {
    // Lock
    empty_sem.P();
    // Copy all 'max_size' bytes to result
    std::string result(shmptr);
    // Unlock
    full_sem.V();
    return result;
}

void SharedMemory::push(SharedMemory::DATA_T input) {
    // Lock
    full_sem.P();
    // Write 'max_size' bytes to shared memory. strncpy fills characters after '\0' byte with zeros
    strncpy( shmptr, input.c_str(), max_size );
    // Unlock
    empty_sem.V();
}