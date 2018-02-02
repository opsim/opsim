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

(*  imageprocess.c        MIXED MODEL
 *
 *  april 95
 *
 *)

unit imageprocess;

interface

(* uit render.c *)
{igamtab1: pushort; }{<= !!!5 external variable}
{igamtab2: pushort; }{<= !!!5 external variable}
{gamtab: pushort; }{<= !!!5 external variable}

procedure convert_rgba_to_abgr(size: integer;  rect: pcardinal);

implementation

//(* maakt premul 255 *)
//
//procedure keyalpha(doel: pchar); 
//var
//c: integer; 
//div: smallint; 
//begin
//  
//  div:= doel[3]; 
//  c:= (doel[0] shl 8) div div; 
//  if c>255
// then
//  doel[0]:= 255; 
//  else
//  doel[0]:= c; 
//  c:= (doel[1] shl 8) div div; 
//  if c>255
// then
//  doel[1]:= 255; 
//  else
//  doel[1]:= c; 
//  c:= (doel[2] shl 8) div div; 
//  if c>255
// then
//  doel[2]:= 255; 
//  else
//  doel[2]:= c; 
//end;
//
//(* vult bron onder doel in met alpha van doel*)
//
//procedure addalphaUnder(doel: pchar;  bron: pchar); 
//var
//c: integer; 
//mul: integer; 
//begin
//  
//  if doel[3]=255
// then
//  exit;
//  if doel[3]=0
// then
//  begin 
//    (* is getest, scheelt  *)
//    *({!!!a type cast? =>} {puint(}doel):=*({!!!a type cast? =>} {puint(}bron); 
//    exit;
//  end;
//  mul:= 255-doel[3]; 
//  c:= doel[0]+((mul*bron[0]) div 255); 
//  
//  if c>255
// then
//  doel[0]:= 255; 
//  else
//  doel[0]:= c; 
//  c:= doel[1]+((mul*bron[1]) div 255); 
//  if c>255
// then
//  doel[1]:= 255; 
//  else
//  doel[1]:= c; 
//  c:= doel[2]+((mul*bron[2]) div 255); 
//  if c>255
// then
//  doel[2]:= 255; 
//  else
//  doel[2]:= c; 
//  c:= doel[3]+((mul*bron[3]) div 255); 
//  if c>255
// then
//  doel[3]:= 255; 
//  else
//  doel[3]:= c; 
//  (* doel[0]= MAX2(doel[0], bron[0]); *)
//end;
//
//(* gamma-gecorr: vult bron onder doel in met alpha van doel *)
//
//procedure addalphaUnderGamma(doel: pchar;  bron: pchar); 
//var
//tot: uint; 
//c: integer; 
//doe: integer; 
//bro: integer; 
//mul: integer; 
//(* hier doel[3]==0 of doel==255 afvangen gebeurt al in skylus *)
//begin
//  
//  
//  
//  
//  mul:= 256-doel[3]; 
//  doe:= igamtab1[doel[0]]; 
//  bro:= igamtab1[bron[0]]; 
//  tot:= (doe+((mul*bro) shr 8)); 
//  if tot>65535
// then
//  tot:= 65535; 
//  doel[0]:= *((gamtab+tot)) shr 8; 
//  doe:= igamtab1[doel[1]]; 
//  bro:= igamtab1[bron[1]]; 
//  tot:= (doe+((mul*bro) shr 8)); 
//  if tot>65535
// then
//  tot:= 65535; 
//  doel[1]:= *((gamtab+tot)) shr 8; 
//  doe:= igamtab1[doel[2]]; 
//  bro:= igamtab1[bron[2]]; 
//  tot:= (doe+((mul*bro) shr 8)); 
//  if tot>65535
// then
//  tot:= 65535; 
//  doel[2]:= *((gamtab+tot)) shr 8; 
//  c:= doel[3]+((mul*bron[3]) div 255); 
//  if c>255
// then
//  doel[3]:= 255; 
//  else
//  doel[3]:= c; 
//  (* doel[0]= MAX2(doel[0], bron[0]); *)
//end;
//
//
//(* doel= bron over doel  *)
//
//procedure addalphaOver(doel: pchar;  bron: pchar); 
//var
//c: integer; 
//mul: integer; 
//begin
//  
//  if bron[3]=0
// then
//  exit;
//  if bron[3]=255
// then
//  begin 
//    (* is getest, scheelt  *)
//    *({!!!a type cast? =>} {puint(}doel):=*({!!!a type cast? =>} {puint(}bron); 
//    exit;
//  end;
//  mul:= 255-bron[3]; 
//  c:= ((mul*doel[0]) div 255)+bron[0]; 
//  
//  if c>255
// then
//  doel[0]:= 255; 
//  else
//  doel[0]:= c; 
//  c:= ((mul*doel[1]) div 255)+bron[1]; 
//  if c>255
// then
//  doel[1]:= 255; 
//  else
//  doel[1]:= c; 
//  c:= ((mul*doel[2]) div 255)+bron[2]; 
//  if c>255
// then
//  doel[2]:= 255; 
//  else
//  doel[2]:= c; 
//  c:= ((mul*doel[3]) div 255)+bron[3]; 
//  if c>255
// then
//  doel[3]:= 255; 
//  else
//  doel[3]:= c; 
//end;
//
//(* telt bron bij doel *)
//
//procedure addalphaAdd(doel: pchar;  bron: pchar); 
//var
//c: integer; 
//mul: integer; 
//begin
//  
//  if doel[3]=0)or(bron[3]=255
// then
//  begin 
//    (* is getest, scheelt veel *)
//    *({!!!a type cast? =>} {puint(}doel):=*({!!!a type cast? =>} {puint(}bron); 
//    exit;
//  end;
//  c:= doel[0]+bron[0]; 
//  if c>255
// then
//  doel[0]:= 255; 
//  else
//  doel[0]:= c; 
//  c:= doel[1]+bron[1]; 
//  if c>255
// then
//  doel[1]:= 255; 
//  else
//  doel[1]:= c; 
//  c:= doel[2]+bron[2]; 
//  if c>255
// then
//  doel[2]:= 255; 
//  else
//  doel[2]:= c; 
//  c:= doel[3]+bron[3]; 
//  if c>255
// then
//  doel[3]:= 255; 
//  else
//  doel[3]:= c; 
//end;
//
//(* telt bron bij doel *)
//
//procedure addalphaAddshort(doel: pushort;  bron: pushort); 
//var
//c: integer; 
//mul: integer; 
//begin
//  
//  if doel[3]=0
// then
//  begin 
//    *({!!!a type cast? =>} {puint(}doel):=*({!!!a type cast? =>} {puint(}bron); 
//    *({!!!a type cast? =>} {puint(}(doel+2)):=*({!!!a type cast? =>} {puint(}(bron+2)); 
//    exit;
//  end;
//  c:= doel[0]+bron[0]; 
//  if c>65535
// then
//  doel[0]:= 65535; 
//  else
//  doel[0]:= c; 
//  c:= doel[1]+bron[1]; 
//  if c>65535
// then
//  doel[1]:= 65535; 
//  else
//  doel[1]:= c; 
//  c:= doel[2]+bron[2]; 
//  if c>65535
// then
//  doel[2]:= 65535; 
//  else
//  doel[2]:= c; 
//  c:= doel[3]+bron[3]; 
//  if c>65535
// then
//  doel[3]:= 65535; 
//  else
//  doel[3]:= c; 
//end;
//
//(* ALPHADDFAC: 
// * 
// *  Z= X alphaover Y:
// *  Zrgb= (1-Xa)*Yrgb + Xrgb
// * 
// * Om ook de add te doen moet (1-Xa) moduleren met 1 via fac
// *  (1-fac)*(1-Xa) + fac <=>
// *  1-Xa-fac+fac*Xa+fac <=> 
// *  Xa*(fac-1)+1
// *)
//
//
//(* doel= bron over doel  *)
//
//procedure addalphaAddfac(doel: pchar;  bron: pchar;  addfac: char); 
//var
//c: integer; 
//mul: integer; 
//begin
//  
//  if doel[3]=0
// then
//  begin 
//    *({!!!a type cast? =>} {puint(}doel):=*({!!!a type cast? =>} {puint(}bron); 
//    exit;
//  end;
//  mul:= 255-(bron[3]*(255-addfac)) div 255; 
//  c:= ((mul*doel[0]) div 255)+bron[0]; 
//  
//  if c>255
// then
//  doel[0]:= 255; 
//  else
//  doel[0]:= c; 
//  c:= ((mul*doel[1]) div 255)+bron[1]; 
//  if c>255
// then
//  doel[1]:= 255; 
//  else
//  doel[1]:= c; 
//  c:= ((mul*doel[2]) div 255)+bron[2]; 
//  if c>255
// then
//  doel[2]:= 255; 
//  else
//  doel[2]:= c; 
//  c:= doel[3]+bron[3]; (* c= ((mul*doel[3])/255)+bron[3]; *)
//  if c>255
// then
//  doel[3]:= 255; 
//  else
//  doel[3]:= c; 
//end;
//
//(* doel= bron over doel  *)
//
//procedure addalphaAddfacshort(doel: pushort;  bron: pushort;  addfac: smallint); 
//var
//c: integer; 
//mul: integer; 
//begin
//  
//  if doel[3]=0
// then
//  begin 
//    *({!!!a type cast? =>} {puint(}doel):=*({!!!a type cast? =>} {puint(}bron); 
//    *({!!!a type cast? =>} {puint(}(doel+2)):=*({!!!a type cast? =>} {puint(}(bron+2)); 
//    exit;
//  end;
//  mul:= $FFFF-(bron[0]*(255-addfac)) div 255; 
//  c:= ((mul*doel[0]) shr 16)+bron[0]; 
//  if c>=$FFF0
// then
//  doel[0]:= $FFF0; 
//  else
//  doel[0]:= c; 
//  c:= ((mul*doel[1]) shr 16)+bron[1]; 
//  if c>=$FFF0
// then
//  doel[1]:= $FFF0; 
//  else
//  doel[1]:= c; 
//  c:= ((mul*doel[2]) shr 16)+bron[2]; 
//  if c>=$FFF0
// then
//  doel[2]:= $FFF0; 
//  else
//  doel[2]:= c; 
//  c:= ((mul*doel[3]) shr 16)+bron[3]; 
//  if c>=$FFF0
// then
//  doel[3]:= $FFF0; 
//  else
//  doel[3]:= c; 
//  
//end;
//
//
//
//
//
//procedure clustered_dots(ibuf: pImBuf;  size: integer); 
//var
//fx: single;
//fy: single;
//fsize: single;
//halfsize: single;
//a: integer; 
//b: integer; 
//x: integer; 
//y: integer; 
//tot: integer; 
//rect: puint; 
//cp: pchar; 
//rt: pchar; 
//begin
//  {rgb_to_bw: array [0..] of single; }{<= !!!5 external variable}
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
//  cspace(ibuf,rgb_to_bw); 
//  halfsize:= 0.5*size; 
//  fsize:= 256 div (halfsize*halfsize); 
//  cp:= {!!!a type cast? =>} {pchar(}ibuf.rect; 
//  for{while} y:=0 to Pred(ibuf.y) { y++}
//  do
//  begin 
//    while{for} 
//    x:= 0; 
//    {to} x<ibuf.x
//    {;}
//    inc(x); 
//    cp:= cp + (4); 
//    do
//    begin 
//      fx:= ((x+y) mod size)-halfsize; 
//      fy:= ((1000+y-x) mod size)-halfsize; 
//      tot:= cp[1]+fsize*((fx*fx+fy*fy))-128; 
//      if tot<0
// then
//      tot:= 0; 
//      else
//      if tot>255
// then
//      tot:= 255; 
//      cp[1]:= cp[2]:=cp[3]:=tot; 
//    end;
//  end;
//end;
//
//
//
//procedure scalefastrect(recto: puint;  rectn: puint;  oldx: integer;  oldy: integer;  newx: integer;  newy: integer); 
//var
//rect: puint; 
//newrect: puint; 
//x: integer; 
//y: integer; 
//ofsx: integer; 
//ofsy: integer; 
//stepx: integer; 
//stepy: integer; 
//begin
//  
//  
//  
//  
//  
//  
//  
//  stepx:= (65536.0*(oldx-1.0) div (newx-1.0))+0.5; 
//  stepy:= (65536.0*(oldy-1.0) div (newy-1.0))+0.5; 
//  ofsy:= 32768; 
//  newrect:= rectn; 
//  for{while} y:=newy downto Succ(0) { y--}
//  do
//  begin 
//    rect:= recto; 
//    rect:= rect + ((ofsy shr 16)*oldx); 
//    ofsy:= ofsy + (stepy); 
//    ofsx:= 32768; 
//    for{while} x:=newx downto Succ(0) { x--}
//    do
//    begin 
//      {*}newrect^{++} inc(newrect); :=rect[ofsx shr 16]; 
//      ofsx:= ofsx + (stepx); 
//    end;
//  end;
//end;
//
//const
//CMBB = MAKE_ID('C','M','B','B'); 
//
//
//
//procedure alpha_dither(sbuf: pImBuf;  mode: char); 
//var
//dp: pinteger; 
//dit: array [0..3] of integer; 
//x: integer; 
//y: integer; 
//a: integer; 
//rt: pchar; 
//begin
//  
//  
//  
//  
//  
//  
//  if mode='r'
// then
//  begin 
//    x:= sbuf.x*sbuf.y; 
//    rt:= {!!!a type cast? =>} {pchar(}sbuf.rect; 
//    while x{--} dec(x); 
//    do
//    begin 
//      if rt[0]=0)or(rt[0]=255
// then
//      ; 
//      else
//      begin 
//        a:= R.rt+rt[0]+(rand() and $FF); 
//        if a>128
// then
//        rt[0]:= 255; 
//        else
//        *({!!!a type cast? =>} {puint(}rt):=0; 
//      end;
//      rt:= rt + (4); 
//    end;
//  end;
//  else
//  if mode='2'
// then
//  begin 
//    dit[0]:= $80; 
//    dit[1]:= $00; 
//    dit[2]:= $C0; 
//    dit[3]:= $40; 
//    rt:= {!!!a type cast? =>} {pchar(}sbuf.rect; 
//    for{while} y:=0 to Pred(sbuf.y) { y++}
//    do
//    begin 
//      if y and 1
// then
//      dp:= dit; 
//      else
//      dp:= dit+2; 
//      while{for} 
//      x:= sbuf.x; 
//      {to} x>0
//      {;}
//      dec(x); 
//      rt:= rt + (4); 
//      do
//      begin 
//        if rt[0]=0)or(rt[0]=255
// then
//        ; 
//        else
//        begin 
//          a:= rt[0]+dp[x and 1]; 
//          if a>128
// then
//          rt[0]:= rt[0]:=255; 
//          else
//          *({!!!a type cast? =>} {puint(}rt):=0; 
//        end;
//      end;
//    end;
//  end;
//end;
//
//
//procedure no_isolated_pixels(sbuf: pImBuf;  max: integer); 
//var
//rt: puint; 
//rt1: puint; 
//iso: uint;
// 
//x: integer; 
//y: integer; 
//cp: pchar; 
//begin
//  
//  
//  iso:=0; 
//  
//  
//  
//  if sbuf.x<5)or(sbuf.y<5
// then
//  exit;
//  
//  rt:= sbuf.rect+sbuf.x; 
//  for{while} y:=2 to Pred(sbuf.y) { y++}
//  do
//  begin 
//    inc(rt); 
//    while{for} 
//    x:= 2; 
//    {to} x<sbuf.x
//    {;}
//    inc(x); 
//    inc(rt); 
//    do
//    begin 
//      if {*}rt^)and(rt[-1]=0)and(rt[1]=0
// then
//      begin 
//        rt1:= rt-sbuf.x; 
//        if rt1[-1]=0)and(rt1[0]=0)and(rt1[1]=0
// then
//        begin 
//          rt1:= rt+sbuf.x; 
//          if rt1[-1]=0)and(rt1[0]=0)and(rt1[1]=0
// then
//          begin 
//            cp:= {!!!a type cast? =>} {pchar(}rt; 
//            if cp[1]<max)and(cp[2]<max)and(cp[3]<max
// then
//            begin 
//              inc(iso); 
//              rt[0]:= 0; 
//            end;
//          end;
//        end;
//      end;
//    end;
//  end;
//end;
//
//
//procedure write_cmbb(sbuf: pImBuf;  str: pchar); 
//var
//vbuf: pImBuf; 
//lp: puint; 
//file: integer; 
//x: integer; 
//y: integer; 
//isdone: smallint;
// 
//minx: smallint; 
//miny: smallint; 
//maxx: smallint; 
//maxy: smallint; 
//size: smallint; 
//rt: pchar; 
//begin
//  
//  
//  
//  
//  
//  isdone:=0; 
//  
//  
//  
//  
//  
//  sbuf.cbits:= 6; 
//  alpha_dither(sbuf,'r'); 
//  (* diepte van tabel *)
//  
//  no_isolated_pixels(sbuf,128); 
//  
//  alpha_to_col0(2); 
//  
//  setdither(0); 
//  (* 1: alleen alpha 0,  2: alpha<128 *)
//  converttocmap(sbuf); 
//  x:= sbuf.x*sbuf.y; 
//  lp:= {!!!a type cast? =>} {puint(}sbuf.rect; 
//  rt:= {!!!a type cast? =>} {pchar(}lp; 
//  (* int to char *)
//  while x{--} dec(x); 
//  do
//  begin 
//    {*}rt^:=(char)*lp; 
//    inc(rt); 
//    inc(lp); 
//  end;
//  minx:= sbuf.x-1; 
//  miny:= sbuf.y-1; 
//  maxx:= maxy:=0; 
//  rt:= {!!!a type cast? =>} {pchar(}sbuf.rect; 
//  (* boundbox berekenen *)
//  for{while} y:=0 to Pred(sbuf.y) { y++}
//  do
//  begin 
//    while{for} 
//    x:= 0; 
//    {to} x<sbuf.x
//    {;}
//    inc(x); 
//    inc(rt); 
//    do
//    begin 
//      if {*}rt^
// then
//      begin 
//        if minx>x
// then
//        minx:= x; 
//        if maxx<x
// then
//        maxx:= x; 
//        if miny>y
// then
//        miny:= y; 
//        if maxy<y
// then
//        maxy:= y; 
//        isdone:= 1; 
//      end;
//    end;
//  end;
//  if isdone=0
// then
//  begin 
//    minx:= maxx:=sbuf.x div 2; 
//    miny:= maxy:=sbuf.y div 2; 
//  end;
//  if minx and 1
// then
//  dec(minx); 
//  if miny and 1
// then
//  dec(miny); 
//  if maxx and 1
// then
//  ; 
//  else
//  inc(maxx); 
//  if maxy and 1
// then
//  ; 
//  else
//  inc(maxy); 
//  file:= open(str,O_BINARY or O_WRONLY+O_CREAT+O_TRUNC,$1B6); (* schrijven *)
//  if file=-1
// then
//  begin 
//    error('Can''t write file');
//    exit;
//  end;
//  x:= CMBB; 
//  write(file,@x,4); 
//  
//  size:= maxx-minx+1; 
//  write(file,@size,2); 
//  size:= maxy-miny+1; 
//  write(file,@size,2); 
//  size:= (minx+maxx) div 2; 
//  size:= size - ((sbuf.x div 2)); 
//  write(file,@size,2); 
//  size:= sbuf.y-1-((miny+maxy) div 2); 
//  size:= size - ((sbuf.y div 2)); 
//  write(file,@size,2); 
//  rt:= ({!!!a type cast? =>} {pchar(}sbuf.rect)+sbuf.x*maxy+minx; 
//  size:= maxx-minx+1; 
//  (* y-geflipt wegschrijven *)
//  for{while} y:=miny to maxy { y++}
//  do
//  begin 
//    write(file,rt,size); 
//    rt:= rt - (sbuf.x); 
//  end;
//  close(file); 
//end;



procedure convert_rgba_to_abgr(size: integer;  rect: pcardinal);
var
cp: pchar;
rt: char; 
begin
  cp:=pchar(rect);
  
  while size >0   do
  begin 
    rt:= cp[0]; 
    cp[0]:= cp[3]; 
    cp[3]:= rt; 
    rt:= cp[1]; 
    cp[1]:= cp[2]; 
    cp[2]:= rt; 
    cp:= cp + (4);

    dec(size);
  end;
end;


end.
