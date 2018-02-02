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

(* drawscene.c  GRAPHICS
 *
 *  jan 95
 *
 *  ook EDIT funkties
 *
 *  alleen routines met GRAPHICS!
 *
 *)

unit drawscene;

interface

uses
blender, screen, blenglob, blendef, scene, space;
//{$include "blender.h"}
//{$include "file.h"}
//{$include "graphics.h"}

procedure set_scene(sce: pScene);

implementation

procedure set_scene(sce: pScene); (* zie ook scene.c: set_scene_bg() *)
var
sc: pbScreen; 
sa: pScrArea; 
vd: pView3D; 
base: pBase; 
begin
  G.scene:= sce;
  sc:= G.main^.screen.first;
  while sc <> nil do
  begin 
  //  if ((U.flag and SCENEGLOBAL) <> 0)or(sc=G.curscreen) then
  //  begin 
  //    if sce<>sc^.scene then
  //    begin 
  //      (* alle area's endlocalview *)
  //      sa:= sc^.areabase.first; 
  //      while sa <> nil do
  //      begin 
  //        endlocalview(sa);
  //        sa:= sa^.next;
  //      end;
  //      sc^.scene:= sce;
  //    end;
  //  end;
  //  sc:= sc^.id.next;
  //end;
  //
  //copy_view3d_lock(0);  (* space.c *)
  //
  //(* zijn er camera's in de views die niet in de scene zitten? *)
  //sc:= G.main.screen.first;
  //while sc <>nil  do
  //begin
  //  if ((U.flag and SCENEGLOBAL)<>0)or(sc=G.curscreen) then
  //  begin
  //    sa:= sc^.areabase.first;
  //    while sa<>nil      do
  //    begin
  //      vd:= sa.spacedata.first;
  //      while vd<>nil    do
  //      begin
  //        if vd.spacetype=SPACE_VIEW3D then
  //        begin
  //          if vd.camera<>0 then
  //          begin
  //            if object_in_scene(vd.camera,sce)=0 then
  //            begin
  //              vd.camera:= 0;
  //              if vd.persp>1 then
  //              vd.persp:= 1;
  //            end;
  //          end;
  //        end;
  //        vd:= vd.next;
  //      end;
  //      sa:= sa.next;
  //    end;
  //  end;
    sc:= sc^.id.next;
  end;

  set_scene_bg(G.scene);

  (* volledige redraw *)
  allqueue(REDRAWALL,0);
  allqueue(REDRAWDATASELECT,0); (* doet remake *)
end;



(* ********************************************************* *)

//bdata: array [0..Pred(129)] of char; 
//cdata: array [0..Pred(129)] of char; 
//int({*}cfunc[64]^); 
//(); 
//{hash: array [0..] of char; }{<= !!!5 external variable}
//ihash: array [0..Pred(512)] of char; 
//warnc: array [0..Pred(55)] of char = (147,70,30,200,118,32,126,68,13,198,249,28,134,116,122,199,53,178,167,64,43,168,212,59,29,239,46,204,69,55,125,180,9,255,133,106,81,205,101,141,111,176,155,246,242,163,86,38,197,95,51,196,31,159,20); 
//
//
//procedure hashdecode(len: integer;  strout: pchar;  strin: pchar;  key1: char;  key2: char;  key3: char;  key4: char); 
//var
//a: integer; 
//begin
//  
//  a:= len; 
//  while a{--} dec(a); 
//  do
//  begin 
//    if a and 1
// then
//    begin 
//      strout[a]:= (ihash[(ihash[strin[a]]-key3) and 255]) and 255; 
//      strout[a]:= (ihash[(ihash[strout[a]]-key1) and 255]) and 255; 
//    end;
//    else
//    begin 
//      strout[a]:= (ihash[(ihash[strin[a]]-key4) and 255]) and 255; 
//      strout[a]:= (ihash[(ihash[strout[a]]-key2) and 255]) and 255; 
//    end;
//    strout[a]:= strout[a]-key1+key2-key3+key4-hash[a]; 
//  end;
//end;
//
//
//
//function MGS(x: integer): integer; {<= !!!6 unknown macro}
//
//
//procedure mark3(str2: pchar); 
//begin
//  (* int val;
//   char *a, *b;
//   
//   a= (str2+20);
//   b= (str2+62);
//   
//   val = MGS(a+1) * MGS(b+4) - MGS(a+4) * MGS(b+2);
//  *)
//end;
//
//(* checksum *)
//
//function mark5(str2: pchar): integer; 
//var
//a: smallint; 
//test: smallint;
// 
//begin
//  
//  test:=0; 
//  for{while} a:=0 to Pred(126) { a++}
//  do
//  test:= test + (str2[a]); 
//  begin
//    result:= (test=MGS(str2+126)); 
//    exit;
//  end;
//end;
//
//
//function testc: integer; 
//begin
//  if ((ihash[cdata[2]]-cdata[27]) and 255)=106
// then
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
//function errorc: integer; 
//begin
//  if testc()
// then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  error(warnc); 
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//
//procedure init_fp; 
//var
//a: integer; 
//begin
//  
//  cfunc[0]:= testc; 
//  cfunc[1]:= errorc; 
//  for{while} a:=2 to Pred(64) { a++}
//  do
//  begin 
//    cfunc[a]:= errorc; 
//  end;
//end;
//
//
//procedure decode_256(strout: pchar;  strin: pchar); 
//var
//a: integer; 
//inval: integer; 
//begin
//  
//  
//  for{while} a:=0 to Pred(128) { a++}
//  do
//  begin 
//    strout[a]:= (strin[ihash[2*a]]-'a')+16*(strin[ihash[2*a+1]]-'a'); 
//  end;
//end;
//
//
//procedure remove_enters(name: pchar;  len: integer); 
//var
//a: integer; 
//b: integer;
// 
//begin
//  
//  b:=0; 
//  for{while} a:=0 to Pred(len) { a++}
//  do
//  begin 
//    if name[a]=10)or(name[a]=13
// then
//    ; 
//    else
//    begin 
//      name[b]:= name[a]; 
//      inc(b); 
//    end;
//  end;
//end;
//
//
//procedure read_key; 
//var
//a: integer; 
//b: integer; 
//file: integer; 
//home: pchar; 
//name: array [0..Pred(320)] of char; 
//temp: array [0..Pred(320)] of char; 
//begin
//  
//  procedure add_radio; 
//  
//  procedure draw_prop_circle; 
//  
//  procedure scriptbuts; 
//  
//  
//  
//  
//  
//  
//  init_fp(); 
//  bzero(bdata,128); 
//  bzero(cdata,128); 
//  home:= gethome(); 
//  if home<>0
// then
//  begin 
//    (* inverse hashtab *)
//    for{while} a:=0 to Pred(256) { a++}
//    do
//    begin 
//      for{while} b:=0 to Pred(256) { b++}
//      do
//      begin 
//        if a=hash[b]
// then
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      ihash[a]:= b; 
//    end;
//    for{while} a:=0 to Pred(256) {a++}
//    do
//    ihash[256+a]:= ihash[a]; 
//    make_file_string(name,home,'.BCkey'); 
//    if {not}0=fop_exists(name)
// then
//    begin 
//      if G.f@G_DEBUG
// then
//      printf('Unable to find key at: %s\n',name);
//      make_file_string(name,home,'BCkey'); 
//    end;
//    if {not}0=fop_exists(name)
// then
//    begin 
//      if G.f@G_DEBUG
// then
//      printf('Unable to find key at: %s\n',name);
//      make_file_string(name,home,'BCkey.txt'); 
//    end;
//    file:= open(name,O_BINARY or O_RDONLY); 
//    if file>0
// then
//    begin 
//      if G.f@G_DEBUG
// then
//      printf('Found key at: %s\n',name);
//      read(file,name,280); 
//      remove_enters(name,280); 
//      decode_256(temp,name); 
//      if mark5(temp)=0
// then
//      begin 
//        printf('invalid C Key\n');
//      end;
//      else
//      begin 
//        memcpy(cdata,temp,4); 
//        hashdecode(48,cdata+4,temp+4,cdata[0],cdata[1],cdata[2],cdata[3]); 
//        cdata[51]:= 0; 
//        hashdecode(48,cdata+4+48,temp+4+48,cdata[20]+cdata[24],cdata[21]+cdata[25],cdata[22]+cdata[26],cdata[23]+cdata[27]); 
//        (* make a printable string *)
//        memcpy(cdata+100,temp+100,28); 
//      end;
//      close(file); 
//      if TEST_C_KEY=0
// then
//      begin 
//        printf('invalid C Key\n');
//      end;
//      cfunc[37]:= (int(* )())add_radio; 
//      cfunc[13]:= (int(* )())add_radio; 
//      cfunc[23]:= (int(* )())write_vrml_fs; 
//      cfunc[18]:= (int(* )())write_dxf_fs; 
//      cfunc[15]:= (int(* )())draw_prop_circle; 
//      cfunc[11]:= (int(* )())scriptbuts; 
//    end;
//    else
//    begin 
//      if (G.f and G_DEBUG) <> 0
// then
//      printf('can'#39't open %s\n',name);
//      for{while} a:=0 to Pred(64) { a++}
//      do
//      cdata[a+52]:= 1; 
//    end;
//    hashdecode(16,versionstr,versionstr,12,96,86,5); 
//    hashdecode(55,warnc,warnc,12,96,86,5); 
//  end;
//end;
//
//
//{MGS(x) (((uchar * )(x))[0] << 8 | ((uchar * )(x))[1])}
//function MGS(x: integer): integer; 
//begin
//  result:= (({!!!a type cast? =>} {puchar(}(x))[0] shl 8 or ({!!!a type cast? =>} {puchar(}(x))[1])
//end;

end.
