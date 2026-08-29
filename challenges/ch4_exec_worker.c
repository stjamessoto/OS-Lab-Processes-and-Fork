// Challenge 4 — Exec an internal helper program with a custom environment.
// Assumes ./worker exists in the current directory (built from worker.c).
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }
    if (pid == 0) {
        char *argv[] = { "worker", "alpha", "beta", (char *)NULL };
        char *envp[] = { "MYVAR=hello", (char *)NULL };
        execle("./worker", "worker", "alpha", "beta", (char *)NULL, envp);
        // Fallback to PATH-independent execv if execle path differs:
        execv("./worker", argv);
        perror("execle/execv worker");
        _exit(1);
    } else {
        waitpid(pid, NULL, 0);
        printf("[parent] worker finished\n");
    }
    return 0;
}
