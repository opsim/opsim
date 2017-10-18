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
 *  drawbpaint.c   maart 97     GRAPHICS
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "bpaint.h"

#define XIC 20
#define YIC 21

int	get_win_mouse(short *mval)
{
	static Device mdev[2]= {MOUSEX, MOUSEY};
	static short oval[2] ={0, 0};
	int rv = 0;
	
	getmouseco_sc(mval);
	
	if (mval[0] != oval[0]) rv = 1;
	if (mval[1] != oval[1]) rv = 1;
	
	oval[0] = mval[0];
	oval[1] = mval[1];
	return rv;	
}



void paint_buttons()
{
	SpacePaint *sp;
	int xco;
	char naam[256];

	sp= curarea->spacedata.first;

	sprintf(naam, "header %d", curarea->headwin);
	DefButBlock(naam, curarea->headwin, G.font, 35, 2, 2);

	DefBut(ICONROW|CHA,B_NEWSPACE, "ICON 0 0 0", 6,0,XIC,YIC, &(curarea->butspacetype), 1.0, 10.0, 0, 0);

	/* FULL WINDOW */
	xco= 25;
	if(curarea->full) DefBut(BUT,B_FULL, "ICON 0 1 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0);
	else DefBut(BUT,B_FULL, "ICON 0 0 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0);
	
	/* STOW ALL */
	if(sp->stowall) DefBut(TOG|SHO,B_BP_STOWALL, "ICON 0 2 8",	xco+=XIC,0,XIC,YIC, &sp->stowall, 0, 0, 0, 0);
	else DefBut(TOG|SHO,B_BP_STOWALL, "ICON 0 3 8",	xco+=XIC,0,XIC,YIC, &sp->stowall, 0, 0, 0, 0);
	
	/* altijd als laatste doen */
	curarea->headbutlen= xco+2*XIC;
}

void draw_work_image(SpacePaint *sp)
{
	int	xmin, ymin, xmax, ymax, x1, y1;
	Image *ima;
	ImBuf *ibuf;
	ima = sp->work_image;
	
	if (!ima) return;
	if (!ima->ok) return;
	
	if (!ima->ibuf){
		ima->ibuf = loadiffname(ima->name, IB_rect);
		if (!ima->ibuf){ ima->ok = 0; return; }
	}
	ibuf = ima->ibuf;
	
	if (sp->zoom <  1.0)  sp->zoom =   1.0;
	if (sp->zoom > 99.0)  sp->zoom = 100.0;
	
	xmin= curarea->winrct.xmin; xmax= curarea->winrct.xmax;
	ymin= curarea->winrct.ymin; ymax= curarea->winrct.ymax;
	
	x1= xmin+(curarea->winx- sp->zoom*ibuf->x)/2;
	y1= ymin+(curarea->winy- sp->zoom*ibuf->y)/2;

	x1-= sp->zoom * sp->lrect_sx;
	y1-= sp->zoom * sp->lrect_sy;

	rectwrite_part(xmin, ymin, xmax, ymax,	x1, y1,	ibuf->x, ibuf->y, sp->zoom, sp->zoom, ibuf->rect);
}

void drawpaintspace()
{
	SpacePaint *sp;
	IconButtonManager *ibm;
	
	sp= curarea->spacedata.first;
	
	glClearColor(0.3125, 0.3125, 0.3125, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	ibm = sp->ibm;
	
	ibm->screen_x			= curarea->winrct.xmin;
	ibm->screen_y			= curarea->winrct.ymin;
	ibm->window_y_location	= curarea->winy;
	
	if (sp->work_image) draw_work_image(sp);

	if (!sp->stowall){
		check_image_count(sp);
		draw_icon_windows(&ibm);
	}
	get_stowing(&ibm, &(sp->stowing));
}

void move_image(SpacePaint *sp)
{
	ImBuf *ibuf;
	static Device mdev[2]= {MOUSEX, MOUSEY};
	short mval[2], oval[2];
	
	if (!sp->work_image) return;
	
	set_icon_cursor(sp->ibm, 17, 2);
	get_win_mouse(oval);
	
	oval[0] += sp->lrect_sx * sp->zoom;
	oval[1] += sp->lrect_sy * sp->zoom;
	while(get_mbut()&M_MOUSE) {
		if (get_win_mouse(mval)){
			sp->lrect_sx = (oval[0] - mval[0]) / sp->zoom;
			sp->lrect_sy = (oval[1] - mval[1]) / sp->zoom;
			
			drawpaintspace();
			myswapbuffers();
		}
	}
}

