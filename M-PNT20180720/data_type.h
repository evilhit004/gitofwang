//------------------------------------------------------------------------------------
// ������Ͷ���
//------------------------------------------------------------------------------------
#ifndef __DATA_TYPE_H
#define __DATA_TYPE_H

/* exact-width signed integer types */
typedef signed char int8_t;
typedef signed int 	int16_t;
typedef signed long int32_t;

/* exact-width unsigned integer types */
typedef unsigned char 	uint8_t;
typedef unsigned int 	uint16_t;
typedef unsigned long 	uint32_t;


union uc2ul{
		uint32_t 		lData; 
		uint8_t		 	ucData[4];
    };
    
union uc2l{
		int32_t			lData; 
		uint8_t		 	ucData[4];
    };    
    
union uc2f{
		float 			fData; 
		uint8_t		 	ucData[4];
    };
	
union uc2us{
		uint16_t		sData;
		uint8_t 		ucData[2];
    };
    
union uc2s{
		int16_t			sData;
		uint8_t 		ucData[2];
    };	

union us2f
	{
		float f_data;
		uint16_t s_data[2];
	};

union us2ul
	{
		uint32_t ul_data;
		uint16_t s_data[2];
	};

   
#endif

