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


/* usiblender.c  jan 94		GRAPHICS
 * 
 * 
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "file.h"
#include "datatoc.h"

#ifdef __sgi
#include <sys/schedctl.h>
#endif
#include <signal.h>
#ifdef __FreeBSD__
#include <floatingpoint.h>
#include <sys/rtprio.h>
#endif

extern unsigned short fullscreen;
extern unsigned short borderless;
char bprogname[128];
UserDef U;


void exit_usiblender()
{
	extern char *fsmenu;	/* filesel.c */
	
	if(G.background && G.scene) {		/* network patch */
		end_camera_network();
		end_sectors();
	}
	freeAllRad();

	if(G.obedit) {
		if(G.obedit->type==OB_FONT) {
			free_editText();
		}
		else if(G.obedit->type==OB_MBALL) freelistN(&editelems);
		free_editmesh();
	}

	free_editLatt();

	free_blender();	/* blender.c, doet hele library */
	free_hashedgetab();

	FreeButs(0);
	free_ipocopybuf();
	freefastshade();
	free_vertexpaint();
	
	/* editnurb kan blijven bestaan buiten editmode */
	freeNurblist(&editNurb);

	if(fsmenu) freeN(fsmenu);
	
	free_render_data();
	free_filt_mask();
	
	if (txt_cut_buffer) freeN(txt_cut_buffer);
	
	end_python();
	
	if(totblock!=0) {
		printf("Error Totblck: %d\n",totblock);
		printmemlist();
	}
	delete_autosave();
	printf("Blender quit\n");
	
	exit(G.afbreek==1);
}



void initcursors()
{
	static unsigned short map[32];
	short a;

	/* cursor 1= kruisje
	 * cursor 2= wait
	 * cursor 3= vpaint
	 * cursor 4= faceselect
	 */

	for(a=0; a<32; a++) map[a]= 0;
	
	/* wait */
	for(a=0;a<16;a++) map[a]= 0xFFFF;
	
	/* kruisje */
	for(a=0;a<16;a++) map[a]= 256+128;
	map[7]=map[8]= 0xFFFF-512-256-128-64;
	map[6]=map[9]=0;

	/* vpaint */
	/* facesel */
	for(a=0;a<16;a++) map[a]= 256+128;
	for(a=16;a<32;a++) map[a]= 256+128;
	map[7]=map[8]= 0xFFFF-512-256-128-64;
	map[6]=map[9]=0;
	map[7+16]=map[8+16]= 0xFFFF-512-256-128-64;
	map[6+16]=map[9+16]=0;
	
	
}

void initbuttons()
{
	extern void *ipofont;
	struct ImBuf *bbuf, *pbuf;
	int *helvfont;
	char str[256];
	
	G.font= GLUT_BITMAP_HELVETICAB_12;
	G.fonts= GLUT_BITMAP_HELVETICAB_10;
	G.fontss= GLUT_BITMAP_HELVETICAB_8;
	
	AutoButFontSize(G.fonts, G.fontss);
	
	/* colors for headerbuttons */
	DefButCol(1, OVERDRAW, 0, 1,4,3,1, 1,1,1,1);
	DefButCol(2, RGBDRAW, 0x909090, 0xA0A0A0, 0, 0xFFFFFF, 0);	/* grijs */
	DefButCol(3, RGBDRAW, 0x909090, 0x80A080, 0, 0xFFFFFF, 0);	/* groen */
	DefButCol(4, RGBDRAW, 0x909090, 0xA08080, 0, 0xFFFFFF, 0);	/* blauw */
	DefButCol(5, RGBDRAW, 0x909090, 0x80A0B0, 0, 0xFFFFFF, 0);	/* zalm */
	DefButCol(6, RGBDRAW, 0x909090, 0x808080, 0, 0xFFFFFF, 0);	/* middelgrijs */
	DefButCol(7, RGBDRAW, 0x909090, 0x4040B0, 0, 0x0000FF, 0);	/* red alert */

	/* colors for blenderbuttons */
	DefButCol(10, RGBDRAW, 0x606060, 0x909090, 0, 0xFFFFFF, 0);	/* grijs */
	DefButCol(11, RGBDRAW, 0x606060, 0x949070, 0, 0xFFFFFF, 0);	/* groen */
	DefButCol(12, RGBDRAW, 0x606060, 0xA09090, 0, 0xFFFFFF, 0);	/* blauw */
	DefButCol(13, RGBDRAW, 0x606060, 0x8090A0, 0, 0xFFFFFF, 0);	/* zalm */
	DefButCol(14, RGBDRAW, 0x607070, 0x998892, 0, 0xFFFFFF, 0);	/* paars */

	/* IKONEN INLADEN */
	
	bbuf= loadiffmem((int*)datatoc_blenderbuttons, LI_rect);
	
	if(bbuf==0) {
		printf("Can't load buttonimage\n");
		exit(0);
	}
	
	DefButIcon(0, bbuf->rect, bbuf->x, bbuf->y, 20, 21);
	bbuf->rect= 0;
	freeImBuf(bbuf);

	ipofont= GLUT_BITMAP_HELVETICA_10;

	clear_matcopybuf();
}

void initpatterns()
{
	ushort pat[16], x;
}	


void breekaf(int sig)
{
	static int count = 0;
	
	G.afbreek = 2;
	
	if (sig == 2) {
		if (G.renderd == FALSE) {
			if (count) exit(2);
			printf("Press ^C again if once doesn't work\n");
		} else {
			if (count == 20) exit(2);
		}
		count++;
	}
}


void savecore()
{
	char scestr[FILE_MAXDIR], tstr[FILE_MAXFILE];
	extern int noBlog;
	extern void write_file();
	
	noBlog= 1;		
	strcpy(scestr, G.sce);	/* even bewaren */
	G.f |= G_DISABLE_OK;
	
	write_file("/usr/tmp/core.blend");
	
	G.f &= ~G_DISABLE_OK;
	strcpy(G.sce, scestr);
	noBlog= 0;
}

void fpe_handler (int sig)
{
	printf("SIGFPE trapped\n");
}

int main(argc,argv)	/* deze mainfunktie alleen voor editor, background krijgt andere */
int argc;
char **argv;
{
	Scene *sce;
	extern char DNAstr[];
	int a, stax, stay, sizx, sizy;
	char *cp, tstr[100];
	short dofork=1;
	
#ifdef WIN32	
	extern short forceborderless; /* To allow borderless on Windows */
#endif

#ifdef __FreeBSD__
	fpsetmask(0);
#endif
#if defined(__sgi)
	signal (SIGFPE, fpe_handler);
#endif

	/* voor play anim */
	strcpy(bprogname, argv[0]);

	initglobals();	/* blender.c */
	
	
	/* eerste testen op background */
	G.f |= G_SCENESCRIPT;
	for(a=1; a<argc; a++) {
		if(argv[a][0] == '-') {
			switch(argv[a][1]) {
			case 'a':
				playanim(argc-1, argv+1);
				exit(0);
				break;
			case 'b':
				if (strcmp(argv[a], "-badzr252")==0) {
					extern int badzr252;
					badzr252= 1;
					continue;
				}
				/* background met lage prioriteit */
				
#ifdef __sgi
				if (schedctl(NDPRI, 0, NDPLOMIN - 2) == -1) printf("no ndpri \n");
				/*if (schedctl(SLICE, 0, 1) == -1) printf("no slice \n");*/
				schedctl(SLICE, 0, 1);
#endif
				
			case 'B':
				/* background met normale prioriteit */

				G.background = 1;
				a= argc;
				break;

			case 'y':
				G.f &= ~G_SCENESCRIPT;
				break;

			case 'Y':
				printf ("-y was used to disable scene scripts because,\n");
				printf ("\t-p being taken, Ton was of the opinion that Y\n");
				printf ("\tlooked like a split (disabled) snake, and also\n");
				printf ("\twas similar to a python's tongue (unproven).\n\n");

				printf ("\tZr agreed because it gave him a reason to add a\n");
				printf ("\tcompletely useless text into Blender.\n\n");
				
				printf ("\tADDENDUM! Ton, in defense, found this picture of\n");
				printf ("\tan Australian python, exhibiting her (his/its) forked\n");
				printf ("\tY tongue. It could be part of an H Zr retorted!\n\n");
				printf ("\thttp://www.users.bigpond.com/snake.man/\n");
				
				exit(252);
				
			case 'h':			
				printf ("Blender V %d.%d\n", G.version/100, G.version%100);
				printf ("Usage: blender [options ...] [file]\n");
				
				printf ("\nRender options:\n");
				printf ("  -b <file>\tRender <file> in background\n");
				printf ("    -S <name>\tSet scene <name>\n");				
				printf ("    -f <frame>\tRender frame <frame> and save it\n");				
				printf ("    -s <frame>\tSet start to frame <frame> (use with -a)\n");
				printf ("    -e <frame>\tSet end to frame (use with -a)<frame>\n");
				printf ("    -a\t\tRender animation\n");

				printf ("\nAnimation options:\n");
				printf ("  -a <file(s)>\tPlayback <file(s)>\n");
				printf ("    -m\t\tRead from disk (Don't buffer)\n");

				printf ("\nWindow options:\n");
				printf ("  -w\t\tForce opening with borders\n");
#ifdef WIN32				
				printf ("  -W\t\tForce opening without borders\n");
#endif				
				printf ("  -p <sx> <sy> <w> <h>\tOpen with lower left corner at <sx>, <sy>\n");
				printf ("                      \tand width and height <w>, <h>\n");
				
				
				printf ("\nMisc options:\n");
				printf ("  -f\t\tPrevent forking in foreground mode\n");
				printf ("  -d\t\tTurn debugging on\n");
				printf ("  -h\t\tPrint this help text\n");
				printf ("  -y\t\tDisable OnLoad scene scripts, use -Y to find out why its -y\n");

				exit(0);
								
			default:
				break;
			}
		}
	}

#ifdef __sgi

	setuid(getuid()); /* einde superuser */
	
	/* Signalen zetten. Even wachten met USR1 en INT, want die kunnen
	 * ook het lezen van de file onderbreken. Geeft van die slordige
	 * foutmeldingen....
	 */
	
	sigset(SIGUSR2, savecore);	/* als er een SIGUSR2 gegeven word dump dan de file */
	sighold(SIGUSR1);
	sighold(SIGINT);
	sigset(SIGUSR1, breekaf);   /* nette kill gebruikt door [traces|render]daemon */
	sigset(SIGINT, breekaf);   /* nette kill gebruikt door [traces|render]daemon */

#endif /* __sgi */

	bzero(&cur_sdna, sizeof(struct SDNA));
	cur_sdna.data= DNAstr+4;
	cur_sdna.datalen= *( (int *)DNAstr);
	init_structDNA(&cur_sdna);

	init_render_data();	/* moet vooraan staan ivm R.winpos uit defaultfile */
	
	if(G.background==0) {
	
		a= 1;	/* patch zodat glutinit van de argumenten afblijft */

 		glutInit(&argc, argv);
		
		getdisplaysize();
		
		for(a=1; a<argc; a++) {
			if(argv[a][0] == '-') {
				switch(argv[a][1]) {
				case 'p':	/* prefsize */
					if (argc-a < 5) {
						printf ("-p requires four arguments\n");
						exit(1);
					}
					a++;
					stax= atoi(argv[a]);
					a++;
					stay= atoi(argv[a]);
					a++;
					sizx= atoi(argv[a]);
					a++;
					sizy= atoi(argv[a]);
	
					setprefsize(stax, stay, sizx, sizy);
					fullscreen=0;
/* 					a= argc; */
					break;
				case 'd':
					G.f |= G_DEBUG;		/* std output printf's */ 
					printf ("Blender V %d.%d\n", G.version/100, G.version%100);
					break;
				case 'f':
					argv[a][1]='F'; /* bypass frame rendering */
					dofork= 0;
					break;
				case 'w':
					borderless=0;
					break;

#ifdef WIN32					
				case 'W':
					forceborderless=1;
					break;
#endif
				}
			}
		}

		/* fork voordat X opstart */
		#if !defined(__BeOS) && !defined(WIN32) && !defined(MESA31)
		if (dofork) if (fork()) exit(0);
		#endif
		
		read_key();	/* has debug mode! */
		start_python();
				
		initpatterns();
		initscreen();	/* voor (visuele) snelheid, dit eerst, dan setscreen */
		initcursors();
		init_screen_cursors();
		initbuttons();
		start_autosave(0);
		init_draw_rects();	/* drawobject.c */
		init_gl_stuff();	/* drawview.c */
		myContextSetup();	/* mywindow.c */
		glutDrawInit();

		read_homefile();
		load_firstfont();
		if(G.main->vfont.first==0) {
			printf("can't find default vectorfont\n");
			exit(0);
		}

		readBlog();
		strcpy(G.lib, G.sce);

		
	}
	else {
		read_key();
		start_python();
	}

	init_filt_mask();
	
	/* OK we zijn er klaar voor */
	
#ifdef __sgi
	sigrelse(SIGUSR1);
	sigrelse(SIGINT);
#endif

	for(a=1; a<argc; a++) {
		if (G.afbreek==1) break;

		if(argv[a][0] == '-') {
			switch(argv[a][1]) {
			case 'p':	/* prefsize */
				a+= 4;
				break;
			case 'I':
				/* lees standard in voor nieuwe frames */
				/* gets geeft error op PC */
				#ifdef __sgi
				if (G.scene) {
					G.renderd = TRUE;

					G.real_sfra = SFRA;
					G.real_efra = EFRA;

					printf("blender: reading stdin\n");
					fflush(stdout);
					
					while (gets(tstr)) {
						if (G.afbreek==1) break;
						
						if (strcmp(tstr, "EXIT") == 0) {
							EFRA= G.real_efra;
							SFRA= G.real_sfra;
							exit_render_stuff();
							
						} else {
							SFRA = atoi(tstr);
							EFRA = SFRA;
							animrender();
						}
						
						/* zorg ervoor dat alle printf's aangekomen zijn */
						fflush(stdout);
						fflush(stderr);
						usleep(100);

						if (G.afbreek==1) break;
						/* zend signaal naar ouder dat ik klaar ben */
						if (getppid()) kill(getppid(), SIGUSR1);
					}
				}
				#endif
				break;
			case 'f':
				a++;
				if (G.scene) {
					G.real_sfra = SFRA;
					G.real_efra = EFRA;
					SFRA = atoi(argv[a]);
					EFRA = SFRA;
					animrender();
				}
				break;
			case 'a':
				if (G.scene) {
					G.real_sfra = SFRA;
					G.real_efra = EFRA;
					animrender();
					exit_render_stuff();
				}
				break;
			case 'S':
				if(++a < argc) {
					set_scene_name(argv[a]);
				}
				break;
			case 's':
				a++;
				if(G.scene) {
					if (a < argc) SFRA = atoi(argv[a]);
				}
				break;
			case 'e':
				a++;
				if(G.scene) {
					if (a < argc) EFRA = atoi(argv[a]);
				}
				break;
			}
		}
		else {
			read_file(argv[a]);
		}
	}
	
	if(G.background) exit_usiblender();
	setscreen(G.curscreen);
	
	if(G.main->scene.first==0) {
		sce= add_scene("1");
		set_scene(sce);
	}
	
	qenter(Q_FIRSTTIME, 1);	

#ifdef WIN32
	if (glutGetDepth()<=8) {
		error("Blender requires more than 256 colors to run!");
		exit(1);
	}
#endif

	glutMainLoop();

	return 0;
}

