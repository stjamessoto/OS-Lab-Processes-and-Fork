// Challenge 7 — Build a pipeline `ls | grep <pattern>` manually.
// Usage: ./ch7_pipeline <pattern>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <pattern>\n", argv[0]);
        return 1;
    }
    const char *pattern = argv[1];

    int fd[2];
    if (pipe(fd) == -1) { perror("pipe"); return 1; }

    pid_t p1 = fork();
    if (p1 < 0) { perror("fork"); return 1; }
    if (p1 == 0) {
        // ls -> pipe write end
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]); close(fd[1]);
        execlp("ls", "ls", (char *)NULL);
        perror("execlp ls"); _exit(1);
    }

    pid_t p2 = fork();
    if (p2 < 0) { perror("fork"); return 1; }
    if (p2 == 0) {
        // grep <- pipe read end
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]); close(fd[1]);
        execlp("grep", "grep", pattern, (char *)NULL);
        perror("execlp grep"); _exit(1);
    }

    close(fd[0]); close(fd[1]);
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
    return 0;
}
