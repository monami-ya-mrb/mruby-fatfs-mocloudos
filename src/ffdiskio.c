#ifdef __MINIOS__
/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include <stdint.h>
#include <sys/stat.h>
#include "diskio.h"		/* FatFs lower layer API */
#include "mruby.h"

extern int mocloudos_disk_status(int blk_id, struct stat *buf); 
extern int mocloudos_disk_read(int blk_id, uint8_t *buff, uint32_t sector, uint32_t count); 
extern int mocloudos_disk_write(int blk_id, uint8_t *buff, uint32_t sector, uint32_t count); 

extern void printk(const char* fmt, ...);
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

/* blk_id : Physical drive nmuber (0..) */
DSTATUS disk_initialize (BYTE blk_id)
{
	return disk_status(blk_id);
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

/* blk_id : Physical drive nmuber (0..) */
DSTATUS disk_status (BYTE blk_id)
{
	struct stat buf;
	DSTATUS status;

	mocloudos_disk_status((int)blk_id, &buf);
	if ((buf.st_mode & 0222) == 0) {
		status = STA_PROTECT;
	} else {
		status = 0;
	}
	/* TODO: more checks */

	return status;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

/* blk_id   : Physical drive nmuber (0..) */
/* buff     : Data buffer to store read data */
/* sector   : Sector address (LBA) */
/* count    : Number of sectors to read (1..128) */
DRESULT disk_read (BYTE blk_id, BYTE *buff, DWORD sector,  UINT count)
{
	DRESULT res;

	if (mocloudos_disk_read((int) blk_id, (uint8_t *)buff, (uint32_t) sector, (uint32_t) count) >= 0) {
		res = 0;
	} else {
		res = RES_PARERR;
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
/* blk_id   : Physical drive nmuber (0..) */
/* buff     : Data to be written */
/* sector   : Sector address (LBA) */
/* count    : Number of sectors to write (1..128) */
DRESULT disk_write (BYTE blk_id, const BYTE *buff, DWORD sector, UINT count)
{
	DRESULT res;

	if (mocloudos_disk_write((int) blk_id, (uint8_t *)buff, (uint32_t) sector, (uint32_t) count) >= 0) {
		res = 0;
	} else {
		res = RES_PARERR;
	}

printk("disk_write %d\n", res);
	return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE blk_id,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	struct stat buf;
	int disk_fd = mocloudos_get_disk_fd(blk_id);

	if (cmd == CTRL_SYNC) {
	  /* do nothing */
	  res = RES_OK;
	} else if (cmd == GET_SECTOR_COUNT) {
	  (void) fstat(disk_fd, &buf);
	  *((DWORD*)buff) = buf.st_size / buf.st_blksize;
	  res = RES_OK;
	} else if (cmd == GET_SECTOR_SIZE) {
	  (void) fstat(disk_fd, &buf);
	  *((WORD*)buff) = buf.st_blksize;
	  res = RES_OK;
	} else if (cmd == GET_BLOCK_SIZE) {
	  *((DWORD*)buff) = 1;
	  res = RES_OK;
	} else if (cmd == CTRL_ERASE_SECTOR) {
	  /* do nothing */
	  res = RES_OK;
	} else {
	  res = RES_PARERR;
	}

	return res;
}
#endif


#endif /* __MINIOS__ */
