/*
   This program is provided under the LGPL license ver 2.
   Written by Katsumi.
   http://hp.vector.co.jp/authors/VA016157/
   kmorimatsu@users.sourceforge.jp
*/

#include "./../ps2.h"
#include <p32xxxx.h>
#include <plib.h>
#include "./../keymap.h"

/*
	PORTB8: data
	PORTB9: clock (INT1)
*/

#define ps2_clock_out TRISBbits.TRISB9
#define ps2_data_out  TRISBbits.TRISB8
#define ps2_clock_in  PORTBbits.RB9
#define ps2_data_in   PORTBbits.RB8
#define ps2_clock_lat LATBbits.LATB9
#define ps2_data_lat  LATBbits.LATB8

unsigned long ps2data=0;
unsigned long ps2status=1;
char scancode;
unsigned char keyscan_buffer[9];

unsigned char key_nmi=0;
unsigned char edit_keybuff=0;
unsigned char edit_newkey=0;
unsigned char edit_mode=0;
unsigned char req_action=0;
static unsigned char edit_make=0;


void initPS2(){
	// Weak pull-up
	CNPUBbits.CNPUB8=1;
	CNPUBbits.CNPUB9=1;
	// IDLE mode
	ps2_clock_lat=0;
	ps2_data_lat=0;
	ps2_clock_out=1;
	ps2_data_out=1;

	// INT1 is assigned to RB9 pin (clock in)
	// Enable interrupt as priority 5
	INT1R=4;
	INTCONbits.INT1EP=0; // Falling edge
	IPC1bits.INT1IP=5;
	IPC1bits.INT1IS=0;
	IFS0bits.INT1IF=0;
	IEC0bits.INT1IE=1;

}

void __ISR(_EXTERNAL_1_VECTOR, IPL5SOFT) ps2Int()
{
	IFS0CLR=_IFS0_INT1IF_MASK; //IFS0bits.INT1IF=0;
	if (ps2_data_in) ps2data|=ps2status;
	ps2status<<=1;
}

// PS2 key input

void getkey()
{
    if (0x200000<ps2status) {
        if ((ps2data&0x200800)==0x200000) {
	// Parity check is not done.
            if (edit_mode)
                get_editor_key ((ps2data>>12)&0xff);
            else
                map_key((ps2data>>12)&0xff);
	}
	ps2status=0x800;
	ps2data&=0x7ff;
    }
    if (ps2status==0x800) {
        if ((ps2data&0x401)==0x400) {
            // Parity check is not done.
            if (edit_mode)
                get_editor_key ((ps2data>>1)&0xff);
            else
                map_key((ps2data>>1)&0xff);
	}
	ps2status=1;
	ps2data=0;
    }
}

void get_editor_key(unsigned char scancode)
{
    static unsigned char breakcode=0;
    BYTE s;
    if (scancode==0xf0) // key released ?
    {
        breakcode=1;
        return;
    }
    if (breakcode)
    {
        for (s=0;s<40;s++)
            {
                if (scancodes[s][0]==scancode)
                {
                    edit_keybuff=scancodes[s][1];
                    edit_newkey=1;
                }
            }
    }
    breakcode=0;
}

void map_key(unsigned char scancode)
{
    static unsigned char breakcode=0;
    BYTE alt_buff;
    BYTE cnt,bt;

    switch (scancode)
    {
        case 0xf0:  // a key was released
            breakcode=1;
            return;
        case 0x05: // F1
            edit_mode=1;
            req_action=1;
            return;
        case 0x06: //F2
            edit_mode=1;
            req_action=2;
            return;
        case 0x7e:  // scroll lock
            key_nmi=1;
            return;
    }
    
    if (breakcode)   //key released
    {
        breakcode=0;
        switch(scancode)
        {
            case 0x14:
                keyscan_buffer[0]=0; // CTRL released
                return;
            case 0x12:
            case 0x59:
                keyscan_buffer[0]=0; // Shift released
                return;
            case 0x11:
                keyscan_buffer[5]=0; // ALT released
                return;
            default:
                alt_buff=keyscan_buffer[5]&(1<<6); // save ALT (graphics key) state
                memset(keyscan_buffer+1,0,8);      // (graphics key shares row 
                keyscan_buffer[5]=alt_buff;        // with normal keys)
                return;
        }
    } else
    {
        switch(scancode)
        {
            case 0x14: // CTRL pressed
                keyscan_buffer[0]=1<<3;
                return;
            case 0x12:
            case 0x59: // shift pressed
                keyscan_buffer[0]=1<<4;
                return;
            case 0x11: // ALT pressed
                keyscan_buffer[5]=1<<6;
                return;
            default:
                for (cnt=0;cnt<9;cnt++)
                {
                    for (bt=0;bt<8;bt++)
                    {
                        if (keymap[cnt][bt]==scancode)
                        {
                            keyscan_buffer[cnt]=1<<bt;
                         }
                    }
                }
        }
    }
}