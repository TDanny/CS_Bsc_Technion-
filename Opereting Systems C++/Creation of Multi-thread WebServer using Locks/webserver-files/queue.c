#include "queue.h"

 

Queue queueCreate(int max_size){
    Queue q = malloc(sizeof(*q));
    q->size = 0;
    q->head = NULL;
    q->tail = NULL;
    q->max_size = max_size;

    return q;
}

int queueFull(Queue queue){
    if(queue->size == queue->max_size)
        return 1;
    else
        return 0;
}

int queueEmpty(Queue queue){
    if(queue->size == 0)
        return 1;
    else
        return 0;
}

Node nodeCreate(int val, struct timeval arrival){
    Node n = malloc(sizeof(*n));
    n->value = val;
    n->arrival_time = arrival;
    n->next = NULL;

    return n;
}


void enqueue(Queue queue, int value, struct timeval arrival){
    if(queueFull(queue))
        return ;
    Node new_node = nodeCreate(value, arrival);
    if(queueEmpty(queue)){
        queue->head = new_node;
        queue->tail = new_node;
    } else{
        queue->tail->next = new_node;
        queue->tail = new_node;
    }

    queue->size++;
    return ;
}

int dequeue(Queue queue){
    if(queueEmpty(queue))
        return -1;
    Node n = queue->head;
    int tmp_value = n->value;
    queue->head = n->next;
    if(queue->head == NULL){
        queue->tail = NULL;
    } 
    queue->size--;
    free(n);
    return tmp_value;
}

int dequeueByIndex(Queue queue, int index){
    if(queueEmpty(queue))
        return -1;

    if(index < 0 || index >= queueMaxSize(queue))
        return -1;

    if(index == 0){
        return dequeue(queue);
    }

    Node node_to_dequeue = queue->head;
    Node prev_node = NULL;
    for(int i = 0; i < index; i++){
        prev_node = node_to_dequeue;
        node_to_dequeue = node_to_dequeue->next;
    }

    int value = node_to_dequeue->value;
    prev_node->next = node_to_dequeue->next;
    
    if(index == queueCurrSize(queue) - 1){
        queue->tail = prev_node;
    }
    queue->size--;
    free(node_to_dequeue);
    return value;
}

void queueDestroy(Queue queue){
    Node curr = queue->head;
    Node next = NULL;
    while(curr){
        next= curr->next;
        free(curr);
        curr = next;
    }
    free(queue);
}

void queuePrint(Queue queue){
    if (queueEmpty(queue)){
        printf("Queue is empty\n");
        return;
    }

    Node temp = queue->head;
    while (temp){
        printf("%d ", temp->value);
        temp = temp->next;
    }
    printf("\n");
}

Node queueFindByValue(Queue queue , int value){
    Node itr = queue->head;
    while(itr){
        if(itr->value==value)
            return itr;
        itr=itr->next;
    }
    return NULL;
}

int queueFindIndex(Queue queue, int value){
    Node itr = queue->head;
    int index=0;
    while(itr!=NULL && itr->value!=value){
        if(index >= queueCurrSize(queue)){
            return -1;
        }
        itr=itr->next;
        index++;
    }
    return index;
}

int queueCurrSize(Queue queue){
    return queue->size;
}


int queueMaxSize(Queue queue){
    return queue->max_size;
}

Node queueGetHead(Queue queue){
    return queue->head;
}

struct timeval queueHeadArrivaltime(Queue queue){
    Node head =  queue->head;
    return head->arrival_time;
}