#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    printf("only once\n");
    int pid = fork();
    wait(0); // 等待子进程结束
    printf("twice\n");
    if (pid == 0) {
        // 子进程执行
        printf("Subprocess\n");
    } else if (pid > 0) {
        // 父进程执行
        wait(0); // 等待子进程结束
        printf("parent process\n");
    } else {
        printf("fork failed");
    }
    exit(0);
}