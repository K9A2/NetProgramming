#include <stdio.h>
#include <stdlib.h>

#include <time.h>

void PrintErrorAndExit(char *errorStr)
{
	printf("%s\n", errorStr);
	printf("Program terminated.\n");
	exit(-1);
}

void printCurrentTime()
{
	time_t localTime;				 /*define a longint time varible*/
	localTime = time(NULL);			 /*system time and date*/
	printf("%s", ctime(&localTime)); /*english format output*/
}
