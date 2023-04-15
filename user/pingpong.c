#include <kernel/types.h>
#include <user/user.h>
#include <stddef.h>

void Pipe(int *fd) {
    int r = pipe(fd);
    if (r < 0) {
        exit(-1);
    }
}

int main(int argc, char *argv[]) {
    int parent_to_son[2];
    int son_to_parent[2];
    Pipe(parent_to_son);
    Pipe(son_to_parent);
    
    int pid = fork();
    if (pid == 0) {
        // 子进程
        close(parent_to_son[1]);
        close(son_to_parent[0]);

        char c;
        read(parent_to_son[0], &c, 1);
        close(parent_to_son[0]);
        fprintf(1, "%d: received ping\n", getpid());
        
        write(son_to_parent[1], &c, 1);
        close(son_to_parent[1]);
        exit(0);
    } else {
        close(parent_to_son[0]);
        close(son_to_parent[1]);

        char msg = 'a';
        write(parent_to_son[1], &msg, 1);
        close(parent_to_son[1]);

        wait(NULL);
        read(son_to_parent[0], &msg, 1);
        close(son_to_parent[0]);
        fprintf(1, "%d: received pong\n", getpid());
        exit(0);
    }
}
