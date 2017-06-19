#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>

#include "dg_echo.c"

#define SERVER_UDP_PORT 3333

/*
This server receives the data from sender and returns it to the sender without
modifications.
*/
int main(int argc, char *argv[])
{

	int sockfd;
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;

	/* Open a UDP socket */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		PrintErrorAndExit("Can not open a UDP socket.");
	}
	printf("Opened UDP socket.\n");

	/* Load address for client */
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SERVER_UDP_PORT);

	//Bind the client address to its socket
	if ((bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) == -1) {
		PrintErrorAndExit("server: can not bind local address.");
	}
	printf("Binded address to socket.\n");

	//Echo received messages back to sender
	dg_echo(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr));

	return 0;

}
