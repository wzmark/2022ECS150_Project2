#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	size_t maxCount;
	size_t currentNumberOfThread;
	//queue_t queue;

	/* TODO Phase 3 */
};

sem_t sem_create(size_t count)
{
	struct semaphore* semaphore;
	semaphore->maxCount = count;
	semaphore->currentNumberOfThread = 0;
	//semaphore->queue = queue_create();
	return semaphore;
}

int sem_destroy(sem_t sem)
{
	sem->currentNumberOfThread = 0;
	sem->maxCount = 0;
	/*
	if(queue_iterate(sem->queue, queue_delete) == -1){
		return -1;
	}
	*/
	return 0;
}

int sem_down(sem_t sem)
{
	if(sem == NULL){
		return -1;
	}
	sem->currentNumberOfThread -= 1;
	return 0;
}

int sem_up(sem_t sem)
{
	if(sem == NULL){
		return -1;
	}
	sem->currentNumberOfThread -= 1;
	return 0;
}

