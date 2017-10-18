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

/* gltest.c 

 * 
 * dec 98
 */

#include "blender.h"
#include "graphics.h"
#include "screen.h"

struct Global G;

int displaysizex= 0, displaysizey= 0;
int prefsizx= 0, prefsizy= 0, prefstax= 0, prefstay= 0;
short scrmousex, scrmousey;	/* worden gezet door mousecallback */
short mainwin=0, winqueue_break= 0, cursonedge=0, keycode[100];

float light_pos1[] = { -0.3, 0.3, 0.90, 0.0 }; 
float light_pos2[] = { 0.3, -0.3, -0.90, 0.0 }; 


void myortho2(float x1, float x2, float y1, float y2)
{
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(x1, x2, y1, y2);

	glMatrixMode(GL_MODELVIEW);
}

void init_gl_stuff()	
{
	float mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
	float mat_shininess[] = { 35.0 };
	float light_col1[] = { 0.8, 0.8, 0.8, 0.0 }; 
	float one= 1.0;
		
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos1); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_col1); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular); 

	glEnable(GL_LIGHT0);
	
	#if defined(__FreeBSD) || defined(linux)
	glDisable(GL_DITHER);	/* op sgi/sun hardware && 12 bits */
	#endif
	
	/* glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, &one); */
	
	glDepthFunc(GL_LEQUAL);
	/* scaling matrices */
	glEnable(GL_NORMALIZE);

	glShadeModel(GL_FLAT);

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glDisable(GL_LOGIC_OP);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);

	glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
	glPixelTransferi(GL_RED_SCALE, 1);
	glPixelTransferi(GL_RED_BIAS, 0);
	glPixelTransferi(GL_GREEN_SCALE, 1);
	glPixelTransferi(GL_GREEN_BIAS, 0);
	glPixelTransferi(GL_BLUE_SCALE, 1);
	glPixelTransferi(GL_BLUE_BIAS, 0);
	glPixelTransferi(GL_ALPHA_SCALE, 1);
	glPixelTransferi(GL_ALPHA_BIAS, 0);
}

static float c_r=0.0, c_b=0.0, c_g=0.0;

void myglClearColor(float a, float b, float c, float d)
{
	c_r= a;
	c_g= b;
	c_b= c;
}

void myglClear(int a)
{
	
	glColor3f(c_r, c_g, c_b);

	
	glRects(-1000, -1000, 2000, 2000);
}



void mousefunc(int button, int state, int x, int y)
{
	short mval[2];
	int buf[40*40*4];
	
	
	/* glutGetMouse(mval); */
	
	if(button==GLUT_LEFT_BUTTON) {
		/* if(state==GLUT_DOWN) qenter(LEFTMOUSE, 1); */
		/* else qenter(LEFTMOUSE, 0); */
	}
	else if(button==GLUT_MIDDLE_BUTTON) {
		/* if(state==GLUT_DOWN) qenter(MIDDLEMOUSE, 1); */
		/* else qenter(MIDDLEMOUSE, 0); */
	}
	else if(button==GLUT_RIGHT_BUTTON) {
		/* if(state==GLUT_DOWN) qenter(RIGHTMOUSE, 1); */
		/* else qenter(RIGHTMOUSE, 0); */
	}	
	glClearColor(0.5, 0.2, 0.6, .0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glDrawBuffer(GL_FRONT);
	glReadBuffer(GL_BACK);
	
	glRasterPos2f(40.0,  40.0);

printf("head back to front\n");

	glReadPixels(0, 0, 30, 30, GL_RGB, GL_UNSIGNED_BYTE,  buf);
	glDrawPixels(30, 30, GL_RGB, GL_UNSIGNED_BYTE,  buf);

	/* glCopyPixels(0,  0,  40,  40, GL_COLOR); */
	
	glDrawBuffer(GL_BACK);
/*	glutFlush(); */
	glFlush();
}


void redraw1func()
{
	short mval[2];
	int buf[40*40*4];

	glClearColor(0.5, 0.2, 0.6, .0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glReadBuffer(GL_BACK);
	
	glRasterPos2f(40.0,  40.0);

printf("redraw head back to front\n");

	glReadPixels(0, 0, 30, 30, GL_RGBA, GL_UNSIGNED_BYTE,  buf);

	glDrawBuffer(GL_FRONT);

	glDrawPixels(30, 30, GL_RGBA, GL_UNSIGNED_BYTE,  buf);

	/* glCopyPixels(0,  0,  40,  40, GL_COLOR); */
	
	glDrawBuffer(GL_BACK);
	/* qenter(REDRAW, 1); */
}

void redraw2func()
{
	
	/* qenter(REDRAW, 2); */
}

void visiblefunc(int dummy)
{
	/* geen winfreeze */
	
}

void inputchangefunc(int state)
{
	int win;
	
	if(state) {	
		/* qenter(INPUTCHANGE, glutGetWindow()); */
	}
}


void reshapefunc(int x, int y)
{
	bScreen *sc;
	int wx, wy, orx, ory;

	/* alleen voor hoofdwindow */
	
	/* qenter(REDRAW, mainwin); */

	wx= glutGet(GLUT_WINDOW_WIDTH);
	wy= glutGet(GLUT_WINDOW_HEIGHT);

	orx= glutGet(GLUT_WINDOW_X);
	ory= displaysizey -wy- glutGet(GLUT_WINDOW_Y);

	prefstax= orx;
	prefstay= ory;
	prefsizx= wx;
	prefsizy= wy;
	

}

void mousemovefunc(int x, int y)
{
	/* ommappen */
	scrmousex= x;
	scrmousey= prefsizy-y;

	/* qenter(MOUSEX, scrmousex); (een te veel?) */
	/* qenter(MOUSEY,  scrmousey); */

}

void sdrawline(short x1, short y1, short x2,short y2)
{
	short v[2];

	glBegin(GL_LINE_STRIP);
	v[0] = x1; v[1] = y1;
	glVertex2sv(v);
	v[0] = x2; v[1] = y2;
	glVertex2sv(v);
	glEnd();
}



void headerbox(int selcol, int width)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
	
	if(selcol) glClearColor(.7, .7, .6, 0.0);
	else glClearColor(.6, .6, .6, 0.0);
	
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3ub(0, 0, 0);
	sdrawline(0,  HEADERY,  width,  HEADERY);

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



void rawkeyfunc(int key, int val, int y)
{
	



	glClearColor(0.5, 0.5, 0.5, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3ub(100, 10, 100);
	glRecti(10, 10, 100, 100);
	
	headerbox(0x406070, 256);

	glColor3ub(200, 100, 200);
	glRecti(70, 10, 140, 140);
	
	glColor3ub(0, 0, 0);
	
	glRasterPos2i(50, 10);
	glutBitmapStr(G.font, "Hallo deze tekst is een voorbeeld");	
	
	glutSwapBuffers();
}

void keyboardfunc(uchar key, int x, int y)
{
	/* qenter(KEYBD, key); */
	glutSwapBuffers();
}

void test()
{
	glEnable(GL_SCISSOR_TEST);

	glClearColor(0.5, 0.5, 0.5, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glViewport(100, 100, 120, 56);
	glScissor(100, 100, 120, 56);
	
	glClearColor(0.8, 0.2, 0.5, 0.8);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, 320, 256);
	glScissor(0, 0, 320, 256);
	
	glutSwapBuffers();
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

	/* glutIdleFunc(test); */
	
}

int mywinopen(int mode, short posx, short posy, short sizex, short sizey)
{
	int win;
	
	glutInitDisplayMode(mode);
	glutInitWindowSize(sizex, sizey);
	glutInitWindowPosition(posx, displaysizey - sizey - posy);

	win= glutCreateWindow("Blender");
	
	/* niet netjes: alleen eerste keer! */
	/* if(win==1) init_my_mainwin(win); */
	
	/* anders matrices in de war */
	/* winset(win); */

	if(win==1) {
		glutDisplayFunc(redraw2func);
		glutReshapeFunc(reshapefunc);
	}
	else glutDisplayFunc(redraw2func);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	myortho2(-0.5, (float)sizex-0.5, -0.5, (float)sizey-0.5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0,  0, sizex-0, sizey-0);
	glScissor(0,  0, sizex-0, sizey-0);


	/* WARN! geen standaard fie */
	/* glutDoWorkList(); */
	
	return win;
}

struct byte1 {
	char rt;
};
struct short1 {
	short rt;
};
struct short2 {
	short rt1, rt2;
};
struct int1 {
	int rt;
};
struct short3 {
	short rt1, rt2, rt3;
};

struct alignerror1 {
	int rt1;
	int *rt2;
	int rt3;
};

struct alignerror {
	struct int1 rt1;
	struct short2 rt2;
	
};

int main(argc,argv)
int argc;
char **argv;
{
	long lval;
	int a, dep, window, *rt;
			
	a= 1;	/* patch zodat glutinit van de argumenten afblijft */
	glutInit(&a, argv);

	displaysizex= glutGet(GLUT_SCREEN_WIDTH);
	displaysizey= glutGet(GLUT_SCREEN_HEIGHT);

	lval = -1;
	PRINT(lx, lval);
	rt= (int *)lval;
	PRINT(lx, rt);
	dep= 0;
	lval= dep;
	PRINT(lx, &lval);
	rt= 0;
	if(rt == 0) {
		printf("rt is nul\n");
	}
	
	PRINT2(d, d, sizeof(struct byte1), sizeof(struct short1));
	PRINT2(d, d, sizeof(struct short2), sizeof(struct short3));
	PRINT2(d, d, sizeof(struct int1), sizeof(struct alignerror));
	
	
	PRINT2(d, d, displaysizex, displaysizey);

	PRINT2(d, d, sizeof(short), sizeof(int));
	PRINT2(d, d, sizeof(uint), sizeof(float));
	PRINT2(d, d, sizeof(long), sizeof(ulong));
	PRINT2(d, d, sizeof(char *), sizeof(short *));
	PRINT2(d, d, sizeof(long *), sizeof(int *));

	G.order= 1;
	if( ((char *)&G.order)[1] ) {
		G.order= B_ENDIAN;
		printf("Big endian: hoera!\n");
	}
	else {
		G.order= L_ENDIAN;
		printf("Little endian: PC...\n");
	}


	window = mywinopen(GLUT_RGB | GLUT_DOUBLE, 0, 0, 320, 256);

	dep= glutGet(GLUT_WINDOW_BUFFER_SIZE);
	PRINT(d, dep);
	
	do_the_glut_funcs();

	init_gl_stuff();

	G.font= GLUT_BITMAP_HELVETICA_12;
	G.fonts= GLUT_BITMAP_HELVETICA_10;
		
	glEnable(GL_SCISSOR_TEST);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	
	glutMainLoop();
	sleep(100);
	
	exit(0);
}

/* #endif */

