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

(*  scene.c      MIXED MODEL
 *
 *  jan 95
 *
 *
 *)

unit scene;

interface

uses
  blender, rct;

function add_scene(name: pchar): pScene;
procedure set_scene_bg(sce: pScene);

(* niet scene zelf vrijgeven *)

implementation

uses
  cfuncs,
  util, library_, blenglob, blendef;

procedure free_scene(sce: pScene); 
var
base: pBase; 
begin
  base:= sce^.base.first; 
  while base <> nil  do
  begin 
    dec(base^._object^.id.us);
    base:= base^.next; 
  end;
  (* pas op: niet objects vrijgeven! *)

  freelistN(@sce^.base);
  //free_editing(sce^.ed);
  if sce^.radio<>nil then
  freeN(sce^.radio); 
  sce^.radio:= nil;

  //free_scriptlink(@sce^.scriptlink); 
end;

function add_scene(name: pchar): pScene;
var
sce: pScene;
begin
  sce:= alloc_libblock(@G.main^.scene,ID_SCE,name);
  sce^.lay:= 1;

  sce^.r.mode:= R_GAMMA;
  sce^.r.cfra:= 1;
  sce^.r.sfra:= 1;
  sce^.r.efra:= 250;
  sce^.r.xsch:= 320;
  sce^.r.ysch:= 256;
  sce^.r.xasp:= 1;
  sce^.r.yasp:= 1;
  sce^.r.xparts:= 1;
  sce^.r.yparts:= 1;
  sce^.r.size:= 100;
  sce^.r.planes:= 24;
  sce^.r.quality:= 90;
  sce^.r.framapto:= 100;
  sce^.r.images:= 100;
  sce^.r.framelen:= 1.0;
  sce^.r.frs_sec:= 25;

  strcpy(sce^.r.backbuf,'//backbuf');
  strcpy(sce^.r.pic,U.renderdir);
  strcpy(sce^.r.ftype,'//ftype');

  init_rctf(@sce^.r.safety,0.1,0.9,0.1,0.9);
  sce^.r.osa:= 8;

  sce^.grav:= 0.1;
  sce^.maxdrawsector:= 12;

  exit(sce);
end;

//function copy_scene(sce: pScene;  level: integer): pScene; 
//var
//scen: pScene; 
//id: pID; 
//base: pBase; 
//obase: pBase; 
//ob: pObject; 
//cu: pCurve; 
//me: pMesh; 
//mb: pMetaBall; 
//ma: pMaterial; 
//la: pLamp; 
//a: integer;
//begin

//  (* level 0: alle objects shared
//   * level 1: alle objectdata shared
//   * level 2: volledige kopie
//   *)

//  (* level 0 *)
//  scen:= copy_libblock(sce); 
//  duplicatelist(@(scen.base),@(sce^.base));

//  clear_id_newpoins();

//  id_us_plus(pID(scen.world));
//  id_us_plus(pID(scen.set));
//  scen.ed:= 0; 
//  scen.radio:= 0; 
//  obase:= sce^.base.first; 
//  base:= scen.base.first; 
//  while base
//  do
//  begin 
//    inc(base^._object^.id.us); 
//    if obase=sce^.basact then
//    scen.basact:= base; 
//    obase:= obase.next; 
//    base:= base.next; 
//  end;
//  if level=0 then
//  begin
//    result:= scen; 
//    exit;
//    (* level 1 *)
//  end;
//  G.scene:= scen; 
//  single_object_users(0); 
//  ID_NEW(G.scene.camera); 

//  (*  camera *)
//  (* level 2 *)
//  if level>=2 then
//  begin 
//    if scen.world<>0 then
//    begin 
//      dec(scen.world.id.us); 
//      scen.world:= copy_world(scen.world); 
//    end;
//    single_obdata_users(0); 
//    single_mat_users_expand(); 
//    single_tex_users_expand(); 
//  end;
//  clear_id_newpoins(); 

//  copy_scriptlink(@sce^.scriptlink); 

//  begin
//    result:= scen; 
//    exit;
//  end;
//end;

//function object_in_scene(ob: pObject;  sce: pScene): integer; 
//var
//base: pBase; 
//begin

//  base:= sce^.base.first; 
//  while base
//  do
//  begin 
//    if base^._object^=ob then
//      exit(1);
//    base:= base.next; 
//  end;
//    exit(0);
//end;

procedure sort_baselist(sce: pScene);
var
tempbase: ListBase;
noparentbase: ListBase;
notyetbase: ListBase;
base: pBase;
test: pBase;
par: pObject;
doit: integer;
domore: integer=0;
lastdomore: integer=1;
//ika: pIka;
begin
  (* alles in volgorde van parent en track *)

  (* volgorde gelijk houden als er niets veranderd is! *)
  (* hier waren problemen met campos array's: volgorde camera's is van belang *)
  while domore<>lastdomore  do
  begin

    lastdomore:= domore;
    domore:= 0;
    tempbase.first:= nil;
    tempbase.last:=nil;
    noparentbase.first:=nil;
    noparentbase.last:=nil;
    notyetbase.first:= nil;
    notyetbase.last:=nil;

    while base=sce^.base.first    do
    begin
      remlink(@sce^.base,base);

      par:= nil;
      if base^._object^._type=OB_IKA then
      begin
        //ika:=base^._object^.data;
        //par:= ika.parent;
      end;

      if (par<>nil)or(base^._object^.parent<>nil)or(base^._object^.track<>nil) then
      begin
        doit:= 0;
        if base^._object^.parent<>nil then
        inc(doit);
        if base^._object^.track<>nil then
        inc(doit);
        if par<>nil then
        inc(doit);
        test:= tempbase.first;
        while test <>nil        do
        begin
          if test^._object=base^._object^.parent then
          dec(doit);
          if test^._object=base^._object^.track then
          dec(doit);
          if test^._object=par then
          dec(doit);
          if doit=0 then
          break;
          test:= test^.next;
        end;
        if test<>nil then
        insertlink(@tempbase,test,base)
        else
        begin
          addhead(@tempbase,base);
          inc(domore);
        end;
      end
      else
      addtail(@noparentbase,base);
    end;
    sce^.base:= noparentbase;
    addlisttolist(@sce^.base,@tempbase);
    addlisttolist(@sce^.base,@notyetbase);
  end;
end;

procedure set_scene_bg(sce: pScene);
var
base: pBase;
ob: pObject;
begin
  G.scene:= sce;

  (* objecten deselecteren (voor dataselect) *)
  ob:= G.main^._object.first;
  while ob<> nil  do
  begin
    ob^.flag:= ob^.flag and ( not SELECT);
    ob := ob^.id.next;
  end;

  (* baselijst sorteren *)
  sort_baselist(sce);

  (* layers en flags uit bases naar objecten kopieeren *)
  base:= FIRSTBASE;
  while base<>nil  do
  begin
    base^._object^.lay:= base^.lay;
    base^._object^.flag:= base^.flag;
    base^._object^.ctime:= -1234567.0;   (* forceer ipo *)
    base:= base^.next;
  end;

  //do_all_ipos();  (* layers/materials *)
  //do_all_scripts(SCRIPT_FRAMECHANGED);
  //do_all_keys();
  //do_all_ikas();
end;

//procedure set_scene_name(name: pchar); 
//var
//sce: pScene; 
//str: array [0..Pred(128)] of char; 
//begin

//  sce:= G.main^.scene.first;
//  while sce
//  do
//  begin 
//    if lstrcmp(name,sce^.id.name+2)=0 then
//    begin 
//      set_scene_bg(sce); 
//      exit;
//    end;
//    sce:= sce^.id.next; 
//  end;
//  sprintf(str,'Can''t find scene: %s',name); 
//  error(str); 
//end;

//(* used by metaballs
// * doesnt return the original duplicated object, only dupli's
// *)
//var {was static}
//dupob: pObject; 
//fase: integer; 

//function next_object(val: integer;  {!!!3 unknown typedef}): integer; 
//var
//run_again: integer;
// (* init *)
//begin
//  {duplilist: ListBase; }{<= !!!5 external variable}

//  run_again:=1; (* init *)
//  if val=0 then
//  begin 
//    fase:= F_START; 
//    dupob:= 0; 
//  end;
//  else
//  begin 
//    (* run_again is set when a duplilist has been ended *)
//    while run_again
//    do
//    begin 
//      run_again:= 0; (* de eerste base *)
//      if fase=F_START then
//      begin 
//        {*}base^:=FIRSTBASE; 
//        if {*}base^ then
//        begin 
//          {*}ob^:=(*base).object; 
//          fase:= F_SCENE; 
//        end;
//        else
//        begin 
//          (* uitzondering: een lege scene *)
//          if G.scene.set)and(G.scene.set.base.first then
//          begin 
//            {*}base^:=G.scene.set.base.first; 
//            {*}ob^:=(*base).object; 
//            fase:= F_SET; 
//          end;
//        end;
//      end;
//      else
//      begin 
//        if {*}base^)and(fase<>F_DUPLI then
//        begin 
//          {*}base^:=(*base).next; 
//          if {*}base^ then
//          {*}ob^:=(*base).object; 
//          else
//          begin 
//            if fase=F_SCENE then
//            begin 
//              (* de scene is klaar, we gaan door met de set *)
//              if G.scene.set)and(G.scene.set.base.first then
//              begin 
//                {*}base^:=G.scene.set.base.first; 
//                {*}ob^:=(*base).object; 
//                fase:= F_SET; 
//              end;
//            end;
//          end;
//        end;
//      end;
//      if {*}base^=0 then
//      fase:= F_START; 
//      else
//      begin 
//        if fase<>F_DUPLI then
//        begin 
//          if (*base).object.transflag and OB_DUPLI then
//          begin 
//            make_duplilist(G.scene,(*base).object); 
//            dupob:= duplilist.first; 
//          end;
//        end;
//        (* dupli's afhandelen *)
//        if dupob<>0 then
//        begin 
//          {*}ob^:=dupob; 
//          fase:= F_DUPLI; 
//          dupob:= dupob.id.next; 
//        end;
//        else
//        if fase=F_DUPLI then
//        begin 
//          fase:= F_SCENE; 
//          free_duplilist(); 
//          run_again:= 1; 
//        end;
//      end;
//    end;
//  end;
//  begin
//    result:= fase; 
//    exit;
//  end;
//end;

end.
