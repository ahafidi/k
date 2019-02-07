/*
 * K - hafidi_a
 * 2015-04-28
 */

#include "gdt.h"
#include "misc.h"

#include "../sdk/libc/stdio.h"
#include "../sdk/libc/stddef.h"

s_gdt_entry gdt[3]; // Automatically set to null

void gdt_pretty_printer(s_gdt_entry entry)
{
  clear_screen();

  printf("GDT entry at %p address\n", &entry);

  printf("|- Segment limit (first part)           = %p\n",
         entry.segment_limit_a);

  printf("|- Base Address (first part)            = %p\n",
         entry.base_address_a);

  printf("|- Base Address (second part)           = %p\n",
         entry.base_address_b);

  printf("|- Type (segment type)                  = %p\n",
         entry.type);

  printf("|- S (descriptor type)                  = %p\n",
         entry.s);

  printf("|- DPL (descriptor privilege level)     = %p\n",
         entry.dpl);

  printf("|- P (segment present)                  = %p\n",
         entry.p);

  printf("|- Segment limit (second part)          = %p\n",
         entry.segment_limit_b);

  printf("|- Available for use by system software = %p\n",
         entry.avl);

  printf("|- L                                    = %p\n",
         entry.l);

  printf("|- DB                                   = %p\n",
         entry.db);

  printf("|- G                                    = %p\n",
         entry.g);

  printf("`- Base Address (third part)            = %p\n",
         entry.base_address_c);
}

static void build_gdt(s_gdt_entry* gdt)
{
  // First entry : Null segment
  // Already null, so nothing to do.

  // Second entry : Kernel Code
  gdt[1].base_address_a = 0;
  gdt[1].base_address_b = 0;
  gdt[1].base_address_c = 0;

  gdt[1].segment_limit_a = 0xffff;
  gdt[1].segment_limit_b = 0xf;

  gdt[1].type = 8; // Code Execute-Only
  gdt[1].g = 1; // the limit is interpreted as a number of pages
  gdt[1].s = 1; // code or data
  gdt[1].p = 1; // present
  gdt[1].db = 1; // 32bit segment

  // Third entry : Kernel Data
  gdt[2].base_address_a = 0;
  gdt[2].base_address_b = 0;
  gdt[2].base_address_c = 0;

  gdt[2].segment_limit_a = 0xffff;
  gdt[2].segment_limit_b = 0xf;

  gdt[2].type = 2; // Data Read-Write
  gdt[2].g = 1;
  gdt[2].s = 1;
  gdt[2].p = 1;
  gdt[2].db = 1;

  // All other fields stay null
  // clear_screen();
  // gdt_pretty_printer(gdt[2]);
}

static void load_gdt(s_gdt_entry* gdt)
{
  s_gdt_register gdtr;

  gdtr.limit = sizeof (s_gdt_entry) * 3 - 1;
  gdtr.base = (t_uint32) gdt;

  __asm__ volatile ("lgdt %0\n"
                    : // no output
                    : "m" (gdtr)
                    : "memory");

  // clear_screen();
  // printf("GDT Load. Limit=%d & Base=%p \n", gdtr.limit, gdtr.base);
}

void init_gdt(void)
{
  clear_screen();

  build_gdt(gdt);

  load_gdt(gdt);

  printf("GDT initialization : done.\n");
}

void load_segment_selectors(void)
{
  // Code Selector (eip is relative to this segment)
  // s_segmentation_register cs;
  // cs.index = 1;
  // cs = 0d8 = 0x8 = 0b1000
  __asm__ volatile ("ljmp $8, $1f\n"
                    "1:\t\n"
                    : // no output
                    : // no input
                    : /* no clobber */ );

  // Data Selector
  // s_segmentation_register ds;
  // ds.index = 2;
  // 0d16 = 0x10 = 0b10000
  __asm__ volatile ("mov $16, %%ax\n"
                    "mov %%ax, %%ds\n"
                    : // no output
                    : //"r" (ds)
                    : "eax");

  printf("Segment selectors loading : done.\n");
}
