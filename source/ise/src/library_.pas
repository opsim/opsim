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
(*  library.c   aug 94     MIXED MODEL
 * 
 *  jan 95
 * 
 *  afhandeling ID's en libraries
 *  allocceren en vrijgeven alle library data
 * 
 *)
unit library_;

interface

uses
util;

function alloc_libblock(lb: pListBase;  type_: smallint;  name: pchar): pointer;
procedure free_libblock(lb: pListBase;  idv: pointer);

implementation

uses
blender, blenglob,
blendef,
screen, space,
cfuncs;

const
  MAX_IDPUP = 24;

(* ************* ALGEMEEN ************************ *)

//procedure id_lib_extern(id: pID);
//begin
//  if id<>nil then
//  begin 
//    if id.flag and LIB_INDIRECT then
//    begin 
//      id.flag:= id.flag - (LIB_INDIRECT); 
//      id.flag:= id.flag or (LIB_EXTERN); 
//    end;
//  end;
//end;

//procedure id_us_plus(id: pID); 
//begin
//  if id<>nil then
//  begin 
//    inc(id.us); 
//    if id.flag and LIB_INDIRECT then
//    begin 
//      id.flag:= id.flag - (LIB_INDIRECT); 
//      id.flag:= id.flag or (LIB_EXTERN); 
//    end;
//  end;
//end;

function wich_libbase(main: pMain;  type_: smallint): pListBase;
begin
  case type_ of
    //ID_SCE: exit(@main^.scene);
    //ID_LI: exit(@main^.library);
    //ID_OB: exit(@main^.object);
    //ID_ME: exit(@main^.mesh);
    //ID_CU: exit(@main^.curve);
    //ID_MB: exit(@main^.mball);
    //ID_MA: exit(@main^.mat);
    //ID_TE: exit(@main^.tex);
    //ID_IM: exit(@main^.image);
    //ID_IK: exit(@main^.ika);
    //ID_WV: exit(@main^.wave);
    //ID_LT: exit(@main^.latt);
    //ID_SE: exit(@main^.sector);
    //ID_LF: exit(@main^.life);
    //ID_LA: exit(@main^.lamp);
    //ID_CA: exit(@main^.camera);
    //ID_IP: exit(@main^.ipo);
    //ID_KE: exit(@main^.key);
    //ID_WO: exit(@main^.world);
    ID_SCR: exit(@main^.screen);
    //ID_VF: exit(@main^.vfont);
    //ID_TXT: exit(@main^.text);
  end;

    exit(nil);
end;

//function set_listbasepointers(main: pMain;  {!!!3 unknown typedef}): integer; 
//begin
//  lb[0]:= @(main.ipo); 
//  lb[1]:= @(main.key); 
//  lb[2]:= @(main.image); 
//  lb[3]:= @(main.tex); 
//  lb[4]:= @(main.mat); 
//  lb[5]:= @(main.vfont); 
//  lb[6]:= @(main.mesh); 
//  lb[7]:= @(main.curve); 
//  lb[8]:= @(main.mball); 
//  lb[9]:= @(main.ika); 
//  lb[10]:= @(main.wave); 
//  lb[11]:= @(main.latt); 
//  lb[12]:= @(main.sector); 
//  lb[13]:= @(main.life); 
//  lb[14]:= @(main.lamp); 
//  lb[15]:= @(main.camera); 
//  lb[16]:= @(main.world); 
//  lb[17]:= @(main.screen); 
//  lb[18]:= @(main.object); 
//  lb[19]:= @(main.scene); 
//  lb[20]:= @(main.library); 
//  lb[21]:= @(main.text); (* BACKWARDS! let op volgorde van vrijgeven! (mesh<->mat) *)
//  begin
//    result:= 22; 
//    exit;
//  end;
//end;

(* *********** ALLOC EN FREE *****************

//free_libblock(ListBase *lb, ID *id )
// lijstbasis en datablok geven, alleen ID wordt uitgelezen
//void free_liblist(ListBase *lb)
//void *alloc_libblock(ListBase *lb, type, name)
// hangt in lijst en geeft nieuw ID
// ***************************** *)

function new_id(lb: pListBase;  id: pID;  tname: pchar): integer; forward;

function alloc_libblock_notest(type_: smallint): pID;
var
id: pID = nil;
begin
  case type_ of
    ID_SCE:
      id:= callocN(sizeof(Scene),'scene');
//    ID_LI:
//      id:= callocN(sizeof(Library),'library');
//    ID_OB:
//      id:= callocN(sizeof(Object),'object');
//    ID_ME:
//      id:= callocN(sizeof(Mesh),'mesh');
//    ID_CU:
//      id:= callocN(sizeof(Curve),'curve');
//    ID_MB:
//      id:= callocN(sizeof(MetaBall),'mball');
//    ID_MA:
//      id:= callocN(sizeof(Material),'mat');
//    ID_TE:
//      id:= callocN(sizeof(Tex),'tex');
//    ID_IM:
//      id:= callocN(sizeof(Image),'image');
//    ID_IK:
//      id:= callocN(sizeof(Ika),'ika');
//    ID_WV:
//      id:= callocN(sizeof(Wave),'wave');
//    ID_LT:
//      id:= callocN(sizeof(Lattice),'latt');
//    ID_SE:
//      id:= callocN(sizeof(Sector),'sector');
//    ID_LF:
//      id:= callocN(sizeof(Life),'life');
//    ID_LA:
//      id:= callocN(sizeof(Lamp),'lamp');
//    ID_CA:
//      id:= callocN(sizeof(Camera),'camera');
//    ID_IP:
//      id:= callocN(sizeof(Ipo),'ipo');
//    ID_KE:
//      id:= callocN(sizeof(Key),'key');
//    ID_WO:
//      id:= callocN(sizeof(World),'world');
    ID_SCR:
      id:= callocN(sizeof(bScreen), 'screen');
//    ID_VF:
//      id:= callocN(sizeof(VFont),'vfont');
//    ID_TXT:
//      id:= callocN(sizeof(Text),'text');
  end;

  exit(id);
end;

function alloc_libblock(lb: pListBase;  type_: smallint;  name: pchar): pointer;
var
id: pID = nil;
idtest: pID;
begin
  id:= alloc_libblock_notest(type_);
  if id<>nil then
  begin
    addtail(lb,id);
    id^.us:= 1;
   psmallint(@id^.name[0])^:=type_;
    new_id(lb,id,name);

    (* alfabetisch opnieuw invoegen: zit in new_id *)
  end;

  exit(id);
end;

//procedure copy_libblock(rt: pinteger); 
//var
//memh: pMemHead; 
//idn: pID; 
//id: pID; 
//lb: pListBase; 
//cp: pchar; 
//cpn: pchar; 
//begin

//  id:= rt; 
//  lb:= wich_libbase(G.main,GS(id.name)); 
//  idn:= alloc_libblock(lb,GS(id.name),id.name+2); 
//  memh:=  {pMemHead(}idn;
//  dec(memh); 
//  if memh.len-sizeof(ID)>0 then
//  begin 
//    cp:=  {pchar(}id;
//    cpn:=  {pchar(}idn;
//    memcpy(cpn+sizeof(ID),cp+sizeof(ID),memh.len-sizeof(ID)); 
//  end;
//  id._new:= idn; 
//  idn.flag:= idn.flag or (LIB_NEW); 
//  begin
//    result:= idn; 
//    exit;
//  end;
//end;

//procedure free_library(lib: pLibrary); 
//begin
//end;

procedure free_libblock(lb: pListBase;  idv: pointer);
var
id: pID;
begin
  id:=idv;
  case GS(id^.name) of    (* GetShort uit util.h *)
//    ID_SCE:
//      free_scene( {pScene(}id);
//    ID_LI:
//      free_library( {pLibrary(}id);
//    ID_OB:
//      free_object( {pObject(}id);
//    ID_ME:
//      free_mesh( {pMesh(}id);
//    ID_CU:
//      free_curve( {pCurve(}id);
//    ID_MB:
//      free_mball( {pMetaBall(}id);
//    ID_MA:
//      free_material( {pMaterial(}id);
//    ID_TE:
//      free_texture( {pTex(}id);
//    ID_IM:
//      free_image( {pImage(}id);
//    ID_IK:
//      free_ika( {pIka(}id);
//    ID_WV:
//    (* free_wave(id); *)
//    begin
//    end;
//    ID_LT:
//      free_lattice( {pLattice(}id);
//    ID_SE:
//      free_sector( {pSector(}id);
//    ID_LF:
//      free_life( {pLife(}id);
//    ID_LA:
//      free_lamp( {pLamp(}id);
//    ID_CA:
//      free_camera(id);
//    ID_IP:
//      free_ipo( {pIpo(}id);
//    ID_KE:
//      free_key( {pKey(}id);
//    ID_WO:
//      free_world( {pWorld(}id);
    ID_SCR:
      free_screen(pbScreen(id));
//    ID_VF:
//      free_vfont( {pVFont(}id);
//    ID_TXT:
//      free_text( {pText(}id);
  end;
  remlink(lb,id);
  freeN(id);
  allspace(OOPS_TEST,0);
end;
(* test users *)

//procedure free_libblock_us(lb: pListBase;  idv: pinteger); 
//var
//id: pID;

//begin
//  id:=idv; 
//  dec(id.us); 
//  if id.us<0 then
//  begin 
//    if id.lib<>nil then
//    printf('ERROR block %s %s users %d\n',id.lib.name,id.name,id.us); 
//    else
//    printf('ERROR block %s users %d\n',id.name,id.us); 
//  end;
//  if id.us=0 then
//  begin 
//    if GS(id.name)=ID_OB then
//    free_object_fromscene( {pObject(}id);
//    free_libblock(lb,id); 
//  end;
//end;

//procedure free_liblist(lb: pListBase); 
//var
//id: pID; 
//nid: pID; 
//begin

//  id:= lb.first; 
//  while id
//  do
//  begin 
//    nid:= id.next; 
//    free_libblock(lb,id); 
//    id:= nid; 
//  end;
//end;

//procedure free_main(lb: pListBase;  main: pMain); 
//var
//lbarray: array [0..Pred(30)] of pListBase; 
//a: integer; 
//begin(* ook aanroepen bij file inlezen, erase all, etc *)

//  a:= set_listbasepointers(main,lbarray); 
//  while a{--} dec(a); 
//  do
//  begin 
//    free_liblist(lbarray[a]); 
//  end;
//  if lb<>nil then
//  begin 
//    remlink(lb,main); 
//    freeN(main); 
//    if G.main=main then
//    G.main:= 0; 
//  end;
//  free_matcopybuf(); 
//  (* extra: copybuf vrijgeven *)
//  R.backbuf:= 0; 
//  G.scene:= 0; 
//  (* hangende vars voorkomen *)
//end;
//type
//Main = record
//end;

//procedure free_mainlist; 
//begin

//  main:= G.mainbase.first; 
//  while main
//  do
//  begin 
//    nmain:= main.next; 
//    free_main(@G.mainbase,main); 
//    main:= nmain; 
//  end;
//end;

//procedure add_main_to_main(main: pMain;  from: pMain); 
//var
//lbarray: array [0..Pred(30)] of pListBase; 
//fromarray: array [0..Pred(30)] of pListBase; 
//a: integer; 
//begin

//  a:= set_listbasepointers(main,lbarray); 
//  a:= set_listbasepointers(from,fromarray); 
//  while a{--} dec(a); 
//  do
//  begin 
//    addlisttolist(lbarray[a],fromarray[a]); 
//  end;
//end;

//procedure join_main; 
//var
//main: pMain; 
//nmain: pMain; 
//begin

//  main:= G.main^.next;
//  while main
//  do
//  begin 
//    nmain:= main.next; 
//    add_main_to_main(G.main,main); 
//    remlink(@G.mainbase,main); 
//    freeN(main); 
//    main:= nmain; 
//  end;
//end;

//procedure split_libdata(lb: pListBase;  first: pMain); 
//var
//lbn: pListBase; 
//id: pID; 
//idnext: pID; 
//main: pMain; 
//begin

//  id:= lb.first; 
//  while id
//  do
//  begin 
//    idnext:= id.next; 
//    if id.lib<>nil then
//    begin 
//      main:= first; 
//      while main
//      do
//      begin 
//        if main.curlib=id.lib then
//        begin 
//          lbn:= wich_libbase(main,GS(id.name)); 
//          remlink(lb,id); 
//          addtail(lbn,id); 
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        main:= main.next; 
//      end;
//      if main=0 then
//      printf('error split_libdata\n'); 
//    end;
//    id:= idnext; 
//  end;
//end;

//procedure split_main; 
//var
//lbarray: array [0..Pred(30)] of pListBase; 
//main: pMain; 
//next: pMain; 
//lib: pLibrary; 
//a: integer; 
//begin

//  if G.mainbase.first<>G.mainbase.last then
//  begin 
//    printf('error split main\n'); 
//    exit;
//  end;
//  lib:= G.main^.library.first;
//  if lib<>nil then
//  begin 
//    while lib
//    do
//    begin 
//      main:= callocN(sizeof(Main),'newmain'); 
//      addtail(@G.mainbase,main); 
//      main.curlib:= lib; 
//      lib:= lib.id.next; 
//    end;
//    next:= G.main^.next;
//    a:= set_listbasepointers(G.main,lbarray); 
//    while a{--} dec(a); 
//    do
//    begin 
//      split_libdata(lbarray[a],next); 
//    end;
//  end;
//end;

//function find_main(dir: pchar): pMain; 
//var
//main: pMain; 
//lib: pLibrary; 
//begin

//  main:= G.mainbase.first; 
//  while main
//  do
//  begin 
//    if main.curlib<>nil then
//    begin 
//      if lstrcmp(dir,main.curlib.name)=0 then
//      begin
//        result:= main; 
//        exit;
//      end;
//    end;
//    else
//    if lstrcmp(dir,main.name)=0 then
//    begin
//      result:= main; 
//      exit;
//    end;
//    main:= main.next; 
//  end;
//  main:= callocN(sizeof(Main),'findmain'); 
//  addtail(@G.mainbase,main); 
//  lib:= alloc_libblock(@(main.library),ID_LI,'lib'); 
//  strcpy(lib.name,dir); 
//  main.curlib:= lib; 
//  begin
//    result:= main; 
//    exit;
//  end;
//end;

//function count_mainblocks_flag(main: pMain;  flag: integer): integer; 
//var
//lbarray: array [0..Pred(30)] of pListBase; 
//id: pID; 
//a: integer; 
//tot: integer;

//begin

//  tot:=0; 
//  a:= set_listbasepointers(main,lbarray); 
//  while a{--} dec(a); 
//  do
//  begin 
//    id:= lbarray[a].first; 
//    while id
//    do
//    begin 
//      if id.flag and flag then
//      inc(tot); 
//      id:= id.next; 
//    end;
//  end;
//  begin
//    result:= tot; 
//    exit;
//  end;
//end;
//(* ***************** ID ************************ *)
//(* type: "OB" of "MA" etc *)

//function find_id(type: pchar;  name: pchar): pID; 
//var
//id: pID; 
//lb: pListBase; 
//begin

//  lb:= wich_libbase(G.main,GS(type)); 
//  id:= lb.first; 
//  while id
//  do
//  begin 
//    if lstrcmp(id.name+2,name)=0 then
//    begin
//      result:= id; 
//      exit;
//    end;
//    id:= id.next; 
//  end;
//    exit(0);
//end;

//procedure IDnames_to_pupstring_nr{!!!3 unknown typedef}; 
//var
//id: pID; 
//fake: integer; 
//len: integer;

//count: integer; 
//extra: array [0..Pred(8)] of char; 
//begin(* string naderhand vrijgeven *)

//  len:=50; 

//  {*}nr^:=-1; 
//  extra[3]:= 0; 
//  id:= lb.first; 
//  count:= 0; (* sym[0]= 171; sym[1]= 0; *)
//  (* lengte berekenen, doe er 30 bij voor evt extra item *)
//  while id
//  do
//  begin 
//    len:= len + (5+lstrlen(id.name)); 
//    inc(count); 
//    if count>maxpup then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    id:= id.next; 
//  end;
//  if count>maxpup then
//  begin 
//    {*}str^:=mallocN(40,'IDnames_pup'); 
//    strcpy({*}str^,'DataBrowse %x-2'); 
//    exit;
//  end;
//  {*}str^:=mallocN(len,'IDnames_pup'); 
//  (*str)[0]:=0; 
//  if link=0 then
//  strcpy({*}str^,'  %t|'); 
//  id:= lb.first; 
//  count:= 0; 
//  while id
//  do
//  begin 
//    if id=link then
//    {*}nr^:=count; 
//    fake:= id.flag and LIB_FAKEUSER; 
//    if id.lib)and(id.us=0 then
//    strcat({*}str^,'L0 '); 
//    else
//    if id.lib)and(fake then
//    strcat({*}str^,'LF '); 
//    else
//    if id.lib<>nil then
//    strcat({*}str^,'L   '); 
//    else
//    if fake<>nil then
//    strcat({*}str^,'  F '); 
//    else
//    if id.us=0 then
//    strcat({*}str^,'  0 '); 
//    else
//    if id.us<0 then
//    strcat({*}str^,'-1W '); 
//    else
//    strcat({*}str^,'     '); 
//    strcat({*}str^,id.name+2); 
//    if id.next<>nil then
//    strcat({*}str^,'|'); 
//    inc(count); 
//    if count>45 then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    id:= id.next; 
//  end;
//  len:= lstrlen(*str); (* verwijderen laatste OR teken *)
//  if len)and((*str)[len-1]='|' then
//  (*str)[len-1]:=0; 
//end;

//procedure IDnames_to_pupstring{!!!3 unknown typedef}; 
//begin
//  IDnames_to_pupstring_nr(str,lb,link,nr,MAX_IDPUP); 
//end;

//procedure IPOnames_to_pupstring{!!!3 unknown typedef}; 
//var
//id: pID; 
//ipo: pIpo; 
//fake: integer; 
//len: integer;

//count: integer; 
//extra: array [0..Pred(8)] of char; 
//begin(* string naderhand vrijgeven *)

//  len:=30; 

//  {*}nr^:=-1; 
//  extra[5]:= 0; 
//  id:= lb.first; 
//  count:= 0; (* lengte berekenen, doe er 30 bij voor evt extra item *)
//  while id
//  do
//  begin 
//    ipo:=  {pIpo(}id;
//    if blocktype=ipo.blocktype then
//    begin 
//      len:= len + (5+lstrlen(id.name)); 
//      inc(count); 
//      if count>MAX_IDPUP then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    id:= id.next; 
//  end;
//  if count>MAX_IDPUP then
//  begin 
//    {*}str^:=mallocN(40,'IPOnames_pup'); 
//    strcpy({*}str^,'DataBrowse %x-2'); 
//    exit;
//  end;
//  {*}str^:=mallocN(len,'IPOnames_pup'); 
//  (*str)[0]:=0; 
//  if link=0 then
//  strcpy({*}str^,'  %t|'); 
//  id:= lb.first; 
//  count:= 0; 
//  while id
//  do
//  begin 
//    ipo:=  {pIpo(}id;
//    if blocktype=ipo.blocktype then
//    begin 
//      if id=link then
//      {*}nr^:=count; 
//      fake:= id.flag and LIB_FAKEUSER; 
//      if id.lib)and(id.us=0 then
//      strcat({*}str^,'L0 '); 
//      else
//      if id.lib)and(fake then
//      strcat({*}str^,'L F '); 
//      else
//      if id.lib<>nil then
//      strcat({*}str^,'L   '); 
//      else
//      if fake<>nil then
//      strcat({*}str^,'  F '); 
//      else
//      if id.us=0 then
//      strcat({*}str^,'  0 '); 
//      else
//      if id.us<0 then
//      strcat({*}str^,'-1W '); 
//      else
//      strcat({*}str^,'     '); 
//      strcat({*}str^,id.name+2); 
//      if id.next<>nil then
//      strcat({*}str^,'|'); 
//      inc(count); 
//    end;
//    id:= id.next; 
//  end;
//  len:= lstrlen(*str); (* verwijderen laatste OR teken *)
//  if len)and((*str)[len-1]='|' then
//  (*str)[len-1]:=0; 
//end;

//function has_id_number(id: pID): integer; 
//var
//a: integer; 
//len: integer; 
//name: pchar; 
//begin

//  name:= id.name+2; 
//  len:= lstrlen(name); 
//  if len<2 then
//    exit(0);
//  if name[len-1]='.' then
//    exit(0);
//  while dec(len); {--}len
//  do
//  begin 
//    if name[len]='.' then
//      exit(1);
//    if isdigit(name[len])=0 then
//      exit(0);
//  end;
//    exit(0);
//end;

procedure splitIDname(name: pchar;  left: pchar;  nr: pinteger);
var
len: integer;
a: integer;
begin

  {*}nr^:=0;
  strncpy(left,name,21);

  len:=lstrlen(name);
  a:=len;
  if (a>1)and(name[a-1]='.') then
  exit;

  while a<>0  do
  begin
    if name[a]='.' then
    begin
      left[a]:= #0; {*}nr^:=atol(name+a+1);
      exit;
    end;
    if not isdigit(name[a]) then
    break; {<= !!!b possible in "switch" - then remove this line}

    left[a]:= #0;

    dec(a);
  end;

  strcpy(left,name);
end;

procedure sort_alpha_id(lb: pListBase;  id: pID);
var
idtest: pID; (* alfabetisch opnieuw invoegen *)
begin
  if lb^.first<>lb^.last then
  begin
    remlink(lb,id);
    idtest:= lb^.first;
    while idtest<>nil     do
    begin
      if (strcasecmp(idtest^.name,id^.name)>0)or(idtest^.lib<>nil) then
      begin
        insertlinkbefore(lb,idtest,id);
        break;
      end;
      idtest:= pID(idtest^.next);
    end;

    (* als laatste *)
    if idtest=nil then
      addtail(lb,id);
  end;
end;

(* alleen locale blokken: externe en indirekte hebben al een unieke ID *)
(* return 1: nieuwe naam gemaakt *)
function new_id(lb: pListBase;  id: pID;  tname: pchar): integer;
var
  idtest: pID;
  nr: integer = 0;
  nrtest: integer;
  maxtest: integer = 32;
  a: integer;
  aname: array [0..Pred(32)] of char;
  name: pchar;
  left: array [0..Pred(24)] of char;
  leftest: array [0..Pred(24)] of char;
  in_use: array [0..Pred(32)] of char;
begin
  (* - naam splitsen
    * - zoeken
    *)

  if id^.lib<>nil then
    exit(0);

  if tname=nil then
  name:= id^.name+2
  else
  begin
    (* tname can be const *)
    strncpy(aname,tname,21);
    name:= aname;

    if lstrlen(name)>21 then
    name[21]:= #0;
  end;

  if lb=nil then
  lb:= wich_libbase(G.main, psmallint(@id^.name[0])^);

  (* eerste fase: bestaat de id al? *)
  idtest:= lb^.first;
  while idtest<>nil   do
  begin
    if (id<>idtest)and(idtest^.lib=nil) then
    begin
      (* niet alphabetic testen! *)
      (* optim *)
      if idtest^.name[2]=name[0] then
      begin
        if strcmp(name,idtest^.name+2)=0 then
        break;
      end;
    end;
    idtest:= pID(idtest^.next);
  end;

  (* if there is no double return *)
  if idtest=nil then
  begin
    strcpy(id^.name+2,name);
    exit(0);
  end;

  bzero(@in_use[0],maxtest);
  splitIDname(name,left,@nr);

  if (nr>999)and(lstrlen(left)>16) then
  left[16]:= #0
  else
  if lstrlen(left)>17 then
  left[17]:= #0;

  idtest:= pID(lb^.first);
  while idtest<>nil   do
  begin
    if (id<>idtest)and(idtest^.lib=nil) then
    begin
      splitIDname(idtest^.name+2,leftest,@nrtest);
      if strcmp(left,leftest)=0 then
      begin
        if nrtest<maxtest then
        in_use[nrtest]:= #1;
        if nr<=nrtest then
        nr:= nrtest+1;
      end;
    end;
    idtest:= pID(idtest^.next);
  end;
  for{while} a:=0 to Pred(maxtest) { a++}  do
  begin
    if a>=nr then
    break;
    if in_use[a]=#0 then
    begin
      nr:= a;
      break;
    end;
  end;
  if nr=0 then
  sprintf(id^.name+2,'%s',[left])
  else
  begin
    sprintf(id^.name+2,'%s.%0.3d',[left,nr]);
  end;
  sort_alpha_id(lb,id);
    exit(1);
end;

//procedure clear_id_newpoins; 
//var
//lbarray: array [0..Pred(30)] of pListBase; 
//id: pID; 
//a: integer; 
//begin

//  a:= set_listbasepointers(G.main,lbarray); 
//  while a{--} dec(a); 
//  do
//  begin 
//    id:= lbarray[a].first; 
//    while id
//    do
//    begin 
//      id._new:= 0; 
//      id.flag:= id.flag and ( not LIB_NEW); 
//      id:= id.next; 
//    end;
//  end;
//end;

//procedure all_local; 
//var
//lbarray: array [0..Pred(30)] of pListBase; 
//tempbase: ListBase;

//id: pID; 
//idn: pID; 
//idp: pID; 
//a: integer; 
//begin

//  tempbase:=(0,0); 

//  a:= set_listbasepointers(G.main,lbarray); 
//  while a{--} dec(a); 
//  do
//  begin 
//    id:= lbarray[a].first; 
//    while id
//    do
//    begin 
//      id._new:= 0; 
//      id.flag:= id.flag and ( not (LIB_EXTERN or LIB_INDIRECT or LIB_NEW)); 
//      idn:= id.next; (* id wordt mogelijk opnieuw ingevoegd *)
//      if id.lib<>nil then
//      begin 
//        id.lib:= 0; 
//        new_id(lbarray[a],id,0); 
//        sort_alpha_id(lbarray[a],id); 
//        (* new_id doet dit alleen bij dubbele namen *)
//      end;
//      else
//      begin 
//        (* patch: testen of de zaak wel alphabetisch is *)
//        (*
//            if(idn) {
//             if(strcasecmp(id->name, idn->name)>0) {
//              remlink(lbarray[a], id);
//              addtail(&tempbase, id);
//             }
//             else if(id->prev) {
//              idp= id->prev;
//              if(strcasecmp(idp->name, id->name)>0) {
//               remlink(lbarray[a], id);
//               addtail(&tempbase, id);
//              }
//             }
//            }
//        *)
//      end;
//      id:= idn; 
//    end;
//    (* patch2: zorgen dat de zaak wel alphabetisch is *)
//    while id:=tempbase.first
//    do
//    begin 
//      remlink(@tempbase,id); 
//      addtail(lbarray[a],id); 
//      new_id(lbarray[a],id,0); 
//    end;
//  end;
//end;

//procedure test_idbutton(name: pchar); 
//var
//lb: pListBase; 
//id: pID; 
//idtest: pID; 
//begin(* vanuit buttons: als naam al bestaat: new_id aanroepen *)

//  lb:= wich_libbase(G.main,GS(name-2)); 
//  if lb=0 then
//  exit;
//  (* zoek welke id *)
//  idtest:= lb.first; 
//  while idtest
//  do
//  begin 
//    if lstrcmp(idtest.name+2,name)=0 then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    idtest:= idtest.next; 
//  end;
//  if idtest<>nil then
//  if new_id(lb,idtest,name)=0 then
//  sort_alpha_id(lb,idtest); 
//end;

//procedure rename_id(id: pID;  name: pchar); 
//var
//lb: pListBase; 
//idtest: pID; 
//begin

//  strncpy(id.name+2,name,21); 
//  lb:= wich_libbase(G.main,GS(id.name)); 
//  new_id(lb,id,name); 
//end;

end.
