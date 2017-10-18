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


#define OBJECTBLOK "iff"

ushort start_iff(struct ImBuf *ibuf, int file)
{
	uint *point, size, *buf, start;
	
	if ((point=buf=(uint *)malloc(32768))==0) return FALSE;

	*point++ = FORM;				/* FORMxxxxILBM in buffer */
	*point++ = 0;

	if (IS_amiga(ibuf)){
		struct BitMapHeader *bmhd;

		*point++ = ILBM;
		*point++ = CAMG;
		*point++ = 4;
		*point++ = (ibuf->ftype & 0xffff);

		*point++=BMHD;
		*point++=sizeof(struct BitMapHeader);

		bmhd=(struct BitMapHeader *)point;		/* bmhd wijst naar plek waar bmhd moet komen */
		point=(uint *)((char *)point+sizeof(struct BitMapHeader));	/* pointer alvast verder zetten */

		bmhd->w=ibuf->x;
		bmhd->h=ibuf->y;
		bmhd->pageWidth=ibuf->x;
		bmhd->pageHeight=ibuf->y;
		bmhd->x=0;
		bmhd->y=0;
		bmhd->nPlanes=ibuf->depth;
		bmhd->masking=0;
		if (ibuf->flags & IB_vert){
			bmhd->compression=2;
		}
		else{
			bmhd->compression=1;
		}
		bmhd->pad1=0;
		bmhd->transparentColor=0;
		bmhd->xAspect=1;
		bmhd->yAspect=1;
	} else if (IS_cdi(ibuf)){
		struct Ihdr *ihdr;

		*point++=IMAG;
		*point++=IHDR;
		*point++=sizeof(struct Ihdr);

		ihdr=(struct Ihdr *)point;
		point=(uint *)((char *)point+sizeof(struct Ihdr));	/* pointer alvast verder zetten */

		ihdr->w = ibuf->x;
		ihdr->lsize = ibuf->x;
		ihdr->h = ibuf->y;
		ihdr->model = (ibuf->ftype & 0xffff);
		ihdr->dyuvkind = 0;
		switch (ihdr->model){
		case DYUV:
			ihdr->depth = 8;
			start = getdyuvstart();
			ihdr->yuvstart.y = (start >> 16) & 0xff;
			ihdr->yuvstart.u = (start >>  8) & 0xff;
			ihdr->yuvstart.v = (start >>  0) & 0xff;
			break;
		case RL7:
			ihdr->depth = 24;
			ihdr->yuvstart.y = 0;
			ihdr->yuvstart.u = 0;
			ihdr->yuvstart.v = 0;
			break;
		}
	} else if (IS_anim(ibuf)){
		struct Adat *adat;
		extern float adat_gamma;
		extern float adat_distort;
		
		*point++ = ANIM;
		*point++ = ADAT;
		*point++ = BIG_LONG(sizeof(struct Adat));

		adat = (struct Adat *)point;
		point = (uint *)((char *)point+sizeof(struct Adat));	/* pointer alvast verder zetten */

		adat->w = BIG_SHORT(ibuf->x);
		adat->h = BIG_SHORT(ibuf->y);

		adat->type = BIG_SHORT(ibuf->ftype);
		adat->xorig = BIG_SHORT(ibuf->xorig);
		adat->yorig = BIG_SHORT(ibuf->yorig);
		adat->pad = 0;
		adat->gamma = adat_gamma;
		adat->distort = adat_distort;
	}

	size=((uchar *)point-(uchar *)buf);
	if (write(file,buf,size)!=size){
		free(buf);
		return (FALSE);
	}

	if (ibuf->cmap){
		if (IS_anim(ibuf)){
			size = ibuf->maxcol * sizeof(int);
			buf[0] = CMAP;
			buf[1] = BIG_LONG(size);
			if (write(file,buf,8) != 8){
				free(buf);
				return (FALSE);
			}
			if (write(file,ibuf->cmap,size) != size){
				free(buf);
				return (FALSE);
			}
		} else{
			uchar *cpoint,*cols;
			uint i,bits;

			point = buf;
			if (IS_amiga(ibuf)){
				*(point++) = CMAP;
				*(point++) = BIG_LONG(3*ibuf->maxcol);
			} else if (IS_cdi(ibuf)){
				*(point++) = PLTE;
				*(point++) = BIG_LONG((3*ibuf->maxcol) + 4);
				*(point++) = BIG_LONG(ibuf->maxcol);
			}

			cpoint = (uchar *) point;
			cols = (uchar *)ibuf->cmap;
			if ((ibuf->cbits > 0) && (ibuf->cbits < 8)){
				bits = ~((1 << (8-ibuf->cbits)) - 1);
			} else bits = -1;

			if (IS_ham(ibuf)) bits = -1;
			
			for (i=0 ; i<ibuf->maxcol ; i++){
				*(cpoint++) = cols[0] & bits;
				*(cpoint++) = cols[1] & bits;
				*(cpoint++) = cols[2] & bits;
				cols += 4;
			}
			if (ibuf->maxcol & 1) *(cpoint++)=0;

			size=(cpoint-(uchar *)buf);
			if (write(file,buf,size)!=size){
				free(buf);
				return (FALSE);
			}
		}
	}

	if (IS_amiga(ibuf)) buf[0] = BODY;
	if (IS_cdi(ibuf)) buf[0] = IDAT;
	if (IS_anim(ibuf)) buf[0] = BODY;
	buf[1]=0;

	if (write(file,buf,8)!=8){
		free(buf);
		return(FALSE);
	}

	free(buf);
	return (TRUE);
}


ushort update_iff(int file, int code)
{
	int	buf[2], filelen, skip;
	uchar nop;

	if (file<=0) return (FALSE);

	filelen = filesize(file)-8;			/* filelengte berekenen */

	lseek(file,0L,2);		/* seek end */

	if (filelen & 1){						/* lengte even maken */
		switch(code){
		case BODY:
			nop = IFFNOP;
			break;
		case IDAT:
			nop = 0;
			break;
		}
		if (write(file,&nop,1)!=1) return (FALSE);
		filelen++;
	}
	lseek(file,4L,0);

	buf[0] = BIG_LONG(filelen);
	
	if (write(file, buf, 4) != 4) return (FALSE);
	if (code == 0) return (TRUE);

	filelen-=4;
	lseek(file,4L,1);

	while (filelen>0){		/* zoek BODY op */
		read(file, buf, 8);
		filelen -= 8;
		if (buf[0] == code) break;
		
		skip = BIG_LONG(buf[1]) + 1 & ~1;
		filelen -= skip;
		lseek(file, skip, 1);
	}
	if (filelen <= 0) {
		printf("update_iff: couldn't find chunk\n");
		return (FALSE);
	}

	lseek(file, -4L, 1);
	
	buf[0] = BIG_LONG(filelen);
	
	if (write(file, buf, 4)!=4) return (FALSE);

	return (TRUE);
}

