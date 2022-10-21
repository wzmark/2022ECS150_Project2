#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

enum{
	STATE_READY,
	STATE_RUNNING,
	STATE_EXIT,
	STATE_BLOCKED
};


struct uthread_tcb {
	int tid;

	uthread_ctx_t *context;
	int threadState; //0 is ready to run, 1 is running, 2 is exit 3 is blocked
	void* stack;
	//yvoid* arg;
};

typedef struct uthread_tcb  uthread_tcb;


typedef struct{
	int threadIdCount;
	struct uthread_tcb *runningThread;
	struct uthread_tcb *mainThread;
	queue_t queueOfBlocked;
	queue_t queueOfZombie;
	queue_t queueOfReady;
}ScheduleController;

ScheduleController scheduleController;

struct uthread_tcb *uthread_current(void)
{
	return scheduleController.runningThread;
}

int ThreadInitialize(uthread_func_t func, void *arg, uthread_tcb* thread){
	thread = (uthread_tcb*)malloc(sizeof(uthread_tcb));
	
	thread->stack = uthread_ctx_alloc_stack();
	if(thread->stack == NULL){
		return -1;
	}
	if(scheduleController.threadIdCount == 0){
		
		thread->threadState = STATE_READY;
	}else{
		//initialize main thread
		thread->threadState = STATE_RUNNING;
	}
	uthread_ctx_init(thread->context, thread->stack, func, arg);
	thread->tid = scheduleController.threadIdCount;
	scheduleController.threadIdCount += 1;
	return 0;
}

void uthread_yield(void)
{
	uthread_ctx_t* context = scheduleController.runningThread->context;
	scheduleController.runningThread->threadState = STATE_READY;
	if(queue_enqueue(scheduleController.queueOfReady, (void*)scheduleController.runningThread) == -1){
		return;
	}

	if(queue_length(scheduleController.queueOfReady) != 0){
		queue_dequeue(scheduleController.queueOfReady, (void**)scheduleController.runningThread);
		scheduleController.runningThread->threadState = STATE_RUNNING;
	}else{
		scheduleController.runningThread = scheduleController.mainThread;
		scheduleController.runningThread->threadState = STATE_RUNNING;
	}
	uthread_ctx_switch(context, scheduleController.runningThread->context);

}

void uthread_exit(void)
{
	uthread_ctx_t* context = scheduleController.runningThread->context;
	scheduleController.runningThread->threadState = STATE_EXIT;
	if(queue_enqueue(scheduleController.queueOfZombie, (void*)scheduleController.runningThread) == -1){
		return;
	}

	if(queue_length(scheduleController.queueOfReady) != 0){
		queue_dequeue(scheduleController.queueOfReady, (void**)scheduleController.runningThread);
		scheduleController.runningThread->threadState = STATE_RUNNING;
	}else{
		scheduleController.runningThread = scheduleController.mainThread;
		scheduleController.runningThread->threadState = STATE_RUNNING;
	}
	uthread_ctx_switch(context, scheduleController.runningThread->context);
}

int uthread_create(uthread_func_t func, void *arg)
{
	uthread_tcb* thread;
	if(ThreadInitialize(func, arg, thread) == -1){
		return -1;
	}
	if(queue_enqueue(scheduleController.queueOfReady, (void*)thread) == -1){
		return -1;
	}
	return 0;
}

int uthread_start(uthread_func_t func, void *arg)
{
	scheduleController.threadIdCount = 0;
	uthread_tcb* thread;
	if(ThreadInitialize(func, arg, thread) == -1){
		return -1;
	}
	scheduleController.mainThread = thread;
	scheduleController.runningThread = thread;
	scheduleController.queueOfBlocked = queue_create();
	scheduleController.queueOfReady = queue_create();
	scheduleController.queueOfZombie = queue_create();
	if(scheduleController.queueOfBlocked == NULL ||
		scheduleController.queueOfReady == NULL ||
		scheduleController.queueOfZombie == NULL){
			return -1;
		}
	return 0;

}

void uthread_block(void)
{
	/* TODO Phase 4 */
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 4 */
	if(uthread->threadState == 1){

	}
}

