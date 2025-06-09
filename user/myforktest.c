#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int pid;
    pid = fork();
    if(pid > 0){
        sleep(10); // 等待子进城先输出
        printf("parent: child=%d\n", pid);
        pid = wait(0);
        printf("child %d is done\n", pid);
    } else if(pid == 0){
        printf("child: exiting\n");
        exit(0);
    } else {
        printf("fork error\n");
    }
    exit(0);
}