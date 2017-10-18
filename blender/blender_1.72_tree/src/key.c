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


/*  key.c      MIXED MODEL
 * 
 *  mei 95
 *  
 * 
 */

#include "blender.h"
#include "ipo.h"

#define KEY_BPOINT		1
#define KEY_BEZTRIPLE	2

int slurph_opt= 1;


void free_key(Key *key)
{
	KeyBlock *kb;
	
	if(key->ipo) key->ipo->id.us--;
	
	
	while(kb= key->block.first) {
		
		if(kb->data) freeN(kb->data);
		
		remlink(&key->block, kb);
		freeN(kb);
	}
	
}


Key *add_key(ID *id)	/* algemeen */
{
	Key *key;
	char *el;
	
	key= alloc_libblock(&G.main->key, ID_KE, "Key");
	
	key->type= KEY_NORMAL;
	key->from= id;
	
	if( GS(id->name)==ID_ME) {
		el= key->elemstr;
		
		el[0]= 3;
		el[1]= IPO_FLOAT;
		el[2]= 0;
		
		key->elemsize= 12;
	}
	else if( GS(id->name)==ID_LT) {
		el= key->elemstr;
		
		el[0]= 3;
		el[1]= IPO_FLOAT;
		el[2]= 0;
		
		key->elemsize= 12;
	}
	else if( GS(id->name)==ID_CU) {
		el= key->elemstr;
		
		el[0]= 4;
		el[1]= IPO_BPOINT;
		el[2]= 0;
		
		key->elemsize= 16;
	}
	
	return key;
}

Key *copy_key(Key *key)
{
	Key *keyn;
	KeyBlock *kbn, *kb;
	int a;
	
	if(key==0) return 0;
	
	keyn= copy_libblock(key);
	
	keyn->ipo= copy_ipo(key->ipo);

	duplicatelist(&keyn->block, &key->block);
	
	kb= key->block.first;
	kbn= keyn->block.first;
	while(kbn) {
		
		if(kbn->data) kbn->data= dupallocN(kbn->data);
		if( kb==key->refkey ) keyn->refkey= kbn;
		
		kbn= kbn->next;
		kb= kb->next;
	}
	
	return keyn;
}

void make_local_key(Key *key)
{
	
	/* - zijn er alleen lib users: niet doen
	 * - zijn er alleen locale users: flag zetten
	 * - mixed: copy
	 */
	if(key==0) return;
	
	key->id.lib= 0;
	new_id(0, (ID *)key, 0);
	make_local_ipo(key->ipo);
}


void sort_keys(Key *key)
{
	KeyBlock *kb;
	int doit=1;
	
	while(doit) {
		doit= 0;
		
		kb= key->block.first;
		while(kb) {
			if(kb->next) {
				if(kb->pos > kb->next->pos) {
					remlink(&key->block, kb);
					
					/* insertlink(lb, prevlink, newlink): newlink komt na prevlink */
					insertlink(&key->block, kb->next, kb);
					
					doit= 1;
					break;
				}
			}
			kb= kb->next;
		}	
	}
	
}

/**************** do the key ****************/


void set_four_ipo(float d, float *data, int type)
{
	float d2, d3, fc;
	
	if(type==KEY_LINEAR) {
		data[0]= 0;
		data[1]= 1.0-d;
		data[2]= d;
		data[3]= 0.0;
	}
	else {
		d2= d*d;
		d3= d2*d;
		
		if(type==KEY_CARDINAL) {

			fc= 0.71;
			
			data[0]= -fc*d3		+2.0*fc*d2		-fc*d;
			data[1]= (2.0-fc)*d3	+(fc-3.0)*d2				+1.0;
			data[2]= (fc-2.0)*d3	+(3.0-2.0*fc)*d2 +fc*d;
			data[3]= fc*d3			-fc*d2;
		}
		else if(type==KEY_BSPLINE) {

			data[0]= -0.1666*d3	+0.5*d2	-0.5*d	+0.16666;
			data[1]= 0.5*d3		-d2				+0.6666;
			data[2]= -0.5*d3		+0.5*d2	+0.5*d	+0.1666;
			data[3]= 0.1666*d3			;
		}
	}
}

void set_afgeleide_four_ipo(float d, float *data, int type)
{
	float d2, d3, fc;
	
	if(type==KEY_LINEAR) {

	}
	else {
		d2= d*d;
		
		if(type==KEY_CARDINAL) {

			fc= 0.71;
			
			data[0]= -3.0*fc*d2		+4.0*fc*d		-fc;
			data[1]= 3.0*(2.0-fc)*d2	+2.0*(fc-3.0)*d;
			data[2]= 3.0*(fc-2.0)*d2	+2.0*(3.0-2.0*fc)*d +fc;
			data[3]= 3.0*fc*d2			-2.0*fc*d;
		}
		else if(type==KEY_BSPLINE) {

			data[0]= -0.1666*3.0*d2	+d	-0.5;
			data[1]= 1.5*d2		-2.0*d;
			data[2]= -1.5*d2		+d	+0.5;
			data[3]= 0.1666*3.0*d2			;
		}
	}
}

int setkeys(float fac, ListBase *lb, KeyBlock *k[], float *t, int cycl)
{
	/* return 1 betekent k[2] is de positie, 0 is interpoleren */
	KeyBlock *k1, *firstkey;
	float d, dpos, ofs=0, lastpos, temp, fval[4];
	short aantal=0, bsplinetype;

	firstkey= lb->first;
	k1= lb->last;
	lastpos= k1->pos;
	dpos= lastpos - firstkey->pos;

	if(fac < firstkey->pos) fac= firstkey->pos;
	else if(fac > k1->pos) fac= k1->pos;

	k1=k[0]=k[1]=k[2]=k[3]= firstkey;
	t[0]=t[1]=t[2]=t[3]= k1->pos;

	/* if(fac<0.0 || fac>1.0) return 1; */

	if(k1->next==0) return 1;

	if(cycl) {	/* voorsorteren */
		k[2]= k1->next;
		k[3]= k[2]->next;
		if(k[3]==0) k[3]=k1;
		while(k1) {
			if(k1->next==0) k[0]=k1;
			k1=k1->next;
		}
		k1= k[1];
		t[0]= k[0]->pos;
		t[1]+= dpos;
		t[2]= k[2]->pos + dpos;
		t[3]= k[3]->pos + dpos;
		fac+= dpos;
		ofs= dpos;
		if(k[3]==k[1]) { 
			t[3]+= dpos; 
			ofs= 2.0*dpos;
		}
		if(fac<t[1]) fac+= dpos;
		k1= k[3];
	}
	else {		/* voorsorteren */
		/* waarom dit voorsorteren niet eerder gedaan? voor juist interpoleren in begin noodz. */
		k[2]= k1->next;
		t[2]= k[2]->pos;
		k[3]= k[2]->next;
		if(k[3]==0) k[3]= k[2];
		t[3]= k[3]->pos;
		k1= k[3];
	}
	
	while( t[2]<fac ) {	/* goede plek vinden */
		if(k1->next==0) {
			if(cycl) {
				k1= firstkey;
				ofs+= dpos;
			}
			else if(t[2]==t[3]) break;
		}
		else k1= k1->next;

		t[0]= t[1]; 
		k[0]= k[1];
		t[1]= t[2]; 
		k[1]= k[2];
		t[2]= t[3]; 
		k[2]= k[3];
		t[3]= k1->pos+ofs; 
		k[3]= k1;

		if(ofs>2.1+lastpos) break;
	}
	
	bsplinetype= 0;
	if(k[1]->type==KEY_BSPLINE || k[2]->type==KEY_BSPLINE) bsplinetype= 1;


	if(cycl==0) {
		if(bsplinetype==0) {	/* B spline gaat niet door de punten */
			if(fac<=t[1]) {		/* fac voor 1e key */
				t[2]= t[1];
				k[2]= k[1];
				return 1;
			}
			if(fac>=t[2] ) {	/* fac na 2e key */
				return 1;
			}
		}
		else if(fac>t[2]) {	/* laatste key */
			fac= t[2];
			k[3]= k[2];
			t[3]= t[2];
		}
	}

	d= t[2]-t[1];
	if(d==0.0) {
		if(bsplinetype==0) {
			return 1;	/* beide keys gelijk */
		}
	}
	else d= (fac-t[1])/d;

	/* interpolatie */
	
	set_four_ipo(d, t, k[1]->type);

	if(k[1]->type != k[2]->type) {
		set_four_ipo(d, fval, k[2]->type);
		
		temp= 1.0-d;
		t[0]= temp*t[0]+ d*fval[0];
		t[1]= temp*t[1]+ d*fval[1];
		t[2]= temp*t[2]+ d*fval[2];
		t[3]= temp*t[3]+ d*fval[3];
	}

	return 0;

}

void flerp(aantal, in, f0, f1, f2, f3, t)	/* float */
int aantal;
float *in, *f0, *f1, *f2, *f3;
float *t;
{
	int a;

	for(a=0; a<aantal; a++) {
		in[a]= t[0]*f0[a]+t[1]*f1[a]+t[2]*f2[a]+t[3]*f3[a];
	}
}

void cp_key(int start, int end, int tot, char *poin, Key *key, KeyBlock *k, int mode)
{
	float ktot, kd;
	int elemsize, poinsize, a, b, *ofsp, ofs[32], flagflo=0;
	char *k1;
	char *cp, elemstr[8];

	if(key->from==0) return;

	if( GS(key->from->name)==ID_ME ) {
		ofs[0]= sizeof(MVert);
		ofs[1]= 0;
		poinsize= ofs[0];
	}
	else if( GS(key->from->name)==ID_LT ) {
		ofs[0]= sizeof(BPoint);
		ofs[1]= 0;
		poinsize= ofs[0];
	}
	else if( GS(key->from->name)==ID_CU ) {
		if(mode==KEY_BPOINT) ofs[0]= sizeof(BPoint);
		else ofs[0]= sizeof(BezTriple);
		
		ofs[1]= 0;
		poinsize= ofs[0];
	}


	if(end>tot) end= tot;
	
	k1= k->data;

	if(tot != k->totelem) {
		ktot= 0.0;
		flagflo= 1;
		if(k->totelem) {
			kd= k->totelem/(float)tot;
		}
		else return;
	}

	/* deze uitzondering is om slurphing mogelijk te maken */
	if(start!=0) {
		
		poin+= poinsize*start;
		
		if(flagflo) {
			ktot+= start*kd;
			a= ffloor(ktot);
			if(a) {
				ktot-= a;
				k1+= a*key->elemsize;
			}
		}
		else k1+= start*key->elemsize;
	}	
	

	if(mode==KEY_BEZTRIPLE) {
		elemstr[0]= 1;
		elemstr[1]= IPO_BEZTRIPLE;
		elemstr[2]= 0;
	}
	
	/* alleen in dit stuk, hierboven niet! */
	elemsize= key->elemsize;
	if(mode==KEY_BEZTRIPLE) elemsize*= 3;

	for(a=start; a<end; a++) {
		cp= key->elemstr;
		if(mode==KEY_BEZTRIPLE) cp= elemstr;

		ofsp= ofs;
		
		while( cp[0] ) {
			
			switch(cp[1]) {
			case IPO_FLOAT:
				
				memcpy(poin, k1, 4*cp[0]);
				poin+= ofsp[0];

				break;
			case IPO_BPOINT:
				memcpy(poin, k1, 3*4);
				memcpy(poin+16, k1+12, 4);
				
				poin+= ofsp[0];				

				break;
			case IPO_BEZTRIPLE:
				memcpy(poin, k1, 4*12);
				poin+= ofsp[0];	

				break;
			}
			
			cp+= 2; ofsp++;
		}
		
		/* gaan we moeilijk doen */
		if(flagflo) {
			ktot+= kd;
			while(ktot>=1.0) {
				ktot-= 1.0;
				k1+= elemsize;
			}
		}
		else k1+= elemsize;
		
		if(mode==KEY_BEZTRIPLE) a+=2;
	}
}

void cp_cu_key(Curve *cu, KeyBlock *kb, int start, int end)
{
	Nurb *nu;
	int a, step, tot, a1, a2;
	char *poin;

	tot= count_curveverts(&cu->nurb);
	nu= cu->nurb.first;
	a= 0;
	while(nu) {
		if(nu->bp) {
			
			step= nu->pntsu*nu->pntsv;
			
			/* uitzondering omdat keys graag met volledige blokken werken */
			poin= (char *)nu->bp->vec;
			poin -= a*sizeof(BPoint);
			
			a1= MAX2(a, start);
			a2= MIN2(a+step, end);
			
			if(a1<a2) cp_key(a1, a2, tot, poin, cu->key, kb, KEY_BPOINT);
		}
		else if(nu->bezt) {
			
			step= 3*nu->pntsu;
			
			poin= (char *)nu->bezt->vec;
			poin -= a*sizeof(BezTriple);

			a1= MAX2(a, start);
			a2= MIN2(a+step, end);

			if(a1<a2) cp_key(a1, a2, tot, poin, cu->key, kb, KEY_BEZTRIPLE);
			
		}
		a+= step;
		nu=nu->next;
	}
}


void rel_flerp(aantal, in, ref, out, fac)
int aantal;
float *in, *ref, *out, fac;
{
	int a;

	for(a=0; a<aantal; a++) {
		in[a]-= fac*(ref[a]-out[a]);
	}
}



void do_rel_key(int start, int end, int tot, char *basispoin, Key *key, float ctime, int mode)
{
	KeyBlock *kb;
	IpoCurve *icu;
	float fac[32], fval;
	int *ofsp, ofs[3], elemsize, poinsize, a, b;
	char *cp, *poin, *reffrom, *from, elemstr[8];
	
	if(key->from==0) return;
	if(key->ipo==0) return;
	
	if( GS(key->from->name)==ID_ME ) {
		ofs[0]= sizeof(MVert);
		ofs[1]= 0;
		poinsize= ofs[0];
	}
	else if( GS(key->from->name)==ID_LT ) {
		ofs[0]= sizeof(BPoint);
		ofs[1]= 0;
		poinsize= ofs[0];
	}
	else if( GS(key->from->name)==ID_CU ) {
		if(mode==KEY_BPOINT) ofs[0]= sizeof(BPoint);
		else ofs[0]= sizeof(BezTriple);
		
		ofs[1]= 0;
		poinsize= ofs[0];
	}
	
	if(end>tot) end= tot;
	
	/* in geval beztriple */
	elemstr[0]= 1;				/* aantal ipofloats */
	elemstr[1]= IPO_BEZTRIPLE;
	elemstr[2]= 0;

	/* alleen in dit stuk, hierboven niet! */
	elemsize= key->elemsize;
	if(mode==KEY_BEZTRIPLE) elemsize*= 3;

	/* step one: fetch ipo values */
	icu= key->ipo->curve.first;
	for(a=0; a<KEY_TOTIPO; a++) fac[a]= 0.0;
	while(icu) {
		fac[icu->adrcode]= icu->curval;
		icu= icu->next;
	}
	
	/* step 2 init */
	cp_key(start, end, tot, basispoin, key, key->refkey, mode);
	
	/* step 3: do it */
	
	a= 1;
	kb= key->block.first;
	while(kb) {
		
		if(kb!=key->refkey) {
			fval= fac[a];
			a++;
			if(a==32) break;

			/* no difference allowed */
			if(kb->totelem==tot) {
				
				poin= basispoin;
				reffrom= key->refkey->data;
				from= kb->data;
				
				poin+= start*ofs[0];
				reffrom+= elemsize*start;
				from+= elemsize*start;
				
				for(b=start; b<end; b++) {
				
					cp= key->elemstr;	
					if(mode==KEY_BEZTRIPLE) cp= elemstr;
					
					ofsp= ofs;
					
					while( cp[0] ) {	/* cp[0]==aantal */
						
						switch(cp[1]) {
						case IPO_FLOAT:
							rel_flerp(cp[0], poin, reffrom, from, fval);
							
							break;
						case IPO_BPOINT:
							rel_flerp(3, poin, reffrom, from, fval);
							rel_flerp(1, poin+16, reffrom+16, from+16, fval);
			
							break;
						case IPO_BEZTRIPLE:
							rel_flerp(9, poin, reffrom, from, fval);
			
							break;
						}
						
						poin+= ofsp[0];				
						
						cp+= 2;
						ofsp++;
					}
					
					reffrom+= elemsize;
					from+= elemsize;
					
				}
			}
		}
		kb= kb->next;
	}
}


void do_key(int start, int end, int tot, char *poin, Key *key, KeyBlock **k, float *t, int mode)
{
	float k1tot, k2tot, k3tot, k4tot;
	float k1d, k2d, k3d, k4d;
	int a, b, ofs[32], *ofsp;
	int flagdo= 15, flagflo=0, elemsize, poinsize=0;
	char *k1, *k2, *k3, *k4;
	char *cp, elemstr[8];;

	if(key->from==0) return;

	if( GS(key->from->name)==ID_ME ) {
		ofs[0]= sizeof(MVert);
		ofs[1]= 0;
		poinsize= ofs[0];
	}
	else if( GS(key->from->name)==ID_LT ) {
		ofs[0]= sizeof(BPoint);
		ofs[1]= 0;
		poinsize= ofs[0];
	}
	else if( GS(key->from->name)==ID_CU ) {
		if(mode==KEY_BPOINT) ofs[0]= sizeof(BPoint);
		else ofs[0]= sizeof(BezTriple);
		
		ofs[1]= 0;
		poinsize= ofs[0];
	}
	
	if(end>tot) end= tot;

	k1= k[0]->data;
	k2= k[1]->data;
	k3= k[2]->data;
	k4= k[3]->data;

	/* testen op meer of minder punten (per key!) */
	if(tot != k[0]->totelem) {
		k1tot= 0.0;
		flagflo |= 1;
		if(k[0]->totelem) {
			k1d= k[0]->totelem/(float)tot;
		}
		else flagdo -= 1;
	}
	if(tot != k[1]->totelem) {
		k2tot= 0.0;
		flagflo |= 2;
		if(k[0]->totelem) {
			k2d= k[1]->totelem/(float)tot;
		}
		else flagdo -= 2;
	}
	if(tot != k[2]->totelem) {
		k3tot= 0.0;
		flagflo |= 4;
		if(k[0]->totelem) {
			k3d= k[2]->totelem/(float)tot;
		}
		else flagdo -= 4;
	}
	if(tot != k[3]->totelem) {
		k4tot= 0.0;
		flagflo |= 8;
		if(k[0]->totelem) {
			k4d= k[3]->totelem/(float)tot;
		}
		else flagdo -= 8;
	}

		/* deze uitzondering is om slurphing mogelijk te maken */
	if(start!=0) {

		poin+= poinsize*start;
		
		if(flagdo & 1) {
			if(flagflo & 1) {
				k1tot+= start*k1d;
				a= ffloor(k1tot);
				if(a) {
					k1tot-= a;
					k1+= a*key->elemsize;
				}
			}
			else k1+= start*key->elemsize;
		}
		if(flagdo & 2) {
			if(flagflo & 2) {
				k2tot+= start*k2d;
				a= ffloor(k2tot);
				if(a) {
					k2tot-= a;
					k2+= a*key->elemsize;
				}
			}
			else k2+= start*key->elemsize;
		}
		if(flagdo & 4) {
			if(flagflo & 4) {
				k3tot+= start*k3d;
				a= ffloor(k3tot);
				if(a) {
					k3tot-= a;
					k3+= a*key->elemsize;
				}
			}
			else k3+= start*key->elemsize;
		}
		if(flagdo & 8) {
			if(flagflo & 8) {
				k4tot+= start*k4d;
				a= ffloor(k4tot);
				if(a) {
					k4tot-= a;
					k4+= a*key->elemsize;
				}
			}
			else k4+= start*key->elemsize;
		}

	}

	/* in geval beztriple */
	elemstr[0]= 1;				/* aantal ipofloats */
	elemstr[1]= IPO_BEZTRIPLE;
	elemstr[2]= 0;

	/* alleen in dit stuk, hierboven niet! */
	elemsize= key->elemsize;
	if(mode==KEY_BEZTRIPLE) elemsize*= 3;

	for(a=start; a<end; a++) {
	
		cp= key->elemstr;	
		if(mode==KEY_BEZTRIPLE) cp= elemstr;
		
		ofsp= ofs;
		
		while( cp[0] ) {	/* cp[0]==aantal */
			
			switch(cp[1]) {
			case IPO_FLOAT:
				flerp(cp[0], poin, k1, k2, k3, k4, t);
				poin+= ofsp[0];				

				break;
			case IPO_BPOINT:
				flerp(3, poin, k1, k2, k3, k4, t);
				flerp(1, poin+16, k1+12, k2+12, k3+12, k4+12, t);
				
				poin+= ofsp[0];				

				break;
			case IPO_BEZTRIPLE:
				flerp(9, poin, k1, k2, k3, k4, t);
				poin+= ofsp[0];				

				break;
			}
			

			cp+= 2;
			ofsp++;
		}
		/* gaan we moeilijk doen: als keys van lengte verschillen */
		if(flagdo & 1) {
			if(flagflo & 1) {
				k1tot+= k1d;
				while(k1tot>=1.0) {
					k1tot-= 1.0;
					k1+= elemsize;
				}
			}
			else k1+= elemsize;
		}
		if(flagdo & 2) {
			if(flagflo & 2) {
				k2tot+= k2d;
				while(k2tot>=1.0) {
					k2tot-= 1.0;
					k2+= elemsize;
				}
			}
			else k2+= elemsize;
		}
		if(flagdo & 4) {
			if(flagflo & 4) {
				k3tot+= k3d;
				while(k3tot>=1.0) {
					k3tot-= 1.0;
					k3+= elemsize;
				}
			}
			else k3+= elemsize;
		}
		if(flagdo & 8) {
			if(flagflo & 8) {
				k4tot+= k4d;
				while(k4tot>=1.0) {
					k4tot-= 1.0;
					k4+= elemsize;
				}
			}
			else k4+= elemsize;
		}
		
		if(mode==KEY_BEZTRIPLE) a+= 2;
	}
}

void switch_endian_keyblock(Key *key, KeyBlock *kb)
{
	int elemsize, a, b;
	char *data, *poin, *cp;
	
	elemsize= key->elemsize;
	data= kb->data;
	
	for(a=0; a<kb->totelem; a++) {
	
		cp= key->elemstr;	
		poin= data;
		
		while( cp[0] ) {	/* cp[0]==aantal */
			
			switch(cp[1]) {		/* cp[1]= type */
			case IPO_FLOAT:
			case IPO_BPOINT:
			case IPO_BEZTRIPLE:
				b= cp[0];
				while(b--) {
					*((int *)poin)= le_int(*((int *)poin));
					poin+= 4;
				}
				break;
			}

			cp+= 2;
			
		}
		data+= elemsize;
	}
}




void do_mesh_key(Mesh *me)
{
	KeyBlock *k[4], *kb;
	float cfra, ctime, t[4], delta, loc[3], size[3];
	int a, flag, step;
	
	if(me->totvert==0) return;
	if(me->key==0) return;
	if(me->key->block.first==0) return;
	
	if(me->key->slurph && me->key->type!=KEY_RELATIVE ) {
		delta= me->key->slurph;
		delta/= me->totvert;
		
		step= 1;
		if(me->totvert>100 && slurph_opt) {
			step= me->totvert/50;
			delta*= step;
			/* in do_key en cp_key wordt a>tot afgevangen */
		}
		
		cfra= CFRA;
		
		for(a=0; a<me->totvert; a+=step, cfra+= delta) {
			
			ctime= bsystem_time(0, 0, cfra, 0.0);
			if(calc_ipo_spec(me->key->ipo, KEY_SPEED, &ctime)==0) {
				ctime /= 100.0;
				CLAMP(ctime, 0.0, 1.0);
			}
		
			flag= setkeys(ctime, &me->key->block, k, t, 0);
			if(flag==0) {
				
				do_key(a, a+step, me->totvert, (char *)me->mvert->co, me->key, k, t, 0);
			}
			else {
				cp_key(a, a+step, me->totvert, (char *)me->mvert->co, me->key, k[2], 0);
			}
		}
		
		if(flag && k[2]==me->key->refkey) tex_space_mesh(me);
		else boundbox_mesh(me, loc, size);
		
	}
	else {
		
		ctime= bsystem_time(0, 0, (float)CFRA, 0.0);
		if(calc_ipo_spec(me->key->ipo, KEY_SPEED, &ctime)==0) {
			ctime /= 100.0;
			CLAMP(ctime, 0.0, 1.0);
		}
		
		if(me->key->type==KEY_RELATIVE) {
			do_rel_key(0, me->totvert, me->totvert, (char *)me->mvert->co, me->key, ctime, 0);
		}
		else {
			flag= setkeys(ctime, &me->key->block, k, t, 0);
			if(flag==0) {
				
				do_key(0, me->totvert, me->totvert, (char *)me->mvert->co, me->key, k, t, 0);
			}
			else {
				cp_key(0, me->totvert, me->totvert, (char *)me->mvert->co, me->key, k[2], 0);
	
			}
			
			if(flag && k[2]==me->key->refkey) tex_space_mesh(me);
			else boundbox_mesh(me, loc, size);
		}
	}
}

void do_cu_key(Curve *cu, KeyBlock **k, float *t)
{
	Nurb *nu;
	int a, step, tot;
	char *poin;
	
	tot= count_curveverts(&cu->nurb);
	nu= cu->nurb.first;
	a= 0;
	while(nu) {
		if(nu->bp) {
			
			step= nu->pntsu*nu->pntsv;
			
			/* uitzondering omdat keys graag met volledige blokken werken */
			poin= (char *)nu->bp->vec;
			poin -= a*sizeof(BPoint);
			
			do_key(a, a+step, tot, poin, cu->key, k, t, KEY_BPOINT);
		}
		else if(nu->bezt) {
			
			step= 3*nu->pntsu;
			
			poin= (char *)nu->bezt->vec;
			poin -= a*sizeof(BezTriple);

			do_key(a, a+step, tot, poin, cu->key, k, t, KEY_BEZTRIPLE);
			
		}
		a+= step;
		nu=nu->next;
	}
}

void do_rel_cu_key(Curve *cu, float ctime)
{
	Nurb *nu;
	int a, step, tot;
	char *poin;
	
	tot= count_curveverts(&cu->nurb);
	nu= cu->nurb.first;
	a= 0;
	while(nu) {
		if(nu->bp) {
			
			step= nu->pntsu*nu->pntsv;
			
			/* uitzondering omdat keys graag met volledige blokken werken */
			poin= (char *)nu->bp->vec;
			poin -= a*sizeof(BPoint);
			
			do_rel_key(a, a+step, tot, poin, cu->key, ctime, KEY_BPOINT);
		}
		else if(nu->bezt) {
			
			step= 3*nu->pntsu;
			
			poin= (char *)nu->bezt->vec;
			poin -= a*sizeof(BezTriple);

			do_rel_key(a, a+step, tot, poin, cu->key, ctime, KEY_BEZTRIPLE);
			
		}
		a+= step;
		nu=nu->next;
	}
}

void do_curve_key(Curve *cu)
{
	KeyBlock *k[4], *kb;
	Nurb *nu;
	float cfra, ctime, t[4], delta, loc[3], size[3];
	int a, flag, step, tot;
	
	tot= count_curveverts(&cu->nurb);
	
	if(tot==0) return;
	if(cu->key==0) return;
	if(cu->key->block.first==0) return;
	
	if(cu->key->slurph) {
		delta= cu->key->slurph;
		delta/= tot;
		
		step= 1;
		if(tot>100 && slurph_opt) {
			step= tot/50;
			delta*= step;
			/* in do_key en cp_key wordt a>tot afgevangen */
		}
		
		cfra= CFRA;
		
		for(a=0; a<tot; a+=step, cfra+= delta) {
			
			ctime= bsystem_time(0, 0, cfra, 0.0);
			if(calc_ipo_spec(cu->key->ipo, KEY_SPEED, &ctime)==0) {
				ctime /= 100.0;
				CLAMP(ctime, 0.0, 1.0);
			}
		
			flag= setkeys(ctime, &cu->key->block, k, t, 0);
			if(flag==0) {
				
				/* do_key(a, a+step, tot, (char *)cu->mvert->co, cu->key, k, t, 0); */
			}
			else {
				/* cp_key(a, a+step, tot, (char *)cu->mvert->co, cu->key, k[2],0); */
			}
		}

		if(flag && k[2]==cu->key->refkey) tex_space_curve(cu);
		
		
	}
	else {
		
		ctime= bsystem_time(0, 0, (float)CFRA, 0.0);
		if(calc_ipo_spec(cu->key->ipo, KEY_SPEED, &ctime)==0) {
			ctime /= 100.0;
			CLAMP(ctime, 0.0, 1.0);
		}
		
		if(cu->key->type==KEY_RELATIVE) {
			do_rel_cu_key(cu, ctime);
		}
		else {
			flag= setkeys(ctime, &cu->key->block, k, t, 0);
			
			if(flag==0) do_cu_key(cu, k, t);
			else cp_cu_key(cu, k[2], 0, tot);
					
			if(flag && k[2]==cu->key->refkey) tex_space_curve(cu);
		}
	}
}

void do_latt_key(Lattice *lt)
{
	KeyBlock *k[4], *kb;
	float delta, cfra, ctime, t[4];
	int a, tot, flag;
	
	if(lt->key==0) return;
	if(lt->key->block.first==0) return;
	
	tot= lt->pntsu*lt->pntsv*lt->pntsw;

	if(lt->key->slurph) {
		delta= lt->key->slurph;
		delta/= (float)tot;
		
		cfra= CFRA;
		
		for(a=0; a<tot; a++, cfra+= delta) {
			
			ctime= bsystem_time(0, 0, cfra, 0.0);
			if(calc_ipo_spec(lt->key->ipo, KEY_SPEED, &ctime)==0) {
				ctime /= 100.0;
				CLAMP(ctime, 0.0, 1.0);
			}
		
			flag= setkeys(ctime, &lt->key->block, k, t, 0);
			if(flag==0) {
				
				do_key(a, a+1, tot, (char *)lt->def->vec, lt->key, k, t, 0);
			}
			else {
				cp_key(a, a+1, tot, (char *)lt->def->vec, lt->key, k[2], 0);
			}
		}		
	}
	else {
		ctime= bsystem_time(0, 0, (float)CFRA, 0.0);
		if(calc_ipo_spec(lt->key->ipo, KEY_SPEED, &ctime)==0) {
			ctime /= 100.0;
			CLAMP(ctime, 0.0, 1.0);
		}
	
		if(lt->key->type==KEY_RELATIVE) {
			do_rel_key(0, tot, tot, (char *)lt->def->vec, lt->key, ctime, 0);
		}
		else {

			flag= setkeys(ctime, &lt->key->block, k, t, 0);
			if(flag==0) {
				do_key(0, tot, tot, (char *)lt->def->vec, lt->key, k, t, 0);
			}
			else {
				cp_key(0, tot, tot, (char *)lt->def->vec, lt->key, k[2], 0);
			}
		}
	}
	
	if(lt->flag & LT_OUTSIDE) outside_lattice(lt);
}



void do_all_keys()
{
	Key *key;
	int idcode;
	
	key= G.main->key.first;
	while(key) {
		if(key->from) {
			idcode= GS(key->from->name);
		
			if(idcode==ID_ME) do_mesh_key( (Mesh *)key->from);
			else if(idcode==ID_CU) do_curve_key( (Curve *)key->from);
			else if(idcode==ID_LT) do_latt_key( (Lattice *)key->from);
		}
		key= key->id.next;
	}
}

void do_ob_key(Object *ob)
{
	if(ob->type==OB_MESH) do_mesh_key( ob->data);
	else if(ob->type==OB_CURVE) do_curve_key( ob->data);
	else if(ob->type==OB_SURF) do_curve_key( ob->data);
	else if(ob->type==OB_LATTICE) do_latt_key( ob->data);
}

void do_spec_key(Key *key)
{
	int idcode;
	
	if(key==0) return;
	
	idcode= GS(key->from->name);
	
	if(idcode==ID_ME) do_mesh_key( (Mesh *)key->from);
	else if(idcode==ID_CU) do_curve_key( (Curve *)key->from);
	else if(idcode==ID_LT) do_latt_key( (Lattice *)key->from);
	
}

