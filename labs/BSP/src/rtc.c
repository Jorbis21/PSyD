#include <s3c44b0x.h>
#include <s3cev40.h>
#include <rtc.h>

extern void isr_TICK_dummy( void );
uint32 dectoBCD(uint32 num){
	  uint32 ones = 0;
	  uint32 tens = 0;
	  uint32 temp = 0;
	  ones = num%10;
	  temp = num/10;
	  tens = temp<<4;
	  return (tens + ones);
}

uint32 BCDtodec(uint32 BCD){
	uint32 result;
	    result=(BCD>>12)*1000;
	    result+=((BCD>>8)&0x0f)*100;
	    result+=((BCD>>4)&0x0f)*10;
	    result+=((BCD)&0x0f);
	    return result;
}

void rtc_init( void )
{
    TICNT   = 0x0;
    RTCALM  = 0x0;
    RTCRST  = 0x0;
        
    RTCCON  = 0x9;
    
    BCDYEAR = 0xd;
    BCDMON  = 0x1;
    BCDDAY  = 0x1;
    BCDDATE = 0x2;
    BCDHOUR = 0x0;
    BCDMIN  = 0x0;
    BCDSEC  = 0x0;

    ALMYEAR = 0x0;
    ALMMON  = 0x0;
    ALMDAY  = 0x0;
    ALMHOUR = 0x0;
    ALMMIN  = 0x0;
    ALMSEC  = 0x0;

    RTCCON &= 0;//revisar todos los que tengan esto que soy un cazurro
}

void rtc_puttime( rtc_time_t *rtc_time )
{
    RTCCON |= 0x9;
    
    BCDYEAR = dectoBCD(rtc_time->year);
    BCDMON  = dectoBCD(rtc_time->mon);
    BCDDAY  = dectoBCD(rtc_time->mday);
    BCDDATE = dectoBCD(rtc_time->wday);
    BCDHOUR = dectoBCD(rtc_time->hour);
    BCDMIN  = dectoBCD(rtc_time->min);
    BCDSEC  = dectoBCD(rtc_time->sec);
        
    RTCCON &= 0;
}

void rtc_gettime( rtc_time_t *rtc_time )
{
    RTCCON |= 1;
    
    rtc_time->year = BCDtodec(BCDYEAR);
    rtc_time->mon  = BCDtodec(BCDMON);
    rtc_time->mday = BCDtodec(BCDDAY);
    rtc_time->wday = BCDtodec(BCDDATE);
    rtc_time->hour = BCDtodec(BCDHOUR);
    rtc_time->min  = BCDtodec(BCDMIN);
    rtc_time->sec  = BCDtodec(BCDSEC);
    if( ! rtc_time->sec ){
    	rtc_time->year = BCDtodec(BCDYEAR);
    	    rtc_time->mon  = BCDtodec(BCDMON);
    	    rtc_time->mday = BCDtodec(BCDDAY);
    	    rtc_time->wday = BCDtodec(BCDDATE);
    	    rtc_time->hour = BCDtodec(BCDHOUR);
    	    rtc_time->min  = BCDtodec(BCDMIN);
    	    rtc_time->sec  = BCDtodec(BCDSEC);
    };

    RTCCON &= 0;
}


void rtc_open( void (*isr)(void), uint8 tick_count )
{
    pISR_TICK = (uint32) isr;
    I_ISPC    = BIT_TICK;
    INTMSK   &= ~(BIT_GLOBAL | BIT_TICK);
    TICNT     = (1<<7) | tick_count;
}

void rtc_close( void )
{
    TICNT     = 0x7f;
    INTMSK   |= (1<<20);
    pISR_TICK = (uint32) isr_TICK_dummy;
}
