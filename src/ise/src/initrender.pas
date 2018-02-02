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


(*  initrender.c        RENDER
 *
 *  april 95
 *
 *)
unit initrender;

interface

//{$include "blender.h"}
//{$include "graphics.h"  * voor tekenen sterren * }
//{$include "edit.h"}
//{$include "effect.h"}
//{$include "render.h"}
//{$include "file.h"}
//{$include "ipo.h"}
//{$include "ika.h"}
//{$include "writeavi.h"}
//{$include <math.h>}
//{$include <stdio.h>}
//{$include <limits.h>}
//
//{$ifndef WINDOWS}
//{$include <sys/time.h>  * struct timeval * }
//{$endif}
//
//{$if !defined(__BeOS) && !defined(WINDOWS)}
//{$include <sys/resource.h>  * struct rusage * }
//{$endif}
//
//(* uit render.c *)
//{fmask: array [0..Pred(256)] of float; }{<= !!!5 external variable}
//{centLut: array [0..Pred(16)] of float; }{<= !!!5 external variable}
//{mask1: array [0..Pred(9)] of pushort; }{<= !!!5 external variable}
//{mask2: array [0..Pred(9)] of pushort; }{<= !!!5 external variable}
//{igamtab1: pushort; }{<= !!!5 external variable}
//{igamtab2: pushort; }{<= !!!5 external variable}
//{gamtab: pushort; }{<= !!!5 external variable}
//{cmask: array [0..Pred(256)] of char; }{<= !!!5 external variable}
//{centmask: pchar; }{<= !!!5 external variable}
//(* hieruit *)
//
//procedure setpanorot(part: integer); 
//
//var
//defmaterial: Material; 
//pa: smallint; (* pa is globaal part ivm print *)
//allparts: array [0..Pred(65),0..Pred(4)] of smallint; 
//jit: array [0..Pred(64),0..Pred(2)] of float; 
//panophi: float = 0.0; 
//qscount: integer; 
//(* ********************* *********************** *)
//

procedure screendump;

implementation

uses
  GL,
  blender, blendef, screen, util, cfuncs, blenglob, mywindow,
  toets, filesel;

//procedure init_def_material; 
//var
//ma: pMaterial; 
//begin
//  ma:=  and defmaterial; 
//  init_material( and defmaterial); 
//  init_render_material(ma); 
//  
//end;
//
//
//procedure init_render_data; 
//begin
//  bzero(@R,sizeof(Render)); 
//  bzero(@O,sizeof(Osa)); 
//  O.dxwin[0]:= 1.0; 
//  O.dywin[1]:= 1.0; 
//  R.displaymode:= R_DISPLAYWIN; 
//  R.blove:= (VertRen** )callocN(sizeof(void* )*(MAXVERT shr 8),'Blove'); 
//  R.blovl:= (VlakRen** )callocN(sizeof(void* )*(MAXVLAK shr 8),'Blovl'); 
//  R.bloha:= (HaloRen** )callocN(sizeof(void* )*(MAXVERT shr 8),'Bloha'); 
//  R.la:= (LampRen** )mallocN(MAXLAMP*sizeof(void* ),'renderlamparray'); 
//  init_def_material(); 
//  
//  
//end;
//
//
//procedure free_render_data; 
//begin
//  freeN(R.blove); 
//  R.blove:= 0; 
//  freeN(R.blovl); 
//  R.blovl:= 0; 
//  freeN(R.bloha); 
//  R.bloha:= 0; 
//  freeN(R.la); 
//  R.la:= 0; 
//  if R.rectot<>0{nil} {<= !!!9} 
//  then
//  freeN(R.rectot); 
//  if R.rectz<>0{nil} {<= !!!9} 
//  then
//  freeN(R.rectz); 
//  if R.rectspare<>0{nil} {<= !!!9} 
//  then
//  freeN(R.rectspare); 
//  R.rectot:= 0; 
//  R.rectz:= 0; 
//  R.rectspare:= 0; 
//end_render_material( and defmaterial); 
//
//end;
//
//
//(* ****************** JITTER *********************** *)
//
//
//procedure jitterate1(jit1: pfloat;  jit2: pfloat;  num: integer;  rad1: float); 
//var
//i: integer; 
//j: integer; 
//k: integer; 
//vecx: float; 
//vecy: float; 
//dvecx: float; 
//dvecy: float; 
//x: float; 
//y: float; 
//len: float; 
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
//  while{for} 
//  i:= 2*num-2; 
//  {to} i>=0
//  {;}
//  i:= i - (2); 
//  do
//  begin 
//    dvecx:= dvecy:=0.0; 
//    x:= jit1[i]; 
//    y:= jit1[i+1]; 
//    while{for} 
//    j:= 2*num-2; 
//    {to} j>=0
//    {;}
//    j:= j - (2); 
//    do
//    begin 
//      if i<>j
//      then
//      begin 
//        vecx:= jit1[j]-x-1.0; 
//        vecy:= jit1[j+1]-y-1.0; 
//        for{while} k:=3 downto Succ(0) { k--}
//        do
//        begin 
//          if fabs(vecx)<rad1)and(fabs(vecy)<rad1
//          then
//          begin 
//            len:= fsqrt(vecx*vecx+vecy*vecy); 
//            if len>0)and(len<rad1
//            then
//            begin 
//              len:= len div rad1; 
//              dvecx:= dvecx + (vecx div len); 
//              dvecy:= dvecy + (vecy div len); 
//            end;
//          end;
//          vecx:= vecx + (1.0); 
//          if fabs(vecx)<rad1)and(fabs(vecy)<rad1
//          then
//          begin 
//            len:= fsqrt(vecx*vecx+vecy*vecy); 
//            if len>0)and(len<rad1
//            then
//            begin 
//              len:= len div rad1; 
//              dvecx:= dvecx + (vecx div len); 
//              dvecy:= dvecy + (vecy div len); 
//            end;
//          end;
//          vecx:= vecx + (1.0); 
//          if fabs(vecx)<rad1)and(fabs(vecy)<rad1
//          then
//          begin 
//            len:= fsqrt(vecx*vecx+vecy*vecy); 
//            if len>0)and(len<rad1
//            then
//            begin 
//              len:= len div rad1; 
//              dvecx:= dvecx + (vecx div len); 
//              dvecy:= dvecy + (vecy div len); 
//            end;
//          end;
//          vecx:= vecx - (2.0); 
//          vecy:= vecy + (1.0); 
//        end;
//      end;
//    end;
//    x:= x - (dvecx div 18.0); 
//    y:= y - (dvecy div 18.0); 
//    x:= x - (floor(x)); 
//    y:= y - (floor(y)); 
//    jit2[i]:= x; 
//    jit2[i+1]:= y; 
//    
//  end;
//  memcpy(jit1,jit2,2*num*sizeof(float)); 
//end;
//
//
//procedure jitterate2(jit1: pfloat;  jit2: pfloat;  num: integer;  rad2: float); 
//var
//i: integer; 
//j: integer; 
//vecx: float; 
//vecy: float; 
//dvecx: float; 
//dvecy: float; 
//x: float; 
//y: float; 
//begin
//  
//  
//  
//  
//  
//  
//  
//  while{for} 
//  i:= 2*num-2; 
//  {to} i>=0
//  {;}
//  i:= i - (2); 
//  do
//  begin 
//    dvecx:= dvecy:=0.0; 
//    x:= jit1[i]; 
//    y:= jit1[i+1]; 
//    while{for} 
//    j:= 2*num-2; 
//    {to} j>=0
//    {;}
//    j:= j - (2); 
//    do
//    begin 
//      if i<>j
//      then
//      begin 
//        vecx:= jit1[j]-x-1.0; 
//        vecy:= jit1[j+1]-y-1.0; 
//        if fabs(vecx)<rad2
//        then
//        dvecx:= dvecx + (vecx*rad2); 
//        vecx:= vecx + (1.0); 
//        if fabs(vecx)<rad2
//        then
//        dvecx:= dvecx + (vecx*rad2); 
//        vecx:= vecx + (1.0); 
//        if fabs(vecx)<rad2
//        then
//        dvecx:= dvecx + (vecx*rad2); 
//        if fabs(vecy)<rad2
//        then
//        dvecy:= dvecy + (vecy*rad2); 
//        vecy:= vecy + (1.0); 
//        if fabs(vecy)<rad2
//        then
//        dvecy:= dvecy + (vecy*rad2); 
//        vecy:= vecy + (1.0); 
//        if fabs(vecy)<rad2
//        then
//        dvecy:= dvecy + (vecy*rad2); 
//        
//      end;
//    end;
//    x:= x - (dvecx div 2); 
//    y:= y - (dvecy div 2); 
//    x:= x - (floor(x)); 
//    y:= y - (floor(y)); 
//    jit2[i]:= x; 
//    jit2[i+1]:= y; 
//    
//  end;
//  memcpy(jit1,jit2,2*num*sizeof(float)); 
//end;
//
//
//procedure initjit(jit: pfloat;  num: integer); 
//var
//jit2: pfloat; 
//x: float; 
//rad1: float; 
//rad2: float; 
//rad3: float; 
//i: integer; 
//begin
//  
//  
//  
//  
//  
//  if num=0
//  then
//  exit;
//  
//  jit2:= mallocN(12+2*sizeof(float)*num,'initjit'); 
//  rad1:= 1.0 div fsqrt({!!!a type cast? =>} {float(}num); 
//  rad2:= 1.0 div ({!!!a type cast? =>} {float(}num); 
//  rad3:= fsqrt({!!!a type cast? =>} {float(}num) div ({!!!a type cast? =>} {float(}num); 
//  srand48(31415926+num); 
//  
//  x:= 0; 
//  while{for} 
//  i:= 0; 
//  {to} i<2*num
//  {;}
//  i:= i + (2); 
//  do
//  begin 
//    jit[i]:= x+rad1*(0.5-drand48()); 
//    jit[i+1]:= ({!!!a type cast? =>} {float(}i div 2) div num+rad1*(0.5-drand48()); 
//    x:= x + (rad3); 
//    x:= x - (floor(x)); 
//  end;
//  
//  for{while} i:=0 to Pred(24) { i++}
//  do
//  begin 
//    jitterate1(jit,jit2,num,rad1); 
//    jitterate1(jit,jit2,num,rad1); 
//    jitterate2(jit,jit2,num,rad2); 
//  end;
//  freeN(jit2); 
//  
//end;
//
//var {was static}
//lastjit: integer;
// 
//
//procedure init_render_jit(nr: integer); 
//begin
//  lastjit:=0; 
//  if lastjit=nr
//  then
//  exit;
//  
//  bzero(jit,64*2*4); 
//  initjit(jit[0],nr); 
//  lastjit:= nr; 
//  
//  
//end;
//(* ****************** GAMMA, MASKERS en LUTS **************** *)
//
//
//function calc_weight(weight: pfloat;  i: integer;  j: integer): float; 
//var
//x: float; 
//y: float; 
//dist: float; 
//totw: float;
// 
//a: integer; 
//begin
//  
//  
//  
//  totw:=0.0; 
//  for{while} a:=0 to Pred(R.osa) { a++}
//  do
//  begin 
//    x:= jit[a][0]-0.5+i; 
//    y:= jit[a][1]-0.5+j; 
//    dist:= fsqrt(x*x+y*y); 
//    weight[a]:= 0.0; 
//    
//    if R.r.mode and R_GAUSS
//    then
//    begin 
//      if dist<1.5
//      then
//      begin 
//        x:= dist*1.5; 
//        weight[a]:= (1.0 div exp(x*x)-1.0 div exp(1.5*1.5*1.5*1.5)); 
//      end;
//    end;
//    else
//    begin 
//      if i=0)and(j=0
//      then
//      weight[a]:= 1.0; 
//    end;
//    totw:= totw + (weight[a]); 
//    
//    
//  end;
//  begin
//    result:= totw; 
//    exit;
//  end;
//end;
//
//var {was static}
//firsttime: integer;
// 
//lastgamma: float;
// 
//
//procedure init_filt_mask; 
//var
//gamma: float; 
//igamma: float; 
//weight: array [0..Pred(32)] of float; 
//totw: float; 
//val: float; 
//fpx1: pfloat; 
//fpx2: pfloat; 
//fpy1: pfloat; 
//fpy2: pfloat; 
//i: integer; 
//j: integer; 
//a: integer; 
//m1: pushort; 
//m2: pushort; 
//shweight: array [0..Pred(32)] of ushort; 
//begin
//  firsttime:=1; 
//  lastgamma:=0.0; 
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
//  if firsttime<>0{nil} {<= !!!9} 
//  then
//  begin 
//    for{while} a:=0 to Pred(9) {a++}
//    do
//    begin 
//      mask1[a]:= mallocN(256*sizeof(short),'initfilt'); 
//      mask2[a]:= mallocN(256*sizeof(short),'initfilt'); 
//    end;
//    for{while} a:=0 to Pred(256) { a++}
//    do
//    begin 
//      cmask[a]:= 0; 
//      if a and 1
//      then
//      inc(cmask[a]); 
//      if a and 2
//      then
//      inc(cmask[a]); 
//      if a and 4
//      then
//      inc(cmask[a]); 
//      if a and 8
//      then
//      inc(cmask[a]); 
//      if a and 16
//      then
//      inc(cmask[a]); 
//      if a and 32
//      then
//      inc(cmask[a]); 
//      if a and 64
//      then
//      inc(cmask[a]); 
//      if a and 128
//      then
//      inc(cmask[a]); 
//    end;
//    centmask:= mallocN(65536,'Initfilt3'); 
//    for{while} a:=0 to Pred(16) { a++}
//    do
//    begin 
//      centLut[a]:= -0.45+({!!!a type cast? =>} {float(}a) div 16.0; 
//    end;
//    gamtab:= mallocN(65536*sizeof(short),'initGaus2'); 
//    igamtab1:= mallocN(256*sizeof(short),'initGaus2'); 
//    igamtab2:= mallocN(65536*sizeof(short),'initGaus2'); 
//    
//    
//  end;
//  
//  if R.r.alphamode=R_ALPHAKEY
//  then
//  gamma:= 1.0; (* ??? *)
//  
//  if R.r.mode and R_GAMMA
//  then
//  gamma:= 2.0; 
//  else
//  gamma:= 1.0; 
//  igamma:= 1.0 div gamma; 
//  if gamma<>lastgamma
//  then
//  begin 
//    lastgamma:= gamma; 
//    (* gamtab: in short, uit short *)
//    for{while} a:=0 to Pred(65536) { a++}
//    do
//    begin 
//      val:= a; 
//      val:= val div (65535.0); 
//      if gamma=2.0
//      then
//      val:= fsqrt(val); 
//      else
//      if gamma<>1.0
//      then
//      val:= powf(val,igamma); 
//      gamtab[a]:= (65535.99*val); 
//      
//    end;
//    (* inv gamtab1 : in byte, uit short *)
//    for{while} a:=1 to 256 { a++}
//    do
//    begin 
//      if gamma=2.0
//      then
//      igamtab1[a-1]:= a*a-1; 
//      else
//      if gamma=1.0
//      then
//      igamtab1[a-1]:= 256*a-1; 
//      else
//      begin 
//        val:= a div 256.0; 
//        igamtab1[a-1]:= (65535.0*powf(val,gamma))-1; 
//      end;
//    end;
//    
//    (* inv gamtab2 : in short, uit short *)
//    for{while} a:=0 to Pred(65536) { a++}
//    do
//    begin 
//      val:= a; 
//      val:= val div (65535.0); 
//      if gamma=2.0
//      then
//      val:= val*val; 
//      else
//      val:= powf(val,gamma); 
//      igamtab2[a]:= 65535.0*val; 
//      
//    end;
//  end;
//  
//  if firsttime<>0{nil} {<= !!!9} 
//  then
//  begin 
//    firsttime:= 0; 
//    exit;
//  end;
//  val:= 1.0 div ({!!!a type cast? =>} {float(}R.osa); 
//  for{while} a:=0 to Pred(256) { a++}
//  do
//  begin 
//    fmask[a]:= ({!!!a type cast? =>} {float(}cmask[a])*val; 
//  end;
//  
//  for{while} a:=0 to Pred(9) {a++}
//  do
//  begin 
//    bzero(mask1[a],256*2); 
//    bzero(mask2[a],256*2); 
//  end;
//  totw:= 0.0; 
//  (* bereken totw *)
//  for{while} j:=-1 to Pred(2) { j++}
//  do
//  begin 
//    for{while} i:=-1 to Pred(2) { i++}
//    do
//    begin 
//      totw:= totw + (calc_weight(weight,i,j)); 
//    end;
//  end;
//  
//  for{while} j:=-1 to Pred(2) { j++}
//  do
//  begin 
//    for{while} i:=-1 to Pred(2) { i++}
//    do
//    begin 
//      (* bereken ahv jit met ofset de gewichten *)
//      
//      bzero(weight,32*2); 
//      calc_weight(weight,i,j); 
//      
//      for{while} a:=0 to Pred(16) { a++}
//      do
//      shweight[a]:= weight[a]*(65535.0 div totw); 
//      m1:= mask1[3*(j+1)+i+1]; 
//      m2:= mask2[3*(j+1)+i+1]; 
//      
//      for{while} a:=0 to Pred(256) { a++}
//      do
//      begin 
//        if a and 1
//        then
//        begin 
//          m1[a]:= m1[a] + (shweight[0]); 
//          m2[a]:= m2[a] + (shweight[8]); 
//        end;
//        if a and 2
//        then
//        begin 
//          m1[a]:= m1[a] + (shweight[1]); 
//          m2[a]:= m2[a] + (shweight[9]); 
//        end;
//        if a and 4
//        then
//        begin 
//          m1[a]:= m1[a] + (shweight[2]); 
//          m2[a]:= m2[a] + (shweight[10]); 
//        end;
//        if a and 8
//        then
//        begin 
//          m1[a]:= m1[a] + (shweight[3]); 
//          m2[a]:= m2[a] + (shweight[11]); 
//        end;
//        if a and 16
//        then
//        begin 
//          m1[a]:= m1[a] + (shweight[4]); 
//          m2[a]:= m2[a] + (shweight[12]); 
//        end;
//        if a and 32
//        then
//        begin 
//          m1[a]:= m1[a] + (shweight[5]); 
//          m2[a]:= m2[a] + (shweight[13]); 
//        end;
//        if a and 64
//        then
//        begin 
//          m1[a]:= m1[a] + (shweight[6]); 
//          m2[a]:= m2[a] + (shweight[14]); 
//        end;
//        if a and 128
//        then
//        begin 
//          m1[a]:= m1[a] + (shweight[7]); 
//          m2[a]:= m2[a] + (shweight[15]); 
//        end;
//      end;
//    end;
//  end;
//  fpx1:= mallocN(256*sizeof(float),'initgauss4'); 
//  fpx2:= mallocN(256*sizeof(float),'initgauss4'); 
//  fpy1:= mallocN(256*sizeof(float),'initgauss4'); 
//  fpy2:= mallocN(256*sizeof(float),'initgauss4'); 
//  (* centmask: de juiste subpixel ofset per masker *)
//  
//  for{while} a:=0 to Pred(256) { a++}
//  do
//  begin 
//    fpx1[a]:= fpx2[a]:=0.0; 
//    fpy1[a]:= fpy2[a]:=0.0; 
//    if a and 1
//    then
//    begin 
//      fpx1[a]:= fpx1[a] + (jit[0][0]); 
//      fpy1[a]:= fpy1[a] + (jit[0][1]); 
//      fpx2[a]:= fpx2[a] + (jit[8][0]); 
//      fpy2[a]:= fpy2[a] + (jit[8][1]); 
//    end;
//    if a and 2
//    then
//    begin 
//      fpx1[a]:= fpx1[a] + (jit[1][0]); 
//      fpy1[a]:= fpy1[a] + (jit[1][1]); 
//      fpx2[a]:= fpx2[a] + (jit[9][0]); 
//      fpy2[a]:= fpy2[a] + (jit[9][1]); 
//    end;
//    if a and 4
//    then
//    begin 
//      fpx1[a]:= fpx1[a] + (jit[2][0]); 
//      fpy1[a]:= fpy1[a] + (jit[2][1]); 
//      fpx2[a]:= fpx2[a] + (jit[10][0]); 
//      fpy2[a]:= fpy2[a] + (jit[10][1]); 
//    end;
//    if a and 8
//    then
//    begin 
//      fpx1[a]:= fpx1[a] + (jit[3][0]); 
//      fpy1[a]:= fpy1[a] + (jit[3][1]); 
//      fpx2[a]:= fpx2[a] + (jit[11][0]); 
//      fpy2[a]:= fpy2[a] + (jit[11][1]); 
//    end;
//    if a and 16
//    then
//    begin 
//      fpx1[a]:= fpx1[a] + (jit[4][0]); 
//      fpy1[a]:= fpy1[a] + (jit[4][1]); 
//      fpx2[a]:= fpx2[a] + (jit[12][0]); 
//      fpy2[a]:= fpy2[a] + (jit[12][1]); 
//    end;
//    if a and 32
//    then
//    begin 
//      fpx1[a]:= fpx1[a] + (jit[5][0]); 
//      fpy1[a]:= fpy1[a] + (jit[5][1]); 
//      fpx2[a]:= fpx2[a] + (jit[13][0]); 
//      fpy2[a]:= fpy2[a] + (jit[13][1]); 
//    end;
//    if a and 64
//    then
//    begin 
//      fpx1[a]:= fpx1[a] + (jit[6][0]); 
//      fpy1[a]:= fpy1[a] + (jit[6][1]); 
//      fpx2[a]:= fpx2[a] + (jit[14][0]); 
//      fpy2[a]:= fpy2[a] + (jit[14][1]); 
//    end;
//    if a and 128
//    then
//    begin 
//      fpx1[a]:= fpx1[a] + (jit[7][0]); 
//      fpy1[a]:= fpy1[a] + (jit[7][1]); 
//      fpx2[a]:= fpx2[a] + (jit[15][0]); 
//      fpy2[a]:= fpy2[a] + (jit[15][1]); 
//    end;
//  end;
//  
//  for{while} a:=(1 shl R.osa)-1 downto Succ(0) { a--}
//  do
//  begin 
//    val:= count_mask(a); 
//    i:= (15.9*(fpy1[a and 255]+fpy2[a shr 8]) div val); 
//    i:= i shl (4); 
//    i:= i + ((15.9*(fpx1[a and 255]+fpx2[a shr 8]) div val)); 
//    centmask[a]:= i; 
//  end;
//  freeN(fpx1); 
//  
//  freeN(fpx2); 
//  freeN(fpy1); 
//  freeN(fpy2); 
//end;
//
//
//
//procedure free_filt_mask; 
//var
//a: integer; 
//begin
//  for{while} a:=0 to Pred(9) { a++}
//  do
//  begin 
//    freeN(mask1[a]); 
//    freeN(mask2[a]); 
//  end;
//  freeN(gamtab); 
//  freeN(igamtab1); 
//  freeN(igamtab2); 
//  freeN(centmask); 
//  
//end;
//
//(* ************** INFO FILES ******************* *)
//
//
//
//procedure make_info_file_string(string: pchar;  frame: integer); 
//var
//len: integer; 
//i: integer; 
//num: array [0..Pred(8)] of char; 
//begin
//  
//  
//  if string=0
//  then
//  exit;
//  
//  strcpy(string,G.scene.r.pic); 
//  convertstringcode(string); 
//  strcat(string,'info/'); 
//  
//  
//  (* dir testen, desnoods maken *)
//  if exist(string)=0
//  then
//  begin 
//    recurdir(string); 
//  end;
//  len:= lstrlen(string); 
//  i:= 4-sprintf(num,'%d',frame); 
//  
//  for{while}  downto Succ(0) { i--}
//  do
//  begin 
//    string[len]:= '0'; 
//    inc(len); 
//  end;
//  string[len]:= 0; 
//  strcat(string,num); 
//end;
//
//
//function panotestclip(v: pfloat): integer; 
//var
//abs4: float; 
//c: smallint;
// 
//begin(* gebruiken voor halo's en info's *)
//  
//  c:=0; 
//  if (R.r.mode and R_PANORAMA)=0
//  then
//  begin
//    result:= testclip(v); 
//    exit;
//    
//  end;
//  abs4:= fabs(v[3]); 
//  if v[2]<-abs4
//  then
//  c:= 16; 
//  else
//  (* hier stond vroeger " if(v[2]<0) ", zie clippz() *)
//  if v[2]>abs4
//  then
//  c:= c + (32); 
//  if v[1]>abs4
//  then
//  c:= c + (4); 
//  else
//  if v[1]<-abs4
//  then
//  c:= c + (8); 
//  abs4:= abs4 * (R.r.xparts); 
//  if v[0]>abs4
//  then
//  c:= c + (2); 
//  else
//  if v[0]<-abs4
//  then
//  c:= c + (1); 
//  begin
//    result:= c; 
//    exit;
//  end;
//end;
//
//
//
//function info_calc_schermco(vec: pfloat;  sco: pfloat): integer; 
//var
//si: float; 
//co: float; 
//v1: array [0..Pred(3)] of float; 
//hoco: array [0..Pred(4)] of float; 
//xpartsfac: float; 
//pa: integer;
// 
//begin
//  
//  
//  
//  
//  
//  pa:=0; 
//  xpartsfac:= 0.5 div {!!!a type cast? =>} {float(}R.r.xparts; 
//  sco[0]:= sco[1]:=sco[2]:=0.0; 
//  projectverto(vec,hoco); 
//  
//  (* eerst de normale versie *)
//  hoco[3]:= hoco[3] * (2.0); 
//  (* clipgrens iets ruimer *)
//  
//  if testclip(hoco)=0
//  then
//  begin 
//    hoco[3]:= hoco[3] div (2.0); 
//    sco[0]:= 0.5+xpartsfac*(hoco[0] div hoco[3]); 
//    sco[1]:= 0.5-0.5*(hoco[1] div hoco[3]); 
//    sco[2]:= (1.0+hoco[2] div hoco[3]); 
//    (* antieke schermco's *)
//  end;
//  
//  (* elk part testen *)
//  if R.r.mode and R_PANORAMA
//  then
//  begin 
//    
//    for{while} pa:=0 to Pred(R.r.xparts) { pa++}
//    do
//    begin 
//      setpanorot(pa); 
//      si:= fsin(panophi); 
//      co:= fcos(panophi); 
//      v1[0]:= co*vec[0]+si*vec[2]; 
//      v1[1]:= vec[1]; 
//      v1[2]:= -si*vec[0]+co*vec[2]; 
//      projectverto(v1,hoco); 
//      
//      
//      
//      hoco[3]:= hoco[3] * (2.0); (* clipgrens iets ruimer *)
//      
//      if testclip(hoco)=0
//      then
//      begin 
//        
//        hoco[3]:= hoco[3] div (2.0); 
//        sco[0]:= 0.5+xpartsfac*(hoco[0] div hoco[3])+pa*2.0*xpartsfac-(R.r.xparts-1)*xpartsfac; 
//        sco[1]:= 0.5-0.5*(hoco[1] div hoco[3]); 
//        sco[2]:= (1.0+hoco[2] div hoco[3]); 
//        (* clipgrens iets ruimer *)
//        
//        (* antieke schermco's *)
//      end;
//    end;
//  end;
//  
//  begin
//    result:= 1; 
//    exit;
//  end;
//end;
//
//
//procedure info_calc_drot(ob: pObject;  co: pfloat); 
//var
//vec1: array [0..Pred(3)] of float; 
//vec2: array [0..Pred(3)] of float; 
//len: float; 
//co1: float; 
//co2: float; 
//begin
//  
//  
//  
//  
//  {*}co^:=0.0; 
//  VECCOPY(vec1,ob.obmat[2]); 
//  
//  pushdata(ob,sizeof(Object)); 
//  
//  dec(CFRA); 
//  do_ob_ipo(ob); 
//  where_is_object(ob); 
//  VECCOPY(vec2,ob.obmat[2]); 
//  poplast(ob); 
//  inc(CFRA); 
//  len:= Normalise(vec1); 
//  
//  if len<0.$1
//  then
//  exit;
//  len:= Normalise(vec2); 
//  if len<0.$1
//  then
//  exit;
//  
//  (* co= INPR(vec1, vec2); *)
//  
//  co1:= fatan2(vec1[1],vec1[0]); 
//  co2:= fatan2(vec2[1],vec2[0]); {*}co^:=-co1+co2; 
//  
//  if {*}co^>M_PI
//  then
//  {*}co^-=2.0*M_PI; 
//  else
//  if {*}co^<-M_PI
//  then
//  {*}co^+=2.0*M_PI; 
//end;
//
//
//var {was static}
//fp: pFILE;
// 
//
//procedure info_file(ob: pObject); 
//var
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//vec1: array [0..Pred(3)] of float; 
//co: float; 
//name: array [0..Pred(FILE_MAXDIR+FILE_MAXFILE)] of char; 
//begin(* als ob==0: file sluiten
//    * als file==0: dir testen en file openen
//    * van deze functie staat kopie in writefile.c (Mdec versie)
//    *)
//  fp:=0; 
//  
//  
//  
//  if LongBool(1))or(ob=0
//  then
//  begin 
//    if fp<>0{nil} {<= !!!9} 
//    then
//    fclose(fp); 
//    fp:= 0; 
//    exit;
//  end;
//  
//  if fp=0
//  then
//  begin 
//    make_info_file_string(name,CFRA); 
//    fp:= fopen(name,'w'); 
//    if fp<>0{nil} {<= !!!9} 
//    then
//    begin 
//      fprintf(fp,'INFO\n'); 
//    end;
//  end;
//  if fp=0
//  then
//  exit;
//  
//  if ob.infostart>CFRA
//  then
//  exit;
//  if ob.infoend<>0)and(ob.infoend<CFRA
//  then
//  exit;
//  
//  fprintf(fp,'NAME\n%s\n',ob.id.name+2); 
//  
//  if ob.infoflag and OB_INFO_LOC
//  then
//  fprintf(fp,'LOC\n%f %f %f\n',ob.obmat[3][0],ob.obmat[3][1],ob.obmat[3][2]); 
//  
//  if ob.infoflag and OB_INFO_SCO
//  then
//  begin 
//    
//    Mat4MulMat4(mat,ob.obmat,R.viewmat); 
//    info_calc_schermco(mat[3],vec1); 
//    fprintf(fp,'SCO\n%f %f %f\n',vec1[0],vec1[1],vec1[2]); 
//  end;
//  
//  if ob.infoflag and OB_INFO_DROT
//  then
//  begin 
//    
//    info_calc_drot(ob, and co); 
//    fprintf(fp,'DROT\n%f\n',co); 
//    
//  end;
//end;
//
//
//
//(* ************** END INFO FILES ******************* *)
//
//
//(* ~~~~~~~~~~~~~~~~~ADD~~~~~~~~~~~~~~~~~~~~~ *)
//
//
//function addvert(nr: integer): pVertRen; 
//var
//v: pVertRen; 
//a: integer; 
//begin
//  
//  if nr<0)or(nr>MAXVERT
//  then
//  begin 
//    printf('error in addvert: %d\n',nr); 
//    begin
//      result:= R.blove[0]; 
//      exit;
//    end;
//  end;
//  a:= nr shr 8; 
//  v:= R.blove[a]; 
//  if v=0
//  then
//  begin 
//    v:= {!!!a type cast? =>} {pVertRen(}callocN(256*sizeof(VertRen),'addvert'); 
//    R.blove[a]:= v; 
//  end;
//  v:= v + ((nr and 255)); 
//  begin
//    result:= v; 
//    exit;
//  end;
//end;
//
//
//function addhalo(nr: integer): pHaloRen; 
//var
//h: pHaloRen; 
//a: integer; 
//begin
//  
//  if nr<0)or(nr>MAXVERT
//  then
//  begin 
//    printf('error in addhalo: %d\n',nr); 
//    begin
//      result:= R.bloha[0]; 
//      exit;
//    end;
//  end;
//  a:= nr shr 8; 
//  h:= R.bloha[a]; 
//  if h=0
//  then
//  begin 
//    h:= {!!!a type cast? =>} {pHaloRen(}callocN(256*sizeof(HaloRen),'addhalo'); 
//    R.bloha[a]:= h; 
//  end;
//  h:= h + ((nr and 255)); 
//  begin
//    result:= h; 
//    exit;
//  end;
//end;
//
//
//function addvlak(nr: integer): pVlakRen; 
//var
//v: pVlakRen; 
//a: integer; 
//begin
//  
//  if nr<0)or(nr>MAXVLAK
//  then
//  begin 
//    printf('error in addvlak: %d\n',nr); 
//    begin
//      result:= R.blovl[0]; 
//      exit;
//    end;
//  end;
//  a:= nr shr 8; 
//  v:= R.blovl[a]; 
//  if v=0
//  then
//  begin 
//    v:= {!!!a type cast? =>} {pVlakRen(}callocN(256*sizeof(VlakRen),'addvlak'); 
//    R.blovl[a]:= v; 
//  end;
//  v:= v + ((nr and 255)); 
//  begin
//    result:= v; 
//    exit;
//  end;
//end;
//
//
//function inithalo(ma: pMaterial;  vec: pfloat;  vec1: pfloat;  orco: pfloat;  hasize: float;  vectsize: float): pHaloRen; 
//var
//har: pHaloRen; 
//mtex: pMTex; 
//xn: float; 
//yn: float; 
//zn: float; 
//sco1: array [0..Pred(3)] of float; 
//sco2: array [0..Pred(3)] of float; 
//texvec: array [0..Pred(3)] of float; 
//hoco: array [0..Pred(4)] of float; 
//hoco1: array [0..Pred(4)] of float; 
//begin
//  {Tin: float; }{<= !!!5 external variable}
//  {Tr: float; }{<= !!!5 external variable}
//  {Tg: float; }{<= !!!5 external variable}
//  {Tb: float; }{<= !!!5 external variable}
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  if hasize=0
//  then
//  begin
//    result:= 0; 
//    exit;
//    
//  end;
//  projectverto(vec,hoco); 
//  if hoco[3]=0.0
//  then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  if vec1<>0{nil} {<= !!!9} 
//  then
//  begin 
//    projectverto(vec1,hoco1); 
//    if hoco1[3]=0.0
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//  end;
//  har:= addhalo(R.tothalo{++} inc(R.tothalo); ); 
//  VECCOPY(har.co,vec); 
//  
//  har.hasize:= hasize; 
//  (* projectvert wordt in zbufvlaggen gedaan ivm parts/border/pano *)
//  
//  (* halovect *)
//  if vec1<>0{nil} {<= !!!9} 
//  then
//  begin 
//    
//    har.type:= har.type or (HA_VECT); 
//    zn:= hoco[3]; 
//    har.xs:= 0.5*R.rectx*(hoco[0] div zn); 
//    har.ys:= 0.5*R.recty*(hoco[1] div zn); 
//    har.zs:= $7FFFFF*(1.0+hoco[2] div zn); 
//    xn:= har.xs-0.5*R.rectx*(hoco1[0] div hoco1[3]); 
//    yn:= har.ys-0.5*R.recty*(hoco1[1] div hoco1[3]); 
//    
//    if xn=0.0)or((xn=0.0)and(yn=0.0)
//    then
//    zn:= 0.0; 
//    else
//    zn:= atan2f(yn,xn); 
//    har.sin:= fsin(zn); 
//    har.cos:= fcos(zn); 
//    zn:= VecLenf(vec1,vec); 
//    har.hasize:= vectsize*zn+(1.0-vectsize)*hasize; 
//    VecSubf(har.no,vec,vec1); 
//    
//    
//    Normalise(har.no); 
//  end;
//  
//  if ma.mode and MA_HALO_XALPHA
//  then
//  har.type:= har.type or (HA_XALPHA); 
//  har.alfa:= ma.alpha; 
//  har.r:= 255.0*ma.r; 
//  har.g:= 255.0*ma.g; 
//  har.b:= 255.0*ma.b; 
//  har.add:= 255.0*ma.add; 
//  har.mat:= ma.ren; 
//  har.hard:= ma.har; 
//  har.seed:= ma.ren.seed1 mod 256; 
//  
//  if ma.mode and MA_STAR
//  then
//  har.starpoints:= ma.starc; 
//  if ma.mode and MA_HALO_LINES
//  then
//  har.linec:= ma.linec; 
//  if ma.mode and MA_HALO_RINGS
//  then
//  har.ringc:= ma.ringc; 
//  if ma.mode and MA_HALO_FLARE
//  then
//  har.flarec:= ma.flarec; 
//  
//  if ma.mtex[0]<>0{nil} {<= !!!9} 
//  then
//  begin 
//    
//    if (ma.mode and MA_HALOTEX)
//    then
//    har.tex:= 1; 
//    else
//    begin 
//      
//      mtex:= ma.mtex[0]; 
//      VECCOPY(texvec,vec); 
//      
//      if mtex.texco and TEXCO_NORM
//      then
//      begin 
//        ; 
//      end;
//      else
//      if mtex.texco and TEXCO_OBJECT
//      then
//      begin 
//        (* texvec[0]+= imatbase->ivec[0]; *)
//        (* texvec[1]+= imatbase->ivec[1]; *)
//        (* texvec[2]+= imatbase->ivec[2]; *)
//        (* Mat3MulVecfl(imatbase->imat, texvec); *)
//      end;
//      else
//      begin 
//        if orco<>0{nil} {<= !!!9} 
//        then
//        begin 
//          VECCOPY(texvec,orco); 
//        end;
//      end;
//      externtex(mtex,texvec); 
//      
//      yn:= Tin*mtex.colfac; 
//      zn:= Tin*mtex.varfac; 
//      
//      if mtex.mapto and MAP_COL
//      then
//      begin 
//        zn:= 1.0-yn; 
//        har.r:= 255.0*(yn*Tr+zn*ma.r); 
//        har.g:= 255.0*(yn*Tg+zn*ma.g); 
//        har.b:= 255.0*(yn*Tb+zn*ma.b); 
//      end;
//      if mtex.texco and 16
//      then
//      begin 
//        har.alfa:= 255.0*Tin; 
//      end;
//    end;
//  end;
//  
//  begin
//    result:= har; 
//    exit;
//  end;
//end;
//
//
//function initstar(vec: pfloat;  hasize: float): pHaloRen; 
//var
//har: pHaloRen; 
//zn: float; 
//hoco: array [0..Pred(4)] of float; 
//begin
//  
//  
//  projectverto(vec,hoco); 
//  if (R.r.mode and R_PANORAMA))or(testclip(hoco)=0
//  then
//  begin 
//    har:= addhalo(R.tothalo{++} inc(R.tothalo); ); 
//    VECCOPY(har.co,vec); 
//    
//    (* projectvert wordt in zbufvlaggen overgedaan ivm parts *)
//    har.hasize:= hasize; 
//    har.zd:= 0.0; 
//    
//    begin
//      result:= har; 
//      exit;
//    end;
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//
//procedure add_render_lamp(ob: pObject;  doshadbuf: integer); 
//var
//ver: pVertRen; 
//la: pLamp; 
//lar: pLampRen; 
//har: pHaloRen; 
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//hoco: array [0..Pred(4)] of float; 
//vec: array [0..Pred(3)] of float; 
//hasize: float; 
//hoek: float; 
//xn: float; 
//yn: float; 
//zn: float; 
//ok: integer; 
//z: integer; 
//c: integer; 
//b: pchar; 
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
//  if R.totlamp>=MAXLAMP
//  then
//  begin 
//    printf('lamp overflow\n'); 
//    exit;
//  end;
//  la:= ob.data; 
//  lar:= {!!!a type cast? =>} {pLampRen(}callocN(sizeof(LampRen),'lampren'); 
//  R.la[R.totlamp{++} inc(R.totlamp); ]:= lar; 
//  Mat4MulMat4(mat,ob.obmat,R.viewmat); 
//  
//  Mat4Invert(ob.imat,mat); 
//  Mat3CpyMat4(lar.imat,ob.imat); 
//  
//  lar.type:= la.type; 
//  lar.mode:= la.mode; 
//  lar.energy:= la.energy; 
//  lar.energy:= la.energy*R.wrld.exposure; 
//  
//  if la.mode and LA_NEG
//  then
//  lar.energy:= -lar.energy; 
//  lar.vec[0]:= -mat[2][0]; 
//  lar.vec[1]:= -mat[2][1]; 
//  lar.vec[2]:= -mat[2][2]; 
//  Normalise(lar.vec); 
//  
//  lar.co[0]:= mat[3][0]; 
//  lar.co[1]:= mat[3][1]; 
//  lar.co[2]:= mat[3][2]; 
//  lar.dist:= la.dist; 
//  lar.haint:= la.haint; 
//  lar.distkw:= lar.dist*lar.dist; 
//  lar.r:= lar.energy*la.r; 
//  lar.g:= lar.energy*la.g; 
//  lar.b:= lar.energy*la.b; 
//  lar.spotsi:= 0.5; 
//  lar.spotsi:= fcos(M_PI*la.spotsize div 360.0); 
//  lar.spotbl:= (1.0-lar.spotsi)*la.spotblend; 
//  memcpy(lar.mtex,la.mtex,8*4); 
//  
//  
//  lar.lay:= ob.lay; 
//  lar.ld1:= la.att1; 
//  lar.ld2:= la.att2; 
//  
//  
//  
//  if lar.type=LA_SPOT
//  then
//  begin 
//    
//    Normalise(lar.imat[0]); 
//    Normalise(lar.imat[1]); 
//    Normalise(lar.imat[2]); 
//    xn:= safacos(lar.spotsi); 
//    xn:= fsin(xn) div fcos(xn); 
//    lar.spottexfac:= 1.0 div (xn); 
//    
//    if lar.mode and LA_ONLYSHADOW
//    then
//    begin 
//      if (lar.mode and LA_SHAD)=0
//      then
//      lar.mode:= lar.mode - (LA_ONLYSHADOW); 
//      else
//      if (R.r.mode and R_SHADOW)=0
//      then
//      lar.mode:= lar.mode - (LA_ONLYSHADOW); 
//    end;
//  end;
//  
//  
//  (* imat bases *)
//  
//  
//  (* flag zetten voor spothalo en initvars *)
//  if la.type=LA_SPOT)and((la.mode and LA_HALO)
//  then
//  begin 
//    if la.haint>0.0
//    then
//    begin 
//      R.flag:= R.flag or (R_LAMPHALO); 
//      lar.sh_invcampos[0]:= -lar.co[0]; 
//      lar.sh_invcampos[1]:= -lar.co[1]; 
//      lar.sh_invcampos[2]:= -lar.co[2]; 
//      Mat3MulVecfl(lar.imat,lar.sh_invcampos); 
//      
//      (* camerapos (0,0,0) roteren rondom lamp *)
//      hoek:= safacos(lar.spotsi); 
//      xn:= lar.spotsi; 
//      yn:= fsin(hoek); 
//      lar.sh_zfac:= yn div xn; 
//      lar.sh_invcampos[2]:= lar.sh_invcampos[2] * (lar.sh_zfac); 
//      
//      (* z factor, zodat het volume genormaliseerd is *)
//      (* alvast goed scalen *)
//      
//    end;
//  end;
//  
//  for{while} c:=0 to Pred(6) { c++}
//  do
//  begin 
//    if la.mtex[c])and(la.mtex[c].tex
//    then
//    begin 
//      lar.mode:= lar.mode or (LA_TEXTURE); 
//      if R.flag and R_RENDERING
//      then
//      begin 
//        if R.osa<>0{nil} {<= !!!9} 
//        then
//        begin 
//          if la.mtex[c].tex.type=TEX_IMAGE
//          then
//          lar.mode:= lar.mode or (LA_OSATEX); 
//        end;
//      end;
//    end;
//  end;
//  
//  if R.r.mode and R_SHADOW
//  then
//  if lar.mode and LA_SHAD
//  then
//  if la.type=LA_SPOT
//  then
//  begin 
//    if doshadbuf<>0{nil} {<= !!!9} 
//    then
//    initshadowbuf(lar,la,ob.obmat); 
//  end;
//  lar.org:= dupallocN(lar); 
//  
//end;
//
//
//procedure defaultlamp; 
//var
//lar: pLampRen; 
//begin
//  lar:= {!!!a type cast? =>} {pLampRen(}callocN(sizeof(LampRen),'lampren'); 
//  R.la[R.totlamp{++} inc(R.totlamp); ]:= lar; 
//  lar.type:= LA_SUN; 
//  lar.vec[0]:= -R.viewmat[2][0]; 
//  lar.vec[1]:= -R.viewmat[2][1]; 
//  lar.vec[2]:= -R.viewmat[2][2]; 
//  Normalise(lar.vec); 
//  
//  lar.r:= 1.0; 
//  lar.g:= 1.0; 
//  lar.b:= 1.0; 
//  lar.lay:= 65535; 
//end;
//
//
//procedure split_u_renderfaces(startvlak: integer;  startvert: integer;  usize: integer;  plek: integer;  cyclu: integer); 
//var
//vlr: pVlakRen; 
//v1: pVertRen; 
//v2: pVertRen; 
//a: integer; 
//v: integer; 
//begin
//  
//  
//  
//  
//  if cyclu<>0{nil} {<= !!!9} 
//  then
//  cyclu:= 1; 
//  v:= startvert+plek*usize; 
//  (* geef eerst alle betreffende vertices een pointer naar de nieuwe mee *)
//  for{while} a:=0 to Pred(usize) { a++}
//  do
//  begin 
//    v2:= addvert(R.totvert{++} inc(R.totvert); ); 
//    v1:= addvert(v{++} inc(v); ); {*}v2^:=*v1; 
//    v1.sticky:= {!!!a type cast? =>} {pfloat(}v2; 
//  end;
//  v:= startvlak+plek*(usize-1+cyclu); 
//  (* loop betreffende vlakken af en vervang pointers *)
//  for{while} a:=1-cyclu to Pred(usize) { a++}
//  do
//  begin 
//    vlr:= addvlak(v{++} inc(v); ); 
//    vlr.v1:= {!!!a type cast? =>} {pVertRen(}(vlr.v1.sticky); 
//    vlr.v2:= {!!!a type cast? =>} {pVertRen(}(vlr.v2.sticky); 
//  end;
//end;
//
//
//
//procedure split_v_renderfaces(startvlak: integer;  startvert: integer;  usize: integer;  vsize: integer;  plek: integer;  cyclu: integer;  cyclv: integer); 
//var
//vlr: pVlakRen; 
//v1: pVertRen;
// 
//v2: pVertRen; 
//a: integer; 
//vert: integer; 
//vlak: integer; 
//ofs: integer; 
//begin
//  
//  v1:=0; 
//  
//  
//  
//  
//  if vsize<2
//  then
//  exit;
//  
//  (* loop betreffende vlakken af en maak dubbels *)
//  (* omdat (evt) split_u al gedaan is kan niet met vertex->sticky pointers worden gewerkt  *)
//  (* want vlakken delen al geen punten meer *)
//  
//  if plek+cyclu=usize
//  then
//  plek:= -1; 
//  vlak:= startvlak+(plek+cyclu); 
//  ofs:= (usize-1+cyclu); 
//  
//  for{while} a:=1 to Pred(vsize) { a++}
//  do
//  begin 
//    
//    vlr:= addvlak(vlak); 
//    if vlr.v1=0
//    then
//    exit;
//    (* OEPS, als niet cyclic *)
//    
//    v1:= addvert(R.totvert{++} inc(R.totvert); ); {*}v1^:=*(vlr.v1); 
//    vlr.v1:= v1; 
//    
//    (* vlr= addvlak(vlak+1); *)
//    (* vlr->v1= v1; *)
//    
//    if a>1
//    then
//    begin 
//      vlr:= addvlak(vlak-ofs); 
//      if vlr.v4.sticky<>0{nil} {<= !!!9} 
//      then
//      begin 
//        v1:= addvert(R.totvert{++} inc(R.totvert); ); {*}v1^:=*(vlr.v4); 
//        vlr.v4:= v1; 
//      end;
//      else
//      vlr.v4:= v1; 
//    end;
//    
//    if a=vsize-1
//    then
//    begin 
//      if cyclv<>0{nil} {<= !!!9} 
//      then
//      begin 
//        ; 
//      end;
//      else
//      begin 
//        vlr:= addvlak(vlak); 
//        v1:= addvert(R.totvert{++} inc(R.totvert); ); {*}v1^:=*(vlr.v4); 
//        vlr.v4:= v1; 
//      end;
//    end;
//    vlak:= vlak + (ofs); 
//    
//  end;
//end;
//
//
//
//
//function give_render_material(ob: pObject;  nr: integer): pMaterial; 
//var
//temp: pObject; 
//begin
//  if ob.flag and OB_FROMDUPLI
//  then
//  begin 
//    temp:= {!!!a type cast? =>} {pObject(}ob.id.new; 
//    if temp)and(temp.type=OB_FONT
//    then
//    begin 
//      ob:= temp; 
//    end;
//  end;
//  
//  begin
//    result:= give_current_material(ob,nr); 
//    exit;
//  end;
//end;
//
//
//procedure init_render_surf(ob: pObject); 
//var
//nu: pNurb;
// 
//cu: pCurve; 
//displist: ListBase; 
//dl: pDispList; 
//ver: pVertRen; 
//v1: pVertRen; 
//v2: pVertRen; 
//v3: pVertRen; 
//v4: pVertRen; 
//vlr: pVlakRen; 
//matar: array [0..Pred(32)] of pMaterial; 
//data: pfloat; 
//fp: pfloat; 
//orco: pfloat; 
//n1: array [0..Pred(3)] of float; 
//n2: array [0..Pred(3)] of float; 
//flen: float; 
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//len: integer; 
//a: integer; 
//b: integer; 
//need_orco: integer;
// 
//startvlak: integer; 
//startvert: integer; 
//p1: integer; 
//p2: integer; 
//p3: integer; 
//p4: integer; 
//ika: pIka;
// 
//begin
//  nu:=0; 
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
//  
//  
//  need_orco:=0; 
//  
//  
//  
//  
//  
//  cu:= ob.data; 
//  nu:= cu.nurb.first; 
//  if nu=0
//  then
//  exit;
//  
//  Mat4MulMat4(mat,ob.obmat,R.viewmat); 
//  Mat4Invert(ob.imat,mat); 
//  bzero(matar,4*32); 
//  
//  (* material array *)
//  matar[0]:=  and defmaterial; 
//  for{while} a:=0 to Pred(ob.totcol) { a++}
//  do
//  begin 
//    matar[a]:= give_render_material(ob,a+1); 
//    if matar[a]=0
//    then
//    matar[a]:=  and defmaterial; 
//    if matar[a])and(matar[a].ren.texco and TEXCO_ORCO
//    then
//    begin 
//      need_orco:= 1; 
//    end;
//  end;
//  
//  if ob.parent)and((ob.parent.type=OB_IKA)or(ob.parent.type=OB_LATTICE)
//  then
//  need_orco:= 1; 
//  if cu.orco=0)and(need_orco
//  then
//  make_orco_surf(cu); 
//  orco:= cu.orco; 
//  displist.first:= displist.last:=0; 
//  nu:= cu.nurb.first; 
//  (* een complete displist maken, de basedisplist kan compleet anders zijn *)
//  while nu
//  do
//  begin 
//    if nu.pntsv>1
//    then
//    begin 
//      len:= nu.resolu*nu.resolv; 
//      dl:= callocN(sizeof(DispList)+len*3*sizeof(float),'makeDispList1'); 
//      dl.verts:= callocN(len*3*sizeof(float),'makeDispList01'); 
//      addtail( and displist,dl); 
//      (* makeNurbfaces wil nullen *)
//      
//      dl.parts:= nu.resolu; 
//      dl.nr:= nu.resolv; 
//      dl.col:= nu.mat_nr; 
//      dl.rt:= nu.flag; 
//      data:= dl.verts; 
//      dl.type:= DL_SURF; 
//      (* andersom want makeNurbfaces gaat zo *)
//      
//      if nu.flagv and 1
//      then
//      dl.flag:= dl.flag or (1); (* ook andersom ! *)
//      if nu.flagu and 1
//      then
//      dl.flag:= dl.flag or (2); 
//      makeNurbfaces(nu,data); 
//      
//    end;
//    nu:= nu.next; 
//  end;
//  
//  if ob.parent)and(ob.parent.type=OB_LATTICE
//  then
//  begin 
//    init_latt_deform(ob.parent,ob); 
//    dl:= displist.first; 
//    while dl
//    do
//    begin 
//      
//      fp:= dl.verts; 
//      len:= dl.nr*dl.parts; 
//      while{for} 
//      a:= 0; 
//      {to} a<len
//      {;}
//      inc(a); 
//      fp:= fp + (3); 
//      do
//      calc_latt_deform(fp); 
//      dl:= dl.next; 
//      
//    end;
//  end_latt_deform(); 
//end;
//if ob.parent)and(ob.parent.type=OB_IKA
//then
//begin 
//  ika:=ob.parent.data; 
//  init_skel_deform(ob.parent,ob); 
//  dl:= displist.first; 
//  while dl
//  do
//  begin 
//    
//    fp:= dl.verts; 
//    len:= dl.nr*dl.parts; 
//    while{for} 
//    a:= 0; 
//    {to} a<len
//    {;}
//    inc(a); 
//    fp:= fp + (3); 
//    do
//    calc_skel_deform(ika,fp); 
//    dl:= dl.next; 
//    
//  end;
//end;
//dl:= displist.first; 
//while dl
//do
//begin 
//  if dl.type=DL_SURF
//  then
//  begin 
//    startvert:= R.totvert; 
//    a:= dl.nr*dl.parts; 
//    data:= dl.verts; 
//    while a{--} dec(a); 
//    do
//    begin 
//      ver:= addvert(R.totvert{++} inc(R.totvert); ); 
//      VECCOPY(ver.co,data); 
//      if orco<>0{nil} {<= !!!9} 
//      then
//      begin 
//        ver.orco:= orco; 
//        orco:= orco + (3); 
//      end;
//      Mat4MulVecfl(mat,ver.co); 
//      data:= data + (3); 
//    end;
//    startvlak:= R.totvlak; 
//    
//    for{while} a:=0 to Pred(dl.parts) { a++}
//    do
//    begin 
//      
//      DL_SURFINDEX(dl.flag and 1,dl.flag and 2,dl.nr,dl.parts); 
//      p1:= p1 + (startvert); 
//      p2:= p2 + (startvert); 
//      p3:= p3 + (startvert); 
//      p4:= p4 + (startvert); 
//      for{while}  to Pred(dl.nr) { b++}
//      do
//      begin 
//        v1:= addvert(p1); 
//        v2:= addvert(p2); 
//        v3:= addvert(p3); 
//        v4:= addvert(p4); 
//        flen:= CalcNormFloat4(v1.co,v3.co,v4.co,v2.co,n1); 
//        if flen<>0.0
//        then
//        begin 
//          vlr:= addvlak(R.totvlak{++} inc(R.totvlak); ); 
//          vlr.v1:= v1; 
//          vlr.v2:= v3; 
//          vlr.v3:= v4; 
//          vlr.v4:= v2; 
//          VECCOPY(vlr.n,n1); 
//          vlr.len:= flen; 
//          vlr.lay:= ob.lay; 
//          vlr.mat:= matar[dl.col]; 
//          vlr.ec:= ME_V1V2+ME_V2V3; 
//          vlr.flag:= dl.rt; 
//          if cu.flag and CU_NOPUNOFLIP
//          then
//          begin 
//            vlr.flag:= vlr.flag or (R_NOPUNOFLIP); 
//            vlr.puno:= 15; 
//          end;
//        end;
//        VecAddf(v1.n,v1.n,n1); 
//        
//        VecAddf(v2.n,v2.n,n1); 
//        VecAddf(v3.n,v3.n,n1); 
//        VecAddf(v4.n,v4.n,n1); 
//        p4:= p3; 
//        inc(p3); 
//        p2:= p1; 
//        inc(p1); 
//        
//      end;
//    end;
//    
//    for{while} a:=startvert to Pred(R.totvert) { a++}
//    do
//    begin 
//      ver:= addvert(a); 
//      Normalise(ver.n); 
//    end;
//  end;
//  
//  
//  dl:= dl.next; 
//end;
//freedisplist( and displist); 
//end;
//
//
//procedure init_render_curve(ob: pObject); 
//var
//ika: pIka;
// 
//lt: pLattice;
// 
//cu: pCurve; 
//ver: pVertRen; 
//vlr: pVlakRen; 
//vlr1: pVlakRen; 
//dlbev: ListBase; 
//nu: pNurb;
// 
//dlb: pDispList; 
//dl: pDispList; 
//bl: pBevList; 
//bevp: pBevPoint; 
//matar: array [0..Pred(32)] of pMaterial; 
//len: float; 
//data: pfloat; 
//fp: pfloat; 
//fp1: pfloat; 
//fac: float; 
//n: array [0..Pred(3)] of float; 
//vec: array [0..Pred(3)] of float; 
//widfac: float; 
//size: array [0..Pred(3)] of float; 
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//nr: integer; 
//startvert: integer; 
//startvlak: integer; 
//a: integer; 
//b: integer; 
//p1: integer; 
//p2: integer; 
//p3: integer; 
//p4: integer; 
//totvert: integer; 
//frontside: integer; 
//ofs: integer; 
//end: integer; 
//need_orco: integer;
// 
//firststartvert: integer; 
//index: pinteger; 
//begin
//  ika:=0; 
//  lt:=0; 
//  
//  
//  
//  
//  
//  nu:=0; 
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
//  need_orco:=0; 
//  
//  cu:= ob.data; 
//  nu:= cu.nurb.first; 
//  if nu=0
//  then
//  exit;
//  
//  (* displist testen *)
//  if cu.disp.first=0
//  then
//  makeDispList(ob); 
//  dl:= cu.disp.first; 
//  if cu.disp.first=0
//  then
//  exit;
//  
//  if dl.type<>DL_INDEX3
//  then
//  begin 
//    curve_to_filledpoly(cu, and cu.disp); 
//  end;
//  
//  if cu.bev.first=0
//  then
//  makeBevelList(ob); 
//  firststartvert:= R.totvert; 
//  Mat4MulMat4(mat,ob.obmat,R.viewmat); 
//  
//  
//  Mat4Invert(ob.imat,mat); 
//  bzero(matar,4*32); 
//  
//  (* material array *)
//  matar[0]:=  and defmaterial; 
//  for{while} a:=0 to Pred(ob.totcol) { a++}
//  do
//  begin 
//    matar[a]:= give_render_material(ob,a+1); 
//    if matar[a]=0
//    then
//    matar[a]:=  and defmaterial; 
//    if matar[a].ren.texco and TEXCO_ORCO
//    then
//    begin 
//      need_orco:= 1; 
//    end;
//  end;
//  dlbev.first:= dlbev.last:=0; 
//  (* bevelcurve in displist *)
//  
//  if cu.ext1<>0.0)or(cu.ext2<>0.0)or(cu.bevobj<>0
//  then
//  begin 
//    makebevelcurve(ob, and dlbev); 
//  end;
//  
//  (* uv orco's? aantal punten tellen en malloccen *)
//  if need_orco)and((cu.flag and CU_UV_ORCO)
//  then
//  begin 
//    if cu.flag and CU_PATH
//    then
//    ; 
//    else
//    begin 
//      totvert:= 0; 
//      bl:= cu.bev.first; 
//      while bl
//      do
//      begin 
//        dlb:= dlbev.first; 
//        while dlb
//        do
//        begin 
//          totvert:= totvert + (dlb.nr*bl.nr); 
//          dlb:= dlb.next; 
//        end;
//        bl:= bl.next; 
//      end;
//      fp:= cu.orco:=mallocN(3*sizeof(float)*totvert,'cu->orco'); 
//      bl:= cu.bev.first; 
//      
//      while bl
//      do
//      begin 
//        dlb:= dlbev.first; 
//        while dlb
//        do
//        begin 
//          for{while} b:=0 to Pred(dlb.nr) { b++}
//          do
//          begin 
//            fac:= (2.0*b div {!!!a type cast? =>} {float(}(dlb.nr-1))-1.0; 
//            while{for} 
//            a:= 0; 
//            {to} a<bl.nr
//            {;}
//            inc(a); 
//            fp:= fp + (3); 
//            do
//            begin 
//              fp[0]:= (2.0*a div {!!!a type cast? =>} {float(}(bl.nr-1))-1.0; 
//              fp[1]:= fac; 
//              fp[2]:= 0.0; 
//            end;
//          end;
//          dlb:= dlb.next; 
//        end;
//        bl:= bl.next; 
//      end;
//    end;
//  end;
//  
//  if ob.parent)and(ob.parent.type=OB_LATTICE
//  then
//  begin 
//    lt:= ob.parent.data; 
//    init_latt_deform(ob.parent,ob); 
//    need_orco:= 1; 
//  end;
//  if ob.parent)and(ob.parent.type=OB_IKA
//  then
//  begin 
//    ika:= ob.parent.data; 
//    init_skel_deform(ob.parent,ob); 
//    need_orco:= 1; 
//  end;
//  widfac:= (cu.width-1.0); 
//  bl:= cu.bev.first; 
//  nu:= cu.nurb.first; 
//  (* keypos doen? NOTITIE: pas op : orco's *)
//  
//  (* effect op text? *)
//  
//  (* boundboxclip nog doen *)
//  
//  (* polyzijvlakken:  met bevellist werken *)
//  
//  while bl
//  do
//  begin 
//    
//    if dlbev.first<>0{nil} {<= !!!9} 
//    then
//    begin 
//      (* anders alleen een poly *)
//      
//      dlb:= dlbev.first; (* bevel lus *)
//      while dlb
//      do
//      begin 
//        data:= mallocN(3*sizeof(float)*dlb.nr*bl.nr,'init_render_curve3'); 
//        fp:= data; 
//        fp1:= dlb.verts; 
//        b:= dlb.nr; 
//        (* voor ieder punt van bevelcurve de hele poly doen *)
//        while b{--} dec(b); 
//        do
//        begin 
//          
//          bevp:= {!!!a type cast? =>} {pBevPoint(}(bl+1); 
//          a:= bl.nr; 
//          while a{--} dec(a); 
//          do
//          begin 
//            
//            if cu.flag and CU_3D
//            then
//            begin 
//              vec[0]:= fp1[1]+widfac; 
//              vec[1]:= fp1[2]; 
//              vec[2]:= 0.0; 
//              Mat3MulVecfl(bevp.mat,vec); 
//              
//              fp[0]:= bevp.x+vec[0]; 
//              fp[1]:= bevp.y+vec[1]; 
//              fp[2]:= bevp.z+vec[2]; 
//              
//            end;
//            else
//            begin 
//              
//              fp[0]:= bevp.x+(widfac+fp1[1])*bevp.sina; 
//              fp[1]:= bevp.y+(widfac+fp1[1])*bevp.cosa; 
//              fp[2]:= bevp.z+fp1[2]; 
//              (* hier niet al MatMullen: polyfill moet uniform werken, ongeacht frame *)
//            end;
//            fp:= fp + (3); 
//            inc(bevp); 
//          end;
//          fp1:= fp1 + (3); 
//        end;
//        fp:= data; 
//        startvert:= R.totvert; 
//        nr:= dlb.nr*bl.nr; 
//        (* rendervertices maken *)
//        
//        while nr{--} dec(nr); 
//        do
//        begin 
//          ver:= addvert(R.totvert{++} inc(R.totvert); ); 
//          if lt<>0{nil} {<= !!!9} 
//          then
//          calc_latt_deform(fp); 
//          else
//          if ika<>0{nil} {<= !!!9} 
//          then
//          calc_skel_deform(ika,fp); 
//          VECCOPY(ver.co,fp); 
//          Mat4MulVecfl(mat,ver.co); 
//          fp:= fp + (3); 
//        end;
//        startvlak:= R.totvlak; 
//        
//        for{while} a:=0 to Pred(dlb.nr) { a++}
//        do
//        begin 
//          
//          frontside:= (a>=dlb.nr div 2); 
//          DL_SURFINDEX(bl.poly>0,dlb.type=DL_POLY,bl.nr,dlb.nr); 
//          
//          p1:= p1 + (startvert); 
//          p2:= p2 + (startvert); 
//          p3:= p3 + (startvert); 
//          p4:= p4 + (startvert); 
//          for{while}  to Pred(bl.nr) { b++}
//          do
//          begin 
//            
//            vlr:= addvlak(R.totvlak{++} inc(R.totvlak); ); 
//            vlr.v1:= addvert(p2); 
//            vlr.v2:= addvert(p1); 
//            vlr.v3:= addvert(p3); 
//            vlr.v4:= addvert(p4); 
//            vlr.ec:= ME_V2V3+ME_V3V4; 
//            if a=0
//            then
//            vlr.ec:= vlr.ec + (ME_V1V2); 
//            vlr.flag:= nu.flag; 
//            vlr.lay:= ob.lay; 
//            
//            (* dit is niet echt wetenschappelijk: de vertices
//                   * 2, 3 en 4 geven betere puno's dan 1 2 3: voor en achterkant anders!!
//                   *)
//            
//            if frontside<>0{nil} {<= !!!9} 
//            then
//            vlr.len:= CalcNormFloat(vlr.v2.co,vlr.v3.co,vlr.v4.co,vlr.n); 
//            else
//            vlr.len:= CalcNormFloat(vlr.v1.co,vlr.v2.co,vlr.v3.co,vlr.n); 
//            vlr.mat:= matar[nu.mat_nr]; 
//            p4:= p3; 
//            inc(p3); 
//            p2:= p1; 
//            inc(p1); 
//            
//            
//            
//          end;
//        end;
//        
//        
//        (* dubbele punten maken: POLY SPLITSEN *)
//        if dlb.nr=4)and(cu.bevobj=0
//        then
//        begin 
//          split_u_renderfaces(startvlak,startvert,bl.nr,1,bl.poly>0); 
//          split_u_renderfaces(startvlak,startvert,bl.nr,2,bl.poly>0); 
//        end;
//        bevp:= {!!!a type cast? =>} {pBevPoint(}(bl+1); (* dubbele punten maken: BEVELS SPLITSEN *)
//        for{while} a:=0 to Pred(bl.nr) { a++}
//        do
//        begin 
//          if bevp.f1<>0{nil} {<= !!!9} 
//          then
//          split_v_renderfaces(startvlak,startvert,bl.nr,dlb.nr,a,bl.poly>0,dlb.type=DL_POLY); 
//          inc(bevp); 
//        end;
//        
//        (* puntnormalen *)
//        for{while} a:=startvlak to Pred(R.totvlak) { a++}
//        do
//        begin 
//          vlr:= addvlak(a); 
//          VecAddf(vlr.v1.n,vlr.v1.n,vlr.n); 
//          
//          VecAddf(vlr.v3.n,vlr.v3.n,vlr.n); 
//          VecAddf(vlr.v2.n,vlr.v2.n,vlr.n); 
//          VecAddf(vlr.v4.n,vlr.v4.n,vlr.n); 
//        end;
//        for{while} a:=startvert to Pred(R.totvert) { a++}
//        do
//        begin 
//          ver:= addvert(a); 
//          len:= Normalise(ver.n); 
//          if len=0.0
//          then
//          ver.sticky:= (float* )1; 
//          else
//          ver.sticky:= 0; 
//        end;
//        for{while} a:=startvlak to Pred(R.totvlak) { a++}
//        do
//        begin 
//          vlr:= addvlak(a); 
//          if vlr.v1.sticky<>0{nil} {<= !!!9} 
//          then
//          VECCOPY(vlr.v1.n,vlr.n); 
//          if vlr.v2.sticky<>0{nil} {<= !!!9} 
//          then
//          VECCOPY(vlr.v2.n,vlr.n); 
//          if vlr.v3.sticky<>0{nil} {<= !!!9} 
//          then
//          VECCOPY(vlr.v3.n,vlr.n); 
//          if vlr.v4.sticky<>0{nil} {<= !!!9} 
//          then
//          VECCOPY(vlr.v4.n,vlr.n); 
//        end;
//        dlb:= dlb.next; 
//        freeN(data); 
//        
//        
//      end;
//    end;
//    
//    bl:= bl.next; 
//    nu:= nu.next; 
//  end;
//  
//  if dlbev.first<>0{nil} {<= !!!9} 
//  then
//  begin 
//    freedisplist( and dlbev); 
//  end;
//  
//  if cu.flag and CU_PATH
//  then
//  exit;
//  
//  (* uit de displist kunnen de vulvlakken worden gehaald *)
//  dl:= cu.disp.first; 
//  while dl
//  do
//  begin 
//    if dl.type=DL_INDEX3
//    then
//    begin 
//      
//      startvert:= R.totvert; 
//      data:= dl.verts; 
//      n[0]:= ob.imat[0][2]; 
//      n[1]:= ob.imat[1][2]; 
//      n[2]:= ob.imat[2][2]; 
//      Normalise(n); 
//      
//      
//      while{for} 
//      a:= 0; 
//      {to} a<dl.nr
//      {;}
//      inc(a); 
//      data:= data + (3); 
//      do
//      begin 
//        
//        ver:= addvert(R.totvert{++} inc(R.totvert); ); 
//        VECCOPY(ver.co,data); 
//        Mat4MulVecfl(mat,ver.co); 
//        VECCOPY(ver.n,n); 
//        
//      end;
//      startvlak:= R.totvlak; 
//      index:= dl.index; 
//      while{for} 
//      a:= 0; 
//      {to} a<dl.parts
//      {;}
//      inc(a); 
//      index:= index + (3); 
//      do
//      begin 
//        
//        vlr:= addvlak(R.totvlak{++} inc(R.totvlak); ); 
//        vlr.v1:= addvert(startvert+index[0]); 
//        vlr.v2:= addvert(startvert+index[1]); 
//        vlr.v3:= addvert(startvert+index[2]); 
//        vlr.v4:= 0; 
//        VECCOPY(vlr.n,n); 
//        
//        vlr.mface:= 0; 
//        vlr.mat:= matar[dl.col]; 
//        vlr.puno:= 0; 
//        vlr.flag:= 0; 
//        vlr.ec:= 0; 
//        vlr.lay:= ob.lay; 
//        
//      end;
//    end;
//    
//    dl:= dl.next; 
//  end;
//  
//  if lt<>0{nil} {<= !!!9} 
//  then
//  begin 
//  end_latt_deform(); 
//end;
//
//if need_orco<>0{nil} {<= !!!9} 
//then
//begin 
//  (* de domme methode: snel vervangen; rekening houden met keys! *)
//  
//  VECCOPY(size,cu.size); 
//  nr:= R.totvert-firststartvert; 
//  if nr<>0{nil} {<= !!!9} 
//  then
//  begin 
//    if cu.orco<>0{nil} {<= !!!9} 
//    then
//    begin 
//      fp:= cu.orco; 
//      while nr{--} dec(nr); 
//      do
//      begin 
//        ver:= addvert(firststartvert{++} inc(firststartvert); ); 
//        ver.orco:= fp; 
//        fp:= fp + (3); 
//      end;
//    end;
//    else
//    begin 
//      fp:= cu.orco:=mallocN(sizeof(float)*3*nr,'cu orco'); 
//      while nr{--} dec(nr); 
//      do
//      begin 
//        ver:= addvert(firststartvert{++} inc(firststartvert); ); 
//        ver.orco:= fp; 
//        VECCOPY(fp,ver.co); 
//        
//        Mat4MulVecfl(ob.imat,fp); 
//        fp[0]:= (fp[0]-cu.loc[0]) div size[0]; 
//        fp[1]:= (fp[1]-cu.loc[1]) div size[1]; 
//        fp[2]:= (fp[2]-cu.loc[2]) div size[2]; 
//        fp:= fp + (3); 
//        
//      end;
//    end;
//  end;
//end;
//end;
//
//
//procedure render_particle_system(ob: pObject;  paf: pPartEff); 
//var
//pa: pParticle;
// 
//har: pHaloRen;
// 
//ma: pMaterial;
// 
//mtex: pMTex;
// 
//xn: float; 
//yn: float; 
//zn: float; 
//imat: array [0..Pred(3),0..Pred(3)] of float; 
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//hasize: float; 
//ptime: float; 
//ctime: float; 
//vec: array [0..Pred(3)] of float; 
//vec1: array [0..Pred(3)] of float; 
//view: array [0..Pred(3)] of float; 
//nor: array [0..Pred(3)] of float; 
//a: integer; 
//mat_nr: integer;
// 
//begin
//  pa:=0; 
//  har:=0; 
//  ma:=0; 
//  mtex:=0; 
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
//  mat_nr:=1; 
//  pa:= paf.keys; 
//  if pa=0
//  then
//  begin 
//    build_particle_system(ob); 
//    pa:= paf.keys; 
//    if pa=0
//    then
//    exit;
//  end;
//  ma:= give_render_material(ob,1); 
//  if ma=0
//  then
//  ma:=  and defmaterial; 
//  Mat4MulMat4(mat,ob.obmat,R.viewmat); 
//  
//  Mat4Invert(ob.imat,mat); 
//  Mat4Invert(mat,R.viewmat); 
//  (* hoort zo, voor imat texture *)
//  
//  Mat3CpyMat4(imat,mat); 
//  (* particles hebben geen ob transform meer *)
//  R.flag:= R.flag or (R_HALO); 
//  
//  if ob.ipoflag and OB_OFFS_PARTICLE
//  then
//  ptime:= ob.sf; 
//  else
//  ptime:= 0.0; 
//  ctime:= bsystem_time(ob,0,{!!!a type cast? =>} {float(}CFRA,ptime); 
//  ma.ren.seed1:= ma.seed1; 
//  while{for} 
//  a:= 0; 
//  {to} a<paf.totpart
//  {;}
//  inc(a); 
//  pa:= pa + (paf.totkey); 
//  do
//  begin 
//    
//    if ctime>pa.time
//    then
//    begin 
//      if ctime<pa.time+pa.lifetime
//      then
//      begin 
//        
//        (* let op: ook nog de normaal van de particle berekenen *)
//        if paf.stype=PAF_VECT)or(ma.mode and MA_HALO_SHADE
//        then
//        begin 
//          where_is_particle(paf,pa,ctime,vec); 
//          Mat4MulVecfl(R.viewmat,vec); 
//          where_is_particle(paf,pa,ctime+1.0,vec1); 
//          Mat4MulVecfl(R.viewmat,vec1); 
//        end;
//        else
//        begin 
//          where_is_particle(paf,pa,ctime,vec); 
//          Mat4MulVecfl(R.viewmat,vec); 
//        end;
//        
//        if pa.mat_nr<>mat_nr
//        then
//        begin 
//          mat_nr:= pa.mat_nr; 
//          ma:= give_render_material(ob,mat_nr); 
//          if ma=0
//          then
//          ma:=  and defmaterial; 
//        end;
//        
//        if ma.ipo<>0{nil} {<= !!!9} 
//        then
//        begin 
//          (* correctie voor lifetime *)
//          ptime:= 100.0*(ctime-pa.time) div pa.lifetime; 
//          calc_ipo(ma.ipo,ptime); 
//          execute_ipo({!!!a type cast? =>} {pID(}ma,ma.ipo); 
//        end;
//        hasize:= ma.hasize; 
//        
//        if ma.mode and MA_HALOPUNO
//        then
//        begin 
//          xn:= pa.no[0]; 
//          yn:= pa.no[1]; 
//          zn:= pa.no[2]; 
//          nor[0]:= imat[0][0]*xn+imat[0][1]*yn+imat[0][2]*zn; 
//          nor[1]:= imat[1][0]*xn+imat[1][1]*yn+imat[1][2]*zn; 
//          nor[2]:= imat[2][0]*xn+imat[2][1]*yn+imat[2][2]*zn; 
//          Normalise(nor); 
//          
//          (* transpose ! *)
//          VECCOPY(view,vec); 
//          
//          Normalise(view); 
//          zn:= nor[0]*view[0]+nor[1]*view[1]+nor[2]*view[2]; 
//          if zn>=0.0
//          then
//          hasize:= 0.0; 
//          else
//          hasize:= hasize * (zn*zn*zn*zn); 
//        end;
//        
//        if paf.stype=PAF_VECT
//        then
//        har:= inithalo(ma,vec,vec1,pa.co,hasize,paf.vectsize); 
//        else
//        begin 
//          har:= inithalo(ma,vec,0,pa.co,hasize,0); 
//          if ma.mode and MA_HALO_SHADE
//          then
//          begin 
//            VecSubf(har.no,vec,vec1); 
//            Normalise(har.no); 
//          end;
//        end;
//      end;
//    end;
//    inc(ma.ren.seed1); 
//  end;
//end;
//
//
//
//procedure render_static_particle_system(ob: pObject;  paf: pPartEff); 
//var
//pa: pParticle;
// 
//har: pHaloRen;
// 
//ma: pMaterial;
// 
//mtex: pMTex;
// 
//xn: float; 
//yn: float; 
//zn: float; 
//imat: array [0..Pred(3),0..Pred(3)] of float; 
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//hasize: float; 
//mtime: float; 
//ptime: float; 
//ctime: float; 
//vec: array [0..Pred(3)] of float; 
//vec1: array [0..Pred(3)] of float; 
//view: array [0..Pred(3)] of float; 
//nor: array [0..Pred(3)] of float; 
//a: integer; 
//mat_nr: integer;
// 
//begin
//  pa:=0; 
//  har:=0; 
//  ma:=0; 
//  mtex:=0; 
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
//  mat_nr:=1; 
//  pa:= paf.keys; 
//  if pa=0
//  then
//  begin 
//    build_particle_system(ob); 
//    pa:= paf.keys; 
//    if pa=0
//    then
//    exit;
//  end;
//  ma:= give_render_material(ob,1); 
//  if ma=0
//  then
//  ma:=  and defmaterial; 
//  Mat4MulMat4(mat,ob.obmat,R.viewmat); 
//  
//  Mat4Invert(ob.imat,mat); 
//  Mat3CpyMat4(imat,ob.imat); 
//  (* hoort zo, voor imat texture *)
//  
//  R.flag:= R.flag or (R_HALO); 
//  
//  if ob.ipoflag and OB_OFFS_PARTICLE
//  then
//  ptime:= ob.sf; 
//  else
//  ptime:= 0.0; 
//  ctime:= bsystem_time(ob,0,{!!!a type cast? =>} {float(}CFRA,ptime); 
//  ma.ren.seed1:= ma.seed1; 
//  while{for} 
//  a:= 0; 
//  {to} a<paf.totpart
//  {;}
//  inc(a); 
//  pa:= pa + (paf.totkey); 
//  do
//  begin 
//    
//    where_is_particle(paf,pa,pa.time,vec1); 
//    Mat4MulVecfl(mat,vec1); 
//    mtime:= pa.time+pa.lifetime+paf.staticstep-1; 
//    while{for} 
//    ctime:= pa.time; 
//    {to} ctime<mtime
//    {;}
//    ctime:= ctime + (paf.staticstep); 
//    do
//    begin 
//      (* make sure hair grows until the end.. *)
//      if ctime>pa.time+pa.lifetime
//      then
//      ctime:= pa.time+pa.lifetime; 
//      (* let op: ook nog de normaal van de particle berekenen *)
//      if paf.stype=PAF_VECT)or(ma.mode and MA_HALO_SHADE
//      then
//      begin 
//        where_is_particle(paf,pa,ctime+1.0,vec); 
//        Mat4MulVecfl(mat,vec); 
//      end;
//      else
//      begin 
//        where_is_particle(paf,pa,ctime,vec); 
//        Mat4MulVecfl(mat,vec); 
//      end;
//      
//      if pa.mat_nr<>mat_nr
//      then
//      begin 
//        mat_nr:= pa.mat_nr; 
//        ma:= give_render_material(ob,mat_nr); 
//        if ma=0
//        then
//        ma:=  and defmaterial; 
//      end;
//      
//      if ma.ipo<>0{nil} {<= !!!9} 
//      then
//      begin 
//        (* correctie voor lifetime *)
//        ptime:= 100.0*(ctime-pa.time) div pa.lifetime; 
//        calc_ipo(ma.ipo,ptime); 
//        execute_ipo({!!!a type cast? =>} {pID(}ma,ma.ipo); 
//      end;
//      hasize:= ma.hasize; 
//      
//      if ma.mode and MA_HALOPUNO
//      then
//      begin 
//        xn:= pa.no[0]; 
//        yn:= pa.no[1]; 
//        zn:= pa.no[2]; 
//        nor[0]:= imat[0][0]*xn+imat[0][1]*yn+imat[0][2]*zn; 
//        nor[1]:= imat[1][0]*xn+imat[1][1]*yn+imat[1][2]*zn; 
//        nor[2]:= imat[2][0]*xn+imat[2][1]*yn+imat[2][2]*zn; 
//        Normalise(nor); 
//        
//        (* transpose ! *)
//        VECCOPY(view,vec); 
//        
//        Normalise(view); 
//        zn:= nor[0]*view[0]+nor[1]*view[1]+nor[2]*view[2]; 
//        if zn>=0.0
//        then
//        hasize:= 0.0; 
//        else
//        hasize:= hasize * (zn*zn*zn*zn); 
//      end;
//      
//      if paf.stype=PAF_VECT
//      then
//      har:= inithalo(ma,vec,vec1,pa.co,hasize,paf.vectsize); 
//      else
//      begin 
//        har:= inithalo(ma,vec,0,pa.co,hasize,0); 
//        if ma.mode and MA_HALO_SHADE
//        then
//        begin 
//          VecSubf(har.no,vec,vec1); 
//          Normalise(har.no); 
//        end;
//      end;
//      VECCOPY(vec1,vec); 
//    end;
//    inc(ma.ren.seed1); 
//  end;
//end;
//
//
//
//
//procedure make_render_halos(ob: pObject;  me: pMesh;  ma: pMaterial;  extverts: pfloat); 
//var
//har: pHaloRen; 
//mvert: pMVert; 
//mtex: pMTex; 
//xn: float; 
//yn: float; 
//zn: float; 
//nor: array [0..Pred(3)] of float; 
//view: array [0..Pred(3)] of float; 
//orco: pfloat; 
//tinc: float; 
//vec: array [0..Pred(3)] of float; 
//hasize: float; 
//fp: pfloat; 
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//imat: array [0..Pred(3),0..Pred(3)] of float; 
//start: integer; 
//end: integer; 
//a: integer; 
//ok: integer; 
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
//  Mat4MulMat4(mat,ob.obmat,R.viewmat); 
//  Mat3CpyMat4(imat,ob.imat); 
//  R.flag:= R.flag or (R_HALO); 
//  mvert:= me.mvert; 
//  orco:= me.orco; 
//  start:= 0; 
//end:= me.totvert; 
//set_buildvars(ob,@start, and end); 
//
//
//
//mvert:= mvert + (start); 
//if extverts<>0{nil} {<= !!!9} 
//then
//extverts:= extverts + (3*start); 
//ma.ren.seed1:= ma.seed1; 
//
//while{for} 
//a:= start; 
//{to} a<end
//{;}
//inc(a); 
//inc(mvert); 
//do
//begin 
//  ok:= 1; 
//  if ok<>0{nil} {<= !!!9} 
//  then
//  begin 
//    hasize:= ma.hasize; 
//    if extverts<>0{nil} {<= !!!9} 
//    then
//    begin 
//      VECCOPY(vec,extverts); 
//      extverts:= extverts + (3); 
//    end;
//    else
//    begin 
//      VECCOPY(vec,mvert.co); 
//    end;
//    Mat4MulVecfl(mat,vec); 
//    
//    if ma.mode and MA_HALOPUNO
//    then
//    begin 
//      xn:= mvert.no[0]; 
//      yn:= mvert.no[1]; 
//      zn:= mvert.no[2]; 
//      nor[0]:= imat[0][0]*xn+imat[0][1]*yn+imat[0][2]*zn; 
//      nor[1]:= imat[1][0]*xn+imat[1][1]*yn+imat[1][2]*zn; 
//      nor[2]:= imat[2][0]*xn+imat[2][1]*yn+imat[2][2]*zn; 
//      Normalise(nor); 
//      
//      (* transpose ! *)
//      VECCOPY(view,vec); 
//      
//      Normalise(view); 
//      zn:= nor[0]*view[0]+nor[1]*view[1]+nor[2]*view[2]; 
//      if zn>=0.0
//      then
//      hasize:= 0.0; 
//      else
//      hasize:= hasize * (zn*zn*zn*zn); 
//    end;
//    
//    if orco<>0{nil} {<= !!!9} 
//    then
//    har:= inithalo(ma,vec,0,orco,hasize,0); 
//    else
//    inithalo(ma,vec,0,mvert.co,hasize,0); 
//  end;
//  
//  if orco<>0{nil} {<= !!!9} 
//  then
//  orco:= orco + (3); 
//  inc(ma.ren.seed1); 
//end;
//end;
//
//(* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ *)
//
//
//function contrpuntnormr(n: pfloat;  puno: pfloat): integer; 
//var
//inp: float; 
//begin
//  inp:= n[0]*puno[0]+n[1]*puno[1]+n[2]*puno[2]; 
//  if inp<0.0
//  then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//
//
//procedure normalenrender(startvert: integer;  startvlak: integer); 
//var
//vlr: pVlakRen; 
//vlro: pVlakRen; 
//ver: pVertRen; 
//adrve1: pVertRen; 
//adrve2: pVertRen; 
//adrve3: pVertRen; 
//adrve4: pVertRen; 
//n1: array [0..Pred(3)] of float; 
//n2: array [0..Pred(3)] of float; 
//n3: array [0..Pred(3)] of float; 
//n4: array [0..Pred(3)] of float; 
//adrco: pfloat; 
//adrno: pfloat; 
//tfl: pfloat; 
//fac: float; 
//f1: pfloat; 
//temp: pfloat; 
//a: integer; 
//x: smallint; 
//y: smallint; 
//z: smallint; 
//z1: psmallint;
// 
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
//  
//  
//  
//  z1:=0; 
//  if R.totvlak=0)or(R.totvert=0
//  then
//  exit;
//  if startvert=R.totvert)or(startvlak=R.totvlak
//  then
//  exit;
//  
//  adrco:= {!!!a type cast? =>} {pfloat(}callocN(12+4*sizeof(float)*(R.totvlak-startvlak),'normalen1'); 
//  tfl:= adrco; 
//  (* berekenen cos hoeken en puntmassa's *)
//  for{while} a:=startvlak to Pred(R.totvlak) { a++}
//  do
//  begin 
//    vlr:= addvlak(a); 
//    adrve1:= vlr.v1; 
//    adrve2:= vlr.v2; 
//    adrve3:= vlr.v3; 
//    adrve4:= vlr.v4; 
//    VecSubf(n1,adrve2.co,adrve1.co); 
//    
//    
//    Normalise(n1); 
//    VecSubf(n2,adrve3.co,adrve2.co); 
//    Normalise(n2); 
//    if adrve4=0
//    then
//    begin 
//      VecSubf(n3,adrve1.co,adrve3.co); 
//      Normalise(n3); 
//      *(tfl{++} inc(tfl); ):=safacos(-n1[0]*n3[0]-n1[1]*n3[1]-n1[2]*n3[2]); 
//      *(tfl{++} inc(tfl); ):=safacos(-n1[0]*n2[0]-n1[1]*n2[1]-n1[2]*n2[2]); 
//      *(tfl{++} inc(tfl); ):=safacos(-n2[0]*n3[0]-n2[1]*n3[1]-n2[2]*n3[2]); 
//    end;
//    
//    else
//    begin 
//      VecSubf(n3,adrve4.co,adrve3.co); 
//      Normalise(n3); 
//      VecSubf(n4,adrve1.co,adrve4.co); 
//      Normalise(n4); 
//      *(tfl{++} inc(tfl); ):=safacos(-n4[0]*n1[0]-n4[1]*n1[1]-n4[2]*n1[2]); 
//      *(tfl{++} inc(tfl); ):=safacos(-n1[0]*n2[0]-n1[1]*n2[1]-n1[2]*n2[2]); 
//      *(tfl{++} inc(tfl); ):=safacos(-n2[0]*n3[0]-n2[1]*n3[1]-n2[2]*n3[2]); 
//      *(tfl{++} inc(tfl); ):=safacos(-n3[0]*n4[0]-n3[1]*n4[1]-n3[2]*n4[2]); 
//    end;
//    
//  end;
//  
//  (* alle puntnormalen leegmaken *)
//  for{while} a:=startvert to Pred(R.totvert) { a++}
//  do
//  begin 
//    ver:= addvert(a); 
//    ver.n[0]:= ver.n[1]:=ver.n[2]:=0.0; 
//    
//  end;
//  tfl:= adrco; 
//  (* berekenen normalen en optellen bij puno's *)
//  for{while} a:=startvlak to Pred(R.totvlak) { a++}
//  do
//  begin 
//    vlr:= addvlak(a); 
//    adrve1:= vlr.v1; 
//    adrve2:= vlr.v2; 
//    adrve3:= vlr.v3; 
//    adrve4:= vlr.v4; 
//    temp:= adrve1.n; 
//    fac:= *(tfl{++} inc(tfl); ); 
//    
//    if vlr.flag and R_NOPUNOFLIP
//    then
//    ; 
//    else
//    if contrpuntnormr(vlr.n,temp)
//    then
//    fac:= -fac; *(temp{++} inc(temp); )+=fac*vlr.n[0]; 
//    *(temp{++} inc(temp); )+=fac*vlr.n[1]; 
//    *(temp)+=fac*vlr.n[2]; 
//    temp:= adrve2.n; 
//    fac:= *(tfl{++} inc(tfl); ); 
//    if vlr.flag and R_NOPUNOFLIP
//    then
//    ; 
//    else
//    if contrpuntnormr(vlr.n,temp)
//    then
//    fac:= -fac; *(temp{++} inc(temp); )+=fac*vlr.n[0]; 
//    *(temp{++} inc(temp); )+=fac*vlr.n[1]; 
//    *(temp)+=fac*vlr.n[2]; 
//    temp:= adrve3.n; 
//    fac:= *(tfl{++} inc(tfl); ); 
//    if vlr.flag and R_NOPUNOFLIP
//    then
//    ; 
//    else
//    if contrpuntnormr(vlr.n,temp)
//    then
//    fac:= -fac; *(temp{++} inc(temp); )+=fac*vlr.n[0]; 
//    *(temp{++} inc(temp); )+=fac*vlr.n[1]; 
//    *(temp)+=fac*vlr.n[2]; 
//    
//    if adrve4<>0{nil} {<= !!!9} 
//    then
//    begin 
//      temp:= adrve4.n; 
//      fac:= *(tfl{++} inc(tfl); ); 
//      if vlr.flag and R_NOPUNOFLIP
//      then
//      ; 
//      else
//      if contrpuntnormr(vlr.n,temp)
//      then
//      fac:= -fac; *(temp{++} inc(temp); )+=fac*vlr.n[0]; 
//      *(temp{++} inc(temp); )+=fac*vlr.n[1]; 
//      *(temp)+=fac*vlr.n[2]; 
//    end;
//  end;
//  
//  (* normaliseren puntnormalen *)
//  for{while} a:=startvert to Pred(R.totvert) { a++}
//  do
//  begin 
//    ver:= addvert(a); 
//    Normalise(ver.n); 
//    
//  end;
//  
//  (* puntnormaal omklap-vlaggen voor bij shade *)
//  for{while} a:=startvlak to Pred(R.totvlak) { a++}
//  do
//  begin 
//    vlr:= addvlak(a); 
//    if (vlr.flag and R_NOPUNOFLIP)=0
//    then
//    begin 
//      adrve1:= vlr.v1; 
//      adrve2:= vlr.v2; 
//      adrve3:= vlr.v3; 
//      adrve4:= vlr.v4; 
//      vlr.puno:= 0; 
//      fac:= vlr.n[0]*adrve1.n[0]+vlr.n[1]*adrve1.n[1]+vlr.n[2]*adrve1.n[2]; 
//      if fac<0.0
//      then
//      vlr.puno:= 1; 
//      fac:= vlr.n[0]*adrve2.n[0]+vlr.n[1]*adrve2.n[1]+vlr.n[2]*adrve2.n[2]; 
//      if fac<0.0
//      then
//      vlr.puno:= vlr.puno + (2); 
//      fac:= vlr.n[0]*adrve3.n[0]+vlr.n[1]*adrve3.n[1]+vlr.n[2]*adrve3.n[2]; 
//      if fac<0.0
//      then
//      vlr.puno:= vlr.puno + (4); 
//      if adrve4<>0{nil} {<= !!!9} 
//      then
//      begin 
//        fac:= vlr.n[0]*adrve4.n[0]+vlr.n[1]*adrve4.n[1]+vlr.n[2]*adrve4.n[2]; 
//        if fac<0.0
//        then
//        vlr.puno:= vlr.puno + (8); 
//      end;
//    end;
//  end;
//  freeN(adrco); 
//  
//end;
//
//(* *********** Auto Smooth *********** *)
//
//type
//ASvert = record
//totface: integer; 
//faces: ListBase; 
//end;
//ASvert = ASvert; 
//
//ASface = record
//next: pASface; 
//prev: pASface; 
//vlr: array [0..Pred(4)] of pVlakRen; 
//nver: array [0..Pred(4)] of pVertRen; 
//end;
//ASface = ASface; 
//
//
//procedure as_addvert(v1: pVertRen;  vlr: pVlakRen); 
//var
//asv: pASvert; 
//asf: pASface; 
//a: integer; 
//begin
//  
//  
//  
//  if v1=0{nil}
//  then
//  exit;
//  if v1.svert=0
//  then
//  begin 
//    v1.svert:= callocN(sizeof(ASvert),'asvert'); 
//    asv:= v1.svert; 
//    asf:= callocN(sizeof(ASface),'asface'); 
//    addtail( and asv.faces,asf); 
//  end;
//  asv:= v1.svert; 
//  asf:= asv.faces.last; 
//  for{while} a:=0 to Pred(4) { a++}
//  do
//  begin 
//    if asf.vlr[a]=0
//    then
//    begin 
//      asf.vlr[a]:= vlr; 
//      inc(asv.totface); 
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//  end;
//  (* new face struct *)
//  if a=4
//  then
//  begin 
//    asf:= callocN(sizeof(ASface),'asface'); 
//    addtail( and asv.faces,asf); 
//    asf.vlr[0]:= vlr; 
//    inc(asv.totface); 
//  end;
//end;
//
//
//procedure as_freevert(ver: pVertRen); 
//var
//asv: pASvert; 
//asf: pASface; 
//a: integer; 
//begin
//  
//  
//  asv:= ver.svert; 
//  freelistN( and asv.faces); 
//  freeN(asv); 
//  ver.svert:= 0{nil}; 
//end;
//
//
//function as_testvertex(vlr: pVlakRen;  ver: pVertRen;  asv: pASvert;  thresh: float): integer; 
//var
//asf: pASface; 
//inp: float; 
//a: integer; 
//begin(* return 1: vertex needs a copy *)
//  
//  
//  
//  if vlr=0
//  then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  asf:= asv.faces.first; 
//  while asf
//  do
//  begin 
//    for{while} a:=0 to Pred(4) { a++}
//    do
//    begin 
//      if asf.vlr[a])and(asf.vlr[a]<>vlr
//      then
//      begin 
//        inp:= fabs(vlr.n[0]*asf.vlr[a].n[0]+vlr.n[1]*asf.vlr[a].n[1]+vlr.n[2]*asf.vlr[a].n[2]); 
//        if inp<thresh
//        then
//        begin
//          result:= 1; 
//          exit;
//        end;
//      end;
//    end;
//    asf:= asf.next; 
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//
//function as_findvertex(vlr: pVlakRen;  ver: pVertRen;  asv: pASvert;  thresh: float): pVertRen; 
//var
//asf: pASface; 
//inp: float; 
//a: integer; 
//begin(* return when new vertex already was made *)
//  
//  
//  
//  asf:= asv.faces.first; 
//  while asf
//  do
//  begin 
//    for{while} a:=0 to Pred(4) { a++}
//    do
//    begin 
//      if asf.vlr[a])and(asf.vlr[a]<>vlr
//      then
//      begin 
//        (* this face already made a copy for this vertex! *)
//        if asf.nver[a]<>0{nil} {<= !!!9} 
//        then
//        begin 
//          inp:= fabs(vlr.n[0]*asf.vlr[a].n[0]+vlr.n[1]*asf.vlr[a].n[1]+vlr.n[2]*asf.vlr[a].n[2]); 
//          if inp>=thresh
//          then
//          begin 
//            begin
//              result:= asf.nver[a]; 
//              exit;
//            end;
//          end;
//        end;
//      end;
//    end;
//    asf:= asf.next; 
//  end;
//  begin
//    result:= 0{nil}; 
//    exit;
//  end;
//end;
//
//
//procedure autosmooth(startvert: integer;  startvlak: integer;  degr: integer); 
//var
//asv: pASvert; 
//asf: pASface; 
//ver: pVertRen; 
//v1: pVertRen; 
//vlr: pVlakRen; 
//vertbase: ListBase;
// 
//thresh: float; 
//a: integer; 
//b: integer; 
//totvert: integer; 
//begin
//  
//  
//  
//  
//  
//  vertbase:=(0,0); 
//  
//  
//  
//  
//  thresh:= fcos(M_PI*({!!!a type cast? =>} {float(}degr) div 180.0); (* initialize *)
//  for{while} a:=startvert to Pred(R.totvert) { a++}
//  do
//  begin 
//    ver:= addvert(a); 
//    ver.svert:= 0; 
//  end;
//  (* step one: construct listbase of all vertices and pointers to faces *)
//  for{while} a:=startvlak to Pred(R.totvlak) { a++}
//  do
//  begin 
//    vlr:= addvlak(a); 
//    as_addvert(vlr.v1,vlr); 
//    as_addvert(vlr.v2,vlr); 
//    as_addvert(vlr.v3,vlr); 
//    as_addvert(vlr.v4,vlr); 
//  end;
//  totvert:= R.totvert; (* we now test all vertices, when faces have a normal too much different: they get a new vertex *)
//  for{while} a:=startvert to Pred(totvert) { a++}
//  do
//  begin 
//    ver:= addvert(a); 
//    asv:= ver.svert; 
//    if asv)and(asv.totface>1
//    then
//    begin 
//      asf:= asv.faces.first; 
//      while asf
//      do
//      begin 
//        for{while} b:=0 to Pred(4) { b++}
//        do
//        begin 
//          (* is there a reason to make a new vertex? *)
//          vlr:= asf.vlr[b]; 
//          if as_testvertex(vlr,ver,asv,thresh)
//          then
//          begin 
//            (* already made a new vertex within threshold? *)
//            v1:= as_findvertex(vlr,ver,asv,thresh); 
//            if v1=0
//            then
//            begin 
//              (* make a new vertex *)
//              v1:= addvert(R.totvert{++} inc(R.totvert); ); {*}v1^:=*ver; 
//              v1.svert:= 0; 
//            end;
//            asf.nver[b]:= v1; 
//            if vlr.v1=ver
//            then
//            vlr.v1:= v1; 
//            if vlr.v2=ver
//            then
//            vlr.v2:= v1; 
//            if vlr.v3=ver
//            then
//            vlr.v3:= v1; 
//            if vlr.v4=ver
//            then
//            vlr.v4:= v1; 
//          end;
//        end;
//        asf:= asf.next; 
//      end;
//    end;
//  end;
//  (* free *)
//  for{while} a:=startvert to Pred(R.totvert) { a++}
//  do
//  begin 
//    ver:= addvert(a); 
//    if ver.svert<>0{nil} {<= !!!9} 
//    then
//    as_freevert(ver); 
//  end;
//end;
//
//
//(* ************************************ *)
//
//
//procedure make_orco_s_mesh(ob: pObject); 
//var
//me: pMesh; 
//lbo: ListBase; 
//lb: ListBase; 
//dl: pDispList; 
//fp: pfloat; 
//a: integer; 
//tot: integer; 
//begin
//  {workob: Object; }{<= !!!5 external variable}
//  
//  
//  
//  
//  
//  
//  
//  clear_workob(); 
//  me:= ob.data; 
//  workob.data:= me; 
//  workob.type:= OB_MESH; (* if there's a key, set the first one *)
//  if me.key)and(me.texcomesh=0
//  then
//  begin 
//    showkeypos(me.key,me.key.refkey); 
//  end;
//  lbo:= me.disp; 
//  me.disp.first:= me.disp.last:=0; 
//  makeDispList(@workob); 
//  
//  (* make the s-mesh *)
//  do_ob_key(ob); 
//  (* restore the original mesh *)
//  freedisplist(@workob.disp); 
//  
//  lb:= me.disp; 
//  me.disp:= lbo; 
//  tot:= 0; 
//  dl:= lb.first; 
//  while dl
//  do
//  begin 
//    tot:= tot + (dl.parts*dl.nr); 
//    dl:= dl.next; 
//  end;
//  fp:= me.orco:=mallocN(tot*3*sizeof(float),'smesh orco'); 
//  dl:= lb.first; 
//  while dl
//  do
//  begin 
//    memcpy(fp,dl.verts,dl.parts*dl.nr*3*sizeof(float)); 
//    fp:= fp + (3*dl.parts*dl.nr); 
//    dl:= dl.next; 
//  end;
//  fp:= me.orco; 
//  while{for} 
//  a:= 0; 
//  {to} a<tot
//  {;}
//  inc(a); 
//  fp:= fp + (3); 
//  do
//  begin 
//    fp[0]:= (fp[0]-me.loc[0]) div me.size[0]; 
//    fp[1]:= (fp[1]-me.loc[1]) div me.size[1]; 
//    fp[2]:= (fp[2]-me.loc[2]) div me.size[2]; 
//  end;
//  freedisplist(@lb); 
//end;
//
//
//
//
//procedure init_render_s_mesh(ob: pObject); 
//var
//me: pMesh; 
//dl: pDispList; 
//vlr: pVlakRen; 
//ma: pMaterial; 
//matar: array [0..Pred(32)] of pMaterial; 
//ver: pVertRen; 
//v1: pVertRen; 
//v2: pVertRen; 
//v3: pVertRen; 
//v4: pVertRen; 
//xn: float; 
//yn: float; 
//zn: float; 
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//imat: array [0..Pred(3),0..Pred(3)] of float; 
//data: pfloat; 
//nors: pfloat; 
//orco: pfloat;
// 
//n1: array [0..Pred(3)] of float; 
//flen: float; 
//a: integer; 
//b: integer; 
//need_orco: integer;
// 
//startvlak: integer; 
//startvert: integer; 
//p1: integer; 
//p2: integer; 
//p3: integer; 
//p4: integer; 
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
//  orco:=0; 
//  
//  
//  
//  
//  need_orco:=0; 
//  
//  
//  
//  
//  
//  me:= ob.data; 
//  Mat4MulMat4(mat,ob.obmat,R.viewmat); 
//  
//  Mat4Invert(ob.imat,mat); 
//  Mat3CpyMat4(imat,ob.imat); 
//  bzero(matar,4*32); 
//  
//  (* material array *)
//  matar[0]:=  and defmaterial; 
//  for{while} a:=0 to Pred(ob.totcol) { a++}
//  do
//  begin 
//    matar[a]:= give_render_material(ob,a+1); 
//    if matar[a]=0
//    then
//    matar[a]:=  and defmaterial; 
//    if matar[a].ren.texco and TEXCO_ORCO
//    then
//    begin 
//      need_orco:= 1; 
//    end;
//  end;
//  dl:= me.disp.first; 
//  if dl=0
//  then
//  makeDispList(ob); 
//  dl:= me.disp.first; 
//  if dl=0
//  then
//  exit;
//  if need_orco<>0{nil} {<= !!!9} 
//  then
//  begin 
//    make_orco_s_mesh(ob); 
//    orco:= me.orco; 
//  end;
//  
//  while dl
//  do
//  begin 
//    if dl.type=DL_SURF
//    then
//    begin 
//      startvert:= R.totvert; 
//      a:= dl.nr*dl.parts; 
//      data:= dl.verts; 
//      nors:= dl.nors; 
//      while a{--} dec(a); 
//      do
//      begin 
//        ver:= addvert(R.totvert{++} inc(R.totvert); ); 
//        VECCOPY(ver.co,data); 
//        if orco<>0{nil} {<= !!!9} 
//        then
//        begin 
//          ver.orco:= orco; 
//          orco:= orco + (3); 
//        end;
//        else
//        begin 
//          ver.orco:= data; 
//        end;
//        Mat4MulVecfl(mat,ver.co); 
//        xn:= nors[0]; 
//        yn:= nors[1]; 
//        zn:= nors[2]; 
//        ver.n[0]:= imat[0][0]*xn+imat[0][1]*yn+imat[0][2]*zn; 
//        ver.n[1]:= imat[1][0]*xn+imat[1][1]*yn+imat[1][2]*zn; 
//        ver.n[2]:= imat[2][0]*xn+imat[2][1]*yn+imat[2][2]*zn; 
//        Normalise(ver.n); 
//        (* transpose ! *)
//        data:= data + (3); 
//        nors:= nors + (3); 
//      end;
//      startvlak:= R.totvlak; 
//      
//      for{while} a:=0 to Pred(dl.parts) { a++}
//      do
//      begin 
//        
//        DL_SURFINDEX(dl.flag and 1,dl.flag and 2,dl.nr,dl.parts); 
//        p1:= p1 + (startvert); 
//        p2:= p2 + (startvert); 
//        p3:= p3 + (startvert); 
//        p4:= p4 + (startvert); 
//        for{while}  to Pred(dl.nr) { b++}
//        do
//        begin 
//          v1:= addvert(p1); 
//          v2:= addvert(p2); 
//          v3:= addvert(p3); 
//          v4:= addvert(p4); 
//          flen:= CalcNormFloat4(v1.co,v3.co,v4.co,v2.co,n1); 
//          if flen<>0.0
//          then
//          begin 
//            vlr:= addvlak(R.totvlak{++} inc(R.totvlak); ); 
//            vlr.v1:= v1; 
//            vlr.v2:= v3; 
//            vlr.v3:= v4; 
//            vlr.v4:= v2; 
//            VECCOPY(vlr.n,n1); 
//            vlr.len:= flen; 
//            vlr.lay:= ob.lay; 
//            vlr.mat:= matar[dl.col]; 
//            vlr.ec:= ME_V1V2+ME_V2V3; 
//            vlr.flag:= ME_SMOOTH; 
//            vlr.puno:= 0; 
//            
//            (* vlr->flag |= R_NOPUNOFLIP; *)
//          end;
//          p4:= p3; 
//          inc(p3); 
//          p2:= p1; 
//          inc(p1); 
//          
//        end;
//      end;
//    end;
//    
//    dl:= dl.next; 
//  end;
//end;
//
//
//procedure init_render_mesh(ob: pObject); 
//var
//mface: pMFace; 
//mvert: pMVert; 
//me: pMesh; 
//vlr: pVlakRen; 
//vlr1: pVlakRen; 
//ver: pVertRen; 
//ma: pMaterial; 
//ms: pMSticky; 
//paf: pPartEff; 
//dl: pDispList; 
//vertcol: puint; 
//len: float; 
//xn: float; 
//yn: float; 
//zn: float; 
//nor: array [0..Pred(3)] of float; 
//imat: array [0..Pred(3),0..Pred(3)] of float; 
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//extverts: pfloat;
// 
//orco: pfloat; 
//v1: pfloat; 
//v2: pfloat; 
//v3: pfloat; 
//vec: array [0..Pred(3)] of float; 
//a: integer; 
//a1: integer; 
//ok: integer; 
//do_puno: integer; 
//need_orco: integer;
// 
//totvlako: integer; 
//totverto: integer; 
//vertofs: integer; 
//start: integer; 
//end: integer; 
//flipnorm: integer; 
//warning: integer;
// 
//eff: pEffect;
// 
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
//  extverts:=0; 
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  need_orco:=0; 
//  
//  
//  
//  
//  
//  
//  warning:=0; 
//  me:= ob.data; 
//  if me.flag and ME_SMESH
//  then
//  begin 
//    init_render_s_mesh(ob); 
//    exit;
//  end;
//  
//  if paf:=give_parteff(ob)
//  then
//  begin 
//    if paf.flag and PAF_STATIC
//    then
//    render_static_particle_system(ob,paf); 
//    else
//    render_particle_system(ob,paf); 
//    exit;
//  end;
//  do_puno:= object_deform(ob); 
//  Mat4MulMat4(mat,ob.obmat,R.viewmat); 
//  
//  (* object_deform changes imat *)
//  
//  Mat4Invert(ob.imat,mat); 
//  Mat3CpyMat4(imat,ob.imat); 
//  if me.totvert=0
//  then
//  exit;
//  mvert:= me.mvert; 
//  dl:= find_displist(@ob.disp,DL_VERTS); 
//  if dl<>0{nil} {<= !!!9} 
//  then
//  extverts:= dl.verts; 
//  totvlako:= R.totvlak; 
//  totverto:= R.totvert; 
//  
//  if me.key<>0{nil} {<= !!!9} 
//  then
//  do_puno:= 1; 
//  if ob.effect.first<>0{nil} {<= !!!9} 
//  then
//  begin 
//    eff:=ob.effect.first; 
//    while eff
//    do
//    begin 
//      if eff.type=EFF_WAVE
//      then
//      do_puno:= 1; 
//      eff:= eff.next; 
//    end;
//  end;
//  
//  if me.orco=0
//  then
//  begin 
//    need_orco:= 0; 
//    for{while} a:=1 to ob.totcol { a++}
//    do
//    begin 
//      ma:= give_render_material(ob,a); 
//      if ma<>0{nil} {<= !!!9} 
//      then
//      begin 
//        if ma.ren.texco and TEXCO_ORCO
//        then
//        begin 
//          need_orco:= 1; 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//      end;
//    end;
//    if need_orco<>0{nil} {<= !!!9} 
//    then
//    begin 
//      make_orco_mesh(me); 
//    end;
//  end;
//  orco:= me.orco; 
//  ms:= me.msticky; 
//  vertcol:= {!!!a type cast? =>} {puint(}me.mcol; 
//  ma:= give_render_material(ob,1); 
//  if ma=0
//  then
//  ma:=  and defmaterial; 
//  
//  if ma.mode and MA_HALO
//  then
//  begin 
//    make_render_halos(ob,me,ma,extverts); 
//  end;
//  else
//  begin 
//    
//    while{for} 
//    a:= 0; 
//    {to} a<me.totvert
//    {;}
//    inc(a); 
//    inc(mvert); 
//    do
//    begin 
//      
//      ver:= addvert(R.totvert{++} inc(R.totvert); ); 
//      if extverts<>0{nil} {<= !!!9} 
//      then
//      begin 
//        VECCOPY(ver.co,extverts); 
//        extverts:= extverts + (3); 
//      end;
//      else
//      begin 
//        VECCOPY(ver.co,mvert.co); 
//      end;
//      Mat4MulVecfl(mat,ver.co); 
//      xn:= mvert.no[0]; 
//      yn:= mvert.no[1]; 
//      zn:= mvert.no[2]; 
//      if do_puno=0
//      then
//      begin 
//        (* transpose ! *)
//        ver.n[0]:= imat[0][0]*xn+imat[0][1]*yn+imat[0][2]*zn; 
//        ver.n[1]:= imat[1][0]*xn+imat[1][1]*yn+imat[1][2]*zn; 
//        ver.n[2]:= imat[2][0]*xn+imat[2][1]*yn+imat[2][2]*zn; 
//        Normalise(ver.n); 
//      end;
//      if orco<>0{nil} {<= !!!9} 
//      then
//      begin 
//        ver.orco:= orco; 
//        orco:= orco + (3); 
//      end;
//      if ms<>0{nil} {<= !!!9} 
//      then
//      begin 
//        ver.sticky:= {!!!a type cast? =>} {pfloat(}ms; 
//        inc(ms); 
//      end;
//    end;
//    flipnorm:= -1; 
//    vertofs:= R.totvert-me.totvert; (* nog doen bij keys: de juiste lokale textu coordinaat *)
//    
//    (* Testen of er een flip in de matrix zit: dan vlaknormaal ook flippen *)
//    
//    (* vlakken in volgorde colblocks *)
//    while{for} 
//    a1:= 0; 
//    {to} (a1<ob.totcol)or((a1=0)and(ob.totcol=0))
//    {;}
//    inc(a1); 
//    do
//    begin 
//      
//      ma:= give_render_material(ob,a1+1); 
//      if ma=0
//      then
//      ma:=  and defmaterial; 
//      ok:= 1; 
//      (* testen op 100% transparant *)
//      if ma.alpha=0.0)and(ma.spectra=0.0
//      then
//      begin 
//        ok:= 0; (* texture op transp? *)
//        for{while} a:=0 to Pred(8) { a++}
//        do
//        begin 
//          if ma.mtex[a])and(ma.mtex[a].tex
//          then
//          begin 
//            if ma.mtex[a].mapto and MAP_ALPHA
//            then
//            ok:= 1; 
//          end;
//        end;
//      end;
//      
//      if ok<>0{nil} {<= !!!9} 
//      then
//      begin 
//        
//        start:= 0; 
//      end:= me.totface; 
//      set_buildvars(ob,@start, and end); 
//      mvert:= me.mvert; 
//      mface:= me.mface; 
//      mface:= mface + (start); 
//      while{for} 
//      a:= start; 
//      {to} a<end
//      {;}
//      inc(a); 
//      inc(mface); 
//      do
//      begin 
//        
//        if mface.mat_nr=a1
//        then
//        begin 
//          
//          if mface.v3<>0{nil} {<= !!!9} 
//          then
//          begin 
//            
//            vlr:= addvlak(R.totvlak{++} inc(R.totvlak); ); 
//            vlr.v1:= addvert(vertofs+mface.v1); 
//            vlr.v2:= addvert(vertofs+mface.v2); 
//            vlr.v3:= addvert(vertofs+mface.v3); 
//            if mface.v4<>0{nil} {<= !!!9} 
//            then
//            vlr.v4:= addvert(vertofs+mface.v4); 
//            else
//            vlr.v4:= 0; 
//            (* rendernormalen zijn omgekeerd *)
//            if vlr.v4<>0{nil} {<= !!!9} 
//            then
//            vlr.len:= CalcNormFloat4(vlr.v4.co,vlr.v3.co,vlr.v2.co,vlr.v1.co,vlr.n); 
//            else
//            vlr.len:= CalcNormFloat(vlr.v3.co,vlr.v2.co,vlr.v1.co,vlr.n); 
//            vlr.mface:= mface; 
//            vlr.mat:= ma; 
//            vlr.puno:= mface.puno; 
//            vlr.flag:= mface.flag; 
//            if me.flag and ME_NOPUNOFLIP
//            then
//            begin 
//              vlr.flag:= vlr.flag or (R_NOPUNOFLIP); 
//              vlr.puno:= 15; 
//            end;
//            vlr.ec:= mface.edcode; 
//            vlr.lay:= ob.lay; 
//            if R.r.scemode and R_HOTSPOT
//            then
//            begin 
//              if ob.infoflag and OB_INFO_HOTSPOT
//              then
//              begin 
//                
//                *((char* )@vlr.lay):=ob.infostart; 
//                vlr.flag:= vlr.flag or (R_CMAPCODE); 
//                if ob.infoend=0
//                then
//                vlr.flag:= vlr.flag or (R_VISIBLE); 
//              end;
//            end;
//            
//            if vlr.len=0
//            then
//            dec(R.totvlak); 
//            else
//            begin 
//              if flipnorm=-1
//              then
//              begin 
//                (* per object 1 x testen *)
//                
//                v1:= (me.mvert+mface.v1).co; 
//                v2:= (me.mvert+mface.v2).co; 
//                v3:= (me.mvert+mface.v3).co; 
//                CalcNormFloat(v1,v2,v3,nor); 
//                vec[0]:= imat[0][0]*nor[0]+imat[0][1]*nor[1]+imat[0][2]*nor[2]; 
//                vec[1]:= imat[1][0]*nor[0]+imat[1][1]*nor[1]+imat[1][2]*nor[2]; 
//                vec[2]:= imat[2][0]*nor[0]+imat[2][1]*nor[1]+imat[2][2]*nor[2]; 
//                xn:= vec[0]*vlr.n[0]+vec[1]*vlr.n[1]+vec[2]*vlr.n[2]; 
//                if xn<0.0
//                then
//                flipnorm:= 1; 
//                else
//                flipnorm:= 0; 
//              end;
//              if flipnorm<>0{nil} {<= !!!9} 
//              then
//              begin 
//                vlr.n[0]:= -vlr.n[0]; 
//                vlr.n[1]:= -vlr.n[1]; 
//                vlr.n[2]:= -vlr.n[2]; 
//              end;
//              
//              if vertcol<>0{nil} {<= !!!9} 
//              then
//              vlr.vcol:= vertcol+4*a; 
//              else
//              vlr.vcol:= 0; 
//              (* testen of een vierhoek als driehoek gerenderd moet *)
//              if vlr.v4<>0{nil} {<= !!!9} 
//              then
//              begin 
//                
//                if ma.mode and MA_WIRE
//                then
//                ; 
//                else
//                begin 
//                  CalcNormFloat(vlr.v4.co,vlr.v3.co,vlr.v1.co,nor); 
//                  if flipnorm<>0{nil} {<= !!!9} 
//                  then
//                  begin 
//                    nor[0]:= -nor[0]; 
//                    nor[1]:= -nor[1]; 
//                    nor[2]:= -nor[2]; 
//                  end;
//                  xn:= INPR(nor,vlr.n); 
//                  if xn<0.9990
//                  then
//                  begin 
//                    vlr1:= addvlak(R.totvlak{++} inc(R.totvlak); ); {*}vlr1^:=*vlr; 
//                    VECCOPY(vlr1.n,nor); 
//                    vlr1.v2:= vlr.v3; 
//                    vlr1.v3:= vlr.v4; 
//                    vlr.v4:= vlr1.v4:=0; 
//                    vlr1.puno:= 0; 
//                    if vlr.puno and ME_FLIPV1
//                    then
//                    vlr1.puno:= vlr1.puno or (ME_FLIPV1); 
//                    if vlr.puno and ME_FLIPV3
//                    then
//                    vlr1.puno:= vlr1.puno or (ME_FLIPV2); 
//                    if vlr.puno and ME_FLIPV4
//                    then
//                    vlr1.puno:= vlr1.puno or (ME_FLIPV3); 
//                    if vertcol<>0{nil} {<= !!!9} 
//                    then
//                    warning:= 1; 
//                  end;
//                end;
//              end;
//            end;
//          end;
//          else
//          if mface.v2)and((ma.mode and MA_WIRE)
//          then
//          begin 
//            vlr:= addvlak(R.totvlak{++} inc(R.totvlak); ); 
//            vlr.v1:= addvert(vertofs+mface.v1); 
//            vlr.v2:= addvert(vertofs+mface.v2); 
//            vlr.v3:= vlr.v2; 
//            vlr.v4:= 0; 
//            vlr.n[0]:= vlr.n[1]:=vlr.n[2]:=0.0; 
//            vlr.mface:= mface; 
//            vlr.mat:= ma; 
//            vlr.puno:= mface.puno; 
//            vlr.flag:= mface.flag; 
//            vlr.ec:= ME_V1V2; 
//            vlr.lay:= ob.lay; 
//            
//            
//          end;
//        end;
//      end;
//    end;
//  end;
//end;
//if me.flag and ME_AUTOSMOOTH
//then
//begin 
//  autosmooth(totverto,totvlako,me.smoothresh); 
//  do_puno:= 1; 
//end;
//if do_puno<>0{nil} {<= !!!9} 
//then
//normalenrender(totverto,totvlako); 
//
//if warning<>0{nil} {<= !!!9} 
//then
//printf('WARNING: ob %s with vertcol, non-flat squares\n',ob.id.name+2); 
//end;
//
//
//
//procedure init_render_mball(ob: pObject); 
//var
//mb: pMetaBall; 
//ml: pMetaElem; 
//dl: pDispList; 
//dlo: pDispList; 
//ver: pVertRen; 
//adrve1: pVertRen; 
//adrve2: pVertRen; 
//adrve3: pVertRen; 
//adrve4: pVertRen; 
//vlr: pVlakRen; 
//vlr1: pVlakRen; 
//ma: pMaterial; 
//data: pfloat; 
//nors: pfloat; 
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//imat: array [0..Pred(3),0..Pred(3)] of float; 
//vec: array [0..Pred(3)] of float; 
//fac: float; 
//xn: float; 
//yn: float; 
//zn: float; 
//a: integer; 
//need_orco: integer; 
//startvlak: integer; 
//startvert: integer; 
//index: pinteger; 
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
//  
//  
//  
//  
//  
//  
//  
//  Mat4MulMat4(mat,ob.obmat,R.viewmat); 
//  Mat4Invert(ob.imat,mat); 
//  Mat3CpyMat4(imat,ob.imat); 
//  
//  if has_id_number({!!!a type cast? =>} {pID(}ob)
//  then
//  exit;
//  
//  ma:= give_render_material(ob,1); 
//  if ma=0
//  then
//  ma:=  and defmaterial; 
//  need_orco:= 0; 
//  if ma.ren.texco and TEXCO_ORCO
//  then
//  begin 
//    need_orco:= 1; 
//  end;
//  dlo:= ob.disp.first; 
//  if dlo<>0{nil} {<= !!!9} 
//  then
//  remlink(@ob.disp,dlo); 
//  makeDispList(ob); 
//  
//  dl:= ob.disp.first; 
//  if dl=0
//  then
//  exit;
//  
//  startvert:= R.totvert; 
//  data:= dl.verts; 
//  nors:= dl.nors; 
//  while{for} 
//  a:= 0; 
//  {to} a<dl.nr
//  {;}
//  inc(a); 
//  data:= data + (3); 
//  nors:= nors + (3); 
//  do
//  begin 
//    
//    ver:= addvert(R.totvert{++} inc(R.totvert); ); 
//    VECCOPY(ver.co,data); 
//    Mat4MulVecfl(mat,ver.co); 
//    xn:= -nors[0]; 
//    yn:= -nors[1]; 
//    zn:= -nors[2]; 
//    ver.n[0]:= imat[0][0]*xn+imat[0][1]*yn+imat[0][2]*zn; 
//    ver.n[1]:= imat[1][0]*xn+imat[1][1]*yn+imat[1][2]*zn; 
//    ver.n[2]:= imat[2][0]*xn+imat[2][1]*yn+imat[2][2]*zn; 
//    Normalise(ver.n); 
//    
//    (* rendernormalen zijn omgekeerd *)
//    
//    (* transpose ! *)
//    
//    if need_orco<>0{nil} {<= !!!9} 
//    then
//    ver.orco:= data; 
//  end;
//  startvlak:= R.totvlak; 
//  index:= dl.index; 
//  while{for} 
//  a:= 0; 
//  {to} a<dl.parts
//  {;}
//  inc(a); 
//  index:= index + (4); 
//  do
//  begin 
//    
//    vlr:= addvlak(R.totvlak{++} inc(R.totvlak); ); 
//    vlr.v1:= addvert(startvert+index[0]); 
//    vlr.v2:= addvert(startvert+index[1]); 
//    vlr.v3:= addvert(startvert+index[2]); 
//    vlr.v4:= 0; 
//    vlr.len:= CalcNormFloat(vlr.v3.co,vlr.v2.co,vlr.v1.co,vlr.n); 
//    vlr.mface:= 0; 
//    vlr.mat:= ma; 
//    vlr.puno:= 0; 
//    vlr.flag:= ME_SMOOTH+R_NOPUNOFLIP; 
//    vlr.ec:= 0; 
//    vlr.lay:= ob.lay; 
//    (* rendernormalen zijn omgekeerd *)
//    
//    
//    (* mball -helaas- altijd driehoeken maken omdat vierhoeken erg onregelmatig zijn *)
//    if index[3]<>0{nil} {<= !!!9} 
//    then
//    begin 
//      vlr1:= addvlak(R.totvlak{++} inc(R.totvlak); ); {*}vlr1^:=*vlr; 
//      vlr1.v2:= vlr1.v3; 
//      vlr1.v3:= addvert(startvert+index[3]); 
//      vlr.len:= CalcNormFloat(vlr1.v3.co,vlr1.v2.co,vlr1.v1.co,vlr1.n); 
//    end;
//  end;
//  
//  if need_orco<>0{nil} {<= !!!9} 
//  then
//  begin 
//    (* displist bewaren en scalen *)
//    make_orco_mball(ob); 
//    if dlo<>0{nil} {<= !!!9} 
//    then
//    addhead(@ob.disp,dlo); 
//  end;
//  
//  else
//  begin 
//    freedisplist(@ob.disp); 
//    if dlo<>0{nil} {<= !!!9} 
//    then
//    addtail(@ob.disp,dlo); 
//  end;
//end;
//
//(* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ *)
//(* ook homoco's *)
//
//procedure setzbufvlaggen(
//
//procedure projectfunc{!!!3 unknown typedef}; 
//); 
//var
//vlr: pVlakRen; 
//ver: pVertRen; 
//har: pHaloRen; 
//qs: pQStrip; 
//vs: pVertStrip; 
//abs4: float; 
//zn: float; 
//vec: array [0..Pred(3)] of float; 
//si: float; 
//co: float; 
//hoco: array [0..Pred(4)] of float; 
//a: integer; 
//doit: integer; 
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
//  si:= fsin(panophi); 
//  co:= fcos(panophi); 
//  for{while} a:=0 to Pred(R.totvert) {a++}
//  do
//  begin 
//    if (a and 255)=0
//    then
//    ver:= R.blove[a shr 8]; 
//    else
//    inc(ver); 
//    if R.r.mode and R_PANORAMA
//    then
//    begin 
//      vec[0]:= co*ver.co[0]+si*ver.co[2]; 
//      vec[1]:= ver.co[1]; 
//      vec[2]:= -si*ver.co[0]+co*ver.co[2]; 
//    end;
//    else
//    begin 
//      VECCOPY(vec,ver.co); 
//    end;
//    projectfunc(vec,ver.ho); 
//    ver.clip:= testclip(ver.ho); 
//    
//  end;
//  
//  for{while} a:=0 to Pred(R.tothalo) { a++}
//  do
//  begin 
//    if (a and 255)=0
//    then
//    har:= R.bloha[a shr 8]; 
//    else
//    inc(har); 
//    if R.r.mode and R_PANORAMA
//    then
//    begin 
//      vec[0]:= co*har.co[0]+si*har.co[2]; 
//      vec[1]:= har.co[1]; 
//      vec[2]:= -si*har.co[0]+co*har.co[2]; 
//    end;
//    else
//    begin 
//      VECCOPY(vec,har.co); 
//    end;
//    projectfunc(vec,hoco); 
//    
//    hoco[3]:= hoco[3] * (2.0); 
//    
//    if panotestclip(hoco)
//    then
//    begin 
//      har.miny:= har.maxy:=-10000; 
//      (* de render clipt 'm weg *)
//    end;
//    else
//    begin 
//      zn:= hoco[3] div 2.0; 
//      har.xs:= 0.5*R.rectx*(1.0+hoco[0] div zn); 
//      har.ys:= 0.5*R.recty*(1.0+hoco[1] div zn); 
//      har.zs:= $7FFFFF*(1.0+hoco[2] div zn); 
//      vec[0]:= vec[0] + (har.hasize); 
//      projectfunc(vec,hoco); 
//      
//      vec[0]:= vec[0] - (har.hasize); 
//      zn:= hoco[3]; 
//      har.rad:= fabs(har.xs-0.5*R.rectx*(1.0+hoco[0] div zn)); 
//      (* deze clip is eigenlijk niet OK *)
//      if har.type and HA_ONLYSKY
//      then
//      begin 
//        if har.rad>3.0
//        then
//        har.rad:= 3.0; 
//      end;
//      har.radsq:= har.rad*har.rad; 
//      har.miny:= har.ys-har.rad div R.ycor; 
//      har.maxy:= har.ys+har.rad div R.ycor; 
//      vec[2]:= vec[2] - (har.hasize); 
//      projectfunc(vec,hoco); 
//      
//      
//      
//      (* de Zd is bij pano nog steeds verkeerd: zie testfile in blenderbugs/halo+pano.blend *)
//      
//      (* z is negatief, wordt anders geclipt *)
//      zn:= hoco[3]; 
//      zn:= fabs(har.zs-$7FFFFF*(1.0+hoco[2] div zn)); 
//      har.zd:= CLAMPIS(zn,0,INT_MAX); 
//      
//      (* if( har->zs < 2*har->zd) { *)
//      (* PRINT2(d, d, har->zs, har->zd); *)
//      (* har->alfa= har->mat->alpha * ((float)(har->zs))/(float)(2*har->zd); *)
//      (* } *)
//      
//    end;
//  end;
//  
//  
//  (* vlaggen op 0 zetten als eruit geclipt *)
//  for{while} a:=0 to Pred(R.totvlak) { a++}
//  do
//  begin 
//    if (a and 255)=0
//    then
//    vlr:= R.blovl[a shr 8]; 
//    else
//    inc(vlr); 
//    if vlr.flag and R_CMAPCODE
//    then
//    begin 
//      ; 
//      (* niet helemaal nette hotspot patch *)
//    end;
//    else
//    begin 
//      vlr.flag:= vlr.flag or (R_VISIBLE); 
//      if vlr.v4<>0{nil} {<= !!!9} 
//      then
//      begin 
//        if vlr.v1.clip and vlr.v2.clip and vlr.v3.clip and vlr.v4.clip
//        then
//        vlr.flag:= vlr.flag and ( not R_VISIBLE); 
//      end;
//      else
//      if vlr.v1.clip and vlr.v2.clip and vlr.v3.clip
//      then
//      vlr.flag:= vlr.flag and ( not R_VISIBLE); 
//      
//    end;
//  end;
//end;
//(* qs= R.qstrip.first;
// while(qs) {
//     vs= qs->verts;
//     a= qs->pntsu*qs->pntsv;
//     while(a--) {
//  projectfunc(vs->co, vs->ho);
//  vs->clip= testclip(vs->ho);
//  vs++;
//     }
//     qs= qs->next;
// }
// *)
//
//
//
//function verghalo(x1: phalosort;  x2: phalosort): integer; 
//begin
//  if x1.z<x2.z
//  then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  else
//  if x1.z>x2.z
//  then
//  begin
//    result:= -1; 
//    exit;
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//
//type
//halosort = record
//end;
//
//procedure sort_halos; 
//var
//har: pHaloRen; 
//a: integer; 
//begin
//  
//  HaloRen{!!!e unknown token}{*}bloha^; 
//  if R.tothalo=0
//  then
//  exit;
//  
//  (* datablok maken met halopointers, sorteren *)
//  haso:= hablock:=mallocN(sizeof(structhalosort)*R.tothalo,'hablock'); 
//  for{while} a:=0 to Pred(R.tothalo) { a++}
//  do
//  begin 
//    if (a and 255)=0
//    then
//    har:= R.bloha[a shr 8]; 
//    else
//    inc(har); 
//    haso.har:= har; 
//    haso.z:= har.zs; 
//    inc(haso); 
//  end;
//  qsort(hablock,R.tothalo,sizeof(structhalosort),{!!!a type cast? =>} {pinteger(}verghalo); 
//  
//  bloha:= R.bloha; 
//  R.bloha:= (HaloRen** )callocN(sizeof(void* )*(MAXVERT shr 8),'Bloha'); 
//  haso:= hablock; 
//  (* opnieuw samenstellen van R.bloha *)
//  
//  
//  for{while} a:=0 to Pred(R.tothalo) { a++}
//  do
//  begin 
//    har:= addhalo(a); {*}har^:=*(haso.har); 
//    inc(haso); 
//    
//  end;
//  a:= 0; 
//  (* vrijgeven *)
//  while bloha[a]
//  do
//  begin 
//    freeN(bloha[a]); 
//    inc(a); 
//  end;
//  freeN(bloha); 
//  freeN(hablock); 
//end;
//
//
//var {was static}
//alpha: float;
// 
//(* part==0 alles initialiseren *)
//
//
//procedure setpanorot(part: integer); 
//begin
//  {panovco: float; }{<= !!!5 external variable}
//  {panovsi: float; }{<= !!!5 external variable}
//  alpha:=1.0; 
//  (* part==0 alles initialiseren *)
//  
//  if part=0
//  then
//  begin 
//    
//    alpha:= ({!!!a type cast? =>} {float(}R.r.xsch) div R.viewfac; 
//    alpha:= 2.0*fatan(alpha div 2.0); 
//  end;
//  panophi:= -0.5*(R.r.xparts-1)*alpha+part*alpha; 
//  panovsi:= fsin(-panophi); 
//  panovco:= fcos(-panophi); 
//  
//  
//  (* we roteren alles om de y-as met phi graden *)
//  
//  
//  
//end;
//
//(* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ *)
//
//
//procedure make_existing_file(name: pchar); 
//var
//di: array [0..Pred(FILE_MAXDIR)] of char; 
//fi: array [0..Pred(FILE_MAXFILE)] of char; 
//i: integer; 
//len: integer; 
//begin
//  
//  
//  
//  strcpy(di,name); 
//  splitdirstring(di,fi); 
//  
//  (* exist testen *)
//  if exist(di)=0
//  then
//  begin 
//    recurdir(di); 
//  end;
//end;
//
//type
//ImBuf = record
//end;
//
//procedure schrijfplaatje(name: pchar); 
//var
//temprect: puint;
// 
//i: integer; 
//flags: integer; 
//str: array [0..Pred(FILE_MAXDIR+FILE_MAXFILE)] of char; 
//(* Staat RGBA aan? Zo ja: gebruik alphakanaal voor kleur 0 *)
//begin
//  {rgb_to_bw: array [0..] of float; }{<= !!!5 external variable}
//  
//  function alpha_to_col0{!!!3 unknown typedef}: integer; 
//  
//  temprect:=0; 
//  
//  
//  alpha_to_col0(LongBool(0)); 
//  
//  if R.r.planes=32
//  then
//  begin 
//    (* alles met minder dan 50 % alpha -> col 0 *)
//    if R.r.alphamode=R_ALPHAKEY
//    then
//    alpha_to_col0(2); 
//    else
//    (* uitsluitend met 0 alpha -> col 0 *)
//    alpha_to_col0(1); 
//  end;
//  
//  
//  function ELEM{!!!3 unknown typedef}: if; 
//  begin
//    strcpy(str,R.r.ftype); 
//    convertstringcode(str); 
//    ibuf:= loadiffname(str,IB_test); 
//    if ibuf<>0{nil} {<= !!!9} 
//    then
//    begin 
//      ibuf.x:= R.rectx; 
//      ibuf.y:= R.recty; 
//    end;
//    else
//    begin 
//      error('Can'#39't find filetype'); 
//      G.afbreek:= 1; 
//      exit;
//    end;
//    setdither(2); 
//  end
//  
//  if ibuf=0
//  then
//  begin 
//    ibuf:= allocImBuf(R.rectx,R.recty,R.r.planes,0,0); 
//  end;
//  
//  if ibuf<>0{nil} {<= !!!9} 
//  then
//  begin 
//    ibuf.rect:= {!!!a type cast? =>} {puint(}R.rectot; 
//    if R.r.planes=8
//    then
//    cspace(ibuf,rgb_to_bw); 
//    
//    if R.r.imtype=R_IRIS
//    then
//    begin 
//      ibuf.ftype:= IMAGIC; 
//    end;
//    else
//    if R.r.imtype=R_IRIZ
//    then
//    begin 
//      ibuf.ftype:= IMAGIC; 
//      if ibuf.zbuf=0
//      then
//      begin 
//        if R.rectz<>0{nil} {<= !!!9} 
//        then
//        begin 
//          ibuf.zbuf:= {!!!a type cast? =>} {pinteger(}R.rectz; 
//        end;
//        else
//        printf('no zbuf\n'); 
//      end;
//    end;
//    else
//    if R.r.imtype=R_MDEC
//    then
//    begin 
//      ibuf.ftype:= TGA; 
//    end;
//    else
//    if R.r.imtype=R_TARGA
//    then
//    begin 
//      ibuf.ftype:= TGA; 
//    end;
//    else
//    if R.r.imtype=R_RAWTGA
//    then
//    begin 
//      ibuf.ftype:= RAWTGA; 
//    end;
//    else
//    if R.r.imtype=R_HAMX
//    then
//    begin 
//      (* kopie maken *)
//      temprect:= dupallocN(R.rectot); 
//      ibuf.ftype:= AN_hamx; 
//    end;
//    else
//    if R.r.imtype=R_TANX
//    then
//    begin 
//      temprect:= dupallocN(R.rectot); 
//      ibuf.ftype:= AN_tanx; 
//    end;
//    else
//    if ELEM5(R.r.imtype,R_MOVIE,R_AVIRAW,R_AVIJPEG,R_AVIJMF,R_JPEG90)
//    then
//    begin 
//      if R.r.quality<10
//      then
//      R.r.quality:= 90; 
//      if R.r.mode and R_FIELDS
//      then
//      ibuf.ftype:= JPG_VID or R.r.quality; 
//      else
//      ibuf.ftype:= JPG or R.r.quality; 
//    end;
//    make_existing_file(name); 
//    if R.r.imtype=R_CMBB
//    then
//    write_cmbb(ibuf,name); 
//    else
//    if saveiff(ibuf,name,IB_rect or IB_zbuf)=0
//    then
//    begin 
//      perror(name); 
//      G.afbreek:= 1; 
//    end;
//    freeImBuf(ibuf); 
//    
//    
//    
//    function ELEM{!!!3 unknown typedef}: if; 
//    begin
//      freeN(R.rectot); 
//      R.rectot:= temprect; 
//    end;
//  end;
//  else
//  begin 
//    G.afbreek:= 1; 
//  end;
//end;
//
//
//procedure write_image(name: pchar); 
//var
//str: array [0..Pred(256)] of char; 
//begin(* vanuit filesel *)
//  strcpy(str,name); 
//  convertstringcode(str); 
//  
//  if saveover(str)
//  then
//  begin 
//    if testextensie(str,'.blend')
//    then
//    begin 
//      error('Wrong filename'); 
//      exit;
//    end;
//    waitcursor(1); 
//    schrijfplaatje(str); 
//    strcpy(G.ima,name); 
//    waitcursor(0); 
//  end;
//end;
//
//(* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ *)
//
//(* #pragma optimize("", off) *)
//
//procedure setwindowclip(mode: integer;  jmode: integer); 
//var
//cam: pCamera;
// 
//la: pLamp;
// 
//lens: float; 
//fac: float; 
//minx: float; 
//miny: float; 
//maxx: float; 
//maxy: float; 
//xd: float; 
//yd: float; 
//afmx: float; 
//afmy: float; 
//begin(* jmode>=0: alleen jitter doen, anders berekenen  *)
//  (* mode==1 zet persmat en grvec *)
//  {holoofs: float; }{<= !!!5 external variable}(* render.c *)
//  cam:=0; 
//  la:=0; 
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  if G.scene.camera=0
//  then
//  exit;
//  
//  afmx:= R.afmx; 
//  afmy:= R.afmy; 
//  if mode<>0{nil} {<= !!!9} 
//  then
//  begin 
//    
//    if G.scene.camera.type=OB_LAMP
//    then
//    begin 
//      la:= G.scene.camera.data; 
//      lens:= 35.0; (* fac= fcos( PI*((float)(256- la->spsi))/512.0 ); *)
//      
//      (* phi= facos(fac); *)
//      (* lens= 16.0*fac/fsin(phi); *)
//      R{!!!e unknown token}.:=0.1; 
//      R{!!!e unknown token}.:=1000.0; 
//    end;
//    else
//    if G.scene.camera.type=OB_CAMERA
//    then
//    begin 
//      cam:= G.scene.camera.data; 
//      lens:= cam.lens; 
//      R{!!!e unknown token}.:=cam.clipsta; 
//      R{!!!e unknown token}.:=cam.clipend; 
//    end;
//    else
//    begin 
//      lens:= 16.0; 
//    end;
//    
//    if (R.r.xasp*afmx)>=(R.r.yasp*afmy)
//    then
//    begin 
//      R.viewfac:= (afmx*lens) div 16.0; 
//    end;
//    else
//    begin 
//      R.viewfac:= R.ycor*(afmy*lens) div 16.0; 
//    end;
//    if R.r.mode and R_ORTHO
//    then
//    begin 
//      R{!!!e unknown token}.*=100.0; 
//      R.viewfac:= R.viewfac * (100.0); 
//      (* R. niet doen *)
//    end;
//    R.pixsize:= R. div R.viewfac; 
//    
//    
//  end;
//  minx:= R.xstart+.5; 
//  miny:= R.ycor*(R.ystart+.5); 
//  maxx:= R.xend+.4999; 
//  maxy:= R.ycor*(R.yend+.4999); 
//  
//  if R.flag and R_SEC_FIELD
//  then
//  begin 
//    if R.r.mode and R_ODDFIELD
//    then
//    begin 
//      miny:= miny - (.5*R.ycor); 
//      maxy:= maxy - (.5*R.ycor); 
//    end;
//    else
//    begin 
//      miny:= miny + (.5*R.ycor); 
//      maxy:= maxy + (.5*R.ycor); 
//    end;
//  end;
//  xd:= yd:=0.0; 
//  if jmode<>-1
//  then
//  begin 
//    xd:= jit[jmode mod R.osa][0]; 
//    yd:= R.ycor*jit[jmode mod R.osa][1]; 
//    
//  end;
//  
//  if G.special1 and G_HOLO
//  then
//  begin 
//    if G.scene.camera.type=OB_CAMERA
//    then
//    begin 
//      cam:= G.scene.camera.data; 
//      if cam.flag and CAM_HOLO2
//      then
//      begin 
//        if cam.netend=0.0
//        then
//        cam.netend:= EFRA; 
//        fac:= (CFRA-1.0) div (cam.netend)-0.5; 
//        fac:= fac * ((R.rectx)); 
//        fac:= fac * (cam.hololen1); 
//        holoofs:= -fac; 
//        minx:= minx - (fac); 
//        maxx:= maxx - (fac); 
//        
//      end;
//    end;
//  end;
//  minx:= R.pixsize*(minx+xd); 
//  maxx:= R.pixsize*(maxx+xd); 
//  miny:= R.pixsize*(miny+yd); 
//  maxy:= R.pixsize*(maxy+yd); 
//  
//  if R.r.mode and R_ORTHO
//  then
//  begin 
//    (* hier de  &  vermenigvuldigen is voldoende! *)
//    
//    i_window(minx,maxx,miny,maxy,R.,100.0*R.,R.winmat); 
//  end;
//  else
//  i_window(minx,maxx,miny,maxy,R.,R.,R.winmat); 
//end;
//
//(* #pragma optimize("", on) *)
//
//
//(* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ *)
//
//procedure initparts; 
//var
//nr: smallint; 
//xd: smallint; 
//yd: smallint; 
//xpart: smallint; 
//ypart: smallint; 
//xparts: smallint; 
//yparts: smallint; 
//a: smallint; 
//xminb: smallint; 
//xmaxb: smallint; 
//yminb: smallint; 
//ymaxb: smallint; 
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
//  if R.r.mode and R_BORDER
//  then
//  begin 
//    xminb:= R.r.border.xmin*R.rectx; 
//    xmaxb:= R.r.border.xmax*R.rectx; 
//    yminb:= R.r.border.ymin*R.recty; 
//    ymaxb:= R.r.border.ymax*R.recty; 
//    
//    if xminb<0
//    then
//    xminb:= 0; 
//    if xmaxb>R.rectx
//    then
//    xmaxb:= R.rectx; 
//    if yminb<0
//    then
//    yminb:= 0; 
//    if ymaxb>R.recty
//    then
//    ymaxb:= R.recty; 
//  end;
//  else
//  begin 
//    xminb:= yminb:=0; 
//    xmaxb:= R.rectx; 
//    ymaxb:= R.recty; 
//  end;
//  xparts:= R.r.xparts; 
//  yparts:= R.r.yparts; 
//  (* voor border *)
//  
//  for{while} nr:=0 to Pred(xparts*yparts) {nr++}
//  do
//  allparts[nr][0]:= -1; 
//  xpart:= R.rectx div xparts; 
//  ypart:= R.recty div yparts; (* array leegmaken *)
//  
//  
//  (* als border: testen of aantal parts minder kan *)
//  if R.r.mode and R_BORDER
//  then
//  begin 
//    a:= (xmaxb-xminb-1) div xpart+1; (* zoveel parts in border *)
//    if a<xparts
//    then
//    xparts:= a; 
//    a:= (ymaxb-yminb-1) div ypart+1; (* zoveel parts in border *)
//    if a<yparts
//    then
//    yparts:= a; 
//    xpart:= (xmaxb-xminb) div xparts; 
//    ypart:= (ymaxb-yminb) div yparts; 
//    
//  end;
//  
//  for{while} nr:=0 to Pred(xparts*yparts) { nr++}
//  do
//  begin 
//    
//    if R.r.mode and R_PANORAMA
//    then
//    begin 
//      allparts[nr][0]:= 0; 
//      allparts[nr][1]:= 0; 
//      allparts[nr][2]:= R.rectx; 
//      allparts[nr][3]:= R.recty; 
//    end;
//    else
//    begin 
//      xd:= (nr mod xparts); 
//      yd:= (nr-xd) div xparts; 
//      allparts[nr][0]:= xminb+xd*xpart; 
//      allparts[nr][1]:= yminb+yd*ypart; 
//      if xd<R.r.xparts-1
//      then
//      allparts[nr][2]:= allparts[nr][0]+xpart; 
//      else
//      allparts[nr][2]:= xmaxb; 
//      if yd<R.r.yparts-1
//      then
//      allparts[nr][3]:= allparts[nr][1]+ypart; 
//      else
//      allparts[nr][3]:= ymaxb; 
//      if allparts[nr][2]-allparts[nr][0]<=0
//      then
//      allparts[nr][0]:= -1; 
//      if allparts[nr][3]-allparts[nr][1]<=0
//      then
//      allparts[nr][0]:= -1; 
//    end;
//  end;
//end;
//
//(* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ *)
//(* return 0 als geen goede part *)
//
//function setpart(nr: smallint): smallint; 
//var
//xd: smallint; 
//yd: smallint; 
//xpart: smallint; 
//ypart: smallint; 
//begin
//  
//  
//  
//  if allparts[nr][0]=-1
//  then
//  begin
//    result:= 0; 
//    exit;
//    
//  end;
//  R.xstart:= allparts[nr][0]-R.afmx; 
//  R.ystart:= allparts[nr][1]-R.afmy; 
//  R.xend:= allparts[nr][2]-R.afmx; 
//  R.yend:= allparts[nr][3]-R.afmy; 
//  R.rectx:= R.xend-R.xstart; 
//  R.recty:= R.yend-R.ystart; 
//  begin
//    result:= 1; 
//    exit;
//  end;
//end;
//
//(* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ *)
//
//procedure addparttorect(nr: smallint;  part: pPart); 
//var
//rt: puint; 
//rp: puint; 
//y: smallint; 
//heigth: smallint; 
//len: smallint; 
//(* de juiste offset in rectot *)
//
//begin
//  
//  
//  
//  
//  rt:= R.rectot+(allparts[nr][1]*R.rectx+allparts[nr][0]); 
//  rp:= part.rect; 
//  len:= (allparts[nr][2]-allparts[nr][0]); 
//  heigth:= (allparts[nr][3]-allparts[nr][1]); 
//  for{while} y:=0 to Pred(heigth) {y++}
//  do
//  begin 
//    memcpy(rt,rp,4*len); 
//    rt:= rt + (R.rectx); 
//    rp:= rp + (len); 
//  end;
//end;
//
//
//(* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ *)
//
//procedure freeroteerscene; 
//var
//base: pBase; 
//shb: pShadBuf; 
//ob: pObject; 
//me: pMesh; 
//cu: pCurve; 
//mb: pMetaBall; 
//dl: pDispList; 
//ztile: pulong; 
//lay: uint; 
//a: integer; 
//b: integer; 
//v: integer; 
//ctile: pchar; 
//(* VRIJGEVEN *)
//
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
//  for{while} a:=0 to Pred(R.totlamp) { a++}
//  do
//  begin 
//    if R.la[a].shb<>0{nil} {<= !!!9} 
//    then
//    begin 
//      shb:= R.la[a].shb; 
//      v:= (shb.size*shb.size) div 256; 
//      ztile:= shb.zbuf; 
//      ctile:= shb.cbuf; 
//      while{for} 
//      b:= 0; 
//      {to} b<v
//      {;}
//      inc(b); 
//      inc(ztile); 
//      inc(ctile); 
//      do
//      begin 
//        if {*}ctile^
//        then
//        freeN((void* )*ztile); 
//      end;
//      freeN(shb.zbuf); 
//      freeN(shb.cbuf); 
//      freeN(R.la[a].shb); 
//    end;
//    if R.la[a].org<>0{nil} {<= !!!9} 
//    then
//    freeN(R.la[a].org); 
//    freeN(R.la[a]); 
//  end;
//  a:= 0; 
//  while R.blove[a]
//  do
//  begin 
//    freeN(R.blove[a]); 
//    R.blove[a]:= 0; 
//    inc(a); 
//  end;
//  a:= 0; 
//  while R.blovl[a]
//  do
//  begin 
//    freeN(R.blovl[a]); 
//    R.blovl[a]:= 0; 
//    inc(a); 
//  end;
//  a:= 0; 
//  while R.bloha[a]
//  do
//  begin 
//    freeN(R.bloha[a]); 
//    R.bloha[a]:= 0; 
//    inc(a); 
//  end;
//  lay:= G.scene.lay; 
//  (* layers: in foreground current 3D window renderen *)
//  if G.vd<>0{nil} {<= !!!9} 
//  then
//  lay:= G.vd.lay; 
//  ob:= G.main.object.first; 
//  (* orco vrijgeven. ALle ob's aflopen ivm dupli's en sets *)
//  while ob
//  do
//  begin 
//    
//    
//    function ELEM3{!!!3 unknown typedef}: if; 
//    begin
//      cu:= ob.data; 
//      if cu.orco<>0{nil} {<= !!!9} 
//      then
//      begin 
//        freeN(cu.orco); 
//        cu.orco:= 0; 
//      end;
//    end
//    else
//    if ob.type=OB_MESH
//    then
//    begin 
//      me:= ob.data; 
//      if me.orco<>0{nil} {<= !!!9} 
//      then
//      begin 
//        freeN(me.orco); 
//        me.orco:= 0; 
//      end;
//    end;
//    else
//    if ob.type=OB_MBALL
//    then
//    begin 
//      if ob.disp.first)and(ob.disp.first<>ob.disp.last
//      then
//      begin 
//        dl:= ob.disp.first; 
//        remlink(@ob.disp,dl); 
//        freedisplist(@ob.disp); 
//        addtail(@ob.disp,dl); 
//      end;
//    end;
//    ob:= ob.id.next; 
//  end;
//end_render_textures(); 
//
//end_render_materials(); 
//R.totvlak:= R.totvert:=R.totlamp:=R.tothalo:=0; 
//
//end;
//
//
//{hash: array [0..Pred(512)] of char; }{<= !!!5 external variable}
//(* - er moet een 'vast' aantal sterren gegenereerd worden tussen  en .
// * - alle sterren moeten bij voorkeur op de  liggen en uitsluitend in
// *   helderheid / kleur verschillen.
// * - 
// *)
//
//
//procedure make_stars(wire: integer); 
//var
//har: pHaloRen; 
//dblrand: double; 
//hlfrand: double; 
//vec: array [0..Pred(4)] of float; 
//fx: float; 
//fy: float; 
//fz: float; 
//fac: float; 
//starmindist: float; 
//clipend: float; 
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//stargrid: float; 
//maxrand: float; 
//x: integer; 
//y: integer; 
//z: integer; 
//sx: integer; 
//sy: integer; 
//sz: integer; 
//ex: integer; 
//ey: integer; 
//ez: integer; 
//maxjit: integer; 
//done: integer;
// 
//seed: array [0..Pred(3)] of ushort; 
//camera: pCamera; 
//first: integer;
// 
//tx: float; 
//ty: float; 
//tz: float; 
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
//  float{!!!e unknown token},
//  ,
//  force{!!!e unknown token},
//  alpha{!!!e unknown token}; 
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
//  done:=0; 
//  
//  
//  first:=LongBool(1); 
//  if wire<>0{nil} {<= !!!9} 
//  then
//  R.wrld:= *(G.scene.world); 
//  stargrid:= R.wrld.stardist; 
//  maxrand:= 2.0; 
//  maxjit:= (256.0*R.wrld.starcolnoise); :=R.; 
//  :=R.; 
//  force:= (R.wrld.starsize); 
//  starmindist:= R.wrld.starmindist; 
//  (* om de hoeveel een ster ? *)
//  (* hoeveel mag een ster verschuiven (uitgedrukt in grid eenheden) *)
//  (* hoeveel mag een kleur veranderen *)
//  
//  
//  
//  (* afmeting sterren *)
//  
//  (* minimale vrije ruimte *)
//  
//  if stargrid<=0.10
//  then
//  exit;
//  
//  if wire=0
//  then
//  R.flag:= R.flag or (R_HALO); 
//  else
//  stargrid:= stargrid * (1.0); 
//  Mat4Invert(mat,R.viewmat); 
//  (* tekent er minder *)
//  
//  
//  camera:= G.scene.camera.data; 
//  clipend:= camera.clipend; 
//  sx:= ((mat[3][0]-clipend) div stargrid)-maxrand; 
//  sy:= ((mat[3][1]-clipend) div stargrid)-maxrand; 
//  sz:= ((mat[3][2]-clipend) div stargrid)-maxrand; 
//  ex:= ((mat[3][0]+clipend) div stargrid)+maxrand; 
//  ey:= ((mat[3][1]+clipend) div stargrid)+maxrand; 
//  ez:= ((mat[3][2]+clipend) div stargrid)+maxrand; 
//  dblrand:= maxrand*stargrid; 
//  hlfrand:= 2.0*dblrand; 
//  (* BOUNDINGBOX BEREKENING
//    * bbox loopt van z =  | ,
//    * x = -z | +z,
//    * y = -z | +z
//    *)
//  
//  
//  (* omzetten naar grid coordinaten *)
//  
//  
//  
//  
//  if wire<>0{nil} {<= !!!9} 
//  then
//  begin 
//    cpack(-1); 
//    {$ifdef GLDEF}
//    bgnpoint(); 
//    {$else}
//    glPointSize(1.0); 
//    glBegin(GL_POINTS); 
//    {$endif}
//  end;
//  
//  while{for} 
//  x:= sx; 
//  fx:= sx*stargrid; 
//  {to} x<=ex
//  {;}
//  inc(x); 
//  fx:= fx + (stargrid); 
//  do
//  begin 
//    while{for} 
//    y:= sy; 
//    fy:= sy*stargrid; 
//    {to} y<=ey
//    {;}
//    inc(y); 
//    fy:= fy + (stargrid); 
//    do
//    begin 
//      while{for} 
//      z:= sz; 
//      fz:= sz*stargrid; 
//      {to} z<=ez
//      {;}
//      inc(z); 
//      fz:= fz + (stargrid); 
//      do
//      begin 
//        
//        srand48((hash[z and $ff] shl 24)+(hash[y and $ff] shl 16)+(hash[x and $ff] shl 8)); 
//        vec[0]:= fx+(hlfrand*drand48())-dblrand; 
//        vec[1]:= fy+(hlfrand*drand48())-dblrand; 
//        vec[2]:= fz+(hlfrand*drand48())-dblrand; 
//        vec[3]:= 1.0; 
//        if wire<>0{nil} {<= !!!9} 
//        then
//        begin 
//          (* if(done & 1) glVertex3fv(vec); *)
//          if done and 1
//          then
//          glVertex3fv(vec); 
//          inc(done); 
//        end;
//        else
//        begin 
//          Mat4MulVecfl(R.viewmat,vec); 
//          begin 
//            
//            (* in vec staan globale coordinaten
//                  * bereken afstand tot de kamera
//                  * en bepaal aan de hand daarvan de alpha
//                  *)
//            
//            
//            
//            tx:= vec[0]; 
//            ty:= vec[1]; 
//            tz:= vec[2]; 
//            alpha:= fsqrt(tx*tx+ty*ty+tz*tz); 
//            
//            if alpha>=clipend
//            then
//            alpha:= 0.0; 
//            else
//            if alpha<=starmindist
//            then
//            alpha:= 0.0; 
//            else
//            if alpha<=2.0*starmindist
//            then
//            begin 
//              alpha:= (alpha-starmindist) div starmindist; 
//            end;
//            else
//            begin 
//              alpha:= alpha - (2.0*starmindist); 
//              alpha:= alpha div ((clipend-2.0*starmindist)); 
//              alpha:= 1.0-alpha; 
//            end;
//          end;
//          
//          
//          if alpha<>0.0
//          then
//          begin 
//            fac:= force*drand48(); 
//            har:= initstar(vec,fac); 
//            
//            if har<>0{nil} {<= !!!9} 
//            then
//            begin 
//              har.alfa:= fsqrt(fsqrt(alpha)); 
//              har.add:= 255; 
//              har.r:= har.g:=har.b:=255; 
//              if maxjit<>0{nil} {<= !!!9} 
//              then
//              begin 
//                har.r:= har.r + (((maxjit*drand48()))-maxjit); 
//                har.g:= har.g + (((maxjit*drand48()))-maxjit); 
//                har.b:= har.b + (((maxjit*drand48()))-maxjit); 
//              end;
//              har.hard:= 32; 
//              har.type:= har.type or (HA_ONLYSKY); 
//              inc(done); 
//              
//            end;
//          end;
//        end;
//      end;
//      if done>MAXVERT
//      then
//      begin 
//        printf('Too many stars\n'); 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      if test_break()
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    
//    if done>MAXVERT
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    
//    if test_break()
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//  end;
//  {$ifdef GLDEF}
//  if wire<>0{nil} {<= !!!9} 
//  then
//endpoint(); 
//{$else}
//if wire<>0{nil} {<= !!!9} 
//then
//glEnd(); 
//{$endif}
//end;
//
//
//procedure init_render_object(ob: pObject); 
//var
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//begin
//  ob.flag:= ob.flag or (OB_DONE); 
//  if ob.type=OB_LAMP
//  then
//  add_render_lamp(ob,1); 
//  else
//  if{!!!e unknown token}
//  init_render_curve(ob); 
//  else
//  if ob.type=OB_SURF
//  then
//  init_render_surf(ob); 
//  else
//  if ob.type=OB_MESH
//  then
//  init_render_mesh(ob); 
//  else
//  if ob.type=OB_MBALL
//  then
//  init_render_mball(ob); 
//  else
//  begin 
//    Mat4MulMat4(mat,ob.obmat,R.viewmat); 
//    Mat4Invert(ob.imat,mat); 
//  end;
//end;
//
//
//procedure holoview; 
//var
//cam: pCamera; 
//dist: float; 
//fac: float; 
//fy: float; 
//fz: float; 
//begin
//  
//  
//  
//  
//  if G.scene=0)or(G.scene.camera=0
//  then
//  exit;
//  
//  if G.scene.camera.type=OB_CAMERA
//  then
//  begin 
//    cam:= G.scene.camera.data; 
//    if cam.flag and (CAM_HOLO1 or CAM_HOLO2)
//    then
//    begin 
//      fy:= G.scene.camera.loc[1]; 
//      fz:= G.scene.camera.loc[2]; 
//      dist:= cam.hololen*fsqrt(fy*fy+fz*fz); 
//      fac:= (CFRA-SFRA) div ({!!!a type cast? =>} {float(}(EFRA-SFRA)); 
//      G.scene.camera.loc[0]:= -dist+2*fac*dist; 
//      
//      
//    end;
//  end;
//end;
//
//
//procedure set_normalflags; 
//var
//vlr: pVlakRen; 
//vec: array [0..Pred(3)] of float; 
//xn: float; 
//yn: float; 
//zn: float; 
//a1: integer; (* KLAP NORMAAL EN SNIJ PROJECTIE *)
//begin
//  
//  
//  
//  
//  
//  for{while} a1:=0 to Pred(R.totvlak) { a1++}
//  do
//  begin 
//    if (a1 and 255)=0
//    then
//    vlr:= R.blovl[a1 shr 8]; 
//    else
//    inc(vlr); 
//    if vlr.flag and R_NOPUNOFLIP
//    then
//    begin 
//      (* render normaal flippen, wel niet zo netjes, maar anders dan moet de render() ook over... *)
//      vlr.n[0]:= -vlr.n[0]; 
//      vlr.n[1]:= -vlr.n[1]; 
//      vlr.n[2]:= -vlr.n[2]; 
//    end;
//    else
//    begin 
//      
//      vec[0]:= vlr.v1.co[0]; 
//      vec[1]:= vlr.v1.co[1]; 
//      vec[2]:= vlr.v1.co[2]; 
//      if (vec[0]*vlr.n[0]+vec[1]*vlr.n[1]+vec[2]*vlr.n[2])<0.0
//      then
//      begin 
//        vlr.puno:=  not (vlr.puno); 
//        vlr.n[0]:= -vlr.n[0]; 
//        vlr.n[1]:= -vlr.n[1]; 
//        vlr.n[2]:= -vlr.n[2]; 
//      end;
//    end;
//    xn:= fabs(vlr.n[0]); 
//    yn:= fabs(vlr.n[1]); 
//    zn:= fabs(vlr.n[2]); 
//    if zn>=xn)and(zn>=yn
//    then
//    vlr.snproj:= 0; 
//    else
//    if yn>=xn)and(yn>=zn
//    then
//    vlr.snproj:= 1; 
//    else
//    vlr.snproj:= 2; 
//    
//  end;
//end;
//
//
//procedure roteerscene; 
//var
//base: pBase; 
//ob: pObject; 
//obd: pObject; 
//sce: pScene; 
//cam: pCamera; 
//vlr: pVlakRen; 
//lay: uint; 
//vec: array [0..Pred(3)] of float; 
//xn: float; 
//yn: float; 
//zn: float; 
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//a1: integer; 
//cu: pCurve; 
//begin
//  {slurph_opt: integer; }{<= !!!5 external variable}(* key.c *)
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
//  if G.scene.camera=0
//  then
//  exit;
//  
//  O.dxwin[0]:= 0.5 div {!!!a type cast? =>} {float(}R.r.xsch; 
//  O.dywin[1]:= 0.5 div {!!!a type cast? =>} {float(}R.r.ysch; 
//  slurph_opt:= 0; 
//  R.totvlak:= R.totvert:=R.totlamp:=R.tothalo:=0; 
//  do_all_ipos(); 
//  
//  
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
//  (* cameranetwerk? *)
//  if R.r.scemode and R_NETWORK
//  then
//  render_frame_to_camera(); 
//  
//  if G.special1 and G_HOLO
//  then
//  holoview(); 
//  base:= FIRSTBASE; 
//  (* ivm met optimale berekening track / lattices / etc: extra where_is_ob *)
//  while base
//  do
//  begin 
//    where_is_object(base.object); 
//    if base.next=0)and(G.scene.set)and(base=G.scene.base.last
//    then
//    base:= G.scene.set.base.first; 
//    else
//    base:= base.next; 
//  end;
//  Mat4CpyMat4(R.viewinv,G.scene.camera.obmat); 
//  
//  Mat4Ortho(R.viewinv); 
//  Mat4Invert(R.viewmat,R.viewinv); 
//  
//  (* is niet netjes: nu is de viewinv ongelijk aan de viewmat. voor Texco's enzo. Beter doen! *)
//  if R.r.mode and R_ORTHO
//  then
//  R.viewmat[3][2]:= R.viewmat[3][2] * (100.0); 
//  setwindowclip(1,-1); 
//  
//  ob:= G.main.object.first; (*  geen jit:(-1) *)
//  
//  (* imatflags wissen *)
//  while ob
//  do
//  begin 
//    ob.flag:= ob.flag and ( not OB_DO_IMAT); 
//    ob:= ob.id.next; 
//  end;
//  init_render_world(); 
//  
//  init_render_textures(); 
//  (* moet eerst ivm ambient *)
//  init_render_materials(); 
//  ob:= G.main.object.first; 
//  (* MAAK RENDERDATA *)
//  
//  (* elk object maar 1 x renderen *)
//  while ob
//  do
//  begin 
//    ob.flag:= ob.flag and ( not OB_DONE); 
//    ob:= ob.id.next; 
//  end;
//  lay:= G.scene.lay; 
//  
//  (* layers: in foreground current 3D window renderen *)
//  if G.vd<>0{nil} {<= !!!9} 
//  then
//  lay:= G.vd.lay; 
//  sce:= G.scene; 
//  base:= FIRSTBASE; 
//  while base
//  do
//  begin 
//    ob:= base.object; 
//    if ob.flag and OB_DONE
//    then
//    ; 
//    else
//    begin 
//      
//      where_is_object(ob); 
//      
//      if (base.lay and lay))or((ob.type=OB_LAMP)and((base.lay and G.scene.lay))
//      then
//      begin 
//        
//        (* if(ob->infoflag && (R.flag & R_ANIMRENDER)) info_file(ob); *)
//        
//        if ob.transflag and OB_DUPLI
//        then
//        begin 
//          {duplilist: ListBase; }{<= !!!5 external variable}
//          (* exception: mballs! *)
//          make_duplilist(sce,ob); 
//          if ob.type=OB_MBALL
//          then
//          begin 
//            init_render_object(ob); 
//          end;
//          else
//          begin 
//            obd:= duplilist.first; 
//            if obd<>0{nil} {<= !!!9} 
//            then
//            begin 
//              (* exception, in background render it doesnt make the displist *)
//              
//              function ELEM{!!!3 unknown typedef}: if; 
//              var
//              cu: pCurve; 
//              begin
//                
//                cu:= obd.data; 
//                if cu.disp.first=0
//                then
//                begin 
//                  obd.flag:= obd.flag and ( not OB_FROMDUPLI); 
//                  makeDispList(obd); 
//                  obd.flag:= obd.flag or (OB_FROMDUPLI); 
//                end;
//              end
//            end;
//            obd:= duplilist.first; 
//            while obd
//            do
//            begin 
//              if obd.type<>OB_MBALL
//              then
//              init_render_object(obd); 
//              obd:= obd.id.next; 
//            end;
//          end;
//          free_duplilist(); 
//        end;
//        else
//        init_render_object(ob); 
//      end;
//      
//      else
//      begin 
//        Mat4MulMat4(mat,ob.obmat,R.viewmat); 
//        Mat4Invert(ob.imat,mat); 
//      end;
//      ob.flag:= ob.flag and ( not OB_DO_IMAT); 
//      
//    end;
//    if test_break()
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    
//    if base.next=0)and(G.scene.set)and(base=G.scene.base.last
//    then
//    begin 
//      base:= G.scene.set.base.first; 
//      sce:= G.scene.set; 
//    end;
//    else
//    base:= base.next; 
//    
//  end;
//  info_file(0); 
//  (* infofile sluiten *)
//  ob:= G.main.object.first; 
//  (* imat objecten *)
//  while ob
//  do
//  begin 
//    if ob.flag and OB_DO_IMAT
//    then
//    begin 
//      
//      ob.flag:= ob.flag and ( not OB_DO_IMAT); 
//      Mat4MulMat4(mat,ob.obmat,R.viewmat); 
//      
//      Mat4Invert(ob.imat,mat); 
//    end;
//    ob:= ob.id.next; 
//  end;
//  sort_halos(); 
//  
//  
//  if R.wrld.mode and WO_STARS
//  then
//  make_stars(0); 
//  slurph_opt:= 1; 
//  
//  if test_break()
//  then
//  exit;
//  
//  (* if(R.totlamp==0) defaultlamp(); *)
//  set_normalflags(); 
//end;
//
//(* cmapcode handleiding:
// * 
// * 
// * - globale optie: R.r.scemode & R_HOTSPOT
// * - visible truuk: vlr->flag R_VISIBLE vantevoren zetten, functie zbufvlaggen!
// * - ob->infosta == hotspot nr
// * - ob->infoend == don't render
// *)
//
//
//
//procedure add_cmapcode; 
//var
//ibuf: pImBuf; 
//len: uint; 
//temprect: puint; 
//rt: puint; 
//name: array [0..Pred(256)] of char; 
//begin
//  
//  
//  
//  
//  temprect:= R.rectot; 
//  if temprect=0
//  then
//  exit;
//  
//  len:= R.rectx*R.recty; 
//  R.rectot:= callocN(sizeof(int)*len,'rectcmap'); 
//  zbufferall_cmapcode(); 
//  
//  rt:= R.rectot; 
//  ibuf:= allocImBuf(R.rectx,R.recty,8,0,0); 
//  ibuf.rect:= R.rectot; 
//  sprintf(name,'%scode%04d',G.scene.r.pic,CFRA); 
//  
//  (* kleurnummers als cmap wegschrijven *)
//  
//  
//  
//  saveiff(ibuf,name,IB_rect); 
//  
//  printf('Saved: %s\n',name); 
//  freeImBuf(ibuf); 
//  
//  freeN(R.rectot); 
//  R.rectot:= temprect; 
//end;
//
//var {was static}
//blurrect: puint;
// 
//
//procedure add_to_blurbuf(blur: integer); 
//var
//tot: integer; 
//gamval: integer; 
//facr: smallint; 
//facb: smallint; 
//rtr: pchar; 
//rtb: pchar; 
//begin
//  blurrect:=0; 
//  
//  
//  
//  
//  
//  if blur<0
//  then
//  begin 
//    if blurrect<>0{nil} {<= !!!9} 
//    then
//    begin 
//      if R.rectot<>0{nil} {<= !!!9} 
//      then
//      freeN(R.rectot); 
//      R.rectot:= blurrect; 
//      blurrect:= 0; 
//    end;
//  end;
//  else
//  if blur=R.osa-1
//  then
//  begin 
//    (* eerste keer *)
//    blurrect:= mallocN(R.rectx*R.recty*sizeof(int),'rectblur'); 
//    if R.rectot<>0{nil} {<= !!!9} 
//    then
//    memcpy(blurrect,R.rectot,R.rectx*R.recty*4); 
//  end;
//  else
//  if blurrect<>0{nil} {<= !!!9} 
//  then
//  begin 
//    (* accumuleren *)
//    
//    facr:= 256 div (R.osa-blur); 
//    facb:= 256-facr; 
//    if R.rectot<>0{nil} {<= !!!9} 
//    then
//    begin 
//      rtr:= {!!!a type cast? =>} {pchar(}R.rectot; 
//      rtb:= {!!!a type cast? =>} {pchar(}blurrect; 
//      tot:= R.rectx*R.recty; 
//      while tot{--} dec(tot); 
//      do
//      begin 
//        if *({!!!a type cast? =>} {puint(}rtb)<>*({!!!a type cast? =>} {puint(}rtr)
//        then
//        begin 
//          
//          if R.r.mode and R_GAMMA
//          then
//          begin 
//            gamval:= (facr*igamtab2[rtr[0] shl 8]+facb*igamtab2[rtb[0] shl 8]) shr 8; 
//            rtb[0]:= gamtab[gamval] shr 8; 
//            gamval:= (facr*igamtab2[rtr[1] shl 8]+facb*igamtab2[rtb[1] shl 8]) shr 8; 
//            rtb[1]:= gamtab[gamval] shr 8; 
//            gamval:= (facr*igamtab2[rtr[2] shl 8]+facb*igamtab2[rtb[2] shl 8]) shr 8; 
//            rtb[2]:= gamtab[gamval] shr 8; 
//            gamval:= (facr*igamtab2[rtr[3] shl 8]+facb*igamtab2[rtb[3] shl 8]) shr 8; 
//            rtb[3]:= gamtab[gamval] shr 8; 
//          end;
//          else
//          begin 
//            rtb[0]:= (facr*rtr[0]+facb*rtb[0]) shr 8; 
//            rtb[1]:= (facr*rtr[1]+facb*rtb[1]) shr 8; 
//            rtb[2]:= (facr*rtr[2]+facb*rtb[2]) shr 8; 
//            rtb[3]:= (facr*rtr[3]+facb*rtb[3]) shr 8; 
//          end;
//        end;
//        rtr:= rtr + (4); 
//        rtb:= rtb + (4); 
//      end;
//    end;
//    if blur=0
//    then
//    begin 
//      (* laatste keer *)
//      if R.rectot<>0{nil} {<= !!!9} 
//      then
//      freeN(R.rectot); 
//      R.rectot:= blurrect; 
//      blurrect:= 0; 
//    end;
//  end;
//end;
//
//
//(* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ *)
//(* hierbinnen de PART en FIELD lussen *)
//
//procedure render; 
//var
//part: pPart; 
//mat: array [0..Pred(4),0..Pred(4)] of float; 
//rt: puint; 
//rt1: puint; 
//rt2: puint; 
//file1: integer; 
//file2: integer; 
//len: integer; 
//a1: integer; 
//y: integer; 
//blur: smallint; 
//a: smallint; 
//fields: smallint; 
//fi: smallint; 
//parts: smallint; (* pa is globaal ivm print *)
//(* ZR ADDED THIS *)
//border_buf: puint;
// 
//border_x: uint;
// 
//border_y: uint;
// 
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
//  border_buf:=0{nil}; 
//  border_x:=0{nil}; 
//  border_y:=0{nil}; 
//  if (R.r.mode and R_BORDER))and({not}0=(R.r.mode and R_MOVIECROP)
//  then
//  begin 
//    border_buf:= R.rectot; 
//    border_x:= R.rectx; 
//    border_y:= R.recty; 
//    R.rectot:= 0; 
//  end;
//  (* END BAD ZR *)
//  
//  
//  if R.rectz<>0{nil} {<= !!!9} 
//  then
//  freeN(R.rectz); 
//  R.rectz:= 0; 
//  fields:= 1; 
//  parts:= R.r.xparts*R.r.yparts; 
//  (* FIELDLUS *)
//  
//  if R.r.mode and R_FIELDS
//  then
//  begin 
//    fields:= 2; 
//    R.rectf1:= R.rectf2:=0; 
//    R.r.ysch:= R.r.ysch div (2); 
//    R.afmy:= R.afmy div (2); 
//    R.r.yasp:= R.r.yasp * (2); 
//    R.ycor:= ({!!!a type cast? =>} {float(}R.r.yasp) div ({!!!a type cast? =>} {float(}R.r.xasp); 
//    (* fieldrecten *)
//    
//  end;
//  
//  for{while} fi:=0 to Pred(fields) { fi++}
//  do
//  begin 
//    
//    (* INIT *)
//    srand48(2*CFRA+fi); 
//    R.vlaknr:= -1; 
//    R.flag:= R.flag or (R_RENDERING); 
//    if fi=1
//    then
//    R.flag:= R.flag or (R_SEC_FIELD); 
//    (* MOTIONBLUR lus *)
//    if R.r.mode and R_MBLUR
//    then
//    blur:= R.osa; 
//    else
//    blur:= 1; 
//    while blur{--} dec(blur); 
//    do
//    begin 
//      
//      (* WINDOW *)
//      R.rectx:= R.r.xsch; 
//      R.recty:= R.r.ysch; 
//      R.xstart:= -R.afmx; 
//      R.ystart:= -R.afmy; 
//      R.xend:= R.xstart+R.rectx-1; 
//      R.yend:= R.ystart+R.recty-1; 
//      if R.r.mode and R_MBLUR
//      then
//      set_mblur_offs(R.osa-blur); 
//      initparts(); 
//      
//      setpart(0); 
//      (* altijd doen ivm border *)
//      init_render_display(); 
//      clear_render_display(); 
//      set_timecursor(CFRA); 
//      
//      (* ROTEERSCENE *)
//      
//      if R.rectot<>0{nil} {<= !!!9} 
//      then
//      freeN(R.rectot); 
//      R.rectot:= 0; 
//      if R.rectz<>0{nil} {<= !!!9} 
//      then
//      freeN(R.rectz); 
//      R.rectz:= 0; 
//      roteerscene(); 
//      
//      
//      (* SCHADUWBUFFER *)
//      for{while} a:=0 to Pred(R.totlamp) { a++}
//      do
//      begin 
//        if test_break()
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        if R.la[a].shb<>0{nil} {<= !!!9} 
//        then
//        makeshadowbuf(R.la[a]); 
//      end;
//      make_envmaps(); 
//      
//      (* ENVIRONMENT MAPS *)
//      R.parts.first:= R.parts.last:=0; (* PARTS *)
//      for{while} pa:=0 to Pred(parts) { pa++}
//      do
//      begin 
//        
//        if test_break()
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        
//        if pa<>0{nil} {<= !!!9} 
//        then
//        begin 
//          (* want pa==0 is al gedaan *)
//          if setpart(pa)=0
//          then
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        
//        if R.r.mode and R_MBLUR
//        then
//        setwindowclip(0,blur); 
//        else
//        setwindowclip(0,-1); 
//        
//        if R.r.mode and R_PANORAMA
//        then
//        setpanorot(pa); 
//        setzbufvlaggen(projectverto); 
//        
//        (* HOMOGENE COORDINATEN EN ZBUF EN CLIP OPT (per part) *)
//        if test_break()
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        
//        (* ZBUFFER & SHADE *)
//        R.rectot:= {!!!a type cast? =>} {puint(}callocN(sizeof(int)*R.rectx*R.recty,'rectot'); 
//        R.rectz:= {!!!a type cast? =>} {puint(}mallocN(sizeof(int)*R.rectx*R.recty,'rectz'); 
//        if R.r.mode and R_MBLUR
//        then
//        printrenderinfo(R.osa-blur); 
//        else
//        printrenderinfo(-1); 
//        
//        if R.r.mode and R_OSA
//        then
//        zbufshadeDA(); 
//        else
//        zbufshade(); 
//        if test_break()
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        
//        (* uitzondering *)
//        if (R.r.mode and R_BORDER))and((R.r.mode and R_MOVIECROP)
//        then
//        ; 
//        else
//        begin 
//          (* PART OF BORDER AFHANDELEN *)
//          if parts>1)or((R.r.mode and R_BORDER)
//          then
//          begin 
//            
//            part:= callocN(sizeof(Part),'part'); 
//            addtail(@R.parts,part); 
//            part.rect:= R.rectot; 
//            R.rectot:= 0; 
//            freeN(R.rectz); 
//            
//            R.rectz:= 0; 
//          end;
//        end;
//      end;
//      
//      (* PARTS SAMENVOEGEN OF BORDER INVOEGEN *)
//      
//      (* uitzondering: crop *)
//      if (R.r.mode and R_BORDER))and((R.r.mode and R_MOVIECROP)
//      then
//      ; 
//      else
//      begin 
//        R.rectx:= R.r.xsch; 
//        R.recty:= R.r.ysch; 
//        if R.r.mode and R_PANORAMA
//        then
//        R.rectx:= R.rectx * (R.r.xparts); 
//        if parts>1)or((R.r.mode and R_BORDER)
//        then
//        begin 
//          if R.rectot<>0{nil} {<= !!!9} 
//          then
//          freeN(R.rectot); 
//          (* ZR CHANGED THIS *)
//          if R.r.mode and R_BORDER
//          then
//          begin 
//            if border_x<R.rectx)or(border_y<R.recty)or(border_buf=0{nil}
//            then
//            R.rectot:= {!!!a type cast? =>} {puint(}callocN(sizeof(int)*R.rectx*R.recty,'rectot'); 
//            else
//            R.rectot:= border_buf; 
//          end;
//          else
//          (*      if(R.r.mode & R_BORDER) R.rectot= (uint * )callocN(sizeof(int)*R.rectx*R.recty, "rectot"); *)
//          (* END BAD ZR *)
//          R.rectot:= {!!!a type cast? =>} {puint(}mallocN(sizeof(int)*R.rectx*R.recty,'rectot'); 
//          part:= R.parts.first; 
//          for{while} pa:=0 to Pred(parts) { pa++}
//          do
//          begin 
//            if allparts[pa][0]=-1
//            then
//            break; {<= !!!b possible in "switch" - then remove this line}
//            if part=0
//            then
//            break; {<= !!!b possible in "switch" - then remove this line}
//            
//            if R.r.mode and R_PANORAMA
//            then
//            begin 
//              if pa<>0{nil} {<= !!!9} 
//              then
//              begin 
//                allparts[pa][0]:= allparts[pa][0] + (pa*R.r.xsch); 
//                allparts[pa][2]:= allparts[pa][2] + (pa*R.r.xsch); 
//              end;
//            end;
//            addparttorect(pa,part); 
//            part:= part.next; 
//            
//          end;
//          part:= R.parts.first; 
//          while part
//          do
//          begin 
//            freeN(part.rect); 
//            part:= part.next; 
//          end;
//          freelistN(@R.parts); 
//        end;
//      end;
//      
//      if R.flag and R_HALO
//      then
//      begin 
//        add_halo_flare(); 
//      end;
//      
//      if R.r.scemode and R_HOTSPOT
//      then
//      begin 
//        add_cmapcode(); 
//      end;
//      
//      if R.r.mode and R_MBLUR
//      then
//      begin 
//        add_to_blurbuf(blur); 
//      end;
//      freeroteerscene(); 
//      
//      (* EINDE (blurlus) *)
//      
//      if test_break()
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    add_to_blurbuf(-1); 
//    
//    (* definitief vrijgeven *)
//    
//    (* FIELD AFHANDELEN *)
//    if R.r.mode and R_FIELDS
//    then
//    begin 
//      if R.flag and R_SEC_FIELD
//      then
//      R.rectf2:= R.rectot; 
//      else
//      R.rectf1:= R.rectot; 
//      R.rectot:= 0; 
//    end;
//    
//    if test_break()
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//  end;
//  
//  (* FIELDS SAMENVOEGEN *)
//  if R.r.mode and R_FIELDS
//  then
//  begin 
//    R.r.ysch:= R.r.ysch * (2); 
//    R.afmy:= R.afmy * (2); 
//    R.recty:= R.recty * (2); 
//    R.r.yasp:= R.r.yasp div (2); 
//    if R.rectot<>0{nil} {<= !!!9} 
//    then
//    freeN(R.rectot); 
//    R.rectot:= {!!!a type cast? =>} {puint(}mallocN(sizeof(int)*R.rectx*R.recty,'rectot'); (* komt voor bij afbreek *)
//    
//    if test_break()=0
//    then
//    begin 
//      rt:= R.rectot; 
//      if R.r.mode and R_ODDFIELD
//      then
//      begin 
//        rt2:= R.rectf1; 
//        rt1:= R.rectf2; 
//      end;
//      else
//      begin 
//        rt1:= R.rectf1; 
//        rt2:= R.rectf2; 
//      end;
//      len:= 4*R.rectx; 
//      
//      while{for} 
//      a:= 0; 
//      {to} a<R.recty
//      {;}
//      a:= a + (2); 
//      do
//      begin 
//        memcpy(rt,rt1,len); 
//        rt:= rt + (R.rectx); 
//        rt1:= rt1 + (R.rectx); 
//        memcpy(rt,rt2,len); 
//        rt:= rt + (R.rectx); 
//        rt2:= rt2 + (R.rectx); 
//      end;
//    end;
//  end;
//  
//  (* R.rectx= R.r.xsch; *)
//  (* if(R.r.mode & R_PANORAMA) R.rectx*= R.r.xparts; *)
//  (* R.recty= R.r.ysch; *)
//  
//  (* als border: wel de skybuf doen *)
//  if R.r.mode and R_BORDER
//  then
//  begin 
//    if (R.r.mode and R_MOVIECROP)=0
//    then
//    begin 
//      if R.r.bufflag and 1
//      then
//      begin 
//        R.xstart:= -R.afmx; 
//        R.ystart:= -R.afmy; 
//        rt:= R.rectot; 
//        while{for} 
//        y:= 0; 
//        {to} y<R.recty
//        {;}
//        inc(y); 
//        rt:= rt + (R.rectx); 
//        do
//        scanlinesky({!!!a type cast? =>} {pchar(}rt,y); 
//      end;
//    end;
//  end;
//  set_mblur_offs(0); 
//  
//  
//  (* VRIJGEVEN *)
//  
//  (* zbuf test *)
//  
//  (* don't free R.rectz, only when its size is not the same as R.rectot *)
//  
//  if R.rectz)and(parts=1)and((R.r.mode and R_FIELDS)=0
//  then
//  ; 
//  else
//  begin 
//    if R.rectz<>0{nil} {<= !!!9} 
//    then
//    freeN(R.rectz); 
//    R.rectz:= 0; 
//  end;
//  
//  if R.rectf1<>0{nil} {<= !!!9} 
//  then
//  freeN(R.rectf1); 
//  R.rectf1:= 0; 
//  if R.rectf2<>0{nil} {<= !!!9} 
//  then
//  freeN(R.rectf2); 
//  R.rectf2:= 0; 
//end;
//
//
//(* ~~~~~~~~~~~~~~~~ timer ~~~~~~~~~~~~~~~~~~~~~~ *)
//
//{$if !defined(__BeOS) && !defined(WINDOWS)}
//
//type
//rusage = record
//end;
//timeval = record
//end;
//timezone = record
//end;
//
//
//procedure start_timer; 
//begin
//  getrusage(RUSAGE_SELF,@rus1); 
//  gettimeofday(@tval1,@rt); 
//end;
//
//type
//timezone = record
//end;
//
//procedure end_timer(real: pinteger;  cpu: pinteger); 
//var
//rt1: integer; 
//rt2: integer; 
//begin(* in hondersten van seconde *)
//  
//  
//  getrusage(RUSAGE_SELF,@rus2); 
//  gettimeofday(@tval2,@rt); 
//  rt1:= 100*rus1.ru_utime.tv_sec+rus1.ru_utime.tv_usec div 10000; 
//  rt1:= rt1 + (100*rus1.ru_stime.tv_sec+rus1.ru_stime.tv_usec div 10000); 
//  rt2:= 100*rus2.ru_utime.tv_sec+rus2.ru_utime.tv_usec div 10000; 
//  rt2:= rt2 + (100*rus2.ru_stime.tv_sec+rus2.ru_stime.tv_usec div 10000); 
//  
//  
//  if cpu<>0{nil} {<= !!!9} 
//  then
//  {*}cpu^:=rt2-rt1; 
//  rt1:= 100*tval1.tv_sec+tval1.tv_usec div 10000; 
//  rt2:= 100*tval2.tv_sec+tval2.tv_usec div 10000; 
//  
//  if real<>0{nil} {<= !!!9} 
//  then
//  {*}real^:=rt2-rt1; 
//  {$ifdef WINDOWS}
//  if cpu)and(real
//  then
//  begin 
//    {*}cpu^:=*real; 
//  end;
//  {$endif}
//end;
//
//{$else}
//
//{$ifdef WINDOWS}
//var {was static}
//old_time: integer; 
//type
//tms = record
//end;
//
//
//procedure start_timer; 
//begin
//  old_time:= times(@voidbuf); 
//end;
//
//type
//tms = record
//end;
//
//procedure end_timer(real: pinteger;  cpu: pinteger); 
//begin
//  {*}real^:=(times(@voidbuf)-old_time) div 10; 
//  if cpu<>0{nil} {<= !!!9} 
//  then
//  {*}cpu^:=*real; 
//end;
//{$else}
//(* BeOS *)
//var {was static}
//old_time: integer; 
//
//
//procedure start_timer; 
//begin
//  old_time:= glut_system_time(); 
//end;
//
//
//procedure end_timer(real: pinteger;  cpu: pinteger); 
//begin{*}real^:=(glut_system_time()-old_time) div 10000; 
//  if cpu<>0{nil} {<= !!!9} 
//  then
//  {*}cpu^:=*real; 
//end;
//{$endif}
//{$endif}
//
//(* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ *)
//
//
//
//procedure initrender; 
//var
//bima: pImage; 
//name: array [0..Pred(256)] of char; (* scenedata naar R *)
//cam: pCamera;
// 
//begin
//  
//  {usegamtab: ushort; }{<= !!!5 external variable}
//  R.r:= G.scene.r; 
//  R.rectx:= R.r.xsch; 
//  R.recty:= R.r.ysch; (* voor zekerheid: bij voortijdige return *)
//  
//  (* MAG ER WEL WORDEN GERENDERD *)
//  
//  (* verboden combinatie *)
//  if (R.r.mode and R_BORDER))and((R.r.mode and R_PANORAMA)
//  then
//  begin 
//    error('No border allowed for Panorama'); 
//    G.afbreek:= 1; 
//    exit;
//  end;
//  
//  if R.r.xparts*R.r.yparts>64
//  then
//  begin 
//    error('No more than 64 parts'); 
//    G.afbreek:= 1; 
//    exit;
//  end;
//  
//  if R.r.yparts>1)and((R.r.mode and R_PANORAMA)
//  then
//  begin 
//    error('No Y-Parts allowed for Panorama'); 
//    G.afbreek:= 1; 
//    exit;
//  end;
//  
//  (* BACKBUF TESTEN *)
//  if (R.r.bufflag and 1))and((G.scene.r.scemode and R_OGL)=0
//  then
//  begin 
//    if R.r.alphamode=R_ADDSKY
//    then
//    begin 
//      strcpy(name,R.r.backbuf); 
//      convertstringcode(name); 
//      
//      if R.backbuf<>0{nil} {<= !!!9} 
//      then
//      begin 
//        dec(R.backbuf.id.us); 
//        bima:= R.backbuf; 
//      end;
//      else
//      bima:= 0; 
//      R.backbuf:= add_image(name); 
//      
//      if bima)and(bima.id.us<1
//      then
//      begin 
//        free_image(bima); 
//      end;
//      if R.backbuf=0
//      then
//      begin 
//        error('No backbuf there!'); 
//        G.afbreek:= 1; 
//        exit;
//      end;
//    end;
//  end;
//  usegamtab:= 0; 
//  (* zie hieronder *)
//  
//  if R.r.mode and (R_OSA or R_MBLUR)
//  then
//  begin 
//    R.osa:= R.r.osa; 
//    if R.osa>16
//    then
//    R.osa:= 16; 
//    init_render_jit(R.osa); 
//    
//    init_filt_mask(); 
//    
//    (* wordt af en toe tijdelijk op nul gezet, o.a. in transp zbuf *)
//    if R.r.mode and R_GAMMA
//    then
//    begin 
//      if (R.r.mode and R_OSA)
//      then
//      usegamtab:= 1; 
//    end;
//  end;
//  else
//  R.osa:= 0; 
//  R.r.xsch:= (R.r.size*R.r.xsch) div 100; 
//  R.r.ysch:= (R.r.size*R.r.ysch) div 100; 
//  R.afmx:= R.r.xsch div 2; 
//  R.afmy:= R.r.ysch div 2; 
//  (* WINDOW *)
//  
//  
//  (* when rendered without camera object *)
//  (* it has to done here because of envmaps *)
//  R{!!!e unknown token}.:=0.1; 
//  R{!!!e unknown token}.:=1000.0; 
//  
//  if R.afmx<1)or(R.afmy<1
//  then
//  begin 
//    error('Image too small'); 
//    exit;
//  end;
//  R.ycor:= ({!!!a type cast? =>} {float(}R.r.yasp) div ({!!!a type cast? =>} {float(}R.r.xasp); 
//  start_timer(); 
//  
//  
//  if R.r.scemode and R_DOSEQ
//  then
//  begin 
//    R.rectx:= R.r.xsch; 
//    R.recty:= R.r.ysch; 
//    if R.rectot<>0{nil} {<= !!!9} 
//    then
//    freeN(R.rectot); 
//    R.rectot:= {!!!a type cast? =>} {puint(}callocN(sizeof(int)*R.rectx*R.recty,'rectot'); 
//    if test_break()=0
//    then
//    do_render_seq(); 
//    
//    (* displayen *)
//    if R.rectot<>0{nil} {<= !!!9} 
//    then
//    render_display(0,R.recty-1); 
//  end;
//  else
//  if R.r.scemode and R_OGL
//  then
//  begin 
//    R.rectx:= R.r.xsch; 
//    R.recty:= R.r.ysch; 
//    if R.rectot<>0{nil} {<= !!!9} 
//    then
//    freeN(R.rectot); 
//    R.rectot:= {!!!a type cast? =>} {puint(}callocN(sizeof(int)*R.rectx*R.recty,'rectot'); 
//    init_render_display(); 
//    
//    printrenderinfo(-1); 
//    (* without this, it doesn't display well at SGI. GLUT! *)
//    if R.win<>0{nil} {<= !!!9} 
//    then
//    begin 
//      drawview3d_render(); 
//    end;
//    else
//    error('Not in DispView mode'); 
//  end;
//  else
//  begin 
//    
//    if G.scene.camera=0
//    then
//    G.scene.camera:= find_camera(); 
//    if G.scene.camera=0
//    then
//    begin 
//      error('No camera'); 
//    end;
//    else
//    begin 
//      
//      if G.scene.camera.type=OB_CAMERA
//      then
//      begin 
//        cam:=G.scene.camera.data; 
//        if cam.type=CAM_ORTHO
//        then
//        R.r.mode:= R.r.mode or (R_ORTHO); 
//      end;
//      render(); 
//      
//    end;
//    (* keert terug met complete rect xsch-ysch *)
//  end;
//  (* nog eens displayen: fields/seq/parts/pano etc *)
//  if R.rectot<>0{nil} {<= !!!9} 
//  then
//  begin 
//    init_render_display(); 
//    render_display(0,R.recty-1); 
//  end;
//  else
//  close_render_display(); 
//end_timer(@G.time,@G.cputime); 
//
//(* TIJD berekenen *)
//printrenderinfo(-1); 
//R.osatex:= 0; 
//R.vlr:= 0; 
//R.flag:= 0; 
//(* variabelen weer goed *)
//(* bij cubemap *)
//end;
//
//
//procedure exit_render_stuff; 
//begin
//  if G.afbreek=1
//  then
//  exit;
//  
//  (* in de gaten houden: backgroundrender !!! *)
//  if G.background<>0{nil} {<= !!!9} 
//  then
//  begin 
//    if G.scene.r.scemode and R_NETWORK
//    then
//    init_render_camera_network(); 
//  end;
//  
//  if G.scene.r.imtype=R_MDEC
//  then
//  make_mdec_movies(); 
//end;
//
//
//
//
//procedure animrender; 
//var
//cfrao: integer; 
//name: array [0..Pred(256)] of char; 
//begin
//  
//  if G.scene=0
//  then
//  exit;
//  
//  (* scenedata naar R: (voor backbuf, R.rectx enz) *)
//  R.r:= G.scene.r; 
//  cfrao:= CFRA; 
//  (* START ANIMLUS overal wordt NIET de cfra uit R.r gebruikt: ivm rest blender *)
//  
//  if G.scene.r.scemode and R_OGL
//  then
//  R.r.mode:= R.r.mode and ( not R_PANORAMA); 
//  if R.r.imtype=R_MOVIE
//  then
//  begin 
//    R.rectx:= (R.r.size*R.r.xsch) div 100; 
//    R.recty:= (R.r.size*R.r.ysch) div 100; 
//    if R.r.mode and R_PANORAMA
//    then
//    begin 
//      R.rectx:= R.rectx * (R.r.xparts); 
//      R.recty:= R.recty * (R.r.yparts); 
//    end;
//    {$ifdef __sgi}
//    start_movie(); 
//    {$endif}
//  end;
//  else
//  
//  function ELEM3{!!!3 unknown typedef}: if; 
//  begin
//    R.rectx:= (R.r.size*R.r.xsch) div 100; 
//    R.recty:= (R.r.size*R.r.ysch) div 100; 
//    if R.r.mode and R_PANORAMA
//    then
//    begin 
//      R.rectx:= R.rectx * (R.r.xparts); 
//      R.recty:= R.recty * (R.r.yparts); 
//    end;
//    start_avi(); 
//  end
//  
//  
//  (* in de gaten houden: backgroundrender !!! *)
//  (* if(R.r.scemode & R_NETWORK) init_render_camera_network(); *)
//  
//  for{while} CFRA:=SFRA to EFRA { CFRA++}
//  do
//  begin 
//    
//    R.flag:= R_ANIMRENDER; 
//    initrender(); 
//    
//    (* SCHRIJF PLAATJE *)
//    if test_break()=0
//    then
//    begin 
//      
//      if R.r.imtype=R_MOVIE
//      then
//      {$ifdef __sgi}
//      begin 
//        append_movie(CFRA); 
//        {$endif}
//      end;
//      else
//      if ELEM3(R.r.imtype,R_AVIRAW,R_AVIJPEG,R_AVIJMF)
//      then
//      begin 
//        append_avi(CFRA); 
//      end;
//      else
//      begin 
//        makepicstring(name,CFRA); 
//        schrijfplaatje(name); 
//        if test_break()=0
//        then
//        printf('Saved: %s',name); 
//      end;
//      timestr(G.cputime,name); 
//      
//      printf(' Time: %s (%.2f)\n',name,({!!!a type cast? =>} {float(}(G.time-G.cputime)) div 100); 
//      fflush(stdout); 
//    end;
//    (* nodig voor renderd !! *)
//    
//    if G.afbreek=1
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    
//  end;
//  CFRA:= cfrao; 
//  
//  (* restoren tijd *)
//  if R.r.mode and (R_FIELDS or R_MBLUR)
//  then
//  begin 
//    do_all_ipos(); 
//    do_all_keys(); 
//    do_all_ikas(); 
//  end;
//  
//  if R.r.imtype=R_MOVIE
//  then
//  {$ifdef __sgi}
//  begin 
//  end_movie(); 
//  {$endif}
//end;
//else
//
//function ELEM3{!!!3 unknown typedef}: if; 
//begin
//end_avi(); 
//end;
//if G.background=0
//then
//exit_render_stuff(); 
//end;(* voor end_camera_net *)
//
//(* if(R.r.scemode & R_NETWORK) end_render_camera_network(); *)
//

(* *************************************************** *)
(* ******************* Screendumps ******************** *)

var
dumprect: pcardinal = nil;
dumpsx: integer;
dumpsy: integer;
dumptype: integer;


procedure write_screendump(name: pchar);
//var
//ibuf: pImBuf;
begin
   if dumprect<>nil then
  begin
    //strcpy(G.ima,name);
    //convertstringcode(name);
    //
    //if saveover(name) then
    //begin
    //  waitcursor(1);
    //  ibuf:= allocImBuf(dumpsx,dumpsy,24,0,0);
    //  ibuf.rect:= dumprect;
    //  if G.scene.r.imtype=R_IRIS then
    //  ibuf.ftype:= IMAGIC;
    //  else
    //  if G.scene.r.imtype=R_IRIZ then
    //  ibuf.ftype:= IMAGIC;
    //  else
    //  if G.scene.r.imtype=R_TARGA then
    //  ibuf.ftype:= TGA;
    //  else
    //  if G.scene.r.imtype=R_RAWTGA then
    //  ibuf.ftype:= RAWTGA;
    //  else
    //  if G.scene.r.imtype=R_HAMX then
    //  ibuf.ftype:= AN_hamx;
    //  else
    //  if ELEM5(G.scene.r.imtype,R_MOVIE,R_AVIRAW,R_AVIJPEG,R_AVIJMF,R_JPEG90) then
    //  begin
    //    ibuf.ftype:= JPG or G.scene.r.quality;
    //  end;
    //  else
    //  ibuf.ftype:= TGA;
    //  saveiff(ibuf,name,IB_rect);
    //
    //  freeImBuf(ibuf);
    //  waitcursor(0);
    //end;
    //freeN(dumprect);
    //dumprect:= nil;
  end;
end;


procedure screendump;
var
  x: integer=0;
  y: integer=0;
  imstr: array [0..31] of char;
  win: pbWindow;
begin
  (* dump pakken van frontbuffer *)

  dumpsx:= 0;
  dumpsy:= 0;

  if dumprect<>nil  then
  freeN(dumprect);
  dumprect:= nil;

  if (G.qual and LR_SHIFTKEY)<>0  then
  begin
    x:= 0;
    y:= 0;

    dumpsx:= G.curscreen^.sizex;
    dumpsy:= G.curscreen^.sizey;
  end
  else
  begin
    win:=swinarray[mywinget()];
    if win = nil then
    exit;

    if mywin_inmenu()<>0 then
    begin
      mywin_getmenu_rect(@x,@y, @dumpsx, @dumpsy);
    end
    else
    begin
      x:= win^.xmin;
      y:= win^.ymin;
      dumpsx:= win^.xmax-win^.xmin+1;
      dumpsy:= win^.ymax-win^.ymin+1;
    end;
  end;

  if (dumpsx<>0)and(dumpsy<>0) then
  begin
    dumptype:= G.scene^.r.imtype;
    save_image_filesel_str(imstr);

    dumprect:= mallocN(sizeof(integer)*dumpsx*dumpsy,'dumprect');
    glReadBuffer(GL_FRONT);
    glReadPixels(x,y,dumpsx,dumpsy,GL_RGBA,GL_UNSIGNED_BYTE,dumprect);

    (* filesel openen *)
    activate_fileselect(FILE_SPECIAL,imstr,G.ima,@write_screendump);
  end;
end;


end.
