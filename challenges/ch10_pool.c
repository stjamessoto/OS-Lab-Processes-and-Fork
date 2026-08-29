// Challenge 10 — Parallel worker pool with bounded concurrency.
// Usage: ./ch10_pool M item1 item2 ... itemK
// Spawns at most M child workers at once. Each worker "processes" one item
// (here: sleeps briefly and prints), then the parent replaces it with the next.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static void do_work(const char *item) {
    printf("  [worker PID=%d] processing '%s'\n", getpid(), item);
    fflush(stdout);
    sleep(1); // simulate work
    printf("  [worker PID=%d] done '%s'\n", getpid(), item);
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s M item1 [item2 ...]\n", argv[0]);
        return 1;
    }
    int m = atoi(argv[1]);
    if (m <= 0) { fprintf(stderr, "M must be positive\n"); return 1; }

    int total = argc - 2;
    int next = 0;      // index into items (argv[2 + i])
    int active = 0;    // currently running children

    while (next < total || active > 0) {
        // Launch up to M active children.
        while (active < m && next < total) {
            fflush(stdout); // flush parent buffer so child doesn't re-emit it
            pid_t pid = fork();
            if (pid < 0) { perror("fork"); return 1; }
            if (pid == 0) {
                do_work(argv[2 + next]);
                _exit(0);
            }
            printf("[parent] started worker PID=%d for '%s' (active=%d)\n",
                   pid, argv[2 + next], active + 1);
            next++;
            active++;
        }
        // Wait for one child to finish before launching more.
        if (active > 0) {
            int status;
            pid_t w = wait(&status);
            if (w > 0) {
                printf("[parent] worker PID=%d finished (status %d, active=%d)\n",
                       w, WIFEXITED(status) ? WEXITSTATUS(status) : -1, active - 1);
                active--;
            } else {
                perror("wait");
                break;
            }
        }
    }
    printf("[parent] all %d tasks processed (concurrency limit=%d)\n", total, m);
    return 0;
}
