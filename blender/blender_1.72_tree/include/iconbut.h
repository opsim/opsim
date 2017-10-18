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

/*

 *
 *	iconbut.h    mei 97
 *
 */

#ifndef ICONBUT_H
#define ICONBUT_H

/* key extra's */
#define IC_PLAIN 0
#define IC_SHIFT 1<< 9
#define IC_ALT   1<<10
#define IC_CTRL  1<<11

/* window types */
#define	IC_INFO   1
#define	IC_BUTROW 2
#define	IC_LNKROW 4
#define	IC_IMAGE  8
#define	IC_FREE	  1<<31

/* icon icon types */
#define IC_HEADER	1<<31
#define IC_NOBOX	1<<30
#define IC_ROW		1<<29
#define IC_IMATOG	1<<28
#define IC_TOG		1<<27
#define IC_BUT		1<<26
#define IC_PUT		1<<25

/* icon link position types */
#define IC_POS_HEADER 1
#define IC_POS_GRID   2
#define IC_POS_COORD  3
#define IC_PUT_GRID   4
#define IC_PUT_RESET  5

/* window status */
#define IC_STOW    1
#define IC_STOW2   2
#define IC_STOW3   4

#define IC_MOVE   64
#define IC_MENU  128

/* icon forced draw settings */
#define IC_RESET  0
#define IC_SET    1
#define IC_BACK   0
#define IC_FRONT  2

typedef struct IconButton{
	
	struct IconButton *next;
	
	char text[24];
	short leftmouse, rightmouse; 
	int hotkey;
	
	short ipx[2], ipy[2]; /* image start x,y */
	short ixs[2], iys[2]; /* image x,y size  */
	
	short set, min, max, xtra;
	uint *adress;
	uint icontype;
	
}IconButton;

typedef struct IconLink{
	
	struct IconLink		*next;
	struct IconButton	*button;
	
	short sx, sy, ex, ey;
	
	char  text[24];
	short ipx[2], ipy[2];
	short ixs[2], iys[2];
	int  val;
	uint *adress;

}IconLink;

typedef struct IconWindow{
	
	struct IconWindow *next;
	char name[24];
	
	uint header;
	short type, extra;
	short buttons, ot;
	short id, status;

	short ix, iy;
	short cloc[2], csiz[2];
	
	ImBuf	 *ibuf;
	
	IconLink *_firstlink;
	IconLink *prevlink;

}IconWindow;

typedef struct IconButtonManager{
	
	int *font;
	
	IconWindow  *_firstwindow;
	IconWindow  *prevwindow;
	
	IconButton  *_firstbutton;
	IconButton  *prevbutton;
	
	int    icon_image_x_size;
	uint *icon_image_rect;
	
	short screen_x, screen_y;
	short window_y_location, dummy;
	short header_height, header_witdh;
	short currentwindow, maxwindows;
	
	short currentbutton, maxbuttons;
	short gridx	, gridy;
	
	char text[32];
	short in_icon, info_on;
	IconLink	*inlink;
	IconWindow  *inwindow;
	IconWindow  *infowindow;
	
} IconButtonManager;

	/* iconbut.c */
extern void get_stowing(IconButtonManager **bm, uint *stowing);
extern void set_icon_manager(IconButtonManager **bm, int *font,uint *rect, int ima_x_size, short gridx, short gridy, short header_witdh, short header_height);


#endif	/* ICONBUT_H */


