/************************************************************************
*
* �ļ���   ��   Main_H130.h
* ��Ȩ˵�� ��   ʮ���о�������ϵͳ�о���
* �汾��   ��   1.0
* ����ʱ�� ��   2017.09.10
* ��    �� ��   ����ϵͳ�о���
* �������� ��   H130�߹�����ת�ڵ� ���� Flash����ӳ���
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef __flash__
#define __flash__


#include "omapl137.h"
//#include "omapl137_uart.h"

//////==== Const Heading File ========
#ifndef CONST_H_H
#define CONST_H_H 
#include "Const_All.h"
#endif


struct _CAL_Par
{
	float fk0g[3][3];   //== �����¶�ϵ��
	float fk0a[3][3];		//== �ӱ��¶�ϵ��
	float fp0[3];       //== ��ѹ���¶�ϵ��
	float fEg[3][3];		//== ���ݰ�װ��������
	float fEa[3][3];		//== �ӱ�װ��������
	float fEga[3][3];		//== Gֵ���������
	float fbg[3];       //== ������λ
	float fba[3];       //== �ӱ���λ
	float fKg[3];       //== ���ݱ������
	float fKa[3];       //== �ӱ�������
	float fk0w[3];      //== ������λ������ֵ�����ڳ�����������λ��,��λ�� rad/s
 };


void byte_out( UINT8 cByte,UART_Handle UART_main);
UINT8 byte_in(UART_Handle UART_main);

UINT8 receive_program(UART_Handle UART_main);
void Return_XS(UART_Handle UART_main);          //== ϵ�����
void Read_xs_2(struct _CAL_Par *ptr_pnxscompen_flash);
void receive_xs(UART_Handle UART_main);
//void Read_H130_xs(struct _CAL_Par *ptr_pnxscompen_flash);
//void Read_MCxs(float Mag_G[],float Mag_b[]);    //== ����ϵ�� 
//void Read_xs_1(struct _CAL_Par *ptr_pnxscompen_flash);
//void ReNew_XS_2_Gyro(struct _CAL_Par *ptr_pnxscompen_flash,FLOAT64 GyroW0[]);   //== ��ֹ10s�����ݵ���λ������λд��ϵ���汾2.0�ļ�



#endif
