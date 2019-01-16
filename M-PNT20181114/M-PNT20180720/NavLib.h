//////////////////文件名： NavLib.h ////////////////////////////////////////
////  导航模块的库函数头文件
// 主要功能： 用于生成库函数
/***********************************************************/


#ifndef _NavLib_
#define _NavLib_

//#include "C6747.h"


typedef struct
{
	float Acc[3];		//
	float Gyro[3];
	float Gyro_temp[3];
	float Acc_temp[3];

}IMUDataStruct;

typedef struct
{
	double dAtti[3];
	double dPos[3];
	double dV[3];
	double dVi[3];
	double dQ[4];
	double	dC[3][3];
	double	dT[3][3];
	double	dAibp[3];
	double	dWpbb[3];
	double	dPusa;   //航向 
	double Azimuth; //
	double	dAfa;    //游移角	
	double dInitVi[3];   //GNC 转导航时的初始速度  20170727
	double dInitPos[3];  //GNC 转导航时的初始位置  20170727
}NavResultStruct;

typedef struct _Nav_To_FCTON
{

  unsigned int   head;     // 帧头 
   unsigned int   T_SYS;     // 帧计数
   unsigned int   SC_STA;   // 自检状态
   unsigned int   NAV_STA;   //导航状态
   unsigned int   GNSS_STA;  // GPS状态
   unsigned int   GNSS_Week;  // gps周
   unsigned int   GNSS_mm  ;  // GPS周内秒
            int   NAV_Long ;  // 导航经度
		    int   NAV_Latt ;  // 导航纬度
		    float NAV_High ;  // 导航高度 
		    float NAV_VE   ;  // 导航东速
		    float NAV_VN   ;   //导航北速
		    float NAV_VU   ; // 导航天速
		    float NAV_Pitch;//俯仰角 
		    float NAV_Yaw;  //航向角 
		    float NAV_Roll;// 横滚角 
		    float AccX;    // m/s^2
		    float AccY;
		    float AccZ;
			float Wx;    // °/s
			float Wy;
			float Wz;
			int   GNSS_Long;
			int   GNSS_Latt;
			float GNSS_High;
			float GNSS_VE;
			float GNSS_VN;
			float GNSS_VU;
			float GNSS_PDOP;
			float static_press;
			float static_temp;
			float dynam_press;
			float dynam_temp;
			float imu_temp;
			float groy_temp;
	unsigned int  checksum;
   //

 }NavToFControlStruct;
/***** Const****************/
#define 	C_RE               	6378137.0
#define 	C_E               	3.35281066e-3
#define 	C_WIE          		7.2921165145e-5
#define 	C_PI               	3.141592653589793
#define 	C_R2D         		57.29577951308232
#define  	C_D2R   			0.017453292519943296   
#define 	C_kg               	2.06264806247e+5    
#define 	C_g0_1            	9.7803
#define 	C_NavTime         	0.005     			// 导航时间    // 091013  5ms
#define 	C_up_max   	        1.52  



void    pntowa_WRJ(float AD_Vcc[],IMUDataStruct *IMUData);   		//无人机惯导用,
void	navig( IMUDataStruct *IMUData, NavResultStruct  *NavResult);	
void 	wpbb( IMUDataStruct *IMUData,NavResultStruct  *NavResult);
void 	dqtoq( NavResultStruct  *NavResult)  ;
void 	qto1( NavResultStruct  *NavResult);
void 	ttovh( IMUDataStruct *IMUData, NavResultStruct  *NavResult);
void 	qtot( NavResultStruct  *NavResult) ;
void 	ttoatti( NavResultStruct  *NavResult);
void 	wepp( NavResultStruct  *NavResult);
void 	wipb( NavResultStruct  *NavResult);
void 	update_c( NavResultStruct  *NavResult);
void 	gaccel( NavResultStruct  *NavResult);
void 	ctofild( NavResultStruct  *NavResult);
void 	cvtovi( NavResultStruct  *NavResult);
void 	cvitov( NavResultStruct  *NavResult);
void	postoc( NavResultStruct  *NavResult);

#endif
