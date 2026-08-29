// Challenge 8 — waitpid() with WNOHANG.
// Spawns 3 children sleeping 1,2,3s; parent polls without blocking.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int sleeps[3] = {1, 2, 3};

    for (int i = 0; i < 3; ++i) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); return 1; }
        if (pid == 0) {
            sleep(sleeps[i]);
            _exit(i + 1);
        }
    }

    int remaining = 3;
    while (remaining > 0) {
        int status;
        pid_t w = waitpid(-1, &status, WNOHANG);
        if (w > 0) {
            if (WIFEXITED(status))
                printf("[parent] child %d finished (status %d)\n", w, WEXITSTATUS(status));
            else
                printf("[parent] child %d ended abnormally\n", w);
            remaining--;
        } else if (w == 0) {
            // No child ready yet; do other work instead of blocking.
            usleep(100000); // 100ms
        } else {
            perror("waitpid");
            break;
        }
    }
    printf("[parent] all children reaped\n");
    return 0;
}
