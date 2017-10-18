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


/*  editlattice.c      GRAPHICS
 * 
 *  feb 96
 *  
 *  i.t.t. wat de naam doet vermoeden: ook algemene lattice (calc) functies
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "render.h"
#include "edit.h"
#include "ika.h"

void resizelattice(Lattice *lt);

Lattice *editLatt=0, *deformLatt=0;

float *latticedata=0, latmat[4][4];
int applyflag= 0;


void free_lattice(Lattice *lt)
{
	if(lt->def) freeN(lt->def);
}


Lattice *add_lattice()
{
	Lattice *lt;
	
	lt= alloc_libblock(&G.main->latt, ID_LT, "Lattice");
	
	lt->pntsu=lt->pntsv=lt->pntsw= 2;
	lt->flag= LT_GRID;
	
	lt->typeu= lt->typev= lt->typew= KEY_BSPLINE;
	
	/* tijdelijk */
	lt->def= callocN(sizeof(BPoint), "lattvert");
	
	resizelattice(lt);	/* maakt een regelmatige lattice */
		
	return lt;
}

Lattice *copy_lattice(Lattice *lt)
{
	Lattice *ltn;

	ltn= copy_libblock(lt);
	ltn->def= dupallocN(lt->def);
		
	id_us_plus((ID *)ltn->ipo);

	ltn->key= copy_key(ltn->key);
	if(ltn->key) ltn->key->from= (ID *)ltn;
	
	return ltn;
}


void make_local_lattice(Lattice *lt)
{
	Object *ob;
	Lattice *ltn;
	int local=0, lib=0;
	
	/* - zijn er alleen lib users: niet doen
	 * - zijn er alleen locale users: flag zetten
	 * - mixed: copy
	 */
	
	if(lt->id.lib==0) return;
	if(lt->id.us==1) {
		lt->id.lib= 0;
		lt->id.flag= LIB_LOCAL;
		new_id(0, (ID *)lt, 0);
		return;
	}
	
	ob= G.main->object.first;
	while(ob) {
		if(ob->data==lt) {
			if(ob->id.lib) lib= 1;
			else local= 1;
		}
		ob= ob->id.next;
	}
	
	if(local && lib==0) {
		lt->id.lib= 0;
		lt->id.flag= LIB_LOCAL;
		new_id(0, (ID *)lt, 0);
	}
	else if(local && lib) {
		ltn= copy_lattice(lt);
		ltn->id.us= 0;
		
		ob= G.main->object.first;
		while(ob) {
			if(ob->data==lt) {
				
				if(ob->id.lib==0) {
					ob->data= ltn;
					ltn->id.us++;
					lt->id.us--;
				}
			}
			ob= ob->id.next;
		}
	}
}



void calc_lat_fudu(int flag, int res, float *fu, float *du)
{
	
	if(res==1) {
		*fu= 0.0;
		*du= 0.0;
	}
	else if(flag & LT_GRID) {
		*fu= -0.5*(res-1);
		*du= 1.0;
	}
	else {
		*fu= -1.0;
		*du= 2.0/(res-1);
	}
	
}

void init_latt_deform(Object *oblatt, Object *ob)
{
	/* we maken een array met alle verschillen */
	BPoint *bp;
	float *fp, imat[4][4];
	float vec[3], fu, fv, fw, du=0.0, dv=0.0, dw=0.0;
	int tot, u, v, w;
	
	if(oblatt==G.obedit) deformLatt= editLatt;
	else deformLatt= oblatt->data;
	
	fp= latticedata= mallocN(sizeof(float)*3*deformLatt->pntsu*deformLatt->pntsv*deformLatt->pntsw, "latticedata");
	
	bp= deformLatt->def;

	if(ob) where_is_object(ob);

	/* bijv bij particle systeem: ob==0 */
	if(ob==0) {
		/* in deformspace, matrix berekenen */
		Mat4Invert(latmat, oblatt->obmat);
	
		/* terug: in deform array verwerken */
		Mat4Invert(imat, latmat);
	}
	else {
		/* in deformspace, matrix berekenen */
		Mat4Invert(imat, oblatt->obmat);
		Mat4MulMat4(latmat, ob->obmat, imat);
	
		/* terug: in deform array verwerken */
		Mat4Invert(imat, latmat);
	}
	calc_lat_fudu(deformLatt->flag, deformLatt->pntsu, &fu, &du);
	calc_lat_fudu(deformLatt->flag, deformLatt->pntsv, &fv, &dv);
	calc_lat_fudu(deformLatt->flag, deformLatt->pntsw, &fw, &dw);
	
	/* we berekenen hier steeds de u v w lattice coordinaten, weinig reden ze te onthouden */
	
	vec[2]= fw;
	for(w=0; w<deformLatt->pntsw; w++) {
		vec[1]= fv;
		for(v=0; v<deformLatt->pntsv; v++) {
			vec[0]= fu;
			for(u=0; u<deformLatt->pntsu; u++, bp++) {
				
				VecSubf(fp, bp->vec, vec);
				Mat4Mul3Vecfl(imat, fp);
		
				vec[0]+= du;
				fp+= 3;
			}
			vec[1]+= dv;
		}
		vec[2]+= dw;
	}
}

void calc_latt_deform(float *co)
{
	Lattice *lt;
	float fac, fu, du, u, v, w, tu[4], tv[4], tw[4];
	float *fpw, *fpv, *fpu, vec[3];
	int ui, vi, wi, uu, vv, ww;
	
	if(latticedata==0) return;
	
	lt= deformLatt;	/* kortere notatie! */
	
	/* co is in lokale coords, met latmat behandelen */
	
	VECCOPY(vec, co);
	Mat4MulVecfl(latmat, vec);
	
	/* u v w coords */
	
	if(lt->pntsu>1) {
		calc_lat_fudu(lt->flag, lt->pntsu, &fu, &du);
		u= (vec[0]-fu)/du;
		ui= ffloor(u);
		u -= ui;
		set_four_ipo(u, tu, lt->typeu);
	}
	else {
		tu[0]= tu[2]= tu[3]= 0.0; tu[1]= 1.0;
		ui= 0;
	}
	
	if(lt->pntsv>1) {
		calc_lat_fudu(lt->flag, lt->pntsv, &fu, &du);
		v= (vec[1]-fu)/du;
		vi= ffloor(v);
		v -= vi;
		set_four_ipo(v, tv, lt->typev);
	}
	else {
		tv[0]= tv[2]= tv[3]= 0.0; tv[1]= 1.0;
		vi= 0;
	}
	
	if(lt->pntsw>1) {
		calc_lat_fudu(lt->flag, lt->pntsw, &fu, &du);
		w= (vec[2]-fu)/du;
		wi= ffloor(w);
		w -= wi;
		set_four_ipo(w, tw, lt->typew);
	}
	else {
		tw[0]= tw[2]= tw[3]= 0.0; tw[1]= 1.0;
		wi= 0;
	}
	
	for(ww= wi-1; ww<=wi+2; ww++) {
		w= tw[ww-wi+1];
		
		if(w!=0.0) {
			if(ww>0) {
				if(ww<lt->pntsw) fpw= latticedata + 3*ww*lt->pntsu*lt->pntsv;
				else fpw= latticedata + 3*(lt->pntsw-1)*lt->pntsu*lt->pntsv;
			}
			else fpw= latticedata;
			
			for(vv= vi-1; vv<=vi+2; vv++) {
				v= w*tv[vv-vi+1];
				
				if(v!=0.0) {
					if(vv>0) {
						if(vv<lt->pntsv) fpv= fpw + 3*vv*lt->pntsu;
						else fpv= fpw + 3*(lt->pntsv-1)*lt->pntsu;
					}
					else fpv= fpw;
					
					for(uu= ui-1; uu<=ui+2; uu++) {
						u= v*tu[uu-ui+1];
						
						if(u!=0.0) {
							if(uu>0) {
								if(uu<lt->pntsu) fpu= fpv + 3*uu;
								else fpu= fpv + 3*(lt->pntsu-1);
							}
							else fpu= fpv;
							
							co[0]+= u*fpu[0];
							co[1]+= u*fpu[1];
							co[2]+= u*fpu[2];
						}
					}
				}
			}
		}
	}
}


void end_latt_deform()
{

	freeN(latticedata);
	latticedata= 0;
}


int object_deform(Object *ob)
{
	Mesh *me;
	Curve *cu;
	DispList *dl;
	MVert *mvert;
	float *fp;
	int a, tot;
	
	if(ob->parent==0) return 0;

	if(R.flag & R_RENDERING);
	else if(ob->parent->dt < OB_WIRE) return 0;
	
	/* altijd proberen in deze fie de hele deform te doen: apply! */
	
	if(ob->parent->type==OB_LATTICE) {
		
		init_latt_deform(ob->parent, ob);
		
		if(ob->type==OB_MESH) {
			me= ob->data;
			
			dl= find_displist_create(&ob->disp, DL_VERTS);
			
			mvert= me->mvert;
			if(dl->verts) freeN(dl->verts);
			dl->nr= me->totvert;
			dl->verts= fp= mallocN(3*sizeof(float)*me->totvert, "deform1");
	
			for(a=0; a<me->totvert; a++, mvert++, fp+=3) {
				if(applyflag) calc_latt_deform(mvert->co);
				else {
					VECCOPY(fp, mvert->co);
					calc_latt_deform(fp);
				}
			}
		}
		else if ELEM(ob->type, OB_CURVE, OB_SURF) {
		
			cu= ob->data;
			if(applyflag) {
				Nurb *nu;
				BPoint *bp;
				
				nu= cu->nurb.first;
				while(nu) {
					if(nu->bp) {
						a= nu->pntsu*nu->pntsv;
						bp= nu->bp;
						while(a--) {
							calc_latt_deform(bp->vec);
							bp++;
						}
					}
					nu= nu->next;
				}
			}
			
			/* when apply, do this too, looks more interactive */
			dl= cu->disp.first;
			while(dl) {
				
				fp= dl->verts;
				
				if(dl->type==DL_INDEX3) tot=dl->parts;
				else tot= dl->nr*dl->parts;
				
				for(a=0; a<tot; a++, fp+=3) {
					calc_latt_deform(fp);
				}
				
				dl= dl->next;
			}
		}
		end_latt_deform();

		boundbox_displist(ob);	

		return 1;
	}
	else if(ob->parent->type==OB_IKA) {
		Ika *ika;
		
		if(ob->partype!=PARSKEL) return 0;
		
		ika= ob->parent->data;
		if(ika->def==0) return 0;
		
		init_skel_deform(ob->parent, ob);
		
		if(ob->type==OB_MESH) {
			me= ob->data;
			
			dl= find_displist_create(&ob->disp, DL_VERTS);
			
			mvert= me->mvert;
			if(dl->verts) freeN(dl->verts);
			dl->nr= me->totvert;
			dl->verts= fp= mallocN(3*sizeof(float)*me->totvert, "deform1");
	
			for(a=0; a<me->totvert; a++, mvert++, fp+=3) {
				if(applyflag) calc_skel_deform(ika, mvert->co);
				else {
					VECCOPY(fp, mvert->co);
					calc_skel_deform(ika, fp);
				}
			}
		}
		else if ELEM(ob->type, OB_CURVE, OB_SURF) {
		
			cu= ob->data;
			if(applyflag) {
				Nurb *nu;
				BPoint *bp;
				
				nu= cu->nurb.first;
				while(nu) {
					if(nu->bp) {
						a= nu->pntsu*nu->pntsv;
						bp= nu->bp;
						while(a--) {
							calc_skel_deform(ika, bp->vec);
							bp++;
						}
					}
					nu= nu->next;
				}
			}
			
			/* when apply, do this too, looks more interactive */
			dl= cu->disp.first;
			while(dl) {
				
				fp= dl->verts;
				tot= dl->nr*dl->parts;
				for(a=0; a<tot; a++, fp+=3) {
					calc_skel_deform(ika, fp);
				}
				
				dl= dl->next;
			}
		}
		
		boundbox_displist(ob, ob->data);
		
		return 1;
	}
	
	return 0;

}

void apply_lattice()
{
	Base *base;
	Object *par;
	
	if(okee("Apply Lattice Deform")==0) return;
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			if(par= base->object->parent) {
				if(par->type==OB_LATTICE) {
					
					applyflag= 1;
					object_deform(base->object);
					applyflag= 0;
					
					base->object->parent= 0;
				}
			}
		}
		base= base->next;
	}
	
	allqueue(REDRAWVIEW3D, 0);
}

/* ***************************** */

BPoint *latt_bp(Lattice *lt, int u, int v, int w)
{
	return lt->def+ u + v*lt->pntsu + w*lt->pntsu*lt->pntsv;
}

void outside_lattice(Lattice *lt)
{
	BPoint *bp, *bp1, *bp2;
	int u, v, w;
	float vec[3], fac1, fac2, du=0.0, dv=0.0, dw=0.0;

	bp= lt->def;

	if(lt->pntsu>1) du= 1.0/((float)lt->pntsu-1);
	if(lt->pntsv>1) dv= 1.0/((float)lt->pntsv-1);
	if(lt->pntsw>1) dw= 1.0/((float)lt->pntsw-1);
		
	for(w=0; w<lt->pntsw; w++) {
		
		for(v=0; v<lt->pntsv; v++) {
		
			for(u=0; u<lt->pntsu; u++, bp++) {
				if(u==0 || v==0 || w==0 || u==lt->pntsu-1 || v==lt->pntsv-1 || w==lt->pntsw-1);
				else {
				
					bp->hide= 1;
					bp->f1 &= ~SELECT;
					
					/* u extrema */
					bp1= latt_bp(lt, 0, v, w);
					bp2= latt_bp(lt, lt->pntsu-1, v, w);
					
					fac1= du*u;
					bp->vec[0]= (1.0-fac1)*bp1->vec[0] + fac1*bp2->vec[0];
					bp->vec[1]= (1.0-fac1)*bp1->vec[1] + fac1*bp2->vec[1];
					bp->vec[2]= (1.0-fac1)*bp1->vec[2] + fac1*bp2->vec[2];
					
					/* v extrema */
					bp1= latt_bp(lt, u, 0, w);
					bp2= latt_bp(lt, u, lt->pntsv-1, w);
					
					fac1= dv*v;
					bp->vec[0]+= (1.0-fac1)*bp1->vec[0] + fac1*bp2->vec[0];
					bp->vec[1]+= (1.0-fac1)*bp1->vec[1] + fac1*bp2->vec[1];
					bp->vec[2]+= (1.0-fac1)*bp1->vec[2] + fac1*bp2->vec[2];
					
					/* w extrema */
					bp1= latt_bp(lt, u, v, 0);
					bp2= latt_bp(lt, u, v, lt->pntsw-1);
					
					fac1= dw*w;
					bp->vec[0]+= (1.0-fac1)*bp1->vec[0] + fac1*bp2->vec[0];
					bp->vec[1]+= (1.0-fac1)*bp1->vec[1] + fac1*bp2->vec[1];
					bp->vec[2]+= (1.0-fac1)*bp1->vec[2] + fac1*bp2->vec[2];
					
					VecMulf(bp->vec, 0.3333333);
					
				}
			}
			
		}
		
	}
	
}

void resizelattice(Lattice *lt)
{
	BPoint *bp;
	int u, v, w;
	float vec[3], fu, fv, fw, du=0.0, dv=0.0, dw=0.0;
	

	freeN(lt->def);
	lt->def= callocN(lt->pntsu*lt->pntsv*lt->pntsw*sizeof(BPoint), "lattice bp");
	
	bp= lt->def;
	
	while(lt->pntsu*lt->pntsv*lt->pntsw > 32000) {
		if( lt->pntsu>=lt->pntsv && lt->pntsu>=lt->pntsw) lt->pntsu--;
		else if( lt->pntsv>=lt->pntsu && lt->pntsv>=lt->pntsw) lt->pntsv--;
		else lt->pntsw--;
	}
	
	calc_lat_fudu(lt->flag, lt->pntsu, &fu, &du);
	calc_lat_fudu(lt->flag, lt->pntsv, &fv, &dv);
	calc_lat_fudu(lt->flag, lt->pntsw, &fw, &dw);
	
	vec[2]= fw;
	for(w=0; w<lt->pntsw; w++) {
		vec[1]= fv;
		for(v=0; v<lt->pntsv; v++) {
			vec[0]= fu;
			for(u=0; u<lt->pntsu; u++, bp++) {
				VECCOPY(bp->vec, vec);
				vec[0]+= du;
			}
			vec[1]+= dv;
		}
		vec[2]+= dw;
	}
}

void free_editLatt()
{
	if(editLatt) {
		if(editLatt->def) freeN(editLatt->def);
		freeN(editLatt);
		editLatt= 0;
	}
}


void setflagsLatt(int flag)
{
	BPoint *bp;
	int a;
	
	bp= editLatt->def;
	
	a= editLatt->pntsu*editLatt->pntsv*editLatt->pntsw;
	
	while(a--) {
		if(bp->hide==0) {
			bp->f1= flag;
		}
		bp++;
	}
}



void make_editLatt()
{
	Lattice *lt;
	KeyBlock *actkey=0;
	
	free_editLatt();
	
	lt= G.obedit->data;

	/* keys? */
	if(lt->key) {
		actkey= lt->key->block.first;
		while(actkey) {
			if(actkey->flag & SELECT) break;
			actkey= actkey->next;
		}
	}

	if(actkey) {
		key_to_latt(actkey, lt);
	}

	editLatt= dupallocN(lt);
	
	editLatt->def= dupallocN(lt->def);
	
	setflagsLatt(0);
}


void load_editLatt()
{
	Lattice *lt;
	Base *base;
	KeyBlock *actkey=0;
	BPoint *bp;
	float *fp;
	int tot;
	
	lt= G.obedit->data;
	
	/* zijn er keys? */
	if(lt->key) {
		actkey= lt->key->block.first;
		while(actkey) {
			if(actkey->flag & SELECT) break;
			actkey= actkey->next;
		}
	}

	if(actkey) {
		/* aktieve key: vertices */
		tot= editLatt->pntsu*editLatt->pntsv*editLatt->pntsw;
		
		if(actkey->data) freeN(actkey->data);
		
		fp=actkey->data= callocN(lt->key->elemsize*tot, "actkey->data");
		actkey->totelem= tot;
	
		bp= editLatt->def;
		while(tot--) {
			VECCOPY(fp, bp->vec);
			fp+= 3;
			bp++;
		}
		
		if(actkey) do_spec_key(lt->key);
	}
	else {

		freeN(lt->def);
	
		lt->def= dupallocN(editLatt->def);

		lt->flag= editLatt->flag;

		lt->pntsu= editLatt->pntsu;
		lt->pntsv= editLatt->pntsv;
		lt->pntsw= editLatt->pntsw;
		
		lt->typeu= editLatt->typeu;
		lt->typev= editLatt->typev;
		lt->typew= editLatt->typew;
	}
}

void remake_editLatt()
{
	if(okee("Reload Original data")==0) return;
	
	make_editLatt();
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWBUTSEDIT, 0);
}


void deselectall_Latt()
{
	BPoint *bp;
	int a;
	
	bp= editLatt->def;
	
	a= editLatt->pntsu*editLatt->pntsv*editLatt->pntsw;
	
	allqueue(REDRAWVIEW3D, 0);
	
	while(a--) {
		if(bp->hide==0) {
			if(bp->f1) {
				setflagsLatt(0);
				return;
			}
		}
		bp++;
	}
	setflagsLatt(1);
}


BPoint *findnearestLattvert(int sel)
{
	/* sel==1: selected krijgen een nadeel */
	/* in bp wordt nearest weggeschreven */
	BPoint *bp1, *bp;
	short dist= 100, temp, mval[2], a;

	bp= 0;

	/* projektie doen */
	calc_lattverts_ext();	/* drawobject.c */
	
	getmouseco_areawin(mval);

			
	bp1= editLatt->def;
	
	a= editLatt->pntsu*editLatt->pntsv*editLatt->pntsw;
	
	while(a--) {
		if(bp1->hide==0) {
			temp= abs(mval[0]- bp1->s[0])+ abs(mval[1]- bp1->s[1]);
			if( (bp1->f1 & 1)==sel) temp+=5;
			if(temp<dist) { 
				bp= bp1; 
				dist= temp; 
			}
		}
		bp1++;
	}

	return bp;
}


void mouse_lattice()
{
	BPoint *bp=0;
	uint *rect;
	int a;
	short mval[2], xs, ys, hand;

	bp= findnearestLattvert(1);

	if(bp) {
		if((G.qual & 3)==0) {
		
			setflagsLatt(0);
			bp->f1 |= 1;

			allqueue(REDRAWVIEW3D, 0);
		}
		else {
			
			if(bp->f1 & 1) bp->f1 &= ~1;
			else bp->f1 |= 1;

			allqueue(REDRAWVIEW3D, 0);

		}

		countall();
	}

	rightmouse_transform();
	
}

