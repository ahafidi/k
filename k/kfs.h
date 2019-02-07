/*
 * K - hafidi_a
 */

#ifndef KFS_H_
# define KFS_H_

# include "multiboot.h"
# include "kstd.h"

# define KFS_MAGIC 0xd35f9caa
# define KFS_BLK_SZ 4096
# define KFS_BLK_DATA_SZ (KFS_BLK_SZ - 3 * 4)
# define KFS_DIRECT_BLK 10
# define KFS_INDIRECT_BLK 16
# define KFS_INDIRECT_BLK_CNT 16
# define KFS_FNAME_SZ 32
# define KFS_NAME_SZ 32
# define KFS_FD_AVAILABLE 64

typedef struct
{
  t_uint32 block_index;
  t_uint32 block_usage;
  t_uint32 checksum;
  t_uint8 data[KFS_BLK_DATA_SZ]; // char ?
} __attribute__((packed)) kfs_direct_block;

typedef struct
{
  t_uint32 block_index;
  t_uint32 blocks_count;
  t_uint32 data_blocks_indexes[KFS_INDIRECT_BLK_CNT];
  t_uint32 checksum;
} __attribute__((packed)) kfs_indirect_block;

typedef struct
{
  t_uint32 inumber;
  char filename[KFS_FNAME_SZ];
  t_uint32 file_size;
  t_uint32 blocks_count;
  t_uint32 inode_block_index;
  t_uint32 next_inode_block;
  t_uint32 direct_blocks_count;
  t_uint32 indirect_blocks_count;
  t_uint32 direct_blocks_indexes[KFS_DIRECT_BLK];
  t_uint32 indirect_blocks_indexes[KFS_INDIRECT_BLK];
  t_uint32 checksum;
} __attribute__((packed)) kfs_inode;

typedef struct
{
  t_uint32 magic_number;
  char fs_name[KFS_NAME_SZ];
  t_uint32 creation_timestamp; // t_sint32 ?
  t_uint32 blocks_count;
  t_uint32 inode_count;
  t_uint32 first_inode_block;
  t_uint32 checksum;
} __attribute__((packed)) kfs_superblock;

void init_kfs(multiboot_info_t* info);

#endif /* !KFS_H_ */
