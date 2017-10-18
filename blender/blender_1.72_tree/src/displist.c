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


/*  displist.c      GRAPHICS
 * 
 *  
 *  maart 95
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "edit.h"
#include "render.h"


FastLamp *flar[MAXLAMP];
float fviewmat[4][4];
int totflamp=0;

void free_disp_elem(DispList *dl)
{
	
	if(dl) {
		if(dl->verts) freeN(dl->verts);
		if(dl->nors) freeN(dl->nors);
		if(dl->index) freeN(dl->index);
		if(dl->col1) freeN(dl->col1);
		if(dl->col2) freeN(dl->col2);
		freeN(dl);
	}
}

void freedisplist(ListBase *lb)
{
	DispList *dl;
	
	while(dl= lb->first) {
		remlink(lb, dl);
		free_disp_elem(dl);
	}
}


DispList *find_displist_create(ListBase *lb, int type)
{
	DispList *dl;
	
	dl= lb->first;
	while(dl) {
		if(dl->type==type) return dl;
		dl= dl->next;
	}

	dl= callocN(sizeof(DispList), "find_disp");
	dl->type= type;
	addtail(lb, dl);

	return dl;
}

DispList *find_displist(ListBase *lb, int type)
{
	DispList *dl;
	
	dl= lb->first;
	while(dl) {
		if(dl->type==type) return dl;
		dl= dl->next;
	}

	return 0;
}

void copy_displist(ListBase *lbn, ListBase *lb)
{
	DispList *dln, *dl;
	
	lbn->first= lbn->last= 0;
	
	dl= lb->first;
	while(dl) {
		
		dln= dupallocN(dl);
		addtail(lbn, dln);
		dln->verts= dupallocN(dl->verts);
		dln->nors= dupallocN(dl->nors);
		dln->index= dupallocN(dl->index);
		dln->col1= dupallocN(dl->col1);
		dln->col2= dupallocN(dl->col2);
		
		dl= dl->next;
	}
}

void initfastshade()
{
	Base *base;
	Object *ob;
	Lamp *la;
	FastLamp *fl;
	float mat[4][4];
	int a;

	R.vlr= 0;
	
	init_render_world();
	
	if(totflamp) return;
	if(G.scene->camera==0) G.scene->camera= find_camera();
	if(G.scene->camera==0) return;

	/* uit roteerscene gejat */
	where_is_object(G.scene->camera);
	Mat4CpyMat4(R.viewinv, G.scene->camera->obmat);
	Mat4Ortho(R.viewinv);
	Mat4Invert(fviewmat, R.viewinv);


	/* initrendertexture(); */

	base= FIRSTBASE;
	while(base) {
		ob= base->object;
		if( ob->type==OB_LAMP && (base->lay & G.vd->lay)) {
			
			Mat4MulMat4(mat, ob->obmat, fviewmat);
			
			la= ob->data;
			fl= mallocN(sizeof(FastLamp), "initfastshade2");
			flar[totflamp++]= fl;

			fl->type= la->type;
			fl->mode= la->mode;
			fl->lay= base->lay;
			
			fl->vec[0]= mat[2][0];
			fl->vec[1]= mat[2][1];
			fl->vec[2]= mat[2][2];
			Normalise(fl->vec);
			
			fl->co[0]= mat[3][0];
			fl->co[1]= mat[3][1];
			fl->co[2]= mat[3][2];

			fl->dist= la->dist;
			fl->distkw= fl->dist*fl->dist;
			fl->att1= la->att1;
			fl->att2= la->att2;

			fl->spotsi= fcos( M_PI*la->spotsize/360.0 );
			fl->spotbl= (1.0-fl->spotsi)*la->spotblend;
	
			fl->r= la->energy*la->r;
			fl->g= la->energy*la->g;
			fl->b= la->energy*la->b;
		}
		
		if(base->next==0 && G.scene->set && base==G.scene->base.last) base= G.scene->set->base.first;
		else base= base->next;
	}
}


void freefastshade()
{
	int a;

	for(a=0; a<totflamp; a++) {
		freeN(flar[a]);
		flar[a]=0;
	}
	totflamp= 0;
}


void fastshade(float *co, float *nor, float *orco, Material *ma, char *col1, char *col2, char *vertcol)
{
	FastLamp *fl;
	extern Material defmaterial;	/* initrender.c */
	float i, t, inp, soft, inpr, inpg, inpb, isr=0, isg=0, isb=0, lv[3], lampdist, ld;
	float inpr1, inpg1, inpb1, isr1=0, isg1=0, isb1=0;
	int a, back;
	char col[3];

	if(ma==0) return;
	R.mat= ma;
	R.matren= ma->ren;
	ma= R.matren;

	if(ma->mode & MA_VERTEXCOLP) {
		if(vertcol) {
			ma->r= vertcol[3]/255.0;
			ma->g= vertcol[2]/255.0;
			ma->b= vertcol[1]/255.0;
		}
	}
	
	if(ma->texco) {
		VECCOPY(R.lo, orco);
		VECCOPY(R.vn, nor);
		
		if(ma->texco & TEXCO_GLOB) {
			VECCOPY(R.gl, R.lo);
		}
		if(ma->texco & TEXCO_WINDOW) {
			VECCOPY(R.winco, R.lo);
		}
		if(ma->texco & TEXCO_STICKY) {
			VECCOPY(R.sticky, R.lo);
		}
		if(ma->texco & TEXCO_UV) {
			VECCOPY(R.uv, R.lo);
		}
		if(ma->texco & TEXCO_OBJECT) {
			VECCOPY(R.co, R.lo);
		}
		if(ma->texco & TEXCO_NORM) {
			VECCOPY(R.orn, R.vn);
		}
		if(ma->texco & TEXCO_REFL) {
			
			inp= 2.0*(R.vn[2]);
			R.ref[0]= (inp*R.vn[0]);
			R.ref[1]= (inp*R.vn[1]);
			R.ref[2]= (-1.0+inp*R.vn[2]);
		}

		if(ma->mode & MA_VERTEXCOLP) {
			R.mat->r= ma->r;
			R.mat->g= ma->g;
			R.mat->b= ma->b;
		}
		do_material_tex();
	}

	if(ma->mode & MA_SHLESS) {
		if(vertcol && (ma->mode & (MA_VERTEXCOL+MA_VERTEXCOLP))== MA_VERTEXCOL ) {
			col1[3]= vertcol[3]*ma->r;
			col1[2]= vertcol[2]*ma->g;
			col1[1]= vertcol[1]*ma->b;
		}
		else {
			col1[3]= (255.0*ma->r);
			col1[2]= (255.0*ma->g);
			col1[1]= (255.0*ma->b);
		}
		if(col2) {
			col2[3]= col1[3];
			col2[2]= col1[2];
			col2[1]= col1[1];
		}
		return;
	}

	if( vertcol && (ma->mode & (MA_VERTEXCOL+MA_VERTEXCOLP))== MA_VERTEXCOL ) {
		inpr= inpr1= ma->emit+vertcol[3]/255.0;
		inpg= inpg1= ma->emit+vertcol[2]/255.0;
		inpb= inpb1= ma->emit+vertcol[1]/255.0;
	}
	else {
		inpr= inpg= inpb= inpr1= inpg1= inpb1= ma->emit;
	}
	
	/* col[0]= (255.0*ma->r); */
	/* col[1]= (255.0*ma->g); */
	/* col[2]= (255.0*ma->b); */

	for(a=0; a<totflamp; a++) {
		fl= flar[a];

		/* if(fl->mode & LA_LAYER) if((fl->lay & ma->lay)==0) continue; */

		if(fl->type==LA_SUN || fl->type==LA_HEMI) {
			VECCOPY(lv, fl->vec);
			lampdist= 1.0;
		}
		else {
			lv[0]= fl->co[0] - co[0];
			lv[1]= fl->co[1] - co[1];
			lv[2]= fl->co[2] - co[2];
			ld= fsqrt(lv[0]*lv[0]+lv[1]*lv[1]+lv[2]*lv[2]);
			lv[0]/=ld;
			lv[1]/=ld;
			lv[2]/=ld;

			if(fl->mode & LA_QUAD) {
				t= 1.0;
				if(fl->att1>0.0)
					t= fl->dist/(fl->dist+fl->att1*ld);
				if(fl->att2>0.0)
					t*= fl->distkw/(fl->distkw+fl->att2*ld*ld);

				lampdist= t;
			}
			else {
				lampdist= (fl->dist/(fl->dist+ld));
			}

			if(fl->mode & LA_SPHERE) {
				t= fl->dist - ld;
				if(t<0.0) continue;
				
				t/= fl->dist;
				lampdist*= (t);
			}
		}

		if(fl->type==LA_SPOT) {
			inp= lv[0]*fl->vec[0]+lv[1]*fl->vec[1]+lv[2]*fl->vec[2];
			if(inp<fl->spotsi) continue;
			else {
				t= inp-fl->spotsi;
				i= 1.0;
				soft= 1.0;
				if(t<fl->spotbl && fl->spotbl!=0.0) {
					/* zachte gebied */
					i= t/fl->spotbl;
					t= i*i;
					soft= (3.0*t-2.0*t*i);
					inp*= soft;
				}

				lampdist*=inp;
			}
		}

		inp= nor[0]*lv[0]+ nor[1]*lv[1]+ nor[2]*lv[2];

		back= 0;
		if(inp<0.0) {
			back= 1;
			inp= -inp;
		}
		inp*= lampdist*ma->ref;

		if(back==0) {
			inpr+= inp*fl->r;
			inpg+= inp*fl->g;
			inpb+= inp*fl->b;
		} else if(col2) {
			inpr1+= inp*fl->r;
			inpg1+= inp*fl->g;
			inpb1+= inp*fl->b;
		}
		if(ma->spec) {
			
			lv[2]+= 1.0;
			Normalise(lv);
			t= nor[0]*lv[0]+nor[1]*lv[1]+nor[2]*lv[2];
			if(t>0) {
				t= ma->spec*lampdist*Spec(t, ma->har);
				if(back==0) {
					isr+= t*(fl->r * ma->specr);
					isg+= t*(fl->g * ma->specg);
					isb+= t*(fl->b * ma->specb);
				}
				else if(col2) {
					isr1+= t*(fl->r * ma->specr);
					isg1+= t*(fl->g * ma->specg);
					isb1+= t*(fl->b * ma->specb);
				}
			}
		}

	}

	a= 256*(inpr*ma->r + ma->ambr +isr);
	if(a>255) col1[3]= 255; 
	else col1[3]= a;
	a= 256*(inpg*ma->g + ma->ambg +isg);
	if(a>255) col1[2]= 255; 
	else col1[2]= a;
	a= 256*(inpb*ma->b + ma->ambb +isb);
	if(a>255) col1[1]= 255; 
	else col1[1]= a;

	if(col2) {
		a= 256*(inpr1*ma->r + ma->ambr +isr1);
		if(a>255) col2[3]= 255; 
		else col2[3]= a;
		a= 256*(inpr1*ma->g + ma->ambg +isg1);
		if(a>255) col2[2]= 255; 
		else col2[2]= a;
		a= 256*(inpr1*ma->b + ma->ambb +isb1);
		if(a>255) col2[1]= 255; 
		else col2[1]= a;
	}

}

void addnormalsDispList(Object *ob, ListBase *lb)
{
	DispList *dl;
	Mesh *me;
	MVert *ve1, *ve2, *ve3, *ve4;
	MFace *mface;
	float *vdata, *ndata, nor[3], nor1[3];
	float *v1, *v2, *v3, *v4;
	float *n1, *n2, *n3, *n4;
	int a, b, p1, p2, p3, p4;

	
	if ELEM3(ob->type, OB_MESH, OB_SECTOR, OB_LIFE) {
		
		me= get_mesh(ob);
		
		if(me->flag & ME_SMESH);
		else {
			if(me->totface==0) return;
			
			if(me->disp.first==0) {
				dl= callocN(sizeof(DispList), "meshdisp");
				dl->type= DL_NORS;
				dl->parts= 1;
				dl->nr= me->totface;
				addtail(&me->disp, dl);
			}
			
			if(dl->nors==0) {
				dl->nors= mallocN(sizeof(float)*3*me->totface, "meshnormals");
				n1= dl->nors;
				mface= me->mface;
				a= me->totface;
				while(a--) {
					if(mface->v3) {
						ve1= me->mvert+mface->v1;
						ve2= me->mvert+mface->v2;
						ve3= me->mvert+mface->v3;
						ve4= me->mvert+mface->v4;
						
						if(mface->v4) CalcNormFloat4(ve1->co, ve2->co, ve3->co, ve4->co, n1);
						else CalcNormFloat(ve1->co, ve2->co, ve3->co, n1);
					}
					n1+= 3;
					mface++;
				}
			}
			
			return;
		}
	}

	dl= lb->first;
	
	while(dl) {
		if(dl->type==DL_INDEX3) {
			if(dl->nors==0) {
				dl->nors= callocN(sizeof(float)*3, "dlnors");
				if(dl->verts[2]<0.0) dl->nors[2]= -1.0;
				else dl->nors[2]= 1.0;
			}
		}
		else if(dl->type==DL_SURF) {
			if(dl->nors==0) {
				dl->nors= callocN(sizeof(float)*3*dl->nr*dl->parts, "dlnors");
				
				vdata= dl->verts;
				ndata= dl->nors;
				
				for(a=0; a<dl->parts; a++) {
	
					DL_SURFINDEX(dl->flag & 1, dl->flag & 2, dl->nr, dl->parts);
	
					v1= vdata+ 3*p1; 
					n1= ndata+ 3*p1;
					v2= vdata+ 3*p2; 
					n2= ndata+ 3*p2;
					v3= vdata+ 3*p3; 
					n3= ndata+ 3*p3;
					v4= vdata+ 3*p4; 
					n4= ndata+ 3*p4;
					
					for(; b<dl->nr; b++) {
	
						CalcNormFloat4(v1, v3, v4, v2, nor);
	
						VecAddf(n1, n1, nor);
						VecAddf(n2, n2, nor);
						VecAddf(n3, n3, nor);
						VecAddf(n4, n4, nor);
	
						v2= v1; v1+= 3;
						v4= v3; v3+= 3;
						n2= n1; n1+= 3;
						n4= n3; n3+= 3;
					}
				}
				a= dl->parts*dl->nr;
				v1= ndata;
				while(a--) {
					Normalise(v1);
					v1+= 3;
				}
			}
		}
		dl= dl->next;
	}
}


void shadeDispList(Object *ob)
{
	MFace *mface;
	MVert *mvert;
	DispList *dl, *dlob, *dldeform;
	Material *ma;
	Mesh *me;
	Curve *cu;
	MetaBall *mb;
	extern Material defmaterial;	/* initrender.c */
	float *orco, imat[3][3], tmat[4][4], mat[4][4], vec[3], xn, yn, zn;
	float inp, *fp, *nor, n1[3];
	uint *col1, *col2, *vertcol;
	int *index, a, b, lastmat= -1, need_orco;

	if(ob->flag & OB_FROMDUPLI) return;
	initfastshade();

	Mat4MulMat4(mat, ob->obmat, fviewmat);
	
	Mat4Invert(tmat, mat);
	Mat3CpyMat4(imat, tmat);
	
	/* we halen de dl_verts eruit, deform info */
	dldeform= find_displist(&ob->disp, DL_VERTS);
	if(dldeform) remlink(&ob->disp, dldeform);
	
	/* Metaballs hebben de standaard displist aan het Object zitten */
	if(ob->type!=OB_MBALL) freedisplist(&ob->disp);

	if((R.flag & R_RENDERING)==0) {
		need_orco= 0;
		for(a=0; a<ob->totcol; a++) {
			ma= give_current_material(ob, a+1);
			if(ma) {
				init_render_material(ma);
				if(ma->ren->texco & TEXCO_ORCO) need_orco= 1;
			}
		}
	}
	if(ob->type==OB_MESH) {
		
		me= ob->data;
		
		if(me->flag & ME_SMESH) {
			
			dl= me->disp.first;
			while(dl) {

				dlob= callocN(sizeof(DispList), "displistshade");
				addtail(&ob->disp, dlob);
				dlob->type= DL_VERTCOL;
				dlob->parts= dl->parts;
				dlob->nr= dl->nr;
				
				col1= dlob->col1= mallocN(sizeof(int)*dl->parts*dl->nr, "col1");
			
				ma= give_current_material(ob, dl->col+1);
				if(ma==0) ma= &defmaterial;

				if(dl->type==DL_SURF) {
					if(dl->nors) {
						a= dl->nr*dl->parts;
						fp= dl->verts;
						nor= dl->nors;
						
						while(a--) {
							VECCOPY(vec, fp);
							Mat4MulVecfl(mat, vec);
							
							n1[0]= imat[0][0]*nor[0]+imat[0][1]*nor[1]+imat[0][2]*nor[2];
							n1[1]= imat[1][0]*nor[0]+imat[1][1]*nor[1]+imat[1][2]*nor[2];
							n1[2]= imat[2][0]*nor[0]+imat[2][1]*nor[1]+imat[2][2]*nor[2];
							Normalise(n1);
				
							fastshade(vec, n1, fp, ma, (char *)col1, 0, 0);
							
							fp+= 3; nor+= 3; col1++;
						}
					}
				}
				dl= dl->next;
			}
		}
		else if(me->totvert>0) {
		
			if(me->orco==0 && need_orco) {
				make_orco_mesh(me);
			}
			orco= me->orco;
			/* ms= me->msticky; */
			
			dl= me->disp.first;
			if(dl==0 || dl->nors==0) addnormalsDispList(ob, &me->disp);
			dl= me->disp.first;
			if(dl==0 || dl->nors==0) return;
			nor= dl->nors;
			
			dl= callocN(sizeof(DispList), "displistshade");
			addtail(&ob->disp, dl);
			dl->type= DL_VERTCOL;
			col1= dl->col1= mallocN(4*sizeof(int)*me->totface, "col1");
			col2= 0;
			if(me->tface) tface_to_mcol(me);
			vertcol= (uint *)me->mcol;
			
			if( me->flag & ME_TWOSIDED) {
				col2= dl->col2= mallocN(4*sizeof(int)*me->totface, "col2");
			}
			
			/* even geen puno's */
			mvert= me->mvert;
			a= me->totvert;
			while(FALSE || a--) {
				
				VECCOPY(vec, mvert->co);
				Mat4MulVecfl(mat, vec);
				
				xn= mvert->no[0]; 
				yn= mvert->no[1]; 
				zn= mvert->no[2];
				
				/* transpose ! */
				n1[0]= imat[0][0]*xn+imat[0][1]*yn+imat[0][2]*zn;
				n1[1]= imat[1][0]*xn+imat[1][1]*yn+imat[1][2]*zn;
				n1[2]= imat[2][0]*xn+imat[2][1]*yn+imat[2][2]*zn;
				Normalise(n1);
				
				mvert++;
			}		
			
			mface= me->mface;
			a= me->totface;
			while(a--) {
				
				if(mface->v3) {
				
					/* transpose ! */
					n1[0]= imat[0][0]*nor[0]+imat[0][1]*nor[1]+imat[0][2]*nor[2];
					n1[1]= imat[1][0]*nor[0]+imat[1][1]*nor[1]+imat[1][2]*nor[2];
					n1[2]= imat[2][0]*nor[0]+imat[2][1]*nor[1]+imat[2][2]*nor[2];
					Normalise(n1);
					
					if(lastmat!=mface->mat_nr) {
						ma= give_current_material(ob, mface->mat_nr+1);
						if(ma==0) ma= &defmaterial;
						lastmat= mface->mat_nr;
					}
					
					mvert= me->mvert+mface->v1;
					VECCOPY(vec, mvert->co);
					Mat4MulVecfl(mat, vec);
					
					if(orco)  fastshade(vec, n1, orco+3*mface->v1, ma, (char *)col1, (char *)col2, (char *)vertcol);
					else fastshade(vec, n1, mvert->co, ma, (char *)col1, (char *)col2, (char *)vertcol);
					col1++;
					if(vertcol) vertcol++; 
					if(col2) col2++;
					
					mvert= me->mvert+mface->v2;
					VECCOPY(vec, mvert->co);
					Mat4MulVecfl(mat, vec);
					
					if(orco)  fastshade(vec, n1, orco+3*mface->v2, ma, (char *)col1, (char *)col2, (char *)vertcol);
					else fastshade(vec, n1, mvert->co, ma, (char *)col1, (char *)col2, (char *)vertcol);
					col1++;
					if(vertcol) vertcol++; 
					if(col2) col2++;
					
					mvert= me->mvert+mface->v3;
					VECCOPY(vec, mvert->co);
					Mat4MulVecfl(mat, vec);
					
					if(orco)  fastshade(vec, n1, orco+3*mface->v3, ma, (char *)col1, (char *)col2, (char *)vertcol);
					else fastshade(vec, n1, mvert->co, ma, (char *)col1, (char *)col2, (char *)vertcol);
					col1++;
					if(vertcol) vertcol++; 
					if(col2) col2++;
					
					if(mface->v4) {
						mvert= me->mvert+mface->v4;
						VECCOPY(vec, mvert->co);
						Mat4MulVecfl(mat, vec);
						
						if(orco)  fastshade(vec, n1, orco+3*mface->v4, ma, (char *)col1, (char *)col2, (char *)vertcol);
						else fastshade(vec, n1, mvert->co, ma, (char *)col1, (char *)col2, (char *)vertcol);
					}
					col1++;
					if(vertcol) vertcol++; 
					if(col2) col2++;
						
				}
				else {
					col1+=4;
					if(vertcol) vertcol+=4; 
					if(col2) col2+=4;
				}
	
				nor+= 3;
				mface++;
			}
			
			if(me->orco) {
				freeN(me->orco);
				me->orco= 0;
			}
			if(me->tface) {
				freeN(me->mcol);
				me->mcol= 0;
			}
		}
	}
	else if ELEM3(ob->type, OB_CURVE, OB_SURF, OB_FONT) {
	
		/* nu hebben we wel de normalen nodig */
		cu= ob->data;
		dl= cu->disp.first;
		
		while(dl) {
			dlob= callocN(sizeof(DispList), "displistshade");
			addtail(&ob->disp, dlob);
			dlob->type= DL_VERTCOL;
			dlob->parts= dl->parts;
			dlob->nr= dl->nr;
			
			if(dl->type==DL_INDEX3) {
				col1= dlob->col1= mallocN(sizeof(int)*dl->nr, "col1");
			}
			else {
				col1= dlob->col1= mallocN(sizeof(int)*dl->parts*dl->nr, "col1");
			}
			
		
			ma= give_current_material(ob, dl->col+1);
			if(ma==0) ma= &defmaterial;

			if(dl->type==DL_INDEX3) {
				if(dl->nors) {
					/* er is maar 1 normaal */
					n1[0]= imat[0][0]*dl->nors[0]+imat[0][1]*dl->nors[1]+imat[0][2]*dl->nors[2];
					n1[1]= imat[1][0]*dl->nors[0]+imat[1][1]*dl->nors[1]+imat[1][2]*dl->nors[2];
					n1[2]= imat[2][0]*dl->nors[0]+imat[2][1]*dl->nors[1]+imat[2][2]*dl->nors[2];
					Normalise(n1);
					
					fp= dl->verts;
					
					a= dl->nr;		
					while(a--) {
						VECCOPY(vec, fp);
						Mat4MulVecfl(mat, vec);
						
						fastshade(vec, n1, fp, ma, (char *)col1, 0, 0);
						
						fp+= 3; col1++;
					}
				}
			}
			else if(dl->type==DL_SURF) {
				if(dl->nors) {
					a= dl->nr*dl->parts;
					fp= dl->verts;
					nor= dl->nors;
					
					while(a--) {
						VECCOPY(vec, fp);
						Mat4MulVecfl(mat, vec);
						
						n1[0]= imat[0][0]*nor[0]+imat[0][1]*nor[1]+imat[0][2]*nor[2];
						n1[1]= imat[1][0]*nor[0]+imat[1][1]*nor[1]+imat[1][2]*nor[2];
						n1[2]= imat[2][0]*nor[0]+imat[2][1]*nor[1]+imat[2][2]*nor[2];
						Normalise(n1);
			
						fastshade(vec, n1, fp, ma, (char *)col1, 0, 0);
						
						fp+= 3; nor+= 3; col1++;
					}
				}
			}
			dl= dl->next;
		}
	}
	else if(ob->type==OB_MBALL) {
		/* normalen zijn er al */
		mb= ob->data;
		dl= ob->disp.first;
		
		while(dl) {
			
			if(dl->type==DL_INDEX4) {
				if(dl->nors) {
					
					if(dl->col1) freeN(dl->col1);
					col1= dl->col1= mallocN(sizeof(int)*dl->nr, "col1");
			
					ma= give_current_material(ob, dl->col+1);
					if(ma==0) ma= &defmaterial;
	
					fp= dl->verts;
					nor= dl->nors;
					
					a= dl->nr;		
					while(a--) {
						VECCOPY(vec, fp);
						Mat4MulVecfl(mat, vec);
						
						/* transpose ! */
						n1[0]= imat[0][0]*nor[0]+imat[0][1]*nor[1]+imat[0][2]*nor[2];
						n1[1]= imat[1][0]*nor[0]+imat[1][1]*nor[1]+imat[1][2]*nor[2];
						n1[2]= imat[2][0]*nor[0]+imat[2][1]*nor[1]+imat[2][2]*nor[2];
						Normalise(n1);
					
						fastshade(vec, n1, fp, ma, (char *)col1, 0, 0);
						
						fp+= 3; col1++; nor+= 3;
					}
				}
			}
			dl= dl->next;
		}
	}
	
	if((R.flag & R_RENDERING)==0) {
		for(a=0; a<ob->totcol; a++) {
			ma= give_current_material(ob, a+1);
			if(ma) end_render_material(ma);
		}
	}

	/* deze was er tijdelijk uitgehaald */
	if(dldeform) addtail(&ob->disp, dldeform);
}

void reshadeall_displist()
{
	DispList *dldeform;
	Base *base;
	Object *ob;
	
	freefastshade();
	
	base= FIRSTBASE;
	while(base) {
		if(base->lay & G.vd->lay) {
			
			ob= base->object;
			
			/* we halen de dl_verts eruit, deform info */
			dldeform= find_displist(&ob->disp, DL_VERTS);
			if(dldeform) remlink(&ob->disp, dldeform);
			
			/* Metaballs hebben de standaard displist aan het Object zitten */
			if(ob->type==OB_MBALL) shadeDispList(ob);
			else freedisplist(&ob->disp);
			
			if(dldeform) addtail(&ob->disp, dldeform);
		}
		base= base->next;
	}
}

void count_displist(ListBase *lb, int *totvert, int *totface)
{
	DispList *dl;
	
	dl= lb->first;
	while(dl) {
		
		switch(dl->type) {
		case DL_SURF:
			*totvert+= dl->nr*dl->parts;
			*totface+= (dl->nr-1)*(dl->parts-1);
			break;
		case DL_INDEX3:
		case DL_INDEX4:
			*totvert+= dl->nr;
			*totface+= dl->parts;
			break;
		case DL_POLY:
		case DL_SEGM:
			*totvert+= dl->nr*dl->parts;
		}
		
		dl= dl->next;
	}
}

void curve_to_displist(ListBase *nubase, ListBase *dispbase)
{
	Nurb *nu;
	DispList *dl;
	BezTriple *bezt, *prevbezt;
	BPoint *bp;
	float *data, *v1, *v2;
	int a, len;
	
	nu= nubase->first;
	
	while(nu) {
		if(nu->hide==0) {
			if((nu->type & 7)==CU_BEZIER) {
				
				/* tellen */
				len= 0;
				a= nu->pntsu-1;
				if(nu->flagu & 1) a++;

				prevbezt= nu->bezt;
				bezt= prevbezt+1;
				while(a--) {
					if(a==0 && (nu->flagu & 1)) bezt= nu->bezt;
					
					if(prevbezt->h2==HD_VECT && bezt->h1==HD_VECT) len++;
					else len+= nu->resolu;
					
					if(a==0 && (nu->flagu & 1)==0) len++;
					
					prevbezt= bezt;
					bezt++;
				}
				
				dl= callocN(sizeof(DispList), "makeDispListbez");
				/* len+1 i.v.m. maakbez */
				dl->verts= callocN( (len+1)*3*sizeof(float), "dlverts");
				addtail(dispbase, dl);
				dl->parts= 1;
				dl->nr= len;
				dl->col= nu->mat_nr;

				data= dl->verts;

				if(nu->flagu & 1) {
					dl->type= DL_POLY;
					a= nu->pntsu;
				}
				else {
					dl->type= DL_SEGM;
					a= nu->pntsu-1;
				}
				
				prevbezt= nu->bezt;
				bezt= prevbezt+1;
				
				while(a--) {
					if(a==0 && dl->type== DL_POLY) bezt= nu->bezt;
					
					if(prevbezt->h2==HD_VECT && bezt->h1==HD_VECT) {
						VECCOPY(data, prevbezt->vec[1]);
						data+= 3;
					}
					else {
						v1= prevbezt->vec[1];
						v2= bezt->vec[0];
						maakbez(v1[0], v1[3], v2[0], v2[3], data, nu->resolu);
						maakbez(v1[1], v1[4], v2[1], v2[4], data+1, nu->resolu);
						if((nu->type & 8)==0)
							maakbez(v1[2], v1[5], v2[2], v2[5], data+2, nu->resolu);
						data+= 3*nu->resolu;
					}
					
					if(a==0 && dl->type==DL_SEGM) {
						VECCOPY(data, bezt->vec[1]);
					}
					
					prevbezt= bezt;
					bezt++;
				}
			}
			else if((nu->type & 7)==CU_NURBS) {
				len= nu->pntsu*nu->resolu;
				dl= callocN(sizeof(DispList), "makeDispListsurf");
				dl->verts= callocN(len*3*sizeof(float), "dlverts");
				addtail(dispbase, dl);
				dl->parts= 1;
				dl->nr= len;
				dl->col= nu->mat_nr;

				data= dl->verts;
				if(nu->flagu & 1) dl->type= DL_POLY;
				else dl->type= DL_SEGM;
				makeNurbcurve(nu, data, 3);
			}
			else if((nu->type & 7)==CU_POLY) {
				len= nu->pntsu;
				dl= callocN(sizeof(DispList), "makeDispListpoly");
				dl->verts= callocN(len*3*sizeof(float), "dlverts");
				addtail(dispbase, dl);
				dl->parts= 1;
				dl->nr= len;
				dl->col= nu->mat_nr;

				data= dl->verts;
				if(nu->flagu & 1) dl->type= DL_POLY;
				else dl->type= DL_SEGM;
				
				a= len;
				bp= nu->bp;
				while(a--) {
					VECCOPY(data, bp->vec);
					bp++;
					data+= 3;
				}
			}
		}
		nu= nu->next;
	}
}



void bevels_to_filledpoly(Curve *cu, ListBase *dispbase)
{
	ListBase front, back;
	DispList *dl, *dlnew;
	float *fp, *fp1;
	int a, dpoly;
	
	front.first= front.last= back.first= back.last= 0;
	
	if(cu->flag & CU_3D) return;
	if( (cu->flag & (CU_FRONT+CU_BACK))==0 ) return;
	
	dl= dispbase->first;
	while(dl) {
		if(dl->type==DL_SURF) {
			if(dl->flag==2) {
				if(cu->flag & CU_BACK) {
					dlnew= callocN(sizeof(DispList), "filldisp");
					addtail(&front, dlnew);
					dlnew->verts= fp1= mallocN(sizeof(float)*3*dl->parts, "filldisp1");
					dlnew->nr= dl->parts;
					dlnew->parts= 1;
					dlnew->type= DL_POLY;
					dlnew->col= dl->col;
					
					fp= dl->verts;
					dpoly= 3*dl->nr;
					
					a= dl->parts;
					while(a--) {
						VECCOPY(fp1, fp);
						fp1+= 3;
						fp+= dpoly;
					}
				}
				if(cu->flag & CU_FRONT) {
					dlnew= callocN(sizeof(DispList), "filldisp");
					addtail(&back, dlnew);
					dlnew->verts= fp1= mallocN(sizeof(float)*3*dl->parts, "filldisp1");
					dlnew->nr= dl->parts;
					dlnew->parts= 1;
					dlnew->type= DL_POLY;
					dlnew->col= dl->col;
					
					fp= dl->verts+3*(dl->nr-1);
					dpoly= 3*dl->nr;
					
					a= dl->parts;
					while(a--) {
						VECCOPY(fp1, fp);
						fp1+= 3;
						fp+= dpoly;
					}
				}
			}
		}
		dl= dl->next;
	}

	filldisplist(&front, dispbase);
	filldisplist(&back, dispbase);
	
	freedisplist(&front);
	freedisplist(&back);

	filldisplist(dispbase, dispbase);
	
}

void curve_to_filledpoly(Curve *cu, ListBase *dispbase)
{
	DispList *dl;
	Nurb *nu;
		
	dl= dispbase->first;
	
	if(cu->flag & CU_3D) return;
	
	nu= cu->nurb.first;
	while(nu) {
		if(nu->flagu & CU_CYCLIC) break;
		nu= nu->next;
	}
	if(nu==0) return;

	if(dl->type==DL_SURF) bevels_to_filledpoly(cu, dispbase);
	else {
		if(cu->flag & CU_FRONT) filldisplist(dispbase, dispbase);
	}
}


int dl_onlyzero= 0;

void set_displist_onlyzero(int val)
{
	dl_onlyzero= val;
}

void makeDispList(Object *ob)
{
	Mesh *me;
	MFace *mface;
	MVert *mvert, *a1, *a2, *a3;
	Nurb *nu;
	Curve *cu;
	BezTriple *bezt, *prevbezt;
	BPoint *bp;
	ListBase dlbev, *dispbase;
	DispList *dl, *dlnew, *dlb, *dlpoly, *dle, *dls, *dlt;
	BevList *bl;
	BevPoint *bevp;
	float *data, *data1, *fp1, *v1, *v2, *v3, widfac, vec[3], vec1[3];
	int test, len, a, b, c, draw=0, edges, segs, trias;

	if(ob==0) return;
	if(ob->flag & OB_FROMDUPLI) return;
	freedisplist(&(ob->disp));

	if(ob->type==OB_MESH) {
		me= ob->data;

		freedisplist(&(me->disp));

		
		tex_space_mesh(ob->data);

		object_deform(ob);	

		if(ob->effect.first) object_wave(ob);
		
		if(TEST_C_KEY) {
			if(ob==G.obedit) {
				if(me->flag & ME_SMESH) make_s_editmesh(ob);
				return;
			}
			else if(me->flag & ME_SMESH) {
				make_s_mesh(ob);
			}
		}
	}
	else if(ob->type==OB_MBALL) {

		if( has_id_number((ID *)ob)==0 ) {
			
			metaball_polygonize(ob);
			tex_space_mball(ob);
		}
		else if(ob==G.obedit) {
			ob= find_basis_mball(ob);
			if(ob==0) return;

			metaball_polygonize(ob);
			tex_space_mball(ob);
		}
	}
	else if(ob->type==OB_SURF) {
		
		draw= ob->dt;
		cu= ob->data;
		dispbase= &(cu->disp);
		if(dl_onlyzero && dispbase->first) return;
		freedisplist(dispbase);
		
		if(ob==G.obedit) nu= editNurb.first;
		else nu= cu->nurb.first;

		while(nu) {
			if(nu->hide==0) {
				if(nu->pntsv==1) {
					if(draw==0) len= nu->pntsu;
					else len= nu->pntsu*nu->resolu;
					
					dl= callocN(sizeof(DispList), "makeDispListsurf");
					dl->verts= callocN(len*3*sizeof(float), "dlverts");
					
					addtail(dispbase, dl);
					dl->parts= 1;
					dl->nr= len;
					dl->col= nu->mat_nr;

					data= dl->verts;
					if(nu->flagu & 1) dl->type= DL_POLY;
					else dl->type= DL_SEGM;
					
					if(draw==0) {
						bp= nu->bp;
						while(len--) {
							VECCOPY(data, bp->vec);
							bp++;
							data+= 3;
						}
					}
					else makeNurbcurve(nu, data, 3);
				}
				else {
					if(draw==0 && ob==G.obedit) ;
					else {
						if(draw==0) len= nu->pntsu*nu->pntsv;
						else len= nu->resolu*nu->resolv;
						
						dl= callocN(sizeof(DispList), "makeDispListsurf");
						dl->verts= callocN(len*3*sizeof(float), "dlverts");
						addtail(dispbase, dl);
	
						if(draw==0) {
							dl->parts= nu->pntsv;
							dl->nr= nu->pntsu;
							if(nu->flagu & 1) dl->flag|= 1;
							if(nu->flagv & 1) dl->flag|= 2;
						}
						else {
							dl->parts= nu->resolu;	/* andersom want makeNurbfaces gaat zo */
							dl->nr= nu->resolv;
							if(nu->flagv & 1) dl->flag|= 1;	/* ook andersom ! */
							if(nu->flagu & 1) dl->flag|= 2;
						}
						dl->col= nu->mat_nr;
	
						data= dl->verts;
						dl->type= DL_SURF;
						
						if(draw==0) {
							bp= nu->bp;
							while(len--) {
								VECCOPY(data, bp->vec);
								bp++;
								data+= 3;
							}
						}
						else makeNurbfaces(nu, data);
					}
				}
			}
			nu= nu->next;
		}
		
		tex_space_curve(cu);
		
		if(ob!=G.obedit) object_deform(ob);
	}
	else if ELEM(ob->type, OB_CURVE, OB_FONT) {
		
		draw= ob->dt;
		cu= ob->data;
		dispbase= &(cu->disp);
		if(dl_onlyzero && dispbase->first) return;
		freedisplist(dispbase);
		
		if(cu->path) free_path(cu->path);
		cu->path= 0;
		
		freelistN(&(cu->bev));
		
		if(ob==G.obedit) {
			if(ob->type==OB_CURVE) curve_to_displist(&editNurb, dispbase);
			else curve_to_displist(&cu->nurb, dispbase);
			if(cu->flag & CU_PATH) makeBevelList(ob);
		}
		else if(cu->ext1==0.0 && cu->ext2==0.0 && cu->bevobj==0 && cu->width==1.0) {
			curve_to_displist(&cu->nurb, dispbase);
			if(cu->flag & CU_PATH) makeBevelList(ob);
		}
		else {
			
			makeBevelList(ob);

			dlbev.first= dlbev.last= 0;
			if(cu->ext1!=0.0 || cu->ext2!=0.0 || cu->bevobj) {
				if(ob->dt!=0) makebevelcurve(ob, &dlbev);
			}

			/* met bevellist werken */
			widfac= cu->width-1.0;
			bl= cu->bev.first;
			nu= cu->nurb.first;
			while(bl) {
				if(dlbev.first==0) {
					dl= callocN(sizeof(DispList), "makeDispListbev");
					dl->verts= callocN(3*sizeof(float)*bl->nr, "dlverts");
					addtail(dispbase, dl);
					if(bl->poly!= -1) dl->type= DL_POLY;
					else dl->type= DL_SEGM;
					dl->parts= 1;
					dl->nr= bl->nr;
					dl->col= nu->mat_nr;

					a= dl->nr;
					bevp= (BevPoint *)(bl+1);
					data= dl->verts;
					while(a--) {
						data[0]= bevp->x+widfac*bevp->sina;
						data[1]= bevp->y+widfac*bevp->cosa;
						data[2]= bevp->z;
						bevp++;
						data+=3;
					}
				}
				else {
					data1= data;
					/* voor iedere stuk van de bevel een aparte dispblok maken */
					dlb= dlbev.first;
					while(dlb) {
						dl= callocN(sizeof(DispList), "makeDispListbev1");
						dl->verts= callocN(3*sizeof(float)*dlb->nr*bl->nr, "dlverts");
						addtail(dispbase, dl);
						/* dl->type= dlb->type; */

						dl->type= DL_SURF;
						dl->flag= 0;
						if(dlb->type==DL_POLY) dl->flag++;
						if(bl->poly>=0) dl->flag+=2;

						dl->parts= bl->nr;
						dl->nr= dlb->nr;
						dl->col= nu->mat_nr;

						data= dl->verts;
						bevp= (BevPoint *)(bl+1);
						a= bl->nr;
						while(a--) {	/* voor ieder punt van poly een bevelstuk maken */

							/* roteer bevelstuk en schrijf in data */
							fp1= dlb->verts;
							b= dlb->nr;

							while(b--) {
								if(cu->flag & CU_3D) {
								
									vec[0]= fp1[1]+widfac;
									vec[1]= fp1[2];
									vec[2]= 0.0;
									
									Mat3MulVecfl(bevp->mat, vec);
									
									data[0]= bevp->x+ vec[0];
									data[1]= bevp->y+ vec[1];
									data[2]= bevp->z+ vec[2];
								}
								else {
									data[0]= bevp->x+ (fp1[1]+widfac)*bevp->sina;
									data[1]= bevp->y+ (fp1[1]+widfac)*bevp->cosa;
									data[2]= bevp->z+ fp1[2];
								}

								data+=3;
								fp1+=3;
							}

							bevp++;
						}

						dlb= dlb->next;
					}
				}

				bl= bl->next;
				nu= nu->next;
			}

			if(cu->ext1!=0.0 || cu->ext2!=0.0 || cu->bevobj) {
				freedisplist(&dlbev);
			}
		}

		if(ob!=G.obedit) object_deform(ob);

		tex_space_curve(cu);

	}
}


void test_users_of(void *data, Object *exept)
{
	Base *base;
	
	base= FIRSTBASE;
	while(base) {
		if(base->object->data == data) {
			if(base->object != exept ) {
				if(TRUE);
				else makeDispList(base->object);
			}
		}
		base= base->next;
	}
}

void filldisplist(ListBase *dispbase, ListBase *to)
{
	EditVert *eve,*v1,*vstart,*vlast;
	EditEdge *eed,*e1;
	EditVlak *evl,*nextvl;
	DispList *dlnew=0, *dl;
	ListBase tempbase;
	float *f1;
	int colnr=0, cont=1, tijd, tot, a, b, *index;
	long totvert;
	
	if(dispbase==0) return;
	if(dispbase->first==0) return;

	/* tijd= clock(); */
	if(G.f & G_PLAYANIM == 0) waitcursor(1);

	while(cont) {
		cont= 0;
		totvert=0;
		
		dl= dispbase->first;
		while(dl) {
	
			if(dl->type==DL_POLY) {
				if(colnr<dl->col) cont= 1;
				else if(colnr==dl->col) {
			
					colnr= dl->col;
		
					/* editverts en edges maken */
					f1= dl->verts;
					a= dl->nr;
					eve= v1= 0;
					
					while(a--) {
						vlast= eve;
						
						eve= addfillvert(f1);
						totvert++;
						
						if(vlast==0) v1= eve;
						else {
							eed= addfilledge(vlast, eve);
						}
						f1+=3;
					}
				
					if(eve!=0 && v1!=0) {
						eed= addfilledge(eve, v1);
					}
				}
			}
			dl= dl->next;
		}

		if(totvert && edgefill(0)!=0) {

			/* vlakken tellen */
			tot= 0;
			evl= fillvlakbase.first;
			while(evl) {
				tot++;
				evl= evl->next;
			}

			if(tot) {
				dlnew= callocN(sizeof(DispList), "filldisplist");
				dlnew->type= DL_INDEX3;
				dlnew->col= colnr;
				dlnew->nr= totvert;
				dlnew->parts= tot;

				dlnew->index= mallocN(tot*3*sizeof(int), "dlindex");
				dlnew->verts= mallocN(totvert*3*sizeof(float), "dlverts");
				
				/* vertdata */
				f1= dlnew->verts;
				totvert= 0;
				eve= fillvertbase.first;
				while(eve) {
					VECCOPY(f1, eve->co);
					f1+= 3;
	
					/* indexnummer */
					eve->vn= (EditVert *)totvert;
					totvert++;
					
					eve= eve->next;
				}
				
				/* indexdata */
				evl= fillvlakbase.first;
				index= dlnew->index;
				while(evl) {
					index[0]= (long)evl->v1->vn;
					index[1]= (long)evl->v2->vn;
					index[2]= (long)evl->v3->vn;
					
					index+= 3;
					evl= evl->next;
				}
			}

			addhead(to, dlnew);
			
		}
		end_edgefill();

		colnr++;
	}
	
	/* poly's niet vrijgeven. nodig voor wireframe display */
	
	if(G.f & G_PLAYANIM == 0) waitcursor(0);
	/* printf("time: %d\n",(clock()-tijd)/1000); */

}

/*******************************/
/*****       OUTLINE       *****/
/*******************************/

typedef struct Sample{
	short x, y;
} Sample;

typedef struct Segment{
	/* coordinaten */
	struct Segment * next, * prev;
	float co[2];
} Segment;


int dflt_in_out(struct ImBuf * ibuf, int x, int y)
{
	uchar * rect;
	
	if (ibuf == 0) return (0);
	if (x < 0 || y < 0 || x >= ibuf->x || y >= ibuf->y || ibuf->rect == 0) return (-1);
	
	rect = (uchar *) (ibuf->rect + (y * ibuf->x) + x);
	if (rect[0] > 0x81) return (1);
	return(0);
}


Sample * outline(struct ImBuf * ibuf, int (*in_or_out)())
{
	static int dirs[8][2] = {
		-1,  0,		-1,  1,		0,  1,		 1,  1, 
		 1,  0,		 1, -1,		0, -1,		-1, -1, 
	};
	
	int dir, x, y, in, i;
	int count, sampcount;
	int startx = 0, starty = 0;
	uchar * rect;
	Sample * samp, * oldsamp;
	
	/* wat erin gaat:
	 * 1 - plaatje waarvan outline berekent moet worden, 
	 * 2 - pointer naar functie die bepaalt welke pixel in of uit is
	 */
	
	if (ibuf == 0) return (0);
	if (ibuf->rect == 0) return (0);
	
	if (in_or_out == 0) in_or_out = dflt_in_out;
	in = in_or_out(ibuf, 0, 0);
	
	/* zoek naar eerste overgang en ga van daar uit 'zoeken' */	
	for (y = 0; y < ibuf->y; y++) {
		for (x = 0; x < ibuf->x; x++) {
			if (in_or_out(ibuf, x, y) != in) {
				/* eerste 'andere' punt gevonden !! */
				
				if (x != startx) dir = 0;
				else dir = 6;
				
				startx = x; starty = y;
				count = 1;
				sampcount = 2000;
				samp = mallocN(sampcount * sizeof(Sample), "wire_samples");
				
				do{
					samp[count].x = x; samp[count].y = y;
					count++;
					
					if (count >= sampcount) {
						oldsamp = samp;
						samp = mallocN(2 * sampcount * sizeof(Sample), "wire_samples");
						memcpy(samp, oldsamp, sampcount * sizeof(Sample));
						sampcount *= 2;
						freeN(oldsamp);
					}
					
					i = 0;
					while(in_or_out(ibuf, x + dirs[dir][0], y + dirs[dir][1]) == in) {
						dir = (dir + 1) & 0x7;
						if (i++ == 9) break;
					}
					
					if (i >= 8) {
						/* dit moet een losse punt geweest zijn */
						break;
					}
					
					x += dirs[dir][0];
					y += dirs[dir][1];
					dir = (dir - 3) & 0x7;
				} while(x != startx || y != starty);
				
				if (i >= 8) {
					/* losse punten patch */
					freeN(samp);
				} else {
					count = count - 1;
					samp[0].x = count >> 16;
					samp[0].y = count;
					return(samp);
				}
			}
		}
	}
	/* printf("geen overgang \n"); */
	return(0);
}



/*******************************/
/*****      WIREFRAME      *****/
/*******************************/


float DistToLine2D(v1,v2,v3)   /* met formule van Hesse :GEEN LIJNSTUK! */
short *v1,*v2,*v3;
{
	float a[2],deler;

	a[0] = v2[1]-v3[1];
	a[1] = v3[0]-v2[0];
	deler = fsqrt(a[0]*a[0]+a[1]*a[1]);
	if(deler == 0.0) return 0;

	return fabsf((v1[0]-v2[0])*a[0]+(v1[1]-v2[1])*a[1])/deler;

}

float ComputeMaxShpError(samp, first, last, splitPoint)
    Sample	*samp;			/*  Array of digitized points	*/
    int		first, last;	/*  Indices defining region	*/
    int		*splitPoint;	/*  Point of maximum error	*/
{
    int		i;
    float	maxDist;				/*  Maximum error		*/
    float	dist;					/*  Current error		*/
 
    *splitPoint = (last - first + 1) / 2;
    maxDist = 0.0;
	
    for (i = first + 1; i < last; i++) {				
		dist = DistToLine2D(samp+i, samp+first, samp+last);

		if (dist >= maxDist) {
	    	maxDist = dist;
	    	*splitPoint = i;
		}
    }

    return (maxDist);
}


void FitPoly(samp, first, last, shperr, seglist)
    Sample	*samp;				/*  Array of digitized points */
    int		first, last;		/* Indices of first and last pts in region */
    float	shperr;				/*  User-defined error squared	   */
	ListBase * seglist;
{
    Segment	* seg;				/* Control points segment*/
    float	maxError;			/*  Maximum fitting error	 */
	float	x, y;
    int		splitPoint;			/*  Point to split point set at	 */
    int		nPts;				/*  Number of points in subset  */
    int		i;
	
    nPts = last - first + 1;

    /*  Use heuristic if region only has two points in it */

	seg = mallocN(sizeof(Segment), "wure_segment");

	seg->co[0] = samp[first].x;
	seg->co[1] = samp[first].y;
	
    if (nPts == 2) {
		addtail(seglist, seg);
		return;
    }

	maxError = ComputeMaxShpError(samp, first, last, &splitPoint);
	if (maxError < shperr) {
		addtail(seglist, seg);
		return;
	}
 	
    /* Fitting failed -- split at max error point and fit recursively */
	
    FitPoly(samp, first, splitPoint, shperr, seglist);
    FitPoly(samp, splitPoint, last, shperr, seglist);
	
	freeN(seg);
}


void ibuf2wire(ListBase * wireframe, struct ImBuf * ibuf)
{
	int count;
	Sample * samp;
	
	/* eerst een lijst met samples maken */
	
	samp = outline(ibuf, 0);
	if (samp == 0) return;
	
	count = (samp[0].x << 16) + samp[0].y;
	if (count) FitPoly(samp, 1, count, 1.0, wireframe); /* was 3.0. Frank */

	freeN(samp);
}



void imagestodisplist()
{
	Base *base;
	Object *ob;
	Material *ma;
	Tex *tex;
	Mesh *me;
	ListBase _wireframe, *wireframe;
	DispList *dl;
	Segment *seg;
	float *data, xfac, yfac, xsi, ysi, vec[3];
	int tot;
	
	_wireframe.first= 0;
	_wireframe.last= 0;
	wireframe = &_wireframe;
	
	init_render_textures();
	
	base= FIRSTBASE;
	while(base) {
		if(TESTBASE(base)) {
			if( base->object->type==OB_MESH) {
				ob= base->object;
				me= ob->data;
				
				ma= give_current_material(ob, 1);
	
				if(ma && ma->mtex[0] && ma->mtex[0]->tex) {
					tex= ma->mtex[0]->tex;
					
					/* dit zorgt voor correct laden van nieuwe imbufs */
					externtex(ma->mtex[0], vec);
					
					if(tex->type==TEX_IMAGE && tex->ima && tex->ima->ibuf) {				
						
						ob->dtx |= OB_DRAWIMAGE;
						
						ibuf2wire(wireframe, tex->ima->ibuf);

						tot= 0;
						seg = wireframe->first;
						while (seg) {
							tot++;
							seg = seg->next;
						}
	
						if(tot) {
							freedisplist(&(ob->disp));

							dl= callocN(sizeof(DispList), "makeDispListimage");
							dl->verts= callocN(3*sizeof(float)*tot, "dlverts");
							
							addtail(&(ob->disp), dl);
							dl->type= DL_POLY;
							dl->parts= 1;
							dl->nr= tot;
							
							xsi= 0.5*(tex->ima->ibuf->x);
							ysi= 0.5*(tex->ima->ibuf->y);
							xfac= me->size[0]/xsi;
							yfac= me->size[1]/ysi;
												
							data= dl->verts;
							seg = wireframe->first;
							while (seg) {
								data[0]= xfac*(seg->co[0]-xsi);
								data[1]= yfac*(seg->co[1]-ysi);
								data+= 3;
								seg = seg->next;
							}
							freelistN(wireframe);
						}
					}
				}
			}
		}
		base= base->next;
	}
	
	end_render_textures();
	
	allqueue(REDRAWVIEW3D, 0);
}

/* on frame change */

void test_all_displists()
{
	Base *base;
	Object *ob;
	uint lay;
	
	/* background */	
	if(G.vd==0) lay= G.scene->lay;
	else lay= G.vd->lay;
	
	base= FIRSTBASE;
	while(base) {
		if(base->lay & lay) {
			ob= base->object;
			
			if(ob->type==OB_MBALL && ob->ipo) {
				if(ob->disp.first) makeDispList(ob);
			}
			else if(ob->parent) {
				if ELEM(ob->parent->type, OB_LATTICE, OB_IKA) makeDispList(ob);
			}

			if ELEM(ob->type, OB_CURVE, OB_SURF) {
				if(ob!=G.obedit) {
					if( ((Curve *)(ob->data))->key ) makeDispList(ob);
				}
			}
			else if(ob->type==OB_FONT) {
				Curve *cu= ob->data;
				if(cu->textoncurve) {
					if( ((Curve *)cu->textoncurve->data)->key ) {
						text_to_curve(ob, 0);
						makeDispList(ob);
					}
				}
			}
			else if(ob->type==OB_MESH) {
				if(ob->effect.first) object_wave(ob);
				if(ob!=G.obedit) {
					if( ((Mesh *)(ob->data))->key ) makeDispList(ob);
				}
			}

		}
		if(base->next==0 && G.scene->set && base==G.scene->base.last) base= G.scene->set->base.first;
		else base= base->next;
	}
}


void boundbox_displist(Object *ob)
{
	BoundBox *bb;
	float min[3], max[3];
	DispList *dl;
	float *vert;
	int a, tot=0;
	
	INIT_MINMAX(min, max);

	if(ob->type==OB_MESH) {
		Mesh *me= ob->data;

		dl= find_displist(&ob->disp, DL_VERTS);
		if(!dl) return;

		if(me->bb==0) me->bb= callocN(sizeof(BoundBox), "boundbox");	
		bb= me->bb;

		dl= ob->disp.first;		
		
		vert= dl->verts;
		for(a=0; a<me->totvert; a++, vert+=3) {
			DO_MINMAX(vert, min, max);
		}
	}
	else if(ELEM3(ob->type, OB_CURVE, OB_SURF, OB_FONT)) {
		Curve *cu= ob->data;
		Nurb *nu;
		BPoint *bp;

		if(cu->bb==0) cu->bb= callocN(sizeof(BoundBox), "boundbox");	
		bb= cu->bb;
		
		dl= cu->disp.first;

		while (dl) {
			if(dl->type==DL_INDEX3 || dl->type==DL_INDEX3) tot= dl->nr;
			else tot= dl->nr*dl->parts;
			
			vert= dl->verts;
			for(a=0; a<tot; a++, vert+=3) {
				DO_MINMAX(vert, min, max);
			}

			dl= dl->next;
		}
	}
	
	bb->vec[0][0]=bb->vec[1][0]=bb->vec[2][0]=bb->vec[3][0]= min[0];
	bb->vec[4][0]=bb->vec[5][0]=bb->vec[6][0]=bb->vec[7][0]= max[0];
	
	bb->vec[0][1]=bb->vec[1][1]=bb->vec[4][1]=bb->vec[5][1]= min[1];
	bb->vec[2][1]=bb->vec[3][1]=bb->vec[6][1]=bb->vec[7][1]= max[1];

	bb->vec[0][2]=bb->vec[3][2]=bb->vec[4][2]=bb->vec[7][2]= min[2];
	bb->vec[1][2]=bb->vec[2][2]=bb->vec[5][2]=bb->vec[6][2]= max[2];
}


