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

/*  iconbut.c      MIXED MODEL

 * 
 *  mei 97
 *  
 *  icon button graphics
 *  
 */
#include "blender.h"
#include "graphics.h"
#include "screen.h"
#include "iconbut.h"

void draw_info_text(IconButtonManager *ibm);
void win_resize(IconButtonManager **bm,IconWindow *riw, int size, int resize);

/* ***************************************** FREE MANAGER********************************************* */

void def_icon_cursor(short nr, short ox, short oy, uint *rect, short rectx, short cx, short cy)
{
	int i, j;
	int data, adrs, red;
	unsigned short *curs;

	curs = malloc(32*32*sizeof(int));
	 
	for(i=0; i<16; i++){
		curs[i*2] = 0; curs[i*2+1] = 0;
		for(j=0; j<16; j++){  
			data = rect[ ((15-j)+cx) + (cy+i)*rectx ];
			red = (data & 0xFF00) >> 8;
			
			if (red < 128){
				curs[i] += 1<<j;
			}
		}
	}
	/* OGLXXX
	 * curstype not supported
	 * curstype (C16X1)
	 */
	/*DELETED*/;	/* yo 16x16 x 1color */
	/* OGLXXX
	 * defcursor not supported, see Window Manager
	 * defcursor(nr, curs)
	 */
	/*DELETED*/;
	/* OGLXXX
	 * curorigin not supported
	 * curorigin(nr, ox, oy)
	 */
	/*DELETED*/;
	
	free(curs);
}

void set_icon_cursor(IconButtonManager *ibm, short gx, short gy)
{
	extern struct ButIcon BGicon[];

	def_icon_cursor(15, 8, 8, BGicon[1].rect, BGicon[1].xim, gx*ibm->gridx+5, gy*ibm->gridy+5);
	/* OGLXXX
	 * setcursor not supported -- see Window Manager
	 * setcursor(15, 0, 0)
	 */
	/*DELETED*/;

}


void icon_frontbuffer(int val)
{
	/* OGLXXX frontbuffer: other possibilities include GL_FRONT_AND_BACK */
	glDrawBuffer((val) ? GL_FRONT : GL_BACK);
}

void embox(short sx, short sy, short ex, short ey, int hi, int mid, int low)
{
	short sw;
	
	if (sx > ex) {sw = sx; sx = ex; ex = sw; }
	if (sy > ey) {sw = sy; sy = ey; ey = sw; }
	
	cpack(mid); 
	/* OGLXXX sboxfs not screen-aligned -- See glRect */
	glRects(sx,  sy,  ex,  ey);

	cpack(low); 
	/* OGLXXX
	 * sboxs: remove extra PolygonMode changes.
	 * not screen-aligned -- See glRect
	 */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(sx,  sy,  ex,  ey); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/* OGLXXX
	 * sboxs: remove extra PolygonMode changes.
	 * not screen-aligned -- See glRect
	 */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(sx+1,  sy+1,  ex-1,  ey-1); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	cpack(hi); 
	/* OGLXXX
	 * sboxs: remove extra PolygonMode changes.
	 * not screen-aligned -- See glRect
	 */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(sx,  sy+1,  sx+1,  ey); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/* OGLXXX
	 * sboxs: remove extra PolygonMode changes.
	 * not screen-aligned -- See glRect
	 */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(sx,  ey-1,  ex-1,  ey); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/* OGLXXX
	 * sboxs: remove extra PolygonMode changes.
	 * not screen-aligned -- See glRect
	 */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(sx,  sy,  sx,  sy); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void wirebox(short sx, short sy, short ex, short ey, int hi, int mid, int low)
{
	short sw;
	
	if (sx > ex) {sw = sx; sx = ex; ex = sw; }
	if (sy > ey) {sw = sy; sy = ey; ey = sw; }
	
	cpack(mid); 
	/* OGLXXX sboxfs not screen-aligned -- See glRect */
	glRects(sx,  sy,  ex,  ey);

	cpack(low); 
	/* OGLXXX
	 * sboxs: remove extra PolygonMode changes.
	 * not screen-aligned -- See glRect
	 */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(sx,  sy,  ex,  ey); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/* OGLXXX
	 * sboxs: remove extra PolygonMode changes.
	 * not screen-aligned -- See glRect
	 */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(sx+2,  sy+2,  ex-2,  ey-2); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	cpack(hi); 
	/* OGLXXX
	 * sboxs: remove extra PolygonMode changes.
	 * not screen-aligned -- See glRect
	 */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(sx,  sy,  sx,  ey); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/* OGLXXX
	 * sboxs: remove extra PolygonMode changes.
	 * not screen-aligned -- See glRect
	 */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(sx,  ey,  ex,  ey); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	/* OGLXXX
	 * sboxs: remove extra PolygonMode changes.
	 * not screen-aligned -- See glRect
	 */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(sx+2,  sy+2,  ex-2,  sy+2); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/* OGLXXX
	 * sboxs: remove extra PolygonMode changes.
	 * not screen-aligned -- See glRect
	 */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(ex-2,  sy+2,  ex-2,  ey-2); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void fmprt(short x, short y, char *text, int col)
{
	if (strlen(text) == 0) return;
	cpack (col);
	glRasterPos2s(x,  y);
	fmprstr(text);
}
	
	

void free_icon_manager(IconButtonManager **bm)
{
	IconButtonManager	*ibm;
	IconButton			*ib, *ibn, *ibs, *ibsn;
	IconWindow			*iw, *iwn;
	IconLink			*il, *iln;
	ibm = *bm;
	
	if (!ibm) return;
	
	while(ibm->_firstbutton){
		ib = ibm->_firstbutton;
		ibn = ib->next;
		freeN(ib);
		ibm->_firstbutton = ibn;
	}
	while(ibm->_firstwindow){
		iw = ibm->_firstwindow;
		iwn = iw->next;
		
		while (iw->_firstlink){
			il = iw->_firstlink;
			iln = il->next;
			freeN(il);
			iw->_firstlink = iln;
		}
		
		if (iw->ibuf) freeImBuf(iw->ibuf);
		
		freeN(iw);
		ibm->_firstwindow = iwn;
	}
	
	freeN(ibm);
	ibm = 0;
}
/* ***************************************** FIND ********************************************* */

IconWindow *find_window(IconButtonManager **bm, short window_id)
{
	IconButtonManager	*ibm;
	IconWindow			*iw;
	ibm = *bm;
	iw = ibm->_firstwindow;
	while(iw){
		if (iw->id == window_id) return iw;
		iw = iw->next;
	}
	return 0;
}

IconButton *find_button(IconButtonManager **bm, int leftmouse)
{
	IconButtonManager	*ibm;
	IconButton			*ib;
	ibm = *bm;
	ib = ibm->_firstbutton;
	while(ib){
		if (ib->leftmouse == leftmouse) return ib;
		ib = ib->next;
	}
	return 0;
}


void name_icon(IconButtonManager **bm, int id,char *text)
{
	IconButtonManager	*ibm;
	IconButton			*ib;
	ibm = *bm;
	
	ib = find_button(&ibm,id);
	if (ib){
		strncpy(ib->text,text,24);
		ib->text[23] = 0;
	}
}

void set_link_name(IconLink **ilp, char *text)
{
	IconLink			*il;
	il = *ilp;
	
	if (!il) return;
	strncpy(il->text,text,24);
	il->text[23] = 0;
}
void set_link_icon(IconButtonManager **bm, IconLink **ilp, short gx1, short gy1, short gx2, short gy2) 
{
	IconButtonManager   *ibm;
	IconLink			*il;
	ibm= *bm;
	il = *ilp;
	if (!il) return;
	il->ipx[0] = gx1 * ibm->gridx + 3;
	il->ipx[1] = gx2 * ibm->gridx + 3;
	il->ipy[0] = gy1 * ibm->gridy + 3;
	il->ipy[1] = gy2 * ibm->gridy + 3;
}

void set_icon_val(IconButtonManager **bm, int id,short min, short max, short set)
{
	IconButtonManager	*ibm;
	IconButton			*ib;
	ibm = *bm;
	
	ib = find_button(&ibm,id);
	if (ib){
		ib->min = min;
		ib->max = max;
		ib->set = set;
	}
}

/* ***************************************** ADD ICONS ********************************************* */

IconButton* add_icon(IconButtonManager **bm,short leftmouse,short rightmouse,
				int scakey, int hotkey, 
				short gx1, short gy1, short gx2, short gy2, 
				uint *adress, uint icontype, char *text )
{
	int cb, i;
	IconButtonManager	*ibm;
	IconButton			*ib;
	ibm = *bm;
	
	cb = ibm->currentbutton++;
	if (cb == ibm->maxbuttons){
		printf("Can't add button nr %i, %i defined in addManager;\n",  cb, ibm->maxbuttons);
		return 0;
	}
	
	ib =  mallocN(sizeof(IconButton), "IconButton");
	bzero(ib , sizeof(IconButton));
	
	ib->next = 0;
	if (ibm->prevbutton) ibm->prevbutton->next = ib; 
	else ibm->_firstbutton = ib;
	
	if (text) strncpy(ib->text, text, 24);
	ib->leftmouse	= leftmouse;
	ib->rightmouse	= rightmouse;
	ib->hotkey		= scakey + hotkey;
	ib->adress      = adress;
	ib->icontype    = icontype;
	
	ib->ipx[0] = gx1 * ibm->gridx + 3;
	ib->ipx[1] = gx2 * ibm->gridx + 3;
	ib->ipy[0] = gy1 * ibm->gridy + 3;
	ib->ipy[1] = gy2 * ibm->gridy + 3;
	
	ib->ixs[0] = ibm->gridx - 6;
	ib->ixs[1] = ibm->gridx - 6;
	ib->iys[0] = ibm->gridy - 6;
	ib->iys[1] = ibm->gridy - 6;
	
	if (icontype & IC_HEADER) {
		ib->ipx[0] += 2; ib->ipy[0] += 2; ib->ipx[1] += 2; ib->ipy[1] += 2;
		ib->ixs[0] = ib->ixs[1] = ib->iys[0] =ib->iys[1] = ibm->header_height - 5;
	}
		
	ibm->prevbutton = ib;
	return ib;
}

IconLink *last_link(IconWindow *iw)
{
	IconLink *rl, *il;
	
	rl = 0;
	il = iw->_firstlink;
	while (il){
		rl = il;
		il = il->next;
	}
	return rl;
}


void remove_link(IconWindow *iw, IconLink *il)
{
	IconWindow			*curwin;
	IconLink			*fndlnk, *curlnk, *prelnk, *nxtlnk;
	curwin  = iw;
	fndlnk  = il;
	prelnk  = 0;
	
	curlnk = curwin->_firstlink;
	while (curlnk){
		nxtlnk = curlnk->next;
		if (curlnk == fndlnk){
			if (prelnk){
				prelnk->next = nxtlnk;
				freeN(curlnk);
				return;
			}else{
				curwin->_firstlink = nxtlnk;
				freeN(curlnk);
				return;
			}
		}
		prelnk = curlnk;
		curlnk = nxtlnk;
	}
}

IconLink *link_icon(IconButtonManager **bm, short window_id,short button_id ,int pos_x, int pos_y, short pos_type)
{
	static short ipgx=0, ipgy=0, rw = 0;
	IconButtonManager  *ibm;
	IconWindow			*iw;
	IconButton			*ib;
	IconLink			*il, *nxtlnk;
	ibm = *bm;
	
	iw = find_window(&ibm, window_id); if (!iw) { printf("link_icon error,  window (%i) not found \n", window_id); return; }
	ib = find_button(&ibm, button_id); if (!ib) { printf("link_icon error,  icon (%i) not found \n", button_id); return; }
	
	if (pos_type == IC_PUT_RESET) {
		ipgx=0; 
		ipgy=0;
		rw  =0;
		il = iw->_firstlink;
		while(il){
			nxtlnk = il->next;
			if (il->button->icontype & IC_PUT) {
				remove_link(iw, il);
				iw->buttons--;
			}
			il = nxtlnk;
		}
		iw->prevlink = last_link(iw);
		win_resize(&ibm, iw, 1, 0);	/* set size to 1 */
		return 0;
	}
	
	
	il = mallocN(sizeof(IconLink), "IconLink");	
	bzero (il, sizeof(IconLink));
	iw->buttons++;
	
	il->next = 0;
	if (iw->prevlink) iw->prevlink->next = il;
	else iw->_firstlink = il;

	il->button = ib;
	
	il->ipx[0] = ib->ipx[0]; il->ipy[0] = ib->ipy[0];
	il->ipx[1] = ib->ipx[1]; il->ipy[1] = ib->ipy[1];
	il->ixs[0] = ib->ixs[0]; il->iys[0] = ib->iys[0];
	il->ixs[1] = ib->ixs[1]; il->iys[1] = ib->iys[1];
	strcpy(il->text, ib->text);
	
	switch (pos_type){
	case IC_PUT_GRID:
		il->sx = 4 + ipgx * (ibm->gridx);
		il->ex = il->sx + ibm->gridx - 1;
		il->sy = ibm->header_height + 5 + ipgy * ibm->gridy;
		il->ey = il->sy + ibm->gridy - 1;
		il->val= pos_x;
		il->adress = (uint *) (long) pos_y; 
		
		ipgx+=1;
		if ((rw == 0) && (((ipgx+ iw->extra ) * ibm->gridx) > ibm->header_witdh)) {	
			rw = 1;
			win_resize(&ibm, iw, 0, 1);	/* add size with 1 */
		}
		if (((ipgx+1) * ibm->gridx) > ibm->header_witdh) {	
			ipgx =0;	ipgy+=1; rw = 0;
		}
		break;
	case IC_POS_GRID:
		il->sx = 4 + (short)pos_x * (ibm->gridx);
		il->ex = il->sx + ibm->gridx - 1;
		il->sy = ibm->header_height + 5 + (short)pos_y * ibm->gridy;
		il->ey = il->sy + ibm->gridy - 1;
		break;
	case IC_POS_COORD:
		il->sx = (short)pos_x;
		il->ex = (short)pos_x + ibm->gridx;
		il->ey = (short)pos_y;
		il->sy = (short)pos_y + ibm->gridy;
		break;
	case IC_POS_HEADER:
		switch (pos_x){
			case 0: 
				il->sx = 0; 
				il->ex = ibm->header_height; 
				il->sy = 0; 
				il->ey = ibm->header_height; 
				break;
			case 1: 
				il->sx = ibm->header_witdh - ibm->header_height + 1; 
				il->ex = ibm->header_witdh + 1; 
				il->sy = 0; 
				il->ey = ibm->header_height; 
				break;
		}
		break;
	}
	iw->prevlink = il;
	return il;
}


/* ***************************************** ADD WINDOWS ********************************************* */

void add_window(IconButtonManager **bm, char *name, short id, short ot,	short ix, short iy, short win_type, short rows, short xtra)
{
	int					cw;
	IconButtonManager	*ibm;
	IconWindow			*iw;
	ibm = *bm;
	
	cw = ibm->currentwindow++;
	if (cw == ibm->maxwindows){
		printf("Can't add window nr %i, %i defined in addManager;\n",  cw, ibm->maxwindows);
		return;
	}

	iw =  mallocN(sizeof(IconWindow), "IconWindow");
	bzero(iw , sizeof(IconWindow));
	
	iw->next = 0;
	if (ibm->prevwindow) ibm->prevwindow->next = iw; 
	else ibm->_firstwindow = iw;
	ibm->prevwindow = iw;
	
	if (name) strncpy(iw->name, name, 24);
	
	iw->ibuf	   = 0;
	iw->_firstlink = 0;
	iw->buttons    = 0; /* will be 2 after linking icons */

	iw->cloc[0] = 0;
	iw->cloc[1] = 0;
	iw->csiz[0] = ibm->header_witdh;
	iw->csiz[1] = ibm->gridy * rows + ibm->header_height + 7;
	if (win_type & IC_INFO) {
		ibm->info_on = 1;
		ibm->infowindow = iw;
		iw->csiz[1] = ibm->header_height + 22;
	}
	iw->extra	= xtra;
	iw->id		= id;
	iw->ot		= ot;
	iw->type	= win_type;
	iw->header	= 0;
	
	iw->ix		= ix;	iw->iy		= iy;
	
	add_icon(&ibm,id*2  , 0,0,0,  ix,iy,ix,iy,  &(iw->header), IC_HEADER | IC_BUT    | IC_MENU, "menu");
	add_icon(&ibm,id*2+1, 0,0,0,  1, 0,  2, 0,  &(iw->header), IC_HEADER | IC_IMATOG | IC_STOW, "stow");
	
	link_icon(&ibm, id, id*2  ,   0, 0, IC_POS_HEADER);
	link_icon(&ibm, id, id*2+1,   1, 0, IC_POS_HEADER);
}
void win_resize(IconButtonManager **bm,IconWindow *riw, int size, int resize)
{
	IconButtonManager	*ibm;
	IconWindow			*iw;
	IconLink			*il;
	int ot, rs, ns;
	
	ibm = *bm;
	
	rs = ibm->gridy * resize;
	if (size != 0) { 
		ns = ibm->gridy * size + ibm->header_height + 7;
		rs = ns - riw->csiz[1];
		
	}
	il = riw->_firstlink;
	while (il){
		if (!(il->button->icontype & IC_HEADER) && !(il->button->icontype & IC_PUT))
		{ il->sy += rs; il->ey += rs; }
		il = il->next;
	}
	riw->csiz[1] += rs;
	ot = riw->ot;
	
	iw = ibm->_firstwindow;
	while (iw){
		if (iw->ot > ot) iw->cloc[1] += rs;
		iw = iw->next;
	}
}



void set_stowing(IconButtonManager **bm, uint stow)
{
	int	count = 0;
	IconButtonManager	*ibm;
	IconWindow			*iw;
	ibm = *bm;

	ibm->window_y_location = 0;
	
	iw = ibm->_firstwindow;
	while(iw){
		iw->cloc[1] = ibm->window_y_location;
		
		if (stow & (1<<count)){
			iw->status |= IC_STOW;
			iw->header |= IC_STOW;
			ibm->window_y_location += ibm->header_height+2;
		}else{
			iw->status &= ~IC_STOW;
			iw->header &= ~IC_STOW;
			ibm->window_y_location += iw->csiz[1]+2;
		}
		count++;
		iw=iw->next;
	}
}

void get_stowing(IconButtonManager **bm, uint *stowing)
{
	int	count = 0;
	uint stow = 0;
	IconButtonManager	*ibm;
	IconWindow			*iw;
	ibm = *bm;
	iw = ibm->_firstwindow;
	while(iw){
		if(iw->status & IC_STOW) stow += (1<<count);
		count++;
		iw=iw->next;
	}
	*stowing = stow;
}


/* ***************************************** ADD MANAGER ********************************************* */

void add_icon_manager(IconButtonManager **bm, int maxbuttons, int maxwindows)
{
	IconButtonManager *ibm;
	ibm = *bm;
	if (ibm) {
		printf("IconManager already running, resetting manager\n");
		free_icon_manager(&ibm);
	}
	ibm = mallocN(sizeof(IconButtonManager), "IconButtonManager");	
	bzero (ibm, sizeof(IconButtonManager));
	
	ibm->prevwindow     = 0;
	ibm->_firstwindow   = 0;
	ibm->currentwindow	= 0;
	ibm->maxwindows		= maxwindows;

	ibm->prevbutton     = 0;
	ibm->_firstbutton   = 0;
	ibm->currentbutton	= 0;
	ibm->maxbuttons		= maxbuttons;
	
	*bm = ibm;
}

void set_icon_manager(IconButtonManager **bm, int *font,uint *rect, int ima_x_size, short gridx, short gridy, short header_witdh, short header_height)
{
	IconButtonManager *ibm;
	ibm = *bm;

	ibm->icon_image_x_size = ima_x_size;
	ibm->icon_image_rect   = rect;
	
	ibm->window_y_location	= 0;
	ibm->header_witdh	= header_witdh;
	ibm->header_height	= header_height;
	ibm->gridx			= gridx;
	ibm->gridy			= gridy;
	ibm->font			= font;
}

/* ************************************* MOUSE ICON ***************************************** */

short above_icon (IconButtonManager **bm, short mx,  short my)
{
	short				neg, tsy;
	IconButtonManager	*ibm;
	IconWindow			*iw;
	IconLink			*il;
	ibm = *bm;
	
	ibm->inwindow  = 0;
	ibm->inlink    = 0;
	ibm->in_icon   = 0;
	
	neg = ibm->window_y_location;
	my = neg - my;	mx -= 1;
	
	if (mx < 0) return;
	if (my < 0) return;
	
	iw = ibm->_firstwindow;
	while (iw){
		if ((mx > iw->cloc[0]) && (mx < iw->cloc[0]+iw->csiz[0])){
			tsy = iw->csiz[1];
			if (iw->status & IC_STOW) tsy = ibm->header_height;
			if ( (my > iw->cloc[1]) && (my < iw->cloc[1] + tsy) ){
				ibm->inwindow  = iw;
				my = my - iw->cloc[1] - 1;
				il = iw->_firstlink;
				while (il){
					if ((iw->status & IC_STOW) && ((il->button->icontype & IC_HEADER) != IC_HEADER)) return 0;
					if ((mx > il->sx) && (mx < il->ex) && (my > il->sy) && (my < il->ey)){
						ibm->in_icon = il->button->leftmouse;
						ibm->inlink  = il;
						if (ibm->info_on) draw_info_text(ibm);
						return (ibm->in_icon);	
					}
					il = il->next;
				}
				return 0;
			}
		}
		iw = iw->next;
	}
	if (ibm->info_on) draw_info_text(ibm);
	return 0;
}

/* ************************************* DRAW WINDOWS ***************************************** */


void draw_icon_button(IconButtonManager **bm, IconLink **ilp, short wsx, short wsy, int status)
{
	short sx, sy, ex, ey, inv=0, on=0;
	uint varbits, iconbits, icontype, hi, mid, lo, swp;
	uint *rect;
	IconButtonManager	*ibm;
	IconButton			*ib;
	IconLink			*il;
	ibm = *bm;
	il =  *ilp;
	
	ib = il->button;
	icontype = ib->icontype;
	iconbits = icontype & 0xFFFFFF;
	if (ib->adress){
		varbits  = *(ib->adress);	varbits &= 0xFFFFFF; varbits &= iconbits;
	}else{
		varbits = 0;
	}
	mid = 0xaaaaaa;	hi  = 0xdddddd;	lo  = 0x555555;
	if (status & IC_SET) { hi  = 0x555555;	lo  = 0xdddddd; }
	if (status & IC_FRONT) icon_frontbuffer(TRUE);
	sx = wsx+il->sx;sy = wsy - il->ey ;
	ex = wsx+il->ex;ey = wsy - il->sy;
	if (!(icontype & IC_NOBOX)){
		embox(sx,sy,ex-1,ey-1, hi, mid, lo);
	/* OGLXXX
	 * sboxs: remove extra PolygonMode changes.
	 * not screen-aligned -- See glRect
	 */
		cpack(0);glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRects(sx-1, sy-1, ex, ey); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
		
	if (iconbits == varbits){ 
		if (icontype & IC_ROW){ inv = 1; on = 1; }else{
			if (icontype & IC_TOG){ inv = 1; on = 1; }
			if (icontype & IC_IMATOG){ on = 1; }	
		}
	}
	
	if ((inv) && (!(icontype & IC_NOBOX))) embox(sx,sy,ex-1,ey-1, lo, mid, hi);

	/* OGLXXX
	 * pixmode: see glPixelTransfer man page
	 * Translate parameters.
	 * pixmode: see glPixelTransfer man page
	 */
	glPixelStorei(GL_UNPACK_ROW_LENGTH,  ibm->icon_image_x_size);
	sx += ibm->screen_x; sy += ibm->screen_y;
	rect= ibm->icon_image_rect + (il->ipy[on] * ibm->icon_image_x_size) + il->ipx[on];
	lrectwrite(sx+2,sy+2, sx+2+il->ixs[on],sy+2+il->iys[on], rect);
	/* OGLXXX
	 * pixmode: see glPixelTransfer man page
	 * Translate parameters.
	 * pixmode: see glPixelTransfer man page
	 */
	glPixelStorei(GL_UNPACK_ROW_LENGTH,  0);
	if (status & IC_FRONT) icon_frontbuffer(FALSE);
}

int icon_handler(IconButtonManager **bm, int mousebut)
{
	static Device mdev[2]= {MOUSEX, MOUSEY};
	int 				ox, oy; 
	short				mx, my, mval[2];
	short				ns, in, oin, in_icon;
	uint				varbits, iconbits, icon_type;
	IconButtonManager	*ibm;
	IconWindow			*iw, *cw;
	IconLink			*il, *fl;
	IconButton			*ib;
	
	ibm = *bm;
	il = ibm->inlink;
	iw = ibm->inwindow;
	
	if (!ibm->in_icon) return 0;
	if (!il) return 0;
	
	ib = il->button;
	getorigin (&ox, &oy);
	
	in_icon = ibm->in_icon;
	draw_icon_button(&ibm, &il,  iw->cloc[0], ibm->window_y_location - iw->cloc[1], IC_SET | IC_FRONT);

	while(getbutton(mousebut)) {
		getmouseco_sc(mval);
		
		mx = mval[0] - ox + iw->cloc[0];	
		my = ibm->window_y_location - (mval[1] - oy + iw->cloc[1]);
		in = 0; if ((mx > il->sx) && (mx < il->ex) && (my > il->sy) && (my < il->ey)) in = 1;
		
		if (in != oin){
			draw_icon_button(&ibm, &il, iw->cloc[0], ibm->window_y_location - iw->cloc[1], in | IC_FRONT);
			oin = in;
		}
	}
	if (mousebut == RIGHTMOUSE) draw_icon_button(&ibm, &il,  iw->cloc[0], ibm->window_y_location - iw->cloc[1],IC_FRONT);
	if (!in) return 0;
	if (mousebut == RIGHTMOUSE) in_icon = ib->rightmouse;
	icon_type = ib->icontype;
	
	if (icon_type & IC_HEADER) {
		if (icon_type & IC_MENU) {		/* the menu button */
		}
		if (icon_type & IC_STOW) {		/* the stow button */
			if (iw->status & IC_STOW) {	/* window is stowed, so unstow */
				iw->header &= ~IC_STOW;
				iw->status &= ~IC_STOW;
				ns = iw->csiz[1] - ibm->header_height;
			}else{						/* window is not stowed, so stow */
				iw->header |= IC_STOW;
				iw->status |= IC_STOW;
				ns = -(iw->csiz[1] - ibm->header_height);
			}
			cw = ibm->_firstwindow;
			while (cw){
				if (cw->ot > iw->ot) cw->cloc[1] = cw->cloc[1] + ns;
				cw = cw->next;
			}
		} 
	}
	
	if (ib->adress){
		iconbits  = icon_type & 0xFFFFFF;
		varbits   = *(ib->adress); 
		varbits &= iconbits;
	
		if (icon_type & IC_ROW) *(ib->adress) = icon_type;
		if (icon_type & IC_TOG){
			if (varbits)  *(ib->adress) = *(ib->adress) - iconbits; else  *(ib->adress) = *(ib->adress) + iconbits; 
		}
		if (icon_type & IC_PUT) {
			*(il->adress) = il->val;
		}
	}
	return in_icon;
}

void draw_info_text(IconButtonManager *ibm)
{
	static IconLink	*prev_link = 0;
	short sx,ex,sy,ey;
	
	if (!ibm->infowindow) return;
	if (!ibm->info_on)    return;
	if (prev_link == ibm->inlink) return;
	
	sx = ibm->infowindow->cloc[0] + 4;
	ex = sx + ibm->infowindow->csiz[0] - 8;
	
	ey = ibm->window_y_location - ibm->infowindow->cloc[1];
	sy = ey - ibm->infowindow->csiz[1]+3;
	ey = sy + 12;
	
	icon_frontbuffer(TRUE);
	
	cpack(0x999999); glRecti(sx, sy-1, ex, ey+1);
	if(ibm->inlink) fmprt(sx+4,sy+2, ibm->inlink->text, 0);
	
	prev_link = ibm->inlink;
}

void draw_icon_window(IconButtonManager *ibm, IconWindow *iw)
{
	short wsx, wsy, wex, wey, hdy, gdx;
	IconLink *il;
	
	gdx = ibm->gridx;
	hdy = ibm->header_height;
	wsx = iw->cloc[0]; 
	wex = wsx + iw->csiz[0];
	wey = ibm->window_y_location - iw->cloc[1]; 
	wsy = wey - hdy;
	/* Header */
	cpack(0);
	/* OGLXXX sboxfi not screen-aligned -- See glRect */
	glRecti(wsx-1,  wsy-1,  wex+1,  wey+1);
	embox(wsx + hdy + 1, wsy, wex - hdy - 1,  wey-1, 0xdddddd, 0x999999, 0x555555);
	fmprt(wsx + gdx + 8, wsy + 5, iw->name, 0);
	il = iw->_firstlink;
	draw_icon_button(&ibm,&il, wsx, wey, IC_BACK); il = il->next;
	draw_icon_button(&ibm,&il, wsx, wey, IC_BACK); il = il->next;
	
	/* Window */
	if (!(iw->status & IC_STOW)){
		cpack(0);
	/* OGLXXX
	 * sboxi: remove extra PolygonMode changes.
	 * not screen-aligned -- See glRect
	 */
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glRecti(wsx-1,  wey - iw->csiz[1] - 2,  wex+1,  wey - hdy); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
		wirebox(wsx, wey - iw->csiz[1] - 1, wex, wey - hdy - 2, 0xdddddd, 0x999999, 0x555555);
		while (il){
			draw_icon_button(&ibm,&il, wsx, wey, IC_BACK);
			il = il->next;
		}
		if (iw->type & IC_INFO){
			ibm->info_on = 1;
			if(ibm->text){
				cpack(0x999999); 
				fmprt(wsx+4,wsy-hdy+2, ibm->text, 0);
				ibm->text[0] = 0;
			}
		}
	}
}

void draw_icon_windows(IconButtonManager **bm)
{
	short wsx, wsy, wex, wey, hdy, gdx;
	IconButtonManager	*ibm;
	IconWindow			*iw;
	IconLink			*il;
	ibm = *bm;
	
	fmsetfont(ibm->font);
	ibm->info_on = 0;	
 	
	iw = ibm->_firstwindow;
	while (iw){
		draw_icon_window(ibm, iw);
		iw = iw->next;
	}
}

