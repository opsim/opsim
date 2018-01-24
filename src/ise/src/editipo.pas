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
(* editipo.c   GRAPHICS
 *
 * april 95
 *
 *)

unit editipo;

interface

uses
util;
//{$include "blender.h"}
//{$include "graphics.h"}
//{$include "edit.h"}
//{$include "ipo.h"}
//{$include "ika.h"}
//{$include "sequence.h"}

const
IPOTHRESH = 0.9; 

//procedure transform_ipo(mode: integer); 
//
//procedure make_ipokey; 

procedure free_ipokey(lb: pListBase); 
{ob_ar: array [0..] of integer; }{<= !!!5 external variable}
{ma_ar: array [0..] of integer; }{<= !!!5 external variable}
{seq_ar: array [0..] of integer; }{<= !!!5 external variable}
{cu_ar: array [0..] of integer; }{<= !!!5 external variable}
{key_ar: array [0..] of integer; }{<= !!!5 external variable}
{wo_ar: array [0..] of integer; }{<= !!!5 external variable}
{la_ar: array [0..] of integer; }{<= !!!5 external variable}
{cam_ar: array [0..] of integer; }{<= !!!5 external variable}
implementation

uses
space, ipo;

//procedure getname_ob_ei(nr: integer;  str: pchar;  colipo: integer); 
//begin
//  str[0]:= 0; 
//  case nr of
//    OB_LOC_X:
//    begin
//      strcpy(str,'LocX'); 
//    end;
//    OB_LOC_Y:
//    begin
//      strcpy(str,'LocY'); 
//    end;
//    OB_LOC_Z:
//    begin
//      strcpy(str,'LocZ'); 
//    end;
//    OB_DLOC_X:
//    begin
//      strcpy(str,'dLocX'); 
//    end;
//    OB_DLOC_Y:
//    begin
//      strcpy(str,'dLocY'); 
//    end;
//    OB_DLOC_Z:
//    begin
//      strcpy(str,'dLocZ'); 
//    end;
//    OB_ROT_X:
//    begin
//      strcpy(str,'RotX'); 
//    end;
//    OB_ROT_Y:
//    begin
//      strcpy(str,'RotY'); 
//    end;
//    OB_ROT_Z:
//    begin
//      strcpy(str,'RotZ'); 
//    end;
//    OB_DROT_X:
//    begin
//      strcpy(str,'dRotX'); 
//    end;
//    OB_DROT_Y:
//    begin
//      strcpy(str,'dRotY'); 
//    end;
//    OB_DROT_Z:
//    begin
//      strcpy(str,'dRotZ'); 
//    end;
//    OB_SIZE_X:
//    begin
//      strcpy(str,'SizeX'); 
//    end;
//    OB_SIZE_Y:
//    begin
//      strcpy(str,'SizeY'); 
//    end;
//    OB_SIZE_Z:
//    begin
//      strcpy(str,'SizeZ'); 
//    end;
//    OB_DSIZE_X:
//    begin
//      strcpy(str,'dSizeX'); 
//    end;
//    OB_DSIZE_Y:
//    begin
//      strcpy(str,'dSizeY'); 
//    end;
//    OB_DSIZE_Z:
//    begin
//      strcpy(str,'dSizeZ'); 
//    end;
//    OB_LAY:
//    begin
//      strcpy(str,'Layer'); 
//    end;
//    OB_TIME:
//    begin
//      strcpy(str,'Time'); 
//    end;
//    OB_EFF_X:
//    begin
//      if colipo<>nil 
//      then
//      strcpy(str,'ColR'); 
//      else
//      strcpy(str,'EffX'); 
//    end;
//    OB_EFF_Y:
//    begin
//      if colipo<>nil 
//      then
//      strcpy(str,'ColG'); 
//      else
//      strcpy(str,'EffY'); 
//    end;
//    OB_EFF_Z:
//    begin
//      if colipo<>nil 
//      then
//      strcpy(str,'ColB'); 
//      else
//      strcpy(str,'EffZ'); 
//    end;
//    
//  end;{case?}
//end;
//
//procedure getname_mat_ei(nr: integer;  str: pchar); 
//begin
//  str[0]:= 0; 
//  if nr>=MA_MAP1
//  then
//  begin 
//    case (nr and MA_MAP1-1) of
//      MAP_OFS_X:
//      begin
//        strcpy(str,'OfsX'); 
//      end;
//      MAP_OFS_Y:
//      begin
//        strcpy(str,'OfsY'); 
//      end;
//      MAP_OFS_Z:
//      begin
//        strcpy(str,'OfsZ'); 
//      end;
//      MAP_SIZE_X:
//      begin
//        strcpy(str,'SizeX'); 
//      end;
//      MAP_SIZE_Y:
//      begin
//        strcpy(str,'SizeY'); 
//      end;
//      MAP_SIZE_Z:
//      begin
//        strcpy(str,'SizeZ'); 
//      end;
//      MAP_R:
//      begin
//        strcpy(str,'texR'); 
//      end;
//      MAP_G:
//      begin
//        strcpy(str,'texG'); 
//      end;
//      MAP_B:
//      begin
//        strcpy(str,'texB'); 
//      end;
//      MAP_DVAR:
//      begin
//        strcpy(str,'DefVar'); 
//      end;
//      MAP_COLF:
//      begin
//        strcpy(str,'Col'); 
//      end;
//      MAP_NORF:
//      begin
//        strcpy(str,'Nor'); 
//      end;
//      MAP_VARF:
//      begin
//        strcpy(str,'Var'); 
//      end;
//      
//    end;{case?}
//  end;
//  else
//  begin 
//    case nr of
//      MA_COL_R:
//      begin
//        strcpy(str,'R'); 
//      end;
//      MA_COL_G:
//      begin
//        strcpy(str,'G'); 
//      end;
//      MA_COL_B:
//      begin
//        strcpy(str,'B'); 
//      end;
//      MA_SPEC_R:
//      begin
//        strcpy(str,'SpecR'); 
//      end;
//      MA_SPEC_G:
//      begin
//        strcpy(str,'SpecG'); 
//      end;
//      MA_SPEC_B:
//      begin
//        strcpy(str,'SpecB'); 
//      end;
//      MA_MIR_R:
//      begin
//        strcpy(str,'MirR'); 
//      end;
//      MA_MIR_G:
//      begin
//        strcpy(str,'MirG'); 
//      end;
//      MA_MIR_B:
//      begin
//        strcpy(str,'MirB'); 
//      end;
//      MA_REF:
//      begin
//        strcpy(str,'Ref'); 
//      end;
//      MA_ALPHA:
//      begin
//        strcpy(str,'Alpha'); 
//      end;
//      MA_EMIT:
//      begin
//        strcpy(str,'Emit'); 
//      end;
//      MA_AMB:
//      begin
//        strcpy(str,'Amb'); 
//      end;
//      MA_SPEC:
//      begin
//        strcpy(str,'Spec'); 
//      end;
//      MA_HARD:
//      begin
//        strcpy(str,'Hard'); 
//      end;
//      MA_SPTR:
//      begin
//        strcpy(str,'SpTra'); 
//      end;
//      MA_ANG:
//      begin
//        strcpy(str,'Ang'); 
//      end;
//      MA_MODE:
//      begin
//        strcpy(str,'Mode'); 
//      end;
//      MA_HASIZE:
//      begin
//        strcpy(str,'HaSize'); 
//      end;
//      
//    end;{case?}
//  end;
//end;
//
//procedure getname_world_ei(nr: integer;  str: pchar); 
//begin
//  str[0]:= 0; 
//  if nr>=MA_MAP1
//  then
//  begin 
//    case (nr and MA_MAP1-1) of
//      MAP_OFS_X:
//      begin
//        strcpy(str,'OfsX'); 
//      end;
//      MAP_OFS_Y:
//      begin
//        strcpy(str,'OfsY'); 
//      end;
//      MAP_OFS_Z:
//      begin
//        strcpy(str,'OfsZ'); 
//      end;
//      MAP_SIZE_X:
//      begin
//        strcpy(str,'SizeX'); 
//      end;
//      MAP_SIZE_Y:
//      begin
//        strcpy(str,'SizeY'); 
//      end;
//      MAP_SIZE_Z:
//      begin
//        strcpy(str,'SizeZ'); 
//      end;
//      MAP_R:
//      begin
//        strcpy(str,'texR'); 
//      end;
//      MAP_G:
//      begin
//        strcpy(str,'texG'); 
//      end;
//      MAP_B:
//      begin
//        strcpy(str,'texB'); 
//      end;
//      MAP_DVAR:
//      begin
//        strcpy(str,'DefVar'); 
//      end;
//      MAP_COLF:
//      begin
//        strcpy(str,'Col'); 
//      end;
//      MAP_NORF:
//      begin
//        strcpy(str,'Nor'); 
//      end;
//      MAP_VARF:
//      begin
//        strcpy(str,'Var'); 
//      end;
//      
//    end;{case?}
//  end;
//  else
//  begin 
//    case nr of
//      WO_HOR_R:
//      begin
//        strcpy(str,'HorR'); 
//      end;
//      WO_HOR_G:
//      begin
//        strcpy(str,'HorG'); 
//      end;
//      WO_HOR_B:
//      begin
//        strcpy(str,'HorB'); 
//      end;
//      WO_ZEN_R:
//      begin
//        strcpy(str,'ZenR'); 
//      end;
//      WO_ZEN_G:
//      begin
//        strcpy(str,'ZenG'); 
//      end;
//      WO_ZEN_B:
//      begin
//        strcpy(str,'ZenB'); 
//      end;
//      WO_EXPOS:
//      begin
//        strcpy(str,'Expos'); 
//      end;
//      WO_MISI:
//      begin
//        strcpy(str,'Misi'); 
//      end;
//      WO_MISTDI:
//      begin
//        strcpy(str,'MisDi'); 
//      end;
//      WO_MISTSTA:
//      begin
//        strcpy(str,'MisSta'); 
//      end;
//      WO_MISTHI:
//      begin
//        strcpy(str,'MisHi'); 
//      end;
//      WO_STAR_R:
//      begin
//        strcpy(str,'StarR'); 
//      end;
//      WO_STAR_G:
//      begin
//        strcpy(str,'StarB'); 
//      end;
//      WO_STAR_B:
//      begin
//        strcpy(str,'StarG'); 
//      end;
//      WO_STARDIST:
//      begin
//        strcpy(str,'StarDi'); 
//      end;
//      WO_STARSIZE:
//      begin
//        strcpy(str,'StarSi'); 
//      end;
//      
//    end;{case?}
//  end;
//end;
//
//procedure getname_seq_ei(nr: integer;  str: pchar); 
//begin
//  str[0]:= 0; 
//  case nr of
//    SEQ_FAC1:
//    begin
//      strcpy(str,'Fac'); 
//    end;
//    
//  end;{case?}
//end;
//
//procedure getname_cu_ei(nr: integer;  str: pchar); 
//begin
//  str[0]:= 0; 
//  case nr of
//    CU_SPEED:
//    begin
//      strcpy(str,'Speed'); 
//    end;
//    
//  end;{case?}
//end;
//
//procedure getname_key_ei(nr: integer;  str: pchar); 
//begin
//  str[0]:= 0; 
//  if nr=KEY_SPEED
//  then
//  begin 
//    strcpy(str,'Speed'); 
//    exit;
//  end;
//  sprintf(str,'Key %d',nr); 
//end;
//
//procedure getname_la_ei(nr: integer;  str: pchar); 
//begin
//  str[0]:= 0; 
//  if nr>=MA_MAP1
//  then
//  begin 
//    case (nr and MA_MAP1-1) of
//      MAP_OFS_X:
//      begin
//        strcpy(str,'OfsX'); 
//      end;
//      MAP_OFS_Y:
//      begin
//        strcpy(str,'OfsY'); 
//      end;
//      MAP_OFS_Z:
//      begin
//        strcpy(str,'OfsZ'); 
//      end;
//      MAP_SIZE_X:
//      begin
//        strcpy(str,'SizeX'); 
//      end;
//      MAP_SIZE_Y:
//      begin
//        strcpy(str,'SizeY'); 
//      end;
//      MAP_SIZE_Z:
//      begin
//        strcpy(str,'SizeZ'); 
//      end;
//      MAP_R:
//      begin
//        strcpy(str,'texR'); 
//      end;
//      MAP_G:
//      begin
//        strcpy(str,'texG'); 
//      end;
//      MAP_B:
//      begin
//        strcpy(str,'texB'); 
//      end;
//      MAP_DVAR:
//      begin
//        strcpy(str,'DefVar'); 
//      end;
//      MAP_COLF:
//      begin
//        strcpy(str,'Col'); 
//      end;
//      MAP_NORF:
//      begin
//        strcpy(str,'Nor'); 
//      end;
//      MAP_VARF:
//      begin
//        strcpy(str,'Var'); 
//      end;
//      
//    end;{case?}
//  end;
//  else
//  begin 
//    case nr of
//      LA_ENERGY:
//      begin
//        strcpy(str,'Energ'); 
//      end;
//      LA_COL_R:
//      begin
//        strcpy(str,'R'); 
//      end;
//      LA_COL_G:
//      begin
//        strcpy(str,'G'); 
//      end;
//      LA_COL_B:
//      begin
//        strcpy(str,'B'); 
//      end;
//      LA_DIST:
//      begin
//        strcpy(str,'Dist'); 
//      end;
//      LA_SPOTSI:
//      begin
//        strcpy(str,'SpoSi'); 
//      end;
//      LA_SPOTBL:
//      begin
//        strcpy(str,'SpoBl'); 
//      end;
//      LA_QUAD1:
//      begin
//        strcpy(str,'Quad1'); 
//      end;
//      LA_QUAD2:
//      begin
//        strcpy(str,'Quad2'); 
//      end;
//      LA_HALOINT:
//      begin
//        strcpy(str,'HaInt'); 
//      end;
//      
//    end;{case?}
//  end;
//end;
//
//procedure getname_cam_ei(nr: integer;  str: pchar); 
//begin
//  str[0]:= 0; 
//  case nr of
//    CAM_LENS:
//    begin
//      strcpy(str,'Lens'); 
//    end;
//    CAM_STA:
//    begin
//      strcpy(str,'ClSta'); 
//    end;
//    CAM_END:
//    begin
//      strcpy(str,'ClEnd'); 
//    end;
//    
//  end;{case?}
//end;
//
//function find_ipocurve(ipo: pIpo;  adrcode: integer): pIpoCurve; 
//var
//icu: pIpoCurve; 
//begin
//  
//  if ipo=0
//  then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  icu:= ipo.curve.first; 
//  while icu
//  do
//  begin 
//    if icu.adrcode=adrcode
//    then
//    begin
//      result:= icu; 
//      exit;
//    end;
//    icu:= icu.next; 
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//procedure boundbox_ipocurve(icu: pIpoCurve); 
//var
//bezt: pBezTriple; 
//bp: pBPoint; 
//vec: array [0..Pred(3)] of float;
// 
//min: array [0..Pred(3)] of float; 
//max: array [0..Pred(3)] of float; 
//a: integer; 
//begin
//  
//  
//  vec:=(0.0,0.0,0.0); 
//  
//  
//  
//  if icu.totvert<>nil 
//  then
//  begin 
//    INIT_MINMAX(min,max); 
//    if icu.bezt<>nil 
//    then
//    begin 
//      a:= icu.totvert; 
//      bezt:= icu.bezt; 
//      while a{--} dec(a); 
//      do
//      begin 
//        if icu.vartype and IPO_BITS
//        then
//        begin 
//          vec[0]:= bezt.vec[1][0]; 
//          vec[1]:= 0.0; 
//          DO_MINMAX(vec,min,max); 
//          vec[1]:= 16.0; 
//          DO_MINMAX(vec,min,max); 
//        end;
//        else
//        begin 
//          if icu.ipo=IPO_BEZ)and(a<>icu.totvert-1
//          then
//          begin 
//            DO_MINMAX(bezt.vec[0],min,max); 
//          end;
//          DO_MINMAX(bezt.vec[1],min,max); 
//          if icu.ipo=IPO_BEZ)and(a<>0
//          then
//          begin 
//            DO_MINMAX(bezt.vec[2],min,max); 
//          end;
//        end;
//        inc(bezt); 
//      end;
//    end;
//    else
//    if icu.bp<>nil 
//    then
//    begin 
//      a:= icu.totvert; 
//      bp:= icu.bp; 
//      while a{--} dec(a); 
//      do
//      begin 
//        DO_MINMAX(bp.vec,min,max); 
//        inc(bp); 
//      end;
//    end;
//    if min[0]=max[0]
//    then
//    max[0]:= min[0]+1.0; 
//    if min[1]=max[1]
//    then
//    max[1]:= min[1]+0.1; 
//    icu.totrct.xmin:= min[0]; 
//    icu.totrct.ymin:= min[1]; 
//    icu.totrct.xmax:= max[0]; 
//    icu.totrct.ymax:= max[1]; 
//  end;
//  else
//  begin 
//    icu.totrct.xmin:= icu.totrct.ymin:=0.0; 
//    icu.totrct.xmax:= EFRA; 
//    icu.totrct.ymax:= 1.0; 
//  end;
//end;
//
//procedure boundbox_ipo(ipo: pIpo;  bb: prctf); 
//var
//icu: pIpoCurve; 
//first: integer;
// 
//begin
//  
//  first:=1; 
//  icu:= ipo.curve.first; 
//  while icu
//  do
//  begin 
//    boundbox_ipocurve(icu); 
//    if first<>nil 
//    then
//    begin 
//      {*}bb^:=icu.totrct; 
//      first:= 0; 
//    end;
//    else
//    union_rctf(bb,@(icu.totrct)); 
//    icu:= icu.next; 
//  end;
//end;
//
//procedure editipo_changed(doredraw: integer); 
//var
//ei: pEditIpo; 
//v2d: pView2D; 
//key: pKey; 
//kb: pKeyBlock; 
//dx: float; 
//dy: float; 
//a: integer; 
//first: integer;
// 
//ob: pObject;
// 
//begin
//  
//  
//  
//  
//  
//  
//  
//  first:=1; 
//  ei:= G.sipo.editipo; 
//  if ei=0
//  then
//  exit;
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    if ei.icu<>nil 
//    then
//    begin 
//      (* 2 keer i.v.m. ittereren nieuwe autohandle *)
//      calchandles_ipocurve(ei.icu); 
//      calchandles_ipocurve(ei.icu); 
//      if ei.flag and IPO_VISIBLE
//      then
//      begin 
//        boundbox_ipocurve(ei.icu); 
//        sort_time_ipocurve(ei.icu); 
//        if first<>nil 
//        then
//        begin 
//          G.sipo.v2d.tot:= ei.icu.totrct; 
//          first:= 0; 
//        end;
//        else
//        union_rctf(@(G.sipo.v2d.tot),@(ei.icu.totrct)); 
//      end;
//    end;
//  end;
//  v2d:= @(G.sipo.v2d); (* keylijnen? *)
//  if G.sipo.blocktype=ID_KE
//  then
//  begin 
//    key:= {!!!a type cast? =>} {pKey(}G.sipo.from; 
//    if key)and(key.block.first
//    then
//    begin 
//      kb:= key.block.first; 
//      if kb.pos<v2d.tot.ymin
//      then
//      v2d.tot.ymin:= kb.pos; 
//      kb:= key.block.last; 
//      if kb.pos>v2d.tot.ymax
//      then
//      v2d.tot.ymax:= kb.pos; 
//    end;
//  end;
//  (* is er geen curve? *)
//  if first<>nil 
//  then
//  begin 
//    v2d.tot.xmin:= 0.0; 
//    v2d.tot.xmax:= EFRA; 
//    v2d.tot.ymin:= -0.1; 
//    v2d.tot.ymax:= 1.1; 
//    if G.sipo.blocktype=ID_SEQ
//    then
//    begin 
//      v2d.tot.xmin:= -5.0; 
//      v2d.tot.xmax:= 105.0; 
//      v2d.tot.ymin:= -0.1; 
//      v2d.tot.ymax:= 1.1; 
//    end;
//  end;
//  G.sipo.tot:= v2d.tot; 
//  if doredraw<>nil 
//  then
//  begin 
//    (* als do_ipo altijd wordt aangeroepen: problemen met insertkey, bijvoorbeeld
//       * als alleen een 'loc' wordt ge-insert wordt de 'ob->rot' veranderd.
//       *)
//    if G.sipo.blocktype=ID_OB
//    then
//    begin 
//      (* clear delta loc,rot,size (bij ipo vrijgeven/deleten) *)
//      clear_delta_obipo(G.sipo.ipo); 
//    end;
//    do_ipo(G.sipo.ipo); 
//    addqueue(curarea.headwin,REDRAW,1); 
//    addqueue(curarea.win,REDRAW,1); 
//    allqueue(REDRAWBUTSANIM,0); 
//    if G.sipo.blocktype=ID_OB
//    then
//    begin 
//      ob:={!!!a type cast? =>} {pObject(}G.sipo.from; 
//      if ob)and(ob.type=OB_IKA
//      then
//      itterate_ika(ob); 
//      allqueue(REDRAWVIEW3D,0); 
//    end;
//    else
//    if G.sipo.blocktype=ID_MA
//    then
//    allqueue(REDRAWBUTSMAT,0); 
//    else
//    if G.sipo.blocktype=ID_WO
//    then
//    allqueue(REDRAWBUTSWORLD,0); 
//    else
//    if G.sipo.blocktype=ID_LA
//    then
//    allqueue(REDRAWBUTSLAMP,0); 
//    else
//    if G.sipo.blocktype=ID_CA
//    then
//    begin 
//      allqueue(REDRAWBUTSEDIT,0); 
//      allqueue(REDRAWVIEW3D,0); 
//    end;
//    else
//    if G.sipo.blocktype=ID_SEQ
//    then
//    clear_last_seq(); 
//    else
//    if G.sipo.blocktype=ID_KE
//    then
//    begin 
//      do_spec_key({!!!a type cast? =>} {pKey(}G.sipo.from); 
//      allqueue(REDRAWVIEW3D,0); 
//    end;
//    else
//    if G.sipo.blocktype=ID_CU
//    then
//    begin 
//      calc_curvepath(OBACT); 
//      allqueue(REDRAWVIEW3D,0); 
//    end;
//  end;
//  if G.sipo.showkey<>nil 
//  then
//  make_ipokey(); 
//end;
//
//procedure scale_editipo; 
//var
//ei: pEditIpo; 
//bezt: pBezTriple; 
//facx: float; 
//facy: float; 
//a: integer; 
//b: integer; 
//begin(* komt uit buttons, scale met G.sipo->tot rect *)
//  
//  
//  
//  
//  
//  
//  facx:= (G.sipo.tot.xmax-G.sipo.tot.xmin) div (G.sipo.v2d.tot.xmax-G.sipo.v2d.tot.xmin); 
//  facy:= (G.sipo.tot.ymax-G.sipo.tot.ymin) div (G.sipo.v2d.tot.ymax-G.sipo.v2d.tot.ymin); 
//  ei:= G.sipo.editipo; 
//  if ei=0
//  then
//  exit;
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN{!!!3 unknown typedef}: if; 
//    begin
//      bezt:= ei.icu.bezt; 
//      b:= ei.icu.totvert; 
//      while b{--} dec(b); 
//      do
//      begin 
//        bezt.vec[0][0]:= facx*(bezt.vec[0][0]-G.sipo.v2d.tot.xmin)+G.sipo.tot.xmin; 
//        bezt.vec[1][0]:= facx*(bezt.vec[1][0]-G.sipo.v2d.tot.xmin)+G.sipo.tot.xmin; 
//        bezt.vec[2][0]:= facx*(bezt.vec[2][0]-G.sipo.v2d.tot.xmin)+G.sipo.tot.xmin; 
//        bezt.vec[0][1]:= facy*(bezt.vec[0][1]-G.sipo.v2d.tot.ymin)+G.sipo.tot.ymin; 
//        bezt.vec[1][1]:= facy*(bezt.vec[1][1]-G.sipo.v2d.tot.ymin)+G.sipo.tot.ymin; 
//        bezt.vec[2][1]:= facy*(bezt.vec[2][1]-G.sipo.v2d.tot.ymin)+G.sipo.tot.ymin; 
//        inc(bezt); 
//      end;
//    end
//  end;
//  editipo_changed(1); 
//  allqueue(REDRAWIPO,0); 
//end;
//
//function get_ipo_to_edit{!!!3 unknown typedef}: pIpo; 
//var
//ob: pObject; 
//ma: pMaterial; 
//cu: pCurve; 
//key: pKey; 
//la: pLamp; 
//wo: pWorld; 
//ca: pCamera; 
//begin
//  {last_seq: pSequence; }{<= !!!5 external variable}
//  
//  
//  
//  
//  
//  
//  {*}from^:=0; 
//  ob:= OBACT; 
//  if G.sipo.blocktype=ID_OB
//  then
//  begin 
//    if ob=0
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    {*}from^:={!!!a type cast? =>} {pID(}ob; 
//    begin
//      result:= ob.ipo; 
//      exit;
//    end;
//  end;
//  else
//  if G.sipo.blocktype=ID_MA
//  then
//  begin 
//    if ob=0
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    ma:= give_current_material(ob,ob.actcol); {*}from^:={!!!a type cast? =>} {pID(}ma; 
//    if ma<>nil 
//    then
//    begin
//      result:= ma.ipo; 
//      exit;
//    end;
//  end;
//  else
//  if G.sipo.blocktype=ID_KE
//  then
//  begin 
//    if ob=0
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    key:= give_current_key(ob); {*}from^:={!!!a type cast? =>} {pID(}key; 
//    if key<>nil 
//    then
//    begin
//      result:= key.ipo; 
//      exit;
//    end;
//  end;
//  else
//  if G.sipo.blocktype=ID_SEQ
//  then
//  begin 
//    {*}from^:={!!!a type cast? =>} {pID(}last_seq; 
//    if last_seq<>nil 
//    then
//    begin
//      result:= last_seq.ipo; 
//      exit;
//    end;
//  end;
//  else
//  if G.sipo.blocktype=ID_CU
//  then
//  begin 
//    if ob=0
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    if ob.type<>OB_CURVE
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    cu:= ob.data; {*}from^:={!!!a type cast? =>} {pID(}cu; 
//    begin
//      result:= cu.ipo; 
//      exit;
//    end;
//  end;
//  else
//  if G.sipo.blocktype=ID_WO
//  then
//  begin 
//    wo:= G.scene.world; {*}from^:={!!!a type cast? =>} {pID(}wo; 
//    if wo<>nil 
//    then
//    begin
//      result:= wo.ipo; 
//      exit;
//    end;
//  end;
//  else
//  if G.sipo.blocktype=ID_LA
//  then
//  begin 
//    if ob=0
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    if ob.type<>OB_LAMP
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    la:= ob.data; {*}from^:={!!!a type cast? =>} {pID(}la; 
//    begin
//      result:= la.ipo; 
//      exit;
//    end;
//  end;
//  else
//  if G.sipo.blocktype=ID_CA
//  then
//  begin 
//    if ob=0
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    if ob.type<>OB_CAMERA
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    ca:= ob.data; {*}from^:={!!!a type cast? =>} {pID(}ca; 
//    if ca<>nil 
//    then
//    begin
//      result:= ca.ipo; 
//      exit;
//    end;
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//function ipo_rainbow(cur: integer;  tot: integer): uint; 
//var
//fac: float; 
//dfac: float; 
//sat: float; 
//begin
//  
//  
//  
//  dfac:= 1.0 div ({!!!a type cast? =>} {float(}tot+1.0); (* deze berekening zorgt voor twee verschillende cycles regenboogkleuren *)
//  if cur<tot div 2
//  then
//  fac:= cur*2.0*dfac; 
//  else
//  fac:= (cur-tot div 2)*2.0*dfac+dfac; 
//  sat:= 0.5; 
//  if fac>0.5)and(fac<0.8
//  then
//  sat:= 0.4; 
//  begin
//    result:= hsv_to_cpack(fac,sat,1.0); 
//    exit;
//  end;
//end;
//
//procedure make_ob_editipo(ob: pObject;  si: pSpaceIpo); 
//var
//ei: pEditIpo; 
//a: integer; 
//len: integer; 
//colipo: integer; 
//begin
//  
//  
//  
//  
//  ei:= si.editipo:=callocN(OB_TOTIPO*sizeof(EditIpo),'editipo'); 
//  si.totipo:= OB_TOTIPO; if{!!!e unknown token}
//  colipo:= 1; 
//  else
//  colipo:= 0; 
//  for{while} a:=0 to Pred(OB_TOTIPO) { a++}
//  do
//  begin 
//    getname_ob_ei(ob_ar[a],ei.name,colipo); 
//    ei.adrcode:= ob_ar[a]; if{!!!e unknown token}
//    ei.disptype:= IPO_DISPDEGR; 
//    else
//    if ei.adrcode=OB_LAY
//    then
//    ei.disptype:= IPO_DISPBITS; 
//    else
//    if ei.adrcode=OB_TIME
//    then
//    ei.disptype:= IPO_DISPTIME; 
//    ei.col:= ipo_rainbow(a,OB_TOTIPO); 
//    if colipo<>nil 
//    then
//    begin 
//      len:= lstrlen(ei.name); 
//      if len<>nil 
//      then
//      begin 
//        if ei.name[len-1 ]='R'
//        then
//        ei.col:= $5050FF; 
//        else
//        if ei.name[len-1 ]='G'
//        then
//        ei.col:= $50FF50; 
//        else
//        if ei.name[len-1 ]='B'
//        then
//        ei.col:= $FF7050; 
//      end;
//    end;
//    ei.icu:= find_ipocurve(si.ipo,ei.adrcode); 
//    if ei.icu<>nil 
//    then
//    begin 
//      ei.flag:= ei.icu.flag; 
//    end;
//    inc(ei); 
//  end;
//end;
//
//procedure make_seq_editipo(si: pSpaceIpo); 
//var
//ei: pEditIpo; 
//a: integer; 
//begin
//  
//  
//  ei:= si.editipo:=callocN(SEQ_TOTIPO*sizeof(EditIpo),'editipo'); 
//  si.totipo:= SEQ_TOTIPO; 
//  for{while} a:=0 to Pred(SEQ_TOTIPO) { a++}
//  do
//  begin 
//    getname_seq_ei(seq_ar[a],ei.name); 
//    ei.adrcode:= seq_ar[a]; 
//    ei.col:= ipo_rainbow(a,SEQ_TOTIPO); 
//    ei.icu:= find_ipocurve(si.ipo,ei.adrcode); 
//    if ei.icu<>nil 
//    then
//    begin 
//      ei.flag:= ei.icu.flag; 
//    end;
//    else
//    ei.flag:= ei.flag or (IPO_VISIBLE); 
//    inc(ei); 
//  end;
//end;
//
//procedure make_cu_editipo(si: pSpaceIpo); 
//var
//ei: pEditIpo; 
//a: integer; 
//begin
//  
//  
//  ei:= si.editipo:=callocN(CU_TOTIPO*sizeof(EditIpo),'editipo'); 
//  si.totipo:= CU_TOTIPO; 
//  for{while} a:=0 to Pred(CU_TOTIPO) { a++}
//  do
//  begin 
//    getname_cu_ei(cu_ar[a],ei.name); 
//    ei.adrcode:= cu_ar[a]; 
//    ei.col:= ipo_rainbow(a,CU_TOTIPO); 
//    ei.icu:= find_ipocurve(si.ipo,ei.adrcode); 
//    if ei.icu<>nil 
//    then
//    begin 
//      ei.flag:= ei.icu.flag; 
//    end;
//    else
//    ei.flag:= ei.flag or (IPO_VISIBLE); 
//    inc(ei); 
//  end;
//end;
//
//procedure make_key_editipo(si: pSpaceIpo); 
//var
//key: pKey; 
//ei: pEditIpo; 
//a: integer; 
//begin
//  
//  
//  
//  ei:= si.editipo:=callocN(KEY_TOTIPO*sizeof(EditIpo),'editipo'); 
//  si.totipo:= KEY_TOTIPO; 
//  for{while} a:=0 to Pred(KEY_TOTIPO) { a++}
//  do
//  begin 
//    getname_key_ei(key_ar[a],ei.name); 
//    ei.adrcode:= key_ar[a]; 
//    ei.col:= ipo_rainbow(a,KEY_TOTIPO); 
//    ei.icu:= find_ipocurve(si.ipo,ei.adrcode); 
//    if ei.icu<>nil 
//    then
//    begin 
//      ei.flag:= ei.icu.flag; 
//    end;
//    else
//    if a=0
//    then
//    ei.flag:= ei.flag or (IPO_VISIBLE); 
//    inc(ei); 
//  end;
//  ei:= si.editipo; 
//  key:= {!!!a type cast? =>} {pKey(}G.sipo.from; 
//  if key)and(key.type=KEY_RELATIVE
//  then
//  begin 
//    strcpy(ei.name,'----'); 
//  end;
//  else
//  begin 
//    ei.flag:= ei.flag or (IPO_VISIBLE); 
//  end;
//end;
//
//function texchannel_to_adrcode(channel: integer): integer; 
//begin
//  case channel of
//    0:
//    begin
//      begin
//        result:= MA_MAP1; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    1:
//    begin
//      begin
//        result:= MA_MAP2; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    2:
//    begin
//      begin
//        result:= MA_MAP3; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    3:
//    begin
//      begin
//        result:= MA_MAP4; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    4:
//    begin
//      begin
//        result:= MA_MAP5; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    5:
//    begin
//      begin
//        result:= MA_MAP6; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    6:
//    begin
//      begin
//        result:= MA_MAP7; 
//        exit;
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    7:
//    begin
//      begin
//        result:= MA_MAP8; 
//        exit;
//      end;
//    end;
//  end;{case?}
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//procedure make_mat_editipo(si: pSpaceIpo); 
//var
//ei: pEditIpo; 
//fac: float; 
//dfac: float; 
//sat: float; 
//a: integer; 
//len: integer; 
//begin
//  
//  
//  
//  
//  
//  
//  if si.from=0
//  then
//  exit;
//  ei:= si.editipo:=callocN(MA_TOTIPO*sizeof(EditIpo),'editipo'); 
//  si.totipo:= MA_TOTIPO; 
//  dfac:= 1.0 div (MA_TOTIPO+1); 
//  fac:= 0.0; 
//  for{while} a:=0 to Pred(MA_TOTIPO) { a++}
//  do
//  begin 
//    getname_mat_ei(ma_ar[a],ei.name); 
//    ei.adrcode:= ma_ar[a]; 
//    if ei.adrcode and MA_MAP1
//    then
//    begin 
//      ei.adrcode:= ei.adrcode - (MA_MAP1); 
//      ei.adrcode:= ei.adrcode or (texchannel_to_adrcode(si.channel)); 
//    end;
//    else
//    begin 
//      (* dit was weggecommentaard. Waarom? *)
//      if ei.adrcode=MA_MODE
//      then
//      ei.disptype:= IPO_DISPBITS; 
//    end;
//    ei.col:= ipo_rainbow(a,WO_TOTIPO); 
//    len:= lstrlen(ei.name); 
//    if len<>nil 
//    then
//    begin 
//      if ei.name[len-1 ]='R'
//      then
//      ei.col:= $5050FF; 
//      else
//      if ei.name[len-1 ]='G'
//      then
//      ei.col:= $50FF50; 
//      else
//      if ei.name[len-1 ]='B'
//      then
//      ei.col:= $FF7050; 
//    end;
//    ei.icu:= find_ipocurve(si.ipo,ei.adrcode); 
//    if ei.icu<>nil 
//    then
//    begin 
//      ei.flag:= ei.icu.flag; 
//    end;
//    inc(ei); 
//  end;
//end;
//
//procedure make_world_editipo(si: pSpaceIpo); 
//var
//ei: pEditIpo; 
//fac: float; 
//dfac: float; 
//sat: float; 
//a: integer; 
//len: integer; 
//begin
//  
//  
//  
//  
//  
//  
//  if si.from=0
//  then
//  exit;
//  ei:= si.editipo:=callocN(WO_TOTIPO*sizeof(EditIpo),'editipo'); 
//  si.totipo:= WO_TOTIPO; 
//  dfac:= 1.0 div (WO_TOTIPO+1); 
//  fac:= 0.0; 
//  for{while} a:=0 to Pred(WO_TOTIPO) { a++}
//  do
//  begin 
//    getname_world_ei(wo_ar[a],ei.name); 
//    ei.adrcode:= wo_ar[a]; 
//    if ei.adrcode and MA_MAP1
//    then
//    begin 
//      ei.adrcode:= ei.adrcode - (MA_MAP1); 
//      ei.adrcode:= ei.adrcode or (texchannel_to_adrcode(si.channel)); 
//    end;
//    else
//    begin 
//      if ei.adrcode=MA_MODE
//      then
//      ei.disptype:= IPO_DISPBITS; 
//    end;
//    ei.col:= ipo_rainbow(a,MA_TOTIPO); 
//    len:= lstrlen(ei.name); 
//    if len<>nil 
//    then
//    begin 
//      if ei.name[len-1 ]='R'
//      then
//      ei.col:= $5050FF; 
//      else
//      if ei.name[len-1 ]='G'
//      then
//      ei.col:= $50FF50; 
//      else
//      if ei.name[len-1 ]='B'
//      then
//      ei.col:= $FF7050; 
//    end;
//    ei.icu:= find_ipocurve(si.ipo,ei.adrcode); 
//    if ei.icu<>nil 
//    then
//    begin 
//      ei.flag:= ei.icu.flag; 
//    end;
//    inc(ei); 
//  end;
//end;
//
//procedure make_lamp_editipo(si: pSpaceIpo); 
//var
//ei: pEditIpo; 
//a: integer; 
//begin
//  
//  
//  ei:= si.editipo:=callocN(LA_TOTIPO*sizeof(EditIpo),'editipo'); 
//  si.totipo:= LA_TOTIPO; 
//  for{while} a:=0 to Pred(LA_TOTIPO) { a++}
//  do
//  begin 
//    getname_la_ei(la_ar[a],ei.name); 
//    ei.adrcode:= la_ar[a]; 
//    if ei.adrcode and MA_MAP1
//    then
//    begin 
//      ei.adrcode:= ei.adrcode - (MA_MAP1); 
//      ei.adrcode:= ei.adrcode or (texchannel_to_adrcode(si.channel)); 
//    end;
//    ei.col:= ipo_rainbow(a,LA_TOTIPO); 
//    ei.icu:= find_ipocurve(si.ipo,ei.adrcode); 
//    if ei.icu<>nil 
//    then
//    begin 
//      ei.flag:= ei.icu.flag; 
//    end;
//    inc(ei); 
//  end;
//end;
//
//procedure make_camera_editipo(si: pSpaceIpo); 
//var
//ei: pEditIpo; 
//a: integer; 
//begin
//  
//  
//  ei:= si.editipo:=callocN(CAM_TOTIPO*sizeof(EditIpo),'editipo'); 
//  si.totipo:= CAM_TOTIPO; 
//  for{while} a:=0 to Pred(CAM_TOTIPO) { a++}
//  do
//  begin 
//    getname_cam_ei(cam_ar[a],ei.name); 
//    ei.adrcode:= cam_ar[a]; 
//    ei.col:= ipo_rainbow(a,CAM_TOTIPO); 
//    ei.icu:= find_ipocurve(si.ipo,ei.adrcode); 
//    if ei.icu<>nil 
//    then
//    begin 
//      ei.flag:= ei.icu.flag; 
//    end;
//    inc(ei); 
//  end;
//end;
//
//procedure make_editipo; 
//var
//ei: pEditIpo; 
//ob: pObject; 
//from: pID; 
//rf: prctf; 
//a: integer; 
//begin
//  
//  
//  
//  
//  
//  if G.sipo.editipo<>nil 
//  then
//  freeN(G.sipo.editipo); 
//  G.sipo.editipo:= 0; 
//  G.sipo.totipo:= 0; 
//  ob:= OBACT; 
//  G.sipo.ipo:= get_ipo_to_edit( and from); 
//  G.sipo.from:= from; 
//  if G.sipo.ipo<>nil 
//  then
//  G.sipo.showkey:= G.sipo.ipo.showkey; 
//  if G.sipo.blocktype=ID_OB
//  then
//  begin 
//    if ob=0
//    then
//    exit;
//    ob.ipowin:= ID_OB; 
//    make_ob_editipo(ob,G.sipo); 
//  end;
//  else
//  if G.sipo.blocktype=ID_MA
//  then
//  begin 
//    if ob=0
//    then
//    exit;
//    ob.ipowin:= ID_MA; 
//    make_mat_editipo(G.sipo); 
//  end;
//  else
//  if G.sipo.blocktype=ID_SEQ
//  then
//  make_seq_editipo(G.sipo); 
//  else
//  if G.sipo.blocktype=ID_CU
//  then
//  begin 
//    if ob=0
//    then
//    exit;
//    ob.ipowin:= ID_CU; 
//    make_cu_editipo(G.sipo); 
//  end;
//  else
//  if G.sipo.blocktype=ID_KE
//  then
//  begin 
//    if ob=0
//    then
//    exit;
//    ob.ipowin:= ID_KE; 
//    make_key_editipo(G.sipo); 
//  end;
//  else
//  if G.sipo.blocktype=ID_WO
//  then
//  begin 
//    make_world_editipo(G.sipo); 
//  end;
//  else
//  if G.sipo.blocktype=ID_LA
//  then
//  begin 
//    if ob=0
//    then
//    exit;
//    ob.ipowin:= ID_LA; 
//    make_lamp_editipo(G.sipo); 
//  end;
//  else
//  if G.sipo.blocktype=ID_CA
//  then
//  begin 
//    if ob=0
//    then
//    exit;
//    ob.ipowin:= ID_CA; 
//    make_camera_editipo(G.sipo); 
//  end;
//  if G.sipo.editipo=0
//  then
//  exit;
//  (* rowbut voor VISIBLE select *)
//  G.sipo.rowbut:= 0; 
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    if ei.flag and IPO_VISIBLE
//    then
//    G.sipo.rowbut:= G.sipo.rowbut or ((1 shl a)); 
//    if ei.icu<>nil 
//    then
//    ei.icu.flag:= ei.flag; 
//  end;
//  editipo_changed(0); 
//  if G.sipo.ipo<>nil 
//  then
//  begin 
//    rf:= @(G.sipo.ipo.cur); 
//    if rf.xmin<rf.xmax)and(rf.ymin<rf.ymax
//    then
//    G.v2d.cur:= {*}rf^; 
//  end;
//  else
//  begin 
//    if G.sipo.blocktype=ID_OB
//    then
//    begin 
//      G.v2d.cur.xmin:= 0.0; 
//      G.v2d.cur.xmax:= EFRA; 
//      G.v2d.cur.ymin:= -5.0; 
//      G.v2d.cur.ymax:= +5.0; 
//    end;
//    else
//    if G.sipo.blocktype=ID_CA
//    then
//    begin 
//      G.v2d.cur.xmin:= 0.0; 
//      G.v2d.cur.xmax:= EFRA; 
//      G.v2d.cur.ymin:= 0.0; 
//      G.v2d.cur.ymax:= 100.0; 
//    end;
//    else
//    
//    function ELEM4{!!!3 unknown typedef}: if; 
//    begin
//      G.v2d.cur.xmin:= -0.1; 
//      G.v2d.cur.xmax:= EFRA; 
//      G.v2d.cur.ymin:= -0.1; 
//      G.v2d.cur.ymax:= +1.1; 
//    end
//    else
//    if G.sipo.blocktype=ID_SEQ
//    then
//    begin 
//      G.v2d.cur.xmin:= -5.0; 
//      G.v2d.cur.xmax:= 105.0; 
//      G.v2d.cur.ymin:= -0.1; 
//      G.v2d.cur.ymax:= +1.1; 
//    end;
//    else
//    if G.sipo.blocktype=ID_KE
//    then
//    begin 
//      G.v2d.cur.xmin:= -0.1; 
//      G.v2d.cur.xmax:= EFRA; 
//      G.v2d.cur.ymin:= -0.1; 
//      G.v2d.cur.ymax:= +2.1; 
//    end;
//  end;
//end;
//
//procedure test_editipo; 
//var
//ipo: pIpo; 
//from: pID; 
//begin
//  
//  
//  if G.sipo.editipo=0
//  then
//  make_editipo(); 
//  else
//  begin 
//    ipo:= get_ipo_to_edit( and from); 
//    if G.sipo.ipo<>ipo)or(G.sipo.from<>from
//    then
//    make_editipo(); 
//  end;
//  (* kopie current view *)
//  if G.sipo.ipo<>nil 
//  then
//  G.sipo.ipo.cur:= G.v2d.cur; 
//end;
//(* ****************************************** *)
//totipo_edit: integer; 
//totipo_sel: integer; 
//totipo_vis: integer; 
//totipo_vert: integer; 
//totipo_vertsel: integer; 
//totipo_key: integer; 
//totipo_keysel: integer; 
//
//procedure get_status_editipo; 
//var
//ei: pEditIpo; 
//ik: pIpoKey; 
//bezt: pBezTriple; 
//bp: pBPoint; 
//a: integer; 
//b: integer; 
//begin
//  
//  
//  
//  
//  
//  
//  totipo_vis:= 0; 
//  totipo_sel:= 0; 
//  totipo_edit:= 0; 
//  totipo_vert:= 0; 
//  totipo_vertsel:= 0; 
//  totipo_key:= 0; 
//  totipo_keysel:= 0; 
//  if G.sipo.ipo)and(G.sipo.ipo.id.lib
//  then
//  exit;
//  ei:= G.sipo.editipo; 
//  if ei=0
//  then
//  exit;
//  for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//  do
//  begin 
//    if ei.flag and IPO_VISIBLE
//    then
//    begin 
//      inc(totipo_vis); 
//      if ei.flag and IPO_SELECT
//      then
//      inc(totipo_sel); 
//      if G.sipo.showkey)or((ei.flag and IPO_EDIT)
//      then
//      begin 
//        (* als showkey: wel de vertices tellen (voor grab) *)
//        if G.sipo.showkey=0
//        then
//        inc(totipo_edit); 
//        if ei.icu<>nil 
//        then
//        begin 
//          if ei.icu.bezt<>nil 
//          then
//          begin 
//            bezt:= ei.icu.bezt; 
//            b:= ei.icu.totvert; 
//            while b{--} dec(b); 
//            do
//            begin 
//              if ei.icu.ipo=IPO_BEZ
//              then
//              begin 
//                if bezt.f1 and 1
//                then
//                inc(totipo_vertsel); 
//                if bezt.f3 and 1
//                then
//                inc(totipo_vertsel); 
//                totipo_vert:= totipo_vert + (2); 
//              end;
//              if bezt.f2 and 1
//              then
//              inc(totipo_vertsel); 
//              inc(totipo_vert); 
//              inc(bezt); 
//            end;
//          end;
//          else
//          if ei.icu.bp<>nil 
//          then
//          begin 
//            bp:= ei.icu.bp; 
//            b:= ei.icu.totvert; 
//            while b{--} dec(b); 
//            do
//            begin 
//              if bp.f1 and 1
//              then
//              inc(totipo_vertsel); 
//              inc(totipo_vert); 
//              inc(bp); 
//            end;
//          end;
//        end;
//      end;
//    end;
//    inc(ei); 
//  end;
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    ik:= G.sipo.ipokey.first; 
//    while ik
//    do
//    begin 
//      inc(totipo_key); 
//      if ik.flag and 1
//      then
//      inc(totipo_keysel); 
//      ik:= ik.next; 
//    end;
//  end;
//end;
//
//procedure update_editipo_flags; 
//var
//ei: pEditIpo; 
//ik: pIpoKey; 
//flag: uint; 
//a: integer; 
//begin
//  
//  
//  
//  
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    ei.flag:= ei.flag and ( not IPO_VISIBLE); 
//    flag:= (1 shl a); 
//    if G.sipo.rowbut and flag
//    then
//    ei.flag:= ei.flag or (IPO_VISIBLE); 
//    if ei.icu<>nil 
//    then
//    ei.icu.flag:= ei.flag; 
//  end;
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    ik:= G.sipo.ipokey.first; 
//    while ik
//    do
//    begin 
//      for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//      do
//      begin 
//        if ik.data[a]<>nil 
//        then
//        begin 
//          if ik.flag and 1
//          then
//          begin 
//            ik.data[a].f1:= ik.data[a].f1 or (1); 
//            ik.data[a].f2:= ik.data[a].f2 or (1); 
//            ik.data[a].f3:= ik.data[a].f3 or (1); 
//          end;
//          else
//          begin 
//            ik.data[a].f1:= ik.data[a].f1 and ( not 1); 
//            ik.data[a].f2:= ik.data[a].f2 and ( not 1); 
//            ik.data[a].f3:= ik.data[a].f3 and ( not 1); 
//          end;
//        end;
//      end;
//      ik:= ik.next; 
//    end;
//  end;
//end;
//
//procedure set_editflag_editipo; 
//var
//ei: pEditIpo; 
//a: integer; 
//tot: integer;
// 
//ok: integer;
// (* van showkey direkt door naar editen geselecteerde punten *)
//begin
//  
//  
//  tot:=0; 
//  ok:=0; (* van showkey direkt door naar editen geselecteerde punten *)
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    G.sipo.showkey:= 0; 
//    if G.sipo.ipo<>nil 
//    then
//    G.sipo.ipo.showkey:= 0; 
//    ei:= G.sipo.editipo; 
//    while{for} 
//    a:= 0; 
//    {to} a<G.sipo.totipo
//    {;}
//    inc(a); 
//    inc(ei); 
//    do
//    ei.flag:= ei.flag or (IPO_SELECT); 
//    addqueue(curarea.headwin,REDRAW,1); 
//    allqueue(REDRAWVIEW3D,0); 
//  end;
//  get_status_editipo(); 
//  if G.sipo.ipo)and(G.sipo.ipo.id.lib
//  then
//  exit;
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    if ei.icu<>nil 
//    then
//    begin 
//      if ei.flag and IPO_VISIBLE
//      then
//      begin 
//        if totipo_edit=0)and((ei.flag and IPO_SELECT)
//        then
//        begin 
//          ei.flag:= ei.flag or (IPO_EDIT); 
//          ei.icu.flag:= ei.flag; 
//        end;
//        else
//        if totipo_edit)and((ei.flag and IPO_EDIT)
//        then
//        begin 
//          ei.flag:= ei.flag - (IPO_EDIT); 
//          ei.icu.flag:= ei.flag; 
//        end;
//        else
//        if totipo_vis=1
//        then
//        begin 
//          if ei.flag and IPO_EDIT
//          then
//          ei.flag:= ei.flag - (IPO_EDIT); 
//          else
//          ei.flag:= ei.flag or (IPO_EDIT); 
//          ei.icu.flag:= ei.flag; 
//        end;
//      end;
//    end;
//  end;
//  addqueue(curarea.win,REDRAW,1); 
//end;
//
//procedure swap_selectall_editipo; 
//var
//ob: pObject; 
//ei: pEditIpo; 
//ik: pIpoKey; 
//bezt: pBezTriple; 
//bp: pBPoint; 
//a: integer; 
//b: integer; 
//sel: integer;
// 
//begin
//  
//  
//  
//  
//  
//  
//  
//  sel:=0; 
//  deselectall_key(); 
//  get_status_editipo(); 
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    ik:= G.sipo.ipokey.first; 
//    while ik
//    do
//    begin 
//      if totipo_vertsel<>nil 
//      then
//      ik.flag:= ik.flag and ( not 1); 
//      else
//      ik.flag:= ik.flag or (1); 
//      ik:= ik.next; 
//    end;
//    update_editipo_flags(); 
//    if G.sipo.showkey)and(G.sipo.blocktype=ID_OB
//    then
//    begin 
//      ob:= OBACT; 
//      if ob)and((ob.ipoflag and OB_DRAWKEY)
//      then
//      draw_object_ext(BASACT); 
//    end;
//  end;
//  else
//  if totipo_edit=0
//  then
//  begin 
//    ei:= G.sipo.editipo; 
//    for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//    do
//    begin 
//      if ei.flag and IPO_VISIBLE
//      then
//      begin 
//        if totipo_sel<>nil 
//        then
//        ei.flag:= ei.flag and ( not IPO_SELECT); 
//        else
//        ei.flag:= ei.flag or (IPO_SELECT); 
//      end;
//      inc(ei); 
//    end;
//    update_editipo_flags(); 
//  end;
//  else
//  begin 
//    ei:= G.sipo.editipo; 
//    for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//    do
//    begin 
//      
//      function ISPOIN3{!!!3 unknown typedef}: if; 
//      begin
//        bezt:= ei.icu.bezt; 
//        bp:= ei.icu.bp; 
//        if bezt<>nil 
//        then
//        begin 
//          b:= ei.icu.totvert; 
//          while b{--} dec(b); 
//          do
//          begin 
//            if totipo_vertsel<>nil 
//            then
//            begin 
//              bezt.f1:= bezt.f2:=bezt.f3:=0; 
//            end;
//            else
//            begin 
//              bezt.f1:= bezt.f2:=bezt.f3:=1; 
//            end;
//            inc(bezt); 
//          end;
//        end;
//        else
//        if bp<>nil 
//        then
//        begin 
//          b:= ei.icu.totvert; 
//          while b{--} dec(b); 
//          do
//          begin 
//            if totipo_vertsel<>nil 
//            then
//            bp.f1:= 0; 
//            else
//            bp.f1:= 0; 
//            inc(bp); 
//          end;
//        end;
//      end
//      inc(ei); 
//    end;
//  end;
//  addqueue(curarea.win,REDRAW,1); 
//end;
//
//procedure swap_visible_editipo; 
//var
//ei: pEditIpo; 
//ob: pObject; 
//a: integer; 
//sel: integer;
// 
//begin
//  
//  
//  
//  sel:=0; 
//  get_status_editipo(); 
//  G.sipo.rowbut:= 0; 
//  ei:= G.sipo.editipo; 
//  for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//  do
//  begin 
//    if totipo_vis=0
//    then
//    begin 
//      if ei.icu<>nil 
//      then
//      begin 
//        ei.flag:= ei.flag or (IPO_VISIBLE); 
//        G.sipo.rowbut:= G.sipo.rowbut or ((1 shl a)); 
//      end;
//    end;
//    else
//    ei.flag:= ei.flag and ( not IPO_VISIBLE); 
//    inc(ei); 
//  end;
//  update_editipo_flags(); 
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    make_ipokey(); 
//    ob:= OBACT; 
//    if ob)and((ob.ipoflag and OB_DRAWKEY)
//    then
//    allqueue(REDRAWVIEW3D,0); 
//  end;
//  addqueue(curarea.win,REDRAW,1); 
//end;
//
//procedure deselectall_editipo; 
//var
//ei: pEditIpo; 
//ik: pIpoKey; 
//bezt: pBezTriple; 
//bp: pBPoint; 
//a: integer; 
//b: integer; 
//sel: integer;
// 
//begin
//  
//  
//  
//  
//  
//  
//  sel:=0; 
//  deselectall_key(); 
//  get_status_editipo(); 
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    ik:= G.sipo.ipokey.first; 
//    while ik
//    do
//    begin 
//      ik.flag:= ik.flag and ( not 1); 
//      ik:= ik.next; 
//    end;
//    update_editipo_flags(); 
//  end;
//  else
//  if totipo_edit=0
//  then
//  begin 
//    ei:= G.sipo.editipo; 
//    for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//    do
//    begin 
//      if ei.flag and IPO_VISIBLE
//      then
//      begin 
//        ei.flag:= ei.flag and ( not IPO_SELECT); 
//      end;
//      inc(ei); 
//    end;
//    update_editipo_flags(); 
//  end;
//  else
//  begin 
//    ei:= G.sipo.editipo; 
//    for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//    do
//    begin 
//      
//      function ISPOIN3{!!!3 unknown typedef}: if; 
//      begin
//        bezt:= ei.icu.bezt; 
//        bp:= ei.icu.bp; 
//        if bezt<>nil 
//        then
//        begin 
//          b:= ei.icu.totvert; 
//          while b{--} dec(b); 
//          do
//          begin 
//            bezt.f1:= bezt.f2:=bezt.f3:=0; 
//            inc(bezt); 
//          end;
//        end;
//        else
//        if bp<>nil 
//        then
//        begin 
//          b:= ei.icu.totvert; 
//          while b{--} dec(b); 
//          do
//          begin 
//            bp.f1:= 0; 
//            inc(bp); 
//          end;
//        end;
//      end
//      inc(ei); 
//    end;
//  end;
//  addqueue(curarea.win,REDRAW,1); 
//end;
//
//function findnearest_ipovert{!!!3 unknown typedef}: smallint; 
//var
//ei: pEditIpo; 
//bezt1: pBezTriple; 
//bp1: pBPoint; 
//a: integer; 
//b: integer; 
//dist: smallint;
// 
//temp: smallint; 
//mval: array [0..Pred(2)] of smallint; 
//hpoint: smallint;
// 
//begin(* selected krijgen een nadeel *)
//  (* in icu en (bezt of bp) wordt nearest weggeschreven *)
//  (* return 0 1 2: handlepunt *)
//  
//  
//  
//  
//  
//  dist:=100; 
//  
//  
//  hpoint:=0; {*}icu^:=0; 
//  {*}bezt^:=0; 
//  {*}bp^:=0; 
//  getmouseco_areawin(mval); 
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN3{!!!3 unknown typedef}: if; 
//    begin
//      if ei.icu.bezt<>nil 
//      then
//      begin 
//        bezt1:= ei.icu.bezt; 
//        b:= ei.icu.totvert; 
//        while b{--} dec(b); 
//        do
//        begin 
//          ipoco_to_areaco_noclip(bezt1.vec[0],bezt1.s[0]); 
//          ipoco_to_areaco_noclip(bezt1.vec[1],bezt1.s[1]); 
//          ipoco_to_areaco_noclip(bezt1.vec[2],bezt1.s[2]); 
//          if ei.disptype=IPO_DISPBITS
//          then
//          begin 
//            temp:= abs(mval[0]-bezt1.s[1][0]); 
//          end;
//          else
//          temp:= abs(mval[0]-bezt1.s[1][0])+abs(mval[1]-bezt1.s[1][1]); 
//          if bezt1.f2 and 1
//          then
//          temp:= temp + (5); 
//          if temp<dist
//          then
//          begin 
//            hpoint:= 1; {*}bezt^:=bezt1; 
//            dist:= temp; {*}icu^:=ei.icu; 
//            {*}bp^:=0; 
//          end;
//          if ei.disptype<>IPO_DISPBITS)and(ei.icu.ipo=IPO_BEZ
//          then
//          begin 
//            (* middelste punten een klein voordeel *)
//            temp:= -3+abs(mval[0]-bezt1.s[0][0])+abs(mval[1]-bezt1.s[0][1]); 
//            if bezt1.f1 and 1
//            then
//            temp:= temp + (5); 
//            if temp<dist
//            then
//            begin 
//              hpoint:= 0; {*}bezt^:=bezt1; 
//              dist:= temp; {*}icu^:=ei.icu; 
//              {*}bp^:=0; 
//            end;
//            temp:= abs(mval[0]-bezt1.s[2][0])+abs(mval[1]-bezt1.s[2][1]); 
//            if bezt1.f3 and 1
//            then
//            temp:= temp + (5); 
//            if temp<dist
//            then
//            begin 
//              hpoint:= 2; {*}bezt^:=bezt1; 
//              dist:= temp; {*}icu^:=ei.icu; 
//              {*}bp^:=0; 
//            end;
//          end;
//          inc(bezt1); 
//        end;
//      end;
//      else
//      if ei.icu.bp<>nil 
//      then
//      begin 
//        bp1:= ei.icu.bp; 
//        b:= ei.icu.totvert; 
//        while b{--} dec(b); 
//        do
//        begin 
//          ipoco_to_areaco_noclip(bp1.vec,bp1.s); 
//          temp:= abs(mval[0]-bp1.s[0])+abs(mval[1]-bp1.s[1]); 
//          if (bp1.f1 and 1)
//          then
//          temp:= temp + (5); 
//          if temp<dist
//          then
//          begin 
//            hpoint:= 0; {*}bp^:=bp1; 
//            dist:= temp; {*}icu^:=ei.icu; 
//            {*}bezt^:=0; 
//          end;
//          inc(bp1); 
//        end;
//      end;
//    end
//  end;
//  begin
//    result:= hpoint; 
//    exit;
//  end;
//end;
//
//procedure move_to_frame; 
//var
//ei: pEditIpo; 
//bezt: pBezTriple; 
//id: pID; 
//cfra: float; 
//a: integer; 
//b: integer; 
//ob: pObject;
// 
//begin
//  
//  
//  
//  
//  
//  
//  if G.sipo.editipo=0
//  then
//  exit;
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN{!!!3 unknown typedef}: if; 
//    var
//    ob: pObject;
//     
//    begin
//      if G.sipo.showkey)or((ei.flag and IPO_EDIT)
//      then
//      begin 
//        if ei.icu.bezt<>nil 
//        then
//        begin 
//          b:= ei.icu.totvert; 
//          bezt:= ei.icu.bezt; 
//          while b{--} dec(b); 
//          do
//          begin 
//            if BEZSELECTED(bezt)
//            then
//            begin 
//              cfra:= bezt.vec[1][0] div G.scene.r.framelen; 
//              id:= G.sipo.from; 
//              if id)and(GS(id.name)=ID_OB
//              then
//              begin 
//                ob:={!!!a type cast? =>} {pObject(}id; 
//                if ob.sf<>0.0)and((ob.ipoflag and OB_OFFS_OB)
//                then
//                begin 
//                  cfra:= cfra + (ob.sf div G.scene.r.framelen); 
//                end;
//              end;
//              CFRA:= ffloor(cfra+0.5); 
//              if CFRA<1
//              then
//              CFRA:= 1; 
//              do_global_buttons(B_NEWFRAME); 
//              break; {<= !!!b possible in "switch" - then remove this line}
//            end;
//            inc(bezt); 
//          end;
//        end;
//      end;
//    end
//  end;
//end;
//(* *********************************** *)
//
//procedure do_ipowin_buts(event: smallint); 
//begin
//  if (G.qual and LR_SHIFTKEY)=0
//  then
//  begin 
//    G.sipo.rowbut:= (1 shl event); 
//  end;
//  addqueue(curarea.win,REDRAW,1); 
//  update_editipo_flags(); 
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    make_ipokey(); 
//    if G.sipo.blocktype=ID_OB
//    then
//    allqueue(REDRAWVIEW3D,0); 
//  end;
//end;
//
//procedure do_ipo_selectbuttons; 
//var
//ei: pEditIpo; 
//ei1: pEditIpo; 
//a: integer; 
//nr: integer; 
//mval: array [0..Pred(2)] of smallint; 
//begin
//  
//  
//  
//  
//  
//  if G.sipo.showkey<>nil 
//  then
//  exit;
//  (* geen editipo toestaan: editipo's naar selected omzetten *)
//  get_status_editipo(); 
//  if totipo_edit<>nil 
//  then
//  begin 
//    set_editflag_editipo(); 
//  end;
//  getmouseco_areawin(mval); 
//  (* welke *)
//  nr:= -(mval[1]-curarea.winy+30-G.sipo.butofs-IPOBUTY) div IPOBUTY; 
//  if nr>=0)and(nr<G.sipo.totipo
//  then
//  begin 
//    ei:= G.sipo.editipo; 
//    ei:= ei + (nr); 
//    if ei.icu<>nil 
//    then
//    begin 
//      if (ei.flag and IPO_VISIBLE)=0
//      then
//      begin 
//        ei.flag:= ei.flag or (IPO_VISIBLE); 
//        G.sipo.rowbut:= G.sipo.rowbut or ((1 shl nr)); 
//      end;
//      if (G.qual and LR_SHIFTKEY)=0
//      then
//      begin 
//        ei1:= G.sipo.editipo; 
//        for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//        do
//        begin 
//          ei1.flag:= ei1.flag and ( not IPO_SELECT); 
//          inc(ei1); 
//        end;
//      end;
//      if ei.flag and IPO_SELECT
//      then
//      begin 
//        ei.flag:= ei.flag and ( not IPO_SELECT); 
//      end;
//      else
//      begin 
//        ei.flag:= ei.flag or (IPO_SELECT); 
//      end;
//      update_editipo_flags(); 
//      addqueue(curarea.win,REDRAW,1); 
//    end;
//  end;
//end;
//(* ******************************************* *)
//
//function get_editipo: pEditIpo; 
//var
//ei: pEditIpo; 
//a: integer; 
//sel: integer;
// 
//begin
//  
//  
//  sel:=0; 
//  get_status_editipo(); 
//  if totipo_edit>1
//  then
//  begin 
//    error('Too many editipo's'); 
//    begin
//      result:= 0; 
//      exit;
//    end;
//  end;
//  if G.sipo.editipo=0
//  then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  ei:= G.sipo.editipo; 
//  for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//  do
//  begin 
//    if ei.flag and IPO_VISIBLE
//    then
//    begin 
//      if ei.flag and IPO_EDIT
//      then
//      begin
//        result:= ei; 
//        exit;
//      end;
//      else
//      if totipo_vis=1
//      then
//      begin
//        result:= ei; 
//        exit;
//      end;
//      if ei.flag and IPO_SELECT
//      then
//      begin 
//        if totipo_sel=1
//        then
//        begin
//          result:= ei; 
//          exit;
//        end;
//      end;
//    end;
//    inc(ei); 
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//function get_ipo(from: pID;  make: integer): pIpo; 
//var
//ob: pObject; 
//ma: pMaterial; 
//cu: pCurve; 
//seq: pSequence; 
//key: pKey; 
//wo: pWorld; 
//la: pLamp; 
//ca: pCamera; 
//ipo: pIpo;
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
//  ipo:=0; 
//  if GS(from.name)=ID_OB
//  then
//  begin 
//    ob:= {!!!a type cast? =>} {pObject(}from; 
//    if ob.id.lib<>nil 
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    ipo:= ob.ipo; 
//    if make)and(ipo=0
//    then
//    ipo:= ob.ipo:=add_ipo('ObIpo',ID_OB); 
//  end;
//  else
//  if GS(from.name)=ID_MA
//  then
//  begin 
//    ma:= {!!!a type cast? =>} {pMaterial(}from; 
//    if ma.id.lib<>nil 
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    ipo:= ma.ipo; 
//    if make)and(ipo=0
//    then
//    ipo:= ma.ipo:=add_ipo('MatIpo',ID_MA); 
//  end;
//  else
//  if GS(from.name)=ID_SEQ
//  then
//  begin 
//    seq:= {!!!a type cast? =>} {pSequence(}from; 
//    if seq.type and SEQ_EFFECT
//    then
//    begin 
//      ipo:= seq.ipo; 
//      if make)and(ipo=0
//      then
//      ipo:= seq.ipo:=add_ipo('SeqIpo',ID_SEQ); 
//    end;
//    else
//    begin
//      result:= 0; 
//      exit;
//    end;
//  end;
//  else
//  if GS(from.name)=ID_CU
//  then
//  begin 
//    cu:= {!!!a type cast? =>} {pCurve(}from; 
//    if cu.id.lib<>nil 
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    ipo:= cu.ipo; 
//    if make)and(ipo=0
//    then
//    ipo:= cu.ipo:=add_ipo('CuIpo',ID_CU); 
//  end;
//  else
//  if GS(from.name)=ID_KE
//  then
//  begin 
//    key:= {!!!a type cast? =>} {pKey(}from; 
//    if key.id.lib<>nil 
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    ipo:= key.ipo; 
//    if make)and(ipo=0
//    then
//    ipo:= key.ipo:=add_ipo('KeyIpo',ID_KE); 
//  end;
//  else
//  if GS(from.name)=ID_WO
//  then
//  begin 
//    wo:= {!!!a type cast? =>} {pWorld(}from; 
//    if wo.id.lib<>nil 
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    ipo:= wo.ipo; 
//    if make)and(ipo=0
//    then
//    ipo:= wo.ipo:=add_ipo('WoIpo',ID_WO); 
//  end;
//  else
//  if GS(from.name)=ID_LA
//  then
//  begin 
//    la:= {!!!a type cast? =>} {pLamp(}from; 
//    if la.id.lib<>nil 
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    ipo:= la.ipo; 
//    if make)and(ipo=0
//    then
//    ipo:= la.ipo:=add_ipo('LaIpo',ID_LA); 
//  end;
//  else
//  if GS(from.name)=ID_CA
//  then
//  begin 
//    ca:= {!!!a type cast? =>} {pCamera(}from; 
//    if ca.id.lib<>nil 
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    ipo:= ca.ipo; 
//    if make)and(ipo=0
//    then
//    ipo:= ca.ipo:=add_ipo('CaIpo',ID_CA); 
//  end;
//  else
//  begin
//    result:= 0; 
//    exit;
//  end;
//  begin
//    result:= ipo; 
//    exit;
//  end;
//end;
//
//function get_ipocurve(from: pID;  adrcode: integer): pIpoCurve; 
//var
//ipo: pIpo;
// 
//icu: pIpoCurve;
// (* return 0 als lib *)
//(* ook testen of ipo en ipocurve bestaan *)
//begin
//  ipo:=0; 
//  icu:=0; (* return 0 als lib *)
//  (* ook testen of ipo en ipocurve bestaan *)
//  ipo:= get_ipo(from,1); (* 1= make *)
//  if G.sipo<>nil 
//  then
//  G.sipo.ipo:= ipo; 
//  if ipo)and(ipo.id.lib=0
//  then
//  begin 
//    icu:= ipo.curve.first; 
//    while icu
//    do
//    begin 
//      if icu.adrcode=adrcode
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      icu:= icu.next; 
//    end;
//    if icu=0
//    then
//    begin 
//      icu:= callocN(sizeof(IpoCurve),'ipocurve'); 
//      icu.flag:= icu.flag or (IPO_VISIBLE); 
//      icu.blocktype:= GS(from.name); 
//      icu.adrcode:= adrcode; 
//      set_icu_vars(icu); 
//      addtail(@(ipo.curve),icu); 
//    end;
//  end;
//  begin
//    result:= icu; 
//    exit;
//  end;
//end;
//
//procedure insert_vert_ipo(icu: pIpoCurve;  x: float;  y: float); 
//var
//bp: pBPoint; 
//bezt: pBezTriple; 
//beztr: BezTriple; 
//newbezt: pBezTriple; 
//a: integer; 
//h1: integer; 
//h2: integer; 
//str: array [0..Pred(20)] of char; 
//begin
//  
//  
//  
//  
//  
//  
//  
//  
//  if icu.bp<>nil 
//  then
//  begin 
//    bp:= icu.bp; 
//  end;
//  else
//  begin 
//    bzero( and beztr,sizeof(BezTriple)); 
//    beztr.vec[1][0]:= x; 
//    beztr.vec[1][1]:= y; 
//    beztr.hide:= IPO_BEZ; 
//    beztr.f1:= beztr.f2:=beztr.f3:=SELECT; 
//    beztr.h1:= beztr.h2:=HD_AUTO; 
//    bezt:= icu.bezt; 
//    if bezt=0
//    then
//    begin 
//      icu.bezt:= callocN(sizeof(BezTriple),'beztriple'); *(icu.bezt):=beztr; 
//      icu.totvert:= 1; 
//    end;
//    else
//    begin 
//      (* alle vertices deselect *)
//      while{for} 
//      a:= 0; 
//      {to} a<icu.totvert
//      {;}
//      inc(a); 
//      inc(bezt); 
//      do
//      begin 
//        bezt.f1:= bezt.f2:=bezt.f3:=0; 
//      end;
//      bezt:= icu.bezt; 
//      while{for} 
//      a:= 0; 
//      {to} a<=icu.totvert
//      {;}
//      inc(a); 
//      inc(bezt); 
//      do
//      begin 
//        (* geen dubbele punten *)
//        if a<icu.totvert)and((bezt.vec[1][0]>x-IPOTHRESH)and(bezt.vec[1][0]<x+IPOTHRESH)
//        then
//        begin 
//          *(bezt):=beztr; 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        if a=icu.totvert)or(bezt.vec[1][0]>x
//        then
//        begin 
//          newbezt:= callocN((icu.totvert+1)*sizeof(BezTriple),'beztriple'); 
//          if a>0
//          then
//          memcpy(newbezt,icu.bezt,a*sizeof(BezTriple)); 
//          bezt:= newbezt+a; *(bezt):=beztr; 
//          if a<icu.totvert
//          then
//          memcpy(newbezt+a+1,icu.bezt+a,(icu.totvert-a)*sizeof(BezTriple)); 
//          freeN(icu.bezt); 
//          icu.bezt:= newbezt; 
//          inc(icu.totvert); 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//      end;
//    end;
//    calchandles_ipocurve(icu); 
//    (* handletype goedzetten *)
//    if icu.totvert>2
//    then
//    begin 
//      h1:= h2:=HD_AUTO; 
//      if a>0
//      then
//      h1:= (bezt-1).h2; 
//      if a<icu.totvert-1
//      then
//      h2:= (bezt+1).h1; 
//      bezt.h1:= h1; 
//      bezt.h2:= h2; 
//      calchandles_ipocurve(icu); 
//    end;
//  end;
//end;
//
//procedure add_vert_ipo; 
//var
//ei: pEditIpo; 
//x: float; 
//y: float; 
//val: integer; 
//mval: array [0..Pred(2)] of smallint; 
//begin
//  
//  
//  
//  
//  
//  if G.sipo.ipo)and(G.sipo.ipo.id.lib
//  then
//  exit;
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    G.sipo.showkey:= 0; 
//    free_ipokey(@G.sipo.ipokey); 
//  end;
//  getmouseco_areawin(mval); 
//  if mval[0]>G.v2d.mask.xmax
//  then
//  exit;
//  ei:= get_editipo(); 
//  if ei=0
//  then
//  exit;
//  areamouseco_to_ipoco(mval,@x,@y); 
//  if ei.icu=0
//  then
//  begin 
//    if G.sipo.from<>nil 
//    then
//    ei.icu:= get_ipocurve(G.sipo.from,ei.adrcode); 
//  end;
//  if ei.icu=0
//  then
//  exit;
//  if ei.disptype=IPO_DISPBITS
//  then
//  begin 
//    ei.icu.vartype:= IPO_BITS; 
//    val:= ffloor(y-0.5); 
//    if val<0
//    then
//    val:= 0; 
//    y:= {!!!a type cast? =>} {float(}(1 shl val); 
//  end;
//  insert_vert_ipo(ei.icu,x,y); 
//  ei.flag:= ei.flag or (IPO_SELECT); 
//  ei.icu.flag:= ei.flag; 
//  editipo_changed(1); 
//  (* voor zekerheid: als icu 0 was, of maar 1 curve visible *)
//end;
//
//procedure add_duplicate_editipo; 
//var
//ob: pObject; 
//ei: pEditIpo; 
//icu: pIpoCurve; 
//bezt: pBezTriple; 
//beztn: pBezTriple; 
//new: pBezTriple; 
//tot: integer; 
//a: integer; 
//b: integer; 
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
//  get_status_editipo(); 
//  if totipo_vertsel=0
//  then
//  exit;
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN3{!!!3 unknown typedef}: if; 
//    begin
//      if G.sipo.showkey)or((ei.flag and IPO_EDIT)
//      then
//      begin 
//        icu:= ei.icu; 
//        tot:= 0; 
//        b:= icu.totvert; 
//        bezt:= icu.bezt; (* hoeveel punten *)
//        while b{--} dec(b); 
//        do
//        begin 
//          if bezt.f2 and 1
//          then
//          inc(tot); 
//          inc(bezt); 
//        end;
//        if tot<>nil 
//        then
//        begin 
//          icu.totvert:= icu.totvert + (tot); 
//          new:= beztn:=mallocN(icu.totvert*sizeof(BezTriple),'bezt'); 
//          bezt:= icu.bezt; 
//          b:= icu.totvert-tot; 
//          while b{--} dec(b); 
//          do
//          begin 
//            {*}beztn^:=*bezt; 
//            if bezt.f2 and 1
//            then
//            begin 
//              beztn.f1:= beztn.f2:=beztn.f3:=0; 
//              inc(beztn); {*}beztn^:=*bezt; 
//            end;
//            inc(beztn); 
//            inc(bezt); 
//          end;
//          freeN(icu.bezt); 
//          icu.bezt:= new; 
//          calchandles_ipocurve(icu); 
//        end;
//      end;
//    end
//  end;
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    make_ipokey(); 
//    if G.sipo.blocktype=ID_OB
//    then
//    begin 
//      ob:= OBACT; 
//      if ob)and((ob.ipoflag and OB_DRAWKEY)
//      then
//      allqueue(REDRAWVIEW3D,0); 
//    end;
//  end;
//  transform_ipo('g'); 
//end;
//
//procedure remove_doubles_ipo; 
//var
//ei: pEditIpo; 
//ik: pIpoKey; 
//ikn: pIpoKey; 
//bezt: pBezTriple; 
//new: pBezTriple; 
//new1: pBezTriple; 
//newy: float; 
//val: float; 
//mode: integer; 
//a: integer; 
//b: integer; 
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
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN3{!!!3 unknown typedef}: if; 
//    begin
//      mode:= 0; (* OF de curve is selected OF in editmode OF in keymode *)
//      if G.sipo.showkey)or((ei.flag and IPO_EDIT)
//      then
//      mode:= 1; 
//      else
//      if ei.flag and IPO_SELECT
//      then
//      mode:= 2; 
//      if mode<>nil 
//      then
//      begin 
//        bezt:= ei.icu.bezt; 
//        new:= new1:=mallocN(ei.icu.totvert*sizeof(BezTriple),'newbezt'); {*}new^:=*bezt; 
//        b:= ei.icu.totvert-1; 
//        inc(bezt); 
//        while b{--} dec(b); 
//        do
//        begin 
//          (* mag er verwijderd worden? *)
//          if mode=2)or((bezt.f2 and 1)
//          then
//          begin 
//            (* verschillen de punten? *)
//            if fabs(bezt.vec[1][0]-new.vec[1][0])>0.9
//            then
//            begin 
//              inc(new); {*}new^:=*bezt; 
//            end;
//            else
//            begin 
//              (* gemiddelde *)
//              VecMidf(new.vec[0],new.vec[0],bezt.vec[0]); 
//              VecMidf(new.vec[1],new.vec[1],bezt.vec[1]); 
//              VecMidf(new.vec[2],new.vec[2],bezt.vec[2]); 
//              new.h1:= new.h2:=HD_FREE; 
//              dec(ei.icu.totvert); 
//            end;
//          end;
//          else
//          begin 
//            inc(new); {*}new^:=*bezt; 
//          end;
//          inc(bezt); 
//        end;
//        freeN(ei.icu.bezt); 
//        ei.icu.bezt:= new1; 
//        calchandles_ipocurve(ei.icu); 
//      end;
//    end
//  end;
//  editipo_changed(1); 
//  (* maakt ook ipokeys opnieuw! *)
//  (* dubbele keys weg *)
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    ik:= G.sipo.ipokey.first; 
//    ikn:= ik.next; 
//    while ik)and(ikn
//    do
//    begin 
//      if (ik.flag and 1))and((ikn.flag and 1)
//      then
//      begin 
//        if fabs(ik.val-ikn.val)<0.9
//        then
//        begin 
//          val:= (ik.val+ikn.val) div 2.0; 
//          for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//          do
//          begin 
//            if ik.data[a]<>nil 
//            then
//            ik.data[a].vec[1][0]:= val; 
//            if ikn.data[a]<>nil 
//            then
//            ikn.data[a].vec[1][0]:= val; 
//          end;
//        end;
//      end;
//      ik:= ikn; 
//      ikn:= ikn.next; 
//    end;
//    editipo_changed(1); 
//  end;
//  (* maakt ook ipokeys opnieuw! *)
//  deselectall_editipo(); 
//end;
//
//procedure join_ipo; 
//var
//ob: pObject; 
//ei: pEditIpo; 
//ik: pIpoKey; 
//icu: pIpoCurve; 
//bezt: pBezTriple; 
//beztn: pBezTriple; 
//new: pBezTriple; 
//val: float; 
//mode: integer; 
//tot: integer; 
//a: integer; 
//b: integer; 
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
//  get_status_editipo(); 
//  mode:= pupmenu('Join %t|All Selected %x1|Selected doubles %x2'); 
//  if mode=2
//  then
//  begin 
//    remove_doubles_ipo(); 
//    exit;
//  end;
//  else
//  if mode<>1
//  then
//  exit;
//  (* eerst: meerdere geselecteerde verts in 1 curve *)
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN3{!!!3 unknown typedef}: if; 
//    begin
//      if G.sipo.showkey)or((ei.flag and IPO_EDIT)
//      then
//      begin 
//        icu:= ei.icu; 
//        tot:= 0; 
//        b:= icu.totvert; 
//        bezt:= icu.bezt; (* hoeveel punten *)
//        while b{--} dec(b); 
//        do
//        begin 
//          if bezt.f2 and 1
//          then
//          inc(tot); 
//          inc(bezt); 
//        end;
//        if tot>1
//        then
//        begin 
//          dec(tot); 
//          icu.totvert:= icu.totvert - (tot); 
//          new:= mallocN(icu.totvert*sizeof(BezTriple),'bezt'); 
//          beztn:= new+1; 
//          tot:= 0; 
//          bezt:= icu.bezt; 
//          b:= icu.totvert+tot+1; (* het eerste punt wordt het nieuwe punt *)
//          while b{--} dec(b); 
//          do
//          begin 
//            if bezt.f2 and 1
//            then
//            begin 
//              if tot=0
//              then
//              {*}new^:=*bezt; 
//              else
//              begin 
//                VecAddf(new.vec[0],new.vec[0],bezt.vec[0]); 
//                VecAddf(new.vec[1],new.vec[1],bezt.vec[1]); 
//                VecAddf(new.vec[2],new.vec[2],bezt.vec[2]); 
//              end;
//              inc(tot); 
//            end;
//            else
//            begin 
//              {*}beztn^:=*bezt; 
//              inc(beztn); 
//            end;
//            inc(bezt); 
//          end;
//          VecMulf(new.vec[0],1.0 div ({!!!a type cast? =>} {float(}tot)); 
//          VecMulf(new.vec[1],1.0 div ({!!!a type cast? =>} {float(}tot)); 
//          VecMulf(new.vec[2],1.0 div ({!!!a type cast? =>} {float(}tot)); 
//          freeN(icu.bezt); 
//          icu.bezt:= new; 
//          sort_time_ipocurve(icu); 
//          calchandles_ipocurve(icu); 
//        end;
//      end;
//    end
//  end;
//  editipo_changed(1); 
//  (* dan: in keymode: meerdere geselecteerde keys samenvoegen *)
//  (* maakt ook ipokeys opnieuw! *)
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    ik:= G.sipo.ipokey.first; 
//    val:= 0.0; 
//    tot:= 0; 
//    while ik
//    do
//    begin 
//      if ik.flag and 1
//      then
//      begin 
//        for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//        do
//        begin 
//          if ik.data[a]<>nil 
//          then
//          begin 
//            val:= val + (ik.data[a].vec[1][0]); 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//        end;
//        inc(tot); 
//      end;
//      ik:= ik.next; 
//    end;
//    if tot>1
//    then
//    begin 
//      val:= val div ({!!!a type cast? =>} {float(}tot); 
//      ik:= G.sipo.ipokey.first; 
//      while ik
//      do
//      begin 
//        if ik.flag and 1
//        then
//        begin 
//          for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//          do
//          begin 
//            if ik.data[a]<>nil 
//            then
//            begin 
//              ik.data[a].vec[1][0]:= val; 
//            end;
//          end;
//        end;
//        ik:= ik.next; 
//      end;
//      editipo_changed(0); 
//    end;
//  end;
//  deselectall_editipo(); 
//end;
//
//procedure ipo_snapmenu; 
//var
//icu: pIpoCurve; 
//ei: pEditIpo; 
//bezt: pBezTriple; 
//dx: float; 
//a: integer; 
//b: integer; 
//event: smallint; 
//ok: smallint; 
//ok2: smallint; 
//seq: pSequence; 
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
//  event:= pupmenu('Snap %t|Horizontal %x1|To next %x2|To frame %x3|To current frame%x4'); 
//  if event<1
//  then
//  exit;
//  get_status_editipo(); 
//  ei:= G.sipo.editipo; 
//  while{for} 
//  b:= 0; 
//  {to} b<G.sipo.totipo
//  {;}
//  inc(b); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN3{!!!3 unknown typedef}: if; 
//    var
//    seq: pSequence; 
//    begin
//      ok2:= 0; 
//      if G.sipo.showkey<>nil 
//      then
//      ok2:= 1; 
//      else
//      if totipo_vert)and((ei.flag and IPO_EDIT)
//      then
//      ok2:= 2; 
//      else
//      if totipo_vert=0)and((ei.flag and IPO_SELECT)
//      then
//      ok2:= 3; 
//      if ok2<>nil 
//      then
//      begin 
//        bezt:= ei.icu.bezt; 
//        a:= ei.icu.totvert; 
//        while a{--} dec(a); 
//        do
//        begin 
//          ok:= 0; 
//          if totipo_vert<>nil 
//          then
//          begin 
//            if bezt.f2 and 1
//            then
//            ok:= 1; 
//          end;
//          else
//          ok:= 1; 
//          if ok<>nil 
//          then
//          begin 
//            if event=1
//            then
//            begin 
//              bezt.vec[0][1]:= bezt.vec[2][1]:=bezt.vec[1][1]; 
//              if bezt.h1=HD_AUTO)or(bezt.h1=HD_VECT
//              then
//              bezt.h1:= HD_ALIGN; 
//              if bezt.h2=HD_AUTO)or(bezt.h2=HD_VECT
//              then
//              bezt.h2:= HD_ALIGN; 
//            end;
//            else
//            if event=2
//            then
//            begin 
//              if a<>nil 
//              then
//              begin 
//                bezt.vec[0][1]:= bezt.vec[1][1]:=bezt.vec[2][1]:=(bezt+1).vec[1][1]; 
//                if bezt.h1=HD_AUTO)or(bezt.h1=HD_VECT
//                then
//                bezt.h1:= HD_ALIGN; 
//                if bezt.h2=HD_AUTO)or(bezt.h2=HD_VECT
//                then
//                bezt.h2:= HD_ALIGN; 
//              end;
//            end;
//            else
//            if event=3
//            then
//            begin 
//              bezt.vec[1][0]:= ffloor(bezt.vec[1][0]+0.5); 
//            end;
//            else
//            if event=4
//            then
//            begin 
//              (* to current frame *)
//              if ok2=1)or(ok2=2
//              then
//              begin 
//                if G.sipo.blocktype=ID_SEQ
//                then
//                begin 
//                  
//                  seq:= {!!!a type cast? =>} {pSequence(}G.sipo.from; 
//                  if seq<>nil 
//                  then
//                  begin 
//                    dx:= (CFRA-seq.startdisp); 
//                    dx:= 100.0*dx div ({!!!a type cast? =>} {float(}(seq.enddisp-seq.startdisp)); 
//                    dx:= dx - (bezt.vec[1][0]); 
//                  end;
//                end;
//                else
//                dx:= G.scene.r.framelen*CFRA-bezt.vec[1][0]; 
//                bezt.vec[0][0]:= bezt.vec[0][0] + (dx); 
//                bezt.vec[1][0]:= bezt.vec[1][0] + (dx); 
//                bezt.vec[2][0]:= bezt.vec[2][0] + (dx); 
//              end;
//            end;
//          end;
//          inc(bezt); 
//        end;
//        calchandles_ipocurve(ei.icu); 
//      end;
//    end
//  end;
//  editipo_changed(1); 
//end;
//
//procedure mouse_select_ipo; 
//var
//ob: pObject; 
//ei: pEditIpo; 
//actei: pEditIpo;
// 
//icu: pIpoCurve; 
//ik: pIpoKey; 
//actik: pIpoKey; 
//bezt: pBezTriple; 
//bp: pBPoint; 
//key: pKey; 
//kb: pKeyBlock; 
//actkb: pKeyBlock;
// 
//x: float; 
//y: float; 
//dist: float; 
//mindist: float; 
//a: integer; 
//oldflag: integer; 
//hand: integer; 
//ok: integer; 
//mval: array [0..Pred(2)] of smallint; 
//xo: smallint; 
//yo: smallint; 
//begin
//  
//  
//  actei:=0; 
//  
//  
//  
//  
//  
//  
//  
//  actkb:=0; 
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
//  if G.sipo.editipo=0
//  then
//  exit;
//  get_status_editipo(); 
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    getmouseco_areawin(mval); 
//    areamouseco_to_ipoco(mval,@x,@y); 
//    actik:= 0; 
//    mindist:= 1000.0; 
//    ik:= G.sipo.ipokey.first; 
//    while ik
//    do
//    begin 
//      dist:= fabs(ik.val-x); 
//      if ik.flag and 1
//      then
//      dist:= dist + (1.0); 
//      if dist<mindist
//      then
//      begin 
//        actik:= ik; 
//        mindist:= dist; 
//      end;
//      ik:= ik.next; 
//    end;
//    if actik<>nil 
//    then
//    begin 
//      oldflag:= actik.flag; 
//      if G.qual and LR_SHIFTKEY
//      then
//      ; 
//      else
//      deselectall_editipo(); 
//      if G.qual and LR_SHIFTKEY
//      then
//      begin 
//        if oldflag and 1
//        then
//        actik.flag:= actik.flag and ( not 1); 
//        else
//        actik.flag:= actik.flag or (1); 
//      end;
//      else
//      begin 
//        actik.flag:= actik.flag or (1); 
//      end;
//    end;
//  end;
//  else
//  if totipo_edit<>nil 
//  then
//  begin 
//    hand:= findnearest_ipovert(@icu, and bezt, and bp); 
//    if G.qual and LR_SHIFTKEY
//    then
//    begin 
//      if bezt<>nil 
//      then
//      begin 
//        if hand=1
//        then
//        begin 
//          if BEZSELECTED(bezt)
//          then
//          begin 
//            bezt.f1:= bezt.f2:=bezt.f3:=0; 
//          end;
//          else
//          begin 
//            bezt.f1:= bezt.f2:=bezt.f3:=1; 
//          end;
//        end;
//        else
//        if hand=0
//        then
//        begin 
//          if bezt.f1 and 1
//          then
//          bezt.f1:= 0; 
//          else
//          bezt.f1:= 1; 
//        end;
//        else
//        begin 
//          if bezt.f3 and 1
//          then
//          bezt.f3:= 0; 
//          else
//          bezt.f3:= 1; 
//        end;
//      end;
//      else
//      if bp<>nil 
//      then
//      bp.f1:= bp.f1 or (1); 
//    end;
//    else
//    begin 
//      deselectall_editipo(); 
//      if bezt<>nil 
//      then
//      begin 
//        if hand=1
//        then
//        begin 
//          bezt.f1:= bezt.f1 or (1); 
//          bezt.f2:= bezt.f2 or (1); 
//          bezt.f3:= bezt.f3 or (1); 
//        end;
//        else
//        if hand=0
//        then
//        bezt.f1:= bezt.f1 or (1); 
//        else
//        bezt.f3:= bezt.f3 or (1); 
//      end;
//      else
//      if bp<>nil 
//      then
//      bp.f1:= bp.f1 or (1); 
//    end;
//  end;
//  else
//  begin 
//    (* vertex keys ? *)
//    if G.sipo.blocktype=ID_KE)and(G.sipo.from
//    then
//    begin 
//      key:= {!!!a type cast? =>} {pKey(}G.sipo.from; 
//      ei:= G.sipo.editipo; 
//      if key.type=KEY_NORMAL)or((ei.flag and IPO_VISIBLE)
//      then
//      begin 
//        getmouseco_areawin(mval); 
//        areamouseco_to_ipoco(mval,@x,@y); 
//        mindist:= 20.0*(G.v2d.cur.ymax-G.v2d.cur.ymin) div {!!!a type cast? =>} {float(}curarea.winy; 
//        kb:= key.block.first; (* hoeveel is 20 pixels? *)
//        while kb
//        do
//        begin 
//          dist:= fabs(kb.pos-y); 
//          if kb.flag and SELECT
//          then
//          dist:= dist + (0.$1); 
//          if dist<mindist
//          then
//          begin 
//            actkb:= kb; 
//            mindist:= dist; 
//          end;
//          kb:= kb.next; 
//        end;
//        if actkb<>nil 
//        then
//        begin 
//          ok:= LongBool(1); 
//          if G.obedit)and((actkb.flag and 1)=0
//          then
//          begin 
//            ok:= okee('Copy Key after leaving EditMode'); 
//          end;
//          if ok<>nil 
//          then
//          begin 
//            (* doet ook alle keypos *)
//            deselectall_editipo(); 
//            actkb.flag:= actkb.flag or (1); 
//            showkeypos({!!!a type cast? =>} {pKey(}G.sipo.from,actkb); 
//            (* oldflag= actkb->flag; *)
//            (* if(G.qual & LR_SHIFTKEY); *)
//            (* else { *)
//            (*  deselectall_key(); *)
//            (* } *)
//            (* if(G.qual & LR_SHIFTKEY) { *)
//            (*  if(oldflag & 1) actkb->flag &= ~1; *)
//            (*  else actkb->flag |= 1; *)
//            (* } *)
//            (* else { *)
//            (* } *)
//            (* bereken keypos *)
//          end;
//        end;
//      end;
//    end;
//    (* select curve *)
//    if actkb=0
//    then
//    begin 
//      if totipo_vis=1
//      then
//      begin 
//        ei:= G.sipo.editipo; 
//        while{for} 
//        a:= 0; 
//        {to} a<G.sipo.totipo
//        {;}
//        inc(a); 
//        inc(ei); 
//        do
//        begin 
//          if ei.icu<>nil 
//          then
//          begin 
//            if ei.flag and IPO_VISIBLE
//            then
//            actei:= ei; 
//          end;
//        end;
//      end;
//      else
//      if totipo_vis>1
//      then
//      begin 
//        actei:= select_proj_ipo(0,0); 
//      end;
//      if actei<>nil 
//      then
//      oldflag:= actei.flag; 
//      if G.qual and LR_SHIFTKEY
//      then
//      ; 
//      else
//      deselectall_editipo(); 
//      if actei<>nil 
//      then
//      begin 
//        if G.qual and LR_SHIFTKEY
//        then
//        begin 
//          if oldflag and IPO_SELECT
//          then
//          actei.flag:= actei.flag and ( not IPO_SELECT); 
//          else
//          actei.flag:= actei.flag or (IPO_SELECT); 
//        end;
//        else
//        begin 
//          actei.flag:= actei.flag or (IPO_SELECT); 
//        end;
//      end;
//    end;
//  end;
//  update_editipo_flags(); 
//  force_draw(); 
//  if G.sipo.showkey)and(G.sipo.blocktype=ID_OB
//  then
//  begin 
//    ob:= OBACT; 
//    if ob)and((ob.ipoflag and OB_DRAWKEY)
//    then
//    draw_object_ext(BASACT); 
//  end;
//  getmouseco_areawin(mval); 
//  xo:= mval[0]; 
//  yo:= mval[1]; 
//  while get_mbut()@R_MOUSE
//  do
//  begin 
//    usleep(1); 
//    getmouseco_areawin(mval); 
//    if abs(mval[0]-xo)+abs(mval[1]-yo)>4
//    then
//    begin 
//      if actkb<>nil 
//      then
//      move_keys(); 
//      else
//      transform_ipo('g'); 
//      exit;
//    end;
//  end;
//end;
//
//procedure sethandles_ipo(code: integer); 
//var
//ei: pEditIpo; 
//bezt: pBezTriple; 
//a: integer; 
//b: integer; 
//ok: integer;
// 
//begin(* code==1: set autohandle *)
//  (* code==2: set vectorhandle *)
//  (* als code==3 (HD_ALIGN) toggelt het, vectorhandles worden HD_FREE *)
//  
//  
//  
//  
//  ok:=0; 
//  if G.sipo.ipo)and(G.sipo.ipo.id.lib
//  then
//  exit;
//  if code=1)or(code=2
//  then
//  begin 
//    ei:= G.sipo.editipo; 
//    while{for} 
//    a:= 0; 
//    {to} a<G.sipo.totipo
//    {;}
//    inc(a); 
//    inc(ei); 
//    do
//    begin 
//      
//      function ISPOIN4{!!!3 unknown typedef}: if; 
//      begin
//        bezt:= ei.icu.bezt; 
//        b:= ei.icu.totvert; 
//        while b{--} dec(b); 
//        do
//        begin 
//          if bezt.f1)or(bezt.f3
//          then
//          begin 
//            if bezt.f1<>nil 
//            then
//            bezt.h1:= code; 
//            if bezt.f3<>nil 
//            then
//            bezt.h2:= code; 
//            if bezt.h1<>bezt.h2
//            then
//            begin 
//              if{!!!e unknown token}
//              bezt.h1:= HD_FREE; if{!!!e unknown token}
//              bezt.h2:= HD_FREE; 
//            end;
//          end;
//          inc(bezt); 
//        end;
//        calchandles_ipocurve(ei.icu); 
//      end
//    end;
//  end;
//  else
//  begin 
//    (* is er 1 handle NIET vrij: alles vrijmaken, else ALIGNED maken *)
//    ei:= G.sipo.editipo; 
//    while{for} 
//    a:= 0; 
//    {to} a<G.sipo.totipo
//    {;}
//    inc(a); 
//    inc(ei); 
//    do
//    begin 
//      
//      function ISPOIN4{!!!3 unknown typedef}: if; 
//      begin
//        bezt:= ei.icu.bezt; 
//        b:= ei.icu.totvert; 
//        while b{--} dec(b); 
//        do
//        begin 
//          if bezt.f1)and(bezt.h1
//          then
//          ok:= 1; 
//          if bezt.f3)and(bezt.h2
//          then
//          ok:= 1; 
//          if ok<>nil 
//          then
//          break; {<= !!!b possible in "switch" - then remove this line}
//          inc(bezt); 
//        end;
//      end;
//    end;
//    if ok<>nil 
//    then
//    ok:= HD_FREE; 
//    else
//    ok:= HD_ALIGN; 
//    ei:= G.sipo.editipo; 
//    while{for} 
//    a:= 0; 
//    {to} a<G.sipo.totipo
//    {;}
//    inc(a); 
//    inc(ei); 
//    do
//    begin 
//      
//      function ISPOIN4{!!!3 unknown typedef}: if; 
//      begin
//        bezt:= ei.icu.bezt; 
//        b:= ei.icu.totvert; 
//        while b{--} dec(b); 
//        do
//        begin 
//          if bezt.f1<>nil 
//          then
//          bezt.h1:= ok; 
//          if bezt.f3<>nil 
//          then
//          bezt.h2:= ok; 
//          inc(bezt); 
//        end;
//        calchandles_ipocurve(ei.icu); 
//      end;
//    end;
//  end;
//  editipo_changed(1); 
//end;
//
//procedure set_ipotype; 
//var
//ei: pEditIpo; 
//key: pKey; 
//kb: pKeyBlock; 
//a: integer; 
//event: smallint; 
//begin
//  
//  
//  
//  
//  
//  if G.sipo.ipo)and(G.sipo.ipo.id.lib
//  then
//  exit;
//  if G.sipo.showkey<>nil 
//  then
//  exit;
//  get_status_editipo(); 
//  if G.sipo.blocktype=ID_KE)and(totipo_edit=0)and(totipo_sel=0
//  then
//  begin 
//    key:= {!!!a type cast? =>} {pKey(}G.sipo.from; 
//    if key=0
//    then
//    exit;
//    event:= pupmenu('Key Type %t|Linear %x1|Cardinal %x2|B spline %x3'); 
//    if event<1
//    then
//    exit;
//    kb:= key.block.first; 
//    while kb
//    do
//    begin 
//      if kb.flag and SELECT
//      then
//      begin 
//        kb.type:= 0; 
//        if event=1
//        then
//        kb.type:= KEY_LINEAR; 
//        if event=2
//        then
//        kb.type:= KEY_CARDINAL; 
//        if event=3
//        then
//        kb.type:= KEY_BSPLINE; 
//      end;
//      kb:= kb.next; 
//    end;
//  end;
//  else
//  begin 
//    event:= pupmenu('Ipo Type %t|Constant %x1|Linear %x2|Bezier %x3'); 
//    if event<1
//    then
//    exit;
//    ei:= G.sipo.editipo; 
//    while{for} 
//    a:= 0; 
//    {to} a<G.sipo.totipo
//    {;}
//    inc(a); 
//    inc(ei); 
//    do
//    begin 
//      
//      function ISPOIN3{!!!3 unknown typedef}: if; 
//      begin
//        if event=1
//        then
//        ei.icu.ipo:= IPO_CONST; 
//        else
//        if event=2
//        then
//        ei.icu.ipo:= IPO_LIN; 
//        else
//        ei.icu.ipo:= IPO_BEZ; 
//      end
//    end;
//  end;
//  addqueue(curarea.win,REDRAW,1); 
//end;
//
//procedure borderselect_ipo; 
//var
//ei: pEditIpo; 
//ik: pIpoKey; 
//bezt: pBezTriple; 
//bp: pBPoint; 
//rect: rcti; 
//rectf: rctf; 
//a: integer; 
//b: integer; 
//val: integer; 
//ok: integer; 
//mval: array [0..Pred(2)] of smallint; 
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
//  get_status_editipo(); 
//  val:= get_border(@rect,3); 
//  if val<>nil 
//  then
//  begin 
//    mval[0]:= rect.xmin; 
//    mval[1]:= rect.ymin; 
//    areamouseco_to_ipoco(mval,@rectf.xmin,@rectf.ymin); 
//    mval[0]:= rect.xmax; 
//    mval[1]:= rect.ymax; 
//    areamouseco_to_ipoco(mval,@rectf.xmax,@rectf.ymax); 
//    if G.sipo.showkey<>nil 
//    then
//    begin 
//      ik:= G.sipo.ipokey.first; 
//      while ik
//      do
//      begin 
//        if rectf.xmin<ik.val)and(rectf.xmax>ik.val
//        then
//        begin 
//          if val=LEFTMOUSE
//          then
//          ik.flag:= ik.flag or (1); 
//          else
//          ik.flag:= ik.flag and ( not 1); 
//        end;
//        ik:= ik.next; 
//      end;
//      update_editipo_flags(); 
//    end;
//    else
//    if totipo_edit=0
//    then
//    begin 
//      if rect.xmin<rect.xmax)and(rect.ymin<rect.ymax
//      then
//      select_proj_ipo(@rectf,val); 
//    end;
//    else
//    begin 
//      ei:= G.sipo.editipo; 
//      while{for} 
//      a:= 0; 
//      {to} a<G.sipo.totipo
//      {;}
//      inc(a); 
//      inc(ei); 
//      do
//      begin 
//        
//        function ISPOIN3{!!!3 unknown typedef}: if; 
//        begin
//          if ei.icu.bezt<>nil 
//          then
//          begin 
//            b:= ei.icu.totvert; 
//            bezt:= ei.icu.bezt; 
//            while b{--} dec(b); 
//            do
//            begin 
//              ok:= 0; 
//              if ei.icu.ipo=IPO_BEZ
//              then
//              begin 
//                if in_rctf(@rectf,bezt.vec[0][0],bezt.vec[0][1])
//                then
//                ok:= 1; 
//                if in_rctf(@rectf,bezt.vec[2][0],bezt.vec[2][1])
//                then
//                ok:= 1; 
//              end;
//              if in_rctf(@rectf,bezt.vec[1][0],bezt.vec[1][1])
//              then
//              ok:= 1; 
//              if ok<>nil 
//              then
//              begin 
//                if val=LEFTMOUSE
//                then
//                begin 
//                  bezt.f1:= bezt.f1 or (1); 
//                  bezt.f2:= bezt.f2 or (1); 
//                  bezt.f3:= bezt.f3 or (1); 
//                end;
//                else
//                begin 
//                  bezt.f1:= bezt.f1 and ( not 1); 
//                  bezt.f2:= bezt.f2 and ( not 1); 
//                  bezt.f3:= bezt.f3 and ( not 1); 
//                end;
//              end;
//              inc(bezt); 
//            end;
//          end;
//          else
//          if ei.icu.bp<>nil 
//          then
//          begin 
//          end;
//        end
//      end;
//    end;
//    addqueue(curarea.win,REDRAW,1); 
//  end;
//end;
//
//procedure del_ipo; 
//var
//ei: pEditIpo; 
//bezt: pBezTriple; 
//bezt1: pBezTriple; 
//bezt2: pBezTriple; 
//bp: pBPoint; 
//bp1: pBPoint; 
//bp2: pBPoint; 
//fp: pfloat; 
//a: integer; 
//b: integer; 
//del: integer; 
//event: integer; 
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
//  get_status_editipo(); 
//  if G.sipo.ipo)and(G.sipo.ipo.id.lib
//  then
//  exit;
//  if totipo_edit=0)and(totipo_sel=0)and(totipo_vertsel=0
//  then
//  begin 
//    delete_key(); 
//    exit;
//  end;
//  if okee('Erase selected')=0
//  then
//  exit;
//  (* eerste doorloop, kunnen hele stukken weg? *)
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    del:= 0; 
//    if G.sipo.showkey=0)and(totipo_edit=0
//    then
//    begin 
//      
//      function ISPOIN3{!!!3 unknown typedef}: if; 
//      begin
//        del:= 1; 
//      end
//    end;
//    else
//    begin 
//      
//      function ISPOIN{!!!3 unknown typedef}: if; 
//      begin
//        if G.sipo.showkey)or((ei.flag and IPO_EDIT)
//        then
//        begin 
//          if ei.icu.bezt<>nil 
//          then
//          begin 
//            bezt:= ei.icu.bezt; 
//            b:= ei.icu.totvert; 
//            if b<>nil 
//            then
//            begin 
//              while b
//              do
//              begin 
//                if BEZSELECTED(bezt)
//                then
//                ; 
//                else
//                break; {<= !!!b possible in "switch" - then remove this line}
//                dec(b); 
//                inc(bezt); 
//              end;
//              if b=0
//              then
//              del:= 1; 
//            end;
//          end;
//          else
//          begin 
//            bp:= ei.icu.bp; 
//            b:= ei.icu.totvert; 
//            if b<>nil 
//            then
//            begin 
//              while b
//              do
//              begin 
//                if bp.f1 and 1
//                then
//                ; 
//                else
//                break; {<= !!!b possible in "switch" - then remove this line}
//                dec(b); 
//                inc(bp); 
//              end;
//              if b=0
//              then
//              del:= 1; 
//            end;
//          end;
//        end;
//      end;
//    end;
//    if del<>nil 
//    then
//    begin 
//      remlink(@(G.sipo.ipo.curve),ei.icu); 
//      if ei.icu.bezt<>nil 
//      then
//      freeN(ei.icu.bezt); 
//      if ei.icu.bp<>nil 
//      then
//      freeN(ei.icu.bp); 
//      freeN(ei.icu); 
//      ei.flag:= ei.flag and ( not IPO_SELECT); 
//      ei.flag:= ei.flag and ( not IPO_EDIT); 
//      ei.icu:= 0; 
//    end;
//  end;
//  ei:= G.sipo.editipo; (* tweede doorloop, kleine stukken weg: alleen curves *)
//  while{for} 
//  b:= 0; 
//  {to} b<G.sipo.totipo
//  {;}
//  inc(b); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN{!!!3 unknown typedef}: if; 
//    begin
//      if G.sipo.showkey)or((ei.flag and IPO_EDIT)
//      then
//      begin 
//        event:= 0; 
//        if ei.icu.bezt<>nil 
//        then
//        begin 
//          bezt:= ei.icu.bezt; 
//          for{while} a:=0 to Pred(ei.icu.totvert) { a++}
//          do
//          begin 
//            if BEZSELECTED(bezt)
//            then
//            begin 
//              memcpy(bezt,bezt+1,(ei.icu.totvert-a-1)*sizeof(BezTriple)); 
//              dec(ei.icu.totvert); 
//              dec(a); 
//              event:= 1; 
//            end;
//            else
//            inc(bezt); 
//          end;
//          if event<>nil 
//          then
//          begin 
//            bezt1:= mallocstructN(BezTriple,ei.icu.totvert,'delNurb'); 
//            memcpy(bezt1,ei.icu.bezt,(ei.icu.totvert)*sizeof(BezTriple)); 
//            freeN(ei.icu.bezt); 
//            ei.icu.bezt:= bezt1; 
//          end;
//        end;
//        else
//        if ei.icu.bp<>nil 
//        then
//        begin 
//          bp:= ei.icu.bp; 
//          for{while} a:=0 to Pred(ei.icu.totvert) { a++}
//          do
//          begin 
//            if bp.f1 and 1
//            then
//            begin 
//              memcpy(bp,bp+1,(ei.icu.totvert-a-1)*sizeof(BPoint)); 
//              dec(ei.icu.totvert); 
//              dec(a); 
//              event:= 1; 
//            end;
//            else
//            begin 
//              inc(bp); 
//            end;
//          end;
//          if event<>nil 
//          then
//          begin 
//            bp1:= mallocstructN(BPoint,ei.icu.totvert,'delNurb2'); 
//            memcpy(bp1,ei.icu.bp,(ei.icu.totvert)*sizeof(BPoint)); 
//            freeN(ei.icu.bp); 
//            ei.icu.bp:= bp1; 
//          end;
//        end;
//      end;
//    end;
//  end;
//  editipo_changed(1); 
//end;
//var
//ipocopybuf: ListBase = (0,0); 
//totipocopybuf: integer = 0; 
//
//procedure free_ipocopybuf; 
//var
//icu: pIpoCurve; 
//begin
//  
//  while icu:=ipocopybuf.first
//  do
//  begin 
//    if icu.bezt<>nil 
//    then
//    freeN(icu.bezt); 
//    if icu.bp<>nil 
//    then
//    freeN(icu.bezt); 
//    remlink(@ipocopybuf,icu); 
//    freeN(icu); 
//  end;
//  totipocopybuf:= 0; 
//end;
//
//procedure copy_editipo; 
//var
//ei: pEditIpo; 
//icu: pIpoCurve; 
//a: integer; 
//begin
//  
//  
//  
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    error('cannot copy\n'); 
//    exit;
//  end;
//  free_ipocopybuf(); 
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN{!!!3 unknown typedef}: if; 
//    begin
//      if (ei.flag and IPO_EDIT))or((ei.flag and IPO_SELECT)
//      then
//      begin 
//        icu:= callocN(sizeof(IpoCurve),'ipocopybuf'); {*}icu^:=*(ei.icu); 
//        addtail(@ipocopybuf,icu); 
//        if icu.bezt<>nil 
//        then
//        begin 
//          icu.bezt:= mallocN(icu.totvert*sizeof(BezTriple),'ipocopybuf'); 
//          memcpy(icu.bezt,ei.icu.bezt,icu.totvert*sizeof(BezTriple)); 
//        end;
//        if icu.bp<>nil 
//        then
//        begin 
//          icu.bp:= mallocN(icu.totvert*sizeof(BPoint),'ipocopybuf'); 
//          memcpy(icu.bp,ei.icu.bp,icu.totvert*sizeof(BPoint)); 
//        end;
//        inc(totipocopybuf); 
//      end;
//    end
//  end;
//  if totipocopybuf=0
//  then
//  error('Copybuf is empty'); 
//end;
//
//procedure paste_editipo; 
//var
//ei: pEditIpo; 
//icu: pIpoCurve; 
//a: integer; 
//ok: integer; 
//begin
//  
//  
//  
//  
//  if G.sipo.showkey<>nil 
//  then
//  exit;
//  if totipocopybuf=0
//  then
//  exit;
//  if G.sipo.ipo=0
//  then
//  exit;
//  if G.sipo.ipo)and(G.sipo.ipo.id.lib
//  then
//  exit;
//  get_status_editipo(); 
//  if totipo_vis=0
//  then
//  begin 
//    error('No visible splines'); 
//  end;
//  else
//  if totipo_vis<>totipocopybuf)and(totipo_sel<>totipocopybuf
//  then
//  begin 
//    error('Incompatible paste'); 
//  end;
//  else
//  begin 
//    (* problemen voorkomen: andere splines visible dan select *)
//    if totipo_vis=totipo_sel
//    then
//    totipo_vis:= 0; 
//    icu:= ipocopybuf.first; 
//    if icu=0
//    then
//    exit;
//    ei:= G.sipo.editipo; 
//    while{for} 
//    a:= 0; 
//    {to} a<G.sipo.totipo
//    {;}
//    inc(a); 
//    inc(ei); 
//    do
//    begin 
//      if ei.flag and IPO_VISIBLE
//      then
//      begin 
//        ok:= 0; 
//        if totipo_vis=totipocopybuf
//        then
//        ok:= 1; 
//        if totipo_sel=totipocopybuf)and((ei.flag and IPO_SELECT)
//        then
//        ok:= 1; 
//        if ok<>nil 
//        then
//        begin 
//          ei.icu:= get_ipocurve(G.sipo.from,ei.adrcode); 
//          if ei.icu=0
//          then
//          exit;
//          if ei.icu.bezt<>nil 
//          then
//          freeN(ei.icu.bezt); 
//          ei.icu.bezt:= 0; 
//          if ei.icu.bp<>nil 
//          then
//          freeN(ei.icu.bp); 
//          ei.icu.bp:= 0; 
//          ei.icu.totvert:= icu.totvert; 
//          ei.icu.flag:= ei.flag:=icu.flag; 
//          ei.icu.extrap:= icu.extrap; 
//          ei.icu.ipo:= icu.ipo; 
//          if icu.bezt<>nil 
//          then
//          begin 
//            ei.icu.bezt:= mallocN(icu.totvert*sizeof(BezTriple),'ipocopybuf'); 
//            memcpy(ei.icu.bezt,icu.bezt,icu.totvert*sizeof(BezTriple)); 
//          end;
//          if icu.bp<>nil 
//          then
//          begin 
//            ei.icu.bp:= mallocN(icu.totvert*sizeof(BPoint),'ipocopybuf'); 
//            memcpy(ei.icu.bp,icu.bp,icu.totvert*sizeof(BPoint)); 
//          end;
//          icu:= icu.next; 
//        end;
//      end;
//    end;
//    editipo_changed(1); 
//  end;
//end;
//
//procedure set_exprap_ipo(mode: integer); 
//var
//ei: pEditIpo; 
//a: integer; 
//begin
//  
//  
//  if G.sipo.ipo)and(G.sipo.ipo.id.lib
//  then
//  exit;
//  (* in geval van keys: altijd ok *)
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN{!!!3 unknown typedef}: if; 
//    begin
//      if (ei.flag and IPO_EDIT))or((ei.flag and IPO_SELECT))or((G.sipo.showkey)
//      then
//      begin 
//        ei.icu.extrap:= mode; 
//      end;
//    end
//  end;
//  editipo_changed(1); 
//end;
//
//function find_other_handles(eicur: pEditIpo;  ctime: float;  {!!!3 unknown typedef}): integer; 
//var
//ei: pEditIpo; 
//bezt: pBezTriple; 
//a: integer; 
//b: integer; 
//c: integer;
// 
//totvert: integer; 
//begin
//  
//  
//  
//  
//  c:=1; 
//  
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    if ei<>eicur)and(ei.icu)and((ei.flag and IPO_VISIBLE)
//    then
//    begin 
//      bezt:= ei.icu.bezt; 
//      totvert:= ei.icu.totvert; 
//      while{for} 
//      b:= 0; 
//      {to} b<totvert
//      {;}
//      inc(b); 
//      inc(bezt); 
//      do
//      begin 
//        if bezt.vec[1][0]<ctime+IPOTHRESH)and(bezt.vec[1][0]>ctime-IPOTHRESH
//        then
//        begin 
//          if c>2
//          then
//          begin
//            result:= 0; 
//            exit;
//          end;
//          beztar[c]:= bezt; 
//          inc(c); 
//        end;
//      end;
//    end;
//  end;
//  if c=3
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
//procedure set_speed_editipo(speed: float); 
//var
//ei: pEditIpo; 
//bezt: pBezTriple; 
//beztar: array [0..Pred(3)] of pBezTriple; 
//si: float; 
//co: float; 
//vec1: array [0..Pred(3)] of float; 
//vec2: array [0..Pred(3)] of float; 
//a: integer; 
//b: integer; 
//totvert: integer; 
//didit: integer;
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
//  didit:=0; 
//  if G.sipo.ipo)and(G.sipo.ipo.id.lib
//  then
//  exit;
//  (* uitgaande van 1 visible curve, selected punt, bijhorende punten: lencorr! *)
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN{!!!3 unknown typedef}: if; 
//    begin
//      bezt:= ei.icu.bezt; 
//      totvert:= ei.icu.totvert; 
//      while{for} 
//      b:= 0; 
//      {to} b<totvert
//      {;}
//      inc(b); 
//      inc(bezt); 
//      do
//      begin 
//        if BEZSELECTED(bezt)
//        then
//        begin 
//          beztar[0]:= bezt; 
//          if find_other_handles(ei,bezt.vec[1][0],beztar)
//          then
//          begin 
//            beztar[0].h1:= beztar[0].h2:=HD_ALIGN; 
//            beztar[1].h1:= beztar[1].h2:=HD_ALIGN; 
//            beztar[2].h1:= beztar[2].h2:=HD_ALIGN; 
//            vec1[0]:= (beztar[0].vec[1][1]-beztar[0].vec[0][1]) div (beztar[0].vec[1][0]-beztar[0].vec[0][0]); 
//            vec2[0]:= (beztar[0].vec[1][1]-beztar[0].vec[2][1]) div (beztar[0].vec[2][0]-beztar[0].vec[1][0]); 
//            vec1[1]:= (beztar[1].vec[1][1]-beztar[1].vec[0][1]) div (beztar[1].vec[1][0]-beztar[1].vec[0][0]); 
//            vec2[1]:= (beztar[1].vec[1][1]-beztar[1].vec[2][1]) div (beztar[1].vec[2][0]-beztar[1].vec[1][0]); 
//            vec1[2]:= (beztar[2].vec[1][1]-beztar[2].vec[0][1]) div (beztar[2].vec[1][0]-beztar[2].vec[0][0]); 
//            vec2[2]:= (beztar[2].vec[1][1]-beztar[2].vec[2][1]) div (beztar[2].vec[2][0]-beztar[2].vec[1][0]); 
//            Normalise(vec1); 
//            Normalise(vec2); 
//            VecMulf(vec1,speed); 
//            VecMulf(vec2,speed); 
//            beztar[0].vec[0][1]:= beztar[0].vec[1][1]-vec1[0]*(beztar[0].vec[1][0]-beztar[0].vec[0][0]); 
//            beztar[0].vec[2][1]:= beztar[0].vec[1][1]-vec2[0]*(beztar[0].vec[2][0]-beztar[0].vec[1][0]); 
//            beztar[1].vec[0][1]:= beztar[1].vec[1][1]-vec1[1]*(beztar[1].vec[1][0]-beztar[1].vec[0][0]); 
//            beztar[1].vec[2][1]:= beztar[1].vec[1][1]-vec2[1]*(beztar[1].vec[2][0]-beztar[1].vec[1][0]); 
//            beztar[2].vec[0][1]:= beztar[2].vec[1][1]-vec1[2]*(beztar[2].vec[1][0]-beztar[2].vec[0][0]); 
//            beztar[2].vec[2][1]:= beztar[2].vec[1][1]-vec2[2]*(beztar[2].vec[2][0]-beztar[2].vec[1][0]); 
//            didit:= 1; 
//          end;
//          else
//          begin 
//            error('Cannot set speed'); 
//          end;
//        end;
//      end;
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end
//  end;
//  if didit=0
//  then
//  error('Did not set speed'); 
//  editipo_changed(1); 
//  allqueue(REDRAWIPO,0); 
//end;
//
//procedure insertkey(id: pID;  adrcode: integer); 
//var
//icu: pIpoCurve; 
//ob: pObject; 
//poin: pinteger; 
//curval: float; 
//cfra: float; 
//type: integer; 
//begin
//  
//  
//  
//  
//  
//  
//  if id<>nil 
//  then
//  begin 
//    icu:= get_ipocurve(id,adrcode); 
//    if icu<>nil 
//    then
//    begin 
//      poin:= get_ipo_poin(id,icu,@type); 
//      if poin<>nil 
//      then
//      begin 
//        curval:= read_ipo_poin(poin,type); 
//        cfra:= frame_to_float(CFRA); 
//        if GS(id.name)=ID_OB
//        then
//        begin 
//          ob:= {!!!a type cast? =>} {pObject(}id; 
//          if ob.sf<>0.0)and((ob.ipoflag and OB_OFFS_OB)
//          then
//          begin 
//            (* eigenlijk frametofloat overniew berekenen! daarvoor CFRA als float door kunnen geven *)
//            cfra:= cfra - (ob.sf*G.scene.r.framelen); 
//          end;
//        end;
//        insert_vert_ipo(icu,cfra,curval); 
//      end;
//    end;
//  end;
//end;
//
//procedure insertkey_editipo; 
//var
//ei: pEditIpo; 
//ik: pIpoKey; 
//id: pID; 
//bezt: pBezTriple; 
//fp: pfloat; 
//cfra: float; 
//insertvals: pfloat; 
//a: integer; 
//nr: integer; 
//ok: integer; 
//tot: integer; 
//event: smallint; 
//ob: pObject;
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
//  if G.sipo.showkey<>nil 
//  then
//  event:= pupmenu('Insert KeyVertices %t|Current frame %x1|Selected Keys %x2'); 
//  else
//  event:= pupmenu('Insert KeyVertices %t|Current frame %x1'); 
//  if event<1
//  then
//  exit;
//  ei:= G.sipo.editipo; 
//  while{for} 
//  nr:= 0; 
//  {to} nr<G.sipo.totipo
//  {;}
//  inc(nr); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN{!!!3 unknown typedef}: if; 
//    var
//    ob: pObject;
//     
//    begin
//      ok:= 0; 
//      if G.sipo.showkey<>nil 
//      then
//      ok:= 1; 
//      else
//      if ei.flag and IPO_SELECT
//      then
//      ok:= 1; 
//      if ok<>nil 
//      then
//      begin 
//        (* aantal tellen *)
//        if event=1
//        then
//        tot:= 1; 
//        else
//        begin 
//          ik:= G.sipo.ipokey.first; 
//          tot:= 0; 
//          while ik
//          do
//          begin 
//            if ik.flag and 1
//            then
//            inc(tot); 
//            ik:= ik.next; 
//          end;
//        end;
//        if tot<>nil 
//        then
//        begin 
//          (* correctie voor ob timeoffs *)
//          cfra:= frame_to_float(CFRA); 
//          id:= G.sipo.from; 
//          if id)and(GS(id.name)=ID_OB
//          then
//          begin 
//            ob:={!!!a type cast? =>} {pObject(}id; 
//            if ob.sf<>0.0)and((ob.ipoflag and OB_OFFS_OB)
//            then
//            begin 
//              cfra:= cfra - (ob.sf*G.scene.r.framelen); 
//            end;
//          end;
//          else
//          if id)and(GS(id.name)=ID_SEQ
//          then
//          begin 
//            {last_seq: pSequence; }{<= !!!5 external variable}(* editsequence.c *)
//            if last_seq<>nil 
//            then
//            begin 
//              cfra:= 100.0*(cfra-last_seq.startdisp) div ({!!!a type cast? =>} {float(}(last_seq.enddisp-last_seq.startdisp)); 
//            end;
//          end;
//          insertvals:= mallocN(sizeof(float)*2*tot,'insertkey_editipo'); 
//          calc_ipo(G.sipo.ipo,cfra); 
//          (* zeker zijn dat icu->curval klopt *)
//          if event=1
//          then
//          begin 
//            insertvals[0]:= cfra; 
//            insertvals[1]:= ei.icu.curval; 
//          end;
//          else
//          begin 
//            fp:= insertvals; 
//            ik:= G.sipo.ipokey.first; 
//            while ik
//            do
//            begin 
//              if ik.flag and 1
//              then
//              begin 
//                calc_ipo(G.sipo.ipo,ik.val); 
//                fp[0]:= ik.val; 
//                fp[1]:= ei.icu.curval; 
//                fp:= fp + (2); 
//              end;
//              ik:= ik.next; 
//            end;
//          end;
//          fp:= insertvals; 
//          while{for} 
//          a:= 0; 
//          {to} a<tot
//          {;}
//          inc(a); 
//          fp:= fp + (2); 
//          do
//          begin 
//            insert_vert_ipo(ei.icu,fp[0],fp[1]); 
//          end;
//          freeN(insertvals); 
//          calc_ipo(G.sipo.ipo,{!!!a type cast? =>} {float(}CFRA); 
//        end;
//      end;
//    end
//  end;
//  allqueue(REDRAWIPO,0); 
//  allspace(REMAKEIPO,0); 
//end;
//
//procedure common_insertkey; 
//var
//base: pBase; 
//ob: pObject; 
//ma: pMaterial; 
//id: pID; 
//icu: pIpoCurve; 
//wo: pWorld; 
//la: pLamp; 
//ca: pCamera; 
//tlay: integer; 
//map: integer; 
//event: integer; 
//menustr: array [0..Pred(256)] of char; 
//ika: pIka;
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
//  if curarea.spacetype=SPACE_IPO
//  then
//  begin 
//    insertkey_editipo(); 
//  end;
//  else
//  if curarea.spacetype=SPACE_BUTS
//  then
//  begin 
//    if G.buts.mainb=BUTS_MAT
//    then
//    begin 
//      id:= G.buts.lockpoin; 
//      ma:= G.buts.lockpoin; 
//      if id<>nil 
//      then
//      begin 
//        event:= pupmenu('Insert Key %t|RGB%x0|Alpha%x1|HaSize%x2|Mode %x3|All Color%x10|Ofs%x12|Size%x13|All Mapping%x11'); 
//        if event=-1
//        then
//        exit;
//        map:= texchannel_to_adrcode(ma.texact); 
//        if event=0)or(event=10
//        then
//        begin 
//          insertkey(id,MA_COL_R); 
//          insertkey(id,MA_COL_G); 
//          insertkey(id,MA_COL_B); 
//        end;
//        if event=1)or(event=10
//        then
//        begin 
//          insertkey(id,MA_ALPHA); 
//        end;
//        if event=2)or(event=10
//        then
//        begin 
//          insertkey(id,MA_HASIZE); 
//        end;
//        if event=3)or(event=10
//        then
//        begin 
//          insertkey(id,MA_MODE); 
//        end;
//        if event=10
//        then
//        begin 
//          insertkey(id,MA_SPEC_R); 
//          insertkey(id,MA_SPEC_G); 
//          insertkey(id,MA_SPEC_B); 
//          insertkey(id,MA_REF); 
//          insertkey(id,MA_EMIT); 
//          insertkey(id,MA_AMB); 
//          insertkey(id,MA_SPEC); 
//          insertkey(id,MA_HARD); 
//          insertkey(id,MA_MODE); 
//        end;
//        if event=12)or(event=11
//        then
//        begin 
//          insertkey(id,map+MAP_OFS_X); 
//          insertkey(id,map+MAP_OFS_Y); 
//          insertkey(id,map+MAP_OFS_Z); 
//        end;
//        if event=13)or(event=11
//        then
//        begin 
//          insertkey(id,map+MAP_SIZE_X); 
//          insertkey(id,map+MAP_SIZE_Y); 
//          insertkey(id,map+MAP_SIZE_Z); 
//        end;
//        if event=11
//        then
//        begin 
//          insertkey(id,map+MAP_R); 
//          insertkey(id,map+MAP_G); 
//          insertkey(id,map+MAP_B); 
//          insertkey(id,map+MAP_DVAR); 
//          insertkey(id,map+MAP_COLF); 
//          insertkey(id,map+MAP_NORF); 
//          insertkey(id,map+MAP_VARF); 
//        end;
//      end;
//    end;
//    else
//    if G.buts.mainb=BUTS_WORLD
//    then
//    begin 
//      id:= G.buts.lockpoin; 
//      wo:= G.buts.lockpoin; 
//      if id<>nil 
//      then
//      begin 
//        event:= pupmenu('Insert Key %t|ZenRGB%x0|HorRGB%x1|Mist%x2|stars %x3|Ofs%x12|Size%x13'); 
//        if event=-1
//        then
//        exit;
//        map:= texchannel_to_adrcode(wo.texact); 
//        if event=0
//        then
//        begin 
//          insertkey(id,WO_ZEN_R); 
//          insertkey(id,WO_ZEN_G); 
//          insertkey(id,WO_ZEN_B); 
//        end;
//        if event=1
//        then
//        begin 
//          insertkey(id,WO_HOR_R); 
//          insertkey(id,WO_HOR_G); 
//          insertkey(id,WO_HOR_B); 
//        end;
//        if event=2
//        then
//        begin 
//          insertkey(id,WO_MISI); 
//          insertkey(id,WO_MISTDI); 
//          insertkey(id,WO_MISTSTA); 
//          insertkey(id,WO_MISTHI); 
//        end;
//        if event=3
//        then
//        begin 
//          insertkey(id,WO_STAR_R); 
//          insertkey(id,WO_STAR_G); 
//          insertkey(id,WO_STAR_B); 
//          insertkey(id,WO_STARDIST); 
//          insertkey(id,WO_STARSIZE); 
//        end;
//        if event=12
//        then
//        begin 
//          insertkey(id,map+MAP_OFS_X); 
//          insertkey(id,map+MAP_OFS_Y); 
//          insertkey(id,map+MAP_OFS_Z); 
//        end;
//        if event=13
//        then
//        begin 
//          insertkey(id,map+MAP_SIZE_X); 
//          insertkey(id,map+MAP_SIZE_Y); 
//          insertkey(id,map+MAP_SIZE_Z); 
//        end;
//      end;
//    end;
//    else
//    if G.buts.mainb=BUTS_LAMP
//    then
//    begin 
//      id:= G.buts.lockpoin; 
//      la:= G.buts.lockpoin; 
//      if id<>nil 
//      then
//      begin 
//        event:= pupmenu('Insert Key %t|RGB%x0|Energy%x1|Spotsi%x2|Ofs%x12|Size%x13'); 
//        if event=-1
//        then
//        exit;
//        map:= texchannel_to_adrcode(la.texact); 
//        if event=0
//        then
//        begin 
//          insertkey(id,LA_COL_R); 
//          insertkey(id,LA_COL_G); 
//          insertkey(id,LA_COL_B); 
//        end;
//        if event=1
//        then
//        begin 
//          insertkey(id,LA_ENERGY); 
//        end;
//        if event=2
//        then
//        begin 
//          insertkey(id,LA_SPOTSI); 
//        end;
//        if event=12
//        then
//        begin 
//          insertkey(id,map+MAP_OFS_X); 
//          insertkey(id,map+MAP_OFS_Y); 
//          insertkey(id,map+MAP_OFS_Z); 
//        end;
//        if event=13
//        then
//        begin 
//          insertkey(id,map+MAP_SIZE_X); 
//          insertkey(id,map+MAP_SIZE_Y); 
//          insertkey(id,map+MAP_SIZE_Z); 
//        end;
//      end;
//    end;
//    else
//    if G.buts.mainb=BUTS_EDIT
//    then
//    begin 
//      ob:= OBACT; 
//      if ob)and(ob.type=OB_CAMERA
//      then
//      begin 
//        id:= G.buts.lockpoin; 
//        ca:= G.buts.lockpoin; 
//        if id<>nil 
//        then
//        begin 
//          event:= pupmenu('Insert Key %t|Lens%x0|Clipping%x1'); 
//          if event=-1
//          then
//          exit;
//          if event=0
//          then
//          begin 
//            insertkey(id,CAM_LENS); 
//          end;
//          if event=1
//          then
//          begin 
//            insertkey(id,CAM_STA); 
//            insertkey(id,CAM_END); 
//          end;
//        end;
//      end;
//    end;
//    allqueue(REDRAWIPO,0); 
//    allspace(REMAKEIPO,0); 
//  end;
//  else
//  if curarea.spacetype=SPACE_VIEW3D
//  then
//  begin 
//    base:= FIRSTBASE; 
//    while base
//    do
//    begin 
//      if{!!!e unknown token}
//      break; {<= !!!b possible in "switch" - then remove this line}
//      base:= base.next; 
//    end;
//    if base=0
//    then
//    exit;
//    strcpy(menustr,'Insert Key %t|Loc%x0|Rot%x1|Size%x2|LocRot%x3|LocRotSize%x4|Layer%x5|Avail%x9'); 
//    if ob:=OBACT
//    then
//    begin 
//      if ob.type=OB_MESH
//      then
//      strcat(menustr,'| %x6|Mesh%x7'); 
//      else
//      if ob.type=OB_LATTICE
//      then
//      strcat(menustr,'| %x6|Lattice%x7'); 
//      else
//      if ob.type=OB_CURVE
//      then
//      strcat(menustr,'| %x6|Curve%x7'); 
//      else
//      if ob.type=OB_SURF
//      then
//      strcat(menustr,'| %x6|Surface%x7'); 
//      else
//      if ob.type=OB_IKA
//      then
//      strcat(menustr,'| %x6|Effector%x8'); 
//    end;
//    event:= pupmenu(menustr); 
//    if event=-1
//    then
//    exit;
//    if event=7
//    then
//    begin 
//      if ob.type=OB_MESH
//      then
//      insert_meshkey(ob.data); 
//      else
//      if{!!!e unknown token}
//      insert_curvekey(ob.data); 
//      else
//      if ob.type=OB_LATTICE
//      then
//      insert_lattkey(ob.data); 
//      allqueue(REDRAWIPO,0); 
//      allqueue(REDRAWBUTSANIM,0); 
//      exit;
//    end;
//    base:= FIRSTBASE; 
//    while base
//    do
//    begin 
//      
//      function TESTBASELIB{!!!3 unknown typedef}: if; 
//      var
//      ika: pIka;
//       
//      begin
//        id:= {!!!a type cast? =>} {pID(}(base.object); (* alle curves in ipo deselect *)
//        if base.object.ipo<>nil 
//        then
//        begin 
//          icu:= base.object.ipo.curve.first; 
//          while icu
//          do
//          begin 
//            icu.flag:= icu.flag and ( not IPO_SELECT); 
//            if event=9
//            then
//            insertkey(id,icu.adrcode); 
//            icu:= icu.next; 
//          end;
//        end;
//        if event=0)or(event=3)or(event=4
//        then
//        begin 
//          insertkey(id,OB_LOC_X); 
//          insertkey(id,OB_LOC_Y); 
//          insertkey(id,OB_LOC_Z); 
//        end;
//        if event=1)or(event=3)or(event=4
//        then
//        begin 
//          insertkey(id,OB_ROT_X); 
//          insertkey(id,OB_ROT_Y); 
//          insertkey(id,OB_ROT_Z); 
//        end;
//        if event=2)or(event=4
//        then
//        begin 
//          insertkey(id,OB_SIZE_X); 
//          insertkey(id,OB_SIZE_Y); 
//          insertkey(id,OB_SIZE_Z); 
//        end;
//        if event=5
//        then
//        begin 
//          (* localview weghalen *)
//          tlay:= base.object.lay; 
//          base.object.lay:= base.object.lay and ($FFFFFF); 
//          insertkey(id,OB_LAY); 
//          base.object.lay:= tlay; 
//        end;
//        if event=8
//        then
//        begin 
//          (* deze patch moet omdat duplicators de positie van effg veranderen *)
//          ika:=ob.data; 
//          VecMat4MulVecfl(ika.effg,ob.obmat,ika.effn); 
//          insertkey(id,OB_EFF_X); 
//          insertkey(id,OB_EFF_Y); 
//          insertkey(id,OB_EFF_Z); 
//        end;
//      end
//      base:= base.next; 
//    end;
//    allspace(REMAKEIPO,0); 
//    allqueue(REDRAWIPO,0); 
//    allqueue(REDRAWVIEW3D,0); 
//  end;
//end;
//(* **************************************************** *)
//(* IPOKEY:
// * 
// *   er zijn drie manieren om hiermee om te gaan:
// *   1. hieronder: voor tekenen en editen in Ipo window
// *   2. voor tekenen keys in View3D (zie ipo.c en drawobject.c)
// *   3. voor editen keys in View3D (hieronder en editobject.c)
// * 
// *)

procedure free_ipokey(lb: pListBase);
var
ik: pIpoKey;
begin

  ik:= lb^.first;
  while ik<>nil    do
  begin
    if ik^.data<>nil    then
    freeN(ik^.data);
    ik:= ik^.next;
  end;
  freelistN(lb);
end;

//procedure add_to_ipokey(lb: pListBase;  bezt: pBezTriple;  nr: integer;  len: integer); 
//var
//ik: pIpoKey; 
//ikn: pIpoKey; 
//begin
//  
//  
//  ik:= lb.first; 
//  while ik
//  do
//  begin 
//    if ik.val=bezt.vec[1][0]
//    then
//    begin 
//      if ik.data[nr]=0
//      then
//      begin 
//        (* dubbele punten! *)
//        ik.data[nr]:= bezt; 
//        if bezt.f2 and 1
//        then
//        ik.flag:= 1; 
//        exit;
//      end;
//    end;
//    else
//    if ik.val>bezt.vec[1][0]
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    ik:= ik.next; 
//  end;
//  ikn:= callocN(sizeof(IpoKey),'add_to_ipokey'); 
//  if ik<>nil 
//  then
//  insertlinkbefore(lb,ik,ikn); 
//  else
//  addtail(lb,ikn); 
//  ikn.data:= callocN(sizeof(float* )*len,'add_to_ipokey'); 
//  ikn.data[nr]:= bezt; 
//  ikn.val:= bezt.vec[1][0]; 
//  if bezt.f2 and 1
//  then
//  ikn.flag:= 1; 
//end;
//
//procedure make_ipokey_spec(lb: pListBase;  ipo: pIpo); 
//var
//icu: pIpoCurve; 
//ik: pIpoKey; 
//bezt: pBezTriple; 
//a: integer; 
//b: integer; 
//sel: integer; 
//desel: integer; 
//totvert: integer; 
//totcu: integer;
// 
//begin(* deze fie is voor PSXipo maken. *)
//  
//  
//  
//  
//  
//  
//  
//  
//  totcu:=0; 
//  if ipo=0)or(lb=0
//  then
//  exit;
//  free_ipokey(lb); 
//  icu:= ipo.curve.first; 
//  while icu
//  do
//  begin 
//    inc(totcu); 
//    icu:= icu.next; 
//  end;
//  a:= 0; 
//  icu:= ipo.curve.first; 
//  while icu
//  do
//  begin 
//    if icu.flag and IPO_VISIBLE
//    then
//    begin 
//      bezt:= icu.bezt; 
//      totvert:= icu.totvert; 
//      while{for} 
//      b:= 0; 
//      {to} b<totvert
//      {;}
//      inc(b); 
//      inc(bezt); 
//      do
//      begin 
//        add_to_ipokey(lb,bezt,a,totcu); 
//      end;
//      inc(a); 
//    end;
//    icu:= icu.next; 
//  end;
//end;
//
//procedure make_ipokey; 
//var
//ei: pEditIpo; 
//icu: pIpoCurve; 
//ik: pIpoKey; 
//lb: pListBase; 
//bezt: pBezTriple; 
//a: integer; 
//b: integer; 
//sel: integer; 
//desel: integer; 
//totvert: integer; 
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
//  lb:= @G.sipo.ipokey; 
//  free_ipokey(lb); 
//  ei:= G.sipo.editipo; 
//  if ei=0
//  then
//  exit;
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN{!!!3 unknown typedef}: if; 
//    begin
//      bezt:= ei.icu.bezt; 
//      totvert:= ei.icu.totvert; 
//      while{for} 
//      b:= 0; 
//      {to} b<totvert
//      {;}
//      inc(b); 
//      inc(bezt); 
//      do
//      begin 
//        add_to_ipokey(lb,bezt,a,G.sipo.totipo); 
//      end;
//      ei.flag:= ei.flag and ( not IPO_SELECT); 
//      ei.flag:= ei.flag and ( not IPO_EDIT); 
//      ei.icu.flag:= ei.flag; 
//    end
//  end;
//  ik:= lb.first; (* selectflags testen *)
//  while ik
//  do
//  begin 
//    sel:= desel:=0; 
//    for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//    do
//    begin 
//      if ik.data[a]<>nil 
//      then
//      begin 
//        bezt:= ik.data[a]; 
//        if bezt.f2 and 1
//        then
//        inc(sel); 
//        else
//        inc(desel); 
//      end;
//    end;
//    if sel)and(desel
//    then
//    sel:= 0; 
//    for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//    do
//    begin 
//      if ik.data[a]<>nil 
//      then
//      begin 
//        bezt:= ik.data[a]; 
//        if sel<>nil 
//        then
//        begin 
//          bezt.f1:= bezt.f1 or (1); 
//          bezt.f2:= bezt.f2 or (1); 
//          bezt.f3:= bezt.f3 or (1); 
//        end;
//        else
//        begin 
//          bezt.f1:= bezt.f1 and ( not 1); 
//          bezt.f2:= bezt.f2 and ( not 1); 
//          bezt.f3:= bezt.f3 and ( not 1); 
//        end;
//      end;
//    end;
//    if sel<>nil 
//    then
//    ik.flag:= 1; 
//    else
//    ik.flag:= 0; 
//    ik:= ik.next; 
//  end;
//  get_status_editipo(); 
//end;
//
//procedure make_ipokey_transform(ob: pObject;  lb: pListBase;  sel: integer); 
//var
//icu: pIpoCurve; 
//bezt: pBezTriple; 
//a: integer; 
//adrcode: integer; 
//ok: integer; 
//dloc: integer;
// 
//drot: integer;
// 
//dsize: integer;
// 
//begin
//  
//  
//  
//  
//  
//  dloc:=0; 
//  drot:=0; 
//  dsize:=0; 
//  if ob.ipo=0
//  then
//  exit;
//  if ob.ipo.showkey=0
//  then
//  exit;
//  (* testen: zijn er delta curves? *)
//  icu:= ob.ipo.curve.first; 
//  while icu
//  do
//  begin 
//    if icu.flag and IPO_VISIBLE
//    then
//    begin 
//      case icu.adrcode of
//        OB_DLOC_X,
//        OB_DLOC_Y,
//        OB_DLOC_Z:
//        begin
//          dloc:= 1; 
//        end;
//        OB_DROT_X,
//        OB_DROT_Y,
//        OB_DROT_Z:
//        begin
//          drot:= 1; 
//        end;
//        OB_DSIZE_X,
//        OB_DSIZE_Y,
//        OB_DSIZE_Z:
//        begin
//          dsize:= 1; 
//        end;
//        
//      end;{case?}
//    end;
//    icu:= icu.next; 
//  end;
//  icu:= ob.ipo.curve.first; 
//  while icu
//  do
//  begin 
//    if icu.flag and IPO_VISIBLE
//    then
//    begin 
//      ok:= 0; 
//      case icu.adrcode of
//        OB_DLOC_X,
//        OB_DLOC_Y,
//        OB_DLOC_Z,
//        OB_DROT_X,
//        OB_DROT_Y,
//        OB_DROT_Z,
//        OB_DSIZE_X,
//        OB_DSIZE_Y,
//        OB_DSIZE_Z:
//        begin
//          ok:= 1; 
//        end;
//        OB_LOC_X,
//        OB_LOC_Y,
//        OB_LOC_Z:
//        begin
//          if dloc=0
//          then
//          ok:= 1; 
//        end;
//        OB_ROT_X,
//        OB_ROT_Y,
//        OB_ROT_Z:
//        begin
//          if drot=0
//          then
//          ok:= 1; 
//        end;
//        OB_SIZE_X,
//        OB_SIZE_Y,
//        OB_SIZE_Z:
//        begin
//          if dsize=0
//          then
//          ok:= 1; 
//        end;
//        
//      end;{case?}
//      if ok<>nil 
//      then
//      begin 
//        for{while} a:=0 to Pred(OB_TOTIPO) { a++}
//        do
//        begin 
//          if icu.adrcode=ob_ar[a]
//          then
//          begin 
//            adrcode:= a; 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//        end;
//        bezt:= icu.bezt; 
//        a:= icu.totvert; 
//        while a{--} dec(a); 
//        do
//        begin 
//          if sel=0)or((bezt.f2 and 1)
//          then
//          begin 
//            add_to_ipokey(lb,bezt,adrcode,OB_TOTIPO); 
//          end;
//          inc(bezt); 
//        end;
//      end;
//    end;
//    icu:= icu.next; 
//  end;
//end;
//(* na verplaatsen vertices *)
//
//procedure update_ipokey_val; 
//var
//ik: pIpoKey; 
//a: integer; 
//begin
//  
//  
//  ik:= G.sipo.ipokey.first; 
//  while ik
//  do
//  begin 
//    for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//    do
//    begin 
//      if ik.data[a]<>nil 
//      then
//      begin 
//        ik.val:= ik.data[a].vec[1][0]; 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//    end;
//    ik:= ik.next; 
//  end;
//end;
//
//procedure set_tob_old(old: pfloat;  poin: pfloat); 
//begin
//  old[0]:= *(poin); 
//  old[3]:= *(poin-3); 
//  old[6]:= *(poin+3); 
//end;
//
//procedure set_ipo_pointers_transob(ik: pIpoKey;  tob: pTransOb); 
//var
//bezt: pBezTriple; 
//a: integer; 
//delta: integer;
// 
//begin
//  
//  
//  delta:=0; 
//  tob.locx:= tob.locy:=tob.locz:=0; 
//  tob.rotx:= tob.roty:=tob.rotz:=0; 
//  tob.sizex:= tob.sizey:=tob.sizez:=0; 
//  for{while} a:=0 to Pred(OB_TOTIPO) { a++}
//  do
//  begin 
//    if ik.data[a]<>nil 
//    then
//    begin 
//      bezt:= ik.data[a]; 
//      case ob_ar[a] of
//        OB_LOC_X,
//        OB_DLOC_X:
//        begin
//          tob.locx:= @(bezt.vec[1][1]); 
//        end;
//        OB_LOC_Y,
//        OB_DLOC_Y:
//        begin
//          tob.locy:= @(bezt.vec[1][1]); 
//        end;
//        OB_LOC_Z,
//        OB_DLOC_Z:
//        begin
//          tob.locz:= @(bezt.vec[1][1]); 
//        end;
//        OB_DROT_X:
//        begin
//          delta:= 1; 
//          {goto next_label;}{<= !!!d case label without "break"}
//        end;
//        OB_ROT_X:
//        begin
//          tob.rotx:= @(bezt.vec[1][1]); 
//        end;
//        OB_DROT_Y:
//        begin
//          delta:= 1; 
//          {goto next_label;}{<= !!!d case label without "break"}
//        end;
//        OB_ROT_Y:
//        begin
//          tob.roty:= @(bezt.vec[1][1]); 
//        end;
//        OB_DROT_Z:
//        begin
//          delta:= 1; 
//          {goto next_label;}{<= !!!d case label without "break"}
//        end;
//        OB_ROT_Z:
//        begin
//          tob.rotz:= @(bezt.vec[1][1]); 
//        end;
//        OB_SIZE_X,
//        OB_DSIZE_X:
//        begin
//          tob.sizex:= @(bezt.vec[1][1]); 
//        end;
//        OB_SIZE_Y,
//        OB_DSIZE_Y:
//        begin
//          tob.sizey:= @(bezt.vec[1][1]); 
//        end;
//        OB_SIZE_Z,
//        OB_DSIZE_Z:
//        begin
//          tob.sizez:= @(bezt.vec[1][1]); 
//        end;
//        
//      end;{case?}
//    end;
//  end;
//  (* oldvals voor o.a. undo *)
//  if tob.locx<>nil 
//  then
//  set_tob_old(tob.oldloc,tob.locx); 
//  if tob.locy<>nil 
//  then
//  set_tob_old(tob.oldloc+1,tob.locy); 
//  if tob.locz<>nil 
//  then
//  set_tob_old(tob.oldloc+2,tob.locz); 
//  (* bewaar de eerste oldrot, ivm mapping curves ('1'=10 graden) en correcte berekening *)
//  if tob.rotx<>nil 
//  then
//  set_tob_old(tob.oldrot+3,tob.rotx); 
//  if tob.roty<>nil 
//  then
//  set_tob_old(tob.oldrot+4,tob.roty); 
//  if tob.rotz<>nil 
//  then
//  set_tob_old(tob.oldrot+5,tob.rotz); 
//  (* bewaar de eerste oldsize, dit mag niet de dsize zijn! *)
//  if tob.sizex<>nil 
//  then
//  set_tob_old(tob.oldsize+3,tob.sizex); 
//  if tob.sizey<>nil 
//  then
//  set_tob_old(tob.oldsize+4,tob.sizey); 
//  if tob.sizez<>nil 
//  then
//  set_tob_old(tob.oldsize+5,tob.sizez); 
//  tob.flag:= TOB_IPO; 
//  if delta<>nil 
//  then
//  tob.flag:= tob.flag or (TOB_IPODROT); 
//end;
//
//procedure nextkey(elems: pListBase;  dir: integer); 
//var
//ik: pIpoKey; 
//previk: pIpoKey; 
//a: integer; 
//totsel: integer; 
//begin
//  
//  
//  
//  
//  if dir=1
//  then
//  ik:= elems.last; 
//  else
//  ik:= elems.first; 
//  previk:= 0; 
//  totsel:= 0; 
//  while ik
//  do
//  begin 
//    if ik.flag<>nil 
//    then
//    inc(totsel); 
//    if previk<>nil 
//    then
//    begin 
//      if G.qual and LR_SHIFTKEY
//      then
//      begin 
//        if ik.flag<>nil 
//        then
//        previk.flag:= 1; 
//      end;
//      else
//      previk.flag:= ik.flag; 
//    end;
//    previk:= ik; 
//    if dir=1
//    then
//    ik:= ik.prev; 
//    else
//    ik:= ik.next; 
//    if G.qual and LR_SHIFTKEY
//    then
//    ; 
//    else
//    if ik=0
//    then
//    previk.flag:= 0; 
//  end;
//  (* als geen een key select: *)
//  if totsel=0
//  then
//  begin 
//    if dir=1
//    then
//    ik:= elems.first; 
//    else
//    ik:= elems.last; 
//    if ik<>nil 
//    then
//    ik.flag:= 1; 
//  end;
//end;
//
//function float_to_frame(frame: float): integer; 
//var
//to: integer;
// 
//begin
//  to:={!!!a type cast? =>} {integer(}frame; 
//  if frame-to>0.5
//  then
//  inc(to); 
//  begin
//    result:= to; 
//    exit;
//  end;
//end;
//(* alleen extern aanroepen vanuit view3d queue *)
//
//procedure movekey_ipo(dir: integer); 
//var
//ik: pIpoKey; 
//toframe: float; 
//a: integer; 
//begin
//  
//  
//  
//  if G.sipo.showkey=0
//  then
//  exit;
//  ik:= G.sipo.ipokey.first; 
//  if dir=-1
//  then
//  begin 
//    while ik)and(float_to_frame(ik.val)<CFRA
//    do
//    begin 
//      toframe:= ik.val; 
//      ik:= ik.next; 
//    end;
//  end;
//  else
//  begin 
//    while ik)and(float_to_frame(ik.val)<=CFRA
//    do
//    begin 
//      ik:= ik.next; 
//    end;
//    if ik<>nil 
//    then
//    toframe:= ik.val; 
//  end;
//  a:= float_to_frame(toframe); 
//  if a<>CFRA)and(a>0
//  then
//  begin 
//    CFRA:= a; 
//    do_global_buttons(B_NEWFRAME); 
//  end;
//  allqueue(REDRAWVIEW3D,0); 
//  allqueue(REDRAWIPO,0); 
//  allspace(REMAKEIPO,0); 
//end;
//(* alleen extern aanroepen vanuit view3d queue *)
//
//procedure movekey_obipo(dir: integer); 
//var
//base: pBase; 
//ob: pObject; 
//elems: ListBase; 
//bezt: pBezTriple; 
//ik: pIpoKey; 
//previk: pIpoKey; 
//a: integer; 
//totsel: integer; 
//toframe: float;
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
//  toframe:=CFRA; 
//  base:= FIRSTBASE; 
//  while base
//  do
//  begin 
//    
//    function TESTBASE{!!!3 unknown typedef}: if; 
//    begin
//      ob:= base.object; 
//      if ob.ipo)and(ob.ipo.showkey
//      then
//      begin 
//        elems.first:= elems.last:=0; 
//        make_ipokey_transform(ob, and elems,0); 
//        if elems.first<>nil 
//        then
//        begin 
//          ik:= elems.first; 
//          if dir=-1
//          then
//          begin 
//            while ik)and(float_to_frame(ik.val)<CFRA
//            do
//            begin 
//              toframe:= ik.val; 
//              ik:= ik.next; 
//            end;
//          end;
//          else
//          begin 
//            while ik)and(float_to_frame(ik.val)<=CFRA
//            do
//            begin 
//              ik:= ik.next; 
//            end;
//            if ik<>nil 
//            then
//            toframe:= ik.val; 
//          end;
//          free_ipokey( and elems); 
//        end;
//      end;
//    end
//    base:= base.next; 
//  end;
//  a:= float_to_frame(toframe); 
//  if a<>CFRA)and(a>0
//  then
//  begin 
//    CFRA:= a; 
//    do_global_buttons(B_NEWFRAME); 
//  end;
//  allqueue(REDRAWVIEW3D,0); 
//  allqueue(REDRAWIPO,0); 
//  allspace(REMAKEIPO,0); 
//end;
//(* aanroepen vanuit ipo queue *)
//
//procedure nextkey_ipo(dir: integer); 
//var
//ik: pIpoKey; 
//a: integer; 
//begin
//  
//  
//  if G.sipo.showkey=0
//  then
//  exit;
//  nextkey(@G.sipo.ipokey,dir); 
//  ik:= G.sipo.ipokey.first; (* kopieeren naar beziers *)
//  while ik
//  do
//  begin 
//    for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//    do
//    begin 
//      if ik.data[a]<>nil 
//      then
//      ik.data[a].f1:= ik.data[a].f2:=ik.data[a].f3:=ik.flag; 
//    end;
//    ik:= ik.next; 
//  end;
//  allqueue(REDRAWIPO,0); 
//  if G.sipo.blocktype=ID_OB
//  then
//  allqueue(REDRAWVIEW3D,0); 
//end;
//(* alleen extern aanroepen vanuit view3d queue *)
//
//procedure nextkey_obipo(dir: integer); 
//var
//base: pBase; 
//ob: pObject; 
//elems: ListBase; 
//bezt: pBezTriple; 
//ik: pIpoKey; 
//previk: pIpoKey; 
//a: integer; 
//totsel: integer; (* problem: this doesnt work when you mix dLoc keys with Loc keys *)
//begin
//  
//  
//  
//  
//  
//  
//  
//  base:= FIRSTBASE; 
//  while base
//  do
//  begin 
//    
//    function TESTBASE{!!!3 unknown typedef}: if; 
//    begin
//      ob:= base.object; 
//      if (ob.ipoflag and OB_DRAWKEY))and(ob.ipo)and(ob.ipo.showkey
//      then
//      begin 
//        elems.first:= elems.last:=0; 
//        make_ipokey_transform(ob, and elems,0); 
//        if elems.first<>nil 
//        then
//        begin 
//          nextkey( and elems,dir); 
//          ik:= elems.first; (* kopieeren naar beziers *)
//          while ik
//          do
//          begin 
//            for{while} a:=0 to Pred(OB_TOTIPO) { a++}
//            do
//            begin 
//              if ik.data[a]<>nil 
//              then
//              ik.data[a].f1:= ik.data[a].f2:=ik.data[a].f3:=ik.flag; 
//            end;
//            ik:= ik.next; 
//          end;
//          free_ipokey( and elems); 
//        end;
//      end;
//    end
//    base:= base.next; 
//  end;
//  allqueue(REDRAWVIEW3D,0); 
//  allspace(REMAKEIPO,0); 
//  allqueue(REDRAWIPO,0); 
//end;
//(* **************************************************** *)
//
//procedure remake_ipo_transverts(transmain: pTransVert;  dvec: pfloat;  tot: integer); 
//var
//ei: pEditIpo; 
//tv: pTransVert; 
//bezt: pBezTriple; 
//bp: pBPoint; 
//a: integer; 
//b: integer; 
//begin
//  
//  
//  
//  
//  
//  
//  ei:= G.sipo.editipo; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN{!!!3 unknown typedef}: if; 
//    begin
//      if (ei.flag and IPO_EDIT))or(G.sipo.showkey
//      then
//      begin 
//        if ei.icu.bezt<>nil 
//        then
//        begin 
//          sort_time_ipocurve(ei.icu); 
//        end;
//      end;
//    end
//  end;
//  ei:= G.sipo.editipo; 
//  tv:= transmain; 
//  while{for} 
//  a:= 0; 
//  {to} a<G.sipo.totipo
//  {;}
//  inc(a); 
//  inc(ei); 
//  do
//  begin 
//    
//    function ISPOIN{!!!3 unknown typedef}: if; 
//    begin
//      if (ei.flag and IPO_EDIT))or(G.sipo.showkey
//      then
//      begin 
//        if ei.icu.bezt<>nil 
//        then
//        begin 
//          bezt:= ei.icu.bezt; 
//          b:= ei.icu.totvert; 
//          while b{--} dec(b); 
//          do
//          begin 
//            if ei.icu.ipo=IPO_BEZ
//            then
//            begin 
//              if bezt.f1 and 1
//              then
//              begin 
//                tv.loc:= bezt.vec[0]; 
//                inc(tv); 
//              end;
//              if bezt.f3 and 1
//              then
//              begin 
//                tv.loc:= bezt.vec[2]; 
//                inc(tv); 
//              end;
//            end;
//            if bezt.f2 and 1
//            then
//            begin 
//              tv.loc:= bezt.vec[1]; 
//              inc(tv); 
//            end;
//            inc(bezt); 
//          end;
//          testhandles_ipocurve(ei.icu); 
//        end;
//      end;
//    end;
//  end;
//  if G.sipo.showkey<>nil 
//  then
//  make_ipokey(); 
//  if dvec=0
//  then
//  exit;
//  tv:= transmain; 
//  while{for} 
//  a:= 0; 
//  {to} a<tot
//  {;}
//  inc(a); 
//  inc(tv); 
//  do
//  begin 
//    tv.oldloc[0]:= tv.loc[0]-dvec[0]; 
//    tv.oldloc[1]:= tv.loc[1]-dvec[1]; 
//  end;
//end;
//
//procedure transform_ipo(mode: integer); 
//var
//ei: pEditIpo; 
//icu: pIpoCurve; 
//bezt: pBezTriple; 
//transmain: pTransVert; 
//tv: pTransVert; 
//xref: float;
// 
//yref: float;
// 
//dx: float; 
//dy: float; 
//dvec: array [0..Pred(2)] of float; 
//min: array [0..Pred(3)] of float; 
//max: array [0..Pred(3)] of float; 
//vec: array [0..Pred(2)] of float; 
//div: float; 
//cent: array [0..Pred(2)] of float; 
//size: array [0..Pred(2)] of float; 
//sizefac: float; 
//tot: integer;
// 
//a: integer; 
//b: integer; 
//firsttime: integer;
// 
//afbreek: integer;
// 
//midtog: integer;
// 
//dosort: integer; 
//proj: integer; 
//event: ushort; 
//mval: array [0..Pred(2)] of smallint; 
//val: smallint; 
//xo: smallint; 
//yo: smallint; 
//xn: smallint; 
//yn: smallint; 
//xc: smallint; 
//yc: smallint; 
//str: array [0..Pred(32)] of char; 
//begin
//  
//  
//  
//  
//  
//  xref:=1.0; 
//  yref:=1.0; 
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
//  tot:=0; 
//  
//  
//  firsttime:=1; 
//  afbreek:=0; 
//  midtog:=0; 
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
//  if G.sipo.ipo)and(G.sipo.ipo.id.lib
//  then
//  exit;
//  if G.sipo.editipo=0
//  then
//  exit;
//  if mode='r'
//  then
//  exit;
//  (* vanuit gesture *)
//  INIT_MINMAX(min,max); 
//  get_status_editipo(); 
//  (* welke vertices doen mee *)
//  if totipo_vertsel<>nil 
//  then
//  begin 
//    tot:= totipo_vertsel; 
//    tv:= transmain:=callocN(tot*sizeof(TransVert),'transmain'); 
//    ei:= G.sipo.editipo; 
//    while{for} 
//    a:= 0; 
//    {to} a<G.sipo.totipo
//    {;}
//    inc(a); 
//    inc(ei); 
//    do
//    begin 
//      
//      function ISPOIN{!!!3 unknown typedef}: if; 
//      begin
//        if (ei.flag and IPO_EDIT))or(G.sipo.showkey
//        then
//        begin 
//          if ei.icu.bezt<>nil 
//          then
//          begin 
//            bezt:= ei.icu.bezt; 
//            b:= ei.icu.totvert; 
//            while b{--} dec(b); 
//            do
//            begin 
//              if ei.icu.ipo=IPO_BEZ
//              then
//              begin 
//                if bezt.f1 and 1
//                then
//                begin 
//                  tv.loc:= bezt.vec[0]; 
//                  VECCOPY(tv.oldloc,tv.loc); 
//                  if ei.disptype=IPO_DISPBITS
//                  then
//                  tv.flag:= 1; 
//                  DO_MINMAX2(bezt.vec[1],min,max); 
//                  (* let op: we nemen middelste vertex *)
//                  inc(tv); 
//                end;
//                if bezt.f3 and 1
//                then
//                begin 
//                  tv.loc:= bezt.vec[2]; 
//                  VECCOPY(tv.oldloc,tv.loc); 
//                  if ei.disptype=IPO_DISPBITS
//                  then
//                  tv.flag:= 1; 
//                  DO_MINMAX2(bezt.vec[1],min,max); 
//                  (* let op: we nemen middelste vertex *)
//                  inc(tv); 
//                end;
//              end;
//              if bezt.f2 and 1
//              then
//              begin 
//                tv.loc:= bezt.vec[1]; 
//                VECCOPY(tv.oldloc,tv.loc); 
//                if ei.disptype=IPO_DISPBITS
//                then
//                tv.flag:= 1; 
//                DO_MINMAX2(bezt.vec[1],min,max); 
//                inc(tv); 
//              end;
//              inc(bezt); 
//            end;
//          end;
//        end;
//      end
//    end;
//  end;
//  else
//  if totipo_edit=0)and(totipo_sel<>0
//  then
//  begin 
//    ei:= G.sipo.editipo; 
//    while{for} 
//    a:= 0; 
//    {to} a<G.sipo.totipo
//    {;}
//    inc(a); 
//    inc(ei); 
//    do
//    begin 
//      
//      function ISPOIN3{!!!3 unknown typedef}: if; 
//      begin
//        if ei.icu.bezt)and(ei.icu.ipo=IPO_BEZ
//        then
//        tot:= tot + (3*ei.icu.totvert); 
//        else
//        tot:= tot + (ei.icu.totvert); 
//      end;
//    end;
//    if tot=0
//    then
//    exit;
//    tv:= transmain:=callocN(tot*sizeof(TransVert),'transmain'); 
//    ei:= G.sipo.editipo; 
//    while{for} 
//    a:= 0; 
//    {to} a<G.sipo.totipo
//    {;}
//    inc(a); 
//    inc(ei); 
//    do
//    begin 
//      
//      function ISPOIN3{!!!3 unknown typedef}: if; 
//      begin
//        if ei.icu.bezt<>nil 
//        then
//        begin 
//          bezt:= ei.icu.bezt; 
//          b:= ei.icu.totvert; 
//          while b{--} dec(b); 
//          do
//          begin 
//            if ei.icu.ipo=IPO_BEZ
//            then
//            begin 
//              tv.loc:= bezt.vec[0]; 
//              VECCOPY(tv.oldloc,tv.loc); 
//              if ei.disptype=IPO_DISPBITS
//              then
//              tv.flag:= 1; 
//              inc(tv); 
//              tv.loc:= bezt.vec[2]; 
//              VECCOPY(tv.oldloc,tv.loc); 
//              if ei.disptype=IPO_DISPBITS
//              then
//              tv.flag:= 1; 
//              inc(tv); 
//            end;
//            tv.loc:= bezt.vec[1]; 
//            VECCOPY(tv.oldloc,tv.loc); 
//            if ei.disptype=IPO_DISPBITS
//            then
//            tv.flag:= 1; 
//            DO_MINMAX2(bezt.vec[1],min,max); 
//            inc(tv); 
//            inc(bezt); 
//          end;
//        end;
//      end;
//    end;
//  end;
//  if tot=0
//  then
//  begin 
//    if totipo_edit=0
//    then
//    move_keys(); 
//    exit;
//  end;
//  cent[0]:= (min[0]+max[0]) div 2.0; 
//  cent[1]:= (min[1]+max[1]) div 2.0; 
//  if G.sipo.showkey<>nil 
//  then
//  begin 
//    midtog:= 1; 
//    proj:= 1; 
//  end;
//  ipoco_to_areaco(cent,mval); 
//  xc:= mval[0]; 
//  yc:= mval[1]; 
//  getmouseco_areawin(mval); 
//  xo:= xn:=mval[0]; 
//  yo:= yn:=mval[1]; 
//  dvec[0]:= dvec[1]:=0.0; 
//  sizefac:= fsqrt({!!!a type cast? =>} {float(}((yc-yn)*(yc-yn)+(xn-xc)*(xn-xc))); 
//  if sizefac<2.0
//  then
//  sizefac:= 2.0; 
//  while afbreek=0
//  do
//  begin 
//    getmouseco_areawin(mval); 
//    if mval[0]<>xo)or(mval[1]<>yo)or(firsttime
//    then
//    begin 
//      if mode='g'
//      then
//      begin 
//        dx:= mval[0]-xo; 
//        dy:= mval[1]-yo; 
//        div:= G.v2d.mask.xmax-G.v2d.mask.xmin; 
//        dvec[0]:= dvec[0] + ((G.v2d.cur.xmax-G.v2d.cur.xmin)*(dx) div div); 
//        div:= G.v2d.mask.ymax-G.v2d.mask.ymin; 
//        dvec[1]:= dvec[1] + ((G.v2d.cur.ymax-G.v2d.cur.ymin)*(dy) div div); 
//        if midtog<>nil 
//        then
//        dvec[proj]:= 0.0; 
//        vec[0]:= dvec[0]; 
//        vec[1]:= dvec[1]; 
//        apply_keyb_grid(vec,0.0,1.0,0.1,U.flag and AUTOGRABGRID); 
//        (* vec wordt verderop nog gebruikt: remake_ipo_transverts *)
//        apply_keyb_grid(vec+1,0.0,1.0,0.1,0); 
//        tv:= transmain; 
//        while{for} 
//        a:= 0; 
//        {to} a<tot
//        {;}
//        inc(a); 
//        inc(tv); 
//        do
//        begin 
//          tv.loc[0]:= tv.oldloc[0]+vec[0]; 
//          if tv.flag=0
//          then
//          tv.loc[1]:= tv.oldloc[1]+vec[1]; 
//        end;
//        sprintf(str,'X: %.3f   Y: %.3f  ',vec[0],vec[1]); 
//        headerprint(str); 
//      end;
//      else
//      if mode='s'
//      then
//      begin 
//        size[0]:= size[1]:=(fsqrt({!!!a type cast? =>} {float(}((yc-mval[1])*(yc-mval[1])+(mval[0]-xc)*(mval[0]-xc)))) div sizefac; 
//        if midtog<>nil 
//        then
//        size[proj]:= 1.0; 
//        size[0]:= size[0] * (xref); 
//        size[1]:= size[1] * (yref); 
//        apply_keyb_grid(size,0.0,0.2,0.1,U.flag and AUTOSIZEGRID); 
//        apply_keyb_grid(size+1,0.0,0.2,0.1,U.flag and AUTOSIZEGRID); 
//        tv:= transmain; 
//        while{for} 
//        a:= 0; 
//        {to} a<tot
//        {;}
//        inc(a); 
//        inc(tv); 
//        do
//        begin 
//          tv.loc[0]:= size[0]*(tv.oldloc[0]-cent[0])+cent[0]; 
//          if tv.flag=0
//          then
//          tv.loc[1]:= size[1]*(tv.oldloc[1]-cent[1])+cent[1]; 
//        end;
//        sprintf(str,'sizeX: %.3f   sizeY: %.3f  ',size[0],size[1]); 
//        headerprint(str); 
//      end;
//      xo:= mval[0]; 
//      yo:= mval[1]; 
//      dosort:= 0; 
//      ei:= G.sipo.editipo; 
//      while{for} 
//      a:= 0; 
//      {to} a<G.sipo.totipo
//      {;}
//      inc(a); 
//      inc(ei); 
//      do
//      begin 
//        
//        function ISPOIN{!!!3 unknown typedef}: if; 
//        begin(* let op: als de tijd verkeerd is: niet de handles corrigeren *)
//          if test_time_ipocurve(ei.icu)
//          then
//          inc(dosort); 
//          else
//          testhandles_ipocurve(ei.icu); 
//        end;
//      end;
//      if dosort<>nil 
//      then
//      begin 
//        if mode='g'
//        then
//        remake_ipo_transverts(transmain,vec,tot); 
//        else
//        remake_ipo_transverts(transmain,0,tot); 
//      end;
//      if G.sipo.showkey<>nil 
//      then
//      update_ipokey_val(); 
//      calc_ipo(G.sipo.ipo,{!!!a type cast? =>} {float(}CFRA); 
//      (* update realtime *)
//      if G.sipo.lock<>nil 
//      then
//      begin 
//        if G.sipo.blocktype=ID_MA
//        then
//        begin 
//          force_draw_plus(SPACE_BUTS); 
//        end;
//        else
//        if G.sipo.blocktype=ID_KE
//        then
//        begin 
//          do_ob_key(OBACT); 
//          makeDispList(OBACT); 
//          force_draw_plus(SPACE_VIEW3D); 
//        end;
//        else
//        if G.sipo.blocktype=ID_OB
//        then
//        begin 
//          var
//          base: pBase = FIRSTBASE; 
//          while base
//          do
//          begin 
//            if base.object.ipo=G.sipo.ipo
//            then
//            do_ob_ipo(base.object); 
//            base:= base.next; 
//          end;
//          force_draw_plus(SPACE_VIEW3D); 
//        end;
//        else
//        force_draw(); 
//      end;
//      else
//      begin 
//        force_draw(); 
//      end;
//      firsttime:= 0; 
//    end;
//    else
//    usleep(1); 
//    while qtest()
//    do
//    begin 
//      event:= extern_qread(@val); 
//      if val<>nil 
//      then
//      begin 
//        case event of
//          ESCKEY,
//          LEFTMOUSE,
//          RIGHTMOUSE,
//          SPACEKEY,
//          RETKEY:
//          begin
//            afbreek:= 1; 
//          end;
//          MIDDLEMOUSE:
//          begin
//            if G.sipo.showkey=0
//            then
//            begin 
//              midtog:=  not midtog; 
//              if midtog<>nil 
//              then
//              begin 
//                if abs(mval[0]-xn)>abs(mval[1]-yn)
//                then
//                proj:= 1; 
//                else
//                proj:= 0; 
//                firsttime:= 1; 
//              end;
//            end;
//          end;
//          XKEY,
//          YKEY:
//          begin
//            if event=XKEY
//            then
//            xref:= -xref; 
//            else
//            if G.sipo.showkey=0
//            then
//            yref:= -yref; 
//            firsttime:= 1; 
//          end;
//          LEFTCTRLKEY,
//          RIGHTCTRLKEY:
//          begin
//            firsttime:= 1; 
//          end;
//          else
//          begin
//            if mode='g'
//            then
//            begin 
//              if G.qual and LR_CTRLKEY
//              then
//              begin 
//                if event=LEFTARROWKEY
//                then
//                begin 
//                  dvec[0]:= dvec[0] - (1.0); 
//                  firsttime:= 1; 
//                end;
//                else
//                if event=RIGHTARROWKEY
//                then
//                begin 
//                  dvec[0]:= dvec[0] + (1.0); 
//                  firsttime:= 1; 
//                end;
//                else
//                if event=UPARROWKEY
//                then
//                begin 
//                  dvec[1]:= dvec[1] + (1.0); 
//                  firsttime:= 1; 
//                end;
//                else
//                if event=DOWNARROWKEY
//                then
//                begin 
//                  dvec[1]:= dvec[1] - (1.0); 
//                  firsttime:= 1; 
//                end;
//              end;
//              else
//              arrowsmovecursor(event); 
//            end;
//            else
//            arrowsmovecursor(event); 
//          end;
//        end;{case?}
//      end;
//      if afbreek<>nil 
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//  end;
//  if event=ESCKEY)or(event=RIGHTMOUSE
//  then
//  begin 
//    tv:= transmain; 
//    while{for} 
//    a:= 0; 
//    {to} a<tot
//    {;}
//    inc(a); 
//    inc(tv); 
//    do
//    begin 
//      tv.loc[0]:= tv.oldloc[0]; 
//      tv.loc[1]:= tv.oldloc[1]; 
//    end;
//    dosort:= 0; 
//    ei:= G.sipo.editipo; 
//    while{for} 
//    a:= 0; 
//    {to} a<G.sipo.totipo
//    {;}
//    inc(a); 
//    inc(ei); 
//    do
//    begin 
//      
//      function ISPOIN{!!!3 unknown typedef}: if; 
//      begin
//        if (ei.flag and IPO_EDIT))or(G.sipo.showkey
//        then
//        begin 
//          if test_time_ipocurve(ei.icu)
//          then
//          begin 
//            dosort:= 1; 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//        end;
//      end;
//    end;
//    if dosort<>nil 
//    then
//    remake_ipo_transverts(transmain,0,tot); 
//    ei:= G.sipo.editipo; 
//    while{for} 
//    a:= 0; 
//    {to} a<G.sipo.totipo
//    {;}
//    inc(a); 
//    inc(ei); 
//    do
//    begin 
//      
//      function ISPOIN{!!!3 unknown typedef}: if; 
//      begin
//        if (ei.flag and IPO_EDIT))or(G.sipo.showkey
//        then
//        begin 
//          testhandles_ipocurve(ei.icu); 
//        end;
//      end;
//    end;
//    calc_ipo(G.sipo.ipo,{!!!a type cast? =>} {float(}CFRA); 
//  end;
//  editipo_changed(1); 
//  freeN(transmain); 
//end;
//
//procedure clever_numbuts_ipo; 
//var
//bezt: pBezTriple;
// 
//bezt1: pBezTriple; 
//key: pKey; 
//kb: pKeyBlock; 
//ei: pEditIpo; 
//tot: integer; 
//a: integer; 
//b: integer; 
//scale10: integer;
// 
//totbut: integer;
// 
//begin
//  bezt:=0; 
//  
//  
//  
//  float{!!!e unknown token},
//  delta[3]{!!!e unknown token},
//  old[3]{!!!e unknown token}; 
//  
//  
//  
//  scale10:=0; 
//  totbut:=2; 
//  if G.sipo.ipo)and(G.sipo.ipo.id.lib
//  then
//  exit;
//  if G.sipo.editipo=0
//  then
//  exit;
//  (* welke vertices doen mee *)
//  get_status_editipo(); 
//  if G.qual and LR_SHIFTKEY
//  then
//  totbut:= 1; 
//  if G.vd=0
//  then
//  :=10000.0; 
//  else
//  :=MAX2(G.vd.clipend,10000.0); 
//  if totipo_vertsel<>nil 
//  then
//  begin 
//    ei:= G.sipo.editipo; 
//    while{for} 
//    a:= 0; 
//    {to} a<G.sipo.totipo
//    {;}
//    inc(a); 
//    inc(ei); 
//    do
//    begin 
//      
//      function ISPOIN{!!!3 unknown typedef}: if; 
//      begin
//        if (ei.flag and IPO_EDIT))or(G.sipo.showkey
//        then
//        begin 
//          if ei.icu.bezt<>nil 
//          then
//          begin 
//            bezt1:= ei.icu.bezt; 
//            b:= ei.icu.totvert; 
//            while b{--} dec(b); 
//            do
//            begin 
//              if BEZSELECTED(bezt1)
//              then
//              begin 
//                bezt:= bezt1; 
//                break; {<= !!!b possible in "switch" - then remove this line}
//              end;
//              inc(bezt1); 
//            end;
//          end;
//        end;
//      end
//      if bezt<>nil 
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    if bezt=0
//    then
//    exit;
//    if bezt.f2 and 1
//    then
//    begin 
//      VECCOPY(old,bezt.vec[1]); 
//      if totipo_vis=1)and(G.sipo.blocktype=ID_OB
//      then
//      begin 
//        if{!!!e unknown token}
//        scale10:= 1; if{!!!e unknown token}
//        scale10:= 1; 
//      end;
//      if scale10<>nil 
//      then
//      bezt.vec[1][1]:= bezt.vec[1][1] * (10.0); 
//      add_numbut(0,NUM or FLO,'LocX:',-1000,10000,bezt.vec[1],0); 
//      if totbut=2
//      then
//      add_numbut(1,NUM or FLO,'LocY:',-,,bezt.vec[1]+1,0); 
//      do_clever_numbuts('Active BezierPoint',totbut,REDRAW); 
//      if scale10<>nil 
//      then
//      bezt.vec[1][1]:= bezt.vec[1][1] div (10.0); 
//      VecSubf(delta,bezt.vec[1],old); 
//      VECCOPY(bezt.vec[1],old); 
//      ei:= G.sipo.editipo; (* apply *)
//      while{for} 
//      a:= 0; 
//      {to} a<G.sipo.totipo
//      {;}
//      inc(a); 
//      inc(ei); 
//      do
//      begin 
//        
//        function ISPOIN{!!!3 unknown typedef}: if; 
//        begin
//          if (ei.flag and IPO_EDIT))or(G.sipo.showkey
//          then
//          begin 
//            if ei.icu.bezt<>nil 
//            then
//            begin 
//              bezt:= ei.icu.bezt; 
//              b:= ei.icu.totvert; 
//              while b{--} dec(b); 
//              do
//              begin 
//                if bezt.f2 and 1
//                then
//                begin 
//                  bezt.vec[0][0]:= bezt.vec[0][0] + (delta[0]); 
//                  bezt.vec[1][0]:= bezt.vec[1][0] + (delta[0]); 
//                  bezt.vec[2][0]:= bezt.vec[2][0] + (delta[0]); 
//                  bezt.vec[0][1]:= bezt.vec[0][1] + (delta[1]); 
//                  bezt.vec[1][1]:= bezt.vec[1][1] + (delta[1]); 
//                  bezt.vec[2][1]:= bezt.vec[2][1] + (delta[1]); 
//                end;
//                inc(bezt); 
//              end;
//            end;
//          end;
//        end;
//      end;
//      ei:= G.sipo.editipo; 
//      while{for} 
//      a:= 0; 
//      {to} a<G.sipo.totipo
//      {;}
//      inc(a); 
//      inc(ei); 
//      do
//      begin 
//        
//        function ISPOIN{!!!3 unknown typedef}: if; 
//        begin
//          sort_time_ipocurve(ei.icu); 
//          testhandles_ipocurve(ei.icu); 
//        end;
//      end;
//    end;
//    else
//    if bezt.f1 and 1
//    then
//    begin 
//      add_numbut(0,NUM or FLO,'LocX:',-1000,10000,bezt.vec[0],0); 
//      if totbut=2
//      then
//      add_numbut(1,NUM or FLO,'LocY:',-,,bezt.vec[0]+1,0); 
//      do_clever_numbuts('Active HandlePoint',totbut,REDRAW); 
//    end;
//    else
//    if bezt.f3 and 1
//    then
//    begin 
//      add_numbut(0,NUM or FLO,'LocX:',-1000,10000,bezt.vec[0],0); 
//      if totbut=2
//      then
//      add_numbut(1,NUM or FLO,'LocY:',-,,bezt.vec[2]+1,0); 
//      do_clever_numbuts('Active HandlePoint',totbut,REDRAW); 
//    end;
//    editipo_changed(1); 
//  end;
//  else
//  begin 
//    if G.sipo.blocktype=ID_KE
//    then
//    begin 
//      key:= {!!!a type cast? =>} {pKey(}G.sipo.from; 
//      if key=0
//      then
//      exit;
//      kb:= key.block.first; 
//      while kb
//      do
//      begin 
//        if kb.flag and SELECT
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        kb:= kb.next; 
//      end;
//      if kb)and(G.sipo.rowbut and 1
//      then
//      begin 
//        add_numbut(0,NUM or FLO,'Pos:',-100,100,@kb.pos,0); 
//        do_clever_numbuts('Active Key',1,REDRAW); 
//        sort_keys(key); 
//      end;
//    end;
//  end;
//end;
//
//procedure filter_sampledata(data: pfloat;  sfra: integer;  efra: integer); 
//var
//da: pfloat; 
//a: integer; 
//begin
//  
//  
//  da:= data+1; 
//  while{for} 
//  a:= sfra+1; 
//  {to} a<efra
//  {;}
//  inc(a); 
//  inc(da); 
//  do
//  begin 
//    da[0]:= 0.25*da[-1]+0.5*da[0]+0.25*da[1]; 
//  end;
//end;
//
//procedure sampledata_to_ipocurve(data: pfloat;  sfra: integer;  efra: integer;  icu: pIpoCurve); 
//var
//bezt: pBezTriple; 
//da: pfloat; 
//a: integer; 
//tot: integer; 
//begin
//  
//  
//  
//  
//  filter_sampledata(data,sfra,efra); 
//  filter_sampledata(data,sfra,efra); 
//  icu.ipo:= IPO_LIN; 
//  if icu.bezt<>nil 
//  then
//  freeN(icu.bezt); 
//  icu.bezt:= 0; 
//  tot:= 1; 
//  da:= data+1; (* eerste punt *)
//  while{for} 
//  a:= sfra+1; 
//  {to} a<efra
//  {;}
//  inc(a); 
//  inc(da); 
//  do
//  begin 
//    if IS_EQ(da[0],da[1])=0)and(IS_EQ(da[1],da[2])=0
//    then
//    inc(tot); 
//  end;
//  icu.totvert:= tot; 
//  bezt:= icu.bezt:=callocN(tot*sizeof(BezTriple),'samplebezt'); 
//  bezt.vec[1][0]:= sfra; 
//  bezt.vec[1][1]:= data[0]; 
//  inc(bezt); 
//  da:= data+1; 
//  while{for} 
//  a:= sfra+1; 
//  {to} a<efra
//  {;}
//  inc(a); 
//  inc(da); 
//  do
//  begin 
//    if IS_EQ(da[0],da[1])=0)and(IS_EQ(da[1],da[2])=0
//    then
//    begin 
//      bezt.vec[1][0]:= a; 
//      bezt.vec[1][1]:= da[0]; 
//      inc(bezt); 
//    end;
//  end;
//end;
//
//procedure ipo_record; 
//var
//ei: pEditIpo; 
//ei1: pEditIpo;
// 
//ei2: pEditIpo;
// 
//sa: pScrArea; 
//oldarea: pScrArea; 
//ipo: pIpo; 
//poin: pinteger; 
//swaptime: double; 
//or1: float; 
//or2: float; 
//fac: float; 
//data1: pfloat; 
//data2: pfloat; 
//type: integer; 
//a: integer; 
//afbreek: integer;
// 
//firsttime: integer;
// 
//cfrao: integer; 
//cfra: integer; 
//sfra: integer; 
//efra: integer; 
//event: ushort; 
//anim: smallint; 
//val: smallint; 
//xn: smallint; 
//yn: smallint; 
//mvalo: array [0..Pred(2)] of smallint; 
//mval: array [0..Pred(2)] of smallint; 
//str: array [0..Pred(128)] of char; 
//begin(* 1 of 2 aktieve curves
//    * kopie maken (ESC) 
//    *
//    * nulpunt is de huidige stand (of 0)
//    * dx (dy identiek) is de hoogteverhouding
//    * CTRL start record
//    *)
//  {tottime: double; }{<= !!!5 external variable}
//  
//  ei1:=0; 
//  ei2:=0; 
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
//  afbreek:=0; 
//  firsttime:=1; 
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
//  if G.sipo.from=0
//  then
//  exit;
//  if SFRA>=EFRA
//  then
//  exit;
//  anim:= pupmenu('Record Mouse %t|Still %x1|Play anim %x2'); 
//  if anim<1
//  then
//  exit;
//  if anim<>2
//  then
//  anim:= 0; 
//  ipo:= get_ipo(G.sipo.from,1); (* 1= make *)
//  if G.sipo<>nil 
//  then
//  G.sipo.ipo:= ipo; 
//  ei:= G.sipo.editipo; (* find the curves... *)
//  for{while} a:=0 to Pred(G.sipo.totipo) { a++}
//  do
//  begin 
//    if ei.flag and IPO_VISIBLE
//    then
//    begin 
//      if ei1=0
//      then
//      ei1:= ei; 
//      else
//      if ei2=0
//      then
//      ei2:= ei; 
//      else
//      begin 
//        error('Max 2 visible curves'); 
//        exit;
//      end;
//    end;
//    inc(ei); 
//  end;
//  if ei1=0
//  then
//  begin 
//    error('Select 1 or 2 channels'); 
//    exit;
//  end;
//  (* curves gereedmaken, startwaardes *)
//  if ei1.icu=0
//  then
//  ei1.icu:= get_ipocurve(G.sipo.from,ei1.adrcode); 
//  if ei1.icu=0
//  then
//  exit;
//  poin:= get_ipo_poin(G.sipo.from,ei1.icu,@type); 
//  if poin<>nil 
//  then
//  ei1.icu.curval:= read_ipo_poin(poin,type); 
//  or1:= ei1.icu.curval; 
//  ei1.icu.flag:= ei1.icu.flag or (IPO_LOCK); 
//  if ei2<>nil 
//  then
//  begin 
//    if ei2.icu=0
//    then
//    ei2.icu:= get_ipocurve(G.sipo.from,ei2.adrcode); 
//    if ei2.icu=0
//    then
//    exit;
//    poin:= get_ipo_poin(G.sipo.from,ei2.icu,@type); 
//    if poin<>nil 
//    then
//    ei2.icu.curval:= read_ipo_poin(poin,type); 
//    or2:= ei2.icu.curval; 
//    ei2.icu.flag:= ei2.icu.flag or (IPO_LOCK); 
//  end;
//  fac:= G.v2d.cur.ymax-G.v2d.cur.ymin; 
//  fac:= fac div ({!!!a type cast? =>} {float(}curarea.winy); 
//  oldarea:= curarea; 
//  sa:= G.curscreen.areabase.first; (* welke area *)
//  while sa
//  do
//  begin 
//    if sa.win)and(sa.windraw
//    then
//    begin 
//      if G.sipo.blocktype=ID_MA)or(G.sipo.blocktype=ID_LA
//      then
//      begin 
//        if sa.spacetype=SPACE_BUTS
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      else
//      begin 
//        if sa.spacetype=SPACE_VIEW3D
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//    end;
//    sa:= sa.next; 
//  end;
//  if sa<>nil 
//  then
//  areawinset(sa.win); 
//  (* kandie? *)
//  while get_mbut()@L_MOUSE
//  do
//  usleep(2); 
//  data1:= callocN(sizeof(float)*(EFRA-SFRA+1),'data1'); 
//  data2:= callocN(sizeof(float)*(EFRA-SFRA+1),'data2'); 
//  getmouseco_areawin(mvalo); 
//  xn:= mvalo[0]; 
//  yn:= mvalo[1]; 
//  waitcursor(1); 
//  tottime:= 0.0; 
//  swaptime:= speed_to_swaptime(G.animspeed); 
//  cfrao:= CFRA; 
//  cfra:= efra:=SFRA; 
//  sfra:= EFRA; 
//  while afbreek=0
//  do
//  begin 
//    getmouseco_areawin(mval); 
//    if mval[0]<>mvalo[0])or(mval[1]<>mvalo[1])or(firsttime)or((G.qual and LR_CTRLKEY)
//    then
//    begin 
//      if anim<>nil 
//      then
//      CFRA:= cfra; 
//      else
//      firsttime:= 0; 
//      set_timecursor(cfra); 
//      (* ipo doen: eerst alles daarna de specifieke *)
//      if anim=2
//      then
//      begin 
//        do_all_ipos(); 
//        do_all_keys(); 
//      end;
//      ei1.icu.curval:= or1+fac*(mval[0]-xn); 
//      if ei2<>nil 
//      then
//      ei2.icu.curval:= or2+fac*(mval[1]-yn); 
//      do_ipo_nocalc(G.sipo.ipo); 
//      do_all_visible_ikas(); 
//      if G.qual and LR_CTRLKEY
//      then
//      begin 
//        sprintf(str,'Recording... %d\n',cfra); 
//        data1[cfra-SFRA]:= ei1.icu.curval; 
//        if ei2<>nil 
//        then
//        data2[cfra-SFRA]:= ei2.icu.curval; 
//        sfra:= MIN2(sfra,cfra); 
//        efra:= MAX2(efra,cfra); 
//      end;
//      else
//      sprintf(str,'Mouse Recording. Use CTRL to start. LeftMouse or Space to end'); 
//      do_ob_key(OBACT); 
//      headerprint(str); 
//      if sa<>nil 
//      then
//      sa.windraw(); 
//      tottime:= tottime - (swaptime); (* minimaal swaptime laten voorbijgaan *)
//      while update_time()
//      do
//      usleep(1); 
//      screen_swapbuffers(); 
//      tottime:= 0.0; 
//      mvalo[0]:= mval[0]; 
//      mvalo[1]:= mval[1]; 
//      if anim)or((G.qual and LR_CTRLKEY)
//      then
//      begin 
//        inc(cfra); 
//        if cfra>EFRA
//        then
//        cfra:= SFRA; 
//      end;
//    end;
//    while qtest()
//    do
//    begin 
//      event:= extern_qread(@val); 
//      if val<>nil 
//      then
//      begin 
//        case event of
//          LEFTMOUSE,
//          ESCKEY,
//          SPACEKEY,
//          RETKEY:
//          begin
//            afbreek:= 1; 
//          end;
//          
//        end;{case?}
//      end;
//      if afbreek<>nil 
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//  end;
//  if event<>ESCKEY
//  then
//  begin 
//    sampledata_to_ipocurve(data1+sfra-SFRA,sfra,efra,ei1.icu); 
//    if ei2<>nil 
//    then
//    sampledata_to_ipocurve(data2+sfra-SFRA,sfra,efra,ei2.icu); 
//    (* vervelend als dat aanstaat *)
//    if G.sipo.showkey<>nil 
//    then
//    begin 
//      G.sipo.showkey:= 0; 
//      free_ipokey(@G.sipo.ipokey); 
//    end;
//  end;
//  else
//  begin 
//    (* undo: startwaardes *)
//    poin:= get_ipo_poin(G.sipo.from,ei1.icu,@type); 
//    if poin<>nil 
//    then
//    write_ipo_poin(poin,type,or1); 
//    if ei1.icu.bezt=0
//    then
//    begin 
//      remlink(@(G.sipo.ipo.curve),ei1.icu); 
//      freeN(ei1.icu); 
//      ei1.icu:= 0; 
//    end;
//    if ei2<>nil 
//    then
//    begin 
//      poin:= get_ipo_poin(G.sipo.from,ei2.icu,@type); 
//      if poin<>nil 
//      then
//      write_ipo_poin(poin,type,or2); 
//      if ei2.icu.bezt=0
//      then
//      begin 
//        remlink(@(G.sipo.ipo.curve),ei2.icu); 
//        freeN(ei2.icu); 
//        ei2.icu:= 0; 
//      end;
//    end;
//  end;
//  if ei1.icu<>nil 
//  then
//  ei1.icu.flag:= ei1.icu.flag and ( not IPO_LOCK); 
//  if ei2)and(ei2.icu
//  then
//  ei2.icu.flag:= ei2.icu.flag and ( not IPO_LOCK); 
//  editipo_changed(0); 
//  do_ipo(G.sipo.ipo); 
//  waitcursor(0); 
//  allqueue(REDRAWVIEW3D,0); 
//  if sa<>nil 
//  then
//  addqueue(sa.headwin,REDRAW,1); 
//  addqueue(oldarea.headwin,REDRAW,1); 
//  (* headerprint *)
//  addqueue(oldarea.win,REDRAW,1); 
//  CFRA:= cfrao; 
//  do_global_buttons(B_NEWFRAME); 
//  (* vooropig? *)
//  freeN(data1); 
//  freeN(data2); 
//end;
//
end.
