/*
 * Copyright (c) LSE
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY LSE AS IS AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL LSE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "multiboot.h"
#include "kstd.h"

#include "misc.h"
#include "gdt.h"
#include "pmode.h"
#include "idt.h"
#include "pic.h"
#include "kfs.h"

#include "../sdk/libc/stddef.h"
#include "../sdk/libc/stdio.h"

void k_main(unsigned long magic, multiboot_info_t* info)
{
  (void) magic;
  (void) info;

  init_gdt();

  switch_to_pmode();

  load_segment_selectors();

  init_idt();

  init_pic();

  init_kfs(info);

  /*
  int er = 0;
  int eer = 1 / er;
  ++eer;
  */

  /*
  char star[4] = { '|', '/', '-', '\\' };
  unsigned i = 0;
  while (1)
  {
    clear_screen();
    ++i;
    printf("%c Kernel by hafidi_a %c\n \n", star[i % 4], star[i % 4]);

    printf("Tick = %d\n", gettick());
    printf("Key = %d\n", getkey());
    printf("Key = %d\n", getkey());
    printf("Tick = %d\n", gettick());

    for (unsigned j = 0; j < 100000000; ++j)
      continue;
  }
  */

  while(1);

  return;
}
