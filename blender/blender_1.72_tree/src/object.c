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

/* object.c  		MIXED MODEL

 * 
 * jan 95
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "edit.h"
#include "ipo.h"
#include "sector.h"
#include "ika.h"
#include "render.h"

float originmat[3][3];	/* na where_is_object(), kan her en der gebruikt worden */
Object workob;


void clear_workob()
{
	bzero(&workob, sizeof(Object));
	
	workob.size[0]= workob.size[1]= workob.size[2]= 1.0;
	
}

void copy_baseflags()
{
	Base *base;
	
	base= FIRSTBASE;
	while(base) {
		base->object->flag= base->flag;
		base= base->next;
	}
}

void copy_objectflags()
{
	Base *base;
	
	base= FIRSTBASE;
	while(base) {
		base->flag= base->object->flag;
		base= base->next;
	}
}


/* niet object zelf vrijgeven */
void free_object(Object *ob)
{
	View3D *vd;
	Material *ma;
	Object *temp;
	Sector *se;
	Portal *po;
	bScreen *sc;
	ScrArea *sa;
	Scene *sce;
	ID *id;
	int a, b;
	
	/* specifieke data loskoppelen */
	if(ob->data) {
		id= ob->data;
		id->us--;
		if(id->us==0) {
			if(ob->type==OB_MESH) unlink_mesh(ob->data);
			else if(ob->type==OB_SECTOR) unlink_sector(ob->data);
			else if(ob->type==OB_LIFE) unlink_life(ob->data);
			else if(ob->type==OB_CURVE) unlink_curve(ob->data);
			else if(ob->type==OB_MBALL) unlink_mball(ob->data);
		}
		ob->data= 0;
	}
	
	for(a=0; a<ob->totcol; a++) {
		if(ob->mat[a]) ob->mat[a]->id.us--;
	}
	if(ob->mat) freeN(ob->mat);
	ob->mat= 0;
	if(ob->bb) freeN(ob->bb); 
	ob->bb= 0;
	if(ob->path) free_path(ob->path); 
	ob->path= 0;
	if(ob->ipo) ob->ipo->id.us--;
	
	free_effects(&ob->effect);
	freelistN(&ob->network);
	
	freedisplist(&ob->disp);
	
	/* alle objecten aflopen: parentflags */
	/* tevens portals */
	temp= G.main->object.first;
	while(temp) {
		if(temp!=ob) {
			if(temp->parent==ob) temp->parent=0;
			if(temp->track==ob) temp->track==0;
			if(temp->type==OB_SECTOR) {
				se= temp->data;
				if(ob->type==OB_SECTOR) {
					po= se->portals;
					b= se->totport;
					while(b--) {
						if(po->sector && po->sector->ob==ob) po->sector= 0;
						po++;
					}
				}
			}
		}
		temp= temp->id.next;
	}
	
	/* materialen */
	
	/* bevels */
	
	free_scriptlink(&ob->scriptlink);
	
	/* camera's */
	sce= G.main->scene.first;
	while(sce) {
		if(sce->camera==ob) sce->camera= 0;
		sce= sce->id.next;
	}
	
	/* keys */
	
	/* screens */
	sc= G.main->screen.first;
	while(sc) {
		sa= sc->areabase.first;
		while(sa) {
			vd= sa->spacedata.first;
			while(vd) {
				if(vd->spacetype==SPACE_VIEW3D) {
					if(vd->camera==ob) {
						vd->camera= 0;
						if(vd->persp>1) vd->persp= 1;
					}
				}
				vd= vd->next;
			}
			sa= sa->next;
		}
		sc= sc->id.next;
	}
	
	do_realtimelight(0, 0, 0);
}

void free_object_fromscene(Object *ob)
{
	View3D *vd;
	Base *base;
	Object *obt;
	Material *mat;
	World *wrld;
	bScreen *sc;
	ScrArea *sa;
	Scene *sce;
	Curve *cu;
	Ika *ika;
	Deform *def;
	ID *id;
	Tex *tex;
	int a;
	
	/* alle objecten aflopen: parents en bevels */
	obt= G.main->object.first;
	while(obt) {
		if(obt->id.lib==0) {
			if(obt->parent==ob) {
				obt->parent= 0;
				if(ob->type==OB_LATTICE) freedisplist(&obt->disp);
			}
			if(obt->track==ob) obt->track= 0;
			if(obt->network.first) {
				NetLink *nl= obt->network.first, *next;
				while(nl) {
					next= nl->next;
					if(nl->ob==ob) {
						remlink(&obt->network, nl);
						freeN(nl);
					}
					nl= next;
				}
			}
			if ELEM(obt->type, OB_CURVE, OB_FONT) {
				cu= obt->data;
				if(cu->bevobj==ob) cu->bevobj= 0;
				if(cu->textoncurve==ob) cu->textoncurve= 0;
			}
			if(obt->type==OB_IKA) {
				ika= obt->data;
				if(ika->parent==ob) ika->parent= 0;
				a= ika->totdef;
				def= ika->def;
				while(a--) {
					if(def->ob==ob) {
						ika->totdef= 0;
						freeN(ika->def);
						ika->def= 0;
						break;
					}
					def++;
				}
			}
		}
		obt= obt->id.next;
	}
	
	/* materialen */
	mat= G.main->mat.first;
	while(mat) {
	
		for(a=0; a<8; a++) {
			if(mat->mtex[a] && ob==mat->mtex[a]->object) {
				/* eigenlijk testen op lib */
				mat->mtex[a]->object= 0;
			}
		}

		mat= mat->id.next;
	}
	
	/* textures */
	tex= G.main->tex.first;
	while(tex) {
		if(tex->env) {
			if(tex->env->object == ob) tex->env->object= 0;
		}
		tex= tex->id.next;
	}
	
	/* mballs */
	if(ob->type==OB_MBALL) {
		obt= find_basis_mball(ob);
		if(obt) freedisplist(&obt->disp);
	}
	
	/* worlds */
	wrld= G.main->world.first;
	while(wrld) {
		if(wrld->id.lib==0) {
			for(a=0; a<6; a++) {
				if(wrld->mtex[a] && ob==wrld->mtex[a]->object)
					wrld->mtex[a]->object==0;
			}
		}
		
		wrld= wrld->id.next;
	}
		
	/* scene's */
	sce= G.main->scene.first;
	while(sce) {
		if(sce->id.lib==0) {
			if(sce->camera==ob) sce->camera= 0;
		}
		sce= sce->id.next;
	}
	/* keys */
	
	/* screens */
	sc= G.main->screen.first;
	while(sc) {
		if(sc->scene==G.scene) {
			sa= sc->areabase.first;
			while(sa) {
				vd= sa->spacedata.first;
				while(vd) {
					if(vd->spacetype==SPACE_VIEW3D) {
						if(vd->camera==ob) {
							vd->camera= 0;
							if(vd->persp>1) vd->persp= 1;
						}
						if(vd->localvd && vd->localvd->camera==ob ) {
							vd->localvd->camera= 0;
							if(vd->localvd->persp>1) vd->localvd->persp= 1;
						}
					}
					vd= vd->next;
				}
				sa= sa->next;
			}
		}
		sc= sc->id.next;
	}
}

int exist_object(Object *obtest)
{
	Object *ob;
	
	ob= G.main->object.first;
	while(ob) {
		if(ob==obtest) return 1;
		ob= ob->id.next;
	}
	return 0;
}

void *add_camera()
{
	Camera *cam;
	
	cam=  alloc_libblock(&G.main->camera, ID_CA, "Camera");

	cam->lens= 35.0;
	cam->clipsta= 0.1;
	cam->clipend= 100.0;
	cam->drawsize= 0.5;
	cam->netsta= 0.5;
	cam->netend= 10.0;
	cam->hold= 50;
	
	return cam;
}

Camera *copy_camera(Camera *cam)
{
	Camera *camn;
	
	camn= copy_libblock(cam);
	id_us_plus((ID *)camn->ipo);

	copy_scriptlink(&camn->scriptlink);
	
	return camn;
}

void make_local_camera(Camera *cam)
{
	Object *ob;
	Camera *camn;
	int local=0, lib=0;
	
	/* - zijn er alleen lib users: niet doen
	 * - zijn er alleen locale users: flag zetten
	 * - mixed: copy
	 */
	
	if(cam->id.lib==0) return;
	if(cam->id.us==1) {
		cam->id.lib= 0;
		cam->id.flag= LIB_LOCAL;
		new_id(0, (ID *)cam, 0);
		return;
	}
	
	ob= G.main->object.first;
	while(ob) {
		if(ob->data==cam) {
			if(ob->id.lib) lib= 1;
			else local= 1;
		}
		ob= ob->id.next;
	}
	
	if(local && lib==0) {
		cam->id.lib= 0;
		cam->id.flag= LIB_LOCAL;
		new_id(0, (ID *)cam, 0);
	}
	else if(local && lib) {
		camn= copy_camera(cam);
		camn->id.us= 0;
		
		ob= G.main->object.first;
		while(ob) {
			if(ob->data==cam) {
				
				if(ob->id.lib==0) {
					ob->data= camn;
					camn->id.us++;
					cam->id.us--;
				}
			}
			ob= ob->id.next;
		}
	}
}



void *add_lamp()
{
	Lamp *la;
	
	la=  alloc_libblock(&G.main->lamp, ID_LA, "Lamp");
	
	la->r= la->g= la->b= 1.0;
	la->haint= la->energy= 1.0;
	la->dist= 20.0*G.vd->grid;
	la->spotsize= 45.0;
	la->spotblend= 0.15;
	la->att2= 1.0;
	la->mode= LA_SHAD;
	la->bufsize= 512;
	la->clipsta= 0.5;
	la->clipend= 40.0;
	la->shadspotsize= 45.0;
	la->samp= 3;
	la->bias= 1.0;
	la->soft= 3.0;

	return la;
}

Lamp *copy_lamp(Lamp *la)
{
	Lamp *lan;
	int a;
	
	lan= copy_libblock(la);

	for(a=0; a<8; a++) {
		if(lan->mtex[a]) {
			lan->mtex[a]= mallocN(sizeof(MTex), "copylamptex");
			memcpy(lan->mtex[a], la->mtex[a], sizeof(MTex));
			id_us_plus((ID *)lan->mtex[a]->tex);
		}
	}
	
	id_us_plus((ID *)lan->ipo);

	copy_scriptlink(&la->scriptlink);
	
	return lan;
}

void make_local_lamp(Lamp *la)
{
	Object *ob;
	Lamp *lan;
	int local=0, lib=0;
	
	/* - zijn er alleen lib users: niet doen
	 * - zijn er alleen locale users: flag zetten
	 * - mixed: copy
	 */
	
	if(la->id.lib==0) return;
	if(la->id.us==1) {
		la->id.lib= 0;
		la->id.flag= LIB_LOCAL;
		new_id(0, (ID *)la, 0);
		return;
	}
	
	ob= G.main->object.first;
	while(ob) {
		if(ob->data==la) {
			if(ob->id.lib) lib= 1;
			else local= 1;
		}
		ob= ob->id.next;
	}
	
	if(local && lib==0) {
		la->id.lib= 0;
		la->id.flag= LIB_LOCAL;
		new_id(0, (ID *)la, 0);
	}
	else if(local && lib) {
		lan= copy_lamp(la);
		lan->id.us= 0;
		
		ob= G.main->object.first;
		while(ob) {
			if(ob->data==la) {
				
				if(ob->id.lib==0) {
					ob->data= lan;
					lan->id.us++;
					la->id.us--;
				}
			}
			ob= ob->id.next;
		}
	}
}

void free_camera(Camera *ca)
{
	free_scriptlink(&ca->scriptlink);
}

void free_lamp(Lamp *la)
{
	MTex *mtex;
	int a;

	/* scriptlinks */
		
	free_scriptlink(&la->scriptlink);
	
	for(a=0; a<8; a++) {
		mtex= la->mtex[a];
		if(mtex && mtex->tex) mtex->tex->id.us--;
		if(mtex) freeN(mtex);
	}
	la->ipo= 0;
}

void *add_wave()
{
	return 0;
}


/* **************** VOORKEUR NAMEN ************** */

char *n_mesh=0, *n_curve=0, *n_mball=0, *n_surf=0, *n_font=0, *n_lamp=0, *n_camera=0;

void set_obact_names(Object *ob)
{
	if(ob==0) return;

	switch(ob->type) {
	case OB_MESH:
		n_mesh= ob->id.name+2; break;
	case OB_CURVE:
		n_curve= ob->id.name+2; break;
	case OB_MBALL:
		n_mball= ob->id.name+2; break;
	case OB_SURF:
		n_surf= ob->id.name+2; break;
	case OB_FONT:
		n_font= ob->id.name+2; break;
	case OB_LAMP:
		n_lamp= ob->id.name+2; break;
	case OB_CAMERA:
		n_camera= ob->id.name+2; break;
	}
}

void clear_obact_names()
{
	 n_mesh= n_curve= n_mball= n_surf= n_font= n_lamp= n_camera= 0;
}



/* *************************************************** */


/* algemene add: in G.scene, met layer uit area en default naam */
/* maakt alle minimaal nodige datablokken aan, zonder vertices etc. */
Object *add_object(int type)
{
	Object *ob;
	Base *base;
	float *curs;
	char name[32];
	
	G.f &= ~(G_VERTEXPAINT+G_FACESELECT);
	setcursor_space(SPACE_VIEW3D, CURSOR_STD);
	
	/* de defaultnaam werkt (voorlopig) niet bij mesh, curve en surf (door add_prim) */
	
	ob= OBACT;
	if(ob==0) clear_obact_names();	/* veiligheid */
	
	switch(type) {
		case OB_MESH: if(n_mesh) strcpy(name, n_mesh); else strcpy(name, "Mesh"); break;
		case OB_CURVE: if(n_curve) strcpy(name, n_curve); else strcpy(name, "Curve"); break;
		case OB_SURF: if(n_surf) strcpy(name, n_surf); else strcpy(name, "Surf"); break;
		case OB_FONT: if(n_font) strcpy(name, n_font); else strcpy(name, "Text"); break;
		case OB_MBALL: if(n_mball) strcpy(name, n_mball); else strcpy(name, "Mball"); break;
		case OB_CAMERA: if(n_camera) strcpy(name, n_camera); else strcpy(name, "Camera"); break;
		case OB_LAMP: if(n_lamp) strcpy(name, n_lamp); else strcpy(name, "Lamp"); break;
		case OB_IKA: strcpy(name, "Ika"); break;
		case OB_LATTICE: strcpy(name, "Lattice"); break;
		case OB_SECTOR: strcpy(name, "Sector"); break;
		case OB_LIFE: strcpy(name, "Life"); break;
		case OB_WAVE: strcpy(name, "Wave"); break;
		default:  strcpy(name, "Empty");
	}
	
	ob= alloc_libblock(&G.main->object, ID_OB, name);
	
	/* default object vars */
	ob->type= type;
	
	curs= give_cursor();
	VECCOPY(ob->loc, curs)
	
	if ELEM(type, OB_SECTOR, OB_LIFE);
	else {
		G.vd->viewquat[0]= -G.vd->viewquat[0];
		QuatToEul(G.vd->viewquat, ob->rot);
		G.vd->viewquat[0]= -G.vd->viewquat[0];
	}
	
	ob->size[0]= ob->size[1]= ob->size[2]= 1.0;				
	Mat4One(ob->parentinv);
	ob->dt= OB_SHADED;
	if(U.flag & MAT_ON_OB) ob->colbits= -1;
	
	if(ob->type==OB_LIFE) ob->dtx= OB_AXIS;
	
	if(type==OB_CAMERA || type==OB_LAMP) {
		ob->trackflag= OB_NEGZ;
		ob->upflag= OB_POSY;
	}
	else {
		ob->trackflag= OB_POSY;
		ob->upflag= OB_POSZ;
	}
	ob->ipoflag = OB_OFFS_OB+OB_OFFS_PARENT;
	
	ob->dupon= 1; ob->dupoff= 0;
	ob->dupsta= 1; ob->dupend= 100;
	
	G.totobj++;
	
	/* specific data */
	switch(type) {
		case OB_MESH: ob->data= add_mesh(); G.totmesh++; break;
		case OB_CURVE: ob->data= add_curve(OB_CURVE); G.totcurve++; break;
		case OB_SURF: ob->data= add_curve(OB_SURF); G.totcurve++; break;
		case OB_FONT: ob->data= add_curve(OB_FONT); break;
		case OB_MBALL: ob->data= add_mball(); break;
		case OB_CAMERA: ob->data= add_camera(); break;
		case OB_LAMP: ob->data= add_lamp(); G.totlamp++; break;
		case OB_IKA: ob->data= add_ika(); ob->dt= OB_WIRE; break;
		case OB_LATTICE: ob->data= add_lattice(); ob->dt= OB_WIRE; break;
		case OB_SECTOR: ob->data= add_sector(ob); break;
		case OB_LIFE: ob->data= add_life(ob); break;
		case OB_WAVE: ob->data= add_wave(); break;
	}
		
	/* aan de scene hangen */
	base= callocN( sizeof(Base), "add_base");
	addhead(&G.scene->base, base);
	
	if(G.vd->localview) base->lay= ob->lay= G.vd->layact+G.vd->lay;
	else base->lay= ob->lay= G.vd->layact;
	
	base->object= ob;
	
	redraw_test_buttons(base);
	allqueue(REDRAWIPO, 0);
	allqueue(REDRAWHEADERS, 0);
	deselect_all_area_oops();
	set_select_flag_oops();
	
	/* deselecteren */
	BASACT= base;
	base= FIRSTBASE;
	while(base) {
		base->flag &= ~SELECT;
		base->object->flag= base->flag;
		base= base->next;
	}
	BASACT->flag |= SELECT;
	ob->flag |= SELECT;
	
	allqueue(REDRAWINFO, 1); 	/* 1, want header->win==0! */

	return ob;
}

Object *copy_object(Object *ob)
{
	Object *obn;
	int a;
	
	obn= copy_libblock(ob);
	
	if(ob->totcol) {
		obn->mat= dupallocN(ob->mat);
	}
	
	if(ob->bb) obn->bb= dupallocN(ob->bb);
	obn->path= 0;
	
	obn->effect.first= obn->effect.last= 0;
	copy_effects(&obn->effect, &ob->effect);
	
	obn->network.first= obn->network.last= 0;
	
	copy_scriptlink(&ob->scriptlink);
	
	/* usernummers ophogen */
	id_us_plus((ID *)obn->data);
	id_us_plus((ID *)obn->ipo);
	for(a=0; a<obn->totcol; a++) id_us_plus((ID *)obn->mat[a]);
	
	obn->disp.first= obn->disp.last= 0;
	
	return obn;
}

void expand_local_object(Object *ob)
{
	int a;
	
	id_lib_extern((ID *)ob->ipo);
	id_lib_extern((ID *)ob->data);
	
	for(a=0; a<ob->totcol; a++) {
		id_lib_extern((ID *)ob->mat[a]);
	}
}

void make_local_object(Object *ob)
{
	Object *obn;
	Scene *sce;
	Base *base;
	int local=0, lib=0;
	
	/* - zijn er alleen lib users: niet doen
	 * - zijn er alleen locale users: flag zetten
	 * - mixed: copy
	 */
	
	if(ob->id.lib==0) return;
	if(ob->id.us==1) {
		ob->id.lib= 0;
		ob->id.flag= LIB_LOCAL;
		new_id(0, (ID *)ob, 0);

	}
	else {
		sce= G.main->scene.first;
		while(sce) {
			base= sce->base.first;
			while(base) {
				if(base->object==ob) {
					if(sce->id.lib) lib++;
					else local++;
					break;
				}
				base= base->next;
			}
			sce= sce->id.next;
		}
		
		if(local && lib==0) {
			ob->id.lib= 0;
			ob->id.flag= LIB_LOCAL;
			new_id(0, (ID *)ob, 0);
		}
		else if(local && lib) {
			obn= copy_object(ob);
			obn->id.us= 0;
			
			sce= G.main->scene.first;
			while(sce) {
				if(sce->id.lib==0) {
					base= sce->base.first;
					while(base) {
						if(base->object==ob) {
							base->object= obn;
							obn->id.us++;
							ob->id.us--;
						}
						base= base->next;
					}
				}
				sce= sce->id.next;
			}
		}
	}
	
	expand_local_object(ob);
}

/* *************** CALC ****************** */

/* er zit ook een tijdberekening in de drawobject() */

float bluroffs= 0.0;
int no_speed_curve= 0;

void set_mblur_offs(int blur)
{
	bluroffs= R.r.blurfac*((float)blur);
	bluroffs/= (float)R.r.osa;
}
	
void disable_speed_curve(int val)
{
	no_speed_curve= val;
}

float bsystem_time(Object *ob, Object *par, float cfra, float ofs)
{
	/* geeft float terug ( zie ook frame_to_float in ipo.c) */
	float sf, f;

	if(no_speed_curve==0) if(ob && ob->ipo) cfra= calc_ipo_time(ob->ipo, cfra);
	
	/* tweede field */
	if(R.flag & R_SEC_FIELD) {
		if(R.r.mode & R_FIELDSTILL); else cfra+= .5;
	}

	/* motion blur */
	cfra+= bluroffs;

	/* global time */
	cfra*= G.scene->r.framelen;	
	
	/* ofset frames */
	if(ob && (ob->ipoflag & OB_OFFS_PARENT)) {
		if((ob->partype & PARSLOW)==0) cfra-= ob->sf;
	}
	
	cfra-= ofs;

	return cfra;
}

void object_to_mat3(Object *ob, float mat[][3])	/* no parent */
{
	float smat[3][3], vec[3];
	float rmat[3][3];
	
	if(ob->ipo) {
		vec[0]= ob->rot[0]+ob->drot[0];
		vec[1]= ob->rot[1]+ob->drot[1];
		vec[2]= ob->rot[2]+ob->drot[2];
		EulToMat3(vec, rmat);

		vec[0]= ob->size[0]+ob->dsize[0];
		vec[1]= ob->size[1]+ob->dsize[1];
		vec[2]= ob->size[2]+ob->dsize[2];
		SizeToMat3(vec, smat);
	}
	else {
		EulToMat3(ob->rot, rmat);
		SizeToMat3(ob->size, smat);
	}
	
	Mat3MulMat3(mat, rmat, smat);
}

void object_to_mat4(Object *ob, float mat[][4])
{
	float tmat[3][3];
	
	object_to_mat3(ob, tmat);
	
	Mat4CpyMat3(mat, tmat);
	
	VECCOPY(mat[3], ob->loc);
	if(ob->ipo) {
		mat[3][0]+= ob->dloc[0];
		mat[3][1]+= ob->dloc[1];
		mat[3][2]+= ob->dloc[2];
	}

}

int enable_cu_speed= 1;

void ob_parcurve(Object *ob, Object *par, float mat[][4])
{
	Curve *cu;
	float q[4], vec[4], dir[3], *quat, x1, ctime;
	
	Mat4One(mat);
	
	cu= par->data;
	if(cu->path==0 || cu->path->data==0) calc_curvepath(par);
	if(cu->path==0) return;
	
	/* uitzondering afvangen: curve paden die als duplicator worden gebruikt */
	if(enable_cu_speed) {
		ctime= bsystem_time(ob, par, (float)CFRA, 0.0);
		
		if(calc_ipo_spec(cu->ipo, CU_SPEED, &ctime)==0) {
			ctime /= cu->pathlen;
			CLAMP(ctime, 0.0, 1.0);
		}
	}
	else {
		ctime= CFRA - ob->sf;
		ctime /= cu->pathlen;
		
		CLAMP(ctime, 0.0, 1.0);
	}

 	if( where_on_path(par, ctime, vec, dir) ) {

		if(cu->flag & CU_FOLLOW) {
			quat= vectoquat(dir, ob->trackflag, ob->upflag);

			Normalise(dir);
			q[0]= fcos(0.5*vec[3]);
			x1= fsin(0.5*vec[3]);
			q[1]= -x1*dir[0];
			q[2]= -x1*dir[1];
			q[3]= -x1*dir[2];
			QuatMul(quat, q, quat);
			
			QuatToMat4(quat, mat);
		}
		
		VECCOPY(mat[3], vec);
		
	}
}

void ob_parlimb(Object *ob, Object *par, float mat[][4])
{	
	Ika *ika;
	Limb *li;
	float cmat[3][3], ang=0.0;
	int cur=0;
	
	/* in lokale ob space */
	Mat4One(mat);
	
	ika= par->data;
	li= ika->limbbase.first;
	while(li) {
		ang+= li->alpha;
		if(cur==ob->par1 || li->next==0) break;
		
		cur++;
		li= li->next;
	}
	
	mat[0][0]= fcos(ang);
	mat[1][0]= -fsin(ang);
	mat[0][1]= fsin(ang);
	mat[1][1]= fcos(ang);
	
	mat[3][0]= li->eff[0];
	mat[3][1]= li->eff[1];
	
}

void give_parvert(Object *par, int nr, float *vec)
{
	Mesh *me;
	MVert *mvert;
	EditVert *eve;
	extern ListBase editNurb;
	Nurb *nu;
	Curve *cu;
	BPoint *bp;
	DispList *dl;
	BezTriple *bezt;
	float *fp;
	int a, count;
	
	vec[0]=vec[1]=vec[2]= 0.0;
	
	if(par->type==OB_MESH) {
		if(par==G.obedit) {
			if(nr >= G.totvert) nr= 0;

			count= 0;
			eve= G.edve.first;
			while(eve) {
				if(count==nr) {
					memcpy(vec, eve->co, 12);
					break;
				}
				eve= eve->next;
				count++;
			}
		}
		else {
			me= par->data;
			if(nr >= me->totvert) nr= 0;
			
			/* is er deform */
			dl= find_displist(&par->disp, DL_VERTS);
			if(dl) {
				fp= dl->verts+3*nr;
				VECCOPY(vec, fp);
			}
			else {
				mvert= me->mvert + nr;
				VECCOPY(vec, mvert->co);
			}
		}
	}
	else if ELEM(par->type, OB_CURVE, OB_SURF) {

		cu= par->data;
		nu= cu->nurb.first;
		if(par==G.obedit) nu= editNurb.first;
		
		count= 0;
		while(nu) {
			if((nu->type & 7)==CU_BEZIER) {
				bezt= nu->bezt;
				a= nu->pntsu;
				while(a--) {
					if(count==nr) {
						VECCOPY(vec, bezt->vec[1]);
						break;
					}
					count++;
					bezt++;
				}
			}
			else {
				bp= nu->bp;
				a= nu->pntsu*nu->pntsv;
				while(a--) {
					if(count==nr) {
						memcpy(vec, bp->vec, 12);
						break;
					}
					count++;
					bp++;
				}
			}
			nu= nu->next;
		}

	}
	else if(par->type==OB_IKA) {
		Ika *ika= par->data;
		Limb *li= ika->limbbase.first;
		int cur= 1;
		if(nr) {
			while(li) {
				if(cur==nr || li->next==0) break;
				cur++;
				li= li->next;
			}
			vec[0]= li->eff[0];
			vec[1]= li->eff[1];
		}
	}
	else return;
}

void ob_parvert3(Object *ob, Object *par, float mat[][4])
{
	float cmat[3][3], v1[3], v2[3], v3[3], q[4];
	int a, count, par1, par2, par3;

	/* in lokale ob space */
	Mat4One(mat);
	
	if ELEM3(par->type, OB_MESH, OB_SURF, OB_CURVE) {
		
		give_parvert(par, ob->par1, v1);
		give_parvert(par, ob->par2, v2);
		give_parvert(par, ob->par3, v3);
				
		triatoquat(v1, v2, v3, q);
		QuatToMat3(q, cmat);
		Mat4CpyMat3(mat, cmat);
		
		if(ob->type==OB_CURVE) {
			VECCOPY(mat[3], v1);
		}
		else {
			VecAddf(mat[3], v1, v2);
			VecAddf(mat[3], mat[3], v3);
			VecMulf(mat[3], 0.3333333);
		}
	}
}

int no_parent_ipo=0;
void set_no_parent_ipo(int val)
{
	no_parent_ipo= val;
}

float timefac= 1.0;		/* 50 Hz, dtime:2 */

void set_dtime(int dtime)
{
	timefac= ((float)dtime)/2.0;
}

static int during_script_flag=0;

void disable_where_script(short on)
{
	during_script_flag= on;	
}

int during_script(void) {
	return during_script_flag;
}

void where_is_object_time(Object *ob, float ctime)
{
	Object *par;
	float *fp1, *fp2, tmat[4][4], obmat[4][4], totmat[4][4], slowmat[4][4];
	float stime, fac1, fac2, vec[3];
	int a, ok, pop;
	
	/* nieuwe versie: correcte parent+vertexparent en track+parent */
	/* deze berekent alleen de directe relatie met de parent en track */
	/* hij is sneller, maar moet wel de timeoffs in de gaten houden */
	
	if(ob==0) return;

	if( ctime != ob->ctime) {
		ob->ctime= ctime;
		
		if(ob->ipo) {
			
			stime= bsystem_time(ob, 0, ctime, 0.0);

			calc_ipo(ob->ipo, stime);
			execute_ipo((ID *)ob, ob->ipo);
		}			
	}

	if(ob->type==OB_IKA) {
		Ika *ika= ob->data;
		if(ika->parent) where_is_object_time(ika->parent, ctime);
	}

	if(ob->parent) {
		par= ob->parent;

		if(ob->ipoflag & OB_OFFS_PARENT) ctime-= ob->sf;
		
		pop= 0;
		if(no_parent_ipo==0 && ctime != par->ctime) {
		
			/* alleen voor ipo systemen? */
			pushdata(par, sizeof(Object));
			pop= 1;
			
			where_is_object_time(par, ctime);
		}
		
		object_to_mat4(ob, obmat);

		if(ob->partype & PARSLOW) Mat4CpyMat4(slowmat, ob->obmat);

		switch(ob->partype & PARTYPE) {
		case PAROBJECT:
			
			ok= 0;
			if(par->type==OB_CURVE) {
				if( ((Curve *)par->data)->flag & CU_PATH ) {
					ob_parcurve(ob, par, tmat);
					ok= 1;
				}
			}
			
			if(ok) Mat4MulSerie(totmat, par->obmat, tmat, 0);
			else Mat4CpyMat4(totmat, par->obmat);
			
			break;
			
		case PARLIMB:
			
			ob_parlimb(ob, par, tmat);
			
			Mat4MulSerie(totmat, par->obmat, tmat, 0);
			
			break;
			
		case PARVERT1:
			
			Mat4One(totmat);
			give_parvert(par, ob->par1, vec);
			VecMat4MulVecfl(totmat[3], par->obmat, vec);
			
			break;
		case PARVERT3:
			
			ob_parvert3(ob, par, tmat);
			
			Mat4MulSerie(totmat, par->obmat, tmat, 0);
			
			break;
			
		case PARSKEL:
			
			Mat4One(totmat);
			break;
		}
		
		/* totaal */
		Mat4MulSerie(tmat, totmat, ob->parentinv, 0);
		Mat4MulSerie(ob->obmat, tmat, obmat, 0);
	
		/* dit is een extern bruikbare originmat */
		Mat3CpyMat4(originmat, tmat);
		
		/* origin, voor hulplijntje */
		if( (ob->partype & 15)==PARSKEL ) {
			VECCOPY(ob->orig, par->obmat[3]);
		}
		else {
			VECCOPY(ob->orig, totmat[3]);
		}
		
		if(pop) {
			poplast(par);
		}
		
		if(ob->partype & PARSLOW) {
			
			/* framerate meetellen */
		
			fac1= timefac/(1.0+ fabs(ob->sf));
			if(fac1>=1.0) return;
			fac2= 1.0-fac1;
			
			fp1= ob->obmat[0];
			fp2= slowmat[0];
			for(a=0; a<16; a++, fp1++, fp2++) {
				fp1[0]= fac1*fp1[0] + fac2*fp2[0];
			}
		}
	}
	else {
		object_to_mat4(ob, ob->obmat);
	}
	
	if(ob->track) {
		float *quat;
		
		if( ctime != ob->track->ctime) where_is_object_time(ob->track, ctime);
		
		VecSubf(vec, ob->obmat[3], ob->track->obmat[3]);
		quat= vectoquat(vec, ob->trackflag, ob->upflag);
		QuatToMat3(quat, totmat);
		
		if(ob->parent && (ob->transflag & OB_POWERTRACK)) {
			/* 'tijdelijk' : parent info wissen */
			object_to_mat4(ob, tmat);
			tmat[0][3]= ob->obmat[0][3];
			tmat[1][3]= ob->obmat[1][3];
			tmat[2][3]= ob->obmat[2][3];
			tmat[3][0]= ob->obmat[3][0];
			tmat[3][1]= ob->obmat[3][1];
			tmat[3][2]= ob->obmat[3][2];
			tmat[3][3]= ob->obmat[3][3];
		}
		else Mat4CpyMat4(tmat, ob->obmat);
		
		Mat4MulMat34(ob->obmat, totmat, tmat);

	}

	if(!during_script() && ob->scriptlink.totscript) {
		do_pyscript(ob, SCRIPT_FRAMECHANGED);
	}
}



void where_is_object(Object *ob)
{
	
	/* deze zijn gememcopied */
	if(ob->flag & OB_FROMDUPLI) return;
	
	where_is_object_time(ob, F_CFRA);
}


void where_is_object_simul(Object *ob)
{
	Object *par;
	Ipo *ipo;
	float *fp1, *fp2, tmat[4][4], obmat[4][4], totmat[4][4], slowmat[4][4];
	float stime, fac1, fac2, vec[3];
	int a, ok;
	
	/* nieuwe versie: correcte parent+vertexparent en track+parent */
	/* deze berekent alleen de directe relatie met de parent en track */
	/* GEEN TIMEOFFS */
	
	/* geen ipo! (ivm dloc en realtime-ipos) */
	ipo= ob->ipo;
	ob->ipo= 0;

	if(ob->parent) {
		par= ob->parent;

		object_to_mat4(ob, obmat);

		if(ob->partype & PARSLOW) Mat4CpyMat4(slowmat, ob->obmat);

		switch(ob->partype & 15) {
		case PAROBJECT:
			
			ok= 0;
			if(par->type==OB_CURVE) {
				if( ((Curve *)par->data)->flag & CU_PATH ) {
					ob_parcurve(ob, par, tmat);
					ok= 1;
				}
			}
			
			if(ok) Mat4MulSerie(totmat, par->obmat, tmat, 0);
			else Mat4CpyMat4(totmat, par->obmat);
			
			break;
			
		case PARLIMB:
			
			ob_parlimb(ob, par, tmat);
			
			Mat4MulSerie(totmat, par->obmat, tmat, 0);
			
			break;
			
		case PARVERT1:
			
			Mat4One(totmat);
			VECCOPY(totmat[3], par->obmat[3]);
			
			break;
		case PARVERT3:
			
			ob_parvert3(ob, par, tmat);
			
			Mat4MulSerie(totmat, par->obmat, tmat, 0);
			
			break;
			
		case PARSKEL:
			
			Mat4One(totmat);
			break;
		}
		
		/* totaal */
		Mat4MulSerie(tmat, totmat, ob->parentinv, 0);
		Mat4MulSerie(ob->obmat, tmat, obmat, 0);
	
		if(ob->partype & PARSLOW) {
			fac1= 1.0/(1.0+ fabs(ob->sf));
			fac2= 1.0-fac1;
			fp1= ob->obmat[0];
			fp2= slowmat[0];
			for(a=0; a<16; a++, fp1++, fp2++) {
				fp1[0]= fac1*fp1[0] + fac2*fp2[0];
			}
		}
	}
	else {
		object_to_mat4(ob, ob->obmat);
	}
	
	if(ob->track) {
		float mat[3][3], up[3];
		
		VecSubf(vec, ob->obmat[3], ob->track->obmat[3]);
		up[0]= 0.0; up[1]= 1.0; up[2]= 0.0;
		VecUpMat3(vec, mat, 1);
		
		VECCOPY(ob->obmat[0], mat[0]);
		VECCOPY(ob->obmat[1], mat[1]);
		VECCOPY(ob->obmat[2], mat[2]);

	}
	
	/*  LET OP!!! */
	ob->ipo= ipo;
	
}


void what_does_parent1(Object *par, int partype, int par1, int par2, int par3)
{

	clear_workob();
	Mat4One(workob.parentinv);
	workob.parent= par;
	if(par) 
		workob.track= par->track;	/* LET OP: NIET ECHT NETJES */
	workob.partype= partype;
	workob.par1= par1;
	workob.par2= par2;
	workob.par3= par3;

	where_is_object(&workob);
}


void what_does_parent(Object *ob)
{

	clear_workob();
	Mat4One(workob.parentinv);
	workob.parent= ob->parent;
	workob.track= ob->track;

	workob.trackflag= ob->trackflag;
	workob.upflag= ob->upflag;
	
	workob.partype= ob->partype;
	workob.par1= ob->par1;
	workob.par2= ob->par2;
	workob.par3= ob->par3;

	where_is_object(&workob);
}

BoundBox *unit_boundbox()
{
	BoundBox *bb;
	
	bb= mallocN(sizeof(BoundBox), "bb");

	bb->vec[0][0]=bb->vec[1][0]=bb->vec[2][0]=bb->vec[3][0]= -1.0;
	bb->vec[4][0]=bb->vec[5][0]=bb->vec[6][0]=bb->vec[7][0]= 1.0;
	
	bb->vec[0][1]=bb->vec[1][1]=bb->vec[4][1]=bb->vec[5][1]= -1.0;
	bb->vec[2][1]=bb->vec[3][1]=bb->vec[6][1]=bb->vec[7][1]= 1.0;

	bb->vec[0][2]=bb->vec[3][2]=bb->vec[4][2]=bb->vec[7][2]= -1.0;
	bb->vec[1][2]=bb->vec[2][2]=bb->vec[5][2]=bb->vec[6][2]= 1.0;
	
	return bb;
}

void minmax_object(Object *ob, float *min, float *max)
{
	BoundBox bb;
	Mesh *me;
	Curve *cu;
	float vec[3];
	int a;
	
	switch(ob->type) {
		
	case OB_CURVE:
	case OB_FONT:
	case OB_SURF:
		cu= ob->data;
		
		if(cu->bb==0) tex_space_curve(cu);
		bb= *(cu->bb);
		
		for(a=0; a<8; a++) {
			Mat4MulVecfl(ob->obmat, bb.vec[a]);
			DO_MINMAX(bb.vec[a], min, max);
		}
		break;

	case OB_LIFE:
	case OB_MESH:
	case OB_SECTOR:
		me= get_mesh(ob);
		
		if(me) {
			if(me->bb==0) tex_space_mesh(me);
			bb= *(me->bb);
			
			for(a=0; a<8; a++) {
				Mat4MulVecfl(ob->obmat, bb.vec[a]);
				DO_MINMAX(bb.vec[a], min, max);
			}
			break;
		}
		/* hier geen break!!! */
		
	default:
		DO_MINMAX(ob->obmat[3], min, max);

		VECCOPY(vec, ob->obmat[3]);
		VecAddf(vec, vec, ob->size);
		DO_MINMAX(vec, min, max);

		VECCOPY(vec, ob->obmat[3]);
		VecSubf(vec, vec, ob->size);
		DO_MINMAX(vec, min, max);
		break;
	}
}



