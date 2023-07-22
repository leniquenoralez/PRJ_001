#include "command.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    const char *string = argv[1];
    char outbuf[BUFSIZ];
    char errbuf[BUFSIZ];
    if (command("ls -l", outbuf, BUFSIZ - 1, errbuf, BUFSIZ - 1) == -1)
    {
        perror("command");
        exit(EXIT_FAILURE);
    }
    outbuf[BUFSIZ - 1] = '\0';
    errbuf[BUFSIZ - 1] = '\0';
    printf("stdout:%s\n", outbuf);
    printf("stderr:%s\n", errbuf);
    return EXIT_SUCCESS;

}