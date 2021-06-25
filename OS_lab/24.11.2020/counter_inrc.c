#include<stdio.h>       /* IO related tasks      */
#include<limits.h>      /* For well-known macros */
#include<pthread.h>     /* Threads related tasks */

int global; // shared
int prev;   // shared

void *thread_func(void *vargp) {
	unsigned int counter = 0;  // not shared
	int p = -1;
	int *id = (int *) vargp;
	for(int i=0; i<INT_MAX/4; i++) {
		counter += 1;
		global += 1;
		if(prev == -1)
			prev = *id;
 		else if(prev != *id) {
			prev = *id;
			printf("Thread switched...%d\n", i);
		}
	}
	printf("counter : %d\nglobal : %d\nINT_MAX/2 : %d\nINT_MAX/4 : %d\n", counter, global, INT_MAX/2, INT_MAX/4);
}

int main() {
	pthread_t t0, t1;
	int a = 0, b = 1;
	prev = -1;
	pthread_create(&t0, NULL, thread_func, &a);
	pthread_create(&t1, NULL, thread_func, &b);
	pthread_join(t0, NULL);
	pthread_join(t1, NULL);	
}