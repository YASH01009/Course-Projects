/*
** server.c -- a stream socket server demo (chat application)
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
#include <time.h>
#include <assert.h>

#define PORT "3490" // the port users will be connecting to
#define BACKLOG 10  // how many pending connections queue will hold
#define MAXDATASIZE 100 // max number of bytes we can get at once

// get sockaddr, IPv4:
void *get_in_addr(struct sockaddr *sa)
{
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

void print_msg(int new_fd) 
{
	int numbytes;
	char buff[MAXDATASIZE];
	if((numbytes = recv(new_fd, buff, MAXDATASIZE-1, 0)) == -1 ) {
		perror("recv");
		return;
	}
	buff[numbytes] = '\0';
	printf("Message: %s\n", buff);
} 

void validate_conn(struct addrinfo *p)
{
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}
}

int main(void)
{
	int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	
	
	int numbytes;
	int yes = 1;
	int rv;
	char s[INET_ADDRSTRLEN]; // space to hold the client's address
	
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
	
	validate_conn(p);

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	printf("Server: waiting for connection...\n");
	
	sin_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size); // returns a new socket used for comm with the client
	
	if (new_fd == -1) {
		perror("accept");
		exit(1);
	}
	
	inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
	printf("Server: got connection from %s\n", s);
	close(sockfd); // close the socket that was configured for listening, connections no longer accepted
	
	print_msg(new_fd);
	
	close(new_fd);
	return 0;
}
