/********************************************************************************/
// c6713.h�ļ�
// �汾��		2.1.0.
// �ļ�˵����	C6713��ؼĴ�������ͺ���ԭ��
// ��д�ߣ�		������
// ��дʱ�䣺	2012.09.02.
// ����ʱ�䣺	2014.09.11.
// ����˵����
// 	2014.03.21��
// 		1.��DSP�Ĵ�����ָ��������unsigned long *�޸�Ϊunsigned int *��ʹ��
// 	  	  unsigned long *�����д���ַ�ĳ�ͻ����
//	2014.09.09.
//		1.����PLLInit(void)�޸�ΪPLLInit(unsigned int multi_num)
//	2014.09.11.
//		1.�Ĵ����ĺ궨����(unsigned int *)(ADDR)��ΪADDR
//		2.�ļ���ֻ������C6713��صĶ��壬�����ķ��ڿ���Ӳ������ʦ�޸ĵ�C6713_init.c�ļ���
//		3.����PLLInit(unsigned int)�����޸�ΪC6713_PLLInit(unsigned int)���Է�ֹ
//		  ������DSP����غ��������ظ�
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
