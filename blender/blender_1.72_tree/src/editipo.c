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


/* editipo.c 		GRAPHICS
 * 
 * april 95
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "edit.h"
#include "ipo.h"
#include "ika.h"
#include "sequence.h"

#define IPOTHRESH	0.9

void transform_ipo(int mode);
void make_ipokey();
void free_ipokey(ListBase *lb);

extern int ob_ar[];
extern int ma_ar[];
extern int seq_ar[];
extern int cu_ar[];
extern int key_ar[];
extern int wo_ar[];
extern int la_ar[];
extern int cam_ar[];

void getname_ob_ei(int nr, char *str, int colipo)
{

	str[0]= 0;

	switch(nr) {
	case OB_LOC_X:
		strcpy(str, "LocX"); break;
	case OB_LOC_Y:
		strcpy(str, "LocY"); break;
	case OB_LOC_Z:
		strcpy(str, "LocZ"); break;
	case OB_DLOC_X:
		strcpy(str, "dLocX"); break;
	case OB_DLOC_Y:
		strcpy(str, "dLocY"); break;
	case OB_DLOC_Z:
		strcpy(str, "dLocZ"); break;

	case OB_ROT_X:
		strcpy(str, "RotX"); break;
	case OB_ROT_Y:
		strcpy(str, "RotY"); break;
	case OB_ROT_Z:
		strcpy(str, "RotZ"); break;
	case OB_DROT_X:
		strcpy(str, "dRotX"); break;
	case OB_DROT_Y:
		strcpy(str, "dRotY"); break;
	case OB_DROT_Z:
		strcpy(str, "dRotZ"); break;
		
	case OB_SIZE_X:
		strcpy(str, "SizeX"); break;
	case OB_SIZE_Y:
		strcpy(str, "SizeY"); break;
	case OB_SIZE_Z:
		strcpy(str, "SizeZ"); break;
	case OB_DSIZE_X:
		strcpy(str, "dSizeX"); break;
	case OB_DSIZE_Y:
		strcpy(str, "dSizeY"); break;
	case OB_DSIZE_Z:
		strcpy(str, "dSizeZ"); break;
	
	case OB_LAY:
		strcpy(str, "Layer"); break;

	case OB_TIME:
		strcpy(str, "Time"); break;
	case OB_EFF_X:
		if(colipo) strcpy(str, "ColR");
		else strcpy(str, "EffX");
		break;
	case OB_EFF_Y:
		if(colipo) strcpy(str, "ColG");
		else strcpy(str, "EffY");
		break;
	case OB_EFF_Z:
		if(colipo) strcpy(str, "ColB");
		else strcpy(str, "EffZ");
		break;
	}	

}

void getname_mat_ei(int nr, char *str)
{

	str[0]= 0;

	if(nr>=MA_MAP1) {
		switch( (nr & MA_MAP1-1) ) {
		case MAP_OFS_X:
			strcpy(str, "OfsX"); break;
		case MAP_OFS_Y:
			strcpy(str, "OfsY"); break;
		case MAP_OFS_Z:
			strcpy(str, "OfsZ"); break;
		case MAP_SIZE_X:
			strcpy(str, "SizeX"); break;
		case MAP_SIZE_Y:
			strcpy(str, "SizeY"); break;
		case MAP_SIZE_Z:
			strcpy(str, "SizeZ"); break;
		case MAP_R:
			strcpy(str, "texR"); break;
		case MAP_G:
			strcpy(str, "texG"); break;
		case MAP_B:
			strcpy(str, "texB"); break;
		case MAP_DVAR:
			strcpy(str, "DefVar"); break;
		case MAP_COLF:
			strcpy(str, "Col"); break;
		case MAP_NORF:
			strcpy(str, "Nor"); break;
		case MAP_VARF:
			strcpy(str, "Var"); break;
		}
	}
	else {
		switch(nr) {
		case MA_COL_R:
			strcpy(str, "R"); break;
		case MA_COL_G:
			strcpy(str, "G"); break;
		case MA_COL_B:
			strcpy(str, "B"); break;
		case MA_SPEC_R:
			strcpy(str, "SpecR"); break;
		case MA_SPEC_G:
			strcpy(str, "SpecG"); break;
		case MA_SPEC_B:
			strcpy(str, "SpecB"); break;
		case MA_MIR_R:
			strcpy(str, "MirR"); break;
		case MA_MIR_G:
			strcpy(str, "MirG"); break;
		case MA_MIR_B:
			strcpy(str, "MirB"); break;
		case MA_REF:
			strcpy(str, "Ref"); break;
		case MA_ALPHA:
			strcpy(str, "Alpha"); break;
		case MA_EMIT:
			strcpy(str, "Emit"); break;
		case MA_AMB:
			strcpy(str, "Amb"); break;
		case MA_SPEC:
			strcpy(str, "Spec"); break;
		case MA_HARD:
			strcpy(str, "Hard"); break;
		case MA_SPTR:
			strcpy(str, "SpTra"); break;
		case MA_ANG:
			strcpy(str, "Ang"); break;
		case MA_MODE:
			strcpy(str, "Mode"); break;
		case MA_HASIZE:
			strcpy(str, "HaSize"); break;
		}
	}
}

void getname_world_ei(int nr, char *str)
{

	str[0]= 0;

	if(nr>=MA_MAP1) {
		switch( (nr & MA_MAP1-1) ) {
		case MAP_OFS_X:
			strcpy(str, "OfsX"); break;
		case MAP_OFS_Y:
			strcpy(str, "OfsY"); break;
		case MAP_OFS_Z:
			strcpy(str, "OfsZ"); break;
		case MAP_SIZE_X:
			strcpy(str, "SizeX"); break;
		case MAP_SIZE_Y:
			strcpy(str, "SizeY"); break;
		case MAP_SIZE_Z:
			strcpy(str, "SizeZ"); break;
		case MAP_R:
			strcpy(str, "texR"); break;
		case MAP_G:
			strcpy(str, "texG"); break;
		case MAP_B:
			strcpy(str, "texB"); break;
		case MAP_DVAR:
			strcpy(str, "DefVar"); break;
		case MAP_COLF:
			strcpy(str, "Col"); break;
		case MAP_NORF:
			strcpy(str, "Nor"); break;
		case MAP_VARF:
			strcpy(str, "Var"); break;
		}
	}
	else {
		switch(nr) {
		case WO_HOR_R:
			strcpy(str, "HorR"); break;
		case WO_HOR_G:
			strcpy(str, "HorG"); break;
		case WO_HOR_B:
			strcpy(str, "HorB"); break;
		case WO_ZEN_R:
			strcpy(str, "ZenR"); break;
		case WO_ZEN_G:
			strcpy(str, "ZenG"); break;
		case WO_ZEN_B:
			strcpy(str, "ZenB"); break;

		case WO_EXPOS:
			strcpy(str, "Expos"); break;

		case WO_MISI:
			strcpy(str, "Misi"); break;
		case WO_MISTDI:
			strcpy(str, "MisDi"); break;
		case WO_MISTSTA:
			strcpy(str, "MisSta"); break;
		case WO_MISTHI:
			strcpy(str, "MisHi"); break;

		case WO_STAR_R:
			strcpy(str, "StarR"); break;
		case WO_STAR_G:
			strcpy(str, "StarB"); break;
		case WO_STAR_B:
			strcpy(str, "StarG"); break;

		case WO_STARDIST:
			strcpy(str, "StarDi"); break;
		case WO_STARSIZE:
			strcpy(str, "StarSi"); break;

		}
	}
}


void getname_seq_ei(int nr, char *str)
{

	str[0]= 0;

	switch(nr) {
	case SEQ_FAC1:
		strcpy(str, "Fac"); break;
	}
}

void getname_cu_ei(int nr, char *str)
{

	str[0]= 0;

	switch(nr) {
	case CU_SPEED:
		strcpy(str, "Speed"); break;
	}
}

void getname_key_ei(int nr, char *str)
{

	str[0]= 0;

	if(nr==KEY_SPEED) {
		strcpy(str, "Speed");
		return;
	}
	
	sprintf(str, "Key %d", nr);
}


void getname_la_ei(int nr, char *str)
{
	str[0]= 0;

	if(nr>=MA_MAP1) {
		switch( (nr & MA_MAP1-1) ) {
		case MAP_OFS_X:
			strcpy(str, "OfsX"); break;
		case MAP_OFS_Y:
			strcpy(str, "OfsY"); break;
		case MAP_OFS_Z:
			strcpy(str, "OfsZ"); break;
		case MAP_SIZE_X:
			strcpy(str, "SizeX"); break;
		case MAP_SIZE_Y:
			strcpy(str, "SizeY"); break;
		case MAP_SIZE_Z:
			strcpy(str, "SizeZ"); break;
		case MAP_R:
			strcpy(str, "texR"); break;
		case MAP_G:
			strcpy(str, "texG"); break;
		case MAP_B:
			strcpy(str, "texB"); break;
		case MAP_DVAR:
			strcpy(str, "DefVar"); break;
		case MAP_COLF:
			strcpy(str, "Col"); break;
		case MAP_NORF:
			strcpy(str, "Nor"); break;
		case MAP_VARF:
			strcpy(str, "Var"); break;
		}
	}
	else {
		switch(nr) {
		case LA_ENERGY:
			strcpy(str, "Energ"); break;
		case LA_COL_R:
			strcpy(str, "R"); break;
		case LA_COL_G:
			strcpy(str, "G"); break;
		case LA_COL_B:
			strcpy(str, "B"); break;
		case LA_DIST:
			strcpy(str, "Dist"); break;
		case LA_SPOTSI:
			strcpy(str, "SpoSi"); break;
		case LA_SPOTBL:
			strcpy(str, "SpoBl"); break;
		case LA_QUAD1:
			strcpy(str, "Quad1"); break;
		case LA_QUAD2:
			strcpy(str, "Quad2"); break;
		case LA_HALOINT:
			strcpy(str, "HaInt"); break;
		}
	}
}

void getname_cam_ei(int nr, char *str)
{

	str[0]= 0;

	switch(nr) {
	case CAM_LENS:
		strcpy(str, "Lens"); break;
	case CAM_STA:
		strcpy(str, "ClSta"); break;
	case CAM_END:
		strcpy(str, "ClEnd"); break;
	}
}


IpoCurve *find_ipocurve(Ipo *ipo, int adrcode)
{
	IpoCurve *icu;

	if(ipo==0) return 0;
	
	icu= ipo->curve.first;
	while(icu) {
		if(icu->adrcode==adrcode) return icu;
		icu= icu->next;
	}
	return 0;
}

void boundbox_ipocurve(IpoCurve *icu)
{
	BezTriple *bezt;
	BPoint *bp;
	float vec[3]={0.0,0.0,0.0};
	float min[3], max[3];
	int a;
	
	if(icu->totvert) {
		INIT_MINMAX(min, max);
		
		if(icu->bezt ) {
			a= icu->totvert;
			bezt= icu->bezt;
			while(a--) {
				if(icu->vartype & IPO_BITS) {
					vec[0]= bezt->vec[1][0];
					vec[1]= 0.0;
					DO_MINMAX(vec, min, max);
					
					vec[1]= 16.0;
					DO_MINMAX(vec, min, max);
				}
				else {
					if(icu->ipo==IPO_BEZ && a!=icu->totvert-1) {
						DO_MINMAX(bezt->vec[0], min, max);
					}
					DO_MINMAX(bezt->vec[1], min, max);
					if(icu->ipo==IPO_BEZ && a!=0) {
						DO_MINMAX(bezt->vec[2], min, max);
					}
				}
				
				bezt++;
			}
		}
		else if(icu->bp) {
			a= icu->totvert;
			bp= icu->bp;
			while(a--) {
				DO_MINMAX(bp->vec, min, max);
				bp++;
			}
		}
		if(min[0]==max[0]) max[0]= min[0]+1.0;
		if(min[1]==max[1]) max[1]= min[1]+0.1;
		
		icu->totrct.xmin= min[0];
		icu->totrct.ymin= min[1];
		icu->totrct.xmax= max[0];
		icu->totrct.ymax= max[1];
	}
	else {
		icu->totrct.xmin= icu->totrct.ymin= 0.0;
		icu->totrct.xmax= EFRA;
		icu->totrct.ymax= 1.0;
	}
}

void boundbox_ipo(Ipo *ipo, rctf *bb)
{
	IpoCurve *icu;
	int first= 1;
	
	icu= ipo->curve.first;
	while(icu) {
			
		boundbox_ipocurve(icu);
				
		if(first) {
			*bb= icu->totrct;
			first= 0;
		}
		else union_rctf(bb, &(icu->totrct));
		
		icu= icu->next;
	}
}



void editipo_changed(int doredraw)
{
	EditIpo *ei;
	View2D *v2d;
	Key *key;
	KeyBlock *kb;
	float dx, dy;
	int a, first=1;
	
	ei= G.sipo->editipo;
	if(ei==0) return;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		
		if(ei->icu) {
			
				/* 2 keer i.v.m. ittereren nieuwe autohandle */
			calchandles_ipocurve(ei->icu);
			calchandles_ipocurve(ei->icu);
			
			if(ei->flag & IPO_VISIBLE) {
		
				boundbox_ipocurve(ei->icu);
				sort_time_ipocurve(ei->icu);
				
				if(first) {
					G.sipo->v2d.tot= ei->icu->totrct;
					first= 0;
				}
				else union_rctf(&(G.sipo->v2d.tot), &(ei->icu->totrct));
			}
		}
	}

	v2d= &(G.sipo->v2d);
	
	/* keylijnen? */
	if(G.sipo->blocktype==ID_KE) {
		key= (Key *)G.sipo->from;
		if(key && key->block.first) {
			kb= key->block.first;
			if(kb->pos < v2d->tot.ymin) v2d->tot.ymin= kb->pos;
			kb= key->block.last;
			if(kb->pos > v2d->tot.ymax) v2d->tot.ymax= kb->pos;
		}
	}
	
	
	/* is er geen curve? */
	if(first) {
		v2d->tot.xmin= 0.0;
		v2d->tot.xmax= EFRA;
		v2d->tot.ymin= -0.1;
		v2d->tot.ymax= 1.1;
	
		if(G.sipo->blocktype==ID_SEQ) {
			v2d->tot.xmin= -5.0;
			v2d->tot.xmax= 105.0;
			v2d->tot.ymin= -0.1;
			v2d->tot.ymax= 1.1;
		}
	}
	
	G.sipo->tot= v2d->tot;	
	
	if(doredraw) {
		/* als do_ipo altijd wordt aangeroepen: problemen met insertkey, bijvoorbeeld
		 * als alleen een 'loc' wordt ge-insert wordt de 'ob->rot' veranderd.
		 */


		if(G.sipo->blocktype==ID_OB) { 			
				/* clear delta loc,rot,size (bij ipo vrijgeven/deleten) */
			clear_delta_obipo(G.sipo->ipo);
			
		}
	
		do_ipo(G.sipo->ipo);

		addqueue(curarea->headwin, REDRAW, 1);
		addqueue(curarea->win, REDRAW, 1);
		allqueue(REDRAWBUTSANIM, 0);
		
		if(G.sipo->blocktype==ID_OB) {
			Object *ob= (Object *)G.sipo->from;			
			if(ob && ob->type==OB_IKA) itterate_ika(ob);
			allqueue(REDRAWVIEW3D, 0);
		}
		else if(G.sipo->blocktype==ID_MA) allqueue(REDRAWBUTSMAT, 0);
		else if(G.sipo->blocktype==ID_WO) allqueue(REDRAWBUTSWORLD, 0);
		else if(G.sipo->blocktype==ID_LA) allqueue(REDRAWBUTSLAMP, 0);
		else if(G.sipo->blocktype==ID_CA) {
			allqueue(REDRAWBUTSEDIT, 0);
			allqueue(REDRAWVIEW3D, 0);
		}
		else if(G.sipo->blocktype==ID_SEQ) clear_last_seq();
		else if(G.sipo->blocktype==ID_KE) {
			do_spec_key((Key *)G.sipo->from);
			allqueue(REDRAWVIEW3D, 0);
		}
		else if(G.sipo->blocktype==ID_CU) {
			calc_curvepath(OBACT);
			allqueue(REDRAWVIEW3D, 0);
		}
	}

	if(G.sipo->showkey) make_ipokey();
	
}

void scale_editipo()
{
	/* komt uit buttons, scale met G.sipo->tot rect */
	
	EditIpo *ei;
	BezTriple *bezt;
	float facx, facy;
	int a, b;	
	
	facx= (G.sipo->tot.xmax-G.sipo->tot.xmin)/(G.sipo->v2d.tot.xmax-G.sipo->v2d.tot.xmin);
	facy= (G.sipo->tot.ymax-G.sipo->tot.ymin)/(G.sipo->v2d.tot.ymax-G.sipo->v2d.tot.ymin);
	
	ei= G.sipo->editipo;
	if(ei==0) return;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
			bezt= ei->icu->bezt;
			b= ei->icu->totvert;
			while(b--) {
				
				bezt->vec[0][0]= facx*(bezt->vec[0][0] - G.sipo->v2d.tot.xmin) + G.sipo->tot.xmin;
				bezt->vec[1][0]= facx*(bezt->vec[1][0] - G.sipo->v2d.tot.xmin) + G.sipo->tot.xmin;
				bezt->vec[2][0]= facx*(bezt->vec[2][0] - G.sipo->v2d.tot.xmin) + G.sipo->tot.xmin;
			
				bezt->vec[0][1]= facy*(bezt->vec[0][1] - G.sipo->v2d.tot.ymin) + G.sipo->tot.ymin;
				bezt->vec[1][1]= facy*(bezt->vec[1][1] - G.sipo->v2d.tot.ymin) + G.sipo->tot.ymin;
				bezt->vec[2][1]= facy*(bezt->vec[2][1] - G.sipo->v2d.tot.ymin) + G.sipo->tot.ymin;

				bezt++;
			}
		}
	}
	editipo_changed(1);
	allqueue(REDRAWIPO, 0);
}


Ipo *get_ipo_to_edit(ID **from)
{
	extern Sequence *last_seq;
	Object *ob;
	Material *ma;
	Curve *cu;
	Key *key;
	Lamp *la;
	World *wo;
	Camera *ca;
	
	*from= 0;
	
	ob= OBACT;
	
	if(G.sipo->blocktype==ID_OB) {
		if(ob==0) return 0;
		*from= (ID *)ob;
		return ob->ipo;
	}
	else if(G.sipo->blocktype==ID_MA) {
		if(ob==0) return 0;
		ma= give_current_material(ob, ob->actcol);
		*from= (ID *)ma;
		if(ma) return ma->ipo;
	}
	else if(G.sipo->blocktype==ID_KE) {
		if(ob==0) return 0;
		key= give_current_key(ob);
		*from= (ID *)key;
		if(key) return key->ipo;
	}
	else if(G.sipo->blocktype==ID_SEQ) {
		*from= (ID *)last_seq;
		if(last_seq) return last_seq->ipo;
	}
	else if(G.sipo->blocktype==ID_CU) {
		if(ob==0) return 0;
		if(ob->type!=OB_CURVE) return 0;
		cu= ob->data;
		*from= (ID *)cu;
		return cu->ipo;
	}
	else if(G.sipo->blocktype==ID_WO) {
		wo= G.scene->world;
		*from= (ID *)wo;
		if(wo) return wo->ipo;
	}
	else if(G.sipo->blocktype==ID_LA) {
		if(ob==0) return 0;
		if(ob->type!=OB_LAMP) return 0;
		la= ob->data;
		*from= (ID *)la;
		return la->ipo;
	}
	else if(G.sipo->blocktype==ID_CA) {
		if(ob==0) return 0;		
		if(ob->type!=OB_CAMERA) return 0;
		ca= ob->data;
		*from= (ID *)ca;
		if(ca) return ca->ipo;
	}

	return 0;	
}

uint ipo_rainbow(int cur, int tot)
{
	float fac, dfac, sat;

	dfac= 1.0/( (float)tot+1.0);

	/* deze berekening zorgt voor twee verschillende cycles regenboogkleuren */
	if(cur< tot/2) fac= cur*2.0*dfac;
	else fac= (cur-tot/2)*2.0*dfac +dfac;
	sat= 0.5;
	if(fac>0.5 && fac<0.8) sat= 0.4;
	
	return hsv_to_cpack(fac, sat, 1.0);
}		


void make_ob_editipo(Object *ob, SpaceIpo *si)
{
	EditIpo *ei;
	int a, len, colipo;
	
	ei= si->editipo= callocN(OB_TOTIPO*sizeof(EditIpo), "editipo");
	
	si->totipo= OB_TOTIPO;
	if ELEM(ob->type, OB_SECTOR, OB_LIFE) colipo= 1;
	else colipo= 0;
	
	for(a=0; a<OB_TOTIPO; a++) {
		getname_ob_ei(ob_ar[a], ei->name, colipo);
		ei->adrcode= ob_ar[a];
		
		if ELEM6(ei->adrcode, OB_ROT_X, OB_ROT_Y, OB_ROT_Z, OB_DROT_X, OB_DROT_Y, OB_DROT_Z) ei->disptype= IPO_DISPDEGR;
		else if(ei->adrcode==OB_LAY) ei->disptype= IPO_DISPBITS;
		else if(ei->adrcode==OB_TIME) ei->disptype= IPO_DISPTIME;

		ei->col= ipo_rainbow(a, OB_TOTIPO);

		if(colipo) {
			len= strlen(ei->name);
			if(len) {
				if( ei->name[ len-1 ]=='R') ei->col= 0x5050FF;
				else if( ei->name[ len-1 ]=='G') ei->col= 0x50FF50;
				else if( ei->name[ len-1 ]=='B') ei->col= 0xFF7050;
			}
		}
		
		ei->icu= find_ipocurve(si->ipo, ei->adrcode);
		if(ei->icu) {
			ei->flag= ei->icu->flag;
		}
		
		ei++;
	}
}

void make_seq_editipo(SpaceIpo *si)
{
	EditIpo *ei;
	int a;
	
	ei= si->editipo= callocN(SEQ_TOTIPO*sizeof(EditIpo), "editipo");
	
	si->totipo= SEQ_TOTIPO;
	
	
	for(a=0; a<SEQ_TOTIPO; a++) {
		getname_seq_ei(seq_ar[a], ei->name);
		ei->adrcode= seq_ar[a];
		
		ei->col= ipo_rainbow(a, SEQ_TOTIPO);
		
		ei->icu= find_ipocurve(si->ipo, ei->adrcode);
		if(ei->icu) {
			ei->flag= ei->icu->flag;
		}
		else ei->flag |= IPO_VISIBLE;
		
		ei++;
	}
}

void make_cu_editipo(SpaceIpo *si)
{
	EditIpo *ei;
	int a;
	
	ei= si->editipo= callocN(CU_TOTIPO*sizeof(EditIpo), "editipo");
	
	si->totipo= CU_TOTIPO;
	
	
	for(a=0; a<CU_TOTIPO; a++) {
		getname_cu_ei(cu_ar[a], ei->name);
		ei->adrcode= cu_ar[a];
		
		ei->col= ipo_rainbow(a, CU_TOTIPO);
		
		ei->icu= find_ipocurve(si->ipo, ei->adrcode);
		if(ei->icu) {
			ei->flag= ei->icu->flag;
		}
		else ei->flag |= IPO_VISIBLE;
		 
		ei++;
	}
}

void make_key_editipo(SpaceIpo *si)
{
	Key *key;
	EditIpo *ei;
	int a;
	
	ei= si->editipo= callocN(KEY_TOTIPO*sizeof(EditIpo), "editipo");
	
	si->totipo= KEY_TOTIPO;
	
	for(a=0; a<KEY_TOTIPO; a++) {
		getname_key_ei(key_ar[a], ei->name);
		ei->adrcode= key_ar[a];
		
		ei->col= ipo_rainbow(a, KEY_TOTIPO);
		
		ei->icu= find_ipocurve(si->ipo, ei->adrcode);
		if(ei->icu) {
			ei->flag= ei->icu->flag;
		}
		else if(a==0) ei->flag |= IPO_VISIBLE;
		 
		ei++;
	}
	
	ei= si->editipo;
	key= (Key *)G.sipo->from;
	if(key && key->type==KEY_RELATIVE) {
		strcpy(ei->name, "----");
	}
	else {
		ei->flag |= IPO_VISIBLE;
	}
}

int texchannel_to_adrcode(int channel)
{

	switch(channel) {
		case 0: return MA_MAP1;
		case 1: return MA_MAP2; 
		case 2: return MA_MAP3; 
		case 3: return MA_MAP4; 
		case 4: return MA_MAP5; 
		case 5: return MA_MAP6; 
		case 6: return MA_MAP7; 
		case 7: return MA_MAP8; 
	}
	return 0;
}

void make_mat_editipo(SpaceIpo *si)
{
	EditIpo *ei;
	float fac, dfac, sat;
	int a, len;
	
	if(si->from==0) return;
	
	ei= si->editipo= callocN(MA_TOTIPO*sizeof(EditIpo), "editipo");
	
	si->totipo= MA_TOTIPO;
	
	dfac= 1.0/(MA_TOTIPO+1);
	fac= 0.0;
	
	for(a=0; a<MA_TOTIPO; a++) {
		getname_mat_ei(ma_ar[a], ei->name);
		ei->adrcode= ma_ar[a];
		
		if(ei->adrcode & MA_MAP1) {
			ei->adrcode-= MA_MAP1;
			ei->adrcode |= texchannel_to_adrcode(si->channel);
		}
		else {
			/* dit was weggecommentaard. Waarom? */
			if(ei->adrcode==MA_MODE) ei->disptype= IPO_DISPBITS;
		}
		
		ei->col= ipo_rainbow(a, WO_TOTIPO);
		
		len= strlen(ei->name);
		if(len) {
			if( ei->name[ len-1 ]=='R') ei->col= 0x5050FF;
			else if( ei->name[ len-1 ]=='G') ei->col= 0x50FF50;
			else if( ei->name[ len-1 ]=='B') ei->col= 0xFF7050;
		}
		
		ei->icu= find_ipocurve(si->ipo, ei->adrcode);
		if(ei->icu) {
			ei->flag= ei->icu->flag;
		}
		
		ei++;
	}
}

void make_world_editipo(SpaceIpo *si)
{
	EditIpo *ei;
	float fac, dfac, sat;
	int a, len;
	
	if(si->from==0) return;
	
	ei= si->editipo= callocN(WO_TOTIPO*sizeof(EditIpo), "editipo");
	
	si->totipo= WO_TOTIPO;
	
	dfac= 1.0/(WO_TOTIPO+1);
	fac= 0.0;
	
	for(a=0; a<WO_TOTIPO; a++) {
		getname_world_ei(wo_ar[a], ei->name);
		ei->adrcode= wo_ar[a];
		
		if(ei->adrcode & MA_MAP1) {
			ei->adrcode-= MA_MAP1;
			ei->adrcode |= texchannel_to_adrcode(si->channel);
		}
		else {
			if(ei->adrcode==MA_MODE) ei->disptype= IPO_DISPBITS;
		}
		
		ei->col= ipo_rainbow(a, MA_TOTIPO);
		
		len= strlen(ei->name);
		if(len) {
			if( ei->name[ len-1 ]=='R') ei->col= 0x5050FF;
			else if( ei->name[ len-1 ]=='G') ei->col= 0x50FF50;
			else if( ei->name[ len-1 ]=='B') ei->col= 0xFF7050;
		}
		
		ei->icu= find_ipocurve(si->ipo, ei->adrcode);
		if(ei->icu) {
			ei->flag= ei->icu->flag;
		}
		
		ei++;
	}
}

void make_lamp_editipo(SpaceIpo *si)
{
	EditIpo *ei;
	int a;
	
	ei= si->editipo= callocN(LA_TOTIPO*sizeof(EditIpo), "editipo");
	
	si->totipo= LA_TOTIPO;
	
	
	for(a=0; a<LA_TOTIPO; a++) {
		getname_la_ei(la_ar[a], ei->name);
		ei->adrcode= la_ar[a];

		if(ei->adrcode & MA_MAP1) {
			ei->adrcode-= MA_MAP1;
			ei->adrcode |= texchannel_to_adrcode(si->channel);
		}

		ei->col= ipo_rainbow(a, LA_TOTIPO);
		
		ei->icu= find_ipocurve(si->ipo, ei->adrcode);
		if(ei->icu) {
			ei->flag= ei->icu->flag;
		}
		
		ei++;
	}
}

void make_camera_editipo(SpaceIpo *si)
{
	EditIpo *ei;
	int a;
	
	ei= si->editipo= callocN(CAM_TOTIPO*sizeof(EditIpo), "editipo");
	
	si->totipo= CAM_TOTIPO;
	
	
	for(a=0; a<CAM_TOTIPO; a++) {
		getname_cam_ei(cam_ar[a], ei->name);
		ei->adrcode= cam_ar[a];

		ei->col= ipo_rainbow(a, CAM_TOTIPO);
		
		ei->icu= find_ipocurve(si->ipo, ei->adrcode);
		if(ei->icu) {
			ei->flag= ei->icu->flag;
		}
		
		ei++;
	}
}

void make_editipo()
{
	EditIpo *ei;
	Object *ob;
	ID *from;
	rctf *rf;
	int a;

	if(G.sipo->editipo) freeN(G.sipo->editipo);
	G.sipo->editipo= 0;
	G.sipo->totipo= 0;
	ob= OBACT;

	G.sipo->ipo= get_ipo_to_edit(&from);
	G.sipo->from= from;
	if(G.sipo->ipo) G.sipo->showkey= G.sipo->ipo->showkey;
	
	if(G.sipo->blocktype==ID_OB) {
		if(ob==0) return;
		ob->ipowin= ID_OB;
		make_ob_editipo(ob, G.sipo);
	}
	else if(G.sipo->blocktype==ID_MA) {
		if(ob==0) return;
		ob->ipowin= ID_MA;
		make_mat_editipo(G.sipo);
	}
	else if(G.sipo->blocktype==ID_SEQ) make_seq_editipo(G.sipo);
	else if(G.sipo->blocktype==ID_CU) {
		if(ob==0) return;
		ob->ipowin= ID_CU;
		make_cu_editipo(G.sipo);
	}
	else if(G.sipo->blocktype==ID_KE) {
		if(ob==0) return;
		ob->ipowin= ID_KE;
		make_key_editipo(G.sipo);
	}
	else if(G.sipo->blocktype==ID_WO) {
		make_world_editipo(G.sipo);
	}
	else if(G.sipo->blocktype==ID_LA) {
		if(ob==0) return;
		ob->ipowin= ID_LA;
		make_lamp_editipo(G.sipo);
	}
	else if(G.sipo->blocktype==ID_CA) {
		if(ob==0) return;
		ob->ipowin= ID_CA;
		make_camera_editipo(G.sipo);
	}
	
	if(G.sipo->editipo==0) return;
	
	/* rowbut voor VISIBLE select */
	G.sipo->rowbut= 0;
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		
		if(ei->flag & IPO_VISIBLE) G.sipo->rowbut |= (1<<a);
		
		if(ei->icu) ei->icu->flag= ei->flag;
	}
	editipo_changed(0);

	if(G.sipo->ipo) {
		rf= &(G.sipo->ipo->cur);
		if(rf->xmin<rf->xmax && rf->ymin<rf->ymax) G.v2d->cur= *rf;

	}
	else {
		if(G.sipo->blocktype==ID_OB) {
			G.v2d->cur.xmin= 0.0;
			G.v2d->cur.xmax= EFRA;
			G.v2d->cur.ymin= -5.0;
			G.v2d->cur.ymax= +5.0;
		}
		else if(G.sipo->blocktype==ID_CA) {
			G.v2d->cur.xmin= 0.0;
			G.v2d->cur.xmax= EFRA;
			G.v2d->cur.ymin= 0.0;
			G.v2d->cur.ymax= 100.0;
		}
		else if ELEM4(G.sipo->blocktype, ID_MA, ID_CU, ID_WO, ID_LA) {
			G.v2d->cur.xmin= -0.1;
			G.v2d->cur.xmax= EFRA;
			G.v2d->cur.ymin= -0.1;
			G.v2d->cur.ymax= +1.1;
		}
		else if(G.sipo->blocktype==ID_SEQ) {
			G.v2d->cur.xmin= -5.0;
			G.v2d->cur.xmax= 105.0;
			G.v2d->cur.ymin= -0.1;
			G.v2d->cur.ymax= +1.1;
		}
		else if(G.sipo->blocktype==ID_KE) {
			G.v2d->cur.xmin= -0.1;
			G.v2d->cur.xmax= EFRA;
			G.v2d->cur.ymin= -0.1;
			G.v2d->cur.ymax= +2.1;
		}
	}
}

void test_editipo()
{
	Ipo *ipo;
	ID *from;
	
	if(G.sipo->editipo==0) make_editipo();
	else {
		
		ipo= get_ipo_to_edit(&from);
		if(G.sipo->ipo != ipo || G.sipo->from!=from) make_editipo();
		
	}
	/* kopie current view */
	if(G.sipo->ipo) G.sipo->ipo->cur= G.v2d->cur;
}

/* ****************************************** */

int totipo_edit, totipo_sel, totipo_vis, totipo_vert, totipo_vertsel, totipo_key, totipo_keysel;

void get_status_editipo()
{
	EditIpo *ei;
	IpoKey *ik;
	BezTriple *bezt;
	BPoint *bp;
	int a, b;
	
	totipo_vis= 0;
	totipo_sel= 0;
	totipo_edit= 0;
	totipo_vert= 0;
	totipo_vertsel= 0;
	totipo_key= 0;
	totipo_keysel= 0;
	
	if(G.sipo->ipo && G.sipo->ipo->id.lib) return;
	
	ei= G.sipo->editipo;
	if(ei==0) return;
	for(a=0; a<G.sipo->totipo; a++) {
		if( ei->flag & IPO_VISIBLE ) {
			totipo_vis++;
			if(ei->flag & IPO_SELECT) totipo_sel++;
			if(G.sipo->showkey || (ei->flag & IPO_EDIT)) {
				
				/* als showkey: wel de vertices tellen (voor grab) */
				if(G.sipo->showkey==0) totipo_edit++;
				
				if(ei->icu) {
					if(ei->icu->bezt) {
						bezt= ei->icu->bezt;
						b= ei->icu->totvert;
						while(b--) {
							if(ei->icu->ipo==IPO_BEZ) {
								if(bezt->f1 & 1) totipo_vertsel++;
								if(bezt->f3 & 1) totipo_vertsel++;
								totipo_vert+= 2;
							}
							if(bezt->f2 & 1) totipo_vertsel++;
							
							totipo_vert++;
							bezt++;
						}
					}
					else if(ei->icu->bp) {
						bp= ei->icu->bp;
						b= ei->icu->totvert;
						while(b--) {
							if(bp->f1 & 1) totipo_vertsel++;
							totipo_vert++;
							bp++;
						}
					}
				}
			}
		}
		ei++;
	}
	
	if(G.sipo->showkey) {
		ik= G.sipo->ipokey.first;
		while(ik) {
			totipo_key++;
			if(ik->flag & 1) totipo_keysel++;
			ik= ik->next;
		}
	}
}



void update_editipo_flags()
{
	EditIpo *ei;
	IpoKey *ik;
	uint flag;
	int a;
	
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		ei->flag &= ~IPO_VISIBLE;
		flag= (1<<a);
		if( G.sipo->rowbut & flag ) ei->flag |= IPO_VISIBLE;
		
		if(ei->icu) ei->icu->flag= ei->flag;
		
	}
	
	if(G.sipo->showkey) {
		ik= G.sipo->ipokey.first;
		while(ik) {
			for(a=0; a<G.sipo->totipo; a++) {
				if(ik->data[a]) {
					if(ik->flag & 1) {
						ik->data[a]->f1 |= 1;
						ik->data[a]->f2 |= 1;
						ik->data[a]->f3 |= 1;
					}
					else {
						ik->data[a]->f1 &= ~1;
						ik->data[a]->f2 &= ~1;
						ik->data[a]->f3 &= ~1;
					}
				}
			}
			ik= ik->next;
		}
	}
}

void set_editflag_editipo()
{
	EditIpo *ei;
	int a, tot= 0, ok= 0;
	
	/* van showkey direkt door naar editen geselecteerde punten */
	if(G.sipo->showkey) {
		G.sipo->showkey= 0;
		if(G.sipo->ipo) G.sipo->ipo->showkey= 0;
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++, ei++) ei->flag |= IPO_SELECT;
		addqueue(curarea->headwin, REDRAW, 1);
		allqueue(REDRAWVIEW3D, 0);
	}
	
	get_status_editipo();
	
	if(G.sipo->ipo && G.sipo->ipo->id.lib) return;
	
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {		
		if(ei->icu) {
			if(ei->flag & IPO_VISIBLE) {
				
				if(totipo_edit==0 && (ei->flag & IPO_SELECT)) {
					ei->flag |= IPO_EDIT;
					ei->icu->flag= ei->flag;
				}
				else if(totipo_edit && (ei->flag & IPO_EDIT)) {
					ei->flag -= IPO_EDIT;
					ei->icu->flag= ei->flag;
				}
				else if(totipo_vis==1) {
					if(ei->flag & IPO_EDIT) ei->flag -= IPO_EDIT;
					else ei->flag |= IPO_EDIT;
					ei->icu->flag= ei->flag;
				}
			}
		}
	}
	
	addqueue(curarea->win, REDRAW, 1);
}

void swap_selectall_editipo()
{
	Object *ob;
	EditIpo *ei;
	IpoKey *ik;
	BezTriple *bezt;
	BPoint *bp;
	int a, b, sel=0;
	
	deselectall_key();

	get_status_editipo();
	
	if(G.sipo->showkey) {
		ik= G.sipo->ipokey.first;
		while(ik) {
			if(totipo_vertsel) ik->flag &= ~1;
			else ik->flag |= 1;
			ik= ik->next;
		}
		update_editipo_flags();

		if(G.sipo->showkey && G.sipo->blocktype==ID_OB ) {
			ob= OBACT;
			if(ob && (ob->ipoflag & OB_DRAWKEY)) draw_object_ext(BASACT);
		}
	}
	else if(totipo_edit==0) {
		
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++) {
			if( ei->flag & IPO_VISIBLE ) {
				if(totipo_sel) ei->flag &= ~IPO_SELECT;
				else ei->flag |= IPO_SELECT;
			}
			ei++;
		}
		update_editipo_flags();
	}
	else {
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++) {
			if ISPOIN3(ei, flag & IPO_VISIBLE, flag & IPO_EDIT, icu ) {
				bezt= ei->icu->bezt;
				bp= ei->icu->bp;
				if(bezt) {
					b= ei->icu->totvert;
					while(b--) {
						if(totipo_vertsel) {
							bezt->f1= bezt->f2= bezt->f3= 0;
						}
						else {
							bezt->f1= bezt->f2= bezt->f3= 1;
						}
						bezt++;
					}
				}
				else if(bp) {
					b= ei->icu->totvert;
					while(b--) {
						if(totipo_vertsel) bp->f1= 0;
						else bp->f1= 0;
						bp++;
					}
				}
			}
			ei++;
		}
		
	}
	
	addqueue(curarea->win, REDRAW, 1);
}

void swap_visible_editipo()
{
	EditIpo *ei;
	Object *ob;
	int a, sel=0;
	
	get_status_editipo();
	
	G.sipo->rowbut= 0;
	
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++) {
		if(totipo_vis==0) {
			if(ei->icu) {
				ei->flag |= IPO_VISIBLE;
				G.sipo->rowbut |= (1<<a);
			}
		}
		else ei->flag &= ~IPO_VISIBLE;
		ei++;
	}
	
	update_editipo_flags();
	
	if(G.sipo->showkey) {
		
		make_ipokey();
		
		ob= OBACT;
		if(ob && (ob->ipoflag & OB_DRAWKEY)) allqueue(REDRAWVIEW3D, 0);
	}

	addqueue(curarea->win, REDRAW, 1);
	
}

void deselectall_editipo()
{
	EditIpo *ei;
	IpoKey *ik;
	BezTriple *bezt;
	BPoint *bp;
	int a, b, sel=0;
	
	deselectall_key();

	get_status_editipo();
	
	if(G.sipo->showkey) {
		ik= G.sipo->ipokey.first;
		while(ik) {
			ik->flag &= ~1;
			ik= ik->next;
		}
		update_editipo_flags();

	}
	else if(totipo_edit==0) {
		
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++) {
			if( ei->flag & IPO_VISIBLE ) {
				ei->flag &= ~IPO_SELECT;
			}
			ei++;
		}
		update_editipo_flags();
	}
	else {
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++) {
			if ISPOIN3(ei, flag & IPO_VISIBLE, flag & IPO_EDIT, icu ) {
				bezt= ei->icu->bezt;
				bp= ei->icu->bp;
				if(bezt) {
					b= ei->icu->totvert;
					while(b--) {
						bezt->f1= bezt->f2= bezt->f3= 0;
						bezt++;
					}
				}
				else if(bp) {
					b= ei->icu->totvert;
					while(b--) {
						bp->f1= 0;
						bp++;
					}
				}
			}
			ei++;
		}
	}
	
	addqueue(curarea->win, REDRAW, 1);
}

short findnearest_ipovert(IpoCurve **icu, BezTriple **bezt, BPoint **bp)
{
	/* selected krijgen een nadeel */
	/* in icu en (bezt of bp) wordt nearest weggeschreven */
	/* return 0 1 2: handlepunt */
	EditIpo *ei;
	BezTriple *bezt1;
	BPoint *bp1;
	int a, b;
	short dist= 100, temp, mval[2], hpoint=0;

	*icu= 0;
	*bezt= 0;
	*bp= 0;

	getmouseco_areawin(mval);

	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN3(ei, flag & IPO_VISIBLE, flag & IPO_EDIT, icu) {
			
			if(ei->icu->bezt) {
				bezt1= ei->icu->bezt;
				b= ei->icu->totvert;
				while(b--) {

					ipoco_to_areaco_noclip(bezt1->vec[0], bezt1->s[0]);
					ipoco_to_areaco_noclip(bezt1->vec[1], bezt1->s[1]);
					ipoco_to_areaco_noclip(bezt1->vec[2], bezt1->s[2]);
										
					if(ei->disptype==IPO_DISPBITS) {
						temp= abs(mval[0]- bezt1->s[1][0]);
					}
					else temp= abs(mval[0]- bezt1->s[1][0])+ abs(mval[1]- bezt1->s[1][1]);

					if( bezt1->f2 & 1) temp+=5;
					if(temp<dist) { 
						hpoint= 1; 
						*bezt= bezt1; 
						dist= temp; 
						*icu= ei->icu; 
						*bp= 0; 
					}
					
					if(ei->disptype!=IPO_DISPBITS && ei->icu->ipo==IPO_BEZ) {
						/* middelste punten een klein voordeel */
						temp= -3+abs(mval[0]- bezt1->s[0][0])+ abs(mval[1]- bezt1->s[0][1]);
						if( bezt1->f1 & 1) temp+=5;
						if(temp<dist) { 
							hpoint= 0; 
							*bezt= bezt1; 
							dist= temp; 
							*icu= ei->icu; 
							*bp= 0; 
						}
		
						temp= abs(mval[0]- bezt1->s[2][0])+ abs(mval[1]- bezt1->s[2][1]);
						if( bezt1->f3 & 1) temp+=5;
						if(temp<dist) { 
							hpoint= 2; 
							*bezt=bezt1; 
							dist= temp; 
							*icu= ei->icu; 
							*bp= 0; 
						}
					}
					bezt1++;
				}
			}
			else if(ei->icu->bp) {
				bp1= ei->icu->bp;
				b= ei->icu->totvert;
				while(b--) {
					
					ipoco_to_areaco_noclip(bp1->vec, bp1->s);
					
					temp= abs(mval[0]- bp1->s[0])+ abs(mval[1]- bp1->s[1]);
					
					if( (bp1->f1 & 1)) temp+= 5;
					if(temp<dist) { 
						hpoint= 0; 
						*bp= bp1; 
						dist= temp; 
						*icu= ei->icu; 
						*bezt= 0; 
					}
					bp1++;
				}
			}
		}
	}

	return hpoint;
}


void move_to_frame()
{
	EditIpo *ei;
	BezTriple *bezt;
	ID *id;
	float cfra;
	int a, b;
	
	if(G.sipo->editipo==0) return;

	ei= G.sipo->editipo;

	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
			if(G.sipo->showkey || (ei->flag & IPO_EDIT)) {
			
				if(ei->icu->bezt) {
					
					b= ei->icu->totvert;
					bezt= ei->icu->bezt;
					while(b--) {
						if(BEZSELECTED(bezt)) {
						
							cfra=  bezt->vec[1][0]/G.scene->r.framelen;

							id= G.sipo->from;
							if(id && GS(id->name)==ID_OB ) {
								Object *ob= (Object *)id;
								if(ob->sf!=0.0 && (ob->ipoflag & OB_OFFS_OB) ) {
									cfra+= ob->sf/G.scene->r.framelen;
								}
							}
							CFRA= ffloor(cfra+0.5);
						
							if(CFRA < 1) CFRA= 1;
							do_global_buttons(B_NEWFRAME);
							
							break;
						}
						bezt++;
					}
				}
			}
		}
	}
}

/* *********************************** */

void do_ipowin_buts(short event)
{
	if((G.qual & LR_SHIFTKEY)==0) {
		G.sipo->rowbut= (1<<event);
	}
	addqueue(curarea->win, REDRAW, 1);
	
	update_editipo_flags();

	if(G.sipo->showkey) {
		make_ipokey();
		if(G.sipo->blocktype==ID_OB) allqueue(REDRAWVIEW3D, 0);
	}

}

void do_ipo_selectbuttons()
{
	EditIpo *ei, *ei1;
	int a, nr;
	short mval[2];
	
	if(G.sipo->showkey) return;
	
	/* geen editipo toestaan: editipo's naar selected omzetten */
	get_status_editipo();
	if(totipo_edit) {
		set_editflag_editipo();
	}
	
	/* welke */
	getmouseco_areawin(mval);

	nr= -(mval[1]-curarea->winy+30-G.sipo->butofs-IPOBUTY)/IPOBUTY;
	if(nr>=0 && nr<G.sipo->totipo) {
		ei= G.sipo->editipo;
		ei+= nr;
		
		if(ei->icu) {
			if((ei->flag & IPO_VISIBLE)==0) {
				ei->flag |= IPO_VISIBLE;
				G.sipo->rowbut |= (1<<nr);
			}
	
			if((G.qual & LR_SHIFTKEY)==0) {
				ei1= G.sipo->editipo;
				for(a=0; a<G.sipo->totipo; a++) {
					ei1->flag &= ~IPO_SELECT;
					ei1++;
				}
			}

			if(ei->flag & IPO_SELECT) {
				ei->flag &= ~IPO_SELECT;
			}
			else {
				ei->flag |= IPO_SELECT;
			}

			update_editipo_flags();
			addqueue(curarea->win, REDRAW, 1);
		}
	}
}

/* ******************************************* */

EditIpo *get_editipo()
{
	EditIpo *ei;
	int a, sel=0;
	
	get_status_editipo();
	
	if(totipo_edit>1) {
		error("Too many editipo's");
		return 0;
	}
	if(G.sipo->editipo==0) return 0;
	
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++) {
		if(ei->flag & IPO_VISIBLE) {
			if( ei->flag & IPO_EDIT ) return ei;
			else if(totipo_vis==1) return ei;
			
			if(ei->flag & IPO_SELECT) {
				if(totipo_sel==1) return ei;
			}
		}
		ei++;
	}
	return 0;
}

Ipo *get_ipo(ID *from, int make)
{
	Object *ob;
	Material *ma;
	Curve *cu;
	Sequence *seq;
	Key *key;
	World *wo;
	Lamp *la;
	Camera *ca;
	Ipo *ipo= 0;
	
	if( GS(from->name)==ID_OB) {
		ob= (Object *)from;
		if(ob->id.lib) return 0;
		ipo= ob->ipo;
		
		if(make && ipo==0) ipo= ob->ipo= add_ipo("ObIpo", ID_OB);
	}
	else if( GS(from->name)==ID_MA) {
		ma= (Material *)from;
		if(ma->id.lib) return 0;
		ipo= ma->ipo;
		
		if(make && ipo==0) ipo= ma->ipo= add_ipo("MatIpo", ID_MA);
	}
	else if( GS(from->name)==ID_SEQ) {
		seq= (Sequence *)from;

		if(seq->type & SEQ_EFFECT) {
			ipo= seq->ipo;
			if(make && ipo==0) ipo= seq->ipo= add_ipo("SeqIpo", ID_SEQ);
		}
		else return 0;
	}	
	else if( GS(from->name)==ID_CU) {
		cu= (Curve *)from;
		if(cu->id.lib) return 0;
		ipo= cu->ipo;
		
		if(make && ipo==0) ipo= cu->ipo= add_ipo("CuIpo", ID_CU);
	}
	else if( GS(from->name)==ID_KE) {
		key= (Key *)from;
		if(key->id.lib) return 0;
		ipo= key->ipo;
		
		if(make && ipo==0) ipo= key->ipo= add_ipo("KeyIpo", ID_KE);
	}
	else if( GS(from->name)==ID_WO) {
		wo= (World *)from;
		if(wo->id.lib) return 0;
		ipo= wo->ipo;
		
		if(make && ipo==0) ipo= wo->ipo= add_ipo("WoIpo", ID_WO);
	}
	else if( GS(from->name)==ID_LA) {
		la= (Lamp *)from;
		if(la->id.lib) return 0;
		ipo= la->ipo;
		
		if(make && ipo==0) ipo= la->ipo= add_ipo("LaIpo", ID_LA);
	}
	else if( GS(from->name)==ID_CA) {
		ca= (Camera *)from;
		if(ca->id.lib) return 0;
		ipo= ca->ipo;
		
		if(make && ipo==0) ipo= ca->ipo= add_ipo("CaIpo", ID_CA);
	}
	else return 0;
	
	return ipo;	
}

IpoCurve *get_ipocurve(ID *from, int adrcode)
{
	Ipo *ipo= 0;
	IpoCurve *icu=0;
	
	/* return 0 als lib */
	/* ook testen of ipo en ipocurve bestaan */
	
	ipo= get_ipo(from, 1);	/* 1= make */
	if(G.sipo) G.sipo->ipo= ipo;
	
	if(ipo && ipo->id.lib==0) {
	
		icu= ipo->curve.first;
		while(icu) {
			if(icu->adrcode==adrcode) break;
			icu= icu->next;
		}
		if(icu==0) {
			icu= callocN(sizeof(IpoCurve), "ipocurve");
			
			icu->flag |= IPO_VISIBLE;
			icu->blocktype= GS(from->name);
			icu->adrcode= adrcode;
			
			set_icu_vars(icu);
			
			addtail( &(ipo->curve), icu);
		}
	}
	return icu;
}

void insert_vert_ipo(IpoCurve *icu, float x, float y)
{
	BPoint *bp;
	BezTriple *bezt, beztr, *newbezt;
	int a, h1, h2;
	char str[20];
	
	if(icu->bp) {
		bp= icu->bp;
		
	}
	else {
		bzero(&beztr, sizeof(BezTriple));
		beztr.vec[1][0]= x;
		beztr.vec[1][1]= y;
		beztr.hide= IPO_BEZ;
		beztr.f1= beztr.f2= beztr.f3= SELECT;
		beztr.h1= beztr.h2= HD_AUTO;
		
		bezt= icu->bezt;
		
		if(bezt==0) {
			icu->bezt= callocN( sizeof(BezTriple), "beztriple");
			*(icu->bezt)= beztr;
			icu->totvert= 1;
		}
		else {
			/* alle vertices deselect */
			for(a=0; a<icu->totvert; a++, bezt++) {
				bezt->f1= bezt->f2= bezt->f3= 0;
			}
		
			bezt= icu->bezt;
			for(a=0; a<=icu->totvert; a++, bezt++) {
				
				/* geen dubbele punten */
				if(a<icu->totvert && (bezt->vec[1][0]>x-IPOTHRESH && bezt->vec[1][0]<x+IPOTHRESH)) {
					*(bezt)= beztr;
					break;
				}
				if(a==icu->totvert || bezt->vec[1][0] > x) {
					newbezt= callocN( (icu->totvert+1)*sizeof(BezTriple), "beztriple");
					
					if(a>0) memcpy(newbezt, icu->bezt, a*sizeof(BezTriple));
					
					bezt= newbezt+a;
					*(bezt)= beztr;
					
					if(a<icu->totvert) memcpy(newbezt+a+1, icu->bezt+a, (icu->totvert-a)*sizeof(BezTriple));
					
					freeN(icu->bezt);
					icu->bezt= newbezt;
					
					icu->totvert++;
					break;
				}
			}
		}
		
		
		calchandles_ipocurve(icu);
		
		/* handletype goedzetten */
		if(icu->totvert>2) {
			h1= h2= HD_AUTO;
			if(a>0) h1= (bezt-1)->h2;
			if(a<icu->totvert-1) h2= (bezt+1)->h1;
			bezt->h1= h1;
			bezt->h2= h2;

			calchandles_ipocurve(icu);
		}		
	}
}

void add_vert_ipo()
{
	EditIpo *ei;
	float x, y;
	int val;
	short mval[2];

	if(G.sipo->ipo && G.sipo->ipo->id.lib) return;
	if(G.sipo->showkey) {
		G.sipo->showkey= 0;
		free_ipokey(&G.sipo->ipokey);
	}
	
	getmouseco_areawin(mval);
	
	if(mval[0]>G.v2d->mask.xmax) return;
	
	ei= get_editipo();
	if(ei==0) return;

	areamouseco_to_ipoco(mval, &x, &y);
	
	if(ei->icu==0) {
		if(G.sipo->from)
			ei->icu= get_ipocurve(G.sipo->from, ei->adrcode);
	}
	if(ei->icu==0) return;

	if(ei->disptype==IPO_DISPBITS) {
		ei->icu->vartype= IPO_BITS;
		val= ffloor(y-0.5);
		if(val<0) val= 0;
		y= (float)(1 << val);
	}
	
	insert_vert_ipo(ei->icu, x, y);

	/* voor zekerheid: als icu 0 was, of maar 1 curve visible */
	ei->flag |= IPO_SELECT;
	ei->icu->flag= ei->flag;

	editipo_changed(1);
}

void add_duplicate_editipo()
{
	Object *ob;
	EditIpo *ei;
	IpoCurve *icu;
	BezTriple *bezt, *beztn, *new;
	int tot, a, b;
	
	get_status_editipo();
	if(totipo_vertsel==0) return;
	
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN3(ei, flag & IPO_VISIBLE, icu, icu->bezt) {
			if(G.sipo->showkey || (ei->flag & IPO_EDIT)) {
				icu= ei->icu;
				
				/* hoeveel punten */
				tot= 0;
				b= icu->totvert;
				bezt= icu->bezt;
				while(b--) {
					if(bezt->f2 & 1) tot++;
					bezt++;
				}
				
				if(tot) {
					icu->totvert+= tot;
					new= beztn= mallocN(icu->totvert*sizeof(BezTriple), "bezt");
					bezt= icu->bezt;
					b= icu->totvert-tot;
					while(b--) {
						*beztn= *bezt;
						if(bezt->f2 & 1) {
							beztn->f1= beztn->f2= beztn->f3= 0;
							beztn++;
							*beztn= *bezt;
						}
						beztn++;
						bezt++;
					}
					freeN(icu->bezt);
					icu->bezt= new;
					
					calchandles_ipocurve(icu);
				}
			}
		}
	}
	
	if(G.sipo->showkey) {
		make_ipokey();
		if(G.sipo->blocktype==ID_OB) {
			ob= OBACT;
			if(ob && (ob->ipoflag & OB_DRAWKEY)) allqueue(REDRAWVIEW3D, 0);
		}
	}
	transform_ipo('g');
}

void remove_doubles_ipo()
{
	EditIpo *ei;
	IpoKey *ik, *ikn;
	BezTriple *bezt, *new, *new1;
	float newy, val;
	int mode, a, b;
	
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN3(ei, flag & IPO_VISIBLE, icu, icu->bezt) {
			
			/* OF de curve is selected OF in editmode OF in keymode */
			mode= 0;
			if(G.sipo->showkey || (ei->flag & IPO_EDIT)) mode= 1;
			else if(ei->flag & IPO_SELECT) mode= 2;
			
			if(mode) {
				bezt= ei->icu->bezt;
				new= new1= mallocN(ei->icu->totvert*sizeof(BezTriple), "newbezt");
				*new= *bezt;
				b= ei->icu->totvert-1;
				bezt++;
				while(b--) {
					
					/* mag er verwijderd worden? */
					if(mode==2 || (bezt->f2 & 1)) {
					
						/* verschillen de punten? */
						if( fabs( bezt->vec[1][0]-new->vec[1][0] ) > 0.9 ) {
							new++;
							*new= *bezt;
						}
						else {
							/* gemiddelde */
							VecMidf(new->vec[0], new->vec[0], bezt->vec[0]);
							VecMidf(new->vec[1], new->vec[1], bezt->vec[1]);
							VecMidf(new->vec[2], new->vec[2], bezt->vec[2]);
							
							new->h1= new->h2= HD_FREE;
							
							ei->icu->totvert--;
						}
						
					}
					else {
						new++;
						*new= *bezt;
					}
					bezt++;
				}
				
				freeN(ei->icu->bezt);
				ei->icu->bezt= new1;
				
				calchandles_ipocurve(ei->icu);				
			}
		}
	}
	
	editipo_changed(1);	/* maakt ook ipokeys opnieuw! */

	/* dubbele keys weg */
	if(G.sipo->showkey) {
		ik= G.sipo->ipokey.first;
		ikn= ik->next;
		
		while(ik && ikn) {
			if( (ik->flag & 1) && (ikn->flag & 1) ) {
				if( fabs(ik->val-ikn->val) < 0.9 ) {
					val= (ik->val + ikn->val)/2.0;
					
					for(a=0; a<G.sipo->totipo; a++) {
						if(ik->data[a]) ik->data[a]->vec[1][0]= val;
						if(ikn->data[a]) ikn->data[a]->vec[1][0]= val;						
					}
				}
			}
			ik= ikn;
			ikn= ikn->next;

		}
		
		editipo_changed(1);	/* maakt ook ipokeys opnieuw! */

	}
	deselectall_editipo();
}

void join_ipo()
{
	Object *ob;
	EditIpo *ei;
	IpoKey *ik;
	IpoCurve *icu;
	BezTriple *bezt, *beztn, *new;
	float val;
	int mode, tot, a, b;
	
	get_status_editipo();
	
	mode= pupmenu("Join %t|All Selected %x1|Selected doubles %x2");
	if( mode==2 ) {
		remove_doubles_ipo();
		return;
	}
	else if(mode!=1) return;
	
	/* eerst: meerdere geselecteerde verts in 1 curve */
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN3(ei, flag & IPO_VISIBLE, icu, icu->bezt) {
			if(G.sipo->showkey || (ei->flag & IPO_EDIT)) {
				icu= ei->icu;
				
				/* hoeveel punten */
				tot= 0;
				b= icu->totvert;
				bezt= icu->bezt;
				while(b--) {
					if(bezt->f2 & 1) tot++;
					bezt++;
				}
				
				if(tot>1) {
					tot--;
					icu->totvert-= tot;
					
					new= mallocN(icu->totvert*sizeof(BezTriple), "bezt");
					/* het eerste punt wordt het nieuwe punt */
					beztn= new+1;
					tot= 0;
					
					bezt= icu->bezt;
					b= icu->totvert+tot+1;
					while(b--) {
						
						if(bezt->f2 & 1) {
							if(tot==0) *new= *bezt;
							else {
								VecAddf(new->vec[0], new->vec[0], bezt->vec[0]);
								VecAddf(new->vec[1], new->vec[1], bezt->vec[1]);
								VecAddf(new->vec[2], new->vec[2], bezt->vec[2]);
							}
							tot++;
						}
						else {
							*beztn= *bezt;
							beztn++;
						}
						bezt++;
					}
					
					VecMulf(new->vec[0], 1.0/((float)tot));
					VecMulf(new->vec[1], 1.0/((float)tot));
					VecMulf(new->vec[2], 1.0/((float)tot));
					
					freeN(icu->bezt);
					icu->bezt= new;
					
					sort_time_ipocurve(icu);
					calchandles_ipocurve(icu);
				}
			}
		}
	}
	
	/* dan: in keymode: meerdere geselecteerde keys samenvoegen */
	
	editipo_changed(1);	/* maakt ook ipokeys opnieuw! */
	
	if(G.sipo->showkey) {
		ik= G.sipo->ipokey.first;
		val= 0.0;
		tot= 0;
		while(ik) {
			if(ik->flag & 1) {
				for(a=0; a<G.sipo->totipo; a++) {
					if(ik->data[a]) {
						val+= ik->data[a]->vec[1][0];
						break;
					}
				}
				tot++;
			}
			ik= ik->next;
		}
		if(tot>1) {
			val/= (float)tot;
			
			ik= G.sipo->ipokey.first;
			while(ik) {
				if(ik->flag & 1) {
					for(a=0; a<G.sipo->totipo; a++) {
						if(ik->data[a]) {
							ik->data[a]->vec[1][0]= val;
						}
					}
				}
				ik= ik->next;
			}
			editipo_changed(0);
		}
	}
	deselectall_editipo();
}

void ipo_snapmenu()
{
	IpoCurve *icu;
	EditIpo *ei;
	BezTriple *bezt;
	float dx;
	int a, b;
	short event, ok, ok2;
	
	event= pupmenu("Snap %t|Horizontal %x1|To next %x2|To frame %x3|To current frame%x4");
	if(event < 1) return;
	
	get_status_editipo();

	ei= G.sipo->editipo;
	for(b=0; b<G.sipo->totipo; b++, ei++) {
		if ISPOIN3(ei, flag & IPO_VISIBLE, icu, icu->bezt) {
		
			ok2= 0;
			if(G.sipo->showkey) ok2= 1;
			else if(totipo_vert && (ei->flag & IPO_EDIT)) ok2= 2;
			else if(totipo_vert==0 && (ei->flag & IPO_SELECT)) ok2= 3;
			
			if(ok2) {
				bezt= ei->icu->bezt;
				a= ei->icu->totvert;
				while(a--) {
					ok= 0;
					if(totipo_vert) {
						 if(bezt->f2 & 1) ok= 1;
					}
					else ok= 1;
					
					if(ok) {
						if(event==1) {
							bezt->vec[0][1]= bezt->vec[2][1]= bezt->vec[1][1];
							if(bezt->h1==HD_AUTO || bezt->h1==HD_VECT) bezt->h1= HD_ALIGN;
							if(bezt->h2==HD_AUTO || bezt->h2==HD_VECT) bezt->h2= HD_ALIGN;
						}
						else if(event==2) {
							if(a) {
								bezt->vec[0][1]= bezt->vec[1][1]= bezt->vec[2][1]= (bezt+1)->vec[1][1];
								if(bezt->h1==HD_AUTO || bezt->h1==HD_VECT) bezt->h1= HD_ALIGN;
								if(bezt->h2==HD_AUTO || bezt->h2==HD_VECT) bezt->h2= HD_ALIGN;
							}
						}
						else if(event==3) {
							bezt->vec[1][0]= ffloor(bezt->vec[1][0]+0.5);
						}
						else if(event==4) {	/* to current frame */
							
							if(ok2==1 || ok2==2) {
								
								if(G.sipo->blocktype==ID_SEQ) {
									Sequence *seq;
							
									seq= (Sequence *)G.sipo->from;
									if(seq) {
										dx= (CFRA-seq->startdisp);
										dx= 100.0*dx/((float)(seq->enddisp-seq->startdisp));
										
										dx-= bezt->vec[1][0];
									}
								}
								else dx= G.scene->r.framelen*CFRA - bezt->vec[1][0];
								
								bezt->vec[0][0]+= dx;
								bezt->vec[1][0]+= dx;
								bezt->vec[2][0]+= dx;
							}
						}
					}
					
					bezt++;
				}
				calchandles_ipocurve(ei->icu);
			}
		}
	}
	editipo_changed(1);
}



void mouse_select_ipo()
{
	Object *ob;
	EditIpo *ei, *actei= 0;
	IpoCurve *icu;
	IpoKey *ik, *actik;
	BezTriple *bezt;
	BPoint *bp;
	Key *key;
	KeyBlock *kb, *actkb=0;
	float x, y, dist, mindist;
	int a, oldflag, hand, ok;
	short mval[2], xo, yo;
	
	if(G.sipo->editipo==0) return;
	
	get_status_editipo();
	
	if(G.sipo->showkey) {
		getmouseco_areawin(mval);
	
		areamouseco_to_ipoco(mval, &x, &y);
		actik= 0;
		mindist= 1000.0;
		ik= G.sipo->ipokey.first;
		while(ik) {
			dist= fabs(ik->val-x);
			if(ik->flag & 1) dist+= 1.0;
			if(dist < mindist) {
				actik= ik;
				mindist= dist;
			}
			ik= ik->next;
		}
		if(actik) {
			oldflag= actik->flag;
			
			if(G.qual & LR_SHIFTKEY);
			else deselectall_editipo();
			
			if(G.qual & LR_SHIFTKEY) {
				if(oldflag & 1) actik->flag &= ~1;
				else actik->flag |= 1;
			}
			else {
				actik->flag |= 1;
			}
		}
	}
	else if(totipo_edit) {
	
		hand= findnearest_ipovert(&icu, &bezt, &bp);

		if(G.qual & LR_SHIFTKEY) {
			if(bezt) {
				if(hand==1) {
					if(BEZSELECTED(bezt)) {
						bezt->f1= bezt->f2= bezt->f3= 0;
					}
					else {
						bezt->f1= bezt->f2= bezt->f3= 1;
					}
				}
				else if(hand==0) {
					if(bezt->f1 & 1) bezt->f1= 0;
					else bezt->f1= 1;
				}
				else {
					if(bezt->f3 & 1) bezt->f3= 0;
					else bezt->f3= 1;
				}
			}	
			else if(bp) bp->f1 |= 1;
			
		}
		else {
			deselectall_editipo();
		
			if(bezt) {
				if(hand==1) {
					bezt->f1|= 1; bezt->f2|= 1; bezt->f3|= 1;
				}
				else if(hand==0) bezt->f1|= 1;
				else bezt->f3|= 1;
			}
			else if(bp) bp->f1 |= 1;
		}
	}
	else {
		
		/* vertex keys ? */
		
		if(G.sipo->blocktype==ID_KE && G.sipo->from) {
			key= (Key *)G.sipo->from;
			
			ei= G.sipo->editipo;
			if(key->type==KEY_NORMAL || (ei->flag & IPO_VISIBLE)) {
				getmouseco_areawin(mval);
				
				areamouseco_to_ipoco(mval, &x, &y);
				/* hoeveel is 20 pixels? */
				mindist= 20.0*(G.v2d->cur.ymax-G.v2d->cur.ymin)/(float)curarea->winy;
				
				kb= key->block.first;
				while(kb) {
					dist= fabs(kb->pos-y);
					if(kb->flag & SELECT) dist+= 0.01;
					if(dist < mindist) {
						actkb= kb;
						mindist= dist;
					}
					kb= kb->next;
				}
				if(actkb) {
					ok= TRUE;
					if(G.obedit && (actkb->flag & 1)==0) {
						ok= okee("Copy Key after leaving EditMode");
					}
					if(ok) {
						/* doet ook alle keypos */
						deselectall_editipo();
						
						/* oldflag= actkb->flag; */
						
						/* if(G.qual & LR_SHIFTKEY); */
						/* else { */
						/* 	deselectall_key(); */
						/* } */
						
						/* if(G.qual & LR_SHIFTKEY) { */
						/* 	if(oldflag & 1) actkb->flag &= ~1; */
						/* 	else actkb->flag |= 1; */
						/* } */
						/* else { */
							actkb->flag |= 1;
						/* } */
						
						/* bereken keypos */
						showkeypos((Key *)G.sipo->from, actkb);
					}
				}
			}
		}
			
		/* select curve */
		if(actkb==0) {
			if(totipo_vis==1) {
				ei= G.sipo->editipo;
				for(a=0; a<G.sipo->totipo; a++, ei++) {
					if(ei->icu) {
						if(ei->flag & IPO_VISIBLE) actei= ei;
					}
				}
			}
			else if(totipo_vis>1) {
				actei= select_proj_ipo(0, 0);
			}
			
			if(actei) oldflag= actei->flag;
			
			if(G.qual & LR_SHIFTKEY);
			else deselectall_editipo();
			
			if(actei) {
				if(G.qual & LR_SHIFTKEY) {
					if(oldflag & IPO_SELECT) actei->flag &= ~IPO_SELECT;
					else actei->flag |= IPO_SELECT;
				}
				else {
					actei->flag |= IPO_SELECT;
				}
			}
		}
	}
	
	update_editipo_flags();
	
	force_draw();
	
	if(G.sipo->showkey && G.sipo->blocktype==ID_OB) {
		ob= OBACT;
		if(ob && (ob->ipoflag & OB_DRAWKEY)) draw_object_ext(BASACT);
	}
	
	getmouseco_areawin(mval);
	xo= mval[0]; 
	yo= mval[1];

	while(get_mbut()&R_MOUSE) {
		usleep(1);
		
		getmouseco_areawin(mval);
		if(abs(mval[0]-xo)+abs(mval[1]-yo) > 4) {
			
			if(actkb) move_keys();
			else transform_ipo('g');
			
			return;
		}
	}
}

void sethandles_ipo(int code)
{
	/* code==1: set autohandle */
	/* code==2: set vectorhandle */
	/* als code==3 (HD_ALIGN) toggelt het, vectorhandles worden HD_FREE */
	EditIpo *ei;
	BezTriple *bezt;
	int a, b, ok=0;

	if(G.sipo->ipo && G.sipo->ipo->id.lib) return;

	if(code==1 || code==2) {
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++, ei++) {
			if ISPOIN4(ei, flag & IPO_VISIBLE, flag & IPO_EDIT, icu, icu->bezt) {
				bezt= ei->icu->bezt;
				b= ei->icu->totvert;
				while(b--) {
					if(bezt->f1 || bezt->f3) {
						if(bezt->f1) bezt->h1= code;
						if(bezt->f3) bezt->h2= code;
						
						if(bezt->h1!=bezt->h2) {
							if ELEM(bezt->h1, HD_ALIGN, HD_AUTO) bezt->h1= HD_FREE;
							if ELEM(bezt->h2, HD_ALIGN, HD_AUTO) bezt->h2= HD_FREE;
						}
					}
					bezt++;
				}
				calchandles_ipocurve(ei->icu);
			}
		}
	}
	else {
		/* is er 1 handle NIET vrij: alles vrijmaken, else ALIGNED maken */
		
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++, ei++) {
			if ISPOIN4(ei, flag & IPO_VISIBLE, flag & IPO_EDIT, icu, icu->bezt) {
				bezt= ei->icu->bezt;
				b= ei->icu->totvert;
				while(b--) {
					if(bezt->f1 && bezt->h1) ok= 1;
					if(bezt->f3 && bezt->h2) ok= 1;
					if(ok) break;
					bezt++;
				}
			}
		}
		if(ok) ok= HD_FREE;
		else ok= HD_ALIGN;
		
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++, ei++) {
			if ISPOIN4(ei, flag & IPO_VISIBLE, flag & IPO_EDIT, icu, icu->bezt) {
				bezt= ei->icu->bezt;
				b= ei->icu->totvert;
				while(b--) {
					if(bezt->f1) bezt->h1= ok;
					if(bezt->f3 ) bezt->h2= ok;
	
					bezt++;
				}
				calchandles_ipocurve(ei->icu);
			}
		}
	}
	editipo_changed(1);
}

void set_ipotype()
{
	EditIpo *ei;
	Key *key;
	KeyBlock *kb;
	int a;
	short event;

	if(G.sipo->ipo && G.sipo->ipo->id.lib) return;
	if(G.sipo->showkey) return;
	get_status_editipo();
	
	if(G.sipo->blocktype==ID_KE && totipo_edit==0 && totipo_sel==0) {
		key= (Key *)G.sipo->from;
		if(key==0) return;
		
		event= pupmenu("Key Type %t|Linear %x1|Cardinal %x2|B spline %x3");
		if(event < 1) return;
		
		kb= key->block.first;
		while(kb) {
			if(kb->flag & SELECT) {
				kb->type= 0;
				if(event==1) kb->type= KEY_LINEAR;
				if(event==2) kb->type= KEY_CARDINAL;
				if(event==3) kb->type= KEY_BSPLINE;
			}
			kb= kb->next;
		}
	}
	else {
		event= pupmenu("Ipo Type %t|Constant %x1|Linear %x2|Bezier %x3");
		if(event < 1) return;
		
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++, ei++) {
			if ISPOIN3(ei, flag & IPO_VISIBLE, flag & IPO_SELECT, icu) {
				if(event==1) ei->icu->ipo= IPO_CONST;
				else if(event==2) ei->icu->ipo= IPO_LIN;
				else ei->icu->ipo= IPO_BEZ;
			}
		}
	}
	addqueue(curarea->win, REDRAW, 1);
}

void borderselect_ipo()
{
	EditIpo *ei;
	IpoKey *ik;
	BezTriple *bezt;
	BPoint *bp;
	rcti rect;
	rctf rectf;
	int a, b, val, ok;
	short mval[2];

	get_status_editipo();
	
	val= get_border(&rect, 3);

	if(val) {
		mval[0]= rect.xmin;
		mval[1]= rect.ymin;
		areamouseco_to_ipoco(mval, &rectf.xmin, &rectf.ymin);
		mval[0]= rect.xmax;
		mval[1]= rect.ymax;
		areamouseco_to_ipoco(mval, &rectf.xmax, &rectf.ymax);

		if(G.sipo->showkey) {
			ik= G.sipo->ipokey.first;
			while(ik) {
				if(rectf.xmin<ik->val && rectf.xmax>ik->val) {
					if(val==LEFTMOUSE) ik->flag |= 1;
					else ik->flag &= ~1;
				}
				ik= ik->next;
			}
			update_editipo_flags();
		}
		else if(totipo_edit==0) {
			if(rect.xmin<rect.xmax && rect.ymin<rect.ymax)
				select_proj_ipo(&rectf, val);
		}
		else {
			
			ei= G.sipo->editipo;
			for(a=0; a<G.sipo->totipo; a++, ei++) {
				if ISPOIN3(ei, flag & IPO_VISIBLE, flag & IPO_EDIT, icu) {
					if(ei->icu->bezt) {
						b= ei->icu->totvert;
						bezt= ei->icu->bezt;
						while(b--) {
							
							ok= 0;
							if( ei->icu->ipo==IPO_BEZ ) {
								if(in_rctf(&rectf, bezt->vec[0][0], bezt->vec[0][1])) ok= 1;
								if(in_rctf(&rectf, bezt->vec[2][0], bezt->vec[2][1])) ok= 1;
							}
							if(in_rctf(&rectf, bezt->vec[1][0], bezt->vec[1][1])) ok= 1;
							
							if(ok) {
								if(val==LEFTMOUSE) {
									bezt->f1 |= 1;
									bezt->f2 |= 1;
									bezt->f3 |= 1;
								}
								else {
									bezt->f1 &= ~1;
									bezt->f2 &= ~1;
									bezt->f3 &= ~1;
								}
							}
							
							bezt++;
						}
					}
					else if(ei->icu->bp) {
						
					}
				}
			}
		}
		addqueue(curarea->win, REDRAW, 1);
	}
}


void del_ipo()
{
	EditIpo *ei;
	BezTriple *bezt, *bezt1, *bezt2;
	BPoint *bp, *bp1, *bp2;
	float *fp;
	int a, b;
	int del, event;

	get_status_editipo();
	if(G.sipo->ipo && G.sipo->ipo->id.lib) return;
	
	if(totipo_edit==0 && totipo_sel==0 && totipo_vertsel==0) {
		delete_key();
		return;
	}
	
	if( okee("Erase selected")==0 ) return;

	/* eerste doorloop, kunnen hele stukken weg? */
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
	
		del= 0;
		
		if(G.sipo->showkey==0 && totipo_edit==0) {
			if ISPOIN3(ei, flag & IPO_VISIBLE, flag & IPO_SELECT, icu) {
				del= 1;
			}
		}
		else {
			if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
				if(G.sipo->showkey || (ei->flag & IPO_EDIT)) {
					if(ei->icu->bezt) {
						bezt= ei->icu->bezt;
						b= ei->icu->totvert;
						if(b) {
							while(b) {
								if( BEZSELECTED(bezt) );
								else break;
								b--;
								bezt++;
							}
							if(b==0) del= 1;
						}
					}
					else {
						bp= ei->icu->bp;
						b= ei->icu->totvert;
						if(b) {
							while(b) {
								if(bp->f1 & 1 );
								else break;
								b--;
								bp++;
							}
							if(b==0) del= 1;
						}
					}
				}
			}
		}
		
		if(del) {
			remlink( &(G.sipo->ipo->curve), ei->icu);
			if(ei->icu->bezt) freeN(ei->icu->bezt);
			if(ei->icu->bp) freeN(ei->icu->bp);
			freeN(ei->icu);
			ei->flag &= ~IPO_SELECT;
			ei->flag &= ~IPO_EDIT;
			ei->icu= 0;	
		}
	}
	
	/* tweede doorloop, kleine stukken weg: alleen curves */
	ei= G.sipo->editipo;
	for(b=0; b<G.sipo->totipo; b++, ei++) {
		if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
			if(G.sipo->showkey || (ei->flag & IPO_EDIT)) {
			
				event= 0;
				if(ei->icu->bezt) {
				
					bezt= ei->icu->bezt;
					for(a=0; a<ei->icu->totvert; a++) {
						if( BEZSELECTED(bezt) ) {
							memcpy(bezt, bezt+1, (ei->icu->totvert-a-1)*sizeof(BezTriple));
							ei->icu->totvert--;
							a--;
							event= 1;
						}
						else bezt++;
					}
					if(event) {
						bezt1= mallocstructN(BezTriple, ei->icu->totvert, "delNurb");
						memcpy(bezt1, ei->icu->bezt, (ei->icu->totvert)*sizeof(BezTriple) );
						freeN(ei->icu->bezt);
						ei->icu->bezt= bezt1;
					}
				}
				else if(ei->icu->bp) {
					bp= ei->icu->bp;
	
					for(a=0; a<ei->icu->totvert; a++) {
						if( bp->f1 & 1 ) {
							memcpy(bp, bp+1, (ei->icu->totvert-a-1)*sizeof(BPoint));
							ei->icu->totvert--;
							a--;
							event= 1;
						}
						else {
							bp++;
						}
					}
					if(event) {
						bp1= mallocstructN(BPoint, ei->icu->totvert, "delNurb2");
						memcpy(bp1, ei->icu->bp, (ei->icu->totvert)*sizeof(BPoint) );
						freeN(ei->icu->bp);
						ei->icu->bp= bp1;
					}
				}
			}
		}
	}
	
	editipo_changed(1);
	
}

ListBase ipocopybuf={0, 0};
int totipocopybuf=0;

void free_ipocopybuf()
{
	IpoCurve *icu;
	
	while(icu= ipocopybuf.first) {
		if(icu->bezt) freeN(icu->bezt);
		if(icu->bp) freeN(icu->bezt);
		remlink(&ipocopybuf, icu);
		freeN(icu);
	}
	totipocopybuf= 0;
}

void copy_editipo()
{
	EditIpo *ei;
	IpoCurve *icu;
	int a;
	
	if(G.sipo->showkey) {
		error("cannot copy\n");
		return;
	}
	
	free_ipocopybuf();
	
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
			if( (ei->flag & IPO_EDIT) || (ei->flag & IPO_SELECT) ) {
				icu= callocN(sizeof(IpoCurve), "ipocopybuf");
				*icu= *(ei->icu);
				addtail(&ipocopybuf, icu);
				if(icu->bezt) {
					icu->bezt= mallocN(icu->totvert*sizeof(BezTriple), "ipocopybuf");
					memcpy(icu->bezt, ei->icu->bezt, icu->totvert*sizeof(BezTriple));
				}
				if(icu->bp) {
					icu->bp= mallocN(icu->totvert*sizeof(BPoint), "ipocopybuf");
					memcpy(icu->bp, ei->icu->bp, icu->totvert*sizeof(BPoint));
				}
				totipocopybuf++;
			}
		}
	}
	
	if(totipocopybuf==0) error("Copybuf is empty");
}

void paste_editipo()
{
	EditIpo *ei;
	IpoCurve *icu;
	int a, ok;
	
	if(G.sipo->showkey) return;
	
	if(totipocopybuf==0) return;
	if(G.sipo->ipo==0) return;
	if(G.sipo->ipo && G.sipo->ipo->id.lib) return;

	get_status_editipo();
	
	if(totipo_vis==0) {
		error("No visible splines");
	}
	else if(totipo_vis!=totipocopybuf && totipo_sel!=totipocopybuf) {
		error("Incompatible paste");
	}
	else {
		/* problemen voorkomen: andere splines visible dan select */
		if(totipo_vis==totipo_sel) totipo_vis= 0;
		
		icu= ipocopybuf.first;
		if(icu==0) return;

		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++, ei++) {
			if(ei->flag & IPO_VISIBLE) {
				ok= 0;
				if(totipo_vis==totipocopybuf) ok= 1;
				if(totipo_sel==totipocopybuf && (ei->flag & IPO_SELECT)) ok= 1;
	
				if(ok) {
			
					ei->icu= get_ipocurve(G.sipo->from, ei->adrcode);
					if(ei->icu==0) return;
					
					if(ei->icu->bezt) freeN(ei->icu->bezt);
					ei->icu->bezt= 0;
					if(ei->icu->bp) freeN(ei->icu->bp);
					ei->icu->bp= 0;
					
					ei->icu->totvert= icu->totvert;
					ei->icu->flag= ei->flag= icu->flag;
					ei->icu->extrap= icu->extrap;
					ei->icu->ipo= icu->ipo;
					
					if(icu->bezt) {
						ei->icu->bezt= mallocN(icu->totvert*sizeof(BezTriple), "ipocopybuf");
						memcpy(ei->icu->bezt, icu->bezt, icu->totvert*sizeof(BezTriple));
					}
					if(icu->bp) {
						ei->icu->bp= mallocN(icu->totvert*sizeof(BPoint), "ipocopybuf");
						memcpy(ei->icu->bp, icu->bp, icu->totvert*sizeof(BPoint));
					}
					
					icu= icu->next;
					
				}
			}
		}
		editipo_changed(1);
	}
}

void set_exprap_ipo(int mode)
{
	EditIpo *ei;
	int a;
		
	if(G.sipo->ipo && G.sipo->ipo->id.lib) return;
	/* in geval van keys: altijd ok */

	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
			if( (ei->flag & IPO_EDIT) || (ei->flag & IPO_SELECT) || (G.sipo->showkey) ) {
				ei->icu->extrap= mode;
			}
		}
	}
	editipo_changed(1);
}

int find_other_handles(EditIpo *eicur, float ctime, BezTriple **beztar)
{
	EditIpo *ei;
	BezTriple *bezt;
	int a, b, c= 1, totvert;
	
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if(ei!=eicur && ei->icu && (ei->flag & IPO_VISIBLE)) {
			
			bezt= ei->icu->bezt;
			totvert= ei->icu->totvert;
			
			for(b=0; b<totvert; b++, bezt++) {
				if( bezt->vec[1][0] < ctime+IPOTHRESH &&  bezt->vec[1][0] > ctime-IPOTHRESH) {
					if(c>2) return 0;
					beztar[c]= bezt;
					c++;
				}
			}
		}
	}
	
	if(c==3) return 1;
	return 0;
}

void set_speed_editipo(float speed)
{
	EditIpo *ei;
	BezTriple *bezt, *beztar[3];
	float si, co, vec1[3], vec2[3];
	int a, b, totvert, didit=0;
		
	if(G.sipo->ipo && G.sipo->ipo->id.lib) return;

	/* uitgaande van 1 visible curve, selected punt, bijhorende punten: lencorr! */

	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
			bezt= ei->icu->bezt;
			totvert= ei->icu->totvert;
			
			for(b=0; b<totvert; b++, bezt++) {
				if(BEZSELECTED(bezt)) {
					
					beztar[0]= bezt;
					
					if( find_other_handles(ei, bezt->vec[1][0], beztar) ) {
						beztar[0]->h1= beztar[0]->h2= HD_ALIGN;
						beztar[1]->h1= beztar[1]->h2= HD_ALIGN;
						beztar[2]->h1= beztar[2]->h2= HD_ALIGN;
						
						vec1[0]= (beztar[0]->vec[1][1] - beztar[0]->vec[0][1]) / (beztar[0]->vec[1][0] - beztar[0]->vec[0][0]) ;
						vec2[0]= (beztar[0]->vec[1][1] - beztar[0]->vec[2][1]) / (beztar[0]->vec[2][0] - beztar[0]->vec[1][0]) ;
						
						vec1[1]= (beztar[1]->vec[1][1] - beztar[1]->vec[0][1]) / (beztar[1]->vec[1][0] - beztar[1]->vec[0][0]) ;
						vec2[1]= (beztar[1]->vec[1][1] - beztar[1]->vec[2][1]) / (beztar[1]->vec[2][0] - beztar[1]->vec[1][0]) ;
						
						vec1[2]= (beztar[2]->vec[1][1] - beztar[2]->vec[0][1]) / (beztar[2]->vec[1][0] - beztar[2]->vec[0][0]) ;
						vec2[2]= (beztar[2]->vec[1][1] - beztar[2]->vec[2][1]) / (beztar[2]->vec[2][0] - beztar[2]->vec[1][0]) ;
						
						Normalise(vec1);
						Normalise(vec2);
						
						VecMulf(vec1, speed);
						VecMulf(vec2, speed);
						
						beztar[0]->vec[0][1]= beztar[0]->vec[1][1] - vec1[0]*(beztar[0]->vec[1][0] - beztar[0]->vec[0][0]) ;
						beztar[0]->vec[2][1]= beztar[0]->vec[1][1] - vec2[0]*(beztar[0]->vec[2][0] - beztar[0]->vec[1][0]) ;
						
						beztar[1]->vec[0][1]= beztar[1]->vec[1][1] - vec1[1]*(beztar[1]->vec[1][0] - beztar[1]->vec[0][0]) ;
						beztar[1]->vec[2][1]= beztar[1]->vec[1][1] - vec2[1]*(beztar[1]->vec[2][0] - beztar[1]->vec[1][0]) ;
						
						beztar[2]->vec[0][1]= beztar[2]->vec[1][1] - vec1[2]*(beztar[2]->vec[1][0] - beztar[2]->vec[0][0]) ;
						beztar[2]->vec[2][1]= beztar[2]->vec[1][1] - vec2[2]*(beztar[2]->vec[2][0] - beztar[2]->vec[1][0]) ;
						
						didit= 1;
					}
					else {
						error("Cannot set speed");
					}
				}
			}
			break;	
		}
	}
	
	if(didit==0) error("Did not set speed");
	
	editipo_changed(1);
	allqueue(REDRAWIPO, 0);
}


void insertkey(ID *id, int adrcode)
{
	IpoCurve *icu;
	Object *ob;
	void *poin;
	float curval, cfra;
	int type;
	
	if(id) {
		icu= get_ipocurve(id, adrcode);
		
		if(icu) {
			poin= get_ipo_poin(id, icu, &type);
			if(poin) {
				curval= read_ipo_poin(poin, type);
				
				cfra= frame_to_float(CFRA);
				
				if( GS(id->name)==ID_OB ) {
					ob= (Object *)id;
					if(ob->sf!=0.0 && (ob->ipoflag & OB_OFFS_OB) ) {
						/* eigenlijk frametofloat overniew berekenen! daarvoor CFRA als float door kunnen geven */
						cfra-= ob->sf*G.scene->r.framelen;
					}
				}
				
				insert_vert_ipo(icu, cfra, curval);
			}
		}
	}
}

void insertkey_editipo()
{
	EditIpo *ei;
	IpoKey *ik;
	ID *id;
	BezTriple *bezt;
	float *fp, cfra, *insertvals;
	int a, nr, ok, tot;
	short event;
	
	if(G.sipo->showkey)
		event= pupmenu("Insert KeyVertices %t|Current frame %x1|Selected Keys %x2");
	else 
		event= pupmenu("Insert KeyVertices %t|Current frame %x1");
	
	if(event<1) return;
	
	ei= G.sipo->editipo;
	for(nr=0; nr<G.sipo->totipo; nr++, ei++) {
		if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
		
			ok= 0;
			if(G.sipo->showkey) ok= 1;
			else if(ei->flag & IPO_SELECT) ok= 1;

			if(ok) {
				/* aantal tellen */
				if(event==1) tot= 1;
				else {
					ik= G.sipo->ipokey.first;
					tot= 0;
					while(ik) {
						if(ik->flag & 1) tot++;
						ik= ik->next;
					}
				}
				if(tot) {
				
					/* correctie voor ob timeoffs */
					cfra= frame_to_float(CFRA);
					id= G.sipo->from;	
					if(id && GS(id->name)==ID_OB ) {
						Object *ob= (Object *)id;
						if(ob->sf!=0.0 && (ob->ipoflag & OB_OFFS_OB) ) {
							cfra-= ob->sf*G.scene->r.framelen;
						}
					}
					else if(id && GS(id->name)==ID_SEQ) {
						extern Sequence *last_seq;	/* editsequence.c */
						
						if(last_seq) {
							cfra= 100.0*(cfra-last_seq->startdisp)/((float)(last_seq->enddisp-last_seq->startdisp));
						}
					}
			
					insertvals= mallocN(sizeof(float)*2*tot, "insertkey_editipo");
					/* zeker zijn dat icu->curval klopt */
					calc_ipo(G.sipo->ipo, cfra);
					
					if(event==1) {
						insertvals[0]= cfra;
						
						insertvals[1]= ei->icu->curval;
					}
					else {
						fp= insertvals;
						ik= G.sipo->ipokey.first;
						while(ik) {
							if(ik->flag & 1) {
								calc_ipo(G.sipo->ipo, ik->val);

								fp[0]= ik->val;
								fp[1]= ei->icu->curval;
								fp+= 2;
							}
							ik= ik->next;
						}
					}
					fp= insertvals;
					for(a=0; a<tot; a++, fp+=2) {
						insert_vert_ipo(ei->icu, fp[0], fp[1]);
					}
					
					freeN(insertvals);
					calc_ipo(G.sipo->ipo, (float)CFRA);
				}
			}
		}
	}
	allqueue(REDRAWIPO, 0);
	allspace(REMAKEIPO, 0);
}


void common_insertkey()
{
	Base *base;
	Object *ob;
	Material *ma;
	ID *id;
	IpoCurve *icu;
	World *wo;
	Lamp *la;
	Camera *ca;
	int tlay, map, event;
	char menustr[256];
	
	if(curarea->spacetype==SPACE_IPO) {
		insertkey_editipo();
	}
	else if(curarea->spacetype==SPACE_BUTS) {
		
		if(G.buts->mainb==BUTS_MAT) {
			id= G.buts->lockpoin;
			ma= G.buts->lockpoin;
			if(id) {
				event= pupmenu("Insert Key %t|RGB%x0|Alpha%x1|HaSize%x2|Mode %x3|All Color%x10|Ofs%x12|Size%x13|All Mapping%x11");
				if(event== -1) return;
				
				map= texchannel_to_adrcode(ma->texact);
				
				if(event==0 || event==10) {
					insertkey(id, MA_COL_R);
					insertkey(id, MA_COL_G);
					insertkey(id, MA_COL_B);
				}
				if(event==1 || event==10) {
					insertkey(id, MA_ALPHA);
				}
				if(event==2 || event==10) {
					insertkey(id, MA_HASIZE);
				}
				if(event==3 || event==10) {
					insertkey(id, MA_MODE);
				}
				if(event==10) {
					insertkey(id, MA_SPEC_R);
					insertkey(id, MA_SPEC_G);
					insertkey(id, MA_SPEC_B);
					insertkey(id, MA_REF);
					insertkey(id, MA_EMIT);
					insertkey(id, MA_AMB);
					insertkey(id, MA_SPEC);
					insertkey(id, MA_HARD);
					insertkey(id, MA_MODE);
				}
				if(event==12 || event==11) {
					insertkey(id, map+MAP_OFS_X);
					insertkey(id, map+MAP_OFS_Y);
					insertkey(id, map+MAP_OFS_Z);
				}
				if(event==13 || event==11) {
					insertkey(id, map+MAP_SIZE_X);
					insertkey(id, map+MAP_SIZE_Y);
					insertkey(id, map+MAP_SIZE_Z);
				}
				if(event==11) {
					insertkey(id, map+MAP_R);
					insertkey(id, map+MAP_G);
					insertkey(id, map+MAP_B);
					insertkey(id, map+MAP_DVAR);
					insertkey(id, map+MAP_COLF);
					insertkey(id, map+MAP_NORF);
					insertkey(id, map+MAP_VARF);
				}
			}
		}
		else if(G.buts->mainb==BUTS_WORLD) {
			id= G.buts->lockpoin;
			wo= G.buts->lockpoin;
			if(id) {
				event= pupmenu("Insert Key %t|ZenRGB%x0|HorRGB%x1|Mist%x2|stars %x3|Ofs%x12|Size%x13");
				if(event== -1) return;
				
				map= texchannel_to_adrcode(wo->texact);
				
				if(event==0) {
					insertkey(id, WO_ZEN_R);
					insertkey(id, WO_ZEN_G);
					insertkey(id, WO_ZEN_B);
				}
				if(event==1) {
					insertkey(id, WO_HOR_R);
					insertkey(id, WO_HOR_G);
					insertkey(id, WO_HOR_B);
				}
				if(event==2) {
					insertkey(id, WO_MISI);
					insertkey(id, WO_MISTDI);
					insertkey(id, WO_MISTSTA);
					insertkey(id, WO_MISTHI);
				}
				if(event==3) {
					insertkey(id, WO_STAR_R);
					insertkey(id, WO_STAR_G);
					insertkey(id, WO_STAR_B);
					insertkey(id, WO_STARDIST);
					insertkey(id, WO_STARSIZE);
				}
				if(event==12) {
					insertkey(id, map+MAP_OFS_X);
					insertkey(id, map+MAP_OFS_Y);
					insertkey(id, map+MAP_OFS_Z);
				}
				if(event==13) {
					insertkey(id, map+MAP_SIZE_X);
					insertkey(id, map+MAP_SIZE_Y);
					insertkey(id, map+MAP_SIZE_Z);
				}
			}
		}
		else if(G.buts->mainb==BUTS_LAMP) {
			id= G.buts->lockpoin;
			la= G.buts->lockpoin;
			if(id) {
				event= pupmenu("Insert Key %t|RGB%x0|Energy%x1|Spotsi%x2|Ofs%x12|Size%x13");
				if(event== -1) return;
				
				map= texchannel_to_adrcode(la->texact);
				
				if(event==0) {
					insertkey(id, LA_COL_R);
					insertkey(id, LA_COL_G);
					insertkey(id, LA_COL_B);
				}
				if(event==1) {
					insertkey(id, LA_ENERGY);
				}
				if(event==2) {
					insertkey(id, LA_SPOTSI);
				}
				if(event==12) {
					insertkey(id, map+MAP_OFS_X);
					insertkey(id, map+MAP_OFS_Y);
					insertkey(id, map+MAP_OFS_Z);
				}
				if(event==13) {
					insertkey(id, map+MAP_SIZE_X);
					insertkey(id, map+MAP_SIZE_Y);
					insertkey(id, map+MAP_SIZE_Z);
				}
				
			}
		}
		else if(G.buts->mainb==BUTS_EDIT) {
			ob= OBACT;
			if(ob && ob->type==OB_CAMERA) {
				id= G.buts->lockpoin;
				ca= G.buts->lockpoin;
				if(id) {
					event= pupmenu("Insert Key %t|Lens%x0|Clipping%x1");
					if(event== -1) return;

					if(event==0) {
						insertkey(id, CAM_LENS);
					}
					if(event==1) {
						insertkey(id, CAM_STA);
						insertkey(id, CAM_END);
					}
				}
			}
		}
		
		allqueue(REDRAWIPO, 0);
		allspace(REMAKEIPO, 0);
	}
	else if(curarea->spacetype==SPACE_VIEW3D) {
		
		base= FIRSTBASE;
		while(base) {
			if TESTBASELIB(base) break;
			base= base->next;
		}
		if(base==0) return;
		
		strcpy(menustr, "Insert Key %t|Loc%x0|Rot%x1|Size%x2|LocRot%x3|LocRotSize%x4|Layer%x5|Avail%x9");
		
		if(ob= OBACT) {
			if(ob->type==OB_MESH) strcat(menustr, "| %x6|Mesh%x7");
			else if(ob->type==OB_LATTICE) strcat(menustr, "| %x6|Lattice%x7");
			else if(ob->type==OB_CURVE) strcat(menustr, "| %x6|Curve%x7");
			else if(ob->type==OB_SURF) strcat(menustr, "| %x6|Surface%x7");
			else if(ob->type==OB_IKA) strcat(menustr, "| %x6|Effector%x8");
		}
		
		event= pupmenu(menustr);
		if(event== -1) return;
		
		if(event==7) {
			if(ob->type==OB_MESH) insert_meshkey(ob->data);
			else if ELEM(ob->type, OB_CURVE, OB_SURF) insert_curvekey(ob->data);
			else if(ob->type==OB_LATTICE) insert_lattkey(ob->data);
			
			allqueue(REDRAWIPO, 0);
			allqueue(REDRAWBUTSANIM, 0);
			return;
		}
		
		base= FIRSTBASE;
		while(base) {
			if TESTBASELIB(base) {
				id= (ID *)(base->object);
				
				/* alle curves in ipo deselect */
				if(base->object->ipo) {
					icu= base->object->ipo->curve.first;
					while(icu) {
						icu->flag &= ~IPO_SELECT;
						if(event==9) insertkey(id, icu->adrcode);
						icu= icu->next;
					}
				}
				
				if(event==0 || event==3 ||event==4) {
					insertkey(id, OB_LOC_X);
					insertkey(id, OB_LOC_Y);
					insertkey(id, OB_LOC_Z);
				}
				if(event==1 || event==3 ||event==4) {
					insertkey(id, OB_ROT_X);
					insertkey(id, OB_ROT_Y);
					insertkey(id, OB_ROT_Z);
				}
				if(event==2 || event==4) {
					insertkey(id, OB_SIZE_X);
					insertkey(id, OB_SIZE_Y);
					insertkey(id, OB_SIZE_Z);
				}
				if(event==5) {
					/* localview weghalen */
					tlay= base->object->lay;
					base->object->lay &= 0xFFFFFF;
					insertkey(id, OB_LAY);
					base->object->lay= tlay;
				}
				if(event==8) {
					/* deze patch moet omdat duplicators de positie van effg veranderen */
					Ika *ika= ob->data;
					VecMat4MulVecfl(ika->effg, ob->obmat, ika->effn);

					insertkey(id, OB_EFF_X);
					insertkey(id, OB_EFF_Y);
					insertkey(id, OB_EFF_Z);
				}
			}
			base= base->next;
		}
		allspace(REMAKEIPO, 0);
		allqueue(REDRAWIPO, 0);
		allqueue(REDRAWVIEW3D, 0);
	}
	
}


/* **************************************************** */

/* IPOKEY:
 * 
 *   er zijn drie manieren om hiermee om te gaan:
 *   1. hieronder: voor tekenen en editen in Ipo window
 *   2. voor tekenen keys in View3D (zie ipo.c en drawobject.c)
 *   3. voor editen keys in View3D (hieronder en editobject.c)
 * 
 */


void free_ipokey(ListBase *lb)
{
	IpoKey *ik;
	
	ik= lb->first;
	while(ik) {
		if(ik->data) freeN(ik->data);
		ik= ik->next;
	}
	freelistN(lb);
}


void add_to_ipokey(ListBase *lb, BezTriple *bezt, int nr, int len)
{
	IpoKey *ik, *ikn;
	
	ik= lb->first;
	while(ik) {
		
		if( ik->val==bezt->vec[1][0] ) {
			if(ik->data[nr]==0) {	/* dubbele punten! */
				ik->data[nr]= bezt;
				if(bezt->f2 & 1) ik->flag= 1;
				return;
			}
		}
		else if(ik->val > bezt->vec[1][0]) break;
		
		ik= ik->next;
	}	
	
	ikn= callocN(sizeof(IpoKey), "add_to_ipokey");	
	if(ik) insertlinkbefore(lb, ik, ikn);
	else addtail(lb, ikn);

	ikn->data= callocN(sizeof(float *)*len, "add_to_ipokey");
	ikn->data[nr]= bezt;
	ikn->val= bezt->vec[1][0];

	if(bezt->f2 & 1) ikn->flag= 1;
}

void make_ipokey_spec(ListBase *lb, Ipo *ipo)
{
	/* deze fie is voor PSXipo maken. */
	IpoCurve *icu;
	IpoKey *ik;
	BezTriple *bezt;
	int a, b, sel, desel, totvert, totcu=0;
	
	if(ipo==0 || lb==0) return;
	free_ipokey(lb);
	
	icu= ipo->curve.first;
	while(icu) {
		totcu++;
		icu= icu->next;
	}
	a= 0;
	icu= ipo->curve.first;
	while(icu) {
		if(icu->flag & IPO_VISIBLE) {
			bezt= icu->bezt;
			totvert= icu->totvert;
			
			for(b=0; b<totvert; b++, bezt++) {
				add_to_ipokey(lb, bezt, a, totcu);
			}
			a++;
		}
		icu= icu->next;
	}
}

void make_ipokey(void)
{
	EditIpo *ei;
	IpoCurve *icu;
	IpoKey *ik;
	ListBase *lb;
	BezTriple *bezt;
	int a, b, sel, desel, totvert;
	
	lb= &G.sipo->ipokey;
	free_ipokey(lb);
	
	ei= G.sipo->editipo;
	if(ei==0) return;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
			bezt= ei->icu->bezt;
			totvert= ei->icu->totvert;
			
			for(b=0; b<totvert; b++, bezt++) {
				add_to_ipokey(lb, bezt, a, G.sipo->totipo);
			}
			
			ei->flag &= ~IPO_SELECT;
			ei->flag &= ~IPO_EDIT;
			ei->icu->flag= ei->flag;
		}
	}
	
	/* selectflags testen */
	ik= lb->first;
	while(ik) {
		sel= desel= 0;
		for(a=0; a<G.sipo->totipo; a++) {
			if(ik->data[a]) {
				bezt= ik->data[a];
				if(bezt->f2 & 1) sel++;
				else desel++;
			}
		}
		if(sel && desel) sel= 0;
		for(a=0; a<G.sipo->totipo; a++) {
			if(ik->data[a]) {
				bezt= ik->data[a];
				if(sel) {
					bezt->f1 |= 1;
					bezt->f2 |= 1;
					bezt->f3 |= 1;
				}
				else {
					bezt->f1 &= ~1;
					bezt->f2 &= ~1;
					bezt->f3 &= ~1;
				}
			}
		}
		if(sel) ik->flag = 1;
		else ik->flag= 0;
		
		ik= ik->next;
	}
	get_status_editipo();
}

void make_ipokey_transform(Object *ob, ListBase *lb, int sel)
{
	IpoCurve *icu;
	BezTriple *bezt;
	int a, adrcode, ok, dloc=0, drot=0, dsize=0;
	
	if(ob->ipo==0) return;
	if(ob->ipo->showkey==0) return;
	
	/* testen: zijn er delta curves? */
	icu= ob->ipo->curve.first;
	while(icu) {
		if(icu->flag & IPO_VISIBLE) {
			switch(icu->adrcode) {
			case OB_DLOC_X:
			case OB_DLOC_Y:
			case OB_DLOC_Z:
				dloc= 1;
				break;
			case OB_DROT_X:
			case OB_DROT_Y:
			case OB_DROT_Z:
				drot= 1;
				break;
			case OB_DSIZE_X:
			case OB_DSIZE_Y:
			case OB_DSIZE_Z:
				dsize= 1;
				break;
			}
		}
		icu= icu->next;
	}

	icu= ob->ipo->curve.first;
	while(icu) {
		if(icu->flag & IPO_VISIBLE) {
			ok= 0;
			
			switch(icu->adrcode) {
			case OB_DLOC_X:
			case OB_DLOC_Y:
			case OB_DLOC_Z:
			case OB_DROT_X:
			case OB_DROT_Y:
			case OB_DROT_Z:
			case OB_DSIZE_X:
			case OB_DSIZE_Y:
			case OB_DSIZE_Z:
				ok= 1;
				break;
				
			case OB_LOC_X:
			case OB_LOC_Y:
			case OB_LOC_Z:
				if(dloc==0) ok= 1;
				break;
			case OB_ROT_X:
			case OB_ROT_Y:
			case OB_ROT_Z:
				if(drot==0) ok= 1;
				break;
			case OB_SIZE_X:
			case OB_SIZE_Y:
			case OB_SIZE_Z:
				if(dsize==0) ok= 1;
				break;
			}
			if(ok) {
				for(a=0; a<OB_TOTIPO; a++) {
					if(icu->adrcode==ob_ar[a]) {
						adrcode= a;
						break;
					}
				}
			
				bezt= icu->bezt;
				a= icu->totvert;
				while(a--) {
					if(sel==0 || (bezt->f2 & 1)) {
						add_to_ipokey(lb, bezt, adrcode, OB_TOTIPO);
					}
					bezt++;
				}
			}
		}
		icu= icu->next;
	}
}

void update_ipokey_val()	/* na verplaatsen vertices */
{
	IpoKey *ik;
	int a;
	
	ik= G.sipo->ipokey.first;
	while(ik) {
		for(a=0; a<G.sipo->totipo; a++) {
			if(ik->data[a]) {
				ik->val= ik->data[a]->vec[1][0];
				break;
			}
		}
		ik= ik->next;
	}
}

void set_tob_old(float *old, float *poin)
{
	old[0]= *(poin);
	old[3]= *(poin-3);
	old[6]= *(poin+3);
}

void set_ipo_pointers_transob(IpoKey *ik, TransOb *tob)
{
	BezTriple *bezt;
	int a, delta= 0;

	tob->locx= tob->locy= tob->locz= 0;
	tob->rotx= tob->roty= tob->rotz= 0;
	tob->sizex= tob->sizey= tob->sizez= 0;
	
	for(a=0; a<OB_TOTIPO; a++) {
		if(ik->data[a]) {
			bezt= ik->data[a];
		
			switch( ob_ar[a] ) {
			case OB_LOC_X:
			case OB_DLOC_X:
				tob->locx= &(bezt->vec[1][1]); break;
			case OB_LOC_Y:
			case OB_DLOC_Y:
				tob->locy= &(bezt->vec[1][1]); break;
			case OB_LOC_Z:
			case OB_DLOC_Z:
				tob->locz= &(bezt->vec[1][1]); break;
		
			case OB_DROT_X:
				delta= 1;
			case OB_ROT_X:
				tob->rotx= &(bezt->vec[1][1]); break;
			case OB_DROT_Y:
				delta= 1;
			case OB_ROT_Y:
				tob->roty= &(bezt->vec[1][1]); break;
			case OB_DROT_Z:
				delta= 1;
			case OB_ROT_Z:
				tob->rotz= &(bezt->vec[1][1]); break;
				
			case OB_SIZE_X:
			case OB_DSIZE_X:
				tob->sizex= &(bezt->vec[1][1]); break;
			case OB_SIZE_Y:
			case OB_DSIZE_Y:
				tob->sizey= &(bezt->vec[1][1]); break;
			case OB_SIZE_Z:
			case OB_DSIZE_Z:
				tob->sizez= &(bezt->vec[1][1]); break;		
			}	
		}
	}
	
	/* oldvals voor o.a. undo */
	if(tob->locx) set_tob_old(tob->oldloc, tob->locx);
	if(tob->locy) set_tob_old(tob->oldloc+1, tob->locy);
	if(tob->locz) set_tob_old(tob->oldloc+2, tob->locz);
		
		/* bewaar de eerste oldrot, ivm mapping curves ('1'=10 graden) en correcte berekening */
	if(tob->rotx) set_tob_old(tob->oldrot+3, tob->rotx);
	if(tob->roty) set_tob_old(tob->oldrot+4, tob->roty);
	if(tob->rotz) set_tob_old(tob->oldrot+5, tob->rotz);
	
		/* bewaar de eerste oldsize, dit mag niet de dsize zijn! */
	if(tob->sizex) set_tob_old(tob->oldsize+3, tob->sizex);
	if(tob->sizey) set_tob_old(tob->oldsize+4, tob->sizey);
	if(tob->sizez) set_tob_old(tob->oldsize+5, tob->sizez);

	tob->flag= TOB_IPO;
	if(delta) tob->flag |= TOB_IPODROT;
}



void nextkey(ListBase *elems, int dir)
{
	IpoKey *ik, *previk;
	int a, totsel;
	
	if(dir==1) ik= elems->last;
	else ik= elems->first;
	previk= 0;
	totsel= 0;
	
	while(ik) {
		
		if(ik->flag) totsel++;
		
		if(previk) {
			if(G.qual & LR_SHIFTKEY) {
				if(ik->flag) previk->flag= 1;
			}
			else previk->flag= ik->flag;
		}
		
		previk= ik;
		if(dir==1) ik= ik->prev;
		else ik= ik->next;
		
		if(G.qual & LR_SHIFTKEY);
		else if(ik==0) previk->flag= 0;
	}
	
	/* als geen een key select: */
	if(totsel==0) {
		if(dir==1) ik= elems->first;
		else ik= elems->last;
		
		if(ik) ik->flag= 1;
	}
}

static int float_to_frame (float frame) 
{
	int to= (int) frame;
	
	if (frame-to>0.5) to++;
	
	return to;	
}

void movekey_ipo(int dir)		/* alleen extern aanroepen vanuit view3d queue */
{
	IpoKey *ik;
	float toframe;
	int a;
	
	if(G.sipo->showkey==0) return;

	ik= G.sipo->ipokey.first;
	if (dir==-1) {
		while (ik && float_to_frame(ik->val)<CFRA) {
			toframe= ik->val;
			ik= ik->next;
		}
	} else {
		while (ik && float_to_frame(ik->val)<=CFRA) {
			ik= ik->next;
		}
		if (ik) toframe= ik->val;
	}
	
	a= float_to_frame(toframe);
	
	if (a!=CFRA && a>0) {
		CFRA= a;
		
		do_global_buttons(B_NEWFRAME);
	}
	
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWIPO, 0);
	allspace(REMAKEIPO, 0);
}

void movekey_obipo(int dir)		/* alleen extern aanroepen vanuit view3d queue */
{
	Base *base;
	Object *ob;
	ListBase elems;
	BezTriple *bezt;
	IpoKey *ik, *previk;
	int a, totsel;
	float toframe= CFRA;
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) {
			ob= base->object;
			if(ob->ipo && ob->ipo->showkey) {
				elems.first= elems.last= 0;
				make_ipokey_transform(ob, &elems, 0);

				if(elems.first) {
					ik= elems.first;
					if (dir==-1) {
						while (ik && float_to_frame(ik->val)<CFRA) {
							toframe= ik->val;
							ik= ik->next;
						}
					} else {
						while (ik && float_to_frame(ik->val)<=CFRA) {
							ik= ik->next;
						}
						if (ik) toframe= ik->val;
					}
										
					free_ipokey(&elems);
				}
			}
		}
		
		base= base->next;
	}
	
	a= float_to_frame(toframe);
	
	if (a!=CFRA && a>0) {
		CFRA= a;
		
		do_global_buttons(B_NEWFRAME);
	}
	
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWIPO, 0);
	allspace(REMAKEIPO, 0);
}

void nextkey_ipo(int dir)			/* aanroepen vanuit ipo queue */
{
	IpoKey *ik;
	int a;
	
	if(G.sipo->showkey==0) return;
	
	nextkey(&G.sipo->ipokey, dir);
	
	/* kopieeren naar beziers */
	ik= G.sipo->ipokey.first;
	while(ik) {
		for(a=0; a<G.sipo->totipo; a++) {
			if(ik->data[a]) ik->data[a]->f1= ik->data[a]->f2= ik->data[a]->f3= ik->flag;
		}
		ik= ik->next;
	}		

	allqueue(REDRAWIPO, 0);
	if(G.sipo->blocktype == ID_OB) allqueue(REDRAWVIEW3D, 0);
}

void nextkey_obipo(int dir)		/* alleen extern aanroepen vanuit view3d queue */
{
	Base *base;
	Object *ob;
	ListBase elems;
	BezTriple *bezt;
	IpoKey *ik, *previk;
	int a, totsel;
	
	/* problem: this doesnt work when you mix dLoc keys with Loc keys */
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) {
			ob= base->object;
			if( (ob->ipoflag & OB_DRAWKEY) && ob->ipo && ob->ipo->showkey) {
				elems.first= elems.last= 0;
				make_ipokey_transform(ob, &elems, 0);

				if(elems.first) {
					
					nextkey(&elems, dir);
					
					/* kopieeren naar beziers */
					ik= elems.first;
					while(ik) {
						for(a=0; a<OB_TOTIPO; a++) {
							if(ik->data[a]) ik->data[a]->f1= ik->data[a]->f2= ik->data[a]->f3= ik->flag;
						}
						ik= ik->next;
					}
					
					free_ipokey(&elems);
				}
			}
		}
		
		base= base->next;
	}
	allqueue(REDRAWVIEW3D, 0);
	allspace(REMAKEIPO, 0);
	allqueue(REDRAWIPO, 0);
}


/* **************************************************** */

void remake_ipo_transverts(TransVert *transmain, float *dvec, int tot)
{
	EditIpo *ei;
	TransVert *tv;
	BezTriple *bezt;
	BPoint *bp;
	int a, b;
	
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		
		if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
			if( (ei->flag & IPO_EDIT) || G.sipo->showkey) {
				
				if(ei->icu->bezt) {
					sort_time_ipocurve(ei->icu);
				}
			}
		}
	}

	ei= G.sipo->editipo;
	tv= transmain;
	for(a=0; a<G.sipo->totipo; a++, ei++) {
		
		if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
			if( (ei->flag & IPO_EDIT) || G.sipo->showkey) {
			
				if(ei->icu->bezt) {
					bezt= ei->icu->bezt;
					b= ei->icu->totvert;
					while(b--) {
						if(ei->icu->ipo==IPO_BEZ) {
							if(bezt->f1 & 1) {
								tv->loc= bezt->vec[0];
								tv++;
							}
							if(bezt->f3 & 1) {
								tv->loc= bezt->vec[2];
								tv++;
							}
						}
						if(bezt->f2 & 1) {
							tv->loc= bezt->vec[1];
							tv++;
						}
						
						bezt++;
					}
					testhandles_ipocurve(ei->icu);
				}
			}
		}
	}
	
	if(G.sipo->showkey) make_ipokey();
	
	if(dvec==0) return;
	
	tv= transmain;
	for(a=0; a<tot; a++, tv++) {
		tv->oldloc[0]= tv->loc[0]-dvec[0];
		tv->oldloc[1]= tv->loc[1]-dvec[1];
	}
}

void transform_ipo(int mode)
{
	EditIpo *ei;
	IpoCurve *icu;
	BezTriple *bezt;
	TransVert *transmain, *tv;
	float xref=1.0, yref=1.0, dx, dy, dvec[2], min[3], max[3], vec[2], div, cent[2], size[2], sizefac;
	int tot=0, a, b, firsttime=1, afbreek=0, midtog= 0, dosort, proj;
	ushort event;
	short mval[2], val, xo, yo, xn, yn, xc, yc;
	char str[32];
	
	if(G.sipo->ipo && G.sipo->ipo->id.lib) return;
	if(G.sipo->editipo==0) return;
	if(mode=='r') return;	/* vanuit gesture */
	
	INIT_MINMAX(min, max);
	
	/* welke vertices doen mee */
	get_status_editipo();
	if(totipo_vertsel) {
		tot= totipo_vertsel;
		tv=transmain= callocN(tot*sizeof(TransVert), "transmain");
		
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++, ei++) {
			
			if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
				if( (ei->flag & IPO_EDIT) || G.sipo->showkey) {

				
					if(ei->icu->bezt) {
						bezt= ei->icu->bezt;
						b= ei->icu->totvert;
						while(b--) {
							if(ei->icu->ipo==IPO_BEZ) {
								if(bezt->f1 & 1) {
									tv->loc= bezt->vec[0];
									VECCOPY(tv->oldloc, tv->loc);
									if(ei->disptype==IPO_DISPBITS) tv->flag= 1;
									
									/* let op: we nemen middelste vertex */
									DO_MINMAX2(bezt->vec[1], min, max);

									tv++;
								}
								if(bezt->f3 & 1) {
									tv->loc= bezt->vec[2];
									VECCOPY(tv->oldloc, tv->loc);
									if(ei->disptype==IPO_DISPBITS) tv->flag= 1;
									
									/* let op: we nemen middelste vertex */
									DO_MINMAX2(bezt->vec[1], min, max);

									tv++;
								}
							}
							if(bezt->f2 & 1) {
								tv->loc= bezt->vec[1];
								VECCOPY(tv->oldloc, tv->loc);
								if(ei->disptype==IPO_DISPBITS) tv->flag= 1;
								DO_MINMAX2(bezt->vec[1], min, max);
								tv++;
							}
							bezt++;
						}
					}
				}
			}
		}
		
	}
	else if(totipo_edit==0 && totipo_sel!=0) {
		
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++, ei++) {
			if ISPOIN3(ei, flag & IPO_VISIBLE, flag & IPO_SELECT, icu) {
				if(ei->icu->bezt && ei->icu->ipo==IPO_BEZ) tot+= 3*ei->icu->totvert;
				else tot+= ei->icu->totvert;
			}
		}
		if(tot==0) return;
		
		tv=transmain= callocN(tot*sizeof(TransVert), "transmain");

		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++, ei++) {
			if ISPOIN3(ei, flag & IPO_VISIBLE, flag & IPO_SELECT, icu) {
				if(ei->icu->bezt) {
					
					bezt= ei->icu->bezt;
					b= ei->icu->totvert;
					while(b--) {
						if(ei->icu->ipo==IPO_BEZ) {
							tv->loc= bezt->vec[0];
							VECCOPY(tv->oldloc, tv->loc);
							if(ei->disptype==IPO_DISPBITS) tv->flag= 1;
							tv++;
						
							tv->loc= bezt->vec[2];
							VECCOPY(tv->oldloc, tv->loc);
							if(ei->disptype==IPO_DISPBITS) tv->flag= 1;
							tv++;
						}
						tv->loc= bezt->vec[1];
						VECCOPY(tv->oldloc, tv->loc);
						if(ei->disptype==IPO_DISPBITS) tv->flag= 1;
						
						DO_MINMAX2(bezt->vec[1], min, max);
						
						tv++;
						
						bezt++;
					}
				}
			}
		}

	}

	if(tot==0) {
		if(totipo_edit==0) move_keys();
		return;
	}

	cent[0]= (min[0]+max[0])/2.0;
	cent[1]= (min[1]+max[1])/2.0;

	if(G.sipo->showkey) {
		midtog= 1;
		proj= 1;
	}
	
	ipoco_to_areaco(cent, mval);
	xc= mval[0];
	yc= mval[1];
	
	getmouseco_areawin(mval);
	xo= xn= mval[0];
	yo= yn= mval[1];
	dvec[0]= dvec[1]= 0.0;
	
	sizefac= fsqrt( (float)((yc-yn)*(yc-yn)+(xn-xc)*(xn-xc)) );
	if(sizefac<2.0) sizefac= 2.0;

	while(afbreek==0) {
		getmouseco_areawin(mval);
		if(mval[0]!=xo || mval[1]!=yo || firsttime) {
			
			if(mode=='g') {
			
				dx= mval[0]- xo;
				dy= mval[1]- yo;
	
				div= G.v2d->mask.xmax-G.v2d->mask.xmin;
				dvec[0]+= (G.v2d->cur.xmax-G.v2d->cur.xmin)*(dx)/div;
	
				div= G.v2d->mask.ymax-G.v2d->mask.ymin;
				dvec[1]+= (G.v2d->cur.ymax-G.v2d->cur.ymin)*(dy)/div;
				
				if(midtog) dvec[proj]= 0.0;
				
				/* vec wordt verderop nog gebruikt: remake_ipo_transverts */
				vec[0]= dvec[0];
				vec[1]= dvec[1];
				
				apply_keyb_grid(vec, 0.0, 1.0, 0.1, U.flag & AUTOGRABGRID);
				apply_keyb_grid(vec+1, 0.0, 1.0, 0.1, 0);
				
				tv= transmain;
				for(a=0; a<tot; a++, tv++) {
					tv->loc[0]= tv->oldloc[0]+vec[0];

					if(tv->flag==0) tv->loc[1]= tv->oldloc[1]+vec[1];
				}
				
				sprintf(str, "X: %.3f   Y: %.3f  ", vec[0], vec[1]);
				headerprint(str);
			}
			else if(mode=='s') {
				
				size[0]=size[1]= (fsqrt( (float)((yc-mval[1])*(yc-mval[1])+(mval[0]-xc)*(mval[0]-xc)) ))/sizefac;
				
				if(midtog) size[proj]= 1.0;
				size[0]*= xref;
				size[1]*= yref;
				
				apply_keyb_grid(size, 0.0, 0.2, 0.1, U.flag & AUTOSIZEGRID);
				apply_keyb_grid(size+1, 0.0, 0.2, 0.1, U.flag & AUTOSIZEGRID);

				tv= transmain;

				for(a=0; a<tot; a++, tv++) {
					tv->loc[0]= size[0]*(tv->oldloc[0]-cent[0])+ cent[0];
					if(tv->flag==0) tv->loc[1]= size[1]*(tv->oldloc[1]-cent[1])+ cent[1];
				}
				
				sprintf(str, "sizeX: %.3f   sizeY: %.3f  ", size[0], size[1]);
				headerprint(str);
				
			}
			
			xo= mval[0];
			yo= mval[1];
				
			dosort= 0;
			ei= G.sipo->editipo;
			for(a=0; a<G.sipo->totipo; a++, ei++) {
				if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
					
					/* let op: als de tijd verkeerd is: niet de handles corrigeren */
					if (test_time_ipocurve(ei->icu) ) dosort++;
					else testhandles_ipocurve(ei->icu);
				}
			}
			
			if(dosort) {
				if(mode=='g') remake_ipo_transverts(transmain, vec, tot);
				else remake_ipo_transverts(transmain, 0, tot);
			}
			if(G.sipo->showkey) update_ipokey_val();
			
			calc_ipo(G.sipo->ipo, (float)CFRA);

			/* update realtime */
			if(G.sipo->lock) {
				if(G.sipo->blocktype==ID_MA) {
					force_draw_plus(SPACE_BUTS);
				}
				else if(G.sipo->blocktype==ID_KE) {
					do_ob_key(OBACT);
					makeDispList(OBACT);
					force_draw_plus(SPACE_VIEW3D);
				}
				else if(G.sipo->blocktype==ID_OB) {
					Base *base= FIRSTBASE;
					
					while(base) {
						if(base->object->ipo==G.sipo->ipo) do_ob_ipo(base->object);
						base= base->next;
					}
					force_draw_plus(SPACE_VIEW3D);
				}
				else force_draw();
			}
			else {
				force_draw();
			}
			firsttime= 0;
		}
		else usleep(1);
		
		while(qtest()) {
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
					if(G.sipo->showkey==0) {
						midtog= ~midtog;
						if(midtog) {
							if( abs(mval[0]-xn) > abs(mval[1]-yn)) proj= 1;
							else proj= 0;
							firsttime= 1;
						}
					}
					break;
				case XKEY:
				case YKEY:
					if(event==XKEY) xref= -xref;
					else if(G.sipo->showkey==0) yref= -yref;
					firsttime= 1;
					break;
				case LEFTCTRLKEY:
				case RIGHTCTRLKEY:
					firsttime= 1;
					break;
				default:
					if(mode=='g') {
						if(G.qual & LR_CTRLKEY) {
							if(event==LEFTARROWKEY) {dvec[0]-= 1.0; firsttime= 1;}
							else if(event==RIGHTARROWKEY) {dvec[0]+= 1.0; firsttime= 1;}
							else if(event==UPARROWKEY) {dvec[1]+= 1.0; firsttime= 1;}
							else if(event==DOWNARROWKEY) {dvec[1]-= 1.0; firsttime= 1;}
						}
						else arrowsmovecursor(event);
					}
					else arrowsmovecursor(event);
				}
			}
			if(afbreek) break;
		}
	}
	
	if(event==ESCKEY || event==RIGHTMOUSE) {
		tv= transmain;
		for(a=0; a<tot; a++, tv++) {
			tv->loc[0]= tv->oldloc[0];
			tv->loc[1]= tv->oldloc[1];
		}
		
		dosort= 0;
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++, ei++) {
			if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
				if( (ei->flag & IPO_EDIT) || G.sipo->showkey) {
					if( test_time_ipocurve(ei->icu)) {
						dosort= 1;
						break;
					}
				}
			}
		}
		
		if(dosort) remake_ipo_transverts(transmain, 0, tot);
		
		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++, ei++) {
			if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
				if( (ei->flag & IPO_EDIT) || G.sipo->showkey) {
					testhandles_ipocurve(ei->icu);
				}
			}
		}
		calc_ipo(G.sipo->ipo, (float)CFRA);
	}
	
	editipo_changed(1);

	freeN(transmain);
}

void clever_numbuts_ipo()
{
	BezTriple *bezt=0, *bezt1;
	Key *key;
	KeyBlock *kb;
	EditIpo *ei;
	float far, delta[3], old[3];
	int tot, a, b, scale10=0, totbut=2;

	if(G.sipo->ipo && G.sipo->ipo->id.lib) return;
	if(G.sipo->editipo==0) return;
	
	/* welke vertices doen mee */
	get_status_editipo();

	if(G.qual & LR_SHIFTKEY) totbut= 1;

	if(G.vd==0) far= 10000.0;
	else far= MAX2(G.vd->clipend, 10000.0);

	if(totipo_vertsel) {

		ei= G.sipo->editipo;
		for(a=0; a<G.sipo->totipo; a++, ei++) {
			
			if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
				if( (ei->flag & IPO_EDIT) || G.sipo->showkey) {

					if(ei->icu->bezt) {
						bezt1= ei->icu->bezt;
						b= ei->icu->totvert;
						while(b--) {
							if(BEZSELECTED(bezt1)) {
								bezt= bezt1;
								break;
							}
							bezt1++;
						}
						
					}
				}
			}
			if(bezt) break;
		}
		
		if(bezt==0) return;

		if(bezt->f2 & 1) {
			
			VECCOPY(old, bezt->vec[1]);
			
			if(totipo_vis==1 && G.sipo->blocktype==ID_OB) {
				if ELEM4(ei->icu->adrcode, OB_TIME, OB_ROT_X, OB_ROT_Y, OB_ROT_Z) scale10= 1;
				if ELEM3(ei->icu->adrcode, OB_DROT_X, OB_DROT_Y, OB_DROT_Z) scale10= 1;
			}
			if(scale10) bezt->vec[1][1]*= 10.0;

			add_numbut(0, NUM|FLO, "LocX:", -1000, 10000, bezt->vec[1], 0);
			if(totbut==2) add_numbut(1, NUM|FLO, "LocY:", -far, far, bezt->vec[1]+1, 0);
			do_clever_numbuts("Active BezierPoint", totbut, REDRAW);

			if(scale10) bezt->vec[1][1]/= 10.0;

			VecSubf(delta, bezt->vec[1], old);
			VECCOPY(bezt->vec[1], old);
			
			/* apply */
			ei= G.sipo->editipo;
			for(a=0; a<G.sipo->totipo; a++, ei++) {
				if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
					if( (ei->flag & IPO_EDIT) || G.sipo->showkey) {
						if(ei->icu->bezt) {
							bezt= ei->icu->bezt;
							b= ei->icu->totvert;
							while(b--) {
								if(bezt->f2 & 1) {
									bezt->vec[0][0]+= delta[0];
									bezt->vec[1][0]+= delta[0];
									bezt->vec[2][0]+= delta[0];

									bezt->vec[0][1]+= delta[1];
									bezt->vec[1][1]+= delta[1];
									bezt->vec[2][1]+= delta[1];
								}
								bezt++;
							}
						}
					}
				}
			}

			ei= G.sipo->editipo;
			for(a=0; a<G.sipo->totipo; a++, ei++) {
				if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
					sort_time_ipocurve(ei->icu);
					testhandles_ipocurve(ei->icu);
				}
			}

		}
		else if(bezt->f1 & 1) {
			add_numbut(0, NUM|FLO, "LocX:", -1000, 10000, bezt->vec[0], 0);
			if(totbut==2) add_numbut(1, NUM|FLO, "LocY:", -far, far, bezt->vec[0]+1, 0);
		
			do_clever_numbuts("Active HandlePoint", totbut, REDRAW);
		}
		else if(bezt->f3 & 1) {
			add_numbut(0, NUM|FLO, "LocX:", -1000, 10000, bezt->vec[0], 0);
			if(totbut==2) add_numbut(1, NUM|FLO, "LocY:", -far, far, bezt->vec[2]+1, 0);
		
			do_clever_numbuts("Active HandlePoint", totbut, REDRAW);
		}

		editipo_changed(1);
	}
	else {
		
		if(G.sipo->blocktype==ID_KE) {
			key= (Key *)G.sipo->from;
			
			if(key==0) return;
			
			kb= key->block.first;
			while(kb) {
				if(kb->flag & SELECT) break;
				kb= kb->next;
			}			
			if(kb && G.sipo->rowbut&1) {
				add_numbut(0, NUM|FLO, "Pos:", -100, 100, &kb->pos, 0);
				do_clever_numbuts("Active Key", 1, REDRAW);		
				sort_keys(key);
			}
		}
	}
}

void filter_sampledata(float *data, int sfra, int efra)
{
	float *da;
	int a;
	
	da= data+1;
	for(a=sfra+1; a<efra; a++, da++) {
		da[0]= 0.25*da[-1] + 0.5*da[0] + 0.25*da[1];
	}
	
}

void sampledata_to_ipocurve(float *data, int sfra, int efra, IpoCurve *icu)
{
	BezTriple *bezt;
	float *da;
	int a, tot;
	
	filter_sampledata(data, sfra, efra);
	filter_sampledata(data, sfra, efra);
	
	icu->ipo= IPO_LIN;
	
	if(icu->bezt) freeN(icu->bezt);
	icu->bezt= 0;
	
	tot= 1;	/* eerste punt */
	da= data+1;
	for(a=sfra+1; a<efra; a++, da++) {
		if( IS_EQ(da[0], da[1])==0 && IS_EQ(da[1], da[2])==0 ) tot++;
	}
	
	icu->totvert= tot;
	bezt= icu->bezt= callocN(tot*sizeof(BezTriple), "samplebezt");
	bezt->vec[1][0]= sfra;
	bezt->vec[1][1]= data[0];
	bezt++;
	da= data+1;
	for(a=sfra+1; a<efra; a++, da++) {
		if( IS_EQ(da[0], da[1])==0 && IS_EQ(da[1], da[2])==0 ) {
			bezt->vec[1][0]= a;
			bezt->vec[1][1]= da[0];
			bezt++;
		}
	}	
}

void ipo_record()
{
	/* 1 of 2 aktieve curves
	 * kopie maken (ESC) 
	 *
	 * nulpunt is de huidige stand (of 0)
	 * dx (dy identiek) is de hoogteverhouding
	 * CTRL start record
	 */
	extern double tottime;
	EditIpo *ei, *ei1=0, *ei2=0;
	ScrArea *sa, *oldarea;
	Ipo *ipo;
	void *poin;
	double swaptime;
	float or1, or2, fac, *data1, *data2;
	int type, a, afbreek=0, firsttime=1, cfrao, cfra, sfra, efra;
	ushort event;
	short anim, val, xn, yn, mvalo[2], mval[2];
	char str[128];
	
	if(G.sipo->from==0) return;
	if(SFRA>=EFRA) return;
	
	anim= pupmenu("Record Mouse %t|Still %x1|Play anim %x2");
	if(anim < 1) return;
	if(anim!=2) anim= 0;

	ipo= get_ipo(G.sipo->from, 1);	/* 1= make */
	if(G.sipo) G.sipo->ipo= ipo;

	/* find the curves... */
	
	ei= G.sipo->editipo;
	for(a=0; a<G.sipo->totipo; a++) {
		if(ei->flag & IPO_VISIBLE) {
			
			if(ei1==0) ei1= ei;
			else if(ei2==0) ei2= ei;
			else {
				error("Max 2 visible curves");
				return;
			}
		}
		ei++;
	}

	if(ei1==0) {
		error("Select 1 or 2 channels");
		return;
	}
	
	/* curves gereedmaken, startwaardes */
	if(ei1->icu==0) ei1->icu= get_ipocurve(G.sipo->from, ei1->adrcode);
	if(ei1->icu==0) return;
	poin= get_ipo_poin(G.sipo->from, ei1->icu, &type);
	if(poin) ei1->icu->curval= read_ipo_poin(poin, type);
	or1= ei1->icu->curval;
	ei1->icu->flag |= IPO_LOCK;
	
	if(ei2) {
		if(ei2->icu==0)  ei2->icu= get_ipocurve(G.sipo->from, ei2->adrcode);
		if(ei2->icu==0) return;
		poin= get_ipo_poin(G.sipo->from, ei2->icu, &type);
		if(poin) ei2->icu->curval= read_ipo_poin(poin, type);
		or2= ei2->icu->curval;
		ei2->icu->flag |= IPO_LOCK;
	}

	fac= G.v2d->cur.ymax - G.v2d->cur.ymin;
	fac/= (float)curarea->winy;

	/* welke area */
	oldarea= curarea;
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->win && sa->windraw) {
			if(G.sipo->blocktype==ID_MA || G.sipo->blocktype==ID_LA) {
				if(sa->spacetype==SPACE_BUTS) break;
			}
			else {
				if(sa->spacetype==SPACE_VIEW3D) break;
			}
		}		
		sa= sa->next;	
	}
	if(sa) areawinset(sa->win);
	
	/* kandie? */
	while(get_mbut()&L_MOUSE) usleep(2);
	data1= callocN(sizeof(float)*(EFRA-SFRA+1), "data1");
	data2= callocN(sizeof(float)*(EFRA-SFRA+1), "data2");
	
	getmouseco_areawin(mvalo);
	xn= mvalo[0]; yn= mvalo[1];
	waitcursor(1);
	
	tottime= 0.0;
	swaptime= speed_to_swaptime(G.animspeed);
	cfrao= CFRA;
	cfra=efra= SFRA;
	sfra= EFRA;
	
	while(afbreek==0) {
		
		getmouseco_areawin(mval);
		
		if(mval[0]!= mvalo[0] || mval[1]!=mvalo[1] || firsttime || (G.qual & LR_CTRLKEY)) {
			if(anim) CFRA= cfra;
			else firsttime= 0;

			set_timecursor(cfra);
			
			/* ipo doen: eerst alles daarna de specifieke */
			if(anim==2) {
				do_all_ipos();
				do_all_keys();
			}

			ei1->icu->curval= or1 + fac*(mval[0]-xn);
			if(ei2) ei2->icu->curval= or2 + fac*(mval[1]-yn);

			do_ipo_nocalc(G.sipo->ipo);
			do_all_visible_ikas();
			
			if(G.qual & LR_CTRLKEY) {
				sprintf(str, "Recording... %d\n", cfra);
				data1[ cfra-SFRA ]= ei1->icu->curval;
				if(ei2) data2[ cfra-SFRA ]= ei2->icu->curval;
				
				sfra= MIN2(sfra, cfra);
				efra= MAX2(efra, cfra);
			}
			else sprintf(str, "Mouse Recording. Use CTRL to start. LeftMouse or Space to end");
			
			do_ob_key(OBACT);

			headerprint(str);

			if(sa) sa->windraw();

			/* minimaal swaptime laten voorbijgaan */
			tottime -= swaptime;
			while (update_time()) usleep(1);

			screen_swapbuffers();
			
			tottime= 0.0;
			
			mvalo[0]= mval[0];
			mvalo[1]= mval[1];
			
			if(anim || (G.qual & LR_CTRLKEY)) {
				cfra++;
				if(cfra>EFRA) cfra= SFRA;
			}
		}
		
		while(qtest()) {
			event= extern_qread(&val);
			if(val) {
				switch(event) {
				case LEFTMOUSE: case ESCKEY: case SPACEKEY: case RETKEY:
					afbreek= 1;
					break;
				}
			}
			if(afbreek) break;
		}
	}
	
	if(event!=ESCKEY) {
		sampledata_to_ipocurve(data1+sfra-SFRA, sfra, efra, ei1->icu);
		if(ei2) sampledata_to_ipocurve(data2+sfra-SFRA, sfra, efra, ei2->icu);

		/* vervelend als dat aanstaat */
		if(G.sipo->showkey) {
			G.sipo->showkey= 0;
			free_ipokey(&G.sipo->ipokey);
		}
	}
	else {
		/* undo: startwaardes */
		poin= get_ipo_poin(G.sipo->from, ei1->icu, &type);
		if(poin) write_ipo_poin(poin, type, or1);
		if(ei1->icu->bezt==0) {
			remlink( &(G.sipo->ipo->curve), ei1->icu);
			freeN(ei1->icu);
			ei1->icu= 0;
		}
		if(ei2) {
			poin= get_ipo_poin(G.sipo->from, ei2->icu, &type);
			if(poin) write_ipo_poin(poin, type, or2);
			if(ei2->icu->bezt==0) {
				remlink( &(G.sipo->ipo->curve), ei2->icu);
				freeN(ei2->icu);
				ei2->icu= 0;
			}
		}
	}
	
	if(ei1->icu) ei1->icu->flag &= ~IPO_LOCK;	
	if(ei2 && ei2->icu) ei2->icu->flag &= ~IPO_LOCK;	
	
	editipo_changed(0);
	do_ipo(G.sipo->ipo);
	waitcursor(0);
	allqueue(REDRAWVIEW3D, 0);
	if(sa) addqueue(sa->headwin, REDRAW, 1);	/* headerprint */
	addqueue(oldarea->headwin, REDRAW, 1);
	addqueue(oldarea->win, REDRAW, 1);
	CFRA= cfrao;
	
	/* vooropig? */
	do_global_buttons(B_NEWFRAME);
	
	freeN(data1);
	freeN(data2);
}




