// pipeline_demo.c — implements `ls | grep <pattern>` using fork twice + one pipe.
// Usage: ./pipeline_demo [pattern]   (default pattern: "\.c$")
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    const char *pattern = (argc > 1) ? argv[1] : "\\.c$";

    int fd[2];
    if (pipe(fd) == -1) { perror("pipe"); exit(1); }

    // Child 1: ls -> pipe write end
    pid_t p1 = fork();
    if (p1 < 0) { perror("fork"); exit(1); }
    if (p1 == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execlp("ls", "ls", (char *)NULL);
        perror("execlp ls");
        _exit(1);
    }

    // Child 2: grep <- pipe read end
    pid_t p2 = fork();
    if (p2 < 0) { perror("fork"); exit(1); }
    if (p2 == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        close(fd[0]);
        execlp("grep", "grep", pattern, (char *)NULL);
        perror("execlp grep");
        _exit(1);
    }

    // Parent: close both ends and wait
    close(fd[0]);
    close(fd[1]);
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
    return 0;
}
