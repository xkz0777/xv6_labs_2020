#include <kernel/types.h>
#include <user/user.h>
#include <stddef.h>

int main(int argc, char *argv[]) {
    int fd[2];
    int r = pipe(fd);
    if (r < 0) {
        exit(-1);
    }
    int pid = fork();
    if (pid == 0) {
        // 子进程
        close(fd[1]);
        char msg[2];
        read(fd[0], msg, 1);
        close(fd[0]);
        fprintf(1, "%d: received ping\n", getpid());
        exit(0);
    } else {
        close(fd[0]);
        char *msg = "a";
        write(fd[1], msg, 1);
        close(fd[1]);
        wait(NULL);
        fprintf(1, "%d: received pong\n", getpid());
        exit(0);
    }
}
