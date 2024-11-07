//----------------------------------------------------------------
//
//  4190.307 Operating Systems (Fall 2024)
//
//  Project #4: xSwap: Compressed Swap for xv6
//
//  November 7, 2024
//
//  Jin-Soo Kim (jinsoo.kim@snu.ac.kr)
//  Systems Software & Architecture Laboratory
//  Dept. of Computer Science and Engineering
//  Seoul National University
//
//----------------------------------------------------------------

#include "param.h"
#include "types.h"
#include "memlayout.h"
#include "elf.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"
#include "xswap.h"

int nalloc4k, zalloc4k, zalloc2k;
int nswapin, nswapout;


uint64
sys_memstat()
{
  // FILL HERE








  return 0;
}


// Called when ^x is pressed
void
mallocstat(void)
{
  printf("total: %d, nalloc4k: %d, zalloc4k: %d, zalloc2k: %d, swapin: %d, swapout: %d\n",
    nalloc4k+zalloc4k+zalloc2k, nalloc4k, zalloc4k, zalloc2k, nswapin, nswapout);
}

