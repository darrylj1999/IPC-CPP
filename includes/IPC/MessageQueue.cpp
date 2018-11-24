#include "MessageQueue.h"

#define STRING_IS_DEFAULT_DATA_TYPE

MessageQueue::MessageQueue(): 
    seed( time(NULL) ),
    max_size(200),
    filename("MessageQueue.h") {
    // Getting message queue's key
    key_t msg_key = ftok(filename.c_str(), seed);
    if ( msg_key < 0 ) {
        perror("ftok");
        std::cerr << "ftok called with " << filename << ' ' << seed << std::endl;
        exit(-1);
    }

    // Creating message queue and retrieving id
    msqid = msgget(msg_key, 0666 | IPC_CREAT);
    if ( msqid < 0 ) {
        perror("msgget");
        exit(-1);
    }
}

MessageQueue::MessageQueue(std::string t_filename, int t_max_size, int t_seed):
    seed(t_seed),
    max_size(t_max_size),
    filename(t_filename) {
    // Create file if does not exist
    int fid = creat(filename.c_str(), 0666);
    close(fid);

    // Getting message queue's key
    key_t msg_key = ftok(filename.c_str(), seed);
    if ( msg_key < 0 ) {
        perror("ftok");
        std::cerr << "ftok called with " << filename << ' ' << seed << std::endl;
        exit(-1);
    }

    // Creating message queue and retrieving id
    msqid = msgget(msg_key, 0666 | IPC_CREAT);
    if ( msqid < 0 ) {
        perror("msgget");
        exit(-1);
    }
}

MessageQueue::~MessageQueue() {
    // Deallocating message queue
    msgctl(msqid, IPC_RMID, NULL);
}

void MessageQueue::send(int msgtype, const MessageQueue::DATA_T* data) {
    #ifdef STRING_IS_DEFAULT_DATA_TYPE
        int msgsize = ( data -> length() ) + 1;
        send(msgtype, data, msgsize);
    #else
        int msg_size = sizeof MessageQueue::DATA_T;
        #warning "String is not being used as default data type!"
    #endif
}

void MessageQueue::send(int msgtype, const MessageQueue::DATA_T* data, int msgsize) {
    STORAGE_T temp { msgtype, *data };
    int status = msgsnd(msqid, &temp, msgsize, 0);
    if ( status < 0 ) {
        perror("msgsnd");
        exit(-1);
    }
}

MessageQueue::DATA_T MessageQueue::recieve(int msgtype) {
    void* temp_ptr = nullptr;
    int status = msgrcv(msqid, temp_ptr, max_size, msgtype, 0);
    if ( status < 0 ) {
        perror("msgrcv");
        exit(-1);
    }
    STORAGE_T* temp = (STORAGE_T*) temp_ptr;
    return temp -> data;
}