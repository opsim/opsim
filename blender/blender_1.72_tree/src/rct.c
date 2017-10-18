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



/*
 * 
 * rct.c
 * 
 * april 95
 * 
 */

#include "blender.h"

int in_rcti(rect, x, y)
rcti *rect;
int x, y;
{

	if(x<rect->xmin) return 0;
	if(x>rect->xmax) return 0;
	if(y<rect->ymin) return 0;
	if(y>rect->ymax) return 0;
	return 1;
}

int in_rctf(rctf *rect, float x, float y)
{

	if(x<rect->xmin) return 0;
	if(x>rect->xmax) return 0;
	if(y<rect->ymin) return 0;
	if(y>rect->ymax) return 0;
	return 1;
}

void union_rctf(rctf *rct1, rctf *rct2)
{

	if(rct1->xmin>rct2->xmin) rct1->xmin= rct2->xmin;
	if(rct1->xmax<rct2->xmax) rct1->xmax= rct2->xmax;
	if(rct1->ymin>rct2->ymin) rct1->ymin= rct2->ymin;
	if(rct1->ymax<rct2->ymax) rct1->ymax= rct2->ymax;
}

void init_rctf(rctf *rect, float xmin, float xmax, float ymin, float ymax)
{
	rect->xmin= xmin;
	rect->xmax= xmax;
	rect->ymin= ymin;
	rect->ymax= ymax;
}

int isect_rctf(rctf *src1, rctf *src2, rctf *dest)
{
	float xmin, xmax;
	float ymin, ymax;

	xmin = MAX2(src1->xmin,src2->xmin);
	xmax = MIN2(src1->xmax,src2->xmax);
	ymin = MAX2(src1->ymin,src2->ymin);
	ymax = MIN2(src1->ymax,src2->ymax);
	
	if(xmax>=xmin && ymax>=ymin) {
		if(dest) {
			dest->xmin = xmin;
			dest->xmax = xmax;
			dest->ymin = ymin;
			dest->ymax = ymax;
		}
		return 1;
	}
	else {
		if(dest) {
			dest->xmin = 0;
			dest->xmax = 0;
			dest->ymin = 0;
			dest->ymax = 0;
		}
		return 0;
	}
}

