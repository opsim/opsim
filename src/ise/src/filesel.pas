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

(*  filesel.c       GRAPHICS

 *
 *  jan 95
 *
 *)

unit filesel;

interface

uses
  blender;

procedure activate_fileselect(_type: integer;  title: pchar;  _file: pchar; func: retfunc);

implementation

uses
  cfuncs,
  blendef, screen, space, mydevice, readfile, util;

//function fnmatch: integer;
//begin
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
{$ifdef MIPS1}

//procedure usleep(val: integer); 
//begin
//  sginap(val); 
//end;
{$endif}

//const
//FILESELHEAD = 60; 
//FILESEL_DY = 16; 

//NOTACTIVE = 0; 
//ACTIVATE = 1; 
//INACTIVATE = 2; 

//function STARTSWITH(x: integer; y: integer): integer; {<= !!!6 unknown macro}

//procedure library_to_filelist(sfile: pSpaceFile); 

//procedure main_to_filelist(sfile: pSpaceFile); 

function code_to_groupname(code: integer): pchar; forward;

//procedure filesel_select_objects(sfile: pSpaceFile); 

//procedure active_file_object(sfile: pSpaceFile); 

//(* globals *)

var
//scrollrct: rcti; 
//textrct: rcti; 
//bar: rcti; 
//filebuty1: integer; 
//filebuty2: integer; 
//page_ofs: integer; 
//collumwidth: integer; 
//selecting: integer = 0; 
  filetoname: integer = 0;
//pixels_to_ofs: float; 
//otherdir: array [0..Pred(FILE_MAXDIR)] of char; 
//fslog: array [0..Pred(200)] of char = #0; 
//fsmenu: pchar = 0; 
//otherarea: pScrArea; 
//(* ******************* SORT ******************* *)

//function compare: integer; 
//(* storage.c *)

//function compare_date(entry1: pdirentry;  entry2: pdirentry): integer; 
//begin(* type is gelijk aan stat.st_mode *)

//  if S_ISDIR(entry1.type) then
//  begin 
//    if S_ISDIR(entry2.type)=0 then
//    begin
//      result:= (-1); 
//      exit;
//    end;
//  end;
//  else
//  begin 
//    if S_ISDIR(entry2.type) then
//    begin
//      result:= (1); 
//      exit;
//    end;
//  end;
//  if S_ISREG(entry1.type) then
//  begin 
//    if S_ISREG(entry2.type)=0 then
//    begin
//      result:= (-1); 
//      exit;
//    end;
//  end;
//  else
//  begin 
//    if S_ISREG(entry2.type) then
//    begin
//      result:= (1); 
//      exit;
//    end;
//  end;
//  if (entry1.type and S_IFMT)<(entry2.type and S_IFMT) then
//  begin
//    result:= (-1); 
//    exit;
//  end;
//  if (entry1.type and S_IFMT)>(entry2.type and S_IFMT) then
//  begin
//    result:= (1); 
//    exit;

//    (*
//     if ( entry1->s.st_ctime < entry2->s.st_ctime) return 1;
//     if ( entry1->s.st_ctime > entry2->s.st_ctime) return -1;
//    *)
//  end;
//  if entry1.s.st_mtime<entry2.s.st_mtime then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  if entry1.s.st_mtime>entry2.s.st_mtime then
//  begin
//    result:= -1; 
//    exit;
//  end;
//  else
//  begin
//    result:= strcasecmp(entry1.relname,entry2.relname); 
//    exit;
//  end;
//end;

//function compare_size(entry1: pdirentry;  entry2: pdirentry): integer; 
//begin(* type is gelijk aan stat.st_mode *)

//  if S_ISDIR(entry1.type) then
//  begin 
//    if S_ISDIR(entry2.type)=0 then
//    begin
//      result:= (-1); 
//      exit;
//    end;
//  end;
//  else
//  begin 
//    if S_ISDIR(entry2.type) then
//    begin
//      result:= (1); 
//      exit;
//    end;
//  end;
//  if S_ISREG(entry1.type) then
//  begin 
//    if S_ISREG(entry2.type)=0 then
//    begin
//      result:= (-1); 
//      exit;
//    end;
//  end;
//  else
//  begin 
//    if S_ISREG(entry2.type) then
//    begin
//      result:= (1); 
//      exit;
//    end;
//  end;
//  if (entry1.type and S_IFMT)<(entry2.type and S_IFMT) then
//  begin
//    result:= (-1); 
//    exit;
//  end;
//  if (entry1.type and S_IFMT)>(entry2.type and S_IFMT) then
//  begin
//    result:= (1); 
//    exit;

//  end;
//  if entry1.s.st_size<entry2.s.st_size then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  if entry1.s.st_size>entry2.s.st_size then
//  begin
//    result:= -1; 
//    exit;
//  end;
//  else
//  begin
//    result:= strcasecmp(entry1.relname,entry2.relname); 
//    exit;
//  end;
//end;

//(* **************************************** *)

//procedure clear_global_filesel_vars; 
//begin
//  selecting:= 0; 
//end;

//procedure addfilename_to_fsmenu(name: pchar); 
//var
//temp: pchar; 
//len: integer; 
//begin

//  len:= lstrlen(name); 
//  if len=0 then
//  exit;

//  if fsmenu<>0 then
//  len:= len + (lstrlen(fsmenu)); 
//  if fsmenu<>0 then
//  begin 
//    (* komtie voor? *)
//    if strstr(fsmenu,name) then
//    exit;
//    temp:= mallocN(len+2,'fsmenu'); 
//    strcpy(temp,fsmenu); 
//    strcat(temp,'|'); 
//    strcat(temp,name); 
//    freeN(fsmenu); 
//    fsmenu:= temp; 
//  end;
//  else
//  begin 
//    fsmenu:= mallocN(len+2,'fsmenu'); 
//    strcpy(fsmenu,name); 
//  end;
//end;

//procedure filesel_statistics(sfile: pSpaceFile;  totfile: pinteger;  selfile: pinteger;  totlen: pfloat;  sellen: pfloat); 
//var
//a: integer; 
//len: integer; 
//begin

//  {*}totfile^:=*selfile:=0; 
//  {*}totlen^:=*sellen:=0; 
//  if sfile^.filelist=0 then
//  exit;
//  for{while} a:=0 to Pred(sfile^.totfile) { a++}
//  do
//  begin 
//    if (sfile^.filelist[a].type and S_IFDIR)=0 then
//    begin 
//      (*totfile)++; 
//      len:= sfile^.filelist[a].s.st_size; (*totlen)+=(len div 1048576.0); 

//      if sfile^.filelist[a].flags and ACTIVE then
//      begin 
//        (*selfile)++; 
//        (*sellen)+=(len div 1048576.0); 
//      end;
//    end;
//  end;
//end;

//{$ifdef __sgi}

//procedure workspacecommand(sfile: pSpaceFile); 
//var
//pipein: pFILE; 
//pipeout: pFILE; 
//pipestart: smallint;

//cmd: array [0..Pred(512)] of char; 
//begin

//  pipestart:=LongBool(0); 

//  if sfile^.file[0]<>0 then
//  begin 
//    waitcursor(1); 
//    sprintf(cmd,'%s%s',sfile^.dir,sfile^.file); 
//    if ispic(cmd)=TGA then
//    begin 
//      (* patch *)
//      sprintf(cmd,'paint %s%s',sfile^.dir,sfile^.file); 
//      system(cmd); 
//    end;
//    else
//    begin 
//      sprintf(cmd,'wstype -v %s/%s',sfile^.dir,sfile^.file); 
//      pipein:= popen(cmd,'r'); 
//      pipeout:= popen('sh -s ','w'); 
//      pipestart:= LongBool(0); 
//      if pipein<>0{nil})and(pipeout<>0 then
//      begin 
//        fprintf(pipeout,'cd %s\n',sfile^.dir);
//        fprintf(pipeout,'LEADER=%s/%s\n',sfile^.dir,sfile^.file);
//        fprintf(pipeout,'SELECTED=%s/%s\n',sfile^.dir,sfile^.file);
//        fprintf(pipeout,'ARGC=1\n');

//        while fscanf(pipein,' %511[^\n]\n',cmd)<>EOF
//        do
//        begin 
//          (*fprintf(stderr, "%s\n", cmd);*)
//          if STARTSWITH('TYPE',cmd) then
//          begin 
//            fprintf(pipeout,'LEADERTYPE=%s\n',cmd+5);
//          end;
//          else
//          if STARTSWITH('CMD OPEN',cmd) then
//          begin 
//            pipestart:= LongBool(1); 
//            fprintf(pipeout,'%s\n',cmd+8);
//          end;
//          else
//          if STARTSWITH('CMD',cmd) then
//          begin 
//            pipestart:= LongBool(0); 
//          end;
//          else
//          if pipestart<>0 then
//          begin 
//            fprintf(pipeout,'%s\n',cmd);
//          end;
//        end;
//      end;
//      if pipein<>0{nil} then
//      pclose(pipein); 
//      if pipeout<>0{nil} then
//      pclose(pipeout); 
//    end;
//    waitcursor(0); 
//  end;
//end;
//{$endif}

//(* *************** HULPFUNKTIES ******************* *)

//procedure char_switch(string: pchar;  from: char;  to: char); 
//begin
//  while {*}string^<>0
//  do
//  begin 
//    if {*}string^=from then
//    {*}string^:=to; 
//    inc(string); 
//  end;
//end;

//(* This is a really ugly function... its purpose is to
// * take the space file name and clean it up, replacing
// * excess file entry stuff (like /tmp/../tmp/../)
// *)

//procedure checkdir(dir: pchar); 
//var
//a: smallint; 
//start: pchar; 
//eind: pchar; 
//tmp: array [0..Pred(FILE_MAXDIR+FILE_MAXFILE)] of char; 
//begin

//  make_file_string(tmp,dir,); 
//  strcpy(dir,tmp); 
//  {$ifdef WINDOWS}
//  if dir[0]='.' then
//  begin 
//    (* komt voor, o.a. bij FILE_MAIN *)
//    dir[0]:= #92; 
//    dir[1]:= 0; 
//    exit;
//  end;

//  while start:=strstr(dir,'\..\')
//  do
//  begin 
//    eind:= start+lstrlen('\..\')-1; 
//    a:= start-dir-1; 
//    while a>0
//    do
//    begin 
//      if dir[a]=#92 then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      dec(a); 
//    end;
//    strcpy(dir+a,eind); 
//  end;

//  while start:=strstr(dir,'\.\')
//  do
//  begin 
//    eind:= start+lstrlen('\.\')-1; 
//    strcpy(start,eind); 
//  end;

//  while start:=strstr(dir,'\\')
//  do
//  begin 
//    eind:= start+lstrlen('\\')-1; 
//    strcpy(start,eind); 
//  end;

//  if a:=lstrlen(dir) then
//  begin 
//    (* eerst alle '\s' weghalen aan het eind *)
//    while a>0)and(dir[a-1]=#92
//    do
//    begin 
//      dec(a); 
//      dir[a]:= 0; 
//    end;
//  end;
//  strcat(dir,'\'); 

//  {$else}
//  if dir[0]='.' then
//  begin 
//    (* komt voor, o.a. bij FILE_MAIN *)
//    dir[0]:= '/'; 
//    dir[1]:= 0; 
//    exit;
//  end;
//  while start:=strstr(dir,'/../')
//  do
//  begin 
//    eind:= start+lstrlen('/../')-1; 
//    a:= start-dir-1; 
//    while a>0
//    do
//    begin 
//      if dir[a]='/' then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      dec(a); 
//    end;
//    strcpy(dir+a,eind); 
//  end;

//  while start:=strstr(dir,'/./')
//  do
//  begin 
//    eind:= start+lstrlen('/./')-1; 
//    strcpy(start,eind); 
//  end;

//  while start:=strstr(dir,'//')
//  do
//  begin 
//    eind:= start+lstrlen('//')-1; 
//    strcpy(start,eind); 
//  end;

//  if a:=lstrlen(dir) then
//  begin 
//    (* eerst alle '/' weghalen aan het eind *)
//    while dir[a-1]='/'
//    do
//    begin 
//      dec(a); 
//      dir[a]:= 0; 
//      if a<=0 then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//  end;
//  strcat(dir,'/'); 

//  {$endif}
//end;

//type
//direntry = record
//end;

//procedure test_flags_file(sfile: pSpaceFile); 
//var
//num: integer; 
//mval: array [0..Pred(2)] of smallint; 
//begin

//  file:= sfile^.filelist; 
//  while{for} 
//  num:= 0; 
//  {to} num<sfile^.totfile
//  {;}
//  inc(num); 
//  inc(file); 
//  do
//  begin 
//    file.flags:= 0; 
//    file.type:= file.s.st_mode; (* restore het geknoei van hieronder *)
//    if sfile^.type=FILE_BLENDER)or(sfile^.type=FILE_LOADLIB then
//    begin 
//      if testextensie(file.relname,'.blend') then
//      begin 
//        file.flags:= file.flags or (BLENDERFILE); 
//        if sfile^.type=FILE_LOADLIB then
//        begin 
//          file.type:= file.type and ( not S_IFMT); 
//          file.type:= file.type or (S_IFDIR); 
//        end;
//      end;
//      else
//      if testextensie(file.relname,'.psx') then
//      begin 
//        file.flags:= file.flags or (PSXFILE); 
//      end;
//    end;
//    else
//    if sfile^.type=FILE_SPECIAL then
//    begin 
//      if testextensie(file.relname,'.jpg'))or(testextensie(file.relname,'.tga'))or(testextensie(file.relname,'.rgb'))or(testextensie(file.relname,'.sgi') then
//      begin 
//        file.flags:= file.flags or (IMAGEFILE); 
//      end;
//      else
//      if testextensie(file.relname,'.avi'))or(testextensie(file.relname,'.mv') then
//      begin 
//        file.flags:= file.flags or (MOVIEFILE); 
//      end;
//    end;
//  end;
//end;

//type
//direntry = record
//end;

//procedure sort_filelist(sfile: pSpaceFile); 
//var
//num: integer; 
//act: integer;

//begin

//  act:=0; 
//  case sfile^.sort of
//    FILE_SORTALPHA:
//    begin
//      qsort(sfile^.filelist,sfile^.totfile,sizeof(structdirentry),{!!!a type cast? =>} {pinteger(}compare); 
//    end;
//    FILE_SORTDATE:
//    begin
//      qsort(sfile^.filelist,sfile^.totfile,sizeof(structdirentry),{!!!a type cast? =>} {pinteger(}compare_date); 
//    end;
//    FILE_SORTSIZE:
//    begin
//      qsort(sfile^.filelist,sfile^.totfile,sizeof(structdirentry),{!!!a type cast? =>} {pinteger(}compare_size); 
//    end;
//    FILE_SORTEXTENS:
//    begin
//      qsort(sfile^.filelist,sfile^.totfile,sizeof(structdirentry),{!!!a type cast? =>} {pinteger(}compare); 
//    end;

//  end;{case?}
//  sfile^.act:= -1; 
//  file:= sfile^.filelist; 
//  while{for} 
//  num:= 0; 
//  {to} num<sfile^.totfile
//  {;}
//  inc(num); 
//  inc(file); 
//  do
//  begin 
//    file.flags:= file.flags and ( not HILITE); 
//  end;
//end;

//procedure read_dir(sfile: pSpaceFile); 
//var
//num: integer; 
//len: integer; 
//wdir: array [0..Pred(FILE_MAXDIR)] of char; 
//(* sfile->act wordt gebruikt o.a. bij databrowse: dubbele namen van library objecten *)
//begin
//  {files: pdirentry; }{<= !!!5 external variable}(* storage.c *)
//  {actnum: integer; }{<= !!!5 external variable}
//  {totnum: integer; }{<= !!!5 external variable}(* storage.c *)

//  sfile^.act:= -1; 
//  if sfile^.type=FILE_MAIN then
//  begin 
//    main_to_filelist(sfile); 
//    exit;
//  end;
//  else
//  if sfile^.type=FILE_LOADLIB then
//  begin 
//    library_to_filelist(sfile); 
//    if sfile^.libfiledata<>0 then
//    exit;
//  end;
//  actnum:= totnum:=0; 
//  files:= 0; 
//  hide_dot_files(sfile^.flag and FILE_HIDE_DOT); 

//  (* als actnum een waarde heeft (aantal files) wordt vrijgegeven *)
//  getwdN(wdir); 
//  sfile^.totfile:= getdir(sfile^.dir,@(sfile^.filelist)); 
//  chdir(wdir); 
//  if sfile^.sort<>FILE_SORTALPHA then
//  sort_filelist(sfile); 
//  sfile^.maxnamelen:= 0; 
//  for{while} num:=0 to Pred(sfile^.totfile) { num++}
//  do
//  begin 
//    len:= fmgetstrwidth(G.font,sfile^.filelist[num].relname); 
//    if len>sfile^.maxnamelen then
//    sfile^.maxnamelen:= len; 
//    if filetoname<>0 then
//    begin 
//      if lstrcmp(sfile^.file,sfile^.filelist[num].relname)=0 then
//      begin 
//        sfile^.ofs:= num-(sfile^.collums*(curarea^.winy-FILESELHEAD-20) div (2*FILESEL_DY)); 
//        filetoname:= 0; 
//      end;
//    end;
//  end;
//  test_flags_file(sfile); 
//  filetoname:= 0; 
//end;

procedure freefilelist(sfile: pSpaceFile);
var
num: integer;
begin
  num:= sfile^.totfile-1;

  if sfile^.filelist=nil then
  exit;

  while num>=0 do
  begin
    free(sfile^.filelist[num].relname);

    if sfile^.filelist[num]._string<> nil then
    free(sfile^.filelist[num]._string);

    dec(num);
  end;
  free(sfile^.filelist);
  sfile^.filelist:= nil;
end;

//procedure make_exist(dir: pchar); 
//var
//a: integer; 
//tmp: array [0..Pred(FILE_MAXDIR+FILE_MAXFILE)] of char; 
//{$ifdef WINDOWS}
//begin

//  char_switch(dir,'/',#92); 
//  {$else}
//  char_switch(dir,#92,'/'); 
//  {$endif}
//  a:= lstrlen(dir); 
//  {$ifdef WINDOWS}
//  while fop_exists(dir)=0
//  do
//  begin 
//    dec(a); 
//    while dir[a]<>#92
//    do
//    begin 
//      dec(a); 
//      if a<=0 then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    if a>=0 then
//    dir[a+1]:= 0; 
//    else
//    begin 
//      strcpy(dir,'\'); 
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//  end;
//  {$else}
//  while exist(dir)=0
//  do
//  begin 
//    dec(a); 
//    while dir[a]<>'/'
//    do
//    begin 
//      dec(a); 
//      if a<=0 then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    if a>=0 then
//    dir[a+1]:= 0; 
//    else
//    begin 
//      strcpy(dir,'/'); 
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//  end;
//  {$endif}
//end;

//procedure split_dirfile(string: pchar;  dir: pchar;  file: pchar); 
//var
//a: integer; 
//begin

//  dir[0]:= 0; 
//  file[0]:= 0; 
//  {$ifdef WINDOWS}
//  if lstrlen(string) then
//  begin 
//    if string[0]='/')or(string[0]=#92 then
//    begin 
//      strcpy(dir,string); 
//    end;
//    else
//    if string[1]=':')and(string[2]=#92 then
//    begin 
//      strcpy(dir,string); 
//    end;
//    else
//    begin 
//      getwdN(dir); 
//      strcat(dir,'/'); 
//      strcat(dir,string); 
//      strcpy(string,dir); 
//    end;
//    make_exist(dir); 
//    if S_ISDIR(exist(dir)) then
//    begin 
//      strcpy(file,string+lstrlen(dir)); 

//      if strrchr(file,#92) then
//      strcpy(file,strrchr(file,#92)+1); 
//      if a:=lstrlen(dir) then
//      begin 
//        if dir[a-1]<>#92 then
//        strcat(dir,'\'); 
//      end;
//    end;
//    else
//    begin 
//      a:= lstrlen(dir)-1; 
//      while a>0)and(dir[a]<>#92
//      do
//      dec(a); 
//      dir[a+1]:= 0; 
//      strcpy(file,string+lstrlen(dir)); 
//    end;
//  end;
//  else
//  begin 
//    strcpy(dir,'\'); 
//    file[0]:= 0; 
//  end;
//  {$else}
//  if lstrlen(string) then
//  begin 
//    if string[0]='/' then
//    begin 
//      strcpy(dir,string); 
//    end;
//    else
//    if string[1]=':')and(string[2]=#92 then
//    begin 
//      string:= string + (2); 
//      strcpy(dir,string); 
//    end;
//    else
//    begin 
//      getwdN(dir); 
//      strcat(dir,'/'); 
//      strcat(dir,string); 
//      strcpy(string,dir); 
//    end;
//    make_exist(dir); 

//    if S_ISDIR(exist(dir)) then
//    begin 
//      strcpy(file,string+lstrlen(dir)); 

//      if strrchr(file,'/') then
//      strcpy(file,strrchr(file,'/')+1); 
//      if a:=lstrlen(dir) then
//      begin 
//        if dir[a-1]<>'/' then
//        strcat(dir,'/'); 
//      end;
//    end;
//    else
//    begin 
//      a:= lstrlen(dir)-1; 
//      while dir[a]<>'/'
//      do
//      dec(a); 
//      dir[a+1]:= 0; 
//      strcpy(file,string+lstrlen(dir)); 
//    end;
//  end;
//  else
//  begin 
//    getwdN(dir); 
//    strcat(dir,'/'); 
//    file[0]:= 0; 
//  end;
//  {$endif}
//end;

//procedure split_sfile(sfile: pSpaceFile;  s1: pchar); 
//var
//a: smallint; 
//string: array [0..Pred(FILE_MAXDIR+FILE_MAXFILE)] of char; 
//dir: array [0..Pred(FILE_MAXDIR)] of char; 
//file: array [0..Pred(FILE_MAXFILE)] of char; 
//begin

//  strcpy(string,s1); 
//  split_dirfile(string,dir,file); 

//  if sfile^.filelist<>0 then
//  begin 
//    if lstrcmp(dir,sfile^.dir)<>0 then
//    begin 
//      freefilelist(sfile); 
//    end;
//    else
//    test_flags_file(sfile); 
//  end;
//  strcpy(sfile^.file,file); 
//  make_file_string(sfile^.dir,dir,); 
//end;

//procedure parent(sfile: pSpaceFile); 
//var
//a: smallint; 
//dir: pchar; (* als databrowse: geen parent *)
//begin

//  if sfile^.type=FILE_MAIN)and(sfile^.returnfunc then
//  exit;

//  dir:= sfile^.dir; 
//  {$ifdef WINDOWS}
//  if a:=lstrlen(dir) then
//  begin 
//    (* eerst alle '/' weghalen aan het eind *)
//    while dir[a-1]=#92
//    do
//    begin 
//      dec(a); 
//      dir[a]:= 0; 
//      if a<=0 then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//  end;
//  if a:=lstrlen(dir) then
//  begin 
//    (* daarna alles weghalen tot aan '/' *)
//    while dir[a-1]<>#92
//    do
//    begin 
//      dec(a); 
//      dir[a]:= 0; 
//      if a<=0 then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//  end;
//  if a:=lstrlen(dir) then
//  begin 
//    if dir[a-1]<>#92 then
//    strcat(dir,'\'); 
//  end;
//  else
//  if sfile^.type<>FILE_MAIN then
//  strcpy(dir,'\'); 
//  {$else}
//  if a:=lstrlen(dir) then
//  begin 
//    (* eerst alle '/' weghalen aan het eind *)
//    while dir[a-1]='/'
//    do
//    begin 
//      dec(a); 
//      dir[a]:= 0; 
//      if a<=0 then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//  end;
//  if a:=lstrlen(dir) then
//  begin 
//    (* daarna alles weghalen tot aan '/' *)
//    while dir[a-1]<>'/'
//    do
//    begin 
//      dec(a); 
//      dir[a]:= 0; 
//      if a<=0 then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//  end;
//  if a:=lstrlen(dir) then
//  begin 
//    if dir[a-1]<>'/' then
//    strcat(dir,'/'); 
//  end;
//  else
//  if sfile^.type<>FILE_MAIN then
//  strcpy(dir,'/'); 
//  {$endif}
//  make_exist(sfile^.dir); 
//  (* to be sure *)
//  freefilelist(sfile); 

//  sfile^.ofs:= 0; 
//  addqueue(curarea^.win,REDRAW,1); 
//end;

//type
//direntry = record
//end;

//procedure swapselect_file(sfile: pSpaceFile); 
//var
//num: integer; 
//act: integer;

//begin

//  act:=0; 
//  file:= sfile^.filelist; 
//  while{for} 
//  num:= 0; 
//  {to} num<sfile^.totfile
//  {;}
//  inc(num); 
//  inc(file); 
//  do
//  begin 
//    if file.flags and ACTIVE then
//    begin 
//      act:= 1; 
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//  end;
//  file:= sfile^.filelist+2; 
//  while{for} 
//  num:= 2; 
//  {to} num<sfile^.totfile
//  {;}
//  inc(num); 
//  inc(file); 
//  do
//  begin 
//    if act<>0 then
//    file.flags:= file.flags and ( not ACTIVE); 
//    else
//    file.flags:= file.flags or (ACTIVE); 
//  end;
//end;

//function find_active_file(sfile: pSpaceFile;  x: smallint;  y: smallint): integer; 
//var
//ofs: integer; 
//begin

//  if y>textrct.ymax then
//  y:= textrct.ymax; 
//  if y<=textrct.ymin then
//  y:= textrct.ymin+1; 
//  ofs:= (x-textrct.xmin) div collumwidth; 
//  if ofs<0 then
//  ofs:= 0; 
//  ofs:= ofs * ((textrct.ymax-textrct.ymin)); 
//  begin
//    result:= sfile^.ofs+(ofs+textrct.ymax-y) div FILESEL_DY; 
//    exit;
//  end;
//end;

//(* ********************** DRAW ******************************* *)

//procedure calc_file_rcts(sfile: pSpaceFile); 
//var
//tot: integer; 
//h: integer; 
//len: integer; 
//fac: float; 
//start: float; 
//totfile: float; 
//begin

//  scrollrct.xmin:= 15; 
//  scrollrct.xmax:= 35; 
//  scrollrct.ymin:= 10; 
//  scrollrct.ymax:= curarea^.winy-10-FILESELHEAD; 
//  textrct.xmin:= scrollrct.xmax+10; 
//  textrct.xmax:= curarea^.winx-10; 
//  textrct.ymin:= scrollrct.ymin; 
//  textrct.ymax:= scrollrct.ymax; 
//  if textrct.xmax-textrct.xmin<60 then
//  textrct.xmax:= textrct.xmin+60; 
//  len:= (textrct.ymax-textrct.ymin) mod FILESEL_DY; 
//  textrct.ymin:= textrct.ymin + (len); 
//  scrollrct.ymin:= scrollrct.ymin + (len); 
//  filebuty1:= curarea^.winy-FILESELHEAD; 
//  filebuty2:= filebuty1+FILESELHEAD div 2-6; 
//  len:= sfile^.maxnamelen+25; (* aantal kolommen *)
//  if sfile^.type=FILE_MAIN then
//  len:= len + (100); 
//  else
//  if sfile^.flag and FILE_SHOWSHORT then
//  len:= len + (100); 
//  else
//  len:= len + (380); 
//  sfile^.collums:= (textrct.xmax-textrct.xmin) div len; 
//  if sfile^.collums<1 then
//  sfile^.collums:= 1; 
//  else
//  if sfile^.collums>8 then
//  sfile^.collums:= 8; 
//  if (U.flag and FSCOLLUM)=0 then
//  if sfile^.type<>FILE_MAIN then
//  sfile^.collums:= 1; 
//  collumwidth:= (textrct.xmax-textrct.xmin) div sfile^.collums; 
//  totfile:= sfile^.totfile+0.5; 
//  tot:= FILESEL_DY*totfile; 

//  if tot<>0 then
//  fac:= ({!!!a type cast? =>} {float(}sfile^.collums*(scrollrct.ymax-scrollrct.ymin)) div ({!!!a type cast? =>} {float(}tot); 
//  else
//  fac:= 1.0; 
//  if sfile^.ofs<0 then
//  sfile^.ofs:= 0; 
//  if tot<>0 then
//  start:= ({!!!a type cast? =>} {float(}sfile^.ofs) div (totfile); 
//  else
//  start:= 0.0; 
//  if fac>1.0 then
//  fac:= 1.0; 
//  if start+fac>1.0 then
//  begin 
//    sfile^.ofs:= fceil((1.0-fac)*totfile); 
//    start:= ({!!!a type cast? =>} {float(}sfile^.ofs) div (totfile); 
//    fac:= 1.0-start; 
//  end;
//  bar.xmin:= scrollrct.xmin+2; 
//  bar.xmax:= scrollrct.xmax-2; 
//  h:= (scrollrct.ymax-scrollrct.ymin)-4; 
//  bar.ymax:= scrollrct.ymax-2-start*h; 
//  bar.ymin:= bar.ymax-fac*h; 
//  pixels_to_ofs:= (totfile) div {!!!a type cast? =>} {float(}(h+3); 
//  page_ofs:= fac*totfile; 

//end;

//filescrollselect: integer = 0; 

//procedure draw_filescroll(sfile: pSpaceFile); 
//begin
//  if scrollrct.ymin+10>=scrollrct.ymax then
//  exit;
//  cpack($707070); 
//  glRecti(scrollrct.xmin,scrollrct.ymin,scrollrct.xmax,scrollrct.ymax); 
//  EmbossBox2(scrollrct.xmin-2,scrollrct.ymin-2,scrollrct.xmax+2,scrollrct.ymax+1,1,$404040,$A0A0A0); 

//  cpack($909090); 

//  glRecti(bar.xmin,bar.ymin,bar.xmax,bar.ymax); 
//  EmbossBox2(bar.xmin-2,bar.ymin-2,bar.xmax+2,bar.ymax+2,filescrollselect,$404040,$A0A0A0); 

//end;

//procedure regelrect(col: uint;  x: integer;  y: integer); 
//begin
//  cpack(col); 
//  glRects(x-17,y-3,x+collumwidth-21,y+11); 
//end;

//procedure printregel(sfile: pSpaceFile;  files: pdirentry;  x: integer;  y: integer); 
//var
//boxcol: uint;

//s: pchar; 
//begin
//  boxcol:=0; 
//  case files.flags and (HILITE+ACTIVE) of
//    HILITE+ACTIVE:
//    begin
//      boxcol:= ($C08080); 
//    end;
//    HILITE:
//    begin
//      boxcol:= ($808080); 
//    end;
//    ACTIVE:
//    begin
//      boxcol:= ($C07070); 
//    end;

//  end;{case?}

//  if boxcol<>0 then
//  begin 
//    regelrect(boxcol,x,y); 
//  end;

//  if files.flags and BLENDERFILE then
//  begin 
//    cpack($A0A0); 
//    glRects(x-14,y,x-8,y+7); 
//  end;
//  else
//  if files.flags and PSXFILE then
//  begin 
//    cpack($A060B0); 
//    glRects(x-14,y,x-8,y+7); 
//  end;
//  else
//  if files.flags and IMAGEFILE then
//  begin 
//    cpack($F08040); 
//    glRects(x-14,y,x-8,y+7); 
//  end;
//  else
//  if files.flags and MOVIEFILE then
//  begin 
//    cpack($70A070); 
//    glRects(x-14,y,x-8,y+7); 
//  end;
//  if S_ISDIR(files.type) then
//  cpack($FFFFFF); 
//  else
//  cpack($0); 
//  fmsetfont(G.font); 

//  s:= files.string; 
//  if s<>0 then
//  begin 
//    glRasterPos2i(x,y); 
//    fmprstr(files.relname); 
//    x:= x + (sfile^.maxnamelen+100); 
//    glRasterPos2i(x-fmgetstrwidth(G.font,files.size),y); 
//    (* glutFontDraw(files->relname, x, y); *)

//    fmprstr(files.size); 

//    if sfile^.flag and FILE_SHOWSHORT then

//    {$ifndef WINDOWS}
//    exit;
//    (* rwx rwx rwx *)
//    x:= x + (20); 
//    glRasterPos2i(x,y); 
//    fmprstr(files.mode1); 
//    x:= x + (30); 
//    glRasterPos2i(x,y); 
//    fmprstr(files.mode2); 
//    x:= x + (30); 
//    glRasterPos2i(x,y); 
//    fmprstr(files.mode3); 
//    x:= x + (30); 
//    glRasterPos2i(x,y); 
//    (* owner time date *)
//    fmprstr(files.owner); 
//    {$endif}
//    x:= x + (60); 
//    glRasterPos2i(x,y); 
//    fmprstr(files.time); 
//    x:= x + (50); 
//    glRasterPos2i(x,y); 
//    fmprstr(files.date); 
//  end;
//  else
//  begin 
//    glRasterPos2i(x,y); 
//    fmprstr(files.relname); 
//    if files.nr<>0 then
//    begin 
//      (* extra info *)
//      x:= x + (sfile^.maxnamelen+20); 
//      glRasterPos2i(x,y); 
//      fmprstr(files.extra); 
//    end;
//  end;
//end;

//function calc_filesel_regel(sfile: pSpaceFile;  nr: integer;  valx: pinteger;  valy: pinteger): integer; 
//var
//val: integer; 
//coll: integer; 
//begin(* sco van de de regel *)

//  nr:= nr - (sfile^.ofs); 
//  val:= (textrct.ymax-textrct.ymin) div FILESEL_DY; 
//  coll:= nr div val; 
//  nr:= nr - (coll*val); {*}valy^:=textrct.ymax-FILESEL_DY+3-nr*FILESEL_DY; 
//  {*}valx^:=coll*collumwidth+textrct.xmin+20; 

//  (* aantal regels in de hoogte *)
//  if nr<0)or(coll>sfile^.collums then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  begin
//    result:= 1; 
//    exit;
//  end;
//end;

//type
//direntry = record
//end;

//procedure set_active_file(sfile: pSpaceFile;  act: integer); 
//var
//num: integer; 
//redraw: integer;

//newflag: integer; 
//old: integer;

//new: integer;

//x: integer; 
//y: integer; 
//begin

//  redraw:=0; 

//  old:=0; 
//  new:=0; 
//  file:= sfile^.filelist; 
//  if file=0 then
//  exit;
//  while{for} 
//  num:= 0; 
//  {to} num<sfile^.totfile
//  {;}
//  inc(num); 
//  inc(file); 
//  do
//  begin 
//    if num=act then
//    begin 
//      if selecting)and(num>1 then
//      begin 
//        newflag:= HILITE or (file.flags and  not ACTIVE); 
//        if selecting=ACTIVATE then
//        newflag:= newflag or (ACTIVE); 
//        if file.flags<>newflag then
//        redraw:= redraw or (1); 
//        file.flags:= newflag; 
//      end;
//      else
//      begin 
//        if file.flags and HILITE then
//        ; 
//        else
//        begin 
//          file.flags:= file.flags or (HILITE); 
//          redraw:= redraw or (2); 
//          new:= num; 
//        end;
//      end;
//    end;
//    else
//    begin 
//      if file.flags and HILITE then
//      begin 
//        file.flags:= file.flags and ( not HILITE); 
//        redraw:= redraw or (2); 
//        old:= num; 
//      end;
//    end;
//  end;
//  {$ifdef IRISGL}
//  if redraw<>0 then
//  begin 
//    addqueue(curarea^.win,REDRAW,1); 
//  end;
//  {$else}
//  if redraw=2 then
//  begin 

//    glDrawBuffer(GL_FRONT); 
//    glScissor(curarea^.winrct.xmin,curarea^.winrct.ymin,curarea^.winx-12,curarea^.winy); 

//    if calc_filesel_regel(sfile,old,@x,@y) then
//    begin 
//      regelrect($686868,x,y); 
//      printregel(sfile,sfile^.filelist+old,x,y); 
//    end;
//    (* myCopySubBuffer(x+curarea->winrct.xmin-20, y+curarea->winrct.ymin-4, collumwidth, FILESEL_DY+7); *)
//    if calc_filesel_regel(sfile,new,@x,@y) then
//    begin 
//      glDrawBuffer(GL_FRONT); 
//      printregel(sfile,sfile^.filelist+new,x,y); 
//    end;
//    (* myCopySubBuffer(x+curarea->winrct.xmin-20, y+curarea->winrct.ymin-4, collumwidth, FILESEL_DY+7); *)
//    glScissor(curarea^.winrct.xmin,curarea^.winrct.ymin,curarea^.winx,curarea^.winy); 
//    glDrawBuffer(GL_BACK); 

//  end;
//  else
//  if redraw<>0 then
//  begin 
//    addqueue(curarea^.win,REDRAW,1); 
//  end;
//  {$endif}
//end;

//type
//direntry = record
//end;

//procedure draw_filetext(sfile: pSpaceFile); 
//var
//a: integer; 
//x: integer; 
//y: integer; 
//coll: integer; 
//mval: array [0..Pred(2)] of smallint; 
//begin

//  if textrct.ymin+10>=textrct.ymax then
//  exit;

//  (* kader *)
//  cpack($686868); 
//  glRecti(textrct.xmin,textrct.ymin,textrct.xmax,textrct.ymax); 
//  x:= textrct.xmin+collumwidth; 
//  (* kolommen *)
//  while{for} 
//  a:= 1; 
//  {to} a<sfile^.collums
//  {;}
//  inc(a); 
//  x:= x + (collumwidth); 
//  do
//  begin 
//    cpack($303030); 
//    sdrawline(x,textrct.ymin,x,textrct.ymax); 
//    cpack($B0B0B0); 
//    sdrawline(x+1,textrct.ymin,x+1,textrct.ymax); 
//  end;
//  fmsetfont(G.font); 

//  (* de teksten *)
//  if sfile^.filelist=0 then
//  exit;
//  (* test: als muis niet in area staat: de HILITE wissen *)
//  getmouseco_areawin(mval); 
//  if mval[0]<0)or(mval[0]>curarea^.winx then
//  begin 
//    files:= sfile^.filelist+sfile^.ofs; 
//    while{for} 
//    a:= sfile^.ofs; 
//    {to} a<sfile^.totfile
//    {;}
//    inc(a); 
//    inc(files); 
//    do
//    files.flags:= files.flags and ( not HILITE); 
//  end;
//  files:= sfile^.filelist+sfile^.ofs; 
//  while{for} 
//  a:= sfile^.ofs; 
//  {to} a<sfile^.totfile
//  {;}
//  inc(a); 
//  inc(files); 
//  do
//  begin 
//    if calc_filesel_regel(sfile,a,@x,@y)=0 then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    printregel(sfile,files,x,y); 
//  end;
//  EmbossBox2(textrct.xmin-2,textrct.ymin-2,textrct.xmax+2,textrct.ymax+2,1,$303030,$C0C0C0); 

//  (* wissen tekenfoutjes, tekst teveel aan de rechterkant: *)
//  glColor3f(.5625,.5625,.5625); 
//  glRecti(textrct.xmax+2,textrct.ymin,textrct.xmax+10,textrct.ymax); 
//end;

//procedure drawfilespace; 
//var
//sfile: pSpaceFile; 
//act: integer; 
//mval: array [0..Pred(2)] of smallint; 
//naam: array [0..Pred(20)] of char; 
//begin

//  glClearColor(.5625,.5625,.5625,0.0); 
//  glClear(GL_COLOR_BUFFER_BIT); 
//  sfile:= curarea^.spacedata.first; 
//  if sfile^.filelist=0 then
//  begin 
//    read_dir(sfile); 
//    calc_file_rcts(sfile); 
//    getmouseco_areawin(mval); 
//    (* act berekenen *)
//    act:= find_active_file(sfile,mval[0],mval[1]); 
//    if act>=0)and(act<sfile^.totfile then
//    sfile^.filelist[act].flags:= sfile^.filelist[act].flags or (HILITE); 
//  end;
//  else
//  calc_file_rcts(sfile); 
//  sprintf(naam,'win %d',curarea^.win); 

//  (* HEADER *)
//  DefButBlock(naam,curarea^.win,G.font,10,6,4); 
//  SetButLock(sfile^.type=FILE_MAIN)and(sfile^.returnfunc,0{nil}); 

//  DefBut(TEX,1,,textrct.xmin,filebuty1,textrct.xmax-textrct.xmin,21,sfile^.file,0.0,{!!!a type cast? =>} {float(}FILE_MAXFILE-1,0,0); 

//  DefBut(TEX,2,,textrct.xmin,filebuty2,textrct.xmax-textrct.xmin,21,sfile^.dir,0.0,{!!!a type cast? =>} {float(}FILE_MAXFILE-1,0,0); 
//  SetButShape(2); 
//  if fsmenu<>0 then
//  DefBut(MENU or SHO,3,fsmenu,scrollrct.xmin,filebuty1,scrollrct.xmax-scrollrct.xmin,21,@sfile^.menu,0,0,0,0); 
//  DefButt(BUT,4,'P',scrollrct.xmin,filebuty2,scrollrct.xmax-scrollrct.xmin,21,0,0,0,0,0,'Move to the parent directory (PKEY)'); 
//  draw_filescroll(sfile); 

//  draw_filetext(sfile); 
//  addqueue(curarea^.headwin,REDRAW,1); 
//  (* andere diskfree etc ? *)
//  curarea^.win_swap:= WIN_BACK_OK; 

//end;

//procedure do_filescroll(sfile: pSpaceFile); 
//var
//fac: float; 
//tot: integer; 
//dy: integer; 
//mval: array [0..Pred(2)] of smallint; 
//oldy: smallint; 
//yo: smallint; 
//begin

//  tot:= FILESEL_DY*sfile^.totfile; 
//  if tot<>0 then
//  fac:= ({!!!a type cast? =>} {float(}(scrollrct.ymax-scrollrct.ymin)) div ({!!!a type cast? =>} {float(}tot); 
//  else
//  fac:= 1.0; 
//  calc_file_rcts(sfile); 

//  filescrollselect:= 1; 
//  glDrawBuffer(GL_FRONT); 
//  (* voor mooiigheid *)

//  EmbossBox2(bar.xmin-2,bar.ymin-2,bar.xmax+2,bar.ymax+2,filescrollselect,$404040,$A0A0A0); 
//  glDrawBuffer(GL_BACK); 
//  getmouseco_areawin(mval); 
//  oldy:= yo:=mval[1]; 
//  while get_mbut()@L_MOUSE
//  do
//  begin 
//    getmouseco_areawin(mval); 
//    if yo<>mval[1] then
//    begin 
//      dy:= ffloor(0.5+({!!!a type cast? =>} {float(}(oldy-mval[1]))*pixels_to_ofs); 
//      if dy<>0 then
//      begin 
//        sfile^.ofs:= sfile^.ofs + (dy); 
//        if sfile^.ofs<0 then
//        begin 
//          sfile^.ofs:= 0; 
//          oldy:= mval[1]; 
//        end;
//        else
//        oldy:= ffloor(0.5+{!!!a type cast? =>} {float(}oldy-{!!!a type cast? =>} {float(}dy div pixels_to_ofs); 
//        curarea^.windraw(); 
//        screen_swapbuffers(); 
//      end;
//      yo:= mval[1]; 
//    end;
//    else
//    usleep(2); 
//  end;
//  filescrollselect:= 0; 
//  glDrawBuffer(GL_FRONT); 

//  (* voor mooiigheid *)
//  EmbossBox2(bar.xmin-2,bar.ymin-2,bar.xmax+2,bar.ymax+2,filescrollselect,$404040,$A0A0A0); 
//  glDrawBuffer(GL_BACK); 
//end;

procedure activate_fileselect(_type: integer;  title: pchar;  _file: pchar; func: retfunc);
var
sfile: pSpaceFile;
str: pchar;
group: array [0..23] of char;
name: array [0..FILE_MAXDIR - 1] of char;
temp: array [0..FILE_MAXFILE - 1] of char;
begin
  if curarea=nil then
  exit;
  if curarea^.win=0 then
  exit;

  newspace(curarea,SPACE_FILE);
  addqueue(curarea^.win,REDRAW,1);

  (* image wegpushen? Niet bij databrowse *)
  //if R.winpop=0)and(R.win)and({not}0=(type=FILE_MAIN)and(func) then
  //begin
  //  toggle_render_display();
  //end;

  (* is misschien dubbelop, voor geval area al SPACE_FILE is met andere filenaam *)
  addqueue(curarea^.headwin,CHANGED,1);

  name[2]:= #0;
  strcpy(name,_file);

  sfile:= curarea^.spacedata.first;
  sfile^.returnfunc:= func;
  sfile^._type:= _type;
  sfile^.ofs:= 0;
  (* sfile->act wordt gebruikt bij databrowse: dubbele namen van library objecten *)
  sfile^.act:= -1;

  if convertstringcode(name)<>0 then
  sfile^.flag:= sfile^.flag or (FILE_STRINGCODE)
  else
  sfile^.flag:= sfile^.flag and ( not FILE_STRINGCODE);

  if _type=FILE_MAIN then
  begin
    strcpy(sfile^._file,name+2);

    str:= code_to_groupname(GS(name));
    strcpy(sfile^.dir,str);

    (* alles vrijgeven *)
    if sfile^.libfiledata<>nil then
    freeN(sfile^.libfiledata);
    sfile^.libfiledata:= nil;

    freefilelist(sfile);
  end
  else
  if _type=FILE_LOADLIB then
  begin
    //strcpy(sfile^.dir,name);
    //if is_a_library(sfile,temp,group) then
    //begin
    //  (* dit geval is om een reload van library-filelist te veroorzaken *)
    //  if sfile^.libfiledata=nil then
    //  begin
    //    freefilelist(sfile);
    //  end;
    //end
    //else
    //begin
    //  split_sfile(sfile,name);
    //  if sfile^.libfiledata<>nil then
    //  freeN(sfile^.libfiledata);
    //  sfile^.libfiledata:= 0;
    //end;
  end
  else
  begin
    //(* FILE_BLENDER *)
    //split_sfile(sfile,name); (* test ook de filelist *)

    //(* vrijgeven: filelist en libfiledata kloppen niet meer *)
    //if sfile^.libfiledata<>nil then
    //freeN(sfile^.libfiledata);
    //sfile^.libfiledata:= 0;
  end;
  strncpy(sfile^.title,title,23);
  filetoname:= 1;
end;

//procedure activate_imageselect(type: integer;  title: pchar;  file: pchar;  

//procedure func{!!!3 unknown typedef}; 
//); 
//var
//simasel: pSpaceImaSel; 
//str: pchar; 
//dir: array [0..Pred(FILE_MAXDIR)] of char; 
//name: array [0..Pred(FILE_MAXFILE)] of char; 
//begin

//  if curarea=0 then
//  exit;
//  if curarea^.win=0 then
//  exit;
//  newspace(curarea,SPACE_IMASEL); 
//  (* image wegpushen? Niet bij databrowse *)
//  if R.winpop=0)and(R.win)and(func then
//  begin 
//    toggle_render_display(); 
//  end;
//  addqueue(curarea^.headwin,CHANGED,1); 
//  (* is misschien dubbelop, voor geval area al SPACE_FILE is met andere filenaam *)
//  addqueue(curarea^.win,CHANGED,1); 
//  name[2]:= 0; 
//  strcpy(name,file); 

//  simasel:= curarea^.spacedata.first; 
//  simasel.returnfunc:= func; 

//  if convertstringcode(name) then
//  simasel.mode:= simasel.mode or (IMS_STRINGCODE); 
//  else
//  simasel.mode:= simasel.mode and ( not IMS_STRINGCODE); 
//  split_dirfile(name,dir,simasel.file); 
//  if lstrcmp(dir,simasel.dir)<>0 then
//  simasel.fase:= 0; 
//  strcpy(simasel.dir,dir); 
//  strncpy(simasel.title,title,23); 
//end;
//(* filetoname= 1; *)

//procedure activate_databrowse(id: pID;  idcode: integer;  fromcode: integer;  retval: integer;  

//procedure func{!!!3 unknown typedef}; 
//); 
//var
//lb: pListBase; 
//sfile: pSpaceFile; 
//str: array [0..Pred(32)] of char; 
//begin

//  if id=0 then
//  begin 
//    lb:= wich_libbase(G.main,idcode); 
//    id:= lb.first; 
//  end;
//  if id<>0 then
//  strcpy(str,id.name); 
//  else
//  exit;
//  activate_fileselect(FILE_MAIN,'SELECT DATABLOCK',str,(void(* )(char* ))func); 
//  sfile:= curarea^.spacedata.first; 
//  sfile^.retval:= retval; 
//  sfile^.ipotype:= fromcode; 
//end;

//procedure filesel_prevspace; 
//var
//sfile: pSpaceFile; 
//begin

//  sfile:= curarea^.spacedata.first; 
//  if sfile^.next<>0 then
//  begin 
//    remlink( and curarea^.spacedata,sfile); 
//    addtail( and curarea^.spacedata,sfile); 
//    sfile:= curarea^.spacedata.first; 
//    newspace(curarea,sfile^.spacetype); 

//  end;
//  else
//  newspace(curarea,SPACE_INFO); 
//end;

//function countselect(sfile: pSpaceFile): integer; 
//var
//a: integer; 
//count: integer;

//begin

//  count:=0; 
//  for{while} a:=0 to Pred(sfile^.totfile) { a++}
//  do
//  begin 
//    if sfile^.filelist[a].flags and ACTIVE then
//    begin 
//      inc(count); 
//    end;
//  end;
//  begin
//    result:= count; 
//    exit;
//  end;
//end;

//function getotherdir: integer; 
//var
//sa: pScrArea; 
//sfile: pSpaceFile;

//begin

//  sfile:=0; 
//  sa:= G.curscreen.areabase.first; 
//  while sa
//  do
//  begin 
//    if sa<>curarea then
//    begin 
//      if sa.spacetype=SPACE_FILE then
//      begin 
//        (* al een gevonden *)
//        if sfile<>0 then
//        begin
//          result:= 0; 
//          exit;
//        end;
//        sfile:= sa.spacedata.first; 
//        if sfile^.type and FILE_UNIX then
//        begin 
//          otherarea:= sa; 
//          make_file_string(otherdir,sfile^.dir,); 
//        end;
//        else
//        sfile:= 0; 
//      end;
//    end;
//    sa:= sa.next; 
//  end;
//  if sfile<>0 then
//  begin
//    result:= 1; 
//    exit;
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;

//procedure reread_other_fs; 
//var
//sfile: pSpaceFile; (* oppassen: alleen aanroepen als getotherdir goed is afgelopen *)
//begin
//  sfile:= otherarea.spacedata.first; 
//  freefilelist(sfile); 
//  addqueue(otherarea.win,REDRAW,1); 
//end;

//procedure free_filesel_spec(dir: pchar); 
//var
//sc: pbScreen; 
//sa: pScrArea; 
//sfile: pSpaceFile; 
//len: integer; 
//str: array [0..Pred(FILE_MAXDIR+FILE_MAXFILE)] of char; (* dir moet eindigen op '/' *)
//begin(* alle filesels met 'dir' worden vrijgegeven *)

//  strcpy(str,dir); 
//  len:= lstrlen(str); 
//  while len)and(str[len-1]<>'/'
//  do
//  begin 
//    dec(len); 
//  end;
//  if len=0 then
//  exit;
//  sc:= G.main.screen.first; 
//  while sc
//  do
//  begin 
//    sa:= sc.areabase.first; 
//    while sa
//    do
//    begin 
//      sfile:= sa.spacedata.first; 
//      while sfile
//      do
//      begin 
//        if sfile^.spacetype=SPACE_FILE then
//        begin 
//          if lstrcmp(sfile^.dir,str)=0 then
//          freefilelist(sfile); 
//        end;
//        sfile:= sfile^.next; 
//      end;
//      sa:= sa.next; 
//    end;
//    sc:= sc.id.next; 
//  end;
//end;

//type
//direntry = record
//end;

//procedure filesel_execute(sfile: pSpaceFile); 
//var
//a: integer; 
//menup: psmallint; 
//name: array [0..Pred(FILE_MAXDIR)] of char; 
//begin

//  filesel_prevspace(); 

//  if sfile^.type=FILE_LOADLIB then
//  begin 
//    library_append(sfile); 
//    allqueue(REDRAWALL,1); 
//  end;
//  else
//  if sfile^.returnfunc<>0 then
//  begin 
//    addfilename_to_fsmenu(sfile^.dir); 
//    if sfile^.type=FILE_MAIN then
//    begin 
//      if sfile^.retval=B_TEXBROWSE then
//      menup:= @G.buts.texnr; 
//      else
//      if sfile^.retval=B_WTEXBROWSE then
//      menup:= @G.buts.texnr; 
//      else
//      if sfile^.retval=B_LTEXBROWSE then
//      menup:= @G.buts.texnr; 
//      else
//      if sfile^.retval=B_IPOBROWSE then
//      menup:= @G.sipo.menunr; 
//      else
//      if sfile^.retval=B_INFOSCE then
//      menup:= @G.curscreen.scenenr; 
//      else
//      if sfile^.retval=B_INFOSCR then
//      menup:= @G.curscreen.screennr; 
//      else
//      if sfile^.retval=B_SIMABROWSE then
//      menup:= @G.sima.imanr; 
//      else
//      menup:= @G.buts.menunr; 
//      if sfile^.act>=0 then
//      begin 
//        if sfile^.filelist<>0 then
//        begin 
//          files:= sfile^.filelist+sfile^.act; {*}menup^:=files.nr-1; 
//        end;
//        else
//        {*}menup^:=sfile^.act; 
//      end;
//      else
//      begin 
//        {*}menup^:=-1; 
//        for{while} a:=0 to Pred(sfile^.totfile) { a++}
//        do
//        begin 
//          if lstrcmp(sfile^.filelist[a].relname,sfile^.file)=0 then
//          begin 
//            {*}menup^:=a; 
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//        end;
//      end;
//      sfile^.returnfunc(sfile^.retval); 
//    end;
//    else
//    begin 
//      if strncmp(sfile^.title,'SAVE',4)=0 then
//      free_filesel_spec(sfile^.dir); 
//      strcpy(name,sfile^.dir); 
//      strcat(name,sfile^.file); 
//      if sfile^.flag and FILE_STRINGCODE then
//      makestringcode(name); 
//      sfile^.returnfunc(name); 

//    end;
//  end;
//end;

//procedure do_filesel_buttons(event: smallint;  sfile: pSpaceFile); 
//var
//butname: array [0..Pred(FILE_MAXDIR)] of char; 
//i: integer; 
//match: integer;

//begin

//  if event=1 then
//  begin 
//    if strchr(sfile^.file,'*'))or(strchr(sfile^.file,'?'))or(strchr(sfile^.file,'[') then
//    begin 

//      match:=LongBool(0); 
//      for{while} i:=2 to Pred(sfile^.totfile) { i++}
//      do
//      begin 
//        if fnmatch(sfile^.file,sfile^.filelist[i].relname,0)=0 then
//        begin 
//          sfile^.filelist[i].flags:= sfile^.filelist[i].flags or (ACTIVE); 
//          match:= LongBool(1); 
//        end;
//      end;
//      if match<>0 then
//      strcpy(sfile^.file,); 
//      if sfile^.type=FILE_MAIN then
//      filesel_select_objects(sfile); 
//      addqueue(curarea^.win,REDRAW,1); 
//    end;
//  end;
//  else
//  if event=2 then
//  begin 
//    (* reuse the butname variable *)
//    checkdir(sfile^.dir); 
//    make_file_string(butname,sfile^.dir,); 

//    (* strip the trailing slash if its a real dir *)
//    if lstrlen(butname)<>1 then
//    butname[lstrlen(butname)-1]:= 0; 
//    if sfile^.type and FILE_UNIX then
//    begin 
//      if {not}0=fop_exists(butname) then
//      begin 
//        if okee('Makedir') then
//        begin 
//          fop_recurdir(butname); 
//          if {not}0=fop_exists(butname) then
//          parent(sfile); 
//        end;
//        else
//        parent(sfile); 
//      end;
//    end;
//    freefilelist(sfile); 
//    sfile^.ofs:= 0; 
//    addqueue(curarea^.win,REDRAW,1); 
//  end;
//  else
//  if event=3 then
//  begin 
//    (* welke string *)
//    getname_menu_but(butname,fsmenu,sfile^.menu); 
//    strcpy(sfile^.dir,butname); 
//    make_exist(sfile^.dir); 
//    checkdir(sfile^.dir); 
//    freefilelist(sfile); 
//    sfile^.ofs:= 0; 
//    addqueue(curarea^.win,REDRAW,1); 
//    sfile^.act:= -1; 
//  end;
//  else
//  if event=4 then
//  parent(sfile); 
//end;

//function docmd(cmd: pchar): integer; 
//var
//file: integer; 
//err: integer; 
//size: integer; 
//val: smallint; 
//begin

//  if qtest() then
//  begin 
//    if extern_qread(@val)=ESCKEY then
//    begin 
//      begin
//        result:= (-1); 
//        exit;
//      end;
//    end;
//  end;
//  waitcursor(1); 
//  (* 
//   if(fslog[0]==0) {
//    sprintf(fslog, "/tmp/fslog%d", getpid());
//   }

//   strcpy(string, cmd);
//   strcat(string, " 1>/dev/null 2>>");
//   strcat(string, fslog);

//   remove(fslog);
//  *)

//  err:= system(cmd); 
//  waitcursor(0); 

//  (*
//   if (err = system(string)){
//    if (file = open(fslog, O_BINARY|O_RDONLY)){
//     if ((size = read(file, string, sizeof(string)-1)) > 0){
//      string[size] = 0;
//      error(string);
//     } else error(cmd);
//     close(file);
//    } else error(cmd);
//   }
//   remove(fslog);
//  *)

//  begin
//    result:= (err); 
//    exit;
//  end;
//end;

//procedure databrowse_replace(sfile: pSpaceFile); 
//var
//old: pMaterial; 
//new: pMaterial; 
//i: integer; 
//str: array [0..Pred(128)] of char; 
//begin

//  if lstrcmp(sfile^.dir,'Material/')=0 then
//  begin 
//    if sfile^.act=-1 then
//    begin 
//      error('Select with leftmouse'); 
//      exit;
//    end;
//    new:= {!!!a type cast? =>} {pMaterial(}sfile^.filelist[sfile^.act].poin; 
//    if new<>0 then
//    begin 
//      sprintf(str,'Replace with Material %s',new.id.name+2); 
//      if okee(str)=0 then
//      exit;
//      for{while} i:=0 to Pred(sfile^.totfile) { i++}
//      do
//      begin 
//        if sfile^.filelist[i].flags and ACTIVE then
//        begin 
//          old:= {!!!a type cast? =>} {pMaterial(}sfile^.filelist[i].poin; 
//          replace_material(old,new); 
//        end;
//      end;
//      freefilelist(sfile); 
//      allqueue(REDRAWBUTSMAT,0); 
//    end;
//  end;
//  addqueue(curarea^.win,REDRAW,1); 
//end;

//procedure fs_fake_users(sfile: pSpaceFile); 
//var
//id: pID; 
//a: integer; (* alleen bij F4 DATABROWSE *)
//begin

//  if sfile^.returnfunc<>0 then
//  exit;
//  for{while} a:=0 to Pred(sfile^.totfile) { a++}
//  do
//  begin 
//    if sfile^.filelist[a].flags and ACTIVE then
//    begin 
//      id:= {!!!a type cast? =>} {pID(}sfile^.filelist[a].poin; 
//      if id<>0 then
//      begin 
//        if id.flag and LIB_FAKEUSER then
//        begin 
//          id.flag:= id.flag - (LIB_FAKEUSER); 
//          dec(id.us); 
//        end;
//        else
//        begin 
//          id.flag:= id.flag or (LIB_FAKEUSER); 
//          inc(id.us); 
//        end;
//      end;
//    end;
//  end;
//  freefilelist(sfile); 
//  addqueue(curarea^.win,REDRAW,1); 
//end;

//procedure make_file_string(string: pchar;  dir: pchar;  file: pchar); 
//var
//lslash: pchar; (* Get the file name, chop everything past the last slash (ie. the filename) *)
//begin
//  if {not}0=string)or({not}0=dir)or({not}0=file then
//  exit;
//  (* We don't want any NULLs *)
//  string[0]:= 0; (* ton *)

//  (* Resolve relative references *)
//  if dir[0]='/')and(dir[1]='/' then
//  begin 
//    strcpy(string,G.sce); 
//    lslash:= (strrchr(string,'/')>strrchr(string,#92)) {was ?}if then strrchr(string,'/') {was :}else strrchr(string,#92); 
//    if lslash<>0 then
//    *(lslash+1):=0; 
//    dir:= dir + (2); 

//    (* Skip over the relative reference *)
//  end;
//  strcat(string,dir); 

//  (* Make sure string ends in one (and only one) slash *)
//  if string[lstrlen(string)-1]<>'/')and(string[lstrlen(string)-1]<>#92 then
//  strcat(string,'/'); 
//  while {*}file^)and((*file='/')or(*file=#92)
//  do
//  inc(file); 
//  strcat(string,file); 
//  (* Trim slashes from the front of file *)
//  (* Push all slashes to the system preferred direction *)
//  {$ifdef WINDOWS}
//  char_switch(string,'/',#92); 
//  {$else}
//  char_switch(string,#92,'/'); 
//  {$endif}
//end;

//type
//tms = record
//end;
//var {was static}
//acto: integer;

//prevtime: integer;

//procedure winqreadfilespace(event: ushort;  val: smallint); 
//var
//sfile: pSpaceFile; 
//act: integer; 
//do_draw: integer;

//i: integer; 
//test: integer; 
//newtime: integer; 
//ret: integer; 
//qual: smallint; 
//mval: array [0..Pred(2)] of smallint; 
//numlen: ushort; 
//head: array [0..Pred(FILE_MAXDIR)] of char; 
//tail: array [0..Pred(FILE_MAXFILE)] of char; 
//str: array [0..Pred(FILE_MAXDIR+FILE_MAXFILE+12)] of char; 
//begin
//  acto:=0; 
//  prevtime:=0; 

//  do_draw:=0; 

//  sfile:= curarea^.spacedata.first; 
//  if sfile=0 then
//  exit;
//  if sfile^.filelist=0 then
//  begin 
//    (* wel buttons doen *)
//    if val)and(event=LEFTMOUSE then
//    begin 
//      FrontbufferButs(LongBool(1)); 
//      event:= DoButtons(); 
//      FrontbufferButs(LongBool(0)); 
//      if event<>0 then
//      do_filesel_buttons(event,sfile); 
//    end;
//    (*  NIET de headerbuttons! *)
//    exit;
//  end;
//  if curarea^.win=0 then
//  exit;
//  calc_file_rcts(sfile); 
//  getmouseco_areawin(mval); 

//  (* om hangen te voorkomen *)
//  if selecting)and({not}0=(get_mbut()@R_MOUSE) then
//  selecting:= 0; 

//  if val<>0 then
//  begin 
//    case event of
//      LEFTMOUSE,
//      MIDDLEMOUSE:
//      begin
//        if mval[0]>scrollrct.xmin)and(mval[0]<scrollrct.xmax)and(mval[1]>scrollrct.ymin)and(mval[1]<scrollrct.ymax then
//        begin 
//          do_filescroll(sfile); 
//        end;
//        else
//        if mval[0]>textrct.xmin)and(mval[0]<textrct.xmax)and(mval[1]>textrct.ymin)and(mval[1]<textrct.ymax then
//        begin 
//          (* sfile->act wordt gebruikt bij databrowse: dubbelenamen van library objecten *)
//          sfile^.act:= act:=find_active_file(sfile,mval[0],mval[1]); 
//          if act>=0)and(act<sfile^.totfile then
//          begin 
//            if S_ISDIR(sfile^.filelist[act].type) then
//            begin 
//              strcat(sfile^.dir,sfile^.filelist[act].relname); 
//              strcat(sfile^.dir,'/'); 
//              checkdir(sfile^.dir); 
//              freefilelist(sfile); 
//              sfile^.ofs:= 0; 
//              do_draw:= 1; 
//            end;
//            else
//            begin 
//              if lstrcmp(sfile^.file,sfile^.filelist[act].relname) then
//              begin 
//                do_draw:= 1; 
//                strcpy(sfile^.file,sfile^.filelist[act].relname); 
//              end;
//              if event=MIDDLEMOUSE)and(sfile^.type then
//              filesel_execute(sfile); 
//            end;
//          end;
//        end;
//        else
//        begin 
//          FrontbufferButs(LongBool(1)); 
//          event:= DoButtons(); 
//          FrontbufferButs(LongBool(0)); 
//          if event<>0 then
//          do_filesel_buttons(event,sfile); 
//        end;
//        (*  NIET de headerbuttons! *)
//        {$ifdef __sgi}
//        if event=LEFTMOUSE)and((sfile^.type and (FILE_UNIX)) then
//        begin 
//          if mval[0]>textrct.xmin)and(mval[0]<textrct.xmax)and(mval[1]>textrct.ymin)and(mval[1]<textrct.ymax then
//          begin 
//            newtime:= times(@voidbuf); 
//            if (newtime-prevtime<=20) then
//            begin 
//              workspacecommand(sfile); 
//              newtime:= newtime - (60); 
//            end;
//            prevtime:= newtime; 
//          end;
//        end;
//        {$endif}
//      end;
//      RIGHTMOUSE:
//      begin
//        act:= find_active_file(sfile,mval[0],mval[1]); 
//        acto:= act; 
//        if act>=0)and(act<sfile^.totfile then
//        begin 

//          if sfile^.filelist[act].flags and ACTIVE then
//          begin 
//            sfile^.filelist[act].flags:= sfile^.filelist[act].flags and ( not ACTIVE); 
//            selecting:= INACTIVATE; 
//          end;
//          else
//          begin 
//            test:= sfile^.filelist[act].relname[0]; 
//            if act>=2)or(test<>'.' then
//            sfile^.filelist[act].flags:= sfile^.filelist[act].flags or (ACTIVE); 
//            selecting:= ACTIVATE; 
//          end;
//          do_draw:= 1; 
//        end;
//      end;
//      MOUSEY:
//      begin
//        act:= find_active_file(sfile,mval[0],mval[1]); 
//        set_active_file(sfile,act); 
//        if selecting)and(act<>acto then
//        begin 
//          while 1
//          do
//          begin 
//            if acto>=2)and(acto<sfile^.totfile then
//            begin 
//              if selecting=ACTIVATE then
//              sfile^.filelist[acto].flags:= sfile^.filelist[acto].flags or (ACTIVE); 
//              else
//              if selecting=INACTIVATE then
//              sfile^.filelist[acto].flags:= sfile^.filelist[acto].flags and ( not ACTIVE); 
//            end;
//            if acto<act then
//            inc(acto); 
//            else
//            if acto>act then
//            dec(acto); 
//            else
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//        end;

//        acto:= act; 
//      end;
//      PAGEUPKEY:
//      begin
//        sfile^.ofs:= sfile^.ofs - (page_ofs); 
//        do_draw:= 1; 
//      end;
//      PAGEDOWNKEY:
//      begin
//        sfile^.ofs:= sfile^.ofs + (page_ofs); 
//        do_draw:= 1; 
//      end;
//      HOMEKEY:
//      begin
//        sfile^.ofs:= 0; 
//        do_draw:= 1; 
//      end;
//      ENDKEY:
//      begin
//        sfile^.ofs:= sfile^.totfile; 
//        do_draw:= 1; 
//      end;
//      AKEY:
//      begin
//        swapselect_file(sfile); 
//        if sfile^.type=FILE_MAIN then
//        filesel_select_objects(sfile); 
//        do_draw:= 1; 
//      end;
//      BKEY,
//      CKEY,
//      LKEY:
//      begin
//        if event=LKEY)and(sfile^.type=FILE_MAIN)and((G.qual and LR_CTRLKEY) then
//        begin 
//          databrowse_replace(sfile); 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        (* doorgeven *)
//        {goto next_label;}{<= !!!d case label without "break"}
//      end;
//      MKEY:
//      begin
//        if sfile^.type=FILE_MAIN then
//        break; {<= !!!b possible in "switch" - then remove this line}

//        if {not}0=countselect(sfile) then
//        begin 
//          error('No files selected'); 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        if {not}0=getotherdir() then
//        begin 
//          error('No second fileselect'); 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        if {not}0=lstrcmp(sfile^.dir,otherdir) then
//        begin 
//          error('Same directories'); 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;

//        if event=BKEY then
//        sprintf(str,'Backup to %s',otherdir); 
//        else
//        if event=CKEY then
//        sprintf(str,'Copy to %s',otherdir); 
//        else
//        if event=LKEY then
//        sprintf(str,'Linked copy to %s',otherdir); 
//        else
//        if event=MKEY then
//        sprintf(str,'Move to %s',otherdir); 
//        if {not}0=okee(str) then
//        break; {<= !!!b possible in "switch" - then remove this line}

//        for{while} i:=0 to Pred(sfile^.totfile) { i++}
//        do
//        begin 
//          if sfile^.filelist[i].flags and ACTIVE then
//          begin 
//            make_file_string(str,sfile^.dir,sfile^.filelist[i].relname); 

//            if event=BKEY then
//            ret:= fop_backup(sfile^.filelist[i].relname,sfile^.dir,otherdir); 
//            else
//            if event=CKEY then
//            ret:= fop_copy(str,otherdir); 
//            else
//            if event=LKEY then
//            ret:= fop_link(str,otherdir); 
//            else
//            if event=MKEY then
//            ret:= fop_move(str,otherdir); 
//            if ret<>0 then
//            begin 
//              error('Command failed, see console'); 
//              break; {<= !!!b possible in "switch" - then remove this line}
//            end;
//            else
//            sfile^.filelist[i].flags:= sfile^.filelist[i].flags and ( not ACTIVE); 
//          end;
//        end;
//        do_draw:= 1; 
//        if event=BKEY)or(event=MKEY then
//        freefilelist(sfile); 
//        reread_other_fs(); 
//      end;
//      RKEY:
//      begin
//        if sfile^.type=FILE_MAIN then
//        begin 
//          databrowse_replace(sfile); 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        (* doorgeven aan TKEY! *)
//        {goto next_label;}{<= !!!d case label without "break"}
//      end;
//      TKEY:
//      begin
//        if sfile^.type=FILE_MAIN then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        if {not}0=countselect(sfile) then
//        begin 
//          error('No files selected'); 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;

//        if event=TKEY then
//        sprintf(str,'Touch'); 
//        else
//        if event=RKEY then
//        sprintf(str,'Remove from %s',sfile^.dir); 
//        qual:= G.qual; (* want na okee() heb je de shift losgelaten *)
//        if {not}0=okee(str) then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        for{while} i:=0 to Pred(sfile^.totfile) { i++}
//        do
//        begin 
//          if sfile^.filelist[i].flags and ACTIVE then
//          begin 
//            make_file_string(str,sfile^.dir,sfile^.filelist[i].relname); 

//            if event=TKEY then
//            ret:= fop_touch(str); 
//            else
//            if event=RKEY then
//            begin 
//              if qual and LR_SHIFTKEY then
//              ret:= fop_delete(str,0,1); 
//              else
//              if S_ISDIR(sfile^.filelist[i].type) then
//              ret:= fop_delete(str,1,0); 
//              else
//              ret:= fop_delete(str,0,0); 
//            end;

//            if ret<>0 then
//            begin 
//              error('Command failed, see console'); 
//              break; {<= !!!b possible in "switch" - then remove this line}
//            end;
//            else
//            sfile^.filelist[i].flags:= sfile^.filelist[i].flags and ( not ACTIVE); 
//          end;
//        end;
//        do_draw:= 1; 
//        freefilelist(sfile); 
//      end;
//      PKEY:

//      begin
//        if G.qual and LR_SHIFTKEY then
//        begin 
//          {bprogname: array [0..] of char; }{<= !!!5 external variable}(* usiblender.c *)
//          sprintf(str,'%s -a %s%s',bprogname,sfile^.dir,sfile^.file); 
//          system(str); 
//        end;
//        else
//        parent(sfile); 
//      end;
//      IKEY:

//      begin
//        if sfile^.type=FILE_MAIN then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        sprintf(str,'$IMAGEEDITOR %s%s',sfile^.dir,sfile^.file); 
//        system(str); 
//      end;
//      EKEY:
//      begin
//        if sfile^.type=FILE_MAIN then
//        break; {<= !!!b possible in "switch" - then remove this line}
//        sprintf(str,'$WINEDITOR %s%s',sfile^.dir,sfile^.file); 
//        system(str); 
//      end;
//      FKEY:
//      begin
//        if sfile^.type=FILE_MAIN then
//        begin 
//          fs_fake_users(sfile); 
//        end;
//      end;
//      PADPLUSKEY,
//      EQUALKEY:
//      begin
//        if G.qual and LR_CTRLKEY then
//        newname(sfile^.file,+100); 
//        else
//        if G.qual and LR_SHIFTKEY then
//        newname(sfile^.file,+10); 
//        else
//        newname(sfile^.file,+1); 
//        do_draw:= 1; 
//      end;
//      PADMINUS,
//      MINUSKEY:
//      begin
//        if G.qual and LR_CTRLKEY then
//        newname(sfile^.file,-100); 
//        else
//        if G.qual and LR_SHIFTKEY then
//        newname(sfile^.file,-10); 
//        else
//        newname(sfile^.file,-1); 
//        do_draw:= 1; 
//      end;
//      BACKSLASHKEY,
//      SLASHKEY:
//      begin
//        if sfile^.type=FILE_MAIN then

//        {$ifdef WINDOWS}
//        break; {<= !!!b possible in "switch" - then remove this line}
//        strcpy(sfile^.dir,'\'); 
//        {$else}
//        strcpy(sfile^.dir,'/'); 
//        {$endif}
//        freefilelist(sfile); 
//        sfile^.ofs:= 0; 
//        do_draw:= 1; 
//      end;
//      PERIODKEY:
//      begin
//        freefilelist(sfile); 
//        do_draw:= 1; 
//      end;
//      ESCKEY:
//      begin
//        filesel_prevspace(); 
//      end;
//      PADENTER,
//      RETKEY:
//      begin
//        if sfile^.type<>0 then
//        filesel_execute(sfile); 
//      end;

//    end;{case?}
//  end;
//  else
//  if event=RIGHTMOUSE then
//  begin 
//    selecting:= NOTACTIVE; 
//    if sfile^.type=FILE_MAIN then
//    filesel_select_objects(sfile); 
//  end;
//  else
//  if event=LEFTMOUSE then
//  begin 
//    if sfile^.type=FILE_MAIN then
//    active_file_object(sfile); 
//  end;

//  if do_draw<>0 then
//  addqueue(curarea^.win,REDRAW,1); 
//end;

//(* ************* LIBRARY FILESEL ******************* *)

//libdir: array [0..Pred(100)] of integer; 

//function groupname_to_code(group: pchar): integer; 
//begin
//  if strncmp(group,'Scene',5)=0 then
//  begin
//    result:= ID_SCE; 
//    exit;
//  end;
//  if strncmp(group,'Object',6)=0 then
//  begin
//    result:= ID_OB; 
//    exit;
//  end;
//  if strncmp(group,'Mesh',4)=0 then
//  begin
//    result:= ID_ME; 
//    exit;
//  end;
//  if strncmp(group,'Curve',5)=0 then
//  begin
//    result:= ID_CU; 
//    exit;
//  end;
//  if strncmp(group,'Metaball',8)=0 then
//  begin
//    result:= ID_MB; 
//    exit;
//  end;
//  if strncmp(group,'Material',8)=0 then
//  begin
//    result:= ID_MA; 
//    exit;
//  end;
//  if strncmp(group,'Texture',7)=0 then
//  begin
//    result:= ID_TE; 
//    exit;
//  end;
//  if strncmp(group,'Image',5)=0 then
//  begin
//    result:= ID_IM; 
//    exit;
//  end;
//  if strncmp(group,'Ika',3)=0 then
//  begin
//    result:= ID_IK; 
//    exit;
//  end;
//  if strncmp(group,'Wave',4)=0 then
//  begin
//    result:= ID_WV; 
//    exit;
//  end;
//  if strncmp(group,'Lattice',7)=0 then
//  begin
//    result:= ID_LT; 
//    exit;
//  end;
//  if strncmp(group,'Lamp',4)=0 then
//  begin
//    result:= ID_LA; 
//    exit;
//  end;
//  if strncmp(group,'Camera',6)=0 then
//  begin
//    result:= ID_CA; 
//    exit;
//  end;
//  if strncmp(group,'Key',3)=0 then
//  begin
//    result:= ID_KE; 
//    exit;
//  end;
//  if strncmp(group,'Ipo',3)=0 then
//  begin
//    result:= ID_IP; 
//    exit;
//  end;
//  if strncmp(group,'World',5)=0 then
//  begin
//    result:= ID_WO; 
//    exit;
//  end;
//  if strncmp(group,'Screen',6)=0 then
//  begin
//    result:= ID_SCR; 
//    exit;
//  end;
//  if strncmp(group,'VFont',5)=0 then
//  begin
//    result:= ID_VF; 
//    exit;
//  end;
//  if strncmp(group,'Text',4)=0 then
//  begin
//    result:= ID_TXT; 
//    exit;
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;

function code_to_groupname(code: integer): pchar;
var
  str: pchar;
begin
  case code of
    ID_SCE: str := 'Scene/';
    //ID_OB:  str := 'Object/';
    //ID_ME:  str := 'Mesh/';
    //ID_CU:  str := 'Curve/';
    //ID_MB:  str := 'Metaball/';
    //ID_MA:  str := 'Material/';
    //ID_TE:  str := 'Texture/';
    //ID_IM:  str := 'Image/';
    //ID_IK:  str := 'Ika/';
    //ID_WV:  str := 'Wave/';
    //ID_LT:  str := 'Lattice/';
    //ID_LA:  str := 'Lamp/';
    //ID_CA:  str := 'Camera/';
    //ID_KE:  str := 'Key/';
    //ID_IP:  str := 'Ipo/';
    //ID_WO:  str := 'World/';
    //ID_TXT: str := 'Text/';
    //ID_SCR: str := 'Screen/';
    //ID_VF:  str := 'Vfont/';
    else
      str:= '';
  end;
  exit(str);
end;

//function count_libfiles(sfile: pSpaceFile;  idcode: integer): integer; 
//var
//bhead: pBHead; 
//tot: integer;

//afbreek: integer;

//fd: pchar; 
//begin

//  tot:=0; 
//  afbreek:=0; 

//  fd:= sfile^.libfiledata; 
//  while afbreek=0
//  do
//  begin 
//    bhead:= {!!!a type cast? =>} {pBHead(}fd; 
//    if bhead.code=ENDB then
//    afbreek:= 1; 
//    else
//    if bhead.code=idcode then
//    inc(tot); 
//    fd:= fd + (bhead.len+sizeof(BHead)); 
//  end;
//  begin
//    result:= tot; 
//    exit;
//  end;
//end;

//function count_libdirs(sfile: pSpaceFile): integer; 
//var
//bhead: pBHead; 
//tot: integer;

//a: integer; 
//afbreek: integer;

//fd: pchar; 
//begin

//  tot:=0; 

//  afbreek:=0; 

//  libdir[0]:= 0; 
//  fd:= sfile^.libfiledata; 
//  while afbreek=0
//  do
//  begin 
//    bhead:= {!!!a type cast? =>} {pBHead(}fd; 
//    case bhead.code of
//      GLOB,
//      DATA,
//      USER,
//      TEST,
//      DNA1,
//      ID_SCR,
//      ID_LA,
//      ID_CA:
//      begin
//      end;
//      ENDB:
//      begin
//        afbreek:= 1; 
//      end;
//      else
//      begin
//        for{while} a:=0 to Pred(tot) { a++}
//        do
//        begin 
//          if libdir[a]=bhead.code then
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        if a=tot then
//        begin 
//          libdir[tot]:= bhead.code; 
//          inc(tot); 
//          if tot>99 then
//          tot:= 99; 
//        end;
//      end;
//    end;{case?}
//    fd:= fd + (bhead.len+sizeof(BHead)); 
//  end;
//  begin
//    result:= tot; 
//    exit;
//  end;
//end;

//function is_a_library(sfile: pSpaceFile;  dir: pchar;  group: pchar): integer; 
//var
//len: integer; 
//fd: pchar; 
//begin(* return ok als een blenderfile, in dir staat de filenaam,
//    * in group het type libdata
//    *)

//  strcpy(dir,sfile^.dir); 
//  len:= lstrlen(dir); 
//  if len<7 then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  if dir[len-1]<>'/')and(dir[len-1]<>#92 then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  group[0]:= 0; 
//  dir[len-1]:= 0; 
//  fd:= (strrchr(dir,'/')>strrchr(dir,#92)) {was ?}if then strrchr(dir,'/') {was :}else strrchr(dir,#92); 
//  (* Find the last slash *)

//  if fd=0 then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  {*}fd^:=0; 
//  if testextensie(fd+1,'.blend') then
//  begin 
//    {*}fd^:='/'; 
//  end;
//  else
//  begin 
//    strcpy(group,fd+1); 
//    fd:= (strrchr(dir,'/')>strrchr(dir,#92)) {was ?}if then strrchr(dir,'/') {was :}else strrchr(dir,#92); 
//    (* Find the last slash *)
//    if fd=0)or(testextensie(fd+1,'.blend')=0 then
//    begin
//      result:= 0; 
//      exit;
//    end;
//  end;
//  begin
//    result:= 1; 
//    exit;
//  end;
//end;

//procedure library_to_filelist(sfile: pSpaceFile); 
//var
//bhead: pBHead; 
//id: pID; 
//file: integer; 
//filelen: integer; 
//temp: integer; 
//temp1: integer; 
//a: integer; 
//actual: integer; 
//ok: integer; 
//idcode: integer; 
//len: integer; 
//dir: array [0..Pred(FILE_MAXDIR)] of char; 
//group: array [0..Pred(24)] of char; 
//fd: pchar; 
//str: pchar; 
//(* naam testen *)
//begin

//  ok:= is_a_library(sfile,dir,group); 
//  if ok=0 then
//  begin 
//    (* vrijgeven *)
//    if sfile^.libfiledata<>0 then
//    freeN(sfile^.libfiledata); 
//    sfile^.libfiledata:= 0; 
//    exit;
//  end;
//  (* en daar gaat ie *)
//  (* voorlopig alleen filedata inlezen als libfiledata==0 *)
//  if sfile^.libfiledata=0 then
//  begin 
//    sfile^.libfiledata:= openblenderfile(dir, and filelen); 
//    if sfile^.libfiledata=0 then
//    exit;
//  end;
//  if group[0]=0 then
//  begin 
//    (* directories maken *)
//    sfile^.totfile:= count_libdirs(sfile); 
//    sfile^.totfile:= sfile^.totfile + (2); 
//    sfile^.filelist:= (structdirentry* )malloc(sfile^.totfile*sizeof(structdirentry)); 
//    actual:= 2; 
//    for{while} a:=0 to Pred(sfile^.totfile) { a++}
//    do
//    begin 
//      RtlZeroMemory(@(sfile^.filelist[a]),sizeof(structdirentry)); 
//      if a=0 then
//      begin 
//        sfile^.filelist[a].relname:= strdup('.'); 
//        sfile^.filelist[a].type:= sfile^.filelist[a].type or (S_IFDIR); 
//      end;
//      else
//      if a=1 then
//      begin 
//        sfile^.filelist[a].relname:= strdup('..'); 
//        sfile^.filelist[a].type:= sfile^.filelist[a].type or (S_IFDIR); 
//      end;
//      else
//      begin 
//        case libdir[a-2] of
//          ID_SCE:
//          begin
//            str:= 'Scene'; 
//          end;
//          ID_OB:
//          begin
//            str:= 'Object'; 
//          end;
//          ID_ME:
//          begin
//            str:= 'Mesh'; 
//          end;
//          ID_CU:
//          begin
//            str:= 'Curve'; 
//          end;
//          ID_MB:
//          begin
//            str:= 'Metaball'; 
//          end;
//          ID_MA:
//          begin
//            str:= 'Material'; 
//          end;
//          ID_TE:
//          begin
//            str:= 'Texture'; 
//          end;
//          ID_IP:
//          begin
//            str:= 'Ipo'; 
//          end;
//          ID_WO:
//          begin
//            str:= 'World'; 
//          end;
//          ID_TXT:
//          begin
//            str:= 'Text'; 
//          end;
//          ID_LA:
//          begin
//            str:= 'Lamp'; 
//          end;
//          else
//          begin
//            str:= 0; 
//            dec(actual); *({!!!a type cast? =>} {psmallint(}group):=libdir[a-2]; 
//            group[3]:= 0; 
//          end;
//        end;{case?}
//        if str<>0 then
//        begin 
//          sfile^.filelist[actual].relname:= strdup(str); 
//          sfile^.filelist[actual].type:= sfile^.filelist[actual].type or (S_IFDIR); 
//        end;
//        inc(actual); 
//      end;
//    end;
//    sfile^.totfile:= actual; 
//    qsort(sfile^.filelist,actual,sizeof(structdirentry),{!!!a type cast? =>} {pinteger(}compare); 
//  end;
//  else
//  begin 

//    (* files maken *)
//    idcode:= groupname_to_code(group); 
//    sfile^.totfile:= count_libfiles(sfile,idcode); 
//    sfile^.totfile:= sfile^.totfile + (2); 
//    sfile^.filelist:= (structdirentry* )malloc(sfile^.totfile*sizeof(structdirentry)); 
//    RtlZeroMemory(@(sfile^.filelist[0]),sizeof(structdirentry)); 
//    sfile^.filelist[0].relname:= strdup('.'); 
//    sfile^.filelist[0].type:= sfile^.filelist[0].type or (S_IFDIR); 
//    RtlZeroMemory(@(sfile^.filelist[1]),sizeof(structdirentry)); 
//    sfile^.filelist[1].relname:= strdup('..'); 
//    sfile^.filelist[1].type:= sfile^.filelist[1].type or (S_IFDIR); 
//    actual:= 2; 
//    fd:= sfile^.libfiledata; 
//    while LongBool(1)
//    do
//    begin 
//      bhead:= {!!!a type cast? =>} {pBHead(}fd; 
//      if bhead.code=ENDB then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      else
//      if bhead.code=idcode then
//      begin 
//        RtlZeroMemory(@(sfile^.filelist[actual]),sizeof(structdirentry)); 
//        id:= {!!!a type cast? =>} {pID(}(bhead+1); 
//        sfile^.filelist[actual].relname:= strdup(id.name+2); 
//        inc(actual); 

//      end;
//      fd:= fd + (bhead.len+sizeof(BHead)); 

//    end;
//    qsort(sfile^.filelist,sfile^.totfile,sizeof(structdirentry),{!!!a type cast? =>} {pinteger(}compare); 
//  end;
//  sfile^.maxnamelen:= 0; 
//  for{while} a:=0 to Pred(sfile^.totfile) { a++}
//  do
//  begin 
//    len:= fmgetstrwidth(G.font,sfile^.filelist[a].relname); 
//    if len>sfile^.maxnamelen then
//    sfile^.maxnamelen:= len; 
//  end;
//end;

//(* ******************* DATA SELECT ********************* *)

//procedure filesel_select_objects(sfile: pSpaceFile); 
//var
//ob: pObject; 
//base: pBase; 
//sce: pScene; 
//a: integer; (* alleen bij F4 DATABROWSE *)
//begin

//  if sfile^.returnfunc<>0 then
//  exit;
//  if lstrcmp(sfile^.dir,'Object/')=0 then
//  begin 
//    for{while} a:=0 to Pred(sfile^.totfile) { a++}
//    do
//    begin 
//      ob:= {!!!a type cast? =>} {pObject(}sfile^.filelist[a].poin; 
//      if ob<>0 then
//      begin 
//        if sfile^.filelist[a].flags and ACTIVE then
//        ob.flag:= ob.flag or (SELECT); 
//        else
//        ob.flag:= ob.flag and ( not SELECT); 
//      end;
//    end;

//    base:= FIRSTBASE; 
//    while base
//    do
//    begin 
//      base.flag:= base.object.flag; 
//      base:= base.next; 
//    end;
//    allqueue(REDRAWVIEW3D,0); 
//  end;
//  else
//  if lstrcmp(sfile^.dir,'Scene/')=0 then
//  begin 
//    for{while} a:=0 to Pred(sfile^.totfile) { a++}
//    do
//    begin 
//      sce:= {!!!a type cast? =>} {pScene(}sfile^.filelist[a].poin; 
//      if sce<>0 then
//      begin 
//        if sfile^.filelist[a].flags and ACTIVE then
//        sce.r.scemode:= sce.r.scemode or (R_BG_RENDER); 
//        else
//        sce.r.scemode:= sce.r.scemode and ( not R_BG_RENDER); 
//      end;
//    end;

//    allqueue(REDRAWBUTSRENDER,0); 
//  end;
//end;

//procedure active_file_object(sfile: pSpaceFile); 
//var
//ob: pObject; 
//base: pBase; 
//sce: pScene; 
//a: integer; (* alleen bij F4 DATABROWSE *)
//begin

//  if sfile^.returnfunc<>0 then
//  exit;
//  if lstrcmp(sfile^.dir,'Object/')=0 then
//  begin 
//    if sfile^.act>=0 then
//    begin 
//      ob:= {!!!a type cast? =>} {pObject(}sfile^.filelist[sfile^.act].poin; 
//      if ob<>0 then
//      begin 
//        set_active_object(ob); 
//        if BASACT)and(BASACT.object=ob then
//        begin 
//          BASACT.flag:= BASACT.flag or (SELECT); 
//          sfile^.filelist[sfile^.act].flags:= sfile^.filelist[sfile^.act].flags or (ACTIVE); 
//          allqueue(REDRAWVIEW3D,0); 
//          addqueue(curarea^.win,REDRAW,1); 
//        end;
//      end;
//    end;
//  end;
//end;

//type
//direntry = record
//end;

//procedure main_to_filelist(sfile: pSpaceFile); 
//var
//id: pID; 
//lb: pListBase; 
//a: integer; 
//fake: integer; 
//idcode: integer; 
//len: integer; 
//ok: integer; 
//totlib: integer; 
//totbl: integer; 
//dir: array [0..Pred(FILE_MAXDIR)] of char; 
//group: array [0..Pred(24)] of char; 
//fd: pchar; 
//str: pchar; 
//begin

//  if sfile^.dir[0]='/' then
//  sfile^.dir[0]:= 0; 
//  if sfile^.dir[0]<>0 then
//  begin 
//    idcode:= groupname_to_code(sfile^.dir); 
//    if idcode=0 then
//    sfile^.dir[0]:= 0; 
//  end;
//  if sfile^.dir[0]=0 then
//  begin 
//    (* directories maken *)
//    sfile^.totfile:= 19; 
//    sfile^.filelist:= (structdirentry* )malloc(sfile^.totfile*sizeof(structdirentry)); 
//    for{while} a:=0 to Pred(sfile^.totfile) { a++}
//    do
//    begin 
//      RtlZeroMemory(@(sfile^.filelist[a]),sizeof(structdirentry)); 
//      sfile^.filelist[a].type:= sfile^.filelist[a].type or (S_IFDIR); 
//    end;
//    sfile^.filelist[0].relname:= strdup('..'); 
//    sfile^.filelist[1].relname:= strdup('.'); 
//    sfile^.filelist[2].relname:= strdup('Scene'); 
//    sfile^.filelist[3].relname:= strdup('Object'); 
//    sfile^.filelist[4].relname:= strdup('Mesh'); 
//    sfile^.filelist[5].relname:= strdup('Curve'); 
//    sfile^.filelist[6].relname:= strdup('Metaball'); 
//    sfile^.filelist[7].relname:= strdup('Material'); 
//    sfile^.filelist[8].relname:= strdup('Texture'); 
//    sfile^.filelist[9].relname:= strdup('Image'); 
//    sfile^.filelist[10].relname:= strdup('Ika'); 
//    sfile^.filelist[11].relname:= strdup('Wave'); 
//    sfile^.filelist[12].relname:= strdup('Lattice'); 
//    sfile^.filelist[13].relname:= strdup('Lamp'); 
//    sfile^.filelist[14].relname:= strdup('Camera'); 
//    sfile^.filelist[15].relname:= strdup('Ipo'); 
//    sfile^.filelist[16].relname:= strdup('World'); 
//    sfile^.filelist[17].relname:= strdup('Screen'); 
//    sfile^.filelist[18].relname:= strdup('VFont'); 
//    sfile^.filelist[18].relname:= strdup('Text'); 
//    qsort(sfile^.filelist,sfile^.totfile,sizeof(structdirentry),{!!!a type cast? =>} {pinteger(}compare); 
//  end;
//  else
//  begin 

//    (* files maken *)
//    idcode:= groupname_to_code(sfile^.dir); 
//    lb:= wich_libbase(G.main,idcode); 
//    if lb=0 then
//    exit;
//    id:= lb.first; 
//    sfile^.totfile:= 0; 
//    while id
//    do
//    begin 
//      if sfile^.returnfunc)and(idcode=ID_IP then
//      begin 
//        if sfile^.ipotype=({!!!a type cast? =>} {pIpo(}id).blocktype then
//        inc(sfile^.totfile); 
//      end;
//      else
//      inc(sfile^.totfile); 
//      id:= id.next; 
//    end;
//    if sfile^.returnfunc=0 then
//    sfile^.totfile:= sfile^.totfile + (2); 
//    sfile^.filelist:= (structdirentry* )malloc(sfile^.totfile*sizeof(structdirentry)); 
//    files:= sfile^.filelist; 
//    if sfile^.returnfunc=0 then
//    begin 
//      RtlZeroMemory(@(sfile^.filelist[0]),sizeof(structdirentry)); 
//      sfile^.filelist[0].relname:= strdup('.'); 
//      sfile^.filelist[0].type:= sfile^.filelist[0].type or (S_IFDIR); 
//      RtlZeroMemory(@(sfile^.filelist[1]),sizeof(structdirentry)); 
//      sfile^.filelist[1].relname:= strdup('..'); 
//      sfile^.filelist[1].type:= sfile^.filelist[1].type or (S_IFDIR); 
//      files:= files + (2); 
//    end;
//    id:= lb.first; 
//    totlib:= totbl:=0; 
//    while id
//    do
//    begin 
//      ok:= 0; 
//      if sfile^.returnfunc)and(idcode=ID_IP then
//      begin 
//        if sfile^.ipotype=({!!!a type cast? =>} {pIpo(}id).blocktype then
//        ok:= 1; 
//      end;
//      else
//      ok:= 1; 
//      if ok<>0 then
//      begin 
//        RtlZeroMemory(files,sizeof(structdirentry)); 
//        files.relname:= strdup(id.name+2); 
//        if sfile^.returnfunc=0 then
//        begin 
//          (* F4 DATA BROWSE *)
//          if idcode=ID_OB then
//          begin 
//            if ({!!!a type cast? =>} {pObject(}id).flag and SELECT then
//            files.flags:= files.flags or (ACTIVE); 
//          end;
//          else
//          if idcode=ID_SCE then
//          begin 
//            if ({!!!a type cast? =>} {pScene(}id).r.scemode and R_BG_RENDER then
//            files.flags:= files.flags or (ACTIVE); 
//          end;
//        end;
//        files.nr:= totbl+1; 
//        files.poin:= id; 
//        fake:= id.flag and LIB_FAKEUSER; 
//        if id.lib)and(fake then
//        sprintf(files.extra,'LF %d',id.us); 
//        else
//        if id.lib<>0 then
//        sprintf(files.extra,'L    %d',id.us); 
//        else
//        if fake<>0 then
//        sprintf(files.extra,'F    %d',id.us); 
//        else
//        sprintf(files.extra,'      %d',id.us); 
//        if id.lib<>0 then
//        begin 
//          if totlib=0 then
//          firstlib:= files; 
//          inc(totlib); 
//        end;
//        inc(files); 
//        inc(totbl); 
//      end;
//      id:= id.next; 
//    end;
//    (* alleen qsort van libraryblokken *)
//    if totlib>1 then
//    begin 
//      qsort(firstlib,totlib,sizeof(structdirentry),{!!!a type cast? =>} {pinteger(}compare); 
//    end;
//  end;
//  sfile^.maxnamelen:= 0; 
//  for{while} a:=0 to Pred(sfile^.totfile) { a++}
//  do
//  begin 
//    len:= fmgetstrwidth(G.font,sfile^.filelist[a].relname); 
//    if len>sfile^.maxnamelen then
//    sfile^.maxnamelen:= len; 
//    if filetoname<>0 then
//    begin 
//      if lstrcmp(sfile^.file,sfile^.filelist[a].relname)=0 then
//      begin 
//        sfile^.ofs:= a-(sfile^.collums*(curarea^.winy-FILESELHEAD-10) div (2*FILESEL_DY)); 
//        filetoname:= 0; 
//        if sfile^.returnfunc<>0 then
//        sfile^.filelist[a].flags:= sfile^.filelist[a].flags or (ACTIVE); 
//      end;
//    end;
//  end;
//end;

//{STARTSWITH(x, y) (strncmp(x, y, sizeof(x) - 1) == 0)}
//function STARTSWITH(x: integer; y: integer): integer; 
//begin
//  result:= (strncmp(x,y,sizeof(x)-1)=0)
//end;

end.
