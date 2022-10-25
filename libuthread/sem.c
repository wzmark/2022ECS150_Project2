#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include "queue.h"
#include "sem.h"
#include "private.h"
//#include "uthread.c"


struct uthread_tcb {
	int tid;

	uthread_ctx_t context;
	int threadState; //0 is ready to run, 1 is running, 2 is exit 3 is blocked
	void* stack;
	
};

typedef struct uthread_tcb  uthread_tcb;

struct semaphore {
	int maxCount;
	int currentNumberOfThread;
	int lock;
	queue_t queue;

	/* TODO Phase 3 */
};
struct semaphore* semaphore;

sem_t sem_create(size_t count)
{
	semaphore = (struct semaphore*)malloc(sizeof(semaphore));
	semaphore->maxCount = count;
	semaphore->currentNumberOfThread = 0;
	semaphore->queue = queue_create();
	return semaphore;
}



//typedef struct uthread_tcb  uthread_tcb;
int sem_destroy(sem_t sem)
{
	sem->currentNumberOfThread = 0;
	sem->maxCount = 0;
	
	return 0;
}

//wait
int sem_down(sem_t sem)
{
	if(sem == NULL){
		return -1;
	}
	
	sem->currentNumberOfThread -= 1;
	//printf("1");
	if(sem->currentNumberOfThread < 0){
		
		struct uthread_tcb* thread = (uthread_tcb*)malloc(sizeof(uthread_tcb));
		thread = uthread_current();
		queue_enqueue(sem->queue, thread);
		uthread_block();
	}
	
	
	
	return 0;
}

//unblock
int sem_up(sem_t sem)
{
		if(sem == NULL){
				return -1;
		}
		sem->currentNumberOfThread += 1;
		if(sem->currentNumberOfThread >= 0 && queue_length(sem->queue) >0){
				uthread_tcb* thread = (uthread_tcb*)malloc(sizeof(uthread_tcb));
				queue_dequeue(sem->queue, (void**)&thread);
				uthread_unblock(thread);
		}
		return 0;
}

