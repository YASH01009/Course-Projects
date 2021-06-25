/*
** client.c -- a datagram "client" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVERPORT "4950" // the port users will be connecting to
#define MAXBUFLEN 100

void validate_args(int argc)
{
	if (argc != 2) {
		fprintf(stderr,"usage: client hostname\n");
		exit(1);
	}
}

void validate_socket(struct addrinfo *p)
{
	if (p == NULL) {
		fprintf(stderr, "client: failed to create socket\n");
		exit(2);
	}
}

void send_msg(int sockfd, struct addrinfo *p)
{
	int numbytes;

	if ((numbytes = sendto(sockfd, "Hello, dear!!", 13, 0, p->ai_addr, p->ai_addrlen)) == -1) {
		perror("client: sendto");	
		exit(1);
	}

	printf("client: sent %d bytes\n", numbytes);
}

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;

	validate_args(argc);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
		break;
	}

	validate_socket(p);

	send_msg(sockfd, p);

	char buf[MAXBUFLEN], s[INET_ADDRSTRLEN];
	struct sockaddr_storage their_addr; // server's address info
	socklen_t addr_len;

	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}	

	buf[numbytes] = '\0';
	printf("client received message: \"%s\"\n", buf);
	printf("client: received %d bytes\n", numbytes);

	freeaddrinfo(servinfo);

	close(sockfd);

	return 0;
}