#ifndef __MESSAGE_QUEUE_H__
#define __MESSAGE_QUEUE_H__

#include <time.h>
#include <string>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>

// key_t key = ftok(filename, seed)
// int msqid = msgget(key, permissions)
// int errno = msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
// msgsz does not count the id field

// int errno = msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg)
// msgsz does not count the id field
// msgsz can be the max number of bytes the message can be
// Offers blocking read

// msgctl(msqid, IPC_RMID, NULL); -> Removes message queue

#define MAX_MESSAGE 255

class MessageQueue {
    // Custom Types
    public:
        typedef std::string DATA_T;
        struct STORAGE_T {
            long mtype; 
            char mtext[MAX_MESSAGE];
            STORAGE_T(int type): mtype(type) {};
            STORAGE_T(int type, DATA_T in):
                mtype(type)
            {
                int sz = in.length();
                strncpy(mtext, in.c_str(), sz);
                mtext[sz] = '\0';
            }
            DATA_T get() {
                std::string result(mtext);
                // memset(mtext, 0, MAX_MESSAGE);
                return result;
            }
        };

    // Data Members
    private:
        int seed, msqid, max_size;
        std::string filename;
    
    // Methods
    public:
        MessageQueue(std::string t_filename, int t_max_size, int t_seed);
        ~MessageQueue();

        // Function to create a 'postbox' and return corresponding msgtype
        // int createPostBox(const DATA_T* init_data=nullptr);
        // send 'data' to 'msgtype'
        void send(int msgtype, const DATA_T data);
        void send(int msgtype, const DATA_T data, int msgsize);
        // recieve 'data' from 'msgtype'
        DATA_T recieve(int msgtype);
};

#endif