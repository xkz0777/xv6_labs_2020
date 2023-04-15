#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
#include <stddef.h>

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(1, "xargs: missing command\n");
        exit(-1);
    }
    char *new_argv[MAXARG];
    int i = 0;
    for (; i < argc - 1; ++i) {
        new_argv[i] = argv[i + 1];
    }

    char buf[512];
    int len = 0;
    int r = 0;
    while ((r = read(0, buf + len, 512 - len)) != 0) {
        len += r;
    }

    int base = 0;
    for (int j = 0; j < len; ++j) {
        if (buf[j] == '\n') {
            char *arg = (char *) malloc(512);
            memcpy(arg, buf + base, j - base);
            new_argv[i++] = arg;
            base = j + 1;
        }
    }

    int pid = fork();
    if (pid == 0) {
        exec(new_argv[0], new_argv);
    } else {
        wait(NULL);
    }
    exit(0);
}
