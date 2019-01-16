/********************************************************************************/
// fpga_spi.c
// 版本：		2.1.0.
// 文件说明：	FPGA SPI模块c文件
// 编写者：		车鹏宇
// 编写时间：	2014.09.29.
// 更新时间：	
// 更新说明：
/********************************************************************************/
#include "fpga_spi.h"

//-----------------------------------------------------------------
// SPI数据发送函数
//-----------------------------------------------------------------
void SPI_Write(unsigned int addrbase, unsigned short data)
{
	unsigned char	status = 0;
	
	status = SPI_Status_Read(addrbase);		
	while((status & MASK_SPI_IDLE) != MASK_SPI_IDLE)// SPI传输空闲中
	{
		status = SPI_Status_Read(addrbase);
	}
	
	SPI_THR_Write(data,addrbase);					// 写SPI发送寄存器
	SPI_Trans(addrbase);							// 启动1次SPI传输			
}	
