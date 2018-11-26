#include <iostream>
#include <unistd.h>
#include "KernelSemaphore.h"

int main() {
	KernelSemaphore a("kernel_semaphore.txt", 1);
	if ( !fork() ) { // Child
		a.P();
		std::cout << "Child got the lock" << std::endl;
	}
	else { // Parent
		// Acquire
		a.P();
		std::cout << "Parent acquired the lock. Going to sleep" << std::endl;
		// 5 seconds
		usleep(5e6);
		// Release
		a.V();
		usleep (5e6);
	}
}
