#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(char *dirname, char *filename) {
    int fd = open(dirname, 0);

    if (fd < 0) {
        fprintf(2, "find: directory %s doesnt exists\n", dirname);
        return;
    }


    struct stat st;
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", dirname);
        close(fd);
        return;
    }

    if (st.type != T_DIR) {
        fprintf(2, "find: %s isn't a directory\n");
        close(fd);
        return;
    }

    char buf[512]; // 用来拼接文件夹名和文件名
    if (strlen(dirname) + 1 + DIRSIZ + 1 > sizeof(buf)){
        fprintf(1, "find: path too long\n");
        return;
    }
    strcpy(buf, dirname);
    char *end = buf + strlen(buf);

    struct dirent de;

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) { // 这时 de.name 是空，并且被认为是 T_DIR，导致错误递归
            continue;
        }
        if (strcmp(de.name, ".") == 0) {
            continue;
        }
        if (strcmp(de.name, "..") == 0) {
            continue;
        }
        char *p = end;
        *p++ = '/';
        memmove(p, de.name, DIRSIZ);
        p[strlen(de.name)] = 0;

        if (stat(buf, &st) < 0) {
            fprintf(1, "find: cannot stat %s\n", buf);
        }
        
        switch (st.type) {
            case T_FILE:
                if (strcmp(de.name, filename) == 0) {
                    fprintf(1, "%s\n", buf);
                }
                break;
            case T_DIR:
                find(buf, filename);
                break;
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(1, "find: missing operands\n");
    }

    find(argv[1], argv[2]);

    exit(0);
}
