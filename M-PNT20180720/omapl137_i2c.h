/*
 *  Copyright 2008 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  I2C header file
 *
 */

#ifndef I2C_
#define I2C_

#include "OMAPL137.h"


/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */
Int16 EVMOMAPL137_I2C_init (void );
Int16 EVMOMAPL137_I2C_close(void );
//Int16 EVMOMAPL137_I2C_write( Uint16 i2c_addr, Uint8* data, Uint16 len,Uint8  stopack);
//Int16 EVMOMAPL137_I2C_read ( Uint16 i2c_addr, Uint8* data, Uint16 len,Uint8  stopack);
//void EVMOMAPL137_wait(Uint32 delay);

#endif
