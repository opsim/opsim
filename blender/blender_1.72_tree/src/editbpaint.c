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
 *  drawbpaint.c   mei 97     GRAPHICS
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "bpaint.h"

void fetch_image(SpacePaint *sp)
{
	int i;
	Image *ima;
	
	i = sp->image;
	ima = G.main->image.first;
	if (!ima) return;
	
	while(i){
		if (ima) ima = ima->id.next; else return;
		i--;
	}
	if (!(ima->id.flag & LIB_FAKEUSER)){	
		ima->id.flag = ima->id.flag | LIB_FAKEUSER;	
		ima->id.us++;
	}
		
	sp->work_image = ima;
	sp->workimage  = sp->image;
	if (sp->zoom < 1.0) sp->zoom = 1.0;
	
}

void do_paint_buttons(ushort event)
{
	if(curarea->win==0) return;

	switch(event) {
	case B_BP_STOWALL:
		addqueue(curarea->headwin, REDRAW, 1);
		addqueue(curarea->win, REDRAW, 1);
		break;
	case REDRAW:
		break;
	}
}

void menu_handler(short iconbut)
{
	SpacePaint *sp;
	sp = curarea->spacedata.first;

	switch(iconbut) {
	case BP_IMA_START:
		fetch_image(sp);
	break;
	}
}

void winqreadpaintspace(ushort event, short val)
{
	short mval[2], mousemove;
	int iconbut;
	SpacePaint			*sp;
	IconButtonManager	*ibm;
	
	if(val==0) return;
	
	sp  = curarea->spacedata.first;
	ibm = sp->ibm;
	mousemove = 0;
	
	switch(event) {
	case PADMINUS:
		sp->zoom--;
		if (sp->zoom < 1.0) sp->zoom = 1.0;
		sprintf(sp->ibm->text, "Zoom set to %.2f",sp->zoom); 
		addqueue(curarea->win, REDRAW, 1);
		break;
	case PADPLUSKEY:
		sp->zoom++;
		if (sp->zoom > 99.0) sp->zoom = 100.0;
		sprintf(sp->ibm->text, "Zoom set to %.2f",sp->zoom); 
		addqueue(curarea->win, REDRAW, 1);
		break;
	
	case MOUSEX:
	case MOUSEY:
		getmouseco_areawin(mval);	/* lokaal screen */
		sp->mousex = mval[0];
		sp->mousey = mval[1];
		mousemove  = 1;
		break;
	case MIDDLEMOUSE:
		if (sp->in_icon) {
			/* grep button */
		}else{
			if (sp->work_image) move_image(sp);
			addqueue(curarea->win, REDRAW, 1);
		}
		break;
	case RIGHTMOUSE:
	case LEFTMOUSE:
		if (sp->in_icon) {
			iconbut = icon_handler(&ibm, event);
			if (iconbut){
				menu_handler(iconbut);
				addqueue(curarea->win, REDRAW, 1);	
			}
		}
	}
	
	if ((mousemove) && (!sp->stowall)){
		sp->in_icon = above_icon(&ibm, sp->mousex, sp->mousey);
	}
}

