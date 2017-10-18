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

#ifndef IFF_H

#define IFF_H


#include <sys/types.h>
#include "avi.h"
#include "util.h"
#include "floatpatch.h"

#ifdef __sgi
#include <dmedia/moviefile.h>
#else
/* #define MVid char */
typedef void * MVid;
#define DMparams char
#define DMinterlacing char
#define	DMorientation char 
#endif

#define CAT  MAKE_ID('C','A','T',' ')
#define FORM MAKE_ID('F','O','R','M')
#define ILBM MAKE_ID('I','L','B','M')
#define BMHD MAKE_ID('B','M','H','D')
#define CMAP MAKE_ID('C','M','A','P')
#define CAMG MAKE_ID('C','A','M','G')
#define BODY MAKE_ID('B','O','D','Y')
#define RIFF MAKE_ID('R', 'I', 'F', 'F')

#define ANIM MAKE_ID('A','N','I','M')
#define ADAT MAKE_ID('A','D','A','T')
#define CODE MAKE_ID('C','O','D','E')
#define ANHD MAKE_ID('A','N','H','D')
#define DLTA MAKE_ID('D','L','T','A')
#define BLCK MAKE_ID('B','L','C','K')

#define IMAG MAKE_ID('I','M','A','G')
#define IHDR MAKE_ID('I','H','D','R')
#define IDAT MAKE_ID('I','D','A','T')
#define PLTE MAKE_ID('P','L','T','E')

#define DDAT MAKE_ID('D','D','A','T')

#define MAXRUN 126
#define MAXDAT 126
#define IFFNOP 128

#define IB_rect			(1 << 0)
#define IB_planes		(1 << 1)
#define IB_cmap			(1 << 2)
#define IB_char			(1 << 3)

#define IB_vert			(1 << 4)
#define IB_word			(1 << 5)
#define IB_freem		(1 << 6)
#define IB_test			(1 << 7)

#define IB_ttob			(1 << 8)
#define IB_subdlta		(1 << 9)
#define IB_charonlong	(1 << 10)
#define IB_fields		(1 << 11)
#define IB_yuv			(1 << 12)
#define IB_zbuf			(1 << 13)

#define AMI	    (1 << 31)
#define CDI	    (1 << 30)
#define Anim	(1 << 29)
#define TGA	    (1 << 28)
#define JPG		(1 << 27)
#define TIM	    (1 << 26)

#define TIM_CLUT	(010)
#define TIM_4		(TIM | TIM_CLUT | 0)
#define TIM_8		(TIM | TIM_CLUT | 1)
#define TIM_16		(TIM | 2)
#define TIM_24		(TIM | 3)

#define RAWTGA	(TGA | 1)

#define JPG_STD	(JPG | (0 << 8))
#define JPG_VID	(JPG | (1 << 8))
#define JPG_JST	(JPG | (2 << 8))
#define JPG_MAX	(JPG | (3 << 8))
#define JPG_MSK	(0xffffff00)

#define AM_ham	    (0x0800 | AMI)
#define AM_hbrite   (0x0080 | AMI)
#define AM_lace	    (0x0004 | AMI)
#define AM_hires    (0x8000 | AMI)
#define AM_hblace   (AM_hbrite | AM_lace)
#define AM_hilace   (AM_hires | AM_lace)
#define AM_hamlace  (AM_ham | AM_lace)

#define RGB888	1
#define RGB555	2
#define DYUV	3
#define CLUT8	4
#define CLUT7	5
#define CLUT4	6
#define CLUT3	7
#define RL7	8
#define RL3	9
#define MPLTE	10

#define DYUV1	0
#define DYUVE	1

#define CD_rgb8		(RGB888 | CDI)
#define CD_rgb5		(RGB555 | CDI)
#define CD_dyuv		(DYUV | CDI)
#define CD_clut8	(CLUT8 | CDI)
#define CD_clut7	(CLUT7 | CDI)
#define CD_clut4	(CLUT4 | CDI)
#define CD_clut3	(CLUT3 | CDI)
#define CD_rl7		(RL7 | CDI)
#define CD_rl3		(RL3 | CDI)
#define CD_mplte	(MPLTE | CDI)

#define C233	1
#define YUVX	2
#define HAMX	3
#define TANX	4

#define AN_c233			(Anim | C233)
#define AN_yuvx			(Anim | YUVX)
#define AN_hamx			(Anim | HAMX)
#define AN_tanx			(Anim | TANX)

#define IS_amiga(x)		(x->ftype & AMI)
#define IS_ham(x)		((x->ftype & AM_ham) == AM_ham)
#define IS_hbrite(x)	((x->ftype & AM_hbrite) == AM_hbrite)
#define IS_lace(x)		((x->ftype & AM_lace) == AM_lace)
#define IS_hires(x)		((x->ftype & AM_hires) == AM_hires)
#define IS_hblace(x)	((x->ftype & AM_hblace) == AM_hblace)
#define IS_hilace(x)	((x->ftype & AM_hilace) == AM_hilace)
#define IS_hamlace(x)	((x->ftype & AM_hamlace) == AM_hamlace)

#define IS_cdi(x)		(x->ftype & CDI)
#define IS_rgb8(x)		(x->ftype == CD_rgb8)
#define IS_rgb5(x)		(x->ftype == CD_rgb5)
#define IS_dyuv(x)		(x->ftype == CD_dyuv)
#define IS_clut8(x)		(x->ftype == CD_clut8)
#define IS_clut7(x)		(x->ftype == CD_clut7)
#define IS_clut4(x)		(x->ftype == CD_clut4)
#define IS_clut3(x)		(x->ftype == CD_clut3)
#define IS_rl7(x)		(x->ftype == CD_rl7)
#define IS_rl3(x)		(x->ftype == CD_rl3)
#define IS_mplte(x)		(x->ftype == CD_mplte)

#define IS_anim(x)		(x->ftype & Anim)
#define IS_c233(x)		(x->ftype == AN_c233)
#define IS_c233d4(x)	(x->ftype == AN_c233d4)
#define IS_hamx(x)		(x->ftype == AN_hamx)
#define IS_yuvx(x)		(x->ftype == AN_yuvx)
#define IS_tanx(x)		(x->ftype == AN_tanx)

#define IS_tga(x)		(x->ftype & TGA)
#define IS_tim(x)		(x->ftype & TIM)

#define IMAGIC 	0732
#define IS_iris(x)		(x->ftype == IMAGIC)

#define IS_jpg(x)		(x->ftype & JPG)
#define IS_stdjpg(x)	((x->ftype & JPG_MSK) == JPG_STD)
#define IS_vidjpg(x)	((x->ftype & JPG_MSK) == JPG_VID)
#define IS_jstjpg(x)	((x->ftype & JPG_MSK) == JPG_JST)
#define IS_maxjpg(x)	((x->ftype & JPG_MSK) == JPG_MAX)

#define AN_INIT an_stringdec = stringdec; an_stringenc = stringenc;

#ifdef AMIGA
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define fchmod(a,b)
#define fceil(x)	(-floor(-x))
#endif

typedef struct ImBuf{
	short	x,y;		/* breedte in pixels, hoogte in scanlines */
	short	skipx;		/* breedte in ints om bij volgende scanline te komen */
	uchar	depth;		/* actieve aantal bits/bitplanes */
	uchar	cbits;		/* aantal active bits in cmap */
	ushort	mincol;
	ushort	maxcol;
	int	type;		/* 0=abgr, 1=bitplanes */
	int	ftype;
	uint	*cmap;		/* int array van kleuren */
	uint	*rect;		/* databuffer */
	uint	**planes;	/* bitplanes */
	uchar	*chardata;	/* voor cdi-compressie */
	int	flags;
	int	mall;		/* wat is er intern gemalloced en mag weer vrijgegeven worden */
	short	xorig, yorig;
	char	name[127];
	char	namenull;
	int	userflags;
	int	*zbuf;
	void	*userdata;
}ImBuf;

struct BitMapHeader
{
	ushort	w, h;		/* in pixels */
	ushort	x, y;
	char	nPlanes;
	char	masking;
	char	compression;
	char	pad1;
	ushort	transparentColor;
	char	xAspect, yAspect;
	short	pageWidth, pageHeight;
};

struct Yuv
{
	uchar	y,u,v;
};

struct Ihdr
{
	ushort		w,lsize,h,model,depth;
	uchar		dyuvkind;
	struct Yuv	yuvstart;
};

typedef struct Adat
{
	ushort w, h;
	ushort type;
	ushort xorig, yorig;
	ushort pad;
	float gamma;
	float distort;
}Adat;

typedef struct TARGA 
{
	unsigned char numid;	
	unsigned char maptyp;
	unsigned char imgtyp;	
	short maporig;
	short mapsize;
	unsigned char mapbits;
	short xorig;
	short yorig;
	short xsize;
	short ysize;
	unsigned char pixsize;
	unsigned char imgdes;
} TARGA;

struct ErrEntry
{
	uint error;
	uchar b,g,r;
};

struct MedCut
{
	uint toterr;
	short num;
	uchar minb,maxb;
	uchar ming,maxg;
	uchar minr,maxr;
};

struct Rgb
{
	uchar	r,g,b;
};

#define ANIM_NONE		(0)
#define ANIM_SEQUENCE	(1 << 0)
#define ANIM_DIR		(1 << 1)
#define ANIM_ANIM5		(1 << 2)
#define ANIM_TGA		(1 << 3)
#define ANIM_MOVIE		(1 << 4)
#define ANIM_MDEC		(1 << 5)
#define ANIM_AVI		(1 << 6)

#define ANIM_ONCE		(1 << 31)
#define ANIM_DFLT		(ANIM_SEQUENCE | ANIM_ANIM5 | ANIM_ONCE | ANIM_MOVIE | ANIM_MDEC)

#define ANIM5_MMAP		0
#define ANIM5_MALLOC	1
#define ANIM5_SNGBUF	2
#define ANIM5_XOR		4

#define PLAY_INTERVAL	0	/* voor begin = 0 */
#define PLAY_LOOP		1
#define PLAY_PINGPONG	2
#define PLAY_ONCE		3	/* voor begin = 1e plaatje */

typedef struct Anim5Delta {
	struct Anim5Delta * next, * prev;
	void * data;
	int type;
}Anim5Delta;

typedef struct Mdec{
	ushort	magic, version;
	ushort	sizex;
	ushort	sizey;
	ushort	frames;
	ushort	flags;

	ushort	current;
	ushort	audio_size;
	uchar	ytab[64];
	uchar	ctab[64];	
	int		fd;
	int		padint[16];
	uint 	offset[1];
}Mdec;

typedef struct SliceData
{
	unsigned quant	:6;
	unsigned rlsize :15;
	unsigned hufsize:11;
}SliceData;

struct anim {
	int flags, ib_flags;
	int curtype;
	int curposition;	/* index  0 = 1e,  1 = 2e, enz. */
	int playmode;		/* PINGPONG, LOOP, ONCE */
	int duration;
	int x, y;
	
		/* voor op nummer */
	char name[256];
		/* voor sequence */
	char first[256];
	
		/* anim5 */
	struct ListBase	anim5base;
	void		* anim5mmap;
	int		anim5len;
	struct Anim5Delta *anim5curdlta;
	void		(*anim5decode)(struct ImBuf *, uchar *);
	int		anim5flags;
	
		/* movie */
	MVid movie;
	MVid track;
	DMparams *params;
	DMorientation orientation; 
	size_t framesize;
	DMinterlacing interlacing;
	
		/* data */
	ImBuf * ibuf1, * ibuf2;
	
		/* avi */
	AviMovie *avi;

		/* mdec */

    Mdec	* mdec;
	int		mdeclen;
};


typedef struct Anhd{
	uchar type, mask;
	ushort w, h;
	ushort x, y;
	ushort abs16, abs, reala6, real;
	uchar interleave, pad0;
	ushort bits16, bits;
	uchar pad[16];
}Anhd;

#define camg ftype

#define LI_rect		IB_rect
#define LI_planes	IB_planes
#define LI_kcmap	IB_cmap
#define LI_cmap		IB_cmap
#define LI_char		IB_char
#define LI_word		IB_word
#define LI_freem	IB_freem
#define LI_test		IB_test

#define SI_rect		IB_rect
#define SI_planes	IB_planes
#define SI_kcmap	IB_cmap
#define SI_cmap		IB_cmap
#define SI_char		IB_char
#define SI_vert		IB_vert

#define initmoviepointers() \
{ \
	extern DMboolean (*IsMovie)(const char * name); \
	extern DMstatus  (* OpenMovie)(const char * name, int oflag, MVid * movie ); \
	extern DMstatus  (* CloseMovie)( MVid movie ); \
	extern DMstatus  (* ReadFrames)( MVid track, MVframe frameIndex, MVframe frameCount, size_t bufferSize, void* buffer ); \
	extern DMstatus  (* FindTrack)( MVid movie, DMmedium medium, MVid* returnTrack ); \
	extern DMparams* (* GetParams)( MVid movieOrTrack ); \
	extern size_t (* ImageFrameSize)(const DMparams* params ); \
	extern int (* ParamsGetInt)( const DMparams* params, const char* paramName ); \
	extern int (* ParamsGetEnum)( const DMparams* params, const char* paramName ); \
	extern double (* ParamsGetFloat)( const DMparams* params, const char* paramName ); \
	extern MVframe (* GetTrackLength)( MVid track ); \
	extern int (* GetErrno)(); \
	extern const char * (* GetErrorStr)(int); \
	\
	IsMovie = mvIsMovieFile; \
	OpenMovie = mvOpenFile; \
	CloseMovie = mvClose; \
	ReadFrames = mvReadFrames; \
	FindTrack = mvFindTrackByMedium; \
	GetParams = mvGetParams; \
	ImageFrameSize = dmImageFrameSize; \
	ParamsGetInt = dmParamsGetInt; \
	ParamsGetEnum = dmParamsGetEnum; \
	ParamsGetFloat = dmParamsGetFloat; \
	GetTrackLength = mvGetTrackLength; \
	GetErrno = mvGetErrno; \
	GetErrorStr = mvGetErrorStr; \
}

extern float rgbyuv[4][4];
extern float yuvrgb[4][4];
extern float rgbbeta[4][4];
extern int IB_verbose;

extern struct ImBuf *allocImBuf(short,short,uchar,uint,uchar);
extern struct ImBuf *dupImBuf(struct ImBuf *);
extern void freeImBuf(struct ImBuf*);

extern uint **copyplanelist(struct ImBuf *);
extern void bptolscanl(uint *buf, int size, uint **list, int nobp, int offset);
extern void bptolong(struct ImBuf *ibuf);
extern void ltobpscanl(uint *rect, int x, uint **list, int nobp, int offset);
extern void longtobp(struct ImBuf *ibuf);

extern short makedelta(char *pic1,char *pic2,char *delta);
extern short addcmapImBuf(struct ImBuf *);
extern int colcspace(int col, float mat[][4]);
/*
extern void clearrect(struct ImBuf *);
extern void addbpl(struct ImBuf *,struct ImBuf *);
extern void subbpl(struct ImBuf *,struct ImBuf *);
extern void cspace(struct ImBuf *,float *);
extern void swapbitsbpl(struct ImBuf *);
extern void deswapbitsbpl(struct ImBuf *);
extern void quantize24(struct ImBuf *);
extern void swapbitsrect(struct ImBuf *);
extern void dequantize24(struct ImBuf *);
extern void filter(struct ImBuf *);
extern void bptolong(struct ImBuf *);
extern void longtobp(struct ImBuf *);
extern void sub(struct ImBuf *,struct ImBuf *);
*/

extern short converttocmap(struct ImBuf* ibuf);
extern short quantbpl(struct ImBuf *);
extern short saveiff(struct ImBuf *,char *,int);

extern short encodebodyh(struct ImBuf *ibuf, int file);
extern short encodebodyv(struct ImBuf *ibuf, int file);
extern struct ImBuf *loadamiga(int *iffmem,int flags);

extern struct ImBuf *loadiffmem(int *,int);
extern struct ImBuf *loadifffile(int,int);
extern struct ImBuf *loadiffname(char *,int);
extern struct ImBuf *testiffname(char *,int);
extern struct ImBuf *onehalf(struct ImBuf *);
extern struct ImBuf *onethird(struct ImBuf *);
extern struct ImBuf *halflace(struct ImBuf *);
extern struct ImBuf *half_x(struct ImBuf *);
extern struct ImBuf *half_y(struct ImBuf *);
extern struct ImBuf *double_x(struct ImBuf *);
extern struct ImBuf *double_y(struct ImBuf *);
extern struct ImBuf *double_fast_x(struct ImBuf *);
extern struct ImBuf *double_fast_y(struct ImBuf *);
extern int ispic(char *);
extern void floyd(struct ImBuf *, short, short);
extern void dit2(struct ImBuf *, short, short);
extern void dit3(struct ImBuf *, short, short);
extern void dit4(struct ImBuf *, short, short);
extern void dit0(struct ImBuf *, short, short);
extern void (*ditherfunc)(struct ImBuf *, short, short);
extern struct ImBuf *scaleImBuf(struct ImBuf *, short, short);
extern struct ImBuf *scalefastImBuf(struct ImBuf *, short, short);
extern struct ImBuf *scalefieldImBuf(struct ImBuf *, short, short);
extern struct ImBuf *scalefastfieldImBuf(struct ImBuf *, short, short);

extern struct ImBuf * anim_nextpic(struct anim * anim);
extern struct ImBuf * anim_getnext(struct anim * anim);
extern struct ImBuf * anim_absolute(struct anim * anim, int position);
extern struct anim * open_anim(char * name, int flags, int ib_flags);
extern void close_anim(struct anim * anim);

extern long getdither();

#endif

