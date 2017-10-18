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


/* screen.h    dec 93 jan 94 */

/*
 * 
 *	LET OP: util.h (ListBase) en graphics.h (vec2s) ook nodig
 * 
 */

#ifndef SCREEN_H
#define SCREEN_H


#define MAXWIN		128

/* Als je EDGEWIDTH verandert, ook globale array edcol[] goedzetten */
#define EDGEWIDTH	5
#define EDGEWIDTH2	(2)
#define AREAGRID	4
#define AREAMINX	32
#define HEADERY		21
#define AREAMINY	(HEADERY+EDGEWIDTH)

#define NOHEADER	0
#define HEADERDOWN	1
#define HEADERTOP	2

/* sa->win_swap */
#define WIN_FRONT_OK	1
#define WIN_BACK_OK		2
#define WIN_EQUAL		3

#define L_SCROLL 1			/* left scrollbar */
#define R_SCROLL 2
#define VERT_SCROLL 3
#define T_SCROLL 4
#define B_SCROLL 8
#define HOR_SCROLL 12

#define SPACE_EMPTY		0	/* deze getallen moeten kleiner dan 100 zijn ivm buttoncodes */
#define SPACE_VIEW3D	1
#define SPACE_IPO		2
#define SPACE_OOPS		3
#define SPACE_BUTS		4
#define SPACE_FILE		5
#define SPACE_IMAGE		6
#define SPACE_INFO		7
#define SPACE_SEQ		8
#define SPACE_TEXT		9
#define SPACE_IMASEL	10
#define SPACE_PAINT		11

#define MAXQUEUE 256

/* queue events: naar blendef.h */

typedef struct bScreen {
	ID id;
	ListBase vertbase, edgebase, areabase;
	Scene *scene;
	short startx, endx, starty, endy;	/* framebuffer coords */
	short sizex, sizey;
	short scenenr, screennr;			/* alleen voor pupmenu */
	short full, rt;
	short mainwin, winakt;
} bScreen;


typedef struct ScrVert {
	struct ScrVert *next, *prev, *new;
	vec2s vec;
	int flag;
} ScrVert;

typedef struct ScrEdge {
	struct ScrEdge *next, *prev;
	ScrVert *v1, *v2;
	short border;			/* 1 als op rand screen */
	short flag;
	int pad;
} ScrEdge;

typedef struct ScrArea {
	struct ScrArea *next, *prev;
	ScrVert *v1, *v2, *v3, *v4;
	bScreen *full;			/* als area==full, dit is de parent */
	short *headqueue, *hq, *winqueue, *wq;
	float winmat[4][4];
	rcti totrct, headrct, winrct;

	short headwin, win;
	short headertype;		/* 0=niets, 1= down, 2= up */
	char spacetype, butspacetype;	/* SPACE_...  */
	short winx, winy;		/* size */
	char head_swap, head_equal;
	char win_swap, win_equal;
	
	short headbutlen, headbutofs;
	short cursor, rt;
	
	void (*headchange)(), (*winchange)();
	void (*headdraw)(void), (*windraw)(void);
	/* opmerking: fuctiepointers met types binnen de halen geeft problemen met SDNA */
	void (*headqread)(), (*winqread)();
	
	ListBase spacedata;
} ScrArea;

/* ********** MYWINDOW ******* */
#
#
typedef struct bWindow {
	struct bWindow *next, *prev;
	int id, parent_id;

	int xmin, xmax, ymin, ymax;
	float viewmat[4][4], winmat[4][4];
	
} bWindow;


#define winset		mywinset
#define winget		mywinget
#define ortho		myortho
#define ortho2		myortho2
#define window(a, b, c, d, e, f)		mywindow(a, b, c, d, e, f)
#define winclose	mywinclose
#define loadmatrix	myloadmatrix
#define getmatrix	mygetmatrix
#define multmatrix	mymultmatrix
#define getsize		mygetsize
#define getorigin	mygetorigin

extern ListBase swindowbase;	/* uit mywindow.c */

/* ********** END MYWINDOW ******* */

/* GLOBALS   (uit screen.c) */

extern ScrArea *curarea;
extern int displaysizex, displaysizey;

/* mywindow.c */
extern		short get_qual(void);
extern		short get_mbut(void);

void myglClearColor(float a, float b, float c, float d);
extern    void custom_cursor(char *data1, char *data2, int size);
extern    void drawmode(int mode);
extern    int *fmfindfont(char *name);
extern    int fmgetstrwidth(void *handle, char *str);
extern    void fminit();
extern    void fmprstr(char *str);
extern    void fmrotatepagematrix(float degrees);
extern    int *fmscalefont();
extern    void fmsetfont(void *type);
extern    short getbutton(int val);
extern    void getmouse(short *mval);
extern    short getqual();
extern    int give_new_winid();
extern    void init_my_mainwin(int win);
extern    void loadmatrix_win(float mat[][4], int swin);
extern    void myContextSetup();
extern    void myCopySubBuffer(int x, int y, int sx, int sy);
extern    void myXcolor(int col);
extern    void myXdrawline(int x1, int y1, int x2, int y2);
extern    void myXrect(int x1, int y1, int x2, int y2);
extern    void myXrectf(int x1, int y1, int x2, int y2);
extern    void my_get_frontbuffer_image(int x, int y, int sx, int sy);
extern    void my_put_frontbuffer_image();
extern    void mygetmatrix(float mat[][4]);
extern    void mygetorigin(int *x, int *y);
extern    void mygetsingmatrix(float mat[][4]);
extern    void mygetsize(int *x, int *y);
extern    void mygetsuborigin(int *x, int *y);
extern    void myloadmatrix(float mat[][4]);
extern    void mymultmatrix(float mat[][4]);
extern    void myortho(float x1, float x2, float y1, float y2, float n, float f);
extern    void myortho2(float x1, float x2, float y1, float y2);
extern    void myswapbuffers(void);
extern    int myswinopen(int winid, int xmin, int xmax, int ymin, int ymax);
extern    void mywinclose(int winid);
extern    void mywindow(float x1, float x2, float y1, float y2, float n, float f);
extern    int mywinexist(int wid);
extern    int mywinget(void);
extern    void mywinposition(int swin, int xmin, int xmax, int ymin, int ymax); /* let op: andere syntax */
extern    void mywinset(int wid);
extern    void overdrawdump(void);
extern    void screendump(void);
extern    void sdrawXORline(int x0, int y0, int x1, int y1);
extern    void sdrawXORline4(int nr, int x0, int y0, int x1, int y1);
extern    void setlinestyle(int nr);
extern    void setvaluator( short dev, short a, short b, short c);
extern    void write_screendump(char *name);


/* screen.c */
extern      void addafterqueue(short win, ushort event, short val);
extern      void addqueue(short win, ushort event, short val);
extern      bScreen *addscreen(char *name);		/* gebruik de setprefsize(); als je anders dan fullscreen wilt */
extern      int afterqtest();
extern      void append_afterqueue();
extern      void area_fullscreen();	/* met curarea */
extern      void areawinset(short win);
extern      void calc_arearcts(ScrArea *sa);
extern      void closeareawin(ScrArea *sa);
extern      void closeheadwin(ScrArea *sa);
extern      ScrArea *closest_bigger_area();
extern      void copy_areadata(ScrArea *sa1, ScrArea *sa2);
extern      void copy_screen(bScreen *to, bScreen *from);
extern      void decodekeytab();
extern      bScreen *default_foursplit(); 
extern      bScreen *default_twosplit(); 
extern      void defheadchange();
extern      void defheaddraw();
extern      void defheadqread(ScrArea *sa);
extern      void defwinchange();
extern      void defwindraw();
extern      void defwinmat();
extern      void defwinqread(ScrArea *sa);
extern      void del_area(ScrArea *sa);
extern      void do_the_glut_funcs();
extern      void drawedge(short x1, short y1, short x2, short y2);
extern      void drawscredge(ScrEdge *se);
extern      void drawscreen();
extern      void duplicate_screen();
extern      void editsplitpoint();
extern      short ext_qtest();
/* extern      ushort special_qread(short *val); */
extern      ushort extern_qread(short *val);
extern      ScrArea *findcurarea();
extern      ScrEdge *findscredge(ScrVert *v1, ScrVert *v2);
extern      void free_screen(bScreen *sc);
extern      void getdisplaysize();
extern      void getmouseco_areawin(short *mval);		/* interne area coordinaten */
extern      void getmouseco_headwin(short *mval);		/* interne area coordinaten */
extern      void getmouseco_sc(short *mval);		/* screen coordinaten */
extern      void header_back_to_front(ScrArea *sa);
extern      void header_front_to_back(ScrArea *sa);
extern      void headerbox(int selcol, int width);
extern      void headmenu(ScrArea *sa);
extern      void init_screen_cursors();
extern      void initscreen();
extern      void inputchangefunc(int state);
extern      int is_allowed_to_change_screen(bScreen *new);
extern      short isqueued( short dev );
extern      void joinarea(ScrArea *sa);
extern      void keyboardfunc(uchar key, int x, int y);
extern      void markdirty_all(void);
extern      void mousefunc(int button, int state, int x, int y);
extern      void mousemovefunc(int x, int y);
extern      void moveareas(void);
extern      ushort myqtest(void);
extern      int mywinopen(int mode, short posx, short posy, short sizex, short sizey);
extern      void openareawin(ScrArea *sa);
extern      void openheadwin(ScrArea *sa);
extern      void qenter(ushort event, short val);
extern      ushort qread(short *val);
extern      void qreset(void);
extern      ushort qtest(void);
extern      void rawkeyfunc(int key, int val, int y);
extern      void redraw1func(void);
extern      void redraw2func(void);
extern      void remake_qual(void);
extern      void removedouble_scredges(void);
extern      void removedouble_scrverts(void);
extern      void removenotused_scredges(void);
extern      void removenotused_scrverts(void);
extern      void reset_autosave(void);
extern      void reshapefunc(int x, int y);
extern      void rgbmul(uint *col, int mul);
extern      ushort screen_qread(short *val);
extern      void screen_swapbuffers(void);
extern      void screenmain(void);
extern      void scrollheader(void);
extern      int select_area(int spacetype);
extern      void set_cursonedge(short mx, short my);
extern      void setcursor_space(int spacetype, short cur);
extern      void setdisplaysize(int ex, int ey);
extern      void setprefsize(int stax, int stay, int sizx, int sizy);
extern      void setscreen(bScreen *sc);
extern      void splitarea(ScrArea *sa, char dir, float fac);
extern      void start_autosave(int val);
extern      void tempcursor(int curs);
extern      ScrArea *test_edge_area(ScrArea *sa, ScrEdge *se);
extern      void test_scale_screen(bScreen *sc);
extern      void testareas(void);
extern      short testsplitpoint(ScrArea *sa, char dir, float fac);
extern      void visiblefunc(int dummy);
extern      void waitcursor(int val);
extern      void wich_cursor(ScrArea *sa);
extern      void window_front_to_back(ScrArea *sa);

	/* space.c */
extern       void allqueue(ushort event, short val);
extern       void allspace(ushort event, short val);
extern       void changebutspace();
extern       void changeview2d();
extern       void changeview3d();
extern       void copy_view3d_lock(short val);
extern       void drawemptyspace();
extern       void drawinfospace();
extern       void duplicatespacelist(ListBase *lb1, ListBase *lb2);
extern       void extern_set_butspace(int fkey);
extern       void force_draw();
extern       void freespacelist(ListBase *lb);
extern       void handle_view3d_lock();
extern       void init_butspace(ScrArea *sa);
extern       void init_filespace(ScrArea *sa);
extern       void init_imagespace(ScrArea *sa);
extern       void init_oopsspace(ScrArea *sa);
extern       void init_seqspace(ScrArea *sa);
extern       void init_v2d_oops(View2D *v2d);
extern       void initipo(ScrArea *sa);
extern       void initview3d(ScrArea *sa);
extern       void newspace(ScrArea *sa, int type);
extern       void set_func_space(ScrArea *sa);
extern       void set_rects_butspace(SpaceButs *buts);
extern       void winqread3d(ushort event, short val);
extern       void winqreadbutspace(ushort event, short val);
extern       void winqreadimagespace(ushort event, short val);
extern       void winqreadinfospace(ushort event, short val);
extern       void winqreadipo(ushort event, short val);
extern       void winqreadoopsspace(ushort event, short val);
extern       void winqreadsequence(ushort event, short val);

#endif /* SCREEN_H */

