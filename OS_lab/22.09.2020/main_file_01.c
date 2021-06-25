#include<stdio.h>
#include<sys/time.h>
struct timeval startread, startcalc, finish, readtime,calctime, overalltime;

int main() {
    
    gettimeofday(&startread, NULL);
    //read input data
    // open the required file
    FILE* fptr = fopen("tmp.txt", "r");

    // read the file to an array
    int arr[100];
    int n, i, t, count;
    count = 0;
    fscanf(fptr, " %d", &n);
    for(i = 0; i < n; i++) {
        fscanf(fptr, " %d", &t);
        arr[i] = t;
    }

    gettimeofday(&startcalc, NULL);
    
    for(i = 0; i < n; i++)
        if(arr[i] == 3) count++;

    gettimeofday(&finish, NULL);

    timersub(&startcalc , &startread, &readtime);
    timersub(&finish , &startcalc, &calctime);
    timersub(&finish , &startread, &overalltime);

    //output result
    printf("%ld\n", readtime.tv_usec);
    printf("%ld\n", calctime.tv_usec);
    printf("%ld\n", overalltime.tv_usec);
}