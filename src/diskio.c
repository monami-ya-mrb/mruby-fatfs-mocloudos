#include <os.h>
#include <xmalloc.h>
#include <console.h>
#include <netfront.h>
#include <lwip/api.h>
#include <mini-os/blkfront.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 

static int disk_fd[1];

void
mocloudos_initialize_block_devices(void)
{
  struct blkfront_dev *blk_dev;
  static struct blkfront_info blk_info;

  blk_dev = init_blkfront("device/vbd/769", &blk_info);
  if (blk_dev) {
    disk_fd[0] = blkfront_open(blk_dev);
  } else {
    disk_fd[0] = -1;
    printk("Block devices #0 not found.\n");
  }
}

int
mocloudos_get_disk_fd(int pdrv)
{
  int result;

  if (pdrv >= sizeof(disk_fd) / sizeof(disk_fd[0]) || disk_fd[pdrv] < 0) {
    result = -1;
  } else {
    result = disk_fd[pdrv];
  }

  return result;
}

int
mocloudos_disk_status(int pdrv, struct stat *buf)
{
  int fd;

  fd = mocloudos_get_disk_fd(pdrv);
  if (fd >= 0) {
    fd = fstat(fd, buf);
  }

  return fd;
}

static int
mocloudos_disk_rwop(int pdrv, uint8_t *buff, uint32_t sector, uint32_t count, int write)
{
  int fd;
  int ret;
  struct stat stat_buf;

  ret = mocloudos_get_disk_fd(pdrv);
  if (ret >= 0) {
    fd = ret;
    ret = fstat(fd, &stat_buf);
    if (ret >= 0) {
      off_t offset = stat_buf.st_blksize * sector;
      size_t length = stat_buf.st_blksize * count;
      ret = lseek(fd, offset, SEEK_SET);
      if (ret >= 0) {
	ret = blkfront_posix_rwop(fd, buff, length, write);
      }
    }
  }

  return ret;
}

int
mocloudos_disk_read(int pdrv, uint8_t *buff, uint32_t sector, uint32_t count)
{
  return mocloudos_disk_rwop(pdrv, buff, sector, count, 0);
}

int
mocloudos_disk_write(int pdrv, uint8_t *buff, uint32_t sector, uint32_t count)
{
  return mocloudos_disk_rwop(pdrv, buff, sector, count, 1);
}

