// Saved registers for kernel context switches.
struct context {
  uint32 ra;
  uint32 sp;

  // callee-saved
  uint32 s0;
  uint32 s1;
  uint32 s2;
  uint32 s3;
  uint32 s4;
  uint32 s5;
  uint32 s6;
  uint32 s7;
  uint32 s8;
  uint32 s9;
  uint32 s10;
  uint32 s11;

  uint32 a0, a1, a2, a3, a4, a5, a6, a7;  // System call argument registers
  uint32 epc;
};

// Per-CPU state.
struct cpu {
  struct proc *proc;          // The process running on this cpu, or null.
  struct context context;     // swtch() here to enter scheduler().
  int noff;                   // Depth of push_off() nesting.
  int intena;                 // Were interrupts enabled before push_off()?
};

extern struct cpu cpus[NCPU];

enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// Per-process state
struct proc {
  enum procstate state;        // Process state
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  int xstate;                  // Exit status to be returned to parent's wait
  int pid;                     // Process ID

  struct proc *parent;         // Parent process

  uint32 kstack;               // Virtual address of kernel stack
  uint32 sz;                   // Size of process memory (bytes)
  pagetable_t pagetable;       // User page table
  struct context context;      // swtch() here to run process
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)
};

