// Challenge 1 — Single fork() lifecycle
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }
    if (pid == 0) {
        printf("Hello from child (PID=%d, PPID=%d)\n", getpid(), getppid());
        sleep(2);
        _exit(7);
    } else {
        int status;
        pid_t w = waitpid(pid, &status, 0);
        if (w == -1) { perror("waitpid"); return 1; }
        if (WIFEXITED(status))
            printf("Parent: child %d exited with status %d\n", w, WEXITSTATUS(status));
        else
            printf("Parent: child %d did not exit normally\n", w);
    }
    return 0;
}
