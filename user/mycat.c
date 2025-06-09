#include "kernel/types.h"
#include "user/user.h"

int main() {
    char buf[512];
    int n;

    for(;;){
        n = read(0, buf, sizeof buf); // 从标准输入读取数据，n 是读取的字节数
        // 如果 n 为 0，表示 EOF（文件结束），则退出循环
        if(n == 0)
            break;
        // 如果 n 小于 0，表示读取错误，输出错误信息并退出
        // 使用 fprintf 输出错误信息到标准错误（文件描述符 2）
        if(n < 0){
            fprintf(2, "read error\n"); // 输出错误信息到标准错误
            exit(0);
        }
        // 将接收到的输入写入标准输出，实现回显
        // 使用 n 来确保写入的字节数与读取的字节数一致
        if(write(1, buf, n) != n){
            fprintf(2, "write error\n"); // 输出错误信息到标准错误
            exit(0);
        }
    }

    printf("%s", buf); // 输出当前的缓冲区内容到标准输出

    exit(0);
}