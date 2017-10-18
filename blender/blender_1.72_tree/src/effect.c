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



/*  effect.c        MIX MODEL
 * 
 *  dec 95
 * 
 */

#include "blender.h"
#include "render.h"
#include "effect.h"


Effect *add_effect(int type)
{
	Effect *eff=0;
	BuildEff *bld;
	PartEff *paf;
	WaveEff *wav;
	int a;
	
	switch(type) {
	case EFF_BUILD:
		bld= callocN(sizeof(BuildEff), "neweff");
		eff= (Effect *)bld;
		
		bld->sfra= 1.0;
		bld->len= 100.0;
		break;
		
	case EFF_PARTICLE:
		paf= callocN(sizeof(PartEff), "neweff");
		eff= (Effect *)paf;
		
		paf->sta= 1.0;
		paf->end= 100.0;
		paf->lifetime= 50.0;
		for(a=0; a<PAF_MAXMULT; a++) {
			paf->life[a]= 50.0;
			paf->child[a]= 4;
			paf->mat[a]= 1;
		}
		
		paf->totpart= 1000;
		paf->totkey= 8;
		paf->staticstep= 5;
		paf->defvec[2]= 1.0;
		paf->nabla= 0.05;
		
		break;
		
	case EFF_WAVE:
		wav= callocN(sizeof(WaveEff), "neweff");
		eff= (Effect *)wav;
		
		wav->flag |= (WAV_X+WAV_Y+WAV_CYCL);
		
		wav->height= 0.5;
		wav->width= 1.5;
		wav->speed= 0.5;
		wav->narrow= 1.5;
		wav->lifetime= 0.0;
		wav->damp= 10.0;
		
		break;
	}
	
	eff->type= eff->buttype= type;
	eff->flag |= SELECT;
	
	return eff;
}

void free_effect(Effect *eff)
{
	PartEff *paf;
	
	if(eff->type==EFF_PARTICLE) {
		paf= (PartEff *)eff;
		if(paf->keys) freeN(paf->keys);
	}
	freeN(eff);	
}


void free_effects(ListBase *lb)
{
	Effect *eff;
	
	while(eff= lb->first) {
		remlink(lb, eff);
		free_effect(eff);
	}
}

Effect *copy_effect(Effect *eff) 
{
	Effect *effn;
	
	effn= dupallocN(eff);
	if(effn->type==EFF_PARTICLE) ((PartEff *)effn)->keys= 0;

	return effn;	
}

void copy_act_effect(Object *ob)
{
	/* return de aktieve eff gekopieerd */
	Effect *effn, *eff;
	
	eff= ob->effect.first;
	while(eff) {
		if(eff->flag & SELECT) {
			
			effn= copy_effect(eff);
			addtail(&ob->effect, effn);
			
			eff->flag &= ~SELECT;
			return;
			
		}
		eff= eff->next;
	}
	
	/* als tie hier komt: new effect */
	eff= add_effect(EFF_BUILD);
	addtail(&ob->effect, eff);
			
}

void copy_effects(ListBase *lbn, ListBase *lb)
{
	Effect *eff, *effn;
	BuildEff *bld;
	PartEff *paf;

	free_effects(lbn);

	eff= lb->first;
	while(eff) {
		effn= copy_effect(eff);
		addtail(lbn, effn);
		
		eff= eff->next;
	}
	
}

void deselectall_eff(Object *ob)
{
	Effect *eff= ob->effect.first;
	
	while(eff) {
		eff->flag &= ~SELECT;
		eff= eff->next;
	}
}

void set_buildvars(Object *ob, int *start, int *end)
{
	Mesh *me;
	Curve *cu;
	BuildEff *bld;
	float ctime;
	
	bld= ob->effect.first;
	while(bld) {
		if(bld->type==EFF_BUILD) {
			ctime= bsystem_time(ob, 0, (float)CFRA, bld->sfra-1.0);
			if(ctime < 0.0) {
				*end= *start;
			}
			else if(ctime < bld->len) {
				*end= *start+(*end - *start)*ctime/bld->len;
			}
			
			return;
		}
		bld= bld->next;
	}
}

/* ***************** PARTICLES ***************** */

Particle *new_particle(PartEff *paf)
{
	static Particle *pa;
	static int cur;

	/* afspraak: als paf->keys==0: alloc */	
	if(paf->keys==0) {
		pa= paf->keys= callocN( paf->totkey*paf->totpart*sizeof(Particle), "particlekeys" );
		cur= 0;
	}
	else {
		if(cur && cur<paf->totpart) pa+=paf->totkey;
		cur++;
	}
	return pa;
}

PartEff *give_parteff(Object *ob)
{
	PartEff *paf;
	
	paf= ob->effect.first;
	while(paf) {
		if(paf->type==EFF_PARTICLE) return paf;
		paf= paf->next;
	}
	return 0;
}

void where_is_particle(PartEff *paf, Particle *pa, float ctime, float *vec)
{
	Particle *p[4];
	float dt, t[4];
	int a;
	
	if(paf->totkey==1) {
		VECCOPY(vec, pa->co);
		return;
	}
	
	/* eerst op zoek naar de eerste particlekey */
	a= (paf->totkey-1)*(ctime-pa->time)/pa->lifetime;
	if(a>=paf->totkey) a= paf->totkey-1;
	
	pa+= a;
	
	if(a>0) p[0]= pa-1; else p[0]= pa;
	p[1]= pa;
	
	if(a+1<paf->totkey) p[2]= pa+1; else p[2]= pa;
	if(a+2<paf->totkey) p[3]= pa+2; else p[3]= p[2];
	
	if(p[1]==p[2]) dt= 0.0;
	else dt= (ctime-p[1]->time)/(p[2]->time - p[1]->time);

	if(paf->flag & PAF_BSPLINE) set_four_ipo(dt, t, KEY_BSPLINE);
	else set_four_ipo(dt, t, KEY_CARDINAL);

	vec[0]= t[0]*p[0]->co[0] + t[1]*p[1]->co[0] + t[2]*p[2]->co[0] + t[3]*p[3]->co[0];
	vec[1]= t[0]*p[0]->co[1] + t[1]*p[1]->co[1] + t[2]*p[2]->co[1] + t[3]*p[3]->co[1];
	vec[2]= t[0]*p[0]->co[2] + t[1]*p[1]->co[2] + t[2]*p[2]->co[2] + t[3]*p[3]->co[2];

}


void particle_tex(MTex *mtex, PartEff *paf, float *co, float *no)
{				
	extern float Tin, Tr, Tg, Tb;
	float old;
	
	externtex(mtex, co);
	
	if(paf->texmap==PAF_TEXINT) {
		Tin*= paf->texfac;
		no[0]+= Tin*paf->defvec[0];
		no[1]+= Tin*paf->defvec[1];
		no[2]+= Tin*paf->defvec[2];
	}
	else if(paf->texmap==PAF_TEXRGB) {
		no[0]+= (Tr-0.5)*paf->texfac;
		no[1]+= (Tg-0.5)*paf->texfac;
		no[2]+= (Tb-0.5)*paf->texfac;
	}
	else {	/* PAF_TEXGRAD */
		
		old= Tin;
		co[0]+= paf->nabla;
		externtex(mtex, co);
		no[0]+= (old-Tin)*paf->texfac;
		
		co[0]-= paf->nabla;
		co[1]+= paf->nabla;
		externtex(mtex, co);
		no[1]+= (old-Tin)*paf->texfac;
		
		co[1]-= paf->nabla;
		co[2]+= paf->nabla;
		externtex(mtex, co);
		no[2]+= (old-Tin)*paf->texfac;
		
	}
}

void make_particle_keys(int depth, int nr, PartEff *paf, Particle *part, float *force, int deform, MTex *mtex)
{
	Particle *pa, *opa;
	float damp, deltalife;
	int b, rt1, rt2;
	
	damp= 1.0-paf->damp;
	pa= part;
	
	/* startsnelheid: random */
	if(paf->randfac!=0.0) {
		pa->no[0]+= paf->randfac*( drand48() -0.5);
		pa->no[1]+= paf->randfac*( drand48() -0.5);
		pa->no[2]+= paf->randfac*( drand48() -0.5);
	}
	
	/* startsnelheid: texture */
	if(mtex && paf->texfac!=0.0) {
		particle_tex(mtex, paf, pa->co, pa->no);
	}
	
	/* keys */
	if(paf->totkey>1) {
		
		deltalife= pa->lifetime/(paf->totkey-1);
		opa= pa;
		pa++;
		
		b= paf->totkey-1;
		while(b--) {
			/* nieuwe tijd */
			pa->time= opa->time+deltalife;
			
			/* nieuwe plek */
			pa->co[0]= opa->co[0] + deltalife*opa->no[0];
			pa->co[1]= opa->co[1] + deltalife*opa->no[1];
			pa->co[2]= opa->co[2] + deltalife*opa->no[2];
			
			/* nieuwe snelheid */
			pa->no[0]= opa->no[0] + deltalife*force[0];
			pa->no[1]= opa->no[1] + deltalife*force[1];
			pa->no[2]= opa->no[2] + deltalife*force[2];

			/* snelheid: texture */
			if(mtex && paf->texfac!=0.0) {
				particle_tex(mtex, paf, pa->co, pa->no);
			}
			if(damp!=1.0) {
				pa->no[0]*= damp;
				pa->no[1]*= damp;
				pa->no[2]*= damp;
			}
	
			opa= pa;
			pa++;
			/* opa wordt onderin ook gebruikt */
		}
	}

	if(deform) {
		/* alle keys deformen */
		pa= part;
		b= paf->totkey;
		while(b--) {
			calc_latt_deform(pa->co);
			pa++;
		}
	}
	
	/* de grote vermenigvuldiging */
	if(depth<PAF_MAXMULT && paf->mult[depth]!=0.0) {
		
		/* uit gemiddeld 'mult' deel van de particles ontstaan 'child' nieuwe */
		damp = nr;
		rt1= damp*paf->mult[depth];
		rt2= (damp+1.0)*paf->mult[depth];
		if(rt1!=rt2) {
			
			for(b=0; b<paf->child[depth]; b++) {
				pa= new_particle(paf);
				*pa= *opa;
				pa->lifetime= paf->life[depth];
				if(paf->randlife!=0.0) {
					pa->lifetime*= 1.0+ paf->randlife*( drand48() - 0.5);
				}
				pa->mat_nr= paf->mat[depth];
				
				make_particle_keys(depth+1, b, paf, pa, force, deform, mtex);
			}
		}
	}
}

void init_mv_jit(float *jit, int num)
{
	float *jit2, x, rad1, rad2, rad3;
	int i;

	if(num==0) return;

	rad1=  1.0/fsqrt((float)num);
	rad2= 1.0/((float)num);
	rad3= fsqrt((float)num)/((float)num);

	srand48(31415926 + num);
	x= 0;
	for(i=0; i<2*num; i+=2) {
	
		jit[i]= x+ rad1*(0.5-drand48());
		jit[i+1]= ((float)i/2)/num +rad1*(0.5-drand48());
		
		jit[i]-= ffloor(jit[i]);
		jit[i+1]-= ffloor(jit[i+1]);
		
		x+= rad3;
		x -= floor(x);
	}

	jit2= mallocN(12 + 2*sizeof(float)*num, "initjit");

	for (i=0 ; i<4 ; i++) {
		jitterate1(jit, jit2, num, rad1);
		jitterate1(jit, jit2, num, rad1);
		jitterate2(jit, jit2, num, rad2);
	}
	freeN(jit2);
}


void give_mesh_mvert(Mesh *me, int nr, float *co, short *no)
{
	static float *jit=0;
	static int jitlevel=1;
	MVert *mvert;
	MFace *mface;
	float u, v, *v1, *v2, *v3, *v4;
	int curface, curjit;
	short *n1, *n2, *n3, *n4;
	
	/* signal */
	if(me==0) {
		if(jit) freeN(jit);
		jit= 0;
		return;
	}
	
	if(me->totface==0 || nr<me->totvert) {
		mvert= me->mvert + (nr % me->totvert);
		VECCOPY(co, mvert->co);
		VECCOPY(no, mvert->no);
	}
	else {
		
		nr-= me->totvert;
		
		if(jit==0) {
			jitlevel= nr/me->totface;
			if(jitlevel==0) jitlevel= 1;
			if(jitlevel>100) jitlevel= 100;
			
			jit= callocN(2+ jitlevel*2*sizeof(float), "jit");
			init_mv_jit(jit, jitlevel);
			
		}

		curjit= nr/me->totface;
		curjit= curjit % jitlevel;

		curface= nr % me->totface;
		
		mface= me->mface;
		mface+= curface;
		
		v1= (me->mvert+(mface->v1))->co;
		v2= (me->mvert+(mface->v2))->co;
		n1= (me->mvert+(mface->v1))->no;
		n2= (me->mvert+(mface->v2))->no;
		if(mface->v3==0) {
			v3= (me->mvert+(mface->v2))->co;
			v4= (me->mvert+(mface->v1))->co;
			n3= (me->mvert+(mface->v2))->no;
			n4= (me->mvert+(mface->v1))->no;
		}
		else if(mface->v4==0) {
			v3= (me->mvert+(mface->v3))->co;
			v4= (me->mvert+(mface->v1))->co;
			n3= (me->mvert+(mface->v3))->no;
			n4= (me->mvert+(mface->v1))->no;
		}
		else {
			v3= (me->mvert+(mface->v3))->co;
			v4= (me->mvert+(mface->v4))->co;
			n3= (me->mvert+(mface->v3))->no;
			n4= (me->mvert+(mface->v4))->no;
		}

		u= jit[2*curjit];
		v= jit[2*curjit+1];

		co[0]= (1.0-u)*(1.0-v)*v1[0] + (1.0-u)*(v)*v2[0] + (u)*(v)*v3[0] + (u)*(1.0-v)*v4[0];
		co[1]= (1.0-u)*(1.0-v)*v1[1] + (1.0-u)*(v)*v2[1] + (u)*(v)*v3[1] + (u)*(1.0-v)*v4[1];
		co[2]= (1.0-u)*(1.0-v)*v1[2] + (1.0-u)*(v)*v2[2] + (u)*(v)*v3[2] + (u)*(1.0-v)*v4[2];
		
		no[0]= (1.0-u)*(1.0-v)*n1[0] + (1.0-u)*(v)*n2[0] + (u)*(v)*n3[0] + (u)*(1.0-v)*n4[0];
		no[1]= (1.0-u)*(1.0-v)*n1[1] + (1.0-u)*(v)*n2[1] + (u)*(v)*n3[1] + (u)*(1.0-v)*n4[1];
		no[2]= (1.0-u)*(1.0-v)*n1[2] + (1.0-u)*(v)*n2[2] + (u)*(v)*n3[2] + (u)*(1.0-v)*n4[2];
		
	}
}


void build_particle_system(Object *ob)
{
	Object *par;
	PartEff *paf;
	Particle *pa;
	Mesh *me;
	MVert *mvert;
	MTex *mtexcol=0, *mtexmove=0;
	Material *ma;
	float framelenont, ftime, dtime, force[3], imat[3][3], deltalife, vec[3];
	float ofs, fac, prevobmat[4][4], child, sfraont, co[3];
	int deform=0, turbul=1, a, cur, cfraont, cfralast, totpart, delta;
	short *sp, no[3];
	
	if(ob->type!=OB_MESH) return;
	me= ob->data;
	if(me->totvert==0) return;
	
	ma= give_current_material(ob, 1);
	if(ma) {
		mtexmove= ma->mtex[7];
		mtexcol= ma->mtex[0];
	}
	
	paf= give_parteff(ob);
	if(paf==0) return;

	waitcursor(1);
	
	disable_speed_curve(1);
	
	/* alle particles genereren */
	if(paf->keys) freeN(paf->keys);
	paf->keys= 0;
	new_particle(paf);	

	cfraont= CFRA;
	cfralast= -1000;
	framelenont= G.scene->r.framelen;
	G.scene->r.framelen= 1.0;
	sfraont= ob->sf;
	ob->sf= 0.0;
	
	/* mult generaties? */
	totpart= paf->totpart;
	for(a=0; a<PAF_MAXMULT; a++) {
		if(paf->mult[a]!=0.0) {
			/* interessante formule! opdezewijze is na 'x' generaties het totale aantal paf->totpart */
			totpart/= (1.0+paf->mult[a]*paf->child[a]);
		}
		else break;
	}

	ftime= paf->sta;
	dtime= (paf->end - paf->sta)/totpart;
	
	/* hele hiera onthouden */
	par= ob;
	while(par) {
		pushdata(par, sizeof(Object));
		par= par->parent;
	}

	/* alles op eerste frame zetten */
	CFRA= cfralast= ffloor(ftime);
	par= ob;
	while(par) {
		/* do_ob_ipo(par); */
		do_ob_key(par);
		par= par->parent;
	}
	do_mat_ipo(ma);
	
	if((paf->flag & PAF_STATIC)==0) {
		where_is_object(ob);
		Mat4CpyMat4(prevobmat, ob->obmat);
		Mat4Invert(ob->imat, ob->obmat);
		Mat3CpyMat4(imat, ob->imat);
	}
	else {
		Mat4One(prevobmat);
		Mat3One(imat);
	}
	
	srand48(paf->seed);
	
	/* gaat anders veuls te hard */
	force[0]= paf->force[0]*0.05;
	force[1]= paf->force[1]*0.05;
	force[2]= paf->force[2]*0.05;
	
	deform= (ob->parent && ob->parent->type==OB_LATTICE);
	if(deform) init_latt_deform(ob->parent, 0);
	
	/* init */
	give_mesh_mvert(me, totpart, co, no);
	
	for(a=0; a<totpart; a++, ftime+=dtime) {
		
		pa= new_particle(paf);
		pa->time= ftime;
		
		/* ob op juiste tijd zetten */
		
		if((paf->flag & PAF_STATIC)==0) {
		
			cur= ffloor(ftime) + 1 ;		/* + 1 heeft een reden: (obmat/prevobmat) anders beginnen b.v. komeetstaartjes te laat */
			if(cfralast != cur) {
				CFRA= cfralast= cur;
	
				/* later bijgevoegd: blur? */
				bsystem_time(ob, ob->parent, (float)CFRA, 0.0);
				
				par= ob;
				while(par) {
					/* do_ob_ipo(par); */
					par->ctime= -1234567.0;
					do_ob_key(par);
					par= par->parent;
				}
				do_mat_ipo(ma);
				Mat4CpyMat4(prevobmat, ob->obmat);
				where_is_object(ob);
				Mat4Invert(ob->imat, ob->obmat);
				Mat3CpyMat4(imat, ob->imat);
			}
		}
		/* coordinaat ophalen */
		if(paf->flag & PAF_FACE) give_mesh_mvert(me, a, co, no);
		else {
			mvert= me->mvert + (a % me->totvert);
			VECCOPY(co, mvert->co);
			VECCOPY(no, mvert->no);
		}
		
		VECCOPY(pa->co, co);
		
		if(paf->flag & PAF_STATIC);
		else {
			Mat4MulVecfl(ob->obmat, pa->co);
		
			VECCOPY(vec, co);
			Mat4MulVecfl(prevobmat, vec);
			
			/* eerst even startsnelheid: object */
			VecSubf(pa->no, pa->co, vec);
			VecMulf(pa->no, paf->obfac);
			
			/* nu juiste interframe co berekenen */	
			fac= (ftime- ffloor(ftime));
			pa->co[0]= fac*pa->co[0] + (1.0-fac)*vec[0];
			pa->co[1]= fac*pa->co[1] + (1.0-fac)*vec[1];
			pa->co[2]= fac*pa->co[2] + (1.0-fac)*vec[2];
		}
		
		/* startsnelheid: normaal */
		if(paf->normfac!=0.0) {
			/* sp= mvert->no; */
				/* transpose ! */
			vec[0]= imat[0][0]*no[0] + imat[0][1]*no[1] + imat[0][2]*no[2];
			vec[1]= imat[1][0]*no[0] + imat[1][1]*no[1] + imat[1][2]*no[2];
			vec[2]= imat[2][0]*no[0] + imat[2][1]*no[1] + imat[2][2]*no[2];		
		
			Normalise(vec);
			VecMulf(vec, paf->normfac);
			VecAddf(pa->no, pa->no, vec);
		}
		pa->lifetime= paf->lifetime;
		if(paf->randlife!=0.0) {
			pa->lifetime*= 1.0+ paf->randlife*( drand48() - 0.5);
		}
		pa->mat_nr= 1;
		
		make_particle_keys(0, a, paf, pa, force, deform, mtexmove);
	}
	
	if(deform) end_latt_deform();
		
	/* restore */
	CFRA= cfraont;
	G.scene->r.framelen= framelenont;
	give_mesh_mvert(0, 0, 0, 0);


	/* hele hiera terug */
	par= ob;
	while(par) {
		popfirst(par);
		/* geen ob->ipo doen: insertkey behouden */
		do_ob_key(par);
		par= par->parent;
	}

	/* restore: NA popfirst */
	ob->sf= sfraont;

	disable_speed_curve(0);

	waitcursor(0);

}

/* ************* WAVE **************** */

void calc_wave_deform(WaveEff *wav, float ctime, float *co)
{
	/* co is in lokale coords */
	float lifefac, x, y, amplit;
	
	/* mag eigenlijk niet voorkomen */
	if((wav->flag & (WAV_X+WAV_Y))==0) return;	

	lifefac= wav->height;
	
	if( wav->lifetime!=0.0) {
		x= ctime - wav->timeoffs;
		if(x>wav->lifetime) {
			
			lifefac= x-wav->lifetime;
			
			if(lifefac > wav->damp) lifefac= 0.0;
			else lifefac= wav->height*(1.0 - fsqrt(lifefac/wav->damp));
		}
	}
	if(lifefac==0.0) return;

	x= co[0]-wav->startx;
	y= co[1]-wav->starty;

	if(wav->flag & WAV_X) {
		if(wav->flag & WAV_Y) amplit= fsqrt( (x*x + y*y));
		else amplit= x;
	}
	else amplit= y;
	
	/* zo maaktie mooie cirkels */
	amplit-= (ctime-wav->timeoffs)*wav->speed;

	if(wav->flag & WAV_CYCL) {
		amplit = fmodf(amplit-wav->width, 2.0*wav->width) + wav->width;
	}

	/* GAUSSIAN */
	
	if(amplit> -wav->width && amplit<wav->width) {
	
		amplit = amplit*wav->narrow;
		amplit= 1.0/exp(amplit*amplit) - wav->minfac;

		co[2]+= lifefac*amplit;
	}
}

void object_wave(Object *ob)
{
	WaveEff *wav;
	DispList *dl;
	Mesh *me;
	MVert *mvert;
	float *fp, ctime;
	int a, first;
	
	/* is er een mave */
	wav= ob->effect.first;
	while(wav) {
		if(wav->type==EFF_WAVE) break;
		wav= wav->next;
	}
	if(wav==0) return;
	
	if(ob->type==OB_MESH) {

		ctime= bsystem_time(ob, 0, (float)CFRA, 0.0);
		first= 1;
		
		me= ob->data;
		dl= find_displist_create(&ob->disp, DL_VERTS);

		if(dl->verts) freeN(dl->verts);
		dl->nr= me->totvert;
		dl->verts= mallocN(3*4*me->totvert, "wave");

		wav= ob->effect.first;
		while(wav) {
			if(wav->type==EFF_WAVE) {
				
				/* voorberekenen */
				wav->minfac= 1.0/exp(wav->width*wav->narrow*wav->width*wav->narrow);
				if(wav->damp==0) wav->damp= 10.0;
				
				mvert= me->mvert;
				fp= dl->verts;
				
				for(a=0; a<me->totvert; a++, mvert++, fp+=3) {
					if(first) VECCOPY(fp, mvert->co);
					calc_wave_deform(wav, ctime, fp);
				}
				first= 0;
			}
			wav= wav->next;
		}
	}
}

