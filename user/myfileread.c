#include "kernel/types.h"
#include "kernel/fcntl.h"   // for O_RDONLY
#include "user/user.h"

int main() {
    char *argv[2];
    argv[0] = "cat";
    argv[1] = 0;  // NULL terminated argv

    if(fork() == 0){
        // 子进程中关闭标准输入（fd = 0），以便 open 替代它
        close(0);
        open("data.txt", O_RDONLY);  // 此 open 返回 fd=0，选择最小的可用文件描述符
        exec("cat", argv); // 执行 cat 程序
        // 如果 exec 失败才会运行下面的 printf
        // exec 成功后不会返回到这里
        // 因为 exec 会用新的程序替换当前进程的映像
        printf("exec failed\n");
        exit(1);
    }

    wait(0); // 父进程等待子进程
    exit(0);
}