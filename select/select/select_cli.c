#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define MAXLINE 512
#define SEND_PORT 8003

//
int readline(const int fd, void *recvline, const int maxlen);

//
int writen(const int fd, const void *sendline, const int n);

int max(int a, int b) {
    if (a > b)
        return a;
    else
        return b;
}

//
void handle_client(FILE *fp, int sockfd) {
    int maxFDP, stdineof, n;
    int lineNumber = 1;
    //File descriptor set
    fd_set rset;

    //Send and Receive buffer
    char sendline[MAXLINE];
    char recvline[MAXLINE];

    stdineof = 0;

    //Clear and initialize the file descriptor
    FD_ZERO(&rset);

    printf("Read from file:\n");
    while (1) {
        if (stdineof == 0) {
            //Add a new file descriptor to the descriptor set
            FD_SET(fileno(fp), &rset);
        }
        //Add the socket to sever into descriptor set
        FD_SET(sockfd, &rset);

        maxFDP = max(fileno(fp), sockfd) + 1;

        //Set SELECT into BLOCK mode
        if (select(maxFDP, &rset, NULL, NULL, NULL) < 0) {
            perror("Select error");
        }
        //printf("ready:%d\n",n);

        if (FD_ISSET(sockfd, &rset)) { //socket ready
            //printf("socket ready  %d\n",stdineof);
            if ((readline(sockfd, recvline, MAXLINE)) == 0) { //read a line from socket
                //if it's NULL,
                if (stdineof == 1) {
                    //If reached the EOF, then return to main
                    return;                    
                }
                else {
                    printf("handle_client: server terminated prematurely\n");
                    exit(-1);
                }
            }
            fputs(recvline, stdout);
        }
        //Whether the file is ready
        if (FD_ISSET(fileno(fp), &rset)) {
            //Read a single line for file
            if ((fgets(sendline, MAXLINE, fp)) == NULL) {
                //Failed or reached EOF
                stdineof = 1;
                //Shutdown the socket with no more receptions or transmissions
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            writen(sockfd, sendline, (int) strlen(sendline));
            //Print sent line
            printf("Line %d: ", lineNumber++);
            fputs(sendline, stdout);
            //Clear the buffer for next line
            memset(sendline, 0, sizeof(sendline));
        }
    }
}

int main(int argc, char **argv) {
    int client_sockfd;
    FILE *client_file;
    struct sockaddr_in serv_addr;
    //char *file;

    //Validate parameters, argv[1] is Server IP, argv[2] is file path
    // if (argc != 3)
    // {
    //     printf("parameter error!\n");
    //     exit(0);
    // }

    //Open a UDP sokcet
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SEND_PORT);
    //Server IP address is in argv[1]
    inet_aton(argv[1], &serv_addr.sin_addr);

    //Connect to a UDP server
    if ((connect(client_sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) < 0) {
        perror("Connect error");
        exit(0);
    }

    //Open a file in read-only mode
    if ((client_file = fopen(argv[2], "r")) == NULL) {
        perror("Read file error");
        exit(-1);
    }
    //Print information
    printf("File descriptor: %d\n", fileno(client_file));
    printf("File path: %s\n", argv[2]);
    printf("--------------------------\n");
    //
    handle_client(client_file, client_sockfd);

    fclose(client_file);
    close(client_sockfd);
    return 0;
}

int readline(const int fd, void *recvline, const int maxlen) {
    int n, rc;
    char c, *ptr;

    ptr = recvline;
    for (n = 1; n < maxlen; n++) {
        if ((rc = (int) read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;
        } else if (rc == 0) {
            if (n == 1)
                return 0;
            else
                break;
        } else
            return -1;
    }
    *ptr = 0;
    return n;
}

int writen(const int fd, const void *sendline, const int n) {
    int n_left;
    int n_write;
    const char *nptr;

    nptr = sendline;
    n_left = n;
    while (n_left > 0) {
        if ((n_write = (int) write(fd, nptr, (size_t) n_left)) <= 0) {
            if (errno == EINTR)
                n_write = 0;
            else
                return -1;
        }
        n_left -= n_write;
        nptr += n_write;
    }
    return (n);
}
