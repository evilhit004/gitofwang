/************************************************************************
*
* �ļ���   ��   GPS_DVL.h
* ��Ȩ˵�� ��   ʮ���о�������ϵͳ�о���
* �汾��   ��   1.0
* ����ʱ�� ��   2016.09.10
* ��    �� ��   ����ϵͳ�о���
* �������� ��   GPS�������
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
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


#define		WGS84_A			(6378137.0E0)						//���򳤰��ᣨ����������뾶a?
#define		WGS84_F			(1/298.257223563)		// ������ʵĵ���


typedef struct
{	
    double  Pos[3];
    double  V[3];
    double  PosStd[3];
	double  VelStd[3];
    double  Course;           // GPS������
	double  DGPS_Course;      // ˫����GPS�����
    double  SVs_used;
    double  pdop;
    double  Vearth;
	float   g_uisAcc;
	double	 g_uisVcc;		// �ٶȾ�������
    unsigned char    Pos_OK;
    unsigned char    Vel_OK;    
    int    Nogps_time;  			// GPS��Чʱ���ۻ�
	unsigned int   posttime;              // �ܳ���
    unsigned char	 Flag_Synchro;			// GPS�����崥����־
    unsigned char	 Flag_KalGpsValid;		// GPS�����Ƿ���Ч��־,��ЧʱΪ 0x55
	unsigned int   Tweek_second;          // GPS������
	unsigned short   Tweek;  
	double  CECFV[3];                // GPS��
//    ucCnt_GpsDelayValid  
}GPS_STR;


/********************* Subprogram Declamation ***************************/
void Read_Gps_PASHR( volatile unsigned short *address_uart, GPS_STR *GpsDat);  
void Read_Gps_BESTB( volatile unsigned short *address_uart, GPS_STR *GpsDat);  
void Read_Gps_GGAVTG(GPS_STR *GpsDat);	
void Read_Gps_WRJ(unsigned int address_uart,GPS_STR *GpsDat);		//���˻��ã������ַ����������
 
unsigned char  JudgeGpsValid( GPS_STR *GpsDat );
unsigned char  Read_DVL( volatile unsigned short *address_uart, double *dDVLVelo );  
	 
void 	gvmov(NavResultStruct  *NavResult,GPS_STR *GpsDat);			// GPS�ٶ������ߵ��ٶ�
void	Gps_pos_modify(NavResultStruct  *NavResult,GPS_STR *GpsDat);	// GPSλ�������ߵ�λ��
double  GPS_Course(GPS_STR *GpsDat);	// GPS�ٶ��󺽼��ǣ����غ����ǣ�ͬʱ�ṹ���ں�����Ҳ����

void XYZ_to_BLH(double X, double Y, double Z, double* B, double* L, double* H);
void ReadGnssData(volatile unsigned short *address_uart,GPS_STR *GpsDat); // NOVTAL
void Speed_XYZ_to_BLH(double B, double L, double H, double speedX, double speedY, double speedZ,
						  double* speedB, double* speedL, double* speedH);

#endif

