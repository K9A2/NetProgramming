#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>

#include "Utils.c"

#define MAXLINE 512
#define SEND_PORT 8003
#define REQUEST_QUEUE_COUNT 64

//static ssize_t my_read(int fd, char *ptr);
unsigned int readline(const int fd, void *vptr, const size_t maxlen);

//ssize_t: unsigned int
unsigned int writen(const int fd, const void *sendline, const int n);

int main(int argc, char **argv) {
    int i, maxi, maxfd;
    //maxi: the max client value, see client below
    //maxfd: max file descriptor, used in select. fd is allocated by system.
    int listen_fd, connfd, sockfd;
    //listen_fd: server listening fd
    //connfd: server fd connecting to a client
    //sockfd:
    int num_fd, client[FD_SETSIZE], logFile;
    //FD_SETSIZE is a macro const int, which is defined in sys/select.h,
    //it represents the maximum value of fd_set type, usually 1024.
    //Array client: used to record which fd is set in allset,
    //if client[i]>0, client[i] is a fd to be checked. the front items of client array are >0 and the tails are <0
    //

    int lineNumber = 1;

    ssize_t n;
    fd_set rset, allset;
    char line[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in client_addr, serv_addr;

    /* Prepare log file */
    if ((logFile = open("./tmp.log", O_RDWR)) < 0) {
        //Failed to open log file
        perror("Can not open log file, trying to create new log file");
        //exit(0);
    }
    if ((logFile = open("./tmp.log", O_WRONLY | O_CREAT | O_TRUNC)) < 0) {
        perror("Can not create log file");
        printf("Program terminated.\n");
        exit(-1);
    }

    //Set local address
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SEND_PORT);

    //Bind the address to this sockets for incoming messages
    bind(listen_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    listen(listen_fd, REQUEST_QUEUE_COUNT);
    maxfd = listen_fd;
    maxi = -1;

    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }

    //initialize the whole set
    FD_ZERO(&allset);
    //add listen_fd to the whole set
    FD_SET(listen_fd, &allset);

    printf("Ready to receive!\n");

    while (1) {
        rset = allset;
        num_fd = select(maxfd + 1, &rset, NULL, NULL, NULL);

        //listen fd ready, accept it
        if (FD_ISSET(listen_fd, &rset)) {
            //Accept remote connection request
            connfd = accept(listen_fd, (struct sockaddr *) &client_addr, &clilen);

            for (i = 0; i < FD_SETSIZE; i++) {
                //find the min client not filled with connfd
                if (client[i] < 0) {
                    client[i] = connfd;
                    break;
                }
            }
            if (i == FD_SETSIZE) { //all of clients are filled
                printf("too many clients\n");
                exit(0);
            }

            //add connect fd
            FD_SET(connfd, &allset);

            if (connfd > maxfd){
                maxfd = connfd;
            }
            if (i > maxi) {
                maxi = i;
            }
            if (--num_fd <= 0) {
                continue;
            }
        } //end of if

        //some connfd ready
        for (i = 0; i <= maxi; i++) {
            if ((sockfd = client[i]) < 0) {
                continue;
            }
            //if sockfd is set
            if (FD_ISSET(sockfd, &rset)) {
                //Read from client socket
                if ((n = readline(sockfd, line, MAXLINE)) == 0) { //read end
                    if (strcmp(line, "exit") == 0) {
                        printf("Terminated by client.\n");
                        exit(-1);
                    }
                    close(sockfd);
                    //clear sockfd from allset
                    FD_CLR(sockfd, &allset);

                    client[i] = -1;
                } else {
                    //read not end
                    printf("[%d]Data received at: ", lineNumber++);
                    printCurrentTime();
                    //Log transactions to file
                    writen(logFile, line, (int) n);
                    //Print on screen
                    printf("\t%s", line);
                }
                if (--num_fd <= 0) {
                    break;
                }
            }
        }
    }
}

unsigned int readline(const int fd, void *vptr, const size_t maxlen) {
    int n, rc;
    char c, *ptr;
    ptr = vptr;
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
            return (unsigned int) -1; //errno, errno set by read();
    }
    *ptr = 0;
    return (unsigned int) n;
}

unsigned int writen(const int fd, const void *vptr, const int n) {
    int n_left;
    int n_write;
    const char *nptr;
    nptr = vptr;
    n_left = n;
    while (n_left > 0) {
        if ((n_write = (int) write(fd, nptr, (size_t) n_left)) <= 0) {
            if (errno == EINTR)
                n_write = 0;
            else
                return (unsigned int) -1;
        }
        n_left -= n_write;
        nptr += n_write;
    }
    return (unsigned int) n;
}
