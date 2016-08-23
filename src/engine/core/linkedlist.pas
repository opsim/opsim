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

unit LinkedList;

{$mode objfpc}{$H-}

interface

type
  pLink = ^Link;

  Link = record
    next, prev: pLink;   //<
  end;

  pListBase = ^ListBase;

  ListBase = record
    first, last: pointer;   //<
  end;

{
   This is the procedure addhead
   @param listbase:
   @param vlink:
}
procedure addhead(listbase: pListBase; vlink: pointer);
{
   This is the procedure addtail
   @param listbase:
   @param vlink:
}
procedure addtail(listbase: pListBase; vlink: pointer);
{
   This is the function callocN
   @param len:
   @return the pointer type
}
function callocN(len: PtrUInt): pointer;
{
   This is the function countlist
   @param listbase:
   @return the integer type
}
function countlist(listbase: pListBase): integer;
{
   This is the procedure freeN
   @param vmemh:
}
procedure freeN(vmemh: pointer);
{
   This is the procedure freelinkN
   @param listbase:
   @param vlink:
}
procedure freelinkN(listbase: pListBase; vlink: pointer);
{
   This is the procedure freelist
   @param listbase:
}
procedure freelist(listbase: pListBase);
{
   This is the procedure freelistN
   @param listbase:
}
procedure freelistN(listbase: pListBase);
{
   This is the procedure insertlink
   @param listbase:
   @param vprevlink:
   @param vnewlink:
}
procedure insertlink(listbase: pListBase; vprevlink, vnewlink: pointer);
{
   This is the procedure insertlinkbefore
   @param listbase:
   @param vnextlink:
   @param vnewlink:
}
procedure insertlinkbefore(listbase: pListBase; vnextlink, vnewlink: pointer);
{
   This is the function mallocN
   @param len:
   @return the pointer type
}
function mallocN(len: PtrUInt): pointer;
{
   This is the procedure remlink
   @param listbase:
   @param vlink:
}
procedure remlink(listbase: pListBase; vlink: pointer);

implementation

uses
  SysUtils;

procedure addhead(listbase: pListBase; vlink: pointer);
var
  link: pLink;
begin
  link := vlink;

  if (link = nil) then
    exit;
  if (listbase = nil) then
    exit;

  link^.next := listbase^.first;
  link^.prev := nil;

  if (listbase^.first <> nil) then
    pLink(listbase^.first)^.prev := link;
  if (listbase^.last = nil) then
    listbase^.last := link;
  listbase^.first := link;
end;


procedure addtail(listbase: pListBase; vlink: pointer);
var
  link: pLink;
begin
  link := vlink;

  if link = nil then
    exit;
  if listbase = nil then
    exit;

  link^.next := nil;
  link^.prev := listbase^.last;

  if listbase^.last <> nil then
    pLink(listbase^.last)^.next := link;
  if listbase^.first = nil then
    listbase^.first := link;
  listbase^.last := link;
end;


procedure remlink(listbase: pListBase; vlink: pointer);
var
  link: pLink;
begin
  link := vlink;

  if link = nil then
    exit;
  if listbase = nil then
    exit;

  if link^.next <> nil then
    link^.next^.prev := link^.prev;
  if link^.prev <> nil then
    link^.prev^.next := link^.next;

  if listbase^.last = link then
    listbase^.last := link^.prev;
  if listbase^.first = link then
    listbase^.first := link^.next;
end;


procedure freelinkN(listbase: pListBase; vlink: pointer);
var
  link: pLink;
begin
  link := vlink;

  if link = nil then
    exit;
  if listbase = nil then
    exit;

  remlink(listbase, link);
  freeN(link);
end;

procedure insertlink(listbase: pListBase; vprevlink, vnewlink: pointer);
var
  prevlink, newlink: pLink;
begin
  prevlink := vprevlink;
  newlink := vnewlink;

  (* newlink komt na prevlink *)

  if newlink = nil then
    exit;
  if listbase = nil then
    exit;

  (* lege lijst *)
  if listbase^.first = nil then
  begin

    listbase^.first := newlink;
    listbase^.last := newlink;
    exit;
  end;

  (* inserten voor eerste element *)
  if prevlink = nil then
  begin
    newlink^.next := listbase^.first;
    newlink^.prev := nil;
    newlink^.next^.prev := newlink;
    listbase^.first := newlink;
    exit;
  end;

  (* aan einde lijst *)
  if listbase^.last = prevlink then
    listbase^.last := newlink;

  newlink^.next := prevlink^.next;
  prevlink^.next := newlink;
  if newlink^.next <> nil then
    newlink^.next^.prev := newlink;
  newlink^.prev := prevlink;
end;

procedure insertlinkbefore(listbase: pListBase; vnextlink, vnewlink: pointer);
var
  nextlink, newlink: pLink;
begin
  nextlink := vnextlink;
  newlink := vnewlink;

  (* newlink komt voor nextlink *)

  if newlink = nil then
    exit;
  if listbase = nil then
    exit;

  if listbase^.first = nil then
  begin (* lege lijst *)
    listbase^.first := newlink;
    listbase^.last := newlink;
    exit;
  end;
  if nextlink = nil then
  begin  (* inserten aan einde lijst *)
    newlink^.prev := listbase^.last;
    newlink^.next := nil;
    pLink(listbase^.last)^.next := newlink;
    listbase^.last := newlink;
    exit;
  end;

  if listbase^.first = nextlink then (* aan begin lijst *)
    listbase^.first := newlink;

  newlink^.next := nextlink;
  newlink^.prev := nextlink^.prev;
  nextlink^.prev := newlink;
  if newlink^.prev <> nil then
    newlink^.prev^.next := newlink;
end;

procedure freelist(listbase: pListBase);
var
  link, next: pLink;
begin
  if listbase = nil then
    exit;
  link := listbase^.first;
  while link <> nil do
  begin
    next := link^.next;
    freeN(link);
    link := next;
  end;
  listbase^.first := nil;
  listbase^.last := nil;
end;

procedure freelistN(listbase: pListBase);
var
  link, next: pLink;
begin
  if listbase = nil then
    exit;

  link := listbase^.first;
  while link <> nil do
  begin
    next := link^.next;
    freeN(link);
    link := next;
  end;
  listbase^.first := nil;
  listbase^.last := nil;
end;

function callocN(len: PtrUInt): pointer;
var
  ptr: pointer;
begin
  ptr := mallocN(len);
  FillChar(ptr^, len, 0);
  exit(ptr);
end;

function countlist(listbase: pListBase): integer;
var
  link: pLink;
  Count: integer = 0;
begin
  if listbase <> nil then
  begin
    link := listbase^.first;
    while link <> nil do
    begin
      Inc(Count);
      link := link^.next;
    end;
  end;
  exit(Count);
end;

procedure freeN(vmemh: pointer);
begin
  freemem(vmemh);
end;

function mallocN(len: PtrUInt): pointer;
begin
  exit(GetMem(len));
end;

end.
