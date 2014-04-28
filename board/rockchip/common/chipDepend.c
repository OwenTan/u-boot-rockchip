
#include    "config.h"
#include   <asm/io.h>

#define     DELAY_ARM_FREQ      50
#define     ASM_LOOP_INSTRUCTION_NUM     4
#define     ASM_LOOP_PER_US    (DELAY_ARM_FREQ/ASM_LOOP_INSTRUCTION_NUM) //
 
/***************************************************************************
��������:��ʱ
��ڲ���:us��
���ڲ���:
���ú���:
***************************************************************************/
extern uint32 Timer0Get100ns( void );
void DRVDelayUs(uint32 count)
{
#if 0
    uint32 tmp;
    uint32 TimerEnd = Timer0Get100ns() + count * 13;
    tmp =  ASM_LOOP_PER_US;//15;
    if(tmp)
        tmp *= count;
    else
        tmp = 1;

    while (--tmp) 
    {
        if(Timer0Get100ns() > TimerEnd)
            break;
    }
#else
	__udelay(count);
#endif
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
        for (i=0; i<8; i++);
}


/***************************************************************************
��������:��ʱ
��ڲ���:ms��
���ڲ���:
���ú���:
***************************************************************************/
void DRVDelayMs(uint32 count)
{
    DRVDelayUs(1000*count);
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
    return (*((REG32*)PMU_SYS_REG0));
}

void ISetLoaderFlag(uint32 flag)
{
    if(*((REG32*)PMU_SYS_REG0) == flag)
        return;
    *((REG32*)PMU_SYS_REG0) = flag;
}

uint32 IReadLoaderMode(void)
{
    return (*((REG32*)PMU_SYS_REG1));
}


void FW_NandDeInit(void)
{
#ifdef RK_FLASH_BOOT_EN
    if(gpMemFun->flag == BOOT_FROM_FLASH)
    {
        FtlDeInit();
        FlashDeInit();
    }
#endif
}
int32 SCUSelSDClk(uint32 sdmmcId, uint32 div)
{
    if((div == 0))//||(sdmmcId > 1))
    {
        return (-1);
    }
#if 1
    if(0 == sdmmcId)
    {
        g_cruReg->cru_clksel_con[11] = (0x3Ful<<16)|(div-1)<<0;
    }
    else if(1 == sdmmcId)
    {
        g_cruReg->cru_clksel_con[12] = (0x3Ful<<16)|(div-1)<<0;
    }
    else    //emmc
    {
        //RkPrintf("SCUSelSDClk 2 %d\n",div);
        g_cruReg->cru_clksel_con[12] = (0xFFul<<24)|(div-1)<<8 |(1<<14);//emmc use gerenall pll
    }
#endif
    return(0);
}






