/*
 * Author:  Marc van Kempen (marc@bowtie.nl) 
 * Desc:    Font api for glut, depends on platform
 *	    specific font drawing routines, to speed
 *	    things up a bit.
 *
 * (C) 1998 NeoGeo B.V. 
 * All rights reserved
 */

#include <glut.h>
#include "glutint.h"

#define MAXCOLVAL 65535

/***********************************************************************
 *
 * Static font and color information
 *
 * For X11 define these fonts, for other platforms
 * similar arrays can be defined 
 *
 ***********************************************************************/

static char *_blender_fonts[] = {
    "fixed",
    "-adobe-helvetica-medium-r-*--12-*-*-*-*-*-*-*",
    "-adobe-helvetica-medium-r-*--14-*-*-*-*-*-*-*",
    "-adobe-times-medium-r-*--12-*-*-*-*-*-*-*",
    "-adobe-times-medium-r-*--14-*-*-*-*-*-*-*"
};
static GC _gc;
static Drawable _draw;	
static Visual *_visual = NULL;
static Colormap _cmap;
static XFontStruct *_xf_struct = NULL;
static int _initialized = False;

/***********************************************************************
 *
 * Visual stuff
 *
 ***********************************************************************/

static int
__glutMiscInit(void)
/*
 * Desc: initialize the best visual we can get
 *	 set defaults for the font, foreground and 
 *	 function.
 * Post: return 0 on error, 1 on success
 */
{
    XVisualInfo vis_temp, *vis_info;
    int nvis, found, i;
    int depths[] = { 8, 15, 16, 24, 32 };
    int n = sizeof(depths);

    vis_temp.screen = DefaultScreen(__glutDisplay);
    found = False;
    for (i=0; i<n && !found; i++) {
	vis_temp.depth = depths[i];
	vis_info = XGetVisualInfo(__glutDisplay, VisualDepthMask,
				  &vis_temp, &nvis);
	if (nvis > 0) {
	    /* we found a matching visual */
	    found = True;
	}
    }
    if (found) {
	/* now try to create a colormap for this visual */
	/* if it's an 8 bit display, don't bother with
	   colors, just black and white will do fine */
	if (vis_temp.depth == 8) {
	    _visual = DefaultVisual(__glutDisplay, __glutScreen);
	    _cmap = DefaultColormap(__glutDisplay, __glutScreen);
	} else {
	    _visual = vis_info[0].visual;
	    XFree(vis_info);
	    _cmap = XCreateColormap(__glutDisplay, __glutCurrentWindow->win,
				    _visual, AllocNone);
	}
    } else {
	/* no suitable visuals found */
	_visual = DefaultVisual(__glutDisplay, __glutScreen);
	_cmap = DefaultColormap(__glutDisplay, __glutScreen);
    }
    _gc = XCreateGC(__glutDisplay, __glutCurrentWindow->win, 0, NULL);
    XSetForeground(__glutDisplay, _gc, BlackPixel(__glutDisplay, __glutScreen));
    XSetFunction(__glutDisplay, _gc, GXinvert);
    _initialized = True;

	/* dirty patch: no window switch allowed! */
	_draw= __glutCurrentWindow->win;
    return(1);
} /* glutMiscInit() */

/***********************************************************************
 *
 * Api calls 
 *
 ***********************************************************************/

int APIENTRY
glutFontColor(float r, float g, float b)
/*
 * Desc: set the foreground color of the font
 * Pre:  0.0<=r<=1.0 && 0.0<=g<=1.0 && 0.0<=b<=1.0
 * Post: return 0 on error, 1 on success
 */
{
    int i;
    XColor xcol;

    if (!_initialized) {
	__glutMiscInit();
    }

    if (r<0.0 || r>1.0 || g<0.0 || g>1.0 || b<0.0 || b>1.0) {
	return(0);
    }

    /* set color, does this also work on 8bit, 16bit displays? */
    xcol.flags = DoRed | DoGreen | DoBlue;
    xcol.red = (int) r * MAXCOLVAL;
    xcol.green = (int) g * MAXCOLVAL;
    xcol.blue = (int) b * MAXCOLVAL;
    i = XAllocColor(__glutDisplay, _cmap, &xcol);
    if (!i) {
	return(0);
    }
    XSetForeground(__glutDisplay, _gc, xcol.pixel);

    return(1);
} /* glutFontColor() */

int APIENTRY
glutFontSet(int font)
/*
 * Desc: set the current font to <font>
 *	 return 0 on error, 1 on success
 */
{
    char **fonts;
    int n;

    if (!_initialized) {
	__glutMiscInit();
    }

    if (font < 0 || font > GLUT_FONT_MAX) {
	return(0);
    }

    /* if we have a font struct, free it first */
    if (_xf_struct) {
	XFreeFont(__glutDisplay, _xf_struct);
	_xf_struct = NULL;
    }

    /* look the font up */
    fonts = XListFonts(__glutDisplay, _blender_fonts[font], 10, &n);
    if (n==0) {
	_xf_struct = NULL;
	return(0);
    } else {
	_xf_struct = XLoadQueryFont(__glutDisplay, fonts[0]);
	if (_xf_struct == NULL) {
	    return(0);
	} else {
	    XSetFont(__glutDisplay, _gc, _xf_struct->fid);
	    return(1);
	}
    }
} /* glutFontSet() */

int APIENTRY
glutFontDraw(char *str, int x, int y)
/*
 * Desc: draw a text on the screen in a fixed font
 * Post: return 0 on error, 1 on success
 */
{
    if (!_initialized) {
	__glutMiscInit();
    }

    XDrawString(__glutDisplay, __glutCurrentWindow->win, 
		_gc, x, y, str, strlen(str));

    return(1);
} /* glutDrawText() */

int APIENTRY 
glutFontLen(char *str)
/*
 * Desc: return the length of the charachter string
 *	 in pixels when printed with the current font
 * Pre:	 Font has been initialized.
 * Post: return -1 on error, length of the string in pixels
 *	 on success
 */
{
    int size, w;
    char *p;
    int ch;

    if (!_initialized) {
	__glutMiscInit();
    }
    if (!_xf_struct) {
	return(-1);
    }
    p = str;
    size = 0;
    while (*p) {
	if (*p > _xf_struct->min_char_or_byte2 && 
	    *p < _xf_struct->max_char_or_byte2) {
		ch = *p - _xf_struct->min_char_or_byte2;
		w = _xf_struct->per_char[ch].width;
		size += w;
	} else {
		printf("character (%c) not in font?\n", *p);
	}
	p++;
    }
    return(size);
} /* glutFontLen() */

void APIENTRY
glutDrawInverted(int x0, int y0, int x1, int y1)
/*
 * Desc: draw a line with INVERT set, this can be
 *	 used to create a Rubber band effect
 */
{
    if (!_initialized) {
	__glutMiscInit();
    }

    XDrawLine(__glutDisplay, _draw, _gc, x0, y0, x1, y1);

    return;
} /* glutRubberBand() */

void APIENTRY
glutFlush(void)
/*
 * Desc: flush the drawing queue
 */
{
    XFlush(__glutDisplay);
    return;
} /* glutFlush() */
