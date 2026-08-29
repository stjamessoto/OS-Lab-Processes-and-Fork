// Challenge 2 — Multiple children in a loop
// Usage: ./ch2_multi_child N
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s N\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    if (n <= 0) { fprintf(stderr, "N must be positive\n"); return 1; }

    for (int i = 0; i < n; ++i) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); return 1; }
        if (pid == 0) {
            printf("[child %d] PID=%d exiting with code %d\n", i, getpid(), i + 1);
            _exit(i + 1);
        }
    }

    // Parent waits for all children in the order they finish.
    int order = 0;
    for (int i = 0; i < n; ++i) {
        int status;
        pid_t w = wait(&status);
        if (w == -1) { perror("wait"); break; }
        if (WIFEXITED(status))
            printf("[parent] (#%d finished) child %d exited with status %d\n",
                   order++, w, WEXITSTATUS(status));
        else
            printf("[parent] child %d did not exit normally\n", w);
    }
    return 0;
}
