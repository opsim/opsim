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


/* hoort bij button.c */

#ifndef BUTTON_H
#define BUTTON_H

/* dit is oud */
#define PAPER 100
#define PEN 7
#define BUT_IN 13
#define BUT_OUT 12


#define CHA	32
#define SHO	64
/* #define LON	96 */
#define INT	96
#define FLO	128
/* #define DOU	160 */
#define FUN	192
#define BIT	256

#define BUTPOIN	(128+64+32)

#define BUT	(1<<9)
#define ROW	(2<<9)
#define TOG	(3<<9)
#define SLI	(4<<9)
#define	NUM	(5<<9)
#define TEX	(6<<9)
#define TOG3	(7<<9)
#define BUTRET	(8<<9)
#define TOGN	(9<<9)
#define LABEL	(10<<9)
#define MENU	(11<<9)
#define ICONROW	(12<<9)
#define ICONTOG	(13<<9)
#define NUMSLI	(14<<9)
#define COL		(15<<9)
#define IDPOIN	(16<<9)
#define HSVSLI 	(17<<9)

#define BUTTYPE	(31<<9)

#define MAXBUTSTR	20

#define BTST(a,b)	( ( (a) & 1<<(b) )!=0 )
#define BCLR(a,b)	( (a) & ~(1<<(b)) )
#define BSET(a,b)	( (a) | 1<<(b) )

#define RGBDRAW		1


struct But
{
	short type,nr;
	char *str;
	char strdata[MAXBUTSTR];
	short x1,y1,x2,y2;
	char *poin;
	float min,max;
	short a1,a2,rt[4];
	void (*func)();

	void *font;
	char *tip, *lockstr;
	short lock, win;
	short col,drawtype;
};

struct ButCol
{
	short drawmode;
	short rgb;
	unsigned int back,paper_sel,paper_desel,pen_sel,pen_desel;
	unsigned int border1,border2,border3,border4;
	short drmode, rt;	/* drawmode is ook naam fie */
};

struct ButBlock
{
	struct ButBlock *next;
	struct But *first;
	int aantal,window;
	char naam[20];
};

struct Bgrp{
    struct Bgrp *next,*prev;
    short type,nr;
    char *str;
    short x1,y1,x2,y2;
    char *poin;
    float min,max;
    short a1,a2;
	
	void *font;
	short col,drawtype;
	void * func;
};

struct ButIcon {
	short xim, yim;
	unsigned int *rect;
	short xofs, yofs;
};

extern void AutoButFontSize(void *size8, void *size6);
extern void ButtonsGetmouse(short *adr);
extern struct But *DefBut(short type, short nr, char *str, short x1, short y1, short x2, short y2, void *poin, float min, float max, short a1, short a2);
extern struct But *DefButt(short type, short nr, char *str, short x1, short y1, short x2, short y2, void *poin, float min, float max, short a1, short a2, char *tip);
extern void DefButBlock(char *str, int win, void *font, short aantal, short col, short drawtype);
extern void ClearButLock();
extern void FreeButBlock(char *str);
extern void fdrawline(float x1, float y1, float x2,float y2);
extern void getname_menu_but(char *butname, char *str, int item);
extern void SetButFunc(void (*func)());
extern short DoButtons();
extern char *GetButTip();

extern void EmbossBox2(short x1, short y1, short x2, short y2, short sel, uint dark, uint light);


#endif /* BUTTON_H */

