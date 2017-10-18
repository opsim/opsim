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

#include "avi.h"

#include <stdlib.h>

void *avi_converter_from_avi_rgb (AviMovie *movie, int stream, unsigned char *buffer, int *size) {
	int x, y,i, rowstride;
	unsigned char *buf;
	AviBitmapInfoHeader *bi;
	short bits= 32;

	bi= (AviBitmapInfoHeader *) movie->streams[stream].sf;
	if (bi) bits= bi->BitCount;

	if (bits==16) {
		unsigned short *pxl;
		unsigned char *to;
		#ifdef WORDS_BIGENDIAN
		unsigned char  *pxla;
		#endif		  
		
		buf = mallocN (movie->header->Height * movie->header->Width * 3, "fromavirgbbuf");

		y= movie->header->Height;
		to= buf;
				
		while (y--) {
			pxl= (unsigned short *) (buffer + y * movie->header->Width * 2);
			
			#ifdef WORDS_BIGENDIAN
			pxla= (unsigned char *)pxl;
			#endif

			x= movie->header->Width;
			while (x--) {
				#ifdef WORDS_BIGENDIAN
				i= pxla[0];
				pxla[0]= pxla[1];
				pxla[1]= i;
	
				pxla+=2;
				#endif
			
				*(to++)= ((*pxl>>10)&0x1f)*8;
				*(to++)= ((*pxl>>5)&0x1f)*8;
				*(to++)= (*pxl&0x1f)*8;
				pxl++;	
			}
		}

		freeN (buffer);
		
		return buf;
	} else {
		buf = mallocN (movie->header->Height * movie->header->Width * 3, "fromavirgbbuf");
	
		rowstride = movie->header->Width*3;
		if (bits!=16) if (movie->header->Width%2) rowstride++;
	
		for (y=0; y < movie->header->Height; y++) {
			memcpy (&buf[y*movie->header->Width*3], &buffer[((movie->header->Height-1)-y)*rowstride], movie->header->Width*3);
		}
	
		for (y=0; y < movie->header->Height*movie->header->Width*3; y+=3) {
			i = buf[y];
			buf[y] = buf[y+2];
			buf[y+2] = i;
		}
	
		freeN (buffer);
	
		return buf;
	}
}

void *avi_converter_to_avi_rgb (AviMovie *movie, int stream, unsigned char *buffer, int *size) {
	int y, x, i, rowstride;
	unsigned char *buf;

	*size= movie->header->Height * movie->header->Width * 3;
	if (movie->header->Width%2) *size+= movie->header->Height;
	
	buf = mallocN (*size,"toavirgbbuf");

	rowstride = movie->header->Width*3;
	if (movie->header->Width%2) rowstride++;

	for (y=0; y < movie->header->Height; y++) {
		memcpy (&buf[y*rowstride], &buffer[((movie->header->Height-1)-y)*movie->header->Width*3], movie->header->Width*3);
	}

	for (y=0; y < movie->header->Height; y++) {
		for (x=0; x < movie->header->Width*3; x+=3) {
			i = buf[y*rowstride+x];
			buf[y*rowstride+x] = buf[y*rowstride+x+2];
			buf[y*rowstride+x+2] = i;
		}
	}

	freeN (buffer);

	return buf;
}

