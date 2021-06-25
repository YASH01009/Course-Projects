/**
*** receiver is the client
*** Implementation of a sliding window protocol
*** Compilation: gcc -o receiver receiver.c 
**/

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
#include <pthread.h>
#include <stdbool.h>

#define PORT "3490" // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once
#define D 30  // total number of packets to be transmitted
#define W 4  // window size
#define P 4  // packet size in bytes
#define A 4  // size of an ack pkt in bytes

int packets[D]; // packets to be received over unreliable channel
int packet_ptr = 0; // this points to the packet to be received next
/**
*** Protocol Used: Selective Repeat
*** Packet format: first bit (MSB) for the partity check (even parity),
*** the next three bits for the sequence number, and the next one
*** bit is used for signalling to inform that the trasmission has ended 
*** value 0 for all normal transmissions and 1 for signalling,
*** and the remaining 27 bits contain the useful data to be transmitted  
*** over the channel.
*** ACK pkt format: LSB is the parity bit, next three bits from the left
*** form the sequence number. (0000 "110" '0'): seq - 6, parity - 0
**/

// get sockaddr, IPv4:
void *get_in_addr(struct sockaddr *sa) {
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

void recv_data_func(int new_fd) {
	int data, seq, w_curr = 0;
	int w_packets[2*W], i, numbytes;
	bool acks[2*W];
	for(i = 0; i < 2*W; i++) acks[i] = false;
	// w_curr is the sequence# that the receiver is waiting for
	// seq is the sequence# that is in the packet received

	while(1) {
		if((numbytes = recv(new_fd, &data, P, 0)) == -1) perror("recv");
		else
			if(!(__builtin_popcount(data) & 1)) {
				// if the signal for termination is sensed
				if(data & (1 << 27))
					break;
				// extract the seq number
				seq = data & (7 << 28); 
				seq >>= 28;
				printf("Message Received: Packet #%d received. (Data: %d)\n", seq, data);
				// check if the pkt can be accepted
				if((w_curr <= W && (seq >= w_curr && seq < w_curr + W)) ||
					(w_curr > W && (seq >= w_curr || seq < ((w_curr + W)%(W << 1))))) {
					// check if we already have the data
					if(!acks[seq]) {
						data &= ((1 << 27)-1);
						w_packets[seq] = data;
						acks[seq] = true;
						printf("Packet #%d received and utilized. (Data: %d)\n", seq, data);
					} else
						printf("Packet #%d received but not utilized. (Data: %d)\n", seq, data);
				} else {
					printf("Message Received: Packet #%d can not be accepted. (Data: %d, w_curr: %d)\n", seq, data, w_curr);
				}
				// window movement
				while(acks[w_curr]) {
					acks[w_curr] = false;
					packets[packet_ptr++] = w_packets[w_curr++];
					w_curr %= 2*W;
				}
				// send the ack pkt
				data = 0;
				data = (seq << 1);
				if(__builtin_popcount(data) & 1)
					data ^= 1;
				if((numbytes = send(new_fd, &data, A, 0)) == -1) perror("send");
				else printf("Acknowledgement Sent: Packet #%d.\n", seq);
			} else
				printf("Packet Received: A packet with error.\n");
	}
}

int main(int argc, char *argv[]) {
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
	
	recv_data_func(sockfd);

	int i;
	for(i = 0; i < D; i++)
		printf("Packet #%d, Data: %d\n", i, packets[i]);
	printf("\n");
	
	close(sockfd);
	return 0;
}


