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


/* drawimage.c  juli 96		GRAPHICS
 * 
 * 
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "render.h"
#include "sector.h"


void rectwrite_part(int winxmin, int winymin, int winxmax, int winymax, int x1, int y1, int xim, int yim, float zoomx, float zoomy, uint *rect)
{
	int cx, cy, oldxim, x2, y2;
	
	oldxim= xim;

	xim--;
	yim--;
		
	/* coordinaten hoe 't op scherm komt */
	x2= x1+ zoomx*xim;
	y2= y1+ zoomy*yim;

	/* partiele clip */
	if(x1<winxmin) {
		/* recten bij OpenGL mogen niet links/onder van windowrand beginnen */
		cx= winxmin-x1+(int)zoomx;
		/* zorg ervoor dat de rect pixelnauwkeurig wordt neergezet */
		cx/= zoomx;
		x1+= zoomx*cx;
		xim-= cx;
		rect+= cx;
	}
	if(y1<winymin) {
		cy= winymin-y1+(int)zoomy;
		cy/= zoomy;
		y1+= zoomy*cy;
		rect+= cy*oldxim;
		yim-= cy;
	}
	if(x2>winxmax) {
		cx= x2-winxmax;
		cx/= zoomx;
		xim-= cx;
	}
	if(y2>winymax) {
		cy= y2-winymax;
		cy/= zoomy;
		yim-= cy;
	}
	
	if(xim<=0) return;
	if(yim<=0) return;

	if(G.curscreen->winakt!= R.win) {
		/* dit is vanwege IRISGL, recten altijd in screen coordinaten */
		winset(G.curscreen->mainwin);
		glScissor(winxmin, winymin, winxmax-winxmin+1, winymax-winymin+1);
	}
	
	glPixelStorei(GL_UNPACK_ROW_LENGTH,  oldxim);
	
	glPixelZoom(zoomx,  zoomy);

	glRasterPos2i(x1, y1);
	glDrawPixels(xim, yim, GL_RGBA, GL_UNSIGNED_BYTE,  rect);

	glPixelZoom(1.0,  1.0);

	glPixelStorei(GL_UNPACK_ROW_LENGTH,  0);
	
	if(G.curscreen->winakt!= R.win) winset(curarea->win);
}

/* nog nergens gebuikt? */
void set_imagespace(Image *ima, int zoom)
{
	ScrArea *sa;
	SpaceImage *sima;
	
	/* welke area? */
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->spacetype==SPACE_IMAGE) {
			break;
		}
		sa= sa->next;
	}
	
	if(sa) {
		addqueue(sa->win, REDRAW, 1);
		addqueue(sa->headwin, REDRAW, 1);
		sima= sa->spacedata.first;
		sima->image= ima;
		if(zoom) sima->zoom= zoom;
	}
}

void calc_image_view(SpaceImage *sima, char mode)
{
	int xim=100, yim=100;
	int x1, y1;
	float zoom;
	
	if(sima->image && sima->image->ibuf) {
		xim= sima->image->ibuf->x;
		yim= sima->image->ibuf->y;
	}
	
	sima->v2d.tot.xmin= 0;
	sima->v2d.tot.ymin= 0;
	sima->v2d.tot.xmax= xim;
	sima->v2d.tot.ymax= yim;
	
	sima->v2d.mask.xmin= sima->v2d.mask.ymin= 0;
	sima->v2d.mask.xmax= curarea->winx;
	sima->v2d.mask.ymax= curarea->winy;


	/* welk deel van de imageruimte zien we: */
	/* zelfde berekening als lrectwrite: plek linksonder */
	x1= curarea->winrct.xmin+(curarea->winx-sima->zoom*xim)/2;
	y1= curarea->winrct.ymin+(curarea->winy-sima->zoom*yim)/2;

	x1-= sima->zoom*sima->xof;
	y1-= sima->zoom*sima->yof;

	/* float! */
	zoom= sima->zoom;
	
	/* relatieve afbeeld links */
	sima->v2d.cur.xmin= ((curarea->winrct.xmin - (float)x1)/zoom);
	sima->v2d.cur.xmax= sima->v2d.cur.xmin + ((float)curarea->winx/zoom);
	
	/* relatieve afbeeld links */
	sima->v2d.cur.ymin= ((curarea->winrct.ymin-(float)y1)/zoom);
	sima->v2d.cur.ymax= sima->v2d.cur.ymin + ((float)curarea->winy/zoom);
	
	if(mode=='s') {
		xim= 32768.0/xim;
		yim= 32768.0/yim;
		
		sima->v2d.cur.xmin*= xim;
		sima->v2d.cur.xmax*= xim;
		sima->v2d.cur.ymin*= yim;
		sima->v2d.cur.ymax*= yim;
	}
	
}

void what_image(SpaceImage *sima)
{
	extern TFace *lasttface;	/* editsector.c */
	Mesh *me;
		
	if(sima->mode==SI_TEXTURE) {
		if(G.f & G_FACESELECT) {

			sima->image= 0;
			me= get_mesh(OBACT);
			set_lasttface();
			
			if(me && me->tface && lasttface) {
				if(lasttface->mode & TF_TEX) {
					sima->image= lasttface->tpage;
					
					if(sima->flag & SI_EDITTILE);
					else sima->curtile= lasttface->tile;
					
					if(sima->image) {
						if(lasttface->mode & TF_TILES) sima->image->tpageflag |= IMA_TILES;
						else sima->image->tpageflag &= ~IMA_TILES;
					}
				}
			}
		}
	}
}

void image_changed(SpaceImage *sima, int dotile)
{
	TFace *tface;
	Mesh *me;
	int a;
	
	if(sima->mode==SI_TEXTURE) {
		
		if(G.f & G_FACESELECT) {
			me= get_mesh(OBACT);
			if(me && me->tface) {
				tface= me->tface;
				a= me->totface;
				while(a--) {
					if(tface->flag & SELECT) {
						
						if(dotile==2) {
							tface->mode &= ~TF_TILES;
						}
						else {
							tface->tpage= sima->image;
							tface->mode |= TF_TEX;
						
							if(dotile) tface->tile= sima->curtile;
						}
						
						if(sima->image) {
							if(sima->image->tpageflag & IMA_TILES) tface->mode |= TF_TILES;
							else tface->mode &= ~TF_TILES;
						
							if(sima->image->id.us==0) sima->image->id.us= 1;
						}
					}
					tface++;
				}
				allqueue(REDRAWVIEW3D, 0);
				allqueue(REDRAWBUTSGAME, 0);
			}
		}
	}
}


void uvco_to_areaco(short *vec, short *mval)
{
	float x, y;

	mval[0]= 3200;
	
	x= (vec[0] - G.v2d->cur.xmin)/(G.v2d->cur.xmax-G.v2d->cur.xmin);
	if(x>=0.0 && x<=1.0) {
		y= (vec[1] - G.v2d->cur.ymin)/(G.v2d->cur.ymax-G.v2d->cur.ymin);
		if(y>=0.0 && y<=1.0) {
			
			mval[0]= G.v2d->mask.xmin + x*(G.v2d->mask.xmax-G.v2d->mask.xmin);
			mval[1]= G.v2d->mask.ymin + y*(G.v2d->mask.ymax-G.v2d->mask.ymin);
			
		}
	}
}

void uvco_to_areaco_noclip(short *vec, short *mval)
{
	float x, y;

	mval[0]= 3200;
	
	x= (vec[0] - G.v2d->cur.xmin)/(G.v2d->cur.xmax-G.v2d->cur.xmin);
	y= (vec[1] - G.v2d->cur.ymin)/(G.v2d->cur.ymax-G.v2d->cur.ymin);

	x= G.v2d->mask.xmin + x*(G.v2d->mask.xmax-G.v2d->mask.xmin);
	y= G.v2d->mask.ymin + y*(G.v2d->mask.ymax-G.v2d->mask.ymin);
	
	if(x<-32760) mval[0]= -32760;
	else if(x>32760) mval[0]= 32760;
	else mval[0]= x;

	if(y<-32760) mval[1]= -32760;
	else if(y>32760) mval[1]= 32760;
	else mval[1]= y;
}


void draw_tfaces()
{
	TFace *tface;
	MFace *mface;
	Mesh *me;
	uint col;
	int a;
	short mval[2];
	
	glPointSize(2.0);
	
	if(G.f & G_FACESELECT) {
		me= get_mesh(OBACT);
		if(me && me->tface) {
			
			calc_image_view(G.sima, 's');	/* short */
			ortho2(G.v2d->cur.xmin, G.v2d->cur.xmax, G.v2d->cur.ymin, G.v2d->cur.ymax);

			tface= me->tface;
			mface= me->mface;
			a= me->totface;
			
			while(a--) {
				if(mface->v3 && (tface->flag & SELECT) ) {
				
					cpack(0x0);
					glBegin(GL_LINE_LOOP);
						glVertex2sv( tface->uv[0] );
						glVertex2sv( tface->uv[1] );
						glVertex2sv( tface->uv[2] );
						if(mface->v4) glVertex2sv( tface->uv[3] );
					glEnd();
				
					setlinestyle(2);
					/* kleuren: R=x G=y */
					
					if(tface->flag & ACTIVE) cpack(0xFF00); else cpack(0xFFFFFF);
	
					glBegin(GL_LINE_STRIP);
						glVertex2sv( tface->uv[0] );
						glVertex2sv( tface->uv[1] );
					glEnd();
					
					if(tface->flag & ACTIVE) cpack(0xFF); else cpack(0xFFFFFF);
	
					glBegin(GL_LINE_STRIP);
						glVertex2sv( tface->uv[0] );
						if(mface->v4) glVertex2sv( tface->uv[3] ); else glVertex2sv( tface->uv[2] );
					glEnd();
					
					cpack(0xFFFFFF);
	
					glBegin(GL_LINE_STRIP);
						glVertex2sv( tface->uv[1] );
						glVertex2sv( tface->uv[2] );
						if(mface->v4) glVertex2sv( tface->uv[3] );
					glEnd();
					
					setlinestyle(0);
					
					glBegin(GL_POINTS);
					
					if(tface->flag & TF_SEL1) col= 0x77FFFF; else col= 0xFF70FF;
					cpack(col);
					glVertex2sv(tface->uv[0]);
					
					if(tface->flag & TF_SEL2) col= 0x77FFFF; else col= 0xFF70FF;
					cpack(col);
					glVertex2sv(tface->uv[1]);
					
					if(tface->flag & TF_SEL3) col= 0x77FFFF; else col= 0xFF70FF;
					cpack(col);
					glVertex2sv(tface->uv[2]);
					
					if(mface->v4) {
						if(tface->flag & TF_SEL4) col= 0x77FFFF; else col= 0xFF70FF;
						cpack(col);
						glVertex2sv(tface->uv[3]);
					}
					glEnd();
				}
					
				tface++;
				mface++;
			}
		}
	}
	glPointSize(1.0);
}

uint *get_part_from_ibuf(ImBuf *ibuf, short startx, short starty, short endx, short endy)

{
	uint *rt, *rp, *rectmain;
	short y, heigth, len;

	/* de juiste offset in rectot */

	rt= ibuf->rect+ (starty*ibuf->x+ startx);

	len= (endx-startx);
	heigth= (endy-starty);

	rp=rectmain= mallocN(heigth*len*sizeof(int), "rect");
	
	for(y=0; y<heigth; y++) {
		memcpy(rp, rt, len*4);
		rt+= ibuf->x;
		rp+= len;
	}
	return rectmain;
}

void drawimagespace()
{
	ImBuf *ibuf;
	TFace *tface;
	uint *rect;
	int x1, y1, xmin, xmax, ymin, ymax;
	short sx, sy, dx, dy;
	char str[256];
	
	glClearColor(.1875, .1875, .1875, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT);

	curarea->win_swap= WIN_BACK_OK;
	
	what_image(G.sima);
	if(G.sima->image==0) return;
	
	xmin= curarea->winrct.xmin; xmax= curarea->winrct.xmax;
	ymin= curarea->winrct.ymin; ymax= curarea->winrct.ymax;
	
	if(G.sima->image->ibuf==0) {
		strcpy(str, G.sima->image->name);
		convertstringcode(str);
		G.sima->image->ibuf = loadiffname(str , LI_rect);
	}	
	ibuf= G.sima->image->ibuf;

	if(ibuf==0 || ibuf->rect==0) return;
		
	/* plek berekenen */
	x1= xmin+(curarea->winx-G.sima->zoom*ibuf->x)/2;
	y1= ymin+(curarea->winy-G.sima->zoom*ibuf->y)/2;

	x1-= G.sima->zoom*G.sima->xof;
	y1-= G.sima->zoom*G.sima->yof;

	
	if(G.sima->flag & SI_EDITTILE) {
		rectwrite_part(xmin, ymin, xmax, ymax, x1, y1, ibuf->x, ibuf->y, (float)G.sima->zoom, (float)G.sima->zoom, ibuf->rect);
		
		dx= ibuf->x/G.sima->image->xrep;
		dy= ibuf->y/G.sima->image->yrep;
		sy= (G.sima->curtile / G.sima->image->xrep);
		sx= G.sima->curtile - sy*G.sima->image->xrep;

		sx*= dx;
		sy*= dy;
		
		calc_image_view(G.sima, 'p');	/* pixel */
		ortho2(G.v2d->cur.xmin, G.v2d->cur.xmax, G.v2d->cur.ymin, G.v2d->cur.ymax);
		
		cpack(0x0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(sx,  sy,  sx+dx-1,  sy+dy-1); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		cpack(0xFFFFFF);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(sx+1,  sy+1,  sx+dx,  sy+dy); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if(G.sima->mode==SI_TEXTURE) {
		if(G.sima->image->tpageflag & IMA_TILES) {
			
			
			/* eventjes laten staan */
			if(G.sima->image->xrep<1) return;
			if(G.sima->image->yrep<1) return;
			
			if(G.sima->curtile >= G.sima->image->xrep*G.sima->image->yrep) 
				G.sima->curtile = G.sima->image->xrep*G.sima->image->yrep - 1; 
			
			dx= ibuf->x/G.sima->image->xrep;
			dy= ibuf->y/G.sima->image->yrep;
			
			sy= (G.sima->curtile / G.sima->image->xrep);
			sx= G.sima->curtile - sy*G.sima->image->xrep;
	
			sx*= dx;
			sy*= dy;
			
			rect= get_part_from_ibuf(ibuf, sx, sy, sx+dx, sy+dy);
			
			/* rect= ibuf->rect; */
			for(sy= 0; sy+dy<=ibuf->y; sy+= dy) {
				for(sx= 0; sx+dx<=ibuf->x; sx+= dx) {
					
					rectwrite_part(xmin, ymin, xmax, ymax, 
						x1+sx*G.sima->zoom, y1+sy*G.sima->zoom, dx, dy, (float)G.sima->zoom, (float)G.sima->zoom, rect);
				}
			}
			
			freeN(rect);
		}
		else 
			rectwrite_part(xmin, ymin, xmax, ymax, x1, y1, ibuf->x, ibuf->y, (float)G.sima->zoom,(float)G.sima->zoom, ibuf->rect);
	
		draw_tfaces();
	}
	
	calc_image_view(G.sima, 's');	/* short */
}

void image_viewmove()
{
	short mval[2], mvalo[2], xof, yof;
	
	getmouseco_sc(mvalo);
	
	while(get_mbut()&(L_MOUSE|M_MOUSE)) {

		getmouseco_sc(mval);
		
		xof= (mvalo[0]-mval[0])/G.sima->zoom;
		yof= (mvalo[1]-mval[1])/G.sima->zoom;
		
		if(xof || yof) {
			
			G.sima->xof+= xof;
			G.sima->yof+= yof;
			
			mvalo[0]= mval[0];
			mvalo[1]= mval[1];
			
			curarea->windraw();
			screen_swapbuffers();
		}		
		else usleep(2);
	}
}

void image_home()
{
	
	if(curarea->spacetype!=SPACE_IMAGE) return;
	
	if(G.sima->image==0 || G.sima->image->ibuf==0) return;
	
	G.sima->zoom= 1;
	G.sima->xof= G.sima->yof= 0;
	
	calc_image_view(G.sima, 'p');
	
	addqueue(curarea->win, REDRAW, 1);
}

