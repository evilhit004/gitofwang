/********************************************************************************/
// fpga_motor.c
// 版本：		2.1.0.
// 文件说明：	FPGA步进电机模块c文件
// 编写者：		车鹏宇
// 编写时间：	2014.12.25.
// 更新时间：	
// 更新说明：
/********************************************************************************/
#include "fpga_motor.h"

/*-------------------------------------------------------------------
-- 马达控制函数
-- 串口FIFO清零
-- AddrBase:		基地址
-- Pulse_Freq:		脉冲频率分频系数
-- Pulse_Num:		脉冲个数
-- Pulse_Dir:		方向
-- 返回值：			无
-------------------------------------------------------------------*/
void Motor_Ctrl(unsigned int AddrBase, unsigned int Pulse_Freq, unsigned short Pulse_Num, unsigned char Pulse_Dir)
{
	unsigned short Pulse_FreqL = 0;
	unsigned short Pulse_FreqH = 0;
	
	Pulse_FreqL = Pulse_Freq & 0xFFFF;
	Pulse_FreqH = (Pulse_Freq >> 16) & 0xFFFF;
	
	MOTOR_WRITE_FDIVL(AddrBase,Pulse_FreqL);		// 写马达脉冲分频系数低16位 
	MOTOR_WRITE_FDIVH(AddrBase,Pulse_FreqH);		// 写马达脉冲分频系数高16位 
	MOTOR_WRITE_PNUM(AddrBase,Pulse_Num);			// 写马达脉冲个数               
	MOTOR_WRITE_DIR(AddrBase,Pulse_Dir);			// 写马达脉冲方向               								
}



