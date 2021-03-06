#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void) {
    return fork();
}

int
sys_exit(void) {
    exit();
    return 0;  // not reached
}

int
sys_set_cid(void) {
    int cid = 0;
    argint(0, &cid);
    proc->cid = cid;
    return 0;
}

int
sys_wait(void) {
    return wait();
}

int
sys_kill(void) {
    int pid;

    if (argint(0, &pid) < 0)
        return -1;
    return kill(pid);
}

int
sys_getpid(void) {
    return getpid();
}

int
sys_getppid(void) {
    return proc->parent->pid;
}

void waitpid(void) {
    int childPid;
    argint(0, &childPid);

}

int
sys_wait2(void) {
    return wait2();
}

int
sys_sem_init(void) {
    int sem;
    int value;

    if (argint(0, &sem) < 0)
        return -1;
    if (argint(1, &value) < 0)
        return -1;

    return sem_init(sem, value);
}

int
sys_sem_destroy(void) {
    int sem;

    if (argint(0, &sem) < 0)
        return -1;

    return sem_destroy(sem);
}

int sys_sem_wait(void) {
    int sem;
    int count;

    if (argint(0, &sem) < 0)
        return -1;
    if (argint(1, &count) < 0)
        return -1;

    return sem_wait(sem, count);
}

int
sys_sem_signal(void) {
    int sem;
    int count;

    if (argint(0, &sem) < 0)
        return -1;
    if (argint(1, &count) < 0)
        return -1;

    return sem_signal(sem, count);
}

int
sys_nice() {
    return nice();
}

int
sys_niceTwo() {

    //cprintf("in nice syscall , priority is : %d\n" , proc->priority);
    if (proc && proc->priority > 0) {
        (proc->priority) = proc->priority - 2;
        return 0;
    } else
        return -1;
}

int
sys_sbrk(void) {
    int addr;
    int n;

    if (argint(0, &n) < 0)
        return -1;
    addr = proc->sz;
    if (growproc(n) < 0)
        return -1;
    return addr;
}

int
sys_sleep(void) {
    int n;
    uint ticks0;

    if (argint(0, &n) < 0)
        return -1;
    acquire(&tickslock);
    ticks0 = ticks;
    while (ticks - ticks0 < n) {
        if (proc->killed) {
            release(&tickslock);
            return -1;
        }
        sleep(&ticks, &tickslock);
    }
    release(&tickslock);
    return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void) {
    uint xticks;

    acquire(&tickslock);
    xticks = ticks;
    release(&tickslock);
    return xticks;
}
