#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define SERVPORT 4444
//Maximum connection count
#define BACKLOG 10

/*
This program receives the messages from remote client and displays them on 
screen until user close it explicitly
*/
int main()
{

	//Socket descriptor
	int sockfd;
	int client_fd;

	int sin_size = sizeof(struct sockaddr_in);
	int recvbytes;

	char inputBuffer[256];
	char *input = inputBuffer;

	//Structure describing an IPv4 socket address
	struct sockaddr_in my_addr;
	struct sockaddr_in client_addr;

	//Set configurations
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SERVPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero), 8);

	//Create socket descriptor
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		perror("Unable to create socket: \n");
		printf("Program terminated.\n");
		exit(1);
	}

	//Bind a combination of IP and port to a specific socket
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("Unable to bind the address to socket: ");
		printf("\nProgram terminated.\n");
		exit(1);
	}

	/* 
	Server will listen to this socket until terminated.
	Client connect to this socket by invoke connect().
	*/
	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("Can not listen to this socket: ");
		printf("\nProgram terminated.\n");
		exit(1);
	}

	//Receive message from client
	while (1)
	{

		if ((client_fd = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&sin_size)) == -1)
		{
			perror("Unable to accept this incoming request: ");
			printf("\nProgram terminated.\n");
			continue;
		}

		printf("Received connection from: %s:%d.\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
		//Create a new process to manage this connection
		if (!fork())
		{
			//Send a welcome message to client
			if (send(client_fd, "Hello, you are connected!\n", 26, 0) == -1)
			{
				perror("Unable to send a message back to client: ");
				printf("\nProgram terminated.\n");
				close(client_fd);
				exit(0);
			}

			//Print all messages reveived from client
			while (1)
			{
				memset(inputBuffer, 0, sizeof(inputBuffer));
				recvbytes = recv(client_fd, inputBuffer, sizeof(inputBuffer), 0);
				inputBuffer[recvbytes] = '\0';
				printf("%s\n", inputBuffer);
				if (strcmp(input, "exit") == 0)
				{
					printf("Program terminated by remote client.\n");
					close(client_fd);
					exit(0);
				}
			}

			close(client_fd);
		}
	}
}
