// zombie_demo.c — child exits immediately, parent sleeps before reaping.
// During the sleep window the child is a zombie (state Z). Observe with `ps -l`.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }
    if (pid == 0) {
        printf("[child] PID=%d exiting immediately\n", getpid());
        _exit(0);
    } else {
        printf("[parent] PID=%d created child %d; sleeping 10s (child is a zombie)\n",
               getpid(), pid);
        printf("[parent] run `ps -l` in another terminal to see child in Z state\n");
        fflush(stdout);
        sleep(10);
        int status;
        wait(&status);   // reap the zombie
        printf("[parent] reaped child; zombie gone\n");
    }
    return 0;
}
