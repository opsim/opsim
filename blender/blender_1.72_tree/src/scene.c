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


/*  scene.c      MIXED MODEL
 * 
 *  jan 95
 *  
 * 
 */

#include "blender.h"
#include "ika.h"


/* niet scene zelf vrijgeven */
void free_scene(Scene *sce)
{
	Base *base;

	base= sce->base.first;
	while(base) {
		base->object->id.us--;
		base= base->next;
	}
	/* pas op: niet objects vrijgeven! */

	freelistN(&sce->base);
	free_editing(sce->ed);
	if(sce->radio) freeN(sce->radio);
	sce->radio= 0;
	
	free_scriptlink(&sce->scriptlink);
}

Scene *add_scene(char *name)
{
	Scene *sce;
	
	sce= alloc_libblock(&G.main->scene, ID_SCE, name);
	sce->lay= 1;

	sce->r.mode= R_GAMMA;
	sce->r.cfra= 1;
	sce->r.sfra= 1;
	sce->r.efra= 250;
	sce->r.xsch= 320;
	sce->r.ysch= 256;
	sce->r.xasp= 1;
	sce->r.yasp= 1;
	sce->r.xparts= 1;
	sce->r.yparts= 1;
	sce->r.size= 100;
	sce->r.planes= 24;
	sce->r.quality= 90;
	sce->r.framapto= 100;
	sce->r.images= 100;
	sce->r.framelen= 1.0;
	sce->r.frs_sec= 25;
	
	strcpy(sce->r.backbuf, "//backbuf");
	strcpy(sce->r.pic, U.renderdir);
	strcpy(sce->r.ftype, "//ftype");
	
	init_rctf(&sce->r.safety, 0.1, 0.9, 0.1, 0.9);
	sce->r.osa= 8;
	
	sce->grav= 0.1;
	sce->maxdrawsector= 12;
	
	return sce;
}

Scene *copy_scene(Scene *sce, int level)
{
	/* level 0: alle objects shared
	 * level 1: alle objectdata shared
	 * level 2: volledige kopie
	 */
	Scene *scen;
	ID *id;
	Base *base, *obase;
	Object *ob;
	Curve *cu;
	Mesh *me;
	MetaBall *mb;
	Material *ma;
	Lamp *la;
	int a;
	
	/* level 0 */
	scen= copy_libblock(sce);
	duplicatelist(&(scen->base), &(sce->base));
	
	clear_id_newpoins();
	
	id_us_plus((ID *)scen->world);
	id_us_plus((ID *)scen->set);
	
	scen->ed= 0;
	scen->radio= 0;
	
	obase= sce->base.first;
	base= scen->base.first;
	while(base) {
		base->object->id.us++;
		if(obase==sce->basact) scen->basact= base;
		
		obase= obase->next;
		base= base->next;
	}
	
	if(level==0) return scen;
	
	/* level 1 */
	G.scene= scen;
	single_object_users(0);

	/*  camera */
	ID_NEW(G.scene->camera);
		
	/* level 2 */
	if(level>=2) {
		if(scen->world) {
			scen->world->id.us--;
			scen->world= copy_world(scen->world);
		}
		single_obdata_users(0);
		single_mat_users_expand();
		single_tex_users_expand();
	}

	clear_id_newpoins();

	copy_scriptlink(&sce->scriptlink);

	return scen;
}

int object_in_scene(Object *ob, Scene *sce)
{
	Base *base;
	
	base= sce->base.first;
	while(base) {
		if(base->object == ob) return 1;
		base= base->next;
	}
	return 0;
}

void sort_baselist(Scene *sce)
{
	/* alles in volgorde van parent en track */
	ListBase tempbase, noparentbase, notyetbase;
	Base *base, *test;
	Object *par;
	int doit, domore= 0, lastdomore=1;
	
	
	/* volgorde gelijk houden als er niets veranderd is! */
	/* hier waren problemen met campos array's: volgorde camera's is van belang */
	
	while(domore!=lastdomore) {

		lastdomore= domore;
		domore= 0;
		tempbase.first= tempbase.last= 0;
		noparentbase.first= noparentbase.last= 0;
		notyetbase.first= notyetbase.last= 0;
		
		while(base= sce->base.first) {
			remlink(&sce->base, base);
			
			par= 0;
			if(base->object->type==OB_IKA) {
				Ika *ika= base->object->data;
				par= ika->parent;
			}

			if(par || base->object->parent || base->object->track) {
				
				doit= 0;
				if(base->object->parent) doit++;
				if(base->object->track) doit++;
				if(par) doit++;
				
				test= tempbase.first;
				while(test) {
					
					if(test->object==base->object->parent) doit--;
					if(test->object==base->object->track) doit--;
					if(test->object==par) doit--;
					
					if(doit==0) break;
					test= test->next;
				}
				
				if(test) insertlink(&tempbase, test, base);
				else {
					addhead(&tempbase, base);
					domore++;
				}
				
			}
			else addtail(&noparentbase, base);
			
		}
		sce->base= noparentbase;
		addlisttolist(&sce->base, &tempbase);
		addlisttolist(&sce->base, &notyetbase);

	}
}


void set_scene_bg(Scene *sce)
{
	Base *base;
	Object *ob;
	
	G.scene= sce;
	
	/* objecten deselecteren (voor dataselect) */
	ob= G.main->object.first;
	while(ob) {
		ob->flag &= ~SELECT;
		ob= ob->id.next;
	}

	/* baselijst sorteren */
	sort_baselist(sce);

	/* layers en flags uit bases naar objecten kopieeren */
	base= FIRSTBASE;
	while(base) {
		
		base->object->lay= base->lay;
		base->object->flag= base->flag;
		base->object->ctime= -1234567.0;	/* forceer ipo */
		base= base->next;
	}

	do_all_ipos();	/* layers/materials */
	do_all_scripts(SCRIPT_FRAMECHANGED);
	do_all_keys();
	do_all_ikas();
}

void set_scene_name(char *name)
{
	Scene *sce;
	char str[128];
	
	
	sce= G.main->scene.first;
	while(sce) {
		if(strcmp(name, sce->id.name+2)==0) {
			set_scene_bg(sce);
			return;
		}
		sce= sce->id.next;
	}
	sprintf(str, "Can't find scene: %s", name);
	
	error(str);
}

/* used by metaballs
 * doesnt return the original duplicated object, only dupli's
 */
int next_object(int val, Base **base, Object **ob)
{
	extern ListBase duplilist;
	static Object *dupob;
	static int fase;
	int run_again=1;
	
	/* init */
	if(val==0) {
		fase= F_START;
		dupob= 0;
	}
	else {
		
		/* run_again is set when a duplilist has been ended */
		while(run_again) {
			run_again= 0;
			
				
				
			/* de eerste base */
			if(fase==F_START) {
				*base= FIRSTBASE;
				if(*base) {
					*ob= (*base)->object;
					fase= F_SCENE;
				}
				else {
					/* uitzondering: een lege scene */
					if(G.scene->set && G.scene->set->base.first) {
						*base= G.scene->set->base.first;
						*ob= (*base)->object;
						fase= F_SET;
					}
				}
			}
			else {
				if(*base && fase!=F_DUPLI) {
					*base= (*base)->next;
					if(*base) *ob= (*base)->object;
					else {
						if(fase==F_SCENE) {
							/* de scene is klaar, we gaan door met de set */
							if(G.scene->set && G.scene->set->base.first) {
								*base= G.scene->set->base.first;
								*ob= (*base)->object;
								fase= F_SET;
							}
						}
					}
				}
			}
			
			if(*base == 0) fase= F_START;
			else {
				if(fase!=F_DUPLI) {
					if( (*base)->object->transflag & OB_DUPLI) {
						
						make_duplilist(G.scene, (*base)->object);
						dupob= duplilist.first;
						
					}
				}
				/* dupli's afhandelen */
				if(dupob) {
					
					*ob= dupob;
					fase= F_DUPLI;
					
					dupob= dupob->id.next;
				}
				else if(fase==F_DUPLI) {
					fase= F_SCENE;
					free_duplilist();
					run_again= 1;
				}
				
			}
		}
	}
	
	return fase;
}

