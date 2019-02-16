/*------------------------------------------------------------------------/
/  STM32F100 RTC control module
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2013, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/

#include "rtc.h"

#define F_LSE	32768	/* LSE oscillator frequency */


static
const uint8_t samurai[] = {31,28,31,30,31,30,31,31,30,31,30,31};

static
uint8_t rtcok;


/*------------------------------------------*/
/* Initialize RTC                           */
/*------------------------------------------*/

int rtc_initialize (void)	/* 1:RTC is available, 0:RTC is not available */
{
	uint32_t n;

	rtcok = 0;		/* RTC is not available */

	return rtcok;
}



/*------------------------------------------*/
/* Set time in UTC                          */
/*------------------------------------------*/

int rtc_setutc (uint32_t tmr)
{
	uint32_t n = 0;

	return n ? 1 : 0;
}



/*------------------------------------------*/
/* Get time in UTC                          */
/*------------------------------------------*/

int rtc_getutc (uint32_t* tmr)
{
	uint32_t t1, t2;

	return 0;
}



/*------------------------------------------*/
/* Get time in calendar form                */
/*------------------------------------------*/

int rtc_gettime (RTCTIME* rtc)
{
	uint32_t utc, n, i, d;

	return 1;
}



/*------------------------------------------*/
/* Set time in calendar form                */
/*------------------------------------------*/

int rtc_settime (const RTCTIME* rtc)
{
	uint32_t utc, i, y;

	return rtc_setutc(utc);
}


