#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main() {
    int p[2];                    // 用于管道通信的文件描述符数组
    char *argv[2];
    argv[0] = "wc";              // 要执行的程序是 wc
    argv[1] = 0;                 // exec 参数必须以 NULL 结尾

    pipe(p);                     // 创建管道，p[0] 为读端，p[1] 为写端

    if(fork() == 0) {            // 子进程
        close(0);                // 关闭标准输入（fd = 0）
        dup(p[0]);               // 把 p[0] 复制到 fd = 0，相当于把标准输入重定向到管道读端
        close(p[0]);             // 关闭原始的 p[0]
        close(p[1]);             // 不再需要写端
        exec("wc", argv);   // 执行 wc 程序，从标准输入读取内容
        printf("exec failed\n"); // 如果 exec 失败，输出错误信息
        exit(1);                 // 退出子进程
    } else {                     // 父进程
        write(p[1], "hello world\n", 12); // 向管道写入数据
        close(p[0]);             // 父进程不读取
        close(p[1]);             // 写完关闭写端
        wait(0);                 // 等待子进程结束
    }

    exit(0);
}