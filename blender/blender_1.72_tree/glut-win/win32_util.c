
/* Copyright (c) Nate Robins, 1997. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */


#include "glutint.h"
#include "glutstroke.h"
#include "glutbitmap.h"
#if !defined(__CYGWIN32__)
#include <sys/timeb.h>
#endif

#include "glut_blender.h"

extern BitmapFontRec glutBitmapHelvetica10, glutBitmapHelveticaBold8, glutBitmapHelveticaBold10, glutBitmapHelveticaBold12;
extern BitmapFontRec glutBitmapScreen12, glutBitmapScreen15;

static int __win32_translate_key(int wParam, int lParam);

#ifndef __CYGWIN__
int
gettimeofday(struct timeval* tp, void* tzp)
{
	struct timeb tb;

	ftime(&tb);
	tp->tv_sec = tb.time;
	tp->tv_usec = tb.millitm * 1000;

	/* 0 indicates that the call succeeded. */
	return 0;
}
#endif

void*
__glutFont(void *font)
{
	switch((int)font) {
	case (int)GLUT_BITMAP_HELVETICA_10:
		return &glutBitmapHelvetica10;
	case (int)GLUT_BITMAP_HELVETICAB_8:
		return &glutBitmapHelveticaBold8;
	case (int)GLUT_BITMAP_HELVETICAB_10:
		return &glutBitmapHelveticaBold10;
	case (int)GLUT_BITMAP_HELVETICAB_12:
		return &glutBitmapHelveticaBold12;
	case (int)GLUT_BITMAP_SCREEN_12:
		return &glutBitmapScreen12;
	case (int)GLUT_BITMAP_SCREEN_15:
		return &glutBitmapScreen15;
	default:
		return NULL;
	}
}

int
__glutGetTransparentPixel(Display * dpy, XVisualInfo * vinfo)
{
	/* the transparent pixel on Win32 is always index number 0.  So if
     we put this routine in this file, we can avoid compiling the
     whole of layerutil.c which is where this routine normally comes
     from. */
	return 0;
}

void
__glutAdjustCoords(Window parent, int* x, int* y, int* width, int* height)
{
	RECT rect;

	/* adjust the window rectangle because Win32 thinks that the x, y,
     width & height are the WHOLE window (including decorations),
     whereas GLUT treats the x, y, width & height as only the CLIENT
     area of the window. */
	rect.left = *x; 
	rect.top = *y;
	rect.right = *x + *width; 
	rect.bottom = *y + *height;

	/* must adjust the coordinates according to the correct style
     because depending on the style, there may or may not be
     borders. */
	AdjustWindowRect(&rect, WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
	    (parent ? WS_CHILD : WS_OVERLAPPEDWINDOW),
	    FALSE);
	/* FALSE in the third parameter = window has no menu bar */

	/* readjust if the x and y are offscreen */
	if(rect.left < 0)
		*x = 0;
	else
		*x = rect.left;

	if(rect.top < 0)
		*y = 0;
	else
		*y = rect.top;

	*width = rect.right - rect.left;	/* adjusted width */
	*height = rect.bottom - rect.top;	/* adjusted height */
}

extern unsigned __glutMenuButton;
extern GLUTidleCB __glutIdleFunc;
extern GLUTtimer *__glutTimerList;
extern void handleTimeouts(void);
extern GLUTmenuItem *__glutGetUniqueMenuItem(GLUTmenu * menu, int unique);
static HMENU __glutHMenu;
static int active;
HCURSOR cust_curs=NULL;

LONG WINAPI __glutWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINT         point;			/* Point structure. */
	PAINTSTRUCT   ps;			/* Paint structure. */
	LPMINMAXINFO  minmax;			/* Minimum/maximum info structure. */
	LPWINDOWPOS   winpos;
	GLUTwindow*   window;			/* GLUT window associated with message. */
	GLUTmenu*     menu;			/* GLUT menu associated with message. */
	int x, y, width, height, key;
	int button = -1;

	if ((window=__glutGetWindow(hwnd))) {
		if(active != window->num) {
			if(window && window->entry) {
				window->entry(1);
			}
		}
		active= window->num;
	}

	switch(msg) {
	case WM_CREATE:
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_DESTROY:
		return 0;

	case WM_PAINT:
		window = __glutGetWindow(hwnd);
		if (window) {
			BeginPaint(hwnd, &ps);		/* Must have this for some Win32 reason. */
#if TAKE_THIS_OUT
			if (window->colormap) {
				SelectPalette(window->hdc, window->colormap->cmap, FORCE_FOREGROUND);
				RealizePalette(window->hdc);	       /* Remap the custom palette. */
			}
#endif
			EndPaint(hwnd, &ps);
			if (window->win == hwnd) {
				__glutPostRedisplay(window, GLUT_REPAIR_WORK);
			} else if (window->overlay && window->overlay->win == hwnd) {
				__glutPostRedisplay(window, GLUT_OVERLAY_REPAIR_WORK);
			}
		}
		return 0;

	case WM_SYSCHAR:
	case WM_CHAR:	
		window = __glutGetWindow(hwnd);
		if (!window) {

		}
		/* Win32 is dumb and sends these messages only to the parent
       window.  Therefore, find out if we're in a child window and
       call the child windows keyboard callback if we are. */
		if (window->parent) {
			GetCursorPos(&point);
			ScreenToClient(hwnd, &point);
			hwnd = ChildWindowFromPoint(hwnd, point);
			window = __glutGetWindow(hwnd);
		}
		if (window->keyboard) {
			GetCursorPos(&point);
			ScreenToClient(window->win, &point);
			__glutSetWindow(window);
			__glutModifierMask = 0;
			if (GetKeyState(VK_SHIFT) < 0)	/* < 0 = high order bit is on */
				__glutModifierMask |= ShiftMask;
			if (GetKeyState(VK_CONTROL) < 0)
				__glutModifierMask |= ControlMask;
			if (GetKeyState(VK_MENU) < 0)
				__glutModifierMask |= Mod1Mask;
			window->keyboard((char)wParam, point.x, point.y);
			__glutModifierMask = (unsigned int) ~0;
		}
		return 0;

	case WM_SYSKEYUP:
	case WM_KEYUP:		
		window = __glutGetWindow(hwnd);

		/* Win32 is dumb and sends these messages only to the parent
       window.  Therefore, find out if we're in a child window and
       call the child windows keyboard callback if we are. */
		if (window->parent) {
			GetCursorPos(&point);
			ScreenToClient(hwnd, &point);
			hwnd = ChildWindowFromPoint(hwnd, point);
			window = __glutGetWindow(hwnd);
		}
		if (window->special) {
			key = __win32_translate_key(wParam, lParam);
			if (key < 0) {
				/* We don't support this key, let the default procedure
	   handle the event */
				goto defproc;
			}
			GetCursorPos(&point);
			ScreenToClient(window->win, &point);
			__glutSetWindow(window);
			__glutModifierMask = 0;
			if (GetKeyState(VK_SHIFT) < 0)	/* < 0 = high order bit is on */
				__glutModifierMask |= ShiftMask;
			if (GetKeyState(VK_CONTROL) < 0)
				__glutModifierMask |= ControlMask;
			if (GetKeyState(VK_MENU) < 0)
				__glutModifierMask |= Mod1Mask;
			window->special(key, 0, 0);
			__glutModifierMask = (unsigned int) ~0;
		}
		return 0;

	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		window = __glutGetWindow(hwnd);

		/* Win32 is dumb and sends these messages only to the parent
       window.  Therefore, find out if we're in a child window and
       call the child windows keyboard callback if we are. */
		if (window->parent) {
			GetCursorPos(&point);
			ScreenToClient(hwnd, &point);
			hwnd = ChildWindowFromPoint(hwnd, point);
			window = __glutGetWindow(hwnd);
		}
		if (window->special) {
			key = __win32_translate_key(wParam, lParam);
			if (key < 0) {
				/* We don't support this key, let the default procedure handle the event */
				goto defproc;
			}
			GetCursorPos(&point);
			ScreenToClient(window->win, &point);
			__glutSetWindow(window);
			__glutModifierMask = 0;
			if (GetKeyState(VK_SHIFT) < 0)	/* < 0 = high order bit is on */
				__glutModifierMask |= ShiftMask;
			if (GetKeyState(VK_CONTROL) < 0)
				__glutModifierMask |= ControlMask;
			if (GetKeyState(VK_MENU) < 0)
				__glutModifierMask |= Mod1Mask;
			window->special(key, 1, 0);
			__glutModifierMask = (unsigned int) ~0;
		}
		return 0;

	case WM_LBUTTONDOWN:
		button = GLUT_LEFT_BUTTON;
	case WM_MBUTTONDOWN:
		if (button < 0)
			button = GLUT_MIDDLE_BUTTON;
	case WM_RBUTTONDOWN:
		if (button < 0)
			button = GLUT_RIGHT_BUTTON;

		/* finish the menu if we get a button down message (user must have
       cancelled the menu). */
		if (__glutMappedMenu) {
			/* TODO: take this out once the menu on middle mouse stuff works
	 properly. */
			if (button == GLUT_MIDDLE_BUTTON)
				return 0;
			GetCursorPos(&point);
			ScreenToClient(hwnd, &point);
			__glutItemSelected = NULL;
			__glutFinishMenu(hwnd, point.x, point.y);
			return 0;
		}

		/* set the capture so we can get mouse events outside the window */
		SetCapture(hwnd);

		/* Win32 doesn't return the same numbers as X does when the mouse
       goes beyond the upper or left side of the window.  roll the
       Win32's 0..2^16 pointer co-ord range to 0 +/- 2^15. */
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		if(x & 1 << 15) x -= (1 << 16);
		if(y & 1 << 15) y -= (1 << 16);

		window = __glutGetWindow(hwnd);
		if (window) {
			menu = __glutGetMenuByNum(window->menu[button]);
			if (menu) {
				point.x = LOWORD(lParam); 
				point.y = HIWORD(lParam);
				ClientToScreen(window->win, &point);
				__glutMenuButton = button == GLUT_RIGHT_BUTTON ? TPM_RIGHTBUTTON :
				    button == GLUT_LEFT_BUTTON  ? TPM_LEFTBUTTON :
				    0x0001;
				__glutStartMenu(menu, window, point.x, point.y, x, y);
			} else if (window->mouse) {

				__glutSetWindow(window);
				__glutModifierMask = 0;
				if (GetKeyState(VK_SHIFT) < 0)	/* < 0 = high order bit is on. */
					__glutModifierMask |= ShiftMask;
				if (GetKeyState(VK_CONTROL) < 0)
					__glutModifierMask |= ControlMask;
				if (GetKeyState(VK_MENU) < 0)
					__glutModifierMask |= Mod1Mask;
				window->mouse(button, GLUT_DOWN, x, y);
				__glutModifierMask = (unsigned int)~0;
			} else {
				/* Stray mouse events.  Ignore. */
			}
		}
		return 0;

	case WM_LBUTTONUP:
		button = GLUT_LEFT_BUTTON;
	case WM_MBUTTONUP:
		if (button < 0)
			button = GLUT_MIDDLE_BUTTON;
	case WM_RBUTTONUP:
		if (button < 0)
			button = GLUT_RIGHT_BUTTON;

		/* Bail out if we're processing a menu. */
		if (__glutMappedMenu) {
			GetCursorPos(&point);
			ScreenToClient(hwnd, &point);
			/* if we're getting the middle button up signal, then something
	 on the menu was selected. */
			if (button == GLUT_MIDDLE_BUTTON) {
				return 0;
				/* For some reason, the code below always returns -1 even
	   though the point IS IN THE ITEM!  Therefore, just bail out if
	   we get a middle mouse up.  The user must select using the
	   left mouse button.  Stupid Win32. */
#if 0
				int item = MenuItemFromPoint(hwnd, __glutHMenu, point);
				printf("item = %d %d %d\n", item, point.x, point.y);
				if (item != -1)
					__glutItemSelected = (GLUTmenuItem*)GetMenuItemID(__glutHMenu, item);
				else
					__glutItemSelected = NULL;
				__glutFinishMenu(hwnd, point.x, point.y);
#endif
			} else {
				__glutItemSelected = NULL;
				__glutFinishMenu(hwnd, point.x, point.y);
			}
			return 0;
		}

		/* Release the mouse capture. */
		ReleaseCapture();

		window = __glutGetWindow(hwnd);
		if (window && window->mouse) {
			/* Win32 doesn't return the same numbers as X does when the
	 mouse goes beyond the upper or left side of the window.  roll
	 the Win32's 0..2^16 pointer co-ord range to 0 +/- 2^15. */
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			if(x & 1 << 15) x -= (1 << 16);
			if(y & 1 << 15) y -= (1 << 16);

			__glutSetWindow(window);
			__glutModifierMask = 0;
			if (GetKeyState(VK_SHIFT) < 0)	/* < 0 = high order bit is on */
				__glutModifierMask |= ShiftMask;
			if (GetKeyState(VK_CONTROL) < 0)
				__glutModifierMask |= ControlMask;
			if (GetKeyState(VK_MENU) < 0)
				__glutModifierMask |= Mod1Mask;
			window->mouse(button, GLUT_UP, x, y);
			__glutModifierMask = (unsigned int)~0;
		} else {
			/* Window might have been destroyed and all the 
	 events for the window may not yet be received. */
		}
		return 0;

	case WM_ENTERMENULOOP:
		/* KLUDGE: create a timer that fires every 100 ms when we start a
       menu so that we can still process the idle & timer events (that
       way, the timers will fire during a menu pick and so will the
       idle func. */
		SetTimer(hwnd, 1, 1, NULL);
		return 0;

	case WM_TIMER:
#if 0
		/* if the timer id is 2, then this is the timer that is set up in
       the main glut message processing loop, and we don't want to do
       anything but acknowledge that we got it.  It is used to prevent
       CPU spiking when an idle function is installed. */
		if (wParam == 2)
			return 0;
#endif

		/* only worry about the idle function and the timeouts, since
       these are the only events we expect to process during
       processing of a menu. */
		/* we no longer process the idle functions (as outlined in the
       README), since drawing can't be done until the menu has
       finished...it's pretty lame when the animation goes on, but
       doesn't update, so you get this weird jerkiness. */
#if 0    
		if (__glutIdleFunc)
			__glutIdleFunc();
#endif
		if (__glutTimerList)
			handleTimeouts();
		return 0;

	case WM_EXITMENULOOP:
		/* nuke the above created timer...we don't need it anymore, since
       the menu is gone now. */
		KillTimer(hwnd, 1);
		return 0;

	case WM_MENUSELECT:
		if (lParam != 0)
			__glutHMenu = (HMENU)lParam;
		return 0;

	case WM_COMMAND:
		if (__glutMappedMenu) {
			if (GetSubMenu(__glutHMenu, LOWORD(wParam)))
				__glutItemSelected = NULL;
			else
				__glutItemSelected = 
				    __glutGetUniqueMenuItem(__glutMappedMenu, LOWORD(wParam));
			GetCursorPos(&point);
			ScreenToClient(hwnd, &point);
			__glutFinishMenu(hwnd, point.x, point.y);
		}
		return 0;

	case WM_MOUSEMOVE:
		if (GetFocus() != hwnd)
			return 0;

		if (!__glutMappedMenu) {
			window = __glutGetWindow(hwnd);
			if (window) {
				/* If motion function registered _and_ buttons held *
             down, call motion function...  */
				x = LOWORD(lParam);
				y = HIWORD(lParam);

				/* Win32 doesn't return the same numbers as X does when the
	   mouse goes beyond the upper or left side of the window.
	   roll the Win32's 0..2^16 pointer co-ord range to 0..+/-2^15. */
				if(x & 1 << 15) x -= (1 << 16);
				if(y & 1 << 15) y -= (1 << 16);

				if (window->motion && wParam &
				    (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)) {
					__glutSetWindow(window);
					window->motion(x, y);
				}
				/* If passive motion function registered _and_
	   buttons not held down, call passive motion
	   function...  */
				else if (window->passive &&
				    ((wParam &
				    (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)) ==
				    0)) {
					__glutSetWindow(window);
					window->passive(x, y);
				}
			}
		} else {
			/* Motion events are thrown away when a pop up menu is
	 active. */
		}
		return 0;

	case WM_GETMINMAXINFO:
		/* this voodoo is brought to you by Win32 (again).  It allows the
       window to be bigger than the screen, and smaller than 100x100
       (although it doesn't seem to help the y minimum). */
		minmax = (LPMINMAXINFO)lParam;
		minmax->ptMaxSize.x = __glutScreenWidth;
		minmax->ptMaxSize.y = __glutScreenHeight;
		minmax->ptMinTrackSize.x = 0;
		minmax->ptMinTrackSize.y = 0;
		minmax->ptMaxTrackSize.x = __glutScreenWidth + 
		    GetSystemMetrics(SM_CXSIZE) * 2;
		minmax->ptMaxTrackSize.y = __glutScreenHeight + 
		    GetSystemMetrics(SM_CXSIZE) * 2 + GetSystemMetrics(SM_CYCAPTION);
		return 0;

/*
	case WM_WINDOWPOSCHANGED:
		goto defproc;
		window= __glutGetWindow(hwnd);
		if (window) {
			winpos = (LPWINDOWPOS) lParam;

			__glutSetWindow(window);
					
			GdiFlush();
			window->reshape(window->width, window->height);
			window->forceReshape = FALSE;
				
			__glutPostRedisplay(window, GLUT_REPAIR_WORK);
		}
		return 0;

	case WM_MOVE:
		window= __glutGetWindow(hwnd);
		if (window) {
			window->reshape(window->width, window->height);
			window->forceReshape = FALSE;
			__glutAdjustCoords(window, window->width
		}
 		glutInitWindowPosition((int)LOWORD(lParam), (int)HIWORD(lParam));
		return 0;
*/

	case WM_SIZE:
		window = __glutGetWindow(hwnd);
		if (window) {
			width = LOWORD(lParam);
			height = HIWORD(lParam);
			if (width != window->width || height != window->height) {
				window->width = width;
				window->height = height;
				__glutSetWindow(window);
				/* Do not execute OpenGL out of sequence with respect
				   to the SetWindowPos request! */
				GdiFlush();
				window->reshape(width, height);
				window->forceReshape = FALSE;
				
				/* A reshape should be considered like posting a
				   repair request. */
				__glutPostRedisplay(window, GLUT_REPAIR_WORK);
			}
		}
		return 0;

	case WM_SETCURSOR:
		if (cust_curs) {
 			SetCursor(cust_curs); 
			return 0;
		} else {
			/* If the cursor is not in the client area, then we want to send
	       this message to the default window procedure ('cause its
	       probably in the border or title, and we don't handle that
		   cursor.  otherwise, set our cursor.  Win32 makes us set the
	       cursor every time the mouse moves (DUMB!). */
			if(LOWORD(lParam) != HTCLIENT)
				goto defproc;
			window = __glutGetWindow(hwnd);
			if (window) {
				/* Since Win32 allows the parent to control a child windows
				 cursor, if the cursor is in a child of this window, bail
				 out. */
				GetCursorPos(&point);
				ScreenToClient(hwnd, &point);
				if (hwnd != ChildWindowFromPoint(hwnd, point))

					__glutCurrentWindow = window;
				glutSetCursor(window->cursor);
			}
			/* TODO: check out the info in DevStudio on WM_SETCURSOR in the
		   DefaultAction section. */
			return 1;
		}

	case WM_SETFOCUS:
		window = __glutGetWindow(hwnd);
		if (window) {
			if (window->entry) {
				window->entryState = WM_SETFOCUS;
				__glutSetWindow(window);
				window->entry(GLUT_ENTERED);
				/* XXX Generation of fake passive notify?  See how    much
	   work the X11 code does to support fake passive    notify
	   callbacks. */
			}
		}
		return 0;

	case WM_KILLFOCUS:
		window = __glutGetWindow(hwnd);
		if (window) {
			if (window->entry) {
				window->entryState = WM_KILLFOCUS;
				__glutSetWindow(window);
				window->entry(GLUT_LEFT);
			}
		}
		return 0;

	case WM_ACTIVATE:
		window = __glutGetWindow(hwnd);
		/* make sure we re-select the correct palette if needed */
		if (LOWORD(wParam)) {
			PostMessage(hwnd, WM_PALETTECHANGED, 0, 0);
		}
		if (window) {
			GLUTwindow* child;
			int visState;
			visState = !IsIconic(window->win);
			if (visState) {			/* Not iconic. */
				visState = IsWindowVisible(window->win);
			}
			if (visState != window->visState) {
				if (window->windowStatus) {
					window->visState = visState;
					__glutSetWindow(window);
					window->windowStatus(visState);
				}
				/* Since Win32 only sends an activate for the toplevel window,
	   update the visibility for all the child windows. */
				child = window->children;
				while (child) {
					child->visState = visState;
					if (child->windowStatus) {
						child->visState = visState;
						__glutSetWindow(child);
						child->windowStatus(visState);
					}
					child = child->siblings;
				}
			}
		}
		return 0;

		/* Colour Palette Management */
	case WM_PALETTECHANGED:
		if (hwnd != (HWND)wParam)  /* don't respond to the message that we sent! */
			return(FALSE);
		break;

		/* fall through to WM_QUERYNEWPALETTE */

	case WM_QUERYNEWPALETTE:
		window = __glutGetWindow(hwnd);
		if (window && window->colormap) {
			UnrealizeObject(window->colormap->cmap);
			SelectPalette(window->hdc, window->colormap->cmap, FALSE);
			RealizePalette(window->hdc);
			return TRUE;
		}
		return FALSE;

#if 0
		/* Miscellaneous messages (don't really need to enumerate them,
     but it's good to know what you're not getting sometimes.) */
	case WM_NCHITTEST:
		/* this event is generated by every mouse move event. */
		goto defproc;
	case WM_NCMOUSEMOVE:
		goto defproc;
	case WM_NCACTIVATE:
		goto defproc;
	case WM_NCPAINT:
		goto defproc;
	case WM_NCCALCSIZE:
		goto defproc;
	case WM_NCCREATE:
		goto defproc;
	case WM_NCDESTROY:
		goto defproc;
	case WM_NCLBUTTONDOWN:
		goto defproc;
	case WM_SETTEXT:
		goto defproc;
	case WM_GETTEXT:
		goto defproc;
	case WM_ACTIVATEAPP:
		goto defproc;
	case WM_GETICON:
		goto defproc;
	case WM_ERASEBKGND:
		goto defproc;
	case WM_WINDOWPOSCHANGING:
		goto defproc;
	case WM_MOUSEACTIVATE:
		goto defproc;
	case WM_SHOWWINDOW:
		goto defproc;
	case WM_MOVING:
		goto defproc;
	case WM_KEYUP:
		goto defproc;
	case WM_CAPTURECHANGED:
		goto defproc;
	case WM_SYSCOMMAND:
		goto defproc;
	case WM_ENTERSIZEMOVE:
		goto defproc;
	case WM_ENTERIDLE:
		goto defproc;
#endif

	default:
		goto defproc;
	}

defproc:
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

static int
__win32_translate_key(int wParam, int lParam)
{
	switch (wParam) {
		/* regular keys */
		/* directional keys */
	case VK_CAPITAL: 
		return(211);
	case VK_CONTROL: 
		return(212);
	case VK_MENU:	 
		return(213);
	case VK_SHIFT:	
		return(216);
	case VK_ESCAPE:	
		return(218);
	case VK_TAB:	
		return(219);
	case VK_RETURN:	
		return(220);
	case VK_SPACE:	
		return(221);
		/*
	case VK_LINEFEED???
*/
	case VK_BACK:
		return(223);

	case VK_DELETE: 
		return(224);
		/*
	case VIRGULEKEY
*/

	case VK_LEFT:   
		return(137);
	case VK_DOWN:   
		return(138);
	case VK_RIGHT:  
		return(139);
	case VK_UP:     
		return(140);

		/* numpad keys */
	case VK_NUMPAD2:	
		return(150);
	case VK_NUMPAD4:	
		return(151);
	case VK_NUMPAD6:	
		return(152);
	case VK_NUMPAD8:	
		return(153);
	case VK_NUMPAD1:	
		return(154);
	case VK_NUMPAD3:	
		return(155);
	case VK_NUMPAD5:	
		return(156);
	case VK_NUMPAD7:	
		return(157);
	case VK_NUMPAD9:	
		return(158);

	case VK_DECIMAL:	
		return(199);

	case VK_DIVIDE:		
		return(159);
	case VK_MULTIPLY:	
		return(160);
	case VK_NUMPAD0:	
		return(161);
	case VK_SUBTRACT:	
		return(162);
	case VK_SEPARATOR:	
		return(163);
	case VK_ADD:		
		return(164);


		/* function keys */
	case VK_F1:     
		return(300);
	case VK_F2:     
		return(301);
	case VK_F3:     
		return(302);
	case VK_F4:     
		return(303);
	case VK_F5:     
		return(304);
	case VK_F6:     
		return(305);
	case VK_F7:     
		return(306);
	case VK_F8:     
		return(307);
	case VK_F9:     
		return(308);
	case VK_F10:    
		return(309);
	case VK_F11:    
		return(310);
	case VK_F12:    
		return(312);
	case VK_PAUSE:  
		return(165);
	case VK_INSERT: 
		return(166);
	case VK_HOME:   
		return(167);
	case VK_PRIOR:  
		return(168);
	case VK_NEXT:   
		return(169);
	case VK_END:    
		return(170);
	}

	switch (wParam) {
	case 186:	/* ; */
		return(225);
	case 190:	/* . */
		return(226);
	case 188:	/* , */
		return(227);
	case 222:	/* " */
		return(228);
	case 192:	/* ` */
		return(229);
	case 189:	/* - */
		return(230);

	case 191:	/* / */
		return(232);
	case 220:	/* \ */
		return(233);
	case 187:	/* = */
		return(234);
	case 219:	/* [ */
		return(235);
	case 221:	/* ] */
		return(236);
	}

	if (wParam >= 'A' && wParam <= 'Z') {
		return(wParam + 'a' - 'A');
	}

	if (wParam >= '0' && wParam <= '9') {
		return(wParam);
	}

	/* printf("glut: unsupported key, wParam=%d(%c), lParam=%d\n", */
	/*     wParam, wParam, lParam); */

	return(-1);
}
