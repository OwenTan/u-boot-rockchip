/********************************************************************************
*********************************************************************************
		COPYRIGHT (c)   2001-2012 BY ROCK-CHIP FUZHOU
				--  ALL RIGHTS RESERVED  --

File Name:	    config.h
Author:		    YIFENG ZHAO
Created:        2012-02-07
Modified:       ZYF
Revision:		1.00
********************************************************************************
********************************************************************************/
#ifndef _CONFIG_H
#define _CONFIG_H

//#define FPGA_EMU

//ƽ̨����
#define     RK28XX   0x0
#define     RK2828   0x2
#define     RK29XX   0x10
#define     RK292X   0x12
#define     RK30XX   0x20
#define     RK31XX   0x30
#define     RK32XX   0x40
#define     RK_ALL   0xFF

#define     PALTFORM    RK30XX

//#define     LINUX_LOADER
#define     SECURE_BOOT_ENABLE
#define     SECURE_BOOT_ENABLE_ALWAY
#define     SECURE_BOOT_LOCK
//#define     ERASE_DRM_KEY_EN
//#define   SECURE_BOOT_TEST
#define     MALLOC_DISABLE
//#define   INSTANT_BOOT_EN
#define     LOAD_OEM_DATA
//#define     RK_LOADER_FOR_FT
#define     DRIVERS_UART
#define     DRIVERS_NAND   
#define 	RK28_FPGA		0
#define     RK_SDMMC_BOOT_EN
//#define     L2CACHE_ENABLE
//define		DRIVERS_MMU
#define __packed __attribute__((packed))
#define __align(x) __attribute__ ((aligned(x)))

//ģ������
#ifdef RK_SPI_BOOT_EN
#define DRIVERS_SPI
#endif

//ϵͳ����
#define     MAX_COMBINATION_KEY     6

//��ͷ�ļ�
#define Assert(cond,msg,num)
#define min(X, Y)				\
	({ typeof(X) __x = (X);			\
		typeof(Y) __y = (Y);		\
		(__x < __y) ? __x : __y; })

#define max(X, Y)				\
	({ typeof(X) __x = (X);			\
		typeof(Y) __y = (Y);		\
		(__x > __y) ? __x : __y; })

#define MIN(x, y)  min(x, y)
#define MAX(x, y)  max(x, y)
#ifdef DEBUG_FLASH
#define     PRINTF  printf
#else
#define     PRINTF(...)
#endif
#define PRINT_I PRINTF
#define PRINT_D PRINTF
#define PRINT_E PRINTF
#define PRINT_W PRINTF
#define RkPrintf PRINTF

#include <common.h>
#include <asm/arch/rkplat.h>

//ƽ̨�޹�ͷ�ļ�
#include    "storage.h"

//ƽ̨���ͷ�ļ�
#include    "chipDepend.h"

//ϵͳ���ͷ�ļ�
#include    "parameter.h"



#ifdef  DRIVERS_SPI
#include    "../common/spi/SpiFlash.h"
#include    "../common/spi/SpiBoot.h"
#endif

#ifdef CONFIG_PL330_DMA
#include    <api_pl330.h>
#endif


#define	BOOT_ONLY           //����ֻ�����м����������, �������������ɲ��Ե��ļ�ϵͳ
#define DEBUG				// ����DEBUGģʽ������ӡ��Ϣ���������

#define USE_RECOVER		// cmy: ��ֹRecover����(�Զ��޸�kernel/boot/recovery)
#define USE_RECOVER_IMG

//#define DRIVERS_USB_APP

#ifndef BOOT_ONLY
#define	IN_SYSTEM           //Ƕ�뵽ϵͳ���ʱ����
#define	OS_FILE             //����OS�µ��ļ�ϵͳ
#endif

//#include 			"../common/dma.h"
extern uint32 SecureBootEn;
extern uint32 SecureBootCheckOK;
extern uint32 g_BootRockusb;
extern uint32  SecureBootLock;
extern uint32  SecureBootLock_backup;


// by cmy
#define SYS_LOADER_REBOOT_FLAG   0x5242C300  //��24��TAG,��8λ�Ǳ��
#define SYS_KERNRL_REBOOT_FLAG   0xC3524200  //��24��TAG,��8λ�Ǳ��
#define SYS_LOADER_ERR_FLAG      0X1888AAFF

enum {
    BOOT_NORMAL=                  0,
    BOOT_LOADER,     /* enter loader rockusb mode */
    BOOT_MASKROM,    /* enter maskrom rockusb mode*/
    BOOT_RECOVER,    /* enter recover */
    BOOT_NORECOVER,  /* do not enter recover */
    BOOT_WINCE,      /* FOR OTHER SYSTEM */
    BOOT_WIPEDATA,   /* enter recover and wipe data. */
    BOOT_WIPEALL,    /* enter recover and wipe all data. */
    BOOT_CHECKIMG,   /* check firmware img with backup part(in loader mode)*/
    BOOT_FASTBOOT,   
    BOOT_SECUREBOOT_DISABLE,  
    BOOT_CHARGING,
    BOOT_MAX         /* MAX VALID BOOT TYPE.*/
};

uint32 RkldTimerGetTick( void );
void loader_tag_set_timer( __u32 t0 , __u32 t1);
extern	void UsbIsr(void);


#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

