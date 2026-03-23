#include "../kernel/types.h"
#include "../kernel/param.h"
#include "../kernel/memlayout.h"
#include "../kernel/procinfo.h"
#include "../user/user.h"

static const char* state_name(int s)
{
    switch(s){
        case 2:
            return "S";
        case 3:
            return "R";
        case 4:
            return "R";
        case 5:
            return "Z";
        case 1:
            return "U";
        default:
            return "?";
    }
}

static void sort_by_mem_desc(struct uproc *ps, int n)
{
    int i;
    int j;
    for(i = 0; i < n; i++){
        for(j = i + 1; j < n; j++){
            if(ps[j].sz > ps[i].sz){
                struct uproc t = ps[i];
                ps[i] = ps[j];
                ps[j] = t;
            }
        }
    }
}

static void print_snapshot(int iter, int batch_mode)
{
    struct uproc ps[NPROC];
    int n;
    int i;
    int running = 0;
    int zombie = 0;
    int used = 0;
    int sleeping = 0;
    uint64 proc_mem = 0;
    uint64 total_mem_kib = (PHYSTOP - KERNBASE) / 1024;
    uint64 used_mem_kib;
    uint64 free_mem_kib;
    int cpu_est;

    n = getprocs(ps, NPROC);
    if(n < 0){
        fprintf(2, "top: getprocs failed\n");
        exit(1);
    }

    sort_by_mem_desc(ps, n);

    for(i = 0; i < n; i++){
        used++;
        if(ps[i].state == 4 || ps[i].state == 3)
            running++;
        else if(ps[i].state == 2)
            sleeping++;
        else if(ps[i].state == 5)
            zombie++;
        proc_mem += ps[i].sz;
    }

    used_mem_kib = proc_mem / 1024;
    free_mem_kib = (used_mem_kib >= total_mem_kib) ? 0 : (total_mem_kib - used_mem_kib);
    cpu_est = (used > 0) ? (running * 100 / used) : 0;

    if(!batch_mode)
        printf("\033[2J\033[H");

    printf("top - up %d ticks, snapshot %d\n", uptime(), iter);
    printf("Tasks: %d total, %d running, %d sleeping, %d zombie\n",
            used, running, sleeping, zombie);
    printf("%%Cpu(s): %d us, %d id  (estimation on xv6)\n",
            cpu_est, 100 - cpu_est);
    printf("KiB Mem : %lu total, %lu used, %lu free\n",
            total_mem_kib, used_mem_kib, free_mem_kib);
    printf("\n");
    printf("PID PPID S KILL MEM(KiB) COMMAND\n");

    for(i = 0; i < n; i++){
        printf("%d %d %s %d %lu %s\n",
            ps[i].pid,
            ps[i].ppid,
            state_name(ps[i].state),
            ps[i].killed,
            ps[i].sz / 1024,
            ps[i].name);
    }
}

static int wait_interval_or_quit(int interval_ticks, int batch_mode)
{
    int t;
    int c;

    if(batch_mode){
        sleep(interval_ticks);
        return 0;
    }

    for(t = 0; t < interval_ticks; t++){
        c = getkey();
        if(c == 'q' || c == 'Q')
            return 1;
        sleep(1);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int interval = 10;
    int rounds = -1;
    int loop_forever = 0;
    int batch_mode = 0;
    int iter = 1;
    int i;

    for(i = 1; i < argc; i++){
        if(strcmp(argv[i], "-d") == 0){
            if(i + 1 >= argc){
                fprintf(2, "usage: top [-b] [-d ticks] [-n rounds] [-l]\n");
                exit(1);
            }
            interval = atoi(argv[++i]);
        } else if(strcmp(argv[i], "-n") == 0){
            if(i + 1 >= argc){
                fprintf(2, "usage: top [-b] [-d ticks] [-n rounds] [-l]\n");
                exit(1);
            }
            rounds = atoi(argv[++i]);
        } else if(strcmp(argv[i], "-b") == 0){
            batch_mode = 1;
        } else if(strcmp(argv[i], "-l") == 0){
            loop_forever = 1;
        } else {
            fprintf(2, "usage: top [-b] [-d ticks] [-n rounds] [-l]\n");
            exit(1);
        }
    }

    if(interval <= 0)
        interval = 10;
    if(rounds == 0)
        rounds = 1;

    if(loop_forever)
        rounds = -1;

    if(!batch_mode)
        printf("(top) press q to quit\n");

    while(rounds < 0 || iter <= rounds){
        print_snapshot(iter, batch_mode);
        iter++;
        if(rounds < 0 || iter <= rounds){
        if(wait_interval_or_quit(interval, batch_mode))
            break;
        }
    }

    exit(0);
}
