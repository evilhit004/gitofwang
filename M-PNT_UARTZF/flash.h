/************************************************************************
*
* 文件名   ：   Main_H130.h
* 版权说明 ：   十三研究所捷联系统研究室
* 版本号   ：   1.0
* 创建时间 ：   2017.09.10
* 作    者 ：   捷联系统研究室
* 功能描述 ：   H130高过载旋转炮弹 —— Flash相关子程序
* 使用注意 ： 
* 修改记录 ：  
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
	float fk0g[3][3];   //== 陀螺温度系数
	float fk0a[3][3];		//== 加表温度系数
	float fp0[3];       //== 气压计温度系数
	float fEg[3][3];		//== 陀螺安装误差逆矩阵
	float fEa[3][3];		//== 加表安装误差逆矩阵
	float fEga[3][3];		//== G值敏感性误差
	float fbg[3];       //== 陀螺零位
	float fba[3];       //== 加表零位
	float fKg[3];       //== 陀螺标度因数
	float fKa[3];       //== 加表标度因数
	float fk0w[3];      //== 陀螺零位的修正值，用于长期贮存后扣零位用,单位： rad/s
 };


void byte_out( UINT8 cByte,UART_Handle UART_main);
UINT8 byte_in(UART_Handle UART_main);

UINT8 receive_program(UART_Handle UART_main);
void Return_XS(UART_Handle UART_main);          //== 系数会读
void Read_xs_2(struct _CAL_Par *ptr_pnxscompen_flash);
void receive_xs(UART_Handle UART_main);
//void Read_H130_xs(struct _CAL_Par *ptr_pnxscompen_flash);
//void Read_MCxs(float Mag_G[],float Mag_b[]);    //== 读磁系数 
//void Read_xs_1(struct _CAL_Par *ptr_pnxscompen_flash);
//void ReNew_XS_2_Gyro(struct _CAL_Par *ptr_pnxscompen_flash,FLOAT64 GyroW0[]);   //== 静止10s扣陀螺的零位，将零位写入系数版本2.0文件



#endif
