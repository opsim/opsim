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
(*  blender.c   jan 94     MIXED MODEL
 * 
 * algemene hulp funkties en data
 * 
 *)
unit blender;

interface

uses
ctypes,cfuncs,
util,
library_, arithb, storage;

{ *********** vec *************  }

type
    pvec2s= ^vec2s;
  vec2s = record
      x : smallint;
      y : smallint;
    end;

  vec2i = record
      x : longint;
      y : longint;
    end;

  vec2f = record
      x : single;
      y : single;
    end;

  vec2d = record
      x : double;
      y : double;
    end;

  vec3i = record
      x : longint;
      y : longint;
      z : longint;
    end;

  vec3f = record
      x : single;
      y : single;
      z : single;
    end;

  vec3d = record
      x : double;
      y : double;
      z : double;
    end;

  vec4i = record
      x : longint;
      y : longint;
      z : longint;
      w : longint;
    end;

  vec4f = record
      x : single;
      y : single;
      z : single;
      w : single;
    end;

  vec4d = record
      x : double;
      y : double;
      z : double;
      w : double;
    end;

  prcti= ^rcti;
  rcti = record
      xmin : longint;
      xmax : longint;
      ymin : longint;
      ymax : longint;
    end;

  prctf = ^rctf;
  rctf = record
      xmin : single;
      xmax : single;
      ymin : single;
      ymax : single;
    end;
    
{ *******************************************  }
{ * * * * * * *  L I B R A R Y  * * * * * * *  }
{ *******************************************  }
{ **************** MAIN *********************  }

  pBoundBox = ^BoundBox;
  BoundBox = record
      vec : array[0..7] of array[0..2] of single;
    end;

  pOcInfo = ^OcInfo;
  OcInfo = record
      dvec : array[0..2] of single;
      size : array[0..2] of single;
    end;
{ let op: Sequence heeft identiek begin.  }

pLibrary = ^_Library;

  pID = ^ID;
  ppID = ^pID;
  ID = record
      next : pointer;
      prev : pointer;
      _new : pID;
      lib : pLibrary;
      name : array[0..23] of char;
      us : smallint;
      flag : smallint;
      pad : longint;
    end;
{ tot, idblock en filedata zijn voor lezen en schrijven  }

  _Library = record
      id : ID;
      idblock : pID;
      filedata : pchar;
      name : array[0..159] of char;
      tot : longint;
      pad : longint;
    end;

  pMain = ^Main;
  Main = record
      next : pMain;
      prev : pMain;
      name : array[0..159] of char;
      versionfile : smallint;
      rt : smallint;
      curlib : pLibrary;
      scene : ListBase;
      _library : ListBase;
      _object : ListBase;
      mesh : ListBase;
      curve : ListBase;
      mball : ListBase;
      mat : ListBase;
      tex : ListBase;
      image : ListBase;
      ika : ListBase;
      wave : ListBase;
      latt : ListBase;
      sector : ListBase;
      life : ListBase;
      lamp : ListBase;
      camera : ListBase;
      ipo : ListBase;
      key : ListBase;
      world : ListBase;
      screen : ListBase;
      vfont : ListBase;
      text : ListBase;
    end;
{ **************** IPO *********************  }
  pIpo = ^Ipo;
  Ipo = record
      id : ID;
      curve : ListBase;
      cur : rctf;
      blocktype : smallint;
      showkey : smallint;
      pad : longint;
    end;

  pPath = ^Path;
  Path = record
      len : longint;
      data : psingle;
      totdist : single;
    end;
{ **************** KEY *********************  }

  pKeyBlock = ^KeyBlock;
  KeyBlock = record
      next : pKeyBlock;
      prev : pKeyBlock;
      pos : single;
      flag : smallint;
      totelem : smallint;
      _type : smallint;
      rt : smallint;
      pad : longint;
      data : pointer;
    end;

  pKey = ^Key;
  Key = record
      id : ID;
      refkey : pKeyBlock;
      elemstr : array[0..31] of char;
      elemsize : longint;
      curval : single;
      block : ListBase;
      ipo : pIpo;
      from : pID;
      _type : smallint;
      totkey : smallint;
      slurph : smallint;
      actkey : smallint;
    end;
{ **************** TEXT ***********************  }

  ScriptLink = record
      scripts : ppID;
      flag : psmallint;
      actscript : smallint;
      totscript : smallint;
    end;

  pTextLine = ^TextLine;
  TextLine = record
      next : pTextLine;
      prev : pTextLine;
      line : pchar;
      len : longint;
      blen : longint;
    end;

  Text = record
      id : ID;
      name : pchar;
      flags : longint;
      nlines : longint;
      lines : ListBase;
      curl : pTextLine;
      sell : pTextLine;
      curc : longint;
      selc : longint;
      undo_buf : pbyte;
      undo_pos : longint;
      undo_len : longint;
      compiled : pointer;
    end;
{ **************** CAMERA *********************  }
{ network camera  }

  Camera = record
      id : ID;
      _type : smallint;
      flag : smallint;
      drawzoom : smallint;
      hold : smallint;
      clipsta : single;
      clipend : single;
      netsta : single;
      netend : single;
      lens : single;
      drawsize : single;
      hololen : single;
      hololen1 : single;
      ipo : pIpo;
      scriptlink : ScriptLink;
    end;
{ **************** IMAGE *********************  }
{ texture pagina  }
{ om subregio's te kunnen repeaten  }

  pImage = ^Image;
  Image = record
      id : ID;
      name : array[0..159] of char;
      //anim : panim;
      //ibuf : pImBuf;
      //mipmap : array[0..9] of pImBuf;
      ok : smallint;
      flag : smallint;
      lastframe : smallint;
      lastquality : smallint;
      tpageflag : smallint;
      totbind : smallint;
      xrep : smallint;
      yrep : smallint;
      twsta : smallint;
      twend : smallint;
      bindcode : dword;
      repbind : pdword;
    end;
{ **************** TEX *********************  }
      pObject = ^_Object;
      pTex = ^Tex;

  pMTex = ^MTex;
  MTex = record
      texco : smallint;
      mapto : smallint;
      maptoneg : smallint;
      blendtype : smallint;
      _object : pObject;
      tex : pTex;
      projx : char;
      projy : char;
      projz : char;
      mapping : char;
      ofs : array[0..2] of single;
      size : array[0..2] of single;
      texflag : smallint;
      colormodel : smallint;
      r : single;
      g : single;
      b : single;
      k : single;
      def_var : single;
      colfac : single;
      norfac : single;
      varfac : single;
    end;

  pPluginTex = ^PluginTex;
  PluginTex = record
      name : array[0..159] of char;
      handle : pointer;
      pname : pchar;
      stnames : pchar;
      stypes : longint;
      vars : longint;
      varstr : pointer;
      result : psingle;
      cfra : psingle;
      data : array[0..31] of single;
      doit : function :longint;cdecl;
      callback : procedure ;cdecl;
      version : longint;
      pad : longint;
    end;

  CBData = record
      r : single;
      g : single;
      b : single;
      a : single;
      pos : single;
      cur : longint;
    end;

  pColorBand = ^ColorBand;
  ColorBand = record
      flag : smallint;
      tot : smallint;
      cur : smallint;
      ipotype : smallint;
      data : array[0..15] of CBData;
    end;
{ type ENV_LOAD  }
{ these images are dynamic, not part of the main struct  }

  pEnvMap = ^EnvMap;
  EnvMap = record
      _object : pObject;
      ima : pImage;
      cube : array[0..5] of pImage;
      imat : array[0..3] of array[0..3] of single;
      _type : smallint;
      stype : smallint;
      clipsta : single;
      clipend : single;
      notlay : dword;
      cuberes : longint;
      ok : smallint;
      lastframe : smallint;
    end;

  Tex = record
      id : ID;
      noisesize : single;
      turbul : single;
      bright : single;
      contrast : single;
      rfac : single;
      gfac : single;
      bfac : single;
      filtersize : single;
      noisedepth : smallint;
      noisetype : smallint;
      imaflag : smallint;
      flag : smallint;
      _type : smallint;
      stype : smallint;
      cropxmin : single;
      cropymin : single;
      cropxmax : single;
      cropymax : single;
      xrepeat : smallint;
      yrepeat : smallint;
      extend : smallint;
      len : smallint;
      frames : smallint;
      offset : smallint;
      sfra : smallint;
      fie_ima : smallint;
      norfac : single;
      nor : psingle;
      ipo : pIpo;
      ima : pImage;
      plugin : pPluginTex;
      coba : pColorBand;
      env : pEnvMap;
      fradur : array[0..3] of array[0..1] of smallint;
    end;
{ **************** LAMP *********************  }
{ texact is voor buttons  }

  Lamp = record
      id : ID;
      _type : smallint;
      mode : smallint;
      colormodel : smallint;
      totex : smallint;
      r : single;
      g : single;
      b : single;
      k : single;
      energy : single;
      dist : single;
      spotsize : single;
      spotblend : single;
      haint : single;
      att1 : single;
      att2 : single;
      bufsize : smallint;
      samp : smallint;
      clipsta : single;
      clipend : single;
      shadspotsize : single;
      bias : single;
      soft : single;
      texact : smallint;
      shadhalostep : smallint;
      mtex : array[0..7] of pMTex;
      ipo : pIpo;
      scriptlink : ScriptLink;
    end;
{ **************** WAVE *********************  }

  Wave = record
      id : ID;
      ipo : pIpo;
    end;
{ **************** IKA *********************  }
{ **************** MATERIAL *********************  }
{ LET OP: type veranderen? ook in ipo.h doen  }
{ lay: voor dynamics  }
{ onderstaand is voor buttons en render }

  pMaterial = ^Material;
  ppMaterial = ^pMaterial;
  Material = record
      id : ID;
      colormodel : smallint;
      lay : smallint;
      r : single;
      g : single;
      b : single;
      specr : single;
      specg : single;
      specb : single;
      mirr : single;
      mirg : single;
      mirb : single;
      ambr : single;
      ambb : single;
      ambg : single;
      amb : single;
      emit : single;
      ang : single;
      spectra : single;
      alpha : single;
      ref : single;
      spec : single;
      zoffs : single;
      add : single;
      har : smallint;
      seed1 : char;
      seed2 : char;
      mode : longint;
      flarec : smallint;
      starc : smallint;
      linec : smallint;
      ringc : smallint;
      hasize : single;
      flaresize : single;
      subsize : single;
      flareboost : single;
      rgbsel : char;
      texact : char;
      pr_type : char;
      septex : char;
      texco : smallint;
      mapto : smallint;
      mtex : array[0..7] of pMTex;
      ipo : pIpo;
      ren : pMaterial;
      scriptlink : ScriptLink;
    end;
{ **************** VFont *********************  }

  pVFontData = ^VFontData;
  VFontData = record
      nurbsbase : array[0..255] of ListBase;
      resol : array[0..255] of single;
      width : array[0..255] of single;
      points : array[0..255] of psingle;
      kcount : smallint;
      rt : smallint;
      kern : psmallint;
    end;

  pVFont = ^VFont;
  VFont = record
      id : ID;
      name : array[0..254] of char;
      namenull : char;
      scale : single;
      flag : smallint;
      rt : smallint;
      data : pVFontData;
    end;
{ **************** METABALL *********************  }

  pMetaElem = ^MetaElem;
  MetaElem = record
      next : pMetaElem;
      prev : pMetaElem;
      _type : smallint;
      lay : smallint;
      flag : smallint;
      selcol : smallint;
      x : single;
      y : single;
      z : single;
      expx : single;
      expy : single;
      expz : single;
      rad : single;
      rad2 : single;
      s : single;
      len : single;
      maxrad2 : single;
      pad : longint;
      mat : psingle;
      imat : psingle;
    end;

  MetaBall = record
      id : ID;
      bb : pBoundBox;
      elems : ListBase;
      disp : ListBase;
      ipo : pIpo;
      mat : ppMaterial;
      flag : smallint;
      totcol : smallint;
      texflag : longint;
      loc : array[0..2] of single;
      size : array[0..2] of single;
      rot : array[0..2] of single;
      wiresize : single;
      rendersize : single;
      thresh : single;
    end;
{ **************** CURVE *********************  }

  pBevList = ^BevList;
  BevList = record
      next : pBevList;
      prev : pBevList;
      nr : smallint;
      flag : smallint;
      poly : smallint;
      gat : smallint;
    end;

  BevPoint = record
      x : single;
      y : single;
      z : single;
      alfa : single;
      sina : single;
      cosa : single;
      mat : array[0..2] of array[0..2] of single;
      f1 : smallint;
      f2 : smallint;
    end;

  pBezTriple = ^BezTriple;
  ppBezTriple = ^pBezTriple;
  BezTriple = record
      vec : array[0..2] of array[0..2] of single;
      alfa : single;
      s : array[0..2] of array[0..1] of smallint;
      h1 : smallint;
      h2 : smallint;
      f1 : char;
      f2 : char;
      f3 : char;
      hide : char;
    end;

  pBPoint = ^BPoint;
  BPoint = record
      vec : array[0..3] of single;
      alfa : single;
      s : array[0..1] of smallint;
      f1 : smallint;
      hide : smallint;
    end;

  pNurb = ^Nurb;
  Nurb = record
      next : pNurb;
      prev : pNurb;
      _type : smallint;
      mat_nr : smallint;
      hide : smallint;
      flag : smallint;
      pntsu : smallint;
      pntsv : smallint;
      resolu : smallint;
      resolv : smallint;
      orderu : smallint;
      orderv : smallint;
      flagu : smallint;
      flagv : smallint;
      knotsu : psingle;
      knotsv : psingle;
      bp : pBPoint;
      bezt : pBezTriple;
    end;
{ default  }
{ font stuk  }

  Curve = record
      id : ID;
      bb : pBoundBox;
      nurb : ListBase;
      disp : ListBase;
      bevobj : pObject;
      textoncurve : pObject;
      ipo : pIpo;
      path : pPath;
      key : pKey;
      mat : ppMaterial;
      bev : ListBase;
      orco : psingle;
      loc : array[0..2] of single;
      size : array[0..2] of single;
      rot : array[0..2] of single;
      texflag : longint;
      pathlen : smallint;
      totcol : smallint;
      flag : smallint;
      bevresol : smallint;
      width : single;
      ext1 : single;
      ext2 : single;
      resolu : smallint;
      resolv : smallint;
      len : smallint;
      lines : smallint;
      pos : smallint;
      spacemode : smallint;
      spacing : single;
      linedist : single;
      shear : single;
      fsize : single;
      xof : single;
      yof : single;
      str : pchar;
      family : array[0..23] of char;
      vfont : pVFont;
    end;
{ **************** IPOCURVE (Ipo staat bovenin) *********************  }

  pIpoCurve = ^IpoCurve;
  IpoCurve = record
      next : pIpoCurve;
      prev : pIpoCurve;
      bp : pBPoint;
      bezt : pBezTriple;
      maxrct : rctf;
      totrct : rctf;
      blocktype : smallint;
      adrcode : smallint;
      vartype : smallint;
      totvert : smallint;
      ipo : smallint;
      extrap : smallint;
      flag : smallint;
      rt : smallint;
      ymin : single;
      ymax : single;
      bitmask : dword;
      curval : single;
    end;
{ **************** MESH *********************  }

  MFace = record
      v1 : word;
      v2 : word;
      v3 : word;
      v4 : word;
      puno : char;
      mat_nr : char;
      edcode : char;
      flag : char;
    end;

  MFaceInt = record
      v1 : longint;
      v2 : longint;
      v3 : longint;
      v4 : longint;
      puno : char;
      mat_nr : char;
      edcode : char;
      flag : char;
    end;

  pMVert = ^MVert;
  MVert = record
      co : array[0..2] of single;
      no : array[0..2] of smallint;
      flag : char;
      mat_nr : char;
    end;

  pMCol = ^MCol;
  MCol = record
      a : char;
      r : char;
      g : char;
      b : char;
    end;

  pMSticky = ^MSticky;
  MSticky = record
      co : array[0..1] of single;
    end;
{ niet in file  }

  pMesh = ^Mesh;
  Mesh = record
      id : ID;
      bb : pBoundBox;
      effect : ListBase;
      disp : ListBase;
      ipo : pIpo;
      key : pKey;
      mat : ppMaterial;
      mface : pointer;
      dface : pointer;
      tface : pointer;
      mvert : pMVert;
      mcol : pMCol;
      msticky : pMSticky;
      texcomesh : pMesh;
      orco : psingle;
      oc : pOcInfo;
      totvert : longint;
      totface : longint;
      texflag : longint;
      loc : array[0..2] of single;
      size : array[0..2] of single;
      rot : array[0..2] of single;
      smoothresh : smallint;
      flag : smallint;
      subdiv : smallint;
      totcol : smallint;
      cubemapsize : single;
      rtf : single;
    end;
{ **************** LATTICE *********************  }

  Lattice = record
      id : ID;
      pntsu : smallint;
      pntsv : smallint;
      pntsw : smallint;
      flag : smallint;
      typeu : char;
      typev : char;
      typew : char;
      _type : char;
      pad : longint;
      def : pBPoint;
      ipo : pIpo;
      key : pKey;
    end;
{ **************** OBJECT *********************  }
{ kunnen vertexnrs zijn  }
{ rot en drot moeten achterelkaar! (transform('r' en 's'))  }
{ voor bij render, tijdens simulate, tijdelijk: ipokeys van transform   }
{ kopie van Base  }
{ kopie van Base  }
{ nul==van obdata  }
{ ipowin: blocktype laatste ipowindow  }
{ tijdens realtime  }
{ kopie van mesh of curve of meta  }

  _Object = record
      id : ID;
      _type : smallint;
      partype : smallint;
      par1 : longint;
      par2 : longint;
      par3 : longint;
      parent : pObject;
      track : pObject;
      ipo : pIpo;
      path : pPath;
      bb : pBoundBox;
      data : pointer;
      effect : ListBase;
      network : ListBase;
      disp : ListBase;
      mat : ppMaterial;
      loc : array[0..2] of single;
      dloc : array[0..2] of single;
      orig : array[0..2] of single;
      size : array[0..2] of single;
      dsize : array[0..2] of single;
      rot : array[0..2] of single;
      drot : array[0..2] of single;
      obmat : array[0..3] of array[0..3] of single;
      parentinv : array[0..3] of array[0..3] of single;
      imat : array[0..3] of array[0..3] of single;
      lay : dword;
      flag : smallint;
      colbits : smallint;
      transflag : char;
      ipoflag : char;
      trackflag : char;
      upflag : char;
      ipowin : smallint;
      infoend : smallint;
      infoflag : smallint;
      infostart : smallint;
      dupon : smallint;
      dupoff : smallint;
      dupsta : smallint;
      dupend : smallint;
      sf : single;
      ctime : single;
      dfras : word;
      rt : word;
      dt : char;
      dtx : char;
      totcol : char;
      actcol : char;
      scriptlink : ScriptLink;
    end;
{ **************** WORLD *********************  }

  pWorld = ^World;
  World = record
      id : ID;
      colormodel : smallint;
      totex : smallint;
      texact : smallint;
      mistype : smallint;
      horr : single;
      horg : single;
      horb : single;
      hork : single;
      zenr : single;
      zeng : single;
      zenb : single;
      zenk : single;
      ambr : single;
      ambg : single;
      ambb : single;
      ambk : single;
      fastcol : dword;
      exposure : single;
      skytype : smallint;
      mode : smallint;
      misi : single;
      miststa : single;
      mistdist : single;
      misthi : single;
      starr : single;
      starg : single;
      starb : single;
      stark : single;
      starsize : single;
      starmindist : single;
      stardist : single;
      starcolnoise : single;
      dofsta : smallint;
      dofend : smallint;
      dofmin : smallint;
      dofmax : smallint;
      pad : longint;
      ipo : pIpo;
      mtex : array[0..7] of ^MTex;
      scriptlink : ScriptLink;
    end;
{ **************** SCENE *********************  }
{ bit 0 en 1: limits laten zien  }
{ patch en elem limits  }
{ voor afbeelden  }

  pRadio = ^Radio;
  Radio = record
      hemires : smallint;
      maxiter : smallint;
      drawtype : smallint;
      flag : smallint;
      subshootp : smallint;
      subshoote : smallint;
      nodelim : smallint;
      maxsublamp : smallint;
      pama : smallint;
      pami : smallint;
      elma : smallint;
      elmi : smallint;
      maxnode : longint;
      convergence : single;
      radfac : single;
      gamma : single;
    end;
{ hoe gaat tijd gedefinieerd worden?  }
{ plaatjes  }
{ hiermee rekenen?  }
{ size in %, max in Kb  }
{ uit buttons  }

  RenderData = record
      cfra : smallint;
      sfra : smallint;
      efra : smallint;
      images : smallint;
      framapto : smallint;
      flag : smallint;
      ctime : single;
      framelen : single;
      blurfac : single;
      size : smallint;
      maximsize : smallint;
      xsch : smallint;
      ysch : smallint;
      xasp : smallint;
      yasp : smallint;
      xparts : smallint;
      yparts : smallint;
      safety : rctf;
      border : rctf;
      winpos : smallint;
      planes : smallint;
      imtype : smallint;
      bufflag : smallint;
      quality : smallint;
      scemode : smallint;
      mode : smallint;
      alphamode : smallint;
      dogamma : smallint;
      osa : smallint;
      frs_sec : smallint;
      pad : smallint;
      backbuf : array[0..159] of char;
      pic : array[0..159] of char;
      ftype : array[0..159] of char;
      movie : array[0..159] of char;
    end;

  pBase = ^Base;
  Base = record
      next : pBase;
      prev : pBase;
      lay : dword;
      selcol : dword;
      flag : longint;
      sx : smallint;
      sy : smallint;
      _object : pObject;
    end;
{ enkele realtime vars  }

  pScene = ^Scene;
  Scene = record
      id : ID;
      camera : pObject;
      world : pWorld;
      _set : pScene;
      ima : pImage;
      base : ListBase;
      basact : pBase;
      cursor : array[0..2] of single;
      lay : dword;
      grav : single;
      rtf : single;
      maxdrawsector : smallint;
      flag : smallint;
      pad : longint;
      ed : pointer;
      radio : pRadio;
      r : RenderData;
      scriptlink : ScriptLink;
    end;
{ *******************************************  }
{ * * * * * * *  SPACE STRUCTS  * * * * * * *  }
{ *******************************************  }

  BGpic = record
      ima : pImage;
      tex : pTex;
      xof : single;
      yof : single;
      size : single;
      zoom : single;
      blend : single;
      xim : smallint;
      yim : smallint;
      rect : pdword;
    end;

  {undef near }
  {undef far }
  {define near clipsta }
  {define far clipend }
  { 	single lens, grid, clipsta, clipend;  }
  { moeten achter elkaar blijven staan ivm als pointer doorgeven  }

  pView3D = ^View3D;
    View3D = record
        next : ^View3D;
        prev : ^View3D;
        spacetype : longint;
        viewmat : array[0..3] of array[0..3] of single;
        viewinv : array[0..3] of array[0..3] of single;
        persmat : array[0..3] of array[0..3] of single;
        persinv : array[0..3] of array[0..3] of single;
        viewquat : array[0..3] of single;
        dist : single;
        persp : smallint;
        view : smallint;
        drawtype : smallint;
        localview : smallint;
        lay : longint;
        layact : longint;
        scenelock : smallint;
        around : smallint;
        camzoom : smallint;
        flag : smallint;
        lens : single;
        grid : single;
        _near : single;
        _far : single;
        ofs : array[0..2] of single;
        cursor : array[0..2] of single;
        camera : pObject;
        mx : smallint;
        my : smallint;
        mxo : smallint;
        myo : smallint;
        pr_xmin : smallint;
        pr_xmax : smallint;
        pr_ymin : smallint;
        pr_ymax : smallint;
        pr_sizex : smallint;
        pr_sizey : smallint;
        gridlines : smallint;
        viewbut : smallint;
        pr_facx : single;
        pr_facy : single;
        bgpic : ^BGpic;
        localvd : ^View3D;
      end;

  {undef near }
  {undef far }
  {define near bnearsub }
  {define far bfarsub }

    pView2D = ^View2D;
    View2D = record
        tot : rctf;
        cur : rctf;
        vert : rcti;
        hor : rcti;
        mask : rcti;
        min : array[0..1] of single;
        max : array[0..1] of single;
        minzoom : single;
        maxzoom : single;
        scroll : smallint;
        keeptot : smallint;
        keepaspect : smallint;
        keepzoom : smallint;
      end;

    pSpaceIpo = ^SpaceIpo;
    SpaceIpo = record
        next : pSpaceIpo;
        prev : pSpaceIpo;
        spacetype : longint;
        rowbut : dword;
        v2d : View2D;
        editipo : pointer;
        ipokey : ListBase;
        ipo : pIpo;
        from : pID;
        totipo : smallint;
        lock : smallint;
        butofs : smallint;
        channel : smallint;
        showkey : smallint;
        blocktype : smallint;
        menunr : smallint;
        rt : smallint;
        tot : rctf;
      end;
  { texnr en menunr moeten shorts blijven  }
  { temps  }
  { preview render  }

    pSpaceButs = ^SpaceButs;
    SpaceButs = record
        next : pSpaceButs;
        prev : pSpaceButs;
        spacetype : longint;
        pad1 : longint;
        v2d : View2D;
        mainb : smallint;
        menunr : smallint;
        lock : smallint;
        mainbo : smallint;
        lockpoin : pointer;
        texnr : smallint;
        texfrom : char;
        rt2 : char;
        rectx : smallint;
        recty : smallint;
        rect : pdword;
        cury : smallint;
        modeltype : smallint;
        scriptblock : smallint;
        pad2 : smallint;
      end;

    pSpaceSeq = ^SpaceSeq;
    SpaceSeq = record
        next : pSpaceSeq;
        prev : pSpaceSeq;
        spacetype : longint;
        pad1 : longint;
        v2d : View2D;
        mainb : smallint;
        zoom : smallint;
        pad2 : longint;
      end;

    retfunc = procedure(name: pchar);

    pSpaceFile = ^SpaceFile;
    SpaceFile = record
        next : pSpaceFile;
        prev : pSpaceFile;
        spacetype : longint;
        pad : longint;
        filelist : pdirentry;
        totfile : longint;
        title : array[0..23] of char;
        dir : array[0..159] of char;
        _file : array[0..79] of char;
        _type : smallint;
        ofs : smallint;
        flag : smallint;
        sort : smallint;
        maxnamelen : smallint;
        collums : smallint;
        libfiledata : pchar;
        retval : smallint;
        ipotype : smallint;
        menu : smallint;
        act : smallint;
        returnfunc : retfunc;
      end;

    pSpaceOops = ^SpaceOops;
    SpaceOops = record
        next : pSpaceOops;
        prev : pSpaceOops;
        spacetype : longint;
        pad : longint;
        v2d : View2D;
        oops : ListBase;
        lock : smallint;
        visiflag : smallint;
        flag : smallint;
        rt : smallint;
        lockpoin : pointer;
      end;

    pSpaceImage = ^SpaceImage;
    SpaceImage = record
        next : pSpaceImage;
        prev : pSpaceImage;
        spacetype : longint;
        pad : longint;
        v2d : View2D;
        image : pImage;
        mode : smallint;
        zoom : smallint;
        imanr : smallint;
        curtile : smallint;
        xof : smallint;
        yof : smallint;
        flag : smallint;
        rt : smallint;
      end;

    pSpaceText = ^SpaceText;
    SpaceText = record
        next : pSpaceText;
        prev : pSpaceText;
        spacetype : longint;
        left : longint;
        text : pText;
        top : longint;
        viewlines : longint;
        flags : smallint;
        menunr : smallint;
        font_id : longint;
        lheight : longint;
        pix_per_line : single;
        txtscroll : rcti;
        txtbar : rcti;
        py_draw : pointer;
        py_event : pointer;
        py_button : pointer;
        py_head_draw : pointer;
        py_head_event : pointer;
        py_head_button : pointer;
      end;
  { SpaceImaSel in aparte headerfile  }
  { SpacePaint in aparte headerfile  }
  { ************* GLOBAL *******************  }

    dl_GourFace = record
        co : array[0..2] of array[0..2] of single;
        col : array[0..5] of dword;
      end;

    FastLamp = record
        _type : smallint;
        mode : smallint;
        lay : smallint;
        rt : smallint;
        co : array[0..2] of single;
        vec : array[0..2] of single;
        dist : single;
        distkw : single;
        att1 : single;
        att2 : single;
        spotsi : single;
        spotbl : single;
        r : single;
        g : single;
        b : single;
      end;
  { rt wordt gebruikt door initrenderNurbs  }

  pDispList = ^DispList;
    DispList = record
        next : pDispList;
        prev : pDispList;
        _type : smallint;
        flag : smallint;
        parts : longint;
        nr : longint;
        col : smallint;
        rt : smallint;
        verts : psingle;
        nors : psingle;
        index : plongint;
        col1 : pdword;
        col2 : pdword;
      end;

    UserDef = record
        flag : smallint;
        dupflag : smallint;
        savetime : longint;
        tempdir : array[0..63] of char;
        fontdir : array[0..63] of char;
        renderdir : array[0..63] of char;
        textudir : array[0..63] of char;
        plugseqdir : array[0..63] of char;
        versions : smallint;
        rt : smallint;
      end;
  { active pointers  }
  { denk aan file.c  }
  { fonts, allocated global data  }
  { strings: lastsaved  }
  { totalen  }
  { realtime  }
  { variabelen van frank  }
  { special versions  }

    //Global = record
    //    vd : pView3D;
    //    v2d : pView2D;
    //    sipo : pSpaceIpo;
    //    buts : pSpaceButs;
    //    sima : pSpaceImage;
    //    soops : pSpaceOops;
    //    stext : pSpaceText;
    //    main : pMain;
    //    scene : pScene;
    //    curscreen : pbScreen;
    //    obedit : pObject;
    //    font : pointer;
    //    fonts : pointer;
    //    fontss : pointer;
    //    mainbase : ListBase;
    //    ima : array[0..159] of char;
    //    sce : array[0..159] of char;
    //    lib : array[0..159] of char;
    //    psx : array[0..159] of char;
    //    totobj : smallint;
    //    totlamp : smallint;
    //    totobjsel : smallint;
    //    totcurve : smallint;
    //    totmesh : smallint;
    //    totmat : smallint;
    //    totvert : longint;
    //    totface : longint;
    //    totvertsel : longint;
    //    totfacesel : longint;
    //    time : longint;
    //    cputime : longint;
    //    machine : smallint;
    //    afbreek : smallint;
    //    f : smallint;
    //    moving : smallint;
    //    colact : smallint;
    //    zbuf : smallint;
    //    qual : smallint;
    //    background : smallint;
    //    imagewin : smallint;
    //    animspeed : smallint;
    //    version : smallint;
    //    versionfile : smallint;
    //    simulf : smallint;
    //    fields : smallint;
    //    order : smallint;
    //    rt : smallint;
    //    edve : ListBase;
    //    eded : ListBase;
    //    edvl : ListBase;
    //    textcurs : array[0..3] of array[0..1] of single;
    //    cursector : pSector;
    //    sectorbuf : array[0..(SE_MAXBUF)-1] of ^Sector;
    //    lifebuf : array[0..(LF_MAXBUF)-1] of pObject;
    //    actvarar : psmallint;
    //    dfra : longint;
    //    dfrao : longint;
    //    dfras : smallint;
    //    totsect : smallint;
    //    maxsect : smallint;
    //    totlife : smallint;
    //    renderd : longint;
    //    real_sfra : longint;
    //    real_efra : longint;
    //    save_over : longint;
    //    special1 : smallint;
    //    special2 : smallint;
    //  end;

procedure duplicatelist(list1: pListBase;  list2: pListBase);
procedure initglobals;
procedure addlisttolist(list1: pListBase;  list2: pListBase);

var
matone: Mat4 = ((1,0,0,0),(0,1,0,0),(0,0,1,0),(0,0,0,1));
U: UserDef;

implementation

uses
blendef,
blenglob, object_;

var
versionstr: array [0..47] of char = (#14, #92, #240, #247, #43, #199, #126, #71, #69, #113, #64, #241, #115, #139, #242, #216, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0, #0);

versionfstr: array [0..23] of char; (* voor files *)

(* ********** vrijgeven ********** *)

//procedure free_blender;
//var
//main: pMain;
//begin

//  freestructDNA_all();
//  free_mainlist();
//  (* genfile.c *)
//  freeImBufdata();
//  (* library.c *)
//end;(* imbuf lib *)

procedure addlisttolist(list1: pListBase;  list2: pListBase);
begin
  if list2^.first=nil then
  exit;

  if list1^.first=nil then
  begin
    list1^.first:= list2^.first;
    list1^.last:= list2^.last;
  end
  else
  begin
    pLink(list1^.last)^.next:=list2^.first;
    pLink(list2^.first)^.prev:=list1^.last;
    list1^.last:= list2^.last;
  end;
  list2^.first:= nil;
  list2^.last:=nil;
end;

function dupallocN(mem: pointer): pointer;
var
memh: pMemHead;
new: pinteger;
begin
  if mem=nil then
       exit(nil);

  memh:= pMemHead(mem);
  dec(memh);
  if memh^.len=0
 then
  begin
    printf('error: dupallocN with len==0 %s\n',[memh^.name]);
      exit(nil);
  end
  else
  begin
    new:= mallocN(memh^.len,'dupli_alloc');
    memcpy(new,mem,memh^.len);
      exit(new);
  end;
end;

//function alloc_len(mem: pinteger): integer;
//var
//memh: pMemHead;
//begin

//  if mem=0 then
//    exit(0);
//  memh:=  {pMemHead(}mem;
//  dec(memh);
//  begin
//    result:= memh.len;
//    exit;
//  end;
//end;

(* kopie van 2 naar 1 *)

procedure duplicatelist(list1: pListBase;  list2: pListBase);
var
   link1, link2: pLink;
begin
                     list1^.last:=nil;
  list1^.first:=list1^.last ;
  link2:= list2^.first;
  while link2 <>nil do
  begin
    link1:= dupallocN(link2);
    addtail(list1,link1);
    link2:= link2^.next;
  end;
end;

procedure initglobals;
begin
  bzero(@G,sizeof(Global));

  G.animspeed:= 4;

  G.main:= callocN(sizeof(Main),'initglobals');
  addtail(@G.mainbase,G.main);
  strcpy(G.ima,'//');
  strcpy(G.psx,'//');

  G.version:= 172;

  G.order:= 1;

  sprintf(versionfstr,'BLENDER  %d',[G.version]);

  if pchar(@G.order + 1)^<>#0 then
  begin
    G.order:= B_ENDIAN;
    versionfstr[9]:= 'V';
  end
  else
  begin
    G.order:= L_ENDIAN;
    versionfstr[9]:= 'v';
  end;

  if sizeof(pointer)=8 then
  versionfstr[8]:= '-'
  else
  versionfstr[8]:= '_';

  sprintf(@versionstr[16],'%d',[G.version]);

  {$ifdef __sgi}
  initmoviepointers();   (* define in iff.h *)
  {$endif}

  clear_workob(); (* object.c *)
end;

end.
