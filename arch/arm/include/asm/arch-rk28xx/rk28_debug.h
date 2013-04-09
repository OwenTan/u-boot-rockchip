/******************************************************************/
/*	Copyright (C)  ROCK-CHIPS FUZHOU . All Rights Reserved.  	  */
/*******************************************************************
File 	:	define.c
Desc 	:	���������й��ô���
Author 	:  	yangkai
Date 	:	2008-11-12
Notes 	:   

********************************************************************/
#ifndef _HW_DEFINE_H
#define _HW_DEFINE_H

#define DEBUG_DRIVER//�쳣��鿪��
#define DEBUG_MSG   //��Ϣ�������

#ifdef DEBUG_DRIVER


#define Assert(cond,msg,num)                \
    do{                                     \
        if (!(cond))                        \
        {                                   \
            printf("%-45s %12d  %s  %d\n", msg, num, __FILE__,__LINE__);\
            abort();                        \
        }                                   \
    }while(0);

/*

#define Assert(cond,msg,num)                \
    do{                                     \
        if (!(cond))                        \
        {                                   \
            EdbgOutputDebugString("%s %s %d  %s  %d\n", #cond, msg, num, __FILE__,__LINE__);\
            abort();                        \
        }                                   \
    }while(0);
*/


#else

#define Assert(cond,msg,num)

#endif


#ifdef  DEBUG_MSG



#define PRINTF(...)     \
    do\
    {\
        printf(__VA_ARGS__);\
        printf("  %s:%d\n",__FILE__,__LINE__);\
    }while(0);

/*



#define PRINTF(...)     \
            do\
            {\
                EdbgOutputDebugString(__VA_ARGS__);\
                EdbgOutputDebugString("  %s:%d\n",__FILE__,__LINE__);\
            }while(0);

*/

#else

#define PRINTF(...)

#endif

#endif
