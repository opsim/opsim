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


/*  material.c        MIX MODEL
 * 
 *  maart 95
 * 
 */

#include "blender.h"
#include "render.h"
#include "sector.h"

void free_material(Material *ma)
{
	int a;
	MTex *mtex;

	free_scriptlink(&ma->scriptlink);
	
	if(ma->ren) freeN(ma->ren);
	ma->ren= 0;
	
	for(a=0; a<8; a++) {
		mtex= ma->mtex[a];
		if(mtex && mtex->tex) mtex->tex->id.us--;
		if(mtex) freeN(mtex);
	}
}

void init_material(Material *ma)
{
	ma->lay= 1;
	ma->r= ma->g= ma->b= ma->ref= 0.8;
	ma->specr= ma->specg= ma->specb= 1.0;
	ma->mirr= ma->mirg= ma->mirb= 1.0;
	ma->amb= 0.5;
	ma->alpha= ma->add= 1.0;
	ma->spec= ma->hasize= 0.5;
	ma->har= 50;
	ma->starc= ma->ringc= 4;
	ma->linec= 12;
	ma->flarec= 1;
	ma->flaresize= ma->subsize= 1.0;
	
	ma->mode= MA_TRACEBLE+MA_SHADOW;	
}

Material *add_material(char *name)
{
	Material *ma;

	ma= alloc_libblock(&G.main->mat, ID_MA, name);
	
	init_material(ma);
	
	return ma;	
}

Material *copy_material(Material *ma)
{
	Material *man;
	int a;
	
	man= copy_libblock(ma);
	
	id_us_plus((ID *)man->ipo);
	
	for(a=0; a<8; a++) {
		if(ma->mtex[a]) {
			man->mtex[a]= mallocN(sizeof(MTex), "copymaterial");
			memcpy(man->mtex[a], ma->mtex[a], sizeof(MTex));
			id_us_plus((ID *)man->mtex[a]->tex);
		}
	}
	
	copy_scriptlink(&ma->scriptlink);
	
	return man;
}

void make_local_material(Material *ma)
{
	Object *ob;
	Mesh *me;
	Curve *cu;
	MetaBall *mb;
	Material *man;
	int a, local=0, lib=0;
	
	/* - zijn er alleen lib users: niet doen
	 * - zijn er alleen locale users: flag zetten
	 * - mixed: copy
	 */
	
	if(ma->id.lib==0) return;
	if(ma->id.us==1) {
		ma->id.lib= 0;
		ma->id.flag= LIB_LOCAL;
		new_id(0, (ID *)ma, 0);
		for(a=0; a<8; a++) {
			if(ma->mtex[a]) id_lib_extern((ID *)ma->mtex[a]->tex);
		}
		
		return;
	}
	
	/* test objects */
	ob= G.main->object.first;
	while(ob) {
		if(ob->mat) {
			for(a=0; a<ob->totcol; a++) {
				if(ob->mat[a]==ma) {
					if(ob->id.lib) lib= 1;
					else local= 1;
				}
			}
		}
		ob= ob->id.next;
	}
	/* test meshes */
	me= G.main->mesh.first;
	while(me) {
		if(me->mat) {
			for(a=0; a<me->totcol; a++) {
				if(me->mat[a]==ma) {
					if(me->id.lib) lib= 1;
					else local= 1;
				}
			}
		}
		me= me->id.next;
	}
	/* test curves */
	cu= G.main->curve.first;
	while(cu) {
		if(cu->mat) {
			for(a=0; a<cu->totcol; a++) {
				if(cu->mat[a]==ma) {
					if(cu->id.lib) lib= 1;
					else local= 1;
				}
			}
		}
		cu= cu->id.next;
	}
	/* test mballs */
	mb= G.main->mball.first;
	while(mb) {
		if(mb->mat) {
			for(a=0; a<mb->totcol; a++) {
				if(mb->mat[a]==ma) {
					if(mb->id.lib) lib= 1;
					else local= 1;
				}
			}
		}
		mb= mb->id.next;
	}
	
	if(local && lib==0) {
		ma->id.lib= 0;
		ma->id.flag= LIB_LOCAL;
		
		for(a=0; a<8; a++) {
			if(ma->mtex[a]) id_lib_extern((ID *)ma->mtex[a]->tex);
		}
		
		new_id(0, (ID *)ma, 0);
	}
	else if(local && lib) {
		man= copy_material(ma);
		man->id.us= 0;
		
		/* do objects */
		ob= G.main->object.first;
		while(ob) {
			if(ob->mat) {
				for(a=0; a<ob->totcol; a++) {
					if(ob->mat[a]==ma) {
						if(ob->id.lib==0) {
							ob->mat[a]= man;
							man->id.us++;
							ma->id.us--;
						}
					}
				}
			}
			ob= ob->id.next;
		}
		/* do meshes */
		me= G.main->mesh.first;
		while(me) {
			if(me->mat) {
				for(a=0; a<me->totcol; a++) {
					if(me->mat[a]==ma) {
						if(me->id.lib==0) {
							me->mat[a]= man;
							man->id.us++;
							ma->id.us--;
						}
					}
				}
			}
			me= me->id.next;
		}
		/* do curves */
		cu= G.main->curve.first;
		while(cu) {
			if(cu->mat) {
				for(a=0; a<cu->totcol; a++) {
					if(cu->mat[a]==ma) {
						if(cu->id.lib==0) {
							cu->mat[a]= man;
							man->id.us++;
							ma->id.us--;
						}
					}
				}
			}
			cu= cu->id.next;
		}
		/* do mballs */
		mb= G.main->mball.first;
		while(mb) {
			if(mb->mat) {
				for(a=0; a<mb->totcol; a++) {
					if(mb->mat[a]==ma) {
						if(mb->id.lib==0) {
							mb->mat[a]= man;
							man->id.us++;
							ma->id.us--;
						}
					}
				}
			}
			mb= mb->id.next;
		}
	}
}

Material ***give_matarar(Object *ob)
{
	Mesh *me;
	Curve *cu;
	MetaBall *mb;
	
	if(ob->type==OB_MESH) {
		me= ob->data;
		return &(me->mat);
	}
	else if(ob->type==OB_SECTOR) {
		Sector *se= ob->data;
		
		me= se->dynamesh;
		return &(me->mat);
	}
	else if(ob->type==OB_LIFE) {
		Life *lf= ob->data;
		
		me= lf->dynamesh;
		if(me==0) return 0;
		return &(me->mat);
	}
	else if ELEM3(ob->type, OB_CURVE, OB_FONT, OB_SURF) {
		cu= ob->data;
		return &(cu->mat);
	}
	else if(ob->type==OB_MBALL) {
		mb= ob->data;
		return &(mb->mat);
	}
	return 0;
}

short *give_totcolp(Object *ob)
{
	Mesh *me;
	Curve *cu;
	MetaBall *mb;
	
	if(ob->type==OB_MESH) {
		me= ob->data;
		return &(me->totcol);
	}
	else if(ob->type==OB_SECTOR) {
		Sector *se= ob->data;
		
		me= se->dynamesh;
		return &(me->totcol);
	}
	else if(ob->type==OB_LIFE) {
		Life *lf= ob->data;
		
		me= lf->dynamesh;
		if(me==0) return 0;
		return &(me->totcol);
	}
	else if ELEM3(ob->type, OB_CURVE, OB_FONT, OB_SURF) {
		cu= ob->data;
		return &(cu->totcol);
	}
	else if(ob->type==OB_MBALL) {
		mb= ob->data;
		return &(mb->totcol);
	}
	return 0;
}


Material *give_current_material(Object *ob, int act)
{
	Material ***matarar, *ma;
	Mesh *me;
	Curve *cu;
	MetaBall *mb;
	
	if(ob==0) return 0;
	if(ob->totcol==0) return 0;
	
	if(act>ob->totcol) act= ob->totcol;
	else if(act==0) act= 1;

	if( BTST(ob->colbits, act-1) ) {	/* aan object */
		ma= ob->mat[act-1];
	}
	else {								/* aan data */
		matarar= give_matarar(ob);
		
		if(matarar && *matarar) ma= (*matarar)[act-1];
		else ma= 0;
		
	}
	
	return ma;
}

ID *material_from(Object *ob, int act)
{

	if(ob==0) return 0;

	if(ob->totcol==0) return ob->data;
	if(act==0) act= 1;

	if( BTST(ob->colbits, act-1) ) return (ID *)ob;
	else if(ob->type==OB_SECTOR) {
		Sector *se= ob->data;
		
		return (ID *)se->dynamesh;
	}
	else if(ob->type==OB_LIFE) {
		Life *lf= ob->data;
		
		return (ID *)lf->dynamesh;
	}
	else return ob->data;
}

void test_object_materials(ID *id)
{
	/* ob mat-array evenlang maken als obdata mat-array */
	Object *ob;
	Mesh *me;
	Sector *se;
	Life *lf;
	Curve *cu;
	MetaBall *mb;
	Material **newmatar;
	int sectrue, totcol=0;

	if(id==0) return;

	if( GS(id->name)==ID_ME ) {
		me= (Mesh *)id;
		totcol= me->totcol;
	}
	else if( GS(id->name)==ID_CU ) {
		cu= (Curve *)id;
		totcol= cu->totcol;
	}
	else if( GS(id->name)==ID_MB ) {
		mb= (MetaBall *)id;
		totcol= mb->totcol;
	}
	else if( GS(id->name)==ID_SE ) {
		se= (Sector *)id;
		if(se->dynamesh) {
			totcol= se->dynamesh->totcol;
			id= (ID *)se->dynamesh;
		}
		else id= 0;

		
	}
	else if( GS(id->name)==ID_LF ) {
		lf= (Life *)id;
		if(lf->dynamesh) {
			totcol= lf->dynamesh->totcol;
			id= (ID *)lf->dynamesh;
		}
		else id= 0;
	}
	else return;

	ob= G.main->object.first;
	while(ob) {
		
		/* vervelende patch ivm sectors: */
		sectrue= 0;
		if(ob->type==OB_SECTOR) {
			se= ob->data;
			if(se->dynamesh==(Mesh *)id) sectrue= 1;
		}
		else if(ob->type==OB_LIFE) {
			lf= ob->data;
			if(lf->dynamesh==(Mesh *)id) sectrue= 1;
		}
		
		if(ob->data==id || sectrue) {
		
			if(totcol==0) {
				if(ob->totcol) {
					freeN(ob->mat);
					ob->mat= 0;
				}
			}
			else if(ob->totcol<totcol) {
				newmatar= callocN(sizeof(void *)*totcol, "newmatar");
				if(ob->totcol) {
					memcpy(newmatar, ob->mat, sizeof(void *)*ob->totcol);
					freeN(ob->mat);
				}
				ob->mat= newmatar;
			}
			ob->totcol= totcol;
			if(ob->totcol && ob->actcol==0) ob->actcol= 1;
			if(ob->actcol>ob->totcol) ob->actcol= ob->totcol;
		}
		ob= ob->id.next;
	}
}


void assign_material(Object *ob, Material *ma, int act)
{
	Material *mao, **matar, ***matarar;
	ID *id;
	short *totcolp;

	if(act>MAXMAT) return;
	if(act<1) act= 1;
	
	/* test arraylens */
	
	totcolp= give_totcolp(ob);
	matarar= give_matarar(ob);
	
	if(totcolp==0 || matarar==0) return;
	
	if( act > *totcolp) {
		matar= callocN(sizeof(void *)*act, "matarray1");
		if( *totcolp) {
			memcpy(matar, *matarar, sizeof(void *)*( *totcolp ));
			freeN(*matarar);
		}
		*matarar= matar;
		*totcolp= act;
	}
	
	if(act > ob->totcol) {
		matar= callocN(sizeof(void *)*act, "matarray2");
		if( ob->totcol) {
			memcpy(matar, ob->mat, sizeof(void *)*( ob->totcol ));
			freeN(ob->mat);
		}
		ob->mat= matar;
		ob->totcol= act;
	}
	
	/* doe 't */

	if( BTST(ob->colbits, act-1) ) {	/* aan object */
		mao= ob->mat[act-1];
		if(mao) mao->id.us--;
		ob->mat[act-1]= ma;
	}
	else {	/* aan data */
		mao= (*matarar)[act-1];
		if(mao) mao->id.us--;
		(*matarar)[act-1]= ma;
	}
	id_us_plus((ID *)ma);
	test_object_materials(ob->data);
}

void new_material_to_objectdata(Object *ob)
{
	Material *ma;
	
	if(ob==0) return;
	if(ob->totcol>=MAXMAT) return;
	
	ma= give_current_material(ob, ob->actcol);
	if(ma==0) {
		ma= add_material("Material");
		ma->id.us= 0;
	}
	
	if(ob->actcol) {
		if( BTST(ob->colbits, ob->actcol-1) ) {
			ob->colbits= BSET(ob->colbits, ob->totcol);
		}
	}
	
	assign_material(ob, ma, ob->totcol+1);
	ob->actcol= ob->totcol;
}


void replace_material(Material *old, Material *new)
{
	Material ***matarar;
	ID *id;
	Object *ob;
	int a;
	
	ob= G.main->object.first;
	while(ob) {
		if(ob->totcol && ob->id.lib==0) {
			matarar= give_matarar(ob);
			for(a=1; a<=ob->totcol; a++) {
				if(ob->mat[a-1] == old) {
					if(old) old->id.us--;
					id_us_plus((ID *)new);
					ob->mat[a-1]= new;
				}
				id= ob->data;
				if( (*matarar)[a-1] == old  && id->lib==0) {
					if(old) old->id.us--;
					id_us_plus((ID *)new);
					(*matarar)[a-1]= new;
				}
			}
		}
		ob= ob->id.next;
	}
}


void init_render_material(Material *ma)
{
	MTex *mtex;
	int a, traflag=0, needuv=0;
	
	if(ma->ren) return;

	if(ma->flarec==0) ma->flarec= 1;

	ma->ren= mallocN(sizeof(Material), "initrendermaterial");
	memcpy(ma->ren, ma, sizeof(Material));
	
	/* alle texcoflags van mtex adden */
	ma= ma->ren;
	ma->texco= 0;
	ma->mapto= 0;
	for(a=0; a<8; a++) {
		mtex= ma->mtex[a];
		if(mtex && mtex->tex) {
		
			/* force std. ref mapping for envmap */
			if(mtex->tex->type==TEX_ENVMAP) {
/* 				mtex->texco= TEXCO_REFL; */
/* 				mtex->projx= PROJ_X; */
/* 				mtex->projy= PROJ_Y; */
/* 				mtex->projz= PROJ_Z; */
/* 				mtex->mapping= MTEX_FLAT; */
			}
			/* hier niet testen op mtex->object en mtex->texco op TEXCO_ORCO zetten: mtex is linked! */
			
			ma->texco |= mtex->texco;
			ma->mapto |= mtex->mapto;
			if(R.osa) {
				if ELEM3(mtex->tex->type, TEX_IMAGE, TEX_PLUGIN, TEX_ENVMAP) ma->texco |= TEXCO_OSA;
			}
			if(mtex->mapto==MAP_ALPHA) traflag= 1;
			
			if(ma->texco & (511)) needuv= 1;
			
			if(mtex->object) mtex->object->flag |= OB_DO_IMAT;
			
		}
	}
	if(ma->mode & MA_ZTRA) {
		/* if(ma->alpha==0.0 || ma->alpha==1.0) */
		R.flag |= R_ZTRA;
	}
	if(ma->mode & MA_VERTEXCOLP) ma->mode |= MA_VERTEXCOL; 
	if(ma->mode & MA_VERTEXCOL) needuv= 1;
	
	if(needuv) ma->texco |= NEED_UV;

	ma->ambr= ma->amb*R.wrld.ambr;
	ma->ambg= ma->amb*R.wrld.ambg;
	ma->ambb= ma->amb*R.wrld.ambb;
	
}

void init_render_materials()
{
	Material *ma;
	
	ma= G.main->mat.first;
	while(ma) {
		if(ma->id.us) init_render_material(ma);
		ma= ma->id.next;
	}
	
}

void end_render_material(Material *ma)
{
	
	if(ma->ren) freeN(ma->ren);
	ma->ren= 0;

	if(ma->mode & MA_VERTEXCOLP) {
		ma->r= ma->g= ma->b= 1.0;
	}
}

void end_render_materials()
{
	Material *ma;
	
	ma= G.main->mat.first;
	while(ma) {
		if(ma->id.us) end_render_material(ma);
		ma= ma->id.next;
	}
	
}


/* ****************** */

char colname_array[125][20]= {
"Black","DarkRed","HalveRed","Red","Red",
"DarkGreen","DarkOlive","Brown","Chocolate","OrangeRed",
"HalveGreen","GreenOlive","DryOlive","Goldenrod","DarkOrange",
"LightGreen","Chartreuse","YellowGreen","Yellow","Gold",
"Green","LawnGreen","GreenYellow","LightOlive","Yellow",
"DarkBlue","DarkPurple","HotPink","VioletPink","RedPink",
"SlateGray","DarkGrey","PalePurple","IndianRed","Tomato",
"SeaGreen","PaleGreen","GreenKhaki","LightBrown","LightSalmon",
"SpringGreen","PaleGreen","MediumOlive","YellowBrown","LightGold",
"LightGreen","LightGreen","LightGreen","GreenYellow","PaleYellow",
"HalveBlue","DarkSky","HalveMagenta","VioletRed","DeepPink",
"SteelBlue","SkyBlue","Orchid","LightHotPink","HotPink",
"SeaGreen","SlateGray","MediumGrey","Burlywood","LightPink",
"SpringGreen","Aquamarine","PaleGreen","Khaki","PaleOrange",
"SpringGreen","SeaGreen","PaleGreen","PaleWhite","YellowWhite",
"LightBlue","Purple","MediumOrchid","Magenta","Magenta",
"RoyalBlue","SlateBlue","MediumOrchid","Orchid","Magenta",
"DeepSkyBlue","LightSteelBlue","LightSkyBlue","Violet","LightPink",
"Cyaan","DarkTurquoise","SkyBlue","Grey","Snow",
"Mint","Mint","Aquamarine","MintCream","Ivory",
"Blue","Blue","DarkMagenta","DarkOrchid","Magenta",
"SkyBlue","RoyalBlue","LightSlateBlue","MediumOrchid","Magenta",
"DodgerBlue","SteelBlue","MediumPurple","PalePurple","Plum",
"DeepSkyBlue","PaleBlue","LightSkyBlue","PalePurple","Thistle",
"Cyan","ColdBlue","PaleTurquoise","GhostWhite","White"
};

void automatname(Material *ma)
{
	int nr, r, g, b;
	float ref;
	
	if(ma==0) return;
	if(ma->mode & MA_SHLESS) ref= 1.0;
	else ref= ma->ref;
	
	r= 4.99*(ref*ma->r);
	g= 4.99*(ref*ma->g);
	b= 4.99*(ref*ma->b);
	nr= r + 5*g + 25*b;
	if(nr>124) nr= 124;
	new_id(&G.main->mat, (ID *)ma, colname_array[nr]);
	
}


void delete_material_index()
{
	Material *mao, **matar, ***matarar;
	ID *id;
	Object *ob, *obt;
	Mesh *me;
	Curve *cu;
	Nurb *nu;
	MFace *mface;
	short *totcolp;
	int a, actcol, sectrue;
	
	if(G.obedit) {
		error("Unable to perform function in EditMode");
		return;
	}
	ob= OBACT;
	if(ob==0 || ob->totcol==0) return;
	
	/* neem als uitgangspunt de mesh/curve/mball, verwijder 1 index, EN bij alle ob's 
	 * die ook zelfde ob->data hebben
	 * 
	 * Daarna ook indexen in mesh/curve/mball wijzigen!!!
	 */
	
	totcolp= give_totcolp(ob);
	matarar= give_matarar(ob);

	/* we deleten de actcol */
	if(ob->totcol) {
		mao= (*matarar)[ob->actcol-1];
		if(mao) mao->id.us--;
	}
	
	for(a=ob->actcol; a<ob->totcol; a++) {
		(*matarar)[a-1]= (*matarar)[a];
	}
	(*totcolp)--;
	
	if(*totcolp==0) {
		freeN(*matarar);
		*matarar= 0;
	}
	
	actcol= ob->actcol;
	obt= G.main->object.first;
	while(obt) {
	
		/* vervelende patch ivm sectors: */
		sectrue= 0;
		if(obt->type==OB_SECTOR) {
			Sector *se= obt->data;
			if(se->dynamesh==ob->data) sectrue= 1;
		}
		else if(obt->type==OB_LIFE) {
			Life *lf= obt->data;
			if(lf->dynamesh==ob->data) sectrue= 1;
		}
				
	
		if(obt->data==ob->data || sectrue) {
			
			/* LET OP: actcol hier niet van ob of van obt pakken (kan nul worden) */
			mao= obt->mat[actcol-1];
			if(mao) mao->id.us--;
		
			for(a=actcol; a<obt->totcol; a++) obt->mat[a-1]= obt->mat[a];
			obt->totcol--;
			if(obt->actcol > obt->totcol) obt->actcol= obt->totcol;
			
			if(obt->totcol==0) {
				freeN(obt->mat);
				obt->mat= 0;
			}
		}
		obt= obt->id.next;
	}
	allqueue(REDRAWBUTSMAT, 0);
	

	/* indexen van mesh goedzetten */

	if ELEM3(ob->type, OB_MESH, OB_SECTOR, OB_LIFE) {
		me= get_mesh(ob);
		mface= me->mface;
		a= me->totface;
		while(a--) {
			if(mface->mat_nr && mface->mat_nr>=actcol-1) mface->mat_nr--;
			mface++;
		}
	}
	else if ELEM(ob->type, OB_CURVE, OB_SURF) {
		cu= ob->data;
		nu= cu->nurb.first;
		
		while(nu) {
			if(nu->mat_nr && nu->mat_nr>=actcol-1) nu->mat_nr--;
			nu= nu->next;
		}
		makeDispList(ob);
	}
}

