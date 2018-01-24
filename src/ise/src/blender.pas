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
(*  blender.c   jan 94     MIXED MODEL
 * 
 * algemene hulp funkties en data
 * 
 *)
unit blender;

interface

uses
ctypes,cfuncs,
util,
library_, arithb;

//{$ifndef BLENDER_H}
//{$define BLENDER_H}
//{$include <fcntl.h>}
//{$include <math.h>}
//{$include <stdio.h>}
//{$include <stdlib.h>}
//{$include <sys/types.h>}
//{$ifndef WIN32}
//{$include <sys/times.h>}
//{$endif}
//{$if defined(__sgi)}
//{$include <sys/schedctl.h>}
//{$endif}
//{$include <string.h>}
//{$ifndef WIN32}
//{$include <strings.h>}
//{$include <unistd.h>}
//{$endif}
//{$if !defined(__BeOS) && !defined(WIN32)}
//{$include <dlfcn.h>}
//{$endif}
//{$ifdef WIN32}
//{$include <limits.h>}
//{$include "winstuff.h"}
//{$endif}
//{$ifdef __BeOS}
//{$include "beosstuff.h"}
//{$endif}
//{$include "util.h"}
//{$include "iff.h"}
//{$include "blendef.h"}
//{$include "floatpatch.h"}
//{$include "plugin.h"}

(* *********** vec ************* *)
type
pvec2s = ^vec2s;
vec2s = record
x: smallint;
y: smallint;
end;

//vec2i = record
//x: integer;
//y: integer;
//end;
//vec2i = vec2i;
//vec2f = record
//x: float;
//y: float;
//end;
//vec2f = vec2f;
//vec2d = record
//x: double;
//y: double;
//end;
//vec2d = vec2d;
//vec3i = record
//x: integer;
//y: integer;
//z: integer;
//end;
//vec3i = vec3i;
//vec3f = record
//x: float;
//y: float;
//z: float;
//end;
//vec3f = vec3f;
//vec3d = record
//x: double;
//y: double;
//z: double;
//end;
//vec3d = vec3d;
//vec4i = record
//x: integer;
//y: integer;
//z: integer;
//w: integer;
//end;
//vec4i = vec4i;
//vec4f = record
//x: float;
//y: float;
//z: float;
//w: float;
//end;
//vec4f = vec4f;
//vec4d = record
//x: double;
//y: double;
//z: double;
//w: double;
//end;
//vec4d = vec4d;

prcti = ^rcti;
rcti = record
xmin: integer;
xmax: integer;
ymin: integer;
ymax: integer;
end;

prctf = ^rctf;
rctf = record
xmin: cfloat;
xmax: cfloat;
ymin: cfloat;
ymax: cfloat;
end;

(* ******************************************* *)
(* * * * * * * *  L I B R A R Y  * * * * * * * *)
(* ******************************************* *)
(* **************** MAIN ********************* *)
//BoundBox = record
//vec: array [0..Pred(8),0..Pred(3)] of float; 
//end;
//BoundBox = BoundBox; 
//OcInfo = record
//dvec: array [0..Pred(3)] of float; 
//size: array [0..Pred(3)] of float; 
//end;
//OcInfo = OcInfo;

type
pLibrary = ^Library__;

(* let op: Sequence heeft identiek begin. *)
pID = ^ID;
ID = record
next: pinteger;
prev: pinteger;
new: pID;
lib: pLibrary;
name: array [0..23] of char;
us: smallint;
flag: smallint;
pad: integer;
end;

Library__ = record
//id: ID; 
//idblock: pID; 
//filedata: pchar; 
//name: array [0..Pred(160)] of char; 
//tot: integer; 
//pad: integer; (* tot, idblock en filedata zijn voor lezen en schrijven *)
end;

pMain = ^Main;
Main = record
//next: pMain; 
//prev: pMain; 
//name: array [0..Pred(160)] of char; 
//versionfile: smallint; 
//rt: smallint; 
//curlib: pLibrary; 
//scene: ListBase; 
//library: ListBase; 
//object: ListBase; 
//mesh: ListBase; 
//curve: ListBase; 
//mball: ListBase; 
//mat: ListBase; 
//tex: ListBase; 
//image: ListBase; 
//ika: ListBase; 
//wave: ListBase; 
//latt: ListBase; 
//sector: ListBase; 
//life: ListBase; 
//lamp: ListBase; 
//camera: ListBase; 
//ipo: ListBase; 
//key: ListBase; 
//world: ListBase; 
screen: ListBase;
//vfont: ListBase; 
//text: ListBase; 
end;

(* **************** IPO ********************* *)
pIpo = ^Ipo;
Ipo = record
id: ID;
curve: ListBase;
cur: rctf;
blocktype: smallint;
showkey: smallint;
pad: integer;
end;

//Path = record
//len: integer; 
//data: pfloat; 
//totdist: float; 
//end;
//Path = Path; 
//(* **************** KEY ********************* *)
//KeyBlock = record
//next: pKeyBlock; 
//prev: pKeyBlock; 
//pos: float; 
//flag: smallint; 
//totelem: smallint; 
//type: smallint; 
//rt: smallint; 
//pad: integer; 
//data: pinteger; 
//end;
//KeyBlock = KeyBlock; 
//Key = record
//id: ID; 
//refkey: pKeyBlock; 
//elemstr: array [0..Pred(32)] of char; 
//elemsize: integer; 
//curval: float; 
//block: ListBase; 
//ipo: pIpo; 
//from: pID; 
//type: smallint; 
//totkey: smallint; 
//slurph: smallint; 
//actkey: smallint; 
//end;
//Key = Key; 
//(* **************** TEXT *********************** *)
//ScriptLink = record
//{!!!3 unknown typedef}
//end;
//ScriptLink = ScriptLink; 
//TextLine = record
//next: pTextLine; 
//prev: pTextLine; 
//line: pchar; 
//len: integer; 
//blen: integer; 
//end;
//TextLine = TextLine; 
//Text = record
//id: ID; 
//name: pchar; 
//flags: integer; 
//nlines: integer; 
//lines: ListBase; 
//curl: pTextLine; 
//sell: pTextLine; 
//curc: integer; 
//selc: integer; 
//undo_buf: pchar; 
//undo_pos: integer; 
//undo_len: integer; 
//compiled: pinteger; 
//end;
//Text = Text; 
//(* **************** CAMERA ********************* *)
//Camera = record
//id: ID; 
//type: smallint; 
//flag: smallint; 
//drawzoom: smallint; 
//hold: smallint; 
//clipsta: float; 
//clipend: float; 
//netsta: float; 
//netend: float; (* network camera *)
//lens: float; 
//drawsize: float; 
//hololen: float; 
//hololen1: float; 
//ipo: pIpo; 
//scriptlink: ScriptLink; 
//end;
//Camera = Camera; 
//(* **************** IMAGE ********************* *)
//Image = record
//id: ID; 
//name: array [0..Pred(160)] of char; 
//anim: panim; 
//ibuf: pImBuf; 
//mipmap: array [0..Pred(10)] of pImBuf; 
//ok: smallint; 
//flag: smallint; 
//lastframe: smallint; 
//lastquality: smallint; (* texture pagina *)
//tpageflag: smallint; 
//totbind: smallint; 
//xrep: smallint; 
//yrep: smallint; 
//twsta: smallint; 
//twend: smallint; 
//bindcode: UINT; 
//repbind: pUINT; (* om subregio's te kunnen repeaten *)
//end;
//Image = Image; 
//(* **************** TEX ********************* *)
//MTex = record
//texco: smallint; 
//mapto: smallint; 
//maptoneg: smallint; 
//blendtype: smallint; 
//object: pObject; 
//tex: pTex; 
//projx: char; 
//projy: char; 
//projz: char; 
//mapping: char; 
//ofs: array [0..Pred(3)] of float; 
//size: array [0..Pred(3)] of float; 
//texflag: smallint; 
//colormodel: smallint; 
//r: float; 
//g: float; 
//b: float; 
//k: float; 
//def_var: float; 
//colfac: float; 
//norfac: float; 
//varfac: float; 
//end;
//MTex = MTex; 
//PluginTex = record
//name: array [0..Pred(160)] of char; 
//handle: pinteger; 
//pname: pchar; 
//stnames: pchar; 
//stypes: integer; 
//vars: integer; 
//varstr: pinteger; 
//result: pfloat; 
//cfra: pfloat; 
//data: array [0..Pred(32)] of float; 
//doit: function: integer; 
//callback: procedure; 
//version: integer; 
//pad: integer; 
//end;
//PluginTex = PluginTex; 
//CBData = record
//r: float; 
//g: float; 
//b: float; 
//a: float; 
//pos: float; 
//cur: integer; 
//end;
//CBData = CBData; 
//ColorBand = record
//flag: smallint; 
//tot: smallint; 
//cur: smallint; 
//ipotype: smallint; 
//data: array [0..Pred(16)] of CBData; 
//end;
//ColorBand = ColorBand; 
//EnvMap = record
//object: pObject; 
//ima: pImage; (* type ENV_LOAD *)
//cube: array [0..Pred(6)] of pImage; (* these images are dynamic, not part of the main struct *)
//imat: array [0..Pred(4),0..Pred(4)] of float; 
//type: smallint; 
//stype: smallint; 
//clipsta: float; 
//clipend: float; 
//notlay: UINT; 
//cuberes: integer; 
//ok: smallint; 
//lastframe: smallint; 
//end;
//EnvMap = EnvMap; 
//Tex = record
//id: ID; 
//noisesize: float; 
//turbul: float; 
//bright: float; 
//contrast: float; 
//rfac: float; 
//gfac: float; 
//bfac: float; 
//filtersize: float; 
//noisedepth: smallint; 
//noisetype: smallint; 
//imaflag: smallint; 
//flag: smallint; 
//type: smallint; 
//stype: smallint; 
//cropxmin: float; 
//cropymin: float; 
//cropxmax: float; 
//cropymax: float; 
//xrepeat: smallint; 
//yrepeat: smallint; 
//extend: smallint; 
//len: smallint; 
//frames: smallint; 
//offset: smallint; 
//sfra: smallint; 
//fie_ima: smallint; 
//norfac: float; 
//nor: pfloat; 
//ipo: pIpo; 
//ima: pImage; 
//plugin: pPluginTex; 
//coba: pColorBand; 
//env: pEnvMap; 
//fradur: array [0..Pred(4),0..Pred(2)] of smallint; 
//end;
//Tex = Tex; 
//(* **************** LAMP ********************* *)
//Lamp = record
//id: ID; 
//type: smallint; 
//mode: smallint; 
//colormodel: smallint; 
//totex: smallint; 
//r: float; 
//g: float; 
//b: float; 
//k: float; 
//energy: float; 
//dist: float; 
//spotsize: float; 
//spotblend: float; 
//haint: float; 
//att1: float; 
//att2: float; 
//bufsize: smallint; 
//samp: smallint; 
//clipsta: float; 
//clipend: float; 
//shadspotsize: float; 
//bias: float; 
//soft: float; (* texact is voor buttons *)
//texact: smallint; 
//shadhalostep: smallint; 
//mtex: array [0..Pred(8)] of pMTex; 
//ipo: pIpo; 
//scriptlink: ScriptLink; 
//end;
//Lamp = Lamp; 
//(* **************** WAVE ********************* *)
//Wave = record
//id: ID; 
//ipo: pIpo; 
//end;
//Wave = Wave; 
//(* **************** IKA ********************* *)
//(* **************** MATERIAL ********************* *)
//(* LET OP: type veranderen? ook in ipo.h doen *)
//Material = record
//id: ID; 
//colormodel: smallint; 
//lay: smallint; (* lay: voor dynamics *)
//r: float; 
//g: float; 
//b: float; 
//specr: float; 
//specg: float; 
//specb: float; 
//mirr: float; 
//mirg: float; 
//mirb: float; 
//ambr: float; 
//ambb: float; 
//ambg: float; 
//amb: float; 
//emit: float; 
//ang: float; 
//spectra: float; 
//alpha: float; 
//ref: float; 
//spec: float; 
//zoffs: float; 
//add: float; 
//har: smallint; 
//seed1: char; 
//seed2: char; 
//mode: integer; 
//flarec: smallint; 
//starc: smallint; 
//linec: smallint; 
//ringc: smallint; 
//hasize: float; 
//flaresize: float; 
//subsize: float; 
//flareboost: float; (* onderstaand is voor buttons en render*)
//rgbsel: char; 
//texact: char; 
//pr_type: char; 
//septex: char; 
//texco: smallint; 
//mapto: smallint; 
//mtex: array [0..Pred(8)] of pMTex; 
//ipo: pIpo; 
//ren: pMaterial; 
//scriptlink: ScriptLink; 
//end;
//Material = Material; 
//(* **************** VFont ********************* *)
//VFontData = record
//nurbsbase: array [0..Pred(256)] of ListBase; 
//resol: array [0..Pred(256)] of float; 
//width: array [0..Pred(256)] of float; 
//points: array [0..Pred(256)] of pfloat; 
//kcount: smallint; 
//rt: smallint; 
//kern: psmallint; 
//end;
//VFontData = VFontData; 
//VFont = record
//id: ID; 
//name: array [0..Pred(255)] of char; 
//namenull: char; 
//scale: float; 
//flag: smallint; 
//rt: smallint; 
//data: pVFontData; 
//end;
//VFont = VFont; 
//(* **************** METABALL ********************* *)
//MetaElem = record
//next: pMetaElem; 
//prev: pMetaElem; 
//type: smallint; 
//lay: smallint; 
//flag: smallint; 
//selcol: smallint; 
//x: float; 
//y: float; 
//z: float; 
//expx: float; 
//expy: float; 
//expz: float; 
//rad: float; 
//rad2: float; 
//s: float; 
//len: float; 
//maxrad2: float; 
//pad: integer; 
//mat: pfloat; 
//imat: pfloat; 
//end;
//MetaElem = MetaElem; 
//MetaBall = record
//id: ID; 
//bb: pBoundBox; 
//elems: ListBase; 
//disp: ListBase; 
//ipo: pIpo; {!!!3 unknown typedef}
//end;
//MetaBall = MetaBall; 
//(* **************** CURVE ********************* *)
//BevList = record
//next: pBevList; 
//prev: pBevList; 
//nr: smallint; 
//flag: smallint; 
//poly: smallint; 
//gat: smallint; 
//end;
//BevList = BevList; 
//BevPoint = record
//x: float; 
//y: float; 
//z: float; 
//alfa: float; 
//sina: float; 
//cosa: float; 
//mat: array [0..Pred(3),0..Pred(3)] of float; 
//f1: smallint; 
//f2: smallint; 
//end;
//BevPoint = BevPoint;

ppBezTriple = ^pBezTriple;
pBezTriple = ^BezTriple;
BezTriple = record
vec: array [0..Pred(3),0..Pred(3)] of cfloat;
alfa: cfloat;
s: array [0..Pred(3),0..Pred(2)] of smallint;
h1: smallint;
h2: smallint;
f1: char;
f2: char;
f3: char;
hide: char;
end;

//BPoint = record
//vec: array [0..Pred(4)] of float; 
//alfa: float; 
//s: array [0..Pred(2)] of smallint; 
//f1: smallint; 
//hide: smallint; 
//end;
//BPoint = BPoint; 
//Nurb = record
//next: pNurb; 
//prev: pNurb; 
//type: smallint; 
//mat_nr: smallint; 
//hide: smallint; 
//flag: smallint; 
//pntsu: smallint; 
//pntsv: smallint; 
//resolu: smallint; 
//resolv: smallint; 
//orderu: smallint; 
//orderv: smallint; 
//flagu: smallint; 
//flagv: smallint; 
//knotsu: pfloat; 
//knotsv: pfloat; 
//bp: pBPoint; 
//bezt: pBezTriple; 
//end;
//Nurb = Nurb; 
//Curve = record
//id: ID; 
//bb: pBoundBox; 
//nurb: ListBase; 
//disp: ListBase; 
//bevobj: pObject; 
//textoncurve: pObject; 
//ipo: pIpo; 
//path: pPath; 
//key: pKey; {!!!3 unknown typedef}
//end;
//Curve = Curve; 

(* **************** IPOCURVE (Ipo staat bovenin) ********************* *)
pIpoCurve = ^IpoCurve;
IpoCurve = record
next: pIpoCurve;
prev: pIpoCurve;
//bp: pBPoint;
//bezt: pBezTriple;
maxrct: rctf;
totrct: rctf;
blocktype: smallint;
adrcode: smallint;
vartype: smallint;
totvert: smallint;
ipo: smallint;
extrap: smallint;
flag: smallint;
rt: smallint;
ymin: cfloat;
ymax: cfloat;
bitmask: cUINT;
curval: cfloat;
end;

//(* **************** MESH ********************* *)
//MFace = record
//v1: ushort; 
//v2: ushort; 
//v3: ushort; 
//v4: ushort; 
//puno: char; 
//mat_nr: char; 
//edcode: char; 
//flag: char; 
//end;
//MFace = MFace; 
//MFaceInt = record
//v1: integer; 
//v2: integer; 
//v3: integer; 
//v4: integer; 
//puno: char; 
//mat_nr: char; 
//edcode: char; 
//flag: char; 
//end;
//MFaceInt = MFaceInt; 
//MVert = record
//co: array [0..Pred(3)] of float; 
//no: array [0..Pred(3)] of smallint; 
//flag: char; 
//mat_nr: char; 
//end;
//MVert = MVert; 
//MCol = record
//a: char; 
//r: char; 
//g: char; 
//b: char; 
//end;
//MCol = MCol; 
//MSticky = record
//co: array [0..Pred(2)] of float; 
//end;
//MSticky = MSticky; 
//Mesh = record
//id: ID; 
//bb: pBoundBox; 
//effect: ListBase; 
//disp: ListBase; 
//ipo: pIpo; 
//key: pKey; {!!!3 unknown typedef}
//end;
//Mesh = Mesh; 
//(* **************** LATTICE ********************* *)
//Lattice = record
//id: ID; 
//pntsu: smallint; 
//pntsv: smallint; 
//pntsw: smallint; 
//flag: smallint; 
//typeu: char; 
//typev: char; 
//typew: char; 
//type: char; 
//pad: integer; 
//def: pBPoint; 
//ipo: pIpo; 
//key: pKey; 
//end;
//Lattice = Lattice; 
//(* **************** OBJECT ********************* *)
//Object = record
//id: ID; 
//type: smallint; 
//partype: smallint; 
//par1: integer; 
//par2: integer; 
//par3: integer; (* kunnen vertexnrs zijn *)
//parent: pObject; 
//track: pObject; 
//ipo: pIpo; 
//path: pPath; 
//bb: pBoundBox; 
//data: pinteger; 
//effect: ListBase; 
//network: ListBase; 
//disp: ListBase; {!!!3 unknown typedef}
//end;
//Object = Object; 
//(* **************** WORLD ********************* *)
//World = record
//id: ID; 
//colormodel: smallint; 
//totex: smallint; 
//texact: smallint; 
//mistype: smallint; 
//horr: float; 
//horg: float; 
//horb: float; 
//hork: float; 
//zenr: float; 
//zeng: float; 
//zenb: float; 
//zenk: float; 
//ambr: float; 
//ambg: float; 
//ambb: float; 
//ambk: float; 
//fastcol: UINT; 
//exposure: float; 
//skytype: smallint; 
//mode: smallint; 
//misi: float; 
//miststa: float; 
//mistdist: float; 
//misthi: float; 
//starr: float; 
//starg: float; 
//starb: float; 
//stark: float; 
//starsize: float; 
//starmindist: float; 
//stardist: float; 
//starcolnoise: float; 
//dofsta: smallint; 
//dofend: smallint; 
//dofmin: smallint; 
//dofmax: smallint; 
//pad: integer; 
//ipo: pIpo; 
//mtex: array [0..Pred(8)] of pMTex; 
//scriptlink: ScriptLink; 
//end;
//World = World; 
//(* **************** SCENE ********************* *)
//Radio = record
//hemires: smallint; 
//maxiter: smallint; 
//drawtype: smallint; 
//flag: smallint; (* bit 0 en 1: limits laten zien *)
//subshootp: smallint; 
//subshoote: smallint; 
//nodelim: smallint; 
//maxsublamp: smallint; 
//pama: smallint; 
//pami: smallint; 
//elma: smallint; 
//elmi: smallint; (* patch en elem limits *)
//maxnode: integer; 
//convergence: float; 
//radfac: float; 
//gamma: float; (* voor afbeelden *)
//end;
//Radio = Radio; 
//RenderData = record
//cfra: smallint; (* hoe gaat tijd gedefinieerd worden? *)
//sfra: smallint; 
//efra: smallint; (* plaatjes *)
//images: smallint; 
//framapto: smallint; 
//flag: smallint; 
//ctime: float; (* hiermee rekenen? *)
//framelen: float; 
//blurfac: float; 
//size: smallint; 
//maximsize: smallint; (* size in %, max in Kb *)
//xsch: smallint; 
//ysch: smallint; 
//xasp: smallint; 
//yasp: smallint; (* uit buttons *)
//xparts: smallint; 
//yparts: smallint; 
//safety: rctf; 
//border: rctf; 
//winpos: smallint; 
//planes: smallint; 
//imtype: smallint; 
//bufflag: smallint; 
//quality: smallint; 
//scemode: smallint; 
//mode: smallint; 
//alphamode: smallint; 
//dogamma: smallint; 
//osa: smallint; 
//frs_sec: smallint; 
//pad: smallint; 
//backbuf: array [0..Pred(160)] of char; 
//pic: array [0..Pred(160)] of char; 
//ftype: array [0..Pred(160)] of char; 
//movie: array [0..Pred(160)] of char; 
//end;
//RenderData = RenderData; 
//Base = record
//next: pBase; 
//prev: pBase; 
//lay: UINT; 
//selcol: UINT; 
//flag: integer; 
//sx: smallint; 
//sy: smallint; 
//object: pObject; 
//end;
//Base = Base;

pScene = ^Scene;
Scene = record
//id: ID; 
//camera: pObject; 
//world: pWorld; 
//set: pScene; 
//ima: pImage; 
//base: ListBase; 
//basact: pBase; 
//cursor: array [0..Pred(3)] of float; 
//lay: UINT; (* enkele realtime vars *)
//grav: float; 
//rtf: float; 
//maxdrawsector: smallint; 
//flag: smallint; 
//pad: integer; 
//ed: pinteger; 
//radio: pRadio; 
//r: RenderData; 
//scriptlink: ScriptLink; 
end;

(* ******************************************* *)
(* * * * * * * *  SPACE STRUCTS  * * * * * * * *)
(* ******************************************* *)
//BGpic = record
//ima: pImage; 
//tex: pTex; 
//xof: float; 
//yof: float; 
//size: float; 
//zoom: float; 
//blend: float; 
//xim: smallint; 
//yim: smallint; 
//rect: pUINT; 
//end;
//BGpic = BGpic; 
//{$undef}
//{$undef}
//{$define clipsta}
//{$define clipend}

pView3D = ^View3D;
View3D = record
next: pView3D;
prev: pView3D;
spacetype: integer;
viewmat: array [0..Pred(4),0..Pred(4)] of cfloat;
viewinv: array [0..Pred(4),0..Pred(4)] of cfloat;
persmat: array [0..Pred(4),0..Pred(4)] of cfloat;
persinv: array [0..Pred(4),0..Pred(4)] of cfloat;
viewquat: array [0..Pred(4)] of cfloat;
dist: cfloat;
persp: smallint;
view: smallint;
drawtype: smallint;
localview: smallint;
lay: integer;
layact: integer;
scenelock: smallint;
around: smallint;
camzoom: smallint;
flag: smallint;
lens: cfloat;
grid: cfloat; {!!!3 unknown typedef}
end;

//{$undef}
//{$undef}
//{$define bnearsub}
//{$define bfarsub}

pView2D = ^View2D;
View2D = record
tot: rctf;
cur: rctf;
vert: rcti;
hor: rcti;
mask: rcti;
min: array [0..Pred(2)] of cfloat;
max: array [0..Pred(2)] of cfloat;
minzoom: cfloat;
maxzoom: cfloat;
scroll: smallint;
keeptot: smallint;
keepaspect: smallint;
keepzoom: smallint;
end;

pSpaceIpo = ^SpaceIpo;
SpaceIpo = record
next: pSpaceIpo;
prev: pSpaceIpo;
spacetype: integer;
rowbut: cUINT;
v2d: View2D;
editipo: pinteger;
ipokey: ListBase;
//ipo: pIpo;
from: pID;
totipo: smallint;
lock: smallint;
butofs: smallint;
channel: smallint;
showkey: smallint;
blocktype: smallint;
menunr: smallint;
rt: smallint;
tot: rctf;
end;

pSpaceButs = ^SpaceButs;
SpaceButs = record
next: pSpaceButs;
prev: pSpaceButs;
spacetype: integer;
pad1: integer;
v2d: View2D;
mainb: smallint;
menunr: smallint; (* texnr en menunr moeten shorts blijven *)
lock: smallint;
mainbo: smallint;
lockpoin: pinteger;
texnr: smallint;
texfrom: char;
rt2: char; (* temps *)
(* preview render *)
rectx: smallint;
recty: smallint;
rect: pcUINT;
cury: smallint;
modeltype: smallint;
scriptblock: smallint;
pad2: smallint;
end;

pSpaceSeq = ^SpaceSeq;
SpaceSeq = record
next: pSpaceSeq;
prev: pSpaceSeq;
spacetype: integer;
pad1: integer;
v2d: View2D;
mainb: smallint;
zoom: smallint;
pad2: integer;
end;

pSpaceFile = ^SpaceFile;
SpaceFile = record
next: pSpaceFile;
prev: pSpaceFile;
spacetype: integer;
pad: integer;
//filelist: pdirentry;
totfile: integer;
title: array [0..Pred(24)] of char;
dir: array [0..Pred(160)] of char;
//file: array [0..Pred(80)] of char;
_type: smallint;
ofs: smallint;
flag: smallint;
sort: smallint;
maxnamelen: smallint;
collums: smallint;
libfiledata: pchar;
retval: smallint;
ipotype: smallint;
menu: smallint;
act: smallint;
returnfunc: procedure;
end;

//SpaceOops = record
//next: pSpaceOops; 
//prev: pSpaceOops; 
//spacetype: integer; 
//pad: integer; 
//v2d: View2D; 
//oops: ListBase; 
//lock: smallint; 
//visiflag: smallint; 
//flag: smallint; 
//rt: smallint; 
//lockpoin: pinteger; 
//end;
//SpaceOops = SpaceOops; 
//SpaceImage = record
//next: pSpaceImage; 
//prev: pSpaceImage; 
//spacetype: integer; 
//pad: integer; 
//v2d: View2D; 
//image: pImage; 
//mode: smallint; 
//zoom: smallint; 
//imanr: smallint; 
//curtile: smallint; 
//xof: smallint; 
//yof: smallint; 
//flag: smallint; 
//rt: smallint; 
//end;
//SpaceImage = SpaceImage; 
//SpaceText = record
//next: pSpaceText; 
//prev: pSpaceText; 
//spacetype: integer; 
//left: integer; 
//text: pText; 
//top: integer; 
//viewlines: integer; 
//flags: smallint; 
//menunr: smallint; 
//font_id: integer; 
//lheight: integer; 
//pix_per_line: float; 
//txtscroll: rcti; 
//txtbar: rcti; 
//py_draw: pinteger; 
//py_event: pinteger; 
//py_button: pinteger; 
//py_head_draw: pinteger; 
//py_head_event: pinteger; 
//py_head_button: pinteger; 
//end;
//SpaceText = SpaceText; 
//(* SpaceImaSel in aparte headerfile *)
//(* SpacePaint in aparte headerfile *)

//(* ************* GLOBAL ******************* *)
//dl_GourFace = record
//co: array [0..Pred(3),0..Pred(3)] of float; 
//col: array [0..Pred(6)] of UINT; 
//end;
//dl_GourFace = dl_GourFace; 
//FastLamp = record
//type: smallint; 
//mode: smallint; 
//lay: smallint; 
//rt: smallint; 
//co: array [0..Pred(3)] of float; 
//vec: array [0..Pred(3)] of float; 
//dist: float; 
//distkw: float; 
//att1: float; 
//att2: float; 
//spotsi: float; 
//spotbl: float; 
//r: float; 
//g: float; 
//b: float; 
//end;
//FastLamp = FastLamp; 
//DispList = record
//next: pDispList; 
//prev: pDispList; 
//type: smallint; 
//flag: smallint; 
//parts: integer; 
//nr: integer; 
//col: smallint; 
//rt: smallint; (* rt wordt gebruikt door initrenderNurbs *)
//verts: pfloat; 
//nors: pfloat; 
//index: pinteger; 
//col1: pUINT; 
//col2: pUINT; 
//end;
//DispList = DispList; 
//UserDef = record
//flag: smallint; 
//dupflag: smallint; 
//savetime: integer; 
//tempdir: array [0..Pred(64)] of char; 
//fontdir: array [0..Pred(64)] of char; 
//renderdir: array [0..Pred(64)] of char; 
//textudir: array [0..Pred(64)] of char; 
//plugseqdir: array [0..Pred(64)] of char; 
//versions: smallint; 
//rt: smallint; 
//end;
//UserDef = UserDef;

//{$include "exports.h"}
//{$endif}
(* BLENDER_H *)

procedure duplicatelist(list1: pListBase;  list2: pListBase);
procedure initglobals;

var
matone: Mat4 = ((1,0,0,0),(0,1,0,0),(0,0,1,0),(0,0,0,1));

implementation

uses
blendef,
blenglob;

//{$include "blender.h"}
//type
//Global = record
//end;

var
versionstr: array [0..47] of char = (#14, #92, #240, #247, #43, #199, #126, #71, #69, #113, #64, #241, #115, #139, #242, #216, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0);

versionfstr: array [0..23] of char; (* voor files *)

//(* ********** vrijgeven ********** *)
//
//procedure free_blender;
//var
//main: pMain; 
//begin
//  
//  freestructDNA_all(); 
//  free_mainlist(); 
//  (* genfile.c *)
//  freeImBufdata(); 
//  (* library.c *)
//end;(* imbuf lib *)
//
//
//procedure addlisttolist(list1: pListBase;  list2: pListBase); 
//begin
//  if list2.first=0
//  then
//  exit;
//  if list1.first=0
//  then
//  begin 
//    list1.first:= list2.first; 
//    list1.last:= list2.last; 
//  end;
//  else
//  begin 
//    ((structLink* )list1.last).next:=list2.first; 
//    ((structLink* )list2.first).prev:=list1.last; 
//    list1.last:= list2.last; 
//  end;
//  list2.first:= list2.last:=0; 
//end;

function dupallocN(mem: pointer): pointer;
var
memh: pMemHead;
new: pinteger;
begin
  if mem=nil  then
       exit(nil);

  memh:= pMemHead(mem);
  dec(memh);
  if memh^.len=0
  then
  begin
    printf('error: dupallocN with len==0 %s\n',[memh^.name]);
      exit(nil);
  end
  else
  begin
    new:= mallocN(memh^.len,'dupli_alloc');
    memcpy(new,mem,memh^.len);
      exit(new);
  end;
end;

//function alloc_len(mem: pinteger): integer; 
//var
//memh: pMemHead; 
//begin
//  
//  if mem=0
//  then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  memh:= {!!!a type cast? =>} {pMemHead(}mem; 
//  dec(memh); 
//  begin
//    result:= memh.len; 
//    exit;
//  end;
//end;
//(* kopie van 2 naar 1 *)
//type
//Link = record
//end;

procedure duplicatelist(list1: pListBase;  list2: pListBase);
var
   link1, link2: pLink;
begin
                     list1^.last:=nil;
  list1^.first:=list1^.last ;
  link2:= list2^.first;
  while link2 <>nil do
  begin
    link1:= dupallocN(link2);
    addtail(list1,link1);
    link2:= link2^.next;
  end;
end;

procedure initglobals;
begin
  bzero(@G,sizeof(Global));
//  G.animspeed:= 4; 
  G.main:= callocN(sizeof(Main),'initglobals');
  addtail(@G.mainbase,G.main);
//  strcpy(G.ima,'//'); 
//  strcpy(G.psx,'//'); 
  G.version:= 172;

  G.order:= 1;

  sprintf(versionfstr,'BLENDER  %d',[G.version]);

  if pchar(@G.order + 1)^<>#0  then
  begin
    G.order:= B_ENDIAN;
    versionfstr[9]:= 'V';
  end
  else
  begin
    G.order:= L_ENDIAN;
    versionfstr[9]:= 'v';
  end;

  if sizeof(pointer)=8 then
  versionfstr[8]:= '-'
  else
  versionfstr[8]:= '_';

  sprintf(@versionstr[16],'%d',[G.version]);

//  {$ifdef __sgi}
//  initmoviepointers(); 
//  (* define in iff.h *)
//  {$endif}
//  clear_workob(); 
end;(* object.c *)


end.
