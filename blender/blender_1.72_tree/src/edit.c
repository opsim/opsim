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

 

/* edit.c  mei 94		GRAPHICS
 * 
 * 
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "edit.h"
#include "sector.h"

/* ************************************************ */
/* pushpop faciliteit: om tijdelijk data te bewaren */

ListBase ppmain={0, 0};

typedef struct PushPop {
	struct PushPop *next, *prev;
	void *data;
	int len;
} PushPop;

void pushdata(void *data, int len)
{
	PushPop *pp;
	
	pp= mallocN(sizeof(PushPop), "pushpop");
	addtail(&ppmain, pp);
	pp->data= mallocN(len, "pushpop");
	pp->len= len;
	memcpy(pp->data, data, len);
}

void popfirst(void *data)
{
	PushPop *pp;
	
	pp= ppmain.first;
	if(pp) {
		memcpy(data, pp->data, pp->len);
		remlink(&ppmain, pp);
		freeN(pp->data);
		freeN(pp);
	}
	else printf("error in popfirst\n");
}

void poplast(void *data)
{
	PushPop *pp;
	
	pp= ppmain.last;
	if(pp) {
		memcpy(data, pp->data, pp->len);
		remlink(&ppmain, pp);
		freeN(pp->data);
		freeN(pp);
	}
	else printf("error in poplast\n");
}

void free_pushpop()
{
	PushPop *pp;
	
	while(pp= ppmain.first) {
		remlink(&ppmain, pp);
		freeN(pp->data);
		freeN(pp);
	}	
}

void pushpop_test()
{
	if(ppmain.first) printf("pushpop not empty\n");
	free_pushpop();
}

/* ************************************************ */

#ifdef IRISGL

int get_border(rcti *rect, short col)
{
	float winmat[4][4], viewmat[4][4];
	float dvec[4], fac1, fac2;
	int retval=1;
	ushort event;
	short mval[2], mvalo[4], val, x1, y1;
	char str[64];
	
	areawinset(curarea->win);
	drawmode(PUPDRAW);
	
	initgrabz(0.0, 0.0, 0.0);
	
	/* windowmatrix default */
	getmatrix(viewmat);
	mmode(MPROJECTION);
	getmatrix(winmat);
	mmode(MVIEWING);
	defwinmat();
	
	getmouseco_areawin(mvalo);
	color(col);
	sboxs(0, mvalo[1], curarea->winx, mvalo[1]);
	sboxs(mvalo[0], 0, mvalo[0], curarea->winy);

	while(TRUE) {
		getmouseco_areawin(mval);
		if(mvalo[0]!=mval[0] || mvalo[1]!=mval[1] || qtest()) {
			color(0);
			sboxs(0, mvalo[1], curarea->winx, mvalo[1]);
			sboxs(mvalo[0], 0, mvalo[0], curarea->winy);
			color(col);
			sboxs(0, mval[1], curarea->winx, mval[1]);
			sboxs(mval[0], 0, mval[0], curarea->winy);
			
			mvalo[0]= mval[0];
			mvalo[1]= mval[1];
			
			event= extern_qread(&val);
			if(val) {
				if(event==ESCKEY) {
					retval= 0;
					break;
				}
				else if(event==BKEY) {
					retval= 0;
					break;
				}
				else if(event==LEFTMOUSE) break;
				else if(event==MIDDLEMOUSE) break;
				else if(event==RIGHTMOUSE) break;
			}
		}
		else usleep(2);
	}
	color(0);
	sboxs(0, mvalo[1], curarea->winx, mvalo[1]);
	sboxs(mvalo[0], 0, mvalo[0], curarea->winy);

	if(retval) {
		x1= mval[0];
		y1= mval[1];
		
		getmouseco_areawin(mvalo);
		color(col);
		sboxs(x1, y1, mvalo[0], mvalo[1]);
		
		while(TRUE) {
			getmouseco_areawin(mval);
			if(mvalo[0]!=mval[0] || mvalo[1]!=mval[1] || qtest()) {
				color(0);
				sboxs(x1, y1, mvalo[0], mvalo[1]);
				sboxfs(10, 10, 400, 32);
				color(col);
				sboxs(x1, y1, mval[0], mval[1]);
				
				mvalo[0]= mval[0];
				mvalo[1]= mval[1];
				
				event= extern_qread(&val);
				if(val==0) {
					if(event==ESCKEY) {
						retval= 0;
						break;
					}
					else if(event==LEFTMOUSE) break;
					else if(event==MIDDLEMOUSE) break;
					else if(event==RIGHTMOUSE) break;
				}
				if(curarea->spacetype==SPACE_VIEW3D) {
					if(G.vd->persp==0) {
						window_to_3d(dvec, mvalo[0]-x1, mvalo[1]-y1);
						glRasterPos2i(10, 10);
						sprintf(str, "X %.4f  Y %.4f  Z %.4f  Dia %.4f", dvec[0], dvec[1], dvec[2], fsqrt(dvec[0]*dvec[0]+dvec[1]*dvec[1]+dvec[2]*dvec[2]));
						charstr(str);
					}
					else if(G.vd->persp==2) {
						fac1= (mvalo[0]-x1)/( (float) (G.vd->pr_xmax-G.vd->pr_xmin) );
						fac1*= 0.01*G.scene->r.size*G.scene->r.xsch;
						
						fac2= (mvalo[1]-y1)/( (float) (G.vd->pr_ymax-G.vd->pr_ymin) );
						fac2*= 0.01*G.scene->r.size*G.scene->r.ysch;
						
						glRasterPos2i(10, 10);
						sprintf(str, "X %.1f  Y %.1f  Dia %.1f", fabs(fac1), fabs(fac2), fsqrt(fac1*fac1 + fac2*fac2) );
						charstr(str);
					}
				}
				else if(curarea->spacetype==SPACE_IPO) {
					mvalo[2]= x1;
					mvalo[3]= y1;
					areamouseco_to_ipoco(mval, dvec, dvec+1);
					areamouseco_to_ipoco(mvalo+2, dvec+2, dvec+3);
					glRasterPos2i(18, 18);
					sprintf(str, "Time: %.4f  Y %.4f", dvec[0]-dvec[2], dvec[1]-dvec[3]);
					charstr(str);
				}
			}
			else sginap(2);
		}
		color(0);
		clear();
		
		if(retval) {
			rect->xmin= x1;
			rect->ymin= y1;
			rect->xmax= mval[0];
			rect->ymax= mval[1];
			retval= event;
			
			/* normaliseren */
			if(rect->xmin>rect->xmax) SWAP(int, rect->xmin, rect->xmax);
			if(rect->ymin>rect->ymax) SWAP(int, rect->ymin, rect->ymax);
			
			if(rect->xmin==rect->xmax) retval= 0;
			if(rect->ymin==rect->ymax) retval= 0;
		}
	}

	drawmode(NORMALDRAW);

	/* restore matrices */
	loadmatrix(viewmat);
	mmode(MPROJECTION);
	loadmatrix(winmat);
	mmode(MVIEWING);
	
	if(event==BKEY && G.obedit && curarea->spacetype==SPACE_VIEW3D)
		circle_select();
	
	return retval;
}

#else

int get_border(rcti *rect, short col)
{
	extern short scrmousex, scrmousey;
	float dvec[4], fac1, fac2;
	int retval=1;
	ushort event;
	short mval[2], mvalo[4], val, x1, y1;
	char str[64];
	
	#if defined(__sgi) || defined(__SUN)
	/* this is a dirty patch: XgetImage gets sometimes the backbuffer */
	my_get_frontbuffer_image(0, 0, 1, 1);
	my_put_frontbuffer_image();
	#endif

	winset(G.curscreen->mainwin);
	
	/* pietsje groter, 1 pixel aan de rand */
	glReadBuffer(GL_FRONT);
	glDrawBuffer(GL_FRONT);

	if (curarea->spacetype==SPACE_IPO)
		my_get_frontbuffer_image(20+curarea->winrct.xmin, 30+curarea->winrct.ymin, 300, 22);
	else
		my_get_frontbuffer_image(0+curarea->winrct.xmin, 10+curarea->winrct.ymin, 300, 22);
	
	winset(curarea->win);
	
	glDrawBuffer(GL_FRONT);
	persp(0);
	initgrabz(0.0, 0.0, 0.0);
	
	getmouseco_areawin(mvalo);

	sdrawXORline4(0, 0,  mvalo[1],  curarea->winx,  mvalo[1]);
	sdrawXORline4(1, mvalo[0],  0,  mvalo[0],  curarea->winy); 

	while(TRUE) {
	
		/* als een renderwinodow open is en de muis gaat erin */
		persp(1);
		winset(curarea->win);
		persp(0);
		
		getmouseco_areawin(mval);
		if(mvalo[0]!=mval[0] || mvalo[1]!=mval[1]) {

			sdrawXORline4(0, 0,  mval[1],  curarea->winx,  mval[1]);
			sdrawXORline4(1, mval[0],  0,  mval[0],  curarea->winy); 
			
			mvalo[0]= mval[0];
			mvalo[1]= mval[1];
		}
		event= extern_qread(&val);

		if(event && val) {
			if(event==ESCKEY) {
				retval= 0;
				break;
			}
			else if(event==BKEY) {
				retval= 0;
				break;
			}
			else if(event==LEFTMOUSE) break;
			else if(event==MIDDLEMOUSE) break;
			else if(event==RIGHTMOUSE) break;
		}
		else usleep(2);
	}
	
	sdrawXORline4(-1, 0, 0, 0, 0);
	fmsetfont(G.fonts);
	
	if(retval) {
		x1= mval[0];
		y1= mval[1];
		
		getmouseco_areawin(mvalo);

		sdrawXORline4(0, x1, y1, x1, mvalo[1]); 
		sdrawXORline4(1, x1, mvalo[1], mvalo[0], mvalo[1]); 
		sdrawXORline4(2, mvalo[0], mvalo[1], mvalo[0], y1); 
		sdrawXORline4(3,  mvalo[0], y1, x1, y1); 
			
		while(TRUE) {
			getmouseco_areawin(mval);
			if(mvalo[0]!=mval[0] || mvalo[1]!=mval[1]) {

				sdrawXORline4(0, x1, y1, x1, mval[1]); 
				sdrawXORline4(1, x1, mval[1], mval[0], mval[1]); 
				sdrawXORline4(2, mval[0], mval[1], mval[0], y1); 
				sdrawXORline4(3,  mval[0], y1, x1, y1); 

				mvalo[0]= mval[0];
				mvalo[1]= mval[1];
			}
			
			event= extern_qread(&val);
			
			/* still because of the renderwindow... */
			persp(1);
			winset(curarea->win);
			persp(0);
				
			if(val==0) {
				if(event==ESCKEY) {
					retval= 0;
					break;
				}
				else if(event==LEFTMOUSE) break;
				else if(event==MIDDLEMOUSE) break;
				else if(event==RIGHTMOUSE) break;
			}
			if(event==0) usleep(2);
			
			if(curarea->spacetype==SPACE_VIEW3D) {
				glColor3f(0.4375, 0.4375, 0.4375); 
				glRecti(0, 10, 250, 20);
				glColor3f(0.0, 0.0, 0.0); 

				if(G.vd->persp==0) {
					window_to_3d(dvec, mvalo[0]-x1, mvalo[1]-y1);

					glRasterPos2i(10,  10);
					sprintf(str, "X %.4f  Y %.4f  Z %.4f  Dia %.4f", dvec[0], dvec[1], dvec[2], fsqrt(dvec[0]*dvec[0]+dvec[1]*dvec[1]+dvec[2]*dvec[2]));
					fmprstr(str);
				}
				else if(G.vd->persp==2) {
					fac1= (mvalo[0]-x1)/( (float) (G.vd->pr_xmax-G.vd->pr_xmin) );
					fac1*= 0.01*G.scene->r.size*G.scene->r.xsch;
					
					fac2= (mvalo[1]-y1)/( (float) (G.vd->pr_ymax-G.vd->pr_ymin) );
					fac2*= 0.01*G.scene->r.size*G.scene->r.ysch;
					
					glRasterPos2i(10,  10);
					sprintf(str, "X %.1f  Y %.1f  Dia %.1f", fabs(fac1), fabs(fac2), fsqrt(fac1*fac1 + fac2*fac2) );
					fmprstr(str);
				}
			}
			else if(curarea->spacetype==SPACE_IPO) {
				glColor3f(.40625, .40625, .40625);
				glRecti(20, 30, 170, 40);
				glColor3f(0.0, 0.0, 0.0); 
							
				mvalo[2]= x1;
				mvalo[3]= y1;
				areamouseco_to_ipoco(mval, dvec, dvec+1);
				areamouseco_to_ipoco(mvalo+2, dvec+2, dvec+3);

				glRasterPos2i(30,  30);
				sprintf(str, "Time: %.4f  Y %.4f", dvec[0]-dvec[2], dvec[1]-dvec[3]);
				fmprstr(str);
			}
		}
		sdrawXORline4(-1, 0, 0, 0, 0);
		
		if(retval) {
			rect->xmin= x1;
			rect->ymin= y1;
			rect->xmax= mval[0];
			rect->ymax= mval[1];
			retval= event;

			/* normaliseren */
			if(rect->xmin>rect->xmax) SWAP(int, rect->xmin, rect->xmax);
			if(rect->ymin>rect->ymax) SWAP(int, rect->ymin, rect->ymax);
			
			if(rect->xmin==rect->xmax) retval= 0;
			if(rect->ymin==rect->ymax) retval= 0;
		}
	}


	/* wissen */
	if(event!=BKEY) {
		if ELEM(curarea->spacetype, SPACE_VIEW3D, SPACE_IPO) {
			my_put_frontbuffer_image();
		}
	}
	glDrawBuffer(GL_BACK);

	persp(1);
	
	if(event==BKEY && G.obedit && curarea->spacetype==SPACE_VIEW3D)
		circle_select();
	
	return retval;
}

#endif

void count_object(Object *ob, int sel)
{
	Mesh *me;
	Curve *cu;
	Sector *se;
	int tot=0, totf=0;
	
	switch(ob->type) {
	case OB_MESH:
	case OB_SECTOR:
	case OB_LIFE:
		G.totmesh++;
		me= get_mesh(ob);
		if(me) {
			G.totvert+= me->totvert;
			G.totface+= me->totface;
			if(sel) {
				G.totvertsel+= me->totvert;
				G.totfacesel+= me->totface;
			}
		}
		break;
	case OB_LAMP:
		G.totlamp++;
		break;
	case OB_SURF:
	case OB_CURVE:
	case OB_FONT:
		G.totcurve++;
		tot=totf= 0;
		cu= ob->data;
		if(cu->disp.first==0) makeDispList(ob);
		count_displist( &cu->disp, &tot, &totf);
		G.totvert+= tot;
		G.totface+= totf;
		if(sel) {
			G.totvertsel+= tot;
			G.totfacesel+= totf;
		}
		break;
	case OB_MBALL:
		count_displist( &ob->disp, &tot, &totf);
		G.totvert+= tot;
		G.totface+= totf;
		if(sel) {
			G.totvertsel+= tot;
			G.totfacesel+= totf;
		}
		
		break;
	}
	
}

void countall()
{
	extern Lattice *editLatt;
	extern ListBase editNurb;
	/* extern ListBase bpbase; */
	Base *base;
	Object *ob;
	Mesh *me;
	Curve *cu;
	Sector *se;
	Nurb *nu;
	BezTriple *bezt;
	BPoint *bp;
	MetaElem *ml;
	DispList *dl;
	/* struct BodyPoint *bop; */
	struct EditVert *eve;
	struct EditVlak *evl;
	int tot, a;

	G.totvert= G.totvertsel= G.totfacesel= G.totface= G.totobj= 
	    G.totmesh= G.totlamp= G.totcurve= G.totobjsel= 0;

	if(G.obedit) {
		
		if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) {
			eve= G.edve.first;
			while(eve) {
				G.totvert++;
				if(eve->f & 1) G.totvertsel++;
				eve= eve->next;
			}
			evl= G.edvl.first;
			while(evl) {
				G.totface++;
				if(evl->v1->f & 1) {
					if(evl->v2->f & 1) {
						if(evl->v3->f & 1) {
							if(evl->v4)  {
								if(evl->v4->f & 1) G.totfacesel++;
							}
							else {
								G.totfacesel++;
							}
						}
					}
				}
				evl= evl->next;
			}
		}
		else if ELEM3(G.obedit->type, OB_CURVE, OB_SURF, OB_FONT) {
			nu= editNurb.first;
			while(nu) {
				if((nu->type & 7)==CU_BEZIER) {
					bezt= nu->bezt;
					a= nu->pntsu;
					while(a--) {
						G.totvert+=3;
						if(bezt->f1) G.totvertsel++;
						if(bezt->f2) G.totvertsel++;
						if(bezt->f3) G.totvertsel++;
						bezt++;
					}
				}
				else {
					bp= nu->bp;
					a= nu->pntsu*nu->pntsv;
					while(a--) {
						G.totvert++;
						if(bp->f1 & 1) G.totvertsel++;
						bp++;
					}
				}
				nu= nu->next;
			}
		}
		else if(G.obedit->type==OB_MBALL) {
			ml= editelems.first;
			while(ml) {
				G.totvert++;
				if(ml->flag & SELECT) G.totvertsel++;
				ml= ml->next;
			}
		}
		else if(G.obedit->type==OB_LATTICE) {
			bp= editLatt->def;
			
			a= editLatt->pntsu*editLatt->pntsv*editLatt->pntsw;
			while(a--) {
				G.totvert++;
				if(bp->f1 & 1) G.totvertsel++;
				bp++;
			}
		}

		allqueue(REDRAWINFO, 1);	/* 1, want header->win==0! */
		return;
	}
	else if(G.f & (G_FACESELECT + G_VERTEXPAINT)) {
		me= get_mesh(OBACT);
		if(me) {
			G.totface= me->totface;
			G.totvert= me->totvert;
		}
		allqueue(REDRAWINFO, 1);	/* 1, want header->win==0! */
		return;
	}

	if(G.vd==0) return;
	if(G.scene==0) return;

	base= FIRSTBASE;
	while(base) {
		if(G.vd->lay & base->lay) {
			
			G.totobj++;
			if(base->flag & SELECT) G.totobjsel++;
			
			count_object(base->object, base->flag & SELECT);
			
			if(base->object->transflag & OB_DUPLI) {
				extern ListBase duplilist;

				make_duplilist(G.scene, base->object);
				ob= duplilist.first;
				while(ob) {
					G.totobj++;
					count_object(ob, base->flag & SELECT);
					ob= ob->id.next;
				}
				free_duplilist();
			}
		}
		base= base->next;
	}
	allqueue(REDRAWINFO, 1); 	/* 1, want header->win==0! */
}

void snapmenu()
{
	extern TransVert *transvmain;
	extern int tottrans;
	extern float originmat[3][3];	/* object.c */
	TransVert *tv;
	Base *base;
	Object *ob;
	float gridf, *curs, imat[3][3], bmat[3][3], vec[3], min[3], max[3], centroid[3];
	int count, a;
	short event;

	event= pupmenu("SNAP %t|Sel -> Grid%x1|Sel -> Curs%x2|Curs-> Grid%x3|Curs-> Sel%x4");

	gridf= G.vd->grid;
	curs= give_cursor();
	
	if(event== 1 || event==2) {  /* sel->grid  sel->curs  */

		if(G.obedit) {
			if ELEM6(G.obedit->type, OB_LATTICE, OB_MESH, OB_SECTOR, OB_LIFE, OB_SURF, OB_CURVE) make_trans_verts(bmat[0], bmat[1], 0);
			if(tottrans==0) return;
			
			Mat3CpyMat4(bmat, G.obedit->obmat);
			Mat3Inv(imat, bmat);

			tv= transvmain;
			for(a=0; a<tottrans; a++, tv++) {
			
				if(event==2) {
					
					vec[0]= curs[0]-G.obedit->obmat[3][0];
					vec[1]= curs[1]-G.obedit->obmat[3][1];
					vec[2]= curs[2]-G.obedit->obmat[3][2];
				}
				else {
					VECCOPY(vec, tv->loc);
					Mat3MulVecfl(bmat, vec);
					VecAddf(vec, vec, G.obedit->obmat[3]);
					vec[0]= G.vd->grid*ffloor(.5+ vec[0]/gridf);
					vec[1]= G.vd->grid*ffloor(.5+ vec[1]/gridf);
					vec[2]= G.vd->grid*ffloor(.5+ vec[2]/gridf);
					VecSubf(vec, vec, G.obedit->obmat[3]);
				}
				Mat3MulVecfl(imat, vec);
				VECCOPY(tv->loc, vec);

			}
			freeN(transvmain);
			transvmain= 0;
			
			if ELEM(G.obedit->type, OB_SURF, OB_CURVE) makeDispList(G.obedit);
			
			allqueue(REDRAWVIEW3D, 0);
			return;
		}

		base= FIRSTBASE;
		while(base) {
			if(TESTBASELIB(base)) {
				ob= base->object;
				
				if(event==2) {
					vec[0]= -ob->obmat[3][0] + curs[0];
					vec[1]= -ob->obmat[3][1] + curs[1];
					vec[2]= -ob->obmat[3][2] + curs[2];
				}
				else {
					vec[0]= -ob->obmat[3][0]+G.vd->grid*ffloor(.5+ ob->obmat[3][0]/gridf);
					vec[1]= -ob->obmat[3][1]+G.vd->grid*ffloor(.5+ ob->obmat[3][1]/gridf);
					vec[2]= -ob->obmat[3][2]+G.vd->grid*ffloor(.5+ ob->obmat[3][2]/gridf);
				}
				if(ob->parent) {
					where_is_object(ob);
					
					Mat3Inv(imat, originmat);
					Mat3MulVecfl(imat, vec);
					ob->loc[0]+= vec[0];
					ob->loc[1]+= vec[1];
					ob->loc[2]+= vec[2];
				}
				else {
					ob->loc[0]+= vec[0];
					ob->loc[1]+= vec[1];
					ob->loc[2]+= vec[2];
				}
			}
			
			base= base->next;
		}
		allqueue(REDRAWVIEW3D, 0);
	}
	else if(event==3) {   /* curs to grid */
		curs[0]= G.vd->grid*ffloor(.5+curs[0]/gridf);
		curs[1]= G.vd->grid*ffloor(.5+curs[1]/gridf);
		curs[2]= G.vd->grid*ffloor(.5+curs[2]/gridf);
		
		allqueue(REDRAWVIEW3D, 0);
	}
	else if(event==4) {   /* curs to sel */
		count= 0;
		INIT_MINMAX(min, max);
		centroid[0]= centroid[1]= centroid[2]= 0.0;
		
		if(G.obedit) {
			if ELEM6(G.obedit->type, OB_LATTICE, OB_MESH, OB_SURF, OB_SECTOR, OB_LIFE, OB_CURVE) make_trans_verts(bmat[0], bmat[1], 0);
			if(tottrans==0) return;
			
			Mat3CpyMat4(bmat, G.obedit->obmat);

			tv= transvmain;
			for(a=0; a<tottrans; a++, tv++) {
				VECCOPY(vec, tv->loc);
				Mat3MulVecfl(bmat, vec);
				VecAddf(vec, vec, G.obedit->obmat[3]);
				VecAddf(centroid, centroid, vec);
				DO_MINMAX(vec, min, max);
			}
			
			if(G.vd->around==V3D_CENTROID) {
				VecMulf(centroid, 1.0/(float)tottrans);
				VECCOPY(curs, centroid);
			}
			else {
				curs[0]= (min[0]+max[0])/2;
				curs[1]= (min[1]+max[1])/2;
				curs[2]= (min[2]+max[2])/2;
			}
			freeN(transvmain);
			transvmain= 0;

		}
		else {
			base= FIRSTBASE;
			while(base) {
				if(TESTBASE(base)) {
					VECCOPY(vec, base->object->obmat[3]);
					VecAddf(centroid, centroid, vec);
					DO_MINMAX(vec, min, max);
					count++;
				}
				base= base->next;
			}
			if(count) {
				if(G.vd->around==V3D_CENTROID) {
					VecMulf(centroid, 1.0/(float)count);
					VECCOPY(curs, centroid);
				}
				else {
					curs[0]= (min[0]+max[0])/2;
					curs[1]= (min[1]+max[1])/2;
					curs[2]= (min[2]+max[2])/2;
				}
			}
		}
		allqueue(REDRAWVIEW3D, 0);
	}
}

