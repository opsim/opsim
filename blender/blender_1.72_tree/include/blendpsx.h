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


/* 
 *  blendpsx.h    juli 96
 * 
 *  MIXED TYPE!!!
 *  deze moet ook aan de BLENDER ZELF kunnen hangen
 * 
 */


#ifndef BLENDPSX_H
#define BLENDPSX_H

/* ************* MATRIX flag ************ */

#define MAT_ONE		0x100
#define MAT_90		0x200
#define MAT_180		0x400
#define MAT_270		0x800
#define MAT_FAST	0xF00
#define MAT_CALC	0x1000

/* ************* PRIMITIVES ***************** */

#define P_F3	0
#define P_F4	1
#define P_G3	2
#define P_G4	3

#define P_FT3	4
#define P_FT4	5
#define P_GT3	6
#define P_GT4	7

#define P_LF3	8
#define P_LF4	9
#define P_LG3	10
#define P_LG4	11

#define P_LFT3	12
#define P_LFT4	13
#define P_LGT3	14
#define P_LGT4	15

	/* puno light: zelfde struct als met flat light, alleen gouraud versies */
#define P_LPG3	16
#define P_LPG4	17
#define P_LPGT3	18
#define P_LPGT4	19

	/* twosided: alleen gourtex (lighted nog doen) */
#define P_2GT3	20
#define P_2GT4	21
#define P_2LGT3	22
#define P_2LGT4	23

	/* OBcolor: alleen niet lighted */
#define P_OBF3	24
#define P_OBF4	25
#define P_OBG3	26
#define P_OBG4	27

#define P_OBFT3	28
#define P_OBFT4	29
#define P_OBGT3	30
#define P_OBGT4	31

#define P_BILLB		32
#define P_OBBILLB	33

#define P_MASK3		34
#define P_MASK4		35

#define P_SHADOW	36

/* ******** FLAT **************** */

typedef struct {
	POLY_F3 po;
	/* POLY_F3 po2; */
	SVECTOR *v1, *v2, *v3;
	CVECTOR c0;
} PrimF3;

typedef struct {
	POLY_F4 po;
	/* POLY_F4 po2; */
	SVECTOR *v1, *v2, *v3, *v4;
	CVECTOR c0;
} PrimF4;

typedef struct {
	POLY_F3 po;
	/* POLY_F3 po2; */
	SVECTOR *v1, *v2, *v3;
	CVECTOR c0;
	SVECTOR no;
} PrimLF3;

typedef struct {
	POLY_F4 po;
	/* POLY_F4 po2; */
	SVECTOR *v1, *v2, *v3, *v4;
	CVECTOR c0;
	SVECTOR no;
} PrimLF4;

/* ******** GOUR **************** */

typedef struct {
	POLY_G3 po;
	/* POLY_G3 po2; */
	SVECTOR *v1, *v2, *v3;
	CVECTOR c0, c1, c2;
} PrimG3;

typedef struct {
	POLY_G4 po;
	/* POLY_G4 po2; */
	SVECTOR *v1, *v2, *v3, *v4;
	CVECTOR c0, c1, c2, c3;
} PrimG4;

typedef struct {
	POLY_G3 po;
	/* POLY_G3 po2; */
	SVECTOR *v1, *v2, *v3;
	CVECTOR c0, c1, c2;
	SVECTOR no;
} PrimLG3;

typedef struct {
	POLY_G4 po;
	/* POLY_G4 po2; */
	SVECTOR *v1, *v2, *v3, *v4;
	CVECTOR c0, c1, c2, c3;
	SVECTOR no;
} PrimLG4;

/* ******** FLAT TEX **************** */

typedef struct {
	DR_TWIN dr;
	POLY_FT3 po;
	/* POLY_FT3 po2; */
	SVECTOR *v1, *v2, *v3;
	CVECTOR c0;
} PrimFT3;

typedef struct {
	DR_TWIN dr;
	POLY_FT4 po;
	/* POLY_FT4 po2; */
	SVECTOR *v1, *v2, *v3, *v4;
	CVECTOR c0;
} PrimFT4;

typedef struct {
	DR_TWIN dr;
	POLY_FT3 po;
	/* POLY_FT3 po2; */
	SVECTOR *v1, *v2, *v3;
	CVECTOR c0;
	SVECTOR no;
} PrimLFT3;

typedef struct {
	DR_TWIN dr;
	POLY_FT4 po;
	/* POLY_FT4 po2; */
	SVECTOR *v1, *v2, *v3, *v4;
	CVECTOR c0;
	SVECTOR no;
} PrimLFT4;

/* ******** GOUR TEX **************** */

typedef struct {
	DR_TWIN dr;
	POLY_GT3 po;
	/* POLY_GT3 po2; */
	SVECTOR *v1, *v2, *v3;
	CVECTOR c0, c1, c2;
} PrimGT3;

typedef struct {
	DR_TWIN dr;
	POLY_GT4 po;
	/* POLY_GT4 po2; */
	SVECTOR *v1, *v2, *v3, *v4;
	CVECTOR c0, c1, c2, c3;
} PrimGT4;

typedef struct {
	DR_TWIN dr;
	POLY_GT3 po;
	/* POLY_GT3 po2; */
	SVECTOR *v1, *v2, *v3;
	CVECTOR c0, c1, c2;
	SVECTOR no;
} PrimLGT3;

typedef struct {
	DR_TWIN dr;
	POLY_GT4 po;
	/* POLY_GT4 po2; */
	SVECTOR *v1, *v2, *v3, *v4;
	CVECTOR c0, c1, c2, c3;
	SVECTOR no;
} PrimLGT4;


/* *********** SPECIALS ************* */

typedef struct {
	DR_TWIN dr;
	POLY_FT4 po;
	/* POLY_FT4 po2; */
	CVECTOR c0;
	SVECTOR cent;		/* cent.pad= size */
} PrimBillb;



/* ****************** LIB ************* */

typedef struct pID {
	void *next, *prev;
	struct pLibrary *lib;
	char name[4];
	short us, flag;
} pID;


typedef struct pLibrary {
	pID id;
	int tot;		/* tot, idblock en filedata zijn voor lezen en schrijven */
	pID *idblock;
	char *filedata;
	char name[80];
} pLibrary;


/* *************** IPO ******************** */

/* ipo->type: gedeeltelijk flags */
#define	IP_LOC		1
#define	IP_ROT		2
#define	IP_SIZE		4
#define IP_OBCOL	8
#define	IP_FROMOB	15

typedef struct pIpo {
	void *next, *prev;

	short type, nr_keys;	/* keys staan achter ipodata */
	short nr_elems, sta;
	short elemsize, rt;
	int offs[3];			/* zodat loc-ipo's shortjes blijven */
} pIpo;


/* **************** IMAGE ********************* */

/*  ima->type */
#define TIM_4BITS	0
#define TIM_8BITS	1
#define TIM_16BITS	2
/* ima->flag */
#define IMA_TWINANIM	2
#define IMA_COLCYCLE	4
#define IMA_FREECLUT	128


typedef struct pImage {
	pID id;
	
	char name[80];
	
	/* imbuf included: */
	short x, y, sx, sy;		/* x,y: size   sx,sy: schermco 1e pixel */
	char type, flag;
	short clutlen;
	void *rect;
	void *clut;
	
	
	/* texture pagina */
	short tpageflag, tpagenr;  /* nr: vanaf 0 */
	short clutnr, rt;
	short xrep, yrep;		   /* clutcoord in framebuf */	

	short fba_rect, fba_clut;

	/* twin data: */
	char twsta, twend, twcur, twtot;
	
	
} pImage;



/* **************** OBJECT ********************* */



/* dt == drawtype */
#define OB_DRAWSHADED	1

#define PAROBJECT		0
#define PARVERT1		5
#define PARSLOW			16

typedef struct pObject {
	pID id;
	short type, partype;
	short flag, dt;
	uint lay;
	CVECTOR obcol;

	struct pObject *parent, *track;
	void *data;
	
	int loc[3];
	short size[3], rot[3];
	MATRIX obmat;
	MATRIX imat;
	
	/* 40  bytes: kunnen ook extra gemallocced worden!!! */
	short loclight[3][3], sf;
	short lcolor[3][3];
	ushort dfras;

	ListBase network;
	
} pObject;


/* **************** MESH ********************* */

typedef struct pOcInfo {
	short dvec[3];
	ushort size[3];
} pOcInfo;

#ifndef ME_SIDONE
#define ME_ISDONE	1
#endif

#define ME_PUNO			2
#define ME_EFFECT		(8+16)
#define ME_UVEFFECT		8
#define ME_VCOLEFFECT	16

typedef struct pMesh {
	pID id;

	short totvert, totpacket;
	short totface, flag;
	
	void *packetdata, *dface;
	short *mvert;
	
	int lastfra, polysize;
	pOcInfo *oc;
	
} pMesh;

/* ***************** SECTOR *********************** */


/* type */
#define SE_CUBE		0


/* portal->type */
#define PO_XPOS		1
#define PO_XNEG		2
#define PO_YPOS		3
#define PO_YNEG		4
#define PO_ZPOS		5
#define PO_ZNEG		6
#define PO_FACE		32

	/* netlink->flag */
#define NL_BACKWARD		1
#define NL_BOTH			2


typedef struct pNetLink {
	struct pNetLink *next, *prev;
	pObject *ob;
	short type, flag;
	short sfra, len;			/* sfra: offset in 'movie' */
	
	
} pNetLink;

typedef struct pCamPos{
	pObject *ob;
	MATRIX piramat;
	short axis[2];
	char visi, direction, dist, d_axis;	/* LOGICAL VALUES */
	short view, dura, hold, flag;
	
	pNetLink *nl;
	
	int sta, end;		/* z-visible vanuit camera */
	short lens, rt;		/* lens: tijdelijk voor berekenen piramat */
		
	short cfie, sfra;	/* cfie: tijdens nl afspelen van 0->tot, sfra: offset in 'movie' */
} pCamPos;

typedef struct BMovie {
	struct Movie *mv;
	
	char file[64];	

	short cfra, lastfra;
	short flag, count;
} BMovie;

typedef struct pLBuf {
	short tot, max;
	pObject **ob;
} pLBuf;


typedef struct pSector {
	pID id;

	pObject *ob;
	short type, flag, totport, actport;
	
	short size[3];					/* in lokale ob coords */
	short bbsize[3], totcam;		/* bbsize= boundbox size in wereld coords */
	short depth, rt;
	
	struct pPortal *portals;
	pLBuf lbuf;
	
	pMesh *dynamesh, *texmesh;
	pCamPos *campos;
	
	ListBase ipo;
	
	BMovie *movie;
	char *name;

} pSector;

typedef struct pPortal {

	pSector *sector;
	pObject *ob;
		
	short type, flag;
	
} pPortal;


/* ***************** LIFE *********************** */

/* komen uit de gl/device.h */
/* #define LEFTARROWKEY	73	 */
/* #define DOWNARROWKEY	74	 */
/* #define RIGHTARROWKEY	80	 */
/* #define UPARROWKEY		81	 */
/* #define PAD2		64	 */
/* #define PAD4		63	 */
/* #define PAD6		70	 */
/* #define PAD8		68	 */

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

/* sensor_input retval: */
#define SN_CHANGED		1
#define SN_DOFORCE		2
#define SN_DOSPEED		4
#define SN_DOOMEGA		8
#define SN_ROTCHANGED	16



/* ev->flag */
#define SN_HOLD			1
#define SN_NOT			2


/* ev->event */
#define SN_NEAR			64
	/* ev->fac: mindist, ev->fac1: enddist, ev->var: memory */
#define SN_CONTACT		65
	/* ev->name: naam mat, ev->poin: mat */
#define SN_TIMER		66
	/* ev->var: triggertime */
#define SN_VAR_EQUAL	67
	/* ev->var: testwaarde */
#define SN_VAR_INTERVAL	68
	/* ev->sfac: min, ev->sfac1:max */
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
	/* ac->sta: tijd, ac->poin: object */
#define SN_TRACKTO			213
	/* ac->var: hoeveelheid */
#define SN_ADD_DAMAGE		214
#define SN_NO_DAMAGE		215
	/* ac->min, ac->max: dist, ac->force=height */
	/* ac->fac= slow voor kijk-corr */
#define SN_ROBBIE_M			216

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

typedef struct pMaterial {
	pID id;
	
	short ref, fh_dist, fh_int, fh_frict;
	short frict, fh_xyfrict, lay, pad1;

} pMaterial;

typedef struct pEvent {
	short event, var, sfac, sfac1;
	short shiftval;
	char flag, pulse;
	void *poin;		/* hier staat in psxfile bij ACTIONVAR een arraynr */
} pEvent;

typedef struct pAction {
	short action, go, cur, var;
	short sta, end;
	short force, min, max, flag, fac, rt;
	void *poin;		/* hier staat in psxfile bij ACTIONVAR een arraynr */
} pAction;

typedef struct pSensor {
	
	short totaction, totevent;
	short event, evento;		/* voor 'hold' test */
	short flag, rt;
	
	pEvent *events;
	pAction *actions;
} pSensor;


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


typedef struct pDFace {
	short *v1, *v2, *v3, *v4;	/* voor snelheid: volgorde van belang!!! */
	int dist;
	pMaterial *ma;
	short no[3];
	ushort ocx, ocy, ocz;
	short proj;
	char flag, edcode;
	
} pDFace;



typedef struct pLife {
	pID id;
	pSector *sector;
	
	char type, lay;
	short flag;
	int oldloc[3], loc[3];		/* loc ook in object zetten */
	short speed[3], totsens;	
	int oldloc1[3], loc1[3]; 	/* lokale sector co's */
	short speed1[3], timer;
	int startloc[3];
	short startrot[3];
	short rot[3], rotspeed[3];
	MATRIX oldimat;
	short mass, frict, rotfrict, sfra;
	short cfra, axsize, frictfac, dflag;
	short state[4];
	short floorloc[3], rt;			/* offset tov loc[] */
	
	pSensor *sensors;
	pMaterial *contact;
	pObject *collision, *from;
	pDFace *floor;
	int colloc[3];					/* collision loc */
	
	pMesh *dynamesh;
	pMesh *texmesh;
	
	pLBuf links;
	
	ListBase ipo;
	
} pLife;


/* *************** MAIN ********************** */

typedef struct pMain {
	struct pMain *next, *prev;
	char name[80];
	char *filedata;
	pLibrary *curlib;
	ListBase scene;
	ListBase library;
	ListBase object;
	ListBase mesh;
	ListBase mat;
	ListBase image;
	ListBase ika;
	ListBase sector;
	ListBase life;
	ListBase lamp;
	ListBase camera;
	ListBase ipo;
	ListBase key;
	ListBase world;
	
} pMain;


/* *************** SCENE ********************** */

typedef struct pBase {
	struct pBase *next, *prev;
	uint lay;
	int flag;
	pObject *object;
	
} pBase;


typedef struct pScene {
	pID id;
	pObject *camera;
	/* World *world; */
	struct pScene *set;
	uint lay;
	
	ListBase base;
	pBase *basact;

	short maxdrawsector, grav, lens, rt;
	char mist, mir, mig, mib;

} pScene;

/* ******************** GLOBAL ******************** */

#define SE_MAXBUF	32
#define LF_MAXBUF	32
#define MAXACTVAR	100

/* G.f: naar psxblend.h ivm writeblendpsx.c */

typedef struct pGlobal {
	
	/* active pointers */
	pMain *main;
	pScene *scene;
	pObject *camera;
	pSector *cursector, *sectorbuf[SE_MAXBUF];
	pObject *lifebuf[LF_MAXBUF];
	short *actvarar;
	
	struct PsxFont *font;
	
	ListBase mainbase;

	MATRIX viewmat;
	pMaterial defaultmaterial;
	
	int dfra, dfrao;			/* display-frame, om goed te onderscheiden van SGI-cfra */          
	short totsect, maxsect, totface, totlife;
	
	ushort qual, dfras;
	short versionfile, f, fields, fb_buf3, rt;
	
} pGlobal;




#endif /* BLENDPSX_H */

