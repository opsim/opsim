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


/*  ika.c      MIXED MODEL
 * 
 *  april 96
 *  
 * 
 */

#include "blender.h"
#include "ika.h"


#define TOLER 0.000076


void unlink_ika(Ika *ika)
{
	/* loskoppelen: */
	

}

/* niet Ika zelf vrijgeven */
void free_ika(Ika *ika)
{

	unlink_ika(ika);
	
	freelistN(&ika->limbbase);
	
	if(ika->def) freeN(ika->def);
}

Ika *add_ika()
{
	Ika *ika;
	Limb *li;
	
	ika= alloc_libblock(&G.main->ika, ID_IK, "Ika");
	ika->flag = IK_GRABEFF | IK_XYCONSTRAINT;

	ika->xyconstraint= 0.5;
	ika->mem= 0.3;
	ika->iter= 6;
	
	return ika;
}

Ika *copy_ika(Ika *ika)
{
	Ika *ikan;
	
	ikan= copy_libblock(ika);
	
	duplicatelist(&ikan->limbbase, &ika->limbbase);

	ikan->def= dupallocN(ikan->def);
	
	return ikan;
}

void make_local_ika(Ika *ika)
{
	Object *ob;
	Ika *ikan;
	int local=0, lib=0;
	
	/* - zijn er alleen lib users: niet doen
	 * - zijn er alleen locale users: flag zetten
	 * - mixed: copy
	 */
	
	if(ika->id.lib==0) return;
	if(ika->id.us==1) {
		ika->id.lib= 0;
		ika->id.flag= LIB_LOCAL;
		new_id(0, (ID *)ika, 0);
		return;
	}
	
	ob= G.main->object.first;
	while(ob) {
		if(ob->data==ika) {
			if(ob->id.lib) lib= 1;
			else local= 1;
		}
		ob= ob->id.next;
	}
	
	if(local && lib==0) {
		ika->id.lib= 0;
		ika->id.flag= LIB_LOCAL;
		new_id(0, (ID *)ika, 0);
	}
	else if(local && lib) {
		ikan= copy_ika(ika);
		ikan->id.us= 0;
		
		ob= G.main->object.first;
		while(ob) {
			if(ob->data==ika) {
				
				if(ob->id.lib==0) {
					ob->data= ikan;
					ikan->id.us++;
					ika->id.us--;
				}
			}
			ob= ob->id.next;
		}
	}
}

int count_limbs(Object *ob)
{
	int tot=0;
	Ika *ika;
	Limb *li;
	
	if(ob->type!=OB_IKA) return 0;
	ika= ob->data;
	
	li= ika->limbbase.first;
	while(li) {
		tot++;
		li= li->next;
	}
	return tot;
}

/* ************************************************** */


/* aan hand van eff[] de len en alpha */
void calc_limb(Limb *li)
{
	Limb *prev= li;
	float vec[2], alpha= 0.0;
	
	/* alpha van 'parents' */
	while(prev=prev->prev) alpha+= prev->alpha;
	
	if(li->prev) {
		vec[0]= -li->prev->eff[0];
		vec[1]= -li->prev->eff[1];
	}
	else vec[0]= vec[1]= 0.0;
	
	vec[0]+= li->eff[0];
	vec[1]+= li->eff[1];

	li->alpha= fatan2(vec[1], vec[0]) - alpha;
	li->len= fsqrt(vec[0]*vec[0] + vec[1]*vec[1]);

}

/* aan hand van len en alpha worden de eindpunten berekend */
void calc_ika(Ika *ika, Limb *li)
{
	float alpha=0.0, co, si;

	if(li) {		
		Limb *prev= li;
		while(prev=prev->prev) alpha+= prev->alpha;
	}
	else li= ika->limbbase.first;

	while(li) {
if(li->alpha != li->alpha) li->alpha= 0.0;	/* NaN patch */

		alpha+= li->alpha;

		co= fcos(alpha);
		si= fsin(alpha);
		
		li->eff[0]= co*li->len;
		li->eff[1]= si*li->len;
		
		if(li->prev) {
			li->eff[0]+= li->prev->eff[0];
			li->eff[1]+= li->prev->eff[1];
		}
		
		if(li->next==0) {
			ika->eff[0]= li->eff[0];
			ika->eff[1]= li->eff[1];
		}
		
		li= li->next;
	}
}

void init_defstate_ika(Object *ob)
{
	Ika *ika;
	Limb *li;
	
	ika= ob->data;
	ika->totx= 0.0;
	ika->toty= 0.0;
	li= ika->limbbase.first;
	
	calc_ika(ika, 0);	/* correcte eindpunten */
	
	while(li) {
		li->alphao= li->alpha;
		li->leno= li->len;
		
		li= li->next;
	}
	ika->eff[2]= 0.0;
	VecMat4MulVecfl(ika->effg, ob->obmat, ika->eff);
}

void itterate_limb(Ika *ika, Limb *li)
{
	float da, n1[2], n2[2], len1, len2;
	
	if(li->prev) {
		n1[0]= ika->eff[0] - li->prev->eff[0];
		n1[1]= ika->eff[1] - li->prev->eff[1];
		n2[0]= ika->effn[0] - li->prev->eff[0];
		n2[1]= ika->effn[1] - li->prev->eff[1];
	}
	else {
		n1[0]= ika->eff[0];
		n1[1]= ika->eff[1];
		n2[0]= ika->effn[0];
		n2[1]= ika->effn[1];
	}
	len1= fsqrt(n1[0]*n1[0] + n1[1]*n1[1]);
	len2= fsqrt(n2[0]*n2[0] + n2[1]*n2[1]);

	da= (1.0-li->fac)*safacos( (n1[0]*n2[0]+n1[1]*n2[1])/(len1*len2) );

	if(n1[0]*n2[1] < n1[1]*n2[0]) da= -da;
	
	li->alpha+= da;
	
}

void rotate_ika(Object *ob, Ika *ika)
{
	Limb *li;
	float len1, len2, da, n1[2], n2[2];
	
	/* terug roteren */
	euler_rot(ob->rot, -ika->toty, 'y');
	ika->toty= 0.0;
	
	where_is_object(ob);
	
	Mat4Invert(ob->imat, ob->obmat);
	VecMat4MulVecfl(ika->effn, ob->imat, ika->effg);
	
	li= ika->limbbase.last;
	if(li==0) return;
	
	n1[0]= ika->eff[0];
	n2[0]= ika->effn[0];
	n2[1]= ika->effn[2];
	
	len2= fsqrt(n2[0]*n2[0] + n2[1]*n2[1]);
	
	if(len2>TOLER) {
		da= (n2[0])/(len2);
		if(n1[0]<0.0) da= -da;
		
		/* als de x comp bijna nul is kan dit gebeuren */
		if(da<=-1.0+TOLER || da>=1.0) ;
		else {
		
			da= safacos( da );
			if(n1[0]*n2[1] > 0.0) da= -da;
	
			euler_rot(ob->rot, da, 'y');
			ika->toty= da;
		}
	}
}

void rotate_ika_xy(Object *ob, Ika *ika)
{
	Limb *li;
	float len1, len2, ang, da, n1[3], n2[3], axis[3], quat[4];
	
	/* terug roteren */
	euler_rot(ob->rot, -ika->toty, 'y');
	euler_rot(ob->rot, -ika->totx, 'x');
	
	where_is_object(ob);

	Mat4Invert(ob->imat, ob->obmat);
	VecMat4MulVecfl(ika->effn, ob->imat, ika->effg);
	
	li= ika->limbbase.last;
	if(li==0) return;
	
	/* ika->eff = old situation */
	/* ika->effn = desired situation */
	
	VECCOPY(n1, ika->effn);
	n1[2]= 0.0;
	VECCOPY(n2, ika->effn);
	
	Normalise(n1);
	Normalise(n2);

	ang= n1[0]*n2[0] + n1[1]*n2[1] + n1[2]*n2[2];
	ang= safacos(ang);
		
	if(ang<-0.0000001 || ang>0.00000001) {
		Crossf(axis, n1, n2);
		Normalise(axis);
		quat[0]= fcos(0.5*ang);
		da= fsin(0.5*ang);
		quat[1]= da*axis[0];
		quat[2]= da*axis[1];
		quat[3]= da*axis[2];
	
		QuatToEul(quat, axis);

		ika->totx= axis[0];
		CLAMP(ika->totx, -ika->xyconstraint, ika->xyconstraint);
		ika->toty= axis[1];
		CLAMP(ika->toty, -ika->xyconstraint, ika->xyconstraint);
	}

	euler_rot(ob->rot, ika->totx, 'x');
	euler_rot(ob->rot, ika->toty, 'y');
}

void itterate_ika(Object *ob)
{
	Ika *ika;
	Limb *li;
	float da, vec[3];
	int it;

	disable_where_script(1);

	ika= ob->data;
	if((ika->flag & IK_GRABEFF)==0) return;
	
	/* memory: grote tijdsprongen afvangen */
	it= abs(ika->lastfra-CFRA);
	ika->lastfra= CFRA;
	if(it>10) {
		
		/* one itteration extra */
		itterate_ika(ob);
	}
	else {
		li= ika->limbbase.first;
		while(li) {
			li->alpha= (1.0-ika->mem)*li->alpha + ika->mem*li->alphao;
			if(li->fac==1.0) li->fac= 0.05;	/* oude files: kan weg in juni 96 */
			li= li->next;
		}
	}
	calc_ika(ika, 0);
	
	/* effector heeft parent? */
	if(ika->parent) {
		
		if(ika->partype==PAROBJECT) {
			if(ika->parent->ctime != F_CFRA) where_is_object(ika->parent);
			VECCOPY(ika->effg, ika->parent->obmat[3]);
		}
		else {
			what_does_parent1(ika->parent, ika->partype, ika->par1, 0, 0);
			VECCOPY(ika->effg, workob.obmat[3]);
		}
	}


	/* y-as goed draaien */
	if(ika->flag & IK_XYCONSTRAINT) 
		rotate_ika_xy(ob, ika);
	else
		rotate_ika(ob, ika);

	it= ika->iter;
	while(it--) {
		
		where_is_object(ob);
		Mat4Invert(ob->imat, ob->obmat);
		VecMat4MulVecfl(ika->effn, ob->imat, ika->effg);
		/* forward: dan gaan ook de eerste limbs */
		li= ika->limbbase.first;
		while(li) {
			
			itterate_limb(ika, li);
			
			/* zet je calc_ika() buiten deze lus: lange kettingen instabiel */
			calc_ika(ika, li);

			li= li->next;
		}

		where_is_object(ob);
		Mat4Invert(ob->imat, ob->obmat);
		VecMat4MulVecfl(ika->effn, ob->imat, ika->effg);

		/* backward */
		li= ika->limbbase.last;
		while(li) {
			
			itterate_limb(ika, li);
			
			/* zet je calc_ika() buiten deze lus: lange kettingen instabiel */
			calc_ika(ika, li);

			li= li->prev;
		}
	}

	disable_where_script(0);
}


void do_all_ikas()
{
	Base *base;
	
	base= FIRSTBASE;
	while(base) {
		
		if(base->object->type==OB_IKA) itterate_ika(base->object);

		base= base->next;
	}
}

void do_all_visible_ikas()
{
	Base *base;
	
	base= FIRSTBASE;
	while(base) {
		if(base->lay & G.vd->lay) {
			if(base->object->type==OB_IKA) itterate_ika(base->object);
		}
		base= base->next;
	}
}

/* ******************** DEFORM ************************ */


void init_skel_deform(Object *par, Object *ob)
{
	Deform *def;
	Ika *ika;
	int a;
	
	/*  deform:
	 * 
	 *  ob_vec x ob_obmat x def_imat (weight fie) x def_obmat x ob_imat = ob_vec'
	 *   
	 *           <----- premat ---->                <---- postmat ---->
	 */
	
	if(par->type!=OB_IKA) return;
	
	Mat4Invert(ob->imat, ob->obmat);

	ika= par->data;
	a= ika->totdef;
	def= ika->def;
	while(a--) {
		
		what_does_parent1(def->ob, def->partype, def->par1, def->par2, def->par3);
		
		Mat4MulMat4(def->premat, ob->obmat, def->imat);
		Mat4MulMat4(def->postmat, workob.obmat, ob->imat);

		def++;
	}
}


void calc_skel_deform(Ika *ika, float *co)
{
	Deform *def;
	int a;
	float totw=0.0, weight, fac, len, vec[3], totvec[3];
	
	def= ika->def;
	if(def==0) return;
	a= ika->totdef;
	totvec[0]=totvec[1]=totvec[2]= 0.0;
	
	while(a--) {
		
		VecMat4MulVecfl(vec, def->premat, co);
		
		len= fsqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
		if(def->vec[0]==0.0) len+= len;
		else len+= fsqrt( (vec[0]+def->vec[0])*(vec[0]+def->vec[0]) + vec[1]*vec[1] + vec[2]*vec[2]);
		
		/* def->vec[0]= len limb */
		
		weight= 1.0/(0.001+len);
		weight*= weight;
		weight*= weight;
		weight*= def->fac;

		len-= def->vec[0];

		if(len >= def->dist) {
			weight= 0.0;
		}
		else {
			fac= (def->dist - len)/def->dist;
			weight*= fac;
		}
		
		if(weight > 0.0) {
			Mat4MulVecfl(def->postmat, vec);
			
			VecMulf(vec, weight);
			VecAddf(totvec, totvec, vec);
		
			totw+= weight;
		}
		def++;
	}
	
	if(totw==0.0) return;
	
	co[0]= totvec[0]/totw;
	co[1]= totvec[1]/totw;
	co[2]= totvec[2]/totw;
	
}

