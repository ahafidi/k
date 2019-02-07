/*
 * K - hafidi_a
 */

#include "timer.h"

#include "misc.h"
#include "libvga_io.h"

#include "../sdk/libc/stdio.h"
#include "../sdk/libc/stddef.h"

unsigned long tick;

void handler_timer(void)
{
  tick++;

  // Send an EOI : End Of Interrupt
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
  // 0b0010 0000 = 0x20
  OUTB(0x20, 0x20); // send to master only because the timer is on master
}
