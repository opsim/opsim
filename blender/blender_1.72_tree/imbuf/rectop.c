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


#define OBJECTBLOK "rectop"

void rectxor(drect,srect,x)
int *drect,*srect,x;
{
	for(;x>0;x--){
		*drect++ ^= *srect++;
	}
}

void rectmakepremul(drect,srect,x)
uchar *drect,*srect,x;
{
	uchar alpha;
	
	for(;x>0;x--){
		alpha = drect[0];
		drect[1] = (drect[1] * alpha ) / 255;
		drect[2] = (drect[2] * alpha ) / 255;
		drect[3] = (drect[3] * alpha ) / 255;
		drect += 4;
	}
}

void rectmakegenlock(drect,srect,x)
uchar *drect,*srect,x;
{
	int col;
	uchar alpha;
	
	for(;x>0;x--){
		alpha = drect[0];
		if (alpha) {
			col =  (drect[1] * 255 ) / alpha;
			if (col > 255) col = 255;
			drect[1] = col;

			col =  (drect[2] * 255 ) / alpha;
			if (col > 255) col = 255;
			drect[2] = col;

			col =  (drect[3] * 255 ) / alpha;
			if (col > 255) col = 255;
			drect[3] = col;
		}
		drect += 4;
	}
}

void rectsub(drect,srect,x)
int *drect,*srect,x;
{
	for(;x>0;x--){
		*drect++ -= *srect++;
	}
}

void rectaddchar(drect,srect,x)
uchar *drect,*srect;
int x;
{
	int col;
	
	for(;x>0;x--){
		col = drect[0] + srect[0];
		if (col > 255) drect[0] = 255;
		else drect[0] = col;

		col = drect[1] + srect[1];
		if (col > 255) drect[1] = 255;
		else drect[1] = col;
		
		col = drect[2] + srect[2];
		if (col > 255) drect[2] = 255;
		else drect[2] = col;

		col = drect[3] + srect[3];
		if (col > 255) drect[3] = 255;
		else drect[3] = col;

		drect += 4;
		srect += 4;
	}
}

void rectcpymask(drect, srect, x, mask)
uint *drect,*srect;
int x;
{
	int invmask = ~mask;
	
	for(; x > 0; x--){
		drect[0] = (drect[0] & invmask) | (srect[0] & mask);
		drect ++;
		srect ++;
	}
}


void rectsignedadd(drect,srect,x)
uchar *drect,*srect;
int x;
{
	int col;
	
	for(;x>0;x--){
		col = drect[0] + (srect[0] << 1) - 256;;
		if (col & 256) {
			if (col < 0) drect[0] = 0;
			else drect[0] = 255;
		} else drect[0] = col;

		col = drect[1] + (srect[1] << 1) - 256;
		if (col & 256) {
			if (col < 0) drect[1] = 0;
			else drect[1] = 255;
		} else drect[1] = col;
		
		col = drect[2] + (srect[2] << 1) - 256;
		if (col & 256) {
			if (col < 0) drect[2] = 0;
			else drect[2] = 255;
		} else drect[2] = col;

		col = drect[3] + (srect[3] << 1) - 256;
		if (col & 256) {
			if (col < 0) drect[3] = 0;
			else drect[3] = 255;
		} else drect[3] = col;

		drect += 4;
		srect += 4;
	}
}

void rectadd(drect,srect,x)
int *drect,*srect,x;
{
	for(;x>0;x--){
		*drect++ += *srect++;
	}
}

void rectsubchar(drect,srect,x)
uchar *drect,*srect;
int x;
{
	for(;x>0;x--){
		drect[0] -= srect[0];
		drect[1] -= srect[1];
		drect[2] -= srect[2];
		drect[3] -= srect[3];
		drect += 4;
		srect += 4;
	}
}

void rectsub2(drect, srect, x)
uchar *drect, *srect;
int x;
{
	short col;
	
	for (; x > 0; x--){
		col = *drect - *srect++;
		if (col < 0) *drect++ = ((col + 1) >> 1) + 128;
		else *drect++ = (col >> 1) + 128;
		
		col = *drect - *srect++;
		if (col < 0) *drect++ = ((col + 1) >> 1) + 128;
		else *drect++ = (col >> 1) + 128;
		
		col = *drect - *srect++;
		if (col < 0) *drect++ = ((col + 1) >> 1) + 128;
		else *drect++ = (col >> 1) + 128;
		
		col = *drect - *srect++;
		if (col < 0) *drect++ = ((col + 1) >> 1) + 128;
		else *drect++ = (col >> 1) + 128;
	}
}


void rectadd2(drect, srect, x)
uchar *drect, *srect;
int x;
{
	short col;
	
	for (; x > 0; x--){
		col = *drect + ((*srect++ - 128) << 1);
		if (col & 256) {
			if (col < 0) col = 0;
			else col = 255;
		}
		*drect++ = col;

		col = *drect + ((*srect++ - 128) << 1);
		if (col & 256) {
			if (col < 0) col = 0;
			else col = 255;
		}
		*drect++ = col;
		
		col = *drect + ((*srect++ - 128) << 1);
		if (col & 256) {
			if (col < 0) col = 0;
			else col = 255;
		}
		*drect++ = col;

		col = *drect + ((*srect++ - 128) << 1);
		if (col & 256) {
			if (col < 0) col = 0;
			else col = 255;
		}
		*drect++ = col;
	}
}


void rectorvalue(drect, srect, x, value)
uint *drect, *srect;
int x, value;
{
	for(;x > 0; x--)
		*drect++ |= value;
}

void rectandvalue(drect, srect, x, value)
uint *drect, *srect;
int x, value;
{
	for(;x > 0; x--)
		*drect++ &= value;
}

void rector(drect, srect, x)
uint *drect, *srect;
int x;
{
	for(;x > 0; x--)
		*drect++ |= *srect++;
}


void rectcpy(drect, srect, x)
uint *drect, *srect;
int x;
{
	memcpy(drect,srect, x * sizeof(int));
}


void rectcpyalpha(drect, srect, x)
uchar *drect, *srect;
int x;
{
	for(;x > 0; x--) {
		drect[0] = srect[0];
		drect += 4;
		srect += 4;
	}
}


void rectfill(drect, srect, x, value)
uint *drect, *srect;
int x, value;
{
	for (;x > 0; x--) *drect++ = value;
}


void rectblend(drect, srect, x, value)
uchar *drect, *srect;
int x, value;
{
	short v1, v2;

	v1 = *((float *) &value) * 256;
	v2 = 256 - v1;

	if (srect != drect){
		for (;x > 0; x--){
			drect[0] = ((v2 * drect[0]) + (v1 * srect[0])) >> 8;
			drect[1] = ((v2 * drect[1]) + (v1 * srect[1])) >> 8;
			drect[2] = ((v2 * drect[2]) + (v1 * srect[2])) >> 8;
			drect[3] = ((v2 * drect[3]) + (v1 * srect[3])) >> 8;
			drect += 4; 
			srect += 4;
		}
	} else{
		for (;x > 0; x--){
			drect[0] = (v2 * drect[0]) >> 8;
			drect[1] = (v2 * drect[1]) >> 8;
			drect[2] = (v2 * drect[2]) >> 8;
			drect[3] = (v2 * drect[3]) >> 8;
			drect += 4;
		}
	}
}


void rectalphaover_oud(drect, srect, x)
uchar * drect, *srect;
int x;
{
	uchar alpha;

	for ( ; x > 0; x--){
		alpha = srect[0];
		if (alpha){
			if (alpha == 255){
				*((uint *) drect) = *((uint *) srect);
			} else{
				drect[0] += (alpha * (256 - drect[0])) >> 8;
				drect[1] += (alpha * (srect[1] - drect[1])) >> 8;
				drect[2] += (alpha * (srect[2] - drect[2])) >> 8;
				drect[3] += (alpha * (srect[3] - drect[3])) >> 8;
			}
		}
		drect += 4;
		srect += 4;
	}
}


void rectalphaover(drect, srect, x)
uchar * drect, *srect;
int x;
{
	ushort alpha;
	ushort col;

	for ( ; x > 0; x--){
		alpha = srect[0];
		if (alpha){
			if (alpha == 255){
				*((uint *) drect) = *((uint *) srect);
			} else{
				alpha++;
				drect[0] += (alpha * (255 - drect[0])) >> 8; 
				drect[1] += (alpha * (srect[1] - drect[1])) >> 8;
				drect[2] += (alpha * (srect[2] - drect[2])) >> 8;
				drect[3] += (alpha * (srect[3] - drect[3])) >> 8;
			}
		}
		drect += 4;
		srect += 4;
	}
}


void rectalphaunder(drect, srect, x)
uchar * drect, *srect;
int x;
{
	ushort alpha;
	ushort col;

	for ( ; x > 0; x--){
		alpha = drect[0];
		if (alpha != 255) {
			if (alpha == 0){
				*((uint *) drect) = *((uint *) srect);
			} else{
				alpha = 256 - alpha;
				drect[0] += (alpha * (255 - drect[0])) >> 8; 
				drect[1] += (alpha * (srect[1] - drect[1])) >> 8;
				drect[2] += (alpha * (srect[2] - drect[2])) >> 8;
				drect[3] += (alpha * (srect[3] - drect[3])) >> 8;
			}
		}
		drect += 4;
		srect += 4;
	}
}


void rectpremulalphaover(drect, srect, x)
uchar * drect, *srect;
int x;
{
	uchar alpha;
	ushort col;
	/* premulalpha ! */

	for ( ; x > 0; x--){
		alpha = srect[0];
		if (alpha){
			if (alpha == 255){
				*((uint *) drect) = *((uint *) srect);
			} else{
				alpha = 255 - alpha;

				col = ((alpha * drect[0]) / 255) + srect[0];
				if (col > 255) drect[0] = 255;
				else drect[0] = col;

				col = ((alpha * drect[1]) / 255) + srect[1];
				if (col > 255) drect[1] = 255;
				else drect[1] = col;

				col = ((alpha * drect[2]) / 255) + srect[2];
				if (col > 255) drect[2] = 255;
				else drect[2] = col;

				col = ((alpha * drect[3]) / 255) + srect[3];
				if (col > 255) drect[3] = 255;
				else drect[3] = col;
			}
		}
		drect += 4;
		srect += 4;
	}
}


void rectpremulalphaunder(drect, srect, x)
uchar * drect, *srect;
int x;
{
	uchar alpha;
	ushort col;
	/* premulalpha ! */

	for ( ; x > 0; x--){
		alpha = 255 - drect[0];
		
		if (alpha){
			if (alpha == 255){
				*((uint *) drect) = *((uint *) srect);
			} else{
				col = ((alpha * srect[0]) / 255) + drect[0];
				if (col > 255) drect[0] = 255;
				else drect[0] = col;

				col = ((alpha * srect[1]) / 255) + drect[1];
				if (col > 255) drect[1] = 255;
				else drect[1] = col;

				col = ((alpha * srect[2]) / 255) + drect[2];
				if (col > 255) drect[2] = 255;
				else drect[2] = col;

				col = ((alpha * srect[3]) / 255) + drect[3];
				if (col > 255) drect[3] = 255;
				else drect[3] = col;
			}
		}
		drect += 4;
		srect += 4;
	}
}


void rectop(dbuf,sbuf,destx,desty,srcx,srcy,width,height,operation, value)
struct ImBuf *dbuf,*sbuf;
int destx,desty,srcx,srcy,width,height, value;
void (*operation)();
{
	uint *drect,*srect;

	if (dbuf == 0) return;
	if (operation == 0) return;

	if (destx < 0){
		srcx -= destx ;
		width += destx ;
		destx = 0;
	}
	if (srcx < 0){
		destx -= srcx ;
		width += destx ;
		srcx = 0;
	}
	if (desty < 0){
		srcy -= desty ;
		height += desty ;
		desty = 0;
	}
	if (srcy < 0){
		desty -= srcy ;
		height += desty ;
		srcy = 0;
	}

	if (width > dbuf->x - destx) width = dbuf->x - destx;
	if (height > dbuf->y - desty) height = dbuf->y - desty;
	if (sbuf){
		if (width > sbuf->x - srcx) width = sbuf->x - srcx;
		if (height > sbuf->y - srcy) height = sbuf->y - srcy;
	}

	if (width <= 0) return;
	if (height <= 0) return;

	drect = dbuf->rect;
	if (sbuf) srect = sbuf->rect;

	drect += desty * dbuf->x;
	drect += destx;
	destx = dbuf->x;

	if (sbuf) {
		srect += srcy * sbuf->x;
		srect += srcx;
		srcx = sbuf->x;
	} else{
		srect = drect;
		srcx = destx;
	}

	for (;height > 0; height--){
		operation(drect,srect,width, value);
		drect += destx;
		srect += srcx;
	}
}


void rectoptot(dbuf,sbuf,operation, value)
struct ImBuf *dbuf,*sbuf;
void (*operation)();
int value;
{
	rectop(dbuf,sbuf,0,0,0,0,32767,32767,operation, value);
}


void rectopeven(dbuf,sbuf,operation, value)
struct ImBuf *dbuf,*sbuf;
void (*operation)();
int value;
{
	int dx, dy, sx, sy;
	int max;

	if (dbuf) {
		dx = dbuf->x; 
		dy = dbuf->y;
		max = dx;
		dbuf->x <<= 1;
		dbuf->y = (dbuf->y + 1) >> 1;
	}
	if (sbuf) {
		sx = sbuf->x; 
		sy = sbuf->y;
		if (sx < max) max = sx;
		sbuf->x <<= 1;
		sbuf->y = (sbuf->y + 1) >> 1;
	}

	rectop(dbuf,sbuf,0,0,0,0,max,32767,operation, value);

	if (dbuf) { 
		dbuf->x = dx; 
		dbuf->y = dy;
	}
	if (sbuf) { 
		sbuf->x = sx; 
		sbuf->y = sy;
	}
}


void rectopodd(dbuf,sbuf,operation, value)
struct ImBuf *dbuf,*sbuf;
void (*operation)();
int value;
{
	int dx, dy, sx, sy;

	if (dbuf) {
		dx = dbuf->x; 
		dy = dbuf->y;
		dbuf->x <<= 1;
		dbuf->y >>= 1;
	}
	if (sbuf) {
		sx = sbuf->x; 
		sy = sbuf->y;
		sbuf->x <<= 1;
		sbuf->y >>= 1;
	} else {
		sx = dx;
		sy = dy;
	}

	rectop(dbuf,sbuf,dx,0,sx,0,32767,32767,operation, value);

	if (dbuf) { 
		dbuf->x = dx; 
		dbuf->y = dy;
	}
	if (sbuf) { 
		sbuf->x = sx; 
		sbuf->y = sy;
	}
}


