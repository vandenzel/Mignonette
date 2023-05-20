#!/usr/bin/perl

my @string = ();

my $row = 0;

while ( my $line = <DATA> ) {
    chomp $line;
    for ( my $col = 0; $col < length( $line ); $col++ ) {
        if ( substr( $line, $col, 1 ) eq 'o' ) { 
            $string[ $col ] |= ( 1 << $row );
            print STDERR "#";
        } else {
	    print STDERR ".";
        }
    }
    print STDERR "\n";
    $row++;
}

foreach ( @string ) {
    printf STDERR "%x", $_ / 16;
}
printf STDERR "\n";

foreach ( @string ) {
    printf STDERR "%x", $_ % 16;
}
printf STDERR "\n";

my $banner_text = join( ', ', ( map { sprintf "0x%x", $_ } @string ) );
my $banner_length = scalar @string;

open C, '>', 'tekst.h';
print C << "EOF_C";
/* Generated banner text */

uint8_t banner_length = $banner_length + 7;
uint8_t banner_text[] PROGMEM = { 
	0, 0, 0, 0, 0, 0, 0,
	$banner_text, 
	0, 0, 0, 0, 0, 0, 0
};
EOF_C
close C;

__DATA__
 oo  oooo o    o  o o  o o  o ooo  oo       o  o ooo oooo o  o o   o  oo  oo   oo  ooo   o o       o 
o    o    o    o  o o o  o o   o  o         oo o  o  o    o  o o   o   o o  o o  o o  o  o o    o   o
o oo ooo  o    o  o oo   oo    o  o oo      oooo  o  ooo  o  o o o o   o oooo oooo oooo  o o        o
o  o o    o    o  o o o  o o   o  o  o      o oo  o  o    o  o o o o   o o  o o  o o o          o   o
 oo  oooo oooo  oo  o  o o  o ooo  oo       o  o ooo oooo  oo  ooooo oo  o  o o  o o  o  o o       o 

