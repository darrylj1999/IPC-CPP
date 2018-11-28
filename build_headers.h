#ifdef MQREQCHANNEL
    #define VERSION "Message Queues"
    #define RequestChannel MQRequestChannel
#elif defined SHMREQCHANNEL
    #define VERSION "Shared Memory"
    #define RequestChannel SHMRequestChannel
#else
    #define VERSION "Named Pipes"
    #define RequestChannel FIFORequestChannel
#endif
