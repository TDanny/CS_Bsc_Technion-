#include "segel.h"
#include "request.h"
#include "queue.h"

// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//


//globals saved on HEAP

void* thread_function(void* args);

pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_cond_t cond_block;

Queue wait_queue = NULL;
Queue worker_queue = NULL;

int *static_threads;
int *dynamic_threads;
int *total_threads;

// HW3: Parse the new arguments too
void getargs(int *port, int *thread_number, int *queue_size, char *policy, int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <port_number>\n", argv[0]);
        exit(1);
    }
    *port = atoi(argv[1]);
    *thread_number = atoi(argv[2]);
    *queue_size = atoi(argv[3]);
    strcpy(policy, argv[4]);
}


int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, threads_number, queue_max_size;
    struct sockaddr_in clientaddr;
    char policy[10];
    getargs(&port, &threads_number, &queue_max_size, policy, argc, argv);
    srand(time(NULL));


    // queues create
    wait_queue = queueCreate(queue_max_size);   // after implementation of queue
    worker_queue = queueCreate(queue_max_size); // after implementation of queue

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&cond_block, NULL);
       
    static_threads = malloc(sizeof(int)*threads_number);
    dynamic_threads = malloc(sizeof(int)*threads_number);
    total_threads = malloc(sizeof(int)*threads_number);
    for(int i =0; i < threads_number; i++){
        static_threads[i] = 0;
        dynamic_threads[i] = 0;
        total_threads[i] = 0;
    }

    // threads create
    pthread_t* threads = malloc(sizeof(threads)*threads_number);
    for(int i = 0; i< threads_number; i++){
        int* index = malloc(sizeof (int)*1);
        index[0]=i;
        pthread_create(&threads[i], NULL, thread_function, (void *)index);
    }


    /*
    pthread_t *threads = malloc(sizeof(pthread_t)*threads_number);
    int i,rc;

    for (i = 0; i < threads_number; ++i){
        int thread_args[1];
        thread_args[0]=i;
        //printf("Creating thread %d\n", i);
        rc = pthread_create(&threads[i], NULL, thread_function, (void *)thread_args);

    }
    */
    listenfd = Open_listenfd(port);
    while(1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *)&clientlen);
        struct timeval arrival;
        gettimeofday(&arrival, NULL);

        pthread_mutex_lock(&mutex);  //we are going to  work on  the jobs in the wait_queue
        if(queueCurrSize(worker_queue) + queueCurrSize(wait_queue) == queue_max_size){ 
            //max requests achived

            //policy block
            if(strcmp(policy, "block") == 0){
                while (queueCurrSize(worker_queue) + queueCurrSize(wait_queue) == queue_max_size){
                    pthread_cond_wait(&cond_block, &mutex); //dont get other requests just wait until there is space
                }
            }

            // policy drop_tail
            else if(strcmp(policy, "dt") == 0){
                Close(connfd);
                pthread_mutex_unlock(&mutex);
                continue;
            }

            // policy drop_head
            else if(strcmp(policy, "dh") == 0){
                if (queueEmpty(wait_queue)){
                    Close(connfd);
                    pthread_mutex_unlock(&mutex);
                    continue;
                }
                else{
                    int fd = dequeue(wait_queue);
                    Close(fd);
                }
            }

            // policy drop_random
            else if(strcmp(policy, "random") == 0){
                if(queueEmpty(wait_queue)){
                    Close(connfd);
                    pthread_mutex_unlock(&mutex);
                    continue;
                }
                else{
                    int reqeusts_to_drop = (int)((queueCurrSize(wait_queue) + 1) / 2);
                    for (int i = 0; i < reqeusts_to_drop; i++){
                        //if(queueEmpty(wait_queue))
                        //    break;
                        int index_to_remove = rand() % queueCurrSize(wait_queue); //got it from the web because there is no example in output.c
                        int fd = dequeueByIndex(wait_queue, index_to_remove);
                        Close(fd);
                    }
                }
            }

        }


        enqueue(wait_queue, connfd, arrival);
        pthread_cond_signal(&cond);  //send to threads , continue work
        pthread_mutex_unlock(&mutex);
    }
}

void* thread_function(void* args){
    int thread_index = ((int *)args)[0];

    while(1){  //makes the thread not to die
        pthread_mutex_lock(&mutex);
        while(queueEmpty(wait_queue)){
            pthread_cond_wait(&cond, &mutex);  // mutex unlocked and wait for cond and then waiting for mutex
                                              //pthread_cond_wait prevents busy wait
        }

        //mutex locked
        struct timeval arrival = queueHeadArrivaltime(wait_queue);
        struct timeval handle;
        gettimeofday(&handle, NULL);

        struct timeval diffrence;
        timersub(&handle, &arrival, &diffrence);  //calculate the differnce time between arraivl and handle

        int fd = dequeue(wait_queue);  //the job to do , and prevvent from  other threads to get it
        enqueue(worker_queue, fd, arrival);

        pthread_mutex_unlock(&mutex);
        //mutex unlocked

        requestHandle(fd, arrival, diffrence , thread_index , static_threads, dynamic_threads , total_threads);
        Close(fd);
        
        pthread_mutex_lock(&mutex);
        //mutex locked

        int job_index = queueFindIndex(worker_queue, fd);
        dequeueByIndex(worker_queue, job_index);
        
        pthread_cond_signal(&cond_block);
        pthread_mutex_unlock(&mutex); 
        //mutex unlocked
        
    }

}

    


 
