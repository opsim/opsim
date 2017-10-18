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


#define OBJECTBLOK "cdi"

uchar decdyuvtab[]={
	0,   1,   4,   9,
	16,  27,  44,  79,
	128, 177, 212, 229,
	240, 247, 252, 255
};

uint dyuvstart = 0x108080;		/* volgorde = yuv */


void setdyuvstart(uint start)
{
	dyuvstart = start;
}


uint getdyuvstart()
{
	return (dyuvstart);
}


void decdyuv(ibuf,ihdr,idat)
struct ImBuf *ibuf;
struct Ihdr *ihdr;
uchar *idat;
{
	short x,y,skip;
	uchar *rect,dy,du,dv,code;
	
	skip = 4 * (ihdr->lsize - ihdr->w);
	rect = (uchar *) ibuf->rect;

	dy = ihdr->yuvstart.y;
	du = ihdr->yuvstart.u;
	dv = ihdr->yuvstart.v;
	setdyuvstart((dy << 16) + (du << 8) + dv);
	
	for(y=ihdr->h;y>0;y--){

		dy = ihdr->yuvstart.y;
		du = ihdr->yuvstart.u;
		dv = ihdr->yuvstart.v;

		for(x=0;x<ihdr->w;x++){
			rect[0] = 0xff;
			code = *(idat++);
			dy += decdyuvtab[code & 0xf];
			rect[1] = dy;
			if (x & 1){
				dv += decdyuvtab[code >> 4];
				rect[3]=dv;
			} else{
				du += decdyuvtab[code >> 4];
				rect[2]=du;
			}
			rect +=4;
		}
		rect += skip;
	}

	rect = (uchar *) ibuf->rect;

	for(y=ihdr->h;y>0;y--){

		du = ihdr->yuvstart.u;
		dv = ihdr->yuvstart.v;

		for(x=0;x<(ihdr->w-1);x++){
			if (x & 1){
				dv = rect[3];
				rect[2] = (du + rect[6]) >> 1;
			} else{
				du = rect[2];
				rect[3] = (dv + rect[7]) >> 1;
			}
			rect +=4;
		}
		if (x & 1){
			rect[2] = du;
		} else{
			rect[3] = dv;
		}
		rect +=4;
		rect += skip;
	}
	cspace(ibuf,dyuvrgb);
}


void dec_rl7(ibuf,ihdr,idat)
struct ImBuf *ibuf;
struct Ihdr *ihdr;
register uchar *idat;
{
	int x,y,code,len, step;
	uchar *rect;
	extern void rectfill();
	
	if (ibuf->rect){
		/* rect wissen */
		rectoptot(ibuf, 0, rectfill, 0);
		rect = ((uchar *) ibuf->rect) + 3;
		step = 4;
	} else {
		rect = ibuf->chardata;
		step = 1;
	}
	
	for(y=ihdr->h;y>0;y--){
		x=ihdr->w;
		while (x>0){
			code = *(idat++);
			if (code & 128){
				code &= ~128;
				len = *(idat ++);
				if (len < 2){
					if (len == 0) len = x;
					else printf("Error in rl7: len = 1.\n");
				}
				x -= len;
				for (;len>0;len--){
					*rect = code;
					rect += step;
				}
			} else {
				*rect = code;
				rect += step;
				x--;
			}
		}

		if (x) {
			printf("error in RL7\n");
			break;
		}
	}
}


void dec_rl3(ibuf,ihdr,idat)
struct ImBuf *ibuf;
struct Ihdr *ihdr;
register uchar *idat;
{
	int x,y,code,len;
	register uint *rect;

	rect = ibuf->rect;
	
	for(y=ihdr->h;y>0;y--){
		x=ihdr->w / 2;
		while (x>0){
			code = *(idat++);
			if (code & 128){
				code &= ~128;
				len = *(idat ++);
				if (len < 2){
					if (len == 0) len = x;
					else printf("Error in rl3: len = 1.\n");
				}
				x -= len;
				for (;len>0;len--){
					*(rect++) = (code >> 4) & 0x7;
					*(rect++) = code & 0x7;
				}
			} else {
				*(rect++) = (code >> 4) & 0x7;
				*(rect++) = code & 0x7;
				x--;
			}
		}

		if (x) {
			printf("error in RL3\n");
			break;
		}
	}
}

uchar *enc_rl7_scanl(buf,rect,x)
uchar *buf;
int *rect;
int x;
{
	short last,this,xstart;
	uchar *wbuf;

	wbuf = buf;
	for (xstart = x - 1; xstart>0 ; xstart--) *(buf++) = *(rect++);

	buf[0] = buf[-1]; 	/* zodat ie op 't eind altijd met een run moet eindigen */
	buf = wbuf;
	last = *(buf++);
	x--;

	while (x>0){
		x--;
		if ((this = *(buf)++) == last){
			*(wbuf++) = last | 128;
			xstart = x + 1;
			while (x > 0){
				x--;
				if ((this = *(buf++)) != last) break;
			}
			if (x) {
				xstart -= x;
				while (xstart > 255) {
					*(wbuf++) = 255;
					*(wbuf++) = last | 128;
					xstart -= 255;
				}
				if (xstart != 1) *(wbuf++) = xstart;
				else wbuf[-1] = last & ~128;

			} else *(wbuf++) = 0;
		} else {
			*(wbuf++) = last & ~128;
		}
		last = this;
	}
	return(wbuf);
}


void codedyuv(uchar *buf, uchar *enc, int x, int start)
{
	int col, dif, code, ncol;
	int i;

	col = start;
	for (i = 0; i<x ; i++){
		dif = buf[i] - col;
		code = enc[dif & 0xff];
		ncol = col + decdyuvtab[code];

		/* check over- / underflow */
		if ((dif ^ ncol) & 256){
			if (dif < 0) code ++;
			else code -- ;
			ncol = col + decdyuvtab[code];
		}

		buf[i] = code;
		col = ncol & 0xff;
	}
}


void make_encdyuvtab(uchar *enc)
{
	int i=0, dec, this, next, cntr;

	for (dec = 0; dec < sizeof(decdyuvtab) - 1; dec++){
		this = decdyuvtab[dec];
		next = decdyuvtab[dec+1];
		cntr = (this + next) >> 1;
		while (i <= cntr) enc[i++] = dec;
	}
	while (i <= next) enc[i++] = dec;
}


short enc_dyuv(ibuf,file)
struct ImBuf *ibuf;
int file;
{
	uchar *_bufy, *_bufu, *_bufv, *_rect;
	uchar  *bufy,  *bufu,  *bufv,  *rect;
	uchar enctab[256];
	int x, y, sizex, halfx, size, start;
	short ok = TRUE;

	if (ibuf == 0) return(0);
	if (ibuf->rect == 0 && ibuf->chardata == 0) return(0);

	if (ibuf->chardata){
		size = ((ibuf->x + 3) & ~3) * ibuf->y;
		if (write(file, ibuf->chardata, size) != size) return(FALSE);
		return(TRUE);
	}

	/*cspace(ibuf, rgbbeta);*/
	cspace(ibuf, rgbdyuv);
	make_encdyuvtab(enctab);

	sizex = ibuf->x;
	halfx = sizex >> 1;

	_bufy = mallocN(sizex, "y");
	_bufu = mallocN(halfx, "u");
	_bufv = mallocN(halfx, "v");

	if (_bufy == 0 || _bufu == 0 || _bufv == 0){
		if (_bufy) freeN(_bufy);
		if (_bufu) freeN(_bufu);
		if (_bufv) freeN(_bufv);
		return(0);
	}
	
	start = getdyuvstart();
	_rect = (uchar *) ibuf->rect;
	for (y = ibuf->y; y>0; y--){
		rect = _rect + 2;
		bufy = _bufy;
		for (x = sizex; x>0; x--){
			*bufy++ = *rect;
			rect += 4;
		}

		rect = _rect + 1;
		bufu = _bufu;
		for (x = halfx; x>0; x--){
			*bufu++ = (rect[0] + rect[4]) >> 1;
			rect += 8;
		}

		rect = _rect + 3;
		bufv = _bufv;
		for (x = halfx; x>0; x--){
			*bufv++ = (rect[0] + rect[4]) >> 1;
			rect += 8;
		}

		codedyuv(_bufy, enctab, sizex, (start >> 16) & 0xff);
		codedyuv(_bufu, enctab, halfx, (start >>  8) & 0xff);
		codedyuv(_bufv, enctab, halfx, (start >>  0) & 0xff);

		bufy = _bufy;
		bufu = _bufu;
		bufv = _bufv;
		for (x = 0; x<sizex; x++){
			if (x & 1) *bufy++ += *bufv++ << 4;
			else *bufy++ += *bufu++ << 4;
		}

		if(write(file, _bufy, sizex) != sizex) {
			ok = FALSE;
			break;
		}
		_rect += (sizex << 2);
	}

	free(_bufy);
	free(_bufu);
	free(_bufv);
	return(ok);
}


short enc_rl7(ibuf,file)
struct ImBuf *ibuf;
int file;
{
	short ok=TRUE;
	int x,y,size;
	uchar *buf,*end;
	uint *rect;

	buf = malloc(ibuf->x + 1);
	if (buf==0) return (FALSE);

	y = ibuf->y;
	x = ibuf->x;
	rect = ibuf->rect;

	for (;y>0;y--){
		end = enc_rl7_scanl(buf,rect,x);
		rect += x;
		size = end-buf;
		if (write(file,buf,size) != size){
			ok = FALSE;
			break;
		}
	}

	free(buf);
	return(ok);
}


uchar *enc_rl3_scanl(buf,rect,x)
uchar *buf;
int *rect;
int x;
{
	short last,this,xstart;
	uchar *wbuf;

	x /= 2;
	wbuf = buf;
	for (xstart = x - 1; xstart>0 ; xstart--){
		last = (*(rect++) & 0x7) << 4;
		*(buf++) = last | (*(rect++) & 0x7);
	}

	buf[0] = buf[-1]; 	/* zodat ie op 't eind altijd met een run moet eindigen */
	/* mocht dit niet werken bij rl3 verander dan xstart = x-1 weer in x
		en zet 'n vlag als de laatste pixel een unieke pixel was. Bij rl7 kon namelijk
		de laatste unieke pixel ook index nul hebben, zodat daarop vergelijken een fout kon
		veroorzaken
	*/
	buf = wbuf;
	last = *(buf++);
	x--;

	while (x>0){
		x--;
		if ((this = *(buf)++) == last){
			*(wbuf++) = last | 128;
			xstart = x + 1;
			while (x > 0){
				x--;
				if ((this = *(buf++)) != last) break;
			}
			if (x) {
				xstart -= x;
				while (xstart > 255) {
					*(wbuf++) = 255;
					*(wbuf++) = last | 128;
					xstart -= 255;
				}
				if (xstart != 1) *(wbuf++) = xstart;
				else wbuf[-1] = last | 8;

			} else *(wbuf++) = 0;
		} else {
			*(wbuf++) = last | 8;
		}
		last = this;
	}


	return(wbuf);
}


short enc_rl3(ibuf,file)
struct ImBuf *ibuf;
int file;
{
	short ok=TRUE;
	int x,y,size;
	uchar *buf,*end;
	uint *rect;

	buf = malloc((ibuf->x / 2) + 1);
	if (buf==0) return (FALSE);

	y = ibuf->y;
	x = ibuf->x;
	rect = ibuf->rect;

	for (;y>0;y--){
		end = enc_rl3_scanl(buf,rect,x);
		rect += x;
		size = end-buf;
		if (write(file,buf,size) != size){
			ok = FALSE;
			break;
		}
	}

	free(buf);
	return(ok);
}


void decclut8(ibuf,ihdr,idat)
struct ImBuf *ibuf;
struct Ihdr *ihdr;
uchar *idat;
{
	uint * rect;
	int i;
	
	rect = ibuf->rect;
	for (i = ibuf->x * ibuf->y; i > 0; i--){
		*rect++ = *idat++;
	}
}


struct ImBuf *loadcdi(iffmem,flags)
int *iffmem,flags;
{
	int chunk,totlen,len,*mem,*cmap=0,cmapofs,cmaplen;
	uchar *idat=0;
	struct Ihdr ihdr;
	struct ImBuf *ibuf=0;

	mem=iffmem;
	ihdr.w=0;

	if (GL(mem) != FORM) return (0);
	if (GL(mem+2) != IMAG) return (0);
	totlen= (GL(mem+1) + 1) & ~1;
	mem += 3;
	totlen -= 4;

	while(totlen>0){
		chunk=GL(mem);
		len= (GL(mem+1) + 1) & ~1;
		mem += 2;

		totlen -= len+8;

		switch (chunk){
		case IHDR:
			memcpy(&ihdr,mem,sizeof(struct Ihdr));
			break;
		case PLTE:
			cmap=mem+1;
			cmapofs = ((ushort *)mem)[0];
			cmaplen = ((ushort *)mem)[1];
			break;
		case IDAT:
			idat=(uchar *)mem;
			break;
		}
		mem = (int *)((uchar *)mem +len);
		if (idat) break;
	}

	if (ihdr.w==0) return (0);
	if (idat==0) return (0);

/*
    printf("w:%d w:%d h:%d model:%d\n",ihdr.w,ihdr.lsize,ihdr.h,ihdr.model);
    printf("dpth:%d dyuvk:%d y:%d u:%d v:%d\n",ihdr.depth,ihdr.dyuvkind,ihdr.yuvstart.y,ihdr.yuvstart.u,ihdr.yuvstart.v);
*/	
	
	if (flags & IB_test) ibuf=allocImBuf(ihdr.w,ihdr.h,24,0,0);
	else if (flags & IB_char) ibuf=allocImBuf(ihdr.w,ihdr.h,24,IB_char | IB_charonlong,0);
	else ibuf=allocImBuf(ihdr.w,ihdr.h,24,IB_rect,0);

	if (ibuf==0) return (0);

	ibuf->ftype = (CDI | ihdr.model);

	if (cmap){
		/*		printf("cmapofs: %d cmaplen:%d \n",cmapofs,cmaplen);
		*/		ibuf->maxcol = cmaplen;
		addcmapImBuf(ibuf);
		makecolarray(ibuf,cmap);
	}

	if (flags & IB_test){
		if (flags & IB_freem) free(iffmem);
		return(ibuf);
	}

	switch(ihdr.model){
	case DYUV:
		if (flags & IB_char && ibuf->chardata) memcpy(ibuf->chardata, idat, ihdr.lsize * ihdr.h);
		else /*if (flags & IB_rect)*/ decdyuv(ibuf,&ihdr,idat);
		break;
	case CLUT7:
	case CLUT8:
		if (flags & IB_char && ibuf->chardata) memcpy(ibuf->chardata, idat, ihdr.lsize * ihdr.h);
		else /*if (flags & IB_rect)*/ decclut8(ibuf,&ihdr,idat);
		break;
	case RL7:
		dec_rl7(ibuf,&ihdr,idat);
		break;
	case RL3:
		dec_rl3(ibuf,&ihdr,idat);
		break;
	default:
		freeImBuf(ibuf);
		return(0);
	}

	if (ibuf->cmap != 0 && (flags & IB_cmap) == 0) applycmap(ibuf);
	if (flags & IB_freem) free(iffmem);

	if ((flags & IB_ttob) == 0) flipy(ibuf);
	
	if (ibuf) {
		if (ibuf->rect) 
			convert_rgba_to_abgr((ibuf->x+ibuf->skipx)*ibuf->y, ibuf->rect);
	}
	
	return (ibuf);
}

