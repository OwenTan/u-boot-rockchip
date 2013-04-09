/******************************************************************/
/*	Copyright (C)  ROCK-CHIPS FUZHOU . All Rights Reserved.  	  */
/*******************************************************************
File    :   driver.h
Desc    :   �������������������ͷ�ļ���
            ����.c�ļ���Ӧֻ������һ��ͷ�ļ���
Author  :   rk28 driver
Date    :   2008-11-18
Notes   :   

********************************************************************/
#ifndef _RK29_DRIVERS_H
#define _RK29_DRIVERS_H

/********************************************************************
���¶�����ģ�鿪��ʹ��, �ж���ʹ����Ӧģ��
********************************************************************/

//#define     DRIVERS_TIMER       //���嶨ʱ��ģ��ʹ��
//#define     DRIVERS_SCU         //����SCUģ��ʹ��, ��Ҫ��ʱ����͵�Դ��Ĺ���
#define     DRIVERS_UART        //���崮��ģ��ʹ��, ���������MODEMģʽ
//#define     DRIVERS_GPIO        //����GPIOģ��ʹ��
//#define     DRIVERS_SPI         //����SPIģ��ʹ��, ��������ģʽ
//#define     DRIVERS_I2C         //����I2Cģ��ʹ��, ��������ģʽ
//#define     DRIVERS_GRF         //����ͨ�üĴ����ļ�(General Register File)ģ��ʹ��, ��Ҫ��IOMUX��IO����
//#define     DRIVERS_ADC        // ����ADC ģ��ʹ��
/********************************************************************
**                 common head files                                *
********************************************************************/
#include    "rk29_debug.h"
#include    "rk29_typedef.h"
#include    "rk29_memmap.h"
#include    "rk29_drivers_delay.h"

/********************************************************************
**                  drivers head files                      *
********************************************************************/
//#include    "rk28_timer.h"
//#include    "rk28_grf.h"
//#include    "rk28_scu.h"
//#include    "rk28_spi.h"
//#include    "rk28_i2c.h"
//#include    "rk28_gpio.h"
#include    "rk29_uart.h"
//#include    "rk29_api_scu.h"
//#include    "rk28_adc.h"

#endif   /* _RK29_DRIVERS_H */
