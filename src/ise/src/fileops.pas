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

unit fileops;

interface

function fop_exists(_file: pchar): integer;

implementation

uses
  SysUtils;

//function first_slash(string: pchar): pchar; 
//var
//ffslash: pchar; 
//fbslash: pchar; 
//begin

//  ffslash:= strchr(string,'/'); 
//  fbslash:= strchr(string,#92); 
//  if {not}0=ffslash then
//  begin
//    result:= fbslash; 
//    exit;
//  end;
//  else
//  if {not}0=fbslash then
//  begin
//    result:= ffslash; 
//    exit;
//  end;
//  if {!!!a type cast? =>} {integer(}ffslash<{!!!a type cast? =>} {integer(}fbslash then
//  begin
//    result:= ffslash; 
//    exit;
//  end;
//  else
//  begin
//    result:= fbslash; 
//    exit;
//  end;
//end;

//function last_slash(string: pchar): pchar; 
//var
//lfslash: pchar; 
//lbslash: pchar; 
//begin

//  lfslash:= strrchr(string,'/'); 
//  lbslash:= strrchr(string,#92); 
//  if {not}0=lfslash then
//  begin
//    result:= lbslash; 
//    exit;
//  end;
//  else
//  if {not}0=lbslash then
//  begin
//    result:= lfslash; 
//    exit;
//  end;
//  if {!!!a type cast? =>} {integer(}lfslash<{!!!a type cast? =>} {integer(}lbslash then
//  begin
//    result:= lbslash; 
//    exit;
//  end;
//  else
//  begin
//    result:= lfslash; 
//    exit;
//  end;
//end;

{$ifdef WINDOWS}

//var {was static}
//str: array [0..Pred(MAXPATHLEN+12)] of char; 

//function fop_delete(file: pchar;  dir: integer;  recursive: integer): integer; 
//var
//err: integer; 
//begin
//  if recursive<>0 then
//  begin 
//    error('Recursive delete is unsupported on Windows'); 
//    err:= 1; 
//  end;
//  else
//  if dir<>0 then
//  begin 
//    err:= {not}0=RemoveDirectory(file); 
//    if err<>0 then
//    printf('Unable to remove directory'); 
//  end;
//  else
//  begin 
//    err:= {not}0=DeleteFile(file); 
//    if err<>0 then
//    error('Unable to delete file'); 
//  end;

//  begin
//    result:= err; 
//    exit;
//  end;
//end;

//function fop_touch(file: pchar): integer; 
//begin
//  error('Touching files is unsupported on Windows'); 
//  begin
//    result:= 1; 
//    exit;
//  end;
//end;

//function fop_move(file: pchar;  to: pchar): integer; 
//var
//err: integer; 
//begin
//  err:= {not}0=MoveFile(file,to); 
//  if err<>0 then
//  error('Unable to move file'); 

//  begin
//    result:= err; 
//    exit;
//  end;
//end;

//function fop_copy(file: pchar;  to: pchar): integer; 
//var
//err: integer; 
//begin
//  err:= {not}0=CopyFile(file,to,LongBool(0)); 
//  if err<>0 then
//  error('Unable to copy file!'); 

//  begin
//    result:= err; 
//    exit;
//  end;
//end;

//function fop_link(file: pchar;  to: pchar): integer; 
//begin
//  error('Linking files is unsupported on Windows'); 
//  begin
//    result:= 1; 
//    exit;
//  end;
//end;

//function fop_backup(file: pchar;  from: pchar;  to: pchar): integer; 
//begin
//  error('Backing up files is unsupported on Windows'); 
//  begin
//    result:= 1; 
//    exit;
//  end;
//end;

function fop_exists(_file: pchar): integer;
begin
  if FileExists(_file) or DirectoryExists(_file) then
    exit(1)
  else
    exit(0);
end;

//procedure fop_recurdir(dirname: pchar); 
//var
//lslash: pchar; 
//tmp: array [0..Pred(MAXPATHLEN)] of char; 
//err: integer; 
//begin

//  if fop_exists(dirname) then
//  exit;

//  strcpy(tmp,dirname); 
//  lslash:= last_slash(tmp); 
//  if {not}0=lslash then
//  exit;
//  (* Split about the last slash and recurse *)
//  {*}lslash^:=0; 
//  fop_recurdir(tmp); 

//  if {not}0=CreateDirectory(dirname,0{nil}) then
//  error('Unable to create directory\n');
//end;

//function fop_rename(from: pchar;  to: pchar): integer; 
//begin
//  if {not}0=fop_exists(from) then
//  begin
//    result:= 0; 
//    exit;

//  end;
//  if fop_exists(to) then
//  if fop_delete(to,0,0) then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  begin
//    result:= rename(from,to); 
//    exit;
//  end;
//end;

{$else}
(* The sane UNIX world *)

//var {was static}
//str: array [0..Pred(MAXPATHLEN+12)] of char; 

//function fop_delete(file: pchar;  dir: integer;  recursive: integer): integer; 
//begin
//  if recursive<>0 then
//  sprintf(str,'/bin/rm -rf %s',file); 
//  else
//  if dir<>0 then
//  sprintf(str,'/bin/rmdir "%s"',file); 
//  else
//  sprintf(str,'/bin/rm -f "%s"',file); 

//  begin
//    result:= docmd(str); 
//    exit;
//  end;
//end;

//function fop_touch(file: pchar): integer; 
//begin
//  sprintf(str,'/bin/touch %s',file); 

//  begin
//    result:= docmd(str); 
//    exit;
//  end;
//end;

//function fop_move(file: pchar;  to: pchar): integer; 
//begin
//  sprintf(str,'/bin/mv -f %s %s',file,to); 

//  begin
//    result:= docmd(str); 
//    exit;
//  end;
//end;

//function fop_copy(file: pchar;  to: pchar): integer; 
//begin
//  sprintf(str,'/bin/cp -rf "%s" %s',file,to); 

//  begin
//    result:= docmd(str); 
//    exit;
//  end;
//end;

//function fop_link(file: pchar;  to: pchar): integer; 
//begin
//  sprintf(str,'/bin/ln -f %s %s',file,to); 
//  begin
//    result:= docmd(str); 
//    exit;
//  end;
//end;

//function fop_backup(file: pchar;  from: pchar;  to: pchar): integer; 
//begin
//  sprintf(str,'/bin/su root -c ''cd %s; /bin/tar cf - "%s" | (/bin/cd %s; /bin/tar xf -)''',from,file,to);
//  begin
//    result:= docmd(str); 
//    exit;
//  end;
//end;

//function fop_exists(file: pchar): integer; 
//begin
//  begin
//    result:= exist(file); 
//    exit;
//  end;
//end;

//procedure fop_recurdir(dirname: pchar); 
//var
//lslash: pchar; 
//tmp: array [0..Pred(MAXPATHLEN)] of char; 
//begin

//  if fop_exists(dirname) then
//  exit;

//  strcpy(tmp,dirname); 
//  lslash:= last_slash(tmp); 
//  if {not}0=lslash then
//  exit;
//  (* Split about the last slash and recurse *)
//  {*}lslash^:=0; 
//  fop_recurdir(tmp); 

//  mkdir(dirname,$1FF); 
//end;

//function fop_rename(from: pchar;  to: pchar): integer; 
//begin
//  if {not}0=fop_exists(from) then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  if fop_exists(to) then
//  if fop_delete(to,0,0) then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  begin
//    result:= rename(from,to); 
//    exit;
//  end;
//end;

{$endif}

end.
