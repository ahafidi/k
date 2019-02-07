/*
 * K - hafidi_a
 */

#ifndef IDT_H_
# define IDT_H_

# include "kstd.h"

// typedef unsigned int t_uint32; // already define in kstd.h

# define HANDLER_INTERRUPT_GATE(id, name)                                     \
void handler_interrupt_gate_##id(void)                                        \
{					                                      \
  printf("k: error: %d - %s\n", id, name);	                              \
  while (1)                                                                   \
    continue;                                                                 \
}                                                                             \

typedef struct
{
  t_uint32 limit : 16;
  t_uint32 base : 32;
} __attribute__((packed)) s_idt_register;

typedef struct
{
  t_uint32 offset_a : 16;
  t_uint32 segment_selector : 16;

  t_uint32 reserved : 5;
  t_uint32 const_a : 3;
  t_uint32 const_b : 5;
  t_uint32 dpl : 2;
  t_uint32 p : 1;
  t_uint32 offset_b : 16;
} __attribute__((packed)) s_idt_entry;

void init_idt_entry(s_idt_entry* entry, void* addr_proc);

void idt_pretty_printer(s_idt_entry* entry);

void init_idt(void);

#endif /* !IDT_H_ */
