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

#ifndef UTIL_H

#define UTIL_H

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#ifdef debug
#include <malloc.h>
#endif

#ifndef	NULL
#define NULL			0
#endif

#ifndef	FALSE
#define FALSE			0
#endif

#ifndef	TRUE
#define TRUE			1
#endif

#ifndef ulong
#define ulong unsigned long
#endif

#ifndef ushort
#define ushort unsigned short
#endif

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

#define mallocstruct(x,y) (x*)malloc((y)* sizeof(x))
#define callocstruct(x,y) (x*)calloc((y), sizeof(x))
#define mallocstructN(x,y,name) (x*)mallocN((y)* sizeof(x),name)
#define callocstructN(x,y,name) (x*)callocN((y)* sizeof(x),name)

#define RMK(x)

#define ELEM(a, b, c)		( (a)==(b) || (a)==(c) )
#define ELEM3(a, b, c, d)	( ELEM(a, b, c) || (a)==(d) )
#define ELEM4(a, b, c, d, e)	( ELEM(a, b, c) || ELEM(a, d, e) )
#define ELEM5(a, b, c, d, e, f)	( ELEM(a, b, c) || ELEM3(a, d, e, f) )
#define ELEM6(a, b, c, d, e, f, g)	( ELEM(a, b, c) || ELEM4(a, d, e, f, g) )
#define ELEM7(a, b, c, d, e, f, g, h)	( ELEM3(a, b, c, d) || ELEM4(a, e, f, g, h) )
#define ELEM8(a, b, c, d, e, f, g, h, i)	( ELEM4(a, b, c, d, e) || ELEM4(a, f, g, h, i) )

#define STREQ(str, a)		( strcmp((str), (a))==0 )
#define STREQ2(str, a, b)	( STREQ(str, a) || STREQ(str, b) )
#define STREQ3(str, a, b, c)	( STREQ2(str, a, b) || STREQ(str, c) )

#define MIN2(x,y)		( (x)<(y) ? (x) : (y) )
#define MIN3(x,y,z)		MIN2( MIN2((x),(y)) , (z) )
#define MIN4(x,y,z,a)		MIN2( MIN2((x),(y)) , MIN2((z),(a)) )

#define MAX2(x,y)		( (x)>(y) ? (x) : (y) )
#define MAX3(x,y,z)		MAX2( MAX2((x),(y)) , (z) )
#define MAX4(x,y,z,a)		MAX2( MAX2((x),(y)) , MAX2((z),(a)) )

#define SWAP(type, a, b)	{ type sw_ap; sw_ap=(a); (a)=(b); (b)=sw_ap; }

#ifndef ABS
#define ABS(x)	((x) < 0 ? -(x) : (x))
#endif

#ifdef AMIGA
#	define GL(x) (*((int *)(x)))
#else
#	define GL(x) (((ushort *)(x))[0] << 16 | ((ushort *)(x))[1])
#endif

#define GS(x) (((uchar *)(x))[0] << 8 | ((uchar *)(x))[1])
#define GSS(x) (((uchar *)(x))[1] << 8 | ((uchar *)(x))[0])

#define SWAP_L(x) (((x << 24) & 0xff000000) | ((x << 8) & 0xff0000) | ((x >> 8) & 0xff00) | ((x >> 24) & 0xff))
#define SWAP_S(x) (((x << 8) & 0xff00) | ((x >> 8) & 0xff))

#define SWAP_SHORT(x) (((unsigned short) (x) << 8) | ((unsigned short) (x) >> 8))
#define SWAP_LONG(x) (((x) << 24) | (((x) & 0xff00) << 8) | (((x) >> 8) & 0xff00) | (((x) >> 24) & 0xff))
#define ENDIAN_NOP(x) (x)

#define GET_BIG_LONG(x) (((uchar *) (x))[0] << 24 | ((uchar *) (x))[1] << 16 | ((uchar *) (x))[2] << 8 | ((uchar *) (x))[3])
#define GET_LITTLE_LONG(x) (((uchar *) (x))[3] << 24 | ((uchar *) (x))[2] << 16 | ((uchar *) (x))[1] << 8 | ((uchar *) (x))[0])

#if defined( __sgi) || defined (__SUN) || defined (__PPC__)
#define MAKE_ID(a,b,c,d) ( (int)(a)<<24 | (int)(b)<<16 | (c)<<8 | (d) )
#define GET_ID GET_BIG_LONG
#define LITTLE_SHORT SWAP_SHORT
#define LITTLE_LONG SWAP_LONG
#define BIG_SHORT ENDIAN_NOP
#define BIG_LONG ENDIAN_NOP
#else
#define MAKE_ID(a,b,c,d) ( (int)(d)<<24 | (int)(c)<<16 | (b)<<8 | (a) )
#define GET_ID GET_LITTLE_LONG
#define LITTLE_SHORT ENDIAN_NOP
#define LITTLE_LONG ENDIAN_NOP
#define BIG_SHORT SWAP_SHORT
#define BIG_LONG SWAP_LONG
#endif

#ifdef __ANSI_CPP__
	/* ansi */
	#define NEW(x) (x*)mallocN(sizeof(x),# x)
	#define CLN(x) (x*)callocN(sizeof(x),# x)
	#define PRINT(d, var1)	printf(# var1 ":%" # d "\n", var1)
	#define PRINT2(d, e, var1, var2)	printf(# var1 ":%" # d " " # var2 ":%" # e "\n", var1, var2)
	#define PRINT3(d, e, f, var1, var2, var3)	printf(# var1 ":%" # d " " # var2 ":%" # e " " # var3 ":%" # f "\n", var1, var2, var3)
	#define PRINT4(d, e, f, g, var1, var2, var3, var4)	printf(# var1 ":%" # d " " # var2 ":%" # e " " # var3 ":%" # f " " # var4 ":%" # g "\n", var1, var2, var3, var4)
#else
	/* -cckr */
#define NEW(x) (x*)mallocN(sizeof(x),"x")
#define CLN(x) (x*)callocN(sizeof(x),"x")
	#define PRINT(d, var1)	printf(# var1 ":%" # d "\n", var1)
	#define PRINT2(d, e, var1, var2)	printf(# var1 ":%" # d " " # var2 ":%" # e "\n", var1, var2)
	#define PRINT3(d, e, f, var1, var2, var3)	printf(# var1 ":%" # d " " # var2 ":%" # e " " # var3 ":%" # f "\n", var1, var2, var3)
	#define PRINT4(d, e, f, g, var1, var2, var3, var4)	printf(# var1 ":%" # d " " # var2 ":%" # e " " # var3 ":%" # f " " # var4 ":%" # g "\n", var1, var2, var3, var4)
#endif


typedef struct Link
{
	struct Link *next,*prev;
} Link;

/* never change the size of this! genfile.c detects pointerlen with it */
typedef struct ListBase 
{
	void *first, *last;
} ListBase;

/* 8 byte alignment! */

typedef struct MemHead {
	int tag1;
	int len;
	struct MemHead *next,*prev;
	char * name;
	char * nextname;
	int level;
	int tag2;
} MemHead;

typedef struct MemTail {
	int tag3, pad;
} MemTail;



#define MEMTAG1 MAKE_ID('M', 'E', 'M', 'O')
#define MEMTAG2 MAKE_ID('R', 'Y', 'B', 'L')
#define MEMTAG3 MAKE_ID('O', 'C', 'K', '!')
#define MEMFREE MAKE_ID('F', 'R', 'E', 'E')

#define MEMNEXT(x) ((MemHead *)(((char *) x) - ((char *) & (((MemHead *)0)->next))))

extern int totblock;
extern int mem_in_use;


#ifdef debug
#define malloc(x) mallocN(x,"debug")
#define free(x) freeN(x)
#define calloc(x,y) callocN((x)*(y),"debug")
#endif

/* ******* util.c */
extern          void Sginap(int Ticks);
extern          void addhead(ListBase *listbase, void *vlink);
extern          void addtail(ListBase *listbase, void *vlink);
extern          void ap_framelen(char * cmd, int i);
extern          void *callocN(int len, char *str);
extern          char *check_memlist(MemHead *memh);
extern          int countlist(ListBase *listbase);
extern          int file_size(char * name);
extern          short fileselect(char *title, char *dir);
extern          short freeN(void *vmemh);   
extern          void freelinkN(ListBase *listbase, void *vlink);
extern          void freelist(ListBase *listbase);
extern          void freelistN(ListBase *listbase);
extern          void insertlink(ListBase *listbase, void *vprevlink, void *vnewlink);
extern          void insertlinkbefore(ListBase *listbase, void *vnextlink, void *vnewlink);
extern          void * load_to_mem(char * name);
extern          void *mallocN(int len, char *str);
extern          void newname(char * name, int add);
extern          void printmemlist();
extern          void rem_memblock(MemHead *memh);
extern          void remlink(ListBase *listbase, void *vlink);
extern          int string_to_tcode(char * str);
extern          int stringdec(char *string, char *kop, char *staart, ushort *numlen);
extern          void stringenc(char *string, char *kop, char *staart, ushort numlen, int pic);
extern          short stringselect(char *title, char *file);
extern          int strnmatch(char *s1, char *s2);
extern          char *tcode_to_string(int len);


#endif /* UTIL_H */

