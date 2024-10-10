// Simplified Physical Memory Layout for RV32I

#define UART0 0x10000000L
#define UART0_IRQ 10

// virtio mmio interface
#define VIRTIO0 0x10001000
#define VIRTIO0_IRQ 1

// qemu puts platform-level interrupt controller (PLIC) here.
#define PLIC 0x0c000000L
#define PLIC_PRIORITY (PLIC + 0x0)
#define PLIC_PENDING (PLIC + 0x1000)
#define PLIC_SENABLE(hart) (PLIC + 0x2080 + (hart)*0x100)
#define PLIC_SPRIORITY(hart) (PLIC + 0x201000 + (hart)*0x2000)
#define PLIC_SCLAIM(hart) (PLIC + 0x201004 + (hart)*0x2000)

// Kernel starts lower in RV32I systems
#define KERNBASE 0x80000000L     // Start kernel at 1MB
#define PHYSTOP (KERNBASE + 32*1024*1024) // Up to 32MB of RAM

// User memory layout. This can be simplified or customized for RV32I
// No trampoline or trapframe necessary for now.

// map kernel stacks beneath the trampoline,
// each surrounded by invalid guard pages.
#define KSTACK(p) (PHYSTOP - ((p)+1)* 2*PGSIZE)