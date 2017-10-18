#include <GL/glut.h>
#include "glutint.h"

extern Display *__glutDisplay;
extern int __glutScreen;
extern GLUTwindow *__glutCurrentWindow;

void glutDrawInit(void)
{
}

static void sys_coord(int *x, int *y) 
{
	*y= __glutCurrentWindow->height - *y;
	
/* 	*y= *y - __glutCurrentWindow->y; */
}

void glutDrawLine(int x1, int y1, int x2, int y2)
{
	sys_coord(&x1, &y1);
	sys_coord(&x2, &y2);
	
	MoveToEx(__glutCurrentWindow->hdc, x1, y1, NULL);
	LineTo(__glutCurrentWindow->hdc, x2, y2);
}

void glutDrawRect(int x1, int y1, int x2, int y2)
{
	sys_coord(&x1, &y1);
	sys_coord(&x2, &y2);

	MoveToEx(__glutCurrentWindow->hdc, x1, y1, NULL);
	LineTo(__glutCurrentWindow->hdc, x1, y2);

	MoveToEx(__glutCurrentWindow->hdc, x1, y1, NULL);
	LineTo(__glutCurrentWindow->hdc, x2, y1);

	MoveToEx(__glutCurrentWindow->hdc, x2, y2, NULL);
	LineTo(__glutCurrentWindow->hdc, x1, y2);

	MoveToEx(__glutCurrentWindow->hdc, x2, y2, NULL);
	LineTo(__glutCurrentWindow->hdc, x2, y1);
}

void glutDrawRectFilled(int x1, int y1, int x2, int y2)
{
	int y;

	sys_coord(&x1, &y1);
	sys_coord(&x2, &y2);

	for(y=y2; y<y1; y++) {
		MoveToEx(__glutCurrentWindow->hdc, x1, y, NULL);
		LineTo(__glutCurrentWindow->hdc, x2, y);
	}
}

static void __glutDrawSetColor(unsigned char r, unsigned char g, unsigned char b)
{
	HPEN newpen;

	newpen= CreatePen (PS_SOLID, 1, RGB(r, g, b));
	if (newpen != NULL) {
		newpen= SelectObject(GetDC(__glutCurrentWindow->win), newpen);
		DeleteObject (newpen);
	}
	
	return;
}

void glutDrawSetColor(int col)
{
	if(col==GLUTCOLWHITE) __glutDrawSetColor(240, 240, 240);
	else if(col==GLUTCOLDGREY) __glutDrawSetColor(80, 80, 80);
	else if(col==GLUTCOLLGREY) __glutDrawSetColor(210, 210, 210);
	else if(col==GLUTCOLGREY) __glutDrawSetColor(160, 160, 160);
	else if(col==GLUTCOLBLACK) __glutDrawSetColor(0, 0, 0);
	else printf ("ERROR: Bad color code passed to DrawSetColor\n");
}

void glutDrawUpdate(void)
{

}
