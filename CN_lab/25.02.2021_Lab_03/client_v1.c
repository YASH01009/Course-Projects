/*
** client.c -- a stream socket calculator(client) demo
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

#define PORT "3490" // the port client will be connecting to
#define MAXDATASIZE 10 // max number of bytes we can get at once

// get sockaddr, IPv4:
void *get_in_addr(struct sockaddr *sa)
{
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;
	char msg_send[MAXDATASIZE];
	char msg_recv[MAXDATASIZE];
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
	
	while(1) {
		// send the question
		printf("Enter a question like \"2+3\" with no spaces (or \"good bye\" to end): ");
		scanf("%[^\n]%*c", msg_send);
		
		if ((numbytes = send(sockfd, msg_send, strlen(msg_send), 0)) == -1)
			perror("send");
		
		if (strcmp(msg_send, "good bye") == 0)
			break;

		// receive the result
		if ((numbytes = recv(sockfd, msg_recv, MAXDATASIZE-1, 0)) == -1)
			perror("recv");
		else {
			msg_recv[numbytes] = '\0';
			printf("Server: %s\n", msg_recv);
		}
	}
	
	close(sockfd);
	return 0;
}