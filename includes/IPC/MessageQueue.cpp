#include "MessageQueue.h"

#define STRING_IS_DEFAULT_DATA_TYPE

inline void ERROR(const char* func) {
    perror(func);
    exit(-1);
}
inline void ERROR(std::string func) { ERROR(func.c_str()); }

MessageQueue::MessageQueue(std::string t_filename, int t_max_size, int t_seed):
    seed(t_seed),
    max_size(t_max_size),
    filename(t_filename) {
    // Create file if does not exist
    int fid = creat(filename.c_str(), 0666);
    if ( fid < 0 ) ERROR("creat (" + filename + ")");
    close(fid);

    // Getting message queue's key
    key_t msg_key = ftok(filename.c_str(), seed);
    if ( msg_key < 0 ) ERROR("ftok (" + filename + ")");

    // Creating message queue and retrieving id
    msqid = msgget(msg_key, 0666 | IPC_CREAT);
    if ( msqid < 0 ) ERROR("msgget (" + filename + ")");
}

MessageQueue::~MessageQueue() {
    // Deallocating message queue
    msgctl(msqid, IPC_RMID, NULL);
    remove(filename.c_str());
}

void MessageQueue::send(int msgtype, const MessageQueue::DATA_T data) {
    #ifdef STRING_IS_DEFAULT_DATA_TYPE
        int msgsize = data.length() + 1;
    #else
        #warning "String is not being used as default data type!"
        int msgsize = sizeof MessageQueue::DATA_T;
    #endif
    send(msgtype, data, msgsize);
}

void MessageQueue::send(int msgtype, const MessageQueue::DATA_T data, int msgsize) {
    STORAGE_T temp(msgtype, data);
    // temp.mtype = msgtype;
    // strncpy(temp.mtext, data.c_str(), msgsize-1);
    int status = msgsnd(msqid, &temp, msgsize, 0);
    /*
    ERROR("msgsnd");
    */
}

MessageQueue::DATA_T MessageQueue::recieve(int msgtype) {
    STORAGE_T temp(msgtype);
    // temp.mtype = msgtype;
    int status = msgrcv(msqid, &temp, max_size, msgtype, 0);
    /*
    ERROR("msgrcv");
    */
    std::string result = temp.get();
    return result;
}