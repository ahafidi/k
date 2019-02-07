/*
 * K - hafidi_a
 */
#include "pic.h"

#include "misc.h"
#include "libvga_io.h"

#include "../sdk/libc/stdio.h"
#include "../sdk/libc/stddef.h"

static void init_icw_registers(void)
{
  // ICW : Initialization Command Word

  // ICW1 (port 0x20 / port 0xA0)
  //
  // 0 0 0 1 | x 0 x x
  //           |   | |ver u
  //           |   | +---- (1)
  //           |   +------ (2)
  //           +---------- (3)
  //
  // (1) ICW4 present (set) or not (clear)
  // (2) single controller (set) or cascade mode (clear)
  // (3) level triggered mode (set) or edge triggered mode (clear)
  //
  // Od17 = 0x11 = 0b0001 0001
  OUTB(0x20, 0x11); // master
  OUTB(0xA0, 0x11); // slave

  // ICW2 (port 0x21 / port 0xA1)
  //
  // x x x x | x 0 0 0
  // | | | |   |
  // +-+-+-+---+---------- (1)
  //
  // (1) Interrupt vector base address
  //
  // 0d32 = 0x20 = 0b0010 0000
  OUTB(0x21, 0x20); // master
  // 0d40 = 0x28 = 0b0010 1000
  OUTB(0xA1, 0x28); // slave


  // ICW3 (port 0x21 / port 0xA1)
  //
  // Master PICver u
  // x x x x | x x x x
  // | | | |   | | | |
  // +-+-+-+---+-+-+-+---- (1)
  //
  // (1) For each bit, indicate whether the corresponding IRQ
  //     is masked (set) or not (clear)
  //
  // Slave PIC
  // 0 0 0 0 | 0 x x x
  //             | | |
  //             +-+-+---- (2)
  //
  // (1) For each bit, indicate whether a slave PIC is connected to this
  //     pin (set) or not (clear)
  // (2) Indicate to the slave his slave ID (which pin of the master it
  //     is connected to)
  //
  // 0d4 = 0x04 = 0b0000 0100
  OUTB(0x21, 0x04); // master
  // 0d2 = 0x02 = 0b0000 0010
  OUTB(0xA1, 0x02); // slave

  // ICW4 (port 0x21 / port 0xA1)
  //
  // 0 0 0 x | x x x 1
  //       |   | | |
  //       |   | | +------ (1)
  //       |   +-+-------- (2)
  //       +-------------- (3)
  //
  // (1) Automatic (set) EOI or normal (clear) EOI
  // (2) Buffering mode
  // (3) Special mode fully nested (set) or not (clear)
  //
  // 0d1 = 0x01 = 0b0000 0001
  OUTB(0x21, 0x01); // master
  OUTB(0xA1, 0x01); // slave
}

static void init_ocw_registers(void)
{
  // OCW : Operation Control Word

  // OCW1 (port 0x21 / port 0xA1)
  //
  // x x x x | x x x x
  // | | | |   | | | |
  // +-+-+-+---+-+-+-+---- (1)
  //
  // (1) For each bit, indicate whether the corresponding IRQ
  //     is masked (set) or not (clear)
  OUTB(0x21, 0xFC); // master FC
  OUTB(0xA1, 0xFF); // master

  // OCW2 (port 0x20 / port 0xA0)
  //
  // x x x 0 | 0 x x x
  // | | |       | | |
  // | | |       | | |
  // | | |       +-+-+---- (1)
  // | | +---------------- (2)
  // | +------------------ (3)
  // +-------------------- (4)
  //
  // (1) Interrupt level to be acted upon when sending a specific command
  // (2) Send an EOI (end of interrupt command) (set)
  // (3) Send a specific (set) or a non-specific (clear) command
  // (4) Rotate priorities (set) or not (clear)
  //OUTB(0x20, 0x01); // master
  //OUTB(0xA0, 0x01); // master
}

static void init_pit(void)
{
  // 0x43 : Control Register
  //
  // c  c  io io m  m  m  b
  // ----  ----- -------  |
  //  |      |      |     +-- (1)
  //  |      |      +-------- (2)
  //  |      +--------------- (3)
  //  +---------------------- (4)
  //
  // (1) Binary counter (unset) or BCD counter (set)
  // (2) Mode to use, for example, 000 for mode 0, or 010 for mode 2
  // (3) Registers read/write policy:
  //    00: special mode to read the counters
  //    01: read/write the Least Significant Byte only
  //    10: read/write the Most Significant Byte only
  //    11: read/write LSB first, then MSB
  // (4) Counter to setup (00 - 10)
  //
  // 0d52 = 0x34 = 0b0011 0100
  OUTB(0x43, 0x34);

  // 0x40 : Counter 0
  // This register should be written with two outb : 2 * 8 = 16
  //
  // Interrupt rate : 100 Hz (desired frequency)
  // Input clock frequency = 1 193 182 Hz (internal frequency)
  // divider = internal_frequency / desired_frequency
  // divider = 11 931 = 0x2E9B
  OUTB(0x40, 0x9B);
  OUTB(0x40, 0x2E);
}

void init_pic(void)
{
  init_icw_registers();

  init_ocw_registers();

  init_pit();

  STI(); // enable interruptions

  clear_screen();

  printf("PIC 8259A initialization : done\n");
}
