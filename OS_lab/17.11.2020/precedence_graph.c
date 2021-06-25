#include<stdio.h>       /* IO related tasks      */
#include<limits.h>      /* For well-known macros */
#include<pthread.h>     /* Threads related tasks */

void *thread_func(void *vargp) {
	int *myid = (int *)vargp;
	int i;
	// added so, this takes more time slices
	// during scheduling and random execution
	// of order is noticed more clearly
	for(int i=0; i<INT_MAX; i++);
	printf("P%d is executing...\n", *myid);
}

int main() {
	int arr[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	pthread_t thread_id[8];

	pthread_create(&thread_id[1], NULL, thread_func, (void *)&arr[1]); // creating P1
	pthread_join(thread_id[1], NULL);                                  // terminating P1

    // before P2, P3 are created, P1's termination is ensured with pthread_join
	pthread_create(&thread_id[2], NULL, thread_func, (void *)&arr[2]); // creating P2
	pthread_create(&thread_id[3], NULL, thread_func, (void *)&arr[3]); // creating P3

	pthread_join(thread_id[2], NULL);                                  // terminating P2

    // before P4 is created, P2's termination is ensured with pthread_join
	pthread_create(&thread_id[4], NULL, thread_func, (void *)&arr[4]); // creating P4
	pthread_join(thread_id[4], NULL);                                  // terminating P4

    // before P5, P6 are created, P4's termination is ensured with pthread_join
	pthread_create(&thread_id[5], NULL, thread_func, (void *)&arr[5]); // creating P5
	pthread_create(&thread_id[6], NULL, thread_func, (void *)&arr[6]); // creating P6

	pthread_join(thread_id[3], NULL);                                  // terminating P3
	pthread_join(thread_id[5], NULL);                                  // terminating P5
	pthread_join(thread_id[6], NULL);                                  // terminating P6

    // before P4 is created, P2's termination is ensured with pthread_join
	pthread_create(&thread_id[7], NULL, thread_func, (void *)&arr[7]); // creating P7
	pthread_join(thread_id[7], NULL);                                  // terminating P7
}