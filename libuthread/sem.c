#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include "queue.h"
#include "sem.h"
#include "private.h"


//struct to implement to help use this struct in sem.c
struct uthread_tcb {
		int tid;
		uthread_ctx_t context;
		int threadState; //0 is ready to run, 1 is running, 2 is exit 3 is blocked
		void* stack;
	
};


typedef struct uthread_tcb  uthread_tcb;

struct semaphore {
		int maxCount; //max number of count
		int currentNumberOfThread; //current count
		queue_t queue; //queue to store the thread was blocked by this thread
};

struct semaphore* semaphore;

sem_t sem_create(size_t count)
{
		//initialize the sem
		//create memory space and set number
		semaphore = (struct semaphore*)malloc(sizeof(semaphore));
		semaphore->maxCount = count;
		semaphore->currentNumberOfThread = count;
		//queue create
		semaphore->queue = queue_create();
		return semaphore;
}



int sem_destroy(sem_t sem)
{
		//reset back to zero
		sem->currentNumberOfThread = 0;
		sem->maxCount = 0;
		return 0;
}

//wait
int sem_down(sem_t sem)
{
	//if sem is null direct return 
		if(sem == NULL){
				return -1;
		}
		//count minus 1
		sem->currentNumberOfThread -= 1;
		//check whether count is less than 0
		if(sem->currentNumberOfThread < 0){
				//get current thread
				struct uthread_tcb* thread = 
					(uthread_tcb*)malloc(sizeof(uthread_tcb));
				thread = uthread_current();
				//put it back to queue
				queue_enqueue(sem->queue, thread);
				//block current thread
				uthread_block();
		}
		
		
		
		return 0;
}

//unblock
int sem_up(sem_t sem)
{
		//if sem is null direct return 
		if(sem == NULL){
				return -1;
		}
		//count add 1
		sem->currentNumberOfThread += 1;
		if(sem->currentNumberOfThread >= 0 && queue_length(sem->queue) >0){
				//get block thread
				uthread_tcb* thread = (uthread_tcb*)malloc(sizeof(uthread_tcb));
				queue_dequeue(sem->queue, (void**)&thread);
				//unblock this thread
				uthread_unblock(thread);
		}
		return 0;
}

