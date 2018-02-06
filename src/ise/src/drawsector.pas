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

(*  drawsector.c     GRAPHICS
 *
 *  maart 96
 *
 *
 *)

unit drawsector;

interface

{$if defined(GL_EXT_texture_object)}

//function glBindTexture(A: integer; B: integer): integer; {<= !!!6 unknown macro}

//function glGenTextures(A: integer; B: integer): integer; {<= !!!6 unknown macro}

//function glDeleteTextures(A: integer; B: integer): integer; {<= !!!6 unknown macro}

//function glPolygonOffset(A: integer; B: integer): integer; {<= !!!6 unknown macro}

{$else}

//const
//GL_FUNC_ADD_EXT = GL_FUNC_ADD; 
//(* #define GL_FUNC_REVERSE_SUBTRACT_EXT GL_FUNC_REVERSE_SUBTRACT *)
//(* #define GL_POLYGON_OFFSET_EXT   GL_POLYGON_OFFSET *)

{$endif}

procedure init_realtime_GL;

var
texmat1: array [0..3,0..3] of single; 
texmat4: array [0..3,0..3] of single; 
//curpage: pImage = nil;
curtile: integer = 0; 
curmode: integer = 0; 
texwindx: smallint; 
texwindy: smallint; 
texwinsx: smallint; 
texwinsy: smallint; 
source: integer; 
dest: integer; 

implementation

uses
  GL,
  arithb;

//procedure copy_part_from_ibuf(ibuf: pImBuf;  rect: puint;  startx: smallint;  starty: smallint;  endx: smallint;  endy: smallint); 
//var
//rt: puint; 
//rp: puint; 
//y: smallint; 
//heigth: smallint; 
//len: smallint; 
//(* de juiste offset in rectot *)

//begin

//  rt:= ibuf.rect+(starty*ibuf.x+startx); 
//  len:= (endx-startx); 
//  heigth:= (endy-starty); 
//  rp:= rect; 

//  for{while} y:=0 to Pred(heigth) { y++}
//  do
//  begin 
//    memcpy(rp,rt,len*4); 
//    rt:= rt + (ibuf.x); 
//    rp:= rp + (len); 
//  end;
//end;

//procedure free_realtime_image(ima: pImage); 
//begin
//  if ima.bindcode<>0 then
//  begin 
//    glDeleteTextures(1,@ima.bindcode); 
//    ima.bindcode:= 0; 
//  end;
//  if ima.repbind<>0 then
//  begin 
//    glDeleteTextures(ima.totbind,ima.repbind); 
//    freeN(ima.repbind); 
//    ima.repbind:= 0; 
//  end;
//end;

//procedure make_repbind(ima: pImage); 
//var
//a: integer; 
//begin

//  if ima=0)or(ima.ibuf=0 then
//  exit;

//  if ima.repbind<>0 then
//  begin 
//    glDeleteTextures(ima.totbind,ima.repbind); 
//    freeN(ima.repbind); 
//    ima.repbind:= 0; 
//  end;
//  ima.totbind:= ima.xrep*ima.yrep; 
//  if ima.totbind>1 then
//  begin 
//    ima.repbind:= callocN(sizeof(int)*ima.totbind,'repbind'); 
//  end;
//end;

//var {was static}
//alphamode: integer;

//lasttface: pTFace;

//function set_tpage(tface: pTFace): integer; 
//var
//ima: pImage; 
//mat: array [0..3,0..3] of single; 
//rect: puint; 
//bind: puint; 
//tpx: integer; 
//tpy: integer; 
//tilemode: integer; 
//mode: integer; (* afschakelen *)
//str: array [0..Pred(256)] of char; 
//begin
//  alphamode:=-1; 
//  lasttface:=0; 

//  if tface=0 then
//  begin 
//    if lasttface=0 then
//    begin
//      result:= 0; 
//      exit;
//    end;
//    lasttface:= 0; 
//    curtile:= 0; 
//    curpage:= 0; 
//    if curmode<>0 then
//    begin 
//      glMatrixMode(GL_TEXTURE); 
//      glLoadMatrixf(texmat1); 
//      glMatrixMode(GL_MODELVIEW); 
//    end;
//    curmode:= 0; 
//    alphamode:= -1; 
//    glDisable(GL_BLEND); 
//    glDisable(GL_TEXTURE_2D); 
//    begin
//      result:= 0; 
//      exit;
//    end;
//  end;
//  lasttface:= tface; 
//  if alphamode<>tface.transp then
//  begin 
//    alphamode:= tface.transp; 
//    if alphamode<>0 then
//    begin 
//      glEnable(GL_BLEND); 
//      if alphamode=TF_ADD then
//      begin 
//        glBlendFunc(GL_ONE,GL_ONE); 
//      end;
//      (*  glBlendEquationEXT(GL_FUNC_ADD_EXT); *)
//      else
//      if alphamode=TF_ALPHA then
//      begin 
//        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
//      end;
//      (*  glBlendEquationEXT(GL_FUNC_ADD_EXT); *)
//    end;
//    (* else { *)
//    (*  glBlendFunc(GL_ONE, GL_ONE); *)
//    (*  glBlendEquationEXT(GL_FUNC_REVERSE_SUBTRACT_EXT); *)
//    (* } *)
//    else
//    glDisable(GL_BLEND); 
//  end;
//  ima:= tface.tpage; 
//  tilemode:= tface.mode and TF_TILES; 

//  if ima=curpage)and(curtile=tface.tile)and(tilemode=curmode then
//  begin
//    result:= ima<>0; 
//    exit;

//  end;
//  if tilemode<>curmode then
//  begin 
//    glMatrixMode(GL_TEXTURE); 
//    if tilemode<>0 then
//    begin 
//      glLoadMatrixf(texmat4); 
//    end;
//    else
//    glLoadMatrixf(texmat1); 
//    glMatrixMode(GL_MODELVIEW); 

//  end;

//  if ima=0)or(ima.ok=0 then
//  begin 
//    glDisable(GL_TEXTURE_2D); 
//    curtile:= tface.tile; 
//    curpage:= 0; 
//    curmode:= tilemode; 
//    begin
//      result:= 0; 
//      exit;
//    end;
//  end;

//  if ima.ibuf=0 then
//  begin 

//    strcpy(str,ima.name); 
//    convertstringcode(str); 
//    ima.ibuf:= loadiffname(str,LI_rect); 
//    if ima.ibuf=0 then
//    begin 
//      ima.ok:= 0; 
//      curtile:= tface.tile; 
//      curpage:= 0; 
//      curmode:= tilemode; 
//      glDisable(GL_TEXTURE_2D); 

//      begin
//        result:= 0; 
//        exit;
//      end;
//    end;
//  end;

//  if ima.tpageflag and IMA_TWINANIM then
//  curtile:= ima.lastframe; 
//  else
//  curtile:= tface.tile; 
//  if tilemode<>0 then
//  begin 
//    if ima.repbind=0 then
//    make_repbind(ima); 
//    if curtile>=ima.totbind then
//    curtile:= 0; (* this happens when you change repeat buttons *)
//    if ima.repbind<>0 then
//    bind:= ima.repbind+curtile; 
//    else
//    bind:= @ima.bindcode; 
//    if {*}bind^=0 then
//    begin 
//      texwindx:= ima.ibuf.x div ima.xrep; 
//      texwindy:= ima.ibuf.y div ima.yrep; 
//      if curtile>=ima.xrep*ima.yrep then
//      curtile:= ima.xrep*ima.yrep-1; 
//      texwinsy:= curtile div ima.xrep; 
//      texwinsx:= curtile-texwinsy*ima.xrep; 
//      texwinsx:= texwinsx * (texwindx); 
//      texwinsy:= texwinsy * (texwindy); 
//      tpx:= texwindx; 
//      tpy:= texwindy; 
//      rect:= ima.ibuf.rect+texwinsy*ima.ibuf.x+texwinsx; 
//    end;
//  end;
//  else
//  begin 
//    bind:= @ima.bindcode; 
//    if {*}bind^=0 then
//    begin 
//      tpx:= ima.ibuf.x; 
//      tpy:= ima.ibuf.y; 
//      rect:= ima.ibuf.rect; 
//    end;
//  end;

//  if {*}bind^=0 then
//  begin 
//    glGenTextures(1,bind); 
//    if (G.f and G_DEBUG) <> 0 then
//    begin 
//      PRINT(s,ima.id.name+2); 
//      PRINT2(d,d,*bind,tpx); 
//      PRINT2(d,d,curtile,tilemode); 
//    end;
//    glBindTexture(GL_TEXTURE_2D,*bind); 

//    if tilemode<>0 then
//    glPixelStorei(GL_UNPACK_ROW_LENGTH,ima.ibuf.x); 
//    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,tpx,tpy,0,GL_RGBA,GL_UNSIGNED_BYTE,rect); 
//    if tilemode<>0 then
//    glPixelStorei(GL_UNPACK_ROW_LENGTH,0); 
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
//    (* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); *)
//    (* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); *)

//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
//    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE); 
//  end;
//  else
//  glBindTexture(GL_TEXTURE_2D,*bind); 
//  glEnable(GL_TEXTURE_2D); 
//  curpage:= ima; 
//  curmode:= tilemode; 

//  begin
//    result:= 1; 
//    exit;
//  end;
//end;

//procedure get_co_portal(se: pSector;  type: integer;  ofs: pfloat;  cent: pfloat); 
//begin
//  cent[0]:= cent[1]:=cent[2]:=0.0; 
//  if type=PO_XPOS)or(type=PO_XNEG then
//  begin 
//    if type=PO_XPOS then
//    cent[0]:= 0.98*se.size[0]; 
//    else
//    cent[0]:= -0.98*se.size[0]; 
//    cent[1]:= cent[1] + (ofs[0]); 
//    cent[2]:= cent[2] + (ofs[1]); 

//  end;
//  else
//  if type=PO_YPOS)or(type=PO_YNEG then
//  begin 
//    if type=PO_YPOS then
//    cent[1]:= 0.98*se.size[1]; 
//    else
//    cent[1]:= -0.98*se.size[1]; 
//    cent[0]:= cent[0] + (ofs[0]); 
//    cent[2]:= cent[2] + (ofs[1]); 

//  end;
//  else
//  if type=PO_ZPOS)or(type=PO_ZNEG then
//  begin 
//    if type=PO_ZPOS then
//    cent[2]:= 0.98*se.size[2]; 
//    else
//    cent[2]:= -0.98*se.size[2]; 
//    cent[0]:= cent[0] + (ofs[0]); 
//    cent[1]:= cent[1] + (ofs[1]); 

//  end;
//end;

//procedure draw_portal(se: pSector;  po: pPortal;  act: integer); 
//var
//cent: array [0..2] of single; 
//size: single; 
//veci: array [0..2] of single; 
//vecj: array [0..2] of single; 
//col: array [0..3] of single; 
//cox: integer; 
//coy: integer; 
//begin

//  size:= 0.1*MAX3(se.size[0],se.size[1],se.size[2]); 
//  if size>0.5 then
//  size:= 0.5; 
//  get_co_portal(se,po.type,po.ofs,cent); 
//  if po.type=PO_XPOS)or(po.type=PO_XNEG then
//  begin 
//    cox:= 1; 
//    coy:= 2; 
//  end;
//  else
//  if po.type=PO_YPOS)or(po.type=PO_YNEG then
//  begin 
//    cox:= 0; 
//    coy:= 2; 
//  end;
//  else
//  begin 
//    cox:= 0; 
//    coy:= 1; 
//  end;
//  glGetFloatv(GL_CURRENT_COLOR,col); 

//  if G.f and G_BACKBUFSEL then
//  ; 
//  else
//  begin 
//    if po.sector<>0 then
//    glColor3ub(0,255,0); 
//    else
//    glColor3ub(0,0,0); 
//  end;
//  cent[cox]:= cent[cox] + (0.$5); 
//  cent[coy]:= cent[coy] - (0.$5); 
//  glBegin(GL_POLYGON); 

//  glVertex3fv(cent); 
//  cent[coy]:= cent[coy] + (size); 
//  glVertex3fv(cent); 
//  cent[cox]:= cent[cox] - (size); 
//  glVertex3fv(cent); 
//  cent[coy]:= cent[coy] - (size); 
//  glVertex3fv(cent); 
//  cent[cox]:= cent[cox] + (size); 
//  glEnd(); 
//  if G.f and G_BACKBUFSEL then
//  exit;

//  if act<>0 then
//  glColor3ub(255,255,255); 
//  else
//  if po.sector<>0 then
//  glColor3ub(225,200,0); 
//  else
//  if col[0]<>0.0 then
//  glColor3ub(150,50,150); 
//  else
//  glColor3ub(50,50,50); 
//  glBegin(GL_LINE_LOOP); 
//  glVertex3fv(cent); 
//  cent[coy]:= cent[coy] + (size); 
//  glVertex3fv(cent); 
//  cent[cox]:= cent[cox] - (size); 
//  glVertex3fv(cent); 
//  cent[coy]:= cent[coy] - (size); 
//  glVertex3fv(cent); 
//  cent[cox]:= cent[cox] + (size); 
//  glEnd(); 
//  glColor3f(col[0],col[1],col[2]); 

//end;

//procedure spack(ucol: uint); 
//var
//cp: pchar;

//begin
//  cp:=(char* )@ucol; 
//  glColor3ub(cp[3],cp[2],cp[1]); 
//end;

//procedure draw_hide_tfaces(ob: pObject;  me: pMesh); 
//var
//tface: pTFace; 
//mface: pMFace; 
//v1: pfloat; 
//v2: pfloat; 
//v3: pfloat; 
//v4: pfloat; 
//a: integer; 
//begin

//  if me=0)or(me.tface=0 then
//  exit;

//  mface:= me.mface; 
//  tface:= me.tface; 
//  cpack($0); 

//  setlinestyle(1); 
//  while{for} 
//  a:= me.totface; 
//  {to} a>0
//  {;}
//  dec(a); 
//  inc(mface); 
//  inc(tface); 
//  do
//  begin 
//    if mface.v3=0 then
//    begin
//      {!!!7 possible troubles with "for" =>}
//      continue
//    end; 
//    if (tface.flag and TF_HIDE) then
//    begin 

//      v1:= (me.mvert+mface.v1).co; 
//      v2:= (me.mvert+mface.v2).co; 
//      v3:= (me.mvert+mface.v3).co; 
//      if mface.v4<>0 then
//      v4:= (me.mvert+mface.v4).co; 
//      else
//      v4:= 0; 
//      glBegin(GL_LINE_LOOP); 
//      glVertex3fv(v1); 
//      glVertex3fv(v2); 
//      glVertex3fv(v3); 
//      if mface.v4<>0 then
//      glVertex3fv(v4); 
//      glEnd(); 
//    end;
//  end;
//  setlinestyle(0); 
//end;

//procedure draw_tfaces3D(ob: pObject;  me: pMesh); 
//var
//mface: pMFace; 
//tface: pTFace; 
//v1: pfloat; 
//v2: pfloat; 
//v3: pfloat; 
//v4: pfloat; 
//a: integer; 
//begin

//  if me=0)or(me.tface=0 then
//  exit;

//  glDisable(GL_DEPTH_TEST); 
//  mface:= me.mface; 
//  tface:= me.tface; 
//  (* SELECT faces *)
//  while{for} 
//  a:= me.totface; 
//  {to} a>0
//  {;}
//  dec(a); 
//  inc(mface); 
//  inc(tface); 
//  do
//  begin 
//    if mface.v3=0 then
//    begin
//      {!!!7 possible troubles with "for" =>}
//      continue
//    end; 
//    if tface.flag and ACTIVE then
//    begin
//      {!!!7 possible troubles with "for" =>}
//      continue
//    end; 
//    if tface.flag and TF_HIDE then
//    begin
//      {!!!7 possible troubles with "for" =>}
//      continue
//    end; 
//    if tface.flag and SELECT then
//    begin 
//      v1:= (me.mvert+mface.v1).co; 
//      v2:= (me.mvert+mface.v2).co; 
//      v3:= (me.mvert+mface.v3).co; 
//      if mface.v4<>0 then
//      v4:= (me.mvert+mface.v4).co; 
//      else
//      v4:= 0; 
//      cpack($0); 
//      glBegin(GL_LINE_LOOP); 
//      glVertex3fv(v1); 
//      glVertex3fv(v2); 
//      glVertex3fv(v3); 
//      if v4<>0 then
//      glVertex3fv(v4); 
//      glEnd(); 
//      cpack($FFFFFF); 

//      setlinestyle(1); 
//      glBegin(GL_LINE_LOOP); 
//      glVertex3fv(v1); 
//      glVertex3fv(v2); 
//      glVertex3fv(v3); 
//      if v4<>0 then
//      glVertex3fv(v4); 
//      glEnd(); 
//      setlinestyle(0); 
//    end;
//  end;
//  mface:= me.mface; 
//  tface:= me.tface; (* ACTIVE faces *)
//  while{for} 
//  a:= me.totface; 
//  {to} a>0
//  {;}
//  dec(a); 
//  inc(mface); 
//  inc(tface); 
//  do
//  begin 
//    if mface.v3=0 then
//    begin
//      {!!!7 possible troubles with "for" =>}
//      continue
//    end; 
//    if (tface.flag and ACTIVE))and((tface.flag and SELECT) then
//    begin 

//      v1:= (me.mvert+mface.v1).co; 
//      v2:= (me.mvert+mface.v2).co; 
//      v3:= (me.mvert+mface.v3).co; 
//      if mface.v4<>0 then
//      v4:= (me.mvert+mface.v4).co; 
//      else
//      v4:= 0; 
//      cpack($FF); 
//      (* kleuren: R=x G=y *)
//      glBegin(GL_LINE_STRIP); 
//      glVertex3fv(v1); 
//      if v4<>0 then
//      glVertex3fv(v4); 
//      else
//      glVertex3fv(v3); 
//      glEnd(); 
//      cpack($FF00); 
//      glBegin(GL_LINE_STRIP); 
//      glVertex3fv(v1); 
//      glVertex3fv(v2); 
//      glEnd(); 
//      cpack($0); 
//      glBegin(GL_LINE_STRIP); 
//      glVertex3fv(v2); 
//      glVertex3fv(v3); 
//      if v4<>0 then
//      glVertex3fv(v4); 
//      glEnd(); 
//      cpack($FFFFFF); 
//      setlinestyle(1); 
//      glBegin(GL_LINE_LOOP); 
//      glVertex3fv(v1); 
//      glVertex3fv(v2); 
//      glVertex3fv(v3); 
//      if mface.v4<>0 then
//      glVertex3fv(v4); 
//      glEnd(); 
//      setlinestyle(0); 

//    end;
//  end;
//  glEnable(GL_DEPTH_TEST); 

//end;

//rvecmat: array [0..2,0..2] of single; 
//rcolmat: array [0..2,0..2] of single; 
//punopoin: array [0..3] of psmallint; 
//var {was static}
//lampar: array [0..Pred(24)] of pObject; 
//totlamp: integer;

//curlamp: integer;

//need_init: integer;

//function do_realtimelight(ob: pObject;  tface: pTFace;  col: pfloat): integer; 
//var
//la: pLamp; 
//lf: pLife; 
//inp: single; 
//lay: uint; 
//a: integer; 
//cp: pchar; 
//base: pBase;

//(* lamp array aanlegen *)
//begin

//  totlamp:=0; 
//  curlamp:=0; 
//  need_init:=0; 

//  if G.vd=0 then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  if G.vd.localvd<>0 then
//  lay:= G.vd.localvd.lay; 
//  else
//  lay:= G.vd.lay; 
//  if ob=0 then
//  begin 
//    need_init:= 1; 
//    begin
//      result:= 0; 
//      exit;
//    end;
//  end;
//  if ob)and(need_init then
//  begin 
//    base:=FIRSTBASE; 
//    (* lamp array aanlegen *)
//    totlamp:= 0; 
//    while base
//    do
//    begin 
//      if base.lay and lay then
//      begin 
//        if base.object.type=OB_LAMP then
//        begin 
//          la:= base.object.data; 
//          if la.type=LA_SUN then
//          begin 
//            lampar[totlamp]:= base.object; 
//            inc(totlamp); 
//            if totlamp>=24 then
//            break; {<= !!!b possible in "switch" - then remove this line}
//          end;
//        end;
//        else

//        function ELEM{!!!3 unknown typedef}: if; 
//        begin
//          where_is_object(base.object); 
//          (* imat nodig voor lampberekening *)
//          Mat4Invert(base.object.imat,base.object.obmat); 
//        end
//      end;
//      base:= base.next; 
//    end;
//    need_init:= 0; 
//  end;
//  if totlamp=0 then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  if G.f and G_VERTEXPAINT then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  if tface=0 then
//  begin 
//    (* init matrices *)
//    curlamp:= 0; 
//    for{while} a:=0 to Pred(totlamp) { a++}
//    do
//    begin 
//      if lampar[a].lay and ob.lay then
//      begin 
//        la:= lampar[a].data; 
//        rvecmat[curlamp][0]:= lampar[a].obmat[2][0]; 
//        rvecmat[curlamp][1]:= lampar[a].obmat[2][1]; 
//        rvecmat[curlamp][2]:= lampar[a].obmat[2][2]; 
//        Mat4Mul3Vecfl(ob.imat,rvecmat[curlamp]); 

//        Normalise(rvecmat[curlamp]); 
//        rcolmat[curlamp][0]:= la.energy*la.r div 32767.0; 
//        rcolmat[curlamp][1]:= la.energy*la.g div 32767.0; 
//        rcolmat[curlamp][2]:= la.energy*la.b div 32767.0; 
//        inc(curlamp); (* deze schaal wordt terug gecorrigeerd in writeblendpsx *)
//        if curlamp>=3 then
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//    end;
//    begin
//      result:= curlamp; 
//      exit;
//    end;
//  end;
//  if curlamp=0 then
//  begin
//    result:= 0; 
//    exit;
//    (* voorlopig (even) alleen solid *)
//  end;
//  bzero(col,48); 
//  cp:= {!!!a type cast? =>} {pchar(}tface.col; 
//  a:= curlamp; 
//  if tface.mode and TF_SHAREDCOL then
//  begin 
//    while a{--} dec(a); 
//    do
//    begin 
//      inp:= INPR(punopoin[0],rvecmat[a]) div 255.0; 
//      if inp>0.0 then
//      begin 
//        col[0]:= col[0] + (inp*rcolmat[a][0]*cp[3]); 
//        col[1]:= col[1] + (inp*rcolmat[a][1]*cp[2]); 
//        col[2]:= col[2] + (inp*rcolmat[a][2]*cp[1]); 
//      end;
//      inp:= INPR(punopoin[1],rvecmat[a]) div 255.0; 
//      if inp>0.0 then
//      begin 
//        col[3]:= col[3] + (inp*rcolmat[a][0]*cp[7]); 
//        col[4]:= col[4] + (inp*rcolmat[a][1]*cp[6]); 
//        col[5]:= col[5] + (inp*rcolmat[a][2]*cp[5]); 
//      end;
//      inp:= INPR(punopoin[2],rvecmat[a]) div 255.0; 
//      if inp>0.0 then
//      begin 
//        col[6]:= col[6] + (inp*rcolmat[a][0]*cp[11]); 
//        col[7]:= col[7] + (inp*rcolmat[a][1]*cp[10]); 
//        col[8]:= col[8] + (inp*rcolmat[a][2]*cp[9]); 
//      end;
//      if punopoin[3]<>0 then
//      begin 
//        inp:= INPR(punopoin[3],rvecmat[a]) div 255.0; 
//        if inp>0.0 then
//        begin 
//          col[9]:= col[9] + (inp*rcolmat[a][0]*cp[15]); 
//          col[10]:= col[10] + (inp*rcolmat[a][1]*cp[14]); 
//          col[11]:= col[11] + (inp*rcolmat[a][2]*cp[13]); 
//        end;
//      end;
//    end;

//  end;
//  else
//  begin 
//    while a{--} dec(a); 
//    do
//    begin 
//      inp:= INPR(tface.no,rvecmat[a]) div 255.0; 
//      if inp>0.0 then
//      begin 
//        col[0]:= col[0] + (inp*rcolmat[a][0]*cp[3]); 
//        col[1]:= col[1] + (inp*rcolmat[a][1]*cp[2]); 
//        col[2]:= col[2] + (inp*rcolmat[a][2]*cp[1]); 
//        if tface.mode and TF_GOUR then
//        begin 
//          col[3]:= col[3] + (inp*rcolmat[a][0]*cp[7]); 
//          col[4]:= col[4] + (inp*rcolmat[a][1]*cp[6]); 
//          col[5]:= col[5] + (inp*rcolmat[a][2]*cp[5]); 
//          col[6]:= col[6] + (inp*rcolmat[a][0]*cp[11]); 
//          col[7]:= col[7] + (inp*rcolmat[a][1]*cp[10]); 
//          col[8]:= col[8] + (inp*rcolmat[a][2]*cp[9]); 
//          col[9]:= col[9] + (inp*rcolmat[a][0]*cp[15]); 
//          col[10]:= col[10] + (inp*rcolmat[a][1]*cp[14]); 
//          col[11]:= col[11] + (inp*rcolmat[a][2]*cp[13]); 
//        end;
//      end;
//    end;
//  end;
//  begin
//    result:= 1; 
//    exit;
//  end;
//end;

//type
//tra_ob = record
//next: ptra_ob; 
//prev: ptra_ob; 
//ob: pObject; 
//me: pMesh; 
//dt: integer; 
//end;
//tra_ob = tra_ob; 

//const
//MAX_TRA_OB = 64; 

//var {was static}
//tot_tra_ob: integer = 0; 
//tra_ob_ar: array [0..Pred(MAX_TRA_OB)] of tra_ob; 

//procedure add_tra_object(ob: pObject;  me: pMesh;  dt: integer); 
//begin
//  if tot_tra_ob>=MAX_TRA_OB then
//  exit;
//  tra_ob_ar[tot_tra_ob].ob:= ob; 
//  tra_ob_ar[tot_tra_ob].me:= me; 
//  tra_ob_ar[tot_tra_ob].dt:= dt; 
//  inc(tot_tra_ob); 
//end;

//(* maximum dt: precies volgens ingestelde waardes *)

//procedure draw_tface_mesh(ob: pObject;  me: pMesh;  dt: integer); 
//var
//lf: pLife; 
//tface: pTFace; 
//mface: pMFace; 
//mvert: pMVert; 
//ima: pImage; 
//v1: pfloat; 
//v2: pfloat; 
//v3: pfloat; 
//v4: pfloat; 
//col: array [0..3,0..2] of single; 
//coli: array [0..3] of single; 
//obcol: uint; 
//a: integer; 
//mode: integer; 
//islight: smallint; 
//istex: smallint; 
//istra: smallint;

//cp: pchar; 
//se: pSector;

//lf: pLife;

//begin

//  istra:=0; 

//  if me=0 then
//  exit;
//  if ob.type=OB_LIFE then
//  lf:= ob.data; 
//  else
//  lf:= 0; 
//  glShadeModel(GL_SMOOTH); 
//  glGetFloatv(GL_CURRENT_COLOR,coli); 

//  (* als meshes uit lib gelezen zijn en alleen mcol hebben: *)
//  if me.tface=0 then
//  make_tfaces(me); 
//  islight:= do_realtimelight(ob,0,0); 

//  (* de ob color *)
//  if ob.type=OB_SECTOR then
//  begin 
//    se:=ob.data; 
//    obcol:= rgb_to_mcol(se.r,se.g,se.b); 
//  end;
//  else
//  if ob.type=OB_LIFE then
//  begin 
//    lf:=ob.data; 
//    obcol:= rgb_to_mcol(lf.r,lf.g,lf.b); 
//  end;
//  glCullFace(GL_BACK); 
//  (* eerst alle texture polys *)
//  glEnable(GL_CULL_FACE); 
//  if G.vd.drawtype=OB_TEXTURE then
//  istex:= 1; 
//  else
//  istex:= 0; 
//  (* signal to NOT draw transparant separate *)
//  if (G.f and G_SIMULATION)=0 then
//  istra:= 2; 
//  if dt>OB_SOLID then
//  begin 
//    mface:= me.mface; 
//    tface:= me.tface; 
//    while{for} 
//    a:= me.totface; 
//    {to} a>0
//    {;}
//    dec(a); 
//    inc(mface); 
//    inc(tface); 
//    do
//    begin 
//      if mface.v3=0 then
//      begin
//        {!!!7 possible troubles with "for" =>}
//        continue
//      end; 
//      if tface.flag and TF_HIDE then
//      begin
//        {!!!7 possible troubles with "for" =>}
//        continue
//      end; 
//      if istex)and(tface.transp then
//      begin 
//        if istra=0 then
//        begin 
//          add_tra_object(ob,me,dt); 
//          istra:= 1; 
//        end;
//        if istra=1 then
//        begin
//          {!!!7 possible troubles with "for" =>}
//          continue
//        end; 
//      end;
//      mode:= tface.mode; 
//      if mode and TF_OBCOL then
//      tface.col[0]:= obcol; 
//      v1:= (me.mvert+mface.v1).co; 
//      v2:= (me.mvert+mface.v2).co; 
//      v3:= (me.mvert+mface.v3).co; 
//      if mface.v4<>0 then
//      v4:= (me.mvert+mface.v4).co; 
//      else
//      v4:= 0; 
//      if mode and TF_SHAREDCOL then
//      begin 
//        punopoin[0]:= {!!!a type cast? =>} {psmallint(}(v1+3); 
//        punopoin[1]:= {!!!a type cast? =>} {psmallint(}(v2+3); 
//        punopoin[2]:= {!!!a type cast? =>} {psmallint(}(v3+3); 
//        if v4<>0 then
//        punopoin[3]:= {!!!a type cast? =>} {psmallint(}(v4+3); 
//        else
//        punopoin[3]:= 0; 
//      end;

//      if mode and TF_TWOSIDE then
//      begin 
//        glDisable(GL_CULL_FACE); 
//      end;
//      else
//      begin 
//        glEnable(GL_CULL_FACE); 
//      end;

//      if istex)and((mode and TF_TEX) then
//      begin 
//        (* in set_tpage worden dingen gedaan die niet binnen een bgnpolygon mogen liggen *)
//        if set_tpage(tface) then
//        begin 
//          if islight)and((mode and TF_LIGHT) then
//          begin 
//            do_realtimelight(ob,tface,col[0]); 
//            glBegin(GL_POLYGON); 
//            glTexCoord2sv(tface.uv[0]); 
//            glColor3fv(col[0]); 

//            glVertex3fv(v1); 
//            glTexCoord2sv(tface.uv[1]); 
//            if mode and TF_GOUR then
//            glColor3fv(col[1]); 
//            glVertex3fv(v2); 
//            glTexCoord2sv(tface.uv[2]); 
//            if mode and TF_GOUR then
//            glColor3fv(col[2]); 
//            glVertex3fv(v3); 
//            if v4<>0 then
//            begin 
//              glTexCoord2sv(tface.uv[3]); 
//              if mode and TF_GOUR then
//              glColor3fv(col[3]); 
//              glVertex3fv(v4); 
//            end;
//            glEnd(); 
//          end;
//          else
//          begin 
//            glBegin(GL_POLYGON); 
//            glTexCoord2sv(tface.uv[0]); 
//            spack(tface.col[0]); 

//            glVertex3fv(v1); 
//            glTexCoord2sv(tface.uv[1]); 
//            if mode and TF_GOUR then
//            spack(tface.col[1]); 
//            glVertex3fv(v2); 
//            glTexCoord2sv(tface.uv[2]); 
//            if mode and TF_GOUR then
//            spack(tface.col[2]); 
//            glVertex3fv(v3); 
//            if v4<>0 then
//            begin 
//              glTexCoord2sv(tface.uv[3]); 
//              if mode and TF_GOUR then
//              spack(tface.col[3]); 
//              glVertex3fv(v4); 
//            end;
//            glEnd(); 
//          end;
//        end;
//        else
//        begin 
//          (* waarschuwings polygoon *)

//          glBegin(GL_POLYGON); 
//          cpack($FF00FF); 
//          glVertex3fv(v1); 
//          glVertex3fv(v2); 
//          glVertex3fv(v3); 
//          if v4<>0 then
//          glVertex3fv(v4); 
//          glEnd(); 
//        end;
//      end;
//      else
//      begin 
//        set_tpage(0); 
//        glBegin(GL_POLYGON); 
//        if islight)and((tface.mode and TF_LIGHT) then
//        begin 
//          do_realtimelight(ob,tface,col[0]); 
//          glColor3fv(col[0]); 
//          glVertex3fv(v1); 
//          if mode and TF_GOUR then
//          begin 
//            glColor3fv(col[1]); 
//            glVertex3fv(v2); 
//            glColor3fv(col[2]); 
//            glVertex3fv(v3); 
//            if v4<>0 then
//            begin 
//              glColor3fv(col[3]); 
//              glVertex3fv(v4); 
//            end;
//          end;
//          else
//          begin 
//            glVertex3fv(v2); 
//            glVertex3fv(v3); 
//            if v4<>0 then
//            glVertex3fv(v4); 
//          end;
//        end;
//        else
//        begin 

//          cp:= (char* )@(tface.col[0]); 
//          glColor3ub(cp[3],cp[2],cp[1]); 
//          glVertex3fv(v1); 
//          if mode and TF_GOUR then
//          begin 
//            glColor3ub(cp[7],cp[6],cp[5]); 
//            glVertex3fv(v2); 
//            glColor3ub(cp[11],cp[10],cp[9]); 
//            glVertex3fv(v3); 
//            if v4<>0 then
//            begin 
//              glColor3ub(cp[15],cp[14],cp[13]); 
//              glVertex3fv(v4); 
//            end;
//          end;
//          else
//          begin 
//            glVertex3fv(v2); 
//            glVertex3fv(v3); 
//            if v4<>0 then
//            glVertex3fv(v4); 
//          end;
//        end;
//        glEnd(); 
//      end;
//    end;
//    set_tpage(0); 
//    (* textures uitzetten *)
//  end;

//  else
//  begin 
//    (* alle niet-texture polys *)
//    mface:= me.mface; 
//    tface:= me.tface; 
//    while{for} 
//    a:= me.totface; 
//    {to} a>0
//    {;}
//    dec(a); 
//    inc(mface); 
//    inc(tface); 
//    do
//    begin 
//      if mface.v3=0 then
//      begin
//        {!!!7 possible troubles with "for" =>}
//        continue
//      end; 
//      if tface.flag and TF_HIDE then
//      begin
//        {!!!7 possible troubles with "for" =>}
//        continue
//      end; 
//      mode:= tface.mode; 
//      if tface.mode and TF_TWOSIDE then
//      begin 
//        glDisable(GL_CULL_FACE); 
//      end;
//      else
//      begin 
//        glEnable(GL_CULL_FACE); 
//      end;

//      if mode and TF_OBCOL then
//      tface.col[0]:= obcol; 
//      v1:= (me.mvert+mface.v1).co; 
//      v2:= (me.mvert+mface.v2).co; 
//      v3:= (me.mvert+mface.v3).co; 
//      if mface.v4<>0 then
//      v4:= (me.mvert+mface.v4).co; 
//      else
//      v4:= 0; 
//      if mode and TF_SHAREDCOL then
//      begin 
//        punopoin[0]:= {!!!a type cast? =>} {psmallint(}(v1+3); 
//        punopoin[1]:= {!!!a type cast? =>} {psmallint(}(v2+3); 
//        punopoin[2]:= {!!!a type cast? =>} {psmallint(}(v3+3); 
//        if v4<>0 then
//        punopoin[3]:= {!!!a type cast? =>} {psmallint(}(v4+3); 
//        else
//        punopoin[3]:= 0; 
//      end;
//      glBegin(GL_POLYGON); 
//      if islight)and((tface.mode and TF_LIGHT) then
//      begin 
//        do_realtimelight(ob,tface,col[0]); 
//        glColor3fv(col[0]); 
//        glVertex3fv(v1); 
//        if mode and TF_GOUR then
//        begin 
//          glColor3fv(col[1]); 
//          glVertex3fv(v2); 
//          glColor3fv(col[2]); 
//          glVertex3fv(v3); 
//          if v4<>0 then
//          begin 
//            glColor3fv(col[3]); 
//            glVertex3fv(v4); 
//          end;
//        end;
//        else
//        begin 
//          glVertex3fv(v2); 
//          glVertex3fv(v3); 
//          if v4<>0 then
//          glVertex3fv(v4); 
//        end;
//      end;
//      else
//      begin 
//        cp:= (char* )@(tface.col[0]); 
//        if mode and TF_GOUR then
//        begin 

//          glColor3ub(cp[3],cp[2],cp[1]); 
//          glVertex3fv(v1); 
//          glColor3ub(cp[7],cp[6],cp[5]); 
//          glVertex3fv(v2); 
//          glColor3ub(cp[11],cp[10],cp[9]); 
//          glVertex3fv(v3); 
//          if v4<>0 then
//          begin 
//            glColor3ub(cp[15],cp[14],cp[13]); 
//            glVertex3fv(v4); 
//          end;
//        end;
//        else
//        begin 
//          (* GL_FLAT *)
//          glColor3ub(cp[3],cp[2],cp[1]); 
//          glVertex3fv(v1); 
//          glVertex3fv(v2); 
//          glVertex3fv(v3); 
//          if v4<>0 then
//          glVertex3fv(v4); 
//        end;
//      end;
//      glEnd(); 
//    end;
//  end;
//  glShadeModel(GL_FLAT); 
//  glDisable(GL_CULL_FACE); 
//  draw_hide_tfaces(ob,me); 

//  if ob=OBACT)and((G.f and G_FACESELECT) then
//  begin 
//    draw_tfaces3D(ob,me); 
//  end;
//  glColor3f(coli[0],coli[1],coli[2]); 
//end;

//procedure draw_tface_meshes_tra; 
//var
//ob: pObject; 
//me: pMesh; 
//lf: pLife; 
//tface: pTFace; 
//mface: pMFace; 
//mvert: pMVert; 
//ima: pImage; 
//v1: pfloat; 
//v2: pfloat; 
//v3: pfloat; 
//v4: pfloat; 
//col: array [0..3,0..2] of single; 
//coli: array [0..3] of single; 
//obcol: uint; 
//a: integer; 
//t: integer; 
//mode: integer; 
//dt: integer; 
//islight: smallint; 
//cp: pchar; 
//se: pSector;

//tmp: pchar; 
//c: char; 
//lf: pLife;

//vec: array [0..2] of single; 
//mat: array [0..3,0..3] of single; 
//len: single; 
//begin

//  if G.vd.drawtype<>OB_TEXTURE then
//  exit;

//  glDepthMask(0); 
//  glShadeModel(GL_SMOOTH); 
//  glGetFloatv(GL_CURRENT_COLOR,coli); 
//  glCullFace(GL_BACK); 

//  glEnable(GL_CULL_FACE); 

//  for{while} t:=0 to Pred(tot_tra_ob) { t++}
//  do
//  begin 
//    ob:= tra_ob_ar[t].ob; 
//    me:= tra_ob_ar[t].me; 
//    dt:= tra_ob_ar[t].dt; 
//    multmatrix(ob.obmat); 

//    if me=0 then
//    exit;
//    if ob.type=OB_LIFE then
//    lf:= ob.data; 
//    else
//    lf:= 0; 
//    islight:= do_realtimelight(ob,0,0); (* de ob color *)
//    if ob.type=OB_SECTOR then
//    begin 
//      se:=ob.data; 
//      obcol:= rgb_to_mcol(se.r,se.g,se.b); 
//    end;
//    else
//    if ob.type=OB_LIFE then
//    begin 

//      lf:=ob.data; 
//      obcol:= rgb_to_mcol(lf.r,lf.g,lf.b); 
//    end;
//    if dt>OB_SOLID then
//    begin 
//      mface:= me.mface; 
//      tface:= me.tface; 
//      while{for} 
//      a:= me.totface; 
//      {to} a>0
//      {;}
//      dec(a); 
//      inc(mface); 
//      inc(tface); 
//      do
//      begin 
//        if mface.v3=0 then
//        begin
//          {!!!7 possible troubles with "for" =>}
//          continue
//        end; 
//        if tface.flag and TF_HIDE then
//        begin
//          {!!!7 possible troubles with "for" =>}
//          continue
//        end; 
//        if tface.transp<>0 then
//        ; 
//        else
//        begin
//          {!!!7 possible troubles with "for" =>}
//          continue
//        end; 
//        mode:= tface.mode; 
//        if mode and TF_OBCOL then
//        tface.col[0]:= obcol; 
//        v1:= (me.mvert+mface.v1).co; 
//        v2:= (me.mvert+mface.v2).co; 
//        v3:= (me.mvert+mface.v3).co; 
//        if mface.v4<>0 then
//        v4:= (me.mvert+mface.v4).co; 
//        else
//        v4:= 0; 
//        if mode and TF_SHAREDCOL then
//        begin 
//          punopoin[0]:= {!!!a type cast? =>} {psmallint(}(v1+3); 
//          punopoin[1]:= {!!!a type cast? =>} {psmallint(}(v2+3); 
//          punopoin[2]:= {!!!a type cast? =>} {psmallint(}(v3+3); 
//          if v4<>0 then
//          punopoin[3]:= {!!!a type cast? =>} {psmallint(}(v4+3); 
//          else
//          punopoin[3]:= 0; 
//        end;
//        if mode and TF_TWOSIDE then
//        begin 
//          glDisable(GL_CULL_FACE); 
//        end;
//        else
//        begin 
//          glEnable(GL_CULL_FACE); 
//        end;
//        if mode and TF_TEX then
//        begin 
//          (* in set_tpage worden dingen gedaan die niet binnen een bgnpolygon mogen liggen *)
//          if set_tpage(tface) then
//          begin 
//            if mode and TF_SHADOW then
//            begin 
//              if lf<>0 then
//              begin 

//                glBegin(GL_POLYGON); 
//                glTexCoord2sv(tface.uv[0]); 
//                spack(tface.col[0]); 
//                VecAddf(vec,v1,lf.floorloc); 
//                glVertex3fv(vec); 
//                glTexCoord2sv(tface.uv[1]); 
//                if mode and TF_GOUR then
//                spack(tface.col[1]); 
//                VecAddf(vec,v2,lf.floorloc); 
//                glVertex3fv(vec); 
//                glTexCoord2sv(tface.uv[2]); 
//                if mode and TF_GOUR then
//                spack(tface.col[2]); 
//                VecAddf(vec,v3,lf.floorloc); 
//                glVertex3fv(vec); 
//                if v4<>0 then
//                begin 
//                  glTexCoord2sv(tface.uv[3]); 
//                  if mode and TF_GOUR then
//                  spack(tface.col[3]); 
//                  VecAddf(vec,v4,lf.floorloc); 
//                  glVertex3fv(vec); 
//                end;
//                glEnd(); 
//              end;
//            end;
//            else
//            if islight)and((mode and TF_LIGHT) then
//            begin 
//              do_realtimelight(ob,tface,col[0]); 
//              glBegin(GL_POLYGON); 
//              glTexCoord2sv(tface.uv[0]); 
//              glColor3fv(col[0]); 
//              glVertex3fv(v1); 
//              glTexCoord2sv(tface.uv[1]); 
//              if mode and TF_GOUR then
//              glColor3fv(col[1]); 
//              glVertex3fv(v2); 
//              glTexCoord2sv(tface.uv[2]); 
//              if mode and TF_GOUR then
//              glColor3fv(col[2]); 
//              glVertex3fv(v3); 
//              if v4<>0 then
//              begin 
//                glTexCoord2sv(tface.uv[3]); 
//                if mode and TF_GOUR then
//                glColor3fv(col[3]); 
//                glVertex3fv(v4); 
//              end;
//              glEnd(); 
//            end;
//            else
//            if mode and TF_BILLBOARD then
//            begin 
//              (* actually halo! *)

//              glPushMatrix(); 
//              getmatrix(mat); 
//              len:= fsqrt(mat[0][0]*mat[0][0]+mat[1][1]*mat[1][1]+mat[2][2]*mat[2][2]); 
//              mat[0][0]:= mat[1][1]:=mat[2][2]:=len; 
//              mat[0][1]:= mat[0][2]:=0.0; 
//              mat[1][0]:= mat[1][2]:=0.0; 
//              mat[2][0]:= mat[2][1]:=0.0; 
//              mat[0][3]:= mat[1][3]:=mat[2][3]:=0.0; 
//              mat[3][3]:= 1.0; 
//              loadmatrix(mat); 

//              glBegin(GL_POLYGON); 
//              glTexCoord2sv(tface.uv[0]); 
//              spack(tface.col[0]); 
//              glVertex3fv(v1); 
//              glTexCoord2sv(tface.uv[1]); 
//              if mode and TF_GOUR then
//              spack(tface.col[1]); 
//              glVertex3fv(v2); 
//              glTexCoord2sv(tface.uv[2]); 
//              if mode and TF_GOUR then
//              spack(tface.col[2]); 
//              glVertex3fv(v3); 
//              if v4<>0 then
//              begin 
//                glTexCoord2sv(tface.uv[3]); 
//                if mode and TF_GOUR then
//                spack(tface.col[3]); 
//                glVertex3fv(v4); 
//              end;
//              glEnd(); 
//              glPopMatrix(); 
//            end;
//            else
//            begin 
//              glBegin(GL_POLYGON); 
//              glTexCoord2sv(tface.uv[0]); 
//              spack(tface.col[0]); 
//              glVertex3fv(v1); 
//              glTexCoord2sv(tface.uv[1]); 
//              if mode and TF_GOUR then
//              spack(tface.col[1]); 
//              glVertex3fv(v2); 
//              glTexCoord2sv(tface.uv[2]); 
//              if mode and TF_GOUR then
//              spack(tface.col[2]); 
//              glVertex3fv(v3); 
//              if v4<>0 then
//              begin 
//                glTexCoord2sv(tface.uv[3]); 
//                if mode and TF_GOUR then
//                spack(tface.col[3]); 
//                glVertex3fv(v4); 
//              end;
//              glEnd(); 
//            end;
//          end;
//        end;
//      end;
//    end;
//    loadmatrix(G.vd.viewmat); 
//  end;
//  set_tpage(0); 
//  (* textures uitzetten *)
//  tot_tra_ob:= 0; 
//  glDepthMask(1); 

//  glShadeModel(GL_FLAT); 
//  glDisable(GL_CULL_FACE); 
//  glColor3f(coli[0],coli[1],coli[2]); 
//end;

//(* col: restore van ghost *)

//procedure drawsector(ob: pObject;  dt: integer;  col: uint); 
//var
//obedit: pObject; 
//se: pSector; 
//po: pPortal; 
//a: integer; 
//flag: integer; 
//begin

//  se:= ob.data; 
//  if se.flag and SE_GHOST_OTHER then
//  begin 
//    if (G.f and (G_SIMULATION or G_BACKBUFSEL))=0 then
//    begin 
//      flag:= se.flag; 
//      obedit:= G.obedit; 
//      G.obedit:= 0; 
//      if se.flag and SE_SHOW_TEXMESH then
//      begin 
//        se.flag:= se.flag and ( not SE_SHOW_TEXMESH); 
//        setlinestyle(3); 
//        drawcube_size(se.size); 
//        setlinestyle(0); 
//      end;
//      else
//      begin 
//        se.flag:= se.flag or (SE_SHOW_TEXMESH); 
//      end;
//      cpack($505050); 
//      drawmeshwire(ob); 
//      se.flag:= flag; 
//      G.obedit:= obedit; 
//      cpack(col); 
//    end;
//  end;

//  if se.flag and SE_SHOW_TEXMESH then
//  begin 

//    if ob=G.obedit then
//    drawmeshwire(ob); 
//    else
//    if dt=OB_BOUNDBOX then
//    drawboundbox(ob); 
//    else
//    if dt=OB_WIRE then
//    drawmeshwire(ob); 
//    else
//    begin 
//      if (G.f and (G_PICKSEL)) then
//      drawmeshwire(ob); 
//      else
//      if (G.f and (G_BACKBUFSEL)) then
//      drawmeshsolid(ob,0); 
//      else
//      draw_tface_mesh(ob,se.texmesh,dt); 
//    end;
//  end;
//  else
//  begin 

//    if G.f and G_SIMULATION then
//    ; 
//    else
//    if ob<>G.obedit then
//    begin 
//      setlinestyle(3); 
//      drawcube_size(se.size); 
//      setlinestyle(0); 
//      if ob.dt>OB_WIRE then
//      begin 
//        po:= se.portals; 
//        if ob=OBACT then
//        begin 
//          for{while} a:=1 to se.totport { a++}
//          do
//          begin 
//            draw_portal(se,po,a=se.actport); 
//            inc(po); 
//          end;
//        end;
//        else
//        begin 
//          a:= se.totport; 
//          while a{--} dec(a); 
//          do
//          begin 
//            draw_portal(se,po,0); 
//            inc(po); 
//          end;
//        end;
//      end;
//    end;

//    if dt>OB_SOLID then
//    dt:= OB_SOLID; (* shaded bij dynamesh: werkt niet *)
//    if ob=G.obedit then
//    drawmeshwire(ob); 
//    else
//    if dt=OB_BOUNDBOX then
//    drawboundbox(ob); 
//    else
//    if dt=OB_WIRE then
//    drawmeshwire(ob); 
//    else
//    drawDispList(ob,dt); 
//  end;
//end;

(* *************** *)

procedure init_realtime_GL;
begin
  Mat4One(texmat1);
  Mat4One(texmat4);
  Mat4MulFloat3(@texmat1,1.0 / 32767.0);
  Mat4MulFloat3(@texmat4,4.0 / 32767.0);

  glMatrixMode(GL_TEXTURE);
  glLoadMatrixf(@texmat1);
  glMatrixMode(GL_MODELVIEW);

  {$ifdef __sgi}
  glBlendEquationEXT(GL_FUNC_ADD_EXT);
  {$endif}

  (* glPolygonOffset(-0.001, -0.001); *)
end;

//procedure sector_simulate; 
//var
//cam: pCamera;

//params: array [0..Pred(5)] of single; 
//loc: array [0..2] of single; 
//rot: array [0..2] of single; 
//begin
//  cam:=0; 

//  if G.scene.camera=0)or(G.scene.camera.type<>OB_CAMERA then
//  begin 
//    error('no (correct) camera'); 
//    exit;
//  end;
//  VECCOPY(loc,G.scene.camera.loc); 

//  (* soms is de aktieve camera niet in een sector *)
//  VECCOPY(rot,G.scene.camera.rot); 
//  G.f:= G.f or (G_SIMULATION); 
//  G.simulf:= 0; 
//  cam:= G.scene.camera.data; 
//  if G.vd.drawtype>OB_WIRE then
//  begin 
//    if G.scene.world)and((G.scene.world.mode and WO_MIST) then
//    begin 
//      glFogi(GL_FOG_MODE,GL_LINEAR); 
//      glFogf(GL_FOG_DENSITY,0.1); 
//      glFogf(GL_FOG_START,G.scene.world.miststa); 
//      glFogf(GL_FOG_END,G.scene.world.miststa+G.scene.world.mistdist); 
//      params[0]:= G.scene.world.horr; 
//      params[1]:= G.scene.world.horg; 
//      params[2]:= G.scene.world.horb; 
//      params[3]:= 0.0; 
//      glFogfv(GL_FOG_COLOR,params); 
//      glEnable(GL_FOG); 
//    end;
//  end;
//  waitcursor(1); 

//  while LongBool(1)
//  do
//  begin 
//    sector_go(); 
//    if G.simulf and G_LOADFILE then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    if G.simulf and G_RESTART then
//    ; 
//    if G.simulf and G_QUIT then
//    begin 
//      reset_slowparents(); 
//      break; {<= !!!b possible in "switch" - then remove this line}
//      (* editobject.c *)
//    end;
//    G.simulf:= 0; 
//  end;
//  waitcursor(0); 

//  glDisable(GL_FOG); 

//  G.f:= G.f and ( not G_SIMULATION); 
//  allqueue(REDRAWVIEW3D,0); 

//  allqueue(REDRAWBUTSALL,0); 
//  VECCOPY(G.scene.camera.loc,loc); 
//  (* soms is de aktieve camera niet in een sector *)
//  VECCOPY(G.scene.camera.rot,rot); 
//end;

//{$endif}

//{glBindTexture(A,B)     glBindTextureEXT(A,B)}
//function glBindTexture(A: integer; B: integer): integer; 
//begin
//  result:= glBindTextureEXT(A,B)
//end;

//{glGenTextures(A,B)     glGenTexturesEXT(A,B)}
//function glGenTextures(A: integer; B: integer): integer; 
//begin
//  result:= glGenTexturesEXT(A,B)
//end;

//{glDeleteTextures(A,B)  glDeleteTexturesEXT(A,B)}
//function glDeleteTextures(A: integer; B: integer): integer; 
//begin
//  result:= glDeleteTexturesEXT(A,B)
//end;

//{glPolygonOffset(A,B)  glPolygonOffsetEXT(A,B)}
//function glPolygonOffset(A: integer; B: integer): integer; 
//begin
//  result:= glPolygonOffsetEXT(A,B)
//end;

end.
