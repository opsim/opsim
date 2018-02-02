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

 * fromtype1 -
 *  Convert an Adobe type 1 font into .of or .sf format.
 *
 *    Paul Haeberli - 1990
 *)

unit psfont;

interface

implementation

uses
blender, objfnt;

const
SKIP = 4; 
LINELEN = 2048; 
NOTHEX = (100); 
MC1 = 52845; 
MC2 = 22719; 
MAXSUBRS = 1000; 
MAXCHARS = 1000; 
MAXTRIES = 30; 

(* postscript commands *)
HSTEM = (1); 
VSTEM = (3); 
VMOVETO = (4); 
RLINETO = (5); 
HLINETO = (6); 
VLINETO = (7); 
RRCURVETO = (8); 
CLOSEPATH = (9); 
CALLSUBR = (10); 
RETURN = (11); 
HSBW = (13); 
ENDCHAR = (14); 
RMOVETO = (21); 
HMOVETO = (22); 
VHCURVETO = (30); 
HVCURVETO = (31); 
DOTSECTION = (256+0); 
VSTEM3 = (256+1); 
HSTEM3 = (256+2); 
SEAC = (256+6); 
SBW = (256+7); 
_DIV = (256+12); 
CALLOTHERSUBR = (256+16); 
POP = (256+17); 
SETCURRENTPOINT = (256+33); 
WHAT0 = (0); 

var
oneline: array [0..Pred(LINELEN)] of char; 
fnt: pobjfnt; 
mr: word; 
bindat: pchar; 
datbytes: integer; 
firsted: integer; 
chardata: array [0..Pred(2000)] of smallint; 
nshorts: integer; 
thecharwidth: integer; 
thesidebearing: integer; 
npnts: integer; 
nloops: integer; 
nvertpos: integer; 
fakepos: integer; 
fakemax: integer; 
beztol: single = 100.0;
(* extern: uit de libfm *)

my_subrs: array [0..Pred(MAXSUBRS)] of pchar; 
my_sublen: array [0..Pred(MAXSUBRS)] of cardinal;
my_chars: array [0..Pred(MAXCHARS)] of pchar; 
my_charlen: array [0..Pred(MAXCHARS)] of cardinal;
my_charname: array [0..Pred(MAXCHARS)] of pchar; 
my_nsubrs: integer; 
my_nchars: integer; 
sidebearing: array [0..Pred(MAXCHARS)] of smallint; 
tok: array [0..Pred(LINELEN)] of char; 
//inf: pFILE; 
sp_npnts: integer; 
sp_nloops: integer; 
(* 
 * interpreter globals
 *)


mat: array [0..Pred(2),0..Pred(2)] of single;
pcstack: array [0..Pred(100)] of pchar; 
pc: pchar; 
pcsp: integer; 
coordpos: integer; 
coordsave: array [0..Pred(7),0..Pred(2)] of integer; 
incusp: integer; 
retstack: array [0..Pred(1000)] of integer; 
retsp: integer; 
stack: array [0..Pred(1000)] of integer; 
sp: integer; 
savesplines: integer = 1; 

// ISOcharlist: array [0..NASCII-1] of pschar = (
//	'/space', 		040,	0,
//	    '/exclam', 		041,	0,
//	    '/quotedbl', 	042,	0,
//	    '/numbersign', 	043,	0,
//	    '/dollar', 		044,	0,
//	    '/percent', 	045,	0,
//	    '/ampersand', 	046,	0,
//	    '/quoteright', 	047,	0,
//
//	    '/parenleft', 	050,	0,
//	    '/parenright', 	051,	0,
//	    '/asterisk', 	052,	0,
//	    '/plus', 		053,	0,
//	    '/comma', 		054,	0,
//	    '/hyphen', 		055,	0,
//	    '/period', 		056,	0,
//	    '/slash', 		057,	0,
//
//	    '/zero', 		060,	0,
//	    '/one', 		061,	0,
//	    '/two', 		062,	0,
//	    '/three', 		063,	0,
//	    '/four', 		064,	0,
//	    '/five', 		065,	0,
//	    '/six', 		066,	0,
//	    '/seven', 		067,	0,
//
//	    '/eight', 		070,	0,
//	    '/nine', 		071,	0,
//	    '/colon', 		072,	0,
//	    '/semicolon', 	073,	0,
//	    '/less',	 	074,	0,
//	    '/equal',	 	075,	0,
//	    '/greater',	 	076,	0,
//	    '/question', 	077,	0,
//
//	    '/at',	 	0100,	0,
//	    '/A',	 	0101,	0,
//	    '/B',	 	0102,	0,
//	    '/C',	 	0103,	0,
//	    '/D',	 	0104,	0,
//	    '/E',	 	0105,	0,
//	    '/F',	 	0106,	0,
//	    '/G',	 	0107,	0,
//
//	    '/H',	 	0110,	0,
//	    '/I',	 	0111,	0,
//	    '/J',	 	0112,	0,
//	    '/K',	 	0113,	0,
//	    '/L',	 	0114,	0,
//	    '/M',	 	0115,	0,
//	    '/N',	 	0116,	0,
//	    '/O',	 	0117,	0,
//
//	    '/P',	 	0120,	0,
//	    '/Q',	 	0121,	0,
//	    '/R',	 	0122,	0,
//	    '/S',	 	0123,	0,
//	    '/T',	 	0124,	0,
//	    '/U',	 	0125,	0,
//	    '/V',	 	0126,	0,
//	    '/W',	 	0127,	0,
//
//	    '/X',	 	0130,	0,
//	    '/Y',	 	0131,	0,
//	    '/Z',	 	0132,	0,
//	    '/bracketleft', 	0133,	0,
//	    '/backslash',	0134,	0,
//	    '/bracketright', 	0135,	0,
//	    '/asciicircum',	0136,	0,
//	    '/underscore', 	0137,	0,
//
//	    '/quoteleft', 	0140,	0,
//	    '/a',	 	0141,	0,
//	    '/b',	 	0142,	0,
//	    '/c', 		0143,	0,
//	    '/d',		0144,	0,
//	    '/e', 		0145,	0,
//	    '/f',		0146,	0,
//	    '/g',	 	0147,	0,
//
//	    '/h',	 	0150,	0,
//	    '/i',	 	0151,	0,
//	    '/j',	 	0152,	0,
//	    '/k', 		0153,	0,
//	    '/l',		0154,	0,
//	    '/m', 		0155,	0,
//	    '/n',		0156,	0,
//	    '/o',	 	0157,	0,
//
//	    '/p',	 	0160,	0,
//	    '/q',	 	0161,	0,
//	    '/r',	 	0162,	0,
//	    '/s', 		0163,	0,
//	    '/t',		0164,	0,
//	    '/u',		0165,	0,
//	    '/v', 		0166,	0,
//	    '/w',		0167,	0,
//
//	    '/x',	 	0170,	0,
//	    '/y',	 	0171,	0,
//	    '/z',	 	0172,	0,
//	    '/braceleft', 	0173,	0,
//	    '/bar',		0174,	0,
//	    '/braceright',	0175,	0,
//	    '/asciitilde', 	0176,	0,
//	    '/',		0177,	0,
//
//
//	    /* nonstandard defs */
//
//	'/quotedblleft',	0200,	0,
//	    '/quotedblright',	0201,	0,
//	    '/quotedblbase',	0202,	0,
//	    '/quotesinglbase',	0203,	0,
//	    '/guilsinglleft',	0204,	0,
//	    '/guilsinglright',	0205,	0,
//	    '/endash',		0206,	0,
//	    '/dagger',		0207,	0,
//
//	    '/daggerdbl',	0210,	0,
//	    '/trademark',	0211,	0,
//	    '/bullet',		0212,	0,
//	    '/perthousand',	0213,	0,
//	    '/Lslash',		0214,	0,
//	    '/OE',		0215,	0,
//	    '/lslash',		0216,	0,
//	    '/oe',		0217,	0,
//
//	    /* endnonstandard defs */
//
//	'/dotlessi',	0220,	0,
//	    '/grave',		0221,	0,
//	    '/acute',		0222,	0,
//	    '/circumflex',	0223,	0,
//	    '/tilde',		0224,	0,
//	    '/',		0225,	0,
//	    '/breve',		0226,	0,
//	    '/dotaccent',	0227,	0,
//
//	    '/',		0230,	0,
//	    '/',		0231,	0,
//	    '/ring',		0232,	0,
//	    '/',		0233,	0,
//	    '/',		0234,	0,
//	    '/hungarumlaut',	0235,	0,
//	    '/ogonek',		0236,	0,
//	    '/caron',		0237,	0,
//
//	    '/',		0240,	0,
//	    '/exclamdown',	0241,	0,
//	    '/cent',		0242,	0,
//	    '/sterling',	0243,	0,
//	    '/florin',		0244,	0,
//	    '/yen',		0245,	0,
//	    '/brokenbar',	0246,	0,
//	    '/section',		0247,	0,
//
//	    '/dieresis',	0250,	0,
//	    '/copyright',	0251,	0,
//	    '/ordfeminine',	0252,	0,
//	    '/guillemotleft',	0253,	0,
//	    '/logicalnot',	0254,	0,
//	    '/hyphen',		0255,	0,
//	    '/registered',	0256,	0,
//	    '/macron',		0257,	0,
//
//	    '/degree',		0260,	0,
//	    '/plusminus',	0261,	0,
//	    '/twosuperior',	0262,	0,
//	    '/threesuperior',	0263,	0,
//	    '/acute',		0264,	0,
//	    '/mu',		0265,	0,
//	    '/paragraph',	0266,	0,
//	    '/periodcentered',	0267,	0,
//
//	    '/cedilla',		0270,	0,
//	    '/onesuperior',	0271,	0,
//	    '/ordmasculine',	0272,	0,
//	    '/guillemotright',	0273,	0,
//	    '/onequarter',	0274,	0,
//	    '/onehalf',		0275,	0,
//	    '/threequarters',	0276,	0,
//	    '/questiondown',	0277,	0,
//
//	    '/Agrave',		0300,	0,
//	    '/Aacute',		0301,	0,
//	    '/Acircumflex',	0302,	0,
//	    '/Atilde',		0303,	0,
//	    '/Adieresis',	0304,	0,
//	    '/Aring',		0305,	0,
//	    '/AE',		0306,	0,
//	    '/Ccedilla',	0307,	0,
//
//	    '/Egrave',		0310,	0,
//	    '/Eacute',		0311,	0,
//	    '/Ecircumflex',	0312,	0,
//	    '/Edieresis',	0313,	0,
//	    '/Igrave',		0314,	0,
//	    '/Iacute',		0315,	0,
//	    '/Icircumflex',	0316,	0,
//	    '/Idieresis',	0317,	0,
//
//	    '/Eth',		0320,	0,
//	    '/Ntilde',		0321,	0,
//	    '/Ograve',		0322,	0,
//	    '/Oacute',		0323,	0,
//	    '/Ocircumflex',	0324,	0,
//	    '/Otilde',		0325,	0,
//	    '/Odieresis',	0326,	0,
//	    '/multiply',	0327,	0,
//
//	    '/Oslash',		0330,	0,
//	    '/Ugrave',		0331,	0,
//	    '/Uacute',		0332,	0,
//	    '/Ucircumflex',	0333,	0,
//	    '/Udieresis',	0334,	0,
//	    '/Yacute',		0335,	0,
//	    '/Thorn',		0336,	0,
//	    '/germandbls',	0337,	0,
//
//	    '/agrave',		0340,	0,
//	    '/aacute',		0341,	0,
//	    '/acircumflex',	0342,	0,
//	    '/atilde',		0343,	0,
//	    '/adieresis',	0344,	0,
//	    '/aring',		0345,	0,
//	    '/ae',		0346,	0,
//	    '/ccedilla',	0347,	0,
//
//	    '/egrave',		0350,	0,
//	    '/eacute',		0351,	0,
//	    '/ecircumflex',	0352,	0,
//	    '/edieresis',	0353,	0,
//	    '/igrave',		0354,	0,
//	    '/iacute',		0355,	0,
//	    '/icircumflex',	0356,	0,
//	    '/idieresis',	0357,	0,
//
//	    '/eth',		0360,	0,
//	    '/ntilde',		0361,	0,
//	    '/ograve',		0362,	0,
//	    '/oacute',		0363,	0,
//	    '/ocircumflex',	0364,	0,
//	    '/otilde',		0365,	0,
//	    '/odieresis',	0366,	0,
//	    '/divide',		0367,	0,
//
//	    '/oslash',		0370,	0,
//	    '/ugrave',		0371,	0,
//	    '/uacute',		0372,	0,
//	    '/ucircumflex',	0373,	0,
//	    '/udieresis',	0374,	0,
//	    '/yacute',		0375,	0,
//	    '/thorn',		0376,	0,
//	    '/ydieresis',	0377,	0,
//};
//
//
//short STDvsISO [][2] = {
//	0341, 0306, /* AE */
//	0351, 0330, /* Oslash */
//	0302, 0222, /* acute */
//	0361, 0346, /* ae */
//	0306, 0226, /* breve */
//	0317, 0237, /* caron */
//	0313, 0270, /* cedilla */
//	0303, 0223, /* circumflex */
//	0250, 0244, /* currency */
//	0310, 0250, /* dieresis */
//	0307, 0227, /* dotaccent */
//	0365, 0220, /* dotlessi */
//	0373, 0337, /* germandbls */
//	0301, 0221, /* grave */
//	0315, 0235, /* hungarumlaut */
//	0055, 0255, /* hyphen */
//	0305, 0257, /* macron */
//	0316, 0236, /* ogenek */
//	0343, 0252, /* ordfeminine */
//	0353, 0272, /* ordmasculine */
//	0371, 0370, /* oslash */
//	0264, 0267, /* periodcentered */
//	0312, 0232, /* ring */
//	0304, 0224, /* tilde */
//};

(* from objfont.c de rest zit in lfm_s !!*)

(* START 5.2 *)

//applytocharverts(objfnt*fnt,intc,int(*func)()); 
//begin 
//  index: integer; 
//  cd: pchardesc; 
//  sptr: psmallint; 
//  nverts: integer; 
//  index:= chartoindex(fnt,c); 
//  if index<0
//  then
//  exit;
//  cd:= fnt.my_chars+index; 
//  if cd.data<>0{nil} {<= !!!9} 
//  then
//  begin 
//    sptr:= cd.data; 
//    case fnt.type of
//      TM_TYPE:
//      begin
//        while 1
//        do
//        begin 
//          case {*}sptr^{++} inc(sptr);  of
//            TM_BGNTMESH,
//            TM_SWAPTMESH,
//            TM_ENDBGNTMESH:
//            begin
//            end;
//            TM_RETENDTMESH,
//            TM_RET:
//            begin
//              exit;
//            end;
//          end;{case?}
//          nverts:= {*}sptr^{++} inc(sptr); ; 
//          while nverts{--} dec(nverts); 
//          do
//          begin 
//            {!!!a type cast? =>} {func(}(sptr); 
//            sptr:= sptr + (2); 
//          end;
//        end;
//      end;
//      PO_TYPE:
//      begin
//        while 1
//        do
//        begin 
//          case {*}sptr^{++} inc(sptr);  of
//            PO_BGNLOOP,
//            PO_ENDBGNLOOP:
//            begin
//            end;
//            PO_RETENDLOOP,
//            PO_RET:
//            begin
//              exit;
//            end;
//            
//          end;{case?}
//          nverts:= {*}sptr^{++} inc(sptr); ; 
//          while nverts{--} dec(nverts); 
//          do
//          begin 
//            {!!!a type cast? =>} {func(}(sptr); 
//            sptr:= sptr + (2); 
//          end;
//        end;
//      end;
//      SP_TYPE:
//      begin
//        while 1
//        do
//        begin 
//          case {*}sptr^{++} inc(sptr);  of
//            SP_MOVETO:
//            begin
//              {!!!a type cast? =>} {func(}(sptr); 
//              sptr:= sptr + (2); 
//            end;
//            SP_LINETO:
//            begin
//              {!!!a type cast? =>} {func(}(sptr); 
//              sptr:= sptr + (2); 
//            end;
//            SP_CURVETO:
//            begin
//              {!!!a type cast? =>} {func(}(sptr+0); 
//              {!!!a type cast? =>} {func(}(sptr+2); 
//              {!!!a type cast? =>} {func(}(sptr+4); 
//              sptr:= sptr + (6); 
//            end;
//            SP_CLOSEPATH:
//            begin
//            end;
//            SP_RETCLOSEPATH:
//            begin
//              exit;
//            end;
//            SP_RET:
//            begin
//              exit;
//            end;
//            
//          end;{case?}
//        end;
//      end;
//      else
//      begin
//        fprintf(stderr,'applytocharverts: bad obj font type\n');
//      end;
//    end;{case?}
//  end;
//end;
//
//
//
//implementation
//
//
//function chartoindex(fnt: pobjfnt;  c: integer): integer; 
//begin
//  if c<fnt.charmin
//  then
//  begin
//    result:= -1; 
//    exit;
//  end;
//  if c>fnt.charmax
//  then
//  begin
//    result:= -1; 
//    exit;
//  end;
//  begin
//    result:= c-fnt.charmin; 
//    exit;
//  end;
//end;
//
//
//
//function getchardesc(fnt: pobjfnt;  c: integer): pchardesc; 
//var
//index: integer; 
//begin
//  index:= chartoindex(fnt,c); 
//  if index<0
//  then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  begin
//    result:= fnt.my_chars+index; 
//    exit;
//  end;
//end;
//
//var {was static}
//xmin: integer; 
//xmax: integer; 
//ymin: integer; 
//ymax: integer; 
//
//
//function bboxcalc(v: array [0..Pred(2)] of smallint): integer; 
//begin
//  if v[0]<xmin
//  then
//  xmin:= v[0]; 
//  if v[1]<ymin
//  then
//  ymin:= v[1]; 
//  if v[0]>xmax
//  then
//  xmax:= v[0]; 
//  if v[1]>ymax
//  then
//  ymax:= v[1]; 
//end;
//calccharbboxes(objfnt*fnt); 
//
//begin 
//  var
//  c: integer; 
//  cd: pchardesc; 
//  for{while} c:=fnt.charmin to fnt.charmax { c++}
//  do
//  begin 
//    xmin:= ymin:=NOBBOX; 
//    xmax:= ymax:=-NOBBOX; 
//    applytocharverts(fnt,c,bboxcalc); 
//    cd:= getchardesc(fnt,c); 
//    if xmin=NOBBOX
//    then
//    begin 
//      cd.llx:= NOBBOX; 
//      cd.lly:= NOBBOX; 
//      cd.urx:= NOBBOX; 
//      cd.ury:= NOBBOX; 
//    end;
//    else
//    begin 
//      cd.llx:= xmin; 
//      cd.lly:= ymin; 
//      cd.urx:= xmax; 
//      cd.ury:= ymax; 
//    end;
//  end;
//end;
//
//
//
//function newobjfnt(type: integer;  charmin: integer;  charmax: integer;  fscale: integer): pobjfnt; 
//var
//fnt: pobjfnt; 
//sptr: psmallint; 
//i: integer; 
//begin
//  
//  
//  fnt:= {!!!a type cast? =>} {pobjfnt(}mallocN(sizeof(objfnt),'newobjfnt'); 
//  fnt.freeaddr:= 0; 
//  fnt.type:= type; 
//  fnt.charmin:= charmin; 
//  fnt.charmax:= charmax; 
//  fnt.my_nchars:= fnt.charmax-fnt.charmin+1; 
//  fnt.scale:= fscale; 
//  fnt.my_chars:= {!!!a type cast? =>} {pchardesc(}mallocN(fnt.my_nchars*sizeof(chardesc),'newobjfnt2'); 
//  bzero(fnt.my_chars,fnt.my_nchars*sizeof(chardesc)); 
//  begin
//    result:= fnt; 
//    exit;
//  end;
//end;
//addchardata(objfnt*fnt,intc,short*data,intnshorts); 
//
//
//begin 
//  index: integer; 
//  cd: pchardesc; 
//  index:= chartoindex(fnt,c); 
//  if index<0
//  then
//  begin 
//    fprintf(stderr,'Addchardata bad poop\n');
//    exit;
//  end;
//  cd:= fnt.my_chars+index; 
//  fnt.freeaddr:= 0; 
//  cd.datalen:= nshorts*sizeof(short); 
//  cd.data:= {!!!a type cast? =>} {psmallint(}mallocN(cd.datalen,'addchardata'); 
//  bcopy(data,cd.data,cd.datalen); 
//end;
//addcharmetrics(objfnt*fnt,intc,intmovex,intmovey); 
//
//begin 
//  index: integer; 
//  cd: pchardesc; 
//  index:= chartoindex(fnt,c); 
//  if index<0
//  then
//  begin 
//    fprintf(stderr,'Addcharmetrics bad poop\n');
//    exit;
//  end;
//  cd:= fnt.my_chars+index; 
//  cd.movex:= movex; 
//  cd.movey:= movey; 
//end;
//fakechar(objfnt*fnt,intc,intwidth); 
//
//
//begin 
//  chardata: array [0..Pred(1)] of smallint; 
//  chardata[0]:= PO_RET; 
//  addchardata(fnt,c,chardata,1); 
//  addcharmetrics(fnt,c,width,0); 
//end;
//freeobjfnt(objfnt*fnt); 
//
//
//begin 
//  i: integer; 
//  cd: pchardesc; 
//  cd:= fnt.my_chars; 
//  for{while} i:=0 to Pred(fnt.my_nchars) { i++}
//  do
//  begin 
//    if cd.data<>0{nil} {<= !!!9} 
//    then
//    freeN(cd.data); 
//    inc(cd); 
//  end;
//  freeN(fnt.my_chars); 
//  freeN(fnt); 
//end;
//sizeoffile(FILE*f); 
//
//
//(* END 5.2 *)
//
//begin 
//  pos: integer; 
//  ret: integer; 
//  pos:= ftell(f); 
//  if (ret:=fseek(f,0,2))<0
//  then
//  begin 
//    fprintf(stderr,'sizeoffile: seek error\n');
//    begin
//      result:= (0); 
//      exit;
//    end;
//  end;
//  ret:= ftell(f); 
//  if fseek(f,pos,0)<0
//  then
//  begin 
//    fprintf(stderr,'sizeoffile: seek error\n');
//    begin
//      result:= (0); 
//      exit;
//    end;
//  end;
//  begin
//    result:= ret; 
//    exit;
//  end;
//end;
//writeobjfnt(char*name,objfnt*fnt); 
//
//begin 
//  of: pFILE; 
//  i: integer; 
//  magic: integer; 
//  calccharbboxes(fnt); 
//  of:= fopen(name,'wb'); 
//  if {not}0=of
//  then
//  begin 
//    fprintf(stderr,'writeobjfnt: can''t open output file %s\n',name);
//    begin
//      result:= 0; 
//      exit;
//    end;
//  end;
//  magic:= OFMAGIC; 
//  fwrite(@magic,sizeof(int),1,of); 
//  fwrite(fnt,sizeof(objfnt),1,of); 
//  fwrite(fnt.my_chars,fnt.my_nchars*sizeof(chardesc),1,of); 
//  for{while} i:=0 to Pred(fnt.my_nchars) { i++}
//  do
//  begin 
//    if fnt.my_chars[i].datalen>0
//    then
//    fwrite(fnt.my_chars[i].data,fnt.my_chars[i].datalen,1,of); 
//  end;
//  fclose(of); 
//end;
//
//
//
//function STDtoISO(c: smallint): smallint; 
//var
//i: smallint;
// 
//begin
//  i:=(sizeof(STDvsISO) div (2*sizeof(short)))-1; 
//  for{while}  downto 0 { i--}
//  do
//  begin 
//    if STDvsISO[i][0]=c
//    then
//    begin
//      result:= (STDvsISO[i][1]); 
//      exit;
//    end;
//  end;
//  begin
//    result:= (c); 
//    exit;
//  end;
//end;
//
//
//(*
// * read the font matrix out of the font file
// *
// *)
//
//
//function newfgets(s: pchar;  n: integer;  stream: pFILE): pchar; 
//var
//read: integer;
// 
//c: integer; 
//p: pchar; 
//begin
//  read:=0; 
//  
//  p:= s; 
//  while n>0
//  do
//  begin 
//    c:= getc(stream); 
//    if c=EOF
//    then
//    begin 
//      if read=0
//      then
//      begin
//        result:= (0); 
//        exit;
//      end;
//      {*}p^:=0; 
//      begin
//        result:= (s); 
//        exit;
//      end;
//    end;
//    if c=10)or(c=13
//    then
//    begin 
//      {*}p^:=0; 
//      begin
//        result:= (s); 
//        exit;
//      end;
//    end;
//    {*}p^{++} inc(p); :=c; 
//  end;
//  {*}p^:=0; 
//  begin
//    result:= (s); 
//    exit;
//  end;
//end;
//
//
//function readfontmatrix(name: pchar;  mat: array [0..Pred(2),0..Pred(2)] of float): integer; 
//var
//inf: pFILE; 
//cptr: pchar; 
//a: float; 
//b: float; 
//c: float; 
//d: float; 
//e: float; 
//f: float; 
//(* open the input file *)
//begin
//  
//  
//  
//  
//  
//  
//  
//  inf:= fopen(name,'rb'); 
//  if {not}0=inf
//  then
//  begin 
//    fprintf(stderr,'fromtype1: can''t open input file %s\n',name);
//    begin
//      result:= (-1); 
//      exit;
//    end;
//  end;
//  
//  (* look for the FontMatrix def *)
//  while 1
//  do
//  begin 
//    if {not}0=newfgets(oneline,LINELEN,inf)
//    then
//    begin 
//      fprintf(stderr,'fromtype1: no FontMatrix found\n');
//      begin
//        result:= (-1); 
//        exit;
//      end;
//    end;
//    cptr:= strchr(oneline,'/'); 
//    if cptr<>0{nil} {<= !!!9} 
//    then
//    begin 
//      if strncmp(cptr,'/FontMatrix',11)=0
//      then
//      begin 
//        cptr:= strchr(cptr,'['); 
//        if {not}0=cptr
//        then
//        begin 
//          fprintf(stderr,'fromtype1: bad FontMatrix line\n');
//          begin
//            result:= (-1); 
//            exit;
//          end;
//        end;
//        sscanf(cptr+1,'%f %f %f %f %f %f\n', and a, and b, and c, and d, and e, and f);
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//    end;
//  end;
//  fclose(inf); 
//  mat[0][0]:= 1000.0*a; 
//  mat[1][0]:= 1000.0*b; 
//  mat[0][1]:= 1000.0*c; 
//  mat[1][1]:= 1000.0*d; 
//  begin
//    result:= (0); 
//    exit;
//  end;
//end;
//
//(*
// *  decryption subroutines
// *
// *)
//
//
//function mdecrypt(cipher: char): char; 
//var
//plain: char; 
//begin
//  plain:= (cipher xor (mr shr 8)); 
//  mr:= (cipher+mr)*MC1+MC2; 
//  begin
//    result:= plain; 
//    exit;
//  end;
//end;
//decryptdata(unsignedchar*cptr,intn); 
//
//begin 
//  while n{--} dec(n); 
//  do
//  begin 
//    {*}cptr^:=mdecrypt(*cptr); 
//    inc(cptr); 
//  end;
//end;
//decryptprogram(unsignedchar*buf,intlen); 
//
//begin 
//  i: integer; 
//  resetdecrypt(4330); 
//  for{while} i:=0 to Pred(len) { i++}
//  do
//  begin 
//    if i<SKIP
//    then
//    mdecrypt(buf[i]); 
//    else
//    buf[i-SKIP]:= mdecrypt(buf[i]); 
//  end;
//  begin
//    result:= len-SKIP; 
//    exit;
//  end;
//end;
//
//
//(*
// * decode the eexec part of the file
// *
// *)
//
//
//function decodetype1(inname: pchar;  outname: pchar): integer; 
//var
//inf: pFILE; 
//outf: pFILE; 
//hptr: pchar; 
//bptr: pchar; 
//i: integer; 
//totlen: integer; 
//hexbytes: integer; 
//c: integer; 
//hexdat: pchar; 
//hextab: array [0..Pred(256)] of char; 
//(* make hex table *)
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
//  if {not}0=firsted
//  then
//  begin 
//    for{while} i:=0 to Pred(256) { i++}
//    do
//    begin 
//      if i>='0')and(i<='9'
//      then
//      hextab[i]:= i-'0'; 
//      else
//      if i>='a')and(i<='f'
//      then
//      hextab[i]:= 10+i-'a'; 
//      else
//      if i>='A')and(i<='F'
//      then
//      hextab[i]:= 10+i-'A'; 
//      else
//      hextab[i]:= NOTHEX; 
//    end;
//  end;
//  inf:= fopen(inname,'rb'); 
//  (* open input file *)
//  if {not}0=inf
//  then
//  begin 
//    fprintf(stderr,'fromtype1: can''t open input file\n');
//    begin
//      result:= (-1); 
//      exit;
//    end;
//  end;
//  totlen:= sizeoffile(inf); 
//  hexdat:= (unsignedchar* )mallocN(totlen,'hexdat'); 
//  bindat:= (unsignedchar* )mallocN(totlen,'bindat'); 
//  (* allocate buffers *)
//  
//  (* look for eexec part of file *)
//  while 1
//  do
//  begin 
//    if {not}0=newfgets(oneline,LINELEN,inf)
//    then
//    begin 
//      fprintf(stderr,'fromtype1: no currentfile eexec found\n');
//      begin
//        result:= (-1); 
//        exit;
//      end;
//    end;
//    oneline[16]:= 0; 
//    if lstrcmp(oneline,'currentfile eexe')=0
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//  end;
//  mr:= 55665; 
//  c:= getc(inf); 
//  ungetc(c,inf); 
//  
//  (* initialize decryption variables *)
//  
//  (* first byte == 0 for binary data (???) *)
//  
//  if hextab[c]<>NOTHEX
//  then
//  begin 
//    (* read all the hex bytes into the hex buffer *)
//    hexbytes:= 0; 
//    while newfgets(oneline,LINELEN,inf)
//    do
//    begin 
//      hptr:= (unsignedchar* )oneline; 
//      while {*}hptr^
//      do
//      begin 
//        if hextab[{*}hptr^]<>NOTHEX
//        then
//        hexdat[hexbytes{++} inc(hexbytes); ]:= {*}hptr^; 
//        inc(hptr); 
//      end;
//    end;
//    fclose(inf); 
//    
//    (* check number of hex bytes *)
//    if hexbytes and 1
//    then
//    dec(hexbytes); 
//    datbytes:= hexbytes div 2; 
//    hptr:= hexdat; 
//    bptr:= bindat; 
//    c:= datbytes; 
//    (* translate hex data to binary *)
//    while c{--} dec(c); 
//    do
//    begin 
//      {*}bptr^{++} inc(bptr); :=(hextab[hptr[0]] shl 4)+hextab[hptr[1]]; 
//      hptr:= hptr + (2); 
//    end;
//    decryptdata(bindat,datbytes); 
//    
//    (* decrypt the data *)
//  end;
//  
//  else
//  begin 
//    datbytes:= fread(bindat,1,totlen,inf); 
//    fclose(inf); 
//    
//    if (bindat[2] shl 8+bindat[3])=$800
//    then
//    begin 
//      (* order data (remove 6 bytes headers) *)
//      i:= datbytes; 
//      hptr:= bptr:=bindat+4; 
//      hptr:= hptr + (2); 
//      while i>0
//      do
//      begin 
//        if i>2046
//        then
//        c:= 2046; 
//        else
//        c:= i; 
//        memcpy(bptr,hptr,c); 
//        
//        bptr:= bptr + (2046); 
//        hptr:= hptr + (2046+6); 
//        i:= i - (2046+6); 
//        datbytes:= datbytes - (6); 
//      end;
//      decryptdata(bindat+4,datbytes); 
//      
//      (* decrypt the data *)
//    end;
//    else
//    begin 
//      decryptdata(bindat+6,datbytes); 
//    end;
//  end;
//  
//  {$ifdef DEBUG}
//  outf:= fopen(outname,'wb'); 
//  fwrite(bindat,datbytes,1,outf); 
//  fclose(outf); 
//  {$endif}
//  freeN(hexdat); 
//  
//end;
//fakefopen(); 
//
//(* 
// * fake file reading funcs
// *
// *
// *)
//
//begin 
//  fakepos:= 0; 
//  fakemax:= datbytes; 
//end;
//fakegettoken(unsignedchar*str); 
//
//
//begin 
//  c: integer; 
//  cptr: pchar; 
//  start: pchar; 
//  start:= {!!!a type cast? =>} {pchar(}str; 
//  cptr:= bindat+fakepos; 
//  c:= {*}cptr^{++} inc(cptr); ; 
//  inc(fakepos); 
//  if c<>#13#10
//  then
//  begin 
//    while isspace(c)
//    do
//    begin 
//      c:= {*}cptr^{++} inc(cptr); ; 
//      inc(fakepos); 
//    end;
//    while fakepos<fakemax)and({not}0=isspace(c)
//    do
//    begin 
//      {*}str^{++} inc(str); :=c; 
//      c:= {*}cptr^{++} inc(cptr); ; 
//      inc(fakepos); 
//    end;
//    if c=#13#10
//    then
//    dec(fakepos); 
//  end;
//  {*}str^:=0; 
//  if fakepos>fakemax
//  then
//  begin 
//    fprintf(stderr,'fromtype1: unexpected eof\n');
//    strcpy(start,'end'); 
//  end;
//end;
//fakefgets(char*buf,intmax); 
//
//begin 
//  cptr: pchar; 
//  cptr:= (unsignedchar* )(bindat+fakepos); 
//  while max{--} dec(max); 
//  do
//  begin 
//    {*}buf^{++} inc(buf); :=*cptr; 
//    inc(fakepos); 
//    if {*}cptr^=10)or(*cptr=13
//    then
//    begin
//      result:= 1; 
//      exit;
//    end;
//    inc(cptr); 
//    if fakepos>fakemax
//    then
//    begin
//      result:= 0; 
//      exit;
//    end;
//  end;
//  begin
//    result:= 0; 
//    exit;
//  end;
//end;
//
//
//function fakefread(n: integer): pchar; 
//begin
//  fakepos:= fakepos + (n); 
//  begin
//    result:= bindat+fakepos-n; 
//    exit;
//  end;
//end;
//applymat(floatmat[][2],float*x,float*y); 
//
//begin 
//  var
//  tx: float; 
//  ty: float; 
//  tx:= ((*x)*mat[0][0])+((*y)*mat[0][1]); 
//  ty:= ((*x)*mat[1][0])+((*y)*mat[1][1]); {*}x^:=tx; 
//  {*}y^:=ty; 
//end;
//
//
//
//function loadpostcriptfont(filename: pchar): pobjfnt; 
//var
//i: integer; 
//j: integer; 
//k: integer; 
//d: integer; 
//index: integer; 
//len: integer; 
//nread: integer; 
//namelen: integer; 
//cptr: pchar; 
//begin
//  
//  
//  
//  
//  
//  
//  
//  
//  fnt:= 0; 
//  bindat:= 0; (* read the font matrix from the font *)
//  if readfontmatrix(filename,mat)
//  then
//  begin
//    result:= (0); 
//    exit;
//    
//    (* decode the font data *)
//  end;
//  decodetype1(filename,'/usr/tmp/type1.dec'); 
//  fakefopen(); 
//  
//  (* open the input file *)
//  
//  (* look for the /Subrs def and get my_nsubrs *)
//  while 1
//  do
//  begin 
//    if {not}0=fakefgets(oneline,LINELEN)
//    then
//    begin 
//      fprintf(stderr,'fromtype1: no /Subrs found\n');
//      my_nsubrs:= 0; 
//      fakefopen(); 
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    cptr:= strchr(oneline,'/'); 
//    if cptr<>0{nil} {<= !!!9} 
//    then
//    begin 
//      if strncmp(cptr,'/Subrs',6)=0
//      then
//      begin 
//        my_nsubrs:= atoi(cptr+6); 
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//    end;
//  end;
//  
//  (* read the Subrs in one by one *)
//  for{while} i:=0 to Pred(my_nsubrs) { i++}
//  do
//  my_sublen[i]:= 0; 
//  for{while} i:=0 to Pred(my_nsubrs) { i++}
//  do
//  begin 
//    for{while} k:=0 to Pred(MAXTRIES) { k++}
//    do
//    begin 
//      fakegettoken(tok); 
//      if lstrcmp(tok,'dup')=0
//      then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    if k=MAXTRIES
//    then
//    begin 
//      fprintf(stderr,'dup for subr %d not found in range\n',i);
//    end;
//    (*exit(1);*)
//    fakegettoken(tok); 
//    
//    (* get the Subr index here *)
//    index:= atoi(tok); 
//    (* check to make sure it is in range *)
//    if index<0)or(index>my_nsubrs
//    then
//    begin 
//      fprintf(stderr,'bad Subr index %d\n',index);
//    end;
//    (*exit(1);*)
//    fakegettoken(tok); 
//    
//    (* get the number of bytes to read *)
//    nread:= atoi(tok); 
//    fakegettoken(tok); 
//    my_sublen[index]:= nread; 
//    my_subrs[index]:= fakefread(nread); 
//    fakegettoken(tok); 
//    
//    (* read in the subroutine *)
//  end;
//  
//  (* look for the CharStrings *)
//  while 1
//  do
//  begin 
//    fakegettoken(tok); 
//    cptr:= strchr(tok,'/'); 
//    if cptr)and(lstrcmp(cptr,'/CharStrings')=0
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//  end;
//  fakegettoken(tok); 
//  
//  fakegettoken(tok); 
//  (* skip my_ncharscrings *)
//  fakegettoken(tok); 
//  (* skip dict *)
//  fakegettoken(tok); 
//  (* skip dup *)
//  fakegettoken(tok); 
//  (* skip begin *)
//  my_nchars:= 0; (* skip newline *)
//  
//  (* read the CharStrings one by one *)
//  for{while} i:=0 to Pred(MAXCHARS) { i++}
//  do
//  begin 
//    
//    (* check for end *)
//    fakegettoken(tok); 
//    if lstrcmp(tok,'end')=0
//    then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    
//    (* get the char name and allocate space for it *)
//    namelen:= lstrlen(tok); 
//    my_charname[i]:= {!!!a type cast? =>} {pchar(}mallocN(namelen+1,'my_charname'); 
//    strcpy(my_charname[i],tok); 
//    fakegettoken(tok); 
//    
//    (* get the number of bytes to read *)
//    nread:= atoi(tok); 
//    fakegettoken(tok); 
//    my_charlen[i]:= nread; 
//    my_chars[i]:= fakefread(nread); 
//    fakegettoken(tok); 
//    
//    (* read in the char description *)
//    
//    (* skip the end of line *)
//    fakegettoken(tok); 
//    inc(my_nchars); 
//  end;
//  decryptall(); 
//  
//  (* decrypt the character descriptions *)
//  setcharlist(); 
//  makeobjfont(savesplines); 
//  
//  (* make the obj font *)
//  
//  if bindat<>0{nil} {<= !!!9} 
//  then
//  freeN(bindat); 
//  (* system("rm /usr/tmp/type1.dec"); *)
//  
//  begin
//    result:= (fnt); 
//    exit;
//  end;
//end;
//setcharlist(); 
//
//(*
//main(argc, argv)
//int argc;
//char **argv;
//{
//    objfnt *fnt;
//
//    beztol = 5.0;
//    savesplines = 0;
//    fnt = 0;
//    
//    if(argc<3) {
//  fprintf(stderr,"usage: fromtype1 type1font [font.of tol] [font.sf 0.0]\n");
//  exit(1);
//    }
//
//    if(argc>3) beztol = atof(argv[3]);
//    if(beztol<0.0001) {
//  savesplines = 1;
//  beztol = 100.0;
//    }
//
//    fnt = loadpostcriptfont(argv[1]);
//    if (fnt) writeobjfnt(argv[2],fnt);
//}
//
//*)
//begin 
//  name: pchar; 
//  found: char; 
//  i: integer; 
//  j: integer; 
//  for{while} i:=0 to Pred(NASCII) { i++}
//  do
//  ISOcharlist[i].prog:= -1; 
//  for{while} j:=0 to Pred(my_nchars) { j++}
//  do
//  begin 
//    name:= my_charname[j]; 
//    if name<>0{nil} {<= !!!9} 
//    then
//    begin 
//      found:= 0; 
//      for{while} i:=0 to Pred(NASCII) { i++}
//      do
//      begin 
//        if ISOcharlist[i].name)and((lstrcmp(name,ISOcharlist[i].name)=0)
//        then
//        begin 
//          ISOcharlist[i].prog:= j; 
//          found:= 1; 
//        end;
//      end;
//      freeN(name); 
//      (*if (found == 0) printf("no match found for: %s\n", name);*)
//      my_charname[j]:= 0; 
//    end;
//  end;
//end;
//resetdecrypt(intn); 
//
//(*
// * Decryption support
// *
// *
// *)
//begin 
//  mr:= n; 
//end;
//decryptall(); 
//
//begin 
//  i: integer; 
//  for{while} i:=0 to Pred(my_nsubrs) { i++}
//  do
//  my_sublen[i]:= decryptprogram(my_subrs[i],my_sublen[i]); 
//  for{while} i:=0 to Pred(my_nchars) { i++}
//  do
//  my_charlen[i]:= decryptprogram(my_chars[i],my_charlen[i]); 
//end;
//drawchar(intc); 
//
//(*
// * run the character program 
// *
// *
// *)
//
//begin 
//  if savesplines<>0{nil} {<= !!!9} 
//  then
//  begin 
//    spline_beginchar(); 
//  end;
//  else
//  begin 
//    poly_beginchar(); 
//  end;
//  initstack(); 
//  initpcstack(); 
//  initretstack(); 
//  pc:= my_chars[c]; 
//  runprog(); 
//  if savesplines<>0{nil} {<= !!!9} 
//  then
//  begin 
//    spline_endchar(); 
//  end;
//  else
//  begin 
//    poly_endchar(); 
//  end;
//end;
//initpcstack(); 
//
//(*
// * pc stack support
// *
// *)
//
//begin 
//  pcsp:= 0; 
//end;
//pushpc(unsignedchar*pc); 
//
//begin 
//  pcstack[pcsp]:= pc; 
//  inc(pcsp); 
//end;
//
//
//function poppc: pchar; 
//begin
//  dec(pcsp); 
//  if pcsp<0
//  then
//  begin 
//    fprintf(stderr,'\nYUCK: pc stack under flow\n');
//    pcsp:= 0; 
//    begin
//      result:= 0; 
//      exit;
//    end;
//  end;
//  begin
//    result:= pcstack[pcsp]; 
//    exit;
//  end;
//end;
//initstack(); 
//
//(*
// * Data stack support
// *
// *)
//
//begin 
//  sp:= 0; 
//end;
//push(val); 
//
//var
//val: integer; 
//begin 
//  stack[sp]:= val; 
//  inc(sp); 
//end;
//pop(); 
//
//begin 
//  dec(sp); 
//  if sp<0
//  then
//  begin 
//    fprintf(stderr,'\nYUCK: stack under flow\n');
//    sp:= 0; 
//    begin
//      result:= 0; 
//      exit;
//    end;
//  end;
//  begin
//    result:= stack[sp]; 
//    exit;
//  end;
//end;
//initretstack(); 
//
//(*
// * call/return data stack
// *
// *)
//
//begin 
//  retsp:= 0; 
//end;
//retpush(val); 
//
//val: integer; 
//begin 
//  retstack[retsp]:= val; 
//  inc(retsp); 
//end;
//retpop(); 
//
//begin 
//  dec(retsp); 
//  if retsp<0
//  then
//  begin 
//    fprintf(stderr,'\nYUCK: ret stack under flow\n');
//    retsp:= 0; 
//    begin
//      result:= 0; 
//      exit;
//    end;
//  end;
//  begin
//    result:= retstack[retsp]; 
//    exit;
//  end;
//end;
//runprog(); 
//
//(*
// * Character interpreter
// *
// *)
//
//begin 
//  v: integer; 
//  w: integer; 
//  num: integer; 
//  cmd: integer; 
//  while 1
//  do
//  begin 
//    v:= {*}pc^{++} inc(pc); ; 
//    if v>=0)and(v<=31
//    then
//    begin 
//      if v=12
//      then
//      begin 
//        w:= {*}pc^{++} inc(pc); ; 
//        cmd:= 256+w; 
//      end;
//      else
//      cmd:= v; 
//      if {not}0=docommand(cmd)
//      then
//      begin 
//        exit;
//      end;
//    end;
//    else
//    if v>=32)and(v<=246
//    then
//    begin 
//      num:= v-139; 
//      push(num); 
//    end;
//    else
//    if v>=247)and(v<=250
//    then
//    begin 
//      w:= {*}pc^{++} inc(pc); ; 
//      num:= (v-247)*256+w+108; 
//      push(num); 
//    end;
//    else
//    if v>=251)and(v<=254
//    then
//    begin 
//      w:= {*}pc^{++} inc(pc); ; 
//      num:= -(v-251)*256-w-108; 
//      push(num); 
//    end;
//    else
//    if v=255
//    then
//    begin 
//      num:= {*}pc^{++} inc(pc); ; 
//      num:= num shl (8); 
//      num:= num or ({*}pc^{++} inc(pc); ); 
//      num:= num shl (8); 
//      num:= num or ({*}pc^{++} inc(pc); ); 
//      num:= num shl (8); 
//      num:= num or ({*}pc^{++} inc(pc); ); 
//      push(num); 
//    end;
//  end;
//end;
//subr1(); 
//
//(*
// * execute the program:
// *
// *
// *)
//
//begin 
//  coordpos:= 0; 
//  incusp:= 1; 
//end;
//subr2(); 
//
//begin 
//  x: integer; 
//  y: integer; 
//  getmove(@x,@y); 
//  if coordpos>=7
//  then
//  begin 
//    fprintf(stderr,'subr2: bad poop\n');
//  end;
//  (*exit(1);*)
//  coordsave[coordpos][0]:= x; 
//  coordsave[coordpos][1]:= y; 
//  inc(coordpos); 
//end;
//subr0(); 
//
//begin 
//  x0: integer; 
//  y0: integer; 
//  x1: integer; 
//  y1: integer; 
//  x2: integer; 
//  y2: integer; 
//  x3: integer; 
//  y3: integer; 
//  xpos: integer; 
//  ypos: integer; 
//  noise: integer; 
//  ypos:= pop(); 
//  xpos:= pop(); 
//  noise:= pop(); 
//  if coordpos<>7
//  then
//  begin 
//    fprintf(stderr,'subr0: bad poop\n');
//  end;
//  (*exit(1);*)
//  x0:= coordsave[0][0]; 
//  y0:= coordsave[0][1]; 
//  x1:= coordsave[1][0]+x0; 
//  y1:= coordsave[1][1]+y0; 
//  x2:= coordsave[2][0]; 
//  y2:= coordsave[2][1]; 
//  x3:= coordsave[3][0]; 
//  y3:= coordsave[3][1]; 
//  rcurveto(x1,y1,x1+x2,y1+y2,x1+x2+x3,y1+y2+y3); 
//  
//  x1:= coordsave[4][0]; 
//  y1:= coordsave[4][1]; 
//  x2:= coordsave[5][0]; 
//  y2:= coordsave[5][1]; 
//  x3:= coordsave[6][0]; 
//  y3:= coordsave[6][1]; 
//  rcurveto(x1,y1,x1+x2,y1+y2,x1+x2+x3,y1+y2+y3); 
//  getpos(@x0,@y0); 
//  retpush(y0); 
//  retpush(x0); 
//  incusp:= 0; 
//end;
//
//
//procedure append_poly_offset(ofsx: smallint;  ofsy: smallint;  data: psmallint); 
//var
//nverts: integer; 
//begin
//  if data=0
//  then
//  exit;
//  
//  while 1
//  do
//  begin 
//    case chardata[nshorts{++} inc(nshorts); ]:=*data{++} inc(data);  of
//      PO_BGNLOOP:
//      begin
//        dec(nshorts); 
//      end;
//      PO_RETENDLOOP,
//      (* voor de eerste keer *)
//      PO_RET:
//      begin
//        exit;
//      end;
//      
//    end;{case?}
//    nverts:= chardata[nshorts{++} inc(nshorts); ]:=*data{++} inc(data); ; 
//    while nverts{--} dec(nverts); 
//    do
//    begin 
//      chardata[nshorts{++} inc(nshorts); ]:= (*data{++} inc(data); )+ofsx; 
//      chardata[nshorts{++} inc(nshorts); ]:= (*data{++} inc(data); )+ofsy; 
//    end;
//  end;
//end;
//
//
//
//procedure append_spline_offset(ofsx: smallint;  ofsy: smallint;  data: psmallint); 
//var
//nverts: integer;
// 
//begin
//  nverts:=0; 
//  if data=0
//  then
//  exit;
//  
//  while 1
//  do
//  begin 
//    case chardata[nshorts{++} inc(nshorts); ]:=*data{++} inc(data);  of
//      SP_MOVETO,
//      SP_LINETO:
//      begin
//        nverts:= 1; 
//      end;
//      SP_CURVETO:
//      begin
//        nverts:= 3; 
//      end;
//      SP_RETCLOSEPATH,
//      SP_RET:
//      begin
//        exit;
//      end;
//      
//    end;{case?}
//    
//    for{while}  downto Succ(0) { nverts--}
//    do
//    begin 
//      chardata[nshorts{++} inc(nshorts); ]:= (*data{++} inc(data); )+ofsx; 
//      chardata[nshorts{++} inc(nshorts); ]:= (*data{++} inc(data); )+ofsy; 
//    end;
//  end;
//end;
//docommand(intcmd); 
//
//
//begin 
//  x: integer; 
//  y: integer; 
//  w: integer; 
//  c1: integer; 
//  c2: integer; 
//  dx1: integer; 
//  dy1: integer; 
//  dx2: integer; 
//  dy2: integer; 
//  dx3: integer; 
//  dy3: integer; 
//  fdx1: float; 
//  fdy1: float; 
//  i: integer; 
//  sub: integer; 
//  n: integer; 
//  p: integer; 
//  nlen: integer; 
//  subpc: pchar; 
//  cd: pchardesc; 
//  ndata: psmallint; 
//  case cmd of
//    WHAT0:
//    begin
//      fprintf(stderr,'\nYUCK: WHAT0\n');
//    end;
//    HSTEM:
//    begin
//      pop(); 
//      pop(); 
//    end;
//    VSTEM:
//    (*printf("hstem: %d %d\n", pop(), pop());*)
//    begin
//      pop(); 
//      pop(); 
//    end;
//    VMOVETO:
//    (*printf("vstem: %d %d\n", pop(), pop());*)
//    begin
//      y:= pop(); 
//      rmoveto(0,y); 
//    end;
//    RLINETO:
//    begin
//      y:= pop(); 
//      x:= pop(); 
//      rlineto(x,y); 
//    end;
//    HLINETO:
//    begin
//      x:= pop(); 
//      rlineto(x,0); 
//    end;
//    VLINETO:
//    begin
//      y:= pop(); 
//      rlineto(0,y); 
//    end;
//    RRCURVETO:
//    begin
//      dy3:= pop(); 
//      dx3:= pop(); 
//      dy2:= pop(); 
//      dx2:= pop(); 
//      dy1:= pop(); 
//      dx1:= pop(); 
//      rcurveto(dx1,dy1,dx1+dx2,dy1+dy2,dx1+dx2+dx3,dy1+dy2+dy3); 
//    end;
//    CLOSEPATH:
//    begin
//      closepath(); 
//    end;
//    CALLSUBR:
//    begin
//      sub:= pop(); 
//      subpc:= my_subrs[sub]; 
//      if {not}0=subpc
//      then
//      begin 
//        fprintf(stderr,'\nYUCK no sub addr\n');
//      end;
//      pushpc(pc); 
//      pc:= subpc; 
//    end;
//    RETURN:
//    begin
//      pc:= poppc(); 
//    end;
//    HSBW:
//    begin
//      w:= pop(); 
//      x:= pop(); 
//      setwidth(w,x); 
//      sbpoint(x,0); 
//    end;
//    ENDCHAR:
//    begin
//      closepath(); 
//    end;
//    RMOVETO:
//    begin
//      y:= pop(); 
//      x:= pop(); 
//      rmoveto(x,y); 
//    end;
//    HMOVETO:
//    begin
//      x:= pop(); 
//      rmoveto(x,0); 
//    end;
//    VHCURVETO:
//    begin
//      dy3:= 0; 
//      dx3:= pop(); 
//      dy2:= pop(); 
//      dx2:= pop(); 
//      dy1:= pop(); 
//      dx1:= 0; 
//      rcurveto(dx1,dy1,dx1+dx2,dy1+dy2,dx1+dx2+dx3,dy1+dy2+dy3); 
//    end;
//    HVCURVETO:
//    begin
//      dy3:= pop(); 
//      dx3:= 0; 
//      dy2:= pop(); 
//      dx2:= pop(); 
//      dy1:= 0; 
//      dx1:= pop(); 
//      rcurveto(dx1,dy1,dx1+dx2,dy1+dy2,dx1+dx2+dx3,dy1+dy2+dy3); 
//    end;
//    DOTSECTION:
//    begin
//    end;
//    VSTEM3:
//    (*printf("vstem3\n");*)
//    begin
//      pop(); 
//      pop(); 
//      pop(); 
//      pop(); 
//      pop(); 
//      pop(); 
//    end;
//    HSTEM3:
//    (*printf("hstem3\n");*)
//    begin
//      pop(); 
//      pop(); 
//      pop(); 
//      pop(); 
//      pop(); 
//      pop(); 
//    end;
//    SEAC:
//    begin
//      if 0
//      then
//      begin 
//        printf('seac: %3d %3d %3d %3d %3d\n',pop(),pop(),pop(),pop(),pop());
//      end;
//      else
//      begin 
//        c2:= STDtoISO(pop()); 
//        c1:= STDtoISO(pop()); 
//        cd:= getchardesc(fnt,c1); (* accent *)
//        (* letter *)
//        
//        if cd<>0{nil} {<= !!!9} 
//        then
//        begin 
//          memcpy(chardata,cd.data,cd.datalen); 
//          nshorts:= cd.datalen div sizeof(short); 
//        end;
//        cd:= getchardesc(fnt,c2); 
//        if cd)and(cd.data)and(cd.datalen
//        then
//        begin 
//          nlen:= cd.datalen div sizeof(short); 
//          ndata:= cd.data; 
//          if nshorts<>0{nil} {<= !!!9} 
//          then
//          begin 
//            if savesplines<>0{nil} {<= !!!9} 
//            then
//            begin 
//              case chardata[nshorts-1] of
//                SP_RET:
//                begin
//                  dec(nshorts); 
//                end;
//                SP_RETCLOSEPATH:
//                begin
//                  chardata[nshorts-1]:= SP_CLOSEPATH; 
//                end;
//                
//              end;{case?}
//            end;
//            else
//            begin 
//              case chardata[nshorts-1] of
//                PO_RET:
//                begin
//                  printf('PO_RET in character disription ?\n');
//                  dec(nshorts); 
//                end;
//                PO_RETENDLOOP:
//                begin
//                  if ndata[0]=PO_BGNLOOP
//                  then
//                  begin 
//                    chardata[nshorts-1]:= PO_ENDBGNLOOP; 
//                  end;
//                  else
//                  begin 
//                    printf('new character doesn''t start with PO_BGNLOOP ?\n');
//                  end;
//                end;
//                
//              end;{case?}
//            end;
//          end;
//          dy1:= pop(); 
//          dx1:= pop()+sidebearing[c1]-sidebearing[c2]; 
//          pop(); 
//          
//          (* i.p.v. the sidebearing[c1] moet misschen thesidebearing gebruikt worden *)
//          
//          fdx1:= dx1; 
//          fdy1:= dy1; 
//          applymat(mat, and fdx1, and fdy1); 
//          
//          dx1:= floor(fdx1); 
//          dy1:= floor(fdy1); 
//          if savesplines<>0{nil} {<= !!!9} 
//          then
//          begin 
//            append_spline_offset(dx1,dy1,ndata); 
//          end;
//          else
//          begin 
//            append_poly_offset(dx1,dy1,ndata); 
//          end;
//        end;
//        
//        (*printf("first: %d %d\n", cd->data[0], cd->data[1]);*)
//        fflush(stdout); 
//      end;
//    end;
//    SBW:
//    begin
//      w:= pop(); 
//      y:= pop(); 
//      fprintf(stderr,'sbw: width: %d %d\n',w,y);
//      y:= pop(); 
//      x:= pop(); 
//      fprintf(stderr,'sbw: side: %d %d\n',x,y);
//      setwidth(w,x); 
//      sbpoint(x,y); 
//    end;
//    DIV:
//    begin
//      x:= pop(); 
//      y:= pop(); 
//      push(x div y); 
//    end;
//    CALLOTHERSUBR:
//    begin
//      sub:= pop(); 
//      n:= pop(); 
//      if sub=0
//      then
//      subr0(); 
//      else
//      if sub=1
//      then
//      subr1(); 
//      else
//      if sub=2
//      then
//      subr2(); 
//      else
//      begin 
//        for{while} i:=0 to Pred(n) { i++}
//        do
//        begin 
//          retpush(pop()); 
//        end;
//      end;
//    end;
//    POP:
//    begin
//      push(retpop()); 
//    end;
//    SETCURRENTPOINT:
//    begin
//      y:= pop(); 
//      x:= pop(); 
//      sbpoint(x,y); 
//    end;
//    else
//    (*fprintf(stderr,"\nYUCK bad instruction %d\n",cmd);*)
//    begin
//    end;
//    
//  end;{case?}
//  if pc=0)or(cmd=ENDCHAR)or(cmd=WHAT0)or(cmd=SEAC
//  then
//  begin
//    result:= 0; 
//    exit;
//  end;
//  else
//  begin
//    result:= 1; 
//    exit;
//  end;
//end;
//
//(* 
// *    graphics follows 
// *
// *
// *)
//
//startx: integer; 
//starty: integer; 
//curx: integer; 
//cury: integer; 
//nextx: integer; 
//nexty: integer; 
//delx: integer; 
//dely: integer; 
//started: integer; 
//(* poly output stuff *)
//
//setwidth(intw,intx); 
//begin 
//  thecharwidth:= w; 
//  thesidebearing:= x; 
//end;
//poly_beginchar(); 
//
//begin 
//  npnts:= 0; 
//  nloops:= 0; 
//end;
//poly_endchar(); 
//
//begin 
//  if nloops=0
//  then
//  chardata[nshorts{++} inc(nshorts); ]:= PO_RET; 
//  else
//  chardata[nshorts{++} inc(nshorts); ]:= PO_RETENDLOOP; 
//end;
//poly_close(); 
//
//begin 
//  chardata[nvertpos]:= npnts; 
//  npnts:= 0; 
//end;
//poly_pnt(floatx,floaty); 
//
//begin 
//  v: array [0..Pred(3)] of smallint; 
//  ix: integer; 
//  iy: integer; 
//  applymat(mat,@x,@y); 
//  ix:= floor(x); 
//  iy:= floor(y); 
//  if npnts=0
//  then
//  begin 
//    if nloops=0
//    then
//    begin 
//      chardata[nshorts{++} inc(nshorts); ]:= PO_BGNLOOP; 
//      nvertpos:= nshorts{++} inc(nshorts); ; 
//    end;
//    else
//    begin 
//      chardata[nshorts{++} inc(nshorts); ]:= PO_ENDBGNLOOP; 
//      nvertpos:= nshorts{++} inc(nshorts); ; 
//    end;
//    inc(nloops); 
//  end;
//  chardata[nshorts{++} inc(nshorts); ]:= ix; 
//  chardata[nshorts{++} inc(nshorts); ]:= iy; 
//  inc(npnts); 
//  
//end;
//spline_beginchar(); 
//
//(* spline output stuff *)
//
//begin 
//  sp_npnts:= 0; 
//  sp_nloops:= 0; 
//end;
//spline_endchar(); 
//
//begin 
//  if sp_nloops=0
//  then
//  chardata[nshorts{++} inc(nshorts); ]:= SP_RET; 
//  else
//  chardata[nshorts{++} inc(nshorts); ]:= SP_RETCLOSEPATH; 
//end;
//spline_close(); 
//
//begin 
//  chardata[nshorts{++} inc(nshorts); ]:= SP_CLOSEPATH; 
//  sp_npnts:= 0; 
//  sp_nloops:= 0; 
//end;
//spline_line(floatx0,floaty0,floatx1,floaty1); 
//
//begin 
//  applymat(mat,@x0,@y0); 
//  applymat(mat,@x1,@y1); 
//  
//  if sp_npnts=0
//  then
//  begin 
//    chardata[nshorts{++} inc(nshorts); ]:= SP_MOVETO; 
//    chardata[nshorts{++} inc(nshorts); ]:= floor(x0); 
//    chardata[nshorts{++} inc(nshorts); ]:= floor(y0); 
//    inc(sp_npnts); 
//    inc(sp_nloops); 
//  end;
//  chardata[nshorts{++} inc(nshorts); ]:= SP_LINETO; 
//  chardata[nshorts{++} inc(nshorts); ]:= floor(x1); 
//  chardata[nshorts{++} inc(nshorts); ]:= floor(y1); 
//  inc(sp_npnts); 
//end;
//spline_curveto(floatx0,floaty0,floatx1,floaty1,floatx2,floaty2,floatx3,floaty3); 
//
//begin 
//  applymat(mat,@x0,@y0); 
//  applymat(mat,@x1,@y1); 
//  applymat(mat,@x2,@y2); 
//  applymat(mat,@x3,@y3); 
//  
//  if sp_npnts=0
//  then
//  begin 
//    chardata[nshorts{++} inc(nshorts); ]:= SP_MOVETO; 
//    chardata[nshorts{++} inc(nshorts); ]:= floor(x0); 
//    chardata[nshorts{++} inc(nshorts); ]:= floor(y0); 
//    inc(sp_npnts); 
//    inc(sp_nloops); 
//  end;
//  chardata[nshorts{++} inc(nshorts); ]:= SP_CURVETO; 
//  chardata[nshorts{++} inc(nshorts); ]:= floor(x1); 
//  chardata[nshorts{++} inc(nshorts); ]:= floor(y1); 
//  chardata[nshorts{++} inc(nshorts); ]:= floor(x2); 
//  chardata[nshorts{++} inc(nshorts); ]:= floor(y2); 
//  chardata[nshorts{++} inc(nshorts); ]:= floor(x3); 
//  chardata[nshorts{++} inc(nshorts); ]:= floor(y3); 
//end;
//getmove(int*x,int*y); 
//
//
//begin 
//  {*}x^:=delx; 
//  {*}y^:=dely; 
//end;
//(*  printf("ingetmove\n"); *)
//getpos(int*x,int*y); 
//
//begin 
//  {*}x^:=curx; 
//  {*}y^:=cury; 
//end;
//savestart(intx,inty); 
//
//begin 
//  startx:= x; 
//  starty:= y; 
//  started:= 1; 
//end;
//sbpoint(intx,inty); 
//
//begin 
//  curx:= x; 
//  cury:= y; 
//end;
//moveto(intx,inty); 
//
//begin 
//  curx:= x; 
//  cury:= y; 
//  savestart(curx,cury); 
//end;
//rmoveto(intx,inty); 
//
//begin 
//  if incusp<>0{nil} {<= !!!9} 
//  then
//  begin 
//    delx:= x; 
//    dely:= y; 
//  end;
//  else
//  begin 
//    curx:= curx + (x); 
//    cury:= cury + (y); 
//    savestart(curx,cury); 
//  end;
//end;
//drawline(floatx0,floaty0,floatx1,floaty1,floatdx0,floatdy0,floatdx1,floatdy1); 
//
//begin 
//  if x0<>x1)or(y0<>y1
//  then
//  poly_pnt(x1,y1); 
//end;
//rlineto(intx,inty); 
//
//
//begin 
//  dx: float; 
//  dy: float; 
//  nextx:= curx+x; 
//  nexty:= cury+y; 
//  dx:= nextx-curx; 
//  dy:= nexty-cury; 
//  if savesplines<>0{nil} {<= !!!9} 
//  then
//  spline_line(curx,cury,nextx,nexty); 
//  else
//  drawline(curx,cury,nextx,nexty,dx,dy,dx,dy); 
//  curx:= nextx; 
//  cury:= nexty; 
//end;
//closepath(); 
//
//begin 
//  dx: float; 
//  dy: float; 
//  if started<>0{nil} {<= !!!9} 
//  then
//  begin 
//    dx:= startx-curx; 
//    dy:= starty-cury; 
//    if savesplines<>0{nil} {<= !!!9} 
//    then
//    begin 
//      spline_close(); 
//    end;
//    else
//    begin 
//      drawline(curx,cury,startx,starty,dx,dy,dx,dy); 
//      poly_close(); 
//    end;
//    started:= 0; 
//  end;
//end;
//bezadapt(floatx0,floaty0,floatx1,floaty1,floatx2,floaty2,floatx3,floaty3,floatbeztol); 
//
//begin 
//  ax0: float; 
//  ay0: float; 
//  ax1: float; 
//  ay1: float; 
//  ax2: float; 
//  ay2: float; 
//  ax3: float; 
//  ay3: float; 
//  bx0: float; 
//  by0: float; 
//  bx1: float; 
//  by1: float; 
//  bx2: float; 
//  by2: float; 
//  bx3: float; 
//  by3: float; 
//  midx: float; 
//  midy: float; 
//  linx: float; 
//  liny: float; 
//  dx: float; 
//  dy: float; 
//  mag: float; 
//  midx:= (x0+3*x1+3*x2+x3) div 8.0; 
//  midy:= (y0+3*y1+3*y2+y3) div 8.0; 
//  linx:= (x0+x3) div 2.0; 
//  liny:= (y0+y3) div 2.0; 
//  dx:= midx-linx; 
//  dy:= midy-liny; 
//  mag:= dx*dx+dy*dy; 
//  if mag<(beztol*beztol)
//  then
//  drawline(x0,y0,x3,y3,x1-x0,y1-y0,x3-x2,y3-y2); 
//  else
//  begin 
//    ax0:= x0; 
//    ay0:= y0; 
//    ax1:= (x0+x1) div 2; 
//    ay1:= (y0+y1) div 2; 
//    ax2:= (x0+2*x1+x2) div 4; 
//    ay2:= (y0+2*y1+y2) div 4; 
//    ax3:= midx; 
//    ay3:= midy; 
//    bezadapt(ax0,ay0,ax1,ay1,ax2,ay2,ax3,ay3,beztol); 
//    bx0:= midx; 
//    by0:= midy; 
//    bx1:= (x1+2*x2+x3) div 4; 
//    by1:= (y1+2*y2+y3) div 4; 
//    bx2:= (x2+x3) div 2; 
//    by2:= (y2+y3) div 2; 
//    bx3:= x3; 
//    by3:= y3; 
//    bezadapt(bx0,by0,bx1,by1,bx2,by2,bx3,by3,beztol); 
//    
//  end;
//end;
//drawbez(floatx0,floaty0,floatx1,floaty1,floatx2,floaty2,floatx3,floaty3); 
//
//begin 
//  bezadapt(x0,y0,x1,y1,x2,y2,x3,y3,beztol); 
//end;
//rcurveto(intdx1,intdy1,intdx2,intdy2,intdx3,intdy3); 
//
//
//begin 
//  x0: integer; 
//  y0: integer; 
//  x1: integer; 
//  y1: integer; 
//  x2: integer; 
//  y2: integer; 
//  x3: integer; 
//  y3: integer; 
//  x0:= curx; 
//  y0:= cury; 
//  x1:= curx+dx1; 
//  y1:= cury+dy1; 
//  x2:= curx+dx2; 
//  y2:= cury+dy2; 
//  x3:= curx+dx3; 
//  y3:= cury+dy3; 
//  if savesplines<>0{nil} {<= !!!9} 
//  then
//  begin 
//    spline_curveto(x0,y0,x1,y1,x2,y2,x3,y3); 
//  end;
//  else
//  begin 
//    drawbez(x0,y0,x1,y1,x2,y2,x3,y3); 
//  end;
//  curx:= x3; 
//  cury:= y3; 
//end;
//makeobjfont(intsavesplines); 
//
//(*
// * saveobjfont -
// *  save an object font.
// *
// *)
//
//(* generic routines *)
//
//begin 
//  i: integer; 
//  j: integer; 
//  c: integer; 
//  if savesplines<>0{nil} {<= !!!9} 
//  then
//  fnt:= newobjfnt(SP_TYPE,32,32+NASCII-1,9840); 
//  else
//  fnt:= newobjfnt(PO_TYPE,32,32+NASCII-1,9840); 
//  for{while} i:=0 to Pred(NASCII) { i++}
//  do
//  begin 
//    c:= i+32; 
//    if ISOcharlist[i].prog>=0
//    then
//    begin 
//      (*printf("decoding %s\n", ISOcharlist[i].name);*)
//      
//      nshorts:= 0; 
//      drawchar(ISOcharlist[i].prog); 
//      addchardata(fnt,c,chardata,nshorts); 
//      addcharmetrics(fnt,c,thecharwidth,0); 
//      sidebearing[c]:= thesidebearing; 
//    end;
//    else
//    if c=' '
//    then
//    begin 
//      printf('faking space %d\n',i);
//      fakechar(fnt,' ',400); 
//    end;
//  end;
//end;

end.
