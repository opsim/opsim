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


/*  editsector.c     GRAPHICS
 * 
 *  maart 96
 *  
 * 
 */

#ifndef FREE

#include "blender.h"
#include "graphics.h"
#include "sector.h"

#define PORTALDIST	0.25


int nearest_portal(Object *sectob, float *centcur)
{
	Sector *se;
	Portal *po;
	float dist, size, cent[3];
	int b;
	
	if(sectob==0) return 0;
	
	se= sectob->data;
	po= se->portals;
	b= se->totport;

	size= PORTALDIST*MAX3(se->size[0], se->size[1], se->size[2]);
	if(size>1.5) size= 1.5;
	
	while(b--) {
		
		get_co_portal(se, po->type, po->ofs, cent);
		Mat4MulVecfl(sectob->obmat, cent);

		dist= VecLenf(cent, centcur);
		if(dist<=size) return 1;

		po++;
	}
	
	return 0;
}

void portal_detect(Object *ob)
{
	Portal *po, *pocur;
	Base *base;
	Sector *secur;
	float centcur[3];
	int a, b;
	
	/* alle portals van deze sector testen */
	
	secur= ob->data;
	/* ach ja, je weet nooit (na dupli bijv) */
	secur->ob= ob;
	
	/* voor alle portals... */
	pocur= secur->portals;
	a= secur->totport;
	while(a--) {
		
		get_co_portal(secur, pocur->type, pocur->ofs, centcur);
		Mat4MulVecfl(ob->obmat, centcur);
		
		/* geldt huidige portal nog? */
		if(pocur->sector && pocur->sector->ob) {
			if( (ob->lay & pocur->sector->ob->lay) && ( nearest_portal(pocur->sector->ob, centcur)) );
			else pocur->sector= 0;
		}
		
		/* als portals niet (meer) ergens naar wijzen */
		if(pocur->sector==0) {
		
			/* lopen we alle sectoren af... */
			base= FIRSTBASE;
			while(base) {
				if(base->lay & ob->lay) {
					if(base->object->type==OB_SECTOR && ob!=base->object) {
						
						if( nearest_portal(base->object, centcur) ) {
							pocur->sector= base->object->data;
							break;
						}
					}
				}
				base= base->next;
			}
		}
		pocur++;
	}
}

void sector_calc_imat()
{
	Base *base= FIRSTBASE;
	while(base) {
		if(base->lay & G.vd->lay) {
			if(base->object->type==OB_SECTOR) {
				Mat4Invert(base->object->imat, base->object->obmat);
			}
		}
		base= base->next;
	}
}

void sector_update(int testsel)
{
	Life *lf;
	Sector *se;
	Base *base;
	int a;
	
	/* tijdens grab etc.:
	 * 
	 *  - lifes inside sector testen
	 *  - portals
	 *  
	 */
	
	/* voor light bijv */
	sector_calc_imat();
	
	base= FIRSTBASE;
	while(base) {
		if(base->lay & G.vd->lay) {
			if(testsel==0 || (base->flag & SELECT)) {
				if(base->object->type==OB_SECTOR) {
					se= base->object->data;
					se->ob= base->object;

					portal_detect(base->object);
					
				}
				else if(base->object->type==OB_LIFE) {
					
					/* voor light */
					Mat4Invert(base->object->imat, base->object->obmat);
					
					life_in_sector_test(base->object);
				}
			}
		}
		base= base->next;
	}
	
}

/* ************************ TEXTURE *********************** */

TFace *lasttface=0;

void set_lasttface()
{
	Mesh *me;
	TFace *tface;
	int a;
	
	lasttface= 0;
	
	me= get_mesh(OBACT);
	if(me==0 || me->tface==0) return;
	
	tface= me->tface;
	a= me->totface;
	while(a--) {
		if(tface->flag & ACTIVE) {
			lasttface= tface;
			return;
		}
		tface++;
	}
}

void default_uv(TFace *tface, int size)
{
	int dy;
	
	if(size>32767) size= 32767;

	if(size<32760) dy= 32767-size;
	else dy= 0;
	
	tface->uv[0][0]= 0;
	tface->uv[0][1]= size+dy;
	
	tface->uv[1][0]= 0;
	tface->uv[1][1]= dy;
	
	tface->uv[2][0]= size;
	tface->uv[2][1]= dy;
	
	tface->uv[3][0]= size;
	tface->uv[3][1]= size+dy;
	
	
}

void default_tface(TFace *tface)
{
	default_uv(tface, 32767);
	tface->col[0]= tface->col[1]= tface->col[2]= tface->col[3]= 0x808080;
	tface->mode= TF_GOUR+TF_TEX;
	tface->flag= 0;
	tface->tpage= 0;
}

void make_tfaces(Mesh *me)
{
	TFace *tface;
	int a;
	
	a= me->totface;
	if(a==0) return;
	tface= me->tface= callocN(a*sizeof(TFace), "tface");
	while(a--) {
		default_tface(tface);
		tface->mode = TF_GOUR;
		tface++;
	}
	if(me->mcol) {
		mcol_to_tface(me, 1);
	}
}

void reveal_tface()
{
	Mesh *me;
	TFace *tface;
	int a;
	
	me= get_mesh(OBACT);
	if(me==0 || me->tface==0 || me->totface==0) return;
	
	tface= me->tface;
	a= me->totface;
	while(a--) {
		if(tface->flag & TF_HIDE) {
			tface->flag |= SELECT;
			tface->flag -= TF_HIDE;
		}
		tface++;
	}

	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWIMAGE, 0);
}



void hide_tface()
{
	Mesh *me;
	TFace *tface;
	int a;
	
	me= get_mesh(OBACT);
	if(me==0 || me->tface==0 || me->totface==0) return;
	
	if(G.qual & LR_ALTKEY) {
		reveal_tface();
		return;
	}
	
	tface= me->tface;
	a= me->totface;
	while(a--) {
		if(tface->flag & TF_HIDE);
		else {
			if(G.qual & LR_SHIFTKEY) {
				if( (tface->flag & SELECT)==0) tface->flag |= TF_HIDE;
			}
			else {
				if( (tface->flag & SELECT)) tface->flag |= TF_HIDE;
			}
		}
		if(tface->flag & TF_HIDE) tface->flag &= ~SELECT;
		
		tface++;
	}
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWIMAGE, 0);
	
}

void select_linked_tfaces()
{
	Mesh *me;
	TFace *tface;
	MFace *mface;
	int a, doit=1;
	char *cpmain, *cp;
	
	me= get_mesh(OBACT);
	if(me==0 || me->tface==0 || me->totface==0) return;
	
	cp= cpmain= callocN(me->totvert, "cpmain");
	
	while(doit) {
		doit= 0;
		
		/* select connected: array vullen */
		tface= me->tface;
		mface= me->mface;
		a= me->totface;
		while(a--) {
			if(tface->flag & TF_HIDE);
			else if(tface->flag & SELECT) {
				if( mface->v3) {
					cpmain[mface->v1]= 1;
					cpmain[mface->v2]= 1;
					cpmain[mface->v3]= 1;
					if(mface->v4) cpmain[mface->v4]= 1;
				}
			}
			tface++; mface++;
		}
		
		/* omgekeerd: vanuit array vlakken selecteren */

		tface= me->tface;
		mface= me->mface;
		a= me->totface;
		while(a--) {
			if(tface->flag & TF_HIDE);
			else if((tface->flag & SELECT)==0) {
				if( mface->v3) {
					if(mface->v4) {
						if(cpmain[mface->v4]) {
							tface->flag |= SELECT;
							doit= 1;
						}
					}
					if( cpmain[mface->v1] || cpmain[mface->v2] || cpmain[mface->v3] ) {
						tface->flag |= SELECT;
						doit= 1;
					}
				}
			}
			tface++; mface++;
		}
		
	}
	freeN(cpmain);
	
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWIMAGE, 0);
	
}

void deselectall_tface()
{
	Mesh *me;
	TFace *tface;
	int a, sel;
		
	me= get_mesh(OBACT);
	if(me==0 || me->tface==0) return;
	
	tface= me->tface;
	a= me->totface;
	sel= 0;
	while(a--) {
		if(tface->flag & TF_HIDE);
		else if(tface->flag & SELECT) sel= 1;
		tface++;
	}
	
	tface= me->tface;
	a= me->totface;
	while(a--) {
		if(tface->flag & TF_HIDE);
		else {
			if(sel) tface->flag &= ~SELECT;
			else tface->flag |= SELECT;
		}
		tface++;
	}
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWIMAGE, 0);

}

void rotate_uv_tface()
{
	Mesh *me;
	TFace *tface;
	MFace *mface;
	int a, u1, v1;
	short mode;
	
	me= get_mesh(OBACT);
	if(me==0 || me->tface==0) return;
	
	mode= pupmenu("OK? %t|Rot UV %x1| Rot VertexCol %x2");
	
	if(mode<1) return;
	
	tface= me->tface;
	mface= me->mface;
	a= me->totface;
	while(a--) {
		if(tface->flag & SELECT) {
			if(mode==1) {
				u1= tface->uv[0][0];
				v1= tface->uv[0][1];
				
				tface->uv[0][0]= tface->uv[1][0];
				tface->uv[0][1]= tface->uv[1][1];
	
				tface->uv[1][0]= tface->uv[2][0];
				tface->uv[1][1]= tface->uv[2][1];
	
				if(mface->v4) {
					tface->uv[2][0]= tface->uv[3][0];
					tface->uv[2][1]= tface->uv[3][1];
				
					tface->uv[3][0]= u1;
					tface->uv[3][1]= v1;
				}
				else {
					tface->uv[2][0]= u1;
					tface->uv[2][1]= v1;
				}
			}
			else if(mode==2) {
				u1= tface->col[0];
				
				tface->col[0]= tface->col[1];
				tface->col[1]= tface->col[2];
	
				if(mface->v4) {
					tface->col[2]= tface->col[3];
					tface->col[3]= u1;
				}
				else {
					tface->col[2]= u1;
				}
			}
		}
		tface++;
		mface++;
	}
	
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWIMAGE, 0);
}



void face_select()
{
	Object *ob;
	Mesh *me;
	TFace *tface, *tsel;
	uint col;
	int a, index;
	short mval[2];
	
	ob= OBACT;
	me= get_mesh(ob);
	if(me==0 || me->tface==0) return;
	
	if(ob->lay & G.vd->lay); else error("Active object not in this layer!");

	if(curarea->win_swap==WIN_EQUAL) G.vd->flag |= V3D_NEEDBACKBUFDRAW;
	
	if(G.vd->flag & V3D_NEEDBACKBUFDRAW) {
		backdrawview3d(0);
	}

	getmouseco_areawin(mval);
	glReadPixels(mval[0]+curarea->winrct.xmin,  mval[1]+curarea->winrct.ymin, ( mval[0]+curarea->winrct.xmin)-(mval[0]+curarea->winrct.xmin)+1, ( mval[1]+curarea->winrct.ymin)-( mval[1]+curarea->winrct.ymin)+1, GL_RGBA, GL_UNSIGNED_BYTE,  &col);
	if(G.order==B_ENDIAN) SWITCH_INT(col);
PRINT(x, col);	
	col &= 0xF0F0F0;
	index= ( (col & 0xF00000) >>12) + ((col & 0xF000)>>8) + ((col & 0xF0)>>4);

	if(index<=0 || index>me->totface) return;
	
	tsel= ( (TFace *)me->tface ) + (index-1);

	if(tsel->flag & TF_HIDE) return;
	
	/* clear flags */
	tface= me->tface;
	a= me->totface;
	while(a--) {
		
		if(G.qual & LR_SHIFTKEY) tface->flag &= ~ACTIVE;
		else tface->flag &= ~(ACTIVE+SELECT);
		
		tface++;
	}
	
	tsel->flag |= ACTIVE;
	
	if(G.qual & LR_SHIFTKEY) {
		if(tsel->flag & SELECT) tsel->flag &= ~SELECT;
		else tsel->flag |= SELECT;
	}
	else tsel->flag |= SELECT;
	
	lasttface= tsel;
	
	/* image window redraw */
	allqueue(REDRAWIMAGE, 0);
	allqueue(REDRAWBUTSGAME, 0);
	allqueue(REDRAWVIEW3D, 0);
}


void face_borderselect()
{
	Mesh *me;
	TFace *tface;
	rcti rect;
	uint *rectm, *rt;
	int a, sx, sy, index, val;
	char *selar;
	
	me= get_mesh(OBACT);
	if(me==0 || me->tface==0) return;
	if(me->totface==0) return;
	
	val= get_border(&rect, 3);
	
	if(val) {
		selar= callocN(me->totface, "selar");
		
		sx= (rect.xmax-rect.xmin+1);
		sy= (rect.ymax-rect.ymin+1);
		if(sx*sy<=0) return;
		
		rt=rectm= mallocN(sizeof(int)*sx*sy, "selrect");
		glReadPixels(rect.xmin+curarea->winrct.xmin,  rect.ymin+curarea->winrct.ymin, ( rect.xmax+curarea->winrct.xmin)-(rect.xmin+curarea->winrct.xmin)+1, ( rect.ymax+curarea->winrct.ymin)-( rect.ymin+curarea->winrct.ymin)+1, GL_RGBA, GL_UNSIGNED_BYTE,  rectm);
		if(G.order==B_ENDIAN) convert_rgba_to_abgr(sx*sy, rectm);

		a= sx*sy;
		while(a--) {
			index= ( (*rt & 0xF00000) >>12) + ((*rt & 0xF000)>>8) + ((*rt & 0xF0)>>4);
			if(index && index<=me->totface) selar[index-1]= 1;
			
			rt++;
		}
		
		tface= me->tface;
		for(a=0; a<me->totface; a++, tface++) {
			if(selar[a]) {
				if(tface->flag & TF_HIDE);
				else {
					if(val==LEFTMOUSE) tface->flag |= SELECT;
					else tface->flag &= ~SELECT;
				}
			}
		}
		
		freeN(rectm);
		freeN(selar);
		allqueue(REDRAWVIEW3D, 0);
		allqueue(REDRAWIMAGE, 0);
	}
}

void uv_autocalc_tface()
{
	Mesh *me;
	TFace *tface;
	MFace *mface;
	MVert *mv;
	Object *ob;
	extern float cumapsize;	/* buttons.c */
	float no[3], *loc, mat[4][4];
	int a, b, uvn, uvi[4][2], dx, dy, ycor;
	short min[2], max[2], cox, coy, mode, adr[2];
	
	me= get_mesh(ob=OBACT);
	if(me==0 || me->tface==0) return;
	if(me->totface==0) return;
	
	mode= pupmenu("UV Calculation %t|Cube %x2|Cylinder %x3|Sphere %x4|Bounds to 64 %x64|Bounds to 128 %x128|Standard 64 %x65|Standard 128 %x129|Standard 256 %x257| From Window %x5");
	if(mode<1) return;

	if(mode==5 || mode==64 || mode==128) {		/* standard 64/128: eerst window proj */
		multmatrix(ob->obmat);
		Mat4SwapMat4(G.vd->persmat, mat);
		mygetsingmatrix(G.vd->persmat);
		
		tface= me->tface;
		mface= me->mface;
		for(a=0; a<me->totface; a++, mface++, tface++) {
			if(tface->flag & SELECT) {
				
				if(mface->v3==0) continue;
				
				project_short( (me->mvert+mface->v1)->co, adr);
				if(adr[0]!=3200) {
					tface->uv[0][0]= (adr[0]*32767)/curarea->winx;
					tface->uv[0][1]= (adr[1]*32767)/curarea->winy;
				}
				project_short( (me->mvert+mface->v2)->co, adr);
				if(adr[0]!=3200) {
					tface->uv[1][0]= (adr[0]*32767)/curarea->winx;
					tface->uv[1][1]= (adr[1]*32767)/curarea->winy;
				}
				project_short( (me->mvert+mface->v3)->co, adr);
				if(adr[0]!=3200) {
					tface->uv[2][0]= (adr[0]*32767)/curarea->winx;
					tface->uv[2][1]= (adr[1]*32767)/curarea->winy;
				}
				if(mface->v4) {
					project_short( (me->mvert+mface->v4)->co, adr);
					if(adr[0]!=3200) {
						tface->uv[3][0]= (adr[0]*32767)/curarea->winx;
						tface->uv[3][1]= (adr[1]*32767)/curarea->winy;
					}
				}
			}
		}
	}
	
	if(mode==5);		/* even afvangen ivm laatste else */
	else if(mode==2) {
		tface= me->tface;
		mface= me->mface;
		mv= me->mvert;
		loc= ob->obmat[3];
		
		for(a=0; a<me->totface; a++, mface++, tface++) {
			if(tface->flag & SELECT) {
				if(mface->v3==0) continue;
				
				CalcNormFloat((mv+mface->v1)->co, (mv+mface->v2)->co, (mv+mface->v3)->co, no);
				
				no[0]= fabs(no[0]);
				no[1]= fabs(no[1]);
				no[2]= fabs(no[2]);
				
				cox=0; coy= 1;
				if(no[2]>=no[0] && no[2]>=no[1]);
				else if(no[1]>=no[0] && no[1]>=no[2]) coy= 2;
				else { cox= 1; coy= 2;}
				
				uvi[0][0]= 16384+16384*cumapsize*(loc[cox] + (mv+mface->v1)->co[cox]);
				uvi[0][1]= 16384+16384*cumapsize*(loc[coy] + (mv+mface->v1)->co[coy]);
				uvi[1][0]= 16384+16384*cumapsize*(loc[cox] + (mv+mface->v2)->co[cox]);
				uvi[1][1]= 16384+16384*cumapsize*(loc[coy] + (mv+mface->v2)->co[coy]);
				uvi[2][0]= 16384+16384*cumapsize*(loc[cox] + (mv+mface->v3)->co[cox]);
				uvi[2][1]= 16384+16384*cumapsize*(loc[coy] + (mv+mface->v3)->co[coy]);
				if(mface->v4) {
					uvi[3][0]= 16384+16384*cumapsize*(loc[cox] + (mv+mface->v4)->co[cox]);
					uvi[3][1]= 16384+16384*cumapsize*(loc[coy] + (mv+mface->v4)->co[coy]);
				}
				
				dx= dy= 0;
				if(mface->v4) b= 3; else b= 2;
				for(; b>=0; b--) {
					while(uvi[b][0] + dx < 0) dx+= 16384;
					while(uvi[b][0] + dx > 32768) dx-= 16384;
					while(uvi[b][1] + dy < 0) dy+= 16384;
					while(uvi[b][1] + dy > 32768) dy-= 16384;
				}

				if(mface->v4) b= 3; else b= 2;
				for(; b>=0; b--) {
					uvi[b][0]+= dx;
					CLAMP(uvi[b][0], 0, 32767);
					tface->uv[b][0]= uvi[b][0];
					
					uvi[b][1]+= dy;
					CLAMP(uvi[b][1], 0, 32767);
					tface->uv[b][1]= uvi[b][1];
				}
				
				
			}
		}
	}
	else if ELEM(mode, 128, 64) {
	
		/* minmax */
		min[0]= min[1]= 32767;
		max[0]= max[1]= 0;
		tface= me->tface;
		mface= me->mface;
		for(a=0; a<me->totface; a++, mface++, tface++) {
			if(tface->flag & SELECT) {
				if(mface->v3==0) continue;
				
				if(mface->v4) b= 3; else b= 2;
				for(; b>=0; b--) {
					min[0]= MIN2(tface->uv[b][0], min[0]);
					min[1]= MIN2(tface->uv[b][1], min[1]);
					max[0]= MAX2(tface->uv[b][0], max[0]);
					max[1]= MAX2(tface->uv[b][1], max[1]);
				}
			}
		}
		
		dx= max[0]-min[0];
		dy= max[1]-min[1];
		ycor= 32767 - (mode<<7);
		
		tface= me->tface;
		mface= me->mface;
		for(a=0; a<me->totface; a++, mface++, tface++) {
			if(tface->flag & SELECT) {
				if(mface->v3==0) continue;
				
				if(mface->v4) b= 3; else b= 2;
				for(; b>=0; b--) {
					tface->uv[b][0]=  ((tface->uv[b][0] - min[0])*(mode<<7) )/dx;
					tface->uv[b][1]=  ycor + ((tface->uv[b][1] - min[1])*(mode<<7) )/dy;
				}
			}
		}
	}
	else if ELEM3(mode, 257, 129, 65) {
		mode--;
		tface= me->tface;
		for(a=0; a<me->totface; a++, mface++, tface++) {
			if(tface->flag & SELECT) {
				default_uv(tface, mode<<7);
			}
		}
	}
	else {
		error("Sorry, not yet");
	}
	
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWIMAGE, 0);
	loadmatrix(G.vd->viewmat);
	Mat4SwapMat4(G.vd->persmat, mat);
	
}


#endif

