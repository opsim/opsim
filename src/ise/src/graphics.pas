(**
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
 *)
(* graphics.h    feb 98 *)
(*
 *
 * geschikt gemakt voor X en OpenGL
 *
 *)
unit graphics;

interface

uses
  GL, GLu, GLut;

//{$ifndef GRAPHICS_H}
//{$define GRAPHICS_H}
//{$include "GL/gl.h"}
//{$include "GL/glu.h"}
//{$ifdef __BeOS}
//{$include "GL/glut.h"}
//{$else}
//{$ifdef __WIN32}
//{$include <GL/glut.h>}
//{$else}
//{$include "glut.h"}
//{$endif}
//{$endif}
//{$ifndef __WIN32}
//(* Win32 handelt het ophalen van fonts anders af *)
//(* aanvulling op glut.h *)
//{glutBitmapHelveticaBold8: pinteger; }{<= !!!5 external variable}
//{glutBitmapHelveticaBold10: pinteger; }{<= !!!5 external variable}
//{glutBitmapHelveticaBold12: pinteger; }{<= !!!5 external variable}
//{glutBitmapScreen12: pinteger; }{<= !!!5 external variable}
//{glutBitmapScreen15: pinteger; }{<= !!!5 external variable}(* Bitmap font constants (use these in GLUT program). *)
//const
//GLUT_BITMAP_HELVETICAB_8 = (@glutBitmapHelveticaBold8); 
//GLUT_BITMAP_HELVETICAB_10 = (@glutBitmapHelveticaBold10); 
//GLUT_BITMAP_HELVETICAB_12 = (@glutBitmapHelveticaBold12); 
//GLUT_BITMAP_SCREEN_12 = (@glutBitmapScreen12); 
//GLUT_BITMAP_SCREEN_15 = (@glutBitmapScreen15); 
//{$endif}
//{$if !defined __BeOS && !defined __WIN32}
//(* patch voor struct Object *)
//{$define _XtObject_h}
//{$include <X11/Xlib.h>}
//{$include <X11/StringDefs.h>}
//{$include <X11/keysym.h>  }
//{$include <GL/glx.h> }
//{$endif}
//{$include "objfnt.h"}
//{$include "Button.h"}
//{$include "screen.h"}
//{$include "mydevice.h"}
//
//function LINE2S(v1: integer; v2: integer): integer; {<= !!!6 unknown macro}
//
//function LINE3S(v1: integer; v2: integer): integer; {<= !!!6 unknown macro}
//
//function LINE2I(v1: integer; v2: integer): integer; {<= !!!6 unknown macro}
//
//function LINE3I(v1: integer; v2: integer): integer; {<= !!!6 unknown macro}
//
//function LINE2F(v1: integer; v2: integer): integer; {<= !!!6 unknown macro}
//
//function LINE3F(v1: integer; v2: integer): integer; {<= !!!6 unknown macro}
//IRIS = 1; 
//ELAN = 2; 
//ENTRY = 3; 
//{$undef VIDEO}
//VIDEO = 4; 
//M_O2 = 5; 
//(* used for glSelect *)
//type
//IGLuint = GLuint; 
//
//function SELBUFFER_INDEX(a: integer): integer; {<= !!!6 unknown macro}
//
//function getcursorN: smallint; 
//(* usiblender.c *)
//
//procedure sdrawline(x1: smallint;  y1: smallint;  x2: smallint;  y2: smallint); 
//(* Button.c *)
//
//procedure fdrawline(x1: float;  y1: float;  x2: float;  y2: float); 
//(* ****************** OGL ******************************* *)
//(* voor OpenGl compatibility *)
//Device = smallint;

const
//NORMALDRAW = 0; 
//PUPDRAW = 1; 
//OVERDRAW = 2; 
CURSOR_VPAINT = GLUT_CURSOR_LEFT_ARROW;
CURSOR_FACESEL = GLUT_CURSOR_RIGHT_ARROW;
CURSOR_WAIT = GLUT_CURSOR_WAIT;
CURSOR_EDIT = GLUT_CURSOR_CROSSHAIR;
CURSOR_STD = GLUT_CURSOR_INHERIT;

//(* Mij afspraak is cpack een getal dat als 0xFFaa66 of zo kan worden uitgedrukt. Is dus gevoelig voor endian. 
// * Met deze define wordt het altijd goed afgebeeld
// *)
//
//function cpack(x: integer): integer; {<= !!!6 unknown macro}
//
//function glMultMatrixf(x: integer): integer; {<= !!!6 unknown macro}
//
//function glLoadMatrixf(x: integer): integer; {<= !!!6 unknown macro}
//
//function lrectwrite(a: integer; b: integer; c: integer; d: integer; rect: integer): integer; {<= !!!6 unknown macro}
//(* TBOXX: breedte in pixels *)
//TBOXXL = 80; 
//TBOXXR = 170; 
//TBOXX = (TBOXXL+TBOXXR); 
//(* TBOXEL: aantal elementen onder elkaar *)
//TBOXEL = 14; 
//(* TBOXH: hoogte van 1 element *)
//TBOXH = 20; 
//TBOXY = TBOXH*TBOXEL; 
//TBOXBLACK = 1; 
//TBOXDGREY = 2; 
//TBOXWHITE = 3; 
//TBOXGREY = 4; 
//TBOXLGREY = 5; 
//(* view.c *)
//
//function boundbox_clip(obmat: array [0..,0..Pred(4)] of float;  bb: pBoundBox): integer; 
//
//procedure calctrackballvec(area: prcti;  mval: psmallint;  vec: pfloat); 
//
//procedure calctrackballvecfirst(area: prcti;  mval: psmallint;  vec: pfloat); 
//
//procedure centreview; 
//
//procedure endlocalview(sa: pScrArea); 
//
//function give_cursor: pfloat; 
//
//procedure initgrabz(x: float;  y: float;  z: float); 
//
//procedure initlocalview; 
//
//procedure obmat_to_viewmat(ob: pObject); 
//
//procedure persp(a: integer); 
//
//procedure persp_general(a: integer); 
//
//procedure project_short(vec: pfloat;  adr: psmallint); 
//
//procedure project_short_noclip(vec: pfloat;  adr: psmallint); 
//
//procedure restore_localviewdata(vd: pView3D); 
//
//function selectprojektie(buffer: pIGLuint;  x1: smallint;  y1: smallint;  x2: smallint;  y2: smallint): smallint; 
//
//procedure setviewmatrixview3d; 
//
//procedure setwinmatrixview3d(rect: prctf); 
//
//procedure view3d_home(centre: integer); 
//
//procedure viewmove(mode: integer); 
//
//procedure window_to_3d(vec: pfloat;  mx: smallint;  my: smallint); 
//{$endif}
//(* GRAPHICS_H *)

implementation


//{LINE2S(v1, v2) {glBegin(GL_LINES); glVertex2sv((short * )(v1)); glVertex2sv((short * )(v2)); glEnd();}}
//function LINE2S(v1: integer; v2: integer): integer; 
//begin
//  result:= (glBegin(GL_LINES);glVertex2sv({!!!a type cast? =>} {psmallint(}(v1));glVertex2sv({!!!a type cast? =>} {psmallint(}(v2));glEnd();)
//end;
//
//{LINE3S(v1, v2) {glBegin(GL_LINES); glVertex3sv((short * )(v1)); glVertex3sv((short * )(v2)); glEnd();}}
//function LINE3S(v1: integer; v2: integer): integer; 
//begin
//  result:= (glBegin(GL_LINES);glVertex3sv({!!!a type cast? =>} {psmallint(}(v1));glVertex3sv({!!!a type cast? =>} {psmallint(}(v2));glEnd();)
//end;
//
//{LINE2I(v1, v2) {glBegin(GL_LINES); glVertex2iv((int * )(v1)); glVertex2iv((int * )(v2)); glEnd();}}
//function LINE2I(v1: integer; v2: integer): integer; 
//begin
//  result:= (glBegin(GL_LINES);glVertex2iv({!!!a type cast? =>} {pinteger(}(v1));glVertex2iv({!!!a type cast? =>} {pinteger(}(v2));glEnd();)
//end;
//
//{LINE3I(v1, v2) {glBegin(GL_LINES); glVertex3iv((int * )(v1)); glVertex3iv((int * )(v2)); glEnd();}}
//function LINE3I(v1: integer; v2: integer): integer; 
//begin
//  result:= (glBegin(GL_LINES);glVertex3iv({!!!a type cast? =>} {pinteger(}(v1));glVertex3iv({!!!a type cast? =>} {pinteger(}(v2));glEnd();)
//end;
//
//{LINE2F(v1, v2) {glBegin(GL_LINES); glVertex2fv((float * )(v1)); glVertex2fv((float * )(v2)); glEnd();}}
//function LINE2F(v1: integer; v2: integer): integer; 
//begin
//  result:= (glBegin(GL_LINES);glVertex2fv({!!!a type cast? =>} {pfloat(}(v1));glVertex2fv({!!!a type cast? =>} {pfloat(}(v2));glEnd();)
//end;
//
//{LINE3F(v1, v2) {glBegin(GL_LINES); glVertex3fv((float * )(v1)); glVertex3fv((float * )(v2)); glEnd();}}
//function LINE3F(v1: integer; v2: integer): integer; 
//begin
//  result:= (glBegin(GL_LINES);glVertex3fv({!!!a type cast? =>} {pfloat(}(v1));glVertex3fv({!!!a type cast? =>} {pfloat(}(v2));glEnd();)
//end;
//
//{SELBUFFER_INDEX(a) (4*a+3)}
//function SELBUFFER_INDEX(a: integer): integer; 
//begin
//  result:= (4*a+3)
//end;
//
//{cpack(x) glColor3ub( ((x)&0xFF), (((x)>>8)&0xFF), (((x)>>16)&0xFF) )}
//function cpack(x: integer): integer; 
//begin
//  result:= glColor3ub(((x) and $FF),(((x) shr 8) and $FF),(((x) shr 16) and $FF))
//end;
//
//{glMultMatrixf(x)  glMultMatrixf( (float * )(x))}
//function glMultMatrixf(x: integer): integer; 
//begin
//  result:= glMultMatrixf({!!!a type cast? =>} {pfloat(}(x))
//end;
//
//{glLoadMatrixf(x)  glLoadMatrixf( (float * )(x))}
//function glLoadMatrixf(x: integer): integer; 
//begin
//  result:= glLoadMatrixf({!!!a type cast? =>} {pfloat(}(x))
//end;
//
//{lrectwrite(a, b, c, d, rect) {glRasterPos2i(a,  b);glDrawPixels((c)-(a)+1, (d)-(b)+1, GL_RGBA, GL_UNSIGNED_BYTE,  rect);}}
//function lrectwrite(a: integer; b: integer; c: integer; d: integer; rect: integer): integer; 
//begin
//  result:= (glRasterPos2i(a,b);glDrawPixels((c)-(a)+1,(d)-(b)+1,GL_RGBA,GL_UNSIGNED_BYTE,rect);)
//end;

end.
