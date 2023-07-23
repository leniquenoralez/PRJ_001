#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <paths.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

int command(const char *string, char *outbuf, int outlen, char *errbuf, int errlen)
{
    pid_t pid;

    int status, pstat;
    int outPipefd[2];
    int errPipefd[2];

    int outPipe = pipe(outPipefd);
    int errPipe = pipe(errPipefd);

    sigset_t mask, omask;
    struct sigaction sa = {
        .sa_handler = SIG_IGN,
        .sa_flags = 0,
        .sa_mask = 0,
    };


    if (outPipe == -1 || errPipe == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {
        close(STDOUT_FILENO);
        dup(outPipefd[1]);

        close(STDERR_FILENO);
        dup(errPipefd[1]);

        close(outPipefd[0]);
        close(errPipefd[0]);

        execlp(_PATH_BSHELL, "sh", "-c", string, NULL);
        _exit(127);
    }
    close(STDIN_FILENO);

    sigemptyset(&sa.sa_mask);
    dup(outPipefd[0]);
    close(outPipefd[1]);
    close(errPipefd[1]);

    sigemptyset(&mask);

    sigaddset(&mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mask, &omask);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    waitpid(pid, &pstat, 0);

    read(outPipefd[0], outbuf, outlen);
    read(errPipefd[0], errbuf, errlen);
    return (pid == -1 ? pid : pstat);
}