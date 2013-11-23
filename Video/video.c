/*
   This program is provided under the LGPL license ver 2.
   Written by Katsumi.
   http://hp.vector.co.jp/authors/VA016157/
   kmorimatsu@users.sourceforge.jp
   -------
   Converted to PAL Timings and NASCOM resolution by antibyte 11/2013
   This creates a 48x16 character display.
   Font is 8x16, so final resolution is 384x256
   Allmost all modern TV's should be able to handle both PAL and NTSC.
   Nascom produced a 8x12 cut font version for sale in NTSC regions.

 ****** NOTE: ignore values in comments, most are still ntsc based *****

 
 */

#include <plib.h>
#include "video.h"
#include "cgrom.h"
#include "./../ps2.h"
#include "./../emulation.h"


/*

PAL:

  	Raster frequency is 15625 hz, so one raster takes 64 usecs.
	Use 3072 clocks, resulting in 15625 hz
	Note that 512 clocks of 8 MHz (=SPI clock speed) also produces the same frequency.

 */

/*	Minimum time between two interruption events is 2.3 usecs,
	that corresponds to 92 clocks.
*/

// RB0=Sync
#define sync LATBbits.LATB0 //remapped for better PCB layout ,was 13
#define toggleSync() LATBINV=0x01 // remapped, was 2000
#define hclocks 3072

unsigned char VRAM[84*16];  // video buffer
unsigned char VRAM2[84*16]; // video backup buffer

void initVideo(){
	unsigned long i;
        for (i=0;i<0x0300;i++) {
		VRAM[i]=32;
	}
	g_ntscVisible=1;
	g_Vsync=1;
	sync=1;

	// RB13 and RA4 are output pins
	TRISBbits.TRISB0=0; // was A4
	TRISBbits.TRISB13=0;

	// Timer2 is used for sync signal
	// On, Stop in Idle Mode, PBCLK, 1:1 prescale, 16-bit mode
	// Enable interrupt as priority 7
	TMR2=0;
	PR2=0xFFFF;
	T2CON=0xA000;
	T2CONbits.ON=1;
	IPC2bits.T2IP=7;
	IPC2bits.T2IS=0;
	IFS0bits.T2IF=0;
	IEC0bits.T2IE=1;

	// Timer4 is used for video signal
	// Off, Stop in Idle Mode, PBCLK, 1:2 prescale, 16-bit mode
	// Enable interrupt as priority 6
	TMR4=0;
	PR4=0xFFFF;
	T4CON=0x2010;
	IPC4bits.T4IP=6;
	IPC4bits.T4IS=0;
	IFS0bits.T4IF=0;
	IEC0bits.T4IE=1;

	// SPI2 module settings follow
	// PBCLOCK, Enable SPI2, Stop in Idle Mode, Enable SDO, 32 bit mode,
	// SS pin not used, Master mode, Disable SDI
	// SPI clock=REFCLK/(2x(SPI2BRG+1))=8 Mhz. (REFCLK=48 Mhz; SPI2BRG=2)
	// All extended setting are off (SPI2CON2).

	SPI2CON=0x0100A83C;
	SPI2CON2=0x0300;
	SPI2BRG=2;
	// Output SDO2 RB13 - remapped, was RA4
        RPB13R=0x04;
}

/*	PAL Sync signals:
	raster 1-3: L for 2.3 usecs, H for 29.5 usec, L for 2.3 usecs, H for 29.5 usec
	(110)         (110)
	2.3u__________2.3u__________                 110+1414+110+1414=3048
	|__|          |__|          
	       29.5u         29.5u
          (1414)        (1414)

	raster 4-6: L for 27.1 usecs, H for 4.7 usec, L for 27.1 usecs, H for 4.7 usec
      (1299)        (1299)
	   27.1u    __   27.1u    __                 1299+225+1299+225=3048
	|__________|  |__________|  
	           4.7u          4.7u
              (225)         (225)

	raster 7-9: L for 2.3 usecs, H for 29.5 usec, L for 2.3 usecs, H for 29.5 usec
	(110)         (110)
	2.3u__________2.3u__________                 110+1414+110+1414=3048
	|__|          |__|          
	       29.5u         29.5u
          (1414)        (1414)

	raster 10-262: H for 1.5 usecs, L for 4.7 usecs, H for 57.4 usecs
      (225)
	___4.7u_____________________                 72+225+2751=3048
	   |__|          
	1.65u          57.4u 57,65
    (72)         (2751)

	This must produces 16275 Hz sync signal.
	Following intterupt function takes about 1 micro second.
	This will be 0.5 micro second if IPL7SRS will be used.
*/

#define bporch 79

const unsigned short palSyncTiming[]={
// raster 1-3
	112-1,1424-1,112-1,1424-1,112-1,1424-1,112-1,1424-1,112-1,1424-1,112-1,1424-1,
// raster 4-6
	1311-1,225-1,1311-1,225-1,1311-1,225-1,1311-1,225-1,1311-1,225-1,1311-1,225-1,
// raster 7-9
	112-1,1424-1,112-1,1424-1,112-1,1424-1,112-1,1424-1,112-1,1424-1,112-1,1424+79-1,
// raster 10-309
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,225-1,2768+79-1,
        // raster 310-312
	225-1,2768+79-1,225-1,2768+79-1,225-1,2768-1

};


void __ISR(_TIMER_2_VECTOR, IPL7SOFT)ntscSyncInt(){
	static unsigned long status=0;
	unsigned int i16;
	IFS0CLR=_IFS0_T2IF_MASK; //IFS0bits.T2IF=0;
	toggleSync();
	PR2=i16=palSyncTiming[status++];
	if (i16==(2768-1)) {
		// The end of the sequence of statuses.
		// Reset parameter.
		sync=1;
		status=0;
		// Trigger Timer4 for video signal
		// Note that video signal starts from raster 42.
		// Also note that timer for video works at 24 Mhz.
		TMR4=TMR2>>1;
		PR4=(2775+hclocks*41+72+225)/2-32-1; // Needs to be tuned to define horizontal position.
		if (1/*g_Vgate*/) T4CONSET=_T4CON_ON_MASK; //T4CONbits.ON=1;
		g_Vsync=0; // Pseudo vertical sync signal starts here.
	}
}
/*	Video signal construction
	1 raster: 1536 cycles of timer working at 24 Mhz (24 clocks/usec; 24 clocks/8 dots)
	status:  1    2    3    4    5    6    7 
	         |    |    |    |    |    |    |
              (113)
	       ___4.7u___________________________
	          |__|          
	       1.5u            57.4u
           (30)           (1381)
	// This routine takes about 2 micro seconds.
	// This will take about 1 micro second if IPL7SRS will be used.
*/
void __ISR(_TIMER_4_VECTOR, IPL6SOFT) ntscVideoInt(){
	static unsigned long status=1;
	static unsigned long addr=0;
	static unsigned long fontLine=0;
	unsigned long i1,i2,i3,i4;
	static unsigned char* cgrom_line;
	IFS0CLR=_IFS0_T4IF_MASK; //IFS0bits.T4IF=0;
	switch(status){
		case 1:
			status=2;
			g_Vsync=1; // Pseudo vertical sync signal ends here.
			if (!g_ntscVisible) {
				T4CONCLR=_T4CON_ON_MASK; //T4CONbits.ON=0;
			} else {
				PR4=190; // Needs to be tuned (must be less than 192, more than 170.6).
				cgrom_line=&cgrom[fontLine];
			}
			break;
		
                case 8:
			status=1;
			if (fontLine<15*256) {
				fontLine+=256;
				addr-=48;
			} else {
				fontLine=0;
				if (767<addr) {
					// Video signal was already end. Let's stop timer
					T4CONCLR=_T4CON_ON_MASK; //T4CONbits.ON=0;
					// Reset parameters
					addr=0;
					fontLine=0;
					break;
				}
			}
			PR4=(hclocks/2)-(190+1)*7-1;
			break;
		default:
			status++;
			i1=(cgrom_line[VRAM[addr+0]])<<24;
			i2=(cgrom_line[VRAM[addr+1]])<<16;
			i3=(cgrom_line[VRAM[addr+2]])<<8;
			i4=(cgrom_line[VRAM[addr+3]]);
			SPI2BUF=i1|i2|i3|i4;
			addr+=4;
			i1=(cgrom_line[VRAM[addr+0]])<<24;
			i2=(cgrom_line[VRAM[addr+1]])<<16;
			i3=(cgrom_line[VRAM[addr+2]])<<8;
			i4=(cgrom_line[VRAM[addr+3]]);
			SPI2BUF=i1|i2|i3|i4;
			addr+=4;
			break;
	}
}

void printchar2(char pos,unsigned char chr) {    // for debugging
    showHex (pos,chr);
    return;
    
}

void v_print(char * text)
{
    int c=0;
    while(1)
    {
        if(text[c])
        {
            printchar(text[c]);
            c++;
        } else
        break;
    }
}

void printchar(unsigned char chr)
{
    static unsigned char cursor_x = 0;
    static unsigned char cursor_y = 0;
    VRAM[cursor_x+cursor_y*48]=32; // hide cursor block
    switch (chr)
    {
        case 12:    // Clear screen
            memset(&VRAM,32,768);
            cursor_x=cursor_y=0;
            break;
        case 10:    // New line
            cursor_x=0;
            cursor_y++;
            break;
        case 8:     // Backspace
            if (cursor_x>0)
                cursor_x--;
            break;
        default:    // Print char
            VRAM[cursor_x+cursor_y*48]=chr;
            cursor_x++;
            break;
    }

    if (cursor_x>47){
        cursor_x=0;
        cursor_y++;
    }
    if (cursor_y>15) {
        memmove(VRAM, VRAM+48,768-48); // scroll up one line
        memset(VRAM+768-48,32,48);  // erase last line
        cursor_y=15;
    }
    VRAM[cursor_x+cursor_y*48]=219; // show cursor block
}

void save_vram()
{
    memcpy(VRAM2,VRAM,768);
}
void restore_vram()
{
    memcpy(VRAM,VRAM2,768);
}