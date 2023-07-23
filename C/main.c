#include "command.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    const char *string = argv[1];
    char outbuf[BUFSIZ];
    char errbuf[BUFSIZ];


    if (!string)
    {
        printf("Usage: ./command [command]");
        return EXIT_FAILURE;
    }


    if (command(string, outbuf, BUFSIZ - 1, errbuf, BUFSIZ - 1) == -1)
    {
        printf("EXIT_FAILURE");
        perror("command");
        return EXIT_FAILURE;
    }
    outbuf[BUFSIZ - 1] = '\0';
    errbuf[BUFSIZ - 1] = '\0';
    printf("stdout:%s\n", outbuf);
    printf("stderr:%s\n", errbuf);
    return EXIT_SUCCESS;

}