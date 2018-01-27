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
(*  space.c   jan/juli 94     GRAPHICS
 *
 *  - hier het initialiseren en vrijgeven van SPACE data
 *
 *)
unit space;

interface

uses
ctypes, cfuncs,
util, screen;

procedure freespacelist(lb: pListBase);
procedure duplicatespacelist(lb1, lb2: pListBase);
procedure newspace(sa: pScrArea;  type_: integer);

implementation

uses
GL,
blender, blenglob, editipo, blendef, mywindow;
////{$include "blender.h"}
////{$include "sequence.h"}
////{$include "graphics.h"}
//
//procedure defheaddraw; 
//(* screen.c *)
//
//procedure force_draw; 

procedure set_func_space(sa: pScrArea); forward;

//procedure allqueue(event: ushort;  val: smallint); 
//
//implementation
//
//
//procedure drawemptyspace; 
//begin
//  glClearColor(0.5,0.5,0.5,0.0); 
//  glClear(GL_COLOR_BUFFER_BIT); 
//end;
//(* ************* SPACE: VIEW3D  ************* *)
//
//procedure drawview3d; 
//
//procedure copy_view3d_lock(val: smallint); 
//var
//vd: pView3D; 
//sa: pScrArea;
//sc: pbScreen; 
//bit: integer; (* van G.scene naar andere views kopieeren *)
//begin
//  
//  
//  
//  sc:= G.main.screen.first; 
//  while sc
//  do
//  begin 
//    if sc^.scene=G.scene
//    then
//    begin 
//      sa:= sc^.areabase.first;
//      while sa
//      do
//      begin 
//        vd:= sa^.spacedata.first; 
//        while vd
//        do
//        begin 
//          if vd.spacetype=SPACE_OOPS)and(val=REDRAW
//          then
//          begin 
//            if sa^.win<>nil 
//            then
//            addqueue(sa^.win,REDRAW,1); 
//          end;
//          else
//          if vd.spacetype=SPACE_VIEW3D)and(vd.scenelock
//          then
//          begin 
//            if vd.localview=0
//            then
//            begin 
//              vd.lay:= G.scene.lay; 
//              vd.camera:= G.scene.camera; 
//              if vd.camera=0)and(vd.persp>1
//              then
//              vd.persp:= 1; 
//              if (vd.lay and vd.layact)=0
//              then
//              begin 
//                bit:= 0; 
//                while bit<32
//                do
//                begin 
//                  if vd.lay and (1 shl bit)
//                  then
//                  begin 
//                    vd.layact:= 1 shl bit; 
//                    break; {<= !!!b possible in "switch" - then remove this line}
//                  end;
//                  inc(bit); 
//                end;
//              end;
//              if val=REDRAW)and(vd=sa^.spacedata.first
//              then
//              begin 
//                if sa^.win<>nil 
//                then
//                addqueue(sa^.win,REDRAW,1); 
//                if sa^.headwin<>nil 
//                then
//                addqueue(sa^.headwin,REDRAW,1); 
//              end;
//            end;
//          end;
//          vd:= vd.next; 
//        end;
//        sa:= sa^.next; 
//      end;
//    end;
//    sc:= sc^.id.next;
//  end;
//end;
//
//procedure handle_view3d_lock; 
//begin
//  if G.vd.localview=0)and(G.vd.scenelock)and(curarea.spacetype=SPACE_VIEW3D
//  then
//  begin 
//    (* naar scene kopieeren *)
//    G.scene.lay:= G.vd.lay; 
//    G.scene.camera:= G.vd.camera; 
//    copy_view3d_lock(REDRAW); 
//  end;
//end;
//
//procedure changeview3d; 
//begin
//  setwinmatrixview3d(0); 
//end;(* 0= geen pick rect *)
//
//var {was static}
//padevent: integer;
// 
//
//procedure winqread3d(event: ushort;  val: smallint); 
//var
//ob: pObject; 
//curs: pfloat; 
//doredraw: integer;
// 
//textedit: integer; 
//pupval: integer; 
//begin
//  
//  padevent:=0; 
//  
//  doredraw:=0; 
//  
//  
//  if curarea.win=0
//  then
//  exit;
//  (* hier komtie vanuit sa->headqread() *)
//  if event=MOUSEY
//  then
//  exit;
//  if val<>nil 
//  then
//  begin 
//    (* TEXTEDITING?? *)
//    if G.obedit)and(G.obedit.type=OB_FONT
//    then
//    begin 
//      case event of
//        LEFTMOUSE:
//        begin
//          mouse_cursor(); 
//        end;
//        MIDDLEMOUSE:
//        begin
//          if U.flag and VIEWMOVE
//          then
//          begin 
//            if G.qual and LR_SHIFTKEY
//            then
//            viewmove(0); 
//            else
//            if G.qual and LR_CTRLKEY
//            then
//            viewmove(2); 
//            else
//            viewmove(1); 
//          end;
//          else
//          begin 
//            if G.qual and LR_SHIFTKEY
//            then
//            viewmove(1); 
//            else
//            if G.qual and LR_CTRLKEY
//            then
//            viewmove(2); 
//            else
//            viewmove(0); 
//          end;
//          {goto next_label;}{<= !!!d case label without "break"}
//        end;
//        UKEY:
//        begin
//          if G.qual and LR_ALTKEY
//          then
//          begin 
//            remake_editText(); 
//            doredraw:= 1; 
//            padevent:= 2; 
//          end;
//        end;
//        KEYBD:
//        begin
//          if padevent=0
//          then
//          begin 
//            do_textedit(event,val); 
//          end;
//        end;
//        PAD0,
//        PAD1,
//        PAD2,
//        PAD3,
//        PAD4,
//        PAD5,
//        PAD6,
//        PAD7,
//        PAD8,
//        PAD9,
//        PADENTER:
//        (* hier is PADPLUSKEY en PADMINUS weg vanwege de 'KEYBD' event *)
//        begin
//          persptoetsen(event); 
//          padevent:= 2; 
//          doredraw:= 1; 
//        end;
//        else
//        begin
//          do_textedit(event,val); 
//        end;
//        
//      end;{case?}
//      if padevent<>nil 
//      then
//      dec(padevent); 
//    end;
//    else
//    begin 
//      case event of
//        BACKBUFDRAW:
//        begin
//          backdrawview3d(1); 
//        end;
//        LEFTMOUSE:
//        begin
//          if G.f and G_VERTEXPAINT
//          then
//          vertex_paint(); 
//          else
//          mouse_cursor(); 
//        end;
//        MIDDLEMOUSE:
//        begin
//          if U.flag and VIEWMOVE
//          then
//          begin 
//            if G.qual and LR_SHIFTKEY
//            then
//            viewmove(0); 
//            else
//            if G.qual and LR_CTRLKEY
//            then
//            viewmove(2); 
//            else
//            viewmove(1); 
//          end;
//          else
//          begin 
//            if G.qual and LR_SHIFTKEY
//            then
//            viewmove(1); 
//            else
//            if G.qual and LR_CTRLKEY
//            then
//            viewmove(2); 
//            else
//            viewmove(0); 
//          end;
//        end;
//        RIGHTMOUSE:
//        begin
//          if G.obedit)and((G.qual and LR_CTRLKEY)=0
//          then
//          begin 
//            if{!!!e unknown token}
//            mouse_mesh(); 
//            else
//            if{!!!e unknown token}
//            mouse_nurb(); 
//            else
//            if G.obedit.type=OB_MBALL
//            then
//            mouse_mball(); 
//            else
//            if G.obedit.type=OB_LATTICE
//            then
//            mouse_lattice(); 
//          end;
//          else
//          if G.qual and LR_CTRLKEY
//          then
//          mouse_select(); 
//          else
//          if G.f and G_FACESELECT
//          then
//          face_select(); 
//          else
//          if G.f and G_VERTEXPAINT
//          then
//          sample_vpaint(); 
//          else
//          mouse_select(); 
//        end;
//        ONEKEY:
//        begin
//          do_layer_toets(0); 
//        end;
//        TWOKEY:
//        begin
//          do_layer_toets(1); 
//        end;
//        THREEKEY:
//        begin
//          do_layer_toets(2); 
//        end;
//        FOURKEY:
//        begin
//          do_layer_toets(3); 
//        end;
//        FIVEKEY:
//        begin
//          do_layer_toets(4); 
//        end;
//        SIXKEY:
//        begin
//          do_layer_toets(5); 
//        end;
//        SEVENKEY:
//        begin
//          do_layer_toets(6); 
//        end;
//        EIGHTKEY:
//        begin
//          do_layer_toets(7); 
//        end;
//        NINEKEY:
//        begin
//          do_layer_toets(8); 
//        end;
//        ZEROKEY:
//        begin
//          do_layer_toets(9); 
//        end;
//        MINUSKEY:
//        begin
//          do_layer_toets(10); 
//        end;
//        EQUALKEY:
//        begin
//          do_layer_toets(11); 
//        end;
//        ACCENTGRAVEKEY:
//        begin
//          do_layer_toets(-1); 
//        end;
//        AKEY:
//        begin
//          if G.qual and LR_CTRLKEY
//          then
//          apply_object(); 
//          else
//          if G.qual and LR_SHIFTKEY
//          then
//          begin 
//            tbox_setmain(0); 
//            toolbox(); 
//          end;
//          else
//          begin 
//            if G.obedit<>nil 
//            then
//            begin 
//              if{!!!e unknown token}
//              deselectall_mesh(); 
//              else
//              if{!!!e unknown token}
//              deselectall_nurb(); 
//              else
//              if G.obedit.type=OB_MBALL
//              then
//              deselectall_mball(); 
//              else
//              if G.obedit.type=OB_LATTICE
//              then
//              deselectall_Latt(); 
//            end;
//            else
//            begin 
//              if G.f and G_FACESELECT
//              then
//              deselectall_tface(); 
//              else
//              deselectall(); 
//            end;
//          end;
//        end;
//        BKEY:
//        begin
//          if G.qual and LR_SHIFTKEY
//          then
//          set_render_border(); 
//          else
//          borderselect(); 
//        end;
//        CKEY:
//        begin
//          if G.qual and LR_CTRLKEY
//          then
//          begin 
//            copymenu(); 
//          end;
//          else
//          if G.qual and LR_ALTKEY
//          then
//          begin 
//            convertmenu(); 
//          end;
//          (* editobject.c *)
//          else
//          if G.qual and LR_SHIFTKEY
//          then
//          begin 
//            view3d_home(1); 
//            curs:= give_cursor(); 
//            curs[0]:= curs[1]:=curs[2]:=0.0; 
//            addqueue(curarea.win,REDRAW,1); 
//          end;
//          else
//          if G.obedit<>0)and(ELEM(G.obedit.type,OB_CURVE,OB_SURF)
//          then
//          begin 
//            makecyclicNurb(); 
//            makeDispList(G.obedit); 
//            allqueue(REDRAWVIEW3D,0); 
//          end;
//          else
//          begin 
//            curs:= give_cursor(); 
//            G.vd.ofs[0]:= -curs[0]; 
//            G.vd.ofs[1]:= -curs[1]; 
//            G.vd.ofs[2]:= -curs[2]; 
//            addqueue(curarea.win,REDRAW,1); 
//          end;
//        end;
//        DKEY:
//        begin
//          if G.qual and LR_SHIFTKEY
//          then
//          begin 
//            if G.obedit<>nil 
//            then
//            begin 
//              if{!!!e unknown token}
//              adduplicate_mesh(); 
//              else
//              if G.obedit.type=OB_MBALL
//              then
//              adduplicate_mball(); 
//              else
//              if{!!!e unknown token}
//              adduplicate_nurb(); 
//            end;
//            else
//            adduplicate(0); 
//          end;
//          else
//          if G.qual and LR_ALTKEY
//          then
//          begin 
//            if G.obedit=0
//            then
//            adduplicate(0); 
//          end;
//          else
//          if G.qual and LR_CTRLKEY
//          then
//          begin 
//            imagestodisplist(); 
//          end;
//          {$ifdef FREE}
//          else
//          begin 
//            pupval:= pupmenu('Draw mode%t|BoundBox %x1|Wire %x2|OpenGL Solid %x3|Shaded solid %x4'); 
//            if pupval>0
//            then
//            begin 
//              G.vd.drawtype:= pupval; 
//              doredraw:= 1; 
//            end;
//          end;
//          {$else}
//          else
//          if G.main.sector.first<>nil 
//          then
//          sector_simulate(); 
//          {$endif}
//        end;
//        EKEY:
//        begin
//          if G.obedit<>nil 
//          then
//          begin 
//            if{!!!e unknown token}
//            extrude_mesh(); 
//            else
//            if G.obedit.type=OB_CURVE
//            then
//            addvert_Nurb('e'); 
//            else
//            if G.obedit.type=OB_SURF
//            then
//            extrude_nurb(); 
//          end;
//          else
//          begin 
//            ob:= OBACT; 
//            if ob)and(ob.type=OB_IKA
//            then
//            if okee('extrude IKA')
//            then
//            extrude_ika(ob,1); 
//          end;
//        end;
//        FKEY:
//        begin
//          if G.obedit<>nil 
//          then
//          begin 
//            
//            function ELEM3{!!!3 unknown typedef}: if; 
//            begin
//              if G.qual and LR_SHIFTKEY
//              then
//              fill_mesh(); 
//              else
//              if G.qual and LR_ALTKEY
//              then
//              beauty_fill(); 
//              else
//              addedgevlak_mesh(); 
//            end
//            else
//            if{!!!e unknown token}
//            addsegment_nurb(); 
//          end;
//          else
//          if G.qual and LR_CTRLKEY
//          then
//          sort_faces(); 
//          else
//          if G.qual and LR_ALTKEY
//          then
//          first_base(); 
//          else
//          if G.qual and LR_SHIFTKEY
//          then
//          fly(); 
//          else
//          {$ifndef FREE}
//          begin 
//            set_faceselect(); 
//            {$endif}
//          end;
//        end;
//        GKEY:
//        begin
//          if G.qual and LR_ALTKEY
//          then
//          clear_object('g'); 
//          else
//          transform('g'); 
//        end;
//        HKEY:
//        begin
//          if G.obedit<>nil 
//          then
//          begin 
//            
//            function ELEM3{!!!3 unknown typedef}: if; 
//            begin
//              if G.qual and LR_ALTKEY
//              then
//              reveal_mesh(); 
//              else
//              hide_mesh(G.qual and LR_SHIFTKEY); 
//            end;
//            else
//            if G.obedit.type=OB_SURF
//            then
//            begin 
//              if G.qual and LR_ALTKEY
//              then
//              revealNurb(); 
//              else
//              hideNurb(G.qual and LR_SHIFTKEY); 
//            end;
//            else
//            if G.obedit.type=OB_CURVE
//            then
//            begin 
//              if G.qual and 48
//              then
//              autocalchandlesNurb_all(1); 
//              else
//              (* flag=1, selected *)
//              if G.qual and 3
//              then
//              sethandlesNurb(1); 
//              else
//              sethandlesNurb(3); 
//              makeDispList(G.obedit); 
//              allqueue(REDRAWVIEW3D,0); 
//            end;
//          end;
//          else
//          if G.f and G_FACESELECT
//          then
//          hide_tface(); 
//        end;
//        IKEY:
//        begin
//        end;
//        JKEY:
//        begin
//          if G.qual and LR_CTRLKEY
//          then
//          begin 
//            if ob:=OBACT
//            then
//            begin 
//              if ob.type=OB_MESH
//              then
//              join_mesh(); 
//              else
//              if ob.type=OB_CURVE
//              then
//              join_curve(OB_CURVE); 
//              else
//              if ob.type=OB_SURF
//              then
//              join_curve(OB_SURF); 
//              else
//              if ob.type=OB_SECTOR
//              then
//              join_sector(); 
//            end;
//            (* editmesh.c *)
//          end;
//        end;
//        KKEY:
//        begin
//          if G.obedit<>nil 
//          then
//          begin 
//            if G.obedit.type=OB_SURF
//            then
//            printknots(); 
//          end;
//          else
//          begin 
//            if G.qual and LR_SHIFTKEY
//            then
//            begin 
//              if G.f and G_VERTEXPAINT
//              then
//              clear_vpaint(); 
//              else
//              select_select_keys(); 
//            end;
//            else
//            if G.qual and LR_CTRLKEY
//            then
//            make_skeleton(); 
//            else
//            (*      else if(G.qual & LR_ALTKEY) delete_skeleton(); *)
//            set_ob_ipoflags(); 
//          end;
//        end;
//        LKEY:
//        begin
//          if G.obedit<>nil 
//          then
//          begin 
//            if{!!!e unknown token}
//            selectconnected_mesh(); 
//            else
//            if{!!!e unknown token}
//            selectconnected_nurb(); 
//          end;
//          else
//          begin 
//            if G.qual and LR_SHIFTKEY
//            then
//            selectlinks(); 
//            else
//            if G.qual and LR_CTRLKEY
//            then
//            linkmenu(); 
//            else
//            if G.f and G_FACESELECT
//            then
//            select_linked_tfaces(); 
//            else
//            make_local(); 
//          end;
//        end;
//        MKEY:
//        begin
//          movetolayer(); 
//        end;
//        NKEY:
//        begin
//          if G.obedit<>nil 
//          then
//          begin 
//            
//            function ELEM3{!!!3 unknown typedef}: if; 
//            begin
//              if G.qual and LR_SHIFTKEY
//              then
//              begin 
//                if okee('Recalc normals inside')
//                then
//                righthandfaces(2); 
//              end;
//              else
//              begin 
//                if okee('Recalc normals outside')
//                then
//                righthandfaces(1); 
//              end;
//              allqueue(REDRAWVIEW3D,0); 
//            end;
//          end;
//          {$ifndef FREE}
//          else
//          if G.qual and LR_CTRLKEY
//          then
//          add_networklink(); 
//          {$endif}
//        end;
//        OKEY:
//        begin
//          if G.qual and LR_ALTKEY
//          then
//          clear_object('o'); 
//          else
//          if G.obedit<>nil 
//          then
//          begin 
//            {prop_mode: integer; }{<= !!!5 external variable}
//            if G.qual and LR_SHIFTKEY
//            then
//            prop_mode:= {not}0=prop_mode; 
//            else
//            G.f:= G.f xor (G_PROPORTIONAL); 
//            allqueue(REDRAWHEADERS,0); 
//          end;
//        end;
//        PKEY:
//        begin
//          if G.obedit<>nil 
//          then
//          begin 
//            if G.qual<>nil 
//            then
//            begin 
//              if G.qual and LR_CTRLKEY
//              then
//              make_parent(); 
//            end;
//            else
//            if{!!!e unknown token}
//            separate_mesh(); 
//            else
//            if{!!!e unknown token}
//            separate_nurb(); 
//          end;
//          else
//          if G.qual and LR_CTRLKEY
//          then
//          make_parent(); 
//          else
//          if G.qual and LR_ALTKEY
//          then
//          clear_parent(); 
//        end;
//        RKEY:
//        begin
//          if G.obedit=0)and((G.f and G_FACESELECT)
//          then
//          rotate_uv_tface(); 
//          else
//          if G.qual and LR_ALTKEY
//          then
//          clear_object('r'); 
//          else
//          if G.qual and LR_SHIFTKEY
//          then
//          selectrow_nurb(); 
//          else
//          transform('r'); 
//        end;
//        SKEY:
//        begin
//          if G.qual and LR_ALTKEY
//          then
//          begin 
//            if G.obedit<>nil 
//            then
//            ; 
//            else
//            clear_object('s'); 
//          end;
//          else
//          if G.qual and LR_SHIFTKEY
//          then
//          snapmenu(); 
//          else
//          if G.qual and LR_CTRLKEY
//          then
//          begin 
//            if G.obedit<>nil 
//            then
//            transform('S'); 
//          end;
//          else
//          transform('s'); 
//        end;
//        TKEY:
//        begin
//          if G.qual and LR_CTRLKEY
//          then
//          begin 
//            if G.obedit<>nil 
//            then
//            begin 
//              
//              function ELEM3{!!!3 unknown typedef}: if; 
//              begin
//                convert_to_triface(0); 
//                allqueue(REDRAWVIEW3D,0); 
//                countall(); 
//              end;
//            end;
//            else
//            make_track(); 
//          end;
//          else
//          if G.qual and LR_ALTKEY
//          then
//          begin 
//            if G.obedit)and(G.obedit.type=OB_CURVE
//            then
//            clear_tilt(); 
//            else
//            clear_track(); 
//          end;
//          else
//          begin 
//            if G.obedit<>nil 
//            then
//            transform('t'); 
//            else
//            texspace_edit(); 
//          end;
//        end;
//        UKEY:
//        begin
//          if G.obedit<>nil 
//          then
//          begin 
//            if{!!!e unknown token}
//            remake_editMesh(); 
//            else
//            if{!!!e unknown token}
//            remake_editNurb(); 
//            else
//            if G.obedit.type=OB_LATTICE
//            then
//            remake_editLatt(); 
//          end;
//          else
//          if G.f and G_FACESELECT
//          then
//          uv_autocalc_tface(); 
//          else
//          if G.f and G_VERTEXPAINT
//          then
//          vpaint_undo(); 
//          else
//          single_user(); 
//        end;
//        VKEY:
//        begin
//          if G.obedit<>nil 
//          then
//          begin 
//            if G.obedit.type=OB_CURVE
//            then
//            begin 
//              sethandlesNurb(2); 
//              makeDispList(G.obedit); 
//              allqueue(REDRAWVIEW3D,0); 
//            end;
//          end;
//          else
//          if G.qual and LR_ALTKEY
//          then
//          image_aspect(); 
//          else
//          set_vpaint(); 
//        end;
//        WKEY:
//        begin
//          if G.qual and LR_SHIFTKEY
//          then
//          begin 
//            transform('w'); 
//          end;
//          else
//          if G.qual and LR_ALTKEY
//          then
//          begin 
//            (* if(G.obedit && G.obedit->type==OB_MESH) write_videoscape(); *)
//          end;
//          else
//          if G.qual and LR_CTRLKEY
//          then
//          begin 
//            if G.obedit<>nil 
//            then
//            begin 
//              
//              function ELEM{!!!3 unknown typedef}: if; 
//              begin
//                switchdirectionNurb2(); 
//              end;
//            end;
//          end;
//          else
//          special_editmenu(); 
//        end;
//        XKEY:
//        begin
//          if G.obedit<>nil 
//          then
//          begin 
//            if{!!!e unknown token}
//            delete_mesh(); 
//            else
//            if{!!!e unknown token}
//            delNurb(); 
//            else
//            if G.obedit.type=OB_MBALL
//            then
//            delete_mball(); 
//          end;
//          else
//          delete_obj(0); 
//        end;
//        YKEY:
//        begin
//          if G.obedit<>nil 
//          then
//          begin 
//            if{!!!e unknown token}
//            split_mesh(); 
//          end;
//        end;
//        ZKEY:
//        begin
//          do_realtimelight(0,0,0); 
//          if G.qual and LR_CTRLKEY
//          then
//          begin 
//            reshadeall_displist(); 
//            G.vd.drawtype:= OB_SHADED; 
//          end;
//          else
//          if G.qual and LR_SHIFTKEY
//          then
//          begin 
//            if G.vd.drawtype=OB_SHADED
//            then
//            G.vd.drawtype:= OB_WIRE; 
//            else
//            G.vd.drawtype:= OB_SHADED; 
//          end;
//          else
//          if G.qual and LR_ALTKEY
//          then
//          begin 
//            if G.vd.drawtype=OB_TEXTURE
//            then
//            G.vd.drawtype:= OB_SOLID; 
//            else
//            G.vd.drawtype:= OB_TEXTURE; 
//          end;
//          else
//          begin 
//            if G.vd.drawtype=OB_SOLID)or(G.vd.drawtype=OB_SHADED
//            then
//            G.vd.drawtype:= OB_WIRE; 
//            else
//            G.vd.drawtype:= OB_SOLID; 
//          end;
//          addqueue(curarea.headwin,REDRAW,1); 
//          addqueue(curarea.win,REDRAW,1); 
//        end;
//        HOMEKEY:
//        begin
//          view3d_home(0); 
//        end;
//        COMMAKEY:
//        begin
//          G.vd.around:= V3D_CENTRE; 
//          addqueue(curarea.headwin,REDRAW,1); 
//        end;
//        PERIODKEY:
//        begin
//          G.vd.around:= V3D_CURSOR; 
//          addqueue(curarea.headwin,REDRAW,1); 
//        end;
//        PADVIRGULEKEY:
//        (* '/' *)
//        begin
//          if G.vd.localview<>nil 
//          then
//          begin 
//            G.vd.localview:= 0; 
//          endlocalview(curarea); 
//        end;
//        else
//        begin 
//          G.vd.localview:= 1; 
//          initlocalview(); 
//        end;
//        addqueue(curarea.headwin,REDRAW,1); 
//      end;
//      PADASTERKEY:
//      (* '*' *)
//      begin
//        ob:= OBACT; 
//        if ob<>nil 
//        then
//        begin 
//          obmat_to_viewmat(ob); 
//          if G.vd.persp=2
//          then
//          G.vd.persp:= 1; 
//          addqueue(curarea.win,REDRAW,1); 
//        end;
//      end;
//      PADPERIOD:
//      (* '.' *)
//      begin
//        centreview(); 
//      end;
//      PAGEUPKEY:
//      begin
//        if G.qual and LR_CTRLKEY
//        then
//        movekey_obipo(1); 
//        else
//        nextkey_obipo(1); 
//      end;
//      PAGEDOWNKEY:
//      (* in editipo.c *)
//      begin
//        if G.qual and LR_CTRLKEY
//        then
//        movekey_obipo(-1); 
//        else
//        nextkey_obipo(-1); 
//      end;
//      PAD0,
//      PAD1,
//      PAD2,
//      PAD3,
//      PAD4,
//      PAD5,
//      PAD6,
//      PAD7,
//      PAD8,
//      PAD9,
//      PADMINUS,
//      PADPLUSKEY,
//      PADENTER:
//      begin
//        persptoetsen(event); 
//        doredraw:= 1; 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      
//    end;{case?}
//  end;
//end;
//if doredraw<>nil 
//then
//addqueue(curarea.win,REDRAW,1); 
//end;
//
//procedure initview3d(sa: pScrArea);
//var
//vd: pView3D; 
//begin
//  
//  vd:= callocN(sizeof(View3D),'initview3d'); 
//  addhead(@sa^.spacedata,vd); 
//  set_func_space(sa); 
//  (* addhead! niet addtail *)
//  vd.spacetype:= SPACE_VIEW3D; 
//  vd.viewquat[0]:= 1.0; 
//  vd.viewquat[1]:= vd.viewquat[2]:=vd.viewquat[3]:=0.0; 
//  vd.persp:= 1; 
//  vd.drawtype:= OB_WIRE; 
//  vd.view:= 7; 
//  vd.dist:= 10.0; 
//  vd.lens:= 35.0; 
//  vd.clipsta:= 0.$1; 
//  vd.clipend:= 500.0; 
//  vd.grid:= 1.0; 
//  vd.gridlines:= 16; 
//  vd.lay:= vd.layact:=1; 
//  if G.scene<>nil 
//  then
//  begin 
//    vd.lay:= vd.layact:=G.scene.lay; 
//    vd.camera:= G.scene.camera; 
//  end;
//  vd.scenelock:= 1; 
//end;
//(* ******************** SPACE: IPO ********************** *)
//
//procedure drawipo; 
//
//procedure changeview2d; 
//var
//v2d: pView2D; 
//xmin: float; 
//xmax: float; 
//ymin: float; 
//ymax: float; 
//begin
//  
//  
//  
//  
//  
//  if G.v2d=0
//  then
//  exit;
//  ortho2(G.v2d.cur.xmin,G.v2d.cur.xmax,G.v2d.cur.ymin,G.v2d.cur.ymax); 
//  test_view2d(); 
//end;
//
//procedure winqreadipo(event: ushort;  val: smallint); 
//var
//ob: pObject; 
//dx: float; 
//dy: float; 
//doredraw: integer;
// 
//begin
//  
//  
//  
//  doredraw:=0; 
//  if curarea.win=0
//  then
//  exit;
//  if val<>nil 
//  then
//  begin 
//    case event of
//      LEFTMOUSE:
//      begin
//        if in_ipo_buttons()
//        then
//        begin 
//          FrontbufferButs(LongBool(1)); 
//          val:= DoButtons(); 
//          FrontbufferButs(LongBool(0)); 
//          if val<>nil 
//          then
//          do_ipowin_buts(val-1); 
//          else
//          begin 
//            do_ipo_selectbuttons(); 
//            doredraw:= 1; 
//          end;
//        end;
//        if G.qual and LR_CTRLKEY
//        then
//        add_vert_ipo(); 
//        else
//        gesture(); 
//      end;
//      MIDDLEMOUSE:
//      begin
//        if in_ipo_buttons()
//        then
//        begin 
//          scroll_ipobuts(); 
//        end;
//        else
//        view2dmove(); 
//      end;
//      RIGHTMOUSE:
//      (* in drawipo.c *)
//      begin
//        mouse_select_ipo(); 
//      end;
//      PADPLUSKEY:
//      begin
//        dx:= 0.1154*(G.v2d.cur.xmax-G.v2d.cur.xmin); 
//        dy:= 0.1154*(G.v2d.cur.ymax-G.v2d.cur.ymin); 
//        if val=SPACE_BUTS
//        then
//        begin 
//          dx:= dx div (2.0); 
//          dy:= dy div (2.0); 
//        end;
//        G.v2d.cur.xmin:= G.v2d.cur.xmin + (dx); 
//        G.v2d.cur.xmax:= G.v2d.cur.xmax - (dx); 
//        G.v2d.cur.ymin:= G.v2d.cur.ymin + (dy); 
//        G.v2d.cur.ymax:= G.v2d.cur.ymax - (dy); 
//        test_view2d(); 
//        doredraw:= 1; 
//      end;
//      PADMINUS:
//      begin
//        dx:= 0.15*(G.v2d.cur.xmax-G.v2d.cur.xmin); 
//        dy:= 0.15*(G.v2d.cur.ymax-G.v2d.cur.ymin); 
//        if val=SPACE_BUTS
//        then
//        begin 
//          dx:= dx div (2.0); 
//          dy:= dy div (2.0); 
//        end;
//        G.v2d.cur.xmin:= G.v2d.cur.xmin - (dx); 
//        G.v2d.cur.xmax:= G.v2d.cur.xmax + (dx); 
//        G.v2d.cur.ymin:= G.v2d.cur.ymin - (dy); 
//        G.v2d.cur.ymax:= G.v2d.cur.ymax + (dy); 
//        test_view2d(); 
//        doredraw:= 1; 
//      end;
//      PAGEUPKEY:
//      begin
//        if G.qual and LR_CTRLKEY
//        then
//        movekey_ipo(1); 
//        else
//        nextkey_ipo(1); 
//      end;
//      PAGEDOWNKEY:
//      begin
//        if G.qual and LR_CTRLKEY
//        then
//        movekey_ipo(-1); 
//        else
//        nextkey_ipo(-1); 
//      end;
//      HOMEKEY:
//      begin
//        do_ipo_buttons(B_IPOHOME); 
//      end;
//      AKEY:
//      begin
//        if in_ipo_buttons()
//        then
//        begin 
//          swap_visible_editipo(); 
//        end;
//        else
//        swap_selectall_editipo(); 
//      end;
//      BKEY:
//      begin
//        borderselect_ipo(); 
//      end;
//      CKEY:
//      begin
//        move_to_frame(); 
//      end;
//      DKEY:
//      begin
//        if G.qual and LR_SHIFTKEY
//        then
//        add_duplicate_editipo(); 
//      end;
//      GKEY:
//      begin
//        transform_ipo('g'); 
//      end;
//      HKEY:
//      begin
//        if G.qual and LR_SHIFTKEY
//        then
//        sethandles_ipo(HD_AUTO); 
//        else
//        sethandles_ipo(HD_ALIGN); 
//      end;
//      JKEY:
//      begin
//        join_ipo(); 
//      end;
//      KKEY:
//      begin
//        if G.sipo.showkey<>nil 
//        then
//        begin 
//          G.sipo.showkey:= 0; 
//          swap_selectall_editipo(); 
//        end;
//        (* sel all *)
//        else
//        G.sipo.showkey:= 1; 
//        free_ipokey(@G.sipo.ipokey); 
//        if G.sipo.ipo<>nil 
//        then
//        G.sipo.ipo.showkey:= G.sipo.showkey; 
//        addqueue(curarea.headwin,REDRAW,1); 
//        allqueue(REDRAWVIEW3D,0); 
//        doredraw:= 1; 
//      end;
//      RKEY:
//      begin
//        ipo_record(); 
//      end;
//      SKEY:
//      begin
//        if G.qual and LR_SHIFTKEY
//        then
//        ipo_snapmenu(); 
//        else
//        transform_ipo('s'); 
//      end;
//      TKEY:
//      begin
//        set_ipotype(); 
//      end;
//      VKEY:
//      begin
//        sethandles_ipo(HD_VECT); 
//      end;
//      XKEY:
//      begin
//        if G.qual and LR_SHIFTKEY
//        then
//        delete_key(); 
//        else
//        del_ipo(); 
//      end;
//      
//    end;{case?}
//  end;
//  if doredraw<>nil 
//  then
//  addqueue(curarea.win,REDRAW,1); 
//end;
//
//procedure initipo(sa: pScrArea);
//var
//sipo: pSpaceIpo; 
//begin
//  
//  sipo:= callocN(sizeof(SpaceIpo),'initipo'); 
//  addhead(@sa^.spacedata,sipo); 
//  set_func_space(sa); 
//  sipo.spacetype:= SPACE_IPO; 
//  sipo.v2d.tot.xmin:= 0.0; 
//  sipo.v2d.tot.ymin:= -10.0; 
//  sipo.v2d.tot.xmax:= G.scene.r.efra; 
//  sipo.v2d.tot.ymax:= 10.0; 
//  sipo.v2d.cur:= sipo.v2d.tot; 
//  sipo.v2d.min[0]:= 0.$1; 
//  sipo.v2d.min[1]:= 0.$1; 
//  sipo.v2d.max[0]:= 15000.0; 
//  sipo.v2d.max[1]:= 10000.0; 
//  sipo.v2d.scroll:= L_SCROLL+B_SCROLL; 
//  sipo.v2d.keeptot:= 0; 
//  sipo.blocktype:= ID_OB; 
//  (* sipo space loopt van (0,-?) tot (??,?) *)
//end;

(* ******************** SPACE: INFO ********************** *)

procedure drawinfospace;
var
fac: single;
naam: array [0..Pred(20)] of char;
str: pchar;
begin

//  procedure reset_autosave; 



  glClearColor(0.5,0.5,0.5,0.0);
  glClear(GL_COLOR_BUFFER_BIT);
//  fac:= ( {float(}curarea.winx) div 1280.0; 
//  ortho2(0.0,1280.0,0.0,curarea.winy div fac); 
//  sprintf(naam,'win %d',curarea.win); 
//  DefButBlock(naam,curarea.win,G.font,35,2,2); 
//  SetButFunc(reset_autosave); 
//  SetButCol(11); 
//  DefButt(TOG or SHO or BIT or 0,0,'Auto Temp Save',45,32,126,20,@(U.flag),0,0,0,0,'Enables/Disables the automatic temp. file saving'); 
//  SetButCol(10); 
//  DefButt(TEX,0,'Dir:',45,10,127,20,U.tempdir,1.0,63.0,0,0,'The directory for temp. files'); 
//  DefButt(NUM or INT,0,'Time:',174,32,91,20,@(U.savetime),1.0,60.0,0,0,'The time in minutes to wait between temp. saves'); 
//  SetButCol(13); 
//  DefButt(BUT,B_LOADTEMP,'Load Temp',174,10,90,20,0,0,0,0,0,'Loads the most recently saved temp file'); 
//  SetButCol(10); 
//  DefButt(NUM or SHO,0,'Versions:',281,10,86,42,@U.versions,0.0,32.0,0,0,'The number of old versions to maintain when saving'); 
//  SetButCol(11); 
//  DefButt(TOG or SHO or BIT or 4,0,'Scene Global',389,32,86,20,@(U.flag),0,0,0,0,'Forces the current Scene to be displayed in all Screens'); 
//  DefButt(TOG or SHO or BIT or 5,0,'TrackBall',389,10,84,20,@(U.flag),0,0,0,0,'Switches between trackball and turntable view rotation methods (MiddleMouse)'); 
//  DefButt(TOG or SHO or BIT or 12,0,'2-Mouse',477,10,88,20,@(U.flag),0,0,0,0,'Maps ALT+LeftMouse to MiddleMouse button'); 
//  DefButt(TOG or SHO or BIT or 8,0,'Mat on Obj',567,9,78,20,@(U.flag),0,0,0,0,'Sets whether Material data is linked to Obj or ObjData'); 
//  DefButt(TOG or SHO or BIT or 9,B_U_CAPSLOCK,'NoCapsLock',478,32,87,20,@(U.flag),0,0,0,0,'Deactives the CapsLock button (only applies to text input)'); 
//  DefButt(TOG or SHO or BIT or 10,0,'Viewmove',569,32,76,20,@(U.flag),0,0,0,0,'Sets the default action for the middle mouse button'); 
//  DefButt(TOG or SHO or BIT or 11,0,'ToolTips',663,10,66,43,@(U.flag),0,0,0,0,'Enables/Disables tooltips'); 
//  DefButt(TOG or SHO or BIT or 1,0,'Grab Grid',743,32,146,20,@(U.flag),0,0,0,0,'Changes default step mode for grabbing'); 
//  DefButt(TOG or SHO or BIT or 2,0,'Rot Grid',815,10,74,20,@(U.flag),0,0,0,0,'Changes default step mode for rotation'); 
//  DefButt(TOG or SHO or BIT or 3,0,'Size Grid',743,10,68,20,@(U.flag),0,0,0,0,'Changes default step mode for scaling'); 
//  DefButt(TOG or SHO or BIT or 0,0,'Dupli Mesh',902,10,90,42,@(U.dupflag),0,0,0,0,'Causes Mesh data to be duplicated with CTRL+d'); 
//  DefButt(TOG or SHO or BIT or 1,0,'Curve',995,32,50,20,@(U.dupflag),0,0,0,0,'Causes Curve data to be duplicated with CTRL+d'); 
//  DefButt(TOG or SHO or BIT or 2,0,'Surf',995,10,50,20,@(U.dupflag),0,0,0,0,'Causes Surface data to be duplicated with CTRL+d'); 
//  DefButt(TOG or SHO or BIT or 3,0,'Text',1048,32,50,20,@(U.dupflag),0,0,0,0,'Causes Text data to be duplicated with CTRL+d'); 
//  DefButt(TOG or SHO or BIT or 4,0,'MBall',1048,10,50,20,@(U.dupflag),0,0,0,0,'Causes Metaball data to be duplicated with CTRL+d'); 
//  DefButt(TOG or SHO or BIT or 5,0,'Lamp',1101,32,50,20,@(U.dupflag),0,0,0,0,'Causes Lamp data to be duplicated with CTRL+d'); 
//  DefButt(TOG or SHO or BIT or 6,0,'Ipo',1101,10,50,20,@(U.dupflag),0,0,0,0,'Causes Ipo data to be duplicated with CTRL+d'); 
//  DefButt(TOG or SHO or BIT or 7,0,'Material',1153,32,70,20,@(U.dupflag),0,0,0,0,'Causes Material data to be duplicated with CTRL+d'); 
//  DefButt(TOG or SHO or BIT or 8,0,'Texture',1153,10,70,20,@(U.dupflag),0,0,0,0,'Causes Texture data to be duplicated with CTRL+d'); 
//  SetButCol(10); 
//  DefButt(TEX,0,'Fonts:',45,60,245,20,U.fontdir,1.0,63.0,0,0,'The default directory to search when loading fonts'); 
//  DefButt(TEX,0,'Render:',295,60,245,20,U.renderdir,1.0,63.0,0,0,'The default directory to choose for rendering'); 
//  DefButt(TEX,0,'Textures:',545,60,245,20,U.textudir,1.0,63.0,0,0,'The default directory to search when loading textures'); 
//  DefButt(TEX,0,'SeqPlugin:',795,60,245,20,U.plugseqdir,1.0,63.0,0,0,'The default directory to search when loading sequence plugins'); 
//  if TEST_C_KEY<>nil 
//  then
//  begin 
//    glRasterPos2i(545,84); 
//    cpack($0); 
//    fmprstr('C Key registered by: '); 
//    fmprstr(cdata+20); 
//  end;
end;

procedure winqreadinfospace(event: word;  val: smallint);
var
str: pchar;
begin
//  
//  if val<>nil 
//  then
//  begin 
//    case event of
//      LEFTMOUSE:
//      begin
//        FrontbufferButs(LongBool(1)); 
//        event:= DoButtons(); 
//        FrontbufferButs(LongBool(0)); 
//        if event<>nil 
//        then
//        begin 
//          if event=B_U_CAPSLOCK
//          then
//          disable_capslock(U.flag and NO_CAPSLOCK); 
//          else
//          do_global_buttons(event); 
//        end;
//      end;
//      MOUSEY:
//      begin
//        if U.flag and TOOLTIPS
//        then
//        begin 
//          str:= GetButTip(); 
//          if str<>nil 
//          then
//          begin 
//            set_info_text(str); 
//            allqueue(REDRAWINFO,1); 
//          end;
//        end;
//      end;
//      
//    end;{case?}
//  end;
end;

//(* ******************** SPACE: BUTS ********************** *)
//
//procedure drawbutspace; 
//(* buttons.c *)
//
//procedure changebutspace; 
//var
//v2d: pView2D; 
//xmin: float; 
//xmax: float; 
//ymin: float; 
//ymax: float; 
//begin
//  
//  
//  
//  
//  
//  if G.v2d=0
//  then
//  exit;
//  test_view2d(); 
//  ortho2(G.v2d.cur.xmin,G.v2d.cur.xmax,G.v2d.cur.ymin,G.v2d.cur.ymax); 
//end;
//
//procedure winqreadbutspace(event: ushort;  val: smallint); 
//var
//sa: pScrArea;
//sa3d: pScrArea;
//str: pchar; 
//dx: float; 
//dy: float; 
//doredraw: integer;
// 
//begin
//  
//  
//  
//  
//  
//  doredraw:=0; 
//  if curarea.win=0
//  then
//  exit;
//  if val<>nil 
//  then
//  begin 
//    case event of
//      LEFTMOUSE:
//      begin
//        FrontbufferButs(LongBool(1)); 
//        event:= DoButtons(); 
//        FrontbufferButs(LongBool(0)); 
//        do_blenderbuttons(event); 
//      end;
//      MIDDLEMOUSE:
//      begin
//        view2dmove(); 
//      end;
//      PADPLUSKEY,
//      (* in drawipo.c *)
//      PADMINUS:
//      begin
//        val:= SPACE_BUTS; 
//        winqreadipo(event,val); 
//      end;
//      RENDERPREVIEW:
//      begin
//        previewrender(); 
//      end;
//      MOUSEY:
//      begin
//        if U.flag and TOOLTIPS
//        then
//        begin 
//          str:= GetButTip(); 
//          if str<>nil 
//          then
//          begin 
//            set_info_text(str); 
//            allqueue(REDRAWINFO,1); 
//          end;
//        end;
//      end;
//      HOMEKEY:
//      begin
//        do_buts_buttons(B_BUTSHOME); 
//      end;
//      PAD0,
//      (* als er maar 1 3d win is: ook persptoetsen doen *)
//      PAD1,
//      PAD2,
//      PAD3,
//      PAD4,
//      PAD5,
//      PAD6,
//      PAD7,
//      PAD8,
//      PAD9,
//      PADENTER,
//      ZKEY:
//      begin
//        sa3d:= 0; 
//        sa:= G.curscreen.areabase.first; 
//        while sa
//        do
//        begin 
//          if sa^.spacetype=SPACE_VIEW3D
//          then
//          begin 
//            if sa3d<>nil 
//            then
//            exit;
//            sa3d:= sa; 
//          end;
//          sa:= sa^.next; 
//        end;
//        if sa3d<>nil 
//        then
//        begin 
//          sa:= curarea; 
//          areawinset(sa3d.win); 
//          if event=ZKEY
//          then
//          winqread3d(event,val); 
//          else
//          persptoetsen(event); 
//          addqueue(sa3d.win,REDRAW,1); 
//          addqueue(sa3d.headwin,REDRAW,1); 
//          areawinset(sa^.win); 
//        end;
//      end;
//    end;{case?}
//  end;
//  if doredraw<>nil 
//  then
//  addqueue(curarea.win,REDRAW,1); 
//end;
//
//procedure set_rects_butspace(buts: pSpaceButs); 
//begin
//  buts.v2d.tot.xmin:= 0.0; 
//  buts.v2d.tot.ymin:= 0.0; 
//  buts.v2d.tot.xmax:= 1279.0; 
//  buts.v2d.tot.ymax:= 228.0; 
//  buts.v2d.min[0]:= 256.0; 
//  buts.v2d.min[1]:= 42.0; 
//  buts.v2d.max[0]:= 1600.0; 
//  buts.v2d.max[1]:= 450.0; 
//  buts.v2d.minzoom:= 0.5; 
//  buts.v2d.maxzoom:= 1.41; 
//  buts.v2d.scroll:= 0; 
//  buts.v2d.keepaspect:= 1; 
//  buts.v2d.keepzoom:= 1; 
//  buts.v2d.keeptot:= 1; 
//  (* buts space loopt van (0,0) tot (1280, 228) *)
//end;
//
//procedure init_butspace(sa: pScrArea);
//var
//buts: pSpaceButs; 
//begin
//  
//  buts:= callocN(sizeof(SpaceButs),'initbuts'); 
//  addhead(@sa^.spacedata,buts); 
//  set_func_space(sa); 
//  buts.spacetype:= SPACE_BUTS; 
//  set_rects_butspace(buts); 
//  (* set_rects doet alleen defaults, zodat na het filezen de cur niet verandert *)
//  buts.v2d.cur:= buts.v2d.tot; 
//end;
//
//procedure extern_set_butspace(fkey: integer); 
//var
//sa: pScrArea;
//buts: pSpaceButs; (* als een ftoets ingedrukt: de dichtsbijzijnde buttonwindow wordt gezet *)
//begin
//  
//  if curarea.spacetype=SPACE_BUTS
//  then
//  sa:= curarea; 
//  else
//  begin 
//    (* area vinden *)
//    sa:= G.curscreen.areabase.first; 
//    while sa
//    do
//    begin 
//      if sa^.spacetype=SPACE_BUTS
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      sa:= sa^.next; 
//    end;
//  end;
//  if sa=0
//  then
//  exit;
//  if sa<>curarea
//  then
//  areawinset(sa^.win); 
//  buts:= sa^.spacedata.first; 
//  if fkey=F4KEY
//  then
//  buts.mainb:= BUTS_LAMP; 
//  else
//  if fkey=F5KEY
//  then
//  buts.mainb:= BUTS_MAT; 
//  else
//  if fkey=F6KEY
//  then
//  buts.mainb:= BUTS_TEX; 
//  else
//  if fkey=F7KEY
//  then
//  buts.mainb:= BUTS_ANIM; 
//  else
//  if fkey=F8KEY
//  then
//  buts.mainb:= BUTS_WORLD; 
//  else
//  if fkey=F9KEY
//  then
//  buts.mainb:= BUTS_EDIT; 
//  else
//  if fkey=F10KEY
//  then
//  buts.mainb:= BUTS_RENDER; 
//  addqueue(sa^.headwin,REDRAW,1); 
//  addqueue(sa^.win,REDRAW,1); 
//  preview_changed(sa^.win); 
//end;
//(* ******************** SPACE: SEQUENCE ********************** *)
//
//procedure drawseqspace; 
//
//procedure winqreadsequence(event: ushort;  val: smallint); 
//var
//sseq: pSpaceSeq; 
//ob: pObject; 
//dx: float; 
//dy: float; 
//doredraw: integer;
// 
//cfra: integer; 
//first: integer; 
//mval: array [0..Pred(2)] of smallint; 
//begin
//  
//  {last_seq: pSequence; }{<= !!!5 external variable}
//  
//  
//  
//  doredraw:=0; 
//  
//  
//  
//  if curarea.win=0
//  then
//  exit;
//  sseq:= curarea.spacedata.first; 
//  if val<>nil 
//  then
//  begin 
//    case event of
//      LEFTMOUSE:
//      begin
//        if sseq.mainb)or(view2dmove()=0
//        then
//        begin 
//          first:= 1; 
//          set_special_seq_update(1); 
//          repeat
//          begin 
//            getmouseco_areawin(mval); 
//            areamouseco_to_ipoco(mval, and dx, and dy); 
//            cfra:=  {integer(}dx; 
//            if cfra<1
//            then
//            cfra:= 1; (* else if(cfra> EFRA) cfra= EFRA; *)
//            if cfra<>CFRA)or(first
//            then
//            begin 
//              first:= 0; 
//              CFRA:= cfra; 
//              force_draw(); 
//            end;
//          end;
//          until not {0=}(get_mbut()@L_MOUSE);
//          set_special_seq_update(0); 
//          do_global_buttons(B_NEWFRAME); 
//        end;
//      end;
//      MIDDLEMOUSE:
//      begin
//        if sseq.mainb<>nil 
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        view2dmove(); 
//      end;
//      RIGHTMOUSE:
//      (* in drawipo.c *)
//      begin
//        if sseq.mainb<>nil 
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        mouse_select_seq(); 
//      end;
//      PADPLUSKEY:
//      begin
//        if sseq.mainb<>nil 
//        then
//        begin 
//          inc(sseq.zoom); 
//          if sseq.zoom>8
//          then
//          sseq.zoom:= 8; 
//        end;
//        else
//        begin 
//          if G.qual<>nil 
//          then
//          begin 
//            if G.qual and LR_SHIFTKEY
//            then
//            insert_gap(25,CFRA); 
//            else
//            if G.qual and LR_ALTKEY
//            then
//            insert_gap(250,CFRA); 
//            allqueue(REDRAWSEQ,0); 
//          end;
//          else
//          begin 
//            dx:= 0.1154*(G.v2d.cur.xmax-G.v2d.cur.xmin); 
//            G.v2d.cur.xmin:= G.v2d.cur.xmin + (dx); 
//            G.v2d.cur.xmax:= G.v2d.cur.xmax - (dx); 
//            test_view2d(); 
//          end;
//        end;
//        doredraw:= 1; 
//      end;
//      PADMINUS:
//      begin
//        if sseq.mainb<>nil 
//        then
//        begin 
//          dec(sseq.zoom); 
//          if sseq.zoom<1
//          then
//          sseq.zoom:= 1; 
//        end;
//        else
//        begin 
//          if G.qual<>nil 
//          then
//          begin 
//            if G.qual and LR_SHIFTKEY
//            then
//            no_gaps(); 
//          end;
//          else
//          begin 
//            dx:= 0.15*(G.v2d.cur.xmax-G.v2d.cur.xmin); 
//            G.v2d.cur.xmin:= G.v2d.cur.xmin - (dx); 
//            G.v2d.cur.xmax:= G.v2d.cur.xmax + (dx); 
//            test_view2d(); 
//          end;
//        end;
//        doredraw:= 1; 
//      end;
//      HOMEKEY:
//      begin
//        do_seq_buttons(B_SEQHOME); 
//      end;
//      PADPERIOD:
//      begin
//        if last_seq<>nil 
//        then
//        begin 
//          CFRA:= last_seq.startdisp; 
//          G.v2d.cur.xmin:= last_seq.startdisp-(last_seq.len div 20); 
//          G.v2d.cur.xmax:= last_seq.enddisp+(last_seq.len div 20); 
//          do_global_buttons(B_NEWFRAME); 
//        end;
//      end;
//      AKEY:
//      begin
//        if sseq.mainb<>nil 
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        if G.qual and LR_SHIFTKEY
//        then
//        begin 
//          add_sequence(); 
//        end;
//        else
//        swap_select_seq(); 
//      end;
//      BKEY:
//      begin
//        if sseq.mainb<>nil 
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        borderselect_seq(); 
//      end;
//      CKEY:
//      begin
//        if last_seq)and((last_seq.flag and (SEQ_LEFTSEL+SEQ_RIGHTSEL))
//        then
//        begin 
//          if last_seq.flag and SEQ_LEFTSEL
//          then
//          CFRA:= last_seq.startdisp; 
//          else
//          CFRA:= last_seq.enddisp-1; 
//          dx:= CFRA-(G.v2d.cur.xmax+G.v2d.cur.xmin) div 2; 
//          G.v2d.cur.xmax:= G.v2d.cur.xmax + (dx); 
//          G.v2d.cur.xmin:= G.v2d.cur.xmin + (dx); 
//          do_global_buttons(B_NEWFRAME); 
//        end;
//        else
//        change_sequence(); 
//      end;
//      DKEY:
//      begin
//        if sseq.mainb<>nil 
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        if G.qual and LR_SHIFTKEY
//        then
//        add_duplicate_seq(); 
//      end;
//      EKEY:
//      begin
//        write_edl(); 
//      end;
//      FKEY:
//      begin
//        set_filter_seq(); 
//      end;
//      GKEY:
//      begin
//        if sseq.mainb<>nil 
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        transform_seq('g'); 
//      end;
//      MKEY:
//      begin
//        if G.qual and LR_CTRLKEY
//        then
//        make_effect_movie(); 
//        else
//        if G.qual and LR_ALTKEY
//        then
//        un_meta(); 
//        else
//        make_meta(); 
//      end;
//      SKEY:
//      begin
//        if G.qual and LR_SHIFTKEY
//        then
//        seq_snapmenu(); 
//      end;
//      TKEY:
//      begin
//        touch_seq_files(); 
//      end;
//      XKEY:
//      begin
//        if sseq.mainb<>nil 
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        del_seq(); 
//      end;
//      
//    end;{case?}
//  end;
//  if doredraw<>nil 
//  then
//  addqueue(curarea.win,REDRAW,1); 
//end;
//
//procedure init_seqspace(sa: pScrArea);
//var
//sseq: pSpaceSeq; 
//begin
//  
//  sseq:= callocN(sizeof(SpaceSeq),'initseqspace'); 
//  addhead(@sa^.spacedata,sseq); 
//  set_func_space(sa); 
//  sseq.spacetype:= SPACE_SEQ; 
//  sseq.zoom:= 1; 
//  sseq.v2d.tot.xmin:= 0.0; 
//  sseq.v2d.tot.ymin:= 0.0; 
//  sseq.v2d.tot.xmax:= 250.0; 
//  sseq.v2d.tot.ymax:= 8.0; 
//  sseq.v2d.cur:= sseq.v2d.tot; 
//  sseq.v2d.min[0]:= 10.0; 
//  sseq.v2d.min[1]:= 4.0; 
//  sseq.v2d.max[0]:= 32000.0; 
//  sseq.v2d.max[1]:= MAXSEQ; 
//  sseq.v2d.minzoom:= 0.1; 
//  sseq.v2d.maxzoom:= 10.0; 
//  sseq.v2d.scroll:= L_SCROLL+B_SCROLL; 
//  sseq.v2d.keepaspect:= 0; 
//  sseq.v2d.keepzoom:= 0; 
//  sseq.v2d.keeptot:= 0; 
//  (* seq space loopt van (0,8) tot (250, 0) *)
//end;
//(* ******************** SPACE: FILE ********************** *)
//
//procedure drawfilespace; 
//
//procedure winqreadfilespace{!!!3 unknown typedef}; 
//
//procedure init_filespace(sa: pScrArea);
//var
//sfile: pSpaceFile; 
//begin
//  
//  sfile:= callocN(sizeof(SpaceFile),'initfilespace'); 
//  addhead(@sa^.spacedata,sfile); 
//  sfile^.dir[0]:= '/'; 
//  sfile^.type:= FILE_UNIX; 
//  set_func_space(sa); 
//  sfile^.spacetype:= SPACE_FILE; 
//end;
//(* ******************** SPACE: IMAGE ********************** *)
//
//procedure drawimagespace; 
//
//procedure winqreadimagespace(event: ushort;  val: smallint); 
//var
//sima: pSpaceImage; 
//begin
//  
//  if val=0
//  then
//  exit;
//  sima:= curarea.spacedata.first; 
//  case event of
//    LEFTMOUSE:
//    begin
//      if G.qual and LR_SHIFTKEY
//      then
//      mouseco_to_curtile(); 
//      else
//      gesture(); 
//    end;
//    MIDDLEMOUSE:
//    begin
//      image_viewmove(); 
//    end;
//    RIGHTMOUSE:
//    begin
//      mouse_select_sima(); 
//    end;
//    PADPLUSKEY:
//    begin
//      inc(sima.zoom); 
//      addqueue(curarea.win,REDRAW,1); 
//    end;
//    HOMEKEY:
//    begin
//      image_home(); 
//    end;
//    PADMINUS:
//    begin
//      if sima.zoom>1
//      then
//      begin 
//        dec(sima.zoom); 
//        addqueue(curarea.win,REDRAW,1); 
//      end;
//    end;
//    AKEY:
//    begin
//      select_swap_tface_uv(); 
//    end;
//    BKEY:
//    begin
//      borderselect_sima(); 
//    end;
//    GKEY:
//    begin
//      transform_tface_uv('g'); 
//    end;
//    NKEY:
//    begin
//      if G.qual and LR_CTRLKEY
//      then
//      replace_names_but(); 
//    end;
//    RKEY:
//    begin
//      transform_tface_uv('r'); 
//    end;
//    SKEY:
//    begin
//      transform_tface_uv('s'); 
//    end;
//    
//  end;{case?}
//end;
//
//procedure init_imagespace(sa: pScrArea);
//var
//sima: pSpaceImage; 
//begin
//  
//  sima:= callocN(sizeof(SpaceImage),'initimaspace'); 
//  addhead(@sa^.spacedata,sima); 
//  set_func_space(sa); 
//  sima.spacetype:= SPACE_IMAGE; 
//  sima.zoom:= 1; 
//end;
//(* ******************** SPACE: IMASEL ********************** *)
//
//procedure drawimasel; 
//
//procedure winqreadimasel{!!!3 unknown typedef}; 
//(* alles naar imasel.c *)
//(* ******************** SPACE: OOPS ********************** *)
//
//procedure drawoopsspace; 
//
//procedure winqreadoopsspace(event: ushort;  val: smallint); 
//var
//soops: pSpaceOops; 
//dx: float; 
//dy: float; 
//begin
//  
//  
//  
//  if val=0
//  then
//  exit;
//  soops:= curarea.spacedata.first; 
//  case event of
//    LEFTMOUSE:
//    begin
//      gesture(); 
//    end;
//    MIDDLEMOUSE:
//    begin
//      view2dmove(); 
//    end;
//    RIGHTMOUSE:
//    (* in drawipo.c *)
//    begin
//      mouse_select_oops(); 
//    end;
//    PADPLUSKEY:
//    begin
//      dx:= 0.1154*(G.v2d.cur.xmax-G.v2d.cur.xmin); 
//      dy:= 0.1154*(G.v2d.cur.ymax-G.v2d.cur.ymin); 
//      G.v2d.cur.xmin:= G.v2d.cur.xmin + (dx); 
//      G.v2d.cur.xmax:= G.v2d.cur.xmax - (dx); 
//      G.v2d.cur.ymin:= G.v2d.cur.ymin + (dy); 
//      G.v2d.cur.ymax:= G.v2d.cur.ymax - (dy); 
//      test_view2d(); 
//      addqueue(curarea.win,REDRAW,1); 
//    end;
//    PADMINUS:
//    begin
//      dx:= 0.15*(G.v2d.cur.xmax-G.v2d.cur.xmin); 
//      dy:= 0.15*(G.v2d.cur.ymax-G.v2d.cur.ymin); 
//      G.v2d.cur.xmin:= G.v2d.cur.xmin - (dx); 
//      G.v2d.cur.xmax:= G.v2d.cur.xmax + (dx); 
//      G.v2d.cur.ymin:= G.v2d.cur.ymin - (dy); 
//      G.v2d.cur.ymax:= G.v2d.cur.ymax + (dy); 
//      test_view2d(); 
//      addqueue(curarea.win,REDRAW,1); 
//    end;
//    HOMEKEY:
//    begin
//      do_oops_buttons(B_OOPSHOME); 
//    end;
//    AKEY:
//    begin
//      swap_select_all_oops(); 
//      addqueue(curarea.win,REDRAW,1); 
//    end;
//    BKEY:
//    begin
//      borderselect_oops(); 
//    end;
//    GKEY:
//    begin
//      transform_oops('g'); 
//    end;
//    LKEY:
//    begin
//      if G.qual and LR_SHIFTKEY
//      then
//      select_backlinked_oops(); 
//      else
//      select_linked_oops(); 
//    end;
//    SKEY:
//    begin
//      if G.qual and LR_ALTKEY
//      then
//      shrink_oops(); 
//      else
//      if G.qual and LR_SHIFTKEY
//      then
//      shuffle_oops(); 
//      else
//      transform_oops('s'); 
//    end;
//    ONEKEY:
//    begin
//      do_layer_toets(0); 
//    end;
//    TWOKEY:
//    begin
//      do_layer_toets(1); 
//    end;
//    THREEKEY:
//    begin
//      do_layer_toets(2); 
//    end;
//    FOURKEY:
//    begin
//      do_layer_toets(3); 
//    end;
//    FIVEKEY:
//    begin
//      do_layer_toets(4); 
//    end;
//    SIXKEY:
//    begin
//      do_layer_toets(5); 
//    end;
//    SEVENKEY:
//    begin
//      do_layer_toets(6); 
//    end;
//    EIGHTKEY:
//    begin
//      do_layer_toets(7); 
//    end;
//    NINEKEY:
//    begin
//      do_layer_toets(8); 
//    end;
//    ZEROKEY:
//    begin
//      do_layer_toets(9); 
//    end;
//    MINUSKEY:
//    begin
//      do_layer_toets(10); 
//    end;
//    EQUALKEY:
//    begin
//      do_layer_toets(11); 
//    end;
//    ACCENTGRAVEKEY:
//    begin
//      do_layer_toets(-1); 
//    end;
//    
//  end;{case?}
//end;
//
//procedure init_v2d_oops(v2d: pView2D); 
//begin
//  v2d.tot.xmin:= -28.0; 
//  v2d.tot.xmax:= 28.0; 
//  v2d.tot.ymin:= -28.0; 
//  v2d.tot.ymax:= 28.0; 
//  v2d.cur:= v2d.tot; 
//  v2d.min[0]:= 10.0; 
//  v2d.min[1]:= 4.0; 
//  v2d.max[0]:= 320.0; 
//  v2d.max[1]:= 320.0; 
//  v2d.minzoom:= 0.$1; 
//  v2d.maxzoom:= 2.0; 
//  v2d.scroll:= 0; 
//  v2d.keepaspect:= 1; 
//  v2d.keepzoom:= 0; 
//  v2d.keeptot:= 0; 
//  (* v2d->scroll= L_SCROLL+B_SCROLL; *)
//end;
//
//procedure init_oopsspace(sa: pScrArea);
//var
//soops: pSpaceOops; 
//begin
//  
//  soops:= callocN(sizeof(SpaceOops),'initoopsspace'); 
//  addhead(@sa^.spacedata,soops); 
//  set_func_space(sa); 
//  soops.visiflag:= OOPS_OB+OOPS_MA+OOPS_ME+OOPS_TE+OOPS_CU+OOPS_IP; 
//  soops.spacetype:= SPACE_OOPS; 
//  init_v2d_oops(@soops.v2d); 
//end;
//(* ******************** SPACE: PAINT ********************** *)
//(* ******************** SPACE: Text ********************** *)
//
//procedure drawtextspace; 
//
//procedure winqreadtextspace{!!!3 unknown typedef}; 
(* ******************** SPACE: ALGEMEEN ********************** *)

procedure newspace(sa: pScrArea;  type_: integer);
var
v3d: pView3D;
begin
  if type_>=0  then
  begin
    if sa^.spacetype<>type_    then
    begin
      sa^.spacetype:= type_;
      sa^.butspacetype:= type_;
      sa^.headbutofs:= 0;

      wich_cursor(sa);

      addqueue(sa^.headwin,CHANGED,1);
      addqueue(sa^.win,CHANGED,1);

      loadmatrix_win(matone,sa^.win);

      v3d:= sa^.spacedata.first; (* zoeken of er al een bestaat, we gebruiken v3d als algemeen voorbeeld *)
      while v3d <>nil       do
      begin
        if v3d^.spacetype=type_         then
        begin
          remlink(@sa^.spacedata,v3d);
          addhead(@sa^.spacedata,v3d);
          set_func_space(sa);
          exit;
        end;
        v3d:= v3d^.next;
      end;
      (* er bestaat er nog geen: nieuwe maken *)
      if type_=SPACE_EMPTY       then
      begin
        set_func_space(sa);
      end
      else
      if type_=SPACE_VIEW3D      then
      begin
        set_func_space(sa);
//        initview3d(sa);
      end
      else
//      if type_=SPACE_IPO       then
//      begin 
//        initipo(sa); 
//      end
//      else
      if type_=SPACE_INFO       then
      begin
        set_func_space(sa);
      end
//      else
//      if type_=SPACE_BUTS      then
//      begin 
//        init_butspace(sa); 
//      end
//      else
//      if type_=SPACE_FILE       then
//      begin 
//        init_filespace(sa); 
//      end
//      else
//      if type_=SPACE_SEQ      then
//      begin 
//        init_seqspace(sa); 
//      end
//      else
//      if type_=SPACE_IMAGE      then
//      begin 
//        init_imagespace(sa); 
//      end
//      else
//      if type_=SPACE_IMASEL       then
//      begin 
//        init_imaselspace(sa); 
//      end
//      else
//      if type_=SPACE_OOPS      then
//      begin 
//        init_oopsspace(sa); 
//      end
//      else
//      if type_=SPACE_PAINT      then
//      begin 
//      end
//      else
//      if type_=SPACE_TEXT       then
//      begin 
//        init_textspace(sa); 
//      end;
    end;
  end;
end;

procedure freespacelist(lb: pListBase);
var
sfile: pSpaceFile; (* voorbeeld *)
buts: pSpaceButs;
si: pSpaceIpo;
vd: pView3D;
begin
  sfile:= lb^.first;
  while sfile  <> nil  do
  begin
    if sfile^.spacetype=SPACE_FILE    then
    begin
      if sfile^.libfiledata<>nil       then
      freeN(sfile^.libfiledata);
    end
    else
    if sfile^.spacetype=SPACE_BUTS     then
    begin
      buts:= pSpaceButs(sfile);
      if buts^.rect<>nil then
      freeN(buts^.rect);
      if G.buts=buts       then
      G.buts:= nil;
    end
    else
    if sfile^.spacetype=SPACE_IPO     then
    begin
      si:= pSpaceIpo(sfile);
      if si^.editipo<>nil      then
      freeN(si^.editipo);
      free_ipokey(@si^.ipokey);
      if G.sipo=si       then
      G.sipo:= nil;
    end ;
    //else
    //if sfile^.spacetype=SPACE_VIEW3D     then
    //begin
    //  vd:= pView3D(sfile);
    //  if vd^.bgpic<>nil      then
    //  begin
    //    if vd^.bgpic.rect<>nil        then
    //    freeN(vd^.bgpic.rect);
    //    if vd^.bgpic.ima<>nil        then
    //    dec(vd^.bgpic.ima.id.us);
    //    freeN(vd^.bgpic);
    //  end;
    //  if vd^.localvd<>nil      then
    //  freeN(vd^.localvd);
    //  if G.vd=vd       then
    //  G.vd:= 0;
    //end;
    //else
    //if sfile^.spacetype=SPACE_OOPS     then
    //begin
    //  free_oopspace(sfile);
    //end;
    //else
    //if sfile^.spacetype=SPACE_IMASEL
    //then
    //begin
    //  free_imasel(sfile);
    //end;
    //else
    //if sfile^.spacetype=SPACE_PAINT    then
    //begin
    //end;
    //else
    //if sfile^.spacetype=SPACE_TEXT     then
    //begin
    //  free_textspace(sfile);
    //end;
    sfile:= sfile^.next;
  end;
  freelistN(lb);
end;

procedure duplicatespacelist(lb1, lb2: pListBase);
var
sfile: pSpaceFile; (* voorbeeld *)
buts: pSpaceButs;
si: pSpaceIpo;
vd: pView3D;
//so: pSpaceOops;
begin
  duplicatelist(lb1,lb2);

  (* lb1 is kopie van lb2, van lb2 geven we de filelist vrij *)

  sfile:= lb2^.first;
  while sfile<>nil do
  begin
//    if sfile^.spacetype=SPACE_FILE    then
//    begin 
//      sfile^.libfiledata:= 0; 
//      sfile^.filelist:= 0; 
//    end
//    else
//    if sfile^.spacetype=SPACE_OOPS     then
//    begin 
//      so:= {pSpaceOops(}sfile; 
//      so.oops.first:= so.oops.last:=0; 
//    end
//    else
//    if sfile^.spacetype=SPACE_IMASEL    then
//    begin 
//      check_imasel_copy(sfile); 
//    end
//    else
//    if sfile^.spacetype=SPACE_TEXT    then
//    begin 
//      check_text_copy(sfile); 
//    end
//    else
//    if sfile^.spacetype=SPACE_PAINT     then
//    begin 
//    end;
    sfile:= sfile^.next;
  end;

  sfile:= lb1^.first;
  while sfile  <>nil  do
  begin
//    if sfile^.spacetype=SPACE_BUTS    then
//    begin 
//      buts:=  {pSpaceButs(}sfile; 
//      buts.rect:= 0; 
//    end
//    else
//    if sfile^.spacetype=SPACE_IPO     then
//    begin 
//      si:=  {pSpaceIpo(}sfile; 
//      si.editipo:= 0; 
//      si.ipokey.first:= si.ipokey.last:=0; 
//    end
//    else
//    if sfile^.spacetype=SPACE_VIEW3D    then
//    begin 
//      vd:=  {pView3D(}sfile; 
//      if vd.bgpic<>nil       then
//      begin 
//        vd.bgpic:= dupallocN(vd.bgpic); 
//        vd.bgpic.rect:= 0; 
//        if vd.bgpic.ima<>nil         then
//        inc(vd.bgpic.ima.id.us); 
//      end;
//    end;
    sfile:= sfile^.next;
  end;

  (* nog een keer: van oude View3D de localview restoren (ivm full) *)
  vd:= lb2^.first;
  while vd <>nil  do
  begin
//    if vd.spacetype=SPACE_VIEW3D    then
//    begin 
//      if vd.localvd<>nil      then
//      begin 
//        restore_localviewdata(vd); 
//        vd.localvd:= 0; 
//        vd.localview:= 0; 
//        vd.lay:= vd.lay and ($FFFFFF); 
//      end;
//    end;
    vd:= vd^.next;
  end;
end;

procedure set_func_space(sa: pScrArea);
var
sseq: pSpaceSeq;
buts: pSpaceButs; (* ook na file inlezen: terugzetten functie pointers *)

begin
  (* default *)
  sa^.windraw:= nil;
  sa^.winchange:= nil;
  sa^.winqread:= nil;
  sa^.headdraw:= @defheaddraw;
  sa^.headchange:= nil;
  sa^.headqread:= nil;
  case sa^.spacetype of
//    SPACE_VIEW3D:
//    begin
//      sa^.windraw:= drawview3d; 
//      sa^.winchange:= changeview3d; 
//      sa^.winqread:= (void(* )())winqread3d; 
//      sa^.headqread:= (void(* )())winqread3d; 
//    end;
//    SPACE_IPO:
//    begin
//      sa^.windraw:= drawipo; 
//      sa^.winchange:= changeview2d; 
//      sa^.winqread:= (void(* )())winqreadipo; 
//      sa^.headqread:= (void(* )())winqreadipo; 
//    end;
//    SPACE_BUTS:
//    begin
//      sa^.windraw:= drawbutspace; 
//      sa^.winchange:= changebutspace; 
//      sa^.winqread:= (void(* )())winqreadbutspace; 
//      sa^.headqread:= (void(* )())winqreadbutspace; 
//    end;
//    SPACE_FILE:
//    begin
//      sa^.windraw:= drawfilespace; 
//      sa^.winqread:= (void(* )())winqreadfilespace; 
//      sa^.headqread:= (void(* )())winqreadfilespace; 
//    end;
    SPACE_INFO:
    begin
      sa^.windraw:= @drawinfospace;
      sa^.winqread:= @winqreadinfospace;
    end;
//    SPACE_SEQ:
//    begin
//      sa^.windraw:= drawseqspace; 
//      sa^.winchange:= changeview2d; 
//      sa^.winqread:= (void(* )())winqreadsequence; 
//      sseq:= sa^.spacedata.first; 
//      sseq.v2d.keeptot:= 0; 
//    end;
//    SPACE_IMAGE:
//    (* voor oude files *)
//    begin
//      sa^.windraw:= drawimagespace; 
//      sa^.winqread:= (void(* )())winqreadimagespace; 
//    end;
//    SPACE_IMASEL:
//    begin
//      sa^.windraw:= drawimasel; 
//      sa^.winqread:= (void(* )())winqreadimasel; 
//    end;
//    SPACE_OOPS:
//    begin
//      sa^.windraw:= drawoopsspace; 
//      sa^.winchange:= changeview2d; 
//      sa^.winqread:= (void(* )())winqreadoopsspace; 
//      sa^.headqread:= (void(* )())winqreadoopsspace; 
//    end;
    SPACE_PAINT:
    begin
    end;
//    SPACE_TEXT:
//    begin
//      sa^.windraw:= drawtextspace; 
//      sa^.winqread:= (void(* )())winqreadtextspace; 
//    end;
  end;
end;
//(* wordt overal aangeroepen *)
//
//procedure allqueue(event: ushort;  val: smallint); 
//var
//sa: pScrArea;
//v3d: pView3D; 
//buts: pSpaceButs; 
//si: pSpaceIpo; 
//sfile: pSpaceFile; 
//si: pSpaceIpo; 
//begin
//  
//  
//  
//  
//  
//  sa:= G.curscreen.areabase.first; 
//  while sa
//  do
//  begin 
//    if event=REDRAWALL
//    then
//    begin 
//      addqueue(sa^.win,REDRAW,1); 
//      addqueue(sa^.headwin,REDRAW,1); 
//    end;
//    else
//    if sa^.win<>val
//    then
//    begin 
//      case event of
//        REDRAWHEADERS:
//        begin
//          addqueue(sa^.headwin,REDRAW,1); 
//        end;
//        REDRAWVIEW3D:
//        begin
//          if sa^.spacetype=SPACE_VIEW3D
//          then
//          begin 
//            addqueue(sa^.win,REDRAW,1); 
//            if val<>nil 
//            then
//            addqueue(sa^.headwin,REDRAW,1); 
//          end;
//        end;
//        REDRAWVIEW3D_Z:
//        begin
//          if sa^.spacetype=SPACE_VIEW3D
//          then
//          begin 
//            v3d:= sa^.spacedata.first; 
//            if v3d^.drawtype=OB_SOLID
//            then
//            begin 
//              addqueue(sa^.win,REDRAW,1); 
//              if val<>nil 
//              then
//              addqueue(sa^.headwin,REDRAW,1); 
//            end;
//          end;
//        end;
//        REDRAWVIEWCAM:
//        begin
//          if sa^.spacetype=SPACE_VIEW3D
//          then
//          begin 
//            v3d:= sa^.spacedata.first; 
//            if v3d^.persp>1
//            then
//            addqueue(sa^.win,REDRAW,1); 
//          end;
//        end;
//        REDRAWINFO:
//        begin
//          if sa^.spacetype=SPACE_INFO
//          then
//          begin 
//            addqueue(sa^.headwin,REDRAW,1); 
//          end;
//        end;
//        REDRAWIMAGE:
//        begin
//          if sa^.spacetype=SPACE_IMAGE
//          then
//          begin 
//            addqueue(sa^.win,REDRAW,1); 
//            addqueue(sa^.headwin,REDRAW,1); 
//          end;
//        end;
//        REDRAWIPO:
//        begin
//          if sa^.spacetype=SPACE_IPO
//          then
//          begin 
//            
//            addqueue(sa^.headwin,REDRAW,1); 
//            addqueue(sa^.win,REDRAW,1); 
//            if val<>nil 
//            then
//            begin 
//              si:= sa^.spacedata.first; 
//              si.blocktype:= val; 
//            end;
//          end;
//          else
//          if sa^.spacetype=SPACE_OOPS
//          then
//          begin 
//            addqueue(sa^.win,REDRAW,1); 
//          end;
//        end;
//        REDRAWBUTSALL:
//        begin
//          if sa^.spacetype=SPACE_BUTS
//          then
//          begin 
//            addqueue(sa^.win,REDRAW,1); 
//            addqueue(sa^.headwin,REDRAW,1); 
//          end;
//        end;
//        REDRAWBUTSHEAD:
//        begin
//          if sa^.spacetype=SPACE_BUTS
//          then
//          begin 
//            addqueue(sa^.headwin,REDRAW,1); 
//          end;
//        end;
//        REDRAWBUTSVIEW:
//        begin
//          if sa^.spacetype=SPACE_BUTS
//          then
//          begin 
//            buts:= sa^.spacedata.first; 
//            if buts.mainb=BUTS_VIEW
//            then
//            begin 
//              addqueue(sa^.win,REDRAW,1); 
//              addqueue(sa^.headwin,REDRAW,1); 
//            end;
//          end;
//        end;
//        REDRAWBUTSLAMP:
//        begin
//          if sa^.spacetype=SPACE_BUTS
//          then
//          begin 
//            buts:= sa^.spacedata.first; 
//            if buts.mainb=BUTS_LAMP
//            then
//            begin 
//              addqueue(sa^.win,REDRAW,1); 
//              addqueue(sa^.headwin,REDRAW,1); 
//            end;
//          end;
//        end;
//        REDRAWBUTSMAT:
//        begin
//          if sa^.spacetype=SPACE_BUTS
//          then
//          begin 
//            buts:= sa^.spacedata.first; 
//            if buts.mainb=BUTS_MAT
//            then
//            begin 
//              addqueue(sa^.win,REDRAW,1); 
//              addqueue(sa^.headwin,REDRAW,1); 
//            end;
//          end;
//        end;
//        REDRAWBUTSTEX:
//        begin
//          if sa^.spacetype=SPACE_BUTS
//          then
//          begin 
//            buts:= sa^.spacedata.first; 
//            if buts.mainb=BUTS_TEX
//            then
//            begin 
//              addqueue(sa^.win,REDRAW,1); 
//              addqueue(sa^.headwin,REDRAW,1); 
//            end;
//          end;
//        end;
//        REDRAWBUTSANIM:
//        begin
//          if sa^.spacetype=SPACE_BUTS
//          then
//          begin 
//            buts:= sa^.spacedata.first; 
//            if buts.mainb=BUTS_ANIM
//            then
//            begin 
//              addqueue(sa^.win,REDRAW,1); 
//              addqueue(sa^.headwin,REDRAW,1); 
//            end;
//          end;
//        end;
//        REDRAWBUTSWORLD:
//        begin
//          if sa^.spacetype=SPACE_BUTS
//          then
//          begin 
//            buts:= sa^.spacedata.first; 
//            if buts.mainb=BUTS_WORLD
//            then
//            begin 
//              addqueue(sa^.win,REDRAW,1); 
//              addqueue(sa^.headwin,REDRAW,1); 
//            end;
//          end;
//        end;
//        REDRAWBUTSRENDER:
//        begin
//          if sa^.spacetype=SPACE_BUTS
//          then
//          begin 
//            buts:= sa^.spacedata.first; 
//            if buts.mainb=BUTS_RENDER
//            then
//            begin 
//              addqueue(sa^.win,REDRAW,1); 
//              addqueue(sa^.headwin,REDRAW,1); 
//            end;
//          end;
//        end;
//        REDRAWBUTSEDIT:
//        begin
//          if sa^.spacetype=SPACE_BUTS
//          then
//          begin 
//            buts:= sa^.spacedata.first; 
//            if buts.mainb=BUTS_EDIT
//            then
//            begin 
//              addqueue(sa^.win,REDRAW,1); 
//              addqueue(sa^.headwin,REDRAW,1); 
//            end;
//          end;
//        end;
//        REDRAWBUTSGAME:
//        begin
//          if sa^.spacetype=SPACE_BUTS
//          then
//          begin 
//            buts:= sa^.spacedata.first; 
//            
//            function ELEM{!!!3 unknown typedef}: if; 
//            begin
//              addqueue(sa^.win,REDRAW,1); 
//              addqueue(sa^.headwin,REDRAW,1); 
//            end
//          end;
//        end;
//        REDRAWBUTSRADIO:
//        begin
//          if sa^.spacetype=SPACE_BUTS
//          then
//          begin 
//            buts:= sa^.spacedata.first; 
//            if buts.mainb=BUTS_RADIO
//            then
//            begin 
//              addqueue(sa^.win,REDRAW,1); 
//              addqueue(sa^.headwin,REDRAW,1); 
//            end;
//          end;
//        end;
//        REDRAWBUTSSCRIPT:
//        begin
//          if sa^.spacetype=SPACE_BUTS
//          then
//          begin 
//            buts:= sa^.spacedata.first; 
//            if buts.mainb=BUTS_SCRIPT
//            then
//            begin 
//              addqueue(sa^.win,REDRAW,1); 
//              addqueue(sa^.headwin,REDRAW,1); 
//            end;
//          end;
//        end;
//        REDRAWDATASELECT:
//        begin
//          if sa^.spacetype=SPACE_FILE
//          then
//          begin 
//            sfile:= sa^.spacedata.first; 
//            if sfile^.type=FILE_MAIN
//            then
//            begin 
//              freefilelist(sfile); 
//              addqueue(sa^.win,REDRAW,1); 
//            end;
//          end;
//          else
//          if sa^.spacetype=SPACE_OOPS
//          then
//          begin 
//            addqueue(sa^.win,REDRAW,1); 
//          end;
//        end;
//        REDRAWSEQ:
//        begin
//          if sa^.spacetype=SPACE_SEQ
//          then
//          begin 
//            addqueue(sa^.win,CHANGED,1); 
//            addqueue(sa^.win,REDRAW,1); 
//            addqueue(sa^.headwin,REDRAW,1); 
//          end;
//        end;
//        REDRAWOOPS:
//        begin
//          if sa^.spacetype=SPACE_OOPS
//          then
//          begin 
//            addqueue(sa^.win,REDRAW,1); 
//          end;
//        end;
//        REDRAWPAINT:
//        begin
//          if sa^.spacetype=SPACE_PAINT
//          then
//          begin 
//            addqueue(sa^.win,REDRAW,1); 
//          end;
//        end;
//        REDRAWTEXT:
//        begin
//          if sa^.spacetype=SPACE_TEXT
//          then
//          begin 
//            addqueue(sa^.win,REDRAW,1); 
//          end;
//        end;
//        
//      end;{case?}
//    end;
//    sa:= sa^.next; 
//  end;
//end;
//
//procedure allspace(event: ushort;  val: smallint); 
//var
//sc: pbScreen; 
//sa: pScrArea;
//v3d: pView3D; 
//buts: pSpaceButs; 
//si: pSpaceIpo;
// 
//so: pSpaceOops;
// 
//begin
//  
//  
//  
//  
//  sc:= G.main.screen.first; 
//  while sc
//  do
//  begin 
//    sa:= sc^.areabase.first;
//    while sa
//    do
//    begin 
//      v3d:= sa^.spacedata.first; 
//      while v3d
//      do
//      begin 
//        case event of
//          REMAKEIPO:
//          begin
//            if v3d^.spacetype=SPACE_IPO
//            then
//            begin 
//              si:= {pSpaceIpo(}v3d; 
//              if si.editipo<>nil 
//              then
//              freeN(si.editipo); 
//              si.editipo:= 0; 
//              free_ipokey(@si.ipokey); 
//            end;
//          end;
//          OOPS_TEST:
//          begin
//            if v3d^.spacetype=SPACE_OOPS
//            then
//            begin 
//              so:= {pSpaceOops(}v3d; 
//              so.flag:= so.flag or (SO_TESTBLOCKS); 
//            end;
//          end;
//          
//        end;{case?}
//        v3d:= v3d^.next; 
//      end;
//      sa:= sa^.next; 
//    end;
//    sc:= sc^.id.next;
//  end;
//end;
//
//procedure force_draw; 
//var
//tempsa: pScrArea;
//sa: pScrArea;
//begin(* alle area's die (ongeveer) zelfde laten zien als curarea *)
//  
//  
//  curarea.windraw(); 
//  tempsa:= curarea; 
//  sa:= G.curscreen.areabase.first; 
//  while sa
//  do
//  begin 
//    if sa<>tempsa)and(sa^.spacetype=tempsa^.spacetype
//    then
//    begin 
//      if sa^.spacetype=SPACE_VIEW3D
//      then
//      begin 
//        if ( {pView3D(}sa^.spacedata.first).lay and ( {pView3D(}tempsa^.spacedata.first).lay
//        then
//        begin 
//          areawinset(sa^.win); 
//          sa^.windraw(); 
//        end;
//      end;
//      else
//      if sa^.spacetype=SPACE_IPO
//      then
//      begin 
//        areawinset(sa^.win); 
//        sa^.windraw(); 
//      end;
//      else
//      if sa^.spacetype=SPACE_SEQ
//      then
//      begin 
//        areawinset(sa^.win); 
//        sa^.windraw(); 
//      end;
//    end;
//    sa:= sa^.next; 
//  end;
//  if curarea<>tempsa
//  then
//  areawinset(tempsa^.win); 
//  screen_swapbuffers(); 
//end;
//
//procedure force_draw_plus(type: integer); 
//var
//tempsa: pScrArea;
//sa: pScrArea;
//begin(* alle area's die (ongeveer) zelfde laten zien als curarea EN areas van 'type' *)
//  
//  
//  curarea.windraw(); 
//  tempsa:= curarea; 
//  sa:= G.curscreen.areabase.first; 
//  while sa
//  do
//  begin 
//    if sa<>tempsa)and((sa^.spacetype=tempsa^.spacetype)or(sa^.spacetype=type)
//    then
//    begin 
//      if sa^.spacetype=SPACE_VIEW3D
//      then
//      begin 
//        areawinset(sa^.win); 
//        sa^.windraw(); 
//      end;
//      else
//      if sa^.spacetype=SPACE_IPO
//      then
//      begin 
//        areawinset(sa^.win); 
//        sa^.windraw(); 
//      end;
//      else
//      if sa^.spacetype=SPACE_SEQ
//      then
//      begin 
//        areawinset(sa^.win); 
//        sa^.windraw(); 
//      end;
//      else
//      if sa^.spacetype=SPACE_BUTS
//      then
//      begin 
//        areawinset(sa^.win); 
//        sa^.windraw(); 
//      end;
//    end;
//    sa:= sa^.next; 
//  end;
//  if curarea<>tempsa
//  then
//  areawinset(tempsa^.win); 
//  screen_swapbuffers(); 
//end;
//
//procedure force_draw_all; 
//var
//tempsa: pScrArea;
//sa: pScrArea;
//begin(* alle area's die (ongeveer) zelfde laten zien als curarea EN areas van 'type' *)
//  
//  
//  drawscreen(); 
//  tempsa:= curarea; 
//  sa:= G.curscreen.areabase.first; 
//  while sa
//  do
//  begin 
//    if sa^.headwin<>nil 
//    then
//    begin 
//      areawinset(sa^.headwin); 
//      defheadchange(); 
//      sa^.headdraw(); 
//    end;
//    if sa^.win<>nil 
//    then
//    begin 
//      areawinset(sa^.win); 
//      sa^.windraw(); 
//    end;
//    sa:= sa^.next; 
//  end;
//  if curarea<>tempsa
//  then
//  areawinset(tempsa^.win); 
//  screen_swapbuffers(); 
//end;
//
end.
