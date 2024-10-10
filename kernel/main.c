#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

void main() {
    // Basic initialization without paging or interrupt handling
    consoleinit();
    printfinit();
    printf("\nxv6 kernel is booting\n");
    
    // Initialize subsystems (without paging, PLIC, or traps)
    kinit();      // physical page allocator
    //printf("\nnow here\n");
    //kvminit(); //alllocate kernel pages for processes

    procinit();   // process table

    binit();      // buffer cache
    iinit();      // inode table
    fileinit();   // file table
    
      // first user process
    inituart_process();
    //userinit(); 
    // Start cooperative scheduler
    scheduler();
}

