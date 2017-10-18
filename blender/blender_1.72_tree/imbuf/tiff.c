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


#define OBJECTBLOK "tiff"

/* geeft een imbuf structuur terug die de stiff omschrijft */

int stiff_eof, stiff_count;

int get4(FILE * in, int type)
{
	int ret;
	int last;
	
	if (type == 0) {
		ret  = getc(in) << 24;
		ret |= getc(in) << 16;
		ret |= getc(in) << 8;
		ret |= last = getc(in);
	} else {
		ret  = getc(in);
		ret |= getc(in) << 8;
		ret |= getc(in) << 16;
		ret |= last = getc(in) << 24;
	}
	
	stiff_eof = (last == EOF);
	stiff_count += 4;
	return(ret);
}


int get2(FILE * in, int type)
{
	int ret;
	int last;
	
	if (type == 0) {
		ret = getc(in) << 8;
		ret |= last = getc(in);
	} else {
		ret = getc(in);
		ret |= last = getc(in) << 8;
	}
	
	stiff_eof = (last == EOF);
	stiff_count += 2;
	return(ret);
}


struct ImBuf * read_stiff(FILE * in)
{
	int offset, tag;
	int type = 0; /* BIG ENDIAN */
	short entrys;
	
	stiff_count = 0;
	
	tag = get2(in, type);

	if (tag == 'II') type = -1;
	else if (tag == 'MM') type = 0;
	else return (0);
	
	tag = get2(in, type);
	if (tag != 0x0042) return(0);
	
	/* waar begint de eerste Image File Directory ? */
	
	offset = get4(in, type);
	if (stiff_eof) return (0);
	
	if (stiff_count > offset) return(0);
	while (stiff_count < offset) {
		if (getc(in) == EOF) return(0);
		stiff_count++;
	}
	
	entrys = get2(in, type);
	if (entrys < 1) return (0);
	
	return(allocImBuf(10, 10, 32, 0, 0));
}

