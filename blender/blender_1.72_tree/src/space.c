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


/*  space.c   jan/juli 94     GRAPHICS
 * 
 *  - hier het initialiseren en vrijgeven van SPACE data
 * 
 */

#include "blender.h"
#include "sequence.h"
#include "graphics.h"

extern void defheaddraw();	/* screen.c */

void force_draw();
void set_func_space(ScrArea *sa);
void allqueue(ushort event, short val);


void drawemptyspace()
{

	glClearColor(0.5, 0.5, 0.5, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT);
	
}

/* ************* SPACE: VIEW3D  ************* */

extern void drawview3d();


void copy_view3d_lock(short val)
{
	View3D *vd;
	ScrArea *sa;
	bScreen *sc;
	int bit;
	
	/* van G.scene naar andere views kopieeren */
	sc= G.main->screen.first;
	
	while(sc) {
		if(sc->scene==G.scene) {
			sa= sc->areabase.first;
			while(sa) {

				vd= sa->spacedata.first;
				while(vd) {
					if(vd->spacetype==SPACE_OOPS && val==REDRAW) {
						if(sa->win) addqueue(sa->win, REDRAW, 1);
					}
					else if(vd->spacetype==SPACE_VIEW3D && vd->scenelock) {
						if(vd->localview==0) {
							vd->lay= G.scene->lay;
							vd->camera= G.scene->camera;
							
							if(vd->camera==0 && vd->persp>1) vd->persp= 1;
							
							if( (vd->lay & vd->layact) == 0) {
								bit= 0;
								while(bit<32) {
									if(vd->lay & (1<<bit)) {
										vd->layact= 1<<bit;
										break;
									}
									bit++;
								}
							}
							
							if(val==REDRAW && vd==sa->spacedata.first) {
								if(sa->win) addqueue(sa->win, REDRAW, 1);
								if(sa->headwin) addqueue(sa->headwin, REDRAW, 1);
							}
						}
					}
					vd= vd->next;
				}
				sa= sa->next;
			}
		}
		sc= sc->id.next;
	}
}

void handle_view3d_lock()
{

	if(G.vd->localview==0 && G.vd->scenelock && curarea->spacetype==SPACE_VIEW3D) {

		/* naar scene kopieeren */
		G.scene->lay= G.vd->lay;
		G.scene->camera= G.vd->camera;
	
		copy_view3d_lock(REDRAW);
	}
}


void changeview3d()
{
	
	setwinmatrixview3d(0);	/* 0= geen pick rect */
	
}


void winqread3d(ushort event, short val)
{
	Object *ob;
	static int padevent= 0;
	float *curs;
	int doredraw= 0, textedit, pupval;
	
	if(curarea->win==0) return;	/* hier komtie vanuit sa->headqread() */
	if(event==MOUSEY) return;
	
	if(val) {
		/* TEXTEDITING?? */
		if(G.obedit && G.obedit->type==OB_FONT) {
			switch(event) {
			
			case LEFTMOUSE:
				mouse_cursor();
				break;
			case MIDDLEMOUSE:
				if(U.flag & VIEWMOVE) {
					if(G.qual & LR_SHIFTKEY) viewmove(0);
					else if(G.qual & LR_CTRLKEY) viewmove(2);
					else viewmove(1);
				}
				else {
					if(G.qual & LR_SHIFTKEY) viewmove(1);
					else if(G.qual & LR_CTRLKEY) viewmove(2);
					else viewmove(0);
				}
			case UKEY:
				if(G.qual & LR_ALTKEY) {
					remake_editText();
					doredraw= 1;
					padevent= 2;
				}
				break;
			case KEYBD:
				if(padevent==0) {
					do_textedit(event, val);
				}
				break;
			case PAD0: case PAD1: case PAD2: case PAD3: case PAD4:
			case PAD5: case PAD6: case PAD7: case PAD8: case PAD9:
			case PADENTER:
				/* hier is PADPLUSKEY en PADMINUS weg vanwege de 'KEYBD' event */
				persptoetsen(event);
				padevent= 2;
				doredraw= 1;
				break;
				
			default:
				do_textedit(event, val);
				break;
			}
			if(padevent) padevent--;
		}
		else {
			switch(event) {
			
			case BACKBUFDRAW:
				backdrawview3d(1);
				break;
				
			case LEFTMOUSE:
				if(G.f & G_VERTEXPAINT) vertex_paint();
				else mouse_cursor();
				break;
			case MIDDLEMOUSE:
				if(U.flag & VIEWMOVE) {
					if(G.qual & LR_SHIFTKEY) viewmove(0);
					else if(G.qual & LR_CTRLKEY) viewmove(2);
					else viewmove(1);
				}
				else {
					if(G.qual & LR_SHIFTKEY) viewmove(1);
					else if(G.qual & LR_CTRLKEY) viewmove(2);
					else viewmove(0);
				}
				break;
			case RIGHTMOUSE:
				if(G.obedit && (G.qual & LR_CTRLKEY)==0) {
					if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) mouse_mesh();
					else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) mouse_nurb();
					else if(G.obedit->type==OB_MBALL) mouse_mball();
					else if(G.obedit->type==OB_LATTICE) mouse_lattice();
				}
				else if( G.qual & LR_CTRLKEY ) mouse_select();
				else if(G.f & G_FACESELECT) face_select();
				else if( G.f & G_VERTEXPAINT) sample_vpaint();
				else mouse_select();
				break;
			
			case ONEKEY:
				do_layer_toets(0); break;
			case TWOKEY:
				do_layer_toets(1); break;
			case THREEKEY:
				do_layer_toets(2); break;
			case FOURKEY:
				do_layer_toets(3); break;
			case FIVEKEY:
				do_layer_toets(4); break;
			case SIXKEY:
				do_layer_toets(5); break;
			case SEVENKEY:
				do_layer_toets(6); break;
			case EIGHTKEY:
				do_layer_toets(7); break;
			case NINEKEY:
				do_layer_toets(8); break;
			case ZEROKEY:
				do_layer_toets(9); break;
			case MINUSKEY:
				do_layer_toets(10); break;
			case EQUALKEY:
				do_layer_toets(11); break;
			case ACCENTGRAVEKEY:
				do_layer_toets(-1); break;
				
			case AKEY:
				if(G.qual & LR_CTRLKEY) apply_object();
				else if(G.qual & LR_SHIFTKEY) {
					tbox_setmain(0);
					toolbox();
				}
				else {
					if(G.obedit) {
						if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) deselectall_mesh();
						else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) deselectall_nurb();
						else if(G.obedit->type==OB_MBALL) deselectall_mball();
						else if(G.obedit->type==OB_LATTICE) deselectall_Latt();
					}
					else {
						if(G.f & G_FACESELECT) deselectall_tface();
						else deselectall();
					}
				}
				break;
			case BKEY:
				if(G.qual & LR_SHIFTKEY) set_render_border();
				else borderselect();
				break;
			case CKEY:
				if(G.qual & LR_CTRLKEY) {
					copymenu();
				}
				else if(G.qual & LR_ALTKEY) {
					convertmenu();	/* editobject.c */
				}
				else if(G.qual & LR_SHIFTKEY) {
					view3d_home(1);
					curs= give_cursor();
					curs[0]=curs[1]=curs[2]= 0.0;
					addqueue(curarea->win, REDRAW, 1);
				}
				else if(G.obedit!=0 && ELEM(G.obedit->type, OB_CURVE, OB_SURF) ) {
					makecyclicNurb();
					makeDispList(G.obedit);
					allqueue(REDRAWVIEW3D, 0);
				}
				else {
					curs= give_cursor();
					G.vd->ofs[0]= -curs[0];
					G.vd->ofs[1]= -curs[1];
					G.vd->ofs[2]= -curs[2];
					addqueue(curarea->win, REDRAW, 1);
				}
			
				break;
			case DKEY:
				if(G.qual & LR_SHIFTKEY) {
					if(G.obedit) {
						if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) adduplicate_mesh();
						else if(G.obedit->type==OB_MBALL) adduplicate_mball();
						else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) adduplicate_nurb();
					}
					else adduplicate(0);
				}
				else if(G.qual & LR_ALTKEY) {
					if(G.obedit==0) adduplicate(0);
				}
				else if(G.qual & LR_CTRLKEY) {
					imagestodisplist();
				}
				#ifdef FREE
				else {
					pupval= pupmenu("Draw mode%t|BoundBox %x1|Wire %x2|OpenGL Solid %x3|Shaded solid %x4");
					if(pupval>0) {
						G.vd->drawtype= pupval;
						doredraw= 1;
					}
				}
				#else
				else if(G.main->sector.first) sector_simulate();
				#endif
				
				break;
			case EKEY:
				if(G.obedit) {
					if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) extrude_mesh();
					else if(G.obedit->type==OB_CURVE) addvert_Nurb('e');
					else if(G.obedit->type==OB_SURF) extrude_nurb();
				}
				else {
					ob= OBACT;
					if(ob && ob->type==OB_IKA) if(okee("extrude IKA")) extrude_ika(ob, 1);
				}
				break;
			case FKEY:
				if(G.obedit) {
					if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) {
						if(G.qual & LR_SHIFTKEY) fill_mesh();
						else if(G.qual & LR_ALTKEY) beauty_fill();
						else addedgevlak_mesh();
					}
					else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) addsegment_nurb();
				}
				else if(G.qual & LR_CTRLKEY) sort_faces();
				else if(G.qual & LR_ALTKEY) first_base();
				else if(G.qual & LR_SHIFTKEY) fly();
				else {
					#ifndef FREE
					set_faceselect();
					#endif
				}
				
				break;
			case GKEY:
				if(G.qual & LR_ALTKEY) clear_object('g');
				else transform('g');
				break;
			case HKEY:
				if(G.obedit) {
					if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) {
						if(G.qual & LR_ALTKEY) reveal_mesh();
						else hide_mesh(G.qual & LR_SHIFTKEY);
					}
					else if(G.obedit->type== OB_SURF) {
						if(G.qual & LR_ALTKEY) revealNurb();
						else hideNurb(G.qual & LR_SHIFTKEY);
					}
					else if(G.obedit->type==OB_CURVE) {
					
						if(G.qual & 48) autocalchandlesNurb_all(1);	/* flag=1, selected */
						else if(G.qual & 3) sethandlesNurb(1);
						else sethandlesNurb(3);
						
						makeDispList(G.obedit);
						
						allqueue(REDRAWVIEW3D, 0);
					}
				}
				else if(G.f & G_FACESELECT) hide_tface();
				
				break;
			case IKEY:
				break;
				
			case JKEY:
				if(G.qual & LR_CTRLKEY) {
					if(ob= OBACT) {
						if(ob->type == OB_MESH) join_mesh();
						else if(ob->type == OB_CURVE) join_curve(OB_CURVE);
						else if(ob->type == OB_SURF) join_curve(OB_SURF);
						else if(ob->type == OB_SECTOR) join_sector();	/* editmesh.c */
					}
				}
				break;
			case KKEY:
				if(G.obedit) {
					if(G.obedit->type==OB_SURF) printknots();
				}
				else {
					if(G.qual & LR_SHIFTKEY) {
						if(G.f & G_VERTEXPAINT) clear_vpaint();
						else select_select_keys();
					}
					else if(G.qual & LR_CTRLKEY) make_skeleton();
/* 					else if(G.qual & LR_ALTKEY) delete_skeleton(); */
					else set_ob_ipoflags();
				}
				
				break;
			case LKEY:
				if(G.obedit) {
					if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) selectconnected_mesh();
					else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) selectconnected_nurb();
				}
				else {
				
					if(G.qual & LR_SHIFTKEY) selectlinks();
					else if(G.qual & LR_CTRLKEY) linkmenu();
					else if(G.f & G_FACESELECT) select_linked_tfaces();
					else make_local();
				}
				break;
			case MKEY:
				movetolayer();
				break;
			case NKEY:
				if(G.obedit) {
					if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) {
						if(G.qual & LR_SHIFTKEY) {
							 if(okee("Recalc normals inside")) righthandfaces(2);
						}
						else {
							 if(okee("Recalc normals outside")) righthandfaces(1);
						}
						allqueue(REDRAWVIEW3D, 0);
					}
				}
				#ifndef FREE
				else if(G.qual & LR_CTRLKEY) add_networklink();
				#endif
				break;
			case OKEY:
				if(G.qual & LR_ALTKEY) clear_object('o');
				else if(G.obedit) {
					extern int prop_mode;

					if (G.qual & LR_SHIFTKEY) prop_mode= !prop_mode;
					else G.f ^= G_PROPORTIONAL;

					allqueue(REDRAWHEADERS, 0);
				}
				break;
			case PKEY:
			
				if(G.obedit) {
					if(G.qual) {
						if(G.qual & LR_CTRLKEY) make_parent();
					}
					else if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) separate_mesh();
					else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) separate_nurb();
				}
				else if(G.qual & LR_CTRLKEY) make_parent();
				else if(G.qual & LR_ALTKEY) clear_parent();
				break;
			case RKEY:
				if(G.obedit==0 && (G.f & G_FACESELECT)) rotate_uv_tface();
				else if(G.qual & LR_ALTKEY) clear_object('r');
				else if(G.qual & LR_SHIFTKEY) selectrow_nurb();
				else transform('r');
				break;
			case SKEY:
				if(G.qual & LR_ALTKEY) {
					if(G.obedit);
					else clear_object('s');
				}
				else if(G.qual & LR_SHIFTKEY) snapmenu();
				else if(G.qual & LR_CTRLKEY) {
					if(G.obedit) transform('S');
				}
				else transform('s');
				break;
			case TKEY:
				if(G.qual & LR_CTRLKEY) {
					if(G.obedit) {
						if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) {
							convert_to_triface(0);
							allqueue(REDRAWVIEW3D, 0);
							countall();
						}
					}
					else make_track();
				}
				else if(G.qual & LR_ALTKEY) {
					if(G.obedit && G.obedit->type==OB_CURVE) clear_tilt();
					else clear_track();
				}
				else {
					if(G.obedit) transform('t');
					else texspace_edit();
				}
				
				break;
			case UKEY:
				if(G.obedit) {
					if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) remake_editMesh();
					else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) remake_editNurb();
					else if(G.obedit->type==OB_LATTICE) remake_editLatt();
				}
				else if(G.f & G_FACESELECT) uv_autocalc_tface();
				else if(G.f & G_VERTEXPAINT) vpaint_undo();
				else single_user();
				
				break;
			case VKEY:
			
				if(G.obedit) {
					if(G.obedit->type==OB_CURVE) {
						sethandlesNurb(2);
						makeDispList(G.obedit);
						allqueue(REDRAWVIEW3D, 0);
					}
				}
				else if(G.qual & LR_ALTKEY) image_aspect();
				else set_vpaint();
				
				break;
			case WKEY:
				if(G.qual & LR_SHIFTKEY) {
					transform('w');
				}
				else if(G.qual & LR_ALTKEY) {
					/* if(G.obedit && G.obedit->type==OB_MESH) write_videoscape(); */
				}
				else if(G.qual & LR_CTRLKEY) {
					if(G.obedit) {
						if ELEM(G.obedit->type,  OB_CURVE, OB_SURF) {
							switchdirectionNurb2();
						}
					}
				}
				else special_editmenu();
				
				break;
			case XKEY:
				if(G.obedit) {
					if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) delete_mesh();
					else if ELEM(G.obedit->type, OB_CURVE, OB_SURF) delNurb();
					else if(G.obedit->type==OB_MBALL) delete_mball();
				}
				else delete_obj(0);
				break;
			case YKEY:
				if(G.obedit) {
					if ELEM3(G.obedit->type, OB_MESH, OB_SECTOR, OB_LIFE) split_mesh();
				}
				break;
			case ZKEY:
				do_realtimelight(0, 0, 0);
				if(G.qual & LR_CTRLKEY) {
					reshadeall_displist();
					G.vd->drawtype= OB_SHADED;
				}
				else if(G.qual & LR_SHIFTKEY) {
					if(G.vd->drawtype== OB_SHADED) G.vd->drawtype= OB_WIRE;
					else G.vd->drawtype= OB_SHADED;
				}
				else if(G.qual & LR_ALTKEY) {
					if(G.vd->drawtype== OB_TEXTURE) G.vd->drawtype= OB_SOLID;
					else G.vd->drawtype= OB_TEXTURE;
				}
				else {
					if(G.vd->drawtype==OB_SOLID || G.vd->drawtype==OB_SHADED) G.vd->drawtype= OB_WIRE;
					else G.vd->drawtype= OB_SOLID;
				}
				
				
				addqueue(curarea->headwin, REDRAW, 1);
				addqueue(curarea->win, REDRAW, 1);
				break;
				
			
			case HOMEKEY:
				view3d_home(0);
				break;
			case COMMAKEY:
				G.vd->around= V3D_CENTRE;
				addqueue(curarea->headwin, REDRAW, 1);
				break;
				
			case PERIODKEY:
				G.vd->around= V3D_CURSOR;
				addqueue(curarea->headwin, REDRAW, 1);
				break;
			
			case PADVIRGULEKEY:	/* '/' */
				if(G.vd->localview) {
					G.vd->localview= 0;
					endlocalview(curarea);
				}
				else {
					G.vd->localview= 1;
					initlocalview();
				}
				addqueue(curarea->headwin, REDRAW, 1);
				break;
			case PADASTERKEY:	/* '*' */
				ob= OBACT;
				if(ob) {
					obmat_to_viewmat(ob);
					if(G.vd->persp==2) G.vd->persp= 1;
					addqueue(curarea->win, REDRAW, 1);
				}
				break;
			case PADPERIOD:	/* '.' */
				centreview();
				break;
			
			case PAGEUPKEY:
				if(G.qual & LR_CTRLKEY) movekey_obipo(1);
				else nextkey_obipo(1);	/* in editipo.c */
				break;

			case PAGEDOWNKEY:
				if(G.qual & LR_CTRLKEY) movekey_obipo(-1);
				else nextkey_obipo(-1);
				break;
				
			case PAD0: case PAD1: case PAD2: case PAD3: case PAD4:
			case PAD5: case PAD6: case PAD7: case PAD8: case PAD9:
			case PADMINUS: case PADPLUSKEY: case PADENTER:
				persptoetsen(event);
				doredraw= 1;
				break;
			
				break;
			}
		}
	}
	
	if(doredraw) addqueue(curarea->win, REDRAW, 1);
}

void initview3d(ScrArea *sa)
{
	View3D *vd;
	
	vd= callocN(sizeof(View3D), "initview3d");
	addhead(&sa->spacedata, vd);	/* addhead! niet addtail */

	set_func_space(sa);
	
	vd->spacetype= SPACE_VIEW3D;
	vd->viewquat[0]= 1.0;
	vd->viewquat[1]= vd->viewquat[2]= vd->viewquat[3]= 0.0;
	vd->persp= 1;
	vd->drawtype= OB_WIRE;
	vd->view= 7;
	vd->dist= 10.0;
	vd->lens= 35.0;
	vd->clipsta= 0.01;
	vd->clipend= 500.0;
	vd->grid= 1.0;
	vd->gridlines= 16;
	vd->lay= vd->layact= 1;
	if(G.scene) {
		vd->lay= vd->layact= G.scene->lay;
		vd->camera= G.scene->camera;
	}
	vd->scenelock= 1;
}


/* ******************** SPACE: IPO ********************** */

extern void drawipo();

void changeview2d()
{
	View2D *v2d;
	float xmin, xmax, ymin, ymax;
	
	if(G.v2d==0) return;
	
	ortho2(G.v2d->cur.xmin, G.v2d->cur.xmax, G.v2d->cur.ymin, G.v2d->cur.ymax);
	test_view2d();
}

void winqreadipo(ushort event, short val)
{
	Object *ob;
	float dx, dy;
	int doredraw= 0;
	
	if(curarea->win==0) return;

	if(val) {
		switch(event) {
		case LEFTMOUSE:
			if( in_ipo_buttons() ) {
				FrontbufferButs(TRUE);
				val= DoButtons();
				FrontbufferButs(FALSE);
				if(val) do_ipowin_buts(val-1);
				else {
					do_ipo_selectbuttons();
					doredraw= 1;
				}
			}			
			if(G.qual & LR_CTRLKEY) add_vert_ipo();
			else gesture();
			
			break;
		case MIDDLEMOUSE:
			if(in_ipo_buttons()) {
				scroll_ipobuts();
			}
			else view2dmove();	/* in drawipo.c */
			break;
		case RIGHTMOUSE:
			mouse_select_ipo();
			break;
		case PADPLUSKEY:
			dx= 0.1154*(G.v2d->cur.xmax-G.v2d->cur.xmin);
			dy= 0.1154*(G.v2d->cur.ymax-G.v2d->cur.ymin);
			if(val==SPACE_BUTS) {
				dx/=2.0; dy/= 2.0;
			}
			G.v2d->cur.xmin+= dx;
			G.v2d->cur.xmax-= dx;
			G.v2d->cur.ymin+= dy;
			G.v2d->cur.ymax-= dy;
			test_view2d();
			doredraw= 1;
			break;
		case PADMINUS:
			dx= 0.15*(G.v2d->cur.xmax-G.v2d->cur.xmin);
			dy= 0.15*(G.v2d->cur.ymax-G.v2d->cur.ymin);
			if(val==SPACE_BUTS) {
				dx/=2.0; dy/= 2.0;
			}
			G.v2d->cur.xmin-= dx;
			G.v2d->cur.xmax+= dx;
			G.v2d->cur.ymin-= dy;
			G.v2d->cur.ymax+= dy;
			test_view2d();
			doredraw= 1;
			break;
		case PAGEUPKEY:
			if(G.qual & LR_CTRLKEY) movekey_ipo(1);
			else nextkey_ipo(1);
			break;
		case PAGEDOWNKEY:
			if(G.qual & LR_CTRLKEY) movekey_ipo(-1);
			else nextkey_ipo(-1);
			break;
		case HOMEKEY:
			do_ipo_buttons(B_IPOHOME);
			break;
			
		case AKEY:
			if(in_ipo_buttons()) {
				swap_visible_editipo();
			}
			else swap_selectall_editipo();
			break;
		case BKEY:
			borderselect_ipo();
			break;
		case CKEY:
			move_to_frame();
			break;
		case DKEY:
			if(G.qual & LR_SHIFTKEY) add_duplicate_editipo();
			break;
		case GKEY:
			transform_ipo('g');
			break;
		case HKEY:
			if(G.qual & LR_SHIFTKEY) sethandles_ipo(HD_AUTO);
			else sethandles_ipo(HD_ALIGN);
			break;
		case JKEY:
			join_ipo();
			break;
		case KKEY:
			if(G.sipo->showkey) {
				G.sipo->showkey= 0;
				swap_selectall_editipo();	/* sel all */
			}
			else G.sipo->showkey= 1;
			free_ipokey(&G.sipo->ipokey);
			if(G.sipo->ipo) G.sipo->ipo->showkey= G.sipo->showkey;

			addqueue(curarea->headwin, REDRAW, 1);
			allqueue(REDRAWVIEW3D, 0);
			doredraw= 1;
			break;
		case RKEY:
			ipo_record();
			break;
		case SKEY:
			if(G.qual & LR_SHIFTKEY) ipo_snapmenu();
			else transform_ipo('s');
			break;
		case TKEY:
			set_ipotype();
			break;
		case VKEY:
			sethandles_ipo(HD_VECT);
			break;
		case XKEY:
			if(G.qual & LR_SHIFTKEY) delete_key();
			else del_ipo();
			break;
		}
	}

	if(doredraw) addqueue(curarea->win, REDRAW, 1);
}

void initipo(ScrArea *sa)
{
	SpaceIpo *sipo;
	
	sipo= callocN(sizeof(SpaceIpo), "initipo");
	addhead(&sa->spacedata, sipo);

	set_func_space(sa);
	
	sipo->spacetype= SPACE_IPO;
	/* sipo space loopt van (0,-?) tot (??,?) */
	sipo->v2d.tot.xmin= 0.0;
	sipo->v2d.tot.ymin= -10.0;
	sipo->v2d.tot.xmax= G.scene->r.efra;
	sipo->v2d.tot.ymax= 10.0;

	sipo->v2d.cur= sipo->v2d.tot;

	sipo->v2d.min[0]= 0.01;
	sipo->v2d.min[1]= 0.01;

	sipo->v2d.max[0]= 15000.0;
	sipo->v2d.max[1]= 10000.0;
	
	sipo->v2d.scroll= L_SCROLL+B_SCROLL;
	sipo->v2d.keeptot= 0;

	sipo->blocktype= ID_OB;
}

/* ******************** SPACE: INFO ********************** */

void drawinfospace()
{
	extern void reset_autosave();
	float fac;
	char naam[20], *str;

	glClearColor(0.5, 0.5, 0.5, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT);

	fac= ((float)curarea->winx)/1280.0;
	ortho2(0.0, 1280.0, 0.0, curarea->winy/fac);
		
	sprintf(naam, "win %d", curarea->win);
	DefButBlock(naam, curarea->win, G.font, 35, 2, 2);
	
	SetButFunc(reset_autosave);
	SetButCol(11);
	DefButt(TOG|SHO|BIT|0, 0, "Auto Temp Save", 45,32,126,20, &(U.flag), 0, 0, 0, 0, "Enables/Disables the automatic temp. file saving");
	SetButCol(10);
	DefButt(TEX, 0, "Dir:",	45,10,127,20, U.tempdir, 1.0, 63.0, 0, 0, "The directory for temp. files");
	DefButt(NUM|INT, 0, "Time:", 174,32,91,20, &(U.savetime), 1.0, 60.0, 0, 0, "The time in minutes to wait between temp. saves");
	SetButCol(13);
	DefButt(BUT, B_LOADTEMP, "Load Temp", 174,10,90,20, 0, 0, 0, 0, 0, "Loads the most recently saved temp file");

	SetButCol(10);
	DefButt(NUM|SHO, 0, "Versions:", 281,10,86,42, &U.versions, 0.0, 32.0, 0, 0, "The number of old versions to maintain when saving");
	
	SetButCol(11);
	DefButt(TOG|SHO|BIT|4, 0, "Scene Global", 389,32,86,20, &(U.flag), 0, 0, 0, 0, "Forces the current Scene to be displayed in all Screens");
	DefButt(TOG|SHO|BIT|5, 0, "TrackBall",	389,10,84,20, &(U.flag), 0, 0, 0, 0, "Switches between trackball and turntable view rotation methods (MiddleMouse)");
	DefButt(TOG|SHO|BIT|12, 0, "2-Mouse",	477,10,88,20, &(U.flag), 0, 0, 0, 0,  "Maps ALT+LeftMouse to MiddleMouse button");
	DefButt(TOG|SHO|BIT|8, 0, "Mat on Obj",	567,9,78,20, &(U.flag), 0, 0, 0, 0, "Sets whether Material data is linked to Obj or ObjData");
	DefButt(TOG|SHO|BIT|9, B_U_CAPSLOCK, "NoCapsLock",	478,32,87,20, &(U.flag), 0, 0, 0, 0, "Deactives the CapsLock button (only applies to text input)");
	DefButt(TOG|SHO|BIT|10, 0, "Viewmove",	569,32,76,20, &(U.flag), 0, 0, 0, 0, "Sets the default action for the middle mouse button");

	DefButt(TOG|SHO|BIT|11, 0, "ToolTips",	663,10,66,43, &(U.flag), 0, 0, 0, 0, "Enables/Disables tooltips");
	
	DefButt(TOG|SHO|BIT|1, 0, "Grab Grid",	743,32,146,20, &(U.flag), 0, 0, 0, 0, "Changes default step mode for grabbing");
	DefButt(TOG|SHO|BIT|2, 0, "Rot Grid",	815,10,74,20, &(U.flag), 0, 0, 0, 0, "Changes default step mode for rotation");
	DefButt(TOG|SHO|BIT|3, 0, "Size Grid",	743,10,68,20, &(U.flag), 0, 0, 0, 0, "Changes default step mode for scaling");
	
	DefButt(TOG|SHO|BIT|0, 0, "Dupli Mesh",	902,10,90,42, &(U.dupflag), 0, 0, 0, 0, "Causes Mesh data to be duplicated with CTRL+d");
	DefButt(TOG|SHO|BIT|1, 0, "Curve",		995,32,50,20, &(U.dupflag), 0, 0, 0, 0, "Causes Curve data to be duplicated with CTRL+d");
	DefButt(TOG|SHO|BIT|2, 0, "Surf",		995,10,50,20, &(U.dupflag), 0, 0, 0, 0, "Causes Surface data to be duplicated with CTRL+d");
	DefButt(TOG|SHO|BIT|3, 0, "Text",		1048,32,50,20, &(U.dupflag), 0, 0, 0, 0, "Causes Text data to be duplicated with CTRL+d");
	DefButt(TOG|SHO|BIT|4, 0, "MBall",		1048,10,50,20, &(U.dupflag), 0, 0, 0, 0, "Causes Metaball data to be duplicated with CTRL+d");
	DefButt(TOG|SHO|BIT|5, 0, "Lamp",		1101,32,50,20, &(U.dupflag), 0, 0, 0, 0, "Causes Lamp data to be duplicated with CTRL+d");
	DefButt(TOG|SHO|BIT|6, 0, "Ipo",			1101,10,50,20, &(U.dupflag), 0, 0, 0, 0, "Causes Ipo data to be duplicated with CTRL+d");
	DefButt(TOG|SHO|BIT|7, 0, "Material",	1153,32,70,20, &(U.dupflag), 0, 0, 0, 0, "Causes Material data to be duplicated with CTRL+d");
	DefButt(TOG|SHO|BIT|8, 0, "Texture",		1153,10,70,20, &(U.dupflag), 0, 0, 0, 0, "Causes Texture data to be duplicated with CTRL+d");

	SetButCol(10);
	DefButt(TEX, 0, "Fonts:",		45,60,245,20, U.fontdir, 1.0, 63.0, 0, 0, "The default directory to search when loading fonts");
	DefButt(TEX, 0, "Render:",		295,60,245,20, U.renderdir, 1.0, 63.0, 0, 0, "The default directory to choose for rendering");
	DefButt(TEX, 0, "Textures:",		545,60,245,20, U.textudir, 1.0, 63.0, 0, 0, "The default directory to search when loading textures");
	DefButt(TEX, 0, "SeqPlugin:",	795,60,245,20, U.plugseqdir, 1.0, 63.0, 0, 0, "The default directory to search when loading sequence plugins");

	if(TEST_C_KEY) {
		glRasterPos2i(545, 84);
		cpack(0x0);
		fmprstr("C Key registered by: ");
		fmprstr(cdata+20);
	}
}

void winqreadinfospace(ushort event, short val)
{
	char *str;
	
	if(val) {
		switch(event) {
		case LEFTMOUSE:
			FrontbufferButs(TRUE);
			event= DoButtons();
			FrontbufferButs(FALSE);
			if(event) {
				if(event==B_U_CAPSLOCK) disable_capslock(U.flag & NO_CAPSLOCK);
				else do_global_buttons(event);
			}
			break;	
		case MOUSEY:
			if(U.flag & TOOLTIPS) {
				str= GetButTip();
				if(str) {
					set_info_text(str);
					allqueue(REDRAWINFO, 1);
				}
			}
			break;
		}
	}
}

/* ******************** SPACE: BUTS ********************** */

extern void drawbutspace();	/* buttons.c */

void changebutspace()
{
	View2D *v2d;
	float xmin, xmax, ymin, ymax;
	
	if(G.v2d==0) return;
	
	test_view2d();
	ortho2(G.v2d->cur.xmin, G.v2d->cur.xmax, G.v2d->cur.ymin, G.v2d->cur.ymax);
}

void winqreadbutspace(ushort event, short val)
{
	ScrArea *sa, *sa3d;
	char *str;
	float dx, dy;
	int doredraw= 0;
	
	if(curarea->win==0) return;

	if(val) {
		switch(event) {
		case LEFTMOUSE:
			FrontbufferButs(TRUE);
			event= DoButtons();
			FrontbufferButs(FALSE);
			do_blenderbuttons(event);
			
			break;
		case MIDDLEMOUSE:
			view2dmove();	/* in drawipo.c */
			break;
		case PADPLUSKEY:
		case PADMINUS:
			val= SPACE_BUTS;
			winqreadipo(event, val);
			break;
		case RENDERPREVIEW:
			previewrender();
			break;
		
		case MOUSEY:
			if(U.flag & TOOLTIPS) {
				str= GetButTip();
				if(str) {
					set_info_text(str);
					allqueue(REDRAWINFO, 1);
				}
			}
			break;
		
		case HOMEKEY:
			do_buts_buttons(B_BUTSHOME);
			break;


		/* als er maar 1 3d win is: ook persptoetsen doen */
		case PAD0: case PAD1: case PAD2: case PAD3: case PAD4:
		case PAD5: case PAD6: case PAD7: case PAD8: case PAD9:
		case PADENTER: case ZKEY:
			sa3d= 0;
			sa= G.curscreen->areabase.first;
			while(sa) {
				if(sa->spacetype==SPACE_VIEW3D) {
					if(sa3d) return;
					sa3d= sa;
				}
				sa= sa->next;
			}
			if(sa3d) {
				sa= curarea;
				areawinset(sa3d->win);
				
				if(event==ZKEY) winqread3d(event, val);
				else persptoetsen(event);
				
				addqueue(sa3d->win, REDRAW, 1);
				addqueue(sa3d->headwin, REDRAW, 1);
				areawinset(sa->win);
			}
		}
	}

	if(doredraw) addqueue(curarea->win, REDRAW, 1);
}

void set_rects_butspace(SpaceButs *buts)
{
	/* buts space loopt van (0,0) tot (1280, 228) */

	buts->v2d.tot.xmin= 0.0;
	buts->v2d.tot.ymin= 0.0;
	buts->v2d.tot.xmax= 1279.0;
	buts->v2d.tot.ymax= 228.0;
	
	buts->v2d.min[0]= 256.0;
	buts->v2d.min[1]= 42.0;

	buts->v2d.max[0]= 1600.0;
	buts->v2d.max[1]= 450.0;
	
	buts->v2d.minzoom= 0.5;
	buts->v2d.maxzoom= 1.41;
	
	buts->v2d.scroll= 0;
	buts->v2d.keepaspect= 1;
	buts->v2d.keepzoom= 1;
	buts->v2d.keeptot= 1;
	
}

void init_butspace(ScrArea *sa)
{
	SpaceButs *buts;
	
	buts= callocN(sizeof(SpaceButs), "initbuts");
	addhead(&sa->spacedata, buts);

	set_func_space(sa);
	buts->spacetype= SPACE_BUTS;
	
	/* set_rects doet alleen defaults, zodat na het filezen de cur niet verandert */
	set_rects_butspace(buts);
	buts->v2d.cur= buts->v2d.tot;
}

void extern_set_butspace(int fkey)
{
	ScrArea *sa;
	SpaceButs *buts;
	
	/* als een ftoets ingedrukt: de dichtsbijzijnde buttonwindow wordt gezet */
	if(curarea->spacetype==SPACE_BUTS) sa= curarea;
	else {
		/* area vinden */
		sa= G.curscreen->areabase.first;
		while(sa) {
			if(sa->spacetype==SPACE_BUTS) break;
			sa= sa->next;
		}
	}
	
	if(sa==0) return;
	
	if(sa!=curarea) areawinset(sa->win);
	
	buts= sa->spacedata.first;
	
	if(fkey==F4KEY) buts->mainb= BUTS_LAMP;
	else if(fkey==F5KEY) buts->mainb= BUTS_MAT;
	else if(fkey==F6KEY) buts->mainb= BUTS_TEX;
	else if(fkey==F7KEY) buts->mainb= BUTS_ANIM;
	else if(fkey==F8KEY) buts->mainb= BUTS_WORLD;
	else if(fkey==F9KEY) buts->mainb= BUTS_EDIT;
	else if(fkey==F10KEY) buts->mainb= BUTS_RENDER;

	addqueue(sa->headwin, REDRAW, 1);
	addqueue(sa->win, REDRAW, 1);
	preview_changed(sa->win);
}

/* ******************** SPACE: SEQUENCE ********************** */

extern void drawseqspace();

void winqreadsequence(ushort event, short val)
{
	SpaceSeq *sseq;
	extern Sequence *last_seq;
	Object *ob;
	float dx, dy;
	int doredraw= 0, cfra, first;
	short mval[2];
	
	if(curarea->win==0) return;

	sseq= curarea->spacedata.first;

	if(val) {
		switch(event) {
		case LEFTMOUSE:
			if(sseq->mainb || view2dmove()==0) {
				
				first= 1;		
				set_special_seq_update(1);

				do {
					getmouseco_areawin(mval);
					areamouseco_to_ipoco(mval, &dx, &dy);
					
					cfra= (int)dx;
					if(cfra< 1) cfra= 1;
					/* else if(cfra> EFRA) cfra= EFRA; */
					
					if( cfra!=CFRA || first ) {
						first= 0;
				
						CFRA= cfra;
						force_draw();
					}
				
				} while(get_mbut()&L_MOUSE);
				
				set_special_seq_update(0);
				
				do_global_buttons(B_NEWFRAME);
			}
			break;
		case MIDDLEMOUSE:
			if(sseq->mainb) break;
			view2dmove();	/* in drawipo.c */
			break;
		case RIGHTMOUSE:
			if(sseq->mainb) break;
			mouse_select_seq();
			break;
		case PADPLUSKEY:
			if(sseq->mainb) {
				sseq->zoom++;
				if(sseq->zoom>8) sseq->zoom= 8;
			}
			else {
				if(G.qual) {
					if(G.qual & LR_SHIFTKEY) insert_gap(25, CFRA);
					else if(G.qual & LR_ALTKEY) insert_gap(250, CFRA);
					allqueue(REDRAWSEQ, 0);
				}
				else {
					dx= 0.1154*(G.v2d->cur.xmax-G.v2d->cur.xmin);
					G.v2d->cur.xmin+= dx;
					G.v2d->cur.xmax-= dx;
					test_view2d();
				}
			}
			doredraw= 1;
			break;
		case PADMINUS:
			if(sseq->mainb) {
				sseq->zoom--;
				if(sseq->zoom<1) sseq->zoom= 1;
			}
			else {
				if(G.qual) {
					if(G.qual & LR_SHIFTKEY) no_gaps();
				}
				else {
					dx= 0.15*(G.v2d->cur.xmax-G.v2d->cur.xmin);
					G.v2d->cur.xmin-= dx;
					G.v2d->cur.xmax+= dx;
					test_view2d();
				}
			}
			doredraw= 1;
			break;
		case HOMEKEY:
			do_seq_buttons(B_SEQHOME);
			break;
		case PADPERIOD:	
			if(last_seq) {
				CFRA= last_seq->startdisp;
				G.v2d->cur.xmin= last_seq->startdisp- (last_seq->len/20);
				G.v2d->cur.xmax= last_seq->enddisp+ (last_seq->len/20);
				do_global_buttons(B_NEWFRAME);
			}
			break;
			
		case AKEY:
			if(sseq->mainb) break;
			if(G.qual & LR_SHIFTKEY) {
				add_sequence();
			}
			else swap_select_seq();
			break;
		case BKEY:
			if(sseq->mainb) break;
			borderselect_seq();
			break;
		case CKEY:
			if(last_seq && (last_seq->flag & (SEQ_LEFTSEL+SEQ_RIGHTSEL))) {
				if(last_seq->flag & SEQ_LEFTSEL) CFRA= last_seq->startdisp;
				else CFRA= last_seq->enddisp-1;
				
				dx= CFRA-(G.v2d->cur.xmax+G.v2d->cur.xmin)/2;
				G.v2d->cur.xmax+= dx;
				G.v2d->cur.xmin+= dx;
				do_global_buttons(B_NEWFRAME);
			}
			else change_sequence();
			break;
		case DKEY:
			if(sseq->mainb) break;
			if(G.qual & LR_SHIFTKEY) add_duplicate_seq();
			break;
		case EKEY:
			write_edl();
			break;
		case FKEY:
			set_filter_seq();
			break;
		case GKEY:
			if(sseq->mainb) break;
			transform_seq('g');
			break;
		case MKEY:
			if(G.qual & LR_CTRLKEY)  make_effect_movie();
			else if(G.qual & LR_ALTKEY) un_meta();
			else make_meta();
			break;
		case SKEY:
			if(G.qual & LR_SHIFTKEY) seq_snapmenu();
			break;
		case TKEY:
			touch_seq_files();
			break;
		case XKEY:
			if(sseq->mainb) break;
			del_seq();
			break;
		}
	}

	if(doredraw) addqueue(curarea->win, REDRAW, 1);
}


void init_seqspace(ScrArea *sa)
{
	SpaceSeq *sseq;
	
	sseq= callocN(sizeof(SpaceSeq), "initseqspace");
	addhead(&sa->spacedata, sseq);

	set_func_space(sa);
	
	sseq->spacetype= SPACE_SEQ;
	sseq->zoom= 1;
	
	/* seq space loopt van (0,8) tot (250, 0) */

	sseq->v2d.tot.xmin= 0.0;
	sseq->v2d.tot.ymin= 0.0;
	sseq->v2d.tot.xmax= 250.0;
	sseq->v2d.tot.ymax= 8.0;
	
	sseq->v2d.cur= sseq->v2d.tot;

	sseq->v2d.min[0]= 10.0;
	sseq->v2d.min[1]= 4.0;

	sseq->v2d.max[0]= 32000.0;
	sseq->v2d.max[1]= MAXSEQ;
	
	sseq->v2d.minzoom= 0.1;
	sseq->v2d.maxzoom= 10.0;
	
	sseq->v2d.scroll= L_SCROLL+B_SCROLL;
	sseq->v2d.keepaspect= 0;
	sseq->v2d.keepzoom= 0;
	sseq->v2d.keeptot= 0;
}



/* ******************** SPACE: FILE ********************** */

extern void drawfilespace();
extern void winqreadfilespace(ushort, short);


void init_filespace(ScrArea *sa)
{
	SpaceFile *sfile;
	
	sfile= callocN(sizeof(SpaceFile), "initfilespace");
	addhead(&sa->spacedata, sfile);

	sfile->dir[0]= '/';
	sfile->type= FILE_UNIX;

	set_func_space(sa);

	sfile->spacetype= SPACE_FILE;
}

/* ******************** SPACE: IMAGE ********************** */

extern void drawimagespace();

void winqreadimagespace(ushort event, short val)
{
	SpaceImage *sima;
	
	if(val==0) return;
	sima= curarea->spacedata.first;
	
	switch(event) {
	case LEFTMOUSE:
		if(G.qual & LR_SHIFTKEY) mouseco_to_curtile();
		else gesture();
		break;
	case MIDDLEMOUSE:
		image_viewmove();
		break;
	case RIGHTMOUSE:
		mouse_select_sima();
		break;
	case PADPLUSKEY:
		sima->zoom++;
		addqueue(curarea->win, REDRAW, 1);
		break;
	case HOMEKEY:
		image_home();
		break;
	case PADMINUS:
		if(sima->zoom>1) {
			sima->zoom--;
			addqueue(curarea->win, REDRAW, 1);
		}
		break;
		
	case AKEY:
		select_swap_tface_uv();
		break;
	case BKEY:
		borderselect_sima();
		break;
	case GKEY:
		transform_tface_uv('g');
		break;
	case NKEY:
		if(G.qual & LR_CTRLKEY) replace_names_but();
		break;
	case RKEY:
		transform_tface_uv('r');
		break;
	case SKEY:
		transform_tface_uv('s');
		break;
	}
}


void init_imagespace(ScrArea *sa)
{
	SpaceImage *sima;
	
	sima= callocN(sizeof(SpaceImage), "initimaspace");
	addhead(&sa->spacedata, sima);

	set_func_space(sa);
	
	sima->spacetype= SPACE_IMAGE;
	sima->zoom= 1;
}


/* ******************** SPACE: IMASEL ********************** */

extern void drawimasel();
extern void winqreadimasel(ushort, short);


/* alles naar imasel.c */


/* ******************** SPACE: OOPS ********************** */

extern void drawoopsspace();

void winqreadoopsspace(ushort event, short val)
{
	SpaceOops *soops;
	float dx, dy;

	if(val==0) return;
	soops= curarea->spacedata.first;
	
	switch(event) {
	case LEFTMOUSE:
		gesture();
		break;
	case MIDDLEMOUSE:
		view2dmove();	/* in drawipo.c */
		break;
	case RIGHTMOUSE:
		mouse_select_oops();
		break;
	case PADPLUSKEY:
	
		dx= 0.1154*(G.v2d->cur.xmax-G.v2d->cur.xmin);
		dy= 0.1154*(G.v2d->cur.ymax-G.v2d->cur.ymin);
		G.v2d->cur.xmin+= dx;
		G.v2d->cur.xmax-= dx;
		G.v2d->cur.ymin+= dy;
		G.v2d->cur.ymax-= dy;
		test_view2d();
		addqueue(curarea->win, REDRAW, 1);
		break;
	
	case PADMINUS:

		dx= 0.15*(G.v2d->cur.xmax-G.v2d->cur.xmin);
		dy= 0.15*(G.v2d->cur.ymax-G.v2d->cur.ymin);
		G.v2d->cur.xmin-= dx;
		G.v2d->cur.xmax+= dx;
		G.v2d->cur.ymin-= dy;
		G.v2d->cur.ymax+= dy;
		test_view2d();
		addqueue(curarea->win, REDRAW, 1);
		break;
		
	case HOMEKEY:	
		do_oops_buttons(B_OOPSHOME);
		break;
		
	case AKEY:
		swap_select_all_oops();
		addqueue(curarea->win, REDRAW, 1);
		break;
	case BKEY:
		borderselect_oops();
		break;
	case GKEY:
		transform_oops('g');
		break;
	case LKEY:
		if(G.qual & LR_SHIFTKEY) select_backlinked_oops();
		else select_linked_oops();
		break;
	case SKEY:
		
		if(G.qual & LR_ALTKEY) shrink_oops();
		else if(G.qual & LR_SHIFTKEY) shuffle_oops();
		else transform_oops('s');
		break;

	case ONEKEY:
		do_layer_toets(0); break;
	case TWOKEY:
		do_layer_toets(1); break;
	case THREEKEY:
		do_layer_toets(2); break;
	case FOURKEY:
		do_layer_toets(3); break;
	case FIVEKEY:
		do_layer_toets(4); break;
	case SIXKEY:
		do_layer_toets(5); break;
	case SEVENKEY:
		do_layer_toets(6); break;
	case EIGHTKEY:
		do_layer_toets(7); break;
	case NINEKEY:
		do_layer_toets(8); break;
	case ZEROKEY:
		do_layer_toets(9); break;
	case MINUSKEY:
		do_layer_toets(10); break;
	case EQUALKEY:
		do_layer_toets(11); break;
	case ACCENTGRAVEKEY:
		do_layer_toets(-1); break;
	
	}
}

void init_v2d_oops(View2D *v2d)
{
	v2d->tot.xmin= -28.0;
	v2d->tot.xmax= 28.0;
	v2d->tot.ymin= -28.0;
	v2d->tot.ymax= 28.0;
	
	v2d->cur= v2d->tot;

	v2d->min[0]= 10.0;
	v2d->min[1]= 4.0;

	v2d->max[0]= 320.0;
	v2d->max[1]= 320.0;
	
	v2d->minzoom= 0.01;
	v2d->maxzoom= 2.0;
	
	/* v2d->scroll= L_SCROLL+B_SCROLL; */
	v2d->scroll= 0;
	v2d->keepaspect= 1;
	v2d->keepzoom= 0;
	v2d->keeptot= 0;
	
}

void init_oopsspace(ScrArea *sa)
{
	SpaceOops *soops;
	
	soops= callocN(sizeof(SpaceOops), "initoopsspace");
	addhead(&sa->spacedata, soops);

	set_func_space(sa);
	
	soops->visiflag= OOPS_OB+OOPS_MA+OOPS_ME+OOPS_TE+OOPS_CU+OOPS_IP;
	
	soops->spacetype= SPACE_OOPS;
	init_v2d_oops(&soops->v2d);
}

/* ******************** SPACE: PAINT ********************** */


/* ******************** SPACE: Text ********************** */

extern void drawtextspace();
extern void winqreadtextspace(ushort, short);





/* ******************** SPACE: ALGEMEEN ********************** */


void newspace(ScrArea *sa, int type)
{
	View3D *v3d;
	
	if(type>=0) {
		
		if(sa->spacetype != type) {
			
			sa->spacetype= type;
			sa->butspacetype= type;
			sa->headbutofs= 0;
			
			wich_cursor(sa);
			
			addqueue(sa->headwin, CHANGED, 1);
			addqueue(sa->win, CHANGED, 1);

			loadmatrix_win(matone, sa->win);

			/* zoeken of er al een bestaat, we gebruiken v3d als algemeen voorbeeld */
			v3d= sa->spacedata.first;
			while(v3d) {
			
				if(v3d->spacetype==type) {
					remlink(&sa->spacedata, v3d);
					addhead(&sa->spacedata, v3d);
					set_func_space(sa);

					return;
				}
				v3d= v3d->next;
			}

			/* er bestaat er nog geen: nieuwe maken */
			
			if(type==SPACE_EMPTY) {
				set_func_space(sa);
			}
			else if(type==SPACE_VIEW3D) {
				initview3d(sa);
			}
			else if(type==SPACE_IPO) {
				initipo(sa);
			}
			else if(type==SPACE_INFO) {
				set_func_space(sa);
			}
			else if(type==SPACE_BUTS) {
				init_butspace(sa);
			}
			else if(type==SPACE_FILE) {
				init_filespace(sa);
			}
			else if(type==SPACE_SEQ) {
				init_seqspace(sa);
			}
			else if(type==SPACE_IMAGE) {
				init_imagespace(sa);
			}
			else if(type==SPACE_IMASEL) {
				init_imaselspace(sa);
			}
			else if(type==SPACE_OOPS) {
				init_oopsspace(sa);
			}
			else if(type==SPACE_PAINT) {
			}
			else if(type==SPACE_TEXT) {
				init_textspace(sa);
			}
		}
	}
}

void freespacelist(ListBase *lb)
{
	SpaceFile *sfile;	/* voorbeeld */
	SpaceButs *buts;
	SpaceIpo *si;
	View3D *vd;

	sfile= lb->first;
	while(sfile) {
	
		if(sfile->spacetype==SPACE_FILE) {
			if(sfile->libfiledata) freeN(sfile->libfiledata);
		}
		else if(sfile->spacetype==SPACE_BUTS) {
			buts= (SpaceButs *)sfile;
			if(buts->rect) freeN(buts->rect);
			if(G.buts==buts) G.buts= 0;
		}
		else if(sfile->spacetype==SPACE_IPO) {
			si= (SpaceIpo *)sfile;
			if(si->editipo) freeN(si->editipo);
			free_ipokey(&si->ipokey);
			if(G.sipo==si) G.sipo= 0;
		}
		else if(sfile->spacetype==SPACE_VIEW3D) {
			vd= (View3D *)sfile;
			if(vd->bgpic) {
				if(vd->bgpic->rect) freeN(vd->bgpic->rect);
				if(vd->bgpic->ima) vd->bgpic->ima->id.us--;
				freeN(vd->bgpic);
			}
			if(vd->localvd) freeN(vd->localvd);
			if(G.vd==vd) G.vd= 0;
		}
		else if(sfile->spacetype==SPACE_OOPS) {
			free_oopspace(sfile);
		}
		else if(sfile->spacetype==SPACE_IMASEL) {
			free_imasel(sfile);
		}
		else if(sfile->spacetype==SPACE_PAINT) {
		}
		else if(sfile->spacetype==SPACE_TEXT) {
			free_textspace(sfile);
		}
		sfile= sfile->next;
	}
	freelistN(lb);
}

void duplicatespacelist(ListBase *lb1, ListBase *lb2)
{
	SpaceFile *sfile;	/* voorbeeld */
	SpaceButs *buts;
	SpaceIpo *si;
	View3D *vd;
	
	duplicatelist(lb1, lb2);
	
	/* lb1 is kopie van lb2, van lb2 geven we de filelist vrij */
	
	sfile= lb2->first;
	while(sfile) {
		if(sfile->spacetype==SPACE_FILE) {
			sfile->libfiledata= 0;
			sfile->filelist= 0;
		}
		else if(sfile->spacetype==SPACE_OOPS) {
			SpaceOops *so= (SpaceOops *)sfile;
			so->oops.first= so->oops.last= 0;
		}
		else if(sfile->spacetype==SPACE_IMASEL) {
			check_imasel_copy(sfile);
		}
		else if(sfile->spacetype==SPACE_TEXT) {
			check_text_copy(sfile);
		}
		else if(sfile->spacetype==SPACE_PAINT) {
		}
		sfile= sfile->next;
	}
	
	sfile= lb1->first;
	while(sfile) {
		if(sfile->spacetype==SPACE_BUTS) {
			buts= (SpaceButs *)sfile;
			buts->rect= 0;
		}
		else if(sfile->spacetype==SPACE_IPO) {
			si= (SpaceIpo *)sfile;
			si->editipo= 0;
			si->ipokey.first= si->ipokey.last= 0;
		}
		else if(sfile->spacetype==SPACE_VIEW3D) {
			vd= (View3D *)sfile;
			if(vd->bgpic) {
				vd->bgpic= dupallocN(vd->bgpic);
				vd->bgpic->rect= 0;
				if(vd->bgpic->ima) vd->bgpic->ima->id.us++;
			}
		}
		sfile= sfile->next;
	}

	/* nog een keer: van oude View3D de localview restoren (ivm full) */
	vd= lb2->first;
	while(vd) {
		if(vd->spacetype==SPACE_VIEW3D) {
			if(vd->localvd) {
				restore_localviewdata(vd);
				vd->localvd= 0;
				vd->localview= 0;
				vd->lay &= 0xFFFFFF;
			}
		}
		vd= vd->next;
	}
}

void set_func_space(ScrArea *sa)
{
	SpaceSeq *sseq;
	SpaceButs *buts;

	/* ook na file inlezen: terugzetten functie pointers */
	
	/* default */
	sa->windraw= 0;
	sa->winchange= 0;
	sa->winqread= 0;
	
	sa->headdraw= defheaddraw;
	sa->headchange= 0;
	sa->headqread= 0;

	switch(sa->spacetype) {
	case SPACE_VIEW3D:

		sa->windraw= drawview3d;
		sa->winchange= changeview3d;
		sa->winqread= ( void (*)() )winqread3d;
		sa->headqread= ( void (*)() )winqread3d;

		break;
	case SPACE_IPO:

		sa->windraw= drawipo;
		sa->winchange= changeview2d;
		sa->winqread= ( void (*)() )winqreadipo;
		sa->headqread= ( void (*)() )winqreadipo;

		break;
	case SPACE_BUTS:

		sa->windraw= drawbutspace;
		sa->winchange= changebutspace;
		sa->winqread= ( void (*)() )winqreadbutspace;
		sa->headqread= ( void (*)() )winqreadbutspace;

		break;
	case SPACE_FILE:

		sa->windraw= drawfilespace;
		sa->winqread= ( void (*)() )winqreadfilespace;
		sa->headqread= ( void (*)() )winqreadfilespace;

		break;
	case SPACE_INFO:
		sa->windraw= drawinfospace;
		sa->winqread= ( void (*)() )winqreadinfospace;
		break;
		
	case SPACE_SEQ:
		sa->windraw= drawseqspace;
		sa->winchange= changeview2d;
		sa->winqread= ( void (*)() )winqreadsequence;
		
		/* voor oude files */
		sseq= sa->spacedata.first;
		sseq->v2d.keeptot= 0;
		
		break;
	case SPACE_IMAGE:
		sa->windraw= drawimagespace;
		sa->winqread= ( void (*)() )winqreadimagespace;
		break;
	case SPACE_IMASEL:
		sa->windraw= drawimasel;
		sa->winqread= ( void (*)() )winqreadimasel;
		break;
	case SPACE_OOPS:
		sa->windraw= drawoopsspace;
		sa->winchange= changeview2d;
		sa->winqread= ( void (*)() )winqreadoopsspace;
		sa->headqread= ( void (*)() )winqreadoopsspace;
		break;
	case SPACE_PAINT:
		break;
		
	case SPACE_TEXT:
		sa->windraw= drawtextspace;
		sa->winqread= ( void (*)() )winqreadtextspace;

		break;
		
	}

}

/* wordt overal aangeroepen */
void allqueue(ushort event, short val)
{
	ScrArea *sa;
	View3D *v3d;
	SpaceButs *buts;
	SpaceIpo *si;
	SpaceFile *sfile;

	sa= G.curscreen->areabase.first;
	while(sa) {
		if(event==REDRAWALL) {
			addqueue(sa->win, REDRAW, 1);
			addqueue(sa->headwin, REDRAW, 1);
		}
		else if(sa->win != val) {
			switch(event) {
				
			case REDRAWHEADERS:
				addqueue(sa->headwin, REDRAW, 1);
				break;
			case REDRAWVIEW3D:
				if(sa->spacetype==SPACE_VIEW3D) {
					addqueue(sa->win, REDRAW, 1);
					if(val) addqueue(sa->headwin, REDRAW, 1);
				}
				break;
			case REDRAWVIEW3D_Z:
				if(sa->spacetype==SPACE_VIEW3D) {
					v3d= sa->spacedata.first;
					if(v3d->drawtype==OB_SOLID) {
						addqueue(sa->win, REDRAW, 1);
						if(val) addqueue(sa->headwin, REDRAW, 1);
					}
				}
				break;
			case REDRAWVIEWCAM:
				if(sa->spacetype==SPACE_VIEW3D) {
					v3d= sa->spacedata.first;
					if(v3d->persp>1) addqueue(sa->win, REDRAW, 1);
				}
				break;
			case REDRAWINFO:
				if(sa->spacetype==SPACE_INFO) {
					addqueue(sa->headwin, REDRAW, 1);
				}
				break;
			case REDRAWIMAGE:
				if(sa->spacetype==SPACE_IMAGE) {
					addqueue(sa->win, REDRAW, 1);
					addqueue(sa->headwin, REDRAW, 1);
				}
				break;
			case REDRAWIPO:
				if(sa->spacetype==SPACE_IPO) {
					SpaceIpo *si;
					addqueue(sa->headwin, REDRAW, 1);
					addqueue(sa->win, REDRAW, 1);
					if(val) {
						si= sa->spacedata.first;
						si->blocktype= val;
					}
				}
				else if(sa->spacetype==SPACE_OOPS) {
					addqueue(sa->win, REDRAW, 1);
				}
				
				break;
				
			case REDRAWBUTSALL:
				if(sa->spacetype==SPACE_BUTS) {
					addqueue(sa->win, REDRAW, 1);
					addqueue(sa->headwin, REDRAW, 1);
				}
				break;
			case REDRAWBUTSHEAD:
				if(sa->spacetype==SPACE_BUTS) {
					addqueue(sa->headwin, REDRAW, 1);
				}
				break;
			case REDRAWBUTSVIEW:
				if(sa->spacetype==SPACE_BUTS) {
					buts= sa->spacedata.first;
					if(buts->mainb==BUTS_VIEW) {
						addqueue(sa->win, REDRAW, 1);
						addqueue(sa->headwin, REDRAW, 1);
					}
				}
				break;
			case REDRAWBUTSLAMP:
				if(sa->spacetype==SPACE_BUTS) {
					buts= sa->spacedata.first;
					if(buts->mainb==BUTS_LAMP) {
						addqueue(sa->win, REDRAW, 1);
						addqueue(sa->headwin, REDRAW, 1);
					}
				}
				break;
			case REDRAWBUTSMAT:
				if(sa->spacetype==SPACE_BUTS) {
					buts= sa->spacedata.first;
					if(buts->mainb==BUTS_MAT) {
						addqueue(sa->win, REDRAW, 1);
						addqueue(sa->headwin, REDRAW, 1);
					}
				}
				break;
			case REDRAWBUTSTEX:
				if(sa->spacetype==SPACE_BUTS) {
					buts= sa->spacedata.first;
					if(buts->mainb==BUTS_TEX) {
						addqueue(sa->win, REDRAW, 1);
						addqueue(sa->headwin, REDRAW, 1);
					}
				}
				break;
			case REDRAWBUTSANIM:
				if(sa->spacetype==SPACE_BUTS) {
					buts= sa->spacedata.first;
					if(buts->mainb==BUTS_ANIM) {
						addqueue(sa->win, REDRAW, 1);
						addqueue(sa->headwin, REDRAW, 1);
					}
				}
				break;
			case REDRAWBUTSWORLD:
				if(sa->spacetype==SPACE_BUTS) {
					buts= sa->spacedata.first;
					if(buts->mainb==BUTS_WORLD) {
						addqueue(sa->win, REDRAW, 1);
						addqueue(sa->headwin, REDRAW, 1);
					}
				}
				break;
			case REDRAWBUTSRENDER:
				if(sa->spacetype==SPACE_BUTS) {
					buts= sa->spacedata.first;
					if(buts->mainb==BUTS_RENDER) {
						addqueue(sa->win, REDRAW, 1);
						addqueue(sa->headwin, REDRAW, 1);
					}
				}
				break;
			case REDRAWBUTSEDIT:
				if(sa->spacetype==SPACE_BUTS) {
					buts= sa->spacedata.first;
					if(buts->mainb==BUTS_EDIT) {
						addqueue(sa->win, REDRAW, 1);
						addqueue(sa->headwin, REDRAW, 1);
					}
				}
				break;
			case REDRAWBUTSGAME:
				if(sa->spacetype==SPACE_BUTS) {
					buts= sa->spacedata.first;
					if ELEM(buts->mainb, BUTS_GAME, BUTS_FPAINT) {
						addqueue(sa->win, REDRAW, 1);
						addqueue(sa->headwin, REDRAW, 1);
					}
				}
				break;
			case REDRAWBUTSRADIO:
				if(sa->spacetype==SPACE_BUTS) {
					buts= sa->spacedata.first;
					if(buts->mainb==BUTS_RADIO) {
						addqueue(sa->win, REDRAW, 1);
						addqueue(sa->headwin, REDRAW, 1);
					}
				}
				break;
			case REDRAWBUTSSCRIPT:
				if(sa->spacetype==SPACE_BUTS) {
					buts= sa->spacedata.first;
					if(buts->mainb==BUTS_SCRIPT) {
						addqueue(sa->win, REDRAW, 1);
						addqueue(sa->headwin, REDRAW, 1);
					}
				}
				break;
			case REDRAWDATASELECT:
				if(sa->spacetype==SPACE_FILE) {
					sfile= sa->spacedata.first;
					if(sfile->type==FILE_MAIN) {
						freefilelist(sfile);
						addqueue(sa->win, REDRAW, 1);
					}
				}
				else if(sa->spacetype==SPACE_OOPS) {
					addqueue(sa->win, REDRAW, 1);
				}
				break;
			case REDRAWSEQ:
				if(sa->spacetype==SPACE_SEQ) {
					addqueue(sa->win, CHANGED, 1);
					addqueue(sa->win, REDRAW, 1);
					addqueue(sa->headwin, REDRAW, 1);
				}
				break;
			case REDRAWOOPS:
				if(sa->spacetype==SPACE_OOPS) {
					addqueue(sa->win, REDRAW, 1);
				}
				break;
			case REDRAWPAINT:
				if(sa->spacetype==SPACE_PAINT) {
					addqueue(sa->win, REDRAW, 1);
				}
				break;
			case REDRAWTEXT:
				if(sa->spacetype==SPACE_TEXT) {
					addqueue(sa->win, REDRAW, 1);
				}
				break;
			}
		}
		sa= sa->next;
	}
}

void allspace(ushort event, short val)
{
	bScreen *sc;
	ScrArea *sa;
	View3D *v3d;
	SpaceButs *buts;
	

	sc= G.main->screen.first;
	while(sc) {
		sa= sc->areabase.first;
		while(sa) {
		
			v3d= sa->spacedata.first;
			while(v3d) {
				switch(event) {
				
				case REMAKEIPO:
					if(v3d->spacetype==SPACE_IPO) {
						SpaceIpo *si= (SpaceIpo *)v3d;
						if(si->editipo) freeN(si->editipo);
						si->editipo= 0;
						free_ipokey(&si->ipokey);
					}
					break;
										
				case OOPS_TEST:
					if(v3d->spacetype==SPACE_OOPS) {
						SpaceOops *so= (SpaceOops *)v3d;
						so->flag |= SO_TESTBLOCKS;
					}
					break;
				}

				v3d= v3d->next;
			}
			sa= sa->next;
		}
		sc= sc->id.next;
	}
}


void force_draw()
{
	/* alle area's die (ongeveer) zelfde laten zien als curarea */
	ScrArea *tempsa, *sa;

	curarea->windraw();
	
	tempsa= curarea;
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa!=tempsa && sa->spacetype==tempsa->spacetype) {
			if(sa->spacetype==SPACE_VIEW3D) {
				if( ((View3D *)sa->spacedata.first)->lay & ((View3D *)tempsa->spacedata.first)->lay) {
					areawinset(sa->win);
					sa->windraw();
				}
			}
			else if(sa->spacetype==SPACE_IPO) {
				areawinset(sa->win);
				sa->windraw();
			}
			else if(sa->spacetype==SPACE_SEQ) {
				areawinset(sa->win);
				sa->windraw();
			}
		}
		sa= sa->next;
	}
	if(curarea!=tempsa) areawinset(tempsa->win);

	screen_swapbuffers();

}

void force_draw_plus(int type)
{
	/* alle area's die (ongeveer) zelfde laten zien als curarea EN areas van 'type' */
	ScrArea *tempsa, *sa;

	curarea->windraw();

	tempsa= curarea;
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa!=tempsa && (sa->spacetype==tempsa->spacetype || sa->spacetype==type)) {
			if(sa->spacetype==SPACE_VIEW3D) {
				areawinset(sa->win);
				sa->windraw();
			}
			else if(sa->spacetype==SPACE_IPO) {
				areawinset(sa->win);
				sa->windraw();
			}
			else if(sa->spacetype==SPACE_SEQ) {
				areawinset(sa->win);
				sa->windraw();
			}
			else if(sa->spacetype==SPACE_BUTS) {
				areawinset(sa->win);
				sa->windraw();
			}
		}
		sa= sa->next;
	}
	if(curarea!=tempsa) areawinset(tempsa->win);

	screen_swapbuffers();

}

void force_draw_all()
{
	/* alle area's die (ongeveer) zelfde laten zien als curarea EN areas van 'type' */
	ScrArea *tempsa, *sa;

	drawscreen();

	tempsa= curarea;
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa->headwin) {
			areawinset(sa->headwin);
			defheadchange();
			sa->headdraw();
		}
		if(sa->win) {
			areawinset(sa->win);
			sa->windraw();
		}
		sa= sa->next;
	}
	if(curarea!=tempsa) areawinset(tempsa->win);

	screen_swapbuffers();

}

