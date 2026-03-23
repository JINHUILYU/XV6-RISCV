// init: 第一个用户态进程，负责启动并守护 shell。

#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../kernel/spinlock.h"
#include "../kernel/sleeplock.h"
#include "../kernel/fs.h"
#include "../kernel/file.h"
#include "user.h"
#include "../kernel/fcntl.h"

// 传给 exec 的参数：argv[0] 是程序名。
char *argv[] = { "sh", 0 };

int main(void)
{
    int pid, wpid;

	// 确保控制台设备可用：若打开失败则创建 console 设备节点后重试。
    if(open("console", O_RDWR) < 0){
		mknod("console", CONSOLE, 0);
    	open("console", O_RDWR);
  	}
	// 复制 fd=0 到 fd=1/2，作为 stdout/stderr。
  	dup(0);  // stdout
  	dup(0);  // stderr

	// 持续拉起 shell：shell 退出后由 init 重新启动。
  	for(;;){
    	printf("init: starting sh\n");
		// fork 一个子进程去 exec shell。
    	pid = fork();
    	if(pid < 0){
      		printf("init: fork failed\n");
      		exit(1);
    	}
		// 子进程路径：exec 成功不会返回；返回表示 exec 失败。
		if(pid == 0){
			exec("sh", argv);
			printf("init: exec sh failed\n");
			exit(1);
		}
		// 父进程路径：等待 shell 或其他被托管（孤儿）子进程退出。
		for(;;){
			// this call to wait() returns if the shell exits,
			// or if a parentless process exits.
			wpid = wait((int *) 0);
			if(wpid == pid){
				// the shell exited; restart it.
				break;
			} else if(wpid < 0){
				printf("init: wait returned an error\n");
				exit(1);
			} else {
				// it was a parentless process; do nothing.
			}
		}
	}
}
