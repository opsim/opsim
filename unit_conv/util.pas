(**
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
 *)

(* Version: $Id: util.h,v 1.2 2000/07/25 08:53:07 nzc Exp $ *)

unit util;

{$mode objfpc}{$H+}

interface

uses
ctypes;

//#ifdef debug
//#include <malloc.h>
//#endif

//#ifndef	NULL
//#define NULL			0
//#endif

//#ifndef	FALSE
//#define FALSE			0
//#endif

//#ifndef	TRUE
//#define TRUE			1
//#endif

//#ifndef ulong
//#define ulong unsigned long
//#endif

//#ifndef ushort
//#define ushort unsigned short
//#endif

//#ifndef uchar
//#define uchar unsigned char
//#endif

//#ifndef uint
//#define uint unsigned int
//#endif

//#define mallocstruct(x,y) (x*)malloc((y)* sizeof(x))
//#define callocstruct(x,y) (x*)calloc((y), sizeof(x))
//#define mallocstructN(x,y,name) (x*)mallocN((y)* sizeof(x),name)
//#define callocstructN(x,y,name) (x*)callocN((y)* sizeof(x),name)

//#define RMK(x)

//#define ELEM(a, b, c)		( (a)=(b) or (a)=(c) )
//#define ELEM3(a, b, c, d)	( ELEM(a, b, c) or (a)=(d) )
//#define ELEM4(a, b, c, d, e)	( ELEM(a, b, c) or ELEM(a, d, e) )
//#define ELEM5(a, b, c, d, e, f)	( ELEM(a, b, c) or ELEM3(a, d, e, f) )
//#define ELEM6(a, b, c, d, e, f, g)	( ELEM(a, b, c) or ELEM4(a, d, e, f, g) )
//#define ELEM7(a, b, c, d, e, f, g, h)	( ELEM3(a, b, c, d) or ELEM4(a, e, f, g, h) )
//#define ELEM8(a, b, c, d, e, f, g, h, i)	( ELEM4(a, b, c, d, e) or ELEM4(a, f, g, h, i) )
function ELEM(a, b, c: clong): boolean;
function ELEM3(a, b, c, d: clong): boolean;
function ELEM4(a, b, c, d, e: clong): boolean;
function ELEM5(a, b, c, d, e, f: clong): boolean;
function ELEM6(a, b, c, d, e, f, g: clong): boolean;
function ELEM7(a, b, c, d, e, f, g, h: clong): boolean;
function ELEM8(a, b, c, d, e, f, g, h, i: clong): boolean;

//#define STREQ(str, a)		( strcmp((str), (a))=0 )
//#define STREQ2(str, a, b)	( STREQ(str, a) or STREQ(str, b) )
//#define STREQ3(str, a, b, c)	( STREQ2(str, a, b) or STREQ(str, c) )

function MIN2(x,y: cfloat): cfloat;
function MIN3(x,y,z: cfloat): cfloat;
function MIN4(x,y,z,a: cfloat): cfloat;

function MAX2(x,y: cfloat): cfloat;
function MAX3(x,y,z: cfloat): cfloat;
function MAX4(x,y,z,a: cfloat): cfloat;

//#define SWAP(type, a, b)	{ type sw_ap; sw_ap=(a); (a)=(b); (b)=sw_ap; }

//#ifndef ABS
//#define ABS(x)	((x) < 0 ? -(x) : (x))
//#endif

//#ifdef AMIGA
//#	define GL(x) (*((int *)(x)))
//#else
//#	define GL(x) (((ushort *)(x))[0] << 16 | ((ushort *)(x))[1])
//#endif

//#define GS(x) (((uchar *)(x))[0] << 8 | ((uchar *)(x))[1])
//#define GSS(x) (((uchar *)(x))[1] << 8 | ((uchar *)(x))[0])

//#define SWAP_L(x) (((x << 24) & 0xff000000) | ((x << 8) & 0xff0000) | ((x >> 8) & 0xff00) | ((x >> 24) & 0xff))
//#define SWAP_S(x) (((x << 8) & 0xff00) | ((x >> 8) & 0xff))

//#define SWAP_SHORT(x) (((unsigned short) (x) << 8) | ((unsigned short) (x) >> 8))
//#define SWAP_LONG(x) (((x) << 24) | (((x) & 0xff00) << 8) | (((x) >> 8) & 0xff00) | (((x) >> 24) & 0xff))
//#define ENDIAN_NOP(x) (x)

//#define GET_BIG_LONG(x) (((uchar *) (x))[0] << 24 | ((uchar *) (x))[1] << 16 | ((uchar *) (x))[2] << 8 | ((uchar *) (x))[3])
//#define GET_LITTLE_LONG(x) (((uchar *) (x))[3] << 24 | ((uchar *) (x))[2] << 16 | ((uchar *) (x))[1] << 8 | ((uchar *) (x))[0])

//#if defined( __sgi) or defined (__SUN) or defined (__PPC__)
//#define MAKE_ID(a,b,c,d) ( (int)(a)<<24 | (int)(b)<<16 | (c)<<8 | (d) )
//#define GET_ID GET_BIG_LONG
//#define LITTLE_SHORT SWAP_SHORT
//#define LITTLE_LONG SWAP_LONG
//#define BIG_SHORT ENDIAN_NOP
//#define BIG_LONG ENDIAN_NOP
//#else
//#define MAKE_ID(a,b,c,d) ( (int)(d)<<24 | (int)(c)<<16 | (b)<<8 | (a) )
//#define GET_ID GET_LITTLE_LONG
//#define LITTLE_SHORT ENDIAN_NOP
//#define LITTLE_LONG ENDIAN_NOP
//#define BIG_SHORT SWAP_SHORT
//#define BIG_LONG SWAP_LONG
//#endif

//#ifdef __ANSI_CPP__
//	(* ansi *)
//	#define NEW(x) (x*)mallocN(sizeof(x),# x)
//	#define CLN(x) (x*)callocN(sizeof(x),# x)
//	#define PRINT(d, var1)	printf(# var1 ":%" # d "\n", var1)
//	#define PRINT2(d, e, var1, var2)	printf(# var1 ":%" # d " " # var2 ":%" # e "\n", var1, var2)
//	#define PRINT3(d, e, f, var1, var2, var3)	printf(# var1 ":%" # d " " # var2 ":%" # e " " # var3 ":%" # f "\n", var1, var2, var3)
//	#define PRINT4(d, e, f, g, var1, var2, var3, var4)	printf(# var1 ":%" # d " " # var2 ":%" # e " " # var3 ":%" # f " " # var4 ":%" # g "\n", var1, var2, var3, var4)
//#else
//	(* -cckr *)
//#define NEW(x) (x*)mallocN(sizeof(x),"x")
//#define CLN(x) (x*)callocN(sizeof(x),"x")
//	#define PRINT(d, var1)	printf(# var1 ":%" # d "\n", var1)
//	#define PRINT2(d, e, var1, var2)	printf(# var1 ":%" # d " " # var2 ":%" # e "\n", var1, var2)
//	#define PRINT3(d, e, f, var1, var2, var3)	printf(# var1 ":%" # d " " # var2 ":%" # e " " # var3 ":%" # f "\n", var1, var2, var3)
//	#define PRINT4(d, e, f, g, var1, var2, var3, var4)	printf(# var1 ":%" # d " " # var2 ":%" # e " " # var3 ":%" # f " " # var4 ":%" # g "\n", var1, var2, var3, var4)
//#endif


type
pLink = ^Link;
Link  = record
	next,prev: pLink;
end;

(* never change the size of this! genfile.c detects pointerlen with it *)
pListBase = ^ListBase;
ListBase = record
	first, last: pointer;
end;

(* 8 byte alignment! *)

pMemHead = ^MemHead;
MemHead = record
	 tag1: cint;
	 len: cint;
	 next,prev: pMemHead;
	 name: pchar;
	 nextname: pchar;
	 level: cint;
	  tag2: cint;
end;

pMemTail = ^MemTail;
MemTail = record
	 tag3, pad: cint;
end;



//#define MEMTAG1 MAKE_ID('M', 'E', 'M', 'O')
//#define MEMTAG2 MAKE_ID('R', 'Y', 'B', 'L')
//#define MEMTAG3 MAKE_ID('O', 'C', 'K', '!')
//#define MEMFREE MAKE_ID('F', 'R', 'E', 'E')
const
MEMTAG1 = 1234;
MEMTAG2 = 5678;
MEMTAG3 = 9012;
MEMFREE = 3456;

//#define MEMNEXT(x) ((MemHead *)(((char *) x) - ((char *) & (((MemHead *)0)^.next))))

//var
//  totblock: cint;
// mem_in_use: cint;


//#ifdef debug
//#define malloc(x) mallocN(x,"debug")
//#define free(x) freeN(x)
//#define calloc(x,y) callocN((x)*(y),"debug")
//#endif

//(* ******* util.c *)
//extern          procedure Sginap(int Ticks);
procedure addhead( listbase: pListBase; vlink: pointer);
procedure addtail( listbase: pListBase; vlink: pointer);
//extern          procedure ap_framelen(char * cmd, int i);
function callocN(len: cint; str: pchar): pointer;
function check_memlist(memh: pMemHead): PChar;
function countlist(listbase: pListBase): integer;
//extern          int file_size(char * name);
//extern          short fileselect(char *title, char *dir);
function           freeN( vmemh: pointer): cshort;
//extern          procedure freelinkN(listbase: pListBase, procedure *vlink);
//extern          procedure freelist(listbase: pListBase);
procedure freelistN( listbase: pListBase);
//extern          procedure insertlink(listbase: pListBase, procedure *vprevlink, procedure *vnewlink);
procedure insertlinkbefore(listbase: pListBase; vnextlink, vnewlink: pointer);
//extern          procedure * load_to_mem(char * name);
function mallocN(len: cint; str: pchar): pointer;
//procedure newname(name: pchar; add: integer);
procedure printmemlist;
procedure rem_memblock( memh: pMemHead);
procedure remlink( listbase: pListBase; vlink: pointer);
//extern          int string_to_tcode(char * str);
//function stringdec(_string, kop, staart: pchar; numlen: pcushort): integer;
//extern          procedure stringenc(char *string, char *kop, char *staart, ushort numlen, int pic);
//extern          short stringselect(char *title, char *file);
//extern          int strnmatch(char *s1, char *s2);
//extern          char *tcode_to_string(int len);


//#endif (* UTIL_H *)


implementation

uses
sysutils, cfuncs;

function MEMNEXT(x: pMemHead): pMemHead; {$note make inline}
begin
  Result := pMemHead(pchar(x) - pchar(@(pMemHead(nil)^.next)));
end;

//(**
// * $Id:$
// * ***** BEGIN GPL/BL DUAL LICENSE BLOCK *****
// *
// * The contents of this file may be used under the terms of either the GNU
// * General Public License Version 2 or later (the "GPL", see
// * http://www.gnu.org/licenses/gpl.html ), or the Blender License 1.0 or
// * later (the "BL", see http://www.blender.org/BL/ ) which has to be
// * bought from the Blender Foundation to become active, in which case the
// * above mentioned GPL option does not apply.
// *
// * The Original Code is Copyright (C) 2002 by NaN Holding BV.
// * All rights reserved.
// *
// * The Original Code is: all of this file.
// *
// * Contributor(s): none yet.
// *
// * ***** END GPL/BL DUAL LICENSE BLOCK *****
// *)

//#include <stdio.h>

//#include "util.h"
//#include <fcntl.h>

//#ifdef WIN32
//#include "winstuff.h"
//#define strncasecmp(a, b, n) strncmp(a, b, n)
//#endif

//#ifndef WIN32
//#include <sys/time.h>
//#endif

var
totblock: cint=0;
mem_in_use : cint = 0;
current_mem_level : cint = 0;

 _membase: ListBase;
membase: pListBase = @_membase;

//function stringdec(_string, kop, staart: pchar; numlen: cushort): integer;
//var
//  len, len2, nums, nume: cushort;
//  i: cshort;
//  found: boolean =FALSE;
//begin
//	len2 := strlen(_string);
//        len :=  len2;
//
//	if (len > 6) then begin
//		if (strncasecmp(_string + len - 6, ".blend", 6) = 0) len -= 6;
//		else if (strncasecmp(string + len - 6, ".trace", 6) = 0) len -= 6;
//	end;
//
//	if (len = len2) begin
//		if (len > 4) begin
//			(* .jf0 en .jf1 voor jstreams afvangen *)
//			if (strncasecmp(string + len - 4, ".jf", 3) = 0) len -= 4;
//			else if (strncasecmp(string + len - 4, ".tga", 4) = 0) len -= 4;
//			else if (strncasecmp(string + len - 4, ".jpg", 4) = 0) len -= 4;
//			else if (strncasecmp(string + len - 4, ".txt", 4) = 0) len -= 4;
//			else if (strncasecmp(string + len - 4, ".cyc", 4) = 0) len -= 4;
//			else if (strncasecmp(string + len - 4, ".enh", 4) = 0) len -= 4;
//			else if (strncasecmp(string + len - 4, ".rgb", 4) = 0) len -= 4;
//			else if (strncasecmp(string + len - 4, ".psx", 4) = 0) len -= 4;
//		end;
//	end;
//
//	for (i = len - 1; i >= 0; i--)begin
//		if (string[i] = '/') break;
//		if (isdigit(string[i])) begin
//			if (found)begin
//				nums = i;
//			end;
//			elsebegin
//				nume = i;
//				nums = i;
//				found = TRUE;
//			end;
//		end;
//		elsebegin
//			if (found) break;
//		end;
//	end;
//	if (found)begin
//		if (staart) strcpy(staart,&string[nume+1]);
//		if (kop) begin
//			strcpy(kop,string);
//			kop[nums]=0;
//		end;
//		if (numlen) *numlen = nume-nums+1;
//		return ((int)atoi(&(string[nums])));
//	end;
//	if (staart) strcpy(staart, string + len);
//	if (kop) begin
//		strncpy(kop, string, len);
//		kop[len] = 0;
//	end;
//	if (numlen) *numlen=0;
//	return 0;
//end;


//procedure stringenc(char *string, char *kop, char *staart, ushort numlen, int pic)
//begin
//	char numstr[10]="";
//	ushort len,i;

//	strcpy(string,kop);
	
//	if (pic>0 or numlen=4) begin
//		len= sprintf(numstr,"%d",pic);

//		for(i=len;i<numlen;i++)begin
//			strcat(string,"0");
//		end;
//		strcat(string,numstr);
//	end;
//	strcat(string,staart);
//end;


//procedure newname(name: pchar; add: integer);
//var
//  head, tail: string[128];
//  pic: integer;
//  digits: cushort;
//begin
//	pic := stringdec(name, head, tail, @digits);
//
//	(* gaan we van 100 -> 99 of van 10 naar 9 *)
//	if (add < 0) and (digits < 4) and (digits > 0) then
//        begin
//		int i, exp;
//		exp = 1;
//		for (i = digits; i > 1; i--) exp *= 10;
//		if (pic >= exp and (pic + add) < exp) digits--;
//	end;
//
//	pic += add;
//
//	if(digits=4 and pic<0) pic= 0;
//	stringenc(name, head, tail, digits, pic);
//end;

function ELEM(a, b, c: clong): boolean;
begin
  ELEM := ( (a=b) or (a=c) );
end;

function ELEM3(a, b, c, d: clong): boolean;
begin
  ELEM3 := ( ELEM(a, b, c) or (a=d) );
end;

function ELEM4(a, b, c, d, e: clong): boolean;
begin
  ELEM4 := ( ELEM(a, b, c) or ELEM(a, d, e) );
end;

function ELEM5(a, b, c, d, e, f: clong): boolean;
begin
  ELEM5 := ( ELEM(a, b, c) or ELEM3(a, d, e, f) );
end;

function ELEM6(a, b, c, d, e, f, g: clong): boolean;
begin
  ELEM6 := ( ELEM(a, b, c) or ELEM4(a, d, e, f, g) );
end;

function ELEM7(a, b, c, d, e, f, g, h: clong): boolean;
begin
  ELEM7 := ( ELEM3(a, b, c, d) or ELEM4(a, e, f, g, h) );
end;

function ELEM8(a, b, c, d, e, f, g, h, i: clong): boolean;
begin
  ELEM8 := ( ELEM4(a, b, c, d, e) or ELEM4(a, f, g, h, i) );
end;


function MIN2(x, y: cfloat): cfloat;
begin
  if x<y then
          MIN2 := x
  else
          MIN2 := y;
end;

function MIN3(x, y, z: cfloat): cfloat;
begin
  MIN3 := MIN2( MIN2(x,y) , z);
end;

function MIN4(x, y, z, a: cfloat): cfloat;
begin
   MIN4 := MIN2( MIN2(x,y) , MIN2(z,a) );
end;

function MAX2(x, y: cfloat): cfloat;
begin
  if  x>y then
          MAX2 := x
  else
          MAX2 := y;
end;

function MAX3(x, y, z: cfloat): cfloat;
begin
  MAX3 := MAX2( MAX2(x,y) , z );
end;

function MAX4(x, y, z, a: cfloat): cfloat;
begin
  MAX4 := MAX2( MAX2(x,y) , MAX2(z,a) );
end;

procedure addhead( listbase: pListBase; vlink: pointer);
var
  link : pLink;
begin
	link := vlink;

	if (link = nil) then exit;
	if (listbase = nil) then exit;

	link^.next := listbase^.first;
	link^.prev := nil;

	if (listbase^.first<>nil) then pLink(listbase^.first)^.prev := link;
	if (listbase^.last = nil) then listbase^.last := link;
	listbase^.first := link;
end;


procedure addtail(listbase: pListBase; vlink: pointer);
var
link : pLink;
begin
	link:= vlink;

	if (link = nil) then exit;
	if (listbase = nil) then exit;

	link^.next := nil;
	link^.prev := listbase^.last;

	if (listbase^.last<>nil) then pLink(listbase^.last)^.next := link;
	if (listbase^.first = nil) then listbase^.first := link;
	listbase^.last := link;
end;


procedure remlink(listbase: pListBase; vlink: pointer);
var
  link: pLink;
begin
	link:= vlink;

	if (link = nil) then exit;
	if (listbase = nil) then exit;

	if (link^.next<>nil) then link^.next^.prev := link^.prev;
	if (link^.prev<>nil ) then link^.prev^.next := link^.next;

	if (listbase^.last = link) then  listbase^.last := link^.prev;
	if (listbase^.first = link) then listbase^.first := link^.next;
end;


//procedure freelinkN(listbase: pListBase, procedure *vlink)
//{
//	struct Link *link= vlink;

//	if (link = 0) return;
//	if (listbase = 0) return;

//	remlink(listbase,link);
//	freeN(link);
//}


//procedure insertlink(listbase: pListBase, procedure *vprevlink, procedure *vnewlink)
//{
//	struct Link *prevlink= vprevlink, *newlink= vnewlink;

//	(* newlink komt na prevlink *)

//	if (newlink = 0) return;
//	if (listbase = 0) return;

//	if(listbase^.first=0) { (* lege lijst *)
//		listbase^.first= newlink;
//		listbase^.last= newlink;
//		return;
//	}
//	if (prevlink= 0) {	(* inserten voor eerste element *)
//		newlink^.next= listbase^.first;
//		newlink^.prev= 0;
//		newlink^.next^.prev= newlink;
//		listbase^.first= newlink;
//		return;
//	}

//	if (listbase^.last= prevlink) (* aan einde lijst *)
//		listbase^.last = newlink;

//	newlink^.next= prevlink^.next;
//	prevlink^.next= newlink;
//	if(newlink^.next) newlink^.next^.prev= newlink;
//	newlink^.prev= prevlink;
//}

procedure insertlinkbefore(listbase: pListBase; vnextlink, vnewlink: pointer);
var
  nextlink, newlink: pLink;
begin
	nextlink:= vnextlink; newlink:= vnewlink;

	(* newlink komt voor nextlink *)

	if (newlink = nil) then exit;
	if (listbase = nil) then exit;

	if(listbase^.first=nil) then begin (* lege lijst *)
		listbase^.first:= newlink;
		listbase^.last:= newlink;
		exit;
	end;
	if (nextlink= nil) then begin	(* inserten aan einde lijst *)
		newlink^.prev:= listbase^.last;
		newlink^.next:= nil;
		pLink(listbase^.last)^.next:= newlink;
		listbase^.last:= newlink;
		exit;
	end;

	if (listbase^.first= nextlink) then (* aan begin lijst *)
		listbase^.first := newlink;

	newlink^.next:= nextlink;
	newlink^.prev:= nextlink^.prev;
	nextlink^.prev:= newlink;
	if(newlink^.prev<>nil) then newlink^.prev^.next:= newlink;
end;


//procedure freelist(listbase: pListBase)
//{
//	struct Link *link,*next;

//	if (listbase = 0) return;
//	link= listbase^.first;
//	while(link) {
//		next= link^.next;
//		free(link);
//		link= next;
//	}
//	listbase^.first=0;
//	listbase^.last=0;
//}

procedure freelistN(listbase: pListBase);
var
 link,next: pLink;
begin

	if (listbase = nil) then exit;
	link:= listbase^.first;
	while(link<>nil) do begin
		next:= link^.next;
		freeN(link);
		link:= next;
	end;
	listbase^.first:=nil;
	listbase^.last:=nil;
end;


function countlist(listbase: pListBase): integer;
var
  link: pLink;
  count : integer = 0;
begin
  if listbase<>nil then
  begin
    link := listbase^.first;
    while link<>nil do
    begin
      inc(count);
      link := link^.next;
    end;
  end;
  exit(count);
end;

//#ifndef WIN32
//short fileselect(char *title, char *dir)
//{
//	char buf[200];
//	int fspipe[2],len,ok;

//	if (pipe(fspipe)) return (0);

//	(*sprintf(buf,"fs '%s' '%d' '%s'",dir,fspipe[1],title);*)
//	sprintf(buf,"fs -d '%s' -p '%d' -t '%s'",dir,fspipe[1],title);

//	len=ok=0;
//	if (system(buf)=0){
//		if (read(fspipe[0],&len,4)=4){
//			if (read(fspipe[0],buf,len)=len){
//				if (len>1){
//					strcpy(dir,buf);
//					ok=1;
//				}
//			}
//		}
//	}

//	close(fspipe[0]);
//	close(fspipe[1]);
//	return (ok);
//}


//short stringselect(char *title, char *file)
//{
//	char buf[200];
//	int fspipe[2],len,ok;

//	if (pipe(fspipe)) return (0);

//	sprintf(buf,"fs -f '%s' -s -p '%d' -t '%s'",file,fspipe[1],title);

//	len=ok=0;
//	if (system(buf)=0){
//		if (read(fspipe[0],&len,4)=4){
//			if (read(fspipe[0],buf,len)=len){
//				if (len>1){
//					strcpy(file,buf);
//					ok=1;
//				}
//			}
//		}
//	}

//	close(fspipe[0]);
//	close(fspipe[1]);
//	return (ok);
//}
//#endif


//#ifdef malloc
//#undef malloc
//#endif

//#ifdef calloc
//#undef calloc
//#endif

//#ifdef free
//#undef free
//#endif


procedure _MemorY_ErroR( block, error: pchar);
begin
	writeln(stderr,'Memoryblock ', block,': ',error);
end;

var
memory_error: procedure( block, error: pchar)  = @_MemorY_ErroR;

function mallocN(len: cint; str: pchar): pointer;
var
 memh: pMemHead;
 memt: pMemTail;
begin

	if(len<=0) then begin
		writeln('Malloc error: len=', len,' in ',str);
		exit(nil);
	end;

	if(sizeof(clong)=8) then
		len := (len + 3 ) and not 3 	(* eenheden van 4 *)
	else
		len := (len + 7 ) and not 7; 	(* eenheden van 8 *)
	
	memh:= pMemHead(malloc(len+sizeof(MemHead)+sizeof(MemTail)));

	if(memh<>nil) then begin
		memh^.tag1 := MEMTAG1;
                memh^.name := StrAlloc(Strlen(str)+1);
		strcopy(memh^.name, str);
		memh^.nextname := nil;
		memh^.len := len;
		memh^.level := current_mem_level;
		memh^.tag2 := MEMTAG2;

		memt := pMemTail(pchar(memh) + sizeof(MemHead) + len);
		memt^.tag3 := MEMTAG3;

		addtail(membase,@memh^.next);
		if (memh^.next<>nil) then memh^.nextname := MEMNEXT(memh^.next)^.name;

		inc(totblock);
		mem_in_use += len;

		exit(PChar(memh) + sizeof(MemHead));
	end;
	writeln('Malloc returns nill: len=', len,' in ',str);
	exit(nil);
end;

function callocN(len: cint; str: pchar): pointer;
var
 memh: pMemHead;
 memt: pMemTail;
begin

	if(len<=0) then begin
		writeln('Malloc error: len=', len, ' in ',str);
		exit(nil);
	end;

	if(sizeof(clong)=8) then
		len := (len + 3 ) and not 3 	(* eenheden van 4 *)
	else
		len := (len + 7 ) and not 7; 	(* eenheden van 8 *)

	memh:= pMemHead(calloc(len+sizeof(MemHead)+sizeof(MemTail),1));

	if(memh<>NIL) then begin
		memh^.tag1 := MEMTAG1;
		memh^.name := str;
		memh^.nextname := NIL;
		memh^.len := len;
		memh^.level := current_mem_level;
		memh^.tag2 := MEMTAG2;

		memt := pMemTail(pchar(memh) + sizeof(MemHead) + len);
		memt^.tag3 := MEMTAG3;

		addtail(membase,@memh^.next);
		if (memh^.next<>nil) then  memh^.nextname := MEMNEXT(memh^.next)^.name;

		inc(totblock);
		mem_in_use += len;

		exit(PChar(memh) + sizeof(MemHead));
	end;
	writeln('Calloc returns nill: len=', len, ' in ',str);
	exit(nil);
end;


procedure rem_memblock( memh: pMemHead);
begin
	remlink(membase,@memh^.next);
	if (memh^.prev<>nil) then begin
		if (memh^.next<>nil) then MEMNEXT(memh^.prev)^.nextname := MEMNEXT(memh^.next)^.name
		else MEMNEXT(memh^.prev)^.nextname := nil;
	end;

	dec(totblock);
	mem_in_use -= memh^.len;
	free(memh);
end;


procedure printmemlist;
var
 membl: pMemHead;
begin
	membl := pMemHead(membase^.first);
	if membl<>nil then membl := MEMNEXT(membl);
	while membl<>nil do
        begin
		writeln(Format('%s len: %d %p',[membl^.name, membl^.len, pointer(membl+1)]));
		//writeln(Format('%s len: %d',[membl^.name, membl^.len]));
		if membl^.next<>nil then
			membl:= MEMNEXT(membl^.next)
		else
                  break;
	end;
end;

function check_memlist(memh: pMemHead): PChar;
var
forw,back,forwok,backok: pMemHead;
	 name: pchar;
begin

	forw := membase^.first;
	if (forw<>nil) then forw := MEMNEXT(forw);
	forwok := nil;
	while(forw<>nil) do begin
		if (forw^.tag1 <> MEMTAG1) or (forw^.tag2 <> MEMTAG2) then break;
		forwok := forw;
		if (forw^.next<>nil) then forw := MEMNEXT(forw^.next)
		else forw := nil;
	end;

	back := pMemHead(membase^.last);
	if (back<>nil) then back := MEMNEXT(back);
	backok := nil;
	while(back<>nil) do begin
		if (back^.tag1 <> MEMTAG1) or (back^.tag2 <> MEMTAG2) then break;
		backok := back;
		if (back^.prev<>nil) then back := MEMNEXT(back^.prev)
		else back := nil;
	end;

	if (forw <> back) then exit ('MORE THAN 1 MEMORYBLOCK CORRUPT');

	if (forw = nil) and (back = nil) then begin
		(* geen foute headers gevonden dan maar op zoek naar memblock*)

		forw := membase^.first;
		if (forw<>nil) then forw := MEMNEXT(forw);
		forwok := nil;
		while(forw<>nil) do begin
			if (forw = memh) then break;
			if (forw^.tag1 <> MEMTAG1) or (forw^.tag2 <> MEMTAG2) then break;
			forwok := forw;
			if (forw^.next<>nil) then forw := MEMNEXT(forw^.next)
			else forw := nil;
		end;
		if (forw = nil) then exit (nil);

		back := pMemHead(membase^.last);
		if (back<>nil) then back := MEMNEXT(back);
		backok := nil;
		while(back<>nil) do begin
			if (back = memh) then break;
			if (back^.tag1 <> MEMTAG1) or (back^.tag2 <> MEMTAG2) then break;
			backok := back;
			if (back^.prev<>nil) then back := MEMNEXT(back^.prev)
			else back := nil;
		end;
	end;

	if (forwok<>nil) then name := forwok^.nextname
	else name := 'No name found';

	if (forw = memh) then begin
		(* voor alle zekerheid wordt dit block maar uit de lijst gehaald *)
		if (forwok<>nil) then begin
			if (backok<>nil) then begin
				forwok^.next := pMemHead(@backok^.next);
				backok^.prev := pMemHead(@forwok^.next);
				forwok^.nextname := backok^.name;
			end else begin
				forwok^.next := nil;
				membase^.last := pLink(@forwok^.next);
			end;
		end else begin
			if (backok<>nil) then begin
				backok^.prev := nil;
				membase^.first := @backok^.next;
			end  else begin
				membase^.first := nil;
                                membase^.last := nil;
			end;
		end;
	end else begin
		memory_error(name,'Aditional error in header');
		exit(nil);
	end;

	exit(name);
end;


function freeN(vmemh: pointer): cshort;		(* anders compileertie niet meer *)
var
 error: cshort = 0;
 memt: pMemTail;
 memh: pMemHead;
 name: pchar;
begin
     memh:= vmemh;

	if (memh = nil) then begin
		memory_error('free','attempt to free nil pointer');
		(* printf("%d\n", (memh+4000)^.tag1); *)
		exit(-1);
	end;

	if(sizeof(clong)=8) then begin
		if (clong(memh) and $7)<>0 then begin
			memory_error('free','attempt to free illegal pointer');
			exit(-1);
		end;
	end
	else begin
		if (clong(memh) and $3) <>0 then begin
			memory_error('free','attempt to free illegal pointer');
			exit(-1);
		end;
	end;
	
	dec(memh);
	if(memh^.tag1 = MEMFREE) and (memh^.tag2 = MEMFREE) then begin
		memory_error(memh^.name,'double free');
		exit(-1);
	end;

	if (memh^.tag1 =MEMTAG1) and (memh^.tag2 = MEMTAG2) and ((memh^.len and $3) = 0) then begin
		memt := pMemTail(pchar(memh) + sizeof(MemHead) + memh^.len);
		if (memt^.tag3 = MEMTAG3)then begin
			
			memh^.tag1 := MEMFREE;
			memh^.tag2 := MEMFREE;
			memt^.tag3 := MEMFREE;
			(* na tags !!! *)
			rem_memblock(memh);
			
			exit(0);
		end;
		error := 2;
		memory_error(memh^.name,'end corrupt');
		name := check_memlist(memh);
		if (name <> nil) then begin
			if (name <> memh^.name) then memory_error(name,'is also corrupt');
		end;
	end else begin
		error := -1;
		name := check_memlist(memh);
		if (name = nil) then memory_error('free','pointer not in memlist')
		else memory_error(name,'error in header');
	end;

	dec(totblock);
	(* hier moet een DUMP plaatsvinden *)

	exit(error);
end;


//procedure ap_framelen(char * cmd, int i)
//{
//	char buf[8];

//	if (i<10) sprintf(buf,"0%d",i);
//	else sprintf(buf,"%d",i);

//	strcat(cmd,buf);
//}


//char *tcode_to_string(int len)
//{
//	int i;
//	int rlen;
//	static char str[64];

//	if (len < 0){
//		strcpy(str, "- ");
//		len = -len;
//	} else{
//		str[0] = 0;
//	}
//	rlen = len;

//	if (len >= 90000){
//		i = rlen/90000;
//		ap_framelen(str,i);
//		strcat(str,":");
//		rlen -= 90000 * i;
//	}
//	if (len >= 1500){
//		i = rlen/1500;
//		ap_framelen(str,i);
//		strcat(str,":");
//		rlen -= 1500 * i;
//	}
//	if (len >= 25){
//		i = rlen/25;
//		ap_framelen(str,i);
//		strcat(str,":");
//		rlen -= 25 * i;
//	}
//	ap_framelen(str,rlen);
//	return(str);
//}


//int string_to_tcode(char * str)
//{
//	int i,val[4],tcode = 0,j=0;
//	char string[64];

//	strcpy(string,str);

//	for (i = 0 ; string[i] ; i++){
//		if (isdigit(string[i]) = 0) string[i] = ' ';
//	}

//	i = sscanf(string,"%ld%ld%ld%ld",val,val+1,val+2,val+3);

//	switch(i){
//	case 4:
//		tcode += 90000 * val[j++];
//	case 3:
//		tcode += 1500 * val[j++];
//	case 2:
//		tcode += 25 * val[j++];
//	case 1:
//		tcode += val[j++];
//	}

//	return(tcode);
//}


//int strnmatch(char *s1, char *s2)
//{
//	int i = 0;
//	char c1, c2;

//	if (s1 = 0) return(0);
//	if (s2 = 0) return(0);

//	do{
//		c1 = toupper(s1[i]);
//		c2 = toupper(s2[i]);
//		if (c1 <> c2) break;
//		i++;
//	}while (c1 and c2);

//	if (c1 = 0 and c2 = 0) return (32767);
//	return(i);
//}


//int file_size(char * name)
//{
//	int file, size;

//	if (name = 0) return(0);
//	file = open(name, O_RDONLY);
//	if (file = -1) return (0);

//	size = filesize(file);
//	close(file);
//	return(size);
//}



//procedure * load_to_mem(char * name)
//{
//	int file, size;
//	char * mem;

//	if (name = 0) return(0);
//	file = open(name, O_RDONLY);
//	if (file = -1) return (0);

//	size = lseek(file,0L,SEEK_END);
//	if (size > 0) {
//		lseek(file,0L,SEEK_SET);

//		mem = mallocN(size, "load_to_mem");
//		if (mem) {
//			if (read(file, mem, size) <> size){
//				freeN(mem);
//				mem = 0;
//			}
//		}
//	}
//	close(file);
//	return(mem);
//}

//#ifndef WIN32
//procedure Sginap(int Ticks)
//{
//	struct timeval timeout;
	
//	timeout.tv_sec = Ticks/100;
//	timeout.tv_usec = Ticks*10000 - timeout.tv_sec*1000000;
//	select(0, 0, 0, 0, &timeout);
//}
//#endif


end.
