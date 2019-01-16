/************************************************************************
*
* 文件名   ：   GPS_DVL.h
* 版权说明 ：   十三研究所捷联系统研究室
* 版本号   ：   1.0
* 创建时间 ：   2016.09.10
* 作    者 ：   捷联系统研究室
* 功能描述 ：   GPS解码程序
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef _GPS_DVL_H_
#define _GPS_DVL_H_


#include "NavLib.h"
//#include "C6747.h"
//#include "Uart_IP.h"


#define C_D2M             1.666666666667 	
#define CRC32_POLYNOMIAL  0xEDB88320L
#define C_HUNREDN 0.01
#define	C_GPS_V_delt	  2


#define		WGS84_A			(6378137.0E0)						//地球长半轴（即赤道横截面半径a?
#define		WGS84_F			(1/298.257223563)		// 地球扁率的倒数


typedef struct
{	
    double  Pos[3];
    double  V[3];
    double  PosStd[3];
	double  VelStd[3];
    double  Course;           // GPS航迹角
	double  DGPS_Course;      // 双天线GPS航向角
    double  SVs_used;
    double  pdop;
    double  Vearth;
	float   g_uisAcc;
	double	 g_uisVcc;		// 速度精度因子
    unsigned char    Pos_OK;
    unsigned char    Vel_OK;    
    int    Nogps_time;  			// GPS无效时间累积
	unsigned int   posttime;              // 跑车用
    unsigned char	 Flag_Synchro;			// GPS秒脉冲触发标志
    unsigned char	 Flag_KalGpsValid;		// GPS数据是否有效标志,有效时为 0x55
	unsigned int   Tweek_second;          // GPS周内秒
	unsigned short   Tweek;  
	double  CECFV[3];                // GPS周
//    ucCnt_GpsDelayValid  
}GPS_STR;


/********************* Subprogram Declamation ***************************/
void Read_Gps_PASHR( volatile unsigned short *address_uart, GPS_STR *GpsDat);  
void Read_Gps_BESTB( volatile unsigned short *address_uart, GPS_STR *GpsDat);  
void Read_Gps_GGAVTG(GPS_STR *GpsDat);	
void Read_Gps_WRJ(unsigned int address_uart,GPS_STR *GpsDat);		//无人机用，输入地址读到的数据
 
unsigned char  JudgeGpsValid( GPS_STR *GpsDat );
unsigned char  Read_DVL( volatile unsigned short *address_uart, double *dDVLVelo );  
	 
void 	gvmov(NavResultStruct  *NavResult,GPS_STR *GpsDat);			// GPS速度修正惯导速度
void	Gps_pos_modify(NavResultStruct  *NavResult,GPS_STR *GpsDat);	// GPS位置修正惯导位置
double  GPS_Course(GPS_STR *GpsDat);	// GPS速度求航迹角，返回航迹角，同时结构体内航迹角也更新

void XYZ_to_BLH(double X, double Y, double Z, double* B, double* L, double* H);
void ReadGnssData(volatile unsigned short *address_uart,GPS_STR *GpsDat); // NOVTAL
void Speed_XYZ_to_BLH(double B, double L, double H, double speedX, double speedY, double speedZ,
						  double* speedB, double* speedL, double* speedH);

#endif

