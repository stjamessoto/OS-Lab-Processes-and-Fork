// Challenge 9 — Create & observe a zombie process.
// Child exits immediately; parent sleeps 10s before wait().
// During the sleep, run `ps -l` in another terminal to see the child in Z state.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }
    if (pid == 0) {
        printf("[child] PID=%d exiting now\n", getpid());
        _exit(0);
    } else {
        printf("[parent] PID=%d, child %d is now a ZOMBIE for 10s\n", getpid(), pid);
        printf("[parent] observe with: ps -l   (look for state Z)\n");
        fflush(stdout);
        sleep(10);
        wait(NULL);   // reap -> zombie disappears
        printf("[parent] child reaped; zombie removed\n");
    }
    return 0;
}
