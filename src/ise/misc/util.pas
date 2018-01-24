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
unit util;

interface

//{$ifndef UTIL_H}
//{$define UTIL_H}
//{$include <sys/types.h>}
//{$include <stdlib.h>}
//{$include <string.h>}
//{$ifdef debug}
//{$include <malloc.h>}
//{$endif}
//{$ifndef NULL}
//const
//NULL =nil;
//{$endif}
//{$ifndef FALSE}
//FALSE =nil;
//{$endif}
//{$ifndef TRUE}
//TRUE = 1;
//{$endif}
//{$ifndef ulong}
//ulong = unsignedlong;
//{$endif}
//{$ifndef ushort}
//ushort = unsignedshort;
//{$endif}
//{$ifndef uchar}
//uchar = unsignedchar;
//{$endif}
//{$ifndef uint}
//uint = unsignedint;
//{$endif}
//
//function mallocstruct(x: integer; y: integer): integer; {<= !!!6 unknown macro}
//
//function callocstruct(x: integer; y: integer): integer; {<= !!!6 unknown macro}
//
//function mallocstructN(x: integer; y: integer; name: integer): integer; {<= !!!6 unknown macro}
//
//function callocstructN(x: integer; y: integer; name: integer): integer; {<= !!!6 unknown macro}
//
//function RMK(x: integer): integer; {<= !!!6 unknown macro}
//
//function ELEM(a: integer; b: integer; c: integer): integer; {<= !!!6 unknown macro}
//
//function ELEM3(a: integer; b: integer; c: integer; d: integer): integer; {<= !!!6 unknown macro}
//
//function ELEM4(a: integer; b: integer; c: integer; d: integer; e: integer): integer; {<= !!!6 unknown macro}
//
//function ELEM5(a: integer; b: integer; c: integer; d: integer; e: integer; f: integer): integer; {<= !!!6 unknown macro}
//
//function ELEM6(a: integer; b: integer; c: integer; d: integer; e: integer; f: integer; g: integer): integer; {<= !!!6 unknown macro}
//
//function ELEM7(a: integer; b: integer; c: integer; d: integer; e: integer; f: integer; g: integer; h: integer): integer; {<= !!!6 unknown macro}
//
//function ELEM8(a: integer; b: integer; c: integer; d: integer; e: integer; f: integer; g: integer; h: integer; i: integer): integer; {<= !!!6 unknown macro}
//
//function STREQ(str: integer; a: integer): integer; {<= !!!6 unknown macro}
//
//function STREQ2(str: integer; a: integer; b: integer): integer; {<= !!!6 unknown macro}
//
//function STREQ3(str: integer; a: integer; b: integer; c: integer): integer; {<= !!!6 unknown macro}
//
//function MIN2(x: integer; y: integer): integer; {<= !!!6 unknown macro}
//
//function MIN3(x: integer; y: integer; z: integer): integer; {<= !!!6 unknown macro}
//
//function MIN4(x: integer; y: integer; z: integer; a: integer): integer; {<= !!!6 unknown macro}
//
//function MAX2(x: integer; y: integer): integer; {<= !!!6 unknown macro}
//
//function MAX3(x: integer; y: integer; z: integer): integer; {<= !!!6 unknown macro}
//
//function MAX4(x: integer; y: integer; z: integer; a: integer): integer; {<= !!!6 unknown macro}
//
//function SWAP(type: integer; a: integer; b: integer): integer; {<= !!!6 unknown macro}
//{$ifndef ABS}
//
//function ABS(x: integer): integer; {<= !!!6 unknown macro}
//{$endif}
//{$ifdef AMIGA}
//
//function GL(x: integer): integer; {<= !!!6 unknown macro}
//{$else}
//
//function GL(x: integer): integer; {<= !!!6 unknown macro}
//{$endif}
//
//function GS(x: integer): integer; {<= !!!6 unknown macro}
//
//function GSS(x: integer): integer; {<= !!!6 unknown macro}
//
//function SWAP_L(x: integer): integer; {<= !!!6 unknown macro}
//
//function SWAP_S(x: integer): integer; {<= !!!6 unknown macro}
//
//function SWAP_SHORT(x: integer): integer; {<= !!!6 unknown macro}
//
//function SWAP_LONG(x: integer): integer; {<= !!!6 unknown macro}
//
//function ENDIAN_NOP(x: integer): integer; {<= !!!6 unknown macro}
//
//function GET_BIG_LONG(x: integer): integer; {<= !!!6 unknown macro}
//
//function GET_LITTLE_LONG(x: integer): integer; {<= !!!6 unknown macro}
//{$if defined( __sgi) || defined (__SUN) || defined (__PPC__)}
//
//function MAKE_ID(a: integer; b: integer; c: integer; d: integer): integer; {<= !!!6 unknown macro}
//GET_ID = GET_BIG_LONG;
//LITTLE_SHORT = SWAP_SHORT;
//LITTLE_LONG = SWAP_LONG;
//BIG_SHORT = ENDIAN_NOP;
//BIG_LONG = ENDIAN_NOP;
//{$else}
//
//function MAKE_ID(a: integer; b: integer; c: integer; d: integer): integer; {<= !!!6 unknown macro}
//GET_ID = GET_LITTLE_LONG;
//LITTLE_SHORT = ENDIAN_NOP;
//LITTLE_LONG = ENDIAN_NOP;
//BIG_SHORT = SWAP_SHORT;
//BIG_LONG = SWAP_LONG;
//{$endif}
//{$ifdef __ANSI_CPP__}
//(* ansi *)
//
//function NEW(x: integer): integer; {<= !!!6 unknown macro}
//
//function CLN(x: integer): integer; {<= !!!6 unknown macro}
//
//function PRINT(d: integer; var1: integer): integer; {<= !!!6 unknown macro}
//
//function PRINT2(d: integer; e: integer; var1: integer; var2: integer): integer; {<= !!!6 unknown macro}
//
//function PRINT3(d: integer; e: integer; f: integer; var1: integer; var2: integer; var3: integer): integer; {<= !!!6 unknown macro}
//
//function PRINT4(d: integer; e: integer; f: integer; g: integer; var1: integer; var2: integer; var3: integer; var4: integer): integer; {<= !!!6 unknown macro}
//{$else}
//(* -cckr *)
//
//function NEW(x: integer): integer; {<= !!!6 unknown macro}
//
//function CLN(x: integer): integer; {<= !!!6 unknown macro}
//
//function PRINT(d: integer; var1: integer): integer; {<= !!!6 unknown macro}
//
//function PRINT2(d: integer; e: integer; var1: integer; var2: integer): integer; {<= !!!6 unknown macro}
//
//function PRINT3(d: integer; e: integer; f: integer; var1: integer; var2: integer; var3: integer): integer; {<= !!!6 unknown macro}
//
//function PRINT4(d: integer; e: integer; f: integer; g: integer; var1: integer; var2: integer; var3: integer; var4: integer): integer; {<= !!!6 unknown macro}
//{$endif}

type
pLink = ^Link;
Link = record
next: pLink;
prev: pLink;
end;

(* never change the size of this! genfile.c detects pointerlen with it *)
  pListBase = ^ListBase;
ListBase = record
first: pointer;
last: pointer;
end;

//(* 8 byte alignment! *)
pMemHead = ^MemHead;
MemHead = record
tag1: integer;
len: integer;
next: pMemHead;
prev: pMemHead;
name: pchar;
nextname: pchar;
level: integer;
tag2: integer;
end;

pMemTail = ^MemTail;
MemTail = record
tag3: integer;
pad: integer;
end;

const
{$IFDEF ENDIAN_LITTLE}
  MEMTAG1 = byte('O') shl 24 or byte('M') shl 16 or byte('E') shl 8 or byte('M');
  MEMTAG2 = byte('L') shl 24 or byte('B') shl 16 or byte('Y') shl 8 or byte('R');
  MEMTAG3 = byte('!') shl 24 or byte('K') shl 16 or byte('C') shl 8 or byte('O');
  MEMFREE = byte('E') shl 24 or byte('E') shl 16 or byte('R') shl 8 or byte('F');
{$ELSE}
  MEMTAG1 = byte('M') shl 24 or byte('E') shl 16 or byte('M') shl 8 or byte('O');
  MEMTAG2 = byte('R') shl 24 or byte('Y') shl 16 or byte('B') shl 8 or byte('L');
  MEMTAG3 = byte('O') shl 24 or byte('C') shl 16 or byte('K') shl 8 or byte('!');
  MEMFREE = byte('F') shl 24 or byte('R') shl 16 or byte('E') shl 8 or byte('E');
{$ENDIF}

//function MEMNEXT(x: integer): integer; {<= !!!6 unknown macro}
//{totblock: integer; }{<= !!!5 external variable}
//{mem_in_use: integer; }{<= !!!5 external variable}
//{$ifdef debug}
//
//function malloc(x: integer): integer; {<= !!!6 unknown macro}
//
//function free(x: integer): integer; {<= !!!6 unknown macro}
//
//function calloc(x: integer; y: integer): integer; {<= !!!6 unknown macro}
//{$endif}
//(* ******* util.c *)
//
//procedure Sginap(Ticks: integer);

procedure addhead(listbase: pListBase;  vlink: pointer);

procedure addtail(listbase: pListBase;  vlink: pointer);

//procedure ap_framelen(cmd: pchar;  i: integer);

function callocN(len: integer;  str: pchar): pointer;

function check_memlist(memh: pMemHead): pchar;

//function countlist(listbase: pListBase): integer;
//
//function file_size(name: pchar): integer;
//
//function fileselect(title: pchar;  dir: pchar): smallint;

function freeN(vmemh: pointer): smallint;

//procedure freelinkN(listbase: pListBase;  vlink: pinteger);
//
//procedure freelist(listbase: pListBase);

procedure freelistN(listbase: pListBase);

//procedure insertlink(listbase: pListBase;  vprevlink: pinteger;  vnewlink: pinteger);

procedure insertlinkbefore(listbase: pListBase;  vnextlink: pointer;  vnewlink: pointer);

//procedure load_to_mem(name: pchar);

function mallocN(len: integer;  str: pchar): pointer;

//procedure newname(name: pchar;  add: integer);

procedure printmemlist;

procedure rem_memblock(memh: pMemHead);

procedure remlink(listbase: pListBase;  vlink: pointer);

//function string_to_tcode(str: pchar): integer;
//
//function stringdec(string: pchar;  kop: pchar;  staart: pchar;  numlen: pushort): integer;
//
//procedure stringenc(string: pchar;  kop: pchar;  staart: pchar;  numlen: ushort;  pic: integer);
//
//function stringselect(title: pchar;  file: pchar): smallint;
//
//function strnmatch(s1: pchar;  s2: pchar): integer;
//
//function tcode_to_string(len: integer): pchar;
//{$endif}
(* UTIL_H *)

implementation

uses
cfuncs;

//{$include <stdio.h>}
//{$include "util.h"}
//{$include <fcntl.h>}
//{$ifdef WIN32}
//{$include "winstuff.h"}
//{$endif}
//{$ifndef WIN32}
//{$include <sys/time.h>}
//{$endif}

var
totblock: integer =0;
mem_in_use: integer =0;
current_mem_level: integer =0;

var
_membase: ListBase;
 membase : pListBase= @_membase;

//#define MEMNEXT(x) ((MemHead *)(((char *) x) - ((char *) & (((MemHead *)0)->next))))
function MEMNEXT(x: pointer): pMemHead;
begin
  MEMNEXT:=pMemHead((pchar(x))-(pchar(@((pMemHead(0))^.next))));
end;

//function stringdec(string: pchar;  kop: pchar;  staart: pchar;  numlen: pushort): integer; 
//var
//len: ushort; 
//len2: ushort; 
//nums: ushort; 
//nume: ushort; 
//i: smallint; 
//found: smallint;
// 
//begin
//  
//  
//  
//  
//  
//  found:=LongBool(0); 
//  len2:= len:=lstrlen(string); 
//  if len>6
//  then
//  begin 
//    if strncasecmp(string+len-6,'.blend',6)=nil
//    then
//    len:= len - (6); 
//    else
//    if strncasecmp(string+len-6,'.trace',6)=nil
//    then
//    len:= len - (6); 
//  end;
//  if len=len2
//  then
//  begin 
//    if len>4
//    then
//    begin 
//      (* .jf0 en .jf1 voor jstreams afvangen *)
//      if strncasecmp(string+len-4,'.jf',3)=nil
//      then
//      len:= len - (4); 
//      else
//      if strncasecmp(string+len-4,'.tga',4)=nil
//      then
//      len:= len - (4); 
//      else
//      if strncasecmp(string+len-4,'.jpg',4)=nil
//      then
//      len:= len - (4); 
//      else
//      if strncasecmp(string+len-4,'.txt',4)=nil
//      then
//      len:= len - (4); 
//      else
//      if strncasecmp(string+len-4,'.cyc',4)=nil
//      then
//      len:= len - (4); 
//      else
//      if strncasecmp(string+len-4,'.enh',4)=nil
//      then
//      len:= len - (4); 
//      else
//      if strncasecmp(string+len-4,'.rgb',4)=nil
//      then
//      len:= len - (4); 
//      else
//      if strncasecmp(string+len-4,'.psx',4)=nil
//      then
//      len:= len - (4); 
//    end;
//  end;
//  for{while} i:=len-1 downto 0 { i--}
//  do
//  begin 
//    if string[i]='/'
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    if isdigit(string[i])
//    then
//    begin 
//      if found<>nil 
//      then
//      begin 
//        nums:= i; 
//      end;
//      else
//      begin 
//        nume:= i; 
//        nums:= i; 
//        found:= LongBool(1); 
//      end;
//    end;
//    else
//    begin 
//      if found<>nil 
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//  end;
//  if found<>nil 
//  then
//  begin 
//    if staart<>nil 
//    then
//    strcpy(staart,@string[nume+1]); 
//    if kop<>nil 
//    then
//    begin 
//      strcpy(kop,string); 
//      kop[nums]:=nil; 
//    end;
//    if numlen<>nil 
//    then
//    {*}numlen^:=nume-nums+1; 
//    begin
//      result:= ({!!!a type cast? =>} {integer(}atoi(@(string[nums]))); 
//      exit;
//    end;
//  end;
//  if staart<>nil 
//  then
//  strcpy(staart,string+len); 
//  if kop<>nil 
//  then
//  begin 
//    strncpy(kop,string,len); 
//    kop[len]:=nil; 
//  end;
//  if numlen<>nil 
//  then
//  {*}numlen^:=nil; 
//  begin
//    result:=nil; 
//    exit;
//  end;
//end;
//
//procedure stringenc(string: pchar;  kop: pchar;  staart: pchar;  numlen: ushort;  pic: integer); 
//var
//numstr: array [0..Pred(10)] of char;
// 
//len: ushort; 
//i: ushort; 
//begin
//  numstr:=#0; 
//  
//  
//  strcpy(string,kop); 
//  if pic>0)or(numlen=4
//  then
//  begin 
//    len:= sprintf(numstr,'%d',pic); 
//    for{while} i:=len to Pred(numlen) {i++}
//    do
//    begin 
//      strcat(string,'0'); 
//    end;
//    strcat(string,numstr); 
//  end;
//  strcat(string,staart); 
//end;
//
//procedure newname(name: pchar;  add: integer); 
//var
//head: array [0..Pred(128)] of char; 
//tail: array [0..Pred(128)] of char; 
//pic: integer; 
//digits: ushort; 
//i: integer; 
//exp: integer; 
//begin
//  
//  
//  
//  
//  pic:= stringdec(name,head,tail, and digits); (* gaan we van 100 -> 99 of van 10 naar 9 *)
//  if add<0)and(digits<4)and(digits>0
//  then
//  begin 
//    
//    
//    exp:= 1; 
//    for{while} i:=digits downto Succ(1) { i--}
//    do
//    exp:= exp * (10); 
//    if pic>=exp)and((pic+add)<exp
//    then
//    dec(digits); 
//  end;
//  pic:= pic + (add); 
//  if digits=4)and(pic<0
//  then
//  pic:=nil; 
//  stringenc(name,head,tail,digits,pic); 
//end;
//type
//Link = record
//end;

procedure addhead(listbase: pListBase;  vlink: pointer);
var
 link: pLink;
begin
  link:= vlink;

  if link=nil  then
  exit;

  if listbase=nil  then
  exit;

  link^.next:= listbase^.first;
  link^.prev:=nil;

  if listbase^.first<>nil  then
  pLink(listbase^.first)^.prev:=link;

  if listbase^.last=nil  then
  listbase^.last:= link;

  listbase^.first:= link;
end;
//type
//Link = record
//end;

procedure addtail(listbase: pListBase;  vlink: pointer);
var
 link: pLink;
begin
  link:= vlink;

  if link=nil  then
  exit;

  if listbase=nil  then
  exit;

  link^.next:= nil;
  link^.prev:= listbase^.last;
  if listbase^.last<>nil  then
  pLink(listbase^.last)^.next:=link;

  if listbase^.first=nil then
  listbase^.first:= link;

  listbase^.last:= link;
end;

//type
//Link = record
//end;

procedure remlink(listbase: pListBase;  vlink: pointer);
var
 link: pLink;
begin
  link:= vlink;

  if link=nil  then
  exit;

  if listbase=nil   then
  exit;

  if link^.next<>nil  then
  link^.next^.prev:= link^.prev;

  if link^.prev<>nil  then
  link^.prev^.next:= link^.next;

  if listbase^.last=link then
  listbase^.last:= link^.prev;

  if listbase^.first=link then
  listbase^.first:= link^.next;
end;

//type
//Link = record
//end;
//
//procedure freelinkN(listbase: pListBase;  vlink: pinteger); 
//begin
//  
//  if link=nil
//  then
//  exit;
//  if listbase=nil
//  then
//  exit;
//  remlink(listbase,link); 
//  freeN(link); 
//end;
//type
//Link = record
//end;(* newlink komt na prevlink *)
//
//procedure insertlink(listbase: pListBase;  vprevlink: pinteger;  vnewlink: pinteger); 
//begin
//  if newlink=nil
//  then
//  exit;
//  if listbase=nil
//  then
//  exit;
//  if listbase^.first=nil
//  then
//  begin 
//    (* lege lijst *)
//    listbase^.first:= newlink; 
//    listbase^.last:= newlink; 
//    exit;
//  end;
//  if prevlink=nil
//  then
//  begin 
//    (* inserten voor eerste element *)
//    newlink^.next:= listbase^.first; 
//    newlink^.prev:=nil; 
//    newlink^.next^.prev:= newlink; 
//    listbase^.first:= newlink; 
//    exit;
//  end;
//  if listbase^.last=prevlink
//  then
//  listbase^.last:= newlink; 
//  newlink^.next:= prevlink^.next; 
//  prevlink^.next:= newlink; (* aan einde lijst *)
//  if newlink^.next<>nil 
//  then
//  newlink^.next^.prev:= newlink; 
//  newlink^.prev:= prevlink; 
//end;
//type
//Link = record
//end;

procedure insertlinkbefore(listbase: pListBase;  vnextlink: pointer;  vnewlink: pointer);
var
nextlink, newlink: pLink;
begin
  	nextlink:= vnextlink;
        newlink:= vnewlink;

        (* newlink komt voor nextlink *)

  if newlink=nil   then
  exit;
  if listbase=nil  then
  exit;

  (* lege lijst *)
  if listbase^.first=nil   then
  begin

    listbase^.first:= newlink;
    listbase^.last:= newlink;
    exit;
  end;

  (* inserten aan einde lijst *)
  if nextlink=nil then
  begin
    newlink^.prev:= listbase^.last;
    newlink^.next:=nil;
    pLink(listbase^.last)^.next:=newlink;
    listbase^.last:= newlink;
    exit;
  end;

  (* aan begin lijst *)
  if listbase^.first=nextlink    then
  listbase^.first:= newlink;

  newlink^.next:= nextlink;
  newlink^.prev:= nextlink^.prev;
  nextlink^.prev:= newlink;

  if newlink^.prev<>nil   then
  newlink^.prev^.next:= newlink;
end;
//type
//Link = record
//end;
//
//procedure freelist(listbase: pListBase); 
//begin
//  
//  if listbase=nil
//  then
//  exit;
//  link:= listbase^.first; 
//  while link
//  do
//  begin 
//    next:= link^.next; 
//    free(link); 
//    link:= next; 
//  end;
//  listbase^.first:=nil; 
//  listbase^.last:=nil; 
//end;
//type
//Link = record
//end;

procedure freelistN(listbase: pListBase);
var
 link,next: pLink;
begin

  if listbase=nil then
  exit;

  link:= listbase^.first;
  while link<>nil  do
  begin
    next:= link^.next;
    freeN(link);
    link:= next;
  end;
  listbase^.first:=nil;
  listbase^.last:=nil;
end;

//function countlist(listbase: pListBase): integer; 
//var
//link: pLink; 
//count: integer;
// 
//begin
//  
//  count:=nil; 
//  if listbase<>nil 
//  then
//  begin 
//    link:= listbase^.first; 
//    while link
//    do
//    begin 
//      inc(count); 
//      link:= link^.next; 
//    end;
//  end;
//  begin
//    result:= (count); 
//    exit;
//  end;
//end;
//{$ifndef WIN32}
//
//function fileselect(title: pchar;  dir: pchar): smallint; 
//var
//buf: array [0..Pred(200)] of char; 
//fspipe: array [0..Pred(2)] of integer; 
//len: integer; 
//ok: integer; 
//begin
//  
//  
//  
//  
//  if pipe(fspipe)
//  then
//  begin
//    result:= (0); 
//    exit;
//    (*sprintf(buf,"fs '%s' '%d' '%s'",dir,fspipe[1],title);*)
//  end;
//  sprintf(buf,'fs -d '%s' -p '%d' -t '%s'',dir,fspipe[1],title);
//  len:= ok:=nil; 
//  if system(buf)=nil
//  then
//  begin 
//    if read(fspipe[0],@len,4)=4
//    then
//    begin 
//      if read(fspipe[0],buf,len)=len
//      then
//      begin 
//        if len>1
//        then
//        begin 
//          strcpy(dir,buf); 
//          ok:= 1; 
//        end;
//      end;
//    end;
//  end;
//  close(fspipe[0]); 
//  close(fspipe[1]); 
//  begin
//    result:= (ok); 
//    exit;
//  end;
//end;
//
//function stringselect(title: pchar;  file: pchar): smallint; 
//var
//buf: array [0..Pred(200)] of char; 
//fspipe: array [0..Pred(2)] of integer; 
//len: integer; 
//ok: integer; 
//begin
//  
//  
//  
//  
//  if pipe(fspipe)
//  then
//  begin
//    result:= (0); 
//    exit;
//  end;
//  sprintf(buf,'fs -f '%s' -s -p '%d' -t '%s'',file,fspipe[1],title);
//  len:= ok:=nil; 
//  if system(buf)=nil
//  then
//  begin 
//    if read(fspipe[0],@len,4)=4
//    then
//    begin 
//      if read(fspipe[0],buf,len)=len
//      then
//      begin 
//        if len>1
//        then
//        begin 
//          strcpy(file,buf); 
//          ok:= 1; 
//        end;
//      end;
//    end;
//  end;
//  close(fspipe[0]); 
//  close(fspipe[1]); 
//  begin
//    result:= (ok); 
//    exit;
//  end;
//end;
//{$endif}
//{$ifdef malloc}
//{$undef malloc}
//{$endif}
//{$ifdef calloc}
//{$undef calloc}
//{$endif}
//{$ifdef free}
//{$undef free}
//{$endif}

procedure _MemorY_ErroR(block, error: pchar);
begin
  fprintf(stderr,'Memoryblock %s: %s\n',[block,error]);
end;

var
memory_error: procedure(block, error: pchar) = @_MemorY_ErroR;

function mallocN(len: integer;  str: pchar): pointer;
var
memh: pMemHead;
memt: pMemTail;
begin
  if len<=0  then
  begin
    printf('Malloc error: len=%d in %s\n',[len,str]);
          exit(nil);
  end;

  if sizeof(longint)=8  then
  len:= (len+3) and  not 3      (* eenheden van 4 *)
  else
  len:= (len+7) and  not 7;      (* eenheden van 8 *)

  memh:= pMemHead(malloc(len+sizeof(MemHead)+sizeof(MemTail)));

  if memh<>nil  then
  begin
    memh^.tag1:= MEMTAG1;
    memh^.name:= str;
    memh^.nextname:= #0;
    memh^.len:= len;
    memh^.level:= current_mem_level;
    memh^.tag2:= MEMTAG2;
    memt:= pMemTail(pchar(memh)+sizeof(MemHead)+len);
    memt^.tag3:= MEMTAG3;
    addtail(membase,@memh^.next);

    if memh^.next<>nil    then
    memh^.nextname:= MEMNEXT(memh^.next)^.name;

    inc(totblock);
    mem_in_use:= mem_in_use + (len);
      inc(memh);

      exit(memh);
  end;

  printf('Malloc returns nill: len=%d in %s\n',[len,str]);
      exit(nil);
end;

function callocN(len: integer;  str: pchar): pointer;
var
memh: pMemHead;
memt: pMemTail;
begin
  if len<=0  then
  begin
    printf('Malloc error: len=%d in %s\n',[len,str]);
    exit(nil);
  end;

  if sizeof(longint)=8 then
  len:= (len+3) and  not 3   (* eenheden van 4 *)
  else
  len:= (len+7) and  not 7;   (* eenheden van 8 *)

  memh:= pMemHead(calloc(len+sizeof(MemHead)+sizeof(MemTail),1));

  if memh<>nil  then
  begin
    memh^.tag1:= MEMTAG1;
    memh^.name:= str;
    memh^.nextname:=nil;
    memh^.len:= len;
    memh^.level:= current_mem_level;
    memh^.tag2:= MEMTAG2;
    memt:= pMemTail(pchar(memh)+sizeof(MemHead)+len);
    memt^.tag3:= MEMTAG3;
    addtail(membase,@memh^.next);

        if memh^.next<>nil    then
        memh^.nextname:= MEMNEXT(memh^.next)^.name;

        inc(totblock);
        mem_in_use:= mem_in_use + len;
          inc(memh);

          exit(memh);
  end;

  printf('Calloc returns nill: len=%d in %s\n',[len,str]);
  exit(nil);
end;

procedure rem_memblock(memh: pMemHead);
begin
  remlink(membase,@memh^.next);
  if memh^.prev<>nil  then
  begin
    if memh^.next<>nil    then
    MEMNEXT(memh^.prev)^.nextname:=MEMNEXT(memh^.next)^.name
    else
    MEMNEXT(memh^.prev)^.nextname:=#0;
  end;
  dec(totblock);
  mem_in_use:= mem_in_use - memh^.len;
  free(memh);
end;

procedure printmemlist;
var
membl: pMemHead;
begin
  printf('total memory used:   %d bytes\n',[mem_in_use]);
  printf('total memory blocks: %d\n\n',[totblock]);

  membl:= membase^.first;
  if membl<>nil    then
  membl:= MEMNEXT(membl);

  while membl <>nil do
  begin
    printf('%24s len: %8d %p\n',[membl^.name,membl^.len,membl+1]);
    if membl^.next<>nil    then
    membl:= MEMNEXT(membl^.next)
    else
    break;
  end;
end;

function check_memlist(memh: pMemHead): pchar;
var
forw: pMemHead;
back: pMemHead;
forwok: pMemHead;
backok: pMemHead;
name: pchar;
begin
  forw:= membase^.first;
  if forw<>nil  then
  forw:= MEMNEXT(forw);

  forwok:= nil;
  while forw <>nil  do
  begin
    if (forw^.tag1<>MEMTAG1)or(forw^.tag2<>MEMTAG2)    then
    break;
    forwok:= forw;
    if forw^.next<>nil    then
    forw:= MEMNEXT(forw^.next)
    else
    forw:= nil;
  end;

  back:= pMemHead(membase^.last);
  if back<>nil  then
  back:= MEMNEXT(back);
  backok:= nil;
  while back<>nil  do
  begin
    if (back^.tag1<>MEMTAG1)or(back^.tag2<>MEMTAG2)    then
    break;

    backok:= back;
    if back^.prev<>nil    then
    back:= MEMNEXT(back^.prev)
    else
    back:= nil;
  end;

  if forw<>back then
    exit('MORE THAN 1 MEMORYBLOCK CORRUPT');

  if (forw=nil)and(back=nil)  then
  begin
    (* geen foute headers gevonden dan maar op zoek naar memblock*)
    forw:= membase^.first;

    if forw<>nil    then
    forw:= MEMNEXT(forw);

    forwok:= nil;
    while forw<>nil    do
    begin
      if forw=memh     then
      break;

      if (forw^.tag1<>MEMTAG1)or(forw^.tag2<>MEMTAG2)      then
      break;

      forwok:= forw;
      if forw^.next<>nil      then
      forw:= MEMNEXT(forw^.next)
      else
      forw:=nil;
    end;

    if forw=nil    then
    exit(nil);

    back:= pMemHead(membase^.last);
    if back<>nil    then
    back:= MEMNEXT(back);
    backok:= nil;
    while back <>nil    do
    begin
      if back=memh        then
      break;

      if (back^.tag1<>MEMTAG1)or(back^.tag2<>MEMTAG2)      then
      break;

      backok:= back;
      if back^.prev<>nil      then
      back:= MEMNEXT(back^.prev)
      else
      back:= nil;
    end;
  end;
  if forwok<>nil  then
  name:= forwok^.nextname
  else
  name:= 'No name found';

  if forw=memh  then
  begin
    (* voor alle zekerheid wordt dit block maar uit de lijst gehaald *)
    if forwok<>nil
    then
    begin
      if backok<>nil
      then
      begin
        forwok^.next:=  backok^.next;
        backok^.prev:=  forwok^.next;
        forwok^.nextname:= backok^.name;
      end
      else
      begin
        forwok^.next:= nil;
        membase^.last:= forwok^.next;
      end;
    end
    else
    begin
      if backok<>nil      then
      begin
        backok^.prev:= nil;
        membase^.first:=   backok^.next;
      end
      else
      begin
        membase^.last:=nil;
        membase^.first:=nil;
      end;
    end;
  end
  else
  begin
    memory_error(name,'Aditional error in header');
    exit(nil);
  end;
  exit(name);
end;

function freeN(vmemh: pointer): smallint; (* anders compileertie niet meer *)
var
error: smallint=0;
memt: pMemTail;
memh: pMemHead;
name: pchar;
begin
  memh:=vmemh;

  if memh=nil  then
  begin
    memory_error('free','attempt to free NULL pointer');
      exit(-1);
   end;
  if sizeof(longint)=8 then
  begin
    if (integer(memh) and $7) <>0    then
    begin
      memory_error('free','attempt to free illegal pointer');
      exit(-1);
    end;
  end
  else
  begin
    if (integer(memh) and $3) <>0    then
    begin
      memory_error('free','attempt to free illegal pointer');
      exit(-1);
    end;
  end;

  dec(memh);
  if (memh^.tag1=MEMFREE)and(memh^.tag2=MEMFREE)  then
  begin
    memory_error(memh^.name,'double free');
    exit(-1);
  end;
  if (memh^.tag1=MEMTAG1)and(memh^.tag2=MEMTAG2)and((memh^.len and $3)=0)  then
  begin
    memt:= pMemTail(pchar(memh)+sizeof(MemHead)+memh^.len);
    if memt^.tag3=MEMTAG3
    then
    begin
      memh^.tag1:= MEMFREE;
      memh^.tag2:= MEMFREE;
      memt^.tag3:= MEMFREE;

      (* na tags !!! *)
      rem_memblock(memh);
      exit(0);
    end;
    error:= 2;
    memory_error(memh^.name,'end corrupt');
    name:= check_memlist(memh);
    if name<>nil    then
    begin
      if name<>memh^.name      then
      memory_error(name,'is also corrupt');
    end;
  end
  else
  begin
    error:= -1;
    name:= check_memlist(memh);
    if name=nil    then
    memory_error('free','pointer not in memlist')
    else
    memory_error(name,'error in header');
  end;
  dec(totblock);
  (* hier moet een DUMP plaatsvinden *)
  exit(error);
end;

//procedure ap_framelen(cmd: pchar;  i: integer); 
//var
//buf: array [0..Pred(8)] of char; 
//begin
//  
//  if i<10
//  then
//  sprintf(buf,'0%d',i); 
//  else
//  sprintf(buf,'%d',i); 
//  strcat(cmd,buf); 
//end;
//var {was static}
//str: array [0..Pred(64)] of char; 
//
//function tcode_to_string(len: integer): pchar; 
//var
//i: integer; 
//rlen: integer; 
//begin
//  
//  
//  
//  if len<0
//  then
//  begin 
//    strcpy(str,'- '); 
//    len:= -len; 
//  end;
//  else
//  begin 
//    str[0]:=nil; 
//  end;
//  rlen:= len; 
//  if len>=90000
//  then
//  begin 
//    i:= rlen div 90000; 
//    ap_framelen(str,i); 
//    strcat(str,':'); 
//    rlen:= rlen - (90000*i); 
//  end;
//  if len>=1500
//  then
//  begin 
//    i:= rlen div 1500; 
//    ap_framelen(str,i); 
//    strcat(str,':'); 
//    rlen:= rlen - (1500*i); 
//  end;
//  if len>=25
//  then
//  begin 
//    i:= rlen div 25; 
//    ap_framelen(str,i); 
//    strcat(str,':'); 
//    rlen:= rlen - (25*i); 
//  end;
//  ap_framelen(str,rlen); 
//  begin
//    result:= (str); 
//    exit;
//  end;
//end;
//
//function string_to_tcode(str: pchar): integer; 
//var
//i: integer; 
//val: array [0..Pred(4)] of integer; 
//tcode: integer;
// 
//j: integer;
// 
//string: array [0..Pred(64)] of char; 
//begin
//  
//  
//  tcode:=nil; 
//  j:=nil; 
//  
//  strcpy(string,str); 
//  while{for} 
//  i:=nil; 
//  {to} string[i]
//  {;}
//  inc(i); 
//  do
//  begin 
//    if isdigit(string[i])=nil
//    then
//    string[i]:= ' '; 
//  end;
//  i:= sscanf(string,'%ld%ld%ld%ld',val,val+1,val+2,val+3); 
//  case i of
//    4:
//    begin
//      tcode:= tcode + (90000*val[j{++} inc(j); ]); 
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    3:
//    begin
//      tcode:= tcode + (1500*val[j{++} inc(j); ]); 
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    2:
//    begin
//      tcode:= tcode + (25*val[j{++} inc(j); ]); 
//      {goto next_label;}{<= !!!d case label without "break"}
//    end;
//    1:
//    begin
//      tcode:= tcode + (val[j{++} inc(j); ]); 
//    end;
//  end;{case?}
//  begin
//    result:= (tcode); 
//    exit;
//  end;
//end;
//
//function strnmatch(s1: pchar;  s2: pchar): integer; 
//var
//i: integer;
// 
//c1: char; 
//c2: char; 
//begin
//  i:=nil; 
//  
//  
//  if s1=nil
//  then
//  begin
//    result:= (0); 
//    exit;
//  end;
//  if s2=nil
//  then
//  begin
//    result:= (0); 
//    exit;
//  end;
//  repeat
//  begin 
//    c1:= toupper(s1[i]); 
//    c2:= toupper(s2[i]); 
//    if c1<>c2
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    inc(i); 
//  end;
//  until not {0=}(c1)and(c2);
//  if c1=nil)and(c2=nil
//  then
//  begin
//    result:= (32767); 
//    exit;
//  end;
//  begin
//    result:= (i); 
//    exit;
//  end;
//end;
//
//function file_size(name: pchar): integer; 
//var
//file: integer; 
//size: integer; 
//begin
//  
//  
//  if name=nil
//  then
//  begin
//    result:= (0); 
//    exit;
//  end;
//  file:= open(name,O_RDONLY); 
//  if file=-1
//  then
//  begin
//    result:= (0); 
//    exit;
//  end;
//  size:= filesize(file); 
//  close(file); 
//  begin
//    result:= (size); 
//    exit;
//  end;
//end;
//
//procedure load_to_mem(name: pchar); 
//var
//file: integer; 
//size: integer; 
//mem: pchar; 
//begin
//  
//  
//  
//  if name=nil
//  then
//  begin
//    result:= (0); 
//    exit;
//  end;
//  file:= open(name,O_RDONLY); 
//  if file=-1
//  then
//  begin
//    result:= (0); 
//    exit;
//  end;
//  size:= lseek(file,0,SEEK_END); 
//  if size>0
//  then
//  begin 
//    lseek(file,0,SEEK_SET); 
//    mem:= mallocN(size,'load_to_mem'); 
//    if mem<>nil 
//    then
//    begin 
//      if read(file,mem,size)<>size
//      then
//      begin 
//        freeN(mem); 
//        mem:=nil; 
//      end;
//    end;
//  end;
//  close(file); 
//  begin
//    result:= (mem); 
//    exit;
//  end;
//end;
//{$ifndef WIN32}
//type
//timeval = record
//end;
//
//procedure Sginap(Ticks: integer); 
//begin
//  
//  timeout.tv_sec:= Ticks div 100; 
//  timeout.tv_usec:= Ticks*10000-timeout.tv_sec*1000000; 
//  select(0,0,0,0,@timeout); 
//end;
//{$endif}

//{mallocstruct(x,y) (x* )malloc((y)* sizeof(x))}
//function mallocstruct(x: integer; y: integer): integer;
//begin
//  result:= {!!!a type cast? =>} {px(}malloc((y)*sizeof(x))
//end;
//
//{callocstruct(x,y) (x* )calloc((y), sizeof(x))}
//function callocstruct(x: integer; y: integer): integer;
//begin
//  result:= {!!!a type cast? =>} {px(}calloc((y),sizeof(x))
//end;
//
//{mallocstructN(x,y,name) (x* )mallocN((y)* sizeof(x),name)}
//function mallocstructN(x: integer; y: integer; name: integer): integer;
//begin
//  result:= {!!!a type cast? =>} {px(}mallocN((y)*sizeof(x),name)
//end;
//
//{callocstructN(x,y,name) (x* )callocN((y)* sizeof(x),name)}
//function callocstructN(x: integer; y: integer; name: integer): integer;
//begin
//  result:= {!!!a type cast? =>} {px(}callocN((y)*sizeof(x),name)
//end;
//
//{RMK(x)}
//function RMK(x: integer): integer;
//begin
//end;
//
//{ELEM(a, b, c)  ( (a)==(b) || (a)==(c) )}
//function ELEM(a: integer; b: integer; c: integer): integer;
//begin
//  result:= ((a)=(b))or((a)=(c))
//end;
//
//{ELEM3(a, b, c, d) ( ELEM(a, b, c) || (a)==(d) )}
//function ELEM3(a: integer; b: integer; c: integer; d: integer): integer;
//begin
//  result:= (ELEM(a,b,c))or((a)=(d))
//end;
//
//{ELEM4(a, b, c, d, e) ( ELEM(a, b, c) || ELEM(a, d, e) )}
//function ELEM4(a: integer; b: integer; c: integer; d: integer; e: integer): integer;
//begin
//  result:= (ELEM(a,b,c))or(ELEM(a,d,e))
//end;
//
//{ELEM5(a, b, c, d, e, f) ( ELEM(a, b, c) || ELEM3(a, d, e, f) )}
//function ELEM5(a: integer; b: integer; c: integer; d: integer; e: integer; f: integer): integer;
//begin
//  result:= (ELEM(a,b,c))or(ELEM3(a,d,e,f))
//end;
//
//{ELEM6(a, b, c, d, e, f, g) ( ELEM(a, b, c) || ELEM4(a, d, e, f, g) )}
//function ELEM6(a: integer; b: integer; c: integer; d: integer; e: integer; f: integer; g: integer): integer;
//begin
//  result:= (ELEM(a,b,c))or(ELEM4(a,d,e,f,g))
//end;
//
//{ELEM7(a, b, c, d, e, f, g, h) ( ELEM3(a, b, c, d) || ELEM4(a, e, f, g, h) )}
//function ELEM7(a: integer; b: integer; c: integer; d: integer; e: integer; f: integer; g: integer; h: integer): integer;
//begin
//  result:= (ELEM3(a,b,c,d))or(ELEM4(a,e,f,g,h))
//end;
//
//{ELEM8(a, b, c, d, e, f, g, h, i) ( ELEM4(a, b, c, d, e) || ELEM4(a, f, g, h, i) )}
//function ELEM8(a: integer; b: integer; c: integer; d: integer; e: integer; f: integer; g: integer; h: integer; i: integer): integer;
//begin
//  result:= (ELEM4(a,b,c,d,e))or(ELEM4(a,f,g,h,i))
//end;
//
//{STREQ(str, a)  ( strcmp((str), (a))==nil )}
//function STREQ(str: integer; a: integer): integer;
//begin
//  result:= (lstrcmp((str),(a))=nil)
//end;
//
//{STREQ2(str, a, b) ( STREQ(str, a) || STREQ(str, b) )}
//function STREQ2(str: integer; a: integer; b: integer): integer;
//begin
//  result:= (STREQ(str,a))or(STREQ(str,b))
//end;
//
//{STREQ3(str, a, b, c) ( STREQ2(str, a, b) || STREQ(str, c) )}
//function STREQ3(str: integer; a: integer; b: integer; c: integer): integer;
//begin
//  result:= (STREQ2(str,a,b))or(STREQ(str,c))
//end;
//
//{MIN2(x,y)  ( (x)<(y) ? (x) : (y) )}
//function MIN2(x: integer; y: integer): integer;
//begin
//  result:= ((x)<(y) {was ?}if  then (x) {was :}else (y))
//end;
//
//{MIN3(x,y,z)  MIN2( MIN2((x),(y)) , (z) )}
//function MIN3(x: integer; y: integer; z: integer): integer;
//begin
//  result:= MIN2(MIN2((x),(y)),(z))
//end;
//
//{MIN4(x,y,z,a)  MIN2( MIN2((x),(y)) , MIN2((z),(a)) )}
//function MIN4(x: integer; y: integer; z: integer; a: integer): integer;
//begin
//  result:= MIN2(MIN2((x),(y)),MIN2((z),(a)))
//end;
//
//{MAX2(x,y)  ( (x)>(y) ? (x) : (y) )}
//function MAX2(x: integer; y: integer): integer;
//begin
//  result:= ((x)>(y) {was ?}if  then (x) {was :}else (y))
//end;
//
//{MAX3(x,y,z)  MAX2( MAX2((x),(y)) , (z) )}
//function MAX3(x: integer; y: integer; z: integer): integer;
//begin
//  result:= MAX2(MAX2((x),(y)),(z))
//end;
//
//{MAX4(x,y,z,a)  MAX2( MAX2((x),(y)) , MAX2((z),(a)) )}
//function MAX4(x: integer; y: integer; z: integer; a: integer): integer;
//begin
//  result:= MAX2(MAX2((x),(y)),MAX2((z),(a)))
//end;
//
//{SWAP(type, a, b) { type sw_ap; sw_ap=(a); (a)=(b); (b)=sw_ap; }}
//function SWAP(type: integer; a: integer; b: integer): integer;
//begin
//  result:= (typesw_ap;sw_ap:=(a);(a):=(b);(b):=sw_ap;)
//end;
//
//{ABS(x) ((x) < 0 ? -(x) : (x))}
//function ABS(x: integer): integer;
//begin
//  result:= ((x)<0 {was ?}if  then -(x) {was :}else (x))
//end;
//
//{GL(x) ( *((int * )(x)))}
//function GL(x: integer): integer;
//begin
//  result:= (*({!!!a type cast? =>} {pinteger(}(x)))
//end;
//
//{GL(x) (((ushort * )(x))[0] << 16 | ((ushort * )(x))[1])}
//function GL(x: integer): integer;
//begin
//  result:= (({!!!a type cast? =>} {pushort(}(x))[0] shl 16 or ({!!!a type cast? =>} {pushort(}(x))[1])
//end;
//
//{GS(x) (((uchar * )(x))[0] << 8 | ((uchar * )(x))[1])}
//function GS(x: integer): integer;
//begin
//  result:= (({!!!a type cast? =>} {puchar(}(x))[0] shl 8 or ({!!!a type cast? =>} {puchar(}(x))[1])
//end;
//
//{GSS(x) (((uchar * )(x))[1] << 8 | ((uchar * )(x))[0])}
//function GSS(x: integer): integer;
//begin
//  result:= (({!!!a type cast? =>} {puchar(}(x))[1] shl 8 or ({!!!a type cast? =>} {puchar(}(x))[0])
//end;
//
//{SWAP_L(x) (((x << 24) & 0xff000000) | ((x << 8) & 0xff0000) | ((x >> 8) & 0xff00) | ((x >> 24) & 0xff))}
//function SWAP_L(x: integer): integer;
//begin
//  result:= (((x shl 24) and $ff000000) or ((x shl 8) and $ff0000) or ((x shr 8) and $ff00) or ((x shr 24) and $ff))
//end;
//
//{SWAP_S(x) (((x << 8) & 0xff00) | ((x >> 8) & 0xff))}
//function SWAP_S(x: integer): integer;
//begin
//  result:= (((x shl 8) and $ff00) or ((x shr 8) and $ff))
//end;
//
//{SWAP_SHORT(x) (((unsigned short) (x) << 8) | ((unsigned short) (x) >> 8))}
//function SWAP_SHORT(x: integer): integer;
//begin
//  result:= (((unsignedshort)(x) shl 8) or ((unsignedshort)(x) shr 8))
//end;
//
//{SWAP_LONG(x) (((x) << 24) | (((x) & 0xff00) << 8) | (((x) >> 8) & 0xff00) | (((x) >> 24) & 0xff))}
//function SWAP_LONG(x: integer): integer;
//begin
//  result:= (((x) shl 24) or (((x) and $ff00) shl 8) or (((x) shr 8) and $ff00) or (((x) shr 24) and $ff))
//end;
//
//{ENDIAN_NOP(x) (x)}
//function ENDIAN_NOP(x: integer): integer;
//begin
//  result:= (x)
//end;
//
//{GET_BIG_LONG(x) (((uchar * ) (x))[0] << 24 | ((uchar * ) (x))[1] << 16 | ((uchar * ) (x))[2] << 8 | ((uchar * ) (x))[3])}
//function GET_BIG_LONG(x: integer): integer;
//begin
//  result:= (({!!!a type cast? =>} {puchar(}(x))[0] shl 24 or ({!!!a type cast? =>} {puchar(}(x))[1] shl 16 or ({!!!a type cast? =>} {puchar(}(x))[2] shl 8 or ({!!!a type cast? =>} {puchar(}(x))[3])
//end;
//
//{GET_LITTLE_LONG(x) (((uchar * ) (x))[3] << 24 | ((uchar * ) (x))[2] << 16 | ((uchar * ) (x))[1] << 8 | ((uchar * ) (x))[0])}
//function GET_LITTLE_LONG(x: integer): integer;
//begin
//  result:= (({!!!a type cast? =>} {puchar(}(x))[3] shl 24 or ({!!!a type cast? =>} {puchar(}(x))[2] shl 16 or ({!!!a type cast? =>} {puchar(}(x))[1] shl 8 or ({!!!a type cast? =>} {puchar(}(x))[0])
//end;
//
//{MAKE_ID(a,b,c,d) ( (int)(a)<<24 | (int)(b)<<16 | (c)<<8 | (d) )}
//function MAKE_ID(a: integer; b: integer; c: integer; d: integer): integer;
//begin
//  result:= ({!!!a type cast? =>} {integer(}(a) shl 24 or {!!!a type cast? =>} {integer(}(b) shl 16 or (c) shl 8 or (d))
//end;
//
//{MAKE_ID(a,b,c,d) ( (int)(d)<<24 | (int)(c)<<16 | (b)<<8 | (a) )}
//function MAKE_ID(a: integer; b: integer; c: integer; d: integer): integer;
//begin
//  result:= ({!!!a type cast? =>} {integer(}(d) shl 24 or {!!!a type cast? =>} {integer(}(c) shl 16 or (b) shl 8 or (a))
//end;
//
//{NEW(x) (x* )mallocN(sizeof(x),# x)}
//function NEW(x: integer): integer;
//begin
//  result:= {!!!a type cast? =>} {px(}mallocN(sizeof(x),x)
//end;
//
//{CLN(x) (x* )callocN(sizeof(x),# x)}
//function CLN(x: integer): integer;
//begin
//  result:= {!!!a type cast? =>} {px(}callocN(sizeof(x),x)
//end;
//
//{PRINT(d, var1) printf(# var1 ":%" # d "\n", var1)}
//function PRINT(d: integer; var1: integer): integer;
//begin
//  result:= printf(var1':%'
//  {$ d "\n", var1)}
//end;
//
//{PRINT2(d, e, var1, var2) printf(# var1 ":%" # d " " # var2 ":%" # e "\n", var1, var2)}
//function PRINT2(d: integer; e: integer; var1: integer; var2: integer): integer;
//begin
//  result:= printf(var1':%'
//  {$ d " " # var2 ":%" # e "\n", var1, var2)}
//end;
//
//{PRINT3(d, e, f, var1, var2, var3) printf(# var1 ":%" # d " " # var2 ":%" # e " " # var3 ":%" # f "\n", var1, var2, var3)}
//function PRINT3(d: integer; e: integer; f: integer; var1: integer; var2: integer; var3: integer): integer;
//begin
//  result:= printf(var1':%'
//  {$ d " " # var2 ":%" # e " " # var3 ":%" # f "\n", var1, var2, var3)}
//end;
//
//{PRINT4(d, e, f, g, var1, var2, var3, var4) printf(# var1 ":%" # d " " # var2 ":%" # e " " # var3 ":%" # f " " # var4 ":%" # g "\n", var1, var2, var3, var4)}
//function PRINT4(d: integer; e: integer; f: integer; g: integer; var1: integer; var2: integer; var3: integer; var4: integer): integer;
//begin
//  result:= printf(var1':%'
//  {$ d " " # var2 ":%" # e " " # var3 ":%" # f " " # var4 ":%" # g "\n", var1, var2, var3, var4)}
//end;
//
//{NEW(x) (x* )mallocN(sizeof(x),"x")}
//function NEW(x: integer): integer;
//begin
//  result:= {!!!a type cast? =>} {px(}mallocN(sizeof(x),'x')
//end;
//
//{CLN(x) (x* )callocN(sizeof(x),"x")}
//function CLN(x: integer): integer;
//begin
//  result:= {!!!a type cast? =>} {px(}callocN(sizeof(x),'x')
//end;
//
//{PRINT(d, var1) printf(# var1 ":%" # d "\n", var1)}
//function PRINT(d: integer; var1: integer): integer;
//begin
//  result:= printf(var1':%'
//  {$ d "\n", var1)}
//end;
//
//{PRINT2(d, e, var1, var2) printf(# var1 ":%" # d " " # var2 ":%" # e "\n", var1, var2)}
//function PRINT2(d: integer; e: integer; var1: integer; var2: integer): integer;
//begin
//  result:= printf(var1':%'
//  {$ d " " # var2 ":%" # e "\n", var1, var2)}
//end;
//
//{PRINT3(d, e, f, var1, var2, var3) printf(# var1 ":%" # d " " # var2 ":%" # e " " # var3 ":%" # f "\n", var1, var2, var3)}
//function PRINT3(d: integer; e: integer; f: integer; var1: integer; var2: integer; var3: integer): integer;
//begin
//  result:= printf(var1':%'
//  {$ d " " # var2 ":%" # e " " # var3 ":%" # f "\n", var1, var2, var3)}
//end;
//
//{PRINT4(d, e, f, g, var1, var2, var3, var4) printf(# var1 ":%" # d " " # var2 ":%" # e " " # var3 ":%" # f " " # var4 ":%" # g "\n", var1, var2, var3, var4)}
//function PRINT4(d: integer; e: integer; f: integer; g: integer; var1: integer; var2: integer; var3: integer; var4: integer): integer;
//begin
//  result:= printf(var1':%'
//  {$ d " " # var2 ":%" # e " " # var3 ":%" # f " " # var4 ":%" # g "\n", var1, var2, var3, var4)}
//end;
//
//{MEMNEXT(x) ((MemHead * )(((char * ) x) - ((char * ) & (((MemHead * )0)->next))))}
//function MEMNEXT(x: integer): integer;
//begin
//  result:= ({!!!a type cast? =>} {pMemHead(}(({!!!a type cast? =>} {pchar(}x)-((char* ) and (({!!!a type cast? =>} {pMemHead(}0).next))))
//end;
//
//{malloc(x) mallocN(x,"debug")}
//function malloc(x: integer): integer;
//begin
//  result:= mallocN(x,'debug')
//end;
//
//{free(x) freeN(x)}
//function free(x: integer): integer;
//begin
//  result:= freeN(x)
//end;
//
//{calloc(x,y) callocN((x)*(y),"debug")}
//function calloc(x: integer; y: integer): integer;
//begin
//  result:= callocN((x)*(y),'debug')
//end;

end.
