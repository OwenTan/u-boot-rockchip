/**@@@+++@@@@******************************************************************
 **
 ** Microsoft Windows Media
 ** Copyright (C) Microsoft Corporation. All rights reserved.
 **
 ***@@@---@@@@******************************************************************
 */

#define     IN_CRC32
//#include    "../config.h"
#include 		<asm/arch/rk30_drivers.h>

/******************************************************************************/
#define CRC16_CCITT         0x1021		// CRC ������ʽ.
#define CRC32_CRC32         0x04C10DB7
////////////////////////////////////////////////////////////////////////////////////////
//#include "crctable.c"
/******************************************************************************/


/******************************************************************************/
// ע�⣺�����λһ��Ϊ"1"������ȥ 
//const INT16U cnCRC_16 = 0x8005; 
// CRC-16 = X16 + X15 + X2 + X0 
//const INT16U cnCRC_CCITT = 0x1021; 
// CRC-CCITT = X16 + X12 + X5 + X0����˵��� 16 λ CRC ����ʽ����һ��Ҫ�� 

//const INT32U cnCRC_32 = 0x04C10DB7; 
// CRC-32 = X32 + X26 + X23 + X22 + X16 + X11 + X10 + X8 + X7 + X5 + X4 + X2 + X1 + X0 

//unsigned long Table_CRC[256]; // CRC �� 

extern unsigned long gTable_Crc32[256];

// ���� 32 λ CRC-32 ֵ 
unsigned long CRC_32( unsigned char * aData, unsigned long aSize ) 
{ 
    unsigned long i; 
    unsigned long nAccum = 0; 
    //    unsigned long crc32Table[256];

    //    CRCBuildTable32( CRC32_CRC32 , crc32Table ); 
    for ( i = 0; i < aSize; i++ ) 
        nAccum = ( nAccum << 8 ) ^ gTable_Crc32[( nAccum >> 24 ) ^ *aData++]; 
    return nAccum; 
} 

unsigned long CRC_32_2( unsigned char * aData, unsigned long aSize, unsigned long nAccum ) 
{ 
    unsigned long i; 

    for ( i = 0; i < aSize; i++ ) 
    {
        if(i%200000 == 0)
            printf(".");
        nAccum = ( nAccum << 8 ) ^ gTable_Crc32[( nAccum >> 24 ) ^ *aData++]; 
    }
    return nAccum; 
} 


// ��� BUFFER CRC У���Ƿ� �д���.���� ��� 4 �� BYTE �� CRC32 ��У��ֵ.
// CMY:
// ��CRCУ��ʧ�ܣ�����0
// ��CRCУ��ɹ�������CRCֵ
uint32 CRC_32CheckBuffer( unsigned char * aData, unsigned long aSize )
{
    uint32 crc = 0;
    int i=0;
    if( aSize <= 4 )
    {
        return 0;
    }
    aSize -= 4;

    // CMY:���ǵ�4Bytes����
    for(i=3; i>=0; i--)
        crc = (crc<<8)+(*(aData+aSize+i));

    if( CRC_32( aData , aSize ) == crc )
        return crc;

    return 0;
}

/////////////////////////////////////////////////////////////

