#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[])
{

    struct hostent *h;
    char inputBuffer[256];
    char *input = inputBuffer;

    printf("Welcome to gethostby name.\n");
    
    /* Had not input a domain name */
    if (argv[1] == NULL)
    {
        printf("Please input a domain name, or you can quit by type exit.\n");
        //Input again
        scanf("%s", inputBuffer);
        if (strcmp(input, "exit") == 0)
        {
            printf("Program terminated.\n");
            exit(0);
        }
    }

    printf("The domain name you input is: \"%s\".\n", inputBuffer);

    h = gethostbyname((char *)inputBuffer);
    if (h == NULL)
    {
        printf("Unable to get the specific host: %s\n.", inputBuffer);
        printf("Program terminated.\n");
        exit(0);
    }
    else
    {
        printf("Host official name is: %s.\n", h->h_name);
        printf("Host aliases is:\n");
        int count = 1;
        char **aliases = h->h_aliases;
        while (*aliases != NULL)
        {
            printf("\t%d: %s\n", count++, *aliases++);
        }
        printf("The IP addresses of that host are:\n");
        count = 1;
        char **addresses = h->h_addr_list;
        while (*addresses != NULL)
        {
            printf("\t%d: %s\n", count++, inet_ntoa(*((struct in_addr *)*addresses++)));
        }
    }

    return 0;
}
