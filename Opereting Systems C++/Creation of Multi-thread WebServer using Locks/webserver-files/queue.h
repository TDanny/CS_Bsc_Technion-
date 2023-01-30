#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct queue *Queue;
typedef struct node *Node;

typedef enum{
    QUEUE_SUCCESS,
    QUEUE_EMPTY,
    QUEUE_OUT_OF_MEMORY,
    QUEUE_NULL_ARG 
} QueueResult;

typedef enum{
    NODE_SUCCESS,
    NODE_OUT_OF_MEMORY,
    NODE_NULL_ARG
} NodeResult;


struct queue{
    int size;
    int max_size;
    Node head;
    Node tail;
};

struct node{
    int value;
    struct timeval arrival_time;
    Node next;
};


Queue queueCreate(int max_size);
int queueFull(Queue queue);
int queueEmpty(Queue queue);
void queueDestroy(Queue queue);
Node nodeCreate(int value, struct timeval arrival);
Node queueGetHead(Queue queue);
int queueCurrSize(Queue queue);
int queueMaxSize(Queue queue);
void enqueue(Queue queue, int value, struct timeval arrival);
int dequeue(Queue queue);
int dequeueByIndex(Queue queue, int index);
Node queueFindByValue(Queue queue , int value);
int queueFindIndex(Queue queue, int value);
void queuePrint(Queue queue);
struct timeval queueHeadArrivaltime(Queue queue);

#endif