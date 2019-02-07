isr: // interruption sub-routine

  xchgl %eax, (%esp)

  // save registers
  // pushl %eax
  pushl %ebx
  pushl %ecx
  pushl %edx
  pushl %ds
  pushl %es
  pushl %fs
  pushl %gs
  pushl %ss
  call *%eax

  // restore registers
  popl %ss
  popl %gs
  popl %fs
  popl %es
  popl %ds
  popl %edx
  popl %ecx
  popl %ebx
  popl %eax

  iret

.global isr_timer
isr_timer:
  pushl $handler_timer
  jmp isr

.global isr_keyboard
isr_keyboard:
  pushl $handler_keyboard
  jmp isr
