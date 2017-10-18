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


/* editview.c  		GRAPHICS
 * 
 * jan ma 95
 * 
 * cursor/gestures/selecteren
 */

#include "blender.h"
#include "graphics.h"
#include "edit.h"

void arrowsmovecursor(ushort event)
{
	extern int displaysizex, displaysizey;	/* screen.c */
	short mval[2], a=0;

	getmouseco_sc(mval);

	if(event==UPARROWKEY) {
		a=1; 
		mval[1]++;
	}
	if(event==DOWNARROWKEY) {
		a=1; 
		mval[1]--;
	}
	if(event==LEFTARROWKEY) {
		a=1; 
		mval[0]--;
	}
	if(event==RIGHTARROWKEY) {
		a=1; 
		mval[0]++;
	}
	if(a) {
		if(mval[0]<0) mval[0]=0;
		if(mval[1]<0) mval[1]=0;
		if(mval[0]>displaysizex-1) mval[0]= displaysizex-1;
		if(mval[1]>displaysizey-1) mval[1]= displaysizey-1;

		warp_pointer(mval[0], mval[1]);
	}
}

#define MOVES 50


char interpret_move(short mcord[][2], int count)
{
	float x1, x2, y1, y2, d1, d2, md, d, inp, sq, mouse[MOVES][2];
	int i, j, dist, dir = 0;
	
	if (count <= 10) return ('g');

	/* van short naar float (tekenen is met shorts) */
	for(j=0; j<count; j++) {
		mouse[j][0]= mcord[j][0];
		mouse[j][1]= mcord[j][1];
	}
	
	/* nieuwe opzet:
	 * 
	 * vanuit eindpunten middelpunt met maximale afstand berekenen
	 * aan de hand van de hoek wordt s / g / r bepaald
	 */
	

	/* filteren */
	
	for( j = 3 ; j > 0; j--){
		x1 = mouse[1][0];
		y1 = mouse[1][1];
		for (i = 2; i < count; i++){
			x2 = mouse[i-1][0];
			y2 = mouse[i-1][1];
			mouse[i-1][0] = ((x1 + mouse[i][0]) /4.0) + (x2 / 2.0);
			mouse[i-1][1] = ((y1 + mouse[i][1]) /4.0) + (y2 / 2.0);
			x1 = x2;
			y1 = y2;
		}
	}

	/* maak directions overzicht */
	for (i = 0; i <= count - 2; i++){
		x1 = mouse[i][0] - mouse[i + 1][0];
		y1 = mouse[i][1] - mouse[i + 1][1];

		if (x1 < -0.5){
			if (y1 < -0.5) dir |= 32;
			else if (y1 > 0.5) dir |= 128;
			else dir |= 64;
		} else if (x1 > 0.5){
			if (y1 < -0.5) dir |= 8;
			else if (y1 > 0.5) dir |= 2;
			else dir |= 4;
		} else{
			if (y1 < -0.5) dir |= 16;
			else if (y1 > 0.5) dir |= 1;
			else dir |= 0;
		}
	}
	
	/* alle kruisjes naar rechts halen */
	for (i = 7; i>=0 ; i--){
		if (dir & 128) dir = (dir << 1) + 1;
		else break;
	}
	dir &= 255;
	for (i = 7; i>=0 ; i--){
		if ((dir & 1) == 0) dir >>= 1;
		else break;
	}
	
	/* theorie zegt: 1 richting: rechte lijn
     * meer aaneengesloten richtingen: cirkel
     * onderbroken en minstens 1 bit gezet in hoogste 4 bits: size
     */
	switch(dir){
	case 1:
		return ('g');
		break;
	case 3:
	case 7:
		x1 = mouse[0][0] - mouse[count >> 1][0];
		y1 = mouse[0][1] - mouse[count >> 1][1];
		x2 = mouse[count >> 1][0] - mouse[count - 1][0];
		y2 = mouse[count >> 1][1] - mouse[count - 1][1];
		d1 = (x1 * x1) + (y1 * y1);
		d2 = (x2 * x2) + (y2 * y2);
		sq = fsqrt(d1);
		x1 /= sq; 
		y1 /= sq;
		sq = fsqrt(d2);
		x2 /= sq; 
		y2 /= sq;
		inp = (x1 * x2) + (y1 * y2);
		/*printf("%f\n", inp);*/
		if (inp > 0.9) return ('g');
		else return ('r');
		break;
	case 15:
	case 31:
	case 63:
	case 127:
	case 255:
		return ('r');
		break;
	default:
		/* bij size moeten minstens een van de hogere bits gezet zijn */
		if (dir < 16) return ('r');
		else return ('s');
	}

	return (0);
}

#ifdef IRISGL

int gesture()
{
	short mcords[MOVES][2];
	int i= 1, end= 0, a;
	short mval[2], val;
	
	if(G.qual) return 0;
	
	getmouseco_areawin(mval);
	
	mcords[0][0] = mval[0];
	mcords[0][1] = mval[1];

	drawmode(OVERDRAW);
	if(curarea->spacetype==SPACE_VIEW3D) persp(0);
	else defwinmat();
	
	color(3);
	glBegin(GL_LINE_STRIP);

	while(get_mbut()&L_MOUSE) {
		
		switch (extern_qread(&val)) {
		case MOUSEY:
			getmouseco_areawin(mval);
			if (mval[0] != mcords[i-1][0] || mval[1] != mcords[i-1][1]) {
				mcords[i][0] = mval[0];
				mcords[i][1] = mval[1];
				glVertex2sv(mcords[i]);
				i++;
			}
			break;
		case MOUSEX:
			break;
		case LEFTMOUSE:
			break;
		default:
			end= 1;
			break;
		}
		if (i == MOVES || end == 1) break;
	}

	glEnd();
	
	/* clear */
	color(0);
	glBegin(GL_LINE_STRIP);
		for(a=1; a<i; a++) glVertex2sv(mcords[a]);
	glEnd();
	
	if(curarea->spacetype==SPACE_VIEW3D) persp(1);
	drawmode(NORMALDRAW);

	if (i > 5) {
		i = interpret_move(mcords, i);
		
		if(i) {
			if(curarea->spacetype==SPACE_IPO) transform_ipo(i);
			else if(curarea->spacetype==SPACE_IMAGE) transform_tface_uv(i);
			else if(curarea->spacetype==SPACE_OOPS) transform_oops('g');
			else transform(i);
		}
		
		return 1;
	}
	return 0;
}

#else

int gesture()
{
	short mcords[MOVES][2];
	int i= 1, end= 0, a;
	ushort event;
	short mval[2], val;
	
	glDrawBuffer(GL_FRONT);
	persp(0);	/* heeft ortho op pixelnivo */
	
	getmouseco_areawin(mval);
	
	mcords[0][0] = mval[0];
	mcords[0][1] = mval[1];
	
	while(get_mbut()&L_MOUSE) {
		
		event= extern_qread(&val);
	
		switch (event) {
		case MOUSEY:
			getmouseco_areawin(mval);
			if( abs(mval[0]-mcords[i-1][0])>3 || abs(mval[1]-mcords[i-1][1])>3 ) {
				mcords[i][0] = mval[0];
				mcords[i][1] = mval[1];
				if(i) {
					sdrawXORline(mcords[i-1][0], mcords[i-1][1], mcords[i][0], mcords[i][1]);
				}
				i++;
			}
			break;
		case MOUSEX:
			break;
		case LEFTMOUSE:
			break;
		default:
			if(event) end= 1;	/* blender returns 0 */
			break;
		}
		if (i == MOVES || end == 1) break;
	}

	for(a=1; a<i; a++) {
		sdrawXORline(mcords[a-1][0], mcords[a-1][1], mcords[a][0], mcords[a][1]);
	}
	
	persp(1);
	glDrawBuffer(GL_BACK);
	
	if (i > 2) {
		i = interpret_move(mcords, i);
		
		if(i) {
			if(curarea->spacetype==SPACE_IPO) transform_ipo(i);
			else if(curarea->spacetype==SPACE_IMAGE) transform_tface_uv(i);
			else if(curarea->spacetype==SPACE_OOPS) transform_oops('g');
			else transform(i);
		}
		
		return 1;
	}
	return 0;
}

#endif

void mouse_cursor()
{
	extern float zfac;	/* view.c */
	float dx, dy, fz, *fp, dvec[3], oldcurs[3];
	short mval[2], mx, my, lr_click=0;
	
	if(gesture()) return;
	
	getmouseco_areawin(mval);
	
	if(mval[0]!=G.vd->mx || mval[1]!=G.vd->my) {

		mx= mval[0];
		my= mval[1];
		
		fp= give_cursor();;
		
		if(G.obedit && ((G.qual & LR_CTRLKEY) || get_mbut()&R_MOUSE )) lr_click= 1;
		VECCOPY(oldcurs, fp);
		
		project_short_noclip(fp, mval);

		initgrabz(fp[0], fp[1], fp[2]);
		
		if(mval[0]!=3200) {
			
			window_to_3d(dvec, mval[0]-mx, mval[1]-my);
			VecSubf(fp, fp, dvec);
			
		}
		else {

			dx= ((float)(mx-(curarea->winx/2)))*zfac/(curarea->winx/2);
			dy= ((float)(my-(curarea->winy/2)))*zfac/(curarea->winy/2);
			
			fz= G.vd->persmat[0][3]*fp[0]+ G.vd->persmat[1][3]*fp[1]+ G.vd->persmat[2][3]*fp[2]+ G.vd->persmat[3][3];
			fz= fz/zfac;
			
			fp[0]= (G.vd->persinv[0][0]*dx + G.vd->persinv[1][0]*dy+ G.vd->persinv[2][0]*fz)-G.vd->ofs[0];
			fp[1]= (G.vd->persinv[0][1]*dx + G.vd->persinv[1][1]*dy+ G.vd->persinv[2][1]*fz)-G.vd->ofs[1];
			fp[2]= (G.vd->persinv[0][2]*dx + G.vd->persinv[1][2]*dy+ G.vd->persinv[2][2]*fz)-G.vd->ofs[2];
		}
		
		allqueue(REDRAWVIEW3D, 1);
	}
	
	if(lr_click) {
		if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) addvert_mesh();
		else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) addvert_Nurb(0);
		
		VECCOPY(fp, oldcurs);
	}
	
}

void deselectall()	/* is toggle */
{
	Base *base;
	int a=0;

	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) {
			a= 1;
			break;
		}
		base= base->next;
	}
	
	base= FIRSTBASE;
	while(base) {
		if(base->lay & G.vd->lay) {
			if(a) base->flag &= ~SELECT;
			else base->flag |= SELECT;
			base->object->flag= base->flag;
		}
		base= base->next;
	}

	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWDATASELECT, 0);
	
	countall();

}

void deselectall_ex(Base *b)   /* ALLES deselect behalve b */
{
	Base *base;

	base= FIRSTBASE;
	while(base) {
		if (base->flag & SELECT) {
			if(b!=base) {
			
				base->flag &= ~SELECT;
				base->object->flag= base->flag;
				draw_object_ext(base);	/* deze test op layer */
			}
		}
		base= base->next;
	}
	countall();
}

uint samplerect(uint *buf, int size, uint dontdo)
{
	Base *base;
	uint *bufmin,*bufmax;
	int a,b,rc,tel,aantal,dirvec[4][2],maxob;
	uint retval=0;
	
	base= LASTBASE;
	if(base==0) return 0;
	maxob= base->selcol;

	aantal= (size-1)/2;
	rc= 0;

	dirvec[0][0]= 1;
	dirvec[0][1]= 0;
	dirvec[1][0]= 0;
	dirvec[1][1]= -size;
	dirvec[2][0]= -1;
	dirvec[2][1]= 0;
	dirvec[3][0]= 0;
	dirvec[3][1]= size;

	bufmin= buf;
	bufmax= buf+ size*size;
	buf+= aantal*size+ aantal;

	for(tel=1;tel<=size;tel++) {

		for(a=0;a<2;a++) {
			for(b=0;b<tel;b++) {

				if(*buf && *buf<=maxob && *buf!=dontdo) return *buf;
				if( *buf==dontdo ) retval= dontdo;	/* als alleen kleur dontdo aanwezig is, wel dontdo teruggeven */
				
				buf+= (dirvec[rc][0]+dirvec[rc][1]);

				if(buf<bufmin || buf>=bufmax) return retval;
			}
			rc++;
			rc &= 3;
		}
	}
	return retval;
}

#define SELECTSIZE	51

void set_active_base(Base *base)
{
	
	BASACT= base;
	
	/* voorkeurnamen */
	set_obact_names(base->object);
	
	/* signalen naar buttons */
	redraw_test_buttons(base);
	
	/* signaal naar ipo */
	allqueue(REDRAWIPO, base->object->ipowin);

}

void set_active_object(Object *ob)
{
	Base *base;
	
	base= FIRSTBASE;
	while(base) {
		if(base->object==ob) {
			set_active_base(base);
			return;
		}
		base= base->next;
	}
}

void mouse_select()
{
	Base *base, *startbase=0, *basact=0, *oldbasact;
	uint rect[SELECTSIZE*SELECTSIZE], *dr, kleur=0;
	IGLuint buffer[MAXPICKBUF];
	int temp, a, dist=100;
	short hits, mval[2], x, y, nobackbuf=0;

	/* iedere keer lijst starten vanuit basact */
	startbase=  FIRSTBASE;
	if(BASACT && BASACT->next) startbase= BASACT->next;

#if defined(__WIN32) || defined(MESA31)
	nobackbuf= 1;
#endif

	getmouseco_areawin(mval);

	if(G.obedit==0 && (G.qual & LR_CTRLKEY)) {

		base= startbase;
		while(base) {
			
			if(base->lay & G.vd->lay) {
				
				project_short(base->object->obmat[3], &base->sx);
				
				temp= abs(base->sx -mval[0]) + abs(base->sy -mval[1]);
				if(base==BASACT) temp+=10;
				if(temp<dist ) {
					basact= base;
					dist= temp;
				}
			}
			base= base->next;
			
			if(base==0) base= FIRSTBASE;
			if(base==startbase) break;
		}
		
		/* volledige redraw als */
		if(G.f & (G_VERTEXPAINT+G_FACESELECT)) allqueue(REDRAWVIEW3D, 0);
		
	}
	else if( (G.qual & LR_CTRLKEY) || nobackbuf) {
		
		hits= selectprojektie(buffer, mval[0]-7, mval[1]-7, mval[0]+7, mval[1]+7);
		if(hits==0) hits= selectprojektie(buffer, mval[0]-21, mval[1]-21, mval[0]+21, mval[1]+21);

		if(hits>0) {

			base= startbase;
			while(base) {
				if(base->lay & G.vd->lay) {
					for(a=0; a<hits; a++) {
						if(base->selcol==buffer[ SELBUFFER_INDEX(a) ]) basact= base;
					}
				}
				if(basact) break;
				
				base= base->next;
				if(base==0) base= FIRSTBASE;
				if(base==startbase) break;
			}
		}
	}
	else {
		/* met rectread testen */
		
		if(curarea->win_swap==WIN_EQUAL) G.vd->flag |= V3D_NEEDBACKBUFDRAW;
		if(G.vd->flag & V3D_NEEDBACKBUFDRAW) {
			backdrawview3d(0);
		}
		
		countall();
		
		a= SELECTSIZE*SELECTSIZE;
		dr= rect;
		while(a--) *(dr++)= 0;
		a= (SELECTSIZE-1)/2;
		x= curarea->winrct.xmin;
		y= curarea->winrct.ymin;
		
		winset(G.curscreen->mainwin);
		
		glReadBuffer(GL_BACK);
		glReadPixels(x+mval[0]-a,  y+mval[1]-a, SELECTSIZE, SELECTSIZE, GL_RGBA, GL_UNSIGNED_BYTE,  rect);
		
		if(G.order==B_ENDIAN) convert_rgba_to_abgr(SELECTSIZE*SELECTSIZE, rect);
		
		dr= rect;
		for(y= mval[1]-a; y<=mval[1]+a; y++) {
			for(x=mval[0]-a; x<=mval[0]+a; x++) {
			
				if(x<0 || y<0 || x>=curarea->winx || y>=curarea->winy) *dr= 0;
				else {

					if(G.f & G_DEBUG) {
						if( (dr[0] & 0xF0F0F0) && dr[0]!=dr[1]) printf("select %x %x\n", *dr, *dr & 0xF0F0F0);
					}

					*dr &= 0xF0F0F0;

				}
				dr++;
			}
		}
			/* niet 2 x de zelfde */
		basact= BASACT;
		if(basact && TESTBASE(basact) ) {
			kleur= samplerect(rect, SELECTSIZE, basact->selcol);
		} else {
			kleur= samplerect(rect, SELECTSIZE, 0);
		}

		if(kleur==0) return;
		else {
			base= FIRSTBASE;
			while(base) {
				if(base->lay & G.vd->lay) {
					if( (base->selcol & 0xF0F0F0) == kleur) break;
				}
				base= base->next;
			}
		}
		basact= base;
	}
	
	if(basact) {
		if(G.obedit) {
			/* alleen select doen */
			deselectall_ex(BASACT);
			basact->flag |= SELECT;
			draw_object_ext(basact);
		}
		else {
			oldbasact= BASACT;
			BASACT= basact;
			
			if((G.qual & LR_SHIFTKEY)==0) {
				deselectall_ex(basact);
				basact->flag |= SELECT;
			}
			else {
				if(oldbasact) if(oldbasact != basact) draw_object_ext(oldbasact);
				
				if(basact->flag & SELECT) {
					if(basact==oldbasact)
						basact->flag &= ~SELECT;
				}
				else basact->flag |= SELECT;
			}
			
			if((basact->flag & SELECT)==0) BASACT= 0;
			basact->object->flag= basact->flag;
			
			draw_object_ext(basact);

			if(oldbasact != basact) {
			
				set_active_base(basact);
				
			}
			allqueue(REDRAWDATASELECT, 0);
		}
		
	}

	countall();

	rightmouse_transform();
}


void borderselect()
{
	rcti rect;
	Base *base;
	Nurb *nu;
	BezTriple *bezt;
	BPoint *bp;
	MetaElem *ml;
	struct EditVert *eve;
	IGLuint buffer[MAXPICKBUF];
	int a;
	short hits, val, tel;
	
	if(G.obedit==0 && (G.f & G_FACESELECT)) {
		face_borderselect();
		return;
	}
	
	val= get_border(&rect, 3);
	if(val) {
		if(G.obedit) {
			if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) {
				
				calc_meshverts_ext();	/* drawobject.c */
				eve= G.edve.first;
				while(eve) {
					if(eve->h==0 && eve->xs>rect.xmin && eve->xs<rect.xmax) {
						if(eve->ys>rect.ymin && eve->ys<rect.ymax) {
							if(val==LEFTMOUSE) eve->f|= 1;
							else eve->f&= 254;
						}
					}
					eve= eve->next;
				}
				if(val!=LEFTMOUSE) tekenvertices_ext(0);
				tekenvertices_ext(1);
				
			}
			else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) {
				
				calc_nurbverts_ext();	/* drawobject.c */
				nu= editNurb.first;
				while(nu) {
					if((nu->type & 7)==CU_BEZIER) {
						bezt= nu->bezt;
						a= nu->pntsu;
						while(a--) {
							if(bezt->hide==0) {
								if(bezt->s[0][0]>rect.xmin && bezt->s[0][0]<rect.xmax) {
									if(bezt->s[0][1]>rect.ymin && bezt->s[0][1]<rect.ymax) {
										if(val==LEFTMOUSE) bezt->f1|= 1;
										else bezt->f1 &= ~1;
									}
								}
								if(bezt->s[1][0]>rect.xmin && bezt->s[1][0]<rect.xmax) {
									if(bezt->s[1][1]>rect.ymin && bezt->s[1][1]<rect.ymax) {
										if(val==LEFTMOUSE) {
											bezt->f1|= 1; bezt->f2|= 1; bezt->f3|= 1;
										}
										else {
											bezt->f1 &= ~1; bezt->f2 &= ~1; bezt->f3 &= ~1;
										}
									}
								}
								if(bezt->s[2][0]>rect.xmin && bezt->s[2][0]<rect.xmax) {
									if(bezt->s[2][1]>rect.ymin && bezt->s[2][1]<rect.ymax) {
										if(val==LEFTMOUSE) bezt->f3|= 1;
										else bezt->f3 &= ~1;
									}
								}
							}
							bezt++;
						}
					}
					else {
						bp= nu->bp;
						a= nu->pntsu*nu->pntsv;
						while(a--) {
							if(bp->hide==0) {
								if(bp->s[0]>rect.xmin && bp->s[0]<rect.xmax) {
									if(bp->s[1]>rect.ymin && bp->s[1]<rect.ymax) {
										if(val==LEFTMOUSE) bp->f1|= 1;
										else bp->f1 &= ~1;
									}
								}
							}
							bp++;
						}
					}
					nu= nu->next;
				}
				allqueue(REDRAWVIEW3D, 0);
			}
			else if(G.obedit->type==OB_MBALL) {
				hits= selectprojektie(buffer, rect.xmin, rect.ymin, rect.xmax, rect.ymax);
				
				ml= editelems.first;
				
				while(ml) {
					for(a=0; a<hits; a++) {
						if(ml->selcol==buffer[ SELBUFFER_INDEX(a) ]) {
							if(val==LEFTMOUSE) ml->flag |= SELECT;
							else ml->flag &= ~SELECT;
							break;
						}
					}
					ml= ml->next;
				}
				allqueue(REDRAWVIEW3D, 0);
			}
			else if(G.obedit->type==OB_LATTICE) {
				
				calc_lattverts_ext();
				
				bp= editLatt->def;
	
				a= editLatt->pntsu*editLatt->pntsv*editLatt->pntsw;
				while(a--) {
					if(bp->hide==0) {
						if(bp->s[0]>rect.xmin && bp->s[0]<rect.xmax) {
							if(bp->s[1]>rect.ymin && bp->s[1]<rect.ymax) {
								if(val==LEFTMOUSE) bp->f1|= 1;
								else bp->f1 &= ~1;
							}
						}
					}
					bp++;
				}
				allqueue(REDRAWVIEW3D, 0);
			}

		}
		else {
			
			hits= selectprojektie(buffer, rect.xmin, rect.ymin, rect.xmax, rect.ymax);

			base= FIRSTBASE;
			while(base) {
				if(base->lay & G.vd->lay) {
					for(a=0; a<hits; a++) {
					
						if(base->selcol==buffer[ SELBUFFER_INDEX(a) ]) {
							if(val==LEFTMOUSE) base->flag |= SELECT;
							else base->flag &= ~SELECT;
							base->object->flag= base->flag;

							draw_object_ext(base);
							break;
						}
					}
				}
				
				base= base->next;
			}
			allqueue(REDRAWDATASELECT, 0);
			
			/* i.v.m. backbufprojektie */
			tel= 1;
			base= FIRSTBASE;
			while(base) {
				/* elke base ivm meerdere windows */
				base->selcol= ((tel & 0xF00)<<12) + ((tel & 0xF0)<<8) + ((tel & 0xF)<<4);
				tel++;
				base= base->next;
			}
		}
		countall();
		
		allqueue(REDRAWINFO, 0);
	}
}

void sdrawXORcirc(short xofs, short yofs, float rad)
{
	float si, co, dphi, phi;
	short tot, sval[2], svalo[2];
	
	tot= 20;
	dphi= 2*M_PI/(float)tot;
	phi= 0.0;
	
	while(tot-- >= 0) {
		si= fsin(phi);
		co= fcos(phi);
		
		svalo[0]= sval[0];
		svalo[1]= sval[1];
		sval[0]= (rad*si);
		sval[1]= (rad*co);
		
		if(tot<19) {
			sdrawXORline(xofs+sval[0], yofs+sval[1], xofs+svalo[0], yofs+svalo[1]);
		}
		
		phi+= dphi;
	}
}

#ifdef IRISGL

void draw_sel_circle(short *mval, short *mvalo, float rad, float rado, int selecting)
{

	/* cirkel tekenen */
	persp(0);
	
	drawmode(PUPDRAW);

	if(mvalo) {
		color(0);
		circ((float)mvalo[0], (float)mvalo[1], rado);
	}
	
	if(mval) {
		if(selecting==LEFTMOUSE) mapcolor(2, 255, 255, 50);
		else if(selecting==MIDDLEMOUSE) mapcolor(2, 255, 50, 255);
		else mapcolor(2, 255, 255, 255);

		color(2);
		circ((float)mval[0], (float)mval[1], rad);
	}
	
	drawmode(NORMALDRAW);	
	
	persp(1);
}

#else

void draw_sel_circle(short *mval, short *mvalo, float rad, float rado, int selecting)
{
	static short no_mvalo=0;
	int xof, yof;

	if(mval==0 && mvalo==0) {	/* signal */
		no_mvalo= 1;
		return;
	}

	persp(0);
	glDrawBuffer(GL_FRONT);

	/* cirkel tekenen */

	if(mvalo && no_mvalo==0) {
		sdrawXORcirc(mvalo[0], mvalo[1], rado);
	}
	
	if(mval) {
		sdrawXORcirc(mval[0], mval[1], rad);
	}
	
	persp(1);
	glDrawBuffer(GL_BACK);
	
	no_mvalo= 0;
}

#endif

void circle_select()
{
	Nurb *nu;
	BezTriple *bezt;
	BPoint *bp;
	EditVert *eve;
	static float rad= 40.0;
	float rado, x, y, trad;
	int a, b, firsttime=1;
	ushort event;
	short oldr, oldb, oldg, mvalo[2], mval[2], val;
	short selecting=0, afbreek=0;
	
	if(G.obedit==0) return;
	
	getmouseco_areawin(mvalo);
	draw_sel_circle(mvalo, 0, rad, rado, selecting);
	
	rado= rad;
	
	while(TRUE) {
		
		/* als een renderwinodow open is en de muis gaat erin */
		winset(curarea->win);

		getmouseco_areawin(mval);
		
		if(mval[0]!=mvalo[0] || mval[1]!=mvalo[1] || rado!=rad || firsttime) {
			firsttime= 0;
			
			draw_sel_circle(mval, mvalo, rad, rado, selecting);
		
			mvalo[0]= mval[0];
			mvalo[1]= mval[1];
			rado= rad;

			if(selecting) {
				
				if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) {
					
					calc_meshverts_ext();	/* drawobject.c */
					eve= G.edve.first;
					while(eve) {
						if(eve->h==0) {
							x= eve->xs-mval[0];
							y= eve->ys-mval[1];
							trad= fsqrt(x*x+y*y);
							if(trad<=rad) {
								if(selecting==LEFTMOUSE) eve->f|= 1;
								else eve->f&= 254;
							}
						}
						eve= eve->next;
					}
					if(G.f & (G_FACESELECT+G_DRAWFACES)) {
						draw_sel_circle(0, 0, 0, 0, 0);	/* signal */
						force_draw();
					}
					else {
						if(selecting!=LEFTMOUSE) tekenvertices_ext(0);
						/* altijd geselecteerde vertices tekenen */
						tekenvertices_ext(1);
					}
				}
				else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) {
					
					calc_nurbverts_ext();	/* drawobject.c */
					nu= editNurb.first;
					while(nu) {
						if((nu->type & 7)==CU_BEZIER) {
							bezt= nu->bezt;
							a= nu->pntsu;
							while(a--) {
								if(bezt->hide==0) {
									x= bezt->s[0][0]-mval[0];
									y= bezt->s[0][1]-mval[1];
									trad= fsqrt(x*x+y*y);
									if(trad<=rad) {
										if(selecting==LEFTMOUSE) bezt->f1|= 1;
										else bezt->f1 &= ~1;
									}
									x= bezt->s[1][0]-mval[0];
									y= bezt->s[1][1]-mval[1];
									trad= fsqrt(x*x+y*y);
									if(trad<=rad) {
										if(selecting==LEFTMOUSE) bezt->f2|= 1;
										else bezt->f2 &= ~1;
									}
									x= bezt->s[2][0]-mval[0];
									y= bezt->s[2][1]-mval[1];
									trad= fsqrt(x*x+y*y);
									if(trad<=rad) {
										if(selecting==LEFTMOUSE) bezt->f3|= 1;
										else bezt->f3 &= ~1;
									}
									
								}
								bezt++;
							}
						}
						else {
							bp= nu->bp;
							a= nu->pntsu*nu->pntsv;
							while(a--) {
								if(bp->hide==0) {
									x= bp->s[0]-mval[0];
									y= bp->s[1]-mval[1];
									trad= fsqrt(x*x+y*y);
									if(trad<=rad) {
										if(selecting==LEFTMOUSE) bp->f1|= 1;
										else bp->f1 &= ~1;
									}
								}
								bp++;
							}
						}
						nu= nu->next;
					}
					draw_sel_circle(0, 0, 0, 0, 0);	/* signal */
					force_draw();
				}
				else if(G.obedit->type==OB_LATTICE) {
					calc_lattverts_ext();
					
					bp= editLatt->def;
		
					a= editLatt->pntsu*editLatt->pntsv*editLatt->pntsw;
					while(a--) {
						if(bp->hide==0) {
							x= bp->s[0]-mval[0];
							y= bp->s[1]-mval[1];
							trad= fsqrt(x*x+y*y);
							if(trad<=rad) {
								if(selecting==LEFTMOUSE) bp->f1|= 1;
								else bp->f1 &= ~1;
							}
						}
						bp++;
					}
					draw_sel_circle(0, 0, 0, 0, 0);	/* signal */
					force_draw();
				}
			}
		}

		event= 0;
		
		while(qtest()) {
		
			event= extern_qread(&val);
			switch(event) {
			
			case LEFTMOUSE:
			case MIDDLEMOUSE:
				if(val) selecting= event;
				else selecting= 0;
				firsttime= 1;
				
				break;
			case PADPLUSKEY:
				if(val) if(rad<200.0) rad*= 1.2;
				break;
			case PADMINUS:
				if(val) if(rad>5.0) rad/= 1.2;
				break;
			
			case ESCKEY: case SPACEKEY: case RIGHTMOUSE:
			case GKEY: case SKEY: case RKEY: case XKEY: case EKEY: case TABKEY:
				afbreek= 1;
				break;

			}
			
			if(afbreek) break;
			
		}
		
		
		if(afbreek) break;
	}
	
	/* cirkel wissen */
	draw_sel_circle(0, mvalo, 0, rad, 1);

	countall();
	allqueue(REDRAWINFO, 0);
}

void set_render_border()
{
	rcti rect;
	float x1, y1, x2, y2;
	short val;

	if(G.vd->persp!=2) return;
	
	val= get_border(&rect, 2);
	if(val) {

		x1= G.vd->pr_xmin;
		y1= G.vd->pr_ymin;
		x2= G.vd->pr_xmax;
		y2= G.vd->pr_ymax;

		G.scene->r.border.xmin=  (rect.xmin-x1)/(x2-x1);
		G.scene->r.border.ymin=  (rect.ymin-y1)/(y2-y1);
		G.scene->r.border.xmax=  (rect.xmax-x1)/(x2-x1);
		G.scene->r.border.ymax=  (rect.ymax-y1)/(y2-y1);
		
		CLAMP(G.scene->r.border.xmin, 0.0, 1.0);
		CLAMP(G.scene->r.border.ymin, 0.0, 1.0);
		CLAMP(G.scene->r.border.xmax, 0.0, 1.0);
		CLAMP(G.scene->r.border.ymax, 0.0, 1.0);
		
		allqueue(REDRAWVIEWCAM, 1);
	}
}



void fly()
{
	extern int displaysizex, displaysizey;
	float speed=0.0, speedo=1.0, zspeed=0.0, dvec[3], *quat, mat[3][3];
	float oldvec[3], oldrot[3];
	int loop=1;
	ushort toets;
	short val, cent[2];
	short mval[2];
	
	if(curarea->spacetype!=SPACE_VIEW3D) return;
	if(G.vd->camera == 0) return;
	if(G.vd->persp<2) return;
	
	VECCOPY(oldvec, G.vd->camera->loc);
	VECCOPY(oldrot, G.vd->camera->rot);
	
	cent[0]= curarea->winrct.xmin+(curarea->winx)/2;
	cent[1]= curarea->winrct.ymin+(curarea->winy)/2;
	
	warp_pointer(cent[0], cent[1]);
	
	cent[0]=  (curarea->winx)/2;
	cent[1]=  (curarea->winy)/2;
	
	headerprint("Fly");
	
	while(loop) {
		getmouseco_areawin(mval);

		while(qtest()) {
			
			toets= extern_qread(&val);
			
			if(val) {
				if(toets==ESCKEY) {
					VECCOPY(G.vd->camera->loc, oldvec);
					VECCOPY(G.vd->camera->rot, oldrot);
					loop= 0;
					break;
				}
				else if(toets==SPACEKEY) {
					loop= 0;
					break;
				}
				else if(toets==LEFTMOUSE) {
					speed+= G.vd->grid/75.0;
					if(get_mbut()&M_MOUSE) speed= 0.0;
				}
				else if(toets==MIDDLEMOUSE) {
					speed-= G.vd->grid/75.0;
					if(get_mbut()&L_MOUSE) speed= 0.0;
				}
			}
		}
		if(loop==0) break;
		
		/* dvec bepalen */
		val= mval[0]-cent[0];
		if(val>20) val-= 20; else if(val< -20) val+= 20; else val= 0;
		dvec[0]= 0.000001*val*val;
		if(val>0) dvec[0]= -dvec[0];
		
		val= mval[1]-cent[1];
		if(val>20) val-= 20; else if(val< -20) val+= 20; else val= 0;
		dvec[1]= 0.000001*val*val;
		if(val>0) dvec[1]= -dvec[1];
		
		dvec[2]= 1.0;
		
		zspeed= 0.0;
		if(get_qual()&LR_CTRLKEY) zspeed= -G.vd->grid/25.0;
		if(get_qual()&LR_ALTKEY) zspeed= G.vd->grid/25.0;
		
		if(speedo!=0.0 || zspeed!=0.0 || dvec[0]!=0.0 || dvec[1]!=0.0) {
		
			Normalise(dvec);
			
			Mat3CpyMat4(mat, G.vd->viewinv);
			Mat3MulVecfl(mat, dvec);
			quat= vectoquat(dvec, 5, 1);	/* track en upflag, niet die van de base: cameraview-berekening gebruikt ze niet */
			
			QuatToEul(quat, G.vd->camera->rot);
			
			compatible_eul(G.vd->camera->rot, oldrot);
			
			VecMulf(dvec, speed);
			G.vd->camera->loc[0]-= dvec[0];
			G.vd->camera->loc[1]-= dvec[1];
			G.vd->camera->loc[2]-= (dvec[2]-zspeed);
			
			winset(curarea->win);
			curarea->windraw();
			screen_swapbuffers();
		}
		speedo= speed;
	}
	
	allqueue(REDRAWVIEW3D, 0);
	addqueue(curarea->headwin, REDRAW, 1);
	
}


