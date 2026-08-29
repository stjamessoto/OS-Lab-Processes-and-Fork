// Challenge 6 — Parent writes 1..10 into a pipe; child reads and prints the sum.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];
    if (pipe(fd) == -1) { perror("pipe"); return 1; }

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        // Child: read numbers, compute sum
        close(fd[1]);
        FILE *in = fdopen(fd[0], "r");
        int num, sum = 0;
        while (fscanf(in, "%d", &num) == 1)
            sum += num;
        fclose(in);
        printf("Sum = %d\n", sum);
        fflush(stdout);
        _exit(0);
    } else {
        // Parent: write 1..10
        close(fd[0]);
        FILE *out = fdopen(fd[1], "w");
        for (int i = 1; i <= 10; ++i)
            fprintf(out, "%d\n", i);
        fclose(out);
        waitpid(pid, NULL, 0);
    }
    return 0;
}
