/*
 * K - hafidi_a
 * 2015-04-28
 */

#ifndef PMODE_H_
# define PMODE_H_

# include "kstd.h"

typedef struct
{
  t_uint32 pe : 1;
  t_uint32 reserved : 31;
} __attribute__((packed)) s_cr0_register;

void switch_to_pmode(void);

#endif /* PMODE_H_ */
