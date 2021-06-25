// C program to print all primes 
// less than N 
#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h> 

#define false 0
#define true 1

int n; 

// function check whether a number 
// is prime or not 
int isPrime(int n) 
{ 
	// Corner cases 
	if (n <= 1) 
		return false; 
	if (n <= 3) 
		return true; 

	// This is checked so that we can skip 
	// middle five numbers in below loop 
	if (n % 2 == 0 || n % 3 == 0) 
		return false; 

    int i;
	for (i = 5; i * i <= n; i = i + 6) 
		if (n % i == 0 || n % (i + 2) == 0) 
			return false; 

	return true; 
} 

// Function to print primes 
void *printPrime(void *vargp) 
{ 
	int i;
	for (i = 2; i <= n; i++) { 
		if (isPrime(i)) 
			printf ("%d ", i); 
	} 
	printf ("\n");
} 

// Driver Code 
int main() 
{ 
	scanf (" %d", &n);
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, printPrime, NULL);
	pthread_join(thread_id, NULL); 
} 
