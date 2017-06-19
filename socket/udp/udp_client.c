#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "dg_client.c"

#define LOCAL_HOST "127.0.0.1"
#define SERVER_PORT 3333

/*
This program sends messages to the localhost, and receives the replies.
Finally, after sending all data in a text file, it terminates itself.
*/
int main(int argc, char *argv[])
{

	/* Descriptor and Socket Address */
	int sockfd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	char filePath[] = "./data.txt";

	/* Load "server_addr" for target remote server */
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(LOCAL_HOST);
	server_addr.sin_port = htons(SERVER_PORT);

	/* Open a UDP socket */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		PrintErrorAndExit("Can not open a UDP socket.");
	}
	printf("Opened UDP socket.\n");

	/* Load address for server */
	bzero((char *)&client_addr, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	//This address receives all messages
	client_addr.sin_addr.s_addr = INADDR_ANY;
	client_addr.sin_port = htons(0);

	//Bind address to socket
	if (bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1)
	{
		PrintErrorAndExit("Can not bind local address to socket.");
	}
	printf("Binded address to socket.\n");

	//Get text data file descriptor
	FILE *file;
	if ((file = fopen(filePath, "r")) == NULL)
	{
		PrintErrorAndExit("Can not open data file.");
	}
	printf("Opened data file.\n");

	//Core process
	dg_client(file, sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	close(sockfd);
	return 0;
}
