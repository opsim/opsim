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

#ifndef IMBUF_H

#define IMBUF_H

#ifdef WIN32
#include "winstuff.h"
#endif
#include <sys/types.h>
#include <stdio.h> 

#ifndef WIN32
#include <unistd.h> 
#endif

#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>

#ifndef __FreeBSD__
#include <malloc.h>
#endif
#if defined(__BeOS) || defined(WIN32)
#define mmap my_mmap
#define munmap my_munmap
#else
#include <sys/mman.h>
#endif
#include "iff.h"

#if !defined(__WIN32) && !defined(__BeOS)
#define O_BINARY 0
#endif

#define SWAP_SHORT(x) (((unsigned short) (x) << 8) | ((unsigned short) (x) >> 8))
#define SWAP_LONG(x) (((x) << 24) | (((x) & 0xff00) << 8) | (((x) >> 8) & 0xff00) | (((x) >> 24) & 0xff))

#define ENDIAN_NOP(x) (x)

#if defined(__sgi) || defined(__SUN) || defined (__PPC__)
#define LITTLE_SHORT SWAP_SHORT
#define LITTLE_LONG SWAP_LONG
#define BIG_SHORT ENDIAN_NOP
#define BIG_LONG ENDIAN_NOP
#else
#define LITTLE_SHORT ENDIAN_NOP
#define LITTLE_LONG ENDIAN_NOP
#define BIG_SHORT SWAP_SHORT
#define BIG_LONG SWAP_LONG
#endif

#define malloc(x) mallocN(x, OBJECTBLOK)
#define free(x) freeN(x)
#define calloc(x,y) callocN((x)*(y),OBJECTBLOK)
#define freelist(x) freelistN(x)

/*
bindkey -r f1,'cc -O -c imbuf.c\n'

bindkeyo -r f1,'make paint \n'
bindkeyo -r f2,'paint /usr/4Dgifts/iristools/images/max5.rgb \n'

bindkey -r f1,'cc paint.c imbuf.c -lgl_s -lm -lfm_s -g -o paint\n'
bindkey -r f2,'/usr/people/bin/compres /usr/people/pics/0600\n'	

compileren met:

lc -Lm -f8 -dAMIGA imbuf
*/

#ifdef SHLIB
void *(*ib_calloc)();
#define calloc(x,y) ib_calloc((x),(y))
void *(*ib_malloc)();
#define malloc(x) ib_malloc(x)
void (*ib_free)();
#define free(x) ib_free(x)
void (*ib_memcpy)();
#define memcpy(x,y,z) ib_memcpy((x),(y),(z))
int (*ib_abs)();
#define abs(x) ib_abs(x)
void (*ib_fprin_tf)();
#define fprintf ib_fprin_tf
int (*ib_sprin_tf)();
#define sprintf ib_sprin_tf
void (*ib_prin_tf)();
#define printf ib_prin_tf
int (*ib_lseek)();
#define lseek(x,y,z) ib_lseek((x),(y),(z))
void *(*ib_mmap)();
#define mmap(u,v,w,x,y,z) ib_mmap((u),(v),(w),(x),(y),(z))
int (*ib_munmap)();
#define munmap(x,y) ib_munmap((x),(y))
int (*ib_open)();
#define open(x,y) ib_open((x),(y))
void (*ib_close)();
#define close(x) ib_close(x)
int (*ib_write)();
#define write(x,y,z) ib_write((x),(y),(z))
int (*ib_read)();
#define read(x,y,z) ib_read((x),(y),(z))
int (*ib_fchmod)();
#define fchmod(x,y) ib_fchmod((x),(y))
int (*ib_remove)();
#define remove(x) ib_remove(x)
size_t (*ib_strlen)();
#define strlen(x) ib_strlen(x)
int (*ib_isdigit)();
#define isdigit(x) ib_isdigit(x)
char *(*ib_strcpy)();
#define strcpy(x,y) ib_strcpy((x),(y))
int (*ib_atoi)();
#define atoi(x) ib_atoi(x)
char *(*ib_strcat)();
#define strcat(x,y) ib_strcat((x),(y))
int (*ib_stat)();
/* #define stat(x,y) ib_stat((x),(y)) */
FILE *ib_iob;
#define _iob ib_iob

#else

#define ib_stat stat

#endif /* SHLIB */


#define WIDTHB(x) (((x+15)>>4)<<1)

extern ushort *quadr;
extern float dyuvrgb[4][4];
extern float rgbdyuv[4][4];

extern struct ImBuf *loadamiga(int *,int);
extern struct ImBuf *loadcdi(int *,int);
extern struct ImBuf *loadanim(int *,int);
extern struct ImBuf *loadtarga(uchar *,int);
extern struct ImBuf *loadiris(uchar *,int);
extern struct ImBuf *loadtim(ushort *,int);
extern struct ImBuf *loadjpeg(char *,int);

extern ushort update_iff(int file, int code);

extern short *coldeltatab(uchar *coltab, short mincol, short maxcol, short cbits);
extern uint getdyuvstart();

#endif	/* IMBUF_H */

