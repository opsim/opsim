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
 * geschikt gemaakt voor X en OpenGL
 *
 *)

unit graphics;

interface

uses
  GL, GLu, GLut;

{$ifndef WINDOWS}
  (* Win32 handelt het ophalen van fonts anders af *)
  { aanvulling op glut.h  }

    var
      glutBitmapHelveticaBold8 : pointer;cvar;external;
      glutBitmapHelveticaBold10 : pointer;cvar;external;
      glutBitmapHelveticaBold12 : pointer;cvar;external;
      glutBitmapScreen12 : pointer;cvar;external;
      glutBitmapScreen15 : pointer;cvar;external;
  { Bitmap font constants (use these in GLUT program).  }

  { was #define dname def_expr }
  function GLUT_BITMAP_HELVETICAB_8 : longint; { return type might be wrong }

  { was #define dname def_expr }
  function GLUT_BITMAP_HELVETICAB_10 : longint; { return type might be wrong }

  { was #define dname def_expr }
  function GLUT_BITMAP_HELVETICAB_12 : longint; { return type might be wrong }

  { was #define dname def_expr }
  function GLUT_BITMAP_SCREEN_12 : longint; { return type might be wrong }

  { was #define dname def_expr }
  function GLUT_BITMAP_SCREEN_15 : longint; { return type might be wrong }

{$endif}

//{$if not defined BEOS and not defined WINDOWS}
//  { patch voor struct Object  }
//{$define _XtObject_h}
//{$include <X11/Xlib.h>}
//{$include <X11/StringDefs.h>}
//{$include <X11/keysym.h>  }
//{$include <GL/glx.h> }
//{$endif}

{ $include "objfnt.h"}
{ $include "Button.h"}
{ $include "screen.h"}
{ $include "mydevice.h"}

  procedure  LINE2S(v1, v2: PGLshort);	//glBegin(GL_LINES); glVertex2sv((short *)(v1)); glVertex2sv((short *)(v2)); glEnd(); }
  procedure  LINE3S(v1, v2: PGLshort);	//glBegin(GL_LINES); glVertex3sv((short *)(v1)); glVertex3sv((short *)(v2)); glEnd(); }
  procedure  LINE2I(v1, v2: PGLint);	//glBegin(GL_LINES); glVertex2iv((int *)(v1)); glVertex2iv((int *)(v2)); glEnd(); }
  procedure  LINE3I(v1, v2: PGLint);	//glBegin(GL_LINES); glVertex3iv((int *)(v1)); glVertex3iv((int *)(v2)); glEnd(); }
  procedure  LINE2F(v1, v2: PGLfloat);	//glBegin(GL_LINES); glVertex2fv((single *)(v1)); glVertex2fv((single *)(v2)); glEnd(); }
  procedure  LINE3F(v1, v2: PGLfloat);	//glBegin(GL_LINES); glVertex3fv((single *)(v1)); glVertex3fv((single *)(v2)); glEnd(); }

  const
    IRIS = 1;
    ELAN = 2;
    ENTRY = 3;
{$undef VIDEO}
    VIDEO = 4;
    M_O2 = 5;
  { used for glSelect  }

  type
    IGLuint = GLuint;
  { was #define dname(params) para_def_expr }
  { argument types are unknown }
  { return type might be wrong }

  function SELBUFFER_INDEX(a : longint) : longint;

  //function getcursorN:smallint;cdecl;

  { usiblender.c  }
  //procedure sdrawline(x1:smallint; y1:smallint; x2:smallint; y2:smallint);cdecl;

  { Button.c  }
  //procedure fdrawline(x1:single; y1:single; x2:single; y2:single);cdecl;

  { ****************** OGL *******************************  }
  { voor OpenGl compatibility  }

  type
    Device = smallint;

  const
    NORMALDRAW = 0;
    PUPDRAW = 1;
    OVERDRAW = 2;
    CURSOR_VPAINT = GLUT_CURSOR_LEFT_ARROW;
    CURSOR_FACESEL = GLUT_CURSOR_RIGHT_ARROW;
    CURSOR_WAIT = GLUT_CURSOR_WAIT;
    CURSOR_EDIT = GLUT_CURSOR_CROSSHAIR;
    CURSOR_STD = GLUT_CURSOR_INHERIT;

    //alias
    GLUT_BITMAP_HELVETICAB_12 = GLUT_BITMAP_HELVETICA_12;
    GLUT_BITMAP_HELVETICAB_10 = GLUT_BITMAP_HELVETICA_10;
    GLUT_BITMAP_HELVETICAB_8 = GLUT_BITMAP_8_BY_13;

  { Mij afspraak is cpack een getal dat als 0xFFaa66 of zo kan worden uitgedrukt. Is dus gevoelig voor endian.
   * Met deze define wordt het altijd goed afgebeeld
    }
  { was #define dname(params) para_def_expr }
  { argument types are unknown }
  { return type might be wrong }

  procedure cpack(x : longint);

  { was #define dname(params) para_def_expr }
  { argument types are unknown }
  { return type might be wrong }
  function glMultMatrixf(x : longint) : longint;

  { was #define dname(params) para_def_expr }
  { argument types are unknown }
  { return type might be wrong }
  function glLoadMatrixf(x : longint) : longint;

  {#define lrectwrite(a, b, c, d, rect)	glRasterPos2i(a,  b);glDrawPixels((c)-(a)+1, (d)-(b)+1, GL_RGBA, GL_UNSIGNED_BYTE,  rect); }
  { TBOXX: breedte in pixels  }
  const
    TBOXXL = 80;
    TBOXXR = 170;
    TBOXX = TBOXXL+TBOXXR;
  { TBOXEL: aantal elementen onder elkaar  }
    TBOXEL = 14;
  { TBOXH: hoogte van 1 element  }
    TBOXH = 20;
  {#define TBOXY	TBOXH*TBOXEL }
    TBOXBLACK = 1;
    TBOXDGREY = 2;
    TBOXWHITE = 3;
    TBOXGREY = 4;
    TBOXLGREY = 5;

  { view.c  }

  //function boundbox_clip(obmat:Parray[0..3] of single; bb:PBoundBox):longint;cdecl;
  //procedure calctrackballvec(area:Prcti; mval:Psmallint; vec:Psingle);cdecl;
  //procedure calctrackballvecfirst(area:Prcti; mval:Psmallint; vec:Psingle);cdecl;
  //procedure centreview;cdecl;
  //procedure endlocalview(sa:PScrArea);cdecl;
  //function give_cursor:^single;cdecl;
  //procedure initgrabz(x:single; y:single; z:single);cdecl;
  //procedure initlocalview;cdecl;
  //procedure obmat_to_viewmat(ob:PObject);cdecl;
  //procedure persp(a:longint);cdecl;
  //procedure persp_general(a:longint);cdecl;
  //procedure project_short(vec:Psingle; adr:Psmallint);cdecl;
  //procedure project_short_noclip(vec:Psingle; adr:Psmallint);cdecl;
  //procedure restore_localviewdata(vd:PView3D);cdecl;
  //function selectprojektie(buffer:PIGLuint; x1:smallint; y1:smallint; x2:smallint; y2:smallint):smallint;cdecl;
  //procedure setviewmatrixview3d;cdecl;
  //procedure setwinmatrixview3d(rect:Prctf);cdecl;
  //procedure view3d_home(centre:longint);cdecl;
  //procedure viewmove(mode:longint);cdecl;
  //procedure window_to_3d(vec:Psingle; mx:smallint; my:smallint);cdecl;

{ $endif}
  { GRAPHICS_H  }

implementation

  //{ was #define dname def_expr }
  //function GLUT_BITMAP_HELVETICAB_8 : longint; { return type might be wrong }
  //  begin
  //    GLUT_BITMAP_HELVETICAB_8:=@(glutBitmapHelveticaBold8);
  //  end;

  //{ was #define dname def_expr }
  //function GLUT_BITMAP_HELVETICAB_10 : longint; { return type might be wrong }
  //  begin
  //    GLUT_BITMAP_HELVETICAB_10:=@(glutBitmapHelveticaBold10);
  //  end;

  //{ was #define dname def_expr }
  //function GLUT_BITMAP_HELVETICAB_12 : longint; { return type might be wrong }
  //  begin
  //    GLUT_BITMAP_HELVETICAB_12:=@(glutBitmapHelveticaBold12);
  //  end;

//  { was #define dname def_expr }
//  function GLUT_BITMAP_SCREEN_12 : longint; { return type might be wrong }
//    begin
//      GLUT_BITMAP_SCREEN_12:=@(glutBitmapScreen12);
//    end;

//  { was #define dname def_expr }
//  function GLUT_BITMAP_SCREEN_15 : longint; { return type might be wrong }
//    begin
//      GLUT_BITMAP_SCREEN_15:=@(glutBitmapScreen15);
//    end;

  { was #define dname(params) para_def_expr }
  { argument types are unknown }
  { return type might be wrong }
  function SELBUFFER_INDEX(a : longint) : longint;
  begin
    SELBUFFER_INDEX:=(4*a)+3;
  end;

  { was #define dname(params) para_def_expr }
  { argument types are unknown }
  { return type might be wrong }
  procedure cpack(x : longint);
  begin
    glColor3ub(x and $FF,(x shr 8) and $FF,(x shr 16) and $FF);
  end;

  { was #define dname(params) para_def_expr }
  { argument types are unknown }
  { return type might be wrong }
  function glMultMatrixf(x : longint) : longint;
  begin
    glMultMatrixf:=glMultMatrixf(x);
  end;

  { was #define dname(params) para_def_expr }
  { argument types are unknown }
  { return type might be wrong }
  function glLoadMatrixf(x : longint) : longint;
  begin
    glLoadMatrixf:=glLoadMatrixf(x);
  end;

//{LINE2S(v1, v2) {glBegin(GL_LINES); glVertex2sv((short * )(v1)); glVertex2sv((short * )(v2)); glEnd();}}
  procedure LINE2S(v1, v2: PGLShort);
begin
  glBegin(GL_LINES);glVertex2sv(v1);glVertex2sv(v2);glEnd();
end;

//{LINE3S(v1, v2) {glBegin(GL_LINES); glVertex3sv((short * )(v1)); glVertex3sv((short * )(v2)); glEnd();}}
  procedure LINE3S(v1, v2: PGLShort);
begin
  glBegin(GL_LINES);glVertex3sv(v1);glVertex3sv(v2);glEnd();
end;

//{LINE2I(v1, v2) {glBegin(GL_LINES); glVertex2iv((int * )(v1)); glVertex2iv((int * )(v2)); glEnd();}}
  procedure LINE2I(v1, v2: PGLint);
begin
  glBegin(GL_LINES);glVertex2iv(v1);glVertex2iv(v2);glEnd();
end;

//{LINE3I(v1, v2) {glBegin(GL_LINES); glVertex3iv((int * )(v1)); glVertex3iv((int * )(v2)); glEnd();}}
  procedure LINE3I(v1, v2: PGLint);
begin
  glBegin(GL_LINES);glVertex3iv(v1);glVertex3iv(v2);glEnd();
end;

//{LINE2F(v1, v2) {glBegin(GL_LINES); glVertex2fv((single * )(v1)); glVertex2fv((single * )(v2)); glEnd();}}
  procedure LINE2F(v1, v2: PGLfloat);
begin
  glBegin(GL_LINES);glVertex2fv(v1);glVertex2fv(v2);glEnd();
end;

//{LINE3F(v1, v2) {glBegin(GL_LINES); glVertex3fv((single * )(v1)); glVertex3fv((single * )(v2)); glEnd();}}
  procedure LINE3F(v1, v2: PGLfloat);
begin
  glBegin(GL_LINES);glVertex3fv(v1);glVertex3fv(v2);glEnd();
end;

//{SELBUFFER_INDEX(a) (4*a+3)}
//function SELBUFFER_INDEX(a: integer): integer; 
//begin
//  result:= (4*a+3)
//end;

//{cpack(x) glColor3ub( ((x)&0xFF), (((x)>>8)&0xFF), (((x)>>16)&0xFF) )}
//function cpack(x: integer): integer; 
//begin
//  result:= glColor3ub(((x) and $FF),(((x) shr 8) and $FF),(((x) shr 16) and $FF))
//end;

//{glMultMatrixf(x)  glMultMatrixf( (single * )(x))}
//function glMultMatrixf(x: integer): integer; 
//begin
//  result:= glMultMatrixf( {pfloat(}(x))
//end;

//{glLoadMatrixf(x)  glLoadMatrixf( (single * )(x))}
//function glLoadMatrixf(x: integer): integer; 
//begin
//  result:= glLoadMatrixf( {pfloat(}(x))
//end;

//{lrectwrite(a, b, c, d, rect) {glRasterPos2i(a,  b);glDrawPixels((c)-(a)+1, (d)-(b)+1, GL_RGBA, GL_UNSIGNED_BYTE,  rect);}}
//function lrectwrite(a: integer; b: integer; c: integer; d: integer; rect: integer): integer; 
//begin
//  result:= (glRasterPos2i(a,b);glDrawPixels((c)-(a)+1,(d)-(b)+1,GL_RGBA,GL_UNSIGNED_BYTE,rect);)
//end;

end.
