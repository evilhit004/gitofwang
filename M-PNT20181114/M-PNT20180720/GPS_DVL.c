/************************************************************************
*
* ÎÄ¼þÃû   £º     GPS.c
* °æÈ¨ËµÃ÷ £º   Ê®ÈýÑÐ¾¿Ëù½ÝÁªÏµÍ³ÑÐ¾¿ÊÒ
* °æ±¾ºÅ   £º     1.0
* ´´½¨Ê±¼ä £º   2016.09.10
* ×÷    Õß £º      ½ÝÁªÏµÍ³ÑÐ¾¿ÊÒ
* ¹¦ÄÜÃèÊö £º   GPS½âÂë³ÌÐò
* Ê¹ÓÃ×¢Òâ £º 
* ÐÞ¸Ä¼ÇÂ¼ £º  
*
************************************************************************/

#include "GPS_DVL.h"
#include <math.h>
#include <stdlib.h>
//////////////////////////////////////////////////////////////////////////
/************************ Paras Declamation *****************************/

//GPS_STR         GpsDat,GpsDatRec;

static	unsigned char	ucRecUart_Com;	 
static	unsigned char	gps_data[256];
static	unsigned short	gps_ptr=0,head_cnt_r=0;
static	unsigned char	 ucGps_Hour,ucGps_Min,ucGps_Sec;	
float   		 UTC=0.0;	
float			 fV_earth_Kn=0.0;
//== GPGGA /
unsigned short	  casegga,casevel,crc_cnt_c1,cChk_js;	 
static unsigned char	  ck_a,ck_b,cChkRec;
unsigned char	  c1[32];

////=== BESTPOS
union
{
	float 		    f;
	unsigned char uc[4];
}data_fchar;	

union
{
	double        d;
	unsigned char uc[8];
}data_dchar;

union
{
 	unsigned int    crc_long;
 	unsigned char crc_char[4];
}crc_r; 

union
{
	 int 	intdata;
	 unsigned char chardata[4];
}c2i;
union
{
	 unsigned short ushortdata;
	 unsigned char 	 chardata[2];
}c2us;
union
{
	 unsigned int uintdata;
	 unsigned char 	chardata[4];
}c2ui;	

unsigned short	msg_id=0,lat_rms=0,lon_rms=0,hgt_rms=0;
unsigned short	msg_len_r=0,msg_len=0,crc_err=0;	

///== Judge Gps
static	unsigned short   ucCnt_GpsDelayValid = 0 ;

///== DVL
unsigned char	  ucRecdvl_Com;	 
unsigned char	  dvl_data[100];
unsigned short	  gps_ptr_dvl=0,casegga_dvl,casevel_dvl,crc_cnt_c1_dvl;	 
unsigned short    cChk_js_dvl;
unsigned char	  ck_a_dvl,ck_b_dvl,cChkRec_dvl;
unsigned char	  c1_dvl[32];
unsigned int      	  pos_time;

//////////////////////////////////////////////////////////////////////////
/********************* Subprogram Declamation ***************************/
char ascii2hex(char ch);
void GetDelt_posfv();
unsigned long CRC32Value(int i);
unsigned long CalculateBlockCRC32(unsigned long ulCount,unsigned char *ucBuffer );
void readnull_Com(volatile unsigned short *address_uart);
void Init_GpsCom( volatile unsigned short *address_uart );
void Init_DvlCom(volatile unsigned short *address_uart );

extern unsigned char recbuffer[2048],ReChar[2048];

extern unsigned char g_ucFlg_GpsPpsValid;  // GPS ppsÓÐÐ§±êÖ¾Î»
extern unsigned char g_ucOrder;

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
///=== Initial GPS com
void Init_GpsCom(volatile unsigned short *address_uart )
{
	*(address_uart+1) = 0x3210;	
	readnull_Com( address_uart);
}

void readnull_Com(volatile unsigned short *address_uart)
{
	int i;
	unsigned char ucNum0,ucChar;  
	ucNum0 = (*(address_uart+1)>>8) & 0xFF;
	for(i = 0; i < ucNum0; i++)
	{
		ucChar = *address_uart & 0xFF;	
	}
}



///=== Get GPS Data
//$PASHR,POS,4003.951000,N,11609.425000,E*05
//$PASHR,POS,3954.780000,N,11615.720000,E*00
//$PASHR,VTG,0000.000,0000.000*31
void Read_Gps_PASHR(volatile unsigned short *address_uart, GPS_STR *GpsDat )  	 
{
    unsigned char   temp,Cnt_FIFO_Remain_Com1;
    int i=0;
    double          fGps_l1,g_fGPS_llh;
    const char      *cptr ;
    static unsigned char  wGPS_State=0;
    static unsigned short checktest=0;
    static unsigned char  ucone = 0;
    
    if( ucone == 0 )
    {
        ucone = 1;
        Init_GpsCom(address_uart);

		GpsDat->Pos[2] = 60;
		GpsDat->Course = 0.0;   
		GpsDat->Vearth = 0.0;   
		GpsDat->SVs_used = 0;    
		GpsDat->pdop = 0;    
		GpsDat->Nogps_time = 0;    
	}
    else
    {
        Cnt_FIFO_Remain_Com1 = (*(address_uart+1)& 0xFF00)>>8;  
        while(Cnt_FIFO_Remain_Com1)
        {
            temp=*address_uart;
        		ucRecUart_Com = temp&0xff;
//        		(*address_uart) = ucRecUart_Com;
        		Cnt_FIFO_Remain_Com1 =( *(address_uart+1)& 0xFF00)>>8;  
        
        		switch(wGPS_State)
        		{
        		case 0:
                if(ucRecUart_Com == '$')   
                {
					wGPS_State = 1;
					gps_ptr =0;
					casevel=0;
					casegga =0;
					crc_cnt_c1=0;
					cChkRec = 0;
					cChk_js = 0;
					gps_data[gps_ptr++] = ucRecUart_Com;
                }
                break;
        		case 1:
                gps_data[gps_ptr++] = ucRecUart_Com;
        
                cChk_js ^= ucRecUart_Com;
                if(ucRecUart_Com == 'P')
		{
                    wGPS_State = 2;
		}
		  else 
		  {
			wGPS_State=0;
//			for(i=0;i<512;i++)	 
//				temp=*address_uart;
		  }			
		  break;
        		case 2:
                gps_data[gps_ptr++] = ucRecUart_Com;
        
                cChk_js ^= ucRecUart_Com;
                if(ucRecUart_Com == 'A')
		  {
			    wGPS_State = 3;
		  }
		  else 
		  {
			wGPS_State=0;
//			for(i=0;i<512;i++)	 
//				temp=*address_uart;
		  }
		  break;	
    
        		case 3:
                gps_data[gps_ptr++] = ucRecUart_Com;
        
                cChk_js ^= ucRecUart_Com;
                if(ucRecUart_Com == 'S')
        			  {
        				    wGPS_State = 4;
        			  }
        			  else 
        			  {
        				    wGPS_State=0;
//        				    for(i=0;i<512;i++)	 
//        					     temp=*address_uart;
        			  }
        			  break;	
    
        		case 4:
                gps_data[gps_ptr++] = ucRecUart_Com;
        
                cChk_js ^= ucRecUart_Com;
                if(ucRecUart_Com == 'H')
        			  {
        				    wGPS_State = 5;
        			  }
        			  else 
        			  {
        				    wGPS_State=0;
//        				    for(i=0;i<512;i++)	 
//        					     temp=*address_uart;
        			  }
        			  break;	
    
        		case 5:
                gps_data[gps_ptr++] = ucRecUart_Com;
        
                cChk_js ^= ucRecUart_Com;
                if(ucRecUart_Com == 'R')
        			  {
        				    wGPS_State = 6;
        			  }
        			  else 
        			  {
        				    wGPS_State=0;
//        				    for(i=0;i<512;i++)	 
//        					     temp=*address_uart;
        			  }
        			  break;	
    
        		case 6:
                gps_data[gps_ptr++] = ucRecUart_Com;
        
                cChk_js ^= ucRecUart_Com;
                if(ucRecUart_Com == ',')
        			  {
        				    wGPS_State = 7;
        			  }
        			  else 
        			  {
        				    wGPS_State=0;
//        				    for(i=0;i<512;i++)	 
//        					     temp=*address_uart;
        			  }
        			  break;	
        			      					
        		case 7:
                gps_data[gps_ptr++] = ucRecUart_Com;
        
                cChk_js ^= ucRecUart_Com;
                if(ucRecUart_Com == 'P')
                {
        				    wGPS_State = 14;
                }
                else if(ucRecUart_Com == 'V')
                {
                    wGPS_State = 24;
                }
                else wGPS_State=0;
                break;
        		
        		//== $PASHR,POS	
        		case 14:
        			  gps_data[gps_ptr++] = ucRecUart_Com;
        
        			  cChk_js ^= ucRecUart_Com;
        			  if(ucRecUart_Com == 'O')
        			  {
        				    wGPS_State = 15;
        			  }
        			  else wGPS_State=0;
        			  break;
        			
        		case 15:
        			  gps_data[gps_ptr++] = ucRecUart_Com;
        
        			  cChk_js ^= ucRecUart_Com;
        			  if(ucRecUart_Com == 'S')
        			  {
        				    wGPS_State = 16;
        			  }
        			  else wGPS_State=0;
        			  break;		
        							
        		case 16:
        			  gps_data[gps_ptr++] = ucRecUart_Com;
        			  cChk_js ^= ucRecUart_Com;
        			  if(ucRecUart_Com == ',')  
        			  {
        				    wGPS_State = 17;
        				    crc_cnt_c1 =0;
        				    casevel=0;
        			  }
        			
        			  if(ucRecUart_Com == '*')  
        				    wGPS_State =0;
        			  break;
        			
        		case 17:
        			  gps_data[gps_ptr++] = ucRecUart_Com;
        			  c1[crc_cnt_c1] = ucRecUart_Com;
        			
        			  if(ucRecUart_Com == '*')  
        			  {
        				    wGPS_State = 18;
        				    c1[crc_cnt_c1] = '\0';
        			  }
        			  else
        			  {
        				    cChk_js ^= ucRecUart_Com;
        			  }						
        	
        			  if((c1[crc_cnt_c1]==',')||(c1[crc_cnt_c1]=='\0'))
                {
        			      c1[crc_cnt_c1]='\0';
        				    casegga++;
        			   	  crc_cnt_c1 = 0;
        			             
            				switch(casegga)
            			  {
            				case 1:
            				    cptr = (const char *)(c1);
            					  g_fGPS_llh = atof(cptr);
            					  g_fGPS_llh = g_fGPS_llh * C_HUNREDN;
            					  fGps_l1=(int)(g_fGPS_llh);
            					  g_fGPS_llh = (fGps_l1+(g_fGPS_llh -fGps_l1)*C_D2M)*C_D2R;
								  GpsDat->Pos[0] = g_fGPS_llh;          
            					  break;
            				case 2:
            //					n_s=c1[0];
            					  break;
            				case 3:
            				    cptr = (const char *)(c1);					
            				    g_fGPS_llh =atof(cptr);
            					  g_fGPS_llh =g_fGPS_llh * C_HUNREDN;
            					  fGps_l1=(int)(g_fGPS_llh );
            					  g_fGPS_llh =fGps_l1+(g_fGPS_llh -fGps_l1)*C_D2M;
								  GpsDat->Pos[1]= g_fGPS_llh*C_D2R;            					  
								  break;
            				case 4:
    // 					        e_w=c1[0];
            					  break;
    
            				default:
            					  if(casegga>15)	//·ÀÖ¹Òæ´¦
            					  {
            						    wGPS_State = 0;
            						    gps_ptr =0;
            						    casevel=0;
            						    casegga =0;
            						    crc_cnt_c1=0;
            						    cChkRec = 0;
            						    cChk_js = 0;
            						}
            					  break;
                    }			 
                }
                else   crc_cnt_c1++;	
        			  break;
        			  
        		case 18:
        			  gps_data[gps_ptr++] = ucRecUart_Com;
        			
        			  wGPS_State = 19;
        			  ck_b = ascii2hex(ucRecUart_Com);
        			  break;
        		case 19:
        			  gps_data[gps_ptr++] = ucRecUart_Com;
        
        			  for(i=1;i< gps_ptr-3;i++)
        				    checktest ^= gps_data[i];
        			
        			  ck_a =ascii2hex(ucRecUart_Com);			 
          		  cChkRec = ck_b*16 + ck_a;	 
                if( cChkRec ==  (cChk_js&0x0ff) )   // Òì»òÐ£ÑéÕýÈ·
                {
					if( GpsDat->Pos[0]*C_R2D <=90 && GpsDat->Pos[0]*C_R2D >-90)
					if( GpsDat->Pos[1]*C_R2D <=180 && GpsDat->Pos[1]*C_R2D >-180)
                    GpsDat->Pos_OK = 1;		
                }       
        			  wGPS_State = 0;
        			  checktest = 0;
        			  break;	
        			
        		//== $PASHR,VTG	==========================///
        		case 24:
        			  gps_data[gps_ptr++] = ucRecUart_Com;
        
        			  cChk_js ^= ucRecUart_Com;
        			  if(ucRecUart_Com == 'T')
        			  {
        				    wGPS_State = 25;
        			  }
        			  else wGPS_State=0;
        			  break;
        			
        		case 25:
        			  gps_data[gps_ptr++] = ucRecUart_Com;
        
        			  cChk_js ^= ucRecUart_Com;
        			  if(ucRecUart_Com == 'G')
        			  {
        				    wGPS_State = 26;
        			  }
        			  else wGPS_State=0;
        			  break;						
        		case 26:
        			  gps_data[gps_ptr++] = ucRecUart_Com;
        			  cChk_js ^= ucRecUart_Com;
        			  if(ucRecUart_Com == ',')  
        			  {
        			    	wGPS_State = 27;
        				    crc_cnt_c1 =0;
        				    casevel=0;
        			  } 
        			
        			  if(ucRecUart_Com == '*')  
        				    wGPS_State =0;
        			  break;
        			
        		case 27:
        			  gps_data[gps_ptr++] = ucRecUart_Com;
        			  c1[crc_cnt_c1] = ucRecUart_Com;
        			
        			  if(ucRecUart_Com == '*')  
        			  {
        				    wGPS_State = 28;
        				    c1[crc_cnt_c1] = '\0';
        			  }
        			  else
        			  {
        				    cChk_js ^= ucRecUart_Com;
        			  }						
        	
        			  if((c1[crc_cnt_c1]==',')||(c1[crc_cnt_c1]=='\0'))
        		    {
        			      c1[crc_cnt_c1]='\0';
        				    casegga++;
        			   	  crc_cnt_c1 = 0;
        			             
            				switch(casegga)
            	      {
            				case 1:
                        cptr = (const char *)(c1);
            					  GpsDat->V[0] = atof(cptr);   //Vele
            					  break;
            				case 2:
            					  cptr = (const char *)(c1);
             					  GpsDat->V[1] = atof(cptr);   //Veln  
             					  GpsDat->V[2] = 0.0;
								  GpsDat->Vearth = sqrt(GpsDat->V[0]*GpsDat->V[0]+GpsDat->V[1]*GpsDat->V[1]);
            					  break;
            				default:
            					  if(casegga>5)	//·ÀÖ¹Òæ´¦
            					  {
                						wGPS_State = 0;
                						gps_ptr =0;
                						casevel=0;
                						casegga =0;
                						crc_cnt_c1=0;
                						cChkRec = 0;
                						cChk_js = 0;
            						}
            					  break;
                    }			 
                }
        			  else   crc_cnt_c1++;	
        			  break;
        		case 28:
        			  gps_data[gps_ptr++] = ucRecUart_Com;
        			
        			  wGPS_State = 29;
        			  ck_b =ascii2hex(ucRecUart_Com);
        			  break;
        		case 29:
        			  gps_data[gps_ptr++] = ucRecUart_Com;
        			  ck_a =ascii2hex(ucRecUart_Com);			 
          			cChkRec = ck_b*16 + ck_a;	 
                if( cChkRec == (cChk_js&0x0ff) )   // Òì»òÐ£ÑéÕýÈ·
                {
					GpsDat->Vel_OK = 1;		
                }       
        			  wGPS_State = 0;
        			  break;	
        		}
        }		
    }
}


  	
///=== Get GPS Data


// Get GPS Data
void Read_Gps_BESTB(volatile unsigned short *address_uart, GPS_STR *GpsDat )   
{
	unsigned char temp,Cnt_FIFO_Remain_Com1,i;
	static unsigned char wGPS_State=0;
		
	Cnt_FIFO_Remain_Com1 = (*(address_uart+2)& 0xFF00)>>8;   
	while(Cnt_FIFO_Remain_Com1)
	{
		temp=*address_uart;	
		ucRecUart_Com = temp&0xff;
		Cnt_FIFO_Remain_Com1 = (*(address_uart+2)& 0xFF00)>>8;   

		switch(wGPS_State)
		{
			case 0:
				if(0xAA == ucRecUart_Com)
				{
					wGPS_State = 1;
					gps_data[gps_ptr++] = ucRecUart_Com;
					gps_data[0] = 0xAA;
				}				
			    break;
			case 1:
				if(0x44 == ucRecUart_Com)
				{
				        wGPS_State = 2;
				        gps_data[1] = 0x44;
				}
				else 
					wGPS_State = 0 ;
				break;
			case 2:
				if(0x12 == ucRecUart_Com)
				{
				    wGPS_State = 3;
				    gps_data[2] = 0x12;
				}
				else 
					wGPS_State = 0 ;
				break;
			case 3:
				  gps_data[3] = ucRecUart_Com;//Ö¡Í·×Ö½Ú³¤
				  gps_ptr = 4;
				  head_cnt_r = gps_data[3];
				  wGPS_State = 4;
				  break;
			case 4:
				  gps_data[gps_ptr++] = ucRecUart_Com;
				  if(gps_ptr == head_cnt_r)//ÊÕÖ¡Í·×Ö½Ú³¤¶È
				  {
					    msg_len_r = (int)gps_data[8] + (int)gps_data[9]*256;
					    msg_len = 0;
					    wGPS_State = 5;
				  }
				  break;
			case 5:
				  gps_data[gps_ptr++] = ucRecUart_Com;	// ÊÕÊý¾Ý³¤¶È
				  msg_len++;
				  if(msg_len == msg_len_r)
				  {
					    wGPS_State = 6;
					    msg_len = 0;
				  }
				  break;
			case 6:
				crc_r.crc_char[msg_len++] = ucRecUart_Com;//Ð£ÑéºÍ
				  if(4 == msg_len)
				  {
					msg_len=0;
					    if( crc_r.crc_long == CalculateBlockCRC32(gps_ptr, gps_data) )
					    {
					        gps_ptr=0;
						      msg_id = (unsigned short)gps_data[4] + (unsigned short)(gps_data[5]*256);//Êý¾ÝID
						      switch(msg_id)
						      {
						      case 0x63:	//BESTVEL
							        for(i = 0; i < 4; i++)
									{
										  data_fchar.uc[i] = gps_data[44+i];
									}
									GpsDat->Vearth = data_fchar.f;
									
									for(i = 0; i < 4; i++)
									{
										  data_fchar.uc[i] = gps_data[48+i];
									}
									GpsDat->Course = data_fchar.f;	
									
									GpsDat->V[0] = GpsDat->Vearth * sin(GpsDat->Course*C_D2R);
									GpsDat->V[1] = GpsDat->Vearth * cos(GpsDat->Course*C_D2R);		
																
									for(i = 0; i < 4; i++)
									{
										  data_fchar.uc[i] = gps_data[52+i];
									}
									GpsDat->V[2] = data_fchar.f;									
														
			//				        TimeStatus = gps_data[13];
			//				        GpsWeek = gps_data[14] + gps_data[15]*256;
			//					    Gpsec = (int)gps_data[16] + (int)gps_data[17]*256+ (int)gps_data[18]*256*256+ (int)gps_data[19]*256*256*256;
			//				        pos_time = Gpsec;//ms
									GpsDat->Vel_OK = 1;
						
									break;
							
			        case 0x2A:	//BESTPOS					
									for( i = 0; i < 8; i++)
									{
										data_dchar.uc[i] = gps_data[36+i];
									}
									GpsDat->Pos[0]  = data_dchar.d;
									for(i = 0; i < 8; i++)
									{
										data_dchar.uc[i] = gps_data[44+i];
									}
									GpsDat->Pos[1]  = data_dchar.d;
									for(i = 0; i < 8; i++)
									{
										data_dchar.uc[i] = gps_data[52+i];
									}
									GpsDat->Pos[2]  = data_dchar.d;
			//						Sv = gps_data[92];
									GpsDat->SVs_used = gps_data[93];
			
									
									for(i=0;i<4;i++)
									{
										data_fchar.uc[i] =  gps_data[68+i];
									}
									lat_rms = data_fchar.f;
									
									for(i=0;i<4;i++)
									{
										data_fchar.uc[i] =  gps_data[72+i];
									}
									lon_rms = data_fchar.f;
									
									for(i=0;i<4;i++)
									{
										data_fchar.uc[i] =  gps_data[76+i];
									}
									hgt_rms = data_fchar.f;
									
									GpsDat->pdop = sqrt(lat_rms*lat_rms+lon_rms*lon_rms);
									//pdop_get = pdop;
									GpsDat->Pos_OK = 1;				
									break;
			        }
					    }
					    else
					    {
						    crc_err++;					
					    }
					    wGPS_State = 0;
				}
				  break;
			default:
				  wGPS_State=0;
			 	  break;	 
		}
	}	
}

////GPS½ÓÊÕÖÐ¶Ï ¶þ½øÖÆÂë  ÓÃÓÚÎÞÈË»ú¹ßµ¼Ä£¿éµÄ½âÂë
////GPS½ÓÊÕÖÐ¶Ï ¶þ½øÖÆÂë  ÓÃÓÚÎÞÈË»ú¹ßµ¼Ä£¿éµÄ½âÂë
void Read_Gps_WRJ(unsigned int address_uart, GPS_STR *GpsDat)
{

	static unsigned char     		 SVs_used_before=0;
	static unsigned short		g_wGPS_State=0;
	static float		g_fGPS_V_before[3]={0.0,0.0,0.0};
	float g_fGPS_deltV[3]={0.0,0.0,0.0};
	static float		g_fGPS_P_before[2]={0.0,0.0};
	float g_fGPS_deltP[2]={0.0,0.0};

	unsigned char	i=0,j=0;

	unsigned short * GPS_FIFO_REG = 0;			
	GPS_FIFO_REG = ((unsigned short *)(address_uart + 0xA));

while((*GPS_FIFO_REG & 0x01) == 0x01)  //¶Á¿ÕFIFO
{	
   // ucRecUart_Com = UART_Recv(UART1_Base);
   ucRecUart_Com = *((unsigned short *)address_uart);
//	float g_uisAcc=0;
	if(g_wGPS_State == 0)
	{
			ck_a = 0;
			ck_b = 0;
			gps_ptr = 0;
	}
	if((g_wGPS_State > 1) && (g_wGPS_State < 7))          //Ð£ÑéºÍ
	{
			ck_a = ck_a + ucRecUart_Com;
			ck_b = ck_b + ck_a;
	}
	switch(g_wGPS_State)
	{
		case 0:
			if(ucRecUart_Com == 0xB5)   
				g_wGPS_State = 1;
			break;
			
		case 1:
			if(ucRecUart_Com == 0x62)  
				g_wGPS_State = 2;
			else                
				g_wGPS_State = 0;
			break;
			
		case 2:
			if(ucRecUart_Com == 0x01)  
				g_wGPS_State = 3;
			else                
				g_wGPS_State = 0;
			break;
		case 3:
			if(ucRecUart_Com == 0x07)  
				g_wGPS_State = 4;
			else                
				g_wGPS_State = 0;
			break;

		case 4:
			if(ucRecUart_Com == 0x5c)  
				g_wGPS_State = 5;
			else                
				g_wGPS_State = 0;
			break;
		case 5:
			if(ucRecUart_Com == 0x00)  
				g_wGPS_State = 6;
			else                
				g_wGPS_State = 0;
			break;
				
		case 6:
			gps_data[gps_ptr++] = ucRecUart_Com;
			if(gps_ptr >= 92)     
				g_wGPS_State = 7;

			break;
		case 7:
			if(ck_a == ucRecUart_Com)   
				g_wGPS_State = 8;
			else                   
				g_wGPS_State = 0;
			break;
		case 8:
			if(ck_b == ucRecUart_Com)   
			{
				g_wGPS_State = 0;					
				for(j = 0; j < 4;j++)
        {
         	c2ui.chardata[j] = gps_data[j];
        }
//		g_uciTow = c2ui.uintdata;
		//ucGps_Hour = fmod(gps_data[8],24)+8;
		ucGps_Hour = gps_data[8]+8;
		if(ucGps_Hour>=24)
		{
		  ucGps_Hour=ucGps_Hour-24;
		}
		ucGps_Min = gps_data[9];						
		ucGps_Sec = gps_data[10];
		
		c2us.chardata[0] = gps_data[76];
		c2us.chardata[1] = gps_data[77];
		GpsDat->pdop = c2us.ushortdata*0.01;
		GpsDat->SVs_used = (int)gps_data[23];	

      	for(j = 0; j < 4;j++)
        {
        		c2i.chardata[j] = gps_data[24+j];
        }
        GpsDat->Pos[1] = ((double)c2i.intdata)*1e-7;///longitude:180*PI;
        for(j = 0; j < 4;j++)
        {
	         c2i.chardata[j] = gps_data[28+j];
        }
      		GpsDat->Pos[0] = ((double)c2i.intdata)*1e-7;///latitude: 180*PI;
        for(j = 0; j < 4;j++)
       	{
  //      	c2i.chardata[j] = gps_data[14+j];  //ÍÖÇòÌå¸ß¶È 
        		c2i.chardata[j] = gps_data[36+j];  //º£°Î¸ß¶È
        }
		GpsDat->Pos[2] = ((double)c2i.intdata)*1e-3;	
								
        for(j = 0; j < 4;j++)
        {
       	 c2i.chardata[j] = gps_data[48+j];
        }
				GpsDat->V[1] = ((float)c2i.intdata)*1e-3;//±±ÏòËÙ¶Èmm/s
				//  Geog_vy = gps_v[1];
				     
        for(j = 0; j < 4;j++)
       {
	       c2i.chardata[j] = gps_data[52+j];
       }
       GpsDat->V[0] = ((float)c2i.intdata)*1e-3;//¶«ËÙËÙ¶Èmm/s
       
       for(j = 0; j < 4;j++)
       {
	       c2i.chardata[j] = gps_data[56+j];
       }
				GpsDat->V[2] = -((float)c2i.intdata)*1e-3;//µØÏòËÙ¶È mm/s

       for(j = 0; j < 4;j++)
       {
	       c2i.chardata[j] = gps_data[60+j];
       }
       GpsDat->Vearth = ((float)c2i.intdata)*1e-3;//µØËÙ mm/s
			      // Geog_vz = gps_v[2];
		for(j = 0; j < 4;j++)
       {
	       c2ui.chardata[j] = gps_data[68+j];
       }
		GpsDat->g_uisAcc = ((float)c2ui.uintdata)*0.001;		//ËÙ¶È¾«¶È  mm/s
		if(g_fGPS_V_before[0]>1e-7/*&&g_fGPS_V_before[1]>1e-7&&g_fGPS_V_before[1]>1e-7*/)
			{
				for(j=0;j<3;j++)
				{
					g_fGPS_deltV[j] = g_fGPS_V_before[j] - GpsDat->V[j];
					g_fGPS_V_before[j] = GpsDat->V[j];
		    }
		  }
		if(g_fGPS_P_before[0]>1e-7/*&&g_fGPS_P_before[1]>1e-7*/)
			{
				for(j=0;j<2;j++)
		    {
		    	g_fGPS_deltP[j] = g_fGPS_P_before[j] - GpsDat->Pos[j];
			    g_fGPS_P_before[j] = GpsDat->Pos[j];
		   }
			}
		if(SVs_used_before>0)
			{
				 if((GpsDat->SVs_used-SVs_used_before)>=-2&&fabs(g_fGPS_deltP[0])<0.1&&fabs(g_fGPS_deltP[1])<0.1)	
      	{
      		GpsDat->Pos_OK = 1;	
      	}
      	else
		  	{
		  		GpsDat->Pos_OK = 0;	
		  	}
		  }


    SVs_used_before = GpsDat->SVs_used;	
		
		if(fabs(g_fGPS_deltV[0])<C_GPS_V_delt && fabs(g_fGPS_deltV[1])<C_GPS_V_delt && GpsDat->g_uisAcc<1.0)
		{
			GpsDat->Vel_OK = 1;
//			g_fGps_course=GPS_Course(GpsDat.V[0],GpsDat.V[1]);
		}
		else
		{
			GpsDat->Vel_OK = 0;
			GpsDat->Vearth =0;
		}					
			
		for(j = 0; j < 4;j++)
		{
			c2i.chardata[j] = gps_data[84+j];  //091217
		}
		GpsDat->Course = ((double)c2i.intdata)*1e-5;//º½¼£½Ç
		for(j = 0; j < 4;j++)
		{
			c2ui.chardata[j] = gps_data[72+j];
		}			       
//		g_fGpsHeadAcc =  ((float)c2i.intdata)*1e-5;	//º½¼£½Ç½âËã¾«
		g_wGPS_State = 0;
		}
		else                   
			g_wGPS_State = 0;
		break;
	}	
}
}



unsigned long CRC32Value(int i)
{
	int j;
	unsigned long ulCRC;
	ulCRC = i;
	for ( j = 8 ; j > 0; j-- )
	{
		if ( ulCRC & 1 )
			ulCRC = ( ulCRC >> 1 ) ^ CRC32_POLYNOMIAL;
		else
		ulCRC >>= 1;
	}
	return ulCRC;
}


unsigned long CalculateBlockCRC32(
unsigned long ulCount,     /* Number of bytes in the data block */
unsigned char *ucBuffer ) /* Data block */
{
	unsigned int ulTemp1;
	unsigned int ulTemp2;
	unsigned int ulCRC = 0;
	while ( ulCount-- != 0 )
	{
		ulTemp1 = ( ulCRC >> 8 ) & 0x00FFFFFFL;
		ulTemp2 = CRC32Value( ((int) ulCRC ^ *ucBuffer++ ) & 0xff );
		ulCRC = ulTemp1 ^ ulTemp2;
	}
	return( ulCRC );
}

	



///=== Initial GPS com
void Init_DvlCom(volatile unsigned short *address_uart )
{
	*(address_uart+1) = 0x3210;	
	readnull_Com( address_uart);
}


///=== Get DVL Data
//$VMWCV,01.00,N*38
unsigned char Read_DVL( volatile unsigned short *address_uart, double *dDVLVelo )  	 
{
    unsigned char   temp,Cnt_FIFO_Remain_DVL,DVLVelo_ok=0;
    char            n_s,i;
    const char      *cptr_dvl ;
    static unsigned char wGPS_State_dvl=0;
    static unsigned short checktest_dvl=0;
	float ftemp;
    static unsigned char  ucone = 0;
    
    if( ucone == 0 )
    {
        ucone = 1;
        Init_DvlCom(address_uart);

		*dDVLVelo = 0;    
	}
    else
    {
        Cnt_FIFO_Remain_DVL = (*(address_uart+1)& 0xFF00)>>8;    
        while(Cnt_FIFO_Remain_DVL)
        {
            temp=*address_uart;
        		ucRecdvl_Com = temp&0xff;
//        		(*address_uart) = ucRecdvl_Com;
        		Cnt_FIFO_Remain_DVL = (*(address_uart+1)& 0xFF00)>>8;  
 
        
        		switch(wGPS_State_dvl)
        		{
        		case 0:
                if(ucRecdvl_Com == '$')   
                {
            				wGPS_State_dvl = 1;
            				gps_ptr_dvl =0;
            				casevel_dvl=0;
            				casegga_dvl =0;
            				crc_cnt_c1_dvl=0;
            				cChkRec_dvl = 0;
            				cChk_js_dvl = 0;
            				dvl_data[gps_ptr_dvl++] = ucRecdvl_Com;
                }
                break;
        		case 1:
                dvl_data[gps_ptr_dvl++] = ucRecdvl_Com;
        
                cChk_js_dvl ^= ucRecdvl_Com;
                if(ucRecdvl_Com == 'V')
        			  {
                    wGPS_State_dvl = 2;
        			  }
        			  else 
        			  {
        				    wGPS_State_dvl=0;
        				    for(i=0;i<512;i++)	 
        					     //temp=cs554;
        					     temp=*address_uart;
        			  }			
        			  break;
        		case 2:
                dvl_data[gps_ptr_dvl++] = ucRecdvl_Com;
        
                cChk_js_dvl ^= ucRecdvl_Com;
                if(ucRecdvl_Com == 'M')
        			  {
        				    wGPS_State_dvl = 3;
        			  }
        			  else 
        			  {
        				    wGPS_State_dvl=0;
        				    for(i=0;i<512;i++)	 
        					     //temp=cs554;
        					     temp=*address_uart;
        			  }
        			  break;	
    
        		case 3:
                dvl_data[gps_ptr_dvl++] = ucRecdvl_Com;
        
                cChk_js_dvl ^= ucRecdvl_Com;
                if(ucRecdvl_Com == 'W')
        			  {
        				    wGPS_State_dvl = 4;
        			  }
        			  else 
        			  {
        				    wGPS_State_dvl=0;
        				    for(i=0;i<512;i++)	 
        					     //temp=cs554;
        					     temp=*address_uart;
        			  }
        			  break;	
    
        		case 4:
                dvl_data[gps_ptr_dvl++] = ucRecdvl_Com;
        
                cChk_js_dvl ^= ucRecdvl_Com;
                if(ucRecdvl_Com == 'C')
        			  {
        				    wGPS_State_dvl = 5;
        			  }
        			  else 
        			  {
        				    wGPS_State_dvl=0;
        				    for(i=0;i<512;i++)	 
        					     //temp=cs554;
        					     temp=*address_uart;
        			  }
        			  break;	
    
        		case 5:
                dvl_data[gps_ptr_dvl++] = ucRecdvl_Com;
        
                cChk_js_dvl ^= ucRecdvl_Com;
                if(ucRecdvl_Com == 'V')
        			  {
        				    wGPS_State_dvl = 6;
        			  }
        			  else 
        			  {
        				    wGPS_State_dvl=0;
        				    for(i=0;i<512;i++)	 
        					     //temp=cs554;
        					     temp=*address_uart;
        			  }
        			  break;	
       							
        		case 6:
        			  dvl_data[gps_ptr_dvl++] = ucRecdvl_Com;
        			  cChk_js_dvl ^= ucRecdvl_Com;
        			  if(ucRecdvl_Com == ',')  
        			  {
        				    wGPS_State_dvl = 7;
        				    crc_cnt_c1_dvl =0;
        				    casevel_dvl=0;
        			  }
        			
        			  if(ucRecdvl_Com == '*')  
        				    wGPS_State_dvl =0;
        			  break;
        			
        		case 7:
        			  dvl_data[gps_ptr_dvl++] = ucRecdvl_Com;
        			  c1_dvl[crc_cnt_c1_dvl] = ucRecdvl_Com;
        			
        			  if(ucRecdvl_Com == '*')  
        			  {
        				    wGPS_State_dvl = 8;
        				    c1_dvl[crc_cnt_c1_dvl] = '\0';
        			  }
        			  else
        			  {
        				    cChk_js_dvl ^= ucRecdvl_Com;
        			  }						
        	
        			  if((c1_dvl[crc_cnt_c1_dvl]==',')||(c1_dvl[crc_cnt_c1_dvl]=='\0'))
                {
        			      c1_dvl[crc_cnt_c1_dvl]='\0';
        				    casegga_dvl++;
        			   	  crc_cnt_c1_dvl = 0;
        			             
            				switch(casegga_dvl)
            			  {
            				case 1:
            				    cptr_dvl = (const char *)(c1_dvl);
    							      ftemp= atof(cptr_dvl);
            					  *dDVLVelo = ftemp;
            					  break;
            				case 3:
            					  n_s = c1_dvl[0];
            					  if( n_s == 'N')
            					    *dDVLVelo = -*dDVLVelo;
            					  break;
     
            				default:
            					  if(casegga_dvl>5)	 
            					  {
            						    wGPS_State_dvl = 0;
            						    gps_ptr_dvl =0;
            						    casevel_dvl=0;
            						    casegga_dvl =0;
            						    crc_cnt_c1_dvl=0;
            						    cChkRec_dvl = 0;
            						    cChk_js_dvl = 0;
            						}
            					  break;
                    }			 
                }
                else   crc_cnt_c1_dvl++;	
        			  break;
        			  
        		case 8:
        			  dvl_data[gps_ptr_dvl++] = ucRecdvl_Com;
        			 
        			  wGPS_State_dvl = 9;
        			  ck_b_dvl = ascii2hex(ucRecdvl_Com);
        			  break;
        		case 9:
        			  dvl_data[gps_ptr_dvl++] = ucRecdvl_Com;
        
        			  for(i=1;i< gps_ptr_dvl-3;i++)
        				    checktest_dvl ^= dvl_data[i];
        			
        			  ck_a_dvl =ascii2hex(ucRecdvl_Com);			 
          		  cChkRec_dvl = ck_b_dvl*16 + ck_a_dvl;	 
                if( cChkRec_dvl ==  (cChk_js_dvl&0x0ff) )   
                {
                    DVLVelo_ok = 1;		
                }       
        			  wGPS_State_dvl = 0;
        			  checktest_dvl = 0;
        			  break;	    		
        		}
        }
    }
    return(DVLVelo_ok);		
}


char ascii2hex(char ch)
{
    if(ch>='0'&&ch<='9')
		    return(ch-'0');
    else if(ch>='a'&&ch<='f')
		    return(ch-'a'+10);
    else if(ch>='A'&&ch<='F')
		    return(ch-'A'+10);
    else 
		    return(0);
}

void gvmov( NavResultStruct  *NavResult,GPS_STR *GpsDat)	// GPSËÙ¶ÈÐÞÕý¹ßµ¼ËÙ¶È
{
	NavResult->dV[0]=GpsDat->V[1]*sin(NavResult->dAfa)+GpsDat->V[0]*cos(NavResult->dAfa);
	NavResult->dV[1]=GpsDat->V[1]*cos(NavResult->dAfa)-GpsDat->V[0]*sin(NavResult->dAfa);
	NavResult->dV[2]=GpsDat->V[2];
}

void Gps_pos_modify(NavResultStruct  *NavResult,GPS_STR *GpsDat)	// GPSÎ»ÖÃÐÞÕý¹ßµ¼Î»ÖÃ
{
	double f_gfai[2];
	static	double g_dLastrangerr[3] = {0.0,0.0,0.0};
	if((GpsDat->Pos[2]<32768.0)&&(GpsDat->Pos[2]>-512.0))
	{	
		g_dLastrangerr[2]=g_dLastrangerr[2]*0.9+(GpsDat->Pos[2]-NavResult->dPos[2])*0.1;
		NavResult->dPos[2]=NavResult->dPos[2]+g_dLastrangerr[2]*0.03;
	}
	if(fabs(GpsDat->Pos[0])<= 75.0)
//	if(fabs(GpsDat->Pos[0])<= 90.0)
	{	
		g_dLastrangerr[0]=g_dLastrangerr[0]*0.9+(GpsDat->Pos[0]*C_D2R-NavResult->dPos[0])*0.1;
		f_gfai[0]=NavResult->dPos[0]+g_dLastrangerr[0]*0.03;
		if(fabs(GpsDat->Pos[1]) <= 180.0) 
		{	
			g_dLastrangerr[1]=g_dLastrangerr[1]*0.9+(GpsDat->Pos[1]*C_D2R-NavResult->dPos[1])*0.1;
			f_gfai[1]=NavResult->dPos[1]+g_dLastrangerr[1]*0.03;
			NavResult->dC[0][0]=-sin(NavResult->dAfa)*sin(f_gfai[0])*cos(f_gfai[1])-cos(NavResult->dAfa)*sin(f_gfai[1]);
			NavResult->dC[0][1]=-sin(NavResult->dAfa)*sin(f_gfai[0])*sin(f_gfai[1])+cos(NavResult->dAfa)*cos(f_gfai[1]);
			NavResult->dC[0][2]= sin(NavResult->dAfa)*cos(f_gfai[0]);
			NavResult->dC[1][0]=-cos(NavResult->dAfa)*sin(f_gfai[0])*cos(f_gfai[1])+sin(NavResult->dAfa)*sin(f_gfai[1]);
			NavResult->dC[1][1]=-cos(NavResult->dAfa)*sin(f_gfai[0])*sin(f_gfai[1])-sin(NavResult->dAfa)*cos(f_gfai[1]);
			NavResult->dC[1][2]= cos(NavResult->dAfa)*cos(f_gfai[0]);
			NavResult->dC[2][0]= cos(f_gfai[0])*cos(f_gfai[1]);
			NavResult->dC[2][1]= cos(f_gfai[0])*sin(f_gfai[1]);
			NavResult->dC[2][2]= sin(f_gfai[0]);
		}
	}
}

/*========================================================
 * ÀûÓÃGPSÎ÷±±ÏòËÙ¶ÈÇó³öË³Ê±ÕëÎªÕýµÄº½¼£½ÇÒªÇó£¬µ¼º½½âËãÊ±ÐèÒª·´Ïò
 *========================================================*/
double  GPS_Course(GPS_STR *GpsDat)
{
	double fAngle,fVelw;
	char   flag = 0;
	double  fGps_Vg=0;
	double	fGps_Yaw=0.0;
	
	fVelw = -GpsDat->V[0];   // Î÷ÏòËÙ?	
	fGps_Vg = sqrt( GpsDat->V[0]*GpsDat->V[0] + GpsDat->V[1]*GpsDat->V[1]);   // µØËÙ
	
	if( fGps_Vg<1.0)
		fGps_Yaw = 0.0;
	else    // ¸ù¾ÝÎ÷ò/±±ÏòËÙ¶È£¬ÅÐ¶ÏÕýÇÐÇtan(Vn/Vw)µÄ´óÐ¡£¨´Ë½Ç¾ùÈ¡ÕæÖµ£©
	{
		if( fabs( fabs(GpsDat->V[1]) - fabs(fVelw) ) < 0.001 )
		{
			if( fabs(fVelw < 0.001) )
				fGps_Yaw = 0.0;
			else 
			{
				fAngle = C_PI/4.0;
				flag =1 ;
			}
		}
		else
		{
			if( fabs(fVelw) > fabs(GpsDat->V[1]))
			{
				fAngle = atan( fabs(GpsDat->V[1]) / fabs(fVelw) );
				flag =1;	
			}	
			else
			{
				fAngle = C_PI/2.0 - atan( fabs(fVelw)/fabs(GpsDat->V[1]) );
				flag =1;
			}
		}
	}
		
	if( flag ==1 )   // º½¼£½ÇfGps_YawÎª´ÓµØËÙ¿ªÊ¼ÄæÊ±Õëµ½Õæ±±µÄ¼Ð½Ç
	{
		if ( fVelw >= 0.0)
		{
			if( GpsDat->V[1] >= 0.0)        	// µØËÙÔÚµÚ¶þÏóÏÞ
				fGps_Yaw = C_PI*3.0/2.0 + fAngle;
			else 							// µØËÙÔÚµÚÈýÏóÏÞ
				fGps_Yaw =C_PI*3.0/2.0 - fAngle;
		}
		else
		{
			if( GpsDat->V[1] >=0.0)			// µØËÙÔÚµÚÒ»ÏóÏÞ
				fGps_Yaw = C_PI/2.0 - fAngle;
			else 							// µØËÙÚµÚËÄÏóÏ?
				fGps_Yaw = C_PI/2.0 +fAngle;
		}
	}
	GpsDat->Course = fGps_Yaw*C_R2D;
	return(fGps_Yaw*C_R2D);
}


/*========================================================
 * ¶ÁÈ¡GPSÊý¾Ýú»
 * 
 * 
 *  CECF->¾­Î³È¶
 *========================================================*/
void XYZ_to_BLH(double X, double Y, double Z, double* B, double* L, double* H)
{
    double p,n,thet,esq,epsq,temp;
	double a, b, f, a2,b2,slat,clat,sth,sth3,cth,cth3; // Save some float math.
	 
	if(fabs(X)<1e-6 && fabs(Y)<1e-6 && fabs(Z)<1e-6)
	{
		*B = 0;
		*L = 0;
		*H = 0;
		return;
	}
	a=WGS84_A;
	f=WGS84_F;

	b = a * (1-f);
	a2 = a * a;
	b2 = b * b;

	p = sqrt( X * X + Y * Y);
	thet = atan(Z * a / ( p * b));
	esq = 1.0 - b2 / a2;
	epsq = a2 / b2 - 1.0;

	sth = sin( thet);
	cth = cos( thet);

	sth3 = sth * sth * sth;
	cth3 = cth * cth * cth;

	temp=p - esq * a * cth3;
	if (temp<0)  
	{  
		*B = atan2(-Z - epsq * b * sth3,-temp)*57.29577951308232;
	}
	else
	{
		*B = atan2(Z + epsq * b * sth3,temp)*57.29577951308232;
	}

	*L = atan2( Y, X)*57.29577951308232;

	clat = cos(*B/57.29577951308232);
	slat = sin(*B/57.29577951308232);
	n = a2/sqrt(a2 * clat * clat + b2 * slat * slat);
	*H = p / clat - n;
}
/*========================================================
 * GPSÊý¾Ý ú»
 *
 * cecf ËÙ¶È ×ª eun
 * 
 *========================================================*/

void Speed_XYZ_to_BLH(double B, double L, double H, double speedX, double speedY, double speedZ,
						  double* speedB, double* speedL, double* speedH)
{
	  double sinL,cosL,sinB,cosB;
	  double rad=1.0;//DEGREE_TO_RAD;
	  B=B/57.29577951308232;
	  L=L/57.29577951308232;
	  sinB=sin(B*rad);	  
	  cosB=cos(B*rad);	  
	  sinL=sin(L*rad);	  
	  cosL=cos(L*rad);	  
 	  *speedB = -cosL*sinB*speedX-sinL*sinB*speedY+cosB*speedZ;
	  *speedL = -sinL*speedX+cosL*speedY;
	  *speedH =  cosL*cosB*speedX+sinL*cosB*speedY+sinB*speedZ;
}

/*========================================================
 * ¶ÁÈ¡GPSÊý¾Ý NOVTAL½ÓÊÕú»
 * ÓÃÓÚGNC
 * GPGGAÓï¾ä
 * BESTXYZÓï¾ä
 *========================================================*/
void ReadGnssData(volatile unsigned short *address_uart,GPS_STR *GpsDat)
{
  
    int    iTemp_ground=0;
	unsigned char  ucRecUart_Com;
	float  fGps_l1,g_fGPS_llh;
	float  ftemp;
	const char      *cptr ;
    static unsigned char  wGPS_State=0;
    static unsigned short checktest=0;
	static double  cecfpostion[3];
	static double  cecfposstd[3];
	static double  cecfvel[3];
	static double  cecfvelstd[3];
	unsigned char  i=0,j=0;
	unsigned char  temp;
	unsigned int iFlagtemp_ground,iNum0_fifoground;
    //
    static unsigned char        SVs_used_before=0;
	static unsigned short 		g_wGPS_State=0;
	static float		g_fGPS_V_before[3]={0.0,0.0,0.0};
	float             g_fGPS_deltV[3]={0.0,0.0,0.0};
	static float		g_fGPS_P_before[2]={0.0,0.0};
	float             g_fGPS_deltP[2]={0.0,0.0};
   //
 
	//iNum0_fifoground=check_fifo_com(address_gps_ComSet);
	iFlagtemp_ground=0;	
		
	for(iTemp_ground = 0; iTemp_ground < iNum0_fifoground; iTemp_ground++)
	{		
	//	ucRecUart_Com = *address_gps_Com & 0xFF;		
	//	*cs554_ground=ucRecUart_Com;		 
    	switch(wGPS_State)
    	{
			case 0:
			if(ucRecUart_Com == '$')                        // GPGGAÓï¾ä
			{
				wGPS_State = 1;
				gps_ptr =0;
				casevel=0;
				casegga =0;
				crc_cnt_c1=0;
				cChkRec = 0;
				cChk_js = 0;
				gps_data[gps_ptr++] = ucRecUart_Com;
			}
			else if(ucRecUart_Com==0xAA)                    // BESTXYZÓï¾ä
			{
			    wGPS_State = 26;
				gps_ptr =0;
				casevel=0;
				casegga =0;
				crc_cnt_c1=0;
				cChkRec = 0;
				cChk_js = 0;
				gps_data[gps_ptr++] = ucRecUart_Com;
			}
           	break;
    		case 1:
    			gps_data[gps_ptr++] = ucRecUart_Com;
    			cChk_js ^= ucRecUart_Com;
				if(ucRecUart_Com == 'G')
				{
					wGPS_State = 2;
				}
				else 
				{
					wGPS_State=0;
				}			
				break;
			case 2:
				gps_data[gps_ptr++] = ucRecUart_Com;
			
				cChk_js ^= ucRecUart_Com;
				if(ucRecUart_Com == 'P')
				{
					wGPS_State = 3;
				}
				else 
				{
					wGPS_State=0;
				}
				break;	
    					
			case 3:
				gps_data[gps_ptr++] = ucRecUart_Com;
			
				cChk_js ^= ucRecUart_Com;
				if(ucRecUart_Com == 'G')
				{
					wGPS_State = 4;
				}
				else if(ucRecUart_Com=='H')
				{
				    wGPS_State = 35;
				}
				else
				{ 
					wGPS_State=0;
				}
			break;
    			
	    	case 4:
    			  gps_data[gps_ptr++] = ucRecUart_Com;
    
    			  cChk_js ^= ucRecUart_Com;
    			  if(ucRecUart_Com == 'G')
    			  {
    				    wGPS_State = 5;
    			  }
    			  else wGPS_State=0;
    		break;
	    	case 5:
    			  gps_data[gps_ptr++] = ucRecUart_Com;
    
    			  cChk_js ^= ucRecUart_Com;
    			  if(ucRecUart_Com == 'A')
    			  {
    				    wGPS_State = 6;
    			  }
    			  else wGPS_State=0;
    		break;						
	    	case 6:
    			  gps_data[gps_ptr++] = ucRecUart_Com;
    			  cChk_js ^= ucRecUart_Com;
    			  if(ucRecUart_Com == ',')  
    			  {
    				    wGPS_State = 7;
    				    crc_cnt_c1 =0;
    				    casevel=0;
    			  }
    			  if(ucRecUart_Com == '*')  
    				    wGPS_State =0;
	    	  break;
	    	  case 7:
	    		  gps_data[gps_ptr++] = ucRecUart_Com;
	    		  c1[crc_cnt_c1] = ucRecUart_Com;
	    		  if(ucRecUart_Com == '*')  
	    		  {
	    			  wGPS_State = 8;
	    			  c1[crc_cnt_c1] = '\0';
	    		  }
	    		  else
	    		  {
	    			  cChk_js ^= ucRecUart_Com;
	    		  }						
    			  if((c1[crc_cnt_c1]==',')||(c1[crc_cnt_c1]=='\0'))
			      {
						c1[crc_cnt_c1]='\0';
						casegga++;
						crc_cnt_c1 = 0;
    			         //
						switch(casegga)
						{
							case 1:
								cptr = (const char *)(c1);
								UTC= atol(cptr);
        					  	ftemp = (int) (UTC/10000);
        					  	ucGps_Hour = fmod(ftemp,24)+8;
        					  	ucGps_Min = (int)((UTC - ftemp*10000)*0.01);
        					  	ucGps_Sec = (int)((UTC - ftemp*10000-ucGps_Min*100));
	        					break;
							case 2:
								cptr = (const char *)(c1);
								g_fGPS_llh = atof(cptr);
								g_fGPS_llh = g_fGPS_llh /100.0;
								fGps_l1=(int)(g_fGPS_llh);
								GpsDat->Pos[0] = fGps_l1+(g_fGPS_llh -fGps_l1)/0.6;
								break;
	        				case 3:
	        //					n_s=c1[0];
	        					 break;
			        		case 4:
								cptr = (const char *)(c1);					
								g_fGPS_llh =atof(cptr);
								g_fGPS_llh =g_fGPS_llh*0.01;
								fGps_l1=(int)(g_fGPS_llh );
								GpsDat->Pos[1] =fGps_l1+(g_fGPS_llh -fGps_l1)/0.6;
								break;
							case 5:
	// 					        e_w=c1[0];
						  		break;
							case 6:        //flag of position
	//				         	 FS=c1[0];
						  	break;
							case 7:	//ÐÇ?
						 		cptr = (const char *)(c1);
						  		GpsDat->SVs_used = atoi(cptr);
	//        					  if(*GpsDat.SVs_used<5)
	//        					  {
	//        						    ucCnt_GpsDelayValid = 0;
	//        					  }
						  	break;
	        				case 8:	 
	        					  cptr = (const char *)(c1);
	        					  GpsDat->pdop=atof(cptr);
	        					  break;
	        				case 9:
	        					  cptr = (const char *)(c1);
	        					  GpsDat->Pos[2]=atof(cptr);      					
	        					  break;
	        				default:
		    					  if(casegga>15)	//·ÀÖ¹Òæ´¦
		    					  {
	    						    wGPS_State = 0;
	    						    gps_ptr =0;
	    						    casevel=0;
	    						    casegga =0;
	    						    crc_cnt_c1=0;
	    						    cChkRec = 0;
	    						    cChk_js = 0;
		    					  }
						  	      break;
						}			 
				}
				else
					crc_cnt_c1++;	
    			  	break;
	    		case 8:
	    			  gps_data[gps_ptr++] = ucRecUart_Com;
	    			
	    			  wGPS_State = 9;
	    			  ck_b = ascii2hex(ucRecUart_Com);
	    			  break;
	    		case 9:
	    			  gps_data[gps_ptr++] = ucRecUart_Com;
	    
	    			  for(i=1;i< gps_ptr-3;i++)
	    				    checktest ^= gps_data[i];
	    			
	    			  ck_a =ascii2hex(ucRecUart_Com);			 
					  cChkRec = ck_b*16 + ck_a;	 
					  if( cChkRec ==  (cChk_js&0x0ff) )   // Òì»òÐ£ÑéÕýÈ·
					  {
							GpsDat->Pos_OK = 1;		
					  }
					  memset(gps_data,0,256);       
					  wGPS_State = 0;
					  checktest = 0;
					 break;	
    			
    		////== BESTXYZ ==============================///
			case 26:
				if(0x44 == ucRecUart_Com)
				{
				        wGPS_State = 27;
				        gps_data[1] = 0x44;
				}
				else 
				{
					wGPS_State = 0 ;
				}
				break;
			case 27:
				 if(0x12 == ucRecUart_Com)
				 {
				    wGPS_State = 28;
				    gps_data[2] = 0x12;
				 }
				 else 
				 {
					wGPS_State = 0 ;
				 }
				 break;
	      	case 28:
				  gps_data[3] = ucRecUart_Com;         //Ö¡Í·×Ö½Ú³¤ 28 byte
				  gps_ptr = 4;
				  head_cnt_r = gps_data[3];
				  wGPS_State = 29;
				  break;
			case 29:
				  gps_data[gps_ptr++] = ucRecUart_Com;
				  if(gps_ptr == head_cnt_r)             //ÊÕÖ¡Í·×Ö½Ú³¤¶È
				  {
					    msg_len_r = (int)gps_data[8] + (int)gps_data[9]*256;
					    msg_len = 0;
					    wGPS_State = 30;
				  }
				  break;
			case 30:
				  gps_data[gps_ptr++] = ucRecUart_Com;	// ÊÕÊý¾Ý³¤¶È
				  msg_len++;
				  if(msg_len == msg_len_r)
				  {
					    wGPS_State = 31;
					    msg_len = 0;
				  }
				  break;
			case 31:
				  crc_r.crc_char[msg_len++] = ucRecUart_Com;//Ð£ÑéºÍ
				  if(4 == msg_len)
				  {
					    msg_len=0;
					    if( crc_r.crc_long == CalculateBlockCRC32(gps_ptr, gps_data) )
					    {
					        gps_ptr=0;

                            for(i=0;i<2;i++)                         // week
							{
							      c2us.chardata[i]=gps_data[i+14];
							}
                            GpsDat->Tweek=c2us.ushortdata;
                            for(i=0;i<4;i++)                         // week mms
							{
							      c2ui.chardata[i]=gps_data[i+16];
							}
                            GpsDat->Tweek_second=c2ui.uintdata;

					        for(i = 0; i < 8; i++)                       // PX
							{
								  data_dchar.uc[i] = gps_data[36+i];
							}
							cecfpostion[0] = data_dchar.d;

							for(i = 0; i < 8; i++)                       // PY
							{
								  data_dchar.uc[i] = gps_data[44+i];
							}
							cecfpostion[1] = data_dchar.d;
							for(i = 0; i < 8; i++)                       // PZ
							{
								  data_dchar.uc[i] = gps_data[52+i];
							}
							cecfpostion[2] = data_dchar.d;

							for(i = 0; i < 4; i++)                       // PX STD
							{
								  data_fchar.uc[i] = gps_data[60+i];
							}
							cecfposstd[0] = (double)data_fchar.f;

							for(i = 0; i < 4; i++)                       // PY STD
							{
								  data_fchar.uc[i] = gps_data[64+i];
							}
							cecfposstd[1] = (double)data_fchar.f;

							for(i = 0; i < 4; i++)                       // PZ STD
							{
								  data_fchar.uc[i] = gps_data[68+i];
							}
							cecfposstd[2] = (double)data_fchar.f;

							for(i = 0; i < 8; i++)                       // PVx
							{
								  data_dchar.uc[i] = gps_data[80+i];
							}
							cecfvel[0] = data_dchar.d;

							for(i = 0; i < 8; i++)                       // PVy
							{
								  data_dchar.uc[i] = gps_data[88+i];
							}
						    cecfvel[1] = data_dchar.d;

							for(i = 0; i < 8; i++)                       // PVz
							{
								  data_dchar.uc[i] = gps_data[96+i];
							}
							cecfvel[2] = data_dchar.d;

							for(i = 0; i < 4; i++)                       // PVX STD
							{
								  data_fchar.uc[i] = gps_data[104+i];
							}
							cecfvelstd[0] = (double)data_fchar.f;

							for(i = 0; i < 4; i++)                       // PVX STD
							{
								  data_fchar.uc[i] = gps_data[108+i];
							}
							cecfvelstd[1] = (double)data_fchar.f;

							for(i = 0; i < 4; i++)                       // PVZ STD
							{
								  data_fchar.uc[i] = gps_data[112+i];
							}
							cecfvelstd[2] = (double)data_fchar.f;
                            
                           // GpsDat->SVs_used=gps_data[117];
                           //gps_data[116]  ÀÐÇÊýÄ¿
                           //gps_data[117]  ½âÎöËùÓÃÎÀÐÇÊýÄ¿
						   // GpsDat->Vel_OK = 1;
							XYZ_to_BLH(cecfpostion[0],cecfpostion[1],cecfpostion[2],&(GpsDat->Pos[0]),&(GpsDat->Pos[1]),&(GpsDat->Pos[2]));
						//	XYZ_to_BLH(cecfposstd[0], cecfposstd[1], cecfposstd[2],&(GpsDat->PosStd[0]),&(GpsDat->PosStd[1]),&(GpsDat->PosStd[2]));
							//
                            Speed_XYZ_to_BLH(GpsDat->Pos[0],GpsDat->Pos[1],GpsDat->Pos[2],cecfvel[0],cecfvel[1],cecfvel[2],&(GpsDat->V[0]),&(GpsDat->V[1]),&(GpsDat->V[2]));
							Speed_XYZ_to_BLH(GpsDat->Pos[0],GpsDat->Pos[1],GpsDat->Pos[2],cecfvelstd[0],cecfvelstd[1],cecfvelstd[2],&(GpsDat->VelStd[0]),&(GpsDat->VelStd[1]),&(GpsDat->VelStd[2]));
                            //
						    GpsDat->Vearth=  sqrt(GpsDat->V[0] * GpsDat->V[0]+  GpsDat->V[1]  * GpsDat->V[1]);  // 20170331
							GpsDat->g_uisVcc=sqrt(cecfvelstd[0]* cecfvelstd[0]+ cecfvelstd[1] * cecfvelstd[1]);

                            //
                           	if(g_fGPS_V_before[0]>1e-7/*&&g_fGPS_V_before[1]>1e-7&&g_fGPS_V_before[1]>1e-7*/)
							{
								 for(j=0;j<3;j++)
								 {
										g_fGPS_deltV[j] = g_fGPS_V_before[j] - GpsDat->V[j];
										g_fGPS_V_before[j] = GpsDat->V[j];
								 }
							}
							if(g_fGPS_P_before[0]>1e-7/*&&g_fGPS_P_before[1]>1e-7*/)
							{
								 for(j=0;j<2;j++)
								 {
									    g_fGPS_deltP[j] = g_fGPS_P_before[j] - GpsDat->Pos[j];
									    g_fGPS_P_before[j] = GpsDat->Pos[j];
								 }
							}
							if(SVs_used_before>0)
							{
								if((GpsDat->SVs_used-SVs_used_before)>=-2&&fabs(g_fGPS_deltP[0])<0.1&&fabs(g_fGPS_deltP[1])<0.1)	
								{
								    GpsDat->Pos_OK = 1;	
								}
								else
								{
								    GpsDat->Pos_OK = 0;	
								}
							}

							SVs_used_before = GpsDat->SVs_used;	

							if(fabs(g_fGPS_deltV[0])<C_GPS_V_delt && fabs(g_fGPS_deltV[1])<C_GPS_V_delt /*&& GpsDat->g_uisAcc<1.0*/)
							{
								GpsDat->Vel_OK = 1;
							}
							else
							{
								GpsDat->Vel_OK = 0;
								GpsDat->Vearth =0;
							}	
							//
						}
						memset(gps_data,0,256);
						wGPS_State = 0 ;
				 }
				break;
			case 35:
				gps_data[gps_ptr++] = ucRecUart_Com;
				cChk_js ^= ucRecUart_Com;
		        if(ucRecUart_Com=='T')
				{
				    wGPS_State = 36;
				}
				else
				{ 
					wGPS_State=0;
				}
			    break;
			case 36:
			    gps_data[gps_ptr++] = ucRecUart_Com;
				cChk_js ^= ucRecUart_Com;
		        if(ucRecUart_Com=='D')
				{
				    wGPS_State = 37;
				}
				else
				{ 
					wGPS_State=0;
				}
			    break;
            case 37:
			     gps_data[gps_ptr++] = ucRecUart_Com;
    			 cChk_js ^= ucRecUart_Com;
    			 if(ucRecUart_Com == ',')  
    			 {
				    wGPS_State = 38;
				    crc_cnt_c1 =0;
				    casevel=0;
    			 }
    			 else
    			 {  
    				wGPS_State =0;
				 }
			     break;
			case 38:       // ¿ªÊ¼½ÓÊÕº½Ïò
			     gps_data[gps_ptr++] = ucRecUart_Com;
	    		 c1[crc_cnt_c1] = ucRecUart_Com;
	    		 if(ucRecUart_Com == '*')  
	    		 {
	    			  wGPS_State = 39;
	    			  c1[crc_cnt_c1] = '\0';
	    		 }
	    		 else
	    		 {
	    			  cChk_js ^= ucRecUart_Com;
	    		 }
	    		 //						
    			 if((c1[crc_cnt_c1]==',')||(c1[crc_cnt_c1]=='\0'))
			     {
						c1[crc_cnt_c1]='\0';
					//	casegga++;
						crc_cnt_c1 = 0;
					//
					    cptr = (const char *)(c1);					
						GpsDat->DGPS_Course=atof(cptr);  // Ë«ÌìÏßGPSº½Ïò
						
				 }
				 else
				 {
					crc_cnt_c1++;
				 }
			     break;
			
			case 39:       //  check_a
    			  gps_data[gps_ptr++] = ucRecUart_Com;
    			  wGPS_State = 39;
    			  ck_b = ascii2hex(ucRecUart_Com);
    			  break;
	    	case 40:    //  check_b      
    			  gps_data[gps_ptr++] = ucRecUart_Com;
    
    			  for(i=1;i< gps_ptr-3;i++)
    				    checktest ^= gps_data[i];
    			
    			  ck_a =ascii2hex(ucRecUart_Com);			 
				  cChkRec = ck_b*16 + ck_a;	 
				  if( cChkRec ==  (cChk_js&0x0ff) )
				  {
					//	GpsDat->Pos_OK = 1;		
				  }
				  memset(gps_data,0,256);       
				  wGPS_State = 0;
				  checktest = 0;
				  break;
			default:
			    wGPS_State = 0 ;
			    break;
    						
    				
    	  }//swich	*/			
    				
     }// for 
}//