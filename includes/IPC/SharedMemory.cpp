#include "SharedMemory.h"

inline void ERROR(const char* func) {
    perror(func);
    exit(-1);
}
inline void ERROR(std::string func) { ERROR(func.c_str()); }

SharedMemory::SharedMemory(std::string t_filename, int t_max_size, int t_seed):
    seed(t_seed),
    max_size(t_max_size),
    filename(t_filename),
    shmmutex(t_filename + "_sem", 1)
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
}

SharedMemory::DATA_T SharedMemory::read() {
    // Lock
    shmmutex.P();
    // Copy all 'max_size' bytes to result
    char* result_c_str = new char[max_size];
    strncpy( result_c_str, shmptr, max_size );
    std::string result(result_c_str);
    delete[] result_c_str;
    // Unlock
    shmmutex.V();
    return result;
}

void SharedMemory::write(SharedMemory::DATA_T input) {
    // Lock
    shmmutex.P();
    // Write 'max_size' bytes to shared memory. strncpy fills characters after '\0' byte with zeros
    strncpy( shmptr, input.c_str(), max_size );
    // Unlock
    shmmutex.V();
}