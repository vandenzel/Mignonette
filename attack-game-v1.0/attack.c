/*
Title:			attack.c 
Date Created:   8/26/08
Last Modified:  8/26/08
Target:			Atmel ATmega168 
Environment:	AVR-GCC
Purpose: Drive multiplexed LED array

Fuse settings: 8 MHz internal RC oscillator, 
clock prescaler turned off (divide by 1)



Written by Chris Brookfield, http://www.evilmadscientist.com/

Copyright 2008 Christian W. Brookfield
Distributed under the terms of the GNU General Public License, please see below.

Additional license terms may be available; please contact us for more information.



More information about this project is at 
http://www.mignonette-game.com/



-------------------------------------------------
USAGE: How to compile and install



A makefile is provided to compile and install this program using AVR-GCC and avrdude.

To use it, follow these steps:
1. Update the header of the makefile as needed to reflect the type of AVR programmer that you use.
2. Open a terminal window and move into the directory with this file and the makefile.  
3. At the terminal enter
		make clean   <return>
		make all     <return>
		make install <return>
4. Make sure that avrdude does not report any errors.  If all goes well, the last few lines output by avrdude
should look something like this:

avrdude: verifying ...
avrdude: XXXX bytes of flash verified

avrdude: safemode: lfuse reads as E2
avrdude: safemode: hfuse reads as D9
avrdude: safemode: efuse reads as FF
avrdude: safemode: Fuses OK

avrdude done.  Thank you.


If you a different programming environment, make sure that you copy over 
the fuse settings from the makefile.


-------------------------------------------------

If you'd like to ask questions or suggest improvements
please contact us with evilmadscientist contact page:
http://www.evilmadscientist.com/staticpages/index.php/contact

-------------------------------------------------


Revision hitory:

8/26/2008 

Initial version
-------------------------------------------------


 This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


*/







//unsigned int NumBadGuys = 70;
#define NumBadGuys 70 






#include <avr/io.h> 
//#include <avr/interrupt.h>
#include <stdlib.h>

#define maskB  5
#define maskD  128 
#define maskC  1




void delayShorter(void)
{
unsigned int delayvar;
	delayvar = 0; 
	while (delayvar <=  100U)		
		{ 
			asm("nop");  
			delayvar++;
		} 
}



void delayShort(void)
{
unsigned int delayvar;
	delayvar = 0; 
	while (delayvar <=  200U)		
		{ 
			asm("nop");  
			delayvar++;
		} 
}

		

void delayLong()
{
unsigned int delayvar;
	delayvar = 0; 
	while (delayvar <=  1000U)		
		{ 
			asm("nop");  
			delayvar++;
		} 


		
	
}










void dc() // Very Long Delay
{
unsigned int delayvar;

delayvar = 0; 
	while (delayvar <=  25000U)		
		{ 
			asm("nop");  
			asm("nop");  
			asm("nop");  
			asm("nop");  
			asm("nop");  
			delayvar++;
		} 
}

		
	
unsigned short redMatrix[5];
unsigned short grnMatrix[5];


void display (void)
{
    
PORTB = (1 << 3) | maskB; // turn on redCol 1
PORTD = redMatrix[0] | maskD;
delayShorter();
PORTD = maskD;

PORTB = (1 << 4) | maskB; // turn on redCol 2
PORTD = redMatrix[1] | maskD;
delayShorter();
PORTD = maskD;

PORTB = (1 << 5) | maskB; // turn on redCol 3
PORTD = redMatrix[2] | maskD;
delayShorter();
PORTD = maskD;

PORTB = (1 << 6) | maskB; // turn on redCol 4
PORTD = redMatrix[3] | maskD;
delayShorter();
PORTD = maskD;

PORTB = (1 << 7) | maskB; // turn on redCol 5
PORTD = redMatrix[4] | maskD;
delayShorter();
PORTD = maskD;

PORTB = maskB;

PORTC = (1 << 1) | maskC; // turn on grnCol 1
PORTD = grnMatrix[0] | maskD;
delayShort();
PORTD = maskD;

PORTC = (1 << 2) | maskC; // turn on grnCol 2
PORTD = grnMatrix[1] | maskD;
delayShort();
PORTD = maskD;

PORTC = (1 << 3) | maskC; // turn on grnCol 3
PORTD = grnMatrix[2] | maskD;
delayShort();
PORTD = maskD;

PORTC = (1 << 4) | maskC; // turn on grnCol 4
PORTD = grnMatrix[3] | maskD;
delayShort();
PORTD = maskD;

PORTC = (1 << 5) | maskC; // turn on grnCol 5
PORTD = grnMatrix[4] | maskD;
delayShort();
PORTD = maskD;


PORTC = maskC;




}


void lose(void) // 'you lose' display
{
redMatrix[0] = 127;
redMatrix[1] = 127;
redMatrix[2] = 127;
redMatrix[3] = 127;
redMatrix[4] = 127;
grnMatrix[0] = 127;
grnMatrix[1] = 127;
grnMatrix[2] = 127;
grnMatrix[3] = 127;
grnMatrix[4] = 127;

}

void levelup(void) // next level
{
grnMatrix[0] = 127;
grnMatrix[1] = 127;
grnMatrix[2] = 127;
grnMatrix[3] = 127;
grnMatrix[4] = 127;

}

void bomb(void) // 'bomb' display
{
grnMatrix[0] = 0;
grnMatrix[1] = 0;
grnMatrix[2] = 8;
grnMatrix[3] = 0;
grnMatrix[4] = 0;

unsigned int blah;
blah = 0;
while (blah < 10)
{
    display();
    blah++;
}


grnMatrix[0] = 0;
grnMatrix[1] = 8;
grnMatrix[2] = 20;
grnMatrix[3] = 8;
grnMatrix[4] = 0;
blah = 0;
while (blah < 10)
{
    display();
    blah++;
}
grnMatrix[0] = 8;
grnMatrix[1] = 20;
grnMatrix[2] = 34;
grnMatrix[3] = 20;
grnMatrix[4] = 8;
blah = 0;
while (blah < 10)
{
    display();
    blah++;
}
grnMatrix[0] = 20;
grnMatrix[1] = 34;
grnMatrix[2] = 65;
grnMatrix[3] = 34;
grnMatrix[4] = 20;
blah = 0;
while (blah < 10)
{
    display();
    blah++;
}
grnMatrix[0] = 34;
grnMatrix[1] = 65;
grnMatrix[2] = 0;
grnMatrix[3] = 65;
grnMatrix[4] = 34;
blah = 0;
while (blah < 10)
{
    display();
    blah++;
}
grnMatrix[0] = 65;
grnMatrix[1] = 0;
grnMatrix[2] = 0;
grnMatrix[3] = 0;
grnMatrix[4] = 65;
blah = 0;
while (blah < 10)
{
    display();
    blah++;
}

}



unsigned short getpoint(unsigned short xin, unsigned short yin)
{

// for color input 0 = dark, 1 = red, 2 = green, 3 = orange    
unsigned short color = 0;


if (redMatrix[yin] & 64 >> xin)
{// red
color += 1;

}
    

if (grnMatrix[yin] & 64 >> xin)
{// green
color += 2;

}
return color;
}

    


void drawpoint(unsigned short xin, unsigned short yin, unsigned short color)
{

// for color input 0 = dark, 1 = red, 2 = green, 3 = orange    

if (color & 1)
{// red
    redMatrix[yin] |= 64 >> xin;
}
    
if (color & 2)
{// green
grnMatrix[yin] |= 64 >> xin;
}
    
}


void xdrawpoint(unsigned short xin, unsigned short yin, unsigned short color)
{

// for color input 0 = dark, 1 = red, 2 = green, 3 = orange    

if (color & 1)
{// red
    redMatrix[yin] = 64 >> xin;
}
    
if (color & 2)
{// green
grnMatrix[yin] = 64 >> xin;
}
    
}




void erasepoint(unsigned short xin, unsigned short yin)
{
  
redMatrix[yin] &= ~(64 >> yin);
grnMatrix[yin] &= ~(64 >> yin);
    
}



void eraseall(void)
{
unsigned short i=0;

while (i < 5) {
    
redMatrix[i] = 0;
grnMatrix[i] = 0;

i++;

}


}




struct badguystruct
{    
unsigned short xpos;
unsigned short ypos;
short color;
    };
    
 


int main (void)
{ 
 
DDRB = 250;
PORTB = 5;
DDRC = 254;
PORTC = 1;
DDRD = 127;
PORTD = 128;




TCCR1A = 242;	
TCCR1B = 25;	
OCR1B = 204;	
	
 
 



unsigned short xc;
unsigned short yc;
unsigned short ra;
unsigned short la;
unsigned short ua;
unsigned short da;
unsigned short pcolor;
unsigned short ca;

unsigned int waiter;
unsigned int waiter2;
unsigned int waiter3;

unsigned short cursor;
unsigned short cursoract;

unsigned short gen;
unsigned int j;
unsigned short bulletcount;
unsigned short buttons;
unsigned short cChecker[7];
unsigned int loser;
unsigned short speed;
unsigned short length;
unsigned short stop;
unsigned int notechange;
unsigned short note;
unsigned short firenote;
unsigned int firenotechange;
unsigned short zup;
unsigned short zdown;
unsigned int waitup;
unsigned int waitdown;
unsigned int zoom;
unsigned int zoomup;
unsigned int db;
unsigned int rcount;




struct badguystruct bga[NumBadGuys];






xc = 0;
yc = 0;
ra = 0;
la = 0;
ua = 0;
da = 0;
ca = 0;
waiter = 0;
cursor = 0;
cursoract = 0;
waiter2 = 0;
gen = 0;
buttons = 0;
waiter3 = 0;
loser = 0;
stop = 0;
notechange = 0;
note = 0;
firenote = 0;
firenotechange = 0;
zup = 0;
zdown = 0;
waitup = 0;
waitdown = 0;
zoom = 0;
zoomup = 0;
db = 1;
rcount = 0;


pcolor = 2;// this determines the player's color. 1 = red, 2 = green, 3 = yellow

bulletcount = 1;// this determines how many bombs you start out with

speed = 600;// this determines how fast the badguys move

length = 100;// this determines how dense the enemy waves are; lower = more dense and higer = less dense



j = 0;
while (j < 7)
{
    cChecker[j] = 0;
    j++;
}



j = 0;
while (j < NumBadGuys)// position badguys
{

    
    
bga[j].xpos = (rand() % length) + 6;
bga[j].ypos = rand() % 5;
bga[j].color = 1;



j++;
}
 


eraseall();



for (;;)  // main loop										
    {
    
    

if (zup == 1)
{
waitup++;
if (waitup == 250)
{
zup = 2;
waitup = 0;
}
}
if (zup == 0)
{
zoomup = 0;
waitup = 0;
}

if (zdown == 0)
{
waitdown = 0;
zoom = 0;
}


    

    if (zdown == 1)
   {
   waitdown++;
if (waitdown == 250)
{
zdown = 2;
waitdown = 0;
}
   }
   
    if (zup == 2) //zip up
{
zoomup++;
    }
    
if (zdown == 2) // zip down
{
zoom ++;
}    

if (zoomup == 50)
{
if (yc > 0)
    yc--;
else
    yc = 0;
zoomup = 0;
}


if (zoom == 50)
{
zoom = 0;
if (yc < 4)
    yc++;
else
    yc = 4;
}
 

notechange ++;


        
    waiter++;
    waiter2++;
    rcount++;
    
if (rcount == 50)
{
eraseall();
rcount = 0;

j = 0;
while (j < NumBadGuys)
{
            drawpoint(bga[j].xpos,bga[j].ypos,bga[j].color);
j++;
}

    }

    
    
    



    drawpoint(xc,yc,2);//draw player
    
    display();
    
    
  
   if (notechange == 500) //C
{
note++;
notechange = 0;
}
if (note == 1)    
{
ICR1 = 15296;	
OCR1A = 7648;
}
    
    if (note == 2) // C# / Db
{
ICR1 = 14440;	
OCR1A = 7220;
}   
    
   if (note == 3) // D
{
ICR1 = 13638;	
OCR1A = 6814;
}    
    
    
   if (note == 4) // D# / Eb
{
ICR1 = 12861;	
OCR1A = 6340;
}    
    

   if (note == 5) // E
{
ICR1 = 12136;	
OCR1A = 6069;
}

   if (note == 6) // F
{
ICR1 = 11461;	
OCR1A = 5730;
}


   if (note == 7) // F# / Gb
{
ICR1 = 10810;	
OCR1A = 5405;
}

   if (note == 8) // G
{
ICR1 = 10204;	
OCR1A = 5102;
}

   if (note == 9) // G# / Ab
{
ICR1 = 9636;	
OCR1A = 4813;
}

   if (note == 10) // A 
{
ICR1 = 9090;	
OCR1A = 4545;
}

   if (note == 11) //A# / Bb
{
ICR1 = 8583;	
OCR1A = 4291;
}

   if (note == 12) // B
{
ICR1 = 8097;	
OCR1A = 4048;
}

if (note == 13) // reset
    note = 0;


// Test buttons!

if ((PINB & 1) == 0) // nw, switch 1
    {
buttons |= 1;// set bit 1
if (zup < 2)
    zup = 1;
    }
else
{

if (buttons & 1)
{
zup = 0;
buttons |= 16;// set bit 5
}
buttons &= ~(1);// clear bit 1
}


    
    
    

if ((PINB & 4) == 0) // sw, switch 2
     { 
	 buttons |= 2;// set bit 2
     if (zdown < 2)
	 zdown = 1;
     }
     
else
{

if (buttons & 2)
{
buttons |= 32;// set bit 6
 zdown = 0;
    }
buttons &= ~(2);// clear bit 2
    }
   
    
 



if ((PINC & 1) == 0) // se, switch 3
     buttons |= 4;// set bit 3
else
{

if (buttons & 4)
    buttons |= 64;// set bit 7
buttons &= ~(4);// clear bit 3
    }

    
    
    
    
if ((PIND & 128) == 0) // ne, switch 4
     buttons |= 8;// set bit 4
else
{

if (buttons & 8)
    buttons |= 128;// set bit 8
buttons &= ~(8);// clear bit 4
    }

    
    
    
// END TEST BUTTONS    

    
if (waiter == 100)     
{
eraseall();






if (buttons & 16) // up
{
if (yc > 0)
    yc--;
else
    yc = 0;
buttons &= ~(16);
}




if (buttons & 32) // down
{
if (yc < 4)
    yc++;
else
    yc = 4;
buttons &= ~(32);
}




if (buttons & 64) // fire
{


buttons &= ~(64);

 
{
    
   
   
     j = 0;
    while (j < db)
    {
        display();
    j++;
    }
    
    
    
    grnMatrix[yc] = 32;
    redMatrix[yc] = 32;
   
    
    
    
    j = 0;
while (j < NumBadGuys)
{
    

    if (bga[j].color ==1)
    {
    if (bga[j].ypos == yc)
    {
    if (bga[j].xpos == 1)
    {
    bga[j].color = 0;
    stop++;
}
    }
}

    j++;
}

    }

if (stop == 0)    
    {j = 0;
    while (j < db)
    {
        display();
    j++;
    }
    
grnMatrix[yc] = 16;
    redMatrix[yc] = 16;
    
    
    j = 0;
while (j < NumBadGuys)
{
   
    
if (bga[j].color ==1)
    {
    if (bga[j].ypos == yc)
    {
    if (bga[j].xpos == 2)
    {
    bga[j].color = 0;
    stop++;
}
    }
    }
    j++;
}
}

if (stop == 0)    
    {  j = 0;
    while (j < db)
    {
        display();
    j++;
    }

grnMatrix[yc] = 8;
  redMatrix[yc] = 8;
  
  
    j = 0;
while (j < NumBadGuys)
{
    
    
    if (bga[j].color ==1)
    {
if (bga[j].ypos == yc)
    {
    if (bga[j].xpos == 3)
    {
        bga[j].color = 0;
    stop++;
}
    }
    }
    j++;
}
}

if (stop == 0)    
    {j = 0;
    while (j < db)
    {
        display();
    j++;
    }
    
grnMatrix[yc] = 4;
    redMatrix[yc] = 4;
    
    
    j = 0;
while (j < NumBadGuys)
{
   
    
    if (bga[j].color ==1)
    {
if (bga[j].ypos == yc)
    {
    if (bga[j].xpos == 4)
    {
    bga[j].color = 0;
    stop++;
}
    }
    }
    j++;
}
}

if (stop == 0)    
    {j = 0;
    while (j < db)
    {
        display();
    j++;
    }
	
grnMatrix[yc] = 2;
        redMatrix[yc] = 2;

	
    j = 0;
while (j < NumBadGuys)
{
    
    
    if (bga[j].color == 1)
    {
if (bga[j].ypos == yc)
    {
    if (bga[j].xpos == 5)
    {
    bga[j].color = 0;
    stop++;
}
    }
    }
    j++;
}
    }
    

if (stop == 0)    
    { j = 0;
    while (j < db)
    {
        display();
    j++;
    }
grnMatrix[yc] = 1;
    redMatrix[yc] = 1;
    
    
    
    j = 0;
while (j < NumBadGuys)
{
    
    
    if (bga[j].color ==1)
    {
if (bga[j].ypos == yc)
    {
    if (bga[j].xpos == 6)
    {
    bga[j].color = 0;
    stop++;
}
    }
    }
    j++;

}
}


if (stop == 0)
  {j = 0;
    while (j < db)
    {
        display();
    j++;
    }
    grnMatrix[yc] = 0;
      redMatrix[yc] = 0;
      
  }
  
    
    
    
stop = 0;
display();


//  Go through list of bad guys:
// if a bad guy is in the same row (checking yc...)
//  AND that bad guy is first in the right X range,   
//  then DELETE HIM
 
     
}

if (buttons & 128)   // Bomb
   {
   if (bulletcount > 0)
  {
  j = 0;
while (j < NumBadGuys)
    {

	if (bga[j].xpos < 7)
	    bga[j].color = 0;
	

		
	
	j++;
    }
bomb();
bulletcount --;

//Clear the badguys from the screen, then show the bomb animation.

//SOUND:
while (firenote < 13)
{firenotechange++;
    
   if (firenotechange == 2000) //c
{
firenote++;
firenotechange = 0;
}
if (firenote == 12)    
{
ICR1 = 15296;	
OCR1A = 7648;
}
    
    if (firenote == 11) // c# / db
{
ICR1 = 14440;	
OCR1A = 7220;
}   
    
   if (firenote == 10) // d
{
ICR1 = 13638;	
OCR1A = 6814;
}    
    
    
   if (firenote == 9) // d# / eb
{
ICR1 = 12861;	
OCR1A = 6340;
}    
    

   if (firenote == 8) // e
{
ICR1 = 12136;	
OCR1A = 6069;
}

   if (firenote == 7) // f
{
ICR1 = 11461;	
OCR1A = 5730;
}


   if (firenote == 6) // f# / gb
{
ICR1 = 10810;	
OCR1A = 5405;
}

   if (firenote == 5) // g
{
ICR1 = 10204;	
OCR1A = 5102;
}

   if (firenote == 4) // g# / ab
{
ICR1 = 9636;	
OCR1A = 4813;
}

   if (firenote == 3) // a 
{
ICR1 = 9090;	
OCR1A = 4545;
}

   if (firenote == 2) //a# / bb
{
ICR1 = 8583;	
OCR1A = 4291;
}

   if (firenote == 1) // b
{
ICR1 = 8097;	
OCR1A = 4048;
}


    }
firenote = 0;


    }
  
buttons &= ~(128);

    

   }
   


//See if you lose:
j = 0;
while (j < NumBadGuys)
    {
        drawpoint(bga[j].xpos,bga[j].ypos,bga[j].color);
	j++;

    
	if (bga[j].xpos == xc)
	{

	if (bga[j].color == 1)
        {
    
while (loser < 300)
{
    lose();
    display();
    
    loser++;
}
loser = 0;

while (loser < 650)
{
    delayShort();
    
    loser++;
}
loser = 0;

while (loser <300)
{
    lose();
    display();
    
    loser++;
}
loser = 0;

while (loser < 650)
{
    delayShort();
    loser++;
}
loser = 0;

while (loser <300)
{
    lose();
    display();
    
    loser++;
}
loser = 0;

while (loser < 650)
{
    delayShort();
    loser++;
}
ICR1 = 0;	
OCR1A = 0;
loser = 0;
while (loser < 1)
{asm("nop");

if ((PINC & 1) == 0) // se, switch 3
     buttons |= 4;// set bit 3
else
{

if (buttons & 4)
    buttons |= 64;// set bit 7
buttons &= ~(4);// clear bit 3
    }    
    
if (buttons & 64) // fire button triggers restart
{
loser++;

j = 0;
while (j < NumBadGuys)
{

    
    

bga[j].xpos += waiter3;

bga[j].color = 1;



j++;
}
waiter3 = 0;
xc = 0;
yc = 0;
notechange = 0;
bulletcount = 1;

    
    }
    }    
	}
	}    
    }
     
    waiter = 0;

}
    

    
    
if (waiter2 == speed)    // move badguys
{

j = 0;
while (j < NumBadGuys)
    {
        bga[j].xpos--;
	if (bga[j].xpos == -1)
	    bga[j].color = 0;
	

		
	
	j++;
    }
    
waiter2 = 0;
waiter3 ++;




    
     
    
}

if (waiter3 == length + 6)   //next level
{
waiter3 = 0;
speed -= 25;
length -= 10;
bulletcount ++;

if (length < 15)
{
length = 100;
speed -= 25;
}
if (speed < 100)
    speed -= 50;


loser = 0;
while (loser <300)
{
    levelup();
    display();
    
    loser++;
}



j = 0;
while (j < NumBadGuys)
{

    
    
bga[j].xpos = (rand() % length) + 5;
bga[j].ypos = rand() % 5;
bga[j].color = 1;



j++;
}
}

     


    

 
     

    
    
    }	//End main loop. 
 
    
    
    
return 1;

    }
