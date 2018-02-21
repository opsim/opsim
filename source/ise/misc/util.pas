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

  function MAKE_ID(a,b,c,d : pchar) : longint;

  type
    pLink = ^Link;
    Link = record
        next : ^Link;
        prev : ^Link;
      end;

  { never change the size of this! genfile.c detects pointerlen with it  }
  pListBase = ^ListBase;
    ListBase = record
        first : pointer;
        last : pointer;
      end;

  { 8 byte alignment!  }

    pMemHead = ^MemHead;
    MemHead = record
        tag1 : longint;
        len : longint;
        next : ^MemHead;
        prev : ^MemHead;
        name : pchar;
        nextname : pchar;
        level : longint;
        tag2 : longint;
      end;

    pMemTail = ^MemTail;
    MemTail = record
        tag3 : longint;
        pad : longint;
      end;

  function MEMTAG1 : longint;
   function MEMTAG2 : longint;
  function MEMTAG3 : longint;

  function MEMFREE : longint;
  function MEMNEXT(x : longint) : pMemHead;

  procedure addhead(listbase:pListBase; vlink:pointer);
  procedure addtail(listbase:pListBase; vlink:pointer);
  function callocN(len:longint; str:Pchar):pointer;
  function check_memlist(memh:PMemHead):pchar;
  function countlist(listbase:pListBase):longint;
  function freeN(vmemh:pointer):smallint;
  procedure freelinkN(listbase:pListBase; vlink:pointer);
  procedure freelist(listbase:pListBase);
  procedure freelistN(listbase:pListBase);
  procedure insertlink(listbase:pListBase; vprevlink:pointer; vnewlink:pointer);
  procedure insertlinkbefore(listbase:pListBase; vnextlink:pointer; vnewlink:pointer);
  function mallocN(len:longint; str:Pchar):pointer;
  procedure printmemlist;
  procedure rem_memblock(memh:PMemHead);
  procedure remlink(listbase:pListBase; vlink:pointer);

implementation

uses
    SysUtils;

    var
    totblock: integer =0;
    mem_in_use: integer =0;
    current_mem_level: integer =0;
    _membase: ListBase;
     membase : pListBase= @_membase;

    function MAKE_ID(a,b,c,d : pchar) : longint;
    begin
      {$ifdef ENDIAN_LITTLE}
      MAKE_ID:=((((longint(a)) shl 24) or ((longint(b)) shl 16)) or (longint(c) shl 8)) or longint(d);
      {$else}
      MAKE_ID:=((((longint(d)) shl 24) or ((longint(c)) shl 16)) or (longint(b) shl 8)) or longint(a);
      {$endif}
    end;

    function MEMTAG1 : longint;
      begin
        MEMTAG1:=MAKE_ID('M','E','M','O');
      end;

  function MEMTAG2 : longint;
    begin
      MEMTAG2:=MAKE_ID('R','Y','B','L');
    end;

function MEMTAG3 : longint;
  begin
    MEMTAG3:=MAKE_ID('O','C','K','!');
  end;

function MEMFREE : longint;
  begin
    MEMFREE:=MAKE_ID('F','R','E','E');
  end;

function MEMNEXT(x : pointer) : pMemHead;
begin
  MEMNEXT:=pMemHead((pchar(x))-(pchar(@((pMemHead(0))^.next))));
end;

function malloc(size: size_t): pointer;
var
  p: pointer;
begin
  p := getmem(size);
  initialize(p);
  exit(p);
end;

function calloc(num, size: size_t): pointer;
var
  p: Pointer;
begin
  p:= malloc(num * size);
  fillchar(p^, num * size, #0);
  exit(p);
end;

procedure free(ptr: pointer);
begin
  finalize(ptr);
  freemem(ptr);
end;

procedure addhead(listbase: pListBase;  vlink: pointer);
var
 link: pLink;
begin
  link:= vlink;

  if link=nil then
  exit;

  if listbase=nil then
  exit;

  link^.next:= listbase^.first;
  link^.prev:=nil;

  if listbase^.first<>nil then
  pLink(listbase^.first)^.prev:=link;

  if listbase^.last=nil then
  listbase^.last:= link;

  listbase^.first:= link;
end;

procedure addtail(listbase: pListBase;  vlink: pointer);
var
 link: pLink;
begin
  link:= vlink;

  if link=nil then
  exit;

  if listbase=nil then
  exit;

  link^.next:= nil;
  link^.prev:= listbase^.last;
  if listbase^.last<>nil then
  pLink(listbase^.last)^.next:=link;

  if listbase^.first=nil then
  listbase^.first:= link;

  listbase^.last:= link;
end;

procedure remlink(listbase: pListBase;  vlink: pointer);
var
 link: pLink;
begin
  link:= vlink;

  if link=nil then
  exit;

  if listbase=nil then
  exit;

  if link^.next<>nil then
  link^.next^.prev:= link^.prev;

  if link^.prev<>nil then
  link^.prev^.next:= link^.next;

  if listbase^.last=link then
  listbase^.last:= link^.prev;

  if listbase^.first=link then
  listbase^.first:= link^.next;
end;

procedure freelinkN(listbase: pListBase;  vlink: pointer);
var
 link: pLink;
begin
  link:= vlink;

  if link=nil then
  exit;

  if listbase=nil then
  exit;

  remlink(listbase,link);
  freeN(link);
end;

procedure insertlink(listbase: pListBase;  vprevlink: pointer;  vnewlink: pointer);
var
 prevlink: pLink;
 newlink: pLink;
begin
  prevlink:= vprevlink;
  newlink:= vnewlink;

  (* newlink komt na prevlink *)

  if newlink=nil then
  exit;
  if listbase=nil then
  exit;

  if listbase^.first=nil then  (* lege lijst *)
  begin
    listbase^.first:= newlink;
    listbase^.last:= newlink;
    exit;
  end;
  if prevlink=nil then    (* inserten voor eerste element *)
  begin
    newlink^.next:= listbase^.first;
    newlink^.prev:=nil;
    newlink^.next^.prev:= newlink;
    listbase^.first:= newlink;
    exit;
  end;

  if listbase^.last=prevlink then  (* aan einde lijst *)
  listbase^.last:= newlink;

  newlink^.next:= prevlink^.next;
  prevlink^.next:= newlink;
  if newlink^.next<>nil then
  newlink^.next^.prev:= newlink;
  newlink^.prev:= prevlink;
end;

procedure insertlinkbefore(listbase: pListBase;  vnextlink: pointer;  vnewlink: pointer);
var
nextlink, newlink: pLink;
begin
  	nextlink:= vnextlink;
        newlink:= vnewlink;

        (* newlink komt voor nextlink *)

  if newlink=nil then
  exit;
  if listbase=nil then
  exit;

  (* lege lijst *)
  if listbase^.first=nil then
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
  if listbase^.first=nextlink then
  listbase^.first:= newlink;

  newlink^.next:= nextlink;
  newlink^.prev:= nextlink^.prev;
  nextlink^.prev:= newlink;

  if newlink^.prev<>nil then
  newlink^.prev^.next:= newlink;
end;

procedure freelist(listbase: pListBase);
var
link: pLink;
next: pLink;
begin
  if listbase=nil then
  exit;

  link:= listbase^.first;
  while link <>nil  do
  begin
    next:= link^.next;
    free(link);
    link:= next;
  end;

  listbase^.first:=nil;
  listbase^.last:=nil;
end;

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

function countlist(listbase: pListBase): integer;
var
link: pLink;
count: integer = 0;
begin
  if listbase<>nil then
  begin
    link:= listbase^.first;
    while link<>nil    do
    begin
      inc(count);
      link:= link^.next;
    end;
  end;

  exit(count);
end;

procedure _MemorY_ErroR(block, error: pchar);
begin
  writeln(stderr,'Memoryblock ', block,': ',error);
end;

var
memory_error: procedure(block, error: pchar) = @_MemorY_ErroR;

function mallocN(len: integer;  str: pchar): pointer;
var
memh: pMemHead;
memt: pMemTail;
begin
  if len<=0 then
  begin
    writeln('Malloc error: len=', len, ' in ',str);
          exit(nil);
  end;

  if sizeof(longint)=8 then
  len:= (len+3) and  not 3      (* eenheden van 4 *)
  else
  len:= (len+7) and  not 7;      (* eenheden van 8 *)

  memh:= pMemHead(malloc(len+sizeof(MemHead)+sizeof(MemTail)));

  if memh<>nil then
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

    if memh^.next<>nil then
    memh^.nextname:= MEMNEXT(memh^.next)^.name;

    inc(totblock);
    mem_in_use:= mem_in_use + (len);
      inc(memh);

      exit(memh);
  end;

  writeln('Malloc returns nill: len=', len,' in ',str);
      exit(nil);
end;

function callocN(len: integer;  str: pchar): pointer;
var
memh: pMemHead;
memt: pMemTail;
begin
  if len<=0 then
  begin
     writeln('Calloc error: len=', len, ' in ',str);
    exit(nil);
  end;

  if sizeof(longint)=8 then
  len:= (len+3) and  not 3   (* eenheden van 4 *)
  else
  len:= (len+7) and  not 7;   (* eenheden van 8 *)

  memh:= pMemHead(calloc(len+sizeof(MemHead)+sizeof(MemTail),1));

  if memh<>nil then
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

        if memh^.next<>nil then
        memh^.nextname:= MEMNEXT(memh^.next)^.name;

        inc(totblock);
        mem_in_use:= mem_in_use + len;
          inc(memh);

          exit(memh);
  end;

  writeln('Calloc returns nill: len=', len,' in ',str);
  exit(nil);
end;

procedure rem_memblock(memh: pMemHead);
begin
  remlink(membase,@memh^.next);
  if memh^.prev<>nil then
  begin
    if memh^.next<>nil then
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
  writeln('total memory used:   ', mem_in_use, ' bytes');
  writeln('total memory blocks: ',totblock);

  membl:= membase^.first;
  if membl<>nil then
  membl:= MEMNEXT(membl);

  while membl <>nil do
  begin
    writeln(Format('%24s len: %8d %p',[membl^.name,membl^.len,membl+1]));
    if membl^.next<>nil then
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
  if forw<>nil then
  forw:= MEMNEXT(forw);

  forwok:= nil;
  while forw <>nil  do
  begin
    if (forw^.tag1<>MEMTAG1)or(forw^.tag2<>MEMTAG2) then
    break;
    forwok:= forw;
    if forw^.next<>nil then
    forw:= MEMNEXT(forw^.next)
    else
    forw:= nil;
  end;

  back:= pMemHead(membase^.last);
  if back<>nil then
  back:= MEMNEXT(back);
  backok:= nil;
  while back<>nil  do
  begin
    if (back^.tag1<>MEMTAG1)or(back^.tag2<>MEMTAG2) then
    break;

    backok:= back;
    if back^.prev<>nil then
    back:= MEMNEXT(back^.prev)
    else
    back:= nil;
  end;

  if forw<>back then
    exit('MORE THAN 1 MEMORYBLOCK CORRUPT');

  if (forw=nil)and(back=nil) then
  begin
    (* geen foute headers gevonden dan maar op zoek naar memblock*)
    forw:= membase^.first;

    if forw<>nil then
    forw:= MEMNEXT(forw);

    forwok:= nil;
    while forw<>nil    do
    begin
      if forw=memh then
      break;

      if (forw^.tag1<>MEMTAG1)or(forw^.tag2<>MEMTAG2) then
      break;

      forwok:= forw;
      if forw^.next<>nil then
      forw:= MEMNEXT(forw^.next)
      else
      forw:=nil;
    end;

    if forw=nil then
    exit(nil);

    back:= pMemHead(membase^.last);
    if back<>nil then
    back:= MEMNEXT(back);
    backok:= nil;
    while back <>nil    do
    begin
      if back=memh then
      break;

      if (back^.tag1<>MEMTAG1)or(back^.tag2<>MEMTAG2) then
      break;

      backok:= back;
      if back^.prev<>nil then
      back:= MEMNEXT(back^.prev)
      else
      back:= nil;
    end;
  end;
  if forwok<>nil then
  name:= forwok^.nextname
  else
  name:= 'No name found';

  if forw=memh then
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
      if backok<>nil then
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

  if memh=nil then
  begin
    memory_error('free','attempt to free nil pointer');
      exit(-1);
   end;
  if sizeof(longint)=8 then
  begin
    if (integer(memh) and $7) <>0 then
    begin
      memory_error('free','attempt to free illegal pointer');
      exit(-1);
    end;
  end
  else
  begin
    if (integer(memh) and $3) <>0 then
    begin
      memory_error('free','attempt to free illegal pointer');
      exit(-1);
    end;
  end;

  dec(memh);
  if (memh^.tag1=MEMFREE)and(memh^.tag2=MEMFREE) then
  begin
    memory_error(memh^.name,'double free');
    exit(-1);
  end;
  if (memh^.tag1=MEMTAG1)and(memh^.tag2=MEMTAG2)and((memh^.len and $3)=0) then
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
    if name<>nil then
    begin
      if name<>memh^.name then
      memory_error(name,'is also corrupt');
    end;
  end
  else
  begin
    error:= -1;
    name:= check_memlist(memh);
    if name=nil then
    memory_error('free','pointer not in memlist')
    else
    memory_error(name,'error in header');
  end;
  dec(totblock);
  (* hier moet een DUMP plaatsvinden *)
  exit(error);
end;

end.
