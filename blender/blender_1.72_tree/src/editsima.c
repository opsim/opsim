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


/* editsima.c 		GRAPHICS
 * 
 * juli 96
 * 
 */

#include "blender.h"
#include "graphics.h"

#include "edit.h"
#include "sector.h"

typedef struct TransShort {
	short *loc;
	short oldloc[3], flag;
} TransShort;


int is_uv_tface_editing_allowed()
{
	Mesh *me;

	if(G.obedit) {error("Unable to perform function in EditMode"); return 0;}
	if(G.sima->mode!=SI_TEXTURE) return 0;
	if(G.sima->image==0 || G.sima->image->ibuf==0) return 0;
	me= get_mesh(OBACT);
	if(me==0 || me->tface==0) return 0;
	
	return 1;
}

void sima_pixelgrid(short *loc, int sx, int sy)
{
	int y= G.sima->image->ibuf->y;
	int x= G.sima->image->ibuf->x, z;
	
	z= 32768/x;
	sx= z*((sx+1)/z);
	CLAMP(sx, 0, 32767);
	loc[0]= sx;
	
	z= 32768/y;
	sy= z*( (sy+1)/z);
	CLAMP(sy, 0, 32767);
	loc[1]= sy;
}

void be_square_tface_uv(Mesh *me)
{
	TFace *tface;
	MFace *mface;
	int a;
	
	/* als 1 punt select: doit (met het select punt) */
	for(a=me->totface, mface= me->mface, tface= me->tface; a>0; a--, tface++, mface++) {
		if(mface->v4) {
			if(tface->flag & SELECT) {
				
				if(tface->flag & TF_SEL1) {
			
					if( tface->uv[1][0] == tface->uv[2][0] ) {
						tface->uv[1][1]= tface->uv[0][1];
						tface->uv[3][0]= tface->uv[0][0];
					}
					else {	
						tface->uv[1][0]= tface->uv[0][0];
						tface->uv[3][1]= tface->uv[0][1];
					}

				}
				if(tface->flag & TF_SEL2) {
					if( tface->uv[2][1] == tface->uv[3][1] ) {
						tface->uv[2][0]= tface->uv[1][0];
						tface->uv[0][1]= tface->uv[1][1];
					}
					else {
						tface->uv[2][1]= tface->uv[1][1];
						tface->uv[0][0]= tface->uv[1][0];
					}

				}
				if(tface->flag & TF_SEL3) {
					if( tface->uv[3][0] == tface->uv[0][0] ) {
						tface->uv[3][1]= tface->uv[2][1];
						tface->uv[1][0]= tface->uv[2][0];
					}
					else {
						tface->uv[3][0]= tface->uv[2][0];
						tface->uv[1][1]= tface->uv[2][1];
					}
				}
				if(tface->flag & TF_SEL4) {
					if( tface->uv[0][1] == tface->uv[1][1] ) {
						tface->uv[0][0]= tface->uv[3][0];
						tface->uv[2][1]= tface->uv[3][1];
					}
					else {
						tface->uv[0][1]= tface->uv[3][1];
						tface->uv[2][0]= tface->uv[3][0];
					}

				}
			}
		}
	}

}

void transform_tface_uv(int mode)
{
	TFace *tface;
	Mesh *me;
	TransShort *transmain, *tv;
	float asp, dx1, dx2, dy1, dy2, phi, dphi, co, si;
	float xref=1.0, yref=1.0, size[2], sizefac;
	float dx, dy, dvec2[2], dvec[2], div, cent[2];
	int min[3], max[3], vec[2], xtra[2];
	int x, y, tot=0, a, b, firsttime=1, afbreek=0, midtog= 0, proj, ivec[2];
	ushort event;
	short mval[2], val, xo, yo, xn, yn, xc, yc;
	char str[32];
	
	if( is_uv_tface_editing_allowed()==0 ) return;
	me= get_mesh(OBACT);
	
	min[0]= min[1]= 32768;
	max[0]= max[1]= -32768;
	
	calc_image_view(G.sima, 's');
	
		
	/* welke vertices doen mee */
	
	for(a=me->totface, tface= me->tface; a>0; a--, tface++) {
		if(tface->flag & SELECT) {
			if(tface->flag & TF_SEL1) tot++;
			if(tface->flag & TF_SEL2) tot++;
			if(tface->flag & TF_SEL3) tot++;
			if(tface->flag & TF_SEL4) tot++;
		}
	}
	if(tot==0) return;
	
	tv=transmain= callocN(tot*sizeof(TransShort), "transmain");

	for(a=me->totface, tface= me->tface; a>0; a--, tface++) {
		if(tface->flag & SELECT) {
			if(tface->flag & TF_SEL1) {
				tv->loc= tface->uv[0];
				tv++;
			}
			if(tface->flag & TF_SEL2) {
				tv->loc= tface->uv[1];
				tv++;
			}
			if(tface->flag & TF_SEL3) {
				tv->loc= tface->uv[2];
				tv++;
			}
			if(tface->flag & TF_SEL4) {
				tv->loc= tface->uv[3];
				tv++;
			}
		}
	}
	
	a= tot;
	tv= transmain;
	while(a--) {
		tv->oldloc[0]= tv->loc[0];
		tv->oldloc[1]= tv->loc[1];
		DO_MINMAX2(tv->loc, min, max);
		tv++;
	}

	cent[0]= (min[0]+max[0])/2.0;
	cent[1]= (min[1]+max[1])/2.0;

	ipoco_to_areaco_noclip(cent, mval);
	xc= mval[0];
	yc= mval[1];
	
	getmouseco_areawin(mval);
	xo= xn= mval[0];
	yo= yn= mval[1];
	dvec[0]= dvec[1]= 0.0;
	dx1= xc-xn; 
	dy1= yc-yn;
	phi= 0.0;
	
	
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
				
				dvec2[0]= dvec[0];
				dvec2[1]= dvec[1];
				apply_keyb_grid(dvec2, 0.0, 32768.0/8.0, 32768.0/16.0, U.flag & AUTOGRABGRID);
				apply_keyb_grid(dvec2+1, 0.0, 32768.0/8.0, 32768.0/16.0, U.flag & AUTOGRABGRID);

				vec[0]= dvec2[0];	/* vec is int */
				vec[1]= dvec2[1];
				
				if(vec[0]< -min[0]) vec[0]= -min[0];
				if(vec[1]< -min[1]) vec[1]= -min[1];
				if(vec[0]> 32767-max[0]) vec[0]= 32767-max[0];
				if(vec[1]> 32767-max[1]) vec[1]= 32767-max[1];
				
				tv= transmain;
				for(a=0; a<tot; a++, tv++) {
					
					x= tv->oldloc[0]+vec[0];
					y= tv->oldloc[1]+vec[1];
						
					sima_pixelgrid(tv->loc, x, y);
				}
				ivec[0]= (vec[0]*G.sima->image->ibuf->x)/32767;
				ivec[1]= (vec[1]*G.sima->image->ibuf->y)/32767;

				if(G.sima->flag & SI_BE_SQUARE) be_square_tface_uv(me);
			
				sprintf(str, "X: %d   Y: %d  ", ivec[0], ivec[1]);
				headerprint(str);
			}
			else if(mode=='r') {

				dx2= xc-mval[0];
				dy2= yc-mval[1];
				
				div= fsqrt( (dx1*dx1+dy1*dy1)*(dx2*dx2+dy2*dy2));
				if(div>1.0) {
				
					dphi= (dx1*dx2+dy1*dy2)/div;
					dphi= safacos(dphi);
					if( (dx1*dy2-dx2*dy1)<0.0 ) dphi= -dphi;
					
					if(G.qual & LR_SHIFTKEY) phi+= dphi/30.0;
					else phi+= dphi;
					
					apply_keyb_grid(&phi, 0.0, (5.0/180)*M_PI, (1.0/180)*M_PI, U.flag & AUTOROTGRID);
					
					dx1= dx2; 
					dy1= dy2;
					
					co= fcos(phi);
					si= fsin(phi);
					asp= (float)G.sima->image->ibuf->y/(float)G.sima->image->ibuf->x;

					
					tv= transmain;
					for(a=0; a<tot; a++, tv++) {
						
						x= ( co*( tv->oldloc[0]-cent[0]) - si*asp*(tv->oldloc[1]-cent[1]) ) +cent[0];
						y= ( si*( tv->oldloc[0]-cent[0])/asp + co*(tv->oldloc[1]-cent[1]) ) +cent[1];
						sima_pixelgrid(tv->loc, x, y);
					}					
					
					
					sprintf(str, "Rot: %.3f  ", phi*180.0/M_PI);
					headerprint(str);
				}
			}
			else if(mode=='s') {
				
				if(min[0]==max[0] || min[1]==max[1]) break;
				
				size[0]=size[1]= (fsqrt( (float)((yc-mval[1])*(yc-mval[1])+(mval[0]-xc)*(mval[0]-xc)) ))/sizefac;
				
				if(midtog) size[proj]= 1.0;
				
				apply_keyb_grid(size, 0.0, 0.1, 0.01, U.flag & AUTOSIZEGRID);
				apply_keyb_grid(size+1, 0.0, 0.1, 0.01, U.flag & AUTOSIZEGRID);

				size[0]*= xref;
				size[1]*= yref;

				/* vier stappen plan: XTRA X */
				xtra[0]= xtra[1]= 0;

				a=b= 0;
				if(size[0]*(min[0]-cent[0]) + cent[0] + xtra[0] < 0) 
					a= -size[0]*(min[0]-cent[0]) - cent[0];
				if(size[0]*(max[0]-cent[0]) + cent[0] + xtra[0] > 32767) 
					b= 32767 - size[0]*(max[0]-cent[0]) - cent[0];
				xtra[0]= (a+b)/2;
				
				/* SIZE X */
				if(size[0]*(min[0]-cent[0]) + cent[0] + xtra[0] < 0) 
					size[0]= (-cent[0]-xtra[0])/(min[0]-cent[0]);
				if(size[0]*(max[0]-cent[0]) + cent[0] +xtra[0] > 32767) 
					size[0]= (32767-cent[0]-xtra[0])/(max[0]-cent[0]);
					
				/* XTRA Y */
				a=b= 0;
				if(size[1]*(min[1]-cent[1]) + cent[1] + xtra[1] < 0) 
					a= -size[1]*(min[1]-cent[1]) - cent[1];
				if(size[1]*(max[1]-cent[1]) + cent[1] + xtra[1] > 32767) 
					b= 32767 - size[1]*(max[1]-cent[1]) - cent[1];
				xtra[1]= (a+b)/2;
				
				/* SIZE Y */
				if(size[1]*(min[1]-cent[1]) + cent[1] +xtra[1] < 0) 
					size[1]= (-cent[1]-xtra[1])/(min[1]-cent[1]);
				if(size[1]*(max[1]-cent[1]) + cent[1] + xtra[1]> 32767) 
					size[1]= (32767-cent[1]-xtra[1])/(max[1]-cent[1]);
				

				/* if(midtog==0) { */
				/* 	if(size[1]>size[0]) size[1]= size[0]; */
				/* 	else if(size[0]>size[1]) size[0]= size[1]; */
				/* } */

				tv= transmain;
				for(a=0; a<tot; a++, tv++) {
					
					x= size[0]*(tv->oldloc[0]-cent[0])+ cent[0] + xtra[0];
					y= size[1]*(tv->oldloc[1]-cent[1])+ cent[1] + xtra[1];
					sima_pixelgrid(tv->loc, x, y);
				}
				
				sprintf(str, "sizeX: %.3f   sizeY: %.3f  ", size[0], size[1]);
				headerprint(str);
				
			}
			
			xo= mval[0];
			yo= mval[1];
			
			force_draw();
			
			firsttime= 0;
			
		}
		else usleep(1);
		
		while(qtest()) {
			event= extern_qread(&val);
			if(val) {
				switch(event) {
				case ESCKEY:
				case LEFTMOUSE:
				case SPACEKEY:
				case RETKEY:
					afbreek= 1;
					break;
				case MIDDLEMOUSE:
					
					midtog= ~midtog;
					if(midtog) {
						if( abs(mval[0]-xn) > abs(mval[1]-yn)) proj= 1;
						else proj= 0;
						firsttime= 1;
					}
				
					break;
				case XKEY:
				case YKEY:
					if(event==XKEY) xref= -xref;
					else yref= -yref;
					
					firsttime= 1;
					break;
				default:
					arrowsmovecursor(event);
				}
			}
			if(afbreek) break;
		}
	}
	
	if(event==ESCKEY) {
		tv= transmain;
		for(a=0; a<tot; a++, tv++) {
			tv->loc[0]= tv->oldloc[0];
			tv->loc[1]= tv->oldloc[1];
		}
	}
	freeN(transmain);
	
	if(mode=='g') if(G.sima->flag & SI_BE_SQUARE) be_square_tface_uv(me);
			
	allqueue(REDRAWVIEW3D, 0);
	addqueue(curarea->headwin, REDRAW, 1);
	addqueue(curarea->win, REDRAW, 1);
}

void select_swap_tface_uv()
{
	Mesh *me;
	TFace *tface;
	MFace *mface;
	int a, sel=0;
	
	if( is_uv_tface_editing_allowed()==0 ) return;
	me= get_mesh(OBACT);

	for(a=me->totface, tface= me->tface; a>0; a--, tface++) {
		if(tface->flag & SELECT) {	
			if(tface->flag & (TF_SEL1+TF_SEL2+TF_SEL3+TF_SEL4)) {
				sel= 1;
				break;
			}
		}
	}
	
	mface= me->mface;
	for(a=me->totface, tface= me->tface; a>0; a--, tface++, mface++) {
		if(tface->flag & SELECT) {
			if(mface->v4) {
				if(sel) tface->flag &= ~(TF_SEL1+TF_SEL2+TF_SEL3+TF_SEL4);
				else tface->flag |= (TF_SEL1+TF_SEL2+TF_SEL3+TF_SEL4);
			}
			else if(mface->v3) {
				if(sel) tface->flag &= ~(TF_SEL1+TF_SEL2+TF_SEL3+TF_SEL4);
				else tface->flag |= (TF_SEL1+TF_SEL2+TF_SEL3);
			}
		}
	}
	
	allqueue(REDRAWIMAGE, 0);
}

void mouse_select_sima()
{
	Mesh *me;
	TFace *tface;
	MFace *mface;
	int a, temp, dist=100;
	short xo, yo, mval[2], uval[2], val;
	char *flagpoin =0;
	
	if( is_uv_tface_editing_allowed()==0 ) return;
	me= get_mesh(OBACT);
	
	getmouseco_areawin(mval);	

	mface= me->mface;
	for(a=me->totface, tface= me->tface; a>0; a--, tface++, mface++) {
		
		if(tface->flag & SELECT) {
		
			uvco_to_areaco_noclip(tface->uv[0], uval);
			temp= abs(mval[0]- uval[0])+ abs(mval[1]- uval[1]);
			if( tface->flag & TF_SEL1) temp+=5;
			if(temp<dist) { 
				flagpoin= &tface->flag;
				dist= temp; 
				val= TF_SEL1;
			}
	
			uvco_to_areaco_noclip(tface->uv[1], uval);
			temp= abs(mval[0]- uval[0])+ abs(mval[1]- uval[1]);
			if( tface->flag & TF_SEL2) temp+=5;
			if(temp<dist) { 
				flagpoin= &tface->flag;
				dist= temp; 
				val= TF_SEL2;
			}
	
			uvco_to_areaco_noclip(tface->uv[2], uval);
			temp= abs(mval[0]- uval[0])+ abs(mval[1]- uval[1]);
			if( tface->flag & TF_SEL3) temp+=5;
			if(temp<dist) { 
				flagpoin= &tface->flag;
				dist= temp; 
				val= TF_SEL3;
			}
	
			if(mface->v4) {
				uvco_to_areaco_noclip(tface->uv[3], uval);
				temp= abs(mval[0]- uval[0])+ abs(mval[1]- uval[1]);
				if( tface->flag & TF_SEL4) temp+=5;
				if(temp<dist) { 
					flagpoin= &tface->flag;
					dist= temp; 
					val= TF_SEL4;
				}
			}
			
		}
	}
	
	if(flagpoin) {
		PRINT(d, val);
		if(G.qual & LR_SHIFTKEY) {
			if(*flagpoin & val) *flagpoin &= ~val;
			else *flagpoin |= val;
		}
		else {
			for(a=me->totface, tface= me->tface; a>0; a--, tface++) {
				if(tface->flag & SELECT) {
					tface->flag &= ~(TF_SEL1+TF_SEL2+TF_SEL3+TF_SEL4);
				}
			}
			
			*flagpoin |= val;
		}
		
	
		glDrawBuffer(GL_FRONT);
		draw_tfaces();
		glDrawBuffer(GL_BACK);
		
		std_rmouse_transform(transform_tface_uv);
	}
}

void borderselect_sima()
{
	Mesh *me;
	TFace *tface;
	MFace *mface;
	rcti rect;
	rctf rectf;
	int a, b, val, ok;
	short mval[2];

	if( is_uv_tface_editing_allowed()==0 ) return;
	me= get_mesh(OBACT);

	val= get_border(&rect, 3);

	if(val) {
		mval[0]= rect.xmin;
		mval[1]= rect.ymin;
		areamouseco_to_ipoco(mval, &rectf.xmin, &rectf.ymin);
		mval[0]= rect.xmax;
		mval[1]= rect.ymax;
		areamouseco_to_ipoco(mval, &rectf.xmax, &rectf.ymax);

		mface= me->mface;
		for(a=me->totface, tface= me->tface; a>0; a--, tface++, mface++) {
		
			if(tface->flag & SELECT) {
				
				if(in_rctf(&rectf, (float)tface->uv[0][0], (float)tface->uv[0][1])) {
					if(val==LEFTMOUSE) tface->flag |= TF_SEL1;
					else tface->flag &= ~TF_SEL1;
				}
				if(in_rctf(&rectf, (float)tface->uv[1][0], (float)tface->uv[1][1])) {
					if(val==LEFTMOUSE) tface->flag |= TF_SEL2;
					else tface->flag &= ~TF_SEL2;
				}
				if(in_rctf(&rectf, (float)tface->uv[2][0], (float)tface->uv[2][1])) {
					if(val==LEFTMOUSE) tface->flag |= TF_SEL3;
					else tface->flag &= ~TF_SEL3;
				}
				if(mface->v4 && in_rctf(&rectf, (float)tface->uv[3][0], (float)tface->uv[3][1])) {
					if(val==LEFTMOUSE) tface->flag |= TF_SEL4;
					else tface->flag &= ~TF_SEL4;
				}
			}
							
		}
		addqueue(curarea->win, REDRAW, 1);
	}
}

void mouseco_to_curtile()
{
	float fx, fy;
	short mval[2], dx, dy;
	
	if( is_uv_tface_editing_allowed()==0) return;

	if(G.sima->image->tpageflag & IMA_TILES) {
		
		G.sima->flag |= SI_EDITTILE;
		
		while(get_mbut()&L_MOUSE) {
			
			calc_image_view(G.sima, 's');
			
			getmouseco_areawin(mval);
			areamouseco_to_ipoco(mval, &fx, &fy);

			if(fx>=0.0 && fy>=0.0 && fx<32767.0 && fy<32767.0) {
			
				fx= (fx/32767.0)*G.sima->image->xrep;
				fy= (fy/32767.0)*G.sima->image->yrep;
				
				mval[0]= fx;
				mval[1]= fy;
				
				G.sima->curtile= mval[1]*G.sima->image->xrep + mval[0];
			}

			curarea->windraw();
			screen_swapbuffers();
		
		}
		
		G.sima->flag &= ~SI_EDITTILE;

		image_changed(G.sima, 1);

		allqueue(REDRAWVIEW3D, 0);
		addqueue(curarea->win, REDRAW, 1);
	}
}

