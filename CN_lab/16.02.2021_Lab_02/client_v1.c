/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT "3490" // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once

// get sockaddr, IPv4:
void *get_in_addr(struct sockaddr *sa)
{
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

void *read_thread(void *vargp) 
{
	int numbytes, new_fd;
	char msg_recv[MAXDATASIZE]; // space to hold the received message
	int *newfd_ptr = (int *)vargp;
	new_fd = *newfd_ptr;

	while(1) {
		// receive the message
		if ((numbytes = recv(new_fd, msg_recv, MAXDATASIZE-1, 0)) == -1)
			perror("recv");
		else {
			msg_recv[numbytes] = '\0';
			for(int i=0; i<8; i++) printf("\b");
			printf("Server: %s\n", msg_recv);
			printf("Client: ");
		}

		if(strcmp(msg_recv, "good bye") == 0)
			break;
	}
}

void *write_thread(void *vargp)
{
	int numbytes, new_fd;
	char msg_send[MAXDATASIZE]; // space to hold the message to be sent
	int *newfd_ptr = (int *)vargp;
	new_fd = *newfd_ptr;

	while(1) {
		printf("Client: ");
		scanf("%[^\n]%*c", msg_send);
		
		// send your message
		if ((numbytes = send(new_fd, msg_send, strlen(msg_send), 0)) == -1)
			perror("send");
		else
			printf("Message sent: %d bytes transferred.\n", numbytes);

		if(strcmp(msg_send, "good bye") == 0)
			break;
	}
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET_ADDRSTRLEN]; // space to hold the server's address

	if (argc != 2) {
		fprintf(stderr,"usage: client hostname\n");
		exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
	printf("Client: connecting to %s\n", s);
	
	pthread_t tid[2];

	pthread_create(&tid[0], NULL, read_thread, (void *)&sockfd);
	pthread_create(&tid[1], NULL, write_thread, (void *)&sockfd);
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	
	close(sockfd);
	return 0;
}