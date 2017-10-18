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


/* screen.c  dec/jan 93/94		GRAPHICS
 * 
 * 
 * 
 * 
 * 
 */


#include "blender.h"
#include "graphics.h"
#include "render.h"
#ifdef __sgi
#include <sys/syssgi.h>
#endif

/* #ifndef FREE */
/* #include "/usr/people/ton/code.h" */
/* #endif */

/* TIPS:
 * 
 * - LET OP DE EDGES,  VERTICES ERVAN MOETEN IN VOLGORDE
	 (laagste pointer eerst). Anders onvoorspelbare effecten!
 * - probleem: flags zijn nog niet echt netjes. Altijd na gebruik
	 op nul zetten.

/* ************************* */

/* ********* Globals *********** */

extern bWindow *swinarray[];	/* mywindow.c */

ScrArea *curarea= 0;
ScrEdge *curedge= 0;
unsigned short fullscreen=1;
unsigned short borderless=1;
int displaysizex= 0, displaysizey= 0;
int prefsizx= 0, prefsizy= 0, prefstax= 0, prefstay= 0;
short scrmousex, scrmousey;	/* worden gezet door mousecallback */
short mainwin=0, winqueue_break= 0, cursonedge=0, keycode[100];
static short mainqueue[MAXQUEUE];
ScrArea *areawinar[MAXWIN];

/* uint edcol[EDGEWIDTH]= {0x0, 0x303030, 0x606060, 0x808080, 0x909090, 0xF0F0F0, 0x0}; */
uint edcol[EDGEWIDTH]= {0x0, 0x505050, 0x909090, 0xF0F0F0, 0x0};

int autosavetime;

/* ********* Funkties *********** */

void getdisplaysize();
bScreen *default_foursplit(), *default_twosplit();
ScrEdge *findscredge(ScrVert *v1, ScrVert *v2);
void setscreen(bScreen *sc);
void drawscreen();
void initscreen();
void moveareas();
void scrollheader();
void testareas();
void area_fullscreen();
void addqueue(short win, ushort event, short val);
void editsplitpoint();
void splitarea(ScrArea *sa, char dir, float fac);
void joinarea(ScrArea *sa);
void select_connected_scredge(bScreen *sc, ScrEdge *curedge);



static ushort hor_ptr_bits[] = {
	0x0000, 0x0000, 0x0000, 0x0000, 
	0x1008, 0x1818, 0x1c38, 0xffff, 
	0xffff, 0x1c38, 0x1818, 0x1008, 
	0x0000, 0x0000, 0x0000, 0x0000, 

	0x0000, 0x0000, 0x0000, 0x381c, 
	0x3c3c, 0x3e7c, 0xffff, 0xffff, 
	0xffff, 0xffff, 0x3e7c, 0x3c3c, 
	0x381c, 0x0000, 0x0000, 0x0000, 
};

static ushort vert_ptr_bits[] = {
	0x0180, 0x0180, 0x0180, 0x0ff0, 
	0x07e0, 0x03c0, 0x0180, 0x0, 
	0x0, 0x0180, 0x03c0, 0x07e0, 
	0x0ff0, 0x0180, 0x0180, 0x0180, 

	0x03c0, 0x03c0, 0x1ff8, 0x1ff8, 
	0x1ff8, 0x0ff0, 0x07e0, 0x03c0, 
	0x03c0, 0x07e0, 0x0ff0, 0x1ff8, 
	0x1ff8, 0x1ff8, 0x03c0, 0x03c0, 
};

static ushort win_ptr_bits[] = {
	0x0000, 0x0180, 0x0180, 0x0180, 
	0x0000, 0x89b3, 0xfdb3, 0xfdb7, 
	0xb5bf, 0xb5bb, 0x85b3, 0x0000, 
	0x0180, 0x0180, 0x0180, 0x0000, 

	0x03c0, 0x03c0, 0x03c0, 0x03c0, 
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
	0x03c0, 0x03c0, 0x03c0, 0x03c0
};


void init_screen_cursors()
{
}

void waitcursor(int val)
{
	ScrArea *sa;
	int oldwin;
	
	if(G.curscreen==0) return;
	if(G.background) return;
	
	if(val) {
		
		/* only once: for time-cursor */
		if(R.flag & R_RENDERING) return;

		if(R.win && R.win==G.curscreen->winakt) {
			oldwin= G.curscreen->winakt;
			mywinset(G.curscreen->mainwin);
			glutSetCursor(CURSOR_WAIT);
			mywinset(oldwin);
		}
		else glutSetCursor(CURSOR_WAIT);
	}
	else if(G.curscreen->winakt>3) {
	
		if(R.flag & R_RENDERING) return;
		sa= areawinar[G.curscreen->winakt];
		if(sa->cursor && sa->win==G.curscreen->winakt) glutSetCursor(sa->cursor);
		else glutSetCursor(CURSOR_STD);
		
	}
	else glutSetCursor(CURSOR_STD);
}


void tempcursor(int curs)
{
	ScrArea *sa;
	
	if(G.curscreen==0) return;
	
	if(curs!=CURSOR_STD) glutSetCursor(curs);
	else if(G.curscreen->winakt) {
		sa= areawinar[G.curscreen->winakt];
		if(sa->win==G.curscreen->winakt)  glutSetCursor(sa->cursor);
		else  glutSetCursor(CURSOR_STD);
	}
}

void wich_cursor(ScrArea *sa)
{
	/* o.a. als nieuwe space gemaakt */
	
	sa->cursor= CURSOR_STD;
	
	if(sa->spacetype==SPACE_VIEW3D) {
		if(G.obedit) sa->cursor= CURSOR_EDIT;
		else if(G.f & G_VERTEXPAINT) sa->cursor= CURSOR_VPAINT;
		else if(G.f & G_FACESELECT) sa->cursor= CURSOR_FACESEL;
	}
	if(sa->win==G.curscreen->winakt) glutSetCursor(sa->cursor);
}


void setcursor_space(int spacetype, short cur)
{
	bScreen *sc;
	ScrArea *sa;
	int oldwin;

	sc= G.main->screen.first;
	while(sc) {
		sa= sc->areabase.first;
		while(sa) {
			if(sa->spacetype==spacetype) {
				sa->cursor= cur;
				if(cur==0) wich_cursor(sa);	/* extra test, bv nodig bij verlaten editmode in vertexpaint */
				if(sc==G.curscreen && sc->winakt==sa->win) glutSetCursor(sa->cursor);
			}
			sa= sa->next;
		}
		sc= sc->id.next;
	}
}


/* *********** CODETAB ******************* */

void decodekeytab()
{
#ifdef __sgi
	extern unsigned char hash[512];
	char ihash[512];
	int a,b,file;
	char *c, hus1, hus2, sl[MAXSYSIDSIZE], str[30];

	file= open("/.Bcode",O_BINARY|O_RDONLY);
	if(file== -1) exit(0);

	read(file,keycode,200);
	close(file);

	syssgi(SGI_SYSID,sl);

	/* inverse hashtab */
	for(a=0;a<256;a++) {
		for(b=0;b<256;b++) {
			if(a==hash[b]) break;
		}
		ihash[a]= b;
	}
	for(a=0;a<256;a++) ihash[256+a]= ihash[a];

	/* bereken hus1 en hus2 ahv sleutel */
	hus1= hash[ sl[0]+ hash[ sl[2]] ];
	hus2= hash[ sl[1]+ hash[ sl[3]] ];

	c= (char *)keycode;
	for(a=0; a<100; a++) {
		c[0]= ( ihash[ (ihash[ c[0] ] -hus1) & 255 ]  ) & 255 ;
		c[1]= ( ihash[ (ihash[ c[1] ] -hus2) & 255 ]  ) & 255 ;
		c+=2;
	}
#endif /* __sgi */
}



/* *********  IN/OUT  ************* */

void getmouseco_sc(short *mval)		/* screen coordinaten */
{

	if(G.curscreen==0) return;

	getmouse(mval);	/* display coordinaten */

	mval[0]-= G.curscreen->startx;
	mval[1]-= G.curscreen->starty;
}

void getmouseco_areawin(short *mval)		/* interne area coordinaten */
{

	getmouseco_sc(mval);
	if(curarea->win) {
		mval[0]-= curarea->winrct.xmin;
		mval[1]-= curarea->winrct.ymin;
	}
}

void getmouseco_headwin(short *mval)		/* interne area coordinaten */
{

	getmouseco_sc(mval);
	if(curarea->headwin) {
		mval[0]-= curarea->headrct.xmin;
		mval[1]-= curarea->headrct.ymin;
	}
}

/* ********  WINDOW & QUEUE ********* */

static short *mainq= mainqueue;

ushort qread(short *val)
{

	if( (long)mainq > (long)mainqueue) {
		mainq-= 2;
		
		*val= mainq[1];
		return ((ushort *)mainq)[0];
	}

	return 0;	
}

void qenter(ushort event, short val)
{
	int size;
	short *end;
	
	/* avoid non-events: qtest()! */
	if(event) {
	
		end= mainqueue+MAXQUEUE-2;
		if( (long)mainq < (long)end) {
			size= ((long)mainq)-((long)mainqueue);
			
			if(size) memmove( mainqueue+2, mainqueue, size);
			( (ushort *)mainqueue)[0]= event;
			mainqueue[1]= val;
			
			mainq+= 2;
		}
	}
}

ushort myqtest()
{
	if(mainq != mainqueue) return ((ushort *)mainq)[-2];;
	return 0;
}

ushort qtest()
{
	ushort event;
#if !defined __BeOS && !defined WIN32
	extern Display *__glutDisplay;
	
	/* combinatie: deze werkt als oude qtest(). wel daarna screen_qread aanroepen */
	
	if(event=myqtest()) return event;
	return (XPending(__glutDisplay));
#else
	
	if(event=myqtest()) return event;
	return glutqtest();
	
#endif
}



void qreset()
{
	mainq= mainqueue;
}

short isqueued( short dev )
{
	return 0;
}


/* *********** AUTOSAVE ************** */

void reset_autosave()
{
	autosavetime= 0;
}

void start_autosave(int val)
{

	/* opniew aanzetten */
	glutTimerFunc(60000, start_autosave, 0);

	if(U.flag & AUTOSAVE) {
		autosavetime++;
		if(autosavetime >= U.savetime) {
			qenter(TIMER0, 1);
		}
	}
	else autosavetime= 0;

}

void set_cursonedge(short mx, short my)
{
	ScrEdge *se, *se1=0, *se2=0, *se3=0, *se4=0;
	float dist, mindist= 100.0, vec1[2], vec2[2], vec3[2], PdistVL2Dfl();

	vec1[0]= mx;
	vec1[1]= my;
	curedge= 0;
	
	/* als de edge element is van curarea: extra voordeel. Dit voor juiste split en join  */
	if(curarea) {
		se1= findscredge(curarea->v1, curarea->v2);
		se2= findscredge(curarea->v2, curarea->v3);
		se3= findscredge(curarea->v3, curarea->v4);
		se4= findscredge(curarea->v4, curarea->v1);
	}
	
	se= G.curscreen->edgebase.first;
	while(se) {

		vec2[0]= se->v1->vec.x;
		vec2[1]= se->v1->vec.y;
		vec3[0]= se->v2->vec.x;
		vec3[1]= se->v2->vec.y;
		dist= PdistVL2Dfl(vec1, vec2, vec3);
		
		if(se==se1 || se==se2 || se==se3 || se==se4) dist-= 5.0;
		
		if(dist<mindist) {
			mindist= dist;
			curedge= se;
		}

		se= se->next;
	}
	
	if(curedge==0) return;
	cursonedge= 1;
	
	if(curedge->v1->vec.x==curedge->v2->vec.x) glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
	else glutSetCursor(GLUT_CURSOR_UP_DOWN);
}

void areawinset(short win)
{
	ScrArea *sa= 0;
	SpaceSeq *sseq;
	
	if(win>3) {
	
		curarea= areawinar[win];
		if(curarea==0) {
			printf("error in areawinar %d ,areawinset\n", win);
			return;
			
		}
		
		switch(curarea->spacetype) {
		case SPACE_VIEW3D:
			G.vd= curarea->spacedata.first;
			break;
		case SPACE_IPO:
			if(G.sipo != curarea->spacedata.first) allqueue(REDRAWBUTSANIM, 0);
			G.sipo= curarea->spacedata.first;
			G.v2d= &G.sipo->v2d;
			break;
		case SPACE_BUTS:
			G.buts= curarea->spacedata.first;
			G.v2d= &G.buts->v2d;
			break;
		case SPACE_SEQ:
			sseq= curarea->spacedata.first;
			G.v2d= &sseq->v2d;
			
		case SPACE_OOPS:
			G.v2d= &((SpaceOops *)curarea->spacedata.first)->v2d;
			G.soops= curarea->spacedata.first;
			break;
		case SPACE_IMASEL:
		
			break;
		case SPACE_IMAGE:
			G.v2d= &((SpaceImage *)curarea->spacedata.first)->v2d;
			G.sima= curarea->spacedata.first;
			break;
		case SPACE_PAINT:
	
			break;
		case SPACE_FILE:
	
			break;
		case SPACE_TEXT:
			G.stext= curarea->spacedata.first;
			break;
		}
	}
	
	if(win) mywinset(win);
}

void headerbox(int selcol, int width)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
	
	if(selcol) glClearColor(.7, .7, .6, 0.0);
	else glClearColor(.6, .6, .6, 0.0);
	
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3ub(0, 0, 0);
	sdrawbox(0,  0,  width,  HEADERY);

	glColor3ub(220, 220, 220);
	sdrawline(0,  HEADERY-1,  width,  HEADERY-1);
	
	glColor3ub(176, 176, 176);
	sdrawline(0,  HEADERY-2,  width,  HEADERY-2);
	
	glColor3ub(128, 128, 128);
	sdrawline(0,  2,  width,  2);
	
	glColor3ub(64, 64, 64);
	sdrawline(0,  1,  width,  1);
	
	glColor3ub(0, 0, 0);
	sdrawline(0,  0,  width,  0);

}

void defheaddraw()
{
	int selcol;
	
	/* aktieve kleur */
	selcol= 0;

	if(G.curscreen->winakt) {
		if(curarea->headwin == G.curscreen->winakt) selcol= 1;
		else if(curarea->win == G.curscreen->winakt) selcol= 1;
	}

	headerbox(selcol, curarea->winx+100);

	/* buttons, functies in window.c */
	switch(curarea->spacetype) {
	case SPACE_EMPTY:
		break;
	case SPACE_FILE:
		file_buttons();
		break;
	case SPACE_INFO:
		info_buttons();
		break;
	case SPACE_VIEW3D:
		view3d_buttons();
		break;
	case SPACE_IPO:
		ipo_buttons();
		break;
	case SPACE_BUTS:
		buts_buttons();
		break;
	case SPACE_SEQ:
		seq_buttons();
		break;
	case SPACE_IMAGE:
		image_buttons();
		break;
	case SPACE_IMASEL:
		imasel_buttons();
		break;
	case SPACE_OOPS:
		oops_buttons();
		break;
	case SPACE_PAINT:
		break;
	case SPACE_TEXT:
		text_buttons();
		break;
	}
	curarea->head_swap= WIN_BACK_OK;
	
}

void defwindraw()
{
	if(curarea->win && curarea->windraw) {
		curarea->windraw();
	}
	else {
		glClearColor(0.4375, 0.4375, 0.4375, 0.0); 
		glClear(GL_COLOR_BUFFER_BIT);
	}
	curarea->win_swap= WIN_BACK_OK;
}

void defheadchange()
{
	float ofs;
	
	if(curarea->headchange) {
		curarea->headchange();
	}
	else {
		ofs= curarea->headbutofs;

		if(curarea->headertype==HEADERDOWN)
			ortho2(-0.5+ofs, curarea->headrct.xmax-curarea->headrct.xmin-0.5+ofs, +0.6, curarea->headrct.ymax-curarea->headrct.ymin+0.6);
		else
			ortho2(-0.5+ofs, curarea->headrct.xmax-curarea->headrct.xmin-0.5+ofs, -0.5, curarea->headrct.ymax-curarea->headrct.ymin-0.5);
	}
}

void defwinchange()
{

	if(curarea->winchange) {
		curarea->winchange();
	}
	else {
		ortho2(-0.5, curarea->winrct.xmax-curarea->winrct.xmin-0.5, -0.5, curarea->winrct.ymax-curarea->winrct.ymin-0.5);
		glLoadIdentity();
	}
}

void defwinmat()
{

	ortho2(-0.5, curarea->winrct.xmax-curarea->winrct.xmin-0.5, -0.5, curarea->winrct.ymax-curarea->winrct.ymin-0.5);
	glLoadIdentity();
}

void headmenu(ScrArea *sa)
{
	float fac;
	
	if(curarea->full) {
		confirm("", "Full window");
		return;
	}

	if( okee("Switch header")) {
		if(sa->headertype==1) sa->headertype=2;
		else if(sa->headertype==2) sa->headertype=1;
		testareas();
	}
}

void defheadqread(ScrArea *sa)
{
	extern void do_py_head_event();	/* py_draw.c */
	ScrArea *tempsa;
	float fac;
	ushort event;
	short val, do_redraw=0, do_change=0;
	char *str;
	
	areawinset(sa->headwin);
	
	while(sa->hq != sa->headqueue) {
		sa->hq-= 2;
		
		event= (ushort)sa->hq[0];
		val= sa->hq[1];

		if(val) {
			
			if(sa->spacetype==SPACE_TEXT && sa->headqread==do_py_head_event) {
				/* exception for a running python script */
				sa->headqread(event, val);
			}
			else if(event==LEFTMOUSE) {

				FrontbufferButs(TRUE);
				event= DoButtons();
				FrontbufferButs(FALSE);
				
				if(event) {
					if(event<1000) do_headerbuttons(event);
					/* er is waarschijnlijk in frontbuf getekend */
					/* sa->win_swap= WIN_FRONT_OK; */
				}
				else {
					mywinset(G.curscreen->mainwin);

					if(G.qual & LR_CTRLKEY) glutPushWindow();
					else {
						glutPopWindow();
						glutDoWorkList();
						R.winpop= 1;	/* flag: need pop */
					}
					
					areawinset(sa->headwin);
				}
			}
			else if(event==MOUSEY) {
				if(U.flag & TOOLTIPS) {
					str= GetButTip();
					if(str) {
						set_info_text(str);
						allqueue(REDRAWINFO, 1);
					}
				}
			}
			else if(event==MIDDLEMOUSE) {
				scrollheader();
				break;
			}
			else if(event==RIGHTMOUSE) {
				headmenu(sa);
			}
			else if(event==REDRAW) {
				do_redraw= 1;
				
			}
			else if(event==CHANGED) {
				sa->head_swap= 0;
				do_change= 1;
				do_redraw= 1;
			
			}
			else if(sa->headqread) {
				sa->headqread(event, val);
			}
			
			if(winqueue_break) return;
		}
	}

	/* test: bestaat window nog */	
	tempsa= areawinar[sa->headwin];
	if(tempsa==0) return;
	
	/* dit onderscheid loopt niet lekker... */
	if(do_change || do_redraw) {
		areawinset(sa->headwin);
		defheadchange();
		
		if(sa->headdraw) sa->headdraw(); 
		
	}
}

ushort winqtest(ScrArea *sa)
{
	if(sa->wq != sa->winqueue) return (ushort) sa->wq[-2];
	return 0;
}

void winqdelete(ScrArea *sa)
{
	if(sa->wq != sa->winqueue) sa->wq-= 2;
}

void winqclear(ScrArea *sa)
{
	if(sa->wq = sa->winqueue);
}


void defwinqread(ScrArea *sa)
{
	ScrArea *tempsa;
	ushort event;
	short val, do_redraw=0, do_change=0;
	
	if(sa!=curarea || sa->win!=winget()) areawinset(sa->win);

	while(sa->wq != sa->winqueue) {
		sa->wq-= 2;
		
		event= (ushort) sa->wq[0];
		val= sa->wq[1];
		
		if(event==REDRAW) {
			do_redraw= 1;
		}
		else if(event==CHANGED) {
			sa->win_swap= 0;
			do_change= 1;
			do_redraw= 1;
		}
		else {
			if(sa->winqread) sa->winqread(event, val);
		}
		if(winqueue_break) return;
	}

	/* test: bestaat window nog */	
	tempsa= areawinar[sa->win];
	if(tempsa==0) return;

	if(do_change || do_redraw) areawinset(sa->win);
	if(do_change) defwinchange(sa->win);
	if(do_redraw) defwindraw();
	
}

void addqueue(short win, ushort event, short val)
{
	ScrArea *sa;
	long size;
	short *end;
	
	/* nieuwe events worden vooraan in het array gezet */
	sa= areawinar[win];

	if(sa) {
	
		if(win==sa->headwin) {
			end= sa->headqueue + MAXQUEUE-2;
			
			if( (long)sa->hq < (long)end) {
				size= ((long)sa->hq)-((long)sa->headqueue);

				if(size) memmove( sa->headqueue+2, sa->headqueue, size);
				((ushort *)sa->headqueue)[0]= event;
				sa->headqueue[1]= val;
				sa->hq+= 2;
			}
		}
		else if(win==sa->win) {
			end= sa->winqueue + MAXQUEUE-2;
			if( (long)sa->wq < (long)end) {
				size= ((long)sa->wq)-((long)sa->winqueue);

				if(size) memmove( sa->winqueue+2, sa->winqueue, size);
				((ushort *)sa->winqueue)[0]= event;
				sa->winqueue[1]= val;
				sa->wq+= 2;
			}
		}
	}
}

short afterqueue[3*MAXQUEUE], *afterq=afterqueue;

void addafterqueue(short win, ushort event, short val)
{
	long poin;
	
	poin= (long)afterqueue;
	poin+= 6*(MAXQUEUE-1);
	
	if( (long)afterq < poin ) {
		afterq[0]= win;
		((ushort *)afterq)[1]= event;
		afterq[2]= val;
		afterq+= 3;
	}
}

void append_afterqueue()
{
	while( afterqueue != afterq) {
		afterq-= 3;
		addqueue(afterq[0], afterq[1], afterq[2]);
	}
}

int afterqtest()
{
	if(afterqueue != afterq) return 1;
	return 0;
}

void remake_qual()
{
	G.qual= get_qual();
}

char ext_load_str[256]= {0, 0};

void add_readfile_event(char *filename)
{	
	qenter(LOAD_FILE, 1);
	strcpy(ext_load_str, filename);
	convertstringcode(ext_load_str);
}

short ext_redraw=0, ext_inputchange=0, ext_mousemove=0, in_ext_qread=0;

ushort screen_qread(short *val)
{
	static oldwin= 0;
	ScrArea *sa;
	bWindow *win;
	ushort event;
	short newwin, rt, devs[2], vals[2];
	
	event= 0;

	if(in_ext_qread==0) {
		if(ext_inputchange) {
			*val= ext_inputchange;
			ext_inputchange= 0;
			event= INPUTCHANGE;
		}
		else if(ext_redraw) {
			*val= ext_redraw;
			ext_redraw= 0;
			event= REDRAW;
		}
		else if(ext_mousemove) {
			ext_mousemove= 0;
			event= MOUSEY;
		}
		else if(afterqueue!=afterq && qtest()==0 ) {
			*val= 0;
			event= AFTERQUEUE;
		}
	}
	
	if(event==0 ) {
		while(myqtest()==0) {
			int ww= 0;
			processEventsAndTimeouts();
			glutDoWorkList();
			if(myqtest()==0) usleep(1);
		}
		event= qread(val);
		
	}

	/* if(event==0) { */
	/* 	while(myqtest()==0) { */
	/* 		if(qtest()) { */
	/* 			processEventsAndTimeouts(); */
	/* 			glutDoWorkList(); */
	/* 			if(myqtest()==0) usleep(1); */
	/* 		} */
	/* 		if(myqtest()==0) if(R.flag & R_RENDERING) return 0; */
	/* 	} */
	/* 	event= qread(val); */
	/* } */

	if(G.curscreen==0) return event;
	
	if(event==Q_FIRSTTIME) {		
		glutDoWorkList();
		
	}
	else if(event==RIGHTSHIFTKEY || event== LEFTSHIFTKEY) {
		if(*val) G.qual |= LR_SHIFTKEY;
		else G.qual &= ~LR_SHIFTKEY;
	}
	else if(event==RIGHTALTKEY || event== LEFTALTKEY) {
		if(*val) G.qual |= LR_ALTKEY;
		else G.qual &= ~LR_ALTKEY;
	}
	else if(event==RIGHTCTRLKEY || event==LEFTCTRLKEY) {
		if(*val) G.qual |= LR_CTRLKEY;
		else G.qual &= ~LR_CTRLKEY;
	}
	else if(event==WINFREEZE || event==WINTHAW) {
		if((R.flag & R_RENDERING)==0) {
			if(R.win) winclose(R.win);
			R.win= 0;
			G.qual= 0;
		}
	}
#if 0
	else if(event==RESHAPE) {
		if(*val == mainwin) {
			bScreen *sc;
			int wx, wy, orx, ory;
			
			init_my_mainwin(mainwin);
						
			wx= glutGet(GLUT_WINDOW_WIDTH);
			wy= glutGet(GLUT_WINDOW_HEIGHT);
		
			orx= glutGet(GLUT_WINDOW_X);
			ory= displaysizey -wy- glutGet(GLUT_WINDOW_Y);
		
			/* Minimizing on windows causes glut to set the
			 * size to 0,  which means things go bonkers
			 */
			if (wx == 0 || wy == 0) return 0;
		
			prefstax= orx;
			prefstay= ory;
			prefsizx= wx;
			prefsizy= wy;
			
			sc= G.main->screen.first;
			while(sc) {
				test_scale_screen(sc);
				sc= sc->id.next;
			}
		
			testareas();
		}
		event= 0;
	}
#endif
	else if(event==INPUTCHANGE || event==REDRAW || event==DRAWEDGES) {
			/* DRAWEDGES: komt vanuit setscreen, qual opnieuw berekenen */
		
		if(*val==1) remake_qual();
		if(event==INPUTCHANGE && in_ext_qread==0) {
		
			if(*val) {
				mywinset(*val);
				G.curscreen->winakt= *val;
				oldwin= *val;
			}
			oldwin= 0;
		}
		
		if(event==REDRAW ) {
			/* kunstmatige mousy voor herberekenen winakt (als b.v. R.win naar achter gepusht */
			qenter(MOUSEY, scrmousey);
		}
		
	}
	else if(event==MOUSEX || event==MOUSEY) {
		
		if(event==MOUSEY && in_ext_qread==0 && (R.win==0 || G.curscreen->winakt!=R.win)) {

			/* testen waar muis staat */
			newwin= 0;
			win= swindowbase.first;
			while(win) {
				if(scrmousex>win->xmin && scrmousex<win->xmax) {
					/* deze uitzondering betreft onderste en bovenste edge: voor edit cursonedge */
					if( (scrmousey==0 && scrmousey==win->ymin) || (scrmousey==G.curscreen->endy && scrmousey==win->ymax)) {
						if(scrmousey>win->ymin && scrmousey<win->ymax) {
							newwin= win->id;
							break;
						}
					}
					else if(scrmousey>=win->ymin && scrmousey<=win->ymax) {
						newwin= win->id;
						break;
					}
				}
				win= win->next;
			}
			/* cursor */
			if(newwin != oldwin) {
				
				if(newwin==0) {
					set_cursonedge(scrmousex, scrmousey);
				}
				else if(oldwin==0) {
					cursonedge= 0;
				}
				if(newwin) {
					sa= areawinar[newwin];
					if(sa->win==newwin) glutSetCursor(sa->cursor);
					else glutSetCursor(CURSOR_STD);
				}
			}
			else if(newwin==0 && oldwin==0) {
				set_cursonedge(scrmousex, scrmousey);
			}
/*
			else if (scrmousex<2 || scrmousey<2 ||
					abs(scrmousex - G.curscreen->sizex)<2||
					abs(scrmousey - G.curscreen->sizey)<2) {
				set_cursonedge(scrmousex, scrmousey);						
			}
*/

			if(newwin!=0) {

				if(newwin != oldwin || G.curscreen->winakt==0) {
					event= INPUTCHANGE;
					*val= newwin;
				}
			}

			oldwin= newwin;
			
		}
	}
	else if(event==TIMER0) {
		event= 0;
		if(in_ext_qread==0) {
			write_autosave();
			autosavetime= 0;
		}
	}
	else if(event==LOAD_FILE) {
		event= 0;
		
		read_file(ext_load_str);
		ext_load_str[0]=0;;
	}
	return event;
}

ushort special_qread(short *val)
{
	/* simul alternatief voor extern_qread */
	ushort event;

	in_ext_qread= 1;	/* niet zo net, wel zo handig (zie screen_qread) */

	event= screen_qread(val);
	
	in_ext_qread= 0;
	
	if(event==REDRAW) ext_redraw= *val;
	else if(event==INPUTCHANGE) ext_inputchange= *val;
	else if(event==MOUSEY || event==MOUSEX) {
		ext_mousemove= 1;
		event= 0;
	}
	
	return event;
}

short ext_qtest()
{
	if(ext_inputchange) return INPUTCHANGE;
	else if(ext_redraw) return REDRAW;
	else if(ext_mousemove) return MOUSEY;
	else return myqtest();
}


ushort extern_qread(short *val)
{
	/* bewaart de laatste INPUTCHANGE en de laatste REDRAW */
	ushort event;
	
	in_ext_qread= 1;	/* niet zo net, wel zo handig (zie screen_qread) */
	
	event= screen_qread(val);
	if(event==REDRAW) ext_redraw= *val;
	else if(event==INPUTCHANGE) ext_inputchange= *val;
	else if(event==MOUSEY || event==MOUSEX) ext_mousemove= 1;
	else if(event==RIGHTSHIFTKEY || event== LEFTSHIFTKEY) {
		if(*val) G.qual |= LR_SHIFTKEY;
		else G.qual &= ~LR_SHIFTKEY;
	}
	else if(event==RIGHTALTKEY || event== LEFTALTKEY) {
		if(*val) G.qual |= LR_ALTKEY;
		else G.qual &= ~LR_ALTKEY;
	}
	else if(event==RIGHTCTRLKEY || event==LEFTCTRLKEY) {
		if(*val) G.qual |= LR_CTRLKEY;
		else G.qual &= ~LR_CTRLKEY;
	}
	else if(G.qual & LR_CTRLKEY && event==F3KEY) {
		screendump();
	}
	in_ext_qread= 0;

	return event;
}

void markdirty_all()
{
	ScrArea *sa;

		
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->win) {
			addqueue(sa->win, REDRAW, 1);
			sa->win_swap &= ~WIN_FRONT_OK;
		}
		if(sa->headwin) {
			addqueue(sa->headwin, REDRAW, 1);
			sa->head_swap &= ~WIN_FRONT_OK;
		}
		sa= sa->next;
	}
}

/* BePatch! glutGetFrontbuffer is defined by glDrawBuffer! In fact the name is wrong... */

void header_front_to_back_glut(ScrArea *sa)
{

	glDrawBuffer(GL_FRONT);
	glRasterPos2f( (float) sa->headrct.xmin -0.1,  (float) sa->headrct.ymin-0.1);
	glutGetFrontBuffer(sa->headrct.xmin,  sa->headrct.ymin,  sa->winx, HEADERY-1);
	
	glDrawBuffer(GL_BACK);
	glutPutFrontBuffer();
	
	sa->head_swap= WIN_EQUAL;
}

void window_front_to_back_glut(ScrArea *sa)
{

	glDrawBuffer(GL_FRONT);
	glRasterPos2f( (float) sa->winrct.xmin -0.1,  (float) sa->winrct.ymin-0.1);
	glutGetFrontBuffer(sa->winrct.xmin,  sa->winrct.ymin,  sa->winx,  sa->winy);
	
	glDrawBuffer(GL_BACK);
	glutPutFrontBuffer();
	
	sa->win_swap= WIN_EQUAL;
}

void header_back_to_front_glut(ScrArea *sa)
{

	glDrawBuffer(GL_BACK);
	glRasterPos2f( (float) sa->headrct.xmin -0.1,  (float) sa->headrct.ymin-0.1);
	glutGetFrontBuffer(sa->headrct.xmin,  sa->headrct.ymin,  sa->winx,  HEADERY-1);
	
	glDrawBuffer(GL_FRONT);
	glutPutFrontBuffer();
	glDrawBuffer(GL_BACK);
	
	sa->head_swap= WIN_EQUAL;
}

/* sgi */

void header_front_to_back_ogl(ScrArea *sa)
{

	glReadBuffer(GL_FRONT);
	
	glRasterPos2f( (float) sa->headrct.xmin -0.1,  (float) sa->headrct.ymin-0.1);
	
	glCopyPixels(sa->headrct.xmin,  sa->headrct.ymin,  sa->winx, HEADERY, GL_COLOR);
	sa->head_swap= WIN_EQUAL;
	
	glReadBuffer(GL_BACK);
}

void window_front_to_back_ogl(ScrArea *sa)
{

	glReadBuffer(GL_FRONT);

	glRasterPos2f( (float) sa->winrct.xmin -0.1,  (float) sa->winrct.ymin-0.1);
	
	glCopyPixels(sa->winrct.xmin,  sa->winrct.ymin,  sa->winx,  sa->winy, GL_COLOR);
	sa->win_swap= WIN_EQUAL;

	glReadBuffer(GL_BACK);
}

void header_back_to_front_ogl(ScrArea *sa)
{

	glDrawBuffer(GL_FRONT);
	
	glRasterPos2f( (float) sa->headrct.xmin -0.1,  (float) sa->headrct.ymin-0.1);

	glCopyPixels(sa->headrct.xmin,  sa->headrct.ymin,  sa->winx,  HEADERY, GL_COLOR);
	sa->head_swap= WIN_EQUAL;
	
	glDrawBuffer(GL_BACK);
}


void screen_swapbuffers_OGL()
{
	ScrArea *sa;
	int doswap= 0, headswap=0, oldwin;

	/* test op swap */
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->win_swap==WIN_BACK_OK) doswap= 1;
		if(sa->head_swap==WIN_BACK_OK) headswap= 1;
		
		sa= sa->next;
	}

	if(doswap==0 && headswap==0) return;

	oldwin= winget();
	mywinset(G.curscreen->mainwin);

	if(doswap) {
		
		sa= G.curscreen->areabase.first;
		while(sa) {
			if(sa->win_swap== WIN_FRONT_OK) window_front_to_back_ogl(sa);
			
			if(sa->head_swap== WIN_FRONT_OK) header_front_to_back_ogl(sa);
			
			sa= sa->next;

		}
		myswapbuffers();
	}
	
	/* headers moeten in front en back gelijk zijn: dus: */
	/* heads van back naar frontbuffer? of omgekeerd? */
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->head_swap==WIN_FRONT_OK) {
			header_front_to_back_ogl(sa);
		}
		else if(sa->head_swap==WIN_BACK_OK) {
			header_back_to_front_ogl(sa);
		}
		
		sa= sa->next;
	}
	
	if(oldwin) areawinset(oldwin);

}

void screen_swapbuffers_GLUT()
{
	ScrArea *sa;
	int doswap= 0, headswap=0, oldwin;

	/* test op swap */
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->win_swap==WIN_BACK_OK) doswap= 1;
		if(sa->head_swap==WIN_BACK_OK) headswap= 1;
		
		sa= sa->next;
	}

	if(doswap==0 && headswap==0) return;

	oldwin= winget();
	mywinset(G.curscreen->mainwin);

	if(doswap) {
		
		sa= G.curscreen->areabase.first;
		while(sa) {
			
			if(sa->win_swap== WIN_FRONT_OK) window_front_to_back_glut(sa);
			
			if(sa->head_swap== WIN_FRONT_OK) header_front_to_back_glut(sa);
			
			sa= sa->next;

		}
		myswapbuffers();
	}
	
	/* headers moeten in front en back gelijk zijn: dus: */
	/* heads van back naar frontbuffer? of omgekeerd? */
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->head_swap==WIN_FRONT_OK) {
			header_front_to_back_glut(sa);
		}
		else if(sa->head_swap==WIN_BACK_OK) {
			header_back_to_front_glut(sa);
		}
		
		sa= sa->next;
	}
	
	if(oldwin) areawinset(oldwin);

}


void screen_swapbuffers_REDRAW()
{
	ScrArea *sa;
	int oldwin, doswap= 0, swap;
	
	oldwin= winget();

	/* dit is een nieuwe implementatie: uitsluitend met redraws en normale swapbuffer */

	/* allemaal front ok? */
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->win && (sa->win_swap & WIN_FRONT_OK)==0) break;
		if((sa->head_swap & WIN_FRONT_OK)==0) break;
		sa= sa->next;
	}
	if(sa==0) return;

	/* printf("front not OK %d %d %d %d\n", sa->win, sa->win_swap, sa->headwin, sa->head_swap); */
	
	sa= G.curscreen->areabase.first;
	while(sa) {
	
		swap= sa->win_swap;
		if( (swap & WIN_BACK_OK) == 0) {
			if(sa->win && sa->windraw) {
				areawinset(sa->win);
				sa->windraw();
				doswap= 1;
			}
			sa->win_swap= swap | WIN_BACK_OK;
		}
		else if( sa->win_swap==WIN_BACK_OK) doswap= 1;
		
		swap= sa->head_swap;
		if( (swap & WIN_BACK_OK) == 0) {
			
			areawinset(sa->headwin);
			if(sa->headdraw) sa->headdraw(); 
			doswap= 1;

			sa->head_swap = swap | WIN_BACK_OK;
		}
		else if( sa->head_swap==WIN_BACK_OK) doswap= 1;
		
		sa= sa->next;
	}

	/* de hele backbuffer moet nu OK zijn */
	if(doswap) {
		myswapbuffers();
	}
	if(oldwin) areawinset(oldwin);

}
	
void screen_swapbuffers_MESA()
{
	ScrArea *sa;

	/* dit is een nieuwe implementatie: uitsluitend met partial swap */

	/* test op swap */
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->win_swap==WIN_BACK_OK) {
			myCopySubBuffer(sa->winrct.xmin, sa->winrct.ymin-1, sa->winx, sa->winy);
			sa->win_swap= WIN_EQUAL;
		}
		if(sa->head_swap==WIN_BACK_OK) {
			myCopySubBuffer(sa->headrct.xmin, sa->headrct.ymin-1, sa->winx, HEADERY);
			sa->head_swap= WIN_EQUAL;
		}
		
		sa= sa->next;
	}
}

void screen_swapbuffers()
{

	#ifdef __sgi
	screen_swapbuffers_OGL();
	#endif

	#ifdef MESA30
		screen_swapbuffers_MESA();
	#else
		screen_swapbuffers_REDRAW();
	#endif

	#ifdef __WIN32
	screen_swapbuffers_REDRAW();
	#endif

	#ifdef __BeOS
	screen_swapbuffers_GLUT();
	#endif

	#ifdef __SUN
		#ifndef MESA
		screen_swapbuffers_OGL();
		#endif
	#endif
}


int is_allowed_to_change_screen(bScreen *new)
{
	/* niet als curscreen is full
	 * niet als obedit && old->scene!=new->scene
	 */
	
	if(new==0) return 0;
	if(G.curscreen->full) return 0;
	if(curarea->full) return 0;
	if(G.obedit) {
		if(G.curscreen->scene!=new->scene) return 0;
	}
	return 1;
}


void screenmain()
{
	extern int Xqual;
	ScrArea *sa, *sa1, *sa2;
	bScreen *sc;
	float fac;
	int wx, wy, orx, ory;
	ushort event;
	short a, val, towin, dodrawscreen= 0, inqueue, debugval= 0;
	
	/* keert terug naar de glut! */

	while(qtest() || afterqtest()) {
		event= screen_qread(&val);
		towin= event;
		
		if( (G.f & G_DEBUG) && event && event!=MOUSEY) {
			PRINT3(d, d, d, event, val, G.qual);
			debugval= 1;
		}

		if(event==LEFTMOUSE) {
			if(val && cursonedge) {
				moveareas();
				towin= 0;
			}
		}
		else if(event==MIDDLEMOUSE) {
			if(val && cursonedge) {
				editsplitpoint();
				towin= 0;
			}
		}
		else if(event==RIGHTMOUSE) {
			if(val && cursonedge) {
				joinarea(curarea);
				towin= 0;
			}
		}
		else if(event==QKEY) {
			if((G.obedit && G.obedit->type==OB_FONT && curarea->spacetype==SPACE_VIEW3D)||curarea->spacetype==SPACE_TEXT);
			else {
				if(okee("QUIT BLENDER")) exit_usiblender();
				towin= 0;
			}
		}
		else if(event==SPACEKEY) {
			if((G.obedit && G.obedit->type==OB_FONT && curarea->spacetype==SPACE_VIEW3D)||curarea->spacetype==SPACE_TEXT);
			else {
				if(val) toolbox();
				towin= 0;
			}
		}
		else if(event==INPUTCHANGE) {
		
			/* welke headers moeten redraw? */
			if(val>3) {	/* eerste drie nummers voor GL */

				if( G.curscreen->winakt  !=  val ) {
					/* de oude en nieuwe area */
					sa1= areawinar[G.curscreen->winakt];
					sa2= areawinar[val];
					
					if(sa1==sa2);
					else {
						if(sa1) addqueue(sa1->headwin, REDRAW, 1);
						if(sa2) addqueue(sa2->headwin, REDRAW, 1);
					}
				}

				/* testen of window nog bestaat (oude event bij join b.v.) */
				if(areawinar[val]) {
					/* als winakt==R.win mag alleen een GL-INPUTCHANGE winakt zetten */
					if(R.win==0 || G.curscreen->winakt!=R.win) G.curscreen->winakt= val;
					clear_global_filesel_vars();

				}
				else {
					G.curscreen->winakt= 0;
				}
			}
			towin= 0;
		}
		else if(event==DRAWEDGES) {
			towin= 0;
			dodrawscreen= 1;
		}
		else if(event==REDRAW) {
			towin= 0;
			if(val==G.curscreen->mainwin) {
				markdirty_all();
				
				dodrawscreen= 1;

			}
			else if(val>3) {
				addqueue(val, REDRAW, val);
			}
			else if(R.win && val==R.win) {
				mywinset(R.win);
				getorigin(&orx, &ory);
				
				R.winxof= orx;
				R.winyof= ory;
				redraw_render_win(val);
			}
		}
		else if(event==RIGHTARROWKEY) {
			if(val && (G.qual & LR_CTRLKEY)) {
				sc= G.curscreen->id.next;
				if(is_allowed_to_change_screen(sc)) setscreen(sc);
				towin= 0;
			}
		}
		else if(event==LEFTARROWKEY) {
			if(val && (G.qual & LR_CTRLKEY)) {
				sc= G.curscreen->id.prev;
				if(is_allowed_to_change_screen(sc)) setscreen(sc);
				towin= 0;
			}
		}
		else if(event==UPARROWKEY || event==DOWNARROWKEY) {
			if(val && (G.qual & LR_CTRLKEY)) {
				area_fullscreen();
				towin= 0;
			}
		}
		else if(event==AFTERQUEUE) {
			append_afterqueue();
		}

		if(towin) {

			towin= blenderqread(event, val);
			
			if(towin && G.curscreen->winakt) addqueue(G.curscreen->winakt, event, val);
		}
		
		/* window queues en swapbuffers */
		event= ext_qtest();
		if(event==0 || event==EXECUTE) {		/* || event==MOUSEY ?? */
		
			inqueue= 1;
			while(inqueue) {
				inqueue= 0;
				winqueue_break= 0;
				
				sa= G.curscreen->areabase.first;
				while(sa) {
					/* bewust eerst header afhandelen, dan rest. Header is soms init */
					if(sa->headwin && sa->headqueue!=sa->hq) {
						defheadqread(sa); inqueue= 1;
					}
					if(winqueue_break) { /* mogelijk nieuwe G.curscreen */
						inqueue= 1;
						break;
					}

					if(sa->win && sa->winqueue!=sa->wq) {
						defwinqread(sa); inqueue= 1;
					}
					if(winqueue_break) { /* mogelijk nieuwe G.curscreen */
						inqueue= 1;
						break;
					}
					sa= sa->next;
				}
			}

			screen_swapbuffers();
			
			if(dodrawscreen) {
				drawscreen();

				dodrawscreen= 0;
			}

		}
		/* restore actieve area */
		if(G.curscreen->winakt != winget()) areawinset(G.curscreen->winakt);
	}

	/* de enige idle */
	usleep(2);

	if(debugval==1 && (G.f & G_DEBUG)) {
		debugval= glGetError();
		while( debugval!=GL_NO_ERROR) {
			printf("GL_ERROR: %d %s\n", debugval, gluErrorString(debugval));
			debugval= glGetError();
		}
		printf("end screenmain\n");
	}
}

/* *********  AREAS  ************* */

void getdisplaysize()
{
	
	displaysizex= glutGet(GLUT_SCREEN_WIDTH);
	displaysizey= glutGet(GLUT_SCREEN_HEIGHT);

}

void setprefsize(int stax, int stay, int sizx, int sizy)
{

	if(stax<0) stax= 0;
	if(stay<0) stay= 0;
	if(sizx<320) sizx= 320;
	if(sizy<256) sizy= 256;

	if(stax+sizx>displaysizex) sizx= displaysizex-stax;
	if(stay+sizy>displaysizey) sizy= displaysizey-stay;
	if(sizx<320 || sizy<256) {
		printf("ERROR: illegal prefsize\n");
		return;
	}
	
	prefstax= stax;
	prefstay= stay;
	prefsizx= sizx;
	prefsizy= sizy;
}


ScrArea *findcurarea()
{
	ScrArea *sa;
	short mval[2];
	
	getmouseco_sc(mval);
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->v1->vec.x<=mval[0] && sa->v3->vec.x>=mval[0]) {
			if(sa->v1->vec.y<=mval[1] && sa->v2->vec.y>=mval[1]) {
				return sa;
			}
		}
		sa= sa->next;
	}
	return 0;
}

ScrVert *addscrvert(ListBase *lb, short x, short y)
{
	ScrVert *sv;
	
	sv= callocN(sizeof(ScrVert), "addscrvert");
	sv->vec.x= x;
	sv->vec.y= y;
	
	if(lb) addtail(lb, sv);
	
	return sv;
}

void sortscrvert(ScrVert **v1, ScrVert **v2)
{
	ScrVert *tmp;
	
	if ((long)*v1 > (long)*v2) {
		tmp= *v1;
		*v1= *v2;
		*v2= tmp;	
	}
}

ScrEdge *addscredge(ListBase *lb, ScrVert *v1, ScrVert *v2)
{
	ScrEdge *se;
	
	se= callocN(sizeof(ScrEdge), "addscredge");

	sortscrvert(&v1, &v2);
	se->v1= v1;
	se->v2= v2;
	
	if(lb) addtail(lb, se);

	return se;
}

ScrEdge *findscredge(ScrVert *v1, ScrVert *v2)
{
	ScrVert *sv;
	ScrEdge *se;
	
	sortscrvert(&v1, &v2);
	se= G.curscreen->edgebase.first;
	while(se) {
		if(se->v1==v1 && se->v2==v2) return se;
		se= se->next;
	}
	return 0;
}

ScrEdge *findscredge_sc(bScreen *sc, ScrVert *v1, ScrVert *v2)
{
	ScrVert *sv;
	ScrEdge *se;
	
	sortscrvert(&v1, &v2);
	se= sc->edgebase.first;
	while(se) {
		if(se->v1==v1 && se->v2==v2) return se;
		se= se->next;
	}
	return 0;
}

void removedouble_scrverts()
{
	ScrVert *v1, *verg;
	ScrEdge *se;
	ScrArea *sa;
	
	verg= G.curscreen->vertbase.first;
	while(verg) {
		if(verg->new==0) {	/* !!! */
			v1= verg->next;
			while(v1) {
				if(v1->new==0) {	/* !?! */
					if(v1->vec.x==verg->vec.x && v1->vec.y==verg->vec.y) {
						/* printf("doublevert\n"); */
						v1->new= verg;
					}
				}
				v1= v1->next;
			}
		}
		verg= verg->next;
	}
	
	/* vervang pointers in edges en vlakken */
	se= G.curscreen->edgebase.first;
	while(se) {
		if(se->v1->new) se->v1= se->v1->new;
		if(se->v2->new) se->v2= se->v2->new;
		/* edges zijn veranderd: dus.... */
		sortscrvert(&(se->v1), &(se->v2));
		se= se->next;
	}
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->v1->new) sa->v1= sa->v1->new;
		if(sa->v2->new) sa->v2= sa->v2->new;
		if(sa->v3->new) sa->v3= sa->v3->new;
		if(sa->v4->new) sa->v4= sa->v4->new;
		sa= sa->next;
	}
	
	/* verwijderen */
	verg= G.curscreen->vertbase.first;
	while(verg) {
		v1= verg->next;
		if(verg->new) {
			remlink(&G.curscreen->vertbase, verg);
			freeN(verg);
		}
		verg= v1;
	}
	
}

void removenotused_scrverts()
{
	ScrVert *sv, *svn;
	ScrEdge *se;
	ScrArea *sa;

	/* ga ervan uit dat de edges goed zijn */
	
	se= G.curscreen->edgebase.first;
	while(se) {
		se->v1->flag= 1;
		se->v2->flag= 1;
		se= se->next;
	}
	
	sv= G.curscreen->vertbase.first;
	while(sv) {
		svn= sv->next;
		if(sv->flag==0) {
			remlink(&G.curscreen->vertbase, sv);
			freeN(sv);
		}
		else sv->flag= 0;
		sv= svn;
	}
}

void removedouble_scredges()
{
	ScrVert *v1;
	ScrEdge *verg, *se, *sn;
	
	/* vergelijken */
	verg= G.curscreen->edgebase.first;
	while(verg) {
		se= verg->next;
		while(se) {
			sn= se->next;
			if(verg->v1==se->v1 && verg->v2==se->v2) {
				remlink(&G.curscreen->edgebase, se);
				freeN(se);
			}
			se= sn;
		}
		verg= verg->next;
	}
}

void removenotused_scredges()
{
	ScrVert *sv;
	ScrEdge *se, *sen;
	ScrArea *sa;
	int a=0;
	
	/* zet flag als edge gebruikt wordt in area */
	sa= G.curscreen->areabase.first;
	while(sa) {
		se= findscredge(sa->v1, sa->v2);
		if(se==0) printf("error: area %d edge 1 bestaat niet\n", a);
		else se->flag= 1;
		se= findscredge(sa->v2, sa->v3);
		if(se==0) printf("error: area %d edge 2 bestaat niet\n", a);
		else se->flag= 1;
		se= findscredge(sa->v3, sa->v4);
		if(se==0) printf("error: area %d edge 3 bestaat niet\n", a);
		else se->flag= 1;
		se= findscredge(sa->v4, sa->v1);
		if(se==0) printf("error: area %d edge 4 bestaat niet\n", a);
		else se->flag= 1;
		sa= sa->next;
		a++;
	}
	se= G.curscreen->edgebase.first;
	while(se) {
		sen= se->next;
		if(se->flag==0) {
			remlink(&G.curscreen->edgebase, se);
			freeN(se);
		}
		else se->flag= 0;
		se= sen;
	}
}

void calc_arearcts(ScrArea *sa)
{

	if(sa->v1->vec.x>0) sa->totrct.xmin= sa->v1->vec.x+EDGEWIDTH2+1;
	else sa->totrct.xmin= sa->v1->vec.x;
	if(sa->v4->vec.x<G.curscreen->sizex-1) sa->totrct.xmax= sa->v4->vec.x-EDGEWIDTH2-1;
	else sa->totrct.xmax= sa->v4->vec.x;
	
	if(sa->v1->vec.y>0) sa->totrct.ymin= sa->v1->vec.y+EDGEWIDTH2+1;
	else sa->totrct.ymin= sa->v1->vec.y;
	if(sa->v2->vec.y<G.curscreen->sizey-1) sa->totrct.ymax= sa->v2->vec.y-EDGEWIDTH2-1;
	else sa->totrct.ymax= sa->v2->vec.y;
		
	sa->winrct= sa->totrct;
	if(sa->headertype) {
		sa->headrct= sa->totrct;
		if(sa->headertype==HEADERDOWN) {
			sa->headrct.ymax= sa->headrct.ymin+HEADERY-1;
			sa->winrct.ymin= sa->headrct.ymax+1;
		}
		else if(sa->headertype==HEADERTOP) {
			sa->headrct.ymin= sa->headrct.ymax-HEADERY+1;
			sa->winrct.ymax= sa->headrct.ymin-1;
		}
	}
	if(sa->winrct.ymin>sa->winrct.ymax) sa->winrct.ymin= sa->winrct.ymax;
	
	/* als speedup voor berekeningen */
	sa->winx= sa->winrct.xmax-sa->winrct.xmin+1;
	sa->winy= sa->winrct.ymax-sa->winrct.ymin+1;
}

void openheadwin(ScrArea *sa)
{

	sa->headwin= myswinopen(G.curscreen->mainwin,
		sa->headrct.xmin, sa->headrct.xmax, sa->headrct.ymin, sa->headrct.ymax);

	glMatrixMode(GL_MODELVIEW);
	
	if(sa->headqueue==0) sa->headqueue= sa->hq= mallocN(sizeof(short)*MAXQUEUE, "headqueue");
	
	areawinar[sa->headwin]= sa;	/* anders doet addqueue het niet */
	addqueue(sa->headwin, CHANGED, 1);
	
}

void openareawin(ScrArea *sa)
{

	sa->win= myswinopen(G.curscreen->mainwin, 
		sa->winrct.xmin, sa->winrct.xmax, sa->winrct.ymin, sa->winrct.ymax);


	if(sa->winqueue==0) sa->winqueue= sa->wq= mallocN(sizeof(short)*MAXQUEUE, "winqueue");

	areawinar[sa->win]= sa;	/* anders doet addqueue het niet */
	addqueue(sa->win, CHANGED, 1);
	
}

void closeheadwin(ScrArea *sa)
{
	
	if(G.curscreen && sa->headwin==G.curscreen->winakt) G.curscreen->winakt= 0;
	
	if(sa->headwin) winclose(sa->headwin);
	sa->headwin= 0;
	sa->headdraw= 0;
	if(sa->headqueue) {
		freeN(sa->headqueue);
		sa->headqueue= sa->hq= 0;
	}
}

void closeareawin(ScrArea *sa)
{

	if(G.curscreen && sa->win==G.curscreen->winakt) G.curscreen->winakt= 0;
	
	if(sa->win) winclose(sa->win);
	sa->win= 0;
	if(sa->winqueue) {
		freeN(sa->winqueue);
		sa->winqueue= sa->wq= 0;
	}
}

void del_area(ScrArea *sa)
{
	closeareawin(sa);
	closeheadwin(sa);

	freespacelist(&sa->spacedata);
}

void copy_areadata(ScrArea *sa1, ScrArea *sa2)
{

	sa1->headertype= sa2->headertype;
	sa1->spacetype= sa2->spacetype;
	sa1->butspacetype= sa2->spacetype;
	sa1->headdraw= sa2->headdraw;
	sa1->windraw= sa2->windraw;
	sa1->headqread= sa2->headqread;
	sa1->winqread= sa2->winqread;
	sa1->headchange= sa2->headchange;
	sa1->winchange= sa2->winchange;
	Mat4CpyMat4(sa1->winmat, sa2->winmat);
	
	freespacelist(&sa1->spacedata);
	
	duplicatespacelist(&sa1->spacedata, &sa2->spacedata);
	
}

ScrArea *addscrarea(lb, v1, v2, v3, v4, headertype, spacetype)
ListBase *lb;
ScrVert *v1, *v2, *v3, *v4;
short headertype, spacetype;
{
	ScrArea *sa;
	
	sa= callocN(sizeof(ScrArea), "addscrarea");
	sa->cursor= CURSOR_STD;
	sa->v1= v1;
	sa->v2= v2;
	sa->v3= v3;
	sa->v4= v4;
	sa->headertype= headertype;
	sa->spacetype= sa->butspacetype= spacetype;

	calc_arearcts(sa);

	if(headertype) openheadwin(sa);
	if(sa->winrct.ymin<sa->winrct.ymax) openareawin(sa);

	if(lb) addtail(lb, sa);

	return sa;
}

void testareas()
{
	ScrArea *sa;
	rcti tempw, temph;
	
	/* testen of header er moet zijn, of weg moet, of verplaatst */
	/* testen of window er moet zijn, of weg moet, of verplaatst */

	sa= G.curscreen->areabase.first;
	while(sa) {
		
		tempw= sa->winrct;
		temph= sa->headrct;
		calc_arearcts(sa);

		/* test header */
		if(sa->headertype) {
			if(sa->headwin==0) openheadwin(sa);
			else {
				
				/* is header op goede plek? */
				if(temph.xmin!=sa->headrct.xmin || temph.xmax!=sa->headrct.xmax ||
					temph.ymin!=sa->headrct.ymin || temph.ymax!=sa->headrct.ymax) {

					addqueue(sa->headwin, CHANGED, 1);
					mywinposition(sa->headwin, sa->headrct.xmin, sa->headrct.xmax, sa->headrct.ymin, sa->headrct.ymax);

				}
				
				if(sa->headbutlen<sa->winx) {
					sa->headbutofs= 0;
					addqueue(sa->headwin, CHANGED, 1);
				}
				else if(sa->headbutofs+sa->winx > sa->headbutlen) {
					sa->headbutofs= sa->headbutlen-sa->winx;
					addqueue(sa->headwin, CHANGED, 1);
				}
			}
		}
		else {
			if(sa->headwin) closeheadwin(sa);
		}

		/* test areawindow */
		if(sa->win==0) {
			if(sa->winrct.ymin<sa->winrct.ymax) openareawin(sa);
		}
		else { /* window te klein? */
			if(sa->winrct.ymin==sa->winrct.ymax) closeareawin(sa);
			else {  /* window veranderd? */
				if(tempw.xmin!=sa->winrct.xmin || tempw.xmax!=sa->winrct.xmax ||
					tempw.ymin!=sa->winrct.ymin || tempw.ymax!=sa->winrct.ymax) {

					addqueue(sa->win, CHANGED, 1);
					mywinposition(sa->win, sa->winrct.xmin, sa->winrct.xmax, sa->winrct.ymin, sa->winrct.ymax);
				}
				
			}
		}

		sa= sa->next;
	}
	
	/* remake global windowarray */
	bzero(areawinar, 4*MAXWIN);
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->headwin) areawinar[sa->headwin]= sa;
		if(sa->win) areawinar[sa->win]= sa;
		sa= sa->next;
	}
	
	/* test of winakt in orde is */	
	if( areawinar[G.curscreen->winakt]==0) G.curscreen->winakt= 0;
}

ScrArea *test_edge_area(ScrArea *sa, ScrEdge *se)
{
	/* test of edge in area ligt, zo niet, 
	   vind een area die 'm wel heeft */
  
	ScrEdge *se1=0, *se2=0, *se3=0, *se4=0;
	
	if(sa) {
		se1= findscredge(sa->v1, sa->v2);
		se2= findscredge(sa->v2, sa->v3);
		se3= findscredge(sa->v3, sa->v4);
		se4= findscredge(sa->v4, sa->v1);
	}
	if(se1!=se && se2!=se && se3!=se && se4!=se) {
		
		sa= G.curscreen->areabase.first;
		while(sa) {
			/* een beetje optimaliseren? */
			if(se->v1==sa->v1 || se->v1==sa->v2 || se->v1==sa->v3 || se->v1==sa->v4) {
				se1= findscredge(sa->v1, sa->v2);
				se2= findscredge(sa->v2, sa->v3);
				se3= findscredge(sa->v3, sa->v4);
				se4= findscredge(sa->v4, sa->v1);
				if(se1==se || se2==se || se3==se || se4==se) return sa;
			}
			sa= sa->next;
		}
	}

	return sa;	/* is keurig 0 als niet gevonden */
}

ScrArea *closest_bigger_area()
{
	ScrArea *sa, *big=0;
	float cent[3], vec[3],len, len1, len2, len3, dist=1000;
	short mval[2];
	
	getmouseco_sc(mval);
	
	cent[0]= mval[0];
	cent[1]= mval[1];
	cent[2]= vec[2]= 0;

	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa!=curarea) {
			if(sa->winy>=curarea->winy) {
			
				/* mimimum van vier hoekpunten */
				vec[0]= sa->v1->vec.x; vec[1]= sa->v1->vec.y;
				len= VecLenf(vec, cent);
				vec[0]= sa->v2->vec.x; vec[1]= sa->v2->vec.y;
				len1= VecLenf(vec, cent);
				vec[0]= sa->v3->vec.x; vec[1]= sa->v3->vec.y;
				len2= VecLenf(vec, cent);
				vec[0]= sa->v4->vec.x; vec[1]= sa->v4->vec.y;
				len3= VecLenf(vec, cent);
				
				len= MIN4(len, len1, len2, len3);
				
				/* plus centrum */
				vec[0]= (sa->v2->vec.x+sa->v3->vec.x)/2;
				vec[1]= (sa->v1->vec.y+sa->v2->vec.y)/2;

				len+= 0.5*VecLenf(vec, cent);
				
				/* min afmeting */
				len-= sa->winy+sa->winx;
				
				if(len<dist) {
					dist= len;
					big= sa;
				}
			}
		}
		sa= sa->next;
	}
	
	if(big) return big;
	else return curarea;
}

/* ************ SCREENBEHEER ************** */

void test_scale_screen(bScreen *sc)
/* testen of screenvertices vergroot/verkleind moeten worden */
/* testen of offset nog klopt */
{
	ScrVert *sv=0;
	ScrEdge *se;
	ScrArea *sa, *san;
	bScreen *old;
	int yval;
	float facx, facy, tempf, min[2], max[2];

	sc->startx= prefstax;
	sc->starty= prefstay;
	sc->endx= prefstax+prefsizx-1;
	sc->endy= prefstay+prefsizy-1;

	/* calculate size */
	sv= sc->vertbase.first;
	min[0]= min[1]= 0.0;
	max[0]= sc->sizex;
	max[1]= sc->sizey;
	while(sv) {
		min[0]= MIN2(min[0], sv->vec.x);
		min[1]= MIN2(min[1], sv->vec.y);
		max[0]= MAX2(max[0], sv->vec.x);
		max[1]= MAX2(max[1], sv->vec.y);
		sv= sv->next;
	}

	/* always make 0.0 left under */
	sv= sc->vertbase.first;
	while(sv) {
		sv->vec.x -= min[0];
		sv->vec.y -= min[1];
		sv= sv->next;
	}
	

	sc->sizex= max[0]-min[0];
	sc->sizey= max[1]-min[1];

	if(sc->sizex!= prefsizx || sc->sizey!= prefsizy) {
		facx= prefsizx;
		facx/= (float)sc->sizex;
		facy= prefsizy;
		facy/= (float)sc->sizey;

		/* make sure it fits! */
		sv= sc->vertbase.first;
		while(sv) {
			tempf= ((float)sv->vec.x)*facx;
			sv->vec.x= (short)(tempf+0.5);
			sv->vec.x+= AREAGRID-1;
			sv->vec.x-=  (sv->vec.x % AREAGRID); 
			
			CLAMP(sv->vec.x, 0, prefsizx);

			tempf= ((float)sv->vec.y )*facy;
			sv->vec.y= (short)(tempf+0.5);
			sv->vec.y+= AREAGRID-1;
			sv->vec.y-=  (sv->vec.y % AREAGRID); 
			
			CLAMP(sv->vec.y, 0, prefsizy);

			sv= sv->next;
		}
		
		sc->sizex= prefsizx;
		sc->sizey= prefsizy;
	}

	/* test for collapsed areas. This could happen in some blender version... */
	sa= sc->areabase.first;
	while(sa) {
		san= sa->next;
		if(sa->v1==sa->v2 || sa->v3==sa->v4 || sa->v2==sa->v3) {
			del_area(sa);
			freeN(sa);
			remlink(&sc->areabase, sa);
		}
		sa= san;
	}

	/* make each window at least HEADERY high */

	sa= sc->areabase.first;
	while(sa) {

		if(sa->v1->vec.y+HEADERY > sa->v2->vec.y) {
			/* lower edge */
			se= findscredge_sc(sc, sa->v4, sa->v1);
			if(se && sa->v1!=sa->v2 ) {
				select_connected_scredge(sc, se);
				
				/* all selected vertices get the right offset */
				yval= sa->v2->vec.y-HEADERY;
				sv= sc->vertbase.first;
				while(sv) {
					/* if is a collapsed area */
					if(sv!=sa->v2 && sv!=sa->v3) {
						if(sv->flag) sv->vec.y= yval;
					}
					sv= sv->next;
				}
			}
		}

		sa= sa->next;
	}

}


void redraw1func()
{
	qenter(REDRAW, 1);
	if(G.f & G_DEBUG) printf("redrawfunc\n");
}

void redraw2func()
{
	
	qenter(REDRAW, 2);
}

void visiblefunc(int dummy)
{
	/* geen winfreeze */
	
}

void inputchangefunc(int state)
{
	int win;
	
	if(state) {	
	#ifdef WIN32
		qenter(INPUTCHANGE, glutGetActiveWindow());
	#else
		qenter(INPUTCHANGE, glutGetWindow());
	#endif
	}
	
	if(G.f & G_DEBUG) printf("inputchangefunc %d %d\n", state, glutGetWindow());
}


void reshapefunc(int x, int y)
{
	bScreen *sc;
	int wx, wy, orx, ory;

	/* alleen voor hoofdwindow */
	if(G.curscreen==0) return;
	
	if(G.f & G_DEBUG) printf("reshapefunc\n");
	
/* 	qenter(RESHAPE, mainwin); */
	qenter(REDRAW, mainwin);

	init_my_mainwin(mainwin);
				
	wx= glutGet(GLUT_WINDOW_WIDTH);
	wy= glutGet(GLUT_WINDOW_HEIGHT);

	orx= glutGet(GLUT_WINDOW_X);
	ory= displaysizey -wy- glutGet(GLUT_WINDOW_Y);

	/* Minimizing on windows causes glut to set the
	 * size to 0,  which means things go bonkers
	 */
	if (wx == 0 || wy == 0) return;

	prefstax= orx;
	prefstay= ory;
	prefsizx= wx;
	prefsizy= wy;
	
	sc= G.main->screen.first;
	while(sc) {
		test_scale_screen(sc);
		sc= sc->id.next;
	}

	testareas();

}

static int mymdown= 0;

void mousefunc(int button, int state, int x, int y)
{
	int but;
	
	if(button==GLUT_LEFT_BUTTON) {
		but= LEFTMOUSE;
		if(U.flag & TWOBUTTONMOUSE) {
			if(G.qual & LR_ALTKEY) but= MIDDLEMOUSE;
		}
		if(state==GLUT_DOWN) qenter(but, 1);
		else qenter(but, 0);

		#ifdef __WIN32
		R.winpop= 1;
		#endif
	}
	else if(button==GLUT_MIDDLE_BUTTON) {
		if(state==GLUT_DOWN) mymdown= 1;
		else mymdown= 0;
		
		if(state==GLUT_DOWN) qenter(MIDDLEMOUSE, 1);
		else qenter(MIDDLEMOUSE, 0);
	}
	else if(button==GLUT_RIGHT_BUTTON) {
		if(state==GLUT_DOWN) qenter(RIGHTMOUSE, 1);
		else qenter(RIGHTMOUSE, 0);
	}
}


void mousemovefunc(int x, int y)
{
	/* ommappen */
	scrmousex= x;
	scrmousey= prefsizy-y;

	qenter(MOUSEX, scrmousex); /* (een te veel?) */
	qenter(MOUSEY, scrmousey);
}

void rawkeyfunc(int key, int val, int y)
{
	#ifdef __BeOS
	key= rawkey_beos(key);
	#endif

	#ifdef WIN32
	/* really silly: windows repeats rawkeys! */
	if (key==212 || key==213 || key==216) {
		static int lastval=-1, lastkey=-1;

		if(lastkey==key && lastval==val) return;

		lastval= val;
		lastkey= key;
	} 
	#endif
	
	if(G.f & G_DEBUG) PRINT3(x, d, d, key, val, y);

	qenter(key, val);
}

void keyboardfunc(uchar key, int x, int y)
{
	if(G.f & G_DEBUG) printf("keyboardfunc: %c %d\n", key, key);
	qenter(KEYBD, key);
}

void do_the_glut_funcs()
{

	glutKeyboardFunc(keyboardfunc);
	glutSpecialFunc(rawkeyfunc);
	glutMouseFunc(mousefunc);
	
	glutMotionFunc(mousemovefunc);
	glutPassiveMotionFunc(mousemovefunc);
	glutVisibilityFunc(visiblefunc);
	glutEntryFunc(inputchangefunc);

}

int mywinopen(int mode, short posx, short posy, short sizex, short sizey)
{
	int win;
	
	glutInitDisplayMode(mode);
	glutInitWindowSize(sizex, sizey);
	glutInitWindowPosition(posx, displaysizey - sizey - posy);

	win= glutCreateWindow("Blender");

	if(win==1) {
		glutDisplayFunc(redraw1func);
		glutReshapeFunc(reshapefunc);
	}
	else {
		glutDisplayFunc(redraw2func);
	}
	
	/* WARN! geen standaard fie */
	if (borderless) glutNoBorder();
	
	/* WARN! geen standaard fie */
	glutDoWorkList();

	#ifdef WIN32
	if (win==1 && fullscreen) glutFullScreen();
	#endif

	/* niet netjes: alleen eerste keer! */
	if(win==1) init_my_mainwin(win);

	/* anders matrices in de war */

	mywinset(win);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ortho2(-0.5, (float)sizex-0.5, -0.5, (float)sizey-0.5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return win;
}


bScreen *addscreen(char *name)		/* gebruik de setprefsize() als je anders dan fullscreen wilt */
{
	/* deze functie zet de variabele G.curscreen
	 * omdat alle hulpfuncties moeten weten welk screen
	 */
	bScreen *sc;
	ScrVert *sv1, *sv2, *sv3, *sv4;
	ScrEdge *se;
	ScrArea *sa;
	short startx, starty, endx, endy;	
	
	sc= G.curscreen= alloc_libblock(&G.main->screen, ID_SCR, name);

	if(prefsizx) {
		startx= prefstax;
		starty= prefstay;
		endx= prefstax+prefsizx-1;
		endy= prefstay+prefsizy-1;
	}
	else {
		prefstax= startx= 0;
		prefstay= starty= 0;
		endx= displaysizex-1;
		endy= displaysizey-1;
		prefsizx= displaysizex;
		prefsizy= displaysizey;
	}

	sc->startx= startx;	sc->starty= starty;
	sc->endx= endx;	sc->endy= endy;
	sc->sizex= sc->endx-sc->startx+1;
	sc->sizey= sc->endy-sc->starty+1;
	
	sc->scene= G.scene;
	
	if(mainwin==0) {
		
		/* #ifndef FREE */
		/* decodekeytab(); */
		/* #endif */
		
		sc->mainwin=mainwin= mywinopen(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH, 
				sc->startx, sc->starty, sc->sizex, sc->sizey);

		do_the_glut_funcs();
		glutIdleFunc(screenmain);

		/* glutInitDisplayMode(GLUT_SINGLE | GLUT_INDEX); */

		glDrawBuffer(GL_FRONT);
		glClearColor(.45, .45, .45, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawBuffer(GL_BACK);
		
		warp_pointer(sc->sizex/2,  sc->sizey/2);
		scrmousex= sc->sizex/2;
		scrmousey= sc->sizey/2;
		
		qenter(REDRAW, 1);
	}
	else sc->mainwin= mainwin;
	
	sv1= addscrvert(&sc->vertbase, 0, 0);
	sv2= addscrvert(&sc->vertbase, 0, sc->endy-sc->starty);
	sv3= addscrvert(&sc->vertbase, sc->sizex-1, sc->sizey-1);
	sv4= addscrvert(&sc->vertbase, sc->sizex-1, 0);
	
	se= addscredge(&sc->edgebase, sv1, sv2);
	se= addscredge(&sc->edgebase, sv2, sv3);
	se= addscredge(&sc->edgebase, sv3, sv4);
	se= addscredge(&sc->edgebase, sv4, sv1);

	sa= addscrarea(&sc->areabase, sv1, sv2, sv3, sv4, HEADERDOWN, SPACE_EMPTY);
	
	G.curscreen= sc;

	return sc;
}

/* niet screen zelf vrijgeven */
void free_screen(bScreen *sc)
{
	ScrArea *sa=0;

	freelistN(&sc->vertbase);
	freelistN(&sc->edgebase);
	
	sa= sc->areabase.first;
	while(sa) {
		del_area(sa);
		if(sa==curarea) curarea= 0;
		sa= sa->next;
	}
	freelistN(&sc->areabase);
	if(G.curscreen==sc) {
		G.curscreen= 0;
		winqueue_break= 1;	/* overal uit queue's gaan */
	}
}

void setscreen(bScreen *sc)
{
	bScreen *sc1;
	ScrArea *sa;
	bWindow *win;
	int firstwin= 0;
	short mval[2];
	
	if(sc->full) {				/* vind de bijhorende full */
		sc1= G.main->screen.first;
		while(sc1) {
			sa= sc1->areabase.first;
			if(sa->full==sc) {
				sc= sc1;
				break;
			}
			sc1= sc1->id.next;
		}
		if(sc1==0) printf("setscreen error\n");
	}

	/* G.curscreen de-activeren */
	if(G.curscreen && G.curscreen!=sc) {

		sa= G.curscreen->areabase.first;
		while(sa) {
			if(sa->win) winclose(sa->win);
			sa->win= 0;
			if(sa->headwin) winclose(sa->headwin);
			sa->headwin= 0;
			
			sa->hq= sa->headqueue; /* queue leeg */
			sa->wq= sa->winqueue;
			
			sa= sa->next;
		}		
	}
	if(G.curscreen != sc) {
		mywinset(sc->mainwin);
	}

	G.curscreen= sc;
	G.scene= sc->scene;
	countall();
	
	/* recalculate winakt */
	getmouseco_sc(mval);

	testareas();
	
	G.curscreen->winakt= 0;
	win= swindowbase.first;
	while(win) {
		if(firstwin==0) firstwin= win->id;
		if(mval[0]>=win->xmin-1 && mval[0]<=win->xmax+2) {
			if(mval[1]>=win->ymin-1 && mval[1]<=win->ymax+2) {
				G.curscreen->winakt= win->id;
				break;
			}
		}
		win= win->next;
	}
	/* als buhv cursor op edge staat */
	if(G.curscreen->winakt==0) G.curscreen->winakt= firstwin;
	
	areawinset(G.curscreen->winakt);	/* zet curarea */
	
	qenter(DRAWEDGES, 1);
	winqueue_break= 1;		/* overal uit de queue's gaan */
	
	curedge= 0;		/* global voor move en join */
}

void area_fullscreen()	/* met curarea */
{
	/* deze funktie toggelt: als area full is wordt de parent weer zichtbaar */
	bScreen *sc, *oldscreen;
	ScrArea *new, *old;
	
	if(curarea->full) {
		sc= curarea->full;	/* de oude screen */
		sc->full= 0;
		
		/* vind oude area */
		old= sc->areabase.first;
		while(old) {
			if(old->full) break;
			old= old->next;
		}
		if(old==0) {error("something wrong in areafullscreen"); return;}
		
		copy_areadata(old, curarea);
		old->full= 0;
		
		free_libblock(&G.main->screen, G.curscreen);
		setscreen(sc);
		
	}
	else {
		/* is er maar 1 area? */
		if(G.curscreen->areabase.first==G.curscreen->areabase.last) return;
		if(curarea->spacetype==SPACE_INFO) return;
		
		G.curscreen->full= 1;
		
		old= curarea;		
		oldscreen= G.curscreen;
		sc= addscreen("temp");		/* deze zet G.curscreen */

		splitarea( (ScrArea *)sc->areabase.first, 'h', 0.99);
		new= sc->areabase.first;
		newspace(new->next, SPACE_INFO);
		
		curarea= old;
		G.curscreen= oldscreen;	/* moet voor setscreen */
		
		/* area kopieeren */
		copy_areadata(new, curarea);
		
		curarea->full= oldscreen;
		new->full= oldscreen;
		new->next->full= oldscreen;
		
		setscreen(sc);
		wich_cursor(new);
		
	}
}

void copy_screen(bScreen *to, bScreen *from)
{
	ScrVert *s1, *s2;
	ScrEdge *se;
	ScrArea *sa;
	ListBase lbase;
	
	/* alles van to vrijgeven */
	free_screen(to);
		
	duplicatelist(&to->vertbase, &from->vertbase);
	duplicatelist(&to->edgebase, &from->edgebase);
	duplicatelist(&to->areabase, &from->areabase);
	
	s1= from->vertbase.first;
	s2= to->vertbase.first;
	while(s1) {
		s1->new= s2;
		s2= s2->next;
		s1= s1->next;
	}
	se= to->edgebase.first;
	while(se) {
		se->v1= se->v1->new;
		se->v2= se->v2->new;
		sortscrvert(&(se->v1), &(se->v2));
		se= se->next;
	}

	sa= to->areabase.first;
	while(sa) {
		sa->v1= sa->v1->new;
		sa->v2= sa->v2->new;
		sa->v3= sa->v3->new;
		sa->v4= sa->v4->new;
		sa->win= 0;
		sa->headwin= 0;
		sa->headqueue= sa->hq= 0;
		sa->winqueue= sa->wq= 0;
		
		duplicatespacelist(&lbase, &sa->spacedata);
		sa->spacedata= lbase;
		
		sa= sa->next;
	}
	
	/* op nul zetten (nodig?) */
	s1= from->vertbase.first;
	while(s1) {
		s1->new= 0;
		s1= s1->next;
	}
}

void duplicate_screen()
{
	bScreen *sc, *oldscreen;
	
	if(G.curscreen->full) return;
	
	/* nieuw screen maken: */

	oldscreen= G.curscreen;
	sc= addscreen(oldscreen->id.name+2);	/* deze zet G.curscreen */
	copy_screen(sc, oldscreen);

	G.curscreen= oldscreen;
	setscreen(sc);

}



/* ************ END SCREENBEHEER ************** */
/* ************  JOIN/SPLIT/MOVE ************** */

void joinarea(ScrArea *sa)
{
	ScrArea *sa2, *san;
	ScrArea *up=0, *down=0, *right=0, *left=0;
	ScrEdge *setest;
	short val=0;
	char str[64];
	
	/* welke edge? */
	if(curedge==0) return;
	
	/* zit edge in area? of anders: welke area */
	sa= test_edge_area(sa, curedge);
	if(sa==0) return;
		
	/* welke edges kunnen ermee? */
	/* vind richtingen met zelfde edge */
	sa2= G.curscreen->areabase.first;
	while(sa2) {
		if(sa2 != sa) {
			setest= findscredge(sa2->v1, sa2->v2);
			if(curedge==setest) right= sa2;
			setest= findscredge(sa2->v2, sa2->v3);
			if(curedge==setest) down= sa2;
			setest= findscredge(sa2->v3, sa2->v4);
			if(curedge==setest) left= sa2;
			setest= findscredge(sa2->v4, sa2->v1);
			if(curedge==setest) up= sa2;
		}
		sa2= sa2->next;
	}
	
	sa2= 0;
	setest= 0;
	
	if(left) val++;
	if(up) val++;
	if(right) val++;
	if(down) val++;
	
	if(val==0) return;
	else if(val==1) {
		if(left) sa2= left;
		else if(up) sa2= up;
		else if(right) sa2= right;
		else if(down) sa2= down;
	}
	
	if(okee("Join")) {
		
		if(sa2) {
			/* nieuwe area is oude sa */
			if(sa2==left) {
				sa->v1= sa2->v1;
				sa->v2= sa2->v2;
				addscredge(&G.curscreen->edgebase, sa->v2, sa->v3);
				addscredge(&G.curscreen->edgebase, sa->v1, sa->v4);
			}
			else if(sa2==up) {
				sa->v2= sa2->v2;
				sa->v3= sa2->v3;
				addscredge(&G.curscreen->edgebase, sa->v1, sa->v2);
				addscredge(&G.curscreen->edgebase, sa->v3, sa->v4);
			}
			else if(sa2==right) {
				sa->v3= sa2->v3;
				sa->v4= sa2->v4;
				addscredge(&G.curscreen->edgebase, sa->v2, sa->v3);
				addscredge(&G.curscreen->edgebase, sa->v1, sa->v4);
			}
			else if(sa2==down) {
				sa->v1= sa2->v1;
				sa->v4= sa2->v4;
				addscredge(&G.curscreen->edgebase, sa->v1, sa->v2);
				addscredge(&G.curscreen->edgebase, sa->v3, sa->v4);
			}
		
			/* edge en area weg */
			/* remlink(&G.curscreen->edgebase, setest); */
			/* freeN(setest); */
			del_area(sa2);
			remlink(&G.curscreen->areabase, sa2);
			if(curarea==sa2) curarea=0;
			freeN(sa2);
			
			removedouble_scredges();
			removenotused_scredges();
			removenotused_scrverts();	/* moet als laatste */
			
			testareas();
			qenter(DRAWEDGES, 1);
				/* test cursor en inputwindow */
			qenter(MOUSEY, -1);		
		}
	}
}

short testsplitpoint(ScrArea *sa, char dir, float fac)
/* return 0: geen split mogelijk */
/* else return (integer) screencoordinaat splitpunt */
{
	short val, x, y;
	
	/* area groot genoeg? */
	val= 0;
	if(sa->v4->vec.x- sa->v1->vec.x <= 2*AREAMINX) return 0;
	if(sa->v2->vec.y- sa->v1->vec.y <= 2*AREAMINY) return 0;

	/* voor zekerheid */
	if(fac<0.0) fac= 0.0;
	if(fac>1.0) fac= 1.0;
	
	if(dir=='h') {
		y= sa->v1->vec.y+ fac*(sa->v2->vec.y- sa->v1->vec.y);
		
		if(sa->v2->vec.y==G.curscreen->sizey-1 && sa->v2->vec.y- y < HEADERY+EDGEWIDTH2) 
			y= sa->v2->vec.y- HEADERY-EDGEWIDTH2;

		else if(sa->v1->vec.y==0 && y- sa->v1->vec.y < HEADERY+EDGEWIDTH2)
			y= sa->v1->vec.y+ HEADERY+EDGEWIDTH2;

		else if(y- sa->v1->vec.y < AREAMINY) y= sa->v1->vec.y+ AREAMINY;
		else if(sa->v2->vec.y- y < AREAMINY) y= sa->v2->vec.y- AREAMINY;
		else y-= (y % AREAGRID);

		return y;
	}
	else {
		x= sa->v1->vec.x+ fac*(sa->v4->vec.x- sa->v1->vec.x);
		if(x- sa->v1->vec.x < AREAMINX) x= sa->v1->vec.x+ AREAMINX;
		else if(sa->v4->vec.x- x < AREAMINX) x= sa->v4->vec.x- AREAMINX;
		else x-= (x % AREAGRID);

		return x;
	}
}

void splitarea(ScrArea *sa, char dir, float fac)
{
	bScreen *sc;
	ScrArea *new;
	ScrVert *sv1, *sv2;
	short split;
	
	if(sa==0) return;
	
	split= testsplitpoint(sa, dir, fac);
	if(split==0) return;
	
	sc= G.curscreen;
	
	areawinset(sa->win);
	
	if(dir=='h') {
		/* nieuwe vertices */
		sv1= addscrvert(&sc->vertbase, sa->v1->vec.x, split);
		
		sv2= addscrvert(&sc->vertbase, sa->v4->vec.x, split);
		
		/* nieuwe edges */
		addscredge(&sc->edgebase, sa->v1, sv1);
		addscredge(&sc->edgebase, sv1, sa->v2);
		addscredge(&sc->edgebase, sa->v3, sv2);
		addscredge(&sc->edgebase, sv2, sa->v4);
		addscredge(&sc->edgebase, sv1, sv2);
		
		/* nieuwe areas: boven */
		new= addscrarea(&sc->areabase, sv1, sa->v2, sa->v3, sv2, sa->headertype, sa->spacetype);
		copy_areadata(new, sa);

		/* area onder */
		sa->v2= sv1;
		sa->v3= sv2;
		
	}
	else {
		/* nieuwe vertices */
		sv1= addscrvert(&sc->vertbase, split, sa->v1->vec.y);
		
		sv2= addscrvert(&sc->vertbase, split, sa->v2->vec.y);
		
		/* nieuwe edges */
		addscredge(&sc->edgebase, sa->v1, sv1);
		addscredge(&sc->edgebase, sv1, sa->v4);
		addscredge(&sc->edgebase, sa->v2, sv2);
		addscredge(&sc->edgebase, sv2, sa->v3);
		addscredge(&sc->edgebase, sv1, sv2);
		
		/* nieuwe areas: links */
		new= addscrarea(&sc->areabase, sa->v1, sa->v2, sv2, sv1, sa->headertype, sa->spacetype);
		copy_areadata(new, sa);

		/* area rechts */
		sa->v1= sv1;		
		sa->v2= sv2;
	}
	
	/* dubbele vertices en edges verwijderen */
	removedouble_scrverts();
	removedouble_scredges();
	removenotused_scredges();
	
	testareas();
}

void editsplitpoint()
{
	ScrArea *sa;
	float fac;
	ushort event;
	short ok= 0, val, split, mval[2], mvalo[2]={-1010, -1010}, col[3];
	char dir;
	
	sa= test_edge_area(curarea, curedge);
	if(sa==0) return;
	
	if(sa->win==0) return;
	if(sa->full) return;
	if(curedge==0) return;
	if(okee("Split")==0) return;	

	if(curedge->v1->vec.x==curedge->v2->vec.x) dir= 'h';
	else dir= 'v';
	
	mywinset(G.curscreen->mainwin);
	glDrawBuffer(GL_FRONT);
	/* hoort al goede matrix te hebben */

	/* rekening houden met grid en minsize */
	while(ok==0) {
		getmouseco_sc(mval);
		
		if(mval[0]!=mvalo[0] || mval[1]!=mvalo[1]) {
			mvalo[0]= mval[0];
			mvalo[1]= mval[1];
			
			if(dir=='h') {
				fac= mval[1]- (sa->v1->vec.y);
				fac/= sa->v2->vec.y- sa->v1->vec.y;
			} else {
				fac= mval[0]- sa->v1->vec.x;
				fac/= sa->v4->vec.x- sa->v1->vec.x;
			}
			
			split= testsplitpoint(sa, dir, fac);
			if(split==0) {
				ok= -1;
			}
			else {

				if(dir=='h') {
					sdrawXORline4(0, sa->totrct.xmin, split, sa->totrct.xmax, split);
					sdrawXORline4(1, sa->totrct.xmin, -1+split, sa->totrct.xmax, -1+split);
				}
				else {
					sdrawXORline4(0, split, sa->totrct.ymin, split, sa->totrct.ymax);
					sdrawXORline4(1, -1+split, sa->totrct.ymin, -1+split, sa->totrct.ymax);
				}
			}
		}
		
		event= extern_qread(&val);
		if(val && event==LEFTMOUSE) {
			if(dir=='h') {
				fac= split- (sa->v1->vec.y);
				fac/= sa->v2->vec.y- sa->v1->vec.y;
			}
			else {
				fac= split- sa->v1->vec.x;
				fac/= sa->v4->vec.x- sa->v1->vec.x;
			}
			ok= 1;
		}
		if(val && event==ESCKEY) {
			ok= -1;
		}

	}

	sdrawXORline4(-1, 0, 0, 0, 0);
	
	if(ok==1) {
		splitarea(sa, dir, fac);
		qenter(DRAWEDGES, 1);
	}
	glDrawBuffer(GL_BACK);
}

void select_connected_scredge(bScreen *sc, ScrEdge *edge)
{
	ScrEdge *se;
	ScrVert *sv;
	int oneselected;
	char dir;
	
	/* select connected, alleen in de juiste richting */
	/* 'dir' is de richting van de EDGE */

	if(edge->v1->vec.x==edge->v2->vec.x) dir= 'v';
	else dir= 'h';

	sv= sc->vertbase.first;
	while(sv) {
		sv->flag= 0;
		sv= sv->next;
	}

	edge->v1->flag= 1;
	edge->v2->flag= 1;

	oneselected= 1;
	while(oneselected) {
		se= sc->edgebase.first;
		oneselected= 0;
		while(se) {
			if(se->v1->flag + se->v2->flag==1) {
				if(dir=='h') if(se->v1->vec.y==se->v2->vec.y) {
					se->v1->flag= se->v2->flag= 1;
					oneselected= 1;
				}
				if(dir=='v') if(se->v1->vec.x==se->v2->vec.x) {
					se->v1->flag= se->v2->flag= 1;
					oneselected= 1;
				}
			}
			se= se->next;
		}
	}
}

void moveareas()
{
	ScrEdge *se;
	ScrVert *v1;
	ScrArea *sa;
	vec2s addvec;
	float PdistVL2Dfl(), vec1[2], vec2[2], vec3[2];
	int dist, mindist= 1<<30;
	ushort event=0;
	short val, split, mval[2], mvalo[2];
	short tel, x1, x2, y1, y2, bigger, smaller, col[3];
	char dir;
	
	if(curarea->full) return;
	
	if(curedge==0 || curedge->border) return;

	if(curedge->v1->vec.x==curedge->v2->vec.x) dir= 'v';
	else dir= 'h';
	
	select_connected_scredge(G.curscreen, curedge);

	/* nu zijn alle vertices met 'flag==1' degene die verplaatst kunnen worden. */
	/* we lopen de areas af en testen vrije ruimte met MINSIZE */
	bigger= smaller= 10000;
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(dir=='h') {	/* als top of down edge select, test hoogte */
			if(sa->v1->flag && sa->v4->flag) {
				if(sa->v2->vec.y==G.curscreen->sizey-1)	/* bovenste edge */
					y1= sa->v2->vec.y - sa->v1->vec.y-HEADERY-EDGEWIDTH2;
				else 
					y1= sa->v2->vec.y - sa->v1->vec.y-AREAMINY;
				if(y1<bigger) bigger= y1;
			}
			else if(sa->v2->flag && sa->v3->flag) {
				if(sa->v1->vec.y==0)	/* onderste edge */
					y1= sa->v2->vec.y - sa->v1->vec.y-HEADERY;
				else
					y1= sa->v2->vec.y - sa->v1->vec.y-AREAMINY;
				if(y1<smaller) smaller= y1;
			}
		}
		else {	/* als left of right edge select, test breedte */
			if(sa->v1->flag && sa->v2->flag) {
				x1= sa->v4->vec.x - sa->v1->vec.x-AREAMINX;
				if(x1<bigger) bigger= x1;
			}
			else if(sa->v3->flag && sa->v4->flag) {
				x1= sa->v4->vec.x - sa->v1->vec.x-AREAMINX;
				if(x1<smaller) smaller= x1;
			}
		}
		sa= sa->next;
	}
	
	mywinset(G.curscreen->mainwin);
	glDrawBuffer(GL_FRONT);
	
	getmouseco_sc(mvalo);
	addvec.x= 0;
	addvec.y= 0;
	
	while(get_mbut()&L_MOUSE) {
		getmouseco_sc(mval);
		
		if(mval[0]!=mvalo[0] || mval[1]!=mvalo[1]) {

			if(dir=='h') {
				addvec.y+= mval[1]-mvalo[1];
				if(addvec.y>bigger) addvec.y= bigger;
				if(addvec.y<-smaller) addvec.y= -smaller;
			}
			else {
				addvec.x+= mval[0]-mvalo[0];
				if(addvec.x>bigger) addvec.x= bigger;
				if(addvec.x<-smaller) addvec.x= -smaller;
			}
			mvalo[0]= mval[0];
			mvalo[1]= mval[1];
			tel= 0;
			
			se= G.curscreen->edgebase.first;
			while(se) {
				if(se->v1->flag && se->v2->flag) {
					/* met areagrid even behelpen, verderop is OK! */
					x1= se->v1->vec.x+addvec.x-(addvec.x % AREAGRID);
					x2= se->v2->vec.x+addvec.x-(addvec.x % AREAGRID);
					y1= se->v1->vec.y+addvec.y-(addvec.y % AREAGRID);
					y2= se->v2->vec.y+addvec.y-(addvec.y % AREAGRID);
					
					sdrawXORline4(tel++, x1,  y1,  x2,  y2);
					sdrawXORline4(tel++, x1+1,  y1+1,  x2+1,  y2+1);
					tel &= 3;
				}
				se= se->next;
			}
		}
		else usleep(2);
		
		event= extern_qread(&val);
		if(val) {
			if(event==ESCKEY) break;
			if(event==LEFTMOUSE) break;
			if(event==SPACEKEY) break;
		}
	}
	/* wissen */
	sdrawXORline4(-1, 0,  0,  0,  0);

	v1= G.curscreen->vertbase.first;
	while(v1) {
		if(v1->flag && event!=ESCKEY) {
		
			/* zo is AREAGRID netjes */
			if(addvec.x && v1->vec.x>0 && v1->vec.x<G.curscreen->sizex-1) {
				v1->vec.x+= addvec.x;
				if(addvec.x != bigger && addvec.x != -smaller) v1->vec.x-= (v1->vec.x % AREAGRID);
			}
			if(addvec.y && v1->vec.y>0 && v1->vec.y<G.curscreen->sizey-1) {
				v1->vec.y+= addvec.y;
				
				/* with these lines commented out you can pull the top bar exact to the screen border. */
				/* if(addvec.y != bigger && addvec.y != -smaller) { */
					v1->vec.y+= AREAGRID-1;
					v1->vec.y-= (v1->vec.y % AREAGRID);
				
				/* } */
			}
		}
		v1->flag= 0;
		v1= v1->next;
	}
	
	if(event!=ESCKEY) {
		removedouble_scrverts();
		removedouble_scredges();
		testareas();
	}
	
	glDrawBuffer(GL_BACK);
	qenter(DRAWEDGES, 1);
}

void scrollheader()
{
	short mval[2], mvalo[2];
	
	if(curarea->headbutlen<curarea->winx) {
		curarea->headbutofs= 0;
	}
	else if(curarea->headbutofs+curarea->winx > curarea->headbutlen) {
		curarea->headbutofs= curarea->headbutlen-curarea->winx;
	}

	getmouseco_sc(mvalo);

	while(get_mbut()&M_MOUSE) {
		getmouseco_sc(mval);
		if(mval[0]!=mvalo[0]) {
			curarea->headbutofs-= (mval[0]-mvalo[0]);

			if(curarea->headbutlen-curarea->winx < curarea->headbutofs) curarea->headbutofs= curarea->headbutlen-curarea->winx;
			if(curarea->headbutofs<0) curarea->headbutofs= 0;

			defheadchange();
		
			if(curarea->headdraw) curarea->headdraw(); 

			screen_swapbuffers();
			
			mvalo[0]= mval[0];
		}
		else usleep(2);
	}
}

int select_area(int spacetype)
{
	/* vanuit editroutines aanroepen, als er meer area's
	 * zijn van type 'spacetype' kan er een area aangegeven worden
	 */
	ScrArea *sa, *sact;
	bWindow *win;
	int tot=0;
	ushort event;
	short val, mval[2];
	
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->spacetype==spacetype) {
			sact= sa;
			tot++;
		}
		sa= sa->next;
	} 
	
	if(tot==0) {
		error("Can't do this! Open correct window");
		return 0;
	}
	
	if(tot==1) {
		if(curarea!=sact) areawinset(sact->win);
		return 1;
	}
	else if(tot>1) {
		tempcursor(GLUT_CURSOR_HELP);
		while(1) {
			event= extern_qread(&val);
			
			if(val) {
				if(event==ESCKEY) break;
				if(event==LEFTMOUSE) break;
				if(event==SPACEKEY) break;
			}
		}
		tempcursor(CURSOR_STD);
		
		/* recalculate winakt */
		getmouseco_sc(mval);
	
		G.curscreen->winakt= 0;
		win= swindowbase.first;
		while(win) {
			if(mval[0]>=win->xmin-1 && mval[0]<=win->xmax+2) {
				if(mval[1]>=win->ymin-1 && mval[1]<=win->ymax+2) {
					G.curscreen->winakt= win->id;
					break;
				}
			}
			win= win->next;
		}
		
		if(event==LEFTMOUSE) {
			sa= areawinar[G.curscreen->winakt];
			if(sa->spacetype==spacetype) areawinset(G.curscreen->winakt);
			else {
				error("wrong window");
				return 0;
			}
		}
	}
	
	if(event==LEFTMOUSE) return 1;
	else return 0;
}

/* ************  END JOIN/SPLIT/MOVE ************** */
/* **************** DRAW SCREENEDGES ***************** */

#define EDGE_EXTEND 3

void drawedge(short x1, short y1, short x2, short y2)
{
	int a, dir;
	vec2f v1, v2;

	v1.x= x1;
	v1.y= y1;
	v2.x= x2;
	v2.y= y2;
	
	if(v1.x== v2.x) {		/* vertikaal */
		dir= v1.y-v2.y;
		if(dir>0) dir= 1; else dir= -1;
		
		if (dir==-1) {
			if (v1.y==0) v1.y-= EDGE_EXTEND;
			if (v2.y==G.curscreen->sizey) v2.y+= EDGE_EXTEND;
		} else {
			if (v2.y==0) v2.y-= EDGE_EXTEND;
			if (v1.y==G.curscreen->sizey) v1.y+= EDGE_EXTEND;
		}
		
		v1.y-= dir*EDGEWIDTH2;
		v2.y+= dir*EDGEWIDTH2;
		
		v1.x+= EDGEWIDTH2;
		v2.x+= EDGEWIDTH2;
		
		for(a=0; a<EDGEWIDTH; a++) {
			cpack(edcol[a]);
			LINE2F(&v1, &v2);
			v1.x--; v2.x--;
					/* en dit voor de afgeronde tuitjes */
			if(a<EDGEWIDTH2) { v1.y+= dir; v2.y-=dir;}
			else { v1.y-= dir; v2.y+=dir;}
		}
	}
	else {					/* horizontaal */
		dir= v1.x-v2.x;
		if(dir>0) dir= 1; else dir= -1;
		
		if (dir==-1) {
			if (v1.x==0) v1.x-= EDGE_EXTEND;
			if (v2.x==G.curscreen->sizex) v2.x+= EDGE_EXTEND;
		} else {
			if (v2.x==0) v2.x-= EDGE_EXTEND;
			if (v1.x==G.curscreen->sizex) v1.x+= EDGE_EXTEND;
		}

		v1.x-= dir*EDGEWIDTH2;
		v2.x+= dir*EDGEWIDTH2;
		
		v1.y-= EDGEWIDTH2;
		v2.y-= EDGEWIDTH2;
		for(a=0; a<EDGEWIDTH; a++) {
			cpack(edcol[a]);
			LINE2F(&v1, &v2);
			v1.y++; v2.y++;
					/* en dit voor de afgeronde tuitjes */
			if(a<EDGEWIDTH2) { v1.x+= dir; v2.x-=dir;}
			else { v1.x-= dir; v2.x+=dir;}
		}
	}
	
}

void drawscredge(ScrEdge *se)
{
	bScreen *sc;
	vec2s *v1, *v2;
	
	sc= G.curscreen;
	
	v1= &(se->v1->vec);
	v2= &(se->v2->vec);
	
	/* borders screen niet tekenen */
	/* vanwege resolutie verschillen (PC/SGI files) de linit een
	 * beetje afronden?
	 */
	se->border= 1;
	if(v1->x<=1 && v2->x<=1) return;
	if(v1->x>=sc->sizex-2 && v2->x>=sc->sizex-2) return;
	if(v1->y<=1 && v2->y<=1) return;
	if(v1->y>=sc->sizey-2 && v2->y>=sc->sizey-2) return;
	se->border= 0;

	drawedge(v1->x, v1->y, v2->x, v2->y);
}

void drawscreen()
{
	static int firsttime= 1;
	ScrEdge *se;
	
	mywinset(G.curscreen->mainwin);
	ortho2(-0.5, (float)G.curscreen->sizex-0.5, -0.5, (float)G.curscreen->sizey-0.5);

	/* edges in mainwin */
	#if !defined(__BeOS ) && !defined(WIN32) && !defined(PPC) && !defined(MESA31)
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif
	
	se= G.curscreen->edgebase.first;
	while(se) {
		drawscredge(se);
		se= se->next;
	}
	
	glDrawBuffer(GL_BACK);

	#if defined(__BeOS) || defined(WIN32) || defined(PPC) || defined(MESA31)
	/* can't do front+back */
	glDrawBuffer(GL_FRONT);
	se= G.curscreen->edgebase.first;
	while(se) {
		drawscredge(se);
		se= se->next;
	}
	
	glDrawBuffer(GL_BACK);
	#endif

	/* for some reason the drawedges is called very late, but then the graphics is initialized correctly! */
	firsttime= 0;
}

/* ********************************* */

bScreen *default_foursplit() 
{
	bScreen *sc;
	ScrArea *sa;
	View3D *vd;
	int tel;
	
	sc= addscreen("screen");
	
	splitarea( (ScrArea *)sc->areabase.first, 'h', 0.25);
	splitarea( (ScrArea *)sc->areabase.last, 'h', 0.49);
	splitarea( (ScrArea *)sc->areabase.last, 'h', 0.99);

	sa= sc->areabase.last;
	newspace(sa, SPACE_INFO);
	/* sa->headertype= NOHEADER; */

			/* alle nieuwe areas komen op einde lijst! */
	sa= sc->areabase.first;
	splitarea( sa->next, 'v', 0.5);
	sa= sc->areabase.first;
	splitarea( sa->next, 'v', 0.5);
	
	sa= sc->areabase.first;
	sa->headertype= HEADERTOP;
	newspace(sa, SPACE_BUTS);
	sa= sa->next;
	
	/* zet de view3d's en geef ze de juiste aanzicht */
	tel= 1;
	while(sa) {
		if(sa->spacetype==0) {
			newspace(sa, SPACE_VIEW3D);
			vd= sa->spacedata.first;
			vd->persp= 0;

			if(tel==1) {
				vd->view= 1;
				vd->viewquat[0]= fcos(M_PI/4.0);
				vd->viewquat[1]= -fsin(M_PI/4.0);
			}
			else if(tel==2) {
				vd->view= 3;
				vd->viewquat[0]= 0.5;
				vd->viewquat[1]= -0.5;
				vd->viewquat[2]= -0.5;
				vd->viewquat[3]= -0.5;
			}
			else if(tel==3) {
				vd->view= 7;
			}
			else vd->persp= 1;
			
			tel++;
		}
		sa= sa->next;
	}
	
	testareas();
	
	return sc;
}

bScreen *default_twosplit() 
{
	bScreen *sc;
	ScrArea *sa;

	sc= addscreen("screen");

	splitarea( (ScrArea *)sc->areabase.first, 'h', 0.99);
	sa= sc->areabase.first;
	newspace(sa, SPACE_VIEW3D);
	newspace(sa->next, SPACE_INFO);
	
	return sc;
}

void initscreen()
{
	/* opent en initialiseert */
	getdisplaysize();
	
	default_twosplit();
}




