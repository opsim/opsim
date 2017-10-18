/**
 * $Id:$
 * ***** BEGIN GPL/BL DUAL LICENSE BLOCK *****
 *
 * The contents of this file may be used under the terms of either the GNU
 * General Public License Version 2 or later (the "GPL", see
 * http://www.gnu.org/licenses/gpl.html ), or the Blender License 1.0 or
 * later (the "BL", see http://www.blender.org/BL/ ) which has to be
 * bought from the Blender Foundation to become active, in which case the
 * above mentioned GPL option does not apply.
 *
 * The Original Code is Copyright (C) 2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL/BL DUAL LICENSE BLOCK *****
 */

#ifndef __MYDEVICE_H__

#define __MYDEVICE_H__

/* OMZETTING VAN X NAAR IRIS GL CODES */

/*
 * 
 *   mouse / timer / window: tot 0x020
 *   eigen codes: 0x4...
 * 
 */



/* MOUSE : 0x00x */

#define LEFTMOUSE	0x001	
#define MIDDLEMOUSE	0x002	
#define RIGHTMOUSE	0x003	
#define MOUSEX		0x004	
#define MOUSEY		0x005	

/* timers */

#define TIMER0		0x006	
#define TIMER1		0x007	
#define TIMER2		0x008	
#define TIMER3		0x009	

/* SYSTEM : 0x01x */

#define KEYBD			0x010	/* keyboard */
#define RAWKEYBD		0x011	/* raw keyboard for keyboard manager */
#define REDRAW			0x012	/* used by port manager to signal redraws */
#define	INPUTCHANGE		0x013	/* input connected or disconnected */
#define	QFULL			0x014	/* queue was filled */
#define WINFREEZE		0x015	/* user wants process in this win to shut up */
#define WINTHAW			0x016	/* user wants process in this win to go again */
#define WINCLOSE		0x017	/* window close */
#define WINQUIT			0x018	/* signal from user that app is to go away */
#define Q_FIRSTTIME		0x019	/* on startup */

/* standard keyboard */

#if defined(__BeOS) || defined(__WIN32)

#define AKEY		'a'
#define BKEY		'b'
#define CKEY		'c'
#define DKEY		'd'
#define EKEY		'e'
#define FKEY		'f'
#define GKEY		'g'
#define HKEY		'h'
#define IKEY		'i'
#define JKEY		'j'
#define KKEY		'k'
#define LKEY		'l'
#define MKEY		'm'
#define NKEY		'n'
#define OKEY		'o'
#define PKEY		'p'
#define QKEY		'q'
#define RKEY		'r'
#define SKEY		's'
#define TKEY		't'
#define UKEY		'u'
#define VKEY		'v'
#define WKEY		'w'
#define XKEY		'x'
#define YKEY		'y'
#define ZKEY		'z'

#define ZEROKEY		'0'
#define ONEKEY		'1'
#define TWOKEY		'2'
#define THREEKEY	'3'
#define FOURKEY		'4'
#define FIVEKEY		'5'
#define SIXKEY		'6'
#define SEVENKEY	'7'
#define EIGHTKEY	'8'
#define NINEKEY		'9'

#define CAPSLOCKKEY		211

#define LEFTCTRLKEY		212
#define LEFTALTKEY 		213
#define	RIGHTALTKEY 	214
#define	RIGHTCTRLKEY 	215
#define RIGHTSHIFTKEY	216
#define LEFTSHIFTKEY	217

#define ESCKEY			218
#define TABKEY			219
#define RETKEY			220
#define SPACEKEY		221
#define LINEFEEDKEY		222
#define BACKSPACEKEY	223
#define DELKEY			224
#define SEMICOLONKEY	225
#define PERIODKEY		226
#define COMMAKEY		227
#define QUOTEKEY		228
#define ACCENTGRAVEKEY	229
#define MINUSKEY		230
#define VIRGULEKEY		231
#define SLASHKEY		232
#define BACKSLASHKEY	233
#define EQUALKEY		234
#define LEFTBRACKETKEY	235
#define RIGHTBRACKETKEY	236

#define LEFTARROWKEY	137
#define DOWNARROWKEY	138
#define RIGHTARROWKEY	139
#define UPARROWKEY		140

#define PAD2		150
#define PAD4		151
#define PAD6		152
#define PAD8		153

#define PAD1		154
#define PAD3		155
#define PAD5		156
#define PAD7		157
#define PAD9		158

#define PADPERIOD		199
#define	PADVIRGULEKEY 	159
#define PADASTERKEY 	160


#define PAD0		161
#define PADMINUS		162
#define PADENTER		163
#define PADPLUSKEY 		164


#define	F1KEY 		300
#define	F2KEY 		301
#define	F3KEY 		302
#define	F4KEY 		303
#define	F5KEY 		304
#define	F6KEY 		305
#define	F7KEY 		306
#define	F8KEY 		307
#define	F9KEY 		308
#define	F10KEY		309
#define	F11KEY		310
#define	F12KEY		312

#define	PAUSEKEY	165
#define	INSERTKEY	166
#define	HOMEKEY 	167
#define	PAGEUPKEY 	168
#define	PAGEDOWNKEY	169
#define	ENDKEY		170


#else
/* Keycodes derived from X */

#define AKEY		XK_a
#define BKEY		XK_b
#define CKEY		XK_c
#define DKEY		XK_d
#define EKEY		XK_e
#define FKEY		XK_f
#define GKEY		XK_g
#define HKEY		XK_h
#define IKEY		XK_i
#define JKEY		XK_j
#define KKEY		XK_k
#define LKEY		XK_l
#define MKEY		XK_m
#define NKEY		XK_n
#define OKEY		XK_o
#define PKEY		XK_p
#define QKEY		XK_q
#define RKEY		XK_r
#define SKEY		XK_s
#define TKEY		XK_t
#define UKEY		XK_u
#define VKEY		XK_v
#define WKEY		XK_w
#define XKEY		XK_x
#define YKEY		XK_y
#define ZKEY		XK_z

#define ZEROKEY		XK_0
#define ONEKEY		XK_1
#define TWOKEY		XK_2
#define THREEKEY	XK_3
#define FOURKEY		XK_4
#define FIVEKEY		XK_5
#define SIXKEY		XK_6
#define SEVENKEY	XK_7
#define EIGHTKEY	XK_8
#define NINEKEY		XK_9

#define CAPSLOCKKEY		XK_Caps_Lock

#define LEFTCTRLKEY		XK_Control_L
#define LEFTALTKEY 		XK_Alt_L
#define	RIGHTALTKEY 	XK_Alt_R
#define	RIGHTCTRLKEY 	XK_Control_R
#define RIGHTSHIFTKEY	XK_Shift_R
#define LEFTSHIFTKEY	XK_Shift_L

#define ESCKEY			XK_Escape
#define TABKEY			XK_Tab
#define RETKEY			XK_Return
#define SPACEKEY		XK_space
#define LINEFEEDKEY		XK_Linefeed
#define BACKSPACEKEY	XK_BackSpace
#define DELKEY			XK_Delete
#define SEMICOLONKEY	XK_semicolon
#define PERIODKEY		XK_period
#define COMMAKEY		XK_comma
#define QUOTEKEY		XK_quoteright
#define ACCENTGRAVEKEY	XK_quoteleft
#define MINUSKEY		XK_minus
#define VIRGULEKEY		XK_slash
#define SLASHKEY		XK_slash
#define BACKSLASHKEY	XK_backslash
#define EQUALKEY		XK_equal
#define LEFTBRACKETKEY	XK_bracketleft
#define RIGHTBRACKETKEY	XK_bracketright

#define LEFTARROWKEY	XK_Left
#define DOWNARROWKEY	XK_Down
#define RIGHTARROWKEY	XK_Right
#define UPARROWKEY		XK_Up

/* dit zijn de arrow keys voor SUN */
#define PAD2		XK_KP_Down
#define PAD4		XK_KP_Left
#define PAD6		XK_KP_Right
#define PAD8		XK_KP_Up

#ifdef __SUN

#define PAD1		0xffde
#define PAD3		0xffe0
#define PAD5		0xffdc
#define PAD7		0xffd8
#define PAD9		0xffda

#define PADPERIOD		0xffff
#define	PADVIRGULEKEY 	0xffd6
#define PADASTERKEY 	0xffd7

#else

#define PAD1		XK_KP_End
#define PAD3		XK_KP_Page_Down
#define PAD5		XK_KP_Begin
#define PAD7		XK_KP_Home
#define PAD9		XK_KP_Page_Up

#define PADPERIOD		XK_KP_Delete
#define	PADVIRGULEKEY 	XK_KP_Divide
#define PADASTERKEY 	XK_KP_Multiply

#endif

#define PAD0		XK_KP_Insert
#define PADMINUS		XK_KP_Subtract
#define PADENTER		XK_KP_Enter
#define PADPLUSKEY 		XK_KP_Add

/* the extended keyboard */

#define	F1KEY 		XK_F1
#define	F2KEY 		XK_F2
#define	F3KEY 		XK_F3
#define	F4KEY 		XK_F4
#define	F5KEY 		XK_F5
#define	F6KEY 		XK_F6
#define	F7KEY 		XK_F7
#define	F8KEY 		XK_F8
#define	F9KEY 		XK_F9
#define	F10KEY		XK_F10
#define	F11KEY		XK_F11
#define	F12KEY		XK_F12

#define	PAUSEKEY	XK_Pause
#define	INSERTKEY	XK_Insert
#define	HOMEKEY 	XK_Home
#define	PAGEUPKEY 	XK_Page_Up
#define	PAGEDOWNKEY	XK_Page_Down
#define	ENDKEY		XK_End

#endif


#endif	/* !__MYDEVICE_H__ */

