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


/* drawipo.c  mei 94		GRAPHICS
 * 
 * 
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "ipo.h"
#include "sequence.h"

#define IPOBUTX	65
#define IPOSTEP 35			/* minimum pixels per gridstep */


void *ipofont;
float ipogrid_dx, ipogrid_dy, ipogrid_startx, ipogrid_starty;
int ipomachtx, ipomachty;

int vertymin, vertymax, horxmin, horxmax;	/* globals om LEFTMOUSE op scrollbar te testen */



void scroll_prstr(float x, float y, float val, char dir, int disptype)
{
	int len, macht;
	char str[32];
	
	if(dir=='v') {
		macht= ipomachty;
		if ELEM(disptype, IPO_DISPDEGR, IPO_DISPTIME) {
			macht+=1;
			val *= 10;
		}
	}
	else macht= ipomachtx;
	
	switch(macht) {
	case 0:
		sprintf(str, "%.1f", val);
		break;
	case -1:
		sprintf(str, "%.2f", val);
		break;
	case -2:
		sprintf(str, "%.3f", val);
		break;
	case -3:
		sprintf(str, "%.4f", val);
		break;
	case -4:
		sprintf(str, "%.5f", val);
		break;
	case -5:
		sprintf(str, "%.6f", val);
		break;
	default:
		if(macht>0) 
			sprintf(str, "%d", (int)val);
		else
			sprintf(str, "%.7f", val);
		
		break;
	}
	
	len= strlen(str);
	if(dir=='h') x-= 4*len;
	else y-= 4*len;
	
	if(dir=='v' && disptype==IPO_DISPDEGR) {
		str[len]= 186;
		str[len+1]= 0;
	}
	
	glRasterPos2f(x,  y);
	fmprstr(str);
}

void step_to_grid(float *step, int *macht)
{
	float loga, rem;

	/* proberen step als 10e macht te schrijven */
	
	loga= flog10(*step);
	*macht= (int)(loga);
	
	rem= loga- *macht;
	rem= powf(10.0, rem);

	if(loga<0.0) {
		if(rem < 0.2) rem= 0.2;
		else if(rem < 0.5) rem= 0.5;
		else rem= 1.0;

		*step= rem*powf(10.0, (float)*macht);
	}
	else {
		if(rem < 2.0) rem= 2.0;
		else if(rem < 5.0) rem= 5.0;
		else rem= 10.0;

		*step= rem*powf(10.0, (float)*macht);

		(*macht)++;
	}
	
}

void calc_ipogrid()
{
	float space, pixels;
	
	/* regel: gridstep is minimaal IPOSTEP pixels */
	/* hoe groot zijn IPOSTEP pixels? */
	
	if(G.v2d==0) return;

	space= G.v2d->cur.xmax - G.v2d->cur.xmin;
	pixels= G.v2d->mask.xmax-G.v2d->mask.xmin;

	ipogrid_dx= IPOSTEP*space/pixels;
	step_to_grid(&ipogrid_dx, &ipomachtx);
	
	if(curarea->spacetype==SPACE_SEQ) {
		if(ipogrid_dx < 1.0) ipogrid_dx= 1.0;
		ipomachtx-= 2;
		if(ipomachtx<-1) ipomachtx= -1;
	}

	space= (G.v2d->cur.ymax - G.v2d->cur.ymin);
	pixels= curarea->winy;
	ipogrid_dy= IPOSTEP*space/pixels;
	step_to_grid(&ipogrid_dy, &ipomachty);
	if(curarea->spacetype==SPACE_SEQ) {
		if(ipogrid_dy < 1.0) ipogrid_dy= 1.0;
		if(ipomachty<1) ipomachty= 1;
	}
	
	ipogrid_startx= G.v2d->cur.xmin-fmodf(G.v2d->cur.xmin, ipogrid_dx);
	if(G.v2d->cur.xmin<0.0) ipogrid_startx-= ipogrid_dx;
	ipogrid_starty= (G.v2d->cur.ymin-fmodf(G.v2d->cur.ymin, ipogrid_dy));
	if(G.v2d->cur.ymin<0.0) ipogrid_starty-= ipogrid_dy;
	
}

void draw_ipogrid()
{
	float vec1[2], vec2[2];
	int a, step;

	vec1[0]= vec2[0]= ipogrid_startx;
	vec1[1]= ipogrid_starty;
	vec2[1]= G.v2d->cur.ymax;
	
	step= (G.v2d->mask.xmax-G.v2d->mask.xmin+1)/IPOSTEP;

	glColor3ub(0x40, 0x40, 0x40);
	for(a=0; a<step; a++) {
		glBegin(GL_LINE_STRIP);
			glVertex2fv(vec1); glVertex2fv(vec2);
		glEnd();
		vec2[0]= vec1[0]+= ipogrid_dx;
	}
	
	vec2[0]= vec1[0]-= 0.5*ipogrid_dx;
	glColor3ub(0x50, 0x50, 0x50);
	step++;
	for(a=0; a<=step; a++) {
		glBegin(GL_LINE_STRIP);
			glVertex2fv(vec1); glVertex2fv(vec2);
		glEnd();
		vec2[0]= vec1[0]-= ipogrid_dx;
	}

	vec1[0]= ipogrid_startx;
	vec1[1]= vec2[1]= ipogrid_starty;
	vec2[0]= G.v2d->cur.xmax;
	
	step= (curarea->winy+1)/IPOSTEP;
	
	glColor3ub(0x40, 0x40, 0x40);
	for(a=0; a<=step; a++) {
		glBegin(GL_LINE_STRIP);
			glVertex2fv(vec1); glVertex2fv(vec2);
		glEnd();
		vec2[1]= vec1[1]+= ipogrid_dy;
	}
	vec2[1]= vec1[1]-= 0.5*ipogrid_dy;
	step++;
	
	if(curarea->spacetype==SPACE_IPO) {
		glColor3ub(0x50, 0x50, 0x50);
		for(a=0; a<step; a++) {
			glBegin(GL_LINE_STRIP);
				glVertex2fv(vec1); glVertex2fv(vec2);
			glEnd();
			vec2[1]= vec1[1]-= ipogrid_dy;
		}
	}
	
	/* assenkruis */
	glColor3ub(0, 0, 0);
	vec1[0]= G.v2d->cur.xmin;
	vec2[0]= G.v2d->cur.xmax;
	vec1[1]= vec2[1]= 0.0;
	glBegin(GL_LINE_STRIP);
		glVertex2fv(vec1); glVertex2fv(vec2);
	glEnd();

	vec1[1]= G.v2d->cur.ymin;
	vec2[1]= G.v2d->cur.ymax;
	vec1[0]= vec2[0]= 0.0;
	glBegin(GL_LINE_STRIP);
		glVertex2fv(vec1); glVertex2fv(vec2);
	glEnd();
	
	if(curarea->spacetype==SPACE_IPO) {

		if(G.sipo->blocktype==ID_SEQ) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRectf(0.0,  0.0,  100.0,  1.0); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}	else if(G.sipo->blocktype==ID_CU) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRectf(0.0,  1.0,  G.v2d->cur.xmax,  1.0); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	
}

void areamouseco_to_ipoco(short *mval, float *x, float *y)
{
	float div, ofs;
	
	div= G.v2d->mask.xmax-G.v2d->mask.xmin;
	ofs= G.v2d->mask.xmin;
	
	*x= G.v2d->cur.xmin+ (G.v2d->cur.xmax-G.v2d->cur.xmin)*(mval[0]-ofs)/div;

	div= G.v2d->mask.ymax-G.v2d->mask.ymin;
	ofs= G.v2d->mask.ymin;

	*y= G.v2d->cur.ymin+ (G.v2d->cur.ymax-G.v2d->cur.ymin)*(mval[1]-ofs)/div;
}

void ipoco_to_areaco(float *vec, short *mval)
{
	float x, y;

	mval[0]= 3200;
	
	x= (vec[0] - G.v2d->cur.xmin)/(G.v2d->cur.xmax-G.v2d->cur.xmin);
	if(x>=0.0 && x<=1.0) {
		y= (vec[1] - G.v2d->cur.ymin)/(G.v2d->cur.ymax-G.v2d->cur.ymin);
		if(y>=0.0 && y<=1.0) {
			
			mval[0]= G.v2d->mask.xmin + x*(G.v2d->mask.xmax-G.v2d->mask.xmin);
			mval[1]= G.v2d->mask.ymin + y*(G.v2d->mask.ymax-G.v2d->mask.ymin);
			
		}
	}
}

void ipoco_to_areaco_noclip(float *vec, short *mval)
{
	float x, y;

	x= (vec[0] - G.v2d->cur.xmin)/(G.v2d->cur.xmax-G.v2d->cur.xmin);

	y= (vec[1] - G.v2d->cur.ymin)/(G.v2d->cur.ymax-G.v2d->cur.ymin);

		
	x= G.v2d->mask.xmin + x*(G.v2d->mask.xmax-G.v2d->mask.xmin);
	y= G.v2d->mask.ymin + y*(G.v2d->mask.ymax-G.v2d->mask.ymin);
	
	if(x<-32760) mval[0]= -32760;
	else if(x>32760) mval[0]= 32760;
	else mval[0]= x;

	if(y<-32760) mval[1]= -32760;
	else if(y>32760) mval[1]= 32760;
	else mval[1]= y;
}

int in_ipo_buttons()
{
	short mval[2];
	
	getmouseco_areawin(mval);
	
	if(mval[0]< G.v2d->mask.xmax) return 0;
	else return 1;
}


void test_view2d()
{
	/* cur mag niet groter dan max, kleiner dan min of buiten tot vallen */
	rctf *cur, *tot;
	float dx, dy, temp, fac, asp, zoom;
	
	cur= &G.v2d->cur;
	tot= &G.v2d->tot;
	
	dx= cur->xmax-cur->xmin;
	dy= cur->ymax-cur->ymin;
	
	if(G.v2d->keepzoom) {
		/* niet op min/max testen: ahv curarea de zoom fixeren */
		zoom= ((float)curarea->winx)/dx;
			
		if(zoom<G.v2d->minzoom || zoom>G.v2d->maxzoom) {
			if(zoom<G.v2d->minzoom) fac= zoom/G.v2d->minzoom;
			else fac= zoom/G.v2d->maxzoom;
			
			dx*= fac;
			temp= 0.5*(cur->xmax+cur->xmin);

			cur->xmin= temp-0.5*dx;
			cur->xmax= temp+0.5*dx;
		}
		zoom= ((float)curarea->winy)/dy;
			
		if(zoom<G.v2d->minzoom || zoom>G.v2d->maxzoom) {
			if(zoom<G.v2d->minzoom) fac= zoom/G.v2d->minzoom;
			else fac= zoom/G.v2d->maxzoom;
		
			dy*= fac;
			temp= 0.5*(cur->ymax+cur->ymin);
			cur->ymin= temp-0.5*dy;
			cur->ymax= temp+0.5*dy;
		}
	}
	else {
		if(dx<G.v2d->min[0]) {
			dx= G.v2d->min[0];
			temp= 0.5*(cur->xmax+cur->xmin);
			cur->xmin= temp-0.5*dx;
			cur->xmax= temp+0.5*dx;
		}
		else if(dx>G.v2d->max[0]) {
			dx= G.v2d->max[0];
			temp= 0.5*(cur->xmax+cur->xmin);
			cur->xmin= temp-0.5*dx;
			cur->xmax= temp+0.5*dx;
		}
		
		if(dy<G.v2d->min[1]) {
			dy= G.v2d->min[1];
			temp= 0.5*(cur->ymax+cur->ymin);
			cur->ymin= temp-0.5*dy;
			cur->ymax= temp+0.5*dy;
		}
		else if(dy>G.v2d->max[1]) {
			dy= G.v2d->max[1];
			temp= 0.5*(cur->ymax+cur->ymin);
			cur->ymin= temp-0.5*dy;
			cur->ymax= temp+0.5*dy;
		}
	}
	
	if(G.v2d->keeptot) {
		dx= cur->xmax-cur->xmin;
		dy= cur->ymax-cur->ymin;
	
		if(dx > tot->xmax-tot->xmin) {
			if(G.v2d->keepzoom==0) {
				if(cur->xmin<tot->xmin) cur->xmin= tot->xmin;
				if(cur->xmax>tot->xmax) cur->xmax= tot->xmax;
			}
			else {
				if(cur->xmax < tot->xmax) {
					dx= tot->xmax-cur->xmax;
					cur->xmin+= dx;
					cur->xmax+= dx;
				}
				else if(cur->xmin > tot->xmin) {
					dx= cur->xmin-tot->xmin;
					cur->xmin-= dx;
					cur->xmax-= dx;
				}
			}
		}
		else {
			if(cur->xmin < tot->xmin) {
				dx= tot->xmin-cur->xmin;
				cur->xmin+= dx;
				cur->xmax+= dx;
			}
			else if(cur->xmax > tot->xmax) {
				dx= cur->xmax-tot->xmax;
				cur->xmin-= dx;
				cur->xmax-= dx;
			}
		}
		
		if(dy > tot->ymax-tot->ymin) {
			if(G.v2d->keepzoom==0) {
				if(cur->ymin<tot->ymin) cur->ymin= tot->ymin;
				if(cur->ymax>tot->ymax) cur->ymax= tot->ymax;
			}
			else {
				if(cur->ymax < tot->ymax) {
					dy= tot->ymax-cur->ymax;
					cur->ymin+= dy;
					cur->ymax+= dy;
				}
				else if(cur->ymin > tot->ymin) {
					dy= cur->ymin-tot->ymin;
					cur->ymin-= dy;
					cur->ymax-= dy;
				}
			}
		}
		else {
			if(cur->ymin < tot->ymin) {
				dy= tot->ymin-cur->ymin;
				cur->ymin+= dy;
				cur->ymax+= dy;
			}
			else if(cur->ymax > tot->ymax) {
				dy= cur->ymax-tot->ymax;
				cur->ymin-= dy;
				cur->ymax-= dy;
			}
		}
	}
	
	if(G.v2d->keepaspect) {
		dx= (cur->ymax-cur->ymin)/(cur->xmax-cur->xmin);
		dy= ((float)curarea->winy)/((float)curarea->winx);
		
		/* dx/dy is de totale aspect */

		if(dx/dy>1.0) {
			
			/* portrait window: x corrigeren */
			dx= cur->ymax-cur->ymin;
			temp= (cur->xmax+cur->xmin);
			
			cur->xmin= temp/2.0 - 0.5*dx/dy;
			cur->xmax= temp/2.0 + 0.5*dx/dy;
		}
		else {
			dx= cur->xmax-cur->xmin;
			temp= (cur->ymax+cur->ymin);
			
			cur->ymin= temp/2.0 - 0.5*dy*dx;
			cur->ymax= temp/2.0 + 0.5*dy*dx;
		}
	}

	
}

void calc_scrollrcts()
{

	G.v2d->mask.xmin= G.v2d->mask.ymin= 0;
	G.v2d->mask.xmax= curarea->winx;
	G.v2d->mask.ymax= curarea->winy;

	if(curarea->spacetype==SPACE_IPO) {
		G.v2d->mask.xmax-= IPOBUTX;
		
		if(G.v2d->mask.xmax<IPOBUTX) G.v2d->mask.xmax= curarea->winx;
	}
	
	if(G.v2d->scroll) {
		if(G.v2d->scroll & L_SCROLL) {
			G.v2d->vert= G.v2d->mask;
			G.v2d->vert.xmax= SCROLLB;
			G.v2d->mask.xmin= SCROLLB+1;
		}
		else if(G.v2d->scroll & R_SCROLL) {
			G.v2d->vert= G.v2d->mask;
			G.v2d->vert.xmin= G.v2d->vert.xmax-SCROLLB;
			G.v2d->mask.xmax= G.v2d->vert.xmin-1;
		}

		if(G.v2d->scroll & B_SCROLL) {
			G.v2d->hor= G.v2d->mask;
			G.v2d->hor.ymax= SCROLLH;
			G.v2d->mask.ymin= SCROLLH+1;
		}
		else if(G.v2d->scroll & T_SCROLL) {
			G.v2d->hor= G.v2d->mask;
			G.v2d->hor.ymin= G.v2d->hor.ymax-SCROLLH;
			G.v2d->mask.ymax= G.v2d->hor.ymin-1;
		}
	}
}

void draw_solution()
{
	EditIpo *ei;
	float vec[2];
	int a, b, val;
	short mval[2];
	
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
			if(G.v2d->scroll & VERT_SCROLL) {
				cpack(ei->col);
				
				if(ei->disptype==IPO_DISPBITS) {
					val= ei->icu->curval;
					b= 0;
					while(b<31) {
						if( val & (1<<b) ) {
							vec[0]= G.v2d->cur.xmin;
							vec[1]= b+1;
							ipoco_to_areaco(vec, mval);
							if(mval[0]!=3200) {
								vec[0]= G.v2d->vert.xmin;
								vec[1]= mval[1];
								glBegin(GL_LINE_STRIP);
									glVertex2fv(vec);
									vec[0]= G.v2d->vert.xmax;
									glVertex2fv(vec);
								glEnd();
							}
						}
						b++;
					}
				}
				else {
					vec[0]= G.v2d->cur.xmin;
					vec[1]= ei->icu->curval;
					ipoco_to_areaco(vec, mval);
					if(mval[0]!=3200) {
						vec[0]= G.v2d->vert.xmin;
						vec[1]= mval[1];
						glBegin(GL_LINE_STRIP);
							glVertex2fv(vec);
							vec[0]= G.v2d->vert.xmax;
							glVertex2fv(vec);
						glEnd();
					}
				}
			}
		}
	}
}

void rgbmul(uint *col, int mul)
{
	int temp;
	char *rt;
	
	rt= (char *)col;

	temp= (mul*rt[1])>>8;
	if(temp>255) rt[1]= 255; else rt[1]= temp;
	temp= (mul*rt[2])>>8;
	if(temp>255) rt[2]= 255; else rt[2]= temp;
	temp= (mul*rt[3])>>8;
	if(temp>255) rt[3]= 255; else rt[3]= temp;
}

void drawscroll(int disptype)
{
	rcti vert, hor;
	float fac, dfac, val, fac2, tim;
	uint dark, darker, light;
	char str[30];

	vert= (G.v2d->vert);
	hor= (G.v2d->hor);
	
	darker= dark= light= 0x686868;
	rgbmul(&darker, 175);
	rgbmul(&dark, 215);
	rgbmul(&light, 325);
	
	cpack(dark);
	if(G.v2d->scroll & HOR_SCROLL) {
		glRecti(hor.xmin,  hor.ymin,  hor.xmax,  hor.ymax);
		glColor3ub(0, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRecti(hor.xmin,  hor.ymin,  hor.xmax,  hor.ymax); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/* slider */
		fac= (G.v2d->cur.xmin- G.v2d->tot.xmin)/(G.v2d->tot.xmax-G.v2d->tot.xmin);
		if(fac<0.0) fac= 0.0;
		horxmin= hor.xmin+fac*(hor.xmax-hor.xmin) + 1;
		
		fac= (G.v2d->cur.xmax- G.v2d->tot.xmin)/(G.v2d->tot.xmax-G.v2d->tot.xmin);
		if(fac>1.0) fac= 1.0;
		horxmax= hor.xmin+fac*(hor.xmax-hor.xmin) -1;
		
		if(horxmin > horxmax-2) horxmin= horxmax-2;
		
		glColor3ub(0x68, 0x68, 0x68);
		glRecti(horxmin,  hor.ymin+1,  horxmax,  hor.ymax-1);

		cpack(light);
		sdrawline(horxmin,  hor.ymax-1,  horxmax,  hor.ymax-1);  /* boven */
		sdrawline(horxmin,  hor.ymin+1,  horxmin,  hor.ymax-1);  /* links */
		cpack(darker);
		sdrawline(horxmin,  hor.ymin+1,  horxmax,  hor.ymin+1);  /* onder */
		sdrawline(horxmax,  hor.ymin+1,  horxmax,  hor.ymax-1);  /* rechts */
		
		/* de cijfers: ipogrid_startx en -dx omzetten naar scroll coordinaten */
		fac= (ipogrid_startx- G.v2d->cur.xmin)/(G.v2d->cur.xmax-G.v2d->cur.xmin);
		fac= hor.xmin+fac*(hor.xmax-hor.xmin);

		dfac= (ipogrid_dx)/(G.v2d->cur.xmax-G.v2d->cur.xmin);
		dfac= dfac*(hor.xmax-hor.xmin);

		fmsetfont(ipofont);
	
		glColor3ub(0, 0, 0);
		val= ipogrid_startx;
		while(fac < hor.xmax) {
			
			if(curarea->spacetype==SPACE_SEQ) {
				fac2= val/(float)G.scene->r.frs_sec;
				tim= ffloor(fac2);
				fac2= fac2-tim;
				scroll_prstr(fac, 3.0+(float)(hor.ymin), tim+G.scene->r.frs_sec*fac2/100.0, 'h', disptype);
			}
			else scroll_prstr(fac, 3.0+(float)(hor.ymin), val, 'h', disptype);
			
			fac+= dfac;
			val+= ipogrid_dx;
		}
	}
	cpack(dark);
	if(G.v2d->scroll & VERT_SCROLL) {
		glRecti(vert.xmin,  vert.ymin,  vert.xmax,  vert.ymax);
		glColor3ub(0, 0, 0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRecti(vert.xmin,  vert.ymin,  vert.xmax,  vert.ymax); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		/* slider */
		fac= (G.v2d->cur.ymin- G.v2d->tot.ymin)/(G.v2d->tot.ymax-G.v2d->tot.ymin);
		if(fac<0.0) fac= 0.0;
		vertymin= vert.ymin+fac*(vert.ymax-vert.ymin) + 1;
		
		fac= (G.v2d->cur.ymax- G.v2d->tot.ymin)/(G.v2d->tot.ymax-G.v2d->tot.ymin);
		if(fac>1.0) fac= 1.0;
		vertymax= vert.ymin+fac*(vert.ymax-vert.ymin) -1;
		
		if(vertymin > vertymax-2) vertymin= vertymax-2;
		
		glColor3ub(0x68, 0x68, 0x68);
		glRecti(vert.xmin+1,  vertymin,  vert.xmax-1,  vertymax);
		
		cpack(light);

		sdrawline(vert.xmin+1,  vertymax,  vert.xmax-1,  vertymax);  /* boven */
		sdrawline(vert.xmin+1,  vertymin,  vert.xmin+1,  vertymax);  /* links */
		cpack(darker);
		sdrawline(vert.xmin+1,  vertymin,  vert.xmax-1,  vertymin);  /* onder */
		sdrawline(vert.xmax-1,  vertymin,  vert.xmax-1,  vertymax);  /* rechts */
		
		/* de cijfers: ipogrid_starty en -dy omzetten naar scroll coordinaten */
		fac= (ipogrid_starty- G.v2d->cur.ymin)/(G.v2d->cur.ymax-G.v2d->cur.ymin);
		fac= vert.ymin+SCROLLH+fac*(vert.ymax-vert.ymin-SCROLLH);

		dfac= (ipogrid_dy)/(G.v2d->cur.ymax-G.v2d->cur.ymin);
		dfac= dfac*(vert.ymax-vert.ymin-SCROLLH);
	
		if(curarea->spacetype==SPACE_SEQ) {
			fmsetfont(G.font);
		
			glColor3ub(0, 0, 0);
			val= ipogrid_starty;
			fac+= 0.5*dfac;
			while(fac < vert.ymax) {
				scroll_prstr((float)(vert.xmax)-14.0, fac, val, 'v', disptype);
				fac+= dfac;
				val+= ipogrid_dy;
			}
		}
		else {
			fmsetfont(ipofont);
			fmrotatepagematrix(90.0);
		
			glColor3ub(0, 0, 0);
			val= ipogrid_starty;
			while(fac < vert.ymax) {
				scroll_prstr((float)(vert.xmax)-3.0, fac, val, 'v', disptype);
				fac+= dfac;
				val+= ipogrid_dy;
			}
			
			fmrotatepagematrix(-90.0);
		}
	}
}

void draw_ipobuts()
{
	EditIpo *ei;
	int a, y, sel, tot;
	char naam[20];
	
	if(curarea->winx<IPOBUTX) return;

	if(G.sipo->butofs) {
		tot= 30+IPOBUTY*G.sipo->totipo;
		if(tot<curarea->winy) G.sipo->butofs= 0;
	}
	
	drawedge(G.v2d->mask.xmax+3, 0, G.v2d->mask.xmax+3, curarea->winy);
	glColor3ub(0x6a, 0x60, 0x60);

	glRects(G.v2d->mask.xmax+6,  0,  curarea->winx,  curarea->winy);
	
	if(G.sipo->totipo==0) return;
	if(G.sipo->editipo==0) return;

	sprintf(naam, "win %d", curarea->win);
	DefButBlock(naam, curarea->win, G.font, 32, 10, 3);

	FrontbufferButs(0);

	ei= G.sipo->editipo;
	y= curarea->winy-30+G.sipo->butofs;
	for(a=0; a<G.sipo->totipo; a++, ei++, y-=IPOBUTY) {
		
		if(ei->icu) {
			cpack(ei->col);

			glRects(G.v2d->mask.xmax+8,  y+3,  G.v2d->mask.xmax+14,  y+16);
			sel= ei->flag & (IPO_SELECT + IPO_EDIT);
			EmbossBox2(G.v2d->mask.xmax+7, y+2, G.v2d->mask.xmax+15, y+IPOBUTY-2, sel, 0x0, 0xFFFFFF);
		}

		DefBut(TOG|INT|BIT|a, a+1, ei->name,  G.v2d->mask.xmax+20, y, IPOBUTX-15, IPOBUTY-1, &(G.sipo->rowbut), 0, 0, 0, 0);
		
		if(G.sipo->blocktype==ID_KE) {
			if(TEST_C_KEY==0) break;
			else {
				Key *key= (Key *)G.sipo->from;
				if(key==0 || key->type==KEY_NORMAL) break;
			}
		}
	}
}

void calc_ipoverts()
{
	EditIpo *ei;
	BezTriple *bezt;
	BPoint *bp;
	int a, b;
	
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
			if(ei->icu->bezt) {
				bezt= ei->icu->bezt;
				b= ei->icu->totvert;
				while(b--) {
					ipoco_to_areaco(bezt->vec[0], bezt->s[0]);
					ipoco_to_areaco(bezt->vec[1], bezt->s[1]);
					ipoco_to_areaco(bezt->vec[2], bezt->s[2]);
					bezt++;
				}
			}
			else if(ei->icu->bp) {
				bp= ei->icu->bp;
				b= ei->icu->totvert;
				while(b--) {
					ipoco_to_areaco(bp->vec, bp->s);
					bp++;
				}
			}
		}
	}
}


void draw_ipovertices(int sel)
{
	EditIpo *ei;
	BezTriple *bezt;
	float v1[2];
	uint col, selcol, deselcol;
	int val, ok, nr, a, b;
	short mval[2];
	
	if(G.f & G_PICKSEL) return;

	glPointSize(3.0);
	glBegin(GL_POINTS);

	ei= G.sipo->editipo;
	for(nr=0; nr<G.sipo->totipo; nr++, ei++) {
		if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
			
			if(G.sipo->showkey) {
				if(sel) col= 0xFFFFFF; else col= 0x0;
			} else if(ei->flag & IPO_EDIT) {
				if(sel) col= 0x77FFFF; else col= 0xFF70FF;
			} else {
				if(sel) col= 0xFFFFFF; else col= 0x0;
				val= (ei->icu->flag & IPO_SELECT)!=0;
				if(sel != val) continue;
			}
			
			cpack(col);
			
			bezt= ei->icu->bezt;
			a= ei->icu->totvert;
			while(a--) {				
					
				if(ei->disptype==IPO_DISPBITS) {
					ok= 0;
					if(ei->flag & IPO_EDIT) {
						if( (bezt->f2 & 1) == sel ) ok= 1;
					}
					else ok= 1;
					
					if(ok) {
						val= bezt->vec[1][1];
						b= 0;
						v1[0]= bezt->vec[1][0];
	
						while(b<31) {
							if(val & (1<<b)) {	
								v1[1]= b+1;
								glVertex3fv(v1);
							}
							b++;
						}
					}
				}
				else {
					
					if(ei->flag & IPO_EDIT) {
						if(ei->icu->ipo==IPO_BEZ) {
							if( (bezt->f1 & 1) == sel )	
								glVertex3fv(bezt->vec[0]);
							if( (bezt->f3 & 1) == sel )	
								glVertex3fv(bezt->vec[2]);
						}
						if( (bezt->f2 & 1) == sel )	
							glVertex3fv(bezt->vec[1]);
						
					}
					else {
						glVertex3fv(bezt->vec[1]);
					}
				}

				bezt++;
			}
		}
	}
	
	glEnd();
	glPointSize(1.0);
}

void draw_ipohandles(int sel)
{
	extern uint nurbcol[];
	EditIpo *ei;
	BezTriple *bezt;
	float *fp;
	uint *col;
	int a, b;

	if(sel) col= nurbcol+4;
	else col= nurbcol;

	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN4(ei, flag & IPO_VISIBLE, flag & IPO_EDIT, icu, disptype!=IPO_DISPBITS) {
			if(ei->icu->ipo==IPO_BEZ) {
				bezt= ei->icu->bezt;
				b= ei->icu->totvert;
				while(b--) {
			
					if( (bezt->f2 & 1)==sel) {
						fp= bezt->vec[0];
						cpack(col[bezt->h1]);

						glBegin(GL_LINE_STRIP); 
						glVertex2fv(fp); glVertex2fv(fp+3); 
							glEnd();
						cpack(col[bezt->h2]);

						glBegin(GL_LINE_STRIP); 
							glVertex2fv(fp+3); glVertex2fv(fp+6); 
						glEnd();
					}
					else if( (bezt->f1 & 1)==sel) {
						fp= bezt->vec[0];
						cpack(col[bezt->h1]);

						glBegin(GL_LINE_STRIP); 
							glVertex2fv(fp); glVertex2fv(fp+3); 
						glEnd();
					}
					else if( (bezt->f3 & 1)==sel) {
						fp= bezt->vec[1];
						cpack(col[bezt->h2]);

						glBegin(GL_LINE_STRIP); 
							glVertex2fv(fp); glVertex2fv(fp+3); 
						glEnd();
					}
			
					bezt++;
				}
			}
		}
	}
}

int pickselcode;

void init_pickselcode()
{
	pickselcode= 1;
}

void draw_ipocurves(int sel)
{
	EditIpo *ei;
	IpoCurve *icu;
	BezTriple *bezt, *prevbezt;
	BPoint *bp;
	float *fp, fac, data[120], v1[2], v2[2], v3[2], v4[2];
	float cycdx=0, cycdy=0, cycxofs, cycyofs;
	int a, b, resol, size, col, cycount, val, nr;
	short mval[2];
	
	
	ei= G.sipo->editipo;
	for(nr=0; nr<G.sipo->totipo; nr++, ei++) {
		if ISPOIN3(ei, flag & IPO_VISIBLE, icu, icu->bezt) {
			
			if(G.f & G_PICKSEL) {
				glLoadName(pickselcode++);
				val= 1;
			}
			else {
				val= (ei->flag & (IPO_SELECT+IPO_EDIT))!=0;
				val= (val==sel);
			}
			
			if(val) {
			
				cycyofs= cycxofs= 0.0;
				cycount= 1;
			
				icu= ei->icu;	
		
				/* curve */
				if(G.sipo->showkey) glColor3ub(0, 0, 0); else cpack(ei->col);
	
				/* cyclic */
				if(icu->extrap & IPO_CYCL) {
					cycdx= (icu->bezt+icu->totvert-1)->vec[1][0] - icu->bezt->vec[1][0];
					cycdy= (icu->bezt+icu->totvert-1)->vec[1][1] - icu->bezt->vec[1][1];
					if(cycdx>0.01) {
	
						while(icu->bezt->vec[1][0]+cycxofs > G.v2d->cur.xmin) {
							cycxofs-= cycdx;
							if(icu->extrap & IPO_DIR) cycyofs-= cycdy;
							cycount++;
						}
						bezt= icu->bezt+(icu->totvert-1);
						fac= 0.0;
						while(bezt->vec[1][0]+fac < G.v2d->cur.xmax) {
							cycount++;
							fac+= cycdx;
						}
					}
				}
				
				while(cycount--) {
					
					if(ei->disptype==IPO_DISPBITS) {
						
						/* lijnen */
						cpack(ei->col);
						bezt= icu->bezt;
						a= icu->totvert;
						
						while(a--) {
							val= bezt->vec[1][1];
							b= 0;
							
							while(b<31) {
								if(val & (1<<b)) {
									v1[1]= b+1;
									
									glBegin(GL_LINE_STRIP);
										if(icu->extrap & IPO_CYCL) ;
										else if(a==icu->totvert-1) {
											v1[0]= G.v2d->cur.xmin+cycxofs;
											glVertex2fv(v1);
										}
										v1[0]= bezt->vec[1][0]+cycxofs;
										glVertex2fv(v1); 
										
										if(a) v1[0]= (bezt+1)->vec[1][0]+cycxofs;
										else if(icu->extrap & IPO_CYCL) ;
										else v1[0]= G.v2d->cur.xmax+cycxofs;
										
										glVertex2fv(v1);
									glEnd();
								}
								b++;
							}
							bezt++;
						}
						
					}
					else {
			
						b= icu->totvert-1;
						prevbezt= icu->bezt;
						bezt= prevbezt+1;
						
						glBegin(GL_LINE_STRIP);
		
						/* extrap naar links? */
						if( (icu->extrap & IPO_CYCL)==0) {
							if(prevbezt->vec[1][0] > G.v2d->cur.xmin) {
								v1[0]= G.v2d->cur.xmin;
								if(icu->extrap==IPO_HORIZ || icu->ipo==IPO_CONST) v1[1]= prevbezt->vec[1][1];
								else {
									fac= (prevbezt->vec[0][0]-prevbezt->vec[1][0])/(prevbezt->vec[1][0]-v1[0]);
									if(fac!=0.0) fac= 1.0/fac;
									v1[1]= prevbezt->vec[1][1]-fac*(prevbezt->vec[0][1]-prevbezt->vec[1][1]);
								}
								glVertex2fv(v1);
							}
						}
						
						if(b==0) {
							v1[0]= prevbezt->vec[1][0]+cycxofs;
							v1[1]= prevbezt->vec[1][1]+cycyofs;
							glVertex2fv(v1);
						}
						
						while(b--) {
							if(icu->ipo==IPO_CONST) {
								v1[0]= prevbezt->vec[1][0]+cycxofs;
								v1[1]= prevbezt->vec[1][1]+cycyofs;
								glVertex2fv(v1);
								v1[0]= bezt->vec[1][0]+cycxofs;
								v1[1]= prevbezt->vec[1][1]+cycyofs;
								glVertex2fv(v1);
							}
							else if(icu->ipo==IPO_LIN) {
								v1[0]= prevbezt->vec[1][0]+cycxofs;
								v1[1]= prevbezt->vec[1][1]+cycyofs;
								glVertex2fv(v1);
							}
							else {
								resol= 3.0*fsqrt(bezt->vec[1][0] - prevbezt->vec[1][0]);
								
								if(resol<2) {
									v1[0]= prevbezt->vec[1][0]+cycxofs;
									v1[1]= prevbezt->vec[1][1]+cycyofs;
									glVertex2fv(v1);
								}
								else {
									if(resol>32) resol= 32;
									
									v1[0]= prevbezt->vec[1][0]+cycxofs;
									v1[1]= prevbezt->vec[1][1]+cycyofs;
									v2[0]= prevbezt->vec[2][0]+cycxofs;
									v2[1]= prevbezt->vec[2][1]+cycyofs;
									
									v3[0]= bezt->vec[0][0]+cycxofs;
									v3[1]= bezt->vec[0][1]+cycyofs;
									v4[0]= bezt->vec[1][0]+cycxofs;
									v4[1]= bezt->vec[1][1]+cycyofs;
			
									correct_bezpart(v1, v2, v3, v4);
									
									maakbez(v1[0], v2[0], v3[0], v4[0], data, resol);
									maakbez(v1[1], v2[1], v3[1], v4[1], data+1, resol);
									
									fp= data;
									while(resol--) {
										glVertex2fv(fp);
										fp+= 3;
									}
								}
							}
							prevbezt= bezt;
							bezt++;
							
							/* laatste punt? */
							if(b==0) {
								v1[0]= prevbezt->vec[1][0]+cycxofs;
								v1[1]= prevbezt->vec[1][1]+cycyofs;
								glVertex2fv(v1);
							}
						}
						
						/* extrap naar rechts? */
						if( (icu->extrap & IPO_CYCL)==0) {
							if(prevbezt->vec[1][0] < G.v2d->cur.xmax) {
								v1[0]= G.v2d->cur.xmax;
								if(icu->extrap==IPO_HORIZ || icu->ipo==IPO_CONST) v1[1]= prevbezt->vec[1][1];
								else {
									fac= (prevbezt->vec[2][0]-prevbezt->vec[1][0])/(prevbezt->vec[1][0]-v1[0]);
									if(fac!=0.0) fac= 1.0/fac;
									v1[1]= prevbezt->vec[1][1]-fac*(prevbezt->vec[2][1]-prevbezt->vec[1][1]);
								}
								glVertex2fv(v1);
							}
						}
						
						glEnd();
						
					}
					cycxofs+= cycdx;
					if(icu->extrap & IPO_DIR) cycyofs+= cycdy;
				}
					
				/* lijn die het einde van een snelheidscurve aangeeft */
				if(G.sipo->blocktype==ID_CU && icu->adrcode==CU_SPEED) {
					b= icu->totvert-1;
					if(b) {
						glColor3ub(0, 0, 0);
						bezt= icu->bezt+b;
						v1[0]= bezt->vec[1][0];
						v1[1]= 0.0;
						LINE2F(v1, bezt->vec[1]);
					}
				}
			}
		}
	}
}

void draw_cfra()
{
	Sequence *seq;
	Object *ob;
	float vec[2];
	
	if(G.sipo->blocktype==ID_SEQ) {
		vec[0]= -100.0;

		seq= (Sequence *)G.sipo->from;
		if(seq) {
			vec[0]= (CFRA-seq->startdisp);
			vec[0]= 100.0*vec[0]/((float)(seq->enddisp-seq->startdisp));
		}
	}
	else {
		vec[0]= CFRA;
		vec[0]*= G.scene->r.framelen;
	}

	vec[1]= G.v2d->cur.ymin;
	glColor3ub(0x40, 0xA0, 0x20);
	glLineWidth(2.0);

	glBegin(GL_LINE_STRIP);
		glVertex2fv(vec);
		vec[1]= G.v2d->cur.ymax;
		glVertex2fv(vec);
	glEnd();
	
	if(G.sipo->blocktype==ID_OB) {
		ob= OBACT;
		if(ob && ob->sf!=0.0 && (ob->ipoflag & OB_OFFS_OB) ) {
			vec[0]-= ob->sf;
			
			glColor3ub(0x10, 0x60, 0);

			glBegin(GL_LINE_STRIP);
				glVertex2fv(vec);
				vec[1]= G.v2d->cur.ymin;
				glVertex2fv(vec);
			glEnd();
		}
	}
	
	glLineWidth(1.0);
}

void draw_ipokey()
{
	IpoKey *ik;
	float vec[2];
	
	if(G.sipo->ipokey.first==0) make_ipokey();
	
	ik= G.sipo->ipokey.first;
	while(ik) {
		if(ik->flag & 1) glColor3ub(0xFF, 0xFF, 0x99);
		else glColor3ub(0xAA, 0xAA, 0x55);
		
		vec[0]= ik->val;
		vec[1]= G.v2d->cur.ymin;

		glBegin(GL_LINE_STRIP);
			glVertex2fv(vec);
			vec[1]= G.v2d->cur.ymax;
			glVertex2fv(vec);
		glEnd();
		
		ik= ik->next;
	}
}

void draw_key(int visible)
{
	Key *key;
	KeyBlock *kb, *act=NULL;
	float vec[2];
	uint col;
	
	key= (Key *)G.sipo->from;
	if(key==0) return;

	if(key->type== KEY_RELATIVE) if(visible==0) return;
	
	kb= key->block.first;
	while(kb) {
		if(kb->type==KEY_LINEAR) setlinestyle(2);
		else if(kb->type==KEY_BSPLINE) setlinestyle(4);
		else setlinestyle(0);
		
		if(kb==key->refkey) col= 0x22FFFF;
		else col= 0xFFFF00;
		
		if( (kb->flag & SELECT)==0)	col-= 0x225500;
		else act= kb;
		
		cpack(col);
	
		vec[0]= G.v2d->cur.xmin;
		vec[1]= kb->pos;
	
		glBegin(GL_LINE_STRIP);
			glVertex2fv(vec);
			vec[0]= G.v2d->cur.xmax;
			glVertex2fv(vec);
		glEnd();
		
		kb= kb->next;
	}

	if(act) {
		if(act->type==KEY_LINEAR) setlinestyle(2);
		else if(act->type==KEY_BSPLINE) setlinestyle(4);
		else setlinestyle(0);
		
		if(act==key->refkey) cpack(0x22FFFF);
		else cpack(0xFFFF00);

		vec[0]= G.v2d->cur.xmin;
		vec[1]= act->pos;
	
		glBegin(GL_LINE_STRIP);
			glVertex2fv(vec);
			vec[0]= G.v2d->cur.xmax;
			glVertex2fv(vec);
		glEnd();
	}
	
	setlinestyle(0);
}

void drawipo()
{
	EditIpo *ei;
	int ofsx, ofsy, a, disptype;

	glClearColor(.40625, .40625, .40625, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	calc_scrollrcts();

	if(curarea->winx>SCROLLB+10 && curarea->winy>SCROLLH+10) {
		if(G.v2d->scroll) {	
			ofsx= curarea->winrct.xmin;	/* ivm mywin */
			ofsy= curarea->winrct.ymin;
			glViewport(ofsx+G.v2d->mask.xmin,  ofsy+G.v2d->mask.ymin, ( ofsx+G.v2d->mask.xmax-1)-(ofsx+G.v2d->mask.xmin)+1, ( ofsy+G.v2d->mask.ymax-1)-( ofsy+G.v2d->mask.ymin)+1); 
			glScissor(ofsx+G.v2d->mask.xmin,  ofsy+G.v2d->mask.ymin, ( ofsx+G.v2d->mask.xmax-1)-(ofsx+G.v2d->mask.xmin)+1, ( ofsy+G.v2d->mask.ymax-1)-( ofsy+G.v2d->mask.ymin)+1);
		}
	}

	test_editipo();	/* test of huidige editipo klopt, make_editipo zet de v2d->cur */

	ortho2(G.v2d->cur.xmin, G.v2d->cur.xmax, G.v2d->cur.ymin, G.v2d->cur.ymax);
	
	if(G.sipo->editipo) {

		/* schaal corrigeren voor graden? */
		disptype= -1;
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++, ei++) {
			if(ei->flag & IPO_VISIBLE) {
				if(disptype== -1) disptype= ei->disptype;
				else if(disptype!=ei->disptype) disptype= 0;
			}
		}
		
		calc_ipogrid();	
		draw_ipogrid();
	
		calc_ipoverts();
		
		draw_cfra();
		
		/* ipokeys */
		if(G.sipo->showkey) draw_ipokey();
		
		if(G.sipo->blocktype==ID_KE) {
			ei= G.sipo->editipo;
			draw_key(ei->flag & IPO_VISIBLE);
		}
		/* draw deselect */
		draw_ipocurves(0);
		draw_ipohandles(0);
		draw_ipovertices(0);
		
		/* draw select */
		draw_ipocurves(1);
		draw_ipohandles(1);
		draw_ipovertices(1);
		
		/* restore viewport */
		winset(curarea->win);
	
		if(curarea->winx>SCROLLB+10 && curarea->winy>SCROLLH+10) {
			
			/* ortho op pixelnivo curarea */
			ortho2(-0.5, curarea->winx+0.5, -0.5, curarea->winy+0.5);
			
			if(G.v2d->scroll) {
				drawscroll(disptype);
				draw_solution();
			}
			
			draw_ipobuts();		
		}
	}
	else {
		calc_ipogrid();
		draw_ipogrid();
	}
	
	curarea->win_swap= WIN_BACK_OK;
}

void scroll_ipobuts()
{
	int tot;
	short yo, mval[2];
	
	tot= 30+IPOBUTY*G.sipo->totipo;
	if(tot<curarea->winy) return;
	
	getmouseco_areawin(mval);
	yo= mval[1];
	
	while(get_mbut()&M_MOUSE) {
		getmouseco_areawin(mval);
		if(mval[1]!=yo) {
			G.sipo->butofs+= (mval[1]-yo);
			if(G.sipo->butofs<0) G.sipo->butofs= 0;
			else if(G.sipo->butofs+curarea->winy>tot) G.sipo->butofs= tot-curarea->winy;
			
			curarea->windraw();
			screen_swapbuffers();
			
			yo= mval[1];
		}
		else usleep(2);
	}
}



void view2dzoom()
{
	float fac, dx, dy;
	short mval[2], mvalo[2];
	
	areawinset(curarea->win);	/* vanuit buttons */
	curarea->head_swap= 0;
	getmouseco_areawin(mvalo);
	
	while(get_mbut()&(L_MOUSE|M_MOUSE)) {
		getmouseco_areawin(mval);
		
		if(mval[0]!=mvalo[0] || mval[1]!=mvalo[1]) {
		
			fac= 0.001*(mval[0]-mvalo[0]);
			dx= fac*(G.v2d->cur.xmax-G.v2d->cur.xmin);
			fac= 0.001*(mval[1]-mvalo[1]);
			dy= fac*(G.v2d->cur.ymax-G.v2d->cur.ymin);
			
			G.v2d->cur.xmin+= dx;
			G.v2d->cur.xmax-= dx;
			if(curarea->spacetype!=SPACE_SEQ) {
				G.v2d->cur.ymin+= dy;
				G.v2d->cur.ymax-= dy;
			}
			
			test_view2d();

			curarea->windraw();
			screen_swapbuffers();
		}
		else usleep(2);
	}
}

int view2dmove()
{
	/* return 1 als er iets gedaan is */
	float facx=0.0, facy=0.0, dx, dy, left=1.0, right=1.0;
	short mval[2], mvalo[2], leftret=1;
	
	/* alles goedzetten */
	curarea->windraw();
	curarea->head_swap= 0;
	
	if(G.qual & LR_CTRLKEY) {
		view2dzoom();
		curarea->head_swap= 0;
		return 0;
	}
		
	/* testen waar muis is */
	getmouseco_areawin(mvalo);
	
	if ELEM3(curarea->spacetype, SPACE_IPO, SPACE_SEQ, SPACE_OOPS) {
	
		if( in_rcti(&G.v2d->mask, (int)mvalo[0], (int)mvalo[1]) ) {
			facx= (G.v2d->cur.xmax-G.v2d->cur.xmin)/(float)(G.v2d->mask.xmax-G.v2d->mask.xmin);
			facy= (G.v2d->cur.ymax-G.v2d->cur.ymin)/(float)(G.v2d->mask.ymax-G.v2d->mask.ymin);
		}
		else if(in_rcti(&G.v2d->vert, (int)mvalo[0], (int)mvalo[1])) {
			facy= -(G.v2d->tot.ymax-G.v2d->tot.ymin)/(float)(G.v2d->mask.ymax-G.v2d->mask.ymin);
			if(get_mbut()&L_MOUSE) {
				/* welk deel van de scrollbar moet bewegen? */
				if(mvalo[1]< (vertymin+vertymax)/2 ) right= 0.0;
				else left= 0.0;
				leftret= 0;
			}
		}
		else if(in_rcti(&G.v2d->hor, (int)mvalo[0], (int)mvalo[1])) {
			facx= -(G.v2d->tot.xmax-G.v2d->tot.xmin)/(float)(G.v2d->mask.xmax-G.v2d->mask.xmin);
			if(get_mbut()&L_MOUSE) {
				/* welk deel van de scrollbar moet bewegen? */
				if(mvalo[0]< (horxmin+horxmax)/2 ) right= 0.0;
				else left= 0.0;
				leftret= 0;
			}
		}
	}
	else {
		facx= (G.v2d->cur.xmax-G.v2d->cur.xmin)/(float)(curarea->winx);
		facy= (G.v2d->cur.ymax-G.v2d->cur.ymin)/(float)(curarea->winy);		
	}
	
	if(get_mbut()&L_MOUSE && leftret) return 0;
	if(facx==0.0 && facy==0.0) return 1;
	
	while(get_mbut()&(L_MOUSE|M_MOUSE)) {
		getmouseco_areawin(mval);
		if(mval[0]!=mvalo[0] || mval[1]!=mvalo[1]) {
			dx= facx*(mvalo[0]-mval[0]);
			dy= facy*(mvalo[1]-mval[1]);
			G.v2d->cur.xmin+= left*dx;
			G.v2d->cur.xmax+= right*dx;
			G.v2d->cur.ymin+= left*dy;
			G.v2d->cur.ymax+= right*dy;
			
			test_view2d();
			
			curarea->windraw();
			screen_swapbuffers();
			
			mvalo[0]= mval[0];
			mvalo[1]= mval[1];
			
		}
		else usleep(2);
	}
	curarea->head_swap= 0;
	return 1;
}


void view2dborder()
{
	
}

EditIpo *select_proj_ipo(rctf *rectf, int event)
{
	EditIpo *ei;
	float xmin, ymin, xmax, ymax;
	IGLuint buffer[MAXPICKBUF];
	int a, b;
	int code, hits;
	short mval[2];
	
	G.f |= G_PICKSEL;

	if(rectf==0) {
		getmouseco_areawin(mval);
		
		mval[0]-= 6; mval[1]-= 6;
		areamouseco_to_ipoco(mval, &xmin, &ymin);
		mval[0]+= 12; mval[1]+= 12;
		areamouseco_to_ipoco(mval, &xmax, &ymax);
		
		ortho2(xmin, xmax, ymin, ymax);
	}
	else ortho2(rectf->xmin, rectf->xmax, rectf->ymin, rectf->ymax);
	
	glSelectBuffer( MAXPICKBUF, buffer); 
	glRenderMode(GL_SELECT);
	glInitNames();	/* deze twee fies zijn waarvoor? Anders werkt het niet */
	glPushName(-1);
	
	init_pickselcode();	/* drawipo.c */
	draw_ipocurves(0);	

	G.f -= G_PICKSEL;
	
	hits= glRenderMode(GL_RENDER);
	glPopName();	/* zie boven (pushname) */
	if(hits<1) return 0;
	
	code= 1;
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN(ei, icu, flag & IPO_VISIBLE) {
			if(rectf) {
				for(b=0; b<hits; b++) {
					if(code==buffer[ SELBUFFER_INDEX(b) ]) {
						if(event==LEFTMOUSE) ei->flag |= IPO_SELECT;
						else ei->flag &= ~IPO_SELECT;
						ei->icu->flag= ei->flag;
					}
				}
			}
			else {
				if(code==buffer[ SELBUFFER_INDEX(0) ]) return ei;
			}
			code++;
		}
	}
	return 0;
}


