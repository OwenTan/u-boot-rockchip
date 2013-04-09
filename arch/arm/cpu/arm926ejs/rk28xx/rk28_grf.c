/******************************************************************/
/*	Copyright (C)  ROCK-CHIPS FUZHOU . All Rights Reserved.  	  */
/*******************************************************************
File 	:	grf.c
Desc 	:	GPIO����ѡ��IOMUX����֤ÿ��PIN��ֻ��һ��������ڿ������� 
Author 	:  	yangkai
Date 	:	2009-01-05
Notes 	:

********************************************************************/
#define IN_GRF
#include  <asm/arch/rk28_drivers.h>
#ifdef DRIVERS_GRF

/*----------------------------------------------------------------------
Name	: IOMUXSetI2C0
Desc	: I2C0���pin������
Params	: type:IOMUX_I2C0--���ó�I2Cʹ��
			   IOMUX_I2C0_GPIO--���ó�GPIO
Return	: 
Notes	:
----------------------------------------------------------------------*/
void IOMUXSetI2C0(eIOMUX_I2C0 type)
{
	if(type)
	{
		g_grfReg->IOMUX_A_CON &= ~IOMUXA_GPIO1_A45;
	}
	else
	{
		g_grfReg->IOMUX_A_CON |= IOMUXA_GPIO1_A45;
	}
}

/*----------------------------------------------------------------------
Name	: IOMUXSetI2C1
Desc	: I2C0���pin������
Params	: type:IOMUX_I2C1_GPIO--���ó�GPIO
			   IOMUX_I2C1_UART1--���ó�UART1�ź���
			   IOMUX_I2C1	  --���ó�I2C�ź���
Return	: 
Notes	:
----------------------------------------------------------------------*/
void IOMUXSetI2C1(eIOMUX_I2C1 type)
{
	g_grfReg = (pGRF_REG)REG_FILE_BASE_ADDR;
	uint32 config = g_grfReg->IOMUX_A_CON;
	config &= ~(0x3<<28);
	config |= (type<<28);
	g_grfReg->IOMUX_A_CON = config;	
}

/*----------------------------------------------------------------------
Name	: GRFInit
Desc	: GRFģ���ʼ��
Params  : 
Return  : 
Notes   : ϵͳ����ʱ��ʼ��IOMUX
----------------------------------------------------------------------*/
void GRFInit(void)
{
    g_grfReg = (pGRF_REG)REG_FILE_BASE_ADDR;
    
}

/*----------------------------------------------------------------------
Name	: IOMUXSetSPI0
Desc	: ����SPI1��عܽ�
Params  : type:IOMUX_SPI0_GPIO  --���ó�GPIO
               IOMUX_SPI0_CSN0  --���ó�SPI1+CSN0�ź���
               IOMUX_SPI0_CSN1  --���ó�SPI1+CSN0�ź���
               IOMUX_SPI0_SDMMC0--������عܽ�ΪSDMMC0�ź���
               IOMUX_SPI0_SDMMC1--������عܽ�ΪSDMMC1�ź���
Return  : 
Notes   :
----------------------------------------------------------------------*/
void IOMUXSetSPI0(eIOMUX_SPI0 type)
{
    uint32 config = g_grfReg->IOMUX_A_CON;
    switch(type)
    {
        case IOMUX_SPI0_CSN0:
            config &= ~0x3000f;
            config |= 0x04;/////////
            break;
        case IOMUX_SPI0_CSN1:
            config &= ~0x3000f;
            config |= 0x10004;
            break;
        case IOMUX_SPI0_SDMMC0:
            config &= ~0x000f;
            config |= 0x0a;
            break;
        case IOMUX_SPI0_SDMMC1:
            config &= ~0x30000;
            config |= 0x20000;
            break;
        case IOMUX_SPI0_GPIO:
            config &= ~0x3000f;
            break;
        default:
            break;
    }
    g_grfReg->IOMUX_A_CON = config;
}

#endif
