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


/* drawview.c  april 94		GRAPHICS
 * 
 * 
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "sector.h"
#include "render.h"


void setalpha_bgpic(BGpic *bgpic)
{
	int x, y, alph;
	char *rect;
	
	alph= (int)(255.0*(1.0-bgpic->blend));
	
	rect= (char *)bgpic->rect;
	for(y=0; y< bgpic->yim; y++) {
		for(x= bgpic->xim; x>0; x--, rect+=4) {
			rect[3]= alph;
		}
	}
}


/* ******************************************** */
#ifdef IRISGL
/* ******************************************** */

float defmatgl[]= {
	AMBIENT, 0.1, 0.1, 0.1,
	DIFFUSE, 0.9, 0.9, 0.9,
	SPECULAR, 0.5, 0.5, 0.5,
	EMISSION, 0.0, 0.0, 0.0, 
	SHININESS, 35.0,
	LMNULL
};

float defmattexgl[]= {
	AMBIENT, 0.0, 0.0, 0.0,
	DIFFUSE, 0.0, 0.0, 0.0,
	SPECULAR, 0.0, 0.0, 0.0,
	EMISSION, 2.0, 2.0, 2.0, 
	SHININESS, 0.0,
	LMNULL
};

float lmodel[]= {
	AMBIENT, 1.0, 1.0, 1.0,
	LOCALVIEWER, 0,
	TWOSIDE, 0, 
	LMNULL
};
float light[]= {
	LCOLOR, 0.8, 0.8, 0.8,
	POSITION, -0.3, 0.3, 0.90, 0.0,
	LMNULL
};
float blue_light[] =
    {LCOLOR,    0.5, 0.5, 0.8,
     POSITION,  0.2, 0.1, -1.0, 0.0, 
     LMNULL};

void deflighting()
{
	lmdef(DEFMATERIAL, 1, 0, defmatgl);
	lmdef(DEFMATERIAL, 2, 0, defmattexgl);
	lmdef(DEFLIGHT, 1, 10, light);
	lmdef(DEFLIGHT, 2, 10, blue_light);
	lmdef(DEFLMODEL, 1, 0, lmodel);
}

void bindlighting()
{
	lmcolor(LMC_COLOR);
	
	lmbind(MATERIAL, 1);
	lmbind(LIGHT0, 1);
	/* lmbind(LIGHT1, 2); */
	lmbind(LMODEL, 1);
	
}

void bindtexlighting()
{
	lmcolor(LMC_EMISSION);
	
	lmbind(MATERIAL, 2);
	lmbind(LIGHT0, 0);
	
}


void two_sided(int val)
{
	
	if(val && lmodel[7]==1.0) return;
	if(val==0 && lmodel[7]==0.0) return;
	
	if(val) lmodel[7]= 1.0;
	else lmodel[7]= 0.0;
	
	lmdef(DEFLMODEL, 1, 0, lmodel);
}

static int maxmat=1;

void set_gl_material(int nr)
{
	if(nr>maxmat) nr= maxmat;
	lmbind(MATERIAL, nr);
}

void def_gl_material(int matnr, Material *ma)
{
	if(ma==0) {
		defmatgl[1]= defmatgl[2]= defmatgl[3]= 0.1;
		
		defmatgl[5]= defmatgl[6]= defmatgl[7]= 0.9;
	}
	else {
		if(ma->mode & MA_SHLESS) {
			defmatgl[5]= defmatgl[6]= defmatgl[7]= 0.0;
			defmatgl[1]= ma->r;
			defmatgl[2]= ma->g;
			defmatgl[3]= ma->b;
		}
		else {
			defmatgl[1]= defmatgl[2]= defmatgl[3]= 0.1;
		
			defmatgl[5]= (ma->ref+ma->emit)*ma->r;
			defmatgl[6]= (ma->ref+ma->emit)*ma->g;
			defmatgl[7]= (ma->ref+ma->emit)*ma->b;

			defmatgl[9]=  ma->spec*ma->specr;
			defmatgl[10]= ma->spec*ma->specg;
			defmatgl[11]= ma->spec*ma->specb;
		}

	}
	if(matnr==0) matnr= 1;
	lmdef(DEFMATERIAL, matnr, 0, defmatgl);
}

void init_gl_materials(Object *ob)
{
	Material *ma;
	int a;
	
	if(ob->totcol==0) {
		def_gl_material(0, 0);
		maxmat= 1;
	}
	else {
		maxmat= ob->totcol;
		for(a=0; a<ob->totcol; a++) {
			ma= give_current_material(ob, a+1);
			def_gl_material(a+1, ma);
		}
	}	
}

void init_gl_stuff(void)	
{
	glEnable(GL_DITHER);
	deflighting();
	bindlighting();
}

void lrectwriteRGBA(int a, int b, int c, int d, uint *rect)
{
	int size;
	char *rectt, *rt1, *rt2;
	
	size= (c-a+1)*(d-b+1);
	rectt= mallocN(size*4, "temprect");

	rt1= (char *)rect;
	rt2= rectt;

	while(size-- > 0) {
		rt2[3]= rt1[0];
		rt2[2]= rt1[1];
		rt2[1]= rt1[2];
		rt2[0]= rt1[3];
		rt1+= 4;
		rt2+= 4;
	}
	
	lrectwrite(a, b, c, d, rectt);
	freeN(rectt);
}

void draw_bgpic(void)
{
	BGpic *bgpic;
	Image *ima;
	float vec[3], fac, asp, zoom;
	uint *rect;
	int x1, y1, x2, y2, cx, cy, xim, yim;
	short mval[2];
	char str[100];
	
	bgpic= G.vd->bgpic;
	if(bgpic==0) return;
	
	if(bgpic->tex) {
		init_render_texture(bgpic->tex);
		free_unused_animimages();
		ima= bgpic->tex->ima;
		end_render_texture(bgpic->tex);
	}
	else {
		ima= bgpic->ima;
	}
	
	if(ima==0) return;
	if(ima->ok==0) return;
	
	/* plaatje testen */
	if(ima->ibuf==0) {
		if(bgpic->tex) {
			ima_ibuf_is_nul(bgpic->tex);
		}
		else {
			strcpy(str, ima->name);
			convertstringcode(str);
			waitcursor(1);
			ima->ibuf = loadiffname(str , LI_rect);
			waitcursor(0);
		}
		if(ima->ibuf==0) {
			ima->ok= 0;
			return;
		}
		if(bgpic->rect) freeN(bgpic->rect);
		bgpic->rect= 0;
	}

	if(G.vd->persp==2) {
		x1= G.vd->pr_xmin+curarea->winrct.xmin;
		y1= G.vd->pr_ymin+curarea->winrct.ymin;
		x2= G.vd->pr_xmax+curarea->winrct.xmin;
		y2= G.vd->pr_ymax+curarea->winrct.ymin;
	}
	else {
		/* windowco berekenen */
		initgrabz(0.0, 0.0, 0.0);
		window_to_3d(vec, 1, 0);
		fac= MAX3( fabs(vec[0]), fabs(vec[1]), fabs(vec[1]) );
		fac= 1.0/fac;
	
		asp= ( (float)ima->ibuf->y)/(float)ima->ibuf->x;
	
		vec[0]= vec[1]= vec[2]= 0.0;
		project_short_noclip(vec, mval);
		cx= curarea->winrct.xmin+mval[0];
		cy= curarea->winrct.ymin+mval[1];
	
		x1=  cx+ fac*(bgpic->xof-bgpic->size);
		y1=  cy+ asp*fac*(bgpic->yof-bgpic->size);
		x2=  cx+ fac*(bgpic->xof+bgpic->size);
		y2=  cy+ asp*fac*(bgpic->yof+bgpic->size);
	}
	
	xim= x2-x1;
	yim= y2-y1;
	zoom= 1.0;
		
	if(xim<4 || yim<4) return;
	
	if(xim > ima->ibuf->x) {
		zoom= xim;
		zoom /= (float)ima->ibuf->x;
		zoom= fceil(zoom);
		
		xim= xim/zoom;
		yim= yim/zoom;
	}

	if(bgpic->rect==0 || zoom!=bgpic->zoom || xim!=bgpic->xim || yim!=bgpic->yim) {
		if(bgpic->rect) freeN(bgpic->rect);
		bgpic->rect= mallocN(xim*yim*4, "bgpicrect");
		
		scalefastrect(ima->ibuf->rect, bgpic->rect, ima->ibuf->x, ima->ibuf->y, xim, yim);

		bgpic->xim= xim;
		bgpic->yim= yim;
		bgpic->zoom= zoom;
		/* convert_rgba_to_abgr(xim*yim,bgpic->rect); */
		setalpha_bgpic(bgpic);
	}
	
	/* coordinaten hoe 't op scherm komt */
	x2= x1+ zoom*(bgpic->xim-1);
	y2= y1+ zoom*(bgpic->yim-1);

	/* volledige clip? */
	
	if(x2 < curarea->winrct.xmin ) return;
	if(y2 < curarea->winrct.ymin ) return;
	if(x1 > curarea->winrct.xmax ) return;
	if(y1 > curarea->winrct.ymax ) return;
	
	/* rectwrite coordinaten */
	xim= xim-1;
	yim= yim-1;
	rect= bgpic->rect;

	/* partiele clip */
	if(x1<curarea->winrct.xmin) {
		cx= curarea->winrct.xmin-x1;
		/* zorg ervoor dat de rect pixelnauwkeurig wordt neergezet */
		cx/= zoom;
		x1+= zoom*cx;
		xim-= cx;
		rect+= cx;
	}
	if(y1<curarea->winrct.ymin) {
		cy= curarea->winrct.ymin-y1;
		cy/= zoom;
		y1+= zoom*cy;
		rect+= cy*bgpic->xim;
		yim-= cy;
	}
	if(x2>curarea->winrct.xmax) {
		cx= x2-curarea->winrct.xmax;
		cx/= zoom;
		xim-= cx;
	}
	if(y2>curarea->winrct.ymax) {
		cy= y2-curarea->winrct.ymax;
		cy/= zoom;
		yim-= cy;
	}
	
	if(xim<=0) return;
	if(yim<=0) return;

	rectzoom(zoom, zoom);

	/* geeft aan dat per scanline rectwriten, ibuf->x in de rect verder gelezen wordt */
	pixmode(PM_STRIDE, bgpic->xim);

	blendfunction(BF_SA, BF_MSA);
	lrectwriteRGBA(x1, y1, x1+xim, y1+yim, rect);
	blendfunction(BF_ONE, BF_ZERO);
	rectzoom(1.0, 1.0);
	
	pixmode(PM_STRIDE, 0);
}


/* ******************************************** */
#else
/* ******************************************** */

float light_pos1[] = { -0.3, 0.3, 0.90, 0.0 }; 
float light_pos2[] = { 0.3, -0.3, -0.90, 0.0 }; 

void init_gl_stuff()	
{
	float mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
	float mat_shininess[] = { 35.0 };
	float light_col1[] = { 0.8, 0.8, 0.8, 0.0 }; 
	float one= 1.0;
		
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos1); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_col1); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular); 

	glEnable(GL_LIGHT0);
	
	#if defined(__FreeBSD) || defined(linux)
	glDisable(GL_DITHER);	/* op sgi/sun hardware && 12 bits */
	#endif
	
	/* glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, &one); */
	
	glDepthFunc(GL_LEQUAL);
	/* scaling matrices */
	glEnable(GL_NORMALIZE);

	glShadeModel(GL_FLAT);

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glDisable(GL_LOGIC_OP);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);

	glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
	glPixelTransferi(GL_RED_SCALE, 1);
	glPixelTransferi(GL_RED_BIAS, 0);
	glPixelTransferi(GL_GREEN_SCALE, 1);
	glPixelTransferi(GL_GREEN_BIAS, 0);
	glPixelTransferi(GL_BLUE_SCALE, 1);
	glPixelTransferi(GL_BLUE_BIAS, 0);
	glPixelTransferi(GL_ALPHA_SCALE, 1);
	glPixelTransferi(GL_ALPHA_BIAS, 0);

	init_realtime_GL();	/* drawsector.c */
}

void two_sided(int val)
{

	/* twosided aan: geft errors bij x flip! */
	glLightModeliv(GL_LIGHT_MODEL_TWO_SIDE, (GLint *)&val);
}

void circf(float x, float y, float rad)
{
	GLUquadricObj *qobj = gluNewQuadric(); 
	
	gluQuadricDrawStyle(qobj, GLU_FILL); 
	
	glPushMatrix(); 
	
	glTranslatef(x,  y, 0.); 
	
	gluDisk( qobj, 0.0,  rad, 32, 1); 
	
	glPopMatrix(); 
	
	gluDeleteQuadric(qobj);
}

void circ(float x, float y, float rad)
{
	GLUquadricObj *qobj = gluNewQuadric(); 
	
	gluQuadricDrawStyle(qobj, GLU_SILHOUETTE); 
	
	glPushMatrix(); 
	
	glTranslatef(x,  y, 0.); 
	
	gluDisk( qobj, 0.0,  rad, 32, 1); 
	
	glPopMatrix(); 
	
	gluDeleteQuadric(qobj);
}

/* ********** IN ONTWIKKELING ********** */

#define MAXMATBUF 16
float matbuf[MAXMATBUF][2][3];

void init_gl_materials(Object *ob)
{
	extern Material defmaterial;
	Material *ma;
	int a;
	
	if(ob->totcol==0) {
		matbuf[0][0][0]= defmaterial.r;
		matbuf[0][0][1]= defmaterial.g;
		matbuf[0][0][2]= defmaterial.b;
		
		matbuf[0][1][0]= defmaterial.specr;
		matbuf[0][1][1]= defmaterial.specg;
		matbuf[0][1][2]= defmaterial.specb;
		
		/* ook matnr 1, displists! */
		VECCOPY(matbuf[1][0], matbuf[0][0]);
		VECCOPY(matbuf[1][1], matbuf[0][1]);
	}
	
	for(a=1; a<=ob->totcol; a++) {
		ma= give_current_material(ob, a);
		if(ma && a<MAXMATBUF) {
			/* if(ma->mode & MA_SHLESS) { */
			/*  */
			/* } */
			/* else { */
				matbuf[a][0][0]= (ma->ref+ma->emit)*ma->r;
				matbuf[a][0][1]= (ma->ref+ma->emit)*ma->g;
				matbuf[a][0][2]= (ma->ref+ma->emit)*ma->b;
				
				matbuf[a][1][0]= ma->spec*ma->specr;
				matbuf[a][1][1]= ma->spec*ma->specg;
				matbuf[a][1][2]= ma->spec*ma->specb;
			/* } */
		}
	}
}

void set_gl_material(int nr)
{

	if(nr<MAXMATBUF) {
		if(matbuf[nr][0]) {
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matbuf[nr][0]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matbuf[nr][1]);
		}
	}
}


void draw_bgpic()
{
	BGpic *bgpic;
	Image *ima;
	float vec[3], fac, asp, zoomx, zoomy;
	uint *rect;
	int x1, y1, x2, y2, cx, cy, xim, yim;
	short mval[2];
	char str[FILE_MAXDIR+FILE_MAXFILE];
	
	bgpic= G.vd->bgpic;
	if(bgpic==0) return;
	
	if(bgpic->tex) {
		init_render_texture(bgpic->tex);
		free_unused_animimages();
		ima= bgpic->tex->ima;
		end_render_texture(bgpic->tex);
	}
	else {
		ima= bgpic->ima;
	}
	
	if(ima==0) return;
	if(ima->ok==0) return;
	
	/* plaatje testen */
	if(ima->ibuf==0) {
	
		if(bgpic->rect) freeN(bgpic->rect);
		bgpic->rect= 0;
		
		if(bgpic->tex) {
			ima_ibuf_is_nul(bgpic->tex);
		}
		else {
			strcpy(str, ima->name);
			convertstringcode(str);
			waitcursor(1);
			ima->ibuf = loadiffname(str , LI_rect);
			waitcursor(0);
		}
		if(ima->ibuf==0) {
			ima->ok= 0;
			return;
		}
	}

	if(bgpic->rect==0) {
		
		bgpic->rect= dupallocN(ima->ibuf->rect);
		bgpic->xim= ima->ibuf->x;
		bgpic->yim= ima->ibuf->y;
		setalpha_bgpic(bgpic);
	}

	if(G.vd->persp==2) {
		x1= G.vd->pr_xmin;
		y1= G.vd->pr_ymin;
		x2= G.vd->pr_xmax;
		y2= G.vd->pr_ymax;
	}
	else {
		/* windowco berekenen */
		initgrabz(0.0, 0.0, 0.0);
		window_to_3d(vec, 1, 0);
		fac= MAX3( fabs(vec[0]), fabs(vec[1]), fabs(vec[1]) );
		fac= 1.0/fac;
	
		asp= ( (float)ima->ibuf->y)/(float)ima->ibuf->x;
	
		vec[0]= vec[1]= vec[2]= 0.0;
		project_short_noclip(vec, mval);
		cx= mval[0];
		cy= mval[1];
	
		x1=  cx+ fac*(bgpic->xof-bgpic->size);
		y1=  cy+ asp*fac*(bgpic->yof-bgpic->size);
		x2=  cx+ fac*(bgpic->xof+bgpic->size);
		y2=  cy+ asp*fac*(bgpic->yof+bgpic->size);
	}
	
	/* volledige clip? */
	
	if(x2 < 0 ) return;
	if(y2 < 0 ) return;
	if(x1 > curarea->winx ) return;
	if(y1 > curarea->winy ) return;
	
	zoomx= x2-x1;
	zoomx /= (float)ima->ibuf->x;
	zoomy= y2-y1;
	zoomy /= (float)ima->ibuf->y;

	glEnable(GL_BLEND);
	if(G.zbuf) glDisable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA); 
	 
	rectwrite_part(curarea->winrct.xmin, curarea->winrct.ymin, curarea->winrct.xmax, curarea->winrct.ymax, 
		x1+curarea->winrct.xmin, y1+curarea->winrct.ymin, ima->ibuf->x, ima->ibuf->y, zoomx, zoomy, bgpic->rect);

	glBlendFunc(GL_ONE,  GL_ZERO); 
	glDisable(GL_BLEND);
	if(G.zbuf) glEnable(GL_DEPTH_TEST);
	 
}

/* ******************************************** */
#endif
/* ******************************************** */


void timestr(int time, char *str)
{
	/* formaat 00:00:00.00 (hr:min:sec) string moet 12 lang */

	float temp;
	short hr,min,sec,hun;

	temp= ((float)time)/(100.0);
	min= ffloor(temp/60.0);
	hr= min/60;
	min-= 60*hr;
	temp-= (float)60*min;
	sec= ffloor(temp);
	temp-= (float)sec;
	hun= ffloor(100*temp);

	if(hr) sprintf(str,"%.2d:%.2d:%.2d.%.2d",hr,min,sec,hun);
	else sprintf(str,"%.2d:%.2d.%.2d",min,sec,hun);
	str[11]=0;
}


void drawgrid()
{
	/* extern short bgpicmode; */
	float wx, wy, x, y, fz, fw, fx, fy, dx, dy;
	float vec4[4];

	vec4[0]=vec4[1]=vec4[2]=0.0; 
	vec4[3]= 1.0;
	Mat4MulVec4fl(G.vd->persmat, vec4);
	fx= vec4[0]; 
	fy= vec4[1]; 
	fz= vec4[2]; 
	fw= vec4[3];

	wx= (curarea->winx/2.0);	/* ivm afrondfoutjes, grid op verkeerde plek */
	wy= (curarea->winy/2.0);

	x= (wx)*fx/fw;
	y= (wy)*fy/fw;

	vec4[0]=vec4[1]=G.vd->grid; 
	vec4[2]= 0.0;
	vec4[3]= 1.0;
	Mat4MulVec4fl(G.vd->persmat, vec4);
	fx= vec4[0]; 
	fy= vec4[1]; 
	fz= vec4[2]; 
	fw= vec4[3];

	dx= fabs(x-(wx)*fx/fw);
	if(dx==0) dx= fabs(y-(wy)*fy/fw);

	if(dx<6.0) {
		dx*= 10.0;
		setlinestyle(3);
		if(dx<6.0) {
			dx*= 10.0;
			if(dx<6.0) {
				setlinestyle(0);
				return;
			}
		}
	}
	
	persp(0);

	cpack(0x505050);
	
	x+= (wx); 
	y+= (wy);
	fx= x/dx;
	fx= x-dx*ffloor(fx);
	
	while(fx< curarea->winx) {
		fdrawline(fx,  0.0,  fx,  (float)curarea->winy); 
		fx+= dx; 
	}

	fy= y/dx;
	fy= y-dx*ffloor(fy);
	

	while(fy< curarea->winy) {
		fdrawline(0.0,  fy,  (float)curarea->winx,  fy); 
		fy+= dx;
	}

	/* kruis in midden */
	if(G.vd->view==3) cpack(0xA0D0A0); /* y-as */
	else cpack(0xA0A0C0);	/* x-as */

	fdrawline(0.0,  y,  (float)curarea->winx,  y); 
	
	if(G.vd->view==7) cpack(0xA0D0A0);	/* y-as */
	else cpack(0xD0A0A0);	/* z-as */

	fdrawline(x,  0.0,  x,  (float)curarea->winy); 

	persp(1);
	setlinestyle(0);
}


void drawfloor()
{
	View3D *vd;
	float vert[3], grid;
	int a, b, gridlines;
	
	vd= curarea->spacedata.first;

	vert[2]= 0.0;

	if(vd->gridlines<3) return;

	gridlines= vd->gridlines/2;
	grid= gridlines*vd->grid;
	
	cpack(0x404040);

	for(a= -gridlines;a<=gridlines;a++) {

		if(a==0) {
			if(vd->persp==0) cpack(0xA0D0A0);
			else cpack(0x402000);
		}
		else if(a==1) {
			cpack(0x404040);
		}
		
	
		glBegin(GL_LINE_STRIP);
			vert[0]= a*vd->grid;
			vert[1]= grid;
			glVertex3fv(vert);
			vert[1]= -grid;
			glVertex3fv(vert);
		glEnd();
	}
	
	cpack(0x404040);
	
	for(a= -gridlines;a<=gridlines;a++) {
		if(a==0) {
			if(vd->persp==0) cpack(0xA0A0D0);
			else cpack(0);
		}
		else if(a==1) {
			cpack(0x404040);
		}
	
		glBegin(GL_LINE_STRIP);
			vert[1]= a*vd->grid;
			vert[0]= grid;
			glVertex3fv(vert );
			vert[0]= -grid;
			glVertex3fv(vert);
		glEnd();
	}

}

void drawcursor()
{

	if(G.f & G_PLAYANIM) return;
	
	project_short( give_cursor(), &G.vd->mx);

	G.vd->mxo= G.vd->mx;
	G.vd->myo= G.vd->my;

	if( G.vd->mx!=3200) {
		
		setlinestyle(0); 
		cpack(0xFF);
		circ((float)G.vd->mx, (float)G.vd->my, 10.0);
		setlinestyle(4); 
		cpack(0xFFFFFF);
		circ((float)G.vd->mx, (float)G.vd->my, 10.0);
		setlinestyle(0);
		cpack(0x0);
		
		sdrawline(G.vd->mx-20, G.vd->my, G.vd->mx-5, G.vd->my);
		sdrawline(G.vd->mx+5, G.vd->my, G.vd->mx+20, G.vd->my);
		sdrawline(G.vd->mx, G.vd->my-20, G.vd->mx, G.vd->my-5);
		sdrawline(G.vd->mx, G.vd->my+5, G.vd->mx, G.vd->my+20);
	}
}

void calc_viewborder()
{
	float a, fac, wx, wy, x1, x2, y1, y2;
	float imax, imay, width;
	
	wx= curarea->winx;
	wy= curarea->winy;
	imax= G.scene->r.xsch;
	imay= G.scene->r.ysch;
	
	fac= ((float)imay*G.scene->r.yasp)/((float)imax*G.scene->r.xasp);
	imay= fac*imax;
	
		/* liggend plaatje */
	if(imax>imay) {
		if(wx<wy) width= wy;
		else width= wx;
		
		fac= width/(2.0*imax);
		
		x1= 0.5*wx-0.25*width;
		x2= 0.5*wx+0.25*width;
		y1= 0.5*wy - 0.5*fac*imay;
		y2= 0.5*wy + 0.5*fac*imay;
	}
	else {
		if(wx<wy) width= wy;
		else width= wx;

		fac= width/(2.0*imay);
		
		y1= 0.5*wy-0.25*width;
		y2= 0.5*wy+0.25*width;
		x1= 0.5*wx - 0.5*fac*imax;
		x2= 0.5*wx + 0.5*fac*imax;
	}


	/* zoom van viewborder */
	fac= (1.41421+( (float)G.vd->camzoom )/50.0);
	fac*= fac;
	
	width= (x2-x1)/4.0;
	a= (x2+x1)/2.0;
	x1= a-width*fac;
	x2= a+width*fac;

	width= (y2-y1)/4.0;
	a= (y2+y1)/2.0;
	y1= a-width*fac;
	y2= a+width*fac;
	
	/* deze getallen voor renderdisplay */
	G.vd->pr_xmin= fceil(x1);
	G.vd->pr_ymin= fceil(y1);
	G.vd->pr_xmax= fceil(x2);
	G.vd->pr_ymax= fceil(y2);
}

void drawviewborder()
{
	float fac, a;
	float x1, x2, y1, y2;
	float x3, y3, x4, y4;

	x1= G.vd->pr_xmin;
	y1= G.vd->pr_ymin;
	x2= G.vd->pr_xmax;
	y2= G.vd->pr_ymax;

	/* rand */
	setlinestyle(3);
	cpack(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
	glRectf(x1+1,  y1-1,  x2+1,  y2-1); 
	
	cpack(0xFFFFFF);
	glRectf(x1,  y1,  x2,  y2); 

	/* border */
	if(G.scene->r.mode & R_BORDER) {
		
		cpack(0);
		x3= x1+ G.scene->r.border.xmin*(x2-x1);
		y3= y1+ G.scene->r.border.ymin*(y2-y1);
		x4= x1+ G.scene->r.border.xmax*(x2-x1);
		y4= y1+ G.scene->r.border.ymax*(y2-y1);
		
		glRectf(x3+1,  y3-1,  x4+1,  y4-1); 
		
		cpack(0x4040FF);
		glRectf(x3,  y3,  x4,  y4); 
	}

	/* safetykader */

	fac= 0.1;
	
	a= fac*(x2-x1);
	x1+= a; 
	x2-= a;

	a= fac*(y2-y1);
	y1+= a;
	y2-= a;

	cpack(0);
	glRectf(x1+1,  y1-1,  x2+1,  y2-1);
	cpack(0xFFFFFF);
	glRectf(x1,  y1,  x2,  y2);

	setlinestyle(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void backdrawview3d(int test)
{
	struct Base *base;
	int tel=1;
	uint kleur;

	#if defined(WIN32) || defined(MESA31)
	if(G.f & (G_VERTEXPAINT|G_FACESELECT));
	else {
		G.vd->flag &= ~V3D_NEEDBACKBUFDRAW;
		return;
	}
	#endif

	if(G.vd->flag & V3D_NEEDBACKBUFDRAW); else return;
	if(G.obedit) {
		G.vd->flag &= ~V3D_NEEDBACKBUFDRAW;
		return;
	}
	
	if(test) {
		if(qtest()) {
			addafterqueue(curarea->win, BACKBUFDRAW, 1);
			return;
		}
	}

	#if defined(__sgi) || defined(__SUN) || defined(__BeOS) || defined(WIN32) || defined(MESA31)
	glDisable(GL_DITHER);
	#endif

	if(G.vd->drawtype > OB_WIRE) G.zbuf= TRUE;
	curarea->win_swap &= ~WIN_BACK_OK;
	
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	if(G.zbuf) {
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
	}
	
	G.f |= G_BACKBUFSEL;
	
	if(G.f & (G_VERTEXPAINT|G_FACESELECT)) {
		base= BASACT;
		if(base && (base->lay && G.vd->lay)) {
			draw_object(base);
		}
	}
	else {

		base= FIRSTBASE;
		while(base) {
			
			/* elke base ivm meerdere windows */
			base->selcol= 0x070707 | ( ((tel & 0xF00)<<12) + ((tel & 0xF0)<<8) + ((tel & 0xF)<<4) );
			tel++;
	
			if(base->lay & G.vd->lay) {
				
				if(test) {
					if(qtest()) {
						addafterqueue(curarea->win, BACKBUFDRAW, 1);
						break;
					}
				}
				
				cpack(base->selcol);
				draw_object(base);
			}
			base= base->next;
		}
	}
	
	if(base==0) G.vd->flag &= ~V3D_NEEDBACKBUFDRAW;

	G.f &= ~G_BACKBUFSEL;
	G.zbuf= FALSE; 
	glDisable(GL_DEPTH_TEST);

	#if defined(__sgi) || defined(__SUN) || defined(__BeOS) || defined (WIN32) || defined(MESA31)
	glEnable(GL_DITHER);
	#endif
}

		
void drawname(Object *ob)
{
	char str[8];
	
	cpack(0x404040);
	glRasterPos3f(0.0,  0.0,  0.0);
	str[0]= ' '; str[1]= 0;
	fmsetfont(G.font);
	fmprstr(str);
	fmprstr(ob->id.name+2);
}


void drawview3d_simul(int make_disp)
{
	Object *ob;
	Sector *se;
	Life *lf, *lfl;
	Material *ma;
	uint col;
	int b, a, flag;

	/* hier doen: anders wordt in header getekend */
	areawinset(curarea->win);

	setwinmatrixview3d(0);	/* 0= geen pick rect */

	setviewmatrixview3d();
	
	Mat4MulMat4(G.vd->persmat, G.vd->viewmat, curarea->winmat);
	Mat4Invert(G.vd->persinv, G.vd->persmat);
	Mat4Invert(G.vd->viewinv, G.vd->viewmat);

	if(G.vd->drawtype > OB_WIRE) {
		G.zbuf= TRUE;
		glEnable(GL_DEPTH_TEST);
		
		if(G.scene->world) glClearColor(G.scene->world->horr, G.scene->world->horg, G.scene->world->horb, 0.0); 
		else glClearColor(0.4375, 0.4375, 0.4375, 0.0); 
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
		glClearColor(0.4375, 0.4375, 0.4375, 0.0); 
		glClear(GL_COLOR_BUFFER_BIT);
	}

	loadmatrix(G.vd->viewmat);
	
	/* extern: camera tekenen */
	if(G.vd->persp!=2) {
		ob= G.scene->camera;
		if(ob && (ob->lay & G.vd->lay)) {
			cpack(0x0);
			multmatrix(ob->obmat);
			drawcamera(ob);
			loadmatrix(G.vd->viewmat);
		}
	}
	
	a= G.totsect;
	while(a-- > 0) {
		ob= G.sectorbuf[a]->ob;

		multmatrix(ob->obmat);
		cpack(0x0);
		drawsector(ob, MIN2(ob->dt, G.vd->drawtype), 0);
		loadmatrix(G.vd->viewmat);
		
		se= ob->data;
		
		if(se->lbuf.tot) {	/* here was an de->depth test... for psx speedup i presume */
			for(b=0; b<se->lbuf.tot; b++) {
				ob= se->lbuf.ob[b];
				if(ob->lay & G.vd->lay) {
					lf= ob->data;

					if(lf->dflag & LF_DONTDRAW);
					else {
						multmatrix(ob->obmat);
						
						col= 0;
						if( ma=lf->contact) col= rgb_to_cpack(ma->r, ma->g, ma->b);
						if( lf->flag & LF_DRAWNEAR) col= (0xF0A020);
						drawlife(ob, MIN2(ob->dt, G.vd->drawtype), col);
						if(G.vd->drawtype!=OB_TEXTURE) {
							if(ob->dtx & OB_DRAWNAME) drawname(ob);
							if(ob->dtx & OB_AXIS) drawaxes( lf->axsize);
						}
						loadmatrix(G.vd->viewmat);
					}
				}
			}
		}
	}

	a= G.totlife;
	while(a--) {
		ob= G.lifebuf[a];
		if(ob->type==OB_LIFE && (ob->lay & G.vd->lay)) {
			
			lf= ob->data;

			if(lf->dflag & LF_DONTDRAW);
			else {
				
				multmatrix(ob->obmat);
				if(G.vd->drawtype!=OB_TEXTURE) {
					col= 0;
					if( ma=lf->contact) col= rgb_to_cpack(ma->r, ma->g, ma->b);
					if( lf->flag & LF_DRAWNEAR) col= (0xF0A020);
					drawlife(ob, MIN2(ob->dt, G.vd->drawtype), col);
					if(ob->dtx & OB_DRAWNAME) drawname(ob);
					if(ob->dtx & OB_AXIS) drawaxes( lf->axsize);	
				}
				else drawlife(ob, MIN2(ob->dt, G.vd->drawtype), 0);
				
				loadmatrix(G.vd->viewmat);
			}

			if(lf->type==LF_DYNAMIC) {				
				for(b=0; b<lf->links.tot; b++) {
					
					ob= lf->links.ob[b];
					
					if(ob->type==OB_LIFE) {
						if(ob->lay & G.vd->lay) {
						
							lfl= ob->data;
		
							if(lfl->dflag & LF_DONTDRAW);
							else {
								multmatrix(ob->obmat);
								if(G.vd->drawtype!=OB_TEXTURE) {
									col= 0;
									if( ma=lfl->contact) col= rgb_to_cpack(ma->r, ma->g, ma->b);
									if( lfl->flag & LF_DRAWNEAR) col= (0xF0A020);
									drawlife(ob, MIN2(ob->dt, G.vd->drawtype), col);
									if(ob->dtx & OB_DRAWNAME) drawname(ob);
									if(ob->dtx & OB_AXIS) drawaxes( lfl->axsize);	
								}
								else drawlife(ob, MIN2(ob->dt, G.vd->drawtype), 0);
								
								loadmatrix(G.vd->viewmat);
							}
						}
					}
				}
			}
		}
	}

	draw_tface_meshes_tra();

	if(G.zbuf) {
		G.zbuf= FALSE;
		glDisable(GL_DEPTH_TEST);
	}
	
	if(G.vd->drawtype!=OB_TEXTURE) {
		if(G.vd->persp>1) {
			/* persp(0); */
			/* drawviewborder(); */
			/* persp(1); */
		}
	}

		/* life.c */
	draw_gamedebug_info();
	
	curarea->win_swap= WIN_BACK_OK;
}

void drawview3d()
{
	Base *base;
	Object *ob;
	
	setwinmatrixview3d(0);	/* 0= geen pick rect */

	setviewmatrixview3d();

	Mat4MulMat4(G.vd->persmat, G.vd->viewmat, curarea->winmat);
	Mat4Invert(G.vd->persinv, G.vd->persmat);
	Mat4Invert(G.vd->viewinv, G.vd->viewmat);

	if(G.vd->drawtype > OB_WIRE) {
		G.zbuf= TRUE;
		glEnable(GL_DEPTH_TEST);
		if(G.f & G_SIMULATION) {
			glClearColor(0.0, 0.0, 0.0, 0.0); 
		}
		else {
			glClearColor(0.4375, 0.4375, 0.4375, 0.0); 
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glLoadIdentity();
	}
	else {
		glClearColor(0.4375, 0.4375, 0.4375, 0.0); 
		glClear(GL_COLOR_BUFFER_BIT);
	}
	
	loadmatrix(G.vd->viewmat);

	if(G.vd->view==0 || G.vd->persp!=0) {
		drawfloor();
		if(G.vd->persp==2) {
			if(G.scene->world) {
				if(G.scene->world->mode & WO_STARS) make_stars(1);
			}
			calc_viewborder();
			if(G.vd->flag & V3D_DISPBGPIC) draw_bgpic();
		}
	}
	else {
		drawgrid();

		if(G.vd->flag & V3D_DISPBGPIC) {
			draw_bgpic();
		}
	}
	
	/* eerst set tekenen */
	if(G.scene->set) {
	
		/* patchje: kleur blijft constant */ 
		G.f |= G_PICKSEL;
		
		base= G.scene->set->base.first;
		while(base) {
			if(G.vd->lay & base->lay) {
				where_is_object(base->object);

				cpack(0x404040);
				draw_object(base);

				if(base->object->transflag & OB_DUPLI) {
					extern ListBase duplilist;
					Base tbase;
					
					tbase= *base;
					
					tbase.flag= OB_FROMDUPLI;
					make_duplilist(G.scene->set, base->object);
					ob= duplilist.first;
					while(ob) {
						tbase.object= ob;
						draw_object(&tbase);
						ob= ob->id.next;
					}
					free_duplilist();
					
				}
			}
			base= base->next;
		}
		
		G.f &= ~G_PICKSEL;
	}
	
/* SILLY CODE!!!! */
/* See next silly code... why is the same code
 * ~ duplicated twice, and then this silly if(FALSE)
 * in part... wacky! and bad!
 */
	/* eerst niet selected en dupli's */
	base= G.scene->base.first;
	while(base) {
		
		if(G.vd->lay & base->lay) {
		
			where_is_object(base->object);

			if(FALSE && base->object->transflag & OB_DUPLI) {
				extern ListBase duplilist;
				Base tbase;

				/* altijd eerst original tekenen vanwege make_displist */
				draw_object(base);

				/* patchje: kleur blijft constant */ 
				G.f |= G_PICKSEL;
				cpack(0x404040);
				
				tbase.flag= OB_FROMDUPLI;
				make_duplilist(G.scene, base->object);

				ob= duplilist.first;
				while(ob) {
					tbase.object= ob;
					draw_object(&tbase);
					ob= ob->id.next;
				}
				free_duplilist();
				
				G.f &= ~G_PICKSEL;				
			}
			else if((base->flag & SELECT)==0) {
				draw_object(base);
			}
			
		}
		
		base= base->next;
	}
	/*  selected */
	base= G.scene->base.first;
	while(base) {
		
		if TESTBASE(base) {
			draw_object(base);
		}
		
		base= base->next;
	}

/* SILLY CODE!!!! */
	/* dupli's, als laatste om zeker te zijn de displisten zijn ok */
	base= G.scene->base.first;
	while(base) {
		
		if(G.vd->lay & base->lay) {
			if(base->object->transflag & OB_DUPLI) {
				extern ListBase duplilist;
				Base tbase;

				/* patchje: kleur blijft constant */ 
				G.f |= G_PICKSEL;
				cpack(0x404040);
				
				tbase.flag= OB_FROMDUPLI;
				make_duplilist(G.scene, base->object);

				ob= duplilist.first;
				while(ob) {
					tbase.object= ob;
					draw_object(&tbase);
					ob= ob->id.next;
				}
				free_duplilist();
				
				G.f &= ~G_PICKSEL;				
			}
		}
		base= base->next;
	}


	if(G.scene->radio) rad_drawall();	/* radview.c */
	
	persp(0);
	
	if(G.vd->persp>1) drawviewborder();
	drawcursor();
	
	persp(1);

	G.vd->flag &= ~V3D_DISPIMAGE;
	curarea->win_swap= WIN_BACK_OK;
	
	if(G.zbuf) {
		G.zbuf= FALSE;
		glDisable(GL_DEPTH_TEST);
	}
	
	#if defined(__WIN32) || defined(MESA31)

	if(G.f & (G_VERTEXPAINT|G_FACESELECT)) {
		G.vd->flag |= V3D_NEEDBACKBUFDRAW;
		addafterqueue(curarea->win, BACKBUFDRAW, 1);
	}

	#else
	
	G.vd->flag |= V3D_NEEDBACKBUFDRAW;
	addafterqueue(curarea->win, BACKBUFDRAW, 1);
	
	#endif
}

void drawview3d_render()
{
	extern short v3d_windowmode;
	Base *base;
	Object *ob;
	int flag;
	
	winset(R.win);
	
	v3d_windowmode= 1;
	setwinmatrixview3d(0);
	v3d_windowmode= 0;
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(R.winmat);
	glMatrixMode(GL_MODELVIEW);
	
	setviewmatrixview3d();
	glLoadMatrixf(G.vd->viewmat);

	Mat4MulMat4(G.vd->persmat, G.vd->viewmat, R.winmat);
	Mat4Invert(G.vd->persinv, G.vd->persmat);
	Mat4Invert(G.vd->viewinv, G.vd->viewmat);

	if(G.vd->drawtype > OB_WIRE) {
		G.zbuf= TRUE;
		glEnable(GL_DEPTH_TEST);
	}

	if(G.scene->world) glClearColor(G.scene->world->horr, G.scene->world->horg, G.scene->world->horb, 0.0); 
	else glClearColor(0.4375, 0.4375, 0.4375, 0.0); 
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	loadmatrix(G.vd->viewmat);
	
	/* abuse! to make sure it doesnt draw the helpstuff */
	G.f |= G_SIMULATION;

	do_all_ipos();
	do_all_scripts(SCRIPT_FRAMECHANGED);
	do_all_keys();
	do_all_ikas();
	test_all_displists();

	/* niet erg nette calc_ipo en where_is forceer */
	ob= G.main->object.first;
	while(ob) {
		ob->ctime= -123.456;
		ob= ob->id.next;
	}

	/* eerst set tekenen */
	if(G.scene->set) {
	
		/* patchje: kleur blijft constant */ 
		G.f |= G_PICKSEL;
		
		base= G.scene->set->base.first;
		while(base) {
			if(G.vd->lay & base->lay) {
				if ELEM3(base->object->type, OB_LAMP, OB_CAMERA, OB_LATTICE);
				else {
					where_is_object(base->object);
	
					cpack(0x404040);
					draw_object(base);
	
					if(base->object->transflag & OB_DUPLI) {
						extern ListBase duplilist;
						Base tbase;
						
						tbase.flag= OB_FROMDUPLI;
						make_duplilist(G.scene->set, base->object);
						ob= duplilist.first;
						while(ob) {
							tbase.object= ob;
							draw_object(&tbase);
							ob= ob->id.next;
						}
						free_duplilist();
					}
				}
			}
			base= base->next;
		}
		
		G.f &= ~G_PICKSEL;
	}
	
	
	/* eerst niet selected en dupli's */
	base= G.scene->base.first;
	while(base) {
		
		if(G.vd->lay & base->lay) {
			if ELEM3(base->object->type, OB_LAMP, OB_CAMERA, OB_LATTICE);
			else {
				where_is_object(base->object);
	
				if(base->object->transflag & OB_DUPLI) {
					extern ListBase duplilist;
					Base tbase;
					
					/* altijd eerst original tekenen vanwege make_displist */
					draw_object(base);
					
					/* patchje: kleur blijft constant */ 
					G.f |= G_PICKSEL;
					cpack(0x404040);
					
					tbase.flag= OB_FROMDUPLI;
					make_duplilist(G.scene, base->object);
					ob= duplilist.first;
					while(ob) {
						tbase.object= ob;
						draw_object(&tbase);
						ob= ob->id.next;
					}
					free_duplilist();
					
					G.f &= ~G_PICKSEL;				
				}
				else if((base->flag & SELECT)==0) {
					draw_object(base);
				}
			}
		}
		
		base= base->next;
	}
	/*  selected */
	base= G.scene->base.first;
	while(base) {
		
		if TESTBASE(base) {
			if ELEM3(base->object->type, OB_LAMP, OB_CAMERA, OB_LATTICE);
			else draw_object(base);
		}
		
		base= base->next;
	}

	if(G.scene->radio) rad_drawall();	/* radview.c */
	
	if(G.zbuf) {
		G.zbuf= FALSE;
		glDisable(GL_DEPTH_TEST);
	}
	
	G.f &= ~G_SIMULATION;
	
	glReadPixels(0, 0, R.rectx, R.recty, GL_RGBA, GL_UNSIGNED_BYTE, R.rectot);
	ortho2(-0.5, (float)R.winx-0.5, -0.5, (float)R.winy-0.5);
	glLoadIdentity();
}


double tottime = 0.0;

int update_time()
{
	struct tms voidbuf;
	static int ltime;
	int time;

	#ifdef __BeOS
	time= glut_system_time()/10000;
	#else
	time = times(&voidbuf);
	#endif
	
	tottime += (time - ltime) / TIME_INTERVAL;
	ltime = time;
	return (tottime < 0.0);
}

double speed_to_swaptime(int speed)
{
	switch(speed) {
	case 1:
		return 1.0/60.0;
	case 2:
		return 1.0/50.0;
	case 3:
		return 1.0/30.0;
	case 4:
		return 1.0/25.0;
	case 5:
		return 1.0/20.0;
	case 6:
		return 1.0/15.0;
	case 7:
		return 1.0/12.5;
	case 8:
		return 1.0/10.0;
	case 9:
		return 1.0/6.0;
	}
}

double key_to_swaptime(int key)
{
	switch(key) {
	case PAD1:
		G.animspeed= 1;
		tottime= 0;
		return speed_to_swaptime(1);
	case PAD2:
		G.animspeed= 2;
		tottime= 0;
		return speed_to_swaptime(2);
	case PAD3:
		G.animspeed= 3;
		tottime= 0;
		return speed_to_swaptime(3);
	case PAD4:
		G.animspeed= 4;
		tottime= 0;
		return speed_to_swaptime(4);
	case PAD5:
		G.animspeed= 5;
		tottime= 0;
		return speed_to_swaptime(5);
	case PAD6:
		G.animspeed= 6;
		tottime= 0;
		return speed_to_swaptime(6);
	case PAD7:
		G.animspeed= 7;
		tottime= 0;
		return speed_to_swaptime(7);
	case PAD8:
		G.animspeed= 8;
		tottime= 0;
		return speed_to_swaptime(8);
	case PAD9:
		G.animspeed= 9;
		tottime= 0;
		return speed_to_swaptime(9);
	}
	
	return speed_to_swaptime(G.animspeed);
}

int play_anim(int mode)
{
	ScrArea *sa, *oldsa;
	double swaptime;
	int cfra, cfraont;
	ushort event=0;
	short val, mval[2];
	char str[12];
	
	/* patch voor zeer oude scenes */
	if(SFRA==0) SFRA= 1;
	if(EFRA==0) EFRA= 250;
	
	if(SFRA>EFRA) return 0;
	
	update_time();
	tottime= 0.0;
	
	swaptime= speed_to_swaptime(G.animspeed);

	/* waitcursor(1); */
	G.f |= G_PLAYANIM;		/* in sequence.c en view.c wordt dit afgevangen */

	if(G.scene->r.scemode & R_NETWORK) {
		init_render_camera_network();
	}
	
	cfraont= CFRA;
	oldsa= curarea;
	
	while(TRUE) {
		
		if (tottime > 0.0) tottime = 0.0;
		
		for(; CFRA<=EFRA; CFRA++) {
			set_timecursor(CFRA);
			do_all_ipos();
			do_all_scripts(SCRIPT_FRAMECHANGED);
			do_all_keys();
			do_all_ikas();

			test_all_displists();
			
			sa= G.curscreen->areabase.first;
			while(sa) {
				if(sa==oldsa) {
					if(sa->win && sa->windraw) {
						/* hier winget() gebruiken: anders wordt in header getekend */
						if(sa->win != winget()) areawinset(sa->win);
						sa->windraw();
					}
				}
				else if(mode) {
					if ELEM(sa->spacetype, SPACE_VIEW3D, SPACE_SEQ) {
						if(sa->win && sa->windraw) {
							/* hier winget() gebruiken: anders wordt in header getekend */
							if(sa->win != winget()) areawinset(sa->win);
							sa->windraw();
						}
					}
				}
				
				sa= sa->next;	
			}
			
			/* minimaal swaptime laten voorbijgaan */
			tottime -= swaptime;
			while (update_time()) usleep(1);

			screen_swapbuffers();
			
			tottime= 0.0;
			
			while(qtest()) {
				event= extern_qread(&val);
				if(event==ESCKEY) break;
				if(val) {
					swaptime= key_to_swaptime(event);
				}
			}
			if(event==ESCKEY || event==SPACEKEY) break;
		}
		if(event==ESCKEY || event==SPACEKEY || mode==2) break;
		
		CFRA= SFRA;
	}

	if(event==SPACEKEY);
	else CFRA= cfraont;
	
	do_all_ipos();
	do_all_keys();
	do_all_ikas();

	if(oldsa!=curarea) areawinset(oldsa->win);
	
	/* restore all areas */
	sa= G.curscreen->areabase.first;
	while(sa) {
		if( (mode && sa->spacetype==SPACE_VIEW3D) || sa==curarea) addqueue(sa->win, REDRAW, 1);
		
		sa= sa->next;	
	}
	
	/* speed button */
	allqueue(REDRAWBUTSANIM, 0);
	
	/* vooropig */
	do_global_buttons(B_NEWFRAME);

	if(G.scene->r.scemode & R_NETWORK) {
		end_render_camera_network();
		allqueue(REDRAWBUTSALL, 0);		/* efra */
	}
	
	waitcursor(0);
	G.f &= ~G_PLAYANIM;
	
	if (event==ESCKEY || event==SPACEKEY) return 1;
	else return 0;
}

