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
 * 
 * 
 *  readblender.c
 * 
 *  jan 96
 * 
 * 
 */


#include "/usr/people/blend/blender.h"
#include "/usr/people/blend/file.h"



/* namaak globals om de boel te neppen */

Global G;



/* ************************************ */
/* ************************************ */
/* ************************************ */
 * 
 * 
 *  LET OP: de routine newlibadr() is - in blender - gewijzigd.
 *  Zou hier ook zo moeten!!!
 * 
/* ************************************ */
/* ************************************ */
/* ************************************ */


char *blender_filedata=0;


/* kopieen uit readfile.c */




/* ************** AFHANDELING OUDE POINTERS ******************* */

typedef struct OldNew {
	void *old, *new;
	int nr;
} OldNew;

extern char *compflags;		/* genfile.c */
void read_libraries();

OldNew *datablocks=0;
OldNew *libblocks=0;
int datacount= 0, maxdatacount=1024;
int libcount= 0, maxlibcount=1024;
void *disable_newlibadr=0;


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


void add_data_adr(void *old, void *new)
/* met dynamische malloc
 * (0, 1) doorgeven herinitialiseert en geeft ongebruikte blokken vrij
 * (0, 0) doorgeven geeft alles vrij 
 */
{
	OldNew *temp;
	int a;
	
	if(old==0) {	/* ongebruikte vrijgeven */
		temp= datablocks;
		for(a=0; a<datacount; a++, temp++) {
			if(temp->nr==0) freeN(temp->new);
		}
		if(new==0 && datablocks) {
			freeN(datablocks);
			datablocks= 0;
			maxdatacount= 1024;
		}
		datacount= 0;
	}
	else {
		if(datablocks==0) {
			datablocks= mallocN(maxdatacount*sizeof(OldNew), "OldNew");
		}
		else if(datacount==maxdatacount) {
			maxdatacount*= 2;
			temp= mallocN(maxdatacount*sizeof(OldNew), "OldNew");
			memcpy(temp, datablocks, maxdatacount*sizeof(OldNew)/2);
			freeN(datablocks);
			datablocks= temp;
		}
		
		temp= datablocks+datacount;
		temp->old= old;
		temp->new= new;
		temp->nr= 0;
		
		datacount++;
	}
}

void add_lib_adr(void *old, void *new)
/* met dynamische malloc
 * (0, 0) doorgeven geeft alles vrij
 * Zet aantal users al op 1!!!
 */
{
	OldNew *temp=0;
	int a;
	
	if(disable_newlibadr) {
		disable_newlibadr= new;
		return;
	}
	
	if(old==0) {	/* alles vrijgeven */
		if(libblocks) freeN(libblocks);
		libblocks= 0;
		maxlibcount= 1024;
		libcount= 0;
	}
	else {
		if(libblocks==0) {
			libblocks= mallocN(maxlibcount*sizeof(OldNew), "OldNew");
		}
		else if(libcount==maxlibcount) {
			maxlibcount*= 2;
			temp= mallocN(maxlibcount*sizeof(OldNew), "OldNew");
			memcpy(temp, libblocks, maxlibcount*sizeof(OldNew)/2);
			freeN(libblocks);
			libblocks= temp;
		}
		
		temp= libblocks+libcount;
		temp->old= old;
		temp->new= new;
		temp->nr= 1;
		
		libcount++;
	}
}

void *newadr(void *adr)		/* alleen direkte datablokken */
{
	static int lastone= 0;
	struct OldNew *onew;

	if(adr) {
		/* op goed geluk: eerst het volgende blok doen */
		if(lastone<datacount-1) {
			lastone++;
			onew= datablocks+lastone;
			if(onew->old==adr) {
				onew->nr++;
				return onew->new;
			}
		}
		
		lastone= 0;
		onew= datablocks;
		while(lastone<datacount) {
			if(onew->old==adr) {
				onew->nr++;
				return onew->new;
			}
			onew++;
			lastone++;
		}
	}
	
	return 0;
}

void *newlibadrP(void *adr)		/* alleen Lib datablokken */
{
	static int lastone= 0;
	struct OldNew *onew;

	if(adr) {
		/* op goed geluk: eerst het volgende blok doen */
		if(lastone<libcount-1) {
			lastone++;
			onew= libblocks+lastone;
			if(onew->old==adr) {
				onew->nr++;
				return onew->new;
			}
		}
		
		lastone= 0;
		onew= libblocks;
		while(lastone<libcount) {
			if(onew->old==adr) {
				onew->nr++;
				return onew->new;
			}
			onew++;
			lastone++;
		}
	}
	
	return 0;
}

void *newlibadr_usP(void *adr)		/* hoogt usernummer op */
{
	ID *id;
	
	id= newlibadrP(adr);
	if(id) {
		id->us++;
	}
	
	return id;
}

void change_libadr(void *old, void *new)
{
	struct OldNew *onew;
	int lastone= 0;
	
	onew= libblocks;
	while(lastone<libcount) {
		
		if(onew->new==old) {
			onew->new= new;
			return;
			/* return; kunnen er meer zijn? */
		}
		onew++;
		lastone++;
	}
	
}



/* ********** END OUDE POINTERS ****************** */
/* ********** READ FILE ****************** */

void read_struct(BHead *bh)
{
	void *temp= 0;

	if(bh->len) {
		if(compflags[bh->SDNAnr]) {		/* flag==0: bestaat niet meer */
			
			temp= mallocN(bh->len, "read_struct");
			memcpy(temp, (bh+1), bh->len);
			
			if(compflags[bh->SDNAnr]==2) reconstruct(bh->SDNAnr, bh->nr, &temp);
		}
	}
	
	add_data_adr(bh->old, temp);
}

void *read_libstruct(BHead *bh)
{
	void *temp= 0;

	if(bh->len) {
		if(compflags[bh->SDNAnr]) {		/* flag==0: bestaat niet meer */

			temp= mallocN(bh->len, "read_libstruct");
			memcpy(temp, (bh+1), bh->len);

			if(compflags[bh->SDNAnr]==2) reconstruct(bh->SDNAnr, bh->nr, &temp);
		}
	}
	
	add_lib_adr(bh->old, temp);

	return temp;
}

/* ************** ALG & MAIN ******************** */

void direct_link_mesh(mesh)
Mesh *mesh;
{
	mesh->mat= newadr(mesh->mat);
	mesh->mface= newadr(mesh->mface);
	mesh->mvert= newadr(mesh->mvert);
	mesh->mcol= newadr(mesh->mcol);
	mesh->msticky= newadr(mesh->msticky);
	
	mesh->disp.first= mesh->disp.last= 0;
	mesh->bb= 0;
}


int read_libblock(Main *main, BHead *bhead, int flag)
{
	/* deze routine leest libblock en direkte data. Met linkfunkties
	 * alles aan elkaar hangen.
	 */
	
	ID *id;
	ListBase *lb;
	int skipdata;
	char *fd;

	if(bhead->code==ID_ID) {
		id= (ID *)(bhead + 1);
		lb= wich_libbase(main, GS(id->name));
		/* printf("idid %s oldp %x\n", id->name, bhead->old); */
	}
	else {
		lb= wich_libbase(main, bhead->code);
	}

	if(lb==0) {
		return bhead->len+sizeof(BHead);
	}
	
	fd= (char *)bhead;
	
	/* libblock inlezen */
	id= read_libstruct(bhead);
	addtail(lb, id);
	id->flag= flag | LIB_NEEDLINK;
	id->lib= main->curlib;
	id->us= 0;
	
	/* deze mag niet door de direct_link molen: is alleen het ID deel */
	if(bhead->code==ID_ID) {
		skipdata= bhead->len+sizeof(BHead);
		return skipdata;	
	}

	skipdata= bhead->len+sizeof(BHead);
	fd+= bhead->len+sizeof(BHead);
	bhead= (BHead *)fd;

	
	/* alle data inlezen */
	while(bhead->code==DATA) {
		
		read_struct(bhead);
		
		skipdata+= bhead->len+sizeof(BHead);
		fd+= bhead->len+sizeof(BHead);
		bhead= (BHead *)fd;		
	}

	/* pointers directe data goedzetten */
	switch( GS(id->name) ) {
		case ID_OB:
			/* direct_link_object(id); */
			break;
		case ID_ME:
			direct_link_mesh(id);
			break;
		case ID_CU:
			/* direct_link_curve(id); */
			break;
		case ID_MB:
			/* direct_link_mball(id); */
			break;
		case ID_MA:
			/* direct_link_material(id); */
			break;
		case ID_TE:
			/* direct_link_texture(id); */
			break;
	}
	
	/* vrijgeven, herinitialiseren */
	add_data_adr(0, (void *)1);	
	
	return skipdata;
}












void read_file_dna(char *filedata)
{
	BHead *bhead;
	int afbreek=0;
	char *fd;
	
	freestructDNA(&old_sdna);

	fd= filedata;
	while(afbreek==0) {
		
		bhead= (BHead *)fd;
		
		if(bhead->code==ENDB) afbreek= 1;
		else if(bhead->code==DNA1) {

			old_sdna.data= mallocN(bhead->len, "sdna");
			memcpy(old_sdna.data, fd+sizeof(BHead), bhead->len);
			afbreek= 1;
		}
		
		fd+= bhead->len+sizeof(BHead);
	}
	
	init_structDNA(&old_sdna);
	set_compareflags_structDNA();
	
}

ID *is_yet_read(Main *main, BHead *bhead)
{
	ListBase *lb;
	ID *idtest, *id;
	
	idtest= (ID *)(bhead +1);
	lb= wich_libbase(main, GS(idtest->name));
	if(lb) {
		id= lb->first;
		while(id) {
			if( strcmp(id->name, idtest->name)==0 ) return id;
			id= id->next;
		}
	}
	return 0;
}


void open_blender(char *name)
{
	static int firsttime=1;
	extern char DNAstr[];
	int len, file, version, temp, temp1, filelen;	
	char str[12];
	
	if(firsttime) {
		bzero(G, sizeof(Global));
		
		G.main= callocN(sizeof(Main), "main");
		
		bzero(&cur_sdna, sizeof(struct SDNA));
		cur_sdna.data= DNAstr+4;
		cur_sdna.datalen= *( (int *)DNAstr);
		init_structDNA(&cur_sdna);

		firsttime= 0;
	}
	
	len= strlen(name);
	if(len<6) return;
	if( name[len-1]=='/' ) return;

	
	file= open(name, O_BINARY|O_RDONLY);
	if(file<=0) {
		printf("Can't find file\n");
		return;
	}
	
	filelen= filesize(file);	

	read(file, &temp, 4);
	read(file, &temp1, 4);
	if(temp!=BLEN || temp1!=DER_) {
		close(file);
		printf("Idiot file!\n");
		return;
	}
	read(file, &version, 4);
	str[0]= ( (char *)&version )[1];
	str[1]= ( (char *)&version )[2];
	str[2]= ( (char *)&version )[3];
	str[3]= 0;
	G.versionfile= atoi(str);
	
	/* hele file inlezen */
	filelen-= 12;
	blender_filedata= mallocN(filelen, "filedata");
	read(file, blender_filedata, filelen);
	close(file);
	
	/* op zoek naar SDNA */
	read_file_dna(blender_filedata);
}

void close_blender()
{
	if(blender_filedata) {
		freeN(blender_filedata);
		blender_filedata= 0;
	}
}

void *append_part(int idcode, char *name)
{
	BHead *bhead;
	ID *id=0;
	int afbreek=0;
	char *fd;
	
	fd= blender_filedata;
	
	if(fd==0) {
		printf("didn't open blenderfile\n");
		return;
	}
	
	while(afbreek==0) {
		
		bhead= (BHead *)fd;
		
		if(bhead->code==ENDB) afbreek= 1;
		else if(bhead->code==idcode) {
			id= (ID *)(bhead+1);
			if(strcmp(id->name+2, name)==0) {
				
				id= is_yet_read(G.main, bhead);
				if(id==0) {
					read_libblock(G.main, bhead, LIB_TESTEXT);
					id= newlibadrP(bhead->old);
				}
				afbreek= 1;
			}
			else id= 0;
		}
		
		fd+= bhead->len+sizeof(BHead);
	}
	
	/* losslingerende blokken vrijgeven */
	add_data_adr(0, 0);
	add_lib_adr(0, 0);

	return id;
}


void test_read()
{
	Mesh *me;
	
	open_blender("/data/rt.blend");

	me= append_part(ID_ME, "mesh");

	if(me) {
		PRINT3(d, d, x, me->totvert, me->totface, me->mface);
	}
	
	me= append_part(ID_ME, "Mesh");
	if(me) {
		PRINT3(d, d, x, me->totvert, me->totface, me->mface);
	}

	me= append_part(ID_ME, "Dinges");
	if(me) {
		PRINT3(d, d, x, me->totvert, me->totface, me->mface);
	}
	
}

