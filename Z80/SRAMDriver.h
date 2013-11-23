/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;                                                                     
;                     Software License Agreement                      
;                                                                     
;     ©2007 Microchip Technology Inc
;     Mirochip Technology Inc. ("Microchip") licenses this software to 
;     you solely for the use with Microchip Products. The software is
;     owned by Microchip and is protected under applicable copyright
;     laws. All rights reserved.
;
;     SOFTWARE IS PROVIDED "AS IS." MICROCHIP EXPRESSLY DISCLAIMS ANY
;     WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
;     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
;     PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
;     BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
;     DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
;     PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
;     BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
;     ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
;     
;                                                                
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;	Filename:			    	SRAMDriver.h
;	Date:				    	January 30, 2009
;	File Version:		  		1.0                             
;	Compiler Used:			MPLAB IDE 8.20
; 	Author:			  	    	Pinakin K Makwana
;	Company:			    	Microchip Technology, Inc.
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



#define SYS_FREQ 		(GetSystemClock())

//#define                 USE_SPI_Module1
#define		USE_SPI_Module2
//#define		USE_SPI_Module3



#define  	SRAMRead   					0x03     	//Read Command for SRAM
#define  	SRAMWrite  					0x02     	//Write Command for SRAM
#define 	SRAMRDSR   					0x05     	//Read the status register
#define  	SRAMWRSR  					0x01     	//Write the status register
#define		SRAMByteMode				0x00
#define		SRAMPageMode				0x80
#define		SRAMSeqMode				0x40
#define		SRAMPageSize				32
#define		DummyByte					0xFF

#if defined(USE_SPI_Module1)
	#define	WriteSPI(a)					SpiChnPutC(1, a); 	//WriteSPI1(a)
	#define	ReadSPI()					SpiChnGetC(1);//ReadSPI1(a)
	#define SPI_Rx_Buf_Full					//SPI1STATbits.SPIRBF
#elif defined(USE_SPI_Module2)
	#define	WriteSPI(a)					WriteSPI2(a)
	#define	ReadSPI(a)					ReadSPI2(a)
	#define SPI_Rx_Buf_Full				SPI2STATbits.SPIRBF
#elif defined(USE_SPI_Module3)
	#define	WriteSPI(a)					WriteSPI3(a)
	#define	ReadSPI(a)					ReadSPI3(a)
	#define SPI_Rx_Buf_Full				SPI3STATbits.SPIRBF
#endif

#define		SRAM_CS					BIT_3			//RB2 used as Chip Select


extern void InitSRAM(void);
extern unsigned char SRAMReadStatusReg(void);
extern char SRAMWriteStatusReg(unsigned char WriteVal);
extern void SRAMCommand(unsigned char AddLB,unsigned char AddMB,unsigned char AddHB, unsigned char RWCmd);
extern char SRAMWriteByte(unsigned char AddLB,unsigned char AddMB,unsigned char AddHB,unsigned char Data);
extern unsigned char SRAMReadByte(unsigned char AddLB,unsigned char AddMB,unsigned char AddHB);
extern unsigned char SRAMWritePage(unsigned char AddLB,unsigned char AddMB,unsigned char AddHB, unsigned char *WriteData);
extern unsigned char SRAMReadPage(unsigned char AddLB,unsigned char AddMB,unsigned char AddHB,unsigned char *ReadData);
extern char SRAMWriteSeq(unsigned char AddLB,unsigned char AddMB,unsigned char AddHB, unsigned char *WriteData,unsigned int WriteCnt);
extern char SRAMReadSeq(unsigned char AddLB,unsigned char AddMB,unsigned char AddHB,unsigned char *ReadData,unsigned int ReadCnt);

