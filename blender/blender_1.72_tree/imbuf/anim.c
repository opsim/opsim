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

#include <sys/types.h>


#ifndef WIN32
#include <dirent.h>
#endif

#include <errno.h>
#include <avi.h>

#ifdef __sgi
#include <movie.h>
#include <dmedia/cl.h>
#include <dmedia/cl_cosmo.h>
#else
#define DMboolean char
#define DMstatus char
#define DMmedium char
#define DMorientation char
#define MVframe char
#define DM_FALSE 0
#define DM_FAILURE 0
#define DM_TRUE 1
#define DM_SUCCESS 1
#define DM_IMAGE 0
#define DM_IMAGE_WIDTH 0
#define DM_IMAGE_HEIGHT 0
#define DM_IMAGE_INTERLACING 0
#define DM_IMAGE_ORIENTATION 0
#define DM_IMAGE_INTERLACED_EVEN 0
#endif

#include "imbuf.h"

#define OBJECTBLOK "anim"


/* for BeOS: mmap functions don't exist */

#if defined(__BeOS) || defined(WIN32)

#define PROT_READ	1
#define MAP_SHARED	1

void *my_mmap(void *addr, int len, int prot, int flags, int file, int offs) {
	void *data= (void *)(-1);

	if(len && file>=0) {
		/* rewind */
		lseek(file, 0L, SEEK_SET);

		data= mallocN(len, "mmap patch");
		read(file, data, len);
	}
	return data;
}

void my_munmap(void *addr, int len) {
	freeN(addr);
}

#endif

/* movie stubs */

DMboolean my_IsMovie(const char * name) {
	/*	static int first = TRUE;
	
	if (first) printf("IsMovie: not initialised\n");
	first = FALSE;*/
	return(DM_FALSE);
}

DMstatus my_OpenMovie(const char * name, int oflag, MVid * movie ) {
	static int first = TRUE;

	if (first) printf("OpenMovie: not initialised\n");
	first = FALSE;
	return(DM_FAILURE);
}

DMstatus my_CloseMovie (MVid movie ) {
	static int first = TRUE;

	if (first) printf("CloseMovie: not initialised\n");
	first = FALSE;
	return(DM_FAILURE);
}

DMstatus My_ReadFrames (MVid track, MVframe frameIndex, MVframe frameCount, size_t bufferSize, void* buffer ) {
	static int first = TRUE;

	if (first) printf("ReadFrames: not initialised\n");
	first = FALSE;
	return (DM_FAILURE);
}

DMstatus My_FindTrack(MVid movie, DMmedium medium, MVid* returnTrack) {
	static int first = TRUE;

	if (first) printf("FindTrack: not initialised\n");
	first = FALSE;
	return (DM_FAILURE);
}

DMparams* my_GetParams( MVid movieOrTrack ) {
	static int first = TRUE;

	if (first) printf("GetParams: not initialised\n");
	first = FALSE;
	return (NULL);
}

size_t my_ImageFrameSize(const DMparams* params ) {
	static int first = TRUE;

	if (first) printf("ImageFrameSize: not initialised\n");
	first = FALSE;
	return (0);
}

int my_ParamsGetInt( const DMparams* params, const char* paramName ) {
	static int first = TRUE;

	if (first) printf("ParamsGetInt: not initialised\n");
	first = FALSE;
	return(0);
}

double my_ParamsGetFloat( const DMparams* params, const char* paramName ) {
	static int first = TRUE;

	if (first) printf("ParamsGetFloat: not initialised\n");
	first = FALSE;
	return(0.0);
}

MVframe my_GetTrackLength( MVid track ) {
	static int first = TRUE;

	if (first) printf("GetTrackLength: not initialised\n");
	first = FALSE;
	return (0);
}

int my_GetErrno() {
	static int first = TRUE;

	if (first) printf("my_GetErrno: not initialised\n");
	first = FALSE;
	return (0);
}

const char* my_GetErrorStr(int error) {
	static int first = TRUE;

	if (first) printf("my_GetErrorStr: not initialised\n");
	first = FALSE;
	return (0);
}

/* movie pointers */

/* mvIsMovieFile */
DMboolean (*IsMovie)(const char * name) = my_IsMovie;
/* mvOpenFile */
DMstatus  (* OpenMovie)(const char * name, 
int oflag, MVid * movie ) = my_OpenMovie;
/* mvClose */
DMstatus  (* CloseMovie)( MVid movie ) = my_CloseMovie;
/* mvReadFrames */
DMstatus  (* ReadFrames)( MVid track, MVframe frameIndex, 
MVframe frameCount, size_t bufferSize, 
void* buffer ) = My_ReadFrames;
/* mvFindTrackByMedium */
DMstatus  (* FindTrack)( MVid movie, DMmedium medium, 
MVid* returnTrack ) = My_FindTrack;
/* mvGetParams */
DMparams* (* GetParams)( MVid movieOrTrack ) = my_GetParams;
/* mvGetErrno */
int (* GetErrno)() = my_GetErrno;
/* mvGetErrorStr */
const char * (* GetErrorStr)(int) = my_GetErrorStr;

/* dmImageFrameSize */
size_t (* ImageFrameSize)(const DMparams* params ) = my_ImageFrameSize;
/* dmParamsGetInt */
int (* ParamsGetInt)( const DMparams* params, 
const char* paramName ) = my_ParamsGetInt;
/* dmParamsGetEnum */
int (* ParamsGetEnum)( const DMparams* params, 
const char* paramName ) = my_ParamsGetInt;
/* dmParamsGetFloat */
double (* ParamsGetFloat)( const DMparams* params, 
const char* paramName ) = my_ParamsGetFloat;
MVframe (* GetTrackLength) ( MVid track ) = my_GetTrackLength;


movie_printerror(char * str) {
	const char * errstr = GetErrorStr(GetErrno());

	if (str) {
		if (errstr) printf("%s: %s\n", str, errstr);
		else printf("%s: returned error\n", str);
	} else printf("%s\n", errstr);
}


int an_stringdec(char *string, char* kop, char *staart,ushort *numlen) {
	ushort len,nums,nume;
	short i,found=FALSE;

	len=strlen(string);

	for(i=len-1;i>=0;i--){
		if (string[i]=='/') break;
		if (isdigit(string[i])) {
			if (found){
				nums=i;
			} else{
				nume=i;
				nums=i;
				found=TRUE;
			}
		} else{
			if (found) break;
		}
	}
	if (found){
		strcpy(staart,&string[nume+1]);
		strcpy(kop,string);
		kop[nums]=0;
		*numlen=nume-nums+1;
		return ((int)atoi(&(string[nums])));
	}
	staart[0]=0;
	strcpy(kop,string);
	*numlen=0;
	return (1);
}


void an_stringenc(char *string, char *kop, char *staart, 
ushort numlen, int pic) {
	char numstr[10];
	ushort len,i;

	len=sprintf(numstr,"%d",pic);

	strcpy(string,kop);
	for(i=len;i<numlen;i++){
		strcat(string,"0");
	}
	strcat(string,numstr);
	strcat(string,staart);
}

/*
  zipfork "cc -g ../animtest.c ../util.o -lgl_s -limbuf -limage -lm -o ../animtest >/dev/console"                                                                           
  zipfork "animtest /mo/pics/martin/Custan1B.ani >/dev/console"                                       
*/


/* om anim5's te kunnen lezen, moet een aantal gegevens bijgehouden worden:
 * Een lijst van pointers naar delta's, in geheugen of ge'mmap'ed
 * 
 * Mogelijk kan er ook een 'skiptab' aangelegd worden, om sneller
 * sprongen te kunnen maken.
 * 
 * Er moeten niet direct al plaatjes gegenereed worden, dit maakt de 
 * routines onbruikbaar om snel naar het goede plaatje te springen.
 * Een routine voert dus de delta's uit, een andere routine maakt van
 * voorgrondplaatje een ibuf;
 */


/*
   een aantal functie pointers moet geinporteerd worden, zodat er niet
   nog meer library's / objects meegelinkt hoeven te worden.

   Dezelfde structuur moet ook gebruikt kunnen worden voor het wegschrijven
   van animaties. Hoe geef je dit aan ?
   
   Hoe snel kunnen 10 .dlta's gedecomprimeerd worden
   (zonder omzetten naar rect).
   
   1 - zoek naar 1e plaatje, animatie die aan de eisen voldoet
   2 - probeer volgende plaatje te vinden:
		anim5 - decomprimeer
		sequence - teller ophogen
		directory - volgende entry
   3 - geen succes ? ga naar 1.
   
   
*/

/*
	1. Initialiseer routine met toegestane reeksen, en eerste naam
		- series op naam (.0001)
		- directories
		- anim5 animaties
		- TGA delta's
		- iff 24bits delta's (.delta)
	
	2. haal volgende (vorige ?) plaatje op.
	
	3. vrijgeven
*/

/* selectie volgorde is:
	1 - anim5()
	2 - name
	3 - dir
*/


void free_anim_anim5(struct anim * anim) {
	int i;
	ListBase * animbase;
	Anim5Delta * delta, * next;

	if (anim == NULL) return;

	animbase = &anim->anim5base;
	delta = animbase->first;

	while (delta) {
		next = delta->next;

		if (delta->type == ANIM5_MALLOC) free(delta->data);
		remlink(animbase, delta);
		free(delta);

		delta = next;
	}

	if (anim->anim5mmap && anim->anim5len)
		munmap( (void *)anim->anim5mmap, anim->anim5len);

	anim->anim5mmap = NULL;
	anim->anim5len = 0;
	anim->anim5curdlta = 0;
	anim->duration = 0;
}

void free_anim_movie(struct anim * anim) {
	if (anim == NULL) return;

	if (anim->movie) {
		CloseMovie(anim->movie);
		anim->movie = NULL;
	}
	anim->duration = 0;
}

void free_anim_avi (struct anim *anim) {
	if (anim == NULL) return;
	if (anim->avi == NULL) return;

	avi_close (anim->avi);
	freeN (anim->avi);
	anim->avi = NULL;

	anim->duration = 0;
}

void free_anim_mdec(struct anim * anim) {
	if (anim == NULL) return;

	if (anim->mdec && anim->mdeclen) {
		munmap( (void *)anim->mdec, anim->mdeclen);
		anim->mdec = NULL;
		anim->mdeclen = 0;
	}
	anim->duration = 0;
}


void free_anim_ibuf(struct anim * anim) {
	int i;

	if (anim == NULL) return;

	if (anim->ibuf1) freeImBuf(anim->ibuf1);
	if (anim->ibuf2) freeImBuf(anim->ibuf2);

	anim->ibuf1 = anim->ibuf2 = NULL;
}


void free_anim(struct anim * anim) {
	int i;

	if (anim == NULL) {
		printf("free anim, anim == NULL\n");
		return;
	}

	free_anim_ibuf(anim);
	free_anim_anim5(anim);
	free_anim_movie(anim);
	free_anim_avi(anim);
	free_anim_mdec(anim);

	free(anim);
}

void close_anim(struct anim * anim) {
	int i;

	if (anim == 0) return;

	free_anim(anim);
}


struct anim * open_anim(char * name, int flags, int ib_flags) {
	struct anim * anim;
	char * dir;
	struct stat st;
	int i;

	anim = callocstructN(struct anim, 1, "anim struct");
	if (anim != NULL) {
		strcpy(anim->name, name);
		anim->flags = flags;
		anim->ib_flags = ib_flags;
	}
	return(anim);
}


int ismpg(char *name) {
	int fp, buf[10];
	int ret = FALSE;

	if ((fp = open(name, O_BINARY|O_RDONLY)) >= 0){
		if (read(fp ,buf, 4) == 4){
			if ((buf[0] == 0x000001b3) | (buf[0] == 0x000001ba)) ret = TRUE;
			/*    printf("mpeg afgevangen\n");*/
		}
		close(fp);
	}

	return(ret);
}


int isavi (char *name) {
    return avi_is_avi (name);
}

int ismdec(char *name) {
	int fp, buf[10];
	int ret = FALSE;

	if ((fp = open(name, O_BINARY|O_RDONLY)) >= 0){
		if (read(fp ,buf, 4) == 4){
			if ((buf[0] == 0x564D0100)) ret = TRUE;
			/*printf("mdec afgevangen\n");*/
		}
		close(fp);
	}

	return(ret);
}


int isanim(char * name) {
	int type;
	struct stat st;

	if (ib_stat(name,&st) == -1) return(0);
	if (((st.st_mode) & S_IFMT) != S_IFREG) return(0);

	if (isavi(name)) return (ANIM_AVI);

	if (IsMovie(name) == DM_TRUE) return(ANIM_MOVIE);
	/* movie library is beschikbaar: MPEG afvangen */

	if (ismdec(name)) return(ANIM_MDEC);

	type = ispic(name);
	if (type == ANIM) return (ANIM_ANIM5);
	if (type) return(ANIM_SEQUENCE);
	return(0);
}


void planes_to_rect(struct ImBuf * ibuf, int flags) {
	if (ibuf == 0) return;

	/* dit komt regelrecht uit de amiga.c */

	if (flags & IB_rect && ibuf->rect == 0) {
		addrectImBuf(ibuf);
		bptolong(ibuf);
		flipy(ibuf);
		freeplanesImBuf(ibuf);

		if (ibuf->cmap){
			if ((flags & IB_cmap) == 0) {
				applycmap(ibuf);
				convert_rgba_to_abgr(ibuf->x*ibuf->y, ibuf->rect);
			}
		} else if (ibuf->depth == 18){
			int i,col;
			uint *rect;

			rect = ibuf->rect;
			for(i=ibuf->x * ibuf->y ; i>0 ; i--){
				col = *rect;
				col = ((col & 0x3f000) << 6) + ((col & 0xfc0) << 4)
				    + ((col & 0x3f) << 2);
				col += (col & 0xc0c0c0) >> 6;
				*rect++ = col;
			}
			ibuf->depth = 24;
		} else if (ibuf->depth <= 8) {
			/* geen colormap en geen 24 bits: zwartwit */
			uchar *rect;
			int size, shift;

			if (ibuf->depth < 8){
				rect = (uchar *) ibuf->rect;
				rect += 3;
				shift = 8 - ibuf->depth;
				for (size = ibuf->x * ibuf->y; size > 0; size --){
					rect[0] <<= shift;
					rect += 4;
				}
			}
			rect = (uchar *) ibuf->rect;
			for (size = ibuf->x * ibuf->y; size > 0; size --){
				rect[1] = rect[2] = rect[3];
				rect += 4;
			}
			ibuf->depth = 8;
		}
	}
}


void anim5decode(struct ImBuf * ibuf, uchar * dlta) {
	uchar depth;
	int skip;
	int *ofspoint;
	uchar **planes;

	/*	samenstelling delta:
		lijst met ofsets voor delta's per bitplane (ofspoint)
		per kolom in delta (point)
			aantal handelingen (noops)
				code
					bijbehorende data
				...
			...
	*/

	dlta += 8;

	ofspoint = (int *)dlta;
	skip = ibuf->skipx * sizeof(int *);
	planes = (uchar **)ibuf->planes;

	for(depth=ibuf->depth ; depth>0 ; depth--){
		if (GET_BIG_LONG(ofspoint)){
			uchar *planestart;
			uchar *point;
			uchar x;

			point = dlta + GET_BIG_LONG(ofspoint);
			planestart = planes[0];
			x = (ibuf->x + 7) >> 3;

			do{
				uchar noop;

				if (noop = *(point++)){
					uchar *plane;
					uchar code;

					plane = planestart;
					do{
						if ((code = *(point++))==0){
							uchar val;

							code = *(point++);
							val = *(point++);
							do {
								plane[0] = val;
								plane += skip;
							} while(--code);

						} else if (code & 128){

							code &= 0x7f;
							do{
								plane[0] = *(point++);
								plane += skip;
							} while(--code);

						} else plane += code * skip;

					} while(--noop);
				}
				planestart++;
			} while(--x);
		}
		ofspoint++;
		planes++;
	}
}


void anim5xordecode(struct ImBuf * ibuf, uchar * dlta) {
	uchar depth;
	int skip;
	int *ofspoint;
	uchar **planes;

	/*	samenstelling delta:
		lijst met ofsets voor delta's per bitplane (ofspoint)
		per kolom in delta (point)
			aantal handelingen (noops)
				code
					bijbehorende data
				...
			...
	*/

	dlta += 8;

	ofspoint = (int *)dlta;
	skip = ibuf->skipx * sizeof(int *);
	planes = (uchar **)ibuf->planes;

	for(depth=ibuf->depth ; depth>0 ; depth--){

		if (GET_BIG_LONG(ofspoint)){
			uchar *planestart;
			uchar *point;
			uchar x;

			point = dlta + GET_BIG_LONG(ofspoint);
			planestart = planes[0];
			x = (ibuf->x + 7) >> 3;

			do{
				uchar noop;

				if (noop = *(point++)){
					uchar *plane;
					uchar code;

					plane = planestart;
					do{
						if ((code = *(point++))==0){
							uchar val;

							code = *(point++);
							val = *(point++);
							do{
								plane[0] ^= val;
								plane += skip;
							}while(--code);

						} else if (code & 128){

							code &= 0x7f;
							do{
								plane[0] ^= *(point++);
								plane += skip;
							}while(--code);

						} else plane += code * skip;

					}while(--noop);
				}
				planestart++;
			}while(--x);
		}
		ofspoint++;
		planes++;
	}
}

int nextanim5(struct anim * anim) {
	ListBase * animbase;
	Anim5Delta * delta;
	struct ImBuf * ibuf;

	errno = EINVAL;
	if (anim == 0) return(-1);
	errno = 0;

	animbase = & anim->anim5base;
	delta = anim->anim5curdlta;

	if (delta == 0) return (-1);

	if (anim->anim5flags & ANIM5_SNGBUF) {
		ibuf = anim->ibuf1;
		if (ibuf == 0) return (0);
		anim->anim5decode(ibuf, delta->data);
	} else {
		ibuf = anim->ibuf2;
		if (ibuf == 0) return (0);
		anim->anim5decode(ibuf, delta->data);
		anim->ibuf2 = anim->ibuf1;
		anim->ibuf1 = ibuf;
	}

	anim->anim5curdlta = anim->anim5curdlta->next;
	anim->curposition++;

	return(0);
}

int rewindanim5(struct anim * anim) {
	Anim5Delta * delta;
	struct ImBuf * ibuf;

	if (anim == 0) return (-1);

	free_anim_ibuf(anim);

	delta = anim->anim5base.first;
	if (delta == 0) return (-1);

	ibuf = loadiffmem(delta->data, IB_planes);
	if (ibuf == 0) return(-1);

	anim->ibuf1 = ibuf;
	if ((anim->anim5flags & ANIM5_SNGBUF) == 0) anim->ibuf2 = dupImBuf(ibuf);

	anim->anim5curdlta = delta->next;
	anim->curposition = 0;

	return(0);
}


int startanim5(struct anim * anim) {
	int file, buf[20], type, totlen;
	uint len;
	short * mem;
	ListBase * animbase;
	Anim5Delta * delta;
	Anhd anhd;

	/* Controles */

	errno = EINVAL;
	if (anim == 0) return(-1);
	errno = 0;

	file = open(anim->name,O_BINARY|O_RDONLY);
	if (file < 0) return (-1);

	if (read(file, buf, 24) != 24) {
		close(file);
		return(-1);
	}

	if ((GET_ID(buf) != FORM) || (GET_ID(buf + 2) != ANIM)
	    || (GET_ID(buf + 3) != FORM) || (GET_ID(buf + 5) != ILBM)){
		printf("No anim5 file %s\n",anim->name);
		close(file);
		return (-1);
	}

	/* de hele file wordt in het geheugen gemapped */

	totlen = filesize(file);

	mem = (short *)mmap( (void *)0, totlen, PROT_READ, MAP_SHARED, file, 0);
	close (file);

	if (mem == (short *) -1) return (-1);

	anhd.interleave = 0;
	anhd.bits = 0;
	anhd.type = 5;

	anim->anim5mmap = mem;
	anim->anim5len = totlen;
	anim->anim5flags = 0;
	anim->duration = 0;

	animbase = & anim->anim5base;
	animbase->first = animbase->last = 0;

	/* eerste plaatje inlezen */

	mem = mem + 6;
	totlen -= 12;

	len = GET_BIG_LONG(mem + 2);
	len = (len + 8 + 1) & ~1;

	delta = NEW(Anim5Delta);

	delta->data = mem;
	delta->type = ANIM5_MMAP;

	addtail(animbase, delta);

	mem += (len >> 1);
	totlen -= len;

	while (totlen > 0) {
		len = GET_BIG_LONG(mem + 2);
		len = (len + 8 + 1) & ~1;

		switch(GET_ID(mem)){
		case FORM:
			len = 12;
			break;
		case ANHD:
			memcpy(&anhd, mem + 4, sizeof(Anhd));
			break;
		case DLTA:
			delta = NEW(Anim5Delta);
			delta->data = mem;
			delta->type = ANIM5_MMAP;
			addtail(animbase, delta);
			break;
		}

		mem += (len >> 1);
		totlen -= len;
	}

	if (anhd.interleave == 1) anim->anim5flags |= ANIM5_SNGBUF;
	if (BIG_SHORT(anhd.bits) & 2) anim->anim5decode = anim5xordecode;
	else anim->anim5decode = anim5decode;

	/* laatste twee delta's wissen */

	delta = animbase->last;
	if (delta) {
		remlink(animbase, delta);
		free(delta);
	}

	if ((anim->anim5flags & ANIM5_SNGBUF) == 0) {
		delta = animbase->last;
		if (delta) {
			remlink(animbase, delta);
			free(delta);
		}
	}

	anim->duration = countlist(animbase);

	return(rewindanim5(anim));
}


struct ImBuf * anim5_fetchibuf(struct anim * anim) {
	struct ImBuf * ibuf;

	if (anim == 0) return (0);

	ibuf = dupImBuf(anim->ibuf1);
	planes_to_rect(ibuf, anim->ib_flags);

	return(ibuf);
}


int estimate_duration(struct anim * anim) {
	char head[256], tail[256], name[256];
	int pic;
	ushort digits;
	/*ListBase _list = {0, 0}, *list = &_list;*/

	pic = stringdec(anim->name, head, tail, &digits);
	if (digits == 0) return(1);

	/* hier moet een listing van een directory opgevraagd worden
   * daarna moeten namen vergleken worden
   */

	return(1);
}

int startmovie(struct anim * anim) {
	errno = EINVAL;
	if (anim == 0) return(-1);
	errno = 0;

	if ( OpenMovie (anim->name, O_BINARY|O_RDONLY, &anim->movie ) != DM_SUCCESS ) {
		printf("Can't open movie: %s\n", anim->name);
		return(-1);
	}
	if ( FindTrack (anim->movie, DM_IMAGE, &anim->track) != DM_SUCCESS ) {
		printf("No image track in movie: %s\n", anim->name);
		CloseMovie(anim->movie);
		return(-1);
	}

	anim->duration = GetTrackLength (anim->track);
	anim->params = GetParams( anim->track );

	anim->x = ParamsGetInt( anim->params, DM_IMAGE_WIDTH);
	anim->y = ParamsGetInt( anim->params, DM_IMAGE_HEIGHT);
	anim->interlacing = ParamsGetEnum (anim->params, DM_IMAGE_INTERLACING);
	anim->orientation = ParamsGetEnum (anim->params, DM_IMAGE_ORIENTATION);
	anim->framesize = ImageFrameSize(anim->params);

	anim->curposition = 0;

	/*printf("x:%d y:%d size:%d interl:%d dur:%d\n", anim->x, anim->y, anim->framesize, anim->interlacing, anim->duration);*/
	return (0);
}

int startavi (struct anim *anim) {
	int i=0;

	anim->avi = mallocN (sizeof(AviMovie),"animavi");

	if (anim->avi == NULL) {
		printf("Can't open avi: %s\n", anim->name);
		return -1;
	}

	if (avi_print_error(avi_open_movie (anim->name, anim->avi))) {
		printf ("Error loading avi: %s\n", anim->name);		
		avi_close (anim->avi);
		freeN (anim->avi);
		anim->avi = NULL;

		anim->duration = 0;
		
		return -1;
	}

	anim->duration = anim->avi->header->TotalFrames;
	anim->params = 0;

	anim->x = anim->avi->header->Width;
	anim->y = anim->avi->header->Height;
	anim->interlacing = 0;
	anim->orientation = 0;
	anim->framesize = anim->x * anim->y * 4;

	anim->curposition = 0;

	/*  printf("x:%d y:%d size:%d interl:%d dur:%d\n", anim->x, anim->y, anim->framesize, anim->interlacing, anim->duration);*/

	return 0;
}

int startmdec(struct anim * anim) {
	int file = -1, size, error = FALSE, flags = 0;
	Mdec * mdec;

	file = open(anim->name, O_BINARY|O_RDONLY);
	if (file != -1) {
		size = filesize(file);
		mdec = (Mdec *)mmap( (void *)0, size, PROT_READ, MAP_SHARED, file, 0);
		close (file);

		if (mdec == (Mdec *) -1) return (-1);

		if (SWAP_S(mdec->magic) == 'MV' && SWAP_S(mdec->version) <= 1) {
			anim->mdec = mdec;
			anim->mdeclen = size;

			anim->x = SWAP_S(mdec->sizex);
			anim->y = SWAP_S(mdec->sizey);
			anim->duration = SWAP_S(mdec->frames);
			/*printf("x:%d y:%d dur:%d\n", anim->x, anim->y, anim->duration);*/

			return(0);
		} else {
			printf("Unsupported mdec\n");
			munmap( (void *)mdec, size);
		}
	} else {
		perror(anim->name);
	}
	return (-1);
}


ImBuf * movie_fetchibuf(struct anim * anim, int position) {
	ImBuf * ibuf;
	extern rectcpy();
	int size;
	uint *rect1, *rect2;

	if (anim == 0) return (0);

	ibuf = allocImBuf(anim->x, anim->y, 24, IB_rect, 0);

	if ( ReadFrames(anim->track, position, 1, ibuf->x * ibuf->y * 
	    sizeof(int), ibuf->rect ) != DM_SUCCESS ) {
		movie_printerror("ReadFrames");
		freeImBuf(ibuf);
		return(0);
	}

	/*
    if (anim->interlacing == DM_IMAGE_INTERLACED_EVEN)
    {
    rect1 = ibuf->rect + (ibuf->x * ibuf->y) - 1;
    rect2 = rect1 - ibuf->x;
    
    for (size = ibuf->x * (ibuf->y - 1); size > 0; size--){
    *rect1-- = *rect2--;
    }
    }
    */

	if (anim->interlacing == DM_IMAGE_INTERLACED_EVEN)
	{
		rect1 = ibuf->rect;
		rect2 = rect1 + ibuf->x;

		for (size = ibuf->x * (ibuf->y - 1); size > 0; size--){
			*rect1++ = *rect2++;
		}
	}
	/*if (anim->orientation == DM_TOP_TO_BOTTOM) flipy(ibuf);*/


	return(ibuf);
}

ImBuf * avi_fetchibuf (struct anim *anim, int position) {
	ImBuf *ibuf;
	int *tmp, *ibufcp;
	int size,y;
	
	if (anim == NULL) return (NULL);

	ibuf = allocImBuf (anim->x, anim->y, 24, IB_rect, 0);

	tmp = avi_read_frame (anim->avi, AVI_FORMAT_RGB32, position,
	    avi_get_stream(anim->avi, AVIST_VIDEO, 0));
	
	if (tmp == NULL) {
		printf ("Error reading frame from AVI");
		freeImBuf (ibuf);
		return NULL;
	}

	for (y=0; y < anim->y; y++) {
		memcpy (&(ibuf->rect)[((anim->y-y)-1)*anim->x],  &tmp[y*anim->x],  
				anim->x * 4);
	}
	
	freeN (tmp);

	return ibuf;
}

ImBuf * mdec_fetchibuf(struct anim * anim, int position) {
	ImBuf * ibuf, * tbuf;
	int framesize, size, start, i, x, offset;
	uint * data, mdectype, mdecsize;
	uchar * mdecdata =  0;
	extern float mdec_ytab[64], mdec_ctab[64];
	SliceData slicedata;
	int * sld, * slicedatap;
	extern rectcpy();

	if (anim == 0) return (0);
	if (anim->mdec == 0) return(0);

	if (position >= anim->duration) return (0);

	start = SWAP_L(anim->mdec->offset[position]);
	framesize = SWAP_L(anim->mdec->offset[position + 1]) - start;

	data = (uint *) ((uchar *) anim->mdec + start);

	while (framesize > 0) {
		size = SWAP_L(data[1]);

		switch (data[0]) {
		case 'MDEC':
			mdecdata = (uchar *) data;
			mdectype = 'MDEC';
			mdecsize = size;
			break;
		case 'SLIC':
			mdecdata = (uchar *) data;
			mdectype = 'SLIC';
			mdecsize = size;
			break;

		case 'INFO':
		case 'CAMI':
		case 'VIEW':
		case 'SSND':
			break;
		default:
			printf("skipping %c%c%c%c size %d\n", data[0] >> 24, 
			    data[0] >> 16, data[0] >> 8, data[0] >> 0, size);
		}

		if (mdecdata) break;

		size = (size + 3) & ~3;
		framesize -= size + 8;
		data += (size + 8) >> 2;
	}

	if (mdecdata == 0) return(0);

	ibuf = allocImBuf(anim->x, anim->y, 24, IB_rect, 0);

	/* matrices goedzetten */

	for (i = 0; i < 64 ; i++) {
		mdec_ytab[i] = anim->mdec->ytab[i];
	}
	for (i = 0; i < 64 ; i++) {
		mdec_ctab[i] = anim->mdec->ctab[i];
	}

	if (mdectype == 'MDEC') {
		huffdecompress(ibuf, mdecdata + 16, mdecsize - 8, mdecdata[12]);
	} else if (mdectype == 'SLIC') {
		tbuf = allocImBuf(16, anim->y, 24, IB_rect, 0);
		sld = (int *) (mdecdata + 8);

		slicedatap = (int *) &slicedata;
		offset = 8;
		offset += 4 * (anim->x / 16);

		for (x = 0; x < anim->x; x += 16) {
			*slicedatap = SWAP_L(*sld);
			sld++;
			/*printf("\n\nslice %d offset %d size %d\n",  x / 16, offset, 4 * slicedata.hufsize);*/
			huffdecompress(tbuf, mdecdata + offset, 
			    4 * slicedata.hufsize, slicedata.quant);
			offset += 4 * slicedata.hufsize;
			rectop(ibuf, tbuf, x, 0, 0, 0, 32767, 32767, rectcpy, 0);
		}
		freeImBuf(tbuf);
		/*printf("end\n\n\n\n\n\n\n");*/
	}

	cspace(ibuf, yuvrgb);
	flipy(ibuf);

	return(ibuf);
}


/* probeer volgende plaatje te lezen */
/* Geen plaatje, probeer dan volgende animatie te openen */
/* gelukt, haal dan eerste plaatje van animatie */

struct ImBuf * anim_getnew(struct anim * anim) {
	struct ImBuf *ibuf = 0;
	int newtype = 0;

	if (anim == NULL) return(0);

	free_anim_anim5(anim);
	free_anim_movie(anim);
	free_anim_avi(anim);
	free_anim_mdec(anim);

	if (anim->curtype != 0 && anim->flags & ANIM_ONCE) return (0);
	newtype = isanim(anim->name);

	/*  if ((newtype & anim->flags) == 0) return (0);*/
	anim->curtype = newtype;

	switch (newtype) {
	case ANIM_ANIM5:
		if (startanim5(anim)) return (0);
		ibuf = anim5_fetchibuf(anim);
		break;
	case ANIM_SEQUENCE:
		ibuf = loadiffname(anim->name, anim->ib_flags);
		if (ibuf) {
			strcpy(anim->first, anim->name);
			anim->duration = estimate_duration(anim);
		}
		break;
	case ANIM_MOVIE:
		if (startmovie(anim)) return (0);
		ibuf = allocImBuf (anim->x, anim->y, 24, 0, 0); /* fake */
		break;
	case ANIM_AVI:
		if (startavi(anim)) return (0);
		ibuf = allocImBuf (anim->x, anim->y, 24, 0, 0);
		break;
	case ANIM_MDEC:
		if (startmdec(anim)) return (0);
		ibuf = allocImBuf (anim->x, anim->y, 24, 0, 0); /* fake */
		break;
	}

	return(ibuf);
}


struct ImBuf * anim_absolute(struct anim * anim, int position) {
	struct ImBuf * ibuf = 0;
	char head[256], tail[256], prevname[256];
	ushort digits;
	int pic, pingpong;

	if (anim == NULL) return(0);

	if (anim->curtype == 0)	{
		ibuf = anim_getnew(anim);
		if (ibuf == NULL) return (0);
		freeImBuf(ibuf); /* ???? */
	}

	switch (anim->playmode) {
	case PLAY_ONCE:
		if (position < 0) position = 0;
		else if (position >= anim->duration) position = anim->duration - 1;
		break;
	case PLAY_LOOP:
		while (position < 0) position += anim->duration;
		while (position >= anim->duration) position -= anim->duration;
		break;
	case PLAY_PINGPONG:
		pingpong = (2 * anim->duration) - 2;
		while (position < 0) position += pingpong;
		while (position > pingpong) position -= pingpong;
		if (position >= anim->duration) position = pingpong - position;
		break;
	case PLAY_INTERVAL:
		if (position < 0) return(0);
		if (position >= anim->duration) return(0);
		break;
	}

	switch(anim->curtype) {
	case ANIM_ANIM5:
		if (anim->curposition > position) rewindanim5(anim);
		while (anim->curposition < position) {
			if (nextanim5(anim)) return (0);
		}
		ibuf = anim5_fetchibuf(anim);
		break;
	case ANIM_SEQUENCE:
		pic = an_stringdec(anim->first, head, tail, &digits);
		pic += position;
		an_stringenc(anim->name, head, tail, digits, pic);
		ibuf = loadiffname(anim->name, LI_rect);
		if (ibuf) {
			anim->curposition = position;
			/* patch... by freeing the cmap you prevent a double apply cmap... */
			/* probably the IB_CMAP option isn't working proper
			 * after the abgr->rgba reconstruction
			 */
			freecmapImBuf(ibuf);
		}
		break;
	case ANIM_MOVIE:
		ibuf = movie_fetchibuf(anim, position);
		if (ibuf) {
			anim->curposition = position;
			convert_rgba_to_abgr(ibuf->x*ibuf->y, ibuf->rect);
		}
		break;
	case ANIM_AVI:
		ibuf = avi_fetchibuf(anim, position);
		if (ibuf) anim->curposition = position;
		break;
	case ANIM_MDEC:
		ibuf = mdec_fetchibuf(anim, position);
		if (ibuf) anim->curposition = position;
		break;
	}

	if (ibuf) {
		if (anim->ib_flags & IB_ttob) flipy(ibuf);
		sprintf(ibuf->name, "%s.%04d", anim->name, anim->curposition + 1);
		
	}
	return(ibuf);
}


struct ImBuf * anim_getnext(struct anim * anim) {
	if (anim == 0) return(0);

	return(anim_absolute(anim, anim->curposition + 1));
}


struct ImBuf * anim_nextpic(struct anim * anim) {
	struct ImBuf * ibuf = 0;

	if (anim == 0) return(0);

	ibuf = anim_getnext(anim);

	return(ibuf);
}

