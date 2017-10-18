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

/* floatpatch.h    dec 98

 * 
 * 
 */

#ifndef FLOATPATCH_H
#define FLOATPATCH_H

/* for 64 bits systemen en Sun */

#ifdef MIPS1
#else

/* #if defined(__FreeBSD__) || defined(linux) || defined(__SUN) */
#define fabsf(a)	fabs((double)(a))

#define facos		acosf
#define acosf(a)	acos((double)(a))

#define fasin		asinf
#define asinf(a)	asin((double)(a))

#define fatan		atanf
#define atanf(a)	atan((double)(a))

#define fatan2			atan2f
#define atan2f(a, b)	atan2((double)(a), (double)(b))

#define fmodf(a, b)		fmod((double)(a), (double)(b))

#define fcos		cosf
#define cosf(a)		cos((double)(a))

#define fsin		sinf
#define sinf(a)		sin((double)(a))

#define ftan		tanf
#define tanf(a)		tan((double)(a))

#define fexp		expf
#define expf(a)		exp((double)(a))

#define flog		logf
#define logf(a)		log((double)(a))

#define flog10		log10f
#define log10f(a)	log10((double)(a))

#define fsqrt		sqrtf
#define sqrtf(a)	sqrt((double)(a))

#define fceil		ceilf
#define ceilf(a)	ceil((double)(a))

#define ffloor		floorf
#define floorf(a)	floor((double)(a))

#define fpow		powf
#define powf(a, b)	pow((double)(a), (double)(b))

/* #endif  */

#endif

#endif /* FLOATPATCH_H */


