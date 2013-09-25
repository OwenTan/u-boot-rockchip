/******************************************************************/
/*   Copyright (C) 2007 ROCK-CHIPS FUZHOU . All Rights Reserved.  */
/*******************************************************************
File :  hw_memmap.h
Desc :  memory map address/device base address

Author : huangxinyu
Date : 2007-05-30
Notes :

$Log: hw_memmap.h,v $
Revision 1.2  2011/01/27 03:43:27  Administrator
*** empty log message ***

Revision 1.1  2011/01/14 10:03:10  Administrator
*** empty log message ***

Revision 1.1  2011/01/07 03:28:04  Administrator
*** empty log message ***

Revision 1.5  2007/11/10 04:24:12  Huangxinyu
�����޸�

Revision 1.4  2007/10/15 09:04:01  Huangxinyu
����RK27�ύ�޸�driver

Revision 1.3  2007/10/08 02:38:41  Lingzhaojun
��Ӱ汾�Զ�ע�ͽű�

*********************************************************************/
#ifndef _HW_MEMMAP_H
#define _HW_MEMMAP_H
#ifdef LINUX
#include <mach/rk29_iomap.h>
#include "../FTL_OSDepend.h"
#define SDMMC0_BASE_ADDR RK29_SDMMC0_PHYS//ioremap(RK29_SDMMC0_PHYS, 0x4000)//
#define SDMMC1_BASE_ADDR RK29_SDMMC1_PHYS//ioremap(RK29_SDMMC1_PHYS, 0x4000)//
#define EMMC_BASE_ADDR   RK29_EMMC_PHYS//ioremap(RK29_EMMC_PHYS, 0x4000)//RK29_EMMC_PHYS
#define REG_FILE_BASE_ADDR RK29_GRF_REG_BASE
//#define GPIO1_BASE_ADDR  RK29_GPIO1_REG_BASE
//#define GPIO3_BASE_ADDR  RK29_GPIO3_REG_BASE
//#define GPIO6_BASE_ADDR  RK29_GPIO6_REG_BASE
#else
//CPU system AXI 1
#define     CPU_AXI_BUS1_BASE_ADDR     0x10000000
#define     VCODEC_BASE_ADDR           0x10104000
#define     VIP_BASE_ADDR              0x10108000
#define     LCDC_BASE_ADDR             0x1010C000
#define     IPP_BASE_ADDR              0x10110000
#define     EBC_BASE_ADDR              0x10114000
#define     I2S_BASE_ADDR_8CH          0x10118000
#define     I2S_BASE_ADDR_2CH          0x1011C000
#define     I2S_BASE_ADDR       I2S_BASE_ADDR_2CH
#define     SPDIF_BASE_ADDR            0x1011E000
#define     GPU_BASE_ADDR              0x10120000
#define     SDRAMC_BASE_ADDR           0x10124000

//CPU AXI 1 APB
#define     CRU_BASE_ADDR              0x20000000
#define     PMU_BASE_ADDR              0x20004000
#define     REG_FILE_BASE_ADDR         0x20008000
#define     RTC_BASE_ADDR              0x2000C000
#define     eFUSE_BASE_ADDR            0x20010000
#define     TZPC_BASE_ADDR             0x20014000
#define     SDMAC0_BASE_ADDR           0x20018000
#define     DMAC0_BASE_ADDR            0x2001C000
#define     DEBUG_BASE_ADDR            0x20024000
#define     I2C0_BASE_ADDR             0x2002C000
#define     UART0_BASE_ADDR            0x20030000
#define     GPIO0_BASE_ADDR            0x20034000
#define     TIMER0_BASE_ADDR           0x20038000
#define     TIMER1_BASE_ADDR           0x2003A000
#define     GPIO4_BASE_ADDR            0x2003C000
#define     GPIO6_BASE_ADDR            0x2003E000

//CPU system AXI 0
#define     CPU_AXI_BUS0_BASE_ADDR     0x15000000
#define     GIC_CPU_BASE_ADDR          0x1012C000
#define     GIC_PERI_BASE_ADDR         0x1012E000

//peri system
#define     USB_HOST_BASE_ADDR         0x10140000
#define     USB_OTG0_BASE_ADDR         0x10180000
#define     USB_OTG_BASE_ADDR_VA       USB_OTG0_BASE_ADDR
#define     USB_OTG1_BASE_ADDR         0x101C0000
#define     MAC_BASE_ADDR              0x10204000
#define     HOST_IF_ADDR               0x1020C000
#define     HSADC_BASE_ADDR            0x10210000
#define     SDMMC0_BASE_ADDR           0x10214000
#define     SDMMC1_BASE_ADDR           0x10218000
#define     EMMC_BASE_ADDR             0x1021C000
#define     PIDF_BASE_ADDR             0x10220000
#define     ARBITER0_BASE_ADDR         0x10224000
#define     ARBITER1_BASE_ADDR         0x10228000
#define     PERI_AXI_BUS0_BASE_ADDR    0x10300000
#define     NANDC_BASE_ADDR            0x10500000

//peri system APB
#define     TIMER2_BASE_ADDR           0x20044000
#define     TIMER3_BASE_ADDR           0x20048000
#define     WDT_BASE_ADDR              0x2004C000
#define     PWM_BASE_ADDR              0x20050000
#define     I2C1_BASE_ADDR             0x20054000
#define     I2C2_BASE_ADDR             0x20058000
#define     I2C3_BASE_ADDR             0x2005C000
#define     UART1_BASE_ADDR            0x20060000
#define     UART2_BASE_ADDR            0x20064000
#define     UART3_BASE_ADDR            0x20068000
#define     ADC_BASE_ADDR              0x2006C000
#define     SPI0_BASE_ADDR             0x20070000
#define     SPI_MASTER_BASE_ADDR        SPI0_BASE_ADDR
#define     SPI1_BASE_ADDR             0x20074000
#define     DMA2_BASE_ADDR             0x20078000
#define     SMC_BASE_ADDR              0x2007C000
#define     GPIO1_BASE_ADDR            0x20080000
#define     GPIO2_BASE_ADDR            0x20084000
#define     GPIO3_BASE_ADDR            0x20088000
#define     GPIO5_BASE_ADDR            0x2008C000

//MEM
#define     BOOT_ROM_ADDR              0x10100000
#define     SDRAM_ADDR                 0x60000000
#define     INTMEM_TZMA_ADDR           0x10130000
#define     SMC0_ADDR                  0x11000000
#define     SMC1_ADDR                  0x12000000  
#endif
#endif /* _HW_MEMMAP_H */
/*********************************************************************
 END OF FILE
*********************************************************************/
