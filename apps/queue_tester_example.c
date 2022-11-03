#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

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

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	int data1 = 5;
	int data2 = 6, *ptr2;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data); // queue <- data0
	queue_enqueue(q, &data1); //queue(data0) <- data1
	queue_enqueue(q, &data2); //queue(data0 data1) <- data 2
	queue_dequeue(q, (void**)&ptr); //queue(data0 data1 data2) -> data0
	TEST_ASSERT(ptr == &data); //test equal
	
	queue_enqueue(q, &data); //queue(data1 data2) <- data 0
	queue_delete(q, &data1); //del data1  queue(data1 data2 data0)
	queue_dequeue(q, (void**)&ptr2); //queue(data2 data0) -> data2
	TEST_ASSERT(ptr2 == &data2); //test equal
	queue_delete(q, &data1); //del data0 queue(data0)
								//empty
	
	
}

int main(void)
{
	test_create();
	test_queue_simple();

	return 0;
}
