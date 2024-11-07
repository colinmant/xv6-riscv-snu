// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit()
{
#ifdef SNU
  printf("Physical memory layout:\n");
  printf("Kernel:      0x%lx - 0x%lx (%d MB, %d pages)\n",
      (uint64) KERNBASE, (uint64) PGROUNDUP((uint64)end),
      (int) ((PGROUNDUP((uint64)end) - KERNBASE) >> 20),
      (int) ((PGROUNDUP((uint64)end) - KERNBASE) >> 12));
  printf("ZONE_FIXED:  0x%lx - 0x%lx (%d MB, %d pages)\n",
      (uint64) PGROUNDUP((uint64)end), (uint64) NORMAL_START,
      (int) ((NORMAL_START - PGROUNDUP((uint64)end)) >> 20),
      (int) ((NORMAL_START - PGROUNDUP((uint64)end)) >> 12));
  printf("ZONE_NORMAL: 0x%lx - 0x%lx (%d MB, %d pages)\n",
      (uint64) NORMAL_START, (uint64) PHYSTOP,
      (int) ((PHYSTOP - NORMAL_START) >> 20),
      (int) ((PHYSTOP - NORMAL_START) >> 12));
  printf("ZONE_ZMEM:   0x%lx - 0x%lx (%d MB, %d pages)\n",
      (uint64) PHYSTOP, (uint64) ZMEMSTOP,
      (int) ((ZMEMSTOP - PHYSTOP) >> 20),
      (int) ((ZMEMSTOP - PHYSTOP) >> 12));
#endif
  initlock(&kmem.lock, "kmem");
#ifdef SNU
  // ZONE_NORMAL, ZONE_ZMEM should be initialized separately
  freerange(end, (void*)NORMAL_START);
#else
  freerange(end, (void*)PHYSTOP);
#endif
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
