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


#define OBJECTBLOK "imbufdivers"


void checkncols(ibuf)
struct ImBuf *ibuf;
{
	uint i;

	if (ibuf==0) return;
	
	if (IS_cdi(ibuf)) return;

	if (IS_amiga(ibuf)){
		if (IS_ham(ibuf)){
			if (ibuf->depth == 0) ibuf->depth = 6;
			ibuf->mincol = 0;
			ibuf->maxcol = 1 << (ibuf->depth - 2);
			/*printf("%d %d\n", ibuf->maxcol, ibuf->depth);*/
			return;
		} else if (IS_hbrite(ibuf)){
			ibuf->mincol = 0;
			ibuf->maxcol = 64;
			ibuf->depth = 6;
			return;
		}
	}

	if (ibuf->maxcol == 0){
		if (ibuf->depth <= 8){
			ibuf->mincol = 0;
			ibuf->maxcol = (1 << ibuf->depth);
			return;
		} else if (ibuf->depth == 0){
			ibuf->depth = 5;
			ibuf->mincol = 0;
			ibuf->maxcol = 32;
		}
		return;
	} else {
		/* ibuf->maxcol is bepalend voor de diepte */
		for (i=1 ; ibuf->maxcol > (1 << i); i++);
		ibuf->depth = i;
		return;
	}
}


void diff_limit_(ibuf1, ibuf2, lim)
struct ImBuf *ibuf1, *ibuf2;
int lim;
{
	short tab[512];
	uchar * rect1, * rect2;
	int i, j;
	
	if (ibuf1 == 0) return;
	if (ibuf1->rect == 0) return;
	if (ibuf2 == 0) return;
	if (ibuf2->rect == 0) return;
	if (lim <= 0) return;
	
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
}


void de_interlace(struct ImBuf *ibuf)
{
	struct ImBuf * tbuf1, * tbuf2;
	extern rectcpy();
	
	if (ibuf == 0) return;
	if (ibuf->flags & IB_fields) return;
	ibuf->flags |= IB_fields;
	
	if (ibuf->rect) {
		/* kopieen aanmaken */
		tbuf1 = allocImBuf(ibuf->x, ibuf->y / 2, 32, IB_rect, 0);
		tbuf2 = allocImBuf(ibuf->x, ibuf->y / 2, 32, IB_rect, 0);
		
		ibuf->x *= 2;	
		rectop(tbuf1, ibuf, 0, 0, 0, 0, 32767, 32767, rectcpy);
		rectop(tbuf2, ibuf, 0, 0, tbuf2->x, 0, 32767, 32767, rectcpy);
	
		ibuf->x /= 2;
		rectop(ibuf, tbuf1, 0, 0, 0, 0, 32767, 32767, rectcpy);
		rectop(ibuf, tbuf2, 0, tbuf2->y, 0, 0, 32767, 32767, rectcpy);
		
		freeImBuf(tbuf1);
		freeImBuf(tbuf2);
	}
	ibuf->y /= 2;
}


void interlace(struct ImBuf *ibuf)
{
	struct ImBuf * tbuf1, * tbuf2;
	extern rectcpy();
	
	if (ibuf == 0) return;
	ibuf->flags &= ~IB_fields;
	
	ibuf->y *= 2;
	
	if (ibuf->rect) {
		/* kopieen aanmaken */
		tbuf1 = allocImBuf(ibuf->x, ibuf->y / 2, 32, IB_rect, 0);
		tbuf2 = allocImBuf(ibuf->x, ibuf->y / 2, 32, IB_rect, 0);
		
		rectop(tbuf1, ibuf, 0, 0, 0, 0, 32767, 32767, rectcpy);
		rectop(tbuf2, ibuf, 0, 0, 0, tbuf2->y, 32767, 32767, rectcpy);

		ibuf->x *= 2;	
		rectop(ibuf, tbuf1, 0, 0, 0, 0, 32767, 32767, rectcpy);
		rectop(ibuf, tbuf2, tbuf2->x, 0, 0, 0, 32767, 32767, rectcpy);
		ibuf->x /= 2;
		
		freeImBuf(tbuf1);
		freeImBuf(tbuf2);
	}
}

void dumpcmap(struct ImBuf * ibuf, char * name)
{
	FILE * fd = 0;
	int i;
	
	if (ibuf == 0) return;
	if (ibuf->cmap == 0) return;
	
	if (name) fd = fopen(name, "wb");
	
	if (fd == 0) fd = stdout;
	for (i = 0; i < ibuf->maxcol; i ++){
		fprintf(fd, "%4d %8x\n", i, ibuf->cmap[i]);
	}
	
	if (fd != stdout) fclose(fd);
}


