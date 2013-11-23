/* 
 * File:   emulation.h
 * Author: andi
 *
 * Created on 2. November 2013, 05:18
 */

#ifndef EMULATION_H
#define	EMULATION_H


#define maxmem 24576 // RAM available for Z80

#define m_write 0
#define m_read 1

WORD nas_hex2val (char *,BYTE);
void nas_val2hex(WORD, char*, BYTE);
void load_nas ();
void save_nas();
unsigned char get_filename(char *,char *);
unsigned char open_file (char *, BYTE);
void close_file ();
void user_info(char *, char *);
unsigned char key_nmi;
unsigned char edit_mode;
unsigned char req_action;
#endif	/* EMULATION_H */

