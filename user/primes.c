#include "kernel/types.h"
#include "user/user.h"
#include <stddef.h>

void Pipe(int *fd) {
    int r = pipe(fd);
    if (r < 0) {
        exit(-1);
    }
}

void primes(int *fd) {
    close(fd[1]); // fd 只用来读

    int prime = 0;
    read(fd[0], &prime, 1);
    fprintf(1, "prime %d\n", prime);

    int p = 0; // 这里因为 int 是四字节，prime 和 p 都要初始化
    // 更好的做法是改成读或者写四字节，或者使用 char 代替 int

    if (read(fd[0], &p, 1) == 0) { // 递归终止
        close(fd[0]);
        exit(0);
    } else {
        int new_fd[2]; // new_fd 用来给子进程传
        Pipe(new_fd);

        int pid = fork();
        if (pid == 0) {
            primes(new_fd);
        } else {
            close(new_fd[0]);
 
            while (1) {
                if (p % prime != 0) {
                    write(new_fd[1], &p, 1);
                }
                if (read(fd[0], &p, 1) == 0) {
                    close(fd[0]);
                    close(new_fd[1]);
                    wait(NULL);
                    exit(0);
                }
            }
        }
    }
}

int main() {
    int fd[2];
    Pipe(fd);

    int pid = fork();

    if (pid == 0) {
        primes(fd);
    } else {
        close(fd[0]);

        for (int i = 2; i < 36; ++i) {
            write(fd[1], &i, 1);
        }
        
        close(fd[1]);
        wait(NULL);
    }

    exit(0);
}
