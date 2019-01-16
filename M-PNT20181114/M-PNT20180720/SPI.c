/***********************************************************************/
/*
 *  DATA:  2016-11-21
 *  OMAPL137-SPI 配置
 *  捷联系统研究室
 */
/***********************************************************************/


#include "Atmel_SPI_Flash.h"
#include "omapl137_gpio.h"


CSL_SpiRegsOvly  spiRegs1 = (CSL_SpiRegsOvly)CSL_SPI_1_REGS;
//transimit and receive a byte via SPIDAT0
void spi1_xfer_simple(const void *dout, void *din)
{
	Uint8 txByte = 0;

	/* do an empty read to clear the current contents */
	//spiRegs->SPIBUF;

	/* wait till transmit buffer is empty */
	while (spiRegs1->SPIBUF & 0x20000000);
	/* set the write data if it's valid */
	if (dout) {
		txByte = *(Uint8*)dout & ((1 << SPI_NUM_OF_TXBITS) - 1);
	}
	/* write to DAT1 is required to keep the serial transfer going */
	/* we just terminate when we reach the end */
	spiRegs1->SPIDAT0 = txByte;

	/* read the data - wait for data availability */
	while(spiRegs1->SPIBUF & 0x80000000);
	if (din) {
		*(Uint8*)din = spiRegs1->SPIBUF & ((1 << SPI_NUM_OF_TXBITS) - 1);			
	}
	else {
		spiRegs1->SPIBUF;
	}

}
//初始化
void SpiInit(void)
{
	/* First reset the SPI chip                                               */
	spiRegs1->SPIGCR0 &= 0xfffffffe;

	wait(100);

	/* now bring the chip out of reset state                                  */
	spiRegs1->SPIGCR0 |= 0x00000001;

	/* enable the CLKMOD and MASTER bits in the SPI global control reg        */
	spiRegs1->SPIGCR1 |=  0x00000003;//bit24 =1, bit[1:0]= 3;

	/* enable the pins so that they are used for the SPI interface(Multiplex) */
	spiRegs1->SPIPC0  = 0
		| ( 1 << 11 )   // DI
		| ( 1 << 10 )   // DO
		| ( 1 << 9 )    // CLK
		| ( 0 << 1 )    // EN1
		| ( 1 << 0 );   // cs


	/* configure the data format in SPIFMT                                    */
	spiRegs1->SPIFMT[0] = 0
		| ( 0 << 20 )   // SHIFTDIR MSB FIRST
		| ( 1 << 18 )
		| ( 1 << 17 )   // Polarity  SPIclocksignalis hign-inactive (before andafterdatatransfer the clocksignalis hign).
		| ( 1 << 16 )   // Phase   SPIclocksignalis notdelayedversusthe transmit/receive datastream
		| ( 16 << 8 )    // Prescale to 150MHz,change to SYSCLK2/15.
		| ( 8 << 0 );   // Char Len


	/* set the preconfigure data format as 0 which is already set above       */
	spiRegs1->SPIDAT1 = 0<<24;//CSL_FMKT(SPI_SPIDAT1_DFSEL,FORMAT0);

	/* dont use any interrupts hence disable them                             */
	spiRegs1->SPIINT0 = 0
		| ( 1 << 24 )//high-impedance
		| ( 0 << 16 )   //
		| ( 0 << 8 )    //
		| ( 0 << 6 )    //
		| ( 0 << 4 );   //

	spiRegs1->SPIGCR1  |= ( 1 << 24 );/* Enable SPI */
}

//-----------------------------------------------------------------
// 读取陀螺数据函数
//-----------------------------------------------------------------
void read_MGZ310_Gyro(unsigned int gyro_swich, float * gyro_output, float * gyrotemp_output)
{
	int i = 0 ;
//	unsigned short status = 0;
	int gyro_data = 0;
	int gyrotemp_data = 0;	
	unsigned char tx_data[9] = {0};
    unsigned char rx_data[9] = {0};
	const Uint8	*txp ;
		  Uint8	*rxp ;
		txp = &tx_data[0];
		rxp = &rx_data[0];
		tx_data[0] = 0xF0;
		tx_data[1] = 0x00;
		tx_data[2] = 0x00;
		tx_data[3] = 0x00;
		tx_data[4] = 0x00;
		tx_data[5] = 0x00;
		tx_data[6] = 0x00;
		tx_data[7] = 0x00;
		tx_data[8] = 0x00;
//		for(j=0;j<9;j++)
//		{
//			rx_data[j] = 0;
//		}
	/* configure the data format in SPIFMT                                    */
	spiRegs1->SPIFMT[0] = 0
		| ( 0 << 20 )   // SHIFTDIR MSB FIRST
		| ( 1 << 18 )
		| ( 1 << 17 )   // Polarity  SPIclocksignalis hign-inactive (before andafterdatatransfer the clocksignalis hign).
		| ( 0 << 16 )   // Phase   SPIclocksignalis notdelayedversusthe transmit/receive datastream
		| ( 16 << 8 )    // Prescale to 150MHz,change to SYSCLK2/15.
		| ( 8 << 0 );   // Char Len
	if(gyro_swich == GYROX)
		{
			EVMOMAPL137_GPIO_setOutput(0x73,0);
		}
			else if (gyro_swich == GYROY)
			{
				EVMOMAPL137_GPIO_setOutput(0x74,0);
			}
			else if (gyro_swich == GYROZ)
				{
					EVMOMAPL137_GPIO_setOutput(0x75,0);
				}
					else
				{
					EVMOMAPL137_GPIO_setOutput(0x73,1);
					EVMOMAPL137_GPIO_setOutput(0x74,1);
					EVMOMAPL137_GPIO_setOutput(0x75,1);
				}	
	for(i=0;i<9;i++)
	{	
	
		spi1_xfer_simple(txp, rxp);
		txp++;
		rxp++;
		
	}
	gyro_data = ((rx_data[5]<<16) + (rx_data[4]<<8) +(rx_data[3])) & 0xFFFFFF;
	if(gyro_swich == GYROX)
		{
			EVMOMAPL137_GPIO_setOutput(0x73,1);
			if((gyro_data & 0x800000) == 0x800000)		// 1 & 负电压
			{
				*gyro_output = -1* ((0x1000000 - gyro_data)* LSB_GYROX);
			}
			else
			{
				*gyro_output = gyro_data* LSB_GYROX;	
			}
		}
			else if (gyro_swich == GYROY)
			{
				EVMOMAPL137_GPIO_setOutput(0x74,1);
				if((gyro_data & 0x800000) == 0x800000)		// 1 & 负电压
				{
					*gyro_output = -1* ((0x1000000 - gyro_data)* LSB_GYROYZ);
				}
				else
				{
					*gyro_output = gyro_data* LSB_GYROYZ;	
				}
			}
			else if (gyro_swich == GYROZ)
				{
					EVMOMAPL137_GPIO_setOutput(0x75,1);
					if((gyro_data & 0x800000) == 0x800000)		// 1 & 负电压
					{
						*gyro_output = -1* ((0x1000000 - gyro_data)* LSB_GYROYZ);
					}
					else
					{
						*gyro_output = gyro_data* LSB_GYROYZ;	
					}
				}		
			else
				{
					EVMOMAPL137_GPIO_setOutput(0x73,1);
					EVMOMAPL137_GPIO_setOutput(0x74,1);
					EVMOMAPL137_GPIO_setOutput(0x75,1);
				}	
		
		gyrotemp_data = ((rx_data[2]<<8) +(rx_data[1])) & 0xFFFF;

	if((gyrotemp_data & 0x8000) == 0x8000)		// 1 & 负电压
		{
			*gyrotemp_output = -1* ((0x10000 - gyrotemp_data)* 0.003125) + 12.5;
		}
		else
		{
			*gyrotemp_output = gyrotemp_data* 0.003125 + 12.5;	
		}	
}

//-----------------------------------------------------------------
// 读取加速度计数据函数
//-----------------------------------------------------------------
void read_MSA8000D_Acc(unsigned int acc_swich, float * acc_output, float * acctemp_output)
{
	int i = 0;
//	unsigned short status = 0;
	int acc_data = 0;
	int acctemp_data = 0;	
	unsigned char tx_data[7] = {0,0,0,0,0,0,0};
    unsigned char rx_data[7] = {0,0,0,0,0,0,0};
	const Uint8	*txp ;
		  Uint8	*rxp ;
		txp = &tx_data[0];
		rxp = &rx_data[0];
		tx_data[0] = 0x50;
		tx_data[1] = 0x00;
		tx_data[2] = 0x00;
		tx_data[3] = 0x00;
		tx_data[4] = 0x00;
		tx_data[5] = 0x00;
		tx_data[6] = 0x00;
				
//		for(j=0;j<7;j++)
//		{
//			rx_data[j] = 0;
//		}
	/* configure the data format in SPIFMT                                    */
	spiRegs1->SPIFMT[0] = 0
		| ( 0 << 20 )   // SHIFTDIR MSB FIRST
		| ( 1 << 18 )
		| ( 0 << 17 )   // Polarity  SPIclocksignalis low-inactive (before andafterdatatransfer the clocksignalis low).
		| ( 1 << 16 )   // Phase   SPIclocksignalis  delayed versus the transmit/receive datastream
		| ( 16 << 8 )    // Prescale to 150MHz,change to SYSCLK2/15.
		| ( 8 << 0 );   // Char Len
	if(acc_swich == ACCX)
		{
			EVMOMAPL137_GPIO_setOutput(0x76,0);
		}
			else if (acc_swich == ACCZ)
			{
				EVMOMAPL137_GPIO_setOutput(0x77,0);
			}
			else if (acc_swich == ACCY)
				{
					EVMOMAPL137_GPIO_setOutput(0x78,0);
				}
				else
				{
					EVMOMAPL137_GPIO_setOutput(0x76,1);
					EVMOMAPL137_GPIO_setOutput(0x77,1);
					EVMOMAPL137_GPIO_setOutput(0x78,1);
				}
	for(i=0;i<7;i++)
	{	
	
		spi1_xfer_simple(txp, rxp);
		txp++;
		rxp++;
		
	}
	
	if(acc_swich == ACCX)
		{
			EVMOMAPL137_GPIO_setOutput(0x76,1);
		}
			else if (acc_swich == ACCZ)
			{
				EVMOMAPL137_GPIO_setOutput(0x77,1);
			}
			else if (acc_swich == ACCY)
				{
					EVMOMAPL137_GPIO_setOutput(0x78,1);
				}
				else
				{
					EVMOMAPL137_GPIO_setOutput(0x76,1);
					EVMOMAPL137_GPIO_setOutput(0x77,1);
					EVMOMAPL137_GPIO_setOutput(0x78,1);
				}
	
		acc_data = ((((rx_data[1] & 0x7F)<<25) + (rx_data[2]<<17) + (rx_data[3]<<9) +(rx_data[4])<<1)>>8) & 0xFFFFFF;
		acctemp_data = ((rx_data[5]<<8) +(rx_data[6])) & 0x3FFF;
	if((acc_data & 0x800000) == 0x800000)		// 1 & 负电压
		{
			*acc_output = -1* ((0x1000000 - acc_data)* 0.000001667);          //加速度计的标度因数为600000LSB/g
		}
		else
		{
			*acc_output = acc_data* 0.000001667;	
		}
	if((acctemp_data & 0x2000) == 0x2000)		// 1 & 负电压
		{
			*acctemp_output = -1* ((0x4000 - acctemp_data)* 0.1);
		}
		else
		{
			*acctemp_output = acctemp_data* 0.1;	
		}	
}

