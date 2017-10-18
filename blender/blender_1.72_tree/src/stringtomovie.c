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

/* makestring.c	ma 96

 * 
 * Dit programma laadt "/pics/blender/lib/text_to_movie.blend",
 * verandert de string en schrijft rt.blend.
 * Daarna wordt een renderopdracht gegeven.
 * 
 * cc stringtomovie.c -o stringtomovie
 */


#include <stdio.h>
#include <fcntl.h>
#include <string.h>


void main(argc, argv)
int argc;
char **argv;
{
	int version, file, filelen;
	char str[256], *blender, *cp;

	if(argc!=3) {
		printf("usage: stringtomovie 'movie.mv string... '\n");
		exit(0);
	}

	strcpy(str, "/pics/blender/lib/text_to_movie.blend");
	file=open(str,O_BINARY|O_RDONLY);
	if(file== -1) {
		exit(0);
	}
	filelen= filesize(file);

	blender= (char *)malloc(filelen);
	read(file, blender, filelen);
	close(file);

	cp= blender+filelen-20;
	while(cp>blender) {
		if( *cp=='a' && *(cp+1)=='b' ) {
			if( *(cp+2)=='c' && *(cp+3)=='d' ) {
				if(strncmp(cp, "abcd    ", 8)==0) {
				
					strcpy(str, argv[2]);
					str[31]= 0;
					strcpy(cp, str);
					
					strcpy(str, "/usr/tmp/rt.blend");
					file=open(str,O_BINARY|O_WRONLY+O_CREAT+O_TRUNC);
					if(file== -1) {
						exit(0);
					}
					#ifndef __BeOS
					fchmod(file, 0664);
					#endif
					write(file, blender, filelen);
					close(file);
					
					free(blender);
					
					system("blender -b /usr/tmp/rt.blend -f 1 > /dev/null");
					
					sprintf(str, "mv /usr/tmp/0001_0001.mv %s\n", argv[1]);
					system(str);
					printf("Saved: %s\n", argv[1]);
				}
			}
		}
		cp--;
	}
	exit(0);
}

