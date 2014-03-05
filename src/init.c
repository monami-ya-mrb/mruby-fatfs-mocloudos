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
#include <mini-os/blkfront.h>
#include "mruby.h"

static int disk_fd[10];

static void
mocloudos_initialize_block_devices(void)
{
  struct blkfront_dev *blk_dev;
  static struct blkfront_info blk_info;

  blk_dev = init_blkfront("device/vbd/769", &blk_info);
  if (blk_dev) {
    disk_fd[0] = blkfront_open(blk_dev);
  } else {
    disk_fd[0] = -1;
    printk("ERROR: Block devices #0 not found.\n");
  }
}

int
mocloudos_get_disk_fd(int blk_id)
{
  int result;

  if (blk_id < 0 ||
      blk_id >= sizeof(disk_fd) / sizeof(disk_fd[0]) ||
      disk_fd[blk_id] < 0) {
    result = -1;
  } else {
    result = disk_fd[blk_id];
  }

  return result;
}

void
mrb_mruby_fatfs_mocloudos_gem_init(mrb_state *mrb)
{
  mocloudos_initialize_block_devices();
}


void
mrb_mruby_fatfs_mocloudos_gem_final(mrb_state *mrb)
{
}
