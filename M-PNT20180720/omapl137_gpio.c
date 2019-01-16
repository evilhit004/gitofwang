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

// #include "evmomapl137_gpio.h"
#include "omapl137_gpio.h"
#include "omapl137.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _GPIO_setDirection( number, direction )                                 *
 *                                                                          *
 *      number    <- GPIO#                                                  *
 *      direction <- 0:OUT 1:IN                                             *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 EVMOMAPL137_GPIO_setDirection( Uint16 number, Uint8 direction )
{
    Uint32 bank_id = ( number >> 5 );
    Uint32 pin_id  = ( 1 << ( number & 0x1F ) );
    Uint32* gpio_dir = ( Uint32* )( GPIO_BASE + GPIO_DIR_BASE + ( bank_id * GPIO_BASE_OFFSET ) );

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
Int16 EVMOMAPL137_GPIO_setOutput( Uint16 number, Uint8 output )
{
    Uint32 bank_id = ( number >> 5 );
    Uint32 pin_id  = ( 1 << ( number & 0x1F ) );
    Uint32* gpio_out = ( Uint32* )( GPIO_BASE + GPIO_OUT_DATA_BASE + ( bank_id * GPIO_BASE_OFFSET ) );

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
/*Int16 EVMOMAPL137_GPIO_getInput( Uint16 number )
{
    Uint32 input;
    Uint32 bank_id = ( number >> 5 );
    Uint32 pin_id  = ( number & 0x1F );
    Uint32* gpio_in = ( Uint32* )( GPIO_BASE + GPIO_IN_DATA_BASE + ( bank_id * GPIO_BASE_OFFSET ) );

    input = *gpio_in;
    input = ( input >> pin_id ) & 1;

    return input;
}*/
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  设置上升沿触发中断                                               *
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
/*void OMAPL137_GPIO_setREI(Uint16 number)
{
	Uint32 bank_id = ( number >> 5 );
    Uint32 pin_id  = ( number & 0x1F );
    Uint32* gpio_setRIS = ( Uint32* )( GPIO_BASE + GPIO_SET_RIS_TRIG_BASE + ( bank_id * GPIO_BASE_OFFSET ) );	
	
	*gpio_setRIS |= ( pin_id );   // Set to HIGH	
}	*/
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  清除上升沿触发中断                                               *
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
/*void OMAPL137_GPIO_clrREI(Uint16 number)
{
	Uint32 bank_id = ( number >> 5 );
    Uint32 pin_id  = ( number & 0x1F );
    Uint32* gpio_clrRIS = ( Uint32* )( GPIO_BASE + GPIO_CLR_RIS_TRIG_BASE + ( bank_id * GPIO_BASE_OFFSET ) );	
	
	*gpio_clrRIS |= ( pin_id );   // Set to HIGH		
}*/
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  设置下降沿触发中断                                               *
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
/*void OMAPL137_GPIO_setFEI(Uint16 number)
{
	Uint32 bank_id = ( number >> 5 );
    Uint32 pin_id  = ( number & 0x1F );
    Uint32* gpio_setFAL = ( Uint32* )( GPIO_BASE + GPIO_SET_FAL_TRIG_BASE + ( bank_id * GPIO_BASE_OFFSET ) );	
	
	*gpio_setFAL |= ( pin_id );   // Set to HIGH
}*/
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  清除下降沿触发中断                                               *
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
/*void OMAPL137_GPIO_clrFEI(Uint16 number)
{
	Uint32 bank_id = ( number >> 5 );
    Uint32 pin_id  = ( number & 0x1F );
    Uint32* gpio_clrFAL = ( Uint32* )( GPIO_BASE + GPIO_CLR_FAL_TRIG_BASE + ( bank_id * GPIO_BASE_OFFSET ) );	
	
	*gpio_clrFAL |= ( pin_id );   // Set to HIGH	
}*/
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  读取GPIO引脚中断状态                                              		*
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
/*Int16 OMAPL137_GPIO_ReadISR(Uint16 number)	
{
	Uint32 ISR_status;
    Uint32 bank_id = ( number >> 5 );
    Uint32 pin_id  = ( number & 0x1F );
    Uint32* gpio_ISR = ( Uint32* )( GPIO_BASE + GPIO_INSTAT_BASE + ( bank_id * GPIO_BASE_OFFSET ) );

    ISR_status = *gpio_ISR;
    ISR_status = ( ISR_status >> pin_id ) & 1;

    return ISR_status;
}*/

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  清除GPIO引脚中断标志                                              		*
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
/*void OMAPL137_GPIO_ClrISR(Uint16 number)	
{
	Uint32 ISR_status = 0;
    Uint32 bank_id = ( number >> 5 );
    Uint32 pin_id  = ( number & 0x1F );
    Uint32* gpio_ISR = ( Uint32* )( GPIO_BASE + GPIO_INSTAT_BASE + ( bank_id * GPIO_BASE_OFFSET ) );

    ISR_status = *gpio_ISR;
    ISR_status = ( ISR_status >> pin_id ) | 1;
}*/
