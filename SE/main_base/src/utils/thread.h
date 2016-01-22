#ifndef __THREAD_H
#define __THREAD_H

#include <pthread.h>

using namespace std;

class Thread{
public:

	Thread();
	~Thread();

	void start();
	void start(void*(*function)(void*), void *data=NULL);
	static void * ThreadedFunction(void * dt);
	virtual void run();
	void wait();

	bool end;
	
protected:
	pthread_t thread;
	void * ReturnValue;
};



#endif