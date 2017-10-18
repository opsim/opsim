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

/* previewrender.c  		GRAPHICS

 * 
 * maart 95
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "render.h"

#define PR_RECTX	100
#define PR_RECTY	100
#define PR_XMIN		10
#define PR_YMIN		10
#define PR_XMAX		190
#define PR_YMAX		190

#define PR_FACY		(PR_YMAX-PR_YMIN-4)/(PR_RECTY)

#define SCANL	1280

uint pr_scan[SCANL];

float pr_lamp[3]= {-.577349, -.577349, 0.577349};

rcti prerect;
int pr_sizex, pr_sizey;
float pr_facx, pr_facy;

uint previewback(int type, int x, int y)
{
	if(type & 2) {
		if(abs(x)>abs(y)) return 0;
		else return 0x40404040;
	}
	else {
		if(abs(x)>abs(y)) return 0x40404040;
		else return 0xa0a0a0a0;
	}
}

void set_previewrect(int xmin, int ymin, int xmax, int ymax)
{
	/* coordinaten omzetten naar absolute windowco's */
	float x, y;
	
	x= xmin; y= ymin;
	view2d_to_window(&x, &y);

	prerect.xmin= ffloor(x);;
	prerect.ymin= ffloor(y);
	
	x= xmax; y= ymax;
	view2d_to_window(&x, &y);

	prerect.xmax= fceil(x);
	prerect.ymax= fceil(y);
	
	pr_sizex= (prerect.xmax-prerect.xmin);
	pr_sizey= (prerect.ymax-prerect.ymin);

	pr_facx= ( (float)pr_sizex-1)/PR_RECTX;
	pr_facy= ( (float)pr_sizey-1)/PR_RECTY;
}

void EmbossBox4(x1, y1, x2, y2, sel, dark, light)
short x1, y1, x2, y2, sel;
uint dark, light;
{

	if(sel) cpack(dark); 
	else cpack(light);	
	
	x1--;
	y1--;
	x2++;
	y2++;
	
	glRects(x1,  y2,  x2,  y2-1);	/* boven */
	glRects(x1,  y1,  x1+1,  y2);	/* links */
	
	if(sel) cpack(light); 
	else cpack(dark);	
	
	glRects(x1,  y1,  x2,  y1+1);	/* onder */
	glRects(x2,  y1,  x2-1,  y2);	/* rechts */
	
}


#ifdef IRISGL

void display_pr_scanline(uint *rect, int starty)
{
	uint *scan;
	int x, y, dx, dy, endy;
	int tx=0, ty;
	short *sp;
	
	rect+= starty*PR_RECTX;
	
	endy= prerect.ymin+ ffloor(pr_facy*(starty+1)) -1;
	starty= prerect.ymin+ ffloor(pr_facy*starty);
	
	/* scanline uitvergroten */
	dx= (int) (65536.0/pr_facx);
	scan= pr_scan;
	sp= (short *)&tx;
	x= pr_sizex;
	while( x-- ) {
		*scan= *rect;
		scan++;
		tx+= dx;
		while( *sp>0 ) {
			tx-= 65536;
			rect++;
		}
	}

	/* scanline neerzetten */
	for(y=starty; y<= endy; y++) {
		lrectwriteRGBA(prerect.xmin, y, prerect.xmax-1, y, pr_scan);
	}
}

void previewdraw()
{
	int y;
	
	init_view2d_calc();
	
	EmbossBox4(PR_XMIN-2, PR_YMIN-2, PR_XMAX+2, PR_YMAX+2, 1, 0x202020, 0x808080);
	set_previewrect(PR_XMIN+2, PR_YMIN+2, PR_XMAX-2, PR_YMAX-2);

	if(G.buts->rect==0) {
		G.buts->rect= callocN(4*PR_RECTX*PR_RECTY, "butsrect");
		preview_changed(curarea->win);
		return;
	}
	if(G.buts->cury==0) {
		preview_changed(curarea->win);
		return;
	}

	for(y=0; y<PR_RECTY; y++) {
		display_pr_scanline(G.buts->rect, y);
	}
	if(G.buts->mainb==BUTS_TEX) {
		draw_tex_crop(G.buts->lockpoin);
	}
}


#else

void display_pr_scanline(uint *rect, int recty)
{
	uint *scan;
	int x, y, dx, dy, endy;
	int tx=0, ty, starty;
	short *sp;

	rect+= recty*PR_RECTX;
	
	/* iets meer uitvergroten in y om GL/mesa bugje te verhelpen */
	glPixelZoom(pr_facx, 1.5+pr_facy);

	glRasterPos2f( (float)PR_XMIN+0.5, 1.0+(float)PR_YMIN + (recty*PR_FACY) );
	glDrawPixels(PR_RECTX, 1, GL_RGBA, GL_UNSIGNED_BYTE,  rect);

	glPixelZoom(1.0, 1.0);
}

void previewdrawNN()
{
	int y;
	
	init_view2d_calc();
	
	set_previewrect(PR_XMIN, PR_YMIN, PR_XMAX, PR_YMAX);

	if(G.buts->rect==0) {
		G.buts->rect= callocN(sizeof(int)*PR_RECTX*PR_RECTY, "butsrect");
		preview_changed(curarea->win);
		EmbossBox4(PR_XMIN, PR_YMIN, PR_XMAX, PR_YMAX, 1, 0x202020, 0x808080);
		return;
	}
	if(G.buts->cury==0) {
		preview_changed(curarea->win);
		EmbossBox4(PR_XMIN, PR_YMIN, PR_XMAX, PR_YMAX, 1, 0x202020, 0x808080);
		return;
	}

	EmbossBox4(PR_XMIN, PR_YMIN, PR_XMAX, PR_YMAX, 1, 0x202020, 0x808080);
	
	for(y=0; y<PR_RECTY; y++) display_pr_scanline(G.buts->rect, y);

	if(G.buts->mainb==BUTS_TEX) {
		draw_tex_crop(G.buts->lockpoin);
	}
}

void previewdraw()
{
	int y;
	
	init_view2d_calc();
	
	set_previewrect(PR_XMIN, PR_YMIN, PR_XMAX, PR_YMAX);

	if(G.buts->rect==0) {
		G.buts->rect= callocN(sizeof(int)*PR_RECTX*PR_RECTY, "butsrect");
		preview_changed(curarea->win);
		EmbossBox4(PR_XMIN, PR_YMIN, PR_XMAX, PR_YMAX, 1, 0x202020, 0x808080);
		return;
	}
	if(G.buts->cury==0) {
		preview_changed(curarea->win);
		EmbossBox4(PR_XMIN, PR_YMIN, PR_XMAX, PR_YMAX, 1, 0x202020, 0x808080);
		return;
	}

	EmbossBox4(PR_XMIN, PR_YMIN, PR_XMAX, PR_YMAX, 1, 0x202020, 0x808080);
	
	rectwrite_part(curarea->winrct.xmin, curarea->winrct.ymin, curarea->winrct.xmax, curarea->winrct.ymax,
		prerect.xmin, prerect.ymin, PR_RECTX, PR_RECTY, pr_facx, pr_facx, G.buts->rect);

	if(G.buts->mainb==BUTS_TEX) {
		draw_tex_crop(G.buts->lockpoin);
	}
}


#endif

void preview_changed(short win)
{
	G.buts->cury= 0;
	addafterqueue(win, RENDERPREVIEW, 1);
}

void draw_tex_crop(Tex *tex)
{
	rcti rct;
	int ret= 0;
	
	if(tex==0) return;
	
	if(tex->type==TEX_IMAGE) {
		if(tex->cropxmin==0.0) ret++;
		if(tex->cropymin==0.0) ret++;
		if(tex->cropxmax==1.0) ret++;
		if(tex->cropymax==1.0) ret++;
		if(ret==4) return;
		
		rct.xmin= PR_XMIN+2+tex->cropxmin*(PR_XMAX-PR_XMIN-4);
		rct.xmax= PR_XMIN+2+tex->cropxmax*(PR_XMAX-PR_XMIN-4);
		rct.ymin= PR_YMIN+2+tex->cropymin*(PR_YMAX-PR_YMIN-4);
		rct.ymax= PR_YMIN+2+tex->cropymax*(PR_YMAX-PR_YMIN-4);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

		cpack(0x0);
		glRecti(rct.xmin+1,  rct.ymin-1,  rct.xmax+1,  rct.ymax-1); 

		cpack(0xFFFFFF);
		glRecti(rct.xmin,  rct.ymin,  rct.xmax,  rct.ymax);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);			
	}
	
}


void sky_preview_pixel(float lens, int x, int y, char *rect)
{
	
	if(R.wrld.skytype & WO_SKYPAPER) {
		R.view[0]= (2*x)/(float)PR_RECTX;
		R.view[1]= (2*y)/(float)PR_RECTY;
		R.view[2]= 0.0;
	}
	else {
		R.view[0]= x;
		R.view[1]= y;
		R.view[2]= -lens*PR_RECTX/32.0;
		Normalise(R.view);
	}
	sky(rect);
}

void lamp_preview_pixel(LampRen *la, int x, int y, char *rect)
{
	float inpr, i, t, dist, distkw, vec[3];
	int col;
	
	R.co[0]= (float)x/(PR_RECTX/4);
	R.co[1]= (float)y/(PR_RECTX/4);
	R.co[2]= 0;
	
	vec[0]= 0.02*x;
	vec[1]= 0.02*y;
	vec[2]= 0.005*PR_RECTX;
	VECCOPY(R.view, vec);
	dist= Normalise(R.view);

	if(la->mode & LA_TEXTURE) do_lamp_tex(la, vec);

	if(la->type==LA_SUN || la->type==LA_HEMI) {
		dist= 1.0;
	}
	else {
		
		if(la->mode & LA_QUAD) {
			
			t= 1.0;
			if(la->ld1>0.0)
				t= la->dist/(la->dist+la->ld1*dist);
			if(la->ld2>0.0) {
				distkw= la->dist*la->dist;
				t= t*distkw/(t*distkw+la->ld2*dist*dist);
			}
			dist= t;
		}
		else {
			dist= (la->dist/(la->dist+dist));
		}
	}

	if(la->type==LA_SPOT) {

		
		if(la->mode & LA_SQUARE) {
			/* slightly smaller... */
			inpr= 1.7*fcos(MAX2(fabs(R.view[0]/R.view[2]) , fabs(R.view[1]/R.view[2]) ));
		}
		else {
			inpr= R.view[2];
		}
		
		t= la->spotsi;
		if(inpr<t) dist= 0.0;
		else {
			t= inpr-t;
			if(t<la->spotbl && la->spotbl!=0.0) {
				/* zachte gebied */
				i= t/la->spotbl;
				t= i*i;
				i= t*i;
				inpr*=(3.0*t-2.0*i);
			}
		}
		dist*=inpr;
	}
	else if(la->type==LA_LOCAL) dist*= R.view[2];
	
	col= 255.0*dist*la->r;
	if(col<=0) rect[0]= 0; else if(col>=255) rect[0]= 255; else rect[0]= col;

	col= 255.0*dist*la->g;
	if(col<=0) rect[1]= 0; else if(col>=255) rect[1]= 255; else rect[1]= col;

	col= 255.0*dist*la->b;
	if(col<=0) rect[2]= 0; else if(col>=255) rect[2]= 255; else rect[2]= col;
}

void init_previewhalo(HaloRen *har, Material *mat)
{
	
	har->type= 0;
	if(mat->mode & MA_HALO_XALPHA) har->type |= HA_XALPHA;
	har->mat= mat;
	har->hard= mat->har;
	har->rad= PR_RECTX/2.0;
	har->radsq= PR_RECTX*PR_RECTX/4.0;
	har->alfa= mat->alpha;
	har->add= 255.0*mat->add;
	har->r= 255.0*mat->r;
	har->g= 255.0*mat->g; 
	har->b= 255.0*mat->b;
	har->xs= PR_RECTX/2.0;
	har->ys= PR_RECTX/2.0;
	har->zs= har->zd= 0;
	har->seed= (mat->seed1 % 256);
	
	if( (mat->mode & MA_HALOTEX) && mat->mtex[0] ) har->tex= 1; else har->tex=0;

	if(mat->mode & MA_STAR) har->starpoints= mat->starc; else har->starpoints= 0;
	if(mat->mode & MA_HALO_LINES) har->linec= mat->linec; else har->linec= 0;
	if(mat->mode & MA_HALO_RINGS) har->ringc= mat->ringc; else har->ringc= 0;
	if(mat->mode & MA_HALO_FLARE) har->flarec= mat->flarec; else har->flarec= 0;
	
	if(har->flarec) {
		har->xs-= PR_RECTX/3;
		har->ys+= PR_RECTX/3;
		
		har->rad*= 0.3;
		har->radsq= har->rad*har->rad;
		
		har->pixels= har->rad*har->rad*har->rad;
	}
}	

void halo_preview_pixel(HaloRen *har, int startx, int endx, int y, char *rect)
{
	float dist, xn, yn, xsq, ysq;
	int x, flofs;
	char front[4];
	
	if(har->flarec) yn= y-PR_RECTX/3;
	else yn= y;
	ysq= yn*yn;
	
	for(x=startx; x<endx; x++) {
		
		if(har->flarec) xn= x+PR_RECTX/3;
		else xn= x;
		
		xsq= xn*xn;
		dist= xsq+ysq;

		
		
		if(dist<har->radsq) {
			shadehalo(har, front, 0, dist, xn, yn, har->flarec);
			addalphaAddfac(rect, front, har->add);
		}
		rect+= 4;
	}
}

void previewflare(HaloRen *har, uint *rect)
{
	float ycor;
	uint *rectot;
	int afmx, afmy, rectx, recty;
	
	/* temps */
	ycor= R.ycor;
	rectx= R.rectx;
	recty= R.recty;
	afmx= R.afmx;
	afmy= R.afmy;
	rectot= R.rectot;

	R.ycor= 1.0;
	R.rectx= PR_RECTX;	
	R.recty= PR_RECTY;
	R.afmx= PR_RECTX/2;
	R.afmy= PR_RECTY/2;
	R.rectot= rect;

	waitcursor(1);

	renderflare(har);

	previewdraw();
	
	waitcursor(0);
	
	/* temps */
	R.ycor= ycor;
	R.rectx= rectx;
	R.recty= recty;
	R.afmx= afmx;
	R.afmy= afmy;
	R.rectot= rectot;
}

void texture_preview_pixel(Tex *tex, int x, int y, char *rect)
{
	extern float Tin, Tr, Tg, Tb, Ta;
	float i, v1, xsq, ysq, texvec[3];
	int rgbnor, tracol, skip=0;
		
	if(tex->type==TEX_IMAGE) {
		v1= 1.0/PR_RECTX;
		
		texvec[0]= 0.5+v1*x;
		texvec[1]= 0.5+v1*y;
		
		/* geen coordmapping, uitzondering: repeat */
		if(tex->xrepeat>1) {
			texvec[0] *= tex->xrepeat;
			if(texvec[0]>1.0) texvec[0] -= (int)(texvec[0]);
		}
		if(tex->yrepeat>1) {
			texvec[1] *= tex->yrepeat;
			if(texvec[1]>1.0) texvec[1] -= (int)(texvec[1]);
		}

	}
	else if(tex->type==TEX_ENVMAP) {
		if(tex->env) {
			ysq= y*y;
			xsq= x*x;
			if(xsq+ysq < (PR_RECTX/2)*(PR_RECTY/2)) {
				texvec[2]= fsqrt( (float)((PR_RECTX/2)*(PR_RECTY/2)-xsq-ysq) );
				texvec[0]= -x;
				texvec[1]= -y;
				Normalise(texvec);

				i= 2.0*(texvec[2]);
				texvec[0]= (i*texvec[0]);
				texvec[1]= (i*texvec[1]);
				texvec[2]= (-1.0+i*texvec[2]);

			}
			else {
				skip= 1;
				Ta= 0.0;
			}
		}
		else {
			skip= 1;
			Ta= 0.0;
		}
	}
	else {
		v1= 2.0/PR_RECTX;
	
		texvec[0]= v1*x;
		texvec[1]= v1*y;
		texvec[2]= 0.0;
	}
	
	/* geeft geen Tin terug */
	if(tex->type==TEX_STUCCI) {
		tex->nor= R.vn;
		R.vn[0]= 1.0;
		R.vn[1]= R.vn[2]= 0.0;
	}
	
	if(skip==0) rgbnor= multitex(tex, texvec, 0, 0);
	else rgbnor= 1;
	
	if(rgbnor & 1) {
		
		rect[0]= 255.0*Tr;
		rect[1]= 255.0*Tg;
		rect[2]= 255.0*Tb;
		
		if(Ta!=1.0) {
			tracol=  64+100*(abs(x)>abs(y));
			tracol= (1.0-Ta)*tracol;
			
			rect[0]= tracol+ (rect[0]*Ta) ;
			rect[1]= tracol+ (rect[1]*Ta) ;
			rect[2]= tracol+ (rect[2]*Ta) ;
					
		}
	}
	else {
	
		if(tex->type==TEX_STUCCI) {
			Tin= 0.5 + 0.7*tex->nor[0];
			CLAMP(Tin, 0.0, 1.0);
		}
		rect[0]= 255.0*Tin;
		rect[1]= 255.0*Tin;
		rect[2]= 255.0*Tin;
	}
}

void shade_preview_pixel(int x, int y, char *rect, int smooth)
{
	Material *mat;
	float v1,inp, inprspec=0, isr=0.0, isb=0.0, isg=0.0;
	float fac, specvec[3], co[3], alpha;
	int temp;
	char *colp, tracol;
	
	mat= R.matren;
	
	if(mat->spec || (mat->texco & TEXCO_REFL)) {
		v1= 1.0/PR_RECTX;
		specvec[0]= v1*x+pr_lamp[0];
		specvec[1]= v1*y+pr_lamp[1];
		specvec[2]= 1.0+pr_lamp[2];
		Normalise(specvec);
	}

	R.refcol[0]= R.refcol[1]= R.refcol[2]= R.refcol[3]= 0.0;

	/* texture afhandeling */
	if(mat->texco) {
		if(smooth) {
			R.lo[0]= R.vn[0];
			R.lo[1]= R.vn[2];
			R.lo[2]= -R.vn[1];
		}
		else {
			v1= 2.0/PR_RECTX;
			R.lo[0]= v1*x;
			R.lo[1]= v1*y;
			R.lo[2]= 0.0;
		}
		if(mat->texco & TEXCO_GLOB) {
			VECCOPY(R.gl, R.lo);
		}
		if(mat->texco & TEXCO_WINDOW) {
			VECCOPY(R.winco, R.lo);
		}
		if(mat->texco & TEXCO_STICKY) {
			VECCOPY(R.sticky, R.lo);
		}
		if(mat->texco & TEXCO_UV) {
			VECCOPY(R.uv, R.lo);
		}
		if(mat->texco & TEXCO_OBJECT) {
			VECCOPY(R.co, R.lo);
		}
		if(mat->texco & TEXCO_NORM) {
			R.orn[0]= R.vn[0];
			R.orn[1]= -R.vn[1];
			R.orn[2]= R.vn[2];
		}
		if(mat->texco & TEXCO_REFL) {
			/* for bump texture */
			VECCOPY(R.view, specvec);
			
			inp= -2.0*(R.vn[0]*specvec[0]+R.vn[1]*specvec[1]+R.vn[2]*specvec[2]);
			R.ref[0]= (specvec[0]+inp*R.vn[0]);
			R.ref[1]= (specvec[1]+inp*R.vn[1]);
			R.ref[2]= (specvec[2]+inp*R.vn[2]);
		}

		do_material_tex();
	}
	
	if(mat->mode & MA_SHLESS) {
		temp= 255.0*(mat->r);
		if(temp>255) rect[0]= 255; else if(temp<0) rect[0]= 0; else rect[0]= temp;

		temp= 255.0*(mat->g);
		if(temp>255) rect[1]= 255; else if(temp<0) rect[1]= 0; else rect[1]= temp;

		temp= 255.0*(mat->b);
		if(temp>255) rect[2]= 255; else if(temp<0) rect[2]= 0; else rect[2]= temp;
	}
	else {
		inp= R.vn[0]*pr_lamp[0]+R.vn[1]*pr_lamp[1]+R.vn[2]*pr_lamp[2];
		if(inp<0.0) inp= 0.0;
		
		if(mat->spec)  {
			if(inp>0.0) {
				v1= specvec[0]*R.vn[0]+specvec[1]*R.vn[1]+specvec[2]*R.vn[2];
				if(v1>0.0) {
					v1= Spec(v1, mat->har);
					inprspec= v1*mat->spec;
					isr= inprspec*mat->specr;
					isg= inprspec*mat->specg;
					isb= inprspec*mat->specb;
				}
			}
		}
		inp= (mat->ref*inp + mat->emit);

		if(R.refcol[0]!=0.0) {
			
			temp= 255.0*( mat->mirr*R.refcol[1] + (1.0 - mat->mirr*R.refcol[0])*(mat->ambr+inp*mat->r) + isr);
			if(temp>255) rect[0]= 255; else if(temp<0) rect[0]= 0; else rect[0]= temp;

			temp= 255.0*( mat->mirg*R.refcol[2] + (1.0 - mat->mirg*R.refcol[0])*(mat->ambg+inp*mat->g) + isg);
			if(temp>255) rect[1]= 255; else if(temp<0) rect[1]= 0; else rect[1]= temp;

			temp= 255.0*( mat->mirb*R.refcol[3] + (1.0 - mat->mirb*R.refcol[0])*(mat->ambb+inp*mat->b) + isb);
			if(temp>255) rect[2]= 255; else if(temp<0) rect[2]= 0; else rect[2]= temp;
		}
		else {
			temp= 255.0*(mat->ambr+inp*mat->r + isr);
			if(temp>255) rect[0]= 255; else if(temp<0) rect[0]= 0; else rect[0]= temp;
	
			temp= 255.0*(mat->ambg+inp*mat->g + isg);
			if(temp>255) rect[1]= 255; else if(temp<0) rect[1]= 0; else rect[1]= temp;
	
			temp= 255.0*(mat->ambb+inp*mat->b + isb);
			if(temp>255) rect[2]= 255; else if(temp<0) rect[2]= 0; else rect[2]= temp;
		}
	}

	if(mat->alpha!=1.0) {
		
		alpha= mat->alpha;
		
			/* ztra shade */
		if(mat->spectra!=0.0) {
			inp= mat->spectra*inprspec;
			if(inp>1.0) inp= 1.0;
			
			alpha= (1.0-inp)*alpha+ inp;
		}

		tracol=  previewback(mat->pr_type, x, y) & 255;
		
		tracol= (1.0-alpha)*tracol;
		
		rect[0]= tracol+ (rect[0]*alpha) ;
		rect[1]= tracol+ (rect[1]*alpha) ;
		rect[2]= tracol+ (rect[2]*alpha) ;

	}
}


void previewrender()
{
	Material *mat=0;
	Tex *tex;
	Image *ima;
	Lamp *la;
	LampRen *lar;
	HaloRen har;
	Object *ob;
	World *wrld;
	float lens;
	int x, y, starty, startx, endy, endx, radsq, xsq, ysq, last;
	uint *rect;
	
	if(G.vd==0) return;	/* kan voorkomen als er geen 3D win geopend is */ 
	if(G.buts->cury>=PR_RECTY) return;
	
	if ELEM4(G.buts->mainb, BUTS_MAT, BUTS_TEX, BUTS_LAMP, BUTS_WORLD);
	else return;
	
	har.flarec= 0;	/* verderop test op postrender flare */
	
	usleep(2);
	if(qtest()) {
		addafterqueue(curarea->win, RENDERPREVIEW, 1);
		return;
	}
	
	R.osatex= 0;
	if(G.buts->mainb==BUTS_MAT) {
		mat= G.buts->lockpoin;
		if(mat==0) return;

		/* rendervars */
		init_render_world();
		init_render_material(mat);
		
		/* imats wissen */
		for(x=0; x<8; x++) {
			if(mat->mtex[x]) {
				if(mat->mtex[x]->tex) {
					init_render_texture(mat->mtex[x]->tex);
					
					if(mat->mtex[x]->tex->env && mat->mtex[x]->tex->env->object) 
						Mat4One(mat->mtex[x]->tex->env->object->imat);
				}
				if(mat->mtex[x]->object) Mat4One(mat->mtex[x]->object->imat);
				if(mat->mtex[x]->object) Mat4One(mat->mtex[x]->object->imat);
			}
		}
		R.vlr= 0;
		R.mat= mat;
		R.matren= mat->ren;
		
		if(mat->mode & MA_HALO) init_previewhalo(&har, mat);
	}
	else if(G.buts->mainb==BUTS_TEX) {
		tex= G.buts->lockpoin;
		if(tex==0) return;
		ima= tex->ima;
		if(ima) last= ima->lastframe;
		init_render_texture(tex);
		free_unused_animimages();
		if(tex->ima) {
			if(tex->ima!=ima) allqueue(REDRAWBUTSTEX, 0);
			else if(last!=ima->lastframe) allqueue(REDRAWBUTSTEX, 0);
		}
		if(tex->env && tex->env->object) 
			Mat4Invert(tex->env->object->imat, tex->env->object->obmat);
	}
	else if(G.buts->mainb==BUTS_LAMP) {
		ob= OBACT;
		if(ob==0 || ob->type!=OB_LAMP) return;
		la= ob->data;
		Mat4One(R.viewmat);
		Mat4One(R.viewinv);
		init_render_world();
		init_render_textures();	/* ze mogen niet twee keer!! (brightness) */
		R.totlamp= 0;
		add_render_lamp(ob, 0);	/* 0=no shadbuf */
		lar= R.la[0];
		
		/* uitzonderingen: */
		lar->spottexfac= 1.0;
		lar->spotsi= fcos( M_PI/3.0 );
		lar->spotbl= (1.0-lar->spotsi)*la->spotblend;
		
		Mat3One(lar->imat);
	}
	else {
		wrld= G.buts->lockpoin;
		if(wrld==0) return;
		
		lens= 35.0;
		if(G.scene->camera) {
			lens= ( (Camera *)G.scene->camera->data)->lens;
		}
		Mat4CpyMat4(R.viewmat, G.vd->viewmat);
		Mat4CpyMat4(R.viewinv, G.vd->viewinv);
		init_render_world();
		init_render_textures();	/* ze mogen niet twee keer!! (brightness) */
	}

	init_view2d_calc();
	set_previewrect(PR_XMIN, PR_YMIN, PR_XMAX, PR_YMAX);

	if(G.buts->rect==0) {
		G.buts->rect= callocN(sizeof(int)*PR_RECTX*PR_RECTY, "butsrect");
	}
	
	starty= -PR_RECTY/2;
	startx= -PR_RECTX/2;
	endy= starty+PR_RECTY;
	endx= startx+PR_RECTX;
	starty+= G.buts->cury;
	
	radsq= (PR_RECTX/2)*(PR_RECTY/2);
	
	glDrawBuffer(GL_FRONT);

	rect= G.buts->rect+ PR_RECTX*G.buts->cury;
	for(y=starty; y<endy; y++) {

		if(G.buts->mainb==BUTS_MAT) {
			
			if(mat->mode & MA_HALO) {
				for(x=startx; x<endx; x++, rect++) {
					rect[0]= previewback(mat->pr_type, x, y);
				}

				if(har.flarec) {
					if(y==endy-1) previewflare(&har, G.buts->rect);
				}
				else {
					halo_preview_pixel(&har, startx, endx, y, (char *) (rect-PR_RECTX));
				}
			}
			else {
				ysq= y*y;
				for(x=startx; x<endx; x++, rect++) {
					xsq= x*x;
					if(mat->pr_type & 1) {
						if(xsq+ysq < radsq) {
							R.vn[2]= fsqrt( (float)(radsq-xsq-ysq) );
							R.vn[0]= -x;
							R.vn[1]= -y;
							Normalise(R.vn);
							shade_preview_pixel(x, y, (char *)rect, 1);
						}
						else {
							rect[0]= previewback(mat->pr_type, x, y);
						}
					}
					else {
						R.vn[0]= R.vn[1]= 0.0;
						R.vn[2]= 1.0;
						shade_preview_pixel(x, y, (char *)rect, 0);
					}
				}
			}
		}
		else if(G.buts->mainb==BUTS_TEX) {
			for(x=startx; x<endx; x++, rect++) {
				texture_preview_pixel(tex, x, y, (char *)rect);
			}
		}
		else if(G.buts->mainb==BUTS_LAMP) {
			for(x=startx; x<endx; x++, rect++) {
				lamp_preview_pixel(lar, x, y, (char *)rect);
			}
		}
		else  {
			for(x=startx; x<endx; x++, rect++) {				
				sky_preview_pixel(lens, x, y, (char *)rect);
			}
		}
		
		if(y<endy-2) {
		
			/* zwart lijntje */

			/* cpack(0x0); */
			/* ysq= PR_YMIN+2+ ((G.buts->cury)*PR_FACY);  */
			/* sdrawline(PR_XMIN+4, ysq, PR_XMAX-4, ysq); */

			if(qtest()) {
				addafterqueue(curarea->win, RENDERPREVIEW, 1);
				break;
			}
		}

		display_pr_scanline(G.buts->rect, G.buts->cury);
		
		G.buts->cury++;
	}

	if(G.buts->cury>=PR_RECTY && G.buts->mainb==BUTS_TEX) 
		draw_tex_crop(G.buts->lockpoin);
	
	glDrawBuffer(GL_BACK);
	#if defined(__WIN32) || defined(MESA31)
	previewdraw();
	#else 
	curarea->win_swap= WIN_FRONT_OK;
	#endif
	
	if(G.buts->mainb==BUTS_MAT) {
		end_render_material(mat);
		for(x=0; x<8; x++) {
			if(mat->mtex[x] && mat->mtex[x]->tex) end_render_texture(mat->mtex[x]->tex);
		}	
	}
	else if(G.buts->mainb==BUTS_TEX) {
		end_render_texture(tex);
	}
	else if(G.buts->mainb==BUTS_WORLD) {
		end_render_textures();
	}
	else if(G.buts->mainb==BUTS_LAMP) {
		if(R.totlamp) {
			if(R.la[0]->org) freeN(R.la[0]->org);
			freeN(R.la[0]);
		}
		R.totlamp= 0;
		end_render_textures();
	}
}

