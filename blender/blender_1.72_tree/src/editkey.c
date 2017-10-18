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


/*  editkey.c      GRAPHICS
 * 
 *  mei 95
 *  
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "edit.h"
#include "ipo.h"


void default_key_ipo(Key *key)
{
	IpoCurve *icu;
	BezTriple *bezt;
	
	key->ipo= add_ipo("KeyIpo", ID_KE);
	
	icu= callocN(sizeof(IpoCurve), "ipocurve");
			
	icu->blocktype= ID_KE;
	icu->adrcode= KEY_SPEED;
	icu->flag= IPO_VISIBLE+IPO_SELECT;
	set_icu_vars(icu);
	
	addtail( &(key->ipo->curve), icu);
	
	icu->bezt= bezt= callocN(2*sizeof(BezTriple), "defaultipo");
	icu->totvert= 2;
	
	bezt->hide= IPO_BEZ;
	bezt->f1=bezt->f2= bezt->f3= SELECT;
	bezt->h1= bezt->h2= HD_AUTO;
	bezt++;
	bezt->vec[1][0]= 100.0;
	bezt->vec[1][1]= 1.0;
	bezt->hide= IPO_BEZ;
	bezt->f1=bezt->f2= bezt->f3= SELECT;
	bezt->h1= bezt->h2= HD_AUTO;
	
	calchandles_ipocurve(icu);
}

	

/* **************************************** */

void mesh_to_key(Mesh *me, KeyBlock *kb)
{
	MVert *mvert;
	float *fp;
	int a, tot;
	
	if(me->totvert==0) return;
	
	if(kb->data) freeN(kb->data);
	
	kb->data= callocN(me->key->elemsize*me->totvert, "kb->data");
	kb->totelem= me->totvert;
	
	mvert= me->mvert;
	fp= kb->data;
	for(a=0; a<kb->totelem; a++, fp+=3, mvert++) {
		VECCOPY(fp, mvert->co);
		
	}
}

void key_to_mesh(KeyBlock *kb, Mesh *me)
{
	MVert *mvert;
	float *fp;
	int a, tot;
	
	mvert= me->mvert;
	fp= kb->data;
	
	tot= MIN2(kb->totelem, me->totvert);
	
	for(a=0; a<tot; a++, fp+=3, mvert++) {
		VECCOPY(mvert->co, fp);
	}
	
}



void insert_meshkey(Mesh *me)
{
	Key *key;
	KeyBlock *kb, *kkb;
	float curpos;
	
	if(me->key==0) {
		me->key= add_key( (ID *)me);
		default_key_ipo(me->key);
	}
	key= me->key;
	
	kb= callocN(sizeof(KeyBlock), "Keyblock");
	addtail(&key->block, kb);
	kb->type= KEY_CARDINAL;
	
	curpos= bsystem_time(0, 0, (float)CFRA, 0.0);
	if(calc_ipo_spec(me->key->ipo, KEY_SPEED, &curpos)==0) {
		curpos /= 100.0;
	}
	kb->pos= curpos;
	
	key->totkey++;
	if(key->totkey==1) key->refkey= kb;
	
	mesh_to_key(me, kb);
	
	sort_keys(me->key);

	/* curent actief: */
	kkb= key->block.first;
	while(kkb) {
		kkb->flag &= ~SELECT;
		if(kkb==kb) kkb->flag |= SELECT;
		
		kkb= kkb->next;
	}
}

/* ******************** */

void latt_to_key(Lattice *lt, KeyBlock *kb)
{
	BPoint *bp;
	float *fp;
	int a, tot;
	
	tot= lt->pntsu*lt->pntsv*lt->pntsw;
	if(tot==0) return;
	
	if(kb->data) freeN(kb->data);
	
	kb->data= callocN(lt->key->elemsize*tot, "kb->data");
	kb->totelem= tot;
	
	bp= lt->def;
	fp= kb->data;
	for(a=0; a<kb->totelem; a++, fp+=3, bp++) {
		VECCOPY(fp, bp->vec);
	}
}

void key_to_latt(KeyBlock *kb, Lattice *lt)
{
	BPoint *bp;
	float *fp;
	int a, tot;
	
	bp= lt->def;
	fp= kb->data;
	
	tot= lt->pntsu*lt->pntsv*lt->pntsw;
	tot= MIN2(kb->totelem, tot);
	
	for(a=0; a<tot; a++, fp+=3, bp++) {
		VECCOPY(bp->vec, fp);
	}
	
}

void insert_lattkey(Lattice *lt)
{
	Key *key;
	KeyBlock *kb, *kkb;
	float curpos;
	
	if(lt->key==0) {
		lt->key= add_key( (ID *)lt);
		default_key_ipo(lt->key);
	}
	key= lt->key;
	
	kb= callocN(sizeof(KeyBlock), "Keyblock");
	addtail(&key->block, kb);
	kb->type= KEY_CARDINAL;
	
	curpos= bsystem_time(0, 0, (float)CFRA, 0.0);
	if(calc_ipo_spec(lt->key->ipo, KEY_SPEED, &curpos)==0) {
		curpos /= 100.0;
	}
	kb->pos= curpos;
	
	key->totkey++;
	if(key->totkey==1) key->refkey= kb;
	
	latt_to_key(lt, kb);
	
	sort_keys(lt->key);

	/* curent actief: */
	kkb= key->block.first;
	while(kkb) {
		kkb->flag &= ~SELECT;
		if(kkb==kb) kkb->flag |= SELECT;
		
		kkb= kkb->next;
	}
}

/* ******************************** */

void curve_to_key(Curve *cu, KeyBlock *kb, ListBase *nurb)
{
	Nurb *nu;
	BezTriple *bezt;
	BPoint *bp;
	float *fp;
	int a, tot;
	
	/* tellen */
	tot= count_curveverts(nurb);
	if(tot==0) return;
	
	if(kb->data) freeN(kb->data);
	
	kb->data= callocN(cu->key->elemsize*tot, "kb->data");
	kb->totelem= tot;
	
	nu= nurb->first;
	fp= kb->data;
	while(nu) {
		
		if(nu->bezt) {
			bezt= nu->bezt;
			a= nu->pntsu;
			while(a--) {
				VECCOPY(fp, bezt->vec[0]);
				fp+= 3;
				VECCOPY(fp, bezt->vec[1]);
				fp+= 3;
				VECCOPY(fp, bezt->vec[2]);
				fp+= 3;
				fp+= 3;	/* alfa's */
				bezt++;
			}
		}
		else {
			bp= nu->bp;
			a= nu->pntsu*nu->pntsv;
			while(a--) {
				VECCOPY(fp, bp->vec);
				fp[3]= bp->alfa;
				
				fp+= 4;
				bp++;
			}
		}
		nu= nu->next;
	}
}

void key_to_curve(KeyBlock *kb, Curve  *cu, ListBase *nurb)
{
	Nurb *nu;
	BezTriple *bezt;
	BPoint *bp;
	float *fp;
	int a, tot;
	
	nu= nurb->first;
	fp= kb->data;
	
	tot= count_curveverts(nurb);

	tot= MIN2(kb->totelem, tot);
	
	while(nu && tot>0) {
		
		if(nu->bezt) {
			bezt= nu->bezt;
			a= nu->pntsu;
			while(a-- && tot>0) {
				VECCOPY(bezt->vec[0], fp);
				fp+= 3;
				VECCOPY(bezt->vec[1], fp);
				fp+= 3;
				VECCOPY(bezt->vec[2], fp);
				fp+= 3;
				fp+= 3;	/* alfa's */
				
				tot-= 3;
				bezt++;
			}
		}
		else {
			bp= nu->bp;
			a= nu->pntsu*nu->pntsv;
			while(a-- && tot>0) {
				VECCOPY(bp->vec, fp);
				bp->alfa= fp[3];
				
				fp+= 4;
				tot--;
				bp++;
			}
		}
		nu= nu->next;
	}
}



void insert_curvekey(Curve *cu)
{
	Key *key;
	KeyBlock *kb, *kkb;
	float curpos;
	
	if(cu->key==0) {
		cu->key= add_key( (ID *)cu);
		default_key_ipo(cu->key);
	}
	key= cu->key;
	
	kb= callocN(sizeof(KeyBlock), "Keyblock");
	addtail(&key->block, kb);
	kb->type= KEY_CARDINAL;
	
	curpos= bsystem_time(0, 0, (float)CFRA, 0.0);
	if(calc_ipo_spec(cu->key->ipo, KEY_SPEED, &curpos)==0) {
		curpos /= 100.0;
	}
	kb->pos= curpos;
	
	key->totkey++;
	if(key->totkey==1) key->refkey= kb;
	
	if(editNurb.first) curve_to_key(cu, kb, &editNurb);
	else curve_to_key(cu, kb, &cu->nurb);
	
	sort_keys(cu->key);

	/* curent actief: */
	kkb= key->block.first;
	while(kkb) {
		kkb->flag &= ~SELECT;
		if(kkb==kb) kkb->flag |= SELECT;
		
		kkb= kkb->next;
	}
}


/* ******************** */

Key *give_current_key(Object *ob)
{
	Mesh *me;
	Curve *cu;
	Lattice *lt;
	
	if(ob->type==OB_MESH) {
		me= ob->data;
		return me->key;
	}
	else if ELEM(ob->type, OB_CURVE, OB_SURF) {
		cu= ob->data;
		return cu->key;
	}
	else if(ob->type==OB_LATTICE) {
		lt= ob->data;
		return lt->key;
	}
	return 0;
}


void showkeypos(Key *key, KeyBlock *kb)
{
	Object *ob;
	Base *base;
	Mesh *me;
	Lattice *lt;
	Curve *cu;
	int a, tot;
	
	/* vanuit ipo */
	ob= OBACT;
	if(ob==0) return;
	
	if(key == give_current_key(ob)) {
		
		if(ob->type==OB_MESH) {
			me= ob->data;

			cp_key(0, me->totvert, me->totvert, (char *)me->mvert->co, me->key, kb, 0);
		}
		else if(ob->type==OB_LATTICE) {
			lt= ob->data;
			tot= lt->pntsu*lt->pntsv*lt->pntsw;
			
			cp_key(0, tot, tot, (char *)lt->def->vec, lt->key, kb, 0);

			/* alle kinderen remakedisplist */
			base= FIRSTBASE;
			while(base) {
				if(ob==base->object->parent) makeDispList(base->object);
				base= base->next;
			}
		}
		else if ELEM(ob->type, OB_CURVE, OB_SURF) {
			cu= ob->data;
			tot= count_curveverts(&cu->nurb);
			cp_cu_key(cu, kb, 0, tot);

			/* alle users remakedisplist */
			base= FIRSTBASE;
			while(base) {
				if(ob->data==base->object->data) makeDispList(base->object);
				base= base->next;
			}
		}
		
		allqueue(REDRAWVIEW3D, 0);
	}
}

void deselectall_key()
{
	KeyBlock *kb;
	Key *key;
	
	if(G.sipo->blocktype!=ID_KE) return;
	key= (Key *)G.sipo->from;
	if(key==0) return;
	
	kb= key->block.first;
	while(kb) {
		kb->flag &= ~SELECT;
		kb= kb->next;
	}
}


void delete_key()
{
	KeyBlock *kb, *kbn;
	Key *key;
	
	if(G.sipo->blocktype!=ID_KE) return;

	if(okee("Erase selected keys")==0) return;
	
	key= (Key *)G.sipo->from;
	if(key==0) return;
	
	kb= key->block.first;
	while(kb) {
		kbn= kb->next;
		if(kb->flag & SELECT) {
			remlink(&key->block, kb);
			key->totkey--;
			if(key->refkey== kb) key->refkey= key->block.first;
			
			if(kb->data) freeN(kb->data);
			freeN(kb);
			
		}
		kb= kbn;
	}
	
	if(key->totkey==0) {
		if(GS(key->from->name)==ID_ME) ((Mesh *)key->from)->key= 0;
		else if(GS(key->from->name)==ID_CU) ((Curve *)key->from)->key= 0;
		else if(GS(key->from->name)==ID_LT) ((Lattice *)key->from)->key= 0;

		free_libblock_us(&(G.main->key), key);
		addqueue(curarea->headwin, REDRAW, 1);	/* ipo ook weg */
	}
	else do_spec_key(key);
	
	allqueue(REDRAWVIEW3D, 0);
	addqueue(curarea->win, REDRAW, 1);
}

void move_keys()
{
	Key *key;
	KeyBlock *kb;
	TransVert *transmain, *tv;
	float div, dy, vec[3], dvec[3];
	int a, tot=0, afbreek=0, firsttime= 1;
	ushort event;
	short mval[2], val, xo, yo;
	char str[32];
	
	if(G.sipo->blocktype!=ID_KE) return;
	
	if(G.sipo->ipo && G.sipo->ipo->id.lib) return;
	if(G.sipo->editipo==0) return;

	key= (Key *)G.sipo->from;
	if(key==0) return;
	
	/* welke keys doen mee */
	kb= key->block.first;
	while(kb) {
		if(kb->flag & SELECT) tot++;
		kb= kb->next;
	}
	
	if(tot==0) return;	
	
	tv=transmain= callocN(tot*sizeof(TransVert), "transmain");
	kb= key->block.first;
	while(kb) {
		if(kb->flag & SELECT) {
			tv->loc= &kb->pos;
			tv->oldloc[0]= kb->pos;
			tv++;
		}
		kb= kb->next;
	}
	
	getmouseco_areawin(mval);
	xo= mval[0];
	yo= mval[1];
	dvec[1]= 0.0;
	

	while(afbreek==0) {
		getmouseco_areawin(mval);
		if(mval[0]!=xo || mval[1]!=yo || firsttime) {
			firsttime= 0;
			
			dy= mval[1]- yo;

			div= G.v2d->mask.ymax-G.v2d->mask.ymin;
			dvec[1]+= (G.v2d->cur.ymax-G.v2d->cur.ymin)*(dy)/div;
			
			VECCOPY(vec, dvec);

			apply_keyb_grid(vec, 0.0, 1.0, 0.1, U.flag & AUTOGRABGRID);
			apply_keyb_grid(vec+1, 0.0, 1.0, 0.1, U.flag & AUTOGRABGRID);

			tv= transmain;
			for(a=0; a<tot; a++, tv++) {
				tv->loc[0]= tv->oldloc[0]+vec[1];
			}
			
			sprintf(str, "Y: %.3f  ", vec[1]);
			headerprint(str);
			
			xo= mval[0];
			yo= mval[1];
				
			force_draw();
		}
		else usleep(1);
		
		while(qtest()) {
			event= extern_qread(&val);
			if(val) {
				switch(event) {
				case ESCKEY:
				case LEFTMOUSE:
				case SPACEKEY:
					afbreek= 1;
					break;
				default:
					arrowsmovecursor(event);
				}
			}
		}
	}
	
	if(event==ESCKEY) {
		tv= transmain;
		for(a=0; a<tot; a++, tv++) {
			tv->loc[0]= tv->oldloc[0];
		}
	}
	
	sort_keys(key);
	do_spec_key(key);
	
	/* voor boundbox */
	editipo_changed(0);

	freeN(transmain);	
	allqueue(REDRAWVIEW3D, 0);
	addqueue(curarea->win, REDRAW, 1);
	addqueue(curarea->headwin, REDRAW, 1);
	
}

