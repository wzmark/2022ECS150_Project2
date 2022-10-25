#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct{
	void* data;
	struct Node* frontNode;
	struct Node* nextNode;
}Node;

struct queue {
	int queueSize;
	

	Node* frontNodeInQueue;
	Node* rearNodeInQueue;
};

int isQueueEmpty(queue_t queue){
	if(queue == NULL || queue->queueSize == 0){
		return -1;
	}else{
		return 1;
	}
}

queue_t queue_create(void)
{
	queue_t newQueue = (queue_t)malloc(sizeof(struct queue));
	
	newQueue->queueSize = 0;
	newQueue->frontNodeInQueue = (Node*)malloc(sizeof(Node));
	newQueue->rearNodeInQueue = (Node*)malloc(sizeof(Node));


	return newQueue;
}

int queue_destroy(queue_t queue)
{
	
	Node* currentNode = queue->frontNodeInQueue;
	while(currentNode != queue->rearNodeInQueue){
		Node* nextNode = (Node*)currentNode->nextNode;
		free(currentNode);
		currentNode = nextNode;
	}
	queue->queueSize = 0;
	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	if(queue_length(queue) == 0){
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->data = data;
		newNode->nextNode = NULL;
		newNode->frontNode = NULL;
		queue->frontNodeInQueue = newNode;
		queue->rearNodeInQueue = newNode;
		queue->queueSize += 1;

	return 0;
	}
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;
	newNode->frontNode = (struct Node*)queue->rearNodeInQueue;
	newNode->nextNode = NULL;

	if(queue->queueSize != 0 && queue->frontNodeInQueue != NULL && queue->rearNodeInQueue != NULL){
		Node* oldNode = queue->rearNodeInQueue;
		oldNode->nextNode = (struct Node*)newNode;
	}else{
		queue->frontNodeInQueue = newNode;
	}
	

	queue->rearNodeInQueue = newNode;
	queue->queueSize += 1;

	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if(isQueueEmpty(queue) == -1){
		return -1;
	}
	*data = queue->frontNodeInQueue->data;
	if(queue_length(queue) == 1){
		queue->frontNodeInQueue = NULL;
		queue->rearNodeInQueue = NULL;
		queue->queueSize -= 1;

		return 0;
	}
	
	
	Node* newFrontNode = (Node*)queue->frontNodeInQueue->nextNode;
	queue->frontNodeInQueue = newFrontNode;
	if(queue_length(queue) != 1){
		queue->frontNodeInQueue->frontNode = NULL;
	}
	
	

	queue->queueSize -= 1;

	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if(isQueueEmpty(queue) == -1 || data == NULL){
		return -1;
	}
	Node* currentNode = queue->frontNodeInQueue;
	if(queue_length(queue) == 1){
		if(currentNode->data == queue->frontNodeInQueue->data){
			queue->frontNodeInQueue = NULL;
			queue->rearNodeInQueue = NULL;
			queue->queueSize -= 1;
			return 0;
		}
	}

	

	while(currentNode != queue->rearNodeInQueue){
		if(currentNode->data == data){
			Node* previousNode;
			Node* nextNode;
			if(currentNode == queue->frontNodeInQueue){
				nextNode = (Node*)currentNode->nextNode;
				nextNode->frontNode = NULL;
				free(currentNode);
				queue->frontNodeInQueue = nextNode;
			}else if(currentNode == queue->rearNodeInQueue){
				previousNode = (Node*)currentNode->frontNode;
				previousNode->nextNode = NULL;
				free(currentNode);
				queue->rearNodeInQueue = previousNode;
			}else{
				previousNode = (Node*)currentNode->frontNode;
				nextNode = (Node*)currentNode->nextNode;
				previousNode->nextNode = (struct Node*)nextNode;
				nextNode->frontNode = (struct Node*)previousNode;
				free(currentNode);
			}
			queue->queueSize -= 1;
			return 0;
		}
		currentNode = (Node*)currentNode->nextNode;
		if(currentNode->data == queue->rearNodeInQueue){
			Node* previousNode = (Node *)queue->rearNodeInQueue->frontNode;
			queue->rearNodeInQueue = previousNode;
			queue->rearNodeInQueue->nextNode = NULL;
		}
	}
	
	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	if(isQueueEmpty(queue) == -1|| func == NULL){
		return -1;
	}
	Node* currentNode = queue->frontNodeInQueue;
	
	while(currentNode != queue->rearNodeInQueue){
		func(queue, currentNode->data);
		currentNode = (Node*)currentNode->nextNode;
	}
	return 0;
}

int queue_length(queue_t queue)
{
	if(queue == NULL){
		return -1;
	}
	if(queue->frontNodeInQueue == NULL && queue->rearNodeInQueue == NULL){
		return 0;
	}
	return queue->queueSize;
}

