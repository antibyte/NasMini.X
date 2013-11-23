/* 
 * File:   ps2.h
 * Author: andi
 *
 * Created on 7. November 2013, 22:49
 */

#ifndef PS2_H
#define	PS2_H

unsigned char keyscan_buffer[9];
unsigned char edit_keybuff;
unsigned char edit_newkey;
static unsigned char edit_make;

void initPS2();
void ps2_received(unsigned char data);
unsigned long ps2data;
unsigned long ps2status;
extern void getkey();
void map_key(unsigned char);
void get_editor_key(unsigned char scancode);


#endif	/* PS2_H */

