// Challenge 3 — execlp() to run ls -la
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }
    if (pid == 0) {
        execlp("ls", "ls", "-la", (char *)NULL);
        perror("execlp");
        _exit(1);
    } else {
        waitpid(pid, NULL, 0);
        printf("[parent] child finished listing directory\n");
    }
    return 0;
}
