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

/* pluginseq.c  		MIXED MODEL

 * 
 * dec 95
 * 
 */

#include <sys/types.h>
#include "plugin.h"
#include "util.h"
#include <math.h>
#include "iff.h"
/*
 * 

cc -g -float -mips1 -c pluginseq.c ; ld pluginseq.o -o pluginseq.so -limbuf -limage -lc -lm ; cp pluginseq.so /pics/blender/plugin/seq/

NIEUWE DEFINITIE: verander alleen maar de waarde van de variabele PLUG
Je kunt vanuit je plugin alle libraries (ook de imbuf) gewoon aanroepen.

set PLUG=pluginseq ; cc -g -float -mips1 -c $PLUG.c ; ld -shared $PLUG.o -o $PLUG.so ; cp $PLUG.so /pics/blender/plugin/seq/

         ^^^^^^^^^
 * 
 * 
 */

/* ******************************************************************* */
/* 				ZELF INVULLEN: struct- en funktienamen niet wijzigen							   */
/* ******************************************************************* */

/* 1. naam: */

	char _name[24]= "specialkey";


/* 2. aantal sub types */

	#define NR_STYPES	1

/* 3. namen van subtypes (maximaal 16 bytes per naam) */

	char _stnames[NR_STYPES][16]= {"--"};

/* 4. informatie over externe variabelen */

	VarStruct _varstr[]= {
	 /* butcode,	naam,       default,min, max */
		NUM|FLO,	"fac",		0.5,	0.0, 1.0, 
		TOG|INT,	"hallo",	0.5,	0.0, 1.0, 
		SLI|FLO,	"g",		0.5,	0.0, 1.0, 
		NUMSLI|FLO,	"b",		0.5,	0.0, 1.0, 
	};

/* 5. hulpstruct om variabelen te casten */

	typedef struct Cast {
		float fac;
		int hallo;
		float g, b;
	} Cast;


/* deze funktie laten staan! */

void plugin_seq_getinfo( int *stypes, int *vars)
{

	*stypes= NR_STYPES;
	*vars= sizeof(_varstr) / sizeof(VarStruct);

}




/* ******************************************************************* */
/* ******************************************************************* */


float _result[8];

/* deze globals worden door blender gezet */

float cfra;

/* allereerste keer */

void plugin_seq_init()
{
	
}

/* dit is een cross!!! */

void plugin_seq_doito(Cast *cast, float facf0, float facf1, int x, int y, struct ImBuf *ibuf1, struct ImBuf *ibuf2, struct ImBuf *out, struct ImBuf *use)
{
	int fac1, fac2, fac3, fac4;
	int xo;
	char *rt1, *rt2, *rt;

	/* <out> = use <use> to <rect1> effect <rect2> 
	 * facf0= factor for field 0
	 * facf1= factor for field 1
	 */
	
	xo= x;
	rt1= (char *)ibuf1->rect;
	rt2= (char *)ibuf2->rect;
	rt= (char *)out->rect;
	
	fac2= 256.0*facf0;
	fac1= 256-fac2;
	fac4= 256.0*facf1;
	fac3= 256-fac4;
	
	while(y--) {
			
		x= xo;
		while(x--) {
	
			rt[0]= (fac1*rt1[0] + fac2*rt2[0])>>8;
			rt[1]= (fac1*rt1[1] + fac2*rt2[1])>>8;
			rt[2]= (fac1*rt1[2] + fac2*rt2[2])>>8;
			rt[3]= (fac1*rt1[3] + fac2*rt2[3])>>8;
			
			rt1+= 4; rt2+= 4; rt+= 4;
		}
		
		if(y==0) break;
		y--;
		
		x= xo;
		while(x--) {
	
			rt[0]= (fac3*rt1[0] + fac4*rt2[0])>>8;
			rt[1]= (fac3*rt1[1] + fac4*rt2[1])>>8;
			rt[2]= (fac3*rt1[2] + fac4*rt2[2])>>8;
			rt[3]= (fac3*rt1[3] + fac4*rt2[3])>>8;
			
			rt1+= 4; rt2+= 4; rt+= 4;
		}
		
	}
}

/* use bepaalt de key tussen 1 en 2 */

void plugin_seq_doit(Cast *cast, float facf0, float facf1, int x, int y, ImBuf *ibuf1, ImBuf *ibuf2, ImBuf *out, ImBuf *use)
{
	int fac1, fac2, fac3, fac4;
	int xo, add;
	char *rt1, *rt2, *rt, *rus;
	
	addzbufImBuf(out);
	
	/* <out> = use <use> to <rect1> effect <rect2> 
	 * facf0= factor for field 0
	 * facf1= factor for field 1
	 */
	
	xo= x;
	rt1= (char *)ibuf1->rect;
	rt2= (char *)ibuf2->rect;
	rt= (char *)out->rect;
	rus= (char *)use->rect;
	
	/* buttons: */
	add= 256*cast->fac;
	
	while(y--) {
			
		x= xo;
		while(x--) {
			
			fac1= MAX3(rus[1]+add, rus[2]+add, rus[3]+add);
			if(fac1>255) fac1= 255;
			fac2= 256-fac1;
			
			rt[0]= (fac1*rt1[0] + fac2*rt2[0])>>8;
			rt[1]= (fac1*rt1[1] + fac2*rt2[1])>>8;
			rt[2]= (fac1*rt1[2] + fac2*rt2[2])>>8;
			rt[3]= (fac1*rt1[3] + fac2*rt2[3])>>8;
			
			rt1+= 4; rt2+= 4; rt+= 4; rus+= 4;
		}		
	}
	
}


