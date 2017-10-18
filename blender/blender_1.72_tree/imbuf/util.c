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


#define OBJECTBLOK "imbufutil"

int ispic(name)
char * name;
{
	TARGA tga;
	struct stat st;
	int fp, buf[10];
	int ofs = 0;
	
	if (ib_stat(name,&st) == -1) return(0);
	if (((st.st_mode) & S_IFMT) == S_IFREG){
		if ((fp = open(name,O_BINARY|O_RDONLY)) >= 0){
			if (read(fp,buf,32)==32){
				close(fp);
				if (buf[ofs] == CAT) ofs += 3;
				if (buf[ofs] == FORM){
					if (buf[ofs + 2] == IMAG) return(CDI);
					if (buf[ofs + 2] == ILBM) return(AMI);
					if (buf[ofs + 2] == ANIM){
						if (buf[ofs + 3] == FORM){
							return(ANIM);
						}else{
							return(Anim);
						}
					}
				} else {
					if (GS(buf) == IMAGIC) return(IMAGIC);
					if (GSS(buf) == IMAGIC) return(IMAGIC);
					if ((BIG_LONG(buf[0]) & 0xfffffff0) == 0xffd8ffe0) return(JPG);

					/* at windows there are ".ffl" files with the same magic numnber... 
					   besides that,  tim images are not really important anymore! */
					/* if ((BIG_LONG(buf[0]) == 0x10000000) && ((BIG_LONG(buf[1]) & 0xf0ffffff) == 0)) return(TIM); */

				}
				if (checktarga(&tga, buf)) return(TGA);
				return(FALSE);
			}
			close(fp);
		}
	}
	return(FALSE);
}

