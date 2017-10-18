
/* Copyright (c) Mark J. Kilgard, 1995. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

#include <glut.h>
#include "glutint.h"

#if !defined(WIN32)
#include <X11/Xatom.h>  /* For XA_CURSOR */
#include <X11/cursorfont.h>
#endif

typedef struct _CursorTable {
#if defined(WIN32)
  char* glyph;
#else
  int glyph;
#endif
  Cursor cursor;
} CursorTable;
/* *INDENT-OFF* */

static CursorTable cursorTable[] = {
  {XC_arrow, None},		  /* GLUT_CURSOR_RIGHT_ARROW */
  {XC_top_left_arrow, None},	  /* GLUT_CURSOR_LEFT_ARROW */
  {XC_hand1, None},		  /* GLUT_CURSOR_INFO */
  {XC_pirate, None},		  /* GLUT_CURSOR_DESTROY */
  {XC_question_arrow, None},	  /* GLUT_CURSOR_HELP */
  {XC_exchange, None},		  /* GLUT_CURSOR_CYCLE */
  {XC_spraycan, None},		  /* GLUT_CURSOR_SPRAY */
  {XC_watch, None},		  /* GLUT_CURSOR_WAIT */
  {XC_xterm, None},		  /* GLUT_CURSOR_TEXT */
  {XC_crosshair, None},		  /* GLUT_CURSOR_CROSSHAIR */
  {XC_sb_v_double_arrow, None},	  /* GLUT_CURSOR_UP_DOWN */
  {XC_sb_h_double_arrow, None},	  /* GLUT_CURSOR_LEFT_RIGHT */
  {XC_top_side, None},		  /* GLUT_CURSOR_TOP_SIDE */
  {XC_bottom_side, None},	  /* GLUT_CURSOR_BOTTOM_SIDE */
  {XC_left_side, None},		  /* GLUT_CURSOR_LEFT_SIDE */
  {XC_right_side, None},	  /* GLUT_CURSOR_RIGHT_SIDE */
  {XC_top_left_corner, None},	  /* GLUT_CURSOR_TOP_LEFT_CORNER */
  {XC_top_right_corner, None},	  /* GLUT_CURSOR_TOP_RIGHT_CORNER */
  {XC_bottom_right_corner, None}, /* GLUT_CURSOR_BOTTOM_RIGHT_CORNER */
  {XC_bottom_left_corner, None},  /* GLUT_CURSOR_BOTTOM_LEFT_CORNER */
};
/* *INDENT-ON* */

static Cursor blankCursor = None;
static Cursor fullCrosshairCusor = None;

static Cursor
getFullCrosshairCursor(void)
{
#if !defined(WIN32)
  Cursor cursor;
  Atom crosshairAtom, actualType;
  int rc, actualFormat;
  unsigned long n, left;
  unsigned long *value;

  if (fullCrosshairCusor == None) {
    crosshairAtom = XInternAtom(__glutDisplay,
      "_SGI_CROSSHAIR_CURSOR", True);
    if (crosshairAtom != None) {
      value = 0;        /* Make compiler happy. */
      rc = XGetWindowProperty(__glutDisplay, __glutRoot,
        crosshairAtom, 0, 1, False, XA_CURSOR, &actualType,
        &actualFormat, &n, &left, (unsigned char **) &value);
      if (rc == Success && actualFormat == 32 && n >= 1) {
        cursor = value[0];
        XFree(value);
        return cursor;
      }
    }
  }
  return XCreateFontCursor(__glutDisplay, XC_crosshair);
#else
  /* we could kludge up an XGetWindowProperty, XInterAtom and
     XCreateFontCursor that worked for just this case, but I dunno if
     it is worth it, hence the #ifdefs instead. */
  return IDC_CROSS;
#endif /* !WIN32 */
}

static Cursor
makeBlankCursor(void)
{
#if !defined(WIN32)
  static char data[1] =
  {0};
  Cursor cursor;
  Pixmap blank;
  XColor dummy;

  blank = XCreateBitmapFromData(__glutDisplay, __glutRoot,
    data, 1, 1);
  if (blank == None)
    __glutFatalError("out of memory.");
  cursor = XCreatePixmapCursor(__glutDisplay, blank, blank,
    &dummy, &dummy, 0, 0);
  XFreePixmap(__glutDisplay, blank);

  return cursor;
#else
  /* we could kludge up an XCreateBitmapFromData, XCreatePixmapCursor
     and XFreePixmap that worked for just this case, but I dunno if it
     is worth it, hence the #ifdefs instead. */
  return NULL;
#endif /* !WIN32 */
}

/* CENTRY */
void APIENTRY 
glutSetCursor(int cursor)
{
  Cursor xcursor;

  if (cursor >= 0 &&
    cursor < sizeof(cursorTable) / sizeof(cursorTable[0])) {
    if (cursorTable[cursor].cursor == None)
      cursorTable[cursor].cursor = XCreateFontCursor(__glutDisplay,
        cursorTable[cursor].glyph);
    xcursor = cursorTable[cursor].cursor;
  } else {
    /* Special cases. */
    switch (cursor) {
    case GLUT_CURSOR_INHERIT:
#if defined(WIN32)
      /* must be a parent - this bit of voodoo is brought to you by
	 your friendly neighborhood Win32 API.  It allows the parent
	 to override the setting of the child windows cursor (ACK!).
	 Therefore, if this parent has children (and the cursor is in
	 one), DON'T SET THE CURSOR.  */
      if (__glutCurrentWindow->parent)
        return;
      else {
        if (__glutCurrentWindow->win != GetFocus())
          return;
        xcursor = cursorTable[0].cursor;
        if (xcursor == NULL)
          xcursor =
            cursorTable[0].cursor =
            LoadCursor(NULL, cursorTable[0].glyph);
      }
#else
      xcursor = None;
#endif
      break;
    case GLUT_CURSOR_NONE:
      if (blankCursor == None)
        blankCursor = makeBlankCursor();
      xcursor = blankCursor;
      break;
    case GLUT_CURSOR_FULL_CROSSHAIR:
      if (fullCrosshairCusor == None)
        fullCrosshairCusor = getFullCrosshairCursor();
      xcursor = fullCrosshairCusor;
      break;
    }
  }
  __glutCurrentWindow->cursor = cursor;
  XDefineCursor(__glutDisplay,
    __glutCurrentWindow->win, xcursor);
  XFlush(__glutDisplay);
}
/* ENDCENTRY */
