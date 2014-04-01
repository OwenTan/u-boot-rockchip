
/******************************************************************/
/*  Copyright (C)  ROCK-CHIPS FUZHOU . All Rights Reserved.       */
/*******************************************************************
File     :  MM.c
Desc   :    �ڴ�����ļ�
Author :  nizy
Date     :  2008-11-20
Notes  :
$Log     :  Revision 1.00  2009/02/14   nizy
********************************************************************/


#include <asm/arch/drivers.h>
#include <common.h>


#ifndef CONFIG_SYS_DCACHE_OFF
void enable_caches(void)
{
	/* Enable D-cache. I-cache is already enabled in start.S */
	dcache_enable();
}
#endif

void MMUDeinit(uint32 adr)
{

#ifndef CONFIG_SYS_L2CACHE_OFF
	v7_outer_cache_disable();
#endif
#ifndef CONFIG_SYS_DCACHE_OFF
	flush_dcache_all();
#endif
#ifndef CONFIG_SYS_ICACHE_OFF
	invalidate_icache_all();
#endif

#ifndef CONFIG_SYS_DCACHE_OFF
	dcache_disable();
#endif

#ifndef CONFIG_SYS_ICACHE_OFF
	icache_disable();
#endif

}

uint32 CacheFlushDRegion(uint32 adr, uint32 size)
{
#ifndef CONFIG_SYS_DCACHE_OFF
	flush_cache(adr, size);
#endif
}

#ifdef CONFIG_ARCH_CPU_INIT
int arch_cpu_init(void)
{
	ChipTypeCheck();
	return 0;
}
#endif


#ifdef CONFIG_DISPLAY_CPUINFO
int print_cpuinfo(void)
{
	printf("CPU:\t\tRK32xx\n");
	printf("arm pll:\t%d\n", rk_get_arm_pll());
	printf("general pll:\t%d\n", rk_get_general_pll());
	printf("codec pll:\t%d\n", rk_get_codec_pll());
	printf("ddr pll:\t%d\n", rk_get_ddr_pll());
	printf("new pll:\t%d\n", rk_get_new_pll());
	return 0;
}
#endif


void reset_cpu(ulong ignored)
{
	SoftReset();
}


