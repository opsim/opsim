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

void *avi_converter_from_rgb32 (AviMovie *movie, int stream, unsigned char *buffer, int *size) {
	int y, x, rowstridea, rowstrideb;
	unsigned char *buf;

	buf = mallocN (movie->header->Height * movie->header->Width * 3, "fromrgb32buf");
	*size = movie->header->Height * movie->header->Width * 3;

	rowstridea = movie->header->Width*3;
	rowstrideb = movie->header->Width*4;

	for (y=0; y < movie->header->Height; y++) {
		for (x=0; x < movie->header->Width; x++) {
			buf[y*rowstridea + x*3 + 0] = buffer[y*rowstrideb + x*4 + 3];
			buf[y*rowstridea + x*3 + 1] = buffer[y*rowstrideb + x*4 + 2];
			buf[y*rowstridea + x*3 + 2] = buffer[y*rowstrideb + x*4 + 1];
		}
	}

	freeN (buffer);

	return buf;
}

void *avi_converter_to_rgb32 (AviMovie *movie, int stream, unsigned char *buffer, int *size) {
	int i;
	unsigned char *buf;
	unsigned char *to, *from;

	buf= mallocN (movie->header->Height * movie->header->Width * 4, "torgb32buf");
	*size= movie->header->Height * movie->header->Width * 4;

	memset(buf, 0, *size);

	to= buf; from= buffer;
	i=movie->header->Height*movie->header->Width;
	
	while(i--) {
		memcpy(to, from, 3);
		to+=4; from+=3;
	}

	freeN (buffer);

	return buf;
}

