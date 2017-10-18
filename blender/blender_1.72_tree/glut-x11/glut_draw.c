#include <X11/Xlib.h>
#include <GL/glx.h>

#include <glut.h>
#include "glutint.h"

extern Display *__glutDisplay;
extern int __glutScreen;
extern GLUTwindow *__glutCurrentWindow;

static XImage *ximage=0;
static GLXDrawable xdraw=0;	/* wordt maar 1 keer gedaan, voor mainwin */
static GC curGC, blackGC, grayGC, whiteGC, darkGC, lightGC;

void glutDrawInit(void)
{
	XGCValues gcvals;
	int dep;

	gcvals.font= 0;
	xdraw= glXGetCurrentDrawable();
	
	dep= glutGet(GLUT_WINDOW_BUFFER_SIZE);
	if(dep<12) printf("Error: screen depth (%d) should be at least 15.\n", dep);
	
	gcvals.foreground = 0;
	blackGC = XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	
#ifdef __sgi
	if(dep==15 || dep==16) {
#else
	if(dep==15) {
#endif
		gcvals.foreground=  (10 + (10<<5) + (10<<10) ) ;
		darkGC= XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	
		gcvals.foreground= (15 + (15<<5) + (15<<10) );
		grayGC= XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	
		gcvals.foreground= (23 + (23<<5) + (23<<10) );
		lightGC= XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	
		gcvals.foreground= (31 + (31<<5) + (31<<10) );
		whiteGC= XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	}
	else {
		gcvals.foreground= (dep==16? (10 + (10<<6) + (10<<11)) : 0x555555 );
		darkGC= XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	
		gcvals.foreground= (dep==16? (15 + (15<<6) + (15<<11)) : 0x999999 );
		grayGC= XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
		
		gcvals.foreground= (dep==16? (23 + (23<<6) + (23<<11)) : 0xcccccc );
		lightGC= XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	
		gcvals.foreground= (dep==16? (31 + (31<<6) + (31<<11)) : 0xffffff );
		whiteGC= XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	}
	curGC= blackGC;	
}

void glutDrawLine(int x1, int y1, int x2, int y2)
{
	/* ander coordinatensysteem! */
	y1= (__glutCurrentWindow->height-y1);
	y2= (__glutCurrentWindow->height-y2);

	XDrawLine(__glutDisplay, xdraw, curGC, x1, y1, x2, y2);
}

void glutDrawRect(int x1, int y1, int x2, int y2)
{
	int sy;
	
	/* ander coordinatensysteem! */
	sy= y2-y1;
	y1= (__glutCurrentWindow->height-y1-sy);

	XDrawRectangle(__glutDisplay, xdraw, curGC, x1, y1 , x2-x1, sy);
}

void glutDrawRectFilled(int x1, int y1, int x2, int y2)
{
	int sy;
	
	/* ander coordinatensysteem! */
	sy= y2-y1;
	y1= (__glutCurrentWindow->height-y1-sy);

	XFillRectangle(__glutDisplay, xdraw, curGC, x1, y1 , x2-x1, sy);		
}

void glutDrawSetColor(int col)
{
	if(col==GLUTCOLWHITE) curGC= whiteGC;
	else if(col==GLUTCOLDGREY) curGC= darkGC;
	else if(col==GLUTCOLLGREY) curGC= lightGC;
	else if(col==GLUTCOLGREY) curGC= grayGC;
	else if(col==GLUTCOLBLACK) curGC= blackGC;	
	else printf ("ERROR: Bad color code passed to DrawSetColor\n");
}

void glutDrawUpdate(void)
{
	XFlush(__glutDisplay);
	XSync(__glutDisplay, 0);
}

void glutCustomCursor(char *data1, char *data2, int size)
{
	Pixmap source, mask;
	Cursor cursor;
	XColor fg, bg;
	
	if(XAllocNamedColor(__glutDisplay, DefaultColormap(__glutDisplay, __glutScreen),
		"White", &fg, &fg) == 0) return;
	if(XAllocNamedColor(__glutDisplay, DefaultColormap(__glutDisplay, __glutScreen),
		"Red", &bg, &bg) == 0) return;


	source= XCreateBitmapFromData(__glutDisplay,  xdraw, data2, size, size);
	mask= XCreateBitmapFromData(__glutDisplay, xdraw, data1, size, size);
		
	cursor= XCreatePixmapCursor(__glutDisplay, source, mask, &fg, &bg, 7, 7);
		
	XFreePixmap(__glutDisplay, source);
	XFreePixmap(__glutDisplay, mask);
		
	XDefineCursor(__glutDisplay, xdraw, cursor);
}
