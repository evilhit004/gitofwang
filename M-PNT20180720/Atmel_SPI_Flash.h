/***********************************************************************/
/*
 *  DATA:  2016-11-21
 *  OMAPL137-SPI 配置
 *  捷联系统研究室
 */
/***********************************************************************/


#ifndef _ATMEL_SPI_FLASH_H_
#define _ATMEL_SPI_FLASH_H_

#include <stddef.h>
#include "omapl137.h"

#define SPI_CLOCK_FREQUENCY	150000000
#define SPI_NUM_OF_TXBITS   8

#define FLASH_PAGE_SIZE       256        // page_size,写入时使用
#define FLASH_SECTOR_SIZE     4*1024     // page_size,写入时使用
////// nr_pages
//#define FLASH_PAGE_NUM         0x1000    
//#define FLASH_PAGE_WIDTH       9    

/* Common parameters */
#define	CFG_HZ							100
#define SPI_FLASH_PROG_TIMEOUT			((100000 * CFG_HZ) / 1000)
#define SPI_FLASH_PAGE_ERASE_TIMEOUT	((500000 * CFG_HZ) / 1000)
//#define SPI_FLASH_SECTOR_ERASE_TIMEOUT	(100000 * CFG_HZ)


/* ATMEL-specific commands */
#define CMD_ATM_WREN		0x06	/* Write Enable */
//#define CMD_ATM_WRDI		0x04	/* Write Disable */
#define CMD_ATM_RDSR		0x05	/* Read Status Register */
#define CMD_ATM_WRSR1		0x01	/* Write Status Register1 */
//#define CMD_ATM_WRSR2		0x31	/* Write Status Register2 */

////#define CMD_ATM_READ		0x03	/* Read Data Bytes */
////#define CMD_ATM_FAST_READ	0x0b	/* Read Data Bytes at Higher Speed */

#define CMD_ATM_PP		0x02	/* Page Program, Main Memory Page Program Through Buffer 1 */
//#define CMD_ATM_DP		0xB9	/* Deep Power-down */

////#define CMD_ATM_PE		0x81	/* Page Erase */
////#define CMD_ATM_BE		0x50	/* Block Erase */
////#define CMD_ATM_SE		0x7C	/* Sector Erase */
//#define CMD_ATM_64KE		0xD8	/*  64-Kbyte  Erase */
//#define CMD_ATM_32KE		0x52	/*  32-Kbyte  Erase */
#define CMD_ATM_4KE			0x20	/*  4-Kbyte  Erase */

#define CMD_ATM_CE		0xC7	/* Chip Erase */

/* read commands */
//#define CMD_READ_ID				0x9f

//#define CMD_READ_ARRAY_SLOW		0x03		// Continuous Array Read (Low Frequency),Up to 50 MHz
#define CMD_READ_ARRAY_FAST		0x0b		// Continuous Array Read,Up to 85 MHz
//#define CMD_READ_ARRAY_MOST_FAST		0x1B		// Continuous Array Read,Up to 100 MHz


//#define ATMEL_SR_WIP		(1 << 0)	/* Write-in-Progress */
#define ATMEL_SR_RDY_MASK	0x1//0x81	// device ready/busy and PAGE SIZE
#define ATMEL_SR_RDY_OK		0x0//0x80	// device ready and PAGE SIZE is 1056 Bytes
//#define ATMEL_PROTECT		0x36
//#define ATMEL_UNPROTECT		0x39


#define CSL_SPI_0_REGS                (0x01C41000u)
#define CSL_SPI_1_REGS                (0x01E12000u)

#define GYROX		0x2
#define GYROY		0x1
#define GYROZ		0x0

#define ACCX		0x6
#define ACCY		0x5
#define ACCZ		0x4

#define LSB_GYROX	    0.0000625
#define LSB_GYROYZ		0.0000625

typedef struct{
    volatile Uint32 SPIGCR0;
    volatile Uint32 SPIGCR1;
    volatile Uint32 SPIINT0;
    volatile Uint32 SPILVL;
    volatile Uint32 SPIFLG;
    volatile Uint32 SPIPC0;
    volatile Uint32 SPIPC1;
    volatile Uint32 SPIPC2;
    volatile Uint32 SPIPC3;
    volatile Uint32 SPIPC4;
    volatile Uint32 SPIPC5;
    volatile Uint8  RSVD0[12];
    volatile Uint32 SPIDAT0;
    volatile Uint32 SPIDAT1;
    volatile Uint32 SPIBUF;
    volatile Uint32 SPIEMU;
    volatile Uint32 SPIDELAY;
    volatile Uint32 SPIDEF;
    volatile Uint32 SPIFMT[4];
    volatile Uint32 INTVEC[2];
} CSL_SpiRegs;

typedef volatile CSL_SpiRegs         *CSL_SpiRegsOvly;


//int Atmel_GlobalProtect(void);
int Atmel_GlobalUnprotect(void);
//send a CMD_READ_ID cmd, and the Flash will return 4byte information about Manufacturer ID and Device ID
//This can be uesed to test the hardware and SPI setting


//int Atmel_ReadID(void);
int Atmel_Write(Uint32 offset, const Uint32 len, Uint8 *buf);
// addr:  specify the starting address location of the first
//byte to read within the memory array
Uint8 * Atmel_Read(const Uint32 addr, const Uint16 len, Uint8 *buf);
//int Atmel_ReadStatusReg(void);
//FLASH擦除,AT25DF16擦出地址为3byte，并且根据擦出的大小，低地址位被flash忽略
//这里的page_addr指的是擦擦除块的首字节地址。
int Atmel_Erase_Once(const Uint32 page_addr,Uint8 cmd0);
int Atmel_Muti4K_Erase(const Uint32 page_addr,Uint32 pagenum);
void FLASHSpiInit(void);

static int spi_flash_cmd_write(const Uint8 *cmd, Uint16 cmd_len, const void *data, Uint32 data_len);

void wait( Uint32 delay);
void spi1_xfer_simple(const void *dout, void *din);
void read_MGZ310_Gyro(unsigned int gyro_swich, float * gyro_output, float * gyrotemp_output);
void read_MSA8000D_Acc(unsigned int acc_swich, float * acc_output, float * acctemp_output);
void SpiInit(void);

#endif
