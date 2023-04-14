#include "kernel/types.h" // strlen 的 uint 在这里声明
#include "user/user.h" // syscall 和 ulib 的函数在这里声明

int main(int argc, char *argv[]) {
    char *err_msg = "sleep: missing operand\nTry 'sleep --help' for more information.";
    if (argc == 1) {
        write(2, err_msg, strlen(err_msg));
        exit(-1);
    }
    int time = atoi(argv[2]);
    sleep(time);
    exit(0);
}
