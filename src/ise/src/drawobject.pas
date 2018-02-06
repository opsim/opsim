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

(* drawobject.c    GRAPHICS

 *
 * jan 95
 *
 *)

unit drawobject;

interface

uses
GL,
blenglob, blendef;

var
rect_desel: array [0..15] of cardinal = ($707070,$0,$0,$707070,$407070,$70cccc,$407070,$0,$aaffff,$ffffff,$70cccc,$0,$70cccc,$aaffff,$407070,$707070);
rect_sel: array [0..15] of cardinal = ($707070,$0,$0,$707070,$702070,$cc50cc,$702070,$0,$ff80ff,$ffffff,$cc50cc,$0,$cc50cc,$ff80ff,$702070,$707070);

rectu_desel: array [0..15] of cardinal = ($ff4e4e4e,$ff5c2309,$ff000000,$ff4e4f4d,$ff000000,$ffff9d72,$ffff601c,$ff000000,$ff5d2409,$ffffffff,$ffff9d72,$ff5b2209,$ff4e4e4e,$ff5c2309,$ff010100,$ff4f4f4f);
rectu_sel: array [0..15] of cardinal = ($ff4e4e4e,$ff403c00,$ff000000,$ff4e4e4d,$ff000000,$fffff64c,$ffaaa100,$ff000000,$ff403c00,$ffffffff,$fffff64c,$ff403c00,$ff4f4f4f,$ff403c00,$ff010100,$ff4e4e4e);

rectl_desel: array [0..80] of cardinal = ($777777,$777777,$a9fefe,$aaffff,$aaffff,$aaffff,$aaffff,$777777,$777777,$777777,$a9fefe,$aafefe,$777777,$777777,$777777,$a9fefe,$a9fefe,$777777,$aaffff,$a9fefe,$4e4e4e,$0,$124040,$0,$4e4e4e,$aafefe,$aaffff,$aaffff,$777777,$0,$227777,$55cccc,$227777,$0,$777777,$aaffff,$aaffff,$777777,$124040,$88ffff,$ffffff,$55cccc,$124040,$777777,$aaffff,$aaffff,$777777,$0,$55cccc,$88ffff,$227777,$0,$777777,$aaffff,$aafefe,$aafefe,$4f4f4f,$0,$124040,$0,$4e4e4e,$a9fefe,$aaffff,$777777,$a9fefe,$a9fefe,$777777,$777777,$777777,$a9fefe,$a9fefe,$777777,$777777,$777777,$a9fefe,$a9fefe,$aaffff,$aaffff,$aaffff,$777777,$777777);
rectl_sel: array [0..80] of cardinal = ($777777,$777777,$ffaaff,$ffaaff,$ffaaff,$ffaaff,$ffaaff,$777777,$777777,$777777,$ffaaff,$ffaaff,$777777,$777777,$777777,$ffaaff,$ffaaff,$777777,$ffaaff,$ffaaff,$4e4e4e,$10101,$402440,$0,$4e4e4e,$ffaaff,$ffaaff,$ffaaff,$777777,$0,$774477,$cc77cc,$774477,$0,$777777,$ffaaff,$ffaaff,$777777,$402440,$ffaaff,$ffffff,$cc77cc,$412541,$777777,$ffaaff,$ffaaff,$777777,$10101,$cc77cc,$ffaaff,$774477,$0,$777777,$ffaaff,$ffaaff,$ffaaff,$4e4e4e,$10101,$402440,$0,$4e4e4e,$ffaaff,$ffaaff,$777777,$ffaaff,$ffaaff,$777777,$777777,$777777,$ffaaff,$ffaaff,$777777,$777777,$777777,$ffaaff,$ffaaff,$ffaaff,$ffaaff,$ffaaff,$777777,$777777);
rectlus_desel: array [0..80] of cardinal = ($777777,$777777,$a9fefe,$aaffff,$aaffff,$aaffff,$aaffff,$777777,$777777,$777777,$a9fefe,$aafefe,$777777,$777777,$777777,$a9fefe,$a9fefe,$777777,$aaffff,$a9fefe,$4e4e4e,$0,$5c2309,$0,$4e4f4d,$aafefe,$aaffff,$aaffff,$777777,$0,$ff601c,$ff9d72,$ff601c,$0,$777777,$aaffff,$aaffff,$777777,$5d2409,$ffceb8,$ff9d72,$ff9d72,$5b2209,$777777,$aaffff,$aaffff,$777777,$10100,$ffceb8,$ffceb8,$ff601c,$0,$777777,$aaffff,$aafefe,$aafefe,$4e4e4e,$0,$5c2309,$10100,$4f4f4f,$a9fefe,$aaffff,$777777,$a9fefe,$a9fefe,$777777,$777777,$777777,$a9fefe,$a9fefe,$777777,$777777,$777777,$a9fefe,$a9fefe,$aaffff,$aaffff,$aaffff,$777777,$777777);
rectlus_sel: array [0..80] of cardinal = ($777777,$777777,$ffaaff,$ffaaff,$ffaaff,$ffaaff,$ffaaff,$777777,$777777,$777777,$ffaaff,$ffaaff,$777777,$777777,$777777,$ffaaff,$ffaaff,$777777,$ffaaff,$ffaaff,$4e4e4e,$10100,$403c00,$0,$4e4e4d,$ffaaff,$ffaaff,$ffaaff,$777777,$0,$aaa100,$fff64c,$aaa100,$0,$777777,$ffaaff,$ffaaff,$777777,$403c00,$fffde2,$ffffff,$fff64c,$403c00,$777777,$ffaaff,$ffaaff,$777777,$10100,$fff64c,$fffde2,$aaa100,$0,$777777,$ffaaff,$ffaaff,$ffaaff,$4f4f4f,$0,$403c00,$10100,$4e4e4e,$ffaaff,$ffaaff,$777777,$ffaaff,$ffaaff,$777777,$777777,$777777,$ffaaff,$ffaaff,$777777,$777777,$777777,$ffaaff,$ffaaff,$ffaaff,$ffaaff,$ffaaff,$777777,$777777);
rectllib_desel: array [0..80] of cardinal = ($ff777777,$ff777777,$b9b237,$b9b237,$b9b237,$b9b237,$b9b237,$ff777777,$ff777777,$ff777777,$b9b237,$b9b237,$ff777777,$ff777777,$ff777777,$b9b237,$b9b237,$ff777777,$b9b237,$b9b237,$4e4e4e,$0,$5c2309,$0,$4e4f4d,$b9b237,$b9b237,$b9b237,$ff777777,$0,$ff601c,$ff9d72,$ff601c,$0,$ff777777,$b9b237,$b9b237,$ff777777,$5d2409,$ffceb8,$ff9d72,$ff9d72,$5b2209,$ff777777,$b9b237,$b9b237,$ff777777,$10100,$ffceb8,$ffceb8,$ff601c,$0,$ff777777,$b9b237,$b9b237,$b9b237,$4e4e4e,$0,$5c2309,$10100,$4f4f4f,$b9b237,$b9b237,$ff777777,$b9b237,$b9b237,$ff777777,$ff777777,$ff777777,$b9b237,$b9b237,$ff777777,$ff777777,$ff777777,$b9b237,$b9b237,$b9b237,$b9b237,$b9b237,$ff777777,$ff777777);
rectllib_sel: array [0..80] of cardinal = ($ff777777,$ff777777,$fff64c,$fff64c,$fff64c,$fff64c,$fff64c,$ff777777,$ff777777,$ff777777,$fff64c,$fff64c,$ff777777,$ff777777,$ff777777,$fff64c,$fff64c,$ff777777,$fff64c,$fff64c,$4e4e4e,$10100,$403c00,$0,$4e4e4d,$fff64c,$fff64c,$fff64c,$ff777777,$0,$aaa100,$fff64c,$aaa100,$0,$ff777777,$fff64c,$fff64c,$ff777777,$403c00,$fffde2,$ffffff,$fff64c,$403c00,$ff777777,$fff64c,$fff64c,$ff777777,$10100,$fff64c,$fffde2,$aaa100,$0,$ff777777,$fff64c,$fff64c,$fff64c,$4f4f4f,$0,$403c00,$10100,$4e4e4e,$fff64c,$fff64c,$ff777777,$fff64c,$fff64c,$ff777777,$ff777777,$ff777777,$fff64c,$fff64c,$ff777777,$ff777777,$ff777777,$fff64c,$fff64c,$fff64c,$fff64c,$fff64c,$ff777777,$ff777777);

rectl_set: array [0..80] of cardinal = ($ff777777,$ff777777,$aaaaaa,$aaaaaa,$aaaaaa,$aaaaaa,$aaaaaa,$ff777777,$ff777777,$ff777777,$aaaaaa,$aaaaaa,$ff777777,$ff777777,$ff777777,$aaaaaa,$aaaaaa,$ff777777,$aaaaaa,$aaaaaa,$4e4e4e,$10100,$202020,$0,$4e4e4d,$aaaaaa,$aaaaaa,$aaaaaa,$ff777777,$0,$aaa100,$aaaaaa,$aaa100,$0,$ff777777,$aaaaaa,$aaaaaa,$ff777777,$202020,$fffde2,$ffffff,$aaaaaa,$202020,$ff777777,$aaaaaa,$aaaaaa,$ff777777,$10100,$aaaaaa,$fffde2,$aaa100,$0,$ff777777,$aaaaaa,$aaaaaa,$aaaaaa,$4f4f4f,$0,$202020,$10100,$4e4e4e,$aaaaaa,$aaaaaa,$ff777777,$aaaaaa,$aaaaaa,$ff777777,$ff777777,$ff777777,$aaaaaa,$aaaaaa,$ff777777,$ff777777,$ff777777,$aaaaaa,$aaaaaa,$aaaaaa,$aaaaaa,$aaaaaa,$ff777777,$ff777777);

const
B_YELLOW = $77FFFF; 
B_PURPLE = $FF70FF; 

var
selcol: cardinal = $FF88FF; 
actselcol: cardinal = $FFBBFF; 

colortab: array [0..9] of cardinal = (
$0,$FF88FF,$FFBBFF,
$403000,$FFFF88,$FFFFBB,
$103040,$66CCCC,$77CCCC,
$FFFFFF);

cube: array [0..7,0..2] of single = (
(-1.0, -1.0, -1.0),
(-1.0, -1.0,  1.0),
(-1.0,  1.0,  1.0),
(-1.0,  1.0, -1.0),
( 1.0, -1.0, -1.0),
( 1.0, -1.0,  1.0),
( 1.0,  1.0,  1.0),
( 1.0,  1.0, -1.0)
);

cube1: array [0..7,0..2] of single = (
(-1.2,-1.2,-1.0),
(-1.0,-1.0,1.0),
(-1.0,1.0,1.0),
(-1.2,1.2,-1.0),
(1.2,-1.2,-1.0),
(1.0,-1.0,1.0),
(1.0,1.0,1.0),
(1.2,1.2,-1.0)
);

procedure init_draw_rects;

implementation

uses
  imageprocess;

procedure init_draw_rects;
begin
  if G.order=B_ENDIAN then
  begin
    convert_rgba_to_abgr(16,rect_desel);
    convert_rgba_to_abgr(16,rect_sel);

    convert_rgba_to_abgr(16,rectu_desel);
    convert_rgba_to_abgr(16,rectu_sel);

    convert_rgba_to_abgr(81,rectl_desel);
    convert_rgba_to_abgr(81,rectl_sel);

    convert_rgba_to_abgr(81,rectlus_desel);
    convert_rgba_to_abgr(81,rectlus_sel);

    convert_rgba_to_abgr(81,rectllib_desel);
    convert_rgba_to_abgr(81,rectllib_sel);

    convert_rgba_to_abgr(81,rectl_set);
  end;
end;

//procedure tekenrect9(vec: pfloat;  rect: puint); 
//var
//dummy: char = #0;
//begin
//  if vec<>0 then
//  begin 
//    glRasterPos3f(vec[0],vec[1],vec[2]); 
//    (* bitmap is used as a patch to move the rasterpos a few pixels *)
//    glBitmap(1,1,0.0,0.0,-4.5,-4.5, and dummy);
//    glDrawPixels(9,9,GL_RGBA,GL_UNSIGNED_BYTE,rect);
//  end;
//end;

//procedure tekenrect4(vec: pfloat;  rect: puint); 
//var
//  dummy: char = #0;
//begin
//  if vec<>0 then
//  begin 
//    glRasterPos3f(vec[0],vec[1],vec[2]); 
//    glBitmap(1,1,0.0,0.0,-2.0,-2.0, and dummy); 
//    (* bitmap is used as a patch to move the rasterpos a few pixels *)
//    glDrawPixels(4,4,GL_RGBA,GL_UNSIGNED_BYTE,rect); 
//  end;
//end;

//procedure tekenrect3(vec: pfloat;  rect: puint); 
//var
//dummy: char;

//begin
//  dummy:=0; 
//  if vec<>0 then
//  begin 
//    glRasterPos3f(vec[0],vec[1],vec[2]); 
//    glBitmap(1,1,0.0,0.0,-1.5,-1.5, and dummy); 
//    (* bitmap is used as a patch to move the rasterpos a few pixels *)
//    glDrawPixels(3,3,GL_RGBA,GL_UNSIGNED_BYTE,rect); 
//  end;
//end;

//procedure helpline(vec: pfloat); 
//var
//vecrot: array [0..2] of single;
//mval: array [0..1] of smallint;
//mval1: array [0..1] of smallint;
//begin
//  VECCOPY(vecrot,vec);
//  if G.obedit<>0 then
//  Mat4MulVecfl(G.obedit.obmat,vecrot); 
//  getmouseco_areawin(mval); 

//  project_short_noclip(vecrot,mval1); 
//  persp(0); 

//  glDrawBuffer(GL_FRONT); 
//  cpack(0); 
//  setlinestyle(3); 

//  glBegin(GL_LINE_STRIP); 
//  glVertex2sv(mval); 
//  glVertex2sv(mval1); 
//  glEnd(); 
//  setlinestyle(0); 
//  persp(1); 
//  glDrawBuffer(GL_BACK);
//end;

//procedure drawaxes(size: single);
//var
//v1: array [0..2] of single;
//v2: array [0..2] of single;
//f800: single;
//f125: single;
//begin

//  fmsetfont(G.font); 
//  f125:= 0.125*size; 
//  f800:= 0.8*size; 
//  v1[0]:= v1[1]:=v1[2]:=v2[0]:=v2[1]:=v2[2]:=0.0; 
//  v2[0]:= size; 
//  LINE3F(v1,v2); 

//  (* X-AS: pijl *)
//  v1[0]:= f800; 
//  v1[1]:= -f125; 
//  LINE3F(v1,v2); 
//  v1[1]:= f125; 
//  LINE3F(v1,v2); 
//  glRasterPos3f(size+f125,0.0,0.0); 
//  fmprstr('x'); 
//  v1[0]:= v1[1]:=v1[2]:=v2[0]:=v2[1]:=v2[2]:=0.0; 
//  v2[1]:= size; 
//  LINE3F(v1,v2); 
//  (* Y-AS: pijl *)
//  v1[1]:= f800; 
//  v1[0]:= -f125; 
//  LINE3F(v1,v2); 
//  v1[0]:= f125; 
//  LINE3F(v1,v2); 
//  glRasterPos3f(0.0,size+f125,0.0); 
//  fmprstr('y'); 
//  v1[0]:= v1[1]:=v1[2]:=v2[0]:=v2[1]:=v2[2]:=0.0; 
//  v2[2]:= size; 
//  LINE3F(v1,v2); 
//  (* Z-AS: pijl *)
//  v1[2]:= f800; 
//  v1[0]:= -f125; 
//  LINE3F(v1,v2); 
//  v1[0]:= f125; 
//  LINE3F(v1,v2); 
//  glRasterPos3f(0.0,0.0,size+f125); 

//  fmprstr('z'); 
//end;

//procedure drawgourcube; 
//var
//vec: array [0..2] of single;
//n: array [0..2] of single;
//begin

//  n[0]:= 0; 
//  n[1]:= 0; 
//  n[2]:= 0; 
//  glBegin(GL_POLYGON); 
//  n[0]:= -1.0; 
//  glNormal3fv(n); 
//  glVertex3fv(cube[0]); 
//  glVertex3fv(cube[1]); 
//  glVertex3fv(cube[2]); 
//  glVertex3fv(cube[3]); 
//  n[0]:= 0; 
//  glEnd(); 
//  glBegin(GL_POLYGON); 

//  n[1]:= -1.0; 
//  glNormal3fv(n); 
//  glVertex3fv(cube[0]); 
//  glVertex3fv(cube[4]); 
//  glVertex3fv(cube[5]); 
//  glVertex3fv(cube[1]); 
//  n[1]:= 0; 
//  glEnd(); 
//  glBegin(GL_POLYGON); 

//  n[0]:= 1.0; 
//  glNormal3fv(n); 
//  glVertex3fv(cube[4]); 
//  glVertex3fv(cube[7]); 
//  glVertex3fv(cube[6]); 
//  glVertex3fv(cube[5]); 
//  n[0]:= 0; 
//  glEnd(); 
//  glBegin(GL_POLYGON); 

//  n[1]:= 1.0; 
//  glNormal3fv(n); 
//  glVertex3fv(cube[7]); 
//  glVertex3fv(cube[3]); 
//  glVertex3fv(cube[2]); 
//  glVertex3fv(cube[6]); 
//  n[1]:= 0; 
//  glEnd(); 
//  glBegin(GL_POLYGON); 

//  n[2]:= 1.0; 
//  glNormal3fv(n); 
//  glVertex3fv(cube[1]); 
//  glVertex3fv(cube[5]); 
//  glVertex3fv(cube[6]); 
//  glVertex3fv(cube[2]); 
//  n[2]:= 0; 
//  glEnd(); 
//  glBegin(GL_POLYGON); 

//  n[2]:= -1.0; 
//  glNormal3fv(n); 
//  glVertex3fv(cube[7]); 
//  glVertex3fv(cube[4]); 
//  glVertex3fv(cube[0]); 
//  glVertex3fv(cube[3]); 
//  glEnd(); 
//end;

//procedure drawcube; 
//begin
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(cube[0]); 
//  glVertex3fv(cube[1]); 
//  glVertex3fv(cube[2]); 
//  glVertex3fv(cube[3]); 
//  glVertex3fv(cube[0]); 
//  glVertex3fv(cube[4]); 
//  glVertex3fv(cube[5]); 
//  glVertex3fv(cube[6]); 
//  glVertex3fv(cube[7]); 
//  glVertex3fv(cube[4]); 
//  glEnd(); 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(cube[1]); 
//  glVertex3fv(cube[5]); 
//  glEnd(); 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(cube[2]); 
//  glVertex3fv(cube[6]); 
//  glEnd(); 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(cube[3]); 
//  glVertex3fv(cube[7]); 
//  glEnd(); 
//end;

//procedure drawcube_size(size: pfloat); 
//begin
//  glPushMatrix(); 

//  glScalef(size[0],size[1],size[2]); 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(cube[0]); 
//  glVertex3fv(cube[1]); 
//  glVertex3fv(cube[2]); 
//  glVertex3fv(cube[3]); 
//  glVertex3fv(cube[0]); 
//  glVertex3fv(cube[4]); 
//  glVertex3fv(cube[5]); 
//  glVertex3fv(cube[6]); 
//  glVertex3fv(cube[7]); 
//  glVertex3fv(cube[4]); 
//  glEnd(); 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(cube[1]); 
//  glVertex3fv(cube[5]); 
//  glEnd(); 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(cube[2]); 
//  glVertex3fv(cube[6]); 
//  glEnd(); 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(cube[3]); 
//  glVertex3fv(cube[7]); 
//  glEnd(); 
//  glPopMatrix(); 
//end;

//procedure tekenshadbuflimits(la: pLamp;  mat: array [0..,0..3] of single);
//var
//sta: array [0..2] of single;
//end: array [0..2] of single;
//lavec: array [0..2] of single;
//s: array [0..1] of smallint;
//begin

//  lavec[0]:= -mat[2][0]; 
//  lavec[1]:= -mat[2][1]; 
//  lavec[2]:= -mat[2][2]; 
//  Normalise(lavec); 
//  sta[0]:= mat[3][0]+la.clipsta*lavec[0]; 
//  sta[1]:= mat[3][1]+la.clipsta*lavec[1]; 
//  sta[2]:= mat[3][2]+la.clipsta*lavec[2]; 
//end[0]:= mat[3][0]+la.clipend*lavec[0]; 
//end[1]:= mat[3][1]+la.clipend*lavec[1]; 
//end[2]:= mat[3][2]+la.clipend*lavec[2]; 
//glBegin(GL_LINE_STRIP); 

//glVertex3fv(sta); 
//glVertex3fv(end); 
//glEnd(); 
//glPointSize(3.0); 

//glBegin(GL_POINTS); 
//cpack(0); 
//glVertex3fv(sta); 
//glVertex3fv(end); 
//glEnd(); 
//glPointSize(1.0); 
//end;

//void{!!!e unknown token}
//lvec: pfloat; 
//vvec: pfloat; 
//inp: single;
//begin 
//  (* camera staat op 0,0,0 *)
//  temp: array [0..2] of single;
//  plane: array [0..2] of single;
//  mat1: array [0..2,0..2] of single;
//  mat2: array [0..2,0..2] of single;
//  mat3: array [0..2,0..2] of single;
//  mat4: array [0..2,0..2] of single;
//  q: array [0..3] of single;
//  co: single;
//  si: single;
//  hoek: single;
//  rt: pfloat; 
//  Normalise(lvec); 
//  Normalise(vvec); 
//  Crossf(temp,vvec,lvec); 
//  (* is dit de goede vector ? *)

//  Crossf(plane,lvec,temp); 
//  (* vergelijking van vlak door vvec en lvec *)
//  Normalise(plane); 
//  (* en dan het vlak loodrecht daarop en evenwijdig aan lvec *)

//  q[1]:= plane[1]; 
//  q[2]:= -plane[0]; 
//  q[3]:= 0; 
//  Normalise(@q[1]); 

//  (* nu hebben we twee vergelijkingen: die van de kegel en die van het vlak, maar we hebben
//   drie onbekenden We halen nu een onbekende weg door het vlak naar z=0 te roteren *)
//  (* Ik heb geen flauw idee of dat mag, we moeten tenslotte twee oplossingen krijgen, maar we
//   proberen het gewoon: vlak vector moet (0,0,1) worden*)

//  (* roteer om uitproduct vector van (0,0,1) en vlakvector, inproduct graden *)
//  (* volgens defenitie volgt dat uitproduct is (plane[1],-plane[0],0), en cos() = plane[2]);*)

//  hoek:= safacos(plane[2]) div 2.0; 
//  co:= fcos(hoek); 
//  si:= fsqrt(1-co*co); 
//  q[0]:= co; 
//  q[1]:= q[1] * (si); 
//  q[2]:= q[2] * (si); 
//  q[3]:= 0; 
//  QuatToMat3(q,mat1); 

//  vvec[0]:= lvec[0]; 
//  vvec[1]:= lvec[1]; 
//  vvec[2]:= lvec[2]; 
//  Mat3One(mat2); 

//  (* lampvector nu over acos(inp) graden roteren *)

//  co:= inp; 
//  si:= fsqrt(1-inp*inp); 
//  mat2[0][0]:= co; 
//  mat2[1][0]:= -si; 
//  mat2[0][1]:= si; 
//  mat2[1][1]:= co; 
//  Mat3MulMat3(mat3,mat2,mat1); 

//  mat2[1][0]:= si; 
//  mat2[0][1]:= -si; 
//  Mat3MulMat3(mat4,mat2,mat1); 

//  Mat3Transp(mat1); 
//  Mat3MulMat3(mat2,mat1,mat3); 

//  Mat3MulVecfl(mat2,lvec); 
//  Mat3MulMat3(mat2,mat1,mat4); 
//  Mat3MulVecfl(mat2,vvec); 

//  exit;
//end;

//procedure drawlamp(ob: pObject); 
//var
//la: pLamp; 
//vec: array [0..2] of single;
//lvec: array [0..2] of single;
//vvec: array [0..2] of single;
//x: single;
//y: single;
//z: single;
//begin

//  la:= ob.data; 
//  vec[0]:= vec[1]:=vec[2]:=0.0; 
//  setlinestyle(4); 
//  if la.type=LA_SPOT then
//  begin 
//    lvec[0]:= lvec[1]:=0.0; 
//    lvec[2]:= 1.0; 
//    x:= G.vd.persmat[0][2]; 
//    y:= G.vd.persmat[1][2]; 
//    z:= G.vd.persmat[2][2]; 
//    vvec[0]:= x*ob.obmat[0][0]+y*ob.obmat[0][1]+z*ob.obmat[0][2]; 
//    vvec[1]:= x*ob.obmat[1][0]+y*ob.obmat[1][1]+z*ob.obmat[1][2]; 
//    vvec[2]:= x*ob.obmat[2][0]+y*ob.obmat[2][1]+z*ob.obmat[2][2]; 
//    y:= fcos(M_PI*la.spotsize div 360.0); 
//    spotvolume(lvec,vvec,y); 

//    x:= -la.dist; 
//    lvec[0]:= lvec[0] * (x); 
//    lvec[1]:= lvec[1] * (x); 
//    lvec[2]:= lvec[2] * (x); 
//    vvec[0]:= vvec[0] * (x); 
//    vvec[1]:= vvec[1] * (x); 
//    vvec[2]:= vvec[2] * (x); 
//    glBegin(GL_LINE_STRIP); 

//    glVertex3fv(vvec); 
//    glVertex3fv(vec); 
//    glVertex3fv(lvec); 
//    glEnd(); 
//    z:= x*fsqrt(1.0-y*y); 
//    x:= x * (y); 
//    glTranslatef(0.0,0.0,x); 

//    if la.mode and LA_SQUARE then
//    begin 
//      vvec[0]:= fabs(z); 
//      vvec[1]:= fabs(z); 
//      vvec[2]:= 0.0; 
//      glBegin(GL_LINE_LOOP); 
//      glVertex3fv(vvec); 
//      vvec[1]:= -fabs(z); 
//      glVertex3fv(vvec); 
//      vvec[0]:= -fabs(z); 
//      glVertex3fv(vvec); 
//      vvec[1]:= fabs(z); 
//      glVertex3fv(vvec); 
//      glEnd(); 
//    end;
//    else
//    circ(0.0,0.0,fabs(z)); 
//  end;
//  else

//  function ELEM{!!!3 unknown typedef}: if; 
//  begin
//    glBegin(GL_LINE_STRIP); 
//    glVertex3fv(vec); 
//    vec[2]:= -la.dist; 
//    glVertex3fv(vec); 
//    glEnd(); 
//  end
//  else
//  begin 
//    if la.mode and LA_SPHERE then
//    begin 

//      var
//      tmat: array [0..3,0..3] of single;
//      imat: array [0..3,0..3] of single;
//      vec[0]:= vec[1]:=vec[2]:=0.0; 
//      getmatrix(tmat); 
//      Mat4Invert(imat,tmat); 
//      drawcircball(vec,la.dist,imat); 
//    end;

//  end;
//  loadmatrix(G.vd.viewmat); 
//  VECCOPY(vec,ob.obmat[3]); 
//  glBegin(GL_LINE_STRIP); 
//  glVertex3fv(vec); 
//  vec[2]:= 0; 
//  glVertex3fv(vec); 
//  glEnd(); 
//  setlinestyle(0); 
//  if la.type=LA_SPOT)and((la.mode and LA_SHAD) then
//  begin 
//    tekenshadbuflimits(la,ob.obmat); 
//  end;
//end;

//procedure draw_limit_line(sta: single;  end: single;  col: cardinal);
//var
//vec: array [0..1,0..2] of single;
//mval: array [0..1] of smallint;
//begin

//  vec[0][0]:= 0.0; 
//  vec[0][1]:= 0.0; 
//  vec[0][2]:= -sta; 
//  vec[1][0]:= 0.0; 
//  vec[1][1]:= 0.0; 
//  vec[1][2]:= -end; 
//  LINE3F(vec[0],vec[1]); 

//  glPointSize(3.0); 

//  glBegin(GL_POINTS); 
//  cpack(col); 
//  glVertex3fv(vec[0]); 
//  glVertex3fv(vec[1]); 
//  glEnd(); 
//  glPointSize(1.0); 
//end;

//procedure drawcamera(ob: pObject); 
//var
//cam: pCamera; 
//wrld: pWorld; 
//vec: array [0..Pred(8),0..3] of single;
//tmat: array [0..3,0..3] of single;
//fac: single;
//facx: single;
//facy: single;
//depth: single;
//mval: array [0..1] of smallint;
//begin(* een staande piramide met (0,0,0) als top *)

//  cam:= ob.data; 
//  glDisable(GL_LIGHTING); 
//  glDisable(GL_CULL_FACE); 
//  fac:= cam.drawsize; (* zo is ie altijd te zien *)
//  if G.vd.persp>=2 then
//  fac:= cam.clipsta+0.1; 
//  depth:= -fac*cam.lens div 16.0; 
//  facx:= fac*1.28; 
//  facy:= fac*1.$14; 
//  vec[0][0]:= 0; 
//  vec[0][1]:= 0; 
//  vec[0][2]:= 0.$1; 
//  vec[1][0]:= facx; 
//  vec[1][1]:= facy; 
//  vec[1][2]:= depth; 
//  vec[2][0]:= facx; 
//  vec[2][1]:= -facy; 
//  vec[2][2]:= depth; 
//  vec[3][0]:= -facx; 
//  vec[3][1]:= -facy; 
//  vec[3][2]:= depth; 
//  vec[4][0]:= -facx; 
//  vec[4][1]:= facy; 
//  vec[4][2]:= depth; 
//  glBegin(GL_LINE_LOOP); 
//  (* GLBUG: z niet op nul vanwege picking op entry *)

//  glVertex3fv(vec[0]); 
//  glVertex3fv(vec[1]); 
//  glVertex3fv(vec[2]); 
//  glVertex3fv(vec[0]); 
//  glVertex3fv(vec[3]); 
//  glVertex3fv(vec[4]); 
//  glEnd(); 
//  glBegin(GL_LINES); 

//  glVertex3fv(vec[2]); 
//  glVertex3fv(vec[3]); 
//  glEnd(); 
//  glBegin(GL_LINES); 

//  glVertex3fv(vec[4]); 
//  glVertex3fv(vec[1]); 
//  glEnd(); 

//  if G.vd.persp>=2 then
//  exit;
//  if G.f and G_BACKBUFSEL then
//  exit;
//  (* pijl aan top *)
//  vec[0][2]:= depth; 
//  glBegin(GL_POLYGON); 

//  vec[0][0]:= -0.2*cam.drawsize; 
//  vec[0][1]:= cam.drawsize; 
//  glVertex3fv(vec[0]); 

//  vec[0][0]:= 0.2*cam.drawsize; 
//  glVertex3fv(vec[0]); 
//  vec[0][1]:= 1.6*cam.drawsize; 
//  glVertex3fv(vec[0]); 
//  vec[0][0]:= -0.2*cam.drawsize; 
//  glVertex3fv(vec[0]); 
//  glEnd(); 
//  glBegin(GL_POLYGON); 

//  vec[0][0]:= -0.4*cam.drawsize; 
//  vec[0][1]:= 1.6*cam.drawsize; 
//  glVertex3fv(vec[0]); 
//  vec[0][0]:= 0.0; 
//  vec[0][1]:= 2.0*cam.drawsize; 
//  glVertex3fv(vec[0]); 
//  vec[0][0]:= 0.4*cam.drawsize; 
//  vec[0][1]:= 1.6*cam.drawsize; 
//  glVertex3fv(vec[0]); 
//  glEnd(); 
//  if cam.flag and (CAM_SHOWLIMITS+CAM_SHOWMIST+CAM_SHOWNETCLIP) then
//  begin 
//    loadmatrix(G.vd.viewmat); 
//    Mat4CpyMat4(vec,ob.obmat); 
//    Mat4Ortho(vec); 
//    multmatrix(vec); 
//    Mat4SwapMat4(G.vd.persmat,tmat); 

//    mygetsingmatrix(G.vd.persmat); 

//    if cam.flag and CAM_SHOWLIMITS then
//    draw_limit_line(cam.clipsta,cam.clipend,B_YELLOW); 

//    if cam.flag and CAM_SHOWNETCLIP then
//    draw_limit_line(cam.netsta,cam.netend,0); 
//    wrld:= G.scene.world; 
//    if cam.flag and CAM_SHOWMIST then
//    if wrld<>0 then
//    draw_limit_line(wrld.miststa,wrld.miststa+wrld.mistdist,$FFFFFF); 
//    Mat4SwapMat4(G.vd.persmat,tmat); 
//  end;
//end;

//procedure tekenvertslatt(sel: smallint); 
//var
//lt: pLattice; 
//bp: pBPoint; 
//rect: puint; 
//a: integer; 
//uxt: integer; 
//u: integer; 
//vxt: integer; 
//v: integer; 
//wxt: integer; 
//w: integer; 
//begin

//  glPointSize(3.0); 

//  if sel<>0 then
//  cpack(B_YELLOW); 
//  else
//  cpack(B_PURPLE); 
//  glBegin(GL_POINTS); 

//  bp:= editLatt.def; 
//  lt:= editLatt; 
//  if lt.flag and LT_OUTSIDE then
//  begin 
//    for{while} w:=0 to lt.pntsw - 1 { w++}
//    do
//    begin 
//      if w=0)or(w=lt.pntsw-1 then
//      wxt:= 1; 
//      else
//      wxt:= 0; 
//      for{while} v:=0 to Pred(lt.pntsv) { v++}
//      do
//      begin 
//        if v=0)or(v=lt.pntsv-1 then
//        vxt:= 1; 
//        else
//        vxt:= 0; 
//        while{for} 
//        u:= 0; 
//        {to} u<lt.pntsu
//        {;}
//        inc(u); 
//        inc(bp); 
//        do
//        begin 
//          if u=0)or(u=lt.pntsu-1 then
//          uxt:= 1; 
//          else
//          uxt:= 0; 
//          if uxt)or(vxt)or(wxt then
//          begin 
//            if bp.hide=0 then
//            begin 
//              if (bp.f1 and 1)=sel then
//              glVertex3fv(bp.vec); 
//            end;
//          end;
//        end;
//      end;
//    end;
//  end;
//  else
//  begin 

//    a:= editLatt.pntsu*editLatt.pntsv*editLatt.pntsw; 
//    while a{--} dec(a); 
//    do
//    begin 
//      if bp.hide=0 then
//      begin 
//        if (bp.f1 and 1)=sel then
//        glVertex3fv(bp.vec); 
//      end;
//      inc(bp); 
//    end;
//  end;
//  glPointSize(1.0); 
//  glEnd(); 
//end;

//procedure calc_lattverts; 
//var
//bp: pBPoint; 
//mat: array [0..3,0..3] of single;
//a: integer; 
//tot: integer; 
//b: integer; 
//sp: psmallint; 
//begin

//  Mat4SwapMat4(G.vd.persmat,mat); 
//  mygetsingmatrix(G.vd.persmat); 
//  bp:= editLatt.def; 
//  a:= editLatt.pntsu*editLatt.pntsv*editLatt.pntsw; 
//  while a{--} dec(a); 
//  do
//  begin 
//    project_short(bp.vec,bp.s); 
//    inc(bp); 
//  end;
//  Mat4SwapMat4(G.vd.persmat,mat); 

//end;

//procedure calc_lattverts_ext; 
//begin
//  areawinset(curarea.win); 

//  multmatrix(G.obedit.obmat); 
//  calc_lattverts(); 
//  loadmatrix(G.vd.viewmat); 
//end;

//procedure drawlattice(ob: pObject); 
//var
//lt: pLattice; 
//bp: pBPoint; 
//bpu: pBPoint; 
//u: integer; 
//v: integer; 
//w: integer; 
//dv: integer; 
//dw: integer; 
//uxt: integer; 
//vxt: integer; 
//wxt: integer; 
//begin

//  lt:= ob.data; 
//  if ob=G.obedit then
//  begin 
//    bp:= editLatt.def; 
//    cpack($004000); 
//  end;
//  else
//  begin 
//    bp:= lt.def; 
//  end;
//  dv:= lt.pntsu; 
//  dw:= dv*lt.pntsv; 
//  if lt.flag and LT_OUTSIDE then
//  begin 
//    for{while} w:=0 to lt.pntsw - 1 { w++}
//    do
//    begin 
//      if w=0)or(w=lt.pntsw-1 then
//      wxt:= 1; 
//      else
//      wxt:= 0; 
//      for{while} v:=0 to Pred(lt.pntsv) { v++}
//      do
//      begin 
//        if v=0)or(v=lt.pntsv-1 then
//        vxt:= 1; 
//        else
//        vxt:= 0; 
//        while{for} 
//        u:= 0; 
//        bpu:= 0; 
//        {to} u<lt.pntsu
//        {;}
//        inc(u); 
//        inc(bp); 
//        do
//        begin 
//          if u=0)or(u=lt.pntsu-1 then
//          uxt:= 1; 
//          else
//          uxt:= 0; 
//          if uxt)or(vxt)or(wxt then
//          begin 
//            if w)and((uxt)or(vxt) then
//            begin 

//              glBegin(GL_LINE_STRIP); 
//              glVertex3fv((bp-dw).vec); 
//              glVertex3fv(bp.vec); 
//              glEnd(); 
//            end;
//            if v)and((uxt)or(wxt) then
//            begin 

//              glBegin(GL_LINES); 
//              glVertex3fv((bp-dv).vec); 
//              glVertex3fv(bp.vec); 
//              glEnd(); 
//            end;
//            if u)and((vxt)or(wxt) then
//            begin 

//              glBegin(GL_LINES); 
//              glVertex3fv(bpu.vec); 
//              glVertex3fv(bp.vec); 
//              glEnd(); 
//            end;
//          end;
//          bpu:= bp; 
//        end;
//      end;
//    end;
//  end;
//  else
//  begin 
//    for{while} w:=0 to lt.pntsw - 1 { w++}
//    do
//    begin 
//      for{while} v:=0 to Pred(lt.pntsv) { v++}
//      do
//      begin 
//        while{for} 
//        u:= 0; 
//        bpu:= 0; 
//        {to} u<lt.pntsu
//        {;}
//        inc(u); 
//        inc(bp); 
//        do
//        begin 
//          if w<>0 then
//          begin 

//            glBegin(GL_LINES); 
//            glVertex3fv((bp-dw).vec); 
//            glVertex3fv(bp.vec); 
//            glEnd(); 
//          end;
//          if v<>0 then
//          begin 

//            glBegin(GL_LINES); 
//            glVertex3fv((bp-dv).vec); 
//            glVertex3fv(bp.vec); 
//            glEnd(); 
//          end;
//          if u<>0 then
//          begin 

//            glBegin(GL_LINES); 
//            glVertex3fv(bpu.vec); 
//            glVertex3fv(bp.vec); 
//            glEnd(); 
//          end;
//          bpu:= bp; 
//        end;
//      end;
//    end;
//  end;
//  if ob=G.obedit then
//  begin 
//    calc_lattverts(); 
//    if G.zbuf<>0 then
//    glDisable(GL_DEPTH_TEST); 
//    tekenvertslatt(0); 
//    tekenvertslatt(1); 
//    if G.zbuf<>0 then
//    glEnable(GL_DEPTH_TEST); 
//  end;
//end;

//(* ***************** ******************** *)

//procedure calc_meshverts; 
//var
//eve: pEditVert; 
//mat: array [0..3,0..3] of single;
//begin

//  if G.edve.first=0 then
//  exit;
//  eve:= G.edve.first; 
//  Mat4SwapMat4(G.vd.persmat,mat); 

//  mygetsingmatrix(G.vd.persmat); 
//  eve:= G.edve.first; 
//  while eve
//  do
//  begin 
//    if eve.h=0 then
//    begin 
//      project_short(eve.co,@(eve.xs)); 
//    end;
//    eve:= eve.next; 
//  end;
//  Mat4SwapMat4(G.vd.persmat,mat); 
//end;

//procedure calc_meshverts_ext; 
//begin
//  areawinset(curarea.win); 

//  multmatrix(G.obedit.obmat); 
//  calc_meshverts(); 
//  loadmatrix(G.vd.viewmat); 
//end;

//procedure calc_Nurbverts(nurb: pNurb); 
//var
//nu: pNurb; 
//bezt: pBezTriple; 
//bp: pBPoint; 
//mat: array [0..3,0..3] of single;
//a: integer; 
//tot: integer; 
//b: integer; 
//sp: psmallint; 
//begin

//  Mat4SwapMat4(G.vd.persmat,mat); 
//  mygetsingmatrix(G.vd.persmat); 
//  nu:= nurb; 
//  while nu
//  do
//  begin 
//    if (nu.type and 7)=1 then
//    begin 
//      bezt:= nu.bezt; 
//      a:= nu.pntsu; 
//      while a{--} dec(a); 
//      do
//      begin 
//        project_short(bezt.vec[0],bezt.s[0]); 
//        project_short(bezt.vec[1],bezt.s[1]); 
//        project_short(bezt.vec[2],bezt.s[2]); 
//        inc(bezt); 
//      end;
//    end;
//    else
//    begin 
//      bp:= nu.bp; 
//      a:= nu.pntsu*nu.pntsv; 
//      while a{--} dec(a); 
//      do
//      begin 
//        project_short(bp.vec,bp.s); 
//        inc(bp); 
//      end;
//    end;
//    nu:= nu.next; 
//  end;
//  Mat4SwapMat4(G.vd.persmat,mat); 

//end;

//procedure calc_nurbverts_ext; 
//begin
//  areawinset(curarea.win); 

//  multmatrix(G.obedit.obmat); 
//  calc_Nurbverts(editNurb.first); 
//  loadmatrix(G.vd.viewmat); 
//end;

//procedure tekenvertices(sel: smallint); 
//var
//eve: pEditVert; 
//xs: smallint; 
//ys: smallint; 
//begin

//  glPointSize(2.0); 
//  if sel<>0 then
//  cpack(B_YELLOW); 
//  else
//  cpack(B_PURPLE); 
//  glBegin(GL_POINTS); 
//  eve:= {!!!a type cast? =>} {pEditVert(}G.edve.first; 
//  while eve
//  do
//  begin 
//    if eve.h=0)and((eve.f and 1)=sel then
//    begin 
//      glVertex3fv(eve.co); 
//    end;
//    eve:= eve.next; 
//  end;
//  glEnd(); 
//  glPointSize(1.0); 

//end;

//procedure tekenvertices_ext(mode: integer); 
//var
//tempsa: pScrArea; 
//sa: pScrArea; 
//vd: pView3D; 
//begin

//  if G.f and (G_FACESELECT+G_DRAWFACES) then
//  begin 
//    allqueue(REDRAWVIEW3D,0); 
//    exit;
//  end;
//  if G.zbuf<>0 then
//  glDisable(GL_DEPTH_TEST); 
//  glDrawBuffer(GL_FRONT); 
//  tempsa:= curarea; 
//  sa:= G.curscreen.areabase.first; 
//  (* alle views aflopen *)
//  while sa
//  do
//  begin 
//    if sa.spacetype=SPACE_VIEW3D then
//    begin 
//      vd:= sa.spacedata.first; 
//      if G.obedit.lay and vd.lay then
//      begin 
//        areawinset(sa.win); 
//        multmatrix(G.obedit.obmat); 
//        calc_meshverts(); 

//        if mode=0)or(mode=2 then
//        tekenvertices(0); 
//        if mode=1)or(mode=2 then
//        tekenvertices(1); 
//        sa.win_swap:= WIN_FRONT_OK; 
//        loadmatrix(G.vd.viewmat); 
//      end;
//    end;
//    sa:= sa.next; 
//  end;
//  if curarea<>tempsa then
//  areawinset(tempsa.win); 
//  glDrawBuffer(GL_BACK); 
//  if G.zbuf<>0 then
//  glEnable(GL_DEPTH_TEST); 
//end;

//(* ************** DRAW DISPLIST ****************** *)

//draw_index_wire: integer = 1; 
//index3_nors_incr: integer = 1; 

//procedure drawDispListwire(dlbase: pListBase); 
//var
//dl: pDispList; 
//parts: integer; 
//nr: integer; 
//ofs: integer; 
//p1: integer; 
//p2: integer; 
//p3: integer; 
//p4: integer; 
//a: integer; 
//b: integer; 
//index: pinteger; 
//data: pfloat; 
//v1: pfloat; 
//v2: pfloat; 
//v3: pfloat; 
//v4: pfloat; 
//side: single;
//begin

//  if dlbase=0 then
//  exit;

//  dl:= dlbase.first; 
//  while dl
//  do
//  begin 
//    data:= dl.verts; 
//    case dl.type of
//      DL_SEGM:
//      begin
//        parts:= dl.parts; 
//        while parts{--} dec(parts); 
//        do
//        begin 
//          nr:= dl.nr; 
//          glBegin(GL_LINE_STRIP); 
//          while nr{--} dec(nr); 
//          do
//          begin 
//            glVertex3fv(data); 
//            data:= data + (3); 
//          end;
//          glEnd(); 
//        end;
//      end;
//      DL_POLY:
//      begin
//        parts:= dl.parts; 
//        while parts{--} dec(parts); 
//        do
//        begin 
//          nr:= dl.nr; 
//          glBegin(GL_LINE_LOOP); 
//          while nr{--} dec(nr); 
//          do
//          begin 
//            glVertex3fv(data); 
//            data:= data + (3); 
//          end;
//          glEnd(); 
//        end;
//      end;
//      DL_SURF:
//      begin
//        parts:= dl.parts; 
//        while parts{--} dec(parts); 
//        do
//        begin 
//          nr:= dl.nr; 
//          if dl.flag and 1 then
//          glBegin(GL_LINE_LOOP); 
//          else
//          glBegin(GL_LINE_STRIP); 

//          while nr{--} dec(nr); 
//          do
//          begin 
//            glVertex3fv(data); 
//            data:= data + (3); 
//          end;
//          if dl.flag and 1 then
//          glEnd(); 
//          else
//          glEnd(); 
//        end;
//        ofs:= 3*dl.nr; 
//        nr:= dl.nr; 
//        while nr{--} dec(nr); 
//        do
//        begin 
//          data:= (dl.verts)+3*nr; 
//          parts:= dl.parts; 
//          if dl.flag and 2 then
//          glBegin(GL_LINE_LOOP); 
//          else

//          glBegin(GL_LINE_STRIP); 
//          while parts{--} dec(parts); 
//          do
//          begin 
//            glVertex3fv(data); 
//            data:= data + (ofs); 
//          end;
//          if dl.flag and 2 then
//          glEnd(); 
//          else
//          glEnd(); 
//        end;
//      end;
//      DL_INDEX3:
//      begin
//        if draw_index_wire<>0 then
//        begin 
//          parts:= dl.parts; 
//          data:= dl.verts; 
//          index:= dl.index; 
//          while parts{--} dec(parts); 
//          do
//          begin 

//            glBegin(GL_LINE_STRIP); 
//            glVertex3fv(data+3*index[0]); 
//            glVertex3fv(data+3*index[1]); 
//            glVertex3fv(data+3*index[2]); 
//            glEnd(); 
//            index:= index + (3); 
//          end;
//        end;
//      end;
//      DL_INDEX4:
//      begin
//        if draw_index_wire<>0 then
//        begin 
//          parts:= dl.parts; 
//          data:= dl.verts; 
//          index:= dl.index; 
//          while parts{--} dec(parts); 
//          do
//          begin 

//            glBegin(GL_LINE_STRIP); 
//            glVertex3fv(data+3*index[0]); 
//            glVertex3fv(data+3*index[1]); 
//            glVertex3fv(data+3*index[2]); 
//            if index[3]<>0 then
//            glVertex3fv(data+3*index[3]); 
//            glEnd(); 
//            index:= index + (4); 
//          end;
//        end;
//      end;

//    end;{case?}
//    dl:= dl.next; 
//  end;
//end;

//procedure drawDispListsolid(lb: pListBase;  ob: pObject); 
//var
//dl: pDispList; 
//parts: integer; 
//nr: integer; 
//ofs: integer; 
//p1: integer; 
//p2: integer; 
//p3: integer; 
//p4: integer; 
//a: integer; 
//b: integer; 
//index: pinteger; 
//data: pfloat; 
//v1: pfloat; 
//v2: pfloat; 
//v3: pfloat; 
//v4: pfloat; 
//side: single;
//ndata: pfloat; 
//n1: pfloat; 
//n2: pfloat; 
//n3: pfloat; 
//n4: pfloat; 
//col: array [0..2] of smallint;
//cp: pchar; 
//begin

//  if lb=0 then
//  exit;
//  if (G.f and G_BACKBUFSEL)=0 then
//  begin 
//    glShadeModel(GL_SMOOTH); 
//    glEnable(GL_LIGHTING); 
//  end;
//  dl:= lb.first; 
//  while dl
//  do
//  begin 
//    data:= dl.verts; 
//    ndata:= dl.nors; 
//    case dl.type of
//      DL_SURF:
//      begin
//        if G.f and G_BACKBUFSEL then
//        begin 
//          for{while} a:=0 to Pred(dl.parts) { a++}
//          do
//          begin 
//            DL_SURFINDEX(dl.flag and 1,dl.flag and 2,dl.nr,dl.parts); 
//            v1:= data+3*p1; 
//            v2:= data+3*p2; 
//            v3:= data+3*p3; 
//            v4:= data+3*p4; 
//            glBegin(GL_QUAD_STRIP); 
//            glVertex3fv(v2); 
//            glVertex3fv(v4); 

//            for{while}  to Pred(dl.nr) { b++}
//            do
//            begin 
//              glVertex3fv(v1); 
//              glVertex3fv(v3); 
//              v2:= v1; 
//              v1:= v1 + (3); 
//              v4:= v3; 
//              v3:= v3 + (3); 

//            end;
//            glEnd(); 
//          end;
//        end;
//        else
//        begin 

//          set_gl_material(dl.col+1); 
//          (*    
//              glBegin(GL_LINES);
//              for(a=0; a<dl->parts*dl->nr; a++) {
//               single nor[3];

//               VECCOPY(nor, data+3*a);
//               glVertex3fv(nor);
//               VecAddf(nor, nor, ndata+3*a);
//               glVertex3fv(nor);
//              }
//              glEnd();
//          *)
//          for{while} a:=0 to Pred(dl.parts) { a++}
//          do
//          begin 
//            DL_SURFINDEX(dl.flag and 1,dl.flag and 2,dl.nr,dl.parts); 
//            v1:= data+3*p1; 
//            v2:= data+3*p2; 
//            v3:= data+3*p3; 
//            v4:= data+3*p4; 
//            n1:= ndata+3*p1; 
//            n2:= ndata+3*p2; 
//            n3:= ndata+3*p3; 
//            n4:= ndata+3*p4; 
//            glBegin(GL_QUAD_STRIP); 
//            glNormal3fv(n2); 
//            glVertex3fv(v2); 
//            glNormal3fv(n4); 
//            glVertex3fv(v4); 

//            for{while}  to Pred(dl.nr) { b++}
//            do
//            begin 
//              glNormal3fv(n1); 
//              glVertex3fv(v1); 
//              glNormal3fv(n3); 
//              glVertex3fv(v3); 
//              v2:= v1; 
//              v1:= v1 + (3); 
//              v4:= v3; 
//              v3:= v3 + (3); 
//              n2:= n1; 
//              n1:= n1 + (3); 
//              n4:= n3; 
//              n3:= n3 + (3); 

//            end;
//            glEnd(); 
//          end;
//        end;
//      end;
//      DL_INDEX3:

//      begin
//        parts:= dl.parts; 
//        data:= dl.verts; 
//        ndata:= dl.nors; 
//        index:= dl.index; 
//        if G.f and G_BACKBUFSEL then
//        begin 
//          while parts{--} dec(parts); 
//          do
//          begin 

//            glBegin(GL_POLYGON); 
//            glVertex3fv(data+3*index[0]); 
//            glVertex3fv(data+3*index[1]); 
//            glVertex3fv(data+3*index[2]); 
//            glEnd(); 
//            index:= index + (3); 
//          end;
//        end;
//        else
//        begin 

//          set_gl_material(dl.col+1); 
//          (* voor poly's is er maar 1 normaal nodig *)
//          if index3_nors_incr=0 then
//          begin 
//            while parts{--} dec(parts); 
//            do
//            begin 

//              glBegin(GL_POLYGON); 
//              glNormal3fv(ndata); 
//              glVertex3fv(data+3*index[0]); 
//              glVertex3fv(data+3*index[1]); 
//              glVertex3fv(data+3*index[2]); 
//              glEnd(); 
//              index:= index + (3); 
//            end;
//          end;
//          else
//          begin 
//            while parts{--} dec(parts); 
//            do
//            begin 

//              glBegin(GL_POLYGON); 
//              ofs:= 3*index[0]; 
//              glNormal3fv(ndata+ofs); 
//              glVertex3fv(data+ofs); 
//              ofs:= 3*index[1]; 
//              glNormal3fv(ndata+ofs); 
//              glVertex3fv(data+ofs); 
//              ofs:= 3*index[2]; 
//              glNormal3fv(ndata+ofs); 
//              glVertex3fv(data+ofs); 
//              glEnd(); 
//              index:= index + (3); 
//            end;
//          end;
//        end;
//      end;
//      DL_INDEX4:

//      begin
//        parts:= dl.parts; 
//        data:= dl.verts; 
//        ndata:= dl.nors; 
//        index:= dl.index; 
//        if G.f and G_BACKBUFSEL then
//        begin 
//          while parts{--} dec(parts); 
//          do
//          begin 

//            glBegin(GL_POLYGON); 
//            glVertex3fv(data+3*index[0]); 
//            glVertex3fv(data+3*index[1]); 
//            glVertex3fv(data+3*index[2]); 
//            if index[3]<>0 then
//            glVertex3fv(data+3*index[3]); 
//            glEnd(); 
//            index:= index + (4); 
//          end;
//        end;
//        else
//        begin 
//          set_gl_material(dl.col+1); 
//          while parts{--} dec(parts); 
//          do
//          begin 

//            glBegin(GL_POLYGON); 
//            ofs:= 3*index[0]; 
//            glNormal3fv(ndata+ofs); 
//            glVertex3fv(data+ofs); 
//            ofs:= 3*index[1]; 
//            glNormal3fv(ndata+ofs); 
//            glVertex3fv(data+ofs); 
//            ofs:= 3*index[2]; 
//            glNormal3fv(ndata+ofs); 
//            glVertex3fv(data+ofs); 
//            if index[3]<>0 then
//            begin 
//              ofs:= 3*index[3]; 
//              glNormal3fv(ndata+ofs); 
//              glVertex3fv(data+ofs); 
//            end;
//            glEnd(); 
//            index:= index + (4); 
//          end;
//        end;
//      end;

//    end;{case?}
//    dl:= dl.next; 
//  end;
//  if G.f and G_BACKBUFSEL then
//  ; 
//  else
//  begin 
//    glShadeModel(GL_FLAT); 
//    glDisable(GL_LIGHTING); 
//  end;
//end;

//procedure drawDispListshaded(lb: pListBase;  ob: pObject); 
//var
//dl: pDispList; 
//dlob: pDispList; 
//parts: integer; 
//nr: integer; 
//ofs: integer; 
//p1: integer; 
//p2: integer; 
//p3: integer; 
//p4: integer; 
//a: integer; 
//b: integer; 
//index: pinteger; 
//data: pfloat; 
//v1: pfloat; 
//v2: pfloat; 
//v3: pfloat; 
//v4: pfloat; 
//side: single;
//extverts: pfloat;

//cdata: puint; 
//c1: puint; 
//c2: puint; 
//c3: puint; 
//c4: puint; 
//col: array [0..2] of smallint;
//cp: pchar; 
//begin

//  extverts:=0; 

//  if lb=0 then
//  exit;

//  glShadeModel(GL_SMOOTH); 
//  dl:= lb.first; 
//  dlob:= ob.disp.first; 
//  while dl)and(dlob
//  do
//  begin 
//    cdata:= dlob.col1; 
//    data:= dl.verts; 
//    if cdata=0 then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    case dl.type of
//      DL_SURF:

//      begin
//        for{while} a:=0 to Pred(dl.parts) { a++}
//        do
//        begin 

//          DL_SURFINDEX(dl.flag and 1,dl.flag and 2,dl.nr,dl.parts); 
//          v1:= data+3*p1; 
//          v2:= data+3*p2; 
//          v3:= data+3*p3; 
//          v4:= data+3*p4; 
//          c1:= cdata+p1; 
//          c2:= cdata+p2; 
//          c3:= cdata+p3; 
//          c4:= cdata+p4; 

//          for{while}  to Pred(dl.nr) { b++}
//          do
//          begin 

//            glBegin(GL_POLYGON); 
//            cp:= {!!!a type cast? =>} {pchar(}c1; 
//            glColor3ub(cp[3],cp[2],cp[1]); 
//            glVertex3fv(v1); 
//            cp:= {!!!a type cast? =>} {pchar(}c2; 
//            glColor3ub(cp[3],cp[2],cp[1]); 
//            glVertex3fv(v2); 
//            cp:= {!!!a type cast? =>} {pchar(}c4; 
//            glColor3ub(cp[3],cp[2],cp[1]); 
//            glVertex3fv(v4); 
//            cp:= {!!!a type cast? =>} {pchar(}c3; 
//            glColor3ub(cp[3],cp[2],cp[1]); 
//            glVertex3fv(v3); 
//            glEnd(); 
//            v2:= v1; 
//            v1:= v1 + (3); 
//            v4:= v3; 
//            v3:= v3 + (3); 
//            c2:= c1; 
//            inc(c1); 
//            c4:= c3; 
//            inc(c3); 

//          end;
//        end;
//      end;
//      DL_INDEX3:

//      begin
//        parts:= dl.parts; 
//        index:= dl.index; 
//        while parts{--} dec(parts); 
//        do
//        begin 

//          glBegin(GL_POLYGON); 
//          cp:= {!!!a type cast? =>} {pchar(}(cdata+index[0]); 
//          glColor3ub(cp[3],cp[2],cp[1]); 
//          glVertex3fv(data+3*index[0]); 
//          cp:= {!!!a type cast? =>} {pchar(}(cdata+index[1]); 
//          glColor3ub(cp[3],cp[2],cp[1]); 

//          glVertex3fv(data+3*index[1]); 
//          cp:= {!!!a type cast? =>} {pchar(}(cdata+index[2]); 
//          glColor3ub(cp[3],cp[2],cp[1]); 

//          glVertex3fv(data+3*index[2]); 
//          glEnd(); 
//          index:= index + (3); 
//        end;
//      end;
//      DL_INDEX4:

//      begin
//        parts:= dl.parts; 
//        index:= dl.index; 
//        while parts{--} dec(parts); 
//        do
//        begin 

//          glBegin(GL_POLYGON); 
//          cp:= {!!!a type cast? =>} {pchar(}(cdata+index[0]); 
//          glColor3ub(cp[3],cp[2],cp[1]); 
//          glVertex3fv(data+3*index[0]); 
//          cp:= {!!!a type cast? =>} {pchar(}(cdata+index[1]); 
//          glColor3ub(cp[3],cp[2],cp[1]); 

//          glVertex3fv(data+3*index[1]); 
//          cp:= {!!!a type cast? =>} {pchar(}(cdata+index[2]); 
//          glColor3ub(cp[3],cp[2],cp[1]); 

//          glVertex3fv(data+3*index[2]); 
//          if index[3]<>0 then
//          begin 
//            cp:= {!!!a type cast? =>} {pchar(}(cdata+index[3]); 
//            glColor3ub(cp[3],cp[2],cp[1]); 
//            glVertex3fv(data+3*index[3]); 
//          end;
//          glEnd(); 
//          index:= index + (4); 
//        end;
//      end;

//    end;{case?}
//    dl:= dl.next; 
//    dlob:= dlob.next; 
//  end;
//  glShadeModel(GL_FLAT); 
//end;

//procedure drawmeshsolid(ob: pObject;  nors: pfloat); 
//var
//me: pMesh; 
//dl: pDispList; 
//mvert: pMVert; 
//tface: pTFace; 
//mface: pMFace; 
//evl: pEditVlak; 
//extverts: pfloat;

//v1: pfloat; 
//v2: pfloat; 
//v3: pfloat; 
//v4: pfloat; 
//glmode: integer; 
//setsmooth: integer;

//a: integer; 
//start: integer; 
//end: integer; 
//matnr: integer;

//vertexpaint: integer; 
//i: integer; 
//no: array [0..2] of smallint;
//n1: psmallint; 
//n2: psmallint; 
//n3: psmallint; 
//n4: psmallint; 
//begin

//  extverts:=0; 

//  setsmooth:=0; 

//  matnr:=-1; 

//  vertexpaint:= (G.f and (G_VERTEXPAINT+G_FACESELECT)))and((ob=OBACT); 
//  me:= get_mesh(ob); 
//  if me=0 then
//  exit;

//  glShadeModel(GL_FLAT); 

//  if (G.f and G_BACKBUFSEL)=0 then
//  begin 
//    glEnable(GL_LIGHTING); 
//    init_gl_materials(ob); 
//    two_sided(me.flag and ME_TWOSIDED); 
//  end;
//  mface:= me.mface; 
//  if (G.f and G_FACESELECT))and(ob=OBACT then
//  tface:= me.tface; 
//  else
//  tface:= 0; 
//  mvert:= me.mvert; 
//  a:= me.totface; 

//  if{!!!e unknown token}
//  glEnable(GL_CULL_FACE); 

//  if ob=G.obedit)or((G.obedit)and(ob.data=G.obedit.data) then
//  begin 
//    evl:= G.edvl.first; 
//    while evl
//    do
//    begin 
//      if evl.v1.h=0)and(evl.v2.h=0)and(evl.v3.h=0 then
//      begin 
//        if evl.mat_nr<>matnr then
//        begin 
//          matnr:= evl.mat_nr; 
//          set_gl_material(matnr+1); 
//        end;
//        if evl.v4)and(evl.v4.h=0 then
//        begin 
//          glBegin(GL_QUADS); 
//          glNormal3fv(evl.n); 
//          glVertex3fv(evl.v1.co); 
//          glVertex3fv(evl.v2.co); 
//          glVertex3fv(evl.v3.co); 
//          glVertex3fv(evl.v4.co); 
//          glEnd(); 
//        end;
//        else
//        begin 

//          glBegin(GL_TRIANGLES); 
//          glNormal3fv(evl.n); 
//          glVertex3fv(evl.v1.co); 
//          glVertex3fv(evl.v2.co); 
//          glVertex3fv(evl.v3.co); 
//          glEnd(); 
//        end;
//      end;
//      evl:= evl.next; 
//    end;
//    glDisable(GL_LIGHTING); 
//    glShadeModel(GL_FLAT); 
//    if ob=G.obedit then
//    begin 
//      calc_meshverts(); 
//      if G.zbuf<>0 then
//      glDisable(GL_DEPTH_TEST); 
//      tekenvertices(0); 
//      tekenvertices(1); 
//      if G.zbuf<>0 then
//      glEnable(GL_DEPTH_TEST); 
//    end;
//  end;
//  else
//  begin 

//    start:= 0; 
//  end:= me.totface; 
//  set_buildvars(ob,@start, and end); 
//  mface:= mface + (start); 
//  if tface<>0 then
//  tface:= tface + (start); 
//  dl:= find_displist(@ob.disp,DL_VERTS); 
//  if dl<>0 then
//  extverts:= dl.verts; 
//  glBegin(GL_QUADS); 
//  glmode:= GL_QUADS; 
//  while{for} 
//  a:= start; 
//  {to} a<end
//  {;}
//  inc(a); 
//  inc(mface); 
//  nors:= nors + (3); 
//  do
//  begin 
//    if mface.v3<>0 then
//    begin 
//      if tface)and((tface.flag and TF_HIDE) then
//      begin 
//        if (G.f and G_BACKBUFSEL)=0 then
//        begin 
//          glBegin(GL_LINE_LOOP); 
//          glVertex3fv((mvert+mface.v1).co); 
//          glVertex3fv((mvert+mface.v2).co); 
//          glVertex3fv((mvert+mface.v3).co); 
//          if mface.v4<>0 then
//          glVertex3fv((mvert+mface.v1).co); 
//          glEnd(); 
//          inc(tface); 
//        end;
//      end;
//      else
//      begin 
//        if extverts<>0 then
//        begin 
//          v1:= extverts+3*mface.v1; 
//          v2:= extverts+3*mface.v2; 
//          v3:= extverts+3*mface.v3; 
//          if mface.v4<>0 then
//          v4:= extverts+3*mface.v4; 
//          else
//          v4:= 0; 
//        end;
//        else
//        begin 
//          v1:= (mvert+mface.v1).co; 
//          v2:= (mvert+mface.v2).co; 
//          v3:= (mvert+mface.v3).co; 
//          if mface.v4<>0 then
//          v4:= (mvert+mface.v4).co; 
//          else
//          v4:= 0; 
//        end;
//        if tface<>0 then
//        begin 
//          if tface.mode and TF_TWOSIDE then
//          glEnable(GL_CULL_FACE); 
//          else
//          glDisable(GL_CULL_FACE); 
//        end;
//        (* dit GL_QUADS grapje is op snelheid getest: factor 2! *)
//        if v4<>0 then
//        begin 
//          if glmode=GL_TRIANGLES then
//          begin 
//            glmode:= GL_QUADS; 
//            glEnd(); 
//            glBegin(GL_QUADS); 
//          end;
//        end;
//        else
//        begin 
//          if glmode=GL_QUADS then
//          begin 
//            glmode:= GL_TRIANGLES; 
//            glEnd(); 
//            glBegin(GL_TRIANGLES); 
//          end;
//        end;
//        if G.f and G_BACKBUFSEL then
//        begin 
//          if vertexpaint<>0 then
//          begin 
//            i:= a+1; 
//            i:= ((i and $F00) shl 12)+((i and $F0) shl 8)+((i and $F) shl 4); 
//            i:= i or ($0F0F0F); 
//            cpack(i); 
//          end;
//          glVertex3fv(v1); 
//          glVertex3fv(v2); 
//          glVertex3fv(v3); 
//          if v4<>0 then
//          glVertex3fv(v4); 
//        end;
//        else
//        begin 
//          if mface.mat_nr<>matnr then
//          begin 
//            matnr:= mface.mat_nr; 
//            set_gl_material(matnr+1); 
//          end;
//          if (me.flag and ME_AUTOSMOOTH)=0)and((mface.flag and ME_SMOOTH) then
//          begin 
//            if setsmooth=0 then
//            begin 
//              glEnd(); 
//              glShadeModel(GL_SMOOTH); 
//              if glmode=GL_TRIANGLES then
//              glBegin(GL_TRIANGLES); 
//              else
//              glBegin(GL_QUADS); 
//              setsmooth:= 1; 
//            end;
//            n1:= (mvert+mface.v1).no; 
//            n2:= (mvert+mface.v2).no; 
//            n3:= (mvert+mface.v3).no; 
//            if v4<>0 then
//            n4:= (mvert+mface.v4).no; 
//            if mface.puno and ME_FLIPV1 then
//            begin 
//              no[0]:= -n1[0]; 
//              no[1]:= -n1[1]; 
//              no[2]:= -n1[2]; 
//              glNormal3sv(no); 
//            end;
//            else
//            glNormal3sv(n1); 
//            glVertex3fv(v1); 
//            if mface.puno and ME_FLIPV2 then
//            begin 
//              no[0]:= -n2[0]; 
//              no[1]:= -n2[1]; 
//              no[2]:= -n2[2]; 
//              glNormal3sv(no); 
//            end;
//            else
//            glNormal3sv(n2); 
//            glVertex3fv(v2); 
//            if mface.puno and ME_FLIPV3 then
//            begin 
//              no[0]:= -n3[0]; 
//              no[1]:= -n3[1]; 
//              no[2]:= -n3[2]; 
//              glNormal3sv(no); 
//            end;
//            else
//            glNormal3sv(n3); 
//            glVertex3fv(v3); 
//            if v4<>0 then
//            begin 
//              if mface.puno and ME_FLIPV4 then
//              begin 
//                no[0]:= -n4[0]; 
//                no[1]:= -n4[1]; 
//                no[2]:= -n4[2]; 
//                glNormal3sv(no); 
//              end;
//              else
//              glNormal3sv(n4); 
//              glVertex3fv(v4); 
//            end;
//          end;
//          else
//          begin 
//            if setsmooth=1 then
//            begin 
//              glEnd(); 
//              glShadeModel(GL_FLAT); 
//              if glmode=GL_TRIANGLES then
//              glBegin(GL_TRIANGLES); 
//              else
//              glBegin(GL_QUADS); 
//              setsmooth:= 0; 
//            end;
//            glNormal3fv(nors); 
//            glVertex3fv(v1); 
//            glVertex3fv(v2); 
//            glVertex3fv(v3); 
//            if v4<>0 then
//            glVertex3fv(v4); 
//          end;
//        end;
//      end;
//      if tface<>0 then
//      inc(tface); 
//    end;
//  end;
//  glEnd(); 
//end;
//if{!!!e unknown token}
//glDisable(GL_CULL_FACE); 

//if G.f and G_BACKBUFSEL then
//begin 
//  glDisable(GL_CULL_FACE); 
//end;
//glDisable(GL_LIGHTING); 
//end;

//procedure drawmeshshaded(ob: pObject;  col1: puint;  col2: puint); 
//var
//me: pMesh; 
//mvert: pMVert; 
//mface: pMFace; 
//tface: pTFace; 
//dl: pDispList; 
//extverts: pfloat;

//v1: pfloat; 
//v2: pfloat; 
//v3: pfloat; 
//v4: pfloat; 
//a: integer; 
//start: integer; 
//end: integer; 
//twoside: integer; 
//cp1: pchar; 
//cp2: pchar; 
//begin

//  extverts:=0; 

//  glShadeModel(GL_SMOOTH); 
//  glDisable(GL_LIGHTING); 
//  me:= ob.data; 
//  mface:= me.mface; (* tekent ie geen hide *)
//  if (G.f and G_FACESELECT))and(ob=OBACT then
//  tface:= me.tface; 
//  else
//  tface:= 0; 
//  mvert:= me.mvert; 
//  a:= me.totface; 
//  twoside:= me.flag and ME_TWOSIDED; 
//  if col2=0 then
//  twoside:= 0; 
//  if twoside<>0 then
//  glEnable(GL_CULL_FACE); 
//  start:= 0; 
//end:= me.totface; 
//set_buildvars(ob,@start, and end); 
//mface:= mface + (start); 
//if tface<>0 then
//tface:= tface + (start); 
//col1:= col1 + (4*start); 
//if col2<>0 then
//col2:= col2 + (4*start); 
//dl:= find_displist(@ob.disp,DL_VERTS); 
//if dl<>0 then
//extverts:= dl.verts; 
//glBegin(GL_QUADS); 

//cp1:= {!!!a type cast? =>} {pchar(}col1; 
//if col2<>0 then
//cp2:= {!!!a type cast? =>} {pchar(}col2; 
//while{for} 
//a:= start; 
//{to} a<end
//{;}
//inc(a); 
//inc(mface); 
//cp1:= cp1 + (16); 
//do
//begin 
//  if mface.v3<>0 then
//  begin 
//    if tface)and((tface.flag and TF_HIDE) then
//    inc(tface); 
//    else
//    begin 
//      if extverts<>0 then
//      begin 
//        v1:= extverts+3*mface.v1; 
//        v2:= extverts+3*mface.v2; 
//        v3:= extverts+3*mface.v3; 
//        if mface.v4<>0 then
//        v4:= extverts+3*mface.v4; 
//        else
//        v4:= 0; 
//      end;
//      else
//      begin 
//        v1:= (mvert+mface.v1).co; 
//        v2:= (mvert+mface.v2).co; 
//        v3:= (mvert+mface.v3).co; 
//        if mface.v4<>0 then
//        v4:= (mvert+mface.v4).co; 
//        else
//        v4:= 0; 
//      end;

//      if tface<>0 then
//      begin 
//        if tface.mode and TF_TWOSIDE then
//        glEnable(GL_CULL_FACE); 
//        else
//        glDisable(GL_CULL_FACE); 
//      end;
//      glColor3ub(cp1[3],cp1[2],cp1[1]); 

//      glVertex3fv(v1); 
//      glColor3ub(cp1[7],cp1[6],cp1[5]); 
//      glVertex3fv(v2); 
//      glColor3ub(cp1[11],cp1[10],cp1[9]); 
//      glVertex3fv(v3); 
//      if v4<>0 then
//      begin 
//        glColor3ub(cp1[15],cp1[14],cp1[13]); 
//        glVertex3fv(v4); 
//      end;
//      else
//      glVertex3fv(v3); 
//      if twoside<>0 then
//      begin 

//        glColor3ub(cp2[11],cp2[10],cp2[9]); 
//        glVertex3fv(v3); 
//        glColor3ub(cp2[7],cp2[6],cp2[5]); 
//        glVertex3fv(v2); 
//        glColor3ub(cp2[3],cp2[2],cp2[1]); 
//        glVertex3fv(v1); 
//        if mface.v4<>0 then
//        begin 
//          glColor3ub(cp2[15],cp2[14],cp2[13]); 
//          glVertex3fv(v4); 
//        end;
//        else
//        glVertex3fv(v1); 
//      end;
//    end;
//  end;
//  if col2<>0 then
//  cp2:= cp2 + (16); 
//end;
//glEnd(); 
//glShadeModel(GL_FLAT); 
//if twoside<>0 then
//glDisable(GL_CULL_FACE); 
//end;

//procedure drawDispList(ob: pObject;  dt: integer); 
//var
//lb: pListBase;

//dl: pDispList; 
//me: pMesh; 
//se: pSector; 
//a: integer; 
//solid: integer; 
//begin
//  lb:=0; 

//  solid:= (dt>OB_WIRE); 
//  case ob.type of
//    OB_MESH,
//    OB_SECTOR,
//    OB_LIFE:
//    begin
//      me:= get_mesh(ob); 
//      if me=0 then
//      exit;
//      if me.bb=0 then
//      tex_space_mesh(me); 
//      if me.totface>4 then
//      if boundbox_clip(ob.obmat,me.bb)=0 then
//      exit;
//      if dt=OB_SOLID then
//      begin 
//        lb:= @me.disp; 
//        if lb.first=0 then
//        addnormalsDispList(ob,lb); 
//        dl:= lb.first; 
//        if dl=0 then
//        exit;
//        if me.flag and ME_SMESH then
//        begin 
//          init_gl_materials(ob); 
//          two_sided(0); 
//          drawDispListsolid(lb,ob); 
//        end;
//        else
//        drawmeshsolid(ob,dl.nors); 
//      end;
//      else
//      if dt=OB_SHADED then
//      begin 
//        if G.f and G_VERTEXPAINT then
//        begin 
//          (* in deze volgorde: vertexpaint heeft soms al mcol gemaakt *)
//          if me.mcol<>0 then
//          drawmeshshaded(ob,{!!!a type cast? =>} {puint(}me.mcol,0); 
//          else
//          if me.tface<>0 then
//          begin 
//            tface_to_mcol(me); 
//            drawmeshshaded(ob,{!!!a type cast? =>} {puint(}me.mcol,0); 
//            freeN(me.mcol); 
//            me.mcol:= 0; 
//          end;
//          else
//          drawmeshwire(ob); 
//        end;
//        else
//        begin 
//          dl:= ob.disp.first; 
//          if dl=0)or(dl.col1=0 then
//          begin 
//            shadeDispList(ob); 
//            dl:= ob.disp.first; 
//          end;
//          if dl<>0 then
//          begin 
//            if me.flag and ME_SMESH then
//            drawDispListshaded(@me.disp,ob); 
//            else
//            drawmeshshaded(ob,dl.col1,dl.col2); 
//          end;
//        end;
//      end;
//      if ob=OBACT)and((G.f and G_FACESELECT) then
//      begin 
//        draw_tfaces3D(ob,me); 
//      end;
//    end;
//    OB_FONT,
//    OB_CURVE:
//    begin
//      lb:= @({!!!a type cast? =>} {pCurve(}ob.data).disp; 
//      if lb.first=0 then
//      makeDispList(ob); 
//      if solid)and(ob<>G.obedit then
//      begin 
//        dl:= lb.first; 
//        if dl=0 then
//        exit;
//        (* regel: dl->type INDEX3 altijd vooraan in lijst *)
//        if dl.type<>DL_INDEX3 then
//        begin 
//          curve_to_filledpoly(ob.data,lb); 
//          dl:= lb.first; 
//        end;
//        if dl.nors=0 then
//        addnormalsDispList(ob,lb); 
//        index3_nors_incr:= 0; 
//        if dt=OB_SHADED then
//        begin 
//          if ob.disp.first=0 then
//          shadeDispList(ob); 
//          drawDispListshaded(lb,ob); 
//        end;
//        else
//        begin 
//          init_gl_materials(ob); 
//          two_sided(0); 
//          drawDispListsolid(lb,ob); 
//        end;
//        index3_nors_incr:= 1; 
//      end;
//      else
//      begin 
//        draw_index_wire:= 0; 
//        drawDispListwire(lb); 
//        draw_index_wire:= 1; 
//      end;
//    end;
//    OB_SURF:
//    begin
//      lb:= @({!!!a type cast? =>} {pCurve(}ob.data).disp; 
//      if lb.first=0 then
//      makeDispList(ob); 
//      if solid<>0 then
//      begin 
//        dl:= lb.first; 
//        if dl=0 then
//        exit;
//        if dl.nors=0 then
//        addnormalsDispList(ob,lb); 
//        if dt=OB_SHADED then
//        begin 
//          if ob.disp.first=0 then
//          shadeDispList(ob); 
//          drawDispListshaded(lb,ob); 
//        end;
//        else
//        begin 
//          init_gl_materials(ob); 
//          two_sided(0); 
//          drawDispListsolid(lb,ob); 
//        end;
//      end;
//      else
//      begin 
//        drawDispListwire(lb); 
//      end;
//    end;
//    OB_MBALL:

//    begin
//      lb:= @ob.disp; 
//      if lb.first=0 then
//      makeDispList(ob); 

//      if solid<>0 then
//      begin 
//        if dt=OB_SHADED then
//        begin 
//          dl:= lb.first; 
//          if dl)and(dl.col1=0 then
//          shadeDispList(ob); 
//          drawDispListshaded(lb,ob); 
//        end;
//        else
//        begin 
//          init_gl_materials(ob); 
//          two_sided(0); 
//          drawDispListsolid(lb,ob); 
//        end;
//      end;
//      else
//      drawDispListwire(lb); 
//    end;

//  end;{case?}
//end;

//(* ******************************** *)

//procedure draw_particle_system(ob: pObject;  paf: pPartEff); 
//var
//pa: pParticle; 
//ptime: single;
//ctime: single;
//vec: array [0..2] of single;
//vec1: array [0..2] of single;
//a: integer; 
//begin

//  pa:= paf.keys; 
//  if pa=0 then
//  begin 
//    build_particle_system(ob); 
//    pa:= paf.keys; 
//    if pa=0 then
//    exit;
//  end;
//  loadmatrix(G.vd.viewmat); 
//  if ob.ipoflag and OB_OFFS_PARTICLE then
//  ptime:= ob.sf; 
//  else
//  ptime:= 0.0; 
//  ctime:= bsystem_time(ob,0,{!!!a type cast? =>} {single(}CFRA,ptime);
//  glPointSize(1.0); 

//  if paf.stype<>PAF_VECT then
//  glBegin(GL_POINTS); 

//  while{for} 
//  a:= 0; 
//  {to} a<paf.totpart
//  {;}
//  inc(a); 
//  pa:= pa + (paf.totkey); 
//  do
//  begin 
//    if ctime>pa.time then
//    begin 
//      if ctime<pa.time+pa.lifetime then
//      begin 
//        if paf.stype=PAF_VECT then
//        begin 
//          where_is_particle(paf,pa,ctime,vec); 
//          where_is_particle(paf,pa,ctime+1.0,vec1); 
//          glBegin(GL_LINE_STRIP); 

//          glVertex3fv(vec); 
//          glVertex3fv(vec1); 
//          glEnd(); 
//        end;
//        else
//        begin 
//          where_is_particle(paf,pa,ctime,vec); 
//          glVertex3fv(vec); 
//        end;
//      end;
//    end;
//  end;
//  if paf.stype<>PAF_VECT then
//  glEnd(); 
//end;

//procedure draw_static_particle_system(ob: pObject;  paf: pPartEff); 
//var
//pa: pParticle; 
//ptime: single;
//ctime: single;
//mtime: single;
//vec: array [0..2] of single;
//vec1: array [0..2] of single;
//a: integer; 
//begin

//  pa:= paf.keys; 
//  if pa=0 then
//  begin 
//    build_particle_system(ob); 
//    pa:= paf.keys; 
//    if pa=0 then
//    exit;
//  end;
//  glPointSize(1.0); 
//  if paf.stype<>PAF_VECT then
//  glBegin(GL_POINTS); 

//  while{for} 
//  a:= 0; 
//  {to} a<paf.totpart
//  {;}
//  inc(a); 
//  pa:= pa + (paf.totkey); 
//  do
//  begin 
//    where_is_particle(paf,pa,pa.time,vec1); 
//    mtime:= pa.time+pa.lifetime+paf.staticstep-1; 
//    while{for} 
//    ctime:= pa.time; 
//    {to} ctime<mtime
//    {;}
//    ctime:= ctime + (paf.staticstep); 
//    do
//    begin 
//      (* make sure hair grows until the end.. *)
//      if ctime>pa.time+pa.lifetime then
//      ctime:= pa.time+pa.lifetime; 
//      if paf.stype=PAF_VECT then
//      begin 
//        where_is_particle(paf,pa,ctime+1,vec); 
//        glBegin(GL_LINE_STRIP); 

//        glVertex3fv(vec); 
//        glVertex3fv(vec1); 
//        glEnd(); 
//        VECCOPY(vec1,vec); 
//      end;
//      else
//      begin 
//        where_is_particle(paf,pa,ctime,vec); 
//        glVertex3fv(vec); 
//      end;
//    end;
//  end;
//  if paf.stype<>PAF_VECT then
//  glEnd(); 
//end;

//procedure drawmeshwire(ob: pObject); 
//var
//se: pSector; 
//me: pMesh; 
//mvert: pMVert; 
//mface: pMFace; 
//dface: pDFace; 
//dl: pDispList; 
//ma: pMaterial; 
//eve: pEditVert; 
//eed: pEditEdge; 
//evl: pEditVlak; 
//fvec: array [0..2] of single;
//cent: array [0..2] of single;
//f1: pfloat; 
//f2: pfloat; 
//f3: pfloat; 
//f4: pfloat; 
//extverts: pfloat;

//a: integer; 
//start: integer; 
//end: integer; 
//test: integer; 
//colbcol: integer;

//ok: integer; 
//se: pSector;

//lf: pLife;

//begin
//  {editbutsize: single; }{<= !!!5 external variable}(* buttons.c *)

//  extverts:=0; 

//  colbcol:=0; 
//  me:= get_mesh(ob); 
//  if ob=G.obedit)or((G.obedit)and(ob.data=G.obedit.data) then
//  begin 
//    if G.zbuf=0)and(me.flag and ME_SMESH then
//    begin 
//      cpack($505050); 
//      drawDispListwire(@me.disp); 
//    end;
//    cpack($0); 
//    if G.obedit=ob then
//    begin 
//      cpack(0); 
//      if ob.type=OB_SECTOR then
//      begin 
//        se:=ob.data; 
//        if se.flag and SE_SHOW_TEXMESH then
//        ; 
//        else
//        cpack($401000); 
//      end;
//      else
//      if ob.type=OB_LIFE then
//      begin 
//        lf:=ob.data; 
//        if lf.flag and LF_SHOW_TEXMESH then
//        ; 
//        else
//        cpack($401000); 
//      end;
//    end;
//    eed:= G.eded.first; 
//    glBegin(GL_LINES); 

//    while eed
//    do
//    begin 
//      if eed.h=0 then
//      begin 
//        glVertex3fv(eed.v1.co); 
//        glVertex3fv(eed.v2.co); 
//      end;
//      eed:= eed.next; 
//    end;
//    glEnd(); 
//    if ob<>G.obedit then
//    exit;
//    calc_meshverts(); 

//    if G.zbuf<>0 then
//    glDisable(GL_DEPTH_TEST); 
//    tekenvertices(0); 
//    tekenvertices(1); 
//    if G.zbuf<>0 then
//    glEnable(GL_DEPTH_TEST); 

//    if G.f and G_DRAWNORMALS then
//    begin 
//      (* normals *)
//      cpack($DDDD22); 
//      glBegin(GL_LINES); 

//      evl:= G.edvl.first; 
//      while evl
//      do
//      begin 
//        if evl.v1.h=0)and(evl.v2.h=0)and(evl.v3.h=0 then
//        begin 
//          if evl.v4<>0 then
//          CalcCent4f(fvec,evl.v1.co,evl.v2.co,evl.v3.co,evl.v4.co); 
//          else
//          CalcCent3f(fvec,evl.v1.co,evl.v2.co,evl.v3.co); 
//          glVertex3fv(fvec); 

//          fvec[0]:= fvec[0] + (editbutsize*evl.n[0]); 
//          fvec[1]:= fvec[1] + (editbutsize*evl.n[1]); 
//          fvec[2]:= fvec[2] + (editbutsize*evl.n[2]); 
//          glVertex3fv(fvec); 
//        end;
//        evl:= evl.next; 
//      end;

//      if me.flag and ME_SMESH then
//      begin 
//        cpack($EE4422); 
//        eve:= G.edve.first; 
//        while eve
//        do
//        begin 
//          if eve.h=0 then
//          begin 
//            VECCOPY(fvec,eve.co); 
//            glVertex3fv(fvec); 
//            fvec[0]:= fvec[0] + (editbutsize*eve.no[0]); 
//            fvec[1]:= fvec[1] + (editbutsize*eve.no[1]); 
//            fvec[2]:= fvec[2] + (editbutsize*eve.no[2]); 
//            glVertex3fv(fvec); 
//          end;
//          eve:= eve.next; 
//        end;
//      end;
//      glEnd(); 
//    end;
//    if G.f and (G_FACESELECT+G_DRAWFACES) then
//    begin 
//      (* vlakken *)
//      evl:= G.edvl.first; 
//      while evl
//      do
//      begin 
//        if evl.v1.h=0)and(evl.v2.h=0)and(evl.v3.h=0 then
//        begin 
//          if vlakselectedAND(evl,1) then
//          cpack($559999); 
//          else
//          cpack($664466); 
//          if evl.v4)and(evl.v4.h=0 then
//          begin 
//            CalcCent4f(cent,evl.v1.co,evl.v2.co,evl.v3.co,evl.v4.co); 
//            glBegin(GL_LINE_LOOP); 
//            VecMidf(fvec,cent,evl.v1.co); 
//            glVertex3fv(fvec); 
//            VecMidf(fvec,cent,evl.v2.co); 
//            glVertex3fv(fvec); 
//            VecMidf(fvec,cent,evl.v3.co); 
//            glVertex3fv(fvec); 
//            VecMidf(fvec,cent,evl.v4.co); 
//            glVertex3fv(fvec); 
//            glEnd(); 
//          end;
//          else
//          begin 

//            CalcCent3f(cent,evl.v1.co,evl.v2.co,evl.v3.co); 
//            glBegin(GL_LINE_LOOP); 
//            VecMidf(fvec,cent,evl.v1.co); 
//            glVertex3fv(fvec); 
//            VecMidf(fvec,cent,evl.v2.co); 
//            glVertex3fv(fvec); 
//            VecMidf(fvec,cent,evl.v3.co); 
//            glVertex3fv(fvec); 
//            glEnd(); 
//          end;
//        end;
//        evl:= evl.next; 
//      end;
//    end;
//  end;
//  else
//  begin 
//    if me=0 then
//    exit;
//    if me.bb=0 then
//    tex_space_mesh(me); 
//    if me.totface>4 then
//    if boundbox_clip(ob.obmat,me.bb)=0 then
//    exit;
//    if me.flag and ME_SMESH then
//    drawDispListwire(@me.disp); 
//    else
//    begin 
//      mvert:= me.mvert; 
//      mface:= me.mface; 
//      dface:= me.dface; 
//      ok:= 0; 
//      if me.totface=0 then
//      ok:= 1; 
//      else
//      begin 
//        ma:= give_current_material(ob,1); 
//        if ma)and((ma.mode and MA_HALO) then
//        ok:= 1; 
//      end;
//      dl:= find_displist(@ob.disp,DL_VERTS); 
//      if dl<>0 then
//      extverts:= dl.verts; 
//      if ok<>0 then
//      begin 
//        start:= 0; 
//      end:= me.totvert; 
//      set_buildvars(ob,@start, and end); 
//      glPointSize(1.5); 
//      glBegin(GL_POINTS); 
//      if extverts<>0 then
//      begin 
//        extverts:= extverts + (3*start); 
//        while{for} 
//        a:= start; 
//        {to} a<end
//        {;}
//        inc(a); 
//        extverts:= extverts + (3); 
//        do
//        begin 
//          glVertex3fv(extverts); 
//        end;
//      end;
//      else
//      begin 
//        mvert:= mvert + (start); 
//        while{for} 
//        a:= start; 
//        {to} a<end
//        {;}
//        inc(a); 
//        inc(mvert); 
//        do
//        begin 
//          glVertex3fv(mvert.co); 
//        end;
//      end;
//      glEnd(); 
//      glPointSize(1.0); 
//    end;
//    else
//    begin 
//      start:= 0; 
//    end:= me.totface; 
//    set_buildvars(ob,@start, and end); 
//    mface:= mface + (start); 
//    while{for} 
//    a:= start; 
//    {to} a<end
//    {;}
//    inc(a); 
//    inc(mface); 
//    do
//    begin 
//      test:= mface.edcode; 
//      if dface<>0 then
//      begin 
//        (* test= mface->edcode>>4; *)
//        if dface.flag and DF_HILITE then
//        cpack($FF00FF); 
//        else
//        cpack(0); 
//        dface.flag:= dface.flag and ( not DF_HILITE); 
//        inc(dface); 
//      end;
//      if test<>0 then
//      begin 
//        if extverts<>0 then
//        begin 
//          f1:= extverts+3*mface.v1; 
//          f2:= extverts+3*mface.v2; 
//        end;
//        else
//        begin 
//          f1:= (mvert+mface.v1).co; 
//          f2:= (mvert+mface.v2).co; 
//        end;
//        if mface.v4<>0 then
//        begin 
//          if extverts<>0 then
//          begin 
//            f3:= extverts+3*mface.v3; 
//            f4:= extverts+3*mface.v4; 
//          end;
//          else
//          begin 
//            f3:= (mvert+mface.v3).co; 
//            f4:= (mvert+mface.v4).co; 
//          end;
//          if test=ME_V1V2+ME_V2V3+ME_V3V4+ME_V4V1 then
//          begin 
//            glBegin(GL_LINE_LOOP); 
//            glVertex3fv(f1); 
//            glVertex3fv(f2); 
//            glVertex3fv(f3); 
//            glVertex3fv(f4); 
//            glEnd(); 
//          end;
//          else
//          if test=ME_V1V2+ME_V2V3+ME_V3V4 then
//          begin 
//            glBegin(GL_LINE_STRIP); 
//            glVertex3fv(f1); 
//            glVertex3fv(f2); 
//            glVertex3fv(f3); 
//            glVertex3fv(f4); 
//            glEnd(); 
//          end;
//          else
//          if test=ME_V2V3+ME_V3V4+ME_V4V1 then
//          begin 
//            glBegin(GL_LINE_STRIP); 
//            glVertex3fv(f2); 
//            glVertex3fv(f3); 
//            glVertex3fv(f4); 
//            glVertex3fv(f1); 
//            glEnd(); 
//          end;
//          else
//          if test=ME_V3V4+ME_V4V1+ME_V1V2 then
//          begin 
//            glBegin(GL_LINE_STRIP); 
//            glVertex3fv(f3); 
//            glVertex3fv(f4); 
//            glVertex3fv(f1); 
//            glVertex3fv(f2); 
//            glEnd(); 
//          end;
//          else
//          if test=ME_V4V1+ME_V1V2+ME_V2V3 then
//          begin 
//            glBegin(GL_LINE_STRIP); 
//            glVertex3fv(f4); 
//            glVertex3fv(f1); 
//            glVertex3fv(f2); 
//            glVertex3fv(f3); 
//            glEnd(); 
//          end;
//          else
//          begin 
//            if test and ME_V1V2 then
//            begin 
//              glBegin(GL_LINE_STRIP); 
//              glVertex3fv(f1); 
//              glVertex3fv(f2); 
//              glEnd(); 
//            end;
//            if test and ME_V2V3 then
//            begin 
//              glBegin(GL_LINE_STRIP); 
//              glVertex3fv(f2); 
//              glVertex3fv(f3); 
//              glEnd(); 
//            end;
//            if test and ME_V3V4 then
//            begin 
//              glBegin(GL_LINE_STRIP); 
//              glVertex3fv(f3); 
//              glVertex3fv(f4); 
//              glEnd(); 
//            end;
//            if test and ME_V4V1 then
//            begin 
//              glBegin(GL_LINE_STRIP); 
//              glVertex3fv(f4); 
//              glVertex3fv(f1); 
//              glEnd(); 
//            end;
//          end;
//        end;
//        else
//        if mface.v3<>0 then
//        begin 
//          if extverts<>0 then
//          f3:= extverts+3*mface.v3; 
//          else
//          f3:= (mvert+mface.v3).co; 
//          if test=ME_V1V2+ME_V2V3+ME_V3V1 then
//          begin 
//            glBegin(GL_LINE_LOOP); 
//            glVertex3fv(f1); 
//            glVertex3fv(f2); 
//            glVertex3fv(f3); 
//            glEnd(); 
//          end;
//          else
//          if test=ME_V1V2+ME_V2V3 then
//          begin 
//            glBegin(GL_LINE_STRIP); 
//            glVertex3fv(f1); 
//            glVertex3fv(f2); 
//            glVertex3fv(f3); 
//            glEnd(); 
//          end;
//          else
//          if test=ME_V2V3+ME_V3V1 then
//          begin 
//            glBegin(GL_LINE_STRIP); 
//            glVertex3fv(f2); 
//            glVertex3fv(f3); 
//            glVertex3fv(f1); 
//            glEnd(); 
//          end;
//          else
//          if test=ME_V1V2+ME_V3V1 then
//          begin 
//            glBegin(GL_LINE_STRIP); 
//            glVertex3fv(f3); 
//            glVertex3fv(f1); 
//            glVertex3fv(f2); 
//            glEnd(); 
//          end;
//          else
//          begin 
//            if test and ME_V1V2 then
//            begin 
//              glBegin(GL_LINE_STRIP); 
//              glVertex3fv(f1); 
//              glVertex3fv(f2); 
//              glEnd(); 
//            end;
//            if test and ME_V2V3 then
//            begin 
//              glBegin(GL_LINE_STRIP); 
//              glVertex3fv(f2); 
//              glVertex3fv(f3); 
//              glEnd(); 
//            end;
//            if test and ME_V3V1 then
//            begin 
//              glBegin(GL_LINE_STRIP); 
//              glVertex3fv(f3); 
//              glVertex3fv(f1); 
//              glEnd(); 
//            end;
//          end;
//        end;
//        else
//        if test and ME_V1V2 then
//        begin 
//          glBegin(GL_LINE_STRIP); 
//          glVertex3fv(f1); 
//          glVertex3fv(f2); 
//          glEnd(); 
//        end;
//      end;
//    end;
//  end;
//end;
//end;
//end;

//nurbcol: array [0..Pred(8)] of cardinal = (0,$9090,$409030,$603080,0,$40fff0,$40c033,$A090F0); 
//void{!!!e unknown token}
//nu: pNurb; 
//sel: smallint; 
//begin 
//  bezt: pBezTriple; 
//  fp: pfloat; 
//  col: puint; 
//  a: integer; 
//  if nu.hide<>0 then
//  exit;
//  if (nu.type and 7)=1 then
//  begin 
//    if sel<>0 then
//    col:= nurbcol+4; 
//    else
//    col:= nurbcol; 
//    bezt:= nu.bezt; 
//    a:= nu.pntsu; 
//    while a{--} dec(a); 
//    do
//    begin 
//      if bezt.hide=0 then
//      begin 
//        if (bezt.f2 and 1)=sel then
//        begin 
//          fp:= bezt.vec[0]; 
//          cpack(col[bezt.h1]); 
//          glBegin(GL_LINE_STRIP); 

//          glVertex3fv(fp); 
//          glVertex3fv(fp+3); 
//          glEnd(); 
//          cpack(col[bezt.h2]); 
//          glBegin(GL_LINE_STRIP); 

//          glVertex3fv(fp+3); 
//          glVertex3fv(fp+6); 
//          glEnd(); 
//        end;
//        else
//        if (bezt.f1 and 1)=sel then
//        begin 
//          fp:= bezt.vec[0]; 
//          cpack(col[bezt.h1]); 
//          glBegin(GL_LINE_STRIP); 

//          glVertex3fv(fp); 
//          glVertex3fv(fp+3); 
//          glEnd(); 
//        end;
//        else
//        if (bezt.f3 and 1)=sel then
//        begin 
//          fp:= bezt.vec[1]; 
//          cpack(col[bezt.h2]); 
//          glBegin(GL_LINE_STRIP); 

//          glVertex3fv(fp); 
//          glVertex3fv(fp+3); 
//          glEnd(); 
//        end;
//      end;
//      inc(bezt); 
//    end;
//  end;
//end;

//void{!!!e unknown token}
//nu: pNurb; 
//sel: smallint; 
//begin 
//  bezt: pBezTriple; 
//  bp: pBPoint; 
//  rect: puint; 
//  a: integer; 
//  if nu.hide<>0 then
//  exit;

//  if sel<>0 then
//  cpack(B_YELLOW); 
//  else
//  cpack(B_PURPLE); 
//  glPointSize(3.0); 
//  glBegin(GL_POINTS); 
//  if (nu.type and 7)=1 then
//  begin 

//    bezt:= nu.bezt; 
//    a:= nu.pntsu; 
//    while a{--} dec(a); 
//    do
//    begin 
//      if bezt.hide=0 then
//      begin 
//        if (bezt.f1 and 1)=sel then
//        glVertex3fv(bezt.vec[0]); 
//        if (bezt.f2 and 1)=sel then
//        glVertex3fv(bezt.vec[1]); 
//        if (bezt.f3 and 1)=sel then
//        glVertex3fv(bezt.vec[2]); 
//      end;
//      inc(bezt); 
//    end;
//  end;
//  else
//  begin 
//    bp:= nu.bp; 
//    a:= nu.pntsu*nu.pntsv; 
//    while a{--} dec(a); 
//    do
//    begin 
//      if bp.hide=0 then
//      begin 
//        if (bp.f1 and 1)=sel then
//        glVertex3fv(bp.vec); 
//      end;
//      inc(bp); 
//    end;
//  end;
//  glEnd(); 
//  glPointSize(1.0); 
//end;

//procedure draw_editnurb(ob: pObject;  nurb: pNurb;  sel: integer); 
//var
//nu: pNurb; 
//bp: pBPoint; 
//bp1: pBPoint; 
//a: integer; 
//b: integer; 
//ofs: integer; 
//begin

//  nu:= nurb; 
//  while nu
//  do
//  begin 
//    if nu.hide=0 then
//    begin 
//      case nu.type and 7 of
//        CU_POLY:
//        begin
//          cpack(nurbcol[3]); 
//          bp:= nu.bp; 
//          for{while} b:=0 to Pred(nu.pntsv) { b++}
//          do
//          begin 
//            if nu.flagu and 1 then
//            glBegin(GL_LINE_LOOP); 
//            else

//            glBegin(GL_LINE_STRIP); 

//            while{for} 
//            a:= 0; 
//            {to} a<nu.pntsu
//            {;}
//            inc(a); 
//            inc(bp); 
//            do
//            begin 
//              glVertex3fv(bp.vec); 
//            end;

//            if nu.flagu and 1 then
//            glEnd(); 
//            else
//            glEnd(); 
//          end;
//        end;
//        CU_NURBS:

//        begin
//          bp:= nu.bp; 
//          for{while} b:=0 to Pred(nu.pntsv) { b++}
//          do
//          begin 
//            bp1:= bp; 
//            inc(bp); 
//            while{for} 
//            a:= nu.pntsu-1; 
//            {to} a>0
//            {;}
//            dec(a); 
//            inc(bp); 
//            do
//            begin 
//              if bp.hide=0)and(bp1.hide=0 then
//              begin 
//                if sel<>0 then
//                begin 
//                  if (bp.f1 and 1))and((bp1.f1 and 1) then
//                  begin 
//                    cpack(nurbcol[5]); 
//                    glBegin(GL_LINE_STRIP); 
//                    glVertex3fv(bp.vec); 
//                    glVertex3fv(bp1.vec); 
//                    glEnd(); 
//                  end;
//                end;
//                else
//                begin 
//                  if (bp.f1 and 1))and((bp1.f1 and 1) then
//                  ; 
//                  else
//                  begin 
//                    cpack(nurbcol[1]); 
//                    glBegin(GL_LINE_STRIP); 
//                    glVertex3fv(bp.vec); 
//                    glVertex3fv(bp1.vec); 
//                    glEnd(); 
//                  end;
//                end;
//              end;
//              bp1:= bp; 
//            end;
//          end;
//          if nu.pntsv>1 then
//          begin 
//            (* surface *)

//            ofs:= nu.pntsu; 
//            for{while} b:=0 to Pred(nu.pntsu) { b++}
//            do
//            begin 
//              bp1:= nu.bp+b; 
//              bp:= bp1+ofs; 
//              while{for} 
//              a:= nu.pntsv-1; 
//              {to} a>0
//              {;}
//              dec(a); 
//              bp:= bp + (ofs); 
//              do
//              begin 
//                if bp.hide=0)and(bp1.hide=0 then
//                begin 
//                  if sel<>0 then
//                  begin 
//                    if (bp.f1 and 1))and((bp1.f1 and 1) then
//                    begin 
//                      cpack(nurbcol[7]); 
//                      glBegin(GL_LINE_STRIP); 
//                      glVertex3fv(bp.vec); 
//                      glVertex3fv(bp1.vec); 
//                      glEnd(); 
//                    end;
//                  end;
//                  else
//                  begin 
//                    if (bp.f1 and 1))and((bp1.f1 and 1) then
//                    ; 
//                    else
//                    begin 
//                      cpack(nurbcol[3]); 
//                      glBegin(GL_LINE_STRIP); 
//                      glVertex3fv(bp.vec); 
//                      glVertex3fv(bp1.vec); 
//                      glEnd(); 
//                    end;
//                  end;
//                end;
//                bp1:= bp; 
//              end;
//            end;
//          end;

//        end;

//      end;{case?}
//    end;
//    nu:= nu.next; 
//  end;
//end;

//procedure drawnurb(ob: pObject;  nurb: pNurb;  dt: integer); 
//var
//cu: pCurve; 
//nu: pNurb; 
//bezt: pBezTriple; 
//bp: pBPoint; 
//bp1: pBPoint; 
//bevp: pBevPoint; 
//bl: pBevList; 
//vec: array [0..2] of single;
//seg: array [0..Pred(12)] of single;
//fp: pfloat; 
//data: pfloat; 
//a: integer; 
//b: integer; 
//ofs: integer; 
//nr: integer; 
//skip: integer; 
//(* eerst handles niet select *)
//begin
//  {editbutsize: single; }{<= !!!5 external variable}(* buttons.c *)

//  nu:= nurb; 
//  while nu
//  do
//  begin 
//    if (nu.type and 7)=CU_BEZIER then
//    begin 
//      tekenhandlesN(nu,0); 
//    end;
//    nu:= nu.next; 
//  end;
//  cpack(0); 
//  (* dan DispList *)
//  cu:= ob.data; 
//  drawDispList(ob,dt); 
//  draw_editnurb(ob,nurb,0); 

//  draw_editnurb(ob,nurb,1); 

//  if cu.flag and CU_3D then
//  begin 
//    if cu.bev.first=0 then
//    makeBevelList(ob); 
//    cpack($0); 
//    bl:= cu.bev.first; 
//    nu:= nurb; 
//    while nu)and(bl
//    do
//    begin 
//      bevp:= {!!!a type cast? =>} {pBevPoint(}(bl+1); 
//      nr:= bl.nr; 
//      skip:= nu.resolu div 16; 
//      while nr{--} dec(nr); >0
//      do
//      begin 
//        glBegin(GL_LINE_STRIP); 
//        vec[0]:= bevp.x-editbutsize*bevp.mat[0][0]; 
//        vec[1]:= bevp.y-editbutsize*bevp.mat[0][1]; 
//        vec[2]:= bevp.z-editbutsize*bevp.mat[0][2]; 
//        glVertex3fv(vec); 
//        vec[0]:= bevp.x+editbutsize*bevp.mat[0][0]; 
//        vec[1]:= bevp.y+editbutsize*bevp.mat[0][1]; 
//        vec[2]:= bevp.z+editbutsize*bevp.mat[0][2]; 
//        glVertex3fv(vec); 
//        glEnd(); 

//        inc(bevp); 
//        a:= skip; 
//        while a{--} dec(a); 
//        do
//        begin 
//          inc(bevp); 
//          dec(nr); 
//        end;
//      end;
//      bl:= bl.next; 
//      nu:= nu.next; 

//    end;
//  end;
//  calc_Nurbverts(nurb); 

//  if G.zbuf<>0 then
//  glDisable(GL_DEPTH_TEST); 
//  nu:= nurb; 
//  while nu
//  do
//  begin 
//    if (nu.type and 7)=1 then
//    tekenhandlesN(nu,1); 
//    tekenvertsN(nu,0); 
//    nu:= nu.next; 
//  end;
//  nu:= nurb; 
//  while nu
//  do
//  begin 
//    tekenvertsN(nu,1); 
//    nu:= nu.next; 
//  end;
//  if G.zbuf<>0 then
//  glEnable(GL_DEPTH_TEST); 
//end;

//procedure tekentextcurs; 
//begin
//  cpack(0); 
//  glBegin(GL_POLYGON); 

//  glVertex2fv(G.textcurs[0]); 
//  glVertex2fv(G.textcurs[1]); 
//  glVertex2fv(G.textcurs[2]); 
//  glVertex2fv(G.textcurs[3]); 
//  glEnd(); 
//end;

//procedure drawcircball(cent: pfloat;  rad: single;  tmat: array [0..,0..3] of single);
//var
//si: single;
//co: single;
//phi: single;
//dphi: single;
//vec: array [0..2] of single;
//vx: array [0..2] of single;
//vy: array [0..2] of single;
//a: integer; 
//tot: integer;

//begin

//  tot:=32; 
//  VECCOPY(vx,tmat[0]); 
//  VECCOPY(vy,tmat[1]); 
//  VecMulf(vx,rad); 
//  VecMulf(vy,rad); 
//  dphi:= 2.0*M_PI div tot; 
//  phi:= 0.0; 
//  glBegin(GL_LINE_LOOP); 
//  while{for} 
//  a:= 0; 
//  {to} a<tot
//  {;}
//  inc(a); 
//  phi:= phi + (dphi); 
//  do
//  begin 
//    si:= fsin(phi); 
//    co:= fcos(phi); 
//    vec[0]:= cent[0]+si*vx[0]+co*vy[0]; 
//    vec[1]:= cent[1]+si*vx[1]+co*vy[1]; 
//    vec[2]:= cent[2]+si*vx[2]+co*vy[2]; 
//    glVertex3fv(vec); 
//  end;
//  glEnd(); 
//end;

//procedure drawmball(ob: pObject;  dt: integer); 
//var
//mb: pMetaBall; 
//ml: pMetaElem; 
//imat: array [0..3,0..3] of single;
//tmat: array [0..3,0..3] of single;
//code: integer;

//begin

//  code:=1; 
//  mb:= ob.data; 
//  if ob=G.obedit then
//  begin 
//    cpack($0); 
//    if (G.f and G_PICKSEL)=0 then
//    drawDispList(ob,dt); 
//    ml:= editelems.first; 
//  end;
//  else
//  begin 
//    drawDispList(ob,dt); 
//    ml:= mb.elems.first; 
//  end;
//  getmatrix(tmat); 

//  Mat4Invert(imat,tmat); 
//  Normalise(imat[0]); 
//  Normalise(imat[1]); 
//  while ml
//  do
//  begin 
//    if ob=G.obedit then
//    begin 
//      if ml.flag and SELECT then
//      cpack($A0A0F0); 
//      else
//      cpack($3030A0); 
//      if G.f and G_PICKSEL then
//      begin 
//        ml.selcol:= code; 
//        glLoadName(code{++} inc(code); ); 
//      end;
//    end;
//    drawcircball(@(ml.x),ml.rad,imat); 
//    ml:= ml.next; 
//  end;
//end;

//procedure drawboundbox(ob: pObject); 
//var
//me: pMesh; 
//bb: pBoundBox;

//vec: pfloat; 
//begin

//  bb:=0; 

//  if ob.type=OB_MESH then
//  begin 
//    bb:= ({!!!a type cast? =>} {pMesh(}ob.data).bb; 
//    if bb=0 then
//    begin 
//      tex_space_mesh(ob.data); 
//      bb:= ({!!!a type cast? =>} {pMesh(}ob.data).bb; 
//    end;
//  end;
//  else

//  function ELEM3{!!!3 unknown typedef}: if; 
//  begin
//    bb:= ({!!!a type cast? =>} {pCurve(}ob.data).bb; 
//    if bb=0 then
//    begin 
//      makeDispList(ob); 
//      bb:= ({!!!a type cast? =>} {pCurve(}ob.data).bb; 
//    end;
//  end
//  else
//  if ob.type=OB_MBALL then
//  begin 
//    bb:= ob.bb; 
//    if bb=0 then
//    begin 
//      makeDispList(ob); 
//      bb:= ob.bb; 
//    end;
//  end;
//  else

//  function ELEM{!!!3 unknown typedef}: if; 
//  begin
//    me:= get_mesh(ob); 
//    if me<>0 then
//    bb:= me.bb; 
//  end;
//  else
//  begin 
//    drawcube(); 
//    exit;
//  end;
//  if bb=0 then
//  exit;
//  vec:= bb.vec[0]; 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(vec); 
//  glVertex3fv(vec+3); 
//  glVertex3fv(vec+6); 
//  glVertex3fv(vec+9); 
//  glVertex3fv(vec); 
//  glVertex3fv(vec+12); 
//  glVertex3fv(vec+15); 
//  glVertex3fv(vec+18); 
//  glVertex3fv(vec+21); 
//  glVertex3fv(vec+12); 
//  glEnd(); 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(vec+3); 
//  glVertex3fv(vec+15); 
//  glEnd(); 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(vec+6); 
//  glVertex3fv(vec+18); 
//  glEnd(); 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(vec+9); 
//  glVertex3fv(vec+21); 
//  glEnd(); 
//end;

//procedure drawtexspace(ob: pObject); 
//var
//me: pMesh; 
//mb: pMetaBall; 
//cu: pCurve; 
//bb: BoundBox; 
//vec: pfloat; 
//loc: pfloat; 
//size: pfloat; 
//begin

//  if ob.type=OB_MESH then
//  begin 
//    me:= ob.data; 
//    size:= me.size; 
//    loc:= me.loc; 
//  end;
//  else

//  function ELEM3{!!!3 unknown typedef}: if; 
//  begin
//    cu:= ob.data; 
//    size:= cu.size; 
//    loc:= cu.loc; 
//  end
//  else
//  if ob.type=OB_MBALL then
//  begin 
//    mb:= ob.data; 
//    size:= mb.size; 
//    loc:= mb.loc; 
//  end;
//  else
//  exit;
//  bb.vec[0][0]:= bb.vec[1][0]:=bb.vec[2][0]:=bb.vec[3][0]:=loc[0]-size[0]; 
//  bb.vec[4][0]:= bb.vec[5][0]:=bb.vec[6][0]:=bb.vec[7][0]:=loc[0]+size[0]; 
//  bb.vec[0][1]:= bb.vec[1][1]:=bb.vec[4][1]:=bb.vec[5][1]:=loc[1]-size[1]; 
//  bb.vec[2][1]:= bb.vec[3][1]:=bb.vec[6][1]:=bb.vec[7][1]:=loc[1]+size[1]; 
//  bb.vec[0][2]:= bb.vec[3][2]:=bb.vec[4][2]:=bb.vec[7][2]:=loc[2]-size[2]; 
//  bb.vec[1][2]:= bb.vec[2][2]:=bb.vec[5][2]:=bb.vec[6][2]:=loc[2]+size[2]; 
//  setlinestyle(2); 

//  vec:= bb.vec[0]; 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(vec); 
//  glVertex3fv(vec+3); 
//  glVertex3fv(vec+6); 
//  glVertex3fv(vec+9); 
//  glVertex3fv(vec); 
//  glVertex3fv(vec+12); 
//  glVertex3fv(vec+15); 
//  glVertex3fv(vec+18); 
//  glVertex3fv(vec+21); 
//  glVertex3fv(vec+12); 
//  glEnd(); 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(vec+3); 
//  glVertex3fv(vec+15); 
//  glEnd(); 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(vec+6); 
//  glVertex3fv(vec+18); 
//  glEnd(); 
//  glBegin(GL_LINE_STRIP); 

//  glVertex3fv(vec+9); 
//  glVertex3fv(vec+21); 
//  glEnd(); 
//  setlinestyle(0); 
//end;

//var {was static}
//warning_recursive: integer;

//procedure draw_object(base: pBase); 
//var
//paf: pPartEff; 
//ob: pObject; 
//cu: pCurve; 
//lf: pLife; 
//me: pMesh; 
//elems: ListBase; 
//ce: pCfraElem; 
//cfraont: single;
//axsize: single;

//rect: puint; 
//col: cardinal;

//sel: integer; 
//drawtype: integer; 
//colindex: integer;

//ipoflag: integer; 
//dt: smallint; 
//dtx: smallint; 
//zbufoff: smallint;

//str: array [0..3] of char;
//temp: array [0..Pred(7),0..2] of single;
//ma: pMaterial;

//ma: pMaterial;

//nl: pNetLink;

//begin

//  axsize:=1.0; 

//  col:=0; 
//  warning_recursive:=0; 

//  colindex:=0; 

//  zbufoff:=0; 

//  ob:= base.object; 
//  (* keys tekenen? *)
//  if base=BASACT)or((base.flag and (SELECT+BA_WASSEL)) then
//  begin 
//    if warning_recursive=0)and(ob<>G.obedit then
//    begin 
//      if ob.ipo)and(ob.ipo.showkey)and((ob.ipoflag and OB_DRAWKEY) then
//      begin 
//        warning_recursive:= 1; 
//        elems.first:= elems.last:=0; 
//        make_cfra_list(ob.ipo, and elems); 
//        cfraont:= CFRA; 
//        drawtype:= G.vd.drawtype; 
//        if drawtype>OB_WIRE then
//        G.vd.drawtype:= OB_WIRE; 
//        sel:= base.flag; 
//        memcpy(temp,@ob.loc,7*3*sizeof(single));
//        ipoflag:= ob.ipoflag; 
//        ob.ipoflag:= ob.ipoflag and ( not OB_OFFS_OB); 
//        set_no_parent_ipo(1); 
//        disable_speed_curve(1); 

//        if (ob.ipoflag and OB_DRAWKEYSEL)=0 then
//        begin 
//          ce:= elems.first; 
//          while ce
//          do
//          begin 
//            if {not}0=ce.sel then
//            begin 
//              CFRA:= ce.cfra div G.scene.r.framelen; 
//              base.flag:= 0; 
//              where_is_object_time(ob,CFRA); 
//              draw_object(base); 
//            end;
//            ce:= ce.next; 
//          end;
//        end;
//        ce:= elems.first; 
//        while ce
//        do
//        begin 
//          if ce.sel<>0 then
//          begin 
//            CFRA:= ce.cfra div G.scene.r.framelen; 
//            base.flag:= SELECT; 
//            where_is_object_time(ob,CFRA); 
//            draw_object(base); 
//          end;
//          ce:= ce.next; 
//        end;
//        set_no_parent_ipo(0); 

//        disable_speed_curve(0); 
//        base.flag:= sel; 
//        ob.ipoflag:= ipoflag; 
//        CFRA:= cfraont; 
//        memcpy(@ob.loc,temp,7*3*sizeof(single));
//        (* restore icu->curval *)
//        where_is_object(ob); 
//        G.vd.drawtype:= drawtype; 
//        freelistN( and elems); 
//        warning_recursive:= 0; 

//      end;
//    end;
//  end;
//  multmatrix(ob.obmat); 
//  (* patch? kinderen met timeoffs verprutsen ouders. Hoe los je dat op! *)
//  (* if( ((int)ob->ctime) != F_CFRA) where_is_object(ob); *)

//  (* welke wire kleur *)
//  if (G.f and (G_BACKBUFSEL+G_PICKSEL))=0 then
//  begin 
//    project_short(ob.obmat[3], and base.sx); 
//    if G.moving=1)and((base.flag and (SELECT+BA_PARSEL)) then
//    colindex:= 9; 
//    else
//    begin 
//      if BASACT=base then
//      begin 
//        if base.flag and (SELECT+BA_WASSEL) then
//        colindex:= 2; 
//      end;
//      else
//      begin 
//        if base.flag and (SELECT+BA_WASSEL) then
//        colindex:= 1; 
//      end;
//      if ob.id.lib<>0 then
//      colindex:= colindex + (3); 
//      else
//      if warning_recursive=1 then
//      colindex:= colindex + (6); 
//    end;
//    (* ZR ADDED THIS *)
//    {$ifdef BADZR252}
//    if colindex=0 then
//    begin 
//      if give_current_material(ob,ob.actcol) then
//      begin 
//        ma:=give_current_material(ob,ob.actcol); 
//        glColor3f(ma.r,ma.g,ma.b); 
//      end;
//    end;
//    else
//    begin 
//      col:= colortab[colindex]; 
//      cpack(col); 
//    end;
//    {$else}
//    col:= colortab[colindex]; 
//    cpack(col); 
//    {$endif}
//  end;
//  (* END BAD ZR *)
//  dt:= MIN2(G.vd.drawtype,ob.dt); (* maximum drawtype *)
//  if G.zbuf=0)and(dt>OB_WIRE then
//  dt:= OB_WIRE; 
//  dtx:= 0; (* faceselect uitzondering: ook solid tekenen als dt==wire *)
//  if ob=OBACT)and((G.f and (G_FACESELECT+G_VERTEXPAINT)) then
//  begin 
//    if ob.type=OB_MESH then
//    begin 
//      if G.f and G_BACKBUFSEL then
//      dt:= OB_SOLID; 
//      else
//      dt:= OB_SHADED; 
//      glClearDepth(1.0); 
//      glClear(GL_DEPTH_BUFFER_BIT); 
//      glEnable(GL_DEPTH_TEST); 
//      zbufoff:= 1; 
//    end;
//    else
//    begin 
//      if dt<OB_SOLID then
//      begin 
//        dt:= OB_SOLID; 
//        glClearDepth(1.); 
//        glClear(GL_DEPTH_BUFFER_BIT); 
//        glEnable(GL_DEPTH_TEST); 
//        zbufoff:= 1; 
//      end;
//    end;
//  end;
//  else
//  if dt>=OB_WIRE then
//  begin 

//    if dt>OB_SOLID then
//    if G.f and G_BACKBUFSEL then
//    dt:= OB_SOLID; 
//    dtx:= ob.dtx; 
//    if G.obedit=ob then
//    begin 
//      if dtx and OB_TEXSPACE then
//      dtx:= OB_TEXSPACE; 
//      else
//      dtx:= 0; 
//    end;
//    if G.f and G_DRAW_EXT then
//    begin 
//      if ob.type=OB_EMPTY)or(ob.type=OB_CAMERA)or(ob.type=OB_LAMP then
//      dt:= OB_WIRE; 
//    end;
//  end;

//  if (G.f and G_DRAW_EXT))and(dt>OB_WIRE then
//  begin 
//    case ob.type of
//      OB_MBALL:
//      begin
//        drawmball(ob,dt); 
//      end;

//    end;{case?}
//  end;
//  else
//  begin 

//    case ob.type of
//      OB_MESH:
//      begin
//        me:= ob.data; 
//        if base.flag and OB_RADIO then
//        ; 
//        else
//        if ob=G.obedit)or((G.obedit)and(ob.data=G.obedit.data) then
//        begin 
//          if dt<=OB_WIRE then
//          drawmeshwire(ob); 
//          else
//          begin 
//            if me.flag and ME_SMESH then
//            begin 
//              init_gl_materials(ob); 
//              two_sided(0); 
//              drawDispListsolid(@me.disp,ob); 
//              drawmeshwire(ob); 
//            end;
//            else
//            drawmeshsolid(ob,0); 
//          end;
//          if ob=G.obedit)and((G.f and G_PROPORTIONAL) then
//          DRAW_PROP_CIRCLE(); 
//        end;
//        else
//        begin 
//          ma:=give_current_material(ob,1); 
//          if dt=OB_BOUNDBOX then
//          drawboundbox(ob); 
//          else
//          if dt=OB_WIRE then
//          drawmeshwire(ob); 
//          else
//          if ma)and((ma.mode and MA_HALO) then
//          drawmeshwire(ob); 
//          else
//          if ({!!!a type cast? =>} {pMesh(}ob.data).tface then
//          begin 
//            if (G.f and (G_BACKBUFSEL)) then
//            drawmeshsolid(ob,0); 
//            else
//            draw_tface_mesh(ob,ob.data,dt); 
//          end;
//          else
//          drawDispList(ob,dt); 
//        end;
//        if ob<>G.obedit)and((G.f and (G_BACKBUFSEL+G_PICKSEL))=0 then
//        begin 
//          if paf:=give_parteff(ob) then
//          begin 
//            if col<>0 then
//            cpack($FFFFFF); 
//            (* zichtbaarheid *)
//            if paf.flag and PAF_STATIC then
//            draw_static_particle_system(ob,paf); 
//            else
//            draw_particle_system(ob,paf); 
//            cpack(col); 
//          end;
//        end;
//      end;
//      OB_FONT:
//      begin
//        cu:= ob.data; 
//        if ob=G.obedit then
//        begin 
//          tekentextcurs(); 
//          cpack($FFFF90); 
//          drawDispList(ob,OB_WIRE); 
//        end;
//        else
//        if dt=OB_BOUNDBOX then
//        drawboundbox(ob); 
//        else
//        if boundbox_clip(ob.obmat,cu.bb) then
//        drawDispList(ob,dt); 
//      end;
//      OB_CURVE,
//      OB_SURF:
//      begin
//        cu:= ob.data; (* een pad niet solid tekenen: wel dus!!! *)
//        (* if(cu->flag & CU_PATH) if(dt>OB_WIRE) dt= OB_WIRE; *)
//        if ob=G.obedit then
//        begin 
//          drawnurb(ob,editNurb.first,dt); 
//          if (G.f and G_PROPORTIONAL) then
//          DRAW_PROP_CIRCLE(); 
//        end;
//        else
//        if dt=OB_BOUNDBOX then
//        drawboundbox(ob); 
//        else
//        if boundbox_clip(ob.obmat,cu.bb) then
//        drawDispList(ob,dt); 
//      end;
//      OB_MBALL:
//      begin
//        if ob=G.obedit then
//        drawmball(ob,dt); 
//        else
//        if dt=OB_BOUNDBOX then
//        drawboundbox(ob); 
//        else
//        drawmball(ob,dt); 
//      end;
//      OB_EMPTY:
//      begin
//        drawaxes(1.0); 
//      end;
//      OB_LAMP:
//      (* doet loadmatrix *)
//      begin
//        drawlamp(ob); 
//      end;
//      OB_CAMERA:
//      begin
//        drawcamera(ob); 
//      end;
//      OB_LATTICE:
//      begin
//        drawlattice(ob); 
//        if ob=G.obedit)and((G.f and G_PROPORTIONAL) then
//        DRAW_PROP_CIRCLE(); 
//      end;
//      OB_SECTOR:
//      begin
//        drawsector(ob,dt,col); 
//      end;
//      OB_LIFE:
//      begin
//        drawlife(ob,dt,col); 
//      end;
//      OB_IKA:
//      begin
//        draw_ika(ob,base.flag and SELECT); 
//      end;
//      else
//      begin
//        drawaxes(1.0); 
//      end;
//    end;{case?}
//  end;

//  (* draw extra: na gewone draw ivm makeDispList *)
//  if dtx<>0 then
//  begin 
//    if G.f and G_SIMULATION then
//    ; 
//    else
//    if dtx and OB_AXIS then
//    begin 
//      if ob.type=OB_LIFE then
//      begin 
//        lf:= ob.data; 
//        axsize:= lf.axsize; 
//      end;
//      drawaxes(axsize); 
//    end;
//    if dtx and OB_BOUNDBOX then
//    drawboundbox(ob); 
//    if dtx and OB_TEXSPACE then
//    drawtexspace(ob); 
//    if dtx and OB_DRAWNAME then
//    begin 
//      if ob.type=OB_LAMP then
//      glRasterPos3fv(ob.obmat[3]); 
//      else
//      glRasterPos3f(0.0,0.0,0.0); 
//      str[0]:= ' '; 
//      str[1]:= 0; 
//      fmsetfont(G.font); 
//      fmprstr(str); 
//      fmprstr(ob.id.name+2); 
//    end;
//    if dtx and OB_DRAWIMAGE then
//    drawDispListwire(@ob.disp); 
//  end;
//  loadmatrix(G.vd.viewmat); 

//  if zbufoff<>0 then
//  glDisable(GL_DEPTH_TEST); 

//  if warning_recursive<>0 then
//  exit;
//  if base.flag and OB_FROMDUPLI then
//  exit;
//  if base.flag and OB_RADIO then
//  exit;
//  if G.f and G_SIMULATION then
//  exit;

//  if (G.f and (G_BACKBUFSEL+G_PICKSEL))=0 then
//  begin 
//    (* hulplijnen e.d. *)
//    if ob.parent)and((ob.parent.lay and G.vd.lay) then
//    begin 
//      setlinestyle(3); 
//      LINE3F(ob.obmat[3],ob.orig); 
//      setlinestyle(0); 
//    end;
//    (* object centers *)
//    if G.zbuf<>0 then
//    glDisable(GL_DEPTH_TEST); 
//    if ob.type=OB_LAMP then
//    begin 
//      if ob.id.lib<>0 then
//      begin 
//        if base.flag and SELECT then
//        rect:= rectllib_sel; 
//        else
//        rect:= rectllib_desel; 
//      end;
//      else
//      if ob.id.us>1 then
//      begin 
//        if base.flag and SELECT then
//        rect:= rectlus_sel; 
//        else
//        rect:= rectlus_desel; 
//      end;
//      else
//      begin 
//        if base.flag and SELECT then
//        rect:= rectl_sel; 
//        else
//        rect:= rectl_desel; 
//      end;
//      tekenrect9(ob.obmat[3],rect); 
//    end;
//    else
//    begin 
//      if ob.id.lib)or(ob.id.us>1 then
//      begin 
//        if base.flag and SELECT then
//        rect:= rectu_sel; 
//        else
//        rect:= rectu_desel; 
//      end;
//      else
//      begin 
//        if base.flag and SELECT then
//        rect:= rect_sel; 
//        else
//        rect:= rect_desel; 
//      end;
//      tekenrect4(ob.obmat[3],rect); 
//    end;
//    if G.zbuf<>0 then
//    glEnable(GL_DEPTH_TEST); 
//    if ob.network.first<>0 then
//    begin 
//      nl:=ob.network.first; 
//      setlinestyle(3); 
//      while nl
//      do
//      begin 
//        LINE3F(ob.obmat[3],nl.ob.obmat[3]); 
//        nl:= nl.next; 
//      end;
//      setlinestyle(0); 
//    end;
//  end;
//  else
//  if (G.f and (G_VERTEXPAINT or G_FACESELECT))=0 then
//  begin 
//    glBegin(GL_POINTS); 
//    glVertex3fv(ob.obmat[3]); 
//    glEnd(); 
//  end;
//end;

//procedure draw_object_ext(base: pBase); 
//var
//tempsa: pScrArea; 
//sa: pScrArea; 
//vd: pView3D; 
//begin

//  if G.vd=0 then
//  exit;
//  if G.vd.drawtype>OB_WIRE then
//  begin 
//    G.zbuf:= 1; 
//    glEnable(GL_DEPTH_TEST); 
//  end;
//  G.f:= G.f or (G_DRAW_EXT); 
//  glDrawBuffer(GL_FRONT); 
//  tempsa:= curarea; 
//  sa:= G.curscreen.areabase.first; 
//  (* alle views aflopen *)
//  while sa
//  do
//  begin 
//    if sa.spacetype=SPACE_VIEW3D then
//    begin 
//      (* er wordt beperkt in beide buffers getekend: selectbuffer! *)
//      vd:= sa.spacedata.first; 
//      if base.lay and vd.lay then
//      begin 

//        areawinset(sa.win); 

//        {$if defined(BEOS) or defined(WINDOWS) or defined(PPC) or defined(MESA31)}
//        glDrawBuffer(GL_FRONT); 

//        draw_object(base); 
//        sa.win_swap:= WIN_FRONT_OK; 
//        {$else}

//        if sa.win_swap=WIN_EQUAL then
//        glDrawBuffer(GL_FRONT_AND_BACK); 
//        else
//        begin 
//          glDrawBuffer(GL_FRONT); 
//          sa.win_swap:= WIN_FRONT_OK; 
//        end;
//        draw_object(base); 
//        {$endif}
//      end;
//    end;
//    sa:= sa.next; 
//  end;
//  if curarea<>tempsa then
//  areawinset(tempsa.win); 
//  G.f:= G.f and ( not G_DRAW_EXT); 
//  glDrawBuffer(GL_BACK); 

//  if G.zbuf<>0 then
//  begin 
//    G.zbuf:= 0; 
//    glDisable(GL_DEPTH_TEST); 
//  end;
//end;

end.
