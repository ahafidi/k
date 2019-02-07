/*
 * K - hafidi_a
 */

#include "keyboard.h"

#include "misc.h"
#include "libvga_io.h"

#include "../sdk/libc/stdio.h"
#include "../sdk/libc/stddef.h"

unsigned char key;

void handler_keyboard(void)
{
  // Clear the keyboard buffer
  // 0x60 : I/O Buffer
  INB(0x60, key);

  /*
  clear_screen();
  // 0x80 = 0b1000 0000
  // 0x7F = 0b0111 1111
  if (key & 0x80) // Key press (clear) or release (set)
    printf("A key is released. Scancode : %d\n", key & 0x7F);
  else
    printf("A key is pressed. Scancode : %d\n", key & 0x7F);
  */

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
  OUTB(0x20, 0x20); // send to master only because the keyboard is on master
}
