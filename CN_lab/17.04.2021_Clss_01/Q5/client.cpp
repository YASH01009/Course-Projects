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
#define QUERIES_NO 5

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	if (argc != 2) {
		fprintf(stderr,"usage: client hostname\n");
		exit(1);
	}

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

	if (p == NULL) {
		fprintf(stderr, "client: failed to create socket\n");
		return 2;
	}

	int queries_limit = QUERIES_NO;

	printf("================================================================================\n"
		"Read the following instructions carefully:\n"
		"1. You are allowed to query the telephone directory for exactly %d times.\n"
		"2. Enter '1' (without quotes) to query the telephone number of a known user.\n"
		"3. Enter '2' (without quotes) to query the user for a known telephone number.\n"
		"Rock!!\n"
		"================================================================================\n", 
		queries_limit);

	int instruction;

	while(queries_limit--) {

		printf("Enter your instruction: ");
		scanf("%d%*c", &instruction);

		printf("Enter the data accordingly: ");
		scanf("%[^\n]%*c", buf+1);

		buf[0] = instruction == 1 ? 'U' : 'N';

		if ((numbytes = sendto(sockfd, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen)) == -1) {
			perror("client: sendto");	
			exit(1);
		}

		printf("client: sent %d bytes to %s\n", numbytes, argv[1]);

		if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, p->ai_addr, &p->ai_addrlen)) == -1) {
			perror("recvfrom");
			exit(1);
		}	

		buf[numbytes] = '\0';
		printf("client received message: %s\n", buf);
		printf("client: received %d bytes from %s\n", numbytes, argv[1]);
	}

	freeaddrinfo(servinfo);
	close(sockfd);

	return 0;
}