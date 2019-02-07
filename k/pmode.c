#include "pmode.h"

#include "../sdk/libc/stdio.h"
#include "../sdk/libc/stddef.h"

void switch_to_pmode(void)
{
  s_cr0_register cr0;

  __asm__ volatile ("movl %%cr0, %0\n"
                    : "=r" (cr0) // output
                    : // no input
                    : );

  // Real-Address Mode -> Protected Mode
  cr0.pe = 1;

  __asm__ volatile ("movl %0, %%eax\n"
                    "movl %%eax, %%cr0\n"
                    : // no output
                    : "r" (cr0) // input
                    : "eax");

  // clear_screen();
  // printf("PE   = %p\n", cr0.pe);
  // printf("MISC = %p\n", cr0.reserved);

  printf("Switching to protected mode : done.\n");
}
