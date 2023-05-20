/* Generated banner text */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "mignonette.h"
#include "tekst.h"

int main() {

    // setup
   
    uint8_t col;
    uint8_t rep;

    DDRC |= 0xff;	// bit 1..5 0111 1110
    DDRD |= 0x7f;	// bit 0..6 0111 1111
    DDRB |= 0xf8;	// bit 3..7 1111 1000

    PORTB = 0x01;

    // loop

    for ( ;; ) {

	for ( col = 0; col < banner_length; col++ ) {

	    for ( rep = 0; rep < 15; rep++ ) {

		PORTD = 0x40;
		PORTB = ( pgm_read_byte( &( banner_text[ col + 0 ] ) ) << 3 );
		_delay_ms(10);

		PORTD = 0x20;
		PORTB = ( pgm_read_byte( &( banner_text[ col + 1 ] ) ) << 3 );
		_delay_ms(10);

		PORTD = 0x10;
		PORTB = ( pgm_read_byte( &( banner_text[ col + 2 ] ) ) << 3 );
		_delay_ms(10);

		PORTD = 0x08;
		PORTB = ( pgm_read_byte( &( banner_text[ col + 3 ] ) ) << 3 );
		_delay_ms(10);

		PORTD = 0x04;
		PORTB = ( pgm_read_byte( &( banner_text[ col + 4 ] ) ) << 3 );
		_delay_ms(10);

		PORTD = 0x02;
		PORTB = ( pgm_read_byte( &( banner_text[ col + 5 ] ) ) << 3 );
		_delay_ms(10);

		PORTD = 0x01;
		PORTB = ( pgm_read_byte( &( banner_text[ col + 6 ] ) ) << 3 );
		_delay_ms(10);
	    }

	}

    } 

    return 0;
}


