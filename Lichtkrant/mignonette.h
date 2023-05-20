/* mignonette.h - api for mignonette hardware */

// four switches

switch_a = iopin( port_b, pin_0, input, pullup );
switch_b = iopin( port_b, pin_2, input, pullup );
switch_c = iopin( port_c, pin_0, input, pullup );
switch_d = iopin( port_d, pin_7, input, pullup );

// display of 5x7 bi-color leds



display = iomatrix( ioport 

// audio output (or digital input)

audio = iopin( port_b, pin_1, output, na );
