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
        // Duplicate write end of out pipe to STDOUT
        if (dup2(outPipefd[1], STDOUT_FILENO) != STDOUT_FILENO)
        {
            perror("dup2 error to stdout");
            return -1;
        }

        // Duplicate write end of out pipe to STDOUT
        if (dup2(errPipefd[1], STDERR_FILENO) != STDERR_FILENO)
        {
            perror("dup2 error to stderr");
            return -1;
        }

        // close unused file descriptors
        close(outPipefd[0]);
        close(errPipefd[0]);

        execlp(_PATH_BSHELL, "sh", "-c", string, NULL);
        _exit(127);
    }

    // Duplicate read end of out pipe to STDIN
    if (dup2(outPipefd[0], STDIN_FILENO) != STDIN_FILENO)
    {
        perror("dup2 error to stdin");
        return -1;
    }

    // close unused file descriptors
    close(outPipefd[1]);
    close(errPipefd[1]);

    // Block SIGCHLD
    sigemptyset(&sa.sa_mask);
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mask, &omask);

    // Ignore SIGINT and SIGQUIT
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    waitpid(pid, &pstat, 0);

    read(outPipefd[0], outbuf, outlen);
    read(errPipefd[0], errbuf, errlen);

    return (pid == -1 ? pid : pstat);
}