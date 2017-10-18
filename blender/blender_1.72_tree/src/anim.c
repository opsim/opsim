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


/*  anim.c      MIXED MODEL
 * 
 *  juli 95
 *  
 * 
 */

#include "blender.h"
#include "ipo.h"
#include "effect.h"

ListBase duplilist= {0, 0}; 

void free_path(Path *path)
{
	if(path->data) freeN(path->data);
	freeN(path);
}


void calc_curvepath(Object *ob)
{
	BevList *bl;
	BevPoint *bevp, *bevpn, *bevpfirst, *bevplast;
	Curve *cu;
	Nurb *nu;
	Path *path;
	float *fp, *f1, *dist, *maxdist, *maxverts, x, y, z;
	float fac, d=0, fac1, fac2;
	float *temp, ctime;
	int a, tot, cycl=0;
	float *ft, *maxdata;
	
	/* in een pad zitten allemaal punten met gelijke afstand: path->len = aantal pt */
	/* NU MET BEVELCURVE!!! */
	
	if(ob==0 || ob->type != OB_CURVE) return;
	cu= ob->data;
	if(ob==G.obedit) nu= editNurb.first;
	else nu= cu->nurb.first;
	
	if(cu->path) free_path(cu->path);
	cu->path= 0;
	
	if((cu->flag & CU_PATH)==0) return;
	
	bl= cu->bev.first;
	if(bl==0) {
		makeDispList(ob);
		bl= cu->bev.first;
	}
	if(bl==0) return;

	cu->path=path= callocN(sizeof(Path), "path");
	
	/* als POLY: laatste punt != eerste punt */
	cycl= (bl->poly!= -1);
	
	if(cycl) tot= bl->nr;
	else tot= bl->nr-1;
	
	path->len= tot+1;
	/* exception: vector handle paths and polygon paths should be subdivided at least a factor 6 (or more?) */
	if(path->len<6*nu->pntsu) path->len= 6*nu->pntsu;
	
	dist= (float *)mallocN((tot+1)*4, "berekenpaddist");

		/* alle lengtes in *dist */
	bevp= bevpfirst= (BevPoint *)(bl+1);
	fp= dist;
	*fp= 0;
	for(a=0; a<tot; a++) {
		fp++;
		if(cycl && a==tot-1) {
			x= bevpfirst->x - bevp->x;
			y= bevpfirst->y - bevp->y;
			z= bevpfirst->z - bevp->z;
		}
		else {
			x= (bevp+1)->x - bevp->x;
			y= (bevp+1)->y - bevp->y;
			z= (bevp+1)->z - bevp->z;
		}
		*fp= *(fp-1)+ fsqrt(x*x+y*y+z*z);
		
		bevp++;
	}
	
	path->totdist= *fp;

		/* de padpunten in path->data */
		/* nu ook met TILT */
	ft= path->data = (float *)callocN(16*path->len, "pathdata");
	
	bevp= bevpfirst;
	bevpn= bevp+1;
	bevplast= bevpfirst + (bl->nr-1);
	fp= dist+1;
	maxdist= dist+tot;
	fac= 1.0/((float)path->len-1);

	for(a=0; a<path->len; a++) {
		
		d= ((float)a)*fac*path->totdist;
		
		/* we zoeken plek 'd' in het array */
		while((d>= *fp) && fp<maxdist) {
			fp++;
			if(bevp<bevplast) bevp++;
			bevpn= bevp+1;
			if(bevpn>bevplast) {
				if(cycl) bevpn= bevpfirst;
				else bevpn= bevplast;
			}
		}
		
		fac1= *(fp)- *(fp-1);
		fac2= *(fp)-d;
		fac1= fac2/fac1;
		fac2= 1.0-fac1;

		ft[0]= fac1*bevp->x+ fac2*(bevpn)->x;
		ft[1]= fac1*bevp->y+ fac2*(bevpn)->y;
		ft[2]= fac1*bevp->z+ fac2*(bevpn)->z;
		ft[3]= fac1*bevp->alfa+ fac2*(bevpn)->alfa;
		
		ft+= 4;

	}
	
	freeN(dist);
}

int interval_test(int min, int max, int p1, int cycl)
{
	
	if(cycl) {
		if( p1 < min) 
			p1=  ((p1 -min) % (max-min+1)) + max+1;
		else if(p1 > max)
			p1=  ((p1 -min) % (max-min+1)) + min;
	}
	else {
		if(p1 < min) p1= min;
		else if(p1 > max) p1= max;
	}
	return p1;
}

int where_on_path(Object *ob, float ctime, float *vec, float *dir)	/* geeft OK terug */
{
	Curve *cu;
	Nurb *nu;
	BevList *bl;
	Path *path;
	float dirvec[3], *fp, *p0, *p1, *p2, *p3, fac, facm;
	float data[4];
	int cycl=0, s0, s1, s2, s3;

	
	if(ob==0 || ob->type != OB_CURVE) return 0;
	cu= ob->data;
	if(cu->path==0 || cu->path->data==0) calc_curvepath(ob);
	path= cu->path;
	fp= path->data;
	
	/* cyclic testen */
	bl= cu->bev.first;
	if(bl && bl->poly> -1) cycl= 1;

	/* ctime is van 0.0-1.0 */
	ctime *= (path->len-1);
	
	s1= ffloor(ctime);
	fac= (float)(s1+1)-ctime;

	/* path->len is gecorrigeerd voor cyclic, zie boven, is beetje warrig! */
	s0= interval_test(0, path->len-1-cycl, s1-1, cycl);
	s1= interval_test(0, path->len-1-cycl, s1, cycl);
	s2= interval_test(0, path->len-1-cycl, s1+1, cycl);
	s3= interval_test(0, path->len-1-cycl, s1+2, cycl);

	p0= fp + 4*s0;
	p1= fp + 4*s1;
	p2= fp + 4*s2;
	p3= fp + 4*s3;

	if(cu->flag & CU_FOLLOW) {
		
		set_afgeleide_four_ipo(1.0-fac, data, KEY_BSPLINE);
		
		dir[0]= data[0]*p0[0] + data[1]*p1[0] + data[2]*p2[0] + data[3]*p3[0] ;
		dir[1]= data[0]*p0[1] + data[1]*p1[1] + data[2]*p2[1] + data[3]*p3[1] ;
		dir[2]= data[0]*p0[2] + data[1]*p1[2] + data[2]*p2[2] + data[3]*p3[2] ;
		
		/* compatible maken met vectoquat */
		dir[0]= -dir[0];
		dir[1]= -dir[1];
		dir[2]= -dir[2];
	}
	
	nu= cu->nurb.first;

	/* zeker van zijn dat de eerste en laatste frame door de punten gaat */
	if((nu->type & 7)==CU_POLY) set_four_ipo(1.0-fac, data, KEY_LINEAR);
	else if((nu->type & 7)==CU_BEZIER) set_four_ipo(1.0-fac, data, KEY_LINEAR);
	else if(s0==s1 || p2==p3) set_four_ipo(1.0-fac, data, KEY_CARDINAL);
	else set_four_ipo(1.0-fac, data, KEY_BSPLINE);

	vec[0]= data[0]*p0[0] + data[1]*p1[0] + data[2]*p2[0] + data[3]*p3[0] ;
	vec[1]= data[0]*p0[1] + data[1]*p1[1] + data[2]*p2[1] + data[3]*p3[1] ;
	vec[2]= data[0]*p0[2] + data[1]*p1[2] + data[2]*p2[2] + data[3]*p3[2] ;

	vec[3]= data[0]*p0[3] + data[1]*p1[3] + data[2]*p2[3] + data[3]*p3[3] ;

	return 1;
}

void frames_duplilist(Object *ob)
{
	extern int enable_cu_speed;	/* object.c */
	Object *new;
	Base *base;
	int cfrao, ok;
	
	cfrao= CFRA;
	if(ob->parent==0 && ob->track==0 && ob->ipo==0) return;

	if(ob->transflag & OB_DUPLINOSPEED) enable_cu_speed= 0;

	/* dit om zeker van te zijn dat er iets gezbufferd wordt: in drawobject.c: dt==wire en boundboxclip */
	if(G.background==0 && ob->type==OB_MESH) {
		Mesh *me= ob->data;
		DispList *dl;
		if(me->disp.first==0) addnormalsDispList(ob, &me->disp);
		if(ob->dt==OB_SHADED) {
			dl= ob->disp.first;
			if(dl==0 || dl->col1==0) shadeDispList(ob);
		}
	}
	
	for(CFRA= ob->dupsta; CFRA<=ob->dupend; CFRA++) {

		ok= 1;
		if(ob->dupoff) {
			ok= CFRA - ob->dupsta;
			ok= ok % (ob->dupon+ob->dupoff);
			if(ok < ob->dupon) ok= 1;
			else ok= 0;
		}
		if(ok) {
			new= mallocN(sizeof(Object), "newobj dupli");
			memcpy(new, ob, sizeof(Object));
			
			/* alleen de basis-ball behoeft een displist */
			if(new->type==OB_MBALL) new->disp.first= new->disp.last= 0;

			addtail(&duplilist, new);
			do_ob_ipo(new);
			where_is_object(new);

			new->flag |= OB_FROMDUPLI;
			new->id.new= (ID *)ob;	/* duplicator bewaren */
		}
	}

	CFRA= cfrao;
	enable_cu_speed= 1;
	do_ob_ipo(ob);
}


void vertex_duplilist(Scene *sce, Object *par)
{
	Object *ob, *new;
	Base *base;
	MVert *mvert;
	Mesh *me;
	float vec[3], pvec[3], dvec[3], qone[4], pmat[4][4], mat[3][3], tmat[4][4];
	float *q2;
	int lay, totvert, a;
	
	qone[0]= 1.0; 
	qone[1]=qone[2]=qone[3]= 0.0;

	Mat4CpyMat4(pmat, par->obmat);
	
	Mat4One(tmat);
	
	lay= G.scene->lay;
	if(G.vd) lay= G.vd->lay;
	
	base= sce->base.first;
	while(base) {

		if(base->object->type>0 && (lay & base->lay) && G.obedit!=base->object) {
			ob= base->object->parent;
			while(ob) {
				if(ob==par) {
				
					ob= base->object;
					/* mballs have a different dupli handling */
					if(ob->type!=OB_MBALL) ob->flag |= OB_DONE;	/* doesnt render */
					
					me= par->data;
					mvert= me->mvert;
					mvert+= (me->totvert-1);
					VECCOPY(pvec, mvert->co);
					Mat4MulVecfl(pmat, pvec);

					mvert= me->mvert;
					totvert= me->totvert;

					for(a=0; a<totvert; a++, mvert++) {
					
						/* bereken de extra offset (tov. nulpunt parent) die de kinderen krijgen */
						VECCOPY(vec, mvert->co);
						Mat4MulVecfl(pmat, vec);
						VecSubf(vec, vec, pmat[3]);
						VecAddf(vec, vec, ob->obmat[3]);
						
						new= mallocN(sizeof(Object), "newobj dupli");
						memcpy(new, ob, sizeof(Object));
						new->flag |= OB_FROMDUPLI;
						new->id.new= (ID *)par;	/* duplicator bewaren */
						
						/* alleen de basis-ball behoeft een displist */
						if(new->type==OB_MBALL) new->disp.first= new->disp.last= 0;
						
						VECCOPY(new->obmat[3], vec);
						
						if(par->transflag & OB_DUPLIROT) {
							VECCOPY(vec, mvert->no);
							vec[0]= -vec[0]; vec[1]= -vec[1]; vec[2]= -vec[2];
							
							q2= vectoquat(vec, ob->trackflag, ob->upflag);
				
							QuatToMat3(q2, mat);
							Mat4CpyMat4(tmat, new->obmat);
							Mat4MulMat43(new->obmat, tmat, mat);
						}
						
						new->parent= 0;
						new->track= 0;
						/* new->borig= base; */
						
						addtail(&duplilist, new);
						
						VECCOPY(pvec, vec);
					
					}
					break;
				}
				ob= ob->parent;
			}
		}
		base= base->next;
	}
}


void particle_duplilist(Scene *sce, Object *par, PartEff *paf)
{
	Object *ob, *new;
	Base *base;
	MVert *mvert;
	Mesh *me;
	Particle *pa;
	float ctime, vec1[3];
	float vec[3], tmat[4][4], mat[3][3];
	float *q2;
	int lay, totvert, a;
	
	pa= paf->keys;
	if(pa==0) {
		build_particle_system(par);
		pa= paf->keys;
		if(pa==0) return;
	}
	
	ctime= bsystem_time(par, 0, (float)CFRA, 0.0);

	lay= G.scene->lay;
	if(G.vd) lay= G.vd->lay;

	base= sce->base.first;
	while(base) {
		
		if(base->object->type>0 && (base->lay & lay) && G.obedit!=base->object) {
			ob= base->object->parent;
			while(ob) {
				if(ob==par) {
				
					ob= base->object;
					
					pa= paf->keys;
					for(a=0; a<paf->totpart; a++, pa+=paf->totkey) {
						
						if(ctime > pa->time) {
							if(ctime < pa->time+pa->lifetime) {
									
								new= mallocN(sizeof(Object), "newobj dupli");
								memcpy(new, ob, sizeof(Object));
								new->flag |= OB_FROMDUPLI;
								new->id.new= (ID *)par;	/* duplicator bewaren */

								/* alleen de basis-ball behoeft een displist */
								if(new->type==OB_MBALL) new->disp.first= new->disp.last= 0;
								
								where_is_particle(paf, pa, ctime, vec);
								if(paf->stype==PAF_VECT) {
									where_is_particle(paf, pa, ctime+1.0, vec1);
									
									VecSubf(vec1, vec1, vec);
									q2= vectoquat(vec1, ob->trackflag, ob->upflag);
						
									QuatToMat3(q2, mat);
									Mat4CpyMat4(tmat, new->obmat);
									Mat4MulMat43(new->obmat, tmat, mat);
								}

								VECCOPY(new->obmat[3], vec);
							
								new->parent= 0;
								new->track= 0;
								
								addtail(&duplilist, new);
								
							}
						}						
					}
					break;
				}
				ob= ob->parent;
			}
		}
		base= base->next;
	}
}


void free_duplilist()
{
	Object *ob;
	
	while( ob= duplilist.first) {
		remlink(&duplilist, ob);
		freeN(ob);
	}
	
}

void make_duplilist(Scene *sce, Object *ob)
{
	PartEff *paf;
	
	if(ob->transflag & OB_DUPLI) {
		if(ob->transflag & OB_DUPLIVERTS) {
			if(ob->type==OB_MESH) {
				if(ob->transflag & OB_DUPLIVERTS) {
					if( paf=give_parteff(ob) ) particle_duplilist(sce, ob, paf);
					else vertex_duplilist(sce, ob);
				}
			}
			else if(ob->type==OB_FONT) {
				font_duplilist(ob);
			}
		}
		else if(ob->transflag & OB_DUPLIFRAMES) frames_duplilist(ob);
	}
}


