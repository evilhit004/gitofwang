;------------------------------------------------------
; Name:			boot.asm
; Description:	OMAPL137 C6747 NOR FLASH Legacy Boot Mode
;				Secondary BootLoader
; Author:		chepengyu
; Data:			2014.07.07.
; Update:	
;------------------------------------------------------
;------------------------------------------------------
; �궨��
;------------------------------------------------------ 
FLASH_START 	.set  0x60004000
RAM_START   	.set  0x11804000
FLASH_SIZE  	.set  0x2D000 		; ���ݳ�������С�޸Ĵ�ֵ(LEN - 0x4000һ�ΰ��˴���ĳ���)��ͬʱҲӦ���޸�hex2bin.cmd�ļ���lenֵ

PLL_CSR         .equ  0x01C11100
PLL_MULT        .equ  0x01C11110     
PLL_POSTDIV     .equ  0x01C11128  
PLL_MULT_VAL	.equ  0x11
PLL_OCSEL    	.equ  0x01C11104
PLL_CKEN        .equ  0x01C11148
PLL_OSCDIV1     .equ  0x01C11124

GPIO_DIR		.equ  0x01E26038
GPIO_VAL		.equ  0x01E2603C

KICK0			.equ  0x01C14038 
KICK1			.equ  0x01C1403C 

PMUX13			.equ  0x01C14154
PMUX14			.equ  0x01C14158 
PMUX15			.equ  0x01C1415C 
PMUX16			.equ  0x01C14160 
PMUX17			.equ  0x01C14164 
PMUX18			.equ  0x01C14168 
PMUX19			.equ  0x01C1416C 

EMIFA_CFG2		.equ  0x68000010

PSC_EMIFA		.equ  12
PSC_GPIO		.equ  12
;------------------------------------------------------
; ��Ŷ���Ͳο����ζ���
;------------------------------------------------------             
    .global		_c_int00
    .def		boot2nd
    
	.sect 		".bootload"
;------------------------------------------------------
; �Ե�ַд���ݺ�
;------------------------------------------------------ 
WRITE_REG .macro addr,data
   		mvkl	addr,A0
    	mvkh  	addr,A0
   		mvkl  	data,A3
   		mvkh  	data,A3
   		STW   	A3,*A0
   		NOP 4   
   		.endm	
;------------------------------------------------------
; �ӳ�ѭ����
;------------------------------------------------------    		
WAIT_PLL .macro count
		mvkl	count,B0
		mvkh	count,B0
dec?	SUB		B0,1,B0
  [B0]	B		dec?
  		NOP		4	
  		.endm			   		
;------------------------------------------------------
; ʹ��PCS��
; 	addr:�����Ӧ��MDCTL�Ĵ�����ַ(���Ե�ַ)
;	pscbase:PSC0(0x01C10000)/PSC1(0x01E27000)�Ļ���ַ
;	num = ������x4
;------------------------------------------------------  
PSC_ENABLE .macro num,pscbase
    	mvkl	pscbase,A5		
   		mvkH	pscbase,A5
    	 
    	MVK		0xA00,A0		; MDCTLn�Ĵ�������NEXT̬ΪEnable state
   		MVK		num,A1
   		ADD		A1,A0,A0
   		ADD		A0,A5,A0
    	    	
    	LDW		*A0,A3
    	CLR		A3,0,4,A3
    	OR		3,A3,A3
    	STW		A3,*A0	 	   		
		NOP 2 
   	
   		MVK  	0x120,A0		; д1��PTCMD�Ĵ�����PD��CURRENT̬ת��ΪNEXT̬  	
    	ADD		A5,A0,A0
    	LDW		*A0,A3
    	OR		1,A3,A3
    	STW		A3,*A0
    	NOP		2
    	
   		mvkl  	0x128,A0		; ���PDת��״̬	
    	mvkh  	0x128,A0
    	ADD		A5,A0,A0
poll1?  LDW		*A0,A3
    	AND		1,A3,B0
  [B0]	BNOP	poll1?,5	
    	
		MVK		0x800,A0		; ���ģ�鵱ǰ״̬�Ƿ�Ϊenable(03h��ʾenable)
        MVK		num,A1  
        ADD		A1,A0,A0
        ADD		A0,A5,A0
          	
poll2?	LDW		*A0,A3 
		EXTU	A3,26,26,A3
		CMPEQ	A3,3,B0
  [!B0] BNOP	poll2?,5	
     			  	 
		.endm     
;------------------------------------------------------
; ���ΰ��˳���
;	����PINMUX
;	����PSC
;	����PLL
;	����GPIO
;	����EMIFA
;	��FLASH�п������뵽RAM
;------------------------------------------------------  	
boot2nd:		
			;----------------------------------
			; ���� SYSCFG            
			;----------------------------------
            WRITE_REG KICK0,0x83e70b13
            WRITE_REG KICK1,0x95a4f1e0 
            WRITE_REG PMUX13,0x11000000
            WRITE_REG PMUX14,0x11111111 
            WRITE_REG PMUX15,0x11111111 
            WRITE_REG PMUX16,0x11111111 
            WRITE_REG PMUX17,0x08111111 
            WRITE_REG PMUX18,0x88111818 
            WRITE_REG PMUX19,0x00000008 
            
			;----------------------------------
			; ���� PSC
			;----------------------------------
			PSC_ENABLE PSC_EMIFA,0x01C10000
			PSC_ENABLE PSC_GPIO,0x01E27000 
			
			WRITE_REG GPIO_DIR,0xFFFFFEEA 
            WRITE_REG GPIO_VAL,0xFFFF 
			
			;----------------------------------
 			; ���� PLL
 			;----------------------------------
 			; step1
 			MVKL 	PLL_CSR,A4		; clear PLLENSRC 
           	MVKH  	PLL_CSR,A4
           	LDW   	*A4,A3           
           	NOP    	4
           	CLR    	A3,5,5,A3        
           	STW    	A3,*A4
           	
           	MV     	A4,B5			; set PLL bypass Mode
           	LDW    	*B5,B4            
           	NOP    	4
           	AND    	-2,B4,B4          
           	STW    	B4,*B5            

           	LDW    	*A4,A3         	; reset PLL   
           	NOP    	4
           	AND    	-9,A3,A3         
           	STW    	A3,*A4 
           	
           	WAIT_PLL 4                   
 			 			                        
            ; step2
           	LDW   	*B5,B4         	; clear PLLENSRC    
           	NOP   	4
           	CLR   	B4,5,5,B4         
           	STW   	B4,*B5            

           	MV     	B5,A4
           	LDW    	*A4,A3            
           	NOP    	4
           	CLR    	A3,8,8,A3         
           	STW    	A3,*A4         
           	
           	LDW     *A4,A3      	; External Clock	       
           	NOP     4
           	SET     A3,8,8,A3         
           	STW     A3,*A4    
           	
           	; step3					; reset PLL
           	LDW     *A4,A3            
           	NOP     4
           	AND     -9,A3,A3          
           	STW     A3,*A4                              
            
            ; step4
            LDW     *B5,B4         	; Power up PLL    
            NOP     4                 
            AND     -3,B4,B4          
            STW     B4,*B5  
            
            ; step5
            MVKL	PLL_MULT_VAL,B4	; set PLL multiplier by 18
            MVKH	PLL_MULT_VAL,B4
            MVKL    PLL_MULT,B5
           	MVKH    PLL_MULT,B5
           	NOP     2
           	STW     B4,*B5 
           	
           	MVKL	PLL_POSTDIV,A4 	; set post-divide by 1 
           	MVKH	PLL_POSTDIV,A4 
         	ZERO    A3
         	SET     A3,0xf,0xf,A3
           	STW     A3,*A4 
           	
           	; step7
           	MVKL	PLL_CSR,A4		; Release from reset
           	MVKH	PLL_CSR,A4			
           	LDW     *A4,A3            
           	NOP     4
           	OR      8,A3,A3           
           	STW     A3,*A4 
           	
           	; ����PLL OBS�۲�ʱ��						
           	MVKL	PLL_OCSEL,B4 	
           	MVKH	PLL_OCSEL,B4
           	LDW		*B4,B5
           	MVK		20,B6
           	OR		B6,B5,B5
           	STW		B5,*B4
           	
           	MVKL	PLL_CKEN,B4 	
           	MVKH	PLL_CKEN,B4
           	LDW		*B4,B5
           	OR		2,B5,B5
           	STW		B5,*B4
           	
           	MVKL	PLL_OSCDIV1,B4 	
           	MVKH	PLL_OSCDIV1,B4
           	LDW		*B4,B5
           	SET     B5,15,15,B5 
           	STW		B5,*B4           	
           	
           	; step8
           	WAIT_PLL 2500   		; wait PLL to lock
           	
           	; step9
           	LDW		*A4,A3			; remove PLL from bypass mode
           	OR		1,A3,A3
           	STW		A3,*A4                                    
			NOP		4
 			
            ;----------------------------------
            ; ���� GPIO            
            ;----------------------------------
            WRITE_REG GPIO_DIR,0xFFFFFEEA 
            WRITE_REG GPIO_VAL,0x0 
            
            ;----------------------------------
            ; ���� EMIFA
            ;----------------------------------
            WRITE_REG EMIFA_CFG2,0x3D4DEA6D 
      
			;----------------------------------      
      		; ���� FLASH �� RAM
      		;----------------------------------
	  		mvkl 	FLASH_START,A4
	  		mvkh 	FLASH_START,A4  		
	  		mvkl 	RAM_START,B4
	  		mvkh 	RAM_START,B4
      		mvkl 	FLASH_SIZE,B5
      		mvkh 	FLASH_SIZE,B5
      		zero 	A1

loop:		mvkl 	GPIO_VAL,A5		; A5���GPIO����ֵ�Ĵ�����ַ
			mvkh 	GPIO_VAL,A5
			ZERO	A2
    		
			SHR 	A4,14,A0		; FLASH��ַ����14λ����������A0
			AND 	0xF,A0,A0		; A0 = (addr>>14) & 0x0F	
			
			AND 	1,A0,A3			; ��ȡA14��ֵ
			OR 		A3,A2,A2
			
			SHR 	A0,1,A0			; ��ȡA15��ֵ
			AND 	1,A0,A3	
			SHL 	A3,2,A3
			OR 		A3,A2,A2
			
			SHR 	A0,1,A0			; ��ȡA16��ֵ
			AND 	1,A0,A3	
			SHL 	A3,4,A3			
			OR 		A3,A2,A2
			
			SHR 	A0,1,A0			; ��ȡA17��ֵ
			AND 	1,A0,A3	
			SHL 	A3,8,A3
			OR 		A3,A2,A2
			
			STW  	A2,*A5			; дGPIO��ֵ
			
    		LDW  	*A4++,B0		; ����FLASH���ݵ�RAM
    		NOP 	5
    		STW  	B0,*B4++
    		ADD  	4,A1,A1
    		CMPLT 	A1,B5,B0
    		NOP
 	[B0] 	b 		loop
    		nop   	5       			
    		b 		_c_int00
    		nop 	5