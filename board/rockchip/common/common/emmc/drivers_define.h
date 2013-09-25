/******************************************************************/
/*	Copyright (C)  ROCK-CHIPS FUZHOU . All Rights Reserved.  	  */
/*******************************************************************
File    :   drivers_define.h
Desc    :   ����������������������뿪��ͷ�ļ���
Author  :   rk28 driver
Date    :   2008-11-18
Notes   :   
$Log: drivers_define.h,v $
Revision 1.1  2011/01/14 10:03:10  Administrator
*** empty log message ***

Revision 1.1  2011/01/07 03:28:04  Administrator
*** empty log message ***

********************************************************************/
#ifndef _DRIVERS_DEFINE_H
#define _DRIVERS_DEFINE_H
#define     DRIVER_ONLY
/********************************************************************
���¶�����ģ�鿪��ʹ��, �ж���ʹ����Ӧģ��
********************************************************************/
#define     DRIVERS_GRF         //����ͨ�üĴ����ļ�(General Register File)ģ��ʹ��, ��Ҫ��IOMUX��IO����
#define     DRIVERS_SCU         //����SCUģ��ʹ��, ��Ҫ��ʱ����͵�Դ��Ĺ���
#define     DRIVERS_INTC        //�����жϿ���ģ��ʹ��
#define     DRIVERS_SDRAM       //����SDRAMģ��ʹ��
#define     DRIVERS_TIMER       //���嶨ʱ��ģ��ʹ��
#define     DRIVERS_WDT         //���忴�Ź�ģ��ʹ��
#define     DRIVERS_RTC         //����RTCģ��ʹ��
#define     DRIVERS_DMA         //����DW_DMAģ��ʹ��
#define     DRIVERS_SDMMC       //����SD����MMC��ģ��ʹ��
#define     DRIVERS_EFUSE       //����eFUSEģ��ʹ��

#define     DRIVERS_UART        //���崮��ģ��ʹ��, ���������MODEMģʽ
#define     DRIVERS_I2C         //����I2Cģ��ʹ��, ��������ģʽ
#define     DRIVERS_I2S         //����I2Sģ��ʹ��
#define     DRIVERS_SPDIF
#define     DRIVERS_PWM         //����PWMģ��ʹ��
#define     DRIVERS_ADC         //����ADCģ��ʹ��
#define     DRIVERS_GPIO        //����GPIOģ��ʹ��
#define     DRIVERS_SPI         //����SPIģ��ʹ��, ��������ģʽ

#define     DRIVERS_LCD         //����LCD������ģ��ʹ��
#define     DRIVERS_LCDC        //����LCD������ģ��ʹ��, ���������޹� 
#define     DRIVERS_MMU         //����MMUģ��ʹ��
#define     DRIVERS_CACHE       //����CACHEģ��ʹ��

#define     DRIVERS_VIP         //������Ƶ���봦��(Video Input Processor)ģ��ʹ��
#define     DRIVERS_NAND        //����NAND FLASHģ��ʹ��

#define     DRIVERS_USB_APP     //����USBӦ�ò�ģ��ʹ��, ����DEVICE��HOST��������

#define     SDMMC_TEST          //


//#define     USBHOSTEN
#endif
