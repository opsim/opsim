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


/* graphics.h    feb 98 */

/*
 * 
 * geschikt gemakt voor X en OpenGL
 * 
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "GL/gl.h"
#include "GL/glu.h"
#ifdef __BeOS
	#include "GL/glut.h"
#else
	#ifdef __WIN32
		#include <GL/glut.h>
	#else
		#include "glut.h"
	#endif
#endif

#ifndef __WIN32 /* Win32 handelt het ophalen van fonts anders af */

/* aanvulling op glut.h */
extern void *glutBitmapHelveticaBold8;
extern void *glutBitmapHelveticaBold10;
extern void *glutBitmapHelveticaBold12;

extern void *glutBitmapScreen12;
extern void *glutBitmapScreen15;

/* Bitmap font constants (use these in GLUT program). */
#define GLUT_BITMAP_HELVETICAB_8	(&glutBitmapHelveticaBold8)
#define GLUT_BITMAP_HELVETICAB_10	(&glutBitmapHelveticaBold10)
#define GLUT_BITMAP_HELVETICAB_12	(&glutBitmapHelveticaBold12)

#define GLUT_BITMAP_SCREEN_12	(&glutBitmapScreen12)
#define GLUT_BITMAP_SCREEN_15	(&glutBitmapScreen15)

#endif

#if !defined __BeOS && !defined __WIN32

/* patch voor struct Object */
#define _XtObject_h

#include <X11/Xlib.h>
#include <X11/StringDefs.h>
#include <X11/keysym.h>  

#include <GL/glx.h> 

#endif


#include "objfnt.h"

#include "Button.h"
#include "screen.h"
#include "mydevice.h"

#define LINE2S(v1, v2)	{glBegin(GL_LINES); glVertex2sv((short *)(v1)); glVertex2sv((short *)(v2)); glEnd();}
#define LINE3S(v1, v2)	{glBegin(GL_LINES); glVertex3sv((short *)(v1)); glVertex3sv((short *)(v2)); glEnd();}
#define LINE2I(v1, v2)	{glBegin(GL_LINES); glVertex2iv((int *)(v1)); glVertex2iv((int *)(v2)); glEnd();}
#define LINE3I(v1, v2)	{glBegin(GL_LINES); glVertex3iv((int *)(v1)); glVertex3iv((int *)(v2)); glEnd();}
#define LINE2F(v1, v2)	{glBegin(GL_LINES); glVertex2fv((float *)(v1)); glVertex2fv((float *)(v2)); glEnd();}
#define LINE3F(v1, v2)	{glBegin(GL_LINES); glVertex3fv((float *)(v1)); glVertex3fv((float *)(v2)); glEnd();}

#define IRIS 1
#define ELAN 2
#define ENTRY 3
#undef VIDEO
#define VIDEO 4
#define M_O2 5

/* used for glSelect */
typedef GLuint	IGLuint;
#define SELBUFFER_INDEX(a)	(4*a+3)

extern short getcursorN();	/* usiblender.c */

extern void sdrawline(short x1, short y1, short x2, short y2); /* Button.c */
extern void fdrawline(float x1, float y1, float x2, float y2);


/* ****************** OGL ******************************* */
/* voor OpenGl compatibility */

typedef short Device;
#define NORMALDRAW		0
#define PUPDRAW			1
#define OVERDRAW		2

#define CURSOR_VPAINT		GLUT_CURSOR_LEFT_ARROW
#define CURSOR_FACESEL		GLUT_CURSOR_RIGHT_ARROW
#define CURSOR_WAIT			GLUT_CURSOR_WAIT
#define CURSOR_EDIT			GLUT_CURSOR_CROSSHAIR
#define CURSOR_STD			GLUT_CURSOR_INHERIT

/* Mij afspraak is cpack een getal dat als 0xFFaa66 of zo kan worden uitgedrukt. Is dus gevoelig voor endian. 
 * Met deze define wordt het altijd goed afgebeeld
 */
#define cpack(x)	glColor3ub( ((x)&0xFF), (((x)>>8)&0xFF), (((x)>>16)&0xFF) )

#define glMultMatrixf(x)		glMultMatrixf( (float *)(x))
#define glLoadMatrixf(x)		glLoadMatrixf( (float *)(x))

#define lrectwrite(a, b, c, d, rect)	{glRasterPos2i(a,  b);glDrawPixels((c)-(a)+1, (d)-(b)+1, GL_RGBA, GL_UNSIGNED_BYTE,  rect);}

	/* TBOXX: breedte in pixels */
#define TBOXXL  80
#define TBOXXR  170
#define TBOXX	(TBOXXL+TBOXXR)
	/* TBOXEL: aantal elementen onder elkaar */
#define TBOXEL	14
	/* TBOXH: hoogte van 1 element */
#define TBOXH	20
#define TBOXY	TBOXH*TBOXEL

#define TBOXBLACK	1
#define TBOXDGREY	2
#define TBOXWHITE	3
#define TBOXGREY	4
#define TBOXLGREY	5


	/* view.c */
extern        int boundbox_clip(float obmat[][4], BoundBox *bb);
extern        void calctrackballvec(rcti *area, short *mval, float *vec);
extern        void calctrackballvecfirst(rcti *area, short *mval, float *vec);
extern        void centreview();	
extern        void endlocalview(ScrArea *sa);
extern        float *give_cursor(void);
extern        void initgrabz(float x, float y, float z);
extern        void initlocalview();
extern        void obmat_to_viewmat(Object *ob);
extern        void persp(int a);
extern        void persp_general(int a);
extern        void project_short(float *vec, short *adr);	
extern        void project_short_noclip(float *vec, short *adr);
extern        void restore_localviewdata(View3D *vd);
extern        short selectprojektie(IGLuint *buffer, short x1, short y1, short x2, short y2);
extern        void setviewmatrixview3d();
extern        void setwinmatrixview3d(rctf *rect);		
extern        void view3d_home(int centre);
extern        void viewmove(int mode);
extern        void window_to_3d(float *vec, short mx, short my);





#endif /* GRAPHICS_H */

