#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int pid = fork();

    if (pid < 0) {
        // 创建失败
        printf("fork failed");
        return 1;
    } else if (pid == 0) {
        // 子进程执行
        printf("Subprocess: PID=%d\n", getpid());
    } else {
        // 父进程执行
        wait(0); // 等待子进程结束
        printf("Parent process, subprocess: PID=%d\n", pid);
    }

    printf("Parent & Child PID=%d\n", getpid());

    exit(0);
}