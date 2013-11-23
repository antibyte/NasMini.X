Single Chip Nascom 2 Emulator

Credits:
Z80 Emulation :         : Marat Fayzullin http://fms.komkon.org/EMUL8/
Video and PS/2 based on : Katsumi http://hp.vector.co.jp/authors/VA016157/
SD & FAT Filesystem     : Elm Chan, Aiden Morrison, Riccardo Leonardi


What's working :

z80 CPU
24K RAM (2K used by video buffer and NAS-SYS)
10K ROM (2K NAS-SYS 3 and 8K Microsoft Basic)
Loading NAS format files from SD card(press F1)
Saving RAM to NAS format on SD card (press F2)
Composite Video 48x16 video output, advanced graphics character ROM is included.
PS/2 keyboard with special keys:
  F1 - Load NAS file from SD card (works for BASIC)
  F2 - Save RAM (0x0100-0x7000) as NAS file to SD card (works for BASIC)
  Scroll lock - Trigger NMI
Single step emulation
RS232 is partialy working. Received Data is corrupted, maybe hardware
problem, as is used a Bluetooth adapter. 

ToDo :

Cassette Emulation (real or/and SD)
RS232 bootloader (if possible)
Map Z80PIO access to I2C Port expander, or remaining physical pins ?

Speed:
Maybe slower than the real thing, but really usable.

Thoughts:
A PIC32MX250 is used featuring USB, but there is not enought space left for any
USB stack.
But as the price difference to the non-USB MX150 is negligible for small
projects, it may be good to have it on board for possible reuse in future
projects.

First steps:
Visit http://www.nascomhomepage.com/
 
Type "J" after start to jump to Microsoft Basic
Most NAS files load to 0x01000. Type E1000 to run them

antibyte 11/2013
