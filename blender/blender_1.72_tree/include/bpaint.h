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

/* bpaint.h    maart 97

 * 
 * 
 */

#ifndef BPAINT_H
#define BPAINT_H

#include "iconbut.h"

/* headerbuttons: 500-549 */

#define B_BP_SELHOME	501
#define B_BP_STOWALL   	502

/*  must start higher than 2*windows */
#define	BP_GLB_TRASH	100 
#define	BP_GLB_SHELF	102 
#define	BP_INI_SHELF	103 

#define	BP_SEL_ZOOM		120 
#define BP_SEL_HAND		122
#define	BP_SEL_BOX		124
#define BP_SEL_MAGIC	126
#define	BP_INI_MAGIC	127
#define	BP_SEL_LASSO	128 
#define	BP_SEL_CPICK	130

#define	BP_DRWPENSEL	150
#define BP_INIPENSEL	151
#define	BP_DRW_BRUSH	152
#define	BP_INI_BRUSH	153
#define	BP_DRW_ABRSH	154
#define	BP_INI_ABRSH	155
#define	BP_DRW_FILL		156
#define	BP_INI_FILL		157
#define	BP_DRW_STAMP	158
#define	BP_INI_STAMP	159
#define	BP_DRW_TEXT		160
#define	BP_INI_TEXT		161
#define	BP_DRW_LINE		162
#define	BP_DRW_RECT		164
#define	BP_DRW_CIRC		166
#define	BP_DRWSPREAD	168
#define	BP_INISPREAD	169
#define	BP_DRW_FRECT	170
#define	BP_DRW_FCIRC	172
#define	BP_DRW_BLUR		174
#define	BP_DRW_BURN		176
#define	BP_DRW_STICK	178
#define	BP_DRW_SHARP	180
#define	BP_DRW_DRAG		182
#define	BP_DRW_SPONS	184

#define	BP_BRS_CLEAR	200
#define	BP_BRS_LOAD		201
#define	BP_BRS_SAVE		202
#define	BP_BRS_BRUSH	204
#define	BP_BRS_SQUARE	206
#define	BP_BRS_DOT		208
#define	BP_BRS_CRAYON	210

#define	BP_IMA_LOAD		250
#define	BP_IMA_SAVE		251
#define	BP_IMA_SCROLL	252
#define	BP_IMA_START	253


#define	BP_ICNINFO		 0
#define	BP_GLOBALS		 1
#define	BP_SELECTORS	 2
#define	BP_DRAWERS		 3
#define	BP_BRUSHES		 4
#define	BP_IMAGES		 5

typedef struct SpacePaint {
	struct SpacePaint *next, *prev;
	int spacetype;
	unsigned int stowing;

	IconButtonManager *ibm;
	
	unsigned int globals;
	unsigned int drawers;
	unsigned int brushes;
	unsigned int layers;
	
	short mousex, mousey;
	short in_icon, in_window;
	
	unsigned int image, workimage;
	Image *work_image;
	float zoom;
	short lrect_sx, lrect_sy;
	short stowall, dummy;
	int pad;
		
} SpacePaint;

	/* bpaint.c */
extern void move_image(SpacePaint *sp);
extern IconLink *link_icon(IconButtonManager **bm, short window_id,short button_id ,int pos_x, int pos_y, short pos_type);
extern void check_image_count(SpacePaint *sp);
extern void init_paintspace(ScrArea *sa);
extern void check_paint_copy(SpacePaint *sp);
extern void init_button_space(SpacePaint *sp);

	/* editbpaint.c */
extern void fetch_image(SpacePaint *sp);
extern void do_paint_buttons(ushort event);
extern void winqreadpaintspace(ushort event, short val);


#endif	/* BPAINT_H */


