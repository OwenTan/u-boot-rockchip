 
#ifndef _CHIP_DEPEND_H
#define _CHIP_DEPEND_H
    
//#include "gpio_oper.h"
#include "configs/rkchip.h"
extern uint8 ChipType;
 
//typedef volatile unsigned int       REG32;
     
//����Loader�����쳣����
#define SYS_LOADER_ERR_FLAG      0X1888AAFF 
    
//extern void ModifyUsbVidPid(USB_DEVICE_DESCRIPTOR * pDeviceDescr);
extern bool UsbPhyReset(void);
extern void FlashCsInit(void);
void ISetLoaderFlag(uint32 flag);
uint32 IReadLoaderFlag(void);
 
#endif	/*  */
