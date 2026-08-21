// worker.c — helper for Challenge 4. Prints its args and MYVAR env var.
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; ++i)
        printf("worker: argv[%d]=%s\n", i, argv[i]);
    const char *v = getenv("MYVAR");
    printf("worker: MYVAR=%s\n", v ? v : "(unset)");
    return 0;
}
