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


/*  mesh.c      MIXED MODEL
 * 
 *  jan/maart 95
 *  
 * 
 */

#include "blender.h"
#include "edit.h"
#include "sector.h"

void unlink_mesh(Mesh *me)
{
	int a;
	
	if(me==0) return;
	
	for(a=0; a<me->totcol; a++) {
		if(me->mat[a]) me->mat[a]->id.us--;
		me->mat[a]= 0;
	}
	if(me->key) me->key->id.us--;
	me->key= 0;
	
	if(me->texcomesh) me->texcomesh= 0;
}


/* niet mesh zelf vrijgeven */
void free_mesh(Mesh *me)
{

	unlink_mesh(me);

	if(me->mat) freeN(me->mat);
	if(me->orco) freeN(me->orco);
	if(me->mface) freeN(me->mface);
	if(me->tface) freeN(me->tface);
	if(me->mvert) freeN(me->mvert);
	if(me->mcol) freeN(me->mcol);
	if(me->msticky) freeN(me->msticky);
	if(me->bb) freeN(me->bb);
	if(me->disp.first) freedisplist(&me->disp);
}

Mesh *add_mesh()
{
	Mesh *me;
	
	me= alloc_libblock(&G.main->mesh, ID_ME, "Mesh");
	
	me->size[0]= me->size[1]= me->size[2]= 1.0;
	me->smoothresh= 30;
	me->texflag= AUTOSPACE;
	me->flag= ME_TWOSIDED;
	me->subdiv= 4;
	
	me->bb= unit_boundbox();
	
	return me;
}

Mesh *copy_mesh(Mesh *me)
{
	Mesh *men, *p=0;
	int a;
	
	men= copy_libblock(me);
	
	men->mat= dupallocN(me->mat);
	for(a=0; a<men->totcol; a++) {
		id_us_plus((ID *)men->mat[a]);
	}
	id_us_plus((ID *)men->texcomesh);
	men->mface= dupallocN(me->mface);

	men->tface= dupallocN(me->tface);

	men->dface= 0;
	men->mvert= dupallocN(me->mvert);
	men->mcol= dupallocN(me->mcol);
	men->msticky= dupallocN(me->msticky);
	men->texcomesh= 0;
	men->orco= 0;
	men->bb= dupallocN(men->bb);
	
	copy_displist(&men->disp, &me->disp);
	
	men->key= copy_key(me->key);
	if(men->key) men->key->from= (ID *)men;
	
	return men;
}

void make_local_tface(Mesh *me)
{
	TFace *tface;
	Image *ima;
	int a;
	
	if(me->tface==0) return;
	
	a= me->totface;
	tface= me->tface;
	while(a--) {
		
		/* speciaal geval: ima altijd meteen lokaal */
		if(tface->tpage) {
			ima= tface->tpage;
			if(ima->id.lib) {
				ima->id.lib= 0;
				ima->id.flag= LIB_LOCAL;
				new_id(0, (ID *)ima, 0);
			}
		}
		tface++;
	}
	
}

void make_local_mesh(Mesh *me)
{
	Object *ob;
	Mesh *men;
	int local=0, lib=0;
	
	/* - zijn er alleen lib users: niet doen
	 * - zijn er alleen locale users: flag zetten
	 * - mixed: copy
	 */
	
	if(me->id.lib==0) return;
	if(me->id.us==1) {
		me->id.lib= 0;
		me->id.flag= LIB_LOCAL;
		new_id(0, (ID *)me, 0);
		
		if(me->tface) make_local_tface(me);
		
		return;
	}
	
	ob= G.main->object.first;
	while(ob) {
		if( me==get_mesh(ob) ) {
			if(ob->id.lib) lib= 1;
			else local= 1;
		}
		else if( me==get_other_mesh(ob) ) {
			if(ob->id.lib) lib= 1;
			else local= 1;
		}
		
		ob= ob->id.next;
	}
	
	if(local && lib==0) {
		me->id.lib= 0;
		me->id.flag= LIB_LOCAL;
		new_id(0, (ID *)me, 0);
		
		if(me->tface) make_local_tface(me);
		
	}
	else if(local && lib) {
		men= copy_mesh(me);
		men->id.us= 0;
		
		ob= G.main->object.first;
		while(ob) {
			if( me==get_mesh(ob) ) {				
				if(ob->id.lib==0) {
					set_mesh(ob, men);
				}
			}
			else if( me==get_other_mesh(ob) ) {				
				if(ob->id.lib==0) {
					set_mesh(ob, men);
				}
			}
			ob= ob->id.next;
		}
	}
}

void boundbox_mesh(Mesh *me, float *loc, float *size)
{
	MVert *mvert;
	BoundBox *bb;
	float min[3], max[3];
	float mloc[3], msize[3];
	int a;
	
	if(me->bb==0) me->bb= callocN(sizeof(BoundBox), "boundbox");
	bb= me->bb;
	
	INIT_MINMAX(min, max);

	if (!loc) loc= mloc;
	if (!size) size= msize;
	
	mvert= me->mvert;
	for(a=0; a<me->totvert; a++, mvert++) {
		DO_MINMAX(mvert->co, min, max);
	}

	if(me->totvert) {
		loc[0]= (min[0]+max[0])/2.0;
		loc[1]= (min[1]+max[1])/2.0;
		loc[2]= (min[2]+max[2])/2.0;
		
		size[0]= (max[0]-min[0])/2.0;
		size[1]= (max[1]-min[1])/2.0;
		size[2]= (max[2]-min[2])/2.0;
	}
	else {
		loc[0]= loc[1]= loc[2]= 0.0;
		size[0]= size[1]= size[2]= 0.0;
	}
	
	bb->vec[0][0]=bb->vec[1][0]=bb->vec[2][0]=bb->vec[3][0]= loc[0]-size[0];
	bb->vec[4][0]=bb->vec[5][0]=bb->vec[6][0]=bb->vec[7][0]= loc[0]+size[0];
	
	bb->vec[0][1]=bb->vec[1][1]=bb->vec[4][1]=bb->vec[5][1]= loc[1]-size[1];
	bb->vec[2][1]=bb->vec[3][1]=bb->vec[6][1]=bb->vec[7][1]= loc[1]+size[1];

	bb->vec[0][2]=bb->vec[3][2]=bb->vec[4][2]=bb->vec[7][2]= loc[2]-size[2];
	bb->vec[1][2]=bb->vec[2][2]=bb->vec[5][2]=bb->vec[6][2]= loc[2]+size[2];
}

void tex_space_mesh(Mesh *me)
{
	KeyBlock *kb;
	float *fp, loc[3], size[3], min[3], max[3];
	int a;
	
	boundbox_mesh(me, loc, size);

	if(me->texflag & AUTOSPACE) {
		if(me->key) {
			kb= me->key->refkey;
			if (kb) {
				
				INIT_MINMAX(min, max);
				
				fp= kb->data;
				for(a=0; a<kb->totelem; a++, fp+=3) {	
					DO_MINMAX(fp, min, max);
				}
				if(kb->totelem) {
					loc[0]= (min[0]+max[0])/2.0; loc[1]= (min[1]+max[1])/2.0; loc[2]= (min[2]+max[2])/2.0;
					size[0]= (max[0]-min[0])/2.0; size[1]= (max[1]-min[1])/2.0; size[2]= (max[2]-min[2])/2.0;
				}
				else {
					loc[0]= loc[1]= loc[2]= 0.0;
					size[0]= size[1]= size[2]= 0.0;
				}
				
			}
		}

		VECCOPY(me->loc, loc);
		VECCOPY(me->size, size);
		me->rot[0]= me->rot[1]= me->rot[2]= 0.0;

		if(me->size[0]==0.0) me->size[0]= 1.0;
		else if(me->size[0]>0.0 && me->size[0]<0.00001) me->size[0]= 0.00001;
		else if(me->size[0]<0.0 && me->size[0]> -0.00001) me->size[0]= -0.00001;
	
		if(me->size[1]==0.0) me->size[1]= 1.0;
		else if(me->size[1]>0.0 && me->size[1]<0.00001) me->size[1]= 0.00001;
		else if(me->size[1]<0.0 && me->size[1]> -0.00001) me->size[1]= -0.00001;
	
		if(me->size[2]==0.0) me->size[2]= 1.0;
		else if(me->size[2]>0.0 && me->size[2]<0.00001) me->size[2]= 0.00001;
		else if(me->size[2]<0.0 && me->size[2]> -0.00001) me->size[2]= -0.00001;
	}
	
}

void make_orco_mesh(Mesh *me)
{
	MVert *mvert;
	KeyBlock *kb;
	float *orco, *fp;
	int a, totvert;
	
	totvert= me->totvert;
	if(totvert==0) return;
	orco= me->orco= mallocN(sizeof(float)*3*totvert, "orco mesh");

	if(me->key && me->texcomesh==0) {
		kb= me->key->refkey;
		if (kb) {		/***** BUG *****/
			fp= kb->data;
			
			for(a=0; a<totvert; a++, orco+=3) {
				orco[0]= (fp[0]-me->loc[0])/me->size[0];
				orco[1]= (fp[1]-me->loc[1])/me->size[1];
				orco[2]= (fp[2]-me->loc[2])/me->size[2];
				
				/* mvert alleen ophogen als totvert <= kb->totelem */
				if(a<kb->totelem) fp+=3;
			}
		}
	}
	else {
		if(me->texcomesh) {
			me= me->texcomesh;
		}	
	
		mvert= me->mvert;
		for(a=0; a<totvert; a++, orco+=3) {
			orco[0]= (mvert->co[0]-me->loc[0])/me->size[0];
			orco[1]= (mvert->co[1]-me->loc[1])/me->size[1];
			orco[2]= (mvert->co[2]-me->loc[2])/me->size[2];
			
			/* mvert alleen ophogen als totvert <= me->totvert */
			if(a<me->totvert) mvert++;
		}
	}
}


void test_index_mface(MFace *mface, int nr)
{
	int a;
	
	/* first test if the face is legal */
	
	if(mface->v1<0) mface->v1= 0;
	if(mface->v2<0) mface->v2= 0;
	if(mface->v3<0) mface->v3= 0;
	if(mface->v4<0) mface->v4= 0;

	if(mface->v3 && mface->v3==mface->v4) {
		mface->v4= 0;
		nr--;
	}
	if(mface->v2 && mface->v2==mface->v3) {
		mface->v3= mface->v4;
		mface->v4= 0;
		nr--;
	}
	if(mface->v1==mface->v2) {
		mface->v2= mface->v3;
		mface->v3= mface->v4;
		mface->v4= 0;
		nr--;
	}

	/* voorkom dat een nul op de verkeerde plek staat */
	if(nr==2) {
		if(mface->v2==0) SWAP(int, mface->v1, mface->v2);
	}
	else if(nr==3) {
		if(mface->v3==0) {
			SWAP(int, mface->v1, mface->v2);
			SWAP(int, mface->v2, mface->v3);
			
			a= mface->edcode;
			mface->edcode= 0;
			if(a & ME_V1V2) mface->edcode |= ME_V3V1;
			if(a & ME_V2V3) mface->edcode |= ME_V1V2;
			if(a & ME_V3V1) mface->edcode |= ME_V2V3;
			if(a & DF_V1V2) mface->edcode |= DF_V3V1;
			if(a & DF_V2V3) mface->edcode |= DF_V1V2;
			if(a & DF_V3V1) mface->edcode |= DF_V2V3;
			
			a= mface->puno;
			mface->puno &= ~15;
			if(a & ME_FLIPV1) mface->puno |= ME_FLIPV2;
			if(a & ME_FLIPV2) mface->puno |= ME_FLIPV3;
			if(a & ME_FLIPV3) mface->puno |= ME_FLIPV1;
		}
	}
	else if(nr==4) {
		if(mface->v3==0 || mface->v4==0) {
			SWAP(int, mface->v1, mface->v3);
			SWAP(int, mface->v2, mface->v4);
			a= mface->edcode;
			mface->edcode= 0;
			if(a & ME_V1V2) mface->edcode |= ME_V3V4;
			if(a & ME_V2V3) mface->edcode |= ME_V2V3;
			if(a & ME_V3V4) mface->edcode |= ME_V1V2;
			if(a & ME_V4V1) mface->edcode |= ME_V4V1;
			if(a & DF_V1V2) mface->edcode |= DF_V3V4;
			if(a & DF_V2V3) mface->edcode |= DF_V2V3;
			if(a & DF_V3V4) mface->edcode |= DF_V1V2;
			if(a & DF_V4V1) mface->edcode |= DF_V4V1;

			a= mface->puno;
			mface->puno &= ~15;
			if(a & ME_FLIPV1) mface->puno |= ME_FLIPV3;
			if(a & ME_FLIPV2) mface->puno |= ME_FLIPV4;
			if(a & ME_FLIPV3) mface->puno |= ME_FLIPV1;
			if(a & ME_FLIPV4) mface->puno |= ME_FLIPV2;
		}
	}
}


void flipnorm_mesh(Mesh *me)
{
	MFace *mface;
	MVert *mvert;
	DispList *dl;
	float *fp;
	int a, temp;
	
	mvert= me->mvert;
	a= me->totvert;
	while(a--) {
		mvert->no[0]= -mvert->no[0];
		mvert->no[1]= -mvert->no[1];
		mvert->no[2]= -mvert->no[2];
		mvert++;
	}
	
	mface= me->mface;
	a= me->totface;
	while(a--) {
		if(mface->v3) {
			if(mface->v4) {
				SWAP(int, mface->v4, mface->v1);
				SWAP(int, mface->v3, mface->v2);
				test_index_mface(mface, 4);
				temp= mface->puno;
				mface->puno &= ~15;
				if(temp & ME_FLIPV1) mface->puno |= ME_FLIPV4;
				if(temp & ME_FLIPV2) mface->puno |= ME_FLIPV3;
				if(temp & ME_FLIPV3) mface->puno |= ME_FLIPV2;
				if(temp & ME_FLIPV4) mface->puno |= ME_FLIPV1;
			}
			else {
				SWAP(int, mface->v3, mface->v1);
				test_index_mface(mface, 3);
				temp= mface->puno;
				mface->puno &= ~15;
				if(temp & ME_FLIPV1) mface->puno |= ME_FLIPV3;
				if(temp & ME_FLIPV2) mface->puno |= ME_FLIPV2;
				if(temp & ME_FLIPV3) mface->puno |= ME_FLIPV1;
			}
		}
		mface++;
	}

	if(me->disp.first) {
		dl= me->disp.first;
		fp= dl->nors;
		if(fp) {
			a= dl->nr;
			while(a--) {
				fp[0]= -fp[0];
				fp[1]= -fp[1];
				fp[2]= -fp[2];
				fp+= 3;
			}
		}
	}
}

Mesh *get_mesh(Object *ob)
{
	
	if(ob==0) return 0;
	if(ob->type==OB_MESH) return ob->data;
	else if(ob->type==OB_SECTOR) {
		Sector *se= ob->data;
		if(se->flag & SE_SHOW_TEXMESH) {
			if(se->texmesh==0) {
				se->texmesh= add_mesh();
				rename_id((ID *)se->texmesh, "TexMesh");
				se->texmesh->flag &= ~ME_TWOSIDED;
			}
			return se->texmesh;
		}
		return se->dynamesh;
	}
	else if(ob->type==OB_LIFE) {
		Life *lf= ob->data;
		if(lf->flag & LF_SHOW_TEXMESH) {
			if(lf->texmesh==0) {
				lf->texmesh= add_mesh();
				rename_id((ID *)lf->texmesh, "TexMesh");
				lf->texmesh->flag &= ~ME_TWOSIDED;
			}
			return lf->texmesh;
		}
		return lf->dynamesh;
	}
	else return 0;
}

Mesh *get_other_mesh(Object *ob)
{
	
	if(ob==0) return 0;
	
	if(ob->type==OB_SECTOR) {
		Sector *se= ob->data;
		if(se->flag & SE_SHOW_TEXMESH) {
			return se->dynamesh;
		}
		return se->texmesh;
	}
	else if(ob->type==OB_LIFE) {
		Life *lf= ob->data;
		if(lf->flag & LF_SHOW_TEXMESH) {
			return lf->dynamesh;
		}
		return lf->texmesh;
	}
	else return 0;
}


void set_mesh(Object *ob, Mesh *me)
{
	Mesh *old=0;
	
	if(ob==0) return;
	
	if(ob->type==OB_MESH) {
		old= ob->data;
		old->id.us--;
		ob->data= me;
		id_us_plus((ID *)me);
	}
	else if(ob->type==OB_SECTOR) {
		Sector *se= ob->data;
		
		if(se->flag & SE_SHOW_TEXMESH) {
			old= se->texmesh;
			se->texmesh= me;
			
			if(me->tface==0) make_tfaces(me);
		}
		else {
			old= se->dynamesh;
			se->dynamesh= me;
		}
		if(old) old->id.us--;
		id_us_plus((ID *)me);
	}
	else if(ob->type==OB_LIFE) {
		Life *lf= ob->data;

		if(lf->flag & LF_SHOW_TEXMESH) {
			old= lf->texmesh;
			lf->texmesh= me;
			
			if(me->tface==0) make_tfaces(me);
		}
		else {
			old= lf->dynamesh;
			lf->dynamesh= me;
		}
		if(old) old->id.us--;
		id_us_plus((ID *)me);
	}
	
	test_object_materials((ID *)me);
}

void mball_to_mesh(ListBase *lb, Mesh *me)
{
	DispList *dl;
	MVert *mvert;
	MFace *mface;
	float *nors, *verts, nor[3];
	int a, *index;
	
	dl= lb->first;
	if(dl==0) return;

	if(dl->type==DL_INDEX4) {
		me->flag= ME_NOPUNOFLIP;
		me->totvert= dl->nr;
		me->totface= dl->parts;
		
		me->mvert=mvert= callocN(dl->nr*sizeof(MVert), "mverts");
		a= dl->nr;
		nors= dl->nors;
		verts= dl->verts;
		while(a--) {
			VECCOPY(mvert->co, verts);
			VECCOPY(nor, nors);
			VecMulf(nor, 32767.0);
			VECCOPY(mvert->no, nor);
			mvert++;
			nors+= 3;
			verts+= 3;
		}
		
		me->mface=mface= callocN(dl->parts*sizeof(MFace), "mface");
		a= dl->parts;
		index= dl->index;
		while(a--) {
			mface->v1= index[0];
			mface->v2= index[1];
			mface->v3= index[2];
			mface->v4= index[3];

			mface->puno= 15;
			mface->edcode= ME_V1V2+ME_V2V3;
			mface->flag = ME_SMOOTH;
			
			mface++;
			index+= 4;
		}

		test_object_materials((ID *)me);
	}	
}

void nurbs_to_mesh(Object *ob)
{
	Object *ob1;
	DispList *dl;
	Mesh *me;
	Curve *cu;
	MVert *mvert;
	MFace *mface;
	float *data, min[3], max[3];
	int a, b, len, ofs, vertcount, startvert, totvert=0, totvlak=0;
	int p1, p2, p3, p4, *index;

	min[0]= min[1]= min[2]= 1.0e10;
	max[0]= max[1]= max[2]= -1.0e10;

	cu= ob->data;

	if(ob->type==OB_CURVE) {
		/* regel: dl->type INDEX3 altijd vooraan in lijst */
		dl= cu->disp.first;
		if(dl->type!=DL_INDEX3) {
			curve_to_filledpoly(ob->data, &cu->disp);
		}
	}

	/* tellen */
	dl= cu->disp.first;
	while(dl) {
		if(dl->type==DL_SEGM) {
			totvert+= dl->parts*dl->nr;
			totvlak+= dl->parts*(dl->nr-1);
		}
		else if(dl->type==DL_POLY) {
			/* cyclic polys are filled. except when 3D */
			if(cu->flag & CU_3D) {
				totvert+= dl->parts*dl->nr;
				totvlak+= dl->parts*dl->nr;
			}
		}
		else if(dl->type==DL_SURF) {
			totvert+= dl->parts*dl->nr;
			totvlak+= (dl->parts-1+((dl->flag & 2)==2))*(dl->nr-1+(dl->flag & 1));
		}
		else if(dl->type==DL_INDEX3) {
			totvert+= dl->nr;
			totvlak+= dl->parts;
		}
		dl= dl->next;
	}
	if(totvert==0) {
		error("can't convert");
		return;
	}

	/* mesh maken */
	me= add_mesh("CuMesh");
	me->totvert= totvert;
	me->totface= totvlak;

	me->totcol= cu->totcol;
	me->mat= cu->mat;
	cu->mat= 0;
	cu->totcol= 0;

	mvert=me->mvert= callocN(me->totvert*sizeof(MVert), "cumesh1");
	mface=me->mface= callocN(me->totface*sizeof(MFace), "cumesh2");

	/* verts en vlakken */
	vertcount= 0;

	dl= cu->disp.first;
	while(dl) {
		if(dl->type==DL_SEGM) {
			startvert= vertcount;
			a= dl->parts*dl->nr;
			data= dl->verts;
			while(a--) {
				VECCOPY(mvert->co, data);
				data+=3;
				vertcount++;
				mvert++;
			}

			for(a=0; a<dl->parts; a++) {
				ofs= a*dl->nr;
				for(b=1; b<dl->nr; b++) {
					mface->v1= startvert+ofs+b-1;
					mface->v2= startvert+ofs+b;
					mface->edcode= ME_V1V2;
					test_index_mface(mface, 2);
					mface++;
				}
			}

		}
		else if(dl->type==DL_POLY) {
			/* cyclic polys are filled */
			/* startvert= vertcount;
			a= dl->parts*dl->nr;
			data= dl->verts;
			while(a--) {
				VECCOPY(mvert->co, data);
				data+=3;
				vertcount++;
				mvert++;
			}

			for(a=0; a<dl->parts; a++) {
				ofs= a*dl->nr;
				for(b=0; b<dl->nr; b++) {
					mface->v1= startvert+ofs+b;
					if(b==dl->nr-1) mface->v2= startvert+ofs;
					else mface->v2= startvert+ofs+b+1;
					mface->edcode= ME_V1V2;
					test_index_mface(mface, 2);
					mface++;
				}
			}
			*/
		}
		else if(dl->type==DL_INDEX3) {
			startvert= vertcount;
			a= dl->nr;
			data= dl->verts;
			while(a--) {
				VECCOPY(mvert->co, data);
				data+=3;
				vertcount++;
				mvert++;
			}

			a= dl->parts;
			index= dl->index;
			while(a--) {
				mface->v1= startvert+index[0];
				mface->v2= startvert+index[1];
				mface->v3= startvert+index[2];
				mface->v4= 0;
	
				mface->puno= 7;
				mface->edcode= ME_V1V2+ME_V2V3;
				test_index_mface(mface, 3);
				
				mface++;
				index+= 3;
			}
	
	
		}
		else if(dl->type==DL_SURF) {
			startvert= vertcount;
			a= dl->parts*dl->nr;
			data= dl->verts;
			while(a--) {
				VECCOPY(mvert->co, data);
				data+=3;
				vertcount++;
				mvert++;
			}

			for(a=0; a<dl->parts; a++) {

				if( (dl->flag & 2)==0 && a==dl->parts-1) break;

				if(dl->flag & 1) {				/* p2 -> p1 -> */
					p1= startvert+ dl->nr*a;	/* p4 -> p3 -> */
					p2= p1+ dl->nr-1;			/* -----> volgende rij */
					p3= p1+ dl->nr;
					p4= p2+ dl->nr;
					b= 0;
				}
				else {
					p2= startvert+ dl->nr*a;
					p1= p2+1;
					p4= p2+ dl->nr;
					p3= p1+ dl->nr;
					b= 1;
				}
				if( (dl->flag & 2) && a==dl->parts-1) {
					p3-= dl->parts*dl->nr;
					p4-= dl->parts*dl->nr;
				}

				for(; b<dl->nr; b++) {
					mface->v1= p1;
					mface->v2= p3;
					mface->v3= p4;
					mface->v4= p2;
					mface->mat_nr= dl->col;
					mface->edcode= ME_V1V2+ME_V2V3;
					test_index_mface(mface, 4);
					mface++;

					p4= p3; 
					p3++;
					p2= p1; 
					p1++;
				}
			}

		}

		dl= dl->next;
	}

	if(ob->data) {
		free_libblock(&G.main->curve, ob->data);
	}
	ob->data= me;
	ob->type= OB_MESH;
	
	tex_space_mesh(me);
	
	/* andere users */
	ob1= G.main->object.first;
	while(ob1) {
		if(ob1->data==cu) {
			ob1->type= OB_MESH;
		
			ob1->data= ob->data;
			id_us_plus((ID *)ob->data);
		}
		ob1= ob1->id.next;
	}

}

/* ********************* SUBDIV MESH ******************** */

/* globals for the smoother */

typedef struct VecNor {
	float vec[3], nor[3], weight, len;
} VecNor;

typedef struct SMeshFace {
	void *next, *prev;
	MFace *mface;
	DispList *dl;
} SMeshFace;

static VecNor *vecnormain;
static int subdivlevel=1;
static ListBase *smfaces;

void add_smface(MFace *mface, DispList *dl, int index)
{
	ListBase *lb;
	SMeshFace *smf;
	
	lb= smfaces+index;
	smf= lb->first;
	while(smf) {
		if ELEM3(index, smf->mface->v1, smf->mface->v2, smf->mface->v3) return;
		if(smf->mface->v4 && smf->mface->v4==index) return;
		smf= smf->next;
	}
	
	/* not found, insert new one */
	
	smf= mallocN(sizeof(SMeshFace), "smf");
	addtail(lb, smf);
	smf->mface= mface;
	smf->dl= dl;
}

SMeshFace *find_smface(int index)
{
	ListBase *lb;
	SMeshFace *smf;
	
	lb= smfaces+index;
	smf= lb->first;
	while(smf) {
		if ELEM3(index, smf->mface->v1, smf->mface->v2, smf->mface->v3) return smf;
		if(smf->mface->v4 && smf->mface->v4==index) return smf;
		smf= smf->next;
	}
	return 0;	
}

void normals_s_mesh(Mesh *me)
{
	DispList *dl;
	MFace *mface;
	float *fp, *no, *v1, *v2, *v3;
	int a, u, v, rowlen, ind2, ind3, ind4;
	
	if(me->totvert==0 || me->totface==0) return;
	
	/* make the look up table */
	smfaces= callocN(me->totvert*sizeof(ListBase), "smfaces");
	
	mface= me->mface;
	dl= me->disp.first;
	for(a=0; a<me->totface; a++, mface++) {
	
		if(mface->v3==0) continue;
		
		add_smface(mface, dl, MIN2(mface->v1, mface->v2));
		add_smface(mface, dl, MIN2(mface->v2, mface->v3));
		if(mface->v4) {
			add_smface(mface, dl, MIN2(mface->v3, mface->v4));
			add_smface(mface, dl, MIN2(mface->v4, mface->v1));
		}	
		else add_smface(mface, dl, MIN2(mface->v3, mface->v1));

		dl= dl->next;
		if(dl==0 && a==me->totface-1) {	/* temporal safety */
			printf("error in displist normals_s_mesh\n");
		}
	}
	
	/* calc normals */
	
	mface= me->mface;
	dl= me->disp.first;
	rowlen= dl->nr;
	for(a=0; a<me->totface; a++, mface++) {
	
		if(mface->v3==0) continue;
		
		fp= dl->verts;
		no= dl->nors;
			
		for(v=0; v<rowlen; v++) {
			for(u=0; u<rowlen; u++, fp+=3, no+=3) {
				
				v1= fp;
				
				if(u==rowlen-1);
				else v2= fp+3;
				
				if(v==rowlen-1);
				else v3= fp+3*rowlen;
				
				CalcNormFloat(v1, v2, v3, no);

			}
		}
		
		
		dl= dl->next;
	}
	
	/* free */
	
	for(a=0; a<me->totvert; a++) {
		freelistN(smfaces+a);
	}
	
	freeN(smfaces);
}

void maakbez_delta(float q0, float q1, float q2, float q3, float *p, float *d, int it)
{
	float rt0,rt1,rt2,rt3,f;
	int a;

	f= (float)it;
	rt0= q0;
	rt1= 3.0*(q1-q0)/f;
	f*= f;
	rt2= 3.0*(q0-2*q1+q2)/f;
	f*= (float)it;
	rt3= (q3-q0+3.0*(q1-q2))/f;
 	
  	q0= rt0;
	q1= rt1+rt2+rt3;
	q2= 2*rt2+6*rt3;
	q3= 6*rt3;
  
  	for(a=0; a<=it; a++) {
		*p= q0;
		*d= q1;
		p+= 3;
		d+= 3;
		q0+= q1;
 		q1+= q2;
 		q2+= q3;
 	}
}

void smooth_subdiv_line(VecNor *vn, int len, int step, int do_norm)
{
	extern float editbutsize;	/* buttons.c */
	VecNor *vn1, *vn2;
	float mfac, fac, veclen, fstep, sin1, sin2, cos1, cos2, edge[3];
	float h1[3], h2[3], ednor1[3], ednor2[3], nora[3];
	float vec[15][3];
	float nor[15][3];
	int a;
	
	if(len>13) return;

	vn1= vn;
	vn2= vn+step*(len-1);

	VecSubf(edge, vn1->vec, vn2->vec);
	
	veclen= Normalise(edge);

	/* cosine angle */
	cos1= (edge[0]*vn1->nor[0] + edge[1]*vn1->nor[1] + edge[2]*vn1->nor[2]);
	cos2= (-edge[0]*vn2->nor[0] - edge[1]*vn2->nor[1] - edge[2]*vn2->nor[2]);

	sin1= safsqrt(1.0 - cos1*cos1);
	sin2= safsqrt(1.0 - cos2*cos2);

	/* the 'real' normals */
	ednor1[0]= vn1->nor[0] - cos1*edge[0];
	ednor1[1]= vn1->nor[1] - cos1*edge[1];
	ednor1[2]= vn1->nor[2] - cos1*edge[2];
	Normalise(ednor1);
	
	ednor2[0]= vn2->nor[0] + cos2*edge[0];
	ednor2[1]= vn2->nor[1] + cos2*edge[1];
	ednor2[2]= vn2->nor[2] + cos2*edge[2];
	Normalise(ednor2);
	
	/* the handles */
	if(veclen>vn1->len) {
		if(vn1->len==0.0) fac= 0.0;
		else {
			mfac= 2.0*((vn1->len)/(veclen+vn1->len));
			mfac= fsqrt(mfac);
			fac= .375*( (1.0-mfac)*vn1->len + (mfac)*veclen );
		}	
	}
	else fac= .375*veclen;

	h1[0]= vn1->vec[0] + fac*(-sin1*edge[0] + cos1*(ednor1[0]) );
	h1[1]= vn1->vec[1] + fac*(-sin1*edge[1] + cos1*(ednor1[1]) );
	h1[2]= vn1->vec[2] + fac*(-sin1*edge[2] + cos1*(ednor1[2]) );
	
	if(veclen>vn2->len) {
		if(vn2->len==0.0) fac= 0.0;
		else {
			mfac= 2.0*((vn2->len)/(veclen+vn2->len));
			mfac= fsqrt(mfac);
			fac= .375*( (1.0-mfac)*vn2->len + (mfac)*veclen );
		}	
	}
	else fac= .375*veclen;

	h2[0]= vn2->vec[0] + fac*(sin2*edge[0] + cos2*(ednor2[0]) );
	h2[1]= vn2->vec[1] + fac*(sin2*edge[1] + cos2*(ednor2[1]) );
	h2[2]= vn2->vec[2] + fac*(sin2*edge[2] + cos2*(ednor2[2]) );
	
	/* interpolate */
	maakbez_delta(vn1->vec[0], h1[0], h2[0], vn2->vec[0], vec[0], nor[0], len-1);
	maakbez_delta(vn1->vec[1], h1[1], h2[1], vn2->vec[1], vec[0]+1, nor[0]+1, len-1);
	maakbez_delta(vn1->vec[2], h1[2], h2[2], vn2->vec[2], vec[0]+2, nor[0]+2, len-1);

	fac= fstep= 1.0/((float)len-1.0);

	vn+= step;
	for(a=1; a<len-1; a++, vn+=step, fac+=fstep) {
		
		mfac= 1.0-fac;
	
		/* normal */
		ednor1[0]= mfac*vn1->nor[0] + fac*vn2->nor[0];
		ednor1[1]= mfac*vn1->nor[1] + fac*vn2->nor[1];
		ednor1[2]= mfac*vn1->nor[2] + fac*vn2->nor[2];
		
		/* make it symmetrical */
		VecAddf(nora, nor[a-1], nor[a]);
		
		if(nora[0]==0.0 && nora[1]==0.0 && nora[2]==0) {
			/* this happens with triangles */
			/* use ednor1 */
		}
		else {
			Crossf(ednor2, ednor1, nora);
			Crossf(ednor1, nora, ednor2);
		}
		
		if(do_norm) {
			VECCOPY(vn->nor, ednor1);
			Normalise(vn->nor);
		}
		else {
			VecAddf(vn->nor, vn->nor, ednor1);
		}

		VecAddf(vn->vec, vn->vec, vec[a]);
		
		vn->len= mfac*vn1->len + fac*vn2->len;

		/* if you only want to examine the handle */	
		/* if(a==1) VecAddf(vn->vec, vn->vec, h1); */
		/* else VecAddf(vn->vec, vn->vec, h2); */
	}
}

DispList *quad_smooth(ListBase *lb, VecNor *vn1, VecNor *vn2, VecNor *vn3, VecNor *vn4)
{
	DispList *dl;
	VecNor *vn;
	float *fp, *no, nor1[3], nor2[3];
	int a, b, rowlen;
	
	/* setup the subdivider */
	rowlen= 1+subdivlevel;
	
	bzero(vecnormain, sizeof(VecNor)*(subdivlevel+2)*(subdivlevel+2));
	
	vn= vecnormain;
	*vn= *vn1;
	vn= vecnormain + rowlen-1;
	*vn= *vn2;
	vn= vecnormain + rowlen*(rowlen-1);
	*vn= *vn4;
	vn= vecnormain + rowlen*(rowlen-1) + rowlen-1;
	*vn= *vn3;
	
	/* subidvide the borders */
	vn= vecnormain;
	smooth_subdiv_line(vn, rowlen, 1, TRUE);
	smooth_subdiv_line(vn, rowlen, rowlen, TRUE);
	vn= vecnormain + rowlen-1;
	smooth_subdiv_line(vn, rowlen, rowlen, TRUE);
	vn= vecnormain + rowlen*(rowlen-1);
	smooth_subdiv_line(vn, rowlen, 1, TRUE);

	/* horizontal interpolation */
	vn= vecnormain+rowlen;
	for(a=1; a<rowlen-1; a++, vn+=rowlen) {
		smooth_subdiv_line(vn, rowlen, 1, FALSE);
	}

	/* vertical interpolation */
	vn= vecnormain+1;
	for(a=1; a<rowlen-1; a++, vn++) {
		smooth_subdiv_line(vn, rowlen, rowlen, FALSE);
	}
	
	/* divide by 2 and normalize */
	vn= vecnormain;
	for(a=0; a<rowlen; a++) {
		for(b=0; b<rowlen; b++, vn++) {
			if(a>0 && a<rowlen-1) {
				if(b>0 && b<rowlen-1) {
					vn->vec[0]*= 0.5;
					vn->vec[1]*= 0.5;
					vn->vec[2]*= 0.5;
					Normalise(vn->nor);
				}
			}
		}
	}
	
	/* make displist */
	dl= callocN(sizeof(DispList), "filldisplist");
	dl->type= DL_SURF;
	dl->col= 0;
	dl->nr= rowlen;
	dl->parts= rowlen;
	addtail(lb, dl);
	fp= dl->verts= mallocN(rowlen*rowlen*3*sizeof(float), "dlverts");
	no= dl->nors= mallocN(rowlen*rowlen*3*sizeof(float), "dlnors");

	vn= vecnormain;
	a= rowlen*rowlen;
	while(a--) {
		VECCOPY(fp, vn->vec);
		VECCOPY(no, vn->nor);
		vn++;
		fp+= 3;
		no+= 3;
	}
	
	return dl;	
}

void vert_smooth_add( VecNor *vn1, VecNor *vn2, float *v1, float *v2)
{
	extern float extr_offs;	/* buttons.c */
	float  fac, cos1, cos2, nor[3];
	
	VecSubf(nor, v1, v2);
	Normalise(nor);
	
	/* cosine angle */
	cos1= (nor[0]*vn1->nor[0] + nor[1]*vn1->nor[1] + nor[2]*vn1->nor[2]);
	cos2= (-nor[0]*vn2->nor[0] - nor[1]*vn2->nor[1] - nor[2]*vn2->nor[2]);

	/* new vertices */
	fac= 1.0*cos1*vn1->len;
	vn1->vec[0]+= v1[0] - fac*vn1->nor[0];
	vn1->vec[1]+= v1[1] - fac*vn1->nor[1];
	vn1->vec[2]+= v1[2] - fac*vn1->nor[2];
	vn1->weight+= 1.0;
	
	fac= 1.0*cos2*vn2->len;
	vn2->vec[0]+= v2[0] - fac*vn2->nor[0];
	vn2->vec[1]+= v2[1] - fac*vn2->nor[1];
	vn2->vec[2]+= v2[2] - fac*vn2->nor[2];
	vn2->weight+= 1.0;
}

void make_s_editmesh(Object *ob)
{
	VecNor *vn, *vertmain, *vn1, *vn2;
	DispList *dl, *dln;
	Mesh *me;
	EditVert *v1, *v2, *v3, *v4;
	EditEdge *eed;
	EditVlak *evl;
	float len;
	int a, totvert;
	
	me= ob->data;

	/* first free old displists */
	dl= me->disp.first;
	while(dl) {
		dln= dl->next;
		if(dl->type==DL_SURF) {
			remlink(&me->disp, dl);
			free_disp_elem(dl);
		}
		dl= dln;
	}

	subdivlevel= me->subdiv;
	if(subdivlevel<2) return;
	if(G.edvl.first==NULL) return;
	
	vecnormain= mallocN( sizeof(VecNor)*(subdivlevel+2)*(subdivlevel+2), "vecnormain");
	
	/* only in editmode, do this: */
	/* if(G.obedit) { */
		recalc_editnormals();
		vertexnormals(0);		/* no flip */
	/* } */

	/* PHASE1: make new vertex locations based at smoothing geometry */
	totvert= 0;
	v1= G.edve.first;
	while(v1) {
		totvert++;
		v1= v1->next;
	}

	vn= vertmain= callocN( sizeof(VecNor)*totvert, "vertmain");

	v1= G.edve.first;
	while(v1) {
		v1->vn= (EditVert *)vn;
		VECCOPY(vn->nor, v1->no); 
		vn->len= -1.0;	/* signal */
		vn++;
		v1= v1->next;
	}

	/* for each vertex, calculate the minimum edge length, that will be the maximum handle size */
	eed= G.eded.first;
	while(eed) {
		len= VecLenf(eed->v1->co, eed->v2->co);
		
		vn= (VecNor *)eed->v1->vn;
		if(vn->len == -1) vn->len= len;
		else vn->len= MIN2(len, vn->len);
	
		vn= (VecNor *)eed->v2->vn;
		if(vn->len == -1) vn->len= len;
		else vn->len= MIN2(len, vn->len);

		eed= eed->next;
	}
	
	
	evl= G.edvl.first;
	while(evl) {
		vert_smooth_add((VecNor *)evl->v1->vn, (VecNor *)evl->v2->vn, evl->v1->co, evl->v2->co);
		vert_smooth_add((VecNor *)evl->v2->vn, (VecNor *)evl->v3->vn, evl->v2->co, evl->v3->co);
		if(evl->v4) {
			vert_smooth_add((VecNor *)evl->v3->vn, (VecNor *)evl->v4->vn, evl->v3->co, evl->v4->co);
			vert_smooth_add((VecNor *)evl->v4->vn, (VecNor *)evl->v1->vn, evl->v4->co, evl->v1->co);
		}
		else {
			vert_smooth_add((VecNor *)evl->v3->vn, (VecNor *)evl->v1->vn, evl->v3->co, evl->v1->co);
		}
		evl= evl->next;
	}
	
	vn= vertmain;
	a= totvert;
	while(a--) {
		if(vn->weight!=0.0) VecMulf(vn->vec, 1.0/vn->weight);
		/* vn->len= -1.0; */
		vn++;
	}
	
	
	/* for each vertex, we recalculate the allowed edlen again, is needed to make nice spheres */
	eed= G.eded.first;
	eed= 0;
	while(eed) {
		vn1= (VecNor *)eed->v1->vn;
		vn2= (VecNor *)eed->v2->vn;
		
		len= VecLenf(vn1->vec, vn2->vec);
		
		if(vn1->len == -1) vn1->len= len;
		else vn1->len= MIN2(len, vn1->len);
	
		if(vn2->len == -1) vn2->len= len;
		else vn2->len= MIN2(len, vn2->len);

		eed= eed->next;
	}
	
	
	/* here it goes! */
	
	evl= G.edvl.first;
	while(evl) {
		v1= evl->v1;
		v2= evl->v2;
		v3= evl->v3;
		if(evl->v4) {
			v4= evl->v4;
			if(v1->h==0 && v2->h==0 && v3->h==0 && v4->h==0) {
				dl= quad_smooth(&me->disp, ((VecNor *)v1->vn),  ((VecNor *)v2->vn),  ((VecNor *)v3->vn),  ((VecNor *)v4->vn)); 
				dl->col= evl->mat_nr;
		
			}
		}
		else if(evl->v3) {
			if(v1->h==0 && v2->h==0 && v3->h==0) {
				dl= quad_smooth(&me->disp, ((VecNor *)v1->vn),  ((VecNor *)v2->vn),  ((VecNor *)v3->vn),  ((VecNor *)v3->vn));
				dl->col= evl->mat_nr;

			}
		}
		evl= evl->next;
	}
	
	freeN(vecnormain);
	freeN(vertmain);

}

void make_s_mesh(Object *ob)
{
	Mesh *me;
	MFace *mface;
	MVert *mvert;
	DispList *dlverts;
	EditVert *eve, **evlist, *eve1, *eve2, *eve3, *eve4;
	EditVlak *evl;
	int tot, a;

	if(G.obedit && ob==G.obedit) {
		printf("error: cant calculate smesh\n");
		return;
	}
	if(G.edve.first) {
		/* printf("warn: editverts!\n"); */
		/* happens when rendering in editmode */
		return;
	}

	me= ob->data;
	tot= me->totvert;

	/* we convert the mesh to editvertices, but not like editmode, this is including deform */

	/* deform information */
	dlverts= find_displist(&ob->disp, DL_VERTS);

	mvert= me->mvert;
	evlist= (EditVert **)mallocN((tot+1)*sizeof(void *),"evlist");
	for(a=0; a<tot; a++, mvert++) {
	
		if(dlverts) eve= addvertlist(dlverts->verts+3*a);
		else eve= addvertlist(mvert->co);
		
		evlist[a]= eve;

		/* eve->no[0]= mvert->no[0]/32767.0; */
		/* eve->no[1]= mvert->no[1]/32767.0; */
		/* eve->no[2]= mvert->no[2]/32767.0; */
	}

	/* edges en vlakken maken */
	mface= me->mface;
	/* mcol= (uint *)me->mcol; */
	/* if(me->mcol) tface= &tempface; */
	
	for(a=0; a<me->totface; a++, mface++) {
		eve1= evlist[mface->v1];
		eve2= evlist[mface->v2];
		if(mface->v3) eve3= evlist[mface->v3]; else eve3= 0;
		if(mface->v4) eve4= evlist[mface->v4]; else eve4= 0;
		
		/* if(mcol) memcpy(tempface.col, mcol, 16); */
		
		evl= addvlaklist(eve1, eve2, eve3, eve4, mface->mat_nr, mface->flag, 0);
		
		/* if(mcol) mcol+=4; */
	}

	freeN(evlist);

	G.totvert= me->totvert;
	G.totface= me->totface;

	make_s_editmesh(ob);

	free_editmesh();
	
}


/* ***************** */

/* this one for NOT in editmode */

void vertexnormals_mesh(Mesh *me, float *extverts)
{
	MVert *mvert;
	MFace *mface;
	float n1[3], n2[3], n3[3], n4[3], co[4], fac1, fac2, fac3, fac4, *temp;
	float *f1, *f2, *f3, *f4, xn, yn, zn, *normals;
	float *v1, *v2, *v3, *v4, opp, len, vnor[3];
	int a, testflip;

	if(me->totvert==0) return;

	testflip= (me->flag & ME_NOPUNOFLIP)==0;
	if(me->flag & ME_SMESH) testflip= 0;
	
	if(me->totface==0) {
		/* namaak puno's voor halopuno! */
		mvert= me->mvert;
		for(a=0; a<me->totvert; a++, mvert++) {
			VECCOPY(n1, mvert->co);
			Normalise(n1);
			mvert->no[0]= 32767.0*n1[0];
			mvert->no[1]= 32767.0*n1[1];
			mvert->no[2]= 32767.0*n1[2];
		}
		return;
	}

	if((me->flag & ME_TWOSIDED)==0) testflip= 0;	/* grote hoeken */
	
	normals= callocN(me->totvert*3*sizeof(float), "normals");
	
	/* berekenen cos hoeken en oppervlakte en optellen bij puno */
	mface= me->mface;
	mvert= me->mvert;
	for(a=0; a<me->totface; a++, mface++) {
		
		if(mface->v3==0) continue;
		
		if(extverts) {
			v1= extverts+3*mface->v1;
			v2= extverts+3*mface->v2;
			v3= extverts+3*mface->v3;
			v4= extverts+3*mface->v4;
		}
		else {		
			v1= (mvert+mface->v1)->co;
			v2= (mvert+mface->v2)->co;
			v3= (mvert+mface->v3)->co;
			v4= (mvert+mface->v4)->co;
		}
		
		VecSubf(n1, v2, v1);
		VecSubf(n2, v3, v2);
		Normalise(n1);
		Normalise(n2);

		if(mface->v4==0) {
			VecSubf(n3, v1, v3);
			Normalise(n3);
			
			co[0]= safacos(-n3[0]*n1[0]-n3[1]*n1[1]-n3[2]*n1[2]);
			co[1]= safacos(-n1[0]*n2[0]-n1[1]*n2[1]-n1[2]*n2[2]);
			co[2]= safacos(-n2[0]*n3[0]-n2[1]*n3[1]-n2[2]*n3[2]);
			
		}
		else {
			VecSubf(n3, v4, v3);
			VecSubf(n4, v1, v4);
			Normalise(n3);
			Normalise(n4);

			co[0]= safacos(-n4[0]*n1[0]-n4[1]*n1[1]-n4[2]*n1[2]);
			co[1]= safacos(-n1[0]*n2[0]-n1[1]*n2[1]-n1[2]*n2[2]);
			co[2]= safacos(-n2[0]*n3[0]-n2[1]*n3[1]-n2[2]*n3[2]);
			co[3]= safacos(-n3[0]*n4[0]-n3[1]*n4[1]-n3[2]*n4[2]);
		}
		
		CalcNormFloat(v1, v2, v3, vnor);
		
		temp= normals+3*mface->v1;
		if(testflip && contrpuntnorm(vnor, temp) ) co[0]= -co[0];
		temp[0]+= co[0]*vnor[0];
		temp[1]+= co[0]*vnor[1];
		temp[2]+= co[0]*vnor[2];
		
		temp= normals+3*mface->v2;
		if(testflip && contrpuntnorm(vnor, temp) ) co[1]= -co[1];
		temp[0]+= co[1]*vnor[0];
		temp[1]+= co[1]*vnor[1];
		temp[2]+= co[1]*vnor[2];
		
		temp= normals+3*mface->v3;
		if(testflip && contrpuntnorm(vnor, temp) ) co[2]= -co[2];
		temp[0]+= co[2]*vnor[0];
		temp[1]+= co[2]*vnor[1];
		temp[2]+= co[2]*vnor[2];
		
		if(mface->v4) {
			temp= normals+3*mface->v4;
			if(testflip && contrpuntnorm(vnor, temp) ) co[3]= -co[3];
			temp[0]+= co[3]*vnor[0];
			temp[1]+= co[3]*vnor[1];
			temp[2]+= co[3]*vnor[2];
		}
	}

	/* normaliseren puntnormalen */
	mvert= me->mvert;
	for(a=0; a<me->totvert; a++, mvert++) {
		len= Normalise(normals+3*a);
		if(len!=0.0) {
			VECCOPY(n1, normals+3*a);
			Normalise(n1);

			mvert->no[0]= 32767.0*n1[0];
			mvert->no[1]= 32767.0*n1[1];
			mvert->no[2]= 32767.0*n1[2];
		}
	}
	
	/* puntnormaal omklap-vlaggen voor bij shade */
	mface= me->mface;
	mvert= me->mvert;
	for(a=0; a<me->totface; a++, mface++) {
		mface->puno=0;			
		
		if(mface->v3==0) continue;
		
		if(extverts) {
			v1= extverts+3*mface->v1;
			v2= extverts+3*mface->v2;
			v3= extverts+3*mface->v3;
		}
		else {		
			v1= (mvert+mface->v1)->co;
			v2= (mvert+mface->v2)->co;
			v3= (mvert+mface->v3)->co;
		}

		CalcNormFloat(v1, v2, v3, vnor);

		if(testflip) {
			f1= normals + 3*mface->v1;
			f2= normals + 3*mface->v2;
			f3= normals + 3*mface->v3;

			fac1= vnor[0]*f1[0] + vnor[1]*f1[1] + vnor[2]*f1[2];
			if(fac1<0.0) {
				mface->puno = ME_FLIPV1;
			}
			fac2= vnor[0]*f2[0] + vnor[1]*f2[1] + vnor[2]*f2[2];
			if(fac2<0.0) {
				mface->puno += ME_FLIPV2;
			}
			fac3= vnor[0]*f3[0] + vnor[1]*f3[1] + vnor[2]*f3[2];
			if(fac3<0.0) {
				mface->puno += ME_FLIPV3;
			}
			if(mface->v4) {
				f4= normals + 3*mface->v4;
				fac4= vnor[0]*f4[0] + vnor[1]*f4[1] + vnor[2]*f4[2];
				if(fac4<0.0) {
					mface->puno += ME_FLIPV4;
				}
			}
		}
		/* proj voor cubemap! */
		xn= fabs(vnor[0]);
		yn= fabs(vnor[1]);
		zn= fabs(vnor[2]);
		
		if(zn>xn && zn>yn) mface->puno += ME_PROJXY;
		else if(yn>xn && yn>zn) mface->puno += ME_PROJXZ;
		else mface->puno += ME_PROJYZ;
		
	}
	
	freeN(normals);
}

void edge_drawflags_mesh(Mesh *me)
{
	MFace *mface;
	int a;
	
	mface= me->mface;
	for(a=0; a<me->totface; a++, mface++) {
		mface->edcode= ME_V1V2|ME_V2V3|ME_V3V4|ME_V4V1;
	}
}

