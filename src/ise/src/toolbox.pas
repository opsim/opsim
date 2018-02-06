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

(*  toolbox.c GRAPHICS
 *
 *  feb 93
 *  dec 93 voor blender
 *)

unit toolbox;

interface

procedure error(str: pchar);

implementation

uses
GL, GLut,
cfuncs,
blenglob, util, graphics, Button, mywindow, screen, mydevice;

(*   ********  NOTES  *********** *****************************
 
 - Toolbox items zelf invullen
 - de colormap kleuren staan met comments in de bgntoolbox()
 - de funktie extern_qread eventueel vervangen
 - let op de benaming van bijzondere toetsen (NumL etc) 
 - meelinken: Button.c,  ivm rawcodes
 
 ***************************** *****************************
*)

var
//tbwinmat: array [0..Pred(4),0..Pred(4)] of float; 
//tbprojmat: array [0..Pred(4),0..Pred(4)] of float; 
//tbx1: integer = 0; 
//tbx2: integer = 0; 
//tby1: integer = 0; 
//tby2: integer = 0; 
tbfontyofs: integer = 0;
//tbmain: integer = 0; 
//tbmemx: integer = TBOXX div 2; 
//tbmemy: integer = (TBOXEL-0.5)*TBOXH; 
tboldwin: integer = 0;
//addmode: integer = 0; 
//tbpat: array [0..Pred(16)] of ushort; 
oldcursor: smallint = 0;
//oldmap: array [0..Pred(4),0..Pred(3)] of smallint; 
//(* variabelen per item *)
//tbstr: pchar; 
//tbstr1: pchar; 
//void({*}tbfunc^); 
//(); 
//tbval: integer; 
//var {was static}
//p50: array [0..] of GLubyte = ($aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,$aa,$aa,$aa,$aa,$55,$55,$55,$55,); 

//(* *********** PC PATCH ************* *)

//procedure ColorFunc(i: integer);
//begin
//  if i=TBOXBLACK then
//  glColor3ub(0,0,0); 
//  else
//  if i=TBOXWHITE then
//  glColor3ub(240,240,240); 
//  else
//  if i=TBOXGREY then
//  glColor3ub(160,160,160); 
//  else
//  glColor3ub(0,0,0); 
//end;

//{$ifdef glIndexi}
//{$undef glIndexi}
//{$endif}

//function glIndexi(i: integer): integer; {<= !!!6 unknown macro}

(* *********** ANSI COMPATIBLE **************** *)

procedure mygetcursor(index: psmallint);
begin
  index^:=glutGet(GLUT_WINDOW_CURSOR);
end;

(* ********************* TOOLBOX ITEMS ************************* *)

//procedure tbox_setinfo(x: integer;  y: integer); 
//begin
//  tbstr:= 0; 
//  tbstr1:= 0; 
//  tbfunc:= 0; 
//  tbval:= 0; (* afhankelijk van tbmain worden vars gezet *)

//  (* HOOFDMENU ITEMS LINKS *)
//  if x=0 then
//  begin 

//    case y of
//      0:
//      begin
//        if addmode=OB_MESH then
//        tbstr:= '  MESH'; 
//        else
//        if addmode=OB_CURVE then
//        tbstr:= '  CURVE'; 
//        else
//        if addmode=OB_SURF then
//        tbstr:= '  SURF'; 
//        else
//        tbstr:= 'ADD'; 
//      end;
//      1:
//      begin
//        tbstr:= 'VIEW'; 
//      end;
//      2:
//      begin
//        tbstr:= 'EDIT'; 
//      end;
//      3:
//      begin
//        tbstr:= 'OBJECT'; 
//      end;
//      4:
//      begin
//        tbstr:= 'OBJECT'; 
//      end;
//      5:
//      begin
//        tbstr:= 'MESH'; 
//      end;
//      6:
//      begin
//        tbstr:= 'CURVE'; 
//      end;
//      7:
//      begin
//        tbstr:= 'KEY'; 
//      end;
//      8:
//      begin
//        tbstr:= 'RENDER'; 
//      end;
//      9:
//      begin
//        tbstr:= 'FILE'; 
//      end;

//    end;{case?}
//  end;
//  else
//  (* TOPICS *)
//  begin 
//    (*
//      if(G.obedit) {
//       addmode= G.obedit->type;
//      }

//    *)
//    (* VIEW TOPICS *)
//    if tbmain=1 then
//    begin 
//      case y of
//        0:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        1:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        2:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        3:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        4:
//        begin
//          tbstr:= 'Centre'; 
//          tbstr1:= 'c'; 
//        end;
//        5:
//        begin
//          tbstr:= 'Home'; 
//          tbstr1:= 'C'; 
//        end;
//        6:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        7:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        8:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        9:
//        begin
//          tbstr:= 'Zbuffer'; 
//          tbstr1:= 'z'; 
//        end;
//        10:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        11:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;

//      end;{case?}
//    end;
//    else

//    (* EDIT TOPICS *)
//    if tbmain=2 then
//    begin 
//      case y of
//        0:
//        begin
//          tbstr:= 'Grabber'; 
//          tbstr1:= 'g'; 
//        end;
//        1:
//        begin
//          tbstr:= 'Rotate'; 
//          tbstr1:= 'r'; 
//        end;
//        2:
//        begin
//          tbstr:= 'Size'; 
//          tbstr1:= 's'; 
//        end;
//        3:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        4:
//        begin
//          tbstr:= 'Shear'; 
//          tbstr1:= 'c|s'; 
//        end;
//        5:
//        begin
//          tbstr:= 'Warp/bend'; 
//          tbstr1:= 'W'; 
//        end;
//        6:
//        begin
//          tbstr:= 'Snapmenu'; 
//          tbstr1:= 'S'; 
//        end;
//        7:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        8:
//        begin
//          tbstr:= '(De)sel All'; 
//          tbstr1:= 'a'; 
//        end;
//        9:
//        begin
//          tbstr:= 'BorderSelect'; 
//          tbstr1:= 'b'; 
//        end;
//        10:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        11:
//        begin
//          tbstr:= 'Quit'; 
//          tbstr1:= 'q'; 
//        end;

//      end;{case?}
//    end;
//    else

//    (* ADD TOPICS *)
//    if tbmain=0 then
//    begin 

//      if addmode=0 then
//      begin 
//        case y of
//          0:
//          begin
//            tbstr:= 'Mesh'; 
//            tbstr1:= '>>'; 
//            tbval:= OB_MESH; 
//          end;
//          1:
//          begin
//            tbstr:= 'Curve'; 
//            tbstr1:= '>>'; 
//            tbval:= OB_CURVE; 
//          end;
//          2:
//          begin
//            tbstr:= 'Surface'; 
//            tbstr1:= '>>'; 
//            tbval:= OB_SURF; 
//          end;
//          3:
//          begin
//            tbstr:= 'Text'; 
//            tbstr1:= ; 
//            tbval:= OB_FONT; 
//            tbfunc:= add_primitiveFont; 
//          end;
//          4:
//          begin
//            tbstr:= 'MetaBall'; 
//            tbstr1:= ; 
//            tbval:= OB_MBALL; 
//            tbfunc:= add_primitiveMball; 
//          end;
//          5:
//          begin
//            tbstr:= 'Empty'; 
//            tbstr1:= 'A'; 
//            tbval:= OB_EMPTY; 
//          end;
//          6:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//            tbval:= 0; 
//          end;
//          7:
//          begin
//            tbstr:= 'Camera'; 
//            tbstr1:= 'A'; 
//            tbval:= OB_CAMERA; 
//          end;
//          8:
//          begin
//            tbstr:= 'Lamp'; 
//            tbstr1:= 'A'; 
//            tbval:= OB_LAMP; 
//          end;
//          9:
//          begin
//            tbstr:= 'Ika'; 
//            tbstr1:= 'A'; 
//            tbval:= OB_IKA; 
//            {$ifdef FREE}
//          end;
//          10:
//          begin
//            tbstr:= ; 
//            tbstr1:= 'A'; 
//            tbval:= 0; 
//          end;
//          11:
//          begin
//            tbstr:= ; 
//            tbstr1:= 'A'; 
//            tbval:= 0; 
//            {$else}
//          end;
//          10:
//          begin
//            tbstr:= 'Sector'; 
//            tbstr1:= 'A'; 
//            tbval:= OB_SECTOR; 
//          end;
//          11:
//          begin
//            tbstr:= 'Life'; 
//            tbstr1:= 'A'; 
//            tbval:= OB_LIFE; 
//            {$endif}
//          end;
//          12:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//            tbval:= 0; 
//          end;
//          13:
//          begin
//            tbstr:= 'Lattice'; 
//            tbstr1:= 'A'; 
//            tbval:= OB_LATTICE; 
//          end;

//        end;{case?}
//        if tbstr1)and(tbstr1[0]='A' then
//        tbfunc:= (void(* )())add_object_draw; 
//      end;
//      else
//      if addmode=OB_MESH)or(addmode=OB_SECTOR)or(addmode=OB_LIFE then
//      begin 
//        case y of
//          0:
//          begin
//            tbstr:= '>Plane'; 
//            tbstr1:= 'A'; 
//            tbval:= 0; 
//          end;
//          1:
//          begin
//            tbstr:= '>Cube'; 
//            tbstr1:= 'A'; 
//            tbval:= 1; 
//          end;
//          2:
//          begin
//            tbstr:= '>Circle'; 
//            tbstr1:= 'A'; 
//            tbval:= 4; 
//          end;
//          3:
//          begin
//            tbstr:= '>UVsphere'; 
//            tbstr1:= 'A'; 
//            tbval:= 11; 
//          end;
//          4:
//          begin
//            tbstr:= '>Icosphere'; 
//            tbstr1:= 'A'; 
//            tbval:= 12; 
//          end;
//          5:
//          begin
//            tbstr:= '>Cylinder'; 
//            tbstr1:= 'A'; 
//            tbval:= 5; 
//          end;
//          6:
//          begin
//            tbstr:= '>Tube'; 
//            tbstr1:= 'A'; 
//            tbval:= 6; 
//          end;
//          7:
//          begin
//            tbstr:= '>Cone'; 
//            tbstr1:= 'A'; 
//            tbval:= 7; 
//          end;
//          8:
//          begin
//            tbstr:= '>'; 
//            tbstr1:= ; 
//          end;
//          9:
//          begin
//            tbstr:= '>Grid'; 
//            tbstr1:= 'A'; 
//            tbval:= 10; 
//          end;
//          10:
//          begin
//            tbstr:= '>'; 
//            tbstr1:= ; 
//          end;
//          11:
//          begin
//            tbstr:= '>Duplicate'; 
//            tbstr1:= 'D'; 
//          end;

//        end;{case?}
//        if tbstr1)and(tbstr1[0]='A' then
//        tbfunc:= add_primitiveMesh; 
//      end;
//      else
//      if addmode=OB_SURF then
//      begin 
//        case y of
//          0:
//          begin
//            tbstr:= '>Curve'; 
//            tbstr1:= 'A'; 
//            tbval:= 0; 
//          end;
//          1:
//          begin
//            tbstr:= '>Circle'; 
//            tbstr1:= 'A'; 
//            tbval:= 1; 
//          end;
//          2:
//          begin
//            tbstr:= '>Surface'; 
//            tbstr1:= 'A'; 
//            tbval:= 2; 
//          end;
//          3:
//          begin
//            tbstr:= '>Tube'; 
//            tbstr1:= 'A'; 
//            tbval:= 3; 
//          end;
//          4:
//          begin
//            tbstr:= '>Sphere'; 
//            tbstr1:= 'A'; 
//            tbval:= 4; 
//          end;
//          5:
//          begin
//            tbstr:= '>Donut'; 
//            tbstr1:= 'A'; 
//            tbval:= 5; 
//          end;
//          6:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          7:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          8:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          9:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          10:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          11:
//          begin
//            tbstr:= '>Duplicate'; 
//            tbstr1:= 'D'; 
//          end;

//        end;{case?}
//        if tbstr1)and(tbstr1[0]='A' then
//        tbfunc:= add_primitiveNurb; 
//      end;
//      else
//      if addmode=OB_CURVE then
//      begin 
//        case y of
//          0:
//          begin
//            tbstr:= '>Bezier Curve'; 
//            tbstr1:= 'A'; 
//            tbval:= 10; 
//          end;
//          1:
//          begin
//            tbstr:= '>Bezier Circle'; 
//            tbstr1:= 'A'; 
//            tbval:= 11; 
//          end;
//          2:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          3:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          4:
//          begin
//            tbstr:= '>Nurbs Curve'; 
//            tbstr1:= 'A'; 
//            tbval:= 40; 
//          end;
//          5:
//          begin
//            tbstr:= '>Nurbs Circle'; 
//            tbstr1:= 'A'; 
//            tbval:= 41; 
//          end;
//          6:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          7:
//          begin
//            tbstr:= '>Path'; 
//            tbstr1:= 'A'; 
//            tbval:= 46; 
//          end;
//          8:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          9:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          10:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          11:
//          begin
//            tbstr:= '>Duplicate'; 
//            tbstr1:= 'D'; 
//          end;

//        end;{case?}
//        if tbstr1)and(tbstr1[0]='A' then
//        tbfunc:= add_primitiveCurve; 
//      end;
//      else
//      if addmode=OB_MBALL then
//      begin 
//        case y of
//          0:
//          begin
//            tbstr:= 'Ball'; 
//            tbstr1:= 'A'; 
//            tbval:= 1; 
//          end;
//          1:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          2:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          3:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          4:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          5:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          6:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          7:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          8:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          9:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          10:
//          begin
//            tbstr:= ; 
//            tbstr1:= ; 
//          end;
//          11:
//          begin
//            tbstr:= 'Duplicate'; 
//            tbstr1:= 'D'; 
//          end;

//        end;{case?}
//        if tbstr1)and(tbstr1[0]='A' then
//        tbfunc:= add_primitiveMball; 
//      end;
//    end;
//    else
//    (* OB TOPICS 1 *)
//    if tbmain=3 then
//    begin 
//      case y of
//        0:
//        begin
//          tbstr:= 'Clear size'; 
//          tbstr1:= 'a|s'; 
//        end;
//        1:
//        begin
//          tbstr:= 'Clear rotation'; 
//          tbstr1:= 'a|r'; 
//        end;
//        2:
//        begin
//          tbstr:= 'Clear location'; 
//          tbstr1:= 'a|g'; 
//        end;
//        3:
//        begin
//          tbstr:= 'Clear origin'; 
//          tbstr1:= 'a|o'; 
//        end;
//        4:
//        begin
//          tbstr:= 'Make Parent'; 
//          tbstr1:= 'c|p'; 
//        end;
//        5:
//        begin
//          tbstr:= 'Clear Parent'; 
//          tbstr1:= 'a|p'; 
//        end;
//        6:
//        begin
//          tbstr:= 'Make Track'; 
//          tbstr1:= 'c|t'; 
//        end;
//        7:
//        begin
//          tbstr:= 'Clear Track'; 
//          tbstr1:= 'a|t'; 
//        end;
//        8:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        9:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        10:
//        begin
//          tbstr:= 'Image Displist'; 
//          tbstr1:= 'c|d'; 
//        end;
//        11:
//        begin
//          tbstr:= 'Image Aspect'; 
//          tbstr1:= 'a|v'; 
//        end;

//      end;{case?}
//    end;
//    else
//    (* OB TOPICS 2 *)
//    if tbmain=4 then
//    begin 
//      case y of
//        0:
//        begin
//          tbstr:= 'EditMode'; 
//          tbstr1:= 'Tab'; 
//        end;
//        1:
//        begin
//          tbstr:= 'Move To Layer'; 
//          tbstr1:= 'm'; 
//        end;
//        2:
//        begin
//          tbstr:= 'Delete'; 
//          tbstr1:= 'x'; 
//        end;
//        3:
//        begin
//          tbstr:= 'Delete All'; 
//          tbstr1:= 'c|x'; 
//        end;
//        4:
//        begin
//          tbstr:= 'Apply Size/Rot'; 
//          tbstr1:= 'c|a'; 
//        end;
//        5:
//        begin
//          tbstr:= 'Apply Deform'; 
//          tbstr1:= 'c|A'; 
//        end;
//        6:
//        begin
//          tbstr:= 'Join'; 
//          tbstr1:= 'c|j'; 
//        end;
//        7:
//        begin
//          tbstr:= 'make local'; 
//          tbstr1:= 'l'; 
//        end;
//        8:
//        begin
//          tbstr:= 'select Links'; 
//          tbstr1:= 'L'; 
//        end;
//        9:
//        begin
//          tbstr:= 'make Links'; 
//          tbstr1:= 'c|l'; 
//        end;
//        10:
//        begin
//          tbstr:= 'Copy menu'; 
//          tbstr1:= 'c|c'; 
//        end;
//        11:
//        begin
//          tbstr:= 'Convert menu'; 
//          tbstr1:= 'a|c'; 
//        end;

//      end;{case?}
//    end;
//    else

//    (* mesh TOPICS *)
//    if tbmain=5 then
//    begin 
//      case y of
//        0:
//        begin
//          tbstr:= 'Select Linked'; 
//          tbstr1:= 'l'; 
//        end;
//        1:
//        begin
//          tbstr:= 'Deselect Linked'; 
//          tbstr1:= 'L'; 
//        end;
//        2:
//        begin
//          tbstr:= 'Extrude'; 
//          tbstr1:= 'e'; 
//        end;
//        3:
//        begin
//          tbstr:= 'Delete Menu'; 
//          tbstr1:= 'x'; 
//        end;
//        4:
//        begin
//          tbstr:= 'Make edge/face'; 
//          tbstr1:= 'f'; 
//        end;
//        5:
//        begin
//          tbstr:= 'Fill'; 
//          tbstr1:= 'F'; 
//        end;
//        6:
//        begin
//          tbstr:= 'Split'; 
//          tbstr1:= 'y'; 
//        end;
//        7:
//        begin
//          tbstr:= 'Undo/reload'; 
//          tbstr1:= 'u'; 
//        end;
//        8:
//        begin
//          tbstr:= 'Calc Normals'; 
//          tbstr1:= 'c|n'; 
//        end;
//        9:
//        begin
//          tbstr:= 'Separate'; 
//          tbstr1:= 'p'; 
//        end;
//        10:
//        begin
//          tbstr:= 'Write Videosc'; 
//          tbstr1:= 'a|w'; 
//        end;
//        11:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;

//      end;{case?}
//    end;
//    else
//    (* CURVE TOPICS *)
//    if tbmain=6 then
//    begin 
//      case y of
//        0:
//        begin
//          tbstr:= 'Select Linked'; 
//          tbstr1:= 'l'; 
//        end;
//        1:
//        begin
//          tbstr:= 'Deselect Link'; 
//          tbstr1:= 'L'; 
//        end;
//        2:
//        begin
//          tbstr:= 'Extrude'; 
//          tbstr1:= 'e'; 
//        end;
//        3:
//        begin
//          tbstr:= 'Delete Menu'; 
//          tbstr1:= 'x'; 
//        end;
//        4:
//        begin
//          tbstr:= 'Make Segment'; 
//          tbstr1:= 'f'; 
//        end;
//        5:
//        begin
//          tbstr:= 'Cyclic'; 
//          tbstr1:= 'c'; 
//        end;
//        6:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        7:
//        begin
//          tbstr:= 'SelectRow'; 
//          tbstr1:= 'R'; 
//        end;
//        8:
//        begin
//          tbstr:= 'Calc Handle'; 
//          tbstr1:= 'h'; 
//        end;
//        9:
//        begin
//          tbstr:= 'Auto Handle'; 
//          tbstr1:= 'H'; 
//        end;
//        10:
//        begin
//          tbstr:= 'Vect Handle'; 
//          tbstr1:= 'v'; 
//        end;
//        11:
//        begin
//          tbstr:= 'PrintWeights'; 
//          tbstr1:= 'w'; 
//        end;

//      end;{case?}
//    end;
//    else
//    (* KEY TOPICS *)
//    if tbmain=7 then
//    begin 
//      case y of
//        0:
//        begin
//          tbstr:= 'Insert'; 
//          tbstr1:= 'i'; 
//        end;
//        1:
//        begin
//          tbstr:= 'Show'; 
//          tbstr1:= 'k'; 
//        end;
//        2:
//        begin
//          tbstr:= 'Next'; 
//          tbstr1:= 'PageUp'; 
//        end;
//        3:
//        begin
//          tbstr:= 'Prev'; 
//          tbstr1:= 'PageDn'; 
//        end;
//        4:
//        begin
//          tbstr:= 'Show+sel'; 
//          tbstr1:= 'K'; 
//        end;
//        5:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        6:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        7:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        8:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        9:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        10:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        11:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;

//      end;{case?}
//    end;
//    else

//    (* RENDER TOPICS *)
//    if tbmain=8 then
//    begin 
//      case y of
//        0:
//        begin
//          tbstr:= 'RenderWindow'; 
//          tbstr1:= 'F11'; 
//        end;
//        1:
//        begin
//          tbstr:= 'Render'; 
//          tbstr1:= 'F12'; 
//        end;
//        2:
//        begin
//          tbstr:= 'Set Border'; 
//          tbstr1:= 'B'; 
//        end;
//        3:
//        begin
//          tbstr:= 'Image Zoom'; 
//          tbstr1:= 'z'; 
//        end;
//        4:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        5:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        6:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        7:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        8:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        9:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        10:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        11:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;

//      end;{case?}
//    end;
//    else
//    (* FILE TOPICS *)
//    if tbmain=9 then
//    begin 
//      case y of
//        0:
//        begin
//          tbstr:= 'Load File As'; 
//          tbstr1:= 'F1'; 
//        end;
//        1:
//        begin
//          tbstr:= 'Load Last'; 
//          tbstr1:= 'c|o'; 
//        end;
//        2:
//        begin
//          tbstr:= 'Append File'; 
//          tbstr1:= 'shift+F1'; 
//        end;
//        3:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        4:
//        begin
//          tbstr:= 'Save File as'; 
//          tbstr1:= 'F2'; 
//        end;
//        5:
//        begin
//          tbstr:= 'Save'; 
//          tbstr1:= 'c|w'; 
//        end;
//        6:
//        begin
//          tbstr:= 'Save Image'; 
//          tbstr1:= 'F3'; 
//        end;
//        7:
//        begin
//          tbstr:= 'Save VRML'; 
//          tbstr1:= 'c|F2'; 
//        end;
//        8:
//        begin
//          tbstr:= 'Save DXF'; 
//          tbstr1:= 'shift+F2'; 
//        end;
//        9:
//        begin
//          tbstr:= 'Save VideoScape'; 
//          tbstr1:= 'a|w'; 
//        end;
//        10:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        11:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        12:
//        begin
//          tbstr:= ; 
//          tbstr1:= ; 
//        end;
//        13:
//        begin
//          tbstr:= 'Quit Blender'; 
//          tbstr1:= 'q'; 
//        end;

//      end;{case?}
//    end;
//  end;
//end;

(* ******************** INIT ************************** *)

procedure dummy;
begin
end;

procedure bgnpupdraw(startx: integer;  starty: integer;  endx: integer;  endy: integer);
begin
  {$if defined(__sgi) or defined(__SUN)}
  (* this is a dirty patch: XgetImage gets sometimes the backbuffer *)
  my_get_frontbuffer_image(0,0,1,1);
  my_put_frontbuffer_image();
  {$endif}

  tboldwin:= winget();

  winset(G.curscreen^.mainwin);

  (* pietsje groter, 1 pixel aan de rand *)

  glReadBuffer(GL_FRONT);
  glDrawBuffer(GL_FRONT);

  my_get_frontbuffer_image(startx-1,starty-1,endx-startx+2,endy-starty+6);

  mygetcursor(@oldcursor);
  glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
  fmsetfont(G.font);

  tbfontyofs:= (TBOXH-11) div 2; (* toolbox, hier stond ooit getheigh *)
end;

procedure endpupdraw;
var
x: integer;
begin
  my_put_frontbuffer_image();
  if tboldwin<>0 then
  begin
    winset(tboldwin);
    glutSetCursor(oldcursor);
  end;
  glReadBuffer(GL_BACK);

  glDrawBuffer(GL_BACK);
end;

(* ********************************************** *)

//procedure asciitoraw(ch: integer;  event: pushort;  qual: pushort); 
//begin
//  if isalpha(ch)=0 then
//  exit;
//  if isupper(ch) then
//  begin 
//    {*}qual^:=LEFTSHIFTKEY; 
//    ch:= tolower(ch); 
//  end;
//  case ch of
//    'a':
//    begin
//      {*}event^:=AKEY; 
//    end;
//    'b':
//    begin
//      {*}event^:=BKEY; 
//    end;
//    'c':
//    begin
//      {*}event^:=CKEY; 
//    end;
//    'd':
//    begin
//      {*}event^:=DKEY; 
//    end;
//    'e':
//    begin
//      {*}event^:=EKEY; 
//    end;
//    'f':
//    begin
//      {*}event^:=FKEY; 
//    end;
//    'g':
//    begin
//      {*}event^:=GKEY; 
//    end;
//    'h':
//    begin
//      {*}event^:=HKEY; 
//    end;
//    'i':
//    begin
//      {*}event^:=IKEY; 
//    end;
//    'j':
//    begin
//      {*}event^:=JKEY; 
//    end;
//    'k':
//    begin
//      {*}event^:=KKEY; 
//    end;
//    'l':
//    begin
//      {*}event^:=LKEY; 
//    end;
//    'm':
//    begin
//      {*}event^:=MKEY; 
//    end;
//    'n':
//    begin
//      {*}event^:=NKEY; 
//    end;
//    'o':
//    begin
//      {*}event^:=OKEY; 
//    end;
//    'p':
//    begin
//      {*}event^:=PKEY; 
//    end;
//    'q':
//    begin
//      {*}event^:=QKEY; 
//    end;
//    'r':
//    begin
//      {*}event^:=RKEY; 
//    end;
//    's':
//    begin
//      {*}event^:=SKEY; 
//    end;
//    't':
//    begin
//      {*}event^:=TKEY; 
//    end;
//    'u':
//    begin
//      {*}event^:=UKEY; 
//    end;
//    'v':
//    begin
//      {*}event^:=VKEY; 
//    end;
//    'w':
//    begin
//      {*}event^:=WKEY; 
//    end;
//    'x':
//    begin
//      {*}event^:=XKEY; 
//    end;
//    'y':
//    begin
//      {*}event^:=YKEY; 
//    end;
//    'z':
//    begin
//      {*}event^:=ZKEY; 
//    end;

//  end;{case?}
//end;

//procedure tbox_execute; 
//var
//event: ushort;

//qual1: ushort;

//qual2: ushort;

//begin(* als tbfunc: functie aanroepen *)
//  (* als tbstr1 is een string: value tbval in queue stopen *)
//  event:=0; 
//  qual1:=0; 
//  qual2:=0; 
//  if tbfunc<>0 then
//  begin 
//    tbfunc(tbval); 
//  end;
//  else
//  if tbstr1<>0 then
//  begin 
//    if lstrcmp(tbstr1,'Tab')=0 then
//    begin 
//      event:= TABKEY; 
//    end;
//    else
//    if lstrcmp(tbstr1,'PageUp')=0 then
//    begin 
//      event:= PAGEUPKEY; 
//    end;
//    else
//    if lstrcmp(tbstr1,'PageDn')=0 then
//    begin 
//      event:= PAGEDOWNKEY; 
//    end;
//    else
//    if lstrcmp(tbstr1,'shift+F1')=0 then
//    begin 
//      qual1:= LEFTSHIFTKEY; 
//      event:= F1KEY; 
//    end;
//    else
//    if lstrcmp(tbstr1,'shift+F2')=0 then
//    begin 
//      qual1:= LEFTSHIFTKEY; 
//      event:= F2KEY; 
//    end;
//    else
//    if lstrlen(tbstr1)<4)or((lstrlen(tbstr1)=4)and(tbstr1[2]='F') then
//    begin 
//      if tbstr1[1]='|' then
//      begin 
//        if tbstr1[0]='c' then
//        qual1:= LEFTCTRLKEY; 
//        else
//        if tbstr1[0]='a' then
//        qual1:= LEFTALTKEY; 
//        if tbstr1[2]='F' then
//        begin 
//          case tbstr1[3] of
//            '1':
//            begin
//              event:= F1KEY; 
//            end;
//            '2':
//            begin
//              event:= F2KEY; 
//            end;
//            '3':
//            begin
//              event:= F3KEY; 
//            end;
//            '4':
//            begin
//              event:= F4KEY; 
//            end;
//            '5':
//            begin
//              event:= F5KEY; 
//            end;
//            '6':
//            begin
//              event:= F6KEY; 
//            end;
//            '7':
//            begin
//              event:= F7KEY; 
//            end;
//            '8':
//            begin
//              event:= F8KEY; 
//            end;
//            '9':
//            begin
//              event:= F9KEY; 
//            end;

//          end;{case?}
//        end;
//        else
//        asciitoraw(tbstr1[2], and event,@qual2); 
//      end;
//      else
//      if tbstr1[1]=0 then
//      begin 
//        asciitoraw(tbstr1[0], and event,@qual2); 
//      end;
//      else
//      if tbstr1[0]='F' then
//      begin 
//        event:= atoi(tbstr1+1); 
//        case event of
//          1:
//          begin
//            event:= F1KEY; 
//          end;
//          2:
//          begin
//            event:= F2KEY; 
//          end;
//          3:
//          begin
//            event:= F3KEY; 
//          end;
//          4:
//          begin
//            event:= F4KEY; 
//          end;
//          5:
//          begin
//            event:= F5KEY; 
//          end;
//          6:
//          begin
//            event:= F6KEY; 
//          end;
//          7:
//          begin
//            event:= F7KEY; 
//          end;
//          8:
//          begin
//            event:= F8KEY; 
//          end;
//          9:
//          begin
//            event:= F9KEY; 
//          end;
//          10:
//          begin
//            event:= F10KEY; 
//          end;
//          11:
//          begin
//            event:= F11KEY; 
//          end;
//          12:
//          begin
//            event:= F12KEY; 
//          end;

//        end;{case?}
//      end;
//    end;
//    if event<>0 then
//    begin 
//      if qual1<>0 then
//      qenter(qual1,1); 
//      if qual2<>0 then
//      qenter(qual2,1); 
//      qenter(event,1); 
//      qenter(event,0); 
//      qenter(EXECUTE,1); 
//      if qual1<>0 then
//      qenter(qual1,0); 
//      if qual2<>0 then
//      qenter(qual2,0); 
//    end;
//  end;
//end;

procedure tbox_getmouse(mval: psmallint);
begin
  getmouseco_sc(mval);
end;

//procedure tbox_setmain(val: integer); 
//begin
//  tbmain:= val; 
//  if tbmain=0)and(G.obedit then
//  begin 
//    addmode:= G.obedit.type; 
//  end;
//end;

//procedure bgntoolbox; 
//var
//xmax: smallint; 
//ymax: smallint; 
//mval: array [0..Pred(2)] of smallint; 
//begin
//  {displaysizex: integer; }{<= !!!5 external variable}
//  {displaysizey: integer; }{<= !!!5 external variable}

//  xmax:= G.curscreen.sizex; 
//  ymax:= G.curscreen.sizey; 
//  tbox_getmouse(mval); 

//  if mval[0]<95 then
//  mval[0]:= 95; 
//  if mval[0]>xmax-95 then
//  mval[0]:= xmax-95; 
//  warp_pointer(mval[0],mval[1]); 

//  tbx1:= mval[0]-tbmemx; 
//  tby1:= mval[1]-tbmemy; 
//  if tbx1<10 then
//  tbx1:= 10; 
//  if tby1<10 then
//  tby1:= 10; 
//  tbx2:= tbx1+TBOXX; 
//  tby2:= tby1+TBOXY; 
//  if tbx2>xmax then
//  begin 
//    tbx2:= xmax-10; 
//    tbx1:= tbx2-TBOXX; 
//  end;
//  if tby2>ymax then
//  begin 
//    tby2:= ymax-10; 
//    tby1:= tby2-TBOXY; 
//  end;
//  bgnpupdraw(tbx1,tby1,tbx2,tby2); 

//end;

//procedure endtoolbox; 
//var
//mval: array [0..Pred(2)] of smallint; 
//begin

//  tbox_getmouse(mval); 
//  if mval[0]>tbx1)and(mval[0]<tbx2 then
//  if mval[1]>tby1)and(mval[1]<tby2 then
//  begin 
//    tbmemx:= mval[0]-(tbx1); 
//    tbmemy:= mval[1]-(tby1); 
//  end;
//endpupdraw(); 
//end;

procedure tbox_embossbox(x1: smallint;  y1: smallint;  x2: smallint;  y2: smallint;  _type: smallint);
(* type: 0=menu, 1=menusel, 2=topic, 3=topicsel *)
begin
  (* X is used while frontbufferdrawing remains slow! *)
  (* for BeOS these functions are written in Kit functions *)
  glColor3ub(0,0,0); (* BLACK *)
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glRecti(x1,y1,x2,y2);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  if (_type=0)or(_type=2) then
  begin
    if _type=2 then
    begin
      glColor3ub(210,210,210); (* LGREY *)
      glRecti(x1+1,y1+1,x2-1,y2-1);
      glColor3ub(240,240,240); (* WHITE *)
    end
    else
    begin
      glColor3ub(160,160,160);     (* GREY *)
      glRecti(x1+1,y1+1,x2-1,y2-1);
      glColor3ub(210,210,210);     (* LGREY *)
    end;

    sdrawline(x1+1,y2-1,x2-1,y2-1);    (* boven *)
    sdrawline(x1+1,y2-2,x2-2,y2-2);    (* boven *)
    sdrawline(x1+1,y1+1,x1+1,y2-1);    (* links *)
    glColor3ub(160,160,160);           (* GREY *)
    sdrawline(x1+1,y1+1,x2-1,y1+1);    (* onder *)
    sdrawline(x2-1,y1+1,x2-1,y2-1);    (* rechts *)

    glColor3ub(0,0,0);                 (* text *)
  end
  else
  begin
    if _type=1 then
    begin
      glColor3ub(80,80,80);            (* DGREY *)
      glRecti(x1+1,y1+1,x2-1,y2-1);
    end
    else
    begin
      glColor3ub(160,160,160);         (* GREY *)
      glRecti(x1+1,y1+1,x2-1,y2-1);
    end;

    glColor3ub(210,210,210);           (* LGREY *)
    sdrawline(x1+1,y1+1,x2-1,y1+1);    (* onder *)
    sdrawline(x2-1,y1+1,x2-1,y2-1);    (* rechts *)
    glColor3ub(80,80,80);              (* DGREY *)
    sdrawline(x1+1,y2-2,x2-1,y2-2);    (* boven *)
    sdrawline(x1+1,y2-1,x2-1,y2-1);    (* boven *)
    sdrawline(x1+1,y1+1,x1+1,y2-1);    (* links *)

    if _type=1 then
    glColor3ub(240,240,240)
    else
    glColor3ub(0,0,0);         (* text *)
  end;
end;

//procedure tbox_drawelem_body{!!!3 unknown typedef}; 
//var
//x1: integer; 
//y1: integer; 
//x2: integer; 
//y2: integer; 
//len1: integer; 
//len2: integer; 
//begin

//  if x=0 then
//  begin 
//    x1:= tbx1; 
//    x2:= tbx1+TBOXXL; 
//  end;
//  else
//  if x=1 then
//  begin 
//    x1:= tbx1+TBOXXL; 
//    x2:= x1+TBOXXR-1; 
//  end;
//  y1:= tby1+(TBOXEL-y-1)*TBOXH; 
//  y2:= y1+TBOXH-1; 
//  tbox_embossbox(x1,y1,x2,y2,type); 
//end;

//procedure tbox_drawelem_text{!!!3 unknown typedef}; 
//var
//x1: integer; 
//y1: integer; 
//x2: integer; 
//y2: integer; 
//len1: integer; 
//len2: integer; 
//begin

//  if x=0 then
//  begin 
//    x1:= tbx1; 
//    x2:= tbx1+TBOXXL; 
//  end;
//  else
//  if x=1 then
//  begin 
//    x1:= tbx1+TBOXXL; 
//    x2:= x1+TBOXXR-1; 
//  end;
//  y1:= tby1+(TBOXEL-y-1)*TBOXH; 
//  y2:= y1+TBOXH-1; 
//  if type=0)or(type=2 then
//  begin 
//    glIndexi(TBOXBLACK); 
//  end;
//  else
//  begin 
//    if type=1 then
//    glColor3ub(240,240,240); 
//    else
//    glColor3ub(0,0,0); 
//  end;
//  tbox_setinfo(x,y); 
//  (* tekst *)
//  if tbstr)and(tbstr[0] then
//  begin 
//    len1:= 5+fmgetstrwidth(G.font,tbstr); 
//    if tbstr1<>0 then
//    len2:= 5+fmgetstrwidth(G.font,tbstr1); 
//    else
//    len2:= 0; 
//    while len1>0)and((len1+len2+5>x2-x1)
//    do
//    begin 
//      tbstr[lstrlen(tbstr)-1]:= 0; 
//      len1:= fmgetstrwidth(G.font,tbstr); 
//    end;
//    glRasterPos2i(x1+5,y1+tbfontyofs); 
//    fmprstr(tbstr); 
//    if tbstr1)and(tbstr1[0] then
//    begin 
//      if type and 1 then
//      begin 
//        glIndexi(TBOXBLACK); 
//        glRecti(x2-len2-2,y1+2,x2-3,y2-2); 
//        glIndexi(TBOXWHITE); 
//        glRasterPos2i(x2-len2,y1+tbfontyofs); 
//        fmprstr(tbstr1); 
//      end;
//      else
//      begin 
//        glIndexi(TBOXBLACK); 
//        glRasterPos2i(x2-len2,y1+tbfontyofs); 
//        fmprstr(tbstr1); 
//      end;
//    end;
//  end;
//end;

//void{!!!e unknown token}
//x: integer; 
//y: integer; 
//type: integer; 
//begin 
//  (* type: 0=menu, 1=menusel, 2=topic, 3=topicsel *)
//  (* gesplitst: op SGI loopt de Xdraw achter op de OpenGL draw
//      en een Xfush helpt niet *)

//  tbox_drawelem_body(x,y,type); 
//  tbox_drawelem_text(x,y,type); 
//end;

//void{!!!e unknown token}
//x: pinteger; 
//y: pinteger; 
//begin 
//  mval: array [0..Pred(2)] of smallint; 
//  tbox_getmouse(mval); 
//  mval[0]:= mval[0] - (tbx1); 
//  if mval[0]<TBOXXL then
//  {*}x^:=0; 
//  else
//  {*}x^:=1; 
//  {*}y^:=mval[1]-tby1; 
//  {*}y^ div =TBOXH; 
//  {*}y^:=TBOXEL-*y-1; 
//  if {*}y^<0 then
//  {*}y^:=0; 
//  if {*}y^>TBOXEL-1 then
//  {*}y^:=TBOXEL-1; 
//end;

//procedure drawtoolbox; 
//var
//x: integer; 
//y: integer; 
//actx: integer; 
//acty: integer; 
//type: integer; 
//begin

//  tbox_getactive( and actx, and acty); 

//  (* de background *)
//  for{while} x:=0 to Pred(2) { x++}
//  do
//  begin 
//    for{while} y:=0 to Pred(TBOXEL) { y++}
//    do
//    begin 
//      if x=0 then
//      type:= 0; 
//      else
//      type:= 2; 
//      if actx=x)and(acty=y then
//      inc(type); 
//      if type=0 then
//      begin 
//        if tbmain=y then
//        type:= 1; 
//      end;
//      tbox_drawelem_body(x,y,type); 
//    end;
//  end;

//  (* de text *)
//  for{while} x:=0 to Pred(2) { x++}
//  do
//  begin 
//    for{while} y:=0 to Pred(TBOXEL) { y++}
//    do
//    begin 
//      if x=0 then
//      type:= 0; 
//      else
//      type:= 2; 
//      if actx=x)and(acty=y then
//      inc(type); 
//      if type=0 then
//      begin 
//        if tbmain=y then
//        type:= 1; 
//      end;
//      tbox_drawelem_text(x,y,type); 
//    end;
//  end;
//end;

//procedure toolbox; 
//var
//actx: integer; 
//acty: integer; 
//x: integer; 
//y: integer; 
//event: ushort; 
//val: smallint; 
//mval: array [0..Pred(2)] of smallint; 
//xo: smallint;

//yo: smallint;
// (* geen overdraw, dus... *)
//begin

//  xo:=-1; 
//  yo:=0; (* geen overdraw, dus... *)
//  if R.win)and(R.win=G.curscreen.winakt then
//  exit;

//  bgntoolbox(); 
//  drawtoolbox(); 
//  (* 
//    * De aktieve window wordt in queue terug gestopt.
//    *)

//  while 1
//  do
//  begin 
//    event:= extern_qread(@val); 
//    if event<>0 then
//    begin 
//      case event of
//        LEFTMOUSE,
//        MIDDLEMOUSE,
//        RIGHTMOUSE,
//        RETKEY,
//        PADENTER:
//        begin
//          if val=1 then
//          begin 
//            tbox_getactive( and actx, and acty); 
//            tbox_setinfo(actx,acty); 
//            if event=RIGHTMOUSE then
//            begin 
//              if addmode<>0 then
//              begin 
//                addmode:= 0; 
//                drawtoolbox(); 
//              end;
//            end;
//            else
//            if tbstr1)and(tbstr1[0]='>' then
//            begin 
//              addmode:= tbval; 
//              drawtoolbox(); 
//            end;
//            else
//            begin 
//            endtoolbox(); 
//            tbox_execute(); 
//            exit;
//          end;
//        end;
//      end;
//      ESCKEY,
//      (* altkeys: om conflicten met overdraw en stow/push/pop te voorkomen *)
//      LEFTALTKEY,
//      RIGHTALTKEY:
//      begin
//        if val<>0 then
//      endtoolbox(); 
//      exit;
//    end;
//  end;{case?}
//end;
//else
//usleep(2); 
//tbox_getmouse(mval); 
//if mval[0]<tbx1-10)or(mval[0]>tbx2+10)or(mval[1]<tby1-10)or(mval[1]>tby2+10 then
//break; {<= !!!b possible in "switch" - then remove this line}
//tbox_getactive( and actx, and acty); 
//(* muisafhandeling en redraw *)
//if xo<>actx)or(yo<>acty then
//begin 
//  if actx=0 then
//  begin 
//    if acty=0 then
//    addmode:= 0; 
//    tbox_drawelem(0,tbmain,0); 
//    tbox_drawelem(0,acty,1); 
//    tbmain:= acty; 
//    addmode:= 0; 
//    for{while} y:=0 to Pred(TBOXEL) { y++}
//    do
//    tbox_drawelem(1,y,2); 
//  end;
//  else
//  if xo>-1 then
//  begin 
//    if xo=0 then
//    tbox_drawelem(xo,yo,1); 
//    else
//    tbox_drawelem(xo,yo,2); 
//    tbox_drawelem(actx,acty,3); 
//  end;
//  xo:= actx; 
//  yo:= acty; 
//end;
//end;
//endtoolbox(); 

//end;

(* ************************************  *)

procedure pupmenu_draw(startx, starty, endx, endy, items: integer; title: pchar; spoin: ppchar; sel: smallint);
var
a: integer = 0;
aold: integer;
yold: integer;
begin
  (* first X drawing, then text *)

  starty:= starty + ((items-1)*TBOXH);
  if title<>#0 then
  begin
    tbox_embossbox(startx,starty+4,endx,starty+TBOXH+4,0);
    glIndexi(TBOXBLACK);
    starty:= starty - (TBOXH);
    a:= 1;
  end;

  yold:= starty;
  aold:= a;
  while a < items do
  begin
    tbox_embossbox(startx,starty,endx,starty+TBOXH,2);
    starty:= starty - (TBOXH);
    inc(a);
  end;

  if title<>#0 then
  begin
    starty:= yold+TBOXH;
    glRasterPos2i(startx+5,starty+tbfontyofs+4);
    fmprstr(title);
  end;
  glIndexi(TBOXBLACK);

  starty:= yold;
  for{while} a:=aold to Pred(items) { a++}
  do
  begin
    glRasterPos2i(startx+5,starty+tbfontyofs);
    fmprstr(spoin[a]);
    starty:= starty - (TBOXH);
  end;
end;

var
lastselected: integer = 1;
laststring: array [0..99] of char = '';

function pupmenu(instr: pchar): smallint; (* zelfde syntax als dopup *)
var
len: integer;
height: integer;
a: integer;
items: integer;
retval: array [0..99] of integer;
xmax: integer;
ymax: integer;
startx: integer;
starty: integer;
endx: integer;
endy: integer;
act: integer;
acto: integer=-1;
value: integer=0;
event: word;
val: smallint;
mval: array [0..Pred(2)] of smallint;
mouseymove: smallint;
mousexmove: smallint;
str: pchar;
astr: pchar;
title: pchar = #0;
spoin: array [0..Pred(100)] of pchar;
begin
  if instr=#0 then
  exit(0);

  (* geen overdraw, dus... *)
  if (G.curscreen=nil) then
  //TODO: restore this ... if (G.curscreen=nil)or ((R.win<>0)and(R.win=G.curscreen.winakt)) then
    exit(0);

  (* kopie string maken *)
  str:= mallocN(lstrlen(instr)+1,'pupmenu');
  memcpy(str,instr,lstrlen(instr)+1);
  bzero(@retval,400);

  (* eerst string uitelkaar pulken, tellen hoeveel elementen, return values *)
  astr:= str;
  spoin[0]:= astr;
  items:= 0;
  while astr^ <> #0  do
  begin
    case astr^ of
      '%':
      begin
        if astr[1]='x' then
        begin
          retval[items]:= atoi(astr+2);
          astr^:=#0;
        end
        else
        if astr[1]='t' then
        begin
          title:= spoin[0];
          astr^:=#0;
        end
        else
        if astr[1]='T' then
        begin
          title:= spoin[0];
          astr^:=#0;
          lastselected:= 1;
          acto:= 1;
          (* voor confirm:eerste item niet selected, ziet er leuker uit *)
        end;
      end;
      '|':
      begin
        if astr[1]<>#0 then
        begin
          inc(items);
          retval[items]:= items;
          spoin[items]:= astr+1;
        end;
        astr^:=#0;
      end;
      '`':
      begin
        astr^:='|';
      end;
    end;
    inc(astr);
  end;

  inc(items); (* het begon bij 0 te tellen *)

  (* voor notice: ziet er leuker uit *)
  if items=1 then
  acto:= 0;

  (* afmetingen en plaats *)
  len:= 0;
  for a:=0 to items-1   do
  begin
    xmax:= fmgetstrwidth(G.font,spoin[a]);
    if xmax>len then
    len:= xmax;
  end;
  len:= len + (10);

  height:= TBOXH*items;

  xmax:= G.curscreen^.sizex;
  ymax:= G.curscreen^.sizey;

  tbox_getmouse(mval);

  if lstrcmp(laststring,spoin[0])<>0 then
  lastselected:= 1;
  strcpy(laststring,spoin[0]);

  startx:= mval[0]-len div 2;
  if (lastselected>=0)and(lastselected<items) then
  begin
    starty:= mval[1]-height+TBOXH div 2+lastselected*TBOXH;
  end
  else
  starty:= mval[1]-height div 2;

  mouseymove:= 0;

  if startx<10 then
  startx:= 10;
  if starty<10 then
  begin
    mouseymove:= 10-starty;
    starty:= 10;
  end;

endx:= startx+len;
endy:= starty+height;
if endx>xmax then
begin
endx:= xmax-10;
startx:= endx-len;
end;
if endy>ymax then
begin
  mouseymove:= ymax-endy-10;
endy:= ymax-10;
starty:= endy-height;
end;

if mouseymove<>0then
begin
  warp_pointer(mval[0],mouseymove+mval[1]);
  mousexmove:= mval[0];
  mouseymove:= mval[1];
end;

bgnpupdraw(startx,starty,endx,endy);

pupmenu_draw(startx,starty,endx,endy,items,title,spoin,1);

while value=0 do
begin
  event:= extern_qread(@val);

  if event<>0 then
  begin
//    case event of
//      LEFTMOUSE,
//      MIDDLEMOUSE,
//      RIGHTMOUSE,
//      RETKEY,
//      PADENTER:
//      begin
//        if val<>0 then
//        begin
//          value:= 2;
//        end;
//      end;
//      ONEKEY:
//      begin
//        if val<>0 then
//        begin
//          value:= 3;
//          act:= 1;

//        end;

//      end;
//      TWOKEY:
//      begin
//        if val<>0 then
//        begin
//          value:= 3;
//          act:= 2;

//        end;

//      end;
//      THREEKEY:
//      begin
//        if val<>0 then
//        begin
//          value:= 3;
//          act:= 3;

//        end;

//      end;
//      FOURKEY:
//      begin
//        if val<>0 then
//        begin
//          value:= 3;
//          act:= 4;

//        end;

//      end;
//      FIVEKEY:
//      begin
//        if val<>0 then
//        begin
//          value:= 3;
//          act:= 5;

//        end;

//      end;
//      SIXKEY:
//      begin
//        if val<>0 then
//        begin
//          value:= 3;
//          act:= 6;

//        end;

//      end;
//      SEVENKEY:
//      begin
//        if val<>0 then
//        begin
//          value:= 3;
//          act:= 7;

//        end;

//      end;
//      EIGHTKEY:
//      begin
//        if val<>0 then
//        begin
//          value:= 3;
//          act:= 8;

//        end;

//      end;
//      NINEKEY:
//      begin
//        if val<>0 then
//        begin
//          value:= 3;
//          act:= 9;

//        end;

//      end;
//      ZEROKEY:
//      begin
//        if val<>0 then
//        begin
//          value:= 3;
//          act:= 10;

//        end;

//      end;
//      DOWNARROWKEY:
//      begin
//        if val)and(endy<ymax-TBOXH then
//        begin
//          starty:= starty + (TBOXH);
//        endy:= endy + (TBOXH);
//        my_put_frontbuffer_image();
//        my_get_frontbuffer_image(startx,starty-1,endx-startx+1,endy-starty+6);
//        pupmenu_draw(startx,starty,endx,endy,items,title,spoin,1);

//      end;
//    end;
//    UPARROWKEY:
//    begin
//      if val)and(starty>TBOXH then
//      begin
//        starty:= starty - (TBOXH);
//      endy:= endy - (TBOXH);
//      my_put_frontbuffer_image();

//      my_get_frontbuffer_image(startx,starty-1,endx-startx+1,endy-starty+6);
//      pupmenu_draw(startx,starty,endx,endy,items,title,spoin,1);

//    end;
//  end;
//  ESCKEY,
//  LEFTALTKEY,
//  RIGHTALTKEY:
//  begin
//    if val<>0 then
//    value:= 1;
//  end;

//end;{case?}
end
else
usleep(2);

if value=3 then
break;

tbox_getmouse(mval);
if (mval[0]<startx-20)or(mval[0]>endx+20)or(mval[1]<starty-20)or(mval[1]>endy+20) then
value:= 1;

act:= items-1-(mval[1]-starty) div TBOXH;
if ((act<>acto)and(act<items))or(value=2) then
begin
  a:= endy-20;

  (* teken acto *)
  if ((title<>nil)and(acto=0))or(value=2) then
  begin
  end
  else
  if acto>=0 then
  begin
    (* vergelijken, acto is init op -1 *)
    tbox_embossbox(startx,a-acto*TBOXH,endx,a-acto*TBOXH+TBOXH,2);
    glIndexi(TBOXBLACK);
    glRasterPos2i(startx+5,a-acto*TBOXH+tbfontyofs);
    fmprstr(spoin[acto]);
  end;
  (* teken act *)
  if (title<>nil)and(act=0) then
  begin
  end
  else
  if act>=0 then
  begin
    tbox_embossbox(startx,a-act*TBOXH,endx,a-act*TBOXH+TBOXH,3);
    glIndexi(TBOXBLACK);
    glRasterPos2i(startx+5,a-act*TBOXH+tbfontyofs);
    fmprstr(spoin[act]);
  end;
  acto:= act;
end;
end;

if value=2 then
usleep(10);

endpupdraw();
freeN(str);

if mouseymove<>0 then
begin
  warp_pointer(mousexmove,mouseymove);
end;

if (value>=2)and(act>=0)and(act<100) then
begin
  lastselected:= act;
  exit(retval[act]);
end;
exit(-1);
end;

function confirm(title: pchar;  item: pchar): smallint;
var
ret: smallint;
str: pchar;
begin
  if item[0]=#0 then
  exit(0);

  str:= mallocN(lstrlen(title)+lstrlen(item)+4,'confirm');
  if title[0]=#0 then
  sprintf(str,'%s',[item])
  else
  sprintf(str,'%s%%T|%s',[title,item]);

  ret:= pupmenu(str);
  freeN(str);

  if ret>0 then
  exit(1)
  else
  exit(0);
end;

//procedure notice(str: pchar); 
//begin
//  confirm(,str); 
//end;

//function saveover(str: pchar): integer; 
//var
//file: integer; 
//begin

//  if G.f and G_DISABLE_OK then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  file:= open(str,O_BINARY or O_RDONLY); 
//  close(file); 
//  if file=-1 then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  else
//  if confirm('SAVE OVER',str) then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;

//function okee(str: pchar): smallint; 
//begin
//  if G.f and G_DISABLE_OK then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  begin
//    result:= confirm('OK?',str); 
//    exit;
//  end;
//end;

procedure error(str: pchar);
var
str1: array [0..99] of char;
begin

  if G.background<>0 then
  begin
    printf('ERROR %s\n',[str]);
    exit;
  end;

  if lstrlen(str)>90 then
  str[90]:= #0;
  sprintf(str1,'ERROR: %s',[str]);
  confirm('',str1);
end;

//procedure errorstr(str1: pchar;  str2: pchar;  str3: pchar); 
//var
//str: array [0..Pred(256)] of char; 
//begin

//  if str1)and(lstrlen(str1)>79 then
//  str1[79]:= 0; 
//  if str2)and(lstrlen(str2)>79 then
//  str2[79]:= 0; 
//  if str3)and(lstrlen(str3)>79 then
//  str3[79]:= 0; 
//  strcpy(str,'ERROR '); 

//  if str1<>0 then
//  strcat(str,str1); 
//  strcat(str,' '); 
//  if str2<>0 then
//  strcat(str,str2); 
//  strcat(str,' '); 
//  if str3<>0 then
//  strcat(str,str3); 

//  if G.background<>0 then
//  begin 
//    printf('ERROR %s\n',str);
//    exit;
//  end;
//  confirm(,str); 
//end;

//(* ****************** EXTRAATJE **************** *)

//function button(var: psmallint;  min: smallint;  max: smallint;  str: pchar): smallint; 
//var
//toets: ushort; 
//x1: smallint; 
//x2: smallint; 
//y1: smallint; 
//y2: smallint; 
//oldmap: array [0..Pred(4),0..Pred(3)] of smallint; 
//val: smallint; 
//mval: array [0..Pred(2)] of smallint; 
//ret: smallint;

//begin

//  ret:=0; 
//  if min>max then
//  min:= max; 
//  getmouseco_sc(mval); 

//  if mval[0]<150 then
//  mval[0]:= 150; 
//  if mval[1]<30 then
//  mval[1]:= 30; 
//  if mval[0]>1240 then
//  mval[0]:= 1240; 
//  if mval[1]>980 then
//  mval[1]:= 980; 
//  winset(G.curscreen.mainwin); 

//  x1:= mval[0]-150; 
//  x2:= mval[0]+20; 
//  y1:= mval[1]-20; 
//  y2:= mval[1]+20; 
//  glIndexi(1); 

//  bgnpupdraw(x1,y1,x2,y2); 
//  glColor3ub(160,160,160); 

//  glRecti(x1,y1,x2,y2); 
//  (* LGREY *)
//  glColor3ub(0,0,0); 
//  sdrawbox(x1,y1,x2,y2); 
//  DefButBlock('button',G.curscreen.mainwin,G.font,2,10,2); 

//  DefBut(NUM or SHO,4001,str,x1+5,y1+10,125,20,var,{!!!a type cast? =>} {float(}min,{!!!a type cast? =>} {float(}max,0,0); 

//  DefBut(BUT,4002,'OK',x1+136,y1+10,25,20,0,0,0,0,0); 

//  while LongBool(1)
//  do
//  begin 
//    toets:= extern_qread(@val); 
//    if val<>0 then
//    begin 
//      if toets=ESCKEY then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      if toets=RETKEY then
//      begin 
//        ret:= 1; 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      getmouseco_sc(mval); 

//      if mval[0]<x1-40)or(mval[0]>x2+40)or(mval[1]<y1-30)or(mval[1]>y2+30 then
//      break; {<= !!!b possible in "switch" - then remove this line}

//      if toets=LEFTMOUSE then
//      begin 
//        FrontbufferButs(LongBool(1)); 
//        if DoButtons()=4002 then
//        begin 
//          ret:= 1; 
//          FrontbufferButs(LongBool(0)); 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        FrontbufferButs(LongBool(0)); 
//      end;
//    end;
//    if toets=0 then
//    usleep(1); 
//  end;
//  until not {0=}(get_mbut()@L_MOUSE);

//  FreeButBlock('button'); 
//  remake_qual(); 

//endpupdraw(); 
//(* in Button.c zit geen extern_qread *)
//areawinset(curarea.win); 
//begin
//  result:= ret; 
//  exit;
//end;
//end;

//var {was static}
//oldlay: uint;

//procedure do_mlayer_toets(event: integer;  lay: puint); 
//begin
//  oldlay:=1; 
//  if event=-1 then
//  begin 
//    if {*}lay^=(2 shl 20)-1 then
//    begin 
//      if G.qual and LR_SHIFTKEY then
//      {*}lay^:=oldlay; 
//    end;
//    else
//    begin 
//      oldlay:= {*}lay^; {*}lay^:=(2 shl 20)-1; 
//    end;
//  end;
//  else
//  begin 
//    if G.qual and LR_ALTKEY then
//    begin 
//      if event<11 then
//      event:= event + (10); 
//    end;
//    if G.qual and LR_SHIFTKEY then
//    begin 
//      if {*}lay^ and (1 shl event) then
//      {*}lay^-=(1 shl event); 
//      else
//      {*}lay^+=(1 shl event); 
//    end;
//    else
//    {*}lay^:=(1 shl event); 
//  end;
//end;

//function movetolayer_buts(lay: puint): integer; 
//var
//oldlay: uint; 
//bit: integer; 
//dx: integer; 
//dy: integer; 
//a: integer; 
//x1: integer; 
//x2: integer; 
//y1: integer; 
//y2: integer; 
//sizex: integer;

//sizey: integer;

//toets: ushort; 
//oldmap: array [0..Pred(4),0..Pred(3)] of smallint; 
//val: smallint; 
//mval: array [0..Pred(2)] of smallint; 
//ret: smallint;

//str: array [0..Pred(12)] of char; 
//begin

//  sizex:=160; 
//  sizey:=30; 

//  ret:=0; 

//  if G.vd.localview<>0 then
//  begin 
//    error('Not in localview '); 
//    begin
//      result:= ret; 
//      exit;
//    end;
//  end;
//  getmouseco_sc(mval); 

//  if mval[0]<sizex div 2 then
//  mval[0]:= sizex div 2; 
//  if mval[1]<sizey div 2 then
//  mval[0]:= sizey div 2; 
//  if mval[0]>G.curscreen.sizex-sizex div 2 then
//  mval[0]:= G.curscreen.sizex-sizex div 2; 
//  if mval[1]>G.curscreen.sizey-sizey div 2 then
//  mval[1]:= G.curscreen.sizey-sizey div 2; 
//  winset(G.curscreen.mainwin); 

//  x1:= mval[0]-sizex div 2; 
//  x2:= mval[0]+sizex div 2; 
//  y1:= mval[1]-sizey div 2; 
//  y2:= mval[1]+sizey div 2; 
//  bgnpupdraw(x1,y1,x2,y2); 
//  DefButBlock('button',G.curscreen.mainwin,G.font,32,10,2); 

//  dx:= (sizex-5) div 12; 
//  dy:= sizey div 2; 
//  for{while} a:=0 to Pred(10) { a++}
//  do
//  begin 
//    DefBut(TOG or INT or BIT or a+10,4001+10+a,,x1+a*dx,y1,dx,dy,lay,0,0,0,0); 
//    DefBut(TOG or INT or BIT or a,4001+a,,x1+a*dx,y1+dy,dx,dy,lay,0,0,0,0); 
//    if a=4 then
//    x1:= x1 + (5); 
//  end;
//  x1:= x1 - (5); 
//  DefBut(BUT,4000,'OK',x1+10*dx+10,y1,2*dx,2*dy,0,0,0,0,0); 

//  while LongBool(1)
//  do
//  begin 
//    toets:= extern_qread(@val); 
//    if val<>0 then
//    begin 
//      oldlay:= {*}lay^; 
//      if toets=ONEKEY then
//      begin 
//        do_mlayer_toets(0,lay); 
//      end;
//      else
//      if toets=TWOKEY then
//      begin 
//        do_mlayer_toets(1,lay); 
//      end;
//      else
//      if toets=THREEKEY then
//      begin 
//        do_mlayer_toets(2,lay); 
//      end;
//      else
//      if toets=FOURKEY then
//      begin 
//        do_mlayer_toets(3,lay); 
//      end;
//      else
//      if toets=FIVEKEY then
//      begin 
//        do_mlayer_toets(4,lay); 
//      end;
//      else
//      if toets=SIXKEY then
//      begin 
//        do_mlayer_toets(5,lay); 
//      end;
//      else
//      if toets=SEVENKEY then
//      begin 
//        do_mlayer_toets(6,lay); 
//      end;
//      else
//      if toets=EIGHTKEY then
//      begin 
//        do_mlayer_toets(7,lay); 
//      end;
//      else
//      if toets=NINEKEY then
//      begin 
//        do_mlayer_toets(8,lay); 
//      end;
//      else
//      if toets=ZEROKEY then
//      begin 
//        do_mlayer_toets(9,lay); 
//      end;
//      else
//      if toets=MINUSKEY then
//      begin 
//        do_mlayer_toets(10,lay); 
//      end;
//      else
//      if toets=EQUALKEY then
//      begin 
//        do_mlayer_toets(11,lay); 
//      end;
//      else
//      if toets=ACCENTGRAVEKEY then
//      begin 
//        do_mlayer_toets(-1,lay); 
//      end;
//      if {*}lay^<>oldlay then
//      begin 
//        FrontbufferButs(LongBool(1)); 
//        SetButs(4001,4020); 
//        FrontbufferButs(LongBool(0)); 
//      end;
//      if toets=ESCKEY then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      else
//      if toets=RETKEY then
//      begin 
//        ret:= 1; 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      else
//      if toets=MOUSEX)or(toets=MOUSEY then
//      begin 
//        getmouseco_sc(mval); 
//        if mval[0]<x1-40)or(mval[0]>x2+40)or(mval[1]<y1-30)or(mval[1]>y2+30 then
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      else
//      if toets=LEFTMOUSE then
//      begin 
//        getmouseco_sc(mval); 
//        FrontbufferButs(LongBool(1)); 
//        val:= DoButtons(); 
//        FrontbufferButs(LongBool(0)); 
//        if val<>0 then
//        begin 
//          if val=4000 then
//          begin 
//            ret:= 1; 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//          else
//          if val>4000)and(val<4032 then
//          begin 
//            FrontbufferButs(LongBool(1)); 
//            if {*}lay^=0)or((G.qual and LR_SHIFTKEY)=0 then
//            begin 
//              bit:= val-4001; {*}lay^:=1 shl bit; 
//              SetButs(4001,4031); 
//            end;
//            FrontbufferButs(LongBool(0)); 
//          end;
//        end;
//      end;
//    end;
//    if toets=0 then
//    usleep(1); 
//  end;
//  until not {0=}(get_mbut()@L_MOUSE);

//  FreeButBlock('button'); 
//endpupdraw(); 

//areawinset(curarea.win); 
//begin
//  result:= ret; 
//  exit;
//end;
//end;

//(* ********************** CLEVER_NUMBUTS ******************** *)

//const
//MAXNUMBUTS = 24; 

//var
//numbuts: array [0..Pred(MAXNUMBUTS)] of VarStruct; 
//numbpoin: array [0..Pred(MAXNUMBUTS)] of pinteger; 
//numbdata: array [0..Pred(MAXNUMBUTS)] of integer; 
//var {was static}
//last: pchar;
// (* avoid ugly updates! *)

//procedure draw_numbuts_tip(str: pchar;  x1: integer;  y1: integer;  x2: integer;  y2: integer); 
//var
//temp: integer; 
//begin
//  last:=0; (* avoid ugly updates! *)

//  if str=last then
//  exit;
//  last:= str; 
//  if str=0 then
//  exit;

//  glColor3ub(160,160,160); 
//  glRecti(x1+4,y2-36,x2-4,y2-16); 
//  (* MGREY *)
//  cpack($0); 

//  temp:= 0; 
//  while fmgetstrwidth(G.fonts,str+temp)>(x2-x1-24)
//  do
//  inc(temp); 
//  fmsetfont(G.fonts); 
//  glRasterPos2i(x1+16,y2-30); 
//  fmprstr(str+temp); 
//end;

//function do_clever_numbuts(name: pchar;  tot: integer;  winevent: integer): integer; 
//var
//varstr: pVarStruct; 
//bit: integer; 
//dx: integer; 
//dy: integer; 
//a: integer; 
//x1: integer; 
//x2: integer; 
//y1: integer; 
//y2: integer; 
//sizex: integer; 
//sizey: integer; 
//toets: ushort; 
//oldmap: array [0..Pred(4),0..Pred(3)] of smallint; 
//val: smallint; 
//mval: array [0..Pred(2)] of smallint; 
//ret: smallint;

//str: pchar; 
//begin

//  ret:=0; 

//  if tot<=0)or(tot>MAXNUMBUTS then
//  begin
//    result:= 0; 
//    exit;

//  end;
//  getmouseco_sc(mval); 
//  sizex:= 235; 
//  sizey:= 30+20*(tot+1); 
//  (* size *)
//  (* midden *)
//  if mval[0]<sizex div 2 then
//  mval[0]:= sizex div 2; 
//  if mval[1]<sizey div 2 then
//  mval[1]:= sizey div 2; 
//  if mval[0]>G.curscreen.sizex-sizex div 2 then
//  mval[0]:= G.curscreen.sizex-sizex div 2; 
//  if mval[1]>G.curscreen.sizey-sizey div 2 then
//  mval[1]:= G.curscreen.sizey-sizey div 2; 
//  winset(G.curscreen.mainwin); 

//  x1:= mval[0]-sizex div 2; 
//  x2:= mval[0]+sizex div 2; 
//  y1:= mval[1]-sizey div 2; 
//  y2:= mval[1]+sizey div 2; 
//  bgnpupdraw(x1,y1,x2,y2); 
//  glColor3ub(160,160,160); 
//  glRecti(x1,y1,x2,y2); 
//  (* MGREY *)
//  glColor3ub(0,0,0); 
//  sdrawbox(x1,y1,x2,y2); 
//  DefButBlock('numbuts',G.curscreen.mainwin,G.font,32,10,2); 
//  DefBut(LABEL,0,name,x1+5,y2-21,sizex-60,19,0,1.0,0.0,0,0); 
//  (* LET OP: TEX BUTTON UITZONDERING *)
//  (* WAARSCHUWING: ALLEEN EEN ENKELE BITJES-BUTTON MOGELIJK: ER WORDT OP KOPIEDATA GEWERKT! *)

//  (* 0.0 op eind: geen clear *)
//  if name[0]='A')and(name[7]='O' then
//  DefBut(LABEL,0,'Rotations in degrees!',x1+5,y2-36,sizex-60,19,0,0.0,0.0,0,0); 
//  varstr:= @numbuts[0]; (* 0.0 op eind: geen clear *)

//  while{for} 
//  a:= 0; 
//  {to} a<tot
//  {;}
//  inc(a); 
//  inc(varstr); 
//  do
//  begin 
//    if (varstr.type and BUTTYPE)=SLI then
//    DefButt(varstr.type,0,varstr.name,x1+50,y2-50-20*a,sizex-120,8,@(numbdata[a]),varstr.min,varstr.max,100,0,varstr.tip); 
//    else
//    if varstr.type=TEX then
//    DefButt(TEX,0,varstr.name,x1+15,y2-55-20*a,sizex-60,19,numbpoin[a],varstr.min,varstr.max,0,0,varstr.tip); 
//    else
//    begin 
//      DefButt(varstr.type,0,varstr.name,x1+15,y2-55-20*a,sizex-60,19,@(numbdata[a]),varstr.min,varstr.max,100,0,varstr.tip); 
//    end;
//  end;
//  DefButt(BUT,4000,'OK',x1+sizex-40,y1+15,25,sizey-50,0,0,0,0,0,'OK: Assign Values'); 

//  draw_numbuts_tip(0,0,0,0,0); 
//  (* init *)

//  while LongBool(1)
//  do
//  begin 
//    toets:= extern_qread(@val); 
//    if val<>0 then
//    begin 
//      if toets=ESCKEY then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      else

//      function ELEM{!!!3 unknown typedef}: if; 
//      begin
//        ret:= 1; 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end
//      else
//      if toets=MOUSEX)or(toets=MOUSEY then
//      begin 
//        getmouseco_sc(mval); 
//        if mval[0]<x1-120)or(mval[0]>x2+120)or(mval[1]<y1-50)or(mval[1]>y2+50 then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        if U.flag and TOOLTIPS then
//        begin 
//          str:= GetButTip(); 
//          if str<>0 then
//          draw_numbuts_tip(str,x1,y1,x2,y2); 
//        end;
//      end;
//      else
//      if toets=LEFTMOUSE then
//      begin 
//        getmouseco_sc(mval); 
//        FrontbufferButs(LongBool(1)); 
//        val:= DoButtons(); 
//        FrontbufferButs(LongBool(0)); 
//        if val<>0 then
//        begin 
//          if val=4000 then
//          begin 
//            ret:= 1; 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//        end;
//      end;
//    end;
//  end;
//  until not {0=}(get_mbut()@L_MOUSE);

//  FreeButBlock('button'); 
//endpupdraw(); 

//areawinset(curarea.win); 
//remake_qual(); 
//(* in Button.c zit geen extern_qread *)
//if ret=1 then
//begin 
//  varstr:= @numbuts[0]; 
//  while{for} 
//  a:= 0; 
//  {to} a<tot
//  {;}
//  inc(a); 
//  inc(varstr); 
//  do
//  begin 
//    if varstr.type=TEX then
//    ; 
//    else
//    if{!!!e unknown token}
//    memcpy(numbpoin[a],numbdata+a,4); 
//    else
//    if (varstr.type and BUTPOIN)=SHO then
//    *({!!!a type cast? =>} {psmallint(}(numbpoin[a])):=*({!!!a type cast? =>} {psmallint(}(numbdata+a)); 
//    if strncmp(varstr.name,'Rot',3)=0 then
//    begin 
//      var
//      fp: pfloat; 
//      fp:= numbpoin[a]; 
//      fp[0]:= M_PI*fp[0] div 180.0; 
//    end;
//  end;
//  if winevent<>0 then
//  begin 
//    sa: pScrArea; 
//    sa:= G.curscreen.areabase.first; 
//    while sa
//    do
//    begin 
//      if sa.spacetype=curarea.spacetype then
//      addqueue(sa.win,winevent,1); 
//      sa:= sa.next; 
//    end;
//  end;
//  begin
//    result:= 1; 
//    exit;
//  end;
//end;
//begin
//  result:= 0; 
//  exit;
//end;
//end;

//procedure add_numbut(nr: integer;  type: integer;  str: pchar;  min: float;  max: float;  poin: pinteger;  tip: pchar); 
//var
//fp: pfloat; 
//begin
//  if nr>=MAXNUMBUTS then
//  exit;

//  numbuts[nr].type:= type; 
//  strcpy(numbuts[nr].name,str); 
//  numbuts[nr].min:= min; 
//  numbuts[nr].max:= max; 
//  if tip<>0 then
//  strcpy(numbuts[nr].tip,tip); 
//  numbpoin[nr]:= poin; (* LET OP: TEX BUTTON UITZONDERING *)
//  if{!!!e unknown token}
//  memcpy(numbdata+nr,poin,4); 
//  if (type and BUTPOIN)=SHO then
//  *({!!!a type cast? =>} {psmallint(}(numbdata+nr)):=*({!!!a type cast? =>} {psmallint(}poin); 
//  if strncmp(numbuts[nr].name,'Rot',3)=0 then
//  begin 

//    fp:= {!!!a type cast? =>} {pfloat(}(numbdata+nr); 
//    fp[0]:= 180.0*fp[0] div M_PI; 
//  end;
//end;

//procedure clever_numbuts; 
//var
//ob: pObject; 
//lim: float; 
//str: array [0..Pred(128)] of char; 
//begin

//  if curarea.spacetype=SPACE_VIEW3D then
//  begin 
//    lim:= 1000.0*MAX2(1.0,G.vd.grid); 
//    if G.obedit=0 then
//    begin 
//      ob:= OBACT; 
//      if ob=0 then
//      exit;
//      add_numbut(0,NUM or FLO,'LocX:',-lim,lim,ob.loc,0); 
//      add_numbut(1,NUM or FLO,'LocY:',-lim,lim,ob.loc+1,0); 
//      add_numbut(2,NUM or FLO,'LocZ:',-lim,lim,ob.loc+2,0); 
//      add_numbut(3,NUM or FLO,'RotX:',-10.0*lim,10.0*lim,ob.rot,0); 
//      add_numbut(4,NUM or FLO,'RotY:',-10.0*lim,10.0*lim,ob.rot+1,0); 
//      add_numbut(5,NUM or FLO,'RotZ:',-10.0*lim,10.0*lim,ob.rot+2,0); 
//      add_numbut(6,NUM or FLO,'SizeX:',-lim,lim,ob.size,0); 
//      add_numbut(7,NUM or FLO,'SizeY:',-lim,lim,ob.size+1,0); 
//      add_numbut(8,NUM or FLO,'SizeZ:',-lim,lim,ob.size+2,0); 
//      sprintf(str,'Object: %s',ob.id.name+2); 
//      do_clever_numbuts(str,9,REDRAW); 
//    end;
//    else
//    if{!!!e unknown token}
//    clever_numbuts_mesh(); 
//    else
//    if{!!!e unknown token}
//    clever_numbuts_curve(); 
//  end;
//  else
//  if curarea.spacetype=SPACE_IPO then
//  begin 
//    clever_numbuts_ipo(); 
//  end;
//  else
//  if curarea.spacetype=SPACE_SEQ then
//  begin 
//    clever_numbuts_seq(); 
//  end;
//end;

//procedure replace_names_but; 
//var
//ima: pImage;

//len: smallint; 
//tot: smallint;

//old: array [0..Pred(64)] of char; 
//new: array [0..Pred(64)] of char; 
//temp: array [0..Pred(80)] of char; 
//begin
//  ima:=G.main.image.first; 

//  tot:=0; 

//  strcpy(old,'/'); 
//  strcpy(new,'/'); 
//  add_numbut(0,TEX,'Old:',0,63,old,0); 
//  add_numbut(1,TEX,'New:',0,63,new,0); 

//  if do_clever_numbuts('Replace image name',2,REDRAW) then
//  begin 
//    len:= lstrlen(old); 
//    while ima
//    do
//    begin 
//      if strncmp(old,ima.name,len)=0 then
//      begin 
//        strcpy(temp,new); 
//        strcat(temp,ima.name+len); 
//        strncpy(ima.name,temp,79); 
//        if ima.ibuf<>0 then
//        freeImBuf(ima.ibuf); 
//        ima.ibuf:= 0; 
//        ima.ok:= 1; 
//        inc(tot); 
//      end;
//      ima:= ima.id.next; 
//    end;
//    sprintf(temp,'Replaced %d names',tot); 

//    notice(temp); 
//  end;
//end;

//{glIndexi(i) ColorFunc(i)}
//function glIndexi(i: integer): integer; 
//begin
//  result:= ColorFunc(i)
//end;

end.
