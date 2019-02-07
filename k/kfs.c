#include "kfs.h"

#include "misc.h"

#include "../sdk/libc/stddef.h"
#include "../sdk/libc/stdio.h"

#define ADLER32_MOD 65521

kfs_superblock* sblock = NULL;
kfs_inode* fd_table[KFS_FD_AVAILABLE];

static t_uint32 calc_checksum(const void* data, size_t sz)
{
  // adler32 algorithm
  const t_uint8* ptr = data;
  t_uint32 a = 1, b = 0;

  for (; sz; ++ptr, --sz)
  {
    a = (a + *ptr) % ADLER32_MOD;
    b = (b + a) % ADLER32_MOD;
  }
  return (b << 16) | a;
}

static void* get_block(t_uint32 index)
{
  char* r = (void*) sblock;
  r += index * KFS_BLK_SZ;
  return (void*) r;
}

static void init_superblock(multiboot_info_t* info)
{
  sblock = (void*) ((module_t*) info->mods_addr)[0].mod_start;
  if (sblock->magic_number != KFS_MAGIC)
  {
    printf("kfs: error: fail magic number\n");
    sblock = NULL;
    return;
  }

  clear_screen();
  printf("kfs: magic number checked\n");

  size_t sz_cksum_sblock = sizeof (kfs_superblock) - sizeof (sblock->checksum);
  t_uint32 cksum = calc_checksum(sblock, sz_cksum_sblock);
  if (cksum == sblock->checksum)
    printf("kfs: checksum checked\n");
  else
    printf("kfs: error: fail checksum\n");
}

static void init_fd_table(void)
{
  if (!sblock->inode_count || sblock->inode_count > 64)
    return; // either no file or not enough fd available

  fd_table[0] = get_block(sblock->first_inode_block);
  for (t_uint32 i = 1; i != sblock->inode_count; ++i)
    fd_table[i] = get_block(fd_table[i - 1]->next_inode_block);

  printf("kfs: file descriptor table initialized\n");
}

void init_kfs(multiboot_info_t* info)
{
  clear_screen();
  init_superblock(info);
  init_fd_table();

  /*
  printf("fs_name=%s\n", sblock->fs_name);
  printf("inode_count=%u\n", sblock->inode_count);

  for (t_uint32 i = 0; i < sblock->inode_count; ++i)
  {
    printf("filename=%s\n", fd_table[i]->filename);
    printf("inumber=%u\n", fd_table[i]->inumber);
  }
  */
}
