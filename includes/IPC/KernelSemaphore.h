#ifndef __KERNEL_SEMAPHORE_H__
#define __KERNEL_SEMAPHORE_H__

class KernelSemaphore {
    private :
        /* INTERNAL DATA STRUCTURES */
    public :
        /* CONSTRUCTOR/DESTRUCTOR */
        KernelSemaphore ( int val ) ;
        ~KernelSemaphore ( ) ; // make sure to remove all allocated resources
        void P ( ) ; /* Acquire Lock*/
        void V ( ) ; /* Release Lock */
};

#endif