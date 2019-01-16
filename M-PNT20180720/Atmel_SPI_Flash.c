/***********************************************************************
* 文件名   ： Atmel_SPI_Flash.c
* 版权说明 ： 十三研究所捷联系统研究室
* 版本号   ： 1.0
 *  DATA:     2016-11-21
 *  OMAPL137-SPI 配置
 *  捷联系统研究室
 ** 使用注意 ： 
*    修改记录 ：
*            ////////////////////////////////////////////////////
*            ////////////////////////////////////////////////////
*            G4.1. 20171123：            
*									 修改内容：
*                  修改方法： 
*                  修改位置：
*            ////////////////////////////////////////////////////
*            ////////////////////////////////////////////////////
***********************************************************************/


#include "Atmel_SPI_Flash.h"
#include "omapl137.h"

#define KICK0_ENABLE_KEY    0x83E70B13u
#define KICK1_ENABLE_KEY    0x95a4f1e0u


CSL_SpiRegsOvly  spiRegs = (CSL_SpiRegsOvly)CSL_SPI_0_REGS;

//static unsigned int data1_reg_val, data1_reg_val_cs_disable;
static const unsigned int data1_reg_val_cs_enable =0x10000000;
static const unsigned int data1_reg_val_cs_disable=0;

void wait( Uint32 delay)
{
	volatile Uint32 i;
	for ( i = 0 ; i <delay ; i++ ){asm(" NOP");
	};
}

//transimit and receive a byte via SPIDAT0
static void spi_xfer_simple(const void *dout, void *din)
{
	Uint8 txByte = 0;

	/* do an empty read to clear the current contents */
	//spiRegs->SPIBUF;

	/* wait till transmit buffer is empty */
	while (spiRegs->SPIBUF & 0x20000000);
	/* set the write data if it's valid */
	if (dout) {
		txByte = *(Uint8*)dout & ((1 << SPI_NUM_OF_TXBITS) - 1);
	}
	/* write to DAT1 is required to keep the serial transfer going */
	/* we just terminate when we reach the end */
	spiRegs->SPIDAT0 = txByte;

	/* read the data - wait for data availability */
	while(spiRegs->SPIBUF & 0x80000000);
	if (din) {
		*(Uint8*)din = spiRegs->SPIBUF & ((1 << SPI_NUM_OF_TXBITS) - 1);			
	}
	else {
		spiRegs->SPIBUF;
	}

}
//transmit a byte and assert the CS pin, this function is a superset of "spi_xfer_simple"
static void spi_xfer_cs(const void *dout, void *din, Uint32 cs_flag)
{
	Uint8 txByte = 0;

	/* do an empty read to clear the current contents */
	//spiRegs->SPIBUF;

	/* wait till transmit buffer is empty */
	while (spiRegs->SPIBUF & 0x20000000);
	/* set the write data if it's valid */
	if (dout) {
		txByte = *(Uint8*)dout & ((1 << SPI_NUM_OF_TXBITS) - 1);//cut off unneccesary bits
	}
	/* write to DAT1 is required to keep the serial transfer going */
	/* we just terminate when we reach the end */
	spiRegs->SPIDAT1 = cs_flag | txByte;

	/* read the data - wait for data availability */
	while(spiRegs->SPIBUF & 0x80000000);
	if (din) {
		*(Uint8*)din = spiRegs->SPIBUF & ((1 << SPI_NUM_OF_TXBITS) - 1);			
	}
	else {
		spiRegs->SPIBUF;
	}
}
//transmit a byte and assert the CS pin
static int spi_xfer_cs_on(Uint16 byte_len, const void *dout, void *din)
{
	int		i;
	const Uint8	*txp = dout;
	Uint8		*rxp = din;

	/* do an empty read to clear the current contents */
	spiRegs->SPIBUF;
	/* keep writing and reading 1 byte until done */
	spi_xfer_cs(txp, rxp, data1_reg_val_cs_enable);

	for (i = 1; i < byte_len; i++) {

		if (dout)	++txp;
		if (din)	++rxp;

		spi_xfer_simple(txp, rxp);
	}

	return 0;

}

static int spi_xfer_cs_off(Uint16 byte_len, const void *dout, void *din)
{
	int		i;
	const Uint8	*txp = dout;
	Uint8		*rxp = din;

	/* do an empty read to clear the current contents */
	spiRegs->SPIBUF;

	/* keep writing and reading 1 byte until done */
	for (i = 0; i < byte_len - 1; i++) {

		spi_xfer_simple(txp, rxp);		
		if (dout)	++txp;
		if (din)	++rxp;
	}	
	spi_xfer_cs(txp, rxp, data1_reg_val_cs_disable);
	return 0;

}



//初始化
void FLASHSpiInit()
{
	/* First reset the SPI chip                                               */
	spiRegs->SPIGCR0 &= 0xfffffffe;

	wait(100);

	/* now bring the chip out of reset state                                  */
	spiRegs->SPIGCR0 |= 0x00000001;

	/* enable the CLKMOD and MASTER bits in the SPI global control reg        */
	spiRegs->SPIGCR1 |=  0x00000003;//bit24 =1, bit[1:0]= 3;

	/* enable the pins so that they are used for the SPI interface(Multiplex) */
	spiRegs->SPIPC0  = 0
		| ( 1 << 11 )   // DI
		| ( 1 << 10 )   // DO
		| ( 1 << 9 )    // CLK
		| ( 0 << 1 )    // EN1
		| ( 1 << 0 );   // cs


	/* configure the data format in SPIFMT                                    */
	spiRegs->SPIFMT[0] = 0
		| ( 0 << 20 )   // SHIFTDIR MSB FIRST
		| ( 1 << 18 )
		| ( 0 << 17 )   // Polarity  SPIclocksignalis low-inactive (before andafterdatatransfer the clocksignalis low).
		| ( 1 << 16 )   // Phase   SPIclocksignalis notdelayedversusthe transmit/receive datastream
		| ( 4 << 8 )    // Prescale to 150MHz,change to SYSCLK2/15.
		| ( 8 << 0 );   // Char Len


	/* set the preconfigure data format as 0 which is already set above       */
	spiRegs->SPIDAT1 = 0<<24;//CSL_FMKT(SPI_SPIDAT1_DFSEL,FORMAT0);

	/* dont use any interrupts hence disable them                             */
	spiRegs->SPIINT0 = 0
		| ( 1 << 24 )//high-impedance
		| ( 0 << 16 )   //
		| ( 0 << 8 )    //
		| ( 0 << 6 )    //
		| ( 0 << 4 );   //

	spiRegs->SPIGCR1  |= ( 1 << 24 );/* Enable SPI */
}

//--------------------------------------
//功能：发出“单字节”命令，读取flash返回值。，
//参数：input：cmd 命令内容（不是指针
//				ret_byte_len 返回数据的长度
//		output:	response返回数据存放指针
//返回值：int 命令执行的状态，错误为-1，正确为0
//---------------------------------------------
static int spi_flash_cmd(const Uint8 cmd, void *response, Uint16 ret_byte_len)
{
	int ret;
	if (ret_byte_len == 0){
		ret = spi_xfer_cs_off(1, &cmd, NULL);
		return 0;
	}
	ret = spi_xfer_cs_on(1, &cmd, NULL);
	if (ret_byte_len) 
		ret = spi_xfer_cs_off(ret_byte_len, NULL, response);
	return ret;
}
//--------------------------------------
//功能：发出命令，读取flash返回值。没有返回值时不应使用此函数！！！
//参数：input：cmd 命令指针
//				cmd_len 命令长度
//				datalen 返回数据的长度
//		output:	data返回数据存放指针
//返回值：int 命令执行的状态，错误为-1，正确为0
//---------------------------------------------
/*static int spi_flash_cmd_read(const Uint8 *cmd, Uint16 cmd_len, void *data, Uint32 data_len)
{
	int ret;
	if (data_len == 0)
		return 0;
	ret = spi_xfer_cs_on(cmd_len, cmd, NULL);
	if (data_len != 0) 
		ret = spi_xfer_cs_off(data_len, NULL, data);	
	return ret;
}*/
//--------------------------------------
//功能：发出命令，不读取任何返回值。命令后没有参数的，不应使用本函数！！！！
//参数：input：cmd 命令指针
//				cmd_len 命令长度
//				datalen 返回数据的长度
//		output:	data返回数据存放指针
//返回值：int 命令执行的状态，错误为-1，正确为0
//---------------------------------------------
static int spi_flash_cmd_write(const Uint8 *cmd, Uint16 cmd_len, const void *data, Uint32 data_len)
{
	int ret;
	ret = spi_xfer_cs_on(cmd_len, cmd, NULL);
	if (data_len) 
		ret = spi_xfer_cs_off(data_len, data, NULL);
	return ret;
}


static int Atmel_wait_ready(Uint32 timeout)
{
	int ret;
	Uint8 status;
	do {
		ret = spi_flash_cmd(CMD_ATM_RDSR, &status, sizeof(status));
		if (ret)
			return -1;
		if ((status & ATMEL_SR_RDY_MASK) == ATMEL_SR_RDY_OK)
			break;
		timeout--;
		if (!timeout)
			break;

	} while (1);

	if ((status & ATMEL_SR_RDY_MASK) == ATMEL_SR_RDY_OK)
		return 0;
	/* Timed out */
	return -1;
}

int Atmel_Write_Wnable(void)
{
	int ret;
	ret = spi_flash_cmd(CMD_ATM_WREN, NULL, 0);
	return ret;
}
// 
/*int Atmel_Write_Disable(void)
{
	int ret;
	ret = spi_flash_cmd(CMD_ATM_WRDI, NULL, 0);
	return ret;

}*/

/*int Atmel_GlobalProtect(void)
{
	Uint8 cmd[2] = {0};
	Atmel_Write_Wnable();
	cmd[0] = CMD_ATM_WRSR1;//0Bh opcode
	cmd[0] = 0xFF;//0Bh opcode
	spi_xfer_cs_on(1, cmd, NULL);
	spi_xfer_cs_off(1, cmd+1, NULL);
	return 0;
}*/

int Atmel_GlobalUnprotect(void)
{
	Uint8 cmd[2] = {0};
	Atmel_Write_Wnable();
	cmd[0] = CMD_ATM_WRSR1;//
	cmd[1] = 0x0;//
	spi_xfer_cs_on(1, cmd, NULL);
	spi_xfer_cs_off(1, cmd+1, NULL);
	return 0;
}

//send a CMD_READ_ID cmd, and the Flash will return 4byte information about Manufacturer ID and Device ID
//This can be uesed to test the hardware and SPI setting
/*int Atmel_ReadID(void)
{
	Uint8 cmd[5] = {0};

	cmd[0] = CMD_READ_ID;//0Bh opcode
	spi_flash_cmd_read(cmd,1,cmd+1,4);
	return (cmd[1]<<24)|((cmd[2]<<16))|((cmd[3]<<8))|((cmd[4]<<0));
}
*/
//continueous array read
// addr:  specify the starting address location of the first
//byte to read within the memory array
//Note:   If the 1Bh opcode is used, then two dummy bytes must be clocked into the
//device after the three address bytes. If the 0Bh opcode is used, then a single dummy byte must
//be clocked in after the address bytes.

static int Atmel_CAR(const Uint32 addr)
{
	Uint8 cmd[5];
	int i, j;

	cmd[0] = CMD_READ_ARRAY_FAST;//0Bh opcode
	for (i = 0, j = 1; i < 24; i += 8)
		cmd[j++] = ((addr >> (16 - i)) & 0xFF);
	cmd[4] = 0x00;
	return spi_xfer_cs_on(sizeof(cmd), cmd, NULL);
}


static int Atmel_ArrayRead(Uint16 Uint8_len, Uint8 *buf)
{
	int i;	
	for (i = 0; i < Uint8_len; i++)		
		spi_xfer_simple(NULL, buf + i);
	return 0;
}


int Atmel_Write(Uint32 offset, const Uint32 len, Uint8 *buf)
{
	unsigned long page_addr;
	unsigned long byte_addr;
	unsigned long page_size;
	Uint32 chunk_len;
	Uint32 actual;
	Int32	 ret, i, j;
	Uint8 cmd[4];

	page_size = FLASH_PAGE_SIZE;
	page_addr = offset / page_size;
	byte_addr = offset % page_size;

	ret = 0;
	for (actual = 0; actual < len; actual += chunk_len) {
		   chunk_len = ((len - actual) < (page_size - byte_addr) ? 
		   				(len - actual) : (page_size - byte_addr));

		cmd[0] = CMD_ATM_PP;
		for (i = 0, j = 1; i < 24; i += 8)
		    cmd[j++] = ((offset >> (16 - i)) & 0xFF);

		Atmel_Write_Wnable();

		ret = spi_flash_cmd_write(cmd, 4, buf + actual, chunk_len);
//		if (ret < 0)       //== G4.1_20171123modify 
//		    {
//		//	printf("SF: Winbond Page Program failed\n");
//            j=0;
//			break;
//		}

		ret = Atmel_wait_ready(SPI_FLASH_PROG_TIMEOUT);
		if (ret < 0) {
		//	printf("SF: Winbond page programming timed out\n");
		   j=0;
//			break;
		}

		page_addr++;
		offset += chunk_len;
		byte_addr = 0;
	}

	//printf(" Successfully programmed %u bytes @ 0x%x\n", len, offset);
	return ret;
}

// addr:  specify the starting address location of the first
//byte to read within the memory array
Uint8 * Atmel_Read(const Uint32 addr, const Uint16 len, Uint8 *buf)
{
	static Uint8  retval[4];  
	//spi_claim_bus();
	Atmel_CAR(addr);
	Atmel_ArrayRead(len,buf);
	if(len%2)//for what?
		Atmel_ArrayRead(3,retval);
	else
		Atmel_ArrayRead(2,retval);
	spi_xfer_cs_off(1, NULL, NULL);	
	//spi_release_bus();
	return retval;
}



/*int Atmel_ReadStatusReg(void)
{
	Uint8 cmd[5] = {0};

	cmd[0] = CMD_ATM_RDSR;//0Bh opcode
	spi_flash_cmd_read(cmd,1,cmd+1,4);
	return (cmd[1]<<24)|((cmd[2]<<16))|((cmd[3]<<8))|((cmd[4]<<0));
}*/

//FLASH擦除,AT25DF16擦出地址为3byte，并且根据擦出的大小，低地址位被flash忽略
//这里的page_addr指的是擦擦除块的首字节地址。
int Atmel_Erase_Once(const Uint32 page_addr,Uint8 cmd0)
{
	int   ret, i , j;
	Uint8 cmd[4]={0XC7,0X94,0X80,0X9A};
	Atmel_Write_Wnable();
	cmd[0] = cmd0;
	//spi_claim_bus();
	ret = 0;
	if(cmd0!=CMD_ATM_CE)
	{
		for (i = 0, j = 1; i < 24; i += 8)
			cmd[j++] = (((page_addr) >> (16 - i)) & 0xFF);
	}
	ret = spi_flash_cmd_write(cmd, 4, NULL, 0);
	//spi_xfer_cs_off(sizeof(cmd),cmd, NULL);
	/* Up to 2 seconds */
	ret = Atmel_wait_ready(SPI_FLASH_PAGE_ERASE_TIMEOUT);
	//spi_release_bus();
	return ret;
}


int Atmel_Muti4K_Erase(const Uint32 page_addr,Uint32 pagenum)
{
	int   ret=-1, i;    //== G4.1_20171123modify
	int page_add_internal = page_addr;
	/*
	 * This function currently uses 4k sector erase only.
	 * probably speed things up by using bulk erase
	 * when possible.
	 */
	for (i = 0; i < pagenum; i++)
	{
		ret = Atmel_Erase_Once(page_add_internal,CMD_ATM_4KE);
		page_add_internal += 4096;
//			wait( 1000);

		if(ret == -1)
			return ret;
	}
	return ret;
}

