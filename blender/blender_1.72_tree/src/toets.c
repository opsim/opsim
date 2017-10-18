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


/*  toets.c   mei 94     GRAPHICS
 * 
 *  Algemene toetsen, bijzondere in de space.c
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "render.h"



void persptoetsen(ushort event)
{
	static Object *oldcamera=0;
	Base *base;
	float zoom, phi, si, q1[4], vec[3], dx=0, dy=0;
	static int perspo=1;
	int ok=0;
	
	short mval[2],rt2,rt3;

	if(G.qual & LR_SHIFTKEY) {
		if(event==PAD0) {
			/* G.vd->persp= 3; */
		}
		else if(event==PAD7) {
			G.vd->viewquat[0]= 0.0;
			G.vd->viewquat[1]= -1.0;
			G.vd->viewquat[2]= 0.0;
			G.vd->viewquat[3]= 0.0;
			G.vd->view= 7;
		}
		else if(event==PAD1) {
			G.vd->viewquat[0]= 0.0;
			G.vd->viewquat[1]= 0.0;
			G.vd->viewquat[2]= -fcos(M_PI/4.0);
			G.vd->viewquat[3]= -fcos(M_PI/4.0);
			G.vd->view=1;
		}
		else if(event==PAD3) {
			G.vd->viewquat[0]= 0.5;
			G.vd->viewquat[1]= -0.5;
			G.vd->viewquat[2]= 0.5;
			G.vd->viewquat[3]= 0.5;
			G.vd->view=3;
		}
		else if(event==PADMINUS) {
			/* deze min en max staan ook in viewmove() */
			if(G.vd->persp==2) {
				if((G.scene->r.scemode & R_NETWORK)) {
					next_camera(-1);
				} else {
					G.vd->camzoom-= 10;
					if(G.vd->camzoom<-30) G.vd->camzoom= -30;
				}
			}
			else if(G.vd->dist<10.0*G.vd->clipend) G.vd->dist*=1.2;
		}
		else if(event==PADPLUSKEY) {
			if(G.vd->persp==2) {
				if((G.scene->r.scemode & R_NETWORK)) {
					next_camera(0);
				} else {
					G.vd->camzoom+= 10;
					if(G.vd->camzoom>300) G.vd->camzoom= 300;
				}
			}
			else if(G.vd->dist> 0.001*G.vd->grid) G.vd->dist*=.83333;
		}
		else {

			initgrabz(0.0, 0.0, 0.0);
			
			if(event==PAD6) window_to_3d(vec, -32, 0);
			else if(event==PAD4) window_to_3d(vec, 32, 0);
			else if(event==PAD8) window_to_3d(vec, 0, -25);
			else if(event==PAD2) window_to_3d(vec, 0, 25);
			G.vd->ofs[0]+= vec[0];
			G.vd->ofs[1]+= vec[1];
			G.vd->ofs[2]+= vec[2];
		}
	}
	else {

		if(event==PAD7) {
			G.vd->viewquat[0]= 1.0;
			G.vd->viewquat[1]= 0.0;
			G.vd->viewquat[2]= 0.0;
			G.vd->viewquat[3]= 0.0;
			G.vd->view=7;
			if(G.vd->persp>=2) G.vd->persp= perspo;
		}
		else if(event==PAD1) {
			G.vd->viewquat[0]= fcos(M_PI/4.0);
			G.vd->viewquat[1]= -fsin(M_PI/4.0);
			G.vd->viewquat[2]= 0.0;
			G.vd->viewquat[3]= 0.0;
			G.vd->view=1;
			if(G.vd->persp>=2) G.vd->persp= perspo;
		}
		else if(event==PAD3) {
			G.vd->viewquat[0]= 0.5;
			G.vd->viewquat[1]= -0.5;
			G.vd->viewquat[2]= -0.5;
			G.vd->viewquat[3]= -0.5;
			G.vd->view=3;
			if(G.vd->persp>=2) G.vd->persp= perspo;
		}
		else if(event==PADMINUS) {
			/* deze min en max staan ook in viewmove() */
			if(G.vd->persp==2) {
				G.vd->camzoom-= 5;
				if(G.vd->camzoom<-30) G.vd->camzoom= -30;
			}
			else if(G.vd->dist<10.0*G.vd->clipend) G.vd->dist*=1.2;
		}
		else if(event==PADPLUSKEY) {
			if(G.vd->persp==2) {
				G.vd->camzoom+= 5;
				if(G.vd->camzoom>300) G.vd->camzoom= 300;
			}
			else if(G.vd->dist> 0.001*G.vd->grid) G.vd->dist*=.83333;
		}
		else if(event==PAD5) {
			if(G.vd->persp==1) G.vd->persp=0;
			else G.vd->persp=1;
		}
		else if(event==PAD0) {
			if(G.qual & LR_ALTKEY) {
				if(oldcamera) {
					base= FIRSTBASE;
					while(base) {
						if(base->object==oldcamera) {
							G.vd->camera= oldcamera;
							break;
						}
						base= base->next;
					}
				}
				
				handle_view3d_lock();
			}
			else if(BASACT) {
				if(G.qual & LR_CTRLKEY) {
					if(G.vd->camera != OBACT) {
						
						if(G.vd->camera && G.vd->camera->type==OB_CAMERA) oldcamera= G.vd->camera;
						
						G.vd->camera= OBACT;
						handle_view3d_lock();
					}
				}
				else if(G.vd->camera==0 && OBACT->type==OB_CAMERA) {
					G.vd->camera= OBACT;
					handle_view3d_lock();
				}
			}
			if(G.vd->camera==0) {
				G.vd->camera= find_camera();
				handle_view3d_lock();
			}
			
			if(G.vd->camera) {
				G.vd->persp= 2;
				G.vd->view= 0;
			}
			
		}
		else if(event==PAD9) {
			countall();
			do_all_ipos();
			do_all_keys();
			do_all_ikas();
			reset_slowparents();	/* editobject.c */
		}
		else if(G.vd->persp<2) {
			if(event==PAD4 || event==PAD6) {
				/* z-as */
				phi= M_PI/24.0;
				if(event==PAD6) phi= -phi;
				si= fsin(phi);
				q1[0]= fcos(phi);
				q1[1]= q1[2]= 0.0;
				q1[3]= si;
				QuatMul(G.vd->viewquat, G.vd->viewquat, q1);
				G.vd->view= 0;
			}
			if(event==PAD2 || event==PAD8) {
				
				/* liggende as */
				VECCOPY(q1+1, G.vd->viewinv[0]);
				
				Normalise(q1+1);
				phi= M_PI/24.0;
				if(event==PAD2) phi= -phi;
				si= fsin(phi);
				q1[0]= fcos(phi);
				q1[1]*= si;
				q1[2]*= si;
				q1[3]*= si;
				QuatMul(G.vd->viewquat, G.vd->viewquat, q1);
				G.vd->view= 0;
			}
		}

		if(G.vd->persp<2) perspo= G.vd->persp;
	}
	addqueue(curarea->win, REDRAW, 1);
	addqueue(curarea->headwin, REDRAW, 1);
}

int untitled(char * name)
{
	if (G.save_over == 0 && U.versions == 0) {
		char * c;
		
/* 		c = strrchr(name, '/'); */
		c= last_slash(name);
		if (c) c[1] = 0;
		strcat(name, "untitled.blend");
		return(TRUE);
	}
	return(FALSE);
}

int save_image_filesel_str(char *str)
{
	switch(G.scene->r.imtype) {
	case R_TARGA:
		strcpy(str, "SAVE TARGA"); return 1;
	case R_RAWTGA:
		strcpy(str, "SAVE RAW TARGA"); return 1;
	case R_IRIS:
		strcpy(str, "SAVE IRIS"); return 1;
	case R_IRIZ:
		strcpy(str, "SAVE IRIS"); return 1;
	case R_HAMX:
		strcpy(str, "SAVE HAMX"); return 1;
	case R_FTYPE:
		strcpy(str, "SAVE FTYPE"); return 1;
	case R_JPEG90:
		strcpy(str, "SAVE JPEG"); return 1;
	default:
		strcpy(str, "SAVE IMAGE"); return 0;
	}	
}

int blenderqread(ushort event, short val)
{
	/* hier alle algemene toetsafhandelingen (niet screen/window/space) */
	/* return 0: niet aan andere queue's doorgeven */
	extern char videosc_dir[];
	extern void read_file(), write_file(), write_image(), write_videoscape(), write_blendpsx();
	extern int textediting;
	SpaceButs *buts;
	ScrArea *sa;
	View3D *vd;
	Object *ob;
	uint *temp;
	int ok, textspace=0;
	char dir[FILE_MAXDIR], str[FILE_MAXFILE];
	
	if(val==0) return 1;
	if(event==MOUSEY || event==MOUSEX) return 1;

	if (curarea && curarea->spacetype==SPACE_TEXT) textspace= 1;

	switch(event) {

	case F1KEY:
		if(G.qual==0) {
			/* this exception because of the '?' button */
			if(curarea->spacetype==SPACE_INFO) {
				sa= closest_bigger_area();
				areawinset(sa->win);
			}
			
			activate_fileselect(FILE_BLENDER, "LOAD FILE", G.sce, read_file);
			return 0;
		}
		else if(G.qual & LR_SHIFTKEY) {
			activate_fileselect(FILE_LOADLIB, "LOAD LIBRARY", G.lib, 0);
			return 0;
		}
		break;
	case F2KEY:
		if(G.qual==0) {
			strcpy(dir, G.sce);
			untitled(dir);
			activate_fileselect(FILE_BLENDER, "SAVE FILE", dir, write_file);
			return 0;
		}
		else if(G.qual & LR_CTRLKEY) {
			write_vrml_fs();
			return 0;
		}
		else if(G.qual & LR_SHIFTKEY) {
			write_dxf_fs();
			return 0;
		}
		break;
	case F3KEY:
		if(G.qual==0) {
			if(!R.rectot) {
				error("No image rendered");
			} else {
				if(G.ima[0]==0) {
					strcpy(dir, G.sce);
					splitdirstring(dir, str);
					strcpy(G.ima, dir);
				}
				
				R.r.imtype= G.scene->r.imtype;
				R.r.quality= G.scene->r.quality;
				R.r.planes= G.scene->r.planes;
			
				if(!save_image_filesel_str(str)) {
					error("Select an image type in DisplayButtons(F10)");
					return 0;
				}
			
				activate_fileselect(FILE_SPECIAL, str, G.ima, write_image);
				return 0;
			}
		}
		else if(G.qual & LR_CTRLKEY) {
			screendump();
		}
		break;
	case F4KEY:
		if(G.qual & LR_SHIFTKEY) {
			SpaceFile *sfile= curarea->spacedata.first;
			/* op zoek naar een bestaande dataselect */
			while(sfile) {
				if(sfile->spacetype==SPACE_FILE) {
					if(sfile->type==FILE_MAIN) {
						newspace(curarea, SPACE_FILE);
						freefilelist(sfile);
			
						sfile->returnfunc= 0;
						sfile->title[0]= 0;
						
						return 0;
					}
				}
				sfile= sfile->next;
			}
			
			bzero(str, 16);
			ob= OBACT;
			if(ob) strcpy(str, ob->id.name);

			activate_fileselect(FILE_MAIN, "DATA SELECT", str, 0);
			return 0;
		}
		else extern_set_butspace(event);
		
		break;
	case F5KEY:
		if(G.qual & LR_SHIFTKEY) {
			addqueue(curarea->headwin, REDRAW, 1);
			curarea->butspacetype= SPACE_VIEW3D;
			do_global_buttons(B_NEWSPACE);
			return 0;
		}
		else extern_set_butspace(event);
		break;
	case F6KEY:
		if(G.qual & LR_SHIFTKEY) {
			addqueue(curarea->headwin, REDRAW, 1);
			curarea->butspacetype= SPACE_IPO;
			do_global_buttons(B_NEWSPACE);
			return 0;
		}
		else extern_set_butspace(event);
		break;
	case F7KEY:
		if(G.qual & LR_SHIFTKEY) {
			addqueue(curarea->headwin, REDRAW, 1);
			curarea->butspacetype= SPACE_BUTS;
			do_global_buttons(B_NEWSPACE);
			return 0;
		}
		else extern_set_butspace(event);
		break;
	case F8KEY:
		if(G.qual & LR_SHIFTKEY) {
			addqueue(curarea->headwin, REDRAW, 1);
			curarea->butspacetype= SPACE_SEQ;
			do_global_buttons(B_NEWSPACE);
			return 0;
		}
		else extern_set_butspace(event);
		break;
	case F9KEY:
		if(G.qual & LR_SHIFTKEY) {
			addqueue(curarea->headwin, REDRAW, 1);
			curarea->butspacetype= SPACE_OOPS;
			do_global_buttons(B_NEWSPACE);
			return 0;
		}
		else extern_set_butspace(event);
		break;
	case F10KEY:
		if(G.qual & LR_SHIFTKEY) {
			addqueue(curarea->headwin, REDRAW, 1);
			curarea->butspacetype= SPACE_IMAGE;
			do_global_buttons(B_NEWSPACE);
			return 0;
		}
		else extern_set_butspace(event);
		break;
	case F11KEY:
		if(G.qual & LR_SHIFTKEY) {
			addqueue(curarea->headwin, REDRAW, 1);
			curarea->butspacetype= SPACE_TEXT;
			do_global_buttons(B_NEWSPACE);
			return 0;
		}
		else toggle_render_display();
		return 0;
		break;
	case F12KEY:
		if(G.qual & LR_SHIFTKEY) {
			addqueue(curarea->headwin, REDRAW, 1);
			curarea->butspacetype= SPACE_IMASEL;
			do_global_buttons(B_NEWSPACE);
			return 0;
		}
		else do_renderfg(0);
		return 0;
		break;
	
	case LEFTARROWKEY:
	case DOWNARROWKEY:
		if(textediting==0 && textspace==0) {
			if(event==DOWNARROWKEY) CFRA-= 10;
			else CFRA--;
			
			if(G.qual & 3) CFRA= SFRA;
			if(CFRA<1) CFRA=1;
	
			do_global_buttons(B_NEWFRAME);
			return 0;
		}
		break;

	case RIGHTARROWKEY:
	case UPARROWKEY:
		if(textediting==0 && textspace==0) {
			if(event==UPARROWKEY) CFRA+= 10;
			else CFRA++;
			if(G.qual & 3) CFRA= EFRA;
			
			do_global_buttons(B_NEWFRAME);
		}
		break;
	
	case ESCKEY:
		/* event doorgeven aan filesel? */
		if(G.curscreen->winakt!=R.win && curarea->spacetype==SPACE_FILE) return 1;
		
		if(R.win && R.winpop==0) {
			toggle_render_display();
			return 0;
		}
		else if(R.rectot) {
			ok= 0;
			sa= G.curscreen->areabase.first;
			while(sa) {
				if(sa->spacetype==SPACE_VIEW3D) {
					vd= sa->spacedata.first;
					if(vd->flag & V3D_DISPIMAGE) {
						addqueue(sa->win, REDRAW, 1);
						ok= 1;
						break;
					}
				}
				sa= sa->next;
			}
			if(ok) return 0;
		}
		break;
	case TABKEY:
		if(G.qual==0 ) {
			if(textspace==0) {
				if(curarea->spacetype==SPACE_IPO) set_editflag_editipo();
				else if(curarea->spacetype==SPACE_SEQ) enter_meta();
				else if(G.vd) {
					/* ook als Alt-E */
					if(G.obedit==0) enter_editmode();
					else exit_editmode(1);
				}
				return 0;
			}
		}
		else if(G.f & G_DEBUG) {
			printf("swap\n");
			
			glutSwapBuffers();
		}
		break;

	case BACKSPACEKEY:
		break;

	case AKEY:
		if(textediting==0 && textspace==0) {
			if(G.qual & LR_ALTKEY) {
				if(G.qual & LR_SHIFTKEY) play_anim(1);
				else play_anim(0);
				return 0;
			}
		}
		break;
	case EKEY:
		if(G.qual & LR_ALTKEY) {
			if(G.vd && textspace==0) {
				if(G.obedit==0) enter_editmode();
				else exit_editmode(1);
				return 0;
			}			
		}
		break;
	case IKEY:
		if(textediting==0 && textspace==0 && curarea->spacetype!=SPACE_FILE && curarea->spacetype!=SPACE_IMASEL) {
			if(G.qual==0) {
				common_insertkey();
				return 0;
			}
		}
		break;
	case JKEY:
		if(textediting==0 && textspace==0) {
			if(R.rectot && G.qual==0) {
				
				if(R.rectspare==0) {
					R.rectspare= (uint *)callocN(sizeof(int)*R.rectx*R.recty, "rectot");
					R.sparex= R.rectx;
					R.sparey= R.recty;
				}
				else if(R.sparex!=R.rectx || R.sparey!=R.recty) {
					temp= (uint *)callocN(sizeof(int)*R.rectx*R.recty, "rectot");
					
					scalefastrect(R.rectspare, temp, R.sparex, R.sparey, R.rectx, R.recty);
					freeN(R.rectspare);
					R.rectspare= temp;
					
					R.sparex= R.rectx;
					R.sparey= R.recty;
				}
				
				SWAP(uint *, R.rectspare, R.rectot);
				
				render_display(0, R.recty-1);
				
				return 0;
			}
		}
		break;

	case NKEY:
		if(textediting==0 && textspace==0 ) {
			if(G.qual & LR_CTRLKEY);
			else if(G.qual==0 || (G.qual & LR_SHIFTKEY)) {
				clever_numbuts();
				return 0;
			}
		}
		break;
		
	case OKEY:
		if(textediting==0) {
			if(G.qual & LR_CTRLKEY) {
				sprintf(str, "Open file: %s", G.sce);
			
				if(okee(str)) {
					strcpy(dir, G.sce);
					read_file(dir);
				}
				return 0;
			}
		}
		break;
		
	case SKEY:
		if(G.obedit==0) {
			if(G.qual & LR_CTRLKEY) {
				if(G.qual & LR_SHIFTKEY);
				else {
					strcpy(dir, G.sce);
					write_file(dir);
					free_filesel_spec(dir);
					return 0;
				}
			}
		}
		break;
	
	case TKEY:
		if(G.qual & LR_ALTKEY) {
		if(G.qual & LR_CTRLKEY) {
			int a, time, event;
			
			event= pupmenu("10 Timer%t|draw|draw+swap");
			
			if(event>0) {
				printf("start timer\n");
				waitcursor(1);
				
				start_timer();
					
				for(a=0; a<10; a++) {
					curarea->windraw();
					if(event==2) screen_swapbuffers();
				}
				
				end_timer(&time, 0);
				
				if(event==1) printf("draw %d\n", time);
				if(event==2) printf("d+sw %d\n", time);
				
				waitcursor(0);
				return 0;
			}
		}}
		break;
				
	case UKEY:
		
		if(textediting==0) {
			if(G.qual & LR_CTRLKEY) {
				if(okee("SAVE USER DEFAULTS")) {
					if( write_homefile()==0) error("Can't write ~/.B.blend");
				}
				return 0;
			}
		}
		break;
		
	case WKEY:
		if(textediting==0) {
			if(G.qual & LR_CTRLKEY) {
				if(G.qual & LR_SHIFTKEY);
				else {
					strcpy(dir, G.sce);
					if (untitled(dir)) {
						activate_fileselect(FILE_BLENDER, "SAVE FILE", dir, write_file);
					} else {
						write_file(dir);
						free_filesel_spec(dir);
					}
					return 0;
				}
			}
			else if(G.qual & LR_ALTKEY) {
				write_videoscape_fs();
			}
		}
		break;
		
	case XKEY:
		if(G.qual & LR_CTRLKEY) {
			if(okee("ERASE ALL")) {
				if( read_homefile()==0) error("No file ~/.B.blend");
			}
			return 0;
		}
		
		break;
	case ZKEY:
		if(R.win && R.win==G.curscreen->winakt) {
			zoomwin();
			return 0;
		}
	}
	
	return 1;
}

