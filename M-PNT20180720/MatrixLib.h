//////////////////�ļ����� MatrixLib.h ////////////////////////////////////////
////  ��������ģ��Ŀ⺯��ͷ�ļ�
/* ��Ҫ���ܣ� �������ɿ⺯��

*/
/***********************************************************/

#ifndef _Matrix_Lib_
#define _Matrix_Lib_


#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
//#include "C6747.h"



void	 MatMult(double a[],double b[],int m1,int n1,int k,double c[]);	    //�������
void	 MatkMult(double out_m[], double in_m[],double k,int rows,int cols);///������Գ��� k
void     MatkMultf(float out_m[], float in_m[],float k,int rows,int cols); ///������Գ��� k �����ȸ���
void     MatSub(double a[],double b[],int m1,int n1,double c[]);		   //�������
void	 MatSubf(float a[],float b[],int m1,int n1,float c[]);	    //�������������
void	 get_m_Start(double ans[],double m[],int n);				//����ÿһ��ÿһ�е�ÿ��Ԫ������Ӧ������ʽ�����A*  
void	 MatT(double a[],int m1,int n1,double at[]);				//����ת��
void	 MatDiv(double a[],float t,int m1,int n1,double at[]);	//������Գ��� t

int	MatVD(double a[],int n,double v[],double eps,int jt);	// VD�ֽ⺯��
int	MatInv(double a[],int n);							    // ��������
int	MatInv1(double  out_m[], double  in_m[],int n);			// �������棬�����������������ʽ����
void	MatPlus(double a[],double b[],int m1,int n1,double c[]);// �������
void    MatPlusf(float a[],float b[],int m1,int n1,float c[]);   // ������� ������
void    Mat_Multf(float *p1,float *p2,int m,int k,int n,float *p3);//������� �����ȸ���
void    MatMultf(float a[],float b[],int m1,int n1,int k,float c[]);	//�������
double	get_fabs_m(double  m[],int n);	//����һ��չ������|Mag_A|
void    Across_Multply(double a[3],double b[3],double c[3]);		//�������


float  DelWildGyroPoint(float *fData,int nNum);		 // ȥ��Ұ��

#endif
