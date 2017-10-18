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


/* 
 * playanim.c	blenderversie jan 98
 * 
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "screen.h"

#if !defined(__BeOS) && !defined(WIN32)
#include <sys/mman.h>	/* mapped memory */
#endif


/* ***************** gl_util.c ****************** */

short mousexN = 0, mouseyN = 0, *mousexNp = &mousexN , *mouseyNp = &mouseyN;
int qualN = 0, winN = 0, *qualNp = &qualN, *winNp = &winN, window = 0;

#define LSHIFT	(1<<0)
#define RSHIFT	(1<<1)
#define SHIFT	(LSHIFT | RSHIFT)
#define LALT	(1<<2)
#define RALT	(1<<3)
#define ALT	(LALT | RALT)
#define LCTRL	(1<<4)
#define RCTRL	(1<<5)
#define LMOUSE	(1<<16)
#define MMOUSE	(1<<17)
#define RMOUSE	(1<<18)
#define MOUSE	(LMOUSE | MMOUSE | RMOUSE)

extern short scrmousex, scrmousey;	/* uit screen.c */
extern unsigned short fullscreen;

int qreadN(val)
short *val;
{
	int event;
	static int qual = 0, mousex = 0, mousey = 0, win = 0;

	switch(event = screen_qread(val)){
	case LEFTMOUSE:
		if (*val) qual |= LMOUSE;
		else qual &= ~LMOUSE;
		break;
	case MIDDLEMOUSE:
		if (*val) qual |= MMOUSE;
		else qual &= ~MMOUSE;
		break;
	case RIGHTMOUSE:
		if (*val) qual |= RMOUSE;
		else qual &= ~RMOUSE;
		break;
	case LEFTSHIFTKEY:
		if (*val) qual |= LSHIFT;
		else qual &= ~LSHIFT;
		break;
	case RIGHTSHIFTKEY:
		if (*val) qual |= RSHIFT;
		else qual &= ~RSHIFT;
		break;
	case LEFTCTRLKEY:
		if (*val) qual |= LCTRL;
		else qual &= ~LCTRL;
		break;
	case RIGHTCTRLKEY:
		if (*val) qual |= RCTRL;
		else qual &= ~RCTRL;
		break;
	case LEFTALTKEY:
		if (*val) qual |= LALT;
		else qual &= ~LALT;
		break;
	case RIGHTALTKEY:
		if (*val) qual |= RALT;
		else qual &= ~RALT;
		break;
	case MOUSEX:
		break;
	case MOUSEY:
		if (mouseyNp) *mouseyNp = scrmousey;
		if (mousexNp) *mousexNp = scrmousex;
		break;
	case INPUTCHANGE:
		win = *val;
		if (win){
			qual = 0;
			if (get_mbut()&L_MOUSE)		qual |= LMOUSE;
			if (get_mbut()&M_MOUSE)		qual |= MMOUSE;
			if (get_mbut()&R_MOUSE)		qual |= RMOUSE;
			if (get_qual()&LR_SHIFTKEY)	qual |= LSHIFT;
			if (get_qual()&LR_ALTKEY)	qual |= LALT;
			if (get_qual()&LR_CTRLKEY)	qual |= LCTRL;
		}
		if (winNp) *winNp = win;
		break;
	}

	if (qualNp) *qualNp = qual;

	return(event);
}

/* ***************** gl_util.c ****************** */




typedef struct pict{
	struct pict *next, *prev;
	char *mem;
	int size;
	char *name;
	struct ImBuf *ibuf;
	struct anim *anim;
	int frame;
	int IB_flags;
}Pict;

struct ListBase _picsbase = {
    0,0};
struct ListBase *picsbase = &_picsbase;
struct ImBuf * tbuf = 0;
int warn = 0, fromdisk = FALSE;
float zoomx = 1.0 , zoomy = 1.0;
double ptottime = 0.0, swaptime = 0.04;
int halvex = FALSE;

void minmaxibuf(ibuf, x1, y1, x2, y2)
struct ImBuf *ibuf;
short *x1, *y1, *x2, *y2;
{
	int x, y, found, step;
	uint *rect;

	rect = ibuf->rect;
	found = FALSE;
	for (y = 0; y<ibuf->y ; y++){
		for (x = ibuf->x; x>0; x--){
			if (*rect++) {
				found = TRUE;
				break;
			}
		}
		if (found) break;
	}

	if (found == FALSE){
		*x1 = *x2 = *y1 = *y2 = 0;
		return;
	}

	*y1 = y;

	rect = ibuf->rect;
	rect += ibuf->x * ibuf->y - 1;

	found = FALSE;
	for (y = ibuf->y - 1; y >= 0 ; y--){
		for (x = ibuf->x; x>0; x--){
			if (*rect--) {
				found = TRUE;
				break;
			}
		}
		if (found) break;
	}

	*y2 = y;


	step = ibuf->x;
	found = FALSE;
	for (x = 0; x < ibuf->x ; x++){
		rect = ibuf->rect + x;
		for (y = ibuf->y; y > 0; y--){
			if (*rect) {
				found = TRUE;
				break;
			}
			rect += step;
		}
		if (found) break;
	}

	*x1 = x;

	step = ibuf->x;
	found = FALSE;
	for (x = ibuf->x - 1; x > 0; x--){
		rect = ibuf->rect + x;
		for (y = ibuf->y; y > 0; y--){
			if (*rect) {
				found = TRUE;
				break;
			}
			rect += step;
		}
		if (found) break;
	}

	*x2 = x;
}


int pupdate_time()
{
	struct tms voidbuf;
	static int ltime;
	int time;

	#ifdef __BeOS
	time= glut_system_time()/10000;
	#else
	time = times(&voidbuf);
	#endif

	ptottime += (time - ltime) / TIME_INTERVAL;
	ltime = time;
	return (ptottime < 0);
}

void getmouseco(mval)
short *mval;
{
	int ofsx, ofsy, sizex, sizey;
	static Device mdev[2]= {MOUSEX, MOUSEY};

	getorigin(&ofsx,&ofsy);
	getsize(&sizex,&sizey);

	getmouseco_sc(mval);

	if(mval[0] < 0) mval[0] = 0;
	if(mval[0] > sizex) mval[0] = sizex;
	
}

void toscreen(ibuf)
struct ImBuf *ibuf;
{
	short mval[2];
	int ofsx, ofsy;
	int sizex, sizey;
	int scrollx;

	if (ibuf == 0){
		printf("no ibuf !\n");
		return;
	}

	ofsx = zoomx * ibuf->xorig;
	ofsy = zoomy * ibuf->yorig;
	sizex = ibuf->x;
	sizey = ibuf->y;

	glRasterPos2f(-0.1, -0.1);
	
	if (IS_hamx(ibuf)) glDisable(GL_DITHER);
	else glEnable(GL_DITHER);
	
	#if defined(__BeOS) || defined(__WIN32)
	glDisable(GL_DITHER);
	#endif
	glDrawPixels(ibuf->x, ibuf->y, GL_RGBA, GL_UNSIGNED_BYTE, ibuf->rect);
	
	pupdate_time();
	if (warn && ptottime >= swaptime){
		cpack(-1);
		glIndexi(4095);
		glRectf(10,  10,  20,  20);
	}

	glutSwapBuffers();
}

void build_pict_list(char * first)
{
	int size,pic,file;
	char *mem, name[256];
	int type, event;
	short val;
	struct pict * picture = 0;
	struct ImBuf *ibuf = 0;
	int count = 0;
	char str[100];
	struct anim * anim;
	
	type = isanim(first);
	if (type == ANIM_SEQUENCE) type = 0;
	
	if (type) {
		anim = open_anim(first, ANIM_DFLT, IB_rect);
		if (anim) {
			ibuf = anim_absolute(anim, 0);
			if (ibuf) {
				toscreen(ibuf);
				freeImBuf(ibuf);
			}
			
			for (pic = 0; pic < anim->duration; pic ++) {
				picture = CLN(Pict);
				picture->anim = anim;
				picture->frame = pic;
				picture->IB_flags = IB_rect;
				sprintf(str, "%s : %d", first, pic + 1);
				picture->name = strdup(str);
				addtail(picsbase, picture);
			}
		} else printf("couldn't open anim %s\n", first);
	} else {
	
		strcpy(name,first);
	
		pupdate_time();
		ptottime = 1.0;
		
/*
     O_DIRECT
            If set, all reads and writes on the resulting file descriptor will
            be performed directly to or from the user program buffer, provided
            appropriate size and alignment restrictions are met.  Refer to the
            F_SETFL and F_DIOINFO commands in the fcntl(2) manual entry for
            information about how to determine the alignment constraints.
            O_DIRECT is a Silicon Graphics extension and is only supported on
            local EFS and XFS file systems.
*/
		while(type = ispic(name)){
			file = open(name, O_BINARY|O_RDONLY, 0);
			if (file < 0) return;
			picture = callocstruct(struct pict , 1);
			if (picture == 0){
				printf("Not enough memory for pict struct \n");
				close(file);
				return;
			}
			size = filesize(file);
			picture->size = size;
			picture->IB_flags = IB_rect;
						
			if ((type & JPG) == FALSE && fromdisk == FALSE) {
				mem=(char *)malloc(size);
				if (mem==0){
					printf("Couldn't get memory\n");
					close(file);
					free(picture);
					return;
				}
		
				if (read(file,mem,size) != size){
					printf("Error while reading %s\n",name);
					close(file);
					free(picture);
					free(mem);
					return;
				}
			} else mem = 0;
			
			picture->mem = mem;
			picture->name = strdup(name);
			close(file);
			addtail(picsbase,picture);
			count++;
			
			pupdate_time();
			
			if (ptottime > 1.0) {				
				if (picture->mem) ibuf = loadiffmem((int *) picture->mem, picture->IB_flags);
				else ibuf = loadiffname(picture->name, picture->IB_flags);
				if (ibuf) {
					toscreen(ibuf);
					freeImBuf(ibuf);
					glDrawBuffer(GL_FRONT);

					cpack(-1);
					glRasterPos2i(10,  10);
					sprintf(str, "%4d: %s", count, name);
					glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);
					glDrawBuffer(GL_BACK);
				}
				pupdate_time();
				ptottime = 0.0;
			}
			
			newname(name, +1);
			
			while(qtest()){
				switch(event = qreadN(&val)){
				case KEYBD:
					if (val == 27) return;
					break;
				}
			}
		}
	}
	return;
}

void reshapeanimfunc(int x, int y)
{
}


playanim(argc,argv)
int argc;
char **argv;
{
	struct ImBuf *ibuf = 0;
	struct pict *picture = 0;
	char name[256], colormap[256][4], * exe = 0, instr[256], killit[256], pupstr[256];
	short val = 0, go = TRUE, ibufx = 0, ibufy = 0, h_ibufx = 0;
	int event, win, stopped = FALSE, maxwinx, maxwiny;
	short c233 = FALSE, yuvx = FALSE, once = FALSE, sstep = FALSE, wait2 = FALSE, resetmap = FALSE, pause = 0;
	short pingpong = FALSE, direction = 1, next = 1, turbo = FALSE, doubleb = TRUE, noskip = FALSE;
	int sizex, sizey, ofsx, ofsy, i;
	pid_t child;
	extern int scalecmapY;
	struct anim * anim = 0;
	int type, a;
	
	int cd = FALSE;
	
	while (argc > 1) {
		if (argv[1][0] == '-'){
			switch(argv[1][1]) {
				case 'm':
					fromdisk = TRUE;
					break;
				case 'x':
					exe = argv[2];
					argc--;
					argv++;
					break;
				case 'p':
					sprintf(instr, "playaifc %s", argv[2]);
					exe = instr;
					argc--;
					argv++;
					pause = 16;
					break;
				default:
					printf("unknown option '%c': skipping\n", argv[1][1]);
					break;
			}
			argc--;
			argv++;
		} else break;
	}
	
	if (exe) {
		strcpy(killit, "/etc/killall ");
		strcat(killit, exe);
	}
	
	if (argc > 1) strcpy(name,argv[1]);
	else {
		getwdN(name);
		if (name[strlen(name)-1] != '/') strcat(name,"/");
	}
		
#ifdef __sgi
	initmoviepointers();
#endif

	type = isanim(name);
	if (type == ANIM_SEQUENCE) type = 0;

	if (type) {
		anim = open_anim(name, ANIM_DFLT, IB_rect);
		if (anim) {
			ibuf = anim_absolute(anim, 0);
			close_anim(anim);
		}
	} else if (ispic(name) == FALSE){
		exit(1);
	}

	if (ibuf == 0) ibuf = loadiffname(name, IB_rect);
	if (ibuf == 0){
		printf("couldn't open %s\n",name);
		exit(1);
	}
	
	#if !defined(__BeOS) && !defined(WIN32)
	if (fork()) exit(0);
	#endif
	
	fullscreen=0;
	
	a= 1;	/* patch zodat glutinit van de argumenten afblijft */
	glutInit(&a, argv);

	window = mywinopen(GLUT_RGB | GLUT_DOUBLE, 0, 0, ibuf->x, ibuf->y);

	glutReshapeFunc(reshapeanimfunc);
	do_the_glut_funcs();

	G.font= GLUT_BITMAP_HELVETICAB_12;
	G.fonts= GLUT_BITMAP_HELVETICAB_10;
	G.fontss= GLUT_BITMAP_HELVETICAB_8;
	
	ibufx = ibuf->x; h_ibufx = ibufx / 1.57; 
	ibufy = ibuf->y;
	
	maxwinx = displaysizex;
	maxwiny = displaysizey;
	
	if (maxwinx % ibuf->x) maxwinx = ibuf->x * (1 + (maxwinx / ibuf->x));
	if (maxwiny % ibuf->y) maxwiny = ibuf->y * (1 + (maxwiny / ibuf->y));
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	
	build_pict_list(name);
	
	for (i = 2; i < argc; i++){
		strcpy(name, argv[i]);
		build_pict_list(name);
	}

	freeImBuf(ibuf); 
	ibuf = 0;

	pupdate_time();
	ptottime = 0;

	while (go){
		if (pingpong) direction = -direction;

		if (direction == 1) picture = picsbase->first;
		else picture = picsbase->last;

		if (picture == 0){
			printf("couldn't find pictures\n");
			go = FALSE;
		}
		if (pingpong){
			if (direction == 1) picture = picture->next;
			else picture = picture->prev;
		}
		if (ptottime > 0.0) ptottime = 0.0;

		while (picture){
			if (ibuf != 0 && ibuf->type == 0) freeImBuf(ibuf);

			if (picture->ibuf) ibuf = picture->ibuf;
			else if (picture->anim) ibuf = anim_absolute(picture->anim, picture->frame);
			else if (picture->mem) ibuf = loadiffmem((int *) picture->mem, picture->IB_flags);
			else ibuf = loadiffname(picture->name, picture->IB_flags);
			
			if (ibuf){
				strcpy(ibuf->name, picture->name);
				
				while (pupdate_time()) usleep(1);
				ptottime -= swaptime;
				toscreen(ibuf);
			} /* else deleten */
			else {
				printf("error: can't play this image type\n");
				exit(0);
			}
			
			if (once){
				if (picture->next == 0) wait2 = TRUE;
				else if (picture->prev == 0) wait2 = TRUE;
			}
			
			next = direction;
			
			while (qtest() != 0 | wait2 != 0){
				if (wait2 && stopped) {
					stopped = FALSE;
				}
			
				event = qreadN(&val);
				/* printf("%d %d\n", event, val); */
				
				if (wait2){
					pupdate_time();
					ptottime = 0;
				}
				switch (event){
				case LEFTARROWKEY:
					if (val){
						sstep = TRUE;
						wait2 = FALSE;
						if (qualN & SHIFT) {
							picture = picsbase->first;
							next = 0;
						} else {
							next = -1;
						}
					}
					break;
				case DOWNARROWKEY:
					if (val){
						wait2 = FALSE;
						if (qualN & SHIFT) {
							next = direction = -1;
						} else {
							next = -10;
							sstep = TRUE;
						}
					}
					break;
				case RIGHTARROWKEY:
					if (val){
						sstep = TRUE;
						wait2 = FALSE;
						if (qualN & SHIFT) {
							picture = picsbase->last;
							next = 0;
						} else {
							next = 1;
						}
					}
					break;
				case UPARROWKEY:
					if (val){
						wait2 = FALSE;
						if (qualN & SHIFT) {
							next = direction = 1;
						} else {
							next = 10;
							sstep = TRUE;
						}
					}
					break;
				case LEFTMOUSE:
				case MOUSEY:
					if (qualN & LMOUSE){
						getsize(&sizex,&sizey);
						picture = picsbase->first;
						i = 0;
						while (picture){
							i ++;
							picture = picture->next;
						}
						i = (i * (mousexN)) / sizex;
						picture = picsbase->first;
						for (; i > 0; i--){
							if (picture->next == 0) break;
							picture = picture->next;
						}
						sstep = TRUE;
						wait2 = FALSE;
						next = 0;
					}
					break;
				case ESCKEY:
					go= FALSE;
					break;
				case EQUALKEY:
					if (val) {
						if (qualN & SHIFT) {
							pause ++;
							printf("pause:%d\n", pause);
						} else swaptime /= 1.1;
					}
					break;
				case MINUSKEY:
					if (val) {
						if (qualN & SHIFT) {
							pause --;
							printf("pause:%d\n", pause);
						} else swaptime *= 1.1;
					}
					break;
				case PAD0:
					if (val){
						if (once) once = wait2 = FALSE;
						else {
							picture = 0;
							once = TRUE;
							wait2 = FALSE;
						}
					}
					break;
				case PADENTER:
					if (val){
						wait2 = sstep = FALSE;
						if (once && picture && picture->prev == 0) {
							if (cd){
								/*CDplayabs(cd, 5, 35, 0, 1);*/
							} else if (exe) {
							#if !defined(FREE) && !defined(__BeOS) && !defined(WIN32)
								system(killit);
								wait(0);								
								if ((child = fork()) == 0) {
									if (pause < 0) usleep(-pause);
									system(exe);
									exit(0);
								}
								if (pause > 0) usleep(pause);
								pupdate_time();
								ptottime = 0;
							#endif
							}
						}
					}
					break;
				case PADPERIOD:
					if (val){
						if (sstep) wait2 = FALSE;
						else {
							sstep = TRUE;
							wait2 = !wait2;
						}
					}
					break;
				case PAD1:
					swaptime = 1.0 / 60.0;
					break;
				case PAD2:
					swaptime = 1.0 / 50.0;
					break;
				case PAD3:
					swaptime = 1.0 / 30.0;
					break;
				case PAD4:
					swaptime = 1.0 / 25.0;
					break;
				case PAD5:
					swaptime = 1.0 / 20.0;
					break;
				case PAD6:
					swaptime = 1.0 / 15.0;
					break;
				case PAD7:
					swaptime = 1.0 / 12.0;
					break;
				case PAD8:
					swaptime = 1.0 / 10.0;
					break;
				case PAD9:
					swaptime = 1.0 / 6.0;
					break;
				case PADPLUSKEY:
					if (val == 0) break;
					zoomx += 2.0;
					zoomy += 2.0;
				case PADMINUS:
					if (val == 0) break;
					if (zoomx > 1.0) zoomx -= 1.0;
					if (zoomy > 1.0) zoomy -= 1.0;
					getorigin(&ofsx,&ofsy);
					getsize(&sizex,&sizey);
					ofsx += sizex/2;
					ofsy += sizey/2;
					sizex = zoomx * ibufx;
					sizey = zoomy * ibufy;
					ofsx -= sizex/2;
					ofsy -= sizey/2;
				case REDRAW:
					getsize(&sizex,&sizey);
					zoomx = (float) sizex / ibufx;
					zoomy = (float) sizey / ibufy;
					zoomx = floorf(zoomx + 0.5);
					zoomy = floorf(zoomy + 0.5);
					if (zoomx < 1.0) zoomx = 1.0;
					if (zoomy < 1.0) zoomy = 1.0;

					sizex = zoomx * ibufx;
					sizey = zoomy * ibufy;

					glViewport(0,  0, ( sizex-1)-(0)+1, ( sizey-1)-( 0)+1);
					glScissor(0,  0, ( sizex-1)-(0)+1, ( sizey-1)-( 0)+1);
					glPixelZoom(zoomx, zoomy);
					ptottime = 0.0;
					toscreen(ibuf);
					while (qtest()) qreadN(&val);
					break;
				case WINCLOSE:
				case WINQUIT:
					go = FALSE;
					break;
				case KEYBD:
					switch(val){
					case 27:
						go = FALSE;
						break;
					case 'a':
					case 'A':
						if (noskip) noskip = FALSE;
						else noskip = TRUE;
						break;
					case 'p':
					case 'P':
						if (pingpong) pingpong = 0;
						else pingpong = 1;
						break;
					case 't':
						break;
					case '?':
						if (ibuf) {
							sprintf(pupstr, " Name: %s | Speed: %.2f frames/s", ibuf->name, 1.0 / swaptime);
							notice(pupstr);
						}
						break;
					case 'w':
					case 'W':
						warn = (warn == 0);
						break;
						break;
					case 'y':
						if (scalecmapY) {
						    scalecmapY = FALSE;
						    ibufy /= 2;
						} else {
						    scalecmapY = TRUE;
						    ibufy *= 2;
						}
						printf("scalecmapY %d\n",  scalecmapY);
						
						break;
					case '/':
						swaptime = 1.0 / 5.0;
						break;
					}
				}
				if (go == FALSE) break;
			}
			
			wait2 = sstep;
			
			if (wait2 == 0 && stopped == 0) {
				stopped = TRUE;
			}

			pupdate_time();
					
			if (picture && next) {
				/* altijd minstens 1 stap zetten */
				while (picture){
					if (next < 0) picture = picture->prev;
					else picture = picture->next;
	
					if (once && picture != 0){
						if (picture->next == 0) wait2 = TRUE;
						else if (picture->prev == 0) wait2 = TRUE;
					}

					if (wait2 || ptottime < swaptime || turbo || noskip) break;
					ptottime -= swaptime;
				}
				if (picture == 0 && sstep) {
					if (next < 0) picture = picsbase->last;
					else if (next > 0) picture = picsbase->first;									
				}
			}
			if (go == FALSE) break;
		}
	}
}

/* #endif */

