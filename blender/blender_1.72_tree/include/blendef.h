/**
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
 */

#ifndef BLENDEF_H

#define BLENDEF_H

#ifdef __WIN32
#else
#ifndef __BeOS
#define O_BINARY	0
#endif
#endif

#ifdef __FreeBSD__
#define dlsym(handle, sym) dlsym(handle, "_" sym)
#endif

#ifdef __WIN32
#define usleep(x)	Sleep(x)
#define TIME_INTERVAL 1000.0
#else
#define TIME_INTERVAL 100.0
#endif

#ifdef ZRS_DEFINES
#define freeN(x) {freeN(x); x=NULL;}
#endif

#ifndef MAXFLOAT
#define MAXFLOAT  ((float)3.40282347e+38)
#endif

/* blendef.h	alle defines voor blenderstructuur
 * 
 * jan 95 maart 95
 * 
 */

#include <float.h>	/* deze moet een keer naar de blender.h */
#include "code.h"

/* **************** ALGEMEEN ********************* */

#define COREDUMP(n)			{static int times=0; if(times++ == n) abort();}
#define VECCOPY(v1,v2) 		{*(v1)= *(v2); *(v1+1)= *(v2+1); *(v1+2)= *(v2+2);}
#define QUATCOPY(v1,v2) 	{*(v1)= *(v2); *(v1+1)= *(v2+1); *(v1+2)= *(v2+2); *(v1+3)= *(v2+3);}

#define INPR(v1, v2)		( (v1)[0]*(v2)[0] + (v1)[1]*(v2)[1] + (v1)[2]*(v2)[2] )
#define CLAMP(a, b, c)		if((a)<(b)) (a)=(b); else if((a)>(c)) (a)=(c)
#define CLAMPIS(a, b, c)	((a)<(b) ? (b) : (a)>(c) ? (c) : (a))
#define CLAMPTEST(a, b, c)	if((b)<(c)) {CLAMP(a, b, c);} else {CLAMP(a, c, b);}

#define IS_EQ(a,b) ((fabs((double)(a)-(b)) >= (double) FLT_EPSILON) ? 0 : 1)

#define INIT_MINMAX(min, max) (min)[0]= (min)[1]= (min)[2]= 1.0e30; (max)[0]= (max)[1]= (max)[2]= -1.0e30;
#define DO_MINMAX(vec, min, max) if( (min)[0]>(vec)[0] ) (min)[0]= (vec)[0]; \
							  if( (min)[1]>(vec)[1] ) (min)[1]= (vec)[1]; \
							  if( (min)[2]>(vec)[2] ) (min)[2]= (vec)[2]; \
							  if( (max)[0]<(vec)[0] ) (max)[0]= (vec)[0]; \
							  if( (max)[1]<(vec)[1] ) (max)[1]= (vec)[1]; \
							  if( (max)[2]<(vec)[2] ) (max)[2]= (vec)[2]; \

#define DO_MINMAX2(vec, min, max) if( (min)[0]>(vec)[0] ) (min)[0]= (vec)[0]; \
							  if( (min)[1]>(vec)[1] ) (min)[1]= (vec)[1]; \
							  if( (max)[0]<(vec)[0] ) (max)[0]= (vec)[0]; \
							  if( (max)[1]<(vec)[1] ) (max)[1]= (vec)[1];

#define MINSIZE(val, size)	( ((val)>=0.0) ? (((val)<(size)) ? (size): (val)) : ( ((val)>(-size)) ? (-size) : (val)))

#define BTST(a,b)	( ( (a) & 1<<(b) )!=0 )
#define BCLR(a,b)	( (a) & ~(1<<(b)) )
#define BSET(a,b)	( (a) | 1<<(b) )
/* bit-row */
#define BROW(min, max)	(((max)>=31? 0xFFFFFFFF: (1<<(max+1))-1) - ((min)? ((1<<(min))-1):0) )

typedef struct { 
	int a, b, c, d;
} byte16;

#define COPY_4(a,b)		*((int *)(a))= *((int *)(b))
#define COPY_8(a,b)		*((byte8 *)(a))= *((byte8 *)(b))
#define COPY_12(a,b)	*((byte12 *)(a))= *((byte12 *)(b))
#define COPY_16(a,b)	*((byte16 *)(a))= *((byte16 *)(b))

#define LONGCOPY(a, b, c)	{int lcpc=c, *lcpa=(int *)a, *lcpb=(int *)b; while(lcpc-->0) *(lcpa++)= *(lcpb++);}

#define SWITCH_LONGINT(a)	{char s_i, *p_i; p_i= (char *)&(a); \
								s_i= p_i[0]; p_i[0]= p_i[7]; p_i[7]= s_i; s_i= p_i[1]; p_i[1]= p_i[6]; p_i[6]= s_i; \
								s_i= p_i[2]; p_i[2]= p_i[5]; p_i[5]= s_i; s_i= p_i[3]; p_i[3]= p_i[4]; p_i[4]= s_i; }
#define SWITCH_INT(a)	{char s_i, *p_i; p_i= (char *)&(a); s_i= p_i[0]; p_i[0]= p_i[3]; p_i[3]= s_i; s_i= p_i[1]; p_i[1]= p_i[2]; p_i[2]= s_i; }
#define SWITCH_SHORT(a)	{char s_i, *p_i; p_i= (char *)&(a); s_i= p_i[0]; p_i[0]= p_i[1]; p_i[1]= s_i; }


#if defined(__sgi) || defined(__SUN) || defined (__PPC__)
/* big endian */
#define MAKE_ID2(c, d)		( (c)<<8 | (d) )
#define MOST_SIG_BYTE				0
#define BBIG_ENDIAN
#else
/* little endian  */
#define MAKE_ID2(c, d)		( (d)<<8 | (c) )
#define MOST_SIG_BYTE				1
#define BLITTLE_ENDIAN
#endif

#define SELECT			1
#define ACTIVE			2
#define NOT_YET			0


#define TESTBASE(base)	( ((base)->flag & SELECT) && ((base)->lay & G.vd->lay) )
#define TESTBASELIB(base)	( ((base)->flag & SELECT) && ((base)->lay & G.vd->lay) && ((base)->object->id.lib==0))
#define FIRSTBASE		G.scene->base.first
#define LASTBASE		G.scene->base.last
#define BASACT			(G.scene->basact)
#define OBACT			(BASACT? BASACT->object: 0)
#define ID_NEW(a)		if( (a) && (a)->id.new ) (a)= (void *)(a)->id.new
#define ID_NEW_US(a)	if( (a)->id.new) {(a)= (void *)(a)->id.new; (a)->id.us++;}
#define ID_NEW_US2(a)	if( ((ID *)a)->new) {(a)= ((ID *)a)->new; ((ID *)a)->us++;}
#define	CFRA			(G.scene->r.cfra)
#define	F_CFRA			((float)(G.scene->r.cfra))
#define	SFRA			(G.scene->r.sfra)
#define	EFRA			(G.scene->r.efra)

#define ISPOIN(a, b, c)			( (a->b) && (a->c) )
#define ISPOIN3(a, b, c, d)		( (a->b) && (a->c) && (a->d) )
#define ISPOIN4(a, b, c, d, e)	( (a->b) && (a->c) && (a->d) && (a->e) )


#define KNOTSU(nu)	    ( (nu)->orderu+ (nu)->pntsu+ (nu->orderu-1)*((nu)->flagu & 1) )
#define KNOTSV(nu)	    ( (nu)->orderv+ (nu)->pntsv+ (nu->orderv-1)*((nu)->flagv & 1) )

/* psfont */
#define FNT_PDRAW 1
#define FNT_HAEBERLI 2


/* isect en scanfill */
#define COMPLIMIT	0.0003


/* **************** MAX ********************* */


#define MAXLAMP		256
/* max lengte material array, 16 vanwege bitjes in matfrom */
#define MAXMAT		16
#define MAXPICKBUF	2000
#define MAXSEQ		32
/*  in Image struct */
#define MAXMIPMAP	10
/*  in ColorBand struct */
#define MAXCOLORBAND 16
/* in buttons.c */
#define MAX_EFFECT	20

/* **************** GLOBAL ********************* */

/* G.f */
#define G_DISABLE_OK	1
#define G_PLAYANIM		2
#define G_TEST_DUPLI	4
#define G_SIMULATION	8
#define G_BACKBUFSEL	16
#define G_PICKSEL		32
#define G_DRAWNORMALS	64
#define G_DRAWFACES		128
#define G_FACESELECT	256
#define G_DRAW_EXT		512
#define G_VERTEXPAINT	1024
#define G_ALLEDGES		2048
#define G_DEBUG			4096
#define G_SCENESCRIPT	8192
#define G_PROPORTIONAL	16384

/* G.simulf */
#define G_LOADFILE	2
#define G_RESTART	4
#define G_QUIT		8
#define G_NETWORK	16

/* G.qual */
#define R_SHIFTKEY	1
#define L_SHIFTKEY	2
#define LR_SHIFTKEY 3
#define R_ALTKEY	4
#define L_ALTKEY	8
#define LR_ALTKEY	12
#define R_CTRLKEY	16
#define L_CTRLKEY	32
#define LR_CTRLKEY  48

/* getbutton */
#define L_MOUSE	1
#define M_MOUSE 2
#define R_MOUSE	4

/* G.order */
#define L_ENDIAN	1
#define B_ENDIAN	0

/* G.special1 */
#define G_HOLO		1


/* do_global_buttons(event) */

/* VIEW3D: 100 */
#define B_HOME			101
#define B_LAY			102
/* pasop: codes 102-132 in gebuik voor layers */
#define B_SCENELOCK		140
#define B_LOCALVIEW		141
#define B_U_CAPSLOCK	142
#define B_EDITMODE		143
#define B_VPAINT		144
#define B_FACESEL		145
#define B_VIEWBUT		146
#define B_PERSP			147
#define B_PROPTOOL		148
#define B_VIEWRENDER	149

/* IPO: 200 */
#define B_IPOHOME		201
#define B_IPOBORDER		202
#define B_IPOCOPY		203
#define B_IPOPASTE		204
#define B_IPOCONT		205
#define B_IPOEXTRAP		206
#define B_IPOCYCLIC		207
#define B_IPOMAIN		208
#define B_IPOSHOWKEY	209
#define B_IPOCYCLICX	210

/* OOPS: 250 */
#define B_OOPSHOME		251
#define B_OOPSBORDER	252
#define B_NEWOOPS		253

/* INFO: 300 */
/* pas op: ook in filesel.c en editobject.c */
#define B_INFOSCR		301
#define B_INFODELSCR	302
#define B_INFOSCE		304
#define B_INFODELSCE	305
#define B_FILEMENU		306

/* IMAGE: 350 */
#define B_SIMAGEHOME	351
#define B_SIMABROWSE	352
#define B_SIMAGELOAD	353
#define B_SIMAGEDRAW	354
#define B_BE_SQUARE		355
#define B_SIMAGEDRAW1	356
#define B_TWINANIM		357
#define B_SIMAGEREPLACE	358

/* BUTS: 400 */
#define B_BUTSHOME		401
#define B_BUTSPREVIEW	402
#define B_MATCOPY		403
#define B_MATPASTE		404
#define B_MESHTYPE		405

/* IMASEL: 450 */
/* in de imasel.h */

/* BPAINT: 500 */
/* in de bpaint.h */

/* TEXT: 500 */
#define B_TEXTBROWSE	501
#define B_TEXTALONE		502
#define B_TEXTLOCAL		503
#define B_TEXTDELETE	504
#define B_TEXTFONT		505
#define B_TEXTSTORE		506

/* FILE: 550 */
#define B_SORTFILELIST	551
#define B_RELOADDIR		552

/* SEQUENCE: 600 */
#define B_SEQHOME		601
#define B_SEQCLEAR		602




#define B_NOTHING		1

#define B_WORLDLOCAL	28
#define B_WORLDALONE	29
#define B_LATTLOCAL		30
#define B_MBALLLOCAL	31
#define B_CAMERALOCAL	32
#define B_OBLOCAL		33
#define B_IPOLOCAL		34
#define B_LAMPLOCAL		35
#define B_MATLOCAL		36
#define B_TEXLOCAL		37
#define B_MESHLOCAL		38
#define B_CURVELOCAL	39

#define B_LATTALONE		40
#define B_MBALLALONE	41
#define B_CAMERAALONE	42
#define B_OBALONE		43
#define B_IPOALONE		44
#define B_LAMPALONE		45
#define B_MATALONE		46
#define B_TEXALONE		47
#define B_MESHALONE		48
#define B_CURVEALONE	49
/* EVENT < 50: alone's en locals */


#define B_LTEXBROWSE	78
#define B_MESHBROWSE	79
#define B_EXTEXBROWSE	80
#define B_LOADTEMP		81
#define B_MATDELETE		82
#define B_TEXDELETE		83
#define B_IPODELETE		84
#define B_WORLDDELETE	85
#define B_WTEXBROWSE	86
#define B_WORLDBROWSE	87
#define B_IPOBROWSE		88
#define B_NEWFRAME		89
#define B_LAMPBROWSE	90
#define B_MATBROWSE		91
#define	B_TEXBROWSE		92
#define	B_EDITBROWSE	93
#define B_AUTOTEXNAME	94
#define B_AUTOMATNAME	95
#define B_MATLOCK		96
#define B_IDNAME		97
#define B_NEWSPACE		98
#define B_FULL			99
#define B_REDR			100

/* editbutflag */
#define B_CLOCKWISE		1
#define B_KEEPORIG		2
#define B_BEAUTY		4
#define B_SMOOTH		8


/* **************** SPACE EVENT CODES ***************  */

#define REMAKEIPO		1
#define OOPS_TEST		2





/* **************** QUEUE EVENTS ********************* */

#define CHANGED				0x4000
#define DRAWEDGES			0x4001
#define AFTERQUEUE			0x4002
#define BACKBUFDRAW			0x4003
#define EXECUTE				0x4004
#define IGNORE_REDRAW		0x4005
#define LOAD_FILE			0x4006
#define RESHAPE				0x4007

/* REDRAWVIEW3D moet de eerste zijn (laagste nummer) voor buttons! */
#define REDRAWVIEW3D		0x4010

#define REDRAWBUTSHEAD		0x4011
#define REDRAWBUTSALL		0x4012
#define REDRAWBUTSVIEW		0x4013
#define REDRAWBUTSLAMP		0x4014
#define REDRAWBUTSMAT		0x4015
#define REDRAWBUTSTEX		0x4016
#define REDRAWBUTSANIM		0x4017
#define REDRAWBUTSWORLD		0x4018
#define REDRAWBUTSRENDER	0x4019
#define REDRAWBUTSEDIT		0x401A
#define REDRAWVIEWCAM		0x401B
#define REDRAWHEADERS		0x401C
#define REDRAWBUTSGAME		0x401D
#define REDRAWBUTSRADIO		0x401E
#define REDRAWVIEW3D_Z		0x401F

#define REDRAWALL			0x4020
#define REDRAWINFO			0x4021
#define RENDERPREVIEW		0x4022
#define REDRAWIPO			0x4023
#define REDRAWDATASELECT	0x4024
#define REDRAWSEQ			0x4025
#define REDRAWIMAGE			0x4026
#define REDRAWOOPS			0x4027
#define REDRAWIMASEL        0x4028
#define AFTERIMASELIMA      0x4029
#define AFTERIMASELGET      0x402A
#define AFTERIMAWRITE       0x402B
#define IMALEFTMOUSE		0x402C
#define AFTERPIBREAD        0x402D
#define REDRAWPAINT	        0x402E
#define REDRAWTEXT	        0x402F
#define REDRAWBUTSSCRIPT	0x4030

/* ***************** DISPLIST ***************** */

#define DL_POLY			0
#define DL_SEGM			1
#define DL_SURF			2
#define DL_TRIA			3
#define DL_INDEX3		4
#define DL_INDEX4		5
#define DL_VERTCOL		6
#define DL_VERTS		7
#define DL_NORS			8

#define DL_SURFINDEX(cyclu, cyclv, sizeu, sizev)	    \
							    \
    if( (cyclv)==0 && a==(sizev)-1) break;		    \
    if(cyclu) {						    \
	p1= sizeu*a;					    \
	p2= p1+ sizeu-1;				    \
	p3= p1+ sizeu;					    \
	p4= p2+ sizeu;					    \
	b= 0;						    \
    }							    \
    else {						    \
	p2= sizeu*a;					    \
	p1= p2+1;					    \
	p4= p2+ sizeu;					    \
	p3= p1+ sizeu;					    \
	b= 1;						    \
    }							    \
    if( (cyclv) && a==sizev-1) {			    \
	p3-= sizeu*sizev;				    \
	p4-= sizeu*sizev;				    \
    }

/* **************** SPACE ********************* */

/* view3d->flag */
#define V3D_DISPIMAGE		1
#define V3D_DISPBGPIC		2
#define V3D_SETUPBUTS		4
#define V3D_NEEDBACKBUFDRAW	8
#define V3D_MODE			(16+32+64)
#define V3D_EDITMODE		16
#define V3D_VERTEXPAINT		32
#define V3D_FACESELECT		64

/* view3d->around */
#define V3D_CENTRE		0
#define V3D_CENTROID	3
#define V3D_CURSOR		1
#define V3D_LOCAL		2


#define BUTS_VIEW			0
#define BUTS_LAMP			1
#define BUTS_MAT			2
#define BUTS_TEX			3
#define BUTS_ANIM			4
#define BUTS_WORLD			5
#define BUTS_RENDER			6
#define BUTS_EDIT			7
#define BUTS_GAME			8
#define BUTS_FPAINT			9
#define BUTS_RADIO			10
#define BUTS_SCRIPT			11

/* deze getallen ook invullen in blender.h SpaceFile: struct dna herkent geen defines */
#define FILE_MAXDIR			160
#define FILE_MAXFILE		80

/* filesel types */
#define FILE_UNIX			8
#define FILE_BLENDER		8
#define FILE_SPECIAL		9

#define FILE_LOADLIB		1
#define FILE_MAIN			2

/* sfile->flag */
#define FILE_SHOWSHORT		1
#define FILE_STRINGCODE		2
#define FILE_LINK			4
#define FILE_HIDE_DOT		8

/* sfile->sort */
#define FILE_SORTALPHA		0
#define FILE_SORTDATE		1
#define FILE_SORTSIZE		2
#define FILE_SORTEXTENS		3

/* files in filesel list: 2=ACTIVE  */
#define HILITE				1
#define BLENDERFILE			4
#define PSXFILE				8
#define IMAGEFILE			16
#define MOVIEFILE			32

#define SCROLLH	16			/* hoogte scrollbar */
#define SCROLLB	16			/* breedte scrollbar */

/* SpaceImage->mode */
#define SI_TEXTURE		0
#define SI_SHOW			1

/* SpaceImage->flag */
#define SI_BE_SQUARE	1
#define SI_EDITTILE		2

/* SpaceOops->flag */
#define SO_TESTBLOCKS	1
#define SO_NEWSELECTED	2

/* SpaceOops->visiflag */
#define OOPS_SCE	1
#define OOPS_OB		2
#define OOPS_ME		4
#define OOPS_CU		8
#define OOPS_MB		16
#define OOPS_LT		32
#define OOPS_LA		64
#define OOPS_MA		128
#define OOPS_TE		256
#define OOPS_IP		512
#define OOPS_LAY	1024
#define OOPS_LI		2048
#define OOPS_IM		4096


/* **************** MAIN ********************* */
/* id->flag: eerste 8 bits altijd op nul zetten bij inlezen */
#define LIB_LOCAL		0
#define LIB_EXTERN		1
#define LIB_INDIRECT	2
#define LIB_TEST		8
#define LIB_TESTEXT		9
#define LIB_TESTIND		10
#define LIB_READ		16
#define LIB_NEEDLINK	32

#define LIB_NEW			256
#define LIB_FAKEUSER	512
/* vrije testflag */
#define LIB_DOIT		1024

/* ID */
#define ID_SCE	MAKE_ID2('S', 'C')
#define ID_LI	MAKE_ID2('L', 'I')
#define ID_OB	MAKE_ID2('O', 'B')
#define ID_ME	MAKE_ID2('M', 'E')
#define ID_CU	MAKE_ID2('C', 'U')
#define ID_MB	MAKE_ID2('M', 'B')
#define ID_MA	MAKE_ID2('M', 'A')
#define ID_TE	MAKE_ID2('T', 'E')
#define ID_IM	MAKE_ID2('I', 'M')
#define ID_IK	MAKE_ID2('I', 'K')
#define ID_WV	MAKE_ID2('W', 'V')
#define ID_LT	MAKE_ID2('L', 'T')
#define ID_SE	MAKE_ID2('S', 'E')
#define ID_LF	MAKE_ID2('L', 'F')
#define ID_LA	MAKE_ID2('L', 'A')
#define ID_CA	MAKE_ID2('C', 'A')
#define ID_IP	MAKE_ID2('I', 'P')
#define ID_KE	MAKE_ID2('K', 'E')
#define ID_WO	MAKE_ID2('W', 'O')
#define ID_SCR	MAKE_ID2('S', 'R')
#define ID_VF	MAKE_ID2('V', 'F')
#define ID_TXT	MAKE_ID2('T', 'X')

#define ID_ID	MAKE_ID2('I', 'D')

#define ID_SEQ	MAKE_ID2('S', 'Q')


/* **************** SCENE ********************* */
#define RAD_PHASE_PATCHES	1
#define RAD_PHASE_FACES		2

/* base->flag en ob->flag */
#define BA_WASSEL			2
#define BA_PARSEL			4
#define BA_WHERE_UPDATE		8
#define BA_DISP_UPDATE		16
#define BA_DO_IPO			32
#define BA_FROMSET			128
#define OB_DO_IMAT			256
#define OB_FROMDUPLI		512
#define OB_DONE				1024
#define OB_RADIO			2048

/* sce->flag */
#define SCE_ADDSCENAME		1

/* return flag next_object function */
#define F_START			0
#define F_SCENE			1
#define F_SET			2
#define F_DUPLI			3

/* **************** OBJECT ********************* */

/* type */
#define OB_EMPTY		0
#define OB_MESH			1
#define OB_CURVE		2
#define OB_SURF			3
#define OB_FONT			4
#define OB_MBALL		5

#define OB_LAMP			10
#define OB_CAMERA		11

#define OB_IKA			20
#define OB_WAVE			21
#define OB_LATTICE		22
#define OB_SECTOR		23
#define OB_LIFE			24

/* partype: eerste 4 bits: type */
#define PARTYPE			15
#define PAROBJECT		0
#define PARCURVE		1
#define PARKEY			2
#define PARLIMB			3
#define PARSKEL			4
#define PARVERT1		5
#define PARVERT3		6

#define PARSLOW			16

/* char! transflag */
#define OB_OFFS_LOCAL	1

#define OB_DUPLI		(8+16)
#define OB_DUPLIFRAMES	8
#define OB_DUPLIVERTS	16
#define OB_DUPLIROT		32
#define OB_DUPLINOSPEED	64

#define OB_POWERTRACK	128

/* char! ipoflag */
#define OB_DRAWKEY			1
#define OB_DRAWKEYSEL		2
#define OB_OFFS_OB			4
#define OB_OFFS_MAT			8
#define OB_OFFS_VKEY		16
#define OB_OFFS_PATH		32
#define OB_OFFS_PARENT		64
#define OB_OFFS_PARTICLE	128


/* trackflag / upflag */
#define OB_POSX			0
#define OB_POSY			1
#define OB_POSZ			2
#define OB_NEGX			3
#define OB_NEGY			4
#define OB_NEGZ			5

/* infoflag */
#define OB_INFO_LOC		1
#define OB_INFO_SCO		2
#define OB_INFO_DROT	4
#define OB_INFO_HOTSPOT	8

/* dt: nummers */
#define OB_BOUNDBOX		1
#define OB_WIRE			2
#define OB_SOLID		3
#define OB_SHADED		4
#define OB_TEXTURE		5

/* dtx: flags */
#define OB_AXIS			2
#define OB_TEXSPACE		4
#define OB_DRAWNAME		8
#define OB_DRAWIMAGE	16

/* **************** MESH ********************* */

/* texflag */
#define AUTOSPACE		1

/* me->flag (let op alternatieven in blendpsx.h) */
#define ME_ISDONE		1
#define ME_NOPUNOFLIP	2
#define ME_TWOSIDED		4
#define ME_UVEFFECT		8
#define ME_VCOLEFFECT	16
#define ME_AUTOSMOOTH	32
#define ME_SMESH		64

/* puno (mface) */
#define ME_FLIPV1		1
#define ME_FLIPV2		2
#define ME_FLIPV3		4
#define ME_FLIPV4		8
#define ME_PROJXY		16
#define ME_PROJXZ		32
#define ME_PROJYZ		64

/* edcode (mface) */
#define ME_V1V2			1
#define ME_V2V3			2
#define ME_V3V1			4
#define ME_V3V4			4
#define ME_V4V1			8
/* resterende vier bits voor dface */


/* flag (mface) */
#define ME_SMOOTH		1
#define ME_VERTEXCOL	4

/* mvert->flag (1=select) */
#define ME_SPHERETEST	2
#define ME_SPHERETEMP	4

/* **************** CURVE ********************* */

/* flag */
#define CU_3D			1
#define CU_FRONT		2
#define CU_BACK			4
#define CU_PATH			8
#define CU_FOLLOW		16
#define CU_UV_ORCO		32
#define CU_NOPUNOFLIP	64

/* spacemode */
#define CU_LEFT			0
#define CU_MIDDLE		1
#define CU_RIGHT		2
#define CU_FLUSH		3

/* flag (nurb) */
#define CU_SMOOTH		ME_SMOOTH

/* type (nurb) */
#define CU_POLY			0
#define CU_BEZIER		1
#define CU_BSPLINE		2
#define CU_CARDINAL		3
#define CU_NURBS		4
#define CU_2D			8

/* flagu flagv (nurb) */
#define CU_CYCLIC		1

/* h1 h2 (beztriple) */
#define HD_FREE		0
#define HD_AUTO		1
#define HD_VECT		2
#define HD_ALIGN	3

/* *************** FONT ****************** */

#define FO_CURS			1
#define FO_CURSUP		2
#define FO_CURSDOWN		3
#define FO_DUPLI		4

/* **************** METABALL ********************* */

#define MB_MAXELEM		1024

/* mb->flag */
#define MB_UPDATE_ALWAYS	0
#define MB_UPDATE_HALFRES	1
#define MB_UPDATE_FAST		2

/* ml->type */
#define MB_BALL		0
#define MB_TUBEX	1
#define MB_TUBEY	2
#define MB_TUBEZ	3
#define MB_CIRCLE	4

/* ml->flag */
#define MB_NEGATIVE	2


/* **************** MATERIAL ********************* */

/* colormodel */
#define MA_RGB			0
#define MA_CMYK			1
#define MA_YUV			2
#define MA_HSV			3

/* mode (is int) */
#define MA_TRACEBLE		1
#define MA_SHADOW		2
#define MA_SHLESS		4
#define MA_WIRE			8
#define MA_VERTEXCOL	16
#define MA_HALO			32
#define MA_ZTRA			64
#define MA_VERTEXCOLP	128
#define MA_ZINV			256
#define MA_HALO_RINGS	256
#define MA_ENV			512
#define MA_HALO_LINES	512
#define MA_ONLYSHADOW	1024
#define MA_HALO_XALPHA	1024
#define MA_STAR			0x800
#define MA_HALOTEX		0x1000
#define MA_HALOPUNO		0x2000
#define MA_NOMIST		0x4000
#define MA_HALO_SHADE	0x4000
#define MA_HALO_FLARE	0x8000

/* texco */
#define TEXCO_ORCO		1
#define TEXCO_REFL		2
#define TEXCO_NORM		4
#define TEXCO_GLOB		8
#define TEXCO_UV		16
#define TEXCO_OBJECT	32
#define TEXCO_LAVECTOR	64
#define TEXCO_VIEW		128
#define TEXCO_STICKY	256
#define TEXCO_OSA		512
#define TEXCO_WINDOW	1024
#define NEED_UV			2048

/* mapto */
#define MAP_COL			1
#define MAP_NORM		2
#define MAP_COLSPEC		4
#define MAP_COLMIR		8
#define MAP_VARS		(0xFFF0)
#define MAP_REF			16
#define MAP_SPEC		32
#define MAP_EMIT		64
#define MAP_ALPHA		128
#define MAP_HAR			256
#define MAP_XTRA		512

/* **************** MTEX ********************* */

/* proj */
#define PROJ_N			0
#define PROJ_X			1
#define PROJ_Y			2
#define PROJ_Z			3

/* texflag */
#define MTEX_RGBTOINT	1
#define MTEX_STENCIL	2
#define MTEX_NEGATIVE	4
#define MTEX_ALPHAMIX	8

/* blendtype */
#define MTEX_BLEND		0
#define MTEX_MUL		1
#define MTEX_ADD		2
#define MTEX_SUB		3

/* **************** TEX ********************* */

/* type */
#define TEX_CLOUDS		1
#define TEX_WOOD		2
#define TEX_MARBLE		3
#define TEX_MAGIC		4
#define TEX_BLEND		5
#define TEX_STUCCI		6
#define TEX_NOISE		7
#define TEX_IMAGE		8
#define TEX_PLUGIN		9
#define TEX_ENVMAP		10

/* imaflag */
#define TEX_INTERPOL	1
#define TEX_USEALPHA	2
#define TEX_MIPMAP		4
#define TEX_FIELDS		8
#define TEX_IMAROT		16
#define TEX_CALCALPHA	32
#define TEX_ANIMCYCLIC	64
#define TEX_ANIM5		128
#define TEX_ANTIALI		256
#define TEX_ANTISCALE	512
#define TEX_STD_FIELD	1024
#define TEX_FILTX		2048
#define TEX_GENLOCK		4096

#define TEX_LASOPPATCH	8192
#define TEX_MORKPATCH	16384

/* flag */
#define TEX_COLORBAND	1
#define TEX_FLIPBLEND	2
#define TEX_NEGALPHA	4

/* extend (begint bij 1 ivm backward comp.) */
#define TEX_EXTEND		1
#define TEX_CLIP		2
#define TEX_REPEAT		3
#define TEX_CLIPCUBE	4

/* noisetype */
#define TEX_NOISESOFT	0
#define TEX_NOISEPERL	1

/* wrap */
#define MTEX_FLAT		0
#define MTEX_CUBE		1
#define MTEX_TUBE		2
#define MTEX_SPHERE		3

/* return value */
#define TEX_INT		0
#define TEX_RGB		1
#define TEX_NOR		2


/* **************** EnvMap ********************* */

/* type */
#define ENV_CUBE	0
#define ENV_PLANE	1
#define ENV_SPHERE	2

/* stype */
#define ENV_STATIC	0
#define ENV_ANIM	1
#define ENV_LOAD	2

/* ok */
#define ENV_NORMAL	1
#define ENV_OSA		2

/* **************** IMAGE ********************* */

/* flag */
#define IMA_HALVE		1
#define IMA_BW			2
#define IMA_FROMANIM	4
#define IMA_USED		8

/* tpageflag */
#define IMA_TILES		1
#define IMA_TWINANIM	2
#define IMA_COLCYCLE	4

/* **************** LAMP ********************* */

/* type */
#define LA_LOCAL		0
#define LA_SUN			1
#define LA_SPOT			2
#define LA_HEMI			3

/* mode */
#define LA_SHAD			1
#define LA_HALO			2
#define LA_LAYER		4
#define LA_QUAD			8
#define LA_NEG			16
#define LA_ONLYSHADOW	32
#define LA_SPHERE		64
#define LA_SQUARE		128
#define LA_TEXTURE		256
#define LA_OSATEX		512

/* mapto */
#define LAMAP_COL		1

/* **************** CAMERA ********************* */

/* type */
#define CAM_PERSP		0
#define CAM_ORTHO		1

/* flag */
#define CAM_SHOWLIMITS	1
#define CAM_SHOWMIST	2
#define CAM_NETWORK		4
#define CAM_SHOWNETCLIP	8
#define CAM_HOLO1		16
#define CAM_HOLO2		32

/* ***************** LATTICE ********************* */

/* flag */
#define LT_GRID		1
#define LT_OUTSIDE	2


/* ***************** IKA ********************* */

/* flag */
#define IK_GRABEFF		1
#define IK_XYCONSTRAINT	2


/* ***************** SECTOR ********************* */


#define DYNA_REF(ma)		(ma->mirr)
#define DYNA_FH_DIST(ma)	(ma->mirg)
#define DYNA_FH_INT(ma)		(ma->mirb)
#define DYNA_FH_FRICT(ma)	(ma->specr)
#define DYNA_FH_XYFRICT(ma)	(ma->emit)
#define DYNA_LAY(ma)		(ma->

/* ma->mode */
#define DYNA_FH_NORMAL	1

/* type */
#define SE_CUBE		0

/* flag */
#define SE_SHOW_TEXMESH		1
#define SE_GHOST_OTHER		8
#define SE_RENDER_NETWORK	16


/* portal->type */
#define PO_XPOS		1
#define PO_XNEG		2
#define PO_YPOS		3
#define PO_YNEG		4
#define PO_ZPOS		5
#define PO_ZNEG		6
#define PO_FACE		32

/* tface->mode (beetje compatible met PMD formaat) */
/* sharedvert bepaalt hoe de PSX versie wordt, sharedcol is vertexkleur en puno */
#define TF_QUAD			1
#define TF_GOUR			2
#define TF_TEX			4
#define TF_SHAREDVERT	8
#define TF_LIGHT		16

#define TF_SHAREDCOL	64
#define TF_TILES		128
#define TF_BILLBOARD	256
#define TF_TWOSIDE		512
#define TF_NO_SUBDIV	1024
#define TF_OBCOL		2048
#define TF_MASK			4096
#define TF_SHADOW		8192

/* tface->flag: 1=select 2=active*/
#define TF_SEL1		4
#define TF_SEL2		8
#define TF_SEL3		16
#define TF_SEL4		32
#define TF_HIDE		64

/* tface->transp */
#define TF_SOLID	0
#define TF_ADD		1
#define TF_ALPHA	2
#define TF_SUB		3

/* OLD */
#define TF_ABR			(256+512)

/* netlink->flag */
#define NL_BACKWARD		1
#define NL_BOTH			2

/* dface->flag */
#define DF_HILITE		1

/* dface->edcode: (zie ook mface) ook voor vertices! */
#define DF_V1			1
#define DF_V2			2
#define DF_V3			4
#define DF_V4			8
#define DF_V1V2			16
#define DF_V2V3			32
#define DF_V3V1			64
#define DF_V3V4			64
#define DF_V4V1			128

/* ***************** LIFE ********************* */

/* sensor_input retval: */
#define SN_CHANGED		1
#define SN_DOFORCE		2
#define SN_DOSPEED		4
#define SN_DOOMEGA		8
#define SN_ROTCHANGED	16



/* komen uit blendpsx.h */
#define SE_MAXBUF	32
#define LF_MAXBUF	32
#define MAXACTVAR	100

#define DTIME	0.02
#define IDTIME	50.0


/* lf->type */
#define LF_PROP			0
#define LF_DYNAMIC		1
#define LF_LINK			2

/* lf->flag: buttons, sgistuff */
#define LF_SENSORS		1

#define LF_SHOW_TEXMESH	4
#define LF_GHOST_OTHER	8
#define LF_DRAWNEAR		16
#define LF_MAINACTOR	32
#define LF_CAMERA_ACTOR	64
#define LF_RECALCLIGHT	128
#define LF_AERO_AXIS_Y	256

#define LF_SPHERE		2048
#define LF_DO_FH		4096

/* lf->dflag : starten altijd op nul */
#define LF_TEMPLIFE		1
#define LF_DYNACHANGED	2
#define LF_OMEGA		4
#define LF_DONTDRAW		8
#define LF_TRIP_POS		16
#define LF_TRIP_NEG		32
#define LF_NO_DAMAGE	64

/* lf->state: lokale vars: namen willekeurig? */
#define LF_STATE	1
#define LF_DAMAGE	2
#define LF_ACTION	3
#define LF_FRAME	4


/* SENSORS */

/* ev->flag */
#define SN_HOLD			1
#define SN_NOT			2

/* ev->event: eerste 32 voor toetsen */
/* ev->var: controller nr */

#define SN_NEAR			64
/* ev->fac: mindist, ev->fac1: enddist, ev->var: memory */
#define SN_CONTACT		65
/* ev->name: naam mat, ev->poin: mat */
#define SN_TIMER		66
/* ev->var: triggertime */
#define SN_VAR_EQUAL	67
/* ev->var: testwaarde:  */
#define SN_VAR_INTERVAL	68
/* ev->sfac: min, ev->sfac1: max */
#define SN_VAR_CHANGED	69
/* ev->var: memory */
#define SN_TRIPFACE		80
/* */
#define SN_COLLISION	81
/* ev->sfac en ev->var: demping */


/* ac->action */

#define SN_X_TRANS		2
#define SN_Y_TRANS		4
#define SN_Z_TRANS		6
#define SN_X_ROT		8
#define SN_Y_ROT		10
#define SN_Z_ROT		12
/* ac->force */

/* ac->flag: limits */
#define SN_LIMITS		1
#define SN_IPOFORCE		2
#define SN_IPO_SETVAR	16


/* 100-199: ipoos */
#define SN_PLAY_OBIPO		100
#define SN_PINGPONG_OBIPO	101
#define SN_FLIPPER_OBIPO	102
#define SN_DELTA_OBIPO		103
#define SN_LOOPSTOP_OBIPO	104
#define SN_LOOPEND_OBIPO	105
#define SN_K2K_OBIPO	106
/* ac->butsta en butend worden ac->min en max */
/* ac->sta en end worden uitgelezen uit ipokeys  */
/* ac->var: bitjes: */
#define SN_K2K_PREV		1
#define SN_K2K_CYCLIC	2
#define SN_K2K_PINGPONG	4
#define SN_K2K_HOLD		8


/* 200-299: divers */
#define SN_LAYERMOVE		200
#define SN_STARTTIMER		201
/* ac->sta: initwaarde timer ac->cur */
#define SN_ADDLIFE			202
#define SN_REPLACELIFE		203
#define SN_LOADFILE			204
#define SN_RESTART			205
#define SN_QUIT				206
#define SN_VISIBLE			207
#define SN_GOTO				208
#define SN_ENDLIFE			209
#define SN_REPLACEMESH		210
#define SN_SETCAMERA		211
/* ac->end: len, ac->sta: startstill */
#define SN_PLAYMOVIE		212
/* ac->sta: tijd, ac->cur: type, ac->poin: object */
#define SN_TRACKTO			213
/* ac->var: hoeveelheid */
#define SN_ADD_DAMAGE		214
/* is een 'hold' event */
#define SN_NO_DAMAGE		215
/* ac->min, ac->max: dist, ac->force=height */
/* ac->fac= slow voor kijk-corr */
#define SN_ROBBIE_M			216
/* ac name: sample */
#define SN_PLAY_SOUND		217

/* ac->force, min, max is x,y,z */
/* ac->var: 0 is abs, 1 is rel */

/* 300-399: vars */
#define SN_SET_VARIABLE		300
#define SN_ADD_VARIABLE		301
#define SN_TOG_VARIABLE		302
#define SN_IPO_VARIABLE		303
#define SN_HOLD_VARIABLE	304

/* ac->sta: min, ac->end: max, ac->cur: initwaarde, ac->flag= flag */
#define SN_VAR_CYCLIC		1
#define SN_VAR_NOPRINT		2

/* **************** IPO ********************* */

/* vartype */
#define IPO_CHAR		0
#define IPO_SHORT		1
#define IPO_INT			2
#define IPO_LONG		3
#define IPO_FLOAT		4
#define IPO_DOUBLE		5
#define IPO_FLOAT_DEGR	6
/* zeer speciaal geval in keys */
#define IPO_BEZTRIPLE	100
#define IPO_BPOINT		101


#define IPO_BITS		16
#define IPO_CHAR_BIT	16
#define IPO_SHORT_BIT	17
#define IPO_INT_BIT		18

/* ipo */
#define IPO_CONST		0
#define IPO_LIN			1
#define IPO_BEZ			2
#define IPO_MIXED		3

/* extrap */
#define IPO_HORIZ		0
#define IPO_DIR			1
#define IPO_CYCL		2
#define IPO_CYCLX		3

/* flag */
#define IPO_VISIBLE		1
#define IPO_SELECT		2
#define IPO_EDIT		4
#define IPO_LOCK		8

/* **************** KEY ********************* */

/* type */
#define KEY_NORMAL		0
#define KEY_RELATIVE	1

/* keyblock->type */
#define KEY_LINEAR		0
#define KEY_CARDINAL	1
#define KEY_BSPLINE		2



/* **************** WORLD ********************* */

/* skytype */
#define WO_SKYBLEND		1
#define WO_SKYREAL		2
#define WO_SKYPAPER		4
/* tijdens render: */
#define WO_SKYTEX		8
#define WO_ZENUP		16

/* mode */
#define WO_MIST			1
#define WO_STARS		2
#define WO_DOF			4

/* mapto */
#define WOMAP_BLEND		1
#define WOMAP_HORIZ		2
#define WOMAP_ZENUP		4
#define WOMAP_ZENDOWN	8

/* **************** SCRIPTLINKS ********************* */

#define SCRIPT_FRAMECHANGED	1
#define SCRIPT_ONLOAD		2

/* **************** RENDERDATA ********************* */

/* bufflag */
#define R_BACKBUF		1
#define R_BACKBUFANIM	2
#define R_FRONTBUF		4
#define R_FRONTBUFANIM	8

/* mode */
#define R_OSA			1
#define R_SHADOW		2
#define R_GAMMA			4
#define R_ORTHO			8
#define R_TRACE			16
#define R_EDGE			32
#define R_FIELDS		64
#define R_FIELDSTILL	128
#define R_GAUSS			256
#define R_BORDER		512
#define R_PANORAMA		1024
#define R_MOVIECROP		2048
#define R_COSMO			4096
/* deze verschillen tussen IrisGL en OpenGL!!! */
#define R_ODDFIELD		8192
#define R_MBLUR			16384

/* scemode */
#define R_DOSEQ			1
#define R_BG_RENDER		2
#define R_NETWORK		4
#define R_HOTSPOT		8
#define R_EXTENSION		16
#define R_OGL			32

/* alphamode */
#define R_ADDSKY		0
#define R_ALPHAPREMUL	1
#define R_ALPHAKEY		2

/* planes */
#define R_PLANES24		24
#define R_PLANES32		32
#define R_PLANESBW		8

/* imtype */
#define R_TARGA		0
#define R_IRIS		1
#define R_HAMX		2
#define R_FTYPE		3
#define R_JPEG90	4
#define R_MOVIE		5
#define R_TANX		6
#define R_IRIZ		7
#define R_MDEC		8
#define R_CMBB		11
#define R_TGA4		12
#define R_HAMX4		13
#define R_RAWTGA	14
#define R_AVIRAW	15
#define R_AVIJPEG	16
#define R_AVIJMF	17


/* **************** RENDER ********************* */

/* mode: zie RENDERDATA */

/* flag */
#define R_ZTRA			1
#define R_HALO			2
#define R_SEC_FIELD		4
#define R_LAMPHALO		8
#define R_RENDERING		16
#define R_ANIMRENDER	32

/* vlakren->flag */
#define R_SMOOTH		ME_SMOOTH
#define R_VISIBLE		2
#define R_VERTEXCOL		ME_VERTEXCOL
#define R_NOPUNOFLIP	8
#define R_CMAPCODE		16

/* vertren->texofs (texcoordinaten offset vanaf vertren->orco */
#define R_UVOFS3	1


/* haloren->type: flags */

#define HA_ONLYSKY		1
#define HA_VECT			2
#define HA_XALPHA		4
#define HA_FLARECIRC	8


/* DISPLAYMODE */
#define R_DISPLAYVIEW	0
#define R_DISPLAYWIN	1
#define R_DISPLAYAUTO	2

/* ***************** USERDEF ****************** */

/* flag */
#define AUTOSAVE		1
#define AUTOGRABGRID	2
#define AUTOROTGRID		4
#define AUTOSIZEGRID	8
#define SCENEGLOBAL		16
#define TRACKBALL		32
#define DUPLILINK		64
#define FSCOLLUM		128
#define MAT_ON_OB		256
#define NO_CAPSLOCK		512
#define VIEWMOVE		1024
#define TOOLTIPS		2048
#define TWOBUTTONMOUSE	4096

/* dupflag */
#define DUPMESH			1
#define DUPCURVE		2
#define DUPSURF			4
#define DUPFONT			8
#define DUPMBALL		16
#define DUPLAMP			32
#define DUPIPO			64
#define DUPMAT			128
#define DUPTEX			256

/* ***************** SEQUENCE ****************** */

/* seq->flag */
#define SEQ_LEFTSEL		2
#define SEQ_RIGHTSEL	4
#define SEQ_OVERLAP		8
#define SEQ_FILTERY		16

/* seq->type LET OP BITJE 3!!! */
#define SEQ_IMAGE		0
#define SEQ_META		1
#define SEQ_SCENE		2
#define SEQ_MOVIE		3

#define SEQ_EFFECT		8
#define SEQ_CROSS		8
#define SEQ_ADD			9
#define SEQ_SUB			10
#define SEQ_ALPHAOVER	11
#define SEQ_ALPHAUNDER	12
#define SEQ_GAMCROSS	13
#define SEQ_MUL			14
#define SEQ_OVERDROP	15
#define SEQ_PLUGIN		24

#if defined(__sgi) || defined(__SUN) || defined (__PPC__)
#define RCOMP	3
#define GCOMP	2
#define BCOMP	1
#define ACOMP	0
/* ZR CHANGED THIS */

#else

/*
#endif

#if defined(__FreeBSD__) || defined(linux) || defined(__BeOS) || defined(__WIN32)
*/
/* END BAD ZR */
#define RCOMP	0
#define GCOMP	1
#define BCOMP	2
#define ACOMP	3
#endif

#ifdef GS
#undef GS
#endif
#define GS(a)	(*((short *)(a)))

#endif

