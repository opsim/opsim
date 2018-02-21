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

(*  toets.c   mei 94     GRAPHICS
 * 
 *  Algemene toetsen, bijzondere in de space.c
 * 
 *)
 
unit toets;

interface

function save_image_filesel_str(str: pchar): integer;
function blenderqread(event: word;  val: smallint): integer;

implementation

uses
  cfuncs, blender, blendef, blenglob, screen, mydevice, filesel;

//var {was static}
//oldcamera: pObject;

//perspo: integer;

//procedure persptoetsen(event: word); 
//var
//base: pBase; 
//zoom: float; 
//phi: float; 
//si: float; 
//q1: array [0..Pred(4)] of float; 
//vec: array [0..Pred(3)] of float; 
//dx: float;

//dy: float;

//ok: integer;

//mval: array [0..Pred(2)] of smallint; 
//rt2: smallint; 
//rt3: smallint; 
//begin
//  oldcamera:=0; 

//  dx:=0; 
//  dy:=0; 
//  perspo:=1; 
//  ok:=0; 

//  if G.qual and LR_SHIFTKEY then
//  begin 
//    if event=PAD0 then
//    begin 
//      (* G.vd->persp= 3; *)
//    end;
//    else
//    if event=PAD7 then
//    begin 
//      G.vd.viewquat[0]:= 0.0; 
//      G.vd.viewquat[1]:= -1.0; 
//      G.vd.viewquat[2]:= 0.0; 
//      G.vd.viewquat[3]:= 0.0; 
//      G.vd.view:= 7; 
//    end;
//    else
//    if event=PAD1 then
//    begin 
//      G.vd.viewquat[0]:= 0.0; 
//      G.vd.viewquat[1]:= 0.0; 
//      G.vd.viewquat[2]:= -fcos(M_PI div 4.0); 
//      G.vd.viewquat[3]:= -fcos(M_PI div 4.0); 
//      G.vd.view:= 1; 
//    end;
//    else
//    if event=PAD3 then
//    begin 
//      G.vd.viewquat[0]:= 0.5; 
//      G.vd.viewquat[1]:= -0.5; 
//      G.vd.viewquat[2]:= 0.5; 
//      G.vd.viewquat[3]:= 0.5; 
//      G.vd.view:= 3; 
//    end;
//    else
//    if event=PADMINUS then
//    begin 
//      (* deze min en max staan ook in viewmove() *)
//      if G.vd.persp=2 then
//      begin 
//        if (G.scene.r.scemode and R_NETWORK) then
//        begin 
//          next_camera(-1); 
//        end;
//        else
//        begin 
//          G.vd.camzoom:= G.vd.camzoom - (10); 
//          if G.vd.camzoom<-30 then
//          G.vd.camzoom:= -30; 
//        end;
//      end;
//      else
//      if G.vd.dist<10.0*G.vd.clipend then
//      G.vd.dist:= G.vd.dist * (1.2); 
//    end;
//    else
//    if event=PADPLUSKEY then
//    begin 
//      if G.vd.persp=2 then
//      begin 
//        if (G.scene.r.scemode and R_NETWORK) then
//        begin 
//          next_camera(0); 
//        end;
//        else
//        begin 
//          G.vd.camzoom:= G.vd.camzoom + (10); 
//          if G.vd.camzoom>300 then
//          G.vd.camzoom:= 300; 
//        end;
//      end;
//      else
//      if G.vd.dist>0.$1*G.vd.grid then
//      G.vd.dist:= G.vd.dist * (.83333); 
//    end;
//    else
//    begin 

//      initgrabz(0.0,0.0,0.0); 
//      if event=PAD6 then
//      window_to_3d(vec,-32,0); 
//      else
//      if event=PAD4 then
//      window_to_3d(vec,32,0); 
//      else
//      if event=PAD8 then
//      window_to_3d(vec,0,-25); 
//      else
//      if event=PAD2 then
//      window_to_3d(vec,0,25); 
//      G.vd.ofs[0]:= G.vd.ofs[0] + (vec[0]); 
//      G.vd.ofs[1]:= G.vd.ofs[1] + (vec[1]); 
//      G.vd.ofs[2]:= G.vd.ofs[2] + (vec[2]); 
//    end;
//  end;
//  else
//  begin 

//    if event=PAD7 then
//    begin 
//      G.vd.viewquat[0]:= 1.0; 
//      G.vd.viewquat[1]:= 0.0; 
//      G.vd.viewquat[2]:= 0.0; 
//      G.vd.viewquat[3]:= 0.0; 
//      G.vd.view:= 7; 
//      if G.vd.persp>=2 then
//      G.vd.persp:= perspo; 
//    end;
//    else
//    if event=PAD1 then
//    begin 
//      G.vd.viewquat[0]:= fcos(M_PI div 4.0); 
//      G.vd.viewquat[1]:= -fsin(M_PI div 4.0); 
//      G.vd.viewquat[2]:= 0.0; 
//      G.vd.viewquat[3]:= 0.0; 
//      G.vd.view:= 1; 
//      if G.vd.persp>=2 then
//      G.vd.persp:= perspo; 
//    end;
//    else
//    if event=PAD3 then
//    begin 
//      G.vd.viewquat[0]:= 0.5; 
//      G.vd.viewquat[1]:= -0.5; 
//      G.vd.viewquat[2]:= -0.5; 
//      G.vd.viewquat[3]:= -0.5; 
//      G.vd.view:= 3; 
//      if G.vd.persp>=2 then
//      G.vd.persp:= perspo; 
//    end;
//    else
//    if event=PADMINUS then
//    begin 
//      (* deze min en max staan ook in viewmove() *)
//      if G.vd.persp=2 then
//      begin 
//        G.vd.camzoom:= G.vd.camzoom - (5); 
//        if G.vd.camzoom<-30 then
//        G.vd.camzoom:= -30; 
//      end;
//      else
//      if G.vd.dist<10.0*G.vd.clipend then
//      G.vd.dist:= G.vd.dist * (1.2); 
//    end;
//    else
//    if event=PADPLUSKEY then
//    begin 
//      if G.vd.persp=2 then
//      begin 
//        G.vd.camzoom:= G.vd.camzoom + (5); 
//        if G.vd.camzoom>300 then
//        G.vd.camzoom:= 300; 
//      end;
//      else
//      if G.vd.dist>0.$1*G.vd.grid then
//      G.vd.dist:= G.vd.dist * (.83333); 
//    end;
//    else
//    if event=PAD5 then
//    begin 
//      if G.vd.persp=1 then
//      G.vd.persp:= 0; 
//      else
//      G.vd.persp:= 1; 
//    end;
//    else
//    if event=PAD0 then
//    begin 
//      if G.qual and LR_ALTKEY then
//      begin 
//        if oldcamera<>0 then
//        begin 
//          base:= FIRSTBASE; 
//          while base
//          do
//          begin 
//            if base.object=oldcamera then
//            begin 
//              G.vd.camera:= oldcamera; 
//              break; {<= !!!b possible in "switch" - then remove this line}
//            end;
//            base:= base.next; 
//          end;
//        end;
//        handle_view3d_lock(); 
//      end;
//      else
//      if BASACT<>0 then
//      begin 
//        if G.qual and LR_CTRLKEY then
//        begin 
//          if G.vd.camera<>OBACT then
//          begin 
//            if G.vd.camera)and(G.vd.camera.type=OB_CAMERA then
//            oldcamera:= G.vd.camera; 
//            G.vd.camera:= OBACT; 
//            handle_view3d_lock(); 
//          end;
//        end;
//        else
//        if G.vd.camera=0)and(OBACT.type=OB_CAMERA then
//        begin 
//          G.vd.camera:= OBACT; 
//          handle_view3d_lock(); 
//        end;
//      end;
//      if G.vd.camera=0 then
//      begin 
//        G.vd.camera:= find_camera(); 
//        handle_view3d_lock(); 
//      end;
//      if G.vd.camera<>0 then
//      begin 
//        G.vd.persp:= 2; 
//        G.vd.view:= 0; 
//      end;
//    end;
//    else
//    if event=PAD9 then
//    begin 
//      countall(); 
//      do_all_ipos(); 
//      do_all_keys(); 
//      do_all_ikas(); 
//      reset_slowparents(); 
//    end;
//    (* editobject.c *)
//    else
//    if G.vd.persp<2 then
//    begin 
//      if event=PAD4)or(event=PAD6 then
//      begin 
//        (* z-as *)
//        phi:= M_PI div 24.0; 
//        if event=PAD6 then
//        phi:= -phi; 
//        si:= fsin(phi); 
//        q1[0]:= fcos(phi); 
//        q1[1]:= q1[2]:=0.0; 
//        q1[3]:= si; 
//        QuatMul(G.vd.viewquat,G.vd.viewquat,q1); 
//        G.vd.view:= 0; 
//      end;
//      if event=PAD2)or(event=PAD8 then
//      begin 
//        (* liggende as *)
//        VECCOPY(q1+1,G.vd.viewinv[0]); 
//        Normalise(q1+1); 
//        phi:= M_PI div 24.0; 
//        if event=PAD2 then
//        phi:= -phi; 
//        si:= fsin(phi); 
//        q1[0]:= fcos(phi); 
//        q1[1]:= q1[1] * (si); 
//        q1[2]:= q1[2] * (si); 
//        q1[3]:= q1[3] * (si); 
//        QuatMul(G.vd.viewquat,G.vd.viewquat,q1); 
//        G.vd.view:= 0; 
//      end;
//    end;

//    if G.vd.persp<2 then
//    perspo:= G.vd.persp; 
//  end;
//  addqueue(curarea^.win,REDRAW,1);
//  addqueue(curarea^.headwin,REDRAW,1);
//end;

//function untitled(name: pchar): integer; 
//var
//c: pchar; (*   c = strrchr(name, '/'); *)
//begin
//  if G.save_over=0)and(U.versions=0 then
//  begin 
//    c:= last_slash(name); 
//    if c<>0 then
//    c[1]:= 0; 
//    strcat(name,'untitled.blend'); 
//    begin
//      result:= (LongBool(1)); 
//      exit;
//    end;
//  end;
//  begin
//    result:= (LongBool(0)); 
//    exit;
//  end;
//end;

function save_image_filesel_str(str: pchar): integer;
begin
  case G.scene^.r.imtype of
    R_TARGA:
    begin
      strcpy(str,'SAVE TARGA');
      exit(1);
    end;
    R_RAWTGA:
    begin
      strcpy(str,'SAVE RAW TARGA');
        exit(1);
    end;
    R_IRIS:
    begin
      strcpy(str,'SAVE IRIS');
        exit(1);
    end;
    R_IRIZ:
    begin
      strcpy(str,'SAVE IRIS');
        exit(1);
    end;
    R_HAMX:
    begin
      strcpy(str,'SAVE HAMX');
        exit(1);
    end;
    R_FTYPE:
    begin
      strcpy(str,'SAVE FTYPE');
        exit(1);
    end;
    R_JPEG90:
    begin
      strcpy(str,'SAVE JPEG');
        exit(1);
    end;
    else
    begin
      strcpy(str,'SAVE IMAGE');
      exit(0);
    end;
  end;
end;

function blenderqread(event: word;  val: smallint): integer;
var
buts: pSpaceButs;
sa: pScrArea;
vd: pView3D;
ob: pObject;
temp: pcardinal;
ok: integer;
textspace: integer = 0;
dir: array [0..Pred(FILE_MAXDIR)] of char;
str: array [0..Pred(FILE_MAXFILE)] of char;
sfile: pSpaceFile;
begin
  (* hier alle algemene toetsafhandelingen (niet screen/window/space) *)
  (* return 0: niet aan andere queue's doorgeven *)

  if val=0 then
    exit(1);
  if (event=MOUSEY)or(event=MOUSEX) then
    exit(1);

  if (curarea<>nil)and(curarea^.spacetype=SPACE_TEXT) then
  textspace:= 1;

  case event of
    F1KEY:
    begin
      //if G.qual=0 then
      //begin
      //  (* this exception because of the '?' button *)
      //  if curarea^.spacetype=SPACE_INFO then
      //  begin
      //    sa:= closest_bigger_area();
      //    areawinset(sa^.win);
      //  end;
      //  activate_fileselect(FILE_BLENDER,'LOAD FILE',G.sce,read_file);
      //    exit(0);
      //end;
      //else
      //if G.qual and LR_SHIFTKEY then
      //begin
      //  activate_fileselect(FILE_LOADLIB,'LOAD LIBRARY',G.lib,0);
      //    exit(0);
      //end;
    end;
    F2KEY:
    begin
      //if G.qual=0 then
      //begin
      //  strcpy(dir,G.sce);
      //  untitled(dir);
      //  activate_fileselect(FILE_BLENDER,'SAVE FILE',dir,write_file);
      //    exit(0);
      //end;
      //else
      //if G.qual and LR_CTRLKEY then
      //begin
      //  write_vrml_fs();
      //    exit(0);
      //end;
      //else
      //if G.qual and LR_SHIFTKEY then
      //begin
      //  write_dxf_fs();
      //    exit(0);
      //end;
    end;
    F3KEY:
    begin
      //if G.qual=0 then
      //begin
      //  if {not}0=R.rectot then
      //  begin
      //    error('No image rendered');
      //  end;
      //  else
      //  begin
      //    if G.ima[0]=0 then
      //    begin
      //      strcpy(dir,G.sce);
      //      splitdirstring(dir,str);
      //      strcpy(G.ima,dir);
      //    end;
      //    R.r.imtype:= G.scene.r.imtype;
      //    R.r.quality:= G.scene.r.quality;
      //    R.r.planes:= G.scene.r.planes;
      //    if {not}0=save_image_filesel_str(str) then
      //    begin
      //      error('Select an image type in DisplayButtons(F10)');
      //        exit(0);
      //    end;
      //    activate_fileselect(FILE_SPECIAL,str,G.ima,write_image);
      //      exit(0);
      //  end;
      //end;
      //else
      //if G.qual and LR_CTRLKEY then
      //begin
      //  screendump();
      //end;
    end;
    F4KEY:
    begin
      //if G.qual and LR_SHIFTKEY then
      //begin
      //  sfile:=curarea^.spacedata.first; (* op zoek naar een bestaande dataselect *)
      //  while sfile
      //  do
      //  begin
      //    if sfile.spacetype=SPACE_FILE then
      //    begin
      //      if sfile.type=FILE_MAIN then
      //      begin
      //        newspace(curarea,SPACE_FILE);
      //        freefilelist(sfile);
      //        sfile.returnfunc:= 0;
      //        sfile.title[0]:= 0;
      //          exit(0);
      //      end;
      //    end;
      //    sfile:= sfile.next;
      //  end;
      //  bzero(str,16);
      //  ob:= OBACT;
      //  if ob<>0 then
      //  strcpy(str,ob.id.name);
      //  activate_fileselect(FILE_MAIN,'DATA SELECT',str,0);
      //
      //    exit(0);
      //end;
      //else
      //extern_set_butspace(event);
    end;
    F5KEY:
    begin
    //  if G.qual and LR_SHIFTKEY then
    //  begin
    //    addqueue(curarea^.headwin,REDRAW,1);
    //    curarea^.butspacetype:= SPACE_VIEW3D;
    //    do_global_buttons(B_NEWSPACE);
    //      exit(0);
    //  end;
    //  else
    //  extern_set_butspace(event);
    end;
    F6KEY:
    begin
    //  if G.qual and LR_SHIFTKEY then
    //  begin
    //    addqueue(curarea^.headwin,REDRAW,1);
    //    curarea^.butspacetype:= SPACE_IPO;
    //    do_global_buttons(B_NEWSPACE);
    //      exit(0);
    //  end;
    //  else
    //  extern_set_butspace(event);
    end;
    F7KEY:
    begin
      //if G.qual and LR_SHIFTKEY then
      //begin
      //  addqueue(curarea^.headwin,REDRAW,1);
      //  curarea^.butspacetype:= SPACE_BUTS;
      //  do_global_buttons(B_NEWSPACE);
      //    exit(0);
      //end;
      //else
      //extern_set_butspace(event);
    end;
    F8KEY:
    begin
      //if G.qual and LR_SHIFTKEY then
      //begin
      //  addqueue(curarea^.headwin,REDRAW,1);
      //  curarea^.butspacetype:= SPACE_SEQ;
      //  do_global_buttons(B_NEWSPACE);
      //    exit(0);
      //end;
      //else
      //extern_set_butspace(event);
    end;
    F9KEY:
    begin
      //if G.qual and LR_SHIFTKEY then
      //begin
      //  addqueue(curarea^.headwin,REDRAW,1);
      //  curarea^.butspacetype:= SPACE_OOPS;
      //  do_global_buttons(B_NEWSPACE);
      //    exit(0);
      //end;
      //else
      //extern_set_butspace(event);
    end;
    F10KEY:
    begin
      //if G.qual and LR_SHIFTKEY then
      //begin
      //  addqueue(curarea^.headwin,REDRAW,1);
      //  curarea^.butspacetype:= SPACE_IMAGE;
      //  do_global_buttons(B_NEWSPACE);
      //    exit(0);
      //end;
      //else
      //extern_set_butspace(event);
    end;
    F11KEY:
    begin
      //if G.qual and LR_SHIFTKEY then
      //begin
      //  addqueue(curarea^.headwin,REDRAW,1);
      //  curarea^.butspacetype:= SPACE_TEXT;
      //  do_global_buttons(B_NEWSPACE);
      //    exit(0);
      //end;
      //else
      //toggle_render_display();
      //  exit(0);
    end;
    F12KEY:
    begin
      //if G.qual and LR_SHIFTKEY then
      //begin
      //  addqueue(curarea^.headwin,REDRAW,1);
      //  curarea^.butspacetype:= SPACE_IMASEL;
      //  do_global_buttons(B_NEWSPACE);
      //    exit(0);
      //end;
      //else
      //do_renderfg(0);
      //  exit(0);
    end;
    LEFTARROWKEY,
    DOWNARROWKEY:
    begin
      //if textediting=0)and(textspace=0 then
      //begin
      //  if event=DOWNARROWKEY then
      //  CFRA:= CFRA - (10);
      //  else
      //  dec(CFRA);
      //  if G.qual and 3 then
      //  CFRA:= SFRA;
      //  if CFRA<1 then
      //  CFRA:= 1;
      //  do_global_buttons(B_NEWFRAME);
      //    exit(0);
      //end;
    end;
    RIGHTARROWKEY,
    UPARROWKEY:
    begin
      //if textediting=0)and(textspace=0 then
      //begin
      //  if event=UPARROWKEY then
      //  CFRA:= CFRA + (10);
      //  else
      //  inc(CFRA);
      //  if G.qual and 3 then
      //  CFRA:= EFRA;
      //  do_global_buttons(B_NEWFRAME);
      //end;
    end;
    ESCKEY:
    (* event doorgeven aan filesel? *)
    begin
    //  if G.curscreen^.winakt<>R.win)and(curarea^.spacetype=SPACE_FILE then
    //    exit(1);
    //  if R.win)and(R.winpop=0 then
    //  begin
    //    toggle_render_display();
    //      exit(0);
    //  end;
    //  else
    //  if R.rectot<>0 then
    //  begin
    //    ok:= 0;
    //    sa:= G.curscreen^.areabase.first;
    //    while sa
    //    do
    //    begin
    //      if sa^.spacetype=SPACE_VIEW3D then
    //      begin
    //        vd:= sa^.spacedata.first;
    //        if vd.flag and V3D_DISPIMAGE then
    //        begin
    //          addqueue(sa^.win,REDRAW,1);
    //          ok:= 1;
    //          break; {<= !!!b possible in "switch" - then remove this line}
    //        end;
    //      end;
    //      sa:= sa^.next;
    //    end;
    //    if ok<>0 then
    //      exit(0);
    //  end;
    end;
    TABKEY:
    begin
      //if G.qual=0 then
      //begin
      //  if textspace=0 then
      //  begin
      //    if curarea^.spacetype=SPACE_IPO then
      //    set_editflag_editipo();
      //    else
      //    if curarea^.spacetype=SPACE_SEQ then
      //    enter_meta();
      //    else
      //    if G.vd<>0 then
      //    begin
      //      (* ook als Alt-E *)
      //      if G.obedit=0 then
      //      enter_editmode();
      //      else
      //      exit_editmode(1);
      //    end;
      //      exit(0);
      //  end;
      //end;
      //else
      //if G.f and G_DEBUG then
      //begin
      //  printf('swap\n');
      //  glutSwapBuffers();
      //end;
    end;
    BACKSPACEKEY:
    begin
    end;
  //  AKEY:
  //  begin
  //    if textediting=0)and(textspace=0 then
  //    begin
  //      if G.qual and LR_ALTKEY then
  //      begin
  //        if G.qual and LR_SHIFTKEY then
  //        play_anim(1);
  //        else
  //        play_anim(0);
  //          exit(0);
  //      end;
  //    end;
  //  end;
  //  EKEY:
  //  begin
  //    if G.qual and LR_ALTKEY then
  //    begin
  //      if G.vd)and(textspace=0 then
  //      begin
  //        if G.obedit=0 then
  //        enter_editmode();
  //        else
  //        exit_editmode(1);
  //          exit(0);
  //      end;
  //    end;
  //  end;
  //  IKEY:
  //  begin
  //    if textediting=0)and(textspace=0)and(curarea^.spacetype<>SPACE_FILE)and(curarea^.spacetype<>SPACE_IMASEL then
  //    begin
  //      if G.qual=0 then
  //      begin
  //        common_insertkey();
  //          exit(0);
  //      end;
  //    end;
  //  end;
  //  JKEY:
  //  begin
  //    if textediting=0)and(textspace=0 then
  //    begin
  //      if R.rectot)and(G.qual=0 then
  //      begin
  //        if R.rectspare=0 then
  //        begin
  //          R.rectspare:= {!!!a type cast? =>} {puint(}callocN(sizeof(int)*R.rectx*R.recty,'rectot');
  //          R.sparex:= R.rectx;
  //          R.sparey:= R.recty;
  //        end;
  //        else
  //        if R.sparex<>R.rectx)or(R.sparey<>R.recty then
  //        begin
  //          temp:= {!!!a type cast? =>} {puint(}callocN(sizeof(int)*R.rectx*R.recty,'rectot');
  //          scalefastrect(R.rectspare,temp,R.sparex,R.sparey,R.rectx,R.recty);
  //          freeN(R.rectspare);
  //          R.rectspare:= temp;
  //          R.sparex:= R.rectx;
  //          R.sparey:= R.recty;
  //        end;
  //        SWAP(uint*,R.rectspare,R.rectot);
  //        render_display(0,R.recty-1);
  //          exit(0);
  //      end;
  //    end;
  //  end;
  //  NKEY:
  //  begin
  //    if textediting=0)and(textspace=0 then
  //    begin
  //      if G.qual and LR_CTRLKEY then
  //      ;
  //      else
  //      if G.qual=0)or((G.qual and LR_SHIFTKEY) then
  //      begin
  //        clever_numbuts();
  //          exit(0);
  //      end;
  //    end;
  //  end;
  //  OKEY:
  //  begin
  //    if textediting=0 then
  //    begin
  //      if G.qual and LR_CTRLKEY then
  //      begin
  //        sprintf(str,'Open file: %s',G.sce);
  //        if okee(str) then
  //        begin
  //          strcpy(dir,G.sce);
  //          read_file(dir);
  //        end;
  //          exit(0);
  //      end;
  //    end;
  //  end;
  //  SKEY:
  //  begin
  //    if G.obedit=0 then
  //    begin
  //      if G.qual and LR_CTRLKEY then
  //      begin
  //        if G.qual and LR_SHIFTKEY then
  //        ;
  //        else
  //        begin
  //          strcpy(dir,G.sce);
  //          write_file(dir);
  //          free_filesel_spec(dir);
  //            exit(0);
  //        end;
  //      end;
  //    end;
  //  end;
  //  TKEY:
  //  begin
  //    if G.qual and LR_ALTKEY then
  //    begin
  //      if G.qual and LR_CTRLKEY then
  //      begin
  //
  //        event:= pupmenu('10 Timer%t|draw|draw+swap');
  //        if event>0 then
  //        begin
  //          printf('start timer\n');
  //          waitcursor(1);
  //          start_timer();
  //          for{while} a:=0 to Pred(10) { a++}
  //          do
  //          begin
  //            curarea^.windraw();
  //            if event=2 then
  //            screen_swapbuffers();
  //          end;
  //        end_timer(@time,0);
  //        if event=1 then
  //        printf('draw %d\n',time);
  //        if event=2 then
  //        printf('d+sw %d\n',time);
  //        waitcursor(0);
  //          exit(0);
  //      end;
  //    end;
  //  end;
  //end;
  //UKEY:
  //begin
  //  if textediting=0 then
  //  begin
  //    if G.qual and LR_CTRLKEY then
  //    begin
  //      if okee('SAVE USER DEFAULTS') then
  //      begin
  //        if write_homefile()=0 then
  //        error('Can''t write ~/.B.blend');
  //      end;
  //        exit(0);
  //    end;
  //  end;
  //end;
  //WKEY:
  //begin
  //  if textediting=0 then
  //  begin
  //    if G.qual and LR_CTRLKEY then
  //    begin
  //      if G.qual and LR_SHIFTKEY then
  //      ;
  //      else
  //      begin
  //        strcpy(dir,G.sce);
  //        if untitled(dir) then
  //        begin
  //          activate_fileselect(FILE_BLENDER,'SAVE FILE',dir,write_file);
  //        end;
  //        else
  //        begin
  //          write_file(dir);
  //          free_filesel_spec(dir);
  //        end;
  //          exit(0);
  //      end;
  //    end;
  //    else
  //    if G.qual and LR_ALTKEY then
  //    begin
  //      write_videoscape_fs();
  //    end;
  //  end;
  //end;
  //XKEY:
  //begin
  //  if G.qual and LR_CTRLKEY then
  //  begin
  //    if okee('ERASE ALL') then
  //    begin
  //      if read_homefile()=0 then
  //      error('No file ~/.B.blend');
  //    end;
  //      exit(0);
  //  end;
  //end;
  //ZKEY:
  //begin
  //  if R.win)and(R.win=G.curscreen^.winakt then
  //  begin
  //    zoomwin();
  //      exit(0);
  //  end;
  //end;
end;
  exit(1);
end;

end.
