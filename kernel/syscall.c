#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"
#include "defs.h"

// Fetch the uint32 at addr from the current process.
int
fetchaddr(uint32 addr, uint32 *ip)
{
  struct proc *p = myproc();
  if(addr >= p->sz || addr+sizeof(uint32) > p->sz) // both tests needed, in case of overflow
    return -1;
  if(copyin(p->pagetable, (char *)ip, addr, sizeof(*ip)) != 0)
    return -1;
  return 0;
}

// Fetch the nul-terminated string at addr from the current process.
// Returns length of string, not including nul, or -1 for error.
int
fetchstr(uint32 addr, char *buf, int max)
{
  struct proc *p = myproc();
  if(copyinstr(p->pagetable, buf, addr, max) < 0)
    return -1;
  return strlen(buf);
}

static uint32 argraw(int n)
{
  struct proc *p = myproc();
  switch (n) {
    case 0:
      return p->context.a0;
    case 1:
      return p->context.a1;
    case 2:
      return p->context.a2;
    case 3:
      return p->context.a3;
    case 4:
      return p->context.a4;
    case 5:
      return p->context.a5;
    default:
      panic("argraw");
      return -1;
  }
}


// Fetch the nth 32-bit system call argument.
void
argint(int n, int *ip)
{
  *ip = argraw(n);
}

// Retrieve an argument as a pointer.
// Doesn't check for legality, since
// copyin/copyout will do that.
void
argaddr(int n, uint32 *ip)
{
  *ip = argraw(n);
}

// Fetch the nth word-sized system call argument as a null-terminated string.
// Copies into buf, at most max.
// Returns string length if OK (including nul), -1 if error.
int
argstr(int n, char *buf, int max)
{
  uint32 addr;
  argaddr(n, &addr);
  return fetchstr(addr, buf, max);
}

// Prototypes for the functions that handle system calls.
//extern uint32 sys_fork(void);
//extern uint32 sys_exit(void);
//extern uint32 sys_wait(void);
extern uint32 sys_pipe(void);
extern uint32 sys_read(void);
//extern uint32 sys_kill(void);
extern uint32 sys_exec(void);
extern uint32 sys_fstat(void);
extern uint32 sys_chdir(void);
extern uint32 sys_dup(void);
//extern uint32 sys_getpid(void);
//extern uint32 sys_sbrk(void);
//extern uint32 sys_sleep(void);
//extern uint32 sys_uptime(void);
extern uint32 sys_open(void);
extern uint32 sys_write(void);
extern uint32 sys_mknod(void);
extern uint32 sys_unlink(void);
extern uint32 sys_link(void);
extern uint32 sys_mkdir(void);
extern uint32 sys_close(void);

// An array mapping syscall numbers from syscall.h
// to the function that handles the system call.
static uint32 (*syscalls[])(void) = {
//[SYS_fork]    sys_fork,
//[SYS_exit]    sys_exit,
//[SYS_wait]    sys_wait,
[SYS_pipe]    sys_pipe,
[SYS_read]    sys_read,
//[SYS_kill]    sys_kill,
[SYS_exec]    sys_exec,
[SYS_fstat]   sys_fstat,
[SYS_chdir]   sys_chdir,
[SYS_dup]     sys_dup,
//[SYS_getpid]  sys_getpid,
//[SYS_sbrk]    sys_sbrk,
//[SYS_sleep]   sys_sleep,
//[SYS_uptime]  sys_uptime,
[SYS_open]    sys_open,
[SYS_write]   sys_write,
[SYS_mknod]   sys_mknod,
[SYS_unlink]  sys_unlink,
[SYS_link]    sys_link,
[SYS_mkdir]   sys_mkdir,
[SYS_close]   sys_close,
};

void syscall(void)
{
  int num;
  struct proc *p = myproc();

  // System call number is in a7
  num = p->context.a7;

  if (num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    // Call the syscall handler and store the return value in a0
    p->context.a0 = syscalls[num]();
  } else {
    printf("%d %s: unknown sys call %d\n", p->pid, p->name, num);
    p->context.a0 = -1;  // Indicate an error
  }
}
