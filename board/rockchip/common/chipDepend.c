
#include    "config.h"
#include   <asm/io.h>
    
#define     DELAY_ARM_FREQ      50
#define     ASM_LOOP_INSTRUCTION_NUM     4
#define     ASM_LOOP_PER_US    (DELAY_ARM_FREQ/ASM_LOOP_INSTRUCTION_NUM)	//
    
/***************************************************************************
��������:��ʱ
��ڲ���:us��
���ڲ���:
���ú���:
***************************************************************************/ 
extern uint32 Timer0Get100ns(void);
void DRVDelayUs(uint32 count) 
{
	
#if 0
	    uint32 tmp;
	uint32 TimerEnd = Timer0Get100ns() + count * 13;
	tmp = ASM_LOOP_PER_US;	//15;
	if (tmp)
		tmp *= count;
	
	else
		tmp = 1;
	 while (--tmp)
		 {
		if (Timer0Get100ns() > TimerEnd)
			break;
		}
	
#else	/*  */
	    __udelay(count);
	
#endif	/*  */
} 

/***************************************************************************
��������:��ʱ
��ڲ���:
���ڲ���:
���ú���:
***************************************************************************/ 
void Delay100cyc(uint16 count) 
{
	uint16 i;
	 while (count--)
		for (i = 0; i < 8; i++) ;
}

  
/***************************************************************************
��������:��ʱ
��ڲ���:ms��
���ڲ���:
���ú���:
***************************************************************************/ 
void DRVDelayMs(uint32 count) 
{
	DRVDelayUs(1000 * count);
}   

/***************************************************************************
��������:��ʱ
��ڲ���:s��
���ڲ���:
���ú���:
***************************************************************************/ 
void DRVDelayS(uint32 count) 
{
	while (count--)
		DRVDelayMs(1000);
}

  
//����Loader�����쳣����
//ϵͳ������ָ����sdramֵΪ�ñ�־���������ɽ���rockusb
//ϵͳ����ʧ�ܱ�־
    uint32 IReadLoaderFlag(void) 
{
	return (*((REG32 *) PMU_SYS_REG0));
}

 void ISetLoaderFlag(uint32 flag) 
{
	if (*((REG32 *) PMU_SYS_REG0) == flag)
		return;
	*((REG32 *) PMU_SYS_REG0) = flag;
}

 uint32 IReadLoaderMode(void)
{
	return (*((REG32 *) PMU_SYS_REG1));
}

        uint32 GetMmcCLK(void)
{
	
	    //return (GetAHBCLK());
	    return rk_get_general_pll() / (1000 * 2);	//emmc automic divide setting freq to 1/2,for get the right freq ,we divide this freq to 1/2
}

  void sdmmcGpioInit(uint32 ChipSel) 
{
	writel(0xffffaaaa, 0xff770020);
	writel(0x000c0008, 0xff770024);
	writel(0x003f002a, 0xff770028);
}   void FW_NandDeInit(void) 
{
	
#ifdef RK_FLASH_BOOT_EN
	    if (gpMemFun->flag == BOOT_FROM_FLASH)
		 {
		FtlDeInit();
		FlashDeInit();
		}
	
#endif	/*  */
#ifdef RK_SDMMC_BOOT_EN
	    SdmmcDeInit();
	
#endif	/*  */
}

 void EmmcPowerEn(uint8 En) 
{
	
// TODO: EMMC ��Դ����
	    if (En)
		 {
		g_EMMCReg->SDMMC_PWREN = 1;	// power enable
		g_EMMCReg->SDMMC_RST_n = 1;	// reset off
		}
	
	else
		 {
		g_EMMCReg->SDMMC_PWREN = 0;	// power disable
		g_EMMCReg->SDMMC_RST_n = 0;	//reset on
		}
}

 void SDCReset(uint32 sdmmcId) 
{
	uint32 data = g_cruReg->CRU_SOFTRST_CON[8];
	data = ((1 << 16) | (1)) << (sdmmcId + 1);
	g_cruReg->CRU_SOFTRST_CON[8] = data;
	DRVDelayUs(100);
	data = ((1 << 16) | (0)) << (sdmmcId + 1);
	g_cruReg->CRU_SOFTRST_CON[8] = data;
	DRVDelayUs(200);
	EmmcPowerEn(1);
}  int32 SCUSelSDClk(uint32 sdmmcId, uint32 div) 
{
	if ((div == 0))	//||(sdmmcId > 1))
	{
		return (-1);
	}
	
#if 1
	    if (0 == sdmmcId)
		 {
		g_cruReg->CRU_CLKSEL_CON[11] = (0x3Ful << 16) | (div - 1) << 0;
		}
	
	else if (1 == sdmmcId)
		 {
		g_cruReg->CRU_CLKSEL_CON[12] = (0x3Ful << 16) | (div - 1) << 0;
		}
	
	else			//emmc
	{
		
		    //RkPrintf("SCUSelSDClk 2 %d\n",div);
		    g_cruReg->CRU_CLKSEL_CON[12] = (0xFFul << 24) | (div - 1) << 8 | (1 << 14);	//emmc use gerenall pll
	}
	
#endif	/*  */
	    return (0);
}

 
//mode=1  changemode to normal mode;
//mode=0  changemode to boot mode
    int32 eMMC_changemode(uint8 mode) 
{
	
#ifdef RK_SDMMC_BOOT_EN    
	    eMMC_SetDataHigh();
	
#endif	/*  */
}

 
/*----------------------------------------------------------------------
Name	: IOMUXSetSDMMC1
Desc	: ����SDMMC1��عܽ�
Params  : type: IOMUX_SDMMC1 ���ó�SDMMC1�ź���
                IOMUX_SDMMC1_OTHER���óɷ�SDMMC1�ź���
Return  : 
Notes   : Ĭ��ʹ��4�ߣ���ʹ��pwr_en, write_prt, detect_n�ź�
----------------------------------------------------------------------*/ 
void IOMUXSetSDMMC(uint32 sdmmcId, uint32 Bits) 
{
	
// TODO:SDMMC IOMUX 
}     
