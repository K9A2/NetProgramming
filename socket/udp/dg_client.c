#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "Utils.c"

#define MAX_BUFFER_SIZE 512

void dg_client(FILE *fp, int sockfd, struct sockaddr *server_addr, int servlen)
{

	int size;
	char sendBuffer[MAX_BUFFER_SIZE];
	char recvBuffer[MAX_BUFFER_SIZE + 1];
	char *buffer = sendBuffer;

	//Batch all data and send once
	while (fgets(buffer, MAX_BUFFER_SIZE, fp) != NULL)
	{
		buffer += strlen(buffer);
	}

	/* Send batched data to remote server */
	size = strlen(sendBuffer);
	if (sendto(sockfd, sendBuffer, size, 0, server_addr, servlen) != size)
	{
		PrintErrorAndExit("Can not send data to remote server.");
	}
	printf("Data sent.\n");

	/* Receive repliy from remote server */
	size = recvfrom(sockfd, recvBuffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)0, (socklen_t *)0);

	if (size < 0)
	{
		PrintErrorAndExit("Receives nothing from remote server.");
	}
	recvBuffer[size] = 0;
	printf("Received from server.\n");
	printCurrentTime();

	//Redirect output to screen
	fputs(recvBuffer, stdout);

}
