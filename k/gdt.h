/*
 * K - hafidi_a
 * 2015-04-28
 */

#ifndef GDT_H_
# define GDT_H_

# include "kstd.h"

typedef struct
{
  t_uint16 limit;
  t_uint32 base;
} __attribute__((packed)) s_gdt_register;

typedef struct
{
  t_uint16 segment_limit_a;

  t_uint16 base_address_a;

  t_uint8 base_address_b;
  t_uint8 type : 4; // Segment type
  t_uint8 s : 1; // Descriptor type
  t_uint8 dpl : 2; // Descriptor Privilege Level
  t_uint8 p : 1; // Segment present

  t_uint8 segment_limit_b : 4;
  t_uint8 avl : 1; // Available for use by system software
  t_uint8 l : 1; // L 64-bit code segment (IA-32e mode only)
  t_uint8 db : 1; // D/B Default operation size
  t_uint8 g : 1; // Granularity
  t_uint8 base_address_c;
} __attribute__((packed)) s_gdt_entry;

/*
typedef struct
{
  t_uint16 rpl : 2; // Requested Privilege Level
  t_uint16 ti : 1; // Table Indicator (0 = GDT | 1 = LDT)
  t_uint16 index : 13;
} __attribute__((packed)) s_segmentation_register;
*/

void gdt_pretty_printer(s_gdt_entry entry);

void init_gdt(void);

void load_segment_selectors(void);

#endif /* !GDT_H_ */
