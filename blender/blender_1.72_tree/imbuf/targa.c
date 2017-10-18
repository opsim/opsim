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


#define OBJECTBLOK "targa"

int tga_out1(data,file)
uint data;
FILE * file;
{
	uchar *p;

	p = (uchar *) & data;
	if (putc(p[0],file) == EOF) return(EOF);
	return (~EOF);
}

int tga_out2(data,file)
uint data;
FILE * file;
{
	uchar *p;

	p = (uchar *) & data;
	if (putc(p[0],file) == EOF) return(EOF);
	if (putc(p[1],file) == EOF) return(EOF);
	return (~EOF);
}


int tga_out3(data,file)
uint data;
FILE * file;
{
	uchar *p;

	p = (uchar *) & data;
	if (putc(p[2],file) == EOF) return(EOF);
	if (putc(p[1],file) == EOF) return(EOF);
	if (putc(p[0],file) == EOF) return(EOF);
	return (~EOF);
}


int tga_out4(data,file)
uint data;
FILE * file;
{
	uchar *p;

	p = (uchar *) & data;
	/* volgorde = bgra */
	if (putc(p[2],file) == EOF) return(EOF);
	if (putc(p[1],file) == EOF) return(EOF);
	if (putc(p[0],file) == EOF) return(EOF);
	if (putc(p[3],file) == EOF) return(EOF);
	return (~EOF);
}

short makebody_tga(ibuf, file, out)
ImBuf * ibuf;
FILE * file;
int (*out)();
{
	register int last,this;
	register int copy, bytes;
	register unsigned int *rect, *rectstart, *temp;
	int y;
	
	for (y = 0; y < ibuf->y; y++) {
		bytes = ibuf->x - 1;
		rectstart = rect = ibuf->rect + (y * ibuf->x);
		last = *rect++;
		this = *rect++;
		copy = last^this;
		while (bytes > 0){
			if (copy){
				do{
					last = this;
					this = *rect++;
					if (last == this){
						if (this == rect[-3]){	/* drie dezelfde? */
							bytes --;		/* bytes goed zetten */
							break;
						}
					}
				}while (--bytes != 0);

				copy = rect-rectstart;
				copy --;
				if (bytes) copy -= 2;

				temp = rect;
				rect = rectstart;

				while (copy){
					last = copy;
					if (copy>=128) last = 128;
					copy -= last;
					if (fputc(last-1,file) == EOF) return(0);
					do{
						if (out(*rect++,file) == EOF) return(0);
					}while(--last != 0);
				}
				rectstart = rect;
				rect = temp;
				last = this;

				copy = FALSE;
			} else {
				while (*rect++ == this){		/* zoek naar eerste afwijkende byte */
					if (--bytes == 0) break;	/* of einde regel */
				}
				rect --;
				copy = rect-rectstart;
				rectstart = rect;
				bytes --;
				this = *rect++;

				while (copy){
					if (copy>128){
						if (fputc(255,file) == EOF) return(0);
						copy -= 128;
					} else {
						if (copy == 1){
							if (fputc(0,file) == EOF) return(0);
						} else if (fputc(127 + copy,file) == EOF) return(0);
						copy = 0;
					}
					if (out(last,file) == EOF) return(0);
				}
				copy=TRUE;
			}
		}
	}
	return (1);
}



int dumptarga(struct ImBuf * ibuf, FILE * file)
{
	int col,size;
	uchar *rect;

	if (ibuf == 0) return;
	if (ibuf->rect == 0) return;

	size = ibuf->x * ibuf->y;
	rect = (uchar *) ibuf->rect;

	if (ibuf->depth <= 8) {
		while(size > 0){
			if (putc(*rect, file) == EOF) return(0);
			size--;
			rect += 4;
		}
	} else if (ibuf->depth <= 16) {
		while(size > 0){
			putc(rect[0], file);
			if (putc(rect[1], file) == EOF) return(0);
			size--;
			rect += 4;
		}
	} else if (ibuf->depth <= 24) {
		while(size > 0){
			putc(rect[2], file);
			putc(rect[1], file);
			if (putc(rect[0], file) == EOF) return(0);
			size--;
			rect += 4;
		}
	} else if (ibuf->depth <= 32) {
		while(size > 0){
			putc(rect[2], file);
			putc(rect[1], file);
			putc(rect[0], file);
			if (putc(rect[3], file) == EOF) return(0);
			size--;
			rect += 4;
		}
	} else return(0);
	
	return(1);
}


short savetarga(ibuf, file, flags)
struct ImBuf * ibuf;
int file, flags;
{
	char buf[20];
	FILE *fildes;
	int i;
	short ok;

	if (ibuf == 0) return (0);
	if (ibuf->rect == 0) return (0);

	memset(buf,0,sizeof(buf));

	/* buf[0] = 0;  lengte string */

	buf[16] = (ibuf->depth + 0x7 ) & ~0x7;
	if (ibuf->cmap) {
		buf[1] = 1;
		buf[2] = 9;
		buf[3] = ibuf->mincol & 0xff;
		buf[4] = ibuf->mincol >> 8;
		buf[5] = ibuf->maxcol & 0xff;
		buf[6] = ibuf->maxcol >> 8;
		buf[7] = 24;
		if ((flags & IB_ttob) == 0) {
			flipy(ibuf);
			buf[17] = 0x20;
		}
	} else if (ibuf->depth > 8 ){
		buf[2] = 10;
	} else{
		buf[2] = 11;
	}

	if (ibuf->ftype == RAWTGA) buf[2] &= ~8;
	
	buf[8] = ibuf->xorig & 0xff;
	buf[9] = ibuf->xorig >> 8;
	buf[10] = ibuf->yorig & 0xff;
	buf[11] = ibuf->yorig >> 8;

	buf[12] = ibuf->x & 0xff;
	buf[13] = ibuf->x >> 8;
	buf[14] = ibuf->y & 0xff;
	buf[15] = ibuf->y >> 8;

	if (flags & IB_ttob) buf[17] ^= 0x20;

	if (write(file, buf, 18) != 18) return (0);
	if (ibuf->cmap){
		/* zoals het er nu naar uitziet, worden de colormapentries vanaf mincol weggeschreven
		 * er worden echter we maxcol entries verwacht
		 * daarom worden die aan het eind nog maar een keer weggeschreven
		 */
		
		for (i = 0 ; i<ibuf->maxcol ; i++){
			if (write(file,((uchar *)(ibuf->cmap + i)) + 1,3) != 3) return (0);
		}
/*		for (i = ibuf->mincol ; i<ibuf->maxcol ; i++){
			if (write(file,((uchar *)(ibuf->cmap + i)) + 1,3) != 3) return (0);
		}
		for (i = 0 ; i<ibuf->mincol ; i++){
			if (write(file,((uchar *)(ibuf->cmap + i)) + 1,3) != 3) return (0);
		}*/
	}
	fildes = fdopen(file,"ab");
	/*setvbuf(fildes, 0, _IOFBF, 8 * BUFSIZ);*/
	
	if (ibuf->cmap && (flags & IB_cmap) == 0) converttocmap(ibuf);
	
	if (ibuf->ftype == RAWTGA) {
		dumptarga(ibuf, fildes);
	} else {
		extern void rectandvalue();
		
		switch((ibuf->depth + 7) >> 3){
		case 1:
			ok = makebody_tga(ibuf, fildes, tga_out1);
			break;
		case 2:
			ok = makebody_tga(ibuf, fildes, tga_out2);
			break;
		case 3:
			ok = makebody_tga(ibuf, fildes, tga_out3);
			break;
		case 4:
			ok = makebody_tga(ibuf, fildes, tga_out4);
			break;
		}
	}
	
	fclose(fildes);
	return (ok);
}


int checktarga(TARGA *tga, char *mem)
{
	tga->numid = mem[0];
	tga->maptyp = mem[1];
	tga->imgtyp = mem[2];

	tga->maporig = GSS(mem+3);
	tga->mapsize = GSS(mem+5);
	tga->mapbits = mem[7];
	tga->xorig = GSS(mem+8);
	tga->yorig = GSS(mem+10);
	tga->xsize = GSS(mem+12);
	tga->ysize = GSS(mem+14);
	tga->pixsize = mem[16];
	tga->imgdes = mem[17];

	if (tga->maptyp > 1) return(0);
	switch (tga->imgtyp){
	case 1:			/* raw cmap */
	case 2:			/* raw rgb */
	case 3:			/* raw b&w */
	case 9:			/* cmap */
	case 10:			/* rgb */
	case 11:			/* b&w */
		break;
	default:
		return(0);
	}
	if (tga->mapsize && tga->mapbits > 32) return(0);
	if (tga->xsize <= 0 || tga->xsize >= 4096) return(0);
	if (tga->ysize <= 0 || tga->ysize >= 4096) return(0);
	if (tga->pixsize > 32) return(0);
	return(1);
}


void decodetarga(ibuf, mem, psize)
struct ImBuf *ibuf;
uchar *mem;
int psize;
{
	int count, col, size;
	uint *rect;
	uchar * cp = (uchar *) &col;
	
	if (ibuf == 0) return;
	if (ibuf->rect == 0) return;

	size = ibuf->x * ibuf->y;
	rect = ibuf->rect;
	
	/* alpha zetten */
	cp[0] = 0xff;
	cp[1] = cp[2] = 0;
	
	while(size > 0){
		count = *mem++;
		if (count >= 128) {
			/*if (count == 128) printf("TARGA: 128 in file !\n");*/
			count -= 127;

			if (psize & 2){
				if (psize & 1){
					/* volgorde = bgra */
					cp[0] = mem[3];
					cp[1] = mem[0];
					cp[2] = mem[1];
					cp[3] = mem[2];
					/*col = (mem[3] << 24) + (mem[0] << 16) + (mem[1] << 8) + mem[2];*/
					mem += 4;
				} else{
					cp[1] = mem[0];
					cp[2] = mem[1];
					cp[3] = mem[2];
					/*col = 0xff000000 + (mem[0] << 16) + (mem[1] << 8) + mem[2];*/
					mem += 3;
				}
			} else{
				if (psize & 1){
					col = (mem[0] << 8) + mem[1];
					mem += 2;
				} else{
					col = *mem++;
				}
			}

			size -= count;
			if (size >= 0) {
				while (count > 0) {
					*rect++ = col;
					count--;
				}
			}
		} else{
			count ++;
			size -= count;
			if (size >= 0) {
				while (count > 0){
					if (psize & 2){
						if (psize & 1){
							/* volgorde = bgra */
							cp[0] = mem[3];
							cp[1] = mem[0];
							cp[2] = mem[1];
							cp[3] = mem[2];
							/*col = (mem[3] << 24) + (mem[0] << 16) + (mem[1] << 8) + mem[2];*/
							mem += 4;
						} else{
							cp[1] = mem[0];
							cp[2] = mem[1];
							cp[3] = mem[2];
							/*col = 0xff000000 + (mem[0] << 16) + (mem[1] << 8) + mem[2];*/
							mem += 3;
						}
					} else{
						if (psize & 1){
							col = (mem[0] << 8) + mem[1];
							mem += 2;
						} else{
							col = *mem++;
						}
					}
					*rect++ = col;
					count --;
				}
			}
		}
	}
	if (size) printf("decodetarga: count would overwrite %d pixels\n", -size);
}

void ldtarga(struct ImBuf * ibuf,uchar * mem, int psize)
{
	int col,size;
	uint *rect;
	uchar * cp = (uchar *) &col;

	if (ibuf == 0) return;
	if (ibuf->rect == 0) return;

	size = ibuf->x * ibuf->y;
	rect = ibuf->rect;

	/* alpha zetten */
	cp[0] = 0xff;
	cp[1] = cp[2] = 0;

	while(size > 0){
		if (psize & 2){
			if (psize & 1){
				/* volgorde = bgra */
				cp[0] = mem[3];
				cp[1] = mem[0];
				cp[2] = mem[1];
				cp[3] = mem[2];
				/*col = (mem[3] << 24) + (mem[0] << 16) + (mem[1] << 8) + mem[2];*/
				mem += 4;
			} else{
				/* zet alpha bij 24 bits kleuren */
				cp[1] = mem[0];
				cp[2] = mem[1];
				cp[3] = mem[2];
				/*col = 0xff000000 + (mem[0] << 16) + (mem[1] << 8) + mem[2];*/
				mem += 3;
			}
		} else{
			if (psize & 1){
				col = (mem[1] << 8) + mem[0];
				mem += 2;
			} else{
				col = *mem++;
			}
		}
		*rect++ = col;
		size--;
	}
}


struct ImBuf *loadtarga(uchar *mem, int flags)
{
	TARGA tga;
	struct ImBuf * ibuf;
	int col, count, size;
	uint * rect;
	uchar * cp = (uchar *) &col;
	
	if (checktarga(&tga,mem) == 0) return(0);

	if (flags & IB_test) ibuf = allocImBuf(tga.xsize,tga.ysize,tga.pixsize,0,0);
	else ibuf = allocImBuf(tga.xsize,tga.ysize,(tga.pixsize + 0x7) & ~0x7,1,0);

	if (ibuf == 0) return(0);
	ibuf->ftype = TGA;
	ibuf->xorig = tga.xorig;
	ibuf->yorig = tga.yorig;
	mem = mem + 18 + tga.numid;
	
	cp[0] = 0xff;
	cp[1] = cp[2] = 0;
	
	if (tga.mapsize){
		ibuf->mincol = tga.maporig;
		ibuf->maxcol = tga.mapsize;
		addcmapImBuf(ibuf);
		ibuf->cbits = 8;
		for (count = 0 ; count < ibuf->maxcol ; count ++) {
			switch (tga.mapbits >> 3) {
				case 4:
					cp[0] = mem[3];
					cp[1] = mem[0];
					cp[2] = mem[1];
					cp[3] = mem[2];
					mem += 4;
					break;
				case 3:
					cp[1] = mem[0];
					cp[2] = mem[1];
					cp[3] = mem[2];
					mem += 3;
					break;
				case 2:
					col = (mem[1] << 8) + mem[0];
					mem += 2;
					break;
				case 1:
					col = *mem++;
					break;
			}
			ibuf->cmap[count] = col;
		}
		
		size = 0;
		for (col = ibuf->maxcol - 1; col > 0; col >>= 1) size++;
		ibuf->depth = size;

		if (tga.mapbits != 32) {	/* alpha bits zetten */
			ibuf->cmap[0] &= BIG_LONG(0x00ffffff);
		}
	}
	
	if (flags & IB_test) return (ibuf);

	if (tga.imgtyp != 1 && tga.imgtyp != 9) freecmapImBuf(ibuf); /* kan soms gebeuren (beuh) */

	switch(tga.imgtyp){
	case 1:
	case 2:
	case 3:
		if (tga.pixsize <= 8) ldtarga(ibuf,mem,0);
		else if (tga.pixsize <= 16) ldtarga(ibuf,mem,1);
		else if (tga.pixsize <= 24) ldtarga(ibuf,mem,2);
		else if (tga.pixsize <= 32) ldtarga(ibuf,mem,3);
		break;
	case 9:
	case 10:
	case 11:
		if (tga.pixsize <= 8) decodetarga(ibuf,mem,0);
		else if (tga.pixsize <= 16) decodetarga(ibuf,mem,1);
		else if (tga.pixsize <= 24) decodetarga(ibuf,mem,2);
		else if (tga.pixsize <= 32) decodetarga(ibuf,mem,3);
		break;
	}
	
	if (ibuf->cmap){
		if ((flags & IB_cmap) == 0) applycmap(ibuf);
	}
	
	if (tga.pixsize == 16 && ibuf->cmap == 0){
		rect = ibuf->rect;
		for (size = ibuf->x * ibuf->y; size > 0; size --){
			col = *rect;
			col = ((col & 0x1f) << 19) +  ((col & 0x3e0) << 6) + ((col & 0x7c00) >> 7) ;
			col += (col & 0xe0e0e0) >> 5;
			*rect++ = col + 0xff000000;
		}
		ibuf->depth = 24;
	}
	
	if (tga.imgtyp == 3 || tga.imgtyp == 11){
		uchar *rect;

		rect = (uchar *) ibuf->rect;
		for (size = ibuf->x * ibuf->y; size > 0; size --){
			rect[0] = 255;
			rect[1] = rect[2] = rect[3];
			rect += 4;
		}
	}
	
	if (flags & IB_ttob) tga.imgdes ^= 0x20;
	if (tga.imgdes & 0x20) flipy(ibuf);

	if (ibuf) {
		if (ibuf->rect && (flags & IB_cmap)==0) 
			convert_rgba_to_abgr((ibuf->x+ibuf->skipx)*ibuf->y, ibuf->rect);
	}
	
	return(ibuf);
}

