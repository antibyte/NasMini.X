/*
   This program is provided under the LGPL license ver 2.
   Written by Katsumi.
   http://hp.vector.co.jp/authors/VA016157/
   kmorimatsu@users.sourceforge.jp
*/

#include <p32xxxx.h>
#include <plib.h>

/* Global variables follow */

unsigned long g_ntscVisible;
unsigned long g_Vsync;
extern unsigned char VRAM[84*16];
void v_print(char * text);

/* Prototyping */

void printchar(unsigned char chr);
void printchar2(char pos, unsigned char chr);
void copy2vram();
void initVideo();
void save_vram();
void restore_vram();

// Debug macro
#define showHex(pos,val)\
	VRAM[pos]=(((val)>>4)<10)?(0x30+((val)>>4)):(0x41-10+((val)>>4));\
	VRAM[pos+1]=(((val)&15)<10)?(0x30+((val)&15)):(0x41-10+((val)&15))

