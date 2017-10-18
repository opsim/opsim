/*
 * Author: Marc van Kempen (marc@bowtie.nl)
 * Desc:   Support functions for Blender
 */

#include <GL/glut.h>
#include "glutint.h"
#include "glut_blender.h"

int 
glutGetDepth(void)
{
	return GetDeviceCaps(__glutCurrentWindow->hdc, BITSPIXEL);
}

int 
glutGetMouseButton(void)
{
	int button;

	if (HIBYTE(GetAsyncKeyState(VK_LBUTTON))) {
		button = 1;
	} else
	if (HIBYTE(GetAsyncKeyState(VK_MBUTTON))) {
		button = 4;
	} else
	if (HIBYTE(GetAsyncKeyState(VK_RBUTTON))) {
		button = 2;
	} else {
		button = 0;
	}
	if (GetInputState()) {
		processEventsAndTimeouts();
	}

	return (button);
}

void 
glutGetMouse(short *mval)
{
	POINT pt;

	if (!GetCursorPos(&pt)) {
		printf ("GetCursorPos failed\n");
	} else {
		ScreenToClient(__glutCurrentWindow->win, &pt);
		mval[0] = pt.x;
		mval[1] = pt.y;
		/* Same trick as in win32_util.c */
		if (mval[0] & 1 << 15) mval[0] -= (1 << 16);
		if (mval[1] & 1 << 15) mval[1] -= (1 << 16);
	}

	return;
}

struct _hbuf{
	int x, y, sx, sy;
	HDC hDC, hMemDC;
	HBITMAP hBitmap;
} hbuf;

void 
glutPutFrontBuffer(void)
{
	BitBlt (hbuf.hDC, hbuf.x, hbuf.y-hbuf.sy, hbuf.sx, hbuf.sy, hbuf.hMemDC, 0, 0, SRCCOPY);

	ReleaseDC (__glutCurrentWindow->win, hbuf.hDC);
	DeleteDC (hbuf.hMemDC);
	DeleteObject (hbuf.hBitmap);
}

void 
glutGetFrontBuffer(int x, int y, int sx, int sy)
{
	hbuf.x = x; hbuf.y = y;
	hbuf.sx = sx; hbuf.sy = sy;

	hbuf.hDC = GetDC (__glutCurrentWindow->win);
	hbuf.hMemDC = CreateCompatibleDC (hbuf.hDC);
	hbuf.hBitmap = CreateCompatibleBitmap(hbuf.hDC, hbuf.sx, hbuf.sy);
	
	SelectObject (hbuf.hMemDC, hbuf.hBitmap);

	BitBlt (hbuf.hMemDC, 0, 0, hbuf.sx, hbuf.sy, hbuf.hDC, hbuf.x, hbuf.y-hbuf.sy, SRCCOPY);

	return;
}

extern HCURSOR cust_curs;

void 
glutCustomCursor(char *data1, char *data2, int size)
{
	int andb[32], xorb[32]; /* 32x32 bit rects */
	int i;
	
	if(GetSystemMetrics(SM_CXCURSOR)!=32 || GetSystemMetrics(SM_CXCURSOR)!=32)
		return;

	/* Set the background to be transparent for the parts we don't overwrite */
	memset(&andb, 255, 32*4);
	memset(&xorb, 0, 32*4);
	
	for(i=0;i<16;i++) {
		memcpy(&(xorb[i]), &(data2[i*2]), 2);
		memset(&(andb[i]), 0, 2);
	}

	if (cust_curs) DestroyCursor(cust_curs);
	cust_curs= CreateCursor(GetModuleHandle(NULL), 0, 0, 32, 32, andb, xorb);

	SetCursor(cust_curs);
}

void 
glutInvertLine(int a, int b, int c, int d)
{
	int old_rop2;
	HDC hdc;

	hdc = __glutCurrentWindow->hdc;
	old_rop2 = GetROP2(hdc);
	SetROP2(hdc, R2_NOT);
	glutDrawLine(a, b, c, d);
	SetROP2(hdc, old_rop2);
	
	return;
}

int 
glutqtest(void)
{
	MSG msg;

	return(PeekMessage(&msg, __glutCurrentWindow->win, 0, 0, PM_NOREMOVE));
}

int
glutGetQual(void)
/*
 * Desc: return SHIFT, CONTROL and ALT (in windows lingo: VK_MENU)
 */
{
	int mask;

	mask = 0;
	if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
		mask |= GLUT_ACTIVE_SHIFT;
	}
	if (HIBYTE(GetAsyncKeyState(VK_CONTROL))) {
		mask |= GLUT_ACTIVE_CTRL;
	}
	if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
		mask |= GLUT_ACTIVE_ALT;
	}
	return(mask);
}

/* ************************** DIRTY!  std gluttimer doesn't work ******************* */

int glutTestEsc()
{
	int retval;
	
	retval= HIBYTE(GetAsyncKeyState(VK_ESCAPE));
	
	return retval;
}

