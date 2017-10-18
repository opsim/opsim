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


/*  library.c   aug 94     MIXED MODEL
 * 
 *  jan 95
 * 
 *  afhandeling ID's en libraries
 *  allocceren en vrijgeven alle library data
 * 
 */

#include "blender.h"
#include "screen.h"
#include "sector.h"
#include "render.h"
#include "ika.h"

#define MAX_IDPUP	24

/* ************* ALGEMEEN ************************ */

void id_lib_extern(ID *id)
{
	if(id) {
		if(id->flag & LIB_INDIRECT) {
			id->flag -= LIB_INDIRECT;
			id->flag |= LIB_EXTERN;
		}
	}
	
}

void id_us_plus(ID *id)
{
	
	if(id) {
		id->us++;
		if(id->flag & LIB_INDIRECT) {
			id->flag -= LIB_INDIRECT;
			id->flag |= LIB_EXTERN;
		}
	}
}


ListBase *wich_libbase(Main *main, short type)
{
	switch( type ) {
	
		case ID_SCE:
			return &(main->scene);
		case ID_LI:
			return &(main->library);
		case ID_OB:
			return &(main->object);
		case ID_ME:
			return &(main->mesh);
		case ID_CU:
			return &(main->curve);
		case ID_MB:
			return &(main->mball);
		case ID_MA:
			return &(main->mat);
		case ID_TE:
			return &(main->tex);
		case ID_IM:
			return &(main->image);
		case ID_IK:
			return &(main->ika);
		case ID_WV:
			return &(main->wave);
		case ID_LT:
			return &(main->latt);
		case ID_SE:
			return &(main->sector);
		case ID_LF:
			return &(main->life);
		case ID_LA:
			return &(main->lamp);
		case ID_CA:
			return &(main->camera);
		case ID_IP:
			return &(main->ipo);
		case ID_KE:
			return &(main->key);
		case ID_WO:
			return &(main->world);
		case ID_SCR:
			return &(main->screen);
		case ID_VF:
			return &(main->vfont);
		case ID_TXT:
			return &(main->text);
	}
	return 0;
}

int set_listbasepointers(Main *main, ListBase **lb)
{

	/* BACKWARDS! let op volgorde van vrijgeven! (mesh<->mat) */

	lb[0]= &(main->ipo);
	lb[1]= &(main->key);
	lb[2]= &(main->image);
	lb[3]= &(main->tex);
	lb[4]= &(main->mat);
	lb[5]= &(main->vfont);
	
	lb[6]= &(main->mesh);
	lb[7]= &(main->curve);
	lb[8]= &(main->mball);
	lb[9]= &(main->ika);
	lb[10]= &(main->wave);
	lb[11]= &(main->latt);
	lb[12]= &(main->sector);
	lb[13]= &(main->life);
	lb[14]= &(main->lamp);
	lb[15]= &(main->camera);

	lb[16]= &(main->world);
	lb[17]= &(main->screen);
	lb[18]= &(main->object);
	lb[19]= &(main->scene);
	lb[20]= &(main->library);

	lb[21]= &(main->text);
	return 22;
}

/* *********** ALLOC EN FREE *****************
  
free_libblock(ListBase *lb, ID *id )
	lijstbasis en datablok geven, alleen ID wordt uitgelezen

void free_liblist(ListBase *lb)

void *alloc_libblock(ListBase *lb, type, name)
	hangt in lijst en geeft nieuw ID

 ***************************** */

int new_id(ListBase *lb, ID *id, char *name);

ID *alloc_libblock_notest(short type)
{
	ID *id= 0;
	
	switch( type ) {
		case ID_SCE:
			id= callocN(sizeof(Scene), "scene");
			break;
		case ID_LI:
			id= callocN(sizeof(Library), "library");
			break;
		case ID_OB:
			id= callocN(sizeof(Object), "object");
			break;
		case ID_ME:
			id= callocN(sizeof(Mesh), "mesh");
			break;
		case ID_CU:
			id= callocN(sizeof(Curve), "curve");
			break;
		case ID_MB:
			id= callocN(sizeof(MetaBall), "mball");
			break;
		case ID_MA:
			id= callocN(sizeof(Material), "mat");
			break;
		case ID_TE:
			id= callocN(sizeof(Tex), "tex");
			break;
		case ID_IM:
			id= callocN(sizeof(Image), "image");
			break;
		case ID_IK:
			id= callocN(sizeof(Ika), "ika");
			break;
		case ID_WV:
			id= callocN(sizeof(Wave), "wave");
			break;
		case ID_LT:
			id= callocN(sizeof(Lattice), "latt");
			break;
		case ID_SE:
			id= callocN(sizeof(Sector), "sector");
			break;
		case ID_LF:
			id= callocN(sizeof(Life), "life");
			break;
		case ID_LA:
			id= callocN(sizeof(Lamp), "lamp");
			break;
		case ID_CA:
			id= callocN(sizeof(Camera), "camera");
			break;
		case ID_IP:
			id= callocN(sizeof(Ipo), "ipo");
			break;
		case ID_KE:
			id= callocN(sizeof(Key), "key");
			break;
		case ID_WO:
			id= callocN(sizeof(World), "world");
			break;
		case ID_SCR:
			id= callocN(sizeof(bScreen), "screen");
			break;
		case ID_VF:
			id= callocN(sizeof(VFont), "vfont");
			break;
		case ID_TXT:
			id= callocN(sizeof(Text), "text");
			break;
	}
	return id;
}

void *alloc_libblock(ListBase *lb, short type, char *name)
{
	ID *id= 0, *idtest;
	
	id= alloc_libblock_notest(type);
	if(id) {
		addtail(lb, id);
		id->us= 1;
		*( (short *)id->name )= type;
		new_id(lb, id, name);

		/* alfabetisch opnieuw invoegen: zit in new_id */
		
	}
	return id;
}

void *copy_libblock(void *rt)
{
	MemHead *memh;
	ID *idn, *id;
	ListBase *lb;
	char *cp, *cpn;
	
	id= rt;

	lb= wich_libbase(G.main, GS(id->name));
	idn= alloc_libblock(lb, GS(id->name), id->name+2);
	
	memh= (MemHead *)idn;
	memh--;
	if(memh->len - sizeof(ID) > 0) {
		cp= (char *)id;
		cpn= (char *)idn;
		memcpy(cpn+sizeof(ID), cp+sizeof(ID), memh->len - sizeof(ID));
	}
	
	id->new= idn;
	idn->flag |= LIB_NEW;
	
	return idn;
}

void free_library(Library *lib)

{

}

void free_libblock(ListBase *lb, void *idv)
{
	ID *id= idv;
	
	switch( GS(id->name) ) {	/* GetShort uit util.h */
		case ID_SCE:
			free_scene((Scene *)id);
			break;
		case ID_LI:
			free_library((Library *)id);
			break;
		case ID_OB:
			free_object((Object *)id);
			break;
		case ID_ME:
			free_mesh((Mesh *)id);
			break;
		case ID_CU:
			free_curve((Curve *)id);
			break;
		case ID_MB:
			free_mball((MetaBall *)id);
			break;
		case ID_MA:
			free_material((Material *)id);
			break;
		case ID_TE:
			free_texture((Tex *)id);
			break;
		case ID_IM:
			free_image((Image *)id);
			break;
		case ID_IK:
			free_ika((Ika *)id);
			break;
		case ID_WV:
			/* free_wave(id); */
			break;
		case ID_LT:
			free_lattice((Lattice *)id);
			break;
		case ID_SE:
			free_sector((Sector *)id);
			break;
		case ID_LF:
			free_life((Life *)id);
			break;
		case ID_LA:
			free_lamp((Lamp *)id);
			break;
		case ID_CA:
			free_camera(id);
			break;
		case ID_IP:
			free_ipo((Ipo *)id);
			break;
		case ID_KE:
			free_key((Key *)id);
			break;
		case ID_WO:
			free_world((World *)id);
			break;
		case ID_SCR:
			free_screen((bScreen *)id);
			break;
		case ID_VF:
			free_vfont((VFont *)id);
			break;
		case ID_TXT:
			free_text((Text *)id);
			break;
	}

	remlink(lb, id);
	freeN(id);

	allspace(OOPS_TEST, 0);
}

void free_libblock_us(ListBase *lb, void *idv)		/* test users */
{
	ID *id= idv;
	
	id->us--;

	if(id->us<0) {
		if(id->lib) printf("ERROR block %s %s users %d\n", id->lib->name, id->name, id->us);
		else printf("ERROR block %s users %d\n", id->name, id->us);
	}
	if(id->us==0) {
		if( GS(id->name)==ID_OB ) free_object_fromscene((Object *)id);
		free_libblock(lb, id);
	}
}


void free_liblist(ListBase *lb)
{
	ID *id, *nid;
	
	id= lb->first;
	while(id) {
		nid= id->next;

		free_libblock(lb, id);
		
		id= nid;
	}
}

void free_main(ListBase *lb, Main *main)
{
	/* ook aanroepen bij file inlezen, erase all, etc */
	ListBase *lbarray[30];
	int a;

	a= set_listbasepointers(main, lbarray);
	while(a--) {
		free_liblist(lbarray[a]);
	}

	if(lb) {
		remlink(lb, main);
		freeN(main);
		if(G.main==main) G.main= 0;
	}

	/* extra: copybuf vrijgeven */
	free_matcopybuf();

	/* hangende vars voorkomen */	
	R.backbuf= 0;
	G.scene= 0;
}

void free_mainlist()
{
	struct Main *main, *nmain;
	
	main= G.mainbase.first;
	while(main) {
		nmain= main->next;
		
		free_main(&G.mainbase, main);
		
		main= nmain;
	}

}

void add_main_to_main(Main *main, Main *from)
{
	ListBase *lbarray[30], *fromarray[30];
	int a;
	
	a= set_listbasepointers(main, lbarray);
	a= set_listbasepointers(from, fromarray);
	while(a--) {
		addlisttolist(lbarray[a], fromarray[a]);
	}
}

void join_main()
{
	Main *main, *nmain;
	
	main= G.main->next;
	while(main) {
		nmain= main->next;
		add_main_to_main(G.main, main);
		remlink(&G.mainbase, main);
		freeN(main);
		main= nmain;
	}
	
}

void split_libdata(ListBase *lb, Main *first)
{
	ListBase *lbn;
	ID *id, *idnext;
	Main *main;
	
	id= lb->first;
	while(id) {
		idnext= id->next;
		if(id->lib) {
			main= first;
			while(main) {
				if(main->curlib==id->lib) {
					lbn= wich_libbase(main, GS(id->name));
					remlink(lb, id);
					addtail(lbn, id);
					break;
				}
				main= main->next;
			}
			if(main==0) printf("error split_libdata\n");
		}
		id= idnext;
	}
}

void split_main()
{
	ListBase *lbarray[30];
	Main *main, *next;
	Library *lib;
	int a;
	
	if(G.mainbase.first!=G.mainbase.last) {
		printf("error split main\n");
		return;
	}
	
	lib= G.main->library.first;
	if(lib) {
	
		while(lib) {
			main= callocN(sizeof(Main), "newmain");
			addtail(&G.mainbase, main);
			main->curlib= lib;
			lib= lib->id.next;
		}
		
		next= G.main->next;
		
		a= set_listbasepointers(G.main, lbarray);
		while(a--) {
			split_libdata(lbarray[a], next);
		}
	}
}

Main *find_main(char *dir)
{
	Main *main;
	Library *lib;
	
	main= G.mainbase.first;
	while(main) {
		if(main->curlib) {
			if(strcmp(dir, main->curlib->name)==0) return main;
		}
		else if(strcmp(dir, main->name)==0) return main;
		
		main= main->next;
	}

	main= callocN(sizeof(Main), "findmain");
	addtail(&G.mainbase, main);
	
	lib= alloc_libblock(&(main->library), ID_LI, "lib");
	strcpy(lib->name, dir);
	main->curlib= lib;
	return main;
}

int count_mainblocks_flag(Main *main, int flag)
{
	ListBase *lbarray[30];
	ID *id;
	int a, tot= 0;

	a= set_listbasepointers(main, lbarray);
	while(a--) {
		id= lbarray[a]->first;
		while(id) {
			if(id->flag & flag) tot++;
			id= id->next;
		}
	}
	return tot;
}

/* ***************** ID ************************ */

ID *find_id(char *type, char *name)		/* type: "OB" of "MA" etc */
{
	ID *id;
	ListBase *lb;
	
	lb= wich_libbase(G.main, GS(type));
	
	id= lb->first;
	while(id) {
		if( strcmp(id->name+2, name)==0 ) return id;
		id= id->next;
	}
	return 0;
}

void IDnames_to_pupstring_nr(char **str, ListBase *lb, ID *link, short *nr, short maxpup)
{
	/* string naderhand vrijgeven */
	ID *id;
	int fake, len=50, count;
	char extra[8];
	
	*nr= -1;
	
	/* sym[0]= 171; sym[1]= 0; */
	extra[3]= 0;
	
	/* lengte berekenen, doe er 30 bij voor evt extra item */
	id= lb->first;
	count= 0;
	while(id) {
		len+= 5+strlen(id->name);
		count++;
		if(count>maxpup) break;
		id= id->next;
	}
	
	if(count>maxpup) {
		*str= mallocN(40, "IDnames_pup");
		strcpy(*str, "DataBrowse %x-2");
		return;
	}
	
	*str= mallocN(len, "IDnames_pup");
	(*str)[0]= 0;
	
	if(link==0) strcpy(*str, "  %t|");
	
	id= lb->first;
	count= 0;
	while(id) {
		if(id==link) *nr= count;
		
		fake= id->flag & LIB_FAKEUSER;
		
		if(id->lib && id->us==0) strcat(*str, "L0 ");
		else if(id->lib && fake) strcat(*str, "LF ");
		else if(id->lib) strcat(*str, "L   ");
		else if(fake) strcat(*str, "  F ");
		else if(id->us==0) strcat(*str, "  0 ");
		else if(id->us<0) strcat(*str, "-1W ");
		else strcat(*str, "     ");
		
		strcat(*str, id->name+2);
		
		if(id->next) strcat(*str, "|");
		
		count++;
		if(count>45) break;
		id= id->next;
	}

	/* verwijderen laatste OR teken */
	len= strlen(*str);
	if(len && (*str)[len-1]=='|') (*str)[len-1]= 0;
}

void IDnames_to_pupstring(char **str, ListBase *lb, ID *link, short *nr)
{
	IDnames_to_pupstring_nr(str, lb, link, nr, MAX_IDPUP);
}

void IPOnames_to_pupstring(char **str, ListBase *lb, ID *link, short *nr, int blocktype)
{
	/* string naderhand vrijgeven */
	ID *id;
	Ipo *ipo;
	int fake, len=30, count;
	char extra[8];
		
	*nr= -1;
	extra[5]= 0;
	
	/* lengte berekenen, doe er 30 bij voor evt extra item */
	id= lb->first;
	count= 0;
	while(id) {
		ipo= (Ipo *)id;
		if(blocktype==ipo->blocktype) {
			len+= 5+strlen(id->name);
			count++;
			if(count>MAX_IDPUP) break;
		}
		id= id->next;
	}

	if(count>MAX_IDPUP) {
		*str= mallocN(40, "IPOnames_pup");
		strcpy(*str, "DataBrowse %x-2");
		return;
	}

	*str= mallocN(len, "IPOnames_pup");
	(*str)[0]= 0;
	
	if(link==0) strcpy(*str, "  %t|");
	
	id= lb->first;
	count= 0;
	while(id) {
		ipo= (Ipo *)id;
		if(blocktype==ipo->blocktype) {
			if(id==link) *nr= count;
			
			fake= id->flag & LIB_FAKEUSER;
			
			if(id->lib && id->us==0) strcat(*str, "L0 ");
			else if(id->lib && fake) strcat(*str, "L F ");
			else if(id->lib) strcat(*str, "L   ");
			else if(fake) strcat(*str, "  F ");
			else if(id->us==0) strcat(*str, "  0 ");
			else if(id->us<0) strcat(*str, "-1W ");
			else strcat(*str, "     ");
			
			strcat(*str, id->name+2);
			
			if(id->next) strcat(*str, "|");
			
			count++;
		}
		id= id->next;
	}
	
	/* verwijderen laatste OR teken */
	len= strlen(*str);
	if(len && (*str)[len-1]=='|') (*str)[len-1]= 0;
}

int has_id_number(ID *id)
{
	int a, len;
	char *name;
	
	name= id->name+2;
	len= strlen(name);
	if(len<2) return 0;
	if(name[len-1]=='.') return 0;
	
	while(--len) {
		if( name[len]=='.') return 1;
		if( isdigit( name[len] )==0 ) return 0;
	}
	return 0;
}

void splitIDname(char *name, char *left, int *nr)
{
	int len, a;
	
	*nr= 0;
	strncpy(left, name, 21);
	
	a=len= strlen(name);
	if(a>1 && name[a-1]=='.') return;
	
	while(a--) {
		if( name[a]=='.' ) {
			left[a]= 0;
			*nr= atol(name+a+1);
			return;
		}
		if( isdigit(name[a])==0 ) break;
		
		left[a]= 0;
	}
	strcpy(left, name);	
}

void sort_alpha_id(ListBase *lb,  ID *id)
{
	ID *idtest;
	
	/* alfabetisch opnieuw invoegen */
	if(lb->first!=lb->last) {
		remlink(lb, id);
		
		idtest= lb->first;
		while(idtest) {

			if(strcasecmp(idtest->name, id->name)>0 || idtest->lib) {
				insertlinkbefore(lb, idtest, id);
				break;
			}
			idtest= idtest->next;
		}
		/* als laatste */
		if(idtest==0) {
			addtail(lb, id);
		}
	}
	
}

int new_id(ListBase *lb, ID *id, char *tname)
/* alleen locale blokken: externe en indirekte hebben al een unieke ID */
/* return 1: nieuwe naam gemaakt */
{
	ID *idtest;
	int nr= 0, nrtest, maxtest=32, a;
	char aname[32], *name, left[24], leftest[24], in_use[32];
	
	/* - naam splitsen
	 * - zoeken
	 */

	if(id->lib) return 0;

	if(tname==0) name= id->name+2;
	else {
		/* tname can be const */
		strncpy(aname, tname, 21);
		name= aname;
		
		if( strlen(name) > 21 ) name[21]= 0;
	}

	if(lb==0) lb= wich_libbase(G.main, GS(id->name));

	/* eerste fase: bestaat de id al? */
	idtest= lb->first;
	while(idtest) {
	
		if(id!=idtest && idtest->lib==0) {
			
			/* niet alphabetic testen! */
			/* optim */
			if( idtest->name[2] == name[0] ) {
				if(strcmp(name, idtest->name+2)==0) break;
			}
		}
		
		idtest= idtest->next;
	}	

	/* if there is no double return */
	if(idtest==0) {
		strcpy(id->name+2, name);
		return 0;
	}
	
	bzero(in_use, maxtest);

	splitIDname(name, left, &nr);
	if(nr>999 && strlen(left)>16) left[16]= 0;
	else if(strlen(left)>17) left[17]= 0;


	idtest= lb->first;
	while(idtest) {
	
		if(id!=idtest && idtest->lib==0) {
			
			splitIDname(idtest->name+2, leftest, &nrtest);
			if(strcmp(left, leftest)==0) {
				
				if(nrtest<maxtest) in_use[nrtest]= 1;
				if(nr <= nrtest) nr= nrtest+1;
			}
		}
		
		idtest= idtest->next;
	}
	
	for(a=0; a<maxtest; a++) {
		if(a>=nr) break;
		if( in_use[a]==0 ) {
			nr= a;
			break;
		}
	}
	
	if(nr==0) sprintf(id->name+2, "%s", left);
	else {
		sprintf(id->name+2, "%s.%0.3d", left, nr);
	}
	
	sort_alpha_id(lb, id);	

	return 1;
}

void clear_id_newpoins()
{
	ListBase *lbarray[30];
	ID *id;
	int a;

	a= set_listbasepointers(G.main, lbarray);
	while(a--) {
		id= lbarray[a]->first;
		while(id) {
			id->new= 0;
			id->flag &= ~LIB_NEW;
			id= id->next;
		}
	}
}

void all_local()
{
	ListBase *lbarray[30], tempbase={0, 0};
	ID *id, *idn, *idp;
	int a;

	a= set_listbasepointers(G.main, lbarray);
	while(a--) {
		id= lbarray[a]->first;
		
		while(id) {
			id->new= 0;
			id->flag &= ~(LIB_EXTERN|LIB_INDIRECT|LIB_NEW);
			
			idn= id->next;		/* id wordt mogelijk opnieuw ingevoegd */
			if(id->lib) {
				id->lib= 0;
				new_id(lbarray[a], id, 0);	/* new_id doet dit alleen bij dubbele namen */
				sort_alpha_id(lbarray[a], id);
			}
			else {
				/* patch: testen of de zaak wel alphabetisch is */
/*
				if(idn) {
					if(strcasecmp(id->name, idn->name)>0) {
						remlink(lbarray[a], id);
						addtail(&tempbase, id);
					}
					else if(id->prev) {
						idp= id->prev;
						if(strcasecmp(idp->name, id->name)>0) {
							remlink(lbarray[a], id);
							addtail(&tempbase, id);
						}
					}
				}
*/				
			}
			
			id= idn;
		}
		
		/* patch2: zorgen dat de zaak wel alphabetisch is */
		while(id=tempbase.first) {
			remlink(&tempbase, id);
			addtail(lbarray[a], id);
			new_id(lbarray[a], id, 0);
		}
	}
}


void test_idbutton(char *name)
{
	/* vanuit buttons: als naam al bestaat: new_id aanroepen */
	ListBase *lb;
	ID *id, *idtest;
	

	lb= wich_libbase(G.main, GS(name-2) );
	if(lb==0) return;
	
	/* zoek welke id */
	idtest= lb->first;
	while(idtest) {
		if( strcmp(idtest->name+2, name)==0) break;
		idtest= idtest->next;
	}

	if(idtest) if( new_id(lb, idtest, name)==0 ) sort_alpha_id(lb, idtest);
}

void rename_id(ID *id, char *name)
{
	ListBase *lb;
	ID *idtest;
	
	strncpy(id->name+2, name, 21);
	lb= wich_libbase(G.main, GS(id->name) );
	
	new_id(lb, id, name);				

}


