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

(* screen.c  dec/jan 93/94  GRAPHICS
 *
 *
 *
 *
 *
 *)
unit screen;

interface
(*
 *
 * LET OP: util.h (ListBase) en graphics.h (vec2s) ook nodig
 *
 *)

uses
  cfuncs,
  util, blender, arithb;

const
  MAXWIN = 128;
{ Als je EDGEWIDTH verandert, ook globale array edcol[] goedzetten  }
  EDGEWIDTH = 5;
  EDGEWIDTH2 = 2;
  AREAGRID = 4;
  AREAMINX = 32;
  HEADERY = 21;
  AREAMINY = HEADERY+EDGEWIDTH;
  NOHEADER = 0;
  HEADERDOWN = 1;
  HEADERTOP = 2;
{ sa->win_swap  }
  WIN_FRONT_OK = 1;
  WIN_BACK_OK = 2;
  WIN_EQUAL = 3;
{ left scrollbar  }
  L_SCROLL = 1;
  R_SCROLL = 2;
  VERT_SCROLL = 3;
  T_SCROLL = 4;
  B_SCROLL = 8;
  HOR_SCROLL = 12;
{ deze getallen moeten kleiner dan 100 zijn ivm buttoncodes  }
  SPACE_EMPTY = 0;
  SPACE_VIEW3D = 1;
  SPACE_IPO = 2;
  SPACE_OOPS = 3;
  SPACE_BUTS = 4;
  SPACE_FILE = 5;
  SPACE_IMAGE = 6;
  SPACE_INFO = 7;
  SPACE_SEQ = 8;
  SPACE_TEXT = 9;
  SPACE_IMASEL = 10;
  SPACE_PAINT = 11;
  MAXQUEUE = 256;
{ queue events: naar blendef.h  }
{ framebuffer coords  }
{ alleen voor pupmenu  }

type
  pbScreen = ^bScreen;
  bScreen = record
      id : ID;
      vertbase : ListBase;
      edgebase : ListBase;
      areabase : ListBase;
      scene : ^Scene;
      startx : smallint;
      endx : smallint;
      starty : smallint;
      endy : smallint;
      sizex : smallint;
      sizey : smallint;
      scenenr : smallint;
      screennr : smallint;
      full : smallint;
      rt : smallint;
      mainwin : smallint;
      winakt : smallint;
    end;

  ppScrVert = ^pScrVert;
  pScrVert = ^ScrVert;
  ScrVert = record
      next : ^ScrVert;
      prev : ^ScrVert;
      _new : ^ScrVert;
      vec : vec2s;
      flag : longint;
    end;
{ 1 als op rand screen  }

pScrEdge = ^ScrEdge;
  ScrEdge = record
      next : ^ScrEdge;
      prev : ^ScrEdge;
      v1 : ^ScrVert;
      v2 : ^ScrVert;
      border : smallint;
      flag : smallint;
      pad : longint;
    end;
{ als area==full, dit is de parent  }
{ 0=niets, 1= down, 2= up  }
{ SPACE_...   }
{ size  }
{ opmerking: fuctiepointers met types binnen de halen geeft problemen met SDNA  }

pScrArea = ^ScrArea;
  ScrArea = record
      next : ^ScrArea;
      prev : ^ScrArea;
      v1 : ^ScrVert;
      v2 : ^ScrVert;
      v3 : ^ScrVert;
      v4 : ^ScrVert;
      full : ^bScreen;
      headqueue : ^smallint;
      hq : ^smallint;
      winqueue : ^smallint;
      wq : ^smallint;
      winmat : array[0..3] of array[0..3] of single;
      totrct : rcti;
      headrct : rcti;
      winrct : rcti;
      headwin : smallint;
      win : smallint;
      headertype : smallint;
      spacetype : byte;
      butspacetype : byte;
      winx : smallint;
      winy : smallint;
      head_swap : byte;
      head_equal : byte;
      win_swap : byte;
      win_equal : byte;
      headbutlen : smallint;
      headbutofs : smallint;
      cursor : smallint;
      rt : smallint;
      headchange : procedure ;
      winchange : procedure ;
      headdraw : procedure ;
      windraw : procedure ;
      headqread : procedure ;
      winqread : procedure (event: word;  val: smallint);
      spacedata : ListBase;
    end;
{ ********** MYWINDOW *******  }
(* error
#
(* error
#
in declaration at line 155 *)*)

  pbWindow = ^bWindow;
  bWindow = record
    next, prev: pbWindow;
	id, parent_id: longint;

	xmin, xmax, ymin, ymax: longint;
	viewmat, winmat: array[0..3, 0..3] of single;
  end;

var
winset: procedure(wid: integer);
  winget: function :longint;
  //  ortho = myortho;
ortho2: procedure(x1, x2, y1, y2: single);

  { was #define dname(params) para_def_expr }
  { argument types are unknown }
  { return type might be wrong }

  //function window(a,b,c,d,e,f : longint) : longint;

var
  winclose: procedure (winid: integer);
//  loadmatrix = myloadmatrix;
//  getmatrix = mygetmatrix;
//  multmatrix = mymultmatrix;
//  getsize = mygetsize;
//  getorigin = mygetorigin;

  //var
  //  swindowbase : ListBase;cvar;external;
{ uit mywindow.c  }
{ ********** END MYWINDOW *******  }
{ GLOBALS   (uit screen.c)  }
    //curarea : ^ScrArea;cvar;external;
    //displaysizex : longint;cvar;external;
{ mywindow.c  }

//function get_qual:smallint;
//
//function get_mbut:smallint;
//
//procedure myglClearColor(a:single; b:single; c:single; d:single);
//
//procedure custom_cursor(data1:Pchar; data2:Pchar; size:longint);
//
//procedure drawmode(mode:longint);
//
//function fmfindfont(name:Pchar):plongint;
//
//function fmgetstrwidth(handle:pointer; str:Pchar):longint;
//
//procedure fminit;
//
//procedure fmprstr(str:Pchar);
//
//procedure fmrotatepagematrix(degrees:single);
//
//function fmscalefont:plongint;
//
//procedure fmsetfont(_type:pointer);
//
//function getbutton(val:longint):smallint;
//
//procedure getmouse(mval:Psmallint);
//
//function getqual:smallint;
//
//function give_new_winid:longint;
//
//procedure init_my_mainwin(win:longint);
//
//procedure loadmatrix_win(var mat:array[0..3] of single; swin:longint);
//
//procedure myContextSetup;
//
//procedure myCopySubBuffer(x:longint; y:longint; sx:longint; sy:longint);
//
//procedure myXcolor(col:longint);
//
//procedure myXdrawline(x1:longint; y1:longint; x2:longint; y2:longint);
//
//procedure myXrect(x1:longint; y1:longint; x2:longint; y2:longint);
//
//procedure myXrectf(x1:longint; y1:longint; x2:longint; y2:longint);
//
//procedure my_get_frontbuffer_image(x:longint; y:longint; sx:longint; sy:longint);
//
//procedure my_put_frontbuffer_image;
//
//procedure mygetmatrix(mat:Parray[0..3] of single);
//
//procedure mygetorigin(x:Plongint; y:Plongint);
//
//procedure mygetsingmatrix(mat:Parray[0..3] of single);
//
//procedure mygetsize(x:Plongint; y:Plongint);
//
//procedure mygetsuborigin(x:Plongint; y:Plongint);
//
//procedure myloadmatrix(mat:Parray[0..3] of single);
//
//procedure mymultmatrix(mat:Parray[0..3] of single);
//
//procedure myortho(x1:single; x2:single; y1:single; y2:single; n:single;
//            f:single);
//
//procedure myortho2(x1:single; x2:single; y1:single; y2:single);
//
//procedure myswapbuffers;
//
//function myswinopen(winid:longint; xmin:longint; xmax:longint; ymin:longint; ymax:longint):longint;
//
//procedure mywinclose(winid:longint);
//
//procedure mywindow(x1:single; x2:single; y1:single; y2:single; n:single;
//            f:single);
//
//function mywinexist(wid:longint):longint;
//
//procedure mywinposition(swin:longint; xmin:longint; xmax:longint; ymin:longint; ymax:longint);
//
//{ let op: andere syntax  }
//procedure mywinset(wid:longint);
//
//procedure overdrawdump;
//
//procedure screendump;
//
//procedure sdrawXORline(x0:longint; y0:longint; x1:longint; y1:longint);
//
//procedure sdrawXORline4(nr:longint; x0:longint; y0:longint; x1:longint; y1:longint);
//
//procedure setlinestyle(nr:longint);
//
//procedure setvaluator(dev:smallint; a:smallint; b:smallint; c:smallint);
//
//procedure write_screendump(name:Pchar);
//
//{ screen.c  }
//procedure addafterqueue(win:smallint; event:word; val:smallint);

procedure addqueue(win:smallint; event:word; val:smallint);

//function addscreen(name:Pchar):^bScreen;
//
//{ gebruik de setprefsize(); als je anders dan fullscreen wilt  }
//function afterqtest:longint;
//
//procedure append_afterqueue;
//
//procedure area_fullscreen;
//
//{ met curarea  }
//procedure areawinset(win:smallint);
//
//procedure calc_arearcts(sa:pScrArea);
//
//procedure closeareawin(sa:pScrArea);
//
//procedure closeheadwin(sa:pScrArea);
//
//function closest_bigger_area:^ScrArea;
//
//procedure copy_areadata(sa1:pScrArea; sa2:pScrArea);
//
//procedure copy_screen(to:PbScreen; from:PbScreen);
//
//procedure decodekeytab;
//
//function default_foursplit:^bScreen;
//
//function default_twosplit:^bScreen;
//
//procedure defheadchange;

procedure defheaddraw;

//procedure defheadqread(sa:pScrArea);
//
//procedure defwinchange;
//
//procedure defwindraw;
//
//procedure defwinmat;
//
//procedure defwinqread(sa:pScrArea);
//
//procedure del_area(sa:pScrArea);
//
//procedure do_the_glut_funcs;
//
//procedure drawedge(x1:smallint; y1:smallint; x2:smallint; y2:smallint);
//
//procedure drawscredge(se:PScrEdge);
//
//procedure drawscreen;
//
//procedure duplicate_screen;
//
//procedure editsplitpoint;
//
//function ext_qtest:smallint;
//
//{ extern      word special_qread(short *val);  }
function extern_qread(val: psmallint): word;

//function findcurarea:^ScrArea;
//
//function findscredge(v1:PScrVert; v2:PScrVert):^ScrEdge;
//
//procedure free_screen(sc:PbScreen);

procedure getdisplaysize;

//procedure getmouseco_areawin(mval:Psmallint);
//
//{ interne area coordinaten  }
//procedure getmouseco_headwin(mval:Psmallint);

{ interne area coordinaten  }
procedure getmouseco_sc(mval:psmallint);

//{ screen coordinaten  }
//procedure header_back_to_front(sa:pScrArea);
//
//procedure header_front_to_back(sa:pScrArea);
//
//procedure headerbox(selcol:longint; width:longint);
//
//procedure headmenu(sa:pScrArea);

procedure init_screen_cursors;

procedure initscreen;

//procedure inputchangefunc(state:longint);
//
//function is_allowed_to_change_screen(_new:PbScreen):longint;
//
//function isqueued(dev:smallint):smallint;
//
//procedure joinarea(sa:pScrArea);
//
//procedure keyboardfunc(key:uchar; x:longint; y:longint);
//
//procedure markdirty_all;
//
//procedure mousefunc(button:longint; state:longint; x:longint; y:longint);
//
//procedure mousemovefunc(x:longint; y:longint);
//
//procedure moveareas;
//
//function myqtest:word;
//
//function mywinopen(mode:longint; posx:smallint; posy:smallint; sizex:smallint; sizey:smallint):longint;
//
//procedure openareawin(sa:pScrArea);
//
//procedure openheadwin(sa:pScrArea);

procedure qenter(event:word; val:smallint);

//function qread(val:Psmallint):word;
//
//procedure qreset;
//
//function qtest:word;
//
//procedure rawkeyfunc(key:longint; val:longint; y:longint);
//
//procedure redraw1func;
//
//procedure redraw2func;
//
//procedure remake_qual;
//
//procedure removedouble_scredges;
//
//procedure removedouble_scrverts;
//
//procedure removenotused_scredges;
//
//procedure removenotused_scrverts;
//
//procedure reset_autosave;
//
//procedure reshapefunc(x:longint; y:longint);
//
//procedure rgbmul(col:Puint; mul:longint);
//
//function screen_qread(val:Psmallint):word;
//
//procedure screen_swapbuffers;
//
//procedure screenmain;
//
//procedure scrollheader;
//
//function select_area(spacetype:longint):longint;
//
//procedure set_cursonedge(mx:smallint; my:smallint);
//
//procedure setcursor_space(spacetype:longint; cur:smallint);
//
//procedure setdisplaysize(ex:longint; ey:longint);

procedure setprefsize(stax, stay, sizx, sizy:longint);

procedure setscreen(sc:pbScreen);

//procedure splitarea(sa:pScrArea; dir:char; fac:single);

procedure start_autosave(val:longint); cdecl;

//procedure tempcursor(curs:longint);
//
//function test_edge_area(sa:pScrArea; se:PScrEdge):^ScrArea;
//
//procedure test_scale_screen(sc:PbScreen);
//
//procedure testareas;
//
//function testsplitpoint(sa:pScrArea; dir:char; fac:single):smallint;
//
//procedure visiblefunc(dummy:longint);

procedure waitcursor(val:longint);

procedure wich_cursor(sa:pScrArea);

//procedure window_front_to_back(sa:pScrArea);
//
//{ space.c  }
//procedure allqueue(event:word; val:smallint);
//
//procedure allspace(event:word; val:smallint);
//
//procedure changebutspace;
//
//procedure changeview2d;
//
//procedure changeview3d;
//
//procedure copy_view3d_lock(val:smallint);
//
//procedure drawemptyspace;
//
//procedure drawinfospace;
//
//procedure duplicatespacelist(lb1:pListBase; lb2:pListBase);
//
//procedure extern_set_butspace(fkey:longint);
//
//procedure force_draw;
//
//procedure freespacelist(lb:pListBase);
//
//procedure handle_view3d_lock;
//
//procedure init_butspace(sa:pScrArea);
//
//procedure init_filespace(sa:pScrArea);
//
//procedure init_imagespace(sa:pScrArea);
//
//procedure init_oopsspace(sa:pScrArea);
//
//procedure init_seqspace(sa:pScrArea);
//
//procedure init_v2d_oops(v2d:PView2D);
//
//procedure initipo(sa:pScrArea);
//
//procedure initview3d(sa:pScrArea);
//
//procedure newspace(sa:pScrArea; _type:longint);
//
//procedure set_func_space(sa:pScrArea);
//
//procedure set_rects_butspace(buts:PSpaceButs);
//
//procedure winqread3d(event:word; val:smallint);
//
//procedure winqreadbutspace(event:word; val:smallint);
//
//procedure winqreadimagespace(event:word; val:smallint);
//
//procedure winqreadinfospace(event:word; val:smallint);
//
//procedure winqreadipo(event:word; val:smallint);
//
//procedure winqreadoopsspace(event:word; val:smallint);
//
//procedure winqreadsequence(event:word; val:smallint);

var
displaysizex: integer =0;
displaysizey: integer =0;
fullscreen: word = 1;
borderless: word = 1;
curarea: pScrArea = nil;

implementation

uses
  GL, GLU, GLUT,
  blenglob, library_, blendef, mywindow, graphics, mydevice,
  edit, initrender,
  space, Button;

{ was #define dname(params) para_def_expr }
{ argument types are unknown }
{ return type might be wrong }
//function window(a,b,c,d,e,f : longint) : longint;
//begin
//  window:=mywindow(a,b,c,d,e,f);
//end;

procedure myglClearColor(a:single; b:single; c:single; d:single);
begin
{ You must implement this function }
end;

////{$include "graphics.h"}
////{$include "render.h"}
//{$ifdef __sgi}
////{$include <sys/syssgi.h>}
//{$endif}
//(* #ifndef FREE *)
//(* #include "/usr/people/ton/code.h" *)
//(* #endif *)
//(* TIPS:
// * 
// * - LET OP DE EDGES,  VERTICES ERVAN MOETEN IN VOLGORDE
//  (laagste pointer eerst). Anders onvoorspelbare effecten!
// * - probleem: flags zijn nog niet echt netjes. Altijd na gebruik
//  op nul zetten.
///* ************************* *)
//(* ********* Globals *********** *)
//{swinarray: array [0..] of pbWindow; }{<= !!!5 external variable}(* mywindow.c *)
var
//curarea: pScrArea = nil;
curedge: pScrEdge = nil;
prefsizx: integer =0;
prefsizy: integer =0;
prefstax: integer =0;
prefstay: integer =0;
scrmousex: smallint;
scrmousey: smallint; (* worden gezet door mousecallback *)
mainwin: smallint =0;
winqueue_break: smallint =0;
//cursonedge: smallint =0;
//keycode: array [0..Pred(100)] of smallint; 
//var {was static}
mainqueue: array [0..Pred(MAXQUEUE)] of smallint;
//var
areawinar: array [0..Pred(MAXWIN)] of pScrArea;

(* uint edcol[EDGEWIDTH]= {0x0, 0x303030, 0x606060, 0x808080, 0x909090, 0xF0F0F0, 0x0}; *)
edcol: array [0..EDGEWIDTH - 1] of cardinal = ($0,$505050,$909090,$F0F0F0,$0);

autosavetime: integer;

(* ********* Funkties *********** *)

//procedure getdisplaysize; 
//
//function default_foursplit: pbScreen; 
//
//function default_twosplit: pbScreen; 
//
//function findscredge(v1: pScrVert;  v2: pScrVert): pScrEdge; 
//
//procedure setscreen(sc: pbScreen); 

procedure drawscreen; forward;

//procedure initscreen; 
//
//procedure moveareas; 
//
//procedure scrollheader; 
//
//procedure testareas; 
//
//procedure area_fullscreen; 
//
//procedure addqueue(win: smallint;  event: word;  val: smallint);
//
//procedure editsplitpoint; 
//
//procedure splitarea(sa: pScrArea;  dir: char;  fac: single);
//
//procedure joinarea(sa: pScrArea);

procedure select_connected_scredge(sc: pbScreen;  edge: pScrEdge); forward;

//var {was static}
//hor_ptr_bits: array [0..] of word = ($0000,$0000,$0000,$0000,$1008,$1818,$1c38,$ffff,$ffff,$1c38,$1818,$1008,$0000,$0000,$0000,$0000,$0000,$0000,$0000,$381c,$3c3c,$3e7c,$ffff,$ffff,$ffff,$ffff,$3e7c,$3c3c,$381c,$0000,$0000,$0000,);
//vert_ptr_bits: array [0..] of word = ($0180,$0180,$0180,$0ff0,$07e0,$03c0,$0180,$0,$0,$0180,$03c0,$07e0,$0ff0,$0180,$0180,$0180,$03c0,$03c0,$1ff8,$1ff8,$1ff8,$0ff0,$07e0,$03c0,$03c0,$07e0,$0ff0,$1ff8,$1ff8,$1ff8,$03c0,$03c0,);
//win_ptr_bits: array [0..] of word = ($0000,$0180,$0180,$0180,$0000,$89b3,$fdb3,$fdb7,$b5bf,$b5bb,$85b3,$0000,$0180,$0180,$0180,$0000,$03c0,$03c0,$03c0,$03c0,$FFFF,$FFFF,$FFFF,$FFFF,$FFFF,$FFFF,$FFFF,$FFFF,$03c0,$03c0,$03c0,$03c0);
//implementation


procedure init_screen_cursors;
begin
end;

procedure waitcursor(val: integer);
var
sa: pScrArea;
oldwin: integer;
begin
  if G.curscreen=nil then
  exit;
  if G.background<>0 then
  exit;

  if val<>0 then
  begin
    (* only once: for time-cursor *)
    //if (R.flag and R_RENDERING) <> 0 then
    //exit;
    //
    //if (R.win<>nil)and(R.win=G.curscreen^.winakt) then
    //begin
    //  oldwin:= G.curscreen^.winakt;
    //  mywinset(G.curscreen^.mainwin);
    //  glutSetCursor(CURSOR_WAIT);
    //  mywinset(oldwin);
    //end
    //else
    glutSetCursor(CURSOR_WAIT);
  end
  else
  if G.curscreen^.winakt>3 then
  begin
    //if (R.flag and R_RENDERING)<>0 then
    //exit;
    sa:= areawinar[G.curscreen^.winakt];
    if (sa^.cursor<>0)and(sa^.win=G.curscreen^.winakt) then
    glutSetCursor(sa^.cursor)
    else
    glutSetCursor(CURSOR_STD);
  end
  else
  glutSetCursor(CURSOR_STD);
end;

//procedure tempcursor(curs: integer); 
//var
//sa: pScrArea;
//begin
//  
//  if G.curscreen=nil
// then
//  exit;
//  if curs<>CURSOR_STD
// then
//  glutSetCursor(curs); 
//  else
//  if G.curscreen^.winakt<>nil 
// then
//  begin 
//    sa:= areawinar[G.curscreen^.winakt]; 
//    if sa^.win=G.curscreen^.winakt
// then
//    glutSetCursor(sa^.cursor); 
//    else
//    glutSetCursor(CURSOR_STD); 
//  end;
//end;

procedure wich_cursor(sa: pScrArea);
begin
  sa^.cursor:= CURSOR_STD; (* o.a. als nieuwe space gemaakt *)

  if sa^.spacetype=SPACE_VIEW3D then
  begin
    if G.obedit<>nil then
    sa^.cursor:= CURSOR_EDIT
    else
    if (G.f and G_VERTEXPAINT) <>0 then
    sa^.cursor:= CURSOR_VPAINT
    else
    if (G.f and G_FACESELECT) <>0 then
    sa^.cursor:= CURSOR_FACESEL;
  end;

  if sa^.win=G.curscreen^.winakt then
  glutSetCursor(sa^.cursor);
end;

//procedure setcursor_space(spacetype: integer;  cur: smallint); 
//var
//sc: pbScreen; 
//sa: pScrArea;
//oldwin: integer; 
//begin
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
//      if sa^.spacetype=spacetype
// then
//      begin 
//        sa^.cursor:= cur; 
//        if cur=nil
// then
//        wich_cursor(sa); 
//        (* extra test, bv nodig bij verlaten editmode in vertexpaint *)
//        if sc=G.curscreen)and(sc^.winakt=sa^.win
// then
//        glutSetCursor(sa^.cursor); 
//      end;
//      sa:= sa^.next; 
//    end;
//    sc:= sc^.id.next;
//  end;
//end;

(* *********** CODETAB ******************* *)

//procedure decodekeytab; 
//var
//ihash: array [0..Pred(512)] of char; 
//a: integer; 
//b: integer; 
//file: integer; 
//c: pchar; 
//hus1: char; 
//hus2: char; 
//sl: array [0..Pred(MAXSYSIDSIZE)] of char; 
//str: array [0..Pred(30)] of char; 
//begin
//  {$ifdef __sgi}
//  {hash: array [0..Pred(512)] of char; }{<= !!!5 external variable}
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  file:= open('/.Bcode',O_BINARY or O_RDONLY); 
//  if file=-1
// then
//  exit(0); 
//  read(file,keycode,200); 
//  close(file); 
//  syssgi(SGI_SYSID,sl); 
//  (* inverse hashtab *)
//  for{while} a:=nil to Pred(256) {a++}
//  do
//  begin 
//    for{while} b:=nil to Pred(256) {b++}
//    do
//    begin 
//      if a=hash[b]
// then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    ihash[a]:= b; 
//  end;
//  for{while} a:=nil to Pred(256) {a++}
//  do
//  ihash[256+a]:= ihash[a]; 
//  hus1:= hash[sl[0]+hash[sl[2]]]; 
//  hus2:= hash[sl[1]+hash[sl[3]]]; 
//  c:=  {pchar(}keycode; (* bereken hus1 en hus2 ahv sleutel *)
//  for{while} a:=nil to Pred(100) { a++}
//  do
//  begin 
//    c[0]:= (ihash[(ihash[c[0]]-hus1) and 255]) and 255; 
//    c[1]:= (ihash[(ihash[c[1]]-hus2) and 255]) and 255; 
//    c:= c + (2); 
//  end;
//  {$endif}
//  (* __sgi *)
//end;

(* *********  IN/OUT  ************* *)


procedure getmouseco_sc(mval: psmallint);  (* screen coordinaten *)
begin
  if G.curscreen=nil then
  exit;

  getmouse(mval); (* display coordinaten *)

  mval[0]:= mval[0] - (G.curscreen^.startx);
  mval[1]:= mval[1] - (G.curscreen^.starty);
end;

//(* interne area coordinaten *)
//
//procedure getmouseco_areawin(mval: psmallint); 
//begin
//  getmouseco_sc(mval); 
//  if curarea^.win<>nil 
// then
//  begin 
//    mval[0]:= mval[0] - (curarea^.winrct.xmin); 
//    mval[1]:= mval[1] - (curarea^.winrct.ymin); 
//  end;
//end;
//(* interne area coordinaten *)
//
//procedure getmouseco_headwin(mval: psmallint); 
//begin
//  getmouseco_sc(mval); 
//  if curarea^.headwin<>nil 
// then
//  begin 
//    mval[0]:= mval[0] - (curarea^.headrct.xmin); 
//    mval[1]:= mval[1] - (curarea^.headrct.ymin); 
//  end;
//end;

(* ********  WINDOW & QUEUE ********* *)

var
mainq: psmallint = @mainqueue;

function qread(val: psmallint): word;
begin
  if PtrUint(mainq)>PtrUint(@mainqueue ) then
  begin
    mainq:= mainq - (2);

    val^:=mainq[1];
      exit(mainq^);
  end;

  exit(0);
end;

procedure qenter(event: word;  val: smallint);
var
size: integer;
end_: psmallint;
begin
  (* avoid non-events: qtest()! *)
  if event<>0 then
  begin
  end_:= @mainqueue[MAXQUEUE-2];
  if PtrUint(mainq)<PtrUint(end_) then
  begin
    size:= mainq-@mainqueue;

    if size<>0 then
    memmove(@mainqueue[2],@mainqueue[0],size);
    mainqueue[0]:=event;
    mainqueue[1]:= val;

    mainq:= mainq + (2);
  end;
end;
end;

function myqtest: word;
begin
  if PtrUint(mainq)<>PtrUint(@mainqueue) then
    exit((mainq-2)^);

    exit(0);
end;

function qtest: word;
var
event: word;
begin
  {$if not defined(BEOS) and not defined(WINDOWS)}
  	extern Display *__glutDisplay;

  	/* combinatie: deze werkt als oude qtest(). wel daarna screen_qread aanroepen */

  	if(event=myqtest()) return event;
  	return (XPending(__glutDisplay));
  {$else}

  	if(event=myqtest()) then 
      exit(event);
  	//todo: uncomment
        //exit(glutqtest);

  {$endif}
end;

procedure qreset;
begin
  mainq:= mainqueue;
end;

function isqueued(dev: smallint): smallint;
begin
    exit(0);
end;

(* *********** AUTOSAVE ************** *)

procedure reset_autosave;
begin
  autosavetime:=0;
end;

procedure start_autosave(val: integer); cdecl;
begin
  (* opniew aanzetten *)
  glutTimerFunc(60000, @start_autosave, 0);

  if (U.flag and AUTOSAVE) <> 0 then
  begin
    inc(autosavetime);
    if autosavetime>=U.savetime then
    begin
      qenter(TIMER0,1);
    end;
  end
  else
  autosavetime:=0;
end;

//procedure set_cursonedge(mx: smallint;  my: smallint); 
//var
//se: pScrEdge; 
//se1: pScrEdge;
// 
//se2: pScrEdge;
// 
//se3: pScrEdge;
// 
//se4: pScrEdge;
// 
//dist: single;
//mindist: single;
// 
//vec1: array [0..1] of single;
//vec2: array [0..1] of single;
//vec3: array [0..1] of single;
//begin
//  
//  se1:=nil; 
//  se2:=nil; 
//  se3:=nil; 
//  se4:=nil; 
//  
//  mindist:=100.0; 
//  
//  
//  
//  
//  function PdistVL2Dfl: single;
//  vec1[0]:= mx; 
//  vec1[1]:= my; 
//  curedge:=nil; (* als de edge element is van curarea: extra voordeel. Dit voor juiste split en join  *)
//  if curarea<>nil 
// then
//  begin 
//    se1:= findscredge(curarea^.v1,curarea^.v2); 
//    se2:= findscredge(curarea^.v2,curarea^.v3); 
//    se3:= findscredge(curarea^.v3,curarea^.v4); 
//    se4:= findscredge(curarea^.v4,curarea^.v1); 
//  end;
//  se:= G.curscreen^.edgebase.first; 
//  while se
//  do
//  begin 
//    vec2[0]:= se^.v1^.vec.x; 
//    vec2[1]:= se^.v1^.vec.y; 
//    vec3[0]:= se^.v2^.vec.x; 
//    vec3[1]:= se^.v2^.vec.y; 
//    dist:= PdistVL2Dfl(vec1,vec2,vec3); 
//    if se=se1)or(se=se2)or(se=se3)or(se=se4
// then
//    dist:= dist - (5.0); 
//    if dist<mindist
// then
//    begin 
//      mindist:= dist; 
//      curedge:= se; 
//    end;
//    se:= se^.next; 
//  end;
//  if curedge=nil
// then
//  exit;
//  cursonedge:= 1; 
//  if curedge^.v1^.vec.x=curedge^.v2^.vec.x
// then
//  glutSetCursor(GLUT_CURSOR_LEFT_RIGHT); 
//  else
//  glutSetCursor(GLUT_CURSOR_UP_DOWN); 
//end;

procedure areawinset(win: smallint);
var
sa: pScrArea = nil;
sseq: pSpaceSeq;
begin
  if win>3 then
  begin
    curarea:= areawinar[win];

    if curarea=nil then
    begin
      printf('error in areawinar %d ,areawinset\n',[win]);
      exit;
    end;

    case curarea^.spacetype of
      SPACE_VIEW3D:
      begin
        G.vd:= curarea^.spacedata.first;
      end;
      SPACE_IPO:
      begin
        if G.sipo<>curarea^.spacedata.first
 then
        allqueue(REDRAWBUTSANIM,0);
        G.sipo:= curarea^.spacedata.first;
        G.v2d:= @G.sipo^.v2d;
      end;
      SPACE_BUTS:
      begin
        G.buts:= curarea^.spacedata.first;
        G.v2d:= @G.buts^.v2d;
      end;
      SPACE_SEQ:
      begin
        sseq:= curarea^.spacedata.first;
        G.v2d:= @sseq^.v2d;
        {goto next_label;}{<= !!!d case label without "break"}
      end;
      SPACE_OOPS:
      begin
        G.v2d:= @(pSpaceOops(curarea^.spacedata.first)^.v2d);
        G.soops:= curarea^.spacedata.first;
      end;
      SPACE_IMASEL:
      begin
      end;
      SPACE_IMAGE:
      begin
        G.v2d:= @(pSpaceImage(curarea^.spacedata.first)^.v2d);
        G.sima:= curarea^.spacedata.first;
      end;
      SPACE_PAINT:
      begin
      end;
      SPACE_FILE:
      begin
      end;
      SPACE_TEXT:
      begin
        G.stext:= curarea^.spacedata.first;
      end;

    end;
  end;
  if win<>0 then
  mywinset(win);
end;

procedure headerbox(selcol: integer;  width: integer);
begin
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  if selcol<>0 then
  glClearColor(0.7,0.7,0.6,0.0)
  else
  glClearColor(0.6,0.6,0.6,0.0);

  glClear(GL_COLOR_BUFFER_BIT);
  glColor3ub(0,0,0);
  sdrawbox(0,0,width,HEADERY);
  glColor3ub(220,220,220);
  sdrawline(0,HEADERY-1,width,HEADERY-1);
  glColor3ub(176,176,176);
  sdrawline(0,HEADERY-2,width,HEADERY-2);
  glColor3ub(128,128,128);
  sdrawline(0,2,width,2);
  glColor3ub(64,64,64);
  sdrawline(0,1,width,1);
  glColor3ub(0,0,0);
  sdrawline(0,0,width,0);
end;

procedure defheaddraw;
var
selcol: integer; (* aktieve kleur *)
begin
  selcol:=0;
  if G.curscreen^.winakt<>0 then
  begin
    if curarea^.headwin=G.curscreen^.winakt then
    selcol:= 1
    else
    if curarea^.win=G.curscreen^.winakt then
    selcol:= 1;
  end;

  headerbox(selcol,curarea^.winx+100);

  (* buttons, functies in window.c *)
  case curarea^.spacetype of
    SPACE_EMPTY:
    begin
    end;
//    SPACE_FILE:
//    begin
//      file_buttons(); 
//    end;
//    SPACE_INFO:
//    begin
//      info_buttons(); 
//    end;
//    SPACE_VIEW3D:
//    begin
//      view3d_buttons(); 
//    end;
//    SPACE_IPO:
//    begin
//      ipo_buttons(); 
//    end;
//    SPACE_BUTS:
//    begin
//      buts_buttons(); 
//    end;
//    SPACE_SEQ:
//    begin
//      seq_buttons(); 
//    end;
//    SPACE_IMAGE:
//    begin
//      image_buttons(); 
//    end;
//    SPACE_IMASEL:
//    begin
//      imasel_buttons(); 
//    end;
//    SPACE_OOPS:
//    begin
//      oops_buttons(); 
//    end;
//    SPACE_PAINT:
//    begin
//    end;
//    SPACE_TEXT:
//    begin
//      text_buttons(); 
//    end;

  end;
  curarea^.head_swap:= WIN_BACK_OK;
end;

procedure defwindraw;
begin
  if (curarea^.win<>0)and(curarea^.windraw <>nil) then
  begin
    curarea^.windraw();
  end
  else
  begin
    glClearColor(0.4375,0.4375,0.4375,0.0);
    glClear(GL_COLOR_BUFFER_BIT);
  end;
  curarea^.win_swap:= WIN_BACK_OK;
end;

//procedure defheadchange; 
//var
//ofs: single;
//begin
//  
//  if curarea^.headchange<>nil 
// then
//  begin 
//    curarea^.headchange(); 
//  end;
//  else
//  begin 
//    ofs:= curarea^.headbutofs; 
//    if curarea^.headertype=HEADERDOWN
// then
//    ortho2(-0.5+ofs,curarea^.headrct.xmax-curarea^.headrct.xmin-0.5+ofs,+0.6,curarea^.headrct.ymax-curarea^.headrct.ymin+0.6); 
//    else
//    ortho2(-0.5+ofs,curarea^.headrct.xmax-curarea^.headrct.xmin-0.5+ofs,-0.5,curarea^.headrct.ymax-curarea^.headrct.ymin-0.5); 
//  end;
//end;

procedure defwinchange;
begin
  if curarea^.winchange<>nil then
  begin
    curarea^.winchange();
  end
  else
  begin
    ortho2(-0.5,curarea^.winrct.xmax-curarea^.winrct.xmin-0.5,-0.5,curarea^.winrct.ymax-curarea^.winrct.ymin-0.5);
    glLoadIdentity();
  end;
end;

//procedure defwinmat; 
//begin
//  ortho2(-0.5,curarea^.winrct.xmax-curarea^.winrct.xmin-0.5,-0.5,curarea^.winrct.ymax-curarea^.winrct.ymin-0.5); 
//  glLoadIdentity(); 
//end;
//
//procedure headmenu(sa: pScrArea);
//var
//fac: single;
//begin
//  
//  if curarea^.full<>nil 
// then
//  begin 
//    confirm(,'Full window'); 
//    exit;
//  end;
//  if okee('Switch header')
// then
//  begin 
//    if sa^.headertype=1
// then
//    sa^.headertype:= 2; 
//    else
//    if sa^.headertype=2
// then
//    sa^.headertype:= 1; 
//    testareas(); 
//  end;
//end;

procedure defheadqread(sa: pScrArea);
var
tempsa: pScrArea;
fac: single;
event: word;
val: smallint;
do_redraw: smallint = 0;
do_change: smallint = 0;
str: pchar;
begin
  //areawinset(sa^.headwin);
  //
  //while sa^.hq<>sa^.headqueue do
  //begin
  //  sa^.hq:= sa^.hq - (2);
  //
  //  event:=  sa^.hq[0];
  //  val:= sa^.hq[1];
  //
  //  if val<>0 then
  //  begin
  //    if (sa^.spacetype=SPACE_TEXT)and(sa^.headqread=do_py_head_event) then
  //    begin
  //      (* exception for a running python script *)
  //      sa^.headqread(event,val);
  //    end
  //    else
  //    if event=LEFTMOUSE then
  //    begin
  //      FrontbufferButs(1);
  //      event:= DoButtons();
  //      FrontbufferButs(0);
  //
  //      if event<>nil then
  //      begin
  //        if event<1000 then
  //        do_headerbuttons(event);
  //      end
  //      (* er is waarschijnlijk in frontbuf getekend *)
  //      (* sa->win_swap= WIN_FRONT_OK; *)
  //      else
  //      begin
  //        mywinset(G.curscreen^.mainwin);
  //        if G.qual and LR_CTRLKEY then
  //        glutPushWindow()
  //        else
  //        begin
  //          glutPopWindow();
  //          glutDoWorkList();
  //          R.winpop:= 1;
  //          (* flag: need pop *)
  //        end;
  //        areawinset(sa^.headwin);
  //      end;
  //    end
  //    else
  //    if event=MOUSEY then
  //    begin
  //      if U.flag and TOOLTIPS then
  //      begin
  //        str:= GetButTip();
  //        if str<>nil then
  //        begin
  //          set_info_text(str);
  //          allqueue(REDRAWINFO,1);
  //        end;
  //      end;
  //    end
  //    else
  //    if event=MIDDLEMOUSE then
  //    begin
  //      scrollheader();
  //      break; {<= !!!b possible in "switch" - then remove this line}
  //    end
  //    else
  //    if event=RIGHTMOUSE then
  //    begin
  //      headmenu(sa);
  //    end
  //    else
  //    if event=REDRAW then
  //    begin
  //      do_redraw:= 1;
  //    end
  //    else
  //    if event=CHANGED then
  //    begin
  //      sa^.head_swap:=0;
  //      do_change:= 1;
  //      do_redraw:= 1;
  //    end
  //    else
  //    if sa^.headqread<>nil then
  //    begin
  //      sa^.headqread(event,val);
  //    end;
  //    if winqueue_break<>0 then
  //    exit;
  //  end;
  //end;
  //tempsa:= areawinar[sa^.headwin]; (* test: bestaat window nog *)
  //if tempsa=nil then
  //exit;
  //(* dit onderscheid loopt niet lekker... *)
  //if (do_change<>0)or(do_redraw<>0) then
  //begin
  //  areawinset(sa^.headwin);
  //  defheadchange();
  //  if sa^.headdraw<>nil then
  //  sa^.headdraw();
  //end;
end;

//function winqtest(sa: pScrArea): word;
//begin
//  if sa^.wq<>sa^.winqueue
// then
//  begin
//    result:=  {word(}sa^.wq[-2];
//    exit;
//  end;
//  begin
//    result:=nil; 
//    exit;
//  end;
//end;
//
//procedure winqdelete(sa: pScrArea);
//begin
//  if sa^.wq<>sa^.winqueue
// then
//  sa^.wq:= sa^.wq - (2); 
//end;
//
//procedure winqclear(sa: pScrArea);
//begin
//  if sa^.wq:=sa^.winqueue
// then
//  ; 
//end;

procedure defwinqread(sa: pScrArea);
var
tempsa: pScrArea;
event: word;
val: smallint;
do_redraw: smallint = 0;
do_change: smallint = 0;
begin
  if (sa<>curarea)or(sa^.win<>winget()) then
  areawinset(sa^.win);
  while sa^.wq<>sa^.winqueue   do
  begin
    sa^.wq:= sa^.wq - (2);
    event:=  {word(}sa^.wq[0];
    val:= sa^.wq[1];
    if event=REDRAW then
    begin
      do_redraw:= 1;
    end
    else
    if event=CHANGED then
    begin
      sa^.win_swap:=0;
      do_change:= 1;
      do_redraw:= 1;
    end
    else
    begin
      if sa^.winqread<>nil then
      sa^.winqread(event,val);
    end;

    if winqueue_break<>0 then
    exit;
  end;

  tempsa:= areawinar[sa^.win]; (* test: bestaat window nog *)
  if tempsa=nil then
  exit;

  if (do_change<>0)or(do_redraw<>0) then
  areawinset(sa^.win);

  if do_change<>0 then
  defwinchange;
  //todo: check this defwinchange(sa^.win);

  if do_redraw<>0 then
  defwindraw();
end;

procedure addqueue(win: smallint;  event: word;  val: smallint);
var
sa: pScrArea;
size: integer;
_end: psmallint;
begin
  (* nieuwe events worden vooraan in het array gezet *)
  sa:= areawinar[win];

  if sa<>nil then
  begin
    if win=sa^.headwin then
    begin
      _end:= sa^.headqueue+MAXQUEUE-2;

      if  PtrUint(sa^.hq)< PtrUint(_end) then
      begin
        size:= PtrUint(sa^.hq)-PtrUint(sa^.headqueue);

        if size<>0  then
        memmove(sa^.headqueue+2,sa^.headqueue,size);
        pword(sa^.headqueue)^:=event;
        sa^.headqueue[1]:= val;
        sa^.hq:= sa^.hq + (2);
      end;
  end
  else
  if win=sa^.win then
  begin
  _end:= sa^.winqueue+MAXQUEUE-2;
  if  PtrUint(sa^.wq)< PtrUint(_end) then
  begin
    size:= PtrUint(sa^.wq)-PtrUint(sa^.winqueue);
    if size<>0 then
    memmove(sa^.winqueue+2,sa^.winqueue,size);
    pword(sa^.winqueue)^:=event;
    sa^.winqueue[1]:= val;
    sa^.wq:= sa^.wq + (2);
  end;
end;
end;
end;

var
afterqueue: array [0..Pred(3*MAXQUEUE)] of smallint;
afterq: psmallint = @afterqueue;

//procedure addafterqueue(win: smallint;  event: word;  val: smallint);
//var
//poin: integer; 
//begin
//  
//  poin:=  {integer(}afterqueue;
//  poin:= poin + (6*(MAXQUEUE-1)); 
//  if  {integer(}afterq<poin
// then
//  begin 
//    afterq[0]:= win; ( {pushort(}afterq)[1]:=event;
//    afterq[2]:= val; 
//    afterq:= afterq + (3); 
//  end;
//end;
//
//procedure append_afterqueue; 
//begin
//  while afterqueue<>afterq
//  do
//  begin 
//    afterq:= afterq - (3); 
//    addqueue(afterq[0],afterq[1],afterq[2]); 
//  end;
//end;

function afterqtest: integer;
begin
  if @afterqueue<>afterq then
    exit(1);

  exit(0);
end;

procedure remake_qual;
begin
  G.qual:= get_qual();
end;

//var
//ext_load_str: array [0..Pred(256)] of char = (0,0); 
//
//procedure add_readfile_event(filename: pchar); 
//begin
//  qenter(LOAD_FILE,1); 
//  strcpy(ext_load_str,filename); 
//  convertstringcode(ext_load_str); 
//end;

var
ext_redraw: smallint =0;
ext_inputchange: smallint =0;
ext_mousemove: smallint =0;
in_ext_qread: smallint =0;
oldwin: integer = 0;

function screen_qread(val: psmallint): word;
var
sa: pScrArea;
win: pbWindow;
event: word;
newwin: smallint;
rt: smallint;
devs: array [0..1] of smallint;
vals: array [0..1] of smallint;
ww: integer;
sc: pbScreen;
wx: integer;
wy: integer;
orx: integer;
ory: integer;
begin
  event:=0;

  if in_ext_qread=0 then
  begin
    if ext_inputchange<>0 then
    begin
      val^:=ext_inputchange;
      ext_inputchange:=0;
      event:= INPUTCHANGE;
    end
    else
    if ext_redraw<>0 then
    begin
      val^:=ext_redraw;
      ext_redraw:=0;
      event:= REDRAW;
    end
    else
    if ext_mousemove<>0 then
    begin
      ext_mousemove:=0;
      event:= MOUSEY;
    end
    else
    if (@afterqueue<>afterq)and(qtest()=0) then
    begin
      val^:=0;
      event:= blendef.AFTERQUEUE;
    end;
  end;
  if event=0 then
  begin
    while myqtest()=0 do
    begin
      ww:=0;
      //processEventsAndTimeouts();
      //glutDoWorkList();
      if myqtest()=0 then
      usleep(1);
    end;
    event:= qread(val);
  end;

  (* if(event==nil) { *)
  (*  while(myqtest()==nil) { *)
  (*   if(qtest()) { *)
  (*    processEventsAndTimeouts(); *)
  (*    glutDoWorkList(); *)
  (*    if(myqtest()==nil) usleep(1); *)
  (*   } *)
  (*   if(myqtest()==nil) if(R.flag & R_RENDERING) return 0; *)
  (*  } *)
  (*  event= qread(val); *)
  (* } *)

  if G.curscreen=nil then
  exit(event);

  if event=Q_FIRSTTIME then
  begin
    //glutDoWorkList();
  end
  else
  if (event=RIGHTSHIFTKEY)or(event=LEFTSHIFTKEY) then
  begin
    if val^<>0 then
    G.qual:= G.qual or (LR_SHIFTKEY)
    else
    G.qual:= G.qual and ( not LR_SHIFTKEY);
  end
  else
  if (event=RIGHTALTKEY)or(event=LEFTALTKEY) then
  begin
    if val^<>0 then
    G.qual:= G.qual or (LR_ALTKEY)
    else
    G.qual:= G.qual and ( not LR_ALTKEY);
  end
  else
  if (event=RIGHTCTRLKEY)or(event=LEFTCTRLKEY) then
  begin
    if val^<>0 then
    G.qual:= G.qual or (LR_CTRLKEY)
    else
    G.qual:= G.qual and ( not LR_CTRLKEY);
  end
  else
  if (event=WINFREEZE)or(event=WINTHAW) then
  begin
    //if (R.flag and R_RENDERING)=0 then
    //begin
    //  if R.win<>nil then
    //  winclose(R.win);
    //  R.win:=0;
    //  G.qual:=0;
    //end;
  end
  {$if 0}
  else
  if event=RESHAPE then
  begin
    if val^=mainwin then
    begin
      init_my_mainwin(mainwin);
      wx:= glutGet(GLUT_WINDOW_WIDTH);
      wy:= glutGet(GLUT_WINDOW_HEIGHT);
      orx:= glutGet(GLUT_WINDOW_X);
      ory:= displaysizey-wy-glutGet(GLUT_WINDOW_Y);

      (* Minimizing on windows causes glut to set the
          * size to 0,  which means things go bonkers
          *)

      if (wx=nil)or(wy=nil) then
      begin
        result:=nil;
        exit;
      end;

      prefstax:= orx;
      prefstay:= ory;
      prefsizx:= wx;
      prefsizy:= wy;
      sc:= G.main.screen.first;
      while sc <> nil do
      begin
        test_scale_screen(sc);
        sc:= sc^.id.next;
      end;
      testareas();
    end;
    event:=nil;
  end
  {$endif}
  else
  if (event=INPUTCHANGE)or(event=REDRAW)or(event=DRAWEDGES) then
  begin
    (* DRAWEDGES: komt vanuit setscreen, qual opnieuw berekenen *)
    if val^=1 then
    remake_qual();

    if (event=INPUTCHANGE)and(in_ext_qread=0) then
    begin
      if val^<>0 then
      begin
        mywinset(val^);
        G.curscreen^.winakt:= val^;
        oldwin:= val^;
      end;
      oldwin:=0;
    end;

    if event=REDRAW then
    begin
      (* kunstmatige mousy voor herberekenen winakt (als b.v. R.win naar achter gepusht *)
      qenter(MOUSEY,scrmousey);
    end;
  end
  else
  if (event=MOUSEX)or(event=MOUSEY) then
  begin
    //if (event=MOUSEY)and(in_ext_qread=nil)and((R.win=nil)or(G.curscreen^.winakt<>R.win) then
    //begin
    //  (* testen waar muis staat *)
    //  newwin:=nil;
    //  win:= swindowbase^.first;
    //  while win
    //  do
    //  begin
    //    if (scrmousex>win.xmin)and(scrmousex<win.xmax) then
    //    begin
    //      (* deze uitzondering betreft onderste en bovenste edge: voor edit cursonedge *)
    //      if (scrmousey=nil)and(scrmousey=win.ymin))or((scrmousey=G.curscreen^.endy)and(scrmousey=win.ymax) then
    //      begin
    //        if (scrmousey>win.ymin)and(scrmousey<win.ymax) then
    //        begin
    //          newwin:= win.id;
    //          break; {<= !!!b possible in "switch" - then remove this line}
    //        end;
    //      end
    //      else
    //      if (scrmousey>=win.ymin)and(scrmousey<=win.ymax) then
    //      begin
    //        newwin:= win.id;
    //        break; {<= !!!b possible in "switch" - then remove this line}
    //      end;
    //    end;
    //    win:= win.next;
    //  end;
    //  (* cursor *)
    //  if newwin<>oldwin then
    //  begin
    //    if newwin=nil then
    //    begin
    //      set_cursonedge(scrmousex,scrmousey);
    //    end;
    //    else
    //    if oldwin=nil then
    //    begin
    //      cursonedge:=nil;
    //    end;
    //    if newwin<>nil then
    //    begin
    //      sa:= areawinar[newwin];
    //      if sa^.win=newwin then
    //      glutSetCursor(sa^.cursor)
    //      else
    //      glutSetCursor(CURSOR_STD);
    //    end;
    //  end;
    //  else
    //  if (newwin=nil)and(oldwin=nil) then
    //  begin
    //    set_cursonedge(scrmousex,scrmousey);
    //  end;
    //  (*
    //     else if (scrmousex<2 or scrmousey<2 or
    //       abs(scrmousex - G.curscreen->sizex)<2 or
    //       abs(scrmousey - G.curscreen->sizey)<2) {
    //      set_cursonedge(scrmousex, scrmousey);
    //     }
    //  *)
    //  if newwin<>0 then
    //  begin
    //    if (newwin<>oldwin)or(G.curscreen^.winakt=nil) then
    //    begin
    //      event:= INPUTCHANGE; {*}val^:=newwin;
    //    end;
    //  end;
    //  oldwin:= newwin;
    //end;
  end
  else
  if event=TIMER0 then
  begin
    event:=0;
    if in_ext_qread=0 then
    begin
      //write_autosave();
      //autosavetime:=0;
    end;
  end
  else
  if event=LOAD_FILE then
  begin
    //event:=0;
    //read_file(ext_load_str);
    //ext_load_str[0]:=#0;
  end;

  exit(event);
end;

//function special_qread(val: psmallint): word;
//var
//event: word;
//begin(* simul alternatief voor extern_qread *)
//  
//  in_ext_qread:= 1; 
//  event:= screen_qread(val); 
//  in_ext_qread:=nil; (* niet zo net, wel zo handig (zie screen_qread) *)
//  if event=REDRAW
// then
//  ext_redraw:= {*}val^; 
//  else
//  if event=INPUTCHANGE
// then
//  ext_inputchange:= {*}val^; 
//  else
//  if event=MOUSEY)or(event=MOUSEX
// then
//  begin 
//    ext_mousemove:= 1; 
//    event:=nil; 
//  end;
//  begin
//    result:= event; 
//    exit;
//  end;
//end;

function ext_qtest: smallint;
begin
  if ext_inputchange<>0 then
  exit(INPUTCHANGE)
  else
  if ext_redraw<>0 then
    exit(REDRAW)
  else
  if ext_mousemove<>0 then
    exit(MOUSEY)
  else
    exit(myqtest());
end;

function extern_qread(val: psmallint): word;
var
event: word; (* bewaart de laatste INPUTCHANGE en de laatste REDRAW *)
begin
  in_ext_qread:= 1;  (* niet zo net, wel zo handig (zie screen_qread) *)

  event:= screen_qread(val);
  if event=REDRAW  then
  ext_redraw:= val^
  else
  if event=INPUTCHANGE then
  ext_inputchange:= val^
  else
  if (event=MOUSEY)or(event=MOUSEX) then
  ext_mousemove:= 1
  else
  if (event=RIGHTSHIFTKEY)or(event=LEFTSHIFTKEY) then
  begin
    if val^<>0 then
    G.qual:= G.qual or (LR_SHIFTKEY)
    else
    G.qual:= G.qual and ( not LR_SHIFTKEY);
  end
  else
  if (event=RIGHTALTKEY)or(event=LEFTALTKEY) then
  begin
    if val^<>0 then
    G.qual:= G.qual or (LR_ALTKEY)
    else
    G.qual:= G.qual and ( not LR_ALTKEY);
  end
  else
  if (event=RIGHTCTRLKEY)or(event=LEFTCTRLKEY) then
  begin
    if val^<>0 then
    G.qual:= G.qual or (LR_CTRLKEY)
    else
    G.qual:= G.qual and ( not LR_CTRLKEY);
  end
  else
  if ((G.qual and LR_CTRLKEY)<>0)and(event=F3KEY) then
  begin
    screendump();
  end;
  in_ext_qread:=0;

  exit(event);
end;

procedure markdirty_all;
var
sa: pScrArea;
begin
  sa:= G.curscreen^.areabase.first;
  while sa <>nil do
  begin
    if sa^.win<>0 then
    begin
      addqueue(sa^.win,REDRAW,1);
      sa^.win_swap:= sa^.win_swap and ( not WIN_FRONT_OK);
    end;
    if sa^.headwin<>0 then
    begin
      addqueue(sa^.headwin,REDRAW,1);
      sa^.head_swap:= sa^.head_swap and ( not WIN_FRONT_OK);
    end;
    sa:= sa^.next;
  end;
end;
//(* BePatch! glutGetFrontbuffer is defined by glDrawBuffer! In fact the name is wrong... *)
//
//procedure header_front_to_back_glut(sa: pScrArea);
//begin
//  glDrawBuffer(GL_FRONT); 
//  glRasterPos2f( {single(}sa^.headrct.xmin-0.1, {single(}sa^.headrct.ymin-0.1);
//  glutGetFrontBuffer(sa^.headrct.xmin,sa^.headrct.ymin,sa^.winx,HEADERY-1); 
//  glDrawBuffer(GL_BACK); 
//  glutPutFrontBuffer(); 
//  sa^.head_swap:= WIN_EQUAL; 
//end;
//
//procedure window_front_to_back_glut(sa: pScrArea);
//begin
//  glDrawBuffer(GL_FRONT); 
//  glRasterPos2f( {single(}sa^.winrct.xmin-0.1, {single(}sa^.winrct.ymin-0.1);
//  glutGetFrontBuffer(sa^.winrct.xmin,sa^.winrct.ymin,sa^.winx,sa^.winy); 
//  glDrawBuffer(GL_BACK); 
//  glutPutFrontBuffer(); 
//  sa^.win_swap:= WIN_EQUAL; 
//end;
//
//procedure header_back_to_front_glut(sa: pScrArea);
//begin
//  glDrawBuffer(GL_BACK); 
//  glRasterPos2f( {single(}sa^.headrct.xmin-0.1, {single(}sa^.headrct.ymin-0.1);
//  glutGetFrontBuffer(sa^.headrct.xmin,sa^.headrct.ymin,sa^.winx,HEADERY-1); 
//  glDrawBuffer(GL_FRONT); 
//  glutPutFrontBuffer(); 
//  glDrawBuffer(GL_BACK); 
//  sa^.head_swap:= WIN_EQUAL; 
//end;
//(* sgi *)
//
//procedure header_front_to_back_ogl(sa: pScrArea);
//begin
//  glReadBuffer(GL_FRONT); 
//  glRasterPos2f( {single(}sa^.headrct.xmin-0.1, {single(}sa^.headrct.ymin-0.1);
//  glCopyPixels(sa^.headrct.xmin,sa^.headrct.ymin,sa^.winx,HEADERY,GL_COLOR); 
//  sa^.head_swap:= WIN_EQUAL; 
//  glReadBuffer(GL_BACK); 
//end;
//
//procedure window_front_to_back_ogl(sa: pScrArea);
//begin
//  glReadBuffer(GL_FRONT); 
//  glRasterPos2f( {single(}sa^.winrct.xmin-0.1, {single(}sa^.winrct.ymin-0.1);
//  glCopyPixels(sa^.winrct.xmin,sa^.winrct.ymin,sa^.winx,sa^.winy,GL_COLOR); 
//  sa^.win_swap:= WIN_EQUAL; 
//  glReadBuffer(GL_BACK); 
//end;
//
//procedure header_back_to_front_ogl(sa: pScrArea);
//begin
//  glDrawBuffer(GL_FRONT); 
//  glRasterPos2f( {single(}sa^.headrct.xmin-0.1, {single(}sa^.headrct.ymin-0.1);
//  glCopyPixels(sa^.headrct.xmin,sa^.headrct.ymin,sa^.winx,HEADERY,GL_COLOR); 
//  sa^.head_swap:= WIN_EQUAL; 
//  glDrawBuffer(GL_BACK); 
//end;
//
//procedure screen_swapbuffers_OGL; 
//var
//sa: pScrArea;
//doswap: integer;
// 
//headswap: integer;
// 
//oldwin: integer; (* test op swap *)
//begin
//  
//  doswap:=nil; 
//  headswap:=nil; 
//  sa:= G.curscreen^.areabase.first; 
//  while sa
//  do
//  begin 
//    if sa^.win_swap=WIN_BACK_OK
// then
//    doswap:= 1; 
//    if sa^.head_swap=WIN_BACK_OK
// then
//    headswap:= 1; 
//    sa:= sa^.next; 
//  end;
//  if doswap=nil)and(headswap=nil
// then
//  exit;
//  oldwin:= winget(); 
//  mywinset(G.curscreen^.mainwin); 
//  if doswap<>nil 
// then
//  begin 
//    sa:= G.curscreen^.areabase.first; 
//    while sa
//    do
//    begin 
//      if sa^.win_swap=WIN_FRONT_OK
// then
//      window_front_to_back_ogl(sa); 
//      if sa^.head_swap=WIN_FRONT_OK
// then
//      header_front_to_back_ogl(sa); 
//      sa:= sa^.next; 
//    end;
//    myswapbuffers(); 
//  end;
//  sa:= G.curscreen^.areabase.first; (* headers moeten in front en back gelijk zijn: dus: *)
//  (* heads van back naar frontbuffer? of omgekeerd? *)
//  while sa
//  do
//  begin 
//    if sa^.head_swap=WIN_FRONT_OK
// then
//    begin 
//      header_front_to_back_ogl(sa); 
//    end;
//    else
//    if sa^.head_swap=WIN_BACK_OK
// then
//    begin 
//      header_back_to_front_ogl(sa); 
//    end;
//    sa:= sa^.next; 
//  end;
//  if oldwin<>nil 
// then
//  areawinset(oldwin); 
//end;
//
//procedure screen_swapbuffers_GLUT; 
//var
//sa: pScrArea;
//doswap: integer;
// 
//headswap: integer;
// 
//oldwin: integer; (* test op swap *)
//begin
//  
//  doswap:=nil; 
//  headswap:=nil; 
//  sa:= G.curscreen^.areabase.first; 
//  while sa
//  do
//  begin 
//    if sa^.win_swap=WIN_BACK_OK
// then
//    doswap:= 1; 
//    if sa^.head_swap=WIN_BACK_OK
// then
//    headswap:= 1; 
//    sa:= sa^.next; 
//  end;
//  if doswap=nil)and(headswap=nil
// then
//  exit;
//  oldwin:= winget(); 
//  mywinset(G.curscreen^.mainwin); 
//  if doswap<>nil 
// then
//  begin 
//    sa:= G.curscreen^.areabase.first; 
//    while sa
//    do
//    begin 
//      if sa^.win_swap=WIN_FRONT_OK
// then
//      window_front_to_back_glut(sa); 
//      if sa^.head_swap=WIN_FRONT_OK
// then
//      header_front_to_back_glut(sa); 
//      sa:= sa^.next; 
//    end;
//    myswapbuffers(); 
//  end;
//  sa:= G.curscreen^.areabase.first; (* headers moeten in front en back gelijk zijn: dus: *)
//  (* heads van back naar frontbuffer? of omgekeerd? *)
//  while sa
//  do
//  begin 
//    if sa^.head_swap=WIN_FRONT_OK
// then
//    begin 
//      header_front_to_back_glut(sa); 
//    end;
//    else
//    if sa^.head_swap=WIN_BACK_OK
// then
//    begin 
//      header_back_to_front_glut(sa); 
//    end;
//    sa:= sa^.next; 
//  end;
//  if oldwin<>nil 
// then
//  areawinset(oldwin); 
//end;

procedure screen_swapbuffers_REDRAW;
var
sa: pScrArea;
oldwin: integer;
doswap: integer = 0;
swap: integer;
begin
  oldwin:= winget();

  sa:= G.curscreen^.areabase.first; (* dit is een nieuwe implementatie: uitsluitend met redraws en normale swapbuffer *)
  (* allemaal front ok? *)

  while sa <>nil  do
  begin
    if (sa^.win<>0)and((sa^.win_swap and WIN_FRONT_OK)=0) then
    break;

    if (sa^.head_swap and WIN_FRONT_OK)=0 then
    break;

    sa:= sa^.next;
  end;
  if sa=nil then
  exit;

  (* printf('front not OK %d %d %d %d\n', [sa^.win, sa^.win_swap, sa^.headwin, sa^.head_swap]); *)
  sa:= G.curscreen^.areabase.first;
  while sa<>nil   do
  begin
    swap:= sa^.win_swap;

    if (swap and WIN_BACK_OK)=0 then
    begin
      if (sa^.win<>0)and(sa^.windraw<>nil) then
      begin
        areawinset(sa^.win);
        sa^.windraw();
        doswap:= 1;
      end;
      sa^.win_swap:= swap or WIN_BACK_OK;
    end
    else
    if sa^.win_swap=WIN_BACK_OK then
    doswap:= 1;

    swap:= sa^.head_swap;
    if (swap and WIN_BACK_OK)=0 then
    begin
      areawinset(sa^.headwin);

      if sa^.headdraw<>nil then
      sa^.headdraw();

      doswap:= 1;

      sa^.head_swap:= swap or WIN_BACK_OK;
    end
    else
    if sa^.head_swap=WIN_BACK_OK then
    doswap:= 1;

    sa:= sa^.next;
  end;

  (* de hele backbuffer moet nu OK zijn *)
  if doswap<>0 then
  begin
    myswapbuffers();
  end;

  if oldwin<>0 then
  areawinset(oldwin);
end;

//procedure screen_swapbuffers_MESA; 
//var
//sa: pScrArea; (* dit is een nieuwe implementatie: uitsluitend met partial swap *)
//(* test op swap *)
//begin
//  sa:= G.curscreen^.areabase.first; 
//  while sa
//  do
//  begin 
//    if sa^.win_swap=WIN_BACK_OK
// then
//    begin 
//      myCopySubBuffer(sa^.winrct.xmin,sa^.winrct.ymin-1,sa^.winx,sa^.winy); 
//      sa^.win_swap:= WIN_EQUAL; 
//    end;
//    if sa^.head_swap=WIN_BACK_OK
// then
//    begin 
//      myCopySubBuffer(sa^.headrct.xmin,sa^.headrct.ymin-1,sa^.winx,HEADERY); 
//      sa^.head_swap:= WIN_EQUAL; 
//    end;
//    sa:= sa^.next; 
//  end;
//end;

procedure screen_swapbuffers;
begin
  {$ifdef __sgi}
  screen_swapbuffers_OGL();
  {$endif}

  {$ifdef MESA30}
  screen_swapbuffers_MESA();
  {$else}
  screen_swapbuffers_REDRAW();
  {$endif}

  {$ifdef WINDOWS}
  screen_swapbuffers_REDRAW();
  {$endif}

  {$ifdef BEOS}
  screen_swapbuffers_GLUT();
  {$endif}

  {$ifdef __SUN}
  {$ifndef MESA}
  screen_swapbuffers_OGL();
  {$endif}
  {$endif}
end;

//function is_allowed_to_change_screen(new: pbScreen): integer; 
//begin(* niet als curscreen is full
//    * niet als obedit and old->scene!=new->scene
//    *)
//  if new=nil
// then
//  begin
//    result:=nil; 
//    exit;
//  end;
//  if G.curscreen^.full<>nil 
// then
//  begin
//    result:=nil; 
//    exit;
//  end;
//  if curarea^.full<>nil 
// then
//  begin
//    result:=nil; 
//    exit;
//  end;
//  if G.obedit<>nil 
// then
//  begin 
//    if G.curscreen^.scene<>new.scene
// then
//    begin
//      result:=nil; 
//      exit;
//    end;
//  end;
//  begin
//    result:= 1; 
//    exit;
//  end;
//end;

procedure screenmain; cdecl;
var
sa: pScrArea;
sa1: pScrArea;
sa2: pScrArea;
sc: pbScreen;
fac: single;
wx: integer;
wy: integer;
orx: integer;
ory: integer;
event: word;
a: smallint;
val: smallint;
towin: smallint;
dodrawscreen: smallint=0;
inqueue: smallint;
debugval: smallint=0;
begin
//  {Xqual: integer; }{<= !!!5 external variable}

   (* keert terug naar de glut! *)

  while (qtest<>0) or (afterqtest<>0) do
  begin
    event:= screen_qread(@val);
    towin:= event;

    if ((G.f and G_DEBUG)<>0)and(event<>0)and(event<>MOUSEY) then
    begin
      PRINT3('d','d','d',event,val,G.qual);
      debugval:= 1;
    end;

    //if event=LEFTMOUSE then
    //begin
    //  if val)and(cursonedge then
    //  begin
    //    moveareas();
    //    towin:=nil;
    //  end;
    //end;
    //else
    //if event=MIDDLEMOUSE then
    //begin
    //  if val)and(cursonedge then
    //  begin
    //    editsplitpoint();
    //    towin:=nil;
    //  end;
    //end;
    //else
    //if event=RIGHTMOUSE then
    //begin
    //  if val)and(cursonedge then
    //  begin
    //    joinarea(curarea);
    //    towin:=nil;
    //  end;
    //end;
    //else
    //if event=QKEY then
    //begin
    //  if (G.obedit)and(G.obedit.type=OB_FONT)and(curarea^.spacetype=SPACE_VIEW3D))or(curarea^.spacetype=SPACE_TEXT then
    //  ;
    //  else
    //  begin
    //    if okee('QUIT BLENDER') then
    //    exit_usiblender();
    //    towin:=nil;
    //  end;
    //end;
    //else
    //if event=SPACEKEY then
    //begin
    //  if (G.obedit)and(G.obedit.type=OB_FONT)and(curarea^.spacetype=SPACE_VIEW3D))or(curarea^.spacetype=SPACE_TEXT then
    //  ;
    //  else
    //  begin
    //    if val<>nil
    // then
    //    toolbox();
    //    towin:=nil;
    //  end;
    //end;
    //else
    //if event=INPUTCHANGE then
    //begin
    //  (* welke headers moeten redraw? *)
    //  if val>3 then
    //  begin
    //    (* eerste drie nummers voor GL *)
    //    if G.curscreen^.winakt<>val then
    //    begin
    //      (* de oude en nieuwe area *)
    //      sa1:= areawinar[G.curscreen^.winakt];
    //      sa2:= areawinar[val];
    //      if sa1=sa2 then
    //      ;
    //      else
    //      begin
    //        if sa1<>nil then
    //        addqueue(sa1.headwin,REDRAW,1);
    //        if sa2<>nil then
    //        addqueue(sa2.headwin,REDRAW,1);
    //      end;
    //    end;
    //    (* testen of window nog bestaat (oude event bij join b.v.) *)
    //    if areawinar[val]<>nil then
    //    begin
    //      (* als winakt==R.win mag alleen een GL-INPUTCHANGE winakt zetten *)
    //      if R.win=nil)or(G.curscreen^.winakt<>R.win then
    //      G.curscreen^.winakt:= val;
    //      clear_global_filesel_vars();
    //    end;
    //    else
    //    begin
    //      G.curscreen^.winakt:=nil;
    //    end;
    //  end;
    //  towin:=nil;
    //end;
    //else
    //if event=DRAWEDGES then
    //begin
    //  towin:=nil;
    //  dodrawscreen:= 1;
    //end;
    //else
    if event=REDRAW then
    begin
      towin:=0;
      if val=G.curscreen^.mainwin then
      begin
        markdirty_all();
        dodrawscreen:= 1;
      end
      else
      if val>3 then
      begin
        addqueue(val,REDRAW,val);
      end;
    //  else
    //  if R.win)and(val=R.win then
    //  begin
    //    mywinset(R.win);
    //    getorigin(@orx,@ory);
    //    R.winxof:= orx;
    //    R.winyof:= ory;
    //    redraw_render_win(val);
    //  end;
    end;
    //else
    //if event=RIGHTARROWKEY then
    //begin
    //  if val)and((G.qual and LR_CTRLKEY) then
    //  begin
    //    sc:= G.curscreen^.id.next;
    //    if is_allowed_to_change_screen(sc)
    // then
    //    setscreen(sc);
    //    towin:=nil;
    //  end;
    //end;
    //else
    //if event=LEFTARROWKEY then
    //begin
    //  if val)and((G.qual and LR_CTRLKEY) then
    //  begin
    //    sc:= G.curscreen^.id.prev;
    //    if is_allowed_to_change_screen(sc)
    // then
    //    setscreen(sc);
    //    towin:=nil;
    //  end;
    //end;
    //else
    //if event=UPARROWKEY)or(event=DOWNARROWKEY then
    //begin
    //  if val)and((G.qual and LR_CTRLKEY) then
    //  begin
    //    area_fullscreen();
    //    towin:=nil;
    //  end;
    //end;
    //else
    //if event=AFTERQUEUE then
    //begin
    //  append_afterqueue();
    //end;
    //if towin<>nil then
    //begin
    //  towin:= blenderqread(event,val);
    //  if towin)and(G.curscreen^.winakt
    // then
    //  addqueue(G.curscreen^.winakt,event,val);
    //end;

    event:= ext_qtest(); (* window queues en swapbuffers *)

    if (event=0)or(event=EXECUTE) then
    begin
      (* or event==MOUSEY ?? *)
      inqueue:= 1;
      while inqueue<>0      do
      begin
        inqueue:=0;
        winqueue_break:=0;
        sa:= G.curscreen^.areabase.first;
        while sa<>nil           do
        begin
          (* bewust eerst header afhandelen, dan rest. Header is soms init *)
          if (sa^.headwin<>0)and(sa^.headqueue<>sa^.hq) then
          begin
            defheadqread(sa);
            inqueue:= 1;
          end;

          if winqueue_break<>0 then
          begin
            (* mogelijk nieuwe G.curscreen *)
            inqueue:= 1;
            break; {<= !!!b possible in "switch" - then remove this line}
          end;

          if (sa^.win<>0)and(sa^.winqueue<>sa^.wq          ) then
          begin
            defwinqread(sa);
            inqueue:= 1;
          end;

          if winqueue_break<>0 then
          begin
            (* mogelijk nieuwe G.curscreen *)
            inqueue:= 1;
            break; {<= !!!b possible in "switch" - then remove this line}
          end;

          sa:= sa^.next;
        end;
      end;

      screen_swapbuffers();

      if dodrawscreen<>0 then
      begin
        drawscreen();
        dodrawscreen:=0;
      end;
    end;

    (* restore actieve area *)
    if G.curscreen^.winakt<>winget() then
    areawinset(G.curscreen^.winakt);
  end;

  (* de enige idle *)
  usleep(2);

  if (debugval=1)and((G.f and G_DEBUG)<>0) then
  begin
    debugval:= glGetError();
    while debugval<>GL_NO_ERROR      do
    begin
      printf('GL_ERROR: %d %s\n',[debugval,gluErrorString(debugval)]);
      debugval:= glGetError();
    end;
    printf('end screenmain\n');
  end;
end;

(* *********  AREAS  ************* *)

procedure getdisplaysize;
begin
  displaysizex:= glutGet(GLUT_SCREEN_WIDTH);
  displaysizey:= glutGet(GLUT_SCREEN_HEIGHT);
end;

procedure setprefsize(stax, stay, sizx, sizy: longint);
begin
  if stax<0 then
  stax:=0;
  if stay<0 then
  stay:=0;
  if sizx<320 then
  sizx:= 320;
  if sizy<256 then
  sizy:= 256;

  if stax+sizx>displaysizex then
  sizx:= displaysizex-stax;
  if stay+sizy>displaysizey then
  sizy:= displaysizey-stay;
  if (sizx<320)or(sizy<256) then
  begin
    printf('ERROR: illegal prefsize\n');
    exit;
  end;

  prefstax:= stax;
  prefstay:= stay;
  prefsizx:= sizx;
  prefsizy:= sizy;
end;

//function findcurarea: pScrArea;
//var
//sa: pScrArea;
//mval: array [0..1] of smallint;
//begin
//  
//  
//  getmouseco_sc(mval); 
//  sa:= G.curscreen^.areabase.first; 
//  while sa
//  do
//  begin 
//    if sa^.v1^.vec.x<=mval[0])and(sa^.v3^.vec.x>=mval[0]
// then
//    begin 
//      if sa^.v1^.vec.y<=mval[1])and(sa^.v2^.vec.y>=mval[1]
// then
//      begin 
//        begin
//          result:= sa; 
//          exit;
//        end;
//      end;
//    end;
//    sa:= sa^.next; 
//  end;
//  begin
//    result:=nil; 
//    exit;
//  end;
//end;

function addscrvert(lb: pListBase;  x: smallint;  y: smallint): pScrVert;
var
sv: pScrVert;
begin
  sv:= callocN(sizeof(ScrVert), 'addscrvert');
  sv^.vec.x:= x;
  sv^.vec.y:= y;

  if lb<> nil then
  addtail(lb,sv);

  exit(sv);
end;

procedure sortscrvert(v1, v2: ppScrVert);
var
tmp: pScrVert;
begin
  if ptruint(v1^)>ptruint(v2^) then
  begin
    tmp:= v1^;
    v1^:=v2^;
    v2^:=tmp;
  end;
end;

function addscredge(lb: pListBase;  v1, v2: pScrVert): pScrEdge;
var
se: pScrEdge;
begin
  se:= callocN(sizeof(ScrEdge), 'addscredge');

  sortscrvert(@v1,@v2);
  se^.v1:= v1;
  se^.v2:= v2;

  if lb<> nil then
  addtail(lb,se);

  exit(se);
end;

function findscredge(v1: pScrVert;  v2: pScrVert): pScrEdge;
var
sv: pScrVert;
se: pScrEdge;
begin
  sortscrvert(@v1,@v2);
  se:= G.curscreen^.edgebase.first;
  while se<>nil  do
  begin
    if (se^.v1=v1)and(se^.v2=v2) then
      exit(se);

    se:= se^.next;
  end;

  exit(nil);
end;

function findscredge_sc(sc: pbScreen;  v1: pScrVert;  v2: pScrVert): pScrEdge;
var
sv: pScrVert;
se: pScrEdge;
begin
  sortscrvert(@v1,@v2);
  se:= sc^.edgebase.first;
  while se <>nil  do
  begin
    if (se^.v1=v1)and(se^.v2=v2) then
    exit(se);
    se:= se^.next;
  end;
  exit(nil);
end;

procedure removedouble_scrverts;
var
v1: pScrVert;
verg: pScrVert;
se: pScrEdge;
sa: pScrArea;
begin
  verg:= G.curscreen^.vertbase.first;
  while verg <>nil   do
  begin
    if verg^._new=nil then
    begin
      (* !!! *)
      v1:= verg^.next;
      while v1 <>nil      do
      begin
        if v1^._new=nil then
        begin
          (* !?! *)
          if (v1^.vec.x=verg^.vec.x)and(v1^.vec.y=verg^.vec.y) then
          begin
            (* printf("doublevert\n"); *)
            v1^._new:= verg;
          end;
        end;
        v1:= v1^.next;
      end;
    end;
    verg:= verg^.next;
  end;

  se:= G.curscreen^.edgebase.first; (* vervang pointers in edges en vlakken *)
  while se <>nil  do
  begin
    if se^.v1^._new<>nil
 then
    se^.v1:= se^.v1^._new;
    if se^.v2^._new<>nil
 then
    se^.v2:= se^.v2^._new;
    sortscrvert(@(se^.v1),@(se^.v2));
    (* edges zijn veranderd: dus.... *)
    se:= se^.next;
  end;
  sa:= G.curscreen^.areabase.first;
  while sa  <>nil  do
  begin
    if sa^.v1^._new<>nil then
    sa^.v1:= sa^.v1^._new;
    if sa^.v2^._new<>nil then
    sa^.v2:= sa^.v2^._new;
    if sa^.v3^._new<>nil then
    sa^.v3:= sa^.v3^._new;
    if sa^.v4^._new<>nil then
    sa^.v4:= sa^.v4^._new;

    sa:= sa^.next;
  end;
  verg:= G.curscreen^.vertbase.first; (* verwijderen *)
  while verg <>nil  do
  begin
    v1:= verg^.next;
    if verg^._new<>nil then
    begin
      remlink(@G.curscreen^.vertbase,verg);
      freeN(verg);
    end;
    verg:= v1;
  end;
end;

//procedure removenotused_scrverts; 
//var
//sv: pScrVert; 
//svn: pScrVert; 
//se: pScrEdge; 
//sa: pScrArea; (* ga ervan uit dat de edges goed zijn *)
//begin
//  
//  
//  
//  se:= G.curscreen^.edgebase.first; 
//  while se
//  do
//  begin 
//    se^.v1^.flag:= 1; 
//    se^.v2^.flag:= 1; 
//    se:= se^.next; 
//  end;
//  sv:= G.curscreen^.vertbase^.first; 
//  while sv
//  do
//  begin 
//    svn:= sv^.next; 
//    if sv^.flag=nil
// then
//    begin 
//      remlink(@G.curscreen^.vertbase,sv); 
//      freeN(sv); 
//    end;
//    else
//    sv^.flag:=nil; 
//    sv:= svn; 
//  end;
//end;

procedure removedouble_scredges;
var
v1: pScrVert;
verg: pScrEdge;
se: pScrEdge;
sn: pScrEdge; (* vergelijken *)
begin
  verg:= G.curscreen^.edgebase.first;
  while verg <>nil  do
  begin
    se:= verg^.next;
    while se<>nil    do
    begin
      sn:= se^.next;
      if (verg^.v1=se^.v1)and(verg^.v2=se^.v2) then
      begin
        remlink(@G.curscreen^.edgebase,se);
        freeN(se);
      end;
      se:= sn;
    end;
    verg:= verg^.next;
  end;
end;

procedure removenotused_scredges;
var
sv: pScrVert;
se: pScrEdge;
sen: pScrEdge;
sa: pScrArea;
a: integer;
 (* zet flag als edge gebruikt wordt in area *)
begin
  a:=0; (* zet flag als edge gebruikt wordt in area *)
  sa:= G.curscreen^.areabase.first;
  while sa<>nil  do
  begin
    se:= findscredge(sa^.v1,sa^.v2);
    if se=nil then
    printf('error: area %d edge 1 bestaat niet\n',[a])
    else
    se^.flag:= 1;
    se:= findscredge(sa^.v2,sa^.v3);
    if se=nil then
    printf('error: area %d edge 2 bestaat niet\n',[a])
    else
    se^.flag:= 1;
    se:= findscredge(sa^.v3,sa^.v4);
    if se=nil then
    printf('error: area %d edge 3 bestaat niet\n',[a])
    else
    se^.flag:= 1;
    se:= findscredge(sa^.v4,sa^.v1);
    if se=nil then
    printf('error: area %d edge 4 bestaat niet\n',[a])
    else
    se^.flag:= 1;
    sa:= sa^.next;
    inc(a);
  end;
  se:= G.curscreen^.edgebase.first;
  while se <>nil  do
  begin
    sen:= se^.next;
    if se^.flag=0 then
    begin
      remlink(@G.curscreen^.edgebase,se);
      freeN(se);
    end
    else
    se^.flag:=0;
    se:= sen;
  end;
end;

procedure calc_arearcts(sa: pScrArea);
begin
  if sa^.v1^.vec.x>0 then
    sa^.totrct.xmin:= sa^.v1^.vec.x+EDGEWIDTH2+1
  else
    sa^.totrct.xmin:= sa^.v1^.vec.x;
  if sa^.v4^.vec.x<G.curscreen^.sizex-1 then
    sa^.totrct.xmax:= sa^.v4^.vec.x-EDGEWIDTH2-1
  else
    sa^.totrct.xmax:= sa^.v4^.vec.x;

  if sa^.v1^.vec.y>0 then
    sa^.totrct.ymin:= sa^.v1^.vec.y+EDGEWIDTH2+1
  else
    sa^.totrct.ymin:= sa^.v1^.vec.y;
  if sa^.v2^.vec.y<G.curscreen^.sizey-1 then
    sa^.totrct.ymax:= sa^.v2^.vec.y-EDGEWIDTH2-1
  else
    sa^.totrct.ymax:= sa^.v2^.vec.y;

  sa^.winrct:= sa^.totrct;
  if sa^.headertype<>0 then
  begin
    sa^.headrct:= sa^.totrct;
    if sa^.headertype=HEADERDOWN then
    begin
      sa^.headrct.ymax:= sa^.headrct.ymin+HEADERY-1;
      sa^.winrct.ymin:= sa^.headrct.ymax+1;
    end
    else
    if sa^.headertype=HEADERTOP then
    begin
      sa^.headrct.ymin:= sa^.headrct.ymax-HEADERY+1;
      sa^.winrct.ymax:= sa^.headrct.ymin-1;
    end;
  end;
  if sa^.winrct.ymin>sa^.winrct.ymax then
  sa^.winrct.ymin:= sa^.winrct.ymax;

    (* als speedup voor berekeningen *)
  sa^.winx:= sa^.winrct.xmax-sa^.winrct.xmin+1;
  sa^.winy:= sa^.winrct.ymax-sa^.winrct.ymin+1;
end;

procedure openheadwin(sa: pScrArea);
begin
  sa^.headwin:= myswinopen(G.curscreen^.mainwin,sa^.headrct.xmin,sa^.headrct.xmax,sa^.headrct.ymin,sa^.headrct.ymax);

  glMatrixMode(GL_MODELVIEW);

  if sa^.headqueue=nil then
  begin
    sa^.hq:=mallocN(sizeof(smallint)*MAXQUEUE, 'headqueue');
    sa^.headqueue:=sa^.hq;
  end;

  areawinar[sa^.headwin]:= sa;    (* anders doet addqueue het niet *)
  //addqueue(sa^.headwin,CHANGED,1);
end;

procedure openareawin(sa: pScrArea);
begin
  sa^.win:= myswinopen(G.curscreen^.mainwin,sa^.winrct.xmin,sa^.winrct.xmax,sa^.winrct.ymin,sa^.winrct.ymax);

  if sa^.winqueue=nil then
  begin
    sa^.wq:=mallocN(sizeof(smallint)*MAXQUEUE, 'winqueue');
  sa^.winqueue:=sa^.wq;
  end;

  areawinar[sa^.win]:= sa;  (* anders doet addqueue het niet *)
  //addqueue(sa^.win,CHANGED,1);
end;

procedure closeheadwin(sa: pScrArea);
begin
  if (G.curscreen<>nil)and(sa^.headwin=G.curscreen^.winakt) then
  G.curscreen^.winakt:=0;

  if sa^.headwin<>0 then
  winclose(sa^.headwin);

  sa^.headwin:=0;
  sa^.headdraw:=nil;
  if sa^.headqueue<>nil then
  begin
    freeN(sa^.headqueue);
    sa^.hq:=nil;
    sa^.headqueue:=sa^.hq;
  end;
end;

procedure closeareawin(sa: pScrArea);
begin
  if (G.curscreen<>nil)and(sa^.win=G.curscreen^.winakt) then
  G.curscreen^.winakt:=0;

  if sa^.win<>0 then
  winclose(sa^.win);

  sa^.win:=0;
  if sa^.winqueue<>nil then
  begin
    freeN(sa^.winqueue);
    sa^.wq:=nil;
    sa^.winqueue:= nil;
  end;
end;

procedure del_area(sa: pScrArea);
begin
  closeareawin(sa);
  closeheadwin(sa);
  freespacelist(@sa^.spacedata);
end;

procedure copy_areadata(sa1: pScrArea;  sa2: pScrArea);
begin
  sa1^.headertype:= sa2^.headertype;
  sa1^.spacetype:= sa2^.spacetype;
  sa1^.butspacetype:= sa2^.spacetype;
  sa1^.headdraw:= sa2^.headdraw;
  sa1^.windraw:= sa2^.windraw;
  sa1^.headqread:= sa2^.headqread;
  sa1^.winqread:= sa2^.winqread;
  sa1^.headchange:= sa2^.headchange;
  sa1^.winchange:= sa2^.winchange;
  Mat4CpyMat4(@sa1^.winmat,@sa2^.winmat);
  freespacelist(@sa1^.spacedata);
  duplicatespacelist(@sa1^.spacedata,@sa2^.spacedata);
end;

function addscrarea(lb: pListBase; v1, v2, v3, v4: pScrVert;headertype: smallint;spacetype: smallint): pScrArea;
var
sa: pScrArea;
begin
  sa:= callocN(sizeof(ScrArea), 'addscrarea');
  sa^.cursor:= CURSOR_STD;
  sa^.v1:= v1;
  sa^.v2:= v2;
  sa^.v3:= v3;
  sa^.v4:= v4;
  sa^.headertype:= headertype;
  sa^.butspacetype:=spacetype;
  sa^.spacetype:= sa^.butspacetype;

  calc_arearcts(sa);

  if headertype<>0 then
  openheadwin(sa);

  if sa^.winrct.ymin<sa^.winrct.ymax then
  openareawin(sa);

  if lb<>nil then
  addtail(lb,sa);

  exit(sa);
end;

procedure testareas;
var
sa: pScrArea;
tempw: rcti;
temph: rcti; (* testen of header er moet zijn, of weg moet, of verplaatst *)
(* testen of window er moet zijn, of weg moet, of verplaatst *)
begin


  sa:= G.curscreen^.areabase.first;
  while sa <>nil  do
  begin
    tempw:= sa^.winrct;
    temph:= sa^.headrct;
    calc_arearcts(sa);
    (* test header *)
    if sa^.headertype<>0 then
    begin
      if sa^.headwin=0 then
      openheadwin(sa)
      else
      begin
        (* is header op goede plek? *)
        if (temph.xmin<>sa^.headrct.xmin)or(temph.xmax<>sa^.headrct.xmax)or(temph.ymin<>sa^.headrct.ymin)or(temph.ymax<>sa^.headrct.ymax) then
        begin
          addqueue(sa^.headwin,CHANGED,1);
          mywinposition(sa^.headwin,sa^.headrct.xmin,sa^.headrct.xmax,sa^.headrct.ymin,sa^.headrct.ymax);
        end;
        if sa^.headbutlen<sa^.winx then
        begin
          sa^.headbutofs:=0;
          addqueue(sa^.headwin,CHANGED,1);
        end
        else
        if sa^.headbutofs+sa^.winx>sa^.headbutlen then
        begin
          sa^.headbutofs:= sa^.headbutlen-sa^.winx;
          addqueue(sa^.headwin,CHANGED,1);
        end;
      end;
    end
    else
    begin
      if sa^.headwin<>0 then
      closeheadwin(sa);
    end;
    (* test areawindow *)
    if sa^.win=0 then
    begin
      if sa^.winrct.ymin<sa^.winrct.ymax then
      openareawin(sa);
    end
    else
    begin
      (* window te klein? *)
      if sa^.winrct.ymin=sa^.winrct.ymax then
      closeareawin(sa)
      else
      begin
        (* window veranderd? *)
        if (tempw.xmin<>sa^.winrct.xmin)or(tempw.xmax<>sa^.winrct.xmax)or(tempw.ymin<>sa^.winrct.ymin)or(tempw.ymax<>sa^.winrct.ymax) then
        begin
          addqueue(sa^.win,CHANGED,1);
          mywinposition(sa^.win,sa^.winrct.xmin,sa^.winrct.xmax,sa^.winrct.ymin,sa^.winrct.ymax);
        end;
      end;
    end;
    sa:= sa^.next;
  end;
  bzero(@areawinar,4*MAXWIN);
  (* remake global windowarray *)
  sa:= G.curscreen^.areabase.first;
  while sa<>nil  do
  begin
    if sa^.headwin<>0 then
    areawinar[sa^.headwin]:= sa;
    if sa^.win<>0 then
    areawinar[sa^.win]:= sa;
    sa:= sa^.next;
  end;
  (* test of winakt in orde is *)
  if areawinar[G.curscreen^.winakt]=nil then
  G.curscreen^.winakt:=0;
end;

//function test_edge_area(sa: pScrArea;  se: pScrEdge): pScrArea;
//var
//se1: pScrEdge;
// 
//se2: pScrEdge;
// 
//se3: pScrEdge;
// 
//se4: pScrEdge;
// 
//begin(* test of edge in area ligt, zo niet, 
//      vind een area die 'm wel heeft *)
//  se1:=nil; 
//  se2:=nil; 
//  se3:=nil; 
//  se4:=nil; 
//  if sa<>nil 
// then
//  begin 
//    se1:= findscredge(sa^.v1,sa^.v2); 
//    se2:= findscredge(sa^.v2,sa^.v3); 
//    se3:= findscredge(sa^.v3,sa^.v4); 
//    se4:= findscredge(sa^.v4,sa^.v1); 
//  end;
//  if se1<>se)and(se2<>se)and(se3<>se)and(se4<>se
// then
//  begin 
//    sa:= G.curscreen^.areabase.first; 
//    while sa
//    do
//    begin 
//      (* een beetje optimaliseren? *)
//      if se^.v1=sa^.v1)or(se^.v1=sa^.v2)or(se^.v1=sa^.v3)or(se^.v1=sa^.v4
// then
//      begin 
//        se1:= findscredge(sa^.v1,sa^.v2); 
//        se2:= findscredge(sa^.v2,sa^.v3); 
//        se3:= findscredge(sa^.v3,sa^.v4); 
//        se4:= findscredge(sa^.v4,sa^.v1); 
//        if se1=se)or(se2=se)or(se3=se)or(se4=se
// then
//        begin
//          result:= sa; 
//          exit;
//        end;
//      end;
//      sa:= sa^.next; 
//    end;
//  end;
//  begin
//    result:= sa; 
//    exit;
//    (* is keurig 0 als niet gevonden *)
//  end;
//end;
//
//function closest_bigger_area: pScrArea;
//var
//sa: pScrArea;
//big: pScrArea;
// 
//cent: array [0..2] of single;
//vec: array [0..2] of single;
//len: single;
//len1: single;
//len2: single;
//len3: single;
//dist: single;
// 
//mval: array [0..1] of smallint;
//begin
//  
//  big:=nil; 
//  
//  
//  
//  
//  
//  
//  dist:=1000; 
//  
//  getmouseco_sc(mval); 
//  cent[0]:= mval[0]; 
//  cent[1]:= mval[1]; 
//  cent[2]:= vec[2]:=nil; 
//  sa:= G.curscreen^.areabase.first; 
//  while sa
//  do
//  begin 
//    if sa<>curarea
// then
//    begin 
//      if sa^.winy>=curarea^.winy
// then
//      begin 
//        (* mimimum van vier hoekpunten *)
//        vec[0]:= sa^.v1^.vec.x; 
//        vec[1]:= sa^.v1^.vec.y; 
//        len:= VecLenf(vec,cent); 
//        vec[0]:= sa^.v2^.vec.x; 
//        vec[1]:= sa^.v2^.vec.y; 
//        len1:= VecLenf(vec,cent); 
//        vec[0]:= sa^.v3^.vec.x; 
//        vec[1]:= sa^.v3^.vec.y; 
//        len2:= VecLenf(vec,cent); 
//        vec[0]:= sa^.v4^.vec.x; 
//        vec[1]:= sa^.v4^.vec.y; 
//        len3:= VecLenf(vec,cent); 
//        len:= MIN4(len,len1,len2,len3); 
//        vec[0]:= (sa^.v2^.vec.x+sa^.v3^.vec.x) div 2; 
//        vec[1]:= (sa^.v1^.vec.y+sa^.v2^.vec.y) div 2; 
//        len:= len + (0.5*VecLenf(vec,cent)); 
//        len:= len - (sa^.winy+sa^.winx); (* plus centrum *)
//        (* min afmeting *)
//        if len<dist
// then
//        begin 
//          dist:= len; 
//          big:= sa; 
//        end;
//      end;
//    end;
//    sa:= sa^.next; 
//  end;
//  if big<>nil 
// then
//  begin
//    result:= big; 
//    exit;
//  end;
//  else
//  begin
//    result:= curarea; 
//    exit;
//  end;
//end;

(* ************ SCREENBEHEER ************** *)

(* testen of screenvertices vergroot/verkleind moeten worden *)
(* testen of offset nog klopt *)
procedure test_scale_screen(sc: pbScreen);
var
sv: pScrVert = nil;
se: pScrEdge;
sa: pScrArea;
san: pScrArea;
old: pbScreen;
yval: integer;
facx: single;
facy: single;
tempf: single;
min: array [0..1] of single;
max: array [0..1] of single;
begin
  sc^.startx:= prefstax;
  sc^.starty:= prefstay;
  sc^.endx:= prefstax+prefsizx-1;
  sc^.endy:= prefstay+prefsizy-1;

  (* calculate size *)
  sv:= sc^.vertbase.first;
  min[0]:=0;
  min[1]:=0;
  max[0]:= sc^.sizex;
  max[1]:= sc^.sizey;
  while sv <>nil  do
  begin
    min[0]:= MIN2(min[0],sv^.vec.x);
    min[1]:= MIN2(min[1],sv^.vec.y);
    max[0]:= MAX2(max[0],sv^.vec.x);
    max[1]:= MAX2(max[1],sv^.vec.y);
    sv:= sv^.next;
  end;

  (* always make 0.0 left under *)
  sv:= sc^.vertbase.first;
  while sv <>nil do
  begin
    sv^.vec.x:= integer(sv^.vec.x - min[0]);
    sv^.vec.y:= integer(sv^.vec.y - min[1]);
    sv:= sv^.next;
  end;

  sc^.sizex:= integer(max[0]-min[0]);
  sc^.sizey:= integer(max[1]-min[1]);

  if (sc^.sizex<>prefsizx)or(sc^.sizey<>prefsizy) then
  begin
    facx:= prefsizx;
    facx:= facx / sc^.sizex;
    facy:= prefsizy;
    facy:= facy / sc^.sizey;

    (* make sure it fits! *)
    sv:= sc^.vertbase.first;
    while sv<>nil     do
    begin
      tempf:= sv^.vec.x*facx;
      sv^.vec.x:=  integer(tempf+0.5);
      sv^.vec.x:= sv^.vec.x + (AREAGRID-1);
      sv^.vec.x:= sv^.vec.x - ((sv^.vec.x mod AREAGRID));
      CLAMP(sv^.vec.x,0,prefsizx);
      tempf:= sv^.vec.y*facy;
      sv^.vec.y:=  integer(tempf+0.5);
      sv^.vec.y:= sv^.vec.y + (AREAGRID-1);
      sv^.vec.y:= sv^.vec.y - ((sv^.vec.y mod AREAGRID));
      CLAMP(sv^.vec.y,0,prefsizy);
      sv:= sv^.next;
    end;

    sc^.sizex:= prefsizx;
    sc^.sizey:= prefsizy;
  end;

  (* test for collapsed areas. This could happen in some blender version... *)
  sa:= sc^.areabase.first;
  while sa<>nil  do
  begin
    san:= sa^.next;
    if (sa^.v1=sa^.v2)or(sa^.v3=sa^.v4)or(sa^.v2=sa^.v3) then
    begin
      del_area(sa);
      freeN(sa);
      remlink(@sc^.areabase,sa);
    end;
    sa:= san;
  end;

  (* make each window at least HEADERY high *)
  sa:= sc^.areabase.first;
  while sa<>nil  do
  begin
    if sa^.v1^.vec.y+HEADERY>sa^.v2^.vec.y then
    begin
      (* lower edge *)
      se:= findscredge_sc(sc,sa^.v4,sa^.v1);
      if (se<>nil)and(sa^.v1<>sa^.v2) then
      begin
        select_connected_scredge(sc,se);

        (* all selected vertices get the right offset *)
        yval:= sa^.v2^.vec.y-HEADERY;
        sv:= sc^.vertbase.first;
        while sv <>nil   do
        begin
          (* if is a collapsed area *)
          if (sv<>sa^.v2)and(sv<>sa^.v3) then
          begin
            if sv^.flag<>0 then
            sv^.vec.y:= yval;
          end;
          sv:= sv^.next;
        end;
      end;
    end;
    sa:= sa^.next;
  end;
end;

procedure redraw1func; cdecl;
begin
  qenter(REDRAW,1);
  if (G.f and G_DEBUG) <> 0 then
  printf('redrawfunc\n');
end;

procedure redraw2func; cdecl;
begin
  qenter(REDRAW,2);
end;

procedure visiblefunc(dummy: integer); cdecl;
begin
  (* geen winfreeze *)
end;

procedure inputchangefunc(state: integer); cdecl;
var
win: integer;
begin
  if state<>0 then
  begin
    {$ifdef WINDOWS}
    qenter(INPUTCHANGE,glutGetWindow()); //glutGetActiveWindow());
    {$else}
    qenter(INPUTCHANGE,glutGetWindow());
    {$endif}
  end;
  if (G.f and G_DEBUG) <> 0 then
  printf('inputchangefunc %d %d\n',[state,glutGetWindow()]);
end;

procedure reshapefunc(x: integer;  y: integer); cdecl;
var
sc: pbScreen;
wx: integer;
wy: integer;
orx: integer;
ory: integer;
begin
  (* alleen voor hoofdwindow *)
  if G.curscreen=nil then
  exit;

  if (G.f and G_DEBUG) <> 0 then
  printf('reshapefunc\n');

  (*  qenter(RESHAPE, mainwin); *)
  qenter(REDRAW,mainwin);

  init_my_mainwin(mainwin);

  wx:= glutGet(GLUT_WINDOW_WIDTH);
  wy:= glutGet(GLUT_WINDOW_HEIGHT);

  orx:= glutGet(GLUT_WINDOW_X);
  ory:= displaysizey-wy-glutGet(GLUT_WINDOW_Y);

  (* Minimizing on windows causes glut to set the
   * size to 0,  which means things go bonkers
   *)
  if (wx=0)or(wy=0) then
  exit;

  prefstax:= orx;
  prefstay:= ory;
  prefsizx:= wx;
  prefsizy:= wy;

  sc:= G.main^.screen.first;
  while sc <>nil  do
  begin
    test_scale_screen(sc);
    sc:= sc^.id.next;
  end;

  testareas();
end;

var
mymdown: integer =0;

procedure mousefunc(button: integer;  state: integer;  x: integer;  y: integer); cdecl;
var
but: integer;
begin

  if button=GLUT_LEFT_BUTTON
 then
  begin
    but:= LEFTMOUSE;
    if (U.flag and TWOBUTTONMOUSE) <> 0 then
    begin
      if( G.qual and LR_ALTKEY) <> 0 then
      but:= MIDDLEMOUSE;
    end;
    if state=GLUT_DOWN then
    qenter(but,1)
    else
    qenter(but,0);

    {$ifdef WINDOWS}
    //R.winpop:= 1;
    {$endif}
  end
  else
  if button=GLUT_MIDDLE_BUTTON then
  begin
    if state=GLUT_DOWN then
    mymdown:= 1
    else
    mymdown:=0;

    if state=GLUT_DOWN then
    qenter(MIDDLEMOUSE,1)
    else
    qenter(MIDDLEMOUSE,0);
  end
  else
  if button=GLUT_RIGHT_BUTTON then
  begin
    if state=GLUT_DOWN then
    qenter(RIGHTMOUSE,1)
    else
    qenter(RIGHTMOUSE,0);
  end;
end;

procedure mousemovefunc(x: integer;  y: integer); cdecl;
begin
  (* ommappen *)
  scrmousex:= x;
  scrmousey:= prefsizy-y;

  qenter(MOUSEX,scrmousex); (* (een te veel?) *)
  qenter(MOUSEY,scrmousey);
end;

var
lastval: integer = -1;
lastkey: integer = -1;

procedure rawkeyfunc(key: integer;  val: integer;  y: integer); cdecl;
begin
  {$ifdef BEOS}
  key:= rawkey_beos(key);
  {$endif}

  {$ifdef WINDOWS}
  (* really silly: windows repeats rawkeys! *)
  if (key=212)or(key=213)or(key=216) then
  begin
    if (lastkey=key)and(lastval=val) then
    exit;

    lastval:= val;
    lastkey:= key;
  end;
  {$endif}

  if (G.f and G_DEBUG) <> 0 then
  PRINT3('x','d','d',key,val,y);

  qenter(key,val);
end;

procedure keyboardfunc(key: byte;  x: integer;  y: integer); cdecl;
begin
  if (G.f and G_DEBUG) <> 0 then
  printf('keyboardfunc: %c %d\n',[key,key]);
  qenter(KEYBD,key);
end;

procedure do_the_glut_funcs;
begin
  glutKeyboardFunc(@keyboardfunc);
  glutSpecialFunc(@rawkeyfunc);
  glutMouseFunc(@mousefunc);

  glutMotionFunc(@mousemovefunc);
  glutPassiveMotionFunc(@mousemovefunc);
  glutVisibilityFunc(@visiblefunc);
  glutEntryFunc(@inputchangefunc);
end;

function mywinopen(mode: integer;  posx, posy, sizex, sizey: smallint): integer;
var
win: integer;
begin

  glutInitDisplayMode(mode);
  glutInitWindowSize(sizex,sizey);
  glutInitWindowPosition(posx,displaysizey-sizey-posy);
  win:= glutCreateWindow('Blender');

  if win=1 then
  begin
    glutDisplayFunc(@redraw1func);
    glutReshapeFunc(@reshapefunc);
  end
  else
  begin
    glutDisplayFunc(@redraw2func);
  end;

  (* WARN! geen standaard fie *)
  //if borderless<>nil then
  //glutNoBorder();
  //glutDoWorkList();

  (* WARN! geen standaard fie *)
  {$ifdef WINDOWS}
  if (win=1)and(fullscreen<>0) then
  glutFullScreen();
  {$endif}

  (* niet netjes: alleen eerste keer! *)
  if win=1 then
  init_my_mainwin(win);

  mywinset(win);
  (* anders matrices in de war *)
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  ortho2(-0.5, sizex-0.5,-0.5, sizey-0.5);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  exit(win);
end;

(* gebruik de setprefsize() als je anders dan fullscreen wilt *)
function addscreen(name: pchar): pbScreen;
var
sc: pbScreen;
sv1: pScrVert;
sv2: pScrVert;
sv3: pScrVert;
sv4: pScrVert;
se: pScrEdge;
sa: pScrArea;
startx: smallint;
starty: smallint;
endx: smallint;
endy: smallint;
begin
  (* deze functie zet de variabele G.curscreen
   * omdat alle hulpfuncties moeten weten welk screen
   *)

  G.curscreen:=alloc_libblock(@G.main^.screen,ID_SCR,name);
    sc:= G.curscreen;

  if prefsizx<>0 then
  begin
    startx:= prefstax;
    starty:= prefstay;
  endx:= prefstax+prefsizx-1;
endy:= prefstay+prefsizy-1;
end
else
begin
  startx:=0;
  prefstax:=0;
  starty:=0;
  prefstay:=0;
  endx:= displaysizex-1;
  endy:= displaysizey-1;
  prefsizx:= displaysizex;
  prefsizy:= displaysizey;
  end;
  sc^.startx:= startx;
  sc^.starty:= starty;
  sc^.endx:= endx;
  sc^.endy:= endy;
  sc^.sizex:= sc^.endx-sc^.startx+1;
  sc^.sizey:= sc^.endy-sc^.starty+1;
  sc^.scene:= G.scene;

    if mainwin=0 then
  begin
    (* #ifndef FREE *)
    (* decodekeytab(); *)
    (* #endif *)
    mainwin:=mywinopen(GLUT_RGB or GLUT_DOUBLE or GLUT_DEPTH,sc^.startx,sc^.starty,sc^.sizex,sc^.sizey);
    sc^.mainwin:= mainwin;
    do_the_glut_funcs();
    glutIdleFunc(@screenmain);
    glDrawBuffer(GL_FRONT);
    (* glutInitDisplayMode(GLUT_SINGLE or GLUT_INDEX); *)
    glClearColor(0.45,0.45,0.45,0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawBuffer(GL_BACK);
    warp_pointer(sc^.sizex div 2,sc^.sizey div 2);
    scrmousex:= sc^.sizex div 2;
    scrmousey:= sc^.sizey div 2;
    qenter(REDRAW,1);
  end
  else
  sc^.mainwin:= mainwin;

  sv1:= addscrvert(@sc^.vertbase,0,0);
  sv2:= addscrvert(@sc^.vertbase,0,sc^.endy-sc^.starty);
  sv3:= addscrvert(@sc^.vertbase,sc^.sizex-1,sc^.sizey-1);
  sv4:= addscrvert(@sc^.vertbase,sc^.sizex-1,0);
  se:= addscredge(@sc^.edgebase,sv1,sv2);
  se:= addscredge(@sc^.edgebase,sv2,sv3);
  se:= addscredge(@sc^.edgebase,sv3,sv4);
  se:= addscredge(@sc^.edgebase,sv4,sv1);
  sa:= addscrarea(@sc^.areabase,sv1,sv2,sv3,sv4,HEADERDOWN,SPACE_EMPTY);
  G.curscreen:= sc;

  exit(sc);
end;

(* niet screen zelf vrijgeven *)
//
//procedure free_screen(sc: pbScreen); 
//var
//sa: pScrArea;
// 
//begin
//  sa:=nil; 
//  freelistN(@sc^.vertbase);
//  freelistN(@sc^.edgebase);
//  sa:= sc^.areabase.first;
//  while sa
//  do
//  begin 
//    del_area(sa); 
//    if sa=curarea
// then
//    curarea:=nil; 
//    sa:= sa^.next; 
//  end;
//  freelistN(@sc^.areabase);
//  if G.curscreen=sc
// then
//  begin 
//    G.curscreen:=nil; 
//    winqueue_break:= 1; 
//    (* overal uit queue's gaan *)
//  end;
//end;

procedure setscreen(sc: pbScreen);
var
sc1: pbScreen;
sa: pScrArea;
win: pbWindow;
firstwin: integer = 0;
mval: array [0..1] of smallint;
begin
  if sc^.full<>0 then   (* vind de bijhorende full *)
  begin
    sc1:= G.main^.screen.first;
    while sc1<>nil do
    begin
      sa:= sc1^.areabase.first;
      if sa^.full=sc then
      begin
        sc:= sc1;
        break;
      end;
      sc1:= sc1^.id.next;
    end;
    if sc1=nil then
    printf('setscreen error\n');
  end;

  (* G.curscreen de-activeren *)
  if (G.curscreen<>nil)and(G.curscreen<>sc) then
  begin

    sa:= G.curscreen^.areabase.first;
    while sa <> nil do
    begin
      if sa^.win<>0 then
      winclose(sa^.win);
      sa^.win:=0;
      if sa^.headwin<>0 then
      winclose(sa^.headwin);
      sa^.headwin:=0;

      sa^.hq:= sa^.headqueue; (* queue leeg *)
      sa^.wq:= sa^.winqueue;

      sa:= sa^.next;
    end;
  end;
  if G.curscreen<>sc then
  begin
    mywinset(sc^.mainwin);
  end;

  G.curscreen:= sc;
  G.scene:= sc^.scene;
  countall();

  (* recalculate winakt *)
  getmouseco_sc(mval);

  testareas();

  G.curscreen^.winakt:=0;
  win:= swindowbase.first;
  while win <> nil do
  begin
    if firstwin=0 then
    firstwin:= win^.id;
    if (mval[0]>=win^.xmin-1)and(mval[0]<=win^.xmax+2) then
    begin
      if (mval[1]>=win^.ymin-1)and(mval[1]<=win^.ymax+2) then
      begin
        G.curscreen^.winakt:= win^.id;
        break;
      end;
    end;
    win:= win^.next;
  end;
  (* als buhv cursor op edge staat *)
  if G.curscreen^.winakt=0 then
  G.curscreen^.winakt:= firstwin;

  areawinset(G.curscreen^.winakt); (* zet curarea *)

  qenter(DRAWEDGES,1);
  winqueue_break:= 1;     (* overal uit de queue's gaan *)

  curedge:=nil;           (* global voor move en join *)
end;

//procedure area_fullscreen; (* met curarea *)
//var
//sc: pbScreen; 
//oldscreen: pbScreen; 
//new: pScrArea;
//old: pScrArea;
//begin(* deze funktie toggelt: als area full is wordt de parent weer zichtbaar *)
//  
//  
//  
//  
//  if curarea^.full<>nil 
// then
//  begin 
//    sc:= curarea^.full; 
//    sc^.full:=nil;
//    old:= sc^.areabase.first; (* de oude screen *)
//    (* vind oude area *)
//    while old
//    do
//    begin 
//      if old.full<>nil 
// then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      old:= old.next; 
//    end;
//    if old=nil
// then
//    begin 
//      error('something wrong in areafullscreen'); 
//      exit;
//    end;
//    copy_areadata(old,curarea); 
//    old.full:=nil; 
//    free_libblock(@G.main.screen,G.curscreen); 
//    setscreen(sc); 
//  end;
//  else
//  begin 
//    (* is er maar 1 area? *)
//    if G.curscreen^.areabase.first=G.curscreen^.areabase.last
// then
//    exit;
//    if curarea^.spacetype=SPACE_INFO
// then
//    exit;
//    G.curscreen^.full:= 1; 
//    old:= curarea; 
//    oldscreen:= G.curscreen; 
//    sc:= addscreen('temp'); 
//    splitarea( {pScrArea(}sc^.areabase.first,'h',0.99);
//    (* deze zet G.curscreen *)
//    new:= sc^.areabase.first;
//    newspace(new.next,SPACE_INFO); 
//    curarea:= old; 
//    G.curscreen:= oldscreen; 
//    copy_areadata(new,curarea); 
//    (* moet voor setscreen *)
//    (* area kopieeren *)
//    curarea^.full:= oldscreen; 
//    new.full:= oldscreen; 
//    new.next.full:= oldscreen; 
//    setscreen(sc); 
//    wich_cursor(new); 
//  end;
//end;
//
//procedure copy_screen(to: pbScreen;  from: pbScreen); 
//var
//s1: pScrVert; 
//s2: pScrVert; 
//se: pScrEdge; 
//sa: pScrArea;
//lbase: ListBase; (* alles van to vrijgeven *)
//begin
//  free_screen(to);
//  duplicatelist(@to.vertbase, and from.vertbase); 
//  duplicatelist(@to.edgebase, and from.edgebase); 
//  duplicatelist(@to.areabase, and from.areabase); 
//  s1:= from.vertbase^.first; 
//  s2:= to.vertbase^.first; 
//  while s1
//  do
//  begin 
//    s1._new:= s2;
//    s2:= s2.next; 
//    s1:= s1.next; 
//  end;
//  se:= to.edgebase.first; 
//  while se
//  do
//  begin 
//    se^.v1:= se^.v1^._new;
//    se^.v2:= se^.v2^._new;
//    sortscrvert(@(se^.v1),@(se^.v2)); 
//    se:= se^.next; 
//  end;
//  sa:= to.areabase.first; 
//  while sa
//  do
//  begin 
//    sa^.v1:= sa^.v1^._new;
//    sa^.v2:= sa^.v2^._new;
//    sa^.v3:= sa^.v3^._new;
//    sa^.v4:= sa^.v4^._new;
//    sa^.win:=nil; 
//    sa^.headwin:=nil; 
//    sa^.headqueue:= sa^.hq:=nil; 
//    sa^.winqueue:= sa^.wq:=nil; 
//    duplicatespacelist(@lbase,@sa^.spacedata); 
//    sa^.spacedata:= lbase; 
//    sa:= sa^.next; 
//  end;
//  s1:= from.vertbase^.first; (* op nul zetten (nodig?) *)
//  while s1
//  do
//  begin 
//    s1._new:=nil;
//    s1:= s1.next; 
//  end;
//end;
//
//procedure duplicate_screen; 
//var
//sc: pbScreen; 
//oldscreen: pbScreen; 
//begin
//  
//  
//  if G.curscreen^.full<>nil 
// then
//  exit;
//  (* nieuw screen maken: *)
//  oldscreen:= G.curscreen; 
//  sc:= addscreen(oldscreen.id.name+2); 
//  copy_screen(sc,oldscreen); 
//  (* deze zet G.curscreen *)
//  G.curscreen:= oldscreen; 
//  setscreen(sc); 
//end;

(* ************ END SCREENBEHEER ************** *)
(* ************  JOIN/SPLIT/MOVE ************** *)

//procedure joinarea(sa: pScrArea);
//var
//sa2: pScrArea;
//san: pScrArea;
//up: pScrArea;
// 
//down: pScrArea;
// 
//right: pScrArea;
// 
//left: pScrArea;
// 
//setest: pScrEdge; 
//val: smallint;
// 
//str: array [0..Pred(64)] of char; (* welke edge? *)
//begin
//  
//  
//  up:=nil; 
//  down:=nil; 
//  right:=nil; 
//  left:=nil; 
//  
//  val:=nil; 
//  if curedge=nil
// then
//  exit;
//  (* zit edge in area? of anders: welke area *)
//  sa:= test_edge_area(sa,curedge); 
//  if sa=nil
// then
//  exit;
//  (* welke edges kunnen ermee? *)
//  (* vind richtingen met zelfde edge *)
//  sa2:= G.curscreen^.areabase.first; 
//  while sa2
//  do
//  begin 
//    if sa2<>sa
// then
//    begin 
//      setest:= findscredge(sa2.v1,sa2.v2); 
//      if curedge=setest
// then
//      right:= sa2; 
//      setest:= findscredge(sa2.v2,sa2.v3); 
//      if curedge=setest
// then
//      down:= sa2; 
//      setest:= findscredge(sa2.v3,sa2.v4); 
//      if curedge=setest
// then
//      left:= sa2; 
//      setest:= findscredge(sa2.v4,sa2.v1); 
//      if curedge=setest
// then
//      up:= sa2; 
//    end;
//    sa2:= sa2.next; 
//  end;
//  sa2:=nil; 
//  setest:=nil; 
//  if left<>nil 
// then
//  inc(val); 
//  if up<>nil 
// then
//  inc(val); 
//  if right<>nil 
// then
//  inc(val); 
//  if down<>nil 
// then
//  inc(val); 
//  if val=nil
// then
//  exit;
//  else
//  if val=1
// then
//  begin 
//    if left<>nil 
// then
//    sa2:= left; 
//    else
//    if up<>nil 
// then
//    sa2:= up; 
//    else
//    if right<>nil 
// then
//    sa2:= right; 
//    else
//    if down<>nil 
// then
//    sa2:= down; 
//  end;
//  if okee('Join')
// then
//  begin 
//    if sa2<>nil 
// then
//    begin 
//      (* nieuwe area is oude sa *)
//      if sa2=left
// then
//      begin 
//        sa^.v1:= sa2.v1; 
//        sa^.v2:= sa2.v2; 
//        addscredge(@G.curscreen^.edgebase,sa^.v2,sa^.v3); 
//        addscredge(@G.curscreen^.edgebase,sa^.v1,sa^.v4); 
//      end;
//      else
//      if sa2=up
// then
//      begin 
//        sa^.v2:= sa2.v2; 
//        sa^.v3:= sa2.v3; 
//        addscredge(@G.curscreen^.edgebase,sa^.v1,sa^.v2); 
//        addscredge(@G.curscreen^.edgebase,sa^.v3,sa^.v4); 
//      end;
//      else
//      if sa2=right
// then
//      begin 
//        sa^.v3:= sa2.v3; 
//        sa^.v4:= sa2.v4; 
//        addscredge(@G.curscreen^.edgebase,sa^.v2,sa^.v3); 
//        addscredge(@G.curscreen^.edgebase,sa^.v1,sa^.v4); 
//      end;
//      else
//      if sa2=down
// then
//      begin 
//        sa^.v1:= sa2.v1; 
//        sa^.v4:= sa2.v4; 
//        addscredge(@G.curscreen^.edgebase,sa^.v1,sa^.v2); 
//        addscredge(@G.curscreen^.edgebase,sa^.v3,sa^.v4); 
//      end;
//      del_area(sa2); 
//      (* edge en area weg *)
//      (* remlink(&G.curscreen->edgebase, setest); *)
//      (* freeN(setest); *)
//      remlink(@G.curscreen^.areabase,sa2); 
//      if curarea=sa2
// then
//      curarea:=nil; 
//      freeN(sa2); 
//      removedouble_scredges(); 
//      removenotused_scredges(); 
//      removenotused_scrverts(); 
//      testareas(); 
//      (* moet als laatste *)
//      qenter(DRAWEDGES,1); 
//      qenter(MOUSEY,-1); 
//      (* test cursor en inputwindow *)
//    end;
//  end;
//end;

(* return 0: geen split mogelijk *)
(* else return (integer) screencoordinaat splitpunt *)
function testsplitpoint(sa: pScrArea;  dir: char;  fac: single): smallint;
var
val: smallint =0;
x: smallint;
y: smallint;
begin
(* area groot genoeg? *)
  if sa^.v4^.vec.x-sa^.v1^.vec.x<=2*AREAMINX then
    exit(0);
  if sa^.v2^.vec.y-sa^.v1^.vec.y<=2*AREAMINY then
    exit(0);

    (* voor zekerheid *)
  if fac<0.0 then
  fac:=0.0;
  if fac>1.0 then
  fac:= 1.0;

  if dir='h' then
  begin
    y:= round(sa^.v1^.vec.y+fac*(sa^.v2^.vec.y-sa^.v1^.vec.y));

    if (sa^.v2^.vec.y=G.curscreen^.sizey-1)and(sa^.v2^.vec.y-y<HEADERY+EDGEWIDTH2) then
    y:= sa^.v2^.vec.y-HEADERY-EDGEWIDTH2
    else
    if (sa^.v1^.vec.y=0)and(y-sa^.v1^.vec.y<HEADERY+EDGEWIDTH2) then
    y:= sa^.v1^.vec.y+HEADERY+EDGEWIDTH2
    else
    if y-sa^.v1^.vec.y<AREAMINY then
    y:= sa^.v1^.vec.y+AREAMINY
    else
    if sa^.v2^.vec.y-y<AREAMINY then
    y:= sa^.v2^.vec.y-AREAMINY
    else
    y:= y - ((y mod AREAGRID));

    exit(y);
  end
  else
  begin
    x:= round(sa^.v1^.vec.x+fac*(sa^.v4^.vec.x-sa^.v1^.vec.x));

    if x-sa^.v1^.vec.x<AREAMINX then
    x:= sa^.v1^.vec.x+AREAMINX
    else
    if sa^.v4^.vec.x-x<AREAMINX then
    x:= sa^.v4^.vec.x-AREAMINX
    else
    x:= x - ((x mod AREAGRID));

    exit(x);
  end;
end;

procedure splitarea(sa: pScrArea;  dir: char;  fac: single);
var
sc: pbScreen;
new: pScrArea;
sv1: pScrVert;
sv2: pScrVert;
split: smallint;
begin
  if sa=nil then
  exit;

  split:= testsplitpoint(sa,dir,fac);
  if split=0 then
  exit;

  sc:= G.curscreen;
  areawinset(sa^.win);
  if dir='h' then
  begin
    (* nieuwe vertices *)
    sv1:= addscrvert(@sc^.vertbase,sa^.v1^.vec.x,split);
    sv2:= addscrvert(@sc^.vertbase,sa^.v4^.vec.x,split);
    addscredge(@sc^.edgebase,sa^.v1,sv1);
    (* nieuwe edges *)
    addscredge(@sc^.edgebase,sv1,sa^.v2);
    addscredge(@sc^.edgebase,sa^.v3,sv2);
    addscredge(@sc^.edgebase,sv2,sa^.v4);
    addscredge(@sc^.edgebase,sv1,sv2);
    new:= addscrarea(@sc^.areabase,sv1,sa^.v2,sa^.v3,sv2,sa^.headertype,sa^.spacetype);
    copy_areadata(new,sa);
    (* nieuwe areas: boven *)
    sa^.v2:= sv1;
    sa^.v3:= sv2;
    (* area onder *)
  end
  else
  begin
    (* nieuwe vertices *)
    sv1:= addscrvert(@sc^.vertbase,split,sa^.v1^.vec.y);
    sv2:= addscrvert(@sc^.vertbase,split,sa^.v2^.vec.y);
    addscredge(@sc^.edgebase,sa^.v1,sv1);
    (* nieuwe edges *)
    addscredge(@sc^.edgebase,sv1,sa^.v4);
    addscredge(@sc^.edgebase,sa^.v2,sv2);
    addscredge(@sc^.edgebase,sv2,sa^.v3);
    addscredge(@sc^.edgebase,sv1,sv2);
    new:= addscrarea(@sc^.areabase,sa^.v1,sa^.v2,sv2,sv1,sa^.headertype,sa^.spacetype);
    copy_areadata(new,sa);
    (* nieuwe areas: links *)
    sa^.v1:= sv1;
    sa^.v2:= sv2;
    (* area rechts *)
  end;

  removedouble_scrverts();
  (* dubbele vertices en edges verwijderen *)
  removedouble_scredges();
  removenotused_scredges();
  testareas();
end;

//procedure editsplitpoint; 
//var
//sa: pScrArea;
//fac: single;
//event: word;
//ok: smallint;
// 
//val: smallint; 
//split: smallint; 
//mval: array [0..1] of smallint;
//mvalo: array [0..1] of smallint;
// 
//col: array [0..2] of smallint;
//dir: char; 
//begin
//  
//  
//  
//  ok:=nil; 
//  
//  
//  
//  mvalo:=(-1010,-1010); 
//  
//  
//  sa:= test_edge_area(curarea,curedge); 
//  if sa=nil
// then
//  exit;
//  if sa^.win=nil
// then
//  exit;
//  if sa^.full<>nil 
// then
//  exit;
//  if curedge=nil
// then
//  exit;
//  if okee('Split')=nil
// then
//  exit;
//  if curedge^.v1^.vec.x=curedge^.v2^.vec.x
// then
//  dir:= 'h'; 
//  else
//  dir:= 'v'; 
//  mywinset(G.curscreen^.mainwin); 
//  glDrawBuffer(GL_FRONT); 
//  (* hoort al goede matrix te hebben *)
//  (* rekening houden met grid en minsize *)
//  while ok=nil
//  do
//  begin 
//    getmouseco_sc(mval); 
//    if mval[0]<>mvalo[0])or(mval[1]<>mvalo[1]
// then
//    begin 
//      mvalo[0]:= mval[0]; 
//      mvalo[1]:= mval[1]; 
//      if dir='h'
// then
//      begin 
//        fac:= mval[1]-(sa^.v1^.vec.y); 
//        fac:= fac div (sa^.v2^.vec.y-sa^.v1^.vec.y); 
//      end;
//      else
//      begin 
//        fac:= mval[0]-sa^.v1^.vec.x; 
//        fac:= fac div (sa^.v4^.vec.x-sa^.v1^.vec.x); 
//      end;
//      split:= testsplitpoint(sa,dir,fac); 
//      if split=nil
// then
//      begin 
//        ok:= -1; 
//      end;
//      else
//      begin 
//        if dir='h'
// then
//        begin 
//          sdrawXORline4(0,sa^.totrct.xmin,split,sa^.totrct.xmax,split); 
//          sdrawXORline4(1,sa^.totrct.xmin,-1+split,sa^.totrct.xmax,-1+split); 
//        end;
//        else
//        begin 
//          sdrawXORline4(0,split,sa^.totrct.ymin,split,sa^.totrct.ymax); 
//          sdrawXORline4(1,-1+split,sa^.totrct.ymin,-1+split,sa^.totrct.ymax); 
//        end;
//      end;
//    end;
//    event:= extern_qread(@val); 
//    if val)and(event=LEFTMOUSE
// then
//    begin 
//      if dir='h'
// then
//      begin 
//        fac:= split-(sa^.v1^.vec.y); 
//        fac:= fac div (sa^.v2^.vec.y-sa^.v1^.vec.y); 
//      end;
//      else
//      begin 
//        fac:= split-sa^.v1^.vec.x; 
//        fac:= fac div (sa^.v4^.vec.x-sa^.v1^.vec.x); 
//      end;
//      ok:= 1; 
//    end;
//    if val)and(event=ESCKEY
// then
//    begin 
//      ok:= -1; 
//    end;
//  end;
//  sdrawXORline4(-1,0,0,0,0); 
//  if ok=1
// then
//  begin 
//    splitarea(sa,dir,fac); 
//    qenter(DRAWEDGES,1); 
//  end;
//  glDrawBuffer(GL_BACK); 
//end;

procedure select_connected_scredge(sc: pbScreen;  edge: pScrEdge);
var
se: pScrEdge;
sv: pScrVert;
oneselected: integer;
dir: char;
begin
  (* select connected, alleen in de juiste richting *)
  (* 'dir' is de richting van de EDGE *)

  if edge^.v1^.vec.x=edge^.v2^.vec.x then
  dir:= 'v'
  else
  dir:= 'h';

  sv:= sc^.vertbase.first;
  while sv <>nil  do
  begin
    sv^.flag:=0;
    sv:= sv^.next;
  end;

  edge^.v1^.flag:= 1;
  edge^.v2^.flag:= 1;

  oneselected:= 1;
  while oneselected <> 0 do
  begin
    se:= sc^.edgebase.first;
    oneselected:=0;
    while se <> nil    do
    begin
      if se^.v1^.flag+se^.v2^.flag=1 then
      begin
        if dir='h' then
        if se^.v1^.vec.y=se^.v2^.vec.y then
        begin
          se^.v1^.flag:= 1;
          se^.v2^.flag:=1;
          oneselected:= 1;
        end;
        if dir='v' then
        if se^.v1^.vec.x=se^.v2^.vec.x then
        begin
          se^.v1^.flag:=1;
          se^.v2^.flag:=1;
          oneselected:= 1;
        end;
      end;
      se:= se^.next;
    end;
  end;
end;

//procedure moveareas; 
//var
//se: pScrEdge; 
//v1: pScrVert; 
//sa: pScrArea;
//addvec: vec2s; 
//vec1: array [0..1] of single;
//vec2: array [0..1] of single;
//vec3: array [0..1] of single;
//dist: integer; 
//mindist: integer;
// 
//event: word;
// 
//val: smallint; 
//split: smallint; 
//mval: array [0..1] of smallint;
//mvalo: array [0..1] of smallint;
//tel: smallint; 
//x1: smallint; 
//x2: smallint; 
//y1: smallint; 
//y2: smallint; 
//bigger: smallint; 
//smaller: smallint; 
//col: array [0..2] of smallint;
//dir: char; 
//begin
//  
//  
//  
//  
//  
//  function PdistVL2Dfl: single;
//  
//  
//  
//  
//  mindist:=1 shl 30; 
//  event:=nil; 
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
//  if curarea^.full<>nil 
// then
//  exit;
//  if curedge=nil)or(curedge^.border
// then
//  exit;
//  if curedge^.v1^.vec.x=curedge^.v2^.vec.x
// then
//  dir:= 'v'; 
//  else
//  dir:= 'h'; 
//  select_connected_scredge(G.curscreen,curedge); 
//  bigger:= smaller:=10000; 
//  sa:= G.curscreen^.areabase.first; (* nu zijn alle vertices met 'flag==1' degene die verplaatst kunnen worden. *)
//  (* we lopen de areas af en testen vrije ruimte met MINSIZE *)
//  while sa
//  do
//  begin 
//    if dir='h'
// then
//    begin 
//      (* als top of down edge select, test hoogte *)
//      if sa^.v1^.flag)and(sa^.v4^.flag
// then
//      begin 
//        if sa^.v2^.vec.y=G.curscreen^.sizey-1
// then
//        y1:= sa^.v2^.vec.y-sa^.v1^.vec.y-HEADERY-EDGEWIDTH2; 
//        else
//        (* bovenste edge *)
//        y1:= sa^.v2^.vec.y-sa^.v1^.vec.y-AREAMINY; 
//        if y1<bigger
// then
//        bigger:= y1; 
//      end;
//      else
//      if sa^.v2^.flag)and(sa^.v3^.flag
// then
//      begin 
//        if sa^.v1^.vec.y=nil
// then
//        y1:= sa^.v2^.vec.y-sa^.v1^.vec.y-HEADERY; 
//        else
//        (* onderste edge *)
//        y1:= sa^.v2^.vec.y-sa^.v1^.vec.y-AREAMINY; 
//        if y1<smaller
// then
//        smaller:= y1; 
//      end;
//    end;
//    else
//    begin 
//      (* als left of right edge select, test breedte *)
//      if sa^.v1^.flag)and(sa^.v2^.flag
// then
//      begin 
//        x1:= sa^.v4^.vec.x-sa^.v1^.vec.x-AREAMINX; 
//        if x1<bigger
// then
//        bigger:= x1; 
//      end;
//      else
//      if sa^.v3^.flag)and(sa^.v4^.flag
// then
//      begin 
//        x1:= sa^.v4^.vec.x-sa^.v1^.vec.x-AREAMINX; 
//        if x1<smaller
// then
//        smaller:= x1; 
//      end;
//    end;
//    sa:= sa^.next; 
//  end;
//  mywinset(G.curscreen^.mainwin); 
//  glDrawBuffer(GL_FRONT); 
//  getmouseco_sc(mvalo); 
//  addvec.x:=nil; 
//  addvec.y:=nil; 
//  while get_mbut()@L_MOUSE
//  do
//  begin 
//    getmouseco_sc(mval); 
//    if mval[0]<>mvalo[0])or(mval[1]<>mvalo[1]
// then
//    begin 
//      if dir='h'
// then
//      begin 
//        addvec.y:= addvec.y + (mval[1]-mvalo[1]); 
//        if addvec.y>bigger
// then
//        addvec.y:= bigger; 
//        if addvec.y<-smaller
// then
//        addvec.y:= -smaller; 
//      end;
//      else
//      begin 
//        addvec.x:= addvec.x + (mval[0]-mvalo[0]); 
//        if addvec.x>bigger
// then
//        addvec.x:= bigger; 
//        if addvec.x<-smaller
// then
//        addvec.x:= -smaller; 
//      end;
//      mvalo[0]:= mval[0]; 
//      mvalo[1]:= mval[1]; 
//      tel:=nil; 
//      se:= G.curscreen^.edgebase.first; 
//      while se
//      do
//      begin 
//        if se^.v1^.flag)and(se^.v2^.flag
// then
//        begin 
//          (* met areagrid even behelpen, verderop is OK! *)
//          x1:= se^.v1^.vec.x+addvec.x-(addvec.x mod AREAGRID); 
//          x2:= se^.v2^.vec.x+addvec.x-(addvec.x mod AREAGRID); 
//          y1:= se^.v1^.vec.y+addvec.y-(addvec.y mod AREAGRID); 
//          y2:= se^.v2^.vec.y+addvec.y-(addvec.y mod AREAGRID); 
//          sdrawXORline4(tel{++} inc(tel); ,x1,y1,x2,y2); 
//          sdrawXORline4(tel{++} inc(tel); ,x1+1,y1+1,x2+1,y2+1); 
//          tel:= tel and (3); 
//        end;
//        se:= se^.next; 
//      end;
//    end;
//    else
//    usleep(2); 
//    event:= extern_qread(@val); 
//    if val<>nil 
// then
//    begin 
//      if event=ESCKEY
// then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      if event=LEFTMOUSE
// then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      if event=SPACEKEY
// then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//  end;
//  sdrawXORline4(-1,0,0,0,0); 
//  (* wissen *)
//  v1:= G.curscreen^.vertbase^.first; 
//  while v1
//  do
//  begin 
//    if v1^.flag)and(event<>ESCKEY
// then
//    begin 
//      (* zo is AREAGRID netjes *)
//      if addvec.x)and(v1^.vec.x>0)and(v1^.vec.x<G.curscreen^.sizex-1
// then
//      begin 
//        v1^.vec.x:= v1^.vec.x + (addvec.x); 
//        if addvec.x<>bigger)and(addvec.x<>-smaller
// then
//        v1^.vec.x:= v1^.vec.x - ((v1^.vec.x mod AREAGRID)); 
//      end;
//      if addvec.y)and(v1^.vec.y>0)and(v1^.vec.y<G.curscreen^.sizey-1
// then
//      begin 
//        v1^.vec.y:= v1^.vec.y + (addvec.y); 
//        v1^.vec.y:= v1^.vec.y + (AREAGRID-1); 
//        v1^.vec.y:= v1^.vec.y - ((v1^.vec.y mod AREAGRID)); 
//        (* with these lines commented out you can pull the top bar exact to the screen border. *)
//        (* if(addvec.y != bigger and addvec.y != -smaller) { *)
//        (* } *)
//      end;
//    end;
//    v1^.flag:=nil; 
//    v1:= v1^.next; 
//  end;
//  if event<>ESCKEY
// then
//  begin 
//    removedouble_scrverts(); 
//    removedouble_scredges(); 
//    testareas(); 
//  end;
//  glDrawBuffer(GL_BACK); 
//  qenter(DRAWEDGES,1); 
//end;
//
//procedure scrollheader; 
//var
//mval: array [0..1] of smallint;
//mvalo: array [0..1] of smallint;
//begin
//  
//  
//  if curarea^.headbutlen<curarea^.winx
// then
//  begin 
//    curarea^.headbutofs:=nil; 
//  end;
//  else
//  if curarea^.headbutofs+curarea^.winx>curarea^.headbutlen
// then
//  begin 
//    curarea^.headbutofs:= curarea^.headbutlen-curarea^.winx; 
//  end;
//  getmouseco_sc(mvalo); 
//  while get_mbut()@M_MOUSE
//  do
//  begin 
//    getmouseco_sc(mval); 
//    if mval[0]<>mvalo[0]
// then
//    begin 
//      curarea^.headbutofs:= curarea^.headbutofs - ((mval[0]-mvalo[0])); 
//      if curarea^.headbutlen-curarea^.winx<curarea^.headbutofs
// then
//      curarea^.headbutofs:= curarea^.headbutlen-curarea^.winx; 
//      if curarea^.headbutofs<0
// then
//      curarea^.headbutofs:=nil; 
//      defheadchange(); 
//      if curarea^.headdraw<>nil 
// then
//      curarea^.headdraw(); 
//      screen_swapbuffers(); 
//      mvalo[0]:= mval[0]; 
//    end;
//    else
//    usleep(2); 
//  end;
//end;
//
//function select_area(spacetype: integer): integer; 
//var
//sa: pScrArea;
//sact: pScrArea;
//win: pbWindow; 
//tot: integer;
// 
//event: word;
//val: smallint; 
//mval: array [0..1] of smallint;
//begin(* vanuit editroutines aanroepen, als er meer area's
//    * zijn van type 'spacetype' kan er een area aangegeven worden
//    *)
//  
//  
//  
//  tot:=nil; 
//  
//  
//  
//  sa:= G.curscreen^.areabase.first; 
//  while sa
//  do
//  begin 
//    if sa^.spacetype=spacetype
// then
//    begin 
//      sact:= sa; 
//      inc(tot); 
//    end;
//    sa:= sa^.next; 
//  end;
//  if tot=nil
// then
//  begin 
//    error('Can't do this! Open correct window');
//    begin
//      result:=nil; 
//      exit;
//    end;
//  end;
//  if tot=1
// then
//  begin 
//    if curarea<>sact
// then
//    areawinset(sact.win); 
//    begin
//      result:= 1; 
//      exit;
//    end;
//  end;
//  else
//  if tot>1
// then
//  begin 
//    tempcursor(GLUT_CURSOR_HELP); 
//    while 1
//    do
//    begin 
//      event:= extern_qread(@val); 
//      if val<>nil 
// then
//      begin 
//        if event=ESCKEY
// then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        if event=LEFTMOUSE
// then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        if event=SPACEKEY
// then
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//    end;
//    tempcursor(CURSOR_STD); 
//    getmouseco_sc(mval); 
//    (* recalculate winakt *)
//    G.curscreen^.winakt:=nil; 
//    win:= swindowbase^.first; 
//    while win
//    do
//    begin 
//      if mval[0]>=win.xmin-1)and(mval[0]<=win.xmax+2
// then
//      begin 
//        if mval[1]>=win.ymin-1)and(mval[1]<=win.ymax+2
// then
//        begin 
//          G.curscreen^.winakt:= win.id; 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//      end;
//      win:= win.next; 
//    end;
//    if event=LEFTMOUSE
// then
//    begin 
//      sa:= areawinar[G.curscreen^.winakt]; 
//      if sa^.spacetype=spacetype
// then
//      areawinset(G.curscreen^.winakt); 
//      else
//      begin 
//        error('wrong window'); 
//        begin
//          result:=nil; 
//          exit;
//        end;
//      end;
//    end;
//  end;
//  if event=LEFTMOUSE
// then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  else
//  begin
//    result:=nil; 
//    exit;
//  end;
//end;

(* ************  END JOIN/SPLIT/MOVE ************** *)

(* **************** DRAW SCREENEDGES ***************** *)
const
EDGE_EXTEND = 3;

procedure drawedge(x1: smallint;  y1: smallint;  x2: smallint;  y2: smallint);
var
a: integer;
dir: integer;
v1: vec2f;
v2: vec2f;
begin
  v1.x:= x1;
  v1.y:= y1;
  v2.x:= x2;
  v2.y:= y2;
  if v1.x=v2.x then
  begin
    (* vertikaal *)
    dir:= integer(v1.y-v2.y);

    if dir>0 then
    dir:= 1
    else
    dir:= -1;

    if dir=-1 then
    begin
      if v1.y=0 then
      v1.y:= v1.y - (EDGE_EXTEND);

      if v2.y=G.curscreen^.sizey then
      v2.y:= v2.y + (EDGE_EXTEND);
    end
    else
    begin
      if v2.y=0 then
      v2.y:= v2.y - (EDGE_EXTEND);

      if v1.y=G.curscreen^.sizey then
      v1.y:= v1.y + (EDGE_EXTEND);
    end;

    v1.y:= v1.y - (dir*EDGEWIDTH2);
    v2.y:= v2.y + (dir*EDGEWIDTH2);
    v1.x:= v1.x + (EDGEWIDTH2);
    v2.x:= v2.x + (EDGEWIDTH2);

    for{while} a:=0 to Pred(EDGEWIDTH)     do
    begin
      cpack(edcol[a]);
      LINE2F(@v1,@v2);
      v1.x -= 1;
      v2.x -= 1; (* en dit voor de afgeronde tuitjes *)

      if a<EDGEWIDTH2 then
      begin
        v1.y:= v1.y + (dir);
        v2.y:= v2.y - (dir);
      end
      else
      begin
        v1.y:= v1.y - (dir);
        v2.y:= v2.y + (dir);
      end;
    end;
  end
  else
  begin
    (* horizontaal *)
    dir:= integer(v1.x-v2.x);

    if dir>0 then
    dir:= 1
    else
    dir:= -1;

    if dir=-1 then
    begin
      if v1.x=0 then
      v1.x:= v1.x - (EDGE_EXTEND);

      if v2.x=G.curscreen^.sizex then
      v2.x:= v2.x + (EDGE_EXTEND);
    end
    else
    begin
      if v2.x=0 then
      v2.x:= v2.x - (EDGE_EXTEND);

      if v1.x=G.curscreen^.sizex then
      v1.x:= v1.x + (EDGE_EXTEND);
    end;
    v1.x:= v1.x - (dir*EDGEWIDTH2);
    v2.x:= v2.x + (dir*EDGEWIDTH2);
    v1.y:= v1.y - (EDGEWIDTH2);
    v2.y:= v2.y - (EDGEWIDTH2);

    for{while} a:=0 to Pred(EDGEWIDTH)     do
    begin
      cpack(edcol[a]);
      LINE2F(@v1,@v2);

      v1.y += 1;
      v2.y += 1; (* en dit voor de afgeronde tuitjes *)

      if a<EDGEWIDTH2 then
      begin
        v1.x:= v1.x + (dir);
        v2.x:= v2.x - (dir);
      end
      else
      begin
        v1.x:= v1.x - (dir);
        v2.x:= v2.x + (dir);
      end;
    end;
  end;
end;

procedure drawscredge(se: pScrEdge);
var
sc: pbScreen;
v1: pvec2s;
v2: pvec2s;
begin
  sc:= G.curscreen;

  v1:= @(se^.v1^.vec);
  v2:= @(se^.v2^.vec);

  se^.border:= 1; (* borders screen niet tekenen *)

  (* vanwege resolutie verschillen (PC/SGI files) de linit een
    * beetje afronden?
    *)

  if (v1^.x<=1)and(v2^.x<=1) then
  exit;

  if (v1^.x>=sc^.sizex-2)and(v2^.x>=sc^.sizex-2) then
  exit;

  if (v1^.y<=1)and(v2^.y<=1) then
  exit;

  if (v1^.y>=sc^.sizey-2)and(v2^.y>=sc^.sizey-2) then
  exit;

  se^.border:=0;

  drawedge(v1^.x,v1^.y,v2^.x,v2^.y);
end;

var {was static}
firsttime: integer = 1;


procedure drawscreen;
var
se: pScrEdge;
begin
  mywinset(G.curscreen^.mainwin);
  ortho2(-0.5, {single(}G.curscreen^.sizex-0.5,-0.5, {single(}G.curscreen^.sizey-0.5);

  (* edges in mainwin *)
  //{$if not defined(BEOS ) and not defined(WINDOWS) and not defined(PPC) and not defined(MESA31)}
  //glDrawBuffer(GL_FRONT_AND_BACK);
  //{$endif}

  se:= G.curscreen^.edgebase.first;
  while se <>nil  do
  begin
    drawscredge(se);
    se:= se^.next;
  end;

  glDrawBuffer(GL_BACK);

  {$if defined(BEOS) or defined(WINDOWS) or defined(PPC) or defined(MESA31)}
  glDrawBuffer(GL_FRONT);
  (* can't do front+back *)
  se:= G.curscreen^.edgebase.first;
  while se<>nil  do
  begin
    drawscredge(se);
    se:= se^.next;
  end;
  glDrawBuffer(GL_BACK);
  {$endif}

  firsttime:=0;
  (* for some reason the drawedges is called very late, but then the graphics is initialized correctly! *)
end;

(* ********************************* *)

//function default_foursplit: pbScreen; 
//var
//sc: pbScreen; 
//sa: pScrArea;
//vd: pView3D; 
//tel: integer; 
//begin
//  
//  
//  
//  
//  sc:= addscreen('screen'); 
//  splitarea( {pScrArea(}sc^.areabase.first,'h',0.25);
//  splitarea( {pScrArea(}sc^.areabase.last,'h',0.49);
//  splitarea( {pScrArea(}sc^.areabase.last,'h',0.99);
//  sa:= sc^.areabase.last;
//  newspace(sa,SPACE_INFO); 
//  sa:= sc^.areabase.first;
//  splitarea(sa^.next,'v',0.5); 
//  (* sa->headertype= NOHEADER; *)
//  (* alle nieuwe areas komen op einde lijst! *)
//  sa:= sc^.areabase.first;
//  splitarea(sa^.next,'v',0.5); 
//  sa:= sc^.areabase.first;
//  sa^.headertype:= HEADERTOP; 
//  newspace(sa,SPACE_BUTS); 
//  sa:= sa^.next; 
//  tel:= 1; (* zet de view3d's en geef ze de juiste aanzicht *)
//  while sa
//  do
//  begin 
//    if sa^.spacetype=nil
// then
//    begin 
//      newspace(sa,SPACE_VIEW3D); 
//      vd:= sa^.spacedata.first; 
//      vd.persp:=nil; 
//      if tel=1
// then
//      begin 
//        vd.view:= 1; 
//        vd.viewquat[0]:= fcos(M_PI div 4.0); 
//        vd.viewquat[1]:= -fsin(M_PI div 4.0); 
//      end;
//      else
//      if tel=2
// then
//      begin 
//        vd.view:= 3; 
//        vd.viewquat[0]:=nil.5; 
//        vd.viewquat[1]:= -0.5; 
//        vd.viewquat[2]:= -0.5; 
//        vd.viewquat[3]:= -0.5; 
//      end;
//      else
//      if tel=3
// then
//      begin 
//        vd.view:= 7; 
//      end;
//      else
//      vd.persp:= 1; 
//      inc(tel); 
//    end;
//    sa:= sa^.next; 
//  end;
//  testareas(); 
//  begin
//    result:= sc; 
//    exit;
//  end;
//end;

function default_twosplit: pbScreen;
var
sc: pbScreen;
sa: pScrArea;
begin
  sc:= addscreen('screen');

  splitarea(pScrArea(sc^.areabase.first),'h',0.99);
  sa:= sc^.areabase.first;
  newspace(sa,SPACE_VIEW3D);
  newspace(sa^.next,SPACE_INFO);

    exit(sc);
end;

procedure initscreen; 
begin
  getdisplaysize(); 
  (* opent en initialiseert *)
  default_twosplit(); 
end;

initialization
  winset := @mywinset;
  winget := @mywinget;
  //ortho := @myortho;
  ortho2 := @myortho2;
  winclose := @mywinclose;

end.
