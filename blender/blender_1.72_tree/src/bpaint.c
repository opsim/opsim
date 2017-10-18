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

/*  bpaint.c      MIXED MODEL

 * 
 *  maart 97
 *  
 *  algemene functies
 *  
 */

#include "blender.h"
#include "graphics.h"
#include "screen.h"
#include "bpaint.h"

void init_button_space(SpacePaint *sp)
{
	IconButtonManager *ibm;
	extern struct ButIcon BGicon[];

	add_icon_manager(&(sp->ibm), 100, 32);
	set_icon_manager(&(sp->ibm), G.font, BGicon[1].rect, BGicon[1].xim,  25,25,  206, 20);
	ibm = sp->ibm;
	
	/*			   leftmouse   , rightmouse  ,scakey,hotkey,x0,y0,x1,y1, variable adress , but type  , help " */
	add_icon(&ibm, BP_GLB_TRASH,0			 ,0       ,	 0 , 3, 0, 4, 0, &(sp->globals), IC_TOG |  1 | IC_NOBOX, "trash");
	add_icon(&ibm, BP_GLB_SHELF,BP_INI_SHELF ,0       ,	 0 , 6, 0, 6, 0, &(sp->globals), IC_TOG |  2, "personal shelf");
	
	add_icon(&ibm, BP_SEL_ZOOM ,0			 ,IC_SHIFT, 'm', 3, 2, 3, 2, &(sp->drawers), IC_ROW | (1<<18), "magnify");
	add_icon(&ibm, BP_SEL_HAND ,0			 ,0		  , ' ',17, 2,17, 2, &(sp->drawers), IC_ROW | (1<<19), "move");
	add_icon(&ibm, BP_SEL_BOX  ,0			 ,0		  , 'b', 8, 2, 8, 2, &(sp->drawers), IC_ROW | (1<<20), "select");
	add_icon(&ibm, BP_SEL_MAGIC,BP_INI_MAGIC ,IC_ALT  , 'm', 7, 2, 7, 2, &(sp->drawers), IC_ROW | (1<<21), "magic");
	add_icon(&ibm, BP_SEL_LASSO,0			 ,0		  , 'l', 6, 2, 6, 2, &(sp->drawers), IC_ROW | (1<<22), "lasso");
	add_icon(&ibm, BP_SEL_CPICK,0			 ,0		  , ',', 0, 2, 0, 2, &(sp->drawers), IC_ROW | (1<<23), "color pick");

	add_icon(&ibm, BP_DRWPENSEL,BP_INIPENSEL ,0		  , '.', 0, 3, 0, 3, &(sp->drawers), IC_ROW | (1<<0), "draw pensil");
	add_icon(&ibm, BP_DRW_BRUSH,BP_INI_BRUSH ,IC_SHIFT, 'b', 1, 3, 1, 3, &(sp->drawers), IC_ROW | (1<<1), "draw brush");
	add_icon(&ibm, BP_DRW_ABRSH,BP_INI_ABRSH ,0		  , 'p', 3, 3, 3, 3, &(sp->drawers), IC_ROW | (1<<2), "airbrush");
	add_icon(&ibm, BP_DRW_FILL ,BP_INI_FILL  ,0		  , 'f', 2, 3, 2, 3, &(sp->drawers), IC_ROW | (1<<3), "fill");
	add_icon(&ibm, BP_DRW_TEXT ,BP_INI_TEXT  ,0		  , 't', 9, 3, 9, 3, &(sp->drawers), IC_ROW | (1<<5), "text");
	add_icon(&ibm, BP_DRW_LINE ,0			 ,0		  , 'v', 4, 3, 4, 3, &(sp->drawers), IC_ROW | (1<<6), "line");
	add_icon(&ibm, BP_DRW_RECT ,0			 ,0		  , 'r', 5, 3, 5, 3, &(sp->drawers), IC_ROW | (1<<7), "square");
	add_icon(&ibm, BP_DRW_CIRC ,0			 ,0		  , 'c', 6, 3, 6, 3, &(sp->drawers), IC_ROW | (1<<8), "circle");
	add_icon(&ibm, BP_DRWSPREAD ,BP_INISPREAD,IC_SHIFT, 'f',10, 3,10, 3, &(sp->drawers), IC_ROW | (1<<9), "spread");
	add_icon(&ibm, BP_DRW_FRECT,0			 ,IC_SHIFT, 'r', 7, 3, 7, 3, &(sp->drawers), IC_ROW | (1<<10), "filled square");
	add_icon(&ibm, BP_DRW_FCIRC,0			 ,IC_SHIFT, 'c', 8, 3, 8, 3, &(sp->drawers), IC_ROW | (1<<11), "filled circle");
	add_icon(&ibm, BP_DRW_BLUR ,0			 ,0		  , 0  ,12, 3,12, 3, &(sp->drawers), IC_ROW | (1<<12), "blur");
	add_icon(&ibm, BP_DRW_BURN ,0			 ,0		  , 0  ,16, 3,16, 3, &(sp->drawers), IC_ROW | (1<<13), "burn");
	add_icon(&ibm, BP_DRW_STICK,0			 ,0		  , 0  ,17, 3,17, 3, &(sp->drawers), IC_ROW | (1<<14), "sticky");
	add_icon(&ibm, BP_DRW_DRAG ,0			 ,0       , 0  ,13, 3,13, 3, &(sp->drawers), IC_ROW | (1<<15), "drag");
	add_icon(&ibm, BP_DRW_SPONS,0			 ,0       , 0  ,14, 3,14, 3, &(sp->drawers), IC_ROW | (1<<16), "sponsh");
	add_icon(&ibm, BP_DRW_STAMP,BP_INI_STAMP ,0		  , 0  ,15, 3,15, 3, &(sp->drawers), IC_ROW | (1<<17), "clone");

	add_icon(&ibm, BP_BRS_CLEAR,0			 ,0       ,	0 , 3, 0, 3, 0,  &(sp->brushes), IC_BUT  , "clear brushes");
	add_icon(&ibm, BP_BRS_LOAD ,0			 ,0       ,	0 , 9, 0, 9, 0,  &(sp->brushes), IC_BUT  , "load brushes");
	add_icon(&ibm, BP_BRS_SAVE ,0			 ,0       ,	0 ,10, 0,10, 0,  &(sp->brushes), IC_BUT  , "save brushes");
	add_icon(&ibm, BP_BRS_BRUSH,0			 ,0       ,	0 , 5, 6, 5, 6,  &(sp->brushes), IC_ROW | (1<<0), "round brush");
	add_icon(&ibm, BP_BRS_SQUARE,0			 ,0       ,	0 , 6, 6, 6, 6,  &(sp->brushes), IC_ROW | (1<<1), "square brush");
	add_icon(&ibm, BP_BRS_DOT  ,0			 ,0       ,	0 , 7, 6, 7, 6,  &(sp->brushes), IC_ROW | (1<<2), "dot brush");
	add_icon(&ibm, BP_BRS_CRAYON,0			 ,0       ,	0 , 8, 6, 8, 6,  &(sp->brushes), IC_ROW | (1<<3), "crayon brush");
	
	
	add_icon(&ibm, BP_IMA_START,  0, 0,0 ,12, 4,13, 4, &(sp->image), IC_PUT , "image");
	add_icon(&ibm, BP_IMA_LOAD,   0, 0,0 , 9, 0, 9, 0,  0, IC_BUT, "load image");
	add_icon(&ibm, BP_IMA_SAVE,   0, 0,0 ,10, 0,10, 0,  0, IC_BUT, "save image");
	
	sp->stowall   = 0;
	sp->globals   = 0;
	sp->drawers   = 1;
	sp->brushes   = 1;
	sp->image = 0;
}

void init_window_space(SpacePaint *sp)
{
	int i;
	IconButtonManager *ibm;
	ibm = sp->ibm;

	/*				 name      , id			 ,ot, icon, wtype	 ,rows, xtra	*/
	add_window(&ibm,"Info",		  BP_ICNINFO,  0,  8, 0, IC_INFO,     0, 0);
	add_window(&ibm,"Globals",    BP_GLOBALS,  1,  0, 1, IC_BUTROW,   1, 0);
	add_window(&ibm,"Selectors",  BP_SELECTORS,2,  1, 1, IC_BUTROW,   1, 0);
	add_window(&ibm,"Drawers",    BP_DRAWERS,  3,  2, 1, IC_BUTROW,   2, 0);
	add_window(&ibm,"Brushes",    BP_BRUSHES,  4,  3, 1, IC_LNKROW,   1, 3);
	add_window(&ibm,"Images",     BP_IMAGES,   5,  7, 1, IC_LNKROW,   1, 2);
	
	link_icon(&ibm, BP_GLOBALS,	  BP_GLB_TRASH,  0, 0, IC_POS_GRID);
	link_icon(&ibm, BP_GLOBALS,	  BP_GLB_SHELF,  7, 0, IC_POS_GRID);

	link_icon(&ibm, BP_SELECTORS, BP_SEL_ZOOM ,  7, 0, IC_POS_GRID);
	link_icon(&ibm, BP_SELECTORS, BP_SEL_HAND ,  6, 0, IC_POS_GRID);
	link_icon(&ibm, BP_SELECTORS, BP_SEL_BOX  ,  5, 0, IC_POS_GRID);
	link_icon(&ibm, BP_SELECTORS, BP_SEL_MAGIC,  4, 0, IC_POS_GRID);
	link_icon(&ibm, BP_SELECTORS, BP_SEL_LASSO,  3, 0, IC_POS_GRID);
	link_icon(&ibm, BP_SELECTORS, BP_SEL_CPICK,  2, 0, IC_POS_GRID);
	
	link_icon(&ibm, BP_DRAWERS,	  BP_DRWPENSEL,  7, 0, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_BRUSH,  6, 0, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_ABRSH,  5, 0, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_FILL ,  4, 0, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_TEXT,   3, 0, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_SPONS,  2, 0, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_BLUR,   1, 0, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_STICK,  0, 0, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_LINE ,  7, 1, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_RECT ,  6, 1, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_CIRC ,  5, 1, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRWSPREAD,  4, 1, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_FRECT,  3, 1, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_FCIRC,  2, 1, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_DRAG ,  1, 1, IC_POS_GRID);
	link_icon(&ibm, BP_DRAWERS,	  BP_DRW_STAMP,  0, 1, IC_POS_GRID);
	
	link_icon(&ibm, BP_BRUSHES,   BP_BRS_BRUSH,  0, 0, IC_POS_GRID);
	link_icon(&ibm, BP_BRUSHES,   BP_BRS_SQUARE, 1, 0, IC_POS_GRID);
	link_icon(&ibm, BP_BRUSHES,   BP_BRS_DOT,    2, 0, IC_POS_GRID);
	link_icon(&ibm, BP_BRUSHES,   BP_BRS_CRAYON, 3, 0, IC_POS_GRID);
	
	link_icon(&ibm, BP_BRUSHES,   BP_BRS_CLEAR,  5, 0, IC_POS_GRID);
	link_icon(&ibm, BP_BRUSHES,   BP_BRS_LOAD,   6, 0, IC_POS_GRID);
	link_icon(&ibm, BP_BRUSHES,   BP_BRS_SAVE,   7, 0, IC_POS_GRID);
	
	link_icon(&ibm, BP_IMAGES,    BP_IMA_LOAD,   6, 0, IC_POS_GRID);
	link_icon(&ibm, BP_IMAGES,    BP_IMA_SAVE,   7, 0, IC_POS_GRID);
}

void check_image_count(SpacePaint *sp)
{
	short	i; 
	short	link_button = 0;
	IconButtonManager *ibm;
	Image			  *ima;
	IconLink		  *il;
	
	ibm = sp->ibm;
	i = 0;
	link_icon(&ibm, BP_IMAGES, BP_IMA_START, 0,0, IC_PUT_RESET);
	ima = G.main->image.first;
	while (ima){
		/*															use link adress   */
		il = (IconLink*)link_icon(&ibm, BP_IMAGES, BP_IMA_START, i, (uint)&(sp->image), IC_PUT_GRID);
		set_link_name(&il, ima->id.name);
		if (ima->id.flag & LIB_FAKEUSER) set_link_icon(&ibm, &il, 13, 4, 13, 4);
		i++;
		ima = ima->id.next;
	}	
}

void init_paintspace(ScrArea *sa)
{
	SpacePaint *sp;
	
	sp= callocN(sizeof(SpacePaint), "initpaintspace");
	addhead(&sa->spacedata, sp);

	set_func_space(sa);
	
	sp->spacetype= SPACE_PAINT;
	sp->stowing = 0xFFFFFF - 33;	/* info & Images not stowed */
	
	init_button_space(sp);
	init_window_space(sp);
	set_stowing(&(sp->ibm), sp->stowing);
	
	sp->zoom = 1.0;
	sp->lrect_sx = 0;
	sp->lrect_sy = 0;
}

void check_paint_copy(SpacePaint *sp)
{
	sp->ibm = 0;
	init_button_space(sp);
	init_window_space(sp);
	set_stowing(&(sp->ibm), sp->stowing);
	sp->work_image = 0;
}

void free_paint(SpacePaint *sp)		/* niet sp zelf vrijgeven */
{
	free_icon_manager(&(sp->ibm));
}

