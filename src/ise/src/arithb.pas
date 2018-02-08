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
(* formules voor blender   *)
unit arithb;

interface

uses
ctypes, cfuncs;

type
  Mat4 = array [0..3,0..3] of single;

procedure Mat4One(var m: Mat4);
procedure Mat4CpyMat4(m1, m2: pcfloat);
procedure Mat4MulFloat3(m: psingle;  f: single);

function VecLenf(v1: psingle;  v2: psingle): single;

implementation

(*
 i_window(left, right, bottom, top, , ,mat)
 Mat3Transp(mat);     Mat4Transp(mat);
  Mat3Inv(mat1,mat2)     Mat4Inv(mat1,mat2)  (mat1= inv mat2, is wel met 3x3 berekend)
  Mat4InvGG(mat1,mat2)        (mat1= inv mat2 uit GraphicGems)
 Mat4Invert(mat1, mat2)  DEZE IS ECHT GOED
 Mat3Adj(mat1,mat2)
 Mat3MulMat3(mat1,mat2,mat3)   Mat4MulMat4(mat1,mat2,mat3)  (mat1=mat2*mat3)
 Mat3MulSerie(answ, m1, m2, m3....)  Mat4MulSerie(answ, m1, m2, m3....)
 
 Mat4CpyMat4(mat1,mat2)        (mat1=mat2)
 Mat3CpyMat4(mat1,mat2)
 Mat4CpyMat3(mat1,mat2)        (WEL clear)
 Mat3CpyMat3(mat1,mat2)
 Mat4SwapMat4(mat1,mat2);
 
 Mat3Clr(mat)      Mat4Clr(mat)
 Mat3One(mat)      Mat4One(mat)
 Mat3MulVecfl(mat,vec)    Mat4MulVecfl(mat,vec)   (vec is 3)
 Mat4MulVec4fl(mat,vec)     (vec is 4)
 Mat3MulVec(mat,vec)     Mat4MulVec(mat,vec) (vec is 3)
 
 Mat3MulFloat(mat,f)     Mat4MulFloat(mat,f)
 Mat4MulFloat3(m,f)      (alleen de scale component )
 Mat3Ortho(mat)      Mat4Ortho(mat)
 VecStar(mat, vec)  matrix wordt Stermatrix van vec, soort uitprodukt met X-Y-Z-assen
short EenheidsMat(mat)        (mat3)
 printmatrix3(mat)      printmatrix4(mat)
 
 QuatToMat3(q,m)
 Mat3ToQuat(m, q)
 QuatMul(q1,q2,q3)      (q1=q2*q3)
 NormalQuat(q)
single Normalise(n)
 VecCopyf(v1,v2)       (v1=v2)
void VecLen(v1,v2)         (afstand tussen twee punten)
single VecLenf(v1,v2)
 VecAddf(v, v1,v2)
 VecSubf(v, v1,v2)
 VecMidf(v, v1,v2)
 VecMulf(v1,f)
 Crossf(vecout,vec1,vec2)
single Inpf(vec1,vec2)
 CalcNormShort(v1,v2,v3,n)
 CalcNormLong(v1,v2,v3,n)
 CalcNormFloat(v1,v2,v3,n)
 
 CalcCent3f(cent, v1, v2, v3) CalcCent4f(cent, v1, v2, v3, v4)
 
single Sqrt3f(f)       (derdemachts wortel)
single Sqrt3d(d)
single DistVL2Dfl(v1,v2,v3)  (afstand v1 tot lijn v2-v3 :GEEN LIJNSTUK!)
single PdistVL2Dfl(v1,v2,v3)  (pointdist v1 tot lijn v2-v3 :LIJNSTUK!)
single AreaF2Dfl(v1,v2,v3)      (oppervlakte van een 2D driehoek)
single AreaQ3Dfl(v1, v2, v3, v4)     (alleen bolle vierhoeken)
single AreaT3Dfl(v1, v2, v3)      (triangles)
single AreaPoly3Dfl(nr, verts, normal)     (met trapezium regel)
 MinMaxRGB(c)
 Spec(inpr,spec) 
*)

(* ************************ FUNKTIES **************************** *)

//const
//SMALL_NUMBER = 1.e-8; 

{$ifdef WINDOWS}

//implementation

//procedure srand48(s: uint); 
//begin
//  srand(s); 
//end;

//function drand48: single; 
//begin
//  begin
//    result:= (( {single(}rand()) div RAND_MAX); 
//    exit;
//  end;
//end;
{$endif}

//function safacos(fac: single): single; 
//begin
//  if fac<=-1.0 then
//  begin
//    result:= M_PI; 
//    exit;
//  end;
//  else
//  if fac>=1.0 then
//  begin
//    result:= 0.0; 
//    exit;
//  end;
//  else
//  begin
//    result:= facos(fac); 
//    exit;
//  end;
//end;

//function safsqrt(fac: single): single; 
//begin
//  if fac<=0.0 then
//  begin
//    result:= 0.0; 
//    exit;
//  end;
//  begin
//    result:= fsqrt(fac); 
//    exit;
//  end;
//end;

//function Normalise(n: psingle): single;
//var
//d: single; 
//begin

//  d:= n[0]*n[0]+n[1]*n[1]+n[2]*n[2]; (* FLT_EPSILON is too large! A larger value causes normalise errors in a scaled down utah teapot *)
//  if d>0.$1 then
//  begin 
//    d:= fsqrt(d); 
//    n[0]:= n[0] div (d); 
//    n[1]:= n[1] div (d); 
//    n[2]:= n[2] div (d); 
//  end;
//  else
//  begin 
//    n[0]:= n[1]:=n[2]:=0.0; 
//    d:= 0.0; 
//  end;
//  begin
//    result:= d; 
//    exit;
//  end;
//end;

//procedure Crossf(c: psingle;  a: psingle;  b: psingle);
//begin
//  c[0]:= a[1]*b[2]-a[2]*b[1]; 
//  c[1]:= a[2]*b[0]-a[0]*b[2]; 
//  c[2]:= a[0]*b[1]-a[1]*b[0]; 
//end;

//function Inpf(v1: psingle;  v2: psingle): single;
//begin
//  begin
//    result:= v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2]; 
//    exit;
//  end;
//end;
//void{!!!e unknown token}
//var
//mat: array [0..,0..2] of single;
//begin 
//  t: single; 
//  t:= mat[0][1]; 
//  mat[0][1]:= mat[1][0]; 
//  mat[1][0]:= t; 
//  t:= mat[0][2]; 
//  mat[0][2]:= mat[2][0]; 
//  mat[2][0]:= t; 
//  t:= mat[1][2]; 
//  mat[1][2]:= mat[2][1]; 
//  mat[2][1]:= t; 
//end;
//void{!!!e unknown token}
//mat: array [0..,0..3] of single;
//begin 
//  t: single; 
//  t:= mat[0][1]; 
//  mat[0][1]:= mat[1][0]; 
//  mat[1][0]:= t; 
//  t:= mat[0][2]; 
//  mat[0][2]:= mat[2][0]; 
//  mat[2][0]:= t; 
//  t:= mat[0][3]; 
//  mat[0][3]:= mat[3][0]; 
//  mat[3][0]:= t; 
//  t:= mat[1][2]; 
//  mat[1][2]:= mat[2][1]; 
//  mat[2][1]:= t; 
//  t:= mat[1][3]; 
//  mat[1][3]:= mat[3][1]; 
//  mat[3][1]:= t; 
//  t:= mat[2][3]; 
//  mat[2][3]:= mat[3][2]; 
//  mat[3][2]:= t; 
//end;

(*
 * invertmat -
 *   computes the inverse of mat and puts it in inverse.  Returns
 * TRUE on success (i.e. can always find a pivot) and FALSE on failure.
 *  Uses Gaussian Elimination with partial (maximal column) pivoting.
 *
 *     Mark Segal - 1992
 *)

//int{!!!e unknown token}
//inverse: array [0..,0..3] of single;
//mat: array [0..,0..3] of single;
//begin 
//  i: integer; 
//  j: integer; 
//  k: integer; 
//  temp: double; 
//  tempmat: array [0..3,0..3] of single;
//  max: single; 
//  maxj: integer; (* Set inverse to identity *)
//  for{while} i:=0 to 3 { i++}
//  do
//  for{while} j:=0 to 3 { j++}
//  do
//  inverse[i][j]:= 0; 
//  for{while} i:=0 to 3 { i++}
//  do
//  inverse[i][i]:= 1; (* Copy original matrix so we don't mess it up *)
//  for{while} i:=0 to 3 { i++}
//  do
//  for{while} j:=0 to 3 { j++}
//  do
//  tempmat[i][j]:= mat[i][j]; 
//  for{while} i:=0 to 3 { i++}
//  do
//  begin 
//    (* Look for row with max pivot *)
//    max:= ABS(tempmat[i][i]); 
//    maxj:= i; 
//    for{while} j:=i+1 to 3 { j++}
//    do
//    begin 
//      if ABS(tempmat[j][i])>max then
//      begin 
//        max:= ABS(tempmat[j][i]); 
//        maxj:= j; 
//      end;
//    end;
//    (* Swap rows if necessary *)
//    if maxj<>i then
//    begin 
//      for{while} k:=0 to 3 { k++}
//      do
//      begin 
//        SWAP(single,tempmat[i][k],tempmat[maxj][k]); 
//        SWAP(single,inverse[i][k],inverse[maxj][k]); 
//      end;
//    end;
//    temp:= tempmat[i][i]; 
//    if temp=0 then
//    exit(0);
//      (* No non-zero pivot *)
//    for{while} k:=0 to 3 { k++}
//    do
//    begin 
//      tempmat[i][k]:= tempmat[i][k] div (temp); 
//      inverse[i][k]:= inverse[i][k] div (temp); 
//    end;
//    for{while} j:=0 to 3 { j++}
//    do
//    begin 
//      if j<>i then
//      begin 
//        temp:= tempmat[j][i]; 
//        for{while} k:=0 to 3 { k++}
//        do
//        begin 
//          tempmat[j][k]:= tempmat[j][k] - (tempmat[i][k]*temp); 
//          inverse[j][k]:= inverse[j][k] - (inverse[i][k]*temp); 
//        end;
//      end;
//    end;
//  end;
//    exit(1);
//end;
//void{!!!e unknown token}
//inverse: array [0..,0..3] of single;
//mat: array [0..,0..3] of single;
//begin 
//  (* alleen HOEK bewarende Matrices *)
//  (* gebaseerd op GG IV pag 205 *)
//  scale: single; 
//  scale:= mat[0][0]*mat[0][0]+mat[1][0]*mat[1][0]+mat[2][0]*mat[2][0]; 
//  if scale=0.0 then
//  exit;
//  scale:= 1.0 div scale; 
//  inverse[0][0]:= scale*mat[0][0]; 
//  inverse[1][0]:= scale*mat[0][1]; 
//  inverse[2][0]:= scale*mat[0][2]; 
//  inverse[0][1]:= scale*mat[1][0]; 
//  inverse[1][1]:= scale*mat[1][1]; 
//  inverse[2][1]:= scale*mat[1][2]; 
//  inverse[0][2]:= scale*mat[2][0]; 
//  inverse[1][2]:= scale*mat[2][1]; 
//  inverse[2][2]:= scale*mat[2][2]; 
//  inverse[3][0]:= -(inverse[0][0]*mat[3][0]+inverse[1][0]*mat[3][1]+inverse[2][0]*mat[3][2]); 
//  inverse[3][1]:= -(inverse[0][1]*mat[3][0]+inverse[1][1]*mat[3][1]+inverse[2][1]*mat[3][2]); 
//  inverse[3][2]:= -(inverse[0][2]*mat[3][0]+inverse[1][2]*mat[3][1]+inverse[2][2]*mat[3][2]); 
//  inverse[0][3]:= inverse[1][3]:=inverse[2][3]:=0.0; 
//  inverse[3][3]:= 1.0; 
//  (* transpose en scale *)
//end;
//void{!!!e unknown token}
//type
//Matrix4 = record
//end;
//begin 
//  var
//  a: smallint; 
//  b: smallint; 
//  det: single; 
//  mat1: array [0..2,0..2] of single;
//  mat2: array [0..2,0..2] of single;

//  procedure Mat3Inv; 

//  procedure Mat3CpyMat4; 

//  procedure Mat4CpyMat3; 
//  Mat3CpyMat4(mat2,m2); 
//  Mat3Inv(mat1,mat2); 
//  Mat4CpyMat3(m1,mat1); 
//end;
//single{!!!e unknown token}
//a: single; 
//b: single; 
//c: single; 
//d: single; 
//begin 
//  begin
//    result:= a*d-b*c; 
//    exit;
//  end;
//end;
//single{!!!e unknown token}
//a1: single; 
//a2: single; 
//a3: single; 
//b1: single; 
//b2: single; 
//b3: single; 
//c1: single; 
//c2: single; 
//c3: single; 
//begin 
//  ans: single; 
//  ans:= a1*Det2x2(b2,b3,c2,c3)-b1*Det2x2(a2,a3,c2,c3)+c1*Det2x2(a2,a3,b2,b3); 
//  begin
//    result:= ans; 
//    exit;
//  end;
//end;
//single{!!!e unknown token}
//m: array [0..,0..3] of single;
//begin 
//  ans: single; 
//  a1: single; 
//  a2: single; 
//  a3: single; 
//  a4: single; 
//  b1: single; 
//  b2: single; 
//  b3: single; 
//  b4: single; 
//  c1: single; 
//  c2: single; 
//  c3: single; 
//  c4: single; 
//  d1: single; 
//  d2: single; 
//  d3: single; 
//  d4: single; 
//  a1:= m[0][0]; 
//  b1:= m[0][1]; 
//  c1:= m[0][2]; 
//  d1:= m[0][3]; 
//  a2:= m[1][0]; 
//  b2:= m[1][1]; 
//  c2:= m[1][2]; 
//  d2:= m[1][3]; 
//  a3:= m[2][0]; 
//  b3:= m[2][1]; 
//  c3:= m[2][2]; 
//  d3:= m[2][3]; 
//  a4:= m[3][0]; 
//  b4:= m[3][1]; 
//  c4:= m[3][2]; 
//  d4:= m[3][3]; 
//  ans:= a1*Det3x3(b2,b3,b4,c2,c3,c4,d2,d3,d4)-b1*Det3x3(a2,a3,a4,c2,c3,c4,d2,d3,d4)+c1*Det3x3(a2,a3,a4,b2,b3,b4,d2,d3,d4)-d1*Det3x3(a2,a3,a4,b2,b3,b4,c2,c3,c4); 
//  begin
//    result:= ans; 
//    exit;
//  end;
//end;
//void{!!!e unknown token}(* out = ADJ(in) *)
//out: array [0..,0..3] of single;
//in: array [0..,0..3] of single;
//begin 
//  a1: single; 
//  a2: single; 
//  a3: single; 
//  a4: single; 
//  b1: single; 
//  b2: single; 
//  b3: single; 
//  b4: single; 
//  c1: single; 
//  c2: single; 
//  c3: single; 
//  c4: single; 
//  d1: single; 
//  d2: single; 
//  d3: single; 
//  d4: single; 
//  a1:= in[0][0]; 
//  b1:= in[0][1]; 
//  c1:= in[0][2]; 
//  d1:= in[0][3]; 
//  a2:= in[1][0]; 
//  b2:= in[1][1]; 
//  c2:= in[1][2]; 
//  d2:= in[1][3]; 
//  a3:= in[2][0]; 
//  b3:= in[2][1]; 
//  c3:= in[2][2]; 
//  d3:= in[2][3]; 
//  a4:= in[3][0]; 
//  b4:= in[3][1]; 
//  c4:= in[3][2]; 
//  d4:= in[3][3]; 
//  out[0][0]:= Det3x3(b2,b3,b4,c2,c3,c4,d2,d3,d4); 
//  out[1][0]:= -Det3x3(a2,a3,a4,c2,c3,c4,d2,d3,d4); 
//  out[2][0]:= Det3x3(a2,a3,a4,b2,b3,b4,d2,d3,d4); 
//  out[3][0]:= -Det3x3(a2,a3,a4,b2,b3,b4,c2,c3,c4); 
//  out[0][1]:= -Det3x3(b1,b3,b4,c1,c3,c4,d1,d3,d4); 
//  out[1][1]:= Det3x3(a1,a3,a4,c1,c3,c4,d1,d3,d4); 
//  out[2][1]:= -Det3x3(a1,a3,a4,b1,b3,b4,d1,d3,d4); 
//  out[3][1]:= Det3x3(a1,a3,a4,b1,b3,b4,c1,c3,c4); 
//  out[0][2]:= Det3x3(b1,b2,b4,c1,c2,c4,d1,d2,d4); 
//  out[1][2]:= -Det3x3(a1,a2,a4,c1,c2,c4,d1,d2,d4); 
//  out[2][2]:= Det3x3(a1,a2,a4,b1,b2,b4,d1,d2,d4); 
//  out[3][2]:= -Det3x3(a1,a2,a4,b1,b2,b4,c1,c2,c4); 
//  out[0][3]:= -Det3x3(b1,b2,b3,c1,c2,c3,d1,d2,d3); 
//  out[1][3]:= Det3x3(a1,a2,a3,c1,c2,c3,d1,d2,d3); 
//  out[2][3]:= -Det3x3(a1,a2,a3,b1,b2,b3,d1,d2,d3); 
//  out[3][3]:= Det3x3(a1,a2,a3,b1,b2,b3,c1,c2,c3); 
//end;
//void{!!!e unknown token}(* van Graphic Gems I, out= INV(in)  *)
//out: array [0..,0..3] of single;
//in: array [0..,0..3] of single;
//begin 
//  i: integer; 
//  j: integer; 
//  det: single; 

//  function det4x4: single; 
//  (* calculate the adjoint matrix *)
//  Mat4Adj(out,in); 
//  det:= Det4x4(out); 
//  if fabs(det)<SMALL_NUMBER then
//  begin 
//    exit;
//  end;
//  (* scale the adjoint matrix to get the inverse *)
//  for{while} i:=0 to 3 { i++}
//  do
//  for{while} j:=0 to 3 { j++}
//  do
//  out[i][j]:= out[i][j] div det; 
//  (* de laatste factor is niet altijd 1. Hierdoor moet eigenlijk nog gedeeld worden *)
//end;
//void{!!!e unknown token}
//m1: array [0..,0..2] of single;
//m2: array [0..,0..2] of single;
//begin 
//  a: smallint; 
//  b: smallint; 
//  det: single; 

//  procedure Mat3Adj; 
//  (* eerst adjoint *)
//  Mat3Adj(m1,m2); 
//  det:= m2[0][0]*(m2[1][1]*m2[2][2]-m2[1][2]*m2[2][1])-m2[1][0]*(m2[0][1]*m2[2][2]-m2[0][2]*m2[2][1])+m2[2][0]*(m2[0][1]*m2[1][2]-m2[0][2]*m2[1][1]); (* dan det oude mat! *)
//  if det=0 then
//  det:= 1; 
//  det:= 1 div det; 
//  for{while} a:=0 to 2 {a++}
//  do
//  begin 
//    for{while} b:=0 to 2 {b++}
//    do
//    begin 
//      m1[a][b]:= m1[a][b] * (det); 
//    end;
//  end;
//end;
//void{!!!e unknown token}
//m1: array [0..,0..2] of single;
//m: array [0..,0..2] of single;
//begin 
//  m1[0][0]:= m[1][1]*m[2][2]-m[1][2]*m[2][1]; 
//  m1[0][1]:= -m[0][1]*m[2][2]+m[0][2]*m[2][1]; 
//  m1[0][2]:= m[0][1]*m[1][2]-m[0][2]*m[1][1]; 
//  m1[1][0]:= -m[1][0]*m[2][2]+m[1][2]*m[2][0]; 
//  m1[1][1]:= m[0][0]*m[2][2]-m[0][2]*m[2][0]; 
//  m1[1][2]:= -m[0][0]*m[1][2]+m[0][2]*m[1][0]; 
//  m1[2][0]:= m[1][0]*m[2][1]-m[1][1]*m[2][0]; 
//  m1[2][1]:= -m[0][0]*m[2][1]+m[0][1]*m[2][0]; 
//  m1[2][2]:= m[0][0]*m[1][1]-m[0][1]*m[1][0]; 
//end;
//void{!!!e unknown token}
//m1: psingle;
//m2: psingle;
//m3: psingle;
//begin 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[4]+m2[2]*m3[8]+m2[3]*m3[12]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[5]+m2[2]*m3[9]+m2[3]*m3[13]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[6]+m2[2]*m3[10]+m2[3]*m3[14]; 
//  m1[3]:= m2[0]*m3[3]+m2[1]*m3[7]+m2[2]*m3[11]+m2[3]*m3[15]; 
//  m1:= m1 + (4); 
//  m2:= m2 + (4); 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[4]+m2[2]*m3[8]+m2[3]*m3[12]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[5]+m2[2]*m3[9]+m2[3]*m3[13]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[6]+m2[2]*m3[10]+m2[3]*m3[14]; 
//  m1[3]:= m2[0]*m3[3]+m2[1]*m3[7]+m2[2]*m3[11]+m2[3]*m3[15]; 
//  m1:= m1 + (4); 
//  m2:= m2 + (4); 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[4]+m2[2]*m3[8]+m2[3]*m3[12]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[5]+m2[2]*m3[9]+m2[3]*m3[13]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[6]+m2[2]*m3[10]+m2[3]*m3[14]; 
//  m1[3]:= m2[0]*m3[3]+m2[1]*m3[7]+m2[2]*m3[11]+m2[3]*m3[15]; 
//  m1:= m1 + (4); 
//  m2:= m2 + (4); 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[4]+m2[2]*m3[8]+m2[3]*m3[12]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[5]+m2[2]*m3[9]+m2[3]*m3[13]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[6]+m2[2]*m3[10]+m2[3]*m3[14]; 
//  m1[3]:= m2[0]*m3[3]+m2[1]*m3[7]+m2[2]*m3[11]+m2[3]*m3[15]; 
//end;
//void{!!!e unknown token}
//m1: psingle;
//m2: psingle;
//m3: psingle;
//begin 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[4]+m2[2]*m3[8]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[5]+m2[2]*m3[9]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[6]+m2[2]*m3[10]; 
//  m1[3]:= m2[0]*m3[3]+m2[1]*m3[7]+m2[2]*m3[11]+m2[3]; 
//  m1:= m1 + (4); 
//  m2:= m2 + (4); 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[4]+m2[2]*m3[8]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[5]+m2[2]*m3[9]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[6]+m2[2]*m3[10]; 
//  m1[3]:= m2[0]*m3[3]+m2[1]*m3[7]+m2[2]*m3[11]+m2[3]; 
//  m1:= m1 + (4); 
//  m2:= m2 + (4); 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[4]+m2[2]*m3[8]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[5]+m2[2]*m3[9]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[6]+m2[2]*m3[10]; 
//  m1[3]:= m2[0]*m3[3]+m2[1]*m3[7]+m2[2]*m3[11]+m2[3]; 
//end;
//void{!!!e unknown token}
//m1: psingle;
//m2: psingle;
//m3: psingle;
//begin 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[3]+m2[2]*m3[6]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[4]+m2[2]*m3[7]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[5]+m2[2]*m3[8]; 
//  m1:= m1 + (3); 
//  m2:= m2 + (3); 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[3]+m2[2]*m3[6]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[4]+m2[2]*m3[7]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[5]+m2[2]*m3[8]; 
//  m1:= m1 + (3); 
//  m2:= m2 + (3); 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[3]+m2[2]*m3[6]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[4]+m2[2]*m3[7]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[5]+m2[2]*m3[8]; 
//end;
//void{!!!e unknown token}(* m1 en m3 zijn mat4, m2 is mat3 *)
//m1: psingle;
//m2: psingle;
//m3: psingle;
//begin 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[4]+m2[2]*m3[8]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[5]+m2[2]*m3[9]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[6]+m2[2]*m3[10]; 
//  m1:= m1 + (4); 
//  m2:= m2 + (3); 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[4]+m2[2]*m3[8]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[5]+m2[2]*m3[9]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[6]+m2[2]*m3[10]; 
//  m1:= m1 + (4); 
//  m2:= m2 + (3); 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[4]+m2[2]*m3[8]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[5]+m2[2]*m3[9]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[6]+m2[2]*m3[10]; 
//end;
//void{!!!e unknown token}(* m1 en m2 zijn mat4, m3 is mat3 *)
//m1: psingle;
//m2: psingle;
//m3: psingle;
//begin 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[3]+m2[2]*m3[6]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[4]+m2[2]*m3[7]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[5]+m2[2]*m3[8]; 
//  m1:= m1 + (4); 
//  m2:= m2 + (4); 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[3]+m2[2]*m3[6]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[4]+m2[2]*m3[7]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[5]+m2[2]*m3[8]; 
//  m1:= m1 + (4); 
//  m2:= m2 + (4); 
//  m1[0]:= m2[0]*m3[0]+m2[1]*m3[3]+m2[2]*m3[6]; 
//  m1[1]:= m2[0]*m3[1]+m2[1]*m3[4]+m2[2]*m3[7]; 
//  m1[2]:= m2[0]*m3[2]+m2[1]*m3[5]+m2[2]*m3[8]; 
//end;

procedure Mat4CpyMat4(m1, m2: pcfloat);
begin
  bcopy(m2,m1,64);
end;

//void{!!!e unknown token}
//m1: psingle;
//m2: psingle;
//begin 
//  t: single; 
//  i: integer; 
//  for{while} i:=0 to 15 {i++}
//  do
//  begin 
//    t:= {*}m1^; {*}m1^:=*m2; 
//    {*}m2^:=t; 
//    inc(m1); 
//    inc(m2); 
//  end;
//end;
//type
//Mat3Row = array [0..2] of single;
//Mat4Row = array [0..3] of single;
//void{!!!e unknown token}
//var
//m1p: psingle;
//m2p: psingle;
//begin 
//  m1: pMat3Row =  {pMat3Row(}m1p; 
//  m2: pMat4Row =  {pMat4Row(}m2p; 
//  m1[0][0]:= m2[0][0]; 
//  m1[0][1]:= m2[0][1]; 
//  m1[0][2]:= m2[0][2]; 
//  m1[1][0]:= m2[1][0]; 
//  m1[1][1]:= m2[1][1]; 
//  m1[1][2]:= m2[1][2]; 
//  m1[2][0]:= m2[2][0]; 
//  m1[2][1]:= m2[2][1]; 
//  m1[2][2]:= m2[2][2]; 
//end;
//void{!!!e unknown token}(* clear *)
//m1: array [0..,0..3] of single;
//m2: array [0..,0..2] of single;
//begin 
//  m1[0][0]:= m2[0][0]; 
//  m1[0][1]:= m2[0][1]; 
//  m1[0][2]:= m2[0][2]; 
//  m1[0][3]:= 0.0; 
//  m1[1][0]:= m2[1][0]; 
//  m1[1][1]:= m2[1][1]; 
//  m1[1][2]:= m2[1][2]; 
//  m1[1][3]:= 0.0; 
//  m1[2][0]:= m2[2][0]; 
//  m1[2][1]:= m2[2][1]; 
//  m1[2][2]:= m2[2][2]; 
//  m1[2][3]:= m1[3][0]:=m1[3][1]:=m1[3][2]:=0.0; 
//  m1[3][3]:= 1.0; 
//end;
//void{!!!e unknown token}(* no clear *)
//m1: array [0..,0..3] of single;
//m2: array [0..,0..2] of single;
//begin 
//  m1[0][0]:= m2[0][0]; 
//  m1[0][1]:= m2[0][1]; 
//  m1[0][2]:= m2[0][2]; 
//  m1[1][0]:= m2[1][0]; 
//  m1[1][1]:= m2[1][1]; 
//  m1[1][2]:= m2[1][2]; 
//  m1[2][0]:= m2[2][0]; 
//  m1[2][1]:= m2[2][1]; 
//  m1[2][2]:= m2[2][2]; 
//end;
//void{!!!e unknown token}
//m1: psingle;
//m2: psingle;
//begin 
//  bcopy(m2,m1,9*sizeof(single)); 
//end;
//void{!!!e unknown token}
//answ: psingle;
//m1: psingle;
//m2: psingle;
//m3: psingle;
//m4: psingle;
//m5: psingle;
//m6: psingle;
//m7: psingle;
//m8: psingle;
//begin 
//  temp: array [0..2,0..2] of single;
//  if m1=0)or(m2=0 then
//  exit;
//  Mat3MulMat3(answ,m2,m1); 
//  if m3<>nil then
//  begin 
//    Mat3MulMat3(temp,m3,answ); 
//    if m4<>nil then
//    begin 
//      Mat3MulMat3(answ,m4,temp); 
//      if m5<>nil then
//      begin 
//        Mat3MulMat3(temp,m5,answ); 
//        if m6<>nil then
//        begin 
//          Mat3MulMat3(answ,m6,temp); 
//          if m7<>nil then
//          begin 
//            Mat3MulMat3(temp,m7,answ); 
//            if m8<>nil then
//            begin 
//              Mat3MulMat3(answ,m8,temp); 
//            end;
//            else
//            Mat3CpyMat3(answ,temp); 
//          end;
//        end;
//        else
//        Mat3CpyMat3(answ,temp); 
//      end;
//    end;
//    else
//    Mat3CpyMat3(answ,temp); 
//  end;
//end;
//void{!!!e unknown token}
//answ: psingle;
//m1: psingle;
//m2: psingle;
//m3: psingle;
//m4: psingle;
//m5: psingle;
//m6: psingle;
//m7: psingle;
//m8: psingle;
//begin 
//  temp: array [0..3,0..3] of single;
//  if m1=0)or(m2=0 then
//  exit;
//  Mat4MulMat4(answ,m2,m1); 
//  if m3<>nil then
//  begin 
//    Mat4MulMat4(temp,m3,answ); 
//    if m4<>nil then
//    begin 
//      Mat4MulMat4(answ,m4,temp); 
//      if m5<>nil then
//      begin 
//        Mat4MulMat4(temp,m5,answ); 
//        if m6<>nil then
//        begin 
//          Mat4MulMat4(answ,m6,temp); 
//          if m7<>nil then
//          begin 
//            Mat4MulMat4(temp,m7,answ); 
//            if m8<>nil then
//            begin 
//              Mat4MulMat4(answ,m8,temp); 
//            end;
//            else
//            Mat4CpyMat4(answ,temp); 
//          end;
//        end;
//        else
//        Mat4CpyMat4(answ,temp); 
//      end;
//    end;
//    else
//    Mat4CpyMat4(answ,temp); 
//  end;
//end;
//void{!!!e unknown token}
//m: psingle;
//begin 
//  bzero(m,64); 
//end;
//void{!!!e unknown token}
//m: psingle;
//begin 
//  bzero(m,36); 
//end;

procedure Mat4One(var m: Mat4);
var
  i, j: byte;
begin
  for i:=0 to 3 do
  for j:= 0 to 3 do
    m[i, j] := 0;

  m[0][0]:= 1;
  m[1][1]:=1;
  m[2][2]:=1;
  m[3][3]:=1;
end;

//void{!!!e unknown token}
//m: array [0..,0..2] of single;
//begin 
//  m[0][0]:= m[1][1]:=m[2][2]:=1.0; 
//  m[0][1]:= m[0][2]:=0.0; 
//  m[1][0]:= m[1][2]:=0.0; 
//  m[2][0]:= m[2][1]:=0.0; 
//end;
//void{!!!e unknown token}
//mat: array [0..,0..3] of single;
//vec: pinteger; 
//begin 
//  x: integer; 
//  y: integer; 
//  x:= vec[0]; 
//  y:= vec[1]; 
//  vec[0]:= x*mat[0][0]+y*mat[1][0]+mat[2][0]*vec[2]+mat[3][0]; 
//  vec[1]:= x*mat[0][1]+y*mat[1][1]+mat[2][1]*vec[2]+mat[3][1]; 
//  vec[2]:= x*mat[0][2]+y*mat[1][2]+mat[2][2]*vec[2]+mat[3][2]; 
//end;
//void{!!!e unknown token}
//mat: array [0..,0..3] of single;
//vec: psingle;
//begin 
//  x: single; 
//  y: single; 
//  x:= vec[0]; 
//  y:= vec[1]; 
//  vec[0]:= x*mat[0][0]+y*mat[1][0]+mat[2][0]*vec[2]+mat[3][0]; 
//  vec[1]:= x*mat[0][1]+y*mat[1][1]+mat[2][1]*vec[2]+mat[3][1]; 
//  vec[2]:= x*mat[0][2]+y*mat[1][2]+mat[2][2]*vec[2]+mat[3][2]; 
//end;
//void{!!!e unknown token}
//in: psingle;
//mat: array [0..,0..3] of single;
//vec: psingle;
//begin 
//  x: single; 
//  y: single; 
//  x:= vec[0]; 
//  y:= vec[1]; 
//  in[0]:= x*mat[0][0]+y*mat[1][0]+mat[2][0]*vec[2]+mat[3][0]; 
//  in[1]:= x*mat[0][1]+y*mat[1][1]+mat[2][1]*vec[2]+mat[3][1]; 
//  in[2]:= x*mat[0][2]+y*mat[1][2]+mat[2][2]*vec[2]+mat[3][2]; 
//end;
//void{!!!e unknown token}
//mat: array [0..,0..3] of single;
//vec: psingle;
//begin 
//  x: single; 
//  y: single; 
//  x:= vec[0]; 
//  y:= vec[1]; 
//  vec[0]:= x*mat[0][0]+y*mat[1][0]+mat[2][0]*vec[2]; 
//  vec[1]:= x*mat[0][1]+y*mat[1][1]+mat[2][1]*vec[2]; 
//  vec[2]:= x*mat[0][2]+y*mat[1][2]+mat[2][2]*vec[2]; 
//end;
//void{!!!e unknown token}
//mat: array [0..,0..3] of single;
//vec: psingle;
//begin 
//  x: single; 
//  y: single; 
//  z: single; 
//  x:= vec[0]; 
//  y:= vec[1]; 
//  z:= vec[2]; 
//  vec[0]:= x*mat[0][0]+y*mat[1][0]+z*mat[2][0]+mat[3][0]*vec[3]; 
//  vec[1]:= x*mat[0][1]+y*mat[1][1]+z*mat[2][1]+mat[3][1]*vec[3]; 
//  vec[2]:= x*mat[0][2]+y*mat[1][2]+z*mat[2][2]+mat[3][2]*vec[3]; 
//  vec[3]:= x*mat[0][3]+y*mat[1][3]+z*mat[2][3]+mat[3][3]*vec[3]; 
//end;
//void{!!!e unknown token}
//mat: array [0..,0..2] of single;
//vec: pinteger; 
//begin 
//  x: integer; 
//  y: integer; 
//  x:= vec[0]; 
//  y:= vec[1]; 
//  vec[0]:= x*mat[0][0]+y*mat[1][0]+mat[2][0]*vec[2]; 
//  vec[1]:= x*mat[0][1]+y*mat[1][1]+mat[2][1]*vec[2]; 
//  vec[2]:= x*mat[0][2]+y*mat[1][2]+mat[2][2]*vec[2]; 
//end;
//void{!!!e unknown token}
//mat: array [0..,0..2] of single;
//vec: psingle;
//begin 
//  x: single; 
//  y: single; 
//  x:= vec[0]; 
//  y:= vec[1]; 
//  vec[0]:= x*mat[0][0]+y*mat[1][0]+mat[2][0]*vec[2]; 
//  vec[1]:= x*mat[0][1]+y*mat[1][1]+mat[2][1]*vec[2]; 
//  vec[2]:= x*mat[0][2]+y*mat[1][2]+mat[2][2]*vec[2]; 
//end;
//void{!!!e unknown token}
//mat: array [0..,0..2] of single;
//vec: pdouble; 
//begin 
//  x: double; 
//  y: double; 
//  x:= vec[0]; 
//  y:= vec[1]; 
//  vec[0]:= x*mat[0][0]+y*mat[1][0]+mat[2][0]*vec[2]; 
//  vec[1]:= x*mat[0][1]+y*mat[1][1]+mat[2][1]*vec[2]; 
//  vec[2]:= x*mat[0][2]+y*mat[1][2]+mat[2][2]*vec[2]; 
//end;
//void{!!!e unknown token}
//mat: array [0..,0..2] of single;
//vec: psingle;
//begin 
//  x: single; 
//  y: single; 
//  x:= vec[0]; 
//  y:= vec[1]; 
//  vec[0]:= x*mat[0][0]+y*mat[0][1]+mat[0][2]*vec[2]; 
//  vec[1]:= x*mat[1][0]+y*mat[1][1]+mat[1][2]*vec[2]; 
//  vec[2]:= x*mat[2][0]+y*mat[2][1]+mat[2][2]*vec[2]; 
//end;

//procedure Mat3MulFloat(m: psingle;  f: single);
//var
//i: integer; 
//begin

//  for{while} i:=0 to Pred(9) {i++}
//  do
//  m[i]:= m[i] * (f); 
//end;

//procedure Mat4MulFloat(m: psingle;  f: single);
//var
//i: integer; 
//begin

//  for{while} i:=0 to Pred(12) {i++}
//  do
//  m[i]:= m[i] * (f); 
//  (* tot 12 tellen: zonder vector *)
//end;

(* alleen de scale component *)
procedure Mat4MulFloat3(m: psingle;  f: single);
var
i: integer;
j: integer;
begin
  for i:=0 to 2 do
    for j:=0 to 2 do
      m[4*i+j] *= f;
end;

//void{!!!e unknown token}
//mat: array [0..,0..2] of single;
//vec: psingle;
//begin 
//  mat[0][0]:= mat[1][1]:=mat[2][2]:=0.0; 
//  mat[0][1]:= -vec[2]; 
//  mat[0][2]:= vec[1]; 
//  mat[1][0]:= vec[2]; 
//  mat[1][2]:= -vec[0]; 
//  mat[2][0]:= -vec[1]; 
//  mat[2][1]:= vec[0]; 
//end;
//short{!!!e unknown token}
//mat: array [0..,0..2] of single;
//begin 
//  if mat[0][0]=1.0)and(mat[0][1]=0.0)and(mat[0][2]=0.0 then
//  if mat[1][0]=0.0)and(mat[1][1]=1.0)and(mat[1][2]=0.0 then
//  if mat[2][0]=0.0)and(mat[2][1]=0.0)and(mat[2][2]=1.0 then
//    exit(1);
//    exit(0);
//end;

//function FloatCompare(v1: psingle;  v2: psingle;  limit: single): integer;
//begin
//  if fabs(v1[0]-v2[0])<limit then
//  begin 
//    if fabs(v1[1]-v2[1])<limit then
//    begin 
//      if fabs(v1[2]-v2[2])<limit then
//        exit(1);
//    end;
//  end;
//    exit(0);
//end;
//void{!!!e unknown token}
//var
//str: pchar; 
//m: array [0..,0..3] of single;
//begin 
//  printf('%s\n',str); 
//  printf('%f %f %f %f\n',m[0][0],m[0][1],m[0][2],m[0][3]); 
//  printf('%f %f %f %f\n',m[1][0],m[1][1],m[1][2],m[1][3]); 
//  printf('%f %f %f %f\n',m[2][0],m[2][1],m[2][2],m[2][3]); 
//  printf('%f %f %f %f\n',m[3][0],m[3][1],m[3][2],m[3][3]); 
//  printf('\n'); 
//end;
//void{!!!e unknown token}
//str: pchar; 
//m: array [0..,0..2] of single;
//begin 
//  printf('%s\n',str); 
//  printf('%f %f %f\n',m[0][0],m[0][1],m[0][2]); 
//  printf('%f %f %f\n',m[1][0],m[1][1],m[1][2]); 
//  printf('%f %f %f\n',m[2][0],m[2][1],m[2][2]); 
//  printf('\n'); 
//end;

(* **************** QUATERNIONS ********** *)

//void{!!!e unknown token}
//q: psingle;
//q1: psingle;
//q2: psingle;
//begin 
//  t0: single; 
//  t1: single; 
//  t2: single; 
//  t0:= q1[0]*q2[0]-q1[1]*q2[1]-q1[2]*q2[2]-q1[3]*q2[3]; 
//  t1:= q1[0]*q2[1]+q1[1]*q2[0]+q1[2]*q2[3]-q1[3]*q2[2]; 
//  t2:= q1[0]*q2[2]+q1[2]*q2[0]+q1[3]*q2[1]-q1[1]*q2[3]; 
//  q[3]:= q1[0]*q2[3]+q1[3]*q2[0]+q1[1]*q2[2]-q1[2]*q2[1]; 
//  q[0]:= t0; 
//  q[1]:= t1; 
//  q[2]:= t2; 
//end;
//void{!!!e unknown token}
//q: psingle;
//m: psingle;
//begin 
//  q0: single; 
//  q1: single; 
//  q2: single; 
//  q3: single; 
//  qda: single; 
//  qdb: single; 
//  qdc: single; 
//  qaa: single; 
//  qab: single; 
//  qac: single; 
//  qbb: single; 
//  qbc: single; 
//  qcc: single; 
//  q0:= M_SQRT2*q[0]; 
//  q1:= M_SQRT2*q[1]; 
//  q2:= M_SQRT2*q[2]; 
//  q3:= M_SQRT2*q[3]; 
//  qda:= q0*q1; 
//  qdb:= q0*q2; 
//  qdc:= q0*q3; 
//  qaa:= q1*q1; 
//  qab:= q1*q2; 
//  qac:= q1*q3; 
//  qbb:= q2*q2; 
//  qbc:= q2*q3; 
//  qcc:= q3*q3; 
//  m[0]:= 1.0-qbb-qcc; 
//  m[1]:= qdc+qab; 
//  m[2]:= -qdb+qac; 
//  m[3]:= -qdc+qab; 
//  m[4]:= 1.0-qaa-qcc; 
//  m[5]:= qda+qbc; 
//  m[6]:= qdb+qac; 
//  m[7]:= -qda+qbc; 
//  m[8]:= 1.0-qaa-qbb; 
//end;
//void{!!!e unknown token}
//q: psingle;
//m: psingle;
//begin 
//  q0: single; 
//  q1: single; 
//  q2: single; 
//  q3: single; 
//  qda: single; 
//  qdb: single; 
//  qdc: single; 
//  qaa: single; 
//  qab: single; 
//  qac: single; 
//  qbb: single; 
//  qbc: single; 
//  qcc: single; 
//  q0:= M_SQRT2*q[0]; 
//  q1:= M_SQRT2*q[1]; 
//  q2:= M_SQRT2*q[2]; 
//  q3:= M_SQRT2*q[3]; 
//  qda:= q0*q1; 
//  qdb:= q0*q2; 
//  qdc:= q0*q3; 
//  qaa:= q1*q1; 
//  qab:= q1*q2; 
//  qac:= q1*q3; 
//  qbb:= q2*q2; 
//  qbc:= q2*q3; 
//  qcc:= q3*q3; 
//  m[0]:= 1.0-qbb-qcc; 
//  m[1]:= qdc+qab; 
//  m[2]:= -qdb+qac; 
//  m[3]:= 0.0; 
//  m[4]:= -qdc+qab; 
//  m[5]:= 1.0-qaa-qcc; 
//  m[6]:= qda+qbc; 
//  m[7]:= 0.0; 
//  m[8]:= qdb+qac; 
//  m[9]:= -qda+qbc; 
//  m[10]:= 1.0-qaa-qbb; 
//  m[11]:= m[12]:=m[13]:=m[14]:=0.0; 
//  m[15]:= 1.0; 
//end;
//void{!!!e unknown token}(* uit Sig.Proc.85 pag 253 *)
//wmat: array [0..,0..2] of single;
//q: psingle;
//begin 

//  procedure Mat3Ortho; 

//  procedure NormalQuat; 
//  tr: double; 
//  s: double; 
//  mat: array [0..2,0..2] of single; (* voor de netheid: op kopie werken *)
//  Mat3CpyMat3(mat,wmat); 
//  Mat3Ortho(mat); 
//  tr:= 0.25*(1.0+mat[0][0]+mat[1][1]+mat[2][2]); (* dit moet EN op eind NormalQuat *)
//  if tr>FLT_EPSILON then
//  begin 
//    s:= sqrt(tr); 
//    q[0]:= s; 
//    s:= s * (4.0); 
//    q[1]:= (mat[1][2]-mat[2][1]) div s; 
//    q[2]:= (mat[2][0]-mat[0][2]) div s; 
//    q[3]:= (mat[0][1]-mat[1][0]) div s; 
//  end;
//  else
//  begin 
//    q[0]:= 0.0; 
//    s:= -0.5*(mat[1][1]+mat[2][2]); 
//    if s>FLT_EPSILON then
//    begin 
//      s:= sqrt(s); 
//      q[1]:= s; 
//      q[2]:= mat[0][1] div (2*s); 
//      q[3]:= mat[0][2] div (2*s); 
//    end;
//    else
//    begin 
//      q[1]:= 0.0; 
//      s:= 0.5*(1.0-mat[2][2]); 
//      if s>FLT_EPSILON then
//      begin 
//        s:= sqrt(s); 
//        q[2]:= s; 
//        q[3]:= mat[1][2] div (2*s); 
//      end;
//      else
//      begin 
//        q[2]:= 0.0; 
//        q[3]:= 1.0; 
//      end;
//    end;
//  end;
//  NormalQuat(q); 
//end;
//void{!!!e unknown token}
//wmat: array [0..,0..2] of single;
//q: psingle;
//begin 

//  procedure Mat3Ortho; 
//  mat: array [0..2,0..2] of single;
//  matr: array [0..2,0..2] of single;
//  matn: array [0..2,0..2] of single;
//  q1: array [0..3] of single;
//  q2: array [0..3] of single;
//  hoek: single; 
//  si: single; 
//  co: single; 
//  nor: array [0..2] of single; (* voor de netheid: op kopie werken *)
//  Mat3CpyMat3(mat,wmat); 
//  Mat3Ortho(mat); 
//  nor[0]:= mat[2][1]; 
//  nor[1]:= -mat[2][0]; 
//  nor[2]:= 0.0; 
//  Normalise(nor); 
//  (* roteer z-as matrix op z-as *)
//  (* uitprodukt met (0,0,1) *)
//  co:= mat[2][2]; 
//  hoek:= 0.5*safacos(co); 
//  co:= fcos(hoek); 
//  si:= fsin(hoek); 
//  q1[0]:= co; 
//  q1[1]:= -nor[0]*si; 
//  q1[2]:= -nor[1]*si; 
//  q1[3]:= -nor[2]*si; 
//  QuatToMat3(q1,matr); 
//  (* hier negatief, waarom is een raadsel *)
//  (* roteer x-as van mat terug volgens inverse q1 *)
//  Mat3Inv(matn,matr); 
//  Mat3MulVecfl(matn,mat[0]); 
//  hoek:= 0.5*fatan2(mat[0][1],mat[0][0]); 
//  co:= fcos(hoek); 
//  si:= fsin(hoek); 
//  q2[0]:= co; 
//  q2[1]:= 0.0; 
//  q2[2]:= 0.0; 
//  q2[3]:= si; 
//  QuatMul(q,q1,q2); 
//  (* en zet de x-asssen gelijk *)
//end;
//void{!!!e unknown token}
//m: psingle;
//q: psingle;
//begin 
//  mat: array [0..2,0..2] of single;
//  Mat3CpyMat4(mat[0],m); 
//  Mat3ToQuat(mat,q); 
//end;
//void{!!!e unknown token}
//q: psingle;
//begin 
//  len: single; 
//  len:= fsqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]); 
//  if len<>0.0 then
//  begin 
//    q[0]:= q[0] div (len); 
//    q[1]:= q[1] div (len); 
//    q[2]:= q[2] div (len); 
//    q[3]:= q[3] div (len); 
//  end;
//  else
//  begin 
//    q[1]:= 1.0; 
//    q[0]:= q[2]:=q[3]:=0.0; 
//  end;
//end;
//var {was static}
//q1: array [0..3] of single;

//function vectoquat(vec: psingle;  axis: smallint;  upflag: smallint): psingle;
//var
//up: array [0..2] of single;
//q2: array [0..3] of single;
//nor: array [0..2] of single;
//fp: psingle;
//mat: array [0..2,0..2] of single;
//hoek: single; 
//si: single; 
//co: single; 
//x2: single; 
//y2: single; 
//z2: single; 
//len1: single; (* eerst roteer naar as *)
//begin

//  if axis>2 then
//  begin 
//    x2:= vec[0]; 
//    y2:= vec[1]; 
//    z2:= vec[2]; 
//    axis:= axis - (3); 
//  end;
//  else
//  begin 
//    x2:= -vec[0]; 
//    y2:= -vec[1]; 
//    z2:= -vec[2]; 
//  end;
//  q1[0]:= 1.0; 
//  q1[1]:= q1[2]:=q1[3]:=0.0; 
//  len1:= fsqrt(x2*x2+y2*y2+z2*z2); 
//  if len1=0.0 then
//  begin
//    result:= (q1); 
//    exit;
//    (* nasty! I need a good routine for this...
//      * problem is a rotation of an Y axis to the negative Y-axis for example.
//      *)
//  end;
//  if axis=0 then
//  begin 
//    (* x-as *)
//    nor[0]:= 0.0; 
//    nor[1]:= -z2; 
//    nor[2]:= y2; 
//    if fabs(y2)+fabs(z2)<0.$1 then
//    begin 
//      nor[1]:= 1.0; 
//    end;
//    co:= x2; 
//  end;
//  else
//  if axis=1 then
//  begin 
//    (* y-as *)
//    nor[0]:= z2; 
//    nor[1]:= 0.0; 
//    nor[2]:= -x2; 
//    if fabs(x2)+fabs(z2)<0.$1 then
//    begin 
//      nor[2]:= 1.0; 
//    end;
//    co:= y2; 
//  end;
//  else
//  begin 
//    (* z-as *)
//    nor[0]:= -y2; 
//    nor[1]:= x2; 
//    nor[2]:= 0.0; 
//    if fabs(x2)+fabs(y2)<0.$1 then
//    begin 
//      nor[0]:= 1.0; 
//    end;
//    co:= z2; 
//  end;
//  co:= co div (len1); 
//  Normalise(nor); 
//  hoek:= 0.5*safacos(co); 
//  si:= fsin(hoek); 
//  q1[0]:= fcos(hoek); 
//  q1[1]:= nor[0]*si; 
//  q1[2]:= nor[1]*si; 
//  q1[3]:= nor[2]*si; 
//  if axis<>upflag then
//  begin 
//    QuatToMat3(q1,mat); 
//    fp:= mat[2]; 
//    if axis=0 then
//    begin 
//      if upflag=1 then
//      hoek:= 0.5*fatan2(fp[2],fp[1]); 
//      else
//      hoek:= -0.5*fatan2(fp[1],fp[2]); 
//    end;
//    else
//    if axis=1 then
//    begin 
//      if upflag=0 then
//      hoek:= -0.5*fatan2(fp[2],fp[0]); 
//      else
//      hoek:= 0.5*fatan2(fp[0],fp[2]); 
//    end;
//    else
//    begin 
//      if upflag=0 then
//      hoek:= 0.5*fatan2(-fp[1],-fp[0]); 
//      else
//      hoek:= -0.5*fatan2(-fp[0],-fp[1]); 
//    end;
//    co:= fcos(hoek); 
//    si:= fsin(hoek) div len1; 
//    q2[0]:= co; 
//    q2[1]:= x2*si; 
//    q2[2]:= y2*si; 
//    q2[3]:= z2*si; 
//    QuatMul(q1,q2,q1); 
//  end;
//  begin
//    result:= (q1); 
//    exit;
//  end;
//end;

//procedure VecUpMat3old(vec: psingle;  mat: array [0..,0..2] of single;  axis: smallint);
//var
//inp: single; 
//up: array [0..2] of single;
//cox: smallint; 
//coy: smallint; 
//coz: smallint; (* up varieeren heeft geen zin, is eigenlijk helemaal geen up!
//  *)
//begin

//  up[0]:= 0.0; 
//  up[1]:= 0.0; 
//  up[2]:= 1.0; 
//  if axis=0 then
//  begin 
//    cox:= 0; 
//    coy:= 1; 
//    coz:= 2; 
//    (* Y up Z tr *)
//  end;
//  if axis=1 then
//  begin 
//    cox:= 1; 
//    coy:= 2; 
//    coz:= 0; 
//    (* Z up X tr *)
//  end;
//  if axis=2 then
//  begin 
//    cox:= 2; 
//    coy:= 0; 
//    coz:= 1; 
//    (* X up Y tr *)
//  end;
//  if axis=3 then
//  begin 
//    cox:= 0; 
//    coy:= 2; 
//    coz:= 1; 
//    (*  *)
//  end;
//  if axis=4 then
//  begin 
//    cox:= 1; 
//    coy:= 0; 
//    coz:= 2; 
//    (*  *)
//  end;
//  if axis=5 then
//  begin 
//    cox:= 2; 
//    coy:= 1; 
//    coz:= 0; 
//    (* Y up X tr *)
//  end;
//  mat[coz][0]:= vec[0]; 
//  mat[coz][1]:= vec[1]; 
//  mat[coz][2]:= vec[2]; 
//  Normalise( {psingle(}mat[coz]);
//  inp:= mat[coz][0]*up[0]+mat[coz][1]*up[1]+mat[coz][2]*up[2]; 
//  mat[coy][0]:= up[0]-inp*mat[coz][0]; 
//  mat[coy][1]:= up[1]-inp*mat[coz][1]; 
//  mat[coy][2]:= up[2]-inp*mat[coz][2]; 
//  Normalise( {psingle(}mat[coy]);
//  Crossf(mat[cox],mat[coy],mat[coz]); 
//end;

//procedure VecUpMat3(vec: psingle;  mat: array [0..,0..2] of single;  axis: smallint);
//var
//inp: single; 
//cox: smallint; 
//coy: smallint; 
//coz: smallint; (* up varieeren heeft geen zin, is eigenlijk helemaal geen up!
//  * zie VecUpMat3old
//  *)
//begin

//  if axis=0 then
//  begin 
//    cox:= 0; 
//    coy:= 1; 
//    coz:= 2; 
//    (* Y up Z tr *)
//  end;
//  if axis=1 then
//  begin 
//    cox:= 1; 
//    coy:= 2; 
//    coz:= 0; 
//    (* Z up X tr *)
//  end;
//  if axis=2 then
//  begin 
//    cox:= 2; 
//    coy:= 0; 
//    coz:= 1; 
//    (* X up Y tr *)
//  end;
//  if axis=3 then
//  begin 
//    cox:= 0; 
//    coy:= 2; 
//    coz:= 1; 
//    (*  *)
//  end;
//  if axis=4 then
//  begin 
//    cox:= 1; 
//    coy:= 0; 
//    coz:= 2; 
//    (*  *)
//  end;
//  if axis=5 then
//  begin 
//    cox:= 2; 
//    coy:= 1; 
//    coz:= 0; 
//    (* Y up X tr *)
//  end;
//  mat[coz][0]:= vec[0]; 
//  mat[coz][1]:= vec[1]; 
//  mat[coz][2]:= vec[2]; 
//  Normalise( {psingle(}mat[coz]);
//  inp:= mat[coz][2]; 
//  mat[coy][0]:= -inp*mat[coz][0]; 
//  mat[coy][1]:= -inp*mat[coz][1]; 
//  mat[coy][2]:= 1.0-inp*mat[coz][2]; 
//  Normalise( {psingle(}mat[coy]);
//  Crossf(mat[cox],mat[coy],mat[coz]); 
//end;

(* **************** VIEW / PROJEKTIE ********************************  *)

//void{!!!e unknown token}
//left: single; 
//right: single; 
//bottom: single; 
//top: single; single{!!!e unknown token},
//; 
//matrix: array [0..,0..3] of single;
//begin 
//  Xdelta: single; 
//  Ydelta: single; 
//  Zdelta: single; 
//  Xdelta:= right-left; 
//  Ydelta:= top-bottom; 
//  Zdelta:= -; 
//  if Xdelta=0.0)or(Ydelta=0.0)or(Zdelta=0.0 then
//  begin 
//    (* fprintf(stderr,"ortho: window width, height, or depth is 0.0\n"); *)
//    exit;
//  end;
//  Mat4One(matrix); 
//  matrix[0][0]:= 2.0 div Xdelta; 
//  matrix[3][0]:= -(right+left) div Xdelta; 
//  matrix[1][1]:= 2.0 div Ydelta; 
//  matrix[3][1]:= -(top+bottom) div Ydelta; 
//  matrix[2][2]:= -2.0 div Zdelta; 
//  matrix[3][2]:= -(+) div Zdelta; 
//  (* note: negate Z *)
//end;

//procedure i_window(left: single;  right: single;  bottom: single;  top: single;  {!!!3 unknown typedef}); 
//var
//Xdelta: single; 
//Ydelta: single; 
//Zdelta: single; 
//begin

//  Xdelta:= right-left; 
//  Ydelta:= top-bottom; 
//  Zdelta:= -; 
//  if Xdelta=0.0)or(Ydelta=0.0)or(Zdelta=0.0 then
//  begin 
//    exit;
//  end;
//  mat[0][0]:= *2.0 div Xdelta; 
//  mat[1][1]:= *2.0 div Ydelta; 
//  mat[2][0]:= (right+left) div Xdelta; 
//  mat[2][1]:= (top+bottom) div Ydelta; 
//  mat[2][2]:= -(+) div Zdelta; 
//  mat[2][3]:= -1.0; 
//  mat[3][2]:= (-2.0**) div Zdelta; 
//  mat[0][1]:= mat[0][2]:=mat[0][3]:=mat[1][0]:=mat[1][2]:=mat[1][3]:=mat[3][0]:=mat[3][1]:=mat[3][3]:=0.0; 
//  (* note: negate Z *)
//end;
//i_translate(Tx,Ty,Tz,mat); 
//Tx: single; 
//Ty: single; 
//Tz: single; 
//mat: array [0..,0..3] of single;
//begin 
//  mat[3][0]:= mat[3][0] + ((Tx*mat[0][0]+Ty*mat[1][0]+Tz*mat[2][0])); 
//  mat[3][1]:= mat[3][1] + ((Tx*mat[0][1]+Ty*mat[1][1]+Tz*mat[2][1])); 
//  mat[3][2]:= mat[3][2] + ((Tx*mat[0][2]+Ty*mat[1][2]+Tz*mat[2][2])); 
//end;
//i_multmatrix(icand,Vm); 
//icand: array [0..,0..3] of single;
//Vm: array [0..,0..3] of single;
//begin 
//  row: integer; 
//  col: integer; 
//  temp: array [0..3,0..3] of single;
//  for{while} row:=0 to 3 { row++}
//  do
//  for{while} col:=0 to 3 { col++}
//  do
//  temp[row][col]:= icand[row][0]*Vm[0][col]+icand[row][1]*Vm[1][col]+icand[row][2]*Vm[2][col]+icand[row][3]*Vm[3][col]; 
//  Mat4CpyMat4(Vm,temp); 
//end;
//i_rotate(angle,axis,mat); 
//angle: single; 
//axis: char; 
//mat: array [0..,0..3] of single;
//begin 
//  row: integer; 
//  col: integer; 
//  temp: array [0..3] of single;
//  cosine: single; 
//  sine: single; 
//  for{while} col:=0 to 3 { col++}
//  do
//  temp[col]:= 0; 
//  angle:= angle*(3.1415926535 div 180.0); 
//  cosine:= fcos(angle); 
//  sine:= fsin(angle); (* init temp to zero matrix *)
//  case axis of
//    'x',
//    'X':
//    begin
//      for{while} col:=0 to 3 { col++}
//      do
//      temp[col]:= cosine*mat[1][col]+sine*mat[2][col]; 
//      for{while} col:=0 to 3 { col++}
//      do
//      begin 
//        mat[2][col]:= -sine*mat[1][col]+cosine*mat[2][col]; 
//        mat[1][col]:= temp[col]; 
//      end;
//    end;
//    'y',
//    'Y':
//    begin
//      for{while} col:=0 to 3 { col++}
//      do
//      temp[col]:= cosine*mat[0][col]-sine*mat[2][col]; 
//      for{while} col:=0 to 3 { col++}
//      do
//      begin 
//        mat[2][col]:= sine*mat[0][col]+cosine*mat[2][col]; 
//        mat[0][col]:= temp[col]; 
//      end;
//    end;
//    'z',
//    'Z':
//    begin
//      for{while} col:=0 to 3 { col++}
//      do
//      temp[col]:= cosine*mat[0][col]+sine*mat[1][col]; 
//      for{while} col:=0 to 3 { col++}
//      do
//      begin 
//        mat[1][col]:= -sine*mat[0][col]+cosine*mat[1][col]; 
//        mat[0][col]:= temp[col]; 
//      end;
//    end;

//  end;{case?}
//end;
//i_polarview(dist,azimuth,incidence,twist,Vm); 
//dist: single; 
//azimuth: single; 
//incidence: single; 
//twist: single; 
//Vm: array [0..,0..3] of single;
//begin 
//  Mat4One(Vm); 
//  i_translate(0.0,0.0,-dist,Vm); 
//  i_rotate(-twist,'z',Vm); 
//  i_rotate(-incidence,'x',Vm); 
//  i_rotate(-azimuth,'z',Vm); 
//end;

//procedure i_lookat(vx: single;  vy: single;  vz: single;  px: single;  py: single;  pz: single;  twist: single;  mat: array [0..,0..3] of single);
//var
//sine: single; 
//cosine: single; 
//hyp: single; 
//hyp1: single; 
//dx: single; 
//dy: single; 
//dz: single; 
//mat1: array [0..3,0..3] of single;
//begin

//  Mat4One(mat); 
//  Mat4One(mat1); 
//  i_rotate(-twist,'z',mat); 
//  dx:= px-vx; 
//  dy:= py-vy; 
//  dz:= pz-vz; 
//  hyp:= dx*dx+dz*dz; 
//  hyp1:= sqrt(dy*dy+hyp); 
//  hyp:= sqrt(hyp); (* hyp squared *)
//  (* the real hyp *)
//  if hyp1<>0.0 then
//  begin 
//    (* rotate X *)
//    sine:= -dy div hyp1; 
//    cosine:= hyp div hyp1; 
//  end;
//  else
//  begin 
//    sine:= 0; 
//    cosine:= 1.0; 
//  end;
//  mat1[1][1]:= cosine; 
//  mat1[1][2]:= sine; 
//  mat1[2][1]:= -sine; 
//  mat1[2][2]:= cosine; 
//  i_multmatrix(mat1,mat); 
//  mat1[1][1]:= mat1[2][2]:=1.0; 
//  mat1[1][2]:= mat1[2][1]:=0.0; (* be careful here to reinit *)
//  (* those modified by the last *)
//  (* paragraph *)
//  if hyp<>0.0 then
//  begin 
//    (* rotate Y *)
//    sine:= dx div hyp; 
//    cosine:= -dz div hyp; 
//  end;
//  else
//  begin 
//    sine:= 0; 
//    cosine:= 1.0; 
//  end;
//  mat1[0][0]:= cosine; 
//  mat1[0][2]:= -sine; 
//  mat1[2][0]:= sine; 
//  mat1[2][2]:= cosine; 
//  i_multmatrix(mat1,mat); 
//  i_translate(-vx,-vy,-vz,mat); 
//end;(* translate viewpoint to origin *)

(* ************************************************  *)

//void{!!!e unknown token}
//mat: array [0..,0..2] of single;
//begin 
//  Normalise(mat[0]); 
//  Normalise(mat[1]); 
//  Normalise(mat[2]); 
//end;
//void{!!!e unknown token}
//mat: array [0..,0..3] of single;
//begin 
//  len: single; 
//  len:= Normalise(mat[0]); 
//  if len<>0.0 then
//  mat[0][3]:= mat[0][3] div (len); 
//  len:= Normalise(mat[1]); 
//  if len<>0.0 then
//  mat[1][3]:= mat[1][3] div (len); 
//  len:= Normalise(mat[2]); 
//  if len<>0.0 then
//  mat[2][3]:= mat[2][3] div (len); 
//end;
//void{!!!e unknown token}
//v1: psingle;
//v2: psingle;
//begin 
//  v1[0]:= v2[0]; 
//  v1[1]:= v2[1]; 
//  v1[2]:= v2[2]; 
//end;
//int{!!!e unknown token}
//v1: pinteger; 
//v2: pinteger; 
//begin 
//  x: single; 
//  y: single; 
//  z: single; 
//  x:= v1[0]-v2[0]; 
//  y:= v1[1]-v2[1]; 
//  z:= v1[2]-v2[2]; 
//  begin
//    result:= ffloor(fsqrt(x*x+y*y+z*z)); 
//    exit;
//  end;
//end;

function VecLenf(v1: psingle;  v2: psingle): single;
var
x: single;
y: single;
z: single;
begin
  x:= v1[0]-v2[0];
  y:= v1[1]-v2[1];
  z:= v1[2]-v2[2];
  exit(fsqrt(x*x+y*y+z*z));
end;

//void{!!!e unknown token}
//v: psingle;
//v1: psingle;
//v2: psingle;
//begin 
//  v[0]:= v1[0]+v2[0]; 
//  v[1]:= v1[1]+v2[1]; 
//  v[2]:= v1[2]+v2[2]; 
//end;
//void{!!!e unknown token}
//v: psingle;
//v1: psingle;
//v2: psingle;
//begin 
//  v[0]:= v1[0]-v2[0]; 
//  v[1]:= v1[1]-v2[1]; 
//  v[2]:= v1[2]-v2[2]; 
//end;
//void{!!!e unknown token}
//v: psingle;
//v1: psingle;
//v2: psingle;
//begin 
//  v[0]:= 0.5*(v1[0]+v2[0]); 
//  v[1]:= 0.5*(v1[1]+v2[1]); 
//  v[2]:= 0.5*(v1[2]+v2[2]); 
//end;

//procedure VecMulf(v1: psingle;  f: single);
//begin
//  v1[0]:= v1[0] * (f); 
//  v1[1]:= v1[1] * (f); 
//  v1[2]:= v1[2] * (f); 
//end;
//void{!!!e unknown token}(* is ook uitprodukt *)
//var
//v1: psmallint; 
//v2: psmallint; 
//v3: psmallint; 
//n: psingle;
//begin 
//  n1: array [0..2] of single;
//  n2: array [0..2] of single;
//  n1[0]:= v1[0]-v2[0]; 
//  n2[0]:= v2[0]-v3[0]; 
//  n1[1]:= v1[1]-v2[1]; 
//  n2[1]:= v2[1]-v3[1]; 
//  n1[2]:= v1[2]-v2[2]; 
//  n2[2]:= v2[2]-v3[2]; 
//  n[0]:= n1[1]*n2[2]-n1[2]*n2[1]; 
//  n[1]:= n1[2]*n2[0]-n1[0]*n2[2]; 
//  n[2]:= n1[0]*n2[1]-n1[1]*n2[0]; 
//  Normalise(n); 
//end;
//void{!!!e unknown token}
//v1: pinteger; 
//v2: pinteger; 
//v3: pinteger; 
//n: psingle;
//begin 
//  n1: array [0..2] of single;
//  n2: array [0..2] of single;
//  n1[0]:= v1[0]-v2[0]; 
//  n2[0]:= v2[0]-v3[0]; 
//  n1[1]:= v1[1]-v2[1]; 
//  n2[1]:= v2[1]-v3[1]; 
//  n1[2]:= v1[2]-v2[2]; 
//  n2[2]:= v2[2]-v3[2]; 
//  n[0]:= n1[1]*n2[2]-n1[2]*n2[1]; 
//  n[1]:= n1[2]*n2[0]-n1[0]*n2[2]; 
//  n[2]:= n1[0]*n2[1]-n1[1]*n2[0]; 
//  Normalise(n); 
//end;

//function CalcNormFloat(v1: psingle;  v2: psingle;  v3: psingle;  n: psingle): single;
//var
//n1: array [0..2] of single;
//n2: array [0..2] of single;
//begin

//  n1[0]:= v1[0]-v2[0]; 
//  n2[0]:= v2[0]-v3[0]; 
//  n1[1]:= v1[1]-v2[1]; 
//  n2[1]:= v2[1]-v3[1]; 
//  n1[2]:= v1[2]-v2[2]; 
//  n2[2]:= v2[2]-v3[2]; 
//  n[0]:= n1[1]*n2[2]-n1[2]*n2[1]; 
//  n[1]:= n1[2]*n2[0]-n1[0]*n2[2]; 
//  n[2]:= n1[0]*n2[1]-n1[1]*n2[0]; 
//  begin
//    result:= Normalise(n); 
//    exit;
//  end;
//end;

//function CalcNormFloat4(v1: psingle;  v2: psingle;  v3: psingle;  v4: psingle;  n: psingle): single;
//var
//n1: array [0..2] of single;
//n2: array [0..2] of single;
//begin(* real cross! *)

//  n1[0]:= v1[0]-v3[0]; 
//  n1[1]:= v1[1]-v3[1]; 
//  n1[2]:= v1[2]-v3[2]; 
//  n2[0]:= v2[0]-v4[0]; 
//  n2[1]:= v2[1]-v4[1]; 
//  n2[2]:= v2[2]-v4[2]; 
//  n[0]:= n1[1]*n2[2]-n1[2]*n2[1]; 
//  n[1]:= n1[2]*n2[0]-n1[0]*n2[2]; 
//  n[2]:= n1[0]*n2[1]-n1[1]*n2[0]; 
//  begin
//    result:= Normalise(n); 
//    exit;
//  end;
//end;

//procedure CalcCent3f(cent: psingle;  v1: psingle;  v2: psingle;  v3: psingle);
//begin
//  cent[0]:= 0.33333*(v1[0]+v2[0]+v3[0]); 
//  cent[1]:= 0.33333*(v1[1]+v2[1]+v3[1]); 
//  cent[2]:= 0.33333*(v1[2]+v2[2]+v3[2]); 
//end;

//procedure CalcCent4f(cent: psingle;  v1: psingle;  v2: psingle;  v3: psingle;  v4: psingle);
//begin
//  cent[0]:= 0.25*(v1[0]+v2[0]+v3[0]+v4[0]); 
//  cent[1]:= 0.25*(v1[1]+v2[1]+v3[1]+v4[1]); 
//  cent[2]:= 0.25*(v1[2]+v2[2]+v3[2]+v4[2]); 
//end;

//function Sqrt3f(f: single): single; 
//begin
//  if f=0.0 then
//    exit(0);
//  if f<0 then
//  begin
//    result:= -fexp(flog(-f) div 3); 
//    exit;
//  end;
//  else
//  begin
//    result:= fexp(flog(f) div 3); 
//    exit;
//  end;
//end;

//function Sqrt3d(d: double): double; 
//begin
//  if d=0.0 then
//    exit(0);
//  if d<0 then
//  begin
//    result:= -exp(log(-d) div 3); 
//    exit;
//  end;
//  else
//  begin
//    result:= exp(log(d) div 3); 
//    exit;
//  end;
//end;

(* afstand v1 tot lijn v2-v3 *)
(* met formule van Hesse :GEEN LIJNSTUK! *)

//function DistVL2Dfl(v1: psingle;  v2: psingle;  v3: psingle): single;
//var
//a: array [0..1] of single;
//deler: single; 
//begin

//  a[0]:= v2[1]-v3[1]; 
//  a[1]:= v3[0]-v2[0]; 
//  deler:= fsqrt(a[0]*a[0]+a[1]*a[1]); 
//  if deler=0.0 then
//    exit(0);
//  begin
//    result:= fabs((v1[0]-v2[0])*a[0]+(v1[1]-v2[1])*a[1]) div deler; 
//    exit;
//  end;
//end;

(* PointDist: WEL LIJNSTUK *)

//function PdistVL2Dfl(v1: psingle;  v2: psingle;  v3: psingle): single;
//var
//labda: single; 
//rc: array [0..1] of single;
//pt: array [0..1] of single;
//len: single; 
//begin

//  rc[0]:= v3[0]-v2[0]; 
//  rc[1]:= v3[1]-v2[1]; 
//  len:= rc[0]*rc[0]+rc[1]*rc[1]; 
//  if len=0.0 then
//  begin 
//    rc[0]:= v1[0]-v2[0]; 
//    rc[1]:= v1[1]-v2[1]; 
//    begin
//      result:= fsqrt(rc[0]*rc[0]+rc[1]*rc[1]); 
//      exit;
//    end;
//  end;
//  labda:= (rc[0]*(v1[0]-v2[0])+rc[1]*(v1[1]-v2[1])) div len; 
//  if labda<=0.0 then
//  begin 
//    pt[0]:= v2[0]; 
//    pt[1]:= v2[1]; 
//  end;
//  else
//  if labda>=1.0 then
//  begin 
//    pt[0]:= v3[0]; 
//    pt[1]:= v3[1]; 
//  end;
//  else
//  begin 
//    pt[0]:= labda*rc[0]+v2[0]; 
//    pt[1]:= labda*rc[1]+v2[1]; 
//  end;
//  rc[0]:= pt[0]-v1[0]; 
//  rc[1]:= pt[1]-v1[1]; 
//  begin
//    result:= fsqrt(rc[0]*rc[0]+rc[1]*rc[1]); 
//    exit;
//  end;
//end;

//function AreaF2Dfl(v1: psingle;  v2: psingle;  v3: psingle): single;
//begin
//  begin
//    result:= .5*fabs((v1[0]-v2[0])*(v2[1]-v3[1])+(v1[1]-v2[1])*(v3[0]-v2[0])); 
//    exit;
//  end;
//end;

(* only convex Quadrilaterals *)

//function AreaQ3Dfl(v1: psingle;  v2: psingle;  v3: psingle;  v4: psingle): single;
//var
//len: single; 
//vec1: array [0..2] of single;
//vec2: array [0..2] of single;
//n: array [0..2] of single;
//begin

//  VecSubf(vec1,v2,v1); 
//  VecSubf(vec2,v4,v1); 
//  Crossf(n,vec1,vec2); 
//  len:= Normalise(n); 
//  VecSubf(vec1,v4,v3); 
//  VecSubf(vec2,v2,v3); 
//  Crossf(n,vec1,vec2); 
//  len:= len + (Normalise(n)); 
//  begin
//    result:= (len div 2.0); 
//    exit;
//  end;
//end;

(* Triangles *)

//function AreaT3Dfl(v1: psingle;  v2: psingle;  v3: psingle): single;
//var
//len: single; 
//vec1: array [0..2] of single;
//vec2: array [0..2] of single;
//n: array [0..2] of single;
//begin

//  VecSubf(vec1,v3,v2); 
//  VecSubf(vec2,v1,v2); 
//  Crossf(n,vec1,vec2); 
//  len:= Normalise(n); 
//  begin
//    result:= (len div 2.0); 
//    exit;
//  end;
//end;

//function MAX2(x: integer; y: integer): integer; {<= !!!6 unknown macro}

//function MAX3(x: integer; y: integer; z: integer): integer; {<= !!!6 unknown macro}

//function AreaPoly3Dfl(nr: integer;  verts: psingle;  normal: psingle): single;
//var
//x: single; 
//y: single; 
//z: single; 
//area: single; 
//max: single; 
//cur: psingle;
//prev: psingle;
//a: integer; 
//px: integer;

//py: integer;
// (* first: find dominant axis: 0==X, 1==Y, 2==Z *)
//begin

//  px:=0; 
//  py:=1; (* first: find dominant axis: 0==X, 1==Y, 2==Z *)
//  x:= fabs(normal[0]); 
//  y:= fabs(normal[1]); 
//  z:= fabs(normal[2]); 
//  max:= MAX3(x,y,z); 
//  if max=y then
//  py:= 2; 
//  else
//  if max=x then
//  begin 
//    px:= 1; 
//    py:= 2; 
//  end;
//  prev:= verts+3*(nr-1); 
//  cur:= verts; 
//  area:= 0; (* The Trapezium Area Rule *)
//  for{while} a:=0 to Pred(nr) { a++}
//  do
//  begin 
//    area:= area + ((cur[px]-prev[px])*(cur[py]+prev[py])); 
//    prev:= cur; 
//    cur:= cur + (3); 
//  end;
//  begin
//    result:= fabs(0.5*area div max); 
//    exit;
//  end;
//end;
//void{!!!e unknown token}
//min: psingle;
//max: psingle;
//vec: psingle;
//begin 
//  if min[0]>vec[0] then
//  min[0]:= vec[0]; 
//  if min[1]>vec[1] then
//  min[1]:= vec[1]; 
//  if min[2]>vec[2] then
//  min[2]:= vec[2]; 
//  if max[0]<vec[0] then
//  max[0]:= vec[0]; 
//  if max[1]<vec[1] then
//  max[1]:= vec[1]; 
//  if max[2]<vec[2] then
//  max[2]:= vec[2]; 
//end;

(* ************ EULER *************** *)

//void{!!!e unknown token}
//eul: psingle;
//mat: array [0..,0..2] of single;
//begin 
//  ci: single; 
//  cj: single; 
//  ch: single; 
//  si: single; 
//  sj: single; 
//  sh: single; 
//  cc: single; 
//  cs: single; 
//  sc: single; 
//  ss: single; 
//  ci:= fcos(eul[0]); 
//  cj:= fcos(eul[1]); 
//  ch:= fcos(eul[2]); 
//  si:= fsin(eul[0]); 
//  sj:= fsin(eul[1]); 
//  sh:= fsin(eul[2]); 
//  cc:= ci*ch; 
//  cs:= ci*sh; 
//  sc:= si*ch; 
//  ss:= si*sh; 
//  mat[0][0]:= cj*ch; 
//  mat[1][0]:= sj*sc-cs; 
//  mat[2][0]:= sj*cc+ss; 
//  mat[0][1]:= cj*sh; 
//  mat[1][1]:= sj*ss+cc; 
//  mat[2][1]:= sj*cs-sc; 
//  mat[0][2]:= -sj; 
//  mat[1][2]:= cj*si; 
//  mat[2][2]:= cj*ci; 
//end;
//void{!!!e unknown token}
//eul: psingle;
//mat: array [0..,0..3] of single;
//begin 
//  ci: single; 
//  cj: single; 
//  ch: single; 
//  si: single; 
//  sj: single; 
//  sh: single; 
//  cc: single; 
//  cs: single; 
//  sc: single; 
//  ss: single; 
//  ci:= fcos(eul[0]); 
//  cj:= fcos(eul[1]); 
//  ch:= fcos(eul[2]); 
//  si:= fsin(eul[0]); 
//  sj:= fsin(eul[1]); 
//  sh:= fsin(eul[2]); 
//  cc:= ci*ch; 
//  cs:= ci*sh; 
//  sc:= si*ch; 
//  ss:= si*sh; 
//  mat[0][0]:= cj*ch; 
//  mat[1][0]:= sj*sc-cs; 
//  mat[2][0]:= sj*cc+ss; 
//  mat[0][1]:= cj*sh; 
//  mat[1][1]:= sj*ss+cc; 
//  mat[2][1]:= sj*cs-sc; 
//  mat[0][2]:= -sj; 
//  mat[1][2]:= cj*si; 
//  mat[2][2]:= cj*ci; 
//  mat[3][0]:= mat[3][1]:=mat[3][2]:=mat[0][3]:=mat[1][3]:=mat[2][3]:=0.0; 
//  mat[3][3]:= 1.0; 
//end;
//void{!!!e unknown token}
//tmat: array [0..,0..2] of single;
//eul: psingle;
//begin 
//  cy: single; 
//  quat: array [0..3] of single;
//  mat: array [0..2,0..2] of single;
//  Mat3ToQuat(tmat,quat); 
//  QuatToMat3(quat,mat); 
//  Mat3CpyMat3(mat,tmat); 
//  Mat3Ortho(mat); 
//  cy:= fsqrt(mat[0][0]*mat[0][0]+mat[0][1]*mat[0][1]); 
//  if cy>16.0*FLT_EPSILON then
//  begin 
//    eul[0]:= fatan2(mat[1][2],mat[2][2]); 
//    eul[1]:= fatan2(-mat[0][2],cy); 
//    eul[2]:= fatan2(mat[0][1],mat[0][0]); 
//  end;
//  else
//  begin 
//    eul[0]:= fatan2(-mat[2][1],mat[1][1]); 
//    eul[1]:= fatan2(-mat[0][2],cy); 
//    eul[2]:= 0.0; 
//  end;
//end;
//void{!!!e unknown token}
//tmat: array [0..,0..2] of single;
//eul: psingle;
//begin 
//  sin1: single; 
//  cos1: single; 
//  sin2: single; 
//  cos2: single; 
//  sin3: single; 
//  cos3: single; 
//  sin1:= -tmat[2][0]; 
//  cos1:= fsqrt(1-sin1*sin1); 
//  if fabs(cos1)>FLT_EPSILON then
//  begin 
//    sin2:= tmat[2][1] div cos1; 
//    cos2:= tmat[2][2] div cos1; 
//    sin3:= tmat[1][0] div cos1; 
//    cos3:= tmat[0][0] div cos1; 
//  end;
//  else
//  begin 
//    sin2:= -tmat[1][2]; 
//    cos2:= tmat[1][1]; 
//    sin3:= 0.0; 
//    cos3:= 1.0; 
//  end;
//  eul[0]:= fatan2(sin3,cos3); 
//  eul[1]:= fatan2(sin1,cos1); 
//  eul[2]:= fatan2(sin2,cos2); 
//  PRINT3(f,f,f,eul[0],eul[1],eul[2]); 
//end;

//procedure QuatToEul(quat: psingle;  eul: psingle);
//var
//mat: array [0..2,0..2] of single;
//begin

//  QuatToMat3(quat,mat); 
//  Mat3ToEul(mat,eul); 
//end;

//procedure EulToQuat(eul: psingle;  quat: psingle);
//var
//ti: single; 
//tj: single; 
//th: single; 
//ci: single; 
//cj: single; 
//ch: single; 
//si: single; 
//sj: single; 
//sh: single; 
//cc: single; 
//cs: single; 
//sc: single; 
//ss: single; 
//begin

//  ti:= eul[0]*0.5; 
//  tj:= eul[1]*0.5; 
//  th:= eul[2]*0.5; 
//  ci:= fcos(ti); 
//  cj:= fcos(tj); 
//  ch:= fcos(th); 
//  si:= fsin(ti); 
//  sj:= fsin(tj); 
//  sh:= fsin(th); 
//  cc:= ci*ch; 
//  cs:= ci*sh; 
//  sc:= si*ch; 
//  ss:= si*sh; 
//  quat[0]:= cj*cc+sj*ss; 
//  quat[1]:= cj*sc-sj*cs; 
//  quat[2]:= cj*ss+sj*cc; 
//  quat[3]:= cj*cs-sj*sc; 
//end;

//procedure VecRotToMat3(vec: psingle;  phi: single;  mat: array [0..,0..2] of single);
//var
//vx: single; 
//vx2: single; 
//vy: single; 
//vy2: single; 
//vz: single; 
//vz2: single; 
//co: single; 
//si: single; 
//begin(* rotatie van phi radialen rond vec *)

//  vx:= vec[0]; 
//  vy:= vec[1]; 
//  vz:= vec[2]; 
//  vx2:= vx*vx; 
//  vy2:= vy*vy; 
//  vz2:= vz*vz; 
//  co:= fcos(phi); 
//  si:= fsin(phi); 
//  mat[0][0]:= vx2+co*(1.0-vx2); 
//  mat[0][1]:= vx*vy*(1.0-co)+vz*si; 
//  mat[0][2]:= vz*vx*(1.0-co)-vy*si; 
//  mat[1][0]:= vx*vy*(1.0-co)-vz*si; 
//  mat[1][1]:= vy2+co*(1.0-vy2); 
//  mat[1][2]:= vy*vz*(1.0-co)+vx*si; 
//  mat[2][0]:= vz*vx*(1.0-co)+vy*si; 
//  mat[2][1]:= vy*vz*(1.0-co)-vx*si; 
//  mat[2][2]:= vz2+co*(1.0-vz2); 
//end;

//procedure euler_rot(beul: psingle;  ang: single;  axis: char);
//var
//eul: array [0..2] of single;
//mat1: array [0..2,0..2] of single;
//mat2: array [0..2,0..2] of single;
//totmat: array [0..2,0..2] of single;
//begin

//  eul[0]:= eul[1]:=eul[2]:=0.0; 
//  if axis='x' then
//  eul[0]:= ang; 
//  else
//  if axis='y' then
//  eul[1]:= ang; 
//  else
//  eul[2]:= ang; 
//  EulToMat3(eul,mat1); 
//  EulToMat3(beul,mat2); 
//  Mat3MulMat3(totmat,mat2,mat1); 
//  Mat3ToEul(totmat,beul); 
//end;
//void{!!!e unknown token}
//size: psingle;
//mat: array [0..,0..2] of single;
//begin 
//  mat[0][0]:= size[0]; 
//  mat[0][1]:= 0.0; 
//  mat[0][2]:= 0.0; 
//  mat[1][1]:= size[1]; 
//  mat[1][0]:= 0.0; 
//  mat[1][2]:= 0.0; 
//  mat[2][2]:= size[2]; 
//  mat[2][1]:= 0.0; 
//  mat[2][0]:= 0.0; 
//end;
//void{!!!e unknown token}
//mat: array [0..,0..2] of single;
//size: psingle;
//begin 
//  vec: array [0..2] of single;
//  VecCopyf(vec,mat[0]); 
//  size[0]:= Normalise(vec); 
//  VecCopyf(vec,mat[1]); 
//  size[1]:= Normalise(vec); 
//  VecCopyf(vec,mat[2]); 
//  size[2]:= Normalise(vec); 
//end;
//void{!!!e unknown token}
//mat: array [0..,0..3] of single;
//size: psingle;
//begin 
//  vec: array [0..2] of single;
//  VecCopyf(vec,mat[0]); 
//  size[0]:= Normalise(vec); 
//  VecCopyf(vec,mat[1]); 
//  size[1]:= Normalise(vec); 
//  VecCopyf(vec,mat[2]); 
//  size[2]:= Normalise(vec); 
//end;

(* ************* SPECIALS ******************* *)

//void{!!!e unknown token}
//v1: psingle;
//v2: psingle;
//v3: psingle;
//quat: psingle;
//begin 
//  (* denkbeeldige x-as, y-as driehoek wordt geroteerd *)
//  vec: array [0..2] of single;
//  q1: array [0..3] of single;
//  q2: array [0..3] of single;
//  n: array [0..2] of single;
//  si: single; 
//  co: single; 
//  hoek: single; 
//  mat: array [0..2,0..2] of single;
//  imat: array [0..2,0..2] of single; (* eerst z-as op vlaknormaal *)
//  CalcNormFloat(v1,v2,v3,vec); 
//  n[0]:= vec[1]; 
//  n[1]:= -vec[0]; 
//  n[2]:= 0.0; 
//  Normalise(n); 
//  if n[0]=0.0)and(n[1]=0.0 then
//  n[0]:= 1.0; 
//  hoek:= -0.5*safacos(vec[2]); 
//  co:= fcos(hoek); 
//  si:= fsin(hoek); 
//  q1[0]:= co; 
//  q1[1]:= n[0]*si; 
//  q1[2]:= n[1]*si; 
//  q1[3]:= 0.0; 
//  QuatToMat3(q1,mat); 
//  (* v1-v2 lijn terug roteren *)
//  Mat3Inv(imat,mat); 
//  VecSubf(vec,v2,v1); 
//  Mat3MulVecfl(imat,vec); 
//  vec[2]:= 0.0; 
//  Normalise(vec); 
//  (* welke hoek maakt deze lijn met x-as? *)
//  hoek:= 0.5*fatan2(vec[1],vec[0]); 
//  co:= fcos(hoek); 
//  si:= fsin(hoek); 
//  q2[0]:= co; 
//  q2[1]:= 0.0; 
//  q2[2]:= 0.0; 
//  q2[3]:= si; 
//  QuatMul(quat,q1,q2); 
//end;
//void{!!!e unknown token}
//c: array [0..] of smallint; 
//begin 
//  if c[0]>255 then
//  c[0]:= 255; 
//  else
//  if c[0]<0 then
//  c[0]:= 0; 
//  if c[1]>255 then
//  c[1]:= 255; 
//  else
//  if c[1]<0 then
//  c[1]:= 0; 
//  if c[2]>255 then
//  c[2]:= 255; 
//  else
//  if c[2]<0 then
//  c[2]:= 0; 
//end;

//procedure hsv_to_rgb(h: single;  s: single;  v: single;  r: psingle;  g: psingle;  b: psingle);
//var
//i: integer; 
//f: single; 
//p: single; 
//q: single; 
//t: single; 
//begin

//  h:= h * (360.0); 
//  if s=0)and(0 then
//  begin 
//    {*}r^:=v; 
//    {*}g^:=v; 
//    {*}b^:=v; 
//  end;
//  else
//  begin 
//    if h=360 then
//    h:= 0; 
//    h:= h div (60); 
//    i:= ffloor(h); 
//    f:= h-i; 
//    p:= v*(1.0-s); 
//    q:= v*(1.0-(s*f)); 
//    t:= v*(1.0-(s*(1.0-f))); 
//    case i of
//      0:
//      begin
//        {*}r^:=v; 
//        {*}g^:=t; 
//        {*}b^:=p; 
//      end;
//      1:
//      begin
//        {*}r^:=q; 
//        {*}g^:=v; 
//        {*}b^:=p; 
//      end;
//      2:
//      begin
//        {*}r^:=p; 
//        {*}g^:=v; 
//        {*}b^:=t; 
//      end;
//      3:
//      begin
//        {*}r^:=p; 
//        {*}g^:=q; 
//        {*}b^:=v; 
//      end;
//      4:
//      begin
//        {*}r^:=t; 
//        {*}g^:=p; 
//        {*}b^:=v; 
//      end;
//      5:
//      begin
//        {*}r^:=v; 
//        {*}g^:=p; 
//        {*}b^:=q; 
//      end;

//    end;{case?}
//  end;
//end;

//procedure rgb_to_hsv(r: single;  g: single;  b: single;  lh: psingle;  ls: psingle;  lv: psingle);
//var
//h: single; 
//s: single; 
//v: single; 
//cmax: single; 
//cmin: single; 
//cdelta: single; 
//rc: single; 
//gc: single; 
//bc: single; 
//begin

//  cmax:= r; 
//  cmin:= r; 
//  cmax:= (g>cmax {was ?}if then g {was :}else cmax);
//  cmin:= (g<cmin {was ?}if then g {was :}else cmin);
//  cmax:= (b>cmax {was ?}if then b {was :}else cmax);
//  cmin:= (b<cmin {was ?}if then b {was :}else cmin);
//  v:= cmax; (* value *)
//  if cmax<>0.0 then
//  s:= (cmax-cmin) div cmax; 
//  else
//  begin 
//    s:= 0.0; 
//    h:= 0.0; 
//  end;
//  if s=0.0 then
//  h:= -1.0; 
//  else
//  begin 
//    cdelta:= cmax-cmin; 
//    rc:= (cmax-r) div cdelta; 
//    gc:= (cmax-g) div cdelta; 
//    bc:= (cmax-b) div cdelta; 
//    if r=cmax then
//    h:= bc-gc; 
//    else
//    if g=cmax then
//    h:= 2.0+rc-bc; 
//    else
//    h:= 4.0+gc-rc; 
//    h:= h*60.0; 
//    if h<0.0 then
//    h:= h + (360.0); 
//  end;
//  {*}ls^:=s; 
//  {*}lh^:=h div 360.0; 
//  if {*}lh^<0.0 then
//  {*}lh^:=0.0; 
//  {*}lv^:=v; 
//end;

(* Bij afspraak is cpack een getal dat als 0xFFaa66 of zo kan worden uitgedrukt. Is dus gevoelig voor endian.
 * Met deze define wordt het altijd goed afgebeeld
 *)

//function hsv_to_cpack(h: single;  s: single;  v: single): uint; 
//var
//r: smallint; 
//g: smallint; 
//b: smallint; 
//rf: single; 
//gf: single; 
//bf: single; 
//col: uint; 
//begin

//  hsv_to_rgb(h,s,v,@rf,@gf, and bf); 
//  r:= rf*255.0; 
//  g:= gf*255.0; 
//  b:= bf*255.0; 
//  col:= (r+(g*256)+(b*256*256)); 
//  begin
//    result:= col; 
//    exit;
//  end;
//end;

//function rgb_to_cpack(r: single;  g: single;  b: single): uint; 
//var
//ir: integer; 
//ig: integer; 
//ib: integer; 
//begin

//  ir:= ffloor(255.0*r); 
//  if ir<0 then
//  ir:= 0; 
//  else
//  if ir>255 then
//  ir:= 255; 
//  ig:= ffloor(255.0*g); 
//  if ig<0 then
//  ig:= 0; 
//  else
//  if ig>255 then
//  ig:= 255; 
//  ib:= ffloor(255.0*b); 
//  if ib<0 then
//  ib:= 0; 
//  else
//  if ib>255 then
//  ib:= 255; 
//  begin
//    result:= (ir+(ig*256)+(ib*256*256)); 
//    exit;
//  end;
//end;

//procedure cpack_to_rgb(col: uint;  r: psingle;  g: psingle;  b: psingle);
//begin{*}r^:=((col) and $FF); 
//  {*}r^ div =255.0; 
//  {*}g^:=(((col) shr 8) and $FF); 
//  {*}g^ div =255.0; 
//  {*}b^:=(((col) shr 16) and $FF); 
//  {*}b^ div =255.0; 
//end;
//int({*}vergfunc^); 

(* ************ NIEUWE QSORT ********************** *)

//();
//var
//vergsize: integer; 
//temppivot: array [0..Pred(40)] of integer; int{!!!e unknown token}
//poin: pinteger; 
//n: integer; 
//pivot: pinteger; 
//begin 
//  i: integer = 0; 
//  j: integer = n-1; 
//  k: integer; 
//  t1: integer; 
//  next: pinteger; 
//  k:= 0; 
//  while k<vergsize
//  do
//  begin 
//    temppivot[k]:= pivot[k]; 
//    inc(k); 
//  end;
//  next:= poin+vergsize*(n-1); 
//  while i<=j
//  do
//  begin 
//    while vergfunc(poin,temppivot)=-1
//    do
//    begin 
//      inc(i); 
//      poin:= poin + (vergsize); 
//    end;
//    while vergfunc(next,temppivot)<>-1
//    do
//    begin 
//      dec(j); 
//      next:= next - (vergsize); 
//    end;
//    if i<j then
//    begin 
//      (* wissel *)
//      if vergsize=1 then
//      begin 
//        t1:= {*}poin^; {*}poin^:=*next; 
//        {*}next^:=t1; 
//        inc(poin); 
//        dec(next); 
//      end;
//      else
//      if vergsize=2 then
//      begin 
//        t1:= poin[0]; 
//        poin[0]:= next[0]; 
//        next[0]:= t1; 
//        t1:= poin[1]; 
//        poin[1]:= next[1]; 
//        next[1]:= t1; 
//        poin:= poin + (2); 
//        next:= next - (2); 
//      end;
//      else
//      begin 
//        k:= 0; 
//        while k<vergsize
//        do
//        begin 
//          t1:= poin[k]; 
//          poin[k]:= next[k]; 
//          next[k]:= t1; 
//          inc(k); 
//        end;
//        poin:= poin + (vergsize); 
//        next:= next - (vergsize); 
//      end;
//      inc(i); 
//      dec(j); 
//    end;
//  end;
//  begin
//    result:= i; 
//    exit;
//  end;
//end;
//void{!!!e unknown token}
//poin: pinteger; 
//n: integer; 
//begin 
//  i: integer; 
//  k: integer; 
//  ok: integer = 0; 
//  pivot: pinteger; 
//  next: pinteger; (* zoek pivot *)
//  if n<3 then
//  begin 
//    if n=2 then
//    begin 
//      next:= poin+vergsize; 
//      k:= vergfunc(poin,next); 
//      if k=1 then
//      begin 
//        k:= 0; 
//        while k<vergsize
//        do
//        begin 
//          i:= poin[k]; 
//          poin[k]:= next[k]; 
//          next[k]:= i; 
//          inc(k); 
//        end;
//      end;
//    end;
//    exit;
//  end;
//  else
//  begin 
//    next:= poin+vergsize; 
//    while{for} 
//    i:= 1; 
//    {to} i<n
//    {;}
//    inc(i); 
//    next:= next + (vergsize); 
//    do
//    begin 
//      k:= vergfunc(poin,next); 
//      if k=1 then
//      begin 
//        pivot:= poin; 
//        ok:= 1; 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      else
//      if k=-1 then
//      begin 
//        pivot:= next; 
//        ok:= 1; 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//    end;
//    if ok<>nil then
//    begin 
//      k:= partitie(poin,n,pivot); 
//      qsortNN(poin,k); 
//      qsortNN(poin+vergsize*k,n-k); 
//    end;
//  end;
//end;
//void{!!!e unknown token}
//poin: pinteger; 
//n: integer; 
//size: integer; 
//int({*}func^); 
//(); 
//begin 
//  vergfunc:= func; 
//  vergsize:= size div 4; 
//  if 4*vergsize<>size then
//  begin 
//    printf('wrong size in qsortN\n'); 
//    exit;
//  end;
//  qsortNN(poin,n); 
//end;

(* ********** PREFAB QSORT: qsortN2(int *data, int nr) , sorteert setjes van 2 ints (bv. edges) ********* *)

//function VERGLONG2(e1: integer; e2: integer): integer; {<= !!!6 unknown macro}
//int{!!!e unknown token}
//poin: pinteger; 
//n: integer; 
//pivot: pinteger; 
//begin 
//  i: integer = 0; 
//  j: integer = n-1; 
//  k: integer; 
//  t1: integer; 
//  next: pinteger; 
//  temppivot[0]:= pivot[0]; 
//  temppivot[1]:= pivot[1]; 
//  next:= poin+2*(n-1); 
//  while i<=j
//  do
//  begin 
//    while VERGLONG2(poin,temppivot)=-1
//    do
//    begin 
//      inc(i); 
//      poin:= poin + (2); 
//    end;
//    while VERGLONG2(next,temppivot)<>-1
//    do
//    begin 
//      dec(j); 
//      next:= next - (2); 
//    end;
//    if i<j then
//    begin 
//      t1:= poin[0]; 
//      poin[0]:= next[0]; 
//      next[0]:= t1; 
//      t1:= poin[1]; 
//      poin[1]:= next[1]; 
//      next[1]:= t1; 
//      poin:= poin + (2); 
//      next:= next - (2); 
//      inc(i); 
//      dec(j); 
//    end;
//  end;
//  begin
//    result:= i; 
//    exit;
//  end;
//end;
//void{!!!e unknown token}
//poin: pinteger; 
//n: integer; 
//begin 
//  i: integer; 
//  k: integer; 
//  ok: integer = 0; 
//  pivot: pinteger; 
//  next: pinteger; (* zoek pivot *)
//  if n<3 then
//  begin 
//    if n=2 then
//    begin 
//      next:= poin+2; 
//      k:= VERGLONG2(poin,next); 
//      if k=1 then
//      begin 
//        i:= poin[0]; 
//        poin[0]:= next[0]; 
//        next[0]:= i; 
//        i:= poin[1]; 
//        poin[1]:= next[1]; 
//        next[1]:= i; 
//      end;
//    end;
//  end;
//  else
//  begin 
//    next:= poin+2; 
//    while{for} 
//    i:= 1; 
//    {to} i<n
//    {;}
//    inc(i); 
//    next:= next + (2); 
//    do
//    begin 
//      k:= VERGLONG2(poin,next); 
//      if k=1 then
//      begin 
//        pivot:= poin; 
//        ok:= 1; 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      else
//      if k=-1 then
//      begin 
//        pivot:= next; 
//        ok:= 1; 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//    end;
//    if ok<>nil then
//    begin 
//      k:= partitie2(poin,n,pivot); 
//      qsortN2(poin,k); 
//      qsortN2(poin+2*k,n-k); 
//    end;
//  end;
//end;

//{MAX2(x,y)  ( (x)>(y) ? (x) : (y) )}
//function MAX2(x: integer; y: integer): integer; 
//begin
//  result:= ((x)>(y) {was ?}if then (x) {was :}else (y))
//end;

//{MAX3(x,y,z)  MAX2( MAX2((x),(y)) , (z) )}
//function MAX3(x: integer; y: integer; z: integer): integer; 
//begin
//  result:= MAX2(MAX2((x),(y)),(z))
//end;

//{VERGLONG2(e1, e2) ( (e1[0] > e2[0])? 1 : ( (e1[0] < e2[0]) ? -1 : ( (e1[1] > e2[1]) ? 1 : ( (e1[1] < e2[1]) ? -1 : 0 ) ) ) )}
//function VERGLONG2(e1: integer; e2: integer): integer; 
//begin
//  result:= ((e1[0]>e2[0]) {was ?}if then 1 {was :}else ((e1[0]<e2[0]) {was ?}if then -1 {was :}else ((e1[1]>e2[1]) {was ?}if then 1 {was :}else ((e1[1]<e2[1]) {was ?}if then -1 {was :}else 0))))
//end;

end.
