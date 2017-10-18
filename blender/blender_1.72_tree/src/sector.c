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


/*  sector.c      MIXED MODEL
 * 
 *  maart 96
 *  
 * 
 */

#ifndef FREE

#include "blender.h"
#include "graphics.h"
#include "sector.h"
#include "psx.h"
#include "blendpsx.h"

/* ******************** NOTES ************************** 

	- de interne klok loopt op 50 Hz.
	- variable klok: erg lelijk en instabiel: botsingen/krachten/ipoos hangen nauw samen (ook met events)
	- klok op 25 Hz: zou kunnen. Is dan lastig naar 17 Hz terug te brengen (alsie framerate niet haalt)
  

   ***************************************************** */



/* debug */
int sectcount;
int matcount, spherecount, cylcount, facecount;


ushort simuldevs[32];
short simulvals[32];

void del_dupli_life(Object *ob);
int (*intersect_func)();

Object *main_actor;
char *simul_load_str;

typedef struct Snijp {
	float labda, minlabda, inpspeed;
	float radius, radiusq, slen;/* genormaliseerd, lengte */
	DFace *face;				/* alleen fh */
	Material *ma;				
	float *obmat;				/* als hier iets staat: ook doen! */
	float no[3];				/* als flag==1, staat hier normaal */
	float speedn[3];			
	ushort ocx, ocy, ocz;		/* van de 'ray' of de life */
	char flag;					/* 0: vlak gesneden, 1: cylinder/sphere, 2:life */
	char lay;
} Snijp;


void ApplyMatrix(void *mat, float *old, float *new)
{
	
	/* if(sectcount<100) matcount++; */
	VecMat4MulVecfl(new, mat, old);
}

/* ************** PORTALS ************** */

int passed_portal(int type, float *size, float *vec)
{
	
	switch(type) {
	case PO_XPOS:
		if(vec[0]>size[0]) return 1;
		break;
	case PO_XNEG:
		if(vec[0]<-size[0]) return 1;
		break;
	case PO_YPOS:
		if(vec[1]>size[1]) return 1;
		break;
	case PO_YNEG:
		if(vec[1]<-size[1]) return 1;
		break;
	case PO_ZPOS:
		if(vec[2]>size[2]) return 1;
		break;
	case PO_ZNEG:
		if(vec[2]<-size[2]) return 1;
		break;
	}
	return 0;
}

int passed_portal_safe(int type, float *size, float *vec, float safe)
{
	
	switch(type) {
	case PO_XPOS:
		if(vec[0]+safe>size[0]) return 1;
		break;
	case PO_XNEG:
		if(vec[0]-safe<-size[0]) return 1;
		break;
	case PO_YPOS:
		if(vec[1]+safe>size[1]) return 1;
		break;
	case PO_YNEG:
		if(vec[1]-safe<-size[1]) return 1;
		break;
	case PO_ZPOS:
		if(vec[2]+safe>size[2]) return 1;
		break;
	case PO_ZNEG:
		if(vec[2]-safe<-size[2]) return 1;
		break;
	}
	return 0;
}

/* ******************** LBUF ************************** */


#define LBUFSTEP	4

void add_to_lbuf(LBuf *lbuf, Object *ob)
{
	
	if(lbuf->max==0) {
		lbuf->ob= callocN(sizeof(void *)*LBUFSTEP, "firstlbuf");
		lbuf->max= LBUFSTEP;
		lbuf->tot= 1;
		lbuf->ob[0]= ob;
	}
	else {
		Object **obar;
		
		/* dubbels: gewoon toestaan */
		
		if(lbuf->tot==lbuf->max) {
			lbuf->max+= LBUFSTEP;
			obar= callocN(sizeof(void *)*lbuf->max, "nlbuf");
			LONGCOPY(obar, lbuf->ob, lbuf->max);
			freeN(lbuf->ob);
			lbuf->ob= obar;
		}
		lbuf->ob[ lbuf->tot ]= ob;
		lbuf->tot++;
		
	}
}

void free_lbuf(LBuf *lbuf)
{
	if(lbuf->ob) freeN(lbuf->ob);
	lbuf->ob= 0;
	lbuf->max= lbuf->tot= 0;
}

void del_from_lbuf(LBuf *lbuf, Object *ob)
{
	int a;
	
	if(lbuf->tot==0 || lbuf->max==0) return;
	
	for(a=0; a<lbuf->tot; a++) {
		
		/* dubbels: wel afvangen */
		
		if(lbuf->ob[a] == ob) {
			lbuf->tot--;
			if(lbuf->tot>a) {
				lbuf->ob[a] = lbuf->ob[ lbuf->tot ];
				lbuf->ob[ lbuf->tot ]= 0;				/* hoeft niet? */
			}
			else lbuf->ob[a]= 0;
		}
		
	}
	
}

/* ****************** OCTREE x:16 y:16 z:16 *************************** */

void init_snijp(Mesh *me, Snijp *sn, float *oldloc, float *speed)
{
	float vec[3], *dvec, *size;
	short a, b, min[3], max[3];
	
	/* optimalisering sphere en cyl */
	VECCOPY(sn->speedn, speed);
	sn->slen= Normalise(sn->speedn);

	sn->ocx= 0;		/* voldoende om hierop te testen */
					/* ivm. returns wel pas op 't eind berekenen */

	
	if(me==0 || me->oc==0 || sn->slen==0.0) return;
	
	dvec= me->oc->dvec;
	size= me->oc->size;
	
	vec[0]= oldloc[0]-dvec[0];
	vec[1]= oldloc[1]-dvec[1];
	vec[2]= oldloc[2]-dvec[2];
					
	if(size[0]!=0.0) {
		a= ffloor(16.0*(vec[0]-sn->radius)/size[0]);
		b= ffloor(16.0*(vec[0]-sn->radius+speed[0])/size[0]);
		min[0]= MIN2(a, b);

		a= ffloor(16.0*(vec[0]+sn->radius)/size[0]);
		b= ffloor(16.0*(vec[0]+sn->radius+speed[0])/size[0]);
		max[0]= MAX2(a, b);

		if(max[0]<0 || min[0]>15) return;
		if(max[0]>15) max[0]= 15;
		if(min[0]< 0) min[0]=  0;
	}
	else {min[0]=0; max[0]= 15;}
	
	if(size[1]!=0.0) {
		a= ffloor(16.0*(vec[1]-sn->radius)/size[1]);
		b= ffloor(16.0*(vec[1]-sn->radius+speed[1])/size[1]);
		min[1]= MIN2(a, b);

		a= ffloor(16.0*(vec[1]+sn->radius)/size[1]);
		b= ffloor(16.0*(vec[1]+sn->radius+speed[1])/size[1]);
		max[1]= MAX2(a, b);

		if(max[1]<0 || min[1]>15) return;
		if(max[1]>15) max[1]= 15;
		if(min[1]< 0) min[1]=  0;
	}
	else {min[1]=0; max[1]= 15;}
	
	if(size[2]!=0.0) {
		a= ffloor(16.0*(vec[2]-sn->radius)/size[2]);
		b= ffloor(16.0*(vec[2]-sn->radius+speed[2])/size[2]);
		min[2]= MIN2(a, b);

		a= ffloor(16.0*(vec[2]+sn->radius)/size[2]);
		b= ffloor(16.0*(vec[2]+sn->radius+speed[2])/size[2]);
		max[2]= MAX2(a, b);

		if(max[2]<0 || min[2]>15) return;
		if(max[2]>15) max[2]= 15;
		if(min[2]<0) min[2]= 0;
	}
	else {min[2]=0; max[2]= 15;}
	
	sn->ocx= BROW(min[0], max[0]);
	sn->ocy= BROW(min[1], max[1]);
	sn->ocz= BROW(min[2], max[2]);

}



/* ****************** INSIDE **************************** */


int sector_cliptest(Sector *se, float *vec)
{
	if(se->type==SE_CUBE) {
		if(vec[0]<-se->size[0]) return PO_XNEG;
		if(vec[1]<-se->size[1]) return PO_YNEG;
		if(vec[2]<-se->size[2]) return PO_ZNEG;
		if(vec[0]>se->size[0]) return PO_XPOS;
		if(vec[1]>se->size[1]) return PO_YPOS;
		if(vec[2]>se->size[2]) return PO_ZPOS;
	}
	
	return 0;
}

int sector_inside(Sector *se, float *vec, float *local)		/* vec is globaal */
{

	if(se->ob==0) return 0;
	ApplyMatrix(se->ob->imat, vec, local);
	if( sector_cliptest(se, local)==0 ) return 1;
	return 0;
}
	
int sector_cliptest_sure(Sector *se, float *vec, float sure)
{
	if(se->type==SE_CUBE) {
		if(vec[0]<-se->size[0]+sure) return PO_XNEG;
		if(vec[1]<-se->size[1]+sure) return PO_YNEG;
		if(vec[2]<-se->size[2]+sure) return PO_ZNEG;
		if(vec[0]>se->size[0]-sure) return PO_XPOS;
		if(vec[1]>se->size[1]-sure) return PO_YPOS;
		if(vec[2]>se->size[2]-sure) return PO_ZPOS;
	}
	
	return 0;
}

/* ook op PSX? */			
int sector_intersect(Sector *se, Object *ob)
{
	Life *lf;
	Mesh *me;
	float vec[3], local[3];
	int a;
	
	if(ob->type!=OB_LIFE) return 0;
	lf= ob->data;
	me= lf->dynamesh;
	if(me==0) me= lf->texmesh;
	if(me==0 || me->bb==0 || me->totface==0) return 0;
	
	for(a=0; a<8; a++) {
		VECCOPY(vec, me->bb->vec[a]);
		Mat4MulVecfl(ob->obmat, vec);
		ApplyMatrix(se->ob->imat, vec, local);
		if( sector_cliptest_sure(se, local, 0.05)==0 ) return 1;
	}
	
	return 0;
}

Sector *find_sector(float *vec, float *local)			/* algemene find */
{
	Base *base;
	
	base= FIRSTBASE;
	while(base) {
		if(base->lay & G.scene->lay) {
			if(base->object->type==OB_SECTOR) {
				
				if(sector_inside(base->object->data, vec, local)) {
					return base->object->data;
				}
			}
		}
		base= base->next;
	}
	return 0;
}


void life_in_sector_test(Object *ob)
{
	Life *lf;
	LBuf *lb;
	Portal *po;
	Sector *se;
	int b, out;
	
	lf= ob->data;
	if(lf->type != LF_DYNAMIC) return;

	/* apart afhandelen voor PROPS! */
	
	/* nog steeds in de huidige sector? */
	if(lf->sector) {
		
		out= sector_cliptest(lf->sector, lf->loc1);
		
		if(out) {
			/* welke portalen */			

			if(lf->dflag & LF_TEMPLIFE) lb= 0;
			else if((lf->flag & LF_MAINACTOR)==0) {
				lb= &(lf->sector->lbuf);
				del_from_lbuf( lb, ob);
			}
			else lb= 0;

			se= lf->sector;
			b= se->totport;
			po= se->portals;
			while(b--) {
				if(po->sector && passed_portal(po->type, se->size, lf->loc1)) {
					
					if(sector_inside(po->sector, lf->loc, lf->loc1)) {
						
						lf->sector= po->sector;
						if(lb) add_to_lbuf( &(lf->sector->lbuf), ob);
						
						return;
					}
				}
				po++;
			}

			/* diagonaal ofzo? */
			se= find_sector(lf->loc, lf->loc1);
			if(se) {
				lf->sector= se;

				if(lb) add_to_lbuf( &(lf->sector->lbuf), ob);
				printf("out, but found one\n");
				return;
			}

			/* helaas: */
			
			
			if(lf->dflag & LF_TEMPLIFE) {
				lf->timer= 0;
			}
			else {
				/* printf("out\n"); */

				lf->loc1[0]= lf->loc1[1]= 0;
				lf->loc1[2]= 0.5*lf->sector->size[2];
				lf->speed[0]= lf->speed[1]= lf->speed[2]= 0;
				
				ApplyMatrix(lf->sector->ob->obmat, lf->loc1, lf->loc);
				/*  misschien speed flippen? twijfelachtig */
			}
		}
	}

	if(lf->sector==0 ) {
		lf->sector= find_sector(lf->loc, lf->loc1);

		if( (lf->dflag & LF_TEMPLIFE)==0) {

/**/	/* dit zit niet in de engine, waarom? */
			if(lf->sector && (lf->flag & LF_MAINACTOR)==0) add_to_lbuf( &(lf->sector->lbuf), ob);
		}
	}
}


void mainactor_in_sector_test()		/* zet G.cursector */
{
	Portal *po;
	float vec[3], local[3];
	int b, out;

	if(main_actor==0) return;
	
	/* vanwege psx */
	vec[0]= main_actor->obmat[3][0];
	vec[1]= main_actor->obmat[3][1];
	vec[2]= main_actor->obmat[3][2];
	
	/* nog steeds in de huidige sector? */
	if(G.cursector) {
		
		/* lokale coord: */
		ApplyMatrix(G.cursector->ob->imat, vec, local);
		
		out= sector_cliptest(G.cursector, local);
		
		if(out) {
			/* welke portalen */
			
			b= G.cursector->totport;
			po= G.cursector->portals;
			while(b--) {
				if(po->sector && passed_portal(po->type, G.cursector->size, local)) {
					
					if(sector_inside(po->sector, vec, local)) {
						G.cursector= po->sector;
						evaluate_camera_network(0, -1);	/* init */
						;
						return;
					}
				}
				po++;
			}

			/* helaas: */
			G.cursector= 0;
		}
	}

	if(G.cursector==0) {
		G.cursector= find_sector(main_actor->obmat[3], local);
		if(G.cursector) {
			evaluate_camera_network(0, -1); /* init */
			;
		}
	}

}


void normal_rot_inv(mat, inv, no)
float mat[][4], inv[][4], *no;
{
	float x, y, z;
	
	x= no[0]*mat[0][0] + no[1]*mat[1][0] + no[2]*mat[2][0];
	y= no[0]*mat[0][1] + no[1]*mat[1][1] + no[2]*mat[2][1];
	z= no[0]*mat[0][2] + no[1]*mat[1][2] + no[2]*mat[2][2];
	
	no[0]= x*inv[0][0] + y*inv[1][0] + z*inv[2][0];
	no[1]= x*inv[0][1] + y*inv[1][1] + z*inv[2][1];
	no[2]= x*inv[0][2] + y*inv[1][2] + z*inv[2][2];
	
	/* geen normalise: inpspeed bevat scale-informatie */
	/* Normalise(no); */
}



/* ********************** DYNA ************************* */


#define UNSUREF	0.02

/*					  labdacor  labda			*/
/*                 -----x---------x-------		*/
/* 						.		  .				*/
/* geval 1         -----.---->    .				*/
/* geval 2            --.---------.-->			*/
/* geval 3              .     ----.------>		*/
/* geval 4              .  -----> .				*/


int intersect_dface(DFace *dface, Snijp *sn, float *oldloc, float *speed)
{
	float ndist, labda, labdacor, s, t, inploc, inpspeed;
	int cox=0, coy=1;


	/* als dface->v3==0 : dface->ocx==0 */
	if( (dface->ocx & sn->ocx)==0 ) return 0;
	if( (dface->ocy & sn->ocy)==0 ) return 0;
	if( (dface->ocz & sn->ocz)==0 ) return 0;

	if((sn->lay & dface->ma->lay)==0) return 0;

	inpspeed= dface->no[0]*speed[0] + dface->no[1]*speed[1] + dface->no[2]*speed[2];

	/* single sided! */	
	if(inpspeed < -TOLER) {
	
		inploc= dface->no[0]*oldloc[0] + dface->no[1]*oldloc[1] + dface->no[2]*oldloc[2];
		ndist= dface->dist - inploc;	/* negatief! */
		
		if(ndist>0.0) return 0;
		
		labda= (ndist)/inpspeed;	/* voor s-t */
		labdacor= (ndist+UNSUREF)/inpspeed;

		/* labda is gegarandeerd > 0! (ndist<0.0) */
		
		/* dit is een soort interval-wiskunde */
		if(labdacor<=sn->minlabda) {
			if(labda<=sn->minlabda) return 0;
		}
		if(labdacor>=sn->labda) {
			if(labda>= sn->labda) return 0;
		}
		
		if(dface->proj==1) coy= 2;
		else if(dface->proj==2) {
			cox= 1; coy= 2;
		}

		s= oldloc[cox] + labda*speed[cox];
		t= oldloc[coy] + labda*speed[coy];

		if( (dface->v2[cox] - s)*(dface->v2[coy] - dface->v1[coy]) - 
			(dface->v2[coy] - t)*(dface->v2[cox] - dface->v1[cox]) < -TOLER ) 
			return 0;

		if( (dface->v3[cox] - s)*(dface->v3[coy] - dface->v2[coy]) -
			(dface->v3[coy] - t)*(dface->v3[cox] - dface->v2[cox])< -TOLER ) 
			return 0;
		
		if(dface->v4==0) {
			if( (dface->v1[cox] - s)*(dface->v1[coy] - dface->v3[coy]) -
				(dface->v1[coy] - t)*(dface->v1[cox] - dface->v3[cox])< -TOLER ) 
				return 0;
		}
		else {
			if( (dface->v4[cox] - s)*(dface->v4[coy] - dface->v3[coy]) -
				(dface->v4[coy] - t)*(dface->v4[cox] - dface->v3[cox])< -TOLER ) 
				return 0;

			if( (dface->v1[cox] - s)*(dface->v1[coy] - dface->v4[coy]) -
				(dface->v1[coy] - t)*(dface->v1[cox] - dface->v4[cox])< -TOLER )
				return 0;
		}

		sn->labda= labdacor;
		sn->inpspeed= inpspeed;
		sn->face= dface;
		sn->flag= 0;
		
		return 1;
	}
	
	return 0;
}

/* genormaliseerde speed en kwadratische straal in sn-struct? inclusief lengte speed */

typedef struct pSnijp {
	int labda, minlabda, inpspeed;
	int radius, radiusq, slen;	/* slen: lengte speed */
	short no[3];				/* als flag==1, staat hier normaal */
	short speedn[3];			/* genormaliseerd */
	ushort ocx, ocy, ocz;		/* van de 'ray' of de life */
	short flag;					/* 0: vlak gesneden, 1: cylinder/sphere */
} pSnijp;

#define FIX(fac)	((int)(4096.0*fac))
#define FIX14(fac)	((int)(16384.0*fac))
#define S14MUL(a, b)           ( ((a)*(b))>>14 )

void fix_coordvec(int *ip, float *fp)
{
	ip[0]= FIX(fp[0]);
	ip[1]= FIX(fp[1]);
	ip[2]= FIX(fp[2]);
	
}

void fix_coordvecs(short *ip, float *fp)
{
	ip[0]= FIX(fp[0]);
	ip[1]= FIX(fp[1]);
	ip[2]= FIX(fp[2]);
}

int SquareRoot0(int num)
{
	return (int)fsqrt( (float)num);
}

int Normalises(short *vec)	/* 12 bits */
{
	float fp[3], len;
	
	VECCOPY(fp, vec);
	len= Normalise(fp);
	vec[0]= FIX(fp[0]);
	vec[1]= FIX(fp[1]);
	vec[2]= FIX(fp[2]);
	
	return (int)(len);
}

void CrossS(c, a, b)
short *c, *a, *b;
{
	c[0] = (a[1] * b[2] - a[2] * b[1])>>12;
	c[1] = (a[2] * b[0] - a[0] * b[2])>>12;
	c[2] = (a[0] * b[1] - a[1] * b[0])>>12;
}


int vertex_sphere_intersect_i(int *v1, pSnijp *sn, int *oldloc, short *speed)
{
	int labda, labdacor, len, bsq, u, disc, rc[3];

	if(sn->slen < 10) return 0;		/* superslow */
	
	rc[0]= oldloc[0]-v1[0];
	rc[1]= oldloc[1]-v1[1];
	rc[2]= oldloc[2]-v1[2];
	bsq= (rc[0]*sn->speedn[0] + rc[1]*sn->speedn[1] + rc[2]*sn->speedn[2])>>12; 
	u= rc[0]*rc[0] + rc[1]*rc[1] + rc[2]*rc[2] - sn->radiusq;

	disc= bsq*bsq - u;
	
	if(disc>=0) {
		disc= SquareRoot0(disc);
		
		u= -bsq - disc;
		/* mag je delen? */
		if( u < -sn->slen || u> sn->slen) return 0;
		
		labdacor= (u<<14)/(sn->slen);		/* 14 bits */
		u= -bsq + disc;
		labda= (u<<12)/(sn->slen>>2);
		
	}
	else return 0;	

	/* twee gevallen waarbij geen snijpunt is */
	if(labdacor>=sn->labda && labda>=sn->labda) return 0;
	if(labdacor<=sn->minlabda && labda<=sn->minlabda) return 0;

PRINT2(f, f, labdacor/16384.0, labda/16384.0);


	/* snijpunt en normaal */
	/* hier 12 bits */
	sn->no[0]= rc[0] + ((labdacor*speed[0])>>14) ;
	sn->no[1]= rc[1] + ((labdacor*speed[1])>>14) ;
	sn->no[2]= rc[2] + ((labdacor*speed[2])>>14) ;

PRINT3(f, f, f, sn->no[0]/4096.0, sn->no[1]/4096.0, sn->no[2]/4096.0);
	
	sn->labda= labdacor;

	/* inpspeed op lengte brengen: twee keer radius!!! (inpspeed wordt weer met normaal vermenigvuldigd)*/
	/* inspeed: 16 bits */
	sn->inpspeed= ( (sn->no[0]*speed[0] + sn->no[1]*speed[1] + sn->no[2]*speed[2])<<4 )/(sn->radiusq>>12);

PRINT(f, sn->inpspeed/65536.0);

	sn->flag= 1;

	if(FALSE) {	/* namaak refl */
		int fac;
		short speed1[3];
		
		VECCOPY(speed1, speed);
		
		fac= S14MUL( -4096-4096, sn->inpspeed);

		speed1[0]+= S14MUL(fac, sn->no[0]);
		speed1[1]+= S14MUL(fac, sn->no[1]);
		speed1[2]+= S14MUL(fac, sn->no[2]);
		
		PRINT3(f, f, f, speed1[0]/4096.0, speed1[1]/4096.0, speed1[2]/4096.0);
	}

	return 1;

}

int sphere_sphere_intersect(float *v1, Snijp *sn, float *oldloc, float *speed)
{
	float labda, labdacor, bsq, u, disc, rc[3];
	float radius, radiusq;
	
	radius= v1[3]+sn->radius;
	radiusq= radius*radius;
				
	rc[0]= oldloc[0]-v1[0];
	rc[1]= oldloc[1]-v1[1];
	rc[2]= oldloc[2]-v1[2];
	bsq= rc[0]*sn->speedn[0] + rc[1]*sn->speedn[1] + rc[2]*sn->speedn[2]; 
	u= rc[0]*rc[0] + rc[1]*rc[1] + rc[2]*rc[2] - radiusq;

	disc= bsq*bsq - u;

	if(disc>=0.0) {
		disc= fsqrt(disc);
		labdacor= (-bsq - disc)/sn->slen;	/* intrede */
		labda= (-bsq + disc)/sn->slen;
	}
	else return 0;	


	/* twee gevallen waarbij geen snijpunt is */
	if(labdacor>=sn->labda && labda>=sn->labda) return 0;
	if(labdacor<=sn->minlabda && labda<=sn->minlabda) return 0;

	/* PATCH!!! */
	if(labdacor<0.0) labdacor/= 2.0;


	/* snijpunt en normaal */
	sn->no[0]= rc[0] + labdacor*speed[0] ;
	sn->no[1]= rc[1] + labdacor*speed[1] ;
	sn->no[2]= rc[2] + labdacor*speed[2] ;

	/* corrigeren: v1[3]= axsize */
	disc= v1[3]/radius;
	sn->no[0]*= disc;
	sn->no[1]*= disc;
	sn->no[2]*= disc;

	sn->labda= labdacor;
	
	/* inpspeed op lengte brengen: twee keer radius!!! (inpspeed wordt weer met normaal vermenigvuldigd)*/
	radiusq= v1[3]*v1[3];
	sn->inpspeed= (sn->no[0]*speed[0] + sn->no[1]*speed[1] + sn->no[2]*speed[2])/(radiusq);

	sn->flag= 2;

	return 1;
}


int vertex_sphere_intersect(float *v1, Snijp *sn, float *oldloc, float *speed)
{
	float labda, labdacor, bsq, u, disc, rc[3];


	if(FALSE){
		pSnijp psn;
		int iv1[3], ioldloc[3];
		short ispeed[3];
		
		fix_coordvec(iv1, v1);
		fix_coordvec(ioldloc, oldloc);
		fix_coordvecs(ispeed, speed);
		
		psn.radius= FIX(sn->radius);
		psn.radiusq= psn.radius*psn.radius;
		psn.labda= FIX14(sn->labda);
		psn.minlabda= FIX14(sn->minlabda);
		
		fix_coordvecs(psn.speedn, sn->speedn);
		psn.slen= FIX(sn->slen);
		
		vertex_sphere_intersect_i(iv1, &psn, ioldloc, ispeed);

	}


	/* if(sectcount<100) spherecount++;		 */

	rc[0]= oldloc[0]-v1[0];
	rc[1]= oldloc[1]-v1[1];
	rc[2]= oldloc[2]-v1[2];
	bsq= rc[0]*sn->speedn[0] + rc[1]*sn->speedn[1] + rc[2]*sn->speedn[2]; 
	u= rc[0]*rc[0] + rc[1]*rc[1] + rc[2]*rc[2] - sn->radiusq;

	disc= bsq*bsq - u;

	if(disc>=0.0) {
		disc= fsqrt(disc);
		labdacor= (-bsq - disc)/sn->slen;	/* intrede */
		labda= (-bsq + disc)/sn->slen;
	}
	else return 0;	

	/* twee gevallen waarbij geen snijpunt is */
	if(labdacor>=sn->labda && labda>=sn->labda) return 0;
	if(labdacor<=sn->minlabda && labda<=sn->minlabda) return 0;

/* PRINT2(f, f, labdacor, labda); */

	/* snijpunt en normaal */
	sn->no[0]= rc[0] + labdacor*speed[0] ;
	sn->no[1]= rc[1] + labdacor*speed[1] ;
	sn->no[2]= rc[2] + labdacor*speed[2] ;

/* PRINT3(f, f, f, sn->no[0], sn->no[1], sn->no[2]); */

	sn->labda= labdacor;
	/* inpspeed op lengte brengen: twee keer radius!!! (inpspeed wordt weer met normaal vermenigvuldigd)*/
	sn->inpspeed= (sn->no[0]*speed[0] + sn->no[1]*speed[1] + sn->no[2]*speed[2])/(sn->radiusq);
	sn->flag= 1;

/* PRINT(f, sn->inpspeed); */
	

	if(FALSE) {	/* namaak refl */
		float fac, speed1[3];
		
		VECCOPY(speed1, speed);
		
		fac= (-2.0)*sn->inpspeed;
	
		speed1[0]+= fac*sn->no[0];
		speed1[1]+= fac*sn->no[1];
		speed1[2]+= fac*sn->no[2];
		
		PRINT3(f, f, f, speed1[0], speed1[1], speed1[2]);
	}

	return 1;
}

int cylinder_edge_intersect_i(int *base, int *v2, pSnijp *sn, int *oldloc, short *speed)
{
	int osp, rdd, s, t, dist, len, len2, labda, labdacor, rc[3];
	short axis[3], n[3], o[3];
	
PRINT(d, sn->slen);
	if(sn->slen < 10) return 0;		/* superslow */
	
	axis[0]= v2[0]-base[0];
	axis[1]= v2[1]-base[1];
	axis[2]= v2[2]-base[2];
	len2= Normalises(axis);		/* op PSX:test eerst! */

	rc[0]= oldloc[0]-base[0];
	rc[1]= oldloc[1]-base[1];
	rc[2]= oldloc[2]-base[2];
	
	CrossS(n, speed, axis);
	len= Normalises(n);
	if(len==0) return 0;

	dist= abs( (rc[0]*n[0] + rc[1]*n[1] + rc[2]*n[2])>>12 );
PRINT2(d, d, dist, sn->radius);
	if( dist>=sn->radius ) return 0;
	
	/* CrossS(o, rc, axis); */
	o[0] = (rc[1] * axis[2] - rc[2] * axis[1])>>12;
	o[1] = (rc[2] * axis[0] - rc[0] * axis[2])>>12;
	o[2] = (rc[0] * axis[1] - rc[1] * axis[0])>>12;

	t= -((o[0]*n[0] + o[1]*n[1] + o[2]*n[2])<<2)/(len);	/* 14 bits */
	
	CrossS(o, n, axis);
	
	osp= (o[0]*speed[0] + o[1]*speed[1] + o[2]*speed[2])>>12;
	rdd= SquareRoot0(sn->radiusq-dist*dist);
	s=  abs( (rdd<<14) / osp);

PRINT2(f, f, s/16384.0, t/16384.0);
	
	labdacor= (t-s);
	labda= (t+s);
	
PRINT2(f, f, labdacor/16384.0, labda/16384.0);

	/* twee gevallen waarbij geen snijpunt is */
	if(labdacor>=sn->labda && labda>=sn->labda) return 0;
	if(labdacor<=sn->minlabda && labda<=sn->minlabda) return 0;
	
	/* normaalvector berekenen */
	/* snijpunt: */
	
	rc[0]= rc[0] + ((labdacor*speed[0])>>14);
	rc[1]= rc[1] + ((labdacor*speed[1])>>14);
	rc[2]= rc[2] + ((labdacor*speed[2])>>14);
	
	s= (rc[0]*axis[0] + rc[1]*axis[1] + rc[2]*axis[2])>>12 ;
	
	if(s<0 || s>len2) return 0;
	
	/* tot aan de laatste return niets in de sn struct invullen! */	
	
	/* 12 bits */
	sn->no[0]= rc[0] - ((s*axis[0])>>12);
	sn->no[1]= rc[1] - ((s*axis[1])>>12);
	sn->no[2]= rc[2] - ((s*axis[2])>>12);
		
	sn->labda= labdacor;
	
	/* 16 bits */
	sn->inpspeed= ( (sn->no[0]*speed[0] + sn->no[1]*speed[1] + sn->no[2]*speed[2])<<4 )/(sn->radiusq>>12);

	sn->flag= 1;

PRINT(f, sn->inpspeed/65536.0);

	if(TRUE) {	/* namaak refl */
		int fac;
		short speed1[3];
		
		VECCOPY(speed1, speed);
		
		fac= S14MUL( -4096-4096, sn->inpspeed);

		speed1[0]+= S14MUL(fac, sn->no[0]);
		speed1[1]+= S14MUL(fac, sn->no[1]);
		speed1[2]+= S14MUL(fac, sn->no[2]);
		
		PRINT3(f, f, f, speed1[0]/4096.0, speed1[1]/4096.0, speed1[2]/4096.0);
	}

	return 1;

}


int cylinder_edge_intersect(float *base, float *v2, Snijp *sn, float *oldloc, float *speed)
{
	float s, t, dist, len, len2, labda, labdacor, axis[3], rc[3], n[3], o[3];
	
	if(FALSE){
		pSnijp psn;
		int ibase[3], iv2[3], ioldloc[3];
		short ispeed[3];
		
		fix_coordvec(iv2, v2);
		fix_coordvec(ibase, base);
		fix_coordvec(ioldloc, oldloc);
		fix_coordvecs(ispeed, speed);
		
		psn.radius= FIX(sn->radius);
		psn.radiusq= psn.radius*psn.radius;
		psn.labda= FIX14(sn->labda);
		psn.minlabda= FIX14(sn->minlabda);
		
		fix_coordvecs(psn.speedn, sn->speedn);
		psn.slen= FIX(sn->slen);
		
		cylinder_edge_intersect_i(ibase, iv2, &psn, ioldloc, ispeed);

	}

	axis[0]= v2[0]-base[0];
	axis[1]= v2[1]-base[1];
	axis[2]= v2[2]-base[2];
	len2= Normalise(axis);

	rc[0]= oldloc[0]-base[0];
	rc[1]= oldloc[1]-base[1];
	rc[2]= oldloc[2]-base[2];

	/* if(sectcount<100) cylcount++; */

	Crossf(n, speed, axis);
	len= Normalise(n);
	if(len==0.0) return 0;

	dist= fabs( rc[0]*n[0] + rc[1]*n[1] + rc[2]*n[2] );

/* PRINT2(f, f, dist, sn->radius); */
	
	if( dist>=sn->radius ) return 0;
	
	Crossf(o, rc, axis);
	t= -(o[0]*n[0] + o[1]*n[1] + o[2]*n[2])/len;
	
	Crossf(o, n, axis);
	s=  fabs(fsqrt(sn->radiusq-dist*dist) / (o[0]*speed[0] + o[1]*speed[1] + o[2]*speed[2]));
/* PRINT2(f, f, s, t );	 */
	labdacor= (t-s);
	labda= (t+s);

/* PRINT2(f, f, labdacor, labda); */

	/* twee gevallen waarbij geen snijpunt is */
	if(labdacor>=sn->labda && labda>=sn->labda) return 0;
	if(labdacor<=sn->minlabda && labda<=sn->minlabda) return 0;
	
	/* normaalvector berekenen */
	/* snijpunt: */
	
	rc[0]= rc[0] + labdacor*speed[0] ;
	rc[1]= rc[1] + labdacor*speed[1] ;
	rc[2]= rc[2] + labdacor*speed[2] ;
	
	s= (rc[0]*axis[0] + rc[1]*axis[1] + rc[2]*axis[2]) ;
	
	if(s<0.0 || s>len2) return 0;
	
	/* tot aan de laatste return niets in de sn struct invullen! */	

	sn->no[0]= (rc[0] - s*axis[0]);
	sn->no[1]= (rc[1] - s*axis[1]);
	sn->no[2]= (rc[2] - s*axis[2]);
		
	sn->labda= labdacor;
	sn->inpspeed= (sn->no[0]*speed[0] + sn->no[1]*speed[1] + sn->no[2]*speed[2])/sn->radiusq;
	sn->flag= 1;

/* PRINT(f, sn->inpspeed); */

	if(FALSE) {	/* namaak refl */
		float fac, speed1[3];
		
		VECCOPY(speed1, speed);
		
		fac= (-2.0)*sn->inpspeed;
	
		speed1[0]+= fac*sn->no[0];
		speed1[1]+= fac*sn->no[1];
		speed1[2]+= fac*sn->no[2];
		
		PRINT3(f, f, f, speed1[0], speed1[1], speed1[2]);
	}

	return 1;
}

int intersect_dface_cyl(DFace *dface, Snijp *sn, float *oldloc, float *speed)
{
	float *v1, *v2, *v3, *v4;
	float ndist, labda, labdacor, s, t, inploc, inpspeed;
	float out;
	short cox=0, coy=1, ok, ed, cytest;
	
	/* als dface->v3==0 : dface->ocx==0 */
	if( (dface->ocx & sn->ocx)==0 ) return 0;
	if( (dface->ocy & sn->ocy)==0 ) return 0;
	if( (dface->ocz & sn->ocz)==0 ) return 0;
	

	if((sn->lay & dface->ma->lay)==0) return 0;

dface->flag |= DF_HILITE;	

	inpspeed= dface->no[0]*speed[0] + dface->no[1]*speed[1] + dface->no[2]*speed[2];

	/* single sided! */	
	if(inpspeed < -TOLER) {
	
		inploc= dface->no[0]*oldloc[0] + dface->no[1]*oldloc[1] + dface->no[2]*oldloc[2];
		ndist= dface->dist - inploc;	/* negatief! */
		
		if(ndist>0.0) return 0;
		
		labda= (ndist)/inpspeed;	/* voor s-t */
		labdacor= (ndist+sn->radius)/inpspeed;

		/* labda is gegarandeerd > 0! (ndist<0.0) */
		
		/* twee gevallen waarbij geen snijpunt is */
		if(labdacor>=sn->labda && labda>=sn->labda) return 0;
		if(labdacor<=sn->minlabda && labda<=sn->minlabda) return 0;
		
		if(dface->proj==1) coy= 2;
		else if(dface->proj==2) {
			cox= 1; coy= 2;
		}
		
		/* testen oftie exact het vlak snijdt */
		if(labdacor<=0.0) {
			/* geen labda, wel met correctie zodat snijp binnen vlak valt */
			/* dit is in feite de loodrechte projektie */
			s= oldloc[cox] - sn->radius*dface->no[cox];
			t= oldloc[coy] - sn->radius*dface->no[coy];
			labdacor= 0.0;
		}
		else {
			/* met correctie zodat snijp binnen vlak valt */
			s= oldloc[cox] + labdacor*speed[cox] - sn->radius*dface->no[cox];
			t= oldloc[coy] + labdacor*speed[coy] - sn->radius*dface->no[coy];
		}

		/* if(sectcount<100) facecount++; */
		
		v1= dface->v1;
		v2= dface->v2;
		v3= dface->v3;
		v4= dface->v4;
		
		cytest= 0;
		out= (v2[cox] - s)*(v2[coy] - v1[coy]) - (v2[coy] - t)*(v2[cox] - v1[cox]);
		if(out > -TOLER) {
			out= (v3[cox] - s)*(v3[coy] - v2[coy]) - (v3[coy] - t)*(v3[cox] - v2[cox]);
			if(out > -TOLER) {
				
				ok= 0;
				
				if(v4==0) {
					out= (v1[cox] - s)*(v1[coy] - v3[coy]) - (v1[coy] - t)*(v1[cox] - v3[cox]);
					if(out > -TOLER) ok= 1;
					else cytest= 3;
				}
				else {
					out= (v4[cox] - s)*(v4[coy] - v3[coy]) - (v4[coy] - t)*(v4[cox] - v3[cox]);

					if(out > -TOLER) {
						out= (v1[cox] - s)*(v1[coy] - v4[coy]) - (v1[coy] - t)*(v1[cox] - v4[cox]);

						if(out > -TOLER) ok= 1;
						else cytest= 5;
					}
					else cytest= 4;
				}
				if(ok) {

					sn->labda= labdacor;
					sn->inpspeed= inpspeed;
					sn->face= dface;
					sn->flag= 0;
					return 1;
				}
			}
			else cytest= 2;
		}
		else cytest= 1;
	
		/* edges (= cylinders) testen */
		ok= 0;
		ed= dface->edcode;
		
		
		switch(cytest) {
		case 1:
			if( (ed & DF_V1V2) && cylinder_edge_intersect(v1, v2, sn, oldloc, speed) )
				ok= 1;
			break;
		case 2:
			if( (ed & DF_V2V3) && cylinder_edge_intersect(v2, v3, sn, oldloc, speed) )
				ok= 1;
			break;
		case 3:
			if( (ed & DF_V3V1) && cylinder_edge_intersect(v3, v1, sn, oldloc, speed) )
				ok= 1;
			break;
		case 4:
			if( (ed & DF_V3V4) && cylinder_edge_intersect(v3, v4, sn, oldloc, speed) )
				ok= 1;
			break;
		case 5:
			if( (ed & DF_V4V1) && cylinder_edge_intersect(v4, v1, sn, oldloc, speed) )
				ok= 1;
			break;
		}

		if(ok) sn->face= dface;
		
		if(ok==0) {
			if( (ed & DF_V1) && vertex_sphere_intersect(v1, sn, oldloc, speed)) {
				sn->face= dface;
				
				return 1;
			}
			if( (ed & DF_V2) && vertex_sphere_intersect(v2, sn, oldloc, speed)) {
				sn->face= dface;
				
				return 1;
			}
			if( (ed & DF_V3) && vertex_sphere_intersect(v3, sn, oldloc, speed)) {
				sn->face= dface;
				
				return 1;
			}
			if(v4) if( (ed & DF_V4) && vertex_sphere_intersect(v4, sn, oldloc, speed)) {
				sn->face= dface;
				
				return 1;
			}
			
		}
		
		/* PATCH: onderzoeken hoe en wat en waarom: extreem negatieve labda's */
		/* waarschijnlijk iets van doen met meerdere snijps? */
		
		if(sn->labda<sn->minlabda) sn->labda= sn->minlabda;

		return ok;
	}
	
	return 0;
}


int intersect_dynalife(Object *ob, Life *lf, Snijp *sn)
{
	extern Material defmaterial;
	Object *obs;
	Life *lfs;
	float mindist, loc1[4];		/* loc1 is incl axsize */
	int a, b, found= 0;

	/* nodig voor genormaliseerde speed */
	init_snijp(0, sn, lf->oldloc1, lf->speed1);

	if(sn->slen < 0.0001) return 0;
	
	a= G.totlife;
	while(a--) {
	
		obs= G.lifebuf[a];
		if(obs->type!=OB_CAMERA) {
			lfs= obs->data;
			if(lf!=lfs && lfs->collision!=ob && lf->from!=obs && lfs->type==LF_DYNAMIC && (lfs->dflag & LF_TEMPLIFE)==0) {
				
				/* manhattan pre-test */
				mindist= 3.0*(sn->radius+lfs->axsize);
				
				if( fabs(lf->loc[0]-lf->loc[0]) > mindist ) continue;
				if( fabs(lf->loc[1]-lf->loc[1]) > mindist ) continue;
				if( fabs(lf->loc[2]-lf->loc[2]) > mindist ) continue;
				
				/* gebruik lokale sector coords */
				ApplyMatrix(lf->sector->ob->imat, lfs->loc, loc1);
				loc1[3]= lfs->axsize;
				
				/* truuk: bollen met stralen r1 en r2 snijden is equiv. 
				   met lijn snijden met bol van straal r1+r2 ! */
				
				if(sphere_sphere_intersect(loc1, sn, lf->oldloc1, lf->speed1)) {
					found= 1;
					sn->ma= &defmaterial;
					sn->obmat= 0;
					sn->face= 0;
					
					lf->collision= obs;
					lfs->collision= ob;
				}
			}
		}
	}
	
	/* hier geen patch: in ruil voor labdacor delen door 2 (sphere_sphere_intersect) */
		
	return found;
}


int intersect_prop(Sector *se, Life *lf, Snijp *sn)
{
	/* botst Life *lf tegen een van de props uit *se ? */
	Object *ob;
	Life *lfs;
	DFace *dface;
	float oldloc2[3], loc2[3], speed2[3];
	int a, b, found= 0;


	for(b=0; b<se->lbuf.tot; b++) {
		ob= se->lbuf.ob[b];
		
		if(ob->lay & G.scene->lay) {	/* ivm layer event */
			
			lfs= ob->data;

			if(lf!=lfs && lfs->dynamesh && lfs->type==LF_PROP) {
						
				/* transformeren naar life coordinaten */
				ApplyMatrix(ob->imat, lf->loc, loc2);
				ApplyMatrix(ob->imat, lf->oldloc, oldloc2);
	
				speed2[0]= loc2[0]-oldloc2[0];
				speed2[1]= loc2[1]-oldloc2[1];
				speed2[2]= loc2[2]-oldloc2[2];
	
				init_snijp(lfs->dynamesh, sn, oldloc2, speed2);
				if(sn->ocx) {

					dface= lfs->dynamesh->dface;
					a= lfs->dynamesh->totface;

					while(a--) {
						if( intersect_func(dface, sn, oldloc2, speed2)) {
							sn->obmat= ob->obmat[0];
							lf->collision= ob;	
							lfs->collision= ob;		/* alleen waar/nietwaar */
							found= 1;
						}
						dface++;
					}
				}
			}
		}
	}
	
	return found;
}

void force_from_prop(Life *lf, Snijp *sn)	/* maar 1 tegelijk!!! */
{
	Object *ob;
	Life *lfs;
	Sector *se;
	DFace *dface;
	float fac, oldloc2[3], loc2[3], speed2[3], *no;
	int a, b, found;

	/* struct sn wordt doorgegeven vanwege de reeds ingevulde radius */

	/* mogelijke denkfout: het eerste de beste vlak wordt gepakt! is soms
	 * niet het vlak met de meeste/juiste force...
	 * Voorlopig afhandelen door Fh ook bij props te doen?
	 */

	se= lf->sector;

	for(b=0; b<se->lbuf.tot; b++) {
		ob= se->lbuf.ob[b];
		
		if(G.scene->lay & ob->lay) {
			lfs= ob->data;
	
			if(lf!=lfs && (lfs->dflag & LF_DYNACHANGED)) {
				if(lfs->type==LF_PROP && lfs->dynamesh) {
							
					sn->minlabda= 0.0;
					sn->labda= 1.0;
					found= 0;
					
					/* werken aan de hand van de 'virtuele' vorige positie van life */
					ApplyMatrix(ob->imat, lf->loc, loc2);
					ApplyMatrix(lfs->oldimat, lf->oldloc, oldloc2);
					
					speed2[0]= loc2[0]-oldloc2[0];
					speed2[1]= loc2[1]-oldloc2[1];
					speed2[2]= loc2[2]-oldloc2[2];
	
					init_snijp(lfs->dynamesh, sn, oldloc2, speed2);
					if(sn->ocx) {
					
						dface= lfs->dynamesh->dface;
						a= lfs->dynamesh->totface;
						
						while(a--) {
							if( intersect_func(dface, sn, oldloc2, speed2)) {
								found= 1;
							}
							dface++;
						}
	
						if(found) {
		
							/* de bots loc */
							oldloc2[0]+= sn->labda*speed2[0];
							oldloc2[1]+= sn->labda*speed2[1];
							oldloc2[2]+= sn->labda*speed2[2];
							
							/* nieuwe speed, hier refl.demping: -1.0: parrallel aan vlak, -2.0: zuivere botsing */
							
							fac= (-1.0- DYNA_REF(sn->face->ma))*sn->inpspeed;
							
							/* oplossing: hier stond +=, komt neer op min speed2 !!! */
							/* min speed2= exact de speed van het botsvlak!!! */

							if(sn->flag==0) no= sn->face->no; else no= sn->no;
							
							speed2[0]= fac*no[0];
							speed2[1]= fac*no[1];
							speed2[2]= fac*no[2];
				
							/* de virtuele oldloc */
							oldloc2[0]-= sn->labda*speed2[0];
							oldloc2[1]-= sn->labda*speed2[1];
							oldloc2[2]-= sn->labda*speed2[2];
			
							/* de (nieuwe) eindlocatie */
							loc2[0]= oldloc2[0]+speed2[0];
							loc2[1]= oldloc2[1]+speed2[1];
							loc2[2]= oldloc2[2]+speed2[2];
					
							ApplyMatrix(ob->obmat, loc2, lf->loc);
							
							Mat4Mul3Vecfl(ob->obmat, speed2);
		
							VECCOPY(lf->speed, speed2);
							/* oldloc niet meer nodig */
		
							return;
						}
					}
				}
			}
		}
	}
}

int test_visibility(float *lookat, float *from, Life *lf, Sector *se)
{
	Snijp sn;
	Mesh *me;
	DFace *dface;
	float fac, loc1[3], oldloc1[3], speed1[3];
	short a, found;
	
	if(se==0 || se->dynamesh==0) return 0;
	
	/* transformeren naar lokale sector coords en intersecten */

	ApplyMatrix(se->ob->imat, from, loc1);
	ApplyMatrix(se->ob->imat, lookat, oldloc1);
	
	/* hier oppassen met shorts! */
	
	speed1[0]= loc1[0]-oldloc1[0];
	speed1[1]= loc1[1]-oldloc1[1];
	speed1[2]= loc1[2]-oldloc1[2];

	me= se->dynamesh;

	sn.minlabda= sn.radius= 0.0;	/* radius op nul voor init_snijp */
	sn.labda= 1.0;
	sn.obmat= 0;
	sn.lay= lf->lay;
	found= 0;

	init_snijp(me, &sn, oldloc1, speed1);

	if(sn.ocx) {
	
		dface= me->dface;
		a= me->totface;
		
		while(a--) {
			if( intersect_dface(dface, &sn, oldloc1, speed1)) found= 1;
			dface++;
		}
		
		if(found) {
			
			/* de bots loc */
			oldloc1[0]+= sn.labda*speed1[0];
			oldloc1[1]+= sn.labda*speed1[1];
			oldloc1[2]+= sn.labda*speed1[2];

			/* reflectie berekenen */
			fac= (-2.0)*sn.inpspeed;
			speed1[0]+= fac*sn.face->no[0];
			speed1[1]+= fac*sn.face->no[1];
			speed1[2]+= fac*sn.face->no[2];
			
			fac= (1.0 - sn.labda);

			/* endloc */
			oldloc1[0]+= fac*speed1[0];
			oldloc1[1]+= fac*speed1[1];
			oldloc1[2]+= fac*speed1[2];
			
			/* blenden */
			loc1[0]= (15*loc1[0] + oldloc1[0])/16.0;
			loc1[1]= (15*loc1[1] + oldloc1[1])/16.0;
			loc1[2]= (15*loc1[2] + oldloc1[2])/16.0;
			
			ApplyMatrix(se->ob->obmat, loc1, from);

			return 1;
		}

	}
	
	return 0;
}


void collision_detect(Object *ob, Life *lf)
{
	/* een soort reetrees routine: zoek dichtstbijzijnde snijpunt */
	/* werken met de lokale life co's */
	Mesh *me;
	Portal *po;
	Sector *se;
	DFace *dface, *from=0;
	float *oldloc1, *speed1, *no, fac, len;
	Snijp sn;
	int a, b, found=1, transback=0, colcount=0;

	lf->collision= 0;

	se= lf->sector;
	me= se->dynamesh;
	if(me==0) return;

	sn.minlabda= 0.0;
	sn.lay= lf->lay;
	
	if(lf->flag & LF_SPHERE) {
		sn.radius= lf->axsize;
		sn.radiusq= sn.radius*sn.radius;
		intersect_func= intersect_dface_cyl;
	}
	else {
		sn.radius= UNSUREF;		/* voor octree */
		intersect_func= intersect_dface;
	}
	
	/* beetje speedup */
	oldloc1= lf->oldloc1;
	speed1= lf->speed1;

	while(found) {

		sn.labda= 1.0;
		sn.obmat= 0;
		found= 0;
		
		init_snijp(me, &sn, oldloc1, speed1);
		if(sn.ocx) {
		
			dface= me->dface;
			a= me->totface;
			
			while(a--) {
				if(dface!=from) {
					if( intersect_func(dface, &sn, oldloc1, speed1)) found= 1;
				}
				dface++;
			}
			
			if(found) lf->collision= se->ob;
		}
		
		/* de prop lifes */
		/* tijdelijk lopen we ze allemaal af */
		/* toch maar vast de globale opnieuw berekenen... */
		if(transback) {
			life_from_inv_sector_co(lf);
			transback= 0;
		}

		if(se->lbuf.tot) found |= intersect_prop(se, lf, &sn);
		
		found |= intersect_dynalife(ob, lf, &sn);
		
		/* als loc buiten sector ligt, doen ook vlakken van portals mee */
		b= se->totport;
		po= se->portals;
		while(b--) {
			
				/* safe: voor krappe sectoren waar andere in steken! */ 

			if(po->sector && po->sector->dynamesh && passed_portal_safe(po->type, se->size, lf->loc1, lf->axsize)) {
				float oldloc2[3], loc2[3], speed2[3];

				/* toch maar vast de globale opnieuw berekenen want... */
				if(transback) {
					life_from_inv_sector_co(lf);
					transback= 0;
				}
				
				/* ... we moeten transformeren naar nieuwe locale coordinaten */

				ApplyMatrix(po->ob->imat, lf->loc, loc2);
				ApplyMatrix(po->ob->imat, lf->oldloc, oldloc2);

				speed2[0]= loc2[0]-oldloc2[0];
				speed2[1]= loc2[1]-oldloc2[1];
				speed2[2]= loc2[2]-oldloc2[2];

				init_snijp(po->sector->dynamesh, &sn, oldloc2, speed2);
				if(sn.ocx) {

					dface= po->sector->dynamesh->dface;
					a= po->sector->dynamesh->totface;
				
					while(a--) {
						if(dface!=from) {
							if( intersect_func(dface, &sn, oldloc2, speed2)) {
								sn.obmat= po->ob->obmat[0];
								lf->collision= po->ob;
								found= 1;
							}
						}
						dface++;
					}
				}

			}
			po++;
		}
		
		if(found) {
			
			colcount++;
			
			transback= 1;
			from= sn.face;
			
			/* de bots loc */
			lf->colloc[0]= oldloc1[0]+ sn.labda*speed1[0];
			lf->colloc[1]= oldloc1[1]+ sn.labda*speed1[1];
			lf->colloc[2]= oldloc1[2]+ sn.labda*speed1[2];
		
			if(sn.flag==0) {
				no= sn.face->no; 
				sn.ma= sn.face->ma;
			}
			else {
				no= sn.no;
				if(sn.flag==1) sn.ma= sn.face->ma;
			}

			/* nieuwe speed, hier refl.demping: -1.0: parrallel aan vlak, -2.0: zuivere botsing */
			fac= (-1.0- DYNA_REF(sn.ma))*sn.inpspeed;
			
			/* pas op: als sn.obmat (in andere object) de no[] roteren */
			if(sn.obmat) {
				float nor[3];
				
				VECCOPY(nor, no);
				
				normal_rot_inv(sn.obmat, se->ob->imat, nor);
				speed1[0]+= fac*nor[0];
				speed1[1]+= fac*nor[1];
				speed1[2]+= fac*nor[2];

			}
			else {
				speed1[0]+= fac*no[0];
				speed1[1]+= fac*no[1];
				speed1[2]+= fac*no[2];
			}
			
			/* de virtuele oldloc */
			oldloc1[0]= lf->colloc[0] - sn.labda*speed1[0];
			oldloc1[1]= lf->colloc[1] - sn.labda*speed1[1];
			oldloc1[2]= lf->colloc[2] - sn.labda*speed1[2];
			
			if(colcount>3) if( sn.labda <= sn.minlabda) found= 0;
			
			if(sn.minlabda>=1.0) found= 0;
			else sn.minlabda= sn.labda;
	
		}
	}
	
	/* als er gebotst is: terug transformeren naar globale coordinaten */
	if(transback) life_from_inv_sector_co(lf);

	/* als laatste: de kracht die static life uitoefenen kan */
	force_from_prop(lf, &sn);

}

/* ******************* hele fh spul: *********************** */

#define FH_SECTOR	1
#define FH_PROP		2


int intersect_fh(DFace *dface, float *oldloc, Snijp *sn)
{
	float s, t, tlab, inploc, inpspeed;
	short cox=0, coy=1;
	
	/* speed: 0, 0, -1 */
	
	/* als dface->v3==0 : dface->ocx==0 */
	if( (dface->ocx & sn->ocx)==0 ) return 0;
	if( (dface->ocy & sn->ocy)==0 ) return 0;
	if( (dface->ocz & sn->ocz)==0 ) return 0;

dface->flag |= DF_HILITE;

	inpspeed= (-1.0)*dface->no[2];

	/* single sided! */	
	if(inpspeed < -TOLER) {
	
		inploc= dface->no[0]*oldloc[0] + dface->no[1]*oldloc[1] + dface->no[2]*oldloc[2];
		
		tlab= (dface->dist - inploc)/inpspeed;
		if(tlab < 0.0 || tlab >= sn->labda) return 0;
		
		if(dface->proj==1) coy= 2;
		else if(dface->proj==2) {
			cox= 1; coy= 2;
		}
		
		s= oldloc[cox];
		if(coy==2) t= oldloc[coy] - tlab;
		else t= oldloc[coy];

		if( (dface->v2[cox] - s)*(dface->v2[coy] - dface->v1[coy]) - 
			(dface->v2[coy] - t)*(dface->v2[cox] - dface->v1[cox]) < 0.0 ) 
			return 0;

		if( (dface->v3[cox] - s)*(dface->v3[coy] - dface->v2[coy]) -
			(dface->v3[coy] - t)*(dface->v3[cox] - dface->v2[cox])< 0.0 ) 
			return 0;
		
		if(dface->v4==0) {
			if( (dface->v1[cox] - s)*(dface->v1[coy] - dface->v3[coy]) -
				(dface->v1[coy] - t)*(dface->v1[cox] - dface->v3[cox])< 0.0 ) 
				return 0;
		}
		else {
			if( (dface->v4[cox] - s)*(dface->v4[coy] - dface->v3[coy]) -
				(dface->v4[coy] - t)*(dface->v4[cox] - dface->v3[cox])< 0.0 ) 
				return 0;

			if( (dface->v1[cox] - s)*(dface->v1[coy] - dface->v4[coy]) -
				(dface->v1[coy] - t)*(dface->v1[cox] - dface->v4[cox])< 0.0 )
				return 0;
		}
		
		sn->labda= tlab;
		sn->face= dface;
		if(tlab<= DYNA_FH_DIST(dface->ma)) {
			sn->flag= FH_SECTOR;
			return 1;
		}
		return 0;
	}
	
	return 0;
}

/* alleen vor props, iets andere return opzet */

int intersect_fh_rot(DFace *dface, float *oldloc, Snijp *sn, float *speed)
{
	float s, t, inploc, inpspeed, tlab;
	short cox=0, coy=1;
	
	/* speed: 0, 0, -1 (maar dan geroteerd) (lengte 1) */
	
	/* als dface->v3==0 : dface->ocx==0 */
	if( (dface->ocx & sn->ocx)==0 ) return 0;
	if( (dface->ocy & sn->ocy)==0 ) return 0;
	if( (dface->ocz & sn->ocz)==0 ) return 0;

dface->flag |= DF_HILITE;

	inpspeed= -dface->no[0]*speed[0] - dface->no[1]*speed[1] - dface->no[2]*speed[2];

	/* single sided! */	
	if(inpspeed < -TOLER) {
	
		inploc= dface->no[0]*oldloc[0] + dface->no[1]*oldloc[1] + dface->no[2]*oldloc[2];
		
		tlab= (dface->dist - inploc)/inpspeed;
		if(tlab < 0.0 || tlab >= sn->labda) return 0;

		if(dface->proj==1) coy= 2;
		else if(dface->proj==2) {
			cox= 1; coy= 2;
		}
		
		s= oldloc[cox] + tlab*speed[cox];
		t= oldloc[coy] + tlab*speed[coy];

		if( (dface->v2[cox] - s)*(dface->v2[coy] - dface->v1[coy]) - 
			(dface->v2[coy] - t)*(dface->v2[cox] - dface->v1[cox]) < 0.0 ) 
			return 0;

		if( (dface->v3[cox] - s)*(dface->v3[coy] - dface->v2[coy]) -
			(dface->v3[coy] - t)*(dface->v3[cox] - dface->v2[cox])< 0.0 ) 
			return 0;
		
		if(dface->v4==0) {
			if( (dface->v1[cox] - s)*(dface->v1[coy] - dface->v3[coy]) -
				(dface->v1[coy] - t)*(dface->v1[cox] - dface->v3[cox])< 0.0 ) 
				return 0;
		}
		else {
			if( (dface->v4[cox] - s)*(dface->v4[coy] - dface->v3[coy]) -
				(dface->v4[coy] - t)*(dface->v4[cox] - dface->v3[cox])< 0.0 ) 
				return 0;

			if( (dface->v1[cox] - s)*(dface->v1[coy] - dface->v4[coy]) -
				(dface->v1[coy] - t)*(dface->v1[cox] - dface->v4[cox])< 0.0 )
				return 0;
		}

		sn->labda= tlab;
		sn->face= dface;
		if(tlab<= DYNA_FH_DIST(dface->ma)) {
			sn->flag= FH_PROP;
		}
		return 1;
	}
	
	return 0;
}


void update_Fheight(Life *lf, float *force)
{
	/* alleen anti-zwaartekracht: pas op met rotaties. Normaal vlak= F_afstoot */
	DFace *dface;
	Material *ma;
	Life *lfs;
	Sector *se;
	Object *propob;
	Snijp sn;
	float *mat, speed[3], alpha, fac, loc2[3], oldloc2[3], loc1[3], no[3];
	int a, b;
	
	lf->contact= 0;
	lf->floor= 0;

	/* is er wel 'n Fh vlak in mesh ? */
	if(lf->sector->dynamesh==0) return;
	
	sn.labda= 32.0;
	sn.face= 0;
	sn.ma= 0;
	sn.flag= 0;
	sn.radius= UNSUREF;
	mat= lf->sector->ob->obmat[0];
	
	speed[0]=speed[1]= 0.0;
	speed[2]= -8.0;
	init_snijp(lf->sector->dynamesh, &sn, lf->loc1, speed);

	dface= lf->sector->dynamesh->dface;
	a= lf->sector->dynamesh->totface;

	while(a--) {
		if(DYNA_FH_DIST(dface->ma)!=0.0) {
			
			/* alleen loodrechte projektie!!! */
			if( intersect_fh(dface, lf->loc1, &sn) ) {
				
				/* geen break: beste fh! */
			}
		}
		
		dface++;
	}

	/* of mogelijk de props? */
	if(sn.flag==0 && lf->sector->lbuf.tot) {

		se= lf->sector;
		for(b=0; b<se->lbuf.tot; b++) {
			propob= se->lbuf.ob[b];
			
			lfs= propob->data;
			
			if(lf!=lfs && lfs->dynamesh) {
				/* is er wel 'n Fh vlak in mesh ? */
				
				/* transformeren naar life coordinaten */
				ApplyMatrix(propob->imat, lf->loc, loc1);
				
				dface= lfs->dynamesh->dface;
				a= lfs->dynamesh->totface;
				speed[0]= -8.0*propob->obmat[2][0];
				speed[1]= -8.0*propob->obmat[2][1];
				speed[2]= -8.0*propob->obmat[2][2];
				
				init_snijp(lfs->dynamesh, &sn, loc1, speed);
					
				speed[0]= propob->obmat[2][0];
				speed[1]= propob->obmat[2][1];
				speed[2]= propob->obmat[2][2];

				while(a--) {
					if(DYNA_FH_DIST(dface->ma)!=0.0) {
						
						/* loodrechte projektie */
						if( intersect_fh_rot(dface, loc1, &sn, speed) ) {
							
							/* normaal roteren volgens life */
							mat= propob->obmat[0];
							
							if(sn.flag == FH_PROP) break;
						}
					}
					
					dface++;
				}
				if(sn.flag == FH_PROP) break;
			}
		}
	}
	
	if(sn.face) {
		
		if(sn.face->ma->mode & DYNA_FH_NORMAL) {
			/* normaal roteren naar world coords: 'wipeout' wandjes, duwen in richting normaal vlak */
			no[0]= sn.face->no[0]*mat[0] + sn.face->no[1]*mat[4] + sn.face->no[2]*mat[8];
			no[1]= sn.face->no[0]*mat[1] + sn.face->no[1]*mat[5] + sn.face->no[2]*mat[9];
			no[2]= sn.face->no[0]*mat[2] + sn.face->no[1]*mat[6] + sn.face->no[2]*mat[10];
		}
		else {
			/* alleen loodrecht */
			no[0]= mat[8];
			no[1]= mat[9];
			no[2]= mat[10];
		}
		
		/* floor locatie voor schaduw: altijd relatief! */
		fac= -1.0*sn.labda;
		lf->floorloc[0]= 0.05*lf->axsize+fac*no[0];
		lf->floorloc[1]= 0.05*lf->axsize+fac*no[1];
		lf->floorloc[2]= 0.05*lf->axsize+fac*no[2];

		lf->floor= sn.face;
		
		if(sn.flag) {

			ma= sn.face->ma;
			lf->contact= ma;
		
			/* sn.labda==0.0: exact op afstand dist */
			sn.labda= 1.0 - sn.labda/DYNA_FH_DIST(ma);
			
			/* let op: 0.1 is ook verwerkt in de writepsx */
			sn.labda*= 0.1*DYNA_FH_INT(ma);
			
			force[0]+= sn.labda*no[0];
			force[1]+= sn.labda*no[1];
			force[2]+= sn.labda*no[2];
	
			/* extra fh friction, alleen in richting normaal */
			alpha= lf->speed[0]*no[0] + lf->speed[1]*no[1] + lf->speed[2]*no[2] ;
			alpha*= DYNA_FH_FRICT(ma);
			
			lf->speed[0] -= alpha*no[0];
			lf->speed[1] -= alpha*no[1];
			lf->speed[2] -= alpha*no[2];
			
			if( (sn.flag==FH_PROP) && (lfs->dflag & LF_DYNACHANGED) && lfs->type==LF_PROP) {
								
				/* werken aan de hand van de 'virtuele' vorige positie van life */
				ApplyMatrix(propob->imat, lf->loc, loc2);
				ApplyMatrix(lfs->oldimat, lf->loc, oldloc2);
	
				no[0]= oldloc2[0]-loc2[0];
				no[1]= oldloc2[1]-loc2[1];
				no[2]= oldloc2[2]-loc2[2];
				
				/* terug transformeren */
				Mat4Mul3Vecfl(propob->obmat, no);
				
				/* dit is de 'stilstaande' speed */
				fac= (DYNA_FH_XYFRICT(ma));
				fac*= lf->frictfac;
				alpha= 1.0-fac;
	
				lf->speed[0]= alpha*lf->speed[0] + fac*no[0];
				lf->speed[1]= alpha*lf->speed[1] + fac*no[1];
				lf->speed[2]= alpha*lf->speed[2] + fac*no[2];
						
			}
			else {
				alpha= 1.0 - lf->frictfac*(DYNA_FH_XYFRICT(ma));
				
				lf->speed[0] *= alpha;
				lf->speed[1] *= alpha;
			}
		}
	}	
}

void aerodynamics_life(Object *ob, Life *lf)
{
	float *q, speed[3], len, inp;
	
	if(lf->aero==0.0) return;
	
	/* the direction vector rotates the speed... */
	/* only X/Y axis now */
	
	if(lf->flag & LF_AERO_AXIS_Y) q= ob->obmat[1];
	else q= ob->obmat[0];
	
	VECCOPY(speed, lf->speed);
	
	len= Normalise(speed);
	if(len > TOLER) {
		inp= q[0]*speed[0] + q[1]*speed[1] + q[2]*speed[2] ;
		
		inp*= lf->aero;
		
		lf->speed[0]= inp*len*q[0] + (1.0-fabs(inp))*lf->speed[0];
		lf->speed[1]= inp*len*q[1] + (1.0-fabs(inp))*lf->speed[1];
		/* keep the z component? */
		/* lf->speed[2]= inp*len*q[2] + (1.0-fabs(inp))*lf->speed[2]; */
	}
}

void update_dynamics(Object *ob)
{
	Life *lf;
	float frict, force[3], omega[3];
	short event;
	
	if(ob->type==OB_CAMERA) {
		where_is_object_simul(ob);
		return;
	}
	
	lf= ob->data;
	
	if(lf->type==LF_DYNAMIC) {
		
		aerodynamics_life(ob, lf);
		
		/* zwaartekracht */
		force[0]= force[1]= 0.0;
		force[2]= -DTIME*G.scene->grav*lf->mass;
		omega[0]=omega[1]=omega[2]= 0.0;		
		
		/* Fh: contact/schaduw. VOOR de sensor-input afhandelen: kan je de wrijving overwinnen */
		if(lf->sector && (lf->flag & LF_DO_FH)) update_Fheight(lf, force);

		/* toetsen */
		if(lf->flag & LF_SENSORS) sensor_input(ob, lf, force, omega);

		lf->speed[0]+= force[0];
		lf->speed[1]+= force[1];
		lf->speed[2]+= force[2];

		lf->rotspeed[0]+= omega[0];
		lf->rotspeed[1]+= omega[1];
		lf->rotspeed[2]+= omega[2];
		
		/* wrijving */
		if(lf->frict!=0.0) {
		
			frict= 1.0-lf->frict*lf->frictfac;
			
			lf->speed[0]*= frict;
			lf->speed[1]*= frict;
			lf->speed[2]*= frict;
		}
		if(lf->rotfrict!=0.0) {
			frict= 1.0-lf->rotfrict;
			lf->rotspeed[0]*= frict;
			lf->rotspeed[1]*= frict;
			lf->rotspeed[2]*= frict;
		}
		
		VECCOPY(lf->oldloc, lf->loc);
		
		lf->loc[0]+= lf->speed[0];
		lf->loc[1]+= lf->speed[1];
		lf->loc[2]+= lf->speed[2];
		
		lf->rot[0]+= lf->rotspeed[0];
		lf->rot[1]+= lf->rotspeed[1];
		lf->rot[2]+= lf->rotspeed[2];
	
		life_to_local_sector_co(lf);

		if(lf->sector) collision_detect(ob, lf);
		if(lf->collision) collision_sensor_input(ob, lf);

		/* floorloc is berekend t.o.v. oldloc */
		lf->floorloc[2]-= lf->speed[2];

		VECCOPY(ob->loc, lf->loc);
		VECCOPY(ob->rot, lf->rot);
		where_is_object_simul(ob);
		Mat4InvertSimp(ob->imat, ob->obmat);
	}
	else {

		if(lf->type==LF_LINK) {

			/* toetsen */
			if(lf->flag & LF_SENSORS) {
		
				if(lf->dflag & LF_OMEGA) {
					
					omega[0]=omega[1]=omega[2]= 0.0;		
					sensor_input(ob, lf, force, omega);	
				
					lf->rotspeed[0]+= omega[0];
					lf->rotspeed[1]+= omega[1];
					lf->rotspeed[2]+= omega[2];
					
					if(lf->rotfrict!=0.0) {
						frict= 1.0-lf->rotfrict;
						lf->rotspeed[0]*= frict;
						lf->rotspeed[1]*= frict;
						lf->rotspeed[2]*= frict;
					}
	
					lf->rot[0]+= lf->rotspeed[0];
					lf->rot[1]+= lf->rotspeed[1];
					lf->rot[2]+= lf->rotspeed[2];
					
					VECCOPY(ob->rot, lf->rot);
				}
				else {
					sensor_input(ob, lf, force, omega);
				}
			}
			/* link: altijd where_is doen */
			where_is_object_simul(ob);
			Mat4InvertSimp(ob->imat, ob->obmat);
		}
		else {

			/* toetsen */
			if(lf->flag & LF_SENSORS) {

				/* force en omega worden niet gebruikt */
				if(event= sensor_input(ob, lf, force, omega) ) {
					if(event & SN_ROTCHANGED) {
						VECCOPY(ob->rot, lf->rot);
					}
					Mat4CpyMat4(lf->oldimat, ob->imat);
					where_is_object_simul(ob);
					Mat4InvertSimp(ob->imat, ob->obmat);
				}
				else {
					lf->dflag &= ~LF_DYNACHANGED;
					if(ob->parent) where_is_object_simul(ob);
				}
			}
			lf->collision= 0;	/* is mogelijk door dynalife gezet */
		}
		
	}	
}


/* ************* REALTIME ************** */

#define SE_MAXBUF	32
#define LF_MAXBUF	32

void view_to_piramat(float mat[][4], float lens, float far)
{
	/* maakt viewmat piramidevormig voor eenvoudige clip */
	
	lens/= 12.0;	/* hier stond 16.0 */
	mat[0][0]*= (256.0/320.0)*lens;
	mat[1][0]*= (256.0/320.0)*lens;
	mat[2][0]*= (256.0/320.0)*lens;
	mat[3][0]*= (256.0/320.0)*lens;
	
	mat[0][1]*= lens;
	mat[1][1]*= lens;
	mat[2][1]*= lens;
	mat[3][1]*= lens;
	
	mat[0][2]*= -1;
	mat[1][2]*= -1;
	mat[2][2]*= -1;
	mat[3][2]*= -1;
	
	mat[3][3]= far;		/* cliptest_sector leest dit uit */
}

void init_sectors()
{
	extern Material defmaterial;
	Base *base;
	Mesh *me;
	Portal *po;
	Sector *se;
	Life *lf;
	int a, printerr=0;
	
	G.cursector= 0;
	G.totsect= 0;
	
	/* vlaggen resetten */
	me= G.main->mesh.first;
	while(me) {
		me->flag &= ~ME_ISDONE;
		me= me->id.next;
	}

	/*  dit zijn globale INT tellers (gaat 10000 uren mee) */
	G.dfra= G.dfrao= 1;
			
	init_dyna_material(&defmaterial);

	base= FIRSTBASE;
	while(base) {
		
		base->object->dfras= 0;
		
		/* geen layertest meer */
		if(base->object->type==OB_SECTOR) {
			
			se= base->object->data;
			se->ob= base->object;
			
			se->lbuf.tot= se->lbuf.max= 0;
			se->lbuf.ob= 0;
			
			/* if(se->texmesh) se->texmesh->lastfra= 0; */
			
			where_is_object_simul(base->object);
			Mat4Invert(base->object->imat, base->object->obmat);
			
			VECCOPY(se->bbsize, se->size);
			Mat4Mul3Vecfl(base->object->obmat, se->bbsize);
			
			portal_detect(base->object);
			
			if(se->dynamesh) {
			
				/* tijdelijke patch */
				if(se->dynamesh->tface) {
					freeN(se->dynamesh->tface);
					se->dynamesh->tface= 0;
					PRINT(s, se->dynamesh->id.name+2);
					printerr= 1;
				}
			
				if(se->dynamesh->flag & ME_ISDONE);
				else {
					init_dynamesh(base->object, se->dynamesh);
					se->dynamesh->flag |= ME_ISDONE;
				}				
			}
		}
		base= base->next;
	}
	
	if(printerr) error("dynamesh had tfaces");
	
	/* portals testen: sectors die niet zijn gedaan wissen */
	/* en de portal->ob pointers (optim?) */
	base= FIRSTBASE;
	while(base) {
		
		if(base->object->type==OB_SECTOR) {
			se= base->object->data;
			po= se->portals;
			a= se->totport;
			while(a--) {
				if(po->sector) {
					if(po->sector->dynamesh->flag & ME_ISDONE) {
						po->ob= po->sector->ob;
					}
					else po->sector= 0;
				}
				po++;
			}
		}
		
		base= base->next;
	}	
}

void end_sectors()
{
	Base *base;
	Sector *se;
	
	base= FIRSTBASE;
	while(base) {
		
		if(base->object->type==OB_SECTOR) {
			
			se= base->object->data;
			se->ob= base->object;
			
			end_dynamesh(se->dynamesh);
			
			free_lbuf(&se->lbuf);
		}
		
		base= base->next;
	}

	G.cursector= 0;
	
}


void add_dyna_life(Object *ob)		/* uitzondering: camera zit er ook in */
{
	int a, *ip=0;
	
	if(G.totlife<LF_MAXBUF) {
		G.lifebuf[G.totlife]= ob;
		G.totlife++;
	}
}

void add_dupli_life(Object *ob, Object *from, int time)
{
	Object *newob, *par;
	Life *lfn, *lf;
	Sensor *sn;
	short a;
	
	if(G.totlife<LF_MAXBUF) {
		newob= dupallocN(ob);

		newob->lay= from->lay;
		
		lf= from->data;

		if(from->parent) {
			
			/* zeer primitieve rotatie ! */
			
			VECCOPY(newob->rot, from->rot);
			par= from->parent;
			while(par) {
				newob->rot[0]+= par->rot[0];
				newob->rot[1]+= par->rot[1];
				newob->rot[2]+= par->rot[2];
				par= par->parent;
			}
			
			VECCOPY(newob->loc, from->obmat[3]);
			
		}
		else if(lf->type==LF_DYNAMIC && lf->collision) {
			VECCOPY(newob->loc, lf->colloc);
			VECCOPY(newob->rot, from->rot);
		}
		else {
			VECCOPY(newob->loc, from->loc);
			VECCOPY(newob->rot, from->rot);
		}
		
		where_is_object_simul(newob);
		add_dyna_life(newob);

		newob->data=lfn= dupallocN(ob->data);

		
		VECCOPY(lfn->loc, newob->loc);
		VECCOPY(lfn->rot, newob->rot);
		
		lfn->sensors= dupallocN(lfn->sensors);
		for(a=lfn->totsens, sn=lfn->sensors; a>0; a--, sn++) {
			sn->events= dupallocN(sn->events);
			sn->actions= dupallocN(sn->actions);
		}
		
		lfn->dflag |= LF_TEMPLIFE;
		lfn->timer= time;
		lfn->collision= 0;
		while(from->parent) from= from->parent;
		lfn->from= from;
		if(from->type==OB_LIFE) lfn->sector= ((Life *)from->data)->sector;
	}
}

void del_dupli_life(Object *ob)
{
	Life *lf;
	Sensor *sn;
	int a, b;
	
	/* opzoeken in array */
	a= G.totlife;
	while(a--) {
		if(G.lifebuf[a]==ob) {
			if(ob->type==OB_LIFE) {
				lf= ob->data;
				if(lf->dflag & LF_TEMPLIFE) {
				
					for(b=lf->totsens, sn=lf->sensors; b>0; b--, sn++) {
						freeN(sn->events);
						freeN(sn->actions);
					}
					freeN(lf->sensors);
					freeN(lf);
					ob->data= 0;
					freeN(ob);
					G.totlife--;
					G.lifebuf[a]= G.lifebuf[G.totlife];
				}
			}
			return;
		}
	}
}

void del_dupli_lifes()
{
	int a;
	
	a= G.totlife;
	while(a--) {
		del_dupli_life(G.lifebuf[a]);
	}
}

char clipcube[24]= {0, 1, 1,
					1, 0, 1, 
					0, 0, 0, 
					1, 1, 0, 
					1, 1, 1, 
					0, 0, 1, 
					1, 0, 0,
					0, 1, 0}; 

int cliptest_sector(float *vec, float *size, float *mat, short round)
{
	/* deze pakt de vier tetraederpunten */
	/* interessante notitie: max= fabs(hoco[2]) zou je denken,
	 * fabs is echter veel te weinig kritisch. Op deze wijze (met sign)
	 * meer uitval achter de camera,  maar wel OK!
	 */
	float hoco[3], min, max, far=mat[15];
	short fl, fand, a, tot;
	char *ctab;

	/* nu de vraag hoeveel zin dit nog heeft? */
	if(round<=3) tot= 8; else tot= 4;
	ctab= clipcube;
	fand= 63;
	
	while(tot--) {
	
		if(ctab[0]) hoco[0]= vec[0] - size[0];
		else hoco[0]= vec[0] + size[0];
		if(ctab[1]) hoco[1]= vec[1] - size[1];
		else hoco[1]= vec[1] + size[1];
		if(ctab[2]) hoco[2]= vec[2] - size[2];
		else hoco[2]= vec[2] + size[2];
		
		Mat4MulVecfl(mat, hoco);
		max= (hoco[2]);
		min= -max;
		fl= 0;
		if(hoco[0] < min) fl+= 1; else if(hoco[0] > max) fl+= 2;
		if(hoco[1] < min) fl+= 4; else if(hoco[1] > max) fl+= 8;
		if(hoco[2] < 0.0) fl+= 16; else if(hoco[2] > far) fl+= 32;
		
		fand &= fl;
		if(fand==0) return 1;

		ctab+= 3;
	}

	return 0;
}

void build_sectorlist(Object *cam)
{
	Portal *po;
	Object *ob;
	Camera *ca;
	Base *base;
	Sector *se;
	float far, lens, piramat[4][4], seview[3], secentre[3];
	short a, b, round, beforesect, startsect;

	float viewfac;
	
	if(G.scene->camera==0) {
		lens= 35.0;
		far= 16.0;
	}
	else {
		ca= G.scene->camera->data;
		lens= ca->lens;
		far= ca->clipend;
	}
	
	startsect= 0;
	G.totsect= 0;
	G.maxsect= G.scene->maxdrawsector;
	
	/* uitzondering afhandelen */ 
	if(G.cursector==0 || cam==0) {
		base= FIRSTBASE;
		while(base) {
			if(base->lay & G.scene->lay) {
				if(base->object->type==OB_SECTOR) {			
					G.sectorbuf[G.totsect]= base->object->data;
					G.totsect++;
					if(G.totsect>=G.maxsect) break;
				}
			}
			base= base->next;
		}
		return;
	}

	G.sectorbuf[0]= G.cursector;
	G.cursector->ob->dfras= G.dfras;
	G.cursector->depth= 0;
	G.totsect= 1;
	
	Mat4Ortho(cam->obmat);
	Mat4Invert(cam->imat, cam->obmat);	/* viewmat */
	Mat4CpyMat4(piramat, cam->imat);
	view_to_piramat(piramat, lens, far);
	
	round= 1;
	
	while(G.totsect<G.maxsect) {
		beforesect= G.totsect;
		
		/* alle sectoren van startsect tot G.totsect aflopen op portals */
		for(b=G.totsect-1; b>=startsect; b--) {
			se= G.sectorbuf[b];

			a= se->totport;
			po= se->portals;
			while(a--) {
				if(po->sector && po->ob->dfras!=G.dfras) {
					po->ob->dfras= G.dfras;
					
					ob= po->ob;

					if(ob->lay & G.scene->lay) {
					
						/* in beeld */
						if(cliptest_sector(ob->obmat[3], po->sector->bbsize, piramat[0], round)) {
							G.sectorbuf[G.totsect]= po->sector;	
							po->sector->depth= round;
							G.totsect++;
							if(G.totsect>=G.maxsect) break;
						}
					}
				}
				
				po++;
			}
			if(G.totsect>=G.maxsect) break;
		}
		
		round++;
		
		/* geen meer bijgekomen */
		if(G.totsect==beforesect) return;
		startsect= beforesect;
	}
}


/* ************* REALTIME mainlus ************** */
	
	/* swapbuffers_OT() */

/* ************* MAIN ************** */

void update_sector_lifes(Sector *se)
{
	Object *ob;
	Life *lf;
	short a, b;

	if(se==0) return;
	
	for(b=0; b<se->lbuf.tot; b++) {
		ob= se->lbuf.ob[b];
		if(ob->lay & G.scene->lay) {
		
			/* lifes zitten in meerdere sectoren of sector 2x afgehandeld */
		
			if(ob->dfras!=G.dfras) {
				ob->dfras= G.dfras;
				
				lf= ob->data;
				/* beweegbare props: ook in lifebuf ivm sector wissel */
				if(lf->type==LF_DYNAMIC) {
					add_dyna_life(ob);
				}
				else {
					update_dynamics(ob);
				}
			}
		}
	}	
}


void update_lifes()
{
	Object *ob;
	Life *lf;
	Portal *po;
	int a, b;
	
	/* cleanup lifebuf  */
	for(a=0; a<G.totlife; a++) {
		ob= G.lifebuf[a];
		if(ob->type==OB_LIFE) {

			lf= ob->data;
			if( (lf->flag & LF_MAINACTOR)==0) {
				if( (lf->dflag & LF_TEMPLIFE)==0 ) {
					G.totlife--;
					G.lifebuf[a]= G.lifebuf[G.totlife];
					a--;
				}
			}
		}
	}
	
	a= G.totsect;
	while(a-- > 0) {
		update_sector_lifes(G.sectorbuf[a]);
	}
	
	if(G.cursector) {
		a= G.cursector->totport;
		po= G.cursector->portals;
		while(a--) {
			update_sector_lifes(po->sector);
			po++;
		}
	}	
	
	/* alle main+link en temp lifes: in volgorde ivm parents! */
	for(a=0; a<G.totlife; a++) {
		ob= G.lifebuf[a];
		if(ob->type==OB_LIFE) {

			life_in_sector_test(ob);

			update_dynamics(ob);
			
			lf= ob->data;
			for(b=0; b<lf->links.tot; b++) update_dynamics(lf->links.ob[b]);
		}
		else where_is_object_simul(ob);		/*	camera */
	}

	/* apart doen ivm delete */
	a= G.totlife;
	while(a--) {
		ob= G.lifebuf[a];
		if(ob->type==OB_LIFE) {
			lf= ob->data;
			if(lf->dflag & LF_TEMPLIFE) {
				if(lf->timer<=0 || lf->sector==0) {
					del_dupli_life(ob);
				}
			}		
		}
	}

}

void update_realtime_textures()
{
	/* wordt aangeroepen met constante */
	Image *ima;
	int a;
	
	ima= G.main->image.first;
	while(ima) {
		if(ima->tpageflag & IMA_TWINANIM) {
			if(ima->twend >= ima->xrep*ima->yrep) ima->twend= ima->xrep*ima->yrep-1;
		
			/* check: zit de bindcode niet het array? Vrijgeven. (nog doen) */
			
			ima->lastframe++;
			if(ima->lastframe > ima->twend) ima->lastframe= ima->twsta;
			
		}
		ima= ima->id.next;
	}
}

void sector_go()
{
	extern double tottime;	/* drawview.c */
	extern int dbswaptime;	/* global in life.c, voor print */
	extern double speed_to_swaptime(int); 	/* drawview.c */
	extern int update_time();
	struct tms voidbuf;
	double swaptime;
	int time, ltime, dtime;
	int a, b;
	ushort event=0;
	short val;
	

	/* per life: sector test
	 *			 sensors
	 *			 damage
	 *			 update dynamics <- -> collision detectie
	 *			 
	 */
	
		/* doet ook mesh_isdone flag */
	init_sectors();
	init_lifes();
	/* init_camera_network(); */	/* this one gives strange errors! */
	init_devs();

	update_time();
	tottime= 0.0;
	swaptime= speed_to_swaptime(G.animspeed);
	#ifdef __WIN32
	ltime = times(&voidbuf)/10;
	#else
		#ifdef __BeOS
		ltime= (glut_system_time())/10000;
		#else
		ltime = times(&voidbuf);
		#endif
	#endif
	dtime= 4;	/* vantevoren invullen (1/25 sec) */	

	
	sectcount=spherecount=cylcount=facecount=matcount= 0;	
	
	
	while(TRUE) {
		/* dynamics lus: stapjes van 0.02 seconden */
		
		G.dfrao= G.dfra;
		G.fields=0;
		
		set_dtime(dtime);	/* global in object.c, ook voor cameranet */
		dbswaptime= dtime;	

		if(dtime>25) dtime= 25;
		
		while(dtime>0) {
			G.dfra++;
			G.dfras= G.dfra;

			event= pad_read();
			if(event==SPACEKEY || event==ESCKEY) {
				G.simulf |= G_QUIT;
				break;
			}
		
			update_lifes();
			
			if(G.dfra & 1) update_realtime_textures();
			
			dtime -= 2;
			G.fields++;
			sectcount++;
		
		}
 
		if(G.simulf & G_NETWORK) {
			evaluate_camera_network(0, 0);
		}
		else {
			mainactor_in_sector_test();
		}
		
		build_sectorlist(G.scene->camera);
	
		drawview3d_simul(0);

		screen_swapbuffers();
		
		/* tijdberekening: in hondersten van seconde */		
		while(dtime<2) {
			#ifdef __WIN32
			time = times(&voidbuf)/10;
			#else
				#ifdef __BeOS
				time= (glut_system_time())/10000;
				#else
				time = times(&voidbuf);
				#endif
			#endif

			dtime+= (time - ltime);
			ltime= time;

			if(dtime<2) usleep(1);
		}

		if(G.simulf & (G_RESTART+G_QUIT)) break;
		if(G.simulf & G_LOADFILE) break;
	}

	/* end_camera_network(); */
	end_sectors();
	end_lifes(event!=SPACEKEY);
	
	set_dtime(2);	/* restore */
	G.qual= 0;
	
	if(G.simulf & G_LOADFILE) {
		char str[64];
		/* two levels of load events. first we must make sure we leave the simul, then we can add to the queue */
		sprintf(str, "%s.blend", simul_load_str);
		add_readfile_event(str);
	}
	
	/* PRINT4(d, d, d, d, sectcount, facecount, cylcount, spherecount); */
	/* PRINT(d, matcount); */

}

#endif

