#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVPORT 4444
#define MAXDATASIZE 100

/*
This program sends messages to server until user close it explicitly
*/
int main(int argc, char *argv[])
{
	//Socket descriptor
	int server_fd;
	int recvbytes;

	char buf[MAXDATASIZE];
	//Server
	struct hostent *host;
	//Structure describing an Internet (IP) socket address
	struct sockaddr_in server_addr;

	//Input Buffer
	char inputBuffer[256];
	char *input = inputBuffer;

	/* Had not input a server host name */
	if (argv[1] == NULL)
	{
		printf("Please input a server host name, or you can quit by type exit.\n");
		//Input again
		scanf("%s", inputBuffer);
		if (strcmp(input, "exit") == 0)
		{
			printf("Program terminated.\n");
			exit(0);
		}
		if ((host = gethostbyname(inputBuffer)) == NULL)
		{
			herror("Unable to process gethostbyname");
			printf("Program terminated.\n");
			exit(1);
		}
	}
	else if ((host = gethostbyname(argv[1])) == NULL)
	{
		herror("Unable to process gethostbyname");
		printf("Program terminated.\n");
		exit(1);
	}

	//Create socket descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		perror("Unable to create socket: \n");
		printf("Program terminated.\n");
		exit(1);
	}

	//AF_INET = IPv4
	server_addr.sin_family = AF_INET;
	//The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
	server_addr.sin_port = htons(SERVPORT);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero), 8);

	//Open socket to server
	if (connect(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		printf("Unable to connect to server: %s.\n", host->h_name);
		printf("Program terminated.\n");
		exit(1);
	}

	//Unable to receive a message from server
	if ((recvbytes = recv(server_fd, buf, MAXDATASIZE, 0)) == -1)
	{
		perror("Unable to receive message from server: ");
		printf("\nProgram terminated.\n");
		exit(1);
	}
	//Print welcome message
	buf[recvbytes] = '\0';
	printf("%s", buf);
	printf("Please input the custom messages that you want to send to server: %s.\n", inet_ntoa(server_addr.sin_addr));

	//Send custom messages to server
	while (1)
	{
		//Clear the input buffer to prepare next message
		memset(inputBuffer, 0, sizeof(inputBuffer));
		scanf("%s", inputBuffer);
		if ((send(server_fd, inputBuffer, strlen(inputBuffer), 0)) == -1)
		{
			perror("Unable to send a message back to client: ");
			printf("\nProgram terminated.\n");
			close(server_fd);
			exit(0);
		}
		if (strcmp(input, "exit") == 0)
		{
			printf("Program terminated by user.\n");
			close(server_fd);
			exit(0);
		}
	}

	close(server_fd);
	return 0;
}
