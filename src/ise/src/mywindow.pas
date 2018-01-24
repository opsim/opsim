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
(* screen.c  juli 94 feb 98  GRAPHICS
 * 
 * vervanging voor een aantal fie's zoals swinopen, winset,  (zie onder)
 * dit alles omdat GL en X te traag zijn
 * 
 * feb: Opengl en toch maar naar X!
 * 
 *)
unit mywindow;

interface

uses
  arithb;

function myswinopen(winid, xmin, xmax, ymin, ymax: integer): integer;
procedure init_my_mainwin(win: integer);
procedure mywinset(wid: integer);
procedure myortho2(x1, x2, y1, y2: single);
procedure warp_pointer(x, y: integer);
procedure mywinposition(swin: integer;  xmin: integer;  xmax: integer;  ymin: integer;  ymax: integer);
procedure loadmatrix_win(mat: Mat4;  swin: integer);
procedure mywinclose(winid: integer);

implementation

uses
  GL, GLu, GLut,
  blender, screen, util,
  cfuncs;

//(*     let op: winid's beginnen met 4, eerste 3 voor GL! *)
//{$include "blender.h"}
//{$include "screen.h"}
//{$include "graphics.h"}

var
swincount: integer = 4;
curswin: integer =0;
swinarray: array [0..Pred(MAXWIN)] of pbWindow;
mainwindow: bWindow;
swindowbase: ListBase = (first:nil; last:nil);

//procedure mygetsize(x: pinteger;  y: pinteger); 
//var
//win: pbWindow; 
//begin
//  
//  if curswin<4
//  then
//  begin 
//    {*}x^:=glutGet(GLUT_WINDOW_WIDTH); 
//    {*}y^:=glutGet(GLUT_WINDOW_HEIGHT); 
//    exit;
//  end;
//  win:= swinarray[curswin]; 
//  if win=nil
//  then
//  exit;
//  {*}x^:=win^.xmax-win^.xmin+1; 
//  {*}y^:=win^.ymax-win^.ymin+1; 
//end;
//
//procedure mygetorigin(x: pinteger;  y: pinteger); 
//var
//win: pbWindow; 
//begin
//  {*}x^:=glutGet(GLUT_WINDOW_X); 
//  {*}y^:=displaysizey-glutGet(GLUT_WINDOW_HEIGHT)-glutGet(GLUT_WINDOW_Y); 
//  if curswin>=4
//  then
//  begin 
//    win:= swinarray[curswin]; 
//    if win<>nil 
//    then
//    begin 
//      {*}x^+=win^.xmin; 
//      {*}y^+=win^.ymin; 
//    end;
//  end;
//end;
//
//procedure mygetsuborigin(x: pinteger;  y: pinteger); 
//var
//win: pbWindow; 
//begin
//  
//  if curswin=1
//  then
//  begin 
//    {*}x^:=*y:=nil; 
//  end;
//  else
//  begin 
//    win:= swinarray[curswin]; 
//    if win=nil
//    then
//    exit;
//    {*}x^:=win^.xmin; 
//    {*}y^:=win^.ymin; 
//  end;
//end;
//
//procedure myloadmatrix(mat: array [0..,0..Pred(4)] of float); 
//var
//win: pbWindow; 
//mode: GLint; 
//begin
//  
//  
//  glLoadMatrixf(mat); 
//  win:= swinarray[curswin]; 
//  if win=nil
//  then
//  exit;
//  glGetIntegerv(GL_MATRIX_MODE,@mode); 
//  if mode=GL_MODELVIEW
//  then
//  Mat4CpyMat4(win^.viewmat,mat); 
//  else
//  Mat4CpyMat4(win^.winmat,mat); 
//end;

procedure loadmatrix_win(mat: Mat4;  swin: integer);
var
win: pbWindow;
mode: GLint;
begin
  win:= swinarray[swin];

  if win=nil  then
  exit;

  glGetIntegerv(GL_MATRIX_MODE,@mode);
  if mode=GL_MODELVIEW   then
  Mat4CpyMat4(@win^.viewmat,@mat)
  else
  Mat4CpyMat4(@win^.winmat,@mat);
end;

//procedure mygetmatrix(mat: array [0..,0..Pred(4)] of float); 
//var
//win: pbWindow; 
//mode: GLint; 
//begin
//  
//  
//  win:= swinarray[curswin]; 
//  if win=nil
//  then
//  exit;
//  glGetIntegerv(GL_MATRIX_MODE,@mode); 
//  if mode=GL_MODELVIEW
//  then
//  Mat4CpyMat4(mat,win^.viewmat); 
//  else
//  Mat4CpyMat4(mat,win^.winmat); 
//end;
//
//procedure mymultmatrixold(mat: array [0..,0..Pred(4)] of float); 
//var
//win: pbWindow; 
//tmat: array [0..Pred(4),0..Pred(4)] of float; 
//begin
//  
//  
//  win:= swinarray[curswin]; 
//  if win=nil
//  then
//  exit;
//  Mat4MulMat4(tmat,mat,win^.viewmat); 
//  Mat4CpyMat4(win^.viewmat,tmat); 
//  glLoadMatrixf(tmat); 
//end;
//
//procedure mymultmatrix(mat: array [0..,0..Pred(4)] of float); 
//var
//win: pbWindow; 
//tmat: array [0..Pred(4),0..Pred(4)] of float; 
//begin
//  
//  
//  glMultMatrixf(mat); 
//  win:= swinarray[curswin]; 
//  if win=nil
//  then
//  exit;
//  glGetFloatv(GL_MODELVIEW_MATRIX,{!!!a type cast? =>} {pfloat(}win^.viewmat); 
//end;
//
//procedure mygetsingmatrix(mat: array [0..,0..Pred(4)] of float); 
//var
//win: pbWindow; 
//matview: array [0..Pred(4),0..Pred(4)] of float; 
//matproj: array [0..Pred(4),0..Pred(4)] of float; 
//mode: integer; 
//begin
//  
//  
//  
//  
//  win:= swinarray[curswin]; 
//  if win=nil
//  then
//  begin 
//    glGetFloatv(GL_PROJECTION_MATRIX,{!!!a type cast? =>} {pfloat(}matproj); 
//    glGetFloatv(GL_MODELVIEW_MATRIX,{!!!a type cast? =>} {pfloat(}matview); 
//    Mat4MulMat4(mat,matview,matproj); 
//  end;
//  else
//  begin 
//    Mat4MulMat4(mat,win^.viewmat,win^.winmat); 
//  end;
//end;

function mywinnewid: integer;
var
wid: integer =0;
a: integer = 4;
begin
  while (wid=0)and(a<MAXWIN)  do
  begin
    if swinarray[a]=nil    then
      wid:= a;
    inc(a);
  end;

  exit(wid);
end;

(* deze winset en winget gaan over de aktieve output window *)
(* de window waar de muis staat is G.curscreen->winakt *)
//function mywinget: integer; 
//begin
//  begin
//    result:= curswin; 
//    exit;
//  end;
//end;

procedure mywinset(wid: integer);
var
win: pbWindow;
begin

  if (wid>1)and(wid<4)  then
  begin
    (* render window *)
    glutSetWindow(wid);
    curswin:= wid;
    exit;
  end;

  win:= swinarray[wid];
  if win=nil   then
  begin
    printf('winset %d: doesn'#39't exist\n',[wid]);
    exit;
  end;

  glutSetWindow(win^.parent_id);

  if win^.parent_id=wid    then
  begin
    (* main window *)
    glViewport(0,0,(win^.xmax-win^.xmin)+1,(win^.ymax-win^.ymin)+1);
    glScissor(0,0,(win^.xmax-win^.xmin)+1,(win^.ymax-win^.ymin)+1);
  end
  else
  begin
    glViewport(win^.xmin,win^.ymin,(win^.xmax)-(win^.xmin)+1,(win^.ymax)-(win^.ymin)+1);
    glScissor(win^.xmin,win^.ymin,(win^.xmax)-(win^.xmin)+1,(win^.ymax)-(win^.ymin)+1);
  end;

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(@win^.winmat[0]);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(@win^.viewmat[0]);
  curswin:= wid;
end;

//function mywinexist(wid: integer): integer; 
//begin
//  begin
//    result:= {!!!a type cast? =>} {integer(}swinarray[wid]; 
//    exit;
//  end;
//end;

var
  firsttime: integer = 1;

function myswinopen(winid, xmin, xmax, ymin, ymax: integer): integer;
var
win: pbWindow;
begin
  win:= callocN(sizeof(bWindow), 'winopen');
  addtail(@swindowbase,win);
  inc(swincount);

  if swincount>=MAXWIN  then
  begin
    printf('too many windows\n');
    exit(0);
  end;

  win^.parent_id:= winid;
  win^.id:= mywinnewid();
  win^.xmin:= xmin;
  win^.ymin:= ymin;
  win^.xmax:= xmax;
  win^.ymax:= ymax;
  swinarray[win^.id]:= win;
  Mat4One(win^.viewmat);
  Mat4One(win^.winmat);
  mywinset(win^.id);

  exit(win^.id);
end;

procedure mywinclose(winid: integer);
var
win: pbWindow;
begin
  if (winid<>0)and(winid<4)  then
  begin
    glutDestroyWindow(winid);
    exit;
  end;

  win:= swinarray[winid];
  if win=nil   then
  begin
    printf('error: close window %d, doesn''t exist\n',[winid]);
    exit;
  end;

  remlink(@swindowbase,win);
  freeN(win);
  swinarray[winid]:=nil;
  if curswin=winid
  then
  curswin:=0;
  dec(swincount);
end;

(* let op: andere syntax *)
procedure mywinposition(swin: integer;  xmin: integer;  xmax: integer;  ymin: integer;  ymax: integer);
var
win: pbWindow;
begin
  win:= swinarray[swin];
  if win=nil  then
  exit;

  win^.xmin:= xmin;
  win^.ymin:= ymin;
  win^.xmax:= xmax;
  win^.ymax:= ymax;
end;

procedure myortho2(x1, x2, y1, y2: single);
var
win: pbWindow;
begin

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(x1,x2,y1,y2);
  win:= swinarray[curswin];

  if win<>nil  then
  glGetFloatv(GL_PROJECTION_MATRIX, @win^.winmat[0]);

  glMatrixMode(GL_MODELVIEW);
end;

//procedure myortho(x1: float;  x2: float;  y1: float;  y2: float;  n: float;  f: float); 
//var
//win: pbWindow; 
//begin
//  
//  glMatrixMode(GL_PROJECTION); 
//  glLoadIdentity(); 
//  glOrtho(x1,x2,y1,y2,n,f); 
//  win:= swinarray[curswin]; 
//  if win<>nil 
//  then
//  glGetFloatv(GL_PROJECTION_MATRIX,{!!!a type cast? =>} {pfloat(}win^.winmat); 
//  glMatrixMode(GL_MODELVIEW); 
//end;
//
//procedure mywindow(x1: float;  x2: float;  y1: float;  y2: float;  n: float;  f: float); 
//var
//win: pbWindow; 
//begin
//  
//  glMatrixMode(GL_PROJECTION); 
//  glLoadIdentity(); 
//  glFrustum(x1,x2,y1,y2,n,f); 
//  win:= swinarray[curswin]; 
//  if win<>nil 
//  then
//  glGetFloatv(GL_PROJECTION_MATRIX,{!!!a type cast? =>} {pfloat(}win^.winmat); 
//  glMatrixMode(GL_MODELVIEW); 
//end;

procedure init_my_mainwin(win: integer);
var
orx: integer;
ory: integer;
sizex: integer;
sizey: integer;
endx: integer;
endy: integer;
begin
  glEnable(GL_SCISSOR_TEST);
  //{$if not defined(__BeOS) && not defined (__WIN32)}
  //glutSetWindow(win);
  //{$endif}
  sizex:= glutGet(GLUT_WINDOW_WIDTH);
  sizey:= glutGet(GLUT_WINDOW_HEIGHT);
  orx:= glutGet(GLUT_WINDOW_X);
  ory:= displaysizey-sizey-glutGet(GLUT_WINDOW_Y);
endx:= orx+sizex-1;
endy:= ory+sizey-1;
mainwindow.xmin:= orx;
mainwindow.ymin:= ory;
mainwindow.xmax:= endx;
mainwindow.ymax:= endy;
ortho2(-0.5,{!!!a type cast? =>} {float(}sizex-0.5,-0.5,{!!!a type cast? =>} {float(}sizey-0.5);
glLoadIdentity();
glGetFloatv(GL_PROJECTION_MATRIX, @mainwindow.winmat[0]);
glGetFloatv(GL_MODELVIEW_MATRIX, @mainwindow.viewmat[0]);
swinarray[win]:= @mainwindow;
mainwindow.parent_id:= win;
curswin:= win;
end;

(* ********** END MY WINDOW ************** *)

//procedure myswapbuffers; 
//var
//sa: pScrArea; 
//begin
//  
//  sa:= G.curscreen.areabase.first; 
//  while sa
//  do
//  begin 
//    if sa.win_swap=WIN_BACK_OK
//    then
//    sa.win_swap:= WIN_FRONT_OK; 
//    if sa.head_swap=WIN_BACK_OK
//    then
//    sa.head_swap:= WIN_FRONT_OK; 
//    sa:= sa.next; 
//  end;
//  glutSwapBuffers(); 
//end;
//(* ************************** FONTS ************************ *)
//fmtype: pinteger =nil; 
//
//procedure fmsetfont(type: pinteger); 
//begin
//  fmtype:= type; 
//end;
//
//procedure fmprstr(str: pchar); 
//begin
//  glutBitmapStr({!!!a type cast? =>} {pinteger(}fmtype,str); 
//end;
//
//procedure fmoutchar(font: pinteger;  c: char); 
//begin
//  glutBitmapCharacter({!!!a type cast? =>} {pinteger(}fmtype,c); 
//end;
//
//function fmstrwidth(str: pchar): integer; 
//var
//i: integer; 
//len: integer; 
//retval: integer;
// 
//begin
//  
//  
//  retval:=nil; 
//  if {not}0=fmtype
//  then
//  begin
//    result:=nil; 
//    exit;
//  end;
//  len:= lstrlen(str); 
//  for{while} i:=nil to Pred(len) { i++}
//  do
//  begin 
//    retval:= retval + (glutBitmapWidth(fmtype,str[i])); 
//  end;
//  begin
//    result:= retval; 
//    exit;
//  end;
//end;
//
//function fmgetstrwidth(handle: pinteger;  str: pchar): integer; 
//var
//i: integer; 
//len: integer; 
//retval: integer;
// 
//begin
//  
//  
//  retval:=nil; 
//  fmsetfont(handle); 
//  len:= {!!!a type cast? =>} {integer(}lstrlen(str); 
//  for{while} i:=nil to Pred(len) { i++}
//  do
//  begin 
//    retval:= retval + (glutBitmapWidth(fmtype,str[i])); 
//  end;
//  begin
//    result:= retval; 
//    exit;
//  end;
//end;
//
//procedure fminit; 
//begin
//end;
//
//function fmfindfont(name: pchar): pinteger; 
//begin
//  begin
//    result:=nil; 
//    exit;
//  end;
//end;
//
//function fmscalefont: pinteger; 
//begin
//  begin
//    result:=nil; 
//    exit;
//  end;
//end;
//
//procedure fmrotatepagematrix(degrees: float); 
//begin
//end;
//(* *********************** PATTERNS ENZO ***************** *)
//
//procedure setlinestyle(nr: integer); 
//begin
//  if nr=nil
//  then
//  begin 
//    glDisable(GL_LINE_STIPPLE); 
//  end;
//  else
//  begin 
//    glEnable(GL_LINE_STIPPLE); 
//    glLineStipple(nr,$AAAA); 
//  end;
//end;
//(* ***************************************************** *)
//(* ***************************************************** *)
//{$if defined(__BeOS) || defined(__WIN32)}
//var {was static}
//ximage: pinteger =nil; 
//xdraw: integer =nil; (* wordt maar 1 keer gedaan, voor mainwin *)
//ov_x: integer; 
//ov_y: integer; 
//ov_sx: integer; 
//ov_sy: integer; 
//
//function get_qual: smallint; 
//var
//retval: integer;
// 
//qual: integer;
// 
//begin
//  retval:=nil; 
//  qual:=glutGetQual(); 
//  if qual and GLUT_ACTIVE_SHIFT
//  then
//  retval:= retval or (LR_SHIFTKEY); 
//  if qual and GLUT_ACTIVE_CTRL
//  then
//  retval:= retval or (LR_CTRLKEY); 
//  if qual and GLUT_ACTIVE_ALT
//  then
//  retval:= retval or (LR_ALTKEY); 
//  begin
//    result:= retval; 
//    exit;
//  end;
//end;
//
//function get_mbut: smallint; 
//var
//mval: integer;
// 
//qual: integer;
// 
//ret: smallint;
// 
//begin
//  mval:=glutGetMouseButton(); 
//  qual:=glutGetQual(); 
//  ret:=nil; 
//  if mval and 1
//  then
//  ret:= ret or (L_MOUSE); 
//  if mval and 4
//  then
//  ret:= ret or (M_MOUSE); 
//  if mval and 2
//  then
//  ret:= ret or (R_MOUSE); 
//  if (U.flag and TWOBUTTONMOUSE))and((qual and GLUT_ACTIVE_ALT)
//  then
//  begin 
//    if ret and L_MOUSE
//    then
//    begin 
//      ret:= ret and ( not L_MOUSE); 
//      ret:= ret or (M_MOUSE); 
//    end;
//  end;
//  begin
//    result:= ret; 
//    exit;
//  end;
//end;
//
//procedure getmouse(mval: psmallint); 
//begin
//  glutGetMouse(mval); 
//  mval[0]:= mval[0] + (G.curscreen.startx); 
//  mval[1]:= G.curscreen.starty+G.curscreen.sizey-mval[1]; 
//  (* returns windowcos *)
//end;
//
//procedure my_put_frontbuffer_image; 
//begin
//  ximage:=nil; 
//  glutPutFrontBuffer(); 
//  (* return; *)
//end;
//
//procedure my_get_frontbuffer_image(x: integer;  y: integer;  sx: integer;  sy: integer); 
//begin
//  ximage:= 1; 
//  ov_x:= x; 
//  ov_y:= y; 
//  ov_sx:= sx; 
//  ov_sy:= sy; 
//  {$ifdef __WIN32}
//  y:= (G.curscreen.sizey-y); (* ander coordinatensysteem! *)
//  if curswin>3
//  then
//  begin 
//    y:= y - (curarea.winrct.ymin); 
//  end;
//  {$endif}
//  glutGetFrontBuffer(x,y,sx,sy); 
//end;
//
//procedure myCopySubBuffer(x: integer;  y: integer;  sx: integer;  sy: integer); 
//begin
//  (* return; *)
//  (* glutPutFrontBuffer();  *)
//end;
//
//procedure myContextSetup; 
//begin
//end;
//{$ifdef __BeOS}
//
//procedure glutNoBorder; 
//begin
//end;
//
//procedure usleep(val: uint); 
//begin
//  suspend_thread(0); 
//  snooze(val); 
//  resume_thread(0); 
//end;
//
//function rawkey_beos(key: integer): integer; 
//begin
//  case key of
//    60:
//    begin
//      begin
//        result:= AKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    80:
//    begin
//      begin
//        result:= BKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    78:
//    begin
//      begin
//        result:= CKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    62:
//    begin
//      begin
//        result:= DKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    41:
//    begin
//      begin
//        result:= EKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    63:
//    begin
//      begin
//        result:= FKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    64:
//    begin
//      begin
//        result:= GKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    65:
//    begin
//      begin
//        result:= HKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    46:
//    begin
//      begin
//        result:= IKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    66:
//    begin
//      begin
//        result:= JKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    67:
//    begin
//      begin
//        result:= KKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    68:
//    begin
//      begin
//        result:= LKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    82:
//    begin
//      begin
//        result:= MKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    81:
//    begin
//      begin
//        result:= NKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    47:
//    begin
//      begin
//        result:= OKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    48:
//    begin
//      begin
//        result:= PKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    39:
//    begin
//      begin
//        result:= QKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    42:
//    begin
//      begin
//        result:= RKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    61:
//    begin
//      begin
//        result:= SKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    43:
//    begin
//      begin
//        result:= TKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    45:
//    begin
//      begin
//        result:= UKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    79:
//    begin
//      begin
//        result:= VKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    40:
//    begin
//      begin
//        result:= WKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    77:
//    begin
//      begin
//        result:= XKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    44:
//    begin
//      begin
//        result:= YKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    76:
//    begin
//      begin
//        result:= ZKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    27:
//    begin
//      begin
//        result:= ZEROKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    18:
//    begin
//      begin
//        result:= ONEKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    19:
//    begin
//      begin
//        result:= TWOKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    20:
//    begin
//      begin
//        result:= THREEKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    21:
//    begin
//      begin
//        result:= FOURKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    22:
//    begin
//      begin
//        result:= FIVEKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    23:
//    begin
//      begin
//        result:= SIXKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    24:
//    begin
//      begin
//        result:= SEVENKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    25:
//    begin
//      begin
//        result:= EIGHTKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    26:
//    begin
//      begin
//        result:= NINEKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    1:
//    begin
//      begin
//        result:= ESCKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    2:
//    begin
//      begin
//        result:= F1KEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    3:
//    begin
//      begin
//        result:= F2KEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    4:
//    begin
//      begin
//        result:= F3KEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    5:
//    begin
//      begin
//        result:= F4KEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    6:
//    begin
//      begin
//        result:= F5KEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    7:
//    begin
//      begin
//        result:= F6KEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    8:
//    begin
//      begin
//        result:= F7KEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    9:
//    begin
//      begin
//        result:= F8KEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    10:
//    begin
//      begin
//        result:= F9KEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    11:
//    begin
//      begin
//        result:= F10KEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    12:
//    begin
//      begin
//        result:= F11KEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    13:
//    begin
//      begin
//        result:= F12KEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    GLUT_SHIFT:
//    begin
//      begin
//        result:= LEFTSHIFTKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    GLUT_ALT:
//    begin
//      begin
//        result:= LEFTALTKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    GLUT_CTRL:
//    begin
//      begin
//        result:= LEFTCTRLKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    38:
//    begin
//      begin
//        result:= TABKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    71:
//    begin
//      begin
//        result:= RETKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    94:
//    begin
//      begin
//        result:= SPACEKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    30:
//    begin
//      begin
//        result:= BACKSPACEKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    52:
//    begin
//      begin
//        result:= DELKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    69:
//    begin
//      begin
//        result:= SEMICOLONKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    84:
//    begin
//      begin
//        result:= PERIODKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    83:
//    begin
//      begin
//        result:= COMMAKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    70:
//    begin
//      begin
//        result:= QUOTEKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    17:
//    begin
//      begin
//        result:= ACCENTGRAVEKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    28:
//    begin
//      begin
//        result:= MINUSKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    85:
//    begin
//      begin
//        result:= VIRGULEKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    51:
//    begin
//      begin
//        result:= BACKSLASHKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    29:
//    begin
//      begin
//        result:= EQUALKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    49:
//    begin
//      begin
//        result:= LEFTBRACKETKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    50:
//    begin
//      begin
//        result:= RIGHTBRACKETKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    97:
//    begin
//      begin
//        result:= LEFTARROWKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    99:
//    begin
//      begin
//        result:= RIGHTARROWKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    87:
//    begin
//      begin
//        result:= UPARROWKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    98:
//    begin
//      begin
//        result:= DOWNARROWKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    100:
//    begin
//      begin
//        result:= PAD0; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    88:
//    begin
//      begin
//        result:= PAD1; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    89:
//    begin
//      begin
//        result:= PAD2; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    90:
//    begin
//      begin
//        result:= PAD3; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    72:
//    begin
//      begin
//        result:= PAD4; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    73:
//    begin
//      begin
//        result:= PAD5; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    74:
//    begin
//      begin
//        result:= PAD6; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    55:
//    begin
//      begin
//        result:= PAD7; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    56:
//    begin
//      begin
//        result:= PAD8; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    57:
//    begin
//      begin
//        result:= PAD9; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    101:
//    begin
//      begin
//        result:= PADPERIOD; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    35:
//    begin
//      begin
//        result:= PADVIRGULEKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    36:
//    begin
//      begin
//        result:= PADASTERKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    91:
//    begin
//      begin
//        result:= PADENTER; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    37:
//    begin
//      begin
//        result:= PADMINUS; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    58:
//    begin
//      begin
//        result:= PADPLUSKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    31:
//    begin
//      begin
//        result:= INSERTKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    32:
//    begin
//      begin
//        result:= HOMEKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    33:
//    begin
//      begin
//        result:= PAGEUPKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    54:
//    begin
//      begin
//        result:= PAGEDOWNKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    53:
//    begin
//      begin
//        result:= ENDKEY; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    else
//    begin
//      begin
//        result:=nil; 
//        exit;
//      end;
//    end;
//  end;{case?}
//end;
//{$endif}
//
//procedure sdrawXORline(x0: integer;  y0: integer;  x1: integer;  y1: integer); 
//begin
//  if x0=x1)and(y0=y1
//  then
//  exit;
//  (* sdrawXORline expects current blender win coordinates *)
//  if curswin>3
//  then
//  begin 
//    x0:= x0 + (curarea.winrct.xmin); 
//    x1:= x1 + (curarea.winrct.xmin); 
//    y0:= y0 + (curarea.winrct.ymin); 
//    y1:= y1 + (curarea.winrct.ymin); 
//  end;
//  glutInvertLine(x0,y0,x1,y1); 
//end;
//var {was static}
//old: array [0..Pred(4),0..Pred(4)] of smallint; 
//flags: array [0..Pred(4)] of char;
// (* automatische onthoud, max 4 lijnen *)
//(* flush *)
//
//procedure sdrawXORline4(nr: integer;  x0: integer;  y0: integer;  x1: integer;  y1: integer); 
//begin
//  
//  flags:=(0,0,0,0); (* automatische onthoud, max 4 lijnen *)
//  (* flush *)
//  if nr=-1
//  then
//  begin 
//    if flags[0]<>nil 
//    then
//    sdrawXORline(old[0][0],old[0][1],old[0][2],old[0][3]); 
//    flags[0]:=nil; 
//    if flags[1]<>nil 
//    then
//    sdrawXORline(old[1][0],old[1][1],old[1][2],old[1][3]); 
//    flags[1]:=nil; 
//    if flags[2]<>nil 
//    then
//    sdrawXORline(old[2][0],old[2][1],old[2][2],old[2][3]); 
//    flags[2]:=nil; 
//    if flags[3]<>nil 
//    then
//    sdrawXORline(old[3][0],old[3][1],old[3][2],old[3][3]); 
//    flags[3]:=nil; 
//  end;
//  else
//  begin 
//    if nr>=nil)and(nr<4
//    then
//    begin 
//      if flags[nr]<>nil 
//      then
//      sdrawXORline(old[nr][0],old[nr][1],old[nr][2],old[nr][3]); 
//      old[nr][0]:= x0; 
//      old[nr][1]:= y0; 
//      old[nr][2]:= x1; 
//      old[nr][3]:= y1; 
//      flags[nr]:= 1; 
//    end;
//    sdrawXORline(x0,y0,x1,y1); 
//  end;
//end;
//(* end of #if defined(__BeOS ) || defined(WIN32) *)
//(* ******************************************* *)
//(* ******************************************* *)
//(* ******************************************* *)
//(* ************* X  STUFF (and some OpenGL that doesn't work at BeOS) ***************** *)
//{$else}
//{__glutDisplay: pDisplay; }{<= !!!5 external variable}(* extern GLUTwindow *__glutCurrentWindow; *)
//{__glutScreen: integer; }{<= !!!5 external variable}
//var {was static}
//ximage: pXImage =nil; 
//xdraw: GLXDrawable =nil; (* wordt maar 1 keer gedaan, voor mainwin *)
//ov_x: integer; 
//ov_y: integer; 
//ov_sx: integer; 
//ov_sy: integer; 
//curgc: GC; 
//blackGC: GC; 
//grayGC: GC; 
//whiteGC: GC; 
//darkGC: GC; 
//lightGC: GC; 
//contextdone: integer =nil; 
//
//procedure sdrawXORline(x0: integer;  y0: integer;  x1: integer;  y1: integer); 
//begin
//  {$ifdef MESA31}
//  {$endif}
//  exit;
//  if x0=x1)and(y0=y1
//  then
//  {$if defined(__sgi) || defined(__SUN)}
//  exit;
//  glDisable(GL_DITHER); 
//  {$endif}
//  glBlendEquationEXT(GL_LOGIC_OP); 
//  glEnable(GL_LOGIC_OP); 
//  glEnable(GL_BLEND); 
//  glLogicOp(GL_INVERT); 
//  sdrawline(x0,y0,x1,y1); 
//  glBlendEquationEXT(GL_FUNC_ADD_EXT); 
//  glDisable(GL_LOGIC_OP); 
//  glDisable(GL_BLEND); 
//  {$if defined(__sgi) || defined(__SUN)}
//  glEnable(GL_DITHER); 
//  {$endif}
//end;
//var {was static}
//old: array [0..Pred(4),0..Pred(4)] of smallint; 
//flags: array [0..Pred(4)] of char;
// 
//{$ifdef MESA31}
//
//procedure sdrawXORline4(nr: integer;  x0: integer;  y0: integer;  x1: integer;  y1: integer); 
//begin
//  
//  flags:=(0,0,0,0); 
//  {$ifdef MESA31}
//  {$endif}
//  (* automatische onthoud, max 4 lijnen *)
//  {$if defined(__sgi) || defined(__SUN)}
//  exit;
//  glDisable(GL_DITHER); 
//  {$endif}
//  glBlendEquationEXT(GL_LOGIC_OP); 
//  glEnable(GL_LOGIC_OP); 
//  glEnable(GL_BLEND); 
//  glLogicOp(GL_INVERT); 
//  (* flush *)
//  if nr=-1
//  then
//  begin 
//    if flags[0]<>nil 
//    then
//    sdrawline(old[0][0],old[0][1],old[0][2],old[0][3]); 
//    flags[0]:=nil; 
//    if flags[1]<>nil 
//    then
//    sdrawline(old[1][0],old[1][1],old[1][2],old[1][3]); 
//    flags[1]:=nil; 
//    if flags[2]<>nil 
//    then
//    sdrawline(old[2][0],old[2][1],old[2][2],old[2][3]); 
//    flags[2]:=nil; 
//    if flags[3]<>nil 
//    then
//    sdrawline(old[3][0],old[3][1],old[3][2],old[3][3]); 
//    flags[3]:=nil; 
//  end;
//  else
//  begin 
//    if nr>=nil)and(nr<4
//    then
//    begin 
//      if flags[nr]<>nil 
//      then
//      sdrawline(old[nr][0],old[nr][1],old[nr][2],old[nr][3]); 
//      old[nr][0]:= x0; 
//      old[nr][1]:= y0; 
//      old[nr][2]:= x1; 
//      old[nr][3]:= y1; 
//      flags[nr]:= 1; 
//    end;
//    sdrawline(x0,y0,x1,y1); 
//  end;
//  glDisable(GL_LOGIC_OP); 
//  glDisable(GL_BLEND); 
//  glBlendEquationEXT(GL_FUNC_ADD_EXT); 
//  {$if defined(__sgi) || defined(__SUN)}
//  glEnable(GL_DITHER); 
//  {$endif}
//end;
//
//procedure myContextSetup; 
//const
//str: pchar; 
//var
//gcvals: XGCValues; 
//dep: integer; 
//child: Window; 
//root: Window; 
//root_x: integer; 
//root_y: integer; 
//win_x: integer; 
//win_y: integer; 
//Xqual: UINT; 
//ret: smallint;
// 
//begin
//  
//  
//  
//  gcvals.font:=nil; 
//  if xdraw=nil
//  then
//  xdraw:= glXGetCurrentDrawable(); 
//  dep:= glutGet(GLUT_WINDOW_BUFFER_SIZE); 
//  if dep<12
//  then
//  printf('Error: screen depth (%d) should be at least 15.\n',dep); 
//  if G.f and G_DEBUG
//  then
//  begin 
//    printf('Display: %d %d\n',displaysizex,displaysizey); 
//    PRINT(d,dep); 
//  end;
//  gcvals.foreground:=nil; 
//  blackGC:= XCreateGC(__glutDisplay,xdraw,GCForeground,@gcvals); 
//  {$ifdef __sgi}
//  if dep=15)or(dep=16
//  then
//  {$else}
//  begin 
//    if dep=15
//    then
//    {$endif}
//    begin 
//      gcvals.foreground:= (10+(10 shl 5)+(10 shl 10)); 
//      darkGC:= XCreateGC(__glutDisplay,xdraw,GCForeground,@gcvals); 
//      gcvals.foreground:= (15+(15 shl 5)+(15 shl 10)); 
//      grayGC:= XCreateGC(__glutDisplay,xdraw,GCForeground,@gcvals); 
//      gcvals.foreground:= (23+(23 shl 5)+(23 shl 10)); 
//      lightGC:= XCreateGC(__glutDisplay,xdraw,GCForeground,@gcvals); 
//      gcvals.foreground:= (31+(31 shl 5)+(31 shl 10)); 
//      whiteGC:= XCreateGC(__glutDisplay,xdraw,GCForeground,@gcvals); 
//    end;
//    else
//    begin 
//      gcvals.foreground:= (dep=16 {was ?}if  then (10+(10 shl 6)+(10 shl 11)) {was :}else $555555); 
//      darkGC:= XCreateGC(__glutDisplay,xdraw,GCForeground,@gcvals); 
//      gcvals.foreground:= (dep=16 {was ?}if  then (15+(15 shl 6)+(15 shl 11)) {was :}else $999999); 
//      grayGC:= XCreateGC(__glutDisplay,xdraw,GCForeground,@gcvals); 
//      gcvals.foreground:= (dep=16 {was ?}if  then (23+(23 shl 6)+(23 shl 11)) {was :}else $cccccc); 
//      lightGC:= XCreateGC(__glutDisplay,xdraw,GCForeground,@gcvals); 
//      gcvals.foreground:= (dep=16 {was ?}if  then (31+(31 shl 6)+(31 shl 11)) {was :}else $ffffff); 
//      whiteGC:= XCreateGC(__glutDisplay,xdraw,GCForeground,@gcvals); 
//    end;
//    curgc:= blackGC; 
//    contextdone:= 1; 
//  end;
//  
//  function get_qual: smallint; 
//  var
//  child: Window; 
//  root: Window; 
//  root_x: integer; 
//  root_y: integer; 
//  win_x: integer; 
//  win_y: integer; 
//  Xqual: UINT; 
//  ret: smallint;
//   
//  begin
//    
//    
//    
//    
//    
//    
//    
//    ret:=nil; 
//    XQueryPointer(__glutDisplay,RootWindow(__glutDisplay,__glutScreen),@root, and child,@root_x,@root_y,@win_x,@win_y,@Xqual); 
//    if Xqual and ShiftMask
//    then
//    ret:= ret or (LR_SHIFTKEY); 
//    if Xqual and ControlMask
//    then
//    ret:= ret or (LR_CTRLKEY); 
//    if Xqual and Mod1Mask
//    then
//    ret:= ret or (LR_ALTKEY); 
//    begin
//      result:= ret; 
//      exit;
//    end;
//  end
//  
//  function get_mbut: smallint; 
//  var
//  child: Window; 
//  root: Window; 
//  root_x: integer; 
//  root_y: integer; 
//  win_x: integer; 
//  win_y: integer; 
//  Xqual: UINT; 
//  ret: smallint;
//   
//  begin
//    
//    
//    
//    
//    
//    
//    
//    ret:=nil; 
//    XQueryPointer(__glutDisplay,RootWindow(__glutDisplay,__glutScreen),@root, and child,@root_x,@root_y,@win_x,@win_y,@Xqual); 
//    if Xqual and Button1Mask
//    then
//    ret:= ret or (L_MOUSE); 
//    if Xqual and Button2Mask
//    then
//    ret:= ret or (M_MOUSE); 
//    if Xqual and Button3Mask
//    then
//    ret:= ret or (R_MOUSE); 
//    if (U.flag and TWOBUTTONMOUSE))and((Xqual and Mod1Mask)
//    then
//    begin 
//      if ret and L_MOUSE
//      then
//      begin 
//        ret:= ret and ( not L_MOUSE); 
//        ret:= ret or (M_MOUSE); 
//      end;
//    end;
//    begin
//      result:= ret; 
//      exit;
//    end;
//  end;
//  
//  procedure getmouse(mval: psmallint); 
//  var
//  child: Window; 
//  root: Window; 
//  root_x: integer; 
//  root_y: integer; 
//  win_x: integer; 
//  win_y: integer; 
//  Xqual: UINT; 
//  begin
//    
//    
//    
//    
//    
//    
//    
//    XQueryPointer(__glutDisplay,RootWindow(__glutDisplay,__glutScreen),@root, and child,@root_x,@root_y,@win_x,@win_y,@Xqual); 
//    mval[0]:= win_x; 
//    mval[1]:= displaysizey-win_y; 
//  end;
//  
//  procedure myCopySubBuffer(x: integer;  y: integer;  sx: integer;  sy: integer); 
//  var
//  id: smallint; 
//  begin
//    
//    if xdraw=nil
//    then
//    xdraw:= glXGetCurrentDrawable(); (* clip: erg belangrijk: 1 pixel teveel en X knalt eruit *)
//    if x<0
//    then
//    x:=nil; 
//    if y<0
//    then
//    y:=nil; 
//    if x+sx>=G.curscreen.sizex
//    then
//    sx:= G.curscreen.sizex-x-1; 
//    if y+sy>=G.curscreen.sizey
//    then
//    sy:= G.curscreen.sizey-y-1; (* only mesa 3.0 (libc5 for now) will have the swap hack *)
//    {$ifdef MESA30}
//    glXCopySubBufferMESA(__glutDisplay,xdraw,x,y,sx,sy); 
//    {$else}
//    id:= curswin; 
//    mywinset(G.curscreen.mainwin); 
//    glDrawBuffer(GL_FRONT); 
//    (* op schermco's zetten *)
//    glRasterPos2i(x,y); 
//    glCopyPixels(x,y,sx,sy,GL_COLOR); 
//    glDrawBuffer(GL_BACK); 
//    mywinset(id); 
//    {$endif}
//  end;
//  
//  procedure glutGetFrontBuffer(a: integer;  b: integer;  c: integer;  d: integer); 
//  begin
//    (* fake function *)
//  end;
//  
//  procedure glutPutFrontBuffer; 
//  begin
//    (* fake function *)
//  end;
//  (* ************ OVERDRAW SUPPORT ************* *)
//  
//  procedure my_put_frontbuffer_image; 
//  var
//  gc: GC; 
//  begin
//    
//    if ximage)and(xdraw
//    then
//    begin 
//      gc:= XCreateGC(__glutDisplay,xdraw,0,0{nil}); 
//      XSetFunction(__glutDisplay,gc,GXcopy); 
//      XPutImage(__glutDisplay,xdraw,gc,ximage,0,0,ov_x,ov_y,ov_sx,ov_sy); 
//      XFreeGC(__glutDisplay,gc); 
//      XDestroyImage(ximage); 
//      ximage:=nil; 
//    end;
//  end;
//  
//  procedure my_get_frontbuffer_image(x: integer;  y: integer;  sx: integer;  sy: integer); 
//  var
//  startx: integer; 
//  starty: integer; 
//  sizex: integer; 
//  sizey: integer; 
//  begin
//    
//    
//    
//    
//    if xdraw=nil
//    then
//    xdraw:= glXGetCurrentDrawable(); 
//    if ximage=nil{nil}
//    then
//    begin 
//      (* clip: erg belangrijk: 1 pixel teveel en X knalt eruit *)
//      (* also: blenderscreen outside 'Display': then prtend the window is smaller! *)
//      if G.curscreen.startx<0
//      then
//      startx:= -G.curscreen.startx; 
//      else
//      startx:=nil; 
//      if G.curscreen.starty<0
//      then
//      starty:= -G.curscreen.starty; 
//      else
//      starty:=nil; 
//      if G.curscreen.sizex+G.curscreen.startx>displaysizex
//      then
//      sizex:= displaysizex-G.curscreen.startx; 
//      else
//      sizex:= G.curscreen.sizex; 
//      if G.curscreen.sizey+G.curscreen.starty>displaysizey
//      then
//      sizey:= displaysizey-G.curscreen.starty; 
//      else
//      sizey:= G.curscreen.sizey; (* OK: *)
//      if x<startx
//      then
//      x:= startx; 
//      if y<starty
//      then
//      y:= starty; 
//      if x+sx>=sizex
//      then
//      sx:= sizex-x-1; 
//      if y+sy>=sizey
//      then
//      sy:= sizey-y-1; 
//      ov_x:= x; 
//      ov_y:= (G.curscreen.sizey-y-sy); 
//      ov_sx:= sx; 
//      ov_sy:= sy; 
//      ximage:= XGetImage(__glutDisplay,xdraw,ov_x,ov_y,ov_sx,ov_sy,AllPlanes,ZPixmap); 
//      (* ander coordinatensysteem! *)
//      (* deze bewaren we omdat bij terugzetten een andere window aktief kan zijn geworden *)
//    end;
//  end;
//  {$endif}

    procedure warp_pointer(x, y: integer);
  begin
    glutWarpPointer(x,glutGet(GLUT_WINDOW_HEIGHT)-y);
  end;

  (*******************)
  (*******************)
  (*  Menu utilities *)

//  function mywin_inmenu: integer; 
//  begin
//    if ximage<>nil 
//    then
//    begin
//      result:= 1; 
//      exit;
//    end;
//    else
//    begin
//      result:=nil; 
//      exit;
//    end;
//  end;
//  
//  procedure mywin_getmenu_rect(x: pinteger;  y: pinteger;  sx: pinteger;  sy: pinteger); 
//  begin{*}x^:=ov_x; 
//    {*}sx^:=ov_sx; 
//    {*}sy^:=ov_sy; 
//    {$if defined(WIN32) || defined (__BeOS)}
//    {*}y^:=ov_y; 
//    {$else}
//    {*}y^:=(G.curscreen.sizey-ov_y)-ov_sy; 
//    {$endif}
//  end;
//end;

initialization
  fillbyte(swinarray, sizeof(swinarray), 0);

end.
