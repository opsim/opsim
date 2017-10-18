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


#define OBJECTBLOK "cspace"

/************************************************************************/
/*				COLORSPACE				*/
/************************************************************************/

extern short * safechromamap;
extern uchar * safelumamap;

int Cspace_err_diff = FALSE;

int cspace_err_diff(int new)
{
	int old;
	
	old = Cspace_err_diff;
	Cspace_err_diff = new;
	
	return(old);
}

int colcspace(int col,float mat[][4])
{
	float r,g,b;
	short x,y,z;

	b = (col & 0xff0000) >> 16;
	g = (col & 0xff00) >> 8;
	r = col & 0xff;

	x = b * mat[0][0] + g * mat[1][0] + r * mat[2][0] + mat[3][0] + .5;
	y = b * mat[0][1] + g * mat[1][1] + r * mat[2][1] + mat[3][1] + .5;
	z = b * mat[0][2] + g * mat[1][2] + r * mat[2][2] + mat[3][2] + .5;

	if (x & 0xFF00){
		if (x < 0) x=0;
		else x = 255;
	}
	if (y & 0xFF00){
		if (y < 0) y=0;
		else y = 255;
	}
	if (z & 0xFF00){
		if (z < 0) z=0;
		else z = 255;
	}

	col = (x << 16) + (y << 8) + z;
	return(col);
}


void fillmattab(val,mattab)
double val;
ushort *mattab;
{
	int tot,ival;
	int i;

	val *= (1 << 22);
	ival = val;
	tot = 32767; /* een half */

	for(i = 256; i > 0; i--){
		*(mattab) = (tot >> 16);
		mattab += 3;
		tot += ival;
	}
}


void cspfill(buf, fill, x)
short *buf,*fill;
int x;
{
	short r,g,b;

	b = fill[0];
	g = fill[1];
	r = fill[2];
	for (;x>0;x--){
		buf[0] = b;
		buf[1] = g;
		buf[2] = r;
		buf += 3;
	}
}


void cspadd(buf,cont,rect,x)
short *buf,cont[][3];
int x;
uchar *rect;
{
	short i;
	for (;x>0;x--){
		i = *(rect);
		rect += 4;
		buf[0] += cont[i][0];
		buf[1] += cont[i][1];
		buf[2] += cont[i][2];
		buf += 3;
	}
}


void cspret(buf,rect,x)
short *buf;
int x;
uchar *rect;
{
	int r,g,b;
	int dr = 0, dg = 0, db = 0;
	
	for(; x > 0; x--){
		b = buf[0];
		g = buf[1];
		r = buf[2];

		if (Cspace_err_diff) {
			r += dr;
			g += dg;
			b += db;
		}
		
		if (b & 0x4000){
			if (b<0) rect[2]=0;
			else rect[2]=255;
		} else rect[2] = b >> 6;

		if (g & 0x4000){
			if (g<0) rect[1]=0;
			else rect[1]=255;
		} else rect[1] = g >> 6;

		if (r & 0x4000){
			if (r<0) rect[0]=0;
			else rect[0]=255;
		} else rect[0] = r >> 6;

		if (Cspace_err_diff) {
			dr = r & 0x3f;
			dg = g & 0x3f;
			db = b & 0x3f;
		}

		buf += 3;
		rect += 4;
	}
}


void rotcspace(ibuf,cont_1,cont_2,cont_3,add)
struct ImBuf *ibuf;
short *cont_1,*cont_2,*cont_3,*add;
{
	short x,y,*buf;
	uchar *rect;

	x=ibuf->x;
	rect= (uchar *)ibuf->rect;

	buf=(short *)malloc(x*3*sizeof(short));
	if (buf){
		for(y=ibuf->y;y>0;y--){
			cspfill(buf,add,x);
			cspadd(buf,cont_1,rect+0,x);
			cspadd(buf,cont_2,rect+1,x);
			cspadd(buf,cont_3,rect+2,x);
			cspret(buf,rect,x);
			rect += x<<2;
		}
		free(buf);
	}
}


void cspace(ibuf,mat)
struct ImBuf *ibuf;
float mat[][4];
{
	short *cont_1,*cont_2,*cont_3,add[3];

	cont_1=(short *)malloc(256*3*sizeof(short));
	cont_2=(short *)malloc(256*3*sizeof(short));
	cont_3=(short *)malloc(256*3*sizeof(short));

	if (cont_1 && cont_2 && cont_3){

		fillmattab(mat[0][0],cont_1);
		fillmattab(mat[0][1],cont_1+1);
		fillmattab(mat[0][2],cont_1+2);

		fillmattab(mat[1][0],cont_2);
		fillmattab(mat[1][1],cont_2+1);
		fillmattab(mat[1][2],cont_2+2);

		fillmattab(mat[2][0],cont_3);
		fillmattab(mat[2][1],cont_3+1);
		fillmattab(mat[2][2],cont_3+2);

		add[0] = (mat[3][0] * 64.0) + .5;
		add[1] = (mat[3][1] * 64.0) + .5;
		add[2] = (mat[3][2] * 64.0) + .5;

		rotcspace(ibuf, cont_1, cont_2, cont_3, add);
	}

	if (cont_1) free(cont_1);
	if (cont_2) free(cont_2);
	if (cont_3) free(cont_3);
}


void init_safechromamap()
{
	float u, u2, v, len;
	short * map, safeu, safev;

	if (safechromamap) return;
	safechromamap = (short *) malloc(256 * 256 * sizeof(short));
	if (safechromamap == 0) return;

	map = safechromamap;

	for (u = -1.0; u < 1.0; u += 2.0 / 256.0){
		u2 = u * u;
		for (v = -1.0; v < 1.0; v += 2.0 / 256.0){
			len = u2 + v * v;
			if (len > 0.75 * 0.75){
				len = 128.0 * 0.75 / sqrt(len);
				safeu = (u * len) + 128.5;
				safev = (v * len) + 128.5;
				*map++ = (safeu << 8) + (safev & 0xff);
			} else{
				*map++ = 0;
			}
		}
	}
}

void init_safelumamap()
{
	int y, u, v;
	float fy, fu, fv;

	if (safelumamap) return;
	safelumamap = mallocstruct(uchar, 256 * 256);
	if (safelumamap == 0) return;

	for (u = 0; u < 256; u++){
		fu = (u - 128.0) / 128.0;
		for (v = 0; v < 256; v++){
			fv = (v - 128.0) / 128.0;
			fy = (fu * fu) + (fv * fv);
			fy *= 256.0;
			if (fy > 255.0) fy = 255.0;
			safelumamap[(u << 8) + v] = fy;
		}
	}
}


void safechroma(struct ImBuf * ibuf)
{
	uchar * rect;
	int i;
	ushort c;

	if (ibuf == 0) return;
	if (ibuf->rect == 0) return;

	if (safechromamap == 0) init_safechromamap();
	if (safechromamap == 0) return;

	rect = (uchar *) ibuf->rect;
	for (i = ibuf->x * ibuf->y; i > 0; i--){
		c = safechromamap[(rect[1] << 8) + rect[3]];
		if (c) {
			rect[1] = c >> 8;
			rect[3] = c;
		}
		rect += 4;
	}
}


void safeluma(struct ImBuf * ibuf)
{
	short x, y, t;
	float val, val2;
	uchar *rect;

	if (ibuf == 0) return;
	if (ibuf->rect == 0) return;

	if (safelumamap == 0) init_safelumamap();
	if (safelumamap == 0) return;

	rect = (uchar *) ibuf->rect;
	for(y = ibuf->y ; y > 0; y--) {
		for(x = ibuf->x ; x > 0; x--) {
			t = safelumamap[(rect[1] << 8) + rect[3]];
			if (t > rect[2]) {
				/* u en v naar beneden scalen */

				val = 1.0 - ((t - rect[2]) / (2.0 * t));
				val2 = (val + (val * val)) / 2.0;

				rect[1] = (val2 * (rect[1] - 128.0)) + 128.5;
				rect[3] = (val2 * (rect[3] - 128.0)) + 128.5;
				
				/* y middelen met 'veilige' waarde */
				
				rect[2] = t + 0.5 + val * (rect[2] - t);
			}
			rect += 4;
		}
	}
}


void gamwarp(ibuf,gamma)
struct ImBuf *ibuf;
double gamma;
{
	uchar gam[256];
	int i;
	uchar *rect;

	if (ibuf == 0) return;
	if (ibuf->rect == 0) return;
	if (gamma == 1.0) return;

	gamma = 1.0 / gamma;
	for (i = 255 ; i >= 0 ; i--) gam[i] = (255.0 * pow(i / 255.0 , gamma)) + 0.5;

	rect = (uchar *) ibuf->rect;
	for (i = ibuf->x * ibuf->y ; i>0 ; i--){
		rect ++;
		*rect ++ = gam[*rect];
		*rect ++ = gam[*rect];
		*rect ++ = gam[*rect];
	}
}

