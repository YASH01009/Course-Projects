#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <errno.h>

#include <string.h>

#include <netdb.h>

#include <netinet/in.h>

#include <sys/socket.h>

#include <arpa/inet.h>

#include <fcntl.h>

#include <sys/sendfile.h>

#include <sys/stat.h>


#include <bits/stdc++.h>

using namespace std;


#define PORT "5000" // the port client will be connecting to

#define BUFFERSIZE 8192

#define MAXMSGSIZE 150 // max number of bytes we can get at once


void recvFile(char *fname, int sockfd)

{

    ssize_t len;

    struct sockaddr_in remote_addr;

    char buffer[BUFFERSIZE];

    int file_size;

    FILE *received_file;

    int remain_data = 0;


    int numbytes = recv(sockfd, buffer, BUFFERSIZE, 0);

    buffer[numbytes] = '\0';

    // cout << "num" << numbytes << "ll\n";


    if (buffer[0] != 'O' || buffer[1] != 'K')

    {

        cout << "Server - " << buffer << "?\n";

        return;

    }


    /* Receiving file size */

    recv(sockfd, buffer, BUFFERSIZE, 0);

    file_size = atoi(buffer);

    //fprintf(stdout, "\nFile size : %d\n", file_size);


    received_file = fopen(fname, "w");

    if (received_file == NULL)

    {

        fprintf(stderr, "Failed to open file  --> %s\n", strerror(errno));

        exit(1);

    }


    remain_data = file_size;


    while ((remain_data > 0) && ((len = recv(sockfd, buffer, BUFFERSIZE, 0)) > 0))

    {

        // fprintf(received_file, "%s", buffer);

        fwrite(buffer, sizeof(char), len, received_file);

        remain_data -= len;

        bzero(buffer, BUFFERSIZE);

    }

    fclose(received_file);

}


void sendFile(char *filename, int sockfd)

{

    int fd = open(filename, O_RDONLY);

    struct stat file_stat;

    int remain_data, sent_bytes = 0, len;

    char file_size[256];


    if (fd == -1)

    {

        fprintf(stderr, "Error opening file --> %s\n", strerror(errno));

        len = send(sockfd, strerror(errno), BUFFERSIZE, 0);

        cout << len << "islen\n";

        return;

    }

    else if (fstat(fd, &file_stat) < 0)

    {

        fprintf(stderr, "Error fstat --> %s\n", strerror(errno));

        len = send(sockfd, strerror(errno), BUFFERSIZE, 0);

        cout << len << "islen\n";

        return;

    }

    else

    {

        send(sockfd, "OK", 3, 0);

    }

    fprintf(stdout, "File Size: \n%d bytes\n", (int)file_stat.st_size);

    sprintf(file_size, "%d", (int)file_stat.st_size);


    /* Sending file size */

    len = send(sockfd, file_size, sizeof(file_size), 0);

    if (len < 0)

    {

        fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));

    }


    remain_data = file_stat.st_size;

    int size = file_stat.st_size;


    char data[BUFFERSIZE] = {0};

    FILE *fp = fopen(filename, "r");


    while (remain_data > 0 && (len = fread(data, 1, sizeof(data), fp)))

    {

        if ((sent_bytes = send(sockfd, data, len, 0)) == -1)

        {

            perror("Error in sending File");

            exit(1);

        }

        remain_data -= sent_bytes;

        memset(data, 0, BUFFERSIZE);

    }


    close(fd);

}


// get IPv4  sockadd

void *getAddress(struct sockaddr *sa)

{

    return &(((struct sockaddr_in *)sa)->sin_addr);

}


int main(int argc, char *argv[])

{

    // Name : T.V.S.S.SRIPAD

    // Roll Number : 18CS01008

    int sockfd;

    struct addrinfo hints, *servinfo, *loopvar;

    int rv;

    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);

    //using IPv4

    hints.ai_family = AF_INET;


    hints.ai_socktype = SOCK_STREAM;


    //getAddressInfo

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)

    {

        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));

        return 1;

    }


    // loop through all the results and connect to the first we can

    for (loopvar = servinfo; loopvar != NULL; loopvar = loopvar->ai_next)

    {

        if ((sockfd = socket(loopvar->ai_family, loopvar->ai_socktype, loopvar->ai_protocol)) == -1)

        {

            perror("client: socket");

            continue;

Yashwin Enamadi, [23.03.21 10:55]
}


        if (connect(sockfd, loopvar->ai_addr, loopvar->ai_addrlen) == -1)

        {

            perror("client: connect");

            close(sockfd);

            continue;

        }


        break;

    }


    if (loopvar == NULL)

    {

        fprintf(stderr, "client: failed to connect\n");

        return 2;

    }


    inet_ntop(loopvar->ai_family, getAddress((struct sockaddr *)loopvar->ai_addr), s, sizeof s);

    cout << "Client: connecting to " << s << "%s\n";


    int bytes, numbytes;

    char msg_send[MAXMSGSIZE], msg_recv[MAXMSGSIZE], msg_recv1[MAXMSGSIZE];

    int isUserSignedIn = 0;


    if ((numbytes = recv(sockfd, msg_recv, MAXMSGSIZE - 1, 0)) == -1)

        perror("recv");

    msg_recv[numbytes] = '\0';


    cout << "Server - " << msg_recv << " \n";


    while (!isUserSignedIn)

    {

        cout << "Enter username \n";

        cin >> msg_send;

        // send UserName to server

        if ((numbytes = send(sockfd, msg_send, sizeof(msg_send), 0)) == -1)

        {

            perror("send");

            exit(1);

        }

        // printf("Enter username is %s\n", msg_send);

        cout << "Enter password \n";


        cin >> msg_send;


        // Ask client for password

        if ((numbytes = send(sockfd, msg_send, sizeof(msg_send), 0)) == -1)

        {

            perror("send");

            exit(1);

        }

        // printf("Enter username is %s\n", msg_send);

        if ((numbytes = recv(sockfd, msg_recv, MAXMSGSIZE - 1, 0)) == -1)

            perror("recv");

        msg_recv[numbytes] = '\0';

        cout << "Server - " << msg_recv << " \n";

        string msg(msg_recv, msg_recv + numbytes);


        if (strcmp(&(msg.substr(0, 22))[0], "LoggedIn successfully!") == 0)

        {

            isUserSignedIn = 1;

            break;

        }

        // printf("Stilll %d", isUserSignedIn);

    }

    while (1)

    {

        // send the question

        cout << " I am waiting..\n";

        if ((numbytes = recv(sockfd, msg_recv, MAXMSGSIZE - 1, 0)) == -1)

            perror("recv");

        msg_recv[numbytes] = '\0';

        cout << "Server - " << msg_recv << " \n";

        int i;

        cin >> i;

        cout << "i is" << i << "\n";

        msg_send[0] = '0' + i;

        msg_send[1] = '\0';

        if ((numbytes = send(sockfd, msg_send, strlen(msg_send), 0)) == -1)

            perror("send");


        // receive the result

        if (i == 1 || i == 2)

        {

            cout << "Enter filename:\n";

            int j = 0;

            string fname;

            cin >> fname;

            for (j = 0; j < MAXMSGSIZE - 1; j++)

            {

                msg_send[j] = (char)fname[j];

            }

            msg_send[j] = '\0';

            // cout << msg_send << "is sent\n";

            if ((numbytes = send(sockfd, msg_send, strlen(msg_send), 0)) == -1)

                perror("send");

            cout << "File Name sent\n";

            if (i == 1)

            {

                recvFile(msg_send, sockfd);

            }

            else

            {

                sendFile(msg_send, sockfd);

            }

        }

        else if (i == 4)

        {

            break;

        }

    }

    close(sockfd);

    freeaddrinfo(servinfo);


    return 0;

}

Yashwin Enamadi, [23.03.21 10:55]
#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <errno.h>

#include <string.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <netdb.h>

#include <arpa/inet.h>

#include <limits.h>

#include <sys/types.h>

#include <fcntl.h>

#include <sys/sendfile.h>

#include <sys/stat.h>


#define BACKLOG 10 // the no of connections queue will hold

#define MAXMSGSIZE 150

#define MAX_FILE_SIZE 500

#define PORT "5000" // the port users will be connecting to

#define MAX_USERS 100

#define BUFFERSIZE 8192


#include <bits/stdc++.h>

using namespace std;


vector<pair<string, string>> users;


int verify(char *username, char *password)

{

    // printf("?%sis un l%sispass\n", username, password);

    for (int i = 0; i < users.size(); i++)

    {

        if (username == users[i].first)

        {

            if (password == users[i].second)

            {

                return i + 1;

            }

        }

    }

    return -1;

}

bool addUser(string username, string password)

{

    int id = users.size();

    users.push_back({username, password});


    string dirpath = "./client" + to_string(id);

    int result = mkdir(&dirpath[0], 0777);

    if (result == -1)

    {

        return 0;

    }

    return 1;

}


void recvFile(char *fname, int sockfd)

{

    ssize_t len;

    struct sockaddr_in remote_addr;

    char buffer[BUFFERSIZE];

    int file_size;

    FILE *received_file;

    int remain_data = 0;


    int numbytes = recv(sockfd, buffer, BUFFERSIZE, 0);

    buffer[numbytes] = '\0';

    // cout << "num" << numbytes << "ll\n";


    if (buffer[0] != 'O' || buffer[1] != 'K')

    {

        cout << "Server - " << buffer << "?\n";

        return;

    }


    /* Receiving file size */

    recv(sockfd, buffer, BUFFERSIZE, 0);

    file_size = atoi(buffer);

    //fprintf(stdout, "\nFile size : %d\n", file_size);


    received_file = fopen(fname, "w");

    if (received_file == NULL)

    {

        fprintf(stderr, "Failed to open file  --> %s\n", strerror(errno));

        exit(1);

    }


    remain_data = file_size;


    while ((remain_data > 0) && ((len = recv(sockfd, buffer, BUFFERSIZE, 0)) > 0))

    {

        // fprintf(received_file, "%s", buffer);

        fwrite(buffer, sizeof(char), len, received_file);

        remain_data -= len;

        bzero(buffer, BUFFERSIZE);

    }

    fclose(received_file);

}


void sendFile(char *filename, int sockfd)

{

    int fd = open(filename, O_RDONLY);

    struct stat file_stat;

    int remain_data, sent_bytes = 0, len;

    char file_size[256];


    if (fd == -1)

    {

        fprintf(stderr, "Error opening file --> %s\n", strerror(errno));

        len = send(sockfd, strerror(errno), BUFFERSIZE, 0);

        cout << len << "islen\n";

        return;

    }

    else if (fstat(fd, &file_stat) < 0)

    {

        fprintf(stderr, "Error fstat --> %s\n", strerror(errno));

        len = send(sockfd, strerror(errno), BUFFERSIZE, 0);

        cout << len << "islen\n";

        return;

    }

    else

    {

        send(sockfd, "OK", 3, 0);

    }

    fprintf(stdout, "File Size: \n%d bytes\n", (int)file_stat.st_size);

    sprintf(file_size, "%d", (int)file_stat.st_size);


    /* Sending file size */

    len = send(sockfd, file_size, sizeof(file_size), 0);

    if (len < 0)

    {

        fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));

    }


    remain_data = file_stat.st_size;

    int size = file_stat.st_size;


    char data[BUFFERSIZE] = {0};

    FILE *fp = fopen(filename, "r");


    while (remain_data > 0 && (len = fread(data, 1, sizeof(data), fp)))

    {

        if ((sent_bytes = send(sockfd, data, len, 0)) == -1)

        {

            perror("Error in sending File");

            exit(1);

        }

        memset(data, 0, BUFFERSIZE);

    }


    close(fd);

}


// get IPv4  sockaddr  ;

void *getAddress(struct sockaddr *sa)

{

    return &(((struct sockaddr_in *)sa)->sin_addr);

}


int main()

{

    // Name : T.V.S.S.SRIPAD

    // Roll Number : 18CS01008

Yashwin Enamadi, [23.03.21 10:55]
int sockfd, sock_conn_fd; // listen on sock_fd, new connection on sock_conn_fd

    struct addrinfo hints, *servinfo, *loopvar;

    struct sockaddr_storage their_addr; // connector's address information

    socklen_t sin_size;


    int yes = 1;

    char s[INET6_ADDRSTRLEN];


    int rv;


    memset(&hints, 0, sizeof hints);

    //using IPv4

    hints.ai_family = AF_INET;

    hints.ai_socktype = SOCK_STREAM;

    hints.ai_flags = AI_PASSIVE; // use my IP


    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)

    {

        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));

        return 1;

    }


    // loop through all the results and bind to the first we can

    for (loopvar = servinfo; loopvar != NULL; loopvar = loopvar->ai_next)

    {

        if ((sockfd = socket(loopvar->ai_family, loopvar->ai_socktype, loopvar->ai_protocol)) == -1)

        {

            perror("server: socket");

            continue;

        }


        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)

        {

            perror("setsockopt");

            exit(1);

        }


        if (bind(sockfd, loopvar->ai_addr, loopvar->ai_addrlen) == -1)

        {

            close(sockfd);

            perror("server: bind");

            continue;

        }


        break;

    }


    freeaddrinfo(servinfo); // all done with this structure


    if (loopvar == NULL)

    {

        fprintf(stderr, "server: failed to bind\n");

        exit(1);

    }

    string uname, pass;

    users.push_back({"ad", "ad"});

    users.push_back({"i", "i"});


    while (1)

    {

        cout << "Do you want to add users Y or N?\n";

        char c;

        cin >> c;

        if (c == 'Y')

        {

            cout << "Enter username:\n";

            cin >> uname;

            cout << "Enter password:\n";

            cin >> pass;

            bool res = addUser(uname, pass);

            if (!res)

                cout << "Previous operation failed please try again\n";

        }

        else

            break;

    }


    if (listen(sockfd, BACKLOG) == -1)

    {

        perror("listen");

        exit(1);

    }


    cout << "Server: waiting for connection...\n";


    while (1)

    {

        sin_size = sizeof their_addr;


        // Accept connections

        sock_conn_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

        if (sock_conn_fd == -1)

        {

            perror("accept");

        }


        inet_ntop(their_addr.ss_family, getAddress((struct sockaddr *)&their_addr), s, sizeof s);

        //Printing address of client

        cout << "Server: got connection from " << s << " \n";


        //fork a child process for communicating with each client

        if (!fork())

        {

            int id;


            char msg_send[MAXMSGSIZE], msg_recv[MAXMSGSIZE];

            int numbytes;

            int isUserSignedIn = 0;

            close(sockfd);


            // Ask the client to enter UserName & password

            if ((numbytes = send(sock_conn_fd, "Welcome to FTP server. Enter username & password to continue..\n", 64, 0)) == -1)

            {

                perror("send");

                exit(1);

            }

            while (!isUserSignedIn)

            {

                // receive the username

                if ((numbytes = recv(sock_conn_fd, msg_recv, MAXMSGSIZE, 0)) == -1)

                {

                    perror("recv");

                    exit(1);

                }

                msg_recv[numbytes] = '\0';

                char *username = msg_recv;

                // printf("%d bytes rec\n", numbytes);

                // receive the question

                if ((numbytes = recv(sock_conn_fd, msg_recv, MAXMSGSIZE, 0)) == -1)

                {

                    perror("recv");

                    exit(1);

                }

                msg_recv[numbytes] = '\0';

                char *password = msg_recv;


                // printf("%d bytes rec\n", numbytes);

Yashwin Enamadi, [23.03.21 10:55]
id = verify(username, password);

                isUserSignedIn = (id == -1) ? 0 : 1;

                // printf("?%sis un l%sispass\n", username, password);

                if (!isUserSignedIn)

                {

                    // printf("YY %d\n", isUserSignedIn);

                    if ((numbytes = send(sock_conn_fd, "Incorrect Username or password. Please try again.\n", 51, 0)) == -1)

                    {

                        perror("send");

                        exit(1);

                    }

                }

                else

                {

                    if ((numbytes = send(sock_conn_fd, "LoggedIn successfully! ", 24, 0)) == -1)

                    {

                        perror("send");

                        exit(1);

                    }


                    break;

                }

            }


            cout << "SentY\n";


            while (1)

            {

                if ((numbytes = send(sock_conn_fd, "Enter 1 for GET\n 2 for PUT\n 3 for MODE\n 4 to Exit\n ", 52, 0)) == -1)

                {

                    perror("send");

                    exit(1);

                }

                cout << "Here\n";

                if ((numbytes = recv(sock_conn_fd, msg_recv, MAXMSGSIZE - 1, 0)) == -1)

                {

                    perror("recv");

                    exit(1);

                }

                msg_recv[numbytes] = '\0';

                int op = atoi(msg_recv);

                cout << "op " << op << "\n";


                if (op == 1 || op == 2)

                {


                    cout << "Waiting for filename\n";

                    if ((numbytes = recv(sock_conn_fd, msg_recv, MAXMSGSIZE - 1, 0)) == -1)

                        perror("recv");

                    msg_recv[numbytes] = '\0';


                    cout << "Filename is " << msg_recv << "\n";


                    if (op == 1)

                    {

                        sendFile(msg_recv, sock_conn_fd);

                    }

                    else

                    {

                        char clientfilefolder[MAXMSGSIZE];

                        string path(msg_recv, msg_recv + numbytes);

                        string dirpath = "./client" + to_string(id);

                        string fullpath = dirpath + "/" + path;

                        // if(result == -1 || !result)

                        recvFile(&fullpath[0], sock_conn_fd);

                    }

                }

                else if (op == 4)

                {

                    break;

                }

            }


            close(sock_conn_fd);

            exit(1);

        }

        else

            close(sock_conn_fd);

    }


    return 0;

}