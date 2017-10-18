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


/* drawoops.c  juli 96		GRAPHICS
 * 
 * 
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "oops.h"

float oopscalex;

void boundbox_oops()
{
	Oops *oops;
	float min[2], max[2];
	int ok= 0;
	
	if(G.soops==0) return;
	
	min[0]= 1000.0;
	max[0]= -10000.0;
	min[1]= 1000.0;
	max[1]= -1000.0;
	
	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {
			ok= 1;
			
			min[0]= MIN2(min[0], oops->x);
			max[0]= MAX2(max[0], oops->x+OOPSX);
			min[1]= MIN2(min[1], oops->y);
			max[1]= MAX2(max[1], oops->y+OOPSY);
		}
		oops= oops->next;
	}
	
	if(ok==0) return;
	
	G.v2d->tot.xmin= min[0];
	G.v2d->tot.xmax= max[0];
	G.v2d->tot.ymin= min[1];
	G.v2d->tot.ymax= max[1];
	
}

void give_oopslink_line(Oops *oops, OopsLink *ol, float *v1, float *v2)
{

	if(ol->to && ol->to->hide==0) {
		v1[0]= oops->x+ol->xof;
		v1[1]= oops->y+ol->yof;
		v2[0]= ol->to->x+OOPSX/2;
		v2[1]= ol->to->y;
	}
	else if(ol->from && ol->from->hide==0) {
		v1[0]= ol->from->x + ol->xof;
		v1[1]= ol->from->y + ol->xof;
		v2[0]= oops->x+OOPSX/2;
		v2[1]= oops->y;
	}
}

void draw_oopslink(Oops *oops)
{
	OopsLink *ol;
	float vec[4];
	
	if(oops->type==ID_SCE) {
		if(oops->flag & SELECT) {
			if(oops->id->lib) cpack(0x4080A0);
			else cpack(0x808080);
		}
		else cpack(0x606060);
	}
	else {
		if(oops->flag & SELECT) {
			if(oops->id->lib) cpack(0x11AAFF);
			else cpack(0xFFFFFF);
		}
		else cpack(0x0);
	}
	
	ol= oops->link.first;
	while(ol) {
		if(ol->to && ol->to->hide==0) {
			
			give_oopslink_line(oops, ol, vec, vec+2);
			
			glBegin(GL_LINE_STRIP);
			glVertex2fv(vec);
			glVertex2fv(vec+2);
			glEnd();
		}
		ol= ol->next;
	}
}

void draw_icon_oops(float *co, short type)
{
	uint *rect;
	extern struct ButIcon BGicon[];
	
	rect= BGicon[0].rect;
	if(rect==0) return;
	
	/* geeft aan dat per scanline lrectwrite, xim in de rect verder gelezen wordt */

	glPixelStorei(GL_UNPACK_ROW_LENGTH,  BGicon[0].xim);

	/* eerste pixel berekenen */
	rect+= (9*21 + 3)*BGicon[0].xim + (6*20 + 3);
	
	switch(type) {
	case ID_OB:
		rect+= 20; break;
	case ID_ME:
		rect+= 40; break;
	case ID_CU:
		rect+= 60; break;
	case ID_MB:
		rect+= 80; break;
	case ID_LT:
		rect+= 100; break;
	case ID_LA:
		rect+= 120; break;
	case ID_MA:
		rect+= 140; break;
	case ID_TE:
		rect+= 160; break;
	case ID_IP:
		rect+= 180; break;
	case ID_LI:
		rect+= 200; break;
	case ID_IM:
		rect+= 220; break;
	}

	/* rasterpos3f for fakegl.c */
	glRasterPos3f(co[0],  co[1]-0.2, 0.0);
 	glDrawPixels(14, 14, GL_RGBA, GL_UNSIGNED_BYTE, rect);

	glPixelStorei(GL_UNPACK_ROW_LENGTH,  0);
}

void mysbox(float x1, float y1, float x2, float y2)
{
	float vec[2];
	
	glBegin(GL_LINE_LOOP);
	vec[0]= x1; vec[1]= y1;
	glVertex2fv(vec);
	vec[0]= x2;
	glVertex2fv(vec);
	vec[1]= y2;
	glVertex2fv(vec);
	vec[0]= x1;
	glVertex2fv(vec);
	glEnd();
}

uint give_oops_color(short type, short sel, uint *border)
{
	uint body;
	/* geeft ook aan of stippellijn getekend moet */

	switch(type) {
	case ID_OB:
		body= 0x606060; break;
	case ID_SCE:
		body= 0x507050; break;
	case ID_MA:
		body= 0x707050; break;
	case ID_TE:
		body= 0x607090; break;
	case ID_IP:
		body= 0x805040; break;
	case ID_LA:
		body= 0x507070; break;
	case ID_LI:
		body= 0x1188CC; break;
	case ID_IM:
		body= 0x25558F; break;
	default:
		body= 0x505060; break;
	}

	if(sel) {
		if(G.moving) *border= 0xE0E0E0;
		else *border= 0xB0B0B0; 
	}
	else *border= 0x0;

	
	return body;
}

void calc_oopstext(char *str, float *v1)
{
	float f1, f2, size;
	short mval[2], len, flen;
	
	ipoco_to_areaco_noclip(v1, mval);
	f1= mval[0];
	v1[0]+= OOPSX;
	ipoco_to_areaco_noclip(v1, mval);
	f2= mval[0];
	size= f2-f1;

	len= strlen(str);
	
	while( (flen= fmgetstrwidth(G.fonts, str)) > size) {
		if(flen < 10 || len<2) break;
		len--;
		str[len]= 0;
	}
	
	mval[0]= (f1+f2-flen+1)/2;
	mval[1]= 1;
	areamouseco_to_ipoco(mval, &f1, &f2);
	
	v1[0]= f1;
	
}

void draw_oops(Oops *oops)
{
	OopsLink *ol;
	float size, v1[2], x1, y1, x2, y2, f1, f2;
	uint body, border;
	short line= 0, mval[2];
	char str[32];
	
	x1= oops->x; x2= oops->x+OOPSX;
	y1= oops->y; y2= oops->y+OOPSY;
	
	if(x2 < G.v2d->cur.xmin || x1 > G.v2d->cur.xmax) return;
	if(y2 < G.v2d->cur.ymin || y1 > G.v2d->cur.ymax) return;

	body= give_oops_color(oops->type, oops->flag & SELECT, &border);
	if(oops->id== (ID *)OBACT) line= 1;
	else if(oops->id== (ID *)G.scene) line= 1;
			
	if(oops->id->us) {
		cpack(body);

		glRectf(x1,  y1,  x2,  y2);
	}
	if(oops->id->lib) {
		if(oops->id->flag & LIB_INDIRECT) cpack(0x1144FF);
		else cpack(0x11AAFF);

		glRectf(x2-0.2*OOPSX,  y2-0.2*OOPSX,  x2-0.1*OOPSX,  y2-0.1*OOPSX);
	}
	
	v1[0]= x1; v1[1]= (y1+y2)/2 -0.3;
	if(oops->type==ID_LI) {
		sprintf(str, "     %s", ((Library *)oops->id)->name);
	}
	else {
		sprintf(str, "     %s", oops->id->name+2);
	}
	calc_oopstext(str, v1);

	/* ICON */
	if(str[1] && oopscalex>1.0) {
		draw_icon_oops(v1, oops->type);
	}
	if(oops->flag & SELECT) cpack(0xFFFFFF); else cpack(0x0);
	fmsetfont(G.fonts);
	glRasterPos3f(v1[0],  v1[1], 0.0);
	fmprstr(str);


	if(line) setlinestyle(2);
	cpack(border);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
	glRectf(x1,  y1,  x2,  y2); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(line) setlinestyle(0);

	/* connectieblokjes */
	ol= oops->link.first;
	while(ol) {
		
		f1= x1+ol->xof; f2= y1+ol->yof;
		
		body= give_oops_color(ol->type, oops->flag & SELECT, &border);
		cpack(body);
	
		glRectf(f1-.2,  f2-.2,  f1+.2,  f2+.2);
		cpack(border);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

		glRectf(f1-.2,  f2-.2,  f1+.2,  f2+.2); 

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		ol= ol->next;
	}

	if(oops->flag & OOPS_REFER) {
		glTranslatef(oops->x,  oops->y,  0.0);
		cpack(0x0);

		glBegin(GL_POLYGON);
		v1[0]= 0.45*OOPSX; v1[1]= 0.0; glVertex2fv(v1);
		v1[0]= 0.46*OOPSX; v1[1]= 0.025*OOPSX; glVertex2fv(v1);
		v1[0]= 0.475*OOPSX; v1[1]= 0.04*OOPSX; glVertex2fv(v1);
		v1[0]= 0.5*OOPSX; v1[1]= 0.05*OOPSX; glVertex2fv(v1);
		v1[0]= 0.525*OOPSX; v1[1]= 0.04*OOPSX; glVertex2fv(v1);
		v1[0]= 0.54*OOPSX; v1[1]= 0.025*OOPSX; glVertex2fv(v1);
		v1[0]= 0.55*OOPSX; v1[1]= 0.0; glVertex2fv(v1);
		glEnd();
		glTranslatef(-oops->x,  -oops->y,  0.0);
	}
}

void drawoopsspace()
{
	Oops *oops;
	int ofsx, ofsy;

	glClearColor(0.4375, 0.4375, 0.4375, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	if(G.soops==0) return;	

	boundbox_oops();
	calc_scrollrcts();

	if(curarea->winx>SCROLLB+10 && curarea->winy>SCROLLH+10) {
		if(G.v2d->scroll) {	
			ofsx= curarea->winrct.xmin;	/* ivm mywin */
			ofsy= curarea->winrct.ymin;

			glViewport(ofsx+G.v2d->mask.xmin,  ofsy+G.v2d->mask.ymin, ( ofsx+G.v2d->mask.xmax-1)-(ofsx+G.v2d->mask.xmin)+1, ( ofsy+G.v2d->mask.ymax-1)-( ofsy+G.v2d->mask.ymin)+1); 
			glScissor(ofsx+G.v2d->mask.xmin,  ofsy+G.v2d->mask.ymin, ( ofsx+G.v2d->mask.xmax-1)-(ofsx+G.v2d->mask.xmin)+1, ( ofsy+G.v2d->mask.ymax-1)-( ofsy+G.v2d->mask.ymin)+1);
		}
	}

	ortho2(G.v2d->cur.xmin, G.v2d->cur.xmax, G.v2d->cur.ymin, G.v2d->cur.ymax);

	oopscalex= .14*((float)curarea->winx)/(G.v2d->cur.xmax-G.v2d->cur.xmin);
	calc_ipogrid();	/* voor scrollvariables */
	build_oops();

	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {
			draw_oopslink(oops);
		}
		oops= oops->next;
	}
	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {
			if(oops->flag & SELECT); else draw_oops(oops);
		}
		oops= oops->next;
	}
	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {
			if(oops->flag & SELECT) draw_oops(oops);
		}
		oops= oops->next;
	}
	
	/* restore viewport */
	winset(curarea->win);

	
	if(G.v2d->scroll) {	
		/* ortho op pixelnivo curarea */
		ortho2(-0.5, curarea->winx+0.5, -0.5, curarea->winy+0.5);
		drawscroll(0);		
	}
	curarea->win_swap= WIN_BACK_OK;
}




