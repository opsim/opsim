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


/* renderfg.c  april 94		GRAPHICS
 * 
 * render foreground routines
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "screen.h"
#include "render.h"

#include <signal.h>
#ifndef __WIN32
#include <sys/time.h>
#endif

extern uint pr_scan[];

/* ~~~~~~~~~~~~~TIMECURSOR~~~~~~~~~~~~~~~~~~~~~~~ */

#if defined(__BeOS)||defined(__WIN32)

char numbar[10][8]= {
	0, 0x1c, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1c,
	0, 0x18, 0x8, 0x8, 0x8, 0x8, 0x8, 0x1c, 
	0, 0x3c, 0x42, 0x4, 0x8, 0x10, 0x20, 0x7e, 
	0, 0x3e, 0x4, 0x8, 0x1c, 0x2, 0x42, 0x3c, 
	0, 0x4, 0xc, 0x14, 0x24, 0x7e, 0x4, 0x4, 
	0, 0x3e, 0x20, 0x3c, 0x2, 0x2, 0x22, 0x1c, 
	0, 0x1c, 0x20, 0x20, 0x3c, 0x22, 0x22, 0x1c, 
	0, 0x3e, 0x2, 0x4, 0x8, 0x10, 0x10, 0x10, 
	0, 0x3c, 0x42, 0x42, 0x3c, 0x42, 0x42, 0x3c, 
	0, 0x1c, 0x22, 0x22, 0x1e, 0x2, 0x22, 0x1c,
};

#else 

char numbar[10][8]= {
	0, 56, 68, 68, 68, 68, 68, 56, 
	0, 24, 16, 16, 16, 16, 16, 56, 
	0, 60, 66, 32, 16, 8, 4, 126, 
	0, 124, 32, 16, 56, 64, 66, 60, 
	0, 32, 48, 40, 36, 126, 32, 32, 
	0, 124, 4, 60, 64, 64, 68, 56, 
	0, 56, 4, 4, 60, 68, 68, 56, 
	0, 124, 64, 32, 16, 8, 8, 8, 
	0, 60, 66, 66, 60, 66, 66, 60, 
	0, 56, 68, 68, 120, 64, 68, 56, 
};

#endif

void set_timecursor(int nr)
{
	static int lastnr= 0xFFFFFF;
	int a, dig;
	static ushort curs[32];
	char *cp;

	if(G.background) return;
	if(G.curscreen->winakt == R.win) return;
	
	if(lastnr>nr) {
		for(a=0; a<16; a++) curs[a]= 0xFFFF;
		for(a=16; a<32; a++) curs[a]= 0x0;
	}
	lastnr= nr;
	
	/* kleiner dan 10 */
	cp= ((char *)(curs+16))+17;
	dig= nr % 10;
	
	for(a=0; a<8; a++, cp+=2) {
		cp[0]= numbar[dig][a];
	}
	
	if(nr>9) {
		cp= ((char *)(curs+16))+16;
		dig= (nr/10) % 10;
		for(a=0; a<8; a++, cp+=2) {
			cp[0]= numbar[dig][a];
		}
		if(nr>99) {
			cp= ((char *)(curs+16))+1;
			dig= (nr/100) % 10;
			for(a=0; a<8; a++, cp+=2) {
				cp[0]= numbar[dig][a];
			}
			if(nr>999) {
				cp= ((char *)(curs+16));
				dig= (nr/1000) % 10;
				for(a=0; a<8; a++, cp+=2) {
					cp[0]= numbar[dig][a];
				}
			}
		}
	}
	
	glutCustomCursor((char *)curs, (char *)(curs+16), 16);
}

/* ~~~~~~~~~~~~~TIMER~~~~~~~~~~~~~~~~~~~~~~~~~ */


#ifdef __BeOS


int test_break()
{
	short val;
	
	if(G.background) return 0;
	if(BeOS_escape==1) G.afbreek= 1;
	
	if(G.afbreek==1) return 1;
	else return 0;
}

void bgntimer()
{
	BeOS_escape= 0;
}

void endtimer()
{
	;
}

#else

#ifdef __WIN32

extern int win32_escape;

int test_break()
{
	short val;
	
	if(G.background) return 0;
	if(glutTestEsc()) G.afbreek= 1;
	
	if(G.afbreek==1) return 1;
	else return 0;
}

void bgntimer()
{
	;
}

void endtimer()
{
	;
}

#else	/* not beos not win32 = unix! */

int test_break()
{
	short val;
	
	if(G.background) return 0;
	if(G.afbreek==2) {

		/* queue testen */
		
		G.afbreek= 0;
		while(qtest()) {
			if( extern_qread(&val) == ESCKEY) {
				G.afbreek= 1;
			}
		}
	}

	if(G.afbreek==1) return 1;
	else return 0;
}


void interruptESC(int sig)
{

	if(G.afbreek==0) G.afbreek= 2;	/* code voor queue lezen */

	/* opnieuw zetten: wordt namelijk gereset */
	signal(SIGVTALRM, interruptESC);
}


void bgntimer()
{

	struct itimerval tmevalue;

	tmevalue.it_interval.tv_sec = 0;
	tmevalue.it_interval.tv_usec = 250000;
	/* wanneer de eerste ? */
	tmevalue.it_value.tv_sec = 0;
	tmevalue.it_value.tv_usec = 10000;

	signal(SIGVTALRM, interruptESC);
	setitimer(ITIMER_VIRTUAL, &tmevalue, 0);

}

void endtimer()
{
	struct itimerval tmevalue;

	tmevalue.it_value.tv_sec = 0;
	tmevalue.it_value.tv_usec = 0;
	setitimer(ITIMER_VIRTUAL, &tmevalue, 0);
	signal(SIGVTALRM, SIG_IGN);

}

#endif
#endif

/* ~~~~~~~~~~~~~DISPLAY~~~~~~~~~~~~~~~~~~~~~~~ */

void close_render_display()
{
	if(G.background) return;

	if(R.win) {
		winclose(R.win);
		R.win= 0;
	}
}

void init_render_display()
{
	ScrArea *sa;
	View3D *vd;
	float xm, ym;
	static int xs=0, ys;
	int xo, yo;
	int winok=0, div, inview=0;
	short mval[2];
	
	if(G.background) return;

	/* eerst uitzoeken waar de display komt */
	if(R.displaymode==R_DISPLAYVIEW) {
		if(R.win) {
			winclose(R.win);
			R.win= 0;
		}
		/* elke area die persp==2 en goede camera heeft: display maken */
		sa= G.curscreen->areabase.first;
		while(sa) {
			if(sa->win && sa->spacetype==SPACE_VIEW3D) {
				vd= sa->spacedata.first;
				if(vd->persp==2 && vd->camera==G.scene->camera) {
					inview= 1;
					areawinset(sa->win);

					vd->pr_sizex= vd->pr_xmax- vd->pr_xmin;
					vd->pr_sizey= vd->pr_ymax- vd->pr_ymin;
					vd->pr_facx= ( (float)vd->pr_sizex)/((float)R.rectx);
					vd->pr_facy= ( (float)vd->pr_sizey)/((float)R.recty);
				}
				else {
					vd->pr_sizex= 0;
				}
			}
			sa= sa->next;
		}
	
		if(inview==0) {
			/* eerste view3d nemen en dze initialiseren */
			sa= G.curscreen->areabase.first;
			while(sa) {
				if(sa->spacetype==SPACE_VIEW3D) {
					vd= sa->spacedata.first;
					
					areawinset(sa->win);
					calc_viewborder();
					
					vd->pr_sizex= vd->pr_xmax- vd->pr_xmin;
					vd->pr_sizey= vd->pr_ymax- vd->pr_ymin;
					vd->pr_facx= ( (float)vd->pr_sizex)/((float)R.rectx);
					vd->pr_facy= ( (float)vd->pr_sizey)/((float)R.recty);
					
					break;
				}
				sa= sa->next;
			}
		}
		
	} else if(R.displaymode==R_DISPLAYWIN) {

		if(xs==0) {
			xs = displaysizex;
			ys = displaysizey;
		}

		/* eerst berekenen: waar moet ie staan */
		if(R.winpos) {
			div= 0; 
			xm= 0.0; 
			ym= 0.0;
			if(R.winpos & (1+8+64)) {
				div++; 
				xm+=.1666;
			}
			if(R.winpos & (2+16+128)) {
				div++; 
				xm+= .50;
			}
			if(R.winpos & (4+32+256)) {
				div++; 
				xm+=.8333;
			}
			xm/= (float)div;
			div= 0;
			if(R.winpos & (1+2+4)) {
				div++; 
				ym+=.1666;
			}
			if(R.winpos & (8+16+32)) {
				div++; 
				ym+= .50;
			}
			if(R.winpos & (64+128+256)) {
				div++; 
				ym+=.8333;
			}
			ym/= (float)div;
			
			R.winxof= (xs*xm- R.rectx/2);
			R.winyof= (xs*ym- R.recty/2);
		}
		if(R.winxof+R.rectx-1>xs) R.winxof= xs-R.rectx-1;
		if(R.winyof+R.recty-1>ys-20) R.winyof= ys-20-R.recty-1;
		if(R.winxof<0) R.winxof=0;
		if(R.winyof<0) R.winyof=0;
		
		if(R.rectx>=xs-2) R.winxof=0;
		if(R.recty>=ys-2) R.winyof=0;
		
		/* window testen */
		if(R.win) {
			winset(R.win);
			if(R.winx!=R.rectx || R.winy!=R.recty) {
				glClearColor(.4375, .4375, .4375, 0.0);
				glClear(GL_COLOR_BUFFER_BIT);
			}
			else winok= 1;
			
			/* alleen als weggepusht */
			
			if(R.winpop) {
				getorigin(&xo, &yo);
				if(xo!=R.winxof || yo!=R.winyof) winok= 0;
			}
		}

		
		if(winok==0) {
			/* (voorlopig) altijd sluiten */
			if(R.win) winclose(R.win);
			R.win= 0;
			
			if(R.win==0) {
				#ifdef __BeOS
				/* glutSmallBorder(); */
				#endif
				
				#if defined(__BeOS) || defined(__WIN32)
				R.win= mywinopen(GLUT_DOUBLE|GLUT_RGB | GLUT_DEPTH, R.winxof, R.winyof, R.rectx, R.recty);
				#else /* unix */
				R.win= mywinopen(GLUT_SINGLE|GLUT_RGB | GLUT_DEPTH, R.winxof, R.winyof, R.rectx, R.recty);
				#endif
				
				/* for R_OGL rendering, do always, can happen after a normal rendering */
				init_gl_stuff();
				
				do_the_glut_funcs();

				/* force inputchange event */
				#if !defined(__BeOS) && !defined(__WIN32)
				glutPushWindow();
				glutDoWorkList();
				glutPopWindow();
				glutDoWorkList();
				#endif
				
				#if !defined(__BeOS)&&!defined(__WIN32)
				glXWaitX();
				#endif
				
				R.winpop= 1;
			}
			else {
				/* dit werkt niet lekker */
				
				glutPositionWindow(R.winxof, displaysizey-R.recty-R.winyof);
				glutReshapeWindow(R.rectx, R.recty);
				
				mywinposition(R.win, R.winxof, R.winyof, R.winxof+R.rectx-1, R.winyof+R.recty-1);
				
				/* om rare reden is deze volgorde van belang? */
				glutDoWorkList();
				glutNoBorder();

			}

			R.winx= R.rectx;
			R.winy= R.recty;
			
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			
			#if defined(__BeOS)||defined(__WIN32)
			glutSwapBuffers();
			#endif
		}
		if(R.winpop) {
			glutPopWindow();
			glutDoWorkList();
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			
			#if defined(__BeOS)||defined(__WIN32)
			glutSwapBuffers();
			#endif
		}
		ortho2(-0.5, (float)R.winx-0.5, -0.5, (float)R.winy-0.5);
		
		R.winpop= 0;
	}
	
	/* moet altijd ivm clear overdraw ? */
	/* areawinset(G.curscreen->winakt); */
}


void display_scanline(View3D *vd, uint *rect, int starty, int endy)
{
	float sx, sy, ey;
	
	/* patch! */
	if(starty>0) starty--;
	if(endy<R.recty-1) endy++;

	rect+= starty*R.rectx;
	
	sy= vd->pr_ymin+ (vd->pr_facy*starty);
	ey= vd->pr_ymin+ (vd->pr_facy*endy);

	if(ey<0) return;
	
	if(sy<0.0) {
		rect+= R.rectx*( (int)((-sy)/vd->pr_facy) );
		sy= 0.0;
	}
	
	sx= vd->pr_xmin;
	if(sx<0.0) {
		rect+= (int)((-sx)/vd->pr_facx);
		sx= 0.0;
	}
	
	glPixelZoom(vd->pr_facx, vd->pr_facy);

	glRasterPos2f(sx, sy);
	glDrawPixels(R.rectx, endy-starty+1, GL_RGBA, GL_UNSIGNED_BYTE,  rect);
	glPixelZoom(1.0, 1.0);
}


void render_display(int starty, int endy)
{
	ScrArea *sa;
	View3D *vd;
	uint *rect;
	int y;
	
	if(G.background) return;

	if(R.displaymode==R_DISPLAYWIN) {
		if(R.win) {
			mywinset(R.win);
			
			rect= R.rectot+starty*R.rectx;
			
			#if defined(__BeOS)||defined(__WIN32)
			glDrawBuffer(GL_FRONT);
			#endif

			glRasterPos2f(-0.01,  -0.01+(float)starty);
			glDrawPixels(R.rectx, endy-starty+1, GL_RGBA, GL_UNSIGNED_BYTE, rect);

			/* rectwrite_part(0, starty, R.rectx, endy-starty+1, 0, 0, R.rectx, endy-starty+1, 1.0, 1.0, rect); */
			
			#if defined(__BeOS)||defined(__WIN32)
			glDrawBuffer(GL_BACK);
			#endif
		}
	}
	else {
		
		sa= G.curscreen->areabase.first;
		while(sa) {
			if(sa->spacetype==SPACE_VIEW3D) {
				vd= sa->spacedata.first;
				if(vd->pr_sizex) {
					if(sa->win != winget()) areawinset(sa->win);
					persp(0);	/* ortho */
	
					glDrawBuffer(GL_FRONT);
					
					display_scanline(vd, R.rectot, starty, endy);

					glDrawBuffer(GL_BACK);
					vd->flag |= V3D_DISPIMAGE;
					
					sa->win_swap= WIN_FRONT_OK;
					
					persp(1);
				}
			}
			sa= sa->next;
		}
	}
}

void clear_render_display()
{

	if(G.background) return;

	if(R.win) {
		winset(R.win);

		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		#if defined(__BeOS)||defined(__WIN32)
		glutSwapBuffers();
		#endif

		/* areawinset(G.curscreen->winakt); */
	}
	else {
		;
	}
}

void redraw_render_win(int val)
{

	if(val==R.win) {
		if(R.rectot) {
			winset(R.win);
	
			glRasterPos2f(-0.01,  -0.01);
			glDrawPixels(R.rectx, R.recty, GL_RGBA, GL_UNSIGNED_BYTE, R.rectot);
			
			#if defined(__BeOS)||defined(__WIN32)
			glutSwapBuffers();
			#endif
		
			/* areawinset(G.curscreen->winakt); */
		}
	}
}

void redraw_render_display()
{
	if(R.win) redraw_render_win(R.win);
	else {
		;
	}
}

void toggle_render_display()
{
	ScrArea *sa;
	View3D *vd;
	int ok= 1;

	if(R.displaymode==R_DISPLAYWIN) {
		
		if(R.rectot) {
			
			if(R.win) {
				winset(R.win);

				if(R.winpop) {
					init_render_display();	/* controleert size en popt */
					R.winpop= 0;
				}
				else {
					/* On windows focus wasn't given to the top window
						after a push... */
					#if defined(__WIN32) || defined(__BeOS)
						/* winset(0); */
						/* glutPopWindow(); */
						/* glutDoWorkList(); */
						winset(G.curscreen->mainwin);
						winclose(R.win);
						R.win= 0;
						/* printf("winclose\n"); */
					#else
						glutPushWindow();
						glutDoWorkList();
					#endif

					R.winpop= 1;
				}
				/* areawinset(G.curscreen->winakt); */
			}
			else {
				init_render_display();
				redraw_render_win(R.win);
			}
		}
	}
	else if(R.displaymode==R_DISPLAYVIEW) {
		if(R.rectot) {
			if(R.win && R.winpop==0) {
				winclose(R.win);
				R.win= 0;

				/* areawinset(G.curscreen->winakt); */
				return;
			}
			sa= G.curscreen->areabase.first;
			while(sa) {
				if(sa->spacetype==SPACE_VIEW3D) {
					vd= sa->spacedata.first;
					if((vd->flag & V3D_DISPIMAGE)) {
						ok= 0;
						break;
					}
				}
				sa= sa->next;
			}
			if(ok) {
				init_render_display();
				render_display(0, R.recty-1);
			}
			else {
				/* redraw maken */
				sa= G.curscreen->areabase.first;
				while(sa) {
					if(sa->spacetype==SPACE_VIEW3D) {
						vd= sa->spacedata.first;
						if((vd->flag & V3D_DISPIMAGE)) addqueue(sa->win, REDRAW, 1);
					}
					sa= sa->next;
				}
			}
		}
	}
}

void zoomwin()
{
	ScrArea *sa;
	static float zoom=3.0;
	float fx, fy;
	int q_event;
	short x, y, cx, cy, cxo=12000, cyo=0, lus=1, val, mval[2];

	winset(R.win);

	glColor3ub(0, 0, 0);	/* voor hulprectje */
	
	while(lus) {
		
		/* windows sometimes screws this up */
		G.curscreen->winakt= R.win;
		
		#if defined(__WIN32) || defined(__BeOS)
		glutGetMouse(mval);
		x= mval[0];
		y= R.recty - mval[1];
		
		#else
		
		getmouse(mval);	/* returns windowcos */
		x= mval[0]-R.winxof;
		y= mval[1]-R.winyof;
		
		#endif

		CLAMP(mval[0], 0, R.rectx);
		CLAMP(mval[1], 0, R.recty);

		/* (x,y) is het samplepunt in de rect */
		/* de linkeronderhoek van de 'virtuele rect zit op: */
		
				
		fx= ((float)x)/(float)R.rectx;
		fy= ((float)y)/(float)R.recty;
		
		cx= -fx*( (zoom-1.0)*R.rectx);
		cy= -fy*( (zoom-1.0)*R.recty);
	
		if(cx!=cxo || cy!=cyo) {
			cxo=cx; cyo=cy;
			
			/* omdat linksonder niet overlappend ge-lrectwrite mag worden... */
			glRecti(0, 0, R.rectx, (int)zoom);
			glRecti(0, 0, (int)zoom, R.recty);

			rectwrite_part(0, 0, R.rectx, R.recty, cx, cy, R.rectx, R.recty, zoom, zoom, R.rectot);

			#if defined(__BeOS)||defined(__WIN32)
			glutSwapBuffers();
			#endif
		}
		while(qtest() && lus==1) {
		
			q_event= extern_qread(&val);
			
			if(val) {
				if(q_event==ESCKEY) lus=0;
				else if(q_event==INPUTCHANGE) {
					lus=0;
				}
				else if(q_event==PADMINUS) {
					zoom-=1.0; cxo= 0;
					if(zoom<2.0) zoom= 2.0;
					else glPixelZoom(zoom,  zoom);
				}
				else if(q_event==PADPLUSKEY) {
					zoom+=1.0;
					if(zoom>15.0) zoom= 15.0;
					else glPixelZoom(zoom,  zoom);
					cxo=0;
				}
			}
		}
		usleep(1);
	}
	
	winset(R.win);
	
	glRasterPos2f(-0.01,  -0.01);
	glDrawPixels(R.rectx, R.recty, GL_RGBA, GL_UNSIGNED_BYTE, R.rectot);
	
	#if defined(__BeOS)||defined(__WIN32)
	glutSwapBuffers();
	#endif
}

/* ***************************************** */

void printrenderinfo(int mode)
{
	ScrArea *sa;
	extern int mem_in_use;
	int x, y;
	char str[300], tstr[32];
	
	if(G.background) return;
	
	timestr(G.cputime, tstr);
	sprintf(str, "RENDER  Fra:%d  Ve:%d Fa:%d  La:%d  Mem:%.2fM Time:%s (%.2f) ",
			CFRA, R.totvert, R.totvlak, R.totlamp,  (mem_in_use>>10)/1024.0, tstr, ((float)(G.time-G.cputime))/100);

	if(R.r.mode & R_FIELDS) {
		if(R.flag & R_SEC_FIELD) strcat(str, "Field B ");
		else strcat(str, "Field A ");
	}

	if(mode!= -1) {
		sprintf(tstr, "Sample: %d ", mode);
		strcat(str, tstr);
	}
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->spacetype==SPACE_INFO) {
			areawinset(sa->headwin);
			glDrawBuffer(GL_FRONT);
			
			x= sa->headbutlen-28;
			y= 6;

			cpack(0xA08060);
			glRecti(x-4,  y-2,  x+55,  y+13);
			
			cpack(0x909090);
			glRecti(x+55,  y-3,  x+1280,  y+14);
			
			cpack(0x0);
			glRasterPos2i(x,  y);
			fmsetfont(G.fonts);
			fmprstr(str);
			glDrawBuffer(GL_BACK);
			
			sa->head_swap= WIN_FRONT_OK;
		}
		sa= sa->next;
	}
}

/* ***************************************** */

int do_renderfg(int anim)
{
	ScrArea *oldarea=curarea;
	int retval;
	
	/* Nasty: the windows version doesn't reset the
	 * cursor if we set it to the wait cursor.	 */
	 
	#ifndef __WIN32
	waitcursor(1);
	#endif

	G.afbreek= 0;

	if(G.obedit && (G.scene->r.scemode & R_OGL)==0) {
		exit_editmode(0);	/* 0 = geen freedata */
	}

	bgntimer();
	if(anim) {
		animrender();
		do_global_buttons(B_NEWFRAME);
	}
	else {
		initrender();
		if(R.r.mode & R_FIELDS) do_global_buttons(B_NEWFRAME);
	}
	endtimer();

	curarea= oldarea;
	/* areawinset(G.curscreen->winakt); */

	R.flag= 0;
	
	waitcursor(0);

	free_filesel_spec(G.scene->r.pic);

	retval= G.afbreek==1;
	G.afbreek= 0;
	
	return retval;
}

void do_renderfg_seq()
{
	ScrArea *oldarea=curarea;
	
	/* only to be called from sequencer. Doesn't reset and set 'afbreek' */

	initrender();
	if(R.r.mode & R_FIELDS) do_global_buttons(B_NEWFRAME);

	curarea= oldarea;
	if(curarea) areawinset(curarea->win);

	R.flag= 0;
	
	free_filesel_spec(G.scene->r.pic);
}


