/*
 *  Copyright 2008 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  GPIO implementation
 *
 *	更新时间：	2014.05.20.
 *	修改人：	chepengyu
 *	更新说明：
 *		1.增加了GPIO中断设置的函数
 */


#include "omapl137_gpio.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _GPIO_setDirection( number, direction )                                 *
 *                                                                          *
 *      number    <- GPIO#                                                  *
 *      direction <- 0:OUT 1:IN                                             *
 *                                                                          *
 * ------------------------------------------------------------------------ */
short EVMOMAPL137_GPIO_setDirection( unsigned short number, unsigned char direction )
{
    unsigned int bank_id = ( number >> 5 );
    unsigned int pin_id  = ( 1 << ( number & 0x1F ) );
    unsigned int* gpio_dir = ( unsigned int * )( GPIO_BASE + GPIO_DIR_BASE + ( bank_id * GPIO_BASE_OFFSET ) );

    if ( ( direction & 1 ) == GPIO_OUT )
        *gpio_dir &= ~( pin_id );  // Set to OUTPUT
    else
        *gpio_dir |= ( pin_id );   // Set to INPUT

    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _GPIO_setOutput( number, output )                                       *
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *      value    <- 0:LOW 1:HIGH                                            *
 *                                                                          *
 * ------------------------------------------------------------------------ */
short EVMOMAPL137_GPIO_setOutput( unsigned short number, unsigned char output )
{
    unsigned int bank_id = ( number >> 5 );
    unsigned int pin_id  = ( 1 << ( number & 0x1F ) );
    unsigned int* gpio_out = ( unsigned int* )( GPIO_BASE + GPIO_OUT_DATA_BASE + ( bank_id * GPIO_BASE_OFFSET ) );

    if ( ( output & 1 )  == 0 )
        *gpio_out &= ~( pin_id );  // Set to LOW
    else
        *gpio_out |= ( pin_id );   // Set to HIGH

    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _GPIO_getInput( number )                                                *
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 *      Returns:    0:LOW                                                   *
 *                  1:HIGH                                                  *
 *                                                                          *
 * ------------------------------------------------------------------------ */
short EVMOMAPL137_GPIO_getInput( unsigned short number )
{
    unsigned int input;
    unsigned int bank_id = ( number >> 5 );
    unsigned int pin_id  = ( number & 0x1F );
    unsigned int* gpio_in = ( unsigned int* )( GPIO_BASE + GPIO_IN_DATA_BASE + ( bank_id * GPIO_BASE_OFFSET ) );

    input = *gpio_in;
    input = ( input >> pin_id ) & 1;

    return input;
}
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  设置上升沿触发中断                                               *
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void OMAPL137_GPIO_setREI(unsigned short number)
{
	unsigned int bank_id = ( number >> 5 );
    unsigned int pin_id  = ( number & 0x1F );
    unsigned int* gpio_setRIS = ( unsigned int* )( GPIO_BASE + GPIO_SET_RIS_TRIG_BASE + ( bank_id * GPIO_BASE_OFFSET ) );	
	
	*gpio_setRIS |= ( pin_id );   // Set to HIGH	
}	
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  清除上升沿触发中断                                               *
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void OMAPL137_GPIO_clrREI(unsigned short number)
{
	unsigned int bank_id = ( number >> 5 );
    unsigned int pin_id  = ( number & 0x1F );
    unsigned int* gpio_clrRIS = ( unsigned int* )( GPIO_BASE + GPIO_CLR_RIS_TRIG_BASE + ( bank_id * GPIO_BASE_OFFSET ) );	
	
	*gpio_clrRIS |= ( pin_id );   // Set to HIGH		
}
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  设置下降沿触发中断                                               *
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void OMAPL137_GPIO_setFEI(unsigned short number)
{
	unsigned int bank_id = ( number >> 5 );
    unsigned int pin_id  = ( number & 0x1F );
    unsigned int* gpio_setFAL = ( unsigned int* )( GPIO_BASE + GPIO_SET_FAL_TRIG_BASE + ( bank_id * GPIO_BASE_OFFSET ) );	
	
	*gpio_setFAL |= ( pin_id );   // Set to HIGH
}
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  清除下降沿触发中断                                               *
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void OMAPL137_GPIO_clrFEI(unsigned short number)
{
	unsigned int bank_id = ( number >> 5 );
    unsigned int pin_id  = ( number & 0x1F );
    unsigned int* gpio_clrFAL = ( unsigned int* )( GPIO_BASE + GPIO_CLR_FAL_TRIG_BASE + ( bank_id * GPIO_BASE_OFFSET ) );	
	
	*gpio_clrFAL |= ( pin_id );   // Set to HIGH	
}
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  读取GPIO引脚中断状态                                              		*
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
short OMAPL137_GPIO_ReadISR(unsigned short number)	
{
	unsigned int ISR_status;
    unsigned int bank_id = ( number >> 5 );
    unsigned int pin_id  = ( number & 0x1F );
    unsigned int* gpio_ISR = ( unsigned int* )( GPIO_BASE + GPIO_INSTAT_BASE + ( bank_id * GPIO_BASE_OFFSET ) );

    ISR_status = *gpio_ISR;
    ISR_status = ( ISR_status >> pin_id ) & 1;

    return ISR_status;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  清除GPIO引脚中断标志                                              		*
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void OMAPL137_GPIO_ClrISR(unsigned short number)	
{
	unsigned int ISR_status = 0;
    unsigned int bank_id = ( number >> 5 );
    unsigned int pin_id  = ( number & 0x1F );
    unsigned int* gpio_ISR = ( unsigned int* )( GPIO_BASE + GPIO_INSTAT_BASE + ( bank_id * GPIO_BASE_OFFSET ) );

    ISR_status = *gpio_ISR;
    ISR_status = ( ISR_status >> pin_id ) | 1;
}
