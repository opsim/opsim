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


/*  headerbuttons.c   mei 94     GRAPHICS
 * 
 * 
 * 
 */

#include "blender.h"
#include "graphics.h"
#include "sequence.h"
#include "ipo.h"
#include "sector.h"
#include "oops.h"
#include "imasel.h"
#include "file.h"
#include "render.h"
#include "text.h"

/* LET OP:  alle headerbuttons voor zelfde window steeds zelfde naam
 *			event B_REDR is standaard redraw
 *
 */


#define XIC 20
#define YIC 20

#define BUTGREY			2
#define BUTGREEN		3
#define BUTBLUE			4
#define BUTSALMON		5
#define MIDGREY			6
#define REDALERT		7


void info_buttons();


/* ********************** GLOBAL ****************************** */

int std_libbuttons(int xco, int lock, int browse, ID *id, ID *parid, short *menupoin, int users, int lib, int del, int autobut)
{
	ListBase *lb;
	Object *ob;
	Ipo *ipo;
	int len, idwasnul=0, idtype;
	char *str, str1[10], *str2;
	
	SetButCol(BUTGREY);
	if(id && lock) {	
		DefBut(TOG|SHO,lock, "ICON 0 3 4",	xco,0,XIC,YIC, &(G.buts->lock), 0, 0, 0, 0);
		xco+= XIC;
	}
	if(browse) {
		if(id==0) {
			idwasnul= 1;
			/* alleen de browse button */
			ob= OBACT;
			if(curarea->spacetype==SPACE_IMAGE) {
				id= G.main->image.first;
			}
			else if(curarea->spacetype==SPACE_IPO) {
				id= G.main->ipo.first;
				/* testen op ipotype */
				while(id) {
					ipo= (Ipo *)id;
					if(G.sipo->blocktype==ipo->blocktype) break;
					id= id->next;
				}
			}
			else if(curarea->spacetype==SPACE_BUTS) {
				if(browse==B_WORLDBROWSE) {
					id= G.main->world.first;
				}
				else if(ob && ob->type && (ob->type<OB_LAMP || ob->type==OB_SECTOR || ob->type==OB_LIFE)) {
					if(G.buts->mainb==BUTS_MAT) id= G.main->mat.first;
					else if(G.buts->mainb==BUTS_TEX) id= G.main->tex.first;
					else if(G.buts->mainb==BUTS_EDIT) {
						if ELEM(ob->type, OB_SECTOR, OB_LIFE) {
							id= G.main->mesh.first;
						}
					}
				}
			}
			else if(curarea->spacetype==SPACE_TEXT) {
				id= G.main->text.first;
			}
		}
		if(id) {
			idtype= GS(id->name);
			lb= wich_libbase(G.main, GS(id->name));
			
			if(idwasnul) id= 0;
			else if(id->us>1) SetButCol(BUTBLUE);
			
			if( idtype==ID_IP ) IPOnames_to_pupstring(&str, lb, id, menupoin, G.sipo->blocktype);
			else IDnames_to_pupstring(&str, lb, id, menupoin);

			if ELEM6( idtype, ID_SCE, ID_SCR, ID_MA, ID_TE, ID_WO, ID_IP) strcat(str, "|ADD NEW %x 32767");
			else if (idtype==ID_TXT) strcat(str, "|OPEN NEW %x 32766 |ADD NEW %x 32767");
			
			SetButLock(G.scene->id.lib!=0, "Can't edit library data");
			if( idtype==ID_SCE || idtype==ID_SCR ) ClearButLock();
			
			if(curarea->spacetype==SPACE_BUTS)
				SetButLock(idtype!=ID_SCR && G.obedit!=0 && G.buts->mainb==BUTS_EDIT, NULL);
			
			if(parid) SetButLock(parid->lib!=0, "Can't edit library data");
			
			DefButt(MENU|SHO, browse, str, xco,0,XIC,YIC, menupoin, 0, 0, 0, 0, "Browse Datablock or Add NEW");
			
			ClearButLock();
		
			freeN(str);
			xco+= XIC;
		}
		else if(curarea->spacetype==SPACE_BUTS) {
			if ELEM3(G.buts->mainb, BUTS_MAT, BUTS_TEX, BUTS_WORLD) {
				SetButLock(G.scene->id.lib!=0, "Can't edit library data");
				if(parid) SetButLock(parid->lib!=0, "Can't edit library data");
				DefButt(MENU|SHO, browse, "ADD NEW %x 32767", xco,0,XIC,YIC, menupoin, 0, 0, 0, 0, "Browse Datablock");
				ClearButLock();
			}
		}
		else if(curarea->spacetype==SPACE_TEXT) {
			DefButt(MENU|SHO, browse, "OPEN NEW %x 32766 | ADD NEW %x 32767", xco,0,XIC,YIC, menupoin, 0, 0, 0, 0, "Browse Datablock");
		}
		else if(curarea->spacetype==SPACE_IPO) {
			SetButLock(G.scene->id.lib!=0, "Can't edit library data");
			if(parid) SetButLock(parid->lib!=0, "Can't edit library data");
			DefButt(MENU|SHO, browse, "ADD NEW %x 32767", xco,0,XIC,YIC, menupoin, 0, 0, 0, 0, "Browse Datablock");
			ClearButLock();
		}
	}

	if(id==0) {
		return xco+XIC;
	}
	
	/* naam */
	if(id->us>1) SetButCol(BUTBLUE);
	if(id->us<=0) {
		SetButCol(REDALERT);
	}
	SetButLock(id->lib!=0, "Can't edit library data");
	
	str1[0]= id->name[0];
	str1[1]= id->name[1];
	str1[2]= ':';
	str1[3]= 0;
	if(strcmp(str1, "SC:")==0) strcpy(str1, "SCE:");
	else if(strcmp(str1, "SR:")==0) strcpy(str1, "SCR:");
	
	if( GS(id->name)==ID_IP) len= 120;
	else len= 140;
	
	SetButFunc(test_idbutton);
	DefButt(TEX, B_IDNAME, str1,	xco, 0, len, YIC, id->name+2, 0.0, 19.0, 0, 0, "Datablock name");
	ClearButLock();
	xco+= len;
	
	if(id->lib) {
		
		if(parid && parid->lib) DefButt(BUT, 0, "ICON 0 6 4",	xco,0,XIC,YIC, 0, 0, 0, 0, 0, "Indirect Library Datablock");
		else DefButt(BUT, lib, "ICON 0 5 4",	xco,0,XIC,YIC, 0, 0, 0, 0, 0, "Library DataBlock, press to make local");
		
		xco+= XIC;
	}
	if(users && id->us>1) {
		sprintf(str1, "%d", id->us);
		if(id->us<100) {
			DefButt(BUT, users, str1,	xco,0,XIC,YIC, 0, 0, 0, 0, 0, "Number of users,  press to make single-user");
			xco+= XIC;
		}
		else {
			DefButt(BUT, users, str1,	xco, 0, XIC+10, YIC, 0, 0, 0, 0, 0, "Number of users,  press to make single-user");
			xco+= XIC+10;
		}
	}

	if(del) {
		if(parid && parid->lib);
		else {
			DefButt(BUT, del, "ICON 0 0 4",	xco,0,XIC,YIC, 0, 0, 0, 0, 0, "Delete link to this Datablock");
			xco+= XIC;
		}
	}
	if(autobut) {
		if(parid && parid->lib);
		else {
			DefButt(BUT, autobut, "ICON 0 7 4",xco,0,XIC,YIC, 0, 0, 0, 0, 0, "Automatic name");
			xco+= XIC;
		}
	}
	
	SetButCol(BUTGREY);	

	return xco;
}


void do_global_buttons(ushort event)
{
	SpaceFile *sfile;
	ListBase *lb;
	Object *ob;
	Material *ma;
	MTex *mtex;
	Mesh *me;
	Curve *cu;
	MetaBall *mb;
	Ipo *ipo;
	Lamp *la;
	World *wrld;
	Sequence *seq;
	Sector *se;
	ID *id, *idtest, *from;
	int nr= 0;
	
	ob= OBACT;
	
	id= 0;	/* id op nul voor texbrowse */
	
	switch(event) {
	
	case B_NEWFRAME:
		addqueue(curarea->win, REDRAW, 1);
		addqueue(curarea->headwin, REDRAW, 1);
		allqueue(REDRAWVIEW3D, 0);
		allqueue(REDRAWIPO, 0);
		allqueue(REDRAWINFO, 1);
		allqueue(REDRAWSEQ, 1);
		allqueue(REDRAWBUTSHEAD, 1);
		allqueue(REDRAWBUTSMAT, 1);
		allqueue(REDRAWBUTSLAMP, 1);

		/* layers/materials, object ipos are calculted in where_is_object (too) */
		do_all_ipos();
		do_all_scripts(SCRIPT_FRAMECHANGED);
		do_all_keys();
		do_all_ikas();
		test_all_displists();
		
		break;		
	case B_REDR:
		addqueue(curarea->win, REDRAW, 1);
		addqueue(curarea->headwin, REDRAW, 1);
		break;
	case B_EDITBROWSE:
		if(ob==0) return;
		if(ob->id.lib) return;
		id= ob->data;
		if(id==0) return;

		if(G.buts->menunr== -2) {
			activate_databrowse((ID *)G.buts->lockpoin, GS(id->name), 0, B_EDITBROWSE, do_global_buttons);
			return;
		}
		if(G.buts->menunr < 0) return;
		
		lb= wich_libbase(G.main, GS(id->name));
		idtest= lb->first;
		while(idtest) {
			if(nr==G.buts->menunr) {
				if(idtest!=id) {
					id->us--;
					id_us_plus(idtest);
					
					ob->data= idtest;
					
					test_object_materials(idtest);
					
					if( GS(idtest->name)==ID_CU ) {
						test_curve_type(ob);
						allqueue(REDRAWBUTSEDIT, 0);
						makeDispList(ob);
					}
					else if( ob->type==OB_MESH ) {
						makeDispList(ob);
					}
					
					allqueue(REDRAWBUTSEDIT, 0);
					allqueue(REDRAWVIEW3D, 0);
					allqueue(REDRAWIPO, 0);
				}
				break;
			}
			nr++;
			idtest= idtest->next;
		}

		break;
	case B_MESHBROWSE:
		if(ob==0) return;
		if(ob->id.lib) return;
		
		id= (ID *)get_mesh(ob);
		if(id==0) id= G.main->mesh.first;
		if(id==0) return;
		
		if(G.buts->menunr== -2) {
			activate_databrowse((ID *)G.buts->lockpoin, GS(id->name), 0, B_MESHBROWSE, do_global_buttons);
			return;
		}
		if(G.buts->menunr < 0) return;
		

		idtest= G.main->mesh.first;
		while(idtest) {
			if(nr==G.buts->menunr) {
					
				set_mesh(ob, (Mesh *)idtest);
				
				allqueue(REDRAWBUTSEDIT, 0);
				allqueue(REDRAWVIEW3D, 0);
				allqueue(REDRAWIPO, 0);

				break;
			}
			nr++;
			idtest= idtest->next;
		}

		break;
	case B_MATBROWSE:
		if(G.buts->menunr== -2) {
			activate_databrowse((ID *)G.buts->lockpoin, ID_MA, 0, B_MATBROWSE, do_global_buttons);
			return;
		}
		
		if(G.buts->menunr < 0) return;
		
		if(G.buts->lock) {
			
		}
		else {
			
			ma= give_current_material(ob, ob->actcol);
			nr= 0;
			
			id= (ID *)ma;
			
			idtest= G.main->mat.first;
			while(idtest) {
				if(nr==G.buts->menunr) {
					break;
				}
				nr++;
				idtest= idtest->next;
			}
			if(idtest==0) {	/* new mat */
				if(id)  idtest= (ID *)copy_material((Material *)id);
				else {
					idtest= (ID *)add_material("Material");
					if ELEM(ob->type, OB_SECTOR, OB_LIFE) 
						init_dyna_material(idtest);
				}
				idtest->us--;
			}
			if(idtest!=id) {
				assign_material(ob, (Material *)idtest, ob->actcol);
				
				allqueue(REDRAWBUTSHEAD, 0);
				allqueue(REDRAWBUTSMAT, 0);
				allqueue(REDRAWIPO, 0);
				preview_changed(curarea->win);
			}
			
		}
		break;
	case B_MATDELETE:
		if(G.buts->lock) {
			
		}
		else {
			ma= give_current_material(ob, ob->actcol);
			if(ma) {
				assign_material(ob, 0, ob->actcol);
				allqueue(REDRAWBUTSHEAD, 0);
				allqueue(REDRAWBUTSMAT, 0);
				allqueue(REDRAWIPO, 0);
				preview_changed(curarea->win);
			}
		}
		break;
	case B_TEXDELETE:
		if(G.buts->lock) {
			
		}
		else {
			if(G.buts->texfrom==0) {	/* from mat */
				ma= give_current_material(ob, ob->actcol);
				if(ma) {
					mtex= ma->mtex[ ma->texact ];
					if(mtex) {
						if(mtex->tex) mtex->tex->id.us--;
						freeN(mtex);
						ma->mtex[ ma->texact ]= 0;
						allqueue(REDRAWBUTSTEX, 0);
						allqueue(REDRAWIPO, 0);
						preview_changed(curarea->win);
					}
				}
			}
			else if(G.buts->texfrom==1) {	/* from world */
				wrld= G.scene->world;
				if(wrld) {
					mtex= wrld->mtex[ wrld->texact ];
					if(mtex) {
						if(mtex->tex) mtex->tex->id.us--;
						freeN(mtex);
						wrld->mtex[ wrld->texact ]= 0;
						allqueue(REDRAWBUTSTEX, 0);
						allqueue(REDRAWIPO, 0);
						preview_changed(curarea->win);
					}
				}
			}
			else {	/* from lamp */
				la= ob->data;
				if(la && ob->type==OB_LAMP) {	/* voor zekerheid */
					mtex= la->mtex[ la->texact ];
					if(mtex) {
						if(mtex->tex) mtex->tex->id.us--;
						freeN(mtex);
						la->mtex[ la->texact ]= 0;
						allqueue(REDRAWBUTSTEX, 0);
						allqueue(REDRAWIPO, 0);
						preview_changed(curarea->win);
					}
				}
			}
		}
		break;
	case B_EXTEXBROWSE:	
	case B_TEXBROWSE:

		if(G.buts->texnr== -2) {
			
			id= G.buts->lockpoin;
			if(event==B_EXTEXBROWSE) {
				id= 0;
				ma= give_current_material(ob, ob->actcol);
				if(ma) {
					mtex= ma->mtex[ ma->texact ];
					if(mtex) id= (ID *)mtex->tex;
				}
			}
			
			activate_databrowse(id, ID_TE, 0, B_TEXBROWSE, do_global_buttons);
			return;
		}
		if(G.buts->texnr < 0) break;
		
		if(G.buts->lock) {
			
		}
		else {
			id= 0;
			
			ma= give_current_material(ob, ob->actcol);
			if(ma) {
				mtex= ma->mtex[ ma->texact ];
				if(mtex) id= (ID *)mtex->tex;
			}

			idtest= G.main->tex.first;
			while(idtest) {
				if(nr==G.buts->texnr) {
					break;
				}
				nr++;
				idtest= idtest->next;
			}
			if(idtest==0) {	/* new tex */
				if(id)  idtest= (ID *)copy_texture((Tex *)id);
				else idtest= (ID *)add_texture("Tex");
				idtest->us--;
			}
			if(idtest!=id && ma) {
				
				if( ma->mtex[ma->texact]==0) ma->mtex[ma->texact]= add_mtex();
				
				ma->mtex[ ma->texact ]->tex= (Tex *)idtest;
				id_us_plus(idtest);
				if(id) id->us--;
				
				allqueue(REDRAWBUTSHEAD, 0);
				allqueue(REDRAWBUTSTEX, 0);
				allqueue(REDRAWBUTSMAT, 0);
				allqueue(REDRAWIPO, 0);
				preview_changed(curarea->win);
			}
		}
		break;
	case B_IPOBROWSE:

		ipo= get_ipo_to_edit(&from);
		id= (ID *)ipo;
		if(from==0) return;
		
		if(G.sipo->menunr== -2) {
			activate_databrowse((ID *)G.sipo->ipo, ID_IP, GS(from->name), B_IPOBROWSE, do_global_buttons);
			return;
		}

		if(G.sipo->menunr < 0) break;

		idtest= G.main->ipo.first;
		while(idtest) {
			if( ((Ipo *)idtest)->blocktype == G.sipo->blocktype) {
				if(nr==G.sipo->menunr) {
					break;
				}
				nr++;
			}
			idtest= idtest->next;
		}
		if(idtest==0) {
			if(ipo) idtest= (ID *)copy_ipo(ipo);
			else {
				nr= GS(from->name);
				if(nr==ID_OB) idtest= (ID *)add_ipo("ObIpo", nr);
				else if(nr==ID_MA) idtest= (ID *)add_ipo("MatIpo", nr);
				else if(nr==ID_SEQ) idtest= (ID *)add_ipo("MatSeq", nr);
				else if(nr==ID_CU) idtest= (ID *)add_ipo("CuIpo", nr);
				else if(nr==ID_KE) idtest= (ID *)add_ipo("KeyIpo", nr);
				else if(nr==ID_WO) idtest= (ID *)add_ipo("WoIpo", nr);
				else if(nr==ID_LA) idtest= (ID *)add_ipo("LaIpo", nr);
				else if(nr==ID_CA) idtest= (ID *)add_ipo("CaIpo", nr);
				else error("Warn2 ton!");
			}
			idtest->us--;
		}
		if(idtest!=id && from) {
			ipo= (Ipo *)idtest;
	
			if(ipo->blocktype==ID_OB) {
				( (Object *)from)->ipo= ipo;
				id_us_plus(idtest);
				allqueue(REDRAWVIEW3D, 0);
			}
			else if(ipo->blocktype==ID_MA) {
				( (Material *)from)->ipo= ipo;
				id_us_plus(idtest);
				allqueue(REDRAWBUTSMAT, 0);
			}
			else if(ipo->blocktype==ID_SEQ) {
				seq= (Sequence *)from;
				if(seq->type & SEQ_EFFECT) {
					id_us_plus(idtest);
					seq->ipo= ipo;
				}
			}
			else if(ipo->blocktype==ID_CU) {
				( (Curve *)from)->ipo= ipo;
				id_us_plus(idtest);
				allqueue(REDRAWVIEW3D, 0);
			}
			else if(ipo->blocktype==ID_KE) {
				( (Key *)from)->ipo= ipo;
				
				id_us_plus(idtest);
				allqueue(REDRAWVIEW3D, 0);
				
			}
			else if(ipo->blocktype==ID_WO) {
				( (World *)from)->ipo= ipo;
				id_us_plus(idtest);
				allqueue(REDRAWBUTSWORLD, 0);
			}
			else if(ipo->blocktype==ID_LA) {
				( (Lamp *)from)->ipo= ipo;
				id_us_plus(idtest);
				allqueue(REDRAWBUTSLAMP, 0);
			}
			else if(ipo->blocktype==ID_CA) {
				( (Camera *)from)->ipo= ipo;
				id_us_plus(idtest);
				allqueue(REDRAWBUTSEDIT, 0);
			}
			else printf("error in browse ipo \n");
			
			if(id) id->us--;
			
			addqueue(curarea->win, REDRAW, 1);
			addqueue(curarea->headwin, REDRAW, 1);
			allqueue(REDRAWIPO, 0);
		}
		break;
	case B_IPODELETE:
		ipo= get_ipo_to_edit(&from);
		if(from==0) return;
		
		ipo->id.us--;
		
		if(ipo->blocktype==ID_OB) ( (Object *)from)->ipo= 0;
		else if(ipo->blocktype==ID_MA) ( (Material *)from)->ipo= 0;
		else if(ipo->blocktype==ID_SEQ) ( (Sequence *)from)->ipo= 0;
		else if(ipo->blocktype==ID_CU) ( (Curve *)from)->ipo= 0;
		else if(ipo->blocktype==ID_KE) ( (Key *)from)->ipo= 0;
		else if(ipo->blocktype==ID_WO) ( (World *)from)->ipo= 0;
		else if(ipo->blocktype==ID_LA) ( (Lamp *)from)->ipo= 0;
		else if(ipo->blocktype==ID_WO) ( (World *)from)->ipo= 0;
		else if(ipo->blocktype==ID_CA) ( (Camera *)from)->ipo= 0;
		else error("Warn ton!");
		
		editipo_changed(1);	/* doredraw */
		allqueue(REDRAWIPO, 0);
		
		break;
	case B_WORLDBROWSE:

		if(G.buts->menunr < 0) break;
		/* geen lock */
			
		wrld= G.scene->world;
		nr= 0;
		
		id= (ID *)wrld;
		
		idtest= G.main->world.first;
		while(idtest) {
			if(nr==G.buts->menunr) {
				break;
			}
			nr++;
			idtest= idtest->next;
		}
		if(idtest==0) {	/* new world */
			if(id) idtest= (ID *)copy_world((World *)id);
			else idtest= (ID *)add_world("World");
			idtest->us--;
		}
		if(idtest!=id) {
			G.scene->world= (World *)idtest;
			id_us_plus(idtest);
			if(id) id->us--;
			
			allqueue(REDRAWBUTSHEAD, 0);
			allqueue(REDRAWBUTSWORLD, 0);
			allqueue(REDRAWIPO, 0);
			preview_changed(curarea->win);
		}
		break;
	case B_WORLDDELETE:
		if(G.scene->world) {
			G.scene->world->id.us--;
			G.scene->world= 0;
			allqueue(REDRAWBUTSWORLD, 0);
			allqueue(REDRAWIPO, 0);
		}
		
		break;
	case B_WTEXBROWSE:

		if(G.buts->texnr== -2) {
			id= 0;
			wrld= G.scene->world;
			if(wrld) {
				mtex= wrld->mtex[ wrld->texact ];
				if(mtex) id= (ID *)mtex->tex;
			}

			activate_databrowse((ID *)id, ID_TE, 0, B_WTEXBROWSE, do_global_buttons);
			return;
		}
		if(G.buts->texnr < 0) break;

		if(G.buts->lock) {
			
		}
		else {
			id= 0;
			
			wrld= G.scene->world;
			if(wrld) {
				mtex= wrld->mtex[ wrld->texact ];
				if(mtex) id= (ID *)mtex->tex;
			}

			idtest= G.main->tex.first;
			while(idtest) {
				if(nr==G.buts->texnr) {
					break;
				}
				nr++;
				idtest= idtest->next;
			}
			if(idtest==0) {	/* new tex */
				if(id)  idtest= (ID *)copy_texture((Tex *)id);
				else idtest= (ID *)add_texture("Tex");
				idtest->us--;
			}
			if(idtest!=id && wrld) {
				
				if( wrld->mtex[wrld->texact]==0) {
					wrld->mtex[wrld->texact]= add_mtex();
					wrld->mtex[wrld->texact]->texco= TEXCO_VIEW;
				}
				wrld->mtex[ wrld->texact ]->tex= (Tex *)idtest;
				id_us_plus(idtest);
				if(id) id->us--;
				
				allqueue(REDRAWBUTSHEAD, 0);
				allqueue(REDRAWBUTSTEX, 0);
				allqueue(REDRAWBUTSWORLD, 0);
				allqueue(REDRAWIPO, 0);
				preview_changed(curarea->win);
			}
		}
		break;
	case B_LAMPBROWSE:
		/* geen lock */
		if(ob==0) return;
		if(ob->type!=OB_LAMP) return;

		if(G.buts->menunr== -2) {
			activate_databrowse((ID *)G.buts->lockpoin, ID_LA, 0, B_LAMPBROWSE, do_global_buttons);
			return;
		}
		if(G.buts->menunr < 0) break;
		
		la= ob->data;
		nr= 0;
		id= (ID *)la;
		
		idtest= G.main->lamp.first;
		while(idtest) {
			if(nr==G.buts->menunr) {
				break;
			}
			nr++;
			idtest= idtest->next;
		}
		if(idtest==0) {	/* geen new lamp */
			return;
		}
		if(idtest!=id) {
			ob->data= (Lamp *)idtest;
			id_us_plus(idtest);
			if(id) id->us--;
			
			allqueue(REDRAWBUTSHEAD, 0);
			allqueue(REDRAWBUTSLAMP, 0);
			allqueue(REDRAWVIEW3D, 0);
			allqueue(REDRAWIPO, 0);
			preview_changed(curarea->win);
		}
		break;
	
	case B_LTEXBROWSE:

		if(ob==0) return;
		if(ob->type!=OB_LAMP) return;

		if(G.buts->texnr== -2) {
			id= 0;
			la= ob->data;
			mtex= la->mtex[ la->texact ];
			if(mtex) id= (ID *)mtex->tex;

			activate_databrowse(id, ID_TE, 0, B_LTEXBROWSE, do_global_buttons);
			return;
		}
		if(G.buts->texnr < 0) break;

		if(G.buts->lock) {
			
		}
		else {
			id= 0;
			
			la= ob->data;
			mtex= la->mtex[ la->texact ];
			if(mtex) id= (ID *)mtex->tex;

			idtest= G.main->tex.first;
			while(idtest) {
				if(nr==G.buts->texnr) {
					break;
				}
				nr++;
				idtest= idtest->next;
			}
			if(idtest==0) {	/* new tex */
				if(id)  idtest= (ID *)copy_texture((Tex *)id);
				else idtest= (ID *)add_texture("Tex");
				idtest->us--;
			}
			if(idtest!=id && la) {
				
				if( la->mtex[la->texact]==0) {
					la->mtex[la->texact]= add_mtex();
					la->mtex[la->texact]->texco= TEXCO_GLOB;
				}
				la->mtex[ la->texact ]->tex= (Tex *)idtest;
				id_us_plus(idtest);
				if(id) id->us--;
				
				allqueue(REDRAWBUTSHEAD, 0);
				allqueue(REDRAWBUTSTEX, 0);
				allqueue(REDRAWBUTSLAMP, 0);
				allqueue(REDRAWIPO, 0);
				preview_changed(curarea->win);
			}
		}
		break;
	
	
	case B_AUTOMATNAME:
		automatname(G.buts->lockpoin);
		allqueue(REDRAWBUTSHEAD, 0);
		break;		
	case B_AUTOTEXNAME:
		if(G.buts->mainb==BUTS_TEX) {
			autotexname(G.buts->lockpoin);
			allqueue(REDRAWBUTSHEAD, 0);
			allqueue(REDRAWBUTSTEX, 0);
		}
		else if(G.buts->mainb==BUTS_MAT) {
			ma= G.buts->lockpoin;
			if(ma->mtex[ ma->texact]) autotexname(ma->mtex[ma->texact]->tex);
			allqueue(REDRAWBUTSMAT, 0);
		}
		else if(G.buts->mainb==BUTS_WORLD) {
			wrld= G.buts->lockpoin;
			if(wrld->mtex[ wrld->texact]) autotexname(wrld->mtex[wrld->texact]->tex);
			allqueue(REDRAWBUTSWORLD, 0);
		}
		else if(G.buts->mainb==BUTS_LAMP) {
			la= G.buts->lockpoin;
			if(la->mtex[ la->texact]) autotexname(la->mtex[la->texact]->tex);
			allqueue(REDRAWBUTSLAMP, 0);
		}
		break;
	case B_NEWSPACE:
			
		newspace(curarea, curarea->butspacetype);
		
		/* uitzondering: filespace */
		if(curarea->spacetype==SPACE_FILE) {
			sfile= curarea->spacedata.first;
			
			if(sfile->type==FILE_MAIN) freefilelist(sfile);
			else sfile->type= FILE_UNIX;
			
			sfile->returnfunc= 0;
			sfile->title[0]= 0;
			if(sfile->filelist) test_flags_file(sfile);
		}
		/* uitzondering: imasel space */
		else if(curarea->spacetype==SPACE_IMASEL) {
			SpaceImaSel *simasel= curarea->spacedata.first;
			simasel->returnfunc= 0;
			simasel->title[0]= 0;
		}

		break;
	case B_LOADTEMP: 	/* is button uit space.c */
		read_autosavefile();
		break;
		
	case B_FULL:
		if(curarea->spacetype!=SPACE_INFO) {
			area_fullscreen();
		}
		break;	

	case B_IDNAME:
		/* redraw omdat naam veranderd is: nieuwe pup */
		addqueue(curarea->headwin, REDRAW, 1);
		allqueue(REDRAWBUTSHEAD, 0);
		allqueue(REDRAWINFO, 1);
		allqueue(REDRAWOOPS, 1);
		/* naam scene ook in set PUPmenu */
		if ELEM(curarea->spacetype, SPACE_BUTS, SPACE_INFO) allqueue(REDRAWBUTSALL, 0);
		
		/* voorkeurnamen */
		set_obact_names(OBACT);
		
		break;
	}
}


void do_global_buttons2(short event)
{
	Base *base;
	Object *ob;
	Material *ma;
	MTex *mtex;
	Mesh *me;
	Curve *cu;
	MetaBall *mb;
	Ipo *ipo;
	Lamp *la;
	Lattice *lt;
	World *wrld;
	Sector *se;
	ID *id, *idtest, *idfrom;	
	
	/* algemeen: Single User mag als from==LOCAL 
	 *			 Make Local mag als (from==LOCAL && id==LIB)
	 */
	
	ob= OBACT;
	
	switch(event) {
		
	case B_LAMPALONE:
		if(ob && ob->id.lib==0) {
			la= ob->data;
			if(la->id.us>1) {
				if(okee("Single user")) {
					ob->data= copy_lamp(la);
					la->id.us--;
				}
			}
		}
		break;
	case B_LAMPLOCAL:
		if(ob && ob->id.lib==0) {
			la= ob->data;
			if(la->id.lib) {
				if(okee("Make local")) {
					make_local_lamp(la);
				}
			}
		}
		break;
	
	case B_CAMERAALONE:
		if(ob && ob->id.lib==0) {
			Camera *ca= ob->data;
			if(ca->id.us>1) {
				if(okee("Single user")) {
					ob->data= copy_camera(ca);
					ca->id.us--;
				}
			}
		}
		break;
	case B_CAMERALOCAL:
		if(ob && ob->id.lib==0) {
			Camera *ca= ob->data;
			if(ca->id.lib) {
				if(okee("Make local")) {
					make_local_camera(ca);
				}
			}
		}
		break;
	case B_WORLDALONE:
		wrld= G.scene->world;
		if(wrld->id.us>1) {
			if(okee("Single user")) {
				G.scene->world= copy_world(wrld);
				wrld->id.us--;
			}
		}
		break;
	case B_WORLDLOCAL:
		wrld= G.scene->world;
		if(wrld && wrld->id.lib) {
			if(okee("Make local")) {
				make_local_world(wrld);
			}
		}
		break;

	case B_LATTALONE:
		if(ob && ob->id.lib==0) {
			lt= ob->data;
			if(lt->id.us>1) {
				if(okee("Single user")) {
					ob->data= copy_lattice(lt);
					lt->id.us--;
				}
			}
		}
		break;
	case B_LATTLOCAL:
		if(ob && ob->id.lib==0) {
			lt= ob->data;
			if(lt->id.lib) {
				if(okee("Make local")) {
					make_local_lattice(lt);
				}
			}
		}
		break;
	
	case B_MATALONE:
		if(ob==0) return;
		ma= give_current_material(ob, ob->actcol);
		idfrom= material_from(ob, ob->actcol);
		if(idfrom && idfrom->lib==0) {
			if(ma->id.us>1) {
				if(okee("Single user")) {
					ma= copy_material(ma);
					ma->id.us= 0;
					assign_material(ob, ma, ob->actcol);
				}
			}
		}
		break;
	case B_MATLOCAL:
		if(ob==0) return;
		idfrom= material_from(ob, ob->actcol);
		if(idfrom->lib==0) {
			ma= give_current_material(ob, ob->actcol);
			if(ma && ma->id.lib) {
				if(okee("Make local")) {
					make_local_material(ma);
				}
			}
		}
		break;

	case B_MESHLOCAL:
		if(ob && ob->id.lib==0) {
			me= get_mesh(ob);
			if(me && me->id.lib) {
				if(okee("Make local")) {
					make_local_mesh(me);
					make_local_key( me->key );
				}
			}
		}
		break;

	case B_MBALLALONE:
		if(ob && ob->id.lib==0) {
			mb= ob->data;
			if(mb->id.us>1) {
				if(okee("Single user")) {
					ob->data= copy_mball(mb);
					mb->id.us--;
					if(ob==G.obedit) allqueue(REDRAWVIEW3D, 0);
				}
			}
		}
		break;
	case B_MBALLLOCAL:
		if(ob && ob->id.lib==0) {
			mb= ob->data;
			if(mb->id.lib) {
				if(okee("Make local")) {
					make_local_mball(mb);
				}
			}
		}
		break;

	case B_CURVEALONE:
		if(ob && ob->id.lib==0) {
			cu= ob->data;
			if(cu->id.us>1) {
				if(okee("Single user")) {
					ob->data= copy_curve(cu);
					cu->id.us--;
					makeDispList(ob);
					if(ob==G.obedit) allqueue(REDRAWVIEW3D, 0);
				}
			}
		}
		break;
	case B_CURVELOCAL:
		if(ob && ob->id.lib==0) {
			cu= ob->data;
			if(cu->id.lib) {
				if(okee("Make local")) {
					make_local_curve(cu);
					make_local_key( cu->key );
					makeDispList(ob);
				}
			}
		}
		break;
		
	case B_TEXALONE:
		if(G.buts->texfrom==0) {	/* from mat */
			if(ob==0) return;
			ma= give_current_material(ob, ob->actcol);
			if(ma && ma->id.lib==0) {
				mtex= ma->mtex[ ma->texact ];
				if(mtex->tex && mtex->tex->id.us>1) {
					if(okee("Single user")) {
						mtex->tex->id.us--;
						mtex->tex= copy_texture(mtex->tex);
					}
				}
			}
		}
		else if(G.buts->texfrom==1) {	/* from world */
			wrld= G.scene->world;
			if(wrld->id.lib==0) {
				mtex= wrld->mtex[ wrld->texact ];
				if(mtex->tex && mtex->tex->id.us>1) {
					if(okee("Single user")) {
						mtex->tex->id.us--;
						mtex->tex= copy_texture(mtex->tex);
					}
				}
			}
		}
		else if(G.buts->texfrom==2) {	/* from lamp */
			if(ob==0 || ob->type!=OB_LAMP) return;
			la= ob->data;
			if(la->id.lib==0) {
				mtex= la->mtex[ la->texact ];
				if(mtex->tex && mtex->tex->id.us>1) {
					if(okee("Single user")) {
						mtex->tex->id.us--;
						mtex->tex= copy_texture(mtex->tex);
					}
				}
			}
		}
		break;
	case B_TEXLOCAL:
		if(G.buts->texfrom==0) {	/* from mat */
			if(ob==0) return;
			ma= give_current_material(ob, ob->actcol);
			if(ma && ma->id.lib==0) {
				mtex= ma->mtex[ ma->texact ];
				if(mtex->tex && mtex->tex->id.lib) {
					if(okee("Make local")) {
						make_local_texture(mtex->tex);
					}
				}
			}
		}
		else if(G.buts->texfrom==1) {	/* from world */
			wrld= G.scene->world;
			if(wrld->id.lib==0) {
				mtex= wrld->mtex[ wrld->texact ];
				if(mtex->tex && mtex->tex->id.lib) {
					if(okee("Make local")) {
						make_local_texture(mtex->tex);
					}
				}
			}
		}
		else if(G.buts->texfrom==2) {	/* from lamp */
			if(ob==0 || ob->type!=OB_LAMP) return;
			la= ob->data;
			if(la->id.lib==0) {
				mtex= la->mtex[ la->texact ];
				if(mtex->tex && mtex->tex->id.lib) {
					if(okee("Make local")) {
						make_local_texture(mtex->tex);
					}
				}
			}
		}
		break;
	
	case B_IPOALONE:
		ipo= get_ipo_to_edit(&idfrom);
		
		if(idfrom && idfrom->lib==0) {
			if(ipo->id.us>1) {
				if(okee("Single user")) {
					if(ipo->blocktype==ID_OB) ((Object *)idfrom)->ipo= copy_ipo(ipo);
					else if(ipo->blocktype==ID_MA) ((Material *)idfrom)->ipo= copy_ipo(ipo);
					else if(ipo->blocktype==ID_SEQ) ((Sequence *)idfrom)->ipo= copy_ipo(ipo);
					else if(ipo->blocktype==ID_CU) ((Curve *)idfrom)->ipo= copy_ipo(ipo);
					else if(ipo->blocktype==ID_KE) ((Key *)idfrom)->ipo= copy_ipo(ipo);
					else if(ipo->blocktype==ID_LA) ((Lamp *)idfrom)->ipo= copy_ipo(ipo);
					else if(ipo->blocktype==ID_WO) ((World *)idfrom)->ipo= copy_ipo(ipo);
					else if(ipo->blocktype==ID_CA) ((Camera *)idfrom)->ipo= copy_ipo(ipo);
					else error("Warn ton!");
					
					ipo->id.us--;
					allqueue(REDRAWIPO, 0);
				}
			}
		}
		break;
	case B_IPOLOCAL:
		ipo= get_ipo_to_edit(&idfrom);
		
		if(idfrom && idfrom->lib==0) {
			if(ipo->id.lib) {
				if(okee("Make local")) {
					make_local_ipo(ipo);
					allqueue(REDRAWIPO, 0);
				}
			}
		}
		break;

	case B_OBALONE:
		if(G.scene->id.lib==0) {
			if(ob->id.us>1) {
				if(okee("Single user")) {
					base= FIRSTBASE;
					while(base) {
						if(base->object==ob) {
							base->object= copy_object(ob);
							ob->id.us--;
							allqueue(REDRAWVIEW3D, 0);
							break;
						}
						base= base->next;
					}
				}
			}
		}
		break;
	case B_OBLOCAL:
		if(G.scene->id.lib==0) {
			if(ob->id.lib) {
				if(okee("Make local")) {
					make_local_object(ob);
					allqueue(REDRAWVIEW3D, 0);
				}
			}
		}
		break;
	case B_MESHALONE:
		if(ob && ob->id.lib==0) {
			
			me= get_mesh(ob);
			
			if(me && me->id.us>1) {
				if(okee("Single user")) {
					Mesh *men= copy_mesh(me);
					men->id.us= 0;
					
					set_mesh(ob, men);
					
					if(ob==G.obedit) allqueue(REDRAWVIEW3D, 0);
				}
			}
		}
		break;
	}
	
	allqueue(REDRAWBUTSALL, 0);
	allqueue(REDRAWOOPS, 0);
}

/* ********************** EMPTY ****************************** */
/* ********************** INFO ****************************** */

void do_info_buttons(ushort event)
{
	bScreen *sc, *oldscreen;
	Scene *sce, *sce1;
	ScrArea *sa;
	int nr, doit;
	
	switch(event) {
	
	case B_INFOSCR:		/* menu select screen */

		if( G.curscreen->screennr== -2) {
			if(curarea->winy <50) {
				sa= closest_bigger_area();
				areawinset(sa->win);
			}
			activate_databrowse((ID *)G.curscreen, ID_SCR, 0, B_INFOSCR, do_info_buttons);
			return;
		}
		if( G.curscreen->screennr < 0) return;
		
		sc= G.main->screen.first;
		nr= 0;
		while(sc) {
			if(nr==G.curscreen->screennr) {
				if(is_allowed_to_change_screen(sc)) setscreen(sc);
				else error("Unable to perform function in EditMode");
				break;
			}
			nr++;
			sc= sc->id.next;
		}
		/* laatste item: NEW SCREEN */
		if(sc==0) {
			duplicate_screen();
		}
		break;
	case B_INFODELSCR:
		doit= 1;

		/* dit event alleen met buttons doen, zodoende nooit vanuit full aanroepbaar */

		if(G.curscreen->id.prev) sc= G.curscreen->id.prev;
		else if(G.curscreen->id.next) sc= G.curscreen->id.next;
		else return;
		if(okee("Delete current screen")) {
			/* vind nieuwe G.curscreen */
			
			oldscreen= G.curscreen;
			setscreen(sc);		/* deze test of sc een full heeft */
			free_libblock(&G.main->screen, oldscreen);
		}
		addqueue(curarea->headwin, REDRAW, 1);

		break;
	case B_INFOSCE:		/* menu select scene */
		
		if( G.obedit) {
			error("Unable to perform function in EditMode");
			return;
		}
		if( G.curscreen->scenenr== -2) {
			if(curarea->winy <50) {
				sa= closest_bigger_area();
				areawinset(sa->win);
			}
			activate_databrowse((ID *)G.scene, ID_SCE, 0, B_INFOSCE, do_info_buttons);
			return;
		}
		if( G.curscreen->scenenr < 0) return;

		sce= G.main->scene.first;
		nr= 0;
		while(sce) {
			if(nr==G.curscreen->scenenr) {
				if(sce!=G.scene) set_scene(sce);
				break;
			}
			nr++;
			sce= sce->id.next;
		}
		/* laatste item: NEW SCENE */
		if(sce==0) {
			nr= pupmenu("Add scene%t|Empty|Link Objects|Link ObData|Full Copy");
			if(nr<= 0) return;
			if(nr==1) {
				sce= add_scene(G.scene->id.name+2);
				sce->r= G.scene->r;
			}
			else sce= copy_scene(G.scene, nr-2);
			
			set_scene(sce);
		}

		break;
	case B_INFODELSCE:
		
		if(G.scene->id.prev) sce= G.scene->id.prev;
		else if(G.scene->id.next) sce= G.scene->id.next;
		else return;
		if(okee("Delete current scene")) {
			
			/* alle sets aflopen */
			sce1= G.main->scene.first;
			while(sce1) {
				if(sce1->set == G.scene) sce1->set= 0;
				sce1= sce1->id.next;
			}
			
			/* alle sequences aflopen */
			clear_scene_in_allseqs(G.scene);
			
			/* alle schermen */
			sc= G.main->screen.first;
			while(sc) {
				if(sc->scene == G.scene) sc->scene= sce;
				sc= sc->id.next;
			}
			free_libblock(&G.main->scene, G.scene);
			set_scene(sce);
		}
	
		break;
	case B_FILEMENU:
		tbox_setmain(9);
		toolbox();
		break;
	}
}

static char *tooltip=0;

void set_info_text(char *str)
{
	/* only one redraw, for tooltips */
	tooltip = str;
}

void info_text(int x, int y)
{
	Object *ob;
	extern float hashvectf[];
	extern int mem_in_use;
	float fac1, fac2, fac3;
	char str[300], tstr[32];
	int colorval;
	
	if(tooltip) {
		sprintf(str,"%s  %s ", versionstr, tooltip);
	}
	else {
		timestr(G.cputime, tstr);
		if(G.obedit) {
			sprintf(str,"%s Fra:%d   Ve:%d-%d Fa:%d-%d  Mem:%.2fM Time:%s (%.2f)   ",
			versionstr, G.scene->r.cfra, G.totvertsel, G.totvert, G.totfacesel, G.totface,
			(mem_in_use>>10)/1024.0,tstr, ((float)(G.time-G.cputime))/100);
		}
		else {
			sprintf(str,"%s  Fra:%d  Ve:%d Fa:%d  Ob:%d-%d La:%d  Mem:%.2fM Time:%s (%.2f)   ",
				versionstr, G.scene->r.cfra, G.totvert, G.totface, G.totobj, G.totobjsel, G.totlamp,  (mem_in_use>>10)/1024.0, tstr, ((float)(G.time-G.cputime))/100);
		}
		ob= OBACT;
		if(ob) {
			strcat(str, ob->id.name+2);
		}
	}
	
	/* promise! Never change these lines agian before 1.73! */
	fac1= fabs(hashvectf[ 2*G.version+1]);
	fac2= 0.4+0.1*hashvectf[ G.version+1];
	fac3= 0.6;
	
	cpack( hsv_to_cpack( fac1 , fac2, fac3) );
	
	glRecti(x-8,  y-4,  x+106,  y+13);

	glColor3ub(0, 0, 0);
	fmsetfont(G.fonts);
	glRasterPos2i(x,  y);

	fmprstr(str);
	
	tooltip= 0;
}


void info_buttons()
{
	bScreen *sc;
	int xco=0;
	char naam[20], *str;

	sprintf(naam, "header %d", curarea->headwin);
	DefButBlock(naam, curarea->headwin, G.font, 35, 2, 2);
	
	if(curarea->full==0) {
	
		DefButt(ICONROW|CHA,B_NEWSPACE, "ICON 0 0 0", 6,0,XIC,YIC, &(curarea->butspacetype), 1.0, 10.0, 0, 0, "Current window type (LeftMouse+drag to change)");
		
		/* STD SCREEN BUTTONS */
		xco+= 38+XIC;
		xco= std_libbuttons(xco, 0, B_INFOSCR, (ID *)G.curscreen, 0, &G.curscreen->screennr, 1, 1, B_INFODELSCR, 0);
	
		/* STD SCENE BUTTONS */
		xco+= 25;
		xco= std_libbuttons(xco, 0, B_INFOSCE, (ID *)G.scene, 0, &G.curscreen->scenenr, 1, 1, B_INFODELSCE, 0);
	}
	else xco= 430;

	if( TEST_C_KEY) {
		bDrawIcon(0, 0x808080, 4, 6, xco+8, 10, YIC);
		xco+= 12;
	}

	info_text(xco+12, 6);
	
	/* altijd als laatste doen */
	curarea->headbutlen= xco+2*XIC;
	
	if(curarea->headbutlen + 4*XIC < curarea->winx) {
		DefButt(BUT, B_FILEMENU, "ICON 0 0 5", curarea->winx-XIC-2, 0,XIC,YIC, 0, 0, 0, 0, 0, "Toolbox menu, hotkey: SPACE");
	}
}

/* ********************** END INFO ****************************** */
/* ********************** SEQUENCE ****************************** */

void do_seq_buttons(short event)
{
	Sequence *seq;
	Editing *ed;
	
	ed= G.scene->ed;
	if(ed==0) return;
	
	switch(event) {
	case B_SEQHOME:
		G.v2d->cur= G.v2d->tot;
		test_view2d();
		addqueue(curarea->win, REDRAW, 1);
		break;
	case B_SEQCLEAR:
		free_imbuf_seq();
		allqueue(REDRAWSEQ, 1);
		break;
	}
	
}

void seq_buttons()
{
	SpaceSeq *sseq;
	int xco;
	char naam[20], str[256];
	
	sseq= curarea->spacedata.first;
	
	sprintf(naam, "header %d", curarea->headwin);
	DefButBlock(naam, curarea->headwin, G.font, 35, 2, 2);

	DefButt(ICONROW|CHA,B_NEWSPACE, "ICON 0 0 0", 6,0,XIC,YIC, &(curarea->butspacetype), 1.0, 10.0, 0, 0, "Current window type (LeftMouse+drag to change)");
	

	/* FULL WINDOW */
	xco= 25;
	if(curarea->full) DefButt(BUT,B_FULL, "ICON 0 1 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Restore smaller windows (CTRL+Up arrow)");
	else DefButt(BUT,B_FULL, "ICON 0 0 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Make fullscreen window (CTRL+Down arrow)");
	
	/* HOME */
	DefButt(BUT, B_SEQHOME, "ICON 0 15 0",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Home (HOMEKEY)");	
	xco+= XIC;
	
	/* IMAGE */
	DefButt(TOG|SHO, B_REDR, "ICON 0 5 0",	xco+=XIC,0,XIC,YIC, &sseq->mainb, 0, 0, 0, 0, "Toggles image display");

	/* ZOOM en BORDER */
	xco+= XIC;
	if(curarea->win) SetButFunc(view2dzoom);
	DefButt(BUT, 0, "ICON 0 13 0",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Zoom view (CTRL+MiddleMouse)");
	DefButt(BUT, B_IPOBORDER, "ICON 0 8 2",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Translate view (MiddleMouse)");

	/* CLEAR MEM */
	xco+= XIC;
	DefButt(BUT, B_SEQCLEAR, "Clear",	xco+=XIC,0,2*XIC,YIC, 0, 0, 0, 0, 0, "Forces a clear of all buffered images in memory");
	
}

/* ********************** END SEQ ****************************** */
/* ********************** VIEW3D ****************************** */

void do_view3d_buttons(short event)
{
	int nr, bit;

	/* pas op: als curarea->win niet bestaat, afvangen als direkt tekenroutines worden aangeroepen */

	switch(event) {
	case B_HOME:
		view3d_home(0);
		break;
	case B_SCENELOCK:
		if(G.vd->scenelock) {
			G.vd->lay= G.scene->lay;
			/* layact zoeken */
			bit= 0;
			while(bit<32) {
				if(G.vd->lay & (1<<bit)) {
					G.vd->layact= 1<<bit;
					break;
				}
				bit++;
			}
			G.vd->camera= G.scene->camera;
			addqueue(curarea->win, REDRAW, 1);
			addqueue(curarea->headwin, REDRAW, 1);
		}
		break;
	case B_LOCALVIEW:
		if(G.vd->localview) initlocalview();
		else endlocalview(curarea);
		addqueue(curarea->headwin, REDRAW, 1);
		break;
	case B_EDITMODE:
		if(G.obedit==0) enter_editmode();
		else exit_editmode(1);
		addqueue(curarea->headwin, REDRAW, 1);
		break;
	
	case B_VPAINT:
		set_vpaint();
		break;
		
	case B_FACESEL:
		set_faceselect();
		break;
		
	case B_VIEWBUT:
	
		if(G.vd->viewbut==1) persptoetsen(PAD7);
		else if(G.vd->viewbut==2) persptoetsen(PAD1);
		else if(G.vd->viewbut==3) persptoetsen(PAD3);
		break;

	case B_PERSP:
	
		if(G.vd->persp==2) persptoetsen(PAD0);
		else {
			G.vd->persp= 1-G.vd->persp;
			persptoetsen(PAD5);
		}
		
		break;
	case B_PROPTOOL:
		allqueue(REDRAWHEADERS, 0);
		break;
	case B_VIEWRENDER:
		G.scene->r.scemode |= R_OGL;
		if(G.qual==0) do_renderfg(0);
		else do_renderfg(1);
		G.scene->r.scemode &= ~R_OGL;
		break;
		
	default:

		if(event>=B_LAY && event<B_LAY+31) {
			if(G.vd->lay!=0 && (G.qual & LR_SHIFTKEY)) {
				
				/* wel actieve layer zoeken */
				
				bit= event-B_LAY;
				if( G.vd->lay & (1<<bit)) G.vd->layact= 1<<bit;
				else {
					if( (G.vd->lay & G.vd->layact) == 0) {
						bit= 0;
						while(bit<32) {
							if(G.vd->lay & (1<<bit)) {
								G.vd->layact= 1<<bit;
								break;
							}
							bit++;
						}
					}
				}
			}
			else {
				bit= event-B_LAY;
				G.vd->lay= 1<<bit;
				G.vd->layact= G.vd->lay;
				addqueue(curarea->headwin, REDRAW, 1);
			}
			addqueue(curarea->win, REDRAW, 1);
			countall();
			
			if(G.vd->scenelock) handle_view3d_lock();
			allqueue(REDRAWOOPS, 0);
		}
		break;
	}
}

void do_layer_toets(short event)
{
	static int oldlay= 1;
	
	if(G.vd==0) return;
	if(G.vd->localview) return;
	
	if(event== -1) {
		if(G.vd->lay== (2<<20)-1) {
			if(G.qual & LR_SHIFTKEY) G.vd->lay= oldlay;
		}
		else {
			oldlay= G.vd->lay;
			G.vd->lay= (2<<20)-1;
		}
		
		if(G.vd->scenelock) handle_view3d_lock();
		addqueue(curarea->win, REDRAW, 1);
	}
	else {
		if(G.qual & LR_ALTKEY) {
			if(event<11) event+= 10;
		}
		if(G.qual & LR_SHIFTKEY) {
			if(G.vd->lay & (1<<event)) G.vd->lay -= (1<<event);
			else  G.vd->lay += (1<<event);
		}
		do_view3d_buttons(event+B_LAY);
	}
	/* redraw lijkt dubbelop: wordt in queue netjes afgehandeld */
	addqueue(curarea->headwin, REDRAW, 1);
	
	if(curarea->spacetype==SPACE_OOPS) allqueue(REDRAWVIEW3D, 1);	/* 1==ook headwin */
	
	sector_update(0);
}

void view3d_buttons()
{
	extern void viewmove();
	static int temp1=1, temp2=2;
	int a, b, xco;
	char naam[20], str[256];
	
	sprintf(naam, "header %d", curarea->headwin);
	DefButBlock(naam, curarea->headwin, G.font, 4, 2, 2);

	DefButt(ICONROW|CHA,B_NEWSPACE, "ICON 0 0 0", 6,0,XIC,YIC, &(curarea->butspacetype), 1.0, 10.0, 0, 0, "Current window type (LeftMouse+drag to change)");
	
	SetButCol(BUTBLUE);	

	/* FULL WINDOW */
	xco= 25;
	if(curarea->full) DefButt(BUT,B_FULL, "ICON 0 1 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Restore smaller windows (CTRL+Up arrow)");
	else DefButt(BUT,B_FULL, "ICON 0 0 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Make fullscreen window (CTRL+Down arrow)");
	
	/* HOME */
	DefButt(BUT, B_HOME, "ICON 0 15 0",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Home (HOMEKEY)");	

	xco+= XIC+8;
	if(G.vd->localview==0) {
		/* LAYERS */
		for(a=0; a<10; a++) {
			DefButt(TOG|INT|BIT|a+10, B_LAY+10+a, "",	xco+a*(XIC/2), 0,			XIC/2, (YIC)/2, &(G.vd->lay), 0, 0, 0, 0, "Layers");
			DefButt(TOG|INT|BIT|a, B_LAY+a, "",			xco+a*(XIC/2), (YIC)/2,	XIC/2, (YIC)/2, &(G.vd->lay), 0, 0, 0, 0, "Layers");
			if(a==4) xco+= 5;
		}
		xco+= (a-2)*(XIC/2)+5;

		/* LOCK */
		DefButt(ICONTOG|SHO, B_SCENELOCK, "ICON 0 3 4",	xco+=XIC,0,XIC,YIC, &(G.vd->scenelock), 0, 0, 0, 0, "Lock layers and used Camera to Scene");
		xco+= XIC;

	}
	else xco+= (10+2)*(XIC/2)+10;
	
	/* LOCALVIEW */
	DefButt(ICONROW|SHO, B_LOCALVIEW, "ICON 0 10 1",	xco+=XIC,0,XIC,YIC, &(G.vd->localview), 0.0, 2.0, 0, 0, "Local View (NumPad /)");
	
	/* PERSP */
	xco+= XIC/2;
	DefButt(ICONROW|SHO, B_PERSP, "ICON 0 0 1",	xco+=XIC,0,XIC,YIC, &(G.vd->persp), 0.0, 2.0, 0, 0, "Perspective mode (NumPad 5, Numpad 0)");
	
	xco+= XIC/2;
	/* AANZICHT */
	
	if(G.vd->view==7) G.vd->viewbut= 1;
	else if(G.vd->view==1) G.vd->viewbut= 2;
	else if(G.vd->view==3) G.vd->viewbut= 3;
	else G.vd->viewbut= 0;
	
	DefButt(ICONROW|SHO, B_VIEWBUT, "ICON 0 2 2", xco+=XIC,0,XIC,YIC, &G.vd->viewbut, 0.0, 3.0, 0, 0, "Top/Front or Side views (Numpad 7, 1, 3)");
	
	/* DRAWTYPE */
	xco+= XIC/2;
	DefButt(ICONROW|SHO, B_REDR, "ICON 0 4 1",	xco+=XIC,0,XIC,YIC, &(G.vd->drawtype), 1.0, 5.0, 0, 0, "Drawtype: boundbox/wire/solid/shaded (ZKEY, SHIFT+Z)");

	/* VIEWMOVE */
	xco+= XIC/2;
	if(curarea->win) SetButFunc(viewmove);
	DefButt(BUT|INT, B_NOTHING, "ICON 0 14 0",	xco+=XIC,0,XIC,YIC, &temp1, 0, 0, 0, 0, "Translate view (SHIFT+MiddleMouse)");
	if(curarea->win) SetButFunc(viewmove);
	DefButt(BUT|INT, B_NOTHING, "ICON 0 13 0",	xco+=XIC,0,XIC,YIC, &temp2, 0, 0, 0, 0, "Zoom view (CTRL+MiddleMouse)");

	/* around */
	xco+= XIC/2;
	DefButt(ROW|SHO, B_REDR, "ICON 0 11 2",	xco+=XIC,0,XIC,YIC, &G.vd->around, 3.0, 0.0, 0, 0, "Rotation/Scaling around boundbox center (COMMAKEY)");
	DefButt(ROW|SHO, B_REDR, "ICON 0 14 2",	xco+=XIC,0,XIC,YIC, &G.vd->around, 3.0, 3.0, 0, 0, "Rotation/Scaling around median point");
	DefButt(ROW|SHO, B_REDR, "ICON 0 12 2",	xco+=XIC,0,XIC,YIC, &G.vd->around, 3.0, 1.0, 0, 0, "Rotation/Scaling around cursor (DOTKEY)");
	DefButt(ROW|SHO, B_REDR, "ICON 0 13 2",	xco+=XIC,0,XIC,YIC, &G.vd->around, 3.0, 2.0, 0, 0, "Rotation/Scaling around individual centers");

	/* mode */
	G.vd->flag &= ~V3D_MODE;
	if(G.obedit) G.vd->flag |= V3D_EDITMODE;
	if(G.f & G_VERTEXPAINT) G.vd->flag |= V3D_VERTEXPAINT;
	#ifndef FREE
	if(G.f & G_FACESELECT) G.vd->flag |= V3D_FACESELECT;
	#endif
	
	xco+= XIC/2;
	DefButt(ICONTOG|SHO|BIT|4, B_EDITMODE, "ICON 0 14 4",	xco+=XIC,0,XIC,YIC, &G.vd->flag, 0, 0, 0, 0, "EditMode (TAB)");
	DefButt(ICONTOG|SHO|BIT|5, B_VPAINT, "ICON 0 16 4",	xco+=XIC,0,XIC,YIC, &G.vd->flag, 0, 0, 0, 0, "VertexPaint Mode (VKEY)");
	#ifndef FREE
	DefButt(ICONTOG|SHO|BIT|6, B_FACESEL, "ICON 0 18 4",	xco+=XIC,0,XIC,YIC, &G.vd->flag, 0, 0, 0, 0, "FaceSelect Mode");
	#endif
	
	if(G.vd->bgpic) {
		xco+= XIC/2;
		DefButt(TOG|SHO|BIT|1, B_REDR, "ICON 0 5 0",	xco+=XIC,0,XIC,YIC, &G.vd->flag, 0, 0, 0, 0, "Display Background picture");
	}
	if(G.obedit && TEST_C_KEY) {
		extern int prop_mode;
		xco+= XIC/2;
		DefButt(ICONTOG|SHO|BIT|14, B_PROPTOOL, "ICON 0 10 4",	xco+=XIC,0,XIC,YIC, &G.f, 0, 0, 0, 0, "Proportional vertex editing");
		if(G.f & G_PROPORTIONAL) {
			DefButt(ROW|INT, 0, "ICON 0 11 5",	xco+=XIC,0,XIC,YIC, &prop_mode, 4.0, 0.0, 0, 0, "Sharp falloff");
			DefButt(ROW|INT, 0, "ICON 0 12 5",	xco+=XIC,0,XIC,YIC, &prop_mode, 4.0, 1.0, 0, 0, "Smooth falloff");
		}
	}
	
	xco+=XIC;
	DefButt(BUT, B_VIEWRENDER, "ICON 0 5 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Render this view. (Hold SHIFT for Anim render)");
	
	/* altijd als laatste doen */
	curarea->headbutlen= xco+2*XIC;
}

/* ********************** VIEW3D ****************************** */
/* ********************** IPO ****************************** */

void do_ipo_buttons(short event)
{
	Ipo *ipo;
	ID *from;
	EditIpo *ei;
	Object *ob;
	View2D *v2d;
	rcti rect;
	float xmin, ymin, dx, dy;
	int a, val, first;
	short mval[2];

	if(curarea->win==0) return;

	switch(event) {
	case B_IPOHOME:
		
		/* boundbox */
			
		v2d= &(G.sipo->v2d);
		first= 1;
		
		ei= G.sipo->editipo;
		if(ei==0) return;
		for(a=0; a<G.sipo->totipo; a++, ei++) {
			if ISPOIN(ei, flag & IPO_VISIBLE, icu) {
			
				boundbox_ipocurve(ei->icu);
				
				if(first) {
					v2d->tot= ei->icu->totrct;
					first= 0;
				}
				else union_rctf(&(v2d->tot), &(ei->icu->totrct));
			}
		}

		/* speciale home */
		if(G.qual & LR_SHIFTKEY) {
			v2d->tot.xmin= SFRA;
			v2d->tot.xmax= EFRA;
		}

		/* beetje uitzoomen */
		dx= 0.10*(v2d->tot.xmax-v2d->tot.xmin);
		dy= 0.10*(v2d->tot.ymax-v2d->tot.ymin);
		
		if(dx<v2d->min[0]) dx= v2d->min[0];
		if(dy<v2d->min[1]) dy= v2d->min[1];
		
		v2d->cur.xmin= v2d->tot.xmin- dx;
		v2d->cur.xmax= v2d->tot.xmax+ dx;
		v2d->cur.ymin= v2d->tot.ymin- dy;
		v2d->cur.ymax= v2d->tot.ymax+ dy;

		test_view2d();
		addqueue(curarea->win, REDRAW, 1);
		break;
	case B_IPOBORDER:
		val= get_border(&rect, 2);
		if(val) {
			mval[0]= rect.xmin;
			mval[1]= rect.ymin;
			areamouseco_to_ipoco(mval, &xmin, &ymin);
			mval[0]= rect.xmax;
			mval[1]= rect.ymax;
			areamouseco_to_ipoco(mval, &(G.v2d->cur.xmax), &(G.v2d->cur.ymax));
			G.v2d->cur.xmin= xmin;
			G.v2d->cur.ymin= ymin;
			
			test_view2d();
			addqueue(curarea->win, REDRAW, 1);
		}
		break;
	case B_IPOCOPY:
		copy_editipo();
		break;
	case B_IPOPASTE:
		paste_editipo();
		break;
	case B_IPOCONT:
		set_exprap_ipo(IPO_HORIZ);
		break;
	case B_IPOEXTRAP:
		set_exprap_ipo(IPO_DIR);
		break;
	case B_IPOCYCLIC:
		set_exprap_ipo(IPO_CYCL);
		break;
	case B_IPOCYCLICX:
		set_exprap_ipo(IPO_CYCLX);
		break;
	case B_IPOMAIN:
		make_editipo();
		addqueue(curarea->win, REDRAW, 1);
		addqueue(curarea->headwin, REDRAW, 1);

		break;
	case B_IPOSHOWKEY:
		/* waarde omkeren vanwege winqread */
		G.sipo->showkey= 1-G.sipo->showkey;
		winqreadipo(KKEY, 1);
		break;
		
	}	
}

void ipo_buttons()
{
	Object *ob;
	ID *id, *from;
	Material *ma;
	int xco;
	char naam[20];

	sprintf(naam, "header %d", curarea->headwin);
	DefButBlock(naam, curarea->headwin, G.font, 35, 2, 2);

	DefButt(ICONROW|CHA,B_NEWSPACE, "ICON 0 0 0", 6,0,XIC,YIC, &(curarea->butspacetype), 1.0, 10.0, 0, 0, "Current window type (LeftMouse+drag to change)");

	SetButCol(BUTSALMON);

	/* FULL WINDOW en HOME */
	xco= 25;
	if(curarea->full) DefButt(BUT,B_FULL, "ICON 0 1 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Restore smaller windows (CTRL+Up arrow)");
	else DefButt(BUT,B_FULL, "ICON 0 0 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Make fullscreen window (CTRL+Down arrow)");
	DefButt(BUT, B_IPOHOME, "ICON 0 15 0",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Home (HOMEKEY)");
	DefButt(ICONTOG|SHO, B_IPOSHOWKEY, "ICON 0 18 6",	xco+=XIC,0,XIC,YIC, &G.sipo->showkey, 0, 0, 0, 0, "Toggles curve/key display (KKEY)");

	/* hoofdmenu: alleen als data available */
	ob= OBACT;
	xco+= XIC/2;
	DefButt(ROW|SHO, B_IPOMAIN, "ICON 0 7 8",	xco+=XIC,0,XIC,YIC, &G.sipo->blocktype, 1.0, (float)ID_OB, 0, 0, "Display Object Ipos ");

	if(ob && (ma=give_current_material(ob, ob->actcol))) {
		DefButt(ROW|SHO, B_IPOMAIN, "ICON 0 8 4",	xco+=XIC,0,XIC,YIC, &G.sipo->blocktype, 1.0, (float)ID_MA, 0, 0, "Display Material Ipos ");
		if(G.sipo->blocktype==ID_MA) {
			DefButt(NUM|SHO, B_IPOMAIN, "",		xco+=XIC,0,XIC-4,YIC, &G.sipo->channel, 0.0, 7.0, 0, 0, "Channel Number of the active Material texture");
			xco-= 4;
		}
	}
	if(G.scene->world) {
		DefButt(ROW|SHO, B_IPOMAIN, "ICON 0 5 7",	xco+=XIC,0,XIC,YIC, &G.sipo->blocktype, 1.0, (float)ID_WO, 0, 0, "Display World Ipos");
		if(G.sipo->blocktype==ID_WO) {
			DefButt(NUM|SHO, B_IPOMAIN, "",		xco+=XIC,0,XIC-4,YIC, &G.sipo->channel, 0.0, 7.0, 0, 0, "Channel Number of the active World texture");
			xco-= 4;
		}
	}
	
	if(ob && ob->type==OB_CURVE)
		DefButt(ROW|SHO, B_IPOMAIN, "ICON 0 4 7",	xco+=XIC,0,XIC,YIC, &G.sipo->blocktype, 1.0, (float)ID_CU, 0, 0, "Display Curve Ipos");

	if(ob && ob->type==OB_CAMERA)
		DefButt(ROW|SHO, B_IPOMAIN, "ICON 0 2 1",	xco+=XIC,0,XIC,YIC, &G.sipo->blocktype, 1.0, (float)ID_CA, 0, 0, "Display Camera Ipos");

	if(ob && ob->type==OB_LAMP) {
		DefButt(ROW|SHO, B_IPOMAIN, "ICON 0 1 7",	xco+=XIC,0,XIC,YIC, &G.sipo->blocktype, 1.0, (float)ID_LA, 0, 0, "Display Lamp Ipos");
		if(G.sipo->blocktype==ID_LA) {
			DefButt(NUM|SHO, B_IPOMAIN, "",		xco+=XIC,0,XIC-4,YIC, &G.sipo->channel, 0.0, 7.0, 0, 0, "Channel Number of the active Lamp texture");
			xco-= 4;
		}
	}
	
	if(ob) {
		if ELEM4(ob->type, OB_MESH, OB_CURVE, OB_SURF, OB_LATTICE)
			DefButt(ROW|SHO, B_IPOMAIN, "ICON 0 7 7",	xco+=XIC,0,XIC,YIC, &G.sipo->blocktype, 1.0, (float)ID_KE, 0, 0, "Display VertexKeys Ipos");
	}
	DefButt(ROW|SHO, B_IPOMAIN, "ICON 0 7 0",	xco+=XIC,0,XIC,YIC, &G.sipo->blocktype, 1.0, (float)ID_SEQ, 0, 0, "Display Sequence Ipos");

	/* NAME ETC */
	id= (ID *)get_ipo_to_edit(&from);
	xco= std_libbuttons(xco+1.5*XIC, 0, B_IPOBROWSE, id, from, &(G.sipo->menunr), B_IPOALONE, B_IPOLOCAL, B_IPODELETE, 0);	

	SetButLock(id && id->lib, "Can't edit library data");

	/* COPY PASTE */
	xco-= XIC/2;
	if(curarea->headertype==HEADERTOP) {
		DefButt(BUT, B_IPOCOPY, "ICON 0 14 7",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Copies the selected curves to the buffer");
		SetButLock(id && id->lib, "Can't edit library data");
		DefButt(BUT, B_IPOPASTE, "ICON 0 13 7",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Pastes the curves from the buffer");
	}
	else {
		DefButt(BUT, B_IPOCOPY, "ICON 0 14 6",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Copies the selected curves to the buffer");
		SetButLock(id && id->lib, "Can't edit library data");
		DefButt(BUT, B_IPOPASTE, "ICON 0 13 6",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Pastes the curves from the buffer");
	}
	xco+=XIC/2;
	
	/* EXTRAP */
	DefButt(BUT, B_IPOCONT, "ICON 0 15 6",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Sets the extend mode to constant");
	DefButt(BUT, B_IPOEXTRAP, "ICON 0 16 6",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Sets the extend mode to extrapolation");
	DefButt(BUT, B_IPOCYCLIC, "ICON 0 17 6",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0,  "Sets the extend mode to cyclic");
	DefButt(BUT, B_IPOCYCLICX, "ICON 0 17 7",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0,  "Sets the extend mode to cyclic extrapolation");
	xco+= XIC/2;

	ClearButLock();
	/* ZOOM en BORDER */
	if(curarea->win) SetButFunc(view2dzoom);
	DefButt(BUT, 0, "ICON 0 13 0",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Zoom view (CTRL+MiddleMouse)");
	DefButt(BUT, B_IPOBORDER, "ICON 0 8 2",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Translate view (MiddleMouse)");
	
	/* draw LOCK */
	xco+= XIC/2;
	DefButt(ICONTOG|SHO, 0, "ICON 0 3 4",	xco+=XIC,0,XIC,YIC, &(G.sipo->lock), 0, 0, 0, 0, "Lock redraw of other windows while editing");

	/* altijd als laatste doen */
	curarea->headbutlen= xco+2*XIC;

}

/* ********************** IPO ****************************** */
/* ********************** BUTS ****************************** */

Material matcopybuf;

void clear_matcopybuf()
{
	bzero(&matcopybuf, sizeof(Material));
}

void free_matcopybuf()	/* wordt ook vanuit library.c aangeroepen als safety bij reload/erase all */
{
	extern MTex mtexcopybuf;	/* buttons.c */
	int a;
	
	for(a=0; a<8; a++) {
		if(matcopybuf.mtex[a]) {
			freeN(matcopybuf.mtex[a]);
			matcopybuf.mtex[a]= 0;
		}
	}
	
	default_mtex(&mtexcopybuf);
}

void do_buts_buttons(short event)
{
	static short matcopied=0;
	MTex *mtex;
	Material *ma;
	ID id;
	int a;
	
	if(curarea->win==0) return;

	switch(event) {
	case B_BUTSHOME:
		G.v2d->cur= G.v2d->tot;
		test_view2d();
		addqueue(curarea->win, REDRAW, 1);
		break;
	case B_BUTSPREVIEW:
		preview_changed(curarea->win);
		addqueue(curarea->headwin, REDRAW, 1);
		addqueue(curarea->win, REDRAW, 1);
		break;
	case B_MATCOPY:
		if(G.buts->lockpoin) {
			
			if(matcopied) free_matcopybuf();
			
			memcpy(&matcopybuf, G.buts->lockpoin, sizeof(Material));
			for(a=0; a<8; a++) {
				mtex= matcopybuf.mtex[a];
				if(mtex) {
					matcopybuf.mtex[a]= dupallocN(mtex);
				}
			}
			matcopied= 1;
		}
		break;
	case B_MATPASTE:
		if(matcopied && G.buts->lockpoin) {
			ma= G.buts->lockpoin;
			/* vrijgeven huidige mat */
			for(a=0; a<8; a++) {
				mtex= ma->mtex[a];
				if(mtex && mtex->tex) mtex->tex->id.us--;
				if(mtex) freeN(mtex);
			}
			
			id= (ma->id);
			memcpy(G.buts->lockpoin, &matcopybuf, sizeof(Material));
			(ma->id)= id;
			
			for(a=0; a<8; a++) {
				mtex= ma->mtex[a];
				if(mtex) {
					ma->mtex[a]= dupallocN(mtex);
					if(mtex->tex) id_us_plus((ID *)mtex->tex);
				}
			}
			preview_changed(curarea->win);
			addqueue(curarea->win, REDRAW, 1);
		}
		break;
	case B_MESHTYPE:
		allqueue(REDRAWBUTSEDIT, 0);
		allqueue(REDRAWVIEW3D, 0);
		break;
	}
}

void buts_buttons()
{
	ID *id, *idfrom;
	Object *ob;
	Material *ma;
	World *wrld;
	MTex *mtex;
	int xco, alone, local, browse;
	char naam[20];

	sprintf(naam, "header %d", curarea->headwin);
	DefButBlock(naam, curarea->headwin, G.font, 35, 2, 2);

	DefButt(ICONROW|CHA,B_NEWSPACE, "ICON 0 0 0", 6,0,XIC,YIC, &(curarea->butspacetype), 1.0, 10.0, 0, 0, "Current window type (LeftMouse+drag to change)");

	/* FULL WINDOW */
	xco= 25;
	if(curarea->full) DefButt(BUT,B_FULL, "ICON 0 1 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Restore smaller windows (CTRL+Up arrow)");
	else DefButt(BUT,B_FULL, "ICON 0 0 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Make fullscreen window (CTRL+Down arrow)");

	/* HOME */
	DefButt(BUT, B_BUTSHOME, "ICON 0 15 0",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Home (HOMEKEY)");
	
	/* keuzemenu */
	xco+= 2*XIC;
	DefButt(ROW|SHO, B_REDR,			"ICON 0 0 7 View",	xco+=XIC, 0, 30, YIC, &(G.buts->mainb), 1.0, (float)BUTS_VIEW, 0, 0, "View buttons");
	DefButt(ROW|SHO, B_BUTSPREVIEW,		"ICON 0 1 7 Lamp",	xco+=30, 0, 30, YIC, &(G.buts->mainb), 1.0, (float)BUTS_LAMP, 0, 0, "Lamp buttons (F4)");
	DefButt(ROW|SHO, B_BUTSPREVIEW,		"ICON 0 2 7 Mat",	xco+=30, 0, 30, YIC, &(G.buts->mainb), 1.0, (float)BUTS_MAT, 0, 0, "Material buttons (F5)");
	DefButt(ROW|SHO, B_BUTSPREVIEW,		"ICON 0 3 7 Tex",	xco+=30, 0, 30, YIC, &(G.buts->mainb), 1.0, (float)BUTS_TEX, 0, 0, "Texture buttons (F6)");
	DefButt(ROW|SHO, B_REDR,			"ICON 0 4 7 Anim",	xco+=30, 0, 30, YIC, &(G.buts->mainb), 1.0, (float)BUTS_ANIM, 0, 0, "Animation buttons (F7)");
	DefButt(ROW|SHO, B_BUTSPREVIEW,		"ICON 0 5 7 World",	xco+=30, 0, 30, YIC, &(G.buts->mainb), 1.0, (float)BUTS_WORLD, 0, 0, "World buttons (F8)");
	DefButt(ROW|SHO, B_REDR,			"ICON 0 7 7 Edit",	xco+=30, 0, 30, YIC, &(G.buts->mainb), 1.0, (float)BUTS_EDIT, 0, 0, "Edit buttons (F9)");
	#ifndef FREE
	DefButt(ROW|SHO, B_REDR,			"ICON 0 8 7 Game",	xco+=30, 0, 30, YIC, &(G.buts->mainb), 1.0, (float)BUTS_GAME, 0, 0, "Realtime buttons");
	#endif
	DefButt(ROW|SHO, B_REDR,			"ICON 0 9 7 FPaint",xco+=30, 0, 30, YIC, &(G.buts->mainb), 1.0, (float)BUTS_FPAINT, 0, 0, "Paint buttons");
	if(TEST_C_KEY) {
		DefButt(ROW|SHO, B_REDR,		"ICON 0 10 7 Radio",xco+=30, 0, 30, YIC, &(G.buts->mainb), 1.0, (float)BUTS_RADIO, 0, 0, "Radiosity buttons");
		DefButt(ROW|SHO, B_REDR,		"ICON 0 11 7 Script",xco+=30, 0, 30, YIC, &(G.buts->mainb), 1.0, (float)BUTS_SCRIPT, 0, 0, "Script buttons");
	}		
	DefButt(ROW|SHO, B_REDR,			"ICON 0 6 7 Scene",	xco+=30, 0, 50, YIC, &(G.buts->mainb), 1.0, (float)BUTS_RENDER, 0, 0, "Display buttons (F10)");
	xco+= 80;
	
	ob= OBACT;
	
	G.buts->lockpoin= 0;
	
	if(G.buts->mainb==BUTS_VIEW) {
		
	}
	else if(G.buts->mainb==BUTS_LAMP) {
		if(ob && ob->type==OB_LAMP) {
			id= ob->data;
			xco= std_libbuttons(xco, 0, B_LAMPBROWSE, id, (ID *)ob, &(G.buts->menunr), B_LAMPALONE, B_LAMPLOCAL, 0, 0);	
			G.buts->lockpoin= id;
		}
	}
	else if(G.buts->mainb==BUTS_MAT) {
		id= 0;
		if(ob && (ob->type<OB_LAMP || ob->type==OB_SECTOR  || ob->type==OB_LIFE) && ob->type) {
			id= (ID *)give_current_material(ob, ob->actcol);
			idfrom= material_from(ob, ob->actcol);
			xco= std_libbuttons(xco, 0, B_MATBROWSE, id, idfrom, &(G.buts->menunr), B_MATALONE, B_MATLOCAL, B_MATDELETE, B_AUTOMATNAME);
			G.buts->lockpoin= id;
		}
	
		/* COPY PASTE */
		if(curarea->headertype==HEADERTOP) {
			DefButt(BUT, B_MATCOPY, "ICON 0 14 7",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Copy Material to the buffer");
			SetButLock(id && id->lib, "Can't edit library data");
			DefButt(BUT, B_MATPASTE, "ICON 0 13 7",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Paste Material from the buffer");
		}
		else {
			DefButt(BUT, B_MATCOPY, "ICON 0 14 6",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Copy Material to the buffer");
			SetButLock(id && id->lib, "Can't edit library data");
			DefButt(BUT, B_MATPASTE, "ICON 0 13 6",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Paste Material from the buffer");
		}
		xco+=XIC;

	}
	else if(G.buts->mainb==BUTS_TEX) {
		if(G.buts->mainbo != G.buts->mainb) {
			if(G.buts->mainbo==BUTS_LAMP) G.buts->texfrom= 2;
			else if(G.buts->mainbo==BUTS_WORLD) G.buts->texfrom= 1;
			else if(G.buts->mainbo==BUTS_MAT) G.buts->texfrom= 0;
		}
		
		if(G.buts->texfrom==0) {
			if(ob && ob->type<OB_LAMP && ob->type) {
				id= 0;
				ma= give_current_material(ob, ob->actcol);
				if(ma) {
					mtex= ma->mtex[ ma->texact ];
					if(mtex) id= (ID *)mtex->tex;
				
					idfrom= (ID *)ma;
					xco= std_libbuttons(xco, 0, B_TEXBROWSE, id, idfrom, &(G.buts->texnr), B_TEXALONE, B_TEXLOCAL, B_TEXDELETE, B_AUTOTEXNAME);
					G.buts->lockpoin= id;
				}
			}
		}
		else if(G.buts->texfrom==1) {
			wrld= G.scene->world;
			if(wrld) {
				id= 0;
				mtex= wrld->mtex[ wrld->texact];
				if(mtex) id= (ID *)mtex->tex;
				idfrom= (ID *)wrld;

				xco= std_libbuttons(xco, 0, B_WTEXBROWSE, id, idfrom, &(G.buts->texnr), B_TEXALONE, B_TEXLOCAL, B_TEXDELETE, B_AUTOTEXNAME);
				G.buts->lockpoin= id;
			}
		}
		else if(G.buts->texfrom==2) {
			Lamp *la;
			if(ob && ob->type==OB_LAMP) {
				la= ob->data;
			
				id= 0;
				mtex= la->mtex[ la->texact];
				if(mtex) id= (ID *)mtex->tex;
				idfrom= (ID *)la;

				xco= std_libbuttons(xco, 0, B_LTEXBROWSE, id, idfrom, &(G.buts->texnr), B_TEXALONE, B_TEXLOCAL, B_TEXDELETE, B_AUTOTEXNAME);
				G.buts->lockpoin= id;
			}
		}
	}
	else if(G.buts->mainb==BUTS_ANIM) {
		if(ob) {
			idfrom= (ID *)G.scene;
			id= (ID *)ob;
			G.buts->lockpoin= id;
			xco= std_libbuttons(xco, 0, 0, id, idfrom, &(G.buts->menunr), B_OBALONE, B_OBLOCAL, 0, 0);
		}
	}
	else if(G.buts->mainb==BUTS_WORLD) {
		id= (ID *)G.scene->world;
		idfrom= (ID *)G.scene;
		xco= std_libbuttons(xco, 0, B_WORLDBROWSE, id, idfrom, &(G.buts->menunr), B_WORLDALONE, B_WORLDLOCAL, B_WORLDDELETE, 0);
		G.buts->lockpoin= id;
	}
	else if(G.buts->mainb==BUTS_RENDER) {
		xco= std_libbuttons(xco, 0, B_INFOSCE, (ID *)G.scene, 0, &(G.curscreen->scenenr), 1, 1, B_INFODELSCE, 0);
		G.buts->lockpoin= G.scene;
		
	}
	else if(G.buts->mainb==BUTS_EDIT) {
		if(ob && ob->data) {
			
			id= ob->data;
			idfrom= (ID *)ob;
			alone= 0;
			local= 0;
			browse= B_EDITBROWSE;
			xco+= 10;
			
			if ELEM3(ob->type, OB_MESH, OB_SECTOR, OB_LIFE) {
				id= (ID *)get_mesh(ob);
				browse= B_MESHBROWSE;
				alone= B_MESHALONE;
				local= B_MESHLOCAL;
				SetButLock(G.obedit!=0, "Unable to perform function in EditMode");
				if(ob->type==OB_LIFE)   DefBut(ICONTOG|SHO|BIT|2, B_MESHTYPE, "ICON 0 10 3",	xco-17,0,XIC-3,YIC, &((Life *)ob->data)->flag, 0, 0, 0, 0);
				if(ob->type==OB_SECTOR) DefBut(ICONTOG|SHO|BIT|0, B_MESHTYPE, "ICON 0 10 3",	xco-17,0,XIC-3,YIC, &((Sector *)ob->data)->flag, 0, 0, 0, 0);
			}
			else if(ob->type==OB_MBALL) {
				alone= B_MBALLALONE;
				local= B_MBALLLOCAL;
			}
			else if ELEM3(ob->type, OB_CURVE, OB_FONT, OB_SURF) {
				alone= B_CURVEALONE;
				local= B_CURVELOCAL;
			}
			else if(ob->type==OB_CAMERA) {
				alone= B_CAMERAALONE;
				local= B_CAMERALOCAL;
			}
			else if(ob->type==OB_LAMP) {
				alone= B_LAMPALONE;
				local= B_LAMPLOCAL;
			}
			else if(ob->type==OB_LATTICE) {
				alone= B_LATTALONE;
				local= B_LATTLOCAL;
			}
			
			xco= std_libbuttons(xco, 0, browse, id, idfrom, &(G.buts->menunr), alone, local, 0, 0);
			G.buts->lockpoin= id;
			
			xco+= XIC;
		}
		if(ob) {
			SetButFunc(test_idbutton);
			DefButt(TEX, B_IDNAME, "OB:",	xco, 0, 135, YIC, ob->id.name+2, 0.0, 19.0, 0, 0, "Active Object name");
			xco+= 135;
		}
	}
	else if(G.buts->mainb==BUTS_GAME) {
		if(ob && (ob->type==OB_SECTOR || ob->type==OB_LIFE) ) {
			id= ob->data;	
			idfrom= (ID *)ob;
			xco= std_libbuttons(xco, 0, 0, id, idfrom, &(G.buts->menunr), 0, 0, 0, 0);
			G.buts->lockpoin= id;
		}
		if(ob) {
			xco+= XIC;
			SetButFunc(test_idbutton);
			DefButt(TEX, B_IDNAME, "OB:",	xco, 0, 135, YIC, ob->id.name+2, 0.0, 19.0, 0, 0, "Active Object name");
			xco+= 135;
		}
	} 
	else if(G.buts->mainb==BUTS_SCRIPT) {
		if(ob)
			DefButt(ROW|SHO, B_REDR, "ICON 0 7 8", xco,0,XIC,YIC, &G.buts->scriptblock,  2.0, (float)ID_OB, 0, 0, "Display Object script links");

		if(ob && (ma=give_current_material(ob, ob->actcol)))
			DefButt(ROW|SHO, B_REDR, "ICON 0 8 4",	xco+=XIC,0,XIC,YIC, &G.buts->scriptblock, 2.0, (float)ID_MA, 0, 0, "Display Material script links ");

		if(G.scene->world) 
			DefButt(ROW|SHO, B_REDR, "ICON 0 5 7",	xco+=XIC,0,XIC,YIC, &G.buts->scriptblock, 2.0, (float)ID_WO, 0, 0, "Display World script links");
	
		if(ob && ob->type==OB_CAMERA)
			DefButt(ROW|SHO, B_REDR, "ICON 0 2 1",	xco+=XIC,0,XIC,YIC, &G.buts->scriptblock, 2.0, (float)ID_CA, 0, 0, "Display Camera script links");

		if(ob && ob->type==OB_LAMP)
			DefButt(ROW|SHO, B_REDR, "ICON 0 1 7",	xco+=XIC,0,XIC,YIC, &G.buts->scriptblock, 2.0, (float)ID_LA, 0, 0, "Display Lamp script links");

		xco+= 20;
	}
	
	DefButt(NUM|SHO, B_NEWFRAME, "",		xco+20,0,60,YIC, &(G.scene->r.cfra), 1.0, 18000.0, 0, 0, "Current Frame");
	xco+= 80;

	G.buts->mainbo= G.buts->mainb;

	/* altijd als laatste doen */
	curarea->headbutlen= xco;
}

/* ********************** BUTS ****************************** */
/* ******************** FILE ********************** */

void do_file_buttons(short event)
{
	SpaceFile *sfile;
	
	if(curarea->win==0) return;
	sfile= curarea->spacedata.first;
	
	switch(event) {
	case B_SORTFILELIST:
		sort_filelist(sfile);
		addqueue(curarea->win, REDRAW, 1);
		break;
	case B_RELOADDIR:
		freefilelist(sfile);
		addqueue(curarea->win, REDRAW, 1);
		break;
	}
	
}

void file_buttons()
{
	SpaceFile *sfile;
	float df, totlen, sellen;
	int xco, totfile, selfile;
	char naam[256];

	sfile= curarea->spacedata.first;

	sprintf(naam, "header %d", curarea->headwin);
	DefButBlock(naam, curarea->headwin, G.font, 35, 2, 2);

	DefButt(ICONROW|CHA,B_NEWSPACE, "ICON 0 0 0", 6,0,XIC,YIC, &(curarea->butspacetype), 1.0, 10.0, 0, 0, "Current buttons window (LeftMouse+drag to change)");

	/* FULL WINDOW */
	xco= 25;
	if(curarea->full) DefButt(BUT,B_FULL, "ICON 0 1 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Restore smaller windows (CTRL+Up arrow)");
	else DefButt(BUT,B_FULL, "ICON 0 0 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Make fullscreen window (CTRL+Down arrow)");
	
	/* SORT TYPE */
	xco+=XIC;
	DefButt(ROW|SHO, B_SORTFILELIST, "ICON 0 14 1",	xco+=XIC,0,XIC,YIC, &sfile->sort, 1.0, 0.0, 0, 0, "Sort files alphabetically");
	DefButt(ROW|SHO, B_SORTFILELIST, "ICON 0 15 1",	xco+=XIC,0,XIC,YIC, &sfile->sort, 1.0, 1.0, 0, 0, "Sort files by time");
	DefButt(ROW|SHO, B_SORTFILELIST, "ICON 0 16 1",	xco+=XIC,0,XIC,YIC, &sfile->sort, 1.0, 2.0, 0, 0, "Sort files by size");	

	cpack(0x0);
	glRasterPos2i(xco+=XIC+10,  5);
	fmprstr(sfile->title);
	
	xco+= fmgetstrwidth(G.font, sfile->title);
	
	DefButt(ICONTOG|SHO|BIT|0, B_REDR, "ICON 0 17 1",xco+=XIC,0,XIC,YIC, &sfile->flag, 0, 0, 0, 0, "Toggle column mode");

	DefButt(TOG|SHO|BIT|3, B_RELOADDIR, "ICON 0 10 5",xco+=XIC,0,XIC,YIC, &sfile->flag, 0, 0, 0, 0, "Hide dot files");

	xco+=XIC+10;

	if(sfile->type==FILE_LOADLIB) {
		DefButt(TOGN|SHO|BIT|2, B_REDR, "Append",		xco+=XIC,0,100,YIC, &sfile->flag, 0, 0, 0, 0, "Causes selected data to be copied into current file");
		DefButt(TOG|SHO|BIT|2, B_REDR, "Link",	xco+=100,0,100,YIC, &sfile->flag, 0, 0, 0, 0, "Causes selected data to be linked by current file");
	}

	if(sfile->type & FILE_UNIX) {
		df= diskfree(sfile->dir)/(1048576.0);

		filesel_statistics(sfile, &totfile, &selfile, &totlen, &sellen);
		
		sprintf(naam, "Free: %.3f Mb   Files: (%d) %d    (%.3f) %.3f Mb", 
					df, selfile,totfile, sellen, totlen);
		
		cpack(0x0);
		glRasterPos2i(xco,  5);
		fmprstr(naam);
	}
	/* altijd als laatste doen */
	curarea->headbutlen= xco+2*XIC;
	
}


/* ********************** FILE ****************************** */
/* ******************** OOPS ********************** */

void do_oops_buttons(short event)
{
	float dx, dy;
	
	if(curarea->win==0) return;

	switch(event) {
	case B_OOPSHOME:
		boundbox_oops();
		G.v2d->cur= G.v2d->tot;
		dx= 0.15*(G.v2d->cur.xmax-G.v2d->cur.xmin);
		dy= 0.15*(G.v2d->cur.ymax-G.v2d->cur.ymin);
		G.v2d->cur.xmin-= dx;
		G.v2d->cur.xmax+= dx;
		G.v2d->cur.ymin-= dy;
		G.v2d->cur.ymax+= dy;		
		test_view2d();
		addqueue(curarea->win, REDRAW, 1);
		break;
		
	case B_NEWOOPS:
		addqueue(curarea->win, REDRAW, 1);
		addqueue(curarea->headwin, REDRAW, 1);
		G.soops->lockpoin= 0;
		break;
	}
	
}

void oops_buttons()
{
	SpaceOops *soops;
	Oops *oops;
	int xco;
	char naam[256];

	soops= curarea->spacedata.first;

	sprintf(naam, "header %d", curarea->headwin);
	DefButBlock(naam, curarea->headwin, G.font, 35, 3, 2);

	DefButt(ICONROW|CHA,B_NEWSPACE, "ICON 0 0 0", 6,0,XIC,YIC, &(curarea->butspacetype), 1.0, 10.0, 0, 0, "Current window type (LeftMouse+drag to change)");

	/* FULL WINDOW */
	xco= 25;
	if(curarea->full) DefButt(BUT,B_FULL, "ICON 0 1 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Restore smaller windows (CTRL+Up arrow)");
	else DefButt(BUT,B_FULL, "ICON 0 0 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Make fullscreen window (CTRL+Down arrow)");
	
	/* HOME */
	DefButt(BUT, B_OOPSHOME, "ICON 0 15 0",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Home (HOMEKEY)");	
	xco+= XIC;
	
	/* ZOOM en BORDER */
	xco+= XIC;
	if(curarea->win) SetButFunc(view2dzoom);
	DefButt(BUT, 0, "ICON 0 13 0",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Zoom view (CTRL+MiddleMouse)");
	DefButt(BUT, B_OOPSBORDER, "ICON 0 8 2",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Translate view (MiddleMouse)");

	/* VISIBLE */
	xco+= XIC;
	DefButt(TOG|SHO|BIT|10,B_NEWOOPS, "lay",		xco+=XIC,0,XIC+10,YIC, &soops->visiflag, 0, 0, 0, 0, "Display Objects based on layer");
	DefButt(TOG|SHO|BIT|0, B_NEWOOPS, "ICON 0 6 8",	xco+=XIC+10,0,XIC,YIC, &soops->visiflag, 0, 0, 0, 0, "Display Scene data");
	DefButt(TOG|SHO|BIT|1, B_NEWOOPS, "ICON 0 7 8",	xco+=XIC,0,XIC,YIC, &soops->visiflag, 0, 0, 0, 0, "Display Object data");
	DefButt(TOG|SHO|BIT|2, B_NEWOOPS, "ICON 0 8 8",	xco+=XIC,0,XIC,YIC, &soops->visiflag, 0, 0, 0, 0, "Display Mesh data");
	DefButt(TOG|SHO|BIT|3, B_NEWOOPS, "ICON 0 9 8",	xco+=XIC,0,XIC,YIC, &soops->visiflag, 0, 0, 0, 0, "Display Curve/Surface/Font data");
	DefButt(TOG|SHO|BIT|4, B_NEWOOPS, "ICON 0 10 8",	xco+=XIC,0,XIC,YIC, &soops->visiflag, 0, 0, 0, 0, "Display Metaball data");
	DefButt(TOG|SHO|BIT|5, B_NEWOOPS, "ICON 0 11 8",	xco+=XIC,0,XIC,YIC, &soops->visiflag, 0, 0, 0, 0, "Display Lattice data");
	DefButt(TOG|SHO|BIT|6, B_NEWOOPS, "ICON 0 12 8",	xco+=XIC,0,XIC,YIC, &soops->visiflag, 0, 0, 0, 0, "Display Lamp data");
	DefButt(TOG|SHO|BIT|7, B_NEWOOPS, "ICON 0 13 8",	xco+=XIC,0,XIC,YIC, &soops->visiflag, 0, 0, 0, 0, "Display Material data");
	DefButt(TOG|SHO|BIT|8, B_NEWOOPS, "ICON 0 14 8",	xco+=XIC,0,XIC,YIC, &soops->visiflag, 0, 0, 0, 0, "Display Texture data");
	DefButt(TOG|SHO|BIT|9, B_NEWOOPS, "ICON 0 15 8",	xco+=XIC,0,XIC,YIC, &soops->visiflag, 0, 0, 0, 0, "Display Ipo data");
	DefButt(TOG|SHO|BIT|12, B_NEWOOPS, "ICON 0 17 8",	xco+=XIC,0,XIC,YIC, &soops->visiflag, 0, 0, 0, 0, "Display Image data");
	DefButt(TOG|SHO|BIT|11, B_NEWOOPS, "ICON 0 16 8",	xco+=XIC,0,XIC,YIC, &soops->visiflag, 0, 0, 0, 0, "Display Library data");

	/* naam */
	if(G.soops->lockpoin) {
		oops= G.soops->lockpoin;
		if(oops->type==ID_LI) strcpy(naam, ((Library *)oops->id)->name);
		else strcpy(naam, oops->id->name);
		
		cpack(0x0);
		glRasterPos2i(xco+=XIC+10,  5);
		fmprstr(naam);

	}

	/* altijd als laatste doen */
	curarea->headbutlen= xco+2*XIC;
	
}


/* ********************** OOPS ****************************** */
/* ******************** PAINT ********************** */

/* bpaint.c */

/* ********************** TEXT ****************************** */

void do_text_buttons(ushort event)
{
	SpaceText *st= curarea->spacedata.first;
	ID *id, *idtest;
	int nr= 0;
	Text *text;
		
	if (!st) return;
	if (st->spacetype != SPACE_TEXT) return;
	
	switch (event) {
	case B_TEXTBROWSE:

		if(st->menunr < 0) break;
			
		text= st->text;

		nr= 0;
		id= (ID *)text;
		
		if (st->menunr==32767) {
			st->text= add_empty_text();

			st->top= 0;
			
			allqueue(REDRAWTEXT, 0);
			allqueue(REDRAWHEADERS, 0);	
		}
		else if (st->menunr==32766) {
			activate_fileselect(FILE_SPECIAL, "LOAD TEXT FILE", G.sce, add_text_fs); 
			return;
		}
		else {		
			idtest= G.main->text.first;
			while(idtest) {
				if(nr==st->menunr) {
					break;
				}
				nr++;
				idtest= idtest->next;
			}
			if(idtest==0) {	/* new text */
				activate_fileselect(FILE_SPECIAL, "LOAD TEXT FILE", G.sce, add_text_fs); 
				return;
			}
			if(idtest!=id) {
				st->text= (Text *)idtest;
				st->top= 0;
				
				pop_space_text(st);
			
				allqueue(REDRAWTEXT, 0);
				allqueue(REDRAWHEADERS, 0);
			}
		}
		break;
				
	case B_TEXTDELETE:
		if(!okee("Really delete text?")) return;
		
		text= st->text;
		if (!text) return;
		
		clear_bad_scriptlinks(text);
	
		free_libblock(&G.main->text, text);

		break;
		
/*
	case B_TEXTSTORE:
		st->text->flags ^= TXT_ISEXT;
		
		allqueue(REDRAWHEADERS, 0);
		break;
*/		 
	case B_TEXTFONT:
		switch(st->font_id) {
		case 0:
			st->lheight= 11; break;
		case 1:
			st->lheight= 13; break;
		}

		allqueue(REDRAWTEXT, 0);
		allqueue(REDRAWHEADERS, 0);

		break;
	}
}

void text_buttons()
{
	int xco;
	char naam[256];
	char *str;
	SpaceText *st= curarea->spacedata.first;
	Text *text;
	
	if (!st || st->spacetype != SPACE_TEXT) return;

	sprintf(naam, "header %d", curarea->headwin);
	DefButBlock(naam, curarea->headwin, G.font, 35, 2, 2);

	DefButt(ICONROW|CHA,B_NEWSPACE, "ICON 0 0 0", 6,0,XIC,YIC, &(curarea->butspacetype), 1.0, 10.0, 0, 0, "Current window type (LeftMouse+drag to change)");

	/* FULL WINDOW */
	xco= 25;
	if(curarea->full) DefButt(BUT,B_FULL, "ICON 0 1 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Restore smaller windows (CTRL+Up arrow)");
	else DefButt(BUT,B_FULL, "ICON 0 0 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Make fullscreen window (CTRL+Down arrow)");
		

	/* STD TEXT BUTTONS */

	xco+= 2*XIC;
	xco= std_libbuttons(xco, 0, B_TEXTBROWSE, (ID*)st->text, 0, &(st->menunr), 0, 0, B_TEXTDELETE, 0);

	text= st->text;
/*
	if (text) {
		if (text->flags & TXT_ISDIRTY && (text->flags & TXT_ISEXT || !(text->flags & TXT_ISMEM)))
			DefButt(BUT,0, "ICON 0 1 5", xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "The text has been changed");

		if (text->flags & TXT_ISEXT) 
			DefButt(BUT,B_TEXTSTORE, "ICON 0 2 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Store text in .blend file");
		else 
			DefButt(BUT,B_TEXTSTORE, "ICON 0 3 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Don't store text in .blend file");
			
		xco+=10;
	}
*/

	xco+=XIC;
	if(st->font_id>1) st->font_id= 0;
	DefButt(MENU|INT, B_TEXTFONT, "Screen 12 %x0|Screen 15%x1", xco,0,XIC+120,YIC, &st->font_id, 0, 0, 0, 0, "Font display menu");
	xco+=XIC+120;
	
	/* always as last  */
	curarea->headbutlen= xco+2*XIC;
}


/* ******************** IMAGE ********************** */

void load_space_image(char *str)	/* aangeroepen vanuit fileselect */
{
	Image *ima=0;
	
	ima= add_image(str);
	if(ima) {
		
		G.sima->image= ima;
		
		free_image(ima);	/* forceer opnieuw inlezen */
		ima->ok= 1;
		image_changed(G.sima, 0);
		
	}
	allqueue(REDRAWIMAGE, 0);
	allqueue(REDRAWPAINT, 0);

}

void image_replace(Image *old, Image *new)
{
	TFace *tface;
	Mesh *me;
	int a, rep=0;
	
	new->tpageflag= old->tpageflag;
	new->twsta= old->twsta;
	new->twend= old->twend;
	new->xrep= old->xrep;
	new->yrep= old->yrep;
	
	me= G.main->mesh.first;
	while(me) {
		
		if(me->tface) {
			tface= me->tface;
			a= me->totface;
			while(a--) {
				if(tface->tpage==old) {
					tface->tpage= new;
					rep++;
				}
				tface++;
			}
		}
		me= me->id.next;
		
	}
	if(rep) {
		if(new->id.us==0) new->id.us= 1;
	}
	else error("Nothing replaced");
}

void replace_space_image(char *str)		/* aangeroepen vanuit fileselect */
{
	Image *ima=0;
	
	ima= add_image(str);
	if(ima) {
		
		if(G.sima->image != ima) {
			image_replace(G.sima->image, ima);
		}
		
		G.sima->image= ima;
		
		free_image(ima);	/* forceer opnieuw inlezen */
		ima->ok= 1;
		/* replace kent ook toe: */
		image_changed(G.sima, 0);
		
	}
	allqueue(REDRAWIMAGE, 0);
	allqueue(REDRAWPAINT, 0);

}


void do_image_buttons(ushort event)
{
	Image *ima;
	ID *id, *idtest;
	int nr;
	char name[256];
	
	if(curarea->win==0) return;
	
	switch(event) {
	case B_SIMAGEHOME:
		image_home();
		break;
		
	case B_SIMABROWSE:	
		if(G.sima->imanr== -2) {
			activate_databrowse((ID *)G.sima->image, ID_IM, 0, B_SIMABROWSE, do_image_buttons);
			return;
		}
		if(G.sima->imanr < 0) break;
	
		nr= 0;
		id= (ID *)G.sima->image;

		idtest= G.main->image.first;
		while(idtest) {
			if(nr==G.sima->imanr) {
				break;
			}
			nr++;
			idtest= idtest->next;
		}
		if(idtest==0) {	/* geen new */
			return;
		}
	
		if(idtest!=id) {
			G.sima->image= (Image *)idtest;
			allqueue(REDRAWIMAGE, 0);
		}
		image_changed(G.sima, 0);	/* ook als image gelijk is: assign! 0==geen tileflag */
		
		break;
	case B_SIMAGELOAD:
		
		if(G.sima->image) strcpy(name, G.sima->image->name);
		else strcpy(name, U.textudir);
		
		activate_fileselect(FILE_SPECIAL, "SELECT IMAGE", name, load_space_image);
		
		break;
	case B_SIMAGEREPLACE:
		
		if(G.sima->image) strcpy(name, G.sima->image->name);
		else strcpy(name, U.textudir);
		
		activate_fileselect(FILE_SPECIAL, "REPLACE IMAGE", name, replace_space_image);
		
		break;
	case B_SIMAGEDRAW:
		
		if(G.f & G_FACESELECT) {
			make_repbind(G.sima->image);
			image_changed(G.sima, 1);
		}
		allqueue(REDRAWVIEW3D, 0);
		allqueue(REDRAWIMAGE, 0);
		break;

	case B_SIMAGEDRAW1:
		image_changed(G.sima, 2);		/* 2: alleen tileflag */
		allqueue(REDRAWVIEW3D, 0);
		allqueue(REDRAWIMAGE, 0);
		break;
		
	case B_TWINANIM:
		if(ima=G.sima->image) {
			if(ima->flag & IMA_TWINANIM) {
				nr= ima->xrep*ima->yrep;
				if(ima->twsta>=nr) ima->twsta= 1;
				if(ima->twend>=nr) ima->twend= nr-1;
				if(ima->twsta>ima->twend) ima->twsta= 1;
				allqueue(REDRAWIMAGE, 0);
			}
		}
		break;
	}
	
}

void image_buttons()
{
	int xco;
	char naam[256];

	sprintf(naam, "header %d", curarea->headwin);
	DefButBlock(naam, curarea->headwin, G.font, 35, 2, 2);

	DefButt(ICONROW|CHA,B_NEWSPACE, "ICON 0 0 0", 6,0,XIC,YIC, &(curarea->butspacetype), 1.0, 10.0, 0, 0, "Current window type (LeftMouse+drag to change)");

	/* FULL WINDOW */
	xco= 25;
	if(curarea->full) DefButt(BUT,B_FULL, "ICON 0 1 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Restore smaller windows (CTRL+Up arrow)");
	else DefButt(BUT,B_FULL, "ICON 0 0 8",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Make fullscreen window (CTRL+Down arrow)");
	
	/* HOME*/
	DefButt(BUT, B_SIMAGEHOME, "ICON 0 15 0",	xco+=XIC,0,XIC,YIC, 0, 0, 0, 0, 0, "Home (HOMEKEY)");
#ifndef FREE
	DefBut(TOG|SHO|BIT|0, B_BE_SQUARE, "ICON 0 12 4",	xco+=XIC,0,XIC,YIC, &G.sima->flag, 0, 0, 0, 0);
#endif

	xco= std_libbuttons(xco+40, 0, B_SIMABROWSE, (ID *)G.sima->image, 0, &(G.sima->imanr), 0, 0, 0, 0);
	
	DefButt(BUT, B_SIMAGELOAD, "Load",		xco,0,2*XIC,YIC, 0, 0, 0, 0, 0, "Load Image");
	xco+= 2*XIC;
	DefButt(BUT, B_SIMAGEREPLACE, "Replace",	xco,0,3*XIC,YIC, 0, 0, 0, 0, 0, "Replace current Image");
	
#ifndef FREE
	if(G.sima->image) {
		xco+= 3*XIC;
		DefBut(TOG|SHO|BIT|0, B_SIMAGEDRAW1, "ICON 0 20 6", xco+=XIC,0,XIC,YIC, &G.sima->image->tpageflag, 0, 0, 0, 0);
		DefBut(NUM|SHO, B_SIMAGEDRAW, "",	xco+=XIC,0,XIC,YIC, &G.sima->image->xrep, 1.0, 16.0, 0, 0);
		DefBut(NUM|SHO, B_SIMAGEDRAW, "",	xco+=XIC,0,XIC,YIC, &G.sima->image->yrep, 1.0, 16.0, 0, 0);
		
		DefBut(TOG|SHO|BIT|1, B_TWINANIM, "Anim", xco+=XIC,0,2*XIC,YIC, &G.sima->image->tpageflag, 0, 0, 0, 0);
		DefBut(NUM|SHO, B_TWINANIM, "",	xco+=2*XIC,0,XIC,YIC, &G.sima->image->twsta, 0.0, 128.0, 0, 0);
		DefBut(NUM|SHO, B_TWINANIM, "",	xco+=XIC,0,XIC,YIC, &G.sima->image->twend, 0.0, 128.0, 0, 0);
		DefBut(TOG|SHO|BIT|2, 0, "Cycle", xco+=XIC,0,2*XIC,YIC, &G.sima->image->tpageflag, 0, 0, 0, 0);
	}
#endif
	
	/* altijd als laatste doen */
	curarea->headbutlen= xco+2*XIC;
	
}


/* ********************** IMAGE ****************************** */
/* ******************** IMASEL ********************** */

/* naar editimasel.c */

/* ********************** IMASEL ****************************** */

/* ******************** ALGEMEEN ********************** */

void do_headerbuttons(short event)
{

	if(event<=50) do_global_buttons2(event);
	else if(event<=100) do_global_buttons(event);
	else if(event<200) do_view3d_buttons(event);
	else if(event<250) do_ipo_buttons(event);
	else if(event<300) do_oops_buttons(event);
	else if(event<350) do_info_buttons(event);
	else if(event<400) do_image_buttons(event);
	else if(event<450) do_buts_buttons(event);
	else if(event<500) do_imasel_buttons(event);
	else if(event<550) do_text_buttons(event);
	else if(event<600) do_file_buttons(event);
	else if(event<650) do_seq_buttons(event);

}


