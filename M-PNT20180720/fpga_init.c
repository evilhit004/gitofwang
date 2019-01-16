/********************************************************************************/
// fpga_init.c
// 版本：		2.1.0.
// 文件说明：	FPGA初始化文件，该文件可由硬件工程师根据硬件系统进行修改
// 编写者：		车鹏宇
// 编写时间：	2014.09.09.
// 更新时间：	2014.09.29.
// 更新说明：
//	2014.09.29.
//		1.更新了AD7734_init函数的SPI操作
/********************************************************************************/
#include "fpga_init.h"

//-----------------------------------------------------------------
// AD7734初始化函数
//-----------------------------------------------------------------
//void AD7734_init(void)
//{
//	// 初始化AD7734 SPI接口
//	SPI_Mode_Set(0x6, AD7734_Base);			// SPI配置为第1个跳变沿发送数据，第2个跳变沿采集数据，空闲状态为‘1’，MSB在前，LSB在后
//	SPI_SCK_Set(0xc, AD7734_Base);			// SCK设置18.432MHz/3 = 6.114MHz
//	SPI_SS_Set(0x2, AD7734_Base);			// SCK第1个跳变沿到CSn的建立时间 20ns x 2 = 40ns	
//	
//	// 初始化AD7734 SPI接口
//	SPI_Mode_Set(0x6, AD7734_Base2);		// SPI配置为第1个跳变沿发送数据，第2个跳变沿采集数据，空闲状态为‘1’，MSB在前，LSB在后
//	SPI_SCK_Set(0xc, AD7734_Base2);			// SCK设置18.432MHz/3 = 6.114MHz
//	SPI_SS_Set(0x2, AD7734_Base2);			// SCK第1个跳变沿到CSn的建立时间 20ns x 2 = 40ns		
//	
//	// 复位AD7734	
////	AD7734_WRITE_CTR(AD7734_Base,AD7734_RST_CMD);	// 复位后需要延迟等待2us 复位低电平持续时间设置为1us
////
//
//// 1			
//	// 设置AD7734 工作模式
//	AD7734_WRITE_SET(AD7734_Base2,0x0);		// 设置SPI传输为非DMA方式
//	
//	SPI_Write(AD7734_Base2,0x00);			// 软件复位序列
//	SPI_Write(AD7734_Base2,0xFF);
//	SPI_Write(AD7734_Base2,0xFF);
//	SPI_Write(AD7734_Base2,0xFF);
//	SPI_Write(AD7734_Base2,0xFF);
//
//	_wait(185180);		// 1个 = 0.054us
//
//	// CH0
//	SPI_Write(AD7734_Base2,0x28);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base2,0x08);		//enable the channel AIN0 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);	
//
//	SPI_Write(AD7734_Base2,0x30);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base2,0x86);		//chop enabled, and the update rate is 2534Hz(395us@6.144MHz)
//	_wait(250);
//	                 
//	// CH1
//	SPI_Write(AD7734_Base2,0x29);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base2,0x08);		//enable the channel AIN1 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//
//	SPI_Write(AD7734_Base2,0x31);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base2,0x86);		//chop enabled, and the update rate is 2534Hz
//	_wait(250);
//	                
//	// CH2
//	SPI_Write(AD7734_Base2,0x2A);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base2,0x08);		//enable the channel AIN2 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//
//	SPI_Write(AD7734_Base2,0x32);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base2,0x86);		//chop enabled, and the update rate is 2534Hz
//	_wait(250);
//	                 
//	// CH3
//	SPI_Write(AD7734_Base2,0x2B);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base2,0x08);		//enable the channel AIN3 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//
//	SPI_Write(AD7734_Base2,0x33);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base2,0x86);		//chop enabled, and the update rate is 2534Hz
//	_wait(250);
//	                 
//	//I/O Port 
//	SPI_Write(AD7734_Base2,0x01);		//Write I/O Port Reg
//	SPI_Write(AD7734_Base2,0x19);		//P0 as an output,P1/SYNCn configured as SYNCn,RDYn pin go low all enabled channels have unread data 
//	_wait(250);
//
///*		
//	// ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base2,0x38);		//Write CH0 Mode Reg
//	SPI_Write(AD7734_Base2,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base2,0x39);		//Write CH1 Mode Reg
//	SPI_Write(AD7734_Base2,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base2,0x3A);		//Write CH2 Mode Reg
//	SPI_Write(AD7734_Base2,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base2,0x3B);		//Write CH3 Mode Reg
//	SPI_Write(AD7734_Base2,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//*/		
//	SPI_Write(AD7734_Base2,0x38);
//	SPI_Write(AD7734_Base2,0x32);
//	_wait(250);
//	
//// 0				
//	
//	// 设置AD7734 工作模式
//	AD7734_WRITE_SET(AD7734_Base,0x0);		// 设置SPI传输为非DMA方式
//	
//	SPI_Write(AD7734_Base,0x00);			// 软件复位序列
//	SPI_Write(AD7734_Base,0xFF);
//	SPI_Write(AD7734_Base,0xFF);
//	SPI_Write(AD7734_Base,0xFF);
//	SPI_Write(AD7734_Base,0xFF);
//	
//	_wait(185180);		
//	
//	// CH0
//	SPI_Write(AD7734_Base,0x28);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base,0x08);		//enable the channel AIN0 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//
//	SPI_Write(AD7734_Base,0x30);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base,0x86);		//chop enabled, and the update rate is 2534Hz(395us@6.144MHz)
//	_wait(250);
//	                 
//	// CH1
//	SPI_Write(AD7734_Base,0x29);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base,0x08);		//enable the channel AIN1 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//	
//	SPI_Write(AD7734_Base,0x31);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base,0x86);		//chop enabled, and the update rate is 2534Hz
//	_wait(250);
//	                
//	// CH2
//	SPI_Write(AD7734_Base,0x2A);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base,0x08);		//enable the channel AIN2 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//
//	SPI_Write(AD7734_Base,0x32);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base,0x86);		//chop enabled, and the update rate is 2534Hz
//	_wait(250);
//	                 
//	// CH3
//	SPI_Write(AD7734_Base,0x2B);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base,0x08);		//enable the channel AIN3 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//
//	SPI_Write(AD7734_Base,0x33);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base,0x86);		//chop enabled, and the update rate is 2534Hz
//	_wait(250);
//	                 
//	//I/O Port 
//	SPI_Write(AD7734_Base,0x01);		//Write I/O Port Reg
//	SPI_Write(AD7734_Base,0x19);		//P0 as an output,P1/SYNCn configured as SYNCn,RDYn pin go low all enabled channels have unread data 
//	_wait(250);
///*		
//	// ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base,0x38);		//Write CH0 Mode Reg
//	SPI_Write(AD7734_Base,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base,0x39);		//Write CH1 Mode Reg
//	SPI_Write(AD7734_Base,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base,0x3A);		//Write CH2 Mode Reg
//	SPI_Write(AD7734_Base,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base,0x3B);		//Write CH3 Mode Reg
//	SPI_Write(AD7734_Base,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//
//	_wait(250);
//*/
//	
//	SPI_Write(AD7734_Base,0x38);
//	SPI_Write(AD7734_Base,0x32);	
//	_wait(2500);
//	
//		
//
//	// 发送SYNCn信号
//	OSP_Write_Width(OSP0_Base,37);		// SYNCn信号宽度2us
//	OSP_Trigger(OSP0_Base);	
//	
//	_wait(250);					
//	
//	
//	// 启动转换
//	AD7734_WRITE_SET(AD7734_Base,0x1);		// 设置SPI传输为DMA方式	
//	AD7734_WRITE_SET(AD7734_Base2,0x1);		// 设置SPI传输为DMA方式		
//	
//}



