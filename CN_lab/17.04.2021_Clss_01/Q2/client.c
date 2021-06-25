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
#include <time.h>
#include <assert.h>

#define PORT "3490" // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once

// get sockaddr, IPv4:
void *get_in_addr(struct sockaddr *sa)
{
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

void print_time(int sockfd) 
{
	char buff[MAXDATASIZE];
	int numbytes;
	if((numbytes = recv(sockfd, buff, MAXDATASIZE-1, 0)) == -1) {
		perror("recv");
		return;
	}
	buff[numbytes] = '\0';
	printf("Time: %s\n", buff);
}

void validate_argv(int argc)
{
	if (argc != 2) {
		fprintf(stderr,"usage: client hostname\n");
		exit(1);
	}
}

void validate_conn(struct addrinfo *p)
{
	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		exit(2);
	}
}

void print_conn(struct addrinfo *p, char *s)
{
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
	printf("Client: connecting to %s\n", s);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET_ADDRSTRLEN]; // space to hold the server's address

	validate_argv(argc);

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

	validate_conn(p);

	print_conn(p, s);

	print_time(sockfd);
	
	close(sockfd);
	return 0;
}