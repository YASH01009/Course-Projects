#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h> 

// A normal C function that is executed as a thread 
// when its name is specified in pthread_create() 

int n, avg = 0, min = 0, max = 0;
int *arr;

void *calc_avg(void *vargp) 
{ 
	int i;
	for(i = 0; i < n; i++)
		avg += arr[i];
	avg /= n;
} 

void *calc_min(void *vargp)
{
	int i, buff = arr[0];
	for(i = 1; i < n; i++)
		if(arr[i] < buff)
			buff = arr[i];
	min = buff;
}

void *calc_max(void *vargp)
{
	int i, buff = arr[0];
	for(i = 1; i < n; i++)
		if(arr[i] > buff)
			buff = arr[i];
	max = buff;
}

int main() 
{ 
	scanf(" %d", &n);
	pthread_t thread_id0, thread_id1, thread_id2;
	int i; 
	
	arr = (int *)malloc(n * sizeof(int));
	for(i = 0; i<n; i++)
		scanf(" %d", &arr[i]);

	pthread_create(&thread_id0, NULL, calc_avg, NULL);
	pthread_create(&thread_id1, NULL, calc_min, NULL);
	pthread_create(&thread_id2, NULL, calc_max, NULL);

	pthread_join(thread_id0, NULL); 
	pthread_join(thread_id1, NULL); 
	pthread_join(thread_id2, NULL); 

	printf("The average value is %d\n", avg);
	printf("The minimum value is %d\n", min);
	printf("The maximum value is %d\n", max);

	return 0;
}
