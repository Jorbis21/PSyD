#include <s3c44b0x.h>
#include <leds.h>

void leds_init( void )
{
    led_off(LEFT_LED);
    led_off(RIGHT_LED);
}

void led_on( uint8 led )
{
	PDATB &=~(led<<9);
}

void led_off( uint8 led )
{
	PDATB |= led<<9;
}

void led_toggle( uint8 led )
{
    uint8 st = led_status(led);
    if(st == 0){
    	led_on(led);
    }
    else{
    	led_off(led);
    }
}

uint8 led_status( uint8 led )
{
	if(PDATB&(1<<led+8)){
		return 0;
	}
	else{
		return 1;
	}
}
