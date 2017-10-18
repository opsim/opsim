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

/* drawtext.c  april 99		GRAPHICS

 * 
 * 
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "render.h"
#include "file.h"
#include "text.h"

static void *curfont= NULL;
static int curfont_width= 0;

void set_txt_font (int id) 
{
	if (id==0) curfont= GLUT_BITMAP_SCREEN_12;
	else if (id==1) curfont= GLUT_BITMAP_SCREEN_15;
	else curfont= GLUT_BITMAP_SCREEN_12;
	
	fmsetfont(curfont);

	curfont_width= glutBitmapWidth(curfont, ' ');
}

/* ZR FIX THIS */

#define RENDER_STRING_SIZE 512
static char temp_char_buf[RENDER_STRING_SIZE];
static char temp_char_accum[RENDER_STRING_SIZE];

static int render_string (char *str) {
	char *in, *out, *accum;
	int r= 0, w= 0, ltab= 0, i;
	
	in= str;
	out= temp_char_buf;
	accum= temp_char_accum;
	
	while(*in && r<RENDER_STRING_SIZE) {
		if (*in=='\t') {
			if (ltab && w%(TXT_TABSIZE+1)) i= TXT_TABSIZE;
			else i= TXT_TABSIZE-(w%(TXT_TABSIZE+1));

			while(i--) {
				*out++= ' ';
				*accum++= r;
				w++;
			}
			ltab= 1;
		} else {
			*out++= *in; *accum++= r;
			w++;
			ltab= 0;
		}
		r++;
		in++;
	}
	*out= 0;
	
	return w;
}

static int text_draw(char *str, int cshift, int maxwidth, int draw, int x, int y) {
	int i, r=0, w= 0;
	int extra, shift;
	char *in, *out, *acc;

	w= render_string(str);
	if(w<cshift ) return 0; /* String is shorter than shift */
	
	in= temp_char_buf+cshift;
	acc= temp_char_accum+cshift;
	w= w-cshift;
	
	if (draw) {
		glRasterPos2i(x, y);
		fmprstr(in);
	} else {
		while (w-- && *acc++ < maxwidth) {
			r+= curfont_width;
		}
	}

	if (cshift && r==0) return 0;
	else return r+TXT_OFFSET;
}

static void set_cursor_to_pos (SpaceText *st, int x, int y, int sel) 
{
	Text *text;
	TextLine **linep;
	int *charp;
	int w;
	
	set_txt_font(st->font_id);

	text= st->text;

	if(sel) { linep= &text->sell; charp= &text->selc; } 
	else { linep= &text->curl; charp= &text->curc; }
	
	y= (curarea->winy - y)/st->lheight;
	
	y-= txt_get_span(text->lines.first, *linep) - st->top;
	
	if (y>0) {
		while (y-- != 0) if((*linep)->next) *linep= (*linep)->next;
	} else if (y<0) {
		while (y++ != 0) if((*linep)->prev) *linep= (*linep)->prev;
	}

	x-= TXT_OFFSET;
	
	x = (x/curfont_width) + st->left;
	
	w= render_string((*linep)->line);
	if(x<w) *charp= temp_char_accum[x];
	else *charp= (*linep)->len;
	
	if(!sel) txt_pop_sel(text);
}

static void draw_cursor (SpaceText *st) {
	int h, x, i;
	Text *text= st->text;
	TextLine *linef, *linel;
	int charf, charl;
	
	set_txt_font(st->font_id);

	if (text->curl==text->sell && text->curc==text->selc) {
		x= text_draw(st->text->curl->line, st->left, st->text->curc, 0, 0, 0);

		if (x) {
			h= txt_get_span(st->text->lines.first, st->text->curl) - st->top;

			glColor3f(1.0, 0.0, 0.0);
			
			glRecti(x-1, curarea->winy-st->lheight*(h)-2, x+1, curarea->winy-st->lheight*(h+1)-2);
		}
	} else {	
		if (txt_get_span(text->curl, text->sell)<0) {
			linef= text->sell;
			charf= text->selc;
			
			linel= text->curl;
			charl= text->curc;
		} else {
			linef= text->curl;
			charf= text->curc;
	
			linel= text->sell;		
			charl= text->selc;
		}
	
			/* Walk to the beginning of visible text */
		h= txt_get_span(st->text->lines.first, linef) - st->top;
		while (h++<-1 && linef!=linel) linef= linef->next;
	
		x= text_draw(linef->line, st->left, charf, 0, 0, 0);

		glColor3f(0.75, 0.44, 0.44);

		while (linef && linef != linel) {
			h= txt_get_span(st->text->lines.first, linef) - st->top;
			if (h>st->viewlines) break;
			
			glRecti(x, curarea->winy-st->lheight*(h)-2, curarea->winx, curarea->winy-st->lheight*(h+1)-2);
			glRecti(TXT_OFFSET-10, curarea->winy-st->lheight*(h+1)-2, TXT_OFFSET, curarea->winy-st->lheight*(h+2)-2);
			x= TXT_OFFSET;
			
			linef= linef->next;
		}
		
		h= txt_get_span(st->text->lines.first, linef) - st->top;

		i= text_draw(linel->line, st->left, charl, 0, 0, 0);
		if(i) glRecti(x, curarea->winy-st->lheight*(h)-2, i, curarea->winy-st->lheight*(h+1)-2);
	}

	glColor3f(0.0, 0.0, 0.0);
}

static void calc_text_rcts(SpaceText *st)
{
	short barheight, barstart;
	int lbarstart, lbarh, ltexth;

	if(st->text==0) return;

	lbarstart= st->top;
	lbarh= 	st->viewlines;
	ltexth= txt_get_span(st->text->lines.first, st->text->lines.last)+1;

	barheight= (lbarh*(curarea->winy-4))/ltexth;
	if (barheight<20) barheight=20;
	
	barstart= (lbarstart*(curarea->winy-4))/ltexth + 2;

	st->txtbar.xmin= 5;
	st->txtbar.xmax= 17;
	st->txtbar.ymin= (curarea->winy-barstart) - barheight;
	st->txtbar.ymax= (curarea->winy) - barstart;

	CLAMP(st->txtbar.ymin, 2, curarea->winy-2);
	CLAMP(st->txtbar.ymax, 2, curarea->winy-2);

	st->pix_per_line= (float) ltexth/curarea->winy;
	if (st->pix_per_line<.1) st->pix_per_line=.1;

	lbarstart= MIN2(txt_get_span(st->text->lines.first, st->text->curl), 
				txt_get_span(st->text->lines.first, st->text->sell));
	lbarh= abs(txt_get_span(st->text->lines.first, st->text->curl)-txt_get_span(st->text->lines.first, st->text->sell));
	
	barheight= (lbarh*(curarea->winy-4))/ltexth;
	if (barheight<2) barheight=2; 
	
	barstart= (lbarstart*(curarea->winy-4))/ltexth + 2;
	
	st->txtscroll.xmin= 5;
	st->txtscroll.xmax= 17;
	st->txtscroll.ymin= (curarea->winy-barstart) - barheight;
	st->txtscroll.ymax= curarea->winy-barstart;
}

static void draw_textscroll(SpaceText *st)
{
	calc_text_rcts(st);

	cpack(0x707070);
	glRecti(2, 2, 20, curarea->winy-2);
	EmbossBox2(0, 0, 22, curarea->winy, 1, 0x404040, 0xA0A0A0);

	cpack(0x909090);
	glRecti(st->txtbar.xmin, st->txtbar.ymin, st->txtbar.xmax, st->txtbar.ymax);

	cpack(0x7777c6);
	glRecti(st->txtscroll.xmin, st->txtscroll.ymin, st->txtscroll.xmax, st->txtscroll.ymax);

	EmbossBox2(st->txtbar.xmin-2, st->txtbar.ymin, st->txtbar.xmax+2, st->txtbar.ymax+2, st->flags & ST_SCROLL_SELECT, 0x404040, 0xA0A0A0);
}

static void screen_skip(SpaceText *st, int lines)
{
	int last;
	
	if (!st) return;
	if (st->spacetype != SPACE_TEXT) return;
	if (!st->text) return;

 	st->top += lines;

	last= txt_get_span(st->text->lines.first, st->text->lines.last);
	last= last-st->viewlines+1;
	
	if (st->top>last) st->top= last;
	if (st->top<0) st->top= 0;
}

/* 
 * mode 1 == view scroll
 * mode 2 == scrollbar
 */
static void do_textscroll(SpaceText *st, int mode)
{
	short delta[2]= {0, 0};
	short mval[2], hold[2], old[2];
	
	calc_text_rcts(st);

	st->flags|= ST_SCROLL_SELECT;

	set_txt_font(st->font_id);
	
	glDrawBuffer(GL_FRONT);
	EmbossBox2(st->txtbar.xmin-2, st->txtbar.ymin, st->txtbar.xmax+2, st->txtbar.ymax+2, st->flags & ST_SCROLL_SELECT, 0x404040, 0xA0A0A0);
	glDrawBuffer(GL_BACK);

	getmouseco_areawin(mval);
	old[0]= hold[0]= mval[0];
	old[1]= hold[1]= mval[1];

	while(get_mbut()&(L_MOUSE|M_MOUSE)) {
		getmouseco_areawin(mval);

		if(old[0]!=mval[0] || old[1]!=mval[1]) {
			if (mode==1) {
				delta[0]= (hold[0]-mval[0])/curfont_width;
				delta[1]= (mval[1]-hold[1])/st->lheight;
			}
			else delta[1]= (hold[1]-mval[1])*st->pix_per_line;
			
			if (delta[0] || delta[1]) {
				screen_skip(st, delta[1]);
				st->left+= delta[0];
				if (st->left<0) st->left= 0;
				
				curarea->windraw();
				screen_swapbuffers();
				
				hold[0]=mval[0];
				hold[1]=mval[1];
			}
			old[0]=mval[0];
			old[1]=mval[1];
		} else {
			usleep(2);
		}
	}
	st->flags^= ST_SCROLL_SELECT;

	glDrawBuffer(GL_FRONT);
	EmbossBox2(st->txtbar.xmin-2, st->txtbar.ymin, st->txtbar.xmax+2, st->txtbar.ymax+2, st->flags & ST_SCROLL_SELECT, 0x404040, 0xA0A0A0);
	glDrawBuffer(GL_BACK);
}

static void do_selection(SpaceText *st)
{
	short mval[2], old[2], d;

	if (!st) return;
	if (st->spacetype != SPACE_TEXT) return;
	if (!st->text) return;

	set_txt_font(st->font_id);
	
	getmouseco_areawin(mval);
	old[0]= mval[0];
	old[1]= mval[1];

	while(get_mbut()&L_MOUSE) {
		getmouseco_areawin(mval);

		if (mval[1]<0 || mval[1]>curarea->winy) {
			d= (old[1]-mval[1])*st->pix_per_line;
			if (d) screen_skip(st, d);

			set_cursor_to_pos(st, mval[0], mval[1]<0?0:curarea->winy, 1);

			curarea->windraw();
			screen_swapbuffers();
		} else if(mval[0]<0 || mval[0]>curarea->winx) {
			if (mval[0]>curarea->winx) st->left++;
			else if (mval[0]<0 && st->left>0) st->left--;
			
			set_cursor_to_pos(st, mval[0], mval[1], 1);
			
			curarea->windraw();
			screen_swapbuffers();
			
			usleep(100);
		} else if(old[0]!=mval[0] || old[1]!=mval[1]) {
			set_cursor_to_pos(st, mval[0], mval[1], 1);

			curarea->windraw();
			screen_swapbuffers();

			old[0]= mval[0];
			old[1]= mval[1];
		} else {
			usleep(2);
		}
	}
}

void drawtextspace()
{
	SpaceText *st= curarea->spacedata.first;
	Text *text;
	int i;
	TextLine *tmp;

	glClearColor(0.6, 0.6,  0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	text= st->text;
	if(!text) return;
	
	/* Make sure all the positional pointers exist */
	if (!text->curl || !text->sell || !text->lines.first || !text->lines.last)
		txt_clean_text(text);
	
	if(st->lheight) st->viewlines= (int) curarea->winy/st->lheight;
	else st->viewlines= 0;
	
	glColor3f(0.0, 0.0, 0.0);
	set_txt_font(st->font_id);
	
	draw_cursor(st);

	i=st->top;
	tmp= text->lines.first;
	while (i>0) {
		if (tmp->next) tmp= tmp->next;
		i--;		
	}

	i=0;
	while (i<st->viewlines && tmp) {
		text_draw(tmp->line, st->left, 0, 1, TXT_OFFSET, curarea->winy-st->lheight*(i+1));
		
		i++;
		tmp= tmp->next;
	}

	draw_textscroll(st);

	curarea->win_swap= WIN_BACK_OK;
}

void pop_space_text (SpaceText *st)
{
	int i, x;

	if(!st) return;
	if(!st->text) return;
	if(!st->text->curl) return;
	if(!curfont) return;
		
	i= txt_get_span(st->text->lines.first, st->text->curl);
	if (st->top+st->viewlines <= i || st->top > i) {
		st->top= i - st->viewlines/2;
	}
	
	x= text_draw(st->text->curl->line, st->left, st->text->curc, 0, 0, 0);

	if (x==0 || x>curarea->winx) {
		st->left= st->text->curc-0.5*(curarea->winx)/curfont_width;
	}

	if (st->top < 0) st->top= 0;
	if (st->left <0) st->left= 0;
}

void winqreadtextspace(ushort event, short val)
{
	int do_draw=0, p;
	short mval[2], tmp;
	int curl=0, curc=0;
	static int sell=0, selc=0;
	int linep2, charp2;
	static short selecting;
	SpaceText *st= curarea->spacedata.first;
	ScrArea *area;
	Text *text;

	if(curarea->win==0) return;

	text= st->text;
	
	getmouseco_areawin(mval);

								 /* test for '&' because of irisgl (ton) */
	if (event==KEYBD && (G.qual==0 || (G.qual & LR_SHIFTKEY)) && val != 127 && val != 8) {
		txt_add_char(text, val);
		pop_space_text(st);
		do_draw= 1;
	} else if(val) {
		/* Get rid of spurious KEYBD events */
		/* Iris GL patch, otherwise we get a KEYBD
		 * event after getting alt-q and the like
		 * but at that point it looks like the alt
		 * is no longer pressed, so a 'q' goes in 
		 * the text
		 */
		if(winqtest(curarea)==KEYBD && G.qual & ~LR_SHIFTKEY) winqdelete(curarea);

		switch(event) {
		case MOUSEX:
		case MOUSEY:
			break;			
			
		case LEFTMOUSE:
			if(mval[0]>2 && mval[0]<20 && mval[1]>2 && mval[1]<curarea->winy-2)
				do_textscroll(st, 2);
			else {			
				if (text) {
					curl= txt_get_span(text->lines.first, text->curl);
					curc= text->curc;			
					sell= txt_get_span(text->lines.first, text->sell);
					selc= text->selc;			

					set_cursor_to_pos(st, mval[0], mval[1], G.qual & LR_SHIFTKEY);

					linep2= txt_get_span(text->lines.first, (G.qual&LR_SHIFTKEY)?text->sell:text->curl);
					charp2= text->selc;
				
					if (!(G.qual&LR_SHIFTKEY))
						if (curl!=linep2 || curc!=charp2)
							txt_undo_add_toop(text, UNDO_CTO, curl, curc, linep2, charp2);

					sell= txt_get_span(text->lines.first, text->sell);
					selc= text->selc;
				}

				selecting = 1;
				do_draw= 1;

				do_selection(st);
			}
			break;
			
		case MIDDLEMOUSE:
			do_textscroll(st, 1);
			break;
			
		case FKEY:
			if (G.qual & LR_ALTKEY && G.qual & LR_SHIFTKEY) {
				p= pupmenu("File %t|New %x0|Open... %x1|Save %x2|Save As...%x3|Close%x4");
				switch(p) {
				case 0:
					st->text= add_empty_text();

					st->text->curl= st->text->lines.first;
					st->text->curc= 0;
					st->top= 0;
					
					allqueue(REDRAWTEXT, 0);
					allqueue(REDRAWHEADERS, 0);
					break;

				case 1:
					activate_fileselect(FILE_SPECIAL, "LOAD TEXT FILE", G.sce, add_text_fs);
					break;

				case 3:
					if(text) text->flags |= TXT_ISMEM;
					
				case 2:
					txt_write_file(text);
					do_draw= 1;
					break;

				case 4:
					if(st->text) {
						st->text= NULL;
						st->text->curl= NULL;
						st->text->curc= 0;

						allqueue(REDRAWTEXT, 0);
						allqueue(REDRAWHEADERS, 0);
					}
					break;

				default:
					break;
				}
			}
			break;

		case EKEY:
			if (G.qual & LR_ALTKEY && G.qual & LR_SHIFTKEY) {
				p= pupmenu("Edit %t|"
							"Cut %x0|"
							"Copy %x1|"
							"Paste %x2|"
							"Print Cut Buffer %x3");
				switch(p) {
				case 0:
					txt_cut_sel(text);
					do_draw= 1;
					break;
				case 1:
					txt_copy_sel(text);
					do_draw= 1;
					break;
				case 2:
					txt_paste_buf(text, txt_cut_buffer);
					do_draw= 1;
					break;
				case 3:
					txt_print_cutbuffer();
					break;
				default:
					break;
				}
			}
			break;

		case VKEY:
			if (G.qual & LR_ALTKEY && G.qual & LR_SHIFTKEY) {
				p= pupmenu("View %t|"
							"Top of File %x0|"
							"Bottom of File %x1|"
							"Page Up %x2|"
							"Page Down %x3");
				switch(p) {
				case 0:
					txt_move_bof(text, 0);
					do_draw= 1;
					break;
					
				case 1:
					txt_move_eof(text, 0);
					do_draw= 1;
					break;
					
				case 2:
					screen_skip(st, -st->viewlines);
					do_draw= 1;
					break;
					
				case 3:
					screen_skip(st, st->viewlines);
					do_draw= 1;
					break;
					
				default:
					break;
				}
			}
			break;

		case SKEY:
			if (G.qual & LR_ALTKEY && G.qual & LR_SHIFTKEY) {
				p= pupmenu("Select %t|"
							"Select All %x0|"
							"Select Line %x1|"
							"Jump to Line %x3");
				switch(p) {
				case 0:
					txt_sel_all(text);
					do_draw= 1;
					break;
					
				case 1:
					txt_sel_line(text);
					do_draw= 1;
					break;
										
				case 3:
					tmp= txt_get_span(text->lines.first, text->curl)+1;
					if(!button(&tmp, 1, txt_get_span(text->lines.first, text->lines.last)+1, "Jump to line:")) break;
					txt_move_toline(text, tmp-1, 0);
					do_draw= 1;
					pop_space_text(st);
					break;
					
				default:
					break;
				}
			}
			break;

			default:
				break;
		}

		switch(event) {
		case AKEY:
			if (G.qual & LR_CTRLKEY) {
				txt_move_bol(text, G.qual & LR_SHIFTKEY);
				do_draw= 1;
				pop_space_text(st);
			} else if (G.qual & LR_ALTKEY) {
				txt_sel_all(text);
				do_draw= 1;
			}
			break;

		case CKEY:
			if (G.qual & LR_ALTKEY) {
				txt_copy_sel(text);
				do_draw= 1;	
			}
			break;

		case DKEY:
			if (G.qual & LR_CTRLKEY) {
				txt_delete_char(text);
				do_draw= 1;
				pop_space_text(st);
			}
			break;

		case EKEY:
			if (G.qual & LR_CTRLKEY) {
				txt_move_eol(text, G.qual & LR_SHIFTKEY);
				do_draw= 1;
				pop_space_text(st);
			}
			break;

		case JKEY:
			if (G.qual & LR_ALTKEY) {
				if(!text) break;
				tmp= txt_get_span(text->lines.first, text->curl)+1;
				if(!button(&tmp, 1, txt_get_span(text->lines.first, text->lines.last)+1, "Jump to line:")) break;
				txt_move_toline(text, tmp-1, 0);
				do_draw= 1;
				pop_space_text(st);
			}
			break;

		case OKEY:
			if (G.qual & LR_ALTKEY)
				activate_fileselect(FILE_SPECIAL, "LOAD TEXT FILE", G.sce, add_text_fs);
				
			break;
			
		case PKEY:
			if (G.qual & LR_ALTKEY)
				txt_do_python(text);
			
			break;
			
		case QKEY:
			if (!(G.qual==0 || G.qual & LR_SHIFTKEY)) {
				if(okee("QUIT BLENDER")) exit_usiblender();
			}
			break;
			
		case RKEY:
			if (G.qual & LR_ALTKEY) {
				txt_do_redo(text);
				do_draw= 1;	
			}
			break;
		
		case SKEY:
			if (G.qual & LR_ALTKEY) {
				if (G.qual & LR_SHIFTKEY) 
					if (text) text->flags |= TXT_ISMEM;
					
				txt_write_file(text);
				do_draw= 1;
			}
			break;
			
		case UKEY:
			if (G.qual & LR_ALTKEY) {
				if (G.qual & LR_SHIFTKEY) txt_print_undo(text);
				else {
				txt_do_undo(text);
				do_draw= 1;
				}
			}
			break;

		case VKEY:
			if (G.qual & LR_ALTKEY) {
				txt_paste_buf(text, txt_cut_buffer);
				do_draw= 1;	
				pop_space_text(st);
			}
			break;

		case XKEY:
			if (G.qual & LR_ALTKEY) {
				txt_cut_sel(text);
				do_draw= 1;	
				pop_space_text(st);
			}
			break;
			
		case RETKEY:
			txt_split_curline(text);
			do_draw= 1;
			pop_space_text(st);
			break;

		case BACKSPACEKEY:
			txt_backspace_char(text);
			do_draw= 1;
			pop_space_text(st);
			break;

		case DELKEY:
			txt_delete_char(text);
			do_draw= 1;
			pop_space_text(st);
			break;

		case DOWNARROWKEY:
			txt_move_down(text, G.qual & LR_SHIFTKEY);
			do_draw= 1;
			pop_space_text(st);
			break;

		case LEFTARROWKEY:
			if (G.qual & LR_SHIFTKEY)
				txt_move_left(text, 1);
			else
				txt_move_left(text, 0);
			
			do_draw= 1;
			pop_space_text(st);
			break;

		case RIGHTARROWKEY:
			if (G.qual & LR_SHIFTKEY)
				txt_move_right(text, 1);
			else
				txt_move_right(text, 0);
				
			do_draw= 1;
			pop_space_text(st);
			break;

		case UPARROWKEY:
			txt_move_up(text, G.qual & LR_SHIFTKEY);
			do_draw= 1;
			pop_space_text(st);
			break;

		case PAGEDOWNKEY:
			screen_skip(st, st->viewlines);
			do_draw= 1;
			break;

		case PAGEUPKEY:
			screen_skip(st, -st->viewlines);
			do_draw= 1;
			break;
			
		default:
			break;
		}
	} else if (!val) {
		switch(event){
		case LEFTMOUSE:
			if (selecting) {
				selecting=0;
				if (text) {
					linep2= txt_get_span(text->lines.first, text->sell);
					charp2= text->selc;
				
					if (sell!=linep2 || selc!=charp2)
						txt_undo_add_toop(text, UNDO_STO, sell, selc, linep2, charp2);
				}
			}

			break;
		default:
			break;
		}
	}

	if (do_draw) {
		area= G.curscreen->areabase.first;
		while(area) {
			st= area->spacedata.first;
			while(st) {
				if(st->spacetype==SPACE_TEXT) {
					addqueue(area->win, REDRAW, 1);
					addqueue(area->headwin, REDRAW, 1);
				}
				st= st->next;
			}
			area= area->next;
		}
	}
}

