/************************************************************************
*
* 文件名   ：   Main_H130.h
* 版权说明 ：   十三研究所捷联系统研究室
* 版本号   ：   1.0
* 创建时间 ：   2017.09.10
* 作    者 ：   捷联系统研究室
* 功能描述 ：   H130高过载旋转炮弹导航解算程序
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

///////////////////////////////////////////////////////////////////////////
//*************************** 开关标准值 *********************************/
///////////////////////////////////////////////////////////////////////////
#ifndef _SWITCH_VALUE
#define _SWITCH_VALUE

#define _TRUE   1
#define _FALSE  0

#endif

///////////////////////////////////////////////////////////////////////////
//********************* redefine the data type ***************************/
///////////////////////////////////////////////////////////////////////////
#ifndef DSP_DATA_TYPES
#define DSP_DATA_TYPES

typedef unsigned char 	UINT8;
typedef unsigned short	UINT16;
typedef unsigned int		UINT32;
typedef unsigned long		UINT64;
typedef signed char 		INT8;
typedef signed short		INT16;
typedef signed int      INT32;
typedef signed long     INT64;
typedef float           FLOAT32;
typedef double          FLOAT64;

#endif


//===== Ratio =====//	
#define C_RATETENMILLION    10000000    //== 10^7
#define C_RATETENTHOUSAND   10000
#define C_RARETWOTHOUSANDS  2000
#define C_RATETHOUSAND      1000
#define C_RATEHUNDRED       100
#define C_RATETWENTY        20
#define C_RATETEN           10
#define C_RATEMILLION_NI    0.000001    //== 10^-6
#define C_RARETWOTHOUS_NI   0.0005
#define C_RATETENTHOUS_NI  	0.0001
#define C_RARETHOUSAND_NI   0.001
#define C_RATEHUNDRED_NI   	0.01
#define C_RATETEN_NI 	  		0.1

#define C_RE               	6378137.0
#define C_E                 3.35281066e-3
#define C_WIE               7.2921165145e-5     //== 15.04/3600/57.3
#define C_PI                3.141592653589793
#define C_R2D               57.29577951308232
#define C_D2R               0.017453292519943296   
#define C_KG                2.06264806247e+5    
#define C_G0_1              9.7803
#define C_G0_BD             9.801275    //== 标定地点的重力加速度值，用于IMU输出使用
//#define C_groy_K0         1.0         //== 0.005			//陀螺每秒漂移值  °/s
#define C_Max               1.52



