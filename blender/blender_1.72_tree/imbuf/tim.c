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


#define OBJECTBLOK "tim"

#define GLS(x) (((uchar *)(x))[3] << 24 | ((uchar *)(x))[2] << 16 | ((uchar *)(x))[1] << 8 | ((uchar *)(x))[0])

void apply_tim_cmap(uint * data, uint count) {
	int col, a, b, g, r, cur;
	
	for (cur = 0; cur < count; cur++) {
		col = *data;
		
		a = col >> 15;
		b = (col >> 10) & 0x1f;
		g = (col >> 5) & 0x1f;
		r = col & 0x1f;
		
		/*if (count < 257) printf("%4d: %d.%2d.%2d.%2d\n", cur, a, b, g, r);*/

		if (col == 0) {
			a = 0;
		} else if (a == 0) {
			a = 255;
		} else {
			a = 128;
		}

		b = (b << 3) | (b >> 2);
		g = (g << 3) | (g >> 2);
		r = (r << 3) | (r >> 2);
			
		*data++ = (a << 24) | (b << 16) | (g << 8) | r;
		/*if (count < 257) printf("0x%08x\n", data[-1]);*/
	}
}

void to_tim_cmap(uint * data, uint count) {
	int col, a, b, g, r, cur;
	
	for (cur = 0; cur < count; cur++) {
		col = *data;
		
		a = col >> 24;
		b = (col >> 19) & 0x1f;
		g = (col >> 11) & 0x1f;
		r = (col >> 3) & 0x1f;
		
		/*if (count < 257) printf("%4d: %d.%2d.%2d.%2d ", cur, a, b, g, r);*/
		
		/* zwart = 100 % transparant
		 * dus niet 100% transparant is NIET zwart
		 */
		
		if (a == 0) b = g = r = 0;
		else a = 255;
		
		/* alpha afhandelen, een mogelijkheid */
		
/*		if (a < 64) {
			a = b = g = r = 0;
		} else if (a < 192) {
			a = 1;
		} else {
			a = 0;
		}
*/
		*data++ = ((a & 0x80) << 8) | (b << 10) | (g << 5) | r;
	}
}

struct ImBuf *loadtim(ushort * mem, int flags)
{
	TARGA tga;
	struct ImBuf * ibuf;
	int count, size, type, clut, x, y, col, depth;
	uint * rect;
	uchar * cmem;
	
	if (mem == 0) return(0);
	
	/*printf("%x %x\n", GLS(mem),  GLS(mem + 2));*/
	type = GLS(mem + 2);
	clut = type & TIM_CLUT;
	type |= TIM;
	
	switch (type) {
		case TIM_4:
			depth = 4;
			break;
		case TIM_8:
			depth = 8;
			break;
		case TIM_16:
			depth = 16;
			break;
		case TIM_24:
			depth = 24;
			break;			
	}
	
	ibuf = allocImBuf(0, 0, depth, 0, 0);
	if (ibuf == 0) return(0);

	ibuf->ftype = type;
	
	mem += 4;

	if (clut) {
		size = GLS(mem);
		mem += 6;	/* ignore x,y and w,h for now */
		
		size -= 12;
		size /= 2;
		ibuf->mincol = 0;
		ibuf->maxcol = size;
		
		addcmapImBuf(ibuf);
		
		for (count = 0; count < size; count++) {
			ibuf->cmap[count] = GSS(mem);
			mem++;
		}
		apply_tim_cmap(ibuf->cmap, size);
	}
	
	size = GLS(mem); mem += 2; size -= 12;
	ibuf->xorig = GSS(mem); mem++;
	ibuf->yorig = GSS(mem); mem++;
	ibuf->x = GSS(mem); mem++;
	ibuf->y = GSS(mem); mem++;
	
	switch(ibuf->ftype) {
		case TIM_4:
			ibuf->x *= 4;
			break;
		case TIM_8:
			ibuf->x *= 2;
			break;
		case TIM_24:
			ibuf->x = (ibuf->x * 2) / 3;
			break;
	}
	
	/*printf("%d %d at %d %d\n", ibuf->x, ibuf->y, ibuf->xorig, ibuf->yorig);*/
	if (flags & IB_test) return(ibuf);
	
	addrectImBuf(ibuf);
	
	rect = ibuf->rect;
	size /= 2;
	
	switch (ibuf->ftype) {
		case TIM_4:
			for (count = 0; count < size; count++) {
				col  = GSS(mem); mem++;
				*rect++ = col & 0xf;
				*rect++ = (col >> 4) & 0xf;
				*rect++ = (col >> 8) & 0xf;
				*rect++ = (col >> 12) & 0xf;
			}
			if ((flags & IB_cmap) == 0) applycmap(ibuf);
			break;
		case TIM_8:
			for (count = 0; count < size; count++) {
				col  = GSS(mem); mem++;
				*rect++ = col & 0xff;
				*rect++ = (col >> 8) & 0xff;				
			}
			if ((flags & IB_cmap) == 0) applycmap(ibuf);
			break;
		case TIM_16:
			for (count = 0; count < size; count++) {
				*rect++ = GSS(mem); mem++;
			}
			if ((flags & IB_cmap) == 0) apply_tim_cmap(ibuf->rect, size);
			break;
		case TIM_24:
			size *= 2;
			size /= 3;
			cmem = (uchar *) mem;
			for (count = 0; count < size; count++) {
				*rect++ = cmem[0] + (cmem[1] <<  8) + (cmem[2] << 16);
				cmem += 3;
			}
			break;
	}
	if ((flags & IB_ttob) == 0) flipy(ibuf);

	if (ibuf) {
		if (ibuf->rect) 
			convert_rgba_to_abgr((ibuf->x+ibuf->skipx)*ibuf->y, ibuf->rect);
	}
	
	return(ibuf);
}

int write_tim_short_data(int file, uint * data, uint count)
{
	int i, ok = TRUE;
	short * _mem, * mem;

	to_tim_cmap(data, count);
	mem = _mem = malloc(count * sizeof(short));
	
	for (i = 0; i < count; i++) {
		*mem++ = GSS(((char *) data) + 2);
		data++;
	}
	
	if (write(file, _mem, count * sizeof(short)) != count * sizeof(short)) ok = FALSE;

	free(_mem);
	return (ok);
}

short savetim(struct ImBuf * ibuf, int file, int flags)
{
	int buf[32], size, ok = TRUE;
	short x, y, *_mem, *mem, col;
	char * cmem;
	uint * rect, lcol;
	
	/*
		verschillende manieren om het alpha bitje van een TIM te gebruiken:
		aan -> geen color 0
		optellen / aftrekken -> overal alpha bit
		aan / uit -> color 0
		antialias -> color 0, alphabit transparant
		
		Plaatjes met meerdere colormaps worden nog even niet ondersteund
	*/
	
	if (ibuf->ftype == TIM) {
		/* automatiese type bepaling */
		if (ibuf->cmap) {
			if (ibuf->depth == 4) ibuf->ftype = TIM_4;
			else if (ibuf->depth == 8) ibuf->ftype = TIM_8;
			else if (ibuf->maxcol <= 16) ibuf->ftype = TIM_4;
			else if (ibuf->maxcol <= 256) ibuf->ftype = TIM_8;
			else {
				printf("To many colors (%d) for colormapped tim.\nIgnoring cmap.\n");
				ibuf->ftype = TIM_16;
				freecmapImBuf(ibuf);
			}
		} else {
			/* for now */
			ibuf->ftype = TIM_16;
		}
	}
	
	buf[0] = 0x10000000;
	buf[1] = ibuf->ftype << 24;
	
	if (write(file, buf, 8) != 8) return(0);
	
	if (ibuf->ftype & TIM_CLUT) {
		if ((flags & IB_cmap) == 0) converttocmap(ibuf);
		
		size = (2 * ibuf->maxcol) + 12;
		buf[0] = GLS(&size);
		buf[1] = 0;	/* nog geen x en y */
		buf[2] = 0x01000000 | (GSS(&ibuf->maxcol));
		
		if (write(file, buf, 12) != 12) return(0);
		if (write_tim_short_data(file, ibuf->cmap, ibuf->maxcol) == FALSE) return(0);
	}
	
	if ((flags & IB_ttob) == 0) flipy(ibuf);
	
	x = ibuf->x;
	y = ibuf->y;
	switch (ibuf->ftype) {
		case TIM_4:
			if (x & 0x3) printf("TIM_4 width not multiple of 4: cropping\n");
			x /= 4;
			break;
		case TIM_8:
			if (x & 0x1) printf("TIM_8 width not multiple of 2: cropping\n");
			x /= 2;
			break;
		case TIM_16:
			break;
		case TIM_24:
			if (x & 0x1) printf("TIM_24 width not multiple of 2: cropping\n");
			x &= ~1;
			x *= 1.5;
			break;
	}
	
	size = (x * y * 2) + 12;
	buf[0] = GLS(&size);
	buf[1] = 0;
	buf[2] = GSS(&x) << 16 | GSS(&y);
	
	if (write(file, buf, 12) != 12) return(0);
	
	size = x * sizeof(short);
	_mem = malloc(size);
	
	switch (ibuf->ftype) {
		case TIM_4:
			for (y = 0; y < ibuf->y; y++) {
				rect = ibuf->rect + (y * ibuf->x);
				mem = _mem;
				for (x = ibuf->x / 4; x > 0; x--) {
					col = (rect[0] & 0xf) | ((rect[1] & 0xf) << 4) | ((rect[2] & 0xf) << 8) | ((rect[3] & 0xf) << 12);
					*mem++ = GSS(&col);
					rect += 4;
				}
				if (write(file, _mem, size) != size) {
					ok = FALSE;
					break;
				}
			}
			break;
		case TIM_8:
			for (y = 0; y < ibuf->y; y++) {
				rect = ibuf->rect + (y * ibuf->x);
				mem = _mem;				
				for (x = ibuf->x / 2; x > 0; x--) {
					
					col = (rect[1] & 0xff) | ((rect[0] & 0xff) << 8);
					*mem++ = col;
					rect += 2;
				}
				if (write(file, _mem, size) != size) {
					ok = FALSE;
					break;
				}
			}
			break;
		case TIM_16:
			ditherfunc(ibuf,1,5);
			ditherfunc(ibuf,2,5);
			ditherfunc(ibuf,3,5);

			ok = write_tim_short_data(file, ibuf->rect, x * y);
			break;
		case TIM_24:
			size = ibuf->x * 3;
			for (y = 0; y < ibuf->y; y++) {
				rect = ibuf->rect + (y * ibuf->x);
				cmem = (char *) _mem;
				
				for (x = (ibuf->x & ~1); x > 0; x--) {
					lcol = *rect++;
					*cmem++ = lcol;
					*cmem++ = lcol >> 8;
					*cmem++ = lcol >> 16;
				}
				
				if (write(file, _mem, size) != size) {
					ok = FALSE;
					break;
				}
			}
			break;
	}
	
	free(_mem);
	
	return (ok);
}

