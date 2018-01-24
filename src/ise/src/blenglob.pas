unit blenglob;

interface

uses
blender,
screen, space, util;

type
Global = record
vd: pView3D; (* active pointers *)
v2d: pView2D;
sipo: pSpaceIpo;
buts: pSpaceButs;
//sima: pSpaceImage;
//soops: pSpaceOops;
//stext: pSpaceText;
main: pMain;
scene: pScene; (* denk aan file.c *)
curscreen: pbScreen;
//obedit: pObject; (* fonts, allocated global data *)
//font: pointer;
//fonts: pointer;
//fontss: pointer;
mainbase: ListBase; (* strings: lastsaved *)
//ima: array [0..Pred(160)] of char;
//sce: array [0..Pred(160)] of char;
//lib: array [0..Pred(160)] of char;
//psx: array [0..Pred(160)] of char; (* totalen *)
//totobj: smallint;
//totlamp: smallint;
//totobjsel: smallint;
//totcurve: smallint;
//totmesh: smallint;
//totmat: smallint;
//totvert: integer;
//totface: integer;
//totvertsel: integer;
//totfacesel: integer;
//time: integer;
//cputime: integer;
//machine: smallint;
//afbreek: smallint;
//f: smallint;
//moving: smallint;
//colact: smallint;
//zbuf: smallint;
//qual: smallint;
//background: smallint;
//imagewin: smallint;
//animspeed: smallint;
version: smallint;
//versionfile: smallint;
//simulf: smallint;
//fields: smallint;
order: smallint;
//rt: smallint;
//edve: ListBase;
//eded: ListBase;
//edvl: ListBase;
//textcurs: array [0..Pred(4),0..Pred(2)] of float; (* realtime *)
//cursector: pSector;
//sectorbuf: array [0..Pred(SE_MAXBUF)] of pSector;
//lifebuf: array [0..Pred(LF_MAXBUF)] of pObject;
//actvarar: psmallint;
//dfra: integer;
//dfrao: integer;
//dfras: smallint;
//totsect: smallint;
//maxsect: smallint;
//totlife: smallint; (* variabelen van frank *)
//renderd: integer;
//real_sfra: integer;
//real_efra: integer;
//save_over: integer; (* special versions *)
//special1: smallint;
//special2: smallint;
end;
pGlobal = ^Global;

var
  G: Global;

implementation

end.

