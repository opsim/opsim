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

/* plugintex.c  		MIXED MODEL

 * 
 * dec 95
 * 
 */

#include "/usr/people/blend/plugin.h"
#include <math.h>
/*
 * 

cc -g -float -c plugintex.c ; ld plugintex.o -o plugintex.so -lc -lm ; cp plugintex.so /pics/textures/proc/

                ^^^^^^^^^        ^^^^^^^^^      ^^^^^^^^^                 ^^^^^^^^^
NIEUWE DEFINITIE: verander alleen maar de waarde van de variabele PLUG
Je kunt vanuit je plugin alle libraries (ook de imbuf) gewoon aanroepen.

set PLUG=plugintex ; cc -g -float -mips1 -c $PLUG.c ; ld -shared $PLUG.o -o $PLUG.so ; cp $PLUG.so /pics/textures/proc/

         ^^^^^^^^^
 * 
 * 
 */

/* ******************************************************************* */
/* 				ZELF INVULLEN: struct- en funktienamen niet wijzigen							   */
/* ******************************************************************* */

/* 1. naam: */

	char _name[24]= "Tiles";


/* 2. aantal sub types */

	#define NR_STYPES	4

/* 3. namen van subtypes (maximaal 16 bytes per naam) */

	char _stnames[NR_STYPES][16]= {"tiles", "blocks", "stripes", "dots"};


/* 4. informatie over externe variabelen */

	VarStruct _varstr[]= {
	 /* butcode,	naam,       default,min, max */
		NUM|INT,	"limit",	5.0,	0.0, 255.0, 
		NUM|FLO,	"size",		0.5,	0.0, 2.0, 
		NUMSLI|FLO, "R ",		0.5,	0.0, 10.0, 	
		NUMSLI|FLO, "G ",		0.5,	0.0, 10.0, 	
		NUMSLI|FLO, "B ",		0.5,	0.0, 10.0,
	};

/* 5. hulpstruct om variabelen te casten */

	typedef struct Cast {
		int limit;
		float size, r, g, b;
	} Cast;


/* deze funktie laten staan! */

void plugin_tex_getinfo( int *stypes, int *vars)
{

	*stypes= NR_STYPES;
	*vars= sizeof(_varstr) / sizeof(VarStruct);

}




/* ******************************************************************* */
/* ******************************************************************* */


/* het resultaat van de texture is Tin,Tr,Tg,Tb,Ta, nor[0],nor[1],nor[2]  */

float _result[8];

/* deze globals worden door blender gezet */

float cfra;

/* allereerste keer */

void plugin_tex_init()
{
	
}


/* return 0: eenkanaals tex, 
   return 1: RGB texture
   return 2: normalen texture */

int plugin_tex_doit(int stype, Cast *cast, float *texvec, float *dxt, float *dyt)
{
	float fac;
	int x, y, z, tt, mul;
	
	mul= 128*cast->size;
	
	if(stype==0 || stype==1) {
		
		x= ffloor(mul*texvec[0]);
		y= ffloor(mul*texvec[1]);
		z= ffloor(mul*texvec[2]);
		
		x &= 255;
		y &= 255;
		z &= 255;

		tt= 0;
		if(x>cast->limit) tt++;
		if(y>cast->limit) tt++;
		if(z>cast->limit) tt++;
		
		if(stype==0) {
			if(tt & 1) _result[0]= 0.0;
			else _result[0]= 1.0;
		}
		else {
			if(tt) _result[0]= 0.0;
			else _result[0]= 1.0;
		}

		return 0;
	}
	else if(stype==2) {
		x= ffloor(mul*texvec[0]);
		x &= 255;

		if(x>cast->limit) _result[0]= 0.0;
		else _result[0]= 1.0;
		
		return 0;
	}
	else if(stype==3) {
		
		fac= 0.0;

			
		for(x=0; x<cast->limit; x++) {

			fac= 0.5+cast->size*fsin( (fsin(texvec[0]*cast->r +4.0*fac)+fsin(texvec[1]*cast->g +4.0*fac)+fsin(texvec[2]*cast->b +4.0*fac)));
		
		}
		_result[0]= fac;
		
		return 0;
	}

	return 1;
}

