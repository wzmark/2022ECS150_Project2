#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

//Basic Node for the double linked list
//elements: data, front node next node
typedef struct{
		void* data;
		struct Node* frontNode;
		struct Node* nextNode;
}Node;

//double linked list for storing thread
//queueSize: current size of queue, dequeue or enqueue will change
//frontNodeInQueue/readNodeInQueue: the first and the last node in the queue
struct queue {
		int queueSize;
		Node* frontNodeInQueue;
		Node* rearNodeInQueue;
};

//check whether current is empty
//queue: passing queue need to check
//return type: -1 is empty 1 is not empty queue
int isQueueEmpty(queue_t queue){
		//check queue size and queue
		if(queue == NULL || queue->queueSize == 0){
				return -1;
		}else{
				return 1;
		}
}

//initialize queue
//return type: queue which created in the function 
queue_t queue_create(void)
{
		//create space for the queue
		queue_t newQueue = (queue_t)malloc(sizeof(struct queue));
		
		//basically initialize all elements in the struct
		newQueue->queueSize = 0;
		newQueue->frontNodeInQueue = (Node*)malloc(sizeof(Node));
		newQueue->rearNodeInQueue = (Node*)malloc(sizeof(Node));

		return newQueue;
}

//remove all the information in the queue
//return type: 0 if successfully remove
int queue_destroy(queue_t queue)
{
		//use while loop to free all the node except the last one
		//start at the first node
		Node* currentNode = queue->frontNodeInQueue;
		while(currentNode != queue->rearNodeInQueue){
				//free current and move target to the next node
				Node* nextNode = (Node*)currentNode->nextNode;
				free(currentNode);
				currentNode = nextNode;
		}
		//free the last one
		currentNode = queue->rearNodeInQueue;
		free(currentNode);
		//reset the queue size
		queue->queueSize = 0;
		free(queue);
		return 0;
}



int queue_enqueue(queue_t queue, void *data)
{
		//special case when queue is empty, need to set data to the last and the first
		if(queue_length(queue) == 0){
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->data = data;
				newNode->nextNode = NULL;
				newNode->frontNode = NULL;
				queue->frontNodeInQueue = newNode;
				queue->rearNodeInQueue = newNode;
				queue->queueSize += 1;
				//not need to continue directly return
				return 0;
		}
		//initialize the new node with argument
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->data = data;
		//front node is the last node of the queue
		newNode->frontNode = (struct Node*)queue->rearNodeInQueue;
		//nextNode do not exist
		newNode->nextNode = NULL;
		//if queue is not empty, put new node into the last part else put into the new
		if(queue->queueSize != 0 && queue->frontNodeInQueue != NULL 
			&& queue->rearNodeInQueue != NULL){
				Node* oldNode = queue->rearNodeInQueue;
				oldNode->nextNode = (struct Node*)newNode;
		}else{
				queue->frontNodeInQueue = newNode;
		}
		
		queue->rearNodeInQueue = newNode;
		//size increase to 1
		queue->queueSize += 1;

		return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
		//check whether queue is empty if empty not need to dequeue
		if(isQueueEmpty(queue) == -1){
				return -1;
		}
		//get first node data
		*data = queue->frontNodeInQueue->data;
		if(queue_length(queue) == 1){
				//reset queue if only one node in the queue and return
				queue->frontNodeInQueue = NULL;
				queue->rearNodeInQueue = NULL;
				queue->queueSize -= 1;
				return 0;
		}
		
		//rebase the front node of the queue
		Node* newFrontNode = (Node*)queue->frontNodeInQueue->nextNode;
		queue->frontNodeInQueue = newFrontNode;
		if(queue_length(queue) != 1){
				queue->frontNodeInQueue->frontNode = NULL;
		}
		//queue size minus one
		queue->queueSize -= 1;

		return 0;
}


int queue_delete(queue_t queue, void *data)
{
		//empty queue not need to delete or data which need to find is empty
		if(isQueueEmpty(queue) == -1 || data == NULL){
				return -1;
		}

		Node* currentNode = queue->frontNodeInQueue;
		//if current queue size is 1, check whether match if match delete and return
		if(queue_length(queue) == 1){
				if(currentNode->data == queue->frontNodeInQueue->data){
						queue->frontNodeInQueue = NULL;
						queue->rearNodeInQueue = NULL;
						free(currentNode);
						queue->queueSize -= 1;
						return 0;
				}
		}

		//use while loop to iterate whole queue
		while(currentNode != queue->rearNodeInQueue){
				if(currentNode->data == data){
						//current node match the data
						Node* previousNode;
						Node* nextNode;
						if(currentNode == queue->frontNodeInQueue){
								//if data locate in the first node of the queue
								//only need to rebase the first node
								nextNode = (Node*)currentNode->nextNode;
								nextNode->frontNode = NULL;
								free(currentNode);
								queue->frontNodeInQueue = nextNode;
						}else if(currentNode == queue->rearNodeInQueue){
								//if data locate in the end node of the queue
								//only need to rebase the last node
								previousNode = (Node*)currentNode->frontNode;
								previousNode->nextNode = NULL;
								free(currentNode);
								queue->rearNodeInQueue = previousNode;
						}else{
								//if data locate in the middle of the queue
								//need to reconnect the previous node and next node
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
				//free for the node locate at the last node of the queue
				if(currentNode->data == queue->rearNodeInQueue){
						Node* previousNode = 
							(Node *)queue->rearNodeInQueue->frontNode;
						queue->rearNodeInQueue = previousNode;
						queue->rearNodeInQueue->nextNode = NULL;
						queue->queueSize -= 1;
				}
		}
		//return -1 if previous code do not succeed
		return -1;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
		//empty queue not need to continue or function which need to use is empty
		if(isQueueEmpty(queue) == -1|| func == NULL){
				return -1;
		}
		//iterate whole queue except last one
		Node* currentNode = queue->frontNodeInQueue;
		while(currentNode != queue->rearNodeInQueue){
				func(queue, currentNode->data);
				currentNode = (Node*)currentNode->nextNode;
		}
		//func the last one
		func(queue, queue->rearNodeInQueue->data);
		return 0;
}

int queue_length(queue_t queue)
{
		//if queue is empty direct return 
		if(queue == NULL){
				return -1;
		}
		//return zero if empty
		if(queue->frontNodeInQueue == NULL && queue->rearNodeInQueue == NULL){
				return 0;
		}
		//direct return the size
		return queue->queueSize;
}

