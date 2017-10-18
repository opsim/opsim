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

/* drawobject.c  		GRAPHICS

 * 
 * jan 95
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "edit.h"
#include "effect.h"
#include "sector.h"

uint rect_desel[16]= {0x707070,0x0,0x0,0x707070,0x407070,0x70cccc,0x407070,0x0,0xaaffff,0xffffff,0x70cccc,0x0,0x70cccc,0xaaffff,0x407070,0x707070};
uint rect_sel[16]= {0x707070,0x0,0x0,0x707070,0x702070,0xcc50cc,0x702070,0x0,0xff80ff,0xffffff,0xcc50cc,0x0,0xcc50cc,0xff80ff,0x702070,0x707070};

uint rectu_desel[16]= {0xff4e4e4e,0xff5c2309,0xff000000,0xff4e4f4d,0xff000000,0xffff9d72,0xffff601c,0xff000000,0xff5d2409,0xffffffff,0xffff9d72,0xff5b2209,0xff4e4e4e,0xff5c2309,0xff010100,0xff4f4f4f};
uint rectu_sel[16]= {0xff4e4e4e,0xff403c00,0xff000000,0xff4e4e4d,0xff000000,0xfffff64c,0xffaaa100,0xff000000,0xff403c00,0xffffffff,0xfffff64c,0xff403c00,0xff4f4f4f,0xff403c00,0xff010100,0xff4e4e4e};

uint rectl_desel[81]= {0x777777,0x777777,0xa9fefe,0xaaffff,0xaaffff,0xaaffff,0xaaffff,0x777777,0x777777,0x777777,0xa9fefe,0xaafefe,0x777777,0x777777,0x777777,0xa9fefe,0xa9fefe,0x777777,0xaaffff,0xa9fefe,0x4e4e4e,0x0,0x124040,0x0,0x4e4e4e,0xaafefe,0xaaffff,0xaaffff,0x777777,0x0,0x227777,0x55cccc,0x227777,0x0,0x777777,0xaaffff,0xaaffff,0x777777,0x124040,0x88ffff,0xffffff,0x55cccc,0x124040,0x777777,0xaaffff,0xaaffff,0x777777,0x0,0x55cccc,0x88ffff,0x227777,0x0,0x777777,0xaaffff,0xaafefe,0xaafefe,0x4f4f4f,0x0,0x124040,0x0,0x4e4e4e,0xa9fefe,0xaaffff,0x777777,0xa9fefe,0xa9fefe,0x777777,0x777777,0x777777,0xa9fefe,0xa9fefe,0x777777,0x777777,0x777777,0xa9fefe,0xa9fefe,0xaaffff,0xaaffff,0xaaffff,0x777777,0x777777};
uint rectl_sel[81]= {0x777777,0x777777,0xffaaff,0xffaaff,0xffaaff,0xffaaff,0xffaaff,0x777777,0x777777,0x777777,0xffaaff,0xffaaff,0x777777,0x777777,0x777777,0xffaaff,0xffaaff,0x777777,0xffaaff,0xffaaff,0x4e4e4e,0x10101,0x402440,0x0,0x4e4e4e,0xffaaff,0xffaaff,0xffaaff,0x777777,0x0,0x774477,0xcc77cc,0x774477,0x0,0x777777,0xffaaff,0xffaaff,0x777777,0x402440,0xffaaff,0xffffff,0xcc77cc,0x412541,0x777777,0xffaaff,0xffaaff,0x777777,0x10101,0xcc77cc,0xffaaff,0x774477,0x0,0x777777,0xffaaff,0xffaaff,0xffaaff,0x4e4e4e,0x10101,0x402440,0x0,0x4e4e4e,0xffaaff,0xffaaff,0x777777,0xffaaff,0xffaaff,0x777777,0x777777,0x777777,0xffaaff,0xffaaff,0x777777,0x777777,0x777777,0xffaaff,0xffaaff,0xffaaff,0xffaaff,0xffaaff,0x777777,0x777777};
uint rectlus_desel[81]= {0x777777,0x777777,0xa9fefe,0xaaffff,0xaaffff,0xaaffff,0xaaffff,0x777777,0x777777,0x777777,0xa9fefe,0xaafefe,0x777777,0x777777,0x777777,0xa9fefe,0xa9fefe,0x777777,0xaaffff,0xa9fefe,0x4e4e4e,0x0,0x5c2309,0x0,0x4e4f4d,0xaafefe,0xaaffff,0xaaffff,0x777777,0x0,0xff601c,0xff9d72,0xff601c,0x0,0x777777,0xaaffff,0xaaffff,0x777777,0x5d2409,0xffceb8,0xff9d72,0xff9d72,0x5b2209,0x777777,0xaaffff,0xaaffff,0x777777,0x10100,0xffceb8,0xffceb8,0xff601c,0x0,0x777777,0xaaffff,0xaafefe,0xaafefe,0x4e4e4e,0x0,0x5c2309,0x10100,0x4f4f4f,0xa9fefe,0xaaffff,0x777777,0xa9fefe,0xa9fefe,0x777777,0x777777,0x777777,0xa9fefe,0xa9fefe,0x777777,0x777777,0x777777,0xa9fefe,0xa9fefe,0xaaffff,0xaaffff,0xaaffff,0x777777,0x777777};
uint rectlus_sel[81]= {0x777777,0x777777,0xffaaff,0xffaaff,0xffaaff,0xffaaff,0xffaaff,0x777777,0x777777,0x777777,0xffaaff,0xffaaff,0x777777,0x777777,0x777777,0xffaaff,0xffaaff,0x777777,0xffaaff,0xffaaff,0x4e4e4e,0x10100,0x403c00,0x0,0x4e4e4d,0xffaaff,0xffaaff,0xffaaff,0x777777,0x0,0xaaa100,0xfff64c,0xaaa100,0x0,0x777777,0xffaaff,0xffaaff,0x777777,0x403c00,0xfffde2,0xffffff,0xfff64c,0x403c00,0x777777,0xffaaff,0xffaaff,0x777777,0x10100,0xfff64c,0xfffde2,0xaaa100,0x0,0x777777,0xffaaff,0xffaaff,0xffaaff,0x4f4f4f,0x0,0x403c00,0x10100,0x4e4e4e,0xffaaff,0xffaaff,0x777777,0xffaaff,0xffaaff,0x777777,0x777777,0x777777,0xffaaff,0xffaaff,0x777777,0x777777,0x777777,0xffaaff,0xffaaff,0xffaaff,0xffaaff,0xffaaff,0x777777,0x777777};
uint rectllib_desel[81]= {0xff777777,0xff777777,0xb9b237,0xb9b237,0xb9b237,0xb9b237,0xb9b237,0xff777777,0xff777777,0xff777777,0xb9b237,0xb9b237,0xff777777,0xff777777,0xff777777,0xb9b237,0xb9b237,0xff777777,0xb9b237,0xb9b237,0x4e4e4e,0x0,0x5c2309,0x0,0x4e4f4d,0xb9b237,0xb9b237,0xb9b237,0xff777777,0x0,0xff601c,0xff9d72,0xff601c,0x0,0xff777777,0xb9b237,0xb9b237,0xff777777,0x5d2409,0xffceb8,0xff9d72,0xff9d72,0x5b2209,0xff777777,0xb9b237,0xb9b237,0xff777777,0x10100,0xffceb8,0xffceb8,0xff601c,0x0,0xff777777,0xb9b237,0xb9b237,0xb9b237,0x4e4e4e,0x0,0x5c2309,0x10100,0x4f4f4f,0xb9b237,0xb9b237,0xff777777,0xb9b237,0xb9b237,0xff777777,0xff777777,0xff777777,0xb9b237,0xb9b237,0xff777777,0xff777777,0xff777777,0xb9b237,0xb9b237,0xb9b237,0xb9b237,0xb9b237,0xff777777,0xff777777};
uint rectllib_sel[81]= {0xff777777,0xff777777,0xfff64c,0xfff64c,0xfff64c,0xfff64c,0xfff64c,0xff777777,0xff777777,0xff777777,0xfff64c,0xfff64c,0xff777777,0xff777777,0xff777777,0xfff64c,0xfff64c,0xff777777,0xfff64c,0xfff64c,0x4e4e4e,0x10100,0x403c00,0x0,0x4e4e4d,0xfff64c,0xfff64c,0xfff64c,0xff777777,0x0,0xaaa100,0xfff64c,0xaaa100,0x0,0xff777777,0xfff64c,0xfff64c,0xff777777,0x403c00,0xfffde2,0xffffff,0xfff64c,0x403c00,0xff777777,0xfff64c,0xfff64c,0xff777777,0x10100,0xfff64c,0xfffde2,0xaaa100,0x0,0xff777777,0xfff64c,0xfff64c,0xfff64c,0x4f4f4f,0x0,0x403c00,0x10100,0x4e4e4e,0xfff64c,0xfff64c,0xff777777,0xfff64c,0xfff64c,0xff777777,0xff777777,0xff777777,0xfff64c,0xfff64c,0xff777777,0xff777777,0xff777777,0xfff64c,0xfff64c,0xfff64c,0xfff64c,0xfff64c,0xff777777,0xff777777};

uint rectl_set[81]= {0xff777777,0xff777777,0xaaaaaa,0xaaaaaa,0xaaaaaa,0xaaaaaa,0xaaaaaa,0xff777777,0xff777777,0xff777777,0xaaaaaa,0xaaaaaa,0xff777777,0xff777777,0xff777777,0xaaaaaa,0xaaaaaa,0xff777777,0xaaaaaa,0xaaaaaa,0x4e4e4e,0x10100,0x202020,0x0,0x4e4e4d,0xaaaaaa,0xaaaaaa,0xaaaaaa,0xff777777,0x0,0xaaa100,0xaaaaaa,0xaaa100,0x0,0xff777777,0xaaaaaa,0xaaaaaa,0xff777777,0x202020,0xfffde2,0xffffff,0xaaaaaa,0x202020,0xff777777,0xaaaaaa,0xaaaaaa,0xff777777,0x10100,0xaaaaaa,0xfffde2,0xaaa100,0x0,0xff777777,0xaaaaaa,0xaaaaaa,0xaaaaaa,0x4f4f4f,0x0,0x202020,0x10100,0x4e4e4e,0xaaaaaa,0xaaaaaa,0xff777777,0xaaaaaa,0xaaaaaa,0xff777777,0xff777777,0xff777777,0xaaaaaa,0xaaaaaa,0xff777777,0xff777777,0xff777777,0xaaaaaa,0xaaaaaa,0xaaaaaa,0xaaaaaa,0xaaaaaa,0xff777777,0xff777777};

#define B_YELLOW	0x77FFFF
#define B_PURPLE	0xFF70FF


uint selcol= 0xFF88FF;
uint actselcol= 0xFFBBFF;

uint colortab[14]=
	{0x0,		0xFF88FF, 0xFFBBFF, 
	 0x403000,	0xFFFF88, 0xFFFFBB, 
	 0x103040,	0x66CCCC, 0x77CCCC, 
	 0xFFFFFF
	};


float cube[8][3] = {
	{-1.0, -1.0, -1.0},
	{-1.0, -1.0,  1.0},
	{-1.0,  1.0,  1.0},
	{-1.0,  1.0, -1.0},
	{ 1.0, -1.0, -1.0},
	{ 1.0, -1.0,  1.0},
	{ 1.0,  1.0,  1.0},
	{ 1.0,  1.0, -1.0},
};

float cube1[8][3] = {
	{-1.2, -1.2, -1.0},
	{-1.0, -1.0,  1.0},
	{-1.0,  1.0,  1.0},
	{-1.2,  1.2, -1.0},
	{ 1.2, -1.2, -1.0},
	{ 1.0, -1.0,  1.0},
	{ 1.0,  1.0,  1.0},
	{ 1.2,  1.2, -1.0},
};

void init_draw_rects()
{
	if(G.order==B_ENDIAN) {
		convert_rgba_to_abgr(16, rect_desel);
		convert_rgba_to_abgr(16, rect_sel);
		
		convert_rgba_to_abgr(16, rectu_desel);
		convert_rgba_to_abgr(16, rectu_sel);
		
		convert_rgba_to_abgr(81, rectl_desel);
		convert_rgba_to_abgr(81, rectl_sel);
	
		convert_rgba_to_abgr(81, rectlus_desel);
		convert_rgba_to_abgr(81, rectlus_sel);
	
		convert_rgba_to_abgr(81, rectllib_desel);
		convert_rgba_to_abgr(81, rectllib_sel);
				
		convert_rgba_to_abgr(81, rectl_set);
	}
}

void tekenrect9(float *vec, uint *rect)
{
	char dummy=0;
	
	if(vec) {
		glRasterPos3f(vec[0], vec[1], vec[2]);
		/* bitmap is used as a patch to move the rasterpos a few pixels */
		glBitmap(1, 1, 0.0, 0.0, -4.5, -4.5, &dummy);
		glDrawPixels(9, 9, GL_RGBA, GL_UNSIGNED_BYTE, rect);
	}
}

void tekenrect4(float *vec, uint *rect)
{
	char dummy=0;
	
	if(vec) {
		glRasterPos3f(vec[0], vec[1], vec[2]);
		/* bitmap is used as a patch to move the rasterpos a few pixels */
		glBitmap(1, 1, 0.0, 0.0, -2.0, -2.0, &dummy);
		glDrawPixels(4, 4, GL_RGBA, GL_UNSIGNED_BYTE, rect);
	}
}

void tekenrect3(float *vec, uint *rect)
{
	char dummy=0;
	
	if(vec) {
		glRasterPos3f(vec[0], vec[1], vec[2]);
		/* bitmap is used as a patch to move the rasterpos a few pixels */
		glBitmap(1, 1, 0.0, 0.0, -1.5, -1.5, &dummy);
		glDrawPixels(3, 3, GL_RGBA, GL_UNSIGNED_BYTE, rect);
	}
}

void helpline(float *vec)
{
	float vecrot[3];
	short mval[2], mval1[2];

	VECCOPY(vecrot, vec);
	if(G.obedit) Mat4MulVecfl(G.obedit->obmat, vecrot);

	getmouseco_areawin(mval);
	project_short_noclip(vecrot, mval1);

	persp(0);
	
	glDrawBuffer(GL_FRONT);
	
	cpack(0);

	setlinestyle(3);
	glBegin(GL_LINE_STRIP); 
		glVertex2sv(mval); 
		glVertex2sv(mval1); 
	glEnd();
	setlinestyle(0);
	
	persp(1);

	glDrawBuffer(GL_BACK);
}

void drawaxes(float size)
{
	float v1[3], v2[3];
	float f800, f125;

	fmsetfont(G.font);

	f125= 0.125*size;
	f800= 0.8*size;
	v1[0]=v1[1]=v1[2]=v2[0]=v2[1]=v2[2]= 0.0;
	
	/* X-AS: pijl */
	v2[0]= size;
	LINE3F(v1, v2);
	v1[0]= f800;
	v1[1]= -f125;
	LINE3F(v1, v2);
	v1[1]= f125;
	LINE3F(v1, v2);
	
	glRasterPos3f(size+f125,  0.0,  0.0);
	fmprstr("x");
	
	/* Y-AS: pijl */
	v1[0]=v1[1]=v1[2]=v2[0]=v2[1]=v2[2]= 0.0;
	v2[1]= size;
	LINE3F(v1, v2);
	v1[1]= f800;
	v1[0]= -f125;
	LINE3F(v1, v2);
	v1[0]= f125;
	LINE3F(v1, v2);
	
	glRasterPos3f(0.0,  size+f125,  0.0);
	fmprstr("y");
	
	/* Z-AS: pijl */
	v1[0]=v1[1]=v1[2]=v2[0]=v2[1]=v2[2]= 0.0;
	v2[2]= size;
	LINE3F(v1, v2);
	v1[2]= f800;
	v1[0]= -f125;
	LINE3F(v1, v2);
	v1[0]= f125;
	LINE3F(v1, v2);

	glRasterPos3f(0.0,  0.0,  size+f125);
	fmprstr("z");
	
}

void drawgourcube()
{
	float vec[3];
	float n[3];

	n[0]=0; n[1]=0; n[2]=0;
	glBegin(GL_POLYGON);
		n[0]= -1.0;
		glNormal3fv(n); 
		glVertex3fv(cube[0]); glVertex3fv(cube[1]); glVertex3fv(cube[2]); glVertex3fv(cube[3]);
		n[0]=0;
	glEnd();

	glBegin(GL_POLYGON);
		n[1]= -1.0;
		glNormal3fv(n); 
		glVertex3fv(cube[0]); glVertex3fv(cube[4]); glVertex3fv(cube[5]); glVertex3fv(cube[1]);
		n[1]=0;
	glEnd();

	glBegin(GL_POLYGON);
		n[0]= 1.0;
		glNormal3fv(n); 
		glVertex3fv(cube[4]); glVertex3fv(cube[7]); glVertex3fv(cube[6]); glVertex3fv(cube[5]);
		n[0]=0;
	glEnd();

	glBegin(GL_POLYGON);
		n[1]= 1.0;
		glNormal3fv(n); 
		glVertex3fv(cube[7]); glVertex3fv(cube[3]); glVertex3fv(cube[2]); glVertex3fv(cube[6]);
		n[1]=0;
	glEnd();

	glBegin(GL_POLYGON);
		n[2]= 1.0;
		glNormal3fv(n); 
		glVertex3fv(cube[1]); glVertex3fv(cube[5]); glVertex3fv(cube[6]); glVertex3fv(cube[2]);
		n[2]=0;
	glEnd();

	glBegin(GL_POLYGON);
		n[2]= -1.0;
		glNormal3fv(n); 
		glVertex3fv(cube[7]); glVertex3fv(cube[4]); glVertex3fv(cube[0]); glVertex3fv(cube[3]);
	glEnd();
}

void drawcube()
{

	glBegin(GL_LINE_STRIP);
		glVertex3fv(cube[0]); glVertex3fv(cube[1]);glVertex3fv(cube[2]); glVertex3fv(cube[3]);
		glVertex3fv(cube[0]); glVertex3fv(cube[4]);glVertex3fv(cube[5]); glVertex3fv(cube[6]);
		glVertex3fv(cube[7]); glVertex3fv(cube[4]);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3fv(cube[1]); glVertex3fv(cube[5]);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3fv(cube[2]); glVertex3fv(cube[6]);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3fv(cube[3]); glVertex3fv(cube[7]);
	glEnd();
}

void drawcube_size(float *size)
{

	glPushMatrix();
	glScalef(size[0],  size[1],  size[2]);
	

	glBegin(GL_LINE_STRIP);
		glVertex3fv(cube[0]); glVertex3fv(cube[1]);glVertex3fv(cube[2]); glVertex3fv(cube[3]);
		glVertex3fv(cube[0]); glVertex3fv(cube[4]);glVertex3fv(cube[5]); glVertex3fv(cube[6]);
		glVertex3fv(cube[7]); glVertex3fv(cube[4]);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3fv(cube[1]); glVertex3fv(cube[5]);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3fv(cube[2]); glVertex3fv(cube[6]);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3fv(cube[3]); glVertex3fv(cube[7]);
	glEnd();
	
	glPopMatrix();
}


void tekenshadbuflimits(Lamp *la, float mat[][4])
{
	float sta[3], end[3], lavec[3];
	short s[2];

	lavec[0]= -mat[2][0];
	lavec[1]= -mat[2][1];
	lavec[2]= -mat[2][2];
	Normalise(lavec);

	sta[0]= mat[3][0]+ la->clipsta*lavec[0];
	sta[1]= mat[3][1]+ la->clipsta*lavec[1];
	sta[2]= mat[3][2]+ la->clipsta*lavec[2];

	end[0]= mat[3][0]+ la->clipend*lavec[0];
	end[1]= mat[3][1]+ la->clipend*lavec[1];
	end[2]= mat[3][2]+ la->clipend*lavec[2];


	glBegin(GL_LINE_STRIP);
		glVertex3fv(sta);
		glVertex3fv(end);
	glEnd();

	glPointSize(3.0);
	glBegin(GL_POINTS);
	cpack(0);
	glVertex3fv(sta);
	glVertex3fv(end);
	glEnd();
	glPointSize(1.0);
}



void spotvolume(lvec,vvec,inp)
float *lvec,*vvec,inp;
{
	/* camera staat op 0,0,0 */
	float temp[3],plane[3],mat1[3][3],mat2[3][3],mat3[3][3],mat4[3][3],q[4],co,si,hoek,*rt;

	Normalise(lvec);
	Normalise(vvec);				/* is dit de goede vector ? */

	Crossf(temp,vvec,lvec);		/* vergelijking van vlak door vvec en lvec */
	Crossf(plane,lvec,temp);		/* en dan het vlak loodrecht daarop en evenwijdig aan lvec */

	Normalise(plane);

	/* nu hebben we twee vergelijkingen: die van de kegel en die van het vlak, maar we hebben
	drie onbekenden We halen nu een onbekende weg door het vlak naar z=0 te roteren */
	/* Ik heb geen flauw idee of dat mag, we moeten tenslotte twee oplossingen krijgen, maar we
	proberen het gewoon: vlak vector moet (0,0,1) worden*/

	/* roteer om uitproduct vector van (0,0,1) en vlakvector, inproduct graden */
	/* volgens defenitie volgt dat uitproduct is (plane[1],-plane[0],0), en cos() = plane[2]);*/

	q[1] = plane[1] ; 
	q[2] = -plane[0] ; 
	q[3] = 0 ;
	Normalise(&q[1]);

	hoek = safacos(plane[2])/2.0;
	co = fcos(hoek);
	si = fsqrt(1-co*co);

	q[0] =  co;
	q[1] *= si;
	q[2] *= si;
	q[3] =  0;

	QuatToMat3(q,mat1);

	/* lampvector nu over acos(inp) graden roteren */

	vvec[0] = lvec[0] ; 
	vvec[1] = lvec[1] ; 
	vvec[2] = lvec[2] ;

	Mat3One(mat2);
	co = inp;
	si = fsqrt(1-inp*inp);

	mat2[0][0] =  co;
	mat2[1][0] = -si;
	mat2[0][1] =  si;
	mat2[1][1] =  co;
	Mat3MulMat3(mat3,mat2,mat1);

	mat2[1][0] =  si;
	mat2[0][1] = -si;
	Mat3MulMat3(mat4,mat2,mat1);
	Mat3Transp(mat1);

	Mat3MulMat3(mat2,mat1,mat3);
	Mat3MulVecfl(mat2,lvec);
	Mat3MulMat3(mat2,mat1,mat4);
	Mat3MulVecfl(mat2,vvec);

	return;
}



void drawlamp(Object *ob)
{
	Lamp *la;
	float vec[3], lvec[3], vvec[3],x,y,z;
	
	la= ob->data;
	vec[0]=vec[1]=vec[2]= 0.0;
	
	setlinestyle(4);
	
	if(la->type==LA_SPOT) {
		
		lvec[0]=lvec[1]= 0.0; 
		lvec[2] = 1.0;
		x = G.vd->persmat[0][2];
		y = G.vd->persmat[1][2];
		z = G.vd->persmat[2][2];
		vvec[0]= x*ob->obmat[0][0] + y*ob->obmat[0][1] + z*ob->obmat[0][2];
		vvec[1]= x*ob->obmat[1][0] + y*ob->obmat[1][1] + z*ob->obmat[1][2];
		vvec[2]= x*ob->obmat[2][0] + y*ob->obmat[2][1] + z*ob->obmat[2][2];

		y = fcos( M_PI*la->spotsize/360.0 );
		spotvolume(lvec, vvec, y);
		x = -la->dist;
		lvec[0] *=  x ; 
		lvec[1] *=  x ; 
		lvec[2] *=  x;
		vvec[0] *= x ; 
		vvec[1] *= x ; 
		vvec[2] *= x;

		glBegin(GL_LINE_STRIP);
			glVertex3fv(vvec);
			glVertex3fv(vec);
			glVertex3fv(lvec);
		glEnd();
		
		z = x*fsqrt(1.0 - y*y);
		x *= y;

		glTranslatef(0.0 ,  0.0 ,  x);
		if(la->mode & LA_SQUARE) {
			vvec[0]= fabs(z);
			vvec[1]= fabs(z);
			vvec[2]= 0.0;
			glBegin(GL_LINE_LOOP);
				glVertex3fv(vvec);
				vvec[1]= -fabs(z);
				glVertex3fv(vvec);
				vvec[0]= -fabs(z);
				glVertex3fv(vvec);
				vvec[1]= fabs(z);
				glVertex3fv(vvec);
			glEnd();
		}
		else circ(0.0, 0.0, fabs(z));
		
	}
	else if ELEM(la->type, LA_HEMI, LA_SUN) {
		glBegin(GL_LINE_STRIP);
			glVertex3fv(vec); 
			vec[2]= -la->dist; 
			glVertex3fv(vec);
		glEnd();
	}
	else {
		if(la->mode & LA_SPHERE) {

			float tmat[4][4], imat[4][4];
			
			vec[0]= vec[1]= vec[2]= 0.0;
			getmatrix(tmat);
			Mat4Invert(imat, tmat);
			
			drawcircball(vec, la->dist, imat);

		}
	}
	loadmatrix(G.vd->viewmat);
	
	VECCOPY(vec, ob->obmat[3]);
	
	glBegin(GL_LINE_STRIP);
		glVertex3fv(vec); 
		vec[2]= 0; 
		glVertex3fv(vec);
	glEnd();
	setlinestyle(0);
		
	if(la->type==LA_SPOT && (la->mode & LA_SHAD) ) {
		tekenshadbuflimits(la, ob->obmat);
	}
}

void draw_limit_line(float sta, float end, uint col)
{
	float vec[2][3];
	short mval[2];
	
	vec[0][0]= 0.0;
	vec[0][1]= 0.0;
	vec[0][2]= -sta;

	vec[1][0]= 0.0;
	vec[1][1]= 0.0;
	vec[1][2]= -end;
	
	LINE3F(vec[0], vec[1]);

	glPointSize(3.0);
	glBegin(GL_POINTS);
	cpack(col);
	glVertex3fv(vec[0]);
	glVertex3fv(vec[1]);
	glEnd();
	glPointSize(1.0);
}		


void drawcamera(Object *ob)
{
	/* een staande piramide met (0,0,0) als top */
	Camera *cam;
	World *wrld;
	float vec[8][4], tmat[4][4], fac, facx, facy, depth;
	short mval[2];

	cam= ob->data;
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	
	/* zo is ie altijd te zien */
	fac= cam->drawsize;
	if(G.vd->persp>=2) fac= cam->clipsta+0.1;
	
	depth= - fac*cam->lens/16.0;
	facx= fac*1.28;
	facy= fac*1.024;
	
	vec[0][0]= 0; vec[0][1]= 0; vec[0][2]= 0.001;	/* GLBUG: z niet op nul vanwege picking op entry */
	vec[1][0]= facx; vec[1][1]= facy; vec[1][2]= depth;
	vec[2][0]= facx; vec[2][1]= -facy; vec[2][2]= depth;
	vec[3][0]= -facx; vec[3][1]= -facy; vec[3][2]= depth;
	vec[4][0]= -facx; vec[4][1]= facy; vec[4][2]= depth;

	glBegin(GL_LINE_LOOP);
		glVertex3fv(vec[0]); 
		glVertex3fv(vec[1]); 
		glVertex3fv(vec[2]); 
		glVertex3fv(vec[0]); 
		glVertex3fv(vec[3]); 
		glVertex3fv(vec[4]);
	glEnd();

	glBegin(GL_LINES);
		glVertex3fv(vec[2]); 
		glVertex3fv(vec[3]);
	glEnd();

	glBegin(GL_LINES);
		glVertex3fv(vec[4]); 
		glVertex3fv(vec[1]);
	glEnd();

	if(G.vd->persp>=2) return;
	if(G.f & G_BACKBUFSEL) return;
	
	/* pijl aan top */
	vec[0][2]= depth;

	glBegin(GL_POLYGON);

		vec[0][0]= -0.2*cam->drawsize; 
		vec[0][1]= cam->drawsize;
		glVertex3fv(vec[0]);
		
		vec[0][0]= 0.2*cam->drawsize;
		glVertex3fv(vec[0]);
		
		vec[0][1]= 1.6*cam->drawsize;
		glVertex3fv(vec[0]);
		
		vec[0][0]= -0.2*cam->drawsize; 
		glVertex3fv(vec[0]);
	glEnd();

	glBegin(GL_POLYGON);
	
		vec[0][0]= -0.4*cam->drawsize;
		vec[0][1]= 1.6*cam->drawsize;
		glVertex3fv(vec[0]);
		
		vec[0][0]= 0.0; 
		vec[0][1]= 2.0*cam->drawsize;
		glVertex3fv(vec[0]);
		
		vec[0][0]= 0.4*cam->drawsize; 
		vec[0][1]= 1.6*cam->drawsize;
		glVertex3fv(vec[0]);
	
	glEnd();
	
	if(cam->flag & (CAM_SHOWLIMITS+CAM_SHOWMIST+CAM_SHOWNETCLIP)) {
		loadmatrix(G.vd->viewmat);
		Mat4CpyMat4(vec, ob->obmat);
		Mat4Ortho(vec);
		multmatrix(vec);

		Mat4SwapMat4(G.vd->persmat, tmat);
		mygetsingmatrix(G.vd->persmat);

		if(cam->flag & CAM_SHOWLIMITS) 
			draw_limit_line(cam->clipsta, cam->clipend, B_YELLOW);

		if(cam->flag & CAM_SHOWNETCLIP) 
			draw_limit_line(cam->netsta, cam->netend, 0);
		
		wrld= G.scene->world;
		if(cam->flag & CAM_SHOWMIST) 
			if(wrld) draw_limit_line(wrld->miststa, wrld->miststa+wrld->mistdist, 0xFFFFFF);
			
		Mat4SwapMat4(G.vd->persmat, tmat);
	}
}

void tekenvertslatt(short sel)
{
	Lattice *lt;
	BPoint *bp;
	uint *rect;
	int a, uxt, u, vxt, v, wxt, w;

	glPointSize(3.0);

	if(sel) cpack(B_YELLOW);
	else cpack(B_PURPLE);

	glBegin(GL_POINTS);

	bp= editLatt->def;
	lt= editLatt;
	
	if(lt->flag & LT_OUTSIDE) {
		
		for(w=0; w<lt->pntsw; w++) {
			if(w==0 || w==lt->pntsw-1) wxt= 1; else wxt= 0;
			for(v=0; v<lt->pntsv; v++) {
				if(v==0 || v==lt->pntsv-1) vxt= 1; else vxt= 0;
				
				for(u=0; u<lt->pntsu; u++, bp++) {
					if(u==0 || u==lt->pntsu-1) uxt= 1; else uxt= 0;
					if(uxt || vxt || wxt) {
						if(bp->hide==0) {
							if((bp->f1 & 1)==sel) glVertex3fv(bp->vec);
						}
					}
				}
			}
		}
	}
	else {

		a= editLatt->pntsu*editLatt->pntsv*editLatt->pntsw;
		while(a--) {
			if(bp->hide==0) {
				if((bp->f1 & 1)==sel) glVertex3fv(bp->vec);
			}
			bp++;
		}
	}
	
	glPointSize(1.0);
	glEnd();	
}

void calc_lattverts()
{
	BPoint *bp;
	float mat[4][4];
	int a, tot, b;
	short *sp;

	Mat4SwapMat4(G.vd->persmat, mat);
	mygetsingmatrix(G.vd->persmat);
	
	 bp= editLatt->def;
	
	a= editLatt->pntsu*editLatt->pntsv*editLatt->pntsw;
	while(a--) {
		project_short(bp->vec, bp->s);
		bp++;
	}

	Mat4SwapMat4(G.vd->persmat, mat);
}


void calc_lattverts_ext()
{

	areawinset(curarea->win);
	multmatrix(G.obedit->obmat);
	calc_lattverts();
	loadmatrix(G.vd->viewmat);
	
}


void drawlattice(Object *ob)
{
	Lattice *lt;
	BPoint *bp, *bpu;
	int u, v, w, dv, dw, uxt, vxt, wxt;

	lt= ob->data;
	if(ob==G.obedit) {
		bp= editLatt->def;
		
		cpack(0x004000);
	}
	else {
		bp= lt->def;
	}
	
	dv= lt->pntsu;
	dw= dv*lt->pntsv;
	
	if(lt->flag & LT_OUTSIDE) {
		
		for(w=0; w<lt->pntsw; w++) {
			
			if(w==0 || w==lt->pntsw-1) wxt= 1; else wxt= 0;
			
			for(v=0; v<lt->pntsv; v++) {
				
				if(v==0 || v==lt->pntsv-1) vxt= 1; else vxt= 0;
				
				for(u=0, bpu=0; u<lt->pntsu; u++, bp++) {
				
					if(u==0 || u==lt->pntsu-1) uxt= 1; else uxt= 0;
					
					if(uxt || vxt || wxt) {
					
						if(w && (uxt || vxt)) {

							glBegin(GL_LINE_STRIP);
							glVertex3fv( (bp-dw)->vec ); glVertex3fv(bp->vec);
							glEnd();
						}
						if(v && (uxt || wxt)) {

							glBegin(GL_LINES);
							glVertex3fv( (bp-dv)->vec ); glVertex3fv(bp->vec);
							glEnd();
						}
						if(u && (vxt || wxt)) {

							glBegin(GL_LINES);
							glVertex3fv(bpu->vec); glVertex3fv(bp->vec);
							glEnd();
						}
					}
					
					bpu= bp;
				}
			}
		}		
	}
	else {
		for(w=0; w<lt->pntsw; w++) {
			
			for(v=0; v<lt->pntsv; v++) {
				
				for(u=0, bpu=0; u<lt->pntsu; u++, bp++) {
				
					if(w) {

						glBegin(GL_LINES);
						glVertex3fv( (bp-dw)->vec ); glVertex3fv(bp->vec);
						glEnd();
					}
					if(v) {

						glBegin(GL_LINES);
						glVertex3fv( (bp-dv)->vec ); glVertex3fv(bp->vec);
						glEnd();
					}
					if(u) {

						glBegin(GL_LINES);
						glVertex3fv(bpu->vec); glVertex3fv(bp->vec);
						glEnd();
					}
					bpu= bp;
				}
			}
		}
	}
	
	if(ob==G.obedit) {
		
		calc_lattverts();
		
		if(G.zbuf) glDisable(GL_DEPTH_TEST);
		
		tekenvertslatt(0);
		tekenvertslatt(1);
		
		if(G.zbuf) glEnable(GL_DEPTH_TEST); 
	}
}

/* ***************** ******************** */

void calc_meshverts()
{
	EditVert *eve;
	float mat[4][4];

	if(G.edve.first==0) return;
	eve= G.edve.first;

	Mat4SwapMat4(G.vd->persmat, mat);
	mygetsingmatrix(G.vd->persmat);

	eve= G.edve.first;
	while(eve) {
		if(eve->h==0) {
			project_short(eve->co, &(eve->xs));
		}
		eve= eve->next;
	}
	Mat4SwapMat4(G.vd->persmat, mat);
}

void calc_meshverts_ext()
{

	areawinset(curarea->win);
	multmatrix(G.obedit->obmat);
	calc_meshverts();
	loadmatrix(G.vd->viewmat);
	
}

void calc_Nurbverts(Nurb *nurb)
{
	Nurb *nu;
	BezTriple *bezt;
	BPoint *bp;
	float mat[4][4];
	int a, tot, b;
	short *sp;

	Mat4SwapMat4(G.vd->persmat, mat);
	mygetsingmatrix(G.vd->persmat);

	nu= nurb;
	while(nu) {
		if((nu->type & 7)==1) {
			bezt= nu->bezt;
			a= nu->pntsu;
			while(a--) {
				project_short(bezt->vec[0], bezt->s[0]);
				project_short(bezt->vec[1], bezt->s[1]);
				project_short(bezt->vec[2], bezt->s[2]);
				bezt++;
			}
		}
		else {
			bp= nu->bp;
			a= nu->pntsu*nu->pntsv;
			while(a--) {
				project_short(bp->vec, bp->s);
				bp++;
			}
		}
		nu= nu->next;
	}

	Mat4SwapMat4(G.vd->persmat, mat);
}

void calc_nurbverts_ext()
{

	areawinset(curarea->win);
	multmatrix(G.obedit->obmat);
	calc_Nurbverts(editNurb.first);
	loadmatrix(G.vd->viewmat);
	
}

void tekenvertices(short sel)
{
	EditVert *eve;
	short xs,ys;

	glPointSize(2.0);
	
	if(sel) cpack(B_YELLOW);
	else cpack(B_PURPLE);
	
	glBegin(GL_POINTS);

	eve= (EditVert *)G.edve.first;
	while(eve) {
		if(eve->h==0 && (eve->f & 1)==sel ) {
			glVertex3fv(eve->co);
		}
		eve= eve->next;
	}
	glEnd();

	glPointSize(1.0);
}

void tekenvertices_ext(int mode)
{
	ScrArea *tempsa, *sa;
	View3D *vd;
	
	if(G.f & (G_FACESELECT+G_DRAWFACES)) {
		allqueue(REDRAWVIEW3D, 0);
		return;
	}
	
	if(G.zbuf) glDisable(GL_DEPTH_TEST);
	
	glDrawBuffer(GL_FRONT);

	/* alle views aflopen */
	tempsa= curarea;
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->spacetype==SPACE_VIEW3D) {
			vd= sa->spacedata.first;
			if(G.obedit->lay & vd->lay) {
				areawinset(sa->win);
				multmatrix(G.obedit->obmat);

				calc_meshverts();
				if(mode==0 || mode==2) tekenvertices(0);
				if(mode==1 || mode==2) tekenvertices(1);
				sa->win_swap= WIN_FRONT_OK;
				
				loadmatrix(G.vd->viewmat);
			}
		}
		sa= sa->next;
	}
	if(curarea!=tempsa) areawinset(tempsa->win);
	
	glDrawBuffer(GL_BACK);
	if(G.zbuf) glEnable(GL_DEPTH_TEST);
}

/* ************** DRAW DISPLIST ****************** */

int draw_index_wire= 1;
int index3_nors_incr= 1;

void drawDispListwire(ListBase *dlbase)
{
	DispList *dl;
	int parts, nr, ofs, p1, p2, p3, p4, a, b, *index;
	float *data, *v1, *v2, *v3, *v4, side;

	if(dlbase==0) return;

	dl= dlbase->first;
	while(dl) {
		data= dl->verts;
	
		switch(dl->type) {
		case DL_SEGM:
			parts= dl->parts;
			while(parts--) {
				nr= dl->nr;
				glBegin(GL_LINE_STRIP);
				while(nr--) {
					glVertex3fv(data);
					data+=3;
				}
				glEnd();
			}
			break;
		case DL_POLY:
			parts= dl->parts;
			while(parts--) {
				nr= dl->nr;
				glBegin(GL_LINE_LOOP);
				while(nr--) {
					glVertex3fv(data);
					data+=3;
				}
				glEnd();
			}
			break;
		case DL_SURF:
			parts= dl->parts;
			while(parts--) {
				nr= dl->nr;
				if(dl->flag & 1) glBegin(GL_LINE_LOOP);
				else glBegin(GL_LINE_STRIP);

				while(nr--) {
					glVertex3fv(data);
					data+=3;
				}
				if(dl->flag & 1) glEnd();
				else glEnd();
			}
			ofs= 3*dl->nr;
			nr= dl->nr;
			while(nr--) {
				data= (  dl->verts )+3*nr;
				parts= dl->parts;
				if(dl->flag & 2) glBegin(GL_LINE_LOOP);

				else glBegin(GL_LINE_STRIP);
				while(parts--) {
					glVertex3fv(data);
					data+=ofs;
				}
				if(dl->flag & 2) glEnd();
				else glEnd();
			}
			break;
			
		case DL_INDEX3:
			if(draw_index_wire) {
				parts= dl->parts;
				data= dl->verts;
				index= dl->index;
				while(parts--) {

					glBegin(GL_LINE_STRIP);
						glVertex3fv(data+3*index[0]);
						glVertex3fv(data+3*index[1]);
						glVertex3fv(data+3*index[2]);
					glEnd();
					index+= 3;
				}
			}
			break;
			
		case DL_INDEX4:
			if(draw_index_wire) {
				parts= dl->parts;
				data= dl->verts;
				index= dl->index;
				while(parts--) {

					glBegin(GL_LINE_STRIP);
						glVertex3fv(data+3*index[0]);
						glVertex3fv(data+3*index[1]);
						glVertex3fv(data+3*index[2]);
						if(index[3]) glVertex3fv(data+3*index[3]);
					glEnd();
					index+= 4;
				}
			}
			break;
			
		}
		dl= dl->next;
	}
}

void drawDispListsolid(ListBase *lb, Object *ob)
{
	DispList *dl;
	int parts, nr, ofs, p1, p2, p3, p4, a, b, *index;
	float *data, *v1, *v2, *v3, *v4, side;
	float *ndata, *n1, *n2, *n3, *n4;
	short col[3];
	char *cp;

	if(lb==0) return;
	if( (G.f & G_BACKBUFSEL)==0 ) { 
		glShadeModel(GL_SMOOTH);
		glEnable(GL_LIGHTING);
	}
	
	dl= lb->first;
	while(dl) {
		data= dl->verts;
		ndata= dl->nors;

		switch(dl->type) {
		case DL_SURF:
			if(G.f & G_BACKBUFSEL) {
				for(a=0; a<dl->parts; a++) {
	
					DL_SURFINDEX(dl->flag & 1, dl->flag & 2, dl->nr, dl->parts);
	
					v1= data+ 3*p1; 
					v2= data+ 3*p2;
					v3= data+ 3*p3; 
					v4= data+ 3*p4;
	
					glBegin(GL_QUAD_STRIP);
					
					glVertex3fv(v2);
					glVertex3fv(v4);

					for(; b<dl->nr; b++) {
						
						glVertex3fv(v1);
						glVertex3fv(v3);

						v2= v1; v1+= 3;
						v4= v3; v3+= 3;
					}
					
					
					glEnd();
				}
			}
			else {

				set_gl_material(dl->col+1);
/*				
				glBegin(GL_LINES);
				for(a=0; a<dl->parts*dl->nr; a++) {
					float nor[3];
					
					VECCOPY(nor, data+3*a);
					glVertex3fv(nor);
					VecAddf(nor, nor, ndata+3*a);
					glVertex3fv(nor);
				}
				glEnd();
*/				
				for(a=0; a<dl->parts; a++) {
					
					DL_SURFINDEX(dl->flag & 1, dl->flag & 2, dl->nr, dl->parts);
	
					v1= data+ 3*p1; 
					v2= data+ 3*p2;
					v3= data+ 3*p3; 
					v4= data+ 3*p4;
					n1= ndata+ 3*p1; 
					n2= ndata+ 3*p2;
					n3= ndata+ 3*p3; 
					n4= ndata+ 3*p4;
					
					glBegin(GL_QUAD_STRIP);
					
					glNormal3fv(n2); glVertex3fv(v2);
					glNormal3fv(n4); glVertex3fv(v4);

					for(; b<dl->nr; b++) {
						
						glNormal3fv(n1); glVertex3fv(v1);
						glNormal3fv(n3); glVertex3fv(v3);

						v2= v1; v1+= 3;
						v4= v3; v3+= 3;
						n2= n1; n1+= 3;
						n4= n3; n3+= 3;
					}
					
					
					glEnd();
				}
			}
			break;

		case DL_INDEX3:
		
			parts= dl->parts;
			data= dl->verts;
			ndata= dl->nors;
			index= dl->index;
			
			if(G.f & G_BACKBUFSEL) {
				while(parts--) {

					glBegin(GL_POLYGON);
						glVertex3fv(data+3*index[0]);
						glVertex3fv(data+3*index[1]);
						glVertex3fv(data+3*index[2]);
					glEnd();
					index+= 3;
				}
			}
			else {

				set_gl_material(dl->col+1);
								
				/* voor poly's is er maar 1 normaal nodig */
				if(index3_nors_incr==0) {
					while(parts--) {

						glBegin(GL_POLYGON);
							glNormal3fv(ndata);
							glVertex3fv(data+3*index[0]);
							glVertex3fv(data+3*index[1]);
							glVertex3fv(data+3*index[2]);
						glEnd();
						index+= 3;
					}
				}
				else {
					while(parts--) {

						glBegin(GL_POLYGON);
							ofs= 3*index[0];
							glNormal3fv(ndata+ofs); glVertex3fv(data+ofs);
							ofs= 3*index[1];
							glNormal3fv(ndata+ofs); glVertex3fv(data+ofs);
							ofs= 3*index[2];
							glNormal3fv(ndata+ofs); glVertex3fv(data+ofs);
						glEnd();
						index+= 3;
					}
				}
			}
			break;

		case DL_INDEX4:

			parts= dl->parts;
			data= dl->verts;
			ndata= dl->nors;
			index= dl->index;

			if(G.f & G_BACKBUFSEL) {
				while(parts--) {

					glBegin(GL_POLYGON);
						glVertex3fv(data+3*index[0]);
						glVertex3fv(data+3*index[1]);
						glVertex3fv(data+3*index[2]);
						if(index[3]) glVertex3fv(data+3*index[3]);
					glEnd();
					index+= 4;
				}
			}
			else {
				
				set_gl_material(dl->col+1);
			
				while(parts--) {

					glBegin(GL_POLYGON);
						ofs= 3*index[0];
						glNormal3fv(ndata+ofs); glVertex3fv(data+ofs);
						ofs= 3*index[1];
						glNormal3fv(ndata+ofs); glVertex3fv(data+ofs);
						ofs= 3*index[2];
						glNormal3fv(ndata+ofs); glVertex3fv(data+ofs);
						if(index[3]) {
							ofs= 3*index[3];
							glNormal3fv(ndata+ofs); glVertex3fv(data+ofs);
						}
					glEnd();
					index+= 4;
				}
			}
			break;
			
		}
		dl= dl->next;
	}
	if(G.f & G_BACKBUFSEL);
	else {
		glShadeModel(GL_FLAT);
		glDisable(GL_LIGHTING);
	}
}

void drawDispListshaded(ListBase *lb, Object *ob)
{
	DispList *dl, *dlob;
	int parts, nr, ofs, p1, p2, p3, p4, a, b, *index;
	float *data, *v1, *v2, *v3, *v4, side, *extverts=0;
	uint *cdata, *c1, *c2, *c3, *c4;
	short col[3];
	char *cp;

	if(lb==0) return;

	glShadeModel(GL_SMOOTH);

	dl= lb->first;
	dlob= ob->disp.first;
	while(dl && dlob) {
		
		cdata= dlob->col1;
		data= dl->verts;
		if(cdata==0) break;
		
		switch(dl->type) {
		case DL_SURF:

			for(a=0; a<dl->parts; a++) {

				DL_SURFINDEX(dl->flag & 1, dl->flag & 2, dl->nr, dl->parts);

				v1= data+ 3*p1; 
				v2= data+ 3*p2;
				v3= data+ 3*p3; 
				v4= data+ 3*p4;
				c1= cdata+ p1; 
				c2= cdata+ p2;
				c3= cdata+ p3; 
				c4= cdata+ p4;

				for(; b<dl->nr; b++) {

					glBegin(GL_POLYGON);
						cp= (char *)c1;
						glColor3ub(cp[3], cp[2], cp[1]);
						glVertex3fv(v1);
						cp= (char *)c2;
						glColor3ub(cp[3], cp[2], cp[1]);
						glVertex3fv(v2);
						cp= (char *)c4;
						glColor3ub(cp[3], cp[2], cp[1]);
						glVertex3fv(v4);
						cp= (char *)c3;
						glColor3ub(cp[3], cp[2], cp[1]);
						glVertex3fv(v3);
					glEnd();

					v2= v1; v1+= 3;
					v4= v3; v3+= 3;
					c2= c1; c1++;
					c4= c3; c3++;
				}
			}
			break;

		case DL_INDEX3:
			
			parts= dl->parts;
			index= dl->index;
			
			while(parts--) {

				glBegin(GL_POLYGON);
					cp= (char *)(cdata+index[0]);
					glColor3ub(cp[3], cp[2], cp[1]);					
					glVertex3fv(data+3*index[0]);

					cp= (char *)(cdata+index[1]);
					glColor3ub(cp[3], cp[2], cp[1]);					
					glVertex3fv(data+3*index[1]);

					cp= (char *)(cdata+index[2]);
					glColor3ub(cp[3], cp[2], cp[1]);					
					glVertex3fv(data+3*index[2]);
				glEnd();
				index+= 3;
			}
			break;

		case DL_INDEX4:
		
			parts= dl->parts;
			index= dl->index;
			while(parts--) {

				glBegin(GL_POLYGON);
					cp= (char *)(cdata+index[0]);
					glColor3ub(cp[3], cp[2], cp[1]);					
					glVertex3fv(data+3*index[0]);

					cp= (char *)(cdata+index[1]);
					glColor3ub(cp[3], cp[2], cp[1]);					
					glVertex3fv(data+3*index[1]);

					cp= (char *)(cdata+index[2]);
					glColor3ub(cp[3], cp[2], cp[1]);					
					glVertex3fv(data+3*index[2]);
					
					if(index[3]) {
					
						cp= (char *)(cdata+index[3]);
						glColor3ub(cp[3], cp[2], cp[1]);	
						glVertex3fv(data+3*index[3]);
					}
				glEnd();
				index+= 4;
			}
			break;
			
		}
		dl= dl->next;
		dlob= dlob->next;
	}
	
	glShadeModel(GL_FLAT);
}


void drawmeshsolid(Object *ob, float *nors)
{
	Mesh *me;
	DispList *dl;
	MVert *mvert;
	TFace *tface;
	MFace *mface;
	EditVlak *evl;
	float *extverts=0, *v1, *v2, *v3, *v4;
	int glmode, setsmooth=0, a, start, end, matnr= -1, vertexpaint, i;
	short no[3], *n1, *n2, *n3, *n4;
	
	vertexpaint= (G.f & (G_VERTEXPAINT+G_FACESELECT)) && (ob==OBACT);
	
	me= get_mesh(ob);
	if(me==0) return;

	glShadeModel(GL_FLAT);

	if( (G.f & G_BACKBUFSEL)==0 ) {
		glEnable(GL_LIGHTING);
		init_gl_materials(ob);
		two_sided( me->flag & ME_TWOSIDED );
	}

	mface= me->mface;
	if( (G.f & G_FACESELECT) && ob==OBACT) tface= me->tface;
	else tface= 0;

	mvert= me->mvert;
	a= me->totface;

	if ELEM(ob->type, OB_SECTOR, OB_LIFE) glEnable(GL_CULL_FACE);

	if(ob==G.obedit || (G.obedit && ob->data==G.obedit->data)) {
		
		evl= G.edvl.first;
		while(evl) {
			if(evl->v1->h==0 && evl->v2->h==0 && evl->v3->h==0) {
				
				if(evl->mat_nr!=matnr) {
					matnr= evl->mat_nr;
					set_gl_material(matnr+1);
				}
				
				if(evl->v4 && evl->v4->h==0) {
				
					glBegin(GL_QUADS);
						glNormal3fv(evl->n);
						glVertex3fv(evl->v1->co);
						glVertex3fv(evl->v2->co);
						glVertex3fv(evl->v3->co);
						glVertex3fv(evl->v4->co);
					glEnd();
				}
				else {

					glBegin(GL_TRIANGLES);
						glNormal3fv(evl->n);
						glVertex3fv(evl->v1->co);
						glVertex3fv(evl->v2->co);
						glVertex3fv(evl->v3->co);
					glEnd();
				}
			}
			evl= evl->next;
		}
		
		glDisable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		
		if(ob==G.obedit) {
			calc_meshverts();
		
			if(G.zbuf) glDisable(GL_DEPTH_TEST);
			tekenvertices(0);
			tekenvertices(1);
			if(G.zbuf) glEnable(GL_DEPTH_TEST);
		}
		
	}
	else {

		start= 0; end= me->totface;
		set_buildvars(ob, &start, &end);
		mface+= start;
		if(tface) tface+= start;
		
		dl= find_displist(&ob->disp, DL_VERTS);
		if(dl) extverts= dl->verts;
	
		glBegin(GL_QUADS);
		glmode= GL_QUADS;
		
		for(a=start; a<end; a++, mface++, nors+=3) {
			if(mface->v3) {
				if(tface && (tface->flag & TF_HIDE)) {
					if( (G.f & G_BACKBUFSEL)==0) {
						glBegin(GL_LINE_LOOP);
						glVertex3fv( (mvert+mface->v1)->co);
						glVertex3fv( (mvert+mface->v2)->co);
						glVertex3fv( (mvert+mface->v3)->co);
						if(mface->v4) glVertex3fv( (mvert+mface->v1)->co);
						glEnd();
						tface++;
					}
				}
				else {
					if(extverts) {
						v1= extverts+3*mface->v1;
						v2= extverts+3*mface->v2;
						v3= extverts+3*mface->v3;
						if(mface->v4) v4= extverts+3*mface->v4;
						else v4= 0;
					}
					else {
						v1= (mvert+mface->v1)->co;
						v2= (mvert+mface->v2)->co;
						v3= (mvert+mface->v3)->co;
						if(mface->v4) v4= (mvert+mface->v4)->co;
						else v4= 0;
					}
					
					
					if(tface) {
						if(tface->mode & TF_TWOSIDE) glEnable(GL_CULL_FACE);
						else glDisable(GL_CULL_FACE);
					}
					
	
					/* dit GL_QUADS grapje is op snelheid getest: factor 2! */
						
					if(v4) {if(glmode==GL_TRIANGLES) {glmode= GL_QUADS; glEnd(); glBegin(GL_QUADS);}}
					else {if(glmode==GL_QUADS) {glmode= GL_TRIANGLES; glEnd(); glBegin(GL_TRIANGLES);}}
						
					if(G.f & G_BACKBUFSEL) {
						if(vertexpaint) {
							i= a+1;
							i= ((i & 0xF00)<<12) + ((i & 0xF0)<<8) + ((i & 0xF)<<4);
							i |= 0x0F0F0F;
							cpack(i);
						}
	
						glVertex3fv( v1 );
						glVertex3fv( v2 );
						glVertex3fv( v3 );
						if(v4) glVertex3fv( v4 );
						
					}
					else {
						
						if(mface->mat_nr!=matnr) {
							matnr= mface->mat_nr;
							set_gl_material(matnr+1);
						}
	
						if( (me->flag & ME_AUTOSMOOTH)==0 && (mface->flag & ME_SMOOTH)) {
							if(setsmooth==0) {
								glEnd();
								glShadeModel(GL_SMOOTH);
								if(glmode==GL_TRIANGLES) glBegin(GL_TRIANGLES);
								else glBegin(GL_QUADS);
								setsmooth= 1;
							}
							n1= (mvert+mface->v1)->no;
							n2= (mvert+mface->v2)->no;
							n3= (mvert+mface->v3)->no;
							if(v4) n4= (mvert+mface->v4)->no;
						
							if(mface->puno & ME_FLIPV1) {
								no[0]= -n1[0]; no[1]= -n1[1]; no[2]= -n1[2];
								glNormal3sv(no);
							}
							else glNormal3sv(n1);
							glVertex3fv( v1 );
							
							if(mface->puno & ME_FLIPV2) {
								no[0]= -n2[0]; no[1]= -n2[1]; no[2]= -n2[2];
								glNormal3sv(no);
							}
							else glNormal3sv(n2);
							glVertex3fv( v2 );
							
							if(mface->puno & ME_FLIPV3) {
								no[0]= -n3[0]; no[1]= -n3[1]; no[2]= -n3[2];
								glNormal3sv(no);
							}
							else glNormal3sv(n3);
							glVertex3fv( v3 );
							
							if(v4) {
								if(mface->puno & ME_FLIPV4) {
									no[0]= -n4[0]; no[1]= -n4[1]; no[2]= -n4[2];
									glNormal3sv(no);
								}
								else glNormal3sv(n4);
								glVertex3fv( v4 );
							}
						}
						else {
							if(setsmooth==1) {
								glEnd();
								glShadeModel(GL_FLAT);
								if(glmode==GL_TRIANGLES) glBegin(GL_TRIANGLES);
								else glBegin(GL_QUADS);
								setsmooth= 0;
							}
							glNormal3fv(nors);
							glVertex3fv( v1 );
							glVertex3fv( v2 );
							glVertex3fv( v3 );
							if(v4) glVertex3fv( v4 );
						}
					}
				}
				if(tface) tface++;
			}
		}
		
		glEnd();
	}
	
	if ELEM(ob->type, OB_SECTOR, OB_LIFE) glDisable(GL_CULL_FACE);

	if(G.f & G_BACKBUFSEL) {
		glDisable(GL_CULL_FACE);
	}
	glDisable(GL_LIGHTING);

}

void drawmeshshaded(Object *ob, uint *col1, uint *col2)
{
	Mesh *me;
	MVert *mvert;
	MFace *mface;
	TFace *tface;
	DispList *dl;
	float *extverts=0, *v1, *v2, *v3, *v4;
	int a, start, end, twoside;
	char *cp1, *cp2;
	
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);
	
	me= ob->data;
	mface= me->mface;
	
	/* tekent ie geen hide */
	if( (G.f & G_FACESELECT) && ob==OBACT) tface= me->tface;
	else tface= 0;
	
	mvert= me->mvert;
	a= me->totface;
	
	twoside= me->flag & ME_TWOSIDED;
	if(col2==0) twoside= 0;
	
	if(twoside) glEnable(GL_CULL_FACE);
	
	start= 0; end= me->totface;
	set_buildvars(ob, &start, &end);
	mface+= start;
	if(tface) tface+= start;
	col1+= 4*start;
	if(col2) col2+= 4*start;
	
	dl= find_displist(&ob->disp, DL_VERTS);
	if(dl) extverts= dl->verts;

	glBegin(GL_QUADS);
	
	cp1= (char *)col1;
	if(col2) cp2= (char *)col2;
	
	for(a=start; a<end; a++, mface++, cp1+= 16) {
		if(mface->v3) {
			if(tface && (tface->flag & TF_HIDE)) tface++;
			else {
				if(extverts) {
					v1= extverts+3*mface->v1;
					v2= extverts+3*mface->v2;
					v3= extverts+3*mface->v3;
					if(mface->v4) v4= extverts+3*mface->v4;
					else v4= 0;
				}
				else {
					v1= (mvert+mface->v1)->co;
					v2= (mvert+mface->v2)->co;
					v3= (mvert+mface->v3)->co;
					if(mface->v4) v4= (mvert+mface->v4)->co;
					else v4= 0;
				}

				if(tface) {
					if(tface->mode & TF_TWOSIDE) glEnable(GL_CULL_FACE);
					else glDisable(GL_CULL_FACE);
				}


				
				glColor3ub(cp1[3], cp1[2], cp1[1]);
				glVertex3fv( v1 );
				glColor3ub(cp1[7], cp1[6], cp1[5]);
				glVertex3fv( v2 );
				glColor3ub(cp1[11], cp1[10], cp1[9]);
				glVertex3fv( v3 );
				if(v4) {
					glColor3ub(cp1[15], cp1[14], cp1[13]);
					glVertex3fv( v4 );
				}
				else glVertex3fv( v3 );
				
				
				if(twoside) {

					glColor3ub(cp2[11], cp2[10], cp2[9]);
					glVertex3fv( v3 );
					glColor3ub(cp2[7], cp2[6], cp2[5]);
					glVertex3fv( v2 );
					glColor3ub(cp2[3], cp2[2], cp2[1]);
					glVertex3fv( v1 );
					if(mface->v4) {
						glColor3ub(cp2[15], cp2[14], cp2[13]);
						glVertex3fv( v4 );
					}
					else glVertex3fv( v1 );
						
				}
			}
		}
		if(col2) cp2+= 16;
	}
	
	glEnd();
	glShadeModel(GL_FLAT);
	if(twoside) glDisable(GL_CULL_FACE);
	
}


void drawDispList(Object *ob, int dt)
{
	ListBase *lb=0;
	DispList *dl;
	Mesh *me;
	Sector *se;
	int a, solid;

	solid= (dt > OB_WIRE);

	switch(ob->type) {
	case OB_MESH:
	case OB_SECTOR:
	case OB_LIFE:
		
		me= get_mesh(ob);
		if(me==0) return;
		
		if(me->bb==0) tex_space_mesh(me);
		if(me->totface>4) if(boundbox_clip(ob->obmat, me->bb)==0) return;
		
		if(dt==OB_SOLID ) {
			
			lb= &me->disp;
			if(lb->first==0) addnormalsDispList(ob, lb);
			
			dl= lb->first;
			if(dl==0) return;
			
			if(me->flag & ME_SMESH) {
				init_gl_materials(ob);
				two_sided(0);
				drawDispListsolid(lb, ob);
			}
			else drawmeshsolid(ob, dl->nors);
			
		}
		else if(dt==OB_SHADED) {
			if( G.f & G_VERTEXPAINT) {
				/* in deze volgorde: vertexpaint heeft soms al mcol gemaakt */
				if(me->mcol) 
					drawmeshshaded(ob, (uint *)me->mcol, 0);
				else if(me->tface) {
					tface_to_mcol(me);
					drawmeshshaded(ob, (uint *)me->mcol, 0);
					freeN(me->mcol); me->mcol= 0;
				}
				else 
					drawmeshwire(ob);
				
			}
			else {
				dl= ob->disp.first;
				
				if(dl==0 || dl->col1==0) {
					shadeDispList(ob);
					dl= ob->disp.first;
				}
				if(dl) {
					if(me->flag & ME_SMESH) 
						drawDispListshaded(&me->disp, ob);
					else 
						drawmeshshaded(ob, dl->col1, dl->col2);
				}
			}
		}
		
		if(ob==OBACT && (G.f & G_FACESELECT)) {
			draw_tfaces3D(ob, me);
		}
		
		break;
		
	case OB_FONT:
	case OB_CURVE:
	
		lb= &((Curve *)ob->data)->disp;
		if(lb->first==0) makeDispList(ob);
		
		if(solid && ob!=G.obedit) {
			dl= lb->first;
			if(dl==0) return;
			
			/* regel: dl->type INDEX3 altijd vooraan in lijst */
			if(dl->type!=DL_INDEX3) {
				curve_to_filledpoly(ob->data, lb);
				dl= lb->first;
			}
			if(dl->nors==0) addnormalsDispList(ob, lb);
			
			index3_nors_incr= 0;
			
			if(dt==OB_SHADED) {
				if(ob->disp.first==0) shadeDispList(ob);
				drawDispListshaded(lb, ob);
			}
			else {
				init_gl_materials(ob);
				two_sided(0);
				drawDispListsolid(lb, ob);
			}
			index3_nors_incr= 1;
		}
		else {
			draw_index_wire= 0;
			drawDispListwire(lb);
			draw_index_wire= 1;
		}
		break;
	case OB_SURF:
	
		lb= &((Curve *)ob->data)->disp;
		if(lb->first==0) makeDispList(ob);
		
		if(solid) {
			dl= lb->first;
			if(dl==0) return;
			
			if(dl->nors==0) addnormalsDispList(ob, lb);
			
			if(dt==OB_SHADED) {
				if(ob->disp.first==0) shadeDispList(ob);
				drawDispListshaded(lb, ob);
			}
			else {
				init_gl_materials(ob);
				two_sided(0);
			
				drawDispListsolid(lb, ob);
			}
		}
		else {
			drawDispListwire(lb);
		}
		break;
	case OB_MBALL:

		lb= &ob->disp;
		if(lb->first==0) makeDispList(ob);

		if(solid) {
			
			if(dt==OB_SHADED) {
				dl= lb->first;
				if(dl && dl->col1==0) shadeDispList(ob);
				drawDispListshaded(lb, ob);
			}
			else {
				init_gl_materials(ob);
				two_sided(0);
			
				drawDispListsolid(lb, ob);
			}
		}
		else drawDispListwire(lb);
		break;
	}
	
}

/* ******************************** */


void draw_particle_system(Object *ob, PartEff *paf)
{
	Particle *pa;
	float ptime, ctime, vec[3], vec1[3];
	int a;
	
	pa= paf->keys;
	if(pa==0) {
		build_particle_system(ob);
		pa= paf->keys;
		if(pa==0) return;
	}
	
	loadmatrix(G.vd->viewmat);
	
	if(ob->ipoflag & OB_OFFS_PARTICLE) ptime= ob->sf;
	else ptime= 0.0;
	ctime= bsystem_time(ob, 0, (float)CFRA, ptime);

	glPointSize(1.0);
	if(paf->stype!=PAF_VECT) glBegin(GL_POINTS);

	for(a=0; a<paf->totpart; a++, pa+=paf->totkey) {
		
		if(ctime > pa->time) {
			if(ctime < pa->time+pa->lifetime) {
			
				if(paf->stype==PAF_VECT) {
					where_is_particle(paf, pa, ctime, vec);
					where_is_particle(paf, pa, ctime+1.0, vec1);
		

					glBegin(GL_LINE_STRIP);
						glVertex3fv(vec);
						glVertex3fv(vec1);
					glEnd();
					
				}
				else {
					where_is_particle(paf, pa, ctime, vec);
					
					glVertex3fv(vec);
						
				}
			}
		}
	}
	if(paf->stype!=PAF_VECT) glEnd();

}

void draw_static_particle_system(Object *ob, PartEff *paf)
{
	Particle *pa;
	float ptime, ctime, mtime, vec[3], vec1[3];
	int a;
	
	pa= paf->keys;
	if(pa==0) {
		build_particle_system(ob);
		pa= paf->keys;
		if(pa==0) return;
	}
	
	glPointSize(1.0);
	if(paf->stype!=PAF_VECT) glBegin(GL_POINTS);

	for(a=0; a<paf->totpart; a++, pa+=paf->totkey) {
		
		where_is_particle(paf, pa, pa->time, vec1);
		
		mtime= pa->time+pa->lifetime+paf->staticstep-1;
		
		for(ctime= pa->time; ctime<mtime; ctime+=paf->staticstep) {
			
			/* make sure hair grows until the end.. */ 
			if(ctime>pa->time+pa->lifetime) ctime= pa->time+pa->lifetime;
			
			if(paf->stype==PAF_VECT) {
				where_is_particle(paf, pa, ctime+1, vec);

				glBegin(GL_LINE_STRIP);
					glVertex3fv(vec);
					glVertex3fv(vec1);
				glEnd();
				
				VECCOPY(vec1, vec);
			}
			else {
				where_is_particle(paf, pa, ctime, vec);
				
				glVertex3fv(vec);
					
			}
		}
	}
	if(paf->stype!=PAF_VECT) glEnd();

}

void drawmeshwire(Object *ob)
{
	extern float editbutsize;	/* buttons.c */
	Sector *se;
	Mesh *me;
	MVert *mvert;
	MFace *mface;
	DFace *dface;
	DispList *dl;
	Material *ma;
	EditVert *eve;
	EditEdge *eed;
	EditVlak *evl;
	float fvec[3], cent[3], *f1, *f2, *f3, *f4, *extverts=0;
	int a, start, end, test, colbcol=0, ok;

	me= get_mesh(ob);

	if(ob==G.obedit || (G.obedit && ob->data==G.obedit->data)) {
		
		if(G.zbuf==0 && me->flag & ME_SMESH) {
			cpack(0x505050);
			drawDispListwire(&me->disp);
		}
		cpack(0x0);
		
		if(G.obedit==ob) {
			cpack(0);
			if(ob->type==OB_SECTOR) {
				Sector *se= ob->data;
				if(se->flag & SE_SHOW_TEXMESH); else cpack(0x401000);
			}
			else if(ob->type==OB_LIFE) {
				Life *lf= ob->data;
				if(lf->flag & LF_SHOW_TEXMESH); else cpack(0x401000);
			}
		}
		
		eed= G.eded.first;

		glBegin(GL_LINES);
		while(eed) {
			if(eed->h==0) {
				glVertex3fv(eed->v1->co);
				glVertex3fv(eed->v2->co);
			}
			eed= eed->next;
		}
		glEnd();
		
		if(ob!=G.obedit) return;
		
		calc_meshverts();

		if(G.zbuf) glDisable(GL_DEPTH_TEST);
		tekenvertices(0);
		tekenvertices(1);
		if(G.zbuf) glEnable(GL_DEPTH_TEST);

		if(G.f & G_DRAWNORMALS) {	/* normals */
			cpack(0xDDDD22);

			glBegin(GL_LINES);
			
			evl= G.edvl.first;
			while(evl) {
				if(evl->v1->h==0 && evl->v2->h==0 && evl->v3->h==0) {
					if(evl->v4) CalcCent4f(fvec, evl->v1->co, evl->v2->co, evl->v3->co, evl->v4->co);
					else CalcCent3f(fvec, evl->v1->co, evl->v2->co, evl->v3->co);

					glVertex3fv(fvec);
					fvec[0]+= editbutsize*evl->n[0];
					fvec[1]+= editbutsize*evl->n[1];
					fvec[2]+= editbutsize*evl->n[2];
					glVertex3fv(fvec);
					
				}
				evl= evl->next;
			}

			if(me->flag & ME_SMESH) {
				cpack(0xEE4422);
			
				eve= G.edve.first;
				while(eve) {
					if(eve->h==0) {
						VECCOPY(fvec, eve->co);
	
						glVertex3fv(fvec);
						fvec[0]+= editbutsize*eve->no[0];
						fvec[1]+= editbutsize*eve->no[1];
						fvec[2]+= editbutsize*eve->no[2];
						glVertex3fv(fvec);
						
					}
					eve= eve->next;
				}
			}
			glEnd();
		}
		if(G.f & (G_FACESELECT+G_DRAWFACES)) {	/* vlakken */
			
			evl= G.edvl.first;
			while(evl) {
				if(evl->v1->h==0 && evl->v2->h==0 && evl->v3->h==0) {
					
					if(vlakselectedAND(evl, 1)) cpack(0x559999);
					else cpack(0x664466);
				
					if(evl->v4 && evl->v4->h==0) {
					
						CalcCent4f(cent, evl->v1->co, evl->v2->co, evl->v3->co, evl->v4->co);
						glBegin(GL_LINE_LOOP);
							VecMidf(fvec, cent, evl->v1->co); glVertex3fv(fvec);
							VecMidf(fvec, cent, evl->v2->co); glVertex3fv(fvec);
							VecMidf(fvec, cent, evl->v3->co); glVertex3fv(fvec);
							VecMidf(fvec, cent, evl->v4->co); glVertex3fv(fvec);
						glEnd();
					}
					else {

						CalcCent3f(cent, evl->v1->co, evl->v2->co, evl->v3->co);
						glBegin(GL_LINE_LOOP);
							VecMidf(fvec, cent, evl->v1->co); glVertex3fv(fvec);
							VecMidf(fvec, cent, evl->v2->co); glVertex3fv(fvec);
							VecMidf(fvec, cent, evl->v3->co); glVertex3fv(fvec);
						glEnd();
					}
				}
				evl= evl->next;
			}
		}
	}
	else {
		
		if(me==0) return;
		
		if(me->bb==0) tex_space_mesh(me);
		if(me->totface>4) if(boundbox_clip(ob->obmat, me->bb)==0) return;
		
		if(me->flag & ME_SMESH) drawDispListwire(&me->disp);
		else {
			
			mvert= me->mvert;
			mface= me->mface;
			dface= me->dface;
			
			ok= 0;
			if(me->totface==0) ok= 1;
			else {
				ma= give_current_material(ob, 1);
				if(ma && (ma->mode & MA_HALO)) ok= 1;
			}
			
			dl= find_displist(&ob->disp, DL_VERTS);
			if(dl) extverts= dl->verts;
			
			if(ok) {
				
				start= 0; end= me->totvert;
				set_buildvars(ob, &start, &end);
				
				glPointSize(1.5);
				glBegin(GL_POINTS);
				
				if(extverts) {
					extverts+= 3*start;
					for(a= start; a<end; a++, extverts+=3) {
						glVertex3fv(extverts);
					}
				}
				else {
					mvert+= start;
					for(a= start; a<end; a++, mvert++) {
						glVertex3fv(mvert->co);
					}
				}
				
				glEnd();
				glPointSize(1.0);
			}
			else {
				
				start= 0; end= me->totface;
				set_buildvars(ob, &start, &end);
				mface+= start;
				
				for(a=start; a<end; a++, mface++) {
					test= mface->edcode;
					
	if(dface ) {
		/* test= mface->edcode>>4; */
		if(dface->flag & DF_HILITE) cpack(0xFF00FF);
		else cpack(0);
		dface->flag &= ~DF_HILITE;
		dface++;
	}
					
					if(test) {
						if(extverts) {
							f1= extverts+3*mface->v1;
							f2= extverts+3*mface->v2;
						}
						else {
							f1= (mvert+mface->v1)->co;
							f2= (mvert+mface->v2)->co;
						}
						
						if(mface->v4) {
							if(extverts) {
								f3= extverts+3*mface->v3;
								f4= extverts+3*mface->v4;
							}
							else {
								f3= (mvert+mface->v3)->co;
								f4= (mvert+mface->v4)->co;
							}
							
							if(test== ME_V1V2+ME_V2V3+ME_V3V4+ME_V4V1) {
								glBegin(GL_LINE_LOOP);
									glVertex3fv(f1); glVertex3fv(f2); glVertex3fv(f3); glVertex3fv(f4);
								glEnd();
							}
							else if(test== ME_V1V2+ME_V2V3+ME_V3V4) {
	
								glBegin(GL_LINE_STRIP);
									glVertex3fv(f1); glVertex3fv(f2); glVertex3fv(f3); glVertex3fv(f4);
								glEnd();
							}
							else if(test== ME_V2V3+ME_V3V4+ME_V4V1) {
	
								glBegin(GL_LINE_STRIP);
									glVertex3fv(f2); glVertex3fv(f3); glVertex3fv(f4); glVertex3fv(f1);
								glEnd();
							}
							else if(test== ME_V3V4+ME_V4V1+ME_V1V2) {
	
								glBegin(GL_LINE_STRIP);
									glVertex3fv(f3); glVertex3fv(f4); glVertex3fv(f1); glVertex3fv(f2);
								glEnd();
							}
							else if(test== ME_V4V1+ME_V1V2+ME_V2V3) {
	
								glBegin(GL_LINE_STRIP);
									glVertex3fv(f4); glVertex3fv(f1); glVertex3fv(f2); glVertex3fv(f3);
								glEnd();
							}
							else {
								if(test & ME_V1V2) {
	
									glBegin(GL_LINE_STRIP);
										glVertex3fv(f1); glVertex3fv(f2);
									glEnd();
								}
								if(test & ME_V2V3) {
	
									glBegin(GL_LINE_STRIP);
										glVertex3fv(f2); glVertex3fv(f3);
									glEnd();
								}
								if(test & ME_V3V4) {
	
									glBegin(GL_LINE_STRIP);
										glVertex3fv(f3); glVertex3fv(f4);
									glEnd();
								}
								if(test & ME_V4V1) {
	
									glBegin(GL_LINE_STRIP);
										glVertex3fv(f4); glVertex3fv(f1);
									glEnd();
								}
							}
						}
						else if(mface->v3) {
							if(extverts) f3= extverts+3*mface->v3;
							else f3= (mvert+mface->v3)->co;
		
							if(test== ME_V1V2+ME_V2V3+ME_V3V1) {
								glBegin(GL_LINE_LOOP);
									glVertex3fv(f1); glVertex3fv(f2); glVertex3fv(f3);
								glEnd();
							}
							else if(test== ME_V1V2+ME_V2V3) {
	
								glBegin(GL_LINE_STRIP);
									glVertex3fv(f1); glVertex3fv(f2); glVertex3fv(f3);
								glEnd();
							}
							else if(test== ME_V2V3+ME_V3V1) {
	
								glBegin(GL_LINE_STRIP);
									glVertex3fv(f2); glVertex3fv(f3); glVertex3fv(f1);
								glEnd();
							}
							else if(test== ME_V1V2+ME_V3V1) {
	
								glBegin(GL_LINE_STRIP);
									glVertex3fv(f3); glVertex3fv(f1); glVertex3fv(f2);
								glEnd();
							}
							else {
								if(test & ME_V1V2) {
	
									glBegin(GL_LINE_STRIP);
										glVertex3fv(f1); glVertex3fv(f2);
									glEnd();
								}
								if(test & ME_V2V3) {
	
									glBegin(GL_LINE_STRIP);
										glVertex3fv(f2); glVertex3fv(f3);
									glEnd();
								}
								if(test & ME_V3V1) {
	
									glBegin(GL_LINE_STRIP);
										glVertex3fv(f3); glVertex3fv(f1);
									glEnd();
								}
							}
						}
						else if(test & ME_V1V2) {
	
							glBegin(GL_LINE_STRIP);
								glVertex3fv(f1); glVertex3fv(f2);
							glEnd();
						}
					}
				}
			}
			
		}
	}
}

uint nurbcol[8]= {
	0, 0x9090, 0x409030, 0x603080, 0, 0x40fff0, 0x40c033, 0xA090F0 };

void tekenhandlesN(nu, sel)
Nurb *nu;
short sel;
{
	BezTriple *bezt;
	float *fp;
	uint *col;
	int a;

	if(nu->hide) return;
	if( (nu->type & 7)==1) {
		if(sel) col= nurbcol+4;
		else col= nurbcol;

		bezt= nu->bezt;
		a= nu->pntsu;
		while(a--) {
			if(bezt->hide==0) {
				if( (bezt->f2 & 1)==sel) {
					fp= bezt->vec[0];
					cpack(col[bezt->h1]);

					glBegin(GL_LINE_STRIP); 
					glVertex3fv(fp);
					glVertex3fv(fp+3); 
					glEnd();
					cpack(col[bezt->h2]);

					glBegin(GL_LINE_STRIP); 
					glVertex3fv(fp+3); 
					glVertex3fv(fp+6); 
					glEnd();
				}
				else if( (bezt->f1 & 1)==sel) {
					fp= bezt->vec[0];
					cpack(col[bezt->h1]);

					glBegin(GL_LINE_STRIP); 
					glVertex3fv(fp); 
					glVertex3fv(fp+3); 
					glEnd();
				}
				else if( (bezt->f3 & 1)==sel) {
					fp= bezt->vec[1];
					cpack(col[bezt->h2]);

					glBegin(GL_LINE_STRIP); 
					glVertex3fv(fp); 
					glVertex3fv(fp+3); 
					glEnd();
				}
			}
			bezt++;
		}
	}
}

void tekenvertsN(nu, sel)
Nurb *nu;
short sel;
{
	BezTriple *bezt;
	BPoint *bp;
	uint *rect;
	int a;

	if(nu->hide) return;

	if(sel) cpack(B_YELLOW);
	else cpack(B_PURPLE);
	glPointSize(3.0);
	
	glBegin(GL_POINTS);
	
	if((nu->type & 7)==1) {

		bezt= nu->bezt;
		a= nu->pntsu;
		while(a--) {
			if(bezt->hide==0) {
				if((bezt->f1 & 1)==sel) glVertex3fv(bezt->vec[0]);
				if((bezt->f2 & 1)==sel) glVertex3fv(bezt->vec[1]);
				if((bezt->f3 & 1)==sel) glVertex3fv(bezt->vec[2]);
			}
			bezt++;
		}
	}
	else {
		bp= nu->bp;
		a= nu->pntsu*nu->pntsv;
		while(a--) {
			if(bp->hide==0) {
				if((bp->f1 & 1)==sel) glVertex3fv(bp->vec);
			}
			bp++;
		}
	}
	
	glEnd();
	glPointSize(1.0);
}

void draw_editnurb(Object *ob, Nurb *nurb, int sel)
{
	Nurb *nu;
	BPoint *bp, *bp1;
	int a, b, ofs;
	
	nu= nurb;
	while(nu) {
		if(nu->hide==0) {
			switch(nu->type & 7) {
			case CU_POLY:
				cpack(nurbcol[3]);
				bp= nu->bp;
				for(b=0; b<nu->pntsv; b++) {
					if(nu->flagu & 1) glBegin(GL_LINE_LOOP);

					else glBegin(GL_LINE_STRIP);

					for(a=0; a<nu->pntsu; a++, bp++) {
						glVertex3fv(bp->vec);
					}

					if(nu->flagu & 1) glEnd();
					else glEnd();
				}
				break;
			case CU_NURBS:

				bp= nu->bp;
				for(b=0; b<nu->pntsv; b++) {
					bp1= bp;
					bp++;
					for(a=nu->pntsu-1; a>0; a--, bp++) {
						if(bp->hide==0 && bp1->hide==0) {
							if(sel) {
								if( (bp->f1 & 1) && ( bp1->f1 & 1) ) {
									cpack(nurbcol[5]);
		
									glBegin(GL_LINE_STRIP);
									glVertex3fv(bp->vec); 
									glVertex3fv(bp1->vec);
									glEnd();
								}
							}
							else {
								if( (bp->f1 & 1) && ( bp1->f1 & 1) );
								else {
									cpack(nurbcol[1]);
		
									glBegin(GL_LINE_STRIP);
									glVertex3fv(bp->vec); 
									glVertex3fv(bp1->vec);
									glEnd();
								}
							}
						}
						bp1= bp;
					}
				}
				if(nu->pntsv > 1) {	/* surface */

					ofs= nu->pntsu;
					for(b=0; b<nu->pntsu; b++) {
						bp1= nu->bp+b;
						bp= bp1+ofs;
						for(a=nu->pntsv-1; a>0; a--, bp+=ofs) {
							if(bp->hide==0 && bp1->hide==0) {
								if(sel) {
									if( (bp->f1 & 1) && ( bp1->f1 & 1) ) {
										cpack(nurbcol[7]);
			
										glBegin(GL_LINE_STRIP);
										glVertex3fv(bp->vec); 
										glVertex3fv(bp1->vec);
										glEnd();
									}
								}
								else {
									if( (bp->f1 & 1) && ( bp1->f1 & 1) );
									else {
										cpack(nurbcol[3]);
			
										glBegin(GL_LINE_STRIP);
										glVertex3fv(bp->vec); 
										glVertex3fv(bp1->vec);
										glEnd();
									}
								}
							}
							bp1= bp;
						}
					}

				}
				break;
			}
		}
		nu= nu->next;
	}
}

void drawnurb(Object *ob, Nurb *nurb, int dt)
{
	extern float editbutsize;	/* buttons.c */
	Curve *cu;
	Nurb *nu;
	BezTriple *bezt;
	BPoint *bp, *bp1;
	BevPoint *bevp;
	BevList *bl;
	float vec[3], seg[12], *fp, *data;
	int a, b, ofs, nr, skip;

	/* eerst handles niet select */
	nu= nurb;
	while(nu) {
		if((nu->type & 7)==CU_BEZIER) {
			tekenhandlesN(nu, 0);
		}
		nu= nu->next;
	}
	
	/* dan DispList */
	
	cpack(0);
	cu= ob->data;
	drawDispList(ob, dt);

	draw_editnurb(ob, nurb, 0);
	draw_editnurb(ob, nurb, 1);

	if(cu->flag & CU_3D) {
	
		if(cu->bev.first==0) makeBevelList(ob);
		
		cpack(0x0);
		bl= cu->bev.first;
		nu= nurb;
		while(nu && bl) {
			bevp= (BevPoint *)(bl+1);		
			nr= bl->nr;
			
			skip= nu->resolu/16;
			
			while(nr-- > 0) {
				
				glBegin(GL_LINE_STRIP);
				vec[0]= bevp->x-editbutsize*bevp->mat[0][0];
				vec[1]= bevp->y-editbutsize*bevp->mat[0][1];
				vec[2]= bevp->z-editbutsize*bevp->mat[0][2];
				glVertex3fv(vec);
				vec[0]= bevp->x+editbutsize*bevp->mat[0][0];
				vec[1]= bevp->y+editbutsize*bevp->mat[0][1];
				vec[2]= bevp->z+editbutsize*bevp->mat[0][2];
				glVertex3fv(vec);

				glEnd();
				
				bevp++;
				
				a= skip;
				while(a--) {
					bevp++;
					nr--;
				}
			}

			bl= bl->next;
			nu= nu->next;
		}
	}

	calc_Nurbverts(nurb);

	if(G.zbuf) glDisable(GL_DEPTH_TEST);
	
	nu= nurb;
	while(nu) {
		if((nu->type & 7)==1) tekenhandlesN(nu, 1);
		tekenvertsN(nu, 0);
		nu= nu->next;
	}

	nu= nurb;
	while(nu) {
		tekenvertsN(nu, 1);
		nu= nu->next;
	}
	
	if(G.zbuf) glEnable(GL_DEPTH_TEST); 
}

void tekentextcurs()
{
	cpack(0);

	glBegin(GL_POLYGON);
	glVertex2fv(G.textcurs[0]);
	glVertex2fv(G.textcurs[1]);
	glVertex2fv(G.textcurs[2]);
	glVertex2fv(G.textcurs[3]);
	glEnd();
}

void drawcircball(float *cent, float rad, float tmat[][4])
{
	float si, co, phi, dphi, vec[3], vx[3], vy[3];
	int a, tot=32;
		
	VECCOPY(vx, tmat[0]);
	VECCOPY(vy, tmat[1]);
	VecMulf(vx, rad);
	VecMulf(vy, rad);
	
	dphi= 2.0*M_PI/tot;
	phi= 0.0;
	
	glBegin(GL_LINE_LOOP);
	for(a=0; a<tot; a++, phi+= dphi) {
		si= fsin(phi);
		co= fcos(phi);
		vec[0]= cent[0]+si*vx[0]+co*vy[0];
		vec[1]= cent[1]+si*vx[1]+co*vy[1];
		vec[2]= cent[2]+si*vx[2]+co*vy[2];
		glVertex3fv(vec);
	}
	glEnd();
}

void drawmball(Object *ob, int dt)
{
	MetaBall *mb;
	MetaElem *ml;
	float imat[4][4], tmat[4][4];
	int code= 1;
	
	mb= ob->data;

	if(ob==G.obedit) {
		cpack(0x0);
		if((G.f & G_PICKSEL)==0 ) drawDispList(ob, dt);
		ml= editelems.first;
	}
	else {
		drawDispList(ob, dt);
		ml= mb->elems.first;
	}

	getmatrix(tmat);
	Mat4Invert(imat, tmat);
	Normalise(imat[0]);
	Normalise(imat[1]);
	
	while(ml) {
		
		if(ob==G.obedit) {
			if(ml->flag & SELECT) cpack(0xA0A0F0);
			else cpack(0x3030A0);
			
			if(G.f & G_PICKSEL) {
				ml->selcol= code;
				glLoadName(code++);
			}
		}
		drawcircball(&(ml->x), ml->rad, imat);
		
		ml= ml->next;
	}

}

void drawboundbox(Object *ob)
{
	Mesh *me;
	BoundBox *bb=0;
	float *vec;
	
	if(ob->type==OB_MESH) {
		bb= ( (Mesh *)ob->data )->bb;
		if(bb==0) {
			tex_space_mesh(ob->data);
			bb= ( (Mesh *)ob->data )->bb;
		}
	}
	else if ELEM3(ob->type, OB_CURVE, OB_SURF, OB_FONT) {
		bb= ( (Curve *)ob->data )->bb;
		if(bb==0) {
			makeDispList(ob);
			bb= ( (Curve *)ob->data )->bb;
		}
	}
	else if(ob->type==OB_MBALL) {
		bb= ob->bb;
		if(bb==0) {
			makeDispList(ob);
			bb= ob->bb;
		}
	}
	else if ELEM(ob->type, OB_SECTOR, OB_LIFE) {
		me= get_mesh(ob);
		if(me) bb= me->bb;
	}
	else {
		drawcube();
		return;
	}
	
	if(bb==0) return;
	
	vec= bb->vec[0];

	glBegin(GL_LINE_STRIP);
		glVertex3fv(vec); glVertex3fv(vec+3);glVertex3fv(vec+6); glVertex3fv(vec+9);
		glVertex3fv(vec); glVertex3fv(vec+12);glVertex3fv(vec+15); glVertex3fv(vec+18);
		glVertex3fv(vec+21); glVertex3fv(vec+12);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3fv(vec+3); glVertex3fv(vec+15);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3fv(vec+6); glVertex3fv(vec+18);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3fv(vec+9); glVertex3fv(vec+21);
	glEnd();
}

void drawtexspace(Object *ob)
{
	Mesh *me;
	MetaBall *mb;
	Curve *cu;
	BoundBox bb;
	float *vec, *loc, *size;
	
	if(ob->type==OB_MESH) {
		me= ob->data;
		size= me->size;
		loc= me->loc;
	}
	else if ELEM3(ob->type, OB_CURVE, OB_SURF, OB_FONT) {
		cu= ob->data;
		size= cu->size;
		loc= cu->loc;
	}
	else if(ob->type==OB_MBALL) {
		mb= ob->data;
		size= mb->size;
		loc= mb->loc;
	}
	else return;
	
	bb.vec[0][0]=bb.vec[1][0]=bb.vec[2][0]=bb.vec[3][0]= loc[0]-size[0];
	bb.vec[4][0]=bb.vec[5][0]=bb.vec[6][0]=bb.vec[7][0]= loc[0]+size[0];
	
	bb.vec[0][1]=bb.vec[1][1]=bb.vec[4][1]=bb.vec[5][1]= loc[1]-size[1];
	bb.vec[2][1]=bb.vec[3][1]=bb.vec[6][1]=bb.vec[7][1]= loc[1]+size[1];

	bb.vec[0][2]=bb.vec[3][2]=bb.vec[4][2]=bb.vec[7][2]= loc[2]-size[2];
	bb.vec[1][2]=bb.vec[2][2]=bb.vec[5][2]=bb.vec[6][2]= loc[2]+size[2];
	
	setlinestyle(2);
		
	vec= bb.vec[0];

	glBegin(GL_LINE_STRIP);
		glVertex3fv(vec); glVertex3fv(vec+3);glVertex3fv(vec+6); glVertex3fv(vec+9);
		glVertex3fv(vec); glVertex3fv(vec+12);glVertex3fv(vec+15); glVertex3fv(vec+18);
		glVertex3fv(vec+21); glVertex3fv(vec+12);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3fv(vec+3); glVertex3fv(vec+15);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3fv(vec+6); glVertex3fv(vec+18);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3fv(vec+9); glVertex3fv(vec+21);
	glEnd();
	
	setlinestyle(0);
}


void draw_object(Base *base)
{
	PartEff *paf;
	Object *ob;
	Curve *cu;
	Life *lf;
	Mesh *me;
	ListBase elems;
	CfraElem *ce;
	float cfraont, axsize=1.0;
	uint *rect, col=0;
	static int warning_recursive= 0;
	int sel, drawtype, colindex= 0, ipoflag;
	short dt, dtx, zbufoff= 0;
	char str[4];
	
	ob= base->object;

	/* keys tekenen? */
	if(base==BASACT || (base->flag & (SELECT+BA_WASSEL))) {
		if(warning_recursive==0 && ob!=G.obedit) {
			if(ob->ipo && ob->ipo->showkey && (ob->ipoflag & OB_DRAWKEY)) {
				float temp[7][3];

				warning_recursive= 1;
				
				elems.first= elems.last= 0;
				make_cfra_list(ob->ipo, &elems);
		
				cfraont= CFRA;
				drawtype= G.vd->drawtype;
				if(drawtype>OB_WIRE) G.vd->drawtype= OB_WIRE;
				sel= base->flag;
				memcpy(temp, &ob->loc, 7*3*sizeof(float));
				
				ipoflag= ob->ipoflag;
				ob->ipoflag &= ~OB_OFFS_OB;
				
				set_no_parent_ipo(1);
				disable_speed_curve(1);

				if ((ob->ipoflag & OB_DRAWKEYSEL)==0) {
					ce= elems.first;
					while(ce) {
						if(!ce->sel) {
							CFRA= ce->cfra/G.scene->r.framelen;
						
							base->flag= 0;
						
							where_is_object_time(ob, CFRA);
							draw_object(base);
						}
						ce= ce->next;
					}
				}
				
				ce= elems.first;
				while(ce) {
					if(ce->sel) {
						CFRA= ce->cfra/G.scene->r.framelen;
						
						base->flag= SELECT;
						
						where_is_object_time(ob, CFRA);
						draw_object(base);
					}
					ce= ce->next;
				}

				set_no_parent_ipo(0);
				disable_speed_curve(0);
				
				base->flag= sel;
				ob->ipoflag= ipoflag;
				
				/* restore icu->curval */
				CFRA= cfraont;
				
				memcpy(&ob->loc, temp, 7*3*sizeof(float));
				where_is_object(ob);
				G.vd->drawtype= drawtype;
				
				freelistN(&elems);

				warning_recursive= 0;
			}
		}
	}
	
	/* patch? kinderen met timeoffs verprutsen ouders. Hoe los je dat op! */
	/* if( ((int)ob->ctime) != F_CFRA) where_is_object(ob); */

	multmatrix(ob->obmat);

	/* welke wire kleur */
	if((G.f & (G_BACKBUFSEL+G_PICKSEL)) == 0) {
		project_short(ob->obmat[3], &base->sx);
		
		if(G.moving==1 && (base->flag & (SELECT+BA_PARSEL))) colindex= 9;
		else {
			if(BASACT==base) {
				if(base->flag & (SELECT+BA_WASSEL)) colindex= 2;
			}
			else {
				if(base->flag & (SELECT+BA_WASSEL)) colindex= 1;
			}
			if(ob->id.lib) colindex+= 3;
			else if(warning_recursive==1) colindex+= 6;
		}	
		
/* ZR ADDED THIS */
#ifdef BADZR252
		if (colindex==0) {
			if (give_current_material(ob, ob->actcol)) {
				Material *ma= give_current_material(ob, ob->actcol);
			
				glColor3f(ma->r, ma->g, ma->b);
			}
		} else {
			col= colortab[colindex];
			cpack(col);
		}
#else
		col= colortab[colindex];
		cpack(col);
#endif		
/* END BAD ZR */		
	}
	
	/* maximum drawtype */
	dt= MIN2(G.vd->drawtype, ob->dt);
	if(G.zbuf==0 && dt>OB_WIRE) dt= OB_WIRE;
	dtx= 0;
	
	/* faceselect uitzondering: ook solid tekenen als dt==wire */
	if(ob==OBACT && (G.f & (G_FACESELECT+G_VERTEXPAINT))) {
		if(ob->type==OB_MESH) {
			
			if(G.f & G_BACKBUFSEL) dt= OB_SOLID;
			else dt= OB_SHADED;
	
			glClearDepth(1.0); glClear(GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			zbufoff= 1;
			
		}
		else {
			if(dt<OB_SOLID) {
				dt= OB_SOLID;
				glClearDepth(1.); glClear(GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				zbufoff= 1;
			}
		}
	}
	else if(dt>=OB_WIRE ) {

		if(dt>OB_SOLID) if(G.f & G_BACKBUFSEL) dt= OB_SOLID;

		dtx= ob->dtx;
		if(G.obedit==ob) {
			if(dtx & OB_TEXSPACE) dtx= OB_TEXSPACE;
			else dtx= 0;
		}
		
		if(G.f & G_DRAW_EXT) {
			if(ob->type==OB_EMPTY || ob->type==OB_CAMERA || ob->type==OB_LAMP) dt= OB_WIRE;
		}
		
	}

	if( (G.f & G_DRAW_EXT) && dt>OB_WIRE) {
		
		switch( ob->type) {
		case OB_MBALL:
			drawmball(ob, dt);
			break;
		}
	}
	else {

		switch( ob->type) {
			
		case OB_MESH:
			me= ob->data;
			
			if(base->flag & OB_RADIO);
			else if(ob==G.obedit || (G.obedit && ob->data==G.obedit->data)) {
				if(dt<=OB_WIRE) drawmeshwire(ob);
				else {
					if(me->flag & ME_SMESH) {
						init_gl_materials(ob);
						two_sided(0);
						drawDispListsolid(&me->disp, ob);
						drawmeshwire(ob);
					}
					else drawmeshsolid(ob, 0);
				}
				if(ob==G.obedit && (G.f & G_PROPORTIONAL)) DRAW_PROP_CIRCLE();
			}
			else {
				Material *ma= give_current_material(ob, 1);
				
				if(dt==OB_BOUNDBOX) drawboundbox(ob);
				else if(dt==OB_WIRE) drawmeshwire(ob);
				else if(ma && (ma->mode & MA_HALO)) drawmeshwire(ob);
				else if( ((Mesh *)ob->data)->tface ) {
					if((G.f & (G_BACKBUFSEL))) drawmeshsolid(ob, 0);
					else draw_tface_mesh(ob, ob->data, dt);
				}
				else  drawDispList(ob, dt);
			}
			if(ob!=G.obedit && (G.f & (G_BACKBUFSEL+G_PICKSEL))==0) {
				if( paf=give_parteff(ob) ) {
					if(col) cpack(0xFFFFFF);	/* zichtbaarheid */
					if(paf->flag & PAF_STATIC) draw_static_particle_system(ob, paf);
					else draw_particle_system(ob, paf);
					cpack(col);
				}
			}
			
			break;
		case OB_FONT:
			cu= ob->data;
			if(ob==G.obedit) {
				tekentextcurs();
				cpack(0xFFFF90);
				drawDispList(ob, OB_WIRE);
			}
			else if(dt==OB_BOUNDBOX) drawboundbox(ob);
			else if(boundbox_clip(ob->obmat, cu->bb)) drawDispList(ob, dt);
				
			break;
		case OB_CURVE:
		case OB_SURF:
			cu= ob->data;
			/* een pad niet solid tekenen: wel dus!!! */
			/* if(cu->flag & CU_PATH) if(dt>OB_WIRE) dt= OB_WIRE; */
			
			if(ob==G.obedit) {
				drawnurb(ob, editNurb.first, dt);
				if((G.f & G_PROPORTIONAL)) DRAW_PROP_CIRCLE();
			}
			else if(dt==OB_BOUNDBOX) drawboundbox(ob);
			else if(boundbox_clip(ob->obmat, cu->bb)) drawDispList(ob, dt);
			
			break;
		case OB_MBALL:
			if(ob==G.obedit) drawmball(ob, dt);
			else if(dt==OB_BOUNDBOX) drawboundbox(ob);
			else drawmball(ob, dt);
			break;
		case OB_EMPTY:
			drawaxes(1.0);
			break;
		case OB_LAMP:
			/* doet loadmatrix */
			drawlamp(ob);
			break;
		case OB_CAMERA:
			drawcamera(ob);
			break;
		case OB_LATTICE:
			drawlattice(ob);
			if(ob==G.obedit && (G.f & G_PROPORTIONAL)) DRAW_PROP_CIRCLE();
			break;
		case OB_SECTOR:
			drawsector(ob, dt, col);
			break;
		case OB_LIFE:
			drawlife(ob, dt, col);
			break;
		case OB_IKA:
			draw_ika(ob, base->flag & SELECT);
			break;
		default:
			drawaxes(1.0);
		}
	}

	/* draw extra: na gewone draw ivm makeDispList */
	if(dtx) {
		if(G.f & G_SIMULATION);
		else if(dtx & OB_AXIS) {
			if(ob->type==OB_LIFE) {lf= ob->data; axsize= lf->axsize;}
			drawaxes(axsize);
		}
		if(dtx & OB_BOUNDBOX) drawboundbox(ob);
		if(dtx & OB_TEXSPACE) drawtexspace(ob);
		if(dtx & OB_DRAWNAME) {
			if(ob->type==OB_LAMP) glRasterPos3fv(ob->obmat[3]);
			else glRasterPos3f(0.0,  0.0,  0.0);
			str[0]= ' '; str[1]= 0;
			fmsetfont(G.font);
			fmprstr(str);
			fmprstr(ob->id.name+2);
		}
		if(dtx & OB_DRAWIMAGE) drawDispListwire(&ob->disp);
	}

	loadmatrix(G.vd->viewmat);

	if(zbufoff) glDisable(GL_DEPTH_TEST);

	if(warning_recursive) return;
	if(base->flag & OB_FROMDUPLI) return;
	if(base->flag & OB_RADIO) return;
	if(G.f & G_SIMULATION) return;

	if((G.f & (G_BACKBUFSEL+G_PICKSEL))==0) {
		/* hulplijnen e.d. */
		
		if(ob->parent && (ob->parent->lay & G.vd->lay)) {
			setlinestyle(3);
			LINE3F(ob->obmat[3], ob->orig);
			setlinestyle(0);
		}
		
		/* object centers */
		if(G.zbuf) glDisable(GL_DEPTH_TEST);
		if(ob->type == OB_LAMP) {
			if(ob->id.lib) {
				if(base->flag & SELECT) rect= rectllib_sel;
				else rect= rectllib_desel;
			}
			else if(ob->id.us>1) {
				if(base->flag & SELECT) rect= rectlus_sel;
				else rect= rectlus_desel;
			}
			else {
				if(base->flag & SELECT) rect= rectl_sel;
				else rect= rectl_desel;
			}
			tekenrect9(ob->obmat[3], rect);
		}
		else {
			if(ob->id.lib || ob->id.us>1) {
				if(base->flag & SELECT) rect= rectu_sel;
				else rect= rectu_desel;
			}
			else {
				if(base->flag & SELECT) rect= rect_sel;
				else rect= rect_desel;
			}
			tekenrect4(ob->obmat[3], rect);
		}
		if(G.zbuf) glEnable(GL_DEPTH_TEST);
		
		if(ob->network.first) {
			NetLink *nl= ob->network.first;
			setlinestyle(3);
			while(nl) {
				LINE3F(ob->obmat[3], nl->ob->obmat[3]);
				nl= nl->next;
			}
			setlinestyle(0);
		}
	}
	else if((G.f & (G_VERTEXPAINT|G_FACESELECT))==0) {
	
		glBegin(GL_POINTS);
			glVertex3fv(ob->obmat[3]);
		glEnd();
		
	}
}

void draw_object_ext(Base *base)
{
	ScrArea *tempsa, *sa;
	View3D *vd;
	
	if(G.vd==0) return;
	
	if(G.vd->drawtype > OB_WIRE) {
		G.zbuf= 1;
		glEnable(GL_DEPTH_TEST);
	}
	
	G.f |= G_DRAW_EXT;
	
	glDrawBuffer(GL_FRONT);

	/* alle views aflopen */
	tempsa= curarea;
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->spacetype==SPACE_VIEW3D) {
			/* er wordt beperkt in beide buffers getekend: selectbuffer! */
			
			vd= sa->spacedata.first;
			if(base->lay & vd->lay) {

				areawinset(sa->win);

				#if defined(__BeOS) || defined(WIN32) || defined(PPC) || defined(MESA31)

				glDrawBuffer(GL_FRONT);
				draw_object(base);
				sa->win_swap= WIN_FRONT_OK;

				#else

				if(sa->win_swap==WIN_EQUAL) glDrawBuffer(GL_FRONT_AND_BACK);
				else {
					glDrawBuffer(GL_FRONT);
					sa->win_swap= WIN_FRONT_OK;
				}
				draw_object(base);
				
				#endif
			}
		}
		sa= sa->next;
	}
	if(curarea!=tempsa) areawinset(tempsa->win);
	
	G.f &= ~G_DRAW_EXT;

	glDrawBuffer(GL_BACK);
	
	if(G.zbuf) {
		G.zbuf= 0;
		glDisable(GL_DEPTH_TEST);
	}
}

