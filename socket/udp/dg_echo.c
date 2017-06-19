#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>

#include "Utils.c"

#define MAX_MESSAGE_SIZE 2048

void dg_echo(int sockfd, struct sockaddr *pcli_addr, int maxclientAddrLength)
{
	int recvByteCount;
	int clientAddrLength;
	char recvBuffer[MAX_MESSAGE_SIZE];

	while (1)
	{
		printf("Ready to receive data.\n");
		clientAddrLength = maxclientAddrLength;
		recvByteCount = recvfrom(sockfd, recvBuffer, MAX_MESSAGE_SIZE, 0, (struct sockaddr *)pcli_addr, (socklen_t *)&clientAddrLength);
		if (recvByteCount < 0)
		{
			PrintErrorAndExit("Received nothing.");
		}

		//Print received data
		printf("Data received at: ");
		printCurrentTime();
		recvBuffer[recvByteCount] = '\0';
		printf("%s", recvBuffer);
		printf("------------------------------------------\n");

		if (sendto(sockfd, recvBuffer, recvByteCount, 0, pcli_addr, clientAddrLength) != recvByteCount)
		{
			PrintErrorAndExit("Can not send reply to sender.");
		}
	}
}
