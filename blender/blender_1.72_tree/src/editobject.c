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


/* editobject.c  		GRAPHICS
 * 
 * jan 95
 * 
 * Theorie: (matrices) A x B x C == A x ( B x C x Binv) x B
 * ofwel: OB x PAR x EDIT = OB x (PAR x EDIT x PARinv) x PAR
 * 
 * 
 */

#include "blender.h" 
#include "graphics.h"
#include "render.h"
#include "edit.h"
#include "ipo.h"
#include "effect.h"
#include "sector.h"
#include "ika.h"
#include <time.h>


TransOb *transmain= 0;
TransVert *transvmain= 0;
int tottrans=0, transmode=0;	/* 1: texspace */

float prop_size= 1.0;
int prop_mode= 0;
float prop_cent[3];

#define TRANS_TEX	1

float centre[3], centroid[3];

void add_object_draw(int type)	/* voor toolbox */
{
	Object *ob;
	
	if ELEM3(curarea->spacetype, SPACE_VIEW3D, SPACE_BUTS, SPACE_INFO) {
		if (G.obedit) exit_editmode(1);

		ob= add_object(type);
	
		if(type==OB_IKA) {
			where_is_object(ob);
			while(TRUE) {
				if( extrude_ika(ob, 1) ) break;
			}
			
			calc_ika(ob->data, 0);
			init_defstate_ika(ob);
		}
		
		allqueue(REDRAWVIEW3D, 0);
	}
}

void delete_obj(int ok)
{
	Base *base, *nbase;
	
	if(G.obedit) return;
	if(G.scene->id.lib) return;
	
	base= FIRSTBASE;
	while(base) {
		nbase= base->next;
		if TESTBASE(base) {
			if(ok==0 &&  (ok=okee("ERASE SELECTED"))==0) return;
			
			remlink(&(G.scene->base), base);
			if(base==BASACT) BASACT= 0;

			free_libblock_us(&(G.main->object), base->object);

			freeN(base);

		}
		base= nbase;
	}
	countall();

	G.f &= ~(G_VERTEXPAINT+G_FACESELECT);
	setcursor_space(SPACE_VIEW3D, CURSOR_STD);
	
	allqueue(REDRAWVIEW3D, 0);
	redraw_test_buttons(BASACT);
	allqueue(REDRAWIPO, 0);
	allqueue(REDRAWDATASELECT, 0);
	allqueue(REDRAWOOPS, 0);
	sector_update(0);
}


void make_track()
{
	Base *base;
	Object *par;
	
	if(G.scene->id.lib) return;
	if(G.obedit) {
		return;
	}
	if(BASACT==0) return;
	
	if(okee("Make Track")==0) return;
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			if(base!=BASACT) {

				base->object->track= BASACT->object;
			}
		}
		base= base->next;
	}
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWOOPS, 0);
	sort_baselist(G.scene);
}

void apply_obmat(Object *ob)
{
	float mat[3][3], imat[3][3], tmat[3][3], eul[3];
	
	if(ob==0) return;
	Mat3CpyMat4(mat, ob->obmat);
	
	VECCOPY(ob->loc, ob->obmat[3]);
	
	Mat3ToEul(mat, ob->rot);
	EulToMat3(ob->rot, tmat);
	Mat3Inv(imat, tmat);
	
	Mat3MulMat3(tmat, imat, mat);
	
	ob->size[0]= tmat[0][0];
	ob->size[1]= tmat[1][1];
	ob->size[2]= tmat[2][2];

}


void clear_parent()
{
	Object *par;
	Base *base;
	int mode;
	
	if(G.obedit) return;
	if(G.scene->id.lib) return;

	mode= pupmenu("OK? %t|Clear Parent %x1| ... and keep transform (clr track) %x2|Clear parent inverse %x3");
	
	if(mode<1) return;

	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			par= 0;
			if(mode==1 || mode==2) {
				if(base->object->type==OB_IKA) {
					Ika *ika= base->object->data;
					ika->parent= 0;
				}
				par= base->object->parent;
				base->object->parent= 0;
			
				if(mode==2) {
					base->object->track= 0;
					apply_obmat(base->object);
				}
			}
			else if(mode==3) {
				Mat4One(base->object->parentinv);
			}
			
			if(par) {
				if(par->type==OB_LATTICE) makeDispList(base->object);
				if(par->type==OB_IKA) makeDispList(base->object);
			}
		}
		base= base->next;
	}
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWOOPS, 0);
}

void clear_track()
{
	Base *base;
	int mode;
	
	if(G.obedit) return;
	if(G.scene->id.lib) return;

	mode= pupmenu("OK? %t|Clear Track %x1| ... and keep transform %x2");

	if(mode<1) return;

	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			base->object->track= 0;

			if(mode==2) {
				apply_obmat(base->object);
			}			
		}
		base= base->next;
	}
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWOOPS, 0);
}

void clear_object(char mode)
{
	Base *base;
	float *v1, *v2, *v3, vec[3], mat[3][3];
	
	if(G.obedit) return;
	if(G.scene->id.lib) return;
	
	if(mode=='r' && okee("Clear rotation")==0) return;
	else if(mode=='g' && okee("Clear location")==0) return;
	else if(mode=='s' && okee("Clear size")==0) return;
	else if(mode=='o' && okee("Clear origin")==0) return;
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			if(mode=='r') {
				bzero(base->object->rot, 3*sizeof(float));
				bzero(base->object->drot, 3*sizeof(float));
			}
			else if(mode=='g') {
				bzero(base->object->loc, 3*sizeof(float));
				bzero(base->object->dloc, 3*sizeof(float));
			}
			else if(mode=='s') {
				bzero(base->object->dsize, 3*sizeof(float));
				base->object->size[0]= 1.0;
				base->object->size[1]= 1.0;
				base->object->size[2]= 1.0;
			}
			else if(mode='o') {
				if(base->object->parent) {
					v1= base->object->loc;
					v3= base->object->parentinv[3];
					
					Mat3CpyMat4(mat, base->object->parentinv);
					VECCOPY(v3, v1);
					v3[0]= -v3[0];
					v3[1]= -v3[1];
					v3[2]= -v3[2];
					Mat3MulVecfl(mat, v3);
				}
			}
		}
		base= base->next;
	}
	
	allqueue(REDRAWVIEW3D, 0);
}

void reset_slowparents()
{
	/* terug op correcte plek */
	Base *base;
	
	base= FIRSTBASE;
	while(base) {
		if(base->object->parent) {
			if(base->object->partype & PARSLOW) {
				base->object->partype -= PARSLOW;
				where_is_object(base->object);
				base->object->partype |= PARSLOW;
			}
		}
		base= base->next;
	}
}

void set_slowparent()
{
	Base *base;

	if( okee("Set Slow parent")==0 ) return;

	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			if(base->object->parent) base->object->partype |= PARSLOW;
		}
		base= base->next;
	}
	
}

void make_vertex_parent()
{
	EditVert *eve;
	Base *base;
	Nurb *nu;
	BezTriple *bezt;
	BPoint *bp;
	Object *par, *ob;
	float vec[3];
	int a, v1=0, v2=0, v3=0, nr=1;
	
	/* er moet 1 of 3 vertices select zijn */
	
	if(G.obedit->type==OB_MESH) {
		eve= G.edve.first;
		while(eve) {
			if(eve->f & 1) {
				if(v1==0) v1= nr;
				else if(v2==0) v2= nr;
				else if(v3==0) v3= nr;
				else break;
			}
			nr++;
			eve= eve->next;
		}
	}
	else if ELEM(G.obedit->type, OB_SURF, OB_CURVE) {
		nu= editNurb.first;
		while(nu) {
			if((nu->type & 7)==CU_BEZIER) {
				bezt= nu->bezt;
				a= nu->pntsu;
				while(a--) {
					if(BEZSELECTED(bezt)) {
						if(v1==0) v1= nr;
						else if(v2==0) v2= nr;
						else if(v3==0) v3= nr;
						else break;
					}
					nr++;
					bezt++;
				}
			}
			else {
				bp= nu->bp;
				a= nu->pntsu*nu->pntsv;
				while(a--) {
					if(bp->f1 & SELECT) {
						if(v1==0) v1= nr;
						else if(v2==0) v2= nr;
						else if(v3==0) v3= nr;
						else break;
					}
					nr++;
					bp++;
				}
			}
			nu= nu->next;
		}
		
	}
	
	if( !(v1 && v2==0 && v3==0) && !(v1 && v2 && v3) ) {
		error("select 1 or 3 vertices");
		return;
	}
	
	if(okee("Make vertex-parent")==0) return;
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			if(base!=BASACT) {
				ob= base->object;
				par= BASACT->object->parent;
				
				while(par) {
					if(par==ob) break;
					par= par->parent;
				}
				if(par) {
					error("Loop in parents");
				}
				else {
					ob->parent= BASACT->object;
					if(v3) {
						ob->partype= PARVERT3;
						ob->par1= v1-1;
						ob->par2= v2-1;
						ob->par3= v3-1;

						/* inverse parent matrix berekenen */
						what_does_parent(ob);
						Mat4Invert(ob->parentinv, workob.obmat);
						clear_workob();
					}
					else {
						ob->partype= PARVERT1;
						ob->par1= v1-1;

						/* inverse parent matrix berekenen */
						what_does_parent(ob);
						Mat4Invert(ob->parentinv, workob.obmat);
						clear_workob();
					}
				}
			}
		}
		base= base->next;
	}
	allqueue(REDRAWVIEW3D, 0);
	
}

int test_parent_loop(Object *par, Object *ob)
{
	/* test if 'ob' is a parent somewhere in par's parents */
	
	if(par==0) return 0;
	if(ob == par) return 1;
	
	if(par->type==OB_IKA) {
		Ika *ika= par->data;
		
		if( ob == ika->parent ) return 1;
		if( test_parent_loop(ika->parent, ob) ) return 1;
	}

	return test_parent_loop(par->parent, ob);

}

void make_parent()
{
	Base *base;
	Object *ob, *par;
	Ika *ika;
	short qual, ok, mode=0, limbnr=0, effchild=0;
	
	if(G.scene->id.lib) return;
	if(G.obedit) {
		if ELEM3(G.obedit->type, OB_MESH, OB_CURVE, OB_SURF) make_vertex_parent();
		
		return;
	}
	if(BASACT==0) return;
	
	qual= G.qual;
	par= BASACT->object;
	
	if(par->type==OB_IKA) {
		
		if(qual & LR_SHIFTKEY)
			mode= pupmenu("Make Parent without inverse%t|Use Vertex %x1|Use Limb %x2|Use Skeleton %x3");
		else 
			mode= pupmenu("Make Parent %t|Use Vertex %x1|Use Limb %x2|Use Skeleton %x3");
		
		if(mode==1) {
			draw_ika_nrs(par, 0);
			if(button(&limbnr, 0, 99, "Vertex: ")==0) {
				allqueue(REDRAWVIEW3D, 0);
				return;
			}
		}
		else if(mode==2) {
			draw_ika_nrs(par, 1);
			if(button(&limbnr, 0, 99, "Limb: ")==0) {
				allqueue(REDRAWVIEW3D, 0);
				return;
			}
		}
		else if(mode==3) {
			ika= par->data;
			if(ika->def==0) {
				error("No skeleton available: use CTRL K");
				return;
			}
		}
		else return;

		if(mode==1) mode= PARVERT1;
		else if(mode==2) mode= PARLIMB;
		else if(mode==3) mode= PARSKEL;

		/* test effchild */
		base= FIRSTBASE;
		while(base) {
			if TESTBASELIB(base) {
				if(base->object->type==OB_IKA && base->object!=par && mode==PARVERT1 ) {
					if(effchild==0) {
						if(okee("Effector as Child")) effchild= 1;
						else effchild= 2;
					}
				}
			}
			if(effchild) break;
			base= base->next;
		}
	}
	else {
		if(qual & LR_SHIFTKEY) {
			if(okee("Make Parent without inverse")==0) return;
		}
		else {
			if(qual & LR_ALTKEY) {
				if(okee("Make VertexParent")==0) return;
			}
			else if(okee("Make Parent")==0) return;

			/* test effchild */
			base= FIRSTBASE;
			while(base) {
				if TESTBASELIB(base) {
					if(base->object->type==OB_IKA && base->object != par) {
						if(effchild==0) {
							if(okee("Effector as Child")) effchild= 1;
							else effchild= 2;
						}
					}
				}
				if(effchild) break;
				base= base->next;
			}
			
			/* nu gaan we alle objecten clearparentandkeeptransformen */
			base= FIRSTBASE;
			while(base) {
				if TESTBASELIB(base) {
					if(base!=BASACT && base->object->parent) {
						if(base->object->type==OB_IKA && effchild==1);
						else {
							base->object->parent= 0;
							apply_obmat(base->object);
						}
					}
				}
				base= base->next;
			}
		}
	}
	
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			if(base!=BASACT) {
				
				ok= 1;
				if(base->object->type==OB_IKA) {
				
					if(effchild==1) {
					
						if( test_parent_loop(par, base->object)==0 ) {
						
							Ika *ika= base->object->data;
							
							ika->parent= par;
							ika->par1= limbnr;
							ika->partype= mode;
							itterate_ika(base->object);
							ok= 0;
						}
						else {
							ok= 0;
							error("Loop in parents");
						}
					}
				}
				
				if(ok) {
					if( test_parent_loop(par, base->object) ) {
						error("Loop in parents");
					}
					else {
						
						if(par->type==OB_IKA) {
							base->object->partype= mode;
							base->object->par1= limbnr;
						}
						else if(qual & LR_ALTKEY) {
							base->object->partype= PARVERT1;
						}
						else {
							base->object->partype= PAROBJECT;
						}
						
						base->object->parent= par;
						
						/* inverse parent matrix berekenen? */
						if( (qual & LR_SHIFTKEY) || base->object->type==OB_LIFE || par->type==OB_LIFE) {
							/* niet dus... */
							Mat4One(base->object->parentinv);
							bzero(base->object->loc, 3*sizeof(float));
						}
						else {
							if(mode==PARSKEL) {
								/* base->object->partype= PAROBJECT; */
								what_does_parent(base->object);
								/* base->object->partype= mode; */
							}
							else what_does_parent(base->object);
							Mat4Invert(base->object->parentinv, workob.obmat);
						}
						
						if(par->type==OB_LATTICE) makeDispList(base->object);
						if(par->type==OB_IKA && mode==PARSKEL) makeDispList(base->object);
					}
				}
			}
		}
		base= base->next;
	}
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWOOPS, 0);
	
	sort_baselist(G.scene);
}


void enter_editmode()
{
	Base *base;
	Object *ob;
	Ika *ika;
	ID *id;
	Mesh *me;
	int ok= 0;
	
	if(G.scene->id.lib) return;
	base= BASACT;
	if(base==0) return;
	if((base->lay & G.vd->lay)==0) return;
	
	ob= base->object;
	if(ob->data==0) return;
	
	id= ob->data;
	if(id->lib) {
		error("Can't edit libdata");
		return;
	}
	
	
	if ELEM3(ob->type, OB_MESH, OB_SECTOR, OB_LIFE) {
		me= get_mesh(ob);
		if( me==0 ) return;
		if(me->id.lib) {
			error("Can't edit libdata");
			return;
		}
		ok= 1;
		G.obedit= ob;
		make_editMesh();
		allqueue(REDRAWBUTSGAME, 0);
	}
	else if(ob->type==OB_IKA) {	/* grabtype */
		base= FIRSTBASE;
		while(base) {
			if TESTBASE(base) {
				if(base->object->type==OB_IKA) {
					ika= base->object->data;
					if(ika->flag & IK_GRABEFF) ika->flag &= ~IK_GRABEFF;
					else ika->flag |= IK_GRABEFF;
				}
			}
			base= base->next;
		}
		allqueue(REDRAWVIEW3D, 0);
	}
	else if(ob->type==OB_FONT) {
		G.obedit= ob;
		ok= 1;
		make_editText();
	}
	else if(ob->type==OB_MBALL) {
		G.obedit= ob;
		ok= 1;
		make_editMball();
	}
	else if(ob->type==OB_LATTICE) {
		G.obedit= ob;
		ok= 1;
		make_editLatt();
	}
	else if(ob->type==OB_SURF || ob->type==OB_CURVE) {
		ok= 1;
		G.obedit= ob;
		make_editNurb();
	}
	allqueue(REDRAWBUTSEDIT, 0);
	countall();
	
	if(ok) {
		setcursor_space(SPACE_VIEW3D, CURSOR_EDIT);
	
		allqueue(REDRAWVIEW3D, 0);
	}
	else G.obedit= 0;
	
	addqueue(curarea->headwin, REDRAW, 1);
}

void exit_editmode(int freedata)	/* freedata==0 bij render */
{
	Base *base;
	Object *ob;
	Curve *cu;

	if(G.obedit==0) return;

	if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) {

		/* tijdelijk */
		countall();
		if(G.totvert>65000) {
			error("too many vertices");
			return;
		}
		load_editMesh();

		if(freedata) free_editmesh();
		if(G.f & G_FACESELECT) allqueue(REDRAWIMAGE, 0);
		
		build_particle_system(G.obedit);
	}
	else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) {
		load_editNurb();
		if(freedata) freeNurblist(&editNurb);
	}
	else if(G.obedit->type==OB_FONT && freedata==1) {
		load_editText();
	}
	else if(G.obedit->type==OB_LATTICE) {
		load_editLatt();
		if(freedata) free_editLatt();
	}
	else if(G.obedit->type==OB_MBALL) {
		load_editMball();
		if(freedata) freelistN(&editelems);
	}

	ob= G.obedit;
	
	/* obedit moet 0 zijn voor curve-extrude, niet voor smeshes */
	if(ob->type==OB_CURVE) G.obedit= 0;
	G.obedit= 0;
	makeDispList(ob);
	
	

	/* heeft dit nog invloed op andere bases? */
	if(ob->type==OB_CURVE) {

		/* test of ob als bevelcurve of textoncurve wordt gebruikt */
		base= FIRSTBASE;
		while(base) {
			if ELEM(base->object->type, OB_CURVE, OB_FONT) {
				cu= base->object->data;
				
				if(cu->textoncurve==ob) {
					text_to_curve(base->object, 0);
					makeDispList(base->object);
				}
				if(cu->bevobj== ob) {
					makeDispList(base->object);
				}
			}
			base= base->next;
		}
		
	}
	else if(ob->type==OB_LATTICE) {
		base= FIRSTBASE;
		while(base) {
			if(base->object->parent==ob) makeDispList(base->object);
			base= base->next;
		}
	}

	if(freedata) {
		setcursor_space(SPACE_VIEW3D, CURSOR_STD);
	
		countall();
		allqueue(REDRAWVIEW3D, 0);
		allqueue(REDRAWBUTSEDIT, 0);
		allqueue(REDRAWBUTSGAME, 0);
	}
	else {
		G.obedit= ob;
	}
	addqueue(curarea->headwin, REDRAW, 1);

}

void check_editmode(int type)
{
	if (G.obedit==0 || G.obedit->type==type) return;

	exit_editmode(1);
}

static int centremode= 0; /* 0 == do centre, 1 == centre new, 2 == centre cursor */

void docentre()
{
	Base *base;
	Object *ob;
	Mesh *me, *meo, *tme, *tmeo;
	Curve *cu;
	MVert *mvert;
	BezTriple *bezt;
	BPoint *bp;
	Nurb *nu, *nu1;
	EditVert *eve;
	float *fp, fac, cent[3], centn[3], min[3], max[3], mat[4][4], omat[3][3];
	int c,a;

	if(G.scene->id.lib) return;

	if(G.obedit) {

		INIT_MINMAX(min, max);
	
		if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) {
			eve= G.edve.first;
			while(eve) {
				DO_MINMAX(eve->co, min, max);
				eve= eve->next;
			}
			cent[0]= (min[0]+max[0])/2.0;
			cent[1]= (min[1]+max[1])/2.0;
			cent[2]= (min[2]+max[2])/2.0;
			
			eve= G.edve.first;
			while(eve) {
				VecSubf(eve->co, eve->co, cent);			
				eve= eve->next;
			}
		}
	}
	
	/* vlaggen resetten */
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			base->object->flag &= ~OB_DONE;
		}
		base= base->next;
	}
	me= G.main->mesh.first;
	while(me) {
		me->flag &= ~ME_ISDONE;
		me= me->id.next;
	}
	
	base= FIRSTBASE;
	while(base) {
		
		if TESTBASELIB(base) {
			if((base->object->flag & OB_DONE)==0) {
				
				base->object->flag |= OB_DONE;
				
				if(G.obedit==0 && (me=get_mesh(base->object)) ) {
					
					if(me->key) {
						error("Mesh with vertexkey!");
						return;
					}
					
					if(centremode==2) {
						float *curs= (float*)give_cursor();
					
						VECCOPY(cent, curs);
						Mat4Invert(base->object->imat, base->object->obmat);
						Mat4MulVecfl(base->object->imat, cent);
					} else {
						INIT_MINMAX(min, max);
		
						mvert= me->mvert;
						for(a=0; a<me->totvert; a++, mvert++) {
							DO_MINMAX(mvert->co, min, max);
						}
				
						cent[0]= (min[0]+max[0])/2.0;
						cent[1]= (min[1]+max[1])/2.0;
						cent[2]= (min[2]+max[2])/2.0;
					}
						
					mvert= me->mvert;
					for(a=0; a<me->totvert; a++, mvert++) {
						VecSubf(mvert->co, mvert->co, cent);
					}
					me->flag |= ME_ISDONE;
					
					if(meo=get_other_mesh(base->object)) {					
						mvert= meo->mvert;
						for(a=0; a<meo->totvert; a++, mvert++) {
							VecSubf(mvert->co, mvert->co, cent);
						}
						tex_space_mesh(meo);
						meo->flag |= ME_ISDONE;
					}
					
					if(centremode) {
						Mat3CpyMat4(omat, base->object->obmat);
						
						VECCOPY(centn, cent);
						Mat3MulVecfl(omat, centn);
						base->object->loc[0]+= centn[0];
						base->object->loc[1]+= centn[1];
						base->object->loc[2]+= centn[2];
						
						/* andere users? */
						ob= G.main->object.first;
						while(ob) {
							if((ob->flag & OB_DONE)==0) {
								tme= get_mesh(ob);
								tmeo= get_other_mesh(ob);
								
								if(tme==me || tmeo==me) {
									
									ob->flag |= OB_DONE;

									Mat3CpyMat4(omat, ob->obmat);
									VECCOPY(centn, cent);
									Mat3MulVecfl(omat, centn);
									ob->loc[0]+= centn[0];
									ob->loc[1]+= centn[1];
									ob->loc[2]+= centn[2];
									
									if(tme && (tme->flag & ME_ISDONE)==0) {
										mvert= tme->mvert;
										for(a=0; a<tme->totvert; a++, mvert++) {
											VecSubf(mvert->co, mvert->co, cent);
										}
										tme->flag |= ME_ISDONE;
									}
									if(tmeo && (tmeo->flag & ME_ISDONE)==0) {
										mvert= tmeo->mvert;
										for(a=0; a<tmeo->totvert; a++, mvert++) {
											VecSubf(mvert->co, mvert->co, cent);
										}
										tmeo->flag |= ME_ISDONE;
									}
								}
							}
							
							ob= ob->id.next;
						}
					}
				
					/* displisten van alle users, ook deze base */
					makeDispList(base->object);
					
					/* DOEN: alle users aflopen... */
					tex_space_mesh(me);
		
				}
				else if ELEM(base->object->type, OB_CURVE, OB_SURF) {
									
					if(G.obedit) {
						nu1= editNurb.first;
					}
					else {
						cu= base->object->data;
						nu1= cu->nurb.first;
					}
					
					if(centremode==2) {
						float *curs= (float*)give_cursor();
					
						cent[0]= curs[0];
						cent[1]= curs[1];
						cent[2]= curs[2];
						VecSubf(cent, cent, base->object->loc);						
					} else {
						INIT_MINMAX(min, max);
	
						nu= nu1;
						while(nu) {
							minmaxNurb(nu, min, max);
							nu= nu->next;
						}
						
						cent[0]= (min[0]+max[0])/2.0;
						cent[1]= (min[1]+max[1])/2.0;
						cent[2]= (min[2]+max[2])/2.0;
					}
								
					nu= nu1;
					while(nu) {
						if( (nu->type & 7)==1) {
							a= nu->pntsu;
							bezt= nu->bezt;
							while(a--) {
								VecSubf(bezt->vec[0], bezt->vec[0], cent);
								VecSubf(bezt->vec[1], bezt->vec[1], cent);
								VecSubf(bezt->vec[2], bezt->vec[2], cent);
								bezt++;
							}
						}
						else {
							a= nu->pntsu*nu->pntsv;
							bp= nu->bp;
							while(a--) {
								VecSubf(bp->vec, bp->vec, cent);
								bp++;
							}
						}
						nu= nu->next;
					}
			
					if(centremode && G.obedit==0) {
						Mat3CpyMat4(omat, base->object->obmat);
						
						Mat3MulVecfl(omat, cent);
						base->object->loc[0]+= cent[0];
						base->object->loc[1]+= cent[1];
						base->object->loc[2]+= cent[2];
					}
			
					if(G.obedit) {
						makeDispList(G.obedit);
						break;
					}
					else makeDispList(base->object);
	
				}
				else if(base->object->type==OB_FONT) {
					/* uit de bb halen */
					
					cu= base->object->data;
					if(cu->bb==0) return;
					
					cu->xof= -0.5*( cu->bb->vec[4][0] - cu->bb->vec[0][0]);
					cu->yof= -0.5 -0.5*( cu->bb->vec[0][1] - cu->bb->vec[2][1]);	/* extra 0.5 is de hoogte van de bovenste regel */
					
					/* klopt niet helemaal, een keer goed doen! */
					cu->xof /= cu->fsize;
					cu->yof /= cu->fsize;
					
					text_to_curve(base->object, 0);
					makeDispList(base->object);
					
					allqueue(REDRAWBUTSEDIT, 0);
				}
			}
		}
		base= base->next;
	}

	allqueue(REDRAWVIEW3D, 0);
}

void docentre_new()
{
	if(G.scene->id.lib) return;

	if(G.obedit) {
		error("Unable to perform function in EditMode");
	}
	else {
		centremode= 1;
		docentre();
		centremode= 0;
	}
}

void docentre_cursor()
{
	if(G.scene->id.lib) return;

	if(G.obedit) {
		error("Unable to perform function in EditMode");
	}
	else {
		centremode= 2;
		docentre();
		centremode= 0;
	}
}

void movetolayer()
{
	Base *base;
	uint lay= 0, local;

	if(G.scene->id.lib) return;

	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) lay |= base->lay;
		base= base->next;
	}
	if(lay==0) return;
	lay &= 0xFFFFFF;
	
	if( movetolayer_buts(&lay)==0 ) return;
	if(lay==0) return;

	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) {
			local= base->lay & 0xFF000000;
			base->lay= lay + local;

			base->object->lay= lay;
		}
		base= base->next;
	}
	countall();
	allqueue(REDRAWBUTSEDIT, 0);
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWOOPS, 0);
	allqueue(REDRAWINFO, 0);
}


void special_editmenu()
{
	extern short editbutflag;
	extern float doublimit;
	EditVlak *evl;
	float fac;
	int a, nr;
	short randfac;
	char str[32];
	
	if(G.obedit==0) {
		if(G.f & G_FACESELECT) {
			Mesh *me= get_mesh(OBACT);
			TFace *tface;
			int a;
			
			if(me==0 || me->tface==0) return;
			
			nr= pupmenu("Specials%t|Set Gouraud|       Tex|       Shared|       Light|Clr Gouraud|       Tex|       Shared|       Light|Spread Vertcolors");
	
			for(a=me->totface, tface= me->tface; a>0; a--, tface++) {
				if(tface->flag & SELECT) {
					switch(nr) {
					case 1:
						tface->mode |= TF_GOUR; break;
					case 2:
						tface->mode |= TF_TEX; break;
					case 3:
						tface->mode |= TF_SHAREDCOL; break;
					case 4:
						tface->mode |= TF_LIGHT; break;
					case 5:
						tface->mode &= ~TF_GOUR; break;
					case 6:
						tface->mode &= ~TF_TEX;
						tface->tpage= 0;
						break;
					case 7:
						tface->mode &= ~TF_SHAREDCOL; break;
					case 8:
						tface->mode &= ~TF_LIGHT; break;
					}
				}
			}
			if(nr==9) {
				tface_to_mcol(me);
				do_shared_vertexcol(me);
				mcol_to_tface(me, 1);
			}
			allqueue(REDRAWVIEW3D, 0);
			allqueue(REDRAWBUTSGAME, 0);
		}
		else if(G.f & G_VERTEXPAINT) {
			Mesh *me= get_mesh(OBACT);
			int a;
			
			if(me==0 || me->mcol==0) return;
			
			nr= pupmenu("Specials%t|Shared VertexCol%x1");
			if(nr==1) {
				copy_vpaint_undo( (uint *)me->mcol, me->totface);
				do_shared_vertexcol(me);
			}
		}
	}
	else if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) {

		nr= pupmenu("Specials%t|Subdivide%x1|Subdivide Fractal%x2|Subdivide Smooth%x3|Remove Doubles%x4|Hide%x5|Reveal%x6|Select swap%x7|Flip Normals %x8|Smooth %x9");
		
		if(nr>0) waitcursor(1);
		
		switch(nr) {
		case 1:
			subdivideflag(1, 0.0, editbutflag);
			break;
		case 2:
			randfac= 10;
			if(button(&randfac, 1, 100, "Rand fac:")==0) return;
			fac= -( (float)randfac )/100;
			subdivideflag(1, fac, editbutflag);
			break;
		case 3:
			subdivideflag(1, 0.0, editbutflag | B_SMOOTH);
			break;
		case 4:
			a= removedoublesflag(1, doublimit);
			sprintf(str, "Removed: %d\n", a);
			notice(str);
			break;
		case 5:
			hide_mesh(0);
			break;
		case 6:
			reveal_mesh();
			break;
		case 7:
			selectswap_mesh();
			break;
		case 8:
			flip_editnormals();
			break;
		case 9:
			vertexsmooth();
			break;
		}		
		
		makeDispList(G.obedit);
		
		if(nr>0) waitcursor(0);
		
	}
	else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) {

		/* nr= pupmenu("Specials%t|Subdivide%x1|Hide%x5|Reveal%x6|Select swap%x7"); */
		nr= pupmenu("Specials%t|Subdivide%x1|Switch Direction");
		
		switch(nr) {
		case 1:
			subdivideNurb();
			break;
		case 2:
			switchdirectionNurb2();
			break;
		}
	}
	countall();
	allqueue(REDRAWVIEW3D, 0);
	
}

void convertmenu()
{
	Base *base, *basen, *basact;
	Object *ob, *ob1;
	Curve *cu;
	MetaBall *mb;
	Mesh *me;
	DispList *dl;
	int ok=0, nr, a;
	
	if(G.scene->id.lib) return;

	ob= OBACT;
	if(ob==0) return;
	if(G.obedit) return;
	
	basact= BASACT;	/* will be restored */
		
	if(ob->type==OB_FONT) {
		nr= pupmenu("Convert Font to%t|Curve");
		if(nr>0) ok= 1;
	}
	else if(ob->type==OB_MBALL) {
		nr= pupmenu("Convert MetaBall to%t|Mesh (keep orginal)");
		if(nr>0) ok= 1;
	}
	else if(ob->type==OB_CURVE) {
		nr= pupmenu("Convert Curve to%t|Mesh");
		if(nr>0) ok= 1;
	}
	else if(ob->type==OB_SURF) {
		nr= pupmenu("Convert Nurbs Surf to%t|Mesh");
		if(nr>0) ok= 1;
	}
	if(ok==0) return;

	/* denk aan meerdere users! */

	/* vlaggen resetten */
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			base->object->flag &= ~OB_DONE;
		}
		base= base->next;
	}

	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			
			ob= base->object;
			
			if(ob->flag & OB_DONE);
			else if(ob->type==OB_FONT) {
				if(nr==1) {
				
					ob->flag |= OB_DONE;
				
					ob->type= OB_CURVE;
					cu= ob->data;
					
					if(cu->vfont) {
						cu->vfont->id.us--;
						cu->vfont= 0;
					}
					/* andere users */
					if(cu->id.us>1) {
						ob1= G.main->object.first;
						while(ob1) {
							if(ob1->data==cu) ob1->type= OB_CURVE;
							ob1= ob1->id.next;
						}
					}
				}
			}
			else if ELEM(ob->type, OB_CURVE, OB_SURF) {
				if(nr==1) {

					ob->flag |= OB_DONE;
					cu= ob->data;
					
					dl= cu->disp.first;
					if(dl==0) makeDispList(ob);

					nurbs_to_mesh(ob); /* doet ook users */

					/* texspace en normalen */
					BASACT= base;
					enter_editmode();
					exit_editmode(1);
					BASACT= basact;
				}
			}
			else if(ob->type==OB_MBALL) {
			
				if(nr==1) {
					
					if(ob->disp.first) {
					
						ob->flag |= OB_DONE;

						ob1= copy_object(ob);

						basen= mallocN(sizeof(Base), "duplibase");
						*basen= *base;
						addhead(&G.scene->base, basen);	/* addhead: anders oneindige lus */
						basen->object= ob1;
						basen->flag &= ~SELECT;
						
						mb= ob1->data;
						mb->id.us--;
						
						ob1->data= add_mesh();
						G.totmesh++;
						ob1->type= OB_MESH;
						
						me= ob1->data;
						if(ob1->totcol) {
							me->mat= dupallocN(mb->mat);
							for(a=0; a<ob1->totcol; a++) id_us_plus((ID *)me->mat[a]);
						}
						
						mball_to_mesh(&ob->disp, ob1->data);
						
					}
				}
			}
		}
		base= base->next;
	}
	
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWOOPS, 0);
	allqueue(REDRAWBUTSEDIT, 0);
}

void copymenu()
{
	Object *ob, *obt;
	Base *base;
	Curve *cu, *cu1;
	Sector *sector, *se;
	Life *life, *lf;
	Camera *camera;
	void *poin1, *poin2=0;
	int a;
	short event;
	char str[140];
	
	if(G.scene->id.lib) return;

	if(OBACT==0) return;
	if(G.obedit) {
		/* obedit_copymenu(); */
		return;
	}
	
	strcpy(str, "COPY %t|Loc%x1|Rot%x2|Size%x3|Drawtype%x4|TimeOffs%x5|Dupli%x6");

	ob= OBACT;
	
	if(ob->type==OB_SECTOR) sector= ob->data;
	else sector= 0;
	if(ob->type==OB_LIFE) life= ob->data;
	else life= 0;
	if(ob->type==OB_CAMERA) camera= ob->data;
	else camera= 0;
	
	if ELEM5(ob->type, OB_MESH, OB_CURVE, OB_SURF, OB_FONT, OB_MBALL) {
		strcat(str, "|Tex Space%x7");
		if(ob->type==OB_MESH) poin2= &(((Mesh *)ob->data)->texflag);
		else if ELEM3(ob->type, OB_CURVE, OB_SURF, OB_FONT) poin2= &(((Curve *)ob->data)->texflag);
		else if(ob->type==OB_MBALL) poin2= &(((MetaBall *)ob->data)->texflag);
	}	
	
	if(ob->type == OB_FONT) strcat(str, "|Font Settings%x8|Bevel Settings%x9");
	if(ob->type == OB_CURVE) strcat(str, "|Bevel Settings%x9");
	if(ob->type == OB_SECTOR) strcat(str, "|Portals%x11|Sectorsize%x12|Render Network%x17");
	if(ob->type == OB_LIFE) strcat(str, "|Sensors%x13");

#ifndef FREE
	if(ob->type == OB_CAMERA) strcat(str, "|Lens%x14|Hold%x15|NetworkClip%x16");
#else
	if(ob->type == OB_CAMERA) strcat(str, "|Lens%x14");
#endif	

	if( give_parteff(ob) ) strcat(str, "|Particle Settings%x10");

	event= pupmenu(str);
	if(event<= 0) return;

	base= FIRSTBASE;
	while(base) {
		if(base != BASACT) {
			if(TESTBASELIB(base)) {
				
				if(event==1) {  /* loc */
					VECCOPY(base->object->loc, ob->loc);
					VECCOPY(base->object->dloc, ob->dloc);
				}
				else if(event==2) {  /* rot */
					VECCOPY(base->object->rot, ob->rot);
					VECCOPY(base->object->drot, ob->drot);
				}
				else if(event==3) {  /* size */
					VECCOPY(base->object->size, ob->size);
					VECCOPY(base->object->dsize, ob->dsize);
				}
				else if(event==4) {  /* drawtype */
					base->object->dt= ob->dt;
					base->object->dtx= ob->dtx;
					if(sector && base->object->type==OB_SECTOR) {
						se= base->object->data;
						se->flag = sector->flag;
					}
					if(life && base->object->type==OB_LIFE) {
						lf= base->object->data;

						if(life->flag & LF_SHOW_TEXMESH) lf->flag |= LF_SHOW_TEXMESH;
						else lf->flag &= ~LF_SHOW_TEXMESH;

						if(life->flag & LF_GHOST_OTHER) lf->flag |= LF_GHOST_OTHER;
						else lf->flag &= ~LF_GHOST_OTHER;
					}
				}
				else if(event==5) {  /* time offs */
					base->object->sf= ob->sf;
				}
				else if(event==6) {  /* dupli */
					base->object->dupon= ob->dupon;
					base->object->dupoff= ob->dupoff;
					base->object->dupsta= ob->dupsta;
					base->object->dupend= ob->dupend;
					
					base->object->transflag &= ~OB_DUPLI;
					base->object->transflag |= (ob->transflag & OB_DUPLI);
					
				}
				else if(event==7) {	/* tex space */
					obt= base->object;
					poin1= 0;
					if(obt->type==OB_MESH) poin1= &(((Mesh *)obt->data)->texflag);
					else if ELEM3(obt->type, OB_CURVE, OB_SURF, OB_FONT) poin1= &(((Curve *)obt->data)->texflag);
					else if(obt->type==OB_MBALL) poin1= &(((MetaBall *)obt->data)->texflag);					
					
					if(poin1) {
						memcpy(poin1, poin2, 4+12+12+12);
					
						if(obt->type==OB_MESH) tex_space_mesh(obt->data);
						else if(obt->type==OB_MBALL) tex_space_mball(obt);
						else tex_space_curve(obt->data);
					}
				}
				else if(event==8) {	/* font settings */
					
					if(base->object->type==ob->type) {
						cu= ob->data;
						cu1= base->object->data;
						
						cu1->spacemode= cu->spacemode;
						cu1->spacing= cu->spacing;
						cu1->linedist= cu->linedist;
						cu1->shear= cu->shear;
						cu1->fsize= cu->fsize;
						cu1->xof= cu->xof;
						cu1->yof= cu->yof;
						cu1->textoncurve= cu->textoncurve;
						if(cu1->vfont) cu1->vfont->id.us--;
						cu1->vfont= cu->vfont;
						id_us_plus((ID *)cu1->vfont);
						text_to_curve(base->object, 0);
						
						strcpy(cu1->family, cu->family);
						
						makeDispList(base->object);
					}
				}
				else if(event==9) {	/* bevel settings */
					
					if ELEM(base->object->type, OB_CURVE, OB_FONT) {
						cu= ob->data;
						cu1= base->object->data;
						
						cu1->bevobj= cu->bevobj;
						cu1->width= cu->width;
						cu1->bevresol= cu->bevresol;
						cu1->ext1= cu->ext1;
						cu1->ext2= cu->ext2;
						
						makeDispList(base->object);
					}
				}
				else if(event==10) {	/* particle settings */
					PartEff *pa1, *pa2;
					char *p1, *p2;
					
					pa1= give_parteff(ob);
					pa2= give_parteff(base->object);

					if(pa1==0 && pa2) {
						remlink( &(base->object->effect), pa2);
						free_effect( (Effect *) pa2);
					}
					else if(pa1 && pa2==0) {
						copy_effects(&(base->object->effect), &ob->effect);
						build_particle_system(base->object);
					}
					else if(pa1 && pa2) {
						if(pa2->keys) freeN(pa2->keys);
						
						p1= (char *)pa1; p2= (char *)pa2;
						memcpy( p2+8, p1+8, sizeof(PartEff) - 8);
						pa2->keys= 0;
						
						build_particle_system(base->object);
					}
				}
				else if(event==11) {	/* portals */
					Portal *po;
					if(sector && base->object->type==OB_SECTOR) {
						se= base->object->data;
						if(se->portals) freeN(se->portals);
						se->portals= dupallocN(sector->portals);
						se->totport= sector->totport;
						se->actport= sector->actport;
						
						/* op nul zetten: anders verwijzing naar zichzelf */
						po= se->portals;
						a= se->totport;
						while(a--) {
							po->sector= 0;
							po++;
						}
					}
				}
				else if(event==12) {	/* sector size */
					if(sector && base->object->type==OB_SECTOR) {
						se= base->object->data;
						VECCOPY(se->size, sector->size);
					}
				}
				else if(event==13) {	/* sensors */
					Sensor *sn;
					if(life && base->object->type==OB_LIFE) {
						lf= base->object->data;
						
						if(lf->sensors) {
							for(a=lf->totsens, sn=lf->sensors; a>0; a--, sn++) {
								if(sn->events) freeN(sn->events);
								if(sn->actions) freeN(sn->actions);
							}
							freeN(lf->sensors);
						}
						
						lf->sensors= dupallocN(life->sensors);
						lf->totsens= life->totsens;
						lf->actsens= life->actsens;
						for(a=lf->totsens, sn=lf->sensors; a>0; a--, sn++) {
							sn->events= dupallocN(sn->events);
							sn->actions= dupallocN(sn->actions);
						}
						lf->flag |= LF_SENSORS;
					}
				}
				else if(event>=14 && event<=16) {	/* lens / hold / netclip */
					Camera *cam;
					
					if(camera && base->object->type==OB_CAMERA) {
						cam= base->object->data;
						if(event==14) cam->lens= camera->lens;
						if(event==15) cam->hold= camera->hold;
						if(event==16) {
							cam->netsta= camera->netsta;
							cam->netend= camera->netend;
						}
					}
				}
				else if(event==17) {	/* render network */
					if(sector && base->object->type==OB_SECTOR) {
						se= base->object->data;
						if(sector->flag & SE_RENDER_NETWORK) se->flag |= SE_RENDER_NETWORK;
						else se->flag &= ~SE_RENDER_NETWORK;
					}
				}
			}
		}
		base= base->next;
	}
	
	allqueue(REDRAWVIEW3D, 0);
	if(event==10) {
		allqueue(REDRAWBUTSANIM, 0);
	}
	else if ELEM3(event, 11, 12, 13) {
		allqueue(REDRAWBUTSGAME, 0);
	}
	
}

void link_to_scene(ushort nr)
{	
	Base *base, *nbase;
	Scene *sce;
	
	/* nr is event, niet nodig */
	/* in G.curscreen->scenenr staat de geselecteerde scene */
	
	sce= G.main->scene.first;
	nr= 0;
	while(sce) {
		if(nr==G.curscreen->scenenr) {
			if(sce!=G.scene) break;
		}
		nr++;
		sce= sce->id.next;
	}

	if(sce==0) return;
	if(sce->id.lib) return;
	
	base= FIRSTBASE;
	while(base) {
		if(TESTBASE(base)) {
			
			nbase= mallocN( sizeof(Base), "newbase");
			*nbase= *base;
			addhead( &(sce->base), nbase);
			id_us_plus((ID *)base->object);
		}
		base= base->next;
	}
}

void linkmenu()
{
	Object *ob, *obt;
	Base *base, *nbase, *sbase;
	Scene *sce;
	Mesh *me;
	ID *id;
	Sector *se, *seo;
	Life *lf, *lfo;
	Material ***matarar, ***obmatarar, **matar1, **matar2;
	int a;
	short event, *totcolp, nr;
	char str[140], *strp;
	

	if(OBACT==0) return;
	ob= OBACT;
	
	strcpy(str, "MAKE LINKS %t|To scene...%x1|Object Ipo%x4");
	
	if(ob->type==OB_MESH)
		strcat(str, "|Mesh data%x2|Materials%x3");
	else if(ob->type==OB_CURVE)
		strcat(str, "|Curve data%x2|Materials%x3");
	else if(ob->type==OB_FONT)
		strcat(str, "|Font data%x2|Materials%x3");
	else if(ob->type==OB_SURF)
		strcat(str, "|Surf data%x2|Materials%x3");
	else if(ob->type==OB_MBALL)
		strcat(str, "|Materials%x3");
	else if(ob->type==OB_CAMERA)
		strcat(str, "|Camera data%x2");
	else if(ob->type==OB_LAMP)
		strcat(str, "|Lamp data%x2");
	else if(ob->type==OB_LATTICE)
		strcat(str, "|Lattice data%x2");
	else if ELEM(ob->type, OB_SECTOR, OB_LIFE)
		strcat(str, "|TexMesh data%x2|DynaMesh data %x5|Materials%x3");

	event= pupmenu(str);
	if(event<= 0) return;

	if(event==1) {
		IDnames_to_pupstring(&strp, &(G.main->scene), 0, &nr);
		
		if(strncmp(strp, "DataBrow", 8)==0) {
			freeN(strp);

			activate_databrowse((ID *)G.scene, ID_SCE, 0, B_INFOSCE, link_to_scene );
			
			return;			
		}
		else {
			event= pupmenu(strp);
			freeN(strp);
		
			if(event<= 0) return;
		
			nr= 1;
			sce= G.main->scene.first;
			while(sce) {
				if(nr==event) break;
				nr++;
				sce= sce->id.next;
			}
			if(sce==G.scene) {
				error("This is current scene");
				return;
			}
			if(sce==0 || sce->id.lib) return;
			
			/* denk eraan: is verderop nog nodig */
			event= 1;
		}
	}

	base= FIRSTBASE;
	while(base) {
		if(event==1 || base != BASACT) {
			
			obt= base->object;

			if(TESTBASE(base)) {
				
				if(event==1) {		/* to scene */
					
					/* testen of het soms al gelinkt is */
					sbase= sce->base.first;
					while(sbase) {
						if(sbase->object==base->object) break;
						sbase= sbase->next;
					}
					if(sbase) {	/* eruit */
						base= base->next;
						continue;
					}
					
					nbase= mallocN( sizeof(Base), "newbase");
					*nbase= *base;
					addhead( &(sce->base), nbase);
					id_us_plus((ID *)base->object);
				}
			}
			if(TESTBASELIB(base)) {
				if(event==2 || event==5) {  /* obdata */
					if(ob->type==obt->type) {
						
						if(ob->type==OB_SECTOR) {
							se= obt->data;
							seo= ob->data;
							
							if(event==2) {	/* texmesh */
								id= (ID *)se->texmesh;
								if(id) id->us--;
								id= (ID *)seo->texmesh;
								id_us_plus(id);
								se->texmesh= (Mesh *)id;
							}
							else {	/* dynamesh */
								id= (ID *)se->dynamesh;
								if(id) id->us--;
								id= (ID *)seo->dynamesh;
								id_us_plus(id);
								se->dynamesh= (Mesh *)id;								
							}
						}
						else if(ob->type==OB_LIFE) {
							lf= obt->data;
							lfo= ob->data;
							
							if(event==2) {	/* texmesh */
								id= (ID *)lf->texmesh;
								if(id) id->us--;
								id= (ID *)lfo->texmesh;
								id_us_plus(id);
								lf->texmesh= (Mesh *)id;
							}
							else {	/* dynamesh */
								id= (ID *)lf->dynamesh;
								if(id) id->us--;
								id= (ID *)lfo->dynamesh;
								id_us_plus(id);
								lf->dynamesh= (Mesh *)id;								
							}
						}
						else {
							id= obt->data;
							id->us--;
							
							id= ob->data;
							id_us_plus(id);
							obt->data= id;
							
							/* als aantal mat indices veranderd zijn: */
							test_object_materials(obt->data);
						}
					}
				}
				else if(event==4) {  /* ob ipo */
					if(obt->ipo) obt->ipo->id.us--;
					obt->ipo= ob->ipo;
					if(obt->ipo) {
						id_us_plus((ID *)obt->ipo);
						do_ob_ipo(obt);
					}
				}
				else if(event==3) {  /* materials */
					
					/* alleen als obt geen materiaal heeft: arrays maken */
					/* van ob naar obt! */
					
					obmatarar= give_matarar(ob);
					matarar= give_matarar(obt);
					totcolp= give_totcolp(obt);

					/* als 1 van de 2 nul is: geen renderbaar object */						
					if( matarar && obmatarar) {
						
						/* voorzichtig met users! Dus eerst kopie orig: */

						if(ob->totcol) {
							matar1= dupallocN(ob->mat);
							matar2= dupallocN(*obmatarar);
						}
						else {
							matar1= matar2= 0;
						}
						
						/* alles van obt los linken */
						for(a=0; a<obt->totcol; a++) {
							if(obt->mat[a]) obt->mat[a]->id.us--;
							if( (*matarar)[a]) (*matarar)[a]->id.us--;
						}
						
						/* vrijgeven */
						if(obt->mat) freeN(obt->mat);
						if(*matarar) freeN(*matarar);
						
						/* hangen kopie er aan */
						obt->mat= matar1;
						*matarar= matar2;
						obt->totcol= ob->totcol;
						*totcolp= ob->totcol;
					
						/* users ophogen */
						for(a=0; a<obt->totcol; a++) {
							if(obt->mat[a]) id_us_plus((ID *)obt->mat[a]);
							if( (*matarar)[a]) id_us_plus((ID *)(*matarar)[a]);
						}

						obt->colbits= ob->colbits;
						
						/* als aantal mat indices veranderd zijn: */
						test_object_materials(obt->data);
					}
				}
			}
		}
		base= base->next;
	}
	
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWOOPS, 0);
	allqueue(REDRAWBUTSHEAD, 0);
}

void make_duplilist_real()
{
	Base *base, *basen;
	Object *ob;
	extern ListBase duplilist;
	
	if(okee("Make dupli's real")==0) return;
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {

			if(base->object->transflag & OB_DUPLI) {
				
				make_duplilist(G.scene, base->object);
				ob= duplilist.first;
				while(ob) {
					
					/* font dupli's kunnen totcol hebben zonder mat, halen ze van parent af
					 * dit zou netter moeten
					 */
					if(ob->mat==0) ob->totcol= 0;
					
					basen= dupallocN(base);
					basen->flag &= ~OB_FROMDUPLI;
					addhead(&G.scene->base, basen);	/* addhead: anders oneindige lus */
					ob->ipo= 0;		/* zeker weten dat de apply werkt */
					ob->parent= ob->track= 0;
					ob->disp.first= ob->disp.last= 0;
					ob->transflag &= ~OB_DUPLI;
					basen->object= copy_object(ob);
					
					apply_obmat(basen->object);
					ob= ob->id.next;
				}
				
				free_duplilist();
				
				base->object->transflag &= ~OB_DUPLI;	
			}
		}
		base= base->next;
	}
	
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWOOPS, 0);
}

Object *find_camera()
{
	Base *base;
	
	base= FIRSTBASE;
	while(base) {
		if(base->object->type==OB_CAMERA) return base->object;
		base= base->next;
	}
	return 0;
}



void apply_object()
{
	Base *base, *basact;
	Object *ob;
	Mesh *me;
	Curve *cu;
	Nurb *nu;
	BPoint *bp;
	BezTriple *bezt;
	MVert *mvert;
	float mat[3][3];
	int a;

	if(G.scene->id.lib) return;
	if(G.obedit) return;
	basact= BASACT;
	
	if(G.qual & LR_SHIFTKEY) {
		ob= OBACT;
		if(ob==0) return;
		
		if(ob->transflag & OB_DUPLI) make_duplilist_real();
		else if(ob->parent && ob->parent->type==OB_LATTICE) apply_lattice();
		
		return;
	}

	if(okee("Apply size/rot")==0) return;

	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			ob= base->object;
			
			if(ob->type==OB_MESH) {
				object_to_mat3(ob, mat);
				me= ob->data;
				
				if(me->id.us>1) {
					error("Can't do multi user mesh");
					return;
				}
				if(me->key) {
					error("Can't do key && mesh");
					return;
				}
				
				mvert= me->mvert;
				for(a=0; a<me->totvert; a++, mvert++) {
					Mat3MulVecfl(mat, mvert->co);
				}
				ob->size[0]= ob->size[1]= ob->size[2]= 1.0;
				ob->rot[0]= ob->rot[1]= ob->rot[2]= 0.0;
				
				where_is_object(ob);
				
				/* texspace en normalen */
				BASACT= base;
				enter_editmode();
				exit_editmode(1);
				BASACT= basact;				
				
			}
			else if ELEM(ob->type, OB_CURVE, OB_SURF) {
				object_to_mat3(ob, mat);
				cu= ob->data;
				
				if(cu->id.us>1) {
					error("Can't do multi user curve");
					return;
				}
				if(cu->key) {
					error("Can't do keys");
					return;
				}
				
				nu= cu->nurb.first;
				while(nu) {
					if( (nu->type & 7)==1) {
						a= nu->pntsu;
						bezt= nu->bezt;
						while(a--) {
							Mat3MulVecfl(mat, bezt->vec[0]);
							Mat3MulVecfl(mat, bezt->vec[1]);
							Mat3MulVecfl(mat, bezt->vec[2]);
							bezt++;
						}
					}
					else {
						a= nu->pntsu*nu->pntsv;
						bp= nu->bp;
						while(a--) {
							Mat3MulVecfl(mat, bp->vec);
							bp++;
						}
					}
					nu= nu->next;
				}
			
				ob->size[0]= ob->size[1]= ob->size[2]= 1.0;
				ob->rot[0]= ob->rot[1]= ob->rot[2]= 0.0;
				
				where_is_object(ob);
				
				/* texspace en normalen */
				BASACT= base;
				enter_editmode();
				exit_editmode(1);
				BASACT= basact;
			}
		}
		base= base->next;
	}
	
	allqueue(REDRAWVIEW3D, 0);
}



/* ************ ALGEMENE  *************** */

static Object *startob;

void start_is_a_parent_selected(Object *ob)
{
	Base *base;
	
	base= FIRSTBASE;
	while(base) {
		base->object->flag &= ~OB_DONE;
		base= base->next;
	}

	startob= ob;
}

Object *is_a_parent_selected(Object *ob)
{
	Object *par;
	
	if(ob->flag & OB_DONE) return 0;
	
	ob->flag |= OB_DONE;
	
	if(ob->parent) {
		if( TESTBASE(ob->parent) ) return ob->parent;
		else {
			par= is_a_parent_selected(ob->parent);
			if(par) return par;
		}
	}
	
	/* IK is more complex in parents... */
	
	if(ob->type==OB_IKA) {
		Ika *ika= ob->data;
		Deform *def= ika->def;
		
		if(def) {
			int a= ika->totdef;
			while(a--) {
				if(def->ob && ob!=def->ob && def->ob!=startob) {
					
					if( TESTBASE(def->ob) ) return def->ob;
					par= is_a_parent_selected(def->ob);
					if(par) return par;
				}
				def++;
			}
		}
		if(ika->parent) {
			if(TESTBASE(ika->parent) ) return ika->parent;
			else return( is_a_parent_selected(ika->parent) );
		}
	}
	
	return 0;
}

void setbaseflags_for_editing(int mode)	/* 0,'g','r','s' */
{
	/*
		als base selected en heeft parent selected:
			base->flag= BA_WASSEL+BA_PARSEL
		als base niet selected en parent selected:
			base->flag= BA_PARSEL
				
	*/
	Base *base, *tbase;
	Object *ob, *tr, *par, *parsel;
	
	copy_baseflags();

	base= FIRSTBASE;
	while(base) {
		base->flag &= ~(BA_PARSEL+BA_WASSEL);
			
		if( (base->lay & G.vd->lay) && base->object->id.lib==0) {
		
			ob= base->object;
			
			start_is_a_parent_selected(ob);
			parsel= is_a_parent_selected(ob);
				/* happens with skeletons... */
			if(parsel==ob) parsel= 0;
			
			/* hier ook parentkey? */

			if(parsel) {
				
				if(base->flag & SELECT) {
					base->flag &= ~SELECT;
					base->flag |= (BA_PARSEL+BA_WASSEL);
				}
				else base->flag |= BA_PARSEL;
			}

			if(mode=='g')  {
				tr= ob->track;
				if(tr && TESTBASE(tr) && (base->flag & SELECT)==0)  
					base->flag |= BA_PARSEL;
			}
			
			/* updates? */
				/* ivm automatische portals */
			if(ob->type==OB_SECTOR) base->flag |= BA_WHERE_UPDATE;
			if(ob->type==OB_IKA) {
				Ika *ika= ob->data;
				if(ika->parent && parsel) base->flag |= BA_WHERE_UPDATE;
			}
			
			if(base->flag & (SELECT | BA_PARSEL)) {
				
				base->flag |= BA_WHERE_UPDATE;
				
				if(ob->parent) {
					if(ob->parent->type==OB_LATTICE) base->flag |= BA_DISP_UPDATE;
					if(ob->parent->type==OB_IKA && ob->partype==PARSKEL) base->flag |= BA_DISP_UPDATE;
				}
				if(ob->track) {
					;
				}
				
				if( give_parteff(ob) ) base->flag |= BA_DISP_UPDATE;
				
				if(ob->type==OB_MBALL) {
					
					tr= find_basis_mball(ob);
					tbase= FIRSTBASE;
					while(tbase) {
						if(tbase->object==tr) break;
						tbase= tbase->next;
					}
					if(tbase) {
						tbase->flag |= BA_DISP_UPDATE;
					}
				}
			}
		}
		base= base->next;
	}
}


void clearbaseflags_for_editing()
{
	Base *base;

	base= FIRSTBASE;
	while(base) {
		if(base->flag & BA_WASSEL) base->flag |= SELECT;
		base->flag &= ~(BA_PARSEL+BA_WASSEL);
		
		base->flag &= ~(BA_DISP_UPDATE+BA_WHERE_UPDATE+BA_DO_IPO);
		
		base= base->next;
	}
	copy_baseflags();
}

void ob_to_transob(Object *ob, TransOb *tob)
{
	float totmat[3][3];
	
	tob->ob= ob;
	
	if(ob->track) {
		Object *tr;
		
		tr= ob->track;
		ob->track= 0;
		where_is_object(ob);
		ob->track= tr;
	}
	
	
	tob->loc= ob->loc;
	VECCOPY(tob->oldloc, tob->loc);
	
	tob->rot= ob->rot;
	VECCOPY(tob->oldrot, tob->rot);
	
	VECCOPY(tob->olddrot, ob->drot);
	
	tob->size= ob->size;
	VECCOPY(tob->oldsize, tob->size);

	VECCOPY(tob->olddsize, ob->dsize);

	/* alleen object, geen parent */
	object_to_mat3(ob, tob->obmat);
	Mat3Inv(tob->obinv, tob->obmat);
	
	Mat3CpyMat4(totmat, ob->obmat);

	/* dit is totmat zonder obmat: dus parmat */
	Mat3MulMat3(tob->parmat, totmat, tob->obinv);
	Mat3Inv(tob->parinv, tob->parmat);

	VECCOPY(tob->obvec, ob->obmat[3]);

	centroid[0]+= tob->obvec[0];
	centroid[1]+= tob->obvec[1];
	centroid[2]+= tob->obvec[2];

	tob->eff= 0;

	if(ob->type==OB_IKA) {
		Ika *ika=ob->data;
		
		calc_ika(ika, 0);
		
		ika->effn[0]= ika->eff[0];
		ika->effn[1]= ika->eff[1];
		ika->effn[2]= 0.0;	
		
		VecMat4MulVecfl(ika->effg, ob->obmat, ika->effn);
		
		if(ika->flag & IK_GRABEFF) {

			tob->eff= ika->effg;
			VECCOPY(tob->oldeff, tob->eff);
			tob->flag |= TOB_IKA;

			/* zodat alleen eff update wordt */
			tob->loc= 0;
		}
		
		/* set_ika_undo_vals(); */
	}
}

void ob_to_tex_transob(Object *ob, TransOb *tob)
{
	Mesh *me;
	Curve *cu;
	MetaBall *mb;
	ID *id;
	
	ob_to_transob(ob, tob);
	
	id= ob->data;
	if(id==0);
	else if( GS(id->name)==ID_ME) {
		me= ob->data;
		me->texflag &= ~AUTOSPACE;
		tob->loc= me->loc;
		tob->rot= me->rot;
		tob->size= me->size;
	}
	else if( GS(id->name)==ID_CU) {
		cu= ob->data;
		cu->texflag &= ~AUTOSPACE;
		tob->loc= cu->loc;
		tob->rot= cu->rot;
		tob->size= cu->size;
	}
	else if( GS(id->name)==ID_MB) {
		mb= ob->data;
		mb->texflag &= ~AUTOSPACE;
		tob->loc= mb->loc;
		tob->rot= mb->rot;
		tob->size= mb->size;
	}
	
	VECCOPY(tob->oldloc, tob->loc);
	VECCOPY(tob->oldrot, tob->rot);
	VECCOPY(tob->oldsize, tob->size);
}

void make_trans_objects()
{
	Base *base;
	Object *ob;
	TransOb *tob;
	ListBase elems;
	IpoKey *ik;
	float cfraont, min[3], max[3];
	int ipoflag, a;
	
	tottrans= 0;
	
	INIT_MINMAX(min, max);
	centroid[0]=centroid[1]=centroid[2]= 0.0;
	
	/* aantal tellen */	
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			ob= base->object;
			
			if(transmode==TRANS_TEX) {
				if(ob->dtx & OB_TEXSPACE) tottrans++;
			}
			else {
				if(ob->ipo && ob->ipo->showkey && (ob->ipoflag & OB_DRAWKEY)) {
					elems.first= elems.last= 0;
					make_ipokey_transform(ob, &elems, 1); /* '1' alleen selected keys */
					
					pushdata(&elems, sizeof(ListBase));
					
					ik= elems.first;
					while(ik) {
						tottrans++;
						ik= ik->next;
					}
					if(elems.first==0) tottrans++;
				}
				else tottrans++;
			}
		}
		base= base->next;
	}

	if(tottrans) tob= transmain= mallocN(tottrans*sizeof(TransOb), "transmain");
	
	reset_slowparents();


	/* dit hieronder wel doen als tottrans==0, i.v.m. vrijgeven pushpop en ipokeys */
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			ob= base->object;
			
			if(transmode==TRANS_TEX) {
				if(ob->dtx & OB_TEXSPACE) {
					tob->flag= 0;
					
					ob_to_tex_transob(ob, tob);
					DO_MINMAX(tob->obvec, min, max);
					
					tob++;					
				}
			}
			else {

				/* van belang! (o.a. bevobj) */
				if(base->flag & SELECT) ob->flag|= SELECT; else ob->flag &= ~SELECT;
	
				if(ob->ipo && ob->ipo->showkey && (ob->ipoflag & OB_DRAWKEY)) {
	
					popfirst(&elems);
	
					if(elems.first) {
						base->flag |= BA_DO_IPO+BA_WASSEL;
						base->flag &= ~SELECT;
					
						cfraont= CFRA;
						set_no_parent_ipo(1);
						ipoflag= ob->ipoflag;
						ob->ipoflag &= ~OB_OFFS_OB;
						
						pushdata(ob->loc, 7*3*4);
						
						ik= elems.first;
						while(ik) {
		
							CFRA= ik->val/G.scene->r.framelen;
							
							do_ob_ipo(ob);
							where_is_object(ob);
							
							ob_to_transob(ob, tob);
							DO_MINMAX(tob->obvec, min, max);
							
							/* doet ook tob->flag en oldvals, moet NA ob_to_transob()! */
							set_ipo_pointers_transob(ik, tob);
							
							tob++;
							ik= ik->next;
						}
						free_ipokey(&elems);
						
						poplast(ob->loc);
						set_no_parent_ipo(0);
						
						CFRA= cfraont;
						ob->ipoflag= ipoflag;
					}
					else {
						tob->flag= 0;
					
						ob_to_transob(ob, tob);
						DO_MINMAX(tob->obvec, min, max);
						
						tob++;
					}
				}
				else {
					tob->flag= 0;
				
					ob_to_transob(ob, tob);
					DO_MINMAX(tob->obvec, min, max);
					
					tob++;
				}
			}
			
		}
		base= base->next;
	}
	
	pushpop_test();	/* alleen voor debug & zekerheid */
	
	if(tottrans==0) return;
	
	centroid[0]/= tottrans;
	centroid[1]/= tottrans;
	centroid[2]/= tottrans;
	
	centre[0]= (min[0]+max[0])/2.0;
	centre[1]= (min[1]+max[1])/2.0;
	centre[2]= (min[2]+max[2])/2.0;
}

/* mode: 1 = proportional */
void make_trans_verts(float *min, float *max, int mode)	
{
	extern Lattice *editLatt;
	Nurb *nu;
	BezTriple *bezt;
	BPoint *bp;
	TransVert *tv;
	MetaElem *ml;
	EditVert *eve;
	int a;

	tottrans= 0;
	
	INIT_MINMAX(min, max);
	centroid[0]=centroid[1]=centroid[2]= 0.0;
	
	countall();
	if(mode) tottrans= G.totvert;
	else tottrans= G.totvertsel;
	
	if(G.totvertsel==0) {
		tottrans= 0;
		return;
	}
	
	tv=transvmain= callocN(tottrans*sizeof(TransVert), "maketransverts");
	
	/* we count again because of hide */
	tottrans= 0;
	
	if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) {
		eve= G.edve.first;
		while(eve) {
			if(eve->h==0) {
				if(mode==1 || (eve->f & 1)) {
					VECCOPY(tv->oldloc, eve->co);
					tv->loc= eve->co;
					tv->flag= eve->f & 1;
					tv++;
					tottrans++;
				}
			}
			eve= eve->next;
		}
	}
	else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) {
		nu= editNurb.first;
		while(nu) {
			if((nu->type & 7)==CU_BEZIER) {
				a= nu->pntsu;
				bezt= nu->bezt;
				while(a--) {
					if(bezt->hide==0) {
						if(mode==1 || (bezt->f1 & 1)) {
							VECCOPY(tv->oldloc, bezt->vec[0]);
							tv->loc= bezt->vec[0];
							tv->flag= bezt->f1 & 1;
							tv++;
							tottrans++;
						}
						if(mode==1 || (bezt->f2 & 1)) {
							VECCOPY(tv->oldloc, bezt->vec[1]);
							tv->loc= bezt->vec[1];
							tv->val= &(bezt->alfa);
							tv->oldval= bezt->alfa;
							tv->flag= bezt->f2 & 1;
							tv++;
							tottrans++;
						}
						if(mode==1 || (bezt->f3 & 1)) {
							VECCOPY(tv->oldloc, bezt->vec[2]);
							tv->loc= bezt->vec[2];
							tv->flag= bezt->f3 & 1;
							tv++;
							tottrans++;
						}
					}
					bezt++;
				}
			}
			else {
				a= nu->pntsu*nu->pntsv;
				bp= nu->bp;
				while(a--) {
					if(bp->hide==0) {
						if(mode==1 || (bp->f1 & 1)) {
							VECCOPY(tv->oldloc, bp->vec);
							tv->loc= bp->vec;
							tv->val= &(bp->alfa);
							tv->oldval= bp->alfa;
							tv->flag= bp->f1 & 1;
							tv++;
							tottrans++;
						}
					}
					bp++;
				}
			}
			nu= nu->next;
		}
	}
	else if(G.obedit->type==OB_MBALL) {
		ml= editelems.first;
		while(ml) {
			if(ml->flag & SELECT) {
				tv->loc= &ml->x;
				VECCOPY(tv->oldloc, tv->loc);
				tv->val= &(ml->rad);
				tv->oldval= ml->rad;
				tv->flag= 1;
				tv++;
				tottrans++;
			}
			ml= ml->next;
		}
	}
	else if(G.obedit->type==OB_LATTICE) {
		bp= editLatt->def;
		
		a= editLatt->pntsu*editLatt->pntsv*editLatt->pntsw;
		
		while(a--) {
			if(mode==1 || (bp->f1 & 1)) {
				if(bp->hide==0) {
					VECCOPY(tv->oldloc, bp->vec);
					tv->loc= bp->vec;
					tv->flag= bp->f1 & 1;
					tv++;
					tottrans++;
				}
			}
			bp++;
		}
	}
	
	/* cent enz berekenen */
	tv= transvmain;
	for(a=0; a<tottrans; a++, tv++) {
		if(tv->flag) {
			centroid[0]+= tv->oldloc[0];
			centroid[1]+= tv->oldloc[1];
			centroid[2]+= tv->oldloc[2];

			DO_MINMAX(tv->oldloc, min, max);
		}
	}
	centroid[0]/= G.totvertsel;
	centroid[1]/= G.totvertsel;
	centroid[2]/= G.totvertsel;

	centre[0]= (min[0]+max[0])/2.0;
	centre[1]= (min[1]+max[1])/2.0;
	centre[2]= (min[2]+max[2])/2.0;
	
}

void draw_prop_circle()
{
	float tmat[4][4], imat[4][4];
	
	if(G.moving) {
		setlinestyle(1);
		cpack(0x303030);
		getmatrix(tmat);
		Mat4Invert(imat, tmat);
		setlinestyle(2);
		drawcircball(prop_cent, prop_size, imat);
		loadmatrix(G.vd->viewmat);
		setlinestyle(0);
	}
}

void set_proportional_weight(TransVert *tv, float *min, float *max)
{
	float dist, xdist, ydist, zdist;
	
	if(tv->oldloc[0]<min[0]) xdist= tv->oldloc[0]-min[0];
	else if(tv->oldloc[0]>max[0]) xdist= tv->oldloc[0]-max[0];
	else xdist= 0.0;

	if(tv->oldloc[1]<min[1]) ydist= tv->oldloc[1]-min[1];
	else if(tv->oldloc[1]>max[1]) ydist= tv->oldloc[1]-max[1];
	else ydist= 0.0;

	if(tv->oldloc[2]<min[2]) zdist= tv->oldloc[2]-min[2];
	else if(tv->oldloc[2]>max[2]) zdist= tv->oldloc[2]-max[2];
	else zdist= 0.0;
	
	dist= fsqrt(xdist*xdist + ydist*ydist + zdist*zdist);
	if(dist==0.0) tv->fac= 1.0;
	else if(dist > prop_size) tv->fac= 0.0;
	else {
		dist= (prop_size-dist)/prop_size;
		if(prop_mode==1) tv->fac= 3.0*dist*dist - 2.0*dist*dist*dist;
		else tv->fac= dist*dist;
	}
}

void special_trans_update()
{
	extern Lattice *editLatt;
	Base *base;
	Curve *cu;
	IpoCurve *icu;
	
	if(G.obedit) {
		if(G.obedit->type==OB_SECTOR) {
			recalc_editnormals();
		}
		else if(G.obedit->type==OB_CURVE) {
			cu= G.obedit->data;
			if(cu->flag & CU_3D) makeBevelList(G.obedit);
			
			calc_curvepath(G.obedit);
		}
		else if(G.obedit->type==OB_LATTICE) {
			
			if(editLatt->flag & LT_OUTSIDE) outside_lattice(editLatt);
			
			base= FIRSTBASE;
			while(base) {
				if(base->lay & G.vd->lay) {
					if(base->object->parent==G.obedit) {
						makeDispList(base->object);
						/* object_deform(base->object); */
					}
				}
				base= base->next;
			}
		}
	}
	else {
		base= FIRSTBASE;
		while(base) {
			if(base->flag & BA_DO_IPO) {
				
				base->object->ctime= -1234567.0;
				
				icu= base->object->ipo->curve.first;
				while(icu) {
					calchandles_ipocurve(icu);
					icu= icu->next;
				}
				
			}
			if(base->object->partype & PARSLOW) {
				base->object->partype -= PARSLOW;
				where_is_object(base->object);
				base->object->partype |= PARSLOW;
			}
			else if(base->flag & BA_WHERE_UPDATE) {
				where_is_object(base->object);
				if(base->object->type==OB_IKA) {
					itterate_ika(base->object);
				}
			}

			base= base->next;
		}
				
		base= FIRSTBASE;
		while(base) {
			
			if(base->flag & BA_DISP_UPDATE) makeDispList(base->object);
			
			base= base->next;
		}
		
		sector_update(1);
	}
	
	if(G.vd->drawtype == OB_SHADED) reshadeall_displist();
	
}


void special_aftertrans_update(char mode, int flip)
{
	TransOb *tob;
	Object *ob;
	Base *base;
	MetaBall *mb;
	Curve *cu;
	Ika *ika;
	int doit, a, redrawipo=0;

	
	/* displaylisten e.d. */
	
	if(G.obedit) {
		if(G.obedit->type==OB_MBALL) {
			mb= G.obedit->data;
			if(mb->flag != MB_UPDATE_ALWAYS) makeDispList(G.obedit);
		}
		else if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) {
			if(flip) flip_editnormals();

			recalc_editnormals();
		}
	}
	else {
		base= FIRSTBASE;
		while(base) {	

			ob= base->object;
			
			if(base->flag & BA_WHERE_UPDATE) {
				
				where_is_object(ob);
				if(ob->type==OB_IKA) {
					ika= ob->data;
					
					/* vooral voor ika NIET in GRABEFF mode, updaten van globale effector */
					VecMat4MulVecfl(ika->effg, ob->obmat, ika->eff);

					itterate_ika(ob);
				}
			}
			if(base->flag & BA_DISP_UPDATE) {
				if(ob->type==OB_MBALL) {
					mb= ob->data;
					if(mb->flag != MB_UPDATE_ALWAYS) makeDispList(ob);
				}
				if( give_parteff(ob) ) build_particle_system(ob);
			}
			if(base->flag & BA_DO_IPO) redrawipo= 1;
			
			if(mode=='s' && ob->type==OB_FONT) {
				doit= 0;
				cu= ob->data;
				
				if(cu->bevobj && (cu->bevobj->flag & SELECT) ) doit= 1;
				else if(cu->textoncurve) {
					if(cu->textoncurve->flag & SELECT) doit= 1;
					else if(ob->flag & SELECT) doit= 1;
				}
				
				if(doit) {
					text_to_curve(ob, 0);
					makeDispList(ob);
				}
			}
			if(mode=='s' && ob->type==OB_CURVE) {
				doit= 0;
				cu= ob->data;
				
				if(cu->bevobj && (cu->bevobj->flag & SELECT) ) 
					makeDispList(ob);
			}
			/* tijdelijk of altijd? */
			if ELEM(ob->type, OB_LIFE, OB_SECTOR) {
				if(ob->ipo==0) {
					ob->size[0]= ob->size[1]= ob->size[2]= 1.0;
				}
			}
			
			where_is_object(ob);	/* altijd ivm track eytc */
			
			base= base->next;
		}
		
		sector_update(0);
	}

	if(redrawipo) allqueue(REDRAWIPO, 0);

	if(G.vd->drawtype == OB_SHADED) reshadeall_displist();
}



void calc_trans_verts()
{
	Nurb *nu;
	Mesh *me;
	
	if(G.obedit->type==OB_MESH) {
		me= G.obedit->data;
		if(me->flag & ME_SMESH) make_s_editmesh(G.obedit);
	}
	else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) {
		nu= editNurb.first;
		while(nu) {
			test2DNurb(nu);
			testhandlesNurb(nu); /* test ook op bezier */
			nu= nu->next;
		}
		makeDispList(G.obedit);
	}
	else if(G.obedit->type==OB_MBALL) makeDispList(G.obedit);
}


int test_midtog_proj(xn, yn, mval)
short xn, yn, *mval;
{
	float x,y,z;

	/* welke beweging is het grootst? die wordt het */
	xn= (xn-mval[0]);
	yn= (yn-mval[1]);
	x = fabs(G.vd->persinv[0][0]*xn + G.vd->persinv[1][0]*yn);
	y = fabs(G.vd->persinv[0][1]*xn + G.vd->persinv[1][1]*yn);
	z = fabs(G.vd->persinv[0][2]*xn + G.vd->persinv[1][2]*yn);

	if(x>=y && x>=z) return 0;
	else if(y>=x && y>=z) return 1;
	else return 2;
}

void apply_keyb_grid(float *val, float fac1, float fac2, float fac3, int invert)
{
	/* fac1 is voor 'niets', fac2 voor CTRL fac3 voor SHIFT */
	int ctrl;

	if(invert) {
		if(G.qual & LR_CTRLKEY) ctrl= 0;
		else ctrl= 1;
	}
	else ctrl= (G.qual & LR_CTRLKEY);

	if(ctrl && (G.qual & LR_SHIFTKEY)) {
		if(fac3!= 0.0) *val= fac3*ffloor(*val/fac3 +.5);
	}
	else if(ctrl) {
		if(fac2!= 0.0) *val= fac2*ffloor(*val/fac2 +.5);
	}
	else {
		if(fac1!= 0.0) *val= fac1*ffloor(*val/fac1 +.5);
	}
	
}


void compatible_eul(float *eul, float *oldrot)
{
	float dx, dy, dz;
	
	/* verschillen van ong 360 graden eerst corrigeren */

	dx= eul[0] - oldrot[0];
	dy= eul[1] - oldrot[1];
	dz= eul[2] - oldrot[2];

/* printf("komt binnen: \n"); */
/* PRINT3(f, f, f, eul[0], eul[1], eul[2]); */
/* PRINT3(f, f, f, dx, dy, dz); */

	while( fabs(dx) > 5.1) {
		if(dx > 0.0) eul[0] -= 2.0*M_PI; else eul[0]+= 2.0*M_PI;
		dx= eul[0] - oldrot[0];
	}
	while( fabs(dy) > 5.1) {
		if(dy > 0.0) eul[1] -= 2.0*M_PI; else eul[1]+= 2.0*M_PI;
		dy= eul[1] - oldrot[1];
	}
	while( fabs(dz) > 5.1 ) {
		if(dz > 0.0) eul[2] -= 2.0*M_PI; else eul[2]+= 2.0*M_PI;
		dz= eul[2] - oldrot[2];
	}
	
/* PRINT3(f, f, f, oldrot[0], oldrot[1], oldrot[2]); */
	

	/* is 1 van de asrotaties groter dan 180 graden en de andere klein? GEEN ELSEIF!! */	
	if( fabs(dx) > 3.2 && fabs(dy)<1.6 && fabs(dz)<1.6 ) {
		if(dx > 0.0) eul[0] -= 2.0*M_PI; else eul[0]+= 2.0*M_PI;
	}
	if( fabs(dy) > 3.2 && fabs(dz)<1.6 && fabs(dx)<1.6 ) {
		if(dy > 0.0) eul[1] -= 2.0*M_PI; else eul[1]+= 2.0*M_PI;
	}
	if( fabs(dz) > 3.2 && fabs(dx)<1.6 && fabs(dy)<1.6 ) {
		if(dz > 0.0) eul[2] -= 2.0*M_PI; else eul[2]+= 2.0*M_PI;
	}
	
return;
	/* opnieuw berekenen */
	dx= eul[0] - oldrot[0];
	dy= eul[1] - oldrot[1];
	dz= eul[2] - oldrot[2];

	/* dit is een bijzonder geval, voor x-z getest */
	
	if( (fabs(dx) > 3.1 && fabs(dz) > 1.5 ) || ( fabs(dx) > 1.5 && fabs(dz) > 3.1 ) ) {
		if(dx > 0.0) eul[0] -= M_PI; else eul[0]+= M_PI;
		if(eul[1] > 0.0) eul[1]= M_PI - eul[1]; else eul[1]= -M_PI - eul[1];
		if(dz > 0.0) eul[2] -= M_PI; else eul[2]+= M_PI;
		
	}
	else if( (fabs(dx) > 3.1 && fabs(dy) > 1.5 ) || ( fabs(dx) > 1.5 && fabs(dy) > 3.1 ) ) {
		if(dx > 0.0) eul[0] -= M_PI; else eul[0]+= M_PI;
		if(dy > 0.0) eul[1] -= M_PI; else eul[1]+= M_PI;
		if(eul[2] > 0.0) eul[2]= M_PI - eul[2]; else eul[2]= -M_PI - eul[2];
	}
	else if( (fabs(dy) > 3.1 && fabs(dz) > 1.5 ) || ( fabs(dy) > 1.5 && fabs(dz) > 3.1 ) ) {
		if(eul[0] > 0.0) eul[0]= M_PI - eul[0]; else eul[0]= -M_PI - eul[0];
		if(dy > 0.0) eul[1] -= M_PI; else eul[1]+= M_PI;
		if(dz > 0.0) eul[2] -= M_PI; else eul[2]+= M_PI;
	}

/* PRINT3(f, f, f, eul[0], eul[1], eul[2]); */
/* printf("\n"); */
}

void headerprint(char *str)
{
	areawinset(curarea->headwin);
	
	headerbox(0xA09090, curarea->winx);
	cpack(0x0);
	glRasterPos2i(20+curarea->headbutofs,  6);
	fmsetfont(G.font);
	fmprstr(str);
	
	curarea->head_swap= WIN_BACK_OK;
	areawinset(curarea->win);
}

void add_ipo_tob_poin(float *poin, float *old, float delta)
{
	if(poin) {
		poin[0]= old[0]+delta;
		poin[-3]= old[3]+delta;
		poin[3]= old[6]+delta;
	}
}

void restore_tob(TransOb *tob)
{

	if(tob->flag & TOB_IPO) {
		add_ipo_tob_poin(tob->locx, tob->oldloc, 0.0);
		add_ipo_tob_poin(tob->locy, tob->oldloc+1, 0.0);
		add_ipo_tob_poin(tob->locz, tob->oldloc+2, 0.0);

		add_ipo_tob_poin(tob->rotx, tob->oldrot+3, 0.0);
		add_ipo_tob_poin(tob->roty, tob->oldrot+4, 0.0);
		add_ipo_tob_poin(tob->rotz, tob->oldrot+5, 0.0);

		add_ipo_tob_poin(tob->sizex, tob->oldsize, 0.0);
		add_ipo_tob_poin(tob->sizey, tob->oldsize+1, 0.0);
		add_ipo_tob_poin(tob->sizez, tob->oldsize+2, 0.0);

	}
	else {
		if(tob->eff) VECCOPY(tob->eff, tob->oldeff);
		if(tob->loc) VECCOPY(tob->loc, tob->oldloc);
		VECCOPY(tob->rot, tob->oldrot);
		VECCOPY(tob->size, tob->oldsize);
	}
}

int cylinder_intersect_test()
{
	extern float editbutsize;
	float *oldloc, speed[3], speedn[3], s, t, labda, labdacor, dist, len, len1, len2, axis[3], *base, rc[3], n[3], o[3];
	EditVert *v1;
	
	v1= G.edve.first;

	base= v1->co;
	v1= v1->next;
	VecSubf(axis, v1->co, base);
	
	v1= v1->next;
	oldloc= v1->co;
	v1= v1->next;
	VecSubf(speed, v1->co, oldloc);
	
	VecSubf(rc, oldloc, base);
	
	/* als we nou speed normaliseren (kan van te voren! */
	VECCOPY(speedn, speed);
	len1= Normalise(speedn);
	/* en de axis ook alvast */
	len2= Normalise(axis);
	
	Crossf(n, speed, axis);
	len= Normalise(n);
	if(len==0.0) return 0;
	
	dist= fabs( rc[0]*n[0] + rc[1]*n[1] + rc[2]*n[2] );
	
	if( dist>=editbutsize ) return 0;
	
	Crossf(o, rc, axis);
	t= -(o[0]*n[0] + o[1]*n[1] + o[2]*n[2])/len;
	
	Crossf(o, n, axis);
	s=  fabs(fsqrt(editbutsize*editbutsize-dist*dist) / (o[0]*speed[0] + o[1]*speed[1] + o[2]*speed[2]));
	
	labdacor= t-s;
	labda= t+s;

	/* twee gevallen waarbij geen snijpunt is */
	if(labdacor>=1.0 && labda>=1.0) return 0;
	if(labdacor<=0.0 && labda<=0.0) return 0;
	
	/* normaalvector berekenen */
	/* snijpunt: */
	
	rc[0]= oldloc[0] + labdacor*speed[0] - base[0];
	rc[1]= oldloc[1] + labdacor*speed[1] - base[1];
	rc[2]= oldloc[2] + labdacor*speed[2] - base[2];
	
	s= (rc[0]*axis[0] + rc[1]*axis[1] + rc[2]*axis[2]) ;
	
	if(s<0.0 || s>len2) return 0;
	
	n[0]= (rc[0] - s*axis[0]);
	n[1]= (rc[1] - s*axis[1]);
	n[2]= (rc[2] - s*axis[2]);

	PRINT3(f, f, f, labdacor, len2, s);	
	PRINT3(f, f, f, rc[0], rc[1], rc[2]);	
	PRINT3(f, f, f, n[0], n[1], n[2]);	

	return 1;
}

int sphere_intersect_test()
{
	extern float editbutsize;
	float *oldloc, speed[3], labda, labdacor, len, bsq, u, disc, *base, rc[3], n[3];
	EditVert *v1;
	
	v1= G.edve.first;
	base= v1->co;
	
	v1= v1->next;
	oldloc= v1->co;
	
	v1= v1->next;
	VecSubf(speed, v1->co, oldloc);
	len= Normalise(speed);
	if(len==0.0) return 0;
	
	VecSubf(rc, oldloc, base);
	bsq= rc[0]*speed[0] + rc[1]*speed[1] + rc[2]*speed[2]; 
	u= rc[0]*rc[0] + rc[1]*rc[1] + rc[2]*rc[2] - editbutsize*editbutsize;

	disc= bsq*bsq - u;
	
	if(disc>=0.0) {
		disc= fsqrt(disc);
		labdacor= (-bsq - disc)/len;	/* intrede */
		labda= (-bsq + disc)/len;
		
		PRINT3(f, f, f, labdacor, labda, editbutsize);
	}
	else return 0;

	/* snijpunt en normaal */
	rc[0]= oldloc[0] + labdacor*speed[0] - base[0];
	rc[1]= oldloc[1] + labdacor*speed[1] - base[1];
	rc[2]= oldloc[2] + labdacor*speed[2] - base[2];


	return 1;
}

int my_clock()
{
	float ftime;
	
	ftime= (float)clock();
	ftime*= 100.0/CLOCKS_PER_SEC;
	
	return (int)ftime;
}

#define XROT 1
#define YROT 2
#define ZROT 3

void transform(int mode)	/* 'g' 'G' 'r' 'R' 's' 'S' 't' or 'w' */
{
	TransOb *tob;
	TransVert *tv;
	float vec[3], min[3], max[3], dvec[3], d_dvec[3], dvecp[3], rot0[3], rot1[3], rot2[3], axis[3];
	float totmat[3][3], omat[3][3], imat[3][3], mat[3][3], tmat[3][3], phi, dphi;
	float persinv[3][3], persmat[3][3], viewinv[4][4], imat4[4][4];
	float *curs, dx1, dx2, dy1, dy2, eul[3], rot[3], phi0, phi1, deler, rad;
	float sizefac, size[3], sizelo[3], smat[3][3], xref=1.0, yref=1.0, zref= 1.0;
	float si, co, dist, startomtrekfac, omtrekfac, oldval[3];
	int rotmode=0, time, fast=0, a, midtog=0, firsttime=1, proj= 0, fout= 0, cameragrab= 0, gridflag;
	ushort event=0;
	short mval[2], mvalmidtog[2], afbreek=0, doit, xn, yn, xc, yc, xo, yo, val;
	char str[100];
	
	if(G.obedit && (G.f & G_PROPORTIONAL)) {
		if(mode=='g') mode= 'G';
		if(mode=='r') mode= 'R';
		if(mode=='s') mode= 'C';
	}
	/* form duplicate routines */
	if(mode=='d') mode= 'g';

	/* kan floating exception veroorzaken op alpha */
	d_dvec[0]= d_dvec[1]= d_dvec[2]= 0.0;
	dvec[0]= dvec[1]= dvec[2]= 0.0;
	
	if(G.scene->id.lib) return;
	
	if(mode=='t') {
		if(G.obedit==0 || G.obedit->type!=OB_CURVE) return;
	}
	if(mode=='w' && G.obedit==0) return;
	
	/* welke data wordt behandeld? */
	if(G.obedit) {
		/* min en max zijn nodig voor de warp */
		if(mode=='G' || mode=='R' || mode=='C') make_trans_verts(min, max, 1);
		else make_trans_verts(min, max, 0);
	}
	else {
		int opt= 0;
		if (mode=='g' || mode=='G') opt= 'g';
		else if (mode=='r' || mode=='R') opt= 'r';
		else if (mode=='s' || mode=='S') opt= 's';
		
		setbaseflags_for_editing(opt);
		
		make_trans_objects();
		
	}
	
	if(tottrans==0) {
		if(G.obedit==0) clearbaseflags_for_editing();
		return;
	}
	
	if(G.obedit==0 && mode=='S') return;
	
	if(G.vd->around==V3D_LOCAL) {
		if(G.obedit) {
			centre[0]= centre[1]= centre[2]= 0.0;
		}
	}
	if(G.vd->around==V3D_CENTROID) {
		VECCOPY(centre, centroid);
	}
	else if(G.vd->around==V3D_CURSOR) {
		curs= give_cursor();
		VECCOPY(centre, curs);
		
		if(G.obedit) {
			VecSubf(centre, centre, G.obedit->obmat[3]);
			Mat3CpyMat4(mat, G.obedit->obmat);
			Mat3Inv(imat, mat);
			Mat3MulVecfl(imat, centre);
		}
	}
	
	/* moving: onderscheid i.v.m. drawobj */
	if(G.obedit) G.moving= 2;
	else G.moving= 1;
	
	areawinset(curarea->win);
	
	/* de persinv is vervuild met translatie, niet gebruiken!! */
	Mat3CpyMat4(persmat, G.vd->persmat);
	Mat3Inv(persinv, persmat);
	
	VECCOPY(rot0, persinv[0]);
	Normalise(rot0);
	VECCOPY(rot1, persinv[1]);
	Normalise(rot1);
	VECCOPY(rot2, persinv[2]);
	Normalise(rot2);

	/* init vars */

	Mat4Invert(viewinv, G.vd->viewmat);

	if(transvmain) {
		VECCOPY(vec, centre);
		Mat4MulVecfl(G.obedit->obmat, vec);
		initgrabz(vec[0], vec[1], vec[2]);
		project_short_noclip(vec, mval);
	}
	else {
		/* voor pannen vanuit cameraview */
		if( G.vd->camera==OBACT && G.vd->persp>1) {
			/* 6.0 = 6 grideenheden */
			centre[0]+= -6.0*rot2[0];
			centre[1]+= -6.0*rot2[1];
			centre[2]+= -6.0*rot2[2];
		}
		
		initgrabz(centre[0], centre[1], centre[2]);
		project_short_noclip(centre, mval);

		if( G.vd->camera==OBACT && G.vd->persp>1) {
			centre[0]+= 6.0*rot2[0];
			centre[1]+= 6.0*rot2[1];
			centre[2]+= 6.0*rot2[2];
		}
	}
	
	VECCOPY(prop_cent, centre);

	xc= mval[0];
	yc= mval[1];
	
	if(G.obedit) {
		Mat3CpyMat4(omat, G.obedit->obmat);
		Mat3Inv(imat, omat);
		
		Mat4Invert(imat4, G.obedit->obmat);
	}
	else {
		if(transmain) {
			if(OBACT &&  G.vd->persp>1 && G.vd->camera==OBACT) {
				cameragrab= 1;
				xc= curarea->winx/2;
				yc= curarea->winy/2;
			}
		}
	}
	
	if((mode=='r' || mode=='s' || mode=='S') && xc==32000) {
		error("centre far out of view");
		fout= 1;
	}

	if(mode=='w' && G.obedit) {
		Mat4MulVecfl(G.obedit->obmat, min);
		Mat4MulVecfl(G.vd->viewmat, min);
		Mat4MulVecfl(G.obedit->obmat, max);
		Mat4MulVecfl(G.vd->viewmat, max);
		
		centre[0]= (min[0]+max[0])/2.0;
		centre[1]= (min[1]+max[1])/2.0;
		centre[2]= (min[2]+max[2])/2.0;

			/* middelpunt is cursor */
		curs= give_cursor();
		VECCOPY(axis, curs);
		Mat4MulVecfl(G.vd->viewmat, axis);
		rad= fsqrt( (axis[0]-centre[0])*(axis[0]-centre[0])+(axis[1]-centre[1])*(axis[1]-centre[1]) );
		dist= max[0]-centre[0];
		if(dist==0.0) fout= 1;
		else startomtrekfac= (90*rad*M_PI)/(360.0*dist);
	}

	getmouseco_areawin(mval);
	xn=xo= mval[0];
	yn=xo= mval[1];
	dx1= xc-xn; 
	dy1= yc-yn;
	phi= phi0= phi1= 0.0;
	
	sizefac= fsqrt( (float)((yc-yn)*(yc-yn)+(xn-xc)*(xn-xc)) );
	if(sizefac<2.0) sizefac= 2.0;

	gridflag= U.flag;
	/* if(transmain) { */
	/* 	Object *ob= OBACT; */
	/* 	if(ob && ob->type==OB_SECTOR && (ob->flag & SELECT)) gridflag= AUTOGRABGRID+AUTOROTGRID+AUTOSIZEGRID; */
	/* } */
	
	while(fout==0 && afbreek==0) {
		
		getmouseco_areawin(mval);
		if(mval[0]!=xo || mval[1]!=yo || firsttime) {
			if(firsttime) {
				
				/* niet zo netjes, maar toch! */
				oldval[0]= oldval[1]= oldval[2]= MAXFLOAT;
				
				/* proportional precalc */
				if(mode=='G' || mode=='R' || mode=='C') {
					if(transvmain) {
						tv= transvmain;
						for(a=0; a<tottrans; a++, tv++) {
							set_proportional_weight(tv, min, max);
						}
					}
				}
			}
			firsttime= 0;
			
			if(mode=='g' || mode=='G') {
				
				/* if(G.edve.first) sphere_intersect_test(); */
				
				if(midtog) {
					if(cameragrab) {
						dx1= 0.002*(mval[1]-yn)*G.vd->grid;
						dvec[0]-= dx1*G.vd->viewinv[2][0];
						dvec[1]-= dx1*G.vd->viewinv[2][1];
						dvec[2]-= dx1*G.vd->viewinv[2][2];
						firsttime= 1;	/* blijftie lopen */
					}
					else {
						window_to_3d(dvec, mval[0]-xn, mval[1]-yn);
						if(proj==0) dvec[1]=dvec[2]= 0.0;
						if(proj==1) dvec[0]=dvec[2]= 0.0;
						if(proj==2) dvec[0]=dvec[1]= 0.0;
					}
				}
				else window_to_3d(dvec, mval[0]-xn, mval[1]-yn);

				/* grids */
				if(G.qual & LR_SHIFTKEY) {
					dvec[0]= 0.1*(dvec[0]-d_dvec[0])+d_dvec[0];
					dvec[1]= 0.1*(dvec[1]-d_dvec[1])+d_dvec[1];
					dvec[2]= 0.1*(dvec[2]-d_dvec[2])+d_dvec[2];
				}
				apply_keyb_grid(dvec, 0.0, G.vd->grid, 0.1*G.vd->grid, gridflag & AUTOGRABGRID);
				apply_keyb_grid(dvec+1, 0.0, G.vd->grid, 0.1*G.vd->grid, gridflag & AUTOGRABGRID);
				apply_keyb_grid(dvec+2, 0.0, G.vd->grid, 0.1*G.vd->grid, gridflag & AUTOGRABGRID);

				if(dvec[0]!=oldval[0] ||dvec[1]!=oldval[1] ||dvec[2]!=oldval[2]) {
					VECCOPY(oldval, dvec);
					
					/* speedup for vertices */
					if (G.obedit) {
						VECCOPY(dvecp, dvec);
						Mat3MulVecfl(imat, dvecp);
					}
					
					/* apply */
					tob= transmain;
					tv= transvmain;
					for(a=0; a<tottrans; a++, tob++, tv++) {
						
						if(transmain) {
							VECCOPY(dvecp, dvec);
							
							if(transmode==TRANS_TEX) Mat3MulVecfl(tob->obinv, dvecp);

							if(tob->flag & TOB_IKA) {
								VecAddf(tob->eff, tob->oldeff, dvecp);
							}
							else Mat3MulVecfl(tob->parinv, dvecp);
							
							if(tob->flag & TOB_IPO) {
								add_ipo_tob_poin(tob->locx, tob->oldloc, dvecp[0]);
								add_ipo_tob_poin(tob->locy, tob->oldloc+1, dvecp[1]);
								add_ipo_tob_poin(tob->locz, tob->oldloc+2, dvecp[2]);
							}
							else if(tob->loc) {
								VecAddf(tob->loc, tob->oldloc, dvecp);
							}
						}
						else {
							if(mode=='G') {
								tv->loc[0]= tv->oldloc[0]+tv->fac*dvecp[0];
								tv->loc[1]= tv->oldloc[1]+tv->fac*dvecp[1];
								tv->loc[2]= tv->oldloc[2]+tv->fac*dvecp[2];
							}
							else VecAddf(tv->loc, tv->oldloc, dvecp);
						}
					}
					sprintf(str, "Dx: %.4f   Dy: %.4f  Dz: %.4f", dvec[0], dvec[1], dvec[2]);
					headerprint(str);

					time= my_clock();

					if(G.obedit) calc_trans_verts();
					special_trans_update();
					
					if(fast==0) {
						force_draw();
						time= my_clock()-time;
						if(time>50) fast= 1;
					}
					else {
						curarea->windraw();
						screen_swapbuffers();
					}
				}
			}
			else if(mode=='r' || mode=='t' || mode=='R') {
				doit= 0;
				dx2= xc-mval[0];
				dy2= yc-mval[1];
				
				if(midtog && (mode=='r' || mode=='R')) {
					phi0+= .007*(float)(dy2-dy1);
					phi1+= .007*(float)(dx1-dx2);
				
					apply_keyb_grid(&phi0, 0.0, (5.0/180)*M_PI, (1.0/180)*M_PI, gridflag & AUTOROTGRID);
					apply_keyb_grid(&phi1, 0.0, (5.0/180)*M_PI, (1.0/180)*M_PI, gridflag & AUTOROTGRID);


					if(oldval[0]!=phi0 || oldval[1]!=phi1) {
						VecRotToMat3(rot0, phi0, smat);
						VecRotToMat3(rot1, phi1, totmat);
						Mat3MulMat3(mat, smat, totmat);
						dx1= dx2;
						dy1= dy2;
						oldval[0]= phi0;
						oldval[1]= phi1;
						doit= 1;
					}
				}
				else {
					deler= fsqrt( (dx1*dx1+dy1*dy1)*(dx2*dx2+dy2*dy2));
					if(deler>1.0) {
					
						dphi= (dx1*dx2+dy1*dy2)/deler;
						dphi= safacos(dphi);
						if( (dx1*dy2-dx2*dy1)>0.0 ) dphi= -dphi;
								
						if(G.qual & LR_SHIFTKEY) phi+= dphi/30.0;
						else phi+= dphi;
						
						apply_keyb_grid(&phi, 0.0, (5.0/180)*M_PI, (1.0/180)*M_PI, gridflag & AUTOROTGRID);
						
						if(oldval[2]!=phi) {
							dx1= dx2; 
							dy1= dy2;
							oldval[2]= phi;
							doit= 1;
							if(rotmode) {
								if(rotmode==XROT) vec[0]= -1.0; else vec[0]= 0.0;
								if(rotmode==YROT) vec[1]= 1.0; else vec[1]= 0.0;
								if(rotmode==ZROT) vec[2]= -1.0; else vec[2]= 0.0;
								VecRotToMat3(vec, phi, mat);
							}
							else VecRotToMat3(rot2, phi, mat);
						}
					}
				}
				if(doit) {
					/* apply */
					tob= transmain;
					tv= transvmain;
					
					for(a=0; a<tottrans; a++, tob++, tv++) {
						if(transmain) {
							/* rotatie in drie stappen:
							 * 1. editrot corrigeren voor parent
							 * 2. hier de euler uit destilleren. Deze stap moet omdat de MatToEul nogal zwak is
							 * 3. deze vermenigvuldigen met eigen rot, euler berekenen.
							 */
							Mat3MulSerie(smat, tob->parmat, mat, tob->parinv, 0);

								/* 2 */
							Mat3ToEul(smat, eul);
							EulToMat3(eul, smat);
							
								/* 3 */
								/* we werken even met de rot+drot */
							VecAddf(eul, tob->oldrot, tob->olddrot);
							EulToMat3(eul, tmat);
							
							Mat3MulMat3(totmat, smat, tmat);
							Mat3ToEul(totmat, eul);
							
							/* Eul mag niet te gek afwijken van oude eul.
							 * Dit is alleen nog maar getest voor dx && dz
							 */
							
							compatible_eul(eul, tob->oldrot);
							
							if(tob->flag & TOB_IPO) {
								
								if(tob->flag & TOB_IPODROT) {
									VecSubf(rot, eul, tob->oldrot);
								}
								else {
									VecSubf(rot, eul, tob->olddrot);
								}

								VecMulf(rot, 9.0/M_PI_2);
								VecSubf(rot, rot, tob->oldrot+3);
								
								add_ipo_tob_poin(tob->rotx, tob->oldrot+3, rot[0]);
								add_ipo_tob_poin(tob->roty, tob->oldrot+4, rot[1]);
								add_ipo_tob_poin(tob->rotz, tob->oldrot+5, rot[2]);

							}
							else {
							
								VecSubf(tob->rot, eul, tob->olddrot);
							}
							
							if(G.vd->around!=V3D_LOCAL) {
								/* translatie */
								VecSubf(vec, tob->obvec, centre);
								Mat3MulVecfl(mat, vec);
								VecAddf(vec, vec, centre);
								/* vec is nu de plek waar het object moet komen */
								VecSubf(vec, vec, tob->obvec);
								Mat3MulVecfl(tob->parinv, vec);

								if(tob->flag & TOB_IPO) {
									add_ipo_tob_poin(tob->locx, tob->oldloc, vec[0]);
									add_ipo_tob_poin(tob->locy, tob->oldloc+1, vec[1]);
									add_ipo_tob_poin(tob->locz, tob->oldloc+2, vec[2]);
								}
								else if(tob->loc) {
									VecAddf(tob->loc, tob->oldloc, vec);
								}
							}
						}
						else {
							if(mode=='t') {
								if(tv->val) *(tv->val)= tv->oldval-phi;
							}
							else {
							
								if(mode=='R') {

									if(midtog) {
										VecRotToMat3(rot0, tv->fac*phi0, smat);
										VecRotToMat3(rot1, tv->fac*phi1, totmat);
										Mat3MulMat3(mat, smat, totmat);
									}
									else VecRotToMat3(rot2, tv->fac*phi, mat);
									
								}

								Mat3MulMat3(totmat, mat, omat);
								Mat3MulMat3(smat, imat, totmat);
							
								VecSubf(vec, tv->oldloc, centre);
								Mat3MulVecfl(smat, vec);
								
								VecAddf(tv->loc, vec, centre);
							}
						}
					}
					
					if(midtog) sprintf(str, "Rotx: %.2f  Roty: %.2f", 180.0*phi0/M_PI, 180.0*phi1/M_PI);
					else if(rotmode) {
						if(rotmode==XROT) sprintf(str, "Rot X: %.2f", 180.0*phi/M_PI);
						else if(rotmode==YROT) sprintf(str, "Rot Y: %.2f", 180.0*phi/M_PI);
						else sprintf(str, "Rot Z: %.2f", 180.0*phi/M_PI);
					}
					else sprintf(str, "Rot: %.2f", 180.0*phi/M_PI);
					headerprint(str);
					
					time= my_clock();

					if(G.obedit) calc_trans_verts();
					special_trans_update();
					
					if(fast==0) {
						force_draw();
						time= my_clock()-time;
						if(time>50) fast= 1;
					}
					else {
						curarea->windraw();
						screen_swapbuffers();
					}
					if(tottrans>1 || G.vd->around==V3D_CURSOR) helpline(centre);
				}
			}
			else if(mode=='s' || mode=='S' || mode=='C') {
				
				if(mode=='S') {
					size[0]= 1.0-(float)(xn-mval[0])*0.005;
					size[1]= 1.0-(float)(yn-mval[1])*0.005;
					size[2]= 1.0;
				}
				else size[0]=size[1]=size[2]= (fsqrt( (float)((yc-mval[1])*(yc-mval[1])+(mval[0]-xc)*(mval[0]-xc)) ))/sizefac;
				
				if(midtog && mode=='s') {
					/* shear has no midtog */
					if(proj==0) size[1]=size[2]= 1.0;
					if(proj==1) size[0]=size[2]= 1.0;
					if(proj==2) size[1]=size[0]= 1.0;
				}

/* X en Y flip, twee methodes: bij  |**| commentaar weghalen maakt flips lokaal */

/**/			/* if(transvmain) { */
	
						/* x flip */
					val= test_midtog_proj(mval[0]+10, mval[1], mval);
					size[val]*= xref;
						/* y flip */
					val= test_midtog_proj(mval[0], mval[1]+10, mval);
					size[val]*= yref;
					
/**/			/* } */


				/* grid */
				apply_keyb_grid(size, 0.0, 0.1, 0.01, gridflag & AUTOSIZEGRID);
				apply_keyb_grid(size+1, 0.0, 0.1, 0.01, gridflag & AUTOSIZEGRID);
				apply_keyb_grid(size+2, 0.0, 0.1, 0.01, gridflag & AUTOSIZEGRID);
				
				if(transmain) {
					size[0]= MINSIZE(size[0], 0.01);
					size[1]= MINSIZE(size[1], 0.01);
					size[2]= MINSIZE(size[2], 0.01);
				}
				
				if(size[0]!=oldval[0] ||size[1]!=oldval[1] ||size[2]!=oldval[2]) {
					VECCOPY(oldval, size);
					
					SizeToMat3(size, mat);

					/* apply */
					tob= transmain;
					tv= transvmain;
					
					for(a=0; a<tottrans; a++, tob++, tv++) {
						if(transmain) {
							/* size moet lokaal t.o.v. ouder EN van eigen rotatie */
							/* lokaal tov. ouder: */
							Mat3MulSerie(smat, tob->parmat, mat, tob->parinv, 0);
							
							/* lokaal tov. eigen rot: */
							Mat3MulSerie(totmat, tob->obmat, smat, tob->obinv, 0);

							sizelo[0]= totmat[0][0];
							sizelo[1]= totmat[1][1];
							sizelo[2]= totmat[2][2];
							
							if(midtog) {
								/* dan klopt de vorige berekening van de juiste size niet meer precies */
								apply_keyb_grid(sizelo, 0.0, 0.1, 0.01, gridflag & AUTOSIZEGRID);
								apply_keyb_grid(sizelo+1, 0.0, 0.1, 0.01, gridflag & AUTOSIZEGRID);
								apply_keyb_grid(sizelo+2, 0.0, 0.1, 0.01, gridflag & AUTOSIZEGRID);
							}

								/* x flip */
/**/						/* sizelo[0]*= xref; */
								/* y flip */
/**/						/* sizelo[1]*= yref; */
								/* z flip */
/**/						/* sizelo[2]*= zref; */


							/* what you see is what you want; not what you get! */
							/* correctie voor delta size */
							if(tob->flag & TOB_IPO) {
								/* deltasize berekenen (gelijk voor size en dsize) */
								
								vec[0]= (tob->oldsize[0]+tob->olddsize[0])*(sizelo[0] -1.0);
								vec[1]= (tob->oldsize[1]+tob->olddsize[1])*(sizelo[1] -1.0);
								vec[2]= (tob->oldsize[2]+tob->olddsize[2])*(sizelo[2] -1.0);

								add_ipo_tob_poin(tob->sizex, tob->oldsize+3, vec[0]);
								add_ipo_tob_poin(tob->sizey, tob->oldsize+4, vec[1]);
								add_ipo_tob_poin(tob->sizez, tob->oldsize+5, vec[2]);
								
							}
							else {
								tob->size[0]= (tob->oldsize[0]+tob->olddsize[0])*sizelo[0] -tob->olddsize[0];
								tob->size[1]= (tob->oldsize[1]+tob->olddsize[1])*sizelo[1] -tob->olddsize[1];
								tob->size[2]= (tob->oldsize[2]+tob->olddsize[2])*sizelo[2] -tob->olddsize[2];
							}
							
							if(G.vd->around!=V3D_LOCAL) {
								/* translatie */
								VecSubf(vec, tob->obvec, centre);
								Mat3MulVecfl(mat, vec);
								VecAddf(vec, vec, centre);
								/* vec is nu de plek waar het object moet komen */
								VecSubf(vec, vec, tob->obvec);
								Mat3MulVecfl(tob->parinv, vec);

								if(tob->flag & TOB_IPO) {
									add_ipo_tob_poin(tob->locx, tob->oldloc, vec[0]);
									add_ipo_tob_poin(tob->locy, tob->oldloc+1, vec[1]);
									add_ipo_tob_poin(tob->locz, tob->oldloc+2, vec[2]);
								}
								else if(tob->loc) {
									if(transmode==TRANS_TEX) ;
									else  VecAddf(tob->loc, tob->oldloc, vec);
								}
							}
						}
						else {
							/* voor print */
							VECCOPY(sizelo, size);
							
							if(mode=='C') {
								size[0]= tv->fac*size[0]+ 1.0-tv->fac;;
								size[1]= tv->fac*size[1]+ 1.0-tv->fac;;
								size[2]= tv->fac*size[2]+ 1.0-tv->fac;;
								SizeToMat3(size, mat);
								VECCOPY(size, oldval);
							}
							
							if(mode=='S') {	/* shear */
								Mat3One(tmat);
								tmat[0][0]= tmat[2][2]= tmat[1][1]= 1.0;
								tmat[1][0]= size[0]-1.0;
								
								Mat3MulMat3(totmat, persmat, omat);
								Mat3MulMat3(mat, tmat, totmat);
								Mat3MulMat3(totmat, persinv, mat);
								Mat3MulMat3(smat, imat, totmat);
							}
							else {
								Mat3MulMat3(totmat, mat, omat);
								Mat3MulMat3(smat, imat, totmat);
							}
							
							VecSubf(vec, tv->oldloc, centre);
							Mat3MulVecfl(smat, vec);
							VecAddf(tv->loc, vec, centre);
							
							if(G.obedit->type==OB_MBALL) *(tv->val)= size[0]*tv->oldval;
						}
					}
					if(mode=='s') 
						sprintf(str, "Sizex: %.3f   Sizey: %.3f  Sizez: %.3f", sizelo[0], sizelo[1], sizelo[2]);
					else if (mode=='S')
						sprintf(str, "Shear: %.3f ", sizelo[0]);
					else if (mode=='C')
						sprintf(str, "Size: %.3f ", sizelo[0]);
					
					headerprint(str);
					
					time= my_clock();

					if(G.obedit) calc_trans_verts();
					special_trans_update();

					if(fast==0) {
						force_draw();
						time= my_clock()-time;
						if(time>50) fast= 1;
					}
					else {
						curarea->windraw();
						screen_swapbuffers();
					}
					if(tottrans>1 || G.vd->around==V3D_CURSOR) helpline(centre);
				}
			}
			else if(mode=='w') {
				
				window_to_3d(dvec, 1, 1);
				
				omtrekfac= startomtrekfac+ 0.05*( mval[1] - yn)*Normalise(dvec);
	
				/* berekenen hoek voor print */
				dist= max[0]-centre[0];
				phi0= 360*omtrekfac*dist/(rad*M_PI);
				
				if(G.qual & LR_CTRLKEY) {
					phi0= 5.0*ffloor(phi0/5.0);
					omtrekfac= (phi0*rad*M_PI)/(360.0*dist);
				}
				
				
				sprintf(str, "Warp %3.3f", phi0); 
				headerprint(str);
	
				/* elke vertex moet apart geroteerd */
				tob= transmain;
				tv= transvmain;
					
				for(a=0; a<tottrans; a++, tob++, tv++) {
					if(transvmain) {
						
						/* punt transleren naar centre, zodanig roteren dat omtrekafstand==afstand */
						
						VECCOPY(vec, tv->oldloc);
						Mat4MulVecfl(G.obedit->obmat, vec);
						Mat4MulVecfl(G.vd->viewmat, vec);
								
						dist= vec[0]-centre[0];
					
						phi0= (omtrekfac*dist/rad) - 0.5*M_PI;
					
						co= fcos(phi0);
						si= fsin(phi0);
						
						vec[0]= (centre[0]-axis[0]);
						vec[1]= (vec[1]-axis[1]);
						
						tv->loc[0]= si*vec[0]+co*vec[1]+axis[0];
						
						tv->loc[1]= co*vec[0]-si*vec[1]+axis[1];
						tv->loc[2]= vec[2];
						
						Mat4MulVecfl(viewinv, tv->loc);
						Mat4MulVecfl(imat4, tv->loc);
						
					}
				}
				
				if(G.obedit) calc_trans_verts();
				special_trans_update();
				
				if(fast==0) {
					time= my_clock();
					force_draw();
					time= my_clock()-time;
					if(time>50) fast= 1;
				}
				else {
					curarea->windraw();
					screen_swapbuffers();
				}
			}
		}
		
		while( qtest() ) {
			event= extern_qread(&val);
			
			if(val) {
				switch(event) {
				case ESCKEY:
				case LEFTMOUSE:
				case RIGHTMOUSE:
				case SPACEKEY:
				case RETKEY:
					afbreek= 1;
					break;
				case MIDDLEMOUSE:
					midtog= ~midtog;
					if(midtog) {
						proj= test_midtog_proj(xn, yn, mval);
						phi0= phi1= 0.0;
						if(cameragrab) {
							dvec[0]= dvec[1]= dvec[2]= 0.0;
						}
					}
					firsttime= 1;
					break;
				case GKEY:
				case RKEY:
				case SKEY:
					getmouseco_areawin(mval);
					xn=xo= mval[0];
					yn=xo= mval[1];
					dx1= xc-xn; 
					dy1= yc-yn;
					phi= phi0= phi1= 0.0;
					sizefac= fsqrt( (float)((yc-yn)*(yc-yn)+(xn-xc)*(xn-xc)) );
					if(sizefac<2.0) sizefac= 2.0;
					
					if (G.obedit && (G.f & G_PROPORTIONAL)) {
						if(event==GKEY) mode= 'G';
						else if(event==RKEY) mode= 'R';
						else if(event==SKEY) mode= 'C';						
					} else {
						if(event==GKEY) mode= 'g';
						else if(event==RKEY) mode= 'r';
						else if(event==SKEY) mode= 's';
					}

					firsttime= 1;
					
					tob= transmain;
					tv= transvmain;
					for(a=0; a<tottrans; a++, tob++, tv++) {
						if(transmain) {
							restore_tob(tob);
						}
						else {
							VECCOPY(tv->loc, tv->oldloc);
						}
					}
					break;
				
				case XKEY:
					xref= -xref;
					if(rotmode==XROT) rotmode= 0;
					else rotmode= XROT;
					firsttime= 1; break;
					
				case YKEY:
					yref= -yref;
					if(rotmode==YROT) rotmode= 0;
					else rotmode= YROT;
					firsttime= 1; break;
					
				case ZKEY:
					zref= -zref;
					if(rotmode==ZROT) rotmode= 0;
					else rotmode= ZROT;
					firsttime= 1; break;
				
				case PADPLUSKEY:
					if(G.f & G_PROPORTIONAL) {
						prop_size*= 1.1;
						firsttime= 1;
					}
					break;
				case PADMINUS:
					if(G.f & G_PROPORTIONAL) {
						prop_size*= 0.90909090;
						firsttime= 1;
					}
					break;
				
				case LEFTSHIFTKEY:
				case RIGHTSHIFTKEY:
					VECCOPY(d_dvec, dvec);
				case LEFTCTRLKEY:
				case RIGHTCTRLKEY:
					firsttime= 1;
					break;
				}
	
				arrowsmovecursor(event);
			}
			if(event==0 || afbreek) break;

		}
		xo= mval[0];
		yo= mval[1];
				
		if( qtest()==0) usleep(1);
		
	}
	G.moving= 0;
	
	if(event==ESCKEY || event==RIGHTMOUSE) {
		tv= transvmain;
		tob= transmain;
		for(a=0; a<tottrans; a++, tob++, tv++) {
			if(transmain) {
				restore_tob(tob);
			}
			else {
				VECCOPY(tv->loc, tv->oldloc);
				if(tv->val) *(tv->val)= tv->oldval;
			}
		}
		if(G.obedit) calc_trans_verts();
		special_trans_update();
	}
	
	a= 0;
	if(xref<0) a++;
	if(yref<0) a++;
	if(zref<0) a++;
	special_aftertrans_update(mode, a & 1);

	allqueue(REDRAWVIEW3D, 0);
	addqueue(curarea->headwin, REDRAW, 1);
	
	clearbaseflags_for_editing();
	
	if(transmain) freeN(transmain);
	transmain= 0;
	if(transvmain) freeN(transvmain);
	transvmain= 0;

	tottrans= 0;
}

void std_rmouse_transform(void (*xf_func)(int))
{
	short mval[2];
	short xo, yo;
	
	getmouseco_areawin(mval);
	xo= mval[0]; 
	yo= mval[1];
	
	while(get_mbut()&R_MOUSE) {
		usleep(1);
			
		getmouseco_areawin(mval);
		if(abs(mval[0]-xo)+abs(mval[1]-yo) > 10) {
			xf_func('g');
			while(get_mbut()&R_MOUSE) usleep(1);
			return;
		}
	}	
}

void rightmouse_transform()
{
	std_rmouse_transform(transform);
}


/* ************************************** */


void single_object_users(int flag)	
	/* hierna wel clear_id_newpoins() aanroepen */
{
	Base *base, *obase;
	Object *ob, *obn;
	int totdup;
	
	/* dupliceren */
	base= FIRSTBASE;
	while(base) {
		ob= base->object;
		
		if( (base->flag & flag)==flag) {

			if(ob->id.lib==0 && ob->id.us>1) {
			
				obn= copy_object(ob);
				ob->id.us--;
				base->object= obn;
			}
		}
		base= base->next;
	}
	
	ID_NEW(G.scene->camera);
	if(G.vd) ID_NEW(G.vd->camera);
	
	/* evt object pointers */
	base= FIRSTBASE;
	while(base) {
		ob= base->object;
		if(ob->id.lib==0) {
			if( (base->flag & flag)==flag) {
				
				ID_NEW(ob->parent);
				ID_NEW(ob->track);
				
			}
		}
		base= base->next;
	}
}

void new_id_matar(Material **matar, int totcol)
{
	ID *id;
	int a;
	
	for(a=0; a<totcol; a++) {
		id= (ID *)matar[a];
		if(id && id->lib==0) {
			if(id->new) {
				matar[a]= (Material *)id->new;
				id_us_plus(id->new);
				id->us--;
			}
			else if(id->us>1) {
				matar[a]= copy_material(matar[a]);
				id->us--;
				id->new= (ID *)matar[a];
			}
		}
	}
}

void single_obdata_users(int flag)
{
	Object *ob;
	Lamp *la;
	Curve *cu;
	Ika *ika;
	Deform *def;
	Base *base;
	Mesh *me;
	Material *ma;
	Sector *se;
	Life *lf;
	ID *id;
	int a;
	
	base= FIRSTBASE;
	while(base) {
		ob= base->object;
		if(ob->id.lib==0 && (base->flag & flag)==flag ) {
			id= ob->data;
			
			if(id && id->us>1 && id->lib==0) {
				
				switch(ob->type) {
				case OB_LAMP:
					if(id && id->us>1 && id->lib==0) {
						ob->data= la= copy_lamp(ob->data);
						for(a=0; a<8; a++) {
							if(la->mtex[a]) {
								ID_NEW(la->mtex[a]->object);
							}
						}
					}
					break;
				case OB_CAMERA:
					ob->data= copy_camera(ob->data);
					break;
				case OB_MESH:
					ob->data= copy_mesh(ob->data);
					break;
				case OB_MBALL:
					ob->data= copy_mball(ob->data);
					break;
				case OB_CURVE:
				case OB_SURF:
				case OB_FONT:
					ob->data= cu= copy_curve(ob->data);
					ID_NEW(cu->bevobj);
					makeDispList(ob);
					break;
				case OB_LATTICE:
					ob->data= copy_lattice(ob->data);
					break;
				case OB_LIFE:
					ob->data= copy_life(ob->data);
					break;
				case OB_SECTOR:
					ob->data= copy_sector(ob->data);
					break;
				case OB_IKA:
					/* this never occurs? IK is always single user */
					ob->data= ika= copy_ika(ob->data);
					ID_NEW(ika->parent);
					
					if(ika->totdef) {
						a= ika->totdef;
						def= ika->def;
						while(a--) {
							ID_NEW(def->ob);
							def++;
						}
					}
					
					break;
				default:
					printf("ERROR single_obdata_users: %s\n", id->name);
					error("Read console");
					return;
				}
				
				id->us--;
				id->new= ob->data;
				
			}
			else if(ob->type==OB_SECTOR) {
				se= ob->data;
				id= (ID *)se->dynamesh;
				if(id && id->us>1) {
					se->dynamesh= copy_mesh(se->dynamesh);
					id->us--;
				}
				id= (ID *)se->texmesh;
				if(id && id->us>1) {
					se->texmesh= copy_mesh(se->texmesh);
					id->us--;
				}
			}
			else if(ob->type==OB_LIFE) {
				lf= ob->data;
				id= (ID *)lf->dynamesh;
				if(id && id->us>1) {
					lf->dynamesh= copy_mesh(lf->dynamesh);
					id->us--;
				}
				id= (ID *)lf->texmesh;
				if(id && id->us>1) {
					lf->texmesh= copy_mesh(lf->texmesh);
					id->us--;
				}
			}
			
			id= (ID *)ob->ipo;
			if(id && id->us>1 && id->lib==0) {
				if(id->new) {
					ob->ipo= (Ipo *)id->new;
					id_us_plus(id->new);
				}
				else {
					ob->ipo= copy_ipo(ob->ipo);
					id->us--;
					id->new= (ID *)ob->ipo;
				}
			}
			switch(ob->type) {
			case OB_LAMP:
				la= ob->data;
				if(la->ipo && la->ipo->id.us>1) {
					la->ipo->id.us--;
					la->ipo= copy_ipo(la->ipo);
				}
			}
			
		}
		base= base->next;
	}
	
	me= G.main->mesh.first;
	while(me) {
		ID_NEW(me->texcomesh);
		me= me->id.next;
	}
}


void single_mat_users(int flag)
{
	Object *ob;
	Base *base;
	Material *ma, *man;
	Tex *tex;
	ID *id;
	int a, b;
	
	
	base= FIRSTBASE;
	while(base) {
		ob= base->object;
		if(ob->id.lib==0 && (flag==0 || (base->flag & SELECT)) ) {
	
			for(a=1; a<=ob->totcol; a++) {
				ma= give_current_material(ob, a);
				if(ma) {
					/* hier niet LIB_NEW testen: deze fie geeft gegarandeerde single_users! */
					
					if(ma->id.us>1) {
						man= copy_material(ma);
					
						man->id.us= 0;
						assign_material(ob, man, a);
						
						if(ma->ipo) {
							man->ipo= copy_ipo(ma->ipo);
							ma->ipo->id.us--;
						}
						
						for(b=0; b<8; b++) {
							if(ma->mtex[b] && ma->mtex[b]->tex) {
								tex= ma->mtex[b]->tex;
								if(tex->id.us>1) {
									ma->mtex[b]->tex= copy_texture(tex);
									tex->id.us--;
								}
							}
						}
						
					}
				}
			}
		}
		base= base->next;
	}
}

void do_single_tex_user(Tex **from)
{
	Tex *tex, *texn;
	
	tex= *from;
	if(tex==0) return;
	
	if(tex->id.new) {
		*from= (Tex *)tex->id.new;
		id_us_plus(tex->id.new);
		tex->id.us--;
	}
	else if(tex->id.us>1) {
		texn= copy_texture(tex);
		tex->id.new= (ID *)texn;
		tex->id.us--;
		*from= texn;
	}
	
}

void single_tex_users_expand()
{
	/* alleen als 'ouder' blokken LIB_NEW zijn */
	Material *ma;
	Lamp *la;
	World *wo;
	Tex *tex, *texn;
	int b;
		
	ma= G.main->mat.first;
	while(ma) {
		if(ma->id.flag & LIB_NEW) {
			for(b=0; b<8; b++) {
				if(ma->mtex[b] && ma->mtex[b]->tex) {
					do_single_tex_user( &(ma->mtex[b]->tex) );
				}
			}
		}
		ma= ma->id.next;
	}

	la= G.main->lamp.first;
	while(la) {
		if(la->id.flag & LIB_NEW) {
			for(b=0; b<6; b++) {
				if(la->mtex[b] && la->mtex[b]->tex) {
					do_single_tex_user( &(la->mtex[b]->tex) );
				}
			}
		}
		la= la->id.next;
	}
	wo= G.main->world.first;
	while(wo) {
		if(wo->id.flag & LIB_NEW) {
			for(b=0; b<6; b++) {
				if(wo->mtex[b] && wo->mtex[b]->tex) {
					do_single_tex_user( &(wo->mtex[b]->tex) );
				}
			}
		}
		wo= wo->id.next;
	}
}

void single_mat_users_expand()
{
	/* alleen als 'ouder' blokken LIB_NEW zijn */

	Object *ob;
	Mesh *me;
	Curve *cu;
	MetaBall *mb;
	Material *ma;
	int a;
	
	ob= G.main->object.first;
	while(ob) {
		if(ob->id.flag & LIB_NEW) {
			new_id_matar(ob->mat, ob->totcol);
		}
		ob= ob->id.next;
	}

	me= G.main->mesh.first;
	while(me) {
		if(me->id.flag & LIB_NEW) {
			new_id_matar(me->mat, me->totcol);
		}
		me= me->id.next;
	}

	cu= G.main->curve.first;
	while(cu) {
		if(cu->id.flag & LIB_NEW) {
			new_id_matar(cu->mat, cu->totcol);
		}
		cu= cu->id.next;
	}

	mb= G.main->mball.first;
	while(mb) {
		if(mb->id.flag & LIB_NEW) {
			new_id_matar(mb->mat, mb->totcol);
		}
		mb= mb->id.next;
	}

	/* material imats  */
	ma= G.main->mat.first;
	while(ma) {
		if(ma->id.flag & LIB_NEW) {
			for(a=0; a<8; a++) {
				if(ma->mtex[a]) {
					ID_NEW(ma->mtex[a]->object);
				}
			}
		}
		ma= ma->id.next;
	}
}

void single_user()
{
	int nr;
	
	if(G.scene->id.lib) return;

	nr= pupmenu("Make Single User%t|Object|Object & ObData|Object & ObData & Materials+Tex|Materials+Tex");
	if(nr>0) {
	
		if(nr==1) single_object_users(1);
	
		else if(nr==2) {
			single_object_users(1);
			single_obdata_users(1);
		}
		else if(nr==3) {
			single_object_users(1);
			single_obdata_users(1);
			single_mat_users(1); /* ook tex */
			
		}
		else if(nr==4) {
			single_mat_users(1);
		}
		
		clear_id_newpoins();

		countall();
		allqueue(REDRAWALL, 0);
	}
}

/* ************************************************************* */


void make_local()
{
	Base *base;
	Object *ob;
	Material *ma, ***matarar;
	Lamp *la;
	Curve *cu;
	ID *id;
	int nr, a, b, mode;
	
	/* LETOP: de functie new_id(..) voegt het id blok opnieuw in!!! */
	
	if(G.scene->id.lib) return;
	
	mode= pupmenu("Make Local%t|Selected %x1|All %x2");
	
	if(mode==2) {
		all_local();
		allqueue(REDRAWALL, 0);
		return;
	}
	else if(mode!=1) return;

	clear_id_newpoins();
	
	base= FIRSTBASE;
	while(base) {
		ob= base->object;
		if( (base->flag & SELECT)) {
			if(ob->id.lib) {
				make_local_object(ob);
			}
		}
		base= base->next;
	}
	
	/* evt object pointers */
	base= FIRSTBASE;
	while(base) {
		ob= base->object;
		if( (base->flag & SELECT)) {
			if(ob->id.lib==0) {
				ID_NEW(ob->parent);
				ID_NEW(ob->track);
			}
		}
		base= base->next;
	}

	base= FIRSTBASE;
	while(base) {
		ob= base->object;
		if( (base->flag & SELECT) ) {
	
			id= ob->data;
			
			if(id) {
				
				switch(ob->type) {
				case OB_LAMP:
					make_local_lamp((Lamp *)id);
					
					la= ob->data;
					id= (ID *)la->ipo;
					if(id && id->lib) make_local_ipo(la->ipo);
					
					break;
				case OB_CAMERA:
					make_local_camera((Camera *)id);
					break;
				case OB_MESH:
					make_local_mesh((Mesh *)id);
					make_local_key( ((Mesh *)id)->key );
					break;
				case OB_MBALL:
					make_local_mball((MetaBall *)id);
					break;
				case OB_SECTOR:
					make_local_sector((Sector *)id);
					break;
				case OB_LIFE:
					make_local_life((Life *)id);
					break;
				case OB_CURVE:
				case OB_SURF:
				case OB_FONT:
					cu= (Curve *)id;
					make_local_curve(cu);
					id= (ID *)cu->ipo;
					if(id && id->lib) make_local_ipo(cu->ipo);
					make_local_key( cu->key );
					break;
				case OB_LATTICE:
					make_local_lattice((Lattice *)id);
					make_local_key( ((Lattice *)id)->key );
					break;
				}
			}
			id= (ID *)ob->ipo;
			if(id && id->lib) make_local_ipo(ob->ipo);
		
		}
		base= base->next;		
	}

	base= FIRSTBASE;
	while(base) {
		ob= base->object;
		if(base->flag & SELECT ) {
			
			if(ob->type==OB_LAMP) {
				la= ob->data;
				for(b=0; b<8; b++) {
					if(la->mtex[b] && la->mtex[b]->tex) {
						make_local_texture(la->mtex[b]->tex);
					}
				}
			}
			else {
				
				for(a=0; a<ob->totcol; a++) {
					ma= ob->mat[a];
					if(ma) {
						make_local_material(ma);
					
						for(b=0; b<8; b++) {
							if(ma->mtex[b] && ma->mtex[b]->tex) {
								make_local_texture(ma->mtex[b]->tex);
							}
						}
						id= (ID *)ma->ipo;
						if(id && id->lib) make_local_ipo(ma->ipo);	
					}
				}
				
				matarar= (Material ***)give_matarar(ob);
				
				for(a=0; a<ob->totcol; a++) {
					ma= (*matarar)[a];
					if(ma) {
						make_local_material(ma);
					
						for(b=0; b<8; b++) {
							if(ma->mtex[b] && ma->mtex[b]->tex) {
								make_local_texture(ma->mtex[b]->tex);
							}
						}
						id= (ID *)ma->ipo;
						if(id && id->lib) make_local_ipo(ma->ipo);	
					}
				}
			}
		}
		base= base->next;
	}


	allqueue(REDRAWALL, 0);

}


void adduplicate(float *dtrans)
/* dtrans is 3 x 3xfloat dloc, drot en dsize */
{
	Base *base, *basen;
	Object *ob, *obn;
	Ika *ika;
	Deform *def;
	Material ***matarar, *ma, *mao;
	ID *id;
	int a, didit, totdup, dupflag;
	
	if(G.scene->id.lib) return;
	clear_id_newpoins();
	
	if( G.qual & LR_ALTKEY ) dupflag= 0;
	else dupflag= U.dupflag;
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
		
			ob= base->object;
			obn= copy_object(ob);
			
			basen= mallocN(sizeof(Base), "duplibase");
			*basen= *base;
			addhead(&G.scene->base, basen);	/* addhead: anders oneindige lus */
			basen->object= obn;
			base->flag &= ~SELECT;
			
			if(BASACT==base) BASACT= basen;

			/* duplicates ahv userflags */
			
			if(dupflag & DUPIPO) {
				id= (ID *)obn->ipo;
				if(id) {
					ID_NEW_US( obn->ipo)
					else obn->ipo= copy_ipo(obn->ipo);
					id->us--;
				}
			}
			
			if(dupflag & DUPMAT) {
				for(a=0; a<obn->totcol; a++) {
					id= (ID *)obn->mat[a];
					if(id) {
						ID_NEW_US(obn->mat[a])
						else obn->mat[a]= copy_material(obn->mat[a]);
						id->us--;
					}
				}
			}
			
			id= obn->data;
			didit= 0;
			
			switch(obn->type) {
			case OB_MESH:
				if(dupflag & DUPMESH) {
					ID_NEW_US2( obn->data )
					else {
						obn->data= copy_mesh(obn->data);
						didit= 1;
					}
					id->us--;
				}
				break;
			case OB_CURVE:
				if(dupflag & DUPCURVE) {
					ID_NEW_US2(obn->data )
					else {
						obn->data= copy_curve(obn->data);
						makeDispList(ob);
						didit= 1;
					}
					id->us--;
				}
				break;
			case OB_SURF:
				if(dupflag & DUPSURF) {
					ID_NEW_US2( obn->data )
					else {
						obn->data= copy_curve(obn->data);
						makeDispList(ob);
						didit= 1;
					}
					id->us--;
				}
				break;
			case OB_FONT:
				if(dupflag & DUPFONT) {
					ID_NEW_US2( obn->data )
					else {
						obn->data= copy_curve(obn->data);
						makeDispList(ob);
						didit= 1;
					}
					id->us--;
				}
				break;
			case OB_MBALL:
				if(dupflag & DUPMBALL) {
					ID_NEW_US2(obn->data )
					else {
						obn->data= copy_mball(obn->data);
						didit= 1;
					}
					id->us--;
				}
				break;
			case OB_LAMP:
				if(dupflag & DUPLAMP) {
					ID_NEW_US2(obn->data )
					else obn->data= copy_lamp(obn->data);
					id->us--;
				}
				break;

			/* altijd dupli's */
		
			case OB_LATTICE:
				ID_NEW_US2(obn->data )
				else obn->data= copy_lattice(obn->data);
				id->us--;
				break;
			case OB_SECTOR:
				ID_NEW_US2(obn->data )
				else obn->data= copy_sector(obn->data);
				id->us--;
				break;
			case OB_LIFE:
				ID_NEW_US2(obn->data )
				else obn->data= copy_life(obn->data);
				id->us--;
				break;
			case OB_CAMERA:
				ID_NEW_US2(obn->data )
				else obn->data= copy_camera(obn->data);
				id->us--;
				break;
			case OB_IKA:
				ID_NEW_US2(obn->data )
				else obn->data= copy_ika(obn->data);
				id->us--;
				break;
			}
			
			if(dupflag & DUPMAT) {
				matarar= give_matarar(obn);
				if(didit && matarar) {
					for(a=0; a<obn->totcol; a++) {
						id= (ID *)(*matarar)[a];
						if(id) {
							ID_NEW_US( (*matarar)[a] )
							else (*matarar)[a]= copy_material((*matarar)[a]);
							
							id->us--;
						}
					}
				}
			}
						
		}
		base= base->next;
	}
	
	/* evt object pointers */
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			
			ID_NEW(base->object->parent);
			ID_NEW(base->object->track);
			
			if(base->object->type==OB_IKA) {
				ika= base->object->data;
				ID_NEW(ika->parent);
				
				a= ika->totdef;
				def= ika->def;
				while(a--) {
					ID_NEW(def->ob);
					def++;
				}
			}
		}
		base= base->next;
	}

	/* materialen */
	if( dupflag & DUPMAT) {
		mao= G.main->mat.first;
		while(mao) {
			if(mao->id.new) {
				
				ma= (Material *)mao->id.new;
				
				if(dupflag & DUPTEX) {
					for(a=0; a<8; a++) {
						if(ma->mtex[a]) {
							id= (ID *)ma->mtex[a]->tex;
							if(id) {
								ID_NEW_US(ma->mtex[a]->tex)
								else ma->mtex[a]->tex= copy_texture(ma->mtex[a]->tex);
								id->us--;
							}
						}
					}
				}
				id= (ID *)ma->ipo;
				if(id) {
					ID_NEW_US(ma->ipo)
					else ma->ipo= copy_ipo(ma->ipo);
					id->us--;
				}
			}
			mao= mao->id.next;
		}
	}

	sort_baselist(G.scene);
	clear_id_newpoins();
	
	countall();
	if(dtrans==0) transform('g');
	
	redraw_test_buttons(BASACT);
	allqueue(REDRAWIPO, 0);	/* ook oops */
}


void selectlinks()
{
	Object *ob;
	Base *base;
	void *obdata;
	Ipo *ipo;
	Material *mat, *mat1;
	Tex *tex=0, *tex1;
	int a, b, nr;
	
	ob= OBACT;
	if(ob==0) return;
	nr= pupmenu("Select links%t|Object Ipo|Object Data|Current Material|Current texture");
	
	if(nr==1) {
		ipo= ob->ipo;
		if(ipo==0) return;
	}
	else if(nr==2) {
		if(ob->data==0) return;
		if ELEM(ob->type, OB_SECTOR, OB_LIFE) obdata= get_mesh(ob);
		else obdata= ob->data;
	}
	else if(nr==3 || nr==4) {
		mat= give_current_material(ob, ob->actcol);
		if(mat==0) return;
		if(nr==4) {
			if(mat->mtex[ mat->texact ]) tex= mat->mtex[ mat->texact ]->tex;
			if(tex==0) return;
		}
	}
	else return;
	
	base= FIRSTBASE;
	while(base) {
		if(base->lay & G.vd->lay) {
			if(nr==1) {
				if(base->object->ipo==ipo) base->flag |= SELECT;
			}
			else if(nr==2) {
				if ELEM(base->object->type, OB_SECTOR, OB_LIFE) {
					if(obdata==get_mesh(base->object) || obdata==get_other_mesh(base->object)) base->flag |= SELECT;
				}
				else if(base->object->data==obdata) base->flag |= SELECT;
			}
			else if(nr==3 || nr==4) {
				ob= base->object;
				
				for(a=1; a<=ob->totcol; a++) {
					mat1= give_current_material(ob, a);
					if(nr==3) {
						if(mat1==mat) base->flag |= SELECT;
					}
					else if(mat1 && nr==4) {
						for(b=0; b<8; b++) {
							if(mat1->mtex[b]) {
								if(tex==mat1->mtex[b]->tex) base->flag |= SELECT;
							}
						}
					}
				}
			}
			base->object->flag= base->flag;
		}
		base= base->next;
	}
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWDATASELECT, 0);
	allqueue(REDRAWOOPS, 0);
}

void image_aspect()
{
	/* alle geselecteerde objecten die imap hebben: scalen in ima verhouding */
	Base *base;
	Object *ob;
	Material *ma;
	Tex *tex;
	Mesh *me;
	Curve *cu;
	float x, y, space;
	int a, b, done;
	
	if(G.obedit) return;
	if(G.scene->id.lib) return;
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			ob= base->object;
			done= 0;
			
			for(a=1; a<=ob->totcol; a++) {
				ma= give_current_material(ob, a);
				if(ma) {
					for(b=0; b<8; b++) {
						if(ma->mtex[b] && ma->mtex[b]->tex) {
							tex= ma->mtex[b]->tex;
							if(tex->type==TEX_IMAGE && tex->ima && tex->ima->ibuf) {
								/* texturespace */
								space= 1.0;
								if(ob->type==OB_MESH) {
									me= ob->data;
									space= me->size[0]/me->size[1];
								}
								else if ELEM3(ob->type, OB_CURVE, OB_FONT, OB_SURF) {
									cu= ob->data;
									space= cu->size[0]/cu->size[1];
								}
							
								x= tex->ima->ibuf->x/space;
								y= tex->ima->ibuf->y;
								
								if(x>y) ob->size[0]= ob->size[1]*x/y;
								else ob->size[1]= ob->size[0]*y/x;
								
								done= 1;
							}
						}
						if(done) break;
					}
				}
				if(done) break;
			}
		}
		base= base->next;
	}
	
	allqueue(REDRAWVIEW3D, 0);
}

void set_ob_ipoflags()
{
	Base *base;
	int set= 1;
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) {
			if(base->object->ipoflag & OB_DRAWKEY) {
				set= 0;
				break;
			}
		}
		base= base->next;
	}
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) {
			if(set) {
				base->object->ipoflag |= OB_DRAWKEY;
				if(base->object->ipo) base->object->ipo->showkey= 1;
			}
			else {
				base->object->ipoflag &= ~OB_DRAWKEY;
			}
		}
		base= base->next;
	}
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWBUTSANIM, 0);
	if(set) {
		allspace(REMAKEIPO, 0);
		allqueue(REDRAWIPO, 0);
	}
}

void select_select_keys()
{
	Base *base;
	uint lay= 0, local;
	IpoCurve *icu;
	BezTriple *bezt;
	int a;
	
	if(G.scene->id.lib) return;

	if(okee("show and select all keys")==0) return;

	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			if(base->object->ipo) {
				base->object->ipoflag |= OB_DRAWKEY;
				base->object->ipo->showkey= 1;
				icu= base->object->ipo->curve.first;
				while(icu) {
					a= icu->totvert;
					bezt= icu->bezt;
					while(a--) {
						bezt->f1 |= SELECT;
						bezt->f2 |= SELECT;
						bezt->f3 |= SELECT;
						bezt++;
					}
					icu= icu->next;
				}
			}
		}
		base= base->next;
	}

	allqueue(REDRAWVIEW3D, 0);
	allspace(REMAKEIPO, 0);
	allqueue(REDRAWIPO, 0);

}


MVert *verg_vert;

int verg_hoogste_zco(MFace *x1, MFace *x2)
{
	float z1, z2;
	MVert *v1, *v2, *v3;
	
	v1= verg_vert+x1->v1;
	v2= verg_vert+x1->v2;
	v3= verg_vert+x1->v3;
	z1= MAX3(v1->co[2], v2->co[2], v3->co[2]);
	
	v1= verg_vert+x2->v1;
	v2= verg_vert+x2->v2;
	v3= verg_vert+x2->v3;
	z2= MAX3(v1->co[2], v2->co[2], v3->co[2]);

	if( z1 > z2 ) return 1;
	else if( z1 < z2) return -1;
	return 0;
}



void sortfaces()
{
	int a;
	Mesh *me;
	
	if(G.scene->id.lib) return;

	if(G.obedit!=0 || BASACT==0 || OBACT->type!= OB_MESH) return;
	if(okee("Sort faces")==0) return;

	me= OBACT->data;
	verg_vert= me->mvert;
	
	qsort(me->mface, me->totface, sizeof(MFace), (void *)verg_hoogste_zco);
}

int vergbaseco(Base **x1, Base **x2)
{

	if( (*x1)->sy > (*x2)->sy ) return 1;
	else if( (*x1)->sy < (*x2)->sy) return -1;
	else if( (*x1)->sx > (*x2)->sx ) return 1;
	else if( (*x1)->sx < (*x2)->sx ) return -1;

	return 0;
}


void auto_timeoffs()
{
	Base *base, **basesort, **bs;
	float start, delta;
	int tot=0, a;
	short offset=25;

	if(BASACT==0) return;
	if(button(&offset, 0, 1000,"Total time")==0) return;

	/* maak array van alle bases, xco yco (scherm) */
	base= FIRSTBASE;
	while(base) {
		if(TESTBASELIB(base)) {
			tot++;
		}
		base= base->next;
	}

	delta= (float)offset/(float)tot;
	start= OBACT->sf;

	bs= basesort= mallocN(sizeof(void *)*tot,"autotimeoffs");
	base= FIRSTBASE;

	while(base) {
		if(TESTBASELIB(base)) {
			*bs= base;
			bs++;
		}
		base= base->next;
	}
	qsort(basesort, tot, sizeof(void *), (void *)vergbaseco);

	bs= basesort;
	for(a=0; a<tot; a++) {
		
		(*bs)->object->sf= start;
		start+= delta;

		bs++;
	}
	freeN(basesort);

	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWBUTSANIM, 0);
}

void texspace_edit()
{
	Base *base;
	int nr=0;
	
	/* eerst testen of van de zichtbare en geselecteerde ob's
	 * wel de texspacedraw aanstaat:
	 */
	
	if(G.obedit) return;
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			break;
		}
		base= base->next;
	}

	if(base==0) {
		return;
	}
	
	/* nr= pupmenu("Texture space %t|Grabber%x1|Size%x2|Rotate%x3"); */
	nr= pupmenu("Texture space %t|Grabber%x1|Size%x2");
	if(nr<1) return;
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASELIB(base) {
			base->object->dtx |= OB_TEXSPACE;
		}
		base= base->next;
	}
	

	transmode= TRANS_TEX;
	
	if(nr==1) transform('g');
	else if(nr==2) transform('s');
	else if(nr==3) transform('r');
	
	transmode= 0;
}

void first_base()
{
	/* maakt de select bases los en insert ze aan begin */
	Base *base, *next;
	
	if(okee("make first base")==0) return;
	
	base= FIRSTBASE;
	while(base) {
		next= base->next;
		
		if(base->flag & SELECT) {
			remlink(&G.scene->base, base);
			addtail(&G.scene->base, base);
		}
		
		base= next;
	}
	
}

