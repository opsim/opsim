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

(* object.c    MIXED MODEL

 *
 * jan 95
 *
 *)

unit object_;

interface

procedure clear_workob;

implementation

uses
  cfuncs,
  blender, blendef;

var
//originmat: array [0..Pred(3),0..Pred(3)] of float; (* na where_is_object(), kan her en der gebruikt worden *)
workob: _Object;

procedure clear_workob; 
begin
  bzero(@workob,sizeof(_Object));
  workob.size[0]:=1.0;
  workob.size[1]:=1.0;
  workob.size[2]:=1.0;
end;

//procedure copy_baseflags; 
//var
//base: pBase; 
//begin

//  base:= FIRSTBASE; 
//  while base
//  do
//  begin 
//    base.object.flag:= base.flag; 
//    base:= base.next; 
//  end;
//end;

//procedure copy_objectflags; 
//var
//base: pBase; 
//begin

//  base:= FIRSTBASE; 
//  while base
//  do
//  begin 
//    base.flag:= base.object.flag; 
//    base:= base.next; 
//  end;
//end;

//(* niet object zelf vrijgeven *)

//procedure free_object(ob: pObject); 
//var
//vd: pView3D; 
//ma: pMaterial; 
//temp: pObject; 
//se: pSector; 
//po: pPortal; 
//sc: pbScreen; 
//sa: pScrArea; 
//sce: pScene; 
//id: pID; 
//a: integer; 
//b: integer; (* specifieke data loskoppelen *)
//begin

//  if ob.data<>0 then
//  begin 
//    id:= ob.data; 
//    dec(id.us); 
//    if id.us=0 then
//    begin 
//      if ob.type=OB_MESH then
//      unlink_mesh(ob.data); 
//      else
//      if ob.type=OB_SECTOR then
//      unlink_sector(ob.data); 
//      else
//      if ob.type=OB_LIFE then
//      unlink_life(ob.data); 
//      else
//      if ob.type=OB_CURVE then
//      unlink_curve(ob.data); 
//      else
//      if ob.type=OB_MBALL then
//      unlink_mball(ob.data); 
//    end;
//    ob.data:= 0; 
//  end;
//  for{while} a:=0 to Pred(ob.totcol) { a++}
//  do
//  begin 
//    if ob.mat[a]<>0 then
//    dec(ob.mat[a].id.us); 
//  end;
//  if ob.mat<>0 then
//  freeN(ob.mat); 
//  ob.mat:= 0; 
//  if ob.bb<>0 then
//  freeN(ob.bb); 
//  ob.bb:= 0; 
//  if ob.path<>0 then
//  free_path(ob.path); 
//  ob.path:= 0; 
//  if ob.ipo<>0 then
//  dec(ob.ipo.id.us); 
//  free_effects(@ob.effect); 
//  freelistN(@ob.network); 
//  freedisplist(@ob.disp); 
//  temp:= G.main^.object.first; (* alle objecten aflopen: parentflags *)
//  (* tevens portals *)
//  while temp
//  do
//  begin 
//    if temp<>ob then
//    begin 
//      if temp.parent=ob then
//      temp.parent:= 0; 
//      if temp.track=ob then
//      temp.track:= :=0; 
//      if temp.type=OB_SECTOR then
//      begin 
//        se:= temp.data; 
//        if ob.type=OB_SECTOR then
//        begin 
//          po:= se.portals; 
//          b:= se.totport; 
//          while b{--} dec(b); 
//          do
//          begin 
//            if po.sector)and(po.sector.ob=ob then
//            po.sector:= 0; 
//            inc(po); 
//          end;
//        end;
//      end;
//    end;
//    temp:= temp.id.next; 
//  end;
//  free_scriptlink(@ob.scriptlink); 
//  (* materialen *)
//  (* bevels *)
//  sce:= G.main^.scene.first; (* camera's *)
//  while sce
//  do
//  begin 
//    if sce.camera=ob then
//    sce.camera:= 0; 
//    sce:= sce.id.next; 
//  end;
//  sc:= G.main^.screen.first; (* keys *)
//  (* screens *)
//  while sc
//  do
//  begin 
//    sa:= sc.areabase.first; 
//    while sa
//    do
//    begin 
//      vd:= sa^.spacedata.first; 
//      while vd
//      do
//      begin 
//        if vd.spacetype=SPACE_VIEW3D then
//        begin 
//          if vd.camera=ob then
//          begin 
//            vd.camera:= 0; 
//            if vd.persp>1 then
//            vd.persp:= 1; 
//          end;
//        end;
//        vd:= vd.next; 
//      end;
//      sa:= sa^.next; 
//    end;
//    sc:= sc.id.next; 
//  end;
//  do_realtimelight(0,0,0); 
//end;

//procedure free_object_fromscene(ob: pObject); 
//var
//vd: pView3D; 
//base: pBase; 
//obt: pObject; 
//mat: pMaterial; 
//wrld: pWorld; 
//sc: pbScreen; 
//sa: pScrArea; 
//sce: pScene; 
//cu: pCurve; 
//ika: pIka; 
//def: pDeform; 
//id: pID; 
//tex: pTex; 
//a: integer; (* alle objecten aflopen: parents en bevels *)
//nl: pNetLink;

//next: pNetLink; 
//begin

//  obt:= G.main^.object.first;
//  while obt
//  do
//  begin 
//    if obt.id.lib=0 then
//    begin 
//      if obt.parent=ob then
//      begin 
//        obt.parent:= 0; 
//        if ob.type=OB_LATTICE then
//        freedisplist(@obt.disp); 
//      end;
//      if obt.track=ob then
//      obt.track:= 0; 
//      if obt.network.first<>0 then
//      begin 
//        nl:=obt.network.first; 

//        while nl
//        do
//        begin 
//          next:= nl.next; 
//          if nl.ob=ob then
//          begin 
//            remlink(@obt.network,nl); 
//            freeN(nl); 
//          end;
//          nl:= next; 
//        end;
//      end;

//      function ELEM{!!!3 unknown typedef}: if; 
//      begin
//        cu:= obt.data; 
//        if cu.bevobj=ob then
//        cu.bevobj:= 0; 
//        if cu.textoncurve=ob then
//        cu.textoncurve:= 0; 
//      end
//      if obt.type=OB_IKA then
//      begin 
//        ika:= obt.data; 
//        if ika.parent=ob then
//        ika.parent:= 0; 
//        a:= ika.totdef; 
//        def:= ika.def; 
//        while a{--} dec(a); 
//        do
//        begin 
//          if def.ob=ob then
//          begin 
//            ika.totdef:= 0; 
//            freeN(ika.def); 
//            ika.def:= 0; 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//          inc(def); 
//        end;
//      end;
//    end;
//    obt:= obt.id.next; 
//  end;
//  mat:= G.main^.mat.first; (* materialen *)
//  while mat
//  do
//  begin 
//    for{while} a:=0 to Pred(8) { a++}
//    do
//    begin 
//      if mat.mtex[a])and(ob=mat.mtex[a].object then
//      begin 
//        (* eigenlijk testen op lib *)
//        mat.mtex[a].object:= 0; 
//      end;
//    end;
//    mat:= mat.id.next; 

//  end;
//  tex:= G.main^.tex.first; (* textures *)
//  while tex
//  do
//  begin 
//    if tex.env<>0 then
//    begin 
//      if tex.env.object=ob then
//      tex.env.object:= 0; 
//    end;
//    tex:= tex.id.next; 
//  end;
//  (* mballs *)
//  if ob.type=OB_MBALL then
//  begin 
//    obt:= find_basis_mball(ob); 
//    if obt<>0 then
//    freedisplist(@obt.disp); 
//  end;
//  wrld:= G.main^.world.first; (* worlds *)
//  while wrld
//  do
//  begin 
//    if wrld.id.lib=0 then
//    begin 
//      for{while} a:=0 to Pred(6) { a++}
//      do
//      begin 
//        if wrld.mtex[a])and(ob=wrld.mtex[a].object then
//        wrld.mtex[a].object:= :=0; 
//      end;
//    end;
//    wrld:= wrld.id.next; 
//  end;
//  sce:= G.main^.scene.first; (* scene's *)
//  while sce
//  do
//  begin 
//    if sce.id.lib=0 then
//    begin 
//      if sce.camera=ob then
//      sce.camera:= 0; 
//    end;
//    sce:= sce.id.next; 
//  end;
//  sc:= G.main^.screen.first; (* keys *)
//  (* screens *)
//  while sc
//  do
//  begin 
//    if sc.scene=G.scene then
//    begin 
//      sa:= sc.areabase.first; 
//      while sa
//      do
//      begin 
//        vd:= sa^.spacedata.first; 
//        while vd
//        do
//        begin 
//          if vd.spacetype=SPACE_VIEW3D then
//          begin 
//            if vd.camera=ob then
//            begin 
//              vd.camera:= 0; 
//              if vd.persp>1 then
//              vd.persp:= 1; 
//            end;
//            if vd.localvd)and(vd.localvd.camera=ob then
//            begin 
//              vd.localvd.camera:= 0; 
//              if vd.localvd.persp>1 then
//              vd.localvd.persp:= 1; 
//            end;
//          end;
//          vd:= vd.next; 
//        end;
//        sa:= sa^.next; 
//      end;
//    end;
//    sc:= sc.id.next; 
//  end;
//end;

//function exist_object(obtest: pObject): integer; 
//var
//ob: pObject; 
//begin

//  ob:= G.main^.object.first;
//  while ob
//  do
//  begin 
//    if ob=obtest then
//      exit(1);
//    ob:= ob.id.next; 
//  end;
//    exit(0);
//end;

//procedure add_camera; 
//var
//cam: pCamera; 
//begin

//  cam:= alloc_libblock(@G.main^.camera,ID_CA,'Camera');
//  cam.lens:= 35.0; 
//  cam.clipsta:= 0.1; 
//  cam.clipend:= 100.0; 
//  cam.drawsize:= 0.5; 
//  cam.netsta:= 0.5; 
//  cam.netend:= 10.0; 
//  cam.hold:= 50; 
//  begin
//    result:= cam; 
//    exit;
//  end;
//end;

//function copy_camera(cam: pCamera): pCamera; 
//var
//camn: pCamera; 
//begin

//  camn:= copy_libblock(cam); 
//  id_us_plus({!!!a type cast? =>} {pID(}camn.ipo); 
//  copy_scriptlink( and camn.scriptlink); 

//  begin
//    result:= camn; 
//    exit;
//  end;
//end;

//procedure make_local_camera(cam: pCamera); 
//var
//ob: pObject; 
//camn: pCamera; 
//local: integer;

//lib: integer;
// (* - zijn er alleen lib users: niet doen
//  * - zijn er alleen locale users: flag zetten
//  * - mixed: copy
//  *)
//begin

//  local:=0; 
//  lib:=0; (* - zijn er alleen lib users: niet doen
//    * - zijn er alleen locale users: flag zetten
//    * - mixed: copy
//    *)
//  if cam.id.lib=0 then
//  exit;
//  if cam.id.us=1 then
//  begin 
//    cam.id.lib:= 0; 
//    cam.id.flag:= LIB_LOCAL; 
//    new_id(0,{!!!a type cast? =>} {pID(}cam,0); 
//    exit;
//  end;
//  ob:= G.main^.object.first;
//  while ob
//  do
//  begin 
//    if ob.data=cam then
//    begin 
//      if ob.id.lib<>0 then
//      lib:= 1; 
//      else
//      local:= 1; 
//    end;
//    ob:= ob.id.next; 
//  end;
//  if local)and(lib=0 then
//  begin 
//    cam.id.lib:= 0; 
//    cam.id.flag:= LIB_LOCAL; 
//    new_id(0,{!!!a type cast? =>} {pID(}cam,0); 
//  end;
//  else
//  if local)and(lib then
//  begin 
//    camn:= copy_camera(cam); 
//    camn.id.us:= 0; 
//    ob:= G.main^.object.first;
//    while ob
//    do
//    begin 
//      if ob.data=cam then
//      begin 
//        if ob.id.lib=0 then
//        begin 
//          ob.data:= camn; 
//          inc(camn.id.us); 
//          dec(cam.id.us); 
//        end;
//      end;
//      ob:= ob.id.next; 
//    end;
//  end;
//end;

//procedure add_lamp; 
//var
//la: pLamp; 
//begin

//  la:= alloc_libblock(@G.main^.lamp,ID_LA,'Lamp');
//  la.r:= la.g:=la.b:=1.0; 
//  la.haint:= la.energy:=1.0; 
//  la.dist:= 20.0*G.vd.grid; 
//  la.spotsize:= 45.0; 
//  la.spotblend:= 0.15; 
//  la.att2:= 1.0; 
//  la.mode:= LA_SHAD; 
//  la.bufsize:= 512; 
//  la.clipsta:= 0.5; 
//  la.clipend:= 40.0; 
//  la.shadspotsize:= 45.0; 
//  la.samp:= 3; 
//  la.bias:= 1.0; 
//  la.soft:= 3.0; 
//  begin
//    result:= la; 
//    exit;
//  end;
//end;

//function copy_lamp(la: pLamp): pLamp; 
//var
//lan: pLamp; 
//a: integer; 
//begin

//  lan:= copy_libblock(la); 
//  for{while} a:=0 to Pred(8) { a++}
//  do
//  begin 
//    if lan.mtex[a]<>0 then
//    begin 
//      lan.mtex[a]:= mallocN(sizeof(MTex),'copylamptex'); 
//      memcpy(lan.mtex[a],la.mtex[a],sizeof(MTex)); 
//      id_us_plus({!!!a type cast? =>} {pID(}lan.mtex[a].tex); 
//    end;
//  end;
//  id_us_plus({!!!a type cast? =>} {pID(}lan.ipo); 
//  copy_scriptlink(@la.scriptlink); 

//  begin
//    result:= lan; 
//    exit;
//  end;
//end;

//procedure make_local_lamp(la: pLamp); 
//var
//ob: pObject; 
//lan: pLamp; 
//local: integer;

//lib: integer;
// (* - zijn er alleen lib users: niet doen
//  * - zijn er alleen locale users: flag zetten
//  * - mixed: copy
//  *)
//begin

//  local:=0; 
//  lib:=0; (* - zijn er alleen lib users: niet doen
//    * - zijn er alleen locale users: flag zetten
//    * - mixed: copy
//    *)
//  if la.id.lib=0 then
//  exit;
//  if la.id.us=1 then
//  begin 
//    la.id.lib:= 0; 
//    la.id.flag:= LIB_LOCAL; 
//    new_id(0,{!!!a type cast? =>} {pID(}la,0); 
//    exit;
//  end;
//  ob:= G.main^.object.first;
//  while ob
//  do
//  begin 
//    if ob.data=la then
//    begin 
//      if ob.id.lib<>0 then
//      lib:= 1; 
//      else
//      local:= 1; 
//    end;
//    ob:= ob.id.next; 
//  end;
//  if local)and(lib=0 then
//  begin 
//    la.id.lib:= 0; 
//    la.id.flag:= LIB_LOCAL; 
//    new_id(0,{!!!a type cast? =>} {pID(}la,0); 
//  end;
//  else
//  if local)and(lib then
//  begin 
//    lan:= copy_lamp(la); 
//    lan.id.us:= 0; 
//    ob:= G.main^.object.first;
//    while ob
//    do
//    begin 
//      if ob.data=la then
//      begin 
//        if ob.id.lib=0 then
//        begin 
//          ob.data:= lan; 
//          inc(lan.id.us); 
//          dec(la.id.us); 
//        end;
//      end;
//      ob:= ob.id.next; 
//    end;
//  end;
//end;

//procedure free_camera(ca: pCamera); 
//begin
//  free_scriptlink( and ca.scriptlink); 
//end;

//procedure free_lamp(la: pLamp); 
//var
//mtex: pMTex; 
//a: integer; 
//(* scriptlinks *)
//begin

//  free_scriptlink(@la.scriptlink); 
//  for{while} a:=0 to Pred(8) { a++}
//  do
//  begin 
//    mtex:= la.mtex[a]; 
//    if mtex)and(mtex.tex then
//    dec(mtex.tex.id.us); 
//    if mtex<>0 then
//    freeN(mtex); 
//  end;
//  la.ipo:= 0; 
//end;

//procedure add_wave; 
//begin
//    exit(0);
//end;

//(* **************** VOORKEUR NAMEN ************** *)

//var
//n_mesh: pchar = 0; 
//n_curve: pchar = 0; 
//n_mball: pchar = 0; 
//n_surf: pchar = 0; 
//n_font: pchar = 0; 
//n_lamp: pchar = 0; 
//n_camera: pchar = 0; 

//procedure set_obact_names(ob: pObject); 
//begin
//  if ob=0 then
//  exit;

//  case ob.type of
//    OB_MESH:
//    begin
//      n_mesh:= ob.id.name+2; 
//    end;
//    OB_CURVE:
//    begin
//      n_curve:= ob.id.name+2; 
//    end;
//    OB_MBALL:
//    begin
//      n_mball:= ob.id.name+2; 
//    end;
//    OB_SURF:
//    begin
//      n_surf:= ob.id.name+2; 
//    end;
//    OB_FONT:
//    begin
//      n_font:= ob.id.name+2; 
//    end;
//    OB_LAMP:
//    begin
//      n_lamp:= ob.id.name+2; 
//    end;
//    OB_CAMERA:
//    begin
//      n_camera:= ob.id.name+2; 
//    end;

//  end;
//end;

//procedure clear_obact_names; 
//begin
//  n_mesh:= n_curve:=n_mball:=n_surf:=n_font:=n_lamp:=n_camera:=0; 
//end;

//(* *************************************************** *)

//(* algemene add: in G.scene, met layer uit area en default naam *)
//(* maakt alle minimaal nodige datablokken aan, zonder vertices etc. *)

//function add_object(type: integer): pObject; 
//var
//ob: pObject; 
//base: pBase; 
//curs: psingle;
//name: array [0..Pred(32)] of char; 
//begin

//  G.f:= G.f and ( not (G_VERTEXPAINT+G_FACESELECT)); 
//  setcursor_space(SPACE_VIEW3D,CURSOR_STD); 
//  ob:= OBACT; (* de defaultnaam werkt (voorlopig) niet bij mesh, curve en surf (door add_prim) *)
//  if ob=0 then
//  clear_obact_names(); 
//  (* veiligheid *)
//  case type of
//    OB_MESH:
//    begin
//      if n_mesh<>0 then
//      strcpy(name,n_mesh); 
//      else
//      strcpy(name,'Mesh'); 
//    end;
//    OB_CURVE:
//    begin
//      if n_curve<>0 then
//      strcpy(name,n_curve); 
//      else
//      strcpy(name,'Curve'); 
//    end;
//    OB_SURF:
//    begin
//      if n_surf<>0 then
//      strcpy(name,n_surf); 
//      else
//      strcpy(name,'Surf'); 
//    end;
//    OB_FONT:
//    begin
//      if n_font<>0 then
//      strcpy(name,n_font); 
//      else
//      strcpy(name,'Text'); 
//    end;
//    OB_MBALL:
//    begin
//      if n_mball<>0 then
//      strcpy(name,n_mball); 
//      else
//      strcpy(name,'Mball'); 
//    end;
//    OB_CAMERA:
//    begin
//      if n_camera<>0 then
//      strcpy(name,n_camera); 
//      else
//      strcpy(name,'Camera'); 
//    end;
//    OB_LAMP:
//    begin
//      if n_lamp<>0 then
//      strcpy(name,n_lamp); 
//      else
//      strcpy(name,'Lamp'); 
//    end;
//    OB_IKA:
//    begin
//      strcpy(name,'Ika'); 
//    end;
//    OB_LATTICE:
//    begin
//      strcpy(name,'Lattice'); 
//    end;
//    OB_SECTOR:
//    begin
//      strcpy(name,'Sector'); 
//    end;
//    OB_LIFE:
//    begin
//      strcpy(name,'Life'); 
//    end;
//    OB_WAVE:
//    begin
//      strcpy(name,'Wave'); 
//    end;
//    else
//    begin
//      strcpy(name,'Empty'); 
//    end;
//  end;
//  ob:= alloc_libblock(@G.main^.object,ID_OB,name);
//  ob.type:= type; 
//  curs:= give_cursor(); 
//  VECCOPY(ob.loc,curs); 
//  (* default object vars *)

//  function ELEM{!!!3 unknown typedef}: if; 
//  else
//  begin 
//    G.vd.viewquat[0]:= -G.vd.viewquat[0]; 
//    QuatToEul(G.vd.viewquat,ob.rot); 
//    G.vd.viewquat[0]:= -G.vd.viewquat[0]; 
//  end;
//  ob.size[0]:= ob.size[1]:=ob.size[2]:=1.0; 
//  Mat4One(ob.parentinv); 
//  ob.dt:= OB_SHADED; 
//  if U.flag and MAT_ON_OB then
//  ob.colbits:= -1; 
//  if ob.type=OB_LIFE then
//  ob.dtx:= OB_AXIS; 
//  if type=OB_CAMERA)or(type=OB_LAMP then
//  begin 
//    ob.trackflag:= OB_NEGZ; 
//    ob.upflag:= OB_POSY; 
//  end;
//  else
//  begin 
//    ob.trackflag:= OB_POSY; 
//    ob.upflag:= OB_POSZ; 
//  end;
//  ob.ipoflag:= OB_OFFS_OB+OB_OFFS_PARENT; 
//  ob.dupon:= 1; 
//  ob.dupoff:= 0; 
//  ob.dupsta:= 1; 
//  ob.dupend:= 100; 
//  inc(G.totobj); (* specific data *)
//  case type of
//    OB_MESH:
//    begin
//      ob.data:= add_mesh(); 
//      inc(G.totmesh); 
//    end;
//    OB_CURVE:
//    begin
//      ob.data:= add_curve(OB_CURVE); 
//      inc(G.totcurve); 
//    end;
//    OB_SURF:
//    begin
//      ob.data:= add_curve(OB_SURF); 
//      inc(G.totcurve); 
//    end;
//    OB_FONT:
//    begin
//      ob.data:= add_curve(OB_FONT); 
//    end;
//    OB_MBALL:
//    begin
//      ob.data:= add_mball(); 
//    end;
//    OB_CAMERA:
//    begin
//      ob.data:= add_camera(); 
//    end;
//    OB_LAMP:
//    begin
//      ob.data:= add_lamp(); 
//      inc(G.totlamp); 
//    end;
//    OB_IKA:
//    begin
//      ob.data:= add_ika(); 
//      ob.dt:= OB_WIRE; 
//    end;
//    OB_LATTICE:
//    begin
//      ob.data:= add_lattice(); 
//      ob.dt:= OB_WIRE; 
//    end;
//    OB_SECTOR:
//    begin
//      ob.data:= add_sector(ob); 
//    end;
//    OB_LIFE:
//    begin
//      ob.data:= add_life(ob); 
//    end;
//    OB_WAVE:
//    begin
//      ob.data:= add_wave(); 
//    end;

//  end;
//  (* aan de scene hangen *)
//  base:= callocN(sizeof(Base),'add_base'); 
//  addhead(@G.scene.base,base); 
//  if G.vd.localview<>0 then
//  base.lay:= ob.lay:=G.vd.layact+G.vd.lay; 
//  else
//  base.lay:= ob.lay:=G.vd.layact; 
//  base.object:= ob; 
//  redraw_test_buttons(base); 
//  allqueue(REDRAWIPO,0); 
//  allqueue(REDRAWHEADERS,0); 
//  deselect_all_area_oops(); 
//  set_select_flag_oops(); 
//  BASACT:= base; 
//  base:= FIRSTBASE; (* deselecteren *)
//  while base
//  do
//  begin 
//    base.flag:= base.flag and ( not SELECT); 
//    base.object.flag:= base.flag; 
//    base:= base.next; 
//  end;
//  BASACT.flag:= BASACT.flag or (SELECT); 
//  ob.flag:= ob.flag or (SELECT); 
//  allqueue(REDRAWINFO,1); 
//  (* 1, want header->win==0! *)

//  begin
//    result:= ob; 
//    exit;
//  end;
//end;

//function copy_object(ob: pObject): pObject; 
//var
//obn: pObject; 
//a: integer; 
//begin

//  obn:= copy_libblock(ob); 
//  if ob.totcol<>0 then
//  begin 
//    obn.mat:= dupallocN(ob.mat); 
//  end;
//  if ob.bb<>0 then
//  obn.bb:= dupallocN(ob.bb); 
//  obn.path:= 0; 
//  obn.effect.first:= obn.effect.last:=0; 
//  copy_effects(@obn.effect,@ob.effect); 
//  obn.network.first:= obn.network.last:=0; 
//  copy_scriptlink(@ob.scriptlink); 
//  id_us_plus({!!!a type cast? =>} {pID(}obn.data); 
//  (* usernummers ophogen *)
//  id_us_plus({!!!a type cast? =>} {pID(}obn.ipo); 
//  for{while} a:=0 to Pred(obn.totcol) { a++}
//  do
//  id_us_plus({!!!a type cast? =>} {pID(}obn.mat[a]); 
//  obn.disp.first:= obn.disp.last:=0; 
//  begin
//    result:= obn; 
//    exit;
//  end;
//end;

//procedure expand_local_object(ob: pObject); 
//var
//a: integer; 
//begin

//  id_lib_extern({!!!a type cast? =>} {pID(}ob.ipo); 
//  id_lib_extern({!!!a type cast? =>} {pID(}ob.data); 
//  for{while} a:=0 to Pred(ob.totcol) { a++}
//  do
//  begin 
//    id_lib_extern({!!!a type cast? =>} {pID(}ob.mat[a]); 
//  end;
//end;

//procedure make_local_object(ob: pObject); 
//var
//obn: pObject; 
//sce: pScene; 
//base: pBase; 
//local: integer;

//lib: integer;
// (* - zijn er alleen lib users: niet doen
//  * - zijn er alleen locale users: flag zetten
//  * - mixed: copy
//  *)
//begin

//  local:=0; 
//  lib:=0; (* - zijn er alleen lib users: niet doen
//    * - zijn er alleen locale users: flag zetten
//    * - mixed: copy
//    *)
//  if ob.id.lib=0 then
//  exit;
//  if ob.id.us=1 then
//  begin 
//    ob.id.lib:= 0; 
//    ob.id.flag:= LIB_LOCAL; 
//    new_id(0,{!!!a type cast? =>} {pID(}ob,0); 
//  end;

//  else
//  begin 
//    sce:= G.main^.scene.first;
//    while sce
//    do
//    begin 
//      base:= sce.base.first; 
//      while base
//      do
//      begin 
//        if base.object=ob then
//        begin 
//          if sce.id.lib<>0 then
//          inc(lib); 
//          else
//          inc(local); 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        base:= base.next; 
//      end;
//      sce:= sce.id.next; 
//    end;
//    if local)and(lib=0 then
//    begin 
//      ob.id.lib:= 0; 
//      ob.id.flag:= LIB_LOCAL; 
//      new_id(0,{!!!a type cast? =>} {pID(}ob,0); 
//    end;
//    else
//    if local)and(lib then
//    begin 
//      obn:= copy_object(ob); 
//      obn.id.us:= 0; 
//      sce:= G.main^.scene.first;
//      while sce
//      do
//      begin 
//        if sce.id.lib=0 then
//        begin 
//          base:= sce.base.first; 
//          while base
//          do
//          begin 
//            if base.object=ob then
//            begin 
//              base.object:= obn; 
//              inc(obn.id.us); 
//              dec(ob.id.us); 
//            end;
//            base:= base.next; 
//          end;
//        end;
//        sce:= sce.id.next; 
//      end;
//    end;
//  end;
//  expand_local_object(ob); 
//end;

//(* *************** CALC ****************** *)

//(* er zit ook een tijdberekening in de drawobject() *)

//bluroffs: float = 0.0; 
//no_speed_curve: integer = 0; 

//procedure set_mblur_offs(blur: integer); 
//begin
//  bluroffs:= R.r.blurfac*({!!!a type cast? =>} {float(}blur); 
//  bluroffs:= bluroffs div ({!!!a type cast? =>} {float(}R.r.osa); 
//end;

//procedure disable_speed_curve(val: integer); 
//begin
//  no_speed_curve:= val; 
//end;

//function bsystem_time(ob: pObject;  par: pObject;  cfra: float;  ofs: float): float; 
//var
//sf: float; 
//f: float; 
//begin(* geeft float terug ( zie ook frame_to_float in ipo.c) *)

//  if no_speed_curve=0 then
//  if ob)and(ob.ipo then
//  cfra:= calc_ipo_time(ob.ipo,cfra); (* tweede field *)
//  if R.flag and R_SEC_FIELD then
//  begin 
//    if R.r.mode and R_FIELDSTILL then
//    ; 
//    else
//    cfra:= cfra + (.5); 
//  end;
//  cfra:= cfra + (bluroffs); 
//  cfra:= cfra * (G.scene.r.framelen); 
//  (* motion blur *)

//  (* global time *)
//  (* ofset frames *)
//  if ob)and((ob.ipoflag and OB_OFFS_PARENT) then
//  begin 
//    if (ob.partype and PARSLOW)=0 then
//    cfra:= cfra - (ob.sf); 
//  end;
//  cfra:= cfra - (ofs); 
//  begin
//    result:= cfra; 
//    exit;
//  end;
//end;

//(* no parent *)

//procedure object_to_mat3(ob: pObject;  mat: array [0..,0..Pred(3)] of float); 
//var
//smat: array [0..Pred(3),0..Pred(3)] of float; 
//vec: array [0..Pred(3)] of float; 
//rmat: array [0..Pred(3),0..Pred(3)] of float; 
//begin

//  if ob.ipo<>0 then
//  begin 
//    vec[0]:= ob.rot[0]+ob.drot[0]; 
//    vec[1]:= ob.rot[1]+ob.drot[1]; 
//    vec[2]:= ob.rot[2]+ob.drot[2]; 
//    EulToMat3(vec,rmat); 
//    vec[0]:= ob.size[0]+ob.dsize[0]; 
//    vec[1]:= ob.size[1]+ob.dsize[1]; 
//    vec[2]:= ob.size[2]+ob.dsize[2]; 
//    SizeToMat3(vec,smat); 

//  end;
//  else
//  begin 
//    EulToMat3(ob.rot,rmat); 
//    SizeToMat3(ob.size,smat); 
//  end;
//  Mat3MulMat3(mat,rmat,smat); 
//end;

//procedure object_to_mat4(ob: pObject;  mat: array [0..,0..Pred(4)] of float); 
//var
//tmat: array [0..Pred(3),0..Pred(3)] of float; 
//begin

//  object_to_mat3(ob,tmat); 
//  Mat4CpyMat3(mat,tmat); 
//  VECCOPY(mat[3],ob.loc); 
//  if ob.ipo<>0 then
//  begin 
//    mat[3][0]:= mat[3][0] + (ob.dloc[0]); 
//    mat[3][1]:= mat[3][1] + (ob.dloc[1]); 
//    mat[3][2]:= mat[3][2] + (ob.dloc[2]); 
//  end;
//end;

//enable_cu_speed: integer = 1; 

//procedure ob_parcurve(ob: pObject;  par: pObject;  mat: array [0..,0..Pred(4)] of float); 
//var
//cu: pCurve; 
//q: array [0..Pred(4)] of float; 
//vec: array [0..Pred(4)] of float; 
//dir: array [0..Pred(3)] of float; 
//quat: psingle;
//x1: float; 
//ctime: float; 
//begin

//  Mat4One(mat); 
//  cu:= par.data; 
//  if cu.path=0)or(cu.path.data=0 then
//  calc_curvepath(par); 
//  if cu.path=0 then
//  exit;
//  (* uitzondering afvangen: curve paden die als duplicator worden gebruikt *)
//  if enable_cu_speed<>0 then
//  begin 
//    ctime:= bsystem_time(ob,par,{!!!a type cast? =>} {float(}CFRA,0.0); 
//    if calc_ipo_spec(cu.ipo,CU_SPEED, and ctime)=0 then
//    begin 
//      ctime:= ctime div (cu.pathlen); 
//      CLAMP(ctime,0.0,1.0); 
//    end;
//  end;
//  else
//  begin 
//    ctime:= CFRA-ob.sf; 
//    ctime:= ctime div (cu.pathlen); 
//    CLAMP(ctime,0.0,1.0); 
//  end;

//  if where_on_path(par,ctime,vec,dir) then
//  begin 

//    if cu.flag and CU_FOLLOW then
//    begin 
//      quat:= vectoquat(dir,ob.trackflag,ob.upflag); 
//      Normalise(dir); 

//      q[0]:= fcos(0.5*vec[3]); 
//      x1:= fsin(0.5*vec[3]); 
//      q[1]:= -x1*dir[0]; 
//      q[2]:= -x1*dir[1]; 
//      q[3]:= -x1*dir[2]; 
//      QuatMul(quat,q,quat); 
//      QuatToMat4(quat,mat); 
//    end;
//    VECCOPY(mat[3],vec); 
//  end;
//end;

//procedure ob_parlimb(ob: pObject;  par: pObject;  mat: array [0..,0..Pred(4)] of float); 
//var
//ika: pIka; 
//li: pLimb; 
//cmat: array [0..Pred(3),0..Pred(3)] of float; 
//ang: float;

//cur: integer;
// (* in lokale ob space *)
//begin

//  ang:=0.0; 
//  cur:=0; (* in lokale ob space *)
//  Mat4One(mat); 
//  ika:= par.data; 
//  li:= ika.limbbase.first; 
//  while li
//  do
//  begin 
//    ang:= ang + (li.alpha); 
//    if cur=ob.par1)or(li.next=0 then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    inc(cur); 
//    li:= li.next; 
//  end;
//  mat[0][0]:= fcos(ang); 
//  mat[1][0]:= -fsin(ang); 
//  mat[0][1]:= fsin(ang); 
//  mat[1][1]:= fcos(ang); 
//  mat[3][0]:= li.eff[0]; 
//  mat[3][1]:= li.eff[1]; 
//end;

//procedure give_parvert(par: pObject;  nr: integer;  vec: psingle);
//var
//me: pMesh; 
//mvert: pMVert; 
//eve: pEditVert; 
//nu: pNurb; 
//cu: pCurve; 
//bp: pBPoint; 
//dl: pDispList; 
//bezt: pBezTriple; 
//fp: psingle;
//a: integer; 
//count: integer; 
//begin

//  {editNurb: ListBase; }{<= !!!5 external variable}

//  vec[0]:= vec[1]:=vec[2]:=0.0; 
//  if par.type=OB_MESH then
//  begin 
//    if par=G.obedit then
//    begin 
//      if nr>=G.totvert then
//      nr:= 0; 
//      count:= 0; 
//      eve:= G.edve.first; 
//      while eve
//      do
//      begin 
//        if count=nr then
//        begin 
//          memcpy(vec,eve.co,12); 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        eve:= eve.next; 
//        inc(count); 
//      end;
//    end;
//    else
//    begin 
//      me:= par.data; 
//      if nr>=me.totvert then
//      nr:= 0; 
//      dl:= find_displist(@par.disp,DL_VERTS); (* is er deform *)
//      if dl<>0 then
//      begin 
//        fp:= dl.verts+3*nr; 
//        VECCOPY(vec,fp); 
//      end;
//      else
//      begin 
//        mvert:= me.mvert+nr; 
//        VECCOPY(vec,mvert.co); 
//      end;
//    end;
//  end;
//  else

//  function ELEM{!!!3 unknown typedef}: if; 
//  begin
//    cu:= par.data; 
//    nu:= cu.nurb.first; 
//    if par=G.obedit then
//    nu:= editNurb.first; 
//    count:= 0; 
//    while nu
//    do
//    begin 
//      if (nu.type and 7)=CU_BEZIER then
//      begin 
//        bezt:= nu.bezt; 
//        a:= nu.pntsu; 
//        while a{--} dec(a); 
//        do
//        begin 
//          if count=nr then
//          begin 
//            VECCOPY(vec,bezt.vec[1]); 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//          inc(count); 
//          inc(bezt); 
//        end;
//      end;
//      else
//      begin 
//        bp:= nu.bp; 
//        a:= nu.pntsu*nu.pntsv; 
//        while a{--} dec(a); 
//        do
//        begin 
//          if count=nr then
//          begin 
//            memcpy(vec,bp.vec,12); 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//          inc(count); 
//          inc(bp); 
//        end;
//      end;
//      nu:= nu.next; 
//    end;
//  end;

//  else
//  if par.type=OB_IKA then
//  begin 
//    var
//    ika: pIka = par.data; 
//    li: pLimb = ika.limbbase.first; 
//    cur: integer = 1; 
//    if nr<>0 then
//    begin 
//      while li
//      do
//      begin 
//        if cur=nr)or(li.next=0 then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        inc(cur); 
//        li:= li.next; 
//      end;
//      vec[0]:= li.eff[0]; 
//      vec[1]:= li.eff[1]; 
//    end;
//  end;
//  else
//  exit;
//end;

//procedure ob_parvert3(ob: pObject;  par: pObject;  mat: array [0..,0..Pred(4)] of float); 
//var
//cmat: array [0..Pred(3),0..Pred(3)] of float; 
//v1: array [0..Pred(3)] of float; 
//v2: array [0..Pred(3)] of float; 
//v3: array [0..Pred(3)] of float; 
//q: array [0..Pred(4)] of float; 
//a: integer; 
//count: integer; 
//par1: integer; 
//par2: integer; 
//par3: integer; 
//(* in lokale ob space *)
//begin

//  Mat4One(mat); 

//  function ELEM3{!!!3 unknown typedef}: if; 
//  begin
//    give_parvert(par,ob.par1,v1); 
//    give_parvert(par,ob.par2,v2); 
//    give_parvert(par,ob.par3,v3); 
//    triatoquat(v1,v2,v3,q); 
//    QuatToMat3(q,cmat); 
//    Mat4CpyMat3(mat,cmat); 
//    if ob.type=OB_CURVE then
//    begin 
//      VECCOPY(mat[3],v1); 
//    end;
//    else
//    begin 
//      VecAddf(mat[3],v1,v2); 
//      VecAddf(mat[3],mat[3],v3); 
//      VecMulf(mat[3],0.3333333); 
//    end;
//  end
//end;

//var
//no_parent_ipo: integer = 0; 

//procedure set_no_parent_ipo(val: integer); 
//begin
//  no_parent_ipo:= val; 
//end;

//var
//timefac: float = 1.0; (* 50 Hz, dtime:2 *)

//procedure set_dtime(dtime: integer); 
//begin
//  timefac:= ({!!!a type cast? =>} {float(}dtime) div 2.0; 
//end;

//var {was static}
//during_script_flag: integer = 0; 

//procedure disable_where_script(on: smallint); 
//begin
//  during_script_flag:= on; 
//end;

//function during_script: integer; 
//begin
//  begin
//    result:= during_script_flag; 
//    exit;
//  end;
//end;

//procedure where_is_object_time(ob: pObject;  ctime: float); 
//var
//par: pObject; 
//fp1: psingle;
//fp2: psingle;
//tmat: array [0..Pred(4),0..Pred(4)] of float; 
//obmat: array [0..Pred(4),0..Pred(4)] of float; 
//totmat: array [0..Pred(4),0..Pred(4)] of float; 
//slowmat: array [0..Pred(4),0..Pred(4)] of float; 
//stime: float; 
//fac1: float; 
//fac2: float; 
//vec: array [0..Pred(3)] of float; 
//a: integer; 
//ok: integer; 
//pop: integer; (* nieuwe versie: correcte parent+vertexparent en track+parent *)
//(* deze berekent alleen de directe relatie met de parent en track *)
//(* hij is sneller, maar moet wel de timeoffs in de gaten houden *)
//ika: pIka;

//quat: psingle;
//begin

//  if ob=0 then
//  exit;

//  if ctime<>ob.ctime then
//  begin 
//    ob.ctime:= ctime; 
//    if ob.ipo<>0 then
//    begin 
//      stime:= bsystem_time(ob,0,ctime,0.0); 
//      calc_ipo(ob.ipo,stime); 

//      execute_ipo({!!!a type cast? =>} {pID(}ob,ob.ipo); 
//    end;
//  end;

//  if ob.type=OB_IKA then
//  begin 
//    ika:=ob.data; 
//    if ika.parent<>0 then
//    where_is_object_time(ika.parent,ctime); 
//  end;

//  if ob.parent<>0 then
//  begin 
//    par:= ob.parent; 
//    if ob.ipoflag and OB_OFFS_PARENT then
//    ctime:= ctime - (ob.sf); 
//    pop:= 0; 
//    if no_parent_ipo=0)and(ctime<>par.ctime then
//    begin 
//      (* alleen voor ipo systemen? *)
//      pushdata(par,sizeof(Object)); 
//      pop:= 1; 
//      where_is_object_time(par,ctime); 
//    end;
//    object_to_mat4(ob,obmat); 

//    if ob.partype and PARSLOW then
//    Mat4CpyMat4(slowmat,ob.obmat); 

//    case ob.partype and PARTYPE of
//      PAROBJECT:
//      begin
//        ok:= 0; 
//        if par.type=OB_CURVE then
//        begin 
//          if ({!!!a type cast? =>} {pCurve(}par.data).flag and CU_PATH then
//          begin 
//            ob_parcurve(ob,par,tmat); 
//            ok:= 1; 
//          end;
//        end;
//        if ok<>0 then
//        Mat4MulSerie(totmat,par.obmat,tmat,0); 
//        else
//        Mat4CpyMat4(totmat,par.obmat); 
//      end;
//      PARLIMB:
//      begin
//        ob_parlimb(ob,par,tmat); 
//        Mat4MulSerie(totmat,par.obmat,tmat,0); 
//      end;
//      PARVERT1:
//      begin
//        Mat4One(totmat); 
//        give_parvert(par,ob.par1,vec); 
//        VecMat4MulVecfl(totmat[3],par.obmat,vec); 
//      end;
//      PARVERT3:
//      begin
//        ob_parvert3(ob,par,tmat); 
//        Mat4MulSerie(totmat,par.obmat,tmat,0); 
//      end;
//      PARSKEL:
//      begin
//        Mat4One(totmat); 
//      end;

//    end;
//    (* totaal *)
//    Mat4MulSerie(tmat,totmat,ob.parentinv,0); 
//    Mat4MulSerie(ob.obmat,tmat,obmat,0); 
//    Mat3CpyMat4(originmat,tmat); 
//    (* dit is een extern bruikbare originmat *)
//    (* origin, voor hulplijntje *)
//    if (ob.partype and 15)=PARSKEL then
//    begin 
//      VECCOPY(ob.orig,par.obmat[3]); 
//    end;
//    else
//    begin 
//      VECCOPY(ob.orig,totmat[3]); 
//    end;
//    if pop<>0 then
//    begin 
//      poplast(par); 
//    end;
//    if ob.partype and PARSLOW then
//    begin 
//      (* framerate meetellen *)
//      fac1:= timefac div (1.0+fabs(ob.sf)); 
//      if fac1>=1.0 then
//      exit;
//      fac2:= 1.0-fac1; 
//      fp1:= ob.obmat[0]; 
//      fp2:= slowmat[0]; 
//      while{for} 
//      a:= 0; 
//      {to} a<16
//      {;}
//      inc(a); 
//      inc(fp1); 
//      inc(fp2); 
//      do
//      begin 
//        fp1[0]:= fac1*fp1[0]+fac2*fp2[0]; 
//      end;
//    end;
//  end;
//  else
//  begin 
//    object_to_mat4(ob,ob.obmat); 
//  end;
//  if ob.track<>0 then
//  begin 

//    if ctime<>ob.track.ctime then
//    where_is_object_time(ob.track,ctime); 
//    VecSubf(vec,ob.obmat[3],ob.track.obmat[3]); 
//    quat:= vectoquat(vec,ob.trackflag,ob.upflag); 
//    QuatToMat3(quat,totmat); 
//    if ob.parent)and((ob.transflag and OB_POWERTRACK) then
//    begin 
//      (* 'tijdelijk' : parent info wissen *)
//      object_to_mat4(ob,tmat); 
//      tmat[0][3]:= ob.obmat[0][3]; 
//      tmat[1][3]:= ob.obmat[1][3]; 
//      tmat[2][3]:= ob.obmat[2][3]; 
//      tmat[3][0]:= ob.obmat[3][0]; 
//      tmat[3][1]:= ob.obmat[3][1]; 
//      tmat[3][2]:= ob.obmat[3][2]; 
//      tmat[3][3]:= ob.obmat[3][3]; 
//    end;
//    else
//    Mat4CpyMat4(tmat,ob.obmat); 
//    Mat4MulMat34(ob.obmat,totmat,tmat); 
//  end;

//  if {not}0=during_script())and(ob.scriptlink.totscript then
//  begin 
//    do_pyscript(ob,SCRIPT_FRAMECHANGED); 
//  end;
//end;

//procedure where_is_object(ob: pObject); 
//begin(* deze zijn gememcopied *)
//  if ob.flag and OB_FROMDUPLI then
//  exit;
//  where_is_object_time(ob,F_CFRA); 
//end;

//procedure where_is_object_simul(ob: pObject); 
//var
//par: pObject; 
//ipo: pIpo; 
//fp1: psingle;
//fp2: psingle;
//tmat: array [0..Pred(4),0..Pred(4)] of float; 
//obmat: array [0..Pred(4),0..Pred(4)] of float; 
//totmat: array [0..Pred(4),0..Pred(4)] of float; 
//slowmat: array [0..Pred(4),0..Pred(4)] of float; 
//stime: float; 
//fac1: float; 
//fac2: float; 
//vec: array [0..Pred(3)] of float; 
//a: integer; 
//ok: integer; (* nieuwe versie: correcte parent+vertexparent en track+parent *)
//(* deze berekent alleen de directe relatie met de parent en track *)
//(* GEEN TIMEOFFS *)
//(* geen ipo! (ivm dloc en realtime-ipos) *)
//mat: array [0..Pred(3),0..Pred(3)] of float; 
//up: array [0..Pred(3)] of float; 
//begin

//  ipo:= ob.ipo; 
//  ob.ipo:= 0; 
//  if ob.parent<>0 then
//  begin 
//    par:= ob.parent; 
//    object_to_mat4(ob,obmat); 

//    if ob.partype and PARSLOW then
//    Mat4CpyMat4(slowmat,ob.obmat); 

//    case ob.partype and 15 of
//      PAROBJECT:
//      begin
//        ok:= 0; 
//        if par.type=OB_CURVE then
//        begin 
//          if ({!!!a type cast? =>} {pCurve(}par.data).flag and CU_PATH then
//          begin 
//            ob_parcurve(ob,par,tmat); 
//            ok:= 1; 
//          end;
//        end;
//        if ok<>0 then
//        Mat4MulSerie(totmat,par.obmat,tmat,0); 
//        else
//        Mat4CpyMat4(totmat,par.obmat); 
//      end;
//      PARLIMB:
//      begin
//        ob_parlimb(ob,par,tmat); 
//        Mat4MulSerie(totmat,par.obmat,tmat,0); 
//      end;
//      PARVERT1:
//      begin
//        Mat4One(totmat); 
//        VECCOPY(totmat[3],par.obmat[3]); 
//      end;
//      PARVERT3:
//      begin
//        ob_parvert3(ob,par,tmat); 
//        Mat4MulSerie(totmat,par.obmat,tmat,0); 
//      end;
//      PARSKEL:
//      begin
//        Mat4One(totmat); 
//      end;

//    end;
//    (* totaal *)
//    Mat4MulSerie(tmat,totmat,ob.parentinv,0); 
//    Mat4MulSerie(ob.obmat,tmat,obmat,0); 
//    if ob.partype and PARSLOW then
//    begin 
//      fac1:= 1.0 div (1.0+fabs(ob.sf)); 
//      fac2:= 1.0-fac1; 
//      fp1:= ob.obmat[0]; 
//      fp2:= slowmat[0]; 
//      while{for} 
//      a:= 0; 
//      {to} a<16
//      {;}
//      inc(a); 
//      inc(fp1); 
//      inc(fp2); 
//      do
//      begin 
//        fp1[0]:= fac1*fp1[0]+fac2*fp2[0]; 
//      end;
//    end;
//  end;
//  else
//  begin 
//    object_to_mat4(ob,ob.obmat); 
//  end;
//  if ob.track<>0 then
//  begin 

//    VecSubf(vec,ob.obmat[3],ob.track.obmat[3]); 
//    up[0]:= 0.0; 
//    up[1]:= 1.0; 
//    up[2]:= 0.0; 
//    VecUpMat3(vec,mat,1); 
//    VECCOPY(ob.obmat[0],mat[0]); 
//    VECCOPY(ob.obmat[1],mat[1]); 
//    VECCOPY(ob.obmat[2],mat[2]); 
//  end;

//  ob.ipo:= ipo; 
//  (*  LET OP!!! *)
//end;

//procedure what_does_parent1(par: pObject;  partype: integer;  par1: integer;  par2: integer;  par3: integer); 
//begin
//  clear_workob(); 

//  Mat4One(workob.parentinv); 
//  workob.parent:= par; 
//  if par<>0 then
//  workob.track:= par.track; 
//  workob.partype:= partype; 
//  workob.par1:= par1; 
//  workob.par2:= par2; 
//  workob.par3:= par3; 
//  where_is_object(@workob); 
//  (* LET OP: NIET ECHT NETJES *)

//end;

//procedure what_does_parent(ob: pObject); 
//begin
//  clear_workob(); 

//  Mat4One(workob.parentinv); 
//  workob.parent:= ob.parent; 
//  workob.track:= ob.track; 
//  workob.trackflag:= ob.trackflag; 
//  workob.upflag:= ob.upflag; 
//  workob.partype:= ob.partype; 
//  workob.par1:= ob.par1; 
//  workob.par2:= ob.par2; 
//  workob.par3:= ob.par3; 
//  where_is_object(@workob); 

//end;

//function unit_boundbox: pBoundBox; 
//var
//bb: pBoundBox; 
//begin

//  bb:= mallocN(sizeof(BoundBox),'bb'); 
//  bb.vec[0][0]:= bb.vec[1][0]:=bb.vec[2][0]:=bb.vec[3][0]:=-1.0; 
//  bb.vec[4][0]:= bb.vec[5][0]:=bb.vec[6][0]:=bb.vec[7][0]:=1.0; 
//  bb.vec[0][1]:= bb.vec[1][1]:=bb.vec[4][1]:=bb.vec[5][1]:=-1.0; 
//  bb.vec[2][1]:= bb.vec[3][1]:=bb.vec[6][1]:=bb.vec[7][1]:=1.0; 
//  bb.vec[0][2]:= bb.vec[3][2]:=bb.vec[4][2]:=bb.vec[7][2]:=-1.0; 
//  bb.vec[1][2]:= bb.vec[2][2]:=bb.vec[5][2]:=bb.vec[6][2]:=1.0; 

//  begin
//    result:= bb; 
//    exit;
//  end;
//end;

//procedure minmax_object(ob: pObject;  min: psingle;  max: psingle);
//var
//bb: BoundBox; 
//me: pMesh; 
//cu: pCurve; 
//vec: array [0..Pred(3)] of float; 
//a: integer; 
//begin

//  case ob.type of
//    OB_CURVE,
//    OB_FONT,
//    OB_SURF:
//    begin
//      cu:= ob.data; 
//      if cu.bb=0 then
//      tex_space_curve(cu); 
//      bb:= *(cu.bb); 
//      for{while} a:=0 to Pred(8) { a++}
//      do
//      begin 
//        Mat4MulVecfl(ob.obmat,bb.vec[a]); 
//        DO_MINMAX(bb.vec[a],min,max); 
//      end;
//    end;
//    OB_LIFE,

//    OB_MESH,
//    OB_SECTOR:
//    begin
//      me:= get_mesh(ob); 
//      if me<>0 then
//      begin 
//        if me.bb=0 then
//        tex_space_mesh(me); 
//        bb:= *(me.bb); 
//        for{while} a:=0 to Pred(8) { a++}
//        do
//        begin 
//          Mat4MulVecfl(ob.obmat,bb.vec[a]); 
//          DO_MINMAX(bb.vec[a],min,max); 
//        end;
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    else
//    (* hier geen break!!! *)
//    begin
//      DO_MINMAX(ob.obmat[3],min,max); 
//      VECCOPY(vec,ob.obmat[3]); 

//      VecAddf(vec,vec,ob.size); 
//      DO_MINMAX(vec,min,max); 
//      VECCOPY(vec,ob.obmat[3]); 

//      VecSubf(vec,vec,ob.size); 
//      DO_MINMAX(vec,min,max); 
//    end;

//  end;
//end;

end.
