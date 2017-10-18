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

/* blender.h    feb 98

 * 
 * 
 */

#ifndef BLENDER_H
#define BLENDER_H


#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifndef WIN32
#include <sys/times.h>
#endif

#if defined(__sgi)
#include <sys/schedctl.h>
#endif
#include <string.h>

#ifndef WIN32
#include <strings.h>
#include <unistd.h>
#endif

#if !defined(__BeOS) && !defined(WIN32)
#include <dlfcn.h>
#endif

#ifdef WIN32
#include "winstuff.h"
#endif

#include "util.h"
#include "iff.h"
#include "blendef.h"
#include "floatpatch.h"
#include "plugin.h"

/* *********** vec ************* */

typedef struct vec2s {
	short x, y;
} vec2s;

typedef struct vec2i {
	int x, y;
} vec2i;

typedef struct vec2f {
	float x, y;
} vec2f;

typedef struct vec2d {
	double x, y;
} vec2d;

typedef struct vec3i {
	int x, y, z;
} vec3i;

typedef struct vec3f {
	float x, y, z;
} vec3f;

typedef struct vec3d {
	double x, y, z;
} vec3d;

typedef struct vec4i {
	int x, y, z, w;
} vec4i;

typedef struct vec4f {
	float x, y, z, w;
} vec4f;

typedef struct vec4d {
	double x, y, z, w;
} vec4d;

typedef struct rcti {
    int xmin, xmax;
    int ymin, ymax;
} rcti;

typedef struct rctf {
    float xmin, xmax;
    float ymin, ymax;
} rctf;

/* ******************************************* */
/* * * * * * * *  L I B R A R Y  * * * * * * * */
/* ******************************************* */


/* **************** MAIN ********************* */

#
#
typedef struct BoundBox {
	float vec[8][3];
} BoundBox;

#
#
typedef struct OcInfo {
	float dvec[3];
	float size[3];
} OcInfo;

/* let op: Sequence heeft identiek begin. */
typedef struct ID {
	void *next, *prev;
	struct ID *new;
	struct Library *lib;
	char name[24];
	short us, flag;
	int pad;
} ID;

typedef struct Library {
	ID id;
	ID *idblock;
	char *filedata;
	char name[160];
	int tot, pad;		/* tot, idblock en filedata zijn voor lezen en schrijven */
} Library;


#
#
typedef struct Main {
	struct Main *next, *prev;
	char name[160];
	short versionfile, rt;
	Library *curlib;
	ListBase scene;
	ListBase library;
	ListBase object;
	ListBase mesh;
	ListBase curve;
	ListBase mball;
	ListBase mat;
	ListBase tex;
	ListBase image;
	ListBase ika;
	ListBase wave;
	ListBase latt;
	ListBase sector;
	ListBase life;
	ListBase lamp;
	ListBase camera;
	ListBase ipo;
	ListBase key;
	ListBase world;
	ListBase screen;
	ListBase vfont;
	ListBase text;
} Main;


/* **************** IPO ********************* */

typedef struct Ipo {
	ID id;
	
	ListBase curve;
	rctf cur;
	short blocktype, showkey;
	int pad;
		
} Ipo;

#
#
typedef struct Path {
	int len;
	float *data;
	float totdist;
} Path;



/* **************** KEY ********************* */

typedef struct KeyBlock {
	struct KeyBlock *next, *prev;
	
	float pos;
	short flag, totelem;
	short type, rt;
	int pad;
	
	void *data;
	
} KeyBlock;


typedef struct Key {
	ID id;
	
	KeyBlock *refkey;
	char elemstr[32];
	int elemsize;
	float curval;
	
	ListBase block;
	Ipo *ipo;
	
	ID *from;

	short type, totkey;
	short slurph, actkey;
	
} Key;

/* **************** CAMERA ********************* */

typedef struct Camera {
	ID id;
	
	short type, flag, drawzoom, hold;
	float clipsta, clipend;
	float netsta, netend;			/* network camera */
	float lens, drawsize;
	float hololen, hololen1;
	
	Ipo *ipo;
	
} Camera;

/* **************** IMAGE ********************* */

typedef struct Image {
	ID id;
	
	char name[160];
	
	struct anim *anim;
	struct ImBuf *ibuf;
	struct ImBuf *mipmap[10];
	
	short ok, flag;
	short lastframe, lastquality;

	/* texture pagina */
	short tpageflag, totbind;
	short xrep, yrep;
	short twsta, twend;
	unsigned int bindcode;
	unsigned int *repbind;	/* om subregio's te kunnen repeaten */
	
} Image;

/* **************** TEX ********************* */

typedef struct MTex {

	short texco, mapto, maptoneg, blendtype;
	struct Object *object;
	struct Tex *tex;
	
	char projx, projy, projz, mapping;
	float ofs[3], size[3];
	
	short texflag, colormodel;
	float r, g, b, k;
	float def_var;
	
	float colfac, norfac, varfac;
	
} MTex;

typedef struct PluginTex {
	char name[160];
	void *handle;
	
	char *pname;
	char *stnames;

	int stypes;
	int vars;
	void *varstr;
	float *result;
	float *cfra;
	
	float data[32];

	int (*doit)();

	void (*callback)();	
	
	int version, pad;
} PluginTex;

typedef struct CBData {
	float r, g, b, a, pos;
	int cur;
} CBData;

typedef struct ColorBand {
	short flag, tot, cur, ipotype;
	CBData data[16];
	
} ColorBand;

typedef struct EnvMap {
	struct Object *object;
	Image *ima;		/* type ENV_LOAD */
	Image *cube[6];		/* these images are dynamic, not part of the main struct */
	float imat[4][4];
	short type, stype;
	float clipsta, clipend;
	unsigned int notlay;
	int cuberes;
	short ok, lastframe;
} EnvMap;

typedef struct Tex {
	ID id;
	
	float noisesize, turbul;
	float bright, contrast, rfac, gfac, bfac;
	float filtersize;
	short noisedepth, noisetype;
	
	short imaflag, flag;
	short type, stype;
	
	float cropxmin, cropymin, cropxmax, cropymax;
	short xrepeat, yrepeat;
	short extend, len;
	short frames, offset, sfra, fie_ima;
	float norfac, *nor;
	
	Ipo *ipo;
	Image *ima;
	PluginTex *plugin;
	ColorBand *coba;
	EnvMap *env;
	
	short fradur[4][2];
	
} Tex;

/* **************** LAMP ********************* */

typedef struct Lamp {
	ID id;
	
	short type, mode;
	
	short colormodel, totex;
	float r, g, b, k;
	
	float energy, dist, spotsize, spotblend;
	float haint;
	float att1, att2;
	
	short bufsize, samp;
	float clipsta, clipend, shadspotsize;
	float bias, soft;
	
	/* texact is voor buttons */
	short texact, shadhalostep;
	
	MTex *mtex[8];
	Ipo *ipo;
	
} Lamp;

/* **************** WAVE ********************* */

typedef struct Wave {
	ID id;
	
	Ipo *ipo;
	
} Wave;

/* **************** IKA ********************* */


/* **************** MATERIAL ********************* */

/* LET OP: type veranderen? ook in ipo.h doen */

typedef struct Material {
	ID id;
	
	short colormodel, lay;		/* lay: voor dynamics */
	float r, g, b;
	float specr, specg, specb;
	float mirr, mirg, mirb;
	float ambr, ambb, ambg;
	
	float amb, emit, ang, spectra;
	float alpha, ref, spec, zoffs, add;
	short har;
	char seed1, seed2;
	
	int mode;
	short flarec, starc, linec, ringc;
	float hasize, flaresize, subsize, flareboost;
	
	/* onderstaand is voor buttons en render*/
	char rgbsel, texact, pr_type, septex;
	short texco, mapto;
	
	MTex *mtex[8];
	Ipo *ipo;
	struct Material *ren;
	
} Material;


/* **************** VFont ********************* */
#
#
typedef struct VFontData {
	struct ListBase nurbsbase[256];
	float	    resol[256];
	float	    width[256];
	float	    *points[256];
	short kcount, rt;
	short *kern;	
} VFontData;

typedef struct VFont {
	ID id;
	
	char name[255], namenull;
	float scale;
	short flag, rt;
	
	VFontData *data;
	
} VFont;

/* **************** METABALL ********************* */

typedef struct MetaElem {
	struct MetaElem *next, *prev;
	
	short type, lay, flag, selcol;
	float x, y, z;
	float expx, expy, expz;
	float rad, rad2, s, len, maxrad2;
	int pad;
	
	float *mat, *imat;
	
} MetaElem;

typedef struct MetaBall {
	ID id;
	
	BoundBox *bb;

	ListBase elems;
	ListBase disp;
	Ipo *ipo;

	Material **mat;

	short flag, totcol;
	int texflag;
	float loc[3];
	float size[3];
	float rot[3];
	float wiresize, rendersize, thresh;
	
} MetaBall;

/* **************** CURVE ********************* */

#
#
typedef struct BevList {
    struct BevList *next, *prev;
    short nr, flag;
    short poly, gat;
} BevList;

#
#
typedef struct BevPoint {
    float x, y, z, alfa, sina, cosa, mat[3][3];
    short f1, f2;
} BevPoint;

typedef struct BezTriple {
	float vec[3][3];
	float alfa;
	short s[3][2];
	short h1, h2;
	char f1, f2, f3, hide;
} BezTriple;

typedef struct BPoint {
	float vec[4];
	float alfa;
	short s[2];
	short f1, hide;
} BPoint;

typedef struct Nurb {
	struct Nurb *next, *prev;
	short type, mat_nr;
	short hide, flag;
	short pntsu, pntsv;
	short resolu, resolv;
	short orderu, orderv;
	short flagu, flagv;
	
	float *knotsu, *knotsv;
	BPoint *bp;
	BezTriple *bezt;
	
} Nurb;


typedef struct Curve {
	ID id;
	
	BoundBox *bb;
	
	ListBase nurb;
	ListBase disp;
	struct Object *bevobj, *textoncurve;
	Ipo *ipo;
	Path *path;
	Key *key;
	Material **mat;
	
	ListBase bev;
	float *orco;

	float loc[3];
	float size[3];
	float rot[3];

	int texflag;

	short pathlen, totcol;
	short flag, bevresol;
	float width, ext1, ext2;
	
	/* default */
	short resolu, resolv;
	
	/* font stuk */
	short len, lines, pos, spacemode;
	float spacing, linedist, shear, fsize;
	float xof, yof;
	
	char *str, family[24];
	VFont *vfont;

} Curve;

/* **************** IPOCURVE (Ipo staat bovenin) ********************* */

typedef struct IpoCurve {
	struct IpoCurve *next,  *prev;
	
	BPoint *bp;
	BezTriple *bezt;

	rctf maxrct, totrct;

	short blocktype, adrcode, vartype;
	short totvert;
	short ipo, extrap;
	short flag, rt;
	float ymin, ymax;
	unsigned int bitmask;
	
	float curval;
	
} IpoCurve;

/* **************** MESH ********************* */

typedef struct MFace {
	ushort v1, v2, v3, v4;
	char puno, mat_nr;
	char edcode, flag;
} MFace;

typedef struct MFaceInt {
	int v1, v2, v3, v4;
	char puno, mat_nr;
	char edcode, flag;
} MFaceInt;

typedef struct MVert {
	float co[3];
	short no[3];
	char flag, mat_nr;
} MVert;

typedef struct MCol {
	char a, r, g, b;
} MCol;

typedef struct MSticky {
	float co[2];
} MSticky;

typedef struct Mesh {
	ID id;

	BoundBox *bb;

	ListBase effect;
	ListBase disp;
	
	Ipo *ipo;
	Key *key;
	Material **mat;

	void *mface, *dface, *tface;
	MVert *mvert;
	MCol *mcol;
	MSticky *msticky;
	struct Mesh *texcomesh;
	float *orco;
	
	OcInfo *oc;		/* niet in file */

	int totvert, totface;
	
	int texflag;
	float loc[3];
	float size[3];
	float rot[3];
	
	short smoothresh, flag;
	short subdiv, totcol;

	float cubemapsize, rtf;
	
} Mesh;

/* **************** LATTICE ********************* */

typedef struct Lattice {
	ID id;
	
	short pntsu, pntsv, pntsw, flag;
	char typeu, typev, typew, type;
	int pad;
	
	BPoint *def;
	
	Ipo *ipo;
	Key *key;
	
} Lattice;

/* **************** TEXT *********************** */

typedef struct TextLine {
	struct TextLine *next, *prev;

	char *line;
	int len, blen;
} TextLine;

typedef struct Text {
	ID id;
	
	char *name;
	
	int flags, nlines;
	
	ListBase lines;
	TextLine *curl, *sell;
	int curc, selc;
	
	unsigned char *undo_buf;
	int undo_pos, pad;
} Text;

/* **************** OBJECT ********************* */

typedef struct Object {
	ID id;

	short type, partype;
	int par1, par2, par3;	/* kunnen vertexnrs zijn */
	
	struct Object *parent, *track;
	struct Ipo *ipo;
	Path *path;
	BoundBox *bb;
	void *data;
	
	ListBase effect;
	ListBase network;
	ListBase disp;
	Material **mat;
	
	/* rot en drot moeten achterelkaar! (transform('r' en 's')) */
	float loc[3], dloc[3], orig[3];
	float size[3], dsize[3];
	float rot[3], drot[3];
	float obmat[4][4];
	float parentinv[4][4];
	float imat[4][4];	/* voor bij render, tijdens simulate, tijdelijk: ipokeys van transform  */
	
	unsigned int lay;				/* kopie van Base */
	short flag;			/* kopie van Base */
	short colbits;		/* nul==van obdata */
	char transflag, ipoflag;
	char trackflag, upflag;
	short ipowin, infoend;	/* ipowin: blocktype laatste ipowindow */
	short infoflag, infostart;
	
	short dupon, dupoff, dupsta, dupend;

	float sf, ctime;
	ushort dfras, rt;		/* tijdens realtime */
	
	char dt, dtx;
	char totcol;	/* kopie van mesh of curve of meta */
	char actcol;
	
	char actscript, totscript;
	ID **scripts;
	
	int padding1;
	short padding2;
} Object;




/* **************** WORLD ********************* */


typedef struct World {
	ID id;
	
	short colormodel, totex;
	short texact, mistype;
	
	float horr, horg, horb, hork;
	float zenr, zeng, zenb, zenk;
	float ambr, ambg, ambb, ambk;
	unsigned int fastcol;
	
	float exposure;
	
	short skytype, mode;
	
	float misi;
	float miststa, mistdist, misthi;
	
	float starr, starg, starb, stark;
	float starsize, starmindist, stardist, starcolnoise;
	
	short dofsta, dofend, dofmin, dofmax;
	int pad;
	
	Ipo *ipo;
	MTex *mtex[8];

} World;


/* **************** SCENE ********************* */

typedef struct Radio {
	short hemires, maxiter;
	short drawtype, flag;			/* bit 0 en 1: limits laten zien */
	short subshootp, subshoote, nodelim, maxsublamp;
	short pama, pami, elma, elmi;	/* patch en elem limits */
	int maxnode;
	float convergence;
	float radfac, gamma;		/* voor afbeelden */
	
} Radio;

typedef struct RenderData {
	
	/* hoe gaat tijd gedefinieerd worden? */
	short cfra, sfra, efra;	/* plaatjes */
	short images, framapto, flag;
	float ctime;			/* hiermee rekenen? */
	float framelen, blurfac;

	short size, maximsize;	/* size in %, max in Kb */
	short xsch, ysch, xasp, yasp;	/* uit buttons */
	short xparts, yparts;
	rctf safety, border;
	
	short winpos, planes, imtype, bufflag, quality, scemode;
	short mode, alphamode, dogamma, osa;
	short frs_sec, pad;
	
	char backbuf[160], pic[160], ftype[160], movie[160];
	
} RenderData;

typedef struct Base {
	struct Base *next, *prev;
	unsigned int lay, selcol;
	int flag;
	short sx, sy;
	Object *object;
	
} Base;

typedef struct Scene {
	ID id;
	Object *camera;
	World *world;
	
	struct Scene *set;
	struct Image *ima;
	
	ListBase base;
	Base *basact;
	
	float cursor[3];
	unsigned int lay;

	/* enkele realtime vars */
	float grav, rtf;
	short maxdrawsector, flag;
	int pad;
	
	void *ed;
	Radio *radio;

	struct RenderData r;

} Scene;

/* ******************************************* */
/* * * * * * * *  SPACE STRUCTS  * * * * * * * */
/* ******************************************* */

typedef struct BGpic {
    Image *ima;
	Tex *tex;
    float xof, yof, size, zoom, blend;
    short xim, yim;
	unsigned int *rect;
} BGpic;

#undef near
#undef far

#define near clipsta
#define far clipend

typedef struct View3D {
	struct View3D *next, *prev;
	int spacetype; 
	float viewmat[4][4];
	float viewinv[4][4];
	float persmat[4][4];
	float persinv[4][4];
	float viewquat[4], dist;
	short persp, view, drawtype, localview;
	int lay, layact;
	short scenelock, around, camzoom, flag;
	
	float lens, grid, near, far;
/* 	float lens, grid, clipsta, clipend; */
	float ofs[3], cursor[3];
	
	Object *camera;
	short mx, my;	/* moeten achter elkaar blijven staan ivm als pointer doorgeven */
	short mxo, myo;

	short pr_xmin, pr_xmax, pr_ymin, pr_ymax;
	short pr_sizex, pr_sizey;
	short gridlines, viewbut;
	float pr_facx, pr_facy;

	BGpic *bgpic;
	struct View3D *localvd;
	
} View3D;

#undef near
#undef far

#define near bnearsub
#define far bfarsub

typedef struct View2D {
	rctf tot, cur;
	rcti vert, hor, mask;
	float min[2], max[2];
	float minzoom, maxzoom;
	short scroll, keeptot;
	short keepaspect, keepzoom;
} View2D;

typedef struct SpaceIpo {
	struct SpaceIpo *next, *prev;
	int spacetype;
	unsigned int rowbut; 
	View2D v2d;
	
	void *editipo;
	ListBase ipokey;
	Ipo *ipo;
	ID *from;

	short totipo, lock;
	short butofs, channel;
	short showkey, blocktype;
	short menunr, rt;
	rctf tot;
} SpaceIpo;

typedef struct SpaceButs {
	struct SpaceButs *next, *prev;
	int spacetype, pad1;
	View2D v2d;
	
	short mainb, menunr;	/* texnr en menunr moeten shorts blijven */
	short lock, mainbo;	
	void *lockpoin;
	
	short texnr;
	char texfrom, rt2;		/* temps */

	/* preview render */
	short rectx, recty;
	unsigned int *rect;
	short cury, modeltype;
	int pad2;
	
} SpaceButs;

typedef struct SpaceSeq {
	struct SpaceSeq *next, *prev;
	int spacetype, pad1;
	View2D v2d;
	
	short mainb, zoom;
	int pad2;
	
} SpaceSeq;

typedef struct SpaceFile {
	struct SpaceFile *next, *prev;
	int spacetype, pad;
	
	struct direntry *filelist;
	int totfile;
	char title[24];
	char dir[160];
	char file[80];
	short type, ofs, flag, sort;
	short maxnamelen, collums;
	char *libfiledata;
	short retval, ipotype;
	short menu, act;
	
	void (*returnfunc)();
	
} SpaceFile;

typedef struct SpaceOops {
	struct SpaceOops *next, *prev;
	int spacetype, pad;
	View2D v2d;
	
	ListBase oops;
	short lock, visiflag, flag, rt;
	void *lockpoin;
	
} SpaceOops;

typedef struct SpaceImage {
	struct SpaceImage *next, *prev;
	int spacetype, pad;
	View2D v2d;
	
	Image *image;
	short mode, zoom;
	short imanr, curtile;
	short xof, yof;
	short flag, rt;
	
} SpaceImage;

typedef struct SpaceText {
	struct SpaceText *next, *prev;
	int spacetype, pad;
	
	Text *text;	

	int top, viewlines;
	short flags, menunr;
	
	int font_id;	
	int lheight;
	
	float pix_per_line;

	rcti txtscroll, txtbar;
} SpaceText;

/* SpaceImaSel in aparte headerfile */
/* SpacePaint in aparte headerfile */


/* ************* GLOBAL ******************* */

#
#
typedef struct dl_GourFace {
	float co[3][3];
	unsigned int col[6];
} dl_GourFace;
#
#
typedef struct FastLamp {
	short type, mode, lay, rt;
	float co[3];
	float vec[3];
	float dist, distkw, att1, att2, spotsi, spotbl, r, g, b;
} FastLamp;
#
#
typedef struct DispList {
    struct DispList *next, *prev;
    short type, flag;
    int parts, nr;
    short col, rt;		/* rt wordt gebruikt door initrenderNurbs */
	float *verts, *nors;
	int *index;
	unsigned int *col1, *col2;
} DispList;


typedef struct UserDef {
	short flag, dupflag;
	int savetime;
	char tempdir[32];
	char fontdir[32];
	char renderdir[32];
	char textudir[32];
	char plugseqdir[32];
	short versions, rt;
} UserDef;


#
#
typedef struct Global {
	
	/* active pointers */
	View3D *vd;
	View2D *v2d;
	SpaceIpo *sipo;
	SpaceButs *buts;
	SpaceImage *sima;
	SpaceOops *soops;
	SpaceText *stext;
	Main *main;
	Scene *scene;				/* denk aan file.c */
	struct bScreen *curscreen;
	Object *obedit;
	
	/* fonts, allocated global data */
	void *font, *fonts, *fontss;
	ListBase mainbase;

	/* strings: lastsaved */
	char ima[160], sce[160], lib[160], psx[160];

	/* totalen */
	short totobj, totlamp, totobjsel, totcurve, totmesh, totmat;
	int totvert, totface, totvertsel, totfacesel;
	int time, cputime;
	
	short machine, afbreek, f, moving, colact, zbuf;
	short qual, background, imagewin, animspeed;
	short version, versionfile, simulf, fields, order, rt;
	
	struct ListBase edve;
	struct ListBase eded;
	struct ListBase edvl;
	
	float textcurs[4][2];
	
	/* realtime */
	struct Sector *cursector, *sectorbuf[SE_MAXBUF];
	Object *lifebuf[LF_MAXBUF];
	short *actvarar;

	int dfra, dfrao;
	short dfras, totsect, maxsect, totlife;

	/* variabelen van frank */
	int renderd;
	int real_sfra, real_efra;
	int	save_over;
	
	/* special versions */
	short special1, special2;
	
} Global;

#include "exports.h"


#endif /* BLENDER_H */

