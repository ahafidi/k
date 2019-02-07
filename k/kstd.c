#include "kstd.h"
#include "kfs.h"

// Default mode : Color Text Mode
// 25 rows
// 80 columns
static t_uint16* g_start_buffer_screen = (void*) 0xb8000;

static off_t g_offset_screen = 0;

static void clear_screen()
{
  t_uint16 clear_character;
  clear_character = (CONS_BACK(CONS_BLACK) | CONS_FRONT(CONS_WHITE)) << 8;
  clear_character |= 0;

  for (unsigned i = 0; i < 25 * 80; ++i)
    g_start_buffer_screen[i] = clear_character;

  g_offset_screen = 0;
}

int write(const char* s, size_t length)
{
  if (length > 1 && s[0] == (char) CONS_ESCAPE && s[1] == (char) CONS_CLEAR)
  {
    clear_screen();
    return 0;
  }

  for (unsigned i = 0; i < length; ++i)
  {
    if (s[i] == '\n')
    {
      while ((g_offset_screen % 80) != 0)
        g_offset_screen++;
      continue;
    }

    t_uint16 current_character;
    current_character = (CONS_BACK(CONS_BLACK) | CONS_FRONT(CONS_WHITE)) << 8;
    current_character |= s[i];

    g_start_buffer_screen[g_offset_screen++] = current_character;

    if (g_offset_screen == 25 * 80)
      g_offset_screen = 0;
  }

  return length;
}

extern unsigned char key;

int getkey(void)
{
  if (!key)
    return -1;

  int r = (key & 0x7F); // only the scan code
  key = 0;

  return r;
}

extern unsigned long tick;

unsigned long gettick(void)
{
  return tick; // returns the time in milliseconds
}

#define KFS_FD_AVAILABLE 64
extern kfs_superblock* sblock;
extern kfs_inode* fd_table[KFS_FD_AVAILABLE];

static int my_strcmp(const char* s1, const char* s2)
{

  for (; *s1 == *s2 && *s1 != '\0'; s1++, s2++)
    continue;

  return (*s1 - *s2);
}

int open(const char* pathname, int flags)
{
  if (flags != O_RDONLY)
    return -1;

  for (t_uint32 i = 0; i < sblock->inode_count; ++i)
  {
    if (pathname[0] == '/')
    {
      if (!my_strcmp(fd_table[i]->filename, &pathname[1]))
        return i;
    }
    else
    {
      if (!my_strcmp(fd_table[i]->filename, pathname))
        return i;
    }
  }

  return -1;
}

ssize_t read(int fd, void* buf, size_t count)
{
  buf=buf;

  if (fd >= KFS_FD_AVAILABLE)
    return -1;

  return count;
}

off_t seek(int filedes, off_t offset, int whence)
{
  filedes = offset + whence;
  return filedes;
}

int close(int fd)
{
  if (fd < KFS_FD_AVAILABLE)
    return 0;

  return -1;
}
