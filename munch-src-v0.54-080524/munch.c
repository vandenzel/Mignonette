/*
 *	munch.c - "Munch" game concept using Mignonette Graphics Library (miggl) - v0.54
 *
 *	author(s): rolf van widenfelt (rolfvw at pizzicato dot com) (c) 2008 - Some Rights Reserved
 *
 *	author(s): mitch altman (c) 2008 - Some Rights Reserved
 *
 *	Note: This source code is licensed under a Creative Commons License, CC-by-nc-sa.
 *		(attribution, non-commercial, share-alike)
 *  	see http://creativecommons.org/licenses/by-nc-sa/3.0/ for details.
 *
 *
 *	hardware requirements:
 *		- Mignonette v1.0
 *
 *
 *	instructions:
 *		Munch was inspired by a conversation with a friend's son, James (age 9).
 *		It is loosely based on Pacman.
 *		The goal of Munch is to "eat" as many of the green
 *		"food bits" while avoiding the yellow "evil dot".
 *
 *	revision history:
 *
 *	- may 24, 2008 - rolf
 *		cleanup.  call this version 0.54.
 *
 *	- may 22, 2008 - rolf
 *		change intro jingle.
 *
 *	- may 18, 2008 - rolf
 *		WEIRD... renamed from munch.c to mig-testmunch.c (and back to Unix line ends), and
 *			now disassembly is complete again (in .lst file).
 *
 *	- may 18, 2008 - rolf
 *		fix comments.
 *		minor cleanup.
 *		rename to munch.c and call this version 0.52.
 *
 *	- may 16, 2008 - rolf
 *		playsong() seems to work!
 *		the challenge is to make little jingles that don't seem too annoying.
 *
 *	- may 13, 2008 - rolf
 *		attempt to add audio.
 *		start with some basic calls at the beginning of the game.
 *
 *	- may 2, 2008 (again) - rolf
 *		fix to replay current level (and the remaining "food bits").
 *		this required adding a mask to initfoodbits().
 *		release as version 0.51.
 *
 *	- may 2, 2008 (AM) - rolf
 *		release as "version 0.5".
 *		minor change to levels (make just three)
 *
 *	- may 1, 2008 - rolf
 *		various game refinements, mainly a splash screen for "lives remaining".
 *		TODO: add a "game over" splash screen, and replay existing level
 *		until all food bits are eaten (or player dies).
 *
 *	- apr 27, 2008 - rolf
 *		release under Creative Commons CC-by-nc-sa license.
 *
 *	- apr 20, 2008 - rolf
 *		remove references to uart (finally!), but keep uart.h here to define F_CPU.
 *
 *	- apr 19, 2008 - rolf
 *		implement 4-button navigation... not as intuitive as i had hoped!
 *		basic game sort of works!
 *
 *	- apr 18, 2008 - rolf
 *		created.
 *
 *
 */

#include <inttypes.h>
#include <avr/io.h>			/* this takes care of definitions for our specific AVR */
#include <avr/pgmspace.h>	/* needed for printf_P, etc */
#include <avr/interrupt.h>	/* for interrupts, ISR macro, etc. */
#include <stdio.h>			// for sprintf, etc.
#include <stdlib.h>			// for abs(), etc.
//#include <string.h>			// for strcpy, etc.

#include "uart.h"			// we keep this here only to define F_CPU (uart.c not needed)

// for _delay_us() macro  (note: this gets F_CPU define from uart.h)
#include <util/delay.h>

#include "mydefs.h"
#include "iodefs.h"

#include "miggl.h"		/* Mignonette Game Library */


/* audio stuff - beware, these song tables take up much memory! (so comment out unused tables) */

#ifdef NOTDEF
//
// a few notes from a well known classical melody (J.S. Bach, Cantata BWV 147)
//
byte ClassicalIntroSong[] = {
N_E4,N_8TH,
N_C4,N_8TH,
N_D4,N_8TH,
N_E4,N_8TH,
N_G4,N_8TH,
N_F4,N_8TH,
N_F4,N_8TH,
N_A4,N_8TH,
N_G4,N_8TH,
N_G4,N_8TH,
N_C5,N_8TH,
N_B4,N_8TH,
N_C5,N_8TH,
N_G4,N_8TH,
N_E4,N_8TH,
N_C4,N_HALF,

N_END,
};
#endif

byte IntroScaleSong[] = {
N_C4,N_16TH,
N_D4,N_16TH,
N_E4,N_16TH,
N_F4,N_16TH,
N_G4,N_8TH,
N_END,
};


// (aka Star Wars)
byte LevelClearedSong[] = {
N_C4,N_8TH_TRIP,
N_REST,N_8TH_TRIP,
N_C4,N_8TH_TRIP,
N_G4,N_HALF_DOT,
N_F4,N_8TH_TRIP,
N_G4,N_8TH_TRIP,
N_A4,N_8TH_TRIP,
N_G4,N_8TH_TRIP,
N_F4,N_8TH_TRIP,
N_G4,N_HALF,
N_REST,N_8TH,
N_END,
};


byte MunchedSong[] = {
N_C4,N_16TH,
N_GS3,N_16TH,
N_FS3,N_16TH,
N_DS3,N_16TH,
N_END,
};


#ifdef NOTDEF
byte FoodBitsSong[] = {
N_C3,N_16TH,
N_END,
};

// when "evil dot" appears
byte EvilEntrySong[] = {
N_C4,N_16TH,
N_CS4,N_16TH,
N_C4,N_16TH,
N_CS4,N_16TH,
N_END,
};
#endif


//
// crude delay of 1 to 255 us
//
void
delay_us(byte usec)
{
	usec++;
	
	while (--usec) {
		_delay_us(1);		// get 1us delay from library macro (see <util/delay.h>)
	}
}


//
// crude delay of 1 to 255 ms
//
void
delay_ms(uint8_t ms)
{
	
	ms++;
	
	while (--ms) {
		_delay_ms(1);		// get 1ms delay from library macro (see <util/delay.h>)
	}
}


//
// crude "sleep" function for 0 to 255 seconds
//
void
delay_sec(uint8_t sec)
{
	uint8_t i;
	for (i = 0; i < sec; i++) {
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
	}
}


#define XMAX (XSCREEN-1)
#define YMAX (YSCREEN-1)


// stores pattern of food bits on screen
static byte FoodBits[5];

// mask to hide eaten food bits
static byte FoodMask[5];


//
// creates pattern of "food bits" for each level.
//
// set "new" flag to create a fresh pattern, otherwise, previous (partially eaten) pattern is used.
//
// note: first call to this function should set "new" flag!
//
byte initfoodbits(byte level, byte new)
{
	uint8_t numbits;
	uint8_t i, b, m;
	
	level = level >> 1;		// XXX repeat each level twice and then repeat after 3 times
	
	while (level > 3) {		// mod 3 (avoids divide)
		level -= 3;
	}
	
	if (new) {
		for (i = 0; i < 5; i++) {
			FoodMask[i] = 0xff;
		}
	}

	if (level == 0) {
		FoodBits[0] = FoodMask[0] & 0;		// note: leftmost bit (bit 7) is not used
		FoodBits[1] = FoodMask[1] & 0x18;
		FoodBits[2] = FoodMask[2] & 0x03;
		FoodBits[3] = FoodMask[3] & 0;
		FoodBits[4] = FoodMask[4] & 0x0c;
		//numbits = 6;
	} else if (level == 1) {
		FoodBits[0] = FoodMask[0] & 0;
		FoodBits[1] = FoodMask[1] & 0x2a;
		FoodBits[2] = FoodMask[2] & 0x04;
		FoodBits[3] = FoodMask[3] & 0x0a;
		FoodBits[4] = FoodMask[4] & 0x11;
		//numbits = 8;
	} else {
		FoodBits[0] = FoodMask[0] & 0x02;
		FoodBits[1] = FoodMask[1] & 0x36;
		FoodBits[2] = FoodMask[2] & 0x11;
		FoodBits[3] = FoodMask[3] & 0x03;
		FoodBits[4] = FoodMask[4] & 0x18;
		//numbits = 11;
	}

	// count bits
	numbits = 0;
	for (i = 0; i < 5; i++) {
		m = FoodBits[i];
		for (b = 0; b < 8; b++, m = m >> 1) {
			if (m & 0x1) {
				numbits++;
			}
		}
	}

	return numbits;			// return number of "food bits" set above
}


void drawfoodbits(void)
{
	uint8_t i;
	
	for (i = 0; i < 5; i++) {
		Disp[i] = FoodBits[i];		// XXX hack: just copy right into green plane 0..4
	}	
}


byte getfoodbit(byte x, byte y)
{
	uint8_t mask;

	if ((x < 7) && (y < 5)) {	// clipping

		mask = 0x40 >> x;
		if (FoodBits[y] & mask) {
			return 1;
		}
	}

	return 0;
}


void clearfoodbit(byte x, byte y)
{
	uint8_t mask;

	if ((x < 7) && (y < 5)) {	// clipping
		mask = 0x40 >> x;
		FoodBits[y] &= ~mask;	// clear bit
		FoodMask[y] &= ~mask;	// clear bit
	}
}


static byte Evilx, Evily;		// "evil dot" location
static uint8_t Evildelay;		// delay used by "chase" algorithm


//
// calc starting position for "evil dot", based on given player coordinates.
//
// note: this must avoid any "food bits".
//
void calcevilstart(byte px, byte py)
{
	static byte evilstarts[4*2] = {1,0, 5,0, 1,4, 5,4};		// candidates for starting position
	int x, y;
	int maxdist, dist;
	byte imax;
	uint8_t i;

	maxdist = 0;
	imax = 99;
	
	for (i = 0; i < 4; i++) {
		
		x = evilstarts[i*2];
		y = evilstarts[i*2+1];
		
		if (!getfoodbit(x, y)) {				// always avoid "food bits"

			dist = abs(x - px) + abs(y - py);	// note: abs() is a built-in gcc function
			if (dist > maxdist) {
				maxdist = dist;
				imax = i;				// choose farthest one from player
			}
		}
	}
	
	if (imax != 99) {
		Evilx = evilstarts[imax*2];		// load chosen start candidate
		Evily = evilstarts[imax*2+1];
	}
}


#define SGN(x) ((x<0)?-1:1)


#define EVDELAY 3

//
// move the "evil dot" towards the player, based on a simple "chase" algorithm.
//
// note: currently this does a "manhattan" walk and if there is a "food bit" in the way,
//		it gets stuck!  (yeah, too simple)
//
void calcevilmove(byte px, byte py)
{
	int8_t dx, dy;
	int8_t sdx, sdy;

	int8_t evx, evy;
	
	evx = Evilx;
	evy = Evily;
	
	dx = px - evx;
	dy = py - evy;
	
	sdx = SGN(dx);		// set to 1 or -1 only
	sdy = SGN(dy);
	
	if (dx || dy) {			// do we need to move?
	
		if (abs(dx) >= abs(dy)) {		// pick an axis (x or y)

			if (!getfoodbit(evx+sdx, evy)) {	// need to avoid "food bits" ...
				Evilx += sdx;

			} else {

				if (Evildelay == 0) {
					Evildelay = EVDELAY;		// set to 1 or higher - adds delay after almost "hitting" a food bit

				} else if (Evildelay == 1) {
					if ((dy != 0) && (!getfoodbit(evx, evy+sdy))) {
						Evily += sdy;
					}
					Evildelay = 0;
				} else {
					Evildelay--;
				}
			}

		} else {

			if (!getfoodbit(evx, evy+sdy)) {
				Evily += sdy;

			} else {

				if (Evildelay == 0) {
					Evildelay = EVDELAY;		// (same as above)

				} else if (Evildelay == 1) {
					if ((dx != 0) && (!getfoodbit(evx+sdx, evy))) {
						Evilx += sdx;
					}
					Evildelay = 0;
				} else {
					Evildelay--;
				}
			}
		}
	}

}


//
// perform short "starburst" animation at location (bx by) using the given frame number.
// returns 1 when finished.
//
byte starburstanim(byte bx, byte by, byte frame)
{

	if (frame == 0 || frame == 2 || frame == 4 || frame == 5) {
		setcolor(YELLOW);
		drawpoint(bx+1, by+1);		// X pattern
		drawpoint(bx-1, by+1);
		drawpoint(bx-1, by-1);
		drawpoint(bx+1, by-1);
	}
	if (frame == 1 || frame == 3) {
		setcolor(YELLOW);
		drawpoint(bx+1, by);		// + pattern
		drawpoint(bx-1, by);
		drawpoint(bx, by+1);
		drawpoint(bx, by-1);
	}
	if (frame >= 6) {
		return 1;	// done
	}
	
	return 0;
}


//
// displays a separate "splash screen" indicating how
// many lives the player has left.
//
// note: XXX assumes 3 lives is maximum
//
void livesremaining_splash(byte numlives)
{
	uint8_t i, j;
	int px, py;
	
	px = 1;
	py = 2;

	cleardisplay();
	setcolor(RED);
	for (i = 0; i < numlives; i++) {
	
		drawpoint(px, py);
		
		px += 2;
		
		// delay a bit
		for (j = 0; j < 3; j++) {
			swapbuffers();
		}
	}

	// delay a bit
	for (j = 0; j < 8; j++) {
		swapbuffers();
	}
}


int
main(void)
{
	//int i;
	int bx, by;
	int dx, dy;
	byte foodcount;
	int graceframes;
	byte evilinterval;
	byte animframe = 0;
	byte level = 0;
	byte numlives = 3;		// number of lives for player

	
    avrinit();

	initswapbuffers();
	swapinterval(10);		// note: display refresh is 100hz (lower number speeds up game)
	cleardisplay();

	start_timer1();			// this starts display refresh and audio processing
	
	button_init();

	initaudio();			// XXX eventually, we remove this!

	//setwavetable(WT_SINE);
	//playsong(ClassicalIntroSong);	// test audio

	//waitaudio();

	playsong(IntroScaleSong);	// beginning of game jingle

	delay_sec(1);

	// show "lives remaining" splash screen
	livesremaining_splash(numlives);
	
	foodcount = 0;		// important: needed for first call of initfoodbits!

	//
	// and now, the "Munch" game
	//
nextlevel:

	bx = 0;
	by = 3;

	if (foodcount == 0) {
		foodcount = initfoodbits(level, 1);
	} else {
		foodcount = initfoodbits(level, 0);		// here, we continue with current food bits
	}
	
	graceframes = 25;		// delay before "evil dot" appears (counts down)
	evilinterval = 8;		// delay before "evil dot" starts moving

	while (1) {
	
		cleardisplay();
		drawfoodbits();
		
		handlebuttons();
	
		// try this button interface...

		dx = 0;
		dy = 0;
		if (ButtonC) {			// button C moves right (advances)
			dx = 1;
			if (ButtonA) {
				dy = -1;
			} else if (ButtonB) {
				dy = 1;
			}
		} else if (ButtonD) {	// button D moves left (retreats)
			dx = -1;
			if (ButtonA) {
				dy = -1;
			} else if (ButtonB) {
				dy = 1;
			}
		}
		
		bx += dx;
		if (bx < 0) bx = 0;
		else if (bx > XMAX) bx = XMAX;
		
		by += dy;
		if (by < 0) by = 0;
		else if (by > YMAX) by = YMAX;

		setcolor(RED);
		drawpoint(bx, by);
		
		if (getfoodbit(bx, by)) {		// did we cross a "food bit"?
		
			//playsong(FoodBitsSong);		// play "munch" sound
			clearfoodbit(bx, by);
			foodcount--;
			if (foodcount == 0) {
				//playnote(N_C5, N_WHOLE);	// play "finished screen" sound
				delay_sec(1);
				
				// all food eaten, go to next level
				level++;

				// cleared 6 levels... let's celebrate!
				if (level % 6 == 0) {
					playsong(LevelClearedSong);
					waitaudio();
				}

				goto nextlevel;

			}
		}

		// draw "evil dot" after grace period finishes
		if (graceframes == 0) {
			if (evilinterval == 0) {
				calcevilmove(bx, by);			// move "evil dot" towards player
				//playnote(N_C5, N_8TH);			// play "evil dot" movement sound
				evilinterval = 4;
			} else {
				evilinterval--;
			}
			setcolor(YELLOW);
			drawpoint(Evilx, Evily);
			
			if ((Evilx == bx) && (Evily == by)) {	// player was "munched" !!

				playsong(MunchedSong);		// play "you were munched" jingle
				
				while (1) {
					if (starburstanim(bx, by, animframe++)) {
						break;
					}
					swapbuffers();
					cleardisplay();
					drawfoodbits();
				}

				delay_sec(1);
				numlives--;
				if (numlives > 0) {
					livesremaining_splash(numlives);
				} else {
					// game over  (XXX should add a "game over" splash screen)
					delay_sec(1);
					goto gameover;
				}
					
				animframe = 0;
				
				// XXX instead of going to next level, we continue the current level!
				goto nextlevel;
			}

		} else if (graceframes == 1) {		// only happens once
			calcevilstart(bx, by);
			//playsong(EvilEntrySong);		// play "evil dot" appearance sound
			graceframes--;
		} else {
			graceframes--;
		}
		

		swapbuffers();			// wait for next display cycle...
	}


gameover:
	return (0);
}
