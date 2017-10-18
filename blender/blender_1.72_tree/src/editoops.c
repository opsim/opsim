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


/*  editoops.c      GRAPHICS
 * 
 *  feb 97
 *  
 *  
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "edit.h"
#include "oops.h"



typedef struct TransOops {
	float *loc;
	float oldloc[2];
} TransOops;


void oops_to_select_objects()
{
	Oops *oops;
	Base *base;
	Object *ob;

	if(G.soops==0) return;	

	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {	
			if(oops->type==ID_OB) {
				ob= (Object *)oops->id;
				if(oops->flag & SELECT) ob->flag |= SELECT;
				else ob->flag &= ~SELECT;
			}
		}
		oops= oops->next;
	}
	base= FIRSTBASE;
	while(base) {
		if(base->flag != base->object->flag) {
			base->flag= base->object->flag;
		}
		base= base->next;
	}

	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWOOPS, 0);
}

void swap_select_all_oops()
{
	Oops *oops;
	int sel= 0;
	
	if(G.soops==0) return;	

	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {	
			if(oops->flag & SELECT) {
				sel= 1;
				break;
			}
		}
		oops= oops->next;
	}

	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {	
			if(sel) oops->flag &= ~SELECT;
			else oops->flag |= SELECT;
		}
		oops= oops->next;
	}
	
	oops_to_select_objects();	/* ook redr */
	
	G.soops->lockpoin= 0;
}

void select_swap_oops()
{
	Oops *oops;
	
	if(G.soops==0) return;	

	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {	
			if(oops->flag & SELECT) oops->flag &= ~SELECT;
			else oops->flag |= SELECT;
		}
		oops= oops->next;
	}
	
	G.soops->lockpoin= 0;
	oops_to_select_objects();	/* ook redr */
}

void deselect_all_oops()
{
	Oops *oops;
	
	if(G.soops==0) return;	

	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {	
			oops->flag &= ~SELECT;
		}
		oops= oops->next;
	}
	G.soops->lockpoin= 0;
}

void set_select_flag_oops()	/* alle areas */
{
	SpaceOops *so;
	ScrArea *sa;
	
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->spacetype==SPACE_OOPS) {
			so= sa->spacedata.first;
			so->flag |= SO_NEWSELECTED;
		}
		sa= sa->next;
	}
	if(G.soops) G.soops->lockpoin= 0;
}

void deselect_all_area_oops()	/* alle areas */
{
	SpaceOops *so;
	Oops *oops;
	ScrArea *sa;
	
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->spacetype==SPACE_OOPS) {
			so= sa->spacedata.first;
			
			oops= so->oops.first;
			while(oops) {
				oops->flag &= ~SELECT;
				oops= oops->next;
			}
		}
		sa= sa->next;
	}
	
	if(G.soops) G.soops->lockpoin= 0;
}

void transform_oops(int mode)
{
	TransOops *transmain, *tv;
	Oops *oops;
	float dx, dy, div, dvec[2], cent[2], min[2], max[2];
	float sizefac, size[2], xref=1.0, yref=1.0;
	int a, tot= 0, midtog= 0;
	ushort event;
	short firsttime= 1, proj, afbreek=0, xc, yc, xo, yo, xn, yn, mval[2];
	short val;
	char str[32];
	
	if(G.soops==0) return;	
		
	/* welke oopsen doen mee */
	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {	
			if(oops->flag & SELECT) {
				tot++;
			}
		}
		oops= oops->next;
	}
	
	if(tot==0) return;
	
	G.moving= 1;
	
	tv=transmain= callocN(tot*sizeof(TransOops), "transmain");
	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {	
			if(oops->flag & SELECT) {
				tv->loc= &oops->x;
				tv->oldloc[0]= tv->loc[0];
				tv->oldloc[1]= tv->loc[1];
				DO_MINMAX2(tv->loc, min, max);
				tv++;
			}
		}
		oops= oops->next;
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
				
				tv= transmain;
				for(a=0; a<tot; a++, tv++) {
					
					tv->loc[0]= tv->oldloc[0]+dvec[0];
					tv->loc[1]= tv->oldloc[1]+dvec[1];
						
				}
			
				sprintf(str, "X: %.2f   Y: %.2f  ", dvec[0], dvec[1]);
				headerprint(str);
			}
			else if(mode=='s') {
				size[0]=size[1]= (fsqrt( (float)((yc-mval[1])*(yc-mval[1])+(mval[0]-xc)*(mval[0]-xc)) ))/sizefac;
				
				if(midtog) size[proj]= 1.0;
				size[0]*= xref;
				size[1]*= yref;
				
				tv= transmain;
				for(a=0; a<tot; a++, tv++) {
				
					tv->loc[0]= size[0]*(tv->oldloc[0]-cent[0])+ cent[0];
					tv->loc[1]= size[1]*(tv->oldloc[1]-cent[1])+ cent[1];
					
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
			
	G.moving= 0;

	addqueue(curarea->headwin, REDRAW, 1);
	addqueue(curarea->win, REDRAW, 1);
}

Oops *find_nearest_oops()
{
	Oops *oops;
	float x, y, facx, facy;
	short mval[2];
	
	getmouseco_areawin(mval);
	areamouseco_to_ipoco(mval, &x, &y);
	
	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide == 0) {
			if(oops->x <=x && oops->x+OOPSX >= x) {
				if(oops->y <=y && oops->y+OOPSY >= y) {		
					return oops;
				}
			}
		}
		oops= oops->next;
	}
	return 0;
}

void do_activate_oops(Oops *oops)
{
	Base *base;
	Object *ob;
	
	switch(oops->type) {
	case ID_SCE:
		if(oops->id) set_scene((Scene *)oops->id);
		break;
	case ID_OB:
		base= FIRSTBASE;
		while(base) {
			if(base->object == (Object *)oops->id) break;
			base= base->next;
		}
		if(base) {
			set_active_base(base);	/* editview.c */
			allqueue(REDRAWVIEW3D, 0);
			allqueue(REDRAWINFO, 1);
		}
		break;
	case ID_MA:
		ob= OBACT;
		if(ob && oops->id) {
			assign_material(ob, (Material *)oops->id, ob->actcol);
			allqueue(REDRAWBUTSMAT, 0);
			addqueue(curarea->win, REDRAW, 1);
		}
		break;
		
	}
}

void mouse_select_oops()
{
	Oops *oops;
	extern float oopslastx, oopslasty;	/* oops.c */
	short xo, yo, mval[2];
	
	if(G.soops==0) return;	
		
	/* welke oopsen doen mee */
	oops= G.soops->oops.first;

	oops= find_nearest_oops();
	if(oops==0) return;
	
	if((G.qual & LR_SHIFTKEY)==0) deselect_all_oops();
	
	if(oops) {
		/* last_seq= seq; */
		
		if(G.qual==0) {
			oops->flag |= SELECT;
		}
		else {
			if(oops->flag & SELECT) {
				oops->flag &= ~SELECT;
			}
			else {
				oops->flag |= SELECT;
			}
		}
		
		oopslastx= oops->x;
		oopslasty= oops->y;
		
		if(G.qual & LR_CTRLKEY) do_activate_oops(oops);
		G.soops->lockpoin= oops;
	}
	
	oops_to_select_objects();	/* ook redr */
	addqueue(curarea->headwin, REDRAW, 1);
	
	force_draw();
	
	std_rmouse_transform(transform_oops);
}

void borderselect_oops()
{
	Oops *oops;
	rcti rect;
	rctf rectf, rq;
	int a, b, val, ok;
	short mval[2];

	if(G.soops==0) return;	
	
	val= get_border(&rect, 3);

	if(val) {
		mval[0]= rect.xmin;
		mval[1]= rect.ymin;
		areamouseco_to_ipoco(mval, &rectf.xmin, &rectf.ymin);
		mval[0]= rect.xmax;
		mval[1]= rect.ymax;
		areamouseco_to_ipoco(mval, &rectf.xmax, &rectf.ymax);

		oops= G.soops->oops.first;
		while(oops) {
			if(oops->hide == 0) {
			
				rq.xmin= oops->x;
				rq.xmax= oops->x+OOPSX;
				rq.ymin= oops->y;
				rq.ymax= oops->y+OOPSY;
		
				if(isect_rctf(&rq, &rectf, 0)) {
					if(val==LEFTMOUSE) {
						oops->flag |= SELECT;
					}
					else {
						oops->flag &= ~SELECT;
					}
				}
			}
			oops= oops->next;
		}

		oops_to_select_objects();	/* ook redr */
	}
}

void select_oops_lib(ID *id)
{
	Oops *oops;
	
	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {	
			if(oops->id->lib== (Library *)id) oops->flag |= OOPS_DOSELECT;
		}
		oops= oops->next;
	}
}

void select_linked_oops()
{
	Oops *oops;
	OopsLink *ol;
	
	if(G.soops==0) return;	

	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {	
			if(oops->flag & SELECT) {
				if(oops->type==ID_LI) select_oops_lib(oops->id);
				ol= oops->link.first;
				while(ol) {
					if(ol->to && ol->to->hide==0) ol->to->flag |= OOPS_DOSELECT;
					ol= ol->next;
				}
			}
		}
		oops= oops->next;
	}
	
	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {	
			if(oops->flag & OOPS_DOSELECT) {
				oops->flag |= SELECT;
				oops->flag &= ~OOPS_DOSELECT;
			}
		}
		oops= oops->next;
	}
	
	oops_to_select_objects();	/* ook redr */
	
}

void select_backlinked_oops()
{
	Oops *oops;
	OopsLink *ol;
	
	if(G.soops==0) return;	

	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {	
			if( (oops->flag & SELECT)==0) {
				ol= oops->link.first;
				while(ol) {
					if(ol->to && ol->to->hide==0) {
						if(ol->to->flag & SELECT) oops->flag |= OOPS_DOSELECT;
					}
					ol= ol->next;
				}
			}
		}
		oops= oops->next;
	}
	
	oops= G.soops->oops.first;
	while(oops) {
		if(oops->hide==0) {	
			if(oops->flag & OOPS_DOSELECT) {
				oops->flag |= SELECT;
				oops->flag &= ~OOPS_DOSELECT;
			}
		}
		oops= oops->next;
	}
	
	oops_to_select_objects();	/* ook redr */
	
}

