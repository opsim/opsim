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

/* *************************************** 




    radio.h	nov/dec 1992
	revised for Blender may 1999


   *************************************** */

#include "blender.h"
#include "graphics.h"

#define DTWIRE		0
#define DTGOUR		2
#define DTSOLID		1

#define PI  M_PI
#define RAD_MAXFACETAB	1024
#define RAD_NEXTFACE(a)	if( ((a) & 1023)==0 ) face= RG.facebase[(a)>>10]; else face++;

/* RG.phase */
#define RAD_SHOOTE 	1
#define RAD_SHOOTP 	2
#define RAD_SOLVE 	3

typedef struct {
	float cam[3], tar[3], up[3];
	float wx1, wx2, wy1, wy2;
	float near, far;
	float viewmat[4][4], winmat[4][4];
	unsigned int *rect, *rectz;
	short rectx, recty;
	int wid;

} RadView;

/* rn->f */
#define RAD_PATCH		1
#define RAD_SHOOT		2
#define RAD_SUBDIV		4
#define RAD_BACKFACE	8


typedef struct RNode {					/* lengte: 76 */
	struct RNode *down1, *down2, *up;
	struct RNode *ed1, *ed2, *ed3, *ed4;
	struct RPatch *par;

	char lev1, lev2, lev3, lev4;		/* edgelevels */
	short type;							/* type: 4==QUAD, 3==TRIA */
	short f;							
	float *v1, *v2, *v3, *v4;
	float totrad[3], area;
	
	unsigned int col;
} RNode;


typedef struct Elem {					/* lengte: 44 */
	struct RPatch *par;

	short type;							/* type: 4==QUAD, 3==TRIA */
	short f;							/* bit 0: patch, bit 1: shootelement */
	float *v1, *v2, *v3, *v4;
	float totrad[3], area;
	
	unsigned int col;
} Elem;


typedef struct Face {					/* lengte: 20 */
	float *v1, *v2, *v3, *v4;
	unsigned int col;
} Face;

/* rp->f1 */
#define RAD_NO_SPLIT	1

typedef struct RPatch {
	struct RPatch *next, *prev;
	RNode *first;			/* first node==patch */

	Object *from;
	
	int type;				/* 3: TRIA, 4: QUAD */
	short f, f1;			/* flags f: als node, alleen subdiv */

	float ref[3], emit[3], unshot[3];
	float cent[3], norm[3];
	float area;

} RPatch;


typedef struct VeNoCo {				/* nodig voor splitconnected */
	struct VeNoCo *next;
	float *v;
	float *n;
	float *col;
	int flag;
} VeNoCo;


typedef	struct EdSort {					/* sorteren edges */
	float *v1, *v2;
	RNode *node;
	int nr;
} EdSort;

typedef struct {
	Radio *radio;
	unsigned int *hemibuf;
	struct ListBase patchbase;
	int totpatch, totelem, totvert, totlamp;
	RNode **elem;						/* globaal array van alle pointers */
	VeNoCo *verts;						/* tijdelijk vertices van patches */
	float *formfactors;				    /* een factor per element */
	float *topfactors, *sidefactors;    /* LUT voor delta's */
	int *index;						/* LUT voor bovenstaande LUT */
	Face **facebase;
	int totface;
	float min[3], max[3], size[3], cent[3];	/* world */
	float maxsize, totenergy;
	float patchmin, patchmax;
	float elemmin, elemmax;
	float radfactor, lostenergy, igamma;		/* radfac zit in button, radfactor wordt berekend */
	int phase;
	
		/* this part is a copy of struct Radio */
	short hemires, maxiter;
	short drawtype, flag;			/* bit 0 en 1: limits laten zien */
	short subshootp, subshoote, nodelim, maxsublamp;
	int maxnode;
	float convergence;
	float radfac, gamma;		/* voor afbeelden */

} RadGlobal;



extern RadGlobal RG;

/* radfactors.c */
extern float calcStokefactor(RPatch *shoot, RPatch *rp, RNode *rn, float *area);
extern void calcTopfactors();
void calcSidefactors();
extern void initradiosity();
extern void rad_make_hocos(RadView *vw);
extern void hemizbuf(RadView *vw);
extern int makeformfactors(RPatch *shoot);
extern void applyformfactors(RPatch *shoot);
extern RPatch *findshootpatch();
extern void setnodeflags(RNode *rn, int flag, int set);
extern void backface_test(RPatch *shoot);
extern void clear_backface_test();
extern void progressiverad();
extern void minmaxradelem(RNode *rn, float *min, float *max);
extern void minmaxradelemfilt(RNode *rn, float *min, float *max, float *errmin, float *errmax);
extern void subdivideshootElements(int it);
extern void subdivideshootPatches(int it);
extern void inithemiwindows();
extern void closehemiwindows();

/* radio.c */
extern void freeAllRad();
extern int rad_phase();
extern void printstatus();
extern void rad_setlimits();
extern void set_radglobal();
extern void add_radio();
extern void delete_radio();
extern int rad_go(void);
extern void rad_limit_subdivide();

/* radnode.c */
extern void setnodelimit(float limit);
extern float *mallocVert();
extern float *callocVert();
extern void freeVert(float *vert);
extern RNode *mallocNode();
extern RNode *callocNode();
extern void freeNode(RNode *node);
extern void freeNode_recurs(RNode *node);
extern RPatch *mallocPatch();
extern RPatch *callocPatch();
extern void freePatch(RPatch *patch);
extern void replaceAllNode(RNode *neighb, RNode *new);
extern void replaceAllNodeInv(RNode *neighb, RNode *old);
extern void replaceAllNodeUp(RNode *neighb, RNode *old);
extern void replaceTestNode(RNode *neighb, RNode **edpp, RNode *new, int level, float *vert);

/* radnode.c */
extern void start_fastmalloc(char *str);
extern int setvertexpointersNode(RNode *neighb, RNode *node, int level, float **v1, float **v2);
extern float edlen(float *v1, float *v2);
extern void deleteNodes(RNode *node);
extern void subdivideTriNode(RNode *node, RNode *edge);
extern void subdivideNode(RNode *node, RNode *edge);
extern int comparelevel(RNode *node, RNode *nb, int level);

/* radpreprocess.c */
extern int VecCompare(float *v1, float *v2, float limit);
extern void splitconnected();
extern int vergedge(const void *v1,const void *v2);
extern void addedge(float *v1, float *v2, EdSort *es);
extern void setedgepointers();
extern void rad_collect_meshes();
extern void countelem(RNode *rn);
extern void countglobaldata();
extern void addelem(RNode ***el, RNode *rn, RPatch *rp);
extern void makeGlobalElemArray();
extern void remakeGlobaldata();
extern void splitpatch(RPatch *old);
extern void addpatch(RPatch *old, RNode *rn);
extern void converttopatches();
extern void make_elements();
extern void subdividelamps();
extern void maxsizePatches();

/* radpostprocess.c */
extern void addaccu(register char *z, register char *t);
extern void addaccuweight(register char *z, register char *t, int w);
extern void triaweight(Face *face, int *w1, int *w2, int *w3);
extern void init_face_tab();
extern Face *addface();
extern void makeface(float *v1, float *v2, float *v3, float *v4, unsigned int col);
extern void anchorQuadface(RNode *rn, float *v1, float *v2, float *v3, float *v4, int flag);
extern void anchorTriface(RNode *rn, float *v1, float *v2, float *v3, int flag);
extern float *findmiddlevertex(RNode *node, RNode *nb, float *v1, float *v2);
extern void make_face_tab();
extern void filterFaces();
extern void calcfiltrad(RNode *rn, float *cd);
extern void filterNodes();
extern void removeEqualNodes(short limit);

/* raddisplay.c */
extern char calculatecolor(float col);
extern void make_node_display();
extern void drawnodeWire(RNode *rn);
extern void drawsingnodeWire(RNode *rn);
extern void drawnodeSolid(RNode *rn);
extern void drawnodeGour(RNode *rn);
extern void drawpatch(RPatch *patch, uint col);
extern void drawnode_ext(RNode *rn, uint col);
extern void drawOverflowElem();
extern void drawfaceGour(Face *face);
extern void drawfaceSolid(Face *face);
extern void drawfaceWire(Face *face);
extern void drawsquare(float *cent, float size, short cox, short coy);
extern void drawlimits();
extern void setcolNode(RNode *rn, uint *col);
extern void pseudoAmb();
extern void rad_drawall();
extern void rad_forcedraw();


