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

unit Button;

interface

uses
graphics;

(* hoort bij button.c *)
{ dit is oud  }

const
  PAPER = 100;
  PEN = 7;
  BUT_IN = 13;
  BUT_OUT = 12;
  CHA = 32;
  SHO = 64;
{ #define LON	96  }
  INT = 96;
  FLO = 128;
{ #define DOU	160  }
  FUN = 192;
  BIT = 256;
  BUTPOIN = (128+64)+32;
  _BUT = 1 shl 9;
  ROW = 2 shl 9;
  TOG = 3 shl 9;
  SLI = 4 shl 9;
  NUM = 5 shl 9;
  TEX = 6 shl 9;
  TOG3 = 7 shl 9;
  BUTRET = 8 shl 9;
  TOGN = 9 shl 9;
  _LABEL = 10 shl 9;
  MENU = 11 shl 9;
  ICONROW = 12 shl 9;
  ICONTOG = 13 shl 9;
  NUMSLI = 14 shl 9;
  COL = 15 shl 9;
  IDPOIN = 16 shl 9;
  HSVSLI = 17 shl 9;
  BUTTYPE = 31 shl 9;
  MAXBUTSTR = 20;

{ was #define dname(params) para_def_expr }
{ argument types are unknown }
{ return type might be wrong }

//function BTST(a,b : longint) : longint;

{ was #define dname(params) para_def_expr }
{ argument types are unknown }
//function BCLR(a,b : longint) : longint;

{ was #define dname(params) para_def_expr }
{ argument types are unknown }
{ return type might be wrong }
//function BSET(a,b : longint) : longint;

const
  RGBDRAW = 1;

type
  pBut = ^But;
  But = record
      _type : smallint;
      nr : smallint;
      str : pchar;
      strdata : array[0..(MAXBUTSTR)-1] of char;
      x1 : smallint;
      y1 : smallint;
      x2 : smallint;
      y2 : smallint;
      poin : pchar;
      min : single;
      max : single;
      a1 : smallint;
      a2 : smallint;
      rt : array[0..3] of smallint;
      func : procedure ;
      font : pointer;
      tip : pchar;
      lockstr : pchar;
      lock : smallint;
      win : smallint;
      col : smallint;
      drawtype : smallint;
    end;

{ drawmode is ook naam fie  }
    pButCol = ^ButCol;
    ButCol = record
      drawmode : smallint;
      rgb : smallint;
      back : dword;
      paper_sel : dword;
      paper_desel : dword;
      pen_sel : dword;
      pen_desel : dword;
      border1 : dword;
      border2 : dword;
      border3 : dword;
      border4 : dword;
      drmode : smallint;
      rt : smallint;
    end;

  pButBlock = ^ButBlock;
  ButBlock = record
      next : pButBlock;
      first : pBut;
      aantal : longint;
      window : longint;
      naam : array[0..19] of char;
    end;

  pBgrp = ^Bgrp;
  Bgrp = record
      next : pBgrp;
      prev : pBgrp;
      _type : smallint;
      nr : smallint;
      str : pchar;
      x1 : smallint;
      y1 : smallint;
      x2 : smallint;
      y2 : smallint;
      poin : pchar;
      min : single;
      max : single;
      a1 : smallint;
      a2 : smallint;
      font : pointer;
      col : smallint;
      drawtype : smallint;
      func : pointer;
    end;

  ButIcon = record
      xim : smallint;
      yim : smallint;
      rect : pdword;
      xofs : smallint;
      yofs : smallint;
    end;

procedure AutoButFontSize(size8:pointer; size6:pointer);

//procedure ButtonsGetmouse(adr:Psmallint);

//function DefBut(_type:smallint; nr:smallint; str:Pchar; x1:smallint; y1:smallint;
//           x2:smallint; y2:smallint; poin:pointer; min:single; max:single;
//           a1:smallint; a2:smallint):pBut;

//function DefButt(_type:smallint; nr:smallint; str:Pchar; x1:smallint; y1:smallint;
//           x2:smallint; y2:smallint; poin:pointer; min:single; max:single;
//           a1:smallint; a2:smallint; tip:Pchar):pBut;

//procedure DefButBlock(str:Pchar; win:longint; font:pointer; aantal:smallint; col:smallint;
//            drawtype:smallint);

//procedure ClearButLock;

//procedure FreeButBlock(str:Pchar);

//procedure fdrawline(x1:single; y1:single; x2:single; y2:single);

//procedure getname_menu_but(butname:Pchar; str:Pchar; item:longint);

//procedure SetButFunc(func:pointer );

//function DoButtons:smallint;

//function GetButTip:pchar;

//procedure EmbossBox2(x1:smallint; y1:smallint; x2:smallint; y2:smallint; sel:smallint;
//            dark:cardinal; light:cardinal);

procedure sdrawline(x1: smallint;  y1: smallint;  x2: smallint;  y2: smallint);
procedure sdrawbox(x1: smallint;  y1: smallint;  x2: smallint;  y2: smallint);

procedure DefButCol(nr, draw: word; back, pap_sel, pap_dsel, pen_sel, pen_dsel: cardinal; b1: cardinal = 0; b2: cardinal = 0; b3: cardinal = 0; b4: cardinal = 0);

implementation

uses
GL, util, blendef;

(*  buttons!                 *)
(* 
 ALLEEN VOOR BLENDER!!!
- En dit als initialisatie
 1. Window openen, of windownrs onthouden:
  
 3. Butblock definieren:
  DefButBlock("Naam",win,helv12,max,col,shape);
  - "Naam" is een charpointer, blokken met dezelfde naam
     worden automatisch vrijgegeven en weer gereserveerd
  - win is een int, het GL windownummer
  - helv12 is een (void * )
  - max is een short, het maximaal aantal buttons
  - col is een short, het nummer van de met DefButCol gemaakte
    kleuren. al aanwezig zijn 0 (cmap) en 1 (overdraw)
  - shape is een short, de vorm van de button, 0 is emboss.1 iets mooier, 
   2 alleen geschikt voor RGBmode.
  Alle DefButs na aanroep van DefButBlock horen bij dit blok en
  hebben als default de met DefButBlock bepaalde eigenschappen.
  Veranderd kunnen daarna nog:
   - SetButFont(int)
   - SetButCol(nr)
   - SetButShape(nr)
 4. Kleuren:
  - Bij colormapbuttons:
   ALLEEN OVERDRAW!
  
   Andere cmapkleuren en overdraw zelf instellen met mapcolor en 
   initialiseren met DefButCol().
   Mapcolors eventueel bij einde programma weer terugzetten.
   De drawmode() bljft staan zoals button is getekend.
  - Bij RGB buttons:
   alleen initialiseren DefButCol noodzakelijk.
   
 5. Buttons aanmaken:
  [ b= ] DefBut(type,nr,"naam",x,y,b,h [,adr,min,max,a1,a2,a3...] )
  - b: DefBut geeft (struct But * ) terug.
  - type: aangeven met #defines uit de include. Het bestaat uit 
    maximaal vier gORde getallen:
    soort:
   BUT  activeringsbutton, in en meteen weer uit.
   TOG  toggle, of aan of uit
   ROW  rijbutton, altijd maar 1 van de rij ingedrukt
   SLI  slider
   NUM  met muisbeweging een getal veranderen
   TEX  tekstbutton
   TOG3 driestand toggle
   BUTRET variatie op BUT, keert na indrukken meteen terug
   TOGN (tog NOT) toggle, of aan of uit. Hier zijn 0 en 1 omgekeerd
   LABEL alleen tekst
   MENU werkt als pupmenu. De aktieve optie wordt weergegeven.
   ICONTOG als tog,  volgende icon in plaatje is de ingedrukte versie
   ICONROW er wordt 1 button getekend, met muis kan gescrold worden
   NUMSLI  links staat getal (tiepen),  rechts slider
   HSVSLI  idem: met HSV omrekenpatch
   COL  afbeelding kleur: zowel CHA als FLO
   
    variabele:
   CHA char
   SHO short
   INT int
   FLO float
   FUN functie
    bitcodes:
   BIT activeert bitvariabele
    bitnummer:
   0-31 bitnummer uit variabele (van rechts af)
    voorbeeld voor type:
   BUT  (zonder variabele)
   SLI|CHA  (slider die char verandert)    
   TOG|SHO|BIT|0 (van de short toggelt bitje 0)
  - nr: is de returncode van DoButtons(), of wordt aan SetButtons()
                  doorgegeven. Verschillende buttons kunnen dezelfde nrs hebben.
  - "naam": is charpointer van constante of variabele.
  - x,y,b,h: startco (x,y) en breedte,hoogte.
  - adr: is pointer naar char,short,etc. Als de pointer nul is,  wordt geen
    button aangemaakt,  wel wordt op de plek in kleur 'back' een rechthoek getekend.
    Op deze wijze kan een button gewist worden.
  - min,max (float) bij sliders en numbuts
  - a1,a2,a3... alleen bij bepaalde types buttons (zie verderop) 
 5. Controle functies:
  - afhandeling buttons:
   nr=DoButtons(); (in hoofdlus)
   Deze functie kijkt alleen in de aktieve window: 
   win= winget().
   als nr= 0 is er geen button geselecteerd. Anders is
   nr het returnnr van de button.
  - alles vrijgeven:
   FreeButs();
  - butblock vrijgeven:
   FreeButBlock("naam");
  - alle buts van nr 'min' tot 'max' opnieuw tekenen:
   SetButs(min,max);
   Na deze funktie is de aktieve window ongewijzigd.
  - nieuw kleurblok aanmaken:
   DefButCol(nr,DRAWMODE,back,pap_sel,pap_dsel,pen_sel,pen_dsel,b1,b2,b3,b4);
   Dit is blok 0:
   DefButCol(0,NORMALDRAW,100, 101,102,103,104, 104,105,106,103);
   nr= kleurbloknummer.
   DRAWMODE is RGBDRAW.
   De rest zijn uints,  rgb kleuren.
  - kleur aangeven:
   SetButCol(nr)
   nr is short van 2-20 (0 en 1 reeds aanwezig) of een met
   DefButCol aangemaakt kleurblok
  - frontbuffer tekenen activeren:
   FrontbufferButs(val)
   val= TRUE of FALSE
  - automatische lettergrootte:
   foreach j ( *.c) utFontSize(size8, size6);
   size8 en size6 zijn void pointers, aanmaken zoals onder punt 2.
   (0, 0) doorgeven schakelt autosize af.
   
 6. Ikonen
  - aanmaken iconstruct
   DefButIcon(nr, rect, xim, yim, xofs, yofs);
   nr is nummer van de iconstruct (max 10).
   rect is het 32 bits plaatje.
   xim yim is de totale afmeting.
   xofs,  yofs is het grid waarin de buttons staan
  - Button met ikoon maken: 
   de naam van de button = "ICON 1 12 4" betekent
    iconstruct 1,  icon '12' in de x, '4' in de y
- De Button types:
 1. BUT
  DefBut(BUT[|VAR],nr,"naam",x,y,b,h,[poin]);
  Een variabele doorgeven hoeft niet.
  Als variabele wordt doorgegeven :
   - FUN: funktie wordt aangeroepen bij indrukken
   - alle andere: als variabele=0 button is uit, anders in.
     kan gebruikt worden om status van variabelen aan te geven.
  Type BIT kan hier niet.
 2. TOG / TOGN
  DefBut(TOG|VAR[|BIT|bitnr],nr,"naam",x,y,b,h,poin);
  De variabele wordt afhankelijk van de stand op 0 of 1 gezet.
  Een funktie wordt met 0 of 1 aangeroepen.
  Toevoegen van |BIT|bitnr zet bitnr van de VAR.
  Type TOGN zet button op select bij NOT TRUE.
  
 3. ROW
  DefBut(ROW|VAR,nr,"naam",x,y,b,h,poin,rijnr,const);
  'rijnr' en 'const' altijd als float doorgeven!
  'rijnr' bepaalt welke buttons bij elkaar horen. 'const' wordt de 
  inhoud van 'poin'.
  Vartypes FUN en BIT kunnen hier niet.
 4. SLI en NUMSLI en HSVSLI
  DefBut(SLI|VAR,nr,"naam",x,y,b,h,poin,min,max,colnr,ofs);
  'min' en 'max' altijd als float doorgeven., 
  Als RGB buttons:
   'colnr' bevat de nummer van de COL button
   
  Vartypes FUN en BIT kunnen hier niet.
 5. NUM
  DefBut(NUM|SHO,nr,"Naam",x,y,b,h,poin,min,max, step);
  min en max altijd als float doorgeven!.
  Vartypes FUN en BIT kunnen hier niet.
  Bij vartype FLO 'step' doorgeven: is in honderdsten de increment bij 1xklikken
  SetButFunc: funktie wordt aangeroepen per veranderd getal.
 6. TEX
  DefBut(TEX,nr,"Naam",x,y,b,h,poin,min,max);
  VARtype is standaard CHA. max is lengte string (als float doorgeven)
  als texbut met enter wordt afgesloten wordt but->min op 1.0
  gezet, anders 0. (voor o.a. OK mededelingen)
  SetButFunc: funktie wordt aan eind editen aangeroepen. Geeft string door.
 7. TOG3
  DefBut(TOG3|CHA|BIT|bitnr,nr,"naam",x,y,b,h,poin);
  Een reare button!
  Dit een CHA|BIT button. of SHO|BIT button
  Bij de derde stand wordt 'bitnr' van *( charpoin+2) gezet.
            of *(shortpoin+1)
 8. LABEL
  DefBut(LABEL,nr,"naam",x,y,b,h, *, sel, clear);
  Alleen tekst. Deze wordt gecentreerd rond x+b en y+h.
  (sel en clear als float doorgeven!)
  sel: als 1.0 witte tekst,  else zwart
  clear: als 1.0 wordt een border getekend
 9. MENU
  DefBut(MENU|VAR, nr, string, x, y, b, h, poin);
  In string staat een aantal menuopties,  met syntax zoals pupmenu:
   - menu onderdelen scheiden met '|'
   - returnval aangeven met %x[nr] (bijv: %x12). Zonder returnval wordt
    automatisch geteld: beginnend bij 0.
  In poin wordt de waarde geschreven.
 10. ICONROW
  DefBut(ICONROW|VAR, nr, "ICON a b c", x, y, b, h, poin, min, max);
  De waarde in poin bepaalt welke icon getekend wordt, van
  icon c tot icon (c+max-min).
 11. COL
  DefBut(COL|VAR, nr, "", x, y, b, h, poin);
  Als VAR zijn CHA en FLO toegestaan
  voorlopig alleen RGB geimplementeerd
 12. IDPOIN
  DefBut(IDPOIN, nr, "naam", x, y, b, h, poinpoin);
  alleen voor Blender: IDpointer,  button displayt naam id.
  Ingedrukt is het een tekstbut. LETOP: pointerpointer!.
  Altijd een funktie meeegeven: func(newname, poinpoin);
  
******************************************************   *)
(* Omschrijving ButGroups
 * 
 * 1. ButGroups vormen een extra laag tussen gebruiker en button structuur
 *    die het mogelijk maakt om een relatieve positie / grootte van een
 *    groep buttons binnen een rechthoekig kader aan te geven.
 * 2. Alle functienamen beginnen met BG.
 * 
 * Functies:
 * 
 * BGflush()
 * Wist alle nog onverwerkte buttons. Bij voorkeur aanroepen
 * voordat je begint.
 * BGadd(, , , , ...)
 * Voegt een Button aan de lijst toe. Zelfde definite als DefBut, 
 * met dit verschil dat x en y genegeerd worden, en dat breedte
 * en hoogte relatieve waardes zijn. Een button met breedte 20
 * wordt twee keer zo breed als een button met breedte 10. Als y
 * hoogte voor een regel wordt de maximale y van die regel aan-
 * gehouden. Buttons houden hun eigen hoogte en lijnen afhankelijk
 * van de richting waarin de buttons getekend worden met de boven of
 * onderkant van de regel (niet getest).
 * BGnewline()
 * Aanroepen om aan te geven dat volgende buttons op een nieuwe 
 * regel moeten komen. Meerdere keren aanroepen om extra spatiering
 * tussen regels te krijgen is toegestaan. Niet aanroepen bij begin
 * en eind van BG defintie, tenzij je daar ook spatiering wilt.
 * BGposition(xofs, yofs, width, height)
 * Om kader aan te geven waarbinnen buttons getekent moeten worden.
 * Kan op elk moment binnen BG definite aangeroepen worden, maar in
 * ieder geval voor BGdraw. Start waardes (0, 0, 100, 100).
 * BGspacing(x, y)
 * Om absolute globale spatiering tussen buttons aan te geven.
 * Kan op elk moment binnen BG definite aangeroepen worden, maar in
 * ieder geval voor BGdraw. Start waardes (2, 2).
 * BGdirection(dir)
 * Of buttons van onder naar boven, of van boven naar onder getekend
 * moeten worden. Mogelijke waardes: 'u' (up), of 'd' (down).
 * Kan op elk moment binnen BG definite aangeroepen worden, maar in
 * ieder geval voor BGdraw. Start waarde ('u').
 * BGdraw()
 * Tekent gedefinieerde buttons binnen rechthoek BGposition(), met
 * spatiering BGspacing() in de richting BGdirection(). Hierna wordt
 * de lijst geflushed.
 *)

////{$include "blender.h"}
////{$include "graphics.h"}
////{$include "screen.h"}

(* ************ GLOBALS ************* *)

var
  BGfirstbutblock: pButBlock = nil;

  BGfirst: pBut;
  BGaantal: smallint;
  BGteller: smallint;
  BGwin: smallint;
  BGfont: pinteger = nil;
  BGfonts: pinteger = nil;
  BGfontss: pinteger = nil;
  BGcol: smallint;
  BGdrawtype: smallint;
  ButDrawFunc: procedure;
  SliderDrawFunc: procedure;

//procedure do_textbut;
//procedure SetButsWin(win: smallint;  min: smallint;  max: smallint);

var
Bxasp: single = 1.0;
Bfrontbuf: integer = 0;
BGaddicon: integer = 0;
BGlock: integer = 0;
Btextleft: integer = 0;
Bnotext: integer = 0;
BGlockstr: pchar = nil;

 BGbutcol: array [0..19] of ButCol;

BGicon: array [0..9] of ButIcon;

(* ************ BUTGRP GLOBALS ************* *)

_butbase : ListBase = (first: nil; last: nil);
butbase: pListBase;
BG_xsp: word = 2;
BG_ysp: word = 2;
BG_w: word = 100;
BG_h: word = 100;
BG_x: smallint = 0;
BG_y: smallint = 0;
BG_dir: char = 'u';

(* ******************************** *)

//procedure drawcolorband(coba: pColorBand;  x1: single;  y1: single;  sizex: single;  sizey: single);
//var
//cbd: pCBData;
//v3: array [0..1] of single;
//v1: array [0..1] of single;
//v2: array [0..1] of single;
//a: integer;
//begin
//  if coba=0 then
//  exit;
//  (* outline *)
//  v1[0]:= x1;
//  v1[1]:= y1;
//  glLineWidth( {GLfloat(}(3));
//  cpack($0);
//  glBegin(GL_LINE_LOOP);
//  glVertex2fv(v1);
//  v1[0]:= v1[0] + (sizex);
//  glVertex2fv(v1);
//  v1[1]:= v1[1] + (sizey);
//  glVertex2fv(v1);
//  v1[0]:= v1[0] - (sizex);
//  glVertex2fv(v1);
//  glEnd();
//  glLineWidth( {GLfloat(}(1));
//  glShadeModel(GL_SMOOTH);
//  cbd:= coba.data;
//  v1[0]:= v2[0]:=x1;
//  v1[1]:= y1;
//  v2[1]:= y1+sizey;
//  glBegin(GL_QUAD_STRIP);
//  glColor3fv( and cbd.r);
//  glVertex2fv(v1);
//  glVertex2fv(v2);
//  while{for}
//  a:= 0;
//  {to} a<coba.tot
//  {;}
//  inc(a);
//  inc(cbd);
//  do
//  begin
//    v1[0]:= v2[0]:=x1+cbd.pos*sizex;
//    glColor3fv( and cbd.r);
//    glVertex2fv(v1);
//    glVertex2fv(v2);
//  end;
//  v1[0]:= v2[0]:=x1+sizex;
//  glVertex2fv(v1);
//  glVertex2fv(v2);
//  glEnd();
//  glShadeModel(GL_FLAT);
//  v1[0]:= v2[0]:=v3[0]:=x1;
//  v1[1]:= y1;
//  v2[1]:= y1+0.5*sizey;
//  v3[1]:= y1+sizey;
//  cbd:= coba.data; (* hulplijntjes *)
//  while{for}
//  a:= 0;
//  {to} a<coba.tot
//  {;}
//  inc(a);
//  inc(cbd);
//  do
//  begin
//    v1[0]:= v2[0]:=v3[0]:=x1+cbd.pos*sizex;
//    if a=coba.cur then
//    glLineWidth( {GLfloat(}(3));
//    cpack($0);
//    LINE2F(v1,v2);
//    cpack($FFFFFF);
//    LINE2F(v2,v3);
//    if a=coba.cur then
//    begin
//      glLineWidth( {GLfloat(}(1));
//    end;
//  end;
//end;

(* ************ FUNC ************* *)

procedure AutoButFontSize(size8: pointer;  size6: pointer);
begin
  BGfonts:= size8;
  BGfontss:= size6;
end;

//procedure myfmsetfont(font: pinteger);
//begin
//  if BGfonts=0)or(Bxasp<1.15 then
//  BGfont:= font;
//  else
//  if BGfontss=0)or(Bxasp<1.59 then
//  BGfont:= BGfonts;
//  else
//  BGfont:= BGfontss;
//  fmsetfont(BGfont);
//end;

//procedure disable_capslock(val: integer);
//begin
//end;

//procedure FrontbufferButs(val: integer);
//begin
//  Bfrontbuf:= val;
//end;
//var
//Bwinmat: array [0..3,0..3] of single;
//getsizex: integer;
//getsizey: integer; void{!!!e unknown token}(* voor rectwrite b.v. *)
//x: pfloat;
//y: pfloat;
//begin
//  gx: single;
//  gy: single;
//  sx: integer;
//  sy: integer;
//  gx:= {*}x^;
//  gy:= {*}y^; {*}x^:=getsizex*(0.5+0.5*(gx*Bwinmat[0][0]+gy*Bwinmat[1][0]+Bwinmat[3][0]));
//  {*}y^:=getsizey*(0.5+0.5*(gx*Bwinmat[0][1]+gy*Bwinmat[1][1]+Bwinmat[3][1]));
//  mygetsuborigin(@sx,@sy);
//  {*}x^+=sx;
//  {*}y^+=sy;
//end;
//void{!!!e unknown token}(* voor muiscursor b.v. *)
//x: pfloat;
//y: pfloat;
//begin
//  a: single;
//  b: single;
//  c: single;
//  d: single;
//  e: single;
//  f: single;
//  px: single;
//  py: single;
//  a:= .5*getsizex*Bwinmat[0][0];
//  b:= .5*getsizex*Bwinmat[1][0];
//  c:= .5*getsizex*(1.0+Bwinmat[3][0]);
//  d:= .5*getsizey*Bwinmat[0][1];
//  e:= .5*getsizey*Bwinmat[1][1];
//  f:= .5*getsizey*(1.0+Bwinmat[3][1]);
//  px:= {*}x^;
//  py:= {*}y^; {*}y^:=(a*(py-f)+d*(c-px)) div (a*e-d*b);
//  {*}x^:=(px-b*(*y)-c) div a;
//end;
//var {was static}
//xmax: integer;
//x: integer;
//y: integer;
//addx: smallint;

//mvalo: array [0..1] of smallint;

//procedure ButtonsGetmouse(adr: psmallint);
//var
//xwin: single;
//ywin: single;
//xt: single;
//yt: single;
//automove: integer;
//begin(* map muiscoordinaat invers naar geprojecteerde coordinaat *)

//  addx:=0;
//  mvalo:=(1,1);

//  if adr=0 then
//  begin
//    xmax:= displaysizex-1;
//    mygetsingmatrix(Bwinmat);
//    getsize(@getsizex,@getsizey);
//    Bxasp:= 2.0 div (getsizex*Bwinmat[0][0]);
//    exit;
//  end;
//  getmouseco_sc(adr);
//  mygetsuborigin(@x,@y);
//  adr[0]:= adr[0] - (x);
//  adr[1]:= adr[1] - (y);
//  automove:= 0; (* testen: als aan rand scherm, automatisch mouseco's maken *)
//  if mvalo[0]+x<=0)and(adr[0]+x<=0 then
//  automove:= 1;
//  else
//  if mvalo[0]+x>=xmax)and(adr[0]+x>=xmax then
//  automove:= 2;
//  if automove<>0 then
//  begin
//    if automove=1 then
//    addx:= addx - (4);
//    else
//    addx:= addx + (4);
//    if addx>2*xmax then
//    addx:= 2*xmax;
//    else
//    if addx<-xmax then
//    addx:= -xmax;
//    usleep(30);
//    adr[0]:= adr[0] + (addx);
//    (* at BeOS doesn't work when there's an event! *)
//  end;
//  else
//  begin
//    addx:= 0;
//    mvalo[0]:= adr[0];
//    mvalo[1]:= adr[1];
//  end;
//  xwin:= adr[0];
//  ywin:= adr[1];
//  window_to_graphics(@xwin,@ywin);
//  adr[0]:=  {smallint(}(xwin+0.5);
//  adr[1]:=  {smallint(}(ywin+0.5);
//end;
//void{!!!e unknown token}
//nr: smallint;
//rect: puint;
//xim: smallint;
//yim: smallint;
//xofs: smallint;
//yofs: smallint;
//begin
//  BGicon[nr].rect:= rect;
//  BGicon[nr].xim:= xim;
//  BGicon[nr].yim:= yim;
//  BGicon[nr].xofs:= xofs;
//  BGicon[nr].yofs:= yofs;
//end;
//temprect: array [0..Pred(40*40*4)] of uint; void{!!!e unknown token}
//nr: smallint;
//paper: uint;
//xnr: smallint;
//ynr: smallint;
//x: smallint;
//y: smallint;
//high: smallint; (* xnr ynr is iconnr; x,y is tekencentrum *)
//begin
//  rii: puint; (* recticon *)
//  xs: single;
//  ys: single;
//  a: integer;
//  b: integer;
//  sizea: integer;
//  sizeb: integer;
//  rfac: integer = 256;
//  gfac: integer = 256;
//  bfac: integer = 256;
//  fac: integer;
//  rd: pchar;
//  ri: pchar;
//  col: pchar; (* rectdraw, recticon *)
//  rd:=  {pchar(}temprect;
//  rii:= BGicon[nr].rect;
//  if rii=0 then
//  begin
//    printf('Non existing iconrect: %d\n',nr);
//    exit;
//  end;
//  rii:= rii + ((3+ynr*BGicon[nr].yofs)*BGicon[nr].xim+xnr*BGicon[nr].xofs+3);
//  sizea:= BGicon[nr].xofs-5;
//  sizeb:= BGicon[nr].yofs-5; (* eerste pixels zijn zwart: grid, en daarbij rand: 3 pixels totaal offset*)
//  (* en natuurlijk de andere rand eraf: *)
//  if {*}rii^ then
//  begin
//    col:= (char* )@paper;
//    ri:=  {pchar(}rii; (* ABGR *)
//    (* eerste kleur icon==paperkleur *)
//    if ri[0])and(ri[1])and(ri[2] then
//    begin
//      rfac:= (col[RCOMP] shl 8) div ri[0];
//      gfac:= (col[GCOMP] shl 8) div ri[1];
//      bfac:= (col[BCOMP] shl 8) div ri[2];
//    end;
//  end;
//  for{while} b:=sizeb downto Succ(0) { b--}
//  do
//  begin
//    ri:=  {pchar(}rii;
//    while{for}
//    a:= sizea;
//    {to} a>0
//    {;}
//    dec(a);
//    ri:= ri + (4);
//    rd:= rd + (4);
//    do
//    begin
//      fac:= (rfac*ri[0]) shr 8;
//      if fac>255 then
//      rd[0]:= 255;
//      else
//      rd[0]:= fac;
//      fac:= (gfac*ri[1]) shr 8;
//      if fac>255 then
//      rd[1]:= 255;
//      else
//      rd[1]:= fac;
//      fac:= (bfac*ri[2]) shr 8;
//      if fac>255 then
//      rd[2]:= 255;
//      else
//      rd[2]:= fac;
//    end;
//    rii:= rii + (BGicon[nr].xim);
//  end;
//  rii:= temprect;
//  high:= high div (Bxasp);
//  if sizeb>high-5 then
//  begin
//    b:= (sizeb-(high-5)) div 2;
//    rii:= rii + (b*sizea);
//    sizeb:= sizeb - (b);
//  end;
//  if sizeb<2 then
//  exit;
//  x:= x - (sizea div 2);
//  y:= y - ((sizeb+1) div 2);
//  xs:= x;
//  ys:= y;
//  x:= ffloor(xs);
//  y:= ffloor(ys+0.51);
//  {$ifdef IRISGL}
//  glClearFlag();
//  {$endif}
//  glRasterPos2i(x,y);
//  if Bxasp>1.1 then
//  glPixelZoom(1.0 div Bxasp,1.0 div Bxasp);
//  glDrawPixels(sizea,sizeb,GL_RGBA,GL_UNSIGNED_BYTE,rii);
//  glPixelZoom(1.0,1.0);
//end;

//procedure fdrawline(x1: single;  y1: single;  x2: single;  y2: single);
//var
//v: array [0..1] of float;
//begin

//  glBegin(GL_LINE_STRIP);
//  v[0]:= x1;
//  v[1]:= y1;
//  glVertex2fv(v);
//  v[0]:= x2;
//  v[1]:= y2;
//  glVertex2fv(v);
//  glEnd();
//end;

//procedure fdrawbox(x1: single;  y1: single;  x2: single;  y2: single);
//var
//v: array [0..1] of float;
//begin

//  glBegin(GL_LINE_STRIP);
//  v[0]:= x1;
//  v[1]:= y1;
//  glVertex2fv(v);
//  v[0]:= x1;
//  v[1]:= y2;
//  glVertex2fv(v);
//  v[0]:= x2;
//  v[1]:= y2;
//  glVertex2fv(v);
//  v[0]:= x2;
//  v[1]:= y1;
//  glVertex2fv(v);
//  v[0]:= x1;
//  v[1]:= y1;
//  glVertex2fv(v);
//  glEnd();
//end;

procedure sdrawline(x1: smallint;  y1: smallint;  x2: smallint;  y2: smallint);
var
v: array [0..1] of smallint;
begin

  glBegin(GL_LINE_STRIP);
  v[0]:= x1;
  v[1]:= y1;
  glVertex2sv(v);
  v[0]:= x2;
  v[1]:= y2;
  glVertex2sv(v);
  glEnd();
end;

procedure sdrawbox(x1: smallint;  y1: smallint;  x2: smallint;  y2: smallint);
var
v: array [0..1] of smallint;
begin

  glBegin(GL_LINE_STRIP);
  v[0]:= x1;
  v[1]:= y1;
  glVertex2sv(v);
  v[0]:= x1;
  v[1]:= y2;
  glVertex2sv(v);
  v[0]:= x2;
  v[1]:= y2;
  glVertex2sv(v);
  v[0]:= x2;
  v[1]:= y1;
  glVertex2sv(v);
  v[0]:= x1;
  v[1]:= y1;
  glVertex2sv(v);
  glEnd();
end;

//type
//But = record
//end;

//procedure FreeButBlockData(bb: pButBlock);
//var
//a: integer;
//begin

//  but:= bb.first;
//  while{for}
//  a:= 0;
//  {to} a<bb.aantal
//  {;}
//  inc(a);
//  inc(but);
//  do
//  begin
//    if but.str)and(but.str<>but.strdata then
//    freeN(but.str);
//  end;
//  freeN(bb.first);
//end;
//type
//ButBlock = record
//end;

//procedure FreeButs(mode: char);
//var
//a: smallint;
//begin

//  bb:= BGfirstbutblock;
//  while bb
//  do
//  begin
//    bn:= bb.next;
//    FreeButBlockData(bb);
//    freeN(bb);
//    bb:= bn;
//  end;
//  BGfirstbutblock:= 0;
//  if mode<>'i' then
//  begin
//    for{while} a:=0 to Pred(10) { a++}
//    do
//    begin
//      if BGicon[a].rect<>0 then
//      freeN(BGicon[a].rect);
//    end;
//  end;
//end;

//function GetButVal(but: pBut;  fvalue: pfloat;  lvalue: pinteger): smallint;
//var
//type: smallint;
//format: smallint;
// (* geeft 0 als geen val, 1 is int, 2 is float *)
//poin: pinteger;
//h: single;
//s: single;
//v: single;
//fp: pfloat;

//begin

//  format:=0; (* geeft 0 als geen val, 1 is int, 2 is float *)

//  type:= but.type and BUTPOIN;
//  poin:= but.poin;
//  if (but.type and BUTTYPE)=HSVSLI then
//  begin

//    fp:= {pfloat(}poin;
//    rgb_to_hsv(fp[0],fp[1],fp[2],@h,@s,@v);
//    case but.str[0] of
//      'H':
//      begin
//        {*}fvalue^:=h;
//      end;
//      'S':
//      begin
//        {*}fvalue^:=s;
//      end;
//      'V':
//      begin
//        {*}fvalue^:=v;
//      end;

//    end;{case?}
//    format:= 2;
//  end;
//  else
//  if type=CHA then
//  begin
//    {*}lvalue^:=* {pchar(}poin;
//    format:= 1;
//  end;
//  else
//  if type=SHO then
//  begin
//    {*}lvalue^:=* {psmallint(}poin;
//    format:= 1;
//  end;
//  else
//  if type=INT then
//  begin
//    {*}lvalue^:=* {pinteger(}poin;
//    format:= 1;
//  end;
//  else
//  if type=FLO then
//  begin
//    {*}fvalue^:=* {pfloat(}poin;
//    format:= 2;
//  end;
//  if format=1 then
//  {*}fvalue^:=(float)*lvalue;
//  else
//  if format=2 then
//  {*}lvalue^:=(int)*fvalue;
//  begin
//    result:= format;
//    exit;
//  end;
//end;

//procedure SetButVal(but: pBut;  fvalue: single;  lvalue: integer);
//var
//type: smallint;
//poin: pinteger;
//h: single;
//s: single;
//v: single;
//fp: pfloat;

//begin

//  type:= but.type and BUTPOIN;
//  poin:= but.poin; (* value is een hsvwaarde: omzetten naar de rgb *)
//  if (but.type and BUTTYPE)=HSVSLI then
//  begin

//    fp:= {pfloat(}but.poin;
//    rgb_to_hsv(fp[0],fp[1],fp[2],@h,@s,@v);
//    case but.str[0] of
//      'H':
//      begin
//        h:= fvalue;
//      end;
//      'S':
//      begin
//        s:= fvalue;
//      end;
//      'V':
//      begin
//        v:= fvalue;
//      end;

//    end;{case?}
//    hsv_to_rgb(h,s,v,fp,fp+1,fp+2);
//  end;
//  else
//  if type=CHA then
//  *( {pchar(}poin):= {char(}lvalue;
//  else
//  if type=SHO then
//  *( {psmallint(}poin):= {smallint(}lvalue;
//  else
//  if type=INT then
//  *( {pinteger(}poin):=lvalue;
//  else
//  if type=FLO then
//  *( {pfloat(}poin):=fvalue;
//end;

//procedure SetButFont(font: pinteger);
//begin
//  BGfont:= font;
//end;

//procedure SetButCol(nr: smallint);
//begin
//  BGcol:= nr;
//end;

//procedure SetButLock(val: integer;  lockstr: pchar);
//begin
//  BGlock:= BGlock or (val);
//  if val<>0 then
//  BGlockstr:= lockstr;
//end;

//procedure ClearButLock;
//begin
//  BGlock:= 0;
//  BGlockstr:= 0{nil};
//end;

procedure DefButCol(nr, draw: word; back, pap_sel, pap_dsel, pen_sel, pen_dsel: cardinal; b1: cardinal = 0; b2: cardinal = 0; b3: cardinal = 0; b4: cardinal = 0);
var
  bc: pButCol;
  cp1, cp2: pchar;
begin
	bc:= @BGbutcol[nr];
	bc^.back:= back;
	bc^.drmode:= draw;
	if draw=RGBDRAW then
        begin
		bc^.drmode:= NORMALDRAW;
		bc^.rgb:= 1;
        end
	else bc^.rgb:= 0;

	bc^.paper_sel:= pap_sel;
	bc^.paper_desel:= pap_dsel;
	bc^.pen_sel:= pen_sel;
	bc^.pen_desel:= pen_dsel;
	bc^.border1:= b1;
	bc^.border2:= b2;
	bc^.border3:= b3;
	bc^.border4:= b4;

	if(draw=RGBDRAW) then
        begin
		cp1:= pchar(@pap_sel);

		(* white *)
		cp2:= pchar(@bc^.border1);
		if(cp1[BCOMP+60]>#255) then cp2[BCOMP]:= #255 else cp2[BCOMP]:= cp1[BCOMP+60];
		if(cp1[GCOMP+60]>#255) then cp2[GCOMP]:= #255 else cp2[GCOMP]:= cp1[GCOMP+60];
		if(cp1[RCOMP+60]>#255) then cp2[RCOMP]:= #255 else cp2[RCOMP]:= cp1[RCOMP+60];

		(* light *)
		cp2:= pchar(@bc^.border2);
		if(cp1[BCOMP+35]>#255) then cp2[BCOMP]:= #255 else cp2[BCOMP]:= cp1[BCOMP+35];
		if(cp1[GCOMP+35]>#255) then cp2[GCOMP]:= #255 else cp2[GCOMP]:= cp1[GCOMP+35];
		if(cp1[RCOMP+35]>#255) then cp2[RCOMP]:= #255 else cp2[RCOMP]:= cp1[RCOMP+35];

		(* grey *)
		cp2:= pchar(@bc^.border3);
		if(cp1[BCOMP-35]<#0) then cp2[BCOMP]:= #0 else cp2[BCOMP]:= cp1[BCOMP-35];
		if(cp1[GCOMP-35]<#0) then cp2[GCOMP]:= #0 else cp2[GCOMP]:= cp1[GCOMP-35];
		if(cp1[RCOMP-35]<#0) then cp2[RCOMP]:= #0 else cp2[RCOMP]:= cp1[RCOMP-35];

		(* dark *)
		cp2:= pchar(@bc^.border4);
		if(cp1[BCOMP-60]<#0) then cp2[BCOMP]:= #0 else cp2[BCOMP]:= cp1[BCOMP-60];
		if(cp1[GCOMP-60]<#0) then cp2[GCOMP]:= #0 else cp2[GCOMP]:= cp1[GCOMP-60];
		if(cp1[RCOMP-60]<#0) then cp2[RCOMP]:= #0 else cp2[RCOMP]:= cp1[RCOMP-60];

	end;
end;

//function IsButSel(b: pBut): smallint;
//var
//value: single;
//lvalue: integer;
//a: smallint;
//push: smallint;

//true: smallint;

//begin

//  push:=0;
//  true:=1;
//  a:= GetButVal(b,@value,@lvalue);
//  if a=0 then
//  begin
//    result:= 0;
//    exit;
//  end;
//  if (b.type and BUTTYPE)=TOGN then
//  true:= 0;
//  if (b.type and BIT) then
//  begin
//    if BTST(lvalue,(b.type and 31)) then
//    begin
//      result:= true;
//      exit;
//    end;
//    begin
//      result:= {not}0=true;
//      exit;
//    end;
//  end;
//  case b.type and BUTTYPE of
//    BUT:
//    begin
//      push:= 0;
//    end;
//    TOG,
//    ICONTOG:
//    begin
//      if value<>0.0 then
//      push:= 1;
//    end;
//    TOGN:
//    begin
//      if value=0.0 then
//      push:= 1;
//    end;
//    ROW:
//    begin
//      if value=b.max then
//      push:= 1;
//    end;

//  end;{case?}
//  begin
//    result:= push;
//    exit;
//  end;
//end;
//(* ************** DRAW ******************* *)
//type
//ButCol = record
//end;

//procedure EmbossBoxN(x1: single;  y1: single;  x2: single;  y2: single;  sel: integer);
//var
//asp1: single;
//asp2: single;
//begin

//  asp1:= Bxasp;
//  asp2:= asp1+Bxasp;
//  if sel<>0 then
//  begin
//    cpack(bc.border3);
//    fdrawline(x1+asp1,y2+asp1,x2-asp1,y2+asp1);
//    cpack(bc.border4);
//    fdrawbox(x1,y1,x2,y2);
//    x1:= x1 + (asp1);
//    x2:= x2 - (asp1);
//    y1:= y1 + (asp1);
//    y2:= y2 - (asp1);
//    cpack($0);
//    fdrawbox(x1,y1,x2,y2);
//    cpack(bc.border1);
//    fdrawline(x1,y1-asp1,x2,y1-asp1);
//    cpack(bc.border2);
//    fdrawline(x1-asp1,y1-asp2,x2+asp1,y1-asp2);
//  end;
//  else
//  begin
//    cpack(0);
//    fdrawbox(x1,y1,x2,y2);
//    x1:= x1 + (asp1);
//    x2:= x2 - (asp1);
//    y1:= y1 + (asp1);
//    y2:= y2 - (asp1);
//    cpack(bc.border1);
//    fdrawline(x1,y2,x2,y2);
//    fdrawline(x1,y1+asp1,x1,y2-asp1);
//    cpack(bc.border2);
//    fdrawline(x1,y2-asp1,x2,y2-asp1);
//    fdrawline(x1+asp1,y1+asp2,x1+asp1,y2-asp1);
//    fdrawline(x2,y1+asp1,x2,y2-asp1);
//    cpack(bc.border3);
//    fdrawline(x1+asp1,y1+asp1,x2-asp1,y1+asp1);
//    cpack(bc.border4);
//    fdrawline(x1,y1,x2,y1);
//    fdrawline(x1,y1-asp2,x2,y1-asp2);
//  end;
//end;

//procedure EmbossBox2(x1: smallint;  y1: smallint;  x2: smallint;  y2: smallint;  sel: smallint;  dark: uint;  light: uint);
//begin
//  if sel<>0 then
//  cpack(dark);
//  else
//  cpack(light);
//  glRects(x1,y2,x2,y2-3);
//  glRects(x1,y1,x1+3,y2);
//  (* boven *)
//  (* links *)
//  if sel<>0 then
//  cpack(light);
//  else
//  cpack(dark);
//  glRects(x1,y1,x2,y1+2);
//  glRects(x2,y1,x2-2,y2);
//  (* onder *)
//end;(* rechts *)

//void{!!!e unknown token}
//var
//str1: pchar;
//str2: pchar;
//f: single;
//x1: smallint;
//y1: smallint;
//x2: smallint;
//y2: smallint;
//sel: smallint;
//col: smallint;
//oud: psmallint;
//begin
//  (* s1: getal, s2: naam, f: van 0.0 tot 1.0.
//      '*oud' zit in butstruct, is oude lengte getalstring *)
//  type
//  ButCol = record
//end;
//var
//s: smallint;
//h: smallint;
//h:= (y2-y1);
//bc:=  and BGbutcol[col];
//cpack($505050);
//glRectf(x1,y1,x2,y2);
//EmbossBoxN( {float(}x1-1, {float(}y1-1, {float(}x2+1, {float(}y2+1,1);
//(* het blokje *)
//sel{!!!e unknown token} {was ?}if then cpack(bc.border1) {was :}else cpack(bc.paper_sel);
//glRects(x1+f,y1+1,x1+h+f,y2-1);
//cpack(bc.border2);
//fdrawline(x1+f,y2-1,x1+f+h,y2-1);
//cpack($0);
//fdrawline(x1+f,y1+1,x1+f+h,y1+1);
//if Bnotext<>0
//then
//exit;
//(* tekst rechts *)
//h:= 12;
//s:= Bxasp*fmgetstrwidth(BGfont,str2);
//cpack(bc.back);
//glRectf(x2+8,y1-3,x2+8+s,y1+h-5);
//cpack(bc.pen_sel);
//glRasterPos2i(x2+8,y1-2);
//fmprstr(str2);
//s:= Bxasp*fmgetstrwidth(BGfont,str1);
//cpack(bc.back);
//(* tekst links *)
//if {*}oud^=0
//then
//{*}oud^:=s;
//glRectf(x1-*oud-8,y1-3,x1-8,y1+h-4);
//{*}oud^:=s;
//cpack(bc.pen_sel);
//glRasterPos2i(x1-s-8,y1-2);
//fmprstr(str1);
//end;
//void{!!!e unknown token}(* CMAP met ditherlijntjes en zwart kader *)
//str: pchar;
//x1: smallint;
//y1: smallint;
//x2: smallint;
//y2: smallint;
//sel: smallint;
//col: smallint;
//begin
//  type
//  ButCol = record
//end;
//var
//s: smallint;
//tbox_embossbox(x1,y1,x2,y2,sel);
//if str[0]=0
//then
//exit;
//s:= Bxasp*fmgetstrwidth(BGfont,str);
//if Btextleft<>0
//then
//x1:= x1+4;
//else
//x1:= (x1+x2-s+1) div 2;
//y1:= (y1+y2-12 div 2-1) div 2;
//glRasterPos2i(x1,y1);
//fmprstr(str);
//end;
//void{!!!e unknown token}(* alleen RGBmode! *)
//str: pchar;
//x1: smallint;
//y1: smallint;
//x2: smallint;
//y2: smallint;
//sel: smallint;
//col: smallint;
//begin
//  type
//  ButCol = record
//end;
//var
//iconr: integer;
//icox: integer;
//icoy: integer;
//s: smallint;
//x: smallint;
//y: smallint;
//cp1: pchar;
//cp2: pchar;
//SetButCol(col);
//bc:=  and BGbutcol[col]; (* background *)
//if sel<>2
//then
//begin
//  if sel<>0 then
//  cpack(bc.border3);
//  else
//  cpack(bc.paper_sel);
//  glRects(x1+1,y1+1,x2-1,y2-1);
//end;
//EmbossBoxN( {float(}x1, {float(}y1, {float(}x2, {float(}y2,sel);
//(* after this function, textbutton needs color *)
//if sel<>0
//then
//cpack(bc.pen_sel);
//else
//cpack($0);
//if strncmp(str,'ICON',4)=0
//then
//begin
//  sscanf(str+4,'%d %d %d\n',@iconr,@icox,@icoy);
//  bDrawIcon(iconr,bc.paper_sel,icox+BGaddicon,icoy,(x1+x2) div 2,(y1+y2) div 2,(y2-y1));
//end;
//else
//if str[0]<>0
//then
//begin
//  while (s:=Bxasp*fmgetstrwidth(BGfont,str))>x2-x1
//  do
//  begin
//    if s<10 then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    if str[1]=0 then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    inc(str);
//  end;
//  if Btextleft<>0 then
//  x:= x1+4;
//  else
//  x:= (x1+x2-s+1) div 2;
//  glRasterPos2f( {float(}x, {float(}(y1+y2-12 div 2-3.0) div 2.0);
//  fmprstr(str);
//end;
//end;
//void{!!!e unknown token}(* textbuttons alleen RGBmode! *)
//str: pchar;
//x1: smallint;
//y1: smallint;
//x2: smallint;
//y2: smallint;
//sel: smallint;
//col: smallint;
//begin
//  type
//  ButCol = record
//end;
//var
//iconr: integer;
//icox: integer;
//icoy: integer;
//s: smallint;
//x: smallint;
//y: smallint;
//cp1: pchar;
//cp2: pchar;
//SetButCol(col);
//bc:=  and BGbutcol[col]; (* background *)
//if sel<>2
//then
//begin
//  cpack(bc.paper_sel);
//  glRects(x1+1,y1+1,x2-1,y2-1);
//end;
//EmbossBoxN( {float(}x1, {float(}y1, {float(}x2, {float(}y2,1);
//(* after this function, textbutton needs color *)
//if sel<>0
//then
//cpack(bc.pen_sel);
//else
//cpack($0);
//if strncmp(str,'ICON',4)=0
//then
//begin
//  sscanf(str+4,'%d %d %d\n',@iconr,@icox,@icoy);
//  bDrawIcon(iconr,bc.paper_sel,icox+BGaddicon,icoy,(x1+x2) div 2,(y1+y2) div 2,(y2-y1));
//end;
//else
//if str[0]<>0
//then
//begin
//  while (s:=Bxasp*fmgetstrwidth(BGfont,str))>x2-x1
//  do
//  begin
//    if s<10 then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    if str[1]=0 then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    inc(str);
//  end;
//  if Btextleft<>0 then
//  x:= x1+4;
//  else
//  x:= (x1+x2-s+1) div 2;
//  glRasterPos2f( {float(}x, {float(}(y1+y2-12 div 2-3.0) div 2.0);
//  fmprstr(str);
//end;
//end;
//void{!!!e unknown token}
//str: pchar;
//x1: smallint;
//y1: smallint;
//x2: smallint;
//y2: smallint;
//sel: smallint;
//col: smallint;
//begin
//  y: integer;
//  if str[0]<>0 then
//  begin
//    if sel<>0 then
//    cpack($FFFFFF);
//    else
//    cpack($0);
//    y:= (y1+y2-12 div 2-2) div 2;
//    glRasterPos2i(x1,y);
//    fmprstr(str);
//  end;
//end;

//function tekentekstbut(b: pBut;  s: pchar;  pos: psmallint): smallint;
//var
//temp: smallint;
//h: smallint;
//t: smallint;
//texcol: smallint;
//ofs: smallint;
//s1: array [0..Pred(150)] of char;
//ch: char;
//begin

//  h:= (b.y1+b.y2-12 div 2-2) div 2;
//  s1[0]:= 0;
//  if {*}pos^=-1 then
//  begin
//    (* niet aktieve tekstbut *)
//    ButDrawFunc(s1,b.x1,b.y1,b.x2,b.y2,0,b.col);
//    strcpy(s1,b.str);
//    strcat(s1,s);
//    ofs:= 0;
//    while Bxasp*fmgetstrwidth(BGfont,s1+ofs)>(b.x2-b.x1-10)
//    do
//    inc(ofs);
//    glRasterPos2i(b.x1+4,h);
//    cpack(BGbutcol[b.col].pen_desel);
//    fmprstr(s1+ofs);
//  end;
//  else
//  begin
//    (*  aktieve tekstbut *)
//    ButDrawFunc(s1,b.x1,b.y1,b.x2,b.y2,1,b.col);
//    strcpy(s1,b.str);
//    strcat(s1,s);
//    if {*}pos^<lstrlen(b.str) then
//    {*}pos^:=lstrlen(b.str);
//    if {*}pos^>lstrlen(s1) then
//    {*}pos^:=lstrlen(s1);
//    ch:= s1[{*}pos^];
//    s1[{*}pos^]:= 0;
//    ofs:= 0;
//    while Bxasp*fmgetstrwidth(BGfont,s1+ofs)>(b.x2-b.x1-10)
//    do
//    inc(ofs);
//    t:= Bxasp*fmgetstrwidth(BGfont,s1+ofs)+3;
//    cpack($FF);
//    (* de cursor *)
//    glRects(b.x1+t,b.y1+2,b.x1+t+3,b.y2-2);
//    texcol:= BGbutcol[b.col].pen_sel;
//    if ofs<>0 then
//    begin
//      cpack(texcol);
//      glRasterPos2i(b.x1+4,h);
//      fmprstr(s1+ofs);
//    end;
//    if ofs=0 then
//    begin
//      s1[{*}pos^]:= ch;
//      temp:= 0;
//      while Bxasp*fmgetstrwidth(BGfont,s1+temp)>(b.x2-b.x1-10)
//      do
//      inc(temp);
//      cpack(texcol);
//      glRasterPos2i(b.x1+4,h);
//      s1[lstrlen(s1)-temp]:= 0;
//      fmprstr(s1);
//    end;
//  end;
//  begin
//    result:= (ofs);
//    exit;
//  end;
//end;
//(* *********** PC PATCH ************* *)
//{$ifdef glIndexi}
//{$undef glIndexi}
//{$endif}

//function glIndexi(i: integer): integer; {<= !!!6 unknown macro}
//(* ****** MENUBUT ****** *)
//{tbwinmat: array [0..3,0..3] of float; }{<= !!!5 external variable}
//{tbprojmat: array [0..3,0..3] of float; }{<= !!!5 external variable}
//{oldmap: array [0..3,0..2] of smallint; }{<= !!!5 external variable}
//{tboldwin: smallint; }{<= !!!5 external variable}
//{tbpat: array [0..15] of word; }{<= !!!5 external variable}
//{tbfontyofs: integer; }{<= !!!5 external variable}void{!!!e unknown token}
//startx: integer;
//starty: integer;
//width: integer;
//height: integer;
//items: integer;
//title: pchar; char{!!!e unknown token}{*}spoin^;
//rows: integer;
//columns: integer;
//begin
//  a: integer;
//  x1: integer;
//  y1: integer;
//  glIndexi(TBOXBLACK);
//  if title<>0 then
//  begin
//    x1:= startx;
//    y1:= starty+4+rows*height;
//    tbox_embossbox(x1,y1,x1+width*columns,y1+height,0);
//    glRasterPos2i(x1+5,y1+tbfontyofs);
//    fmprstr(title);
//  end;
//  for{while} a:=0 to Pred(items) { a++}
//  do
//  begin
//    x1:= startx+width*( {integer(}a div rows);
//    y1:= starty-height*(a mod rows)+(rows-1)*height;
//    tbox_embossbox(x1,y1,x1+width,y1+height,2);
//    glRasterPos2i(x1+5,y1+tbfontyofs);
//    fmprstr(spoin[a]);
//  end;
//end;

//procedure getname_menu_but(butname: pchar;  str: pchar;  item: integer);
//var
//itemcount: integer;

//perc: integer;

//bstr: pchar;
//begin
//  itemcount:=0;
//  perc:=0;

//  bstr:= butname;
//  while {*}str^
//  do
//  begin
//    case {*}str^ of
//      '%':
//      begin
//        if str[1]='t' then
//        begin
//          dec(itemcount);
//        end;
//        else
//        if str[1]='x' then
//        begin
//          perc:= 1;
//          if atoi(str+2)=item then
//          begin
//            bstr[0]:= 0;
//            exit;
//          end;
//        end;
//      end;
//      '|':
//      begin
//        if perc=0)and(itemcount=item then
//        begin
//          bstr[0]:= 0;
//          exit;
//        end;
//        inc(itemcount);
//        bstr:= butname;
//      end;
//      else
//      begin
//        {*}bstr^:=*str;
//        inc(bstr);
//      end;
//    end;{case?}
//    inc(str);
//  end;
//  if itemcount<>item then
//  butname[0]:= 0;
//end;

//function domenubut(but: pBut): integer;
//var
//fvalue: single;
//width: integer;
//height: integer;
//a: integer;
//retval: array [0..Pred(100)] of integer;
//retp: pinteger;
//items: integer;
//xmax: integer;
//ymax: integer;
//startx: integer;
//starty: integer;
//endx: integer;
//endy: integer;
//act: integer;
//acto: integer;

//value: integer;

//event: integer;

//val: smallint;
//winakt: smallint;

//mval: array [0..1] of smallint;
//redrawq: array [0..Pred(10)] of smallint;
//redrawcount: smallint;

//mousemove: array [0..1] of smallint;
//instr: pchar;
//str: pchar;
//astr: pchar;
//title: pchar;

//spoin: array [0..Pred(100)] of pchar;
//columns: integer;

//rows: integer;

//x1: integer;
//y1: integer; (* if(TRUE) { *)
//(*  return pupmenu(but->str); *)
//(* } *)
//butname: array [0..119] of char;
//tstr: pchar;
//min: single;
//max: single;
//temp: integer;
//oldpoin: pinteger;
//oldstr: pinteger;
//begin

//  acto:=-1;
//  value:=0;
//  event:=0;

//  winakt:=0;

//  redrawcount:=0;

//  title:=0;
//  char{!!!e unknown token}{*}spp^;
//  columns:=1;
//  rows:=0;

//  instr:= but.str;
//  str:= mallocN(lstrlen(instr)+1,'pupmenu');
//  memcpy(str,instr,lstrlen(instr)+1);
//  (* kopie string maken *)
//  astr:= str;
//  spoin[0]:= astr;
//  items:= 0;
//  retval[0]:= 0; (* eerst string uitelkaar pulken, tellen hoeveel elementen, return values *)
//  while {*}astr^
//  do
//  begin
//    case {*}astr^ of
//      '%':
//      begin
//        if astr[1]='x' then
//        begin
//          retval[items]:= atoi(astr+2); {*}astr^:=0;
//        end;
//        else
//        if astr[1]='t' then
//        begin
//          title:= spoin[0]; {*}astr^:=0;
//        end;
//      end;
//      '|':
//      begin
//        if astr[1]<>0 then
//        begin
//          inc(items);
//          spoin[items]:= astr+1;
//          if title<>0 then
//          retval[items]:= items-1;
//          else
//          retval[items]:= items;
//        end;
//        {*}astr^:=0;
//      end;

//    end;{case?}
//    inc(astr);
//  end;
//  inc(items); (* het begon bij 0 te tellen *)
//  if title<>0 then
//  begin
//    dec(items);
//    spp:= @spoin[1];
//    retp:= @retval[1];
//  end;
//  else
//  begin
//    spp:= @spoin[0];
//    retp:= @retval[0];
//  end;
//  {$if 0}
//  if items>15 then
//  begin
//    rows:= items div 4;
//    if items mod ( {integer(}items div 3)<=items mod rows then
//    begin
//      rows:= items div 3;
//      if items mod ( {integer(}items div 2)<=items mod rows then
//      begin
//        rows:= items div 2;
//      end;
//    end;
//  end;
//  else
//  begin
//    rows:= items;
//  end;
//  if rows<1 then
//  rows:= 1;
//  if rows)and(items then
//  begin
//    columns:= items div rows;
//    columns:= columns + (items mod rows {was ?}if then 1 {was :}else 0);
//  end;
//  {$else}
//  columns:= (items+20) div 20;
//  if columns<1 then
//  columns:= 1;
//  rows:=  {integer(}items div columns;
//  if rows<1 then
//  rows:= 1;
//  while rows*columns<items
//  do
//  inc(rows);
//  {$endif}
//  width:= 0; (* afmetingen en plaats *)
//  for{while} a:=0 to Pred(items) { a++}
//  do
//  begin
//    xmax:= fmgetstrwidth(G.font,spp[a]);
//    if xmax>width then
//    width:= xmax;
//  end;
//  if width<50 then
//  width:= 50;
//  width:= width + (10);
//  height:= rows*TBOXH;
//  if title<>0 then
//  height:= height + (TBOXH);
//  xmax:= G.curscreen.sizex;
//  ymax:= G.curscreen.sizey;
//  getmouseco_sc(mval);
//  GetButVal(but, and fvalue,@value);
//  for{while} a:=0 to Pred(items) { a++}
//  do
//  begin
//    if retp[a]=value then
//    break; {<= !!!b possible in "switch" - then remove this line}
//  end;
//  if a=items then
//  a:= 0;
//  startx:= mval[0]-width div 2-( {integer(}a div rows)*width;
//  starty:= mval[1]-height+TBOXH div 2+(a mod rows)*TBOXH;
//  if title<>0 then
//  starty:= starty + (TBOXH);
//  mousemove[0]:= mousemove[1]:=0;
//  if startx<10 then
//  begin
//    mousemove[0]:= 10-startx;
//    startx:= 10;
//  end;
//  if starty<10 then
//  begin
//    mousemove[1]:= 10-starty;
//    starty:= 10;
//  end;
//endx:= startx+width*columns;
//endy:= starty+height;
//if endx>xmax
//then
//begin
//  mousemove[0]:= xmax-endx-10;
//endx:= xmax-10;
//startx:= endx-width*columns;
//end;
//if endy>ymax
//then
//begin
//  mousemove[1]:= ymax-endy-10;
//endy:= ymax-10;
//starty:= endy-height;
//end;
//warp_pointer(mval[0]+mousemove[0],mval[1]+mousemove[1]);
//mousemove[0]:= mval[0];
//mousemove[1]:= mval[1];
//bgnpupdraw(startx,starty,endx,endy);
//butmenu_draw(startx,starty,width,TBOXH,items,title,spp,rows,columns);
//value:= 0;
//while value=0
//do
//begin
//  if acto=-1 then
//  ;
//  else
//  event:= extern_qread(@val);
//  if event<>0 then
//  begin
//    case event of
//      LEFTMOUSE,
//      RIGHTMOUSE,
//      RETKEY,
//      PADENTER:
//      begin
//        if val=0 then
//        begin
//          value:= 2;
//        end;
//      end;
//      INPUTCHANGE:
//      (* case MIDDLEMOUSE: *)
//      (*  if(val) value= 3; *)
//      (*  break; *)
//      begin
//        winakt:= val;
//      end;
//      ESCKEY:
//      begin
//        value:= 1;
//      end;
//      REDRAW:
//      begin
//        if val)and(redrawcount<10 then
//        begin
//          redrawq[redrawcount]:= val;
//          inc(redrawcount);
//        end;
//      end;
//    end;{case?}
//  end;
//  else
//  usleep(2);
//  getmouseco_sc(mval);
//  if mval[0]<startx-20)or(mval[0]>endx+20)or(mval[1]<starty-30)or(mval[1]>endy+40 then
//  value:= 1;
//  if mval[0]<startx-20)or(mval[0]>endx+20)or(mval[1]<starty-30)or(mval[1]>endy+40 then
//  value:= 1;
//  a:= (mval[0]-startx) div width;
//  CLAMP(a,0,columns-1);
//  act:= rows*a;
//  a:= (mval[1]-starty) div TBOXH;
//  CLAMP(a,0,rows-1);
//  act:= act + ((rows-1)-a);
//  if act<>acto then
//  begin
//    if acto>=0)and(acto<items then
//    begin
//      (* vergelijken, acto is init op -1 *)
//      x1:= startx+width*( {integer(}acto div rows);
//      y1:= starty-TBOXH*(acto mod rows)+(rows-1)*TBOXH;
//      tbox_embossbox(x1,y1,x1+width,y1+TBOXH,2);
//      glIndexi(TBOXBLACK);
//      glRasterPos2i(x1+5,y1+tbfontyofs);
//      fmprstr(spp[acto]);
//    end;
//    if act>=0)and(act<items then
//    begin
//      x1:= startx+width*( {integer(}act div rows);
//      y1:= starty-TBOXH*(act mod rows)+(rows-1)*TBOXH;
//      tbox_embossbox(x1,y1,x1+width,y1+TBOXH,3);
//      glIndexi(TBOXBLACK);
//      glRasterPos2i(x1+5,y1+tbfontyofs);
//      fmprstr(spp[act]);
//    end;
//    acto:= act;
//  end;
//end;
//endpupdraw();
//freeN(str);
//if winakt<>0
//then
//qenter(INPUTCHANGE,winakt);
//for{while} val:=0 to Pred(redrawcount) { val++}
//do
//qenter(REDRAW,redrawq[val]);
//warp_pointer(mousemove[0],mousemove[1]);
//if value=2)and(act>=0)and(act<items
//then
//begin
//  value:= retp[act];
//  fvalue:=  {float(}value;
//  SetButVal(but,fvalue,value);
//end;
//else
//if value=3
//then
//begin
//  (* maak er een tekstbut van *)

//  oldpoin:= but.poin;
//  oldstr:= but.str;
//  GetButVal(but, and fvalue,@value);
//  getname_menu_but(butname,but.str,value);
//  but.poin:= butname;
//  but.str:= ;
//  min:= but.min;
//  max:= but.max;
//  but.min:= 0.0;
//  but.max:= 79.0;
//  temp:= but.type;
//  but.type:= TEX;
//  do_textbut(but);
//  but.poin:= oldpoin;
//  but.str:= oldstr;
//  but.min:= min;
//  but.max:= max;
//  but.type:= temp;
//  (* test butname en verander zonodig poin *)
//end;
//else
//begin
//  result:= 0;
//  exit;
//end;
//begin
//  result:= 1;
//  exit;
//end;
//end;
//(* *********** END MENUBUT ********************** *)

//procedure SetButShape(nr: smallint);
//begin
//  BGdrawtype:= nr;
//  if nr=1 then
//  begin
//    (* overdraw cmap *)
//    ButDrawFunc:= EmbossBut1;
//    SliderDrawFunc:= EmbossSlider1;
//  end;
//  else
//  if nr=2 then
//  begin
//    (* alleen RGB *)
//    ButDrawFunc:= EmbossBut2;
//    SliderDrawFunc:= EmbossSlider1;
//  end;
//  else
//  if nr=3 then
//  begin
//    (* alleen RGB, voor ipo *)
//    ButDrawFunc:= TextLabelBut;
//    SliderDrawFunc:= EmbossSlider1;
//  end;
//  else
//  if nr=4 then
//  begin
//    (* alleen RGB, speciale tekstbut *)
//    ButDrawFunc:= EmbossBut2a;
//    SliderDrawFunc:= EmbossSlider1;
//  end;
//end;

//procedure DrawBut2(but: pBut;  sel: smallint);
//var
//id: pID;
//f: single;
//value: single;
//fp: pfloat;
//tempcol: uint;
//x1: integer;
//y1: integer;
//x2: integer;
//y2: integer;
//s: integer;
//lvalue: integer;
//olddraw: integer;
//iconr: integer;
//icox: integer;
//icoy: integer;
//sp: psmallint;
//ok: smallint;
//a: smallint;
//h: smallint;
//w: smallint;
//b3: smallint;
//b4: smallint;
//colr: smallint;
//colg: smallint;
//colb: smallint;
//r: char;
//g: char;
//b: char;
//s1: array [0..119] of char;
//butname: array [0..119] of char;
//cp: pchar;
//begin

//  if but=0 then
//  exit;
//  myfmsetfont(but.font);
//  if but.drawtype<>BGdrawtype then
//  SetButShape(but.drawtype);
//  case but.type and BUTTYPE of
//    BUT,
//    ROW,
//    TOG,
//    TOGN,
//    BUTRET:
//    begin
//      ButDrawFunc(but.str,but.x1,but.y1,but.x2,but.y2,sel,but.col);
//    end;
//    ICONTOG:
//    (* tijdelijk icoon ophogen als sel *)
//    begin
//      if sel<>0 then
//      BGaddicon:= 1;
//      ButDrawFunc(but.str,but.x1,but.y1,but.x2,but.y2,sel,but.col);
//    end;
//    ICONROW:
//    (* tijdelijk veranderen van icoon, daarna weer terug! *)
//    begin
//      GetButVal(but,@value,@lvalue);
//      BGaddicon:= lvalue- {integer(}(but.min);
//      ButDrawFunc(but.str,but.x1,but.y1,but.x2,but.y2,sel,but.col);
//      a:= (but.y1+but.y2) div 2;
//      cpack(0);
//      (* teken pijltjes, icon is standaard RGB *)
//      sdrawline(but.x1-1,a-2,but.x1-1,a+2);
//      sdrawline(but.x1-2,a-1,but.x1-2,a+1);
//      sdrawline(but.x1-3,a,but.x1-3,a);
//      cpack($FFFFFF);
//      sdrawline(but.x1-3,a-1,but.x1-1,a-3);
//      cpack(0);
//      sdrawline(but.x2+1,a-2,but.x2+1,a+2);
//      sdrawline(but.x2+2,a-1,but.x2+2,a+1);
//      sdrawline(but.x2+3,a,but.x2+3,a);
//      cpack($FFFFFF);
//      sdrawline(but.x2+3,a-1,but.x2+1,a-3);
//    end;
//    MENU:
//    begin
//      if but.x2-but.x1>30 then
//      begin
//        GetButVal(but,@value,@lvalue);
//        getname_menu_but(butname,but.str,lvalue);
//      end;
//      else
//      begin
//        butname[0]:= 0;
//        but.rt[0]:= 0;
//      end;
//      ButDrawFunc(butname,but.x1,but.y1,but.x2,but.y2,sel,but.col);
//      (* als er ruimte is: teken symbooltje *)
//      if but.rt[0]+10<but.x2-but.x1 then
//      begin
//        h:= but.y2-but.y1;
//        x1:= but.x2-0.66*h;
//        x2:= x1+.33*h;
//        y1:= but.y1+.42*h;
//        y2:= y1+.16*h;
//        cpack(BGbutcol[but.col].pen_desel);
//        glRecti(x1,y1,x2,y2);
//        cpack(BGbutcol[but.col].pen_sel);
//        glRecti(x1-1,y1+1,x2-1,y2+1);
//      end;
//    end;
//    NUM:
//    begin
//      a:= GetButVal(but,@value,@lvalue);
//      s1[0]:= 0;
//      if a=1 then
//      begin
//        sprintf(s1,'%s%d',but.str,lvalue);
//      end;
//      else
//      if a=2 then
//      begin
//        if but.max<10.$1 then
//        sprintf(s1,'%s%.3f',but.str,value);
//        else
//        sprintf(s1,'%s%.2f',but.str,value);
//      end;
//      ButDrawFunc(s1,but.x1,but.y1,but.x2,but.y2,sel,but.col);
//    end;
//    NUMSLI,
//    HSVSLI:
//    begin
//      Btextleft:= 1;
//      a:= GetButVal(but,@value,@lvalue);
//      s1[0]:= 0;
//      if a=1 then
//      begin
//        sprintf(s1,'%s%d',but.str,lvalue);
//      end;
//      else
//      if a=2 then
//      begin
//        if but.max<10.$1 then
//        sprintf(s1,'%s%.3f',but.str,value);
//        else
//        sprintf(s1,'%s%.2f',but.str,value);
//      end;
//      ButDrawFunc(s1,but.x1,but.y1,but.x2,but.y2,sel=1,but.col);
//      Btextleft:= 0;
//      Bnotext:= 1;
//      x1:= but.x1;
//      x2:= but.x2;
//      y1:= but.y1;
//      y2:= but.y2;
//      but.x1:= (but.x1+but.x2) div 2;
//      but.x2:= but.x2 - (9);
//      but.y1:= -4+(but.y1+but.y2) div 2;
//      but.y2:= but.y1+6;
//      s1[0]:= 0;
//      f:= (value-but.min)*(but.x2-but.x1-but.y2+but.y1) div (but.max-but.min);
//      SliderDrawFunc(s1,but.str,f,but.x1,but.y1,but.x2,but.y2,sel=2,but.col,but.rt);
//      (* de slider *)
//      Bnotext:= 0;
//      but.x1:= x1;
//      but.x2:= x2;
//      but.y1:= y1;
//      but.y2:= y2; (* kleurschuif *)
//      if but.a1)and(but.a1<>but.nr then
//      begin
//        (* colornummer, pas op oneindige loop *)
//        if BGbutcol[BGcol].rgb<>0 then
//        begin
//          SetButsWin(but.win,but.a1,but.a1);
//        end;
//      end;
//    end;
//    TOG3:
//    begin
//      tempcol:= BGbutcol[but.col].pen_sel;
//      if sel<>0 then
//      begin
//        ok:= 0;
//        if (but.type and BUTPOIN)=CHA then
//        begin
//          if BTST(*(but.poin+2),(but.type and 31)) then
//          ok:= 1;
//        end;
//        else
//        if (but.type and BUTPOIN)=SHO then
//        begin
//          sp:=  {psmallint(}but.poin;
//          if BTST(sp[1],(but.type and 31)) then
//          ok:= 1;
//        end;
//        if ok<>0 then
//        BGbutcol[but.col].pen_sel:= $FFFF;
//      end;
//      ButDrawFunc(but.str,but.x1,but.y1,but.x2,but.y2,sel,but.col);
//      BGbutcol[but.col].pen_sel:= tempcol;
//    end;
//    LABEL:
//    (* eerst clear *)
//    begin
//      if BGbutcol[but.col].drmode=OVERDRAW then
//      begin
//        if but.max=1.0 then
//        begin
//          glIndexi(BGbutcol[but.col].back);
//          glRects(but.x1-1,but.y1-1,but.x2+1,but.y2+1);
//        end;
//        if but.min=1.0 then
//        glIndexi(BGbutcol[but.col].pen_sel);
//        else
//        glIndexi(BGbutcol[but.col].pen_desel);
//      end;
//      else
//      begin
//        if but.max=1.0 then
//        begin
//          cpack(BGbutcol[but.col].back);
//          glRects(but.x1-1,but.y1-1,but.x2+1,but.y2+1);
//        end;
//        if but.min=1.0 then
//        cpack(BGbutcol[but.col].pen_sel);
//        else
//        cpack(BGbutcol[but.col].pen_desel);
//      end;
//      s:= Bxasp*fmgetstrwidth(BGfont,but.str);
//      x1:= (but.x1+but.x2-s) div 2;
//      y1:= (but.y1+but.y2-12 div 2-1) div 2;
//      glRasterPos2i(x1,y1);
//      fmprstr(but.str);
//    end;
//    SLI:
//    begin
//      a:= GetButVal(but,@value,@lvalue);
//      f:= (value-but.min)*(but.x2-but.x1-but.y2+but.y1) div (but.max-but.min);
//      s1[0]:= 0;
//      if a=1 then
//      begin
//        sprintf(s1,'%d',lvalue);
//      end;
//      else
//      if a=2 then
//      begin
//        sprintf(s1,'%.2f',value);
//      end;
//      SliderDrawFunc(s1,but.str,f,but.x1,but.y1,but.x2,but.y2,sel,but.col,but.rt);
//    end;
//    TEX:
//    begin
//      a:= -1;
//      tekentekstbut(but,but.poin, and a);
//    end;
//    IDPOIN:
//    begin
//      id:= *((ID** )but.poin);
//      strcpy(butname,but.str);
//      if id<>0 then
//      strcat(butname,id.name+2);
//      ButDrawFunc(butname,but.x1,but.y1,but.x2,but.y2,sel,but.col);
//    end;
//    COL:
//    begin
//      ButDrawFunc(but.str,but.x1,but.y1,but.x2,but.y2,2,but.col);
//      if (but.type and BUTPOIN)=FLO then
//      begin
//        fp:=  {pfloat(}but.poin;
//        colr:= ffloor(255.0*fp[0]+0.5);
//        colg:= ffloor(255.0*fp[1]+0.5);
//        colb:= ffloor(255.0*fp[2]+0.5);
//      end;
//      else
//      begin
//        cp:=  {pchar(}but.poin;
//        colr:= cp[0];
//        colg:= cp[1];
//        colb:= cp[2];
//      end;
//      glColor3ub(colr,colg,colb);
//      glRects(but.x1+2,but.y1+2,but.x2-2,but.y2-2);
//    end;

//  end;{case?}
//  BGaddicon:= 0;
//end;

//procedure DrawBut(but: pBut;  sel: smallint);
//begin
//  if Bfrontbuf<>0 then
//  begin
//    glDrawBuffer(GL_FRONT);
//    DrawBut2(but,sel);
//    glDrawBuffer(GL_BACK);
//  end;
//  DrawBut2(but,sel);
//end;
//type
//ButBlock = record
//end;
//But = record
//end;

//procedure SetButs(min: smallint;  max: smallint);
//var
//oldwin: integer;
//actwin: integer;
//push: smallint;
//nr: smallint;
//begin

//  bb:= BGfirstbutblock;
//  but:= 0;
//  oldwin:= actwin:=winget();
//  while bb
//  do
//  begin
//    but:= bb.first;
//    for{while} nr:=0 to Pred(bb.aantal) { nr++}
//    do
//    begin
//      if but.nr>=min)and(but.nr<=max then
//      begin
//        if bb.window<>actwin then
//        begin
//          if mywinexist(bb.window) then
//          begin
//            actwin:= bb.window;
//            ButtonsGetmouse(0);
//            winset(actwin);
//          end;
//          else
//          begin
//            nr:= bb.aantal;
//            begin
//              {!!!7 possible troubles with "for" =>}
//              continue
//            end;
//          end;
//        end;
//        push:= IsButSel(but);
//        DrawBut(but,push);
//      end;
//      inc(but);
//    end;
//    bb:= bb.next;
//  end;
//  if oldwin<>actwin then
//  winset(oldwin);
//end;
//type
//ButBlock = record
//end;
//But = record
//end;

//procedure SetButsWin(win: smallint;  min: smallint;  max: smallint);
//var
//oldwin: integer;
//actwin: integer;
//push: smallint;
//nr: smallint;
//begin

//  bb:= BGfirstbutblock;
//  but:= 0;
//  oldwin:= winget();
//  actwin:= win;
//  if oldwin<>actwin then
//  winset(actwin);
//  while bb
//  do
//  begin
//    if bb.window=win then
//    begin
//      but:= bb.first;
//      while{for}
//      nr:= 0;
//      {to} nr<bb.aantal
//      {;}
//      inc(nr);
//      inc(but);
//      do
//      begin
//        if but.nr>=min)and(but.nr<=max then
//        begin
//          push:= IsButSel(but);
//          DrawBut(but,push);
//        end;
//      end;
//    end;
//    bb:= bb.next;
//  end;
//  if oldwin<>actwin then
//  winset(oldwin);
//end;
//type
//ButBlock = record
//end;

//procedure FreeButBlock(str: pchar);
//begin

//  bb:= BGfirstbutblock;
//  while bb
//  do
//  begin
//    if lstrcmp(str,bb.naam)=0 then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    bb:= bb.next;
//  end;
//  if bb=0 then
//  exit;
//  nextb:= bb.next;
//  b1:= BGfirstbutblock;
//  if bb=b1 then
//  BGfirstbutblock:= nextb;
//  else
//  begin
//    while b1
//    do
//    begin
//      if b1.next=bb then
//      begin
//        b1.next:= nextb;
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      b1:= b1.next;
//    end;
//  end;
//  FreeButBlockData(bb);
//  freeN(bb);
//end;
//type
//ButBlock = record
//end;
//var {was static}
//firsttime: smallint;

//procedure DefButBlock(str: pchar;  win: integer;  font: pinteger;  aantal: smallint;  col: smallint;  drawtype: smallint);
//begin

//  firsttime:=1;
//  if firsttime<>0 then
//  begin
//    firsttime:= 0;
//    BGfirstbutblock:= 0;
//  end;
//  winset(win);
//  myfmsetfont(font);
//  SetButShape(drawtype);
//  FreeButBlock(str);
//  bb:= callocN(sizeof(structButBlock),'DefButBlock');
//  if bb=0 then
//  begin
//    printf('Calloc error in DefButBlock\n');
//    BGfirst:= 0;
//    exit;
//  end;
//  bb.next:= BGfirstbutblock;
//  BGfirstbutblock:= bb;
//  bb.first:= callocN(aantal*sizeof(structBut),'DefButBlock2');
//  bb.aantal:= aantal;
//  bb.window:= win;
//  strncpy(bb.naam,str,19);
//  bb.naam[19]:= 0;
//  BGfirst:= bb.first;
//  BGaantal:= bb.aantal;
//  BGteller:= 0;
//  BGwin:= win;
//  BGfont:= font;
//  BGcol:= col;
//  BGdrawtype:= drawtype;
//  BGlock:= 0;
//  BGlockstr:= 0{nil};
//  ButtonsGetmouse(0);
//  (* globals *)
//  (* voor zekerheid: als winmat veranderd is, tekenen ikonen goed *)
//end;
//type
//But = record
//end;

//procedure SetButFunc(

//procedure func;
//);
//begin

//  b:= BGfirst+BGteller;
//  b.func:= func;
//end;
//type
//But = record
//end;
//var
//value: single;
//maxl: integer;
//lvalue: integer;
//push: smallint;
//a: smallint;
//s: array [0..119] of char;
//butname: array [0..119] of char;
//if type and BUTPOIN
//then
//begin
//  (* er is pointer nodig *)
//  if poin=0 then
//  begin
//    (* als pointer nul is wordt button gewist en niet gedefinieerd *)
//    cpack(BGbutcol[BGcol].back);
//    glRects(x1,y1,x1+x2,y1+y2);
//    begin
//      result:= 0;
//      exit;
//    end;
//  end;
//end;
//if BGteller>=BGaantal-1
//then
//begin
//  type
//  But = record
//end;
//But = record
//end;
//var
//a: integer;
//tmp:= BGfirst;
//BGfirst:= callocN(sizeof(structBut)*2*BGaantal,'new butblock');
//memcpy(BGfirst,tmp,sizeof(structBut)*BGaantal);
//obut:= tmp;
//nbut:= BGfirst;
//while{for}
//a:= 0;
//{to} a<BGaantal
//{;}
//inc(a);
//inc(obut);
//inc(nbut);
//do
//begin
//  if obut.str=obut.strdata then
//  nbut.str:= nbut.strdata;
//end;
//BGaantal:= BGaantal * (2);
//BGfirstbutblock.aantal:= BGaantal;
//BGfirstbutblock.first:= BGfirst;
//freeN(tmp);
//end;
//b:= BGfirst+BGteller;
//inc(BGteller);
//b.type:= type;
//b.nr:= nr; (* ga er van uit dat de font en window goed staan *)
//if lstrlen(str)>=MAXBUTSTR-1
//then
//begin
//  b.str:= callocN(lstrlen(str)+2,'DefBut');
//  strcpy(b.str,str);
//end;
//else
//begin
//  b.str:= b.strdata;
//  strcpy(b.str,str);
//end;
//b.x1:= x1;
//b.y1:= y1;
//b.x2:= (x1+x2);
//b.y2:= (y1+y2);
//b.poin:= poin;
//b.min:= min;
//b.max:= max;
//b.a1:= a1;
//b.a2:= a2;
//b.font:= BGfont;
//b.lock:= BGlock;
//b.lockstr:= BGlockstr;
//b.col:= BGcol;
//b.win:= BGwin;
//b.drawtype:= BGdrawtype; (* pixel-breedte naam: *)
//if (type and BUTTYPE)=MENU
//then
//begin
//  GetButVal(b,@value,@lvalue);
//  getname_menu_but(butname,b.str,lvalue);
//  if butname[0]<>0 then
//  b.rt[0]:= Bxasp*fmgetstrwidth(BGfont,butname);
//  else
//  b.rt[0]:= 0;
//end;
//else
//begin
//  if str[0]<>0 then
//  b.rt[0]:= Bxasp*fmgetstrwidth(BGfont,str);
//  else
//  b.rt[0]:= 0;
//end;
//(* automatische breedte *)
//if x2=0
//then
//begin
//  x2:= b.rt[0]+6;
//  b.x2:= (x1+x2);
//end;
//if (type and BUTTYPE)=NUM
//then
//begin
//  (* spatie toevoegen achter naam *)
//  a:= lstrlen(b.str);
//  if a>0)and(a<MAXBUTSTR-2 then
//  begin
//    if b.str[a-1]<>' ' then
//    begin
//      b.str[a]:= ' ';
//      b.str[a+1]:= 0;
//    end;
//  end;
//end;
//(* testen op min en max *)
//case type and BUTTYPE of
//  NUM,
//  SLI,
//  NUMSLI,
//  HSVSLI:
//  begin
//    GetButVal(b,@value,@lvalue);
//    if value<min then
//    value:= min;
//    if value>max then
//    value:= max;
//    SetButVal(b,value,lvalue);
//  end;

//end;{case?}
//push:= IsButSel(b);
//DrawBut(b,push);
//begin
//  result:= b;
//  exit;
//end;
//end;{case?}
//type
//But = record
//end;
//but:= DefBut(type,nr,str,x1,y1,x2,y2,poin,min,max,a1,a2);
//if but<>0
//then
//begin
//  but.tip:= tip;
//end;
//begin
//  result:= but;
//  exit;
//end;
//end;{case?}
//But = record
//end;

//procedure SetupFile(bb: pButBlock);
//var
//totbut: smallint;
//nr: smallint;
//fp: pFILE;
//begin

//  fp:= fopen('butsetup','w');
//  if fp=0{nil} then
//  ;
//  else
//  begin
//    b:= bb.first;
//    totbut:= bb.aantal;
//    for{while} nr:=0 to Pred(totbut) {nr++}
//    do
//    begin
//      fprintf(fp,'%d,%d,%d,%d   %s\n',b.x1,b.y1,b.x2-b.x1,b.y2-b.y1,b.str);
//      inc(b);
//    end;
//    fclose(fp);
//  end;
//end;

//procedure EditBut(but: pBut);
//var
//mval: array [0..1] of smallint;
//mx: smallint;
//my: smallint;
//dx: smallint;
//dy: smallint;
//w: smallint;
//h: smallint;
//s: smallint;
//push: smallint;
//x: integer;
//y: integer;
//maxx: integer;
//maxy: integer;
//begin

//  getmouseco_sc(mval);
//  getorigin(@x,@y);
//  getsize(@maxx,@maxy);
//  mval[0]:= mval[0] - (x);
//  mval[1]:= mval[1] - (y);
//  mx:= mval[0];
//  my:= mval[1];
//  while get_mbut()@L_MOUSE
//  do
//  begin
//    getmouseco_sc(mval);
//    mval[0]:= mval[0] - (x);
//    mval[1]:= mval[1] - (y);
//    dx:= mval[0]-mx;
//    dy:= mval[1]-my;
//    if get_qual()@LR_SHIFTKEY then
//    begin
//      dx:= dx div (4);
//      dy:= dy div (4);
//    end;
//    if dx<>0)or(dy<>0 then
//    begin
//      if mval[0]<maxx)and(mval[1]<maxy then
//      begin
//        mx:= mval[0];
//        my:= mval[1];
//        cpack(BGbutcol[but.col].back);
//        if Bfrontbuf<>0 then
//        begin
//          glDrawBuffer(GL_FRONT);
//        end;
//        if (but.type and BUTTYPE)=SLI then
//        begin
//          h:= 12;
//          s:= Bxasp*fmgetstrwidth(BGfont,but.str);
//          glRectf(but.x2+6,but.y1-4,but.x2+9+s,but.y1+h);
//          s:= but.rt[0];
//          glRectf(but.x1-12-s,but.y1-4,but.x1-7,but.y1+h);
//        end;
//        glRects(but.x1-2,but.y1-2,but.x2+2,but.y2+2);
//        if {not}0=(get_qual()@LR_ALTKEY) then
//        begin
//          but.x1:= but.x1 + (dx);
//          but.y1:= but.y1 + (dy);
//        end;
//        but.x2:= but.x2 + (dx);
//        but.y2:= but.y2 + (dy);
//        push:= IsButSel(but);
//        DrawBut(but,push);
//        if Bfrontbuf<>0 then
//        begin
//          glDrawBuffer(GL_BACK);
//        end;
//      end;
//    end;
//  end;
//end;

//procedure do_textbut(b: pBut);
//var
//dev: word;
//val: smallint;
//temp: smallint;
//x: smallint;
//mval: array [0..1] of smallint;
//c: smallint;
//len: smallint;

//pos: smallint;

//ofs: smallint;
//qual: smallint;
//s: array [0..Pred(150)] of char;
//begin

//  len:=0;
//  pos:=0;

//  ButtonsGetmouse(mval);
//  DrawBut(b,1);
//  (* doen: eerst pos berekenen ahv muisco *)
//  strcpy(s,b.poin);
//  (* ivm met globals zoals tekenmode *)
//  pos:= 150;
//  ofs:= tekentekstbut(b,s,@pos);
//  while (Bxasp*fmgetstrwidth(BGfont,s+ofs)+b.x1+4)>mval[0]
//  do
//  begin
//    if pos<=ofs then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    dec(pos);
//    s[pos]:= 0;
//  end;
//  if Bfrontbuf<>0 then
//  glDrawBuffer(GL_FRONT);
//  strcpy(s,b.poin);
//  tekentekstbut(b,s,@pos);
//  while get_mbut() and L_MOUSE
//  do
//  usleep(1);
//  len:= lstrlen(s);
//  b.min:= 0.0;
//  {$ifdef IRISGL}
//  qdevice(KEYBD);
//  {$endif}
//  while LongBool(1)
//  do
//  begin
//    dev:= extern_qread(@val);
//    if dev=INPUTCHANGE then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    else
//    if get_mbut() and L_MOUSE then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    else
//    if get_mbut() and R_MOUSE then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    else
//    if dev=ESCKEY then
//    break; {<= !!!b possible in "switch" - then remove this line}
//    else
//    if dev=MOUSEX then
//    val:= 0;
//    else
//    if dev=MOUSEY then
//    val:= 0;
//    else
//    if dev=0 then
//    usleep(1);
//    if dev=KEYBD)and(val then
//    begin
//      c:= val;
//      case c of
//        0:
//        begin
//        end;
//        '\b',
//        'b'+100:
//        (* backspace *)
//        begin
//          if len<>0 then
//          begin
//            if get_qual()@LR_SHIFTKEY then
//            begin
//              s[0]:= 0;
//              pos:= 0;
//              len:= 0;
//            end;
//            else
//            begin
//              temp:= pos-lstrlen(b.str);
//              if temp>0 then
//              begin
//                for{while} x:=temp to lstrlen(s) {x++}
//                do
//                s[x-1]:= s[x];
//                dec(pos);
//                s[dec(len); {--}len]:= #0;
//              end;
//            end;
//          end;
//        end;
//        '\n',
//        '\r':
//        (* niet doen: rawkey afhandelen *)
//        begin
//        end;
//        else
//        begin
//          if c>31)and(c<127 then
//          begin
//            if len<b.max then
//            begin
//              temp:= pos-lstrlen(b.str);
//              for{while} x:=b.max downto Succ(temp) { x--}
//              do
//              s[x]:= s[x-1];
//              s[temp]:= c;
//              inc(pos);
//              inc(len);
//              s[len]:= #0;
//            end;
//          end;
//        end;
//      end;{case?}
//    end;
//    else
//    if val<>0 then
//    begin
//      if dev=RIGHTARROWKEY then
//      begin
//        if G.qual and LR_SHIFTKEY then
//        pos:= 150;
//        else
//        inc(pos);
//      end;
//      else
//      if dev=LEFTARROWKEY then
//      begin
//        if G.qual and LR_SHIFTKEY then
//        pos:= 0;
//        else
//        if pos>0 then
//        dec(pos);
//      end;
//      else
//      if dev=PADENTER)or(dev=RETKEY then
//      begin
//        b.min:= 1.0;
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//    end;
//    if val)and(myqtest()=0 then
//    begin
//      tekentekstbut(b,s,@pos);
//    end;
//  end;
//  {$ifdef IRISGL}
//  unqdevice(KEYBD);
//  {$endif}
//  if dev<>ESCKEY then
//  strcpy(b.poin,s);
//  pos:= -1;
//  if b.func<>0 then
//  b.func(b.poin);
//  if Bfrontbuf<>0 then
//  begin
//    tekentekstbut(b,b.poin,@pos);
//  end;
//  glDrawBuffer(GL_BACK);
//  tekentekstbut(b,b.poin,@pos);
//end;

//procedure act_as_textbut(b: pBut);
//var
//min: single;
//max: single;
//value: single;
//a: integer;
//lvalue: integer;
//temp: integer;
//s: array [0..119] of char;
//point: pchar;
//begin

//  GetButVal(b,@value,@lvalue);
//  if (b.type and BUTPOIN)=FLO then
//  begin
//    sprintf(s,'%.4f',value);
//  end;
//  else
//  begin
//    sprintf(s,'%d',lvalue);
//  end;
//  point:= b.poin;
//  b.poin:= s;
//  min:= b.min;
//  max:= b.max;
//  b.min:= 0.0;
//  b.max:= 15.0;
//  temp:= b.type;
//  b.type:= TEX;
//  do_textbut(b);
//  b.type:= temp;
//  b.poin:= point;
//  if (b.type and BUTPOIN)=FLO then
//  begin
//    value:= atof(s);
//    if value<min then
//    value:= min;
//    if value>max then
//    value:= max;
//    lvalue:= value;
//  end;
//  else
//  begin
//    lvalue:= atoi(s); ;
//    if lvalue<min then
//    lvalue:= min;
//    if lvalue>max then
//    lvalue:= max;
//    value:= lvalue;
//  end;
//  SetButVal(b,value,lvalue);
//  b.min:= min;
//  b.max:= max;
//end;

//procedure do_sliderbut(but: pBut);
//var
//f: single;
//fstart: single;
//tempf: single;
//deler: single;
//value: single;
//a: integer;
//sx: integer;
//h: integer;
//temp: integer;
//pos: integer;

//lvalue: integer;
//mval: array [0..1] of smallint;
//qual: smallint;
//begin

//  pos:=0;

//  a:= GetButVal(but,@value,@lvalue);
//  ButtonsGetmouse(mval);
//  sx:= mval[0];
//  h:= but.y2-but.y1;
//  fstart:= but.max-but.min;
//  fstart:= (value-but.min) div fstart;
//  temp:= 32767;
//  DrawBut(but,2);
//  if (but.type and BUTTYPE)=NUMSLI then
//  deler:= ((but.x2-but.x1) div 2-h);
//  else
//  if (but.type and BUTTYPE)=HSVSLI then
//  deler:= ((but.x2-but.x1) div 2-h);
//  else
//  deler:= (but.x2-but.x1-h);
//  while get_mbut() and L_MOUSE
//  do
//  begin
//    qual:= get_qual();
//    ButtonsGetmouse(mval);
//    f:=  {float(}(mval[0]-sx) div deler+fstart;
//    if qual and LR_CTRLKEY then
//    begin
//      if qual and LR_SHIFTKEY then
//      f:= ffloor(f*100.0) div 100.0;
//      else
//      f:= ffloor(f*10.0) div 10.0;
//    end;
//    else
//    if qual and LR_SHIFTKEY then
//    begin
//      f:= (f-fstart) div 10.0+fstart;
//    end;
//    CLAMP(f,0.0,1.0);
//    tempf:= but.min+f*(but.max-but.min);
//    temp:= ffloor(tempf+.5);
//    GetButVal(but,@value,@lvalue);
//    if (a=1)and(temp<>lvalue))or((tempf<>value) then
//    begin
//      pos:= 1;
//      SetButVal(but,tempf,temp);
//      DrawBut(but,2);
//      if but.func<>0 then
//      but.func(but);
//    end;
//    else
//    usleep(1);
//  end;
//  if temp<>32767)and(pos=0 then
//  begin
//    (* plus 1 of min 1 *)
//    if (but.type and BUTTYPE)=SLI then
//    f:=  {float(}(mval[0]-but.x1) div (but.x2-but.x1-h);
//    else
//    f:=  {float(}(mval[0]-(but.x1+but.x2) div 2) div ((but.x2-but.x1) div 2-h);
//    f:= but.min+f*(but.max-but.min);
//    if a=1 then
//    begin
//      if f<temp then
//      dec(temp);
//      else
//      inc(temp);
//      if temp>=but.min)and(temp<=but.max then
//      SetButVal(but,tempf,temp);
//    end;
//    else
//    begin
//      if f<tempf then
//      tempf:= tempf - (.$1);
//      else
//      tempf:= tempf + (.$1);
//      if tempf>=but.min)and(tempf<=but.max then
//      SetButVal(but,tempf,temp);
//    end;
//  end;
//end;

//procedure donumslibut(but: pBut);
//var
//value: single;
//lvalue: integer;
//mval: array [0..1] of smallint; (* eerste bepalen of het slider is of textbut *)
//begin

//  ButtonsGetmouse(mval);
//  if mval[0]>=-6+(but.x1+but.x2) div 2 then
//  begin
//    (* slider *)
//    DrawBut(but,2);
//    do_sliderbut(but);
//  end;
//  else
//  begin
//    DrawBut(but,1);
//    act_as_textbut(but);
//  end;
//  while get_mbut()@L_MOUSE
//  do
//  usleep(1);
//  DrawBut(but,0);
//  (* hsv patch *)
//  if (but.type and BUTTYPE)=HSVSLI then
//  begin
//    if but.str[0]='H' then
//    begin
//      DrawBut(but+1,0);
//      DrawBut(but+2,0);
//    end;
//    else
//    if but.str[0]='S' then
//    begin
//      DrawBut(but+1,0);
//      DrawBut(but-1,0);
//    end;
//    else
//    if but.str[0]='V' then
//    begin
//      DrawBut(but-1,0);
//      DrawBut(but-2,0);
//    end;
//  end;
//end;
//type
//But = record
//end;
//ButBlock = record
//end;
//var {was static}
//butname: array [0..Pred(128)] of char; (* aktieve window *)

//function GetButTip: pchar;
//var
//value: single;
//nr: integer;
//temp: integer;
//winakt: integer;
//totbut: integer;
//lvalue: integer;
//mval: array [0..1] of smallint;
//begin

//  winakt:= winget();
//  if winakt=0 then
//  begin
//    result:= 0{nil};
//    exit;
//  end;
//  bb:= BGfirstbutblock;
//  while bb
//  do
//  begin
//    b:= 0;
//    while bb
//    do
//    begin
//      if bb.window=winakt then
//      begin
//        b:= bb.first;
//        totbut:= bb.aantal;
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      bb:= bb.next;
//    end;
//    if b=0 then
//    begin
//      result:= 0;
//      exit;
//      (*  geen actieve window *)
//    end;
//    ButtonsGetmouse(0);
//    ButtonsGetmouse(mval);
//    while{for}
//    nr:= 0;
//    {to} nr<totbut
//    {;}
//    inc(nr);
//    inc(b);
//    do
//    begin
//      if mval[0]>=b.x1)and(mval[0]<b.x2 then
//      begin
//        if (b.type and BUTTYPE)=SLI then
//        temp:= 4;
//        else
//        temp:= 0;
//        if mval[1]>=b.y1-temp)and(mval[1]<=b.y2+temp then
//        begin
//          if b.tip<>0 then
//          begin
//            result:= b.tip;
//            exit;
//          end;
//          else
//          begin
//            result:= ;
//            exit;
//          end;
//        end;
//      end;
//    end;
//    bb:= bb.next;
//  end;
//  begin
//    result:= 0;
//    exit;
//  end;
//end;
//type
//But = record
//end;
//ButBlock = record
//end;

//function DoButtons: smallint;
//var
//f: single;
//fstart: single;
//deler: single;
//value: single;
//tempf: single;
//min: single;
//max: single;
//x: integer;
//y: integer;
//lvalue: integer;
//winakt: integer;
//temp: integer;
//sp: psmallint;
//a: smallint;
//nr: smallint;

//mval: array [0..1] of smallint;
//push: smallint;

//w: smallint;
//totbut: smallint;
//sx: smallint;
//sy: smallint;
//h: smallint;
//c: smallint;
//len: smallint;

//pos: smallint;

//ofs: smallint;
//qual: smallint;
//retval: smallint;

//getshift: smallint;
//s: array [0..Pred(150)] of char; (* aktieve window *)
//begin

//  void({*}func^);
//  ();
//  ID{!!!e unknown token}{*}idpp^;
//  ,
//  {*}id^;

//  nr:=0;

//  push:=0;

//  len:=0;
//  pos:=0;

//  retval:=1;

//  winakt:= winget();
//  if winakt=0 then
//  begin
//    result:= 0;
//    exit;
//  end;
//  bb:= BGfirstbutblock;
//  while bb
//  do
//  begin
//    b:= 0;
//    while bb
//    do
//    begin
//      if bb.window=winakt then
//      begin
//        b:= bb.first;
//        totbut:= bb.aantal;
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      bb:= bb.next;
//    end;
//    if b=0 then
//    begin
//      result:= 0;
//      exit;
//      (*  geen actieve window *)
//    end;
//    ButtonsGetmouse(0);
//    ButtonsGetmouse(mval);
//    for{while} nr:=0 to Pred(totbut) { nr++}
//    do
//    begin
//      if mval[0]>=b.x1)and(mval[0]<b.x2 then
//      begin
//        if (b.type and BUTTYPE)=SLI then
//        temp:= 4;
//        else
//        temp:= 0;
//        if mval[1]>=b.y1-temp)and(mval[1]<=b.y2+temp then
//        begin
//          if b.lock<>0 then
//          begin
//            if b.lockstr<>0 then
//            begin
//              error(b.lockstr);
//            end;
//          end;
//          else
//          begin
//            if b.type and BUTPOIN then
//            begin
//              (* er is pointer nodig *)
//              if b.poin=0 then
//              begin
//                printf('DoButton pointer error: %s\n',b.str);
//                begin
//                  result:= 0;
//                  exit;
//                end;
//              end;
//            end;
//            {$ifndef FREE}
//            if get_qual()@LR_CTRLKEY then
//            begin
//              EditBut(b);
//              SetupFile(bb);
//              begin
//                result:= 0;
//                exit;
//              end;
//            end;
//            {$endif}
//            case b.type and BUTTYPE of
//              BUT:
//              begin
//                DrawBut(b,1);
//                push:= 1;
//                if b.func=0 then
//                begin
//                  while get_mbut()@L_MOUSE
//                  do
//                  begin
//                    ButtonsGetmouse(mval);
//                    a:= 0;
//                    if mval[0]>b.x1 then
//                    if mval[0]<b.x2 then
//                    if mval[1]>=b.y1-1 then
//                    if mval[1]<=b.y2+1 then
//                    a:= 1;
//                    if a<>push then
//                    begin
//                      push:= a;
//                      DrawBut(b,push);
//                    end;
//                    usleep(1);
//                  end;
//                end;
//                if push<>0 then
//                begin
//                  if b.func<>0 then
//                  begin
//                    if b.poin<>0 then
//                    begin
//                      GetButVal(b,@value,@lvalue);
//                      b.func(lvalue);
//                    end;
//                    else
//                    b.func();
//                    winset(winakt);
//                    while get_mbut()@L_MOUSE
//                    do
//                    usleep(1);
//                  end;
//                end;
//                DrawBut(b,0);
//                if push=0 then
//                begin
//                  result:= 0;
//                  exit;
//                end;
//              end;
//              TOG,
//              ICONTOG,
//              TOGN:
//              begin
//                GetButVal(b,@value,@lvalue);
//                if b.type and BIT then
//                begin
//                  w:= BTST(lvalue,b.type and 31);
//                  if w<>0 then
//                  lvalue:= BCLR(lvalue,b.type and 31);
//                  else
//                  lvalue:= BSET(lvalue,b.type and 31);
//                  SetButVal(b,value,lvalue);
//                  if w<>0 then
//                  push:= 0;
//                  else
//                  push:= 1;
//                  if (b.type and BUTTYPE)=TOGN then
//                  push:= {not}0=push;
//                  DrawBut(b,push);
//                end;
//                else
//                begin
//                  lvalue:= value;
//                  if value=0.0 then
//                  push:= 1;
//                  else
//                  push:= 0;
//                  if (b.type and BUTTYPE)=TOGN then
//                  push:= {not}0=push;
//                  SetButVal(b, {float(}push,push);
//                  DrawBut(b,push);
//                end;
//                while get_mbut()@L_MOUSE
//                do
//                usleep(1);
//                if b.func<>0 then
//                begin
//                  b.func(lvalue);
//                  winset(winakt);
//                end;
//              end;
//              ROW:
//              begin
//                bt:= bb.first;
//                for{while} temp:=0 to Pred(totbut) {temp++}
//                do
//                begin
//                  if nr<>temp)and((bt.type and BUTTYPE)=ROW then
//                  begin
//                    if bt.min=b.min then
//                    begin
//                      DrawBut(bt,0);
//                    end;
//                  end;
//                  inc(bt);
//                end;
//                SetButVal(b,b.max, {integer(}b.max);
//                DrawBut(b,1);
//              end;
//              SLI:
//              begin
//                DrawBut(b,1);
//                do_sliderbut(b);
//                DrawBut(b,0);
//              end;
//              NUM:
//              (* trekbut *)
//              begin
//                DrawBut(b,1);
//                ButtonsGetmouse(mval);
//                a:= GetButVal(b,@value,@lvalue);
//                sx:= mval[0];
//                fstart:=  {float(}(b.max-b.min);
//                fstart:= (value-b.min) div fstart;
//                f:= fstart;
//                temp:= lvalue;
//                tempf:= value;
//                getshift:= get_qual()@LR_SHIFTKEY;
//                while get_mbut()@L_MOUSE
//                do
//                begin
//                  qual:= get_qual();
//                  if get_mbut()@(M_MOUSE or R_MOUSE))or(getshift then
//                  begin
//                    (* maak er textbut van *)
//                    act_as_textbut(b);
//                    DrawBut(b,0);
//                    begin
//                      result:= b.nr;
//                      exit;
//                    end;
//                  end;
//                  deler:= 500;
//                  if (b.type and BUTPOIN)<>FLO then
//                  begin
//                    if (b.max-b.min)<100 then
//                    deler:= 200.0;
//                    if (b.max-b.min)<25 then
//                    deler:= 50.0;
//                  end;
//                  if qual and LR_SHIFTKEY then
//                  deler:= deler * (10.0);
//                  if qual and LR_ALTKEY then
//                  deler:= deler * (20.0);
//                  ButtonsGetmouse(mval);
//                  if mval[0]<>sx then
//                  begin
//                    f:= f + (( {float(}(mval[0]-sx)) div deler);
//                    if f>1.0 then
//                    f:= 1.0;
//                    if f<0.0 then
//                    f:= 0.0;
//                    sx:= mval[0];
//                    tempf:= (b.min+f*(b.max-b.min));
//                    if a=1 then
//                    begin
//                      temp:= ffloor(tempf+.5);
//                      if tempf=b.min)or(tempf=b.max then
//                      ;
//                      else
//                      if qual and LR_CTRLKEY then
//                      temp:= 10*(temp div 10);
//                      if temp>=b.min)and(temp<=b.max then
//                      begin
//                        a:= GetButVal(b,@value,@lvalue);
//                        if temp<>lvalue then
//                        begin
//                          pos:= 1;
//                          SetButVal(b,tempf,temp);
//                          DrawBut(b,1);
//                          if b.func<>0 then
//                          b.func();
//                          winset(winakt);
//                        end;
//                      end;
//                    end;
//                    else
//                    begin
//                      temp:= 0;
//                      if qual and LR_CTRLKEY then
//                      begin
//                        if tempf=b.min)or(tempf=b.max then
//                        ;
//                        else
//                        if b.max-b.min<2.10 then
//                        tempf:= 0.1*ffloor(10*tempf);
//                        else
//                        if b.max-b.min<21.0 then
//                        tempf:= ffloor(tempf);
//                        else
//                        tempf:= 10.0*ffloor(tempf div 10.0);
//                      end;
//                      if tempf>=b.min)and(tempf<=b.max then
//                      begin
//                        a:= GetButVal(b,@value,@lvalue);
//                        if tempf<>value then
//                        begin
//                          pos:= 1;
//                          SetButVal(b,tempf,temp);
//                          DrawBut(b,1);
//                        end;
//                      end;
//                    end;
//                  end;
//                  usleep(1);
//                end;
//                if pos=0 then
//                begin
//                  (* plus 1 of min 1 *)
//                  if a=1 then
//                  begin
//                    if sx<(b.x1+b.x2) div 2 then
//                    dec(temp);
//                    else
//                    inc(temp);
//                    if temp>=b.min)and(temp<=b.max then
//                    SetButVal(b,tempf,temp);
//                  end;
//                  else
//                  begin
//                    if qual<>0 then
//                    tempf:=  {integer(}(tempf+0.5);
//                    else
//                    begin
//                      if sx<(b.x1+b.x2) div 2 then
//                      tempf:= tempf - (0.$1*b.a1);
//                      else
//                      tempf:= tempf + (0.$1*b.a1);
//                    end;
//                    if tempf>=b.min)and(tempf<=b.max then
//                    SetButVal(b,tempf,temp);
//                  end;
//                end;
//                DrawBut(b,0);
//              end;
//              NUMSLI,
//              HSVSLI:
//              begin
//                donumslibut(b);
//              end;
//              BUTRET:
//              (* keert terug, daarna SetBut aanroepen *)
//              begin
//                DrawBut(b,1);
//                {goto next_label;}{<= !!!d case label without "break"}
//              end;
//              LABEL:
//              (* keert terug, hoeft niet te tekenen  *)
//              begin
//              end;
//              TOG3:
//              (* driestand met bitjes *)
//              begin
//                if (b.type and BUTPOIN)=SHO then
//                begin
//                  sp:=  {psmallint(}b.poin;
//                  if BTST(sp[1],b.type and 31) then
//                  begin
//                    sp[1]:= BCLR(sp[1],b.type and 31);
//                    sp[0]:= BCLR(sp[0],b.type and 31);
//                    DrawBut(b,0);
//                  end;
//                  else
//                  if BTST(sp[0],b.type and 31) then
//                  begin
//                    sp[1]:= BSET(sp[1],b.type and 31);
//                    DrawBut(b,1);
//                  end;
//                  else
//                  begin
//                    sp[0]:= BSET(sp[0],b.type and 31);
//                    DrawBut(b,1);
//                  end;
//                end;
//                else
//                begin
//                  if BTST(*(b.poin+2),b.type and 31) then
//                  begin
//                    *(b.poin+2):=BCLR(*(b.poin+2),b.type and 31);
//                    *(b.poin):=BCLR(*(b.poin),b.type and 31);
//                    DrawBut(b,0);
//                  end;
//                  else
//                  if BTST(*(b.poin),b.type and 31) then
//                  begin
//                    *(b.poin+2):=BSET(*(b.poin+2),b.type and 31);
//                    DrawBut(b,1);
//                  end;
//                  else
//                  begin
//                    *(b.poin):=BSET(*(b.poin),b.type and 31);
//                    DrawBut(b,1);
//                  end;
//                end;
//              end;
//              TEX:
//              (* tekstbut *)
//              begin
//                do_textbut(b);
//              end;
//              MENU:
//              begin
//                DrawBut(b,1);
//                retval:= domenubut(b);
//                DrawBut(b,0);
//              end;
//              ICONROW:
//              begin
//                DrawBut(b,1);
//                ButtonsGetmouse(mval);
//                sx:= mval[0];
//                sy:= mval[1];
//                GetButVal(b,@value,@lvalue);
//                a:= 0;
//                pos:= 0;
//                while get_mbut()@L_MOUSE
//                do
//                begin
//                  ButtonsGetmouse(mval);
//                  if abs(sx-mval[0])+abs(sy-mval[1])>2 then
//                  pos:= 1;
//                  w:= (mval[0]+mval[1]+10-sx-sy) div 20;
//                  if w<>a then
//                  begin
//                    temp:=  {integer(}value+w;
//                    if temp< {integer(}b.min then
//                    temp:= b.min;
//                    if temp> {integer(}b.max then
//                    temp:= b.max;
//                    SetButVal(b,tempf,temp);
//                    DrawBut(b,1);
//                  end;
//                  a:= w;
//                  usleep(2);
//                end;
//                if pos=0 then
//                begin
//                  if sx>(b.x1+b.x2) div 2 then
//                  temp:= value+1;
//                  else
//                  temp:= value-1;
//                  if temp< {integer(}b.min then
//                  temp:= b.min;
//                  if temp> {integer(}b.max then
//                  temp:= b.max;
//                  SetButVal(b,tempf,temp);
//                end;
//                DrawBut(b,0);
//              end;
//              IDPOIN:
//              begin
//                idpp:= (ID** )b.poin;
//                id:= {*}idpp^;
//                if id<>0 then
//                strcpy(s,id.name+2);
//                else
//                s[0]:= 0;
//                b.type:= TEX;
//                func:= b.func;
//                b.func:= 0;
//                b.poin:= s;
//                b.min:= 0.0;
//                b.max:= 22.0;
//                do_textbut(b);
//                b.poin:=  {pchar(}idpp;
//                b.func:= func;
//                b.type:= IDPOIN;
//                b.func(s,idpp);
//                DrawBut(b,0);
//              end;

//            end;{case?}
//            if retval<>0 then
//            begin
//              result:= b.nr;
//              exit;
//            end;
//            else
//            begin
//              result:= 9;
//              exit;
//            end;
//          end;
//        end;
//      end;
//      inc(b);
//    end;
//    bb:= bb.next;
//  end;
//end;
//(* ******************START VAN BUTGROUP FUNCTIES****************** *)
//type
//Link = record
//end;

//procedure BGflush;
//begin

//  while butbase.first
//  do
//  begin
//    link:= butbase.first;
//    remlink(butbase,link);
//    free(link);
//  end;
//end;
//void{!!!e unknown token}
//var
//type: smallint;
//nr: smallint;
//str: pchar;
//x1: smallint;
//y1: smallint;
//x2: smallint;
//y2: smallint;
//poin: pchar;
//min: single;
//max: single;
//a1: smallint;
//a2: smallint;
//begin
//  type
//  Bgrp = record
//end;
//bgrp:= mallocstruct(structBgrp,1);
//bgrp.type:= type;
//bgrp.nr:= nr;
//bgrp.str:= str;
//bgrp.x1:= x1;
//bgrp.y1:= y1;
//bgrp.x2:= x2;
//bgrp.y2:= y2;
//bgrp.poin:= poin;
//bgrp.min:= min;
//bgrp.max:= max;
//bgrp.a1:= a1;
//bgrp.a2:= a2;
//bgrp.font:= BGfont;
//bgrp.col:= BGcol;
//bgrp.drawtype:= BGdrawtype;
//bgrp.func:= 0;
//addtail(butbase,bgrp);
//end;
//void{!!!e unknown token}
//var
//type: smallint;
//nr: smallint;
//str: pchar;
//x2: smallint;
//y2: smallint;
//poin: pchar;
//min: single;
//max: single;
//a1: smallint;
//a2: smallint;
//begin
//  x1: smallint = 0;
//  y1: smallint = 0;
//  BGadd(type,nr,str,x1,y1,x2,y2,poin,min,max,a1,a2);
//end;
//type
//Bgrp = record
//end;(* definitie van newline: poin = x2 = y2 = 0 *)

//procedure BGnewline;
//begin
//  bgrp:= callocstruct(structBgrp,1);
//  addtail(butbase,bgrp);
//end;
//void{!!!e unknown token}
//var
//x: smallint;
//y: smallint;
//w: word;
//h: word;
//begin
//  BG_x:= x;
//  BG_y:= y;
//  BG_w:= w;
//  BG_h:= h;
//end;
//void{!!!e unknown token}
//x: word;
//y: word;
//begin
//  BG_xsp:= x;
//  BG_ysp:= y;
//end;
//void{!!!e unknown token}
//dir: char;
//begin
//  case dir of
//    'U',
//    'u':
//    begin
//      BG_dir:= 'u';
//    end;
//    'D',
//    'd':
//    begin
//      BG_dir:= 'd';
//    end;
//    else
//    begin
//      printf('ButGroup: direction '%c' ignored\n',dir);
//    end;
//  end;{case?}
//end;
//type
//Bgrp = record
//end;

//procedure BGdraw;
//var
//xfac: single;
//yfac: single;
//xpos: single;
//ypos: single;
//x: smallint;
//y: smallint;

//buts: smallint;
//lines: smallint;

//maxy: smallint;
//butsfound: smallint;

//begin

//  y:=0;

//  lines:=0;

//  butsfound:=0;
//  bgrp:= (structBgrp* )butbase.first;
//  if bgrp=0 then
//  exit;
//  while bgrp
//  do
//  begin
//    fstbut:= bgrp;
//    x:= buts:=maxy:=0;
//    while bgrp.x2<>0)and(bgrp.y2<>0
//    do
//    begin
//      inc(buts);
//      x:= x + (bgrp.x2);
//      if bgrp.y2>maxy then
//      maxy:= bgrp.y2;
//      bgrp:= bgrp.next;
//      if bgrp=0 then
//      break; {<= !!!b possible in "switch" - then remove this line}
//    end;
//    inc(lines);
//    if buts<>0 then
//    begin
//      butsfound:= 1;
//      y:= y + (maxy);
//      xfac:= (BG_w-(BG_xsp*(buts-1.0))) div (float)x;
//      xpos:= BG_x;
//      while fstbut.x2<>0)and(fstbut.y2<>0
//      do
//      begin
//        fstbut.x1:= xpos+0.5;
//        xpos:= xpos + (fstbut.x2*xfac);
//        fstbut.x2:= xpos+0.5-fstbut.x1;
//        xpos:= xpos + (BG_xsp);
//        fstbut:= fstbut.next;
//        if fstbut=0 then
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//    end;
//    if bgrp<>0 then
//    bgrp:= bgrp.next;
//    (* over newline heenspringen *)
//  end;
//  if butsfound<>0 then
//  begin
//    yfac:= (BG_h-(BG_ysp*(lines-1.0))) div (float)y;
//    ypos:= BG_y;
//    if BG_dir='d' then
//    begin
//      yfac:= -yfac;
//      ypos:= ypos + (BG_h);
//    end;
//    bgrp:= (structBgrp* )butbase.first;
//    if bgrp=0 then
//    exit;
//    while bgrp
//    do
//    begin
//      fstbut:= bgrp;
//      buts:= maxy:=0;
//      while bgrp.x2<>0)and(bgrp.y2<>0
//      do
//      begin
//        buts:= 1;
//        if bgrp.y2>maxy then
//        maxy:= bgrp.y2;
//        bgrp:= bgrp.next;
//        if bgrp=0 then
//        break; {<= !!!b possible in "switch" - then remove this line}
//      end;
//      if buts<>0 then
//      begin
//        while fstbut.x2<>0)and(fstbut.y2<>0
//        do
//        begin
//          fstbut.y1:= ypos+0.5;
//          fstbut.y2:= (ypos+0.5+yfac*fstbut.y2)-fstbut.y1;
//          if BG_dir='d' then
//          begin
//            fstbut.y2:= -fstbut.y2;
//            fstbut.y1:= fstbut.y1 - (fstbut.y2);
//          end;
//          fstbut:= fstbut.next;
//          if fstbut=0 then
//          break; {<= !!!b possible in "switch" - then remove this line}
//        end;
//        ypos:= ypos + (yfac*maxy);
//      end;
//      if BG_dir='d' then
//      ypos:= ypos - (BG_ysp);
//      else
//      ypos:= ypos + (BG_ysp);
//      if bgrp<>0 then
//      bgrp:= bgrp.next;
//      (* over newline heenspringen *)
//    end;
//  end;
//  bgrp:= (structBgrp* )butbase.first;
//  while bgrp
//  do
//  begin
//    if bgrp.x2<>0)and(bgrp.y2<>0 then
//    begin
//      SetButFont(bgrp.font);
//      SetButCol(bgrp.col);
//      SetButShape(bgrp.drawtype);
//      SetButFunc(bgrp.func);
//      DefBut(bgrp.type,bgrp.nr,bgrp.str,bgrp.x1,bgrp.y1,bgrp.x2,bgrp.y2,bgrp.poin,bgrp.min,bgrp.max,bgrp.a1,bgrp.a2);
//    end;
//    bgrp:= bgrp.next;
//  end;
//  BGflush();
//end;

//{glIndexi(i) ColorFunc(i)}
//function glIndexi(i: integer): integer;
//begin
//  result:= ColorFunc(i)
//end;

procedure init_BGbutcol(num: byte;
drawmode : smallint;
 rgb : smallint;
 back : dword;
 paper_sel : dword;
 paper_desel : dword;
 pen_sel : dword;
 pen_desel : dword;
 border1 : dword;
 border2 : dword;
 border3 : dword;
 border4 : dword);
begin
  with BGbutcol[num] do
  begin
    drawmode :=  drawmode;
    rgb := rgb;
    back := back;
    paper_sel := paper_sel;
    paper_desel := paper_desel;
    pen_sel := pen_sel;
    pen_desel := pen_desel;
    border1 := border1;
    border2 := border2;
    border3 := border3;
    border4 := border4;
    //drmode := drmode;
    //rt := rt;
  end;
end;

initialization
butbase:= @_butbase;

init_BGbutcol(0,NORMALDRAW ,0,
	100,
	101,102,103,104,
	104,105,106,103);
init_BGbutcol(1, OVERDRAW,0,
	0,
	3,1,1,3,
	1,3,1,3 );

end.
