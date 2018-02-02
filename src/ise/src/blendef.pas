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
unit blendef;

interface

{$ifdef __WIN32}
{$else}
{$ifndef __BeOS}

  const
    O_BINARY = 0;
{$endif}
{$endif}
{$ifdef __FreeBSD__}
  {#define dlsym(handle, sym) dlsym(handle, "_" sym) }
{$endif}
{$ifdef __WIN32}
  { was #define dname(params) para_def_expr }
  { argument types are unknown }
  { return type might be wrong }

  function usleep(x : longint) : longint;

  const
    TIME_INTERVAL = 1000.0;
{$else}

  const
    TIME_INTERVAL = 100.0;
{$endif}
{$ifdef ZRS_DEFINES}
  {#define freeN(x) freeN(x); x=NULL; }
{$endif}
{$ifndef MAXFLOAT}

  { was #define dname def_expr }
  function MAXFLOAT : single;

{$endif}
  { blendef.h	alle defines voor blenderstructuur
   *
   * jan 95 maart 95
   *
    }
{ $include <single.h>	/* deze moet een keer naar de blender.h */}
{ $include "code.h"}
  { **************** ALGEMEEN *********************  }
  { #define COREDUMP(n)			static int times=0; if(times++ == n) abort(); }
  { #define VECCOPY(v1,v2) 		*(v1)= *(v2); *(v1+1)= *(v2+1); *(v1+2)= *(v2+2); }
  { #define QUATCOPY(v1,v2) 	*(v1)= *(v2); *(v1+1)= *(v2+1); *(v1+2)= *(v2+2); *(v1+3)= *(v2+3); }
  { #define INPR(v1, v2)		( (v1)[0]*(v2)[0] + (v1)[1]*(v2)[1] + (v1)[2]*(v2)[2] ) }
  procedure CLAMP(var a: smallint; b, c: smallint); //{ #define CLAMP(a, b, c)		if((a)<(b)) (a)=(b); else if((a)>(c)) (a)=(c) }
  { #define CLAMPIS(a, b, c)	((a)<(b) ? (b) : (a)>(c) ? (c) : (a)) }
  { #define CLAMPTEST(a, b, c)	if((b)<(c)) CLAMP(a, b, c); else CLAMP(a, c, b); }
  { #define IS_EQ(a,b) ((fabs((double)(a)-(b)) >= (double) FLT_EPSILON) ? 0 : 1) }
  { #define INIT_MINMAX(min, max) (min)[0]= (min)[1]= (min)[2]= 1.0e30; (max)[0]= (max)[1]= (max)[2]= -1.0e30; }
  { #define DO_MINMAX(vec, min, max) if( (min)[0]>(vec)[0] ) (min)[0]= (vec)[0]; \ }
  { if( (min)[1]>(vec)[1] ) (min)[1]= (vec)[1]; \ }
  { if( (min)[2]>(vec)[2] ) (min)[2]= (vec)[2]; \ }
  { if( (max)[0]<(vec)[0] ) (max)[0]= (vec)[0]; \ }
  { if( (max)[1]<(vec)[1] ) (max)[1]= (vec)[1]; \ }
  { if( (max)[2]<(vec)[2] ) (max)[2]= (vec)[2]; \ }
  { #define DO_MINMAX2(vec, min, max) if( (min)[0]>(vec)[0] ) (min)[0]= (vec)[0]; \ }
  { if( (min)[1]>(vec)[1] ) (min)[1]= (vec)[1]; \ }
  { if( (max)[0]<(vec)[0] ) (max)[0]= (vec)[0]; \ }
  { if( (max)[1]<(vec)[1] ) (max)[1]= (vec)[1]; }
  { #define MINSIZE(val, size)	( ((val)>=0.0) ? (((val)<(size)) ? (size): (val)) : ( ((val)>(-size)) ? (-size) : (val))) }
  { #define BTST(a,b)	( ( (a) & 1<<(b) )!=0 ) }
  { #define BCLR(a,b)	( (a) & ~(1<<(b)) ) }
  { #define BSET(a,b)	( (a) | 1<<(b) ) }
  { /* bit-row */ }
  { #define BROW(min, max)	(((max)>=31? 0xFFFFFFFF: (1<<(max+1))-1) - ((min)? ((1<<(min))-1):0) ) }

  type
    byte16 = record
        a : longint;
        b : longint;
        c : longint;
        d : longint;
      end;
  { #define COPY_4(a,b)		*((int *)(a))= *((int *)(b)) }
  { #define COPY_8(a,b)		*((byte8 *)(a))= *((byte8 *)(b)) }
  { #define COPY_12(a,b)	*((byte12 *)(a))= *((byte12 *)(b)) }
  { #define COPY_16(a,b)	*((byte16 *)(a))= *((byte16 *)(b)) }
  { #define LONGCOPY(a, b, c)	int lcpc=c, *lcpa=(int *)a, *lcpb=(int *)b; while(lcpc-->0) *(lcpa++)= *(lcpb++); }
  { #define SWITCH_LONGINT(a)	char s_i, *p_i; p_i= (char *)&(a); \ }
  { s_i= p_i[0]; p_i[0]= p_i[7]; p_i[7]= s_i; s_i= p_i[1]; p_i[1]= p_i[6]; p_i[6]= s_i; \ }
  { s_i= p_i[2]; p_i[2]= p_i[5]; p_i[5]= s_i; s_i= p_i[3]; p_i[3]= p_i[4]; p_i[4]= s_i;  }
  { #define SWITCH_INT(a)	char s_i, *p_i; p_i= (char *)&(a); s_i= p_i[0]; p_i[0]= p_i[3]; p_i[3]= s_i; s_i= p_i[1]; p_i[1]= p_i[2]; p_i[2]= s_i;  }
  { #define SWITCH_SHORT(a)	char s_i, *p_i; p_i= (char *)&(a); s_i= p_i[0]; p_i[0]= p_i[1]; p_i[1]= s_i;  }
  { #if defined(__sgi) || defined(__SUN) || defined (__PPC__) }
  { /* big endian */ }
  { #define MAKE_ID2(c, d)		( (c)<<8 | (d) ) }
  { #define MOST_SIG_BYTE				0 }
  { #define BBIG_ENDIAN }
  { #else }
  { /* little endian  */ }
  { #define MAKE_ID2(c, d)		( (d)<<8 | (c) ) }
  { #define MOST_SIG_BYTE				1 }
  { #define BLITTLE_ENDIAN }
  { #endif }

  const
    SELECT = 1;
    ACTIVE = 2;
    NOT_YET = 0;
  { #define TESTBASE(base)	( ((base)->flag & SELECT) && ((base)->lay & G.vd->lay) ) }
  { #define TESTBASELIB(base)	( ((base)->flag & SELECT) && ((base)->lay & G.vd->lay) && ((base)->object->id.lib==0)) }
  function FIRSTBASE: pointer; //		G.scene->base.first }
  { #define LASTBASE		G.scene->base.last }
  { #define BASACT			(G.scene->basact) }
  { #define OBACT			(BASACT? BASACT->object: 0) }
  { #define ID_NEW(a)		if( (a) && (a)->id.new ) (a)= (void *)(a)->id.new }
  { #define ID_NEW_US(a)	if( (a)->id.new) (a)= (void *)(a)->id.new; (a)->id.us++; }
  { #define ID_NEW_US2(a)	if( ((ID *)a)->new) (a)= ((ID *)a)->new; ((ID *)a)->us++; }
  function CFRA: smallint; { #define	CFRA			(G.scene->r.cfra) }
  { #define	F_CFRA			((single)(G.scene->r.cfra)) }
  { #define	SFRA			(G.scene->r.sfra) }
  { #define	EFRA			(G.scene->r.efra) }
  { #define ISPOIN(a, b, c)			( (a->b) && (a->c) ) }
  { #define ISPOIN3(a, b, c, d)		( (a->b) && (a->c) && (a->d) ) }
  { #define ISPOIN4(a, b, c, d, e)	( (a->b) && (a->c) && (a->d) && (a->e) ) }
  { #define KNOTSU(nu)	    ( (nu)->orderu+ (nu)->pntsu+ (nu->orderu-1)*((nu)->flagu & 1) ) }
  { #define KNOTSV(nu)	    ( (nu)->orderv+ (nu)->pntsv+ (nu->orderv-1)*((nu)->flagv & 1) ) }

  const
  { psfont  }
    FNT_PDRAW = 1;
    FNT_HAEBERLI = 2;
  { isect en scanfill  }
    COMPLIMIT = 0.0003;
  { **************** MAX *********************  }
    MAXLAMP = 256;
  { max lengte material array, 16 vanwege bitjes in matfrom  }
    MAXMAT = 16;
    MAXPICKBUF = 2000;
    MAXSEQ = 32;
  {  in Image struct  }
    MAXMIPMAP = 10;
  {  in ColorBand struct  }
    MAXCOLORBAND = 16;
  { in buttons.c  }
    MAX_EFFECT = 20;
  { **************** GLOBAL *********************  }
  { G.f  }
    G_DISABLE_OK = 1;
    G_PLAYANIM = 2;
    G_TEST_DUPLI = 4;
    G_SIMULATION = 8;
    G_BACKBUFSEL = 16;
    G_PICKSEL = 32;
    G_DRAWNORMALS = 64;
    G_DRAWFACES = 128;
    G_FACESELECT = 256;
    G_DRAW_EXT = 512;
    G_VERTEXPAINT = 1024;
    G_ALLEDGES = 2048;
    G_DEBUG = 4096;
    G_SCENESCRIPT = 8192;
    G_PROPORTIONAL = 16384;
  { G.simulf  }
    G_LOADFILE = 2;
    G_RESTART = 4;
    G_QUIT = 8;
    G_NETWORK = 16;
  { G.qual  }
    R_SHIFTKEY = 1;
    L_SHIFTKEY = 2;
    LR_SHIFTKEY = 3;
    R_ALTKEY = 4;
    L_ALTKEY = 8;
    LR_ALTKEY = 12;
    R_CTRLKEY = 16;
    L_CTRLKEY = 32;
    LR_CTRLKEY = 48;
  { getbutton  }
    L_MOUSE = 1;
    M_MOUSE = 2;
    R_MOUSE = 4;
  { G.order  }
    L_ENDIAN = 1;
    B_ENDIAN = 0;
  { G.special1  }
    G_HOLO = 1;
  { do_global_buttons(event)  }
  { VIEW3D: 100  }
    B_HOME = 101;
    B_LAY = 102;
  { pasop: codes 102-132 in gebuik voor layers  }
    B_SCENELOCK = 140;
    B_LOCALVIEW = 141;
    B_U_CAPSLOCK = 142;
    B_EDITMODE = 143;
    B_VPAINT = 144;
    B_FACESEL = 145;
    B_VIEWBUT = 146;
    B_PERSP = 147;
    B_PROPTOOL = 148;
    B_VIEWRENDER = 149;
  { IPO: 200  }
    B_IPOHOME = 201;
    B_IPOBORDER = 202;
    B_IPOCOPY = 203;
    B_IPOPASTE = 204;
    B_IPOCONT = 205;
    B_IPOEXTRAP = 206;
    B_IPOCYCLIC = 207;
    B_IPOMAIN = 208;
    B_IPOSHOWKEY = 209;
    B_IPOCYCLICX = 210;
  { OOPS: 250  }
    B_OOPSHOME = 251;
    B_OOPSBORDER = 252;
    B_NEWOOPS = 253;
  { INFO: 300  }
  { pas op: ook in filesel.c en editobject.c  }
    B_INFOSCR = 301;
    B_INFODELSCR = 302;
    B_INFOSCE = 304;
    B_INFODELSCE = 305;
    B_FILEMENU = 306;
  { IMAGE: 350  }
    B_SIMAGEHOME = 351;
    B_SIMABROWSE = 352;
    B_SIMAGELOAD = 353;
    B_SIMAGEDRAW = 354;
    B_BE_SQUARE = 355;
    B_SIMAGEDRAW1 = 356;
    B_TWINANIM = 357;
    B_SIMAGEREPLACE = 358;
  { BUTS: 400  }
    B_BUTSHOME = 401;
    B_BUTSPREVIEW = 402;
    B_MATCOPY = 403;
    B_MATPASTE = 404;
    B_MESHTYPE = 405;
  { IMASEL: 450  }
  { in de imasel.h  }
  { BPAINT: 500  }
  { in de bpaint.h  }
  { TEXT: 500  }
    B_TEXTBROWSE = 501;
    B_TEXTALONE = 502;
    B_TEXTLOCAL = 503;
    B_TEXTDELETE = 504;
    B_TEXTFONT = 505;
    B_TEXTSTORE = 506;
  { FILE: 550  }
    B_SORTFILELIST = 551;
    B_RELOADDIR = 552;
  { SEQUENCE: 600  }
    B_SEQHOME = 601;
    B_SEQCLEAR = 602;
    B_NOTHING = 1;
    B_WORLDLOCAL = 28;
    B_WORLDALONE = 29;
    B_LATTLOCAL = 30;
    B_MBALLLOCAL = 31;
    B_CAMERALOCAL = 32;
    B_OBLOCAL = 33;
    B_IPOLOCAL = 34;
    B_LAMPLOCAL = 35;
    B_MATLOCAL = 36;
    B_TEXLOCAL = 37;
    B_MESHLOCAL = 38;
    B_CURVELOCAL = 39;
    B_LATTALONE = 40;
    B_MBALLALONE = 41;
    B_CAMERAALONE = 42;
    B_OBALONE = 43;
    B_IPOALONE = 44;
    B_LAMPALONE = 45;
    B_MATALONE = 46;
    B_TEXALONE = 47;
    B_MESHALONE = 48;
    B_CURVEALONE = 49;
  { EVENT < 50: alone's en locals  }
    B_LTEXBROWSE = 78;
    B_MESHBROWSE = 79;
    B_EXTEXBROWSE = 80;
    B_LOADTEMP = 81;
    B_MATDELETE = 82;
    B_TEXDELETE = 83;
    B_IPODELETE = 84;
    B_WORLDDELETE = 85;
    B_WTEXBROWSE = 86;
    B_WORLDBROWSE = 87;
    B_IPOBROWSE = 88;
    B_NEWFRAME = 89;
    B_LAMPBROWSE = 90;
    B_MATBROWSE = 91;
    B_TEXBROWSE = 92;
    B_EDITBROWSE = 93;
    B_AUTOTEXNAME = 94;
    B_AUTOMATNAME = 95;
    B_MATLOCK = 96;
    B_IDNAME = 97;
    B_NEWSPACE = 98;
    B_FULL = 99;
    B_REDR = 100;
  { editbutflag  }
    B_CLOCKWISE = 1;
    B_KEEPORIG = 2;
    B_BEAUTY = 4;
    B_SMOOTH = 8;
  { **************** SPACE EVENT CODES ***************   }
    REMAKEIPO = 1;
    OOPS_TEST = 2;
  { **************** QUEUE EVENTS *********************  }
    CHANGED = $4000;
    DRAWEDGES = $4001;
    AFTERQUEUE = $4002;
    BACKBUFDRAW = $4003;
    EXECUTE = $4004;
    IGNORE_REDRAW = $4005;
    LOAD_FILE = $4006;
    RESHAPE = $4007;
  { REDRAWVIEW3D moet de eerste zijn (laagste nummer) voor buttons!  }
    REDRAWVIEW3D = $4010;
    REDRAWBUTSHEAD = $4011;
    REDRAWBUTSALL = $4012;
    REDRAWBUTSVIEW = $4013;
    REDRAWBUTSLAMP = $4014;
    REDRAWBUTSMAT = $4015;
    REDRAWBUTSTEX = $4016;
    REDRAWBUTSANIM = $4017;
    REDRAWBUTSWORLD = $4018;
    REDRAWBUTSRENDER = $4019;
    REDRAWBUTSEDIT = $401A;
    REDRAWVIEWCAM = $401B;
    REDRAWHEADERS = $401C;
    REDRAWBUTSGAME = $401D;
    REDRAWBUTSRADIO = $401E;
    REDRAWVIEW3D_Z = $401F;
    REDRAWALL = $4020;
    REDRAWINFO = $4021;
    RENDERPREVIEW = $4022;
    REDRAWIPO = $4023;
    REDRAWDATASELECT = $4024;
    REDRAWSEQ = $4025;
    REDRAWIMAGE = $4026;
    REDRAWOOPS = $4027;
    REDRAWIMASEL = $4028;
    AFTERIMASELIMA = $4029;
    AFTERIMASELGET = $402A;
    AFTERIMAWRITE = $402B;
    IMALEFTMOUSE = $402C;
    AFTERPIBREAD = $402D;
    REDRAWPAINT = $402E;
    REDRAWTEXT = $402F;
    REDRAWBUTSSCRIPT = $4030;
  { ***************** DISPLIST *****************  }
    DL_POLY = 0;
    DL_SEGM = 1;
    DL_SURF = 2;
    DL_TRIA = 3;
    DL_INDEX3 = 4;
    DL_INDEX4 = 5;
    DL_VERTCOL = 6;
    DL_VERTS = 7;
    DL_NORS = 8;
  { #define DL_SURFINDEX(cyclu, cyclv, sizeu, sizev)	    \ }
  { \ }
  { if( (cyclv)==0 && a==(sizev)-1) break;		    \ }
  { if(cyclu) 						    \ }
  { p1= sizeu*a;					    \ }
  { p2= p1+ sizeu-1;				    \ }
  { p3= p1+ sizeu;					    \ }
  { p4= p2+ sizeu;					    \ }
  { b= 0;						    \ }
  { 							    \ }
  { else 						    \ }
  { p2= sizeu*a;					    \ }
  { p1= p2+1;					    \ }
  { p4= p2+ sizeu;					    \ }
  { p3= p1+ sizeu;					    \ }
  { b= 1;						    \ }
  { 							    \ }
  { if( (cyclv) && a==sizev-1) 			    \ }
  { p3-= sizeu*sizev;				    \ }
  { p4-= sizeu*sizev;				    \ }
  {  }
  { **************** SPACE *********************  }
  { view3d->flag  }
    V3D_DISPIMAGE = 1;
    V3D_DISPBGPIC = 2;
    V3D_SETUPBUTS = 4;
    V3D_NEEDBACKBUFDRAW = 8;
    V3D_MODE = (16+32)+64;
    V3D_EDITMODE = 16;
    V3D_VERTEXPAINT = 32;
    V3D_FACESELECT = 64;
  { view3d->around  }
    V3D_CENTRE = 0;
    V3D_CENTROID = 3;
    V3D_CURSOR = 1;
    V3D_LOCAL = 2;
    BUTS_VIEW = 0;
    BUTS_LAMP = 1;
    BUTS_MAT = 2;
    BUTS_TEX = 3;
    BUTS_ANIM = 4;
    BUTS_WORLD = 5;
    BUTS_RENDER = 6;
    BUTS_EDIT = 7;
    BUTS_GAME = 8;
    BUTS_FPAINT = 9;
    BUTS_RADIO = 10;
    BUTS_SCRIPT = 11;
  { deze getallen ook invullen in blender.h SpaceFile: struct dna herkent geen defines  }
    FILE_MAXDIR = 160;
    FILE_MAXFILE = 80;
  { filesel types  }
    FILE_UNIX = 8;
    FILE_BLENDER = 8;
    FILE_SPECIAL = 9;
    FILE_LOADLIB = 1;
    FILE_MAIN = 2;
  { sfile->flag  }
    FILE_SHOWSHORT = 1;
    FILE_STRINGCODE = 2;
    FILE_LINK = 4;
    FILE_HIDE_DOT = 8;
  { sfile->sort  }
    FILE_SORTALPHA = 0;
    FILE_SORTDATE = 1;
    FILE_SORTSIZE = 2;
    FILE_SORTEXTENS = 3;
  { files in filesel list: 2=ACTIVE   }
    HILITE = 1;
    BLENDERFILE = 4;
    PSXFILE = 8;
    IMAGEFILE = 16;
    MOVIEFILE = 32;
  { hoogte scrollbar  }
    SCROLLH = 16;
  { breedte scrollbar  }
    SCROLLB = 16;
  { SpaceImage->mode  }
    SI_TEXTURE = 0;
    SI_SHOW = 1;
  { SpaceImage->flag  }
    SI_BE_SQUARE = 1;
    SI_EDITTILE = 2;
  { SpaceOops->flag  }
    SO_TESTBLOCKS = 1;
    SO_NEWSELECTED = 2;
  { SpaceOops->visiflag  }
    OOPS_SCE = 1;
    OOPS_OB = 2;
    OOPS_ME = 4;
    OOPS_CU = 8;
    OOPS_MB = 16;
    OOPS_LT = 32;
    OOPS_LA = 64;
    OOPS_MA = 128;
    OOPS_TE = 256;
    OOPS_IP = 512;
    OOPS_LAY = 1024;
    OOPS_LI = 2048;
    OOPS_IM = 4096;
  { **************** MAIN *********************  }
  { id->flag: eerste 8 bits altijd op nul zetten bij inlezen  }
    LIB_LOCAL = 0;
    LIB_EXTERN = 1;
    LIB_INDIRECT = 2;
    LIB_TEST = 8;
    LIB_TESTEXT = 9;
    LIB_TESTIND = 10;
    LIB_READ = 16;
    LIB_NEEDLINK = 32;
    LIB_NEW = 256;
    LIB_FAKEUSER = 512;
  { vrije testflag  }
    LIB_DOIT = 1024;

  { ID  }
ID_SCE = $5343; // 	MAKE_ID2('S', 'C') }
  { #define ID_LI	MAKE_ID2('L', 'I') }
  { #define ID_OB	MAKE_ID2('O', 'B') }
  { #define ID_ME	MAKE_ID2('M', 'E') }
  { #define ID_CU	MAKE_ID2('C', 'U') }
  { #define ID_MB	MAKE_ID2('M', 'B') }
  { #define ID_MA	MAKE_ID2('M', 'A') }
  { #define ID_TE	MAKE_ID2('T', 'E') }
  { #define ID_IM	MAKE_ID2('I', 'M') }
  { #define ID_IK	MAKE_ID2('I', 'K') }
  { #define ID_WV	MAKE_ID2('W', 'V') }
  { #define ID_LT	MAKE_ID2('L', 'T') }
  { #define ID_SE	MAKE_ID2('S', 'E') }
  { #define ID_LF	MAKE_ID2('L', 'F') }
  { #define ID_LA	MAKE_ID2('L', 'A') }
  { #define ID_CA	MAKE_ID2('C', 'A') }
  { #define ID_IP	MAKE_ID2('I', 'P') }
  { #define ID_KE	MAKE_ID2('K', 'E') }
  { #define ID_WO	MAKE_ID2('W', 'O') }
ID_SCR = $5352; //MAKE_ID2('S', 'R') }
  { #define ID_VF	MAKE_ID2('V', 'F') }
  { #define ID_TXT	MAKE_ID2('T', 'X') }
  { #define ID_ID	MAKE_ID2('I', 'D') }
  { #define ID_SEQ	MAKE_ID2('S', 'Q') }

  { **************** SCENE *********************  }
    RAD_PHASE_PATCHES = 1;
    RAD_PHASE_FACES = 2;
  { base->flag en ob->flag  }
    BA_WASSEL = 2;
    BA_PARSEL = 4;
    BA_WHERE_UPDATE = 8;
    BA_DISP_UPDATE = 16;
    BA_DO_IPO = 32;
    BA_FROMSET = 128;
    OB_DO_IMAT = 256;
    OB_FROMDUPLI = 512;
    OB_DONE = 1024;
    OB_RADIO = 2048;
  { sce->flag  }
    SCE_ADDSCENAME = 1;
  { return flag next_object function  }
    F_START = 0;
    F_SCENE = 1;
    F_SET = 2;
    F_DUPLI = 3;
  { **************** OBJECT *********************  }
  { type  }
    OB_EMPTY = 0;
    OB_MESH = 1;
    OB_CURVE = 2;
    OB_SURF = 3;
    OB_FONT = 4;
    OB_MBALL = 5;
    OB_LAMP = 10;
    OB_CAMERA = 11;
    OB_IKA = 20;
    OB_WAVE = 21;
    OB_LATTICE = 22;
    OB_SECTOR = 23;
    OB_LIFE = 24;
  { partype: eerste 4 bits: type  }
    PARTYPE = 15;
    PAROBJECT = 0;
    PARCURVE = 1;
    PARKEY = 2;
    PARLIMB = 3;
    PARSKEL = 4;
    PARVERT1 = 5;
    PARVERT3 = 6;
    PARSLOW = 16;
  { char! transflag  }
    OB_OFFS_LOCAL = 1;
    OB_DUPLI = 8+16;
    OB_DUPLIFRAMES = 8;
    OB_DUPLIVERTS = 16;
    OB_DUPLIROT = 32;
    OB_DUPLINOSPEED = 64;
    OB_POWERTRACK = 128;
  { char! ipoflag  }
    OB_DRAWKEY = 1;
    OB_DRAWKEYSEL = 2;
    OB_OFFS_OB = 4;
    OB_OFFS_MAT = 8;
    OB_OFFS_VKEY = 16;
    OB_OFFS_PATH = 32;
    OB_OFFS_PARENT = 64;
    OB_OFFS_PARTICLE = 128;
  { trackflag / upflag  }
    OB_POSX = 0;
    OB_POSY = 1;
    OB_POSZ = 2;
    OB_NEGX = 3;
    OB_NEGY = 4;
    OB_NEGZ = 5;
  { infoflag  }
    OB_INFO_LOC = 1;
    OB_INFO_SCO = 2;
    OB_INFO_DROT = 4;
    OB_INFO_HOTSPOT = 8;
  { dt: nummers  }
    OB_BOUNDBOX = 1;
    OB_WIRE = 2;
    OB_SOLID = 3;
    OB_SHADED = 4;
    OB_TEXTURE = 5;
  { dtx: flags  }
    OB_AXIS = 2;
    OB_TEXSPACE = 4;
    OB_DRAWNAME = 8;
    OB_DRAWIMAGE = 16;
  { **************** MESH *********************  }
  { texflag  }
    AUTOSPACE = 1;
  { me->flag (let op alternatieven in blendpsx.h)  }
    ME_ISDONE = 1;
    ME_NOPUNOFLIP = 2;
    ME_TWOSIDED = 4;
    ME_UVEFFECT = 8;
    ME_VCOLEFFECT = 16;
    ME_AUTOSMOOTH = 32;
    ME_SMESH = 64;
  { puno (mface)  }
    ME_FLIPV1 = 1;
    ME_FLIPV2 = 2;
    ME_FLIPV3 = 4;
    ME_FLIPV4 = 8;
    ME_PROJXY = 16;
    ME_PROJXZ = 32;
    ME_PROJYZ = 64;
  { edcode (mface)  }
    ME_V1V2 = 1;
    ME_V2V3 = 2;
    ME_V3V1 = 4;
    ME_V3V4 = 4;
    ME_V4V1 = 8;
  { resterende vier bits voor dface  }
  { flag (mface)  }
    ME_SMOOTH = 1;
    ME_VERTEXCOL = 4;
  { mvert->flag (1=select)  }
    ME_SPHERETEST = 2;
    ME_SPHERETEMP = 4;
  { **************** CURVE *********************  }
  { flag  }
    CU_3D = 1;
    CU_FRONT = 2;
    CU_BACK = 4;
    CU_PATH = 8;
    CU_FOLLOW = 16;
    CU_UV_ORCO = 32;
    CU_NOPUNOFLIP = 64;
  { spacemode  }
    CU_LEFT = 0;
    CU_MIDDLE = 1;
    CU_RIGHT = 2;
    CU_FLUSH = 3;
  { flag (nurb)  }
    CU_SMOOTH = ME_SMOOTH;
  { type (nurb)  }
    CU_POLY = 0;
    CU_BEZIER = 1;
    CU_BSPLINE = 2;
    CU_CARDINAL = 3;
    CU_NURBS = 4;
    CU_2D = 8;
  { flagu flagv (nurb)  }
    CU_CYCLIC = 1;
  { h1 h2 (beztriple)  }
    HD_FREE = 0;
    HD_AUTO = 1;
    HD_VECT = 2;
    HD_ALIGN = 3;
  { *************** FONT ******************  }
    FO_CURS = 1;
    FO_CURSUP = 2;
    FO_CURSDOWN = 3;
    FO_DUPLI = 4;
  { **************** METABALL *********************  }
    MB_MAXELEM = 1024;
  { mb->flag  }
    MB_UPDATE_ALWAYS = 0;
    MB_UPDATE_HALFRES = 1;
    MB_UPDATE_FAST = 2;
  { ml->type  }
    MB_BALL = 0;
    MB_TUBEX = 1;
    MB_TUBEY = 2;
    MB_TUBEZ = 3;
    MB_CIRCLE = 4;
  { ml->flag  }
    MB_NEGATIVE = 2;
  { **************** MATERIAL *********************  }
  { colormodel  }
    MA_RGB = 0;
    MA_CMYK = 1;
    MA_YUV = 2;
    MA_HSV = 3;
  { mode (is int)  }
    MA_TRACEBLE = 1;
    MA_SHADOW = 2;
    MA_SHLESS = 4;
    MA_WIRE = 8;
    MA_VERTEXCOL = 16;
    MA_HALO = 32;
    MA_ZTRA = 64;
    MA_VERTEXCOLP = 128;
    MA_ZINV = 256;
    MA_HALO_RINGS = 256;
    MA_ENV = 512;
    MA_HALO_LINES = 512;
    MA_ONLYSHADOW = 1024;
    MA_HALO_XALPHA = 1024;
    MA_STAR = $800;
    MA_HALOTEX = $1000;
    MA_HALOPUNO = $2000;
    MA_NOMIST = $4000;
    MA_HALO_SHADE = $4000;
    MA_HALO_FLARE = $8000;
  { texco  }
    TEXCO_ORCO = 1;
    TEXCO_REFL = 2;
    TEXCO_NORM = 4;
    TEXCO_GLOB = 8;
    TEXCO_UV = 16;
    TEXCO_OBJECT = 32;
    TEXCO_LAVECTOR = 64;
    TEXCO_VIEW = 128;
    TEXCO_STICKY = 256;
    TEXCO_OSA = 512;
    TEXCO_WINDOW = 1024;
    NEED_UV = 2048;
  { mapto  }
    MAP_COL = 1;
    MAP_NORM = 2;
    MAP_COLSPEC = 4;
    MAP_COLMIR = 8;
    MAP_VARS = $FFF0;
    MAP_REF = 16;
    MAP_SPEC = 32;
    MAP_EMIT = 64;
    MAP_ALPHA = 128;
    MAP_HAR = 256;
    MAP_XTRA = 512;
  { **************** MTEX *********************  }
  { proj  }
    PROJ_N = 0;
    PROJ_X = 1;
    PROJ_Y = 2;
    PROJ_Z = 3;
  { texflag  }
    MTEX_RGBTOINT = 1;
    MTEX_STENCIL = 2;
    MTEX_NEGATIVE = 4;
    MTEX_ALPHAMIX = 8;
  { blendtype  }
    MTEX_BLEND = 0;
    MTEX_MUL = 1;
    MTEX_ADD = 2;
    MTEX_SUB = 3;
  { **************** TEX *********************  }
  { type  }
    TEX_CLOUDS = 1;
    TEX_WOOD = 2;
    TEX_MARBLE = 3;
    TEX_MAGIC = 4;
    TEX_BLEND = 5;
    TEX_STUCCI = 6;
    TEX_NOISE = 7;
    TEX_IMAGE = 8;
    TEX_PLUGIN = 9;
    TEX_ENVMAP = 10;
  { imaflag  }
    TEX_INTERPOL = 1;
    TEX_USEALPHA = 2;
    TEX_MIPMAP = 4;
    TEX_FIELDS = 8;
    TEX_IMAROT = 16;
    TEX_CALCALPHA = 32;
    TEX_ANIMCYCLIC = 64;
    TEX_ANIM5 = 128;
    TEX_ANTIALI = 256;
    TEX_ANTISCALE = 512;
    TEX_STD_FIELD = 1024;
    TEX_FILTX = 2048;
    TEX_GENLOCK = 4096;
    TEX_LASOPPATCH = 8192;
    TEX_MORKPATCH = 16384;
  { flag  }
    TEX_COLORBAND = 1;
    TEX_FLIPBLEND = 2;
    TEX_NEGALPHA = 4;
  { extend (begint bij 1 ivm backward comp.)  }
    TEX_EXTEND = 1;
    TEX_CLIP = 2;
    TEX_REPEAT = 3;
    TEX_CLIPCUBE = 4;
  { noisetype  }
    TEX_NOISESOFT = 0;
    TEX_NOISEPERL = 1;
  { wrap  }
    MTEX_FLAT = 0;
    MTEX_CUBE = 1;
    MTEX_TUBE = 2;
    MTEX_SPHERE = 3;
  { return value  }
    TEX_INT = 0;
    TEX_RGB = 1;
    TEX_NOR = 2;
  { **************** EnvMap *********************  }
  { type  }
    ENV_CUBE = 0;
    ENV_PLANE = 1;
    ENV_SPHERE = 2;
  { stype  }
    ENV_STATIC = 0;
    ENV_ANIM = 1;
    ENV_LOAD = 2;
  { ok  }
    ENV_NORMAL = 1;
    ENV_OSA = 2;
  { **************** IMAGE *********************  }
  { flag  }
    IMA_HALVE = 1;
    IMA_BW = 2;
    IMA_FROMANIM = 4;
    IMA_USED = 8;
  { tpageflag  }
    IMA_TILES = 1;
    IMA_TWINANIM = 2;
    IMA_COLCYCLE = 4;
  { **************** LAMP *********************  }
  { type  }
    LA_LOCAL = 0;
    LA_SUN = 1;
    LA_SPOT = 2;
    LA_HEMI = 3;
  { mode  }
    LA_SHAD = 1;
    LA_HALO = 2;
    LA_LAYER = 4;
    LA_QUAD = 8;
    LA_NEG = 16;
    LA_ONLYSHADOW = 32;
    LA_SPHERE = 64;
    LA_SQUARE = 128;
    LA_TEXTURE = 256;
    LA_OSATEX = 512;
  { mapto  }
    LAMAP_COL = 1;
  { **************** CAMERA *********************  }
  { type  }
    CAM_PERSP = 0;
    CAM_ORTHO = 1;
  { flag  }
    CAM_SHOWLIMITS = 1;
    CAM_SHOWMIST = 2;
    CAM_NETWORK = 4;
    CAM_SHOWNETCLIP = 8;
    CAM_HOLO1 = 16;
    CAM_HOLO2 = 32;
  { ***************** LATTICE *********************  }
  { flag  }
    LT_GRID = 1;
    LT_OUTSIDE = 2;
  { ***************** IKA *********************  }
  { flag  }
    IK_GRABEFF = 1;
    IK_XYCONSTRAINT = 2;
  { ***************** SECTOR *********************  }
  { #define DYNA_REF(ma)		(ma->mirr) }
  { #define DYNA_FH_DIST(ma)	(ma->mirg) }
  { #define DYNA_FH_INT(ma)		(ma->mirb) }
  { #define DYNA_FH_FRICT(ma)	(ma->specr) }
  { #define DYNA_FH_XYFRICT(ma)	(ma->emit) }
  { #define DYNA_LAY(ma)		(ma-> }
  { ma->mode  }
    DYNA_FH_NORMAL = 1;
  { type  }
    SE_CUBE = 0;
  { flag  }
    SE_SHOW_TEXMESH = 1;
    SE_GHOST_OTHER = 8;
    SE_RENDER_NETWORK = 16;
  { portal->type  }
    PO_XPOS = 1;
    PO_XNEG = 2;
    PO_YPOS = 3;
    PO_YNEG = 4;
    PO_ZPOS = 5;
    PO_ZNEG = 6;
    PO_FACE = 32;
  { tface->mode (beetje compatible met PMD formaat)  }
  { sharedvert bepaalt hoe de PSX versie wordt, sharedcol is vertexkleur en puno  }
    TF_QUAD = 1;
    TF_GOUR = 2;
    TF_TEX = 4;
    TF_SHAREDVERT = 8;
    TF_LIGHT = 16;
    TF_SHAREDCOL = 64;
    TF_TILES = 128;
    TF_BILLBOARD = 256;
    TF_TWOSIDE = 512;
    TF_NO_SUBDIV = 1024;
    TF_OBCOL = 2048;
    TF_MASK = 4096;
    TF_SHADOW = 8192;
  { tface->flag: 1=select 2=active }
    TF_SEL1 = 4;
    TF_SEL2 = 8;
    TF_SEL3 = 16;
    TF_SEL4 = 32;
    TF_HIDE = 64;
  { tface->transp  }
    TF_SOLID = 0;
    TF_ADD = 1;
    TF_ALPHA = 2;
    TF_SUB = 3;
  { OLD  }
    TF_ABR = 256+512;
  { netlink->flag  }
    NL_BACKWARD = 1;
    NL_BOTH = 2;
  { dface->flag  }
    DF_HILITE = 1;
  { dface->edcode: (zie ook mface) ook voor vertices!  }
    DF_V1 = 1;
    DF_V2 = 2;
    DF_V3 = 4;
    DF_V4 = 8;
    DF_V1V2 = 16;
    DF_V2V3 = 32;
    DF_V3V1 = 64;
    DF_V3V4 = 64;
    DF_V4V1 = 128;
  { ***************** LIFE *********************  }
  { sensor_input retval:  }
    SN_CHANGED = 1;
    SN_DOFORCE = 2;
    SN_DOSPEED = 4;
    SN_DOOMEGA = 8;
    SN_ROTCHANGED = 16;
  { komen uit blendpsx.h  }
    SE_MAXBUF = 32;
    LF_MAXBUF = 32;
    MAXACTVAR = 100;
    DTIME = 0.02;
    IDTIME = 50.0;
  { lf->type  }
    LF_PROP = 0;
    LF_DYNAMIC = 1;
    LF_LINK = 2;
  { lf->flag: buttons, sgistuff  }
    LF_SENSORS = 1;
    LF_SHOW_TEXMESH = 4;
    LF_GHOST_OTHER = 8;
    LF_DRAWNEAR = 16;
    LF_MAINACTOR = 32;
    LF_CAMERA_ACTOR = 64;
    LF_RECALCLIGHT = 128;
    LF_AERO_AXIS_Y = 256;
    LF_SPHERE = 2048;
    LF_DO_FH = 4096;
  { lf->dflag : starten altijd op nul  }
    LF_TEMPLIFE = 1;
    LF_DYNACHANGED = 2;
    LF_OMEGA = 4;
    LF_DONTDRAW = 8;
    LF_TRIP_POS = 16;
    LF_TRIP_NEG = 32;
    LF_NO_DAMAGE = 64;
  { lf->state: lokale vars: namen willekeurig?  }
    LF_STATE = 1;
    LF_DAMAGE = 2;
    LF_ACTION = 3;
    LF_FRAME = 4;
  { SENSORS  }
  { ev->flag  }
    SN_HOLD = 1;
    SN_NOT = 2;
  { ev->event: eerste 32 voor toetsen  }
  { ev->var: controller nr  }
    SN_NEAR = 64;
  { ev->fac: mindist, ev->fac1: enddist, ev->var: memory  }
    SN_CONTACT = 65;
  { ev->name: naam mat, ev->poin: mat  }
    SN_TIMER = 66;
  { ev->var: triggertime  }
    SN_VAR_EQUAL = 67;
  { ev->var: testwaarde:   }
    SN_VAR_INTERVAL = 68;
  { ev->sfac: min, ev->sfac1: max  }
    SN_VAR_CHANGED = 69;
  { ev->var: memory  }
    SN_TRIPFACE = 80;
  {  }
    SN_COLLISION = 81;
  { ev->sfac en ev->var: demping  }
  { ac->action  }
    SN_X_TRANS = 2;
    SN_Y_TRANS = 4;
    SN_Z_TRANS = 6;
    SN_X_ROT = 8;
    SN_Y_ROT = 10;
    SN_Z_ROT = 12;
  { ac->force  }
  { ac->flag: limits  }
    SN_LIMITS = 1;
    SN_IPOFORCE = 2;
    SN_IPO_SETVAR = 16;
  { 100-199: ipoos  }
    SN_PLAY_OBIPO = 100;
    SN_PINGPONG_OBIPO = 101;
    SN_FLIPPER_OBIPO = 102;
    SN_DELTA_OBIPO = 103;
    SN_LOOPSTOP_OBIPO = 104;
    SN_LOOPEND_OBIPO = 105;
    SN_K2K_OBIPO = 106;
  { ac->butsta en butend worden ac->min en max  }
  { ac->sta en end worden uitgelezen uit ipokeys   }
  { ac->var: bitjes:  }
    SN_K2K_PREV = 1;
    SN_K2K_CYCLIC = 2;
    SN_K2K_PINGPONG = 4;
    SN_K2K_HOLD = 8;
  { 200-299: divers  }
    SN_LAYERMOVE = 200;
    SN_STARTTIMER = 201;
  { ac->sta: initwaarde timer ac->cur  }
    SN_ADDLIFE = 202;
    SN_REPLACELIFE = 203;
    SN_LOADFILE = 204;
    SN_RESTART = 205;
    SN_QUIT = 206;
    SN_VISIBLE = 207;
    SN_GOTO = 208;
    SN_ENDLIFE = 209;
    SN_REPLACEMESH = 210;
    SN_SETCAMERA = 211;
  { ac->end: len, ac->sta: startstill  }
    SN_PLAYMOVIE = 212;
  { ac->sta: tijd, ac->cur: type, ac->poin: object  }
    SN_TRACKTO = 213;
  { ac->var: hoeveelheid  }
    SN_ADD_DAMAGE = 214;
  { is een 'hold' event  }
    SN_NO_DAMAGE = 215;
  { ac->min, ac->max: dist, ac->force=height  }
  { ac->fac= slow voor kijk-corr  }
    SN_ROBBIE_M = 216;
  { ac name: sample  }
    SN_PLAY_SOUND = 217;
  { ac->force, min, max is x,y,z  }
  { ac->var: 0 is abs, 1 is rel  }
  { 300-399: vars  }
    SN_SET_VARIABLE = 300;
    SN_ADD_VARIABLE = 301;
    SN_TOG_VARIABLE = 302;
    SN_IPO_VARIABLE = 303;
    SN_HOLD_VARIABLE = 304;
  { ac->sta: min, ac->end: max, ac->cur: initwaarde, ac->flag= flag  }
    SN_VAR_CYCLIC = 1;
    SN_VAR_NOPRINT = 2;
  { **************** IPO *********************  }
  { vartype  }
    IPO_CHAR = 0;
    IPO_SHORT = 1;
    IPO_INT = 2;
    IPO_LONG = 3;
    IPO_FLOAT = 4;
    IPO_DOUBLE = 5;
    IPO_FLOAT_DEGR = 6;
  { zeer speciaal geval in keys  }
    IPO_BEZTRIPLE = 100;
    IPO_BPOINT = 101;
    IPO_BITS = 16;
    IPO_CHAR_BIT = 16;
    IPO_SHORT_BIT = 17;
    IPO_INT_BIT = 18;
  { ipo  }
    IPO_CONST = 0;
    IPO_LIN = 1;
    IPO_BEZ = 2;
    IPO_MIXED = 3;
  { extrap  }
    IPO_HORIZ = 0;
    IPO_DIR = 1;
    IPO_CYCL = 2;
    IPO_CYCLX = 3;
  { flag  }
    IPO_VISIBLE = 1;
    IPO_SELECT = 2;
    IPO_EDIT = 4;
    IPO_LOCK = 8;
  { **************** KEY *********************  }
  { type  }
    KEY_NORMAL = 0;
    KEY_RELATIVE = 1;
  { keyblock->type  }
    KEY_LINEAR = 0;
    KEY_CARDINAL = 1;
    KEY_BSPLINE = 2;
  { **************** WORLD *********************  }
  { skytype  }
    WO_SKYBLEND = 1;
    WO_SKYREAL = 2;
    WO_SKYPAPER = 4;
  { tijdens render:  }
    WO_SKYTEX = 8;
    WO_ZENUP = 16;
  { mode  }
    WO_MIST = 1;
    WO_STARS = 2;
    WO_DOF = 4;
  { mapto  }
    WOMAP_BLEND = 1;
    WOMAP_HORIZ = 2;
    WOMAP_ZENUP = 4;
    WOMAP_ZENDOWN = 8;
  { **************** SCRIPTLINKS *********************  }
    SCRIPT_FRAMECHANGED = 1;
    SCRIPT_ONLOAD = 2;
  { **************** RENDERDATA *********************  }
  { bufflag  }
    R_BACKBUF = 1;
    R_BACKBUFANIM = 2;
    R_FRONTBUF = 4;
    R_FRONTBUFANIM = 8;
  { mode  }
    R_OSA = 1;
    R_SHADOW = 2;
    R_GAMMA = 4;
    R_ORTHO = 8;
    R_TRACE = 16;
    R_EDGE = 32;
    R_FIELDS = 64;
    R_FIELDSTILL = 128;
    R_GAUSS = 256;
    R_BORDER = 512;
    R_PANORAMA = 1024;
    R_MOVIECROP = 2048;
    R_COSMO = 4096;
  { deze verschillen tussen IrisGL en OpenGL!!!  }
    R_ODDFIELD = 8192;
    R_MBLUR = 16384;
  { scemode  }
    R_DOSEQ = 1;
    R_BG_RENDER = 2;
    R_NETWORK = 4;
    R_HOTSPOT = 8;
    R_EXTENSION = 16;
    R_OGL = 32;
  { alphamode  }
    R_ADDSKY = 0;
    R_ALPHAPREMUL = 1;
    R_ALPHAKEY = 2;
  { planes  }
    R_PLANES24 = 24;
    R_PLANES32 = 32;
    R_PLANESBW = 8;
  { imtype  }
    R_TARGA = 0;
    R_IRIS = 1;
    R_HAMX = 2;
    R_FTYPE = 3;
    R_JPEG90 = 4;
    R_MOVIE = 5;
    R_TANX = 6;
    R_IRIZ = 7;
    R_MDEC = 8;
    R_CMBB = 11;
    R_TGA4 = 12;
    R_HAMX4 = 13;
    R_RAWTGA = 14;
    R_AVIRAW = 15;
    R_AVIJPEG = 16;
    R_AVIJMF = 17;
  { **************** RENDER *********************  }
  { mode: zie RENDERDATA  }
  { flag  }
    R_ZTRA = 1;
    R_HALO = 2;
    R_SEC_FIELD = 4;
    R_LAMPHALO = 8;
    R_RENDERING = 16;
    R_ANIMRENDER = 32;
  { vlakren->flag  }
    R_SMOOTH = ME_SMOOTH;
    R_VISIBLE = 2;
    R_VERTEXCOL = ME_VERTEXCOL;
    R_NOPUNOFLIP = 8;
    R_CMAPCODE = 16;
  { vertren->texofs (texcoordinaten offset vanaf vertren->orco  }
    R_UVOFS3 = 1;
  { haloren->type: flags  }
    HA_ONLYSKY = 1;
    HA_VECT = 2;
    HA_XALPHA = 4;
    HA_FLARECIRC = 8;
  { DISPLAYMODE  }
    R_DISPLAYVIEW = 0;
    R_DISPLAYWIN = 1;
    R_DISPLAYAUTO = 2;
  { ***************** USERDEF ******************  }
  { flag  }
    AUTOSAVE = 1;
    AUTOGRABGRID = 2;
    AUTOROTGRID = 4;
    AUTOSIZEGRID = 8;
    SCENEGLOBAL = 16;
    TRACKBALL = 32;
    DUPLILINK = 64;
    FSCOLLUM = 128;
    MAT_ON_OB = 256;
    NO_CAPSLOCK = 512;
    VIEWMOVE = 1024;
    TOOLTIPS = 2048;
    TWOBUTTONMOUSE = 4096;
  { dupflag  }
    DUPMESH = 1;
    DUPCURVE = 2;
    DUPSURF = 4;
    DUPFONT = 8;
    DUPMBALL = 16;
    DUPLAMP = 32;
    DUPIPO = 64;
    DUPMAT = 128;
    DUPTEX = 256;
  { ***************** SEQUENCE ******************  }
  { seq->flag  }
    SEQ_LEFTSEL = 2;
    SEQ_RIGHTSEL = 4;
    SEQ_OVERLAP = 8;
    SEQ_FILTERY = 16;
  { seq->type LET OP BITJE 3!!!  }
    SEQ_IMAGE = 0;
    SEQ_META = 1;
    SEQ_SCENE = 2;
    SEQ_MOVIE = 3;
    SEQ_EFFECT = 8;
    SEQ_CROSS = 8;
    SEQ_ADD = 9;
    SEQ_SUB = 10;
    SEQ_ALPHAOVER = 11;
    SEQ_ALPHAUNDER = 12;
    SEQ_GAMCROSS = 13;
    SEQ_MUL = 14;
    SEQ_OVERDROP = 15;
    SEQ_PLUGIN = 24;

{$if defined(__sgi) or defined(__SUN) or defined (__PPC__)}

  const
    RCOMP = 3;
    GCOMP = 2;
    BCOMP = 1;
    ACOMP = 0;
  { ZR CHANGED THIS  }
{$else}
  {
  #endif

  #if defined(__FreeBSD__) || defined(linux) || defined(__BeOS) || defined(__WIN32)
   }
  { END BAD ZR  }

  const
    RCOMP = 0;
    GCOMP = 1;
    BCOMP = 2;
    ACOMP = 3;
{$endif}
{ $ifdef GS}
{ $undef GS}
{ $endif}
  function GS(a: pchar): integer; {#define GS(a)	(*((short *)(a))) }
{ $endif}

implementation

uses
  sysutils, blenglob;

  { was #define dname(params) para_def_expr }
  { argument types are unknown }
  { return type might be wrong }
  function usleep(x : longint) : longint;
  begin
    Sleep(x);
    usleep:= 0;
  end;

  { was #define dname def_expr }
  function MAXFLOAT : single;
    begin
      MAXFLOAT:=single(3.40282347e+38);
    end;

  procedure CLAMP(var a: smallint; b, c: smallint);
  begin
      if a<b then
        a:=b
      else
        if a>c then
        a:=c;
  end;

  function FIRSTBASE: pointer;
  begin
    exit(G.scene^.base.first);
  end;

  function CFRA: smallint;
  begin
    exit(G.scene^.r.cfra);
  end;

  function GS(a: pchar): integer;
  begin
    exit(psmallint(a)^);
  end;

    { was #define dname(params) para_def_expr }
    { argument types are unknown }
    { return type might be wrong }
    function MINSIZE(val,size : longint) : longint;
    var
       if_local1, if_local2, if_local3 : longint;
    (* result types are not known *)
    begin
      if val<size then
        if_local1:=size
      else
        if_local1:=val;
      if val>(-(size)) then
        if_local2:=-(size)
      else
        if_local2:=val;
      if val>=0.0 then
        if_local3:=if_local1
      else
        if_local3:=if_local2;
      MINSIZE:=if_local3;
    end;

//{dlsym(handle, sym) dlsym(handle, "_" sym)}
//function dlsym(handle: integer; sym: integer): integer; 
//begin
//  result:= dlsym(handle,'_'sym)
//end;
//
//{usleep(x) Sleep(x)}
//function usleep(x: integer): integer; 
//begin
//  result:= Sleep(x)
//end;
//
//{freeN(x) {freeN(x); x=NULL;}}
//function freeN(x: integer): integer; 
//begin
//  result:= (freeN(x);x:=0{nil};)
//end;
//
//{COREDUMP(n)   {static int times=0; if(times++ == n) abort();}}
//function COREDUMP(n: integer): integer; 
//begin
//  result:= (staticinttimes:=0;if(times{++} inc(times); =n)abort();)
//end;
//
//{VECCOPY(v1,v2)   {*(v1)= *(v2); *(v1+1)= *(v2+1); *(v1+2)= *(v2+2);}}
//function VECCOPY(v1: integer; v2: integer): integer; 
//begin
//  result:= (*(v1):=*(v2);*(v1+1):=*(v2+1);*(v1+2):=*(v2+2);)
//end;
//
//{QUATCOPY(v1,v2)  {*(v1)= *(v2); *(v1+1)= *(v2+1); *(v1+2)= *(v2+2); *(v1+3)= *(v2+3);}}
//function QUATCOPY(v1: integer; v2: integer): integer; 
//begin
//  result:= (*(v1):=*(v2);*(v1+1):=*(v2+1);*(v1+2):=*(v2+2);*(v1+3):=*(v2+3);)
//end;
//
//{INPR(v1, v2)  ( (v1)[0]*(v2)[0] + (v1)[1]*(v2)[1] + (v1)[2]*(v2)[2] )}
//function INPR(v1: integer; v2: integer): integer; 
//begin
//  result:= ((v1)[0]*(v2)[0]+(v1)[1]*(v2)[1]+(v1)[2]*(v2)[2])
//end;
//
//{CLAMP(a, b, c)  if((a)<(b)) (a)=(b); else if((a)>(c)) (a)=(c)}
//function CLAMP(a: integer; b: integer; c: integer): integer; 
//begin
//  result:= if((a)<(b))(a):=(b)
//end;
//
//{CLAMPIS(a, b, c) ((a)<(b) ? (b) : (a)>(c) ? (c) : (a))}
//function CLAMPIS(a: integer; b: integer; c: integer): integer; 
//begin
//  result:= ((a)<(b) {was ?}if then (b) {was :}else (a)>(c) {was ?}if then (c) {was :}else (a))
//end;
//
//{CLAMPTEST(a, b, c) if((b)<(c)) {CLAMP(a, b, c);} else {CLAMP(a, c, b);}}
//function CLAMPTEST(a: integer; b: integer; c: integer): integer; 
//begin
//  result:= if((b)<(c))(CLAMP(a,b,c);)else(CLAMP(a,c,b);)
//end;
//
//{IS_EQ(a,b) ((fabs((double)(a)-(b)) >= (double) FLT_EPSILON) ? 0 : 1)}
//function IS_EQ(a: integer; b: integer): integer; 
//begin
//  result:= ((fabs( {double(}(a)-(b))>= {double(}FLT_EPSILON) {was ?}if then 0 {was :}else 1)
//end;
//
//{INIT_MINMAX(min, max) (min)[0]= (min)[1]= (min)[2]= 1.0e30; (max)[0]= (max)[1]= (max)[2]= -1.0e30;}
//function INIT_MINMAX(min: integer; max: integer): integer; 
//begin
//  result:= (min)[0]:=(min)[1]:=(min)[2]:=1.0e30
//end;
//
//{DO_MINMAX(vec, min, max) if( (min)[0]>(vec)[0] ) (min)[0]= (vec)[0];          if( (min)[1]>(vec)[1] ) (min)[1]= (vec)[1];          if( (min)[2]>(vec)[2] ) (min)[2]= (vec)[2];          if( (max)[0]<(vec)[0] ) (max)[0]= (vec)[0];          if( (max)[1]<(vec)[1] ) (max)[1]= (vec)[1];          if( (max)[2]<(vec)[2] ) (max)[2]= (vec)[2]; #define DO_MINMAX2(vec, min, max) if( (min)[0]>(vec)[0] ) (min)[0]= (vec)[0];          if( (min)[1]>(vec)[1] ) (min)[1]= (vec)[1];          if( (max)[0]<(vec)[0] ) (max)[0]= (vec)[0];          if( (max)[1]<(vec)[1] ) (max)[1]= (vec)[1];}
//function DO_MINMAX(vec: integer; min: integer; max: integer): integer; 
//begin
//  result:= if((min)[0]>(vec)[0])(min)[0]:=(vec)[0]
//end;
//
//{MINSIZE(val, size) ( ((val)>=0.0) ? (((val)<(size)) ? (size): (val)) : ( ((val)>(-size)) ? (-size) : (val)))}
//function MINSIZE(val: integer; size: integer): integer; 
//begin
//  result:= (((val)>=0.0) {was ?}if then (((val)<(size)) {was ?}if then (size) {was :}else (val)) {was :}else (((val)>(-size)) {was ?}if then (-size) {was :}else (val)))
//end;
//
//{BTST(a,b) ( ( (a) & 1<<(b) )!=0 )}
//function BTST(a: integer; b: integer): integer; 
//begin
//  result:= (((a) and 1 shl (b))<>0)
//end;
//
//{BCLR(a,b) ( (a) & ~(1<<(b)) )}
//function BCLR(a: integer; b: integer): integer; 
//begin
//  result:= ((a) and  not (1 shl (b)))
//end;
//
//{BSET(a,b) ( (a) | 1<<(b) )}
//function BSET(a: integer; b: integer): integer; 
//begin
//  result:= ((a) or 1 shl (b))
//end;
//
//{BROW(min, max) (((max)>=31? 0xFFFFFFFF: (1<<(max+1))-1) - ((min)? ((1<<(min))-1):0) )}
//function BROW(min: integer; max: integer): integer; 
//begin
//  result:= (((max)>=31 {was ?}if then $FFFFFFFF {was :}else (1 shl (max+1))-1)-((min) {was ?}if then ((1 shl (min))-1) {was :}else 0))
//end;
//
//{COPY_4(a,b)  *((int * )(a))= *((int * )(b))}
//function COPY_4(a: integer; b: integer): integer; 
//begin
//  result:= *( {pinteger(}(a)):=*( {pinteger(}(b))
//end;
//
//{COPY_8(a,b)  *((byte8 * )(a))= *((byte8 * )(b))}
//function COPY_8(a: integer; b: integer): integer; 
//begin
//  result:= *( {pbyte8(}(a)):=*( {pbyte8(}(b))
//end;
//
//{COPY_12(a,b) *((byte12 * )(a))= *((byte12 * )(b))}
//function COPY_12(a: integer; b: integer): integer; 
//begin
//  result:= *( {pbyte12(}(a)):=*( {pbyte12(}(b))
//end;
//
//{COPY_16(a,b) *((byte16 * )(a))= *((byte16 * )(b))}
//function COPY_16(a: integer; b: integer): integer; 
//begin
//  result:= *( {pbyte16(}(a)):=*( {pbyte16(}(b))
//end;
//
//{LONGCOPY(a, b, c) {int lcpc=c, *lcpa=(int * )a, *lcpb=(int * )b; while(lcpc-->0) *(lcpa++)= *(lcpb++);}}
//function LONGCOPY(a: integer; b: integer; c: integer): integer; 
//begin
//  result:= (intlcpc:=c,*lcpa:= {pinteger(}a,*lcpb:= {pinteger(}b;while(lcpc{--} dec(lcpc); >0)*(lcpa{++} inc(lcpa); ):=*(lcpb{++} inc(lcpb); );)
//end;
//
//{SWITCH_LONGINT(a) {char s_i, *p_i; p_i= (char * )&(a);         s_i= p_i[0]; p_i[0]= p_i[7]; p_i[7]= s_i; s_i= p_i[1]; p_i[1]= p_i[6]; p_i[6]= s_i;         s_i= p_i[2]; p_i[2]= p_i[5]; p_i[5]= s_i; s_i= p_i[3]; p_i[3]= p_i[4]; p_i[4]= s_i; }}
//function SWITCH_LONGINT(a: integer): integer; 
//begin
//  result:= (chars_i,*p_i;p_i:=(char* )@(a);s_i:=p_i[0];p_i[0]:=p_i[7];p_i[7]:=s_i;s_i:=p_i[1];p_i[1]:=p_i[6];p_i[6]:=s_i;s_i:=p_i[2];p_i[2]:=p_i[5];p_i[5]:=s_i;s_i:=p_i[3];p_i[3]:=p_i[4];p_i[4]:=s_i;)
//end;
//
//{SWITCH_INT(a) {char s_i, *p_i; p_i= (char * )&(a); s_i= p_i[0]; p_i[0]= p_i[3]; p_i[3]= s_i; s_i= p_i[1]; p_i[1]= p_i[2]; p_i[2]= s_i; }}
//function SWITCH_INT(a: integer): integer; 
//begin
//  result:= (chars_i,*p_i;p_i:=(char* )@(a);s_i:=p_i[0];p_i[0]:=p_i[3];p_i[3]:=s_i;s_i:=p_i[1];p_i[1]:=p_i[2];p_i[2]:=s_i;)
//end;
//
//{SWITCH_SHORT(a) {char s_i, *p_i; p_i= (char * )&(a); s_i= p_i[0]; p_i[0]= p_i[1]; p_i[1]= s_i; }}
//function SWITCH_SHORT(a: integer): integer; 
//begin
//  result:= (chars_i,*p_i;p_i:=(char* )@(a);s_i:=p_i[0];p_i[0]:=p_i[1];p_i[1]:=s_i;)
//end;

{MAKE_ID2(c, d)  ( (c)<<8 | (d) )}
function MAKE_ID2(c, d: pchar): integer;
begin
  {$IFDEF ENDIAN_BIG}
  result:= ((c) shl 8 or (d))
{$ELSE}
result:= (byte(d^) shl 8 or byte(c^))
{$ENDIF}
end;

{ #define ID_SCE	MAKE_ID2('S', 'C') }
{ #define ID_LI	MAKE_ID2('L', 'I') }
{ #define ID_OB	MAKE_ID2('O', 'B') }
{ #define ID_ME	MAKE_ID2('M', 'E') }
{ #define ID_CU	MAKE_ID2('C', 'U') }
{ #define ID_MB	MAKE_ID2('M', 'B') }
{ #define ID_MA	MAKE_ID2('M', 'A') }
{ #define ID_TE	MAKE_ID2('T', 'E') }
{ #define ID_IM	MAKE_ID2('I', 'M') }
{ #define ID_IK	MAKE_ID2('I', 'K') }
{ #define ID_WV	MAKE_ID2('W', 'V') }
{ #define ID_LT	MAKE_ID2('L', 'T') }
{ #define ID_SE	MAKE_ID2('S', 'E') }
{ #define ID_LF	MAKE_ID2('L', 'F') }
{ #define ID_LA	MAKE_ID2('L', 'A') }
{ #define ID_CA	MAKE_ID2('C', 'A') }
{ #define ID_IP	MAKE_ID2('I', 'P') }
{ #define ID_KE	MAKE_ID2('K', 'E') }
{ #define ID_WO	MAKE_ID2('W', 'O') }
//function ID_SCR: integer; begin MAKE_ID2('S', 'R'); end;
{ #define ID_VF	MAKE_ID2('V', 'F') }
{ #define ID_TXT	MAKE_ID2('T', 'X') }
{ #define ID_ID	MAKE_ID2('I', 'D') }
{ #define ID_SEQ	MAKE_ID2('S', 'Q') }

//{TESTBASE(base) ( ((base)->flag & SELECT) && ((base)->lay & G.vd->lay) )}
//function TESTBASE(base: integer): integer; 
//begin
//  result:= (((base).flag and SELECT))and(((base).lay and G.vd.lay))
//end;
//
//{TESTBASELIB(base) ( ((base)->flag & SELECT) && ((base)->lay & G.vd->lay) && ((base)->object->id.lib==0))}
//function TESTBASELIB(base: integer): integer; 
//begin
//  result:= (((base).flag and SELECT))and(((base).lay and G.vd.lay))and(((base).object.id.lib=0))
//end;
//
//{ID_NEW(a)  if( (a) && (a)->id._new ) (a)= (void * )(a)->id._new}
//function ID_NEW(a: integer): integer; 
//begin
//  result:= if((a))and((a).id._new)(a):= {pinteger(}(a).id._new
//end;
//
//{ID_NEW_US(a) if( (a)->id._new) {(a)= (void * )(a)->id._new; (a)->id.us++;}}
//function ID_NEW_US(a: integer): integer; 
//begin
//  result:= if((a).id._new)((a):= {pinteger(}(a).id._new;(a).id.us{++} inc(id.us); ;)
//end;
//
//{ID_NEW_US2(a) if( ((ID * )a)->new) {(a)= ((ID * )a)->new; ((ID * )a)->us++;}}
//function ID_NEW_US2(a: integer): integer; 
//begin
//  result:= if(( {pID(}a)._new)((a):=( {pID(}a)._new;( {pID(}a).us{++} inc(us); ;)
//end;
//
//{ISPOIN(a, b, c)   ( (a->b) && (a->c) )}
//function ISPOIN(a: integer; b: integer; c: integer): integer; 
//begin
//  result:= ((a.b))and((a.c))
//end;
//
//{ISPOIN3(a, b, c, d)  ( (a->b) && (a->c) && (a->d) )}
//function ISPOIN3(a: integer; b: integer; c: integer; d: integer): integer; 
//begin
//  result:= ((a.b))and((a.c))and((a.d))
//end;
//
//{ISPOIN4(a, b, c, d, e) ( (a->b) && (a->c) && (a->d) && (a->e) )}
//function ISPOIN4(a: integer; b: integer; c: integer; d: integer; e: integer): integer; 
//begin
//  result:= ((a.b))and((a.c))and((a.d))and((a.e))
//end;
//
//{KNOTSU(nu)     ( (nu)->orderu+ (nu)->pntsu+ (nu->orderu-1)*((nu)->flagu & 1) )}
//function KNOTSU(nu: integer): integer; 
//begin
//  result:= ((nu).orderu+(nu).pntsu+(nu.orderu-1)*((nu).flagu and 1))
//end;
//
//{KNOTSV(nu)     ( (nu)->orderv+ (nu)->pntsv+ (nu->orderv-1)*((nu)->flagv & 1) )}
//function KNOTSV(nu: integer): integer; 
//begin
//  result:= ((nu).orderv+(nu).pntsv+(nu.orderv-1)*((nu).flagv and 1))
//end;
//
//{DL_SURFINDEX(cyclu, cyclv, sizeu, sizev)                    if( (cyclv)==0 && a==(sizev)-1) break;          if(cyclu) {           p1= sizeu*a;          p2= p1+ sizeu-1;         p3= p1+ sizeu;          p4= p2+ sizeu;          b= 0;              }               else {           p2= sizeu*a;          p1= p2+1;          p4= p2+ sizeu;          p3= p1+ sizeu;          b= 1;              }               if( (cyclv) && a==sizev-1) {        p3-= sizeu*sizev;         p4-= sizeu*sizev;            }}
//function DL_SURFINDEX(cyclu: integer; cyclv: integer; sizeu: integer; sizev: integer): integer; 
//begin
//  result:= if((cyclv)=0)and(a=(sizev)-1)break
//end;
//
//{DYNA_REF(ma)  (ma->mirr)}
//function DYNA_REF(ma: integer): integer; 
//begin
//  result:= (ma.mirr)
//end;
//
//{DYNA_FH_DIST(ma) (ma->mirg)}
//function DYNA_FH_DIST(ma: integer): integer; 
//begin
//  result:= (ma.mirg)
//end;
//
//{DYNA_FH_INT(ma)  (ma->mirb)}
//function DYNA_FH_INT(ma: integer): integer; 
//begin
//  result:= (ma.mirb)
//end;
//
//{DYNA_FH_FRICT(ma) (ma->specr)}
//function DYNA_FH_FRICT(ma: integer): integer; 
//begin
//  result:= (ma.specr)
//end;
//
//{DYNA_FH_XYFRICT(ma) (ma->emit)}
//function DYNA_FH_XYFRICT(ma: integer): integer; 
//begin
//  result:= (ma.emit)
//end;
//
//{DYNA_LAY(ma)  (ma->}
//function DYNA_LAY(ma: integer): integer; 
//begin
//  result:= (ma.
//end;
//
//{GS(a) ( *((short * )(a)))}
//function GS(a: integer): integer; 
//begin
//  result:= (*( {psmallint(}(a)))
//end;
//
end.
