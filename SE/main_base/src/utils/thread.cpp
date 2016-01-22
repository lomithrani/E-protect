#include "thread.h"

Thread::Thread(){

}

Thread::~Thread(){

}

void Thread::run(){

}

void Thread::start(){
	pthread_create(&thread, NULL, &Thread::ThreadedFunction, (void *)this);
}

void Thread::start(void*(*function)(void*), void *data){
	pthread_create(&thread, NULL, function, data);
}

void * Thread::ThreadedFunction(void * dt){
	Thread *t = (Thread*)dt;
	t->run();
	t->end = true;
	return NULL;
}

void Thread::wait(){
	pthread_join(thread,&ReturnValue);
}