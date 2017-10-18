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


/*  blender.c   jan 94     MIXED MODEL
 * 
 * algemene hulp funkties en data
 * 
 */

#include "blender.h"

struct Global G;

float matone[4][4]={
	{1,0,0,0},
	{0,1,0,0},
	{0,0,1,0},
	{0,0,0,1}
};

char versionstr[48]= { 14, 92, 240, 247, 43, 199, 126, 71, 69, 113, 64, 241, 115, 139, 242, 216};


char versionfstr[24];	/* voor files */


/* ********** vrijgeven ********** */

void free_blender()
{
	Main *main;
	
	freestructDNA_all();	/* genfile.c */
	
	free_mainlist();		/* library.c */

	freeImBufdata();		/* imbuf lib */
}

void addlisttolist(ListBase *list1, ListBase *list2)
{

	if(list2->first==0) return;

	if(list1->first==0) {
		list1->first= list2->first;
		list1->last= list2->last;
	}
	else {
		((struct Link *)list1->last)->next= list2->first;
		((struct Link *)list2->first)->prev= list1->last;
		list1->last= list2->last;
	}
	list2->first= list2->last= 0;
}

void *dupallocN(void *mem)
{
	MemHead *memh;
	void *new;
	
	if(mem==0) return 0;
	
	memh= (MemHead *)mem;
	memh--;
	
	if(memh->len==0) {
		printf("error: dupallocN with len==0 %s\n", memh->name);
		return 0;
	}
	else {
		new= mallocN(memh->len, "dupli_alloc");
		memcpy(new, mem, memh->len);
		return new;
	}
}

int alloc_len(void *mem)
{
	MemHead *memh;
	
	if(mem==0) return 0;
	
	memh= (MemHead *)mem;
	memh--;

	return memh->len;
}

void duplicatelist(ListBase *list1, ListBase *list2)  /* kopie van 2 naar 1 */
{
	struct Link *link1, *link2;
	
	list1->first= list1->last= 0;
	
	link2= list2->first;
	while(link2) {

		link1= dupallocN(link2);
		addtail(list1, link1);
		
		link2= link2->next;
	}	
}


void initglobals()
{

	bzero(&G, sizeof(Global));
	
	G.animspeed= 4;
	
	G.main= callocN(sizeof(Main), "initglobals");
	addtail(&G.mainbase, G.main);
	strcpy(G.ima, "//");
	strcpy(G.psx, "//");
	
	G.version= 172;

	G.order= 1;
	
	sprintf(versionfstr, "BLENDER  %d", G.version);
	
	if( ((char *)&G.order)[1] ) {
		G.order= B_ENDIAN;
		versionfstr[8]= 'V';
	}
	else {
		G.order= L_ENDIAN;
		versionfstr[8]= 'v';
	}

	if(sizeof(void *)==8) versionfstr[7]= '-';
	else versionfstr[7]= '_';

	sprintf(versionstr+16, "%d", G.version);

	#ifdef __sgi	
		initmoviepointers();	/* define in iff.h */
	#endif

	clear_workob();	/* object.c */
	
}


