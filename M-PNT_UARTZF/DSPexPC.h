/*-------------------------------------------------------------------
�ļ����ƣ�	DSPexPC.h(DSP��)
�汾��		1.04
��дʱ�䣺	2014.08.01.
����ʱ�䣺	2015.02.04.
��д�ˣ�	������
����˵����
	2014.09.12.		
		1.ȥ��TIME_WTCOMMU�ĺ궨�壬ͳһ����timeout.h�ļ���	
	2015.01.29.		
		1.������ͨ�����ֳɹ���ʧ�ܵı�־����COMMU_SUCC��COMMU_FAULT	
	2015.02.04.		
		1.����������汾�ŵ��������ͼ�����صĺ궨��SOFTV��LEN_D_INFO_SOFTV	
		2.������ȫ�ֱ���info_Version[16]�����������ڴ�������汾��Ϣ
		  ��ȫ�ֱ�����main.c�б�����͸�ֵ����DSPexPC.c�б�����		
		3.����������ָ����뼰����غ궨��
		4.������������ݸ�ʽ�����ô���ĺ궨�壬ʵ������DSP�������
		  ��ʽ�Ĺ���	
-------------------------------------------------------------------*/
#ifndef __DSPEXPC_H
#define __DSPEXPC_H

#define	CMD_HEADER1		0xEB	// ����֡ͷ1
#define	CMD_HEADER2		0x90	// ����֡ͷ2

#define	CODE_CMD_UP		0x1		// �ϴ�ָ�����
#define	CODE_CMD_STA	0x2		// ״ָ̬�����
#define	CODE_CMD_DRQ	0x3		// ��������ָ�����
#define	CODE_CMD_LINK	0x4		// ����ָ�����
#define	CODE_CMD_SET	0x5		// ����ָ�����

#define	CODE_SET_FORMAT	0x1		// ������-���������ʽ ����

#define	V_FORMAT_IMU	0x0		// IMU���ݸ�ʽ
#define	V_FORMAT_CALIB	0x1		// �궨���ݸ�ʽ
#define	V_FORMAT_NAV	0x2		// �������ݸ�ʽ

#define	S_TIMEOUT		0x1		// ��ʱ����
#define	S_CHKERR		0x2		// У��ʹ������
#define	S_ERAERR		0x4		// ����ʧ�ܴ���
#define	S_PRGERR		0x5		// ��дʧ�ܴ���
#define	S_ERAOK			0x6		// �����ɹ�����
#define	S_PRGEOK		0x7		// ��д�ɹ�����
#define	S_UPDONE		0x8		// �ϴ���������
#define	S_LINK			0x9		// ���ֳɹ�����
#define	S_FOMATOK		0xA		// ���������ʽ���óɹ�����

#define	FIRMWARE		0x1		// �̼����ʹ���
#define	PROGRAM			0x2		// �������ʹ���
#define	COEFF			0x3		// ϵ�����ʹ���
#define	FIRMV			0x5		// �̼��汾���ʹ���
#define	ADDRCHK			0x6		// ��ַ������ʹ���
#define	SOFTV			0x8		// ����汾���ʹ���

#define	DATA_HEADER1	0x55	// ����֡ͷ1
#define	DATA_HEADER2	0xAA	// ����֡ͷ2

#define	FSTART_FIRM		0		// �̼���FLASH�е���ʼsector��	
#define	FSTART_PROG		0		// ������FLASH�е���ʼsector��
#define	FSTART_COEFF	10		// ϵ����FLASH�е���ʼsector��

#define	LEN_COEFF		200		// ϵ�����ݳ���
#define LEN_UPDN_ASM	100		// �ϴ�/��������֡�������Ϣ�γ���

#define LEN_D_HEADER	2		// ����֡֡ͷ�˳���
#define LEN_D_INDICATE	3		// ����ָ֡ʾ�γ���
#define LEN_D_CHECK		2		// ����֡У��γ���
// ��Ϣ�γ��� = ֡���� - LEN_D_INDICATE - LEN_D_CHECK

#define LEN_C_HEADER	2		// ָ��֡֡ͷ�˳���
#define LEN_C_INDICATE	1		// ָ��ָ֡ʾ�γ���
#define LEN_C_CHECK		2		// ָ��֡У��γ���

#define LEN_C_INFO_STA	1		// ״ָ̬�����Ϣ�γ���		
#define LEN_C_INFO_DRQ	3		// ��������ָ�����Ϣ�γ���	
#define LEN_C_INFO_UPLOAD	1	// �ϴ�ָ�����Ϣ�γ���	
#define	LEN_C_INFO_LINK	2		// ����ָ�����Ϣ�γ���
#define LEN_C_INFO_SET	2		// ����ָ�����Ϣ�γ���

#define LEN_D_INFO_FIRMV	3	// �̼��汾����֡����Ϣ�γ���
#define LEN_D_INFO_SOFTV	16	// ����汾����֡����Ϣ�γ���	

#define LEN_L			1		// ���ȶγ���

#define SIZE_D_BUFF		120		// ����֡�����С
#define SIZE_C_BUFF		20		// ָ��֡�����С	

#define COMMU_SUCC		0	
#define COMMU_FAULT		-1		

int	parse_command(unsigned char	[], unsigned int );
int parse_data(unsigned char [], unsigned int );
void command_DRq(unsigned char , unsigned char	, unsigned	char , unsigned int );
void command_status(unsigned char , unsigned int );					
int Execute_UpLoad(unsigned char , unsigned int );					
int Execute_DownLoad(unsigned char	, unsigned char , unsigned int );
void assemble_data(unsigned	char , unsigned	char , unsigned short , unsigned char [], unsigned int );
void command_link(unsigned int );
int	commu_link(unsigned int );
int	commu_link1(unsigned int );

extern unsigned char info_Version[16];

#endif	/*	__DSPEXPC_H	*/
