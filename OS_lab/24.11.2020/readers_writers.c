#include<stdio.h>       /* IO related tasks       */
#include<limits.h>      /* For well-known macros  */
#include<pthread.h>     /* Threads related tasks  */
#include<semaphore.h>   /* Using POSIX semaphores */

int readers_count;
sem_t mutex, w_or_r;

#define INF 3
// INF is mimiced as 3

void *reader(void *vargp) {
	int i;
	for(int i=0; i<INF; i++) {
		sem_wait(&mutex);
		readers_count++;
		if(readers_count == 1)
			sem_wait(&w_or_r);
		sem_post(&mutex);
		int *id = (int *)vargp; 
		printf("Reader%d started...\n", *id);
		int j;
		for(int j=0; j<INT_MAX; j++);
		printf("Reader%d completed...\n", *id);
	    sem_wait(&mutex);
	    readers_count--;
	    if(readers_count == 0)
	    	sem_post(&w_or_r);
	    sem_post(&mutex);
	    for(int j=0; j<INT_MAX; j++);
	}
}

void *writer(void *vargp) {
	int i;
	for(int i=0; i<INF; i++) {
		sem_wait(&w_or_r);
		printf("The writer started...\n");
		int j;
		for(int j=0; j<INT_MAX; j++);
		printf("The writer completed...\n");
	    sem_post(&w_or_r);
	    for(int j=0; j<INT_MAX; j++);
	}
}

int main() {
	// intializing the POSIX semaphores
	sem_init(&mutex, 0, 1);
	sem_init(&w_or_r, 0, 1);

	int arr[6] = {0, 1, 2, 3, 4, 5};
	pthread_t thread_id[6];

	pthread_create(&thread_id[0], NULL, writer, (void *)&arr[0]); // creating Writer

	pthread_create(&thread_id[1], NULL, reader, (void *)&arr[1]); // creating Reader1
	pthread_create(&thread_id[2], NULL, reader, (void *)&arr[2]); // creating Reader1
	pthread_create(&thread_id[3], NULL, reader, (void *)&arr[3]); // creating Reader1
	pthread_create(&thread_id[4], NULL, reader, (void *)&arr[4]); // creating Reader1
	pthread_create(&thread_id[5], NULL, reader, (void *)&arr[5]); // creating Reader1

	pthread_join(thread_id[0], NULL);                             // terminating writer

	pthread_join(thread_id[1], NULL);                             // terminating Reader1
	pthread_join(thread_id[2], NULL);                             // terminating Reader2
	pthread_join(thread_id[3], NULL);                             // terminating Reader3
	pthread_join(thread_id[4], NULL);                             // terminating Reader4
	pthread_join(thread_id[5], NULL);                             // terminating Reader5

	sem_destroy(&mutex);
	sem_destroy(&w_or_r);

	return 0;
}