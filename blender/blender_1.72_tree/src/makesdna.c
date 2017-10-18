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



/* makesdna.c */

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
typedef unsigned int uint;
#endif

#ifndef WIN32
#include <strings.h>
#include <unistd.h>
#endif

#include <fcntl.h>
#include "file.h"
#include "util.h"

short le_short(short temp)
{
	short new;
	char *rt=(char *)&temp, *rtn=(char *)&new;

	rtn[0]= rt[1];
	rtn[1]= rt[0];

	return new;
}

int le_int(int temp)
{
	int new;
	char *rt=(char *)&temp, *rtn=(char *)&new;

	rtn[0]= rt[3];
	rtn[1]= rt[2];
	rtn[2]= rt[1];
	rtn[3]= rt[0];

	return new;
}



int main()
{
	struct stat buf;
	int file, filelen, dnalen, a, ok=0;
	char *dnadata, *objectdata, ch, *cp;
	
	/* maak file "DNA" */
	make_structDNA();
	
	/* lees file weer in */
	file= open("DNA", O_BINARY+O_RDONLY);
	if(file== -1) {
		printf("Can't read DNA file\n");
		exit(0);
	}

	fstat(file, &buf);
	dnalen= buf.st_size;	
	dnadata= malloc(dnalen);
	read(file, dnadata, dnalen);
	close(file);
	
	/* schrijf DNA.c */
	file= open("DNA.c", O_BINARY+O_WRONLY+O_CREAT+O_TRUNC, 0666);
	
	if(file== -1) {
		printf("Can't write DNA.c\n");
	}
	else {
		
		write(file, "char DNAstr[]=", 14);
		ch= '"';
		write(file, &ch, 1);
		a= dnalen+4;
		ch= ' ';
		while(a--) {
			write(file, &ch, 1);
		}
		ch= '"';
		write(file, &ch, 1);
		ch= ';';
		write(file, &ch, 1);
		ch= '\n';
		write(file, &ch, 1);
		close(file);
		
		/* compileren */
		#ifdef __sgi
			system("cc -mips1 -c DNA.c");
		#else
			system("gcc -c DNA.c");
		#endif
		
		/* inlezen */
		file= open("DNA.o", O_BINARY+O_RDONLY);
		if(file== -1) {
			printf("Can't read DNA.o \n");
			exit(0);
		}
	
		fstat(file, &buf);
		filelen= buf.st_size;
				
		objectdata= malloc(filelen);
		read(file, objectdata, filelen);
		close(file);
		
		/* zoeken naar 4 spaties */
		cp= objectdata;
		a= filelen;
		while(a--) {
			if(cp[0]==' ' && cp[1]==' ' && cp[2]==' ' && cp[3]==' ') {
				ok= 1;
				memcpy(cp+4, dnadata, dnalen);
				*( (int *)cp)= dnalen;
				break;
			}
			cp++;
		}
		
		/* schrijf DNA.o */
		if(ok) {
			file= open("DNA.o", O_BINARY+O_WRONLY+O_CREAT+O_TRUNC, 0666);
			
			if(file== -1) {
				printf("Can't write DNA.o\n");
			}
			else {
				#if !defined(__BeOS) && !defined(WIN32)
				fchmod(file, 0664);
				#endif
				
				write(file, objectdata, filelen);
				close(file);
				printf("Saved: DNA.o\n");
			}
		}
		else printf("DNA.o is wrong\n");
	}
	
	exit(0);
}

