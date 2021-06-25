/**
*** compilation: g++ -o ftp-client ftp-client.cpp
*** Initial hierarchy of folders to start:
*** /root
***    |- ftp-server
***       |- users
***          |- users.dat
***       |- data
***       |- ftp-server.cpp
***    |- ftp-client
***       |- ftp-client.cpp
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#define PORT "3490" // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once

// get sockaddr, IPv4:
void *get_in_addr(struct sockaddr *sa) {
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

bool getAuthenticated(int sockfd) {
    char msg_buf[MAXDATASIZE];
    int numbytes;
    // prompt for the username
    if((numbytes = recv(sockfd, msg_buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    msg_buf[numbytes] = '\0';
    printf("%s", msg_buf);
    scanf(" %s", msg_buf);
    if((numbytes = send(sockfd, msg_buf, strlen(msg_buf), 0)) == -1) {
        perror("send");
        exit(1);
    }

    if(strcmp(msg_buf, "NEWUSER") == 0) {
        // prompt for the username
        if((numbytes = recv(sockfd, msg_buf, MAXDATASIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        msg_buf[numbytes] = '\0';
        printf("%s", msg_buf);
        scanf(" %s", msg_buf);
        if((numbytes = send(sockfd, msg_buf, strlen(msg_buf), 0)) == -1) {
            perror("send");
            exit(1);
        }
    }

    // prompt for the password
    if((numbytes = recv(sockfd, msg_buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    msg_buf[numbytes] = '\0';
    printf("%s", msg_buf);
    scanf(" %s", msg_buf);
    if((numbytes = send(sockfd, msg_buf, strlen(msg_buf), 0)) == -1) {
        perror("send");
        exit(1);
    }

    // final prompt
    if((numbytes = recv(sockfd, msg_buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    msg_buf[numbytes] = '\0';
    printf("%s\n", msg_buf);

    string t = msg_buf;
    return t.substr(0,7) == "SUCCESS";
}

bool validateFileName(string file_name) {
	return file_name.find('/') == string::npos && file_name.length() > 0;
}

void sendFile(int sockfd) {
	char msg_buf[MAXDATASIZE], t_mode = 'c';
	int numbytes;
	string msg_str, file_path, file_name;
	printf("Enter the path to the file: ");
	cin >> file_path;
	printf("Enter the name of the file: ");
	cin >> file_name;
	printf("Enter the mode of transfer (binary(b)/character(c)): ");
	scanf(" %c", &t_mode);
	fstream file;
	file.open(file_path + "/" + file_name, ios::in);
	if(!file || !validateFileName(file_name)) {
		printf("File doesn't exist.\n");
		return;
	}
	file.close();
	// send the command to the server
	msg_str = "put " + file_name;
	if((numbytes = send(sockfd, &msg_str[0], msg_str.length(), 0)) == -1) {
		perror("send");
		exit(1);
	}
	// receive the response from the server
	if((numbytes = recv(sockfd, msg_buf, MAXDATASIZE-1, 0)) == -1) {
		perror("recv");
		exit(1);
	}
	msg_buf[numbytes] = '\0';
	// ask the user what to do if file already exists
	if(strcmp(msg_buf, "RE") == 0) {
		printf("File already exists on the server. Do you want to overwrite (y/n)??");
		char s;
		scanf(" %c", &s);
		if(s == 'n') {
			// send the reply to the server
			send(sockfd, "NO", 2, 0);
			return;
		}
	}
	// send the file_size to the server
	int fd = open((file_path + "/" + file_name).c_str(), O_RDONLY); 
    struct stat file_stat;
	fstat(fd, &file_stat);
	int file_size = (int)file_stat.st_size;
	close(fd);
	msg_str = to_string(file_size).append(1, t_mode);
	if((numbytes = send(sockfd, &msg_str[0], msg_str.length(), 0)) == -1) {
		perror("send");
		exit(1);
	}
	// receive the response for file_size
	recv(sockfd, msg_buf, MAXDATASIZE, 0);
	// open the file for the transfer
	printf("Progress: ");
	strcpy(msg_buf, (file_path + "/" + file_name).c_str());
	FILE* fp = fopen(msg_buf, t_mode == 'c' ? "r" : "rb"); 
	int bytes_transferred = 0, bytes_read;
	double percentIndicator = 0.05, precision = 0.05;
	// start the file transfer
	while(bytes_transferred < file_size) {
		// read the data into msg_buf
		bytes_read = fread(msg_buf, 1, MAXDATASIZE, fp);
		// send the data to the server
		numbytes = send(sockfd, msg_buf, bytes_read, 0);
		bytes_transferred += numbytes;
		while(bytes_transferred >= percentIndicator * file_size) {
			percentIndicator += precision;
			printf("#");
		}
	}
	fclose(fp);
	printf(" 100%% finished.\n");
	// receive the response from the server
	if((numbytes = recv(sockfd, msg_buf, MAXDATASIZE-1, 0)) == -1) {
		perror("recv");
		exit(1);
	}
	msg_buf[numbytes] = '\0';
	// print the response to the user
	if(strcmp(msg_buf, "NO") == 0)
		printf("There was a problem in uploading you file.\n");
	else
	    printf("File successfully uploaded. %d bytes transferrred.\n", file_size);
}

void getFile(int sockfd) {
	char msg_buf[MAXDATASIZE], t_mode;
	int numbytes;
	string file_name, msg_str;
	printf("Enter the name of the file: ");
	cin >> file_name;
	if(!validateFileName(file_name)) {
		printf("Please enter a valid file name.\n");
		return;
	}
	// send the command to the server
	msg_str = "get " + file_name;
	if((numbytes = send(sockfd, &msg_str[0], file_name.length() + 4, 0)) == -1) {
		perror("send");
		exit(1);
	}
	// receive the response from the server
	if((numbytes = recv(sockfd, msg_buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		exit(1);
	}
	msg_buf[numbytes] = '\0';
	// if the file doesn't exist on the server
	if(strcmp(msg_buf, "NO") == 0) {
		printf("The file doesn't exist on the server.\n");
		return;
	}
	// get the file_size if file exists
	int file_size;
	t_mode = msg_buf[numbytes - 1];
	msg_buf[numbytes - 1] = '\0';
	sscanf(msg_buf, "%d", &file_size);
	// send the response to the server
	strcpy(msg_buf, "OK");
	send(sockfd, msg_buf, strlen(msg_buf), 0);
	// get the file
	printf("Progress: ");
	int bytes_transferred = 0;
	double percentIndicator = 0.05, precision = 0.05;
	FILE* fd;
	fd = fopen(file_name.c_str(), t_mode == 'c' ? "w" : "wb");
	while(bytes_transferred < file_size) {
		numbytes = recv(sockfd, msg_buf, MAXDATASIZE, 0);
		bytes_transferred += numbytes;
		fwrite(msg_buf, numbytes, 1, fd);
		while(bytes_transferred >= percentIndicator * file_size) {
			percentIndicator += precision;
			printf("#");
		}
	}
	printf(" 100%% finished.\n");
	fclose(fd);
	strcpy(msg_buf, ("Successfully downloaded " + file_name + ". " + to_string(file_size) + " bytes transferred.\n").c_str());
	printf("%s", msg_buf);
}

void getList(int sockfd) {
	int data_size, numbytes;
	char msg_buf[MAXDATASIZE];
	// send the command to the server
	if((numbytes = send(sockfd, "lst", 3, 0)) == -1) {
		perror("send");
		exit(1);
	}
	// get the data_size
	if((numbytes = recv(sockfd, msg_buf, MAXDATASIZE - 1, 0)) == -1) {
		perror("recv");
		exit(1);
	}
	msg_buf[numbytes] = '\0';
	sscanf(msg_buf, "%d", &data_size);
	// send the response to the server
	strcpy(msg_buf, "OK");
	send(sockfd, msg_buf, strlen(msg_buf), 0);
	// get the list of files on the server
	int bytes_transferred = 0;
	while(bytes_transferred < data_size) {
		numbytes = recv(sockfd, msg_buf, MAXDATASIZE - 1, 0);
		bytes_transferred += numbytes;
		msg_buf[numbytes] = '\0';
		printf("%s", msg_buf);
	}
}

int main(int argc, char *argv[]) {
	string cmd;
	int sockfd, numbytes;
	char msg_buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET_ADDRSTRLEN]; // space to hold the server's address

	if(argc != 2) {
		fprintf(stderr,"usage: client hostname\n");
		exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if(p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    if(!getAuthenticated(sockfd))
        return 0;
	printf("Client: connecting to %s\n", s);
	
	while(1) {
		printf("ftp> ");
		cin >> cmd;

		if(cmd == "quit") {
			if((numbytes = send(sockfd, "bye", 3, 0)) == -1) {
        		perror("send");
        		exit(1);
    		}
			break;
		}
		
		if(cmd == "put")
			sendFile(sockfd);
		else if(cmd == "get") 
			getFile(sockfd);
		else if(cmd == "lst")
			getList(sockfd);
		else
			printf("Enter a valid command\n");
	}
	
	close(sockfd);
	return 0;
}