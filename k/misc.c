/*
 * hafidi_a
 */

#include "misc.h"

#include "../sdk/libc/stdio.h"
#include "../sdk/libc/stddef.h"

void clear_screen(void)
{
  char tmp_buf[2] = { CONS_ESCAPE, CONS_CLEAR };
  write(tmp_buf, 2);
}
