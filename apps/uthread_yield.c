/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#include <uthread.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)


void thread3(void *arg)
{
	(void)arg;
	//2
	uthread_yield();

	printf("thread3\n");
}

void thread2(void *arg)
{
	(void)arg;
	//1 
	uthread_create(thread3, NULL);
	//1 3
	uthread_yield();
	//2
	printf("thread2\n");
	
}

void thread1(void *arg)
{
	(void)arg;
	// <- 2
	uthread_create(thread2, NULL);
	//2
	uthread_yield();
	//3 2
	printf("thread1\n");

	uthread_yield();
}

int main(void)
{
	uthread_run(false, thread1, NULL);
	
	return 0;
}
