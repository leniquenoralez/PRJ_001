#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int command(const char *string, char *outbuf, int outlen, char *errbuf, int errlen)
{
    pid_t cmdpid;

    int outPipefd[2];
    int errPipefd[2];

    int outPipe = pipe(outPipefd);
    int errPipe = pipe(errPipefd);

    if (outPipe == -1 || errPipe == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cmdpid = fork();
    if (cmdpid < 0)
    {
        perror("fork");
        return -1;
    }

    if (cmdpid == 0)
    {

        close(STDOUT_FILENO);
        dup(outPipefd[1]);

        close(STDERR_FILENO);
        dup(errPipefd[1]);

        close(outPipefd[0]);
        close(errPipefd[0]);

        execlp("/bin/sh","sh", "-c", string, NULL);

        return 0;
    } else {

        if (waitpid(cmdpid, NULL, 0) < 0)
        {
            perror("waitpid error");
            return -1;
        }

        read(outPipefd[0], outbuf, outlen);
        return 0;
    }



    return 0;
}