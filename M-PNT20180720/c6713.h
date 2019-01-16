/********************************************************************************/
// c6713.h文件
// 版本：		2.1.0.
// 文件说明：	C6713相关寄存器定义和函数原型
// 编写者：		车鹏宇
// 编写时间：	2012.09.02.
// 更新时间：	2014.09.11.
// 更新说明：
// 	2014.03.21：
// 		1.将DSP寄存器的指针类型由unsigned long *修改为unsigned int *，使用
// 	  	  unsigned long *会造成写入地址的冲突错误。
//	2014.09.09.
//		1.函数PLLInit(void)修改为PLLInit(unsigned int multi_num)
//	2014.09.11.
//		1.寄存器的宏定义由(unsigned int *)(ADDR)改为ADDR
//		2.文件中只保留与C6713相关的定义，函数改放在可由硬件工程师修改的C6713_init.c文件中
//		3.函数PLLInit(unsigned int)名称修改为C6713_PLLInit(unsigned int)，以防止
//		  和其他DSP的相关函数名称重复
/********************************************************************************/
#ifndef DEC6713_
#define DEC6713_

#define EMIF_GCR       	0x01800000  //EMIF global control
#define EMIF_CE1        0x01800004  //address of EMIF CE1 control reg.
#define EMIF_CE0        0x01800008  //EMIF CE0control
#define EMIF_CE2        0x01800010  //EMIF CE2control
#define EMIF_CE3        0x01800014  //EMIF CE3control
#define EMIF_SDCTRL		0x01800018  //EMIF SDRAM control
#define EMIF_SDRP   	0x0180001C  //EMIF SDRAM timer
#define EMIF_SDEXT	   	0x01800020  //EMIF SDRAM extension

#define DEVCFG			0x019C0200  //device configuration reg

#define GPEN	   		0x01B00000
#define GPDIR	   		0x01B00004
#define GPPOL	   		0x01B00024

#define EXTPOL	   		0x019C0008	//External interrupt polarity

#define I2CIER1	   		0x01B44004	//I2Cx interrupt enable register
#define I2CSTR1	   		0x01B44008	//I2Cx interrupt status register
#define I2CCLKL1	   	0x01B4400C	//I2Cx clock low-time divider register
#define I2CCLKH1		0x01B44010	//I2Cx clock high-time divider register
#define I2CCNT1			0x01B44014	//I2Cx data count register
#define I2CDRR1			0x01B44018	//I2Cx data receive register
#define I2CSAR1			0x01B4401C	//I2Cx slave address register
#define I2CDXR1			0x01B44020	//I2Cx data transmit register
#define I2CMDR1			0x01B44024	//I2Cx mode register
#define I2CISRC1		0x01B44028	//I2Cx interrupt source register
#define I2CPSC1			0x01B44030	//I2Cx prescaler register


#endif
