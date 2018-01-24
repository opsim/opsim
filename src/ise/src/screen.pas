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
  ctypes, cfuncs,
  util, blender, arithb;

const
MAXWIN = 128;
(* Als je EDGEWIDTH verandert, ook globale array edcol[] goedzetten *)
EDGEWIDTH = 5;
EDGEWIDTH2 = (2);
AREAGRID = 4;
AREAMINX = 32;
HEADERY = 21;
AREAMINY = (HEADERY+EDGEWIDTH);
NOHEADER =nil;
HEADERDOWN = 1;
HEADERTOP = 2;
(* sa->win_swap *)
WIN_FRONT_OK = 1;
WIN_BACK_OK = 2;
WIN_EQUAL = 3;
L_SCROLL = 1; (* left scrollbar *)
R_SCROLL = 2;
VERT_SCROLL = 3;
T_SCROLL = 4;
B_SCROLL = 8;
HOR_SCROLL = 12;
SPACE_EMPTY =0; (* deze getallen moeten kleiner dan 100 zijn ivm buttoncodes *)
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
(* queue events: naar blendef.h *)

type
pbScreen = ^bScreen;
bScreen = record
id: ID;
vertbase: ListBase;
edgebase: ListBase;
areabase: ListBase;
scene: pScene;
startx: smallint;
endx: smallint;
starty: smallint;
endy: smallint; (* framebuffer coords *)
sizex: smallint;
sizey: smallint;
scenenr: smallint;
screennr: smallint; (* alleen voor pupmenu *)
full: smallint;
rt: smallint;
mainwin: smallint;
winakt: smallint;
end;

pScrVert = ^ScrVert;
ppScrVert = ^pScrVert;
ScrVert = record
next: pScrVert;
prev: pScrVert;
new: pScrVert;
vec: vec2s;
flag: integer;
end;

pScrEdge = ^ScrEdge;
ScrEdge = record
next: pScrEdge;
prev: pScrEdge;
v1: pScrVert;
v2: pScrVert;
border: smallint; (* 1 als op rand screen *)
flag: smallint;
pad: integer;
end;

pScrArea = ^ScrArea;
ScrArea = record
next: pScrArea;
prev: pScrArea;
v1: pScrVert;
v2: pScrVert;
v3: pScrVert;
v4: pScrVert;
full: pbScreen; (* als area==full, dit is de parent *)
headqueue: psmallint;
hq: psmallint;
winqueue: psmallint;
wq: psmallint;
winmat: array [0..Pred(4),0..Pred(4)] of cfloat;
totrct: rcti;
headrct: rcti;
winrct: rcti;
headwin: smallint;
win: smallint;
headertype: smallint; (* 0=niets, 1= down, 2= up *)
spacetype: byte;
butspacetype: byte; (* SPACE_...  *)
winx: smallint;
winy: smallint; (* size *)
head_swap: byte;
head_equal: char;
win_swap: char;
win_equal: char;
headbutlen: smallint;
headbutofs: smallint;
cursor: smallint;
rt: smallint;
headchange: procedure;
winchange: procedure;
headdraw: procedure;
windraw: procedure;
(* opmerking: fuctiepointers met types binnen de halen geeft problemen met SDNA *)
headqread: procedure;
winqread: procedure;
spacedata: ListBase;
end;

(* ********** MYWINDOW ******* *)
pbWindow = ^bWindow;
bWindow = record
next: pbWindow;
prev: pbWindow;
id: integer;
parent_id: integer;
xmin: integer;
xmax: integer;
ymin: integer;
ymax: integer;
viewmat: Mat4;
winmat: Mat4;
end;

var
winset: procedure(wid: integer);
//winget = mywinget;
//ortho = myortho;
ortho2: procedure(x1, x2, y1, y2: single);

//function window(a: integer; b: integer; c: integer; d: integer; e: integer; f: integer): integer; {<= !!!6 unknown macro}
winclose : procedure (winid: integer);
//loadmatrix = myloadmatrix;
//getmatrix = mygetmatrix;
//multmatrix = mymultmatrix;
//getsize = mygetsize;
//getorigin = mygetorigin;
//{swindowbase: ListBase; }{<= !!!5 external variable}(* uit mywindow.c *)
//(* ********** END MYWINDOW ******* *)
//(* GLOBALS   (uit screen.c) *)
//{curarea: pScrArea; }{<= !!!5 external variable}
//{displaysizex: integer; }{<= !!!5 external variable}
//{displaysizey: integer; }{<= !!!5 external variable}(* mywindow.c *)
//
//function get_qual: smallint;
//
//function get_mbut: smallint;
//
//procedure myglClearColor(a: float;  b: float;  c: float;  d: float);
//
//procedure custom_cursor(data1: pchar;  data2: pchar;  size: integer);
//
//procedure drawmode(mode: integer);
//
//function fmfindfont(name: pchar): pinteger;
//
//function fmgetstrwidth(handle: pinteger;  str: pchar): integer;
//
//procedure fminit;
//
//procedure fmprstr(str: pchar);
//
//procedure fmrotatepagematrix(degrees: float);
//
//function fmscalefont: pinteger;
//
//procedure fmsetfont(type: pinteger);
//
//function getbutton(val: integer): smallint;
//
//procedure getmouse(mval: psmallint);
//
//function getqual: smallint;
//
//function give_new_winid: integer;
//
//procedure init_my_mainwin(win: integer);
//
//procedure loadmatrix_win(mat: array [0..,0..Pred(4)] of float;  swin: integer);
//
//procedure myContextSetup;
//
//procedure myCopySubBuffer(x: integer;  y: integer;  sx: integer;  sy: integer);
//
//procedure myXcolor(col: integer);
//
//procedure myXdrawline(x1: integer;  y1: integer;  x2: integer;  y2: integer);
//
//procedure myXrect(x1: integer;  y1: integer;  x2: integer;  y2: integer);
//
//procedure myXrectf(x1: integer;  y1: integer;  x2: integer;  y2: integer);
//
//procedure my_get_frontbuffer_image(x: integer;  y: integer;  sx: integer;  sy: integer);
//
//procedure my_put_frontbuffer_image;
//
//procedure mygetmatrix(mat: array [0..,0..Pred(4)] of float);
//
//procedure mygetorigin(x: pinteger;  y: pinteger);
//
//procedure mygetsingmatrix(mat: array [0..,0..Pred(4)] of float);
//
//procedure mygetsize(x: pinteger;  y: pinteger);
//
//procedure mygetsuborigin(x: pinteger;  y: pinteger);
//
//procedure myloadmatrix(mat: array [0..,0..Pred(4)] of float);
//
//procedure mymultmatrix(mat: array [0..,0..Pred(4)] of float);
//
//procedure myortho(x1: float;  x2: float;  y1: float;  y2: float;  n: float;  f: float);
//
//procedure myortho2(x1: float;  x2: float;  y1: float;  y2: float);
//
//procedure myswapbuffers;
//
//function myswinopen(winid: integer;  xmin: integer;  xmax: integer;  ymin: integer;  ymax: integer): integer;
//
//procedure mywinclose(winid: integer);
//
//procedure mywindow(x1: float;  x2: float;  y1: float;  y2: float;  n: float;  f: float);
//
//function mywinexist(wid: integer): integer;
//
//function mywinget: integer;
//
//procedure mywinposition(swin: integer;  xmin: integer;  xmax: integer;  ymin: integer;  ymax: integer);
//(* let op: andere syntax *)
//
//procedure mywinset(wid: integer);
//
//procedure overdrawdump;
//
//procedure screendump;
//
//procedure sdrawXORline(x0: integer;  y0: integer;  x1: integer;  y1: integer);
//
//procedure sdrawXORline4(nr: integer;  x0: integer;  y0: integer;  x1: integer;  y1: integer);
//
//procedure setlinestyle(nr: integer);
//
//procedure setvaluator(dev: smallint;  a: smallint;  b: smallint;  c: smallint);
//
//procedure write_screendump(name: pchar);
//(* screen.c *)
//
//procedure addafterqueue(win: smallint;  event: ushort;  val: smallint);
//
procedure addqueue(win: smallint;  event: cushort;  val: smallint);
//
//function addscreen(name: pchar): pbScreen;
//(* gebruik de setprefsize(); als je anders dan fullscreen wilt *)
//
//function afterqtest: integer;
//
//procedure append_afterqueue;
//
//procedure area_fullscreen;
//(* met curarea *)
//
//procedure areawinset(win: smallint);
//
//procedure calc_arearcts(sa: pScrArea);
//
//procedure closeareawin(sa: pScrArea);
//
//procedure closeheadwin(sa: pScrArea);
//
//function closest_bigger_area: pScrArea;
//
//procedure copy_areadata(sa1: pScrArea;  sa2: pScrArea);
//
//procedure copy_screen(to: pbScreen;  from: pbScreen);
//
//procedure decodekeytab;
//
//function default_foursplit: pbScreen;
//
//function default_twosplit: pbScreen;
//
//procedure defheadchange;

procedure defheaddraw;

//procedure defheadqread(sa: pScrArea);
//
//procedure defwinchange;
//
//procedure defwindraw;
//
//procedure defwinmat;
//
//procedure defwinqread(sa: pScrArea);
//
//procedure del_area(sa: pScrArea);
//
//procedure do_the_glut_funcs;
//
//procedure drawedge(x1: smallint;  y1: smallint;  x2: smallint;  y2: smallint);
//
//procedure drawscredge(se: pScrEdge);
//
//procedure drawscreen;
//
//procedure duplicate_screen;
//
//procedure editsplitpoint;
//
//function ext_qtest: smallint;
//(* extern      ushort special_qread(short *val); *)
//
//function extern_qread(val: psmallint): ushort;
//
//function findcurarea: pScrArea;
//
//function findscredge(v1: pScrVert;  v2: pScrVert): pScrEdge;
//
//procedure free_screen(sc: pbScreen);
//
//procedure getdisplaysize;
//
//procedure getmouseco_areawin(mval: psmallint);
//(* interne area coordinaten *)
//
//procedure getmouseco_headwin(mval: psmallint);
//(* interne area coordinaten *)
//
//procedure getmouseco_sc(mval: psmallint);
//(* screen coordinaten *)
//
//procedure header_back_to_front(sa: pScrArea);
//
//procedure header_front_to_back(sa: pScrArea);
//
//procedure headerbox(selcol: integer;  width: integer);
//
//procedure headmenu(sa: pScrArea);
//
//procedure init_screen_cursors;

procedure initscreen;

//procedure inputchangefunc(state: integer);
//
//function is_allowed_to_change_screen(new: pbScreen): integer;
//
//function isqueued(dev: smallint): smallint;
//
//procedure joinarea(sa: pScrArea);
//
//procedure keyboardfunc(key: uchar;  x: integer;  y: integer);
//
//procedure markdirty_all;
//
//procedure mousefunc(button: integer;  state: integer;  x: integer;  y: integer);
//
//procedure mousemovefunc(x: integer;  y: integer);
//
//procedure moveareas;
//
//function myqtest: ushort;
//
//function mywinopen(mode: integer;  posx: smallint;  posy: smallint;  sizex: smallint;  sizey: smallint): integer;
//
//procedure openareawin(sa: pScrArea);
//
//procedure openheadwin(sa: pScrArea);
//
//procedure qenter(event: ushort;  val: smallint);
//
//function qread(val: psmallint): ushort;
//
//procedure qreset;
//
//function qtest: ushort;
//
//procedure rawkeyfunc(key: integer;  val: integer;  y: integer);
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
//procedure reshapefunc(x: integer;  y: integer);
//
//procedure rgbmul(col: puint;  mul: integer);
//
//function screen_qread(val: psmallint): ushort;
//
//procedure screen_swapbuffers;
//
//procedure screenmain;
//
//procedure scrollheader;
//
//function select_area(spacetype: integer): integer;
//
//procedure set_cursonedge(mx: smallint;  my: smallint);
//
//procedure setcursor_space(spacetype: integer;  cur: smallint);
//
//procedure setdisplaysize(ex: integer;  ey: integer);
//
//procedure setprefsize(stax: integer;  stay: integer;  sizx: integer;  sizy: integer);
//
//procedure setscreen(sc: pbScreen);
//
//procedure splitarea(sa: pScrArea;  dir: char;  fac: float);
//
//procedure start_autosave(val: integer);
//
//procedure tempcursor(curs: integer);
//
//function test_edge_area(sa: pScrArea;  se: pScrEdge): pScrArea;
//
//procedure test_scale_screen(sc: pbScreen);
//
//procedure testareas;
//
//function testsplitpoint(sa: pScrArea;  dir: char;  fac: float): smallint;
//
//procedure visiblefunc(dummy: integer);
//
//procedure waitcursor(val: integer);

procedure wich_cursor(sa: pScrArea);

//procedure window_front_to_back(sa: pScrArea);
//(* space.c *)
//
//procedure allqueue(event: ushort;  val: smallint);
//
//procedure allspace(event: ushort;  val: smallint);
//
//procedure changebutspace;
//
//procedure changeview2d;
//
//procedure changeview3d;
//
//procedure copy_view3d_lock(val: smallint);
//
//procedure drawemptyspace;
//
//procedure drawinfospace;
//
//procedure duplicatespacelist(lb1: pListBase;  lb2: pListBase);
//
//procedure extern_set_butspace(fkey: integer);
//
//procedure force_draw;
//
//procedure freespacelist(lb: pListBase);
//
//procedure handle_view3d_lock;
//
//procedure init_butspace(sa: pScrArea);
//
//procedure init_filespace(sa: pScrArea);
//
//procedure init_imagespace(sa: pScrArea);
//
//procedure init_oopsspace(sa: pScrArea);
//
//procedure init_seqspace(sa: pScrArea);
//
//procedure init_v2d_oops(v2d: pView2D);
//
//procedure initipo(sa: pScrArea);
//
//procedure initview3d(sa: pScrArea);
//
//procedure newspace(sa: pScrArea;  type: integer);
//
//procedure set_func_space(sa: pScrArea);
//
//procedure set_rects_butspace(buts: pSpaceButs);
//
//procedure winqread3d(event: ushort;  val: smallint);
//
//procedure winqreadbutspace(event: ushort;  val: smallint);
//
//procedure winqreadimagespace(event: ushort;  val: smallint);
//
//procedure winqreadinfospace(event: ushort;  val: smallint);
//
//procedure winqreadipo(event: ushort;  val: smallint);
//
//procedure winqreadoopsspace(event: ushort;  val: smallint);
//
//procedure winqreadsequence(event: ushort;  val: smallint);
//{$endif}
var
displaysizex: integer =0;
displaysizey: integer =0;

implementation

uses
  GL, GLU, GLUT,
  blenglob, library_, blendef, mywindow, graphics,
  space, Button;

//{$include "graphics.h"}
//{$include "render.h"}
//{$ifdef __sgi}
//{$include <sys/syssgi.h>}
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
curarea: pScrArea = nil;
//curedge: pScrEdge = nil;
//fullscreen: word = 1; 
//borderless: word = 1; 
prefsizx: integer =0;
prefsizy: integer =0;
prefstax: integer =0;
prefstay: integer =0;
scrmousex: smallint;
scrmousey: smallint; (* worden gezet door mousecallback *)
mainwin: smallint =0;
//winqueue_break: smallint =0;
//cursonedge: smallint =0;
//keycode: array [0..Pred(100)] of smallint; 
//var {was static}
//mainqueue: array [0..Pred(MAXQUEUE)] of smallint; 
//var
areawinar: array [0..Pred(MAXWIN)] of pScrArea;

(* uint edcol[EDGEWIDTH]= {0x0, 0x303030, 0x606060, 0x808080, 0x909090, 0xF0F0F0, 0x0}; *)
//uint edcol[EDGEWIDTH]= {0x0, 0x505050, 0x909090, 0xF0F0F0, 0x0};

//edcol: array [0..Pred(EDGEWIDTH)] of uint = ($0,$505050,$909090,$F0F0F0,$0);
//autosavetime: integer; (* ********* Funkties *********** *)
//
//procedure getdisplaysize; 
//
//function default_foursplit: pbScreen; 
//
//function default_twosplit: pbScreen; 
//
//function findscredge(v1: pScrVert;  v2: pScrVert): pScrEdge; 
//
//procedure setscreen(sc: pbScreen); 
//
//procedure drawscreen; 
//
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
//procedure addqueue(win: smallint;  event: ushort;  val: smallint); 
//
//procedure editsplitpoint; 
//
//procedure splitarea(sa: pScrArea;  dir: char;  fac: float); 
//
//procedure joinarea(sa: pScrArea); 
//
//procedure select_connected_scredge(sc: pbScreen;  curedge: pScrEdge); 
//var {was static}
//hor_ptr_bits: array [0..] of ushort = ($0000,$0000,$0000,$0000,$1008,$1818,$1c38,$ffff,$ffff,$1c38,$1818,$1008,$0000,$0000,$0000,$0000,$0000,$0000,$0000,$381c,$3c3c,$3e7c,$ffff,$ffff,$ffff,$ffff,$3e7c,$3c3c,$381c,$0000,$0000,$0000,); 
//vert_ptr_bits: array [0..] of ushort = ($0180,$0180,$0180,$0ff0,$07e0,$03c0,$0180,$0,$0,$0180,$03c0,$07e0,$0ff0,$0180,$0180,$0180,$03c0,$03c0,$1ff8,$1ff8,$1ff8,$0ff0,$07e0,$03c0,$03c0,$07e0,$0ff0,$1ff8,$1ff8,$1ff8,$03c0,$03c0,); 
//win_ptr_bits: array [0..] of ushort = ($0000,$0180,$0180,$0180,$0000,$89b3,$fdb3,$fdb7,$b5bf,$b5bb,$85b3,$0000,$0180,$0180,$0180,$0000,$03c0,$03c0,$03c0,$03c0,$FFFF,$FFFF,$FFFF,$FFFF,$FFFF,$FFFF,$FFFF,$FFFF,$03c0,$03c0,$03c0,$03c0); 
//implementation
//
//
//procedure init_screen_cursors; 
//begin
//end;
//
//procedure waitcursor(val: integer); 
//var
//sa: pScrArea; 
//oldwin: integer; 
//begin
//  
//  
//  if G.curscreen=nil
//  then
//  exit;
//  if G.background<>nil 
//  then
//  exit;
//  if val<>nil 
//  then
//  begin 
//    (* only once: for time-cursor *)
//    if R.flag and R_RENDERING
//    then
//    exit;
//    if R.win)and(R.win=G.curscreen^.winakt
//    then
//    begin 
//      oldwin:= G.curscreen^.winakt; 
//      mywinset(G.curscreen^.mainwin); 
//      glutSetCursor(CURSOR_WAIT); 
//      mywinset(oldwin); 
//    end;
//    else
//    glutSetCursor(CURSOR_WAIT); 
//  end;
//  else
//  if G.curscreen^.winakt>3
//  then
//  begin 
//    if R.flag and R_RENDERING
//    then
//    exit;
//    sa:= areawinar[G.curscreen^.winakt]; 
//    if sa^.cursor)and(sa^.win=G.curscreen^.winakt
//    then
//    glutSetCursor(sa^.cursor); 
//    else
//    glutSetCursor(CURSOR_STD); 
//  end;
//  else
//  glutSetCursor(CURSOR_STD); 
//end;
//
//procedure tempcursor(curs: integer); 
//var
//sa: pScrArea; 
//begin
//  
//  if G.curscreen=nil
//  then
//  exit;
//  if curs<>CURSOR_STD
//  then
//  glutSetCursor(curs); 
//  else
//  if G.curscreen^.winakt<>nil 
//  then
//  begin 
//    sa:= areawinar[G.curscreen^.winakt]; 
//    if sa^.win=G.curscreen^.winakt
//    then
//    glutSetCursor(sa^.cursor); 
//    else
//    glutSetCursor(CURSOR_STD); 
//  end;
//end;

procedure wich_cursor(sa: pScrArea);
begin
  sa^.cursor:= CURSOR_STD; (* o.a. als nieuwe space gemaakt *)

  if sa^.spacetype=SPACE_VIEW3D   then
  begin
    //if G.obedit<>nil     then
    //sa^.cursor:= CURSOR_EDIT
    //else
    //if G.f and G_VERTEXPAINT    then
    //sa^.cursor:= CURSOR_VPAINT
    //else
    //if G.f and G_FACESELECT     then
    //sa^.cursor:= CURSOR_FACESEL;
  end;

  if sa^.win=G.curscreen^.winakt  then
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
//    sa:= sc.areabase.first; 
//    while sa
//    do
//    begin 
//      if sa^.spacetype=spacetype
//      then
//      begin 
//        sa^.cursor:= cur; 
//        if cur=nil
//        then
//        wich_cursor(sa); 
//        (* extra test, bv nodig bij verlaten editmode in vertexpaint *)
//        if sc=G.curscreen)and(sc.winakt=sa^.win
//        then
//        glutSetCursor(sa^.cursor); 
//      end;
//      sa:= sa^.next; 
//    end;
//    sc:= sc.id.next; 
//  end;
//end;
//(* *********** CODETAB ******************* *)
//
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
//  then
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
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    ihash[a]:= b; 
//  end;
//  for{while} a:=nil to Pred(256) {a++}
//  do
//  ihash[256+a]:= ihash[a]; 
//  hus1:= hash[sl[0]+hash[sl[2]]]; 
//  hus2:= hash[sl[1]+hash[sl[3]]]; 
//  c:= {!!!a type cast? =>} {pchar(}keycode; (* bereken hus1 en hus2 ahv sleutel *)
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
//(* *********  IN/OUT  ************* *)
//(* screen coordinaten *)
//
//procedure getmouseco_sc(mval: psmallint); 
//begin
//  if G.curscreen=nil
//  then
//  exit;
//  getmouse(mval); 
//  mval[0]:= mval[0] - (G.curscreen^.startx); 
//  mval[1]:= mval[1] - (G.curscreen^.starty); 
//  (* display coordinaten *)
//end;
//(* interne area coordinaten *)
//
//procedure getmouseco_areawin(mval: psmallint); 
//begin
//  getmouseco_sc(mval); 
//  if curarea^.win<>nil 
//  then
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
//  then
//  begin 
//    mval[0]:= mval[0] - (curarea^.headrct.xmin); 
//    mval[1]:= mval[1] - (curarea^.headrct.ymin); 
//  end;
//end;
//(* ********  WINDOW & QUEUE ********* *)
//var {was static}
//mainq: psmallint = mainqueue; 
//
//function qread(val: psmallint): ushort; 
//begin
//  if {!!!a type cast? =>} {integer(}mainq>{!!!a type cast? =>} {integer(}mainqueue
//  then
//  begin 
//    mainq:= mainq - (2); {*}val^:=mainq[1]; 
//    begin
//      result:= ({!!!a type cast? =>} {pushort(}mainq)[0]; 
//      exit;
//    end;
//  end;
//  begin
//    result:=nil; 
//    exit;
//  end;
//end;

procedure qenter(event: cushort;  val: smallint);
var
size: integer;
end_: psmallint; (* avoid non-events: qtest()! *)
begin
//
//  if event<>nil
//  then
//  begin
//  end:= mainqueue+MAXQUEUE-2;
//  if {!!!a type cast? =>} {integer(}mainq<{!!!a type cast? =>} {integer(}end
//  then
//  begin
//    size:= ({!!!a type cast? =>} {integer(}mainq)-({!!!a type cast? =>} {integer(}mainqueue);
//    if size<>nil
//    then
//    memmove(mainqueue+2,mainqueue,size);
//    ({!!!a type cast? =>} {pushort(}mainqueue)[0]:=event;
//    mainqueue[1]:= val;
//    mainq:= mainq + (2);
//  end;
//end;
end;

//function myqtest: ushort; 
//begin
//  if mainq<>mainqueue
//  then
//  begin
//    result:= ({!!!a type cast? =>} {pushort(}mainq)[-2]; 
//    exit;
//  end;
//  ; 
//  begin
//    result:=nil; 
//    exit;
//  end;
//end;
//
//function qtest: ushort; 
//var
//event: ushort; 
//{$if !defined __BeOS && !defined WIN32}
//begin
//  {__glutDisplay: pDisplay; }{<= !!!5 external variable}(* combinatie: deze werkt als oude qtest(). wel daarna screen_qread aanroepen *)
//  if event:=myqtest()
//  then
//  begin
//    result:= event; 
//    exit;
//  end;
//  begin
//    result:= (XPending(__glutDisplay)); 
//    {$else}
//    exit;
//  end;
//  if event:=myqtest()
//  then
//  begin
//    result:= event; 
//    exit;
//  end;
//  begin
//    result:= glutqtest(); 
//    {$endif}
//    exit;
//  end;
//end;
//
//procedure qreset; 
//begin
//  mainq:= mainqueue; 
//end;
//
//function isqueued(dev: smallint): smallint; 
//begin
//  begin
//    result:=nil; 
//    exit;
//  end;
//end;
//(* *********** AUTOSAVE ************** *)
//
//procedure reset_autosave; 
//begin
//  autosavetime:=nil; 
//end;
//
//procedure start_autosave(val: integer); 
//begin
//  glutTimerFunc(60000,start_autosave,0); 
//  (* opniew aanzetten *)
//  if U.flag and AUTOSAVE
//  then
//  begin 
//    inc(autosavetime); 
//    if autosavetime>=U.savetime
//    then
//    begin 
//      qenter(TIMER0,1); 
//    end;
//  end;
//  else
//  autosavetime:=nil; 
//end;
//
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
//dist: float; 
//mindist: float;
// 
//vec1: array [0..Pred(2)] of float; 
//vec2: array [0..Pred(2)] of float; 
//vec3: array [0..Pred(2)] of float; 
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
//  function PdistVL2Dfl: float; 
//  vec1[0]:= mx; 
//  vec1[1]:= my; 
//  curedge:=nil; (* als de edge element is van curarea: extra voordeel. Dit voor juiste split en join  *)
//  if curarea<>nil 
//  then
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
//    then
//    dist:= dist - (5.0); 
//    if dist<mindist
//    then
//    begin 
//      mindist:= dist; 
//      curedge:= se; 
//    end;
//    se:= se^.next; 
//  end;
//  if curedge=nil
//  then
//  exit;
//  cursonedge:= 1; 
//  if curedge.v1^.vec.x=curedge.v2^.vec.x
//  then
//  glutSetCursor(GLUT_CURSOR_LEFT_RIGHT); 
//  else
//  glutSetCursor(GLUT_CURSOR_UP_DOWN); 
//end;

procedure areawinset(win: smallint);
var
sa: pScrArea = nil;
sseq: pSpaceSeq;
begin
  if win>3  then
  begin
    curarea:= areawinar[win];

    if curarea=nil    then
    begin
      printf('error in areawinar %d ,areawinset'#13#10,[win]);
      exit;
    end;

    case curarea^.spacetype of
//      SPACE_VIEW3D:
//      begin
//        G.vd:= curarea^.spacedata.first; 
//      end;
//      SPACE_IPO:
//      begin
//        if G.sipo<>curarea^.spacedata.first
//        then
//        allqueue(REDRAWBUTSANIM,0); 
//        G.sipo:= curarea^.spacedata.first; 
//        G.v2d:= @G.sipo.v2d; 
//      end;
//      SPACE_BUTS:
//      begin
//        G.buts:= curarea^.spacedata.first; 
//        G.v2d:= @G.buts.v2d; 
//      end;
//      SPACE_SEQ:
//      begin
//        sseq:= curarea^.spacedata.first; 
//        G.v2d:= @sseq.v2d; 
//        {goto next_label;}{<= !!!d case label without "break"}
//      end;
//      SPACE_OOPS:
//      begin
//        G.v2d:= @({!!!a type cast? =>} {pSpaceOops(}curarea^.spacedata.first).v2d; 
//        G.soops:= curarea^.spacedata.first; 
//      end;
//      SPACE_IMASEL:
//      begin
//      end;
//      SPACE_IMAGE:
//      begin
//        G.v2d:= @({!!!a type cast? =>} {pSpaceImage(}curarea^.spacedata.first).v2d; 
//        G.sima:= curarea^.spacedata.first; 
//      end;
//      SPACE_PAINT:
//      begin
//      end;
      SPACE_FILE:
      begin
      end;
//      SPACE_TEXT:
//      begin
//        G.stext:= curarea^.spacedata.first; 
//      end;
//      
    end;
  end;
  if win<>0  then
  mywinset(win);
end;

procedure headerbox(selcol: integer;  width: integer);
begin
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  if selcol<>0   then
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
  if G.curscreen^.winakt<>0  then
  begin
    if curarea^.headwin=G.curscreen^.winakt    then
    selcol:= 1
    else
    if curarea^.win=G.curscreen^.winakt     then
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

//procedure defwindraw; 
//begin
//  if curarea^.win)and(curarea^.windraw
//  then
//  begin 
//    curarea^.windraw(); 
//  end;
//  else
//  begin 
//    glClearColor(0.4375,0.4375,0.4375,0.0); 
//    glClear(GL_COLOR_BUFFER_BIT); 
//  end;
//  curarea^.win_swap:= WIN_BACK_OK; 
//end;
//
//procedure defheadchange; 
//var
//ofs: float; 
//begin
//  
//  if curarea^.headchange<>nil 
//  then
//  begin 
//    curarea^.headchange(); 
//  end;
//  else
//  begin 
//    ofs:= curarea^.headbutofs; 
//    if curarea^.headertype=HEADERDOWN
//    then
//    ortho2(-0.5+ofs,curarea^.headrct.xmax-curarea^.headrct.xmin-0.5+ofs,+0.6,curarea^.headrct.ymax-curarea^.headrct.ymin+0.6); 
//    else
//    ortho2(-0.5+ofs,curarea^.headrct.xmax-curarea^.headrct.xmin-0.5+ofs,-0.5,curarea^.headrct.ymax-curarea^.headrct.ymin-0.5); 
//  end;
//end;
//
//procedure defwinchange; 
//begin
//  if curarea^.winchange<>nil 
//  then
//  begin 
//    curarea^.winchange(); 
//  end;
//  else
//  begin 
//    ortho2(-0.5,curarea^.winrct.xmax-curarea^.winrct.xmin-0.5,-0.5,curarea^.winrct.ymax-curarea^.winrct.ymin-0.5); 
//    glLoadIdentity(); 
//  end;
//end;
//
//procedure defwinmat; 
//begin
//  ortho2(-0.5,curarea^.winrct.xmax-curarea^.winrct.xmin-0.5,-0.5,curarea^.winrct.ymax-curarea^.winrct.ymin-0.5); 
//  glLoadIdentity(); 
//end;
//
//procedure headmenu(sa: pScrArea); 
//var
//fac: float; 
//begin
//  
//  if curarea^.full<>nil 
//  then
//  begin 
//    confirm(,'Full window'); 
//    exit;
//  end;
//  if okee('Switch header')
//  then
//  begin 
//    if sa^.headertype=1
//    then
//    sa^.headertype:= 2; 
//    else
//    if sa^.headertype=2
//    then
//    sa^.headertype:= 1; 
//    testareas(); 
//  end;
//end;
//
//procedure defheadqread(sa: pScrArea); 
//var
//tempsa: pScrArea; 
//fac: float; 
//event: ushort; 
//val: smallint; 
//do_redraw: smallint;
// 
//do_change: smallint;
// 
//str: pchar; 
//begin
//  
//  procedure do_py_head_event; 
//  (* py_draw.c *)
//  
//  
//  
//  
//  do_redraw:=nil; 
//  do_change:=nil; 
//  
//  areawinset(sa^.headwin); 
//  while sa^.hq<>sa^.headqueue
//  do
//  begin 
//    sa^.hq:= sa^.hq - (2); 
//    event:= {!!!a type cast? =>} {ushort(}sa^.hq[0]; 
//    val:= sa^.hq[1]; 
//    if val<>nil 
//    then
//    begin 
//      if sa^.spacetype=SPACE_TEXT)and(sa^.headqread=do_py_head_event
//      then
//      begin 
//        (* exception for a running python script *)
//        sa^.headqread(event,val); 
//      end;
//      else
//      if event=LEFTMOUSE
//      then
//      begin 
//        FrontbufferButs(LongBool(1)); 
//        event:= DoButtons(); 
//        FrontbufferButs(LongBool(0)); 
//        if event<>nil 
//        then
//        begin 
//          if event<1000
//          then
//          do_headerbuttons(event); 
//        end;
//        (* er is waarschijnlijk in frontbuf getekend *)
//        (* sa->win_swap= WIN_FRONT_OK; *)
//        else
//        begin 
//          mywinset(G.curscreen^.mainwin); 
//          if G.qual and LR_CTRLKEY
//          then
//          glutPushWindow(); 
//          else
//          begin 
//            glutPopWindow(); 
//            glutDoWorkList(); 
//            R.winpop:= 1; 
//            (* flag: need pop *)
//          end;
//          areawinset(sa^.headwin); 
//        end;
//      end;
//      else
//      if event=MOUSEY
//      then
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
//      else
//      if event=MIDDLEMOUSE
//      then
//      begin 
//        scrollheader(); 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      else
//      if event=RIGHTMOUSE
//      then
//      begin 
//        headmenu(sa); 
//      end;
//      else
//      if event=REDRAW
//      then
//      begin 
//        do_redraw:= 1; 
//      end;
//      else
//      if event=CHANGED
//      then
//      begin 
//        sa^.head_swap:=nil; 
//        do_change:= 1; 
//        do_redraw:= 1; 
//      end;
//      else
//      if sa^.headqread<>nil 
//      then
//      begin 
//        sa^.headqread(event,val); 
//      end;
//      if winqueue_break<>nil 
//      then
//      exit;
//    end;
//  end;
//  tempsa:= areawinar[sa^.headwin]; (* test: bestaat window nog *)
//  if tempsa=nil
//  then
//  exit;
//  (* dit onderscheid loopt niet lekker... *)
//  if do_change)or(do_redraw
//  then
//  begin 
//    areawinset(sa^.headwin); 
//    defheadchange(); 
//    if sa^.headdraw<>nil 
//    then
//    sa^.headdraw(); 
//  end;
//end;
//
//function winqtest(sa: pScrArea): ushort; 
//begin
//  if sa^.wq<>sa^.winqueue
//  then
//  begin
//    result:= {!!!a type cast? =>} {ushort(}sa^.wq[-2]; 
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
//  then
//  sa^.wq:= sa^.wq - (2); 
//end;
//
//procedure winqclear(sa: pScrArea); 
//begin
//  if sa^.wq:=sa^.winqueue
//  then
//  ; 
//end;
//
//procedure defwinqread(sa: pScrArea); 
//var
//tempsa: pScrArea; 
//event: ushort; 
//val: smallint; 
//do_redraw: smallint;
// 
//do_change: smallint;
// 
//begin
//  
//  
//  
//  do_redraw:=nil; 
//  do_change:=nil; 
//  if sa<>curarea)or(sa^.win<>winget()
//  then
//  areawinset(sa^.win); 
//  while sa^.wq<>sa^.winqueue
//  do
//  begin 
//    sa^.wq:= sa^.wq - (2); 
//    event:= {!!!a type cast? =>} {ushort(}sa^.wq[0]; 
//    val:= sa^.wq[1]; 
//    if event=REDRAW
//    then
//    begin 
//      do_redraw:= 1; 
//    end;
//    else
//    if event=CHANGED
//    then
//    begin 
//      sa^.win_swap:=nil; 
//      do_change:= 1; 
//      do_redraw:= 1; 
//    end;
//    else
//    begin 
//      if sa^.winqread<>nil 
//      then
//      sa^.winqread(event,val); 
//    end;
//    if winqueue_break<>nil 
//    then
//    exit;
//  end;
//  tempsa:= areawinar[sa^.win]; (* test: bestaat window nog *)
//  if tempsa=nil
//  then
//  exit;
//  if do_change)or(do_redraw
//  then
//  areawinset(sa^.win); 
//  if do_change<>nil 
//  then
//  defwinchange(sa^.win); 
//  if do_redraw<>nil 
//  then
//  defwindraw(); 
//end;

procedure addqueue(win: smallint;  event: cushort;  val: smallint);
//var
//sa: pScrArea; 
//size: integer; 
//end: psmallint; (* nieuwe events worden vooraan in het array gezet *)
begin
//  
//  
//  sa:= areawinar[win]; 
//  if sa<>nil 
//  then
//  begin 
//    if win=sa^.headwin
//    then
//    begin 
//    end:= sa^.headqueue+MAXQUEUE-2; 
//    if {!!!a type cast? =>} {integer(}sa^.hq<{!!!a type cast? =>} {integer(}end
//    then
//    begin 
//      size:= ({!!!a type cast? =>} {integer(}sa^.hq)-({!!!a type cast? =>} {integer(}sa^.headqueue); 
//      if size<>nil 
//      then
//      memmove(sa^.headqueue+2,sa^.headqueue,size); 
//      ({!!!a type cast? =>} {pushort(}sa^.headqueue)[0]:=event; 
//      sa^.headqueue[1]:= val; 
//      sa^.hq:= sa^.hq + (2); 
//    end;
//  end;
//  else
//  if win=sa^.win
//  then
//  begin 
//  end:= sa^.winqueue+MAXQUEUE-2; 
//  if {!!!a type cast? =>} {integer(}sa^.wq<{!!!a type cast? =>} {integer(}end
//  then
//  begin 
//    size:= ({!!!a type cast? =>} {integer(}sa^.wq)-({!!!a type cast? =>} {integer(}sa^.winqueue); 
//    if size<>nil 
//    then
//    memmove(sa^.winqueue+2,sa^.winqueue,size); 
//    ({!!!a type cast? =>} {pushort(}sa^.winqueue)[0]:=event; 
//    sa^.winqueue[1]:= val; 
//    sa^.wq:= sa^.wq + (2); 
//  end;
//end;
//end;
end;

//afterqueue: array [0..Pred(3*MAXQUEUE)] of smallint;
//afterq: psmallint = afterqueue; 
//
//procedure addafterqueue(win: smallint;  event: ushort;  val: smallint); 
//var
//poin: integer; 
//begin
//  
//  poin:= {!!!a type cast? =>} {integer(}afterqueue; 
//  poin:= poin + (6*(MAXQUEUE-1)); 
//  if {!!!a type cast? =>} {integer(}afterq<poin
//  then
//  begin 
//    afterq[0]:= win; ({!!!a type cast? =>} {pushort(}afterq)[1]:=event; 
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
//
//function afterqtest: integer; 
//begin
//  if afterqueue<>afterq
//  then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  begin
//    result:=nil; 
//    exit;
//  end;
//end;
//
//procedure remake_qual; 
//begin
//  G.qual:= get_qual(); 
//end;
//var
//ext_load_str: array [0..Pred(256)] of char = (0,0); 
//
//procedure add_readfile_event(filename: pchar); 
//begin
//  qenter(LOAD_FILE,1); 
//  strcpy(ext_load_str,filename); 
//  convertstringcode(ext_load_str); 
//end;
//var
//ext_redraw: smallint =nil; 
//ext_inputchange: smallint =nil; 
//ext_mousemove: smallint =nil; 
//in_ext_qread: smallint =nil; 
//var {was static}
//oldwin: integer;
// 
//
//function screen_qread(val: psmallint): ushort; 
//var
//sa: pScrArea; 
//win: pbWindow; 
//event: ushort; 
//newwin: smallint; 
//rt: smallint; 
//devs: array [0..Pred(2)] of smallint; 
//vals: array [0..Pred(2)] of smallint; 
//ww: integer;
// 
//sc: pbScreen; 
//wx: integer; 
//wy: integer; 
//orx: integer; 
//ory: integer; 
//begin
//  oldwin:=nil; 
//  
//  
//  
//  
//  
//  
//  
//  event:=nil; 
//  if in_ext_qread=nil
//  then
//  begin 
//    if ext_inputchange<>nil 
//    then
//    begin 
//      {*}val^:=ext_inputchange; 
//      ext_inputchange:=nil; 
//      event:= INPUTCHANGE; 
//    end;
//    else
//    if ext_redraw<>nil 
//    then
//    begin 
//      {*}val^:=ext_redraw; 
//      ext_redraw:=nil; 
//      event:= REDRAW; 
//    end;
//    else
//    if ext_mousemove<>nil 
//    then
//    begin 
//      ext_mousemove:=nil; 
//      event:= MOUSEY; 
//    end;
//    else
//    if afterqueue<>afterq)and(qtest()=nil
//    then
//    begin 
//      {*}val^:=nil; 
//      event:= AFTERQUEUE; 
//    end;
//  end;
//  if event=nil
//  then
//  begin 
//    while myqtest()=nil
//    do
//    begin 
//      ww:=nil; 
//      processEventsAndTimeouts(); 
//      glutDoWorkList(); 
//      if myqtest()=nil
//      then
//      usleep(1); 
//    end;
//    event:= qread(val); 
//  end;
//  (* if(event==nil) { *)
//  (*  while(myqtest()==nil) { *)
//  (*   if(qtest()) { *)
//  (*    processEventsAndTimeouts(); *)
//  (*    glutDoWorkList(); *)
//  (*    if(myqtest()==nil) usleep(1); *)
//  (*   } *)
//  (*   if(myqtest()==nil) if(R.flag & R_RENDERING) return 0; *)
//  (*  } *)
//  (*  event= qread(val); *)
//  (* } *)
//  if G.curscreen=nil
//  then
//  begin
//    result:= event; 
//    exit;
//  end;
//  if event=Q_FIRSTTIME
//  then
//  begin 
//    glutDoWorkList(); 
//  end;
//  else
//  if event=RIGHTSHIFTKEY)or(event=LEFTSHIFTKEY
//  then
//  begin 
//    if {*}val^
//    then
//    G.qual:= G.qual or (LR_SHIFTKEY); 
//    else
//    G.qual:= G.qual and ( not LR_SHIFTKEY); 
//  end;
//  else
//  if event=RIGHTALTKEY)or(event=LEFTALTKEY
//  then
//  begin 
//    if {*}val^
//    then
//    G.qual:= G.qual or (LR_ALTKEY); 
//    else
//    G.qual:= G.qual and ( not LR_ALTKEY); 
//  end;
//  else
//  if event=RIGHTCTRLKEY)or(event=LEFTCTRLKEY
//  then
//  begin 
//    if {*}val^
//    then
//    G.qual:= G.qual or (LR_CTRLKEY); 
//    else
//    G.qual:= G.qual and ( not LR_CTRLKEY); 
//  end;
//  else
//  if event=WINFREEZE)or(event=WINTHAW
//  then
//  begin 
//    if (R.flag and R_RENDERING)=nil
//    then
//    begin 
//      if R.win<>nil 
//      then
//      winclose(R.win); 
//      R.win:=nil; 
//      G.qual:=nil; 
//    end;
//  end;
//  {$if 0}
//  else
//  if event=RESHAPE
//  then
//  begin 
//    if {*}val^=mainwin
//    then
//    begin 
//      
//      
//      
//      
//      
//      init_my_mainwin(mainwin); 
//      wx:= glutGet(GLUT_WINDOW_WIDTH); 
//      wy:= glutGet(GLUT_WINDOW_HEIGHT); 
//      orx:= glutGet(GLUT_WINDOW_X); 
//      ory:= displaysizey-wy-glutGet(GLUT_WINDOW_Y); (* Minimizing on windows causes glut to set the
//          * size to 0,  which means things go bonkers
//          *)
//      if wx=nil)or(wy=nil
//      then
//      begin
//        result:=nil; 
//        exit;
//      end;
//      prefstax:= orx; 
//      prefstay:= ory; 
//      prefsizx:= wx; 
//      prefsizy:= wy; 
//      sc:= G.main.screen.first; 
//      while sc
//      do
//      begin 
//        test_scale_screen(sc); 
//        sc:= sc.id.next; 
//      end;
//      testareas(); 
//    end;
//    event:=nil; 
//  end;
//  {$endif}
//  else
//  if event=INPUTCHANGE)or(event=REDRAW)or(event=DRAWEDGES
//  then
//  begin 
//    (* DRAWEDGES: komt vanuit setscreen, qual opnieuw berekenen *)
//    if {*}val^=1
//    then
//    remake_qual(); 
//    if event=INPUTCHANGE)and(in_ext_qread=nil
//    then
//    begin 
//      if {*}val^
//      then
//      begin 
//        mywinset({*}val^); 
//        G.curscreen^.winakt:= {*}val^; 
//        oldwin:= {*}val^; 
//      end;
//      oldwin:=nil; 
//    end;
//    if event=REDRAW
//    then
//    begin 
//      (* kunstmatige mousy voor herberekenen winakt (als b.v. R.win naar achter gepusht *)
//      qenter(MOUSEY,scrmousey); 
//    end;
//  end;
//  else
//  if event=MOUSEX)or(event=MOUSEY
//  then
//  begin 
//    if event=MOUSEY)and(in_ext_qread=nil)and((R.win=nil)or(G.curscreen^.winakt<>R.win)
//    then
//    begin 
//      (* testen waar muis staat *)
//      newwin:=nil; 
//      win:= swindowbase^.first; 
//      while win
//      do
//      begin 
//        if scrmousex>win.xmin)and(scrmousex<win.xmax
//        then
//        begin 
//          (* deze uitzondering betreft onderste en bovenste edge: voor edit cursonedge *)
//          if (scrmousey=nil)and(scrmousey=win.ymin))or((scrmousey=G.curscreen^.endy)and(scrmousey=win.ymax)
//          then
//          begin 
//            if scrmousey>win.ymin)and(scrmousey<win.ymax
//            then
//            begin 
//              newwin:= win.id; 
//              break; {<= !!!b possible in "switch" - then remove this line}
//            end;
//          end;
//          else
//          if scrmousey>=win.ymin)and(scrmousey<=win.ymax
//          then
//          begin 
//            newwin:= win.id; 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//        end;
//        win:= win.next; 
//      end;
//      (* cursor *)
//      if newwin<>oldwin
//      then
//      begin 
//        if newwin=nil
//        then
//        begin 
//          set_cursonedge(scrmousex,scrmousey); 
//        end;
//        else
//        if oldwin=nil
//        then
//        begin 
//          cursonedge:=nil; 
//        end;
//        if newwin<>nil 
//        then
//        begin 
//          sa:= areawinar[newwin]; 
//          if sa^.win=newwin
//          then
//          glutSetCursor(sa^.cursor); 
//          else
//          glutSetCursor(CURSOR_STD); 
//        end;
//      end;
//      else
//      if newwin=nil)and(oldwin=nil
//      then
//      begin 
//        set_cursonedge(scrmousex,scrmousey); 
//      end;
//      (*
//         else if (scrmousex<2 || scrmousey<2 ||
//           abs(scrmousex - G.curscreen->sizex)<2||
//           abs(scrmousey - G.curscreen->sizey)<2) {
//          set_cursonedge(scrmousex, scrmousey);      
//         }
//      *)
//      if newwin<>0
//      then
//      begin 
//        if newwin<>oldwin)or(G.curscreen^.winakt=nil
//        then
//        begin 
//          event:= INPUTCHANGE; {*}val^:=newwin; 
//        end;
//      end;
//      oldwin:= newwin; 
//    end;
//  end;
//  else
//  if event=TIMER0
//  then
//  begin 
//    event:=nil; 
//    if in_ext_qread=nil
//    then
//    begin 
//      write_autosave(); 
//      autosavetime:=nil; 
//    end;
//  end;
//  else
//  if event=LOAD_FILE
//  then
//  begin 
//    event:=nil; 
//    read_file(ext_load_str); 
//    ext_load_str[0]:=nil; ; 
//  end;
//  begin
//    result:= event; 
//    exit;
//  end;
//end;
//
//function special_qread(val: psmallint): ushort; 
//var
//event: ushort; 
//begin(* simul alternatief voor extern_qread *)
//  
//  in_ext_qread:= 1; 
//  event:= screen_qread(val); 
//  in_ext_qread:=nil; (* niet zo net, wel zo handig (zie screen_qread) *)
//  if event=REDRAW
//  then
//  ext_redraw:= {*}val^; 
//  else
//  if event=INPUTCHANGE
//  then
//  ext_inputchange:= {*}val^; 
//  else
//  if event=MOUSEY)or(event=MOUSEX
//  then
//  begin 
//    ext_mousemove:= 1; 
//    event:=nil; 
//  end;
//  begin
//    result:= event; 
//    exit;
//  end;
//end;
//
//function ext_qtest: smallint; 
//begin
//  if ext_inputchange<>nil 
//  then
//  begin
//    result:= INPUTCHANGE; 
//    exit;
//  end;
//  else
//  if ext_redraw<>nil 
//  then
//  begin
//    result:= REDRAW; 
//    exit;
//  end;
//  else
//  if ext_mousemove<>nil 
//  then
//  begin
//    result:= MOUSEY; 
//    exit;
//  end;
//  else
//  begin
//    result:= myqtest(); 
//    exit;
//  end;
//end;
//
//function extern_qread(val: psmallint): ushort; 
//var
//event: ushort; 
//begin(* bewaart de laatste INPUTCHANGE en de laatste REDRAW *)
//  
//  in_ext_qread:= 1; 
//  event:= screen_qread(val); (* niet zo net, wel zo handig (zie screen_qread) *)
//  if event=REDRAW
//  then
//  ext_redraw:= {*}val^; 
//  else
//  if event=INPUTCHANGE
//  then
//  ext_inputchange:= {*}val^; 
//  else
//  if event=MOUSEY)or(event=MOUSEX
//  then
//  ext_mousemove:= 1; 
//  else
//  if event=RIGHTSHIFTKEY)or(event=LEFTSHIFTKEY
//  then
//  begin 
//    if {*}val^
//    then
//    G.qual:= G.qual or (LR_SHIFTKEY); 
//    else
//    G.qual:= G.qual and ( not LR_SHIFTKEY); 
//  end;
//  else
//  if event=RIGHTALTKEY)or(event=LEFTALTKEY
//  then
//  begin 
//    if {*}val^
//    then
//    G.qual:= G.qual or (LR_ALTKEY); 
//    else
//    G.qual:= G.qual and ( not LR_ALTKEY); 
//  end;
//  else
//  if event=RIGHTCTRLKEY)or(event=LEFTCTRLKEY
//  then
//  begin 
//    if {*}val^
//    then
//    G.qual:= G.qual or (LR_CTRLKEY); 
//    else
//    G.qual:= G.qual and ( not LR_CTRLKEY); 
//  end;
//  else
//  if G.qual and LR_CTRLKEY)and(event=F3KEY
//  then
//  begin 
//    screendump(); 
//  end;
//  in_ext_qread:=nil; 
//  begin
//    result:= event; 
//    exit;
//  end;
//end;
//
//procedure markdirty_all; 
//var
//sa: pScrArea; 
//begin
//  
//  sa:= G.curscreen^.areabase.first; 
//  while sa
//  do
//  begin 
//    if sa^.win<>nil 
//    then
//    begin 
//      addqueue(sa^.win,REDRAW,1); 
//      sa^.win_swap:= sa^.win_swap and ( not WIN_FRONT_OK); 
//    end;
//    if sa^.headwin<>nil 
//    then
//    begin 
//      addqueue(sa^.headwin,REDRAW,1); 
//      sa^.head_swap:= sa^.head_swap and ( not WIN_FRONT_OK); 
//    end;
//    sa:= sa^.next; 
//  end;
//end;
//(* BePatch! glutGetFrontbuffer is defined by glDrawBuffer! In fact the name is wrong... *)
//
//procedure header_front_to_back_glut(sa: pScrArea); 
//begin
//  glDrawBuffer(GL_FRONT); 
//  glRasterPos2f({!!!a type cast? =>} {float(}sa^.headrct.xmin-0.1,{!!!a type cast? =>} {float(}sa^.headrct.ymin-0.1); 
//  glutGetFrontBuffer(sa^.headrct.xmin,sa^.headrct.ymin,sa^.winx,HEADERY-1); 
//  glDrawBuffer(GL_BACK); 
//  glutPutFrontBuffer(); 
//  sa^.head_swap:= WIN_EQUAL; 
//end;
//
//procedure window_front_to_back_glut(sa: pScrArea); 
//begin
//  glDrawBuffer(GL_FRONT); 
//  glRasterPos2f({!!!a type cast? =>} {float(}sa^.winrct.xmin-0.1,{!!!a type cast? =>} {float(}sa^.winrct.ymin-0.1); 
//  glutGetFrontBuffer(sa^.winrct.xmin,sa^.winrct.ymin,sa^.winx,sa^.winy); 
//  glDrawBuffer(GL_BACK); 
//  glutPutFrontBuffer(); 
//  sa^.win_swap:= WIN_EQUAL; 
//end;
//
//procedure header_back_to_front_glut(sa: pScrArea); 
//begin
//  glDrawBuffer(GL_BACK); 
//  glRasterPos2f({!!!a type cast? =>} {float(}sa^.headrct.xmin-0.1,{!!!a type cast? =>} {float(}sa^.headrct.ymin-0.1); 
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
//  glRasterPos2f({!!!a type cast? =>} {float(}sa^.headrct.xmin-0.1,{!!!a type cast? =>} {float(}sa^.headrct.ymin-0.1); 
//  glCopyPixels(sa^.headrct.xmin,sa^.headrct.ymin,sa^.winx,HEADERY,GL_COLOR); 
//  sa^.head_swap:= WIN_EQUAL; 
//  glReadBuffer(GL_BACK); 
//end;
//
//procedure window_front_to_back_ogl(sa: pScrArea); 
//begin
//  glReadBuffer(GL_FRONT); 
//  glRasterPos2f({!!!a type cast? =>} {float(}sa^.winrct.xmin-0.1,{!!!a type cast? =>} {float(}sa^.winrct.ymin-0.1); 
//  glCopyPixels(sa^.winrct.xmin,sa^.winrct.ymin,sa^.winx,sa^.winy,GL_COLOR); 
//  sa^.win_swap:= WIN_EQUAL; 
//  glReadBuffer(GL_BACK); 
//end;
//
//procedure header_back_to_front_ogl(sa: pScrArea); 
//begin
//  glDrawBuffer(GL_FRONT); 
//  glRasterPos2f({!!!a type cast? =>} {float(}sa^.headrct.xmin-0.1,{!!!a type cast? =>} {float(}sa^.headrct.ymin-0.1); 
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
//    then
//    doswap:= 1; 
//    if sa^.head_swap=WIN_BACK_OK
//    then
//    headswap:= 1; 
//    sa:= sa^.next; 
//  end;
//  if doswap=nil)and(headswap=nil
//  then
//  exit;
//  oldwin:= winget(); 
//  mywinset(G.curscreen^.mainwin); 
//  if doswap<>nil 
//  then
//  begin 
//    sa:= G.curscreen^.areabase.first; 
//    while sa
//    do
//    begin 
//      if sa^.win_swap=WIN_FRONT_OK
//      then
//      window_front_to_back_ogl(sa); 
//      if sa^.head_swap=WIN_FRONT_OK
//      then
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
//    then
//    begin 
//      header_front_to_back_ogl(sa); 
//    end;
//    else
//    if sa^.head_swap=WIN_BACK_OK
//    then
//    begin 
//      header_back_to_front_ogl(sa); 
//    end;
//    sa:= sa^.next; 
//  end;
//  if oldwin<>nil 
//  then
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
//    then
//    doswap:= 1; 
//    if sa^.head_swap=WIN_BACK_OK
//    then
//    headswap:= 1; 
//    sa:= sa^.next; 
//  end;
//  if doswap=nil)and(headswap=nil
//  then
//  exit;
//  oldwin:= winget(); 
//  mywinset(G.curscreen^.mainwin); 
//  if doswap<>nil 
//  then
//  begin 
//    sa:= G.curscreen^.areabase.first; 
//    while sa
//    do
//    begin 
//      if sa^.win_swap=WIN_FRONT_OK
//      then
//      window_front_to_back_glut(sa); 
//      if sa^.head_swap=WIN_FRONT_OK
//      then
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
//    then
//    begin 
//      header_front_to_back_glut(sa); 
//    end;
//    else
//    if sa^.head_swap=WIN_BACK_OK
//    then
//    begin 
//      header_back_to_front_glut(sa); 
//    end;
//    sa:= sa^.next; 
//  end;
//  if oldwin<>nil 
//  then
//  areawinset(oldwin); 
//end;
//
//procedure screen_swapbuffers_REDRAW; 
//var
//sa: pScrArea; 
//oldwin: integer; 
//doswap: integer;
// 
//swap: integer; 
//begin
//  
//  
//  doswap:=nil; 
//  
//  oldwin:= winget(); 
//  sa:= G.curscreen^.areabase.first; (* dit is een nieuwe implementatie: uitsluitend met redraws en normale swapbuffer *)
//  (* allemaal front ok? *)
//  while sa
//  do
//  begin 
//    if sa^.win)and((sa^.win_swap and WIN_FRONT_OK)=nil
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    if (sa^.head_swap and WIN_FRONT_OK)=nil
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    sa:= sa^.next; 
//  end;
//  if sa=nil
//  then
//  exit;
//  (* printf("front not OK %d %d %d %d\n", sa->win, sa->win_swap, sa->headwin, sa->head_swap); *)
//  sa:= G.curscreen^.areabase.first; 
//  while sa
//  do
//  begin 
//    swap:= sa^.win_swap; 
//    if (swap and WIN_BACK_OK)=nil
//    then
//    begin 
//      if sa^.win)and(sa^.windraw
//      then
//      begin 
//        areawinset(sa^.win); 
//        sa^.windraw(); 
//        doswap:= 1; 
//      end;
//      sa^.win_swap:= swap or WIN_BACK_OK; 
//    end;
//    else
//    if sa^.win_swap=WIN_BACK_OK
//    then
//    doswap:= 1; 
//    swap:= sa^.head_swap; 
//    if (swap and WIN_BACK_OK)=nil
//    then
//    begin 
//      areawinset(sa^.headwin); 
//      if sa^.headdraw<>nil 
//      then
//      sa^.headdraw(); 
//      doswap:= 1; 
//      sa^.head_swap:= swap or WIN_BACK_OK; 
//    end;
//    else
//    if sa^.head_swap=WIN_BACK_OK
//    then
//    doswap:= 1; 
//    sa:= sa^.next; 
//  end;
//  (* de hele backbuffer moet nu OK zijn *)
//  if doswap<>nil 
//  then
//  begin 
//    myswapbuffers(); 
//  end;
//  if oldwin<>nil 
//  then
//  areawinset(oldwin); 
//end;
//
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
//    then
//    begin 
//      myCopySubBuffer(sa^.winrct.xmin,sa^.winrct.ymin-1,sa^.winx,sa^.winy); 
//      sa^.win_swap:= WIN_EQUAL; 
//    end;
//    if sa^.head_swap=WIN_BACK_OK
//    then
//    begin 
//      myCopySubBuffer(sa^.headrct.xmin,sa^.headrct.ymin-1,sa^.winx,HEADERY); 
//      sa^.head_swap:= WIN_EQUAL; 
//    end;
//    sa:= sa^.next; 
//  end;
//end;
//
//procedure screen_swapbuffers; 
//begin
//  {$ifdef __sgi}
//  screen_swapbuffers_OGL(); 
//  {$endif}
//  {$ifdef MESA30}
//  screen_swapbuffers_MESA(); 
//  {$else}
//  screen_swapbuffers_REDRAW(); 
//  {$endif}
//  {$ifdef __WIN32}
//  screen_swapbuffers_REDRAW(); 
//  {$endif}
//  {$ifdef __BeOS}
//  screen_swapbuffers_GLUT(); 
//  {$endif}
//  {$ifdef __SUN}
//  {$ifndef MESA}
//  screen_swapbuffers_OGL(); 
//  {$endif}
//  {$endif}
//end;
//
//function is_allowed_to_change_screen(new: pbScreen): integer; 
//begin(* niet als curscreen is full
//    * niet als obedit && old->scene!=new->scene
//    *)
//  if new=nil
//  then
//  begin
//    result:=nil; 
//    exit;
//  end;
//  if G.curscreen^.full<>nil 
//  then
//  begin
//    result:=nil; 
//    exit;
//  end;
//  if curarea^.full<>nil 
//  then
//  begin
//    result:=nil; 
//    exit;
//  end;
//  if G.obedit<>nil 
//  then
//  begin 
//    if G.curscreen^.scene<>new.scene
//    then
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
//var
//sa: pScrArea; 
//sa1: pScrArea; 
//sa2: pScrArea; 
//sc: pbScreen; 
//fac: float; 
//wx: integer; 
//wy: integer; 
//orx: integer; 
//ory: integer; 
//event: ushort; 
//a: smallint; 
//val: smallint; 
//towin: smallint; 
//dodrawscreen: smallint;
//inqueue: smallint;
//debugval: smallint;
// (* keert terug naar de glut! *)
begin
//  {Xqual: integer; }{<= !!!5 external variable}
//
//  dodrawscreen:=nil; 
//  
//  debugval:=nil; (* keert terug naar de glut! *)
//  while qtest())or(afterqtest()
//  do
//  begin 
//    event:= screen_qread(@val); 
//    towin:= event; 
//    if (G.f and G_DEBUG))and(event)and(event<>MOUSEY
//    then
//    begin 
//      PRINT3(d,d,d,event,val,G.qual); 
//      debugval:= 1; 
//    end;
//    if event=LEFTMOUSE
//    then
//    begin 
//      if val)and(cursonedge
//      then
//      begin 
//        moveareas(); 
//        towin:=nil; 
//      end;
//    end;
//    else
//    if event=MIDDLEMOUSE
//    then
//    begin 
//      if val)and(cursonedge
//      then
//      begin 
//        editsplitpoint(); 
//        towin:=nil; 
//      end;
//    end;
//    else
//    if event=RIGHTMOUSE
//    then
//    begin 
//      if val)and(cursonedge
//      then
//      begin 
//        joinarea(curarea); 
//        towin:=nil; 
//      end;
//    end;
//    else
//    if event=QKEY
//    then
//    begin 
//      if (G.obedit)and(G.obedit.type=OB_FONT)and(curarea^.spacetype=SPACE_VIEW3D))or(curarea^.spacetype=SPACE_TEXT
//      then
//      ; 
//      else
//      begin 
//        if okee('QUIT BLENDER')
//        then
//        exit_usiblender(); 
//        towin:=nil; 
//      end;
//    end;
//    else
//    if event=SPACEKEY
//    then
//    begin 
//      if (G.obedit)and(G.obedit.type=OB_FONT)and(curarea^.spacetype=SPACE_VIEW3D))or(curarea^.spacetype=SPACE_TEXT
//      then
//      ; 
//      else
//      begin 
//        if val<>nil 
//        then
//        toolbox(); 
//        towin:=nil; 
//      end;
//    end;
//    else
//    if event=INPUTCHANGE
//    then
//    begin 
//      (* welke headers moeten redraw? *)
//      if val>3
//      then
//      begin 
//        (* eerste drie nummers voor GL *)
//        if G.curscreen^.winakt<>val
//        then
//        begin 
//          (* de oude en nieuwe area *)
//          sa1:= areawinar[G.curscreen^.winakt]; 
//          sa2:= areawinar[val]; 
//          if sa1=sa2
//          then
//          ; 
//          else
//          begin 
//            if sa1<>nil 
//            then
//            addqueue(sa1.headwin,REDRAW,1); 
//            if sa2<>nil 
//            then
//            addqueue(sa2.headwin,REDRAW,1); 
//          end;
//        end;
//        (* testen of window nog bestaat (oude event bij join b.v.) *)
//        if areawinar[val]<>nil 
//        then
//        begin 
//          (* als winakt==R.win mag alleen een GL-INPUTCHANGE winakt zetten *)
//          if R.win=nil)or(G.curscreen^.winakt<>R.win
//          then
//          G.curscreen^.winakt:= val; 
//          clear_global_filesel_vars(); 
//        end;
//        else
//        begin 
//          G.curscreen^.winakt:=nil; 
//        end;
//      end;
//      towin:=nil; 
//    end;
//    else
//    if event=DRAWEDGES
//    then
//    begin 
//      towin:=nil; 
//      dodrawscreen:= 1; 
//    end;
//    else
//    if event=REDRAW
//    then
//    begin 
//      towin:=nil; 
//      if val=G.curscreen^.mainwin
//      then
//      begin 
//        markdirty_all(); 
//        dodrawscreen:= 1; 
//      end;
//      else
//      if val>3
//      then
//      begin 
//        addqueue(val,REDRAW,val); 
//      end;
//      else
//      if R.win)and(val=R.win
//      then
//      begin 
//        mywinset(R.win); 
//        getorigin(@orx,@ory); 
//        R.winxof:= orx; 
//        R.winyof:= ory; 
//        redraw_render_win(val); 
//      end;
//    end;
//    else
//    if event=RIGHTARROWKEY
//    then
//    begin 
//      if val)and((G.qual and LR_CTRLKEY)
//      then
//      begin 
//        sc:= G.curscreen^.id.next; 
//        if is_allowed_to_change_screen(sc)
//        then
//        setscreen(sc); 
//        towin:=nil; 
//      end;
//    end;
//    else
//    if event=LEFTARROWKEY
//    then
//    begin 
//      if val)and((G.qual and LR_CTRLKEY)
//      then
//      begin 
//        sc:= G.curscreen^.id.prev; 
//        if is_allowed_to_change_screen(sc)
//        then
//        setscreen(sc); 
//        towin:=nil; 
//      end;
//    end;
//    else
//    if event=UPARROWKEY)or(event=DOWNARROWKEY
//    then
//    begin 
//      if val)and((G.qual and LR_CTRLKEY)
//      then
//      begin 
//        area_fullscreen(); 
//        towin:=nil; 
//      end;
//    end;
//    else
//    if event=AFTERQUEUE
//    then
//    begin 
//      append_afterqueue(); 
//    end;
//    if towin<>nil 
//    then
//    begin 
//      towin:= blenderqread(event,val); 
//      if towin)and(G.curscreen^.winakt
//      then
//      addqueue(G.curscreen^.winakt,event,val); 
//    end;
//    event:= ext_qtest(); (* window queues en swapbuffers *)
//    if event=nil)or(event=EXECUTE
//    then
//    begin 
//      (* || event==MOUSEY ?? *)
//      inqueue:= 1; 
//      while inqueue
//      do
//      begin 
//        inqueue:=nil; 
//        winqueue_break:=nil; 
//        sa:= G.curscreen^.areabase.first; 
//        while sa
//        do
//        begin 
//          (* bewust eerst header afhandelen, dan rest. Header is soms init *)
//          if sa^.headwin)and(sa^.headqueue<>sa^.hq
//          then
//          begin 
//            defheadqread(sa); 
//            inqueue:= 1; 
//          end;
//          if winqueue_break<>nil 
//          then
//          begin 
//            (* mogelijk nieuwe G.curscreen *)
//            inqueue:= 1; 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//          if sa^.win)and(sa^.winqueue<>sa^.wq
//          then
//          begin 
//            defwinqread(sa); 
//            inqueue:= 1; 
//          end;
//          if winqueue_break<>nil 
//          then
//          begin 
//            (* mogelijk nieuwe G.curscreen *)
//            inqueue:= 1; 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//          sa:= sa^.next; 
//        end;
//      end;
//      screen_swapbuffers(); 
//      if dodrawscreen<>nil 
//      then
//      begin 
//        drawscreen(); 
//        dodrawscreen:=nil; 
//      end;
//    end;
//    (* restore actieve area *)
//    if G.curscreen^.winakt<>winget()
//    then
//    areawinset(G.curscreen^.winakt); 
//  end;
//  usleep(2); 
//  (* de enige idle *)
//  if debugval=1)and((G.f and G_DEBUG)
//  then
//  begin 
//    debugval:= glGetError(); 
//    while debugval<>GL_NO_ERROR
//    do
//    begin 
//      printf('GL_ERROR: %d %s\n',debugval,gluErrorString(debugval)); 
//      debugval:= glGetError(); 
//    end;
//    printf('end screenmain\n'); 
//  end;
end;

(* *********  AREAS  ************* *)

procedure getdisplaysize;
begin
  displaysizex:= glutGet(GLUT_SCREEN_WIDTH);
  displaysizey:= glutGet(GLUT_SCREEN_HEIGHT);
end;

//procedure setprefsize(stax: integer;  stay: integer;  sizx: integer;  sizy: integer); 
//begin
//  if stax<0
//  then
//  stax:=nil; 
//  if stay<0
//  then
//  stay:=nil; 
//  if sizx<320
//  then
//  sizx:= 320; 
//  if sizy<256
//  then
//  sizy:= 256; 
//  if stax+sizx>displaysizex
//  then
//  sizx:= displaysizex-stax; 
//  if stay+sizy>displaysizey
//  then
//  sizy:= displaysizey-stay; 
//  if sizx<320)or(sizy<256
//  then
//  begin 
//    printf('ERROR: illegal prefsize\n'); 
//    exit;
//  end;
//  prefstax:= stax; 
//  prefstay:= stay; 
//  prefsizx:= sizx; 
//  prefsizy:= sizy; 
//end;
//
//function findcurarea: pScrArea; 
//var
//sa: pScrArea; 
//mval: array [0..Pred(2)] of smallint; 
//begin
//  
//  
//  getmouseco_sc(mval); 
//  sa:= G.curscreen^.areabase.first; 
//  while sa
//  do
//  begin 
//    if sa^.v1^.vec.x<=mval[0])and(sa^.v3^.vec.x>=mval[0]
//    then
//    begin 
//      if sa^.v1^.vec.y<=mval[1])and(sa^.v2^.vec.y>=mval[1]
//      then
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

  if lb<> nil  then
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
    if (se^.v1=v1)and(se^.v2=v2)    then
      exit(se);

    se:= se^.next;
  end;

  exit(nil);
end;

//function findscredge_sc(sc: pbScreen;  v1: pScrVert;  v2: pScrVert): pScrEdge; 
//var
//sv: pScrVert; 
//se: pScrEdge; 
//begin
//  
//  
//  sortscrvert(@v1,@v2); 
//  se:= sc.edgebase.first; 
//  while se
//  do
//  begin 
//    if se^.v1=v1)and(se^.v2=v2
//    then
//    begin
//      result:= se; 
//      exit;
//    end;
//    se:= se^.next; 
//  end;
//  begin
//    result:=nil; 
//    exit;
//  end;
//end;

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
    if verg^.new=nil    then
    begin
      (* !!! *)
      v1:= verg^.next;
      while v1 <>nil      do
      begin
        if v1^.new=nil then
        begin
          (* !?! *)
          if (v1^.vec.x=verg^.vec.x)and(v1^.vec.y=verg^.vec.y)          then
          begin
            (* printf("doublevert\n"); *)
            v1^.new:= verg;
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
    if se^.v1^.new<>nil
    then
    se^.v1:= se^.v1^.new;
    if se^.v2^.new<>nil
    then
    se^.v2:= se^.v2^.new;
    sortscrvert(@(se^.v1),@(se^.v2));
    (* edges zijn veranderd: dus.... *)
    se:= se^.next;
  end;
  sa:= G.curscreen^.areabase.first;
  while sa  <>nil  do
  begin
    if sa^.v1^.new<>nil     then
    sa^.v1:= sa^.v1^.new;
    if sa^.v2^.new<>nil      then
    sa^.v2:= sa^.v2^.new;
    if sa^.v3^.new<>nil     then
    sa^.v3:= sa^.v3^.new;
    if sa^.v4^.new<>nil       then
    sa^.v4:= sa^.v4^.new;

    sa:= sa^.next;
  end;
  verg:= G.curscreen^.vertbase.first; (* verwijderen *)
  while verg <>nil  do
  begin
    v1:= verg^.next;
    if verg^.new<>nil     then
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
//    svn:= sv.next; 
//    if sv.flag=nil
//    then
//    begin 
//      remlink(@G.curscreen^.vertbase,sv); 
//      freeN(sv); 
//    end;
//    else
//    sv.flag:=nil; 
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
      if (verg^.v1=se^.v1)and(verg^.v2=se^.v2)      then
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
    if se=nil      then
    printf('error: area %d edge 1 bestaat niet\n',[a])
    else
    se^.flag:= 1;
    se:= findscredge(sa^.v2,sa^.v3);
    if se=nil     then
    printf('error: area %d edge 2 bestaat niet\n',[a])
    else
    se^.flag:= 1;
    se:= findscredge(sa^.v3,sa^.v4);
    if se=nil     then
    printf('error: area %d edge 3 bestaat niet\n',[a])
    else
    se^.flag:= 1;
    se:= findscredge(sa^.v4,sa^.v1);
    if se=nil     then
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
    if se^.flag=0    then
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
  if sa^.v1^.vec.x>0   then
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
  if sa^.v2^.vec.y<G.curscreen^.sizey-1  then
    sa^.totrct.ymax:= sa^.v2^.vec.y-EDGEWIDTH2-1
  else
    sa^.totrct.ymax:= sa^.v2^.vec.y;

  sa^.winrct:= sa^.totrct;
  if sa^.headertype<>0  then
  begin
    sa^.headrct:= sa^.totrct;
    if sa^.headertype=HEADERDOWN    then
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
  if sa^.winrct.ymin>sa^.winrct.ymax   then
  sa^.winrct.ymin:= sa^.winrct.ymax;

    (* als speedup voor berekeningen *)
  sa^.winx:= sa^.winrct.xmax-sa^.winrct.xmin+1;
  sa^.winy:= sa^.winrct.ymax-sa^.winrct.ymin+1;
end;

procedure openheadwin(sa: pScrArea);
begin
  sa^.headwin:= myswinopen(G.curscreen^.mainwin,sa^.headrct.xmin,sa^.headrct.xmax,sa^.headrct.ymin,sa^.headrct.ymax);

  glMatrixMode(GL_MODELVIEW);

  if sa^.headqueue=nil  then
  begin
    sa^.hq:=mallocN(sizeof(cshort)*MAXQUEUE, 'headqueue');
    sa^.headqueue:=sa^.hq;
  end;

  areawinar[sa^.headwin]:= sa;    (* anders doet addqueue het niet *)
  //addqueue(sa^.headwin,CHANGED,1);
end;

procedure openareawin(sa: pScrArea);
begin
  sa^.win:= myswinopen(G.curscreen^.mainwin,sa^.winrct.xmin,sa^.winrct.xmax,sa^.winrct.ymin,sa^.winrct.ymax);

  if sa^.winqueue=nil  then
  begin
    sa^.wq:=mallocN(sizeof(cshort)*MAXQUEUE, 'winqueue');
  sa^.winqueue:=sa^.wq;
  end;

  areawinar[sa^.win]:= sa;  (* anders doet addqueue het niet *)
  //addqueue(sa^.win,CHANGED,1);
end;

procedure closeheadwin(sa: pScrArea);
begin
  if (G.curscreen<>nil)and(sa^.headwin=G.curscreen^.winakt)  then
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

//procedure del_area(sa: pScrArea); 
//begin
//  closeareawin(sa); 
//  closeheadwin(sa); 
//  freespacelist(@sa^.spacedata); 
//end;

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

  if sa^.winrct.ymin<sa^.winrct.ymax   then
  openareawin(sa);

  if lb<>nil   then
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
    if sa^.headertype<>0    then
    begin
      if sa^.headwin=0      then
      openheadwin(sa)
      else
      begin
        (* is header op goede plek? *)
        if (temph.xmin<>sa^.headrct.xmin)or(temph.xmax<>sa^.headrct.xmax)or(temph.ymin<>sa^.headrct.ymin)or(temph.ymax<>sa^.headrct.ymax) then
        begin
          addqueue(sa^.headwin,CHANGED,1);
          mywinposition(sa^.headwin,sa^.headrct.xmin,sa^.headrct.xmax,sa^.headrct.ymin,sa^.headrct.ymax);
        end;
        if sa^.headbutlen<sa^.winx         then
        begin
          sa^.headbutofs:=0;
          addqueue(sa^.headwin,CHANGED,1);
        end
        else
        if sa^.headbutofs+sa^.winx>sa^.headbutlen         then
        begin
          sa^.headbutofs:= sa^.headbutlen-sa^.winx;
          addqueue(sa^.headwin,CHANGED,1);
        end;
      end;
    end
    else
    begin
      if sa^.headwin<>0      then
      closeheadwin(sa);
    end;
    (* test areawindow *)
    if sa^.win=0    then
    begin
      if sa^.winrct.ymin<sa^.winrct.ymax       then
      openareawin(sa);
    end
    else
    begin
      (* window te klein? *)
      if sa^.winrct.ymin=sa^.winrct.ymax      then
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
    if sa^.headwin<>0    then
    areawinar[sa^.headwin]:= sa;
    if sa^.win<>0        then
    areawinar[sa^.win]:= sa;
    sa:= sa^.next;
  end;
  (* test of winakt in orde is *)
  if areawinar[G.curscreen^.winakt]=nil  then
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
//  then
//  begin 
//    se1:= findscredge(sa^.v1,sa^.v2); 
//    se2:= findscredge(sa^.v2,sa^.v3); 
//    se3:= findscredge(sa^.v3,sa^.v4); 
//    se4:= findscredge(sa^.v4,sa^.v1); 
//  end;
//  if se1<>se)and(se2<>se)and(se3<>se)and(se4<>se
//  then
//  begin 
//    sa:= G.curscreen^.areabase.first; 
//    while sa
//    do
//    begin 
//      (* een beetje optimaliseren? *)
//      if se^.v1=sa^.v1)or(se^.v1=sa^.v2)or(se^.v1=sa^.v3)or(se^.v1=sa^.v4
//      then
//      begin 
//        se1:= findscredge(sa^.v1,sa^.v2); 
//        se2:= findscredge(sa^.v2,sa^.v3); 
//        se3:= findscredge(sa^.v3,sa^.v4); 
//        se4:= findscredge(sa^.v4,sa^.v1); 
//        if se1=se)or(se2=se)or(se3=se)or(se4=se
//        then
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
//cent: array [0..Pred(3)] of float; 
//vec: array [0..Pred(3)] of float; 
//len: float; 
//len1: float; 
//len2: float; 
//len3: float; 
//dist: float;
// 
//mval: array [0..Pred(2)] of smallint; 
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
//    then
//    begin 
//      if sa^.winy>=curarea^.winy
//      then
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
//        then
//        begin 
//          dist:= len; 
//          big:= sa; 
//        end;
//      end;
//    end;
//    sa:= sa^.next; 
//  end;
//  if big<>nil 
//  then
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
//(* ************ SCREENBEHEER ************** *)
//(* testen of screenvertices vergroot/verkleind moeten worden *)
//(* testen of offset nog klopt *)
//
//procedure test_scale_screen(sc: pbScreen); 
//var
//sv: pScrVert;
// 
//se: pScrEdge; 
//sa: pScrArea; 
//san: pScrArea; 
//old: pbScreen; 
//yval: integer; 
//facx: float; 
//facy: float; 
//tempf: float; 
//min: array [0..Pred(2)] of float; 
//max: array [0..Pred(2)] of float; 
//begin
//  sv:=nil; 
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
//  sc.startx:= prefstax; 
//  sc.starty:= prefstay; 
//  sc.endx:= prefstax+prefsizx-1; 
//  sc.endy:= prefstay+prefsizy-1; 
//  sv:= sc.vertbase^.first; 
//  min[0]:= min[1]:=nil.0; 
//  max[0]:= sc.sizex; 
//  max[1]:= sc.sizey; (* calculate size *)
//  while sv
//  do
//  begin 
//    min[0]:= MIN2(min[0],sv.vec.x); 
//    min[1]:= MIN2(min[1],sv.vec.y); 
//    max[0]:= MAX2(max[0],sv.vec.x); 
//    max[1]:= MAX2(max[1],sv.vec.y); 
//    sv:= sv.next; 
//  end;
//  sv:= sc.vertbase^.first; (* always make 0.0 left under *)
//  while sv
//  do
//  begin 
//    sv.vec.x:= sv.vec.x - (min[0]); 
//    sv.vec.y:= sv.vec.y - (min[1]); 
//    sv:= sv.next; 
//  end;
//  sc.sizex:= max[0]-min[0]; 
//  sc.sizey:= max[1]-min[1]; 
//  if sc.sizex<>prefsizx)or(sc.sizey<>prefsizy
//  then
//  begin 
//    facx:= prefsizx; 
//    facx:= facx div ({!!!a type cast? =>} {float(}sc.sizex); 
//    facy:= prefsizy; 
//    facy:= facy div ({!!!a type cast? =>} {float(}sc.sizey); 
//    sv:= sc.vertbase^.first; (* make sure it fits! *)
//    while sv
//    do
//    begin 
//      tempf:= ({!!!a type cast? =>} {float(}sv.vec.x)*facx; 
//      sv.vec.x:= {!!!a type cast? =>} {smallint(}(tempf+0.5); 
//      sv.vec.x:= sv.vec.x + (AREAGRID-1); 
//      sv.vec.x:= sv.vec.x - ((sv.vec.x mod AREAGRID)); 
//      CLAMP(sv.vec.x,0,prefsizx); 
//      tempf:= ({!!!a type cast? =>} {float(}sv.vec.y)*facy; 
//      sv.vec.y:= {!!!a type cast? =>} {smallint(}(tempf+0.5); 
//      sv.vec.y:= sv.vec.y + (AREAGRID-1); 
//      sv.vec.y:= sv.vec.y - ((sv.vec.y mod AREAGRID)); 
//      CLAMP(sv.vec.y,0,prefsizy); 
//      sv:= sv.next; 
//    end;
//    sc.sizex:= prefsizx; 
//    sc.sizey:= prefsizy; 
//  end;
//  sa:= sc.areabase.first; (* test for collapsed areas. This could happen in some blender version... *)
//  while sa
//  do
//  begin 
//    san:= sa^.next; 
//    if sa^.v1=sa^.v2)or(sa^.v3=sa^.v4)or(sa^.v2=sa^.v3
//    then
//    begin 
//      del_area(sa); 
//      freeN(sa); 
//      remlink(@sc.areabase,sa); 
//    end;
//    sa:= san; 
//  end;
//  sa:= sc.areabase.first; (* make each window at least HEADERY high *)
//  while sa
//  do
//  begin 
//    if sa^.v1^.vec.y+HEADERY>sa^.v2^.vec.y
//    then
//    begin 
//      (* lower edge *)
//      se:= findscredge_sc(sc,sa^.v4,sa^.v1); 
//      if se)and(sa^.v1<>sa^.v2
//      then
//      begin 
//        select_connected_scredge(sc,se); 
//        yval:= sa^.v2^.vec.y-HEADERY; 
//        sv:= sc.vertbase^.first; (* all selected vertices get the right offset *)
//        while sv
//        do
//        begin 
//          (* if is a collapsed area *)
//          if sv<>sa^.v2)and(sv<>sa^.v3
//          then
//          begin 
//            if sv.flag<>nil 
//            then
//            sv.vec.y:= yval; 
//          end;
//          sv:= sv.next; 
//        end;
//      end;
//    end;
//    sa:= sa^.next; 
//  end;
//end;

procedure redraw1func; cdecl;
begin
  //qenter(REDRAW,1);
  //if G.f and G_DEBUG
  //then
  //printf('redrawfunc\n');
end;

procedure redraw2func; cdecl;
begin
  //qenter(REDRAW,2);
end;

//procedure visiblefunc(dummy: integer); 
//begin
//  (* geen winfreeze *)
//end;
//
//procedure inputchangefunc(state: integer); 
//var
//win: integer; 
//begin
//  
//  if state<>nil 
//  then
//  {$ifdef WIN32}
//  begin 
//    qenter(INPUTCHANGE,glutGetActiveWindow()); 
//    {$else}
//    qenter(INPUTCHANGE,glutGetWindow()); 
//    {$endif}
//  end;
//  if G.f and G_DEBUG
//  then
//  printf('inputchangefunc %d %d\n',state,glutGetWindow()); 
//end;

procedure reshapefunc(x: integer;  y: integer); cdecl;
//var
//sc: pbScreen; 
//wx: integer; 
//wy: integer; 
//orx: integer; 
//ory: integer; (* alleen voor hoofdwindow *)
begin
//  if G.curscreen=nil
//  then
//  exit;
//  if G.f and G_DEBUG
//  then
//  printf('reshapefunc\n'); 
//  qenter(REDRAW,mainwin); 
//  (*  qenter(RESHAPE, mainwin); *)
//  init_my_mainwin(mainwin); 
//  wx:= glutGet(GLUT_WINDOW_WIDTH); 
//  wy:= glutGet(GLUT_WINDOW_HEIGHT); 
//  orx:= glutGet(GLUT_WINDOW_X); 
//  ory:= displaysizey-wy-glutGet(GLUT_WINDOW_Y); (* Minimizing on windows causes glut to set the
//    * size to 0,  which means things go bonkers
//    *)
//  if wx=nil)or(wy=nil
//  then
//  exit;
//  prefstax:= orx; 
//  prefstay:= ory; 
//  prefsizx:= wx; 
//  prefsizy:= wy; 
//  sc:= G.main.screen.first; 
//  while sc
//  do
//  begin 
//    test_scale_screen(sc); 
//    sc:= sc.id.next; 
//  end;
//  testareas(); 
end;

//var {was static}
//mymdown: integer =nil; 
//
//procedure mousefunc(button: integer;  state: integer;  x: integer;  y: integer); 
//var
//but: integer; 
//begin
//  
//  if button=GLUT_LEFT_BUTTON
//  then
//  begin 
//    but:= LEFTMOUSE; 
//    if U.flag and TWOBUTTONMOUSE
//    then
//    begin 
//      if G.qual and LR_ALTKEY
//      then
//      but:= MIDDLEMOUSE; 
//    end;
//    if state=GLUT_DOWN
//    then
//    qenter(but,1); 
//    else
//    qenter(but,0); 
//    {$ifdef __WIN32}
//    R.winpop:= 1; 
//    {$endif}
//  end;
//  else
//  if button=GLUT_MIDDLE_BUTTON
//  then
//  begin 
//    if state=GLUT_DOWN
//    then
//    mymdown:= 1; 
//    else
//    mymdown:=nil; 
//    if state=GLUT_DOWN
//    then
//    qenter(MIDDLEMOUSE,1); 
//    else
//    qenter(MIDDLEMOUSE,0); 
//  end;
//  else
//  if button=GLUT_RIGHT_BUTTON
//  then
//  begin 
//    if state=GLUT_DOWN
//    then
//    qenter(RIGHTMOUSE,1); 
//    else
//    qenter(RIGHTMOUSE,0); 
//  end;
//end;
//
//procedure mousemovefunc(x: integer;  y: integer); 
//begin
//  scrmousex:= x; 
//  scrmousey:= prefsizy-y; 
//  qenter(MOUSEX,scrmousex); 
//  (* ommappen *)
//  qenter(MOUSEY,scrmousey); 
//  (* (een te veel?) *)
//end;
//var {was static}
//lastval: integer;
// 
//lastkey: integer;
// 
//
//procedure rawkeyfunc(key: integer;  val: integer;  y: integer); 
//begin
//  {$ifdef __BeOS}
//  key:= rawkey_beos(key); 
//  {$endif}
//  {$ifdef WIN32}
//  (* really silly: windows repeats rawkeys! *)
//  if key=212)or(key=213)or(key=216
//  then
//  begin 
//    lastval:=-1; 
//    lastkey:=-1; 
//    if lastkey=key)and(lastval=val
//    then
//    exit;
//    lastval:= val; 
//    lastkey:= key; 
//  end;
//  {$endif}
//  if G.f and G_DEBUG
//  then
//  PRINT3(x,d,d,key,val,y); 
//  qenter(key,val); 
//end;
//
//procedure keyboardfunc(key: uchar;  x: integer;  y: integer); 
//begin
//  if G.f and G_DEBUG
//  then
//  printf('keyboardfunc: %c %d\n',key,key); 
//  qenter(KEYBD,key); 
//end;

procedure do_the_glut_funcs;
begin
//  glutKeyboardFunc(@keyboardfunc); 
//  glutSpecialFunc(@rawkeyfunc); 
//  glutMouseFunc(@mousefunc); 
//  glutMotionFunc(@mousemovefunc); 
//  glutPassiveMotionFunc(@mousemovefunc); 
//  glutVisibilityFunc(@visiblefunc); 
//  glutEntryFunc(@inputchangefunc); 
end;

function mywinopen(mode: integer;  posx: smallint;  posy: smallint;  sizex: smallint;  sizey: smallint): integer;
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
  //if borderless<>nil
  //then
  //glutNoBorder();
  //glutDoWorkList();

  (* WARN! geen standaard fie *)
  {$ifdef WIN32}
  if win=1)and(fullscreen
  then
  glutFullScreen();
  {$endif}

  (* niet netjes: alleen eerste keer! *)
  if win=1    then
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
    //qenter(REDRAW,1);
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
//  freelistN(@sc.vertbase); 
//  freelistN(@sc.edgebase); 
//  sa:= sc.areabase.first; 
//  while sa
//  do
//  begin 
//    del_area(sa); 
//    if sa=curarea
//    then
//    curarea:=nil; 
//    sa:= sa^.next; 
//  end;
//  freelistN(@sc.areabase); 
//  if G.curscreen=sc
//  then
//  begin 
//    G.curscreen:=nil; 
//    winqueue_break:= 1; 
//    (* overal uit queue's gaan *)
//  end;
//end;
//
//procedure setscreen(sc: pbScreen); 
//var
//sc1: pbScreen; 
//sa: pScrArea; 
//win: pbWindow; 
//firstwin: integer;
// 
//mval: array [0..Pred(2)] of smallint; 
//begin
//  
//  
//  
//  firstwin:=nil; 
//  
//  if sc.full<>nil 
//  then
//  begin 
//    (* vind de bijhorende full *)
//    sc1:= G.main.screen.first; 
//    while sc1
//    do
//    begin 
//      sa:= sc1.areabase.first; 
//      if sa^.full=sc
//      then
//      begin 
//        sc:= sc1; 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      sc1:= sc1.id.next; 
//    end;
//    if sc1=nil
//    then
//    printf('setscreen error\n'); 
//  end;
//  (* G.curscreen de-activeren *)
//  if G.curscreen)and(G.curscreen<>sc
//  then
//  begin 
//    sa:= G.curscreen^.areabase.first; 
//    while sa
//    do
//    begin 
//      if sa^.win<>nil 
//      then
//      winclose(sa^.win); 
//      sa^.win:=nil; 
//      if sa^.headwin<>nil 
//      then
//      winclose(sa^.headwin); 
//      sa^.headwin:=nil; 
//      sa^.hq:= sa^.headqueue; 
//      sa^.wq:= sa^.winqueue; 
//      sa:= sa^.next; 
//      (* queue leeg *)
//    end;
//  end;
//  if G.curscreen<>sc
//  then
//  begin 
//    mywinset(sc.mainwin); 
//  end;
//  G.curscreen:= sc; 
//  G.scene:= sc.scene; 
//  countall(); 
//  getmouseco_sc(mval); 
//  (* recalculate winakt *)
//  testareas(); 
//  G.curscreen^.winakt:=nil; 
//  win:= swindowbase^.first; 
//  while win
//  do
//  begin 
//    if firstwin=nil
//    then
//    firstwin:= win.id; 
//    if mval[0]>=win.xmin-1)and(mval[0]<=win.xmax+2
//    then
//    begin 
//      if mval[1]>=win.ymin-1)and(mval[1]<=win.ymax+2
//      then
//      begin 
//        G.curscreen^.winakt:= win.id; 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//    end;
//    win:= win.next; 
//  end;
//  (* als buhv cursor op edge staat *)
//  if G.curscreen^.winakt=nil
//  then
//  G.curscreen^.winakt:= firstwin; 
//  areawinset(G.curscreen^.winakt); 
//  qenter(DRAWEDGES,1); 
//  (* zet curarea *)
//  winqueue_break:= 1; 
//  curedge:=nil; 
//  (* overal uit de queue's gaan *)
//  (* global voor move en join *)
//end;
//(* met curarea *)
//
//procedure area_fullscreen; 
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
//  then
//  begin 
//    sc:= curarea^.full; 
//    sc.full:=nil; 
//    old:= sc.areabase.first; (* de oude screen *)
//    (* vind oude area *)
//    while old
//    do
//    begin 
//      if old.full<>nil 
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      old:= old.next; 
//    end;
//    if old=nil
//    then
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
//    then
//    exit;
//    if curarea^.spacetype=SPACE_INFO
//    then
//    exit;
//    G.curscreen^.full:= 1; 
//    old:= curarea; 
//    oldscreen:= G.curscreen; 
//    sc:= addscreen('temp'); 
//    splitarea({!!!a type cast? =>} {pScrArea(}sc.areabase.first,'h',0.99); 
//    (* deze zet G.curscreen *)
//    new:= sc.areabase.first; 
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
//    s1.new:= s2; 
//    s2:= s2.next; 
//    s1:= s1.next; 
//  end;
//  se:= to.edgebase.first; 
//  while se
//  do
//  begin 
//    se^.v1:= se^.v1^.new; 
//    se^.v2:= se^.v2^.new; 
//    sortscrvert(@(se^.v1),@(se^.v2)); 
//    se:= se^.next; 
//  end;
//  sa:= to.areabase.first; 
//  while sa
//  do
//  begin 
//    sa^.v1:= sa^.v1^.new; 
//    sa^.v2:= sa^.v2^.new; 
//    sa^.v3:= sa^.v3^.new; 
//    sa^.v4:= sa^.v4^.new; 
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
//    s1.new:=nil; 
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
//  then
//  exit;
//  (* nieuw screen maken: *)
//  oldscreen:= G.curscreen; 
//  sc:= addscreen(oldscreen.id.name+2); 
//  copy_screen(sc,oldscreen); 
//  (* deze zet G.curscreen *)
//  G.curscreen:= oldscreen; 
//  setscreen(sc); 
//end;
//(* ************ END SCREENBEHEER ************** *)
//(* ************  JOIN/SPLIT/MOVE ************** *)
//
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
//  then
//  exit;
//  (* zit edge in area? of anders: welke area *)
//  sa:= test_edge_area(sa,curedge); 
//  if sa=nil
//  then
//  exit;
//  (* welke edges kunnen ermee? *)
//  (* vind richtingen met zelfde edge *)
//  sa2:= G.curscreen^.areabase.first; 
//  while sa2
//  do
//  begin 
//    if sa2<>sa
//    then
//    begin 
//      setest:= findscredge(sa2.v1,sa2.v2); 
//      if curedge=setest
//      then
//      right:= sa2; 
//      setest:= findscredge(sa2.v2,sa2.v3); 
//      if curedge=setest
//      then
//      down:= sa2; 
//      setest:= findscredge(sa2.v3,sa2.v4); 
//      if curedge=setest
//      then
//      left:= sa2; 
//      setest:= findscredge(sa2.v4,sa2.v1); 
//      if curedge=setest
//      then
//      up:= sa2; 
//    end;
//    sa2:= sa2.next; 
//  end;
//  sa2:=nil; 
//  setest:=nil; 
//  if left<>nil 
//  then
//  inc(val); 
//  if up<>nil 
//  then
//  inc(val); 
//  if right<>nil 
//  then
//  inc(val); 
//  if down<>nil 
//  then
//  inc(val); 
//  if val=nil
//  then
//  exit;
//  else
//  if val=1
//  then
//  begin 
//    if left<>nil 
//    then
//    sa2:= left; 
//    else
//    if up<>nil 
//    then
//    sa2:= up; 
//    else
//    if right<>nil 
//    then
//    sa2:= right; 
//    else
//    if down<>nil 
//    then
//    sa2:= down; 
//  end;
//  if okee('Join')
//  then
//  begin 
//    if sa2<>nil 
//    then
//    begin 
//      (* nieuwe area is oude sa *)
//      if sa2=left
//      then
//      begin 
//        sa^.v1:= sa2.v1; 
//        sa^.v2:= sa2.v2; 
//        addscredge(@G.curscreen^.edgebase,sa^.v2,sa^.v3); 
//        addscredge(@G.curscreen^.edgebase,sa^.v1,sa^.v4); 
//      end;
//      else
//      if sa2=up
//      then
//      begin 
//        sa^.v2:= sa2.v2; 
//        sa^.v3:= sa2.v3; 
//        addscredge(@G.curscreen^.edgebase,sa^.v1,sa^.v2); 
//        addscredge(@G.curscreen^.edgebase,sa^.v3,sa^.v4); 
//      end;
//      else
//      if sa2=right
//      then
//      begin 
//        sa^.v3:= sa2.v3; 
//        sa^.v4:= sa2.v4; 
//        addscredge(@G.curscreen^.edgebase,sa^.v2,sa^.v3); 
//        addscredge(@G.curscreen^.edgebase,sa^.v1,sa^.v4); 
//      end;
//      else
//      if sa2=down
//      then
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
//      then
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
function testsplitpoint(sa: pScrArea;  dir: char;  fac: cfloat): smallint;
var
val: smallint =0;
x: smallint;
y: smallint;
begin
(* area groot genoeg? *)
  if sa^.v4^.vec.x-sa^.v1^.vec.x<=2*AREAMINX  then
    exit(0);
  if sa^.v2^.vec.y-sa^.v1^.vec.y<=2*AREAMINY   then
    exit(0);

    (* voor zekerheid *)
  if fac<0.0   then
  fac:=0.0;
  if fac>1.0   then
  fac:= 1.0;

  if dir='h'   then
  begin
    y:= round(sa^.v1^.vec.y+fac*(sa^.v2^.vec.y-sa^.v1^.vec.y));

    if (sa^.v2^.vec.y=G.curscreen^.sizey-1)and(sa^.v2^.vec.y-y<HEADERY+EDGEWIDTH2)    then
    y:= sa^.v2^.vec.y-HEADERY-EDGEWIDTH2
    else
    if (sa^.v1^.vec.y=0)and(y-sa^.v1^.vec.y<HEADERY+EDGEWIDTH2)    then
    y:= sa^.v1^.vec.y+HEADERY+EDGEWIDTH2
    else
    if y-sa^.v1^.vec.y<AREAMINY      then
    y:= sa^.v1^.vec.y+AREAMINY
    else
    if sa^.v2^.vec.y-y<AREAMINY     then
    y:= sa^.v2^.vec.y-AREAMINY
    else
    y:= y - ((y mod AREAGRID));

    exit(y);
  end
  else
  begin
    x:= round(sa^.v1^.vec.x+fac*(sa^.v4^.vec.x-sa^.v1^.vec.x));

    if x-sa^.v1^.vec.x<AREAMINX     then
    x:= sa^.v1^.vec.x+AREAMINX
    else
    if sa^.v4^.vec.x-x<AREAMINX     then
    x:= sa^.v4^.vec.x-AREAMINX
    else
    x:= x - ((x mod AREAGRID));

    exit(x);
  end;
end;

procedure splitarea(sa: pScrArea;  dir: char;  fac: cfloat);
var
sc: pbScreen;
new: pScrArea;
sv1: pScrVert;
sv2: pScrVert;
split: smallint;
begin
  if sa=nil   then
  exit;

  split:= testsplitpoint(sa,dir,fac);
  if split=0   then
  exit;

  sc:= G.curscreen;
  areawinset(sa^.win);
  if dir='h'  then
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
//fac: float; 
//event: ushort; 
//ok: smallint;
// 
//val: smallint; 
//split: smallint; 
//mval: array [0..Pred(2)] of smallint; 
//mvalo: array [0..Pred(2)] of smallint;
// 
//col: array [0..Pred(3)] of smallint; 
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
//  then
//  exit;
//  if sa^.win=nil
//  then
//  exit;
//  if sa^.full<>nil 
//  then
//  exit;
//  if curedge=nil
//  then
//  exit;
//  if okee('Split')=nil
//  then
//  exit;
//  if curedge.v1^.vec.x=curedge.v2^.vec.x
//  then
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
//    then
//    begin 
//      mvalo[0]:= mval[0]; 
//      mvalo[1]:= mval[1]; 
//      if dir='h'
//      then
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
//      then
//      begin 
//        ok:= -1; 
//      end;
//      else
//      begin 
//        if dir='h'
//        then
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
//    then
//    begin 
//      if dir='h'
//      then
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
//    then
//    begin 
//      ok:= -1; 
//    end;
//  end;
//  sdrawXORline4(-1,0,0,0,0); 
//  if ok=1
//  then
//  begin 
//    splitarea(sa,dir,fac); 
//    qenter(DRAWEDGES,1); 
//  end;
//  glDrawBuffer(GL_BACK); 
//end;
//
//procedure select_connected_scredge(sc: pbScreen;  edge: pScrEdge); 
//var
//se: pScrEdge; 
//sv: pScrVert; 
//oneselected: integer; 
//dir: char; (* select connected, alleen in de juiste richting *)
//(* 'dir' is de richting van de EDGE *)
//begin
//  
//  
//  
//  if edge.v1^.vec.x=edge.v2^.vec.x
//  then
//  dir:= 'v'; 
//  else
//  dir:= 'h'; 
//  sv:= sc.vertbase^.first; 
//  while sv
//  do
//  begin 
//    sv.flag:=nil; 
//    sv:= sv.next; 
//  end;
//  edge.v1^.flag:= 1; 
//  edge.v2^.flag:= 1; 
//  oneselected:= 1; 
//  while oneselected
//  do
//  begin 
//    se:= sc.edgebase.first; 
//    oneselected:=nil; 
//    while se
//    do
//    begin 
//      if se^.v1^.flag+se^.v2^.flag=1
//      then
//      begin 
//        if dir='h'
//        then
//        if se^.v1^.vec.y=se^.v2^.vec.y
//        then
//        begin 
//          se^.v1^.flag:= se^.v2^.flag:=1; 
//          oneselected:= 1; 
//        end;
//        if dir='v'
//        then
//        if se^.v1^.vec.x=se^.v2^.vec.x
//        then
//        begin 
//          se^.v1^.flag:= se^.v2^.flag:=1; 
//          oneselected:= 1; 
//        end;
//      end;
//      se:= se^.next; 
//    end;
//  end;
//end;
//
//procedure moveareas; 
//var
//se: pScrEdge; 
//v1: pScrVert; 
//sa: pScrArea; 
//addvec: vec2s; 
//vec1: array [0..Pred(2)] of float; 
//vec2: array [0..Pred(2)] of float; 
//vec3: array [0..Pred(2)] of float; 
//dist: integer; 
//mindist: integer;
// 
//event: ushort;
// 
//val: smallint; 
//split: smallint; 
//mval: array [0..Pred(2)] of smallint; 
//mvalo: array [0..Pred(2)] of smallint; 
//tel: smallint; 
//x1: smallint; 
//x2: smallint; 
//y1: smallint; 
//y2: smallint; 
//bigger: smallint; 
//smaller: smallint; 
//col: array [0..Pred(3)] of smallint; 
//dir: char; 
//begin
//  
//  
//  
//  
//  
//  function PdistVL2Dfl: float; 
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
//  then
//  exit;
//  if curedge=nil)or(curedge.border
//  then
//  exit;
//  if curedge.v1^.vec.x=curedge.v2^.vec.x
//  then
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
//    then
//    begin 
//      (* als top of down edge select, test hoogte *)
//      if sa^.v1^.flag)and(sa^.v4^.flag
//      then
//      begin 
//        if sa^.v2^.vec.y=G.curscreen^.sizey-1
//        then
//        y1:= sa^.v2^.vec.y-sa^.v1^.vec.y-HEADERY-EDGEWIDTH2; 
//        else
//        (* bovenste edge *)
//        y1:= sa^.v2^.vec.y-sa^.v1^.vec.y-AREAMINY; 
//        if y1<bigger
//        then
//        bigger:= y1; 
//      end;
//      else
//      if sa^.v2^.flag)and(sa^.v3^.flag
//      then
//      begin 
//        if sa^.v1^.vec.y=nil
//        then
//        y1:= sa^.v2^.vec.y-sa^.v1^.vec.y-HEADERY; 
//        else
//        (* onderste edge *)
//        y1:= sa^.v2^.vec.y-sa^.v1^.vec.y-AREAMINY; 
//        if y1<smaller
//        then
//        smaller:= y1; 
//      end;
//    end;
//    else
//    begin 
//      (* als left of right edge select, test breedte *)
//      if sa^.v1^.flag)and(sa^.v2^.flag
//      then
//      begin 
//        x1:= sa^.v4^.vec.x-sa^.v1^.vec.x-AREAMINX; 
//        if x1<bigger
//        then
//        bigger:= x1; 
//      end;
//      else
//      if sa^.v3^.flag)and(sa^.v4^.flag
//      then
//      begin 
//        x1:= sa^.v4^.vec.x-sa^.v1^.vec.x-AREAMINX; 
//        if x1<smaller
//        then
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
//    then
//    begin 
//      if dir='h'
//      then
//      begin 
//        addvec.y:= addvec.y + (mval[1]-mvalo[1]); 
//        if addvec.y>bigger
//        then
//        addvec.y:= bigger; 
//        if addvec.y<-smaller
//        then
//        addvec.y:= -smaller; 
//      end;
//      else
//      begin 
//        addvec.x:= addvec.x + (mval[0]-mvalo[0]); 
//        if addvec.x>bigger
//        then
//        addvec.x:= bigger; 
//        if addvec.x<-smaller
//        then
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
//        then
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
//    then
//    begin 
//      if event=ESCKEY
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      if event=LEFTMOUSE
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      if event=SPACEKEY
//      then
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
//    then
//    begin 
//      (* zo is AREAGRID netjes *)
//      if addvec.x)and(v1^.vec.x>0)and(v1^.vec.x<G.curscreen^.sizex-1
//      then
//      begin 
//        v1^.vec.x:= v1^.vec.x + (addvec.x); 
//        if addvec.x<>bigger)and(addvec.x<>-smaller
//        then
//        v1^.vec.x:= v1^.vec.x - ((v1^.vec.x mod AREAGRID)); 
//      end;
//      if addvec.y)and(v1^.vec.y>0)and(v1^.vec.y<G.curscreen^.sizey-1
//      then
//      begin 
//        v1^.vec.y:= v1^.vec.y + (addvec.y); 
//        v1^.vec.y:= v1^.vec.y + (AREAGRID-1); 
//        v1^.vec.y:= v1^.vec.y - ((v1^.vec.y mod AREAGRID)); 
//        (* with these lines commented out you can pull the top bar exact to the screen border. *)
//        (* if(addvec.y != bigger && addvec.y != -smaller) { *)
//        (* } *)
//      end;
//    end;
//    v1^.flag:=nil; 
//    v1:= v1^.next; 
//  end;
//  if event<>ESCKEY
//  then
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
//mval: array [0..Pred(2)] of smallint; 
//mvalo: array [0..Pred(2)] of smallint; 
//begin
//  
//  
//  if curarea^.headbutlen<curarea^.winx
//  then
//  begin 
//    curarea^.headbutofs:=nil; 
//  end;
//  else
//  if curarea^.headbutofs+curarea^.winx>curarea^.headbutlen
//  then
//  begin 
//    curarea^.headbutofs:= curarea^.headbutlen-curarea^.winx; 
//  end;
//  getmouseco_sc(mvalo); 
//  while get_mbut()@M_MOUSE
//  do
//  begin 
//    getmouseco_sc(mval); 
//    if mval[0]<>mvalo[0]
//    then
//    begin 
//      curarea^.headbutofs:= curarea^.headbutofs - ((mval[0]-mvalo[0])); 
//      if curarea^.headbutlen-curarea^.winx<curarea^.headbutofs
//      then
//      curarea^.headbutofs:= curarea^.headbutlen-curarea^.winx; 
//      if curarea^.headbutofs<0
//      then
//      curarea^.headbutofs:=nil; 
//      defheadchange(); 
//      if curarea^.headdraw<>nil 
//      then
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
//event: ushort; 
//val: smallint; 
//mval: array [0..Pred(2)] of smallint; 
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
//    then
//    begin 
//      sact:= sa; 
//      inc(tot); 
//    end;
//    sa:= sa^.next; 
//  end;
//  if tot=nil
//  then
//  begin 
//    error('Can't do this! Open correct window');
//    begin
//      result:=nil; 
//      exit;
//    end;
//  end;
//  if tot=1
//  then
//  begin 
//    if curarea<>sact
//    then
//    areawinset(sact.win); 
//    begin
//      result:= 1; 
//      exit;
//    end;
//  end;
//  else
//  if tot>1
//  then
//  begin 
//    tempcursor(GLUT_CURSOR_HELP); 
//    while 1
//    do
//    begin 
//      event:= extern_qread(@val); 
//      if val<>nil 
//      then
//      begin 
//        if event=ESCKEY
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        if event=LEFTMOUSE
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        if event=SPACEKEY
//        then
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
//      then
//      begin 
//        if mval[1]>=win.ymin-1)and(mval[1]<=win.ymax+2
//        then
//        begin 
//          G.curscreen^.winakt:= win.id; 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//      end;
//      win:= win.next; 
//    end;
//    if event=LEFTMOUSE
//    then
//    begin 
//      sa:= areawinar[G.curscreen^.winakt]; 
//      if sa^.spacetype=spacetype
//      then
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
//  then
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
//(* ************  END JOIN/SPLIT/MOVE ************** *)
//(* **************** DRAW SCREENEDGES ***************** *)
//const
//EDGE_EXTEND = 3; 
//
//procedure drawedge(x1: smallint;  y1: smallint;  x2: smallint;  y2: smallint); 
//var
//a: integer; 
//dir: integer; 
//v1: vec2f; 
//v2: vec2f; 
//begin
//  
//  
//  
//  
//  v1^.x:= x1; 
//  v1^.y:= y1; 
//  v2^.x:= x2; 
//  v2^.y:= y2; 
//  if v1^.x=v2^.x
//  then
//  begin 
//    (* vertikaal *)
//    dir:= v1^.y-v2^.y; 
//    if dir>0
//    then
//    dir:= 1; 
//    else
//    dir:= -1; 
//    if dir=-1
//    then
//    begin 
//      if v1^.y=nil
//      then
//      v1^.y:= v1^.y - (EDGE_EXTEND); 
//      if v2^.y=G.curscreen^.sizey
//      then
//      v2^.y:= v2^.y + (EDGE_EXTEND); 
//    end;
//    else
//    begin 
//      if v2^.y=nil
//      then
//      v2^.y:= v2^.y - (EDGE_EXTEND); 
//      if v1^.y=G.curscreen^.sizey
//      then
//      v1^.y:= v1^.y + (EDGE_EXTEND); 
//    end;
//    v1^.y:= v1^.y - (dir*EDGEWIDTH2); 
//    v2^.y:= v2^.y + (dir*EDGEWIDTH2); 
//    v1^.x:= v1^.x + (EDGEWIDTH2); 
//    v2^.x:= v2^.x + (EDGEWIDTH2); 
//    for{while} a:=nil to Pred(EDGEWIDTH) { a++}
//    do
//    begin 
//      cpack(edcol[a]); 
//      LINE2F(@v1,@v2); 
//      dec(v1^.x); 
//      dec(v2^.x); (* en dit voor de afgeronde tuitjes *)
//      if a<EDGEWIDTH2
//      then
//      begin 
//        v1^.y:= v1^.y + (dir); 
//        v2^.y:= v2^.y - (dir); 
//      end;
//      else
//      begin 
//        v1^.y:= v1^.y - (dir); 
//        v2^.y:= v2^.y + (dir); 
//      end;
//    end;
//  end;
//  else
//  begin 
//    (* horizontaal *)
//    dir:= v1^.x-v2^.x; 
//    if dir>0
//    then
//    dir:= 1; 
//    else
//    dir:= -1; 
//    if dir=-1
//    then
//    begin 
//      if v1^.x=nil
//      then
//      v1^.x:= v1^.x - (EDGE_EXTEND); 
//      if v2^.x=G.curscreen^.sizex
//      then
//      v2^.x:= v2^.x + (EDGE_EXTEND); 
//    end;
//    else
//    begin 
//      if v2^.x=nil
//      then
//      v2^.x:= v2^.x - (EDGE_EXTEND); 
//      if v1^.x=G.curscreen^.sizex
//      then
//      v1^.x:= v1^.x + (EDGE_EXTEND); 
//    end;
//    v1^.x:= v1^.x - (dir*EDGEWIDTH2); 
//    v2^.x:= v2^.x + (dir*EDGEWIDTH2); 
//    v1^.y:= v1^.y - (EDGEWIDTH2); 
//    v2^.y:= v2^.y - (EDGEWIDTH2); 
//    for{while} a:=nil to Pred(EDGEWIDTH) { a++}
//    do
//    begin 
//      cpack(edcol[a]); 
//      LINE2F(@v1,@v2); 
//      inc(v1^.y); 
//      inc(v2^.y); (* en dit voor de afgeronde tuitjes *)
//      if a<EDGEWIDTH2
//      then
//      begin 
//        v1^.x:= v1^.x + (dir); 
//        v2^.x:= v2^.x - (dir); 
//      end;
//      else
//      begin 
//        v1^.x:= v1^.x - (dir); 
//        v2^.x:= v2^.x + (dir); 
//      end;
//    end;
//  end;
//end;
//
//procedure drawscredge(se: pScrEdge); 
//var
//sc: pbScreen; 
//v1: pvec2s; 
//v2: pvec2s; 
//begin
//  
//  
//  
//  sc:= G.curscreen; 
//  v1:= @(se^.v1^.vec); 
//  v2:= @(se^.v2^.vec); 
//  se^.border:= 1; (* borders screen niet tekenen *)
//  (* vanwege resolutie verschillen (PC/SGI files) de linit een
//    * beetje afronden?
//    *)
//  if v1^.x<=1)and(v2^.x<=1
//  then
//  exit;
//  if v1^.x>=sc.sizex-2)and(v2^.x>=sc.sizex-2
//  then
//  exit;
//  if v1^.y<=1)and(v2^.y<=1
//  then
//  exit;
//  if v1^.y>=sc.sizey-2)and(v2^.y>=sc.sizey-2
//  then
//  exit;
//  se^.border:=nil; 
//  drawedge(v1^.x,v1^.y,v2^.x,v2^.y); 
//end;
//var {was static}
//firsttime: integer;
// 
//
//procedure drawscreen; 
//var
//se: pScrEdge; 
//begin
//  firsttime:=1; 
//  
//  mywinset(G.curscreen^.mainwin); 
//  ortho2(-0.5,{!!!a type cast? =>} {float(}G.curscreen^.sizex-0.5,-0.5,{!!!a type cast? =>} {float(}G.curscreen^.sizey-0.5); 
//  (* edges in mainwin *)
//  {$if !defined(__BeOS ) && !defined(WIN32) && !defined(PPC) && !defined(MESA31)}
//  glDrawBuffer(GL_FRONT_AND_BACK); 
//  {$endif}
//  se:= G.curscreen^.edgebase.first; 
//  while se
//  do
//  begin 
//    drawscredge(se); 
//    se:= se^.next; 
//  end;
//  glDrawBuffer(GL_BACK); 
//  {$if defined(__BeOS) || defined(WIN32) || defined(PPC) || defined(MESA31)}
//  glDrawBuffer(GL_FRONT); 
//  (* can't do front+back *)
//  se:= G.curscreen^.edgebase.first; 
//  while se
//  do
//  begin 
//    drawscredge(se); 
//    se:= se^.next; 
//  end;
//  glDrawBuffer(GL_BACK); 
//  {$endif}
//  firsttime:=nil; 
//  (* for some reason the drawedges is called very late, but then the graphics is initialized correctly! *)
//end;
//(* ********************************* *)
//
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
//  splitarea({!!!a type cast? =>} {pScrArea(}sc.areabase.first,'h',0.25); 
//  splitarea({!!!a type cast? =>} {pScrArea(}sc.areabase.last,'h',0.49); 
//  splitarea({!!!a type cast? =>} {pScrArea(}sc.areabase.last,'h',0.99); 
//  sa:= sc.areabase.last; 
//  newspace(sa,SPACE_INFO); 
//  sa:= sc.areabase.first; 
//  splitarea(sa^.next,'v',0.5); 
//  (* sa->headertype= NOHEADER; *)
//  (* alle nieuwe areas komen op einde lijst! *)
//  sa:= sc.areabase.first; 
//  splitarea(sa^.next,'v',0.5); 
//  sa:= sc.areabase.first; 
//  sa^.headertype:= HEADERTOP; 
//  newspace(sa,SPACE_BUTS); 
//  sa:= sa^.next; 
//  tel:= 1; (* zet de view3d's en geef ze de juiste aanzicht *)
//  while sa
//  do
//  begin 
//    if sa^.spacetype=nil
//    then
//    begin 
//      newspace(sa,SPACE_VIEW3D); 
//      vd:= sa^.spacedata.first; 
//      vd.persp:=nil; 
//      if tel=1
//      then
//      begin 
//        vd.view:= 1; 
//        vd.viewquat[0]:= fcos(M_PI div 4.0); 
//        vd.viewquat[1]:= -fsin(M_PI div 4.0); 
//      end;
//      else
//      if tel=2
//      then
//      begin 
//        vd.view:= 3; 
//        vd.viewquat[0]:=nil.5; 
//        vd.viewquat[1]:= -0.5; 
//        vd.viewquat[2]:= -0.5; 
//        vd.viewquat[3]:= -0.5; 
//      end;
//      else
//      if tel=3
//      then
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
  //winget := @mywinget;
  //ortho := @myortho;
  ortho2 := @myortho2;
  winclose := @mywinclose;

end.
