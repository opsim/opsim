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


(*  font.c      MIXED MODEL
 *
 *  maart 95
 *
 *
 *)

unit font;

interface

procedure load_firstfont;

implementation

uses
strings,
cfuncs,
blender, blendef, file_, readfile, objfnt, screen, psfont;

//{$include "blender.h"}
//{$include "objfnt.h"}
//{$include "datatoc.h"}
//{$include "file.h"}
//
//const
//MAX_VF_CHARS = 256; 
//
//(* Nieuwe opzet voor vectorfont:
// *
// * geen PDrawfont meer, alles meteen naar Nurb en BezTriple
// *
// *)
//
//type
//chartrans = record
//xof: float; 
//yof: float; 
//rot: float; 
//linenr: smallint; 
//charnr: smallint; 
//end;
//
//procedure free_vfont(vf: pVFont); 
//var
//nu: pNurb; 
//bezt: pBezTriple; 
//i: integer; 
//begin
//  
//  
//  if vf=0
//  then
//  exit;
//  
//  for{while} i:=0 to Pred(MAX_VF_CHARS) { i++}
//  do
//  begin 
//    while nu:=vf.data.nurbsbase[i].first
//    do
//    begin 
//      bezt:= nu.bezt; 
//      if bezt<>0{nil} {<= !!!9} 
//      then
//      freeN(bezt); 
//      remlink(@vf.data.nurbsbase[i],nu); 
//      freeN(nu); 
//    end;
//  end;
//  
//  if vf.data.kern<>0{nil} {<= !!!9} 
//  then
//  freeN(vf.data.kern); 
//  freeN(vf.data); 
//end;
//
//
//
//type
//Nurb = record
//end;
//BezTriple = record
//end;
//
//function objfnt_to_vfont(fnt: pobjfnt;  vfont: pVFont): integer; 
//var
//vfd: pVFontData; 
//cd: pchardesc; 
//_data: psmallint; 
//data: psmallint; 
//a: integer; 
//i: integer; 
//count: integer; 
//stop: integer; 
//ready: integer; 
//meet: integer; 
//first: array [0..Pred(2)] of smallint; 
//last: array [0..Pred(2)] of smallint; 
//scale: float; 
//dx: float; 
//dy: float; 
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
//  if fnt=0
//  then
//  begin
//    result:= (0); 
//    exit;
//    
//  end;
//  if fnt.type<>SP_TYPE
//  then
//  begin 
//    printf('objfnt is wrong type: need splines\n');
//    begin
//      result:= (0); 
//      exit;
//    end;
//  end;
//  scale:= 10.0 div {!!!a type cast? =>} {float(}fnt.scale; 
//  vfd:= vfont.data; (* na IRIX 6.2, schaal klopte niet meer *)
//  for{while} i:=0 to Pred(MAX_VF_CHARS) { i++}
//  do
//  begin 
//    cd:= getchardesc(fnt,i); 
//    if cd)and(cd.data)and(cd.datalen
//    then
//    begin 
//      vfd.width[i]:= scale*cd.movex; 
//      _data:= data:=cd.data; 
//      repeat
//      
//      
//      begin 
//        (* eerst even tellen *)
//        _data:= data; 
//        count:= 0; 
//        ready:= stop:=LongBool(0); 
//        repeat
//        
//        begin 
//          case {*}data^{++} inc(data);  of
//            SP_MOVETO:
//            begin
//              first[0]:= data[0]; 
//              first[1]:= data[1]; 
//              {goto next_label;}{<= !!!d case label without "break"}
//            end;
//            SP_LINETO:
//            begin
//              inc(count); 
//              last[0]:= data[0]; 
//              last[1]:= data[1]; 
//              data:= data + (2); 
//            end;
//            SP_CURVETO:
//            begin
//              inc(count); 
//              last[0]:= data[4]; 
//              last[1]:= data[5]; 
//              data:= data + (6); 
//            end;
//            SP_RET,
//            SP_RETCLOSEPATH:
//            begin
//              stop:= LongBool(1); 
//              ready:= LongBool(1); 
//            end;
//            SP_CLOSEPATH:
//            begin
//              stop:= LongBool(1); 
//            end;
//            
//          end;{case?}
//        end;
//        until not {0=}(stop=LongBool(0));
//        
//        if last[0]=first[0])and(last[1]=first[1]
//        then
//        meet:= 1; 
//        else
//        meet:= LongBool(0); 
//        (* is er meer dan 1 uniek punt ?*)
//        
//        if count-meet>0
//        then
//        begin 
//          data:= _data; 
//          nu:= callocstructN(structNurb,1,'objfnt_nurb'); 
//          bezt:= callocstructN(structBezTriple,count,'objfnt_bezt'); 
//          if nu<>0)and(bezt<>0
//          then
//          begin 
//            addtail(@vfd.nurbsbase[i],nu); 
//            nu.type:= CU_BEZIER+CU_2D; 
//            nu.pntsu:= count; 
//            nu.resolu:= 8; 
//            nu.flagu:= 1; 
//            nu.bezt:= bezt; 
//            stop:= LongBool(0); 
//            repeat
//            
//            (* punten inlezen *)
//            begin 
//              case {*}data^{++} inc(data);  of
//                SP_MOVETO:
//                begin
//                  bezt.vec[1][0]:= scale**data{++} inc(data); ; 
//                  bezt.vec[1][1]:= scale**data{++} inc(data); ; 
//                end;
//                SP_LINETO:
//                begin
//                  bez2:= bezt{++} inc(bezt); ; 
//                  bezt.vec[1][0]:= scale**data{++} inc(data); ; 
//                  bezt.vec[1][1]:= scale**data{++} inc(data); ; 
//                  bezt.h1:= HD_VECT; 
//                  bez2.h2:= HD_VECT; 
//                  dx:= (bezt.vec[1][0]-bez2.vec[1][0]) div 3.0; 
//                  dy:= (bezt.vec[1][1]-bez2.vec[1][1]) div 3.0; 
//                  bezt.vec[0][0]:= bezt.vec[1][0]-dx; 
//                  bezt.vec[0][1]:= bezt.vec[1][1]-dy; 
//                  bez2.vec[2][0]:= bez2.vec[1][0]+dx; 
//                  bez2.vec[2][1]:= bez2.vec[1][1]+dy; 
//                end;
//                SP_CURVETO:
//                (* vector handles *)
//                begin
//                  bezt.vec[2][0]:= scale**data{++} inc(data); ; 
//                  bezt.vec[2][1]:= scale**data{++} inc(data); ; 
//                  bezt.h2:= HD_ALIGN; 
//                  inc(bezt); 
//                  bezt.vec[0][0]:= scale**data{++} inc(data); ; 
//                  bezt.vec[0][1]:= scale**data{++} inc(data); ; 
//                  bezt.vec[1][0]:= scale**data{++} inc(data); ; 
//                  bezt.vec[1][1]:= scale**data{++} inc(data); ; 
//                  bezt.h1:= HD_ALIGN; 
//                end;
//                SP_RET,
//                SP_RETCLOSEPATH:
//                begin
//                  stop:= LongBool(1); 
//                  ready:= LongBool(1); 
//                end;
//                SP_CLOSEPATH:
//                begin
//                  stop:= LongBool(1); 
//                end;
//                
//              end;{case?}
//            end;
//            until not {0=}(stop=LongBool(0));
//            
//            if meet<>0{nil} {<= !!!9} 
//            then
//            begin 
//              (* kopieer handles *)
//              nu.bezt.vec[0][0]:= bezt.vec[0][0]; 
//              nu.bezt.vec[0][1]:= bezt.vec[0][1]; 
//              dec(nu.pntsu); 
//              (* en vergeet laatste punt *)
//            end;
//            else
//            begin 
//              (* vector handles *)
//              bez2:= nu.bezt; 
//              dx:= (bezt.vec[1][0]-bez2.vec[1][0]) div 3.0; 
//              dy:= (bezt.vec[1][1]-bez2.vec[1][1]) div 3.0; 
//              bezt.vec[2][0]:= bezt.vec[1][0]-dx; 
//              bezt.vec[2][1]:= bezt.vec[1][1]-dy; 
//              bez2.vec[0][0]:= bez2.vec[1][0]+dx; 
//              bez2.vec[0][1]:= bez2.vec[1][1]+dy; 
//              bezt.h2:= bez2.h1:=HD_VECT; 
//            end;
//            a:= nu.pntsu; 
//            bezt:= nu.bezt; (* verboden handle combinaties *)
//            while a{--} dec(a); 
//            do
//            begin 
//              if bezt.h1<>HD_ALIGN)and(bezt.h2=HD_ALIGN
//              then
//              bezt.h2:= 0; 
//              else
//              if bezt.h2<>HD_ALIGN)and(bezt.h1=HD_ALIGN
//              then
//              bezt.h1:= 0; 
//              inc(bezt); 
//            end;
//          end;
//          else
//          begin 
//            if nu<>0{nil} {<= !!!9} 
//            then
//            freeN(nu); 
//            if bezt<>0{nil} {<= !!!9} 
//            then
//            freeN(bezt); 
//          end;
//        end;
//        _data:= data; 
//      end;
//      until not {0=}(ready=LongBool(0));
//    end;
//  end;
//  
//  begin
//    result:= (1); 
//    exit;
//  end;
//end;

function load_vfont(name: pchar): pVFont;
var
fnt: pobjfnt;
vfont: pVFont=nil;
ok: integer;
di: array [0..Pred(FILE_MAXDIR)] of char;
fi: array [0..Pred(FILE_MAXFILE)] of char;
begin
  waitcursor(1);

  //fnt:= loadpostcriptfont(name);
  //if fnt<>nil  then
  //begin
  //  strcpy(di,name);
  //  splitdirstring(di,fi);
  //
  //  vfont:= alloc_libblock(@G.main.vfont,ID_VF,fi);
  //  vfont.data:= callocN(sizeof(VFontData),'Vfontdata');
  //
  //  ok:= objfnt_to_vfont(fnt,vfont);
  //  freeobjfnt(fnt);
  //
  //  if ok<>0 then
  //  begin
  //    strncpy(vfont.name,name,sizeof(vfont.name));
  //    vfont.flag:= 0;   (* loaded *)
  //  end
  //  else
  //  begin
  //    free_libblock_us(@G.main.vfont,vfont);
  //  end;
  //end;
  waitcursor(0);

  exit(vfont);
end;

procedure load_firstfont;
var
vf: pVFont;
dir: pchar;
str: array [0..Pred(256)] of char;
begin
  dir:= gethome();
  strcpy(str,dir);
  strcat(str,'/.Bfont');

  vf:= load_vfont(str);
  if vf=nil then
  begin
    //inst_file('.Bfont',datatoc_Bfont,datatoc_Bfont_size);
    vf:= load_vfont(str);
    if vf= nil then
    begin
      printf('Unable to load $HOME/.Bfont\n');
      exit;
    end;
  end;

  vf^.id.us:= 0;
end;

//(* komt uit readile *)
//type
//objfnt = record
//end;
//
//procedure reload_vfont(vfont: pVFont); 
//begin
//  
//  
//  function loadpostcriptfont{!!!3 unknown typedef}: pobjfnt; 
//  
//  fnt:= loadpostcriptfont(vfont.name); 
//  if fnt<>0{nil} {<= !!!9} 
//  then
//  begin 
//    objfnt_to_vfont(fnt,vfont); 
//    freeobjfnt(fnt); 
//  end;
//  vfont.flag:= 0; 
//  (* loaded *)
//end;
//
//
//
//procedure buildchar(cu: pCurve;  ascii: uchar;  ofsx: float;  ofsy: float;  rot: float); 
//var
//bezt1: pBezTriple; 
//bezt2: pBezTriple; 
//nu1: pNurb; 
//nu2: pNurb; 
//fp: pfloat; 
//fsize: float; 
//shear: float; 
//x: float; 
//si: float; 
//co: float; 
//i: integer; 
//(* maak een kopie op afstand ofsx, ofsy met shear*)
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
//  fsize:= cu.fsize; 
//  shear:= cu.shear; 
//  si:= fsin(rot); 
//  co:= fcos(rot); 
//  nu1:= cu.vfont.data.nurbsbase[ascii].first; 
//  while nu1
//  do
//  begin 
//    bezt1:= nu1.bezt; 
//    if bezt1<>0{nil} {<= !!!9} 
//    then
//    begin 
//      nu2:= mallocstructN(structNurb,1,'duplichar_nurb'); 
//      if nu2=0
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      memcpy(nu2,nu1,sizeof(structNurb)); 
//      nu2.resolu:= cu.resolu; 
//      nu2.bp:= 0; 
//      nu2.knotsu:= nu2.knotsv:=0; 
//      nu2.flag:= ME_SMOOTH; 
//      i:= nu2.pntsu; 
//      bezt2:= mallocstructN(structBezTriple,i,'duplichar_bezt2'); (* nu2->trim.first = 0; *)
//      (* nu2->trim.last = 0; *)
//      
//      if bezt2=0
//      then
//      begin 
//        freeN(nu2); 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      memcpy(bezt2,bezt1,i*sizeof(structBezTriple)); 
//      nu2.bezt:= bezt2; 
//      if shear<>0.0
//      then
//      begin 
//        bezt2:= nu2.bezt; 
//        for{while} i:=nu2.pntsu downto Succ(0) { i--}
//        do
//        begin 
//          bezt2.vec[0][0]:= bezt2.vec[0][0] + (shear*bezt2.vec[0][1]); 
//          bezt2.vec[1][0]:= bezt2.vec[1][0] + (shear*bezt2.vec[1][1]); 
//          bezt2.vec[2][0]:= bezt2.vec[2][0] + (shear*bezt2.vec[2][1]); 
//          inc(bezt2); 
//        end;
//      end;
//      if rot<>0.0
//      then
//      begin 
//        bezt2:= nu2.bezt; 
//        for{while} i:=nu2.pntsu downto Succ(0) { i--}
//        do
//        begin 
//          fp:= bezt2.vec[0]; 
//          x:= fp[0]; 
//          fp[0]:= co*x+si*fp[1]; 
//          fp[1]:= -si*x+co*fp[1]; 
//          x:= fp[3]; 
//          fp[3]:= co*x+si*fp[4]; 
//          fp[4]:= -si*x+co*fp[4]; 
//          x:= fp[6]; 
//          fp[6]:= co*x+si*fp[7]; 
//          fp[7]:= -si*x+co*fp[7]; 
//          inc(bezt2); 
//          
//        end;
//      end;
//      bezt2:= nu2.bezt; 
//      for{while} i:=nu2.pntsu downto Succ(0) { i--}
//      do
//      begin 
//        fp:= bezt2.vec[0]; 
//        fp[0]:= (fp[0]+ofsx)*fsize; 
//        fp[1]:= (fp[1]+ofsy)*fsize; 
//        fp[3]:= (fp[3]+ofsx)*fsize; 
//        fp[4]:= (fp[4]+ofsy)*fsize; 
//        fp[6]:= (fp[6]+ofsx)*fsize; 
//        fp[7]:= (fp[7]+ofsy)*fsize; 
//        inc(bezt2); 
//        
//      end;
//      addtail(@(cu.nurb),nu2); 
//    end;
//    nu1:= nu1.next; 
//  end;
//end;
//
//
//type
//chartrans = record
//end;
//var
//cu: pCurve; 
//cucu: pCurve; 
//type
//chartrans = record
//end;
//var
//distfac: float; 
//tabfac: float; 
//ctime: float; 
//dtime: float; 
//tvec: array [0..Pred(4)] of float; 
//vec: array [0..Pred(4)] of float; 
//rotvec: array [0..Pred(3)] of float; 
//minx: float; 
//maxx: float; 
//miny: float; 
//maxy: float; 
//cmat: array [0..Pred(3),0..Pred(3)] of float; 
//timeofs: float; 
//si: float; 
//co: float; 
//sizefac: float; 
//f: pfloat; 
//maxlen: float = 0; 
//xof: float; 
//yof: float; 
//xtrax: float; 
//linedist: float; 
//ld: pfloat; 
//linedata: pfloat; 
//linedata2: pfloat; 
//len: integer; 
//i: integer; 
//slen: integer; 
//oldflag: integer; 
//kern: psmallint; 
//kcount: smallint; 
//str: smallint; 
//cnr: smallint = 0; 
//lnr: smallint = 0; 
//ascii: uchar; 
//mem: puchar; 
//(* opmerking: berekeningen altijd tot en met de '\0' van de string omdat
//    de cursor op die plek moet kunnen staan *)
//
//if ob.type<>OB_FONT
//then
//begin
//  result:= 0; 
//  exit;
//  
//end;
//cu:= ob.data; 
//vfont:= cu.vfont; 
//if vfont=0
//then
//begin
//  result:= 0; 
//  exit;
//end;
//if cu.str=0
//then
//begin
//  result:= 0; 
//  exit;
//  
//  (* aantal regels tellen *)
//end;
//mem:= cu.str; 
//slen:= lstrlen(mem); 
//cu.lines:= 1; 
//while{for} 
//i:= 0; 
//{to} i<=slen
//{;}
//inc(i); 
//inc(mem); 
//do
//begin 
//  ascii:= {*}mem^; 
//  if ascii='\n')or(ascii='\r'
//  then
//  inc(cu.lines); 
//end;
//ct:= chartransdata:=callocstructN(structchartrans,slen+1,'buildtext'); 
//linedata:= mallocN(sizeof(float)*cu.lines,'buildtext2'); 
//linedata2:= mallocN(sizeof(float)*cu.lines,'buildtext2'); 
//xof:= cu.xof; 
//yof:= cu.yof; 
//xtrax:= 0.5*cu.spacing-0.5; 
//linedist:= cu.linedist; 
//(* bereken ofset en rotatie van iedere letter *)
//
//
//for{while} i:=0 to slen { i++}
//do
//begin 
//  ascii:= cu.str[i]; 
//  if ascii='\n')or(ascii='\r')or(ascii=0
//  then
//  begin 
//    ct.xof:= xof; 
//    ct.yof:= yof; 
//    ct.linenr:= lnr; 
//    ct.charnr:= cnr; (* alleen lege regels mogen kleiner dan 1 zijn *)
//    if linedist<1.0
//    then
//    begin 
//      if i<slen)and((cu.str[i+1]='\r')or(cu.str[i+1]='\n')
//      then
//      yof:= yof - (linedist); 
//      else
//      yof:= yof - (1.0); 
//    end;
//    else
//    yof:= yof - (linedist); 
//    maxlen:= MAX2(maxlen,xof); 
//    linedata[lnr]:= xof; 
//    linedata2[lnr]:= cnr; 
//    xof:= cu.xof; 
//    inc(lnr); 
//    cnr:= 0; 
//  end;
//  else
//  if ascii=9
//  then
//  begin 
//    (* TAB *)
//    ct.xof:= xof; 
//    ct.yof:= yof; 
//    ct.linenr:= lnr; 
//    ct.charnr:= cnr{++} inc(cnr); ; 
//    tabfac:= (xof-cu.xof+0.$1); 
//    tabfac:= 2.0*fceil(tabfac div 2.0); 
//    xof:= cu.xof+tabfac; 
//    
//  end;
//  else
//  begin 
//    ct.xof:= xof; 
//    ct.yof:= yof; 
//    ct.linenr:= lnr; 
//    ct.charnr:= cnr{++} inc(cnr); ; 
//    xof:= xof + (vfont.data.width[ascii]+xtrax); 
//    
//    if vfont.data.kern<>0{nil} {<= !!!9} 
//    then
//    begin 
//      kern:= vfont.data.kern; 
//      str:= (ascii shl 8)+cu.str[i+1]; 
//      for{while} kcount:=vfont.data.kcount downto Succ(0) { kcount--}
//      do
//      begin 
//        if {*}kern^{++} inc(kern); =str
//        then
//        begin 
//          xof:= xof + ({*}kern^); 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        inc(kern); 
//      end;
//    end;
//  end;
//  inc(ct); 
//end;
//
//(* met alle fontsettings plekken letters berekenen *)
//if cu.spacemode<>CU_LEFT)and(lnr>1
//then
//begin 
//  ct:= chartransdata; 
//  if cu.spacemode=CU_RIGHT
//  then
//  begin 
//    for{while} i:=0 to Pred(lnr) {i++}
//    do
//    linedata[i]:= maxlen-linedata[i]; 
//    for{while} i:=0 to slen { i++}
//    do
//    begin 
//      ct.xof:= ct.xof + (linedata[ct.linenr]); 
//      inc(ct); 
//    end;
//  end;
//  else
//  if cu.spacemode=CU_MIDDLE
//  then
//  begin 
//    for{while} i:=0 to Pred(lnr) {i++}
//    do
//    linedata[i]:= (maxlen-linedata[i]) div 2; 
//    for{while} i:=0 to slen { i++}
//    do
//    begin 
//      ct.xof:= ct.xof + (linedata[ct.linenr]); 
//      inc(ct); 
//    end;
//  end;
//  else
//  if cu.spacemode=CU_FLUSH
//  then
//  begin 
//    for{while} i:=0 to Pred(lnr) {i++}
//    do
//    if linedata2[i]>1
//    then
//    linedata[i]:= (maxlen-linedata[i]) div (linedata2[i]-1); 
//    for{while} i:=0 to slen { i++}
//    do
//    begin 
//      ct.xof:= ct.xof + (ct.charnr*linedata[ct.linenr]); 
//      inc(ct); 
//    end;
//  end;
//end;
//(* TEXT ON CURVE *)
//if cu.textoncurve<>0{nil} {<= !!!9} 
//then
//begin 
//  cucu:= cu.textoncurve.data; 
//  oldflag:= cucu.flag; 
//  cucu.flag:= cucu.flag or ((CU_PATH+CU_FOLLOW)); 
//  if cucu.path=0
//  then
//  calc_curvepath(cu.textoncurve); 
//  if cucu.path<>0{nil} {<= !!!9} 
//  then
//  begin 
//    
//    Mat3CpyMat4(cmat,cu.textoncurve.obmat); 
//    sizefac:= Normalise(cmat[0]) div cu.fsize; 
//    minx:= miny:=1.0e20; 
//    maxx:= maxy:=-1.0e20; 
//    ct:= chartransdata; 
//    while{for} 
//    i:= 0; 
//    {to} i<=slen
//    {;}
//    inc(i); 
//    inc(ct); 
//    do
//    begin 
//      if minx>ct.xof
//      then
//      minx:= ct.xof; 
//      if maxx<ct.xof
//      then
//      maxx:= ct.xof; 
//      if miny>ct.yof
//      then
//      miny:= ct.yof; 
//      if maxy<ct.yof
//      then
//      maxy:= ct.yof; 
//    end;
//    distfac:= sizefac*cucu.path.totdist div (maxx-minx); 
//    timeofs:= 0.0; (* we zetten de x-coordinaat exact op de curve, de y wordt geroteerd *)
//    (* de lengte correctie *)
//    if distfac>1.0
//    then
//    begin 
//      (* pad langer dan tekst: spacemode doet mee *)
//      distfac:= 1.0 div distfac; 
//      if cu.spacemode=CU_RIGHT
//      then
//      begin 
//        timeofs:= 1.0-distfac; 
//      end;
//      else
//      if cu.spacemode=CU_MIDDLE
//      then
//      begin 
//        timeofs:= (1.0-distfac) div 2.0; 
//      end;
//      else
//      if cu.spacemode=CU_FLUSH
//      then
//      distfac:= 1.0; 
//    end;
//    else
//    distfac:= 1.0; 
//    distfac:= distfac div ((maxx-minx)); 
//    timeofs:= timeofs + (distfac*cu.xof); 
//    ct:= chartransdata; (* niet cyclic *)
//    while{for} 
//    i:= 0; 
//    {to} i<=slen
//    {;}
//    inc(i); 
//    inc(ct); 
//    do
//    begin 
//      (* roteren rond centrum letter *)
//      ascii:= cu.str[i]; 
//      dtime:= distfac*0.35*vfont.data.width[ascii]; 
//      dtime:= distfac*0.0*vfont.data.width[ascii]; 
//      ctime:= timeofs+distfac*(ct.xof-minx); 
//      CLAMP(ctime,0.0,1.0); 
//      (* Waarom is 0.5 te groot? *)
//      (* Waarom is 0.5 te groot? *)
//      where_on_path(cu.textoncurve,ctime,vec,tvec); 
//      (* de goede plek EN de goede rotatie apart berekenen *)
//      where_on_path(cu.textoncurve,ctime+dtime,tvec,rotvec); 
//      VecMulf(vec,sizefac); 
//      ct.rot:= M_PI-fatan2(rotvec[1],rotvec[0]); 
//      si:= fsin(ct.rot); 
//      co:= fcos(ct.rot); 
//      yof:= ct.yof; 
//      ct.xof:= vec[0]+si*yof; 
//      ct.yof:= vec[1]+co*yof; 
//      
//      
//    end;
//    cucu.flag:= oldflag; 
//  end;
//end;
//
//
//if mode=FO_CURSUP)or(mode=FO_CURSDOWN
//then
//begin 
//  (* 2: curs omhoog
//       3: curs omlaag *)
//  ct:= chartransdata+cu.pos; 
//  if mode=FO_CURSUP)and(ct.linenr=0
//  then
//  ; 
//  else
//  if mode=FO_CURSDOWN)and(ct.linenr=lnr
//  then
//  ; 
//  else
//  begin 
//    if mode=FO_CURSUP
//    then
//    lnr:= ct.linenr-1; 
//    else
//    lnr:= ct.linenr+1; 
//    cnr:= ct.charnr; 
//    cu.pos:= 0; 
//    ct:= chartransdata; (* zoek karakter met lnr en cnr *)
//    for{while} i:=0 to Pred(slen) { i++}
//    do
//    begin 
//      if ct.linenr=lnr
//      then
//      begin 
//        if ct.charnr=cnr
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        if (ct+1).charnr=0
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      else
//      if ct.linenr>lnr
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      inc(cu.pos); 
//      inc(ct); 
//    end;
//  end;
//end;
//(* eerst cursor *)
//if ob=G.obedit
//then
//begin 
//  ct:= chartransdata+cu.pos; 
//  si:= fsin(ct.rot); 
//  co:= fcos(ct.rot); 
//  f:= G.textcurs[0]; 
//  f[0]:= cu.fsize*(-0.1*co+ct.xof); 
//  f[1]:= cu.fsize*(0.1*si+ct.yof); 
//  f[2]:= cu.fsize*(0.1*co+ct.xof); 
//  f[3]:= cu.fsize*(-0.1*si+ct.yof); 
//  f[4]:= cu.fsize*(0.1*co+0.8*si+ct.xof); 
//  f[5]:= cu.fsize*(-0.1*si+0.8*co+ct.yof); 
//  f[6]:= cu.fsize*(-0.1*co+0.8*si+ct.xof); 
//  f[7]:= cu.fsize*(0.1*si+0.8*co+ct.yof); 
//end;
//
//if mode=0
//then
//begin 
//  (* nurbdata maken *)
//  freeNurblist( and cu.nurb); 
//  ct:= chartransdata; 
//  for{while} i:=0 to Pred(slen) { i++}
//  do
//  begin 
//    ascii:= cu.str[i]; 
//    buildchar(cu,ascii,ct.xof,ct.yof,ct.rot); 
//    inc(ct); 
//  end;
//end;
//freeN(linedata); 
//
//freeN(linedata2); 
//
//if mode=FO_DUPLI
//then
//begin 
//  begin
//    result:= chartransdata; 
//    exit;
//  end;
//end;
//freeN(chartransdata); 
//
//begin
//  result:= 0; 
//  exit;
//end;
//end;{case?}
//
//
//(* ***************** DUPLI  ***************** *)
//
//
//function find_family_object{!!!3 unknown typedef}: pObject; 
//var
//ob: pObject; 
//flen: integer; 
//begin
//  
//  
//  if obar[ch]<>0{nil} {<= !!!9} 
//  then
//  begin
//    result:= obar[ch]; 
//    exit;
//  end;
//  flen:= lstrlen(family); 
//  ob:= G.main.object.first; 
//  while ob
//  do
//  begin 
//    if ob.id.name[flen+2]=ch
//    then
//    begin 
//      if strncmp(ob.id.name+2,family,flen)=0
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    ob:= ob.id.next; 
//  end;
//  obar[ch]:= ob; 
//  begin
//    result:= ob; 
//    exit;
//  end;
//end;
//
//
//type
//chartrans = record
//end;
//
//procedure font_duplilist(par: pObject); 
//var
//ob: pObject; 
//new: pObject; 
//obar: array [0..Pred(256)] of pObject; 
//cu: pCurve; 
//vec: array [0..Pred(3)] of float; 
//pmat: array [0..Pred(4),0..Pred(4)] of float; 
//fsize: float; 
//xof: float; 
//yof: float; 
//q2: pfloat; 
//lay: integer; 
//slen: integer; 
//a: integer; 
//begin
//  {duplilist: ListBase; }{<= !!!5 external variable}
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
//  Mat4CpyMat4(pmat,par.obmat); 
//  lay:= G.scene.lay; 
//  if G.vd<>0{nil} {<= !!!9} 
//  then
//  lay:= G.vd.lay; 
//  chartransdata:= text_to_curve(par,FO_DUPLI); (* in par staat een familienaam, deze gebruiken om objecten te vinden *)
//  
//  if chartransdata=0
//  then
//  exit;
//  bzero(obar,256*4); 
//  cu:= par.data; 
//  slen:= lstrlen(cu.str); 
//  fsize:= cu.fsize; 
//  xof:= cu.xof; 
//  yof:= cu.yof; 
//  ct:= chartransdata; 
//  set_displist_onlyzero(1); 
//  
//  while{for} 
//  a:= 0; 
//  {to} a<slen
//  {;}
//  inc(a); 
//  inc(ct); 
//  do
//  begin 
//    ob:= find_family_object(obar,cu.family,cu.str[a]); 
//    if ob<>0{nil} {<= !!!9} 
//    then
//    begin 
//      makeDispList(ob); 
//      vec[0]:= fsize*(ct.xof-xof); 
//      vec[1]:= fsize*(ct.yof-yof); 
//      vec[2]:= 0.0; 
//      Mat4MulVecfl(pmat,vec); 
//      new:= mallocN(sizeof(Object),'newobj dupli'); 
//      memcpy(new,ob,sizeof(Object)); 
//      new.flag:= new.flag or (OB_FROMDUPLI); 
//      new.id.new:= {!!!a type cast? =>} {pID(}par; 
//      new.totcol:= par.totcol; 
//      Mat4CpyMat4(new.obmat,par.obmat); 
//      (* duplicator bewaren *)
//      (* voor give_current_material *)
//      VECCOPY(new.obmat[3],vec); 
//      new.parent:= 0; 
//      new.track:= 0; 
//      addtail( and duplilist,new); 
//    end;
//  end;
//  set_displist_onlyzero(0); 
//  freeN(chartransdata); 
//end;


end.
