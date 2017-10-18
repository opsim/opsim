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


/*  drawsector.c     GRAPHICS
 * 
 *  maart 96
 *  
 * 
 */

#ifndef FREE

#include "blender.h"
#include "graphics.h"
#include "sector.h"

#if defined(GL_EXT_texture_object)
#define glBindTexture(A,B)     glBindTextureEXT(A,B)
#define glGenTextures(A,B)     glGenTexturesEXT(A,B)
#define glDeleteTextures(A,B)  glDeleteTexturesEXT(A,B)
#define glPolygonOffset(A,B)  glPolygonOffsetEXT(A,B)

#else

#define GL_FUNC_ADD_EXT					GL_FUNC_ADD
/* #define GL_FUNC_REVERSE_SUBTRACT_EXT	GL_FUNC_REVERSE_SUBTRACT */
/* #define GL_POLYGON_OFFSET_EXT			GL_POLYGON_OFFSET */

#endif

float texmat1[4][4], texmat4[4][4];

Image *curpage=0;
int curtile=0, curmode=0;
short texwindx, texwindy, texwinsx, texwinsy;

int source, dest;


void copy_part_from_ibuf(ImBuf *ibuf, uint *rect, short startx, short starty, short endx, short endy)
{
	uint *rt, *rp;
	short y, heigth, len;

	/* de juiste offset in rectot */

	rt= ibuf->rect+ (starty*ibuf->x+ startx);

	len= (endx-startx);
	heigth= (endy-starty);

	rp=rect;
	
	for(y=0; y<heigth; y++) {
		memcpy(rp, rt, len*4);
		rt+= ibuf->x;
		rp+= len;
	}

}

void free_realtime_image(Image *ima)
{
	if(ima->bindcode) {
		glDeleteTextures(1, &ima->bindcode);
		ima->bindcode= 0;
	}
	if(ima->repbind) {
		glDeleteTextures(ima->totbind, ima->repbind);
	
		freeN(ima->repbind);
		ima->repbind= 0;
	}
}

void make_repbind(Image *ima)
{
	int a;
	
	if(ima==0 || ima->ibuf==0) return;

	if(ima->repbind) {
		glDeleteTextures(ima->totbind, ima->repbind);
		freeN(ima->repbind);
		ima->repbind= 0;
	}
	ima->totbind= ima->xrep*ima->yrep;
	if(ima->totbind>1) {
		ima->repbind= callocN(sizeof(int)*ima->totbind, "repbind");
	}
	
}

int set_tpage(TFace *tface)
{	
	static int alphamode= -1;
	static TFace *lasttface= 0;
	Image *ima;
	float mat[4][4];
	uint *rect, *bind;
	int tpx, tpy, tilemode, mode;
	
	/* afschakelen */
	if(tface==0) {
		if(lasttface==0) return 0;
		
		lasttface= 0;
		curtile= 0;
		curpage= 0;
		if(curmode!=0) {
			glMatrixMode(GL_TEXTURE);
			glLoadMatrixf(texmat1);
			glMatrixMode(GL_MODELVIEW);
		}
		curmode= 0;
		alphamode= -1;
		
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		return 0;
	}
	lasttface= tface;

	if( alphamode != tface->transp) {
		alphamode= tface->transp;

		if(alphamode) {
			glEnable(GL_BLEND);
			
			if(alphamode==TF_ADD) {
				glBlendFunc(GL_ONE, GL_ONE);
			/* 	glBlendEquationEXT(GL_FUNC_ADD_EXT); */
			}
			else if(alphamode==TF_ALPHA) {
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			/* 	glBlendEquationEXT(GL_FUNC_ADD_EXT); */
			}
			/* else { */
			/* 	glBlendFunc(GL_ONE, GL_ONE); */
			/* 	glBlendEquationEXT(GL_FUNC_REVERSE_SUBTRACT_EXT); */
			/* } */
		}
		else glDisable(GL_BLEND);
	}

	ima= tface->tpage;

	tilemode= tface->mode & TF_TILES;

	if(ima==curpage && curtile==tface->tile && tilemode==curmode ) return ima!=0;

	if(tilemode!=curmode) {
		
		glMatrixMode(GL_TEXTURE);
		
		if(tilemode) {
			glLoadMatrixf(texmat4);
		}
		else glLoadMatrixf(texmat1);

		glMatrixMode(GL_MODELVIEW);
		
	}

	if(ima==0 || ima->ok==0) {
		glDisable(GL_TEXTURE_2D);
		
		curtile= tface->tile;
		curpage= 0;
		curmode= tilemode;

		return 0;
	}

	if(ima->ibuf==0) {
		char str[256];
		
		strcpy(str, ima->name);
		convertstringcode(str);

		ima->ibuf = loadiffname(str , LI_rect);
		
		if(ima->ibuf==0) {
			ima->ok= 0;

			curtile= tface->tile;
			curpage= 0;
			curmode= tilemode;
			
			glDisable(GL_TEXTURE_2D);
			return 0;
		}
		
	}

	if(ima->tpageflag & IMA_TWINANIM) curtile= ima->lastframe;
	else curtile= tface->tile;

	if(tilemode) {
		if(ima->repbind==0) make_repbind(ima);
		
		if(curtile>=ima->totbind) curtile= 0;
		
		/* this happens when you change repeat buttons */
		if(ima->repbind) bind= ima->repbind+curtile;
		else bind= &ima->bindcode;
		
		if(*bind==0) {
			
			texwindx= ima->ibuf->x/ima->xrep;
			texwindy= ima->ibuf->y/ima->yrep;
			
			if(curtile>=ima->xrep*ima->yrep) curtile= ima->xrep*ima->yrep-1;
	
			texwinsy= curtile / ima->xrep;
			texwinsx= curtile - texwinsy*ima->xrep;
	
			texwinsx*= texwindx;
			texwinsy*= texwindy;
	
			tpx= texwindx;
			tpy= texwindy;
			rect= ima->ibuf->rect + texwinsy*ima->ibuf->x + texwinsx;
		}
	}
	else {
		bind= &ima->bindcode;
		
		if(*bind==0) {
			tpx= ima->ibuf->x;
			tpy= ima->ibuf->y;
			rect= ima->ibuf->rect;
		}
	}

	if(*bind==0) {
		glGenTextures(1, bind);
		
		if(G.f & G_DEBUG) {
			PRINT(s, ima->id.name+2);
			PRINT2(d, d, *bind, tpx);
			PRINT2(d, d, curtile, tilemode);
		}
		glBindTexture( GL_TEXTURE_2D, *bind);

		if(tilemode) glPixelStorei(GL_UNPACK_ROW_LENGTH,  ima->ibuf->x);
		glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA,  tpx,  tpy, 0, GL_RGBA, GL_UNSIGNED_BYTE, rect);
		if(tilemode) glPixelStorei(GL_UNPACK_ROW_LENGTH,  0);
		
		/* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */
		/* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); */

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		
	}
	else glBindTexture( GL_TEXTURE_2D, *bind);
	
	glEnable(GL_TEXTURE_2D);

	curpage= ima;
	curmode= tilemode;

	return 1;
}


void get_co_portal(Sector *se, int type, float *ofs, float *cent)
{
	
	cent[0]= cent[1]= cent[2]= 0.0;
	
	if(type==PO_XPOS || type==PO_XNEG) {
		if(type==PO_XPOS) cent[0]= 0.98*se->size[0];
		else cent[0]= -0.98*se->size[0];
		
		cent[1]+= ofs[0];
		cent[2]+= ofs[1];

	}
	else if(type==PO_YPOS || type==PO_YNEG) {
		if(type==PO_YPOS) cent[1]= 0.98*se->size[1];
		else cent[1]= -0.98*se->size[1];

		cent[0]+= ofs[0];
		cent[2]+= ofs[1];

	}
	else if(type==PO_ZPOS || type==PO_ZNEG) {
		if(type==PO_ZPOS) cent[2]= 0.98*se->size[2];
		else cent[2]= -0.98*se->size[2];

		cent[0]+= ofs[0];
		cent[1]+= ofs[1];

	}
}

void draw_portal(Sector *se, Portal *po, int act)
{
	float cent[3], size, veci[3], vecj[3], col[4];
	int cox, coy;
	
	size= 0.1*MAX3(se->size[0], se->size[1], se->size[2]);
	if(size>0.5) size= 0.5;
	
	get_co_portal(se, po->type, po->ofs, cent);
	
	if(po->type==PO_XPOS || po->type==PO_XNEG) {
		cox= 1; coy= 2;
	}
	else if(po->type==PO_YPOS || po->type==PO_YNEG) {
		cox= 0; coy= 2;
	}
	else {
		cox= 0; coy= 1;
	}

	glGetFloatv(GL_CURRENT_COLOR, col);
	
	if(G.f & G_BACKBUFSEL); else {
		if(po->sector) glColor3ub(0,  255,  0);
		else glColor3ub(0,  0,  0);
	}
	
	cent[cox]+= 0.05;
	cent[coy]-= 0.05;

	glBegin(GL_POLYGON);
		glVertex3fv(cent);
		cent[coy]+= size;
		glVertex3fv(cent);
		cent[cox]-= size;
		glVertex3fv(cent);
		cent[coy]-= size;
		glVertex3fv(cent);
		cent[cox]+= size;
	glEnd();
	
	if(G.f & G_BACKBUFSEL) return;

	if(act) glColor3ub(255,  255,  255);
	else if(po->sector) glColor3ub(225,  200,  0);
	else if(col[0]!=0.0) glColor3ub(150,  50,  150);
	else glColor3ub(50,  50,  50);
	
	glBegin(GL_LINE_LOOP);
		glVertex3fv(cent);
		cent[coy]+= size;
		glVertex3fv(cent);
		cent[cox]-= size;
		glVertex3fv(cent);
		cent[coy]-= size;
		glVertex3fv(cent);
		cent[cox]+= size;
	glEnd();

	glColor3f(col[0],  col[1],  col[2]);
}

void spack(uint ucol)
{
	char *cp= (char *)&ucol;
	
	glColor3ub(cp[3], cp[2], cp[1]);
}

void draw_hide_tfaces(Object *ob, Mesh *me)
{
	TFace *tface;
	MFace *mface;
	float *v1, *v2, *v3, *v4;
	int a;
	
	if(me==0 || me->tface==0) return;

	mface= me->mface;
	tface= me->tface;

	cpack(0x0);
	setlinestyle(1);
	for(a=me->totface; a>0; a--, mface++, tface++) {
		if(mface->v3==0) continue;
		
		if( (tface->flag & TF_HIDE)) {

			v1= (me->mvert+mface->v1)->co;
			v2= (me->mvert+mface->v2)->co;
			v3= (me->mvert+mface->v3)->co;
			if(mface->v4) v4= (me->mvert+mface->v4)->co; else v4= 0;
		
			glBegin(GL_LINE_LOOP);
				glVertex3fv( v1 );
				glVertex3fv( v2 );
				glVertex3fv( v3 );
				if(mface->v4) glVertex3fv( v4 );
			glEnd();			
		}
	}
	setlinestyle(0);
}


void draw_tfaces3D(Object *ob, Mesh *me)
{
	MFace *mface;
	TFace *tface;
	float *v1, *v2, *v3, *v4;
	int a;
	
	if(me==0 || me->tface==0) return;

	glDisable(GL_DEPTH_TEST);

	mface= me->mface;
	tface= me->tface;
	
	/* SELECT faces */
	for(a=me->totface; a>0; a--, mface++, tface++) {
		if(mface->v3==0) continue;
		if(tface->flag & ACTIVE) continue;
		if(tface->flag & TF_HIDE) continue;
		
		if(tface->flag & SELECT) {
		
			v1= (me->mvert+mface->v1)->co;
			v2= (me->mvert+mface->v2)->co;
			v3= (me->mvert+mface->v3)->co;
			if(mface->v4) v4= (me->mvert+mface->v4)->co; else v4= 0;
		
			cpack(0x0);
			glBegin(GL_LINE_LOOP);
				glVertex3fv( v1 );
				glVertex3fv( v2 );
				glVertex3fv( v3 );
				if(v4) glVertex3fv( v4 );
			glEnd();

			cpack(0xFFFFFF);
			setlinestyle(1);
			glBegin(GL_LINE_LOOP);
				glVertex3fv( v1 );
				glVertex3fv( v2 );
				glVertex3fv( v3 );
				if(v4) glVertex3fv( v4 );
			glEnd();
			setlinestyle(0);
		}
	}
	
	mface= me->mface;
	tface= me->tface;
	
	/* ACTIVE faces */
	for(a=me->totface; a>0; a--, mface++, tface++) {
		if(mface->v3==0) continue;
		
		if( (tface->flag & ACTIVE)  && (tface->flag & SELECT) ) {

			v1= (me->mvert+mface->v1)->co;
			v2= (me->mvert+mface->v2)->co;
			v3= (me->mvert+mface->v3)->co;
			if(mface->v4) v4= (me->mvert+mface->v4)->co; else v4= 0;
		
			/* kleuren: R=x G=y */
			cpack(0xFF);
			glBegin(GL_LINE_STRIP); glVertex3fv(v1); if(v4) glVertex3fv(v4); else glVertex3fv(v3); glEnd();
			cpack(0xFF00);
			glBegin(GL_LINE_STRIP); glVertex3fv(v1); glVertex3fv(v2); glEnd();
			cpack(0x0);
			glBegin(GL_LINE_STRIP); glVertex3fv(v2); glVertex3fv(v3); if(v4) glVertex3fv(v4); glEnd();
			
			cpack(0xFFFFFF);
			setlinestyle(1);
			glBegin(GL_LINE_LOOP);
				glVertex3fv( v1 );
				glVertex3fv( v2 );
				glVertex3fv( v3 );
				if(mface->v4) glVertex3fv( v4 );
			glEnd();

			setlinestyle(0);
		}
	}

	glEnable(GL_DEPTH_TEST);
}

float rvecmat[3][3], rcolmat[3][3];
short *punopoin[4];

int do_realtimelight(Object *ob, TFace *tface, float *col)
{
	static Object *lampar[24];
	static int totlamp=0, curlamp=0, need_init=0;
	Lamp *la;
	Life *lf;
	float inp;
	uint lay;
	int a;
	char *cp;
	
	if(G.vd==0) return 0;
	if(G.vd->localvd) lay= G.vd->localvd->lay;
	else lay= G.vd->lay;
	
	if(ob==0) {
		need_init= 1;
		return 0;
	}
	
	if(ob && need_init) {
		Base *base=FIRSTBASE;

		/* lamp array aanlegen */
		totlamp= 0;
		while(base) {
			if(base->lay & lay) {
				if(base->object->type==OB_LAMP) {
					la= base->object->data;
					if(la->type==LA_SUN) {
						lampar[totlamp]= base->object;
						totlamp++;
						if(totlamp>=24) break;
					}
				}
				else if ELEM(base->object->type, OB_SECTOR, OB_LIFE) {
					/* imat nodig voor lampberekening */
					where_is_object(base->object);
					Mat4Invert(base->object->imat, base->object->obmat);
				}
			}
			base= base->next;
		}
		need_init= 0;
	}
	
	if(totlamp==0) return 0;
	if(G.f & G_VERTEXPAINT) return 0;
		
	if(tface==0) {
		
		/* init matrices */
		curlamp= 0;
		for(a=0; a<totlamp; a++) {
			if(lampar[a]->lay & ob->lay) {
				la= lampar[a]->data;
				
				rvecmat[curlamp][0]= lampar[a]->obmat[2][0];
				rvecmat[curlamp][1]= lampar[a]->obmat[2][1];
				rvecmat[curlamp][2]= lampar[a]->obmat[2][2];

				Mat4Mul3Vecfl(ob->imat, rvecmat[curlamp]);
				
				Normalise(rvecmat[curlamp]);
				
				/* deze schaal wordt terug gecorrigeerd in writeblendpsx */
				rcolmat[curlamp][0]= la->energy*la->r/32767.0;
				rcolmat[curlamp][1]= la->energy*la->g/32767.0;
				rcolmat[curlamp][2]= la->energy*la->b/32767.0;
				
				curlamp++;
				if(curlamp>=3) break;
			}
		}
		return curlamp;
	}
	
	if(curlamp==0) return 0;
	
	/* voorlopig (even) alleen solid */
	bzero(col, 48);
	cp= (char *)tface->col;
	a= curlamp;

	if(tface->mode & TF_SHAREDCOL) {
		while(a--) {
	
			inp= INPR(punopoin[0], rvecmat[a])/255.0;
			if(inp > 0.0) {
				col[0]+= inp*rcolmat[a][0]*cp[3];
				col[1]+= inp*rcolmat[a][1]*cp[2];
				col[2]+= inp*rcolmat[a][2]*cp[1];
			}
			inp= INPR(punopoin[1], rvecmat[a])/255.0;
			if(inp > 0.0) {
				col[3]+= inp*rcolmat[a][0]*cp[7];
				col[4]+= inp*rcolmat[a][1]*cp[6];
				col[5]+= inp*rcolmat[a][2]*cp[5];
			}
			inp= INPR(punopoin[2], rvecmat[a])/255.0;
			if(inp > 0.0) {
				col[6]+= inp*rcolmat[a][0]*cp[11];
				col[7]+= inp*rcolmat[a][1]*cp[10];
				col[8]+= inp*rcolmat[a][2]*cp[9];
			}
			if(punopoin[3]) {
				inp= INPR(punopoin[3], rvecmat[a])/255.0;
				if(inp > 0.0) {
					col[9]+= inp*rcolmat[a][0]*cp[15];
					col[10]+= inp*rcolmat[a][1]*cp[14];
					col[11]+= inp*rcolmat[a][2]*cp[13];
				}
			}

		}
	}
	else {
		while(a--) {
	
			inp= INPR(tface->no, rvecmat[a])/255.0;
			
			if(inp > 0.0) {
				col[0]+= inp*rcolmat[a][0]*cp[3];
				col[1]+= inp*rcolmat[a][1]*cp[2];
				col[2]+= inp*rcolmat[a][2]*cp[1];
				
				if(tface->mode & TF_GOUR) {
					col[3]+= inp*rcolmat[a][0]*cp[7];
					col[4]+= inp*rcolmat[a][1]*cp[6];
					col[5]+= inp*rcolmat[a][2]*cp[5];
	
					col[6]+= inp*rcolmat[a][0]*cp[11];
					col[7]+= inp*rcolmat[a][1]*cp[10];
					col[8]+= inp*rcolmat[a][2]*cp[9];
	
					col[9]+= inp*rcolmat[a][0]*cp[15];
					col[10]+= inp*rcolmat[a][1]*cp[14];
					col[11]+= inp*rcolmat[a][2]*cp[13];
				}
			}
		}
	}
	return 1;
}

typedef struct tra_ob {
	struct tra_ob *next, *prev;
	Object *ob;
	Mesh *me;
	int dt;
} tra_ob;

#define MAX_TRA_OB	64

static int tot_tra_ob=0;

static tra_ob tra_ob_ar[MAX_TRA_OB];

void add_tra_object(Object *ob, Mesh *me, int dt)
{
	if(tot_tra_ob>=MAX_TRA_OB) return;
	tra_ob_ar[tot_tra_ob].ob= ob;
	tra_ob_ar[tot_tra_ob].me= me;
	tra_ob_ar[tot_tra_ob].dt= dt;
	tot_tra_ob++;
}

void draw_tface_mesh(Object *ob, Mesh *me, int dt)	/* maximum dt: precies volgens ingestelde waardes */
{
	Life *lf;
	TFace *tface;
	MFace *mface;
	MVert *mvert;
	Image *ima;
	float *v1, *v2, *v3, *v4, col[4][3], coli[4];
	uint obcol;
	int a, mode;
	short islight, istex, istra=0;
	char *cp;
	
	if(me==0) return;
	if(ob->type==OB_LIFE) lf= ob->data;
	else lf= 0;
	
	glShadeModel(GL_SMOOTH);

	glGetFloatv(GL_CURRENT_COLOR, coli);

	/* als meshes uit lib gelezen zijn en alleen mcol hebben: */
	if(me->tface==0) make_tfaces(me);

	islight= do_realtimelight(ob, 0, 0);

	/* de ob color */
	if(ob->type==OB_SECTOR) {
		Sector *se= ob->data;
		obcol= rgb_to_mcol(se->r, se->g, se->b);
	}
	else if(ob->type==OB_LIFE) {
		Life *lf= ob->data;
		obcol= rgb_to_mcol(lf->r, lf->g, lf->b);
	}
	/* eerst alle texture polys */
	
	glCullFace(GL_BACK); glEnable(GL_CULL_FACE);
	if(G.vd->drawtype==OB_TEXTURE) istex= 1;
	else istex= 0;

	/* signal to NOT draw transparant separate */
	if((G.f & G_SIMULATION)==0) istra= 2;

	if(dt > OB_SOLID) {
		
		mface= me->mface;
		tface= me->tface;
		
		for(a=me->totface; a>0; a--, mface++, tface++) {
			if(mface->v3==0) continue;
			if(tface->flag & TF_HIDE) continue;
			
			if(istex && tface->transp) {
				if(istra==0) {
					add_tra_object(ob, me, dt);
					istra= 1;
				}
				if(istra==1) continue;
			}
			
			mode= tface->mode;

			if(mode & TF_OBCOL) tface->col[0]= obcol;
			
			v1= (me->mvert+mface->v1)->co;
			v2= (me->mvert+mface->v2)->co;
			v3= (me->mvert+mface->v3)->co;
			if(mface->v4) v4= (me->mvert+mface->v4)->co; else v4= 0;
			
			if(mode & TF_SHAREDCOL) {
				punopoin[0]= (short *)(v1+3);
				punopoin[1]= (short *)(v2+3);
				punopoin[2]= (short *)(v3+3);
				if(v4) punopoin[3]= (short *)(v4+3);
				else punopoin[3]= 0;
			}

			if(mode & TF_TWOSIDE) {
				glDisable(GL_CULL_FACE);
			}
			else {
				glEnable(GL_CULL_FACE);
			}

			if(istex && (mode & TF_TEX) ) {
				
				/* in set_tpage worden dingen gedaan die niet binnen een bgnpolygon mogen liggen */
				
				if( set_tpage(tface) ) {
					
					if(islight && (mode & TF_LIGHT)) {
						do_realtimelight(ob, tface, col[0]);
						
						glBegin(GL_POLYGON);
						
						glTexCoord2sv(tface->uv[0]);

						glColor3fv(col[0]);
						glVertex3fv(v1);
						
						glTexCoord2sv(tface->uv[1]);
						if(mode & TF_GOUR) glColor3fv(col[1]);
						glVertex3fv(v2);
			
						glTexCoord2sv(tface->uv[2]);
						if(mode & TF_GOUR) glColor3fv(col[2]);
						glVertex3fv(v3);
			
						if(v4) {
							glTexCoord2sv(tface->uv[3]);
							if(mode & TF_GOUR) glColor3fv(col[3]);
							glVertex3fv(v4);
						}
						glEnd();
					}
					else {
						glBegin(GL_POLYGON);
						
						glTexCoord2sv(tface->uv[0]);

						spack(tface->col[0]);
						glVertex3fv(v1);
						
						glTexCoord2sv(tface->uv[1]);
						if(mode & TF_GOUR) spack(tface->col[1]);
						glVertex3fv(v2);
			
						glTexCoord2sv(tface->uv[2]);
						if(mode & TF_GOUR) spack(tface->col[2]);
						glVertex3fv(v3);
			
						if(v4) {
							glTexCoord2sv(tface->uv[3]);
							if(mode & TF_GOUR) spack(tface->col[3]);
							glVertex3fv(v4);
						}
						glEnd();
					}
				}
				else {
					/* waarschuwings polygoon */

					glBegin(GL_POLYGON);
					cpack(0xFF00FF);
					glVertex3fv(v1);
					glVertex3fv(v2);
					glVertex3fv(v3);
					if(v4) glVertex3fv(v4);
					glEnd();
				}
			}
			else {
				
				set_tpage(0);
				
				glBegin(GL_POLYGON);
				
				if(islight && (tface->mode & TF_LIGHT)) {
					do_realtimelight(ob, tface, col[0]);
					
					glColor3fv(col[0]);
					glVertex3fv(v1);
					if(mode & TF_GOUR) {
						glColor3fv(col[1]);
						glVertex3fv(v2);
						glColor3fv(col[2]);
						glVertex3fv(v3);
						if(v4) {glColor3fv(col[3]); glVertex3fv(v4);}
					}
					else {
						glVertex3fv(v2);
						glVertex3fv(v3);
						if(v4) glVertex3fv(v4);
					}
				}
				else {

					cp= (char *)&(tface->col[0]);
					glColor3ub(cp[3], cp[2], cp[1]);
					
					glVertex3fv(v1);
					
					if(mode & TF_GOUR) {
						glColor3ub(cp[7], cp[6], cp[5]);
						glVertex3fv(v2);
						
						glColor3ub(cp[11], cp[10], cp[9]);
						glVertex3fv(v3);
						if(v4) {
							glColor3ub(cp[15], cp[14], cp[13]);
							glVertex3fv(v4);
						}
					}
					else {
						glVertex3fv(v2);
						glVertex3fv(v3);
						if(v4) glVertex3fv(v4);
					}
					
				}
				glEnd();
			}
		}
		
		/* textures uitzetten */
		set_tpage(0);

	}
	else {
		
		/* alle niet-texture polys */
		
		mface= me->mface;
		tface= me->tface;
	
		for(a=me->totface; a>0; a--, mface++, tface++) {
			if(mface->v3==0) continue;
			if(tface->flag & TF_HIDE) continue;
			
			mode= tface->mode;
			if(tface->mode & TF_TWOSIDE) {
				glDisable(GL_CULL_FACE);
			}
			else {
				glEnable(GL_CULL_FACE);
			}

			if(mode & TF_OBCOL) tface->col[0]= obcol;

			v1= (me->mvert+mface->v1)->co;
			v2= (me->mvert+mface->v2)->co;
			v3= (me->mvert+mface->v3)->co;
			if(mface->v4) v4= (me->mvert+mface->v4)->co; else v4= 0;
			
			if(mode & TF_SHAREDCOL) {
				punopoin[0]= (short *)(v1+3);
				punopoin[1]= (short *)(v2+3);
				punopoin[2]= (short *)(v3+3);
				if(v4) punopoin[3]= (short *)(v4+3);
				else punopoin[3]= 0;
			}
			
			glBegin(GL_POLYGON);
			
			if(islight && (tface->mode & TF_LIGHT)) {
				do_realtimelight(ob, tface, col[0]);
				
				glColor3fv(col[0]);
				glVertex3fv(v1);
				if(mode & TF_GOUR) {
					glColor3fv(col[1]);
					glVertex3fv(v2);
					glColor3fv(col[2]);
					glVertex3fv(v3);
					if(v4) {glColor3fv(col[3]); glVertex3fv(v4);}
				}
				else {
					glVertex3fv(v2);
					glVertex3fv(v3);
					if(v4) glVertex3fv(v4);
				}
			}
			else {
				cp= (char *)&(tface->col[0]);
				
				if(mode & TF_GOUR) {

					glColor3ub(cp[3], cp[2], cp[1]);
					glVertex3fv(v1);
					glColor3ub(cp[7], cp[6], cp[5]);
					glVertex3fv(v2);
					glColor3ub(cp[11], cp[10], cp[9]);
					glVertex3fv(v3);
					if(v4) {
						glColor3ub(cp[15], cp[14], cp[13]);
						glVertex3fv(v4);
					}
				}
				else {	/* GL_FLAT */
					
					glColor3ub(cp[3], cp[2], cp[1]);
					glVertex3fv(v1);
					glVertex3fv(v2);
					glVertex3fv(v3);
					if(v4) glVertex3fv(v4);
				}
			}
			glEnd();
		}
	}
	
	glShadeModel(GL_FLAT);
	glDisable(GL_CULL_FACE);
	
	draw_hide_tfaces(ob, me);

	if(ob==OBACT && (G.f & G_FACESELECT)) {
		draw_tfaces3D(ob, me);
	}
	glColor3f(coli[0],  coli[1],  coli[2]);
}

void draw_tface_meshes_tra()
{
	Object *ob;
	Mesh *me;
	Life *lf;
	TFace *tface;
	MFace *mface;
	MVert *mvert;
	Image *ima;
	float *v1, *v2, *v3, *v4, col[4][3], coli[4];
	uint obcol;
	int a, t, mode, dt;
	short islight;
	char *cp;
	
	if(G.vd->drawtype!=OB_TEXTURE) return;

	glDepthMask(0);
	glShadeModel(GL_SMOOTH);
	glGetFloatv(GL_CURRENT_COLOR, coli);

	glCullFace(GL_BACK); glEnable(GL_CULL_FACE);

	for(t=0; t<tot_tra_ob; t++) {
		
		ob= tra_ob_ar[t].ob;
		me= tra_ob_ar[t].me;
		dt= tra_ob_ar[t].dt;

		multmatrix(ob->obmat);
		
		if(me==0) return;
		if(ob->type==OB_LIFE) lf= ob->data;
		else lf= 0;
		
		islight= do_realtimelight(ob, 0, 0);
	
		/* de ob color */
		if(ob->type==OB_SECTOR) {
			Sector *se= ob->data;
			obcol= rgb_to_mcol(se->r, se->g, se->b);
		}
		else if(ob->type==OB_LIFE) {
			char *tmp, c;
			
			Life *lf= ob->data;
			obcol= rgb_to_mcol(lf->r, lf->g, lf->b);
		}
			
		if(dt>OB_SOLID) {
			
			mface= me->mface;
			tface= me->tface;
			
			for(a=me->totface; a>0; a--, mface++, tface++) {
				if(mface->v3==0) continue;
				if(tface->flag & TF_HIDE) continue;
				
				if(tface->transp);
				else continue;
				
				mode= tface->mode;
	
				if(mode & TF_OBCOL) tface->col[0]= obcol;
				
				v1= (me->mvert+mface->v1)->co;
				v2= (me->mvert+mface->v2)->co;
				v3= (me->mvert+mface->v3)->co;
				if(mface->v4) v4= (me->mvert+mface->v4)->co; else v4= 0;
				
				if(mode & TF_SHAREDCOL) {
					punopoin[0]= (short *)(v1+3);
					punopoin[1]= (short *)(v2+3);
					punopoin[2]= (short *)(v3+3);
					if(v4) punopoin[3]= (short *)(v4+3);
					else punopoin[3]= 0;
				}
	
				if(mode & TF_TWOSIDE) {
					glDisable(GL_CULL_FACE);
				}
				else {
					glEnable(GL_CULL_FACE);
				}
	
				if(mode & TF_TEX) {
					
					/* in set_tpage worden dingen gedaan die niet binnen een bgnpolygon mogen liggen */
					
					if( set_tpage(tface) ) {
						
						if(mode & TF_SHADOW) {
							if(lf) {
								float vec[3];
								
								glBegin(GL_POLYGON);
								
								glTexCoord2sv(tface->uv[0]);
		
								spack(tface->col[0]);
								VecAddf(vec, v1, lf->floorloc);
								glVertex3fv(vec);
								
								glTexCoord2sv(tface->uv[1]);
								if(mode & TF_GOUR) spack(tface->col[1]);
								VecAddf(vec, v2, lf->floorloc);
								glVertex3fv(vec);
					
								glTexCoord2sv(tface->uv[2]);
								if(mode & TF_GOUR) spack(tface->col[2]);
								VecAddf(vec, v3, lf->floorloc);
								glVertex3fv(vec);
					
								if(v4) {
									glTexCoord2sv(tface->uv[3]);
									if(mode & TF_GOUR) spack(tface->col[3]);
									VecAddf(vec, v4, lf->floorloc);
									glVertex3fv(vec);
								}
								glEnd();
							}
						}
						else if(islight && (mode & TF_LIGHT)) {
							do_realtimelight(ob, tface, col[0]);
							
							glBegin(GL_POLYGON);
							
							glTexCoord2sv(tface->uv[0]);
	
							glColor3fv(col[0]);
							glVertex3fv(v1);
							
							glTexCoord2sv(tface->uv[1]);
							if(mode & TF_GOUR) glColor3fv(col[1]);
							glVertex3fv(v2);
				
							glTexCoord2sv(tface->uv[2]);
							if(mode & TF_GOUR) glColor3fv(col[2]);
							glVertex3fv(v3);
				
							if(v4) {
								glTexCoord2sv(tface->uv[3]);
								if(mode & TF_GOUR) glColor3fv(col[3]);
								glVertex3fv(v4);
							}
							glEnd();
						}
						else if(mode & TF_BILLBOARD) {	/* actually halo! */
							float mat[4][4];
							float len;
							
							glPushMatrix();
							getmatrix(mat);
							len= fsqrt(mat[0][0]*mat[0][0] + mat[1][1]*mat[1][1] + mat[2][2]*mat[2][2] );

							mat[0][0]= mat[1][1]= mat[2][2]= len;
							mat[0][1]= mat[0][2]= 0.0;
							mat[1][0]= mat[1][2]= 0.0;
							mat[2][0]= mat[2][1]= 0.0;
							mat[0][3]= mat[1][3]= mat[2][3]= 0.0;
							mat[3][3]= 1.0;
							loadmatrix(mat);
							
							glBegin(GL_POLYGON);
							
							glTexCoord2sv(tface->uv[0]);
	
							spack(tface->col[0]);
							glVertex3fv(v1);
							
							glTexCoord2sv(tface->uv[1]);
							if(mode & TF_GOUR) spack(tface->col[1]);
							glVertex3fv(v2);
				
							glTexCoord2sv(tface->uv[2]);
							if(mode & TF_GOUR) spack(tface->col[2]);
							glVertex3fv(v3);
				
							if(v4) {
								glTexCoord2sv(tface->uv[3]);
								if(mode & TF_GOUR) spack(tface->col[3]);
								glVertex3fv(v4);
							}
							glEnd();
							
							glPopMatrix();
						}
						else {
							glBegin(GL_POLYGON);
							
							glTexCoord2sv(tface->uv[0]);
	
							spack(tface->col[0]);
							glVertex3fv(v1);
							
							glTexCoord2sv(tface->uv[1]);
							if(mode & TF_GOUR) spack(tface->col[1]);
							glVertex3fv(v2);
				
							glTexCoord2sv(tface->uv[2]);
							if(mode & TF_GOUR) spack(tface->col[2]);
							glVertex3fv(v3);
				
							if(v4) {
								glTexCoord2sv(tface->uv[3]);
								if(mode & TF_GOUR) spack(tface->col[3]);
								glVertex3fv(v4);
							}
							glEnd();
						}
					}
				}
			}
		}
		loadmatrix(G.vd->viewmat);
	}
	
	/* textures uitzetten */
	set_tpage(0);

	tot_tra_ob= 0;

	glDepthMask(1);
	glShadeModel(GL_FLAT);
	glDisable(GL_CULL_FACE);
	
	glColor3f(coli[0],  coli[1],  coli[2]);
}

void drawsector(Object *ob, int dt, uint col)		/* col: restore van ghost */
{	
	Object *obedit;
	Sector *se;
	Portal *po;
	int a, flag;
	
	se= ob->data;
	
	if(se->flag & SE_GHOST_OTHER) {
		if( (G.f & (G_SIMULATION|G_BACKBUFSEL))==0) {
			flag= se->flag;
			obedit= G.obedit;
			G.obedit= 0;
		
			if(se->flag & SE_SHOW_TEXMESH) {
				se->flag &= ~SE_SHOW_TEXMESH;
				setlinestyle(3);
				drawcube_size(se->size);
				setlinestyle(0);
			}
			else {
				se->flag |= SE_SHOW_TEXMESH;
			}
			cpack(0x505050);
			drawmeshwire(ob);
			
			se->flag= flag;
			G.obedit= obedit;
			cpack(col);
		}
	}

	if(se->flag & SE_SHOW_TEXMESH) {

		if(ob==G.obedit) drawmeshwire(ob);
		else if(dt==OB_BOUNDBOX) drawboundbox(ob);
		else if(dt==OB_WIRE) drawmeshwire(ob);
		else {
			
			if((G.f & (G_PICKSEL))) drawmeshwire(ob);
			else if((G.f & (G_BACKBUFSEL))) drawmeshsolid(ob, 0);
			else draw_tface_mesh(ob, se->texmesh, dt);
		}
	}
	else {

		if(G.f & G_SIMULATION);
		else if(ob!=G.obedit) {
			
			setlinestyle(3);
			drawcube_size(se->size);
			setlinestyle(0);
			
			if(ob->dt>OB_WIRE) {
				
				po= se->portals;
				if(ob==OBACT) {
					for(a=1; a<=se->totport; a++) {
						draw_portal(se, po, a==se->actport);
						po++;
					}
				}
				else {
					a= se->totport;
					while(a--) {
						draw_portal(se, po, 0);
						po++;
					}
				}
			}
		}

		if(dt>OB_SOLID) dt= OB_SOLID;	/* shaded bij dynamesh: werkt niet */
		
		if(ob==G.obedit) drawmeshwire(ob);
		else if(dt==OB_BOUNDBOX) drawboundbox(ob);
		else if(dt==OB_WIRE) drawmeshwire(ob);
		else drawDispList(ob, dt);
	}
	

}

/* *************** */

void init_realtime_GL()
{		
	Mat4One(texmat1);
	Mat4One(texmat4);
	Mat4MulFloat3((float *)texmat1, 1.0/32767.0);
	Mat4MulFloat3((float *)texmat4, 4.0/32767.0);
	
	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(texmat1);
	glMatrixMode(GL_MODELVIEW);
	
	#ifdef __sgi
	glBlendEquationEXT(GL_FUNC_ADD_EXT);
	#endif
	
	/* glPolygonOffset(-0.001, -0.001); */
}

void sector_simulate()
{
	Camera *cam=0;
	float params[5];
	float loc[3], rot[3];
	
	if(G.scene->camera==0 || G.scene->camera->type!=OB_CAMERA) {
		error("no (correct) camera");
		return;
	}

	/* soms is de aktieve camera niet in een sector */
	VECCOPY(loc, G.scene->camera->loc);
	VECCOPY(rot, G.scene->camera->rot);
	
	G.f |= G_SIMULATION;
	G.simulf= 0;
		
	cam= G.scene->camera->data;
	
	if(G.vd->drawtype > OB_WIRE) {
		if(G.scene->world && (G.scene->world->mode & WO_MIST)) {
			glFogi(GL_FOG_MODE, GL_LINEAR);
			glFogf(GL_FOG_DENSITY, 0.1);
			glFogf(GL_FOG_START, G.scene->world->miststa);
			glFogf(GL_FOG_END, G.scene->world->miststa+G.scene->world->mistdist);
			params[0]= G.scene->world->horr;
			params[1]= G.scene->world->horg;
			params[2]= G.scene->world->horb;
			params[3]= 0.0;
			glFogfv(GL_FOG_COLOR, params); 
			glEnable(GL_FOG);
		}
	}
	
	waitcursor(1);

	while(TRUE) {
		
		sector_go();
		
		if(G.simulf & G_LOADFILE) break;
		if(G.simulf & G_RESTART);
		if(G.simulf & G_QUIT) {
			reset_slowparents();	/* editobject.c */
			break;
		}
		G.simulf= 0;
	}

	waitcursor(0);

	glDisable(GL_FOG);

	G.f &= ~G_SIMULATION;
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWBUTSALL, 0);
	
	/* soms is de aktieve camera niet in een sector */
	VECCOPY(G.scene->camera->loc, loc);
	VECCOPY(G.scene->camera->rot, rot);

}

#endif

