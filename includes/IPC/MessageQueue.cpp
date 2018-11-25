#include "MessageQueue.h"

#define STRING_IS_DEFAULT_DATA_TYPE

MessageQueue::MessageQueue(): 
    clear_files(false),
    seed(0),
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
    clear_files(true),
    seed(t_seed),
    max_size(t_max_size),
    filename(t_filename) {
    // File already exists. Do not remove
    if( access( filename.c_str(), F_OK ) != -1 )
        clear_files = false;
    
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
    if ( clear_files ) remove(filename.c_str());
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
    STORAGE_T temp { msgtype, data };
    int status = msgsnd(msqid, &temp, msgsize, 0);
    if ( status < 0 ) {
        perror("msgsnd");
        exit(-1);
    }
}

MessageQueue::DATA_T MessageQueue::recieve(int msgtype) {
    STORAGE_T temp { msgtype, "" };
    int status = msgrcv(msqid, &temp, max_size, msgtype, 0);
    if ( status < 0 ) {
        perror("msgrcv");
        exit(-1);
    }
    return temp.data;
}