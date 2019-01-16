/*
 *  Copyright 2008 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  GPIO header file
 *
 *	更新时间：	2014.05.20.
 *	修改人：	chepengyu
 *	更新说明：
 *		1.由文件evmomapl137_gpio.h修改而来，将evmomapl137.h中关于GPIO寄存器定义部分加入该文件中
 *		2.修改了GPIO引脚定义名称
 *		3.evmomapl137_gpio.c文件可以直接包含该文件
 *		4.增加了GPIO中断设置的函数原型定义
 */

#ifndef GPIO_H_
#define GPIO_H_


// 寄存器定义

#define GPIO_BASE               0x01E26000

#define GPIO_BINTEN             0x08		// GPIO Interrupt Per-Bank Enable Register	

#define GPIO_DIR_BASE           ( 0x10 )   	// GPIO Banks Direction Register                     
#define GPIO_OUT_DATA_BASE      ( 0x14 )   	// GPIO Banks Output Data Register                   
#define GPIO_SET_DATA_BASE      ( 0x18 )   	// GPIO Banks Set Data Register                      
#define GPIO_CLR_DATA_BASE      ( 0x1C )   	// GPIO Banks Clear Data Register                    
#define GPIO_IN_DATA_BASE       ( 0x20 )   	// GPIO Banks Input Data Register                    
#define GPIO_SET_RIS_TRIG_BASE  ( 0x24 )   	// GPIO Banks Set Rising Edge Interrupt Register     
#define GPIO_CLR_RIS_TRIG_BASE  ( 0x28 )   	// GPIO Banks Clear Rising Edge Interrupt Register   
#define GPIO_SET_FAL_TRIG_BASE  ( 0x2C )   	// GPIO Banks Set Falling Edge Interrupt Register    
#define GPIO_CLR_FAL_TRIG_BASE  ( 0x30 )   	// GPIO Banks Clear Falling Edge Interrupt Register  
#define GPIO_INSTAT_BASE        ( 0x34 )   	// GPIO Banks Interrupt Status Register              

// 参数定义

#define GPIO_IN                 1
#define GPIO_OUT                0

#define GPIO_BASE_OFFSET        0x28
  														
#define GPIO0_0                 0x00
#define GPIO0_1                 0x01
#define GPIO0_2                 0x02
#define GPIO0_3                 0x03
#define GPIO0_4                 0x04
#define GPIO0_5                 0x05
#define GPIO0_6                 0x06
#define GPIO0_7                 0x07
#define GPIO0_8                 0x08
#define GPIO0_9                 0x09
#define GPIO0_10                0x0A
#define GPIO0_11                0x0B
#define GPIO0_12                0x0C
#define GPIO0_13                0x0D
#define GPIO0_14                0x0E
#define GPIO0_15                0x0F

#define GPIO1_0                 0x10
#define GPIO1_1                 0x11
#define GPIO1_2                 0x12
#define GPIO1_3                 0x13
#define GPIO1_4                 0x14
#define GPIO1_5                 0x15
#define GPIO1_6                 0x16
#define GPIO1_7                 0x17
#define GPIO1_8                 0x18
#define GPIO1_9                 0x19
#define GPIO1_10                0x1A
#define GPIO1_11                0x1B
#define GPIO1_12                0x1C
#define GPIO1_13                0x1D
#define GPIO1_14                0x1E
#define GPIO1_15                0x1F

#define GPIO2_0                 0x20
#define GPIO2_1                 0x21
#define GPIO2_2                 0x22
#define GPIO2_3                 0x23
#define GPIO2_4                 0x24
#define GPIO2_5                 0x25
#define GPIO2_6                 0x26
#define GPIO2_7                 0x27
#define GPIO2_8                 0x28
#define GPIO2_9                 0x29
#define GPIO2_10                0x2A
#define GPIO2_11                0x2B
#define GPIO2_12                0x2C
#define GPIO2_13                0x2D
#define GPIO2_14                0x2E
#define GPIO2_15                0x2F

#define GPIO3_0                 0x30
#define GPIO3_1                 0x31
#define GPIO3_2                 0x32
#define GPIO3_3                 0x33
#define GPIO3_4                 0x34
#define GPIO3_5                 0x35
#define GPIO3_6                 0x36
#define GPIO3_7                 0x37
#define GPIO3_8                 0x38
#define GPIO3_9                 0x39
#define GPIO3_10                0x3A
#define GPIO3_11                0x3B
#define GPIO3_12                0x3C
#define GPIO3_13                0x3D
#define GPIO3_14                0x3E
#define GPIO3_15                0x3F

#define GPIO4_0                 0x40
#define GPIO4_1                 0x41
#define GPIO4_2                 0x42
#define GPIO4_3                 0x43
#define GPIO4_4                 0x44
#define GPIO4_5                 0x45
#define GPIO4_6                 0x46
#define GPIO4_7                 0x47
#define GPIO4_8                 0x48
#define GPIO4_9                 0x49
#define GPIO4_10                0x4A
#define GPIO4_11                0x4B
#define GPIO4_12                0x4C
#define GPIO4_13                0x4D
#define GPIO4_14                0x4E
#define GPIO4_15                0x4F

#define GPIO5_0                 0x50
#define GPIO5_1                 0x51
#define GPIO5_2                 0x52
#define GPIO5_3                 0x53
#define GPIO5_4                 0x54
#define GPIO5_5                 0x55
#define GPIO5_6                 0x56
#define GPIO5_7                 0x57
#define GPIO5_8                 0x58
#define GPIO5_9                 0x59
#define GPIO5_10                0x5A
#define GPIO5_11                0x5B
#define GPIO5_12                0x5C
#define GPIO5_13                0x5D
#define GPIO5_14                0x5E
#define GPIO5_15                0x5F

#define GPIO6_0                 0x60
#define GPIO6_1                 0x61
#define GPIO6_2                 0x62
#define GPIO6_3                 0x63
#define GPIO6_4                 0x64
#define GPIO6_5                 0x65
#define GPIO6_6                 0x66
#define GPIO6_7                 0x67
#define GPIO6_8                 0x68
#define GPIO6_9                 0x69
#define GPIO6_10                0x6A
#define GPIO6_11                0x6B
#define GPIO6_12                0x6C
#define GPIO6_13                0x6D
#define GPIO6_14                0x6E
#define GPIO6_15                0x6F

#define GPIO7_0                 0x70
#define GPIO7_1                 0x71
#define GPIO7_2                 0x72
#define GPIO7_3                 0x73
#define GPIO7_4                 0x74
#define GPIO7_5                 0x75
#define GPIO7_6                 0x76
#define GPIO7_7                 0x77
#define GPIO7_8                 0x78
#define GPIO7_9                 0x79
#define GPIO7_10                0x7A
#define GPIO7_11                0x7B
#define GPIO7_12                0x7C
#define GPIO7_13                0x7D
#define GPIO7_14                0x7E
#define GPIO7_15                0x7F

#define GPIO8_0                 0x80
#define GPIO8_1                 0x81
#define GPIO8_2                 0x82
#define GPIO8_3                 0x83
#define GPIO8_4                 0x84
#define GPIO8_5                 0x85
#define GPIO8_6                 0x86
#define GPIO8_7                 0x87
#define GPIO8_8                 0x88
#define GPIO8_9                 0x89
#define GPIO8_10                0x8A
#define GPIO8_11                0x8B
#define GPIO8_12                0x8C
#define GPIO8_13                0x8D
#define GPIO8_14                0x8E
#define GPIO8_15                0x8F

/* ------------------------------------------------------------------------ *
 *  GPIO相关函数原型                                                             *
 * ------------------------------------------------------------------------ */
short EVMOMAPL137_GPIO_setDirection ( unsigned short number, unsigned char direction );	
short EVMOMAPL137_GPIO_setOutput    ( unsigned short number, unsigned char output );
short EVMOMAPL137_GPIO_getInput     ( unsigned short number );

void OMAPL137_GPIO_setREI(unsigned short number);		// 设置GPIO上升沿触发
void OMAPL137_GPIO_clrREI(unsigned short number);		// 清除GPIO上升沿触发

void OMAPL137_GPIO_setFEI(unsigned short number);		// 设置GPIO下降沿触发
void OMAPL137_GPIO_clrFEI(unsigned short number);		// 清除GPIO下降沿触发

short OMAPL137_GPIO_ReadISR(unsigned short number);		// 读取GPIO中断状态
void OMAPL137_GPIO_ClrISR(unsigned short number);		// 清除GPIO中断标志

#endif
