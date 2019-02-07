/*
 * K - hafidi_a
 */
#include "idt.h"

#include "misc.h"
#include "gdt.h"

#include "timer.h"
#include "keyboard.h"

#include "../sdk/libc/stdio.h"
#include "../sdk/libc/stddef.h"

// extern s_gdt_entry gdt[3];

extern void isr_timer(void);
extern void isr_keyboard(void);

s_idt_entry idt[256]; // Automatically set to null

HANDLER_INTERRUPT_GATE(0, "divide error")
HANDLER_INTERRUPT_GATE(1, "debug exception")
HANDLER_INTERRUPT_GATE(2, "nmi interrupt")
HANDLER_INTERRUPT_GATE(3, "breakpoint")
HANDLER_INTERRUPT_GATE(4, "overflow")
HANDLER_INTERRUPT_GATE(5, "bound range exceeded")
HANDLER_INTERRUPT_GATE(6, "invalid opcode (undefined opcode)")
HANDLER_INTERRUPT_GATE(7, "device not available (no math coprocessor)")
HANDLER_INTERRUPT_GATE(8, "double fault")
HANDLER_INTERRUPT_GATE(9, "coprocessor segment overrun (reserved)")
HANDLER_INTERRUPT_GATE(10, "invalid TSS")
HANDLER_INTERRUPT_GATE(11, "segment not present")
HANDLER_INTERRUPT_GATE(12, "stack-segment fault")
HANDLER_INTERRUPT_GATE(13, "general protection")
HANDLER_INTERRUPT_GATE(14, "page fault")
HANDLER_INTERRUPT_GATE(15, "intel reserved")
HANDLER_INTERRUPT_GATE(16, "x87 FPU floating-point error (math fault)")
HANDLER_INTERRUPT_GATE(17, "alignment check")
HANDLER_INTERRUPT_GATE(18, "machine check")
HANDLER_INTERRUPT_GATE(19, "SIMD floating-point exception")
HANDLER_INTERRUPT_GATE(20, "virtualization exception")
HANDLER_INTERRUPT_GATE(21, "intel reserved")
HANDLER_INTERRUPT_GATE(22, "intel reserved")
HANDLER_INTERRUPT_GATE(23, "intel reserved")
HANDLER_INTERRUPT_GATE(24, "intel reserved")
HANDLER_INTERRUPT_GATE(25, "intel reserved")
HANDLER_INTERRUPT_GATE(26, "intel reserved")
HANDLER_INTERRUPT_GATE(27, "intel reserved")
HANDLER_INTERRUPT_GATE(28, "intel reserved")
HANDLER_INTERRUPT_GATE(29, "intel reserved")
HANDLER_INTERRUPT_GATE(30, "intel reserved")
HANDLER_INTERRUPT_GATE(31, "intel reserved")

void init_idt_entry(s_idt_entry* entry, void* addr_proc)
{
  unsigned int offset = (unsigned int) addr_proc;

  entry->offset_a = offset & 0x0000FFFF;
  entry->segment_selector = 0x8;

  // entry->reserved = entry->reserved; // unchanged
  entry->const_a = 0;
  entry->const_b = 0xE; // Ob01110
  entry->dpl = 0;
  entry->p = 1;
  entry->offset_b = (offset & 0xFFFF0000) >> 16;
}

void idt_pretty_printer(s_idt_entry* entry)
{
  clear_screen();

  printf("IDT entry at %p address\n", &entry);

  printf("|- Offset (first part)         = %p\n", entry->offset_a);
  printf("|- Segment Selector            = %p\n", entry->segment_selector);
  printf("|- Part defined (first part)   = %p\n", entry->const_a);
  printf("|- Part defined (second part)  = %p\n", entry->const_b);
  printf("|- DPL                         = %p\n", entry->dpl);
  printf("|- P                           = %p\n", entry->p);
  printf("`- Offset (second part)        = %p\n", entry->offset_b);
}

static void build_idt()
{
  // Native interruption
  init_idt_entry(&idt[0], &handler_interrupt_gate_0);
  init_idt_entry(&idt[1], &handler_interrupt_gate_1);
  init_idt_entry(&idt[2], &handler_interrupt_gate_2);
  init_idt_entry(&idt[3], &handler_interrupt_gate_3);
  init_idt_entry(&idt[4], &handler_interrupt_gate_4);
  init_idt_entry(&idt[5], &handler_interrupt_gate_5);
  init_idt_entry(&idt[6], &handler_interrupt_gate_6);
  init_idt_entry(&idt[7], &handler_interrupt_gate_7);
  init_idt_entry(&idt[8], &handler_interrupt_gate_8);
  init_idt_entry(&idt[9], &handler_interrupt_gate_9);
  init_idt_entry(&idt[10], &handler_interrupt_gate_10);
  init_idt_entry(&idt[11], &handler_interrupt_gate_11);
  init_idt_entry(&idt[12], &handler_interrupt_gate_12);
  init_idt_entry(&idt[13], &handler_interrupt_gate_13);
  init_idt_entry(&idt[14], &handler_interrupt_gate_14);
  init_idt_entry(&idt[15], &handler_interrupt_gate_15);
  init_idt_entry(&idt[16], &handler_interrupt_gate_16);
  init_idt_entry(&idt[17], &handler_interrupt_gate_17);
  init_idt_entry(&idt[18], &handler_interrupt_gate_18);
  init_idt_entry(&idt[19], &handler_interrupt_gate_19);
  init_idt_entry(&idt[20], &handler_interrupt_gate_20);
  init_idt_entry(&idt[21], &handler_interrupt_gate_21);
  init_idt_entry(&idt[22], &handler_interrupt_gate_22);
  init_idt_entry(&idt[23], &handler_interrupt_gate_23);
  init_idt_entry(&idt[24], &handler_interrupt_gate_24);
  init_idt_entry(&idt[25], &handler_interrupt_gate_25);
  init_idt_entry(&idt[26], &handler_interrupt_gate_26);
  init_idt_entry(&idt[27], &handler_interrupt_gate_27);
  init_idt_entry(&idt[28], &handler_interrupt_gate_28);
  init_idt_entry(&idt[29], &handler_interrupt_gate_29);
  init_idt_entry(&idt[30], &handler_interrupt_gate_30);
  init_idt_entry(&idt[31], &handler_interrupt_gate_31);

  // events
  init_idt_entry(&idt[32], &isr_timer);
  init_idt_entry(&idt[33], &isr_keyboard);
}

static void load_idt()
{
  s_idt_register idtr;

  idtr.limit = sizeof (s_idt_entry) * 256 - 1;
  idtr.base = (t_uint32) idt;

  __asm__ volatile ("lidt %0\n"
                    : // no output
                    : "m" (idtr)
                    : "memory");

  // clear_screen();
  // printf("IDT Load. Limit=%d & Base=%p \n", idtr.limit, idtr.base);
}

void init_idt(void)
{
  clear_screen();

  build_idt(idt);

  load_idt(idt);

  printf("IDT initialization : done.\n");
}
