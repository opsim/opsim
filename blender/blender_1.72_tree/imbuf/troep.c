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


#define OBJECTBLOK "imbuftroep"

void quantize24(struct ImBuf *ibuf)
{
	/* Quantiseert een buffer, in stappen van ints, alpha wordt
		overgeslagen */

	uchar *buf1,*buf2;
	uint *rect;
	short size,y;

	rect=ibuf->rect;

	for(y=ibuf->y;y>0;y--){
		size=ibuf->x;
		buf1=(uchar *)rect;
		buf1+=size<<2;
		buf2=buf1-4;
		rect+=size;
		
		for (size--;size>0;size--){
			buf1--;
			buf2--;
			*(--buf1) -= *(--buf2);
			*(--buf1) -= *(--buf2);
			*(--buf1) -= *(--buf2);
		}
	}
}


void swapbitsrect(struct ImBuf *ibuf)
{
	uchar *buf,i;
	short x,y;

	buf=(uchar *)ibuf->rect;

	for(y=ibuf->y; y>0; y--){
		for (x=ibuf->x; x>0; x--){
			buf++;
			i = *(++buf);
			if (i & 128) *buf = i^127;
			i = *(++buf);
			if (i & 128) *buf = i^127;
			i = *(++buf);
			if (i & 128) *buf = i^127;
		}
	}
}


void dequantize24(struct ImBuf *ibuf)
{
	/* deQuantiseert een scanline, in stappen van ints, linkse byte wordt
		overgeslagen */

	uchar *buf1,*buf2;
	short size;

	size=ibuf->x;
	buf1=(uchar *) ibuf->rect;
	buf2=buf1+4;
	
	for (size--;size>0;size--){
		*(buf2++) += *(buf1++);
		*(buf2++) += *(buf1++);
		*(buf2++) += *(buf1++);
		buf1++;
		buf2++;
	}
}

short quantbpl(struct ImBuf *ibuf)
{
	struct ImBuf *ibuft;
	int y,offset,depth;

	ibuft=allocImBuf(ibuf->x,1,ibuf->depth,1,1);
	if (ibuft==0) return(FALSE);
	
	offset= 0;
	depth= ibuf->depth;

	for (y=ibuf->y;y>0;y--){
		bptolscanl(ibuft->rect,ibuf->x,ibuf->planes,depth,offset);
		quantize24(ibuft);

		if (depth != 32) {
			int j,bp,todo;
			uint *rect;

			rect = ibuft->rect;
			todo = 32-depth;
	
			for (j=ibuft->x; j>0; j--) {
				bp= *rect;
				bp<<= todo;
				*(rect++)= bp;
			}
		}

		ltobpscanl(ibuft->rect,ibuf->x,ibuf->planes,depth,offset);
		offset += ibuf->skipx;
	}

	freeImBuf(ibuft);
	return (TRUE);
}


void sculpt(struct ImBuf *ibuf, short offset, char *naam)
{
	int file,size,i;
	uchar *point1,*point2,*buf;
	
	file=open(naam,O_BINARY | O_RDWR | O_CREAT | O_TRUNC);
	if (file<=0) return;
	size= ibuf->x*ibuf->y;
	buf= malloc(size);
	if (buf==0) return;

	point1=(uchar *)ibuf->rect;
	point1+=offset;
	point2=buf;
	
	for(i=size;i>0;i--){
		point1 += 4;
		*(point2++) = *point1;
	}

	write(file,buf,size);
	free(buf);
	close(file);
}


