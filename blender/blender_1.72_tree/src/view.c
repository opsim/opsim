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


/* view.c  jan 94		GRAPHICS
 * 
 * jan 95
 * 
 * calc functies
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "render.h"

#define TRACKBALLSIZE  (1.1)

void persp_general(int a)
{
	/* for all window types, not 3D */
	
	if(a== 0) {
		#ifndef IRISGL
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		#endif

		ortho2(-0.5, ((float)(curarea->winx))-0.5, -0.5, ((float)(curarea->winy))-0.5);
		glLoadIdentity();
	}
	else if(a== 1) {
		#ifndef IRISGL
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		#else
		mmode(MPROJECTION);
		loadmatrix(curarea->winmat);
		mmode(MVIEWING);
		loadmatrix(G.vd->viewmat);
		#endif
		
	}
}

void persp(int a)
{
	/* oppasen met optimaliseren: dan laatste mode in area bewaren */
	/* only 3D windows */

	if(curarea->spacetype!=SPACE_VIEW3D) persp_general(a);
	else if(a== 0) {
		glMatrixMode(GL_PROJECTION);
		getmatrix(curarea->winmat);
		glMatrixMode(GL_MODELVIEW);
		ortho2(-0.5, (float)(curarea->winx)-.05, -0.5, (float)(curarea->winy)-0.5);
		glLoadIdentity();
	}
	else if(a== 1) {
		glMatrixMode(GL_PROJECTION);
		loadmatrix(curarea->winmat);
		glMatrixMode(GL_MODELVIEW);
		loadmatrix(G.vd->viewmat);
	}
}


float zfac=1.0;

void initgrabz(float x, float y, float z)
{
	if(G.vd==0) return;
	zfac= G.vd->persmat[0][3]*x+ G.vd->persmat[1][3]*y+ G.vd->persmat[2][3]*z+ G.vd->persmat[3][3];
}

void window_to_3d(float *vec, short mx, short my)
{
	/* altijd initzgrab aanroepen */
	float dx, dy;
	float fmx, fmy, winx, winy;
	
	/* stupid! */
	winx= curarea->winx;
	winy= curarea->winy;
	fmx= mx;
	fmy= my;
	
	dx= (2.0*fmx)/winx;
	dx*= zfac;
	dy= (2.0*fmy)/winy;
	dy*= zfac;

	vec[0]= (G.vd->persinv[0][0]*dx + G.vd->persinv[1][0]*dy);
	vec[1]= (G.vd->persinv[0][1]*dx + G.vd->persinv[1][1]*dy);
	vec[2]= (G.vd->persinv[0][2]*dx + G.vd->persinv[1][2]*dy);
}

void project_short(float *vec, short *adr)	/* clipt */
{
	float fx, fy, vec4[4];

	adr[0]= 3200;
	VECCOPY(vec4, vec);
	vec4[3]= 1.0;
	
	Mat4MulVec4fl(G.vd->persmat, vec4);

	if( vec4[3]>0.1 ) {
		fx= (curarea->winx/2)+(curarea->winx/2)*vec4[0]/vec4[3];
		
		if( fx>0 && fx<curarea->winx) {
		
			fy= (curarea->winy/2)+(curarea->winy/2)*vec4[1]/vec4[3];
			
			if(fy>0.0 && fy< (float)curarea->winy) {
				adr[0]= ffloor(fx+0.5); 
				adr[1]= ffloor(fy+0.5);
			}
		}
	}
}

void project_short_noclip(float *vec, short *adr)
{
	float fx, fy, vec4[4];

	adr[0]= 3200;
	VECCOPY(vec4, vec);
	vec4[3]= 1.0;
	
	Mat4MulVec4fl(G.vd->persmat, vec4);

	if( vec4[3]>0.1 ) {
		fx= (curarea->winx/2)+(curarea->winx/2)*vec4[0]/vec4[3];
		
		if( fx>-32700 && fx<32700) {
		
			fy= (curarea->winy/2)+(curarea->winy/2)*vec4[1]/vec4[3];
			
			if(fy>-32700.0 && fy<32700.0) {
				adr[0]= ffloor(fx+0.5); 
				adr[1]= ffloor(fy+0.5);
			}
		}
	}
}

int boundbox_clip(float obmat[][4], BoundBox *bb)
{
	/* return 1: afbeelden */
	
	float mat[4][4];
	float vec[4], min, max;
	int a, flag= -1, fl;
	
	if(bb==0) return 1;
	
	Mat4MulMat4(mat, obmat, G.vd->persmat);

	for(a=0; a<8; a++) {
		VECCOPY(vec, bb->vec[a]);
		vec[3]= 1.0;
		Mat4MulVec4fl(mat, vec);
		max= vec[3];
		min= -vec[3];

		fl= 0;
		if(vec[0] < min) fl+= 1;
		if(vec[0] > max) fl+= 2;
		if(vec[1] < min) fl+= 4;
		if(vec[1] > max) fl+= 8;
		if(vec[2] < min) fl+= 16;
		if(vec[2] > max) fl+= 32;
		
		flag &= fl;
		if(flag==0) return 1;
	}

	return 0;

}



/* trackball: deze is t.o.v. een 100% bol formule wel zo mooi */

void calctrackballvecfirst(rcti *area, short *mval, float *vec)
{
	float x, y, radius, d, z, t;
	
	radius= TRACKBALLSIZE;
	
	/* x en y normaliseren */
	x= (area->xmax + area->xmin)/2 -mval[0];
	x/= (float)((area->xmax - area->xmin)/2);
	y= (area->ymax + area->ymin)/2 -mval[1];
	y/= (float)((area->ymax - area->ymin)/2);
	
	d = fsqrt(x*x + y*y);
	if (d < radius*M_SQRT1_2)  	/* Inside sphere */
		z = fsqrt(radius*radius - d*d);
	else
	{ 			/* On hyperbola */
		t = radius / M_SQRT2;
		z = t*t / d;
	}

	vec[0]= x;
	vec[1]= y;
	vec[2]= -z;		/* jawel! */

	if( fabs(vec[2])>fabs(vec[1]) && fabs(vec[2])>fabs(vec[0]) ) {
		vec[0]= 0.0;
		vec[1]= 0.0;
		if(vec[2]>0.0) vec[2]= 1.0; else vec[2]= -1.0;
	}
	else if( fabs(vec[1])>fabs(vec[0]) && fabs(vec[1])>fabs(vec[2]) ) {
		vec[0]= 0.0;
		vec[2]= 0.0;
		if(vec[1]>0.0) vec[1]= 1.0; else vec[1]= -1.0;
	}
	else  {
		vec[1]= 0.0;
		vec[2]= 0.0;
		if(vec[0]>0.0) vec[0]= 1.0; else vec[0]= -1.0;
	}
}

void calctrackballvec(rcti *area, short *mval, float *vec)
{
	float x, y, radius, d, z, t;
	
	radius= TRACKBALLSIZE;
	
	/* x en y normaliseren */
	x= (area->xmax + area->xmin)/2 -mval[0];
	x/= (float)((area->xmax - area->xmin)/4);
	y= (area->ymax + area->ymin)/2 -mval[1];
	y/= (float)((area->ymax - area->ymin)/2);
	
	d = fsqrt(x*x + y*y);
	if (d < radius*M_SQRT1_2)  	/* Inside sphere */
		z = fsqrt(radius*radius - d*d);
	else
	{ 			/* On hyperbola */
		t = radius / M_SQRT2;
		z = t*t / d;
	}

	vec[0]= x;
	vec[1]= y;
	vec[2]= -z;		/* jawel! */

}

void viewmove(int mode)
{
	float firstvec[3], newvec[3], dvec[3];
	float oldquat[4], q1[4], q2[4], si, co, phi;
	float zoom, *fp;
	int rotmode= 1, firsttime=1;
	short val, mval[2], mvalo[2], mbut;
	
	/* winset doen omdat deze vanuit buttons wordt aangeroepen */
	areawinset(curarea->win);
	
	curarea->head_swap= WIN_FRONT_OK;

	initgrabz(-G.vd->ofs[0], -G.vd->ofs[1], -G.vd->ofs[2]);
	
	QUATCOPY(oldquat, G.vd->viewquat);
	getmouseco_sc(mvalo);		/* werk met screencoordinaten ivm trackball functie */
	calctrackballvec(&curarea->winrct, mvalo, firstvec);

	/* cumultime(0); */

	while(TRUE) {
		getmouseco_sc(mval);
		
		if(mval[0]!=mvalo[0] || mval[1]!=mvalo[1]) {
			
			if(firsttime) {
				firsttime= 0;
				/* wordt hier geroteerd, gezoomd of transleerd */
				if(mode==0) {
					if(G.vd->view!=0) addqueue(curarea->headwin, REDRAW, 1);	/* voor button */
					G.vd->view= 0;
				}
						
				if(G.vd->persp==2 || (G.vd->persp==3 && mode!=1)) {
					G.vd->persp= 1;
					curarea->windraw();
					addqueue(curarea->headwin, REDRAW, 1);
				}
			}


			if(mode==0) {	/* viewroteer */
				
				if(U.flag & TRACKBALL) {
					calctrackballvec(&curarea->winrct, mval, newvec);
					
					VecSubf(dvec, newvec, firstvec);
					
					si= fsqrt(dvec[0]*dvec[0]+ dvec[1]*dvec[1]+ dvec[2]*dvec[2]);
					si/= (2.0*TRACKBALLSIZE);
					
					if(si<1.0) {
						Crossf(q1+1, firstvec, newvec);
	
						Normalise(q1+1);
			
						phi= fasin(si);
		
						si= fsin(phi);
						q1[0]= fcos(phi);
						q1[1]*= si;
						q1[2]*= si;
						q1[3]*= si;
						
						QuatMul(G.vd->viewquat, q1, oldquat);
					}
				}
				else {
					/* roteren om z-as (x beweging) en liggende as (y) */
					
					phi= 2*(mval[0]-mvalo[0]);
					phi/= (float)curarea->winx;
					si= fsin(phi);
					q1[0]= fcos(phi);
					q1[1]= q1[2]= 0.0;
					q1[3]= si;
					
					/* liggende as */
					VECCOPY(q2+1, G.vd->viewinv[0]);
					Normalise(q2+1);
					phi= (mvalo[1]-mval[1]);
					phi/= (float)curarea->winy;
					si= fsin(phi);
					q2[0]= fcos(phi);
					q2[1]*= si;
					q2[2]*= si;
					q2[3]*= si;
					
					QuatMul(q1, q1, q2);
					QuatMul(G.vd->viewquat, G.vd->viewquat, q1);
				}
			}
			else if(mode==1) {	/* translate */
				if(G.vd->persp==3) {
					/* zoom= 0.5+0.5*(float)(2<<G.vd->rt1); */
					/* dx-= (mval[0]-mvalo[0])/zoom; */
					/* dy-= (mval[1]-mvalo[1])/zoom; */
					/* G.vd->rt2= dx; */
					/* G.vd->rt3= dy; */
					/* if(G.vd->rt2<-320) G.vd->rt2= -320; */
					/* if(G.vd->rt2> 320) G.vd->rt2=  320; */
					/* if(G.vd->rt3<-250) G.vd->rt3= -250; */
					/* if(G.vd->rt3> 250) G.vd->rt3=  250; */
				}
				else {
					window_to_3d(dvec, mval[0]-mvalo[0], mval[1]-mvalo[1]);
					VecAddf(G.vd->ofs, G.vd->ofs, dvec);
				}
			}
			else if(mode==2) {
				G.vd->dist*= 1.0+(float)(mvalo[0]-mval[0]+mvalo[1]-mval[1])/1000.0;
				
				/* deze limits ook in toets.c */
				if(G.vd->dist<0.001*G.vd->grid) G.vd->dist= 0.001*G.vd->grid;
				if(G.vd->dist>10.0*G.vd->clipend) G.vd->dist=10.0*G.vd->clipend;
				
				mval[1]= mvalo[1]; /* blijft ie zoomen */
				mval[0]= mvalo[0];
			}
			
			mvalo[0]= mval[0];
			mvalo[1]= mval[1];

			curarea->windraw();
			screen_swapbuffers();
		}
		else usleep(2);
		
		/* dit moet onderaan, anders pakt de get_mbut het niet op de PC... */
		if(!(get_mbut()&(L_MOUSE|M_MOUSE))) break;

	}
	
	curarea->head_swap= WIN_FRONT_OK;
	
}

short v3d_windowmode=0;

void setwinmatrixview3d(rctf *rect)		/* rect: voor picking */
{
	Camera *cam=0;
	float zoom, d, near, far, winx, winy;
	float lens, dfac, tfac, fac, x1, y1, x2, y2;
	short orth;
	
	lens= G.vd->lens;
	near= G.vd->clipsta;
	far= G.vd->clipend;
	
	if(G.vd->persp==2) {
		if(G.vd->camera) {
			if(G.vd->camera->type==OB_LAMP ) {
				Lamp *la;
				
				la= G.vd->camera->data;
				fac= fcos( M_PI*la->spotsize/360.0);
				
				x1= safacos(fac);
				lens= 16.0*fac/fsin(x1);
		
				near= la->clipsta;
				far= la->clipend;
			}
			else if(G.vd->camera->type==OB_CAMERA) {
				cam= G.vd->camera->data;
				lens= cam->lens;
				near= cam->clipsta;
				far= cam->clipend;
				
				if(cam->type==CAM_ORTHO) {
					lens*= 100.0;
					near= (near+1.0)*100.0;	/* otherwise zbuffer troubles. a Patch! */
					far*= 100.0;
				}
			}
		}
	}
	
	if(v3d_windowmode) {
		winx= R.rectx;
		winy= R.recty;
	}
	else {
		winx= curarea->winx;
		winy= curarea->winy;
	}
	
	if(winx>winy) d= 0.015625*winx*lens;
	else d= 0.015625*winy*lens;
	
	dfac= near/d;

	/* if(G.vd->persp==1 && G.vd->dproj>1.0) far= G.vd->dproj*far; */

	if(G.vd->persp==0) {
		/* x1= -winx*G.vd->dist/1000.0; */
		x1= -G.vd->dist;
		x2= -x1;
		y1= -winy*G.vd->dist/winx;
		y2= -y1;
		orth= 1;
	}
	else {
		if(G.vd->persp==2) {
			fac= (1.41421+( (float)G.vd->camzoom )/50.0);
			fac*= fac;
		}
		else fac= 2.0;
		
		x1= -dfac*(winx/fac);
		x2= -x1;
		y1= -dfac*(winy/fac);
		y2= -y1;
		
		if(G.vd->persp==2 && (G.special1 & G_HOLO)) {
			if(cam && (cam->flag & CAM_HOLO2)) {
				tfac= fac/4.0;	/* de fac is 1280/640 gecorr voor obszoom */

				if(cam->netend==0.0) cam->netend= EFRA;
				fac= (G.scene->r.cfra-1.0)/(cam->netend)-0.5;
				
				fac*= tfac*(x2-x1);
				fac*= ( cam->hololen1 );
				x1-= fac;
				x2-= fac;
			}
		}
		
		orth= 0;
	}

	if(rect) {		/* picking */
		rect->xmin/= winx;
		rect->xmin= x1+rect->xmin*(x2-x1);
		rect->ymin/= winy;
		rect->ymin= y1+rect->ymin*(y2-y1);
		rect->xmax/= winx;
		rect->xmax= x1+rect->xmax*(x2-x1);
		rect->ymax/= winy;
		rect->ymax= y1+rect->ymax*(y2-y1);

		if(orth) ortho(rect->xmin, rect->xmax, rect->ymin, rect->ymax, -far, far);
		else window(rect->xmin, rect->xmax, rect->ymin, rect->ymax, near, far);

	}
	else {
		if(v3d_windowmode) {
			if(orth) i_ortho(x1, x2, y1, y2, -far, far, R.winmat);
			else {
				if(cam && cam->type==CAM_ORTHO) i_window(x1, x2, y1, y2, near, far, R.winmat);
				else i_window(x1, x2, y1, y2, near, far, R.winmat);
			}
		}
		else {
			if(orth) ortho(x1, x2, y1, y2, -far, far);
			else {
				if(cam && cam->type==CAM_ORTHO) window(x1, x2, y1, y2, near, far);
				else window(x1, x2, y1, y2, near, far);
			}
		}
	}

	if(v3d_windowmode==0) {
		glMatrixMode(GL_PROJECTION);
		getmatrix(curarea->winmat);
		glMatrixMode(GL_MODELVIEW);
	}
}


void obmat_to_viewmat(Object *ob)
{
	float bmat[4][4];

	Mat4CpyMat4(bmat, ob->obmat);
	Mat4Ortho(bmat);
	Mat4Invert(G.vd->viewmat, bmat);
	
	/* viewquat berekenen, o.a. voor add object */
	Mat3CpyMat4(bmat, G.vd->viewmat);
	Mat3ToQuat(bmat, G.vd->viewquat);
}


void setviewmatrixview3d()
{
	Camera *cam;
	float tempf, bmat[4][4], cmat[4][4];
	float view[3];
	float twist, bepaalphitheta();

	if(G.special1 & G_HOLO) holoview();

	if(G.vd->persp>=2) {	    /* obs/camera */
		if(G.vd->camera) {
			
			if(G.f & G_PLAYANIM) {
				if(G.scene->r.scemode & R_NETWORK) render_frame_to_camera();
			}
			if(G.f & G_SIMULATION);
			else where_is_object(G.vd->camera);	
			obmat_to_viewmat(G.vd->camera);
			
			if(G.vd->camera->type==OB_CAMERA) {
				cam= G.vd->camera->data;
				if(cam->type==CAM_ORTHO) G.vd->viewmat[3][2]*= 100.0;
			}
		}
		else {
			QuatToMat4(G.vd->viewquat, G.vd->viewmat);
			G.vd->viewmat[3][2]-= G.vd->dist;
		}
	}
	else {
		
		QuatToMat4(G.vd->viewquat, G.vd->viewmat);
		if(G.vd->persp==1) G.vd->viewmat[3][2]-= G.vd->dist;
		i_translate(G.vd->ofs[0], G.vd->ofs[1], G.vd->ofs[2], G.vd->viewmat);
	}
}


short selectprojektie(IGLuint *buffer, short x1, short y1, short x2, short y2)
{
	rctf rect;
	Base *base;
	short mval[2], code, hits;

	G.f |= G_PICKSEL;
	
	if(x1==0 && x2==0 && y1==0 && y2==0) {
		getmouseco_areawin(mval);
		rect.xmin= mval[0]-7;
		rect.xmax= mval[0]+7;
		rect.ymin= mval[1]-7;
		rect.ymax= mval[1]+7;
	}
	else {
		rect.xmin= x1;
		rect.xmax= x2;
		rect.ymin= y1;
		rect.ymax= y2;
	}
	setwinmatrixview3d(&rect);
	Mat4MulMat4(G.vd->persmat, G.vd->viewmat, curarea->winmat);
	
	if(G.vd->drawtype > OB_WIRE) {
		G.zbuf= TRUE;
		glEnable(GL_DEPTH_TEST);
	}

	glSelectBuffer( MAXPICKBUF, buffer);
	glRenderMode(GL_SELECT);
	glInitNames();	/* deze twee fies zijn waarvoor? Anders werkt het niet */
	glPushName(-1);
	code= 1;
	
	if(G.obedit && G.obedit->type==OB_MBALL) {
		draw_object(BASACT);
	}
	else {
		base= G.scene->base.first;
		while(base) {
			if(base->lay & G.vd->lay) {
				base->selcol= code;
				glLoadName(code);
				draw_object(base);
				code++;
			}
			base= base->next;
		}
	}
	hits= glRenderMode(GL_RENDER);
	glPopName();	/* zie boven (pushname) */
	if(hits<0) error("Too many objects in selectbuf");

	G.f &= ~G_PICKSEL;
	setwinmatrixview3d(0);
	Mat4MulMat4(G.vd->persmat, G.vd->viewmat, curarea->winmat);
	
	if(G.vd->drawtype > OB_WIRE) {
		G.zbuf= 0;
		glDisable(GL_DEPTH_TEST);
	}

	return hits;
}

float *give_cursor()
{
	if(G.vd->localview) return G.vd->cursor;
	else return G.scene->cursor;
}

uint free_localbit()
{
	uint lay;
	Base *base;
	ScrArea *sa;
	bScreen *sc;
	View3D *vd;
	
	lay= 0;
	
	/* soms kunnen we een localview kwijtrijaken: als een area gesloten wordt */
	/* alle area's aflopen: welke localviews zijn in gebruik */
	sc= G.main->screen.first;
	while(sc) {
		sa= sc->areabase.first;
		while(sa) {
			vd= sa->spacedata.first;
			while(vd) {
				if(vd->spacetype==SPACE_VIEW3D) {
					lay |= vd->lay;
				}
				vd= vd->next;
			}
			sa= sa->next;
		}
		sc= sc->id.next;
	}
	
	if( (lay & 0x01000000)==0) return 0x01000000;
	if( (lay & 0x02000000)==0) return 0x02000000;
	if( (lay & 0x04000000)==0) return 0x04000000;
	if( (lay & 0x08000000)==0) return 0x08000000;
	if( (lay & 0x10000000)==0) return 0x10000000;
	if( (lay & 0x20000000)==0) return 0x20000000;
	if( (lay & 0x40000000)==0) return 0x40000000;
	if( (lay & 0x80000000)==0) return 0x80000000;
	
	return 0;
}


void initlocalview()
{
	Base *base;
	Object *ob;
	float size, min[3], max[3], afm[3];
	uint locallay;
	int a, c, ok=0;

	if(G.vd->localvd) return;

	min[0]= min[1]= min[2]= 1.0e10;
	max[0]= max[1]= max[2]= -1.0e10;

	locallay= free_localbit();

	if(locallay==0) {
		error("Sorry,  no more than 8 localviews");
		ok= 0;
	}
	else {
		if(G.obedit) {
			minmax_object(G.obedit, min, max);
			
			ok= 1;
		
			BASACT->lay |= locallay;
			G.obedit->lay= BASACT->lay;
		}
		else {
			base= FIRSTBASE;
			while(base) {
				if TESTBASE(base)  {
					minmax_object(base->object, min, max);
					base->lay |= locallay;
					base->object->lay= base->lay;
					ok= 1;
				}
				base= base->next;
			}
		}
		
		afm[0]= (max[0]-min[0]);
		afm[1]= (max[1]-min[1]);
		afm[2]= (max[2]-min[2]);
		size= MAX3(afm[0], afm[1], afm[2]);
		if(size<=0.01) size= 0.01;
	}
	
	if(ok) {
		G.vd->localvd= mallocN(sizeof(View3D), "localview");
		memcpy(G.vd->localvd, G.vd, sizeof(View3D));

		G.vd->ofs[0]= -(min[0]+max[0])/2.0;
		G.vd->ofs[1]= -(min[1]+max[1])/2.0;
		G.vd->ofs[2]= -(min[2]+max[2])/2.0;

		G.vd->dist= size;

		if(G.vd->persp>1) {
			G.vd->persp= 1;
			
		}
		G.vd->clipsta= 0.1;
		G.vd->cursor[0]= -G.vd->ofs[0];
		G.vd->cursor[1]= -G.vd->ofs[1];
		G.vd->cursor[2]= -G.vd->ofs[2];

		G.vd->lay= locallay;
		
		countall();
		addqueue(curarea->win, REDRAW, 1);
	}
	else {
		/* flags wissen */
		base= FIRSTBASE;
		while(base) {
			if( base->lay & locallay ) {
				base->lay-= locallay;
				if(base->lay==0) base->lay= G.vd->layact;
				if(base->object != G.obedit) base->flag |= SELECT;
				base->object->lay= base->lay;
			}
			base= base->next;
		}
		addqueue(curarea->headwin, REDRAW, 1);
		
		G.vd->localview= 0;
	}
}

void centreview()	/* localview zonder local! */
{
	Base *base;
	float size, min[3], max[3], afm[3];
	int a, c, ok=0;

	min[0]= min[1]= min[2]= 1.0e10;
	max[0]= max[1]= max[2]= -1.0e10;

	if(G.obedit) {
		minmax_object(G.obedit, min, max);
		
		ok= 1;
	}
	else {
		base= FIRSTBASE;
		while(base) {
			if TESTBASE(base)  {
				minmax_object(base->object, min, max);
				ok= 1;
			}
			base= base->next;
		}
	}
	
	if(ok==0) return;
	
	afm[0]= (max[0]-min[0]);
	afm[1]= (max[1]-min[1]);
	afm[2]= (max[2]-min[2]);
	size= MAX3(afm[0], afm[1], afm[2]);
	
	if(size<=0.01) size= 0.01;
	
	

	G.vd->ofs[0]= -(min[0]+max[0])/2.0;
	G.vd->ofs[1]= -(min[1]+max[1])/2.0;
	G.vd->ofs[2]= -(min[2]+max[2])/2.0;

	G.vd->dist= size;

	if(G.vd->persp>1) {
		G.vd->persp= 1;
		
	}
	G.vd->clipsta= 0.1;
	G.vd->cursor[0]= -G.vd->ofs[0];
	G.vd->cursor[1]= -G.vd->ofs[1];
	G.vd->cursor[2]= -G.vd->ofs[2];

	addqueue(curarea->win, REDRAW, 1);

}


void restore_localviewdata(View3D *vd)
{
	if(vd->localvd==0) return;
	
	VECCOPY(vd->ofs, vd->localvd->ofs);
	vd->dist= vd->localvd->dist;
	vd->persp= vd->localvd->persp;
	vd->view= vd->localvd->view;
	vd->clipsta= vd->localvd->clipsta;
	vd->clipend= vd->localvd->clipend;
	vd->lay= vd->localvd->lay;
	vd->layact= vd->localvd->layact;
	vd->drawtype= vd->localvd->drawtype;
	vd->camera= vd->localvd->camera;
	QUATCOPY(vd->viewquat, vd->localvd->viewquat);
	
}

void endlocalview(ScrArea *sa)
{
	View3D *v3d;
	struct Base *base;
	uint locallay;
	
	if(sa->spacetype!=SPACE_VIEW3D) return;
	v3d= sa->spacedata.first;
	
	if(v3d->localvd) {
		
		locallay= v3d->lay & 0xFF000000;
		
		restore_localviewdata(v3d);
		
		freeN(v3d->localvd);
		v3d->localvd= 0;
		v3d->localview= 0;

		/* als in ander window de layers zijn veranderd */
		if(v3d->scenelock) v3d->lay= G.scene->lay;
		
		base= FIRSTBASE;
		while(base) {
			if( base->lay & locallay ) {
				base->lay-= locallay;
				if(base->lay==0) base->lay= v3d->layact;
				if(base->object != G.obedit) base->flag |= SELECT;
				base->object->lay= base->lay;
			}
			base= base->next;
		}

		countall();
		allqueue(REDRAWVIEW3D, 0);	/* ivm select */
		
		sector_update(0);
	}
}

void view3d_home(int centre)
{
	Base *base;
	Object *ob;
	float size, min[3], max[3], afm[3];
	uint locallay;
	int a, c, ok= 1, onedone=0;

	if(centre) {
		min[0]= min[1]= min[2]= 0.0;
		max[0]= max[1]= max[2]= 0.0;
	}
	else {
		min[0]= min[1]= min[2]= 1.0e10;
		max[0]= max[1]= max[2]= -1.0e10;
	}
	
	base= FIRSTBASE;
	if(base==0) return;
	while(base) {
		if(base->lay & G.vd->lay) {
			onedone= 1;
			minmax_object(base->object, min, max);
		}
		base= base->next;
	}
	if(!onedone) return;
	
	afm[0]= (max[0]-min[0]);
	afm[1]= (max[1]-min[1]);
	afm[2]= (max[2]-min[2]);
	size= MAX3(afm[0], afm[1], afm[2]);
	if(size==0.0) ok= 0;
		
	if(ok) {

		G.vd->ofs[0]= -(min[0]+max[0])/2.0;
		G.vd->ofs[1]= -(min[1]+max[1])/2.0;
		G.vd->ofs[2]= -(min[2]+max[2])/2.0;

		G.vd->dist= size;
		
		if(G.vd->persp==2) G.vd->persp= 1;
		
		addqueue(curarea->win, REDRAW, 1);
	}
}



