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

#include "imbuf.h"


#define OBJECTBLOK "rotate"


short _rot_crop = TRUE;

void rotcrop(int val)
{
	_rot_crop = val;
}

void flip270(struct ImBuf * ibuf)
{
	struct ImBuf * tbuf;
	int x, y;
	uint * rect, *frect;
	int skip;

	if (ibuf == 0) return;
	if (ibuf->rect == 0) return;

	tbuf = allocImBuf(ibuf->y, ibuf->x, 32, 1, 0);
	if (tbuf == 0) return;

	rect = ibuf->rect;
	skip = ibuf->y;

	for (y = ibuf->y - 1; y >= 0; y--){
		frect = tbuf->rect + y;
		for (x = ibuf->x; x > 0; x--){
			*frect = *rect++;
			frect += skip;
		}
	}

	memcpy(ibuf->rect, tbuf->rect, ibuf->x * ibuf->y * sizeof(uint));
	x = ibuf->x;
	ibuf->x = ibuf->y;
	ibuf->y = x;
	freeImBuf(tbuf);
}


void flip90(struct ImBuf * ibuf)
{
	struct ImBuf * tbuf;
	int x, y;
	uint * rect, *frect;
	int skip;

	if (ibuf == 0) return;
	if (ibuf->rect == 0) return;

	tbuf = allocImBuf(ibuf->y, ibuf->x, 32, 1, 0);
	if (tbuf == 0) return;

	frect = tbuf->rect;
	skip = tbuf->y;

	for (y = tbuf->y - 1; y >= 0; y--){
		rect = ibuf->rect + y;
		for (x = tbuf->x; x > 0; x--){
			*frect++ = *rect;
			rect += skip;
		}
	}

	memcpy(ibuf->rect, tbuf->rect, ibuf->x * ibuf->y * sizeof(uint));
	x = ibuf->x;
	ibuf->x = ibuf->y;
	ibuf->y = x;
	freeImBuf(tbuf);
}


void flipy(struct ImBuf * ibuf)
{
	short x,y,backx;
	uint *top,*bottom,temp;

	if (ibuf == 0) return;
	if (ibuf->rect == 0) return;

	x = ibuf->x;
	y = ibuf->y;
	backx = x<<1;

	top = ibuf->rect;
	bottom = top + ((y-1) * x);
	y >>= 1;

	for(;y>0;y--){
		for(x = ibuf->x; x > 0; x--){
			temp = *top;
			*(top++) = *bottom;
			*(bottom++) = temp;
		}
		bottom -= backx;
	}
}


void flipx(struct ImBuf * ibuf)
{
	short x, y, halfx;
	uint *left, *right, temp, leftadd, rightadd;

	if (ibuf == 0) return;
	if (ibuf->rect == 0) return;

	halfx = ibuf->x >> 1;
	left = ibuf->rect;
	right = left + ibuf->x - 1;

	leftadd = ibuf->x - halfx;
	rightadd = ibuf->x + halfx;

	for(y = ibuf->y; y > 0; y--){
		for(x = halfx; x > 0; x--){
			temp = *left;
			*(left++) = *right;
			*(right--) = temp;
		}
		left += leftadd;
		right += rightadd;
	}
}


void scroll(ibuf,offs,dir)
struct ImBuf *ibuf;
short offs,dir;
{
	uchar *point;
	int size;

	size = (ibuf->x*ibuf->y)-1;
	point = (uchar *)ibuf->rect;
	point += offs;

	if (dir>0){
		point += size<<2;
		for(;size>0;size--){
			point[4] = point[0];
			point -= 4;
		}
	} else{
		for(;size>0;size--){
			point[0] = point[4];
			point += 4;
		}
	}
}

void cropibuf(struct ImBuf * ibuf)
{
	int x, y, found, step, and;
	short x1, y1, x2, y2;
	uint *rect;
	struct ImBuf tbuf;
	extern void rectcpy();

	if (ibuf == 0) return;
	if (ibuf->rect == 0) return;

	rect = ibuf->rect;
	found = FALSE;
	for (y = 0; y<ibuf->y ; y++){
		for (x = ibuf->x; x>0; x--){
			if (*rect++) {
				found = TRUE;
				break;
			}
		}
		if (found) break;
	}

	if (found == FALSE){
		ibuf->x = 1;
		ibuf->y = 1;
		return;
	}

	y1 = y;

	rect = ibuf->rect;
	rect += ibuf->x * ibuf->y - 1;

	found = FALSE;
	for (y = ibuf->y - 1; y >= 0 ; y--){
		for (x = ibuf->x; x>0; x--){
			if (*rect--) {
				found = TRUE;
				break;
			}
		}
		if (found) break;
	}

	y2 = y;


	step = ibuf->x;
	found = FALSE;
	for (x = 0; x < ibuf->x ; x++){
		rect = ibuf->rect + x;
		for (y = ibuf->y; y > 0; y--){
			if (*rect) {
				found = TRUE;
				break;
			}
			rect += step;
		}
		if (found) break;
	}

	x1 = x;

	step = ibuf->x;
	found = FALSE;
	for (x = ibuf->x - 1; x > 0; x--){
		rect = ibuf->rect + x;
		for (y = ibuf->y; y > 0; y--){
			if (*rect) {
				found = TRUE;
				break;
			}
			rect += step;
		}
		if (found) break;
	}

	x2 = x;

	/* dit moet veilig gaan */

	tbuf = *ibuf;
	tbuf.x = x2 - x1 + 1;
	tbuf.y = y2 - y1 + 1;

	rectop(&tbuf, ibuf, 0, 0, x1, y1, 32767, 32767, rectcpy);
	ibuf->x = tbuf.x;
	ibuf->y = tbuf.y;

	ibuf->xorig += x1;
	ibuf->yorig += y1;
}


#define MINSHDIST 0.001

void shearx(struct ImBuf *ibuf, float dist)
{
	struct ImBuf *tbuf;
	uchar *srect, *drect;
	int x, y;
	float w1, w2, tdist;
	float a, b, g, r, la, lb, lg, lr;
	extern void rectfill();

	if (ibuf == 0) return;
	if (ibuf->rect == 0) return;

	tdist = fabsf(dist * ibuf->y);
	if (tdist < MINSHDIST) return;

	tbuf = allocImBuf(ibuf->x + 1 + tdist, ibuf->y, 32, 1, 0);
	rectoptot(tbuf, 0, rectfill, 0);
	if (tbuf == 0) return;

	if (dist > 0) tdist = 0.0;

	for (y = 0; y < ibuf->y; y++){
		srect = (uchar *) (ibuf->rect + y * ibuf->x);
		drect = (uchar *) (tbuf->rect + y * tbuf->x);
		drect += ((int) tdist) * sizeof(uint);
		w1 = tdist - floorf(tdist);
		tdist += dist;
		if (w1 <= MINSHDIST) {
			memcpy(drect, srect, ibuf->x * sizeof(uint));
		} else if ( w1 >= 1.0 - MINSHDIST) {
			memcpy(drect + sizeof(uint), srect, ibuf->x * sizeof(uint));
		} else {
			w2 = 1.0 - w1;
			la = lb = lg = lr = 0.0;
			for (x = ibuf->x; x > 0; x--){
				a = srect[0]; 
				b = srect[1];
				g = srect[2]; 
				r = srect[3];
				drect[0] = (w1 * la) + (w2 * a) + 0.5;
				drect[1] = (w1 * lb) + (w2 * b) + 0.5;
				drect[2] = (w1 * lg) + (w2 * g) + 0.5;
				drect[3] = (w1 * lr) + (w2 * r) + 0.5;
				srect += 4; 
				drect += 4;
				la = a; 
				lb = b; 
				lg = g; 
				lr = r;
			}
			drect[0] = (w1 * la) + 0.5;
			drect[1] = (w1 * lb) + 0.5;
			drect[2] = (w1 * lg) + 0.5;
			drect[3] = (w1 * lr) + 0.5;
		}
	}

	freerectImBuf(ibuf);
	ibuf->mall |= IB_rect;
	ibuf->rect = tbuf->rect;
	tbuf->rect = 0;
	ibuf->x = tbuf->x;

	freeImBuf(tbuf);
}


void rotibuf(struct ImBuf *ibuf, double angle)
{
	angle = fmodf(angle, 360.0);
	if (angle < 0) angle = 360.0 + angle;

	if (angle < 45.0) {
		/* do nothing */
	} else if (angle < 135.0) {
		flip90(ibuf);
		angle -= 90;
	} else if (angle < 225.0) {
		flipx(ibuf);
		flipy(ibuf);
		angle -= 180;
	} else if (angle < 315.0) {
		flip270(ibuf);
		angle -= 270;
	} else angle = angle - 360;

	angle = angle * M_PI / 180.0;
	shearx(ibuf, -tan(angle / 2.0));
	flip270(ibuf);
	shearx(ibuf, -sin(angle));
	flip90(ibuf);
	shearx(ibuf, -tan(angle / 2.0));
	if (_rot_crop) cropibuf(ibuf);
}

