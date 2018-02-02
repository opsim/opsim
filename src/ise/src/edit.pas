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


(* edit.c  mei 94  GRAPHICS
 *
 *
 *
 *)
unit edit;

interface

uses
util;

//{$include "blender.h"}
//{$include "graphics.h"}
//{$include "edit.h"}
//{$include "sector.h"}

(* ************************************************ *)
(* pushpop faciliteit: om tijdelijk data te bewaren *)

var
ppmain: ListBase = (first: nil; last: nil);

type
  pPushPop = ^PushPop;
  PushPop = record
    next: pPushPop;
    prev: pPushPop;
    data: pinteger;
    len: integer;
  end;

procedure countall;

implementation

uses
  blenglob, blendef, space;

//procedure pushdata(data: pinteger;  len: integer);
//var
//pp: pPushPop; 
//begin
//  pp:= mallocN(sizeof(PushPop),'pushpop'); 
//  addtail(@ppmain,pp); 
//  pp^.data:= mallocN(len,'pushpop');
//  pp^.len:= len;
//  memcpy(pp^.data,data,len); 
//end;
//
//procedure popfirst(data: pinteger); 
//var
//pp: pPushPop; 
//begin
//  
//  pp:= ppmain.first; 
//  if pp<>0{nil} {<= !!!9} 
//  then
//  begin 
//    memcpy(data,pp.data,pp.len); 
//    remlink(@ppmain,pp); 
//    freeN(pp.data); 
//    freeN(pp); 
//  end;
//  else
//  printf('error in popfirst\n');
//end;
//
//
//procedure poplast(data: pinteger); 
//var
//pp: pPushPop; 
//begin
//  
//  pp:= ppmain.last; 
//  if pp<>0{nil} {<= !!!9} 
//  then
//  begin 
//    memcpy(data,pp.data,pp.len); 
//    remlink(@ppmain,pp); 
//    freeN(pp.data); 
//    freeN(pp); 
//  end;
//  else
//  printf('error in poplast\n');
//end;
//
//
//procedure free_pushpop; 
//var
//pp: pPushPop; 
//begin
//  
//  while pp:=ppmain.first
//  do
//  begin 
//    remlink(@ppmain,pp); 
//    freeN(pp.data); 
//    freeN(pp); 
//  end;
//end;
//
//
//procedure pushpop_test; 
//begin
//  if ppmain.first<>0{nil} {<= !!!9} 
//  then
//  printf('pushpop not empty\n');
//  free_pushpop(); 
//end;
//
//(* ************************************************ *)
//
//{$ifdef IRISGL}
//
//
//function get_border(rect: prcti;  col: smallint): integer; 
//var
//winmat: array [0..3,0..3] of float;
//viewmat: array [0..3,0..3] of float;
//dvec: array [0..3] of float;
//fac1: float; 
//fac2: float; 
//retval: integer;
// 
//event: word;
//mval: array [0..1] of smallint;
//mvalo: array [0..3] of smallint;
//val: smallint; 
//x1: smallint; 
//y1: smallint; 
//str: array [0..Pred(64)] of char; 
//begin
//  
//  
//  
//  
//  
//  retval:=1; 
//  
//  
//  
//  
//  
//  
//  
//  areawinset(curarea.win); 
//  drawmode(PUPDRAW); 
//  initgrabz(0.0,0.0,0.0); 
//  getmatrix(viewmat); 
//  (* windowmatrix default *)
//  mmode(MPROJECTION); 
//  getmatrix(winmat); 
//  mmode(MVIEWING); 
//  defwinmat(); 
//  getmouseco_areawin(mvalo); 
//  color(col); 
//  sboxs(0,mvalo[1],curarea.winx,mvalo[1]); 
//  sboxs(mvalo[0],0,mvalo[0],curarea.winy); 
//  
//  while LongBool(1)
//  do
//  begin 
//    getmouseco_areawin(mval); 
//    if mvalo[0]<>mval[0])or(mvalo[1]<>mval[1])or(qtest()
//    then
//    begin 
//      color(0); 
//      sboxs(0,mvalo[1],curarea.winx,mvalo[1]); 
//      sboxs(mvalo[0],0,mvalo[0],curarea.winy); 
//      color(col); 
//      sboxs(0,mval[1],curarea.winx,mval[1]); 
//      sboxs(mval[0],0,mval[0],curarea.winy); 
//      mvalo[0]:= mval[0]; 
//      mvalo[1]:= mval[1]; 
//      event:= extern_qread(@val); 
//      if val<>0{nil} {<= !!!9} 
//      then
//      begin 
//        if event=ESCKEY
//        then
//        begin 
//          retval:= 0; 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        else
//        if event=BKEY
//        then
//        begin 
//          retval:= 0; 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        else
//        if event=LEFTMOUSE
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        else
//        if event=MIDDLEMOUSE
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        else
//        if event=RIGHTMOUSE
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//    end;
//    else
//    usleep(2); 
//  end;
//  color(0); 
//  sboxs(0,mvalo[1],curarea.winx,mvalo[1]); 
//  sboxs(mvalo[0],0,mvalo[0],curarea.winy); 
//  
//  if retval<>0{nil} {<= !!!9} 
//  then
//  begin 
//    x1:= mval[0]; 
//    y1:= mval[1]; 
//    getmouseco_areawin(mvalo); 
//    color(col); 
//    sboxs(x1,y1,mvalo[0],mvalo[1]); 
//    while LongBool(1)
//    do
//    begin 
//      getmouseco_areawin(mval); 
//      if mvalo[0]<>mval[0])or(mvalo[1]<>mval[1])or(qtest()
//      then
//      begin 
//        color(0); 
//        sboxs(x1,y1,mvalo[0],mvalo[1]); 
//        sboxfs(10,10,400,32); 
//        color(col); 
//        sboxs(x1,y1,mval[0],mval[1]); 
//        mvalo[0]:= mval[0]; 
//        mvalo[1]:= mval[1]; 
//        event:= extern_qread(@val); 
//        if val=0
//        then
//        begin 
//          if event=ESCKEY
//          then
//          begin 
//            retval:= 0; 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//          else
//          if event=LEFTMOUSE
//          then
//          break; {<= !!!b possible in "switch" - then remove this line}
//          else
//          if event=MIDDLEMOUSE
//          then
//          break; {<= !!!b possible in "switch" - then remove this line}
//          else
//          if event=RIGHTMOUSE
//          then
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        if curarea.spacetype=SPACE_VIEW3D
//        then
//        begin 
//          if G.vd.persp=0
//          then
//          begin 
//            window_to_3d(dvec,mvalo[0]-x1,mvalo[1]-y1); 
//            glRasterPos2i(10,10); 
//            sprintf(str,'X %.4f  Y %.4f  Z %.4f  Dia %.4f',dvec[0],dvec[1],dvec[2],fsqrt(dvec[0]*dvec[0]+dvec[1]*dvec[1]+dvec[2]*dvec[2])); 
//            charstr(str); 
//          end;
//          else
//          if G.vd.persp=2
//          then
//          begin 
//            fac1:= (mvalo[0]-x1) div ({!!!a type cast? =>} {float(}(G.vd.pr_xmax-G.vd.pr_xmin)); 
//            fac1:= fac1 * (0.$1*G.scene.r.size*G.scene.r.xsch); 
//            fac2:= (mvalo[1]-y1) div ({!!!a type cast? =>} {float(}(G.vd.pr_ymax-G.vd.pr_ymin)); 
//            fac2:= fac2 * (0.$1*G.scene.r.size*G.scene.r.ysch); 
//            glRasterPos2i(10,10); 
//            sprintf(str,'X %.1f  Y %.1f  Dia %.1f',fabs(fac1),fabs(fac2),fsqrt(fac1*fac1+fac2*fac2)); 
//            charstr(str); 
//          end;
//        end;
//        else
//        if curarea.spacetype=SPACE_IPO
//        then
//        begin 
//          mvalo[2]:= x1; 
//          mvalo[3]:= y1; 
//          areamouseco_to_ipoco(mval,dvec,dvec+1); 
//          areamouseco_to_ipoco(mvalo+2,dvec+2,dvec+3); 
//          glRasterPos2i(18,18); 
//          sprintf(str,'Time: %.4f  Y %.4f',dvec[0]-dvec[2],dvec[1]-dvec[3]); 
//          charstr(str); 
//        end;
//      end;
//      else
//      sginap(2); 
//    end;
//    color(0); 
//    clear(); 
//    if retval<>0{nil} {<= !!!9} 
//    then
//    begin 
//      rect.xmin:= x1; 
//      rect.ymin:= y1; 
//      rect.xmax:= mval[0]; 
//      rect.ymax:= mval[1]; 
//      retval:= event; (* normaliseren *)
//      if rect.xmin>rect.xmax
//      then
//      SWAP(int,rect.xmin,rect.xmax); 
//      if rect.ymin>rect.ymax
//      then
//      SWAP(int,rect.ymin,rect.ymax); 
//      if rect.xmin=rect.xmax
//      then
//      retval:= 0; 
//      if rect.ymin=rect.ymax
//      then
//      retval:= 0; 
//    end;
//  end;
//  drawmode(NORMALDRAW); 
//  
//  loadmatrix(viewmat); 
//  
//  (* restore matrices *)
//  mmode(MPROJECTION); 
//  loadmatrix(winmat); 
//  mmode(MVIEWING); 
//  if event=BKEY)and(G.obedit)and(curarea.spacetype=SPACE_VIEW3D
//  then
//  circle_select(); 
//  begin
//    result:= retval; 
//    exit;
//  end;
//end;
//
//{$else}
//
//
//function get_border(rect: prcti;  col: smallint): integer; 
//var
//dvec: array [0..3] of float;
//fac1: float; 
//fac2: float; 
//retval: integer;
// 
//event: word;
//mval: array [0..1] of smallint;
//mvalo: array [0..3] of smallint;
//val: smallint; 
//x1: smallint; 
//y1: smallint; 
//str: array [0..Pred(64)] of char; 
//{$if defined(__sgi) || defined(__SUN)}
//(* this is a dirty patch: XgetImage gets sometimes the backbuffer *)
//begin
//  {scrmousex: smallint; }{<= !!!5 external variable}
//  {scrmousey: smallint; }{<= !!!5 external variable}
//  
//  
//  
//  retval:=1; 
//  
//  
//  
//  
//  
//  
//  my_get_frontbuffer_image(0,0,1,1); 
//  my_put_frontbuffer_image(); 
//  {$endif}
//  winset(G.curscreen.mainwin); 
//  
//  glReadBuffer(GL_FRONT); 
//  (* pietsje groter, 1 pixel aan de rand *)
//  glDrawBuffer(GL_FRONT); 
//  
//  if curarea.spacetype=SPACE_IPO
//  then
//  my_get_frontbuffer_image(20+curarea.winrct.xmin,30+curarea.winrct.ymin,300,22); 
//  else
//  my_get_frontbuffer_image(0+curarea.winrct.xmin,10+curarea.winrct.ymin,300,22); 
//  winset(curarea.win); 
//  glDrawBuffer(GL_FRONT); 
//  persp(0); 
//  initgrabz(0.0,0.0,0.0); 
//  getmouseco_areawin(mvalo); 
//  sdrawXORline4(0,0,mvalo[1],curarea.winx,mvalo[1]); 
//  
//  sdrawXORline4(1,mvalo[0],0,mvalo[0],curarea.winy); 
//  
//  while LongBool(1)
//  do
//  begin 
//    (* als een renderwinodow open is en de muis gaat erin *)
//    persp(1); 
//    winset(curarea.win); 
//    persp(0); 
//    getmouseco_areawin(mval); 
//    if mvalo[0]<>mval[0])or(mvalo[1]<>mval[1]
//    then
//    begin 
//      
//      sdrawXORline4(0,0,mval[1],curarea.winx,mval[1]); 
//      sdrawXORline4(1,mval[0],0,mval[0],curarea.winy); 
//      mvalo[0]:= mval[0]; 
//      mvalo[1]:= mval[1]; 
//    end;
//    event:= extern_qread(@val); 
//    if event)and(val
//    then
//    begin 
//      if event=ESCKEY
//      then
//      begin 
//        retval:= 0; 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      else
//      if event=BKEY
//      then
//      begin 
//        retval:= 0; 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      else
//      if event=LEFTMOUSE
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      else
//      if event=MIDDLEMOUSE
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      else
//      if event=RIGHTMOUSE
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    else
//    usleep(2); 
//  end;
//  sdrawXORline4(-1,0,0,0,0); 
//  fmsetfont(G.fonts); 
//  if retval<>0{nil} {<= !!!9} 
//  then
//  begin 
//    x1:= mval[0]; 
//    y1:= mval[1]; 
//    getmouseco_areawin(mvalo); 
//    sdrawXORline4(0,x1,y1,x1,mvalo[1]); 
//    
//    sdrawXORline4(1,x1,mvalo[1],mvalo[0],mvalo[1]); 
//    sdrawXORline4(2,mvalo[0],mvalo[1],mvalo[0],y1); 
//    sdrawXORline4(3,mvalo[0],y1,x1,y1); 
//    while LongBool(1)
//    do
//    begin 
//      getmouseco_areawin(mval); 
//      if mvalo[0]<>mval[0])or(mvalo[1]<>mval[1]
//      then
//      begin 
//        
//        sdrawXORline4(0,x1,y1,x1,mval[1]); 
//        sdrawXORline4(1,x1,mval[1],mval[0],mval[1]); 
//        sdrawXORline4(2,mval[0],mval[1],mval[0],y1); 
//        sdrawXORline4(3,mval[0],y1,x1,y1); 
//        mvalo[0]:= mval[0]; 
//        mvalo[1]:= mval[1]; 
//        
//      end;
//      event:= extern_qread(@val); 
//      persp(1); 
//      (* still because of the renderwindow... *)
//      winset(curarea.win); 
//      persp(0); 
//      if val=0
//      then
//      begin 
//        if event=ESCKEY
//        then
//        begin 
//          retval:= 0; 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        else
//        if event=LEFTMOUSE
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        else
//        if event=MIDDLEMOUSE
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        else
//        if event=RIGHTMOUSE
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      if event=0
//      then
//      usleep(2); 
//      if curarea.spacetype=SPACE_VIEW3D
//      then
//      begin 
//        glColor3f(0.4375,0.4375,0.4375); 
//        glRecti(0,10,250,20); 
//        glColor3f(0.0,0.0,0.0); 
//        
//        if G.vd.persp=0
//        then
//        begin 
//          window_to_3d(dvec,mvalo[0]-x1,mvalo[1]-y1); 
//          glRasterPos2i(10,10); 
//          
//          sprintf(str,'X %.4f  Y %.4f  Z %.4f  Dia %.4f',dvec[0],dvec[1],dvec[2],fsqrt(dvec[0]*dvec[0]+dvec[1]*dvec[1]+dvec[2]*dvec[2])); 
//          fmprstr(str); 
//        end;
//        else
//        if G.vd.persp=2
//        then
//        begin 
//          fac1:= (mvalo[0]-x1) div ({!!!a type cast? =>} {float(}(G.vd.pr_xmax-G.vd.pr_xmin)); 
//          fac1:= fac1 * (0.$1*G.scene.r.size*G.scene.r.xsch); 
//          fac2:= (mvalo[1]-y1) div ({!!!a type cast? =>} {float(}(G.vd.pr_ymax-G.vd.pr_ymin)); 
//          fac2:= fac2 * (0.$1*G.scene.r.size*G.scene.r.ysch); 
//          glRasterPos2i(10,10); 
//          sprintf(str,'X %.1f  Y %.1f  Dia %.1f',fabs(fac1),fabs(fac2),fsqrt(fac1*fac1+fac2*fac2)); 
//          fmprstr(str); 
//        end;
//      end;
//      else
//      if curarea.spacetype=SPACE_IPO
//      then
//      begin 
//        glColor3f(.40625,.40625,.40625); 
//        glRecti(20,30,170,40); 
//        glColor3f(0.0,0.0,0.0); 
//        mvalo[2]:= x1; 
//        mvalo[3]:= y1; 
//        areamouseco_to_ipoco(mval,dvec,dvec+1); 
//        areamouseco_to_ipoco(mvalo+2,dvec+2,dvec+3); 
//        glRasterPos2i(30,30); 
//        
//        sprintf(str,'Time: %.4f  Y %.4f',dvec[0]-dvec[2],dvec[1]-dvec[3]); 
//        fmprstr(str); 
//      end;
//    end;
//    sdrawXORline4(-1,0,0,0,0); 
//    if retval<>0{nil} {<= !!!9} 
//    then
//    begin 
//      rect.xmin:= x1; 
//      rect.ymin:= y1; 
//      rect.xmax:= mval[0]; 
//      rect.ymax:= mval[1]; 
//      retval:= event; 
//      (* normaliseren *)
//      if rect.xmin>rect.xmax
//      then
//      SWAP(int,rect.xmin,rect.xmax); 
//      if rect.ymin>rect.ymax
//      then
//      SWAP(int,rect.ymin,rect.ymax); 
//      if rect.xmin=rect.xmax
//      then
//      retval:= 0; 
//      if rect.ymin=rect.ymax
//      then
//      retval:= 0; 
//    end;
//  end;
//  
//  
//  (* wissen *)
//  if event<>BKEY
//  then
//  begin 
//    
//    function ELEM{!!!3 unknown typedef}: if; 
//    begin
//      my_put_frontbuffer_image(); 
//    end
//  end;
//  glDrawBuffer(GL_BACK); 
//  persp(1); 
//  
//  if event=BKEY)and(G.obedit)and(curarea.spacetype=SPACE_VIEW3D
//  then
//  circle_select(); 
//  begin
//    result:= retval; 
//    exit;
//  end;
//end;
//
//{$endif}
//
//
//procedure count_object(ob: pObject;  sel: integer); 
//var
//me: pMesh; 
//cu: pCurve; 
//se: pSector; 
//tot: integer;
// 
//totf: integer;
// 
//begin
//  
//  
//  
//  tot:=0; 
//  totf:=0; 
//  case ob.type of
//    OB_MESH,
//    OB_SECTOR,
//    OB_LIFE:
//    begin
//      inc(G.totmesh); 
//      me:= get_mesh(ob); 
//      if me<>0{nil} {<= !!!9} 
//      then
//      begin 
//        G.totvert:= G.totvert + (me.totvert); 
//        G.totface:= G.totface + (me.totface); 
//        if sel<>0{nil} {<= !!!9} 
//        then
//        begin 
//          G.totvertsel:= G.totvertsel + (me.totvert); 
//          G.totfacesel:= G.totfacesel + (me.totface); 
//        end;
//      end;
//    end;
//    OB_LAMP:
//    begin
//      inc(G.totlamp); 
//    end;
//    OB_SURF,
//    OB_CURVE,
//    OB_FONT:
//    begin
//      inc(G.totcurve); 
//      tot:= totf:=0; 
//      cu:= ob.data; 
//      if cu.disp.first=0
//      then
//      makeDispList(ob); 
//      count_displist( and cu.disp,@tot,@totf); 
//      G.totvert:= G.totvert + (tot); 
//      G.totface:= G.totface + (totf); 
//      if sel<>0{nil} {<= !!!9} 
//      then
//      begin 
//        G.totvertsel:= G.totvertsel + (tot); 
//        G.totfacesel:= G.totfacesel + (totf); 
//      end;
//    end;
//    OB_MBALL:
//    begin
//      count_displist(@ob.disp,@tot,@totf); 
//      G.totvert:= G.totvert + (tot); 
//      G.totface:= G.totface + (totf); 
//      if sel<>0{nil} {<= !!!9} 
//      then
//      begin 
//        G.totvertsel:= G.totvertsel + (tot); 
//        G.totfacesel:= G.totfacesel + (totf); 
//      end;
//    end;
//    
//  end;{case?}
//end;

{editLatt: pLattice; }{<= !!!5 external variable}
{editNurb: ListBase; }{<= !!!5 external variable}(* extern ListBase bpbase; *)

procedure countall;
var
//base: pBase;
//ob: pObject;
//me: pMesh;
//cu: pCurve;
//se: pSector;
//nu: pNurb;
//bezt: pBezTriple;
//bp: pBPoint;
//ml: pMetaElem;
//dl: pDispList;
(* struct BodyPoint *bop; *)
//eve: pEditVert;
//evl: pEditVlak;
tot: integer;
a: integer;
begin
  G.totvert:= 0;
  G.totvertsel:=0;
  G.totfacesel:=0;
  G.totface:=0;
  G.totobj:=0;
  G.totmesh:=0;
  G.totlamp:=0;
  G.totcurve:=0;
  G.totobjsel:=0;

  if G.obedit<>nil  then
  begin

    if ELEM3(G.obedit^._type, OB_MESH, OB_SECTOR, OB_LIFE)then
    begin
      //eve:= G.edve.first;
      //while eve  do
      //begin
      //  inc(G.totvert);
      //  if (eve.f and 1)<>0     then
      //  inc(G.totvertsel);
      //  eve:= eve.next;
      //end;
      //evl:= G.edvl.first;
      //while evl <>nil       do
      //begin
      //  inc(G.totface);
      //  if (evl.v1.f and 1)<>0        then
      //  begin
      //    if (evl.v2.f and 1)<>0         then
      //    begin
      //      if (evl.v3.f and 1) <> 0  then
      //      begin
      //        if evl.v4<>0             then
      //        begin
      //          if (evl.v4.f and 1)<>0                then
      //          inc(G.totfacesel);
      //        end
      //        else
      //        begin
      //          inc(G.totfacesel);
      //        end;
      //      end;
      //    end;
      //  end;
      //  evl:= evl.next;
      //end;
    end
    else

    if ELEM3(G.obedit^._type, OB_CURVE, OB_SURF, OB_FONT) then
    begin
      //nu:= editNurb.first;
      //while nu<>nil      do
      //begin
      //  if (nu.type and 7)=CU_BEZIER         then
      //  begin
      //    bezt:= nu.bezt;
      //    a:= nu.pntsu;
      //    while a<>0          do
      //    begin
      //      G.totvert:= G.totvert + (3);
      //      if bezt.f1<>0            then
      //      inc(G.totvertsel);
      //      if bezt.f2<>0            then
      //      inc(G.totvertsel);
      //      if bezt.f3<>0            then
      //      inc(G.totvertsel);
      //      inc(bezt);
      //
      //      dec(a);
      //    end;
      //  end;
      //  else
      //  begin
      //    bp:= nu.bp;
      //    a:= nu.pntsu*nu.pntsv;
      //    while a <> 0         do
      //    begin
      //      inc(G.totvert);
      //      if bp.f1 and 1
      //      then
      //      inc(G.totvertsel);
      //      inc(bp);
      //
      //      dec(a);
      //    end;
      //  end;
      //  nu:= nu.next;
      //end;
    end
    else
    if G.obedit^._type=OB_MBALL    then
    begin
      //ml:= editelems.first;
      //while ml <> nil      do
      //begin
      //  inc(G.totvert);
      //  if (ml.flag and SELECT) <> 0         then
      //  inc(G.totvertsel);
      //  ml:= ml.next;
      //end;
    end
    else
    if G.obedit^._type=OB_LATTICE     then
    begin
      //bp:= editLatt.def;
      //a:= editLatt.pntsu*editLatt.pntsv*editLatt.pntsw;
      //while a<>0     do
      //begin
      //  inc(G.totvert);
      //  if (bp.f1 and 1) <> 0        then
      //  inc(G.totvertsel);
      //  inc(bp);
      //
      //  dec(a);
      //end;
    end;
    allqueue(REDRAWINFO,1);

    (* 1, want header->win==0! *)
    exit;
  end
  else
  if (G.f and (G_FACESELECT+G_VERTEXPAINT)) <> 0  then
  begin
    //me:= get_mesh(OBACT);
    //if me<>nil    then
    //begin
    //  G.totface:= me.totface;
    //  G.totvert:= me.totvert;
    //end;
    //allqueue(REDRAWINFO,1);
    //(* 1, want header->win==0! *)
    //exit;
  end;

  if G.vd=nil then
  exit;
  if G.scene=nil then
  exit;

  //base:= FIRSTBASE;
  //while base <> 0  do
  //begin
  //  if (G.vd.lay<>0) and (base.lay<>0)    then
  //  begin
  //    inc(G.totobj);
  //    if (base.flag and SELECT) <> 0       then
  //    inc(G.totobjsel);
  //    count_object(base.object,base.flag and SELECT);
  //    if( base.object.transflag and OB_DUPLI) <> 0      then
  //    begin
  //      {duplilist: ListBase; }{<= !!!5 external variable}
  //      make_duplilist(G.scene,base.object);
  //      ob:= duplilist.first;
  //      while ob <>nil        do
  //      begin
  //        inc(G.totobj);
  //        count_object(ob,base.flag and SELECT);
  //        ob:= ob.id.next;
  //      end;
  //      free_duplilist();
  //    end;
  //  end;
  //  base:= base.next;
  //end;
  allqueue(REDRAWINFO,1);    (* 1, want header->win==0! *)
end;

//procedure snapmenu; 
//var
//tv: pTransVert; 
//base: pBase; 
//ob: pObject; 
//gridf: float; 
//curs: pfloat; 
//imat: array [0..2,0..2] of float;
//bmat: array [0..2,0..2] of float;
//vec: array [0..2] of float;
//min: array [0..2] of float;
//max: array [0..2] of float;
//centroid: array [0..2] of float;
//count: integer; 
//a: integer; 
//event: smallint; 
//begin
//  {transvmain: pTransVert; }{<= !!!5 external variable}
//  {tottrans: integer; }{<= !!!5 external variable}
//  {originmat: array [0..2,0..2] of float; }{<= !!!5 external variable}(* object.c *)
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
//  event:= pupmenu('SNAP %t|Sel -> Grid%x1|Sel -> Curs%x2|Curs-> Grid%x3|Curs-> Sel%x4'); 
//  gridf:= G.vd.grid; 
//  curs:= give_cursor(); 
//  if event=1)or(event=2
//  then
//  begin 
//    (* sel->grid  sel->curs  *)
//    
//    if G.obedit<>0{nil} {<= !!!9} 
//    then
//    begin 
//      if{!!!e unknown token}
//      make_trans_verts(bmat[0],bmat[1],0); 
//      if tottrans=0
//      then
//      exit;
//      Mat3CpyMat4(bmat,G.obedit.obmat); 
//      Mat3Inv(imat,bmat); 
//      tv:= transvmain; 
//      while{for} 
//      a:= 0; 
//      {to} a<tottrans
//      {;}
//      inc(a); 
//      inc(tv); 
//      do
//      begin 
//        if event=2
//        then
//        begin 
//          vec[0]:= curs[0]-G.obedit.obmat[3][0]; 
//          vec[1]:= curs[1]-G.obedit.obmat[3][1]; 
//          vec[2]:= curs[2]-G.obedit.obmat[3][2]; 
//        end;
//        else
//        begin 
//          VECCOPY(vec,tv.loc); 
//          Mat3MulVecfl(bmat,vec); 
//          VecAddf(vec,vec,G.obedit.obmat[3]); 
//          vec[0]:= G.vd.grid*ffloor(.5+vec[0] div gridf); 
//          vec[1]:= G.vd.grid*ffloor(.5+vec[1] div gridf); 
//          vec[2]:= G.vd.grid*ffloor(.5+vec[2] div gridf); 
//          VecSubf(vec,vec,G.obedit.obmat[3]); 
//        end;
//        Mat3MulVecfl(imat,vec); 
//        VECCOPY(tv.loc,vec); 
//      end;
//      
//      freeN(transvmain); 
//      transvmain:= 0; if{!!!e unknown token}
//      makeDispList(G.obedit); 
//      allqueue(REDRAWVIEW3D,0); 
//      exit;
//    end;
//    base:= FIRSTBASE; 
//    while base
//    do
//    begin 
//      if TESTBASELIB(base)
//      then
//      begin 
//        ob:= base.object; 
//        if event=2
//        then
//        begin 
//          vec[0]:= -ob.obmat[3][0]+curs[0]; 
//          vec[1]:= -ob.obmat[3][1]+curs[1]; 
//          vec[2]:= -ob.obmat[3][2]+curs[2]; 
//        end;
//        else
//        begin 
//          vec[0]:= -ob.obmat[3][0]+G.vd.grid*ffloor(.5+ob.obmat[3][0] div gridf); 
//          vec[1]:= -ob.obmat[3][1]+G.vd.grid*ffloor(.5+ob.obmat[3][1] div gridf); 
//          vec[2]:= -ob.obmat[3][2]+G.vd.grid*ffloor(.5+ob.obmat[3][2] div gridf); 
//        end;
//        if ob.parent<>0{nil} {<= !!!9} 
//        then
//        begin 
//          where_is_object(ob); 
//          Mat3Inv(imat,originmat); 
//          Mat3MulVecfl(imat,vec); 
//          ob.loc[0]:= ob.loc[0] + (vec[0]); 
//          ob.loc[1]:= ob.loc[1] + (vec[1]); 
//          ob.loc[2]:= ob.loc[2] + (vec[2]); 
//        end;
//        else
//        begin 
//          ob.loc[0]:= ob.loc[0] + (vec[0]); 
//          ob.loc[1]:= ob.loc[1] + (vec[1]); 
//          ob.loc[2]:= ob.loc[2] + (vec[2]); 
//        end;
//      end;
//      base:= base.next; 
//    end;
//    allqueue(REDRAWVIEW3D,0); 
//  end;
//  else
//  if event=3
//  then
//  begin 
//    (* curs to grid *)
//    curs[0]:= G.vd.grid*ffloor(.5+curs[0] div gridf); 
//    curs[1]:= G.vd.grid*ffloor(.5+curs[1] div gridf); 
//    curs[2]:= G.vd.grid*ffloor(.5+curs[2] div gridf); 
//    allqueue(REDRAWVIEW3D,0); 
//  end;
//  else
//  if event=4
//  then
//  begin 
//    (* curs to sel *)
//    count:= 0; 
//    INIT_MINMAX(min,max); 
//    centroid[0]:= centroid[1]:=centroid[2]:=0.0; 
//    if G.obedit<>0{nil} {<= !!!9} 
//    then
//    begin 
//      if{!!!e unknown token}
//      make_trans_verts(bmat[0],bmat[1],0); 
//      if tottrans=0
//      then
//      exit;
//      Mat3CpyMat4(bmat,G.obedit.obmat); 
//      tv:= transvmain; 
//      while{for} 
//      a:= 0; 
//      {to} a<tottrans
//      {;}
//      inc(a); 
//      inc(tv); 
//      do
//      begin 
//        VECCOPY(vec,tv.loc); 
//        Mat3MulVecfl(bmat,vec); 
//        VecAddf(vec,vec,G.obedit.obmat[3]); 
//        VecAddf(centroid,centroid,vec); 
//        DO_MINMAX(vec,min,max); 
//      end;
//      if G.vd.around=V3D_CENTROID
//      then
//      begin 
//        VecMulf(centroid,1.0 div {!!!a type cast? =>} {float(}tottrans); 
//        VECCOPY(curs,centroid); 
//      end;
//      else
//      begin 
//        curs[0]:= (min[0]+max[0]) div 2; 
//        curs[1]:= (min[1]+max[1]) div 2; 
//        curs[2]:= (min[2]+max[2]) div 2; 
//      end;
//      freeN(transvmain); 
//      transvmain:= 0; 
//      
//    end;
//    else
//    begin 
//      base:= FIRSTBASE; 
//      while base
//      do
//      begin 
//        if TESTBASE(base)
//        then
//        begin 
//          VECCOPY(vec,base.object.obmat[3]); 
//          VecAddf(centroid,centroid,vec); 
//          DO_MINMAX(vec,min,max); 
//          inc(count); 
//        end;
//        base:= base.next; 
//      end;
//      if count<>0{nil} {<= !!!9} 
//      then
//      begin 
//        if G.vd.around=V3D_CENTROID
//        then
//        begin 
//          VecMulf(centroid,1.0 div {!!!a type cast? =>} {float(}count); 
//          VECCOPY(curs,centroid); 
//        end;
//        else
//        begin 
//          curs[0]:= (min[0]+max[0]) div 2; 
//          curs[1]:= (min[1]+max[1]) div 2; 
//          curs[2]:= (min[2]+max[2]) div 2; 
//        end;
//      end;
//    end;
//    allqueue(REDRAWVIEW3D,0); 
//  end;
//end;

end.
