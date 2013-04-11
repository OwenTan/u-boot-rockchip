#ifndef _PARAMETER_H_
#define _PARAMETER_H_

#include "getmtdpart.h"

#define PARAMETER_NUM		5			    // parameter�ļ��ı��ݸ���
#define MAX_LINE_CHAR		1024	        // Parameters�ж��Line��ɣ�����ÿ��Line���ռ1024 Bytes
#define MAX_LOADER_PARAM	(32*512)		// Parameters��ռ�����Sector��(��tag��length��crc��)
#define PARM_TAG			0x4D524150
#define MAGIC_CODE			0x00280028
#define EATCHAR(x, c) for (; *(x) == (c); (x)++) ; // ȥ���ַ���x�����Ϊc���ַ�


typedef struct tagLoaderParam
{
	uint32	tag;
	uint32	length;
	char	parameter[1];
//	char*	parameter;
	uint32	crc;
}LoaderParam, *PLoaderParam;

typedef struct tagBootInfo
{
	uint32 magic_code;
	uint16 machine_type;
	uint32 kernel_load_addr;
	uint32 uboot_mask;
	
	cmdline_mtd_partition cmd_mtd;
	int index_misc;
	int index_kernel;
	int index_boot;
	int index_recovery;
	int index_system;
	int index_cache;
	int index_kpanic;
	int index_user_data;
#if 0	
	uint32 misc_offset;
	uint32 misc_size;
	
	uint32 kernel_offset;
	uint32 kernel_size;	
	
	uint32 boot_offset;		// ��SectorΪ��λ
	uint32 boot_size;
	
	uint32 recovery_offset;	
	uint32 recovery_size;

	uint32 system_offset;
	uint32 system_size;

	uint32 cache_offset;
	uint32 cache_size;

    uint32 kpanic_offset;
	uint32 kpanic_size;
	
	uint32 user_data_offset;
	uint32 user_data_size;
#endif
    char uboot_autoboottime[5];
	char bootmedia[10];
	char ethaddr[18];  
	char ipaddr[18];
	char serverip[18];
	char bootcmd[128];
	char cmd_line[MAX_LINE_CHAR];
}BootInfo, *PBootInfo;

extern BootInfo			gBootInfo;
extern uint32			gLoaderTlb[];
extern uint32			gParamBuffer[];
extern uint32			parameter_lba;

extern int32 rk30_getParameter(void);
extern int32 rk30_bootParameter(void);
extern int rk30_get_system_parameter(int32 *flashAddr,int32 *flashSize);
#endif
