//////////////////�ļ����� NavLib.h ////////////////////////////////////////
////  ����ģ��Ŀ⺯��ͷ�ļ�
// ��Ҫ���ܣ� �������ɿ⺯��
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
	double	dPusa;   //���� 
	double Azimuth; //
	double	dAfa;    //���ƽ�	
	double dInitVi[3];   //GNC ת����ʱ�ĳ�ʼ�ٶ�  20170727
	double dInitPos[3];  //GNC ת����ʱ�ĳ�ʼλ��  20170727
}NavResultStruct;

typedef struct _Nav_To_FCTON
{

  unsigned int   head;     // ֡ͷ 
   unsigned int   T_SYS;     // ֡����
   unsigned int   SC_STA;   // �Լ�״̬
   unsigned int   NAV_STA;   //����״̬
   unsigned int   GNSS_STA;  // GPS״̬
   unsigned int   GNSS_Week;  // gps��
   unsigned int   GNSS_mm  ;  // GPS������
            int   NAV_Long ;  // ��������
		    int   NAV_Latt ;  // ����γ��
		    float NAV_High ;  // �����߶� 
		    float NAV_VE   ;  // ��������
		    float NAV_VN   ;   //��������
		    float NAV_VU   ; // ��������
		    float NAV_Pitch;//������ 
		    float NAV_Yaw;  //����� 
		    float NAV_Roll;// ����� 
		    float AccX;    // m/s^2
		    float AccY;
		    float AccZ;
			float Wx;    // ��/s
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
#define 	C_NavTime         	0.005     			// ����ʱ��    // 091013  5ms
#define 	C_up_max   	        1.52  



void    pntowa_WRJ(float AD_Vcc[],IMUDataStruct *IMUData);   		//���˻��ߵ���,
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
