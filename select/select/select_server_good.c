#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAXLINE 512
#define SERV_PORT 8003
#define LISTENQ 64

//static ssize_t my_read(int fd, char *ptr);
ssize_t readline(int fd, void *vptr, size_t maxlen);
//ssize_t: unsigned int
ssize_t writen(int fd, const void *vptr, int n);

int main(int argc, char **argv){
	int i,maxi, maxfd;
	//maxi: the max client value, see client below
	//maxfd: max file descriptor, used in select. fd is allocated by system.
	int listen_fd, connfd, sockfd;
	//listen_fd: server listening fd
	//connfd: server fd connecting to a client
	//sockfd: 
	int num_fd, client[FD_SETSIZE], files;
	//FD_SETSIZE is a macro const int, which is defined in sys/select.h,
	//it represents the maximum value of fd_set type, usually 1024.
	//Array client: used to record which fd is set in allset, 
	//if client[i]>0, client[i] is a fd to be checked. the front items of client array are >0 and the tails are <0
	//
	
	ssize_t n;
	fd_set rset, allset;
	char line[MAXLINE];
	socklen_t clilen;
	struct sockaddr_in client_addr,serv_addr;
	
	//open  file;
	files=open("./tmp.log",O_RDWR);
	//open success
	if(files>0){
		printf("The file tmp.log already exit, please move it.\nThen start this program again\n");
		exit(0);
	}
	//open fail, then creat file for write
	files=open("./tmp.log", O_WRONLY|O_CREAT|O_TRUNC);
	
	//INET Address
	listen_fd=socket(AF_INET, SOCK_STREAM, 0);
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(SERV_PORT);

	bind(listen_fd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
	listen(listen_fd,LISTENQ);
	maxfd=listen_fd;
	maxi=-1;

	for(i=0;i<FD_SETSIZE;i++){
		client[i]=-1;
	}

	//initialize allset
	FD_ZERO(&allset);
	//add listen_fd to allset
	FD_SET(listen_fd, &allset);
	
	while (1){
		rset=allset;
		num_fd=select(maxfd+1,&rset,NULL,NULL,NULL);
		
		//listen fd ready, accept it
		if(FD_ISSET(listen_fd,&rset)){						 
			//accept
			connfd=accept(listen_fd, (struct sockaddr *)&client_addr, &clilen);
			
			for(i=0;i<FD_SETSIZE;i++){
				//find the min client not filled with connfd
				if(client[i]<0){
					client[i]=connfd;
					break;
				}
			}
			if(i==FD_SETSIZE){		//all of clients are filled
					printf("too many clients\n");
					exit(0);
			}
			
			//add connect fd
			FD_SET(connfd, &allset);
			
			if(connfd>maxfd)
				maxfd=connfd;
			if(i>maxi)
				maxi=i;
			if(--num_fd<=0) continue;
		}	//end of if
		
		//some connfd ready
		for(i=0;i<=maxi;i++){
			if((sockfd=client[i])<0) continue;
			//if sockfd is set
			if(FD_ISSET(sockfd, &rset)){
				if((n=readline(sockfd, line,MAXLINE))==0){	//read end
					close(sockfd);
					//clear sockfd from allset
					FD_CLR(sockfd, &allset);

					client[i]=-1;
				}else 	//read not end 
					writen(files,line,n);
				if(--num_fd<=0) break;
			}	//end of if
		}		//enf of for
	}	//end of while
	close(files);
	close(listen_fd);
}

ssize_t readline(int fd, void *vptr, size_t maxlen){
	int n, rc;
	char c,*ptr;
	ptr=vptr;
	for(n=1;n<maxlen;n++){
		if((rc=read(fd,&c,1))==1){
			*ptr++=c;
			if(c=='\n') break;
		}else if(rc==0){
			if(n==1) return 0;
			else break;
		}else
			return -1;	//errno, errno set by read();
	}
	*ptr=0;
	return(n);
}

ssize_t writen(int fd, const void *vptr, int n){
	int n_left;
	int n_write;
	const char *nptr;
	nptr=vptr;
	n_left=n;
	while(n_left>0){
		if((n_write=write(fd,nptr,n_left))<=0){
			if(errno==EINTR) n_write=0;
			else return -1;
		}
		n_left-=n_write;
		nptr+=n_write;
	}
	return (n);
}
 
