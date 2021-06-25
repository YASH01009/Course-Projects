/**
*** Compilation: g++ -o ftp-server ftp-server.cpp -lpthread -lrt
*** Execution: ./ftp-server
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

#include <unordered_map>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

using namespace std;

#define USERS_LIST_PATH "users/users.dat"
#define PORT "3490" // the port users will be connecting to
#define BACKLOG 10  // how many pending connections queue will hold
#define MAXDATASIZE 100 // max number of bytes we can get at once
#define UNSUCCESSFUL 0
#define SUCCESSFUL 1
#define INCORRECT 2

sem_t mutex; // semaphore for signaling between threads

class UserManager {
private:
	static UserManager *instance;
	unordered_map<string, pair<string, bool>> usersList;
	UserManager() {
		fstream file;
		file.open(USERS_LIST_PATH, ios::in);
		string username, password;
        while(getline(file, username)) {
            getline(file, password);
            usersList[username] = make_pair(password, false);
        }
        file.close();
	}
public:
	static UserManager *getInstance();
	int registerUser(string username, string password);
	int verifyUser(string username, string password);
	bool userExists(string username);
	void looseUser(string username);
};

UserManager *UserManager::instance = NULL;

UserManager *UserManager::getInstance() {
    if(instance == NULL) 
        instance = new UserManager();
    return instance;
}

int UserManager::registerUser(string username, string password) {
	if(userExists(username))
		return UNSUCCESSFUL;
	usersList[username] = make_pair(password, true);
    fstream file;
    file.open(USERS_LIST_PATH, ios::app);
    file << username << '\n';
    file << password << '\n';
    file.close();
	string dirname = "data/" + username; 
	mkdir(&dirname[0], 0777);
	FILE* fd;
	fd = fopen(("data/" + username + "/3ab67d.txt").c_str(), "w");
	fclose(fd);
	return SUCCESSFUL;
}

int UserManager::verifyUser(string username, string password) {
	if(!userExists(username))
		return UNSUCCESSFUL;
	if(usersList[username].first == password && !usersList[username].second) {
		usersList[username].second = true;
		return SUCCESSFUL;
	}
	return INCORRECT;
}

bool UserManager::userExists(string username) {
	return usersList.find(username) != usersList.end();
}

void UserManager::looseUser(string username) {
	usersList[username].second = false;
}

// get sockaddr, IPv4:
void *get_in_addr(struct sockaddr *sa) {
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

string authenticateUser(int new_fd) {
    char msg_buf[MAXDATASIZE];
    int numbytes, response;
    string msg_str, username, password;
    // create a userManager for authenticating the clients
    UserManager *userManager = userManager->getInstance();

    msg_str = "Enter your username (or NEWUSER to register as a new user): ";
    // send the prompt for the username
    if((numbytes = send(new_fd, &msg_str[0], msg_str.length()*sizeof(char), 0)) == -1) {
        perror("send");
        exit(1);
    }

    // receive the username or NEWUSER command
    if((numbytes = recv(new_fd, msg_buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    msg_buf[numbytes] = '\0';

    if(strcmp(msg_buf, "NEWUSER") == 0) {

        msg_str = "Enter a username: ";
        // send the prompt for the username
        if((numbytes = send(new_fd, &msg_str[0], msg_str.length(), 0)) == -1) {
            perror("send");
            exit(1);
        }

        // receive the new username
        if((numbytes = recv(new_fd, msg_buf, MAXDATASIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        msg_buf[numbytes] = '\0';
        username = msg_buf;

        msg_str = "Enter password: ";
        // send the prompt for the password
        if((numbytes = send(new_fd, &msg_str[0], msg_str.length(), 0)) == -1) {
            perror("send");
            exit(1);
        }

        // receive the new password
        if((numbytes = recv(new_fd, msg_buf, MAXDATASIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        msg_buf[numbytes] = '\0';
        password = msg_buf;

        response = userManager->registerUser(username, password);
        switch(response) {
            case UNSUCCESSFUL:
                msg_str = "Failed to create an user. Username already exists.";
                break;
            case SUCCESSFUL:
                msg_str = "SUCCESS: User created. Currently logged in as '" + username + "'.";
                break;
        }

        // send the final prompt
        if((numbytes = send(new_fd, &msg_str[0], msg_str.length(), 0)) == -1) {
            perror("send");
            exit(1);
        }

        return response == SUCCESSFUL ? username : "NOUSER";
    } 

    username = msg_buf;

    msg_str = "Enter your password: ";
    // send the prompt for the password
    if((numbytes = send(new_fd, &msg_str[0], msg_str.length(), 0)) == -1) {
        perror("send");
        exit(1);
    }

    // receive the password
    if((numbytes = recv(new_fd, msg_buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    msg_buf[numbytes] = '\0';
    password = msg_buf;

    response =  userManager->verifyUser(username, password);

    switch(response) {
        case UNSUCCESSFUL:
            msg_str = "User with username '" + username + "' doesn't exist.";
            break;
        case INCORRECT:
            msg_str = "Incorrect Password.";
            break;
        case SUCCESSFUL:
            msg_str = "SUCCESS: Logged in as '" + username + "'.";
            break;
    }

    // send the final prompt
    if((numbytes == send(new_fd, &msg_str[0], msg_str.length(), 0)) == -1) {
        perror("recv");
        exit(1);
    }

    return response == SUCCESSFUL ? username : "NOUSER";
}

void sendFile(int sockfd, string user, string file_name) {
	char msg_buf[MAXDATASIZE], t_mode = 'c';
	int numbytes;
	string file_path, msg_str;
	file_path = "data/" + user;
	// send the file_size to the client
	// if the file doesn't exist, send "NO"
	strcpy(msg_buf, (file_path + "/" + file_name).c_str());
	int fd = open(msg_buf, O_RDONLY); 
	if(fd < 0) {
		// file doesn't exist on the server
		send(sockfd, "NO", 2, 0);
		return;
	}
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
	strcpy(msg_buf, (file_path + "/" + file_name).c_str());
	FILE* fp = fopen(msg_buf, t_mode == 'c' ? "r" : "rb"); 
	int bytes_transferred = 0, bytes_read;
	// start the file transfer
	while(bytes_transferred < file_size) {
		// read the data into msg_buf
		bytes_read = fread(msg_buf, 1, MAXDATASIZE, fp);
		// send the data to the server
		numbytes = send(sockfd, msg_buf, bytes_read, 0);
		bytes_transferred += numbytes;
	}
	fclose(fp);
	strcpy(msg_buf, (file_name + " sent for " + user + ".\n").c_str());
	printf("%s", msg_buf);
}

void getFile(int sockfd, string user, string file_name) {
	char msg_buf[MAXDATASIZE], t_mode;
	int numbytes;
	fstream file;
	file.open("data/" + user + "/" + file_name, ios::in);
	file ? strcpy(msg_buf, "RE") : strcpy(msg_buf, "OK"); 
	file.close();
	// send the response to the client
	if((numbytes = send(sockfd, msg_buf, 2, 0)) == -1) {
		perror("send");
		exit(1);
	}
	// receive the response from the client
	if((numbytes = recv(sockfd, msg_buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		exit(1);
	}
	msg_buf[numbytes] = '\0';
	// if the file is asked not to replace on the server
	if(strcmp(msg_buf, "NO") == 0)
		return;
	// know the file_size
	int file_size;
	t_mode = msg_buf[numbytes - 1];
	msg_buf[numbytes - 1] = '\0';
	sscanf(msg_buf, "%d", &file_size);
	// send the response to the client
	strcpy(msg_buf, "OK");
	send(sockfd, msg_buf, strlen(msg_buf), 0);
	// get the file
	int bytes_transferred = 0;
	FILE* fd;
	fd = fopen(("data/" + user + "/" + file_name).c_str(), t_mode == 'c' ? "w" : "wb");
	while(bytes_transferred < file_size) {
		numbytes = recv(sockfd, msg_buf, MAXDATASIZE, 0);
		bytes_transferred += numbytes;
		fwrite(msg_buf, numbytes, 1, fd);
	}
	// notify the client that the file is received
	strcpy(msg_buf, "OK");
	if((numbytes = send(sockfd, msg_buf, 2, 0)) == -1) {
		perror("send");
		exit(1);
	}
	fclose(fd);
	strcpy(msg_buf, ("Received " + file_name + " for " + user + ".\n").c_str());
	printf("%s", msg_buf);
	FILE *fptr;
	fptr = fopen(("data/" + user + "/3ab67d.txt").c_str(), "a");
	file_name.append(1, '\n');
	fwrite(&file_name[0], file_name.length(), 1, fptr);
	fclose(fptr);
}

void sendList(int sockfd, string user) {
	int bytes_transferred = 0, bytes_read, numbytes;
	char msg_buf[MAXDATASIZE];
	string msg_str;
	// find the size of the file
	int fd = open(("data/" + user + "/3ab67d.txt").c_str(), O_RDONLY);
	struct stat file_stat;
	fstat(fd, &file_stat);
	int file_size = (int)file_stat.st_size;
	close(fd);
	msg_str = to_string(file_size);
	// send the file_size to the client
	if((numbytes = send(sockfd, &msg_str[0], msg_str.length(), 0)) == -1) {
		perror("send");
		exit(1);
	}
	// get the response from the client
	recv(sockfd, msg_buf, MAXDATASIZE - 1, 0);
	// open the file for transfer
	FILE* fp;
	fp = fopen(("data/" + user + "/3ab67d.txt").c_str(), "r");
    // start the file transfer
	while(bytes_transferred < file_size) {
		// read the data into msg_buf
		bytes_read = fread(msg_buf, 1, MAXDATASIZE, fp);
		// send the data to the server
		numbytes = send(sockfd, msg_buf, bytes_read, 0);
		bytes_transferred += numbytes;
	}
	fclose(fp);
}

void *thread_func(void *vargp) {
    char msg_buf[MAXDATASIZE];
	int numbytes;
	string msg_str, user;
	int new_fd = *((int *)vargp);
	sem_post(&mutex);

    if((user = authenticateUser(new_fd)) == "NOUSER") {
        close(new_fd);
	    pthread_exit(NULL);
    }
	
	while(1) {
		if((numbytes = recv(new_fd, msg_buf, MAXDATASIZE-1, 0)) == -1) {
			perror("recv");
			exit(1);
		}
		msg_buf[numbytes] = '\0';
		msg_str = msg_buf;

		if(msg_str == "bye") {
			UserManager *userManager = UserManager::getInstance();
			userManager->looseUser(user);
			break;
		}

		string s = msg_str.substr(0, 3);
		if(s == "put")
			getFile(new_fd, user, msg_str.substr(4, numbytes-4));
		else if(s == "get")
			sendFile(new_fd, user, msg_str.substr(4, numbytes-4));
		else if(s == "lst")
			sendList(new_fd, user);
		else
			continue;
	}

	close(new_fd);
	pthread_exit(NULL);
}

int main() {
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
        // returns a new socket used for communication with client
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size); 

		if (new_fd == -1) {
			perror("accept");
			exit(1);
		}
		
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
		printf("Server: got connection from %s.\n", s);
		pthread_create(&thread_id, NULL, thread_func, (void *)&new_fd);
		sem_wait(&mutex);
	}
	return 0;
}