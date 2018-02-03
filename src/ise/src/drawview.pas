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

(* drawview.c  april 94  GRAPHICS
 *
 *
 *
 *)

unit drawview;

interface

//{$include "blender.h"}
//{$include "graphics.h"}
//{$include "sector.h"}
//{$include "render.h"}

procedure init_gl_stuff;

implementation

uses
  GL,
  drawsector;

//procedure setalpha_bgpic(bgpic: pBGpic); 
//var
//x: integer; 
//y: integer; 
//alph: integer; 
//rect: pchar; 
//begin
//  
//  
//  
//  
//  alph:= {!!!a type cast? =>} {integer(}(255.0*(1.0-bgpic.blend)); 
//  rect:= {!!!a type cast? =>} {pchar(}bgpic.rect; 
//  for{while} y:=0 to Pred(bgpic.yim) { y++}
//  do
//  begin 
//    while{for} 
//    x:= bgpic.xim; 
//    {to} x>0
//    {;}
//    dec(x); 
//    rect:= rect + (4); 
//    do
//    begin 
//      rect[3]:= alph; 
//    end;
//  end;
//end;


(* ******************************************** *)
{$ifdef IRISGL}
(* ******************************************** *)

//defmatgl: array [0..] of single = (AMBIENT,0.1,0.1,0.1,DIFFUSE,0.9,0.9,0.9,SPECULAR,0.5,0.5,0.5,EMISSION,0.0,0.0,0.0,SHININESS,35.0,LMNULL);
//defmattexgl: array [0..] of single = (AMBIENT,0.0,0.0,0.0,DIFFUSE,0.0,0.0,0.0,SPECULAR,0.0,0.0,0.0,EMISSION,2.0,2.0,2.0,SHININESS,0.0,LMNULL);
//lmodel: array [0..] of single = (AMBIENT,1.0,1.0,1.0,LOCALVIEWER,0,TWOSIDE,0,LMNULL);
//light: array [0..] of single = (LCOLOR,0.8,0.8,0.8,POSITION,-0.3,0.3,0.90,0.0,LMNULL);
//blue_light: array [0..] of single = (LCOLOR,0.5,0.5,0.8,POSITION,0.2,0.1,-1.0,0.0,LMNULL);
//
//
//procedure deflighting; 
//begin
//  lmdef(DEFMATERIAL,1,0,defmatgl); 
//  lmdef(DEFMATERIAL,2,0,defmattexgl); 
//  lmdef(DEFLIGHT,1,10,light); 
//  lmdef(DEFLIGHT,2,10,blue_light); 
//  lmdef(DEFLMODEL,1,0,lmodel); 
//end;
//
//
//procedure bindlighting; 
//begin
//  lmcolor(LMC_COLOR); 
//  lmbind(MATERIAL,1); 
//  lmbind(LIGHT0,1); 
//  lmbind(LMODEL,1); 
//  (* lmbind(LIGHT1, 2); *)
//end;
//
//
//procedure bindtexlighting; 
//begin
//  lmcolor(LMC_EMISSION); 
//  lmbind(MATERIAL,2); 
//  lmbind(LIGHT0,0); 
//end;
//
//
//
//procedure two_sided(val: integer); 
//begin
//  if val)and(lmodel[7]=1.0
// then
//  exit;
//  if val=0)and(lmodel[7]=0.0
// then
//  exit;
//  if val<>0 
// then
//  lmodel[7]:= 1.0; 
//  else
//  lmodel[7]:= 0.0; 
//  lmdef(DEFLMODEL,1,0,lmodel); 
//end;
//
//var {was static}
//maxmat: integer = 1; 
//
//
//procedure set_gl_material(nr: integer); 
//begin
//  if nr>maxmat
// then
//  nr:= maxmat; 
//  lmbind(MATERIAL,nr); 
//end;
//
//
//procedure def_gl_material(matnr: integer;  ma: pMaterial); 
//begin
//  if ma=0
// then
//  begin 
//    defmatgl[1]:= defmatgl[2]:=defmatgl[3]:=0.1; 
//    defmatgl[5]:= defmatgl[6]:=defmatgl[7]:=0.9; 
//  end;
//  else
//  begin 
//    if ma.mode and MA_SHLESS
// then
//    begin 
//      defmatgl[5]:= defmatgl[6]:=defmatgl[7]:=0.0; 
//      defmatgl[1]:= ma.r; 
//      defmatgl[2]:= ma.g; 
//      defmatgl[3]:= ma.b; 
//    end;
//    else
//    begin 
//      defmatgl[1]:= defmatgl[2]:=defmatgl[3]:=0.1; 
//      defmatgl[5]:= (ma.ref+ma.emit)*ma.r; 
//      defmatgl[6]:= (ma.ref+ma.emit)*ma.g; 
//      defmatgl[7]:= (ma.ref+ma.emit)*ma.b; 
//      defmatgl[9]:= ma.spec*ma.specr; 
//      defmatgl[10]:= ma.spec*ma.specg; 
//      defmatgl[11]:= ma.spec*ma.specb; 
//      
//    end;
//  end;
//  
//  if matnr=0
// then
//  matnr:= 1; 
//  lmdef(DEFMATERIAL,matnr,0,defmatgl); 
//end;
//
//
//procedure init_gl_materials(ob: pObject); 
//var
//ma: pMaterial; 
//a: integer; 
//begin
//  
//  
//  if ob.totcol=0
// then
//  begin 
//    def_gl_material(0,0); 
//    maxmat:= 1; 
//  end;
//  else
//  begin 
//    maxmat:= ob.totcol; 
//    for{while} a:=0 to Pred(ob.totcol) { a++}
//    do
//    begin 
//      ma:= give_current_material(ob,a+1); 
//      def_gl_material(a+1,ma); 
//    end;
//  end;
//end;
//
//
//procedure init_gl_stuff; 
//begin
//  glEnable(GL_DITHER); 
//  deflighting(); 
//  bindlighting(); 
//end;
//
//
//procedure lrectwriteRGBA(a: integer;  b: integer;  c: integer;  d: integer;  rect: puint); 
//var
//size: integer; 
//rectt: pchar; 
//rt1: pchar; 
//rt2: pchar; 
//begin
//  
//  
//  
//  
//  size:= (c-a+1)*(d-b+1); 
//  rectt:= mallocN(size*4,'temprect'); 
//  rt1:= {!!!a type cast? =>} {pchar(}rect; 
//  rt2:= rectt; 
//  
//  while size{--} dec(size); >0
//  do
//  begin 
//    rt2[3]:= rt1[0]; 
//    rt2[2]:= rt1[1]; 
//    rt2[1]:= rt1[2]; 
//    rt2[0]:= rt1[3]; 
//    rt1:= rt1 + (4); 
//    rt2:= rt2 + (4); 
//  end;
//  lrectwrite(a,b,c,d,rectt); 
//  freeN(rectt); 
//end;
//
//
//procedure draw_bgpic; 
//var
//bgpic: pBGpic; 
//ima: pImage; 
//vec: array [0..2] of single;
//fac: single;
//asp: single;
//zoom: single;
//rect: puint; 
//x1: integer; 
//y1: integer; 
//x2: integer; 
//y2: integer; 
//cx: integer; 
//cy: integer; 
//xim: integer; 
//yim: integer; 
//mval: array [0..1] of smallint; 
//str: array [0..Pred(100)] of char; 
//begin
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  bgpic:= G.vd.bgpic; 
//  if bgpic=0
// then
//  exit;
//  if bgpic.tex<>0 
// then
//  begin 
//    init_render_texture(bgpic.tex); 
//    free_unused_animimages(); 
//    ima:= bgpic.tex.ima; 
//  end_render_texture(bgpic.tex); 
//end;
//else
//begin 
//  ima:= bgpic.ima; 
//end;
//if ima=0
//then
//exit;
//if ima.ok=0
//then
//exit;
//(* plaatje testen *)
//if ima.ibuf=0
//then
//begin 
//  if bgpic.tex<>0 
// then
//  begin 
//    ima_ibuf_is_nul(bgpic.tex); 
//  end;
//  else
//  begin 
//    strcpy(str,ima.name); 
//    convertstringcode(str); 
//    waitcursor(1); 
//    ima.ibuf:= loadiffname(str,LI_rect); 
//    waitcursor(0); 
//  end;
//  if ima.ibuf=0
// then
//  begin 
//    ima.ok:= 0; 
//    exit;
//  end;
//  if bgpic.rect<>0 
// then
//  freeN(bgpic.rect); 
//  bgpic.rect:= 0; 
//end;
//
//if G.vd.persp=2
//then
//begin 
//  x1:= G.vd.pr_xmin+curarea.winrct.xmin; 
//  y1:= G.vd.pr_ymin+curarea.winrct.ymin; 
//  x2:= G.vd.pr_xmax+curarea.winrct.xmin; 
//  y2:= G.vd.pr_ymax+curarea.winrct.ymin; 
//end;
//else
//begin 
//  (* windowco berekenen *)
//  initgrabz(0.0,0.0,0.0); 
//  window_to_3d(vec,1,0); 
//  fac:= MAX3(fabs(vec[0]),fabs(vec[1]),fabs(vec[1])); 
//  fac:= 1.0 div fac; 
//  asp:= ({!!!a type cast? =>} {single(}ima.ibuf.y) div {!!!a type cast? =>} {single(}ima.ibuf.x;
//  vec[0]:= vec[1]:=vec[2]:=0.0; 
//  project_short_noclip(vec,mval); 
//  cx:= curarea.winrct.xmin+mval[0]; 
//  cy:= curarea.winrct.ymin+mval[1]; 
//  x1:= cx+fac*(bgpic.xof-bgpic.size); 
//  y1:= cy+asp*fac*(bgpic.yof-bgpic.size); 
//  x2:= cx+fac*(bgpic.xof+bgpic.size); 
//  y2:= cy+asp*fac*(bgpic.yof+bgpic.size); 
//end;
//xim:= x2-x1; 
//yim:= y2-y1; 
//zoom:= 1.0; 
//if xim<4)or(yim<4
//then
//exit;
//if xim>ima.ibuf.x
//then
//begin 
//  zoom:= xim; 
//  zoom:= zoom div ({!!!a type cast? =>} {single(}ima.ibuf.x);
//  zoom:= fceil(zoom); 
//  xim:= xim div zoom; 
//  yim:= yim div zoom; 
//end;
//
//if bgpic.rect=0)or(zoom<>bgpic.zoom)or(xim<>bgpic.xim)or(yim<>bgpic.yim
//then
//begin 
//  if bgpic.rect<>0 
// then
//  freeN(bgpic.rect); 
//  bgpic.rect:= mallocN(xim*yim*4,'bgpicrect'); 
//  scalefastrect(ima.ibuf.rect,bgpic.rect,ima.ibuf.x,ima.ibuf.y,xim,yim); 
//  bgpic.xim:= xim; 
//  bgpic.yim:= yim; 
//  bgpic.zoom:= zoom; 
//  setalpha_bgpic(bgpic); 
//  
//  (* convert_rgba_to_abgr(xim*yim,bgpic->rect); *)
//end;
//x2:= x1+zoom*(bgpic.xim-1); 
//y2:= y1+zoom*(bgpic.yim-1); (* coordinaten hoe 't op scherm komt *)
//
//(* volledige clip? *)
//if x2<curarea.winrct.xmin
//then
//exit;
//if y2<curarea.winrct.ymin
//then
//exit;
//if x1>curarea.winrct.xmax
//then
//exit;
//if y1>curarea.winrct.ymax
//then
//exit;
//(* rectwrite coordinaten *)
//xim:= xim-1; 
//yim:= yim-1; 
//rect:= bgpic.rect; 
//(* partiele clip *)
//if x1<curarea.winrct.xmin
//then
//begin 
//  cx:= curarea.winrct.xmin-x1; 
//  cx:= cx div (zoom); 
//  x1:= x1 + (zoom*cx); 
//  xim:= xim - (cx); 
//  rect:= rect + (cx); 
//  (* zorg ervoor dat de rect pixelnauwkeurig wordt neergezet *)
//end;
//if y1<curarea.winrct.ymin
//then
//begin 
//  cy:= curarea.winrct.ymin-y1; 
//  cy:= cy div (zoom); 
//  y1:= y1 + (zoom*cy); 
//  rect:= rect + (cy*bgpic.xim); 
//  yim:= yim - (cy); 
//end;
//if x2>curarea.winrct.xmax
//then
//begin 
//  cx:= x2-curarea.winrct.xmax; 
//  cx:= cx div (zoom); 
//  xim:= xim - (cx); 
//end;
//if y2>curarea.winrct.ymax
//then
//begin 
//  cy:= y2-curarea.winrct.ymax; 
//  cy:= cy div (zoom); 
//  yim:= yim - (cy); 
//end;
//if xim<=0
//then
//exit;
//if yim<=0
//then
//exit;
//
//rectzoom(zoom,zoom); 
//pixmode(PM_STRIDE,bgpic.xim); 
//
//(* geeft aan dat per scanline rectwriten, ibuf->x in de rect verder gelezen wordt *)
//blendfunction(BF_SA,BF_MSA); 
//
//lrectwriteRGBA(x1,y1,x1+xim,y1+yim,rect); 
//blendfunction(BF_ONE,BF_ZERO); 
//rectzoom(1.0,1.0); 
//pixmode(PM_STRIDE,0); 
//end;


(* ******************************************** *)
{$else}
(* ******************************************** *)

var
light_pos1: array [0..3] of single = (-0.3,0.3,0.90,0.0);
light_pos2: array [0..3] of single = (0.3,-0.3,-0.90,0.0);


procedure init_gl_stuff;
var
mat_specular: array [0..3] of single=(0.5,0.5,0.5,1.0);
mat_shininess: array [0..0] of single=(35.0);
light_col1: array [0..3] of single=(0.8,0.8,0.8,0.0);
one: single=1.0;
begin
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess);

  glLightfv(GL_LIGHT0,GL_POSITION,light_pos1);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,light_col1);
  glLightfv(GL_LIGHT0,GL_SPECULAR,mat_specular);

  glEnable(GL_LIGHT0);

  {$if defined(FREEBSD) or defined(LINUX)}
  glDisable(GL_DITHER);          (* op sgi/sun hardware and 12 bits *)
  {$endif}

  (* glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, &one); *)

  glDepthFunc(GL_LEQUAL);
  (* scaling matrices *)
  glEnable(GL_NORMALIZE);

  glShadeModel(GL_FLAT);

  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_FOG);
  glDisable(GL_LIGHTING);
  glDisable(GL_LOGIC_OP);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_TEXTURE_1D);
  glDisable(GL_TEXTURE_2D);

  glPixelTransferi(GL_MAP_COLOR,GL_FALSE);
  glPixelTransferi(GL_RED_SCALE,1);
  glPixelTransferi(GL_RED_BIAS,0);
  glPixelTransferi(GL_GREEN_SCALE,1);
  glPixelTransferi(GL_GREEN_BIAS,0);
  glPixelTransferi(GL_BLUE_SCALE,1);
  glPixelTransferi(GL_BLUE_BIAS,0);
  glPixelTransferi(GL_ALPHA_SCALE,1);
  glPixelTransferi(GL_ALPHA_BIAS,0);

  init_realtime_GL();   (* drawsector.c *)
end;



//procedure two_sided(val: integer); 
//begin
//  glLightModeliv(GL_LIGHT_MODEL_TWO_SIDE,(GLint* )@val); 
//  
//  (* twosided aan: geft errors bij x flip! *)
//end;
//
//
//procedure circf(x: single;  y: single;  rad: single);
//var
//qobj: pGLUquadricObj;
// 
//begin
//  qobj:=gluNewQuadric(); 
//  gluQuadricDrawStyle(qobj,GLU_FILL); 
//  glPushMatrix(); 
//  glTranslatef(x,y,0.); 
//  gluDisk(qobj,0.0,rad,32,1); 
//  glPopMatrix(); 
//  gluDeleteQuadric(qobj); 
//end;
//
//
//procedure circ(x: single;  y: single;  rad: single);
//var
//qobj: pGLUquadricObj;
// 
//begin
//  qobj:=gluNewQuadric(); 
//  gluQuadricDrawStyle(qobj,GLU_SILHOUETTE); 
//  glPushMatrix(); 
//  glTranslatef(x,y,0.); 
//  gluDisk(qobj,0.0,rad,32,1); 
//  glPopMatrix(); 
//  gluDeleteQuadric(qobj); 
//end;
//
//(* ********** IN ONTWIKKELING ********** *)
//
//const
//MAXMATBUF = 16; 
//var
//matbuf: array [0..Pred(MAXMATBUF),0..1,0..2] of single;
//
//
//procedure init_gl_materials(ob: pObject); 
//var
//ma: pMaterial; 
//a: integer; 
//begin
//  {defmaterial: Material; }{<= !!!5 external variable}
//  
//  
//  if ob.totcol=0
// then
//  begin 
//    matbuf[0][0][0]:= defmaterial.r; 
//    matbuf[0][0][1]:= defmaterial.g; 
//    matbuf[0][0][2]:= defmaterial.b; 
//    matbuf[0][1][0]:= defmaterial.specr; 
//    matbuf[0][1][1]:= defmaterial.specg; 
//    matbuf[0][1][2]:= defmaterial.specb; 
//    VECCOPY(matbuf[1][0],matbuf[0][0]); 
//    (* ook matnr 1, displists! *)
//    VECCOPY(matbuf[1][1],matbuf[0][1]); 
//  end;
//  for{while} a:=1 to ob.totcol { a++}
//  do
//  begin 
//    ma:= give_current_material(ob,a); 
//    if ma)and(a<MAXMATBUF
// then
//    begin 
//      (* if(ma->mode & MA_SHLESS) { *)
//      (*  *)
//      (* } *)
//      (* else { *)
//      matbuf[a][0][0]:= (ma.ref+ma.emit)*ma.r; 
//      matbuf[a][0][1]:= (ma.ref+ma.emit)*ma.g; 
//      matbuf[a][0][2]:= (ma.ref+ma.emit)*ma.b; 
//      matbuf[a][1][0]:= ma.spec*ma.specr; 
//      matbuf[a][1][1]:= ma.spec*ma.specg; 
//      matbuf[a][1][2]:= ma.spec*ma.specb; 
//      (* } *)
//    end;
//  end;
//end;
//
//
//procedure set_gl_material(nr: integer); 
//begin
//  if nr<MAXMATBUF
// then
//  begin 
//    if matbuf[nr][0]<>0 
// then
//    begin 
//      glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,matbuf[nr][0]); 
//      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,matbuf[nr][1]); 
//    end;
//  end;
//end;
//
//
//
//procedure draw_bgpic; 
//var
//bgpic: pBGpic; 
//ima: pImage; 
//vec: array [0..2] of single;
//fac: single;
//asp: single;
//zoomx: single;
//zoomy: single;
//rect: puint; 
//x1: integer; 
//y1: integer; 
//x2: integer; 
//y2: integer; 
//cx: integer; 
//cy: integer; 
//xim: integer; 
//yim: integer; 
//mval: array [0..1] of smallint; 
//str: array [0..Pred(FILE_MAXDIR+FILE_MAXFILE)] of char; 
//begin
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  bgpic:= G.vd.bgpic; 
//  if bgpic=0
// then
//  exit;
//  if bgpic.tex<>0 
// then
//  begin 
//    init_render_texture(bgpic.tex); 
//    free_unused_animimages(); 
//    ima:= bgpic.tex.ima; 
//  end_render_texture(bgpic.tex); 
//end;
//else
//begin 
//  ima:= bgpic.ima; 
//end;
//if ima=0
//then
//exit;
//if ima.ok=0
//then
//exit;
//(* plaatje testen *)
//if ima.ibuf=0
//then
//begin 
//  if bgpic.rect<>0 
// then
//  freeN(bgpic.rect); 
//  bgpic.rect:= 0; 
//  if bgpic.tex<>0 
// then
//  begin 
//    ima_ibuf_is_nul(bgpic.tex); 
//  end;
//  else
//  begin 
//    strcpy(str,ima.name); 
//    convertstringcode(str); 
//    waitcursor(1); 
//    ima.ibuf:= loadiffname(str,LI_rect); 
//    waitcursor(0); 
//  end;
//  if ima.ibuf=0
// then
//  begin 
//    ima.ok:= 0; 
//    exit;
//  end;
//end;
//
//if bgpic.rect=0
//then
//begin 
//  bgpic.rect:= dupallocN(ima.ibuf.rect); 
//  bgpic.xim:= ima.ibuf.x; 
//  bgpic.yim:= ima.ibuf.y; 
//  setalpha_bgpic(bgpic); 
//end;
//
//if G.vd.persp=2
//then
//begin 
//  x1:= G.vd.pr_xmin; 
//  y1:= G.vd.pr_ymin; 
//  x2:= G.vd.pr_xmax; 
//  y2:= G.vd.pr_ymax; 
//end;
//else
//begin 
//  (* windowco berekenen *)
//  initgrabz(0.0,0.0,0.0); 
//  window_to_3d(vec,1,0); 
//  fac:= MAX3(fabs(vec[0]),fabs(vec[1]),fabs(vec[1])); 
//  fac:= 1.0 div fac; 
//  asp:= ({!!!a type cast? =>} {single(}ima.ibuf.y) div {!!!a type cast? =>} {single(}ima.ibuf.x;
//  vec[0]:= vec[1]:=vec[2]:=0.0; 
//  project_short_noclip(vec,mval); 
//  cx:= mval[0]; 
//  cy:= mval[1]; 
//  x1:= cx+fac*(bgpic.xof-bgpic.size); 
//  y1:= cy+asp*fac*(bgpic.yof-bgpic.size); 
//  x2:= cx+fac*(bgpic.xof+bgpic.size); 
//  y2:= cy+asp*fac*(bgpic.yof+bgpic.size); 
//end;
//(* volledige clip? *)
//if x2<0
//then
//exit;
//if y2<0
//then
//exit;
//if x1>curarea.winx
//then
//exit;
//if y1>curarea.winy
//then
//exit;
//zoomx:= x2-x1; 
//zoomx:= zoomx div ({!!!a type cast? =>} {single(}ima.ibuf.x);
//zoomy:= y2-y1; 
//zoomy:= zoomy div ({!!!a type cast? =>} {single(}ima.ibuf.y);
//glEnable(GL_BLEND); 
//
//if G.zbuf<>0 
//then
//glDisable(GL_DEPTH_TEST); 
//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
//
//rectwrite_part(curarea.winrct.xmin,curarea.winrct.ymin,curarea.winrct.xmax,curarea.winrct.ymax,x1+curarea.winrct.xmin,y1+curarea.winrct.ymin,ima.ibuf.x,ima.ibuf.y,zoomx,zoomy,bgpic.rect); 
//glBlendFunc(GL_ONE,GL_ZERO); 
//
//glDisable(GL_BLEND); 
//if G.zbuf<>0 
//then
//glEnable(GL_DEPTH_TEST); 
//end;

(* ******************************************** *)
{$endif}
(* ******************************************** *)



//procedure timestr(time: integer;  str: pchar); 
//var
//temp: single;
//hr: smallint; 
//min: smallint; 
//sec: smallint; 
//hun: smallint; 
//begin(* formaat 00:00:00.00 (hr:min:sec) string moet 12 lang *)
//  
//  
//  
//  
//  
//  temp:= ({!!!a type cast? =>} {single(}time) div (100.0);
//  min:= ffloor(temp div 60.0); 
//  hr:= min div 60; 
//  min:= min - (60*hr); 
//  temp:= temp - ({!!!a type cast? =>} {single(}60*min);
//  sec:= ffloor(temp); 
//  temp:= temp - ({!!!a type cast? =>} {single(}sec);
//  hun:= ffloor(100*temp); 
//  if hr<>0 
// then
//  sprintf(str,'%.2d:%.2d:%.2d.%.2d',hr,min,sec,hun); 
//  else
//  sprintf(str,'%.2d:%.2d.%.2d',min,sec,hun); 
//  str[11]:= 0; 
//end;
//
//
//
//procedure drawgrid; 
//var
//wx: single;
//wy: single;
//x: single;
//y: single;
//fz: single;
//fw: single;
//fx: single;
//fy: single;
//dx: single;
//dy: single;
//vec4: array [0..3] of single;
//begin(* extern short bgpicmode; *)
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  vec4[0]:= vec4[1]:=vec4[2]:=0.0; 
//  vec4[3]:= 1.0; 
//  Mat4MulVec4fl(G.vd.persmat,vec4); 
//  fx:= vec4[0]; 
//  fy:= vec4[1]; 
//  fz:= vec4[2]; 
//  fw:= vec4[3]; 
//  wx:= (curarea.winx div 2.0); 
//  wy:= (curarea.winy div 2.0); 
//  x:= (wx)*fx div fw; 
//  y:= (wy)*fy div fw; 
//  vec4[0]:= vec4[1]:=G.vd.grid; 
//  vec4[2]:= 0.0; 
//  vec4[3]:= 1.0; 
//  Mat4MulVec4fl(G.vd.persmat,vec4); 
//  
//  (* ivm afrondfoutjes, grid op verkeerde plek *)
//  
//  
//  fx:= vec4[0]; 
//  fy:= vec4[1]; 
//  fz:= vec4[2]; 
//  fw:= vec4[3]; 
//  dx:= fabs(x-(wx)*fx div fw); 
//  if dx=0
// then
//  dx:= fabs(y-(wy)*fy div fw); 
//  if dx<6.0
// then
//  begin 
//    dx:= dx * (10.0); 
//    setlinestyle(3); 
//    if dx<6.0
// then
//    begin 
//      dx:= dx * (10.0); 
//      if dx<6.0
// then
//      begin 
//        setlinestyle(0); 
//        exit;
//      end;
//    end;
//  end;
//  persp(0); 
//  cpack($505050); 
//  
//  x:= x + ((wx)); 
//  y:= y + ((wy)); 
//  fx:= x div dx; 
//  fx:= x-dx*ffloor(fx); 
//  while fx<curarea.winx
//  do
//  begin 
//    fdrawline(fx,0.0,fx,{!!!a type cast? =>} {single(}curarea.winy);
//    fx:= fx + (dx); 
//  end;
//  fy:= y div dx; 
//  fy:= y-dx*ffloor(fy); 
//  
//  while fy<curarea.winy
//  do
//  begin 
//    fdrawline(0.0,fy,{!!!a type cast? =>} {single(}curarea.winx,fy);
//    fy:= fy + (dx); 
//  end;
//  
//  (* kruis in midden *)
//  if G.vd.view=3
// then
//  cpack($A0D0A0); 
//  else
//  (* y-as *)
//  cpack($A0A0C0); 
//  fdrawline(0.0,y,{!!!a type cast? =>} {single(}curarea.winx,y);
//  (* x-as *)
//  
//  if G.vd.view=7
// then
//  cpack($A0D0A0); 
//  else
//  (* y-as *)
//  cpack($D0A0A0); 
//  fdrawline(x,0.0,x,{!!!a type cast? =>} {single(}curarea.winy);
//  (* z-as *)
//  
//  persp(1); 
//  
//  setlinestyle(0); 
//end;
//
//
//
//procedure drawfloor; 
//var
//vd: pView3D; 
//vert: array [0..2] of single;
//grid: single;
//a: integer; 
//b: integer; 
//gridlines: integer; 
//begin
//  
//  
//  
//  
//  
//  
//  vd:= curarea.spacedata.first; 
//  vert[2]:= 0.0; 
//  
//  if vd.gridlines<3
// then
//  exit;
//  
//  gridlines:= vd.gridlines div 2; 
//  grid:= gridlines*vd.grid; 
//  cpack($404040); 
//  
//  for{while} a:=-gridlines to gridlines {a++}
//  do
//  begin 
//    
//    if a=0
// then
//    begin 
//      if vd.persp=0
// then
//      cpack($A0D0A0); 
//      else
//      cpack($402000); 
//    end;
//    else
//    if a=1
// then
//    begin 
//      cpack($404040); 
//    end;
//    glBegin(GL_LINE_STRIP); 
//    vert[0]:= a*vd.grid; 
//    vert[1]:= grid; 
//    glVertex3fv(vert); 
//    vert[1]:= -grid; 
//    glVertex3fv(vert); 
//    glEnd(); 
//  end;
//  cpack($404040); 
//  for{while} a:=-gridlines to gridlines {a++}
//  do
//  begin 
//    if a=0
// then
//    begin 
//      if vd.persp=0
// then
//      cpack($A0A0D0); 
//      else
//      cpack(0); 
//    end;
//    else
//    if a=1
// then
//    begin 
//      cpack($404040); 
//    end;
//    glBegin(GL_LINE_STRIP); 
//    vert[1]:= a*vd.grid; 
//    vert[0]:= grid; 
//    glVertex3fv(vert); 
//    vert[0]:= -grid; 
//    glVertex3fv(vert); 
//    glEnd(); 
//  end;
//end;
//
//
//
//procedure drawcursor; 
//begin
//  if G.f and G_PLAYANIM
// then
//  exit;
//  project_short(give_cursor(),@G.vd.mx); 
//  G.vd.mxo:= G.vd.mx; 
//  G.vd.myo:= G.vd.my; 
//  
//  if G.vd.mx<>3200
// then
//  begin 
//    setlinestyle(0); 
//    cpack($FF); 
//    circ({!!!a type cast? =>} {single(}G.vd.mx,{!!!a type cast? =>} {single(}G.vd.my,10.0);
//    setlinestyle(4); 
//    cpack($FFFFFF); 
//    circ({!!!a type cast? =>} {single(}G.vd.mx,{!!!a type cast? =>} {single(}G.vd.my,10.0);
//    setlinestyle(0); 
//    cpack($0); 
//    sdrawline(G.vd.mx-20,G.vd.my,G.vd.mx-5,G.vd.my); 
//    sdrawline(G.vd.mx+5,G.vd.my,G.vd.mx+20,G.vd.my); 
//    sdrawline(G.vd.mx,G.vd.my-20,G.vd.mx,G.vd.my-5); 
//    sdrawline(G.vd.mx,G.vd.my+5,G.vd.mx,G.vd.my+20); 
//  end;
//end;
//
//
//procedure calc_viewborder; 
//var
//a: single;
//fac: single;
//wx: single;
//wy: single;
//x1: single;
//x2: single;
//y1: single;
//y2: single;
//imax: single;
//imay: single;
//width: single;
//begin
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  wx:= curarea.winx; 
//  wy:= curarea.winy; 
//  imax:= G.scene.r.xsch; 
//  imay:= G.scene.r.ysch; 
//  fac:= ({!!!a type cast? =>} {single(}imay*G.scene.r.yasp) div ({!!!a type cast? =>} {single(}imax*G.scene.r.xasp);
//  imay:= fac*imax; (* liggend plaatje *)
//  if imax>imay
// then
//  begin 
//    if wx<wy
// then
//    width:= wy; 
//    else
//    width:= wx; 
//    fac:= width div (2.0*imax); 
//    x1:= 0.5*wx-0.25*width; 
//    x2:= 0.5*wx+0.25*width; 
//    y1:= 0.5*wy-0.5*fac*imay; 
//    y2:= 0.5*wy+0.5*fac*imay; 
//  end;
//  else
//  begin 
//    if wx<wy
// then
//    width:= wy; 
//    else
//    width:= wx; 
//    fac:= width div (2.0*imay); 
//    y1:= 0.5*wy-0.25*width; 
//    y2:= 0.5*wy+0.25*width; 
//    x1:= 0.5*wx-0.5*fac*imax; 
//    x2:= 0.5*wx+0.5*fac*imax; 
//    
//  end;
//  fac:= (1.41421+({!!!a type cast? =>} {single(}G.vd.camzoom) div 50.0);
//  fac:= fac * (fac); 
//  width:= (x2-x1) div 4.0; 
//  a:= (x2+x1) div 2.0; 
//  x1:= a-width*fac; 
//  x2:= a+width*fac; 
//  width:= (y2-y1) div 4.0; 
//  a:= (y2+y1) div 2.0; 
//  y1:= a-width*fac; 
//  y2:= a+width*fac; 
//  G.vd.pr_xmin:= fceil(x1); 
//  G.vd.pr_ymin:= fceil(y1); 
//  G.vd.pr_xmax:= fceil(x2); 
//  G.vd.pr_ymax:= fceil(y2); 
//  
//  
//  (* zoom van viewborder *)
//  
//  (* deze getallen voor renderdisplay *)
//end;
//
//
//procedure drawviewborder; 
//var
//fac: single;
//a: single;
//x1: single;
//x2: single;
//y1: single;
//y2: single;
//x3: single;
//y3: single;
//x4: single;
//y4: single;
//begin
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  x1:= G.vd.pr_xmin; 
//  y1:= G.vd.pr_ymin; 
//  x2:= G.vd.pr_xmax; 
//  y2:= G.vd.pr_ymax; 
//  setlinestyle(3); 
//  
//  (* rand *)
//  cpack(0); 
//  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); 
//  glRectf(x1+1,y1-1,x2+1,y2-1); 
//  cpack($FFFFFF); 
//  glRectf(x1,y1,x2,y2); 
//  
//  (* border *)
//  if G.scene.r.mode and R_BORDER
// then
//  begin 
//    cpack(0); 
//    x3:= x1+G.scene.r.border.xmin*(x2-x1); 
//    y3:= y1+G.scene.r.border.ymin*(y2-y1); 
//    x4:= x1+G.scene.r.border.xmax*(x2-x1); 
//    y4:= y1+G.scene.r.border.ymax*(y2-y1); 
//    glRectf(x3+1,y3-1,x4+1,y4-1); 
//    cpack($4040FF); 
//    glRectf(x3,y3,x4,y4); 
//  end;
//  fac:= 0.1; 
//  a:= fac*(x2-x1); 
//  x1:= x1 + (a); 
//  x2:= x2 - (a); 
//  a:= fac*(y2-y1); 
//  y1:= y1 + (a); 
//  y2:= y2 - (a); 
//  cpack(0); 
//  
//  (* safetykader *)
//  
//  
//  
//  glRectf(x1+1,y1-1,x2+1,y2-1); 
//  cpack($FFFFFF); 
//  glRectf(x1,y1,x2,y2); 
//  setlinestyle(0); 
//  
//  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); 
//end;
//
//
//type
//Base = record
//end;
//
//procedure backdrawview3d(test: integer); 
//var
//tel: integer;
// 
//kleur: uint; 
//{$if defined(WINDOWS) or defined(MESA31)}
//begin
//  
//  tel:=1; 
//  if G.f and (G_VERTEXPAINT or G_FACESELECT)
// then
//  ; 
//  else
//  begin 
//    G.vd.flag:= G.vd.flag and ( not V3D_NEEDBACKBUFDRAW); 
//    exit;
//  end;
//  {$endif}
//  
//  if G.vd.flag and V3D_NEEDBACKBUFDRAW
// then
//  ; 
//  else
//  exit;
//  if G.obedit<>0 
// then
//  begin 
//    G.vd.flag:= G.vd.flag and ( not V3D_NEEDBACKBUFDRAW); 
//    exit;
//  end;
//  if test<>0 
// then
//  begin 
//    if qtest()
// then
//    begin 
//      addafterqueue(curarea.win,BACKBUFDRAW,1); 
//      exit;
//    end;
//  end;
//  
//  {$if defined(__sgi) or defined(__SUN) or defined(BEOS) or defined(WINDOWS) or defined(MESA31)}
//  glDisable(GL_DITHER); 
//  {$endif}
//  
//  if G.vd.drawtype>OB_WIRE
// then
//  G.zbuf:= LongBool(1); 
//  curarea.win_swap:= curarea.win_swap and ( not WIN_BACK_OK); 
//  glClearColor(0.0,0.0,0.0,0.0); 
//  if G.zbuf<>0 
// then
//  begin 
//    glEnable(GL_DEPTH_TEST); 
//    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT); 
//  end;
//  else
//  begin 
//    glClear(GL_COLOR_BUFFER_BIT); 
//    glDisable(GL_DEPTH_TEST); 
//  end;
//  G.f:= G.f or (G_BACKBUFSEL); 
//  if G.f and (G_VERTEXPAINT or G_FACESELECT)
// then
//  begin 
//    base:= BASACT; 
//    if base)and((base.lay)and(G.vd.lay)
// then
//    begin 
//      draw_object(base); 
//    end;
//  end;
//  else
//  begin 
//    
//    base:= FIRSTBASE; 
//    while base
//    do
//    begin 
//      (* elke base ivm meerdere windows *)
//      base.selcol:= $070707 or (((tel and $F00) shl 12)+((tel and $F0) shl 8)+((tel and $F) shl 4)); 
//      inc(tel); 
//      if base.lay and G.vd.lay
// then
//      begin 
//        if test<>0 
// then
//        begin 
//          if qtest()
// then
//          begin 
//            addafterqueue(curarea.win,BACKBUFDRAW,1); 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//        end;
//        cpack(base.selcol); 
//        draw_object(base); 
//      end;
//      base:= base.next; 
//    end;
//  end;
//  if base=0
// then
//  G.vd.flag:= G.vd.flag and ( not V3D_NEEDBACKBUFDRAW); 
//  G.f:= G.f and ( not G_BACKBUFSEL); 
//  G.zbuf:= LongBool(0); 
//  glDisable(GL_DEPTH_TEST); 
//  
//  
//  {$if defined(__sgi) or defined(__SUN) or defined(BEOS) or defined (WINDOWS) or defined(MESA31)}
//  glEnable(GL_DITHER); 
//  {$endif}
//end;
//
//
//procedure drawname(ob: pObject); 
//var
//str: array [0..Pred(8)] of char; 
//begin
//  
//  cpack($404040); 
//  glRasterPos3f(0.0,0.0,0.0); 
//  str[0]:= ' '; 
//  str[1]:= 0; 
//  fmsetfont(G.font); 
//  fmprstr(str); 
//  fmprstr(ob.id.name+2); 
//end;
//
//
//
//procedure drawview3d_simul(make_disp: integer); 
//var
//ob: pObject; 
//se: pSector; 
//lf: pLife; 
//lfl: pLife; 
//ma: pMaterial; 
//col: uint; 
//b: integer; 
//a: integer; 
//flag: integer; 
//(* hier doen: anders wordt in header getekend *)
//begin
//  
//  
//  
//  
//  
//  
//  
//  
//  areawinset(curarea.win); 
//  setwinmatrixview3d(0); 
//  
//  setviewmatrixview3d(); 
//  (* 0= geen pick rect *)
//  
//  Mat4MulMat4(G.vd.persmat,G.vd.viewmat,curarea.winmat); 
//  Mat4Invert(G.vd.persinv,G.vd.persmat); 
//  Mat4Invert(G.vd.viewinv,G.vd.viewmat); 
//  
//  if G.vd.drawtype>OB_WIRE
// then
//  begin 
//    G.zbuf:= LongBool(1); 
//    glEnable(GL_DEPTH_TEST); 
//    if G.scene.world<>0 
// then
//    glClearColor(G.scene.world.horr,G.scene.world.horg,G.scene.world.horb,0.0); 
//    else
//    glClearColor(0.4375,0.4375,0.4375,0.0); 
//    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT); 
//  end;
//  else
//  begin 
//    glClearColor(0.4375,0.4375,0.4375,0.0); 
//    glClear(GL_COLOR_BUFFER_BIT); 
//  end;
//  loadmatrix(G.vd.viewmat); 
//  
//  (* extern: camera tekenen *)
//  if G.vd.persp<>2
// then
//  begin 
//    ob:= G.scene.camera; 
//    if ob)and((ob.lay and G.vd.lay)
// then
//    begin 
//      cpack($0); 
//      multmatrix(ob.obmat); 
//      drawcamera(ob); 
//      loadmatrix(G.vd.viewmat); 
//    end;
//  end;
//  a:= G.totsect; 
//  while a{--} dec(a); >0
//  do
//  begin 
//    ob:= G.sectorbuf[a].ob; 
//    multmatrix(ob.obmat); 
//    
//    cpack($0); 
//    drawsector(ob,MIN2(ob.dt,G.vd.drawtype),0); 
//    loadmatrix(G.vd.viewmat); 
//    se:= ob.data; 
//    if se.lbuf.tot<>0 
// then
//    begin 
//      (* here was an de->depth test... for psx speedup i presume *)
//      for{while} b:=0 to Pred(se.lbuf.tot) { b++}
//      do
//      begin 
//        ob:= se.lbuf.ob[b]; 
//        if ob.lay and G.vd.lay
// then
//        begin 
//          lf:= ob.data; 
//          if lf.dflag and LF_DONTDRAW
// then
//          ; 
//          else
//          begin 
//            multmatrix(ob.obmat); 
//            col:= 0; 
//            if ma:=lf.contact
// then
//            col:= rgb_to_cpack(ma.r,ma.g,ma.b); 
//            if lf.flag and LF_DRAWNEAR
// then
//            col:= ($F0A020); 
//            drawlife(ob,MIN2(ob.dt,G.vd.drawtype),col); 
//            if G.vd.drawtype<>OB_TEXTURE
// then
//            begin 
//              if ob.dtx and OB_DRAWNAME
// then
//              drawname(ob); 
//              if ob.dtx and OB_AXIS
// then
//              drawaxes(lf.axsize); 
//            end;
//            loadmatrix(G.vd.viewmat); 
//          end;
//        end;
//      end;
//    end;
//  end;
//  a:= G.totlife; 
//  while a{--} dec(a); 
//  do
//  begin 
//    ob:= G.lifebuf[a]; 
//    if ob.type=OB_LIFE)and((ob.lay and G.vd.lay)
// then
//    begin 
//      lf:= ob.data; 
//      if lf.dflag and LF_DONTDRAW
// then
//      ; 
//      else
//      begin 
//        multmatrix(ob.obmat); 
//        if G.vd.drawtype<>OB_TEXTURE
// then
//        begin 
//          col:= 0; 
//          if ma:=lf.contact
// then
//          col:= rgb_to_cpack(ma.r,ma.g,ma.b); 
//          if lf.flag and LF_DRAWNEAR
// then
//          col:= ($F0A020); 
//          drawlife(ob,MIN2(ob.dt,G.vd.drawtype),col); 
//          if ob.dtx and OB_DRAWNAME
// then
//          drawname(ob); 
//          if ob.dtx and OB_AXIS
// then
//          drawaxes(lf.axsize); 
//        end;
//        else
//        drawlife(ob,MIN2(ob.dt,G.vd.drawtype),0); 
//        loadmatrix(G.vd.viewmat); 
//      end;
//      
//      if lf.type=LF_DYNAMIC
// then
//      begin 
//        for{while} b:=0 to Pred(lf.links.tot) { b++}
//        do
//        begin 
//          ob:= lf.links.ob[b]; 
//          if ob.type=OB_LIFE
// then
//          begin 
//            if ob.lay and G.vd.lay
// then
//            begin 
//              lfl:= ob.data; 
//              if lfl.dflag and LF_DONTDRAW
// then
//              ; 
//              else
//              begin 
//                multmatrix(ob.obmat); 
//                if G.vd.drawtype<>OB_TEXTURE
// then
//                begin 
//                  col:= 0; 
//                  if ma:=lfl.contact
// then
//                  col:= rgb_to_cpack(ma.r,ma.g,ma.b); 
//                  if lfl.flag and LF_DRAWNEAR
// then
//                  col:= ($F0A020); 
//                  drawlife(ob,MIN2(ob.dt,G.vd.drawtype),col); 
//                  if ob.dtx and OB_DRAWNAME
// then
//                  drawname(ob); 
//                  if ob.dtx and OB_AXIS
// then
//                  drawaxes(lfl.axsize); 
//                end;
//                else
//                drawlife(ob,MIN2(ob.dt,G.vd.drawtype),0); 
//                loadmatrix(G.vd.viewmat); 
//              end;
//            end;
//          end;
//        end;
//      end;
//    end;
//  end;
//  draw_tface_meshes_tra(); 
//  
//  
//  if G.zbuf<>0 
// then
//  begin 
//    G.zbuf:= LongBool(0); 
//    glDisable(GL_DEPTH_TEST); 
//  end;
//  if G.vd.drawtype<>OB_TEXTURE
// then
//  begin 
//    if G.vd.persp>1
// then
//    begin 
//      (* persp(0); *)
//      (* drawviewborder(); *)
//      (* persp(1); *)
//    end;
//  end;
//  draw_gamedebug_info(); 
//  
//  (* life.c *)
//  curarea.win_swap:= WIN_BACK_OK; 
//end;
//
//
//procedure drawview3d; 
//var
//base: pBase; 
//ob: pObject; 
//tbase: Base; 
//tbase: Base; 
//(* altijd eerst original tekenen vanwege make_displist *)
//begin
//  
//  
//  setwinmatrixview3d(0); 
//  setviewmatrixview3d(); 
//  (* 0= geen pick rect *)
//  
//  Mat4MulMat4(G.vd.persmat,G.vd.viewmat,curarea.winmat); 
//  
//  Mat4Invert(G.vd.persinv,G.vd.persmat); 
//  Mat4Invert(G.vd.viewinv,G.vd.viewmat); 
//  
//  if G.vd.drawtype>OB_WIRE
// then
//  begin 
//    G.zbuf:= LongBool(1); 
//    glEnable(GL_DEPTH_TEST); 
//    if G.f and G_SIMULATION
// then
//    begin 
//      glClearColor(0.0,0.0,0.0,0.0); 
//    end;
//    else
//    begin 
//      glClearColor(0.4375,0.4375,0.4375,0.0); 
//    end;
//    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT); 
//    glLoadIdentity(); 
//  end;
//  else
//  begin 
//    glClearColor(0.4375,0.4375,0.4375,0.0); 
//    glClear(GL_COLOR_BUFFER_BIT); 
//  end;
//  loadmatrix(G.vd.viewmat); 
//  
//  if G.vd.view=0)or(G.vd.persp<>0
// then
//  begin 
//    drawfloor(); 
//    if G.vd.persp=2
// then
//    begin 
//      if G.scene.world<>0 
// then
//      begin 
//        if G.scene.world.mode and WO_STARS
// then
//        make_stars(1); 
//      end;
//      calc_viewborder(); 
//      if G.vd.flag and V3D_DISPBGPIC
// then
//      draw_bgpic(); 
//    end;
//  end;
//  else
//  begin 
//    drawgrid(); 
//    
//    if G.vd.flag and V3D_DISPBGPIC
// then
//    begin 
//      draw_bgpic(); 
//    end;
//  end;
//  (* eerst set tekenen *)
//  if G.scene.set<>0 
// then
//  begin 
//    (* patchje: kleur blijft constant *)
//    G.f:= G.f or (G_PICKSEL); 
//    base:= G.scene.set.base.first; 
//    while base
//    do
//    begin 
//      if G.vd.lay and base.lay
// then
//      begin 
//        where_is_object(base.object); 
//        cpack($404040); 
//        
//        draw_object(base); 
//        
//        if base.object.transflag and OB_DUPLI
// then
//        begin 
//          {duplilist: ListBase; }{<= !!!5 external variable}
//          
//          tbase:= {*}base^; 
//          tbase.flag:= OB_FROMDUPLI; 
//          make_duplilist(G.scene.set,base.object); 
//          ob:= duplilist.first; 
//          while ob
//          do
//          begin 
//            tbase.object:= ob; 
//            draw_object(@tbase); 
//            ob:= ob.id.next; 
//          end;
//          free_duplilist(); 
//        end;
//      end;
//      base:= base.next; 
//    end;
//    G.f:= G.f and ( not G_PICKSEL); 
//  end;
//  base:= G.scene.base.first; (* SILLY CODE!!!! *)
//  (* See next silly code... why is the same code
//   * ~ duplicated twice, and then this silly if(FALSE)
//   * in part... wacky! and bad!
//   *)
//  (* eerst niet selected en dupli's *)
//  while base
//  do
//  begin 
//    if G.vd.lay and base.lay
// then
//    begin 
//      where_is_object(base.object); 
//      
//      if LongBool(0))and(base.object.transflag and OB_DUPLI
// then
//      begin 
//        {duplilist: ListBase; }{<= !!!5 external variable}
//        draw_object(base); 
//        G.f:= G.f or (G_PICKSEL); 
//        cpack($404040); 
//        
//        (* patchje: kleur blijft constant *)
//        tbase.flag:= OB_FROMDUPLI; 
//        make_duplilist(G.scene,base.object); 
//        ob:= duplilist.first; 
//        while ob
//        do
//        begin 
//          tbase.object:= ob; 
//          draw_object(@tbase); 
//          ob:= ob.id.next; 
//        end;
//        free_duplilist(); 
//        G.f:= G.f and ( not G_PICKSEL); 
//      end;
//      else
//      if (base.flag and SELECT)=0
// then
//      begin 
//        draw_object(base); 
//      end;
//    end;
//    base:= base.next; 
//  end;
//  base:= G.scene.base.first; (*  selected *)
//  while base
//  do
//  begin 
//    
//    function TESTBASE{!!!3 unknown typedef}: if; 
//    begin
//      draw_object(base); 
//    end
//    base:= base.next; 
//  end;
//  base:= G.scene.base.first; 
//  (* SILLY CODE!!!! *)
//  (* dupli's, als laatste om zeker te zijn de displisten zijn ok *)
//  while base
//  do
//  begin 
//    if G.vd.lay and base.lay
// then
//    begin 
//      if base.object.transflag and OB_DUPLI
// then
//      begin 
//        {duplilist: ListBase; }{<= !!!5 external variable}
//        var
//        tbase: Base; 
//        (* patchje: kleur blijft constant *)
//        G.f:= G.f or (G_PICKSEL); 
//        cpack($404040); 
//        tbase.flag:= OB_FROMDUPLI; 
//        make_duplilist(G.scene,base.object); 
//        ob:= duplilist.first; 
//        while ob
//        do
//        begin 
//          tbase.object:= ob; 
//          draw_object(@tbase); 
//          ob:= ob.id.next; 
//        end;
//        free_duplilist(); 
//        G.f:= G.f and ( not G_PICKSEL); 
//      end;
//    end;
//    base:= base.next; 
//  end;
//  
//  
//  if G.scene.radio<>0 
// then
//  rad_drawall(); 
//  persp(0); 
//  (* radview.c *)
//  if G.vd.persp>1
// then
//  drawviewborder(); 
//  drawcursor(); 
//  persp(1); 
//  G.vd.flag:= G.vd.flag and ( not V3D_DISPIMAGE); 
//  curarea.win_swap:= WIN_BACK_OK; 
//  if G.zbuf<>0 
// then
//  begin 
//    G.zbuf:= LongBool(0); 
//    glDisable(GL_DEPTH_TEST); 
//  end;
//  {$if defined(WINDOWS) or defined(MESA31)}
//  
//  if G.f and (G_VERTEXPAINT or G_FACESELECT)
// then
//  begin 
//    G.vd.flag:= G.vd.flag or (V3D_NEEDBACKBUFDRAW); 
//    addafterqueue(curarea.win,BACKBUFDRAW,1); 
//  end;
//  
//  {$else}
//  G.vd.flag:= G.vd.flag or (V3D_NEEDBACKBUFDRAW); 
//  addafterqueue(curarea.win,BACKBUFDRAW,1); 
//  {$endif}
//end;
//
//
//procedure drawview3d_render; 
//var
//base: pBase; 
//ob: pObject; 
//flag: integer; 
//tbase: Base; 
//tbase: Base; (* altijd eerst original tekenen vanwege make_displist *)
//begin
//  {v3d_windowmode: smallint; }{<= !!!5 external variable}
//  
//  
//  
//  winset(R.win); 
//  v3d_windowmode:= 1; 
//  setwinmatrixview3d(0); 
//  v3d_windowmode:= 0; 
//  glMatrixMode(GL_PROJECTION); 
//  glLoadMatrixf(R.winmat); 
//  glMatrixMode(GL_MODELVIEW); 
//  setviewmatrixview3d(); 
//  glLoadMatrixf(G.vd.viewmat); 
//  Mat4MulMat4(G.vd.persmat,G.vd.viewmat,R.winmat); 
//  
//  Mat4Invert(G.vd.persinv,G.vd.persmat); 
//  Mat4Invert(G.vd.viewinv,G.vd.viewmat); 
//  
//  if G.vd.drawtype>OB_WIRE
// then
//  begin 
//    G.zbuf:= LongBool(1); 
//    glEnable(GL_DEPTH_TEST); 
//  end;
//  
//  if G.scene.world<>0 
// then
//  glClearColor(G.scene.world.horr,G.scene.world.horg,G.scene.world.horb,0.0); 
//  else
//  glClearColor(0.4375,0.4375,0.4375,0.0); 
//  glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT); 
//  glLoadIdentity(); 
//  loadmatrix(G.vd.viewmat); 
//  
//  G.f:= G.f or (G_SIMULATION); 
//  do_all_ipos(); 
//  (* abuse! to make sure it doesnt draw the helpstuff *)
//  
//  do_all_scripts(SCRIPT_FRAMECHANGED); 
//  do_all_keys(); 
//  do_all_ikas(); 
//  test_all_displists(); 
//  ob:= G.main.object.first; 
//  (* niet erg nette calc_ipo en where_is forceer *)
//  while ob
//  do
//  begin 
//    ob.ctime:= -123.456; 
//    ob:= ob.id.next; 
//  end;
//  
//  (* eerst set tekenen *)
//  if G.scene.set<>0 
// then
//  begin 
//    (* patchje: kleur blijft constant *)
//    G.f:= G.f or (G_PICKSEL); 
//    base:= G.scene.set.base.first; 
//    while base
//    do
//    begin 
//      if G.vd.lay and base.lay
// then
//      begin 
//        
//        function ELEM3{!!!3 unknown typedef}: if; 
//        else
//        begin 
//          where_is_object(base.object); 
//          cpack($404040); 
//          draw_object(base); 
//          if base.object.transflag and OB_DUPLI
// then
//          begin 
//            {duplilist: ListBase; }{<= !!!5 external variable}
//            
//            tbase.flag:= OB_FROMDUPLI; 
//            make_duplilist(G.scene.set,base.object); 
//            ob:= duplilist.first; 
//            while ob
//            do
//            begin 
//              tbase.object:= ob; 
//              draw_object(@tbase); 
//              ob:= ob.id.next; 
//            end;
//            free_duplilist(); 
//          end;
//        end;
//      end;
//      base:= base.next; 
//    end;
//    G.f:= G.f and ( not G_PICKSEL); 
//  end;
//  base:= G.scene.base.first; (* eerst niet selected en dupli's *)
//  while base
//  do
//  begin 
//    if G.vd.lay and base.lay
// then
//    begin 
//      
//      function ELEM3{!!!3 unknown typedef}: if; 
//      else
//      begin 
//        where_is_object(base.object); 
//        if base.object.transflag and OB_DUPLI
// then
//        begin 
//          {duplilist: ListBase; }{<= !!!5 external variable}
//          draw_object(base); 
//          G.f:= G.f or (G_PICKSEL); 
//          cpack($404040); 
//          (* patchje: kleur blijft constant *)
//          tbase.flag:= OB_FROMDUPLI; 
//          make_duplilist(G.scene,base.object); 
//          ob:= duplilist.first; 
//          while ob
//          do
//          begin 
//            tbase.object:= ob; 
//            draw_object(@tbase); 
//            ob:= ob.id.next; 
//          end;
//          free_duplilist(); 
//          G.f:= G.f and ( not G_PICKSEL); 
//        end;
//        else
//        if (base.flag and SELECT)=0
// then
//        begin 
//          draw_object(base); 
//        end;
//      end;
//    end;
//    base:= base.next; 
//  end;
//  base:= G.scene.base.first; (*  selected *)
//  while base
//  do
//  begin 
//    
//    function TESTBASE{!!!3 unknown typedef}: if; 
//    begin
//      
//      function ELEM3{!!!3 unknown typedef}: if; 
//      else
//      draw_object(base); 
//    end
//    base:= base.next; 
//  end;
//  
//  if G.scene.radio<>0 
// then
//  rad_drawall(); 
//  (* radview.c *)
//  if G.zbuf<>0 
// then
//  begin 
//    G.zbuf:= LongBool(0); 
//    glDisable(GL_DEPTH_TEST); 
//  end;
//  G.f:= G.f and ( not G_SIMULATION); 
//  glReadPixels(0,0,R.rectx,R.recty,GL_RGBA,GL_UNSIGNED_BYTE,R.rectot); 
//  ortho2(-0.5,{!!!a type cast? =>} {single(}R.winx-0.5,-0.5,{!!!a type cast? =>} {single(}R.winy-0.5);
//  glLoadIdentity(); 
//end;
//
//
//var
//tottime: double = 0.0; 
//type
//tms = record
//end;
//var {was static}
//ltime: integer; 
//
//
//function update_time: integer; 
//var
//time: integer; 
//{$ifdef BEOS}
//begin
//  
//  
//  time:= glut_system_time() div 10000; 
//  {$else}
//  time:= times(@voidbuf); 
//  {$endif}
//  tottime:= tottime + ((time-ltime) div TIME_INTERVAL); 
//  ltime:= time; 
//  begin
//    result:= (tottime<0.0); 
//    exit;
//  end;
//end;
//
//
//function speed_to_swaptime(speed: integer): double; 
//begin
//  case speed of
//    1:
//    begin
//      begin
//        result:= 1.0 div 60.0; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    2:
//    begin
//      begin
//        result:= 1.0 div 50.0; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    3:
//    begin
//      begin
//        result:= 1.0 div 30.0; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    4:
//    begin
//      begin
//        result:= 1.0 div 25.0; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    5:
//    begin
//      begin
//        result:= 1.0 div 20.0; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    6:
//    begin
//      begin
//        result:= 1.0 div 15.0; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    7:
//    begin
//      begin
//        result:= 1.0 div 12.5; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    8:
//    begin
//      begin
//        result:= 1.0 div 10.0; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    9:
//    begin
//      begin
//        result:= 1.0 div 6.0; 
//        exit;
//      end;
//    end;
//  end;{case?}
//end;
//
//
//function key_to_swaptime(key: integer): double; 
//begin
//  case key of
//    PAD1:
//    begin
//      G.animspeed:= 1; 
//      tottime:= 0; 
//      begin
//        result:= speed_to_swaptime(1); 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    PAD2:
//    begin
//      G.animspeed:= 2; 
//      tottime:= 0; 
//      begin
//        result:= speed_to_swaptime(2); 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    PAD3:
//    begin
//      G.animspeed:= 3; 
//      tottime:= 0; 
//      begin
//        result:= speed_to_swaptime(3); 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    PAD4:
//    begin
//      G.animspeed:= 4; 
//      tottime:= 0; 
//      begin
//        result:= speed_to_swaptime(4); 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    PAD5:
//    begin
//      G.animspeed:= 5; 
//      tottime:= 0; 
//      begin
//        result:= speed_to_swaptime(5); 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    PAD6:
//    begin
//      G.animspeed:= 6; 
//      tottime:= 0; 
//      begin
//        result:= speed_to_swaptime(6); 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    PAD7:
//    begin
//      G.animspeed:= 7; 
//      tottime:= 0; 
//      begin
//        result:= speed_to_swaptime(7); 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    PAD8:
//    begin
//      G.animspeed:= 8; 
//      tottime:= 0; 
//      begin
//        result:= speed_to_swaptime(8); 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    PAD9:
//    begin
//      G.animspeed:= 9; 
//      tottime:= 0; 
//      begin
//        result:= speed_to_swaptime(9); 
//        exit;
//      end;
//    end;
//  end;{case?}
//  begin
//    result:= speed_to_swaptime(G.animspeed); 
//    exit;
//  end;
//end;
//
//
//function play_anim(mode: integer): integer; 
//var
//sa: pScrArea; 
//oldsa: pScrArea; 
//swaptime: double; 
//cfra: integer; 
//cfraont: integer; 
//event: word;
// 
//val: smallint; 
//mval: array [0..1] of smallint; 
//str: array [0..Pred(12)] of char; (* patch voor zeer oude scenes *)
//begin
//  
//  
//  
//  
//  
//  event:=0; 
//  
//  
//  if SFRA=0
// then
//  SFRA:= 1; 
//  if EFRA=0
// then
//  EFRA:= 250; 
//  if SFRA>EFRA
// then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  update_time(); 
//  tottime:= 0.0; 
//  swaptime:= speed_to_swaptime(G.animspeed); 
//  G.f:= G.f or (G_PLAYANIM); 
//  (* waitcursor(1); *)
//  (* in sequence.c en view.c wordt dit afgevangen *)
//  
//  if G.scene.r.scemode and R_NETWORK
// then
//  begin 
//    init_render_camera_network(); 
//  end;
//  cfraont:= CFRA; 
//  oldsa:= curarea; 
//  while LongBool(1)
//  do
//  begin 
//    if tottime>0.0
// then
//    tottime:= 0.0; 
//    for{while}  to EFRA { CFRA++}
//    do
//    begin 
//      set_timecursor(CFRA); 
//      do_all_ipos(); 
//      do_all_scripts(SCRIPT_FRAMECHANGED); 
//      do_all_keys(); 
//      do_all_ikas(); 
//      test_all_displists(); 
//      
//      sa:= G.curscreen.areabase.first; 
//      while sa
//      do
//      begin 
//        if sa=oldsa
// then
//        begin 
//          if sa.win)and(sa.windraw
// then
//          begin 
//            (* hier winget() gebruiken: anders wordt in header getekend *)
//            if sa.win<>winget()
// then
//            areawinset(sa.win); 
//            sa.windraw(); 
//          end;
//        end;
//        else
//        if mode<>0 
// then
//        begin 
//          
//          function ELEM{!!!3 unknown typedef}: if; 
//          begin
//            if sa.win)and(sa.windraw
// then
//            begin 
//              (* hier winget() gebruiken: anders wordt in header getekend *)
//              if sa.win<>winget()
// then
//              areawinset(sa.win); 
//              sa.windraw(); 
//            end;
//          end
//        end;
//        sa:= sa.next; 
//      end;
//      tottime:= tottime - (swaptime); (* minimaal swaptime laten voorbijgaan *)
//      while update_time()
//      do
//      usleep(1); 
//      screen_swapbuffers(); 
//      
//      tottime:= 0.0; 
//      while qtest()
//      do
//      begin 
//        event:= extern_qread(@val); 
//        if event=ESCKEY
// then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        if val<>0 
// then
//        begin 
//          swaptime:= key_to_swaptime(event); 
//        end;
//      end;
//      if event=ESCKEY)or(event=SPACEKEY
// then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    if event=ESCKEY)or(event=SPACEKEY)or(mode=2
// then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    CFRA:= SFRA; 
//  end;
//  
//  if event=SPACEKEY
// then
//  ; 
//  else
//  CFRA:= cfraont; 
//  do_all_ipos(); 
//  do_all_keys(); 
//  do_all_ikas(); 
//  
//  if oldsa<>curarea
// then
//  areawinset(oldsa.win); 
//  sa:= G.curscreen.areabase.first; (* restore all areas *)
//  while sa
//  do
//  begin 
//    if (mode)and(sa.spacetype=SPACE_VIEW3D))or(sa=curarea
// then
//    addqueue(sa.win,REDRAW,1); 
//    sa:= sa.next; 
//  end;
//  allqueue(REDRAWBUTSANIM,0); 
//  (* speed button *)
//  do_global_buttons(B_NEWFRAME); 
//  (* vooropig *)
//  
//  if G.scene.r.scemode and R_NETWORK
// then
//  begin 
//  end_render_camera_network(); 
//  allqueue(REDRAWBUTSALL,0); 
//end;
//(* efra *)
//waitcursor(0); 
//G.f:= G.f and ( not G_PLAYANIM); 
//if event=ESCKEY)or(event=SPACEKEY
//then
//begin
//  result:= 1; 
//  exit;
//end;
//else
//begin
//  result:= 0; 
//  exit;
//end;
//end;

end.
