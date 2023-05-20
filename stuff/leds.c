// leds.c - individuele leds aansturen op de mignonette

/* vim: set sw=4 sts=4 cindent: */

#include "leds.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

/*
 * led matrix:
 * columns:    PORTD & 0b01111111
 * red rows:   PORTB & 0b11111000
 * green rows: PORTC & 0b00111110
 */

void led_setup( void ) {
	DDRD |= 0b01111111;
	DDRB |= 0b11111000;
	DDRC |= 0b00111110;
}



void setup_led( void ) {
    // led array columns
    DDRD |= 0x7f;        // 0b01111111

    // led array red rows
    DDRB |= 0xf8;        // 0b11111000

    // led array green rows
    DDRC |= 0x3e;        // 0b00111110
}

void setup_buttons( void ) {
}

void setup_speaker( void ) {
}



int main() {

    uint8_t col;

    setup_led();
    setup_buttons();
    setup_speaker();


    while ( 1 )  {
        for( col = 0; col < 7; col++ ) {
            PORTD = ( 1 << col );
            PORTB = red_leds[ col ];
	    _delay_us( 1000 );
	    PORTB = 0;
            PORTC = green_leds[ col ];
            _delay_us( 2000 );
  	    PORTC = 0;
        }
    }


    return 0;
}


