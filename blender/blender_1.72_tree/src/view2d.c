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


/* view2d.c		GRAPHICS
 * 
 * maart 95
 * 
 * 
 * 
 */

#include "blender.h"
#include "graphics.h"

float win2dmat[4][4];
int win2dsizex, win2dsizey;
int win2dorx, win2dory;

void init_view2d_calc()
{

	mygetsingmatrix(win2dmat);
	getsize(&win2dsizex, &win2dsizey);
	mygetsuborigin(&win2dorx, &win2dory);
}		


void view2d_to_window(float *x, float *y)	/* voor rectwrite b.v. */
{
	float gx, gy;
	int sx, sy;
	
	gx= *x;
	gy= *y;
	*x= win2dsizex*(0.5+ 0.5*(gx*win2dmat[0][0]+ gy*win2dmat[1][0]+ win2dmat[3][0]));
	*y= win2dsizey*(0.5+ 0.5*(gx*win2dmat[0][1]+ gy*win2dmat[1][1]+ win2dmat[3][1]));
	
	*x+= win2dorx;
	*y+= win2dory;
}



void window_to_view2d(float *x, float *y)	/* voor muiscursor b.v. */
{
	float a, b, c, d, e, f, px, py;
	
	a= .5*win2dsizex*win2dmat[0][0];
	b= .5*win2dsizex*win2dmat[1][0];
	c= .5*win2dsizex*(1.0+win2dmat[3][0]);

	d= .5*win2dsizey*win2dmat[0][1];
	e= .5*win2dsizey*win2dmat[1][1];
	f= .5*win2dsizey*(1.0+win2dmat[3][1]);
	
	px= *x;
	py= *y;
	
	*y=  (a*(py-f) + d*(c-px))/(a*e-d*b);
	*x= (px- b*(*y)- c)/a;
	
}


