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


/*  editmesh.c      GRAPHICS
 * 
 *  maart 95
 *  
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "edit.h"
#include "sector.h"
#include "render.h"

/*  voor debug:
#define free(a)			freeN(a)
#define malloc(a)		mallocN(a, "malloc")
#define calloc(a, b)	callocN((a)*(b), "calloc")
#define freelist(a)		freelistN(a)
*/

extern short editbutflag;

float icovert[12][3] =
	{0,0,-200, 144.72,-105.144,-89.443,-55.277,-170.128,-89.443, -178.885,0,-89.443,
	-55.277,170.128,-89.443, 144.72,105.144,-89.443, 55.277,-170.128,89.443, -144.72,-105.144,89.443,
	-144.72,105.144,89.443, 55.277,170.128,89.443, 178.885,0,89.443, 0,0,200};
short icovlak[20][3] = {
	1,0,2, 1,0,5, 2,0,3, 3,0,4, 4,0,5, 1,5,10, 2,1,6, 3,2,7,
	4,3,8, 5,4,9, 10,1,6, 6,2,7, 7,3,8, 8,4,9, 9,5,10, 6,10,11,
	7,6,11, 8,7,11, 9,8,11, 10,9,11};

void righthandfaces(int select);

#define TEST_EDITMESH	if(G.obedit==0 || get_mesh(G.obedit)==0) return; \
						if( (G.vd->lay & G.obedit->lay)==0 ) return;

/* ***************** HASH ********************* */

/* HASH struct voor snel opzoeken edges */
struct HashEdge {
	struct EditEdge *eed;
	struct HashEdge *next;
};

struct HashEdge *hashedgetab=0;

/********* qsort routines *********/


struct xvertsort {
	float x;
	EditVert *v1;
};


int vergxco(x1, x2)
struct xvertsort *x1,*x2;
{

	if( x1->x > x2->x ) return 1;
	else if( x1->x < x2->x) return -1;
	return 0;
}

struct vlaksort {
	long x;
	struct EditVlak *evl;
};


int vergvlak(x1, x2)
struct vlaksort *x1, *x2;
{

	if( x1->x > x2->x ) return 1;
	else if( x1->x < x2->x) return -1;
	return 0;
}


/* ************ ADD / REMOVE / FIND ****************** */

#define EDHASH(a, b)	( (a)*256 + (b) )
#define EDHASHSIZE	65536

void check_hashedge(void)
{
	int i, i2,  doubedge=0;
	struct HashEdge *he,  *he2;
	
	for (i=0; i<64; i++) {
		he= hashedgetab+i;
		
		while (he && he->eed) {
			for (i2=i+1; i2<64; i2++) {
				he2= hashedgetab+i2;
				
				while (he2) {
					if (he->eed == he2->eed) doubedge++;
									
					he2= he2->next;
				}	
			}
			
			he= he->next;
		}	
	}
	
	if (doubedge) printf("%d double edges!\n", doubedge);
}

EditVert *addvertlist(float *vec)
{
	EditVert *eve;
	static unsigned char hashnr= 0;

	eve= calloc(sizeof(EditVert),1);
	addtail(&G.edve, eve);
	
	if(vec) VECCOPY(eve->co, vec);

	eve->hash= hashnr++;

	return eve;
}

EditEdge *findedgelist(EditVert *v1, EditVert *v2)
{
	EditVert *v3;
	struct HashEdge *he;

	if(hashedgetab==0) {
		hashedgetab= callocN(EDHASHSIZE*sizeof(struct HashEdge), "hashedgetab");
	}
	
	/* swap ? */
	if( (long)v1 > (long)v2) {
		v3= v2; 
		v2= v1; 
		v1= v3;
	}
	
	/* eerst even op de flip-plek kijken */
	
/* 	he= hashedgetab + EDHASH(v2->hash, v1->hash); */
/* 	if(he->eed && he->eed->v1==v1 && he->eed->v2==v2) return he->eed; */
	
	
	he= hashedgetab + EDHASH(v1->hash, v2->hash);
	
	while(he) {
		
		if(he->eed && he->eed->v1==v1 && he->eed->v2==v2) return he->eed;
		
		he= he->next;
	}
	return 0;
}

void insert_hashedge(EditEdge *eed)
{
	/* er van uitgaande dat eed nog niet in lijst zit, en eerst een find is gedaan */
	
	struct HashEdge *first, *he;

	/* eerst even op de flip-plek kijken */
/* 	he= hashedgetab + EDHASH(eed->v2->hash, eed->v1->hash); */

/* 	if(he->eed==0) { */
/* 		he->eed= eed; */
/* 		return; */
/* 	} */

	first= hashedgetab + EDHASH(eed->v1->hash, eed->v2->hash);

	if( first->eed==0 ) {
		first->eed= eed;
	}
	else {
		he= (struct HashEdge *)malloc(sizeof(struct HashEdge)); 
		he->eed= eed;
		he->next= first->next;
		first->next= he;
	}
}

void remove_hashedge(EditEdge *eed)
{
	/* er van uitgaande dat eed in lijst zit */
	
	struct HashEdge *first, *he, *prev=NULL;


	/* eerst even op de flip-plek kijken */
/* 	first= hashedgetab + EDHASH(eed->v2->hash, eed->v1->hash); */

/* 	if(first->eed==eed) { */
		/* uit lijst verwijderen */
		
/* 		if(first->next) { */
/* 			he= first->next; */
/* 			first->eed= he->eed; */
/* 			first->next= he->next; */
/* 			free(he); */
/* 		} */
/* 		else first->eed= 0; */

/* 		return; */
/* 	} */


	he=first= hashedgetab + EDHASH(eed->v1->hash, eed->v2->hash);

	while(he) {
		if(he->eed == eed) {
			/* uit lijst verwijderen */
			if(he==first) {
				if(first->next) {
					he= first->next;
					first->eed= he->eed;
					first->next= he->next;
					free(he);
				}
				else he->eed= 0;
			}
			else {
				prev->next= he->next;
				free(he);
			}
			return;
		}
		prev= he;
		he= he->next;
	}
}

void free_hashedgetab()
{
	struct HashEdge *he, *first, *hen;
	int a;
/* 	int test[30], nr, toted=0; */
	
	/* for(a=0; a<30; a++) test[a]=0; */
	
	if(hashedgetab) {
	
		first= hashedgetab;
		for(a=0; a<EDHASHSIZE; a++, first++) {
			he= first->next;
			/* nr= 0; */
			/* if(first->eed) toted++; */
			/* if(first->eed) nr++; */
			while(he) {
				hen= he->next;
				free(he);
				he= hen;
				/* nr++; */
			}
			/* if(nr>29) nr= 29; */
			/* test[nr]++; */
		}
		freeN(hashedgetab);
		hashedgetab= 0;

		/* printf("toted %d\n", toted); */
		/* toted= 0; */
		/* for(a=0; a<30; a++) { */
		/* 	printf("tab %d %d\n", a, test[a]); */
		/* } */
	}
}

EditEdge *addedgelist(EditVert *v1, EditVert *v2)
{
	EditVert *v3;
	EditEdge *eed;
	int swap= 0;
	
	/* swap ? */
	if(v1>v2) {
		v3= v2; 
		v2= v1; 
		v1= v3;
		swap= 1;
	}

	if(v1==v2) return 0;
	if(v1==0 || v2==0) return 0;
	
	/* opzoeken in hashlijst */
	eed= findedgelist(v1, v2);
	
	if(eed==0) {

		eed= (EditEdge *)calloc(sizeof(EditEdge), 1);
		eed->v1= v1;
		eed->v2= v2;
		addtail(&G.eded, eed);
		eed->dir= swap;
		insert_hashedge(eed);
	}
	return eed;
}


void remedge(EditEdge *eed)
{

	remlink(&G.eded, eed);

	remove_hashedge(eed);
}

void freevlak(EditVlak *evl)
{
	if(evl->tface) free(evl->tface);
	free(evl);
}

void freevlaklist(ListBase *lb)
{
	EditVlak *evl, *next;
	
	evl= lb->first;
	while(evl) {
		next= evl->next;
		freevlak(evl);
		evl= next;
	}
	lb->first= lb->last= 0;
}

EditVlak *addvlaklist(EditVert *v1, EditVert *v2, EditVert *v3, EditVert *v4, int mat_nr, int flag, struct TFace *tface)
{
	EditVlak *evl;
	EditEdge *e1, *e2=0, *e3=0, *e4=0;
	
	if(mat_nr<0) mat_nr= 0;
	
	/* voeg vlak toe aan lijst en doe meteen de edges */
	e1= addedgelist(v1, v2);
	if(v3) e2= addedgelist(v2, v3);
	if(v4) e3= addedgelist(v3, v4); else e3= addedgelist(v3, v1);
	if(v4) e4= addedgelist(v4, v1);
	
	if(v1==v2 || v2==v3 || v1==v3) return 0;
	if(e2==0) return 0;

	evl= (EditVlak *)malloc(sizeof(EditVlak));
	evl->v1= v1;
	evl->v2= v2;
	evl->v3= v3;
	evl->v4= v4;

	evl->e1= e1;
	evl->e2= e2;
	evl->e3= e3;
	evl->e4= e4;
	
	evl->mat_nr= mat_nr;
	if(tface) {
		evl->tface= malloc(sizeof(TFace));
		*(evl->tface)= *(tface);
	}
	else evl->tface= 0;
	evl->f= 0;
	evl->f1= 0;
	evl->flag= flag;
	
	addtail(&G.edvl, evl);
	if(evl->v4) CalcNormFloat4(v1->co, v2->co, v3->co, v4->co, evl->n);
	else CalcNormFloat(v1->co, v2->co, v3->co, evl->n);

	return evl;
}

int comparevlak(EditVlak *vl1, EditVlak *vl2)
{
	EditVert *v1, *v2, *v3, *v4;
	
	if(vl1->v4 && vl2->v4) {
		v1= vl2->v1;
		v2= vl2->v2;
		v3= vl2->v3;
		v4= vl2->v4;
		
		if(vl1->v1==v1 || vl1->v2==v1 || vl1->v3==v1 || vl1->v4==v1) {
			if(vl1->v1==v2 || vl1->v2==v2 || vl1->v3==v2 || vl1->v4==v2) {
				if(vl1->v1==v3 || vl1->v2==v3 || vl1->v3==v3 || vl1->v4==v3) {
					if(vl1->v1==v4 || vl1->v2==v4 || vl1->v3==v4 || vl1->v4==v4) {
						return 1;
					}
				}
			}
		}
	}
	else if(vl1->v4==0 && vl2->v4==0) {
		v1= vl2->v1;
		v2= vl2->v2;
		v3= vl2->v3;

		if(vl1->v1==v1 || vl1->v2==v1 || vl1->v3==v1) {
			if(vl1->v1==v2 || vl1->v2==v2 || vl1->v3==v2) {
				if(vl1->v1==v3 || vl1->v2==v3 || vl1->v3==v3) {
					return 1;
				}
			}
		}
	}

	return 0;
}


int dubbelvlak(EditVlak *evltest)
{
	
	EditVlak *evl;
	
	evl= G.edvl.first;
	while(evl) {
		if(evl!=evltest) {
			if(comparevlak(evltest, evl)) return 1;
		}
		evl= evl->next;
	}
	return 0;
}

int exist_vlak(v1, v2, v3, v4)
EditVert *v1, *v2, *v3, *v4;
{
	EditVlak *evl, evltest;
	
	evltest.v1= v1;
	evltest.v2= v2;
	evltest.v3= v3;
	evltest.v4= v4;
	
	evl= G.edvl.first;
	while(evl) {
		if(comparevlak(&evltest, evl)) return 1;
		evl= evl->next;
	}
	return 0;
}


int vlakselectedOR(EditVlak *evl, int flag)
{
	
	if(evl->v1->f & flag) return 1;
	if(evl->v2->f & flag) return 1;
	if(evl->v3->f & flag) return 1;
	if(evl->v4 && (evl->v4->f & 1)) return 1;
	return 0;
}

int vlakselectedAND(EditVlak *evl, int flag)
{
	if(evl->v1->f & flag) {
		if(evl->v2->f & flag) {
			if(evl->v3->f & flag) {
				if(evl->v4) {
					if(evl->v4->f & flag) return 1;
				}
				else return 1;
			}
		}
	}
	return 0;
}

void recalc_editnormals()
{
	EditVlak *evl;

	evl= G.edvl.first;
	while(evl) {
		if(evl->v4) CalcNormFloat4(evl->v1->co, evl->v2->co, evl->v3->co, evl->v4->co, evl->n);
		else CalcNormFloat(evl->v1->co, evl->v2->co, evl->v3->co, evl->n);
		evl= evl->next;
	}
}

void flipvlak(EditVlak *evl)
{
	if(evl->v4) {
		SWAP(EditVert *, evl->v2, evl->v4);
		SWAP(EditEdge *, evl->e1, evl->e4);
		SWAP(EditEdge *, evl->e2, evl->e3);
		if(evl->tface) {
			SWAP(short, evl->tface->uv[1][0], evl->tface->uv[3][0]);
			SWAP(short, evl->tface->uv[1][1], evl->tface->uv[3][1]);
		}
	}
	else {
		SWAP(EditVert *, evl->v2, evl->v3);
		SWAP(EditEdge *, evl->e1, evl->e3);
		evl->e2->dir= 1-evl->e2->dir;
		if(evl->tface) {
			SWAP(short, evl->tface->uv[1][0], evl->tface->uv[2][0]);
			SWAP(short, evl->tface->uv[1][1], evl->tface->uv[2][1]);
		}
	}
	if(evl->v4) CalcNormFloat4(evl->v1->co, evl->v2->co, evl->v3->co, evl->v4->co, evl->n);
	else CalcNormFloat(evl->v1->co, evl->v2->co, evl->v3->co, evl->n);
}


void flip_editnormals()
{
	EditVlak *evl;
	
	evl= G.edvl.first;
	while(evl) {
		if( vlakselectedAND(evl, 1) ) {
			flipvlak(evl);
		}
		evl= evl->next;
	}
}

/* ************************ IN & OUT ***************************** */

void edge_normal_compare(EditEdge *eed, EditVlak *evl1)
{
	EditVlak *evl2;
	float cent1[3], cent2[3];
	float inp;
	
	evl2= (EditVlak *)eed->vn;
	if(evl1==evl2) return;
	
	inp= evl1->n[0]*evl2->n[0] + evl1->n[1]*evl2->n[1] + evl1->n[2]*evl2->n[2];
	if(inp<0.999 && inp >-0.999) eed->f= 1;
	
	if ELEM(G.obedit->type, OB_SECTOR, OB_LIFE) {
		
		if(evl1->v4) CalcCent4f(cent1, evl1->v1->co, evl1->v2->co, evl1->v3->co, evl1->v4->co);
		else CalcCent3f(cent1, evl1->v1->co, evl1->v2->co, evl1->v3->co);
		if(evl2->v4) CalcCent4f(cent2, evl2->v1->co, evl2->v2->co, evl2->v3->co, evl2->v4->co);
		else CalcCent3f(cent2, evl2->v1->co, evl2->v2->co, evl2->v3->co);
		
		VecSubf(cent1, cent2, cent1);
		Normalise(cent1);
		inp= cent1[0]*evl1->n[0] + cent1[1]*evl1->n[1] + cent1[2]*evl1->n[2]; 

		if(inp < -0.001 ) eed->f1= 1;
	}

}

void edge_drawflags()
{
	EditVert *eve;
	EditEdge *eed, *e1, *e2, *e3, *e4;
	EditVlak *evl;
	
	/* - tel aantal keren in vlakken gebruikt: 0 en 1 is tekenen
	 * - edges meer dan 1 keer: in *vn zit pointer naar (eerste) vlak
	 * - loop alle vlakken af, is normaal te afwijkend: tekenen (flag wordt 1)
	 * - nieuw: ook cylcinder-snij test voor dynamesh
	 */
	
	recalc_editnormals();
	
	/* init */
	eve= G.edve.first;
	while(eve) {
		eve->f1= 1;		/* wordt bij test op nul gezet */
		eve= eve->next;
	}
	eed= G.eded.first;
	while(eed) {
		eed->f= eed->f1= 0;
		eed->vn= 0;
		eed= eed->next;
	}

	evl= G.edvl.first;
	while(evl) {
		e1= evl->e1;
		e2= evl->e2;
		e3= evl->e3;
		e4= evl->e4;
		if(e1->f<3) e1->f+= 1;
		if(e2->f<3) e2->f+= 1;
		if(e3->f<3) e3->f+= 1;
		if(e4 && e4->f<3) e4->f+= 1;
		
		if(e1->vn==0) e1->vn= (EditVert *)evl;
		if(e2->vn==0) e2->vn= (EditVert *)evl;
		if(e3->vn==0) e3->vn= (EditVert *)evl;
		if(e4 && e4->vn==0) e4->vn= (EditVert *)evl;
		
		evl= evl->next;
	}

	if(G.f & G_ALLEDGES) {
		evl= G.edvl.first;
		while(evl) {
			if(evl->e1->f>=2) evl->e1->f= 1;
			if(evl->e2->f>=2) evl->e2->f= 1;
			if(evl->e3->f>=2) evl->e3->f= 1;
			if(evl->e4 && evl->e4->f>=2) evl->e4->f= 1;
			
			evl= evl->next;
		}		
	}	
	else {
		
		if ELEM(G.obedit->type, OB_SECTOR, OB_LIFE) {
			
			/* single-edges afvangen voor cylinder flag */
			
			eed= G.eded.first;
			while(eed) {
				if(eed->f==1) eed->f1= 1;
				eed= eed->next;
			}
		}

		/* alle vlakken, alle edges met flag==2: vergelijk normaal */
		evl= G.edvl.first;
		while(evl) {
			if(evl->e1->f==2) edge_normal_compare(evl->e1, evl);
			if(evl->e2->f==2) edge_normal_compare(evl->e2, evl);
			if(evl->e3->f==2) edge_normal_compare(evl->e3, evl);
			if(evl->e4 && evl->e4->f==2) edge_normal_compare(evl->e4, evl);
			
			evl= evl->next;
		}

		if ELEM(G.obedit->type, OB_SECTOR, OB_LIFE) {
			
			/* sphere collision flag */
			
			eed= G.eded.first;
			while(eed) {
				if(eed->f1!=1) {
					eed->v1->f1= eed->v2->f1= 0;
				}
				eed= eed->next;
			}
		}
	}
}

int contrpuntnorm(float *n, float *puno)
{
	float inp;

	inp= n[0]*puno[0]+n[1]*puno[1]+n[2]*puno[2];

	/* angles 90 degrees: dont flip */
	if(inp> -0.000001) return 0;

	return 1;
}

void vertexnormals(int testflip)
{
	Mesh *me;
	EditVert *eve;
	EditVlak *evl;	
	float n1[3], n2[3], n3[3], n4[3], co[4], fac1, fac2, fac3, fac4, *temp;
	float *f1, *f2, *f3, *f4, xn, yn, zn;
	float opp, len;
	
	/* this function also called by make_s_mesh() */
	if(G.obedit && G.obedit->type==OB_MESH) {
		me= G.obedit->data;
		if(me->flag & ME_SMESH) testflip= 0;
		if((me->flag & ME_TWOSIDED)==0) testflip= 0;
	}

	if(G.totvert==0) return;

	if(G.totface==0) {
		/* namaak puno's voor halopuno! */
		eve= G.edve.first;
		while(eve) {
			VECCOPY(eve->no, eve->co);
			Normalise( (float *)eve->no);
			eve= eve->next;
		}
		return;
	}

	/* clear normals */	
	eve= G.edve.first;
	while(eve) {
		eve->no[0]= eve->no[1]= eve->no[2]= 0.0;
		eve= eve->next;
	}
	
	/* berekenen cos hoeken en oppervlakte en optellen bij puno */
	evl= G.edvl.first;
	while(evl) {
		VecSubf(n1, evl->v2->co, evl->v1->co);
		VecSubf(n2, evl->v3->co, evl->v2->co);
		Normalise(n1);
		Normalise(n2);

		if(evl->v4==0) {
			VecSubf(n3, evl->v1->co, evl->v3->co);
			Normalise(n3);
			
			/* opp= AreaT3Dfl(evl->v1->co, evl->v2->co, evl->v3->co); */
			/* if(opp!=0.0) opp=1.0/opp;  */
			/* opp= fsqrt(opp); */
			/* for smooth subdivide...*/
			opp= 1.0;
			co[0]= opp*safacos(-n3[0]*n1[0]-n3[1]*n1[1]-n3[2]*n1[2]);
			co[1]= opp*safacos(-n1[0]*n2[0]-n1[1]*n2[1]-n1[2]*n2[2]);
			co[2]= opp*safacos(-n2[0]*n3[0]-n2[1]*n3[1]-n2[2]*n3[2]);
			
		}
		else {
			VecSubf(n3, evl->v4->co, evl->v3->co);
			VecSubf(n4, evl->v1->co, evl->v4->co);
			Normalise(n3);
			Normalise(n4);
			
			/* opp= AreaQ3Dfl(evl->v1->co, evl->v2->co, evl->v3->co, evl->v4->co); */
			/* if(opp!=0.0) opp=1.0/opp;  */
			/* opp= fsqrt(opp); */
			/* for smooth subdivide...*/
			opp= 1.0;
			co[0]= opp*safacos(-n4[0]*n1[0]-n4[1]*n1[1]-n4[2]*n1[2]);
			co[1]= opp*safacos(-n1[0]*n2[0]-n1[1]*n2[1]-n1[2]*n2[2]);
			co[2]= opp*safacos(-n2[0]*n3[0]-n2[1]*n3[1]-n2[2]*n3[2]);
			co[3]= opp*safacos(-n3[0]*n4[0]-n3[1]*n4[1]-n3[2]*n4[2]);
		}
		
		temp= evl->v1->no;
		if(testflip && contrpuntnorm(evl->n, temp) ) co[0]= -co[0];
		temp[0]+= co[0]*evl->n[0];
		temp[1]+= co[0]*evl->n[1];
		temp[2]+= co[0]*evl->n[2];
		
		temp= evl->v2->no;
		if(testflip && contrpuntnorm(evl->n, temp) ) co[1]= -co[1];
		temp[0]+= co[1]*evl->n[0];
		temp[1]+= co[1]*evl->n[1];
		temp[2]+= co[1]*evl->n[2];
		
		temp= evl->v3->no;
		if(testflip && contrpuntnorm(evl->n, temp) ) co[2]= -co[2];
		temp[0]+= co[2]*evl->n[0];
		temp[1]+= co[2]*evl->n[1];
		temp[2]+= co[2]*evl->n[2];
		
		if(evl->v4) {
			temp= evl->v4->no;
			if(testflip && contrpuntnorm(evl->n, temp) ) co[3]= -co[3];
			temp[0]+= co[3]*evl->n[0];
			temp[1]+= co[3]*evl->n[1];
			temp[2]+= co[3]*evl->n[2];
		}
		
		evl= evl->next;
	}

	/* normaliseren puntnormalen */
	eve= G.edve.first;
	while(eve) {
		len= Normalise(eve->no);
		if(len==0.0) {
			VECCOPY(eve->no, eve->co);
			Normalise( eve->no);
		}
		eve= eve->next;
	}
	
	/* puntnormaal omklap-vlaggen voor bij shade */
	evl= G.edvl.first;
	while(evl) {
		evl->f=0;			

		if(testflip) {
			f1= evl->v1->no;
			f2= evl->v2->no;
			f3= evl->v3->no;
			
			fac1= evl->n[0]*f1[0] + evl->n[1]*f1[1] + evl->n[2]*f1[2];
			if(fac1<0.0) {
				evl->f = ME_FLIPV1;
			}
			fac2= evl->n[0]*f2[0] + evl->n[1]*f2[1] + evl->n[2]*f2[2];
			if(fac2<0.0) {
				evl->f += ME_FLIPV2;
			}
			fac3= evl->n[0]*f3[0] + evl->n[1]*f3[1] + evl->n[2]*f3[2];
			if(fac3<0.0) {
				evl->f += ME_FLIPV3;
			}
			if(evl->v4) {
				f4= evl->v4->no;
				fac4= evl->n[0]*f4[0] + evl->n[1]*f4[1] + evl->n[2]*f4[2];
				if(fac4<0.0) {
					evl->f += ME_FLIPV4;
				}
			}
		}
		/* proj voor cubemap! */
		xn= fabs(evl->n[0]);
		yn= fabs(evl->n[1]);
		zn= fabs(evl->n[2]);
		
		if(zn>xn && zn>yn) evl->f += ME_PROJXY;
		else if(yn>xn && yn>zn) evl->f += ME_PROJXZ;
		else evl->f += ME_PROJYZ;
		
		evl= evl->next;
	}
}

void free_editmesh()
{
	if(G.edve.first) freelist(&G.edve);
	if(G.eded.first) freelist(&G.eded);
	if(G.edvl.first) freevlaklist(&G.edvl);
	free_hashedgetab();
	G.totvert= G.totface= 0;
}

void make_editMesh()
{
	Mesh *me;
	MFace *mface;
	TFace *tface;
	MVert *mvert;
	KeyBlock *actkey=0;
	EditVert *eve, **evlist, *eve1, *eve2, *eve3, *eve4;
	EditVlak *evl;
	int tot, a;

	if(G.obedit==0) return;

	/* ivm reload */
	free_editmesh();
	
	me= get_mesh(G.obedit);
	G.totvert= tot= me->totvert;

	if(tot==0) {
		countall();
		return;
	}
	
	waitcursor(1);

	/* keys? */
	if(me->key) {
		actkey= me->key->block.first;
		while(actkey) {
			if(actkey->flag & SELECT) break;
			actkey= actkey->next;
		}
	}

	if(actkey) {
		key_to_mesh(actkey, me);
		tot= actkey->totelem;
	}

	/* editverts aanmaken */
	mvert= me->mvert;

	evlist= (EditVert **)mallocN(tot*sizeof(void *),"evlist");
	for(a=0; a<tot; a++, mvert++) {
		eve= addvertlist(mvert->co);
		evlist[a]= eve;
		eve->no[0]= mvert->no[0]/32767.0;
		eve->no[1]= mvert->no[1]/32767.0;
		eve->no[2]= mvert->no[2]/32767.0;
	}

	if(actkey && actkey->totelem!=me->totvert);
	else {
		TFace tempface;
		uint *mcol;
		
		/* edges en vlakken maken */
		mface= me->mface;
		tface= me->tface;
		mcol= (uint *)me->mcol;
		if(me->mcol) tface= &tempface;
		
		for(a=0; a<me->totface; a++, mface++) {
			eve1= evlist[mface->v1];
			eve2= evlist[mface->v2];
			if(mface->v3) eve3= evlist[mface->v3]; else eve3= 0;
			if(mface->v4) eve4= evlist[mface->v4]; else eve4= 0;
			
			if(mcol) memcpy(tempface.col, mcol, 16);
			else if(me->tface && (tface->flag & SELECT)) {
				if(G.f & G_FACESELECT) {
					eve1->f |= 1;
					eve2->f |= 1;
					if(eve3) eve3->f |= 1;
					if(eve4) eve4->f |= 1;
				}
			}
			
			evl= addvlaklist(eve1, eve2, eve3, eve4, mface->mat_nr, mface->flag, tface);
			
			if(me->tface) tface++;
			if(mcol) mcol+=4;
		}
	}
	freeN(evlist);
	
	countall();
	
	if(me->flag & ME_SMESH) makeDispList(G.obedit);
	
	waitcursor(0);
}


void load_editMesh()
{
	MemHead *memh;
	Mesh *me;
	MFace *mface;
	MVert *mvert;
	MSticky *ms;
	KeyBlock *actkey=0;
	EditVert *eve;
	EditVlak *evl;
	EditEdge *eed;
	float *fp, nor[3];
	int tot, a;

	waitcursor(1);
	countall();
	
	me= get_mesh(G.obedit);
	
	/* zijn er keys? */
	if(me->key) {
		actkey= me->key->block.first;
		while(actkey) {
			if(actkey->flag & SELECT) break;
			actkey= actkey->next;
		}
	}

	
	if(actkey && me->key->refkey!=actkey) {
		/* aktieve key && niet de refkey: alleen vertices */
				
		if(G.totvert) {
			if(actkey->data) freeN(actkey->data);
		
			fp=actkey->data= callocN(me->key->elemsize*G.totvert, "actkey->data");
			actkey->totelem= G.totvert;
	
			eve= G.edve.first;
			while(eve) {
				VECCOPY(fp, eve->co);
				fp+= 3;
				eve= eve->next;
			}
		}
	}
	else if(me->key && actkey==0) {
		/* er zijn keys, alleen veranderingen in mverts schrijven */
		/* als aantal vertices verschillen, beetje onvoorspelbaar */
			
		eve= G.edve.first;
		mvert= me->mvert;
		for(a=0; a<me->totvert; a++, mvert++) {
			VECCOPY(mvert->co, eve->co);
			eve= eve->next;
			if(eve==0) break;
		}
	}
	else {
		/* als er keys zijn: de refkey, anders gewoon de me */
		
		/* deze telt ook of edges niet in vlakken zitten: */
		/* eed->f==0 niet in vlak, f==1 is tekenen */
		/* eed->f1 : flag voor dynaface (cylindertest) */
		/* eve->f1 : flag voor dynaface (sphere test) */
		edge_drawflags();
	
		/* LET OP: op evl->f de punoflag */
		vertexnormals( (me->flag & ME_NOPUNOFLIP)==0 );
	
		eed= G.eded.first;
		while(eed) {
			if(eed->f==0) G.totface++;
			eed= eed->next;
		}
	
		/* nieuw Face blok */
		if(G.totface==0) mface= 0;
		else mface= callocN(G.totface*sizeof(MFace), "loadeditMesh1");
		/* nieuw Vertex blok */
		if(G.totvert==0) mvert= 0;
		else mvert= callocN(G.totvert*sizeof(MVert), "loadeditMesh2");
		
		if(me->mvert) freeN(me->mvert);
		me->mvert= mvert;
		if(me->mface) freeN(me->mface);
		
		me->mface= mface;
		me->totvert= G.totvert;
		me->totface= G.totface;
		
		/* de vertices, gebruik ->vn als teller */
		eve= G.edve.first;
		a=0;

		while(eve) {
			VECCOPY(mvert->co, eve->co);
			mvert->mat_nr= 255;  /* waarvoor ook al weer, haloos? */
			
			/* puno */
			VECCOPY(nor, eve->no);
			VecMulf(nor, 32767.0);
			VECCOPY(mvert->no, nor);

			eve->vn= (EditVert *)(long)(a++);  /* teller */
			
			mvert->flag= 0;
			if(eve->f1==1) mvert->flag |= ME_SPHERETEST;
			
			eve= eve->next;
			mvert++;
		}
	
		/* de vlakken */
		mface= me->mface;
		evl= G.edvl.first;
		while(evl) {
			mface->v1= (long) evl->v1->vn;
			mface->v2= (long) evl->v2->vn;
			mface->v3= (long) evl->v3->vn;
			if(evl->v4) mface->v4= (long) evl->v4->vn;
			
			mface->mat_nr= evl->mat_nr;
			mface->puno= evl->f;
			mface->flag= evl->flag;
			
			/* mat_nr in vertex */
			if(me->totcol>1) {
				mvert= me->mvert+mface->v1;
				if(mvert->mat_nr == 255) mvert->mat_nr= mface->mat_nr;
				mvert= me->mvert+mface->v2;
				if(mvert->mat_nr == 255) mvert->mat_nr= mface->mat_nr;
				mvert= me->mvert+mface->v3;
				if(mvert->mat_nr == 255) mvert->mat_nr= mface->mat_nr;
				if(mface->v4) {
					mvert= me->mvert+mface->v4;
					if(mvert->mat_nr == 255) mvert->mat_nr= mface->mat_nr;
				}
			}
			
			/* dyna cilinder flag minder kritisch testen: 'dubbel' in vlakken laten zitten. 
			 * gaat anders fout bij scherpe hoeken (inpspeed voor een wel, ander niet!)
			 * Mogelijk oplossen door volgorde aan te passen: sphere-cyl-face. Kost te veel?
			 */
			if(evl->e1->f1==1) mface->edcode |= DF_V1V2; 
			if(evl->e2->f1==1) mface->edcode |= DF_V2V3;
			if(evl->v4) {
				if(evl->e3->f1==1) mface->edcode |= DF_V3V4; 
				if(evl->e4->f1==1) mface->edcode |= DF_V4V1; 
			}
			else {
				if(evl->e3->f1==1) mface->edcode |= DF_V3V1; 
			}


			/* letop: evl->e1->f==0 is losse edge */ 
			
			if(evl->e1->f==1) {
				mface->edcode |= ME_V1V2; 
				evl->e1->f= 2;
			}			
			if(evl->e2->f==1) {
				mface->edcode |= ME_V2V3; 
				evl->e2->f= 2;
			}
			if(evl->e3->f==1) {
				if(evl->v4) {
					mface->edcode |= ME_V3V4;
				}
				else {
					mface->edcode |= ME_V3V1;
				}
				evl->e3->f= 2;
			}
			if(evl->e4 && evl->e4->f==1) {
				mface->edcode |= ME_V4V1; 
				evl->e4->f= 2;
			}
			
			/* geen index '0' op plek 3 of 4 */
			if(evl->v4) test_index_mface(mface, 4);
			else test_index_mface(mface, 3);
			
			mface++;
			evl= evl->next;
		}
		
		/* losse edges als vlak toevoegen */
		eed= G.eded.first;
		while(eed) {
			if( eed->f==0 ) {
				mface->v1= (long) eed->v1->vn;
				mface->v2= (long) eed->v2->vn;
				test_index_mface(mface, 2);
				mface->edcode= ME_V1V2;
				mface++;
			}
			eed= eed->next;
		}
		
		tex_space_mesh(me);
		if(actkey) mesh_to_key(me, actkey);
		
		/* texmesh: ahv ->tface alles opnieuw maken */
		if(me->tface && me->totface) {
			TFace *tfn, *tf;
			
			tf=tfn= callocN(sizeof(TFace)*me->totface, "tface");
			evl= G.edvl.first;
			while(evl) {
				
				if(evl->tface) *tf= *(evl->tface);
				else default_tface(tf);
				
				if(G.f & G_FACESELECT) {
					if( vlakselectedAND(evl, 1) ) tf->flag |= SELECT;
					else tf->flag &= ~SELECT;
				}
				
				tf->no[0]= 32760.0*evl->n[0];
				tf->no[1]= 32760.0*evl->n[1];
				tf->no[2]= 32760.0*evl->n[2];
				
				tf++;
				evl= evl->next;
			}
			
			freeN(me->tface);
			me->tface= tfn;
		}
		else if(me->tface) {
			/* freeN(me->tface); */
			/* me->tface= 0; */
		}
		
		/* mcol: ahv indexnrs opnieuw maken */
		if(me->mcol && me->totface) {
			uint *mcn, *mc;
			
			mc=mcn= mallocN(4*sizeof(int)*me->totface, "tface");
			evl= G.edvl.first;
			while(evl) {
			
				if(evl->tface)  memcpy(mc, evl->tface->col, 16);
				else bzero(mc, 16);
				
				mc+=4;
				evl= evl->next;
			}
			
			freeN(me->mcol);
			me->mcol= (MCol *)mcn;
		}
		else if(me->mcol) {
			freeN(me->mcol);
			me->mcol= 0;
		}
	}
	
	if(actkey) do_spec_key(me->key);
	
	/* voor zekerheid: ->vn pointers wissen */
	eve= G.edve.first;
	while(eve) {
		eve->vn= 0;
		eve= eve->next;
	}

	/* displisten van alle users, ook deze */
	freedisplist(&me->disp);
	freedisplist(&G.obedit->disp);
	
	/* sticky */
	if(me->msticky) {
		memh= (MemHead *)(me->msticky);
		memh--;
		tot= memh->len/sizeof(MSticky);
		if(tot<me->totvert) {
			ms= callocN(me->totvert*sizeof(MSticky), "msticky");
			memcpy(ms, me->msticky, tot*sizeof(MSticky));
			freeN(me->msticky);
			me->msticky= ms;
			error("Sticky was too small");
		}
	}
	waitcursor(0);
}


void remake_editMesh()
{

	if(okee("Reload Original data")==0) return;
	
	make_editMesh();
	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);
}

/* *********************  TOOLS  ********************* */


void make_vertexcolo()	/* all, gebruiken bij toetsen */
{
	Base *base;
	Mesh *me;
	DispList *dl;
	int warn=0;
	
	if(G.obedit) {
		error("Unable to perform function in EditMode");
		return;
	}
	
	/* vlaggen resetten */
	me= G.main->mesh.first;
	while(me) {
		me->flag &= ~ME_ISDONE;
		me= me->id.next;
	}

	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			if(base->object->type==OB_MESH) {
				me= base->object->data;
				
				if(me->flag & ME_ISDONE) warn= 1;
				else {
				
					dl= base->object->disp.first;
					
					if(me->flag & ME_TWOSIDED) {
						me->flag &= ~ME_TWOSIDED;
					}
					
					me->flag |= ME_ISDONE;
					
					if(dl==0 || dl->col1==0) {
						shadeDispList(base->object);
						dl= base->object->disp.first;
					}
					if(dl && dl->col1) {
						if(me->mcol) freeN(me->mcol);
						me->mcol= dupallocN(dl->col1);
						if(me->tface) mcol_to_tface(me, 1);
					}
					freedisplist(&(base->object->disp));
				}
			}
		}
		base= base->next;
	}
	
	if(warn) notice("Warning: Linked meshes");
	
	allqueue(REDRAWBUTSEDIT, 0);
	allqueue(REDRAWVIEW3D, 0);
}

void make_vertexcol()	/* single ob, bij button */
{
	Object *ob;
	Mesh *me;
	DispList *dl;
	
	if(G.obedit) {
		error("Unable to perform function in EditMode");
		return;
	}
	
	ob= OBACT;
	me= get_mesh(ob);
	if(me==0) return;

	dl= ob->disp.first;
	
	if(me->flag & ME_TWOSIDED) {
		me->flag &= ~ME_TWOSIDED;
	}
	
	if(dl==0 || dl->col1==0) {
		shadeDispList(ob);
		dl= ob->disp.first;
	}
	if(dl && dl->col1) {
		if(me->mcol) freeN(me->mcol);
		me->mcol= dupallocN(dl->col1);
		if(me->tface) mcol_to_tface(me, 1);
	}
	freedisplist(&(ob->disp));

	allqueue(REDRAWBUTSEDIT, 0);
	allqueue(REDRAWVIEW3D, 0);
}


void make_sticky()
{
	Object *ob;
	Base *base;
	MVert *mvert;
	Mesh *me;
	MSticky *ms;
	float ho[4], mat[4][4];
	int a;
	
	if(G.scene->camera==0) return;
	
	if(G.obedit) {
		error("Unable to perform function in EditMode");
		return;
	}
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			if(base->object->type==OB_MESH) {
				ob= base->object;
				
				me= ob->data;
				mvert= me->mvert;
				if(me->msticky) freeN(me->msticky);
				me->msticky= mallocN(me->totvert*sizeof(MSticky), "sticky");
				
				/* stukje roteerscene */		
				R.r= G.scene->r;
				R.r.xsch= (R.r.size*R.r.xsch)/100;
				R.r.ysch= (R.r.size*R.r.ysch)/100;
				
				R.afmx= R.r.xsch/2;
				R.afmy= R.r.ysch/2;
				
				R.ycor= ( (float)R.r.yasp)/( (float)R.r.xasp);
		
				R.rectx= R.r.xsch; 
				R.recty= R.r.ysch;
				R.xstart= -R.afmx; 
				R.ystart= -R.afmy;
				R.xend= R.xstart+R.rectx-1;
				R.yend= R.ystart+R.recty-1;
		
				where_is_object(G.scene->camera);
				Mat4CpyMat4(R.viewinv, G.scene->camera->obmat);
				Mat4Ortho(R.viewinv);
				Mat4Invert(R.viewmat, R.viewinv);
				
				setwindowclip(1, -1);
		
				where_is_object(ob);
				Mat4MulMat4(mat, ob->obmat, R.viewmat);
		
				ms= me->msticky;
				for(a=0; a<me->totvert; a++, ms++, mvert++) {
					VECCOPY(ho, mvert->co);
					Mat4MulVecfl(mat, ho);
					projectverto(ho, ho);
					ms->co[0]= ho[0]/ho[3];
					ms->co[1]= ho[1]/ho[3];
				}
			}
		}
		base= base->next;
	}
	allqueue(REDRAWBUTSEDIT, 0);
}

void fasterdraw()
{
	Base *base;
	Mesh *me;
	MFace *mface;
	int toggle, a;

	if(G.obedit) return;

	/* vlaggen resetten */
	me= G.main->mesh.first;
	while(me) {
		me->flag &= ~ME_ISDONE;
		me= me->id.next;
	}

	base= FIRSTBASE;
	while(base) {
		if( TESTBASELIB(base) && (base->object->type==OB_MESH)) {
			me= base->object->data;
			if(me->id.lib==0 && (me->flag & ME_ISDONE)==0) {
				me->flag |= ME_ISDONE;
				mface= me->mface;
				toggle= 0;
				for(a=0; a<me->totface; a++) {
					if( (mface->edcode & ME_V1V2) && ( (toggle++) & 1) ) {
						mface->edcode-= ME_V1V2;
					}
					if( (mface->edcode & ME_V2V3) && ( (toggle++) & 1)) {
						mface->edcode-= ME_V2V3;
					}
					if( (mface->edcode & ME_V3V1) && ( (toggle++) & 1)) {
						mface->edcode-= ME_V3V1;
					}
					if( (mface->edcode & ME_V4V1) && ( (toggle++) & 1)) {
						mface->edcode-= ME_V4V1;
					}
					if( (mface->edcode & ME_V3V4) && ( (toggle++) & 1)) {
						mface->edcode-= ME_V3V4;
					}
					mface++;
				}
			}
		}
		base= base->next;
	}

	/* belangrijk?: vlaggen weer resetten */
	me= G.main->mesh.first;
	while(me) {
		me->flag &= ~ME_ISDONE;
		me= me->id.next;
	}

	allqueue(REDRAWVIEW3D, 0);
}

void slowerdraw()		/* reset fasterdraw */
{
	Base *base;
	Mesh *me;
	MFace *mface;
	int a;

	if(G.obedit) return;

	base= FIRSTBASE;
	while(base) {
		if( TESTBASELIB(base) && (base->object->type==OB_MESH)) {
			me= base->object->data;
			if(me->id.lib==0) {
				
				mface= me->mface;
				
				for(a=0; a<me->totface; a++) {
				
					mface->edcode |= ME_V1V2|ME_V2V3;
					mface++;
				}
			}
		}
		base= base->next;
	}

	allqueue(REDRAWVIEW3D, 0);
}


void convert_to_triface(int all)
{
	EditVlak *evl, *evln, *next;
	EditEdge *dia;
	
	evl= G.edvl.first;
	while(evl) {
		next= evl->next;
		if(evl->v4) {
			if(all || vlakselectedAND(evl, 1) ) {
				dia= addedgelist(evl->v1, evl->v3);
				
				evln= calloc(sizeof(EditVlak), 1);
				evln->v1= evl->v1;
				evln->v2= evl->v2;
				evln->v3= evl->v3;
				evln->e1= evl->e1;
				evln->e2= evl->e2;
				evln->e3= dia;
				evln->mat_nr= evl->mat_nr;
				evln->tface= evl->tface;
				evln->flag= evl->flag;
				addtail(&G.edvl, evln);
				
				evln= calloc(sizeof(EditVlak), 1);
				evln->v1= evl->v1;
				evln->v2= evl->v3;
				evln->v3= evl->v4;
				evln->e1= dia;
				evln->e2= evl->e3;
				evln->e3= evl->e4;
				evln->mat_nr= evl->mat_nr;
				if(evl->tface) {
					evln->tface= malloc(sizeof(TFace));
					*evln->tface= *evl->tface;
					evl->tface= 0;
					evln->tface->uv[1][0]= evln->tface->uv[2][0];
					evln->tface->uv[1][1]= evln->tface->uv[2][1];
					evln->tface->uv[2][0]= evln->tface->uv[3][0];
					evln->tface->uv[2][1]= evln->tface->uv[3][1];
					evln->tface->col[1]= evln->tface->col[2];
					evln->tface->col[2]= evln->tface->col[3];

				}
				evln->flag= evl->flag;
				addtail(&G.edvl, evln);
				
				remlink(&G.edvl, evl);
				freevlak(evl);
			}
		}
		evl= next;
	}
	
}


void deselectall_mesh()	/* toggle */
{
	EditVert *eve;
	int a;
	
	if(G.obedit->lay & G.vd->lay) {
		a= 0;
		eve= G.edve.first;
		while(eve) {
			if(eve->f & 1) {
				a= 1;
				break;
			}
			eve= eve->next;
		}
		eve= G.edve.first;
		while(eve) {
			if(eve->h==0) {
				if(a) eve->f&= -2;
				else eve->f|= 1;
			}
			eve= eve->next;
		}
		tekenvertices_ext(a==0);
	}
	countall();
}


void righthandfaces(int select)	/* maakt vlakken rechtsdraaiend */
{
	EditEdge *eed, *ed1, *ed2, *ed3, *ed4;
	EditVlak *evl, *startvl;
	float maxx, maxy, maxz, nor[3], cent[3];
	int totsel, found, foundone, direct, turn;

   /* op basis selectconnected om losse objecten te onderscheiden */

	/* tel per edge hoeveel vlakken het heeft */

	/* vind het meest linkse, voorste, bovenste vlak */

	/* zet normaal naar buiten en de eerste richtings vlaggen in de edges */

	/* loop object af en zet richtingen / richtingsvlaggen: alleen bij edges van 1 of 2 vlakken */
	/* dit is in feit de select connected */

	/* indien nog (selected) vlakken niet gedaan: opnieuw vind de meest linkse ... */

	waitcursor(1);

	eed= G.eded.first;
	while(eed) {
		eed->f= 0;
		eed->f1= 0;
		eed= eed->next;
	}

	/* vlakken en edges tellen */
	totsel= 0;
	evl= G.edvl.first;
	while(evl) {
		if(select==0 || vlakselectedAND(evl, 1) ) {
			evl->f= 1;
			totsel++;
			evl->e1->f1++;
			evl->e2->f1++;
			evl->e3->f1++;
			if(evl->v4) evl->e4->f1++;
		}
		else evl->f= 0;

		evl= evl->next;
	}

	while(totsel>0) {
		/* van buiten naar binnen */

		evl= G.edvl.first;
		startvl= 0;
		maxx= maxy= maxz= -1.0e10;

		while(evl) {
			if(evl->f) {
				CalcCent3f(cent, evl->v1->co, evl->v2->co, evl->v3->co);
				cent[0]= fabs(cent[0])+fabs(cent[1])+fabs(cent[2]);
				
				if(cent[0]>maxx) {
					maxx= cent[0];
					startvl= evl;
				}
			}
			evl= evl->next;
		}
		
		/* eerste vlak goedzetten: normaal berekenen */
		CalcNormFloat(startvl->v1->co, startvl->v2->co, startvl->v3->co, nor);
		CalcCent3f(cent, startvl->v1->co, startvl->v2->co, startvl->v3->co);
		
		/* eerste normaal staat zus of zo */
		if(select) {
			if(select==2) {
				if(cent[0]*nor[0]+cent[1]*nor[1]+cent[2]*nor[2] > 0.0) flipvlak(startvl);
			}
			else {
				if(cent[0]*nor[0]+cent[1]*nor[1]+cent[2]*nor[2] < 0.0) flipvlak(startvl);
			}
		}
		else if(cent[0]*nor[0]+cent[1]*nor[1]+cent[2]*nor[2] < 0.0) flipvlak(startvl);


		eed= startvl->e1;
		if(eed->v1==startvl->v1) eed->f= 1; 
		else eed->f= 2;
		
		eed= startvl->e2;
		if(eed->v1==startvl->v2) eed->f= 1; 
		else eed->f= 2;
		
		eed= startvl->e3;
		if(eed->v1==startvl->v3) eed->f= 1; 
		else eed->f= 2;
		
		eed= startvl->e4;
		if(eed) {
			if(eed->v1==startvl->v4) eed->f= 1; 
			else eed->f= 2;
		}
		
		startvl->f= 0;
		totsel--;

		/* de normalen testen */
		found= 1;
		direct= 1;
		while(found) {
			found= 0;
			if(direct) evl= G.edvl.first;
			else evl= G.edvl.last;
			while(evl) {
				if(evl->f) {
					turn= 0;
					foundone= 0;

					ed1= evl->e1;
					ed2= evl->e2;
					ed3= evl->e3;
					ed4= evl->e4;

					if(ed1->f) {
						if(ed1->v1==evl->v1 && ed1->f==1) turn= 1;
						if(ed1->v2==evl->v1 && ed1->f==2) turn= 1;
						foundone= 1;
					}
					else if(ed2->f) {
						if(ed2->v1==evl->v2 && ed2->f==1) turn= 1;
						if(ed2->v2==evl->v2 && ed2->f==2) turn= 1;
						foundone= 1;
					}
					else if(ed3->f) {
						if(ed3->v1==evl->v3 && ed3->f==1) turn= 1;
						if(ed3->v2==evl->v3 && ed3->f==2) turn= 1;
						foundone= 1;
					}
					else if(ed4 && ed4->f) {
						if(ed4->v1==evl->v4 && ed4->f==1) turn= 1;
						if(ed4->v2==evl->v4 && ed4->f==2) turn= 1;
						foundone= 1;
					}

					if(foundone) {
						found= 1;
						totsel--;
						evl->f= 0;

						if(turn) {
							if(ed1->v1==evl->v1) ed1->f= 2; 
							else ed1->f= 1;
							if(ed2->v1==evl->v2) ed2->f= 2; 
							else ed2->f= 1;
							if(ed3->v1==evl->v3) ed3->f= 2; 
							else ed3->f= 1;
							if(ed4) {
								if(ed4->v1==evl->v4) ed4->f= 2; 
								else ed4->f= 1;
							}

							flipvlak(evl);

						}
						else {
							if(ed1->v1== evl->v1) ed1->f= 1; 
							else ed1->f= 2;
							if(ed2->v1==evl->v2) ed2->f= 1; 
							else ed2->f= 2;
							if(ed3->v1==evl->v3) ed3->f= 1; 
							else ed3->f= 2;
							if(ed4) {
								if(ed4->v1==evl->v4) ed4->f= 1; 
								else ed4->f= 2;
							}
						}
					}
				}
				if(direct) evl= evl->next;
				else evl= evl->prev;
			}
			direct= 1-direct;
		}
	}

	recalc_editnormals();
	
	makeDispList(G.obedit);
	
	waitcursor(0);
}

EditVert *findnearestvert(sel)
short sel;
{
	/* als sel==1 krijgen vertices met flag==1 een nadeel */
	EditVert *eve,*act=0;
	static EditVert *acto=0;
	short dist=100,temp,mval[2];

	if(G.edve.first==0) return 0;

	/* projektie doen */
	calc_meshverts_ext();	/* drawobject.c */
	
	/* er wordt geteld van acto->next tot last en van first tot acto */
	/* bestaat acto ? */
	eve= G.edve.first;
	while(eve) {
		if(eve==acto) break;
		eve= eve->next;
	}
	if(eve==0) acto= G.edve.first;

	if(acto==0) return 0;

	/* is er een aangegeven vertex? deel 1 */
	getmouseco_areawin(mval);
	eve= acto->next;
	while(eve) {
		if(eve->h==0) {
			temp= abs(mval[0]- eve->xs)+ abs(mval[1]- eve->ys);
			if( (eve->f & 1)==sel ) temp+=5;
			if(temp<dist) {
				act= eve;
				dist= temp;
				if(dist<4) break;
			}
		}
		eve= eve->next;
	}
	/* is er een aangegeven vertex? deel 2 */
	if(dist>3) {
		eve= G.edve.first;
		while(eve) {
			if(eve->h==0) {
				temp= abs(mval[0]- eve->xs)+ abs(mval[1]- eve->ys);
				if( (eve->f & 1)==sel ) temp+=5;
				if(temp<dist) {
					act= eve;
					if(temp<4) break;
					dist= temp;
				}
				if(eve== acto) break;
			}
			eve= eve->next;
		}
	}

	acto= act;
	return act;
}

void tekenvertices_special(int mode, EditVert *act)
{
	/* voor speciale gevallen:
	 * mode 0: deselect geselecteerde, teken ze,  behalve act
	 * mode 1: teken alleen act
	 */
	ScrArea *tempsa, *sa;
	View3D *vd;
	EditVert *eve;
	float mat[4][4];
	int doit=0;
	
	/* eerst testen of er wel special vertices zijn */
	
	eve= (EditVert *)G.edve.first;
	while(eve) {
		eve->f1= 0;
		if(eve->h==0) {
			if(mode==0) {
				if(eve!=act && eve->f & 1) {
					doit= 1;
					eve->f1= 1;
					eve->f -= 1;
				}
			}
			else if(mode==1) {
				if(eve==act) eve->f1= 1;
				doit= 1;
			}
		}
		eve= eve->next;
	}
	if(doit==0) return;
	
	if(G.f & (G_FACESELECT+G_DRAWFACES)) {
		addqueue(curarea->win, REDRAW, 1);
		return;
	}
	
	if(G.zbuf) glDisable(GL_DEPTH_TEST);
	
	glDrawBuffer(GL_FRONT);

	/* alle views aflopen */
	tempsa= curarea;
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->spacetype==SPACE_VIEW3D) {
			vd= sa->spacedata.first;
			if(G.obedit->lay & vd->lay) {
				areawinset(sa->win);
				multmatrix(G.obedit->obmat);

				Mat4SwapMat4(G.vd->persmat, mat);
				mygetsingmatrix(G.vd->persmat);
			
				tekenvertices(0);
				tekenvertices(1);
				
				Mat4SwapMat4(G.vd->persmat, mat);

				sa->win_swap= WIN_FRONT_OK;
				
				loadmatrix(G.vd->viewmat);
			}
		}
		sa= sa->next;
	}
	if(curarea!=tempsa) areawinset(tempsa->win);
	
	glDrawBuffer(GL_BACK);
	if(G.zbuf) glEnable(GL_DEPTH_TEST);
}

void mouse_mesh()
{
	EditVert *act=0;

	act= findnearestvert(1);
	if(act) {
		
		if((G.qual & LR_SHIFTKEY)==0) {
			tekenvertices_special(0, act);
		}
		if( (act->f & 1)==0) act->f+= 1;
		else if(G.qual & LR_SHIFTKEY) act->f-= 1;

		tekenvertices_special(1, act);
		countall();
	}

	rightmouse_transform();
}

void selectconnectedAll()
{
	EditVert *v1,*v2;
	EditEdge *eed;
	short flag=1,toggle=0;

	if(G.eded.first==0) return;

	while(flag==1) {
		flag= 0;
		toggle++;
		if(toggle & 1) eed= G.eded.first;
		else eed= G.eded.last;
		while(eed) {
			v1= eed->v1;
			v2= eed->v2;
			if(eed->h==0) {
				if(v1->f & 1) {
					if( (v2->f & 1)==0 ) {
						v2->f |= 1;
						flag= 1;
					}
				}
				else if(v2->f & 1) {
					if( (v1->f & 1)==0 ) {
						v1->f |= 1;
						flag= 1;
					}
				}
			}
			if(toggle & 1) eed= eed->next;
			else eed= eed->prev;
		}
	}
	countall();

	tekenvertices_ext(1);

}


void selectconnected_mesh()
{
	EditVert *eve,*v1,*v2,*act= 0;
	EditEdge *eed;
	short flag=1,sel,toggle=0;

	if(G.eded.first==0) return;

	if(G.qual & LR_CTRLKEY) {
		selectconnectedAll();
		return;
	}

	sel= 3;
	if(G.qual & LR_SHIFTKEY) sel=2;

	act= findnearestvert(sel-2);
	if(act==0) {
		error(" Nothing indicated ");
		return;
	}

	/* testflaggen wissen */
	eve= G.edve.first;
	while(eve) {
		eve->f&= ~2;
		eve= eve->next;
	}
	act->f= (act->f & ~3) | sel;

	while(flag==1) {
		flag= 0;
		toggle++;
		if(toggle & 1) eed= G.eded.first;
		else eed= G.eded.last;
		while(eed) {
			v1= eed->v1;
			v2= eed->v2;
			if(eed->h==0) {
				if(v1->f & 2) {
					if( (v2->f & 2)==0 ) {
						v2->f= (v2->f & ~3) | sel;
						flag= 1;
					}
				}
				else if(v2->f & 2) {
					if( (v1->f & 2)==0 ) {
						v1->f= (v1->f & ~3) | sel;
						flag= 1;
					}
				}
			}
			if(toggle & 1) eed= eed->next;
			else eed= eed->prev;
		}
	}
	countall();
	
	tekenvertices_ext( sel==3 );
}


short extrudeflag(short flag,short type)
{
	/* als type=1 worden oude extrudevlakken verwijderd (ivm spin etc) */
	/* alle verts met (flag & 'flag') extrude */
	/* van oude wordt flag 'flag' gewist, van nieuwe gezet */

	EditVert *eve, *v1, *v2, *v3, *v4, *nextve;
	EditEdge *eed, *e1, *e2, *e3, *e4, *nexted;
	EditVlak *evl, *nextvl, *evln;
	short sel=0, deloud= 0;

	if(G.obedit==0 || get_mesh(G.obedit)==0) return 0;

	/* de vert flag f1 wissen, hiermee test op losse geselecteerde vert */
	eve= G.edve.first;
	while(eve) {
		if(eve->f & flag) eve->f1= 1;
		else eve->f1= 0;
		eve= eve->next;
	}
	/* de edges tellerflag wissen, als selected op 1 zetten */
	eed= G.eded.first;
	while(eed) {
		if( (eed->v1->f & flag) && (eed->v2->f & flag) ) {
			eed->f= 1;
			eed->v1->f1= 0;
			eed->v2->f1= 0;
		}
		else eed->f= 0;
		
		eed->f1= 1;		/* aangeven is 'oude' edge (er worden in deze routine nieuwe gemaakt */
		
		eed= eed->next;
	}


	/* in alle vlak sel een dupl.flag zetten en bijhorende edgeflags ophogen */

	evl= G.edvl.first;
	while(evl) {
		evl->f= 0;
		
		if(vlakselectedAND(evl, flag)) {
			e1= evl->e1;
			e2= evl->e2;
			e3= evl->e3;
			e4= evl->e4;

			if(e1->f < 3) e1->f++;
			if(e2->f < 3) e2->f++;
			if(e3->f < 3) e3->f++;
			if(e4 && e4->f < 3) e4->f++;
			evl->f= 1;
		}
		else if(vlakselectedOR(evl, flag)) {
			e1= evl->e1;
			e2= evl->e2;
			e3= evl->e3;
			e4= evl->e4;
			
			if( (e1->v1->f & flag) && (e1->v2->f & flag) ) e1->f1= 2;
			if( (e2->v1->f & flag) && (e2->v2->f & flag) ) e2->f1= 2;
			if( (e3->v1->f & flag) && (e3->v2->f & flag) ) e3->f1= 2;
			if( e4 && (e4->v1->f & flag) && (e4->v2->f & flag) ) e4->f1= 2;
		}
		
		evl= evl->next;
	}

	/* set direction of edges */
	evl= G.edvl.first;
	while(evl) {
		if(evl->f== 0) {
			if(evl->e1->f==2) {
				if(evl->e1->v1 == evl->v1) evl->e1->dir= 0;
				else evl->e1->dir= 1;
			}
			if(evl->e2->f==2) {
				if(evl->e2->v1 == evl->v2) evl->e2->dir= 0;
				else evl->e2->dir= 1;
			}
			if(evl->e3->f==2) {
				if(evl->e3->v1 == evl->v3) evl->e3->dir= 0;
				else evl->e3->dir= 1;
			}
			if(evl->e4 && evl->e4->f==2) {
				if(evl->e4->v1 == evl->v4) evl->e4->dir= 0;
				else evl->e4->dir= 1;
			}
		}
		evl= evl->next;
	}	


	/* de stand van zaken nu:
		eve->f1==1: losse selected vertex 

		eed->f==0 : edge niet selected, geen extrude
		eed->f==1 : edge selected, komt niet in vlak voor, extrude
		eed->f==2 : edge selected, komt 1 keer in vlak voor, extrude
		eed->f==3 : edge selected, komt in meer vlakken voor, geen extrude
		
		eed->f1==0: nieuwe edge
		eed->f1==1: edge selected,  komt in selected vlak voor,  als f==3: remove
		eed->f1==2: edge selected, komt in NIET selected vlak voor
					
					
		evl->f==1 : vlak dupliceren
	*/

	/* alle geselecteerde vertices kopieeren, */
	/* de pointer naar nieuwe vert in oude struct schrijven op eve->vn */
	eve= G.edve.last;
	while(eve) {
		eve->f&= ~128;  /* wissen voor test later op losse verts */
		if(eve->f & flag) {
			sel= 1;
			v1= addvertlist(0);
			
			VECCOPY(v1->co, eve->co);
			v1->f= eve->f;
			eve->f-= flag;
			eve->vn= v1;
		}
		else eve->vn= 0;
		eve= eve->prev;
	}

	if(sel==0) return 0;

	/* alle edges met eed->f==1 of eed->f==2 worden vlakken */
	/* als deloud==1 worden edges eed->f>2 verwijderd */
	eed= G.eded.last;
	while(eed) {
		nexted= eed->prev;
		if( eed->f<3) {
			eed->v1->f|=128;  /* =geen losse vert! */
			eed->v2->f|=128;
		}
		if( (eed->f==1 || eed->f==2) ) {
			if(eed->f1==2) deloud=1;
			
			/* that dir thing does work somewhat... */
			
			if(eed->dir==1) addvlaklist(eed->v1, eed->v2, eed->v2->vn, eed->v1->vn, G.obedit->actcol-1, 0, 0);
			else addvlaklist(eed->v2, eed->v1, eed->v1->vn, eed->v2->vn, G.obedit->actcol-1, 0, 0);
		}

		eed= nexted;
	}
	if(deloud) {
		eed= G.eded.first;
		while(eed) {
			nexted= eed->next;
			if(eed->f==3 && eed->f1==1) {
				remedge(eed);
				free(eed);
			}
			eed= nexted;
		}
	}
	/* de vlakken dupliceren, eventueel oude verwijderen  */
	evl= G.edvl.first;
	while(evl) {
		nextvl= evl->next;
		if(evl->f & 1) {
		
			v1= evl->v1->vn;
			v2= evl->v2->vn;
			v3= evl->v3->vn;
			if(evl->v4) v4= evl->v4->vn; else v4= 0;
			
			evln= addvlaklist(v1, v2, v3, v4, evl->mat_nr, 0, evl->tface);
			
			if(deloud) {
				remlink(&G.edvl, evl);
				freevlak(evl);
			}
			
		}
		evl= nextvl;
	}
	/* alle verts met eve->vn!=0 
		als eve->f1==1: edge maken
		als flag!=128 :als deloud==1:  verwijderen
	*/
	eve= G.edve.last;
	while(eve) {
		nextve= eve->prev;
		if(eve->vn) {
			if(eve->f1==1) addedgelist(eve,eve->vn);
			else if( (eve->f & 128)==0) {
				if(deloud) {
					remlink(&G.edve,eve);
					free(eve);
				}
			}
		}
		eve->f&= ~128;
		
		eve= nextve;
	}

	/* debug temp: testen op consistente:
	evl= G.edvl.first;
	while(evl) {
		e1= findedgelist(evl->v1, evl->v2);
		e2= findedgelist(evl->v2, evl->v3);
		e3= findedgelist(evl->v3, evl->v1);
		if(e1==0 || e2==0 || e3==0) {
			error("edge not in edgelist");
			break;
		} 
		evl= evl->next;
	}
	*/

	return 1;
}

void rotateflag(short flag, float *cent, float *rotmat)
{
	/* alle verts met (flag & 'flag') rotate */

	EditVert *eve;

	eve= G.edve.first;
	while(eve) {
		if(eve->f & flag) {
			eve->co[0]-=cent[0];
			eve->co[1]-=cent[1];
			eve->co[2]-=cent[2];
			Mat3MulVecfl(rotmat,eve->co);
			eve->co[0]+=cent[0];
			eve->co[1]+=cent[1];
			eve->co[2]+=cent[2];
		}
		eve= eve->next;
	}
}

void translateflag(short flag, float *vec)
{
	/* alle verts met (flag & 'flag') translate */

	EditVert *eve;

	eve= G.edve.first;
	while(eve) {
		if(eve->f & flag) {
			eve->co[0]+=vec[0];
			eve->co[1]+=vec[1];
			eve->co[2]+=vec[2];
		}
		eve= eve->next;
	}
}

short removedoublesflag(short flag, float limit)		/* return aantal */
{
	/* alle verts met (flag & 'flag') worden getest */
	EditVert *eve, *v1, *nextve;
	EditEdge *eed, *e1, *nexted;
	EditVlak *evl, *nextvl;
	struct xvertsort *sortblock, *sb, *sb1;
	struct vlaksort *vlsortblock, *vsb, *vsb1;
	float dist;
	int a, b, test, aantal;

	/* flag 128 wordt gewist, aantal tellen */
	eve= G.edve.first;
	aantal= 0;
	while(eve) {
		eve->f&= ~128;
		if(eve->f & flag) aantal++;
		eve= eve->next;
	}
	if(aantal==0) return 0;

	/* geheugen reserveren en qsorten */
	sb= sortblock= (struct xvertsort *)mallocN(sizeof(struct xvertsort)*aantal,"sortremovedoub");
	eve= G.edve.first;
	while(eve) {
		if(eve->f & flag) {
			sb->x= eve->co[0]+eve->co[1]+eve->co[2];
			sb->v1= eve;
			sb++;
		}
		eve= eve->next;
	}
	qsort(sortblock, aantal, sizeof(struct xvertsort), (void *)vergxco);

	/* testen op doubles */
	sb= sortblock;
	for(a=0; a<aantal; a++) {
		eve= sb->v1;
		if( (eve->f & 128)==0 ) {
			sb1= sb+1;
			for(b=a+1; b<aantal; b++) {
				/* eerste test: simpel dist */
				dist= sb1->x - sb->x;
				if(dist > limit) break;
				
				/* tweede test: is vertex toegestaan */
				v1= sb1->v1;
				if( (v1->f & 128)==0 ) {
					
					dist= fabs(v1->co[0]-eve->co[0]);
					if(dist<=limit) {
						dist= fabs(v1->co[1]-eve->co[1]);
						if(dist<=limit) {
							dist= fabs(v1->co[2]-eve->co[2]);
							if(dist<=limit) {
								v1->f|= 128;
								v1->vn= eve;
							}
						}
					}
				}
				sb1++;
			}
		}
		sb++;
	}
	freeN(sortblock);

	/* edges testen en opnieuw invoegen */
	eed= G.eded.first;
	while(eed) {
		eed->f= 0;
		eed= eed->next;
	}
	eed= G.eded.last;
	while(eed) {
		nexted= eed->prev;

		if(eed->f==0) {
			if( (eed->v1->f & 128) || (eed->v2->f & 128) ) {
				remedge(eed);

				if(eed->v1->f & 128) eed->v1= eed->v1->vn;
				if(eed->v2->f & 128) eed->v2= eed->v2->vn;

				e1= addedgelist(eed->v1,eed->v2);
				
				if(e1) e1->f= 1;
				if(e1!=eed) free(eed);
			}
		}
		eed= nexted;
	}

	/* eerst aantal testvlakken tellen */
	evl= (struct EditVlak *)G.edvl.first;
	aantal= 0;
	while(evl) {
		evl->f= 0;
		if(evl->v1->f & 128) evl->f= 1;
		else if(evl->v2->f & 128) evl->f= 1;
		else if(evl->v3->f & 128) evl->f= 1;
		else if(evl->v4 && (evl->v4->f & 128)) evl->f= 1;
		
		if(evl->f==1) aantal++;
		evl= evl->next;
	}

	/* vlakken testen op dubbele punten en eventueel verwijderen */
	evl= (struct EditVlak *)G.edvl.first;
	while(evl) {
		nextvl= evl->next;
		if(evl->f==1) {
			
			if(evl->v1->f & 128) evl->v1= evl->v1->vn;
			if(evl->v2->f & 128) evl->v2= evl->v2->vn;
			if(evl->v3->f & 128) evl->v3= evl->v3->vn;
			if(evl->v4 && (evl->v4->f & 128)) evl->v4= evl->v4->vn;
		
			test= 0;
			if(evl->v1==evl->v2) test+=1;
			if(evl->v2==evl->v3) test+=2;
			if(evl->v3==evl->v1) test+=4;
			if(evl->v4==evl->v1) test+=8;
			if(evl->v3==evl->v4) test+=16;
			if(evl->v2==evl->v4) test+=32;
			
			if(test) {
				if(evl->v4) {
					if(test==1 || test==2) {
						evl->v2= evl->v3;
						evl->v3= evl->v4;
						evl->v4= 0;
						test= 0;
					}
					else if(test==8 || test==16) {
						evl->v4= 0;
						test= 0;
					}
					else {
						remlink(&G.edvl, evl);
						freevlak(evl);
						aantal--;
					}
				}
				else {
					remlink(&G.edvl, evl);
					freevlak(evl);
					aantal--;
				}
			}
			
			if(test==0) {
				/* edgepointers goedzetten */
				evl->e1= findedgelist(evl->v1, evl->v2);
				evl->e2= findedgelist(evl->v2, evl->v3);
				if(evl->v4==0) {
					evl->e3= findedgelist(evl->v3, evl->v1);
					evl->e4= 0;
				}
				else {
					evl->e3= findedgelist(evl->v3, evl->v4);
					evl->e4= findedgelist(evl->v4, evl->v1);
				}
			}
		}
		evl= nextvl;
	}

	/* dubbele vlakken: sortblock */
	/* opnieuw tellen, nu alle selected vlakken */
	aantal= 0;
	evl= G.edvl.first;
	while(evl) {
		evl->f= 0;
		if(vlakselectedAND(evl, 1)) {
			evl->f= 1;
			aantal++;
		}
		evl= evl->next;
	}

	if(aantal) {
		/* dubbele vlakken: sortblock */
		vsb= vlsortblock= mallocN(sizeof(struct vlaksort)*aantal, "sortremovedoub");
		evl= G.edvl.first;
		while(evl) {
			if(evl->f & 1) {
				if(evl->v4) vsb->x= (long) MIN4( (long)evl->v1, (long)evl->v2, (long)evl->v3, (long)evl->v4);
				else vsb->x= (long) MIN3( (long)evl->v1, (long)evl->v2, (long)evl->v3);

				vsb->evl= evl;
				vsb++;
			}
			evl= evl->next;
		}
		
		qsort(vlsortblock, aantal, sizeof(struct vlaksort), (void *)vergvlak);
			
		vsb= vlsortblock;
		for(a=0; a<aantal; a++) {
			evl= vsb->evl;
			if( (evl->f & 128)==0 ) {
				vsb1= vsb+1;

				for(b=a+1; b<aantal; b++) {
				
					/* eerste test: zelfde poin? */
					if(vsb->x != vsb1->x) break;
					
					/* tweede test: is test toegestaan */
					evl= vsb1->evl;
					if( (evl->f & 128)==0 ) {
						if( comparevlak(evl, vsb->evl)) evl->f |= 128;
						
					}
					vsb1++;
				}
			}
			vsb++;
		}
		
		freeN(vlsortblock);
		
		/* dubbele vlakken eruit */
		evl= (struct EditVlak *)G.edvl.first;
		while(evl) {
			nextvl= evl->next;
			if(evl->f & 128) {
				remlink(&G.edvl, evl);
				freevlak(evl);
			}
			evl= nextvl;
		}
	}
	
	/* dubbele vertices eruit */
	a= 0;
	eve= (struct EditVert *)G.edve.first;
	while(eve) {
		nextve= eve->next;
		if(eve->f & flag) {
			if(eve->f & 128) {
				a++;
				remlink(&G.edve, eve);
				
				free(eve);
			}
		}
		eve= nextve;
	}
	return a;	/* aantal */
}

void xsortvert_flag(flag)
short flag;
{
	/* alle verts met (flag & 'flag') worden gesorteerd */
	EditVert *eve;
	struct xvertsort *sortblock, *sb;
	ListBase tbase;
	int aantal;
	
	/* aantal tellen */
	eve= G.edve.first;
	aantal= 0;
	while(eve) {
		if(eve->f & flag) aantal++;
		eve= eve->next;
	}
	if(aantal==0) return;

	/* geheugen reserveren en qsorten */
	sb= sortblock= (struct xvertsort *)mallocN(sizeof(struct xvertsort)*aantal,"sortremovedoub");
	eve= G.edve.first;
	while(eve) {
		if(eve->f & flag) {
			sb->x= eve->xs;
			sb->v1= eve;
			sb++;
		}
		eve= eve->next;
	}
	qsort(sortblock, aantal, sizeof(struct xvertsort), (void *)vergxco);
	
	/* tijdelijke listbase maken */
	tbase.first= tbase.last= 0;
	sb= sortblock;
	while(aantal--) {
		eve= sb->v1;
		remlink(&G.edve, eve);
		addtail(&tbase, eve);
		sb++;
	}
	
	addlisttolist(&G.edve, &tbase);
	
	freeN(sortblock);
}


void hashvert_flag(int flag)
{
	EditVert *eve;
	struct xvertsort *sortblock, *sb, onth, *new;
	ListBase tbase;
	float mult;
	int aantal, a, b;
	
	/* aantal tellen */
	eve= G.edve.first;
	aantal= 0;
	while(eve) {
		if(eve->f & flag) aantal++;
		eve= eve->next;
	}
	if(aantal==0) return;

	/* geheugen reserveren */
	sb= sortblock= (struct xvertsort *)mallocN(sizeof(struct xvertsort)*aantal,"sortremovedoub");
	eve= G.edve.first;
	while(eve) {
		if(eve->f & flag) {
			sb->v1= eve;
			sb++;
		}
		eve= eve->next;
	}

	srandom(1);
	mult= ((float)aantal)/32768.0;
	
	sb= sortblock;
	for(a=0; a<aantal; a++, sb++) {
		b= (int) (mult*( (float)(random() & 32767) ));
		if(b>=0 && b<aantal) {
			new= sortblock+b;
			onth= *sb;
			*sb= *new;
			*new= onth;
		}
	}

	/* tijdelijke listbase maken */
	tbase.first= tbase.last= 0;
	sb= sortblock;
	while(aantal--) {
		eve= sb->v1;
		remlink(&G.edve, eve);
		addtail(&tbase, eve);
		sb++;
	}
	
	addlisttolist(&G.edve, &tbase);
	
	freeN(sortblock);
}

uint cpack_half(uint col1, uint col2)
{
	char *cp1, *cp2, *cp;
	uint col=0;
	
	cp1= (char *)&col1;
	cp2= (char *)&col2;
	cp=  (char *)&col;
	
	cp[1]= (cp1[1]+cp2[1])>>1;
	cp[2]= (cp1[2]+cp2[2])>>1;
	cp[3]= (cp1[3]+cp2[3])>>1;
	
	return col;
}


void uv_half(short *uv, short *uv1, short *uv2)
{
	uv[0]= (uv1[0]+uv2[0])>>1;
	uv[1]= (uv1[1]+uv2[1])>>1;
	
}

void uv_quart(short *uv, short *uv1)
{
	uv[0]= (uv1[0]+uv1[2]+uv1[4]+uv1[6])>>2;
	uv[1]= (uv1[1]+uv1[3]+uv1[5]+uv1[7])>>2;
}

void set_wuv(int tot, EditVlak *evl, int v1, int v2, int v3, int v4)
{
	/* deze vreemde fie alleen bij de subdiv te gebruiken, de 'w' in de naam slaat nergens op! */
	TFace tface;
	uint *col, col1, col2;
	int a, v;
	short *uv;
	
	if(evl->tface==0) return;
	tface= *(evl->tface);
	uv= evl->tface->uv[0];
	col= evl->tface->col;
	
	if(tot==4) {
		for(a=0; a<4; a++, uv+=2, col++) {
			if(a==0) v= v1;
			else if(a==1) v= v2;
			else if(a==2) v= v3;
			else v= v4;
			
			if(a==3 && v4==0) break;
			
			if(v<=4) {
				memcpy(uv, tface.uv[v-1], 4);
				*col= tface.col[v-1];
			}
			else if(v==8) {
				uv_half(uv, tface.uv[3], tface.uv[0]);
				*col= cpack_half(tface.col[3], tface.col[0]);
			}
			else if(v==9) {
				uv_quart(uv, tface.uv[0]);
				col1= cpack_half(tface.col[1], tface.col[0]);
				col2= cpack_half(tface.col[2], tface.col[3]);
				*col= cpack_half(col1, col2);
			}
			else {
				uv_half(uv, tface.uv[v-5], tface.uv[v-4]);
				*col= cpack_half(tface.col[v-5], tface.col[v-4]);
			}
		}
	}
	else {
		for(a=0; a<3; a++, uv+=2, col++) {
			if(a==0) v= v1;
			else if(a==1) v= v2;
			else v= v3;
		
			if(v<=4) {
				memcpy(uv, tface.uv[v-1], 4);
				*col= tface.col[v-1];
			}
			else if(v==7) {
				uv_half(uv, tface.uv[2], tface.uv[0]);
				*col= cpack_half(tface.col[2], tface.col[0]);
			}
			else {
				uv_half(uv, tface.uv[v-5], tface.uv[v-4]);
				*col= cpack_half(tface.col[v-5], tface.col[v-4]);
			}
		}
	}
}

EditVert *vert_from_number(EditVlak *evl, int nr)
{
	switch(nr) {
	case 0:
		return 0;
	case 1:
		return evl->v1;
	case 2:
		return evl->v2;
	case 3:
		return evl->v3;
	case 4:
		return evl->v4;
	case 5:
		return evl->e1->vn;
	case 6:
		return evl->e2->vn;
	case 7:
		return evl->e3->vn;
	case 8:
		return evl->e4->vn;
	}
	
	return NULL;
}

void addvlak_subdiv(EditVlak *evl, int val1, int val2, int val3, int val4, EditVert *eve)
{
	EditVlak *w;
	EditVert *v1, *v2, *v3, *v4;
	
	if(val1==9) v1= eve;
	else v1= vert_from_number(evl, val1);
	
	if(val2==9) v2= eve;
	else v2= vert_from_number(evl, val2);

	if(val3==9) v3= eve;
	else v3= vert_from_number(evl, val3);

	if(val4==9) v4= eve;
	else v4= vert_from_number(evl, val4);
	
	w= addvlaklist(v1, v2, v3, v4, evl->mat_nr, evl->flag, evl->tface);

	if(w->tface) {
		if(evl->v4) set_wuv(4, w, val1, val2, val3, val4);
		else set_wuv(3, w, val1, val2, val3, val4);
	}
}

static float smoothperc= 0.0;

void smooth_subdiv_vec(float *v1, float *v2, float *n1, float *n2, float *vec)
{
	float len, fac, nor[3], nor1[3], nor2[3];
	
	VecSubf(nor, v1, v2);
	len= 0.5*Normalise(nor);

	VECCOPY(nor1, n1);
	VECCOPY(nor2, n2);

	/* cosine angle */
	fac= nor[0]*nor1[0] + nor[1]*nor1[1] + nor[2]*nor1[2] ;
	
	vec[0]= fac*nor1[0];
	vec[1]= fac*nor1[1];
	vec[2]= fac*nor1[2];

	/* cosine angle */
	fac= -nor[0]*nor2[0] - nor[1]*nor2[1] - nor[2]*nor2[2] ;
	
	vec[0]+= fac*nor2[0];
	vec[1]+= fac*nor2[1];
	vec[2]+= fac*nor2[2];

	vec[0]*= smoothperc*len;
	vec[1]*= smoothperc*len;
	vec[2]*= smoothperc*len;
}

void smooth_subdiv_quad(EditVlak *evl, float *vec)
{
	
	float nor1[3], nor2[3];
	float vec1[3], vec2[3];
	float cent[3];
	
	/* vlr->e1->vn is new vertex inbetween v1 / v2 */
	
	VecMidf(nor1, evl->v1->no, evl->v2->no);
	Normalise(nor1);
	VecMidf(nor2, evl->v3->no, evl->v4->no);
	Normalise(nor2);

	smooth_subdiv_vec( evl->e1->vn->co, evl->e3->vn->co, nor1, nor2, vec1);

	VecMidf(nor1, evl->v2->no, evl->v3->no);
	Normalise(nor1);
	VecMidf(nor2, evl->v4->no, evl->v1->no);
	Normalise(nor2);

	smooth_subdiv_vec( evl->e2->vn->co, evl->e4->vn->co, nor1, nor2, vec2);

	VecAddf(vec1, vec1, vec2);

	CalcCent4f(cent, evl->v1->co,  evl->v2->co,  evl->v3->co,  evl->v4->co);
	VecAddf(vec, cent, vec1);
}

void subdivideflag(int flag, float rad, int beauty)
{
	/* divide alle vlakken met (vertflag & flag) */
	/* als rad>0.0 zet dan nieuw vert op afstand rad van 0,0,0 */
	extern float doublimit;
	EditVert *eve;
	EditEdge *eed, *e1, *e2, *e3, *e4, *nexted;
	EditVlak *evl;
	float fac, vec[3], vec1[3], len1, len2, len3;
	short test;
	
	if(beauty & B_SMOOTH) {
		short perc= 100;

		if(button(&perc, 10, 500, "Percentage:")==0) return;
		
		smoothperc= 0.292*perc/100.0;
	}

	/* edgeflags */
	eed= G.eded.first;
	while(eed) {
		
		if( (eed->v1->f & flag) && (eed->v2->f & flag) ) eed->f= flag;
		else eed->f= 0;
		
		eed= eed->next;
	}
	
	/* als beauty: opp testen en edgeflags wissen van 'lelijke' edges */
	if(beauty & B_BEAUTY) {
		evl= G.edvl.first;
		while(evl) {
			if( vlakselectedAND(evl, flag) ) {
				if(evl->v4) {
				
					/* opp */
					len1= AreaQ3Dfl(evl->v1->co, evl->v2->co, evl->v3->co, evl->v4->co);
					if(len1 <= doublimit) {
						evl->e1->f = 0;
						evl->e2->f = 0;
						evl->e3->f = 0;
						evl->e4->f = 0;
					}
					else {
						len1= VecLenf(evl->v1->co, evl->v2->co) + VecLenf(evl->v3->co, evl->v4->co);
						len2= VecLenf(evl->v2->co, evl->v3->co) + VecLenf(evl->v1->co, evl->v4->co);
						
						if(len1 < len2) {
							evl->e1->f = 0;
							evl->e3->f = 0;
						}
						else if(len1 > len2) {
							evl->e2->f = 0;
							evl->e4->f = 0;
						}
					}
				}
				else {

					/* opp */
					len1= AreaT3Dfl(evl->v1->co, evl->v2->co, evl->v3->co);
					if(len1 <= doublimit) {
						evl->e1->f = 0;
						evl->e2->f = 0;
						evl->e3->f = 0;
					}
					else {
	
						len1= VecLenf(evl->v1->co, evl->v2->co) ;
						len2= VecLenf(evl->v2->co, evl->v3->co) ;
						len3= VecLenf(evl->v3->co, evl->v1->co) ;
						
						if(len1<len2 && len1<len3) {
							evl->e1->f = 0;
						}
						else if(len2<len3 && len2<len1) {
							evl->e2->f = 0;
						}
						else if(len3<len2 && len3<len1) {
							evl->e3->f = 0;
						}
					}
				}
			}
			evl= evl->next;
		}
	}

	if(beauty & B_SMOOTH) {
		
		vertexnormals(0);		/* no1*/
			
	}
	
	/* nieuw punt maken en in edge wegschrijven, flag wissen! is voor vlakkenmaak stuk nodig */
	eed= G.eded.first;
	while(eed) {
		if(eed->f & flag) {
			
			vec[0]= (eed->v1->co[0]+eed->v2->co[0])/2.0;
			vec[1]= (eed->v1->co[1]+eed->v2->co[1])/2.0;
			vec[2]= (eed->v1->co[2]+eed->v2->co[2])/2.0;

			if(rad > 0.0) {   /* perf sph */
				Normalise(vec);
				vec[0]*= rad;
				vec[1]*= rad;
				vec[2]*= rad;
			}
			else if(rad< 0.0) {  /* fract */
				fac= rad* VecLenf(eed->v1->co, eed->v2->co)/16384.0;
				vec1[0]= fac*( (float)(random() & 32767) -16384);
				vec1[1]= fac*( (float)(random() & 32767) -16384);
				vec1[2]= fac*( (float)(random() & 32767) -16384);
				VecAddf(vec, vec, vec1);
			}
			
			if(beauty & B_SMOOTH) {
				smooth_subdiv_vec(eed->v1->co, eed->v2->co, eed->v1->no, eed->v2->no, vec1);
				VecAddf(vec, vec, vec1);
			}
			
			eed->vn= addvertlist(vec);
			eed->vn->f= eed->v1->f;

		}
		else eed->vn= 0;
		
		eed->f= 0; /* moet! */
		
		eed= eed->next;
	}

	/* alle vlakken testen op subdiv edges, 8 of 16 gevallen! */

	evl= G.edvl.last;
	while(evl) {
		if( vlakselectedOR(evl, flag) ) {
			e1= evl->e1;
			e2= evl->e2;
			e3= evl->e3;
			e4= evl->e4;

			test= 0;
			if(e1 && e1->vn) { 
				test+= 1; 
				e1->f= 1;
			}
			if(e2 && e2->vn) { 
				test+= 2; 
				e2->f= 1;
			}
			if(e3 && e3->vn) { 
				test+= 4; 
				e3->f= 1;
			}
			if(e4 && e4->vn) { 
				test+= 8; 
				e4->f= 1;
			}
			
			if(test) {
				if(evl->v4==0) {
					if((test & 3)==3) addvlak_subdiv(evl, 2, 2+4, 1+4, 0, 0);
					if((test & 6)==6) addvlak_subdiv(evl, 3, 3+4, 2+4, 0, 0);
					if((test & 5)==5) addvlak_subdiv(evl, 1, 1+4, 3+4, 0, 0);

					if(test==7) {  /* vier nieuwe vlakken, oude vernieuwt */
						evl->v1= e1->vn;
						evl->v2= e2->vn;
						evl->v3= e3->vn;
						set_wuv(3, evl, 1+4, 2+4, 3+4, 0);
					}
					else if(test==3) {
						addvlak_subdiv(evl, 1+4, 2+4, 3, 0, 0);
						evl->v2= e1->vn;
						set_wuv(3, evl, 1, 1+4, 3, 0);
					}
					else if(test==6) {
						addvlak_subdiv(evl, 2+4, 3+4, 1, 0, 0);
						evl->v3= e2->vn;
						set_wuv(3, evl, 1, 2, 2+4, 0);
					}
					else if(test==5) {
						addvlak_subdiv(evl, 3+4, 1+4, 2, 0, 0);
						evl->v1= e3->vn;
						set_wuv(3, evl, 3+4, 2, 3, 0);
					}
					else if(test==1) {
						addvlak_subdiv(evl, 1+4, 2, 3, 0, 0);
						evl->v2= e1->vn;
						set_wuv(3, evl, 1, 1+4, 3, 0);
					}
					else if(test==2) {
						addvlak_subdiv(evl, 2+4, 3, 1, 0, 0);
						evl->v3= e2->vn;
						set_wuv(3, evl, 1, 2, 2+4, 0);
					}
					else if(test==4) {
						addvlak_subdiv(evl, 3+4, 1, 2, 0, 0);
						evl->v1= e3->vn;
						set_wuv(3, evl, 3+4, 2, 3, 0);
					}
					evl->e1= addedgelist(evl->v1, evl->v2);
					evl->e2= addedgelist(evl->v2, evl->v3);
					evl->e3= addedgelist(evl->v3, evl->v1);
					
				}
				else {
					if(test==15) {
						/* nog een nieuw punt toevoegen */
						CalcCent4f(vec, evl->v1->co, evl->v2->co, evl->v3->co, evl->v4->co);
						
						if(beauty & B_SMOOTH) {
							smooth_subdiv_quad(evl, vec);	/* adds */
						}
						eve= addvertlist(vec);
						
						eve->f |= flag;

						addvlak_subdiv(evl, 2, 2+4, 9, 1+4, eve);
						addvlak_subdiv(evl, 3, 3+4, 9, 2+4, eve);
						addvlak_subdiv(evl, 4, 4+4, 9, 3+4, eve);

						evl->v2= e1->vn;
						evl->v3= eve;
						evl->v4= e4->vn;
						set_wuv(4, evl, 1, 1+4, 9, 4+4);
					}
					else {
						/* kleine hoekpunten */
						if((test & 3)==3) addvlak_subdiv(evl, 1+4, 2, 2+4, 0, 0);
						if((test & 6)==6) addvlak_subdiv(evl, 2+4, 3, 3+4, 0, 0);
						if((test & 12)==12) addvlak_subdiv(evl, 3+4, 4, 4+4, 0, 0);
						if((test & 9)==9) addvlak_subdiv(evl, 4+4, 1, 1+4, 0, 0);
						
						if(test==1) {
							addvlak_subdiv(evl, 1+4, 2, 3, 0, 0);
							addvlak_subdiv(evl, 1+4, 3, 4, 0, 0);
							evl->v2= e1->vn;
							evl->v3= evl->v4;
							evl->v4= 0;
							set_wuv(4, evl, 1, 1+4, 4, 0);
						}
						else if(test==2) {
							addvlak_subdiv(evl, 2+4, 3, 4, 0, 0);
							addvlak_subdiv(evl, 2+4, 4, 1, 0, 0);
							evl->v3= e2->vn;
							evl->v4= 0;
							set_wuv(4, evl, 1, 2, 2+4, 0);
						}
						else if(test==4) {
							addvlak_subdiv(evl, 3+4, 4, 1, 0, 0);
							addvlak_subdiv(evl, 3+4, 1, 2, 0, 0);
							evl->v1= evl->v2;
							evl->v2= evl->v3;
							evl->v3= e3->vn;
							evl->v4= 0;
							set_wuv(4, evl, 2, 3, 3+4, 0);
						}
						else if(test==8) {
							addvlak_subdiv(evl, 4+4, 1, 2, 0, 0);
							addvlak_subdiv(evl, 4+4, 2, 3, 0, 0);
							evl->v1= evl->v3;
							evl->v2= evl->v4;
							evl->v3= e4->vn;
							evl->v4= 0;
							set_wuv(4, evl, 3, 4, 4+4, 0);
						}
						else if(test==3) {
							addvlak_subdiv(evl, 1+4, 2+4, 4, 0, 0);
							addvlak_subdiv(evl, 2+4, 3, 4, 0, 0);
							evl->v2= e1->vn;
							evl->v3= evl->v4;
							evl->v4= 0;
							set_wuv(4, evl, 1, 1+4, 4, 0);
						}
						else if(test==6) {
							addvlak_subdiv(evl, 2+4, 3+4, 1, 0, 0);
							addvlak_subdiv(evl, 3+4, 4, 1, 0, 0);
							evl->v3= e2->vn;
							evl->v4= 0;
							set_wuv(4, evl, 1, 2, 2+4, 0);
						}
						else if(test==12) {
							addvlak_subdiv(evl, 3+4, 4+4, 2, 0, 0);
							addvlak_subdiv(evl, 4+4, 1, 2, 0, 0);
							evl->v1= evl->v2;
							evl->v2= evl->v3;
							evl->v3= e3->vn;
							evl->v4= 0;
							set_wuv(4, evl, 2, 3, 3+4, 0);
						}
						else if(test==9) {
							addvlak_subdiv(evl, 4+4, 1+4, 3, 0, 0);
							addvlak_subdiv(evl, 1+4, 2, 3, 0, 0);
							evl->v1= evl->v3;
							evl->v2= evl->v4;
							evl->v3= e4->vn;
							evl->v4= 0;
							set_wuv(4, evl, 3, 4, 4+4, 0);
						}
						else if(test==5) {
							addvlak_subdiv(evl, 1+4, 2, 3, 3+4, 0);
							evl->v2= e1->vn;
							evl->v3= e3->vn;
							set_wuv(4, evl, 1, 1+4, 3+4, 4);
						}
						else if(test==10) {
							addvlak_subdiv(evl, 2+4, 3, 4, 4+4, 0);
							evl->v3= e2->vn;
							evl->v4= e4->vn;
							set_wuv(4, evl, 1, 2, 2+4, 4+4);
						}
						
						else if(test==7) {
							addvlak_subdiv(evl, 1+4, 2+4, 3+4, 0, 0);
							evl->v2= e1->vn;
							evl->v3= e3->vn;
							set_wuv(4, evl, 1, 1+4, 3+4, 4);
						}
						else if(test==14) {
							addvlak_subdiv(evl, 2+4, 3+4, 4+4, 0, 0);
							evl->v3= e2->vn;
							evl->v4= e4->vn;
							set_wuv(4, evl, 1, 2, 2+4, 4+4);
						}
						else if(test==13) {
							addvlak_subdiv(evl, 3+4, 4+4, 1+4, 0, 0);
							evl->v4= e3->vn;
							evl->v1= e1->vn;
							set_wuv(4, evl, 1+4, 3, 3, 3+4);
						}
						else if(test==11) {
							addvlak_subdiv(evl, 4+4, 1+4, 2+4, 0, 0);
							evl->v1= e4->vn;
							evl->v2= e2->vn;
							set_wuv(4, evl, 4+4, 2+4, 3, 4);
						}
					}
					evl->e1= addedgelist(evl->v1, evl->v2);
					evl->e2= addedgelist(evl->v2, evl->v3);
					if(evl->v4) evl->e3= addedgelist(evl->v3, evl->v4);
					else evl->e3= addedgelist(evl->v3, evl->v1);
					if(evl->v4) evl->e4= addedgelist(evl->v4, evl->v1);
					else evl->e4= 0;
				}
			}
		}
		evl= evl->prev;
	}

	/* alle oude edges verwijderen, eventueel nog nieuwe maken */
	eed= G.eded.first;
	while(eed) {
		nexted= eed->next;
		if( eed->vn ) {
			if(eed->f==0) {  /* niet gebruikt in vlak */
				addedgelist(eed->v1,eed->vn);
				addedgelist(eed->vn,eed->v2);
			}
			remedge(eed);
			free(eed);
		}
		eed= nexted;
	}
	countall();
	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);
}

void adduplicateflag(int flag)
{
	/* oude verts hebben flag 128 gezet en flag 'flag' gewist
	   nieuwe verts hebben flag 'flag' gezet */
	EditVert *eve, *v1, *v2, *v3, *v4;
	EditEdge *eed;
	EditVlak *evl;

	/* eerst vertices */
	eve= G.edve.last;
	while(eve) {
		eve->f&= ~128;
		if(eve->f & flag) {
			v1= addvertlist(eve->co);
			v1->f= eve->f;
			eve->f-= flag;
			eve->f|= 128;
			eve->vn= v1;
		}
		eve= eve->prev;
	}
	eed= G.eded.first;
	while(eed) {
		if( (eed->v1->f & 128) && (eed->v2->f & 128) ) {
			v1= eed->v1->vn;
			v2= eed->v2->vn;
			addedgelist(v1,v2);
		}
		eed= eed->next;
	}

	/* tenslotte de vlakken dupliceren */
	evl= G.edvl.first;
	while(evl) {
		if( (evl->v1->f & 128) && (evl->v2->f & 128) && (evl->v3->f & 128) ) {
			if(evl->v4) {
				if(evl->v4->f & 128) {
					v1= evl->v1->vn;
					v2= evl->v2->vn;
					v3= evl->v3->vn;
					v4= evl->v4->vn;
					addvlaklist(v1, v2, v3, v4, evl->mat_nr, evl->flag, evl->tface);
				}
			}
			else {
				v1= evl->v1->vn;
				v2= evl->v2->vn;
				v3= evl->v3->vn;
				addvlaklist(v1, v2, v3, 0, evl->mat_nr, evl->flag, evl->tface);
			}
		}
		evl= evl->next;
	}
}

void delvlakflag(int flag)
{
	/* alle vlak 3/4 verts flag + edges + losse vertices deleten */
	/* van alle verts wordt 'flag' gewist */
	EditVert *eve,*nextve;
	EditEdge *eed, *nexted;
	EditVlak *evl,*nextvl;

	eed= G.eded.first;
	while(eed) {
		eed->f= 0;
		eed= eed->next;
	}

	evl= G.edvl.first;
	while(evl) {
		nextvl= evl->next;
		if(vlakselectedAND(evl, flag)) {
			
			evl->e1->f= 1;
			evl->e2->f= 1;
			evl->e3->f= 1;
			if(evl->e4) {
				evl->e4->f= 1;
			}
								
			remlink(&G.edvl, evl);
			freevlak(evl);
		}
		evl= nextvl;
	}
	/* alle vlakken 1, 2 (3) verts select edges behouden */
	evl= G.edvl.first;
	while(evl) {
		evl->e1->f= 0;
		evl->e2->f= 0;
		evl->e3->f= 0;
		if(evl->e4) {
			evl->e4->f= 0;
		}

		evl= evl->next;
	}
	
	/* alle edges testen op vertices met flag en wissen */
	eed= G.eded.first;
	while(eed) {
		nexted= eed->next;
		if(eed->f==1) {
			remedge(eed);
			free(eed);
		}
		else if( (eed->v1->f & flag) || (eed->v2->f & flag) ) {
			eed->v1->f&= ~flag;
			eed->v2->f&= ~flag;
		}
		eed= nexted;
	}
	/* vertices met flag nog gezet zijn losse en worden verwijderd */
	eve= G.edve.first;
	while(eve) {
		nextve= eve->next;
		if(eve->f & flag) {
			remlink(&G.edve, eve);
			free(eve);
		}
		eve= nextve;
	}

}

void extrude_mesh()
{
	short a;

	TEST_EDITMESH

	if(okee("Extrude")==0) return;

	waitcursor(1);

	a= extrudeflag(1,1);
	waitcursor(0);
	if(a==0) {
		error("Can't extrude");
	}
	else {
		countall();  /* voor G.totvert in calc_meshverts() */
		calc_meshverts();
		transform('d');
	}

}

void adduplicate_mesh()
{

	TEST_EDITMESH

	waitcursor(1);
	adduplicateflag(1);
	waitcursor(0);

	countall();  /* voor G.totvert in calc_meshverts() */
	transform('d');
}

void split_mesh()
{

	TEST_EDITMESH

	if(okee(" Split ")==0) return;

	waitcursor(1);

	/* eerst duplicate maken */
	adduplicateflag(1);
	/* oude vlakken hebben 3x flag 128 gezet, deze deleten */
	delvlakflag(128);

	waitcursor(0);

	countall();
	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);
}

void separate_mesh()
{
	EditVert *eve, *v1;
	EditEdge *eed, *e1;
	EditVlak *evl, *vl1;
	Object *oldob;
	Mesh *me, *men;
	Base *base, *oldbase;
	ListBase edve, eded, edvl;
	float trans[9];
	int ok, flag;
	
	TEST_EDITMESH
	
	if(okee("Separate")==0) return;

	waitcursor(1);
	
	me= get_mesh(G.obedit);
	if(me->key) {
		error("Can't separate with vertex keys");
		return;
	}
	
	/* we gaan de zaak als volgt neppen:
	 * 1. duplicate object: dit wordt de nieuwe,  oude pointer onthouden
	 * 2: split doen als modig.
	 * 3. alle NIET geselecteerde verts, edges, vlakken apart zetten
	 * 4. loadobeditdata(): dit is de nieuwe ob
	 * 5. freelist en oude verts, eds, vlakken weer terughalen
	 */
	
	/* alleen obedit geselecteerd */
	base= FIRSTBASE;
	while(base) {
		if(base->lay & G.vd->lay) {
			if(base->object==G.obedit) base->flag |= SELECT;
			else base->flag &= ~SELECT;
		}
		base= base->next;
	}
	
	/* testen of split */
	ok= 0;
	eed= G.eded.first;
	while(eed) {
		flag= (eed->v1->f & 1)+(eed->v2->f & 1);
		if(flag==1) {
			ok= 1;
			break;
		}
		eed= eed->next;
	}
	if(ok) {
		/* SPLIT: eerst duplicate maken */
		adduplicateflag(1);
		/* SPLIT: oude vlakken hebben 3x flag 128 gezet, deze deleten */
		delvlakflag(128);
	}
	
	/* apart zetten: alles wat maar enigszins NIET select is */
	edve.first= edve.last= eded.first= eded.last= edvl.first= edvl.last= 0;
	eve= G.edve.first;
	while(eve) {
		v1= eve->next;
		if((eve->f & 1)==0) {
			remlink(&G.edve, eve);
			addtail(&edve, eve);
		}
		eve= v1;
	}
	eed= G.eded.first;
	while(eed) {
		e1= eed->next;
		if( (eed->v1->f & 1)==0 || (eed->v2->f & 1)==0 ) {
			remlink(&G.eded, eed);
			addtail(&eded, eed);
		}
		eed= e1;
	}
	evl= G.edvl.first;
	while(evl) {
		vl1= evl->next;
		if( (evl->v1->f & 1)==0 || (evl->v2->f & 1)==0 || (evl->v3->f & 1)==0 ) {
			remlink(&G.edvl, evl);
			addtail(&edvl, evl);
		}
		evl= vl1;
	}
	
	oldob= G.obedit;
	oldbase= BASACT;
	
	trans[0]=trans[1]=trans[2]=trans[3]=trans[4]=trans[5]= 0.0;
	trans[6]=trans[7]=trans[8]= 1.0;
	G.qual |= LR_ALTKEY;	/* patch om zeker te zijn van gelinkte dupli */
	adduplicate(trans);
	G.qual &= ~LR_ALTKEY;
	
	G.obedit= BASACT->object;	/* basact wordt in adduplicate() gezet */

	men= copy_mesh(me);
	set_mesh(G.obedit, men);
	/* omdat nieuwe mesh een kopie is: aantal users verlagen */
	men->id.us--;
	
	load_editMesh();
	
	BASACT->flag &= ~SELECT;
	
	makeDispList(G.obedit);
	free_editmesh();
	
	G.edve= edve;
	G.eded= eded;
	G.edvl= edvl;
	
	/* hashedges are freed now, make new! */
	eed= G.eded.first;
	while(eed) {
		if( findedgelist(eed->v1, eed->v2)==NULL )
			insert_hashedge(eed);
		eed= eed->next;
	}
	
	G.obedit= oldob;
	BASACT= oldbase;
	BASACT->flag |= SELECT;
	
	waitcursor(0);

	countall();
	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);

}

void extrude_repeat_mesh(int steps, float offs)
{
	float dvec[3], tmat[4][4], bmat[3][3];
/* 	float phi; */
	short a,ok;

	TEST_EDITMESH
	waitcursor(1);

	/* dvec */
	dvec[0]= G.vd->persinv[2][0];
	dvec[1]= G.vd->persinv[2][1];
	dvec[2]= G.vd->persinv[2][2];
	Normalise(dvec);
	dvec[0]*= offs;
	dvec[1]*= offs;
	dvec[2]*= offs;

	/* base correctie */
	Mat3CpyMat4(bmat, G.obedit->obmat);
	/* phi= ((struct ObData *)G.obedit->d)->vv->ws; */
	/* Mat3MulFloat(bmat, phi); */
	Mat3Inv(tmat, bmat);
	Mat3MulVecfl(tmat, dvec);

	for(a=0;a<steps;a++) {
		ok= extrudeflag(1,1);
		if(ok==0) {
			error("Can't extrude");
			break;
		}
		translateflag(1, dvec);
	}

	countall();
	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);
	waitcursor(0);
}

void spin_mesh(int steps,int degr,float *dvec, int mode)
{
	EditVert *eve,*nextve;
	float *curs, si,n[3],q[4],cmat[3][3],imat[3][3];
	float cent[3],bmat[3][3];
	float phi;
	short a,ok;

	TEST_EDITMESH
	
	waitcursor(1);

	/* imat en centrum en afmeting */
	Mat3CpyMat4(bmat, G.obedit->obmat);
	Mat3Inv(imat,bmat);

	curs= give_cursor();
	VECCOPY(cent, curs);
	cent[0]-= G.obedit->obmat[3][0];
	cent[1]-= G.obedit->obmat[3][1];
	cent[2]-= G.obedit->obmat[3][2];
	Mat3MulVecfl(imat, cent);

	phi= degr*M_PI/360.0;
	phi/= steps;
	if(editbutflag & B_CLOCKWISE) phi= -phi;

	if(dvec) {
		n[0]=n[1]= 0.0;
		n[2]= 1.0;
	} else {
		n[0]= G.vd->viewinv[2][0];
		n[1]= G.vd->viewinv[2][1];
		n[2]= G.vd->viewinv[2][2];
		Normalise(n);
	}

	q[0]= fcos(phi);
	si= fsin(phi);
	q[1]= n[0]*si;
	q[2]= n[1]*si;
	q[3]= n[2]*si;
	QuatToMat3(q,cmat);

	Mat3MulMat3(G.obedit->obmat,cmat,bmat);
	Mat3MulMat3(bmat,imat,G.obedit->obmat);

	if(mode==0) if(editbutflag & B_KEEPORIG) adduplicateflag(1);
	ok= 1;

	for(a=0;a<steps;a++) {
		if(mode==0) ok= extrudeflag(1,1);
		else adduplicateflag(1);
		if(ok==0) {
			error("Can't spin");
			break;
		}
		rotateflag(1, cent, bmat[0]);
		if(dvec) {
			Mat3MulVecfl(bmat,dvec);
			translateflag(1,dvec);
		}
	}

	waitcursor(0);
	if(ok==0) {
		/* geen of alleen losse verts select, dups verwijderen */
		eve= G.edve.first;
		while(eve) {
			nextve= eve->next;
			if(eve->f & 1) {
				remlink(&G.edve,eve);
				free(eve);
			}
			eve= nextve;
		}
	}
	countall();
	recalc_editnormals();
	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);
}

void screw_mesh(int steps,int turns)
{
	EditVert *eve,*v1=0,*v2=0;
	EditEdge *eed;
	float dvec[3], nor[3];

	TEST_EDITMESH

	/* eerste voorwaarde: frontview! */
	if(G.vd->view!=1) {
		error("Only in frontview!");
		return;
	}

	/* flags wissen */
	eve= G.edve.first;
	while(eve) {
		eve->f1= 0;
		eve= eve->next;
	}
	/* edges zetten flags in verts */
	eed= G.eded.first;
	while(eed) {
		if(eed->v1->f & 1) {
			if(eed->v2->f & 1) {
				/* oppassen f1 is een byte */
				if(eed->v1->f1<2) eed->v1->f1++;
				if(eed->v2->f1<2) eed->v2->f1++;
			}
		}
		eed= eed->next;
	}
	/* vind twee vertices met eve->f1==1, meer of minder is fout */
	eve= G.edve.first;
	while(eve) {
		if(eve->f1==1) {
			if(v1==0) v1= eve;
			else if(v2==0) v2= eve;
			else {
				v1=0;
				break;
			}
		}
		eve= eve->next;
	}
	if(v1==0 || v2==0) {
		error("No curve selected");
		return;
	}

	/* bereken dvec */
	dvec[0]= ( (v1->co[0]- v2->co[0]) )/(steps);
	dvec[1]= ( (v1->co[1]- v2->co[1]) )/(steps);
	dvec[2]= ( (v1->co[2]- v2->co[2]) )/(steps);

	VECCOPY(nor, G.obedit->obmat[2]);

	if(nor[0]*dvec[0]+nor[1]*dvec[1]+nor[2]*dvec[2]>0.000) {
		dvec[0]= -dvec[0];
		dvec[1]= -dvec[1];
		dvec[2]= -dvec[2];
	}

	spin_mesh(turns*steps, turns*360, dvec, 0);

}

void selectswap_mesh()
{
	EditVert *eve;

	eve= G.edve.first;
	while(eve) {
		if(eve->h==0) {
			if(eve->f & 1) eve->f&= ~1;
			else eve->f|= 1;
		}
		eve= eve->next;
	}
	countall();
	allqueue(REDRAWVIEW3D, 0);

}

/* *******************************  ADD  ********************* */

void addvert_mesh()
{
	EditVert *eve,*v1=0;
	float *curs, mat[3][3],imat[3][3];

	TEST_EDITMESH

	Mat3CpyMat4(mat, G.obedit->obmat);
	Mat3Inv(imat, mat);

	v1= G.edve.first;
	while(v1) {
		if(v1->f & 1) break;
		v1= v1->next;
	}
	eve= v1;	/* voorkomen dat er nog meer select zijn */
	while(eve) {
		eve->f&= ~1;
		eve= eve->next;
	}

	eve= addvertlist(0);
	
	curs= give_cursor();
	VECCOPY(eve->co, curs);
	eve->xs= G.vd->mx;
	eve->ys= G.vd->my;
	VecSubf(eve->co, eve->co, G.obedit->obmat[3]);

	Mat3MulVecfl(imat, eve->co);
	eve->f= 1;

	if(v1) {
		addedgelist(v1, eve);
		v1->f= 0;
	}
	countall();
	allqueue(REDRAWVIEW3D, 0);

	while(get_mbut()&R_MOUSE);

}

void addedgevlak_mesh()
{
	EditVert *eve, *new[4];
	EditVlak *evl;
	EditEdge *eed;
	float con1, con2, con3;
	short aantal=0;

	if( (G.vd->lay & G.obedit->lay)==0 ) return;

	/* hoeveel geselecteerd ? */
	eve= G.edve.first;
	while(eve) {
		if(eve->f & 1) {
			aantal++;
			if(aantal>4) break;			
			new[aantal-1]= eve;
		}
		eve= eve->next;
	}
	if(aantal==2) {
		addedgelist(new[0], new[1]);
		allqueue(REDRAWVIEW3D, 0);
		return;
	}
	if(aantal<2 || aantal>4) {
		error("Can't make edge/face");
		return;
	}

	/* set direction of edges */
	
	eed= G.eded.first;
	while(eed) {
		eed->f= 0;
		eed= eed->next;
	}
	
	evl= G.edvl.first;
	while(evl) {
		evl->e1->f= 1;
		if(evl->e1->v1 == evl->v1) evl->e1->dir= 0;
		else evl->e1->dir= 1;
		
		evl->e2->f= 1;
		if(evl->e2->v1 == evl->v2) evl->e2->dir= 0;
		else evl->e2->dir= 1;

		evl->e3->f= 1;
		if(evl->e3->v1 == evl->v3) evl->e3->dir= 0;
		else evl->e3->dir= 1;

		if(evl->e4) {
			evl->e4->f= 1;
			if(evl->e4->v1 == evl->v4) evl->e4->dir= 0;
			else evl->e4->dir= 1;
		}

		evl= evl->next;
	}	


	if(aantal==3) {
		if(exist_vlak(new[0], new[1], new[2], 0)==0) {
			
			evl= addvlaklist(new[0], new[1], new[2], 0, G.obedit->actcol-1, 0, 0);

			if(evl->e1->f) {
				if(evl->e1->dir==1 && evl->v1==evl->e1->v1 && evl->v2==evl->e1->v2);
				else flipvlak(evl);
			}
			else if(evl->e2->f) {
				if(evl->e2->dir==1 && evl->v2==evl->e2->v1 && evl->v3==evl->e2->v2);
				else flipvlak(evl);
			}
			else if(evl->e3->f) {
				if(evl->e3->dir==1 && evl->v3==evl->e3->v1 && evl->v1==evl->e3->v2);
				else flipvlak(evl);
			}

		}
		else error("Already a face");
	}
	else if(aantal==4) {
		if(exist_vlak(new[0], new[1], new[2], new[3])==0) {
		
			
			con1= convex(new[0]->co, new[1]->co, new[2]->co, new[3]->co);
			con2= convex(new[0]->co, new[2]->co, new[3]->co, new[1]->co);
			con3= convex(new[0]->co, new[3]->co, new[1]->co, new[2]->co);

			if(con1>=con2 && con1>=con3)
				evl= addvlaklist(new[0], new[1], new[2], new[3], G.obedit->actcol-1, 0, 0);
			else if(con2>=con1 && con2>=con3)
				evl= addvlaklist(new[0], new[2], new[3], new[1], G.obedit->actcol-1, 0, 0);
			else 
				evl= addvlaklist(new[0], new[2], new[1], new[3], G.obedit->actcol-1, 0, 0);


			if(evl->e1->f) {
				if(evl->e1->dir==1 && evl->v1==evl->e1->v1 && evl->v2==evl->e1->v2);
				else flipvlak(evl);
			}
			else if(evl->e2->f) {
				if(evl->e2->dir==1 && evl->v2==evl->e2->v1 && evl->v3==evl->e2->v2);
				else flipvlak(evl);
			}
			else if(evl->e3->f) {
				if(evl->e3->dir==1 && evl->v3==evl->e3->v1 && evl->v4==evl->e3->v2);
				else flipvlak(evl);
			}
			else if(evl->e4->f) {
				if(evl->e4->dir==1 && evl->v4==evl->e4->v1 && evl->v1==evl->e4->v2);
				else flipvlak(evl);
			}
			
		}
		else error("Already a face");
	}
	
	countall();
	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);
}

void delete_mesh()
{
	EditVert *eve,*nextve;
	EditVlak *evl,*nextvl;
	EditEdge *eed,*nexted;
	short event;

	TEST_EDITMESH
	
	event= pupmenu("ERASE %t|Vertices%x10|Edges%x1|Faces%x2|All%x3|Edges & Faces%x4|Only Faces%x5");
	if(event<1) return;

	if(event==10 || event==4) {
		eed= G.eded.first;
		while(eed) {
			nexted= eed->next;
			if( (eed->v1->f & 1) || (eed->v2->f & 1) ) {
				remedge(eed);
				free(eed);
			}
			eed= nexted;
		}
		evl= G.edvl.first;
		while(evl) {
			nextvl= evl->next;
			if( vlakselectedOR(evl, 1) ) {
				remlink(&G.edvl, evl);
				freevlak(evl);
			}
			evl= nextvl;
		}
		if(event==10) {
			eve= G.edve.first;
			while(eve) {
				nextve= eve->next;
				if(eve->f & 1) {
					remlink(&G.edve, eve);
					free(eve);
				}
				eve= nextve;
			}
		}
	} else if(event==1) {
		eed= G.eded.first;
		while(eed) {
			nexted= eed->next;
			if( (eed->v1->f & 1) && (eed->v2->f & 1) ) {
				remedge(eed);
				free(eed);
			}
			eed= nexted;
		}
		evl= G.edvl.first;
		while(evl) {
			nextvl= evl->next;
			event=0;
			if( evl->v1->f & 1) event++;
			if( evl->v2->f & 1) event++;
			if( evl->v3->f & 1) event++;
			if(evl->v4 && (evl->v4->f & 1)) event++;
			
			if(event>1) {
				remlink(&G.edvl, evl);
				freevlak(evl);
			}
			evl= nextvl;
		}
		/* om losse vertices te wissen: */
		eed= G.eded.first;
		while(eed) {
			if( eed->v1->f & 1) eed->v1->f-=1;
			if( eed->v2->f & 1) eed->v2->f-=1;
			eed= eed->next;
		}
		eve= G.edve.first;
		while(eve) {
			nextve= eve->next;
			if(eve->f & 1) {
				remlink(&G.edve,eve);
				free(eve);
			}
			eve= nextve;
		}

	}
	else if(event==2) delvlakflag(1);
	else if(event==3) {
		if(G.edve.first) freelist(&G.edve);
		if(G.eded.first) freelist(&G.eded);
		if(G.edvl.first) freevlaklist(&G.edvl);
	}
	else if(event==5) {
		evl= G.edvl.first;
		while(evl) {
			nextvl= evl->next;
			if(vlakselectedAND(evl, 1)) {
				remlink(&G.edvl, evl);
				freevlak(evl);
			}
			evl= nextvl;
		}
	}

	countall();
	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);
}



void add_primitiveMesh(int type)
{
	Mesh *me;
	EditVert *eve, *v1=NULL, *v2, *v3, *v4=NULL, *vtop, *vdown;
	float *curs, d, dia, phi, phid, cent[3], vec[3], imat[3][3], mat[3][3], tmat[4][4];
	float q[4], cmat[3][3];
	static short tot=32, seg=32, subdiv=2;
	short a, b, ext=0, fill=0, totoud, newob=0;
	
	if(G.scene->id.lib) return;

	/* this function also comes from an info window */
	if ELEM(curarea->spacetype, SPACE_VIEW3D, SPACE_INFO); else return;
	if(G.vd==0) return;

	check_editmode(OB_MESH);
	
	/* als geen obedit: nieuw object en in editmode gaan */
	if(G.obedit==0) {
		add_object(OB_MESH);
		G.obedit= BASACT->object;
		
		where_is_object(G.obedit);
		
		make_editMesh();
		setcursor_space(SPACE_VIEW3D, CURSOR_EDIT);
		newob= 1;
	}
	me= G.obedit->data;
	
	/* deselectall */
	eve= G.edve.first;
	while(eve) {
		if(eve->f & 1) eve->f&= ~1;
		eve= eve->next;
	}

	totoud= tot; /* onthouden en terugzetten als cube/plane */
	
	/* imat en centrum en afmeting */
	Mat3CpyMat4(mat, G.obedit->obmat);

	curs= give_cursor();
	VECCOPY(cent, curs);
	cent[0]-= G.obedit->obmat[3][0];
	cent[1]-= G.obedit->obmat[3][1];
	cent[2]-= G.obedit->obmat[3][2];

	if(type!= 11) {
		Mat3CpyMat4(imat, G.vd->viewmat);
		Mat3MulVecfl(imat, cent);
		Mat3MulMat3(cmat, imat, mat);
		Mat3Inv(imat,cmat);
	} else {
		Mat3Inv(imat, mat);
	}
	
	/* ext==extrudeflag, tot==aantal verts in basis */

	switch(type) {
	case 0:		/* plane */
		tot= 4;
		ext= 0;
		fill= 1;
		if(newob) rename_id((ID *)G.obedit, "Plane");
		if(newob) rename_id((ID *)me, "Plane");
		break;
	case 1:		/* cube  */
		tot= 4;
		ext= 1;
		fill= 1;
		if(newob) rename_id((ID *)G.obedit, "Cube");
		if(newob) rename_id((ID *)me, "Cube");
		break;
	case 4:		/* circle  */
		if(button(&tot,3,100,"Vertices:")==0) return;
		ext= 0;
		fill= 0;
		if(newob) rename_id((ID *)G.obedit, "Circle");
		if(newob) rename_id((ID *)me, "Circle");
		break;
	case 5:		/* cylinder  */
		if(button(&tot,3,100,"Vertices:")==0) return;
		ext= 1;
		fill= 1;
		if(newob) rename_id((ID *)G.obedit, "Cylinder");
		if(newob) rename_id((ID *)me, "Cylinder");
		break;
	case 6:		/* tube  */
		if(button(&tot,3,100,"Vertices:")==0) return;
		ext= 1;
		fill= 0;
		if(newob) rename_id((ID *)G.obedit, "Tube");
		if(newob) rename_id((ID *)me, "Tube");
		break;
	case 7:		/* cone  */
		if(button(&tot,3,100,"Vertices:")==0) return;
		ext= 0;
		fill= 1;
		if(newob) rename_id((ID *)G.obedit, "Cone");
		if(newob) rename_id((ID *)me, "Cone");
		break;
	case 10:	/* grid */
		if(button(&tot,2,100,"X res:")==0) return;
		if(button(&seg,2,100,"Y res:")==0) return;
		if(newob) rename_id((ID *)G.obedit, "Grid");
		if(newob) rename_id((ID *)me, "Grid");
		break;
	case 11:	/* UVsphere */
		if(button(&seg,3,100,"Segments:")==0) return;
		if(button(&tot,3,100,"Rings:")==0) return;
		if(newob) rename_id((ID *)G.obedit, "Sphere");
		if(newob) rename_id((ID *)me, "Sphere");
		break;
	case 12:	/* Icosphere */
		if(button(&subdiv,1,5,"Subdivision:")==0) return;
		if(newob) rename_id((ID *)G.obedit, "Sphere");
		if(newob) rename_id((ID *)me, "Sphere");
		break;
	}

	dia= fsqrt(2.0)*G.vd->grid;
	d= -G.vd->grid;
	phid= 2*M_PI/tot;
	phi= .25*M_PI;


	if(type<10) {	/* alles behalve grid of sphere */
		if(ext==0 && type!=7) d= 0;
	
		/* de vertices */
		vtop= vdown= v1= v2= 0;
		for(b=0; b<=ext; b++) {
			for(a=0; a<tot; a++) {
				
				vec[0]= cent[0]+dia*fsin(phi);
				vec[1]= cent[1]+dia*fcos(phi);
				vec[2]= cent[2]+d;
				
				Mat3MulVecfl(imat, vec);
				eve= addvertlist(vec);
				eve->f= 1;
				if(a==0) {
					if(b==0) v1= eve;
					else v2= eve;
				}
				phi+=phid;
			}
			d= -d;
		}
		/* centrum vertices */
		if(fill && type>1) {
			VECCOPY(vec,cent);
			vec[2]-= -d;
			Mat3MulVecfl(imat,vec);
			vdown= addvertlist(vec);
			if(ext || type==7) {
				VECCOPY(vec,cent);
				vec[2]-= d;
				Mat3MulVecfl(imat,vec);
				vtop= addvertlist(vec);
			}
		} else {
			vdown= v1;
			vtop= v2;
		}
		if(vtop) vtop->f= 1;
		if(vdown) vdown->f= 1;
	
		/* boven en ondervlak */
		if(fill) {
			if(tot==4 && (type==0 || type==1)) {
				v3= v1->next->next;
				if(ext) v4= v2->next->next;
				
				addvlaklist(v3, v1->next, v1, v3->next, G.obedit->actcol-1, 0, 0);
				if(ext) addvlaklist(v2, v2->next, v4, v4->next, G.obedit->actcol-1, 0, 0);
				
			}
			else {
				v3= v1;
				v4= v2;
				for(a=1; a<tot; a++) {
					addvlaklist(vdown, v3, v3->next, 0, G.obedit->actcol-1, 0, 0);
					v3= v3->next;
					if(ext) {
						addvlaklist(vtop, v4, v4->next, 0, G.obedit->actcol-1, 0, 0);
						v4= v4->next;
					}
				}
				if(type>1) {
					addvlaklist(vdown, v3, v1, 0, G.obedit->actcol-1, 0, 0);
					if(ext) addvlaklist(vtop, v4, v2, 0, G.obedit->actcol-1, 0, 0);
				}
			}
		}
		else if(type==4) {  /* wel edges bij circle */
			v3= v1;
			for(a=1;a<tot;a++) {
				addedgelist(v3,v3->next);
				v3= v3->next;
			}
			addedgelist(v3,v1);
		}
		/* zijvlakken */
		if(ext) {
			v3= v1;
			v4= v2;
			for(a=1; a<tot; a++) {
				addvlaklist(v3, v3->next, v4->next, v4, G.obedit->actcol-1, 0, 0);
				v3= v3->next;
				v4= v4->next;
			}
			addvlaklist(v3, v1, v2, v4, G.obedit->actcol-1, 0, 0);
		}
		else if(type==7) { /* cone */
			v3= v1;
			for(a=1; a<tot; a++) {
				addvlaklist(vtop, v3->next, v3, 0, G.obedit->actcol-1, 0, 0);
				v3= v3->next;
			}
			addvlaklist(vtop, v1, v3, 0, G.obedit->actcol-1, 0, 0);
		}
		
		if(type<2) tot= totoud;
		
	}
	else if(type==10) {	/*  grid */
		/* alle flags wissen */
		eve= G.edve.first;
		while(eve) {
			eve->f= 0;
			eve= eve->next;
		}
		dia= G.vd->grid;
		/* eerst een segment: de X as */
		phi= -1.0; 
		phid= 2.0/((float)tot-1);
		for(a=0;a<tot;a++) {
			vec[0]= cent[0]+dia*phi;
			vec[1]= cent[1]- dia;
			vec[2]= cent[2];
			Mat3MulVecfl(imat,vec);
			eve= addvertlist(vec);
			eve->f= 1+2+4;
			if (a) addedgelist(eve->prev,eve);
			phi+=phid;
		}
		/* extruden en transleren */
		vec[0]= vec[2]= 0.0;
		vec[1]= dia*phid;
		Mat3MulVecfl(imat, vec);
		for(a=0;a<seg-1;a++) {
			extrudeflag(2,0);
			translateflag(2, vec);
		}
	}
	else if(type==11) {	/*  UVsphere */
		/* alle flags wissen */
		eve= G.edve.first;
		while(eve) {
			eve->f= 0;
			eve= eve->next;
		}
		
		/* eerst een segment */
		phi= 0; 
		phid/=2;
		for(a=0; a<=tot; a++) {
			vec[0]= cent[0]+dia*fsin(phi);
			vec[1]= cent[1];
			vec[2]= cent[2]+dia*fcos(phi);
			Mat3MulVecfl(imat,vec);
			eve= addvertlist(vec);
			eve->f= 1+2+4;
			if(a==0) v1= eve;
			else addedgelist(eve->prev, eve);
			phi+= phid;
		}
		
		/* extruden en roteren */
		phi= M_PI/seg;
		q[0]= fcos(phi);
		q[3]= fsin(phi);
		q[1]=q[2]= 0;
		QuatToMat3(q, cmat);
		Mat3MulMat3(tmat, cmat, mat);
		Mat3MulMat3(cmat, imat, tmat);
		
		for(a=0; a<seg; a++) {
			extrudeflag(2, 0);
			rotateflag(2, v1->co, cmat[0]);
		}
		removedoublesflag(4, 0.01);
	}
	else if(type==12) {	/* Icosphere */
		EditVert *eva[12];

		/* alle flags wissen */
		eve= G.edve.first;
		while(eve) {
			eve->f= 0;
			eve= eve->next;
		}
		dia/=200;
		for(a=0;a<12;a++) {
			vec[0]= dia*icovert[a][0];
			vec[1]= dia*icovert[a][1];
			vec[2]= dia*icovert[a][2];
			eva[a]= addvertlist(vec);
			eva[a]->f= 1+2;
		}
		for(a=0;a<20;a++) {
			v1= eva[ icovlak[a][0] ];
			v2= eva[ icovlak[a][1] ];
			v3= eva[ icovlak[a][2] ];
			addvlaklist(v1, v2, v3, 0, G.obedit->actcol-1, 0, 0);
		}

		dia*=200;
		for(a=1; a<subdiv; a++) subdivideflag(2, dia, 0);
		/* nu pas met imat */
		eve= G.edve.first;
		while(eve) {
			if(eve->f & 2) {
				VecAddf(eve->co,eve->co,cent);
				Mat3MulVecfl(imat,eve->co);
			}
			eve= eve->next;
		}
	}
	
	if(type!=0 && type!=10) righthandfaces(1);
	countall();
	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);

}

void vertexsmooth()
{
	struct EditVert *eve;
	struct EditEdge *eed;
	float *adror, *adr, fac;
	float fvec[3];
	int teller=0;

	if(G.obedit==0) return;

	/* aantal tellen */
	eve= G.edve.first;
	while(eve) {
		if(eve->f & 1) teller++;
		eve= eve->next;
	}
	if(teller==0) return;

	adr=adror= (float *)callocN(3*sizeof(float *)*teller, "vertsmooth");
	eve= G.edve.first;
	while(eve) {
		if(eve->f & 1) {
			eve->vn= (EditVert *)adr;
			eve->f1= 0;
			adr+= 3;
		}
		eve= eve->next;
	}
	
	eed= G.eded.first;
	while(eed) {
		if( (eed->v1->f & 1) || (eed->v2->f & 1) ) {
			fvec[0]= (eed->v1->co[0]+eed->v2->co[0])/2.0;
			fvec[1]= (eed->v1->co[1]+eed->v2->co[1])/2.0;
			fvec[2]= (eed->v1->co[2]+eed->v2->co[2])/2.0;
			
			if((eed->v1->f & 1) && eed->v1->f1<255) {
				eed->v1->f1++;
				VecAddf((float *)eed->v1->vn, (float *)eed->v1->vn, fvec);
			}
			if((eed->v2->f & 1) && eed->v2->f1<255) {
				eed->v2->f1++;
				VecAddf((float *)eed->v2->vn, (float *)eed->v2->vn, fvec);
			}
		}
		eed= eed->next;
	}

	eve= G.edve.first;
	while(eve) {
		if(eve->f & 1) {
			if(eve->f1) {
				adr= (float *)eve->vn;
				fac= 0.5/(float)eve->f1;
				
				eve->co[0]= 0.5*eve->co[0]+fac*adr[0];
				eve->co[1]= 0.5*eve->co[1]+fac*adr[1];
				eve->co[2]= 0.5*eve->co[2]+fac*adr[2];
			}
			eve->vn= 0;
		}
		eve= eve->next;
	}
	freeN(adror);

	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);
}


void vertexnoise()
{
	extern float Tin;
	Material *ma;
	Tex *tex;
	EditVert *eve;
	float b2, ofs, vec[3];

	if(G.obedit==0) return;

	ma= give_current_material(G.obedit, G.obedit->actcol);
	if(ma==0 || ma->mtex[0]==0 || ma->mtex[0]->tex==0) {
		return;
	}
	tex= ma->mtex[0]->tex;
	
	ofs= tex->turbul/200.0;
	
	eve= (struct EditVert *)G.edve.first;
	while(eve) {
		if(eve->f & 1) {
			
			if(tex->type==TEX_STUCCI) {
				
				b2= hnoise(tex->noisesize, eve->co[0], eve->co[1], eve->co[2]);
				if(tex->stype) ofs*=(b2*b2);
				vec[0]= 0.2*(b2-hnoise(tex->noisesize, eve->co[0]+ofs, eve->co[1], eve->co[2]));
				vec[1]= 0.2*(b2-hnoise(tex->noisesize, eve->co[0], eve->co[1]+ofs, eve->co[2]));
				vec[2]= 0.2*(b2-hnoise(tex->noisesize, eve->co[0], eve->co[1], eve->co[2]+ofs));
				
				VecAddf(eve->co, eve->co, vec);
			}
			else {
				
				externtex(ma->mtex[0], eve->co);
			
				eve->co[2]+= 0.05*Tin;
			}
		}
		eve= eve->next;
	}

	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);
}

void hide_mesh(int swap)
{
	struct EditVert *eve;
	struct EditEdge *eed;

	if(G.obedit==0) return;

	if(swap) {
		eve= G.edve.first;
		while(eve) {
			if((eve->f & 1)==0) {
				eve->xs= 3200;
				eve->h= 1;
			}
			eve= eve->next;
		}
	}
	else {
		eve= G.edve.first;
		while(eve) {
			if(eve->f & 1) {
				eve->f-=1;
				eve->xs= 3200;
				eve->h= 1;
			}
			eve= eve->next;
		}
	}
	eed= G.eded.first;
	while(eed) {
		if(eed->v1->h || eed->v2->h) eed->h= 1;
		else eed->h= 0;
		eed= eed->next;
	}

	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);
}


void reveal_mesh(void)
{
	struct EditVert *eve;
	struct EditEdge *eed;

	if(G.obedit==0) return;

	eve= G.edve.first;
	while(eve) {
		if(eve->h) {
			eve->h= 0;
			eve->f|=1;
		}
		eve= eve->next;
	}

	eed= G.eded.first;
	while(eed) {
		eed->h= 0;
		eed= eed->next;
	}

	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);
}

float convex(float *v1, float *v2, float *v3, float *v4)
{
	float cross[3], test[3];
	float inpr;
	
	CalcNormFloat(v1, v2, v3, cross);
	CalcNormFloat(v1, v3, v4, test);

	inpr= cross[0]*test[0]+cross[1]*test[1]+cross[2]*test[2];

	return inpr;
}

void givequadverts(EditVlak *evl, EditVlak *evl1, EditVert **v1, EditVert **v2, EditVert **v3, EditVert **v4)
{
				
	if(evl->v1!=evl1->v1 && evl->v1!=evl1->v2 && evl->v1!=evl1->v3) {
		*v1= evl->v1;
		*v2= evl->v2;
	}
	else if(evl->v2!=evl1->v1 && evl->v2!=evl1->v2 && evl->v2!=evl1->v3) {
		*v1= evl->v2;
		*v2= evl->v3;
	}
	else if(evl->v3!=evl1->v1 && evl->v3!=evl1->v2 && evl->v3!=evl1->v3) {
		*v1= evl->v3;
		*v2= evl->v1;
	}
	
	if(evl1->v1!=evl->v1 && evl1->v1!=evl->v2 && evl1->v1!=evl->v3) {
		*v3= evl1->v1;
		if(evl1->v2== *v2) *v4= evl1->v3;
		else *v4= evl1->v2;
	}
	else if(evl1->v2!=evl->v1 && evl1->v2!=evl->v2 && evl1->v2!=evl->v3) {
		*v3= evl1->v2;
		if(evl1->v3== *v2) *v4= evl1->v1;
		else *v4= evl1->v3;
	}
	else if(evl1->v3!=evl->v1 && evl1->v3!=evl->v2 && evl1->v3!=evl->v3) {
		*v3= evl1->v3;
		if(evl1->v1== *v2) *v4= evl1->v2;
		else *v4= evl1->v1;
	}
}

/* ook weer twee zeer vreemde 'patch' functies om de uv van tfaces te bewaren */

short *set_correct_uv(EditVert *eve,  EditVlak **evla)
{
	
	if(eve== evla[1]->v3) return evla[1]->tface->uv[2];
	if(eve== evla[0]->v3) return evla[0]->tface->uv[2];
	if(eve== evla[1]->v2) return evla[1]->tface->uv[1];
	if(eve== evla[0]->v2) return evla[0]->tface->uv[1];
	if(eve== evla[1]->v1) return evla[1]->tface->uv[0];
	if(eve== evla[0]->v1) return evla[0]->tface->uv[0];
	return 0;
}

void restore_wuv(EditVlak *evl, void **evla)
{
	int *lp;
	
	lp= (int *)set_correct_uv(evl->v1, (EditVlak **)evla);
	((int *)(evl->tface->uv[0]))[0]= lp[0];
	((int *)(evl->tface->uv[0]))[4]= lp[4];
	
	lp= (int *)set_correct_uv(evl->v2, (EditVlak **)evla);
	((int *)(evl->tface->uv[1]))[0]= lp[0];
	((int *)(evl->tface->uv[1]))[4]= lp[4];

	lp= (int *)set_correct_uv(evl->v3, (EditVlak **)evla);
	((int *)(evl->tface->uv[2]))[0]= lp[0];
	((int *)(evl->tface->uv[2]))[4]= lp[4];
	
}


void beauty_fill(void)
{
	EditVert *v1, *v2, *v3, *v4;
	EditEdge *eed, *e1, *e2, *e3, *nexted;
	EditEdge dia1, dia2;
	EditVlak *evl, *nextvl, *w;
	void **evlar, **evla;
	float len1, len2, len3, len4, len5, len6, opp1, opp2, fac1, fac2;
	int totedge, ok, notbeauty=8, onedone;
	
	/* - alle geselecteerde edges met 2 vlakken
	 * - vind die vlakken: opslaan in edges (extra datablok)
	 * - per edge: - test convex
	 *			   - test edge: flip?
						- zoja: remedge,  addedge, alle randedges nieuwe vlakpointers
	 */

	totedge= 0;
	eed= G.eded.first;
	while(eed) {
		eed->vn= 0;
		if( (eed->v1->f & 1) && (eed->v2->f & 1) ) totedge++;
		eed= eed->next;
	}

	if(totedge==0) return;

	if(okee("Beauty Fill")==0) return;

	/* tempblok met vlakpointers */
	evlar= callocN(2*sizeof(void *)*totedge, "beautyfill");

	while( notbeauty ) {
		notbeauty--;
		
		evla= evlar;
		eed= G.eded.first;
		while(eed) {
			eed->f= 0;
			eed->f1= 0;
			if( (eed->v1->f & 1) && (eed->v2->f & 1) ) {
				eed->vn= (EditVert *)evla;
				evla+= 2;
			}
			eed= eed->next;
		}
		
	
		/* alle edges met twee vlakken zoeken */
		evl= G.edvl.first;
		while(evl) {
			evl->f1= 0;
			if(evl->v4==0) {
				if(vlakselectedAND(evl, 1)) {
					
					e1= evl->e1;
					e2= evl->e2;
					e3= evl->e3;
					if(e1->f<3) {
						if(e1->f<2) {
							evla= (void **)e1->vn;
							evla[(int)e1->f]= evl;
						}
						e1->f+= 1;
					}
					if(e2->f<3) {
						if(e2->f<2) {
							evla= (void **)e2->vn;
							evla[(int)e2->f]= evl;
						}
						e2->f+= 1;
					}
					if(e3->f<3) {
						if(e3->f<2) {
							evla= (void **)e3->vn;
							evla[(int)e3->f]= evl;
						}
						e3->f+= 1;
					}
				}
			}
			evl= evl->next;
		}
		
		/* gaatie */
		onedone= 0;
		
		eed= G.eded.first;
		while(eed) {
			nexted= eed->next;
			
			if(eed->f==2) {
				
				
				evla= (void **)eed->vn;
				
				/* geen van de vlakken mag al gedaan zijn */
				ok= 1;
				evl= evla[0];
				if(evl->e1->f1 || evl->e2->f1 || evl->e3->f1) ok= 0;
				evl= evla[1];
				if(evl->e1->f1 || evl->e2->f1 || evl->e3->f1) ok= 0;
				
				if(ok) {
					/* test convex */
					givequadverts(evla[0], evla[1], &v1, &v2, &v3, &v4);
					if( convex(v1->co, v2->co, v3->co, v4->co) > -0.5) {
						
						/* test edges */
						if( ((long)v1) > ((long)v3) ) {
							dia1.v1= v3;
							dia1.v2= v1;
						}
						else {
							dia1.v1= v1;
							dia1.v2= v3;
						}
						
						if( ((long)v2) > ((long)v4) ) {
							dia2.v1= v4;
							dia2.v2= v2;
						}
						else {
							dia2.v1= v2;
							dia2.v2= v4;
						}
						
						/* testregel: 
						 * de oppervlakte gedeeld door de totale edgelengte
						 *
						 */
						
						len1= VecLenf(v1->co, v2->co);
						len2= VecLenf(v2->co, v3->co);
						len3= VecLenf(v3->co, v4->co);
						len4= VecLenf(v4->co, v1->co);
						len5= VecLenf(v1->co, v3->co);
						len6= VecLenf(v2->co, v4->co);
						
						opp1= AreaT3Dfl(v1->co, v2->co, v3->co);
						opp2= AreaT3Dfl(v1->co, v3->co, v4->co);
						
						fac1= opp1/(len1+len2+len5) + opp2/(len3+len4+len5);
						
						opp1= AreaT3Dfl(v2->co, v3->co, v4->co);
						opp2= AreaT3Dfl(v2->co, v4->co, v1->co);
						
						fac2= opp1/(len2+len3+len6) + opp2/(len4+len1+len6);
						
						ok= 0;
						if(fac1 > fac2) {
							if(dia2.v1==eed->v1 && dia2.v2==eed->v2) {
								eed->f1= 1;
								evl= evla[0];
								evl->f1= 1;
								evl= evla[1];
								evl->f1= 1;
								
								w= addvlaklist(v1, v2, v3, 0, evl->mat_nr, evl->flag, evl->tface);
								if(w->tface) restore_wuv(w, evla);
								w= addvlaklist(v1, v3, v4, 0, evl->mat_nr, evl->flag, evl->tface);
								if(w->tface) restore_wuv(w, evla);
								
								onedone= 1;
							}
						}
						else if(fac1 < fac2) {
							if(dia1.v1==eed->v1 && dia1.v2==eed->v2) {
								eed->f1= 1;
								evl= evla[0];
								evl->f1= 1;
								evl= evla[1];
								evl->f1= 1;
								
								w= addvlaklist(v2, v3, v4, 0, evl->mat_nr, evl->flag, evl->tface);
								if(w->tface) restore_wuv(w, evla);
								w= addvlaklist(v1, v2, v4, 0, evl->mat_nr, evl->flag, evl->tface);
								if(w->tface) restore_wuv(w, evla);
								
								onedone= 1;
							}
						}
						
					}
				}
				
			}
			eed= nexted;
		}
		
		/* edges weggooien */
		eed= G.eded.first;
		while(eed) {
			nexted= eed->next;
			if(eed->f1) {
				remedge(eed);
				free(eed);
			}
			eed= nexted;
		}
		
		/* vlakken weggooien */
		evl= G.edvl.first;
		while(evl) {
			nextvl= evl->next;
			if(evl->f1) {
				remlink(&G.edvl, evl);
				freevlak(evl);
			}
			evl= nextvl;
		}
		
		if(onedone==0) break;
	}
	
	freeN(evlar);
	
	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);
}

void join_mesh()
{
	Base *base, *nextb;
	Object *ob;
	Material **matar, *ma;
	Mesh *me;
	MVert *mvert, *mvertmain;
	MFace *mface, *mfacemain;
	uint *mcol=NULL, *mcolmain;
	float imat[4][4], cmat[4][4];
	int a, b, totcol, totvert=0, totface=0, ok=0, vertofs, map[MAXMAT];
	
	
	if(G.obedit) return;
	
	ob= OBACT;
	if(ob->type!=OB_MESH) return;
	
	/* tellen */
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) {
			if(base->object->type==OB_MESH) {
				me= base->object->data;
				totvert+= me->totvert;
				totface+= me->totface;
				
				if(me->tface) {
					error("not allowed with texture faces!");
					return;
				}
				
				if(base->object == ob) ok= 1;
			}
		}
		base= base->next;
	}
	
	/* zodoende is het aktieve object altijd select */ 
	if(ok==0) return;
	
	if(totvert==0 || totvert>64000) return;
	
	if(okee("Join selected Meshes")==0) return;
	
	/* nieuwe materiaal indexen en hoofdarray */
	matar= callocN(sizeof(void *)*MAXMAT, "join_mesh");
	totcol= ob->totcol;
	
	/* obact materials in nieuw hoofdarray, is mooiere start! */
	for(a=1; a<=ob->totcol; a++) {
		matar[a-1]= give_current_material(ob, a);
		id_us_plus((ID *)matar[a-1]);
		/* id->us ophogen: wordt ook verlaagd */
	}
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) {
			if(ob!=base->object && base->object->type==OB_MESH) {
				me= base->object->data;
				if(me->totvert) {
					for(a=1; a<=base->object->totcol; a++) {
						ma= give_current_material(base->object, a);
						if(ma) {
							for(b=0; b<totcol; b++) {
								if(ma == matar[b]) break;
							}
							if(b==totcol) {
								matar[b]= ma;
								ma->id.us++;
								totcol++;
							}
							if(totcol>=MAXMAT-1) break;
						}
					}
				}
			}
			if(totcol>=MAXMAT-1) break;
		}
		base= base->next;
	}

	me= ob->data;
	mvert= mvertmain= mallocN(totvert*sizeof(MVert), "joinmesh1");
	if (totface) mface= mfacemain= mallocN(totface*sizeof(MFace), "joinmesh2");
	if(me->mcol) mcol= mcolmain= callocN(totface*4*sizeof(int), "joinmesh3");
	else mcolmain= 0;

	vertofs= 0;
	
	/* alle geselecteerde meshes invers transformen in obact */
	Mat4Invert(imat, ob->obmat);
	
	base= FIRSTBASE;
	while(base) {
		nextb= base->next;
		if TESTBASE(base) {
			if(base->object->type==OB_MESH) {
				
				me= base->object->data;
				
				if(me->totvert) {
					
					memcpy(mvert, me->mvert, me->totvert*sizeof(MVert));
					
					if(base->object != ob) {
						/* let op: matmul omkeren is ECHT fout */
						Mat4MulMat4(cmat, base->object->obmat, imat);
						
						a= me->totvert;
						while(a--) {
							Mat4MulVecfl(cmat, mvert->co);
							mvert++;
						}
					}
					else mvert+= me->totvert;
					
					if(mcolmain) {
						if(me->mcol) memcpy(mcol, me->mcol, me->totface*4*4);
						mcol+= 4*me->totface;
					}
				}
				if(me->totface) {
				
					/* mapping maken voor materialen */
					bzero(map, 4*MAXMAT);
					for(a=1; a<=base->object->totcol; a++) {
						ma= give_current_material(base->object, a);
						if(ma) {
							for(b=0; b<totcol; b++) {
								if(ma == matar[b]) {
									map[a-1]= b;
									break;
								}
							}
						}
					}

					memcpy(mface, me->mface, me->totface*sizeof(MFace));
					
					a= me->totface;
					while(a--) {
						mface->v1+= vertofs;
						mface->v2+= vertofs;
						if(mface->v3) mface->v3+= vertofs;
						if(mface->v4) mface->v4+= vertofs;
						
						mface->mat_nr= map[(int)mface->mat_nr];
						
						mface++;
					}
				}
				vertofs+= me->totvert;
				
				if(base->object!=ob) {
					remlink(&(G.scene->base), base);
					free_libblock_us(&(G.main->object), base->object);
					freeN(base);
				}
			}
		}
		base= nextb;
	}
	
	me= ob->data;
	
	if(me->mface) freeN(me->mface);
	me->mface= mfacemain;
	if(me->mvert) freeN(me->mvert);
	me->mvert= mvertmain;
	if(me->mcol) freeN(me->mcol);
	me->mcol= (MCol *)mcolmain;
		
	me->totvert= totvert;
	me->totface= totface;
	
	/* oude material array */
	for(a=1; a<=ob->totcol; a++) {
		ma= ob->mat[a-1];
		if(ma) ma->id.us--;
	}
	for(a=1; a<=me->totcol; a++) {
		ma= me->mat[a-1];
		if(ma) ma->id.us--;
	}
	if(ob->mat) freeN(ob->mat);
	if(me->mat) freeN(me->mat);
	ob->mat= me->mat= 0;
	
	if(totcol) {
		me->mat= matar;
		ob->mat= callocN(sizeof(void *)*totcol, "join obmatar");
	}
	else freeN(matar);
	
	ob->totcol= me->totcol= totcol;
	ob->colbits= 0;
	
	/* andere mesh gebruikers */
	test_object_materials((ID *)me);
	
	enter_editmode();
	exit_editmode(1);
	
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWBUTSMAT, 0);
	makeDispList(G.obedit);

}

void join_sector()
{
	Base *base, *nextb;
	Object *ob;
	Sector *se;
	Material **matar, *ma;
	Mesh *me;
	MVert *mvert, *mvertmain;
	MFace *mface, *mfacemain;
	TFace *tface, *tfacemain;
	float imat[4][4], cmat[4][4];
	int a, b, totcol, totvert=0, totface=0, ok=0, vertofs, map[MAXMAT];
	
	
	if(G.obedit) return;
	
	ob= OBACT;
	if(ob->type!=OB_SECTOR) return;
	
	/* deze onhandig lange functie zou wel 's verbouwd mogen worden... */
	
	/* ************************* */
	/* 1: DYNAMESHes samenvoegen */
	/* ************************* */
	
	/* tellen */
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) {
			if(base->object->type==OB_SECTOR) {
				se= base->object->data;
				me= se->dynamesh;
				if(me) {
					totvert+= me->totvert;
					totface+= me->totface;
				
					if(base->object == ob) ok= 1;
				}
			}
		}
		base= base->next;
	}
	
	/* zodoende is het aktieve object altijd select */ 
	if(ok==0) return;
	
	if(totvert==0 || totvert>32767) return;
	
	if(okee("Join selected sectors")==0) return;
	
	/* nieuwe materiaal indexen en hoofdarray */
	matar= callocN(sizeof(void *)*MAXMAT, "join_semesh");
	totcol= ob->totcol;
	
	/* obact materials in nieuw hoofdarray, is mooiere start! */
	for(a=1; a<=ob->totcol; a++) {
		matar[a-1]= give_current_material(ob, a);
		id_us_plus((ID *)matar[a-1]);
		/* id->us ophogen: wordt ook verlaagd */
	}
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) {
			if(ob!=base->object && base->object->type==OB_SECTOR) {
				se= base->object->data;
				me= se->dynamesh;
				if(me && me->totvert) {
					for(a=1; a<=base->object->totcol; a++) {
						ma= give_current_material(base->object, a);
						if(ma) {
							for(b=0; b<totcol; b++) {
								if(ma == matar[b]) break;
							}
							if(b==totcol) {
								matar[b]= ma;
								ma->id.us++;
								totcol++;
							}
							if(totcol>=MAXMAT-1) break;
						}
					}
				}
			}
			if(totcol>=MAXMAT-1) break;
		}
		base= base->next;
	}

	se= ob->data;
	me= se->dynamesh;
	mvert= mvertmain= mallocN(totvert*sizeof(MVert), "joinmesh1");
	if (totface) mface= mfacemain= mallocN(totface*sizeof(MFace), "joinmesh2");
	if(me->mcol) freeN(me->mcol);
	me->mcol= 0;
	vertofs= 0;
	
	/* alle geselecteerde meshes invers transformen in obact */
	Mat4Invert(imat, ob->obmat);
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) {
			if(base->object->type==OB_SECTOR) {
				
				se= base->object->data;
				me= se->dynamesh;
				if(me) {
					if(me->totvert) {
						
						memcpy(mvert, me->mvert, me->totvert*sizeof(MVert));
						
						if(base->object != ob) {
							/* let op: matmul omkeren is ECHT fout */
							Mat4MulMat4(cmat, base->object->obmat, imat);
							
							a= me->totvert;
							while(a--) {
								Mat4MulVecfl(cmat, mvert->co);
								mvert++;
							}
						}
						else mvert+= me->totvert;
						
					}
					if(me->totface) {
					
						/* mapping maken voor materialen */
						bzero(map, 4*MAXMAT);
						for(a=1; a<=base->object->totcol; a++) {
							ma= give_current_material(base->object, a);
							if(ma) {
								for(b=0; b<totcol; b++) {
									if(ma == matar[b]) {
										map[a-1]= b;
										break;
									}
								}
							}
						}
	
						memcpy(mface, me->mface, me->totface*sizeof(MFace));
						
						a= me->totface;
						while(a--) {
							mface->v1+= vertofs;
							mface->v2+= vertofs;
							if(mface->v3) mface->v3+= vertofs;
							if(mface->v4) mface->v4+= vertofs;
							
							mface->mat_nr= map[(int)mface->mat_nr];
							
							mface++;
						}
					}
					vertofs+= me->totvert;
					
				}
			}
		}
		base= base->next;
	}
	
	se= ob->data;
	me= se->dynamesh;
	
	if(me->mface) freeN(me->mface);
	me->mface= mfacemain;
	if(me->mvert) freeN(me->mvert);
	me->mvert= mvertmain;
	
	me->totvert= totvert;
	me->totface= totface;
	
	freedisplist(&me->disp);
	
	/* oude material array */
	for(a=1; a<=ob->totcol; a++) {
		ma= ob->mat[a-1];
		if(ma) ma->id.us--;
	}
	for(a=1; a<=ob->totcol; a++) {
		ma= me->mat[a-1];
		if(ma) ma->id.us--;
	}
	if(ob->mat) freeN(ob->mat);
	if(me->mat) freeN(me->mat);
	ob->mat= me->mat= 0;
	
	if(totcol) {
		me->mat= matar;
		ob->mat= callocN(sizeof(void *)*totcol, "join obmatar");
	}
	else freeN(matar);
	
	ob->totcol= me->totcol= totcol;
	ob->colbits= 0;
	
	/* andere mesh gebruikers */
	test_object_materials((ID *)me);


	/* **************** */
	/*   2: TEXMESH     */
	/* **************** */

	/* tellen */
	totvert= totface= 0;
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) {
			if(base->object->type==OB_SECTOR) {
				se= base->object->data;
				me= se->texmesh;
				if(me) {
					totvert+= me->totvert;
					totface+= me->totface;
				}
			}
		}
		base= base->next;
	}
	
	if(totvert==0 || totvert>32767) return;

	se= ob->data;
	me= se->texmesh;
	mvert= mvertmain= mallocN(totvert*sizeof(MVert), "joinmesh1");
	if (totface) {
		mface= mfacemain= mallocN(totface*sizeof(MFace), "joinmesh2");
		tface= tfacemain= mallocN(totface*sizeof(TFace), "joinmesh3");
	}
	if(me->mcol) freeN(me->mcol);
	me->mcol= 0;
	vertofs= 0;

	base= FIRSTBASE;
	while(base) {
		nextb= base->next;
		if TESTBASE(base) {
			if(base->object->type==OB_SECTOR) {
				
				se= base->object->data;
				me= se->texmesh;
				if(me) {
					if(me->totvert) {
						
						memcpy(mvert, me->mvert, me->totvert*sizeof(MVert));
						
						if(base->object != ob) {
							/* let op: matmul omkeren is ECHT fout */
							Mat4MulMat4(cmat, base->object->obmat, imat);
							
							a= me->totvert;
							while(a--) {
								Mat4MulVecfl(cmat, mvert->co);
								mvert++;
							}
						}
						else mvert+= me->totvert;
						
					}
					if(me->totface) {
					
						memcpy(mface, me->mface, me->totface*sizeof(MFace));
						
						a= me->totface;
						while(a--) {
							mface->v1+= vertofs;
							mface->v2+= vertofs;
							if(mface->v3) mface->v3+= vertofs;
							if(mface->v4) mface->v4+= vertofs;
							mface++;
						}

						memcpy(tface, me->tface, me->totface*sizeof(TFace));						
						tface+= me->totface;
						
					}
					vertofs+= me->totvert;
					
					if(base->object!=ob) {
						remlink(&(G.scene->base), base);
						free_libblock_us(&(G.main->object), base->object);
						freeN(base);
					}
				}
			}
		}
		base= nextb;
	}

	se= ob->data;
	me= se->texmesh;
	
	if(me->mface) freeN(me->mface);
	me->mface= mfacemain;
	if(me->mvert) freeN(me->mvert);
	me->mvert= mvertmain;
	if(me->tface) freeN(me->tface);
	me->tface= tfacemain;
		
	me->totvert= totvert;
	me->totface= totface;
	
	freedisplist(&me->disp);
	freedisplist(&ob->disp);
	
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWBUTSMAT, 0);

}


void clever_numbuts_mesh()
{
	EditVert *eve;
	
	eve= G.edve.first;
	while(eve) {
		if(eve->f & 1) break;
		eve= eve->next;
	}
	if(eve==0) return;

	add_numbut(0, NUM|FLO, "LocX:", -G.vd->clipend, G.vd->clipend, eve->co, 0);
	add_numbut(1, NUM|FLO, "LocY:", -G.vd->clipend, G.vd->clipend, eve->co+1, 0);
	add_numbut(2, NUM|FLO, "LocZ:", -G.vd->clipend, G.vd->clipend, eve->co+2, 0);
	
	do_clever_numbuts("Active Vertex", 3, REDRAW);
}


void insert_radiogour(char *str)
{
	FILE *fp;
	Mesh *me;
	EditVert *eve, **evera, **vr, *v1, *v2, *v3, *v4;
	int a, verts, nr, end, poly, col;
	char s[50];
	
	fp= fopen(str, "r");
	if(fp==NULL) {
		error("Can't read file");
		return;
	}

	fscanf(fp, "%40s", s);

	fscanf(fp, "%d\n", &verts);
	if(verts<=0) {
		fclose(fp);
		error("Read error");
		return;
	}

	if(verts>256000) {
		error("More than 256000 vertices");
		fclose(fp);
		return;
	}
	
	/* zeker zijn dat gouraud kleuren onthouden worden: */
	me= G.obedit->data;
	if(me->mcol==0) me->mcol= mallocN(me->totface*4*sizeof(int), "nepmcol");
	
	vr= evera= mallocN(sizeof(void *)*verts, "eve array");
	
	for(a=0; a<verts; a++, vr++) {
		*vr=eve= addvertlist(0);
		/* fscanf(fp, "%f %f %f %i", eve->co, eve->co+1, eve->co+2, &eve->colg); */
	}
	
	/* de vlakken */
	end= 1;
	while(end>0) {
		end= fscanf(fp,"%d", &poly);
		if(end<=0) break;

		if(poly==3 || poly==4) {
			
			fscanf(fp,"%d", &nr);
			v1= evera[nr];
			fscanf(fp,"%d", &nr);
			v2= evera[nr];
			fscanf(fp,"%d", &nr);
			v3= evera[nr];
			if(poly==4) {
				if( fscanf(fp,"%d", &nr) <=0 ) break;
				v4= evera[nr];
			}
			else v4= 0;
			
			addvlaklist(v1, v2, v3, v4, 0, 0, 0);
			
		}
		else {
			for(a=0;a<poly;a++) {
				end= fscanf(fp,"%d", &nr);
				if(end<=0) break;
			}
		}
		if(end<=0) break;
		
		end= fscanf(fp,"%i\n", &col);

	}

	fclose(fp);
	freeN(evera);
	
	waitcursor(1);
}

MVert *mvertbase;

int verg_mface(x1, x2)
MFace *x1, *x2;
{
	MVert *v1, *v2;
	
	v1= mvertbase+x1->v1;
	v2= mvertbase+x2->v1;
	
	if( v1->co[2] > v2->co[2] ) return 1;
	else if( v1->co[2] < v2->co[2]) return -1;
	return 0;
}


void sort_faces()
{
	Object *ob= OBACT;
	Mesh *me;
	
	if(ob==0) return;
	if(G.obedit) return;
	if(ob->type!=OB_MESH) return;
	
	if(okee("Sort Faces in Z")==0) return;
	me= ob->data;
	if(me->totface==0) return;
	
	mvertbase= me->mvert;
	
	qsort(me->mface, me->totface, sizeof(MFace), (void *)verg_mface);
	
	allqueue(REDRAWVIEW3D, 0);
}

void vertices_to_sphere()
{
	EditVert *eve;
	Object *ob= OBACT;
	float *curs, len, vec[3], cent[3], fac, facm, imat[3][3], bmat[3][3];
	int tot;
	short perc=100;
	
	if(ob==0) return;
	TEST_EDITMESH
	
	if(button(&perc, 1, 100, "Percentage:")==0) return;
	fac= perc/100.0;
	facm= 1.0-fac;
	
	Mat3CpyMat4(bmat, ob->obmat);
	Mat3Inv(imat, bmat);

	/* centrum */
	curs= give_cursor();
	cent[0]= curs[0]-ob->obmat[3][0];
	cent[1]= curs[1]-ob->obmat[3][1];
	cent[2]= curs[2]-ob->obmat[3][2];
	Mat3MulVecfl(imat, cent);

	len= 0.0;
	tot= 0;
	eve= G.edve.first;
	while(eve) {
		if(eve->f & 1) {
			tot++;
			len+= VecLenf(cent, eve->co);
		}
		eve= eve->next;
	}
	len/=tot;
	
	if(len==0.0) len= 10.0;
	
	eve= G.edve.first;
	while(eve) {
		if(eve->f & 1) {
			vec[0]= eve->co[0]-cent[0];
			vec[1]= eve->co[1]-cent[1];
			vec[2]= eve->co[2]-cent[2];
			
			Normalise(vec);
			
			eve->co[0]= fac*(cent[0]+vec[0]*len) + facm*eve->co[0];
			eve->co[1]= fac*(cent[1]+vec[1]*len) + facm*eve->co[1];
			eve->co[2]= fac*(cent[2]+vec[2]*len) + facm*eve->co[2];
			
		}
		eve= eve->next;
	}
	
	allqueue(REDRAWVIEW3D, 0);
	makeDispList(G.obedit);
}

