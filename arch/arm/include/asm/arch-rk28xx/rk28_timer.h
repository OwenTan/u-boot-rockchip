/******************************************************************/
/*	Copyright (C)  ROCK-CHIPS FUZHOU . All Rights Reserved.  	  */
/*******************************************************************
File 	:	timer.h
Desc 	:	TIMER�ڲ��Ĵ�������
Author 	:  	yangkai
Date 	:	2008-11-11
Notes 	:   

********************************************************************/
#ifdef DRIVERS_TIMER
#ifndef _TIMER_H
#define _TIMER_H
/********************************************************************
**                            �궨��                                *
********************************************************************/

/********************************************************************
**                          �ṹ����                                *
********************************************************************/
typedef volatile struct tagTIMER_REG
{
    uint32 Timer1LoadCount;     // Load Count Register
    uint32 Timer1CurrentValue;  // Current Value Register
    uint32 Timer1ControlReg;    // Control Register
    uint32 Timer1EOI;           // End-of-Interrupt Register
    uint32 Timer1IntStatus;     // Interrupt Status Register
    uint32 Timer2LoadCount;
    uint32 Timer2CurrentValue;
    uint32 Timer2ControlReg;
    uint32 Timer2EOI;
    uint32 Timer2IntStatus;
    uint32 Timer3LoadCount;
    uint32 Timer3CurrentValue;
    uint32 Timer3ControlReg;
    uint32 Timer3EOI;
    uint32 Timer3IntStatus;
    uint32 Reserved[(0xa0 - 0x3c)/4];
    uint32 TimersIntStatus;     // Interrupt Status Register
    uint32 TimersEOI;           // End-of-Interrupt Register
    uint32 TimersRawIntStatus;  // Raw Interrupt Status Register
}TIMER_REG, *pTIMER_REG;


/********************************************************************
**                          ��������                                *
********************************************************************/
#undef EXT
#ifdef IN_TIMER
    #define EXT
#else    
    #define EXT extern
#endif    

EXT pTIMER_REG  g_timerReg;     //TIMER�Ĵ����ṹ��ָ��
EXT pFunc       g_timerIRQ[3];  //����TIMER�жϻص�����
EXT uint32      g_timerTick[3]; //����TIMER��ʱ������

/********************************************************************
**                          �ṹ����                                *
********************************************************************/
typedef enum _TIMER_NUM
{
    TIMER1 = 0,
    TIMER2,
    TIMER3,
    TIMER_MAX
}eTIMER_NUM;

/*
typedef enum _TIMER_MODE
{
    TIMER_FREE_RUNNING = 0,
    TIMER_USER_DEFINED,
    TIMER_MODE_MAX
}eTIMER_MODE;
*/

/********************************************************************
**                          ��������                                *
********************************************************************/

/********************************************************************
**                      ���⺯���ӿ�����                            *
********************************************************************/
extern uint32 TimerStart(eTIMER_NUM timerNum,              //����TIMER
                            uint32 msTick, 
                            pFunc callBack);
extern uint32 TimerStop(eTIMER_NUM timerNum);              //ֹͣTIMER
extern uint32 TimerGetCount(eTIMER_NUM timerNum);          //��ѯ��ǰ����ֵ
extern uint32 TimerAdjustCount(void);           //APB CLK�ı�ʱ����������ֵ

extern uint32 TimerMask(eTIMER_NUM timerNum);              //MASK Timer�ж�
extern uint32 TimerUnmask(eTIMER_NUM timerNum);            //UNMASK Timer�ж�


/********************************************************************
**                          �����                                *
********************************************************************/
#endif
#endif
