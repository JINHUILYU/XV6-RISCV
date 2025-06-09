// user/execdemo.c
#include "kernel/types.h"
#include "user/user.h"

int main() {
    char *argv[3];
    argv[0] = "echo";
    argv[1] = "hello";
    argv[2] = 0;

    exec("echo", argv);
    // exec 成功不会到达这里，失败才会执行
    printf("exec error\n");
    exit(1);
}