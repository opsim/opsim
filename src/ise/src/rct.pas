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

(*
 *
 * rct.c
 *
 * april 95
 *
 *)

unit rct;

interface

uses
  blender, util;

function in_rcti(rect: prcti; x, y: integer): integer;
function in_rctf(rect: prctf;  x, y: single): integer;
procedure union_rctf(rct1: prctf;  rct2: prctf);
procedure init_rctf(rect: prctf;  xmin: single;  xmax: single;  ymin: single;  ymax: single);
function isect_rctf(src1: prctf;  src2: prctf;  dest: prctf): integer;

implementation

function in_rcti(rect: prcti; x, y: integer): integer;
begin
  if x<rect^.xmin then
    exit(0);
  if x>rect^.xmax then
    exit(0);
  if y<rect^.ymin then
    exit(0);
  if y>rect^.ymax then
    exit(0);
  exit(1);
end;

function in_rctf(rect: prctf;  x, y: single): integer;
begin
  if x<rect^.xmin then
    exit(0);
  if x>rect^.xmax then
    exit(0);
  if y<rect^.ymin then
    exit(0);
  if y>rect^.ymax then
    exit(0);
  exit(1);
end;


procedure union_rctf(rct1: prctf;  rct2: prctf); 
begin
  if rct1^.xmin>rct2^.xmin then
  rct1^.xmin:= rct2^.xmin; 
  if rct1^.xmax<rct2^.xmax then
  rct1^.xmax:= rct2^.xmax; 
  if rct1^.ymin>rct2^.ymin then
  rct1^.ymin:= rct2^.ymin; 
  if rct1^.ymax<rct2^.ymax then
  rct1^.ymax:= rct2^.ymax; 
end;


procedure init_rctf(rect: prctf;  xmin: single;  xmax: single;  ymin: single;  ymax: single); 
begin
  rect^.xmin:= xmin; 
  rect^.xmax:= xmax; 
  rect^.ymin:= ymin; 
  rect^.ymax:= ymax; 
end;


function isect_rctf(src1: prctf;  src2: prctf;  dest: prctf): integer; 
var
xmin: single; 
xmax: single; 
ymin: single; 
ymax: single; 
begin
  xmin:= MAX2(src1^.xmin,src2^.xmin);
  xmax:= MIN2(src1^.xmax,src2^.xmax); 
  ymin:= MAX2(src1^.ymin,src2^.ymin); 
  ymax:= MIN2(src1^.ymax,src2^.ymax);

  if (xmax>=xmin)and(ymax>=ymin) then
  begin 
    if dest<>nil then
    begin 
      dest^.xmin:= xmin; 
      dest^.xmax:= xmax; 
      dest^.ymin:= ymin; 
      dest^.ymax:= ymax; 
    end;
    exit(1);
  end
  else
  begin 
    if dest<>nil then
    begin 
      dest^.xmin:= 0; 
      dest^.xmax:= 0; 
      dest^.ymin:= 0; 
      dest^.ymax:= 0; 
    end;
    exit(0);
  end;
end;

end.
