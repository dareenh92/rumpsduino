#ifndef __TIMER_H_
#define __TIMER_H_

#include <stdio.h>

#define TM_CTRL ((*(volatile unsigned long *)	(0x40001000)))
#define TM_IF ((*(volatile unsigned long *)	(0x40001004)))
#define TM_CNT ((*(volatile unsigned long *)	(0x40001008)))
#define TM_PR ((*(volatile unsigned long *)	(0x4000100C)))
#define TM_PS ((*(volatile unsigned long *)	(0x40001010)))
#define TM_COMCR ((*(volatile unsigned long *)	(0x40001014)))
#define TM_COMR0 ((*(volatile unsigned long *)	(0x40001018)))
#define TM_COMR1 ((*(volatile unsigned long *)	(0x4000101C)))
#define TM_COMR2 ((*(volatile unsigned long *)	(0x40001020)))
#define TM_CAPCR ((*(volatile unsigned long *)	(0x40001024)))
#define TM_CAPR0 ((*(volatile unsigned long *)	(0x40001028)))
#define TM_CAPR1 ((*(volatile unsigned long *)	(0x4000102C)))
#define TM_OCR ((*(volatile unsigned long *)	(0x40001030)))
#define TM_PWMC ((*(volatile unsigned long *)	(0x40001034)))
#define TM_DBG ((*(volatile unsigned long *)	(0x40001038)))

#endif


