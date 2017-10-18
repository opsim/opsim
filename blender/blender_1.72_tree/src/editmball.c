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


/*  editmball.c      GRAPHICS
 * 
 *  mei 95
 *  
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "edit.h"
#include "render.h"


extern short editbutflag;

ListBase editelems= {0, 0};
MetaElem *lastelem;

void make_editMball()
{
	MetaBall *mb;
	MetaElem *ml, *new;

	freelistN(&editelems);
	lastelem= 0;
	
	mb= G.obedit->data;
	ml= mb->elems.first;
	
	while(ml) {
		new= dupallocN(ml);
		addtail(&editelems, new);
		if(ml->flag & SELECT) lastelem= new;
		
		ml= ml->next;
	}

	allqueue(REDRAWBUTSEDIT, 0);

	countall();
}

void load_editMball()
{
	/* load mball in object */
	MetaBall *mb;
	MetaElem *ml, *new;

	if(G.obedit==0) return;
	
	mb= G.obedit->data;
	freelistN(&(mb->elems));


	ml= editelems.first;
	while(ml) {
		new= dupallocN(ml);
		addtail(&(mb->elems), new);
		
		ml= ml->next;
	}
}

void add_primitiveMball()
{
	MetaBall *mb;
	MetaElem *ml;
	float *curs, mat[3][3], cent[3], imat[3][3], cmat[3][3];
	int newob=0;

	if(G.scene->id.lib) return;
	if(curarea->spacetype!=SPACE_VIEW3D) return;
	
	check_editmode(OB_MBALL);

	/* als geen obedit: nieuw object en in editmode gaan */
	if(G.obedit==0) {
		add_object(OB_MBALL);
		G.obedit= BASACT->object;
		
		where_is_object(G.obedit);

		make_editMball();
		setcursor_space(SPACE_VIEW3D, CURSOR_EDIT);
		newob= 1;
	}
	
	/* deselecteren */
	ml= editelems.first;
	while(ml) {
		ml->flag &= ~SELECT;
		ml= ml->next;
	}
	
	/* imat en centrum en afmeting */
	Mat3CpyMat4(mat, G.obedit->obmat);

	curs= give_cursor();
	VECCOPY(cent, curs);
	cent[0]-= G.obedit->obmat[3][0];
	cent[1]-= G.obedit->obmat[3][1];
	cent[2]-= G.obedit->obmat[3][2];

	Mat3CpyMat4(imat, G.vd->viewmat);
	Mat3MulVecfl(imat, cent);
	Mat3MulMat3(cmat, imat, mat);
	Mat3Inv(imat,cmat);
	
	Mat3MulVecfl(imat, cent);

	ml= callocN(sizeof(MetaElem), "metaelem");
	addtail(&editelems, ml);

	ml->x= cent[0];
	ml->y= cent[1];
	ml->z= cent[2];
	ml->rad= 1.0;
	ml->lay= 1;
	ml->s= 2.0;
	ml->len= 1.0;
	ml->expx= ml->expy= ml->expz= 2.0;
	ml->flag= SELECT;
	
	lastelem= ml;
	
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWBUTSALL, 0);
	makeDispList(G.obedit);
}

void deselectall_mball()
{
	MetaElem *ml;
	int sel= 0;
	
	ml= editelems.first;
	while(ml) {
		if(ml->flag & SELECT) break;
		ml= ml->next;
	}

	if(ml) sel= 1;

	ml= editelems.first;
	while(ml) {
		if(sel) ml->flag &= ~SELECT;
		else ml->flag |= SELECT;
		ml= ml->next;
	}
	allqueue(REDRAWVIEW3D, 0);
}

void mouse_mball()
{
	static MetaElem *startelem=0;
	MetaElem *ml, *act=0;
	int a, hits;
	IGLuint buffer[MAXPICKBUF];
	
	hits= selectprojektie(buffer, 0, 0, 0, 0);

	/* bestaat startelem? */
	ml= editelems.first;
	while(ml) {
		if(ml==startelem) break;
		ml= ml->next;
	}
	if(ml==0) startelem= editelems.first;
	
	if(hits>0) {
		ml= startelem;
		while(ml) {
			/* if(base->lay & G.vd->lay) { */
			
				for(a=0; a<hits; a++) {
					if(ml->selcol==buffer[ SELBUFFER_INDEX(a) ]) act= ml;
				}
			/* } */
			
			if(act) break;
			
			ml= ml->next;
			if(ml==0) ml= editelems.first;
			if(ml==startelem) break;
		}
		if(act) {
			if((G.qual & LR_SHIFTKEY)==0) {
				deselectall_mball();
				if(act->flag & SELECT) deselectall_mball();
				act->flag |= SELECT;
			}
			else {
				if(act->flag & SELECT) {
					act->flag &= ~SELECT;
				}
				else act->flag |= SELECT;
			}
			lastelem= act;
			allqueue(REDRAWVIEW3D, 0);
			allqueue(REDRAWBUTSEDIT, 0);
		}
	}
	rightmouse_transform();
}

void adduplicate_mball()
{
	MetaElem *ml, *new;
	
	ml= editelems.last;
	while(ml) {
		if(ml->flag & SELECT) {
			new= dupallocN(ml);
			addtail(&editelems, new);
			lastelem= new;
			ml->flag &= ~SELECT;
		}
		ml= ml->prev;
	}
	
	transform('g');
	allqueue(REDRAWBUTSEDIT, 0);
}

void delete_mball()
{
	MetaElem *ml, *next;
	
	if(okee("Erase selected")==0) return;
	
	ml= editelems.first;
	while(ml) {
		next= ml->next;
		if(ml->flag & SELECT) {
			if(lastelem==ml) lastelem= 0;
			remlink(&editelems, ml);
			freeN(ml);
		}
		ml= next;
	}
	
	makeDispList(G.obedit);
	allqueue(REDRAWVIEW3D, 0);
	allqueue(REDRAWBUTSEDIT, 0);
}

