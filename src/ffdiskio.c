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

extern int mocloudos_get_disk_fd(int pdrv);
extern int mocloudos_disk_status(int pdrv, struct stat *buf); 
extern int mocloudos_disk_read(int pdrv, uint8_t *buff, uint32_t sector, uint32_t count); 
extern int mocloudos_disk_write(int pdrv, uint8_t *buff, uint32_t sector, uint32_t count); 

extern void printk(const char* fmt, ...);
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	return disk_status(pdrv);
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	struct stat buf;
	DSTATUS status;

	mocloudos_disk_status((int)pdrv, &buf);
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

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;

	if (mocloudos_disk_read((int) pdrv, (uint8_t *)buff, (uint32_t) sector, (uint32_t) count) >= 0) {
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
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res;

	if (mocloudos_disk_write((int) pdrv, (uint8_t *)buff, (uint32_t) sector, (uint32_t) count) >= 0) {
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
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	struct stat buf;
	int fd = mocloudos_get_disk_fd(pdrv);

	if (cmd == CTRL_SYNC) {
	  /* do nothing */
	  res = RES_OK;
	} else if (cmd == GET_SECTOR_COUNT) {
	  (void) fstat(fd, &buf);
	  *((DWORD*)buff) = buf.st_size / buf.st_blksize;
	  res = RES_OK;
	} else if (cmd == GET_SECTOR_SIZE) {
	  (void) fstat(fd, &buf);
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
