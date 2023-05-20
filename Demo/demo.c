/*
 * demo.c - Mignonette "demo" app
 * 
 * Copyright 2011 Alex van Denzel, CC-BY-SA
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

                        /*    _____...    _____...    _____...    _____...    _____...    _____...    _____... */
uint8_t red_leds[ 7 ]   = { 0b11111000, 0b11111000, 0b11011000, 0b11011000, 0b11011000, 0b11111000, 0b11111000 };
                        /*    .._____.    .._____.    .._____.    .._____.    .._____.    .._____.    .._____. */
uint8_t green_leds[ 7 ] = { 0b00000000, 0b00011100, 0b00011100, 0b00011100, 0b00011100, 0b00011100, 0b00000000 };


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


