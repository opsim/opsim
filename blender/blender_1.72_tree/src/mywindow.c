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



/* screen.c  juli 94 feb 98		GRAPHICS
 * 
 * vervanging voor een aantal fie's zoals swinopen, winset,  (zie onder)
 * dit alles omdat GL en X te traag zijn
 * 
 * feb: Opengl en toch maar naar X!
 * 
 */

/*			  let op: winid's beginnen met 4, eerste 3 voor GL! */


#include "blender.h"
#include "screen.h"
#include "graphics.h"

int swincount= 4, curswin=0;
bWindow *swinarray [MAXWIN]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bWindow mainwindow;
ListBase swindowbase= {0, 0};


void mygetsize(int *x, int *y)
{
	bWindow *win;
	
	if(curswin<4) {
		*x= glutGet(GLUT_WINDOW_WIDTH);
		*y= glutGet(GLUT_WINDOW_HEIGHT);
		return;
	}
	
	win= swinarray[curswin];
	if(win==0) return;

	*x= win->xmax-win->xmin+1;
	*y= win->ymax-win->ymin+1;
}

void mygetorigin(int *x, int *y)
{
	bWindow *win;
	
	*x= glutGet(GLUT_WINDOW_X);
	*y= displaysizey -glutGet(GLUT_WINDOW_HEIGHT)- glutGet(GLUT_WINDOW_Y);

	if(curswin>=4) {
		win= swinarray[curswin];
		if(win) {
			*x+= win->xmin;
			*y+= win->ymin;
		}
	}
}

void mygetsuborigin(int *x, int *y)
{
	bWindow *win;
	
	if(curswin==1) {
		*x= *y= 0;	
	}
	else {
		win= swinarray[curswin];
		if(win==0) return;
		
		*x= win->xmin;
		*y= win->ymin;
	}
}

void myloadmatrix(float mat[][4])
{
	bWindow *win;
	GLint mode;

	glLoadMatrixf(mat);

	win= swinarray[curswin];
	if(win==0) return;
	
	glGetIntegerv(GL_MATRIX_MODE, &mode);
	
	if(mode==GL_MODELVIEW) Mat4CpyMat4(win->viewmat, mat);
	else Mat4CpyMat4(win->winmat, mat);
}

void loadmatrix_win(float mat[][4], int swin)
{
	bWindow *win;
	GLint mode;

	win= swinarray[swin];
	if(win==0) return;
	
	glGetIntegerv(GL_MATRIX_MODE, &mode);

	if(mode==GL_MODELVIEW) Mat4CpyMat4(win->viewmat, mat);
	else Mat4CpyMat4(win->winmat, mat);
}

void mygetmatrix(float mat[][4])
{
	bWindow *win;
	GLint mode;

	win= swinarray[curswin];
	if(win==0) return;
	
	glGetIntegerv(GL_MATRIX_MODE, &mode);

	if(mode==GL_MODELVIEW) Mat4CpyMat4(mat, win->viewmat);
	else Mat4CpyMat4(mat, win->winmat);
}

void mymultmatrixold(float mat[][4])
{
	bWindow *win;
	float tmat[4][4];

	win= swinarray[curswin];
	if(win==0) return;
	
	Mat4MulMat4(tmat, mat, win->viewmat);
	Mat4CpyMat4(win->viewmat, tmat);
	glLoadMatrixf(tmat);

}

void mymultmatrix(float mat[][4])
{
	bWindow *win;
	float tmat[4][4];

	glMultMatrixf(mat);
	
	win= swinarray[curswin];
	if(win==0) return;
	
	glGetFloatv(GL_MODELVIEW_MATRIX, (float *)win->viewmat);

}

void mygetsingmatrix(float mat[][4])
{
	bWindow *win;
	float matview[4][4], matproj[4][4];
	int mode;

	win= swinarray[curswin];
	if(win==0) {
		glGetFloatv(GL_PROJECTION_MATRIX, (float *)matproj);
		glGetFloatv(GL_MODELVIEW_MATRIX, (float *)matview);
		Mat4MulMat4(mat, matview, matproj);
	}
	else {
		Mat4MulMat4(mat, win->viewmat, win->winmat);
	}
}

int mywinnewid()
{
	int wid= 0, a= 4;

	while(wid==0 && a<MAXWIN) {
		if(swinarray[a]==0) wid= a;
		a++;
	}

	return wid;
}

/* deze winset en winget gaan over de aktieve output window */
/* de window waar de muis staat is G.curscreen->winakt */

int mywinget()
{
	return curswin;
}


void mywinset(int wid)
{
	bWindow *win;
	
	if(wid>1 && wid<4) {		/* render window */

		glutSetWindow(wid);
		curswin= wid;

		return;
	}
	win= swinarray[wid];
	if(win==0) {
		printf("winset %d: doesn't exist\n", wid);
		return;
	}

	glutSetWindow(win->parent_id);
	
	if(win->parent_id == wid) {	/* main window */

		glViewport(0,  0, ( win->xmax-win->xmin)+1, ( win->ymax-win->ymin)+1);
		glScissor(0,  0, ( win->xmax-win->xmin)+1, ( win->ymax-win->ymin)+1);
	}
	else {
		glViewport(win->xmin,  win->ymin, ( win->xmax)-(win->xmin)+1, ( win->ymax)-( win->ymin)+1);
		glScissor(win->xmin,  win->ymin, ( win->xmax)-(win->xmin)+1, ( win->ymax)-( win->ymin)+1);
	}
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(win->winmat);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(win->viewmat);

	curswin= wid;
}

int mywinexist(int wid)
{
	return (int) swinarray[wid];
}

int myswinopen(int winid, int xmin, int xmax, int ymin, int ymax)
{
	static int firsttime= 1;
	bWindow *win;
	
	win= callocN(sizeof(bWindow), "winopen");
	addtail(&swindowbase, win);

	swincount++;
	
	if(swincount>=MAXWIN) {
		printf("too many windows\n");
		return 0;
	}
	
	win->parent_id= winid;
	win->id= mywinnewid();
	win->xmin= xmin;
	win->ymin= ymin;
	win->xmax= xmax;
	win->ymax= ymax;
	
	swinarray[win->id]= win;
	
	Mat4One(win->viewmat);
	Mat4One(win->winmat);
	
	mywinset(win->id);

	return win->id;
}

void mywinclose(int winid)
{
	bWindow *win;
	
	if(winid && winid<4) {
		glutDestroyWindow(winid);
		return;
	}

	win= swinarray[winid];
	if(win==0) {
		printf("error: close window %d, doesn't exist\n", winid);
		return;
	}
	remlink(&swindowbase, win);
	freeN(win);
	swinarray[winid]= 0;
	
	if(curswin==winid) curswin= 0;

	swincount--;
}

void mywinposition(int swin, int xmin, int xmax, int ymin, int ymax) /* let op: andere syntax */
{
	bWindow *win;
	
	win= swinarray[swin];
	if(win==0) return;
	
	win->xmin= xmin;
	win->ymin= ymin;
	win->xmax= xmax;
	win->ymax= ymax;
	
}


void myortho2(float x1, float x2, float y1, float y2)
{
	bWindow *win;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(x1, x2, y1, y2);

	win= swinarray[curswin];
	if(win) glGetFloatv(GL_PROJECTION_MATRIX, (float *)win->winmat);

	glMatrixMode(GL_MODELVIEW);
}

void myortho(float x1, float x2, float y1, float y2, float n, float f)
{
	bWindow *win;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(x1, x2, y1, y2, n, f);

	win= swinarray[curswin];
	if(win) glGetFloatv(GL_PROJECTION_MATRIX, (float *)win->winmat);
	
	glMatrixMode(GL_MODELVIEW);
}

void mywindow(float x1, float x2, float y1, float y2, float n, float f)
{
	bWindow *win;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(x1, x2, y1, y2, n, f);

	win= swinarray[curswin];
	if(win) glGetFloatv(GL_PROJECTION_MATRIX, (float *)win->winmat);
	
	glMatrixMode(GL_MODELVIEW);
}

void init_my_mainwin(int win)
{
	int orx, ory, sizex, sizey, endx, endy;

	glEnable(GL_SCISSOR_TEST);
	
	#if !defined(__BeOS) && !defined (__WIN32)
	glutSetWindow(win);
	#endif
	
	sizex= glutGet(GLUT_WINDOW_WIDTH);
	sizey= glutGet(GLUT_WINDOW_HEIGHT);

	orx= glutGet(GLUT_WINDOW_X);
	ory= displaysizey -sizey- glutGet(GLUT_WINDOW_Y);

	endx= orx+sizex-1;
	endy= ory+sizey-1;

	mainwindow.xmin= orx;
	mainwindow.ymin= ory;
	mainwindow.xmax= endx;
	mainwindow.ymax= endy;
	
	ortho2(-0.5, (float)sizex-0.5, -0.5, (float)sizey-0.5);
	glLoadIdentity();
		
	glGetFloatv(GL_PROJECTION_MATRIX, (float *)mainwindow.winmat);
	glGetFloatv(GL_MODELVIEW_MATRIX, (float *)mainwindow.viewmat);
	
	swinarray[win]= &mainwindow;
	mainwindow.parent_id= win;

	curswin= win;
}



/* ********** END MY WINDOW ************** */


void myswapbuffers()
{
	ScrArea *sa;
	
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->win_swap==WIN_BACK_OK) sa->win_swap= WIN_FRONT_OK;
		if(sa->head_swap==WIN_BACK_OK) sa->head_swap= WIN_FRONT_OK;
		
		sa= sa->next;
	}

	glutSwapBuffers();
}


/* ************************** FONTS ************************ */

void *fmtype= 0;

void fmsetfont(void *type)
{
	fmtype= type;
}

void fmprstr(char *str)
{
	glutBitmapStr((void *)fmtype, str);
}

void fmoutchar(void *font, char c)
{
	glutBitmapCharacter((void *)fmtype, c);
}

int fmstrwidth (char *str)
{
	int i, len, retval= 0;
	
	if (!fmtype) return 0;

	len = strlen(str);
	for (i = 0; i < len; i++) {
		retval+= glutBitmapWidth(fmtype, str[i]);
	}
	return retval;	
}

int fmgetstrwidth(void *handle, char *str)
{
	int i, len, retval= 0;
	
	fmsetfont(handle);
	len = (int) strlen(str);
	for (i = 0; i < len; i++) {
		retval+= glutBitmapWidth(fmtype, str[i]);
	}
	return retval;
}

void fminit()
{
	
}

int *fmfindfont(char *name)
{
	return 0;
}

int *fmscalefont()
{
	return 0;
}

void fmrotatepagematrix(float degrees)
{

}


/* *********************** PATTERNS ENZO ***************** */


void setlinestyle(int nr)
{
	
	if(nr==0) {
		glDisable(GL_LINE_STIPPLE);
	}
	else {
		
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(nr, 0xAAAA);
	}
	
}



/* ***************************************************** */
/* ***************************************************** */


#if defined(__BeOS) || defined(__WIN32)

static void *ximage=0;
static int xdraw=0;	/* wordt maar 1 keer gedaan, voor mainwin */
static int ov_x, ov_y, ov_sx, ov_sy;

short get_qual(void)
{
	int retval=0, qual= glutGetQual();

	if(qual & GLUT_ACTIVE_SHIFT) retval|= LR_SHIFTKEY;
	if(qual & GLUT_ACTIVE_CTRL) retval |= LR_CTRLKEY;
	if(qual & GLUT_ACTIVE_ALT) retval  |= LR_ALTKEY;
	
	return retval;
}

short get_mbut(void)
{
	int mval= glutGetMouseButton();
	int qual= glutGetQual();
	short ret= 0;
	
	if (mval & 1) ret |= L_MOUSE;
	if (mval & 4) ret |= M_MOUSE;
	if (mval & 2) ret |= R_MOUSE;
	
	if ((U.flag & TWOBUTTONMOUSE) && (qual & GLUT_ACTIVE_ALT)) {
		if (ret & L_MOUSE) {
			ret &= ~L_MOUSE;
			ret |= M_MOUSE;
		}
	}

	return ret;
}

void getmouse(short *mval)
{
	glutGetMouse(mval);	/* returns windowcos */
	mval[0]+= G.curscreen->startx;
	mval[1]=  G.curscreen->starty + G.curscreen->sizey - mval[1];
}

void my_put_frontbuffer_image()
{
	ximage= 0;
	
	/* return; */
	glutPutFrontBuffer();	
	
}

void my_get_frontbuffer_image(int x, int y, int sx, int sy)
{
	ximage= 1;

	ov_x= x;
	ov_y= y;
	ov_sx= sx;
	ov_sy= sy;
	
	#ifdef __WIN32
	/* ander coordinatensysteem! */
	y= (G.curscreen->sizey-y);
	
	if(curswin>3) {
		y -= curarea->winrct.ymin;
	}
	#endif

	glutGetFrontBuffer(x, y, sx, sy);	
}

void myCopySubBuffer(int x, int y, int sx, int sy)
{
	/* return; */
	/* glutPutFrontBuffer();	 */
	
}

void myContextSetup()
{
	
}

#ifdef __BeOS

void glutNoBorder()
{
	
}

void usleep(uint val)
{
	suspend_thread(0);
	snooze(val);
	resume_thread(0);
}

int rawkey_beos(int key)
{
	switch(key)
	{
		case 60: return AKEY;
		case 80: return BKEY;
		case 78: return CKEY;
		case 62: return DKEY;
		case 41: return EKEY;
		case 63: return FKEY;
		case 64: return GKEY;
		case 65: return HKEY;
		case 46: return IKEY;
		case 66: return JKEY;
		case 67: return KKEY;
		case 68: return LKEY;
		case 82: return MKEY;
		case 81: return NKEY;
		case 47: return OKEY;
		case 48: return PKEY;
		case 39: return QKEY;
		case 42: return RKEY;
		case 61: return SKEY;
		case 43: return TKEY;
		case 45: return UKEY;
		case 79: return VKEY;
		case 40: return WKEY;
		case 77: return XKEY;
		case 44: return YKEY;
		case 76: return ZKEY;
		case 27: return ZEROKEY; 
		case 18: return ONEKEY; 
		case 19: return TWOKEY; 
		case 20: return THREEKEY; 
		case 21: return FOURKEY; 
		case 22: return FIVEKEY; 
		case 23: return SIXKEY; 
		case 24: return SEVENKEY; 
		case 25: return EIGHTKEY; 
		case 26: return NINEKEY; 
		
		case 1: return ESCKEY;
		case 2: return F1KEY;
		case 3: return F2KEY;
		case 4: return F3KEY;
		case 5: return F4KEY;
		case 6: return F5KEY;
		case 7: return F6KEY;
		case 8: return F7KEY;
		case 9: return F8KEY;
		case 10: return F9KEY;
		case 11: return F10KEY;
		case 12: return F11KEY;
		case 13: return F12KEY;
		
		case GLUT_SHIFT: return LEFTSHIFTKEY;
		case GLUT_ALT: return LEFTALTKEY;
		case GLUT_CTRL: return LEFTCTRLKEY;
		
		case 38: return TABKEY;
		case 71: return RETKEY;
		case 94: return SPACEKEY;
		case 30: return BACKSPACEKEY;
		case 52: return DELKEY;
		case 69: return SEMICOLONKEY;
		case 84: return PERIODKEY;
		case 83: return COMMAKEY;
		case 70: return QUOTEKEY;
		case 17: return ACCENTGRAVEKEY;
		
		case 28: return MINUSKEY;
		case 85: return VIRGULEKEY;
		case 51: return BACKSLASHKEY;
		case 29: return EQUALKEY;
		case 49: return LEFTBRACKETKEY;
		case 50: return RIGHTBRACKETKEY;
		
		
		case 97: return LEFTARROWKEY;
		case 99: return RIGHTARROWKEY;
		case 87: return UPARROWKEY;
		case 98: return DOWNARROWKEY;
		
		case 100: return PAD0;
		case 88: return PAD1;
		case 89: return PAD2;
		case 90: return PAD3;
		case 72: return PAD4;
		case 73: return PAD5;
		case 74: return PAD6;
		case 55: return PAD7;
		case 56: return PAD8;
		case 57: return PAD9;
		
		
		case 101: return PADPERIOD;
		case 35: return PADVIRGULEKEY;
		case 36: return PADASTERKEY;
		case 91: return PADENTER;
		case 37: return PADMINUS;
		case 58: return PADPLUSKEY;


		
		case 31: return INSERTKEY;
		case 32: return HOMEKEY;
		case 33: return PAGEUPKEY;
		case 54: return PAGEDOWNKEY;
		case 53: return ENDKEY;

		default: return 0;
	}
}
#endif

void sdrawXORline(int x0, int y0, int x1, int y1)
{
	if(x0==x1 && y0==y1) return;

	/* sdrawXORline expects current blender win coordinates */
	if(curswin>3) {
		x0+= curarea->winrct.xmin;
		x1+= curarea->winrct.xmin;
		y0+= curarea->winrct.ymin;
		y1+= curarea->winrct.ymin;
	}

	glutInvertLine(x0, y0, x1, y1);	
}


void sdrawXORline4(int nr, int x0, int y0, int x1, int y1)
{
	static short old[4][4];
	static char flags[4]= {0, 0, 0, 0};

	/* automatische onthoud, max 4 lijnen */

	/* flush */
	if(nr== -1) {
		if(flags[0]) sdrawXORline(old[0][0], old[0][1], old[0][2], old[0][3]);
		flags[0]= 0;
		if(flags[1]) sdrawXORline(old[1][0], old[1][1], old[1][2], old[1][3]);
		flags[1]= 0;
		if(flags[2]) sdrawXORline(old[2][0], old[2][1], old[2][2], old[2][3]);
		flags[2]= 0;
		if(flags[3]) sdrawXORline(old[3][0], old[3][1], old[3][2], old[3][3]);
		flags[3]= 0;
	}
	else {

		if(nr>=0 && nr<4) {
			if(flags[nr]) sdrawXORline(old[nr][0], old[nr][1], old[nr][2], old[nr][3]);
			old[nr][0]= x0;
			old[nr][1]= y0;
			old[nr][2]= x1;
			old[nr][3]= y1;
			flags[nr]= 1;
		}
		sdrawXORline(x0, y0, x1, y1);
	}

}

/* end of #if defined(__BeOS ) || defined(WIN32) */


/* ******************************************* */


/* ******************************************* */
/* ******************************************* */
/* ************* X  STUFF (and some OpenGL that doesn't work at BeOS) ***************** */

#else 


extern Display *__glutDisplay;
/* extern GLUTwindow *__glutCurrentWindow; */
extern int __glutScreen;

static XImage *ximage=0;
static GLXDrawable xdraw=0;	/* wordt maar 1 keer gedaan, voor mainwin */
static int ov_x, ov_y, ov_sx, ov_sy;

static GC curgc, blackGC, grayGC, whiteGC, darkGC, lightGC;
static int contextdone= 0;


void sdrawXORline(int x0, int y0, int x1, int y1)
{
	#ifdef MESA31
	return;
	#endif
	
	if(x0==x1 && y0==y1) return;
	
	#if defined(__sgi) || defined(__SUN)
	glDisable(GL_DITHER);
	#endif
	
    glBlendEquationEXT(GL_LOGIC_OP);
	glEnable(GL_LOGIC_OP); glEnable(GL_BLEND);
	glLogicOp(GL_INVERT);
	
	sdrawline(x0, y0, x1, y1);
	
	glBlendEquationEXT(GL_FUNC_ADD_EXT);
	glDisable(GL_LOGIC_OP); glDisable(GL_BLEND);
	
	#if defined(__sgi) || defined(__SUN)
	glEnable(GL_DITHER);
	#endif
	
}

void sdrawXORline4(int nr, int x0, int y0, int x1, int y1)
{
	static short old[4][4];
	static char flags[4]= {0, 0, 0, 0};

	#ifdef MESA31
	return;
	#endif

	/* automatische onthoud, max 4 lijnen */

	#if defined(__sgi) || defined(__SUN)
	glDisable(GL_DITHER);
	#endif
    glBlendEquationEXT(GL_LOGIC_OP);
	glEnable(GL_LOGIC_OP); glEnable(GL_BLEND);
	glLogicOp(GL_INVERT);


	/* flush */
	if(nr== -1) {
		if(flags[0]) sdrawline(old[0][0], old[0][1], old[0][2], old[0][3]);
		flags[0]= 0;
		if(flags[1]) sdrawline(old[1][0], old[1][1], old[1][2], old[1][3]);
		flags[1]= 0;
		if(flags[2]) sdrawline(old[2][0], old[2][1], old[2][2], old[2][3]);
		flags[2]= 0;
		if(flags[3]) sdrawline(old[3][0], old[3][1], old[3][2], old[3][3]);
		flags[3]= 0;
	}
	else {

		if(nr>=0 && nr<4) {
			if(flags[nr]) sdrawline(old[nr][0], old[nr][1], old[nr][2], old[nr][3]);
			old[nr][0]= x0;
			old[nr][1]= y0;
			old[nr][2]= x1;
			old[nr][3]= y1;
			flags[nr]= 1;
		}
		sdrawline(x0, y0, x1, y1);
	}

	glDisable(GL_LOGIC_OP); glDisable(GL_BLEND);
	glBlendEquationEXT(GL_FUNC_ADD_EXT);
	#if defined(__sgi) || defined(__SUN)
	glEnable(GL_DITHER);
	#endif

}

void myContextSetup()
{
	XGCValues gcvals;
	int dep;
	const char *str;
	
	gcvals.font = 0;
	if(xdraw==0) xdraw= glXGetCurrentDrawable();
	
	dep= glutGet(GLUT_WINDOW_BUFFER_SIZE);
	if(dep<12) printf("Error: screen depth (%d) should be at least 15.\n", dep);

	if(G.f & G_DEBUG) {
		printf("Display: %d %d\n", displaysizex, displaysizey);
		PRINT(d, dep);
	}
	
	gcvals.foreground = 0;
	blackGC = XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	
#ifdef __sgi
	if(dep==15 || dep==16) {
#else
	if(dep==15) {
#endif
		gcvals.foreground =  (10 + (10<<5) + (10<<10) ) ;
		darkGC = XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	
		gcvals.foreground = (15 + (15<<5) + (15<<10) );
		grayGC = XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	
		gcvals.foreground = (23 + (23<<5) + (23<<10) );
		lightGC = XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	
		gcvals.foreground = (31 + (31<<5) + (31<<10) );
		whiteGC = XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	}
	else {
		gcvals.foreground = (dep==16? (10 + (10<<6) + (10<<11)) : 0x555555 );
		darkGC = XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	
		gcvals.foreground = (dep==16? (15 + (15<<6) + (15<<11)) : 0x999999 );
		grayGC = XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	
		gcvals.foreground = (dep==16? (23 + (23<<6) + (23<<11)) : 0xcccccc );
		lightGC = XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	
		gcvals.foreground = (dep==16? (31 + (31<<6) + (31<<11)) : 0xffffff );
		whiteGC = XCreateGC(__glutDisplay, xdraw, GCForeground, &gcvals);
	}
	curgc= blackGC;
	
	contextdone= 1;
}

short get_qual(void)
{
	Window child, root;
	int root_x,root_y;
	int win_x, win_y;
	unsigned int Xqual;
	short ret= 0;
	
	XQueryPointer(__glutDisplay, RootWindow(__glutDisplay, __glutScreen),  
		&root, &child, &root_x, &root_y, &win_x, &win_y, &Xqual);

	if( Xqual & ShiftMask)   ret |= LR_SHIFTKEY;
	if( Xqual & ControlMask) ret |= LR_CTRLKEY;
	if( Xqual & Mod1Mask)    ret |= LR_ALTKEY;
	
	return ret;
}

short get_mbut(void)
{
	Window child, root;
	int root_x,root_y;
	int win_x, win_y;
	unsigned int Xqual;
	short ret= 0;
	
	XQueryPointer(__glutDisplay, RootWindow(__glutDisplay, __glutScreen),  
		&root, &child, &root_x, &root_y, &win_x, &win_y, &Xqual);

	if (Xqual & Button1Mask) ret |= L_MOUSE;
	if (Xqual & Button2Mask) ret |= M_MOUSE;
	if (Xqual & Button3Mask) ret |= R_MOUSE;
	
	if ((U.flag & TWOBUTTONMOUSE) && (Xqual & Mod1Mask)) {
		if (ret & L_MOUSE) {
			ret &= ~L_MOUSE;
			ret |= M_MOUSE;
		}
	}
	
	return ret;
}

void getmouse(short *mval)
{
	Window child, root;
	int root_x,root_y;
	int win_x, win_y;
	unsigned int Xqual;
	
	XQueryPointer(__glutDisplay, RootWindow(__glutDisplay, __glutScreen),  
		&root, &child, &root_x, &root_y, &win_x, &win_y, &Xqual);

	mval[0]= win_x;
	mval[1]= displaysizey - win_y;
}


void myCopySubBuffer(int x, int y, int sx, int sy)
{
	short id;
	
	if(xdraw==0) xdraw= glXGetCurrentDrawable();

	/* clip: erg belangrijk: 1 pixel teveel en X knalt eruit */
	if(x<0) x= 0;
	if(y<0) y= 0;
	if( x+sx >= G.curscreen->sizex) sx= G.curscreen->sizex-x-1;
	if( y+sy >= G.curscreen->sizey) sy= G.curscreen->sizey-y-1;

	/* only mesa 3.0 (libc5 for now) will have the swap hack */

	#ifdef MESA30
		glXCopySubBufferMESA(__glutDisplay, xdraw, x,  y, sx, sy);
	#else
		id= curswin;
		mywinset(G.curscreen->mainwin);		/* op schermco's zetten */

		glDrawBuffer(GL_FRONT);
		glRasterPos2i(x, y);
		glCopyPixels(x, y, sx, sy, GL_COLOR);
		glDrawBuffer(GL_BACK);
	
		mywinset(id);
	#endif

}

void glutGetFrontBuffer(int a, int b, int c, int d)
{
	/* fake function */
}

void glutPutFrontBuffer()
{
	/* fake function */
}


/* ************ OVERDRAW SUPPORT ************* */

void my_put_frontbuffer_image()
{
	GC gc;

	if(ximage && xdraw) {	

		gc= XCreateGC( __glutDisplay, xdraw, 0, NULL );
		XSetFunction( __glutDisplay, gc, GXcopy );
		 
		XPutImage( __glutDisplay, xdraw, gc, ximage, 0, 0, ov_x, ov_y, ov_sx, ov_sy);
		XFreeGC(__glutDisplay, gc);
		
		XDestroyImage(ximage);
		
		ximage= 0;
	}
}

void my_get_frontbuffer_image(int x, int y, int sx, int sy)
{
	int startx, starty, sizex, sizey;

	if(xdraw==0) xdraw= glXGetCurrentDrawable();

	if(ximage==NULL) {

		/* clip: erg belangrijk: 1 pixel teveel en X knalt eruit */
		
		/* also: blenderscreen outside 'Display': then prtend the window is smaller! */
		
		if(G.curscreen->startx<0) startx= -G.curscreen->startx;
		else startx= 0;
		if(G.curscreen->starty<0) starty= -G.curscreen->starty;
		else starty= 0;
				
		if(G.curscreen->sizex + G.curscreen->startx > displaysizex) sizex= displaysizex - G.curscreen->startx;
		else sizex= G.curscreen->sizex;
		if(G.curscreen->sizey + G.curscreen->starty > displaysizey) sizey= displaysizey - G.curscreen->starty;
		else sizey= G.curscreen->sizey;
		
		/* OK: */
		if(x<startx) x= startx;
		if(y<starty) y= starty;
		
		if( x+sx >= sizex) sx= sizex-x-1;
		if( y+sy >= sizey) sy= sizey-y-1;
	
		/* ander coordinatensysteem! */
		ov_x= x;
		ov_y= (G.curscreen->sizey-y-sy);
		ov_sx= sx;
		ov_sy= sy;
	
		/* deze bewaren we omdat bij terugzetten een andere window aktief kan zijn geworden */
		
		ximage = XGetImage( __glutDisplay, xdraw, ov_x, ov_y, ov_sx, ov_sy, AllPlanes, ZPixmap);
	}
}

#endif

void warp_pointer(int x, int y)
{

	glutWarpPointer(x, glutGet(GLUT_WINDOW_HEIGHT)-y);
}


/*******************/
/*******************/
/*  Menu utilities */

int mywin_inmenu(void) {
	if(ximage) return 1;
	else return 0;
}

void mywin_getmenu_rect(int *x, int *y, int *sx, int *sy) {
	*x= ov_x;
	*sx= ov_sx;
	*sy= ov_sy;

#if defined(WIN32) || defined (__BeOS)
	*y= ov_y;
#else
	*y= (G.curscreen->sizey - ov_y) - ov_sy;
#endif	
}

