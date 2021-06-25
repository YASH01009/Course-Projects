#include<stdio.h>       /* IO related tasks      */
#include<limits.h>      /* For well-known macros */
#include<pthread.h>     /* Threads related tasks */
#include<semaphore.h>   /* For using locks       */

int rsrc[2];
sem_t avl[2];

int *getResource(int param) {
	sem_wait(&avl[param]);
	return &rsrc[param];
}

void releaseResource(int param) {
	sem_post(&avl[param]);
}

void *thread_func_t1(void *vargp) {
	int *id = (int *)vargp;

	int *a = getResource(0); // requesting for the resource_0;
	printf("P%d : I am holding the resource_0\n", *id);
	for(int i=0; i<INT_MAX; i++);

	int *b = getResource(1); // requesting for the resource_1;
	printf("P%d : I am holding the resource_1\n", *id);
	printf("P%d : I am holding the resource_0 and resource_1\n", *id);

	releaseResource(0);
	printf("P%d : I released the resource_0\n", *id);

	releaseResource(1);
	printf("P%d : I released the resource_1\n", *id);
}

void *thread_func_t2(void *vargp) {
	int *id = (int *)vargp;

	int *a = getResource(1); // requesting for the resource_0;
	printf("P%d : I am holding the resource_1\n", *id);
	for(int i=0; i<INT_MAX; i++);

	int *b = getResource(0); // requesting for the resource_1;
	printf("P%d : I am holding the resource_0\n", *id);
	printf("P%d : I am holding the resource_0 and resource_1\n", *id);

	releaseResource(0);
	printf("P%d : I released the resource_0\n", *id);
	releaseResource(1);
	printf("P%d : I released the resource_1\n", *id);
}

int main() {

	int arr[5] = {0, 1, 2, 3, 4};
	pthread_t thread_id[5];

	sem_init(&avl[0], 0, 1);
	sem_init(&avl[1], 0, 1);

	pthread_create(&thread_id[0], NULL, thread_func_t1, &arr[0]);
	pthread_create(&thread_id[1], NULL, thread_func_t1, &arr[1]);
	pthread_create(&thread_id[2], NULL, thread_func_t2, &arr[2]);
	pthread_create(&thread_id[3], NULL, thread_func_t2, &arr[3]);
	pthread_create(&thread_id[4], NULL, thread_func_t2, &arr[4]);
	
	pthread_join(thread_id[0], NULL);
	pthread_join(thread_id[1], NULL);
	pthread_join(thread_id[2], NULL);
	pthread_join(thread_id[3], NULL);
	pthread_join(thread_id[4], NULL);

	return 0;
}