/********************************************************************************/
// fpga_ad7894.c
// 版本：		2.1.0.
// 文件说明：	FPGA AD7894模块C文件
// 编写者：		ChePengyu
// 编写时间：	2014.09.20.
// 更新时间：	
// 更新说明：
/********************************************************************************/
#include "fpga_ad7894.h"
#include "fpga_spi.h"

//-----------------------------------------------------------------
// AD7894初始化函数
//-----------------------------------------------------------------
void AD7894_init(unsigned int addrbase, unsigned char Con_Width, unsigned char sck_div, unsigned char spi_mode)				
{
	AD7894_WRITE_SET(Con_Width,addrbase);	//设置AD转换启动信号高电平有效宽度 CONVST = ConWidth*Tclk
	SPI_SCK_Set(sck_div,addrbase);			//设置AD7894 SPI接口的串行移位时钟频率 fSCK = Tclk/sck_div
	SPI_Mode_Set(spi_mode,addrbase);		//SCK上升沿采样，MSB在前，SCK空闲为低电平(mode0)
}
//-----------------------------------------------------------------
// 读AD7894数据
//-----------------------------------------------------------------
void AD7894_Read(float * ptrAD_result, unsigned int addrbase)
{
	unsigned short AD_Data = 0; 
	unsigned char status = 0;

	status = AD7894_READ_STATUS(addrbase);			//查询AD转换完毕标志
	while((status | 0xfe) != 0xfe)				
	{
		status = AD7894_READ_STATUS(addrbase);
	}	
	
	SPI_Trans(addrbase);

	status = SPI_Status_Read(addrbase);
	
	while((status & 0x40) != 0x40)					
	{
		status = SPI_Status_Read(addrbase);
	}
	
 	AD_Data = SPI_RBR_Read(addrbase) & 0x3fff;		//读取AD数据
	*(ptrAD_result) = AD_Data * AD7894_LSB;
}


