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
 *  aug 97
 *  
 *				alle SGI-blender routines
 * 
 */

#ifndef FREE

#include "blender.h"
#include "sector.h"

/* ************ SECTORSPUL ******************* */

void unlink_sector(Sector *se)
{
	/* loskoppelen: */
	
	if(se->dynamesh) se->dynamesh->id.us--;
	if(se->texmesh) se->texmesh->id.us--;
}


/* niet sector zelf vrijgeven */
void free_sector(Sector *se)
{

	unlink_sector(se);

	if(se->portals) freeN(se->portals);
}

Sector *add_sector()
{
	Sector *se;
	Mesh *me;
	Portal *po;
	MVert *mvert;
	MFace *mface;
	TFace *tface;
	int a;
	
	se= alloc_libblock(&G.main->sector, ID_SE, "Sector");
	
	se->type= SE_CUBE;
	se->flag= SE_SHOW_TEXMESH;
	
	se->size[0]= se->size[1]= se->size[2]= 1.0;
	se->r= se->g= se->b= 1.0;
	
	/* standaard plane */
	se->dynamesh= me= add_mesh();
	rename_id((ID *)me, "DynaMesh");
	me->flag &= ~ME_TWOSIDED;
	 
	me->mvert= mvert= callocN(4*sizeof(MVert), "mvert");
	
	me->totvert= 4;
	me->totface= 1;
	
	for(a=0; a<4; a++, mvert++) {
		if(a==1 || a==2) mvert->co[0]= 1.0;
		else mvert->co[0]= -1.0;
		if(a==0 || a==1) mvert->co[1]= 1.0;
		else mvert->co[1]= -1.0;
		mvert->co[2]= -1.0;
	}
	
	me->mface= mface= callocN(sizeof(MFace), "mface");
	
	mface->v1= 0;
	mface->v2= 3;
	mface->v3= 2;
	mface->v4= 1;
	mface->edcode= ME_V1V2+ME_V2V3+ME_V3V4+ME_V4V1;
	
	se->totport= 4;
	se->actport= 1;
	po= se->portals= callocN((se->totport)*sizeof(Portal), "portal");
	po->type= PO_XPOS;
	(po+1)->type= PO_XNEG;
	(po+2)->type= PO_YPOS;
	(po+3)->type= PO_YNEG;


	/* TEXMESH:  ook standaard plane */
	se->texmesh= me= add_mesh();
	rename_id((ID *)me, "TexMesh");
	me->flag &= ~ME_TWOSIDED;
	
	me->totvert= 4;
	me->totface= 1;

	me->mface= dupallocN(se->dynamesh->mface);	
	me->mvert= dupallocN(se->dynamesh->mvert);	
	
	me->tface= tface= callocN(sizeof(TFace), "tface");
	
	default_tface(tface);
	tface->flag |= SELECT|ACTIVE;
	
	return se;
}

Sector *copy_sector(Sector *se)
{
	Sector *sen;
	int a;
	
	sen= copy_libblock(se);

	id_us_plus((ID *)sen->dynamesh);
	id_us_plus((ID *)sen->texmesh);
	
	sen->portals= dupallocN(se->portals);
	sen->ob= 0;
	
	return sen;
}

void make_local_sector(Sector *se)
{
	Object *ob;
	Sector *sen;
	int local=0, lib=0;
	
	/* - zijn er alleen lib users: niet doen
	 * - zijn er alleen locale users: flag zetten
	 * - mixed: copy
	 */
	

	if(se->id.lib==0) return;

	if(se->id.us==1) {
		se->id.lib= 0;
		se->id.flag= LIB_LOCAL;
		new_id(0, (ID *)se, 0);
		
		if(se->texmesh) make_local_mesh(se->texmesh);
		if(se->dynamesh) make_local_mesh(se->dynamesh);

		return;
	}
	
	ob= G.main->object.first;
	while(ob) {
		if(ob->data==se) {
			if(ob->id.lib) lib= 1;
			else local= 1;
		}
		ob= ob->id.next;
	}
	
	if(local && lib==0) {
		se->id.lib= 0;
		se->id.flag= LIB_LOCAL;
		new_id(0, (ID *)se, 0);
	}
	else if(local && lib) {
		sen= copy_sector(se);
		sen->id.us= 0;
		
		ob= G.main->object.first;
		while(ob) {
			if(ob->data==se) {
				
				if(ob->id.lib==0) {
					ob->data= sen;
					sen->id.us++;
					se->id.us--;
				}
			}
			ob= ob->id.next;
		}
	}
}

/* **************** ARRAY EDIT ******************************** */

void add_array_element(void **base, short *act, short *tot, int size)
{
	void *temp;
	char *cp;
	
	if(*act > *tot) *act = *tot;
	if(*act==0) *act= 1;
	
	temp= callocN( (*tot+1)*size, "add_array");
	
	if(*base) {
		memcpy(temp, *base, (*tot)*size);
		
		cp= temp;
		memcpy(cp + (*tot)*size, cp + (*act-1)*size, size);
		freeN(*base);
	}
	
	*base= temp;
	(*tot)++;
	*act = *tot;
	
}

void delete_array_element(void **base, short *act, short *tot, int size)
{
	Portal *po;	
	int a;
	char *cp;
	
	if(*tot == 0) return;
	if(*act==0) *act= 1;
	
	if(*tot == 1) {
		*tot= *act= 0;
		freeN(*base);
		*base= 0;
	}
	else {
		cp= *base;
		cp+= (*act-1)*size;
		for(a= *act-1; a< *tot-1; a++, cp+=size) {
			memcpy(cp, cp+size, size);
		}
	
		(*tot)--;
		if( *tot == 0) *act= 0;
		else if(*act > *tot) *act = *tot;
	}
}



/* ***************** PORTALS ****************************** */



void add_portal(Sector *se)
{
	Portal *po;	
	
	/* aktieve portal: dupli */
	
	add_array_element((void **)&se->portals, &se->actport, &se->totport, sizeof(Portal));
	
	if(se->totport==1) {	/* init */
		po= se->portals;
		po->type= PO_XPOS;
	}
}

void delete_portal(Sector *se)
{
	Portal *po;	
	int a;
	
	/* aktieve portal: del */
	
	delete_array_element((void **)&se->portals, &se->actport, &se->totport, sizeof(Portal));

}

void testportals(Sector *se)
{
	Portal *po;
	float cent[3], dx;	
	int a, cox, coy;

	a= se->totport;
	po= se->portals;
	while(a--) {
		
		get_co_portal(se, po->type, po->ofs, cent);
		
		if(po->type==PO_XPOS || po->type==PO_XNEG) {
			cox= 1; coy= 2;
		}
		else if(po->type==PO_YPOS || po->type==PO_YNEG) {
			cox= 0; coy= 2;
		}
		else {
			cox= 0; coy= 1;
		}
		
		if( fabs(cent[cox]) > se->size[cox] ) {
			if(po->ofs[0]>0.0) po->ofs[0]= se->size[cox];
			else po->ofs[0]= -se->size[cox];
		}
		if( fabs(cent[coy]) > se->size[coy] ) {
			if(po->ofs[1]>0.0) po->ofs[1]= se->size[coy];
			else po->ofs[1]= -se->size[coy];
		}
		
		po++;
	}
}

/* ************ OCTREE **************** */

/* deze versie voor vlakken */
void calculate_ocvec(short *ocvec, float *v1, float *dvec, float *size)
{
	int a;
	
	if(size[0]!=0.0) ocvec[0]= ffloor(16.0*(v1[0]-dvec[0])/size[0]);
	else ocvec[0]= 0;
	CLAMP(ocvec[0], 0, 15);
	
	if(size[1]!=0.0) ocvec[1]= ffloor(16.0*(v1[1]-dvec[1])/size[1]);
	else ocvec[1]= 0;
	CLAMP(ocvec[1], 0, 15);
	
	if(size[2]!=0.0) ocvec[2]= ffloor(16.0*(v1[2]-dvec[2])/size[2]);
	else ocvec[2]= 0;
	CLAMP(ocvec[2], 0, 15);
}

void init_mesh_octree(Mesh *me)
{
	DFace *dface;
	int a, b;
	short min[3], max[3], ocvec[3];
	
	/* ocinfo maken */
	if(me->dface==0) return;
	me->oc= mallocN(sizeof(OcInfo), "oc");
	
	if(me->bb==0) boundbox_mesh(me, me->oc->dvec, me->oc->size);
	
	VecSubf(me->oc->size, me->bb->vec[6], me->bb->vec[0]);
	VECCOPY(me->oc->dvec, me->bb->vec[0]);
	
	if(me->oc->size[0] < 0.01) me->oc->size[0]= 0.0;
	if(me->oc->size[1] < 0.01) me->oc->size[1]= 0.0;
	if(me->oc->size[2] < 0.01) me->oc->size[2]= 0.0;
	
	/* dface->ocx/y/z berekenen */
	dface= me->dface;
	a= me->totface;
	while(a--) {
		
		if(dface->v3==0) {
			dface->ocx= 0;
		}
		else {
			min[0]=min[1]=min[2]= 16;
			max[0]=max[1]=max[2]= 0;
			
			calculate_ocvec(ocvec, dface->v1, me->oc->dvec, me->oc->size);
			DO_MINMAX(ocvec, min, max);
			calculate_ocvec(ocvec, dface->v2, me->oc->dvec, me->oc->size);
			DO_MINMAX(ocvec, min, max);
			calculate_ocvec(ocvec, dface->v3, me->oc->dvec, me->oc->size);
			DO_MINMAX(ocvec, min, max);
			if(dface->v4) {
				calculate_ocvec(ocvec, dface->v4, me->oc->dvec, me->oc->size);
				DO_MINMAX(ocvec, min, max);
			}
			dface->ocx= BROW(min[0], max[0]);
			dface->ocy= BROW(min[1], max[1]);
			dface->ocz= BROW(min[2], max[2]);

		}
		dface++;
	}
}
	

void switch_dir_dface(DFace *dface)
{
	short edcode;
	
	/* niet de normaal flippen: is alleen optim voor intersect */

	/* OP DEZE MANIER GAAT IE OP DE PSX MIS!!!!	
	 *	if(dface->v4) {
	 *		SWAP(float *, dface->v2, dface->v3);
	 *		SWAP(float *, dface->v1, dface->v4);
	 *	}
	 *	else {
	 *		SWAP(float *, dface->v1, dface->v3);
	 *	}
	 */

	/* deze is GOED!! (ook voor vierhoeken ) */
	
	SWAP(float *, dface->v1, dface->v3);

	edcode= 0;
	if(dface->edcode & DF_V1) edcode |= DF_V3;
	if(dface->edcode & DF_V2) edcode |= DF_V2;
	if(dface->edcode & DF_V3) edcode |= DF_V1;
	if(dface->edcode & DF_V4) edcode |= DF_V4;
	
	if(dface->v4) {
		if(dface->edcode & DF_V1V2) edcode |= DF_V2V3;
		if(dface->edcode & DF_V2V3) edcode |= DF_V1V2;
		if(dface->edcode & DF_V3V4) edcode |= DF_V4V1;
		if(dface->edcode & DF_V4V1) edcode |= DF_V3V4;
	}
	else {
		if(dface->edcode & DF_V1V2) edcode |= DF_V2V3;
		if(dface->edcode & DF_V2V3) edcode |= DF_V1V2;
	}
	dface->edcode= edcode;
}


void init_dyna_material(Material *ma)
{
	
	DYNA_REF(ma)= 0.1;
	DYNA_FH_DIST(ma)= 0.0;
	DYNA_FH_INT(ma)= 0.0;
	DYNA_FH_FRICT(ma)= 0.0;
	DYNA_FH_XYFRICT(ma)= 0.0;
	ma->lay= 1;
}

void init_dynamesh(Object *ob, Mesh *me)
{
	extern Material defmaterial;
	MFace *mface;
	MVert *v1, *v2, *v3, *v4;
	DFace *dface;
	Material *ma;
	float xn, yn, zn;
	int a, act=1;
	
	if(me->totface==0) return;

	/* sphereflags vorbereiden */
	v1= me->mvert;
	for(a=0; a<me->totvert; a++, v1++) {
		v1->flag &= ~ME_SPHERETEMP;
		if(v1->flag & ME_SPHERETEST) v1->flag |= ME_SPHERETEMP;
	}

	me->dface= callocN( me->totface*sizeof(DFace), "DFace");
	mface= me->mface;
	dface= me->dface;
	
	ma= give_current_material(ob, 1);
	
	for(a=0; a<me->totface; a++, mface++, dface++) {
	
		v1= (me->mvert+mface->v1);
		v2= (me->mvert+mface->v2);
		if(mface->v3) v3= (me->mvert+mface->v3); else v3= 0;
		if(mface->v4) v4= (me->mvert+mface->v4); else v4= 0;
	
		dface->v1= v1->co;
		dface->v2= v2->co;
		if(v3) dface->v3= v3->co; else dface->v3= 0;
		if(v4) dface->v4= v4->co; else dface->v4= 0;
		
		/* ook dfaces met v3==0. wordt soms uitgelezen */
		if(mface->mat_nr != act-1) {
			act= mface->mat_nr+1;
			ma= give_current_material(ob, act);
		}
		if(ma) dface->ma= ma;
		else dface->ma= &defmaterial;
		
		if(dface->v3) {
			CalcNormFloat(dface->v1, dface->v2, dface->v3, dface->no);
			
			xn= fabs(dface->no[0]);
			yn= fabs(dface->no[1]);
			zn= fabs(dface->no[2]);
			
			/* edge en vertexcode voor cyl en sphere isect */
			dface->edcode= mface->edcode & (~15);
			
			if(FALSE) {
			if(v1->flag & ME_SPHERETEMP) {dface->edcode |= DF_V1; v1->flag -= ME_SPHERETEMP;}
			if(v2->flag & ME_SPHERETEMP) {dface->edcode |= DF_V2; v2->flag -= ME_SPHERETEMP;}
			if(v3->flag & ME_SPHERETEMP) {dface->edcode |= DF_V3; v3->flag -= ME_SPHERETEMP;}
			if(v4) if(v4->flag & ME_SPHERETEMP) {dface->edcode |= DF_V4; v4->flag -= ME_SPHERETEMP;}
			}
			else {	/* elk vlak moet sphere-code hebben: zie ook editmedh.c commentaar */
			if(v1->flag & ME_SPHERETEMP) {dface->edcode |= DF_V1;}
			if(v2->flag & ME_SPHERETEMP) {dface->edcode |= DF_V2;}
			if(v3->flag & ME_SPHERETEMP) {dface->edcode |= DF_V3;}
			if(v4) if(v4->flag & ME_SPHERETEMP) {dface->edcode |= DF_V4;}
			}
			
			/* optimalisering bij intersect_dface() */
			if(zn>=xn && zn>=yn) {
				dface->proj= 0;
				if( (dface->v2[0] - dface->v3[0])*(dface->v2[1] - dface->v1[1]) <
					(dface->v2[1] - dface->v3[1])*(dface->v2[0] - dface->v1[0]) )
					switch_dir_dface(dface);
					
			}
			else if(yn>=xn && yn>=zn) {
				dface->proj= 1;
				if( (dface->v2[0] - dface->v3[0])*(dface->v2[2] - dface->v1[2]) <
					(dface->v2[2] - dface->v3[2])*(dface->v2[0] - dface->v1[0]) )
					switch_dir_dface(dface);
			}
			else {
				dface->proj= 2;
				if( (dface->v2[1] - dface->v3[1])*(dface->v2[2] - dface->v1[2]) <
					(dface->v2[2] - dface->v3[2])*(dface->v2[1] - dface->v1[1]) )
					switch_dir_dface(dface);
			}
			
			dface->dist= (dface->no[0]*dface->v1[0] + dface->no[1]*dface->v1[1] + dface->no[2]*dface->v1[2] ); 
		}
	}

	init_mesh_octree(me);
}

void end_dynamesh(Mesh *me)
{
	
	if(me==0) return;
	if(me->dface) freeN(me->dface);
	me->dface= 0;
	if(me->oc) freeN(me->oc);
	me->oc= 0;
}


/* *************** */

#ifdef IRISGL

void sector_simulate()
{
	Camera *cam=0;
	float params[5]= {8.5, 10.0, 0.0, 0.0, 0.0};	
	float loc[3], rot[3];
	
	if(G.scene->camera==0 || G.scene->camera->type!=OB_CAMERA) {
		error("no (correct) camera");
		return;
	}
	/* soms is de aktieve camera niet in een sector */
	VECCOPY(loc, G.scene->camera->loc);
	VECCOPY(rot, G.scene->camera->rot);
	
	unqdevice(MOUSEX);
	unqdevice(MOUSEY);

	G.f |= G_SIMULATION;
	G.simulf= 0;
		
	cam= G.scene->camera->data;
	params[0]= cam->clipend*0.4;
	params[1]= cam->clipend;
	
	fogvertex(FG_VTX_LIN, params);
	fogvertex(FG_ON, 0);

	waitcursor(1);

	while(TRUE) {
		
		sector_go();
		
		if(G.simulf & G_LOADFILE);
		if(G.simulf & G_RESTART);
		if(G.simulf & G_QUIT) {
			reset_slowparents();	/* editobject.c */
			break;
		}
		G.simulf= 0;
	}

	waitcursor(0);
	qdevice(MOUSEX);
	qdevice(MOUSEY);
	fogvertex(FG_OFF, 0);
	G.f &= ~G_SIMULATION;
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWBUTSALL, 0);
	
	/* soms is de aktieve camera niet in een sector */
	VECCOPY(G.scene->camera->loc, loc);
	VECCOPY(G.scene->camera->rot, rot);

}

#endif

#endif

