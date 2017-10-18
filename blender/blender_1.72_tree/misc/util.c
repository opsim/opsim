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

#include <stdio.h>

#include "util.h"
#include <fcntl.h>

#ifdef WIN32
#include "winstuff.h"
#endif

#ifndef WIN32
#include <sys/time.h>
#endif

int totblock=0;
int mem_in_use = 0;
int current_mem_level = 0;


struct ListBase _membase;
struct ListBase *membase = &_membase;

int stringdec(char *string, char *kop, char *staart, ushort *numlen)
{
	ushort len, len2, nums, nume;
	short i, found=FALSE;

	len2 = len =  strlen( string);
	
	if (len > 6) {
		if (strncasecmp(string + len - 6, ".blend", 6) == 0) len -= 6;
		else if (strncasecmp(string + len - 6, ".trace", 6) == 0) len -= 6;
	}
	
	if (len == len2) {
		if (len > 4) {
			/* .jf0 en .jf1 voor jstreams afvangen */
			if (strncasecmp(string + len - 4, ".jf", 3) == 0) len -= 4;
			else if (strncasecmp(string + len - 4, ".tga", 4) == 0) len -= 4;
			else if (strncasecmp(string + len - 4, ".jpg", 4) == 0) len -= 4;
			else if (strncasecmp(string + len - 4, ".txt", 4) == 0) len -= 4;
			else if (strncasecmp(string + len - 4, ".cyc", 4) == 0) len -= 4;
			else if (strncasecmp(string + len - 4, ".enh", 4) == 0) len -= 4;
			else if (strncasecmp(string + len - 4, ".rgb", 4) == 0) len -= 4;
			else if (strncasecmp(string + len - 4, ".psx", 4) == 0) len -= 4;
		}
	}
	
	for (i = len - 1; i >= 0; i--){
		if (string[i] == '/') break;
		if (isdigit(string[i])) {
			if (found){
				nums = i;
			}
			else{
				nume = i;
				nums = i;
				found = TRUE;
			}
		}
		else{
			if (found) break;
		}
	}
	if (found){
		if (staart) strcpy(staart,&string[nume+1]);
		if (kop) {
			strcpy(kop,string);
			kop[nums]=0;
		}
		if (numlen) *numlen = nume-nums+1;
		return ((int)atoi(&(string[nums])));
	}
	if (staart) strcpy(staart, string + len);
	if (kop) {
		strncpy(kop, string, len);
		kop[len] = 0;
	}
	if (numlen) *numlen=0;
	return 0;
}


void stringenc(char *string, char *kop, char *staart, ushort numlen, int pic)
{
	char numstr[10]="";
	ushort len,i;

	strcpy(string,kop);
	
	if (pic>0 || numlen==4) {
		len= sprintf(numstr,"%d",pic);

		for(i=len;i<numlen;i++){
			strcat(string,"0");
		}
		strcat(string,numstr);
	}
	strcat(string,staart);
}


void newname(char * name, int add)
{
	char head[128], tail[128];
	int pic;
	ushort digits;
	
	pic = stringdec(name, head, tail, &digits);
	
	/* gaan we van 100 -> 99 of van 10 naar 9 */
	if (add < 0 && digits < 4 && digits > 0) {
		int i, exp;
		exp = 1;
		for (i = digits; i > 1; i--) exp *= 10;
		if (pic >= exp && (pic + add) < exp) digits--;
	}
	
	pic += add;
	
	if(digits==4 && pic<0) pic= 0;
	stringenc(name, head, tail, digits, pic);
}


void addhead(ListBase *listbase, void *vlink)
{
	struct Link *link= vlink;

	if (link == 0) return;
	if (listbase == 0) return;

	link->next = listbase->first;
	link->prev = 0;

	if (listbase->first) ((struct Link *)listbase->first)->prev = link;
	if (listbase->last == 0) listbase->last = link;
	listbase->first = link;
}


void addtail(ListBase *listbase, void *vlink)
{
	struct Link *link= vlink;

	if (link == 0) return;
	if (listbase == 0) return;

	link->next = 0;
	link->prev = listbase->last;

	if (listbase->last) ((struct Link *)listbase->last)->next = link;
	if (listbase->first == 0) listbase->first = link;
	listbase->last = link;
}


void remlink(ListBase *listbase, void *vlink)
{
	struct Link *link= vlink;

	if (link == 0) return;
	if (listbase == 0) return;

	if (link->next) link->next->prev = link->prev;
	if (link->prev) link->prev->next = link->next;

	if (listbase->last == link) listbase->last = link->prev;
	if (listbase->first == link) listbase->first = link->next;
}


void freelinkN(ListBase *listbase, void *vlink)
{
	struct Link *link= vlink;

	if (link == 0) return;
	if (listbase == 0) return;

	remlink(listbase,link);
	freeN(link);
}


void insertlink(ListBase *listbase, void *vprevlink, void *vnewlink)
{
	struct Link *prevlink= vprevlink, *newlink= vnewlink;

	/* newlink komt na prevlink */

	if (newlink == 0) return;
	if (listbase == 0) return;

	if(listbase->first==0) { /* lege lijst */
		listbase->first= newlink;
		listbase->last= newlink;
		return;
	}
	if (prevlink== 0) {	/* inserten voor eerste element */
		newlink->next= listbase->first;
		newlink->prev= 0;
		newlink->next->prev= newlink;
		listbase->first= newlink;
		return;
	}

	if (listbase->last== prevlink) /* aan einde lijst */
		listbase->last = newlink;

	newlink->next= prevlink->next;
	prevlink->next= newlink;
	if(newlink->next) newlink->next->prev= newlink;
	newlink->prev= prevlink;
}

void insertlinkbefore(ListBase *listbase, void *vnextlink, void *vnewlink)
{
	struct Link *nextlink= vnextlink, *newlink= vnewlink;

	/* newlink komt voor nextlink */

	if (newlink == 0) return;
	if (listbase == 0) return;

	if(listbase->first==0) { /* lege lijst */
		listbase->first= newlink;
		listbase->last= newlink;
		return;
	}
	if (nextlink== 0) {	/* inserten aan einde lijst */
		newlink->prev= listbase->last;
		newlink->next= 0;
		((struct Link *)listbase->last)->next= newlink;
		listbase->last= newlink;
		return;
	}

	if (listbase->first== nextlink) /* aan begin lijst */
		listbase->first = newlink;

	newlink->next= nextlink;
	newlink->prev= nextlink->prev;
	nextlink->prev= newlink;
	if(newlink->prev) newlink->prev->next= newlink;
}


void freelist(ListBase *listbase)
{
	struct Link *link,*next;

	if (listbase == 0) return;
	link= listbase->first;
	while(link) {
		next= link->next;
		free(link);
		link= next;
	}
	listbase->first=0;
	listbase->last=0;
}

void freelistN(ListBase *listbase)
{
	struct Link *link,*next;

	if (listbase == 0) return;
	link= listbase->first;
	while(link) {
		next= link->next;
		freeN(link);
		link= next;
	}
	listbase->first=0;
	listbase->last=0;
}


int countlist(ListBase *listbase)
{
	Link * link;
	int count = 0;
	
	if (listbase){
		link = listbase->first;
		while(link) {
			count++;
			link= link->next;
		}
	}
	return(count);
}

#ifndef WIN32
short fileselect(char *title, char *dir)
{
	char buf[200];
	int fspipe[2],len,ok;

	if (pipe(fspipe)) return (0);

	/*sprintf(buf,"fs '%s' '%d' '%s'",dir,fspipe[1],title);*/
	sprintf(buf,"fs -d '%s' -p '%d' -t '%s'",dir,fspipe[1],title);

	len=ok=0;
	if (system(buf)==0){
		if (read(fspipe[0],&len,4)==4){
			if (read(fspipe[0],buf,len)==len){
				if (len>1){
					strcpy(dir,buf);
					ok=1;
				}
			}
		}
	}

	close(fspipe[0]);
	close(fspipe[1]);
	return (ok);
}


short stringselect(char *title, char *file)
{
	char buf[200];
	int fspipe[2],len,ok;

	if (pipe(fspipe)) return (0);

	sprintf(buf,"fs -f '%s' -s -p '%d' -t '%s'",file,fspipe[1],title);

	len=ok=0;
	if (system(buf)==0){
		if (read(fspipe[0],&len,4)==4){
			if (read(fspipe[0],buf,len)==len){
				if (len>1){
					strcpy(file,buf);
					ok=1;
				}
			}
		}
	}

	close(fspipe[0]);
	close(fspipe[1]);
	return (ok);
}
#endif


#ifdef malloc
#undef malloc
#endif

#ifdef calloc
#undef calloc
#endif

#ifdef free
#undef free
#endif


void MemorY_ErroR(char *block, char *error)
{
	fprintf(stderr,"Memoryblock %s: %s\n",block,error);
}

void (*memory_error)() = MemorY_ErroR;

void *mallocN(int len, char *str)
{
	MemHead *memh;
	MemTail *memt;

	if(len<=0) {
		printf("Malloc error: len=%d in %s\n",len,str);
		return 0;
	}

	if(sizeof(long)==8)
		len = (len + 3 ) & ~3; 	/* eenheden van 4 */
	else 
		len = (len + 7 ) & ~7; 	/* eenheden van 8 */
	
	memh= (MemHead *)malloc(len+sizeof(MemHead)+sizeof(MemTail));

	if(memh!=0) {
		memh->tag1 = MEMTAG1;
		memh->name = str;
		memh->nextname = 0;
		memh->len = len;
		memh->level = current_mem_level;
		memh->tag2 = MEMTAG2;

		memt = (MemTail *)(((char *) memh) + sizeof(MemHead) + len);
		memt->tag3 = MEMTAG3;

		addtail(membase,&memh->next);
		if (memh->next) memh->nextname = MEMNEXT(memh->next)->name;

		totblock++;
		mem_in_use += len;
		return (++memh);
	}
	printf("Malloc returns nill: len=%d in %s\n",len,str);
	return 0;
}

void *callocN(int len, char *str)
{
	MemHead *memh;
	MemTail *memt;

	if(len<=0) {
		printf("Malloc error: len=%d in %s\n",len,str);
		return 0;
	}

	if(sizeof(long)==8)
		len = (len + 3 ) & ~3; 	/* eenheden van 4 */
	else 
		len = (len + 7 ) & ~7; 	/* eenheden van 8 */

	memh= (MemHead *)calloc(len+sizeof(MemHead)+sizeof(MemTail),1);

	if(memh!=0) {
		memh->tag1 = MEMTAG1;
		memh->name = str;
		memh->nextname = 0;
		memh->len = len;
		memh->level = current_mem_level;
		memh->tag2 = MEMTAG2;

		memt = (MemTail *)(((char *) memh) + sizeof(MemHead) + len);
		memt->tag3 = MEMTAG3;

		addtail(membase,&memh->next);
		if (memh->next) memh->nextname = MEMNEXT(memh->next)->name;

		totblock++;
		mem_in_use += len;
		return (++memh);
	}
	printf("Calloc returns nill: len=%d in %s\n",len,str);
	return 0;
}


void rem_memblock(MemHead *memh)
{
	remlink(membase,&memh->next);
	if (memh->prev){
		if (memh->next) MEMNEXT(memh->prev)->nextname = MEMNEXT(memh->next)->name;
		else MEMNEXT(memh->prev)->nextname = 0;
	}

	totblock--;
	mem_in_use -= memh->len;
	free(memh);
}


void printmemlist()
{
	MemHead *membl;

	membl = membase->first;
	if (membl) membl = MEMNEXT(membl);
	while(membl) {
		printf("%s len: %d %x\n",membl->name,membl->len, membl+1);
		if(membl->next)
			membl= MEMNEXT(membl->next);
		else break;
	}
}

char *check_memlist(MemHead *memh)
{
	MemHead *forw,*back,*forwok,*backok;
	char *name;

	forw = membase->first;
	if (forw) forw = MEMNEXT(forw);
	forwok = 0;
	while(forw){
		if (forw->tag1 != MEMTAG1 || forw->tag2 != MEMTAG2) break;
		forwok = forw;
		if (forw->next) forw = MEMNEXT(forw->next);
		else forw = 0;
	}

	back = (MemHead *) membase->last;
	if (back) back = MEMNEXT(back);
	backok = 0;
	while(back){
		if (back->tag1 != MEMTAG1 || back->tag2 != MEMTAG2) break;
		backok = back;
		if (back->prev) back = MEMNEXT(back->prev);
		else back = 0;
	}

	if (forw != back) return ("MORE THAN 1 MEMORYBLOCK CORRUPT");

	if (forw == 0 && back == 0){
		/* geen foute headers gevonden dan maar op zoek naar memblock*/

		forw = membase->first;
		if (forw) forw = MEMNEXT(forw);
		forwok = 0;
		while(forw){
			if (forw == memh) break;
			if (forw->tag1 != MEMTAG1 || forw->tag2 != MEMTAG2) break;
			forwok = forw;
			if (forw->next) forw = MEMNEXT(forw->next);
			else forw = 0;
		}
		if (forw == 0) return (0);

		back = (MemHead *) membase->last;
		if (back) back = MEMNEXT(back);
		backok = 0;
		while(back){
			if (back == memh) break;
			if (back->tag1 != MEMTAG1 || back->tag2 != MEMTAG2) break;
			backok = back;
			if (back->prev) back = MEMNEXT(back->prev);
			else back = 0;
		}
	}

	if (forwok) name = forwok->nextname;
	else name = "No name found";

	if (forw == memh){
		/* voor alle zekerheid wordt dit block maar uit de lijst gehaald */
		if (forwok){
			if (backok){
				forwok->next = (MemHead *)&backok->next;
				backok->prev = (MemHead *)&forwok->next;
				forwok->nextname = backok->name;
			} else{
				forwok->next = 0;
				membase->last = (struct Link *) &forwok->next;
			}
		} else{
			if (backok){
				backok->prev = 0;
				membase->first = &backok->next;
			} else{
				membase->first = membase->last = 0;
			}
		}
	} else{
		memory_error(name,"Aditional error in header");
		return(0);
	}

	return(name);
}


short freeN(void *vmemh)		/* anders compileertie niet meer */
{
	short error = 0;
	MemTail *memt;
	MemHead *memh= vmemh;
	char *name;

	if (memh == 0){
		memory_error("free","attempt to free NULL pointer");
		return(-1);
	}

	if(sizeof(long)==8) {
		if (((long) memh) & 0x7) {
			memory_error("free","attempt to free illegal pointer");
			return(-1);
		}
	}
	else {
		if (((long) memh) & 0x3) {
			memory_error("free","attempt to free illegal pointer");
			return(-1);
		}
	}
	
	memh--;
	if(memh->tag1 == MEMFREE && memh->tag2 == MEMFREE) {
		memory_error(memh->name,"double free");
		return(-1);
	}

	if ((memh->tag1 == MEMTAG1) && (memh->tag2 == MEMTAG2) && ((memh->len & 0x3) == 0)) {
		memt = (MemTail *)(((char *) memh) + sizeof(MemHead) + memh->len);
		if (memt->tag3 == MEMTAG3){
			
			memh->tag1 = MEMFREE;
			memh->tag2 = MEMFREE;
			memt->tag3 = MEMFREE;
			/* na tags !!! */
			rem_memblock(memh);
			
			return(0);
		}
		error = 2;
		memory_error(memh->name,"end corrupt");
		name = check_memlist(memh);
		if (name != 0){
			if (name != memh->name) memory_error(name,"is also corrupt");
		}
	} else{
		error = -1;
		name = check_memlist(memh);
		if (name == 0) memory_error("free","pointer not in memlist");
		else memory_error(name,"error in header");
	}

	totblock--;
	/* hier moet een DUMP plaatsvinden */

	return(error);
}


void ap_framelen(char * cmd, int i)
{
	char buf[8];

	if (i<10) sprintf(buf,"0%d",i);
	else sprintf(buf,"%d",i);

	strcat(cmd,buf);
}


char *tcode_to_string(int len)
{
	int i;
	int rlen;
	static char str[64];

	if (len < 0){
		strcpy(str, "- ");
		len = -len;
	} else{
		str[0] = 0;
	}
	rlen = len;

	if (len >= 90000){
		i = rlen/90000;
		ap_framelen(str,i);
		strcat(str,":");
		rlen -= 90000 * i;
	}
	if (len >= 1500){
		i = rlen/1500;
		ap_framelen(str,i);
		strcat(str,":");
		rlen -= 1500 * i;
	}
	if (len >= 25){
		i = rlen/25;
		ap_framelen(str,i);
		strcat(str,":");
		rlen -= 25 * i;
	}
	ap_framelen(str,rlen);
	return(str);
}


int string_to_tcode(char * str)
{
	int i,val[4],tcode = 0,j=0;
	char string[64];

	strcpy(string,str);

	for (i = 0 ; string[i] ; i++){
		if (isdigit(string[i]) == 0) string[i] = ' ';
	}

	i = sscanf(string,"%ld%ld%ld%ld",val,val+1,val+2,val+3);

	switch(i){
	case 4:
		tcode += 90000 * val[j++];
	case 3:
		tcode += 1500 * val[j++];
	case 2:
		tcode += 25 * val[j++];
	case 1:
		tcode += val[j++];
	}

	return(tcode);
}


int strnmatch(char *s1, char *s2)
{
	int i = 0;
	char c1, c2;

	if (s1 == 0) return(0);
	if (s2 == 0) return(0);

	do{
		c1 = toupper(s1[i]);
		c2 = toupper(s2[i]);
		if (c1 != c2) break;
		i++;
	}while (c1 && c2);

	if (c1 == 0 && c2 == 0) return (32767);
	return(i);
}


int file_size(char * name)
{
	int file, size;

	if (name == 0) return(0);
	file = open(name, O_RDONLY);
	if (file == -1) return (0);

	size = filesize(file);
	close(file);
	return(size);
}



void * load_to_mem(char * name)
{
	int file, size;
	char * mem;

	if (name == 0) return(0);
	file = open(name, O_RDONLY);
	if (file == -1) return (0);

	size = lseek(file,0L,SEEK_END);
	if (size > 0) {
		lseek(file,0L,SEEK_SET);

		mem = mallocN(size, "load_to_mem");
		if (mem) {
			if (read(file, mem, size) != size){
				freeN(mem);
				mem = 0;
			}
		}
	}
	close(file);
	return(mem);
}

#ifndef WIN32
void Sginap(int Ticks)
{
	struct timeval timeout;
	
	timeout.tv_sec = Ticks/100;
	timeout.tv_usec = Ticks*10000 - timeout.tv_sec*1000000;
	select(0, 0, 0, 0, &timeout);
}
#endif

