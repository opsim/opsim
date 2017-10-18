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

/* ***************************************




    raddisplay.c	nov/dec 1992
					may 1999

    - drawing
    - color calculation for display during solving

 *************************************** */

#include "radio.h"


char calculatecolor(float col)
{
	int b;

	if(RG.gamma==1.0) {
		b= RG.radfactor*col;
	}
	else if(RG.gamma==2.0) {
		b= RG.radfactor*fsqrt(col);
	}
	else {
		b= RG.radfactor*powf(col, RG.igamma);
	}
	
	if(b>255) b=255;
	return b;
}

void make_node_display()
{
	RNode *rn, **el;
	int a;
	char *charcol;

	RG.igamma= 1.0/RG.gamma;
	RG.radfactor= RG.radfac*powf(64*64, RG.igamma);

	el= RG.elem;
	for(a=RG.totelem; a>0; a--, el++) {
		rn= *el;
		charcol= (char *)&( rn->col );

		charcol[3]= calculatecolor(rn->totrad[0]);
		charcol[2]= calculatecolor(rn->totrad[1]);
		charcol[1]= calculatecolor(rn->totrad[2]);
		
		/* gouraudcolor */
		*(rn->v1+3)= 0;
		*(rn->v2+3)= 0;
		*(rn->v3+3)= 0;
		if(rn->v4) *(rn->v4+3)= 0;
	}
	
	el= RG.elem;
	for(a=RG.totelem; a>0; a--, el++) {
		rn= *el;
		addaccuweight( (char *)&(rn->col), (char *)(rn->v1+3), 16 );
		addaccuweight( (char *)&(rn->col), (char *)(rn->v2+3), 16 );
		addaccuweight( (char *)&(rn->col), (char *)(rn->v3+3), 16 );
		if(rn->v4) addaccuweight( (char *)&(rn->col), (char *)(rn->v4+3), 16 );
	}
}

void drawnodeWire(RNode *rn)
{

	if(rn->down1) {
		drawnodeWire(rn->down1);
		drawnodeWire(rn->down2);
	}
	else {
		glBegin(GL_LINE_LOOP);
			glVertex3fv(rn->v1);
			glVertex3fv(rn->v2);
			glVertex3fv(rn->v3);
			if(rn->type==4) glVertex3fv(rn->v4);
		glEnd();
	}
}

void drawsingnodeWire(RNode *rn)
{
	
	glBegin(GL_LINE_LOOP);
		glVertex3fv(rn->v1);
		glVertex3fv(rn->v2);
		glVertex3fv(rn->v3);
		if(rn->type==4) glVertex3fv(rn->v4);
	glEnd();
}

void drawnodeSolid(RNode *rn)
{
	char *cp;
	
	if(rn->down1) {
		drawnodeSolid(rn->down1);
		drawnodeSolid(rn->down2);
	}
	else {
		cp= (char *)&rn->col;
		glColor3ub(cp[3], cp[2], cp[1]);
		glBegin(GL_POLYGON);
			glVertex3fv(rn->v1);
			glVertex3fv(rn->v2);
			glVertex3fv(rn->v3);
			if(rn->type==4) glVertex3fv(rn->v4);
		glEnd();
	}
}

void drawnodeGour(RNode *rn)
{
	char *cp;
	
	if(rn->down1) {
		drawnodeGour(rn->down1);
		drawnodeGour(rn->down2);
	}
	else {
		glBegin(GL_POLYGON);
			cp= (char *)(rn->v1+3);
			glColor3ub(cp[3], cp[2], cp[1]);
			glVertex3fv(rn->v1);
			
			cp= (char *)(rn->v2+3);
			glColor3ub(cp[3], cp[2], cp[1]);
			glVertex3fv(rn->v2);
			
			cp= (char *)(rn->v3+3);
			glColor3ub(cp[3], cp[2], cp[1]);
			glVertex3fv(rn->v3);
			
			if(rn->type==4) {
				cp= (char *)(rn->v4+3);
				glColor3ub(cp[3], cp[2], cp[1]);
				glVertex3fv(rn->v4);
			}
		glEnd();
	}
}

void drawpatch_ext(RPatch *patch, uint col)
{
	ScrArea *sa, *oldsa;
	
	glDrawBuffer(GL_FRONT);
	if(G.zbuf) glDisable(GL_DEPTH_TEST);

	cpack(col);

	oldsa= curarea;

	sa= G.curscreen->areabase.first;
	while(sa) {
		if (sa->spacetype==SPACE_VIEW3D) {
		 	if(sa->win && sa->windraw) {
			 	/* hier winget() gebruiken: anders wordt in header getekend */
			 	if(sa->win != winget()) areawinset(sa->win);
				drawnodeWire(patch->first);
			}
		}
		sa= sa->next;
	}

	if(oldsa && oldsa!=curarea) areawinset(oldsa->win);

	glDrawBuffer(GL_BACK);
	if(G.zbuf) glEnable(GL_DEPTH_TEST);
}

void drawnode_ext(RNode *rn, uint col)
{
	
	glDrawBuffer(GL_FRONT);
	if(G.zbuf) glDisable(GL_DEPTH_TEST);

	cpack(col);
	
	drawnodeWire(rn);

	glDrawBuffer(GL_BACK);
	if(G.zbuf) glEnable(GL_DEPTH_TEST);
}

void drawOverflowElem()
{
	RNode **el, *rn;
	float *fp;
	int a;

	glDrawBuffer(GL_FRONT);
	if(G.zbuf) glDisable(GL_DEPTH_TEST);

	cpack(0xFF9900);

	el= RG.elem;
	fp= RG.formfactors;
	for(a=0; a<RG.totelem; a++, el++, fp++) {
		if(*fp>1.0) {
			rn= *el;
			glBegin(GL_LINE_LOOP);
				glVertex3fv( rn->v1);
				glVertex3fv( rn->v2);
				glVertex3fv( rn->v3);
				if(rn->type==4) glVertex3fv( rn->v4);
			glEnd();
		}
	}

	glDrawBuffer(GL_BACK);
	if(G.zbuf) glEnable(GL_DEPTH_TEST);
}

void drawfaceGour(Face *face)
{
	char *cp;
	
	glBegin(GL_POLYGON);
		cp= (char *)(face->v1+3);
		glColor3ub(cp[3], cp[2], cp[1]);
		glVertex3fv(face->v1);
		
		cp= (char *)(face->v2+3);
		glColor3ub(cp[3], cp[2], cp[1]);
		glVertex3fv(face->v2);
		
		cp= (char *)(face->v3+3);
		glColor3ub(cp[3], cp[2], cp[1]);
		glVertex3fv(face->v3);
		
		if(face->v4) {
			cp= (char *)(face->v4+3);
			glColor3ub(cp[3], cp[2], cp[1]);
			glVertex3fv(face->v4);
		}
	glEnd();
	
}

void drawfaceSolid(Face *face)
{
	char *cp;
	
	cp= (char *)&face->col;
	glColor3ub(cp[3], cp[2], cp[1]);
	
	glBegin(GL_POLYGON);
		glVertex3fv(face->v1);
		glVertex3fv(face->v2);
		glVertex3fv(face->v3);
		if(face->v4) {
			glVertex3fv(face->v4);
		}
	glEnd();
	
}

void drawfaceWire(Face *face)
{
	char *cp;
	
	cp= (char *)&face->col;
	glColor3ub(cp[3], cp[2], cp[1]);

	glBegin(GL_LINE_LOOP);
		glVertex3fv(face->v1);
		glVertex3fv(face->v2);
		glVertex3fv(face->v3);
		if(face->v4) {
			glVertex3fv(face->v4);
		}
	glEnd();
	
}

void drawsquare(float *cent, float size, short cox, short coy)
{
	float vec[3];	

	VECCOPY(vec, cent);
	
	glBegin(GL_LINE_LOOP);
		vec[cox]+= .5*size;
		vec[coy]+= .5*size;
		glVertex3fv(vec);	
		vec[coy]-= size;
		glVertex3fv(vec);
		vec[cox]-= size;
		glVertex3fv(vec);
		vec[coy]+= size;
		glVertex3fv(vec);
	glEnd();	
}

void drawlimits()
{
	/* centreer rond cent */
	short cox=0, coy=1;
	
	if((RG.flag & 3)==2) coy= 2;
	if((RG.flag & 3)==3) {
		cox= 1;	
		coy= 2;	
	}
	
	cpack(0);
	drawsquare(RG.cent, fsqrt(RG.patchmax), cox, coy);
	drawsquare(RG.cent, fsqrt(RG.patchmin), cox, coy);

	drawsquare(RG.cent, fsqrt(RG.elemmax), cox, coy);
	drawsquare(RG.cent, fsqrt(RG.elemmin), cox, coy);

	cpack(0xFFFFFF);
	drawsquare(RG.cent, fsqrt(RG.patchmax), cox, coy);
	drawsquare(RG.cent, fsqrt(RG.patchmin), cox, coy);
	cpack(0xFFFF00);
	drawsquare(RG.cent, fsqrt(RG.elemmax), cox, coy);
	drawsquare(RG.cent, fsqrt(RG.elemmin), cox, coy);
	
}

void setcolNode(RNode *rn, uint *col)
{

	if(rn->down1) {
		 setcolNode(rn->down1, col);
		 setcolNode(rn->down2, col);
	}
	rn->col= *col;
	
	*((uint *)rn->v1+3)= *col;
	*((uint *)rn->v2+3)= *col;
	*((uint *)rn->v3+3)= *col;
	if(rn->v4) *((uint *)rn->v4+3)= *col;
}

void pseudoAmb()
{
	RPatch *rp;
	float fac;
	char col[4];
	
	/* zet pseudo ambient kleuren in de nodes */

	rp= RG.patchbase.first;
	while(rp) {

		if(rp->emit[0]!=0.0 || rp->emit[1]!=0.0 || rp->emit[2]!=0.0) {
			col[1]= col[2]= col[3]= 255;
		}
		else {
			fac= rp->norm[0]+ rp->norm[1]+ rp->norm[2];
			fac= 225.0*(3+fac)/6.0;
			
			col[3]= fac*rp->ref[0];
			col[2]= fac*rp->ref[1];
			col[1]= fac*rp->ref[2];
		}
		
		setcolNode(rp->first, (uint *)col);
		
		rp= rp->next;
	}
}

void rad_drawall()
{
	/* maakt afbeelding van elements of van faces */
	Face *face;
	RNode **el;
	RPatch *rp;
	int a;

	if(G.vd->drawtype<OB_SOLID) {
		glEnable(GL_DEPTH_TEST);
		glClearDepth(1.0); glClear(GL_DEPTH_BUFFER_BIT);
	}
	
	if(RG.totface) {
		if(RG.drawtype==DTGOUR) {
			glShadeModel(GL_SMOOTH);
			for(a=0; a<RG.totface; a++) {
				RAD_NEXTFACE(a);
				
				drawfaceGour(face);
			}
		}
		else if(RG.drawtype==DTSOLID) {
			for(a=0; a<RG.totface; a++) {
				RAD_NEXTFACE(a);
				
				drawfaceSolid(face);
			}
		}
		else {
			if(!(get_qual()&LR_SHIFTKEY)) {

				for(a=0; a<RG.totface; a++) {
					RAD_NEXTFACE(a);
					
					drawfaceWire(face);
				}
			}
			else {
				cpack(0);
				rp= RG.patchbase.first;
				while(rp) {
					drawsingnodeWire(rp->first);
					rp= rp->next;
				}
			}
		}
	}
	else {
		el= RG.elem;
		if(RG.drawtype==DTGOUR) {
			glShadeModel(GL_SMOOTH);
			for(a=RG.totelem; a>0; a--, el++) {
				drawnodeGour(*el);
			}
		}
		else if(RG.drawtype==DTSOLID) {
			for(a=RG.totelem; a>0; a--, el++) {
				drawnodeSolid(*el);
			}
		}
		else {
			cpack(0);
			for(a=RG.totelem; a>0; a--, el++) {
				drawnodeWire(*el);
			}
		}
	}
	glShadeModel(GL_FLAT);
	
	if(RG.totpatch) {
		if(RG.flag & 3) {
			if(G.vd->drawtype>=OB_SOLID) glDisable(GL_DEPTH_TEST);
			drawlimits();
			if(G.vd->drawtype>=OB_SOLID) glEnable(GL_DEPTH_TEST);
		}
	}	
	if(G.vd->drawtype<OB_SOLID) {
		glDisable(GL_DEPTH_TEST);
	}
}

void rad_forcedraw()
{
 	ScrArea *sa, *oldsa;
	
	oldsa= curarea;

	sa= G.curscreen->areabase.first;
	while(sa) {
		if (sa->spacetype==SPACE_VIEW3D) {
		 	if(sa->win && sa->windraw) {
			 	/* hier winget() gebruiken: anders wordt in header getekend */
			 	if(sa->win != winget()) areawinset(sa->win);
			 	sa->windraw();
			}
		}
		sa= sa->next;
	}
	screen_swapbuffers();
	
	if(oldsa && oldsa!=curarea) areawinset(oldsa->win);
}


