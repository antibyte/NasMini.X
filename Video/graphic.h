/*
**  graphic.h
** 
**  Composite video and graphic library
**
**  11/20/07 v 4.0 LDJ NTSC and PAL support 
*/

//#define DOUBLE_BUFFER   // comment if single buffer required
#define PAL            // comment if PAL required

#define VRES     200    // desired vertical resolution
#define HRES     384    // desired horizontal resolution pixel

void initVideo( void);

void haltVideo( void);

void clearScreen( void);

void plot( unsigned x, unsigned y); 

void line( short x0, short y0, short x1, short y1);

void * getAVideo( void);

void clearHScreen( void);

void swapV(void);

void singleV( void);

//
// text on video 
//
extern int cx, cy;

#define Home()      { cx=0; cy=0;}
#define Clrscr()    { clearScreen(); Home();}
#define AT( x, y)   { cx = (x); cy = (y);}
    
void putcV( char a);

void putsV( char *s);
