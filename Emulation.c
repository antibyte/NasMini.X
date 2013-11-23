/* 
 * File:   Emulation.c
 * 
 * Authors and License :
 * Z80 Emulation:  Marat Fayzullin - free for noncommercial use
 * Video, PS/2 : Katsumi - licenced under LGPL 
 * FAT Filesystem : Elm Chan - free, unrestricted use
 * PIC32 FAT FS adaption : Aiden Morrison, Riccardo Leonardi
 * Nascom emulation : antibyte - free, unrestricted use
 *
 */

#define GetSystemClock()     (48000000ul)
#define GetPeripheralClock()    (GetSystemClock())
#define GetInstructionClock()   (GetSystemClock())

#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include <peripheral/osc.h>
#include <peripheral/system.h>

#include "./Z80/Z80.h"
#include "./Z80/GenericTypeDefs.h"
#include "./Z80/Compiler.h"
#include "./z80/msbasic.h"
#include "./z80/nsys3.h"
#include "./Z80/Compiler.h"

#include "./emulation.h"
#include "./Video/video.h"
#include "./ps2.h"
#include "./fatfs/ff.h"
#include "./fatfs/diskio.h"
#include "./uart.h"
#include <sys/attribs.h>

/*
 * 
 */
#pragma config PMDL1WAY = OFF           // Allow multiple reconfigurations
#pragma config IOL1WAY  = OFF           // Allow multiple reconfigurations
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL  = MUL_24
#pragma config FPLLODIV = DIV_2
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config UPLLEN   = OFF            // USB PLL Enable
#pragma config UPLLIDIV = DIV_2         // USB PLL Divider
#pragma config FVBUSONIO = OFF          // VBUS pin control
#pragma config FUSBIDIO = OFF           // USBID pin control
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx1      // ICE/ICD Comm Channel Select
#pragma config JTAGEN   = OFF           // Disable JTAG

#ifdef __DEBUG
#pragma config DEBUG = ON				// Background Debugger ON or OFF
#else
#pragma config DEBUG = OFF				// Background Debugger ON or OFF
#endif


unsigned char memory[maxmem];   // the emulated memory
char keyscan_counter;           // row counter for keymatrix scan
char key_reset;                 // reset keymatrix counter
unsigned char tape_led=0;       // tape_led state
unsigned char single_step=0;    // single_step circuit state
unsigned char serial_data=0;
unsigned char mounted=0;
unsigned char have_disk=0;
unsigned char loadrequest=0;
unsigned char fileNname[15];    // filename buffer

FRESULT FOpenRes;

Z80 engine;

FATFS Fatfs;
FATFS *fs;				/* Pointer to file system object */
FIL file;               		/* File objects */
volatile BYTE rtcYear = 111, rtcMon = 11, rtcMday = 22;	// RTC starting values
volatile BYTE rtcHour = 0, rtcMin = 0, rtcSec = 0;

//Work registers for fs command
DWORD acc_size;			/* Work register for fs command */
WORD acc_files, acc_dirs;
FILINFO Finfo;
const BYTE ft[] = {0,12,16,32};

#define TOGGLES_PER_SEC		1000
#define CORE_TICK_RATE	       (GetSystemClock()/2/TOGGLES_PER_SEC)


int main(int argc, char** argv) {

    SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

   /* USBDeviceInit();
    if ( USBGetDeviceState() == DETACHED_STATE ) {
        USBDeviceAttach();
    }*/

    ANSELA = 0;
    ANSELB = 0; //all ports digital

    INTEnableSystemSingleVectoredInt();
    INTEnableSystemMultiVectoredInt();

    // Software RTC for FAT Filesystem
    OpenCoreTimer(CORE_TICK_RATE);			// Open 1 ms Timer
    // set up the core timer interrupt with a prioirty of 2 and zero sub-priority
    mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_2 | CT_INT_SUB_PRIOR_0));
    
    initVideo();                // start the video engine
    initPS2();                  // setup keyboard
    initUART();                 // setup RS232

    if (!disk_initialize(0))    // initialize sd card
    {
        have_disk=1;
        f_mount(&Fatfs,"/",1);
    }

    engine.IPeriod=500;         // Call 'LoopZ80' every 500 Z80 clocks
    ResetZ80(&engine);         
    uart_putc('X');
    while(1)
    {
      RunZ80(&engine);          // Start Z80 emulation
    }
    return (EXIT_SUCCESS);
}

void WrZ80(register word Addr,register byte Value) // handles Z80 memory writing
{
    word tladdr,vline,vcol;
    tladdr=(Addr-0x800);

    if (Addr>=0x0c00 && Addr<maxmem)
        memory[tladdr]=Value; // write to ram
    else if (Addr>=0x800 && Addr<0xc00)   // access to video ram ?
    {
        if (tladdr>9)   // visible area starts at vram+10
        {               // translate to video engine buffer
            vline=tladdr>>6;
            vcol=tladdr>73?tladdr%64-10:tladdr-10;
            if (vline<15 && vcol<49)
                VRAM[48+vline*48+vcol]=Value;
            if (vline==15 && vcol<48)   // last line in vram is first on screen
                VRAM[vcol]=Value;
        }
        memory[tladdr]=Value;   // store in ram
    }
    return;

}
byte RdZ80(register word Addr)  // handles Z80 memory reading
{
    if (Addr < 0x800)                // 0x000-0x800 is monitor rom
        return nsysrom[Addr];
    else if (Addr < 0x800+maxmem)    // 0x1000-0x7000 is ram
        return memory[Addr-0x800];

    if (Addr>=0xe000)                // 0xe000-0xffff is basic rom
        return basrom[Addr-0xe000];
    return 0;
}

void OutZ80(register word Port,register byte Value)

{
    word pt = Port & 0x00ff;

    switch(pt)
    {
        case 0:
            if (Value&0x01)
                keyscan_counter++;  // scan next keyboard row
            if (Value&0x02)
                keyscan_counter=0;  // reset keyboard matrix counter
            tape_led=!!(Value&0x10);
            if (Value&0x08)         // Single step circuit triggered ?
                engine.Trap=1;
            break;
        case 1:
            uart_putc(Value);
            break;

    }
}

byte InZ80(register word Port)

{
    word pt = Port & 0x00ff;

    switch(pt)
    {
        case 0:         // keyboard matrix scan result (inverted)
             return ~(keyscan_buffer[keyscan_counter]);
        case 1:         // read serial data
            if (uart_rx_count)
            {
                
                //uart_rx_count--;
                return uart_rx_buff[--uart_rx_count];
            }
            return 0;
    
        case 2:         // read serial port status byte
             return 64 | (uart_rx_count ? 128:0);

        default:
             return 0;
    }
}

word LoopZ80(register Z80 *R)   // called periodically from Z80 emulation

{
    getkey();                   // Check for keypress

    if (key_nmi)                // scroll lock was pressed, trigger NMI
    {
        key_nmi=0;
        return INT_NMI;
    }

    if (edit_mode && have_disk)           // F1 - User wants to load "*.NAS" file
    {
        if (req_action==1)
        {
            if (get_filename(fileNname, "NAS format file loading:" ) && !!fileNname[0])
            {
                if (open_file(fileNname,FA_READ))
                {
                    load_nas();
                    close_file();
                    return INT_NONE;
                } else
                    if (!!fileNname[0]);
                        user_info("Fatal Error:", "Could not open the requested file !");
            }
        } else if (req_action==2)       // F2 - User wants to save to "*.NAS" file
        {
            if (get_filename(fileNname, "NAS format memory dump (0x1000-0x7000)." ) && !!fileNname[0])
            {
                if (open_file(fileNname,FA_WRITE))
                {
                    save_nas();
                    close_file();
                    return INT_NONE;
                } else
                    if (!!fileNname[0]);
                        user_info("Fatal error;", "Could not open the requested file !");
            }            
        }
        req_action=0;
    }

    if (tape_led && have_disk && !mounted)
    {
        // todo: cassette emulation
    }
        
    return INT_NONE;

}

void PatchZ80(register Z80 *R)
{
   
}

void save_nas()
{
    if (!have_disk)
        return;

    unsigned char haddr[4];
    unsigned char val[2];
    char eol[4]={0x08,0x08,0x0d,0x0a};
    char eofm[3]={'.',0x0d,0x0a};
    unsigned char r;
    int c;
    unsigned int cksum=0;
    unsigned int wcnt;

    for (c=0;c<maxmem-0x800;c+=8)
    {
        nas_val2hex(0x1000+c,haddr,4);
        cksum+=(0x1000+c)>>2&0x00ff;
        cksum+=(0x1000+c)&0x00ff;
        f_write(&file, &haddr,4,&wcnt);
        f_write(&file, " ",1,&wcnt);
        for (r=0;r<8;r++)
        {
            nas_val2hex(memory[0x800+c+r],val,2);
            cksum+=memory[0x800+c+r];
            f_write(&file, &val,2,&wcnt);
            f_write(&file, " ",1,&wcnt);
        }
        nas_val2hex(cksum&0x00ff,val,2);
        cksum=0;
        f_write(&file, val,2,&wcnt);
        f_write(&file, &eol,4,&wcnt);
    }
    f_write(&file, &eofm,3,&wcnt);
}

/* Load a file in NAS format
   Every line starts with the target addr. followed by
   eight bytes of data and a one byte checksum.
   ASCII format ----------------->|
   AAAA 01 02 03 04 05 06 07 08 09 0x08 0x08 0x0d 0x0a
   checksum is low byte of AA+AA+01 ... +08
   Last line contains only a dot '.' and CR/LF */

void load_nas()
{

    if (!have_disk)
        return;

    BYTE haddr[4];
    BYTE hval[2];
    WORD taddr;
    WORD tval;
    UINT bread=0;
    char l;

    for(;;)
    {
        f_read(&file,&haddr,4,&bread);
        if (haddr[0]=='.') // done if first char in line is a dot
            goto done;
        if (bread!=4) //EOF
            goto err;
        taddr=nas_hex2val(haddr,4);
        if (taddr<0x1000 || taddr>maxmem+0x1000)
        {
            printchar2(0,taddr>>8);
            printchar2(2,taddr&&0x00ff);
            //v_print(10);
            //v_print(haddr);
            while(1);
            user_info("Fatal Error:","Target address outside of allowed range.");
            return;
        }
        for (l=0;l<8;l++)
        {
            f_read(&file, &hval,1,&bread); //dummy (blank)
            f_read(&file, &hval,2,&bread);
            if (!!!bread) //EOF
                goto err;
            memory[taddr-0x800+l]=nas_hex2val(hval,2);
        }
        while (1)
        {
            f_read(&file, &hval,1,&bread);
            if (!bread) //EOF
                goto err;
            if (hval[0]==0x0a) // line finished
                break;
        }
    }
err:
    user_info("Fatal Error:","Unexpected end of file.");
done:
    return;

}

WORD nas_hex2val (char* haddr,BYTE len)
{
    int rval,i=0;
    while (len--)
    {
        rval<<=4;
        rval|=(haddr[i]<='9'?haddr[i]-'0':haddr[i]-'A'+10);
        i++;
    }
    return rval;
}

void nas_val2hex(WORD val, char* hex, BYTE len)
{
	int ch, i = 0;
	while (len--)
	{
		ch = val & 0x000f;
		hex[len] = (ch > 9 ? 'A' + ch-10 : '0' + ch);
			val >>= 4;
	}
}

unsigned char get_filename(char *fname,char *desc_text)
{
    BYTE x=0;
    save_vram();    // save a copy of the screen
    printchar(12);
    v_print(desc_text);
    printchar(10);
    v_print("Enter Filename or press ESC to exit. ");
    printchar(10);
    while (edit_keybuff!='*') // escape='*'
    {
        getkey();
        if (edit_newkey)
        {
            switch(edit_keybuff)
            {
                case('+'):  // enter
                    if (x==0)
                    {
                        fname[0]=0;
                    } else
                    {
                        memcpy(fname,VRAM+96,x);
                        fname[x]=0;
                    }
                    restore_vram();
                    edit_mode=edit_keybuff=edit_newkey=0;
                    return 1;
                 case ('-'):    // backspace
                    printchar (8);
                    if (x>0)
                        x--;
                    edit_newkey=0;
                    continue;
                default:
                    if (x<12)
                    {
                        edit_newkey=0;
                        printchar (edit_keybuff);
                        x++;
                    }
            }
        }
    }
    edit_mode=edit_keybuff=edit_newkey=0;
    restore_vram(); // restore original screen
    return 0;
}


unsigned char open_file (char *fname, unsigned char mode)
{
    FRESULT FOpenRes;
    long accessmode;
    accessmode=(mode==FA_READ)?(FA_READ | FA_OPEN_EXISTING):(FA_OPEN_ALWAYS | FA_CREATE_ALWAYS |FA_WRITE );
    FOpenRes=f_open(&file, fname, (BYTE)accessmode);
    return (FOpenRes==FR_OK)?1:0;
}

void close_file ()
{
    f_close(&file);
}

void user_info(char *info,char *text)
{
    edit_mode=1;
    save_vram();
    printchar(12);
    v_print(info);
    printchar (10);
    v_print(text);
    printchar (10);
    printchar (10);
    v_print("Press ESC to continue.");
    printchar(10);
    while (edit_keybuff!='*')
        getkey();
    restore_vram();
    edit_mode=0;
}


/*****************************************************************************
 * Function:  		void CoreTimerHandler(void)
 * PreCondition:
 * Input:           None
 * Output:          None
 * Side Effects:
 * Overview:        FatFs requires a 1ms tick timer to aid
 *					with low level function timing
 * Note:            Initial Microchip version adapted to work into ISR routine
 *****************************************************************************/
void __ISR(_CORE_TIMER_VECTOR, IPL2SOFT) CoreTimerHandler(void)
{
	static const BYTE dom[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	static int div1k;
	unsigned char n;

	// clear the interrupt flag
	mCTClearIntFlag();
    // update the period
    UpdateCoreTimer(CORE_TICK_RATE);

	disk_timerproc();	// call the low level disk IO timer functions

	// implement a 'fake' RTCC
	if (++div1k >= 1000) {
		div1k = 0;
		if (++rtcSec >= 60) {
			rtcSec = 0;
			if (++rtcMin >= 60) {
				rtcMin = 0;
				if (++rtcHour >= 24) {
					rtcHour = 0;
					n = dom[rtcMon - 1];
					if ((n == 28) && !(rtcYear & 3)) n++;
					if (++rtcMday > n) {
						rtcMday = 1;
						if (++rtcMon > 12) {
							rtcMon = 1;
							rtcYear++;
						}
					}
				}
			}
		}
	}
}


/*********************************************************************
 * Function:  		DWORD get_fattime(void)
 * PreCondition:
 * Input:           None
 * Output:          Time
 * Side Effects:
 * Overview:        when writing fatfs requires a time stamp
 *					in this exmaple we are going to use a counter
 *					If the starter kit has the 32kHz crystal
 *					installed then the RTCC could be used instead
 * Note:
 ********************************************************************/
DWORD get_fattime(void)
{
	DWORD tmr;

	INTDisableInterrupts();
	tmr =	  (((DWORD)rtcYear - 80) << 25)
			| ((DWORD)rtcMon << 21)
			| ((DWORD)rtcMday << 16)
			| (WORD)(rtcHour << 11)
			| (WORD)(rtcMin << 5)
			| (WORD)(rtcSec >> 1);
	INTEnableInterrupts();

	return tmr;
}


/** EOF main.c *************************************************/

