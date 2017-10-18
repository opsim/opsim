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


/*  life.c      MIXED MODEL
 * 
 *  maart 96
 *  
 * 
 */

#ifndef FREE

#include "blender.h"
#include "screen.h"
#include "graphics.h"
#include "sector.h"
#include "ipo.h"

#include "psx.h"
#include "blendpsx.h"		/* voor pIpo */



void unlink_life(Life *lf)
{
	/* loskoppelen: */
	
	if(lf->dynamesh) lf->dynamesh->id.us--;
	if(lf->texmesh) lf->texmesh->id.us--;

}


/* niet life zelf vrijgeven */
void free_life(Life *lf)
{
	Sensor *sn;
	int a;
	
	unlink_life(lf);
	
	if(lf->sensors) {
	
		for(a=lf->totsens, sn=lf->sensors; a>0; a--, sn++) {
			if(sn->events) freeN(sn->events);
			if(sn->actions) freeN(sn->actions);
		}
	
		freeN(lf->sensors);
		
	}
	
}

Life *add_life()
{
	Life *lf;
	
	lf= alloc_libblock(&G.main->life, ID_LF, "Life");
	
	lf->type= LF_PROP;
	
	lf->flag= LF_SENSORS+LF_DO_FH;
	lf->lay= 1;
	lf->mass= 1.0;
	lf->frict= 0.001;
	lf->rotfrict= 0.001;
	lf->axsize= 0.5;
	lf->r= lf->g= lf->b= 1.0;
	
	return lf;
}

Life *copy_life(Life *lf)
{
	Life *lfn;
	Sensor *sn;
	int a;
	
	lfn= copy_libblock(lf);

	id_us_plus((ID *)lfn->dynamesh);
	id_us_plus((ID *)lfn->texmesh);

	lfn->sensors= dupallocN(lfn->sensors);
	for(a=lfn->totsens, sn=lfn->sensors; a>0; a--, sn++) {
		sn->events= dupallocN(sn->events);
		sn->actions= dupallocN(sn->actions);
	}
	
	return lfn;
}

void make_local_life(Life *lf)
{
	Object *ob;
	Life *lfn;
	int local=0, lib=0;
	
	/* - zijn er alleen lib users: niet doen
	 * - zijn er alleen locale users: flag zetten
	 * - mixed: copy
	 */
	
	if(lf->id.us==1) {
		lf->id.lib= 0;
		lf->id.flag= LIB_LOCAL;
		new_id(0, (ID *)lf, 0);
		
		if(lf->texmesh) make_local_mesh(lf->texmesh);
		if(lf->dynamesh) make_local_mesh(lf->dynamesh);

		return;
	}
	if(lf->id.lib==0) return;


	ob= G.main->object.first;
	while(ob) {
		if(ob->data==lf) {
			if(ob->id.lib) lib= 1;
			else local= 1;
		}
		ob= ob->id.next;
	}
	
	if(local && lib==0) {
		lf->id.lib= 0;
		lf->id.flag= LIB_LOCAL;
		new_id(0, (ID *)lf, 0);
	}
	else if(local && lib) {
		lfn= copy_life(lf);
		lfn->id.us= 0;
		
		ob= G.main->object.first;
		while(ob) {
			if(ob->data==lf) {
				
				if(ob->id.lib==0) {
					ob->data= lfn;
					lfn->id.us++;
					lf->id.us--;
				}
			}
			ob= ob->id.next;
		}
	}
}

/* ******************************** */

void life_to_local_sector_co(Life *lf)
{
	if(lf->sector) {
		/* transformeren naar locale coordinaten */
		
		ApplyMatrix(lf->sector->ob->imat, lf->loc, lf->loc1);
		ApplyMatrix(lf->sector->ob->imat, lf->oldloc, lf->oldloc1);
		
		lf->speed1[0]= lf->loc1[0]-lf->oldloc1[0];
		lf->speed1[1]= lf->loc1[1]-lf->oldloc1[1];
		lf->speed1[2]= lf->loc1[2]-lf->oldloc1[2];
	}
}

void life_from_inv_sector_co(Life *lf)
{
	if(lf->sector) {
		/* transformeren vanuit locale coordinaten naar globale */
		
		/* eerst de (nieuwe) eindlocatie */
		lf->loc1[0]= lf->oldloc1[0]+lf->speed1[0];
		lf->loc1[1]= lf->oldloc1[1]+lf->speed1[1];
		lf->loc1[2]= lf->oldloc1[2]+lf->speed1[2];

		ApplyMatrix(lf->sector->ob->obmat, lf->loc1, lf->loc);
		ApplyMatrix(lf->sector->ob->obmat, lf->oldloc1, lf->oldloc);
		if(lf->collision) ApplyMatrix(lf->sector->ob->obmat, lf->colloc, lf->colloc);
		
		lf->speed[0]= lf->loc[0]-lf->oldloc[0];
		lf->speed[1]= lf->loc[1]-lf->oldloc[1];
		lf->speed[2]= lf->loc[2]-lf->oldloc[2];
	}
}

void aerodynamics(Object *ob)
{
	Life *lf;
	float mat[3][3], no[3], eul[3], ang;
	
	lf= ob->data;
	
	ang= lf->speed[0]*ob->obmat[2][0] + lf->speed[1]*ob->obmat[2][1] + lf->speed[2]*ob->obmat[2][2];
	if(ang<0.001) return;	
	
	Crossf(no, lf->speed, (float *)ob->mat[2]);
	

	/* met euler proberen! */
	
	
	Mat3ToEul(mat, eul);
	
	/* beetje interpoleren */
	
}

/* ***************** HET HELE PIPO SPUL ********************* */
/* *****************  (realtime ipo's)  ********************* */


pIpo *make_ob_pipo(Object *ob)
{
	pIpo *pipo;
	ListBase ipokey;
	IpoKey *ik;
	IpoCurve *icu;
	rctf bb;
	float cfra, *fpoin, *colpoin;
	int a, b, nr_elems, type, elemsize, sta, nr_keys=0;
	short *sp;
	
	/* 50 Hz!!! */
	/* elemsize is in float-eenheden, scheelt casten */
	
	if(ob->ipo->curve.first==0) return 0;

	if(ob->type==OB_SECTOR) {
		Sector *se= ob->data;
		colpoin= &se->r;
	}
	else if(ob->type==OB_LIFE) {
		Life *lf= ob->data;
		colpoin= &lf->r;
	}
	else colpoin= 0;
	
	/* de keys tellen */
	ipokey.first= ipokey.last= 0;
	make_ipokey_spec(&ipokey, ob->ipo);

	ik= ipokey.first;
	while(ik) {
		nr_keys++;
		ik= ik->next;
	}
	if(nr_keys<2) nr_keys= 0;
	
	/* */
	boundbox_ipo(ob->ipo, &bb);
	
	/* type en elemsize */
	type= 0;
	icu= ob->ipo->curve.first;
	while(icu) {
		if(icu->adrcode>=OB_LOC_X && icu->adrcode<=OB_DLOC_Z) type |= IP_LOC;
		else if(icu->adrcode>=OB_ROT_X && icu->adrcode<=OB_DROT_Z) type |= IP_ROT;
		else if(icu->adrcode>=OB_SIZE_X && icu->adrcode<=OB_SIZE_Z) type |= IP_SIZE;
		else if(icu->adrcode>=OB_COL_R && icu->adrcode<=OB_COL_B && colpoin) type |= IP_OBCOL;
		icu= icu->next;
	}
	elemsize= 0;
	if(type & IP_OBCOL) elemsize+=3;
	if(type & IP_LOC) elemsize+=3;
	if(type & IP_ROT) elemsize+=3;
	if(type & IP_SIZE) elemsize+=3;
	
	if(elemsize==0) return 0;

	/* maken */	
	nr_elems= 2*ffloor( bb.xmax-bb.xmin + 0.5) + 1;
	sta= 2*ffloor(bb.xmin+0.5);
	if(nr_elems<1) return 0;
	
	pipo= callocN( sizeof(pIpo) + nr_elems*elemsize*sizeof(float) + nr_keys*sizeof(short), "pipo");
	pipo->type= type;
	pipo->nr_elems= nr_elems;
	pipo->elemsize= elemsize;
	pipo->nr_keys= nr_keys;
	pipo->sta= sta;
	fpoin= (float *)(pipo+1);
	
	a= nr_elems;

	cfra= bb.xmin;
	while(a--) {
		
		calc_ipo(ob->ipo, cfra);
		execute_ipo((ID *)ob, ob->ipo);
		
		if(type & IP_OBCOL) {
			for(b=0; b<3; b++) fpoin[b]= colpoin[b];
			fpoin+= 3;
		}
		if(type & IP_LOC) {
			for(b=0; b<3; b++) fpoin[b]= ob->loc[b]+ob->dloc[b];
			fpoin+= 3;
		}
		if(type & IP_ROT) {
			for(b=0; b<3; b++) fpoin[b]= ob->rot[b]+ob->drot[b];
			fpoin+= 3;
		}
		if(type & IP_SIZE) {
			for(b=0; b<3; b++) fpoin[b]= ob->size[b]+ob->dsize[b];
			fpoin+= 3;
		}
		
		cfra+= 0.5;
		CLAMP(cfra, bb.xmin, bb.xmax);
	}

	/* keys doen */
	if(nr_keys) {
		sp= (short *)fpoin;
		ik= ipokey.first;
		while(ik) {
			*sp= ffloor(2.0*ik->val + 0.5);
			sp++;
			ik= ik->next;
		}
	}
	free_ipokey(&ipokey);

	/* restore */
	do_ob_ipo(ob);
	
	return pipo;
}

void convert_ipo(Object *ob)
{
	/* van gewone ipo naar realtime-systeem */
	pIpo *pipo;
	ListBase *ipobase=0;
	
	if(ob->type==OB_SECTOR) {
		Sector *se= ob->data;
		ipobase= &se->ipo;
	}
	else if(ob->type==OB_LIFE) {
		Life *lf= ob->data;
		ipobase= &lf->ipo;
	}
	
	if(ipobase==0) return;
	
	if(ob->ipo) {
		pipo= make_ob_pipo(ob);
		if(pipo) addtail(ipobase, pipo);
	}
	
}

void do_obipo(Object *ob, short cur, short delta, float *speed)	/* delta is ook flag! */
{
	/* alleen de realtime versies */
	Life *lf;
	pIpo *pipo=0;
	float *fpoin, *first, *colpoin, *rotpoin;
	int icur;
	char *poin;
	
	if(ob->type==OB_SECTOR) {
		Sector *se= ob->data;
		pipo= se->ipo.first;
		colpoin= &se->r;
	}
	else if(ob->type==OB_LIFE) {
		lf= ob->data;
		pipo= lf->ipo.first;
		colpoin= &lf->r;
	}

	/* dit is het geval bij dyna's */
	if(speed==0) {
		rotpoin= ob->rot;
	}
	else rotpoin= lf->rot;

	while(pipo) {
		if(pipo->type<=IP_FROMOB) {
			
			icur= cur - pipo->sta;
			
			if(delta==0) {		/* standaard ipo */
			
				CLAMP(icur, 0, pipo->nr_elems-1);
				fpoin= ((float *)(pipo+1))+icur*pipo->elemsize;
				
				if(pipo->type & IP_OBCOL) {
					VECCOPY(colpoin, fpoin);
					fpoin+= 3;
				}
				if(pipo->type & IP_LOC) {
					VECCOPY(ob->loc, fpoin);
					fpoin+= 3;
				}
				if(pipo->type & IP_ROT) {
					VECCOPY(ob->rot, fpoin);
					fpoin+= 3;
				}
				if(pipo->type & IP_SIZE) {
					VECCOPY(ob->size, fpoin);
				}
				
			}
			else {				/* delta ipo */

				/*  deze test is niet overbodig: interval sensors != interval ipo */
				if(delta==1 && (icur<=0 || icur>= pipo->nr_elems));
				else if(delta== -1 && (icur<0 || icur>= pipo->nr_elems-1));
				else {
					
					fpoin= ((float *)(pipo+1))+icur*pipo->elemsize;
					first= fpoin - delta*pipo->elemsize;
				
					if(pipo->type & IP_OBCOL) {
						colpoin[0]+= fpoin[0] - first[0];
						colpoin[1]+= fpoin[1] - first[1];
						colpoin[2]+= fpoin[2] - first[2];
						fpoin+= 3; first+= 3;
					}
					if(pipo->type & IP_LOC) {
						if(speed) {
							speed[0]+= fpoin[0] - first[0];
							speed[1]+= fpoin[1] - first[1];
							speed[2]+= fpoin[2] - first[2];
						}
						else {
							ob->loc[0]+= fpoin[0] - first[0];
							ob->loc[1]+= fpoin[1] - first[1];
							ob->loc[2]+= fpoin[2] - first[2];
						}
						fpoin+= 3; first+= 3;
					}
					if(pipo->type & IP_ROT) {
						rotpoin[0]+= fpoin[0] - first[0];
						rotpoin[1]+= fpoin[1] - first[1];
						rotpoin[2]+= fpoin[2] - first[2];
						fpoin+= 3; first+= 3;
					}
					if(pipo->type & IP_SIZE) {
						ob->size[0]+= fpoin[0] - first[0];
						ob->size[1]+= fpoin[1] - first[1];
						ob->size[2]+= fpoin[2] - first[2];
					}				
				}
				
			}
			
		}
		pipo= pipo->next;
	}
	
}

void do_force_obipo(Object *ob, short cur, float *force, float *omega)
{
	/* alleen de realtime versies */
	pIpo *pipo=0;
	float *fpoin, *first, *colpoin;
	int icur;
	
	if(ob->type==OB_LIFE) {
		Life *lf= ob->data;
		pipo= lf->ipo.first;
		colpoin= &lf->r;
	}

	while(pipo) {
		if(pipo->type<=IP_FROMOB) {
			
			icur= cur - pipo->sta;
			
			if(icur<=0 || icur>= pipo->nr_elems);
			else {
				
				fpoin= ((float *)(pipo+1))+icur*pipo->elemsize;
				first= fpoin - pipo->elemsize;
			
				if(pipo->type & IP_OBCOL) {
					VECCOPY(colpoin, fpoin);
					fpoin+= 3; first+= 3;
				}
				if(pipo->type & IP_LOC) {
					force[0]+= fpoin[0] - first[0];
					force[1]+= fpoin[1] - first[1];
					force[2]+= fpoin[2] - first[2];
					fpoin+= 3; first+= 3;
				}
				if(pipo->type & IP_ROT) {
					omega[0]+= fpoin[0] - first[0];
					omega[1]+= fpoin[1] - first[1];
					omega[2]+= fpoin[2] - first[2];
				}
			}
		}
		pipo= pipo->next;
	}
}

/* return 1: einde bereikt */
int set_k2k_interval(short mode, Action *ac, Life *lf)
{
	pIpo *pipo;
	short a, *sp;
	

	if(ac->action==SN_K2K_OBIPO) {
		pipo= lf->ipo.first;
		while(pipo) {
			if(pipo->type<=IP_FROMOB) {
				if(pipo->nr_keys) {
					sp=  (short *)(((float *)(pipo+1))+ pipo->nr_elems*pipo->elemsize);
					
					if(mode==0) {		/* forw first */
						ac->sta= sp[0];
						ac->end= sp[1];
						lf->cfra= ac->sta;
					}
					else if(mode==2) {		/* backw last */
						sp += (pipo->nr_keys-1);
						ac->sta= sp[0];
						ac->end= sp[-1];
						lf->cfra= ac->sta;
					}
					else if(mode==1) {	/* next */
					
						a= pipo->nr_keys-1;
						while(a--) {
							if(sp[0]==lf->cfra) {
								ac->sta= sp[0];
								ac->end= sp[1];
								return 0;
							}
							sp++;
						}
						/* we zijn hier: eind bereikt */
						if(lf->cfra==sp[0]) return 1;
						/* of niets te vinden */
						return 2;
						
					}
					else if(mode== -1) {	/* prev */
					
						a= pipo->nr_keys-1;
						
						/* kan niet verder terug */
						if(lf->cfra==sp[0]) return 1;
						
						sp++;
						while(a--) {
							if(sp[0]==lf->cfra) {
								ac->sta= sp[0];
								ac->end= sp[-1];
								return 0;
							}
							sp++;
						}
						/* we zijn hier: niets te vinden */
						return 2;
					}
				}
			}
			pipo= pipo->next;
		}
	}
	
	return 0;
}



/* *********************** END IPO ******************************** */
/* ************************* VARS ********************************* */

#define MAXACTVAR 100
int totactionvar=0;

typedef struct actvar {
	struct actvar *next, *prev;
	Object *ob;			/* bij lokale variable */
	char *name;
	short *poin;		/* staat op ->var of op life */
	short nr, var, flag, rt;	
} actvar;

actvar actvarar[MAXACTVAR];


void test_action_var(Object *ob, Action *actest)
{
	Life *lf;
	Sensor *sn;
	Action *ac;
	int a, b;
	
	/* vanuit buttons: is actest->name reeeds in gebruik? dan intwaarde en minmax kopieeren */
	
	lf= G.main->life.first;
	while(lf) {
		sn= lf->sensors;
		a= lf->totsens;
		while(a--) {
			ac= sn->actions;
			b= sn->totaction;
			while(b--) {
				if(ac!=actest) {
					if(ac->action>=300 && ac->action<400) {
						
						if(ac->name[0] && strcmp(ac->name, actest->name)==0 ) {

							if(ac->name[0]!='L' || lf==ob->data) {
								actest->sta= ac->sta;
								actest->end= ac->end;
								actest->cur= ac->cur;
								break;
							}
						}
					}
				}
				ac++;
			}
			sn++;
		}
		lf= lf->id.next;
	}
}

void copy_action_var(Object *ob, Action *actest)
{
	Life *lf;
	Sensor *sn;
	Action *ac;
	int a, b;
	
	/* kopieer actest->name naar alle met dezelfde naam */
	
	lf= G.main->life.first;
	while(lf) {
		sn= lf->sensors;
		a= lf->totsens;
		while(a--) {
			ac= sn->actions;
			b= sn->totaction;
			while(b--) {
				if(ac!=actest) {
					if(ac->action>=300 && ac->action<400) {
						if(ac->name[0] && strcmp(ac->name, actest->name)==0 ) {
						
							if(ac->name[0]!='L' || lf==ob->data) {
								ac->sta= actest->sta;
								ac->end= actest->end;
								ac->cur= actest->cur;
								ac->flag= actest->flag;
							}
						}
					}
				}
				ac++;
			}
			sn++;
		}
		lf= lf->id.next;
	}
}

/* klein probleempje: de Locale vars krijgen ook een plek in het globale array. Bytes verspild! */

void add_action_var(Object *ob, Action *ac)
{
	Life *lf;
	actvar *av;
	int a;
	
	if(ac->name[0]==0) return;
	lf= ob->data;
	
	av= actvarar;
	for(a=0; a<totactionvar; a++, av++) {
		if(strcmp(ac->name, av->name)==0) {
			if(ac->name[0]!='L' || ob==av->ob || av->ob==0) {
			
				ac->poin= av->poin;
				ac->varnr= av->nr;
				
				return;
			}
		}
	}
	/* niets gevonden: */
	if(totactionvar>=MAXACTVAR-1) {
		printf("Action Variable beyond max: %s\n", ac->name);
		return;
	}
	
	av->name= ac->name;

	if(av->name[0]=='L') {
		if(strcmp(av->name, "Lstate")==0) {
			ac->varnr= -LF_STATE;
			ac->poin= lf->state+LF_STATE-1;
			av->ob= ob;
		}
		else if(strcmp(av->name, "Ldamage")==0) {
			ac->varnr= -LF_DAMAGE;
			ac->poin= lf->state+LF_DAMAGE-1;
			av->ob= ob;
		}
		else if(strcmp(av->name, "Laction")==0) {
			ac->varnr= -LF_ACTION;
			ac->poin= lf->state+LF_ACTION-1;
			av->ob= ob;
		}
		else if(strcmp(av->name, "Lframe")==0) {
			ac->varnr= -LF_FRAME;
			ac->poin= lf->state+LF_FRAME-1;
			av->ob= ob;
		}
		else {
			ac->varnr= a;			/* voor psx file */
			ac->poin= &av->var;
			av->ob= 0;
		}
	}
	else {
		ac->varnr= a;			/* voor psx file */
		ac->poin= &av->var;
		av->ob= 0;
	}
	
	av->poin= ac->poin;
	*((short *)ac->poin)= ac->cur;	/* initwaarde */
	av->nr= ac->varnr;
	av->flag= ac->flag;
	totactionvar++;
	
}

void actvars_to_vars(short *vars)	/* voor writeblendpsx */
{
	int a=MAXACTVAR;
	
	while(a--) {
		vars[a]= actvarar[a].var;
	}
}

/* *********************** end VARS ********************* */
/* *********************** DEBUG ********************* */

int dbswaptime;


typedef struct ActionDebug {
	
	struct ActionDebug *next, *prev;
	Action *ac;
	int timer;
	
} ActionDebug;

ListBase ADbase= {0, 0};

void add_action_debug(Action *ac, int timer)
{
	ActionDebug *ad;
	
	ad= mallocN(sizeof(ActionDebug), "adbug");
	addtail(&ADbase, ad);
	ad->ac= ac;
	ad->timer= timer;
}

void draw_gamedebug_info()
{
	ActionDebug *ad, *adn;
	actvar *av;
	Life *lf;
	Action *ac;
	int a, yco;
	char str[256];
	
	persp(0);
	cpack(0xFFFFFF);
	fmsetfont(G.font);

	glRasterPos2s(curarea->winx/2 -40, curarea->winy-14);
	sprintf(str, "swap: %d", dbswaptime);
	fmprstr(str);

	if(G.vd->drawtype==OB_TEXTURE) return;

	a= 0;
	ad= ADbase.first;
	while(ad) {
		adn= ad->next;
		ad->timer--;
		
		if(ad->timer==0) {
			remlink(&ADbase, ad);
			freeN(ad);
		}
		else {
			a++;
			glRasterPos2s(curarea->winx-150, curarea->winy-14*a);

			switch(ad->ac->action) {
			case SN_LOADFILE:
				fmprstr("LOAD: ");
				fmprstr(ad->ac->name);
				break;
			case SN_PLAYMOVIE:
				fmprstr("PLAY MOVIE: ");
				fmprstr(ad->ac->name);
				break;
			default:
				sprintf(str, "Action %d\n", ad->ac->action);
				fmprstr(str);
			}
		}
		
		ad= adn;
	}

	av= actvarar;
	for(yco=a=1; a<=totactionvar; a++, av++) {
		if(av->flag & SN_VAR_NOPRINT);
		else {
			glRasterPos2s(10, curarea->winy-14*yco);
			
			fmprstr(av->name);
			if(av->ob) sprintf(str , "  %d (%s)", *av->poin, av->ob->id.name+2);
			else sprintf(str , "  %d", av->var);
			fmprstr(str);
			yco++;
		}
	}
	
	a++;
	
	lf= G.main->life.first;
	while(lf) {
		if(lf->timer>=0) {
			
			glRasterPos2s(10, 14*a);
			a++;

			fmprstr(lf->id.name+2);
			sprintf(str , "Timer:  %d", lf->timer/2);
			fmprstr(str);
		}

		lf= lf->id.next;
	}
	
	
	persp(1);	
}

/* ******************************* END VAR ************************** */

/* NOTITIE: wat te doen met layers? */


void init_lifes()		/* bij start simulation */
{
	extern Object *main_actor;
	Life *lf;
	Base *base;
	Object *ob, *par;
	Sensor *sn;
	Event *ev;
	Action *ac;
	Sector *se;
	Portal *po;
	float temp, fvec[3];
	int a, b, c;
	
	G.totlife= 0;
	totactionvar= 0;
	main_actor= 0;
	
	/* fmodf(0.2, 2PI) is 0.2
	 * fmodf(-0.2, 2PI) is -0.2
	 * de integerversie is '%'
	 */

	lf= G.main->life.first;
	while(lf) {
		lf->sector= 0;
		lf->timer= -1;
		lf->contact= 0;
		lf->collision= 0;
		lf->floor= 0;
		lf->dflag= 0;
		lf->frictfac= 1.0;
		lf->oldmesh= lf->texmesh;
		
		lf->links.ob= 0;
		lf->links.tot= 0;

		lf= lf->id.next;
	}

	base= FIRSTBASE;
	while(base) {
		ob= base->object;
		if(ob==G.scene->camera) {
			if(ob->parent && ob->parent->type==OB_LIFE) {
				
				par= ob->parent;
				while(par && par->parent) par= par->parent;
				
				lf= par->data;
				add_to_lbuf(&(lf->links), ob);
			}
			else add_dyna_life(base->object);	/* uitzondering */
		}
		else if(base->object->type==OB_LIFE) {
			
			
			lf= ob->data;
			
			if(lf->type==LF_DYNAMIC && (lf->flag & LF_MAINACTOR)) {
				if(main_actor==0) main_actor= ob;
				else if(lf->flag & LF_CAMERA_ACTOR) main_actor= ob;
			}

			/* fixpoint overflows voorkomen */
			ob->rot[0]= fmodf(ob->rot[0], 2.0*M_PI);
			ob->rot[1]= fmodf(ob->rot[1], 2.0*M_PI);
			ob->rot[2]= fmodf(ob->rot[2], 2.0*M_PI);

			lf->speed[0]= lf->speed[1]= lf->speed[2]= 0.0;
			lf->rotspeed[0]= lf->rotspeed[1]= lf->rotspeed[2]= 0.0;
			
			VECCOPY(lf->startloc, ob->loc);		/* enkel als 'undo?' */
			VECCOPY(lf->startrot, ob->rot);
			
			VECCOPY(lf->loc, ob->obmat[3]);	/* !!! */
			VECCOPY(lf->rot, lf->startrot);
			
			VECCOPY(lf->oldloc, lf->loc);
			
			convert_ipo(ob);
				
			do_obipo(ob, 2*lf->sfra, 0, 0);
			
			sn= lf->sensors;
			a= lf->totsens;
			while(a--) {
				
				/* init events: variable events onderin! */
				sn->event= sn->evento= 0;
				
				ev= sn->events;
				b= sn->totevent;
				while(b--) {

					if(ev->flag & SN_NOT) ev->shiftval= -1;
					else ev->shiftval= 0;
					
					ev->poin= 0;
					if(ev->name[0]) {
						if(ev->event==SN_CONTACT) ev->poin= find_id("MA", ev->name);
						else if ELEM3(ev->event, SN_COLLISION, SN_NEAR, SN_TRIPFACE) ev->poin= find_id("OB", ev->name);
					}
					
					if(ev->event==SN_NEAR) {
						/* mindist en enddist */
						if(ev->fac1<ev->fac) ev->fac1= ev->fac;
						/* near flag */
						ev->var= 0;	
					}
					else if(ev->event==SN_COLLISION) {
						ev->var= 0;
					}
					
					ev++;
				}
				
				/* init actions */
				ac= sn->actions;
				b= sn->totaction;
				while(b--) {
				
					if(ac->action<100) {
						/* min max? */
						ac->cur= 0;
						if ELEM3(ac->action, SN_X_ROT, SN_Y_ROT, SN_Z_ROT) lf->dflag |= LF_OMEGA;
					}
					else if(ac->action<200) {
						
						/* ac->go  bepaalt of er aktie ondernomen wordt */ 
						ac->go= 0;
						ac->sta= 2*ac->butsta;
						ac->end= 2*ac->butend;
					

						if(ac->action == SN_DELTA_OBIPO) {
							ac->cur= ac->sta;
						}
						else if(ac->action == SN_PLAY_OBIPO) {
							ac->cur= ac->sta;
						}
						else if ELEM(ac->action, SN_LOOPSTOP_OBIPO, SN_LOOPEND_OBIPO) {
							ac->cur= ac->end;
						}
						else if ELEM(ac->action, SN_PINGPONG_OBIPO, SN_FLIPPER_OBIPO) {
							ac->cur= ac->sta;
						}
						else if(ac->action==SN_K2K_OBIPO) {
							if(ac->var & SN_K2K_PREV) set_k2k_interval(2, ac, lf);
							else set_k2k_interval(0, ac, lf);
							
							ac->cur= ac->sta;
						}
						
						ac->poin= 0;
						if(ac->name[0]) {
							/* even niet: probelemen met DYNA_CHANGED */
							/* ac->poin= find_id("OB", ac->name); */
							if(ac->poin) {
								if( ((Object *)ac->poin)->type!=OB_LIFE ) ac->poin= 0;
							}
						}
						
						if(ac->flag & SN_IPO_SETVAR) {
							add_action_var(ob, ac);
							if(ac->poin) *((short *)ac->poin)= ac->cur/2;
							else error("undefined variable");
						}
						
					}
					else if(ac->action<300) {
						if(ac->action==SN_LAYERMOVE) {
							ac->sta= base->lay;
							ac->end= ac->butend;
						}
						else if(ac->action==SN_STARTTIMER) {
							ac->go= 0;
							ac->sta= 2*ac->butsta;
						}
						else if(ac->action==SN_ADDLIFE || ac->action==SN_REPLACELIFE) {
							ac->poin= find_id("OB", ac->name);
							ac->sta= 2*ac->butsta;
						}
						else if(ac->action==SN_VISIBLE) {
							ac->go= 0;
							ac->cur= 0;
							ac->sta= 2*ac->butsta;
							lf->dflag |= LF_DONTDRAW;
						}
						else if(ac->action==SN_REPLACEMESH) {
							ac->poin= find_id("ME", ac->name);
							ac->sta= 2*ac->butsta;
						}
						else if(ac->action==SN_GOTO) {
							ac->poin= find_id("OB", ac->name);
						}
						else if(ac->action==SN_SETCAMERA) {
							ac->poin= find_id("OB", ac->name);
						}
						else if(ac->action==SN_TRACKTO) {
							ac->poin= find_id("OB", ac->name);
							ac->sta= 2*ac->butsta;
							/* voor backwards compatibility */
							if(ac->cur!=0 && ac->cur!=1) ac->cur= 0;
						}
						else if(ac->action==SN_PLAYMOVIE) {
							Scene *sce;
							
							ac->poin= find_id("OB", ac->name);
							ac->sta= 2*ac->butsta;
							ac->end= 2*ac->butend;
						}
						else if(ac->action==SN_ROBBIE_M) {
							ac->poin= find_id("OB", ac->name);
							ac->fac= 0.0;	/* corr */
						}
						
					}
					else if(ac->action<400) {
						add_action_var(ob, ac);
					}
					ac++;
				}
				sn++;
			}
			
			/* anders dubbelop */
			/* bzero(ob->dloc, 12); */
			/* bzero(ob->drot, 12); */
			
			where_is_object_simul(ob);
			Mat4Invert(ob->imat, ob->obmat);
			Mat4CpyMat4(lf->oldimat, ob->imat);
			
			if(lf->dynamesh==0);
			else if(lf->dynamesh->flag & ME_ISDONE);
			else {
				init_dynamesh(ob, lf->dynamesh);
				lf->dynamesh->flag |= ME_ISDONE;
			}
			
			if(lf->type!=LF_LINK) {
				lf->sector= find_sector(lf->loc, lf->loc1);
				life_to_local_sector_co(lf);
			}
			
			/* alleen zichtbare, ivm ADDLIFE */
			if(base->lay & G.scene->lay ) {
				
			
				if(lf->type==LF_DYNAMIC) {
					if(lf->flag & LF_MAINACTOR) add_dyna_life(ob);
					else if(lf->sector) add_to_lbuf(&(lf->sector->lbuf), ob);
				}
				else if(lf->type==LF_LINK) {
					/* o.a. voor robbie */
					Object *par;
					Life *lfp;
					
					if(lf->flag & LF_MAINACTOR) add_dyna_life(ob);
					else {
						par= ob->parent;
						while(par && par->parent) par= par->parent;
						if(par && par->type==OB_LIFE) {
							lfp= par->data;
						
							add_to_lbuf(&(lfp->links), ob);
						}
						else {
							errorstr("Link has no or wrong parent", ob->id.name+2, 0);
						}
					}
				}
				else if(lf->sector && lf->type==LF_PROP) {
				
					add_to_lbuf(&(lf->sector->lbuf), ob);
					
					/* meerdere sectoren? */
					po= lf->sector->portals;
					a= lf->sector->totport;
					
					while(a--) {
						if(po->sector) {
							if( sector_intersect(po->sector, ob)) {
								add_to_lbuf(&(po->sector->lbuf), ob);
							}
						}
						po++;
					}
				}
			}
		}
		base= base->next;
	}
	
	if(main_actor==0) error("no main actor");
	else {
		lf= main_actor->data;
		/* voorkom ongewenste combinmatie! */
		lf->flag |= LF_CAMERA_ACTOR;
	}
	
	/* variable event pointers */
	base= FIRSTBASE;
	while(base) {
		if(base->object->type==OB_LIFE) {
			ob= base->object;
			lf= ob->data;
		
			sn= lf->sensors;
			a= lf->totsens;
			while(a--) {
				ev= sn->events;
				b= sn->totevent;
				while(b--) {
					/* ook in writeblendpsx.c */
					if ELEM3(ev->event, SN_VAR_EQUAL, SN_VAR_INTERVAL, SN_VAR_CHANGED) {
						ev->poin= 0;

						c= totactionvar;
						while(c--) {
							if(strcmp(ev->name, actvarar[c].name)==0) {
								if(ev->name[0]!='L' || ob==actvarar[c].ob) {
									
									ev->varnr= actvarar[c].nr;
									ev->poin= actvarar[c].poin;
									
								}
							}
						}

					}
					if(ev->poin && ev->event==SN_VAR_CHANGED) {
						ev->var= *((short *)ev->poin);
					}
					ev++;
				}
				sn++;
			}
		}
		base= base->next;
	}
}

void end_lifes(int restore)
{
	Life *lf;
	Sensor *sn;
	Action *ac;
	Event *ev;
	Base *base;
	Object *ob;
	int a, b;
	
	del_dupli_lifes();

	base= FIRSTBASE;
	while(base) {
		if(base->object->type==OB_LIFE) {
			ob= base->object;
			
			ob->ctime= 0.0;	/* forceer herberekening */
			ob->lay= base->lay;	/* kan verzet zijn */
			
			lf= ob->data;
			
			if(restore || lf->type==LF_PROP) {
				VECCOPY(ob->loc, lf->startloc);
				VECCOPY(ob->rot, lf->startrot);
				where_is_object(ob);
			}
			else {
				VecSubf(ob->loc, ob->loc, ob->dloc);
				
			}
			end_dynamesh(lf->dynamesh);
			lf->texmesh= lf->oldmesh;
			lf->contact= 0;
			lf->collision= 0;
			lf->floor= 0;
			
			lf->floorloc[0]= 0.0;
			lf->floorloc[1]= 0.0;
			lf->floorloc[2]= 0.0;
			
			freelistN(&lf->ipo);

			free_lbuf(&lf->links);

			sn= lf->sensors;
			a= lf->totsens;
			while(a--) {
				
				ev= sn->events;
				b= sn->totevent;
				while(b--) {
					ev->poin= 0;
					ev++;
				}
				
				ac= sn->actions;
				b= sn->totaction;
				while(b--) {
					
					ac->poin= 0;
					
					if(ac->action<100);
					else if(ac->action<200) {
						if(ac->action==SN_K2K_OBIPO) {
							if(ac->var & SN_K2K_PINGPONG) ac->var &= ~SN_K2K_PREV;
						}
					}
					ac++;
				}
				sn++;
			}
			
		}
		base= base->next;
	}
	
	freelistN(&ADbase);
	do_realtimelight(0, 0, 0);	/*re-init */
}

/* *********************SENSORS ******************* */

/*
 * 
 * 
 */

#define TOTKEY	19

void eventnames_to_pupstring(char **str)
{

	
	*str= mallocN(1000, "names_pup");
	(*str)[0]= 0;
	
	strcat(*str, "ALWAYS%x31|Near%x64|Contact%x65|Timer%x66|Collision%x81|");
	strcat(*str, "Var Equal%x67|Var Interval%x68|Var Changed%x69|");
	strcat(*str, " %x0|");
	
	strcat(*str, "Shift L%x1|Shift R%x2|");
	strcat(*str, "Ctrl L%x3|Ctrl R%x4|");
	strcat(*str, "Alt L%x5|Alt R%x6|");
	strcat(*str, "Arrow L%x7|Arrow R%x8|");
	strcat(*str, "Arrow Up%x9|Arrow Dn%x10|");
	strcat(*str, "Pad L%x11|Pad R%x12|");
	strcat(*str, "Pad Up%x13|Pad Dn%x14|");
	strcat(*str, "Page Up%x15|Page Dn%x16|");
	strcat(*str, "Insert%x17|Delete%x18");
	
}

void actionnames_to_pupstring(char **str)
{

	
	*str= mallocN(1000, "action_pup");
	(*str)[0]= 0;
	
	strcat(*str, "TRANS X%x2|TRANS Y%x4|TRANS Z%x6|");
	strcat(*str, "ROT X%x8|ROT Y%x10|ROT Z%x12|");
	strcat(*str, "Goto %x208|Track To %x213|Add Damage %x214|No Damage %x215|");
	strcat(*str, "Robbie M %x216|");
	strcat(*str, " %x0|");
	strcat(*str, "Delta ObIpo %x103|");
	strcat(*str, "Play ObIpo %x100|");
	strcat(*str, "PingPong ObIpo %x101|");
	strcat(*str, "Flipper ObIpo %x102|");
	strcat(*str, "LoopStop ObIpo %x104|");
	strcat(*str, "LoopEnd ObIpo %x105|");
	strcat(*str, "Key2Key ObIpo %x106|");
	strcat(*str, " %x0|");
	strcat(*str, "Layer Move %x200|StartTimer %x201|");
	strcat(*str, "Add Life %x202|Replace Life %x203|Replace Mesh %x210|End Life %x209|");
	strcat(*str, "Load File %x204|Restart %x205|Quit %x206|Visible %x207|");
	strcat(*str, "Set Camera %x211|Play Movie %x212");
	strcat(*str, " %x0|");
	strcat(*str, "Variable Set %x300|Variable Add %x301|Variable Tog %x302|");
	strcat(*str, "Variable Hold %x304|Variable to ObIpo%x303");
	
}


void init_devs()
{
	/* indexnummers zijn gelijk aan sensor-eventnummers */
	
	simuldevs[0]= ESCKEY;

	simuldevs[1]= LEFTSHIFTKEY;
	simuldevs[2]= RIGHTSHIFTKEY;
	simuldevs[3]= LEFTCTRLKEY;
	simuldevs[4]= RIGHTCTRLKEY;
	simuldevs[5]= LEFTALTKEY;
	simuldevs[6]= RIGHTALTKEY;

	simuldevs[7]= LEFTARROWKEY;
	simuldevs[8]= RIGHTARROWKEY;	
	simuldevs[9]= UPARROWKEY;
	simuldevs[10]= DOWNARROWKEY;

	simuldevs[11]= PAD4;
	simuldevs[12]= PAD6;	
	simuldevs[13]= PAD8;
	simuldevs[14]= PAD2;

	simuldevs[15]= PAGEUPKEY;
	simuldevs[16]= PAGEDOWNKEY;	
	simuldevs[17]= INSERTKEY;
	simuldevs[18]= DELKEY;
	
	simuldevs[31]= 1;
	
	bzero(simulvals, 32*2);
	
	simulvals[31]= 1;	/* always */
}


void add_sensor(Life *lf)
{
	Sensor *sn;	
	
	/* aktieve sensor: dupli */
	add_array_element((void **)&lf->sensors, &lf->actsens, &lf->totsens, sizeof(Sensor));
	
	/* nieuwe actieve sensor: de event en action arrays testen */
	sn= lf->sensors+lf->actsens-1;
	if(sn->events) sn->events= dupallocN(sn->events);
	else {
		sn->events= callocN(sizeof(Event), "new event");
		sn->totevent= 1;
	}
	if(sn->actions) sn->actions= dupallocN(sn->actions);
	else {
		sn->actions= callocN(sizeof(Action), "new action");
		sn->actions->fac= sn->actions->max= 1.0;
		sn->totaction= 1;
	}
}

void delete_sensor(Life *lf)
{
	Sensor *sn;
	
	if(lf->totsens==0) return;
	/* aktieve sensor: del */
	
	sn= lf->sensors+lf->actsens-1;
	if(sn->events) freeN(sn->events);
	sn->totevent= 0;
	if(sn->actions) freeN(sn->actions);
	sn->totaction= 0;
	
	delete_array_element((void **)&lf->sensors, &lf->actsens, &lf->totsens, sizeof(Sensor));
}



/* iets soortgelijks op de psx maken: combinatie van getbutton en qread */
/* het geheugen (hold, vorige stand) zit in sensors zelf */
short pad_read()
{
	short a, val;
	ushort event= 0;
	
	
	G.qual= 0;

/****** TON LOOK AT THIS *********/
#ifdef IRISGL
	/* events wissen, getbutton */
	
	for(a=0; a<TOTKEY; a++) {
		if(simulvals[a]) {
			if( getbutton(simuldevs[a])==0 ) simulvals[a]= 0;
			else G.qual= 1;
		}
	}
#endif
		
	/* queue lezen */
	while(qtest()) {
		
		event= special_qread(&val);
		
		if(event==SPACEKEY || event==ESCKEY) break;	

		if(event) {

			for(a=0; a<TOTKEY; a++) {
				if(simuldevs[a]==event) {

					simulvals[a] = val;
					G.qual= 1;
					
				}
			}
		}
	}
	return event;
}

void Mat3ToEulFast(mat, eul)
float mat[][3], *eul;
{
	float cy;
	
	cy = fsqrt(mat[0][0]*mat[0][0] + mat[0][1]*mat[0][1]);

	if (cy > 16.0*FLT_EPSILON) {
		eul[0] = fatan2(mat[1][2], mat[2][2]);
		eul[1] = fatan2(-mat[0][2], cy);
		eul[2] = fatan2(mat[0][1], mat[0][0]);
	} else {
		eul[0] = fatan2(-mat[2][1], mat[1][1]);
		eul[1] = fatan2(-mat[0][2], cy);
		eul[2] = 0.0;
	}
}



/* idee: deze op halve klok? */

void camera_behaviour(Object *cam, Life *lfcam, Action *ac)
{
	Object *actor;
	Life *lfactor;
	Portal *po;
	float *fp1, *fp2, mindistsq, maxdistsq;
	float inp, fac, distsq, mat[3][3], lookat[3], from[3], rc[3];
	short a, ok;
	
	actor= ac->poin;
	lfactor= actor->data;
	mindistsq= ac->min*ac->min;
	maxdistsq= ac->max*ac->max;

	/* init */
	lookat[0]= lfactor->loc[0];
	lookat[1]= lfactor->loc[1];
	lookat[2]= lfactor->loc[2];

	inp= MAX2(lfactor->floorloc[2], -5.0 * (lfactor->axsize));

	lookat[2] +=  inp;
	
	from[0]= cam->loc[0];
	from[1]= cam->loc[1];
	from[2]= cam->loc[2];

	/* CONSTRAINT 1: staat camera goed geroteerd in sector (90 graden grid)? */

	/* CONSTRAINT 2: kan cam actor zien? */
	/*				 niet vanuit schaduw!!! */
	ok= test_visibility(lfactor->loc, from, lfcam, lfactor->sector);

	/* if(ok==0 && lfactor->sector) { */
	/* 	a= lfactor->sector->totport; */
	/* 	po= lfactor->sector->portals; */
	/* 	while(a--) { */
	/* 		if( test_visibility(lfactor->loc, from, po->sector) ) break; */
	/* 		po++; */
	/* 	} */
	/* } */
	

	/* CONSTRAINT 3: vaste hoogte boven schaduw */
	if(ok==0) from[2]= (15.0*from[2] + lookat[2] + ac->force)/16.0;
	
	/* CONSTRAINT: achterliggende camera */
	if(ok==0 && G.qual==0) {
		fp1= actor->obmat[0];
		fp2= cam->obmat[0];

		inp= fp1[0]*fp2[0] + fp1[1]*fp2[1] + fp1[2]*fp2[2];
		fac= (-1.0 + inp)/64.0;

		from[0]+= fac*fp1[0];
		from[1]+= fac*fp1[1];
		from[2]+= fac*fp1[2];
		
		/* alleen alstie ervoor ligt: cross testen en loodrechte bijtellen */
		if(inp<0.0) {
			if(fp1[0]*fp2[1] - fp1[1]*fp2[0] > 0.0) {
				from[0]-= fac*fp1[1];
				from[1]+= fac*fp1[0];
			}
			else {
				from[0]+= fac*fp1[1];
				from[1]-= fac*fp1[0];
			}
		}
	}

	/* CONSTRAINT 4: minimum / maximum afstand */
	rc[0]= (lookat[0]-from[0]);
	rc[1]= (lookat[1]-from[1]);
	rc[2]= (lookat[2]-from[2]);
	distsq= rc[0]*rc[0] + rc[1]*rc[1] + rc[2]*rc[2];

	if(distsq > maxdistsq) {
		distsq = 0.25*(distsq-maxdistsq)/distsq;
		
		from[0] += distsq*rc[0];
		from[1] += distsq*rc[1];
		from[2] += distsq*rc[2];
	}
	else if(distsq < mindistsq) {
		distsq = 0.25*(mindistsq-distsq)/mindistsq;
		
		from[0] -= distsq*rc[0];
		from[1] -= distsq*rc[1];
		from[2] -= distsq*rc[2];
	}

	/* CONSTRAINT 4a: nog eens vaste hoogte boven schaduw */
	if(ok==0) from[2]= (15.0*from[2] + lookat[2] + ac->force)/16.0;

	/* CONSTRAINT 5: track naar schaduw */
	rc[0]= (lookat[0]-from[0]);
	rc[1]= (lookat[1]-from[1]);
	rc[2]= (lookat[2]-from[2]);
	VecUpMat3(rc, mat, 1);	/* Z up Track x */
	

	/* CONSTRAINT: klein beetje met aktie meekijken: projecteer x-vec op scherm? */

	fp1= actor->obmat[0];
	fp2= G.vd->viewinv[0];
		
	inp= 0.2*(fp2[0]*fp1[0] + fp2[1]*fp1[1] + fp2[2]*fp1[2]);

	ac->fac= (15.0*ac->fac + inp)/16.0;

	Mat3ToEulFast(mat, cam->rot);
	cam->rot[2]-= ac->fac;
	
	
	VECCOPY(cam->loc, from);

}


void compatible_eulFast(float *eul, float *oldrot)
{
	float dx, dy, dz;
	
	/* verschillen van ong 360 graden corrigeren */

	dx= eul[0] - oldrot[0];
	dy= eul[1] - oldrot[1];
	dz= eul[2] - oldrot[2];

	if( fabs(dx) > 5.1) {
		if(dx > 0.0) eul[0] -= 2.0*M_PI; else eul[0]+= 2.0*M_PI;
	}
	if( fabs(dy) > 5.1) {
		if(dy > 0.0) eul[1] -= 2.0*M_PI; else eul[1]+= 2.0*M_PI;
	}
	if( fabs(dz) > 5.1 ) {
		if(dz > 0.0) eul[2] -= 2.0*M_PI; else eul[2]+= 2.0*M_PI;
	}
}


void track_life_to_life(Life *this, Life *to, float fac, short mode)
{
	float vec[3], mat[3][3];
	
	vec[0]= to->loc[0] - this->loc[0];
	vec[1]= to->loc[1] - this->loc[1];
	vec[2]= to->loc[2] - this->loc[2];
	
	if(mode==0) {	/* alleen z-rot */
		vec[2]= fatan2(vec[1], vec[0]);
		vec[0]= vec[1]= 0;
	}
	else {
		VecUpMat3(vec, mat, 1);	/* Z up Track x */
		Mat3ToEulFast(mat, vec);
	}
	
	if(fac==0.0) {
		VECCOPY(this->rot, vec);
	}
	else {
		/* this rot aanpassen !!!*/
		compatible_eulFast(this->rot, vec);
		
		this->rot[0]= (fac*this->rot[0] + vec[0])/(1.0+fac);
		this->rot[1]= (fac*this->rot[1] + vec[1])/(1.0+fac);
		this->rot[2]= (fac*this->rot[2] + vec[2])/(1.0+fac);
	}
}

int dyna_near_life(Object *prob, float mindist, Object *actor)
{
	Life *lf;
	Object *ob;
	float *vec, *test, dist;
	int a;
	
	/* alle dyna lifes */
	test= prob->obmat[3];
	
	if(actor) {
		vec= actor->obmat[3];

		/**/
		dist= fabs(vec[0]-test[0]);
		if(dist<mindist) {
			dist= fabs(vec[1]-test[1]);
			if(dist<mindist) {
				dist= fabs(vec[2]-test[2]);
				if(dist<mindist) return 1;
			}
		}
	}
	else {
		a= G.totlife;
		while(a--) {
			ob= G.lifebuf[a];
			if( prob != ob && ob->type==OB_LIFE) {
				lf= ob->data;
				if(lf->type==LF_DYNAMIC && (lf->dflag & LF_TEMPLIFE)==0) {
					vec= ob->obmat[3];

					/**/
					dist= fabs(vec[0]-test[0]);
					if(dist<mindist) {
						dist= fabs(vec[1]-test[1]);
						if(dist<mindist) {
							dist= fabs(vec[2]-test[2]);
							if(dist<mindist) return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}


int sensor_input(Object *ob, Life *lf, float *forcetot, float *omegatot)
{
	extern char *simul_load_str;	/* sector.c */
	Sensor *sn;
	Event *ev;
	Action *ac;
	Object *pob, *victim=0;
	float omega[3], force[3], speed[3];
	short event, a, b, pulseval, retval=0, eventlock, mode;
	short *sp, cox;

	
	if(lf->type==LF_DYNAMIC) {
		omega[0]=omega[1]=omega[2]= 0.0;
		force[0]=force[1]=force[2]= 0.0;
		speed[0]=speed[1]=speed[2]= 0.0;
		lf->frictfac= 1.0;
	}
	else if(lf->type==LF_LINK) {
		omega[0]=omega[1]=omega[2]= 0.0;
	}
	if(lf->timer>=0) lf->timer--;

	sn= lf->sensors;
	a= lf->totsens;
	while(a--) {
		
		/* EVENTS */
		
		sn->evento= sn->event;
		
		b= sn->totevent;
		ev= sn->events;
		sn->event= 1;

		while(b--) {
			
			ev->shiftval <<= 1;
			event= 1;
			
			if(ev->event<32) {	/* globale events */
				event= simulvals[ev->event];
				
				if(ev->var==1) event= 0;
				if(ev->event==31) ev->flag= SN_HOLD;
			}
			else {
				switch(ev->event) {
				case SN_NEAR:
					/* ev->var is memory */

					if(ev->var) event= dyna_near_life(ob, ev->fac1, ev->poin);
					else event= dyna_near_life(ob, ev->fac, ev->poin);
					
					if(event) {
						ev->var= 1;
						lf->flag |= LF_DRAWNEAR;	/* alleen SGI */
						victim= ev->poin;
					}
					else {
						ev->var= 0;
						lf->flag &= ~LF_DRAWNEAR;
					}
					break;
					
				case SN_CONTACT:
					if(lf->contact==0) event= 0;
					else if(ev->poin && ev->poin!=lf->contact) event= 0;
					break;

				case SN_COLLISION:

					if(lf->collision==0 || ev->var>0) {
						event= 0;
						if(ev->var>0) ev->var--;
					}
					else if(ev->poin && lf->collision!=ev->poin) {
						event= 0;
						if(ev->var>0) ev->var--;
					}
					else {
						ev->var= ev->sfac;
						victim= lf->collision;
					}
					break;
					
				case SN_TIMER:
					/* timer in fields */
					if( (lf->timer>>1) != ev->var) event= 0;
					break;
				
				case SN_VAR_EQUAL:
					sp= ev->poin;
					if(sp==0 || *sp!=ev->var) event= 0;
					break;
				case SN_VAR_INTERVAL:
					sp= ev->poin;
					if(sp==0 || *sp<ev->sfac || *sp>ev->sfac1) event= 0;
					break;
				case SN_VAR_CHANGED:
					sp= ev->poin;
					if(sp) {
						if(*sp==ev->var) event= 0;
						else ev->var= *sp;
					}
					else event= 0;
					break;
				}
				
			}

			if(ev->flag & SN_NOT) event= 1-event;

			if(event) {
				ev->shiftval |= 1;
			
				if(ev->flag & SN_HOLD) {
					if(ev->pulse) {
						pulseval= (1<< (ev->pulse+1))-1;
						
						if( (ev->shiftval & pulseval)==pulseval ) ev->shiftval &= ~1;
						if((ev->shiftval & 3)!=1) sn->event= 0;
						
					}
				}
				else {
					if((ev->shiftval & 6)) sn->event= 0;	/* klein beetje fuzz */
				}
			}
			else sn->event= 0;
			
			ev++;
		}

		
		/* ACTIONS STARTEN */
		
		if(sn->event) {
		
			eventlock= 0;
			
			ac= sn->actions;
			b= sn->totaction;
			while(b--) {
			
				/* waarop: */
				switch(ac->action) {
				case SN_X_TRANS:
					force[0] += DTIME*ac->force;
					if(lf->frictfac>ac->fac) lf->frictfac= ac->fac;
					retval |= SN_CHANGED+SN_DOFORCE;
					break;
				case SN_Y_TRANS:
					force[1] += DTIME*ac->force;
					if(lf->frictfac>ac->fac) lf->frictfac= ac->fac;
					retval |= SN_CHANGED+SN_DOFORCE;
					break;
				case SN_Z_TRANS:
					force[2] += DTIME*ac->force;
					if(lf->frictfac>ac->fac) lf->frictfac= ac->fac;
					retval |= SN_CHANGED+SN_DOFORCE;
					break;
				
				case SN_GOTO:
					if(ac->poin) {
						Object *ob= ac->poin;
						ob->loc[0]= ac->force;
						ob->loc[1]= ac->min;
						ob->loc[2]= ac->max;
					}
					else {	
						lf->speed[0]= lf->speed[1]= lf->speed[2]= 0.0;
						force[0]= force[1]= force[2]= 0.0;
						lf->loc[0]= ac->force;
						lf->loc[1]= ac->min;
						lf->loc[2]= ac->max;
					}
					break;
					
				case SN_X_ROT:
					omega[0] += DTIME*ac->force;
					retval |= SN_CHANGED+SN_DOOMEGA;
					if(ac->flag & SN_LIMITS) {
						if( lf->rot[0] < ac->min || lf->rot[0]>ac->max) {
							ac->go= 1;
							ac->cur= 50;
						}
					}
					break;
					
				case SN_Y_ROT:
					omega[1] += DTIME*ac->force;
					retval |= SN_CHANGED+SN_DOOMEGA;
					if(ac->flag & SN_LIMITS) {
						if( lf->rot[1] < ac->min || lf->rot[1]>ac->max) {
							ac->go= 1;
							ac->cur= 50;
						}
					}
					break;
					
				case SN_Z_ROT:
					omega[2] += DTIME*ac->force;
					retval |= SN_CHANGED+SN_DOOMEGA;
					if(ac->flag & SN_LIMITS) {
						if( lf->rot[2] < ac->min || lf->rot[2]>ac->max) {
							ac->go= 1;
							ac->cur= 50;
						}
					}
					break;
				
				case SN_TRACKTO:
					if(ac->poin) {
						Object *to=ac->poin;
						retval |= SN_CHANGED+SN_ROTCHANGED;
						track_life_to_life(lf, to->data, ac->sta, ac->cur);
					}
					break;
				
				case SN_ADD_DAMAGE:
					if(victim) {
						if(victim->type==OB_LIFE) {
							Life *lfs= victim->data;
							/* wel 'add health' toestaan */
							if(ac->var<0 || (lfs->dflag & LF_NO_DAMAGE)==0) 
								lfs->state[LF_DAMAGE-1] += ac->var;
						}
					}
					break;

				case SN_NO_DAMAGE:
					lf->dflag |= LF_NO_DAMAGE;
					ac->go= 1;
					break;

				case SN_ROBBIE_M:
					if(ac->poin) {
						camera_behaviour(ob, lf, ac);
						retval |= SN_CHANGED+SN_ROTCHANGED;
					}
					break;
					
				case SN_DELTA_OBIPO:
				case SN_PLAY_OBIPO:
					
					if( sn->evento==0) {	/* KEY_IN */
					
						if(ac->go==0) {
							ac->cur= ac->sta;
							if(ac->end > ac->sta) ac->go= 1;
							else ac->go= -1;
						}
						else eventlock= 1;
					}
					break;
					
				case SN_PINGPONG_OBIPO:
					
					if( sn->evento==0) {	/* KEY_IN */
						if(ac->go) {
							ac->go= -ac->go;
						}
						else if(ac->cur==ac->sta) { /* sta== ook initwaarde en rustwaarde */
							if(ac->end > ac->sta) ac->go= 1;
							else ac->go= -1;
						}
						else if(ac->cur==ac->end) {/* end==rustwaarde */
							if(ac->sta > ac->end) ac->go= 1;
							else ac->go= -1;
						}
					}
					break;
					
				case SN_FLIPPER_OBIPO:
					if( sn->evento==0) {	/* KEY_IN */
						/* niet cur zetten: is flipper! */
						if(ac->end > ac->sta) ac->go= 1;
						else ac->go= -1;
					}
					break;
					
				case SN_LOOPSTOP_OBIPO:
				case SN_LOOPEND_OBIPO:
					
					if( sn->evento==0) {	/* KEY_IN */
						
						if(ac->cur==ac->end) {	/* end== ook initwaarde en rustwaarde */
							ac->cur= ac->sta;
						}
						if(ac->end > ac->sta) ac->go= 1;
						else ac->go= -1;
					}
					break;
				case SN_K2K_OBIPO:
					
					if( sn->evento==0 || (sn->event && (ac->var & SN_K2K_HOLD))) {	/* KEY_IN */
					
						if(ac->go==0) {
							
							if(ac->var & SN_K2K_PREV) mode= set_k2k_interval(-1, ac, lf);
							else mode= set_k2k_interval(1, ac, lf);
							
							if(mode==1) {	/* extrema bereikt */
								if(ac->var & SN_K2K_CYCLIC) {
									if(ac->var & SN_K2K_PREV) set_k2k_interval(2, ac, lf);
									else set_k2k_interval(0, ac, lf);
									mode= 0;
								}
								else if(ac->var & SN_K2K_PINGPONG) {
									if(ac->var & SN_K2K_PREV) ac->var &= ~SN_K2K_PREV;
									else ac->var |= SN_K2K_PREV;
										
									SWAP(short, ac->end, ac->sta);
									mode= 0;
								}
							}

							if(mode==0) {
								ac->cur= ac->sta;
								if(ac->end > ac->sta) ac->go= 1;
								else ac->go= -1;
							}
							
							if(mode==2) {
								eventlock= 1;	
							}
							
						}
						else eventlock= 1;
					}
					break;
					
				case SN_LAYERMOVE:
					ob->lay= 1<<(ac->end-1);
					break;
				case SN_STARTTIMER:
					if(lf->timer<0) lf->timer= ac->sta;
					break;
				case SN_ADDLIFE:
					if(ac->poin) add_dupli_life(ac->poin, ob, ac->sta);
					break;
				case SN_REPLACELIFE:
					break;
				case SN_ENDLIFE:
					lf->timer= 0;
					break;
				case SN_LOADFILE:
					add_action_debug(ac, 150);
					/* G.simulf |= G_LOADFILE; */
					/* simul_load_str= ac->name; */
					break;
				case SN_RESTART:
					G.simulf |= G_RESTART;
					break;
				case SN_QUIT:
					G.simulf |= G_QUIT;
					break;
				case SN_VISIBLE:
					lf->dflag &= ~LF_DONTDRAW;
					ac->go= 1;
					ac->cur= ac->sta;
					
					break;
				case SN_REPLACEMESH:
					lf->texmesh= ac->poin;
					break;
				case SN_SETCAMERA:
					/* geen KEY_IN: voor o.a. near sensors en vars */
					evaluate_camera_network(ac->poin, 0);
					break;
					
				case SN_PLAYMOVIE:
					if( sn->evento==0) {	/* KEY_IN */
						extern Action *playmovie;
						
						playmovie= ac;
						ac->cur= ac->sta;
						
						if(ac->poin==0) add_action_debug(ac, 150);
					}
					break;
					
				case SN_ADD_VARIABLE:
					if(ac->poin) {
						if(eventlock) break;
						sp= ac->poin;
						*sp+= ac->var;
						if(ac->flag & SN_VAR_CYCLIC) {
							if(*sp < ac->sta) *sp= ac->end;
							else if(*sp > ac->end) *sp= ac->sta;
						}
						else {
							CLAMP( *sp , ac->sta, ac->end);
						}
					}
					break;
				case SN_SET_VARIABLE:
					if( ac->poin) {
						*((short *)ac->poin)= ac->var;
					}
					break;
				case SN_HOLD_VARIABLE:
					if( ac->poin) {
						*((short *)ac->poin)= 1;
						ac->go= 1;
					}
					break;
				case SN_TOG_VARIABLE:
					if(ac->poin) {
						if(eventlock) break;
						sp= ac->poin;
						*sp= *sp?0:1;
					}
					break;
				case SN_IPO_VARIABLE:
					if(ac->poin) {
						retval |= SN_CHANGED;
						cox= *((short *)ac->poin);
						do_obipo(ob, 2*cox, 0, 0);
					
					}
					break;
				}	
				
				ac++;
			}
		}
		
		/* LOPENDE ACTIONS */
		
		ac= sn->actions;
		b= sn->totaction;
		while(b--) {

			if(ac->go) {		/* niet alleen bij ipoos!!! */
				
				pob= ob;

				switch(ac->action) {
				
				case SN_X_ROT:
				case SN_Y_ROT:
				case SN_Z_ROT:
				
					ac->cur--;
					if(ac->cur>=0) {
						
						retval |= SN_CHANGED;
						
						if(ac->action==SN_X_ROT) cox= 0;
						else if(ac->action==SN_Y_ROT) cox= 1;
						else cox= 2;
					
						if(lf->rot[cox] < ac->min) {
							lf->rot[cox]= 0.9*lf->rot[cox] + 0.1*ac->min;
							lf->rotspeed[cox]*= 0.8;
						}
						else if(lf->rot[cox] > ac->max) {
							lf->rot[cox]= 0.9*lf->rot[cox] + 0.1*ac->max;
							lf->rotspeed[cox]*= 0.8;
						}
					}
					else ac->go= 0;					
					break;
					
				case SN_NO_DAMAGE:
					if(sn->event==0) {
						lf->dflag &= ~LF_NO_DAMAGE;
						ac->go= 0;
					}
					break;

					
				case SN_DELTA_OBIPO:
					
					retval |= SN_CHANGED;
	
					/* ipo afhandelen als FLIPPER_OBIPO */
					ac->cur+= ac->go;
					CLAMPTEST(ac->cur, ac->sta, ac->end);
					
					if(lf->type==LF_DYNAMIC) {
							if(ac->flag & SN_IPOFORCE) {
							do_force_obipo(pob, ac->cur, force, omega);
							retval |= SN_DOFORCE+SN_DOOMEGA;
						}
						else {
							do_obipo(pob, ac->cur, 1, speed);
							retval |= SN_DOSPEED;
						}
					}
					else do_obipo(pob, ac->cur, ac->go, 0);
					
					/*  zolang 'hold', dx niet op nul zetten */
					if(ac->cur==ac->end) {
						if(sn->event) ;	/* hold */
						else ac->go= 0;
					}
					break;

				case SN_PLAY_OBIPO:
				
					retval |= SN_CHANGED;
					ac->cur+= ac->go;
					CLAMPTEST(ac->cur, ac->sta, ac->end);
					
					if(lf->type==LF_DYNAMIC) {
						if(ac->flag & SN_IPOFORCE) {
							do_force_obipo(pob, ac->cur, force, omega);
							retval |= SN_DOFORCE+SN_DOOMEGA;
						}
						else {
							do_obipo(pob, ac->cur, ac->go, speed);
							retval |= SN_DOSPEED;
						}
					}
					else do_obipo(pob, ac->cur, 0, 0);

	
					if(ac->cur==ac->end) ac->go= 0;
					break;
				
				case SN_PINGPONG_OBIPO:
				
					retval |= SN_CHANGED;
				
					ac->cur+= ac->go;
					CLAMPTEST(ac->cur, ac->sta, ac->end);
					
					if(lf->type==LF_DYNAMIC) {
						if(ac->flag & SN_IPOFORCE) {
							do_force_obipo(pob, ac->cur, force, omega);
							retval |= SN_DOFORCE+SN_DOOMEGA;
						}
						else {
							do_obipo(pob, ac->cur, ac->go, speed);
							retval |= SN_DOSPEED;
						}
					}
					else do_obipo(pob, ac->cur, 0, 0);
					
					if(ac->cur==ac->sta || ac->cur==ac->end) ac->go= 0;
					break;

				case SN_FLIPPER_OBIPO:
				
					if(ac->cur==ac->end && sn->event==sn->evento ) ;	/* hold */
					else {
					
						retval |= SN_CHANGED;
				
						if(sn->event)  ac->cur+= ac->go;	/* start en hold */
						else ac->cur-= ac->go;

						CLAMPTEST(ac->cur, ac->sta, ac->end);
						
						if(lf->type==LF_DYNAMIC) {
							if(ac->flag & SN_IPOFORCE) {
								do_force_obipo(pob, ac->cur, force, omega);
								retval |= SN_DOFORCE+SN_DOOMEGA;
							}
							else {
								if(sn->event) do_obipo(pob, 1, ac->go, speed);
								else do_obipo(pob, ac->cur, -1, speed);
								retval |= SN_DOSPEED;
							}
						}
						else do_obipo(pob, ac->cur, 0, 0);

						if(ac->cur==ac->sta) ac->go= 0;
					}
					break;
					
				case SN_LOOPSTOP_OBIPO:
				
					retval |= SN_CHANGED;
					
					ac->cur+= ac->go;
					CLAMPTEST(ac->cur, ac->sta, ac->end);
					
					if(lf->type==LF_DYNAMIC) {
						if(ac->flag & SN_IPOFORCE) {
							do_force_obipo(pob, ac->cur, force, omega);
							retval |= SN_DOFORCE+SN_DOOMEGA;
						}
						else {
							do_obipo(pob, ac->cur, ac->go, speed);
							retval |= SN_DOSPEED;
						}
					}
					else do_obipo(pob, ac->cur, 0, 0);
					
					if( sn->event==0) ac->go= 0;		/* no hold */
					else if(ac->cur==ac->end) {	/* end== ook initwaarde en rustwaarde */
						ac->cur= ac->sta;
					}
					break;
				
				case SN_LOOPEND_OBIPO:
				
					retval |= SN_CHANGED;
					
					ac->cur+= ac->go;
					CLAMPTEST(ac->cur, ac->sta, ac->end);
					
					if(lf->type==LF_DYNAMIC) {
						if(ac->flag & SN_IPOFORCE) {
							do_force_obipo(pob, ac->cur, force, omega);
							retval |= SN_DOFORCE+SN_DOOMEGA;
						}
						else {
							do_obipo(pob, ac->cur, ac->go, speed);
							retval |= SN_DOSPEED;
						}
					}
					else do_obipo(pob, ac->cur, 0, 0);
					
					if(sn->event==0) {	/* no hold */
						if(ac->cur==ac->end) ac->go= 0;
					}
					else if(ac->cur==ac->end) {	/* end== ook initwaarde en rustwaarde */
						ac->cur= ac->sta;
					}
					break;	
				
				case SN_K2K_OBIPO:
					retval |= SN_CHANGED;
					ac->cur+= ac->go;
					CLAMPTEST(ac->cur, ac->sta, ac->end);
					lf->cfra= ac->cur;
					
					if(lf->type==LF_DYNAMIC) {
						if(ac->flag & SN_IPOFORCE) {
							do_force_obipo(pob, ac->cur, force, omega);
							retval |= SN_DOFORCE+SN_DOOMEGA;
						}
						else {
							do_obipo(pob, ac->cur, ac->go, speed);
							retval |= SN_DOSPEED;
						}
					}
					else do_obipo(pob, ac->cur, 0, 0);
	
					if(ac->cur==ac->end) ac->go= 0;				
					break;

				case SN_VISIBLE:
					if(sn->event==0) {		/* KEY OUT */
						if(ac->cur<=0 ) {		
							lf->dflag |= LF_DONTDRAW;
							ac->go= 0;
						}
						else ac->cur--;
					}
					break;

				case SN_HOLD_VARIABLE:
					if(ac->poin) {
						if(sn->event==0 ) {
							*((short *)ac->poin)= 0;
							ac->go= 0;
						}
					}
					break;
				
				}
				
				if(ac->action>=100 && ac->action<200) {		/*  ipoos */
					if(ac->flag & SN_IPO_SETVAR) {
						if(ac->poin) *((short *)ac->poin)= ac->cur/2;
					}
					/* pob= ac->poin?ac->poin:ob; */
				}
				

			}
			ac++;
		}
		
		sn++;
	}
	
	/* dyna's twee soorten speed: van force en van ipo */
	/* beide moeten lokaal bij dyna's */
	
	if(retval) {
		lf->dflag |= LF_DYNACHANGED;
	
		if(retval & SN_DOFORCE) {
			/* force: lokaal */
			forcetot[0]+= force[0]*ob->obmat[0][0] + force[1]*ob->obmat[1][0] + force[2]*ob->obmat[2][0];
			forcetot[1]+= force[0]*ob->obmat[0][1] + force[1]*ob->obmat[1][1] + force[2]*ob->obmat[2][1];
			forcetot[2]+= force[0]*ob->obmat[0][2] + force[1]*ob->obmat[1][2] + force[2]*ob->obmat[2][2];
		}
		if(retval & SN_DOOMEGA) {
			/* eigenlijk ook lokaal, hoe? */
			omegatot[0]+= omega[0];
			omegatot[1]+= omega[1];
			omegatot[2]+= omega[2];
		}
		if(retval & SN_DOSPEED) {
			/* speed: lokaal */
			lf->speed[0]= speed[0]*ob->obmat[0][0] + speed[1]*ob->obmat[1][0] + speed[2]*ob->obmat[2][0];
			lf->speed[1]= speed[0]*ob->obmat[0][1] + speed[1]*ob->obmat[1][1] + speed[2]*ob->obmat[2][1];
			lf->speed[2]= speed[0]*ob->obmat[0][2] + speed[1]*ob->obmat[1][2] + speed[2]*ob->obmat[2][2];
		}
	}
	return retval;
}



void collision_sensor_input(Object *ob, Life *lf)
{
	/* voor snelheid:
	 *  alleen actions die maar 1 x worden afgehandeld
	 */
	Sensor *sn;
	Event *ev;
	Action *ac;
	int event, a, b;
	short *sp;

	a= lf->totsens;
	if(a==0) return;
	
	sn= lf->sensors;
	while(a--) {

		if(sn->totaction==1) {
		
			/* EVENTS */
			b= sn->totevent;
			ev= sn->events;
			event= 1;
			while(b--) {
				
				switch(ev->event) {
				case SN_COLLISION:
					if(lf->collision==0 || ev->var>0) {
						event= 0;
					}
					else if(ev->poin && lf->collision!=ev->poin) {
						event= 0;
					}
					break;
	
				case SN_TIMER:
					/* timer in fields */
					if( (lf->timer>>1) != ev->var) event= 0;
					break;
				
				case SN_VAR_EQUAL:
					sp= ev->poin;
					if(sp==0 || *sp!=ev->var) event= 0;
					break;
				case SN_VAR_INTERVAL:
					sp= ev->poin;
					if(sp==0 || *sp<ev->sfac || *sp>ev->sfac1) event= 0;
					break;
				case SN_VAR_CHANGED:
					sp= ev->poin;
					if(sp) {
						if(*sp==ev->var) event= 0;
						else ev->var= *sp;
					}
					else event= 0;
				}
				
				if(event==0) break;
				ev++;
			}
			
			/* ACTIONS STARTEN */
			
			if(event) {
				
				ac= sn->actions;
				b= sn->totaction;
				while(b--) {
				
					/* waarop: */
					switch(ac->action) {
					case SN_LAYERMOVE:
						ob->lay= 1<<(ac->end-1);
						break;
	
					case SN_ADDLIFE:
						if(ac->poin) add_dupli_life(ac->poin, ob, ac->sta);
						break;
	
					case SN_REPLACELIFE:
						break;
	
					case SN_ENDLIFE:
						lf->timer= 0;					
						break;
	
					}
					ac++;
				}
			}
		}
		sn++;
	}
}

#endif

