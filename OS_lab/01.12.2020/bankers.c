#include<stdio.h>       /* IO related tasks      */
#include<limits.h>      /* For well-known macros */
#include<pthread.h>     /* Threads related tasks */
#include<semaphore.h>   /* For using locks       */

int rsrc[2];
sem_t mutex;

#define P 5 // #processes
#define R 2 // #resources

// // Declaration of thread condition variable 
// pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 
// // declaring mutex 
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int need[5][2] = {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}};
int maxm[5][2] = {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}};
int allot[5][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
int avail[2] = {1, 1};

int finish[5] = {0, 0, 0, 0, 0};

void print_need() {
	printf("need\n--------\n");
	for(int i = 0; i < P; i++)
		printf("%d %d\n", need[i][0], need[i][1]);
}

void print_allot() {
	printf("allot\n--------\n");
	for(int i = 0; i < P; i++)
		printf("%d %d\n", allot[i][0], allot[i][1]);
}

void print_avail() {
	printf("avail\n--------\n");
	printf("%d %d\n", avail[0], avail[1]);
}

void print_finish() {
	printf("finish\n--------\n");
	printf("%d %d %d %d %d\n", finish[0], finish[1], finish[2], finish[3], finish[4]);
}

// Safety Algorithm
int isSafe() { 

    int count = 0;

    // Safety Algorithm - 1
	int work[R], lFinish[P]; 
    for(int i = 0; i < R; i++) 
        work[i] = avail[i]; 
    for(int i = 0; i < P; i++) {
    	lFinish[i] = finish[i];
    	if(finish[i] == 0)
    		count++;
    } 
    
    while(count) {
    	// Safety Algorithm - 2
	    int pivot  = -1;
	    for(int i = 0; i < P; i++)
	    	if(lFinish[i] == 0 && 
	    		need[i][0] <= work[0] &&
	    		need[i][1] <= work[1]) {
	    		pivot= i;
	    	    break;
	    	}

	    if(pivot == -1)
	    	return 0;

	    // Safety Algorithm - 3
	    work[0] += allot[pivot][0];
	    work[1] += allot[pivot][1];
	    lFinish[pivot] = 1;
	    count--;
    }

    return count == 0 ? 1 : 0;
} 

void arrangeMatr(int param, int id) {
	// mimic as if approved
    allot[id][param]++;
    need[id][param]--;
    avail[param]--;
}

void revertBack(int param, int id) {
	allot[id][param]--;
    need[id][param]++;
    avail[param]++;
}

int *getResource(int param, int id) {

	int approve = 0;
	
	while(approve == 0) {

		sem_wait(&mutex);

		// request <= available
		if(avail[param] > 0) {
		    arrangeMatr(param, id);
		    approve = isSafe();
		    if(approve == 0)
		    	revertBack(param, id);
		}

		sem_post(&mutex);

		// if(approve == 0) {
		// 	printf("P%d has went to sleep\n", id);
		// 	pthread_cond_wait(&cond, &lock);
		// 	printf("P%d has woken up\n", id);
		// }
	}

	// print_need();
 //    print_allot();
 //    print_avail();
 //    print_finish();
	return &rsrc[param];	
}

void releaseResource(int param, int id) {
	sem_wait(&mutex);
	allot[id][param]--;
    need[id][param]++;
    avail[param]++;
    sem_post(&mutex);
    // pthread_cond_signal(&cond);
}

void *thread_func_t1(void *vargp) {
	int *id = (int *)vargp;

	int *a = getResource(0, *id); // requesting for the resource_0;
	printf("P%d : I am holding the resource_0\n", *id);
	for(int i=0; i<INT_MAX; i++);

	int *b = getResource(1, *id); // requesting for the resource_1;
	printf("P%d : I am holding the resource_1\n", *id);
	printf("P%d : I am holding the resource_0 and resource_1\n", *id);

	releaseResource(0, *id);
	printf("P%d : I released the resource_0\n", *id);

	releaseResource(1, *id);
	printf("P%d : I released the resource_1\n", *id);

	finish[*id] = 1;

	// print_need();
 //    print_allot();
 //    print_avail();
 //    print_finish();
}

void *thread_func_t2(void *vargp) {
	int *id = (int *)vargp;

	int *a = getResource(1, *id); // requesting for the resource_0;
	printf("P%d : I am holding the resource_1\n", *id);
	for(int i=0; i<INT_MAX; i++);

	int *b = getResource(0, *id); // requesting for the resource_1;
	printf("P%d : I am holding the resource_0\n", *id);
	printf("P%d : I am holding the resource_0 and resource_1\n", *id);

	releaseResource(0, *id);
	printf("P%d : I released the resource_0\n", *id);
	releaseResource(1, *id);
	printf("P%d : I released the resource_1\n", *id);

	finish[*id] = 1;

	// print_need();
 //    print_allot();
 //    print_avail();
 //    print_finish();
}

int main() {

	int arr[5] = {0, 1, 2, 3, 4};
	pthread_t thread_id[5];

	sem_init(&mutex, 0, 1);

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