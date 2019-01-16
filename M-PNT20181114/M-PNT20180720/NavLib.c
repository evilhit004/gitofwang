/************************************************************/
//////////////////ÎÄ¼þÃû£º NavLib.c ////////////////////////////////////////
//Ö÷Òª¹¦ÄÜ£º ÓÃÓÚ µ¼º½Ä£¿éµÄ¿â
//1.
//
/***********************************************************/
#include "stdio.h"
#include "NavLib.h"
#include "MatrixLib.h"


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


static  FLOAT64	g_dWipb[3]={0,0,0};
static  FLOAT64	g_dWepp[3]={0,0,0};
static  FLOAT64	g_dG_wip[3]={0,0,0};
static  FLOAT64	g_dWipp[3]={0,0,0};
//extern  FLOAT64	g_dGaccel[3];
 FLOAT64	g_dGaccel[3];

/***************************************************************/
/* º¯ÊýÃû³Æ£ºnavig()
    º¯¹¦ÄÜ£º¹ßÐÔµ¼º½º¯Êý
    ÊäÈë²ÎÊý£ºÒÇ±íÊäÈë£¬µ¼º½½á¹û
    Êä³ö²ÎÊý£ºÎÞ */
/***************************************************************/	   
void navig( IMUDataStruct *IMUData, NavResultStruct  *NavResult)
{
	wpbb(IMUData,NavResult);
	dqtoq(NavResult);
	qto1(NavResult);
	qtot(NavResult);
	ttovh(IMUData,NavResult);
	ttoatti(NavResult);
  	wepp(NavResult);
	wipb(NavResult);		
	update_c(NavResult);
	gaccel(NavResult);	
	ctofild(NavResult);
	cvtovi(NavResult);
}

/***************************************************************/
/* º¯ÊýÃû³Æ£º
    º¯Êý¹¦ÄÜ£ºwpbb
    ÊäÈë²ÎÊý£ºÍÓÂÝºÍ¼Ó±íÖµ£¬¹ú¼Êµ¥Î»£¬rad/s  m/s^2,Òò´ËÐèÒª×ª»»Îª½ÇÔöÁ¿³ËÒÔµ¼º½ÖÜÆÚ
    Êä³ö²ÎÊý£º*/
/***************************************************************/	  	
void wpbb( IMUDataStruct *IMUData,NavResultStruct  *NavResult)	  
{
	NavResult->dWpbb[0]=IMUData->Gyro[0]*C_NavTime-g_dWipb[0];
	NavResult->dWpbb[1]=IMUData->Gyro[1]*C_NavTime-g_dWipb[1];
	NavResult->dWpbb[2]=IMUData->Gyro[2]*C_NavTime-g_dWipb[2];

}

/***************************************************************/
/* º¯ÊýÃûÆ£?
    º¯Êý¹¦ÄÜ£ºqÕó¸üÐÂ
    äÈ²ÎÊý?
    Êä³ö²ÎÊý£º*/
/***************************************************************/	  
void dqtoq( NavResultStruct  *NavResult)
{
	FLOAT64 fTemp[4],fTempq[4],fWpb;
	fWpb=NavResult->dWpbb[0]*NavResult->dWpbb[0]+NavResult->dWpbb[1]*NavResult->dWpbb[1]+NavResult->dWpbb[2]*NavResult->dWpbb[2];	
	fTemp[0]=1.0-fWpb/8.0+fWpb*fWpb/384.0;      
  	fTemp[1]=(0.5-fWpb/48.0)*NavResult->dWpbb[0];
	fTemp[2]=(0.5-fWpb/48.0)*NavResult->dWpbb[1];       
	fTemp[3]=(0.5-fWpb/48.0)*NavResult->dWpbb[2];

	fTempq[0]=NavResult->dQ[0]*fTemp[0]-NavResult->dQ[1]*fTemp[1]-NavResult->dQ[2]*fTemp[2]-NavResult->dQ[3]*fTemp[3];
	fTempq[1]=NavResult->dQ[1]*fTemp[0]+NavResult->dQ[0]*fTemp[1]-NavResult->dQ[3]*fTemp[2]+NavResult->dQ[2]*fTemp[3];
	fTempq[2]=NavResult->dQ[2]*fTemp[0]+NavResult->dQ[3]*fTemp[1]+NavResult->dQ[0]*fTemp[2]-NavResult->dQ[1]*fTemp[3];
	fTempq[3]=NavResult->dQ[3]*fTemp[0]-NavResult->dQ[2]*fTemp[1]+NavResult->dQ[1]*fTemp[2]+NavResult->dQ[0]*fTemp[3];	
	NavResult->dQ[0]=fTempq[0];
	NavResult->dQ[1]=fTempq[1];
	NavResult->dQ[2]=fTempq[2];
	NavResult->dQ[3]=fTempq[3];
}

/***************************************************************/
/* º¯ÊýÃû³Æ£º
    º¯¹¦ÄÜ£ºËÄÔªËØµÄ¹éÒ»»¯
    ÊäÈë²ÎÊý£º
    Êä³ö²ÎÊý£º*/
/***************************************************************/	  
void qto1( NavResultStruct  *NavResult)
{
	FLOAT64 ftemp=0;
	ftemp=sqrt(NavResult->dQ[0]*NavResult->dQ[0]+NavResult->dQ[1]*NavResult->dQ[1]+NavResult->dQ[2]*NavResult->dQ[2]+NavResult->dQ[3]*NavResult->dQ[3]);
	NavResult->dQ[0]=NavResult->dQ[0]/ftemp;
	NavResult->dQ[1]=NavResult->dQ[1]/ftemp;
	NavResult->dQ[2]=NavResult->dQ[2]/ftemp;
	NavResult->dQ[3]=NavResult->dQ[3]/ftemp;
}

/***************************************************************/
/* º¯ÊýÃû³Æ£º
    º¯Êý¹¦ÄÜ£ºÓÉ¸üÐÂµÄQÕóÇó½âTÕó
    ÊäÈë²ÎÊý?
    Êä³ö²ÎÊý£º*/
/***************************************************************/	  
void qtot( NavResultStruct  *NavResult)
{
	FLOAT64 fQ00=0.0,fQ01=0.0,fQ02=0.0,fQ03=0.0,fQ11=0.0,fQ12=0.0,fQ13=0.0,fQ22=0.0,fQ23=0.0,fQ33=0.0;
	fQ00=NavResult->dQ[0]*NavResult->dQ[0];    
	fQ01=NavResult->dQ[0]*NavResult->dQ[1];   
	fQ02=NavResult->dQ[0]*NavResult->dQ[2];     
	fQ03=NavResult->dQ[0]*NavResult->dQ[3];     
	fQ11=NavResult->dQ[1]*NavResult->dQ[1];
	fQ12=NavResult->dQ[1]*NavResult->dQ[2];    
	fQ13=NavResult->dQ[1]*NavResult->dQ[3];   
	fQ22=NavResult->dQ[2]*NavResult->dQ[2];     
	fQ23=NavResult->dQ[2]*NavResult->dQ[3];     
	fQ33=NavResult->dQ[3]*NavResult->dQ[3];
	NavResult->dT[0][0]=fQ00+fQ11-fQ22-fQ33;    NavResult->dT[0][1]=2*(fQ12-fQ03);        NavResult->dT[0][2]=2*(fQ13+fQ02);
	NavResult->dT[1][0]=2*(fQ12+fQ03);          NavResult->dT[1][1]=fQ00-fQ11+fQ22-fQ33;  NavResult->dT[1][2]=2*(fQ23-fQ01);
	NavResult->dT[2][0]=2*(fQ13-fQ02);          NavResult->dT[2][1]=2*(fQ23+fQ01);         NavResult->dT[2][2]=fQ00-fQ11-fQ22+fQ33;
}

/***************************************************************/
/* º¯Êýû³Æ£?
    º¯Êý¹¦ÄÜ£ºËÙ¶È¸üÐÂ
    ÊäÈë²ÎÊý£º
    Êä³ö²ÎÊý£º*/
/***************************************************************/	  
void ttovh( IMUDataStruct *IMUData, NavResultStruct  *NavResult)
{
	FLOAT64 fTempg[3]={0.0,0.0,0.0};
	FLOAT64 fhtemp=0;
	NavResult->dAibp[0]=(NavResult->dT[0][0]*IMUData->Acc[0]+NavResult->dT[0][1]*IMUData->Acc[1]+NavResult->dT[0][2]*IMUData->Acc[2]);
	NavResult->dAibp[1]=(NavResult->dT[1][0]*IMUData->Acc[0]+NavResult->dT[1][1]*IMUData->Acc[1]+NavResult->dT[1][2]*IMUData->Acc[2]);
	NavResult->dAibp[2]=(NavResult->dT[2][0]*IMUData->Acc[0]+NavResult->dT[2][1]*IMUData->Acc[1]+NavResult->dT[2][2]*IMUData->Acc[2]);		
	
	fTempg[0]=NavResult->dAibp[0]*C_NavTime-g_dGaccel[0]; 
	fTempg[1]=NavResult->dAibp[1]*C_NavTime-g_dGaccel[1];	
	fTempg[2]=NavResult->dAibp[2]*C_NavTime-g_dGaccel[2];
	
//	fvtemp[0] = NavResult->dV[0];  fvtemp[1] = NavResult->dV[1]; fvtemp[2] = NavResult->dV[2];
	fhtemp = NavResult->dPos[2];
	
	NavResult->dV[0] = NavResult->dV[0]+fTempg[0]; 
	NavResult->dV[1] = NavResult->dV[1]+fTempg[1];	
	NavResult->dV[2] = NavResult->dV[2]+fTempg[2];	   //´ò¿ªÌìÏòËÙ¶ÈÍ¨µÀ¸üÐÂ  20160317
	NavResult->dPos[2] = fhtemp+C_NavTime*(0.5*fTempg[2]+NavResult->dV[2]); 


}

/***************************************************************/
/* º¯ýÃû³Æ£?
    º¯Êý¹¦ÄÜºÓÉTÕóÇó½â×ËÌ¬½Ç
    ÊäÈëÎÊý£?
    Êä³ö²ÎÊý£º*/
/***************************************************************/	  
void ttoatti( NavResultStruct  *NavResult)   
{
	NavResult->dAtti[0]=atan(NavResult->dT[2][1]/sqrt(1-NavResult->dT[2][1]*NavResult->dT[2][1]));
	if(fabs(NavResult->dAtti[0])>C_up_max) /*up_max=1.52*/
	{	
		NavResult->dPusa=atan(NavResult->dT[1][0]/sqrt(1-NavResult->dT[1][0]*NavResult->dT[1][0]));
	}
	else
	{	NavResult->dAtti[1]=atan(-NavResult->dT[2][0]/NavResult->dT[2][2]);
		if(NavResult->dT[2][2]<0.0)
		{	if(NavResult->dAtti[1]>0.0)
				NavResult->dAtti[1]=NavResult->dAtti[1]-C_PI;
			else	
				NavResult->dAtti[1]=NavResult->dAtti[1]+C_PI;
		}					
  		NavResult->dPusa=atan(-NavResult->dT[0][1]/NavResult->dT[1][1]);
		if(NavResult->dT[1][1]<0.0)
		{	
			NavResult->dPusa=NavResult->dPusa+C_PI;
		}
		else
		{	
			if(NavResult->dPusa<0.0)
				NavResult->dPusa=NavResult->dPusa+2.0*C_PI;
		}	
	}

    if(NavResult->dPusa<1e-12)
	{
	     NavResult->dPusa+=2.0 * C_PI;
	}


}
/***************************************************************/
/* º¯ÊýÃû³Æ£º
    º¯Êý¹¦ÄÜ£ºÎ»ÖÃËÙÂÊ¼ÆËã
    ÊäÈë²ÎÊý£º
    Êä³ö²ÎÊý£º*/
/***************************************************************/	  
void wepp( NavResultStruct  *NavResult)  
{
	FLOAT64 fTemp[2][2]={0.0,0.0,0.0,0.0};
	fTemp[0][0]=-2.0*C_E*NavResult->dC[0][2]*NavResult->dC[1][2]/C_RE*C_NavTime;
	fTemp[0][1]=-(1.0-C_E*NavResult->dC[2][2]*NavResult->dC[2][2]+2.0*C_E*NavResult->dC[1][2]*NavResult->dC[1][2])/C_RE*C_NavTime;    
	fTemp[1][0]=(1.0-C_E*NavResult->dC[2][2]*NavResult->dC[2][2]+2.0*C_E*NavResult->dC[0][2]*NavResult->dC[0][2])/C_RE*C_NavTime;
	fTemp[1][1]=-fTemp[0][0];
	g_dWepp[0]=fTemp[0][0]*NavResult->dV[0]+fTemp[0][1]*NavResult->dV[1];
	g_dWepp[1]=fTemp[1][0]*NavResult->dV[0]+fTemp[1][1]*NavResult->dV[1];
	
}
/***************************************************************/
/* º¯ÊýÃû³Æ£º
    º¯Êý¹¦ÄÜºwipb
    ÊäÈë²ÎÊýº
    Êä³ö²ÎÊý£º*/
/***************************************************************/	  
void wipb( NavResultStruct  *NavResult)   
{
	g_dG_wip[0]=g_dWepp[0]+2.0*C_WIE*NavResult->dC[0][2]*C_NavTime;
	g_dG_wip[1]=g_dWepp[1]+2.0*C_WIE*NavResult->dC[1][2]*C_NavTime;
	g_dG_wip[2]=2.0*C_WIE*NavResult->dC[2][2]*C_NavTime;

	g_dWipp[0]=g_dWepp[0]+C_WIE*NavResult->dC[0][2]*C_NavTime;
	g_dWipp[1]=g_dWepp[1]+C_WIE*NavResult->dC[1][2]*C_NavTime;	
	g_dWipp[2]=C_WIE*NavResult->dC[2][2]*C_NavTime;		
	
	g_dWipb[0]=NavResult->dT[0][0]*g_dWipp[0]+NavResult->dT[1][0]*g_dWipp[1]+NavResult->dT[2][0]*g_dWipp[2];	
	g_dWipb[1]=NavResult->dT[0][1]*g_dWipp[0]+NavResult->dT[1][1]*g_dWipp[1]+NavResult->dT[2][1]*g_dWipp[2];
	g_dWipb[2]=NavResult->dT[0][2]*g_dWipp[0]+NavResult->dT[1][2]*g_dWipp[1]+NavResult->dT[2][2]*g_dWipp[2];
}

/***************************************************************/
/* º¯ÊýÃû³Æ£º
    º¯Êý¹¦ÄÜ£ºÎ»ÖÃ¾ØÕóµÄ¸üÐÂ
    Êäë²ÎÊý£?
    Êä³ö²ÎÊý£º*/
/***************************************************************/	  
void update_c( NavResultStruct  *NavResult)
{	
  	FLOAT64 fUpdate_c[3][3]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	FLOAT64 fTempc[3][3]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	fUpdate_c[0][0]=1.0-0.5*g_dWepp[1]*g_dWepp[1];
	fUpdate_c[0][1]=    0.5*g_dWepp[0]*g_dWepp[1];	
	fUpdate_c[0][2]=    -g_dWepp[1];
	fUpdate_c[1][0]=    0.5*g_dWepp[0]*g_dWepp[1];	
	fUpdate_c[1][1]=1.0-0.5*g_dWepp[0]*g_dWepp[0];	
	fUpdate_c[1][2]=     g_dWepp[0];
	fUpdate_c[2][0]=    g_dWepp[1];	                
	fUpdate_c[2][1]=    -g_dWepp[0];	                
	fUpdate_c[2][2]=1.0-0.5*g_dWepp[0]*g_dWepp[0]-0.5*g_dWepp[1]*g_dWepp[1];
	fTempc[0][0]=fUpdate_c[0][0]*NavResult->dC[0][0]+fUpdate_c[0][1]*NavResult->dC[1][0]+fUpdate_c[0][2]*NavResult->dC[2][0];
	fTempc[0][1]=fUpdate_c[0][0]*NavResult->dC[0][1]+fUpdate_c[0][1]*NavResult->dC[1][1]+fUpdate_c[0][2]*NavResult->dC[2][1];
	fTempc[0][2]=fUpdate_c[0][0]*NavResult->dC[0][2]+fUpdate_c[0][1]*NavResult->dC[1][2]+fUpdate_c[0][2]*NavResult->dC[2][2];
	fTempc[1][0]=fUpdate_c[1][0]*NavResult->dC[0][0]+fUpdate_c[1][1]*NavResult->dC[1][0]+fUpdate_c[1][2]*NavResult->dC[2][0];
	fTempc[1][1]=fUpdate_c[1][0]*NavResult->dC[0][1]+fUpdate_c[1][1]*NavResult->dC[1][1]+fUpdate_c[1][2]*NavResult->dC[2][1];
	fTempc[1][2]=fUpdate_c[1][0]*NavResult->dC[0][2]+fUpdate_c[1][1]*NavResult->dC[1][2]+fUpdate_c[1][2]*NavResult->dC[2][2];
	fTempc[2][0]=fUpdate_c[2][0]*NavResult->dC[0][0]+fUpdate_c[2][1]*NavResult->dC[1][0]+fUpdate_c[2][2]*NavResult->dC[2][0];
	fTempc[2][1]=fUpdate_c[2][0]*NavResult->dC[0][1]+fUpdate_c[2][1]*NavResult->dC[1][1]+fUpdate_c[2][2]*NavResult->dC[2][1];
	fTempc[2][2]=fUpdate_c[2][0]*NavResult->dC[0][2]+fUpdate_c[2][1]*NavResult->dC[1][2]+fUpdate_c[2][2]*NavResult->dC[2][2];	
	NavResult->dC[0][0]=fTempc[0][0];     NavResult->dC[0][1]=fTempc[0][1];     NavResult->dC[0][2]=fTempc[0][2];
	NavResult->dC[1][0]=fTempc[1][0];     NavResult->dC[1][1]=fTempc[1][1];     NavResult->dC[1][2]=fTempc[1][2];
	NavResult->dC[2][0]=fTempc[2][0];     NavResult->dC[2][1]=fTempc[2][1];     NavResult->dC[2][2]=fTempc[2][2];
}
/***************************************************************/
/* º¯ÊýÃû³Æ£º
    º¯Êý¹¦ÄÜ£ºÖØÁ¦¼ÓËÙ¶È¼ÆËã
    ÊäÈë²ÎÊý£º
    Êä³ö²ÎÊý£º*/
/***************************************************************/	  
void gaccel( NavResultStruct  *NavResult)
{	
	g_dGaccel[0] = -g_dG_wip[2]*NavResult->dV[1];
	g_dGaccel[1] = g_dG_wip[2]*NavResult->dV[0];
	g_dGaccel[2] = g_dG_wip[0]*NavResult->dV[1]-g_dG_wip[1]*NavResult->dV[0];
  	g_dGaccel[2]+= (5.1799e-2*NavResult->dC[2][2]*NavResult->dC[2][2]-NavResult->dPos[2]*9.4114e-7+C_g0_1)*C_NavTime;
}

/***************************************************************/
/* º¯ÊýÃûÆ£
    º¯Êý¦ÄÜ£ºÓÉËÌ¬½ÇÇó½âÎ»ÖÃ
    ÊäÈë²ÎÊý£º
    Êä³öÎÊý£*/
/***************************************************************/	  
void ctofild( NavResultStruct  *NavResult)
{	
	NavResult->dPos[0]=asin(NavResult->dC[2][2]);
	NavResult->dPos[1]=atan(NavResult->dC[2][1]/NavResult->dC[2][0]);
	if(NavResult->dC[2][0]<0.0)
	{
		if(NavResult->dPos[1]<0.0)
			NavResult->dPos[1]=NavResult->dPos[1]+C_PI;
		else
			NavResult->dPos[1]=NavResult->dPos[1]-C_PI;
	}
	NavResult->dAfa=atan(NavResult->dC[0][2]/NavResult->dC[1][2]);
	if(NavResult->dC[1][2]<0.0)
		NavResult->dAfa=NavResult->dAfa+C_PI;
	else
	{
		if(NavResult->dAfa<0.0)
			NavResult->dAfa=NavResult->dAfa+2.0*C_PI;
	}
	NavResult->dAtti[2]=NavResult->dPusa+NavResult->dAfa;
	if(NavResult->dAtti[2]>=2.0*C_PI)
		NavResult->dAtti[2]=NavResult->dAtti[2]-2.0*C_PI;	
}

/***************************************************************/
/* º¯ýÃû³Æ£?
    º¯Êý¹¦ÄÜ£ºËÙ¶ÈµÄÍ¶Ó°
    ÊäÈë²ÎÊý£º
    Êä³ö²ÎÊý£º*/
/***************************************************************/	  
void cvtovi( NavResultStruct  *NavResult) 
{  
	FLOAT64 cs,ss; 
	cs=cos(NavResult->dAfa);
	ss=sin(NavResult->dAfa);
   
	NavResult->dVi[0]=NavResult->dV[0]*cs-NavResult->dV[1]*ss;
	NavResult->dVi[1]=NavResult->dV[0]*ss+NavResult->dV[1]*cs;
	NavResult->dVi[2]=NavResult->dV[2];
}

/***************************************************************/
/* º¯ýÃû³Æ£: cvitov
    º¯Êý¹¦ÄÜ£ºËÙ¶È´ÓÓÎÒÆµ½Ö¸±±
    ÊäÈë²ÎÊý£º
    Êä³ö²ÎÊý£º*/
/***************************************************************/
void cvitov( NavResultStruct  *NavResult)	//¹ßµ¼ËÙ¶È Í¶Ó°
{
	NavResult->dV[0]=NavResult->dVi[1]*sin(NavResult->dAfa)+NavResult->dVi[0]*cos(NavResult->dAfa);
	NavResult->dV[1]=NavResult->dVi[1]*cos(NavResult->dAfa)-NavResult->dVi[0]*sin(NavResult->dAfa);
	NavResult->dV[2]=NavResult->dVi[2];
}

/***************************************************************/
/* º¯ÊýÃû³Æ£º
    º¯Êý¹¦ÄÜ£ºInit C
    ÊäÈë²ÎÊý£º
    Êäö²ÎÊý£º*/
/***************************************************************/	  
void postoc( NavResultStruct  *NavResult)
{
	FLOAT64 	fSlat, fClat, fSlong, fClong, fSafa, fCafa;
	fSlat=sin(NavResult->dPos[0]);   		fClat=cos(NavResult->dPos[0]);
	fSlong=sin(NavResult->dPos[1]);  	fClong=cos(NavResult->dPos[1]);
	fSafa=sin(NavResult->dAfa);      		fCafa=cos(NavResult->dAfa);
	NavResult->dC[0][0]=-fSafa*fSlat*fClong-fCafa*fSlong;
	NavResult->dC[0][1]=-fSafa*fSlat*fSlong+fCafa*fClong;
	NavResult->dC[0][2]= fSafa*fClat;
	NavResult->dC[1][0]=-fCafa*fSlat*fClong+fSafa*fSlong;
	NavResult->dC[1][1]=-fCafa*fSlat*fSlong-fSafa*fClong;
	NavResult->dC[1][2]= fCafa*fClat;
	NavResult->dC[2][0]= fClat*fClong;
	NavResult->dC[2][1]= fClat*fSlong;
	NavResult->dC[2][2]= fSlat;
	g_dWipb[0]=0.0;
	g_dWipb[1]=C_WIE*sqrt(NavResult->dC[0][2]*NavResult->dC[0][2]+NavResult->dC[1][2]*NavResult->dC[1][2])*C_NavTime;
	g_dWipb[2]=C_WIE*NavResult->dC[2][2]*C_NavTime;

}