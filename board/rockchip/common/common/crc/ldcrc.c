#define     IN_CRC32
#include    "../../armlinux/config.h"

//#pragma arm section code = "LOADER2"

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
    //unsigned long startTime;
    //unsigned long endTime;
    //startTime = RkldTimerGetTick();
    for ( i = 0; i < aSize; i++ ) 
        nAccum = ( nAccum << 8 ) ^ gTable_Crc32[( nAccum >> 24 ) ^ *aData++]; 
    //endTime = RkldTimerGetTick();
    //printf ("CRC_32 times  %d ,aSize = %x\n", endTime - startTime,aSize);
#if 0
    startTime = RkldTimerGetTick();
    ftl_memcpy((void*)0x64000000,(void*)0x63000000,0x1000000);
    endTime = RkldTimerGetTick();
    printf ("memcpy  times  %d ,aSize = %x\n", endTime - startTime,aSize);
    
    startTime = RkldTimerGetTick();
    ftl_memcpy((void*)0x64000000,(void*)0x62000000,0x1000000);
    endTime = RkldTimerGetTick();
    printf ("memcpy times  %d ,aSize = %x\n", endTime - startTime,aSize);

{
    uint32 *pfrom =  (uint32 *)0x62000000;
    uint32 *pto =  (uint32 *)0x64000000;
    startTime = RkldTimerGetTick();
    for(i=0;i<0x1000000/4;i++)
    {
        *pto++ = *pfrom++ ;
    }
    endTime = RkldTimerGetTick();
    printf ("memcpy times  %d ,aSize = %x\n", endTime - startTime,aSize);

    pfrom =  (uint32 *)0x63000000;
    pto =  (uint32 *)0x64000000;
    startTime = RkldTimerGetTick();
    for(i=0;i<0x1000000/4;i++)
    {
        *pto++ = *pfrom++ ;
    }
    endTime = RkldTimerGetTick();
    printf ("memcpy times  %d ,aSize = %x\n", endTime - startTime,aSize);
        
}
#endif
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
    //return 1;
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
//#pragma arm section code
