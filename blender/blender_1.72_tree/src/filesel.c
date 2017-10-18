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

/*  filesel.c       GRAPHICS

 * 
 *  jan 95
 * 
 */

#include "blender.h"
#include "render.h"
#include "graphics.h"
#include "file.h"
#include "imasel.h"

#if defined WIN32 || defined MIPS1 || defined __BeOS
	int fnmatch(){return 0;}
	#ifdef MIPS1
		void usleep(int val) {sginap(val);}
	#endif
#else
	#include <fnmatch.h>
#endif

#ifndef WIN32
#include <sys/param.h>
#endif

#define FILESELHEAD		60
#define FILESEL_DY		16

#define NOTACTIVE			0
#define ACTIVATE			1
#define INACTIVATE			2

#define STARTSWITH(x, y) (strncmp(x, y, sizeof(x) - 1) == 0)

void library_to_filelist(SpaceFile *sfile);
void main_to_filelist(SpaceFile *sfile);
char *code_to_groupname(int code);
void filesel_select_objects(SpaceFile *sfile);
void active_file_object(SpaceFile *sfile);

/* globals */

rcti scrollrct, textrct, bar;
int filebuty1, filebuty2, page_ofs, collumwidth, selecting=0;
int filetoname= 0;
float pixels_to_ofs;
char otherdir[FILE_MAXDIR], fslog[200]="", *fsmenu=0;
ScrArea *otherarea;

/* ******************* SORT ******************* */

extern int compare();		/* storage.c */

int compare_date(struct direntry *entry1, struct direntry *entry2)	
{
	/* type is gelijk aan stat.st_mode */

	if (S_ISDIR(entry1->type)){
		if (S_ISDIR(entry2->type)==0) return (-1);
	} else{
		if (S_ISDIR(entry2->type)) return (1);
	}
	if (S_ISREG(entry1->type)){
		if (S_ISREG(entry2->type)==0) return (-1);
	} else{
		if (S_ISREG(entry2->type)) return (1);
	}
	if ((entry1->type & S_IFMT) < (entry2->type & S_IFMT)) return (-1);
	if ((entry1->type & S_IFMT) > (entry2->type & S_IFMT)) return (1);

/*
	if ( entry1->s.st_ctime < entry2->s.st_ctime) return 1;
	if ( entry1->s.st_ctime > entry2->s.st_ctime) return -1;
*/
	if ( entry1->s.st_mtime < entry2->s.st_mtime) return 1;
	if ( entry1->s.st_mtime > entry2->s.st_mtime) return -1;
	else return strcasecmp(entry1->relname,entry2->relname);
}

int compare_size(struct direntry *entry1, struct direntry *entry2)	
{
	/* type is gelijk aan stat.st_mode */

	if (S_ISDIR(entry1->type)){
		if (S_ISDIR(entry2->type)==0) return (-1);
	} else{
		if (S_ISDIR(entry2->type)) return (1);
	}
	if (S_ISREG(entry1->type)){
		if (S_ISREG(entry2->type)==0) return (-1);
	} else{
		if (S_ISREG(entry2->type)) return (1);
	}
	if ((entry1->type & S_IFMT) < (entry2->type & S_IFMT)) return (-1);
	if ((entry1->type & S_IFMT) > (entry2->type & S_IFMT)) return (1);

	if ( entry1->s.st_size < entry2->s.st_size) return 1;
	if ( entry1->s.st_size > entry2->s.st_size) return -1;
	else return strcasecmp(entry1->relname,entry2->relname);
}


/* **************************************** */

void clear_global_filesel_vars()
{
	selecting= 0;
}

void addfilename_to_fsmenu(char *name)
{
	char *temp;
	int len;

	len= strlen(name);
	if(len==0) return;

	if(fsmenu) len+= strlen(fsmenu);

	if(fsmenu) {
	
		/* komtie voor? */
		if(strstr(fsmenu, name)) return;
		temp= mallocN(len+2, "fsmenu");
		
		strcpy(temp, fsmenu);
		strcat(temp, "|");
		strcat(temp, name);
		freeN(fsmenu);
		fsmenu= temp;
	}
	else {
		fsmenu= mallocN(len+2, "fsmenu");
		strcpy(fsmenu, name);
	}
}

void filesel_statistics(SpaceFile *sfile, int *totfile, int *selfile, float *totlen, float *sellen)
{
	int a, len;
	
	*totfile= *selfile= 0;
	*totlen= *sellen= 0;
	
	if(sfile->filelist==0) return;
	
	for(a=0; a<sfile->totfile; a++) {
		if( (sfile->filelist[a].type & S_IFDIR)==0 ) {
			(*totfile) ++;

			len = sfile->filelist[a].s.st_size;
			(*totlen) += (len/1048576.0); 		

			if(sfile->filelist[a].flags & ACTIVE) {
				(*selfile) ++;
				(*sellen) += (len/1048576.0);
			}
		}
	}
}

#ifdef __sgi
void workspacecommand(SpaceFile *sfile)
{
	FILE * pipein, * pipeout;
	short pipestart = FALSE;
	char cmd[512];
	
	if (sfile->file[0] != 0) {
		waitcursor(1);
		
		sprintf(cmd, "%s%s", sfile->dir, sfile->file);
		
		if (ispic(cmd) == TGA) { /* patch */
			sprintf(cmd, "paint %s%s", sfile->dir, sfile->file);
			system(cmd);
		} else {
			sprintf(cmd, "wstype -v %s/%s", sfile->dir, sfile->file);
			
			pipein = popen(cmd, "r");
			pipeout = popen("sh -s ", "w");
			pipestart = FALSE;
			
			if (pipein != NULL && pipeout != 0) {
				fprintf(pipeout, "cd %s\n", sfile->dir);
				fprintf(pipeout, "LEADER=%s/%s\n", sfile->dir, sfile->file);
				fprintf(pipeout, "SELECTED=%s/%s\n", sfile->dir, sfile->file);
				fprintf(pipeout, "ARGC=1\n");

				while(fscanf(pipein, " %511[^\n]\n", cmd) != EOF) {
					/*fprintf(stderr, "%s\n", cmd);*/
					if (STARTSWITH("TYPE", cmd)) {
						fprintf(pipeout, "LEADERTYPE=%s\n", cmd + 5);
					} else if (STARTSWITH("CMD OPEN", cmd)) {
						pipestart = TRUE;
						fprintf(pipeout, "%s\n", cmd + 8);
					} else if (STARTSWITH("CMD", cmd)) {
						pipestart = FALSE;
					} else if (pipestart) {
						fprintf(pipeout, "%s\n", cmd);
					}
				}
			}
			if (pipein != NULL) pclose(pipein);
			if (pipeout != NULL) pclose(pipeout);					
		}
		waitcursor(0);
	}
}
#endif


/* *************** HULPFUNKTIES ******************* */

void char_switch (char *string, char from, char to) 
{

	while (*string != 0) {
		if (*string == from) *string = to;
		string++;
	}
}

/* This is a really ugly function... its purpose is to
 * take the space file name and clean it up, replacing
 * excess file entry stuff (like /tmp/../tmp/../)
 */

void checkdir(char *dir)
{
	short a;
	char *start, *eind;
	char tmp[FILE_MAXDIR+FILE_MAXFILE];

	make_file_string(tmp, dir, "");
	strcpy(dir, tmp);
	
#ifdef WIN32
	if(dir[0]=='.') {	/* komt voor, o.a. bij FILE_MAIN */
		dir[0]= '\\';
		dir[1]= 0;
		return;
	}	

	while (start = strstr(dir, "\\..\\")) {
		eind = start + strlen("\\..\\") - 1;
		a = start-dir-1;
		while (a>0) {
			if (dir[a] == '\\') break;
			a--;
		}
		strcpy(dir+a,eind);
	}

	while (start = strstr(dir,"\\.\\")){
		eind = start + strlen("\\.\\") - 1;
		strcpy(start,eind);
	}

	while (start = strstr(dir,"\\\\" )){
		eind = start + strlen("\\\\") - 1;
		strcpy(start,eind);
	}

	if(a = strlen(dir)){				/* eerst alle '\\' weghalen aan het eind */
		while(a>0 && dir[a-1] == '\\'){
			a--;
			dir[a] = 0;
		}
	}

	strcat(dir, "\\");
#else	
	if(dir[0]=='.') {	/* komt voor, o.a. bij FILE_MAIN */
		dir[0]= '/';
		dir[1]= 0;
		return;
	}	
	
	while (start = strstr(dir, "/../")) {
		eind = start + strlen("/../") - 1;
		a = start-dir-1;
		while (a>0) {
			if (dir[a] == '/') break;
			a--;
		}
		strcpy(dir+a,eind);
	}

	while (start = strstr(dir,"/./")){
		eind = start + strlen("/./") - 1;
		strcpy(start,eind);
	}

	while (start = strstr(dir,"//" )){
		eind = start + strlen("//") - 1;
		strcpy(start,eind);
	}

	if(a = strlen(dir)){				/* eerst alle '/' weghalen aan het eind */
		while(dir[a-1] == '/'){
			a--;
			dir[a] = 0;
			if (a<=0) break;
		}
	}

	strcat(dir, "/");
#endif
}

void test_flags_file(SpaceFile *sfile)
{
	struct direntry *file;
	int num;
	short mval[2];

	file= sfile->filelist;
	
	for(num=0; num<sfile->totfile; num++, file++) {
		file->flags= 0;
		file->type= file->s.st_mode;	/* restore het geknoei van hieronder */ 
		
		if(sfile->type==FILE_BLENDER || sfile->type==FILE_LOADLIB) {
			if(testextensie(file->relname, ".blend")) {
				file->flags |= BLENDERFILE;
				if(sfile->type==FILE_LOADLIB) {
					file->type &= ~S_IFMT;
					file->type |= S_IFDIR;
				}
			}
			else if(testextensie(file->relname, ".psx")) {
				file->flags |= PSXFILE;
			}
		} else if (sfile->type==FILE_SPECIAL){
			if(testextensie(file->relname, ".jpg") ||
					testextensie(file->relname, ".tga") ||
					testextensie(file->relname, ".rgb") ||
					testextensie(file->relname, ".sgi")) {
				file->flags |= IMAGEFILE;			
			}
			else if(testextensie(file->relname, ".avi") ||
					testextensie(file->relname, ".mv")) {
				file->flags |= MOVIEFILE;			
			}
		}
	}	
}


void sort_filelist(SpaceFile *sfile)
{
	struct direntry *file;
	int num, act= 0;
	
	switch(sfile->sort) {
	case FILE_SORTALPHA:
		qsort(sfile->filelist, sfile->totfile, sizeof(struct direntry), (void *)compare);	
		break;
	case FILE_SORTDATE:
		qsort(sfile->filelist, sfile->totfile, sizeof(struct direntry), (void *)compare_date);	
		break;
	case FILE_SORTSIZE:
		qsort(sfile->filelist, sfile->totfile, sizeof(struct direntry), (void *)compare_size);	
		break;
	case FILE_SORTEXTENS:
		qsort(sfile->filelist, sfile->totfile, sizeof(struct direntry), (void *)compare);	
		break;
	}
	
	sfile->act= -1;

	file= sfile->filelist;
	for(num=0; num<sfile->totfile; num++, file++) {
		file->flags &= ~HILITE;
	}

}

void read_dir(SpaceFile *sfile)
{
	extern struct direntry *files; /* storage.c */
	extern int actnum, totnum;	/* storage.c */
	int num, len;
	char wdir[FILE_MAXDIR];

	/* sfile->act wordt gebruikt o.a. bij databrowse: dubbele namen van library objecten */
	sfile->act= -1;

	if(sfile->type==FILE_MAIN) {
		main_to_filelist(sfile);
		return;
	}
	else if(sfile->type==FILE_LOADLIB) {
		library_to_filelist(sfile);
		if(sfile->libfiledata) return;
	}

	/* als actnum een waarde heeft (aantal files) wordt vrijgegeven */
	actnum= totnum= 0;
	files= 0;
	
	hide_dot_files(sfile->flag & FILE_HIDE_DOT);
	
	getwdN(wdir);
	sfile->totfile= getdir(sfile->dir, &(sfile->filelist));
	chdir(wdir);
	
	if(sfile->sort!=FILE_SORTALPHA) sort_filelist(sfile);
	
	sfile->maxnamelen= 0;

	for (num=0; num<sfile->totfile; num++) {
		
		len = fmgetstrwidth(G.font, sfile->filelist[num].relname);
		if (len > sfile->maxnamelen) sfile->maxnamelen = len;
		
		if(filetoname) {
			if(strcmp(sfile->file, sfile->filelist[num].relname)==0) {
				
				sfile->ofs= num-( sfile->collums*(curarea->winy-FILESELHEAD-20)/(2*FILESEL_DY));
				filetoname= 0;
			}
		}
	}
	test_flags_file(sfile);
	
	filetoname= 0;
}

void freefilelist(SpaceFile *sfile)
{
	int num;

	num= sfile->totfile-1;

	if (sfile->filelist==0) return;
	
	for(; num>=0; num--){
		free(sfile->filelist[num].relname);
		
		if (sfile->filelist[num].string) free(sfile->filelist[num].string);
	}
	free(sfile->filelist);
	sfile->filelist= 0;
}

void make_exist(char *dir)
{
	int a;
	char tmp[FILE_MAXDIR+FILE_MAXFILE];
		

	#ifdef WIN32
		char_switch(dir, '/', '\\');
	#else
		char_switch(dir, '\\', '/');
	#endif	
	
	a = strlen(dir);
	
#ifdef WIN32	
	while(fop_exists(dir) == 0){
		a --;
		while(dir[a] != '\\'){
			a--;
			if (a <= 0) break;
		}
		if (a >= 0) dir[a+1] = 0;
		else {
			strcpy(dir,"\\");
			break;
		}
	}
#else
	while(exist(dir) == 0){
		a --;
		while(dir[a] != '/'){
			a--;
			if (a <= 0) break;
		}
		if (a >= 0) dir[a+1] = 0;
		else {
			strcpy(dir,"/");
			break;
		}
	}
#endif
}

void split_dirfile(char *string, char *dir, char *file)
{
	int a;
	
	dir[0]= 0;
	file[0]= 0;

#ifdef WIN32
	if (strlen(string)) {
		if (string[0] == '/' || string[0] == '\\') { 
			strcpy(dir, string);
		} else if (string[1] == ':' && string[2] == '\\') {
			strcpy(dir, string);
		} else {
			getwdN(dir);
			strcat(dir,"/");
			strcat(dir,string);
			strcpy(string,dir);
		}
		
		make_exist(dir);
		
		if (S_ISDIR(exist(dir))) {
			strcpy(file,string + strlen(dir));

			if (strrchr(file,'\\')) strcpy(file,strrchr(file,'\\')+1);
		
			if (a = strlen(dir)) {
				if (dir[a-1] != '\\') strcat(dir,"\\");
			}
		}
		else {
			a = strlen(dir) - 1;
			while(a>0 && dir[a] != '\\') a--;
			dir[a + 1] = 0;
			strcpy(file, string + strlen(dir));
		}
		
	}
	else {
		strcpy(dir, "\\");
		file[0]=0;
	}
#else
	if (strlen(string)) {
		if (string[0] == '/') { 
			strcpy(dir, string);
		} else if (string[1] == ':' && string[2] == '\\') {
			string+=2;
			strcpy(dir, string);
		} else {
			getwdN(dir);
			strcat(dir,"/");
			strcat(dir,string);
			strcpy(string,dir);
		}

		make_exist(dir);
			
		if (S_ISDIR(exist(dir))) {
			strcpy(file,string + strlen(dir));

			if (strrchr(file,'/')) strcpy(file,strrchr(file,'/')+1);
		
			if (a = strlen(dir)) {
				if (dir[a-1] != '/') strcat(dir,"/");
			}
		}
		else {
			a = strlen(dir) - 1;
			while(dir[a] != '/') a--;
			dir[a + 1] = 0;
			strcpy(file, string + strlen(dir));
		}
	}
	else {
		getwdN(dir);
		strcat(dir, "/");
		file[0] = 0;
	}
#endif
}


void split_sfile(SpaceFile *sfile, char *s1)
{
	short a;
	char string[FILE_MAXDIR+FILE_MAXFILE], dir[FILE_MAXDIR], file[FILE_MAXFILE];

	strcpy(string, s1);

	split_dirfile(string, dir, file);
	
	if(sfile->filelist) {
		if(strcmp(dir, sfile->dir)!=0) {
			freefilelist(sfile);
		}
		else test_flags_file(sfile);
	}
	strcpy(sfile->file, file);
	make_file_string(sfile->dir, dir, "");
}


void parent(SpaceFile *sfile)
{
	short a;
	char *dir;
	
	/* als databrowse: geen parent */
	if(sfile->type==FILE_MAIN && sfile->returnfunc) return;

	dir= sfile->dir;
	
#ifdef WIN32
	if(a = strlen(dir)) {				/* eerst alle '/' weghalen aan het eind */
		while(dir[a-1] == '\\') {
			a--;
			dir[a] = 0;
			if (a<=0) break;
		}
	}
	if(a = strlen(dir)) {				/* daarna alles weghalen tot aan '/' */
		while(dir[a-1] != '\\') {
			a--;
			dir[a] = 0;
			if (a<=0) break;
		}
	}
	if (a = strlen(dir)) {
		if (dir[a-1] != '\\') strcat(dir,"\\");
	}
	else if(sfile->type!=FILE_MAIN) strcpy(dir,"\\");
#else
	if(a = strlen(dir)) {				/* eerst alle '/' weghalen aan het eind */
		while(dir[a-1] == '/') {
			a--;
			dir[a] = 0;
			if (a<=0) break;
		}
	}
	if(a = strlen(dir)) {				/* daarna alles weghalen tot aan '/' */
		while(dir[a-1] != '/') {
			a--;
			dir[a] = 0;
			if (a<=0) break;
		}
	}
	if (a = strlen(dir)) {
		if (dir[a-1] != '/') strcat(dir,"/");
	}
	else if(sfile->type!=FILE_MAIN) strcpy(dir,"/");
#endif
	
	/* to be sure */
	make_exist(sfile->dir);

	freefilelist(sfile);
	sfile->ofs= 0;
	addqueue(curarea->win, REDRAW, 1);
}

void swapselect_file(SpaceFile *sfile)
{
	struct direntry *file;
	int num, act= 0;
	
	file= sfile->filelist;
	for(num=0; num<sfile->totfile; num++, file++) {
		if(file->flags & ACTIVE) {
			act= 1;
			break;
		}
	}
	file= sfile->filelist+2;
	for(num=2; num<sfile->totfile; num++, file++) {
		if(act) file->flags &= ~ACTIVE;
		else file->flags |= ACTIVE;
	}
}

int find_active_file(SpaceFile *sfile, short x, short y)
{
	int ofs;
	
	if(y > textrct.ymax) y= textrct.ymax;
	if(y <= textrct.ymin) y= textrct.ymin+1;
	
	ofs= (x-textrct.xmin)/collumwidth;
	if(ofs<0) ofs= 0;
	ofs*= (textrct.ymax-textrct.ymin);

	return sfile->ofs+ (ofs+textrct.ymax-y)/FILESEL_DY;
	
}


/* ********************** DRAW ******************************* */

void calc_file_rcts(SpaceFile *sfile)
{
	int tot, h, len;
	float fac, start, totfile;
	
	scrollrct.xmin= 15;
	scrollrct.xmax= 35;
	scrollrct.ymin= 10;
	scrollrct.ymax= curarea->winy-10-FILESELHEAD;
	
	textrct.xmin= scrollrct.xmax+10;
	textrct.xmax= curarea->winx-10;
	textrct.ymin= scrollrct.ymin;
	textrct.ymax= scrollrct.ymax;
	
	if(textrct.xmax-textrct.xmin <60) textrct.xmax= textrct.xmin+60;
	
	len= (textrct.ymax-textrct.ymin) % FILESEL_DY;
	textrct.ymin+= len;
	scrollrct.ymin+= len;
	
	filebuty1= curarea->winy-FILESELHEAD;
	filebuty2= filebuty1+FILESELHEAD/2 -6;
	
	
	/* aantal kolommen */
	len= sfile->maxnamelen+25;
	
	if(sfile->type==FILE_MAIN) len+= 100;
	else if(sfile->flag & FILE_SHOWSHORT) len+= 100;
	else len+= 380;
	
	sfile->collums= (textrct.xmax-textrct.xmin)/len;
	
	if(sfile->collums<1) sfile->collums= 1;
	else if(sfile->collums>8) sfile->collums= 8;

	if((U.flag & FSCOLLUM)==0) if(sfile->type!=FILE_MAIN) sfile->collums= 1;
	
	collumwidth= (textrct.xmax-textrct.xmin)/sfile->collums;
	

	totfile= sfile->totfile + 0.5;

	tot= FILESEL_DY*totfile;
	if(tot) fac= ((float)sfile->collums*(scrollrct.ymax-scrollrct.ymin))/( (float)tot);
	else fac= 1.0;
	
	if(sfile->ofs<0) sfile->ofs= 0;
	
	if(tot) start= ( (float)sfile->ofs)/(totfile);
	else start= 0.0;
	if(fac>1.0) fac= 1.0;

	if(start+fac>1.0) {
		sfile->ofs= fceil((1.0-fac)*totfile);
		start= ( (float)sfile->ofs)/(totfile);
		fac= 1.0-start;
	}

	bar.xmin= scrollrct.xmin+2;
	bar.xmax= scrollrct.xmax-2;
	h= (scrollrct.ymax-scrollrct.ymin)-4;
	bar.ymax= scrollrct.ymax-2- start*h;
	bar.ymin= bar.ymax- fac*h;

	pixels_to_ofs= (totfile)/(float)(h+3);
	page_ofs= fac*totfile;
}

int filescrollselect= 0;

void draw_filescroll(SpaceFile *sfile)
{

	if(scrollrct.ymin+10 >= scrollrct.ymax) return;
	
	cpack(0x707070);
	glRecti(scrollrct.xmin,  scrollrct.ymin,  scrollrct.xmax,  scrollrct.ymax);

	EmbossBox2(scrollrct.xmin-2, scrollrct.ymin-2, scrollrct.xmax+2, scrollrct.ymax+1, 1, 0x404040, 0xA0A0A0);

	cpack(0x909090);
	glRecti(bar.xmin,  bar.ymin,  bar.xmax,  bar.ymax);

	EmbossBox2(bar.xmin-2, bar.ymin-2, bar.xmax+2, bar.ymax+2, filescrollselect, 0x404040, 0xA0A0A0);
	
}

void regelrect(uint col, int x, int y)
{
	cpack(col);
	glRects(x-17,  y-3,  x+collumwidth-21,  y+11);

}

void printregel(SpaceFile *sfile, struct direntry *files, int x, int y)
{
	uint boxcol=0;
	char *s;

	switch(files->flags & (HILITE + ACTIVE)) {
	case HILITE+ACTIVE:
		boxcol= (0xC08080);
		break;
	case HILITE:
		boxcol= (0x808080);
		break;
	case ACTIVE:
		boxcol= (0xC07070);
		break;
	}

	if(boxcol) {
		regelrect(boxcol, x, y);
	}

	if(files->flags & BLENDERFILE) {
		cpack(0xA0A0);
		glRects(x-14,  y,  x-8,  y+7);
	}
	else if(files->flags & PSXFILE) {
		cpack(0xA060B0);
		glRects(x-14,  y,  x-8,  y+7);
	}
	else if(files->flags & IMAGEFILE) {
		cpack(0xF08040);
		glRects(x-14,  y,  x-8,  y+7);
	}
	else if(files->flags & MOVIEFILE) {
		cpack(0x70A070);
		glRects(x-14,  y,  x-8,  y+7);
	}
	
	if(S_ISDIR(files->type)) cpack(0xFFFFFF);
	else cpack(0x0);

	fmsetfont(G.font);

	s = files->string;
	if(s) {
		glRasterPos2i(x,  y);
		fmprstr(files->relname);
		/* glutFontDraw(files->relname, x, y); */
		
		x += sfile->maxnamelen + 100;

		glRasterPos2i(x - fmgetstrwidth(G.font, files->size),  y);
		fmprstr(files->size);

		if(sfile->flag & FILE_SHOWSHORT) return;

#ifndef WIN32
		/* rwx rwx rwx */
			x += 20; glRasterPos2i(x, y); 
			fmprstr(files->mode1); 
		
			x += 30; glRasterPos2i(x, y); 
			fmprstr(files->mode2); 
		
			x += 30; glRasterPos2i(x, y); 
			fmprstr(files->mode3); 
		
		/* owner time date */
			x += 30; glRasterPos2i(x, y); 
			fmprstr(files->owner); 
#endif
		
			x += 60; glRasterPos2i(x, y); 
			fmprstr(files->time); 
		
			x += 50; glRasterPos2i(x, y); 
			fmprstr(files->date); 
	}
	else {
		glRasterPos2i(x,  y);
		fmprstr(files->relname);
		
		if(files->nr) {	/* extra info */
			x+= sfile->maxnamelen+20;
			glRasterPos2i(x,  y);
			fmprstr(files->extra);
		}
	}
}


int calc_filesel_regel(SpaceFile *sfile, int nr, int *valx, int *valy)
{
	/* sco van de de regel */
	int val, coll;

	nr-= sfile->ofs;

	/* aantal regels in de hoogte */
	val= (textrct.ymax-textrct.ymin)/FILESEL_DY;
	coll= nr/val;
	nr -= coll*val;
	
	*valy= textrct.ymax-FILESEL_DY+3 - nr*FILESEL_DY;
	*valx= coll*collumwidth + textrct.xmin+20;
	
	if(nr<0 || coll > sfile->collums) return 0;
	return 1;
}

void set_active_file(SpaceFile *sfile, int act)
{
	struct direntry *file;
	int num, redraw= 0, newflag;
	int old=0, new=0;
	
	file= sfile->filelist;
	if(file==0) return;
	
	for(num=0; num<sfile->totfile; num++, file++) {
		if(num==act) {
			
			if(selecting && num>1) {
				newflag= HILITE | (file->flags & ~ACTIVE);
				if(selecting==ACTIVATE) newflag |= ACTIVE;
			
				if(file->flags != newflag) redraw|= 1;
				file->flags= newflag;
			}
			else {
				if(file->flags & HILITE);
				else {
					file->flags |= HILITE;
					redraw|= 2;
					new= num;
				}
			}
		}
		else {
			if(file->flags & HILITE) {
				file->flags &= ~HILITE;
				redraw|= 2;
				old= num;
			}
		}
			
	}
	
	#ifdef IRISGL
	if(redraw) {
		addqueue(curarea->win, REDRAW, 1);
	}
	#else	
	if(redraw==2) {
		int x, y;
		
		glDrawBuffer(GL_FRONT);

		glScissor(curarea->winrct.xmin, curarea->winrct.ymin, curarea->winx-12, curarea->winy);

		if( calc_filesel_regel(sfile, old, &x, &y) ) {
			regelrect(0x686868, x, y);
			printregel(sfile, sfile->filelist+old, x, y);
			/* myCopySubBuffer(x+curarea->winrct.xmin-20, y+curarea->winrct.ymin-4, collumwidth, FILESEL_DY+7); */
		}
		if( calc_filesel_regel(sfile, new, &x, &y) ) {
			glDrawBuffer(GL_FRONT);
			printregel(sfile, sfile->filelist+new, x, y);
			/* myCopySubBuffer(x+curarea->winrct.xmin-20, y+curarea->winrct.ymin-4, collumwidth, FILESEL_DY+7); */
		}
		
		glScissor(curarea->winrct.xmin, curarea->winrct.ymin, curarea->winx, curarea->winy);

		glDrawBuffer(GL_BACK);
	}
	else if(redraw) {
		addqueue(curarea->win, REDRAW, 1);
	}
	#endif
}


void draw_filetext(SpaceFile *sfile)
{
	struct direntry *files;
	int a, x, y, coll;
	short mval[2];
	
	if(textrct.ymin+10 >= textrct.ymax) return;


	/* kader */
	cpack(0x686868);
	glRecti(textrct.xmin,  textrct.ymin,  textrct.xmax,  textrct.ymax);

	/* kolommen */
	x= textrct.xmin+collumwidth;
	for(a=1; a<sfile->collums; a++, x+= collumwidth) {
		cpack(0x303030);
		sdrawline(x,  textrct.ymin,  x,  textrct.ymax); 
		cpack(0xB0B0B0);
		sdrawline(x+1,  textrct.ymin,  x+1,  textrct.ymax); 
	}

	/* de teksten */
	fmsetfont(G.font);
	
	if(sfile->filelist==0) return;
	
	/* test: als muis niet in area staat: de HILITE wissen */
	getmouseco_areawin(mval);
	if(mval[0]<0 || mval[0]>curarea->winx) {
		files= sfile->filelist+sfile->ofs;
		for(a= sfile->ofs; a<sfile->totfile; a++, files++) files->flags &= ~HILITE;
	}
	
	files= sfile->filelist+sfile->ofs;
	for(a= sfile->ofs; a<sfile->totfile; a++, files++) {
	
		if( calc_filesel_regel(sfile, a, &x, &y)==0 ) break;
		
		printregel(sfile, files, x, y);
	}

	/* wissen tekenfoutjes, tekst teveel aan de rechterkant: */
	EmbossBox2(textrct.xmin-2, textrct.ymin-2, textrct.xmax+2, textrct.ymax+2, 1, 0x303030, 0xC0C0C0);
	
	glColor3f(.5625, .5625, .5625);
	glRecti(textrct.xmax+2,  textrct.ymin,  textrct.xmax+10,  textrct.ymax);
}

void drawfilespace()
{
	SpaceFile *sfile;
	int act;
	short mval[2];
	char naam[20];

	glClearColor(.5625, .5625, .5625, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT);

	sfile= curarea->spacedata.first;	
	if(sfile->filelist==0) {
		read_dir(sfile);
		
		calc_file_rcts(sfile);
		
		/* act berekenen */ 
		getmouseco_areawin(mval);
		act= find_active_file(sfile, mval[0], mval[1]);
		if(act>=0 && act<sfile->totfile)
			sfile->filelist[act].flags |= HILITE;
	}
	else calc_file_rcts(sfile);

	/* HEADER */
	sprintf(naam, "win %d", curarea->win);
	DefButBlock(naam, curarea->win, G.font, 10, 6, 4);

	SetButLock( sfile->type==FILE_MAIN && sfile->returnfunc, NULL);

	DefBut(TEX,	    1,"",	textrct.xmin, filebuty1, textrct.xmax-textrct.xmin, 21, sfile->file, 0.0, (float)FILE_MAXFILE-1, 0, 0);
	DefBut(TEX,	    2,"",	textrct.xmin, filebuty2, textrct.xmax-textrct.xmin, 21, sfile->dir, 0.0, (float)FILE_MAXFILE-1, 0, 0);
	SetButShape(2);
	if(fsmenu) DefBut(MENU|SHO,	3, fsmenu, scrollrct.xmin, filebuty1, scrollrct.xmax-scrollrct.xmin, 21, &sfile->menu, 0, 0, 0, 0);
	DefButt(BUT,		4, "P", scrollrct.xmin, filebuty2, scrollrct.xmax-scrollrct.xmin, 21, 0, 0, 0, 0, 0, "Move to the parent directory (PKEY)");


	draw_filescroll(sfile);
	draw_filetext(sfile);
	
	/* andere diskfree etc ? */
	addqueue(curarea->headwin, REDRAW, 1);	

	curarea->win_swap= WIN_BACK_OK;
}

void do_filescroll(SpaceFile *sfile)
{
	float fac;
	int tot, dy;
	short mval[2], oldy, yo;
	
	tot= FILESEL_DY*sfile->totfile;
	if(tot) fac= ((float)(scrollrct.ymax-scrollrct.ymin))/( (float)tot);
	else fac= 1.0;

	calc_file_rcts(sfile);
	
	filescrollselect= 1;
	/* voor mooiigheid */

	glDrawBuffer(GL_FRONT);
	EmbossBox2(bar.xmin-2, bar.ymin-2, bar.xmax+2, bar.ymax+2, filescrollselect, 0x404040, 0xA0A0A0);
	glDrawBuffer(GL_BACK);
	
	getmouseco_areawin(mval);
	oldy=yo= mval[1];
	
	while(get_mbut()&L_MOUSE) {
		getmouseco_areawin(mval);
		
		if(yo!=mval[1]) {
			dy= ffloor(0.5+((float)(oldy-mval[1]))*pixels_to_ofs);
			if(dy) {
				sfile->ofs+= dy;
				if(sfile->ofs<0) {
					sfile->ofs= 0;
					oldy= mval[1];
				}
				else oldy= ffloor(0.5+ (float)oldy - (float)dy/pixels_to_ofs);
	
				curarea->windraw();
				screen_swapbuffers();
	
			}
			yo= mval[1];
		}
		else usleep(2);
	}
	filescrollselect= 0;

	/* voor mooiigheid */
	glDrawBuffer(GL_FRONT);
	EmbossBox2(bar.xmin-2, bar.ymin-2, bar.xmax+2, bar.ymax+2, filescrollselect, 0x404040, 0xA0A0A0);
	glDrawBuffer(GL_BACK);
	
}

void activate_fileselect(int type, char *title, char *file, void (*func)(char *))
{
	SpaceFile *sfile;
	char *str, group[24], name[FILE_MAXDIR], temp[FILE_MAXFILE];
	
	if(curarea==0) return;
	if(curarea->win==0) return;
	
	newspace(curarea, SPACE_FILE);
	addqueue(curarea->win, REDRAW, 1);
	
	/* image wegpushen? Niet bij databrowse */
	if(R.winpop==0 && R.win && !(type==FILE_MAIN && func)) {
		toggle_render_display();
	}
	
	/* is misschien dubbelop, voor geval area al SPACE_FILE is met andere filenaam */
	addqueue(curarea->headwin, CHANGED, 1);
	

	name[2]= 0;
	strcpy(name, file);
	
	sfile= curarea->spacedata.first;
	sfile->returnfunc= func;
	sfile->type= type;
	sfile->ofs= 0;
		/* sfile->act wordt gebruikt bij databrowse: dubbele namen van library objecten */
	sfile->act= -1;
	
	if(convertstringcode(name)) sfile->flag |= FILE_STRINGCODE;
	else sfile->flag &= ~FILE_STRINGCODE;

	if(type==FILE_MAIN) {
		strcpy(sfile->file, name+2);

		str= code_to_groupname( GS(name) );
		strcpy(sfile->dir, str);

		/* alles vrijgeven */
		if(sfile->libfiledata) freeN(sfile->libfiledata);
		sfile->libfiledata= 0;
		
		freefilelist(sfile);
	}
	else if(type==FILE_LOADLIB) {
		strcpy(sfile->dir, name);
		if( is_a_library(sfile, temp, group) ) {
			/* dit geval is om een reload van library-filelist te veroorzaken */
			if(sfile->libfiledata==0) {
				freefilelist(sfile);
			}
		}
		else {
			split_sfile(sfile, name);
			if(sfile->libfiledata) freeN(sfile->libfiledata);
			sfile->libfiledata= 0;
		}
	}
	else {	/* FILE_BLENDER */
		split_sfile(sfile, name);	/* test ook de filelist */
		
		/* vrijgeven: filelist en libfiledata kloppen niet meer */
		if(sfile->libfiledata) freeN(sfile->libfiledata);
		sfile->libfiledata= 0;
	}
	strncpy(sfile->title, title, 23);
	filetoname= 1;
}

void activate_imageselect(int type, char *title, char *file, void (*func)(char *))
{
	SpaceImaSel *simasel;
	char *str, dir[FILE_MAXDIR], name[FILE_MAXFILE];
	
	if(curarea==0) return;
	if(curarea->win==0) return;
	
	newspace(curarea, SPACE_IMASEL);
	
	/* image wegpushen? Niet bij databrowse */
	if(R.winpop==0 && R.win && func) {
		toggle_render_display();
	}
	
	/* is misschien dubbelop, voor geval area al SPACE_FILE is met andere filenaam */
	addqueue(curarea->headwin, CHANGED, 1);
	addqueue(curarea->win, CHANGED, 1);

	name[2]= 0;
	strcpy(name, file);

	simasel= curarea->spacedata.first;
	simasel->returnfunc= func;

	if(convertstringcode(name)) simasel->mode |= IMS_STRINGCODE;
	else simasel->mode &= ~IMS_STRINGCODE;
	
	split_dirfile(name, dir, simasel->file);
	if(strcmp(dir, simasel->dir)!=0) simasel->fase= 0;
	strcpy(simasel->dir, dir);
	
	strncpy(simasel->title, title, 23);

	
	
	/* filetoname= 1; */
}


void activate_databrowse(ID *id, int idcode, int fromcode, int retval, void (*func)(ushort))
{
	ListBase *lb;
	SpaceFile *sfile;
	char str[32];
	
	if(id==0) {
		lb= wich_libbase(G.main, idcode);
		id= lb->first;
	}
	
	if(id) strcpy(str, id->name);
	else return;
	
	activate_fileselect(FILE_MAIN, "SELECT DATABLOCK", str, (void (*) (char*))func);
	
	sfile= curarea->spacedata.first;
	sfile->retval= retval;
	sfile->ipotype= fromcode;
}

void filesel_prevspace()
{
	SpaceFile *sfile;
	
	sfile= curarea->spacedata.first;
	if(sfile->next) {
	
		remlink(&curarea->spacedata, sfile);
		addtail(&curarea->spacedata, sfile);

		sfile= curarea->spacedata.first;
		newspace(curarea, sfile->spacetype);
	}
	else newspace(curarea, SPACE_INFO);
}

int countselect(SpaceFile *sfile)
{
	int a, count=0;

	for(a=0; a<sfile->totfile; a++) {
		if(sfile->filelist[a].flags & ACTIVE) {
			count++;
		}
	}
	return count;
}

int getotherdir()
{
	ScrArea *sa;
	SpaceFile *sfile=0;
	
	sa= G.curscreen->areabase.first;
	while(sa) {
		if(sa!=curarea) {
			if(sa->spacetype==SPACE_FILE) {
				
				/* al een gevonden */
				if(sfile) return 0;
		
				sfile= sa->spacedata.first;

				if(sfile->type & FILE_UNIX) {
					otherarea= sa;
					make_file_string (otherdir, sfile->dir, "");
				}
				else sfile= 0;
			}
		}
		sa= sa->next;
	}
	if(sfile) return 1;
	return 0;
}

void reread_other_fs()
{
	SpaceFile *sfile;
	
	/* oppassen: alleen aanroepen als getotherdir goed is afgelopen */
	
	sfile= otherarea->spacedata.first;
	freefilelist(sfile);
	addqueue(otherarea->win, REDRAW, 1);
}


void free_filesel_spec(char *dir)
{
	/* alle filesels met 'dir' worden vrijgegeven */
	bScreen *sc;
	ScrArea *sa;
	SpaceFile *sfile;
	int len;
	char str[FILE_MAXDIR+FILE_MAXFILE];
	
	/* dir moet eindigen op '/' */
	strcpy(str, dir);
	len= strlen(str);
	while(len && str[len-1]!='/') {
		len--;
	}
	if(len==0) return;
	
	sc= G.main->screen.first;
	while(sc) {
		sa= sc->areabase.first;
		while(sa) {
			sfile= sa->spacedata.first;
			while(sfile) {
				if(sfile->spacetype==SPACE_FILE) {
					if(strcmp(sfile->dir, str)==0) freefilelist(sfile);
				}
				sfile= sfile->next;
			}
			sa= sa->next;
		}
		sc= sc->id.next;
	}
}


void filesel_execute(SpaceFile *sfile)
{
	struct direntry *files;
	int a;
	short *menup;
	char name[FILE_MAXDIR];
	
	filesel_prevspace();

	if(sfile->type==FILE_LOADLIB) {
		library_append(sfile);
		allqueue(REDRAWALL, 1);
	}
	else if(sfile->returnfunc) {
		addfilename_to_fsmenu(sfile->dir);
	
		if(sfile->type==FILE_MAIN) {
			
			if(sfile->retval==B_TEXBROWSE) menup= &G.buts->texnr;
			else if(sfile->retval==B_WTEXBROWSE) menup= &G.buts->texnr;
			else if(sfile->retval==B_LTEXBROWSE) menup= &G.buts->texnr;
			else if(sfile->retval==B_IPOBROWSE) menup= &G.sipo->menunr;
			else if(sfile->retval==B_INFOSCE) menup= &G.curscreen->scenenr;
			else if(sfile->retval==B_INFOSCR) menup= &G.curscreen->screennr;
			else if(sfile->retval==B_SIMABROWSE) menup= &G.sima->imanr;
			else menup= &G.buts->menunr;
			
			if(sfile->act>=0) {
				if(sfile->filelist) {
					files= sfile->filelist+sfile->act;
					*menup= files->nr-1;
				}	
				else *menup= sfile->act;
			}
			else {
				*menup= -1;
				for(a=0; a<sfile->totfile; a++) {
					if( strcmp(sfile->filelist[a].relname, sfile->file)==0) {
						*menup= a;
						break;
					}
				}
			}
			sfile->returnfunc(sfile->retval);
		}
		else {
			if(strncmp(sfile->title, "SAVE", 4)==0) free_filesel_spec(sfile->dir);
			
			strcpy(name, sfile->dir);
			strcat(name, sfile->file);
			
			if(sfile->flag & FILE_STRINGCODE) makestringcode(name);

			sfile->returnfunc(name);
		}
	}
}

void do_filesel_buttons(short event, SpaceFile *sfile)
{
	char butname[FILE_MAXDIR];
	
	if (event == 1) {
		if (strchr(sfile->file, '*') || strchr(sfile->file, '?') || strchr(sfile->file, '[')) {
			int i, match = FALSE;
			
			for (i = 2; i < sfile->totfile; i++) {
				if (fnmatch(sfile->file, sfile->filelist[i].relname, 0) == 0) {
					sfile->filelist[i].flags |= ACTIVE;
					match = TRUE;
				}
			}
			if (match) strcpy(sfile->file, "");
			if(sfile->type==FILE_MAIN) filesel_select_objects(sfile);
			addqueue(curarea->win, REDRAW, 1);
		}
	}
	else if(event== 2) {
		/* reuse the butname variable */
		checkdir(sfile->dir);

		make_file_string(butname, sfile->dir, "");
		/* strip the trailing slash if its a real dir */
		if (strlen(butname)!=1)
			butname[strlen(butname)-1]=0;
		
		if(sfile->type & FILE_UNIX) {
			if (!fop_exists(butname)) {
				if (okee("Makedir")) {
					fop_recurdir(butname);
					if (!fop_exists(butname)) parent(sfile);
				} else parent(sfile);
			}
		}
		freefilelist(sfile);
		sfile->ofs= 0;
		addqueue(curarea->win, REDRAW, 1);
	}
	else if(event== 3) {
		
		/* welke string */
		getname_menu_but(butname, fsmenu, sfile->menu);
		strcpy(sfile->dir, butname);
		make_exist(sfile->dir);
		checkdir(sfile->dir);
		freefilelist(sfile);
		sfile->ofs= 0;
		addqueue(curarea->win, REDRAW, 1);
		
		sfile->act= -1;
		
	}
	else if(event== 4) parent(sfile);
	
}

int docmd(char *cmd)
{
	int file, err, size;
	short val;

	if (qtest()) {
		if (extern_qread(&val) == ESCKEY){
			return(-1);
		}
	}
/*	
	if(fslog[0]==0) {
		sprintf(fslog, "/tmp/fslog%d", getpid());
	}

	strcpy(string, cmd);
	strcat(string, " 1>/dev/null 2>>");
	strcat(string, fslog);

	remove(fslog);
*/

	waitcursor(1);

	err= system(cmd);	
/*
	if (err = system(string)){
		if (file = open(fslog, O_BINARY|O_RDONLY)){
			if ((size = read(file, string, sizeof(string)-1)) > 0){
				string[size] = 0;
				error(string);
			} else error(cmd);
			close(file);
		} else error(cmd);
	}
	remove(fslog);
*/

	waitcursor(0);
	
	return(err);
}


void databrowse_replace(SpaceFile *sfile)
{
	Material *old, *new;
	int i;
	char str[128];
	
	if( strcmp(sfile->dir, "Material/")==0 ) {
	
		if(sfile->act== -1) {
			error("Select with leftmouse");
			return;
		}
		new= (Material *)sfile->filelist[sfile->act].poin;
		
		if(new) {
			sprintf(str, "Replace with Material %s", new->id.name+2);
		
			if(okee(str)==0) return;
	
			for (i = 0; i <sfile->totfile; i++) {
				if (sfile->filelist[i].flags & ACTIVE) {
					old= (Material *)sfile->filelist[i].poin;
					replace_material(old, new);
				}
			}
			freefilelist(sfile);
			allqueue(REDRAWBUTSMAT, 0);
		}
		
	}
	addqueue(curarea->win, REDRAW, 1);
}

void fs_fake_users(SpaceFile *sfile)
{
	ID *id;
	int a;
	
	/* alleen bij F4 DATABROWSE */
	if(sfile->returnfunc) return;
	
	for(a=0; a<sfile->totfile; a++) {
		if(sfile->filelist[a].flags & ACTIVE) {
			id= (ID *)sfile->filelist[a].poin;
			if(id) {
				if( id->flag & LIB_FAKEUSER) {
					id->flag -= LIB_FAKEUSER;
					id->us--;
				}
				else {
					id->flag |= LIB_FAKEUSER;
					id->us++;
				}
			}
		}
	}
	freefilelist(sfile);
	addqueue(curarea->win, REDRAW, 1);
}

void make_file_string (char *string,  char *dir,  char *file)
{

	if (!string || !dir || !file) return; /* We don't want any NULLs */
	
	string[0]= 0; /* ton */

	/* Resolve relative references */	
	if (dir[0] == '/' && dir[1] == '/') {
		char *lslash;
		
		/* Get the file name, chop everything past the last slash (ie. the filename) */
		strcpy(string, G.sce);
		
		lslash= (strrchr(string, '/')>strrchr(string, '\\'))?strrchr(string, '/'):strrchr(string, '\\');
		
		if(lslash) *(lslash+1)= 0;

		dir+=2; /* Skip over the relative reference */
	}
	
	strcat(string, dir);

	/* Make sure string ends in one (and only one) slash */
	if (string[strlen(string)-1] != '/' && string[strlen(string)-1] != '\\')
		strcat(string, "/");
	
	while (*file && (*file == '/' || *file == '\\')) /* Trim slashes from the front of file */
		file++;
		
	strcat (string, file);
	
	/* Push all slashes to the system preferred direction */
	#ifdef WIN32
		char_switch(string, '/', '\\');
	#else
		char_switch(string, '\\', '/');
	#endif	
}

void winqreadfilespace(ushort event, short val)
{
	static int acto=0, prevtime=0;
	struct tms voidbuf;
	SpaceFile *sfile;
	int act, do_draw= 0, i, test, newtime, ret;
	short qual, mval[2];
	ushort numlen;
	char head[FILE_MAXDIR], tail[FILE_MAXFILE], str[FILE_MAXDIR+FILE_MAXFILE+12];
	
	sfile= curarea->spacedata.first;
	if(sfile==0) return;
	if(sfile->filelist==0) {
		/* wel buttons doen */
		if(val && event==LEFTMOUSE) {
			FrontbufferButs(TRUE);
			event= DoButtons();
			FrontbufferButs(FALSE);
				
			if(event) do_filesel_buttons(event, sfile);	/*  NIET de headerbuttons! */
		}
		return;
	}
	
	if(curarea->win==0) return;
	calc_file_rcts(sfile);
	getmouseco_areawin(mval);

	/* om hangen te voorkomen */
	if(selecting && !(get_mbut()&R_MOUSE)) selecting= 0;


	if(val) {
		switch(event) {
		
		case LEFTMOUSE:
			
		case MIDDLEMOUSE:
			if(mval[0]>scrollrct.xmin && mval[0]<scrollrct.xmax && mval[1]>scrollrct.ymin && mval[1]<scrollrct.ymax) {
				do_filescroll(sfile);
			}
			else if(mval[0]>textrct.xmin && mval[0]<textrct.xmax && mval[1]>textrct.ymin && mval[1]<textrct.ymax) {
				
				/* sfile->act wordt gebruikt bij databrowse: dubbelenamen van library objecten */
				
				sfile->act= act= find_active_file(sfile, mval[0], mval[1]);
				
				if(act>=0 && act<sfile->totfile) {
					if(S_ISDIR(sfile->filelist[act].type)) {
						strcat(sfile->dir, sfile->filelist[act].relname);
						strcat(sfile->dir,"/");
						checkdir(sfile->dir);
						freefilelist(sfile);
						sfile->ofs= 0;
						do_draw= 1;
					}
					else {
						if( strcmp(sfile->file, sfile->filelist[act].relname)) {
							do_draw= 1;
							strcpy(sfile->file, sfile->filelist[act].relname);
						}
						if(event==MIDDLEMOUSE && sfile->type) filesel_execute(sfile);
					}
				}
			}
			else {
				FrontbufferButs(TRUE);
				event= DoButtons();
				FrontbufferButs(FALSE);
				
				if(event) do_filesel_buttons(event, sfile);	/*  NIET de headerbuttons! */
			}
#ifdef __sgi			
			if(event==LEFTMOUSE && (sfile->type & (FILE_UNIX))) {
				if(mval[0]>textrct.xmin && mval[0]<textrct.xmax && mval[1]>textrct.ymin && mval[1]<textrct.ymax) {
					newtime = times(&voidbuf);
					if ( (newtime - prevtime <= 20)) {
						workspacecommand(sfile);
						newtime -= 60;
					}
					prevtime = newtime;
				}
			}
#endif			
			break;
		case RIGHTMOUSE:
			act= find_active_file(sfile, mval[0], mval[1]);
			acto= act;
			if(act>=0 && act<sfile->totfile) {

				if (sfile->filelist[act].flags & ACTIVE) {
					sfile->filelist[act].flags &= ~ACTIVE;
					selecting = INACTIVATE;
				}
				else {
					test= sfile->filelist[act].relname[0];
					if (act>=2 || test!='.') sfile->filelist[act].flags |= ACTIVE;
					
					selecting = ACTIVATE;
				}
				do_draw= 1;
			}
			break;
		case MOUSEY:
			act= find_active_file(sfile, mval[0], mval[1]);
			set_active_file(sfile, act);
			
			if(selecting && act!=acto) {
					
				while(1) {
					if (acto >= 2 && acto < sfile->totfile) {
						if (selecting == ACTIVATE) sfile->filelist[acto].flags |= ACTIVE;
						else if (selecting == INACTIVATE) sfile->filelist[acto].flags &= ~ACTIVE;
					}
					if (acto < act) acto++;
					else if (acto > act) acto--;
					else break;
					
				}

			}
			acto= act;
			break;
		
		case PAGEUPKEY:
			sfile->ofs-= page_ofs;
			do_draw= 1;
			break;
		case PAGEDOWNKEY:
			sfile->ofs+= page_ofs;
			do_draw= 1;
			break;
		case HOMEKEY:
			sfile->ofs= 0;
			do_draw= 1;
			break;
		case ENDKEY:
			sfile->ofs= sfile->totfile;
			do_draw= 1;
			break;
		
		case AKEY:
			swapselect_file(sfile);
			if(sfile->type==FILE_MAIN) filesel_select_objects(sfile);
			do_draw= 1;
			break;
			
		case BKEY:
		case CKEY:
		case LKEY:
			if(event==LKEY && sfile->type==FILE_MAIN && (G.qual & LR_CTRLKEY)) {
				databrowse_replace(sfile);
				break;
			}
			/* doorgeven */
		case MKEY:
			if(sfile->type==FILE_MAIN) break;

			if(!countselect(sfile)) {
				error("No files selected");
				break;
			}
			
			if(!getotherdir()) {
				error("No second fileselect");
				break;
			}
			
			if (!strcmp(sfile->dir, otherdir)) {
				error("Same directories");
				break;
			}

			if(event==BKEY) sprintf(str, "Backup to %s", otherdir);
			else if(event==CKEY) sprintf(str, "Copy to %s", otherdir);
			else if(event==LKEY) sprintf(str, "Linked copy to %s", otherdir);
			else if(event==MKEY) sprintf(str, "Move to %s", otherdir);
					
			if (!okee(str)) break;

			for (i = 0; i<sfile->totfile; i++){
				if (sfile->filelist[i].flags & ACTIVE) {			
					make_file_string(str, sfile->dir, sfile->filelist[i].relname);

					if(event==BKEY) ret= fop_backup(sfile->filelist[i].relname, sfile->dir, otherdir);
					else if(event==CKEY) ret= fop_copy(str, otherdir);
					else if(event==LKEY) ret= fop_link(str, otherdir);
					else if(event==MKEY) ret= fop_move(str, otherdir);

					if (ret) {error("Command failed, see console"); break;}
					else sfile->filelist[i].flags &= ~ACTIVE;
				}
			}
			do_draw= 1;
			if(event==BKEY || event==MKEY) 
				freefilelist(sfile);
				
			reread_other_fs();
			
			break;
		case RKEY:
			if(sfile->type==FILE_MAIN) {
				databrowse_replace(sfile);
				break;
			}
			/* doorgeven aan TKEY! */
			
		case TKEY:
			if(sfile->type==FILE_MAIN) break;
			
			if(!countselect(sfile)) {
				error("No files selected");
				break;
			}

			if(event==TKEY) sprintf(str, "Touch");
			else if(event==RKEY) sprintf(str, "Remove from %s", sfile->dir);
			
			qual= G.qual;	/* want na okee() heb je de shift losgelaten */
			if (!okee(str)) break;
			
			for (i = 0; i <sfile->totfile; i++) {
				if (sfile->filelist[i].flags & ACTIVE) {
					make_file_string(str, sfile->dir, sfile->filelist[i].relname);

					if(event==TKEY) ret= fop_touch(str);
					else if(event==RKEY) {
						if(qual & LR_SHIFTKEY) ret= fop_delete(str, 0, 1);
						else if(S_ISDIR(sfile->filelist[i].type)) ret= fop_delete(str, 1, 0);
						else ret= fop_delete(str, 0, 0);
					}

					if (ret) {error("Command failed, see console"); break;}
					else sfile->filelist[i].flags &= ~ACTIVE;
				}
			}
			do_draw= 1;
			freefilelist(sfile);

			break;
				
		case PKEY:
			if(G.qual & LR_SHIFTKEY) {
				extern char bprogname[];	/* usiblender.c */
			
				sprintf(str, "%s -a %s%s", bprogname, sfile->dir, sfile->file);
				system(str);
			}
			else 
				parent(sfile);
				
			break;

		case IKEY:
			if(sfile->type==FILE_MAIN) break;
			
			sprintf(str, "$IMAGEEDITOR %s%s", sfile->dir, sfile->file);
			system(str);
			break;
		
		case EKEY:
			if(sfile->type==FILE_MAIN) break;
			
			sprintf(str, "$WINEDITOR %s%s", sfile->dir, sfile->file);
			system(str);
			break;
		
		case FKEY:
			if(sfile->type==FILE_MAIN) {
				fs_fake_users(sfile);
			}
			break;
				
		case PADPLUSKEY:
		case EQUALKEY:
			if (G.qual & LR_CTRLKEY) newname(sfile->file, +100);
			else if (G.qual & LR_SHIFTKEY) newname(sfile->file, +10);
			else newname(sfile->file, +1);
			
			do_draw= 1;
			break;
			
		case PADMINUS:
		case MINUSKEY:
			if (G.qual & LR_CTRLKEY) newname(sfile->file, -100);
			else if (G.qual & LR_SHIFTKEY) newname(sfile->file, -10);
			else newname(sfile->file, -1);
			
			do_draw= 1;
			break;
			
		case BACKSLASHKEY:
		case SLASHKEY:
			if(sfile->type==FILE_MAIN) break;

#ifdef WIN32
			strcpy(sfile->dir, "\\");
#else
			strcpy(sfile->dir, "/");
#endif
			freefilelist(sfile);
			sfile->ofs= 0;
			do_draw= 1;
			break;
		case PERIODKEY:
			freefilelist(sfile);
			do_draw= 1;
			break;
		case ESCKEY:
			filesel_prevspace();
			break;
		case PADENTER:
		case RETKEY:
			if(sfile->type) filesel_execute(sfile);
			break;
		}
	}
	else if(event==RIGHTMOUSE) {
		selecting = NOTACTIVE;
		if(sfile->type==FILE_MAIN) filesel_select_objects(sfile);
	}
	else if(event==LEFTMOUSE) {
		if(sfile->type==FILE_MAIN) active_file_object(sfile);
	}

	if(do_draw) addqueue(curarea->win, REDRAW, 1);
}




/* ************* LIBRARY FILESEL ******************* */

int libdir[100];

int groupname_to_code(char *group)
{

	if(strncmp(group, "Scene", 5)==0) return ID_SCE;
	if(strncmp(group, "Object", 6)==0) return ID_OB;
	if(strncmp(group, "Mesh", 4)==0) return ID_ME;
	if(strncmp(group, "Curve", 5)==0) return ID_CU;
	if(strncmp(group, "Metaball", 8)==0) return ID_MB;
	if(strncmp(group, "Material", 8)==0) return ID_MA;
	if(strncmp(group, "Texture", 7)==0) return ID_TE;
	if(strncmp(group, "Image", 5)==0) return ID_IM;
	if(strncmp(group, "Ika", 3)==0) return ID_IK;
	if(strncmp(group, "Wave", 4)==0) return ID_WV;
	if(strncmp(group, "Lattice", 7)==0) return ID_LT;
	if(strncmp(group, "Lamp", 4)==0) return ID_LA;
	if(strncmp(group, "Camera", 6)==0) return ID_CA;
	if(strncmp(group, "Key", 3)==0) return ID_KE;
	if(strncmp(group, "Ipo", 3)==0) return ID_IP;
	if(strncmp(group, "World", 5)==0) return ID_WO;
	if(strncmp(group, "Screen", 6)==0) return ID_SCR;
	if(strncmp(group, "VFont", 5)==0) return ID_VF;
	if(strncmp(group, "Text", 4)==0) return ID_TXT;
	return 0;	
}

char *code_to_groupname(int code)
{
	char *str;
	
	switch(code) {
		case ID_SCE: str= "Scene/"; break;
		case ID_OB: str= "Object/"; break;
		case ID_ME: str= "Mesh/"; break;
		case ID_CU: str= "Curve/"; break;
		case ID_MB: str= "Metaball/"; break;
		case ID_MA: str= "Material/"; break;
		case ID_TE: str= "Texture/"; break;
		case ID_IM: str= "Image/"; break;
		case ID_IK: str= "Ika/"; break;
		case ID_WV: str= "Wave/"; break;
		case ID_LT: str= "Lattice/"; break;
		case ID_LA: str= "Lamp/"; break;
		case ID_CA: str= "Camera/"; break;
		case ID_KE: str= "Key/"; break;
		case ID_IP: str= "Ipo/"; break;
		case ID_WO: str= "World/"; break;
		case ID_TXT: str= "Text/"; break;
		case ID_SCR: str= "Screen/"; break;
		case ID_VF: str= "Vfont/"; break;
		default: str= "";
	}
	return str;
}

int count_libfiles(SpaceFile *sfile, int idcode)
{
	BHead *bhead;
	int tot=0, afbreek=0;
	char *fd;
	
	fd= sfile->libfiledata;
	while(afbreek==0) {
		
		bhead= (BHead *)fd;
		
		if(bhead->code==ENDB) afbreek= 1;
		else if(bhead->code==idcode) tot++;
		
		fd+= bhead->len+sizeof(BHead);
	}
	return tot;
}


int count_libdirs(SpaceFile *sfile)
{
	BHead *bhead;
	int tot=0, a, afbreek=0;
	char *fd;
	
	libdir[0]= 0;
	
	fd= sfile->libfiledata;
	while(afbreek==0) {
		
		bhead= (BHead *)fd;
		
		switch(bhead->code) {
		case GLOB:
		case DATA:
		case USER:
		case TEST:
		case DNA1:
		case ID_SCR:
		case ID_LA:
		case ID_CA:
			break;
		case ENDB:
			afbreek= 1;
			break;
		default:
			for(a=0; a<tot; a++) {
				if(libdir[a]==bhead->code) break;
			}
			if(a==tot) {
				libdir[tot]= bhead->code;
				tot++;
				if(tot>99) tot= 99;
			}
		}
		
		fd+= bhead->len+sizeof(BHead);
	}
	return tot;
}

int is_a_library(SpaceFile *sfile, char *dir, char *group)
{
	/* return ok als een blenderfile, in dir staat de filenaam,
	 * in group het type libdata
	 */
	int len;
	char *fd;
	
	strcpy(dir, sfile->dir);
	len= strlen(dir);
	if(len<7) return 0;
	if( dir[len-1] != '/' && dir[len-1] != '\\') return 0;
	
	group[0]= 0;
	dir[len-1]= 0;

	/* Find the last slash */
	fd= (strrchr(dir, '/')>strrchr(dir, '\\'))?strrchr(dir, '/'):strrchr(dir, '\\');

	if(fd==0) return 0;
	*fd= 0;
	if(testextensie(fd+1, ".blend")) {
		*fd= '/';
	}
	else {
		strcpy(group, fd+1);

		/* Find the last slash */
		fd= (strrchr(dir, '/')>strrchr(dir, '\\'))?strrchr(dir, '/'):strrchr(dir, '\\');
		if(fd==0 || testextensie(fd+1, ".blend")==0) return 0;
	}
	return 1;
}

void library_to_filelist(SpaceFile *sfile)
{
	BHead *bhead;
	ID *id;
	int file, filelen, temp, temp1, a, actual, ok, idcode, len;
	char dir[FILE_MAXDIR], group[24], *fd, *str;

	/* naam testen */
	ok= is_a_library(sfile, dir, group);
	
	if(ok==0) {
		/* vrijgeven */
		if(sfile->libfiledata) freeN(sfile->libfiledata);
		sfile->libfiledata= 0;
		return;
	}
	
	/* en daar gaat ie */
	/* voorlopig alleen filedata inlezen als libfiledata==0 */
	if(sfile->libfiledata==0) {
	
		sfile->libfiledata= openblenderfile(dir, &filelen);
		if(sfile->libfiledata==0) return;
	}
	
	if(group[0]==0) {
		
		/* directories maken */
		sfile->totfile= count_libdirs(sfile);

		sfile->totfile+= 2;
		sfile->filelist= (struct direntry *)malloc(sfile->totfile * sizeof(struct direntry));
		actual= 2;
		
		for(a=0; a<sfile->totfile; a++) {
			memset( &(sfile->filelist[a]), 0 , sizeof(struct direntry));
			if(a==0) {
				sfile->filelist[a].relname= strdup(".");
				sfile->filelist[a].type |= S_IFDIR;
			}
			else if(a==1) {
				sfile->filelist[a].relname= strdup("..");
				sfile->filelist[a].type |= S_IFDIR;
			}
			else {
				switch( libdir[a-2] ) {
					case ID_SCE: str= "Scene"; break;
					case ID_OB: str= "Object"; break;
					case ID_ME: str= "Mesh"; break;
					case ID_CU: str= "Curve"; break;
					case ID_MB: str= "Metaball"; break;
					case ID_MA: str= "Material"; break;
					case ID_TE: str= "Texture"; break;
					case ID_IP: str= "Ipo"; break;
					case ID_WO: str= "World"; break;
					case ID_TXT: str= "Text"; break;
					case ID_LA: str= "Lamp"; break;
					default: str=0; actual--;
						*( (short *)group)= libdir[a-2];
						group[3]= 0;
				}
				if(str) {
					sfile->filelist[actual].relname= strdup(str);
					sfile->filelist[actual].type |= S_IFDIR;
				}
				actual++;
			}
		}
		sfile->totfile= actual;
		
		qsort(sfile->filelist, actual, sizeof(struct direntry), (void *)compare);
	}
	else {

		/* files maken */
		idcode= groupname_to_code(group);
		sfile->totfile= count_libfiles(sfile, idcode);
		
		sfile->totfile+= 2;
		sfile->filelist= (struct direntry *)malloc(sfile->totfile * sizeof(struct direntry));
		
		memset( &(sfile->filelist[0]), 0 , sizeof(struct direntry));
		sfile->filelist[0].relname= strdup(".");
		sfile->filelist[0].type |= S_IFDIR;
		memset( &(sfile->filelist[1]), 0 , sizeof(struct direntry));
		sfile->filelist[1].relname= strdup("..");
		sfile->filelist[1].type |= S_IFDIR;
		
		actual= 2;
		fd= sfile->libfiledata;
		while(TRUE) {
			bhead= (BHead *)fd;
			
			if(bhead->code==ENDB) break;
			else if(bhead->code==idcode) {
				memset( &(sfile->filelist[actual]), 0 , sizeof(struct direntry));

				id= (ID *)(bhead+1);
				
				sfile->filelist[actual].relname= strdup(id->name+2);
				actual++;
			}
			
			fd+= bhead->len+sizeof(BHead);

		}
		
		qsort(sfile->filelist, sfile->totfile, sizeof(struct direntry), (void *)compare);
	}
	
	sfile->maxnamelen= 0;
	for(a=0; a<sfile->totfile; a++) {
		len = fmgetstrwidth(G.font, sfile->filelist[a].relname);
		if (len > sfile->maxnamelen) sfile->maxnamelen = len;
	}
}

/* ******************* DATA SELECT ********************* */

void filesel_select_objects(SpaceFile *sfile)
{
	Object *ob;
	Base *base;
	Scene *sce;
	int a;
	
	/* alleen bij F4 DATABROWSE */
	if(sfile->returnfunc) return;
	
	if( strcmp(sfile->dir, "Object/")==0 ) {
		for(a=0; a<sfile->totfile; a++) {
			
			ob= (Object *)sfile->filelist[a].poin;
			
			if(ob) {
				if(sfile->filelist[a].flags & ACTIVE) ob->flag |= SELECT;
				else ob->flag &= ~SELECT;
			}

		}
		base= FIRSTBASE;
		while(base) {
			base->flag= base->object->flag;
			base= base->next;
		}
		allqueue(REDRAWVIEW3D, 0);
	}
	else if( strcmp(sfile->dir, "Scene/")==0 ) {
		
		for(a=0; a<sfile->totfile; a++) {
			
			sce= (Scene *)sfile->filelist[a].poin;
			if(sce) {
				if(sfile->filelist[a].flags & ACTIVE) sce->r.scemode |= R_BG_RENDER;
				else sce->r.scemode &= ~R_BG_RENDER;
			}

		}
		allqueue(REDRAWBUTSRENDER, 0);
	}
}

void active_file_object(SpaceFile *sfile)
{
	Object *ob;
	Base *base;
	Scene *sce;
	int a;
	
	/* alleen bij F4 DATABROWSE */
	if(sfile->returnfunc) return;
	
	if( strcmp(sfile->dir, "Object/")==0 ) {
		if(sfile->act >= 0) {
			
			ob= (Object *)sfile->filelist[sfile->act].poin;
			
			if(ob) {
				set_active_object(ob);
				if(BASACT && BASACT->object==ob) {
					BASACT->flag |= SELECT;
					sfile->filelist[sfile->act].flags |= ACTIVE;
					allqueue(REDRAWVIEW3D, 0);
					addqueue(curarea->win, REDRAW, 1);
				}
			}
		}
	}
}


void main_to_filelist(SpaceFile *sfile)
{
	ID *id;
	struct direntry *files, *firstlib;
	ListBase *lb;
	int a, fake, idcode, len, ok, totlib, totbl;
	char dir[FILE_MAXDIR], group[24], *fd, *str;

	if(sfile->dir[0]=='/') sfile->dir[0]= 0;
	
	if(sfile->dir[0]) {
		idcode= groupname_to_code(sfile->dir);
		if(idcode==0) sfile->dir[0]= 0;
	}
	
	if( sfile->dir[0]==0) {
		
		/* directories maken */
		sfile->totfile= 19;

		sfile->filelist= (struct direntry *)malloc(sfile->totfile * sizeof(struct direntry));
		
		for(a=0; a<sfile->totfile; a++) {
			memset( &(sfile->filelist[a]), 0 , sizeof(struct direntry));
			sfile->filelist[a].type |= S_IFDIR;
		}
		
		sfile->filelist[0].relname= strdup("..");
		sfile->filelist[1].relname= strdup(".");
		sfile->filelist[2].relname= strdup("Scene");
		sfile->filelist[3].relname= strdup("Object");
		sfile->filelist[4].relname= strdup("Mesh");
		sfile->filelist[5].relname= strdup("Curve");
		sfile->filelist[6].relname= strdup("Metaball");
		sfile->filelist[7].relname= strdup("Material");
		sfile->filelist[8].relname= strdup("Texture");
		sfile->filelist[9].relname= strdup("Image");
		sfile->filelist[10].relname= strdup("Ika");
		sfile->filelist[11].relname= strdup("Wave");
		sfile->filelist[12].relname= strdup("Lattice");
		sfile->filelist[13].relname= strdup("Lamp");
		sfile->filelist[14].relname= strdup("Camera");
		sfile->filelist[15].relname= strdup("Ipo");
		sfile->filelist[16].relname= strdup("World");
		sfile->filelist[17].relname= strdup("Screen");
		sfile->filelist[18].relname= strdup("VFont");
		sfile->filelist[18].relname= strdup("Text");
		
		qsort(sfile->filelist, sfile->totfile, sizeof(struct direntry), (void *)compare);
	}
	else {

		/* files maken */
		idcode= groupname_to_code(sfile->dir);
		
		lb= wich_libbase(G.main, idcode );
		if(lb==0) return;
		
		id= lb->first;
		sfile->totfile= 0;
		while(id) {
			
			if(sfile->returnfunc && idcode==ID_IP) {
				if(sfile->ipotype== ((Ipo *)id)->blocktype) sfile->totfile++;
			}
			else sfile->totfile++;
			
			id= id->next;
		}
		
		if(sfile->returnfunc==0) sfile->totfile+= 2;
		sfile->filelist= (struct direntry *)malloc(sfile->totfile * sizeof(struct direntry));
		
		files= sfile->filelist;
		
		if(sfile->returnfunc==0) {
			memset( &(sfile->filelist[0]), 0 , sizeof(struct direntry));
			sfile->filelist[0].relname= strdup(".");
			sfile->filelist[0].type |= S_IFDIR;
			memset( &(sfile->filelist[1]), 0 , sizeof(struct direntry));
			sfile->filelist[1].relname= strdup("..");
			sfile->filelist[1].type |= S_IFDIR;
		
			files+= 2;
		}
		
		id= lb->first;
		totlib= totbl= 0;
		
		while(id) {
			
			ok= 0;
			if(sfile->returnfunc && idcode==ID_IP) {
				if(sfile->ipotype== ((Ipo *)id)->blocktype) ok= 1;
			}
			else ok= 1;
			
			if(ok) {
		
				memset( files, 0 , sizeof(struct direntry));
				files->relname= strdup(id->name+2);
				
				if(sfile->returnfunc==0) { /* F4 DATA BROWSE */
					if(idcode==ID_OB) {
						if( ((Object *)id)->flag & SELECT) files->flags |= ACTIVE;
					}
					else if(idcode==ID_SCE) {
						if( ((Scene *)id)->r.scemode & R_BG_RENDER) files->flags |= ACTIVE;
					}
				}
				files->nr= totbl+1;
				files->poin= id;
				fake= id->flag & LIB_FAKEUSER;
				
				if(id->lib && fake) sprintf(files->extra, "LF %d", id->us);
				else if(id->lib) sprintf(files->extra, "L    %d", id->us);
				else if(fake) sprintf(files->extra, "F    %d", id->us);
				else sprintf(files->extra, "      %d", id->us);
				
				if(id->lib) {
					if(totlib==0) firstlib= files;
					totlib++;
				}
				
				files++;
				totbl++;
			}
			
			id= id->next;
		}
		
		/* alleen qsort van libraryblokken */
		if(totlib>1) {
			qsort(firstlib, totlib, sizeof(struct direntry), (void *)compare);
		}
	}

	sfile->maxnamelen= 0;
	for(a=0; a<sfile->totfile; a++) {
		len = fmgetstrwidth(G.font, sfile->filelist[a].relname);
		if (len > sfile->maxnamelen) sfile->maxnamelen = len;
		
		if(filetoname) {
			if( strcmp(sfile->file, sfile->filelist[a].relname)==0) {
				sfile->ofs= a-( sfile->collums*(curarea->winy-FILESELHEAD-10)/(2*FILESEL_DY));
				filetoname= 0;
				if(sfile->returnfunc) sfile->filelist[a].flags |= ACTIVE;
			}
		}
	}
}




