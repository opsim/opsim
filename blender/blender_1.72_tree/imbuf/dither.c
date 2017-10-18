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


#define OBJECTBLOK "dithers"

/************************************************************************/
/*				DITHERS					*/
/************************************************************************/

void (*ditherfunc)(struct ImBuf *, short, short) = dit0;
long dithertype = '0';
short jitdit;

void floyd(struct ImBuf * ibuf, short ofs, short bits)
{
	short *buf,*errb,x,y,pix,err,and,nand;
	uchar *rect;

	buf = callocstruct(short,ibuf->x+2);
	if (buf == 0) return;

	rect= (uchar *)ibuf->rect;
	rect +=ofs;
	bits = 8 - bits;
	and = (1<<bits)-1;
	nand = ~and;

	for(y=ibuf->y;y>0;y--){
		errb = buf+1;
		for(x=ibuf->x;x>0;x--){
			err = errb[-1] << 3;	/*  *8 */
			err += errb[0];
			err += errb[0] << 2;	/*  *5 */
			err += errb[1];
			err += errb[1] << 1;	/*  *3 */
			err >>= 4;
			pix = *rect+err;
			if (pix>255) pix=255;
			*(errb++) = pix & and;
			*rect = pix & nand;
			rect += 4;
		}
	}
	free(buf);
}


void dit2_new(struct ImBuf * ibuf, short * deltab, short bits)
{
	int x, y, errr, errg, errb, val, add, r, g, b, nr, ng, nb, i, j;
	int col[4];
	uchar * rect, map[255], * cmap, * temp;
	
	/* mapping vullen */

	add = (1 << bits) - 1;
	val = add;
	for (x = 0; x < 256; x++) {
		map[x] = val >> 8;
		val += add;
	}
	
	rect = (uchar *) ibuf->rect;

	/* grijswaardes voor iedere kleur berekenen */
	
	cmap = malloc(4 * ibuf->maxcol);
	memcpy(cmap, ibuf->cmap, 4 * ibuf->maxcol);

	temp = cmap;
	
	for (i = 0; i < ibuf->maxcol; i++){
		temp[0] = (.114 * temp[1]) + (.587 * temp[2]) + (.299 * temp[3]);
		temp += 4;
	}

	for(y = ibuf->y ; y > 0 ; y--){
		
		if (y & 1) add = 2;
		else add = 0;
		
		for(x = ibuf->x ; x > 0 ; x--){
			r = rect[0];
			g = rect[1];
			b = rect[2];
			errr = errg = errb = 0;
			for (i = 0; i < 4; i++) {
				nr = r + errr;
				ng = g + errg;
				nb = b + errb;
				/* clippen */
				if (nr & 0xff00) {
					if (nr < 0) nr = 0;
					else nr = 255;
				}
				if (ng & 0xff00) {
					if (ng < 0) ng = 0;
					else ng = 255;
				}
				if (nb & 0xff00) {
					if (nb < 0) nb = 0;
					else nb = 255;
				}
				nr = map[nr];
				ng = map[ng];
				nb = map[nb];
				val = (((nb << bits) + ng) << bits) + nr;
				col[i] = val = deltab[val << 1];
				val <<= 2;
				
				errr += r - cmap[val + 0];
				errg += g - cmap[val + 1];
				errb += b - cmap[val + 2];
			}
			
			/* kleuren op grijswaarde sorteren */
			for (i = 0; i < 4; i++) col[i] |= cmap[col[i] << 2] << 20;
			
			for (i = 0; i < 3; i++) { 
				for (j = i + 1; j < 4; j++) {
					if (col[j] < col[i]) {val = col[i]; col[i] = col[j]; col[j] = val;}
				}
			}
			
			if (col[1] != col[2]) {	/* geen diagonale dithering */
				if ((col[0] == col[1])  || (col[2] == col[3])) {val = col[0]; col[0] = col[2]; col[2] = val;}
			}
			
			/* trillen verminderen */
			val = col[0]; col[0] = col[3]; col[3] = val;
			
			((uint *) rect)[0] = col[add + (x & 1)];
			rect += 4;
		}
	}
}



void dit2(struct ImBuf * ibuf, short ofs, short bits)
{
	short x,y,pix,and,add1,add2;
	uchar *rect;
	uchar dit[4];

	rect= (uchar *)ibuf->rect;
	rect +=ofs;

	bits = 8 - bits;
	and = ~((1<<bits)-1);
	bits -= 2;

	if (jitdit == FALSE) ofs = 0;
	
	switch(ofs){
	case 3:
		break;
	case 2:
		dit[0]=0;
		dit[1]=1;
		dit[2]=2;
		dit[3]=3;
		break;
	case 1:
		dit[0]=3;
		dit[1]=1;
		dit[2]=0;
		dit[3]=2;
		break;
	case 0:
		dit[0]=0;
		dit[1]=2;
		dit[2]=3;
		dit[3]=1;
		break;
	}
	
	if (bits < 0){
		dit[0] >>= -bits;
		dit[1] >>= -bits;
		dit[2] >>= -bits;
		dit[3] >>= -bits;
	} else{
		dit[0] <<= bits;
		dit[1] <<= bits;
		dit[2] <<= bits;
		dit[3] <<= bits;
	}

	for(y=ibuf->y;y>0;y--){
		if(y & 1){
			add1=dit[0];
			add2=dit[1];
		}
		else{
			add1=dit[2];
			add2=dit[3];
		}
		for(x=ibuf->x;x>0;x--){
			pix = *rect;
			if (x & 1) pix += add1;
			else pix += add2;

			if (pix>255) pix=255;
			*rect = pix & and;
			rect += 4;
		}
	}
}


void dit3(struct ImBuf * ibuf, short ofs, short bits)
{
	short x,y,pix,and,ofs2;
	uchar *rect;
	uchar dit1[6],dit2[6],*dit;


	rect= (uchar *)ibuf->rect;
	rect +=ofs;

	bits = 8-bits;
	and = ~((1<<bits)-1);
	bits -= 2;

	dit1[0]=0<<bits;
	dit1[1]=0<<bits;
	dit1[2]=0<<bits;
	dit1[3]=3<<bits;
	dit1[4]=3<<bits;
	dit1[5]=3<<bits;

	dit2[0]=2<<bits;
	dit2[1]=2<<bits;
	dit2[2]=2<<bits;
	dit2[3]=1<<bits;
	dit2[4]=1<<bits;
	dit2[5]=1<<bits;

	for(y=ibuf->y;y>0;y--){
		if ((y^ofs) & 1){
			dit = dit1;
		}
		else{
			dit = dit2;
		}
		ofs2 = ofs <<1;
		for(x=ibuf->x;x>0;x--){
			ofs2++;
			if (ofs2>5) ofs2=0;

			pix = *rect + dit[ofs2];
			if (pix>255) pix=255;
			*rect = pix & and;
			rect += 4;
		}
	}
}


void dit4(struct ImBuf * ibuf, short ofs, short bits)
{
	short x,y,pix,and;
	uchar *rect;
	uchar dit[16],*add;
	static uchar dit4b[] =	{
		0,4,1,5,8,12,9,13,2,6,3,7,10,14,11,15    	};
	/*	static uchar dit4g[] =	{15,7,13,5,3,11,1,9,12,4,14,6,0,8,2,10}; */
	static uchar dit4g[] =	{
		13,5,3,11,1,9,12,4,14,6,0,8,2,10,15,7    	};
	static uchar dit4r[] = 	{
		0,8,2,10,12,4,14,6,3,11,1,9,15,7,13,5    	};

	rect= (uchar *)ibuf->rect;
	rect +=ofs;

	bits = 8 - bits;
	and = ~((1<<bits)-1);
	bits -= 4;

	if (jitdit == FALSE) ofs = 3;
	
	switch(ofs){
	case 3:
		memcpy(dit,dit4g,sizeof(dit));
		break;
	case 2:
		memcpy(dit,dit4b,sizeof(dit));
		break;
	case 1:
		memcpy(dit,dit4g,sizeof(dit));
		break;
	case 0:
		memcpy(dit,dit4r,sizeof(dit));
		break;
	}

	if (bits < 0){
		for (y = 15 ; y>=0 ; y--) dit[y] >>= -bits;
	} else{
		for (y = 15 ; y>=0 ; y--) dit[y] <<= bits;
	}

	for(y=ibuf->y;y>0;y--){
		add = &dit[4 * (y & 3)];
		for(x=ibuf->x;x>0;x--){
			pix = *rect;
			pix += add[x & 3];

			if (pix>255) pix=255;

			*rect = pix & and;
			rect += 4;
		}
	}
}


void dit0(struct ImBuf * ibuf, short ofs, short bits)
{
	int x, y, and, add, pix;
	uchar *rect;

	rect= (uchar *)ibuf->rect;
	rect +=ofs;

	bits = 8 - bits;
	and = ~((1 << bits)-1);
	add = 1 << (bits - 1);
	
	for (y = ibuf->y; y > 0; y--){
		for (x = ibuf->x; x > 0; x--) {
			pix = *rect + add;
			if (pix > 255) pix = 255; 
			*rect = pix & and;
			rect += 4;
		}
	}
}

void setjitdit(short onoff)
{
	jitdit = onoff;
}

void setdither(void (*func)(struct ImBuf *, short, short))
{
	dithertype = (long) func;
	
	switch((long) func){
	case 0:
	case '0':
		ditherfunc = dit0;
		break;
	case 2:
	case '2':
		ditherfunc = dit2;
		break;
	case '2+':
		ditherfunc = dit2; /* wordt officieel niet gebruikt */
		break;
	case 3:
	case '3':
		ditherfunc = dit3;
		break;
	case 4:
	case '4':
		ditherfunc = dit4;
		break;
	case 'f':
	case 'F':
		ditherfunc = floyd;
		break;
	default:
		ditherfunc = func;
		break;
	}
}

long getdither()
{
	return (  dithertype);
}

