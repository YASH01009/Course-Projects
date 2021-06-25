/**
*** sender is the server
*** Implementation of a sliding window protocol
*** Compilation: gcc -o sender sender.c -lpthread
**/

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
#include <stdbool.h>

#define PORT "3490" // the port users will be connecting to
#define BACKLOG 10  // how many pending connections queue will hold
#define MAXDATASIZE 4  // max number of bytes we can get at once
#define D 30  // total number of packets to be transmitted
#define W 4  // window size
#define P 4  // packet size in bytes
#define A 4  // size of an ack pkt in bytes
#define T 2  // Timeout during retransmission

int packets[D]; // packets to be transmitted over unreliable channel (packets can be lost or ONE bit errors can happen)
int packet_ptr = 0; // this points to the packet to be transmitted next
bool acks[2*W];
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

void fill_in_data(void) {
	int i;
	srand(time(0));
	for(i = 0; i < D; i++) packets[i] = rand()%1000;
	for(i = 0; i < 2*W; i++) acks[i] = true;
}

void *read_thread(void *vargp) {
	int *newfd_ptr = (int *)vargp;
	int new_fd = *newfd_ptr;
	int data, numbytes;

	while(1) {
		if((numbytes = recv(new_fd, &data, A, 0)) == -1) perror("recv");
		else {
			if(!(__builtin_popcount(data) & 1)) { 
				data >>= 1, acks[data] = true;
				printf("Packet Acked from the receiver: Packet #%d.\n", data);
			}
		}
	}
}

void *send_thread(void *vargp) {
	// print the data that you are transferring
	int i;
	for(i = 0; i < D; i++)
		printf("%d ", packets[i]);
	printf("\n");

	int numbytes, new_fd, k, t, data;
	int *newfd_ptr = (int *)vargp;
	new_fd = *newfd_ptr;
	int w_packets[2*W], w_curr = 0; // w_curr is the seq#
	srand(time(0));

	while(packet_ptr < (W + D)) {

		k = ((w_curr+(W << 1))-W)%(W << 1); 
		while(!acks[k]) { // retransmission
			// send the data
			t = rand()%5;
			data = w_packets[k];
			if(t == 0) { // introduce an error
				data ^= (1 << (rand()%32));
				printf("Retransmission: Packet #%d contains an error.\n", k);
				if((numbytes = send(new_fd, &data, P, 0)) == -1) perror("send");
			} else if(t >= 1 && t < 3) { // pkt successfully transmitted
				if((numbytes = send(new_fd, &data, P, 0)) == -1) perror("send");
				else printf("Retransmission: Packet #%d transferred.\n", k);
			} else // pkt is lost
				printf("Retransmission: Packet #%d is lost in transmission.\n", k);
			sleep(T); // wait for 2 seconds and send again if pkt_k is not acked
		}

		// do not send anything if the data is already transmitted once
		if(packet_ptr >= D) {
			packet_ptr++;
			w_curr++;
			w_curr %= 2*W;
			continue;
		}

		// prepare the data
		w_packets[w_curr] = packets[packet_ptr++]; // put the data: 27 LSBs
		w_packets[w_curr] &= ((1 << 27) - 1); // clear the 5 MSBs
		w_packets[w_curr] |= (w_curr << 28); // incorporate the seq#
		if(__builtin_popcount(w_packets[w_curr]) & 1) // incorporate the parity bit
			w_packets[w_curr] |= (1 << 31);

		// send the data
		t = rand()%5;
		data = w_packets[w_curr];
		acks[w_curr] = false;

		if(t == 0) { // introduce an error
			data ^= (1 << (rand()%32));
			printf("Message sent: Packet #%d contains an error.\n", w_curr);
			if((numbytes = send(new_fd, &data, P, 0)) == -1) perror("send");
		} else if(t >= 1 && t < 3) { // pkt successfully transmitted
			if((numbytes = send(new_fd, &data, P, 0)) == -1) perror("send");
			else printf("Message sent: Packet #%d transferred.\n", w_curr);
		} else // pkt is lost
			printf("Message sent: Packet #%d is lost in transmission.\n", w_curr);
		printf("Data: %d, packet_ptr: %d.\n", data, packet_ptr);

		w_curr++;
		w_curr %= 2*W;
	}

	// This step uses the reliability provided by the inside system
	// If this doesn't use that, the it requires to solve the two generals' problem
	data = 0;
	data |= (1 << 31);
	data |= (1 << 27);
	if((numbytes = send(new_fd, &data, P, 0)) == -1) perror("send");
	else printf("Message sent: Signal to end the transmission.\n");
}

int main(void) {
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
	
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	printf("Server: waiting for connection...\n");
	
	sin_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size); // returns a socket used for comm with the client
	
	if (new_fd == -1) {
		perror("accept");
		exit(1);
	}
	
	inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
	printf("Server: got connection from %s\n", s);
	close(sockfd); // close the socket that was configured for listening, connections no longer accepted
	
	pthread_t tid[2];
	fill_in_data();

	pthread_create(&tid[0], NULL, send_thread, (void *)&new_fd);
	pthread_create(&tid[1], NULL, read_thread, (void *)&new_fd);
	pthread_join(tid[0], NULL);
	pthread_cancel(tid[1]);
	
	close(new_fd);
	return 0;
}


