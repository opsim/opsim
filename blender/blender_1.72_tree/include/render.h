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


/* render.h
 *
 *
 * maart 95
 *
 */


#ifndef RENDER_H
#define RENDER_H

/* For #undefs of stupid windows defines */
#ifdef WIN32
#include "winstuff.h"
#endif

#include "effect.h"		/* for PartEff */

#define BRICON		Tin= (Tin-0.5)*tex->contrast+tex->bright-0.5; \
					if(Tin<0.0) Tin= 0.0; else if(Tin>1.0) Tin= 1.0;

#define BRICONRGB	Tr= tex->rfac*((Tr-0.5)*tex->contrast+tex->bright-0.5); \
					if(Tr<0.0) Tr= 0.0; else if(Tr>1.0) Tr= 1.0; \
					Tg= tex->gfac*((Tg-0.5)*tex->contrast+tex->bright-0.5); \
					if(Tg<0.0) Tg= 0.0; else if(Tg>1.0) Tg= 1.0; \
					Tb= tex->bfac*((Tb-0.5)*tex->contrast+tex->bright-0.5); \
					if(Tb<0.0) Tb= 0.0; else if(Tb>1.0) Tb= 1.0;

#define MAXVERT (2<<20)
#define MAXVLAK (2<<20)


#ifdef __BeOS

	/* dirty hack: pointers are negative, indices positive */
	/* pointers are not converted */
	
#define IS_A_POINTER_CODE(a)		((a)<0)
#define POINTER_FROM_CODE(a)		((void *)((a)))
#define POINTER_TO_CODE(a)			((long)(a))


#else

	/* dirty hack: pointers are negative, indices positive */
	/* pointers should be converted to positive numbers */
	
#define IS_A_POINTER_CODE(a)		((a)<0)
#define POINTER_FROM_CODE(a)		((void *)(-(a)))
#define POINTER_TO_CODE(a)			(-(long)(a))

#endif

typedef struct Branch
{
	struct Branch *b[8];
} Branch;

typedef struct HaloRen
{	
	float alfa, xs, ys, rad, radsq, sin, cos, co[3], no[3];
	uint zs, zd;
	short miny, maxy;
	char hard, b, g, r;
	char starpoints, add, type, tex;
	char linec, ringc, seed, flarec;
	float hasize;
	int pixels;
	
	Material *mat;
} HaloRen;

typedef struct LampRen
{
	float xs, ys, dist;
	float co[3];
	short type, mode;
	float r, g, b;
	float energy, haint;
	int lay;
	float spotsi,spotbl;
	float vec[3];
	float xsp, ysp, distkw, inpr;
	float halokw, halo;
	float ld1,ld2;
	struct ShadBuf *shb;
	float imat[3][3];
	float spottexfac;
	float sh_invcampos[3], sh_zfac;	/* sh_= spothalo */
	
	struct LampRen *org;
	MTex *mtex[8];
	
} LampRen;

typedef struct FaceStrip
{
	float n[3];
} FaceStrip;

typedef struct Node
{
	struct VlakRen *v[7];
	struct Node *next;
} Node;

typedef struct Osa
{
	float dxco[3], dyco[3];
	float dxlo[3], dylo[3], dxgl[3], dygl[3], dxuv[3], dyuv[3];
	float dxref[3], dyref[3], dxorn[3], dyorn[3];
	float dxno[3], dyno[3], dxview, dyview;
	float dxlv[3], dylv[3];
	float dxwin[3], dywin[3];
	float dxsticky[3], dysticky[3];
} Osa;

typedef struct Part
{
	struct Part *next, *prev;
	uint *rect;
	short x, y;
} Part;

typedef struct PixStr
{
	struct PixStr *next;
	int vlak0, vlak;
	uint z;
	uint mask;
	short aantal, ronde;
} PixStr;

typedef struct PixStrMain
{
	struct PixStr *ps;
	struct PixStrMain *next;
} PixStrMain;
	
typedef struct QStrip
{
	struct QStrip *next, *prev;
	short pntsu, pntsv;
	short flagu, flagv;
	int firstface;
	Material *mat;
	struct VertStrip *verts;
	struct FaceStrip *faces;
} QStrip;

typedef struct ShadBuf {
	short samp, shadhalostep;
	float persmat[4][4];
	float viewmat[4][4];
	float winmat[4][4];
	float jit[25][2];
	float d,far,pixsize,soft;
	int co[3];
	int size,bias;
	ulong *zbuf;
	char *cbuf;
} ShadBuf;

typedef struct VertRen
{
	float co[3];
	float n[3];
	float ho[4];
	float *orco;
	float *sticky;
	void *svert;			/* smooth vert, only used during initrender */
	short clip, texofs;		/* texofs= flag */
} VertRen;

typedef struct VertStrip
{
	float co[3];
	float n[3];
	float ho[4];
	short clip, flag;  
} VertStrip;

typedef struct VlakRen
{
	struct VertRen *v1, *v2, *v3, *v4;
	float n[3], len;
	Material *mat;
	MFace *mface;
	uint *vcol;
	char snproj, puno;
	char flag, ec;
	uint lay;
} VlakRen;



typedef struct Render
{
	float co[3];
	float lo[3], gl[3], uv[3], ref[3], orn[3], winco[3], sticky[3], vcol[3];
	float itot, i, ic, rgb, norm;
	float vn[3], view[3], *vno, refcol[4];

	float grvec[3], inprz, inprh;
	float imat[3][3];

	float viewmat[4][4], viewinv[4][4];
	float persmat[4][4], persinv[4][4];
	float winmat[4][4];
	
	short flag, osatex, osa, rt;
	short xstart, xend, ystart, yend, afmx, afmy, rectx, recty;
	float near, far, ycor, zcor, pixsize, viewfac;
	
	RenderData r;
	World wrld;
	ListBase parts;
	
	int totvlak, totvert, tothalo, totlamp;
	
	VlakRen *vlr;
	int vlaknr;
	
	Material *mat, *matren;
	LampRen **la;
	VlakRen **blovl;
	VertRen **blove;
	HaloRen **bloha;
	
	uint *rectaccu, *rectz, *rectf1, *rectf2;
	uint *rectot, *rectspare;
	/* for 8 byte systems! */
	long *rectdaps;
	
	short win, winpos, winx, winy, winxof, winyof;
	short winpop, displaymode, sparex, sparey;
	
	Image *backbuf, *frontbuf;
	
} Render;

struct halosort {
	HaloRen *har;
	uint z;
};

extern Render R;
extern Osa O;


	/* initrender.c */
extern    void add_cmapcode(void);
extern    void add_render_lamp(Object *ob, int doshadbuf);
extern    void add_to_blurbuf(int blur);
extern    HaloRen *addhalo(int nr);
extern    void addparttorect(short nr, Part *part);
extern    VertRen *addvert(int nr);
extern    VlakRen *addvlak(int nr);
extern    void animrender(void);
extern    float  calc_weight(float *weight, int i, int j);
extern    int contrpuntnormr(float *n, float *puno);
extern    void defaultlamp(void);
extern    void end_timer(int *real, int *cpu);
extern    void exit_render_stuff(void);
extern    void free_filt_mask(void);
extern    void free_render_data(void);
extern    void freeroteerscene(void);
extern    Material *give_render_material(Object *ob, int nr);
extern    void info_calc_drot(Object *ob, float *co);
extern    int info_calc_schermco(float *vec, float *sco);
extern    void info_file(Object *ob);
extern    void init_def_material(void);
extern    void init_filt_mask(void);
extern    void init_render_curve(Object *ob);
extern    void init_render_data(void);
extern    void init_render_jit(int nr);
extern    void init_render_mball(Object *ob);
extern    void init_render_mesh(Object *ob);
extern    void init_render_object(Object *ob);
extern    void init_render_surf(Object *ob);
extern    HaloRen *inithalo(Material *ma, float *vec, float *vec1, float *orco, float hasize, float vectsize);
extern    void initjit(float *jit, int num);
extern    void initparts(void);
extern    void initrender(void);
extern    HaloRen *initstar(float *vec, float hasize);
extern    void jitterate1(float *jit1, float *jit2, int num, float rad1);
extern    void jitterate2(float *jit1, float *jit2, int num, float rad2);
extern    void make_existing_file(char *name);
extern    void make_info_file_string(char *string, int frame);
extern    void make_render_halos(Object *ob, Mesh *me, Material *ma, float *extverts);
extern    void make_stars(int wire);
extern    void normalenrender(int startvert, int startvlak);
extern    int panotestclip(float *v);
extern    void render(void);  /* hierbinnen de PART en FIELD lussen */
extern    void render_particle_system(Object *ob, PartEff *paf);
extern    void roteerscene(void);
extern    void schrijfplaatje(char *name);
extern    void setpanorot(int part);
extern    short setpart(short nr); /* return 0 als geen goede part */
extern    void setwindowclip(int mode, int jmode);
extern    void setzbufvlaggen( void (*projectfunc)() );
extern    void sort_halos(void);
extern    void split_u_renderfaces(int startvlak, int startvert, int usize, int plek, int cyclu);
extern    void split_v_renderfaces(int startvlak, int startvert, int usize, int vsize, int plek, int cyclu, int cyclv);
extern    void start_timer(void);
extern    int verghalo(struct halosort *x1, struct halosort *x2);
extern    void write_image(char *name);


/* ******* previewrender.c */
extern void display_pr_scanline(uint *rect, int recty);
extern void draw_tex_crop(Tex *tex);
extern void halo_preview_pixel(HaloRen *har, int startx, int endx, int y, char *rect);
extern void init_previewhalo(HaloRen *har, Material *mat);
extern void lamp_preview_pixel(LampRen *la, int x, int y, char *rect);
extern void preview_changed(short win);
extern uint previewback(int type, int x, int y);
extern void previewdraw(void);
extern void previewflare(HaloRen *har, uint *rect);
extern void previewrender(void);
extern void set_previewrect(int xmin, int ymin, int xmax, int ymax);
extern void shade_preview_pixel(int x, int y, char *rect, int smooth);
extern void sky_preview_pixel(float lens, int x, int y, char *rect);
extern void texture_preview_pixel(Tex *tex, int x, int y, char *rect);


	/* render.c */
extern      float CookTorr(float *n, float *l, float *v, int hard);
extern      void add_filt_mask(uint mask, ushort *col, uint *rb1, uint *rb2, uint *rb3);
extern      void add_halo_flare(void);
extern      void addps(long *rd, int vlak, uint z, short ronde);
extern      PixStr *addpsmain(void);
extern      uint calchalo_z(HaloRen *har, uint zz);
extern      int count_mask(ushort mask);
extern      float count_maskf(ushort mask);
extern      void freeps(void);
extern      void halovert(void);
extern      float mistfactor(float *co);	/* dist en hoogte, return alpha */
extern      void renderflare(HaloRen *har);
extern      void renderhalo(HaloRen *har);	/* postprocess versie */
extern      void renderspothalo(ushort *col);

extern      void scanlinehaloPS(uint *rectz, long *rectdelta, uint *rectt, short ys);
extern      void scanlinesky(char *rect, int y);
extern      void shadehalo(HaloRen *har, char *col, uint zz, float dist, float xn, float yn, short flarec);
extern      void shadelamplus(void);
extern      void shadepixel(float x, float y, int vlaknr);
extern      void sky(char *col);
extern      void spothalo(struct LampRen *lar, float *view, float *intens);
extern      void zbufshade(void);
extern      void zbufshadeDA(void);	/* Delta Accum Pixel Struct */

/* ******* shadbuf.c */
extern     void initshadowbuf(LampRen *lar, Lamp *la, float mat[][4]);
extern     void makeshadowbuf(LampRen *lar);
extern     float testshadowbuf(struct ShadBuf *shb, float inp);	
extern	   float shadow_halo(LampRen *lar, float *p1, float *p2);

	/* texture.c */
extern     ColorBand *add_colorband(void);
extern     MTex *add_mtex(void);
extern     PluginTex *add_plugin_tex(char *str);
extern     Tex *add_texture(char *name);
extern     void autotexname(Tex *tex);
extern     int blend(Tex *tex, float *texvec);
extern     int clouds(Tex *tex, float *texvec);
extern     Tex *copy_texture(Tex *tex);
extern     int cubemap(MTex *mtex, float x, float y, float z, float *adr1, float *adr2);
extern     int cubemap_glob(MTex *mtex, float x, float y, float z, float *adr1, float *adr2);
extern     int cubemap_ob(MTex *mtex, float x, float y, float z, float *adr1, float *adr2);
extern     void default_mtex(MTex *mtex);
extern     void default_tex(Tex *tex);
extern     void do_2d_mapping(MTex *mtex, float *t, float *dxt, float *dyt);
extern     int do_colorband(ColorBand *coba);
extern     void do_halo_tex(HaloRen *har, float xn, float yn, float *colf);
extern     void do_lamp_tex(LampRen *la, float *lavec);
extern     void do_material_tex(void);
extern     void do_sky_tex(void);
extern     void end_render_texture(Tex *tex);
extern     void end_render_textures(void);
extern     void externtex(MTex *mtex, float *vec);
extern     void externtexcol(MTex *mtex, float *orco, char *col);
extern     void free_plugin_tex(PluginTex *pit);
extern     void free_texture(Tex *tex);
extern     void init_render_texture(Tex *tex);
extern     void init_render_textures();
extern     int magic(Tex *tex, float *texvec);
extern     void make_local_texture(Tex *tex);
extern     int marble(Tex *tex, float *texvec);
extern     int multitex(Tex *tex, float *texvec, float *dxt, float *dyt);
extern     void open_plugin_tex(PluginTex *pit);
extern     int plugintex(Tex *tex, float *texvec, float *dxt, float *dyt);
extern     int stucci(Tex *tex, float *texvec);
extern     int test_dlerr(const char *name,  const char *symbol);
extern     int texnoise(Tex *tex);
extern     int wood(Tex *tex, float *texvec);


	/* zbuf.c */
extern        void abufsetrow(int y);
extern        void addAlphaOverShort(ushort *doel, ushort *bron);   
extern        void addAlphaUnderShort(ushort *doel, ushort *bron);  

extern			void fillrect(uint *rect, int x, uint y, uint val);

extern        void projectvert(float *v1,float *adr);
extern        void projectverto(float *v1,float *adr);
extern        void zbufferall(void);
extern		  void zbufferall_cmapcode(void);
extern		  void zbuffershad(LampRen *lar);

extern			void bgnaccumbuf(void);
extern			void endaccumbuf(void);

extern		  int testclip(float *v);

	/* envmap.c */
extern int 	envmaptex(Tex *tex, float *texvec, float *dxt, float *dyt);

#endif /* RENDER_H */

