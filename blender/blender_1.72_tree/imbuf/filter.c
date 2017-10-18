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


#define OBJECTBLOK "filter"

/************************************************************************/
/*				FILTERS					*/
/************************************************************************/


void sharprow(point,x)
uchar *point;
int x;
{
	uint c1,c2,c3,error;

	if (x>1){
		c1 = c2 = *point;
		error = 2;
		for(x--;x>0;x--){
			c3 = point[4];
			/*			c1 = 8*c2 - 2*c1 - 2*c3 + error;   /*dit is de dubbele sharpening*/
			c1 = 6*c2 - c1 - c3 + error;
			if (c1 & (256 *4)){
				if (c1 & (512 *4)) c1=0;
				else c1= 255 * 4;
			}
			error = c1 & 3;
			*point = c1 >> 2;
			point += 4;
			c1=c2 ;
			c2=c3;
		}
		c1 = 6*c2 - c1 - c3 + error;
		if (c1 & (256 *4)){
			if (c1 & (512 *4)) c1=0;
			else c1= 255 * 4;
		}
		*point = c1 >> 2;
	}
}



void sharpenx(ibuf)
struct ImBuf *ibuf;
{
	uchar *point;
	short x,y,skip;

	point = (uchar *)ibuf->rect;
	x = ibuf->x;
	y = ibuf->y;
	skip = (x<<2) - 4;

	for (;y>0;y--){
		point++;
		sharprow(point,x);
		point++;
		sharprow(point,x);
		point++;
		sharprow(point,x);
		point++;
		sharprow(point,x);
		point+=skip;
	}
}


void filtrow(point,x)
uchar *point;
int x;
{
	uint c1,c2,c3,error;

	if (x>1){
		c1 = c2 = *point;
		error = 2;
		for(x--;x>0;x--){
			c3 = point[4];
			c1 += (c2<<1) + c3 + error;
			error = c1 & 3;
			*point = c1 >> 2;
			point += 4;
			c1=c2;
			c2=c3;
		}
		*point = (c1 + (c2<<1) + c2 + error) >> 2;
	}
}


void filtcolum(point,y,skip)
uchar *point;
int y,skip;
{
	uint c1,c2,c3,error;
	uchar *point2;

	if (y>1){
		c1 = c2 = *point;
		point2 = point;
		error = 2;
		for(y--;y>0;y--){
			point2 += skip;
			c3 = *point2;
			c1 += (c2<<1) + c3 +error;
			error = c1 & 3;
			*point = c1 >> 2;
			point=point2;
			c1=c2;
			c2=c3;
		}
		*point = (c1 + (c2<<1) + c2 + error) >> 2;
	}
}


void filtery(ibuf)
struct ImBuf *ibuf;
{
	uchar *point;
	short x,y,skip;

	point = (uchar *)ibuf->rect;
	x = ibuf->x;
	y = ibuf->y;
	skip = x<<2;

	for (;x>0;x--){
		if (ibuf->depth > 24) filtcolum(point,y,skip);
		point++;
		filtcolum(point,y,skip);
		point++;
		filtcolum(point,y,skip);
		point++;
		filtcolum(point,y,skip);
		point++;
	}
}


void filterx(ibuf)
struct ImBuf *ibuf;
{
	uchar *point;
	short x,y,skip;

	point = (uchar *)ibuf->rect;
	x = ibuf->x;
	y = ibuf->y;
	skip = (x<<2) - 3;

	for (;y>0;y--){
		if (ibuf->depth > 24) filtrow(point,x);
		point++;
		filtrow(point,x);
		point++;
		filtrow(point,x);
		point++;
		filtrow(point,x);
		point+=skip;
	}
}


void filter(ibuf)
struct ImBuf *ibuf;
{
	filtery(ibuf);
	filterx(ibuf);
}


void filtrowlim(point,x,lim)
uchar *point;
int x;
int lim;
{
	uint c1,c2,c3,error;

	if (x>1){
		c1 = c2 = *point;
		error = 2;
		for(x--;x>0;x--){
			c3 = point[4];
			c1 += (c2<<1) + c3 + error;
			error = c1 & 3;
			c1 >>= 2;
			if (abs((int)(c1 - c2)) > lim) error = 0;
			else *point = c1;
			point += 4;
			c1=c2;
			c2=c3;
		}
		c1 += (c2<<1) + c3 + error;
		c1 >>= 2;
		if (abs((int)(c1 - c2)) > lim);
		else *point = c1;
	}
}


void filtcolumlim(point,y,skip,lim)
uchar *point;
int y,skip;
int lim;
{
	uint c1,c2,c3,error;
	uchar *point2;

	if (y>1){
		c1 = c2 = *point;
		point2 = point;
		error = 2;
		for(y--;y>0;y--){
			point2 += skip;
			c3 = *point2;
			c1 += (c2<<1) + c3 +error;
			error = c1 & 3;
			c1 >>= 2;
			if (abs((int)(c1 - c2)) > lim) error = 0;
			else *point = c1;
			point=point2;
			c1=c2;
			c2=c3;
		}
		c1 += (c2<<1) + c3 + error;
		c1 >>= 2;
		if (abs((int)(c1 - c2)) > lim);
		else *point = c1;
	}
}


void filterylim(ibuf,lim)
struct ImBuf *ibuf;
int lim;
{
	uchar *point;
	short x,y,skip;

	point = (uchar *)ibuf->rect;
	x = ibuf->x;
	y = ibuf->y;
	skip = x<<2;

	for (;x>0;x--){
		point++;
		filtcolumlim(point,y,skip,lim);
		point++;
		filtcolumlim(point,y,skip,lim);
		point++;
		filtcolumlim(point,y,skip,lim);
		point++;
	}
}


void filterxlim(ibuf,lim)
struct ImBuf *ibuf;
int lim;
{
	uchar *point;
	short x,y,skip;

	point = (uchar *)ibuf->rect;
	x = ibuf->x;
	y = ibuf->y;
	skip = (x<<2) - 3;

	for (;y>0;y--){
		point++;
		filtrowlim(point,x,lim);
		point++;
		filtrowlim(point,x,lim);
		point++;
		filtrowlim(point,x,lim);
		point+=skip;
	}
}


void filterlim_oud(ibuf,lim)
struct ImBuf *ibuf;
int lim;
{
	filterylim(ibuf,lim);
	filterxlim(ibuf,lim);
}


void filterlim(ibuf1,lim)
struct ImBuf *ibuf1;
int lim;
{
	short tab[512];
	struct ImBuf * ibuf2;
	uchar * rect1, * rect2;
	int i, j;
	extern rectcpy();

	if (ibuf1 == 0) return;
	if (ibuf1->rect == 0) return;
	if (lim <= 0) return;

	ibuf2 = dupImBuf(ibuf1);


	/* tabel genereren */

	for (i = 511 ; i >= 0 ; i--) tab[i] = 0;
	for (i = 1; i <= (lim + 1) >> 1; i ++){
		tab[256 + i] = i;
		tab[256 - i] = -i;
	}
	for (j = lim >> 1; j > 0; j--){
		tab[256 + i] = j;
		tab[256 - i] = -j;
		i++;
	}

	filterx(ibuf2);

	rect1 = (uchar *) ibuf1->rect;
	rect2 = (uchar *) ibuf2->rect;
	for (i = ibuf1->x * ibuf1->y; i > 0; i--){
		rect1[0] += tab[256 + rect2[0] - rect1[0]];
		rect1[1] += tab[256 + rect2[1] - rect1[1]];
		rect1[2] += tab[256 + rect2[2] - rect1[2]];
		rect1[3] += tab[256 + rect2[3] - rect1[3]];
		rect1 += 4;
		rect2 += 4;
	}

	rectoptot(ibuf2, ibuf1, rectcpy);

	/* fields openklappen en in de y filteren */

	ibuf2->x <<= 1; 
	ibuf2->y >>= 1;
	filtery(ibuf2);

	rect1 = (uchar *) ibuf1->rect;
	rect2 = (uchar *) ibuf2->rect;
	for (i = ibuf1->x * ibuf1->y; i > 0; i--){
		rect1[0] += tab[256 + rect2[0] - rect1[0]];
		rect1[1] += tab[256 + rect2[1] - rect1[1]];
		rect1[2] += tab[256 + rect2[2] - rect1[2]];
		rect1[3] += tab[256 + rect2[3] - rect1[3]];
		rect1 += 4;
		rect2 += 4;
	}

	freeImBuf(ibuf2);
}


void swapfilt(ibuf)
struct ImBuf *ibuf;
{
	uint *point1,*point2,col;
	short x,y,skip;

	y = ibuf->y;
	skip = ibuf->x;
	point1=ibuf->rect;

	for(;y>0;y--){
		x = skip-1;
		for(;x>0;x-=2){
			col=point1[x];
			point1[x]=point1[x-1];
			point1[x-1]=col;
		}
		point1 += skip;
	}

	y = ibuf->y-1;
	skip = ibuf->x;
	point1 = ibuf->rect;
	point2 = point1 + skip;

	for(;y>0;y-=2){
		x = skip;
		for(;x>0;x-=2){
			col = *point1;
			(*point1++) = *point2;
			(*point2++) = col;
		}
		point1 += skip;
		point2 += skip;
	}
	filter(ibuf);
}

