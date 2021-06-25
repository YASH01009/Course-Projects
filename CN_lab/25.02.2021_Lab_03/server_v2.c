/*
** server.c -- a stream socket calculator(server) demo 
** compilation -- gcc -o server_v2 server_v2.c -lpthread -lrt
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

#define PORT "3490" // the port users will be connecting to
#define BACKLOG 10  // how many pending connections queue will hold
#define MAXDATASIZE 10 // max number of bytes we can get at once

sem_t mutex; // semaphore for signaling between threads

void extract(char *msg, int numbytes, char *op, int *op1, int *op2) 
{
	int a1=0, a2=0, ptr=0;
	char b;
	while(msg[ptr]>='0' && msg[ptr]<='9') a1 = a1*10+msg[ptr]-'0', ptr++;
	b = msg[ptr], ptr++;
	while(ptr<numbytes) a2 = a2*10+msg[ptr]-'0', ptr++;
	*op = b, *op1 = a1, *op2 = a2;
}

// get sockaddr, IPv4:
void *get_in_addr(struct sockaddr *sa)
{
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

void *thread_func(void *vargp)
{
	char msg_send[MAXDATASIZE], msg_recv[MAXDATASIZE], op;
	int op1, op2, res, numbytes;
	int new_fd = *((int *)vargp);
	sem_post(&mutex);
		
	while(1) {
		// receive the question
		if ((numbytes = recv(new_fd, msg_recv, MAXDATASIZE-1, 0)) == -1) {
			perror("recv");
			exit(1);
		}

		msg_recv[numbytes] = '\0';
		if (strcmp(msg_recv, "good bye") == 0) {
			break;
		}
		extract(msg_recv, numbytes, &op, &op1, &op2);

		switch(op) {
			case '+':
				res = op1+op2;
				break;
			case '-':
				res = op1-op2;
				break;
			case '*':
				res = op1*op2;
				break;
			case '/':
				if(op2 == 0) res = 0;
				else res = op1/op2;
				break;
		}
		sprintf(msg_send, "%d", res);

		// send the result
		if ((numbytes = send(new_fd, msg_send, strlen(msg_send), 0)) == -1) {
			perror("send");
			exit(1);
		}
	}

	close(new_fd);
	pthread_exit(NULL);
}

int main(void)
{
	int sockfd, new_fd; // listen on sock_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	int yes = 1;
	int rv;
	char s[INET_ADDRSTRLEN]; // space to hold the client's address
	pthread_t thread_id; // thread_id to handle multiple threads
	sem_init(&mutex, 0, 0); // initializing the semaphore
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // use IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}

	freeaddrinfo(servinfo); // all done with this structure
	
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	printf("Server: waiting for connections...\n");
	
	while(1) {
		sin_size = sizeof their_addr;
		int new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size); // returns a new socket used for comm with client

		if (new_fd == -1) {
			perror("accept");
			exit(1);
		}
		
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
		printf("Server: got connection from %s\n", s);
		pthread_create(&thread_id, NULL, thread_func, (void *)&new_fd);
		sem_wait(&mutex);
	}
	return 0;
}