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

 * PSX Polygon Primitive Definitions : psx.h
 * 
 * ook als libgte.h in de SGI include dir: compatibility
 * 
 */

#ifdef WIN32
typedef unsigned char u_char;
typedef unsigned long u_long;
typedef unsigned short u_short;
#endif

typedef struct  {
	short	m[3][3];	/* 3x3 rotation matrix */
	short flag;			/* NIET OFFICIEEL: ENJI !! */
	long    t[3];		/* transfer vector */
} MATRIX;

typedef struct {		/* long word type 3D vector */
	long	vx, vy;
	long	vz, pad;
} VECTOR;

typedef struct {		/* short word type 3D vector */

	short	vx, vy;
	short	vz, pad;
} SVECTOR;

typedef struct {		/* color type vector */

	u_char	r, g, b, cd;
} CVECTOR;

typedef struct {		/* 2D short vector */
	short vx, vy;
} DVECTOR;


typedef struct {
	SVECTOR v;		/* Object(Local) 3D Vertex 	*/
	VECTOR sxyz;		/* Screen 3D Vertex		*/
	DVECTOR sxy;		/* Screen 2D Vertex		*/
	CVECTOR rgb;		/* Vertex Color Data	 	*/
	short txuv,pad;		/* Texture Mapping Data 	*/
	long chx,chy;		/* Clip Window Data 		*/
} EVECTOR;

typedef struct {
	SVECTOR v;
	u_char uv[2]; 
	u_short pad;	/*@@*/

	CVECTOR c;
	DVECTOR sxy;
	u_long  sz;
} RVECTOR;


typedef struct {
	RVECTOR r01,r12,r20;
	RVECTOR	*r0,*r1,*r2;
	u_long *rtn;
} CRVECTOR3;


typedef struct {
	short x, y;		/* offset point on VRAM */
	short w, h;		/* width and height */
} bRECT;

/*
 * Environment 
 */
typedef struct {
	u_long	tag;
	u_long	code[15];
} DR_ENV;				/* Packed Drawing Environment */
	       
typedef struct {
	bRECT	clip;		/* clip area */
	short	ofs[2];		/* drawing offset */
	bRECT	tw;		/* texture window */
	u_short tpage;		/* texture page */	
	u_char	dtd;		/* dither flag (0:off, 1:on) */
	u_char	dfe;		/* flag to draw on display area (0:off 1:on) */
	u_char	isbg;		/* enable to auto-clear */
	u_char	r0, g0, b0;	/* initital background color */
	DR_ENV	dr_env;		/* reserved */
} DRAWENV;
	       
typedef struct {
	bRECT	disp;		/* display area */
	bRECT	screen;		/* display start point */
	u_char	isinter;	/* interlace 0: off 1: on */
	u_char	isrgb24;	/* RGB24 bit mode */
	u_char	pad0, pad1;	/* reserved */
} DISPENV;


typedef struct {
	unsigned	addr: 24;
	unsigned 	len:   8;
	u_char		r0, g0, b0, code;
} P_TAG;

typedef struct {
	u_char	r0, g0, b0, code;
} P_CODE;

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	short	x1,	y1;
	short	x2,	y2;
} POLY_F3;				/* Flat Triangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	short	x1,	y1;
	short	x2,	y2;
	short	x3,	y3;
} POLY_F4;				/* Flat Quadrangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	u0, v0;	
	u_short	clut;
	short	x1,	y1;
	u_char	u1, v1;	
	u_short	tpage;
	short	x2,	y2;
	u_char	u2, v2;	
	u_short	pad1;
} POLY_FT3;				/* Flat Textured Triangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	u0, v0;	
	u_short	clut;
	short	x1,	y1;
	u_char	u1, v1;	
	u_short	tpage;
	short	x2,	y2;
	u_char	u2, v2;	
	u_short	pad1;
	short	x3,	y3;
	u_char	u3, v3;	
	u_short	pad2;
} POLY_FT4;				/* Flat Textured Quadrangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	r1, g1, b1, pad1;
	short	x1,	y1;
	u_char	r2, g2, b2, pad2;
	short	x2,	y2;
} POLY_G3;				/* Gouraud Triangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	r1, g1, b1, pad1;
	short	x1,	y1;
	u_char	r2, g2, b2, pad2;
	short	x2,	y2;
	u_char	r3, g3, b3, pad3;
	short	x3,	y3;
} POLY_G4;				/* Gouraud Quadrangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	u0, v0;	
	u_short	clut;
	u_char	r1, g1, b1, p1;
	short	x1,	y1;
	u_char	u1, v1;	
	u_short	tpage;
	u_char	r2, g2, b2, p2;
	short	x2,	y2;
	u_char	u2, v2;	
	u_short	pad2;
} POLY_GT3;				/* Gouraud Textured Triangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	u0, v0;	
	u_short	clut;
	u_char	r1, g1, b1, p1;
	short	x1,	y1;
	u_char	u1, v1;	
	u_short	tpage;
	u_char	r2, g2, b2, p2;
	short	x2,	y2;
	u_char	u2, v2;	
	u_short	pad2;
	u_char	r3, g3, b3, p3;
	short	x3,	y3;
	u_char	u3, v3;	
	u_short	pad3;
} POLY_GT4;				/* Gouraud Textured Quadrangle */

/*
 * Line Primitive Definitions
 */
typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	short	x1,	y1;
} LINE_F2;				/* Unconnected Flat Line */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	r1, g1, b1, p1;
	short	x1,	y1;
} LINE_G2;				/* Unconnected Gouraud Line */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	short	x1,	y1;
	short	x2,	y2;
	u_long	pad;
} LINE_F3;				/* 2 connected Flat Line */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	r1, g1, b1, p1;
	short	x1,	y1;
	u_char	r2, g2, b2, p2;
	short	x2,	y2;
	u_long	pad;
} LINE_G3;				/* 2 connected Gouraud Line */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	short	x1,	y1;
	short	x2,	y2;
	short	x3,	y3;
	u_long	pad;
} LINE_F4;				/* 3 connected Flat Line Quadrangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	r1, g1, b1, p1;
	short	x1,	y1;
	u_char	r2, g2, b2, p2;
	short	x2,	y2;
	u_char	r3, g3, b3, p3;
	short	x3,	y3;
	u_long	pad;
} LINE_G4;				/* 3 connected Gouraud Line */

/*
 * Block Fill Primitive Definitions
 */
typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	u_short	x0, 	y0;
	u_short	w, 	h;
} BLK_FILL;				/* Clear Frame Buffer */

/*
 * Sprite Primitive Definitions
 */
typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	u0, v0;	
	u_short	clut;
	short	w,	h;
} SPRT;					/* free size Sprite */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	u0, v0;	
	u_short	clut;
} SPRT_16;				/* 16x16 Sprite */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	u0, v0;	
	u_short	clut;
} SPRT_8;				/* 8x8 Sprite */

/*
 * Tile Primitive Definitions
 */
typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	short	w,	h;
} TILE;					/* free size Tile */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
} TILE_16;				/* 16x16 Tile */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
} TILE_8;				/* 8x8 Tile */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
} TILE_1;				/* 1x1 Tile */

/*
 *  Special Primitive Definitions
 */
typedef struct {
	u_long	tag;
	u_long	code[2];
} DR_MODE;				/* Drawing Mode */

typedef struct {
	u_long	tag;
	u_long	code[2];
} DR_PRIO;				/* Priority */

typedef struct {
	u_long	tag;
	u_long	code[2];
} DR_TWIN;				/* Texture Window */

typedef struct {
	u_long	tag;
	u_long	code[2];
} DR_AREA;				/* Drawing Area */

typedef struct {
	u_long	tag;
	u_long	code[2];
} DR_OFFSET;				/* Drawing Offset */

typedef struct {			/* MoveImage */
	u_long	tag;
	u_long	code;
	u_long	code2;
	short	sx, 	sy;
	short	x0,	y0;
	short	w,	h;
} DR_MOVE;

/*
 *	Font Stream Parameters
 */
#define FNT_MAX_ID	8	/* max number of stream ID */
#define FNT_MAX_SPRT	1024	/* max number of sprites in all streams */

/*
 *	Multi-purpose Sony-TMD primitive
 */
typedef struct {
	u_long	id;
	u_char	r0, g0, b0, p0;		/* Color of vertex 0 */
	u_char	r1, g1, b1, p1;		/* Color of vertex 1 */
	u_char	r2, g2, b2, p2;		/* Color of vertex 2 */
	u_char	r3, g3, b3, p3;		/* Color of vertex 3 */
	u_short	tpage, clut;		/* texture page ID, clut ID */
	u_char	u0, v0, u1, v1;		/* texture corner point */
	u_char	u2, v2, u3, v3;

	/* independent vertex model */
	SVECTOR	x0, x1, x2, x3;		/* 3D corner point */
	SVECTOR	n0, n1, n2, n3;		/* 3D corner normal vector */

	/* Common vertex model */
	SVECTOR	*v_ofs;			/* offset to vertex database */
	SVECTOR	*n_ofs;			/* offset to normal database */

	u_short	vert0, vert1; 		/* index of vertex */
	u_short	vert2, vert3;
	u_short	norm0, norm1; 		/* index of normal */
	u_short	norm2, norm3;


} TMD_PRIM;

/*
 *	Multi-purpose TIM image
 */
typedef struct {
	u_long  mode;		/* pixel mode */
	bRECT	*crect;		/* CLUT rectangle on frame buffer */
	u_long	*caddr;		/* CLUT address on main memory */
	bRECT	*prect;		/* texture image rectangle on frame buffer */
	u_long	*paddr;		/* texture image address on main memory */
} TIM_IMAGE;


/*
 * Primitive Handling Macros
 */
#define setlen( p, _len) 	(((P_TAG *)(p))->len  = (u_char)(_len))
#define setaddr(p, _addr)	(((P_TAG *)(p))->addr = (u_long)(_addr))
#define setcode(p, _code)	(((P_TAG *)(p))->code = (u_char)(_code))

#define getlen(p)    		(u_char)(((P_TAG *)(p))->len)
#define getcode(p)   		(u_char)(((P_TAG *)(p))->code)
#define getaddr(p)   		(u_long)(((P_TAG *)(p))->addr)

#define nextPrim(p)  		(void *)((((P_TAG *)(p))->addr)|0x80000000)
#define isendprim(p) 		((((P_TAG *)(p))->addr)==0xffffff)

#define addPrim(ot, p)		setaddr(p, getaddr(ot)), setaddr(ot, p)
#define addPrims(ot, p0, p1)	setaddr(p1, getaddr(ot)),setaddr(ot, p0)

#define catPrim(p0, p1)		setaddr(p0, p1)
#define termPrim(p)		setaddr(p, 0xffffffff)

#define setSemiTrans(p, abe) \
	((abe)?setcode(p, getcode(p)|0x02):setcode(p, getcode(p)&~0x02))

#define setShadeTex(p, tge) \
	((tge)?setcode(p, getcode(p)|0x01):setcode(p, getcode(p)&~0x01))

	/*  ENJI!! */
#define GetGraphType()		0

#define getTPage(tp, abr, x, y) 					\
	((GetGraphType()==1||GetGraphType()==2)?			\
	 ((((tp)&0x3)<<9)|(((abr)&0x3)<<7)|(((y)&0x300)>>3)|(((x)&0x3ff)>>6)):\
	 ((((tp)&0x3)<<7)|(((abr)&0x3)<<5)|(((y)&0x100)>>4)|(((x)&0x3ff)>>6)| \
	 (((y)&0x200)<<2)))

#define getClut(x, y) \
	((y<<6)|((x>>4)&0x3f))




/*	Primitive 	Lentgh		Code				*/
/*--------------------------------------------------------------------	*/
/*									*/
#define setPolyF3(p)	setlen(p, 4),  setcode(p, 0x20)
#define setPolyFT3(p)	setlen(p, 7),  setcode(p, 0x24)
#define setPolyG3(p)	setlen(p, 6),  setcode(p, 0x30)
#define setPolyGT3(p)	setlen(p, 9),  setcode(p, 0x34)
#define setPolyF4(p)	setlen(p, 5),  setcode(p, 0x28)
#define setPolyFT4(p)	setlen(p, 9),  setcode(p, 0x2c)
#define setPolyG4(p)	setlen(p, 8),  setcode(p, 0x38)
#define setPolyGT4(p)	setlen(p, 12), setcode(p, 0x3c)

#define setSprt8(p)	setlen(p, 3),  setcode(p, 0x74)
#define setSprt16(p)	setlen(p, 3),  setcode(p, 0x7c)
#define setSprt(p)	setlen(p, 4),  setcode(p, 0x64)

#define setTile1(p)	setlen(p, 2),  setcode(p, 0x68)
#define setTile8(p)	setlen(p, 2),  setcode(p, 0x70)
#define setTile16(p)	setlen(p, 2),  setcode(p, 0x78)
#define setTile(p)	setlen(p, 3),  setcode(p, 0x60)
#define setBlockFill(p)	setlen(p, 3),  setcode(p, 0x02)
#define setLineF2(p)	setlen(p, 3),  setcode(p, 0x40)
#define setLineG2(p)	setlen(p, 4),  setcode(p, 0x50)
#define setLineF3(p)	setlen(p, 5),  setcode(p, 0x48),(p)->pad   = 0x55555555
#define setLineG3(p)	setlen(p, 7),  setcode(p, 0x58),(p)->pad   = 0x55555555
#define setLineF4(p)	setlen(p, 6),  setcode(p, 0x4c),(p)->pad   = 0x55555555
#define setLineG4(p)	setlen(p, 9),  setcode(p, 0x5c),(p)->pad   = 0x55555555
#define setDrawMove(p)	setlen(p, 5),  setcode(p, 0x01),(p)->code2 = 0x80000000
	

