/*
 * Copyright (C) 2014 Monami-ya LLC, Japan.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <inttypes.h>

#include "fatfs-mocloudos.h"

int
mocloudos_disk_status(int blk_id, struct stat *buf)
{
  int fd;

  fd = mocloudos_get_disk_fd(blk_id);
  if (fd >= 0) {
    fd = fstat(fd, buf);
  }

  return fd;
}

static int
mocloudos_disk_rwop(int blk_id, uint8_t *buff, uint32_t sector, uint32_t count, int write)
{
  int fd;
  int ret;
  struct stat stat_buf;

  ret = mocloudos_get_disk_fd(blk_id);
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
mocloudos_disk_read(int blk_id, uint8_t *buff, uint32_t sector, uint32_t count)
{
  return mocloudos_disk_rwop(blk_id, buff, sector, count, 0);
}

int
mocloudos_disk_write(int blk_id, uint8_t *buff, uint32_t sector, uint32_t count)
{
  return mocloudos_disk_rwop(blk_id, buff, sector, count, 1);
}

