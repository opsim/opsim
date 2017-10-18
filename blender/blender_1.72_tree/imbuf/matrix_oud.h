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

/* deze matrices steken onlogies in elkaar */



float rgbyuv[]={ .299 , .587 , .114,		/* y */
		-.147 ,-.289 , .437,				/* u */
		 .615 ,-.515 ,-.100,				/* v */
		  0 ,  128   , 128 };

float rgbbetaoud[]={
		 .667 ,-.558 ,-.108,				/* r-y	-> r */
		 .299 , .587 , .114,				/* y		-> g */
		-.225 ,-.442 , .667,				/* b-y	-> b */
		  128 ,  0   , 128 };

float rgbbeta_te_veel_kleur[]={
		 .667 ,-.442 ,-.225,				/* b-y 	-> b */
		 .114 , .587 , .299,				/* y 		-> g */
		-.108 ,-.558 , .667,				/* r-y	-> r */
		  128 ,  0   , 128 };

float rgbbeta[]={					/* afgeleid uit videoframer = Y Cr Cb in kopieen van Francois*/
		 .500,	-.331,	-.169,				/* b-y 	-> b */
		 .114,	 .587,	 .299,				/* y 		-> g */
		-.082,	-.418,	 .500,				/* r-y	-> r */
		  128.0,	0.0,		128.0 };

float rgb_to_bw[]={
		 .114,	 .587,	 .299,
		 .114,	 .587,	 .299,
		 .114,	 .587,	 .299,
		0.0,		0.0,		0.0 };

float rgbbeta_gokje[]={
		 .75 * .437 , .75 * -.289 , .75 * -.147,				/* b-y 	-> b */
		 .114 , .587 , .299,				/* y 		-> g */
		.60 * -.100 , .60 * -.515 ,.60 * .615,				/* r-y	-> r */
		  128 ,  0   , 128 };
/*

float dyuvrgb[]={  1.0 ,-0.337 , -.698,
		   1.0 , 1.733 , 0.0  ,
		   1.0 , 0.0   , 1.371,
		132.47 ,-221.8 ,-175.5};
*/

float dyuvrgb[]={  1.0 , 1.733 , 0.0  ,
		   1.0 ,-0.337 , -.698,
		   1.0 , 0.0   , 1.371,
		-221.8 ,132.47 ,-175.5};

