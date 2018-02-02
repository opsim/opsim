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

(*  readfile.c   juni 94     MIXED MODEL

 *
 *  jan 95
 *  maart 95
 *
 *   ZWAK PUNT: newadres berekening en meerdere files: oplossen
 *   bijvoorbeeld door per file aparte newadresarrays te maken?
 *
 *
 *
 LEZEN

 - Bestaande Library (Main) pushen of vrijgeven
 - Nieuwe Main alloceren
 - file inlezen
 - lees SDNA
 - per LibBlock
  - lees LibBlock
  - als Library
   - nieuwe Main maken
   - ID's eraan hangen
  - else
   - lees bijhorende direkte data
   - link direkte data (intern en aan LibBlock)
 - lees FileGlobal
 - lees USER data, als aangegeven (~/.B.blend)
 - file vrijgeven
 - per Library met Scene (per Main)
  - file inlezen
  - lees SDNA
  - alle LibBlocks uit Scene opzoeken en ID's aan Main hagen
   - als extern LibBlock
    - zoek Main's af
     - is al ingelezen:
     - nog niet ingelezen
     - of nieuwe Main maken
  - per LibBlock
   - recursief dieper lezen
   - lees bijhorende direkte data
   - link direkte data (intern en aan LibBlock)
  - file vrijgeven
 - per Library met nog niet gelezen LibBlocks
  - file inlezen
  - lees SDNA
  - per LibBlock
   - recursief dieper lezen
   - lees bijhorende direkte data
   - link direkte data (intern en aan LibBlock)
  - file vrijgeven
 - alle Main's samenvoegen
 - alle LibBlocks linken en indirekte pointers naar libblocks
 - FileGlobal goedzetten en pointers naar Global kopieeren

 *
 *
 *
 *)

unit readfile;

interface

function convertstringcode(str: pchar): integer;
function gethome: pchar;

implementation

uses
cfuncs,
blender, blendef, blenglob, fileops;

//type
//OldNew = record
//old: pinteger; 
//new: pinteger; 
//nr: integer; 
//end;
//OldNew = OldNew; 
//
//{compflags: pchar; }{<= !!!5 external variable}(* genfile.c *)
//{cur_sdna: SDNA; }{<= !!!5 external variable}
//
//procedure read_libraries; 
//
//var
//datablocks: pOldNew = 0; 
//libblocks: pOldNew = 0; 
//datacount: integer = 0; 
//maxdatacount: integer = 1024; 
//libcount: integer = 0; 
//maxlibcount: integer = 1024; 
//pointerlen: integer; 
//{switch_endian: integer; }{<= !!!5 external variable}(* genfile.c *)
//
//disable_newlibadr: pinteger;

(* ******************************************* *)

//{bprogname: array [0..] of char; }{<= !!!5 external variable}(* usiblender.c *)



function gethome: pchar;
var
ret: pchar;

begin
  {$ifdef __BeOS}
  exit('/boot/home/'); (* BeOS 4.5: doubleclick at icon doesnt give home env *)

    {$elif !defined(WINDOWS)}
    exit(getenv('HOME'));
    {$else}
    (* Windows *)
      ret:= getenv('HOME');
  if ret<>nil  then
  begin
    if fop_exists(ret)<>0    then
    exit(ret);
     if (G.f and G_DEBUG)<>0 then
    printf('Unable to find home at: %s\n',[ret]);
  end;
  ret:= getenv('WINDOWS');
  if ret<>nil  then
  begin
    if fop_exists(ret)<>0    then
      exit(ret);
    if (G.f and G_DEBUG)<>0 then
    printf('Unable to find home at: %s\n',[ret]);
  end;
  ret:= getenv('WINDIR');
  if ret<>nil  then
  begin
    if fop_exists(ret)<>0    then
    exit(ret);
     if (G.f and G_DEBUG)<>0 then
    printf('Unable to find home at: %s\n',[ret]);
  end;
  exit('C:\Temp');
    {$endif}
end;

(* ************ DIV ****************** *)


//function testextensie(str: pchar;  ext: pchar): integer; 
//var
//a: smallint; 
//b: smallint; 
//begin
//  
//  a:= lstrlen(str); 
//  b:= lstrlen(ext); 
//  if a=0)or(b=0)or(b>=a
//  then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  while b>0
//  do
//  begin 
//    dec(a); 
//    dec(b); 
//    if str[a]<>ext[b]
//    then
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


function convertstringcode(str: pchar): integer;
var
len: integer = 0;
ret: integer = 0;
slash: pchar;
temp: array [0..Pred(FILE_MAXDIR+FILE_MAXFILE)] of char;
begin
  if (str[0]='/')and(str[1]='/') then
  begin
    strcpy(temp,G.sce);

    (* Find the last slash *)
    if (strrchr(temp,'/')>strrchr(temp,#92)) then
    slash:= strrchr(temp,'/')
    else
      strrchr(temp,#92);
    if slash<>#0  then
    begin
      strcpy(slash+1,str+2);
      strcpy(str,temp);
    end
    else
    begin
      (* pad zonder slash! *)
      strcpy(temp,str+2);
      strcpy(str,temp);
    end;
    ret:= 1;
  end;
  len:= lstrlen(str);
  if (len<>0)and(str[len-1]='#') then
  begin
    sprintf(str+len-1,'%04d',[CFRA]);
  end;
  exit(ret);
end;


//procedure makestringcode(str: pchar); 
//var
//slash: pchar; 
//len: char; 
//temp: array [0..Pred(FILE_MAXDIR)] of char; 
//begin
//  
//  
//  strcpy(temp,G.sce); 
//  slash:= (strrchr(temp,'/')>strrchr(temp,#92)) {was ?}if  then strrchr(temp,'/') {was :}else strrchr(temp,#92); 
//  (* Find the last slash *)
//  if slash<>0{nil} {<= !!!9} 
//  then
//  begin 
//    *(slash+1):=0; 
//    len:= lstrlen(temp); 
//    if len<>0{nil} {<= !!!9} 
//    then
//    begin 
//      if strncmp(str,temp,len)=0
//      then
//      begin 
//        temp[0]:= '/'; 
//        temp[1]:= '/'; 
//        strcpy(temp+2,str+len); 
//        strcpy(str,temp); 
//      end;
//    end;
//  end;
//end;
//
//
//procedure splitdirstring(di: pchar;  fi: pchar); 
//var
//slash: pchar; (* Find the last slash *)
//begin
//  slash:= (strrchr(di,'/')>strrchr(di,#92)) {was ?}if  then strrchr(di,'/') {was :}else strrchr(di,#92); 
//  if slash<>0{nil} {<= !!!9} 
//  then
//  begin 
//    strcpy(fi,slash+1); 
//    *(slash+1):=0; 
//  end;
//end;
//
//
//
//procedure mallocNN(len: integer;  str: pchar); 
//begin
//  PRINT2(d,s,len,str); 
//  begin
//    result:= mallocN(len,str); 
//    exit;
//  end;
//end;
//(* #define mallocN(a, b) mallocNN(a, b) *)
//
//(* ************** AFHANDELING OUDE POINTERS ******************* *)
//
//(* met dynamische malloc
// * (0, 1) doorgeven herinitialiseert en geeft ongebruikte blokken vrij
// * (0, 0) doorgeven geeft alles vrij 
// *)
//
//procedure add_data_adr(old: pinteger;  new: pinteger); 
//var
//temp: pOldNew; 
//a: integer; 
//begin
//  
//  
//  if old=0
//  then
//  begin 
//    (* ongebruikte vrijgeven *)
//    temp:= datablocks; 
//    while{for} 
//    a:= 0; 
//    {to} a<datacount
//    {;}
//    inc(a); 
//    inc(temp); 
//    do
//    begin 
//      if temp.nr=0)and(temp.new
//      then
//      freeN(temp.new); 
//    end;
//    if new=0)and(datablocks
//    then
//    begin 
//      freeN(datablocks); 
//      datablocks:= 0; 
//      maxdatacount:= 1024; 
//    end;
//    datacount:= 0; 
//  end;
//  else
//  begin 
//    if datablocks=0
//    then
//    begin 
//      datablocks:= mallocN(maxdatacount*sizeof(OldNew),'OldNew'); 
//    end;
//    else
//    if datacount=maxdatacount
//    then
//    begin 
//      maxdatacount:= maxdatacount * (2); 
//      temp:= mallocN(maxdatacount*sizeof(OldNew),'OldNew'); 
//      memcpy(temp,datablocks,maxdatacount*sizeof(OldNew) div 2); 
//      freeN(datablocks); 
//      datablocks:= temp; 
//    end;
//    temp:= datablocks+datacount; 
//    temp.old:= old; 
//    temp.new:= new; 
//    temp.nr:= 0; 
//    inc(datacount); 
//  end;
//end;
//
//(* met dynamische malloc
// * (0, 0) doorgeven geeft alles vrij
// * Zet aantal users al op 1!!!
// *)
//
//procedure add_lib_adr(old: pinteger;  new: pinteger); 
//var
//temp: pOldNew;
// 
//a: integer; (* onderscheid tussen twee gevallen:
//  * 
//  * 1. lib obj's in locale scene, ob->parent  (old is uit library)
//  * 2. lib obj's in locale scene: base->object   (old is uit locale scene)
//  * 
//  *)
//begin
//  temp:=0; 
//  if disable_newlibadr<>0{nil} {<= !!!9} 
//  then
//  begin 
//    disable_newlibadr:= new; 
//  end;
//  if old=0
//  then
//  begin 
//    (* alles vrijgeven *)
//    if libblocks<>0{nil} {<= !!!9} 
//    then
//    freeN(libblocks); 
//    libblocks:= 0; 
//    maxlibcount:= 1024; 
//    libcount:= 0; 
//  end;
//  else
//  begin 
//    if libblocks=0
//    then
//    begin 
//      libblocks:= mallocN(maxlibcount*sizeof(OldNew),'OldNew'); 
//    end;
//    else
//    if libcount=maxlibcount
//    then
//    begin 
//      maxlibcount:= maxlibcount * (2); 
//      temp:= mallocN(maxlibcount*sizeof(OldNew),'OldNew'); 
//      memcpy(temp,libblocks,maxlibcount*sizeof(OldNew) div 2); 
//      freeN(libblocks); 
//      libblocks:= temp; 
//    end;
//    temp:= libblocks+libcount; 
//    temp.old:= old; 
//    temp.new:= new; 
//    temp.nr:= 1; 
//    inc(libcount); 
//  end;
//end;
//
//(* alleen direkte datablokken *)
//type
//OldNew = record
//end;
//var {was static}
//lastone: integer;
// 
//
//procedure newadr(adr: pinteger); 
//begin
//  lastone:=0; 
//  if adr<>0{nil} {<= !!!9} 
//  then
//  begin 
//    (* op goed geluk: eerst het volgende blok doen *)
//    if lastone<datacount-1
//    then
//    begin 
//      inc(lastone); 
//      onew:= datablocks+lastone; 
//      if onew.old=adr
//      then
//      begin 
//        inc(onew.nr); 
//        begin
//          result:= onew.new; 
//          exit;
//        end;
//      end;
//    end;
//    lastone:= 0; 
//    onew:= datablocks; 
//    while lastone<datacount
//    do
//    begin 
//      if onew.old=adr
//      then
//      begin 
//        inc(onew.nr); 
//        begin
//          result:= onew.new; 
//          exit;
//        end;
//      end;
//      inc(onew); 
//      inc(lastone); 
//    end;
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//(* alleen Lib datablokken *)
//type
//OldNew = record
//end;
//var {was static}
//lastone: integer;
// 
//
//procedure newlibadr(lib: pinteger;  adr: pinteger); 
//var
//id: pID; (* ook testen op lib: lib==id->lib) *)
//begin
//  lastone:=0; 
//  
//  if adr<>0{nil} {<= !!!9} 
//  then
//  begin 
//    (* op goed geluk: eerst het volgende blok doen *)
//    (* als je dit weglaat gaat newcarou.blend mis *)
//    if lastone<libcount-1
//    then
//    begin 
//      inc(lastone); 
//      onew:= libblocks+lastone; 
//      if onew.old=adr
//      then
//      begin 
//        id:= onew.new; 
//        if id.lib=0)and(lib
//        then
//        ; 
//        else
//        begin 
//          inc(onew.nr); 
//          begin
//            result:= onew.new; 
//            exit;
//          end;
//        end;
//      end;
//    end;
//    lastone:= 0; 
//    onew:= libblocks; 
//    while lastone<libcount
//    do
//    begin 
//      if onew.old=adr
//      then
//      begin 
//        id:= onew.new; 
//        if id)and(id.lib=0)and(lib
//        then
//        ; 
//        else
//        begin 
//          inc(onew.nr); 
//          begin
//            result:= onew.new; 
//            exit;
//          end;
//        end;
//      end;
//      inc(onew); 
//      inc(lastone); 
//    end;
//  end;
//  
//  
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//(* hoogt usernummer op *)
//
//procedure newlibadr_us(lib: pinteger;  adr: pinteger); 
//var
//id: pID; 
//begin
//  
//  id:= newlibadr(lib,adr); 
//  if id<>0{nil} {<= !!!9} 
//  then
//  begin 
//    inc(id.us); 
//  end;
//  begin
//    result:= id; 
//    exit;
//  end;
//end;
//
//(* alleen Lib datablokken *)
//type
//OldNew = record
//end;
//var {was static}
//lastone: integer;
// 
//
//procedure newlibadr_us_type(type: smallint;  adr: pinteger); 
//var
//id: pID; 
//begin
//  lastone:=0; 
//  
//  
//  if adr<>0{nil} {<= !!!9} 
//  then
//  begin 
//    (* op goed geluk: eerst het volgende blok doen *)
//    (* als je dit weglaat gaat newcarou.blend mis *)
//    
//    if lastone<libcount-1
//    then
//    begin 
//      inc(lastone); 
//      onew:= libblocks+lastone; 
//      if onew.old=adr
//      then
//      begin 
//        id:= onew.new; 
//        if GS(id.name)=type
//        then
//        begin 
//          inc(id.us); 
//          inc(onew.nr); 
//          begin
//            result:= id; 
//            exit;
//          end;
//        end;
//      end;
//    end;
//    lastone:= 0; 
//    onew:= libblocks; 
//    while lastone<libcount
//    do
//    begin 
//      if onew.old=adr
//      then
//      begin 
//        id:= onew.new; 
//        if GS(id.name)=type
//        then
//        begin 
//          inc(id.us); 
//          inc(onew.nr); 
//          begin
//            result:= id; 
//            exit;
//          end;
//        end;
//      end;
//      inc(onew); 
//      inc(lastone); 
//    end;
//  end;
//  
//  
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//type
//OldNew = record
//end;
//
//procedure change_libadr(old: pinteger;  new: pinteger); 
//var
//id: pID; 
//lastone: integer;
// 
//begin
//  
//  
//  lastone:=0; 
//  onew:= libblocks; 
//  while lastone<libcount
//  do
//  begin 
//    id:= onew.new; 
//    if id)and(id.lib
//    then
//    begin 
//      if onew.new=old
//      then
//      begin 
//        onew.new:= new; 
//        (* geen return; blijkbaar kunnen er meer zijn? *)
//      end;
//    end;
//    inc(onew); 
//    inc(lastone); 
//  end;
//end;
//
//
//
//(* ********** END OUDE POINTERS ****************** *)
//(* ********** READ FILE ****************** *)
//
//
//procedure switch_endian_structs(bhead: pBHead); 
//var
//oldlen: integer; 
//blocks: integer; 
//spo: psmallint; 
//data: pchar; 
//begin
//  
//  
//  
//  
//  blocks:= bhead.nr; 
//  data:= {!!!a type cast? =>} {pchar(}(bhead+1); 
//  spo:= old_sdna.structs[bhead.SDNAnr]; 
//  oldlen:= old_sdna.typelens[spo[0]]; 
//  while blocks{--} dec(blocks); 
//  do
//  begin 
//    switch_endian_struct(bhead.SDNAnr,data); 
//    data:= data + (oldlen); 
//  end;
//end;
//
//
//procedure read_struct(bh: pBHead); 
//var
//temp: pinteger;
// 
//begin
//  temp:=0; 
//  if bh.len<>0{nil} {<= !!!9} 
//  then
//  begin 
//    if bh.SDNAnr)and(switch_endian
//    then
//    switch_endian_structs(bh); 
//    
//    if compflags[bh.SDNAnr]<>0{nil} {<= !!!9} 
//    then
//    begin 
//      (* flag==0: bestaat niet meer *)
//      (* dit hele ingewikkelde spul is de naam van de struct, let op: old_sdna! *)
//      temp:= mallocN(bh.len,old_sdna.types[{*}old_sdna.structs[bh.SDNAnr]^]); 
//      memcpy(temp,(bh+1),bh.len); 
//      (* temp= mallocN(bh->len, "read_struct"); *)
//      if compflags[bh.SDNAnr]=2
//      then
//      reconstruct(bh.SDNAnr,bh.nr,@temp); 
//    end;
//  end;
//  add_data_adr(bh.old,temp); 
//end;
//
//
//procedure newstructname(bh: pBHead); 
//var
//sp: psmallint; 
//name: pchar; 
//begin(* rijtje hardcoded uitzonderingen *)
//  
//  
//  sp:= old_sdna.structs[bh.SDNAnr]; 
//  if lstrcmp('Screen',old_sdna.types[sp[0]])=0
//  then
//  begin 
//    bh.SDNAnr:= findstruct_nr(@old_sdna,'bScreen'); 
//  end;
//end;
//
//
//
//procedure read_libstruct(bh: pBHead); 
//var
//temp: pinteger;
// 
//begin
//  temp:=0; 
//  if bh.len<>0{nil} {<= !!!9} 
//  then
//  begin 
//    if switch_endian<>0{nil} {<= !!!9} 
//    then
//    switch_endian_structs(bh); 
//    if compflags[bh.SDNAnr]<>0{nil} {<= !!!9} 
//    then
//    begin 
//      (* flag==0: bestaat niet meer *)
//      
//      temp:= mallocN(bh.len,'read_libstruct'); 
//      memcpy(temp,(bh+1),bh.len); 
//      
//      if compflags[bh.SDNAnr]=2
//      then
//      reconstruct(bh.SDNAnr,bh.nr,@temp); 
//    end;
//  end;
//  add_lib_adr(bh.old,temp); 
//  
//  
//  begin
//    result:= temp; 
//    exit;
//  end;
//end;
//
//void{!!!e unknown token}
//bh: pBHead; void{!!!e unknown token}{*}data^; 
//begin 
//  (* dubbele pointer ivm reconstruct *)
//  
//  if bh.len<>0{nil} {<= !!!9} 
//  then
//  begin 
//    
//    if bh.SDNAnr)and(switch_endian
//    then
//    switch_endian_structs(bh); 
//    if compflags[bh.SDNAnr]<>0{nil} {<= !!!9} 
//    then
//    begin 
//      (* flag==0: bestaat niet meer *)
//      {*}data^:=mallocN(bh.len,'read_struct_expl'); 
//      memcpy({*}data^,(bh+1),bh.len); 
//      if compflags[bh.SDNAnr]=2
//      then
//      reconstruct(bh.SDNAnr,bh.nr,data); 
//    end;
//  end;
//end;
//
//(* alleen direkte data *)
//
//procedure link_list(lb: pListBase); 
//var
//ln: pLink; 
//prev: pLink; 
//begin
//  
//  
//  if lb.first=0
//  then
//  exit;
//  
//  lb.first:= newadr(lb.first); 
//  ln:= lb.first; 
//  prev:= 0; 
//  while ln
//  do
//  begin 
//    ln.next:= newadr(ln.next); 
//    ln.prev:= prev; 
//    prev:= ln; 
//    ln:= ln.next; 
//  end;
//  lb.last:= prev; 
//end;
//
//
//procedure test_pointer_array{!!!3 unknown typedef}; 
//var
//lpoin: pDWORDLONG; 
//lmat: pDWORDLONG; 
//{$else}
//lpoin: pinteger; 
//lmat: pinteger; 
//{$endif}
//len: integer; 
//ipoin: pinteger; 
//imat: pinteger; 
//(* mat is still 'old_sdna' *)
//begin
//  {$ifdef WIN32}
//  
//  
//  
//  
//  if {*}mat^
//  then
//  begin 
//    len:= alloc_len(*mat) div old_sdna.pointerlen; 
//    if old_sdna.pointerlen=8)and(cur_sdna.pointerlen=4
//    then
//    begin 
//      ipoin:= imat:=mallocN(len*4,'newmatar'); 
//      lpoin:= {*}mat^; 
//      while len{--} dec(len); >0
//      do
//      begin 
//        if switch_endian<>0{nil} {<= !!!9} 
//        then
//        SWITCH_LONGINT({*}lpoin^); 
//        {*}ipoin^:=(*lpoin) shr 3; 
//        inc(ipoin); 
//        inc(lpoin); 
//      end;
//      freeN({*}mat^); 
//      {*}mat^:=imat; 
//    end;
//    if old_sdna.pointerlen=4)and(cur_sdna.pointerlen=8
//    then
//    begin 
//      lpoin:= lmat:=mallocN(len*8,'newmatar'); 
//      ipoin:= {*}mat^; 
//      while len{--} dec(len); >0
//      do
//      begin 
//        {*}lpoin^:=*ipoin; 
//        inc(ipoin); 
//        inc(lpoin); 
//      end;
//      freeN({*}mat^); 
//      {*}mat^:=lmat; 
//    end;
//  end;
//end;
//
//(* ************ READ scriptlinks *************** *)
//
//
//procedure lib_link_scriptlink(id: pID;  slink: pScriptLink); 
//var
//i: integer; 
//begin
//  for{while} i:=0 to Pred(slink.totscript) { i++}
//  do
//  begin 
//    slink.scripts[i]:= newlibadr(id.lib,slink.scripts[i]); 
//  end;
//end;
//
//
//procedure direct_link_scriptlink(slink: pScriptLink); 
//var
//a: integer; 
//begin
//  slink.scripts:= newadr(slink.scripts); 
//  slink.flag:= newadr(slink.flag); 
//  if switch_endian<>0{nil} {<= !!!9} 
//  then
//  begin 
//    
//    for{while} a:=0 to Pred(slink.totscript) { a++}
//    do
//    begin 
//      SWITCH_SHORT(slink.flag[a]); 
//    end;
//  end;
//end;
//
//(* ************ READ voorbeeld ***************** *)
//
//
//procedure lib_link_vb(main: pMain); 
//var
//me: pMesh; 
//begin
//  
//  me:= main.mesh.first; 
//  while me
//  do
//  begin 
//    if me.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      me.id.flag:= me.id.flag - (LIB_NEEDLINK); 
//    end;
//    me:= me.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//me: pMesh; 
//begin 
//  
//end;
//
//(* ************ READ IKA ***************** *)
//
//
//procedure lib_link_ika(main: pMain); 
//var
//ika: pIka; 
//a: integer; 
//def: pDeform; 
//begin
//  
//  
//  
//  ika:= main.ika.first; 
//  while ika
//  do
//  begin 
//    if ika.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      ika.parent:= newlibadr(ika.id.lib,ika.parent); 
//      a:= ika.totdef; 
//      def:= ika.def; 
//      while a{--} dec(a); 
//      do
//      begin 
//        def.ob:= newlibadr(ika.id.lib,def.ob); 
//        inc(def); 
//      end;
//      ika.id.flag:= ika.id.flag - (LIB_NEEDLINK); 
//    end;
//    ika:= ika.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//ika: pIka; 
//begin 
//  link_list(@ika.limbbase); 
//  ika.def:= newadr(ika.def); 
//  
//  (* afvangen fout uit V.138 en ouder *)
//  if ika.def=0
//  then
//  ika.totdef:= 0; 
//end;
//
//
//(* ************ READ CAMERA ***************** *)
//
//
//procedure lib_link_camera(main: pMain); 
//var
//ca: pCamera; 
//a: integer; 
//begin
//  
//  
//  ca:= main.camera.first; 
//  while ca
//  do
//  begin 
//    if ca.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      ca.ipo:= newlibadr_us(ca.id.lib,ca.ipo); 
//      lib_link_scriptlink( and ca.id, and ca.scriptlink); 
//      
//      ca.id.flag:= ca.id.flag - (LIB_NEEDLINK); 
//    end;
//    ca:= ca.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//ca: pCamera; 
//begin 
//  direct_link_scriptlink( and ca.scriptlink); 
//end;
//
//(* ************ READ Life ***************** *)
//
//
//procedure lib_link_life(main: pMain); 
//var
//lf: pLife; 
//begin
//  
//  lf:= main.life.first; 
//  while lf
//  do
//  begin 
//    if lf.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      lf.sector:= newlibadr(lf.id.lib,lf.sector); 
//      lf.dynamesh:= newlibadr_us(lf.id.lib,lf.dynamesh); 
//      lf.texmesh:= newlibadr_us(lf.id.lib,lf.texmesh); 
//      lf.id.flag:= lf.id.flag - (LIB_NEEDLINK); 
//      
//    end;
//    lf:= lf.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//lf: pLife; 
//begin 
//  sn: pSensor; 
//  a: integer; 
//  lf.sensors:= newadr(lf.sensors); 
//  a:= lf.totsens; 
//  sn:= lf.sensors; 
//  while a{--} dec(a); 
//  do
//  begin 
//    sn.events:= newadr(sn.events); 
//    sn.actions:= newadr(sn.actions); 
//    inc(sn); 
//  end;
//end;
//
//(* ************ READ Sector ***************** *)
//
//
//procedure lib_link_sector(main: pMain); 
//var
//se: pSector; 
//po: pPortal; 
//a: integer; 
//begin
//  
//  
//  
//  se:= main.sector.first; 
//  while se
//  do
//  begin 
//    if se.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      se.ob:= newlibadr(se.id.lib,se.ob); 
//      se.dynamesh:= newlibadr_us(se.id.lib,se.dynamesh); 
//      se.texmesh:= newlibadr_us(se.id.lib,se.texmesh); 
//      po:= se.portals; 
//      a:= se.totport; 
//      while a{--} dec(a); 
//      do
//      begin 
//        po.sector:= newlibadr(se.id.lib,po.sector); 
//        inc(po); 
//      end;
//      se.id.flag:= se.id.flag - (LIB_NEEDLINK); 
//    end;
//    se:= se.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//se: pSector; 
//begin 
//  
//  (* portals *)
//  se.portals:= newadr(se.portals); 
//end;
//
//(* ************ READ LATTICE ***************** *)
//
//
//procedure lib_link_latt(main: pMain); 
//var
//lt: pLattice; 
//begin
//  
//  lt:= main.latt.first; 
//  while lt
//  do
//  begin 
//    if lt.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      lt.ipo:= newlibadr_us(lt.id.lib,lt.ipo); 
//      lt.key:= newlibadr_us(lt.id.lib,lt.key); 
//      lt.id.flag:= lt.id.flag - (LIB_NEEDLINK); 
//    end;
//    lt:= lt.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//lt: pLattice; 
//begin 
//  lt.def:= newadr(lt.def); 
//  
//end;
//
//(* ************ READ LAMP ***************** *)
//
//
//procedure lib_link_lamp(main: pMain); 
//var
//la: pLamp; 
//mtex: pMTex; 
//a: integer; 
//begin
//  
//  
//  
//  la:= main.lamp.first; 
//  while la
//  do
//  begin 
//    if la.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      
//      for{while} a:=0 to Pred(8) { a++}
//      do
//      begin 
//        mtex:= la.mtex[a]; 
//        if mtex<>0{nil} {<= !!!9} 
//        then
//        begin 
//          mtex.tex:= newlibadr_us(la.id.lib,mtex.tex); 
//          mtex.object:= newlibadr(la.id.lib,mtex.object); 
//        end;
//      end;
//      la.ipo:= newlibadr_us(la.id.lib,la.ipo); 
//      lib_link_scriptlink(@la.id,@la.scriptlink); 
//      la.id.flag:= la.id.flag - (LIB_NEEDLINK); 
//    end;
//    la:= la.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//la: pLamp; 
//begin 
//  a: integer; 
//  direct_link_scriptlink(@la.scriptlink); 
//  for{while} a:=0 to Pred(8) { a++}
//  do
//  begin 
//    la.mtex[a]:= newadr(la.mtex[a]); 
//  end;
//end;
//
//(* ************ READ keys ***************** *)
//
//
//procedure lib_link_key(main: pMain); 
//var
//key: pKey; 
//begin
//  
//  key:= main.key.first; 
//  while key
//  do
//  begin 
//    if key.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      key.ipo:= newlibadr_us(key.id.lib,key.ipo); 
//      key.from:= newlibadr(key.id.lib,key.from); 
//      key.id.flag:= key.id.flag - (LIB_NEEDLINK); 
//    end;
//    key:= key.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//key: pKey; 
//begin 
//  kb: pKeyBlock; 
//  link_list(@(key.block)); 
//  key.refkey:= newadr(key.refkey); 
//  kb:= key.block.first; 
//  while kb
//  do
//  begin 
//    kb.data:= newadr(kb.data); 
//    if switch_endian<>0{nil} {<= !!!9} 
//    then
//    switch_endian_keyblock(key,kb); 
//    kb:= kb.next; 
//  end;
//end;
//
//(* ************ READ mball ***************** *)
//
//
//procedure lib_link_mball(main: pMain); 
//var
//mb: pMetaBall; 
//a: integer; 
//begin
//  
//  
//  mb:= main.mball.first; 
//  while mb
//  do
//  begin 
//    if mb.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      for{while} a:=0 to Pred(mb.totcol) { a++}
//      do
//      mb.mat[a]:= newlibadr_us(mb.id.lib,mb.mat[a]); 
//      mb.ipo:= newlibadr_us(mb.id.lib,mb.ipo); 
//      mb.id.flag:= mb.id.flag - (LIB_NEEDLINK); 
//      
//      
//    end;
//    mb:= mb.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//mb: pMetaBall; 
//begin 
//  
//  mb.mat:= newadr(mb.mat); 
//  test_pointer_array((void** )@mb.mat); 
//  link_list(@(mb.elems)); 
//  
//  mb.disp.first:= mb.disp.last:=0; 
//  mb.bb:= 0; 
//  
//end;
//
//(* ************ READ WORLD ***************** *)
//
//
//procedure lib_link_world(main: pMain); 
//var
//wrld: pWorld; 
//mtex: pMTex; 
//a: integer; 
//begin
//  
//  
//  
//  wrld:= main.world.first; 
//  while wrld
//  do
//  begin 
//    if wrld.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      wrld.ipo:= newlibadr_us(wrld.id.lib,wrld.ipo); 
//      for{while} a:=0 to Pred(8) { a++}
//      do
//      begin 
//        mtex:= wrld.mtex[a]; 
//        if mtex<>0{nil} {<= !!!9} 
//        then
//        begin 
//          mtex.tex:= newlibadr_us(wrld.id.lib,mtex.tex); 
//          mtex.object:= newlibadr(wrld.id.lib,mtex.object); 
//        end;
//      end;
//      lib_link_scriptlink(@wrld.id,@wrld.scriptlink); 
//      
//      wrld.id.flag:= wrld.id.flag - (LIB_NEEDLINK); 
//    end;
//    wrld:= wrld.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//wrld: pWorld; 
//begin 
//  a: integer; 
//  direct_link_scriptlink(@wrld.scriptlink); 
//  for{while} a:=0 to Pred(8) { a++}
//  do
//  begin 
//    wrld.mtex[a]:= newadr(wrld.mtex[a]); 
//  end;
//end;
//
//
//(* ************ READ IPO ***************** *)
//
//
//procedure lib_link_ipo(main: pMain); 
//var
//ipo: pIpo; 
//begin
//  
//  ipo:= main.ipo.first; 
//  while ipo
//  do
//  begin 
//    if ipo.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      ipo.id.flag:= ipo.id.flag - (LIB_NEEDLINK); 
//    end;
//    ipo:= ipo.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//ipo: pIpo; 
//begin 
//  icu: pIpoCurve; 
//  link_list(@(ipo.curve)); 
//  icu:= ipo.curve.first; 
//  while icu
//  do
//  begin 
//    icu.bezt:= newadr(icu.bezt); 
//    icu.bp:= newadr(icu.bp); 
//    icu:= icu.next; 
//  end;
//end;
//
//(* ************ READ VFONT ***************** *)
//
//void{!!!e unknown token}
//vf: pVFont; 
//begin 
//  a: integer; 
//  vf.data:= callocN(sizeof(VFontData),'vfontdata'); 
//  vf.flag:= 1; 
//  (* NEEDLOAD *)
//end;
//
//(* ************ READ TEXT ****************** *)
//
//
//procedure lib_link_text(main: pMain); 
//var
//text: pText; 
//tmp: pTextLine; 
//begin
//  
//  
//  text:= main.text.first; 
//  while text
//  do
//  begin 
//    if text.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      text.id.flag:= text.id.flag - (LIB_NEEDLINK); 
//    end;
//    text:= text.id.next; 
//  end;
//end;
//
//
//procedure print_text(text: pText); 
//var
//line: pTextLine; 
//begin
//  
//  printf('-- Text: %s (%d)--\n',text.id.name,text.nlines);
//  line:= text.lines.first; 
//  while line
//  do
//  begin 
//    printf('%x %x %x %d: <%s>\n',line.prev,line,line.next,line.len,line.line);
//    line:= line.next; 
//  end;
//  printf('Main cursor: %x %d\n',text.curl,text.curc);
//  printf('Selection cursor: %x %d\n',text.sell,text.selc);
//end;
//
//void{!!!e unknown token}
//text: pText; 
//begin 
//  ln: pTextLine; 
//  text.name:= newadr(text.name); 
//  text.undo_pos:= -1; 
//  text.undo_len:= TXT_INIT_UNDO; 
//  text.undo_buf:= mallocN(text.undo_len,'undo buf'); 
//  text.compiled:= 0{nil}; 
//  if text.flags and TXT_ISEXT
//  then
//  begin 
//    reopen_text(text); 
//  end;
//  else
//  begin 
//    
//    if text.lines.first=0
//    then
//    exit;
//    link_list(@text.lines); 
//    text.curl:= newadr(text.curl); 
//    text.sell:= newadr(text.sell); 
//    ln:= text.lines.first; 
//    while ln
//    do
//    begin 
//      ln.line:= newadr(ln.line); 
//      if ln.len<>lstrlen(ln.line)
//      then
//      begin 
//        printf('Error loading text, line lengths differ\n');
//        ln.len:= lstrlen(ln.line); 
//      end;
//      ln:= ln.next; 
//    end;
//    text.flags:= text.flags or (TXT_ISTMP); 
//  end;
//  text.id.us:= 1; 
//  (*  print_text (text); *)
//end;
//
//(* ************ READ IMAGE ***************** *)
//
//
//procedure lib_link_image(main: pMain); 
//var
//ima: pImage; 
//begin
//  
//  ima:= main.image.first; 
//  while ima
//  do
//  begin 
//    if ima.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      ima.id.flag:= ima.id.flag - (LIB_NEEDLINK); 
//    end;
//    ima:= ima.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//ima: pImage; 
//begin 
//  a: integer; 
//  ima.ibuf:= 0; 
//  ima.anim:= 0; 
//  for{while} a:=0 to Pred(MAXMIPMAP) { a++}
//  do
//  ima.mipmap[a]:= 0; 
//  ima.repbind:= 0; 
//  ima.bindcode:= 0; 
//  ima.ok:= 1; 
//end;
//
//
//(* ************ READ CURVE ***************** *)
//
//
//procedure lib_link_curve(main: pMain); 
//var
//cu: pCurve; 
//a: integer; 
//begin
//  
//  
//  cu:= main.curve.first; 
//  while cu
//  do
//  begin 
//    if cu.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      for{while} a:=0 to Pred(cu.totcol) { a++}
//      do
//      cu.mat[a]:= newlibadr_us(cu.id.lib,cu.mat[a]); 
//      cu.bevobj:= newlibadr(cu.id.lib,cu.bevobj); 
//      cu.textoncurve:= newlibadr(cu.id.lib,cu.textoncurve); 
//      cu.vfont:= newlibadr_us(cu.id.lib,cu.vfont); 
//      cu.ipo:= newlibadr_us(cu.id.lib,cu.ipo); 
//      cu.key:= newlibadr_us(cu.id.lib,cu.key); 
//      cu.id.flag:= cu.id.flag - (LIB_NEEDLINK); 
//      
//      
//    end;
//    cu:= cu.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//cu: pCurve; 
//begin 
//  nu: pNurb; 
//  cu.mat:= newadr(cu.mat); 
//  test_pointer_array((void** ) and cu.mat); 
//  cu.str:= newadr(cu.str); 
//  if cu.vfont=0
//  then
//  link_list(@(cu.nurb)); 
//  else
//  begin 
//    cu.nurb.first:= cu.nurb.last:=0; 
//  end;
//  cu.bev.first:= cu.bev.last:=0; 
//  cu.disp.first:= cu.disp.last:=0; 
//  cu.path:= 0; 
//  nu:= cu.nurb.first; 
//  while nu
//  do
//  begin 
//    nu.bezt:= newadr(nu.bezt); 
//    nu.bp:= newadr(nu.bp); 
//    nu.knotsu:= newadr(nu.knotsu); 
//    nu.knotsv:= newadr(nu.knotsv); 
//    if switch_endian<>0{nil} {<= !!!9} 
//    then
//    begin 
//      switch_endian_knots(nu); 
//    end;
//    nu:= nu.next; 
//  end;
//  cu.bb:= 0; 
//end;
//
//(* ************ READ TEX ***************** *)
//
//
//procedure lib_link_texture(main: pMain); 
//var
//tex: pTex; 
//begin
//  
//  tex:= main.tex.first; 
//  while tex
//  do
//  begin 
//    if tex.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      tex.ima:= newlibadr_us(tex.id.lib,tex.ima); 
//      tex.ipo:= newlibadr_us(tex.id.lib,tex.ipo); 
//      if tex.env<>0{nil} {<= !!!9} 
//      then
//      tex.env.object:= newlibadr(tex.id.lib,tex.env.object); 
//      tex.id.flag:= tex.id.flag - (LIB_NEEDLINK); 
//    end;
//    tex:= tex.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//tex: pTex; 
//begin 
//  tex.plugin:= newadr(tex.plugin); 
//  if tex.plugin<>0{nil} {<= !!!9} 
//  then
//  begin 
//    tex.plugin.handle:= 0; 
//    open_plugin_tex(tex.plugin); 
//  end;
//  tex.coba:= newadr(tex.coba); 
//  tex.env:= newadr(tex.env); 
//  if tex.env<>0{nil} {<= !!!9} 
//  then
//  begin 
//    tex.env.ima:= 0; 
//    bzero(tex.env.cube,6*sizeof(void* )); 
//    tex.env.ok:= 0; 
//  end;
//end;
//
//
//
//(* ************ READ MATERIAL ***************** *)
//
//
//procedure lib_link_material(main: pMain); 
//var
//ma: pMaterial; 
//mtex: pMTex; 
//a: integer; 
//begin
//  
//  
//  
//  ma:= main.mat.first; 
//  while ma
//  do
//  begin 
//    if ma.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      ma.ipo:= newlibadr_us(ma.id.lib,ma.ipo); 
//      for{while} a:=0 to Pred(8) { a++}
//      do
//      begin 
//        mtex:= ma.mtex[a]; 
//        if mtex<>0{nil} {<= !!!9} 
//        then
//        begin 
//          mtex.tex:= newlibadr_us(ma.id.lib,mtex.tex); 
//          mtex.object:= newlibadr(ma.id.lib,mtex.object); 
//        end;
//      end;
//      lib_link_scriptlink(@ma.id,@ma.scriptlink); 
//      
//      ma.id.flag:= ma.id.flag - (LIB_NEEDLINK); 
//    end;
//    ma:= ma.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//ma: pMaterial; 
//begin 
//  a: integer; 
//  direct_link_scriptlink(@ma.scriptlink); 
//  for{while} a:=0 to Pred(8) { a++}
//  do
//  begin 
//    ma.mtex[a]:= newadr(ma.mtex[a]); 
//  end;
//  ma.ren:= 0; 
//  (* mag niet blijven hangen, maarja *)
//end;
//
//(* ************ READ MESH ***************** *)
//
//
//procedure lib_link_mesh(main: pMain); 
//var
//me: pMesh; 
//tface: pTFace; 
//ima: pImage; 
//a: integer; 
//begin
//  
//  
//  
//  
//  me:= main.mesh.first; 
//  while me
//  do
//  begin 
//    if me.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      for{while} a:=0 to Pred(me.totcol) { a++}
//      do
//      me.mat[a]:= newlibadr_us(me.id.lib,me.mat[a]); 
//      me.ipo:= newlibadr_us(me.id.lib,me.ipo); 
//      me.key:= newlibadr_us(me.id.lib,me.key); 
//      me.texcomesh:= newlibadr_us(me.id.lib,me.texcomesh); 
//      if me.tface<>0{nil} {<= !!!9} 
//      then
//      begin 
//        a:= me.totface; 
//        tface:= me.tface; 
//        while a{--} dec(a); 
//        do
//        begin 
//          tface.tpage:= newlibadr(me.id.lib,tface.tpage); 
//          if tface.tpage<>0{nil} {<= !!!9} 
//          then
//          begin 
//            ima:= tface.tpage; 
//            if ima.id.us=0
//            then
//            ima.id.us:= 1; 
//          end;
//          (* was already swapped in genfile, should be! *)
//          if switch_endian<>0{nil} {<= !!!9} 
//          then
//          begin 
//            SWITCH_INT(tface.col[0]); 
//            SWITCH_INT(tface.col[1]); 
//            SWITCH_INT(tface.col[2]); 
//            SWITCH_INT(tface.col[3]); 
//          end;
//          inc(tface); 
//        end;
//      end;
//      me.id.flag:= me.id.flag - (LIB_NEEDLINK); 
//    end;
//    me:= me.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//mesh: pMesh; 
//begin 
//  mesh.mat:= newadr(mesh.mat); 
//  test_pointer_array((void** )@mesh.mat); 
//  mesh.mvert:= newadr(mesh.mvert); 
//  mesh.mface:= newadr(mesh.mface); 
//  mesh.tface:= newadr(mesh.tface); 
//  mesh.mcol:= newadr(mesh.mcol); 
//  mesh.msticky:= newadr(mesh.msticky); 
//  mesh.disp.first:= mesh.disp.last:=0; 
//  mesh.bb:= 0; 
//  mesh.oc:= 0; 
//  mesh.dface:= 0; 
//  mesh.orco:= 0; 
//end;
//
//(* ************ READ OBJECT ***************** *)
//
//
//procedure lib_link_object(main: pMain); 
//var
//ob: pObject; 
//nl: pNetLink; 
//poin: pinteger; 
//warn: integer;
// 
//a: integer; 
//begin
//  
//  
//  
//  warn:=0; 
//  
//  ob:= main.object.first; 
//  while ob
//  do
//  begin 
//    if ob.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      ob.parent:= newlibadr(ob.id.lib,ob.parent); 
//      ob.track:= newlibadr(ob.id.lib,ob.track); 
//      ob.ipo:= newlibadr_us(ob.id.lib,ob.ipo); 
//      poin:= ob.data; 
//      ob.data:= newlibadr_us(ob.id.lib,ob.data); 
//      if ob.data=0)and(poin<>0
//      then
//      begin 
//        ob.type:= OB_EMPTY; 
//        warn:= 1; 
//        if ob.id.lib<>0{nil} {<= !!!9} 
//        then
//        printf('Can'#39't find obdata of %s lib %s\n',ob.id.name+2,ob.id.lib.name);
//        else
//        printf('Object %s lost data. Lib:%x\n',ob.id.name+2,ob.id.lib);
//      end;
//      for{while} a:=0 to Pred(ob.totcol) { a++}
//      do
//      ob.mat[a]:= newlibadr_us(ob.id.lib,ob.mat[a]); 
//      ob.id.flag:= ob.id.flag - (LIB_NEEDLINK); 
//      nl:= ob.network.first; (* dit stond er eerst: weggehaald omdat de fie give_base_to... er niet meer is *)
//      (* if(ob->id.us) ob->id.flag -= LIB_NEEDLINK; *)
//      (* als us==0 wordt verderop nog een base gemaakt *)
//      while nl
//      do
//      begin 
//        nl.ob:= newlibadr(ob.id.lib,nl.ob); 
//        nl:= nl.next; 
//      end;
//      lib_link_scriptlink(@ob.id,@ob.scriptlink); 
//      
//    end;
//    ob:= ob.id.next; 
//  end;
//  if warn<>0{nil} {<= !!!9} 
//  then
//  error('WARNING IN CONSOLE'); 
//end;
//
//void{!!!e unknown token}
//ob: pObject; 
//begin 
//  paf: pPartEff; 
//  ob.disp.first:= ob.disp.last:=0; 
//  direct_link_scriptlink(@ob.scriptlink); 
//  
//  ob.mat:= newadr(ob.mat); 
//  test_pointer_array((void** )@ob.mat); 
//  
//  link_list(@ob.effect); 
//  paf:= ob.effect.first; 
//  while paf
//  do
//  begin 
//    if paf.type=EFF_PARTICLE
//    then
//    begin 
//      paf.keys:= 0; 
//    end;
//    if paf.type=EFF_WAVE
//    then
//    begin 
//    end;
//    paf:= paf.next; 
//  end;
//  link_list(@ob.network); 
//  
//  ob.bb:= 0; 
//  
//end;
//
//(* ************ READ SCENE ***************** *)
//
//
//procedure lib_link_scene(main: pMain); 
//var
//sce: pScene; 
//base: pBase; 
//next: pBase; 
//ed: pEditing; 
//seq: pSequence; 
//a: integer; 
//begin
//  
//  
//  
//  
//  
//  
//  sce:= main.scene.first; 
//  while sce
//  do
//  begin 
//    if sce.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      sce.id.us:= 1; 
//      sce.camera:= newlibadr(sce.id.lib,sce.camera); 
//      sce.world:= newlibadr_us(sce.id.lib,sce.world); 
//      sce.set:= newlibadr(sce.id.lib,sce.set); 
//      sce.ima:= newlibadr_us(sce.id.lib,sce.ima); 
//      base:= sce.base.first; 
//      while base
//      do
//      begin 
//        next:= base.next; 
//        base.object:= newlibadr_us_type(ID_OB,base.object); (* base->object= newlibadr_us(sce->id.lib, base->object); *)
//        
//        if base.object=0
//        then
//        begin 
//          printf('LIB ERROR: base removed\n');
//          remlink(@sce.base,base); 
//          if base=sce.basact
//          then
//          sce.basact:= 0; 
//          freeN(base); 
//        end;
//        base:= next; 
//      end;
//      ed:= sce.ed; 
//      if ed<>0{nil} {<= !!!9} 
//      then
//      begin 
//        WHILE_SEQ(ed.seqbasep); 
//        begin 
//          if seq.ipo<>0{nil} {<= !!!9} 
//          then
//          seq.ipo:= newlibadr_us(sce.id.lib,seq.ipo); 
//          if seq.scene<>0{nil} {<= !!!9} 
//          then
//          seq.scene:= newlibadr(sce.id.lib,seq.scene); 
//          seq.anim:= 0; 
//        end;
//        END_SEQ{!!!e unknown token}
//      end;
//      sce.id.flag:= sce.id.flag - (LIB_NEEDLINK); 
//    end;
//    lib_link_scriptlink(@sce.id,@sce.scriptlink); 
//    sce:= sce.id.next; 
//  end;
//end;
//
//
//procedure link_recurs_seq(lb: pListBase); 
//var
//seq: pSequence; 
//begin
//  link_list(lb); 
//  seq:= lb.first; 
//  while seq
//  do
//  begin 
//    if seq.seqbase.first<>0{nil} {<= !!!9} 
//    then
//    link_recurs_seq(@seq.seqbase); 
//    seq:= seq.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//sce: pScene; 
//begin 
//  ed: pEditing; 
//  seq: pSequence; 
//  strip: pStrip; 
//  se: pStripElem; 
//  ms: pMetaStack; 
//  a: integer; 
//  link_list(@(sce.base)); 
//  sce.basact:= newadr(sce.basact); 
//  sce.radio:= newadr(sce.radio); 
//  
//  if sce.ed<>0{nil} {<= !!!9} 
//  then
//  begin 
//    ed:= sce.ed:=newadr(sce.ed); 
//    ed.metastack.first:= ed.metastack.last:=0; 
//    link_recurs_seq( and ed.seqbase); 
//    (* recursief sequenties linken, ook lb wordt goedgezet *)
//    ed.seqbasep:=  and ed.seqbase; 
//    WHILE_SEQ(ed.seqbasep); 
//    begin 
//      seq.seq1:= newadr(seq.seq1); 
//      seq.seq2:= newadr(seq.seq2); 
//      seq.seq3:= newadr(seq.seq3); (* eigenlijk een patch: na invoering drie-seq effects *)
//      if seq.seq3=0
//      then
//      seq.seq3:= seq.seq2; 
//      seq.curelem:= 0; 
//      seq.plugin:= newadr(seq.plugin); 
//      if seq.plugin<>0{nil} {<= !!!9} 
//      then
//      open_plugin_seq(seq.plugin,seq.name+2); 
//      seq.strip:= newadr(seq.strip); 
//      if seq.strip)and(seq.strip.done=0
//      then
//      begin 
//        seq.strip.done:= 1; (* standaard: strips van effecten/meta's worden niet weggeschreven, wel malloccen *)
//        if seq.type=SEQ_IMAGE
//        then
//        begin 
//          seq.strip.stripdata:= newadr(seq.strip.stripdata); 
//          se:= seq.strip.stripdata; 
//          if se<>0{nil} {<= !!!9} 
//          then
//          begin 
//            while{for} 
//            a:= 0; 
//            {to} a<seq.strip.len
//            {;}
//            inc(a); 
//            inc(se); 
//            do
//            begin 
//              se.ok:= 1; 
//              se.ibuf:= 0; 
//            end;
//          end;
//        end;
//        else
//        if seq.type=SEQ_MOVIE
//        then
//        begin 
//          (* alleen eerste stripelem zit in file *)
//          se:= newadr(seq.strip.stripdata); 
//          if se<>0{nil} {<= !!!9} 
//          then
//          begin 
//            seq.strip.stripdata:= callocN(seq.len*sizeof(StripElem),'stripelem'); {*}seq.strip.stripdata^:=*se; 
//            freeN(se); 
//            se:= seq.strip.stripdata; 
//            while{for} 
//            a:= 0; 
//            {to} a<seq.strip.len
//            {;}
//            inc(a); 
//            inc(se); 
//            do
//            begin 
//              se.ok:= 1; 
//              se.ibuf:= 0; 
//              se.nr:= a+1; 
//            end;
//          end;
//        end;
//        else
//        if seq.len>0
//        then
//        seq.strip.stripdata:= callocN(seq.len*sizeof(StripElem),'stripelem'); 
//        
//      end;
//    end;
//    END_SEQ{!!!e unknown token}
//  end;
//  direct_link_scriptlink(@sce.scriptlink); 
//end;
//
//(* ************ READ SCREEN ***************** *)
//
//
//procedure lib_link_screen(main: pMain); 
//var
//sc: pbScreen; 
//sa: pScrArea; 
//sv: pScrVert; 
//se: pScrEdge; 
//v3d: pView3D; 
//v2d: pView2D; 
//si: pSpaceIpo; 
//buts: pSpaceButs; 
//sfile: pSpaceFile; 
//oops: pOops; 
//sima: pSpaceImage;
// 
//st: pSpaceText;
// 
//so: pSpaceOops;
// (* patch als deze in oude files zit *)
//begin
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  sc:= main.screen.first; 
//  while sc
//  do
//  begin 
//    if sc.id.flag and LIB_NEEDLINK
//    then
//    begin 
//      sc.id.us:= 1; 
//      sc.scene:= newlibadr(sc.id.lib,sc.scene); 
//      sa:= sc.areabase.first; 
//      while sa
//      do
//      begin 
//        sa.full:= newlibadr(sc.id.lib,sa.full); 
//        v3d:= sa.spacedata.first; (* v3d als voorbeeld *)
//        while v3d
//        do
//        begin 
//          if v3d.spacetype=SPACE_VIEW3D
//          then
//          begin 
//            
//            v3d.camera:= newlibadr(sc.id.lib,v3d.camera); 
//            if v3d.bgpic<>0{nil} {<= !!!9} 
//            then
//            begin 
//              v3d.bgpic.ima:= newlibadr_us(sc.id.lib,v3d.bgpic.ima); 
//              v3d.bgpic.tex:= newlibadr_us(sc.id.lib,v3d.bgpic.tex); 
//              v3d.bgpic.rect:= 0; 
//            end;
//            if v3d.localvd<>0{nil} {<= !!!9} 
//            then
//            begin 
//              v3d.localvd.camera:= newlibadr(sc.id.lib,v3d.localvd.camera); 
//            end;
//          end;
//          else
//          if v3d.spacetype=SPACE_IPO
//          then
//          begin 
//            si:= {!!!a type cast? =>} {pSpaceIpo(}v3d; 
//            si.editipo:= 0; 
//            si.from:= 0; 
//            si.ipokey.first:= si.ipokey.last:=0; 
//            si.ipo:= newlibadr(sc.id.lib,si.ipo); 
//          end;
//          else
//          if v3d.spacetype=SPACE_BUTS
//          then
//          begin 
//            buts:= {!!!a type cast? =>} {pSpaceButs(}v3d; 
//            buts.rect:= 0; 
//            buts.lockpoin:= 0; 
//            if main.versionfile<132
//            then
//            set_rects_butspace(buts); 
//          end;
//          else
//          if v3d.spacetype=SPACE_FILE
//          then
//          begin 
//            sfile:= {!!!a type cast? =>} {pSpaceFile(}v3d; 
//            sfile.filelist:= 0; 
//            sfile.libfiledata:= 0; 
//            sfile.returnfunc:= 0; 
//          end;
//          else
//          if v3d.spacetype=SPACE_IMASEL
//          then
//          begin 
//            check_imasel_copy({!!!a type cast? =>} {pSpaceImaSel(}v3d); 
//          end;
//          else
//          if v3d.spacetype=SPACE_PAINT
//          then
//          begin 
//          end;
//          else
//          if v3d.spacetype=SPACE_IMAGE
//          then
//          begin 
//            sima:={!!!a type cast? =>} {pSpaceImage(}v3d; 
//            sima.image:= newlibadr(sc.id.lib,sima.image); 
//          end;
//          else
//          if v3d.spacetype=SPACE_TEXT
//          then
//          begin 
//            st:={!!!a type cast? =>} {pSpaceText(}v3d; 
//            st.text:= newlibadr(sc.id.lib,st.text); 
//            st.py_draw:= 0{nil}; 
//            st.py_event:= 0{nil}; 
//            st.py_button:= 0{nil}; 
//            st.py_head_draw:= 0{nil}; 
//            st.py_head_event:= 0{nil}; 
//            st.py_head_button:= 0{nil}; 
//          end;
//          else
//          if v3d.spacetype=SPACE_OOPS
//          then
//          begin 
//            so:={!!!a type cast? =>} {pSpaceOops(}v3d; (* patch als deze in oude files zit *)
//            if so.v2d.cur.xmin=so.v2d.cur.xmax
//            then
//            begin 
//              init_v2d_oops(@so.v2d); 
//            end;
//            oops:= so.oops.first; 
//            while oops
//            do
//            begin 
//              oops.id:= newlibadr(0,oops.id); 
//              oops:= oops.next; 
//            end;
//            so.lockpoin:= 0; 
//          end;
//          v3d:= v3d.next; 
//        end;
//        sa:= sa.next; 
//      end;
//      sc.id.flag:= sc.id.flag - (LIB_NEEDLINK); 
//    end;
//    sc:= sc.id.next; 
//  end;
//end;
//
//void{!!!e unknown token}
//sc: pbScreen; 
//begin 
//  sa: pScrArea; 
//  sv: pScrVert; 
//  se: pScrEdge; 
//  sen: pScrEdge; 
//  v3d: pView3D; 
//  soops: pSpaceOops; 
//  oops: pOops; 
//  link_list(@(sc.vertbase)); 
//  link_list(@(sc.edgebase)); 
//  link_list(@(sc.areabase)); 
//  sc.winakt:= 0; 
//  se:= sc.edgebase.first; 
//  (* edges *)
//  while se
//  do
//  begin 
//    se.v1:= newadr(se.v1); 
//    se.v2:= newadr(se.v2); 
//    if {!!!a type cast? =>} {integer(}se.v1>{!!!a type cast? =>} {integer(}se.v2
//    then
//    begin 
//      sv:= se.v1; 
//      se.v1:= se.v2; 
//      se.v2:= sv; 
//    end;
//    if se.v1=0{nil}
//    then
//    begin 
//      printf('error reading screen... file corrupt\n');
//      se.v1:= se.v2; 
//    end;
//    se:= se.next; 
//  end;
//  sa:= sc.areabase.first; 
//  (* areas *)
//  while sa
//  do
//  begin 
//    link_list(@(sa.spacedata)); 
//    v3d:= sa.spacedata.first; 
//    while v3d
//    do
//    begin 
//      if v3d.spacetype=SPACE_VIEW3D
//      then
//      begin 
//        v3d.bgpic:= newadr(v3d.bgpic); 
//        v3d.localvd:= newadr(v3d.localvd); 
//      end;
//      else
//      if v3d.spacetype=SPACE_OOPS
//      then
//      begin 
//        soops:= {!!!a type cast? =>} {pSpaceOops(}v3d; 
//        link_list(@(soops.oops)); 
//        oops:= soops.oops.first; 
//        while oops
//        do
//        begin 
//          oops.link.first:= oops.link.last:=0; 
//          oops:= oops.next; 
//        end;
//      end;
//      v3d:= v3d.next; 
//    end;
//    sa.v1:= newadr(sa.v1); 
//    sa.v2:= newadr(sa.v2); 
//    sa.v3:= newadr(sa.v3); 
//    sa.v4:= newadr(sa.v4); 
//    sa.win:= sa.headwin:=0; 
//    sa.cursor:= CURSOR_STD; 
//    sa.headqueue:= sa.hq:=sa.winqueue:=sa.wq:=0; 
//    set_func_space(sa); 
//    
//    sa:= sa.next; 
//    (* space.c *)
//  end;
//  test_scale_screen(sc); 
//  
//  (* vertices en offset *)
//end;
//
//(* ********** READ LIBRARY *************** *)
//
//
//void{!!!e unknown token}
//lib: pLibrary; 
//begin 
//  newmain: pMain; (* nieuwe main *)
//  newmain:= callocN(sizeof(Main),'directlink'); 
//  addtail(@G.mainbase,newmain); 
//  newmain.curlib:= lib; 
//  
//end;
//
//void{!!!e unknown token}
//main: pMain; 
//begin 
//  lib: pLibrary; 
//  lib:= main.library.first; 
//  while lib
//  do
//  begin 
//    lib.id.us:= 1; 
//    lib:= lib.id.next; 
//  end;
//end;
//
//
//(* ************** ALG & MAIN ******************** *)
//
//
//function read_libblock(main: pMain;  bhead: pBHead;  flag: integer): integer; 
//var
//id: pID; 
//lb: pListBase; 
//skipdata: integer; 
//fd: pchar; 
//begin(* deze routine leest libblock en direkte data. Met linkfunkties
//    * alles aan elkaar hangen.
//    *)
//  
//  
//  
//  if bhead.code=ID_ID
//  then
//  begin 
//    id:= {!!!a type cast? =>} {pID(}(bhead+1); 
//    lb:= wich_libbase(main,GS(id.name)); 
//    (* printf("idid %s oldp %x\n", id->name, bhead->old); *)
//  end;
//  else
//  begin 
//    lb:= wich_libbase(main,bhead.code); 
//  end;
//  
//  if lb=0
//  then
//  begin 
//    (* PRINT2(s, d, ((char * )&(bhead->code))+2, bhead->len); *)
//    if lb=0
//    then
//    begin
//      result:= bhead.len+sizeof(BHead); 
//      exit;
//    end;
//  end;
//  fd:= {!!!a type cast? =>} {pchar(}bhead; 
//  id:= read_libstruct(bhead); 
//  addtail(lb,id); 
//  (* libblock inlezen *)
//  id.flag:= (id.flag and $FF00) or flag or LIB_NEEDLINK; 
//  id.lib:= main.curlib; (* eerste acht bits wissen *)
//  if id.flag and LIB_FAKEUSER
//  then
//  id.us:= 1; 
//  else
//  id.us:= 0; (* deze mag niet door de direct_link molen: is alleen het ID deel *)
//  if bhead.code=ID_ID
//  then
//  begin 
//    skipdata:= bhead.len+sizeof(BHead); 
//    begin
//      result:= skipdata; 
//      exit;
//    end;
//  end;
//  skipdata:= bhead.len+sizeof(BHead); 
//  fd:= fd + (bhead.len+sizeof(BHead)); 
//  bhead:= {!!!a type cast? =>} {pBHead(}fd; 
//  
//  (* alle data inlezen *)
//  while bhead.code=DATA
//  do
//  begin 
//    read_struct(bhead); 
//    skipdata:= skipdata + (bhead.len+sizeof(BHead)); 
//    fd:= fd + (bhead.len+sizeof(BHead)); 
//    bhead:= {!!!a type cast? =>} {pBHead(}fd; 
//  end;
//  
//  (* pointers directe data goedzetten *)
//  case GS(id.name) of
//    ID_SCR:
//    begin
//      direct_link_screen(id); 
//    end;
//    ID_SCE:
//    begin
//      direct_link_scene(id); 
//    end;
//    ID_OB:
//    begin
//      direct_link_object(id); 
//    end;
//    ID_ME:
//    begin
//      direct_link_mesh(id); 
//    end;
//    ID_CU:
//    begin
//      direct_link_curve(id); 
//    end;
//    ID_MB:
//    begin
//      direct_link_mball(id); 
//    end;
//    ID_MA:
//    begin
//      direct_link_material(id); 
//    end;
//    ID_TE:
//    begin
//      direct_link_texture(id); 
//    end;
//    ID_IM:
//    begin
//      direct_link_image(id); 
//    end;
//    ID_LA:
//    begin
//      direct_link_lamp(id); 
//    end;
//    ID_VF:
//    begin
//      direct_link_vfont(id); 
//    end;
//    ID_TXT:
//    begin
//      direct_link_text(id); 
//    end;
//    ID_IP:
//    begin
//      direct_link_ipo(id); 
//    end;
//    ID_KE:
//    begin
//      direct_link_key(id); 
//    end;
//    ID_LT:
//    begin
//      direct_link_latt(id); 
//    end;
//    ID_IK:
//    begin
//      direct_link_ika(id); 
//    end;
//    ID_SE:
//    begin
//      direct_link_sector(id); 
//    end;
//    ID_LF:
//    begin
//      direct_link_life(id); 
//    end;
//    ID_WO:
//    begin
//      direct_link_world(id); 
//    end;
//    ID_LI:
//    begin
//      direct_link_library(id); 
//    end;
//    ID_CA:
//    begin
//      direct_link_camera(id); 
//    end;
//    
//  end;{case?}
//  (* vrijgeven, herinitialiseren *)
//  add_data_adr(0,(void* )1); 
//  begin
//    result:= skipdata; 
//    exit;
//  end;
//end;
//
//
//procedure link_global(fg: pFileGlobal); 
//begin
//  R.displaymode:= fg.displaymode; 
//  R.winpos:= fg.winpos; 
//  strcpy(G.psx,fg.psx); 
//  
//  
//  G.curscreen:= newlibadr(0,fg.curscreen); 
//  if G.curscreen=0
//  then
//  G.curscreen:= G.main.screen.first; 
//  G.scene:= G.curscreen.scene; 
//  G.obedit:= 0; 
//  G.buts:= 0; 
//  G.v2d:= 0; 
//  G.vd:= 0; 
//  G.soops:= 0; 
//  G.sima:= 0; 
//  G.sipo:= 0; 
//end;
//
//
//procedure vcol_to_fcol(me: pMesh); 
//var
//mface: pMFace; 
//mcol: puint; 
//mcoln: puint; 
//mcolmain: puint; 
//a: integer; 
//begin
//  
//  
//  
//  
//  if me.totface=0)or(me.mcol=0
//  then
//  exit;
//  mcoln:= mcolmain:=mallocN(4*sizeof(int)*me.totface,'mcoln'); 
//  mcol:= {!!!a type cast? =>} {puint(}me.mcol; 
//  mface:= me.mface; 
//  while{for} 
//  a:= me.totface; 
//  {to} a>0
//  {;}
//  dec(a); 
//  inc(mface); 
//  do
//  begin 
//    mcoln[0]:= mcol[mface.v1]; 
//    mcoln[1]:= mcol[mface.v2]; 
//    mcoln[2]:= mcol[mface.v3]; 
//    mcoln[3]:= mcol[mface.v4]; 
//    mcoln:= mcoln + (4); 
//  end;
//  freeN(me.mcol); 
//  me.mcol:= {!!!a type cast? =>} {pMCol(}mcolmain; 
//end;
//
//
//procedure oldstyle_events(lf: pLife); 
//var
//sn: pSensor; 
//a: integer; 
//begin
//  
//  
//  sn:= lf.sensors; 
//  a:= lf.totsens; 
//  while a{--} dec(a); 
//  do
//  begin 
//    case sn.action of
//      1:
//      begin
//        sn.force:= -sn.force; 
//        sn.action:= SN_X_TRANS; 
//      end;
//      3:
//      begin
//        sn.force:= -sn.force; 
//        sn.action:= SN_Y_TRANS; 
//      end;
//      5:
//      begin
//        sn.force:= -sn.force; 
//        sn.action:= SN_Z_TRANS; 
//      end;
//      7:
//      begin
//        sn.force:= -sn.force; 
//        sn.action:= SN_X_ROT; 
//      end;
//      9:
//      begin
//        sn.force:= -sn.force; 
//        sn.action:= SN_Y_ROT; 
//      end;
//      11:
//      begin
//        sn.force:= -sn.force; 
//        sn.action:= SN_Z_ROT; 
//      end;
//      
//    end;{case?}
//    inc(sn); 
//  end;
//end;
//
//
//procedure do_versions(main: pMain); 
//var
//tex: pTex; 
//ob: pObject; 
//ma: pMaterial; 
//sce: pScene; 
//a: integer; 
//b: integer; 
//me: pMesh; (* mcol is veranderd *)
//
//lf: pLife;
// 
//se: pSector;
// 
//sc: pbScreen; 
//sa: pScrArea; 
//vd: pView3D; 
//lf: pLife; 
//sn: pSensor; (* nieuwe variabele: gridlines *)
//lf: pLife; 
//sn: pSensor; (* sensor omgebouwd *)
//
//lf: pLife; 
//cam: pCamera; 
//nl: pNetLink; (* nieuwe variabele *)
//me: pMesh;
// 
//me: pMesh;
// 
//mvert: pMVert; 
//mface: pMFace; 
//a: integer; 
//edcode: smallint; (* edge drawflags veranderd *)
//lf: pLife; 
//sn: pSensor; 
//ac: pAction; (* frictfac in action op 1.0 zetten *)
//lf: pLife; 
//sn: pSensor; 
//ev: pEvent; 
//ac: pAction; 
//sethold: integer; (* flag in action zetten, holdflags in events *)
//lf: pLife; 
//sn: pSensor; 
//ev: pEvent; 
//ac: pAction; (*  naam wissen *)
//lf: pLife; 
//ac: pAction; 
//sn: pSensor; 
//ma: pMaterial; 
//sc: pbScreen; 
//sa: pScrArea; 
//tex: pTex; 
//tex: pTex; (* r-g-b-fac in texure *)
//me: pMesh;
// 
//me: pMesh;
// 
//tface: pTFace; 
//ika: pIka;
// 
//def: pDeform; 
//nr: integer; 
//cp: pchar; 
//me: pMesh;
// 
//sc: pbScreen;
// 
//sa: pScrArea; 
//sipo: pSpaceIpo; 
//ob: pObject;
// 
//paf: pPartEff; 
//sc: pbScreen;
// 
//sa: pScrArea; 
//st: pSpaceText; 
//begin(* PAS OP: pointers van libdata zijn nog niet omgezet *)
//  
//  
//  
//  
//  
//  if main.versionfile=100
//  then
//  begin 
//    
//    (* tex->extend en tex->imageflag veranderd: *)
//    tex:= main.tex.first; 
//    while tex
//    do
//    begin 
//      if tex.id.flag and LIB_NEEDLINK
//      then
//      begin 
//        if tex.extend=0
//        then
//        begin 
//          if tex.xrepeat)or(tex.yrepeat
//          then
//          tex.extend:= TEX_REPEAT; 
//          else
//          begin 
//            tex.extend:= TEX_EXTEND; 
//            tex.xrepeat:= tex.yrepeat:=1; 
//          end;
//        end;
//        if tex.imaflag and TEX_ANIM5
//        then
//        begin 
//          tex.imaflag:= tex.imaflag or (TEX_MORKPATCH); 
//          tex.imaflag:= tex.imaflag or (TEX_ANTIALI); 
//        end;
//      end;
//      tex:= tex.id.next; 
//    end;
//  end;
//  if main.versionfile<=101
//  then
//  begin 
//    (* frame mapping *)
//    sce:= main.scene.first; 
//    while sce
//    do
//    begin 
//      sce.r.framapto:= 100; 
//      sce.r.images:= 100; 
//      sce.r.framelen:= 1.0; 
//      sce:= sce.id.next; 
//    end;
//  end;
//  if main.versionfile<=102
//  then
//  begin 
//    (* init halo's op 1.0 *)
//    ma:= main.mat.first; 
//    while ma
//    do
//    begin 
//      ma.add:= 1.0; 
//      ma:= ma.id.next; 
//    end;
//  end;
//  if main.versionfile<=103
//  then
//  begin 
//    (* nieuwe variabele in object: colbits *)
//    ob:= main.object.first; 
//    while ob
//    do
//    begin 
//      ob.colbits:= 0; 
//      if ob.totcol<>0{nil} {<= !!!9} 
//      then
//      begin 
//        for{while} a:=0 to Pred(ob.totcol) { a++}
//        do
//        begin 
//          if ob.mat[a]<>0{nil} {<= !!!9} 
//          then
//          ob.colbits:= ob.colbits or ((1 shl a)); 
//        end;
//      end;
//      ob:= ob.id.next; 
//    end;
//  end;
//  if main.versionfile<=104
//  then
//  begin 
//    (* de timeoffs zit op betere plek *)
//    ob:= main.object.first; 
//    while ob
//    do
//    begin 
//      if ob.transflag and 1
//      then
//      begin 
//        ob.transflag:= ob.transflag - (1); 
//        ob.ipoflag:= ob.ipoflag or (OB_OFFS_OB); 
//      end;
//      ob:= ob.id.next; 
//    end;
//  end;
//  if main.versionfile<=105
//  then
//  begin 
//    ob:= main.object.first; 
//    while ob
//    do
//    begin 
//      ob.dupon:= 1; 
//      ob.dupoff:= 0; 
//      ob.dupsta:= 1; 
//      ob.dupend:= 100; 
//      ob:= ob.id.next; 
//    end;
//  end;
//  if main.versionfile<=106
//  then
//  begin 
//    me:= main.mesh.first; 
//    while me
//    do
//    begin 
//      if me.mcol<>0{nil} {<= !!!9} 
//      then
//      vcol_to_fcol(me); 
//      me:= me.id.next; 
//    end;
//  end;
//  if main.versionfile<=107
//  then
//  begin 
//    (* tot dec 96 ofzo *)
//    lf:=main.life.first; 
//    while lf
//    do
//    begin 
//      oldstyle_events(lf); 
//      lf:= lf.id.next; 
//    end;
//    sce:= main.scene.first; 
//    while sce
//    do
//    begin 
//      sce.r.mode:= sce.r.mode or (R_GAMMA); 
//      sce:= sce.id.next; 
//    end;
//    ob:= main.object.first; 
//    while ob
//    do
//    begin 
//      ob.ipoflag:= ob.ipoflag or (OB_OFFS_PARENT); 
//      if ob.dt=0
//      then
//      ob.dt:= 3; 
//      ob:= ob.id.next; 
//    end;
//  end;
//  if main.versionfile<=108
//  then
//  begin 
//    se:=main.sector.first; 
//    while se
//    do
//    begin 
//      se.size[0]:= se.size[1]:=se.size[2]:=1.0; 
//      se:= se.id.next; 
//    end;
//  end;
//  if main.versionfile<=109
//  then
//  begin 
//    
//    
//    
//    
//    sc:= main.screen.first; 
//    while sc
//    do
//    begin 
//      sa:= sc.areabase.first; 
//      while sa
//      do
//      begin 
//        vd:= sa.spacedata.first; 
//        while vd
//        do
//        begin 
//          if vd.spacetype=SPACE_VIEW3D
//          then
//          begin 
//            if vd.gridlines=0
//            then
//            vd.gridlines:= 20; 
//          end;
//          vd:= vd.next; 
//        end;
//        sa:= sa.next; 
//      end;
//      sc:= sc.id.next; 
//    end;
//    lf:= main.life.first; (* sensors rename: *)
//    while lf
//    do
//    begin 
//      sn:= lf.sensors; 
//      a:= lf.totsens; 
//      while a{--} dec(a); 
//      do
//      begin 
//        sn.sta:= sn.fac1; 
//        sn.end:= sn.fac2; 
//        sn.fac1:= sn.fac2:=0.0; 
//        inc(sn); 
//      end;
//      lf:= lf.id.next; 
//    end;
//  end;
//  if main.versionfile<=110
//  then
//  begin 
//    
//    lf:= main.life.first; 
//    while lf
//    do
//    begin 
//      sn:= lf.sensors; 
//      a:= lf.totsens; 
//      while a{--} dec(a); 
//      do
//      begin 
//        sn.totevent:= 1; 
//        sn.events:= callocN(sizeof(Event),'event'); 
//        sn.events.event:= sn.event1; 
//        if sn.event1=19
//        then
//        begin 
//          sn.events.event:= 31; (* always *)
//          if sn.action=SN_PLAY_OBIPO
//          then
//          sn.action:= SN_LOOPSTOP_OBIPO; 
//        end;
//        sn.totaction:= 1; 
//        sn.actions:= callocN(sizeof(Action),'action'); 
//        sn.actions.action:= sn.action; 
//        sn.actions.force:= sn.force; 
//        sn.actions.butsta:= ffloor(sn.sta+0.5); 
//        sn.actions.butend:= ffloor(sn.end+0.5); 
//        inc(sn); 
//      end;
//      lf:= lf.id.next; 
//    end;
//  end;
//  if main.versionfile<=111
//  then
//  begin 
//    
//    
//    cam:= main.camera.first; 
//    while cam
//    do
//    begin 
//      if cam.netsta=0.0
//      then
//      cam.netsta:= 1.0; 
//      if cam.netend=0.0
//      then
//      cam.netend:= 20.0; 
//      cam:= cam.id.next; 
//    end;
//    ob:= main.object.first; (* vergeten DNA weg te schrijven in BHead *)
//    while ob
//    do
//    begin 
//      freelistN(@ob.network); 
//      ob:= ob.id.next; 
//    end;
//    lf:= main.life.first; (* nieuwe default optie *)
//    while lf
//    do
//    begin 
//      lf.flag:= lf.flag or (LF_DO_FH); 
//      lf:= lf.id.next; 
//    end;
//  end;
//  if main.versionfile<=112
//  then
//  begin 
//    me:=main.mesh.first; 
//    while me
//    do
//    begin 
//      me.cubemapsize:= 1.0; 
//      me:= me.id.next; 
//    end;
//  end;
//  if main.versionfile<=113
//  then
//  begin 
//    ma:= main.mat.first; 
//    while ma
//    do
//    begin 
//      if ma.flaresize=0.0
//      then
//      ma.flaresize:= 1.0; 
//      ma.subsize:= 1.0; 
//      ma.flareboost:= 1.0; 
//      ma:= ma.id.next; 
//    end;
//  end;
//  if main.versionfile<=114
//  then
//  begin 
//    me:=main.mesh.first; 
//    
//    
//    
//    while me
//    do
//    begin 
//      a:= me.totface; 
//      mface:= me.mface; 
//      while a{--} dec(a); 
//      do
//      begin 
//        if mface.edcode and 16
//        then
//        begin 
//          mface.edcode:= mface.edcode - (16); 
//          mface.edcode:= mface.edcode or (ME_V3V1); 
//          mface.edcode:= mface.edcode or (DF_V1V2 or DF_V2V3 or DF_V3V4 or DF_V4V1); 
//          (* cylinder *)
//        end;
//        inc(mface); 
//      end;
//      a:= me.totvert; 
//      mvert:= me.mvert; 
//      while a{--} dec(a); 
//      do
//      begin 
//        mvert.flag:= ME_SPHERETEST; 
//        inc(mvert); 
//      end;
//      me:= me.id.next; 
//    end;
//  end;
//  if main.versionfile<=115
//  then
//  begin 
//    
//    
//    lf:= main.life.first; 
//    while lf
//    do
//    begin 
//      a:= lf.totsens; 
//      sn:= lf.sensors; 
//      while a{--} dec(a); 
//      do
//      begin 
//        ac:= sn.actions; 
//        b:= sn.totaction; 
//        while b{--} dec(b); 
//        do
//        begin 
//          ac.fac:= 1.0; 
//          inc(ac); 
//        end;
//        inc(sn); 
//      end;
//      lf:= lf.id.next; 
//    end;
//  end;
//  if main.versionfile<=116
//  then
//  begin 
//    
//    
//    
//    
//    lf:= main.life.first; 
//    while lf
//    do
//    begin 
//      a:= lf.totsens; 
//      sn:= lf.sensors; 
//      while a{--} dec(a); 
//      do
//      begin 
//        sethold:= 0; 
//        ac:= sn.actions; 
//        b:= sn.totaction; 
//        while b{--} dec(b); 
//        do
//        begin 
//          if ac.action<100
//          then
//          begin 
//            sethold:= 1; 
//          end;
//          else
//          if ac.action<200
//          then
//          begin 
//            ac.flag:= ac.flag or (SN_IPOFORCE); 
//          end;
//          else
//          if ac.action=SN_ADDLIFE
//          then
//          sethold:= -1; 
//          inc(ac); 
//        end;
//        ev:= sn.events; 
//        b:= sn.totevent; 
//        while b{--} dec(b); 
//        do
//        begin 
//          if sethold=-1
//          then
//          ; 
//          else
//          (* niks doen: add life geen hold *)
//          begin 
//            if sethold)or((sn.flag and SN_HOLD)
//            then
//            ev.flag:= SN_HOLD; if{!!!e unknown token}
//            ev.flag:= SN_HOLD; if{!!!e unknown token}
//            ev.flag:= SN_HOLD; 
//          end;
//          inc(ev); 
//        end;
//        sn.flag:= 0; 
//        inc(sn); 
//      end;
//      lf:= lf.id.next; 
//    end;
//  end;
//  if main.versionfile<=117
//  then
//  begin 
//    
//    
//    
//    lf:= main.life.first; 
//    while lf
//    do
//    begin 
//      (* stond in buttons.c *)
//      if lf.axsize=0.0
//      then
//      lf.axsize:= 0.5; 
//      a:= lf.totsens; 
//      sn:= lf.sensors; 
//      while a{--} dec(a); 
//      do
//      begin 
//        ev:= sn.events; 
//        b:= sn.totevent; 
//        while b{--} dec(b); 
//        do
//        begin 
//          if ev.event=SN_NEAR
//          then
//          ev.name[0]:= 0; 
//          inc(ev); 
//        end;
//        inc(sn); 
//      end;
//      lf:= lf.id.next; 
//    end;
//  end;
//  if main.versionfile<=119
//  then
//  begin 
//    (* incl 118 *)
//    
//    
//    
//    
//    lf:= main.life.first; 
//    while lf
//    do
//    begin 
//      if lf.lay=0
//      then
//      lf.lay:= 1; 
//      a:= lf.totsens; 
//      sn:= lf.sensors; (* goto naam wissen *)
//      while a{--} dec(a); 
//      do
//      begin 
//        ac:= sn.actions; 
//        b:= sn.totaction; 
//        while b{--} dec(b); 
//        do
//        begin 
//          if ac.action=SN_GOTO
//          then
//          ac.name[0]:= 0; 
//          inc(ac); 
//        end;
//        inc(sn); 
//      end;
//      lf:= lf.id.next; 
//    end;
//    ma:= main.mat.first; 
//    while ma
//    do
//    begin 
//      if ma.lay=0
//      then
//      ma.lay:= 1; 
//      ma:= ma.id.next; 
//    end;
//  end;
//  
//  (* eentje overgeslagen voor bug in freeware versie *)
//  if main.versionfile<=121
//  then
//  begin 
//    (* O2 versie gemaakt. *)
//  end;
//  if main.versionfile<=122
//  then
//  begin 
//    (* dithering gaat soms af (backbuf, pas sinds 121) *)
//    (* relatieve paden hersteld *)
//    (* sequences: endframe van seq wordt op betere plek geprint *)
//  end;
//  if main.versionfile<=123
//  then
//  begin 
//    (* nog een paar O2 foutjes: keylines in ipo window *)
//    (* vertices halo object (O2) nu ook goed *)
//    (* zoomwin: ook op O2 *)
//    (* bug eruit: schaduw render in ortho *)
//  end;
//  if main.versionfile<=124
//  then
//  begin 
//    (* inventor lezer *)
//    (* key kleur 24 bits beveiligd *)
//    (* schrijf plaatje: je kun niet naderhand 24bits naar 32 omzetten *)
//  end;
//  if main.versionfile<=125
//  then
//  begin 
//    (* bug vanwege compileer fout (makefile/.h dependency)*)
//  end;
//  if main.versionfile<=126
//  then
//  begin 
//    (* overdraw text beter (clever numbuts) *)
//    (* bug uit inventor lezer: node ambientColor werd niet herkend *)
//    (* bugje uit toolbox: clear loc= alt-g *)
//  end;
//  
//  if main.versionfile<=130
//  then
//  begin 
//    (* openGL en GLUT *)
//    
//    
//    
//    sc:= main.screen.first; 
//    while sc
//    do
//    begin 
//      sa:= sc.areabase.first; 
//      while sa
//      do
//      begin 
//        if sa.cursor=0
//        then
//        sa.cursor:= CURSOR_STD; 
//        sa:= sa.next; 
//      end;
//      sc:= sc.id.next; 
//    end;
//  end;
//  if main.versionfile<=131
//  then
//  begin 
//    (* jpeq quality button *)
//    (* anim5 and blacksmith demo *)
//    (* foutje uit transp zbuf: te vroege afbreek *)
//    (* geen paarse code meer als imap onvindbaar is meer *)
//    (* locx werd niet geprint: string overflow! *)
//    (* unieke namen: werkte niet *)
//    (* toolbox menu: ook alt en ctrl keys *)
//  end;
//  if main.versionfile<=132
//  then
//  begin 
//    (* strings in Userdef: eroverheen! *)
//    (* betere overdraw implementatie (numbuts) *)
//    (* snapmenu redraw *)
//    (* warp met 1 vertex *)
//  end;
//  if main.versionfile<=133
//  then
//  begin 
//    (* bug uit 'make edge face' (array overflow *)
//    (* volledig X getekende menu's *)
//    (* storage.c terug *)
//  end;
//  if main.versionfile<=134
//  then
//  begin 
//    (* Play (flipbook) restored *)
//    (* Timecursor restored *)
//    (* Debug option -d; prints a lot of info in console *)
//    (* Text Object. Accentcodes fixed: ALT+BACKSPACE *)
//    (* Cursor was sometimes wrong after reading files *)
//    (* Texspace draw error: dashed lines *)
//    (* Draw Schematic View now with icons in Objects *)
//    (* Ortho camera: zbuffer improved. / still not OK *)
//    (* Text Object. Character pound= alt-l *)
//    (* In editmode and 'set', draw error fixed. *)
//    (* Scanline display during rendering had dropouts *)
//    (* Sometimes - after render - frontbuffer drawing wasn't disabled *)
//    (* Sometimes the render window got black and Blender 'hanged'. *)
//    (* Better 'active window' implementation. *)
//    (* Automatic name was too critical, more intuitive now *)
//    tex:= main.tex.first; 
//    while tex
//    do
//    begin 
//      if tex.rfac=0.0)and(tex.gfac=0.0)and(tex.bfac=0.0
//      then
//      begin 
//        tex.rfac:= 1.0; 
//        tex.gfac:= 1.0; 
//        tex.bfac:= 1.0; 
//        tex.filtersize:= 1.0; 
//      end;
//      tex:= tex.id.next; 
//    end;
//  end;
//  
//  if main.versionfile<=135
//  then
//  begin 
//    (* 'Windows' key resistant *)
//    (* Preview-render: RGB flip (material, lamp, world) *)
//    (* Fileselect draw error: 2nd time no redraw! *)
//    (* Names error: names were not unique automatically *)
//    (* Metaball display error: because of previous *)
//    (* CTRL and ALT and SHIFT keys sometimes were locked *)
//  end;
//  if main.versionfile<=136
//  then
//  begin 
//    (* Files incompatibility Colorband PC-SGI solved *)
//    (* RightMouse selecting was blocked after border-select *)
//    (* Border select: print size *)
//    (* Inventor: reads some 2.0 syntaxes too. Under development *)
//    (* Shift/Ctrl/Alt release events got lost while moving view *)
//    (* Particles draw (size) error fixed *)
//    (* Display type 'DispView' works *)
//    (* Metaballs convert to Mesh, normals error fixed. *)
//  end;
//  if main.versionfile<=137
//  then
//  begin 
//  end;
//  if main.versionfile<=138
//  then
//  begin 
//    (* fixed: z buffer draw and Mesh with no materials: coredump! *)
//    (* bug removed from calculation 3D Bevel Objects *)
//    (* view translation in perspective fixed *)
//    (* Drawing with ortho camera fixed *)
//    (* timing error FreeBSD version fixed *)
//    (* Mesa 3.0 included in static version *)
//    (* New: LeftMouse+RightMouse allowed at numerber-button to type in values *)
//    (* Vertex paint bug fixed *)
//    (* New: ALT+(1, 2, 3...) for layers 11, 12, 13... *)
//  end;
//  if main.versionfile<=140
//  then
//  begin 
//    tex:= main.tex.first; 
//    while tex
//    do
//    begin 
//      if tex.rfac=0.0)and(tex.gfac=0.0)and(tex.bfac=0.0
//      then
//      begin 
//        tex.rfac:= 1.0; 
//        tex.gfac:= 1.0; 
//        tex.bfac:= 1.0; 
//        tex.filtersize:= 1.0; 
//      end;
//      tex:= tex.id.next; 
//    end;
//  end;
//  if main.versionfile<=153
//  then
//  begin 
//    
//    sce:= main.scene.first; 
//    while sce
//    do
//    begin 
//      if sce.r.blurfac=0.0
//      then
//      sce.r.blurfac:= 1.0; 
//      sce:= sce.id.next; 
//    end;
//  end;
//  if main.versionfile<=163
//  then
//  begin 
//    
//    sce:= main.scene.first; 
//    while sce
//    do
//    begin 
//      if sce.r.frs_sec=0
//      then
//      sce.r.frs_sec:= 25; 
//      sce:= sce.id.next; 
//    end;
//  end;
//  if main.versionfile<=164
//  then
//  begin 
//    me:=main.mesh.first; 
//    while me
//    do
//    begin 
//      me.smoothresh:= 30; 
//      me:= me.id.next; 
//    end;
//  end;
//  
//  if main.versionfile<=165
//  then
//  begin 
//    me:=main.mesh.first; 
//    
//    ika:=main.ika.first; 
//    
//    
//    
//    while ika
//    do
//    begin 
//      ika.xyconstraint:= .5; 
//      def:= ika.def; 
//      nr:= ika.totdef; 
//      while nr{--} dec(nr); 
//      do
//      begin 
//        if def.fac=0.0
//        then
//        def.fac:= 1.0; 
//        if def.dist=0.0
//        then
//        def.dist:= 1.0+def.vec[0]; 
//        inc(def); 
//      end;
//      ika:= ika.id.next; 
//    end;
//    while me
//    do
//    begin 
//      if me.tface<>0{nil} {<= !!!9} 
//      then
//      begin 
//        nr:= me.totface; 
//        tface:= me.tface; 
//        while nr{--} dec(nr); 
//        do
//        begin 
//          cp:= (char* )@tface.col[0]; 
//          if cp[1]>126
//          then
//          cp[1]:= 255; 
//          else
//          cp[1]:= cp[1] * (2); 
//          if cp[2]>126
//          then
//          cp[2]:= 255; 
//          else
//          cp[2]:= cp[2] * (2); 
//          if cp[3]>126
//          then
//          cp[3]:= 255; 
//          else
//          cp[3]:= cp[3] * (2); 
//          cp:= (char* )@tface.col[1]; 
//          if cp[1]>126
//          then
//          cp[1]:= 255; 
//          else
//          cp[1]:= cp[1] * (2); 
//          if cp[2]>126
//          then
//          cp[2]:= 255; 
//          else
//          cp[2]:= cp[2] * (2); 
//          if cp[3]>126
//          then
//          cp[3]:= 255; 
//          else
//          cp[3]:= cp[3] * (2); 
//          cp:= (char* )@tface.col[2]; 
//          if cp[1]>126
//          then
//          cp[1]:= 255; 
//          else
//          cp[1]:= cp[1] * (2); 
//          if cp[2]>126
//          then
//          cp[2]:= 255; 
//          else
//          cp[2]:= cp[2] * (2); 
//          if cp[3]>126
//          then
//          cp[3]:= 255; 
//          else
//          cp[3]:= cp[3] * (2); 
//          cp:= (char* )@tface.col[3]; 
//          if cp[1]>126
//          then
//          cp[1]:= 255; 
//          else
//          cp[1]:= cp[1] * (2); 
//          if cp[2]>126
//          then
//          cp[2]:= 255; 
//          else
//          cp[2]:= cp[2] * (2); 
//          if cp[3]>126
//          then
//          cp[3]:= 255; 
//          else
//          cp[3]:= cp[3] * (2); 
//          inc(tface); 
//        end;
//      end;
//      me:= me.id.next; 
//    end;
//  end;
//  
//  if main.versionfile<=169
//  then
//  begin 
//    me:=main.mesh.first; 
//    while me
//    do
//    begin 
//      if me.subdiv=0
//      then
//      me.subdiv:= 4; 
//      me:= me.id.next; 
//    end;
//  end;
//  if main.versionfile<=169
//  then
//  begin 
//    sc:=main.screen.first; 
//    
//    
//    while sc
//    do
//    begin 
//      sa:= sc.areabase.first; 
//      while sa
//      do
//      begin 
//        sipo:= sa.spacedata.first; 
//        while sipo
//        do
//        begin 
//          if sipo.spacetype=SPACE_IPO
//          then
//          begin 
//            sipo.v2d.max[0]:= 15000.0; 
//          end;
//          sipo:= sipo.next; 
//        end;
//        sa:= sa.next; 
//      end;
//      sc:= sc.id.next; 
//    end;
//  end;
//  
//  if main.versionfile<=170
//  then
//  begin 
//    ob:=main.object.first; 
//    
//    while ob
//    do
//    begin 
//      if paf:=give_parteff(ob)
//      then
//      begin 
//        if paf.staticstep=0
//        then
//        paf.staticstep:= 5; 
//      end;
//      ob:= ob.id.next; 
//    end;
//  end;
//  
//  if main.versionfile<=171
//  then
//  begin 
//    sc:=main.screen.first; 
//    
//    
//    while sc
//    do
//    begin 
//      sa:= sc.areabase.first; 
//      while sa
//      do
//      begin 
//        st:= sa.spacedata.first; 
//        while st
//        do
//        begin 
//          if st.spacetype=SPACE_TEXT
//          then
//          begin 
//            if st.font_id>1
//            then
//            begin 
//              st.font_id:= 0; 
//              st.lheight:= 13; 
//            end;
//          end;
//          st:= st.next; 
//        end;
//        sa:= sa.next; 
//      end;
//      sc:= sc.id.next; 
//    end;
//  end;
//end;(* onder in blender.c de nummers wijzigen! *)
//
//
//
//
//procedure lib_link_all(main: pMain); 
//begin
//  lib_link_screen(main); 
//  
//  
//  lib_link_scene(main); 
//  lib_link_object(main); 
//  lib_link_curve(main); 
//  lib_link_mball(main); 
//  lib_link_material(main); 
//  lib_link_texture(main); 
//  lib_link_image(main); 
//  lib_link_ipo(main); 
//  lib_link_key(main); 
//  lib_link_world(main); 
//  lib_link_lamp(main); 
//  lib_link_latt(main); 
//  lib_link_ika(main); 
//  lib_link_camera(main); 
//  lib_link_sector(main); 
//  lib_link_life(main); 
//  lib_link_mesh(main); 
//  
//  lib_link_library(main); 
//  (* als laatste: tpage images met users op nul *)
//  
//end;(* alleen users goedzetten *)
//
//
//
//
//function read_file_dna(filedata: pchar;  filelen: integer): integer; 
//var
//bhead: pBHead; 
//afbreek: integer;
// 
//fd: pchar; 
//begin
//  
//  afbreek:=0; 
//  
//  freestructDNA(@old_sdna); 
//  fd:= filedata; 
//  while filelen>0)and(afbreek<>3
//  do
//  begin 
//    bhead:= {!!!a type cast? =>} {pBHead(}fd; 
//    if bhead.code=ENDB
//    then
//    afbreek:= afbreek + (2); 
//    else
//    if bhead.code=DNA1
//    then
//    begin 
//      
//      old_sdna.data:= mallocN(bhead.len,'sdna'); 
//      memcpy(old_sdna.data,fd+sizeof(BHead),bhead.len); 
//      afbreek:= afbreek + (1); 
//    end;
//    fd:= fd + (bhead.len+sizeof(BHead)); 
//    filelen:= filelen - (bhead.len+sizeof(BHead)); 
//  end;
//  
//  if afbreek=3
//  then
//  begin 
//    init_structDNA(@old_sdna); 
//    set_compareflags_structDNA(); 
//    begin
//      result:= 1; 
//      exit;
//    end;
//  end;
//  else
//  begin 
//    if old_sdna.data<>0{nil} {<= !!!9} 
//    then
//    freeN(old_sdna.data); 
//    old_sdna.data:= 0; 
//    error('File not complete'); 
//    begin
//      result:= 0; 
//      exit;
//    end;
//  end;
//end;
//
//
//procedure reconstruct_bheads{!!!3 unknown typedef}; 
//var
//bhead8: pBHead8; 
//bhead4: pBHead4; 
//totbh: integer;
// 
//len: integer; 
//filelenn: integer; 
//fd: pchar; 
//fdn: pchar; 
//filedatan: pchar; 
//begin
//  
//  
//  totbh:=0; 
//  
//  
//  
//  
//  
//  if pointerlen=4
//  then
//  begin 
//    (* count bheads *)
//    len:= {*}filelen^; 
//    fd:= {*}filedata^; 
//    while len>0
//    do
//    begin 
//      bhead4:= {!!!a type cast? =>} {pBHead4(}fd; 
//      inc(totbh); 
//      if bhead4.code=ENDB
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      fd:= fd + (bhead4.len+sizeof(BHead4)); 
//      len:= len - (bhead4.len+sizeof(BHead4)); 
//    end;
//    filelenn:= {*}filelen^+4*totbh; 
//  end;
//  else
//  begin 
//    (* count bheads *)
//    len:= {*}filelen^; 
//    fd:= {*}filedata^; 
//    while len>0
//    do
//    begin 
//      bhead8:= {!!!a type cast? =>} {pBHead8(}fd; 
//      inc(totbh); 
//      if bhead8.code=ENDB
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      fd:= fd + (bhead8.len+sizeof(BHead8)); 
//      len:= len - (bhead8.len+sizeof(BHead8)); 
//    end;
//    filelenn:= {*}filelen^-4*totbh+4; 
//    (* + 4: ENDB *)
//  end;
//  fdn:= filedatan:=mallocN(filelenn,'filedatan'); 
//  fd:= {*}filedata^; 
//  if pointerlen=4
//  then
//  begin 
//    len:= {*}filelen^; 
//    while len>0
//    do
//    begin 
//      bhead4:= {!!!a type cast? =>} {pBHead4(}fd; 
//      bhead8:= {!!!a type cast? =>} {pBHead8(}fdn; 
//      bhead8.code:= bhead4.code; 
//      bhead8.len:= bhead4.len; 
//      if bhead8.code=ENDB
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      bhead8.old:= bhead4.old; 
//      bhead8.SDNAnr:= bhead4.SDNAnr; 
//      bhead8.nr:= bhead4.nr; 
//      if bhead4.len<>0{nil} {<= !!!9} 
//      then
//      memcpy(bhead8+1,bhead4+1,bhead4.len); 
//      fd:= fd + (bhead4.len+sizeof(BHead4)); 
//      fdn:= fdn + (bhead8.len+sizeof(BHead8)); 
//      len:= len - (bhead4.len+sizeof(BHead4)); 
//    end;
//  end;
//  else
//  begin 
//    len:= {*}filelen^; 
//    while len>0
//    do
//    begin 
//      bhead4:= {!!!a type cast? =>} {pBHead4(}fdn; 
//      bhead8:= {!!!a type cast? =>} {pBHead8(}fd; 
//      bhead4.code:= bhead8.code; 
//      bhead4.len:= bhead8.len; 
//      if bhead4.code=ENDB
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      if switch_endian<>0{nil} {<= !!!9} 
//      then
//      begin 
//        SWITCH_LONGINT(bhead8.old); 
//      end;
//      bhead4.old:= bhead8.old shr 3; 
//      bhead4.SDNAnr:= bhead8.SDNAnr; 
//      bhead4.nr:= bhead8.nr; 
//      if bhead8.len<>0{nil} {<= !!!9} 
//      then
//      memcpy(bhead4+1,bhead8+1,bhead8.len); 
//      fdn:= fdn + (bhead4.len+sizeof(BHead4)); 
//      fd:= fd + (bhead8.len+sizeof(BHead8)); 
//      len:= len - (bhead8.len+sizeof(BHead8)); 
//    end;
//  end;
//  freeN({*}filedata^); 
//  
//  {*}filedata^:=filedatan; 
//  {*}filelen^:=filelenn; 
//end;
//
//
//procedure switch_endian_bhead4(filedata: pchar;  filelen: integer); 
//var
//bhead: pBHead4; 
//fd: pchar; 
//str: array [0..Pred(5)] of char; 
//begin
//  
//  
//  
//  str[4]:= 0; 
//  fd:= filedata; 
//  while filelen>0
//  do
//  begin 
//    bhead:= {!!!a type cast? =>} {pBHead4(}fd; 
//    (* de ID_.. codes *)
//    if (bhead.code and $FFFF)=0
//    then
//    bhead.code:= bhead.code shr (16); 
//    if bhead.code=ENDB
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    
//    SWITCH_INT(bhead.len); 
//    SWITCH_INT(bhead.SDNAnr); 
//    SWITCH_INT(bhead.nr); 
//    fd:= fd + (bhead.len+sizeof(BHead4)); 
//    filelen:= filelen - (bhead.len+sizeof(BHead4)); 
//    
//  end;
//end;
//
//
//procedure switch_endian_bhead8(filedata: pchar;  filelen: integer); 
//var
//bhead: pBHead8; 
//fd: pchar; 
//str: array [0..Pred(5)] of char; 
//begin
//  
//  
//  
//  str[4]:= 0; 
//  fd:= filedata; 
//  while filelen>0
//  do
//  begin 
//    bhead:= {!!!a type cast? =>} {pBHead8(}fd; 
//    (* de ID_.. codes *)
//    if (bhead.code and $FFFF)=0
//    then
//    bhead.code:= bhead.code shr (16); 
//    if bhead.code=ENDB
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    
//    SWITCH_INT(bhead.len); 
//    SWITCH_INT(bhead.SDNAnr); 
//    SWITCH_INT(bhead.nr); 
//    fd:= fd + (bhead.len+sizeof(BHead8)); 
//    filelen:= filelen - (bhead.len+sizeof(BHead8)); 
//    
//  end;
//end;
//
//
//function openblenderfile(name: pchar;  filelen: pinteger): pchar; 
//var
//len: integer; 
//file: integer; 
//version: integer; 
//temp: integer; 
//temp1: integer; 
//filedata: pchar; 
//str: array [0..Pred(16)] of char; 
//begin
//  
//  
//  
//  
//  
//  
//  
//  len:= lstrlen(name); 
//  if len<6
//  then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  if name[len-1]='/')or(name[len-1]=#92
//  then
//  begin
//    result:= 0; 
//    exit;
//    
//  end;
//  if strstr(name,'.blend')
//  then
//  begin 
//    file:= open(name,O_BINARY or O_RDONLY); 
//    if file<=0
//    then
//    begin 
//      errorstr('Can'#39't find file',name,0); 
//      begin
//        result:= 0; 
//        exit;
//      end;
//    end;
//    {*}filelen^:=filesize(file); 
//    read(file,str,12); 
//    
//    if strncmp(str,'BLENDER',7)<>0
//    then
//    begin 
//      close(file); 
//      errorstr('Not a Blender file: ',name,0); 
//      begin
//        result:= 0; 
//        exit;
//      end;
//    end;
//    (* long pointer test *)
//    if str[7]='_'
//    then
//    pointerlen:= 4; 
//    else
//    pointerlen:= 8; 
//    switch_endian:= 0; (* endian test *)
//    if str[8]='V')and(G.order=L_ENDIAN
//    then
//    begin 
//      switch_endian:= 1; 
//    end;
//    else
//    if str[8]='v')and(G.order=B_ENDIAN
//    then
//    begin 
//      switch_endian:= 1; 
//    end;
//    str[12]:= 0; 
//    G.versionfile:= atoi(str+9); (*filelen)-=12; 
//    filedata:= mallocN(*filelen+12,'filedata'); 
//    read(file,filedata,*filelen); 
//    (* hele file inlezen *)
//    (* + 12: anders errors bij Little endina files. waarom??? *)
//    close(file); 
//    (* reconstruct_bheads doet ook de pointer endian switchen *)
//    if switch_endian<>0{nil} {<= !!!9} 
//    then
//    begin 
//      if pointerlen=4
//      then
//      switch_endian_bhead4(filedata,*filelen); 
//      else
//      switch_endian_bhead8(filedata,*filelen); 
//    end;
//    
//    if pointerlen<>sizeof(void* )
//    then
//    reconstruct_bheads( and filedata,filelen); 
//    
//    (* op zoek naar SDNA *)
//    if read_file_dna(filedata,*filelen)
//    then
//    begin 
//      begin
//        result:= filedata; 
//        exit;
//      end;
//    end;
//    else
//    begin 
//      freeN(filedata); 
//      begin
//        result:= 0; 
//        exit;
//      end;
//    end;
//  end;
//  else
//  begin 
//    read_exotic(name); 
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//
//procedure read_file(dir: pchar); 
//var
//bhead: pBHead; 
//main: pMain; 
//ob: pObject; 
//cu: pCurve; 
//fg: pFileGlobal; 
//user: pUserDef; 
//vf: pVFont; 
//ok: integer; 
//len: integer; 
//filelen: integer; 
//skipdata: integer; 
//temp: integer; 
//temp1: integer; 
//version: integer; 
//filedata: pchar; 
//fd: pchar; 
//begin
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  waitcursor(1); 
//  filedata:= openblenderfile(dir, and filelen); 
//  if filedata<>0{nil} {<= !!!9} 
//  then
//  begin 
//    G.save_over:= LongBool(1); 
//    strcpy(G.sce,dir); 
//    strcpy(G.main.name,dir); 
//    freeAllRad(); 
//    (* is gegarandeerd current file *)
//    (* er is maar 1 Main, dus alleen inhoud vrijgeven *)
//    free_main(0,G.main); 
//    G.curscreen:= 0; 
//    FreeButs('i'); 
//    freefastshade(); 
//    (* behalve icons *)
//    G.main.versionfile:= G.versionfile; (* anders oude lampgegevens *)
//    (* kan per main verschillen *)
//    if G.obedit<>0{nil} {<= !!!9} 
//    then
//    begin 
//      freeNurblist( and editNurb); 
//      free_editmesh(); 
//      free_editText(); 
//    end;
//    G.f:= G.f and ( not (G_VERTEXPAINT+G_FACESELECT)); 
//    fd:= filedata; 
//    ok:= 0; 
//    main:= G.main; 
//    (* alle data inlezen: *)
//    while filelen>0
//    do
//    begin 
//      bhead:= {!!!a type cast? =>} {pBHead(}fd; 
//      case bhead.code of
//        GLOB:
//        begin
//          read_struct_expl(bhead, and fg); 
//          skipdata:= bhead.len+sizeof(BHead); 
//        end;
//        DATA:
//        begin
//          skipdata:= bhead.len+sizeof(BHead); 
//        end;
//        DNA1:
//        begin
//          skipdata:= bhead.len+sizeof(BHead); 
//        end;
//        USER:
//        begin
//          read_struct_expl(bhead,@user); 
//          U:= {*}user^; 
//          freeN(user); 
//          skipdata:= bhead.len+sizeof(BHead); 
//        end;
//        TEST:
//        begin
//          skipdata:= bhead.len+sizeof(BHead); 
//        end;
//        REND:
//        begin
//          skipdata:= bhead.len+sizeof(BHead); 
//        end;
//        ENDB:
//        begin
//          ok:= 1; 
//          skipdata:= 8; 
//        end;
//        ID_LI:
//        begin
//          skipdata:= read_libblock(G.main,bhead,LIB_LOCAL); 
//          main:= G.mainbase.last; 
//        end;
//        ID_ID:
//        begin
//          skipdata:= read_libblock(main,bhead,LIB_READ+LIB_EXTERN); 
//        end;
//        else
//        begin
//          skipdata:= read_libblock(G.main,bhead,LIB_LOCAL); 
//        end;
//      end;{case?}
//      if ok<>0{nil} {<= !!!9} 
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//      
//      fd:= fd + (skipdata); 
//      filelen:= filelen - (skipdata); 
//    end;
//    freeN(filedata); 
//    
//    
//    if ok=0
//    then
//    begin 
//      printf('ERROR: file incomplete\n');
//      error('Warning: file not complete'); 
//    end;
//    do_versions(G.main); 
//    read_libraries(); 
//    (* voor read_libraries *)
//    lib_link_all(G.main); 
//    (* LibData linken *)
//    link_global(fg); 
//    add_data_adr(0,0); 
//    (* als laatste *)
//    
//    
//    (* losslingerende blokken vrijgeven *)
//    add_lib_adr(0,0); 
//    vf:= G.main.vfont.first; (* VECTORFONTS *)
//    (* afvangen: .Bfont van ander systeem *)
//    if vf<>0{nil} {<= !!!9} 
//    then
//    begin 
//      len:= lstrlen(vf.name); 
//      if len>5)and(lstrcmp(vf.name+len-5,'Bfont')=0
//      then
//      begin 
//        sprintf(vf.name,'%s/.Bfont',gethome()); 
//      end;
//    end;
//    while vf
//    do
//    begin 
//      reload_vfont(vf); 
//      vf:= vf.id.next; 
//    end;
//    ob:= G.main.object.first; (* weinig DispListen, wel text_to_curve *)
//    while ob
//    do
//    begin 
//      if ob.type=OB_FONT
//      then
//      begin 
//        cu:= ob.data; 
//        if cu.nurb.first=0
//        then
//        text_to_curve(ob,0); 
//      end;
//      else
//      if ob.type=OB_MESH
//      then
//      begin 
//        makeDispList(ob); 
//        if ob.effect.first<>0{nil} {<= !!!9} 
//        then
//        object_wave(ob); 
//      end;
//      ob:= ob.id.next; 
//      
//    end;
//    freeN(fg); 
//    if G.background=0
//    then
//    begin 
//      setscreen(G.curscreen); 
//      countall(); 
//    end;
//    set_scene_bg(G.scene); 
//    reset_autosave(); 
//    (* baseflags *)
//    clear_obact_names(); 
//    set_obact_names(OBACT); 
//    (* voor add object *)
//    do_realtimelight(0,0,0); 
//  end;
//  (* re-init *)
//  if G.background=0
//  then
//  waitcursor(0); 
//  
//  if G.f and G_SCENESCRIPT
//  then
//  begin 
//    {ext_load_str: array [0..Pred(256)] of char; }{<= !!!5 external variable}
//    if ext_load_str[0]<>0{nil} {<= !!!9} 
//    then
//    force_draw_all(); 
//    do_pyscript(G.scene,SCRIPT_ONLOAD); 
//  end;
//end;
//
//
//procedure inst_file(name: pchar;  data: pchar;  size: integer); 
//var
//fname: array [0..Pred(MAXPATHLEN)] of char; 
//fp: pFILE; 
//begin
//  
//  make_file_string(fname,gethome(),name); 
//  fp:= fopen(fname,'wb'); 
//  if fp=0{nil}
//  then
//  begin 
//    printf('Could not install %s\n',fname);
//    printf('Check permissions on %s.\n',gethome());
//    exit(1); 
//  end;
//  fwrite(data,size,1,fp); 
//  
//  fclose(fp); 
//end;
//
//
//function read_homefile: integer; 
//var
//file: integer; 
//ret: integer; 
//str: pchar; 
//home: pchar; 
//tstr: array [0..Pred(FILE_MAXDIR+FILE_MAXFILE)] of char; 
//scestr: array [0..Pred(FILE_MAXDIR)] of char; 
//begin
//  
//  
//  
//  
//  
//  home:= gethome(); 
//  if home<>0{nil} {<= !!!9} 
//  then
//  begin 
//    (* a vectorfont can't be read from memory... so has to be saved in $HOME *)
//    make_file_string(tstr,home,'.Bfont'); 
//    file:= open(tstr,O_BINARY or O_RDONLY); 
//    if file<0
//    then
//    inst_file('.Bfont',datatoc_Bfont,datatoc_Bfont_size); 
//    else
//    close(file); 
//    make_file_string(tstr,home,'.Bfs'); 
//    
//    file:= open(tstr,O_BINARY or O_RDONLY); 
//    if file<0
//    then
//    inst_file('.Bfs',datatoc_Bfs,datatoc_Bfs_size); 
//    else
//    close(file); 
//    make_file_string(tstr,home,'.B.blend'); 
//    
//    file:= open(tstr,O_BINARY or O_RDONLY); 
//    if file<0
//    then
//    begin 
//      inst_file('.B.blend',datatoc_B_blend,datatoc_B_blend_size); 
//      file:= open(tstr,O_BINARY or O_RDONLY); 
//    end;
//    if file>=0
//    then
//    begin 
//      (* bestand gevonden *)
//      close(file); 
//      strcpy(scestr,G.sce); 
//      read_file(tstr); 
//      (* even bewaren *)
//      strcpy(G.sce,scestr); 
//      G.save_over:= LongBool(0); 
//      disable_capslock(U.flag and NO_CAPSLOCK); 
//      (* nog wat afhandelen? *)
//      (* holobutton *)
//      if lstrcmp(G.scene.r.ftype,'*@&#')=0
//      then
//      G.special1:= G_HOLO; 
//      if U.tempdir[0]='/')and(U.tempdir[1]=0
//      then
//      begin 
//        (*     str= getenv("TEMP"); *)
//        (*     if(str) strcpy(U.tempdir, str); *)
//        (*     else strcpy(U.tempdir, "/tmp/"); *)
//      end;
//      begin
//        result:= 1; 
//        exit;
//      end;
//    end;
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//
//
//procedure read_autosavefile; 
//var
//file: integer; 
//tstr: array [0..Pred(FILE_MAXDIR)] of char; 
//scestr: array [0..Pred(FILE_MAXDIR)] of char; 
//tmp2: array [0..Pred(32)] of char; 
//save_over: integer; 
//begin
//  
//  
//  
//  
//  strcpy(scestr,G.sce); 
//  sprintf(tmp2,'%d.blend',abs(getpid())); 
//  (* even bewaren *)
//  make_file_string(tstr,U.tempdir,tmp2); 
//  save_over:= G.save_over; 
//  read_file(tstr); 
//  
//  G.save_over:= save_over; 
//  strcpy(G.sce,scestr); 
//end;
//
//
//(* ************* APPEND LIBRARY ************** *)
//
//bheadlib: pBHead; 
//
//
//function find_bhead(old: pinteger;  filedata: pchar): pBHead; 
//var
//bhead: pBHead; 
//afbreek: integer;
// 
//fd: pchar; 
//begin
//  
//  afbreek:=0; 
//  
//  bheadlib:= 0; 
//  if old=0
//  then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  fd:= filedata; 
//  while afbreek=0
//  do
//  begin 
//    bhead:= {!!!a type cast? =>} {pBHead(}fd; 
//    if bhead.code=ID_LI
//    then
//    begin 
//      bheadlib:= bhead; 
//    end;
//    if bhead.code=ENDB
//    then
//    afbreek:= 1; 
//    else
//    if bhead.old=old
//    then
//    begin
//      result:= bhead; 
//      exit;
//    end;
//    fd:= fd + (bhead.len+sizeof(BHead)); 
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//
//function is_yet_read(main: pMain;  bhead: pBHead): pID; 
//var
//lb: pListBase; 
//idtest: pID; 
//id: pID; 
//begin
//  
//  
//  
//  idtest:= {!!!a type cast? =>} {pID(}(bhead+1); 
//  lb:= wich_libbase(main,GS(idtest.name)); 
//  if lb<>0{nil} {<= !!!9} 
//  then
//  begin 
//    id:= lb.first; 
//    while id
//    do
//    begin 
//      if lstrcmp(id.name,idtest.name)=0
//      then
//      begin
//        result:= id; 
//        exit;
//      end;
//      id:= id.next; 
//    end;
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//
//procedure expand_doit(main: pMain;  filedata: pchar;  old: pinteger); 
//var
//bhead: pBHead; 
//lib: pLibrary; 
//id: pID; 
//begin
//  
//  
//  bhead:= find_bhead(old,filedata); 
//  if bhead<>0{nil} {<= !!!9} 
//  then
//  begin 
//    
//    (* andere library? *)
//    if bhead.code=ID_ID
//    then
//    begin 
//      if bheadlib<>0{nil} {<= !!!9} 
//      then
//      begin 
//        lib:= {!!!a type cast? =>} {pLibrary(}(bheadlib+1); 
//        main:= find_main(lib.name); 
//        id:= is_yet_read(main,bhead); 
//        if id=0
//        then
//        begin 
//          read_libblock(main,bhead,LIB_READ+LIB_INDIRECT); 
//          printf('expand: other lib %s\n',lib.name);
//        end;
//        else
//        begin 
//          printf('expand: already linked: %s lib: %s\n',id.name,lib.name);
//          add_lib_adr(bhead.old,id); 
//          (* if(id->lib==0) add_lib_adr(bhead->old, id); *)
//          (* bovenstaand is niet nodig! Kan toch niet misgaan? *)
//        end;
//      end;
//    end;
//    else
//    begin 
//      id:= is_yet_read(main,bhead); 
//      if id=0
//      then
//      begin 
//        id:= {!!!a type cast? =>} {pID(}(bhead+1); 
//        read_libblock(main,bhead,LIB_TESTIND); 
//      end;
//      else
//      begin 
//        (* printf("expand: al ingelezen %s\n", id->name); *)
//        add_lib_adr(bhead.old,id); 
//      end;
//    end;
//  end;
//end;
//
//
//procedure expand_key(main: pMain;  filedata: pchar;  key: pKey); 
//var
//a: integer; 
//begin
//  
//  expand_doit(main,filedata,key.ipo); 
//end;
//
//
//
//procedure expand_texture(main: pMain;  filedata: pchar;  tex: pTex); 
//var
//a: integer; 
//begin
//  
//  expand_doit(main,filedata,tex.ima); 
//end;
//
//
//procedure expand_material(main: pMain;  filedata: pchar;  ma: pMaterial); 
//var
//a: integer; 
//begin
//  
//  for{while} a:=0 to Pred(8) { a++}
//  do
//  begin 
//    if ma.mtex[a]<>0{nil} {<= !!!9} 
//    then
//    begin 
//      expand_doit(main,filedata,ma.mtex[a].tex); 
//      expand_doit(main,filedata,ma.mtex[a].object); 
//    end;
//  end;
//  expand_doit(main,filedata,ma.ipo); 
//end;
//
//
//procedure expand_lamp(main: pMain;  filedata: pchar;  la: pLamp); 
//var
//a: integer; 
//begin
//  
//  for{while} a:=0 to Pred(8) { a++}
//  do
//  begin 
//    if la.mtex[a]<>0{nil} {<= !!!9} 
//    then
//    begin 
//      expand_doit(main,filedata,la.mtex[a].tex); 
//      expand_doit(main,filedata,la.mtex[a].object); 
//    end;
//  end;
//  expand_doit(main,filedata,la.ipo); 
//end;
//
//
//procedure expand_lattice(main: pMain;  filedata: pchar;  lt: pLattice); 
//var
//a: integer; 
//begin
//  
//  expand_doit(main,filedata,lt.ipo); 
//  expand_doit(main,filedata,lt.key); 
//end;
//
//
//procedure expand_sector(main: pMain;  filedata: pchar;  se: pSector); 
//var
//a: integer; 
//begin
//  
//  expand_doit(main,filedata,se.dynamesh); 
//  expand_doit(main,filedata,se.texmesh); 
//end;
//
//
//procedure expand_life(main: pMain;  filedata: pchar;  lf: pLife); 
//var
//a: integer; 
//begin
//  
//  expand_doit(main,filedata,lf.dynamesh); 
//  expand_doit(main,filedata,lf.texmesh); 
//end;
//
//
//
//
//procedure expand_world(main: pMain;  filedata: pchar;  wrld: pWorld); 
//var
//a: integer; 
//begin
//  
//  for{while} a:=0 to Pred(8) { a++}
//  do
//  begin 
//    if wrld.mtex[a]<>0{nil} {<= !!!9} 
//    then
//    begin 
//      expand_doit(main,filedata,wrld.mtex[a].tex); 
//      expand_doit(main,filedata,wrld.mtex[a].object); 
//    end;
//  end;
//  expand_doit(main,filedata,wrld.ipo); 
//end;
//
//
//
//procedure expand_mball(main: pMain;  filedata: pchar;  mb: pMetaBall); 
//var
//a: integer; 
//begin
//  
//  for{while} a:=0 to Pred(mb.totcol) { a++}
//  do
//  begin 
//    expand_doit(main,filedata,mb.mat[a]); 
//  end;
//end;
//
//
//procedure expand_curve(main: pMain;  filedata: pchar;  cu: pCurve); 
//var
//a: integer; 
//begin
//  
//  for{while} a:=0 to Pred(cu.totcol) { a++}
//  do
//  begin 
//    expand_doit(main,filedata,cu.mat[a]); 
//  end;
//  expand_doit(main,filedata,cu.vfont); 
//  expand_doit(main,filedata,cu.key); 
//  expand_doit(main,filedata,cu.ipo); 
//  expand_doit(main,filedata,cu.bevobj); 
//  expand_doit(main,filedata,cu.textoncurve); 
//end;
//
//
//procedure expand_mesh(main: pMain;  filedata: pchar;  me: pMesh); 
//var
//a: integer; 
//tface: pTFace; 
//begin
//  
//  
//  for{while} a:=0 to Pred(me.totcol) { a++}
//  do
//  begin 
//    expand_doit(main,filedata,me.mat[a]); 
//  end;
//  expand_doit(main,filedata,me.key); 
//  expand_doit(main,filedata,me.texcomesh); 
//  if me.tface<>0{nil} {<= !!!9} 
//  then
//  begin 
//    tface:= me.tface; 
//    a:= me.totface; 
//    while a{--} dec(a); 
//    do
//    begin 
//      if tface.tpage<>0{nil} {<= !!!9} 
//      then
//      expand_doit(main,filedata,tface.tpage); 
//      inc(tface); 
//    end;
//  end;
//end;
//
//
//procedure expand_object(main: pMain;  filedata: pchar;  ob: pObject); 
//var
//a: integer; 
//begin
//  
//  expand_doit(main,filedata,ob.data); 
//  expand_doit(main,filedata,ob.ipo); 
//  for{while} a:=0 to Pred(ob.totcol) { a++}
//  do
//  begin 
//    expand_doit(main,filedata,ob.mat[a]); 
//  end;
//end;
//
//
//procedure expand_scene(main: pMain;  filedata: pchar;  sce: pScene); 
//var
//base: pBase; 
//begin
//  
//  base:= sce.base.first; 
//  while base
//  do
//  begin 
//    expand_doit(main,filedata,base.object); 
//    base:= base.next; 
//  end;
//  expand_doit(main,filedata,sce.camera); 
//  expand_doit(main,filedata,sce.world); 
//end;
//
//
//procedure expand_camera(main: pMain;  filedata: pchar;  ca: pCamera); 
//begin
//  expand_doit(main,filedata,ca.ipo); 
//  
//end;
//
//
//procedure expand_main(main: pMain;  filedata: pchar); 
//var
//lbarray: array [0..Pred(30)] of pListBase; 
//id: pID; 
//a: integer; 
//doit: integer;
// 
//begin
//  
//  
//  
//  doit:=1; 
//  if filedata=0
//  then
//  exit;
//  while doit
//  do
//  begin 
//    doit:= 0; 
//    a:= set_listbasepointers(main,lbarray); 
//    while a{--} dec(a); 
//    do
//    begin 
//      id:= lbarray[a].first; 
//      while id
//      do
//      begin 
//        if id.flag and LIB_TEST
//        then
//        begin 
//          case GS(id.name) of
//            ID_OB:
//            begin
//              expand_object(main,filedata,{!!!a type cast? =>} {pObject(}id); 
//            end;
//            ID_ME:
//            begin
//              expand_mesh(main,filedata,{!!!a type cast? =>} {pMesh(}id); 
//            end;
//            ID_CU:
//            begin
//              expand_curve(main,filedata,{!!!a type cast? =>} {pCurve(}id); 
//            end;
//            ID_MB:
//            begin
//              expand_mball(main,filedata,{!!!a type cast? =>} {pMetaBall(}id); 
//            end;
//            ID_SCE:
//            begin
//              expand_scene(main,filedata,{!!!a type cast? =>} {pScene(}id); 
//            end;
//            ID_MA:
//            begin
//              expand_material(main,filedata,{!!!a type cast? =>} {pMaterial(}id); 
//            end;
//            ID_TE:
//            begin
//              expand_texture(main,filedata,{!!!a type cast? =>} {pTex(}id); 
//            end;
//            ID_WO:
//            begin
//              expand_world(main,filedata,{!!!a type cast? =>} {pWorld(}id); 
//            end;
//            ID_LT:
//            begin
//              expand_lattice(main,filedata,{!!!a type cast? =>} {pLattice(}id); 
//            end;
//            ID_LF:
//            begin
//              expand_life(main,filedata,{!!!a type cast? =>} {pLife(}id); 
//            end;
//            ID_SE:
//            begin
//              expand_sector(main,filedata,{!!!a type cast? =>} {pSector(}id); 
//            end;
//            ID_LA:
//            begin
//              expand_lamp(main,filedata,{!!!a type cast? =>} {pLamp(}id); 
//            end;
//            ID_KE:
//            begin
//              expand_key(main,filedata,{!!!a type cast? =>} {pKey(}id); 
//              {goto next_label;}{<= !!!d case label without "break"}
//            end;
//            ID_CA:
//            begin
//              expand_camera(main,filedata,{!!!a type cast? =>} {pCamera(}id); 
//            end;
//            
//          end;{case?}
//          
//          doit:= 1; 
//          id.flag:= id.flag - (LIB_TEST); 
//        end;
//        id:= id.next; 
//      end;
//    end;
//  end;
//end;
//
//
//procedure give_base_to_objects(sce: pScene;  lb: pListBase); 
//var
//ob: pObject; 
//base: pBase; 
//ok: integer; (* alle objects die LIB_EXTERN en LIB_NEEDLINK zijn, een base geven *)
//begin
//  
//  
//  ob:= lb.first; 
//  while ob
//  do
//  begin 
//    
//    if ob.id.us=0
//    then
//    begin 
//      
//      if ob.id.flag and LIB_NEEDLINK
//      then
//      begin 
//        ob.id.flag:= ob.id.flag - (LIB_NEEDLINK); 
//        if ob.id.flag and LIB_INDIRECT
//        then
//        begin 
//          base:= callocN(sizeof(Base),'add_ext_base'); 
//          addtail(@(sce.base),base); 
//          base.lay:= ob.lay; 
//          base.object:= ob; 
//          ob.id.us:= 1; 
//          ob.id.flag:= ob.id.flag - (LIB_INDIRECT); 
//          ob.id.flag:= ob.id.flag or (LIB_EXTERN); 
//          
//        end;
//      end;
//    end;
//    ob:= ob.id.next; 
//  end;
//end;
//
//
//
//procedure append_named_part(sfile: pSpaceFile;  main: pMain;  name: pchar;  idcode: integer); 
//var
//ob: pObject; 
//base: pBase; 
//bhead: pBHead; 
//id: pID; 
//afbreek: integer;
// 
//fd: pchar; 
//begin
//  
//  
//  
//  
//  afbreek:=0; 
//  
//  fd:= sfile.libfiledata; 
//  while afbreek=0
//  do
//  begin 
//    bhead:= {!!!a type cast? =>} {pBHead(}fd; 
//    if bhead.code=ENDB
//    then
//    afbreek:= 1; 
//    else
//    if bhead.code=idcode
//    then
//    begin 
//      id:= {!!!a type cast? =>} {pID(}(bhead+1); 
//      if lstrcmp(id.name+2,name)=0
//      then
//      begin 
//        id:= is_yet_read(main,bhead); 
//        if id=0
//        then
//        begin 
//          read_libblock(main,bhead,LIB_TESTEXT); 
//        end;
//        else
//        begin 
//          printf('append: already linked\n');
//          add_lib_adr(bhead.old,id); 
//          if id.flag and LIB_INDIRECT
//          then
//          begin 
//            id.flag:= id.flag - (LIB_INDIRECT); 
//            id.flag:= id.flag or (LIB_EXTERN); 
//          end;
//        end;
//        if idcode=ID_OB
//        then
//        begin 
//          (* los object: base geven *)
//          base:= callocN(sizeof(Base),'app_nam_part'); 
//          addtail(@(G.scene.base),base); 
//          if id=0
//          then
//          ob:= main.object.last; 
//          else
//          ob:= {!!!a type cast? =>} {pObject(}id; 
//          base.lay:= ob.lay; 
//          base.object:= ob; 
//          inc(ob.id.us); 
//        end;
//        afbreek:= 1; 
//      end;
//    end;
//    fd:= fd + (bhead.len+sizeof(BHead)); 
//  end;
//end;
//
//
//procedure append_id_part(filedata: pchar;  main: pMain;  id: pID); 
//var
//bhead: pBHead; 
//idread: pID; 
//afbreek: integer;
// 
//fd: pchar; 
//begin
//  
//  
//  afbreek:=0; 
//  
//  fd:= filedata; 
//  while afbreek=0
//  do
//  begin 
//    bhead:= {!!!a type cast? =>} {pBHead(}fd; 
//    if bhead.code=ENDB
//    then
//    afbreek:= 1; 
//    else
//    if bhead.code=GS(id.name)
//    then
//    begin 
//      idread:= {!!!a type cast? =>} {pID(}(bhead+1); 
//      if lstrcmp(id.name,idread.name)=0
//      then
//      begin 
//        id.flag:= id.flag - (LIB_READ); 
//        id.flag:= id.flag or (LIB_TEST); 
//        read_libblock(main,bhead,id.flag); 
//        afbreek:= 1; 
//      end;
//    end;
//    fd:= fd + (bhead.len+sizeof(BHead)); 
//  end;
//end;
//
//
//(* append aan G.scene *)
//
//procedure library_append(sfile: pSpaceFile); 
//var
//main: pMain; 
//vf: pVFont; 
//ob: pObject; 
//cu: pCurve; 
//a: integer; 
//totsel: integer;
// 
//idcode: integer; 
//dir: array [0..Pred(FILE_MAXDIR)] of char; 
//group: array [0..Pred(32)] of char; (* is er sprake van een library? *)
//begin
//  
//  
//  
//  
//  
//  totsel:=0; 
//  
//  
//  if is_a_library(sfile,dir,group)=0
//  then
//  begin 
//    error('Not a library'); 
//    exit;
//  end;
//  if sfile.libfiledata=0
//  then
//  begin 
//    error('library not loaded'); 
//    exit;
//  end;
//  if group[0]=0
//  then
//  begin 
//    error('Nothing indicated'); 
//    exit;
//  end;
//  if lstrcmp(G.main.name,dir)=0
//  then
//  begin 
//    error('Cannot use current file as library'); 
//    exit;
//  end;
//  (* zijn er geselecteerde files? *)
//  for{while} a:=0 to Pred(sfile.totfile) { a++}
//  do
//  begin 
//    if sfile.filelist[a].flags and ACTIVE
//    then
//    begin 
//      inc(totsel); 
//    end;
//  end;
//  if totsel=0
//  then
//  begin 
//    (* is de aangegeven file in de filelist? *)
//    if sfile.file[0]<>0{nil} {<= !!!9} 
//    then
//    begin 
//      for{while} a:=0 to Pred(sfile.totfile) { a++}
//      do
//      begin 
//        if lstrcmp(sfile.filelist[a].relname,sfile.file)=0
//        then
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      if a=sfile.totfile
//      then
//      begin 
//        error('Wrong indicated name'); 
//        exit;
//      end;
//    end;
//    else
//    begin 
//      error('Nothing indicated'); 
//      exit;
//    end;
//  end;
//  split_main(); 
//  (* nu hebben OF geselecteerde, OF 1 aangegeven file *)
//  (* mains maken *)
//  main:= find_main(dir); 
//  idcode:= groupname_to_code(group); (* welke moeten wij hebben? *)
//  
//  if totsel=0
//  then
//  begin 
//    append_named_part(sfile,main,sfile.file,idcode); 
//  end;
//  else
//  begin 
//    for{while} a:=0 to Pred(sfile.totfile) { a++}
//    do
//    begin 
//      if sfile.filelist[a].flags and ACTIVE
//      then
//      begin 
//        append_named_part(sfile,main,sfile.filelist[a].relname,idcode); 
//      end;
//    end;
//  end;
//  expand_main(main,sfile.libfiledata); 
//  (* de main consistent maken *)
//  read_libraries(); 
//  (* als expand nog andere libs gevonden heeft: *)
//  lib_link_all(G.main); 
//  vf:= G.main.vfont.first; 
//  (* losse objects aan G.scene hangen deze hebben nog een linkflag
//      moet na lib_link ivm gelinkte scenes (ob->us==0) *)
//  
//  (* indirecte objects kunnen geen kwaad *)
//  (* als je deze terugzet, denk aan de 'need_link' flag: doe een find naar 'give_base_to' *)
//  (* give_base_to_objects(G.scene, &(G.main->object)); *)
//  
//  
//  
//  (* VECTORFONTS *)
//  while vf
//  do
//  begin 
//    if vf.id.lib)and(vf.flag=1
//    then
//    reload_vfont(vf); 
//    vf:= vf.id.next; 
//  end;
//  ob:= G.main.object.first; 
//  set_displist_onlyzero(1); 
//  (* DISPLISTEN *)
//  while ob
//  do
//  begin 
//    if ob.id.lib<>0{nil} {<= !!!9} 
//    then
//    begin 
//      if ob.type=OB_FONT
//      then
//      begin 
//        cu:= ob.data; 
//        if cu.nurb.first=0
//        then
//        text_to_curve(ob,0); 
//      end;
//      makeDispList(ob); 
//    end;
//    else
//    if ob.type=OB_MESH)and(ob.parent)and(ob.parent.type=OB_LATTICE
//    then
//    makeDispList(ob); 
//    ob:= ob.id.next; 
//  end;
//  set_displist_onlyzero(0); 
//  add_data_adr(0,0); 
//  
//  (* losslingerende blokken vrijgeven *)
//  add_lib_adr(0,0); 
//  strcpy(G.lib,sfile.dir); 
//  (* in sfile->dir staat de HELE libnaam *)
//  if TEST_C_KEY<>0{nil} {<= !!!9} 
//  then
//  begin 
//    if (sfile.flag and FILE_LINK)=0
//    then
//    all_local(); 
//  end;
//  else
//  begin 
//    if sfile.flag and FILE_LINK
//    then
//    error('Linking not suported, data is appended'); 
//    all_local(); 
//  end;
//  
//  (* voorlopige patch om te voorkomen dat de switch_endian 2x gebeurt *)
//  if switch_endian<>0{nil} {<= !!!9} 
//  then
//  begin 
//    if sfile.libfiledata<>0{nil} {<= !!!9} 
//    then
//    begin 
//      freeN(sfile.libfiledata); 
//      sfile.libfiledata:= 0; 
//    end;
//  end;
//end;
//
//(* ************* READ LIBRARY ************** *)
//
//
//procedure read_libraries; 
//var
//main: pMain; 
//lib: pLibrary; 
//id: pID; 
//idn: pID; 
//lbarray: array [0..Pred(30)] of pListBase; 
//a: integer; 
//doit: integer;
// 
//tot: integer; 
//filelen: integer; 
//filedata: pchar; 
//begin
//  
//  
//  
//  
//  
//  
//  doit:=1; 
//  
//  
//  
//  while doit
//  do
//  begin 
//    doit:= 0; 
//    main:= G.main.next; (* test 1: inlezen libdata *)
//    while main
//    do
//    begin 
//      
//      disable_newlibadr:= (void* )1; 
//      tot:= count_mainblocks_flag(main,LIB_READ); 
//      if tot<>0{nil} {<= !!!9} 
//      then
//      begin 
//        filedata:= main.curlib.filedata; 
//        if filedata=0
//        then
//        begin 
//          printf('read lib %s\n',main.curlib.name);
//          filedata:= openblenderfile(main.curlib.name, and filelen); 
//          main.curlib.filedata:= filedata; 
//        end;
//        if filedata<>0{nil} {<= !!!9} 
//        then
//        begin 
//          doit:= 1; 
//          a:= set_listbasepointers(main,lbarray); 
//          while a{--} dec(a); 
//          do
//          begin 
//            id:= lbarray[a].first; 
//            while id
//            do
//            begin 
//              idn:= id.next; 
//              if id.flag and LIB_READ
//              then
//              begin 
//                remlink(lbarray[a],id); 
//                append_id_part(filedata,main,id); 
//                (* printf("change adr: %s %x\n", id->name, disable_newlibadr); *)
//                if disable_newlibadr=(void* )1
//                then
//                begin 
//                  printf('LIB ERROR: can'#39't find %s\n',id.name);
//                  change_libadr(id,0); 
//                end;
//                else
//                change_libadr(id,disable_newlibadr); 
//                freeN(id); 
//              end;
//              id:= idn; 
//            end;
//          end;
//        end;
//        else
//        printf('ERROR: can'#39't find lib %s \n',main.curlib.name);
//      end;
//      disable_newlibadr:= 0; 
//      expand_main(main,main.curlib.filedata); 
//      
//      main:= main.next; 
//    end;
//  end;
//  main:= G.main.next; 
//  while main
//  do
//  begin 
//    if main.curlib.filedata<>0{nil} {<= !!!9} 
//    then
//    freeN(main.curlib.filedata); 
//    main.curlib.filedata:= 0; 
//    a:= set_listbasepointers(main,lbarray); (* test of er libblocken niet zijn gelezen *)
//    while a{--} dec(a); 
//    do
//    begin 
//      id:= lbarray[a].first; 
//      while id
//      do
//      begin 
//        idn:= id.next; 
//        if id.flag and LIB_READ
//        then
//        begin 
//          remlink(lbarray[a],id); 
//          printf('LIB ERROR: can'#39't find %s\n',id.name);
//          change_libadr(id,0); 
//          freeN(id); 
//        end;
//        id:= idn; 
//      end;
//    end;
//    (* sommige mains moeten nog worden ingelezen, dan is versionfile nog nul! *)
//    if main.versionfile<>0{nil} {<= !!!9} 
//    then
//    do_versions(main); 
//    main:= main.next; 
//    
//  end;
//  join_main(); 
//end;

end.
