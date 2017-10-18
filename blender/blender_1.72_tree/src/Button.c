/**
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
 */

/* 	buttons!	  		         			*/


/* 

	ALLEEN VOOR BLENDER!!!

- En dit als initialisatie
	1. Window openen, of windownrs onthouden:
		

	3. Butblock definieren:
		DefButBlock("Naam",win,helv12,max,col,shape);

		- "Naam" is een charpointer, blokken met dezelfde naam
		   worden automatisch vrijgegeven en weer gereserveerd
		- win is een int, het GL windownummer
		- helv12 is een (void *)
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

		- b: DefBut geeft (struct But *) terug.
		- type: aangeven met #defines uit de include. Het bestaat uit 
		  maximaal vier gORde getallen:
		  soort:
			BUT		activeringsbutton, in en meteen weer uit.
			TOG		toggle, of aan of uit
			ROW		rijbutton, altijd maar 1 van de rij ingedrukt
			SLI		slider
			NUM		met muisbeweging een getal veranderen
			TEX		tekstbutton
			TOG3	driestand toggle
			BUTRET	variatie op BUT, keert na indrukken meteen terug
			TOGN	(tog NOT) toggle, of aan of uit. Hier zijn 0 en 1 omgekeerd
			LABEL	alleen tekst
			MENU	werkt als pupmenu. De aktieve optie wordt weergegeven.
			ICONTOG als tog,  volgende icon in plaatje is de ingedrukte versie
			ICONROW	er wordt 1 button getekend, met muis kan gescrold worden
			NUMSLI  links staat getal (tiepen),  rechts slider
			HSVSLI  idem: met HSV omrekenpatch
			COL		afbeelding kleur: zowel CHA als FLO
			
		  variabele:
			CHA	char
			SHO	short
			INT	int
			FLO	float
			FUN	functie
		  bitcodes:
			BIT	activeert bitvariabele
		  bitnummer:
			0-31	bitnummer uit variabele (van rechts af)

		  voorbeeld voor type:
			BUT		(zonder variabele)
			SLI|CHA		(slider die char verandert)		  
			TOG|SHO|BIT|0	(van de short toggelt bitje 0)
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
			nr=DoButtons();	(in hoofdlus)
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
			foreach j (*.c) utFontSize(size8, size6);
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
			- returnval aangeven met %x[nr]	(bijv: %x12). Zonder returnval wordt
				automatisch geteld: beginnend bij 0.
		In poin wordt de waarde geschreven.
	10. ICONROW
		DefBut(ICONROW|VAR, nr, "ICON a b c", x, y, b, h, poin, min, max);
		De waarde in poin bepaalt welke icon getekend wordt, van
		icon c tot icon (c+max-min).
	11.	COL
		DefBut(COL|VAR, nr, "", x, y, b, h, poin);
		Als VAR zijn CHA en FLO toegestaan
		voorlopig alleen RGB geimplementeerd
	12.	IDPOIN
		DefBut(IDPOIN, nr, "naam", x, y, b, h, poinpoin);
		alleen voor Blender: IDpointer,  button displayt naam id.
		Ingedrukt is het een tekstbut. LETOP: pointerpointer!.
		Altijd een funktie meeegeven: func(newname, poinpoin);
		
******************************************************			*/

/* Omschrijving ButGroups
 * 
 * 1. ButGroups vormen een extra laag tussen gebruiker en button structuur
 *    die het mogelijk maakt om een relatieve positie / grootte van een
 *    groep buttons binnen een rechthoekig kader aan te geven.
 * 2. Alle functienamen beginnen met BG.
 * 
 * Functies:
 * 
 * BGflush()
 *	Wist alle nog onverwerkte buttons. Bij voorkeur aanroepen
 *	voordat je begint.
 * BGadd(, , , , ...)
 *	Voegt een Button aan de lijst toe. Zelfde definite als DefBut, 
 *	met dit verschil dat x en y genegeerd worden, en dat breedte
 *	en hoogte relatieve waardes zijn. Een button met breedte 20
 *	wordt twee keer zo breed als een button met breedte 10. Als y
 *	hoogte voor een regel wordt de maximale y van die regel aan-
 *	gehouden. Buttons houden hun eigen hoogte en lijnen afhankelijk
 *	van de richting waarin de buttons getekend worden met de boven of
 *	onderkant van de regel (niet getest).
 * BGnewline()
 *	Aanroepen om aan te geven dat volgende buttons op een nieuwe 
 *	regel moeten komen. Meerdere keren aanroepen om extra spatiering
 *	tussen regels te krijgen is toegestaan. Niet aanroepen bij begin
 *	en eind van BG defintie, tenzij je daar ook spatiering wilt.
 * BGposition(xofs, yofs, width, height)
 *	Om kader aan te geven waarbinnen buttons getekent moeten worden.
 *	Kan op elk moment binnen BG definite aangeroepen worden, maar in
 *	ieder geval voor BGdraw. Start waardes (0, 0, 100, 100).
 * BGspacing(x, y)
 *	Om absolute globale spatiering tussen buttons aan te geven.
 *	Kan op elk moment binnen BG definite aangeroepen worden, maar in
 *	ieder geval voor BGdraw. Start waardes (2, 2).
 * BGdirection(dir)
 *	Of buttons van onder naar boven, of van boven naar onder getekend
 *	moeten worden. Mogelijke waardes: 'u' (up), of 'd' (down).
 *	Kan op elk moment binnen BG definite aangeroepen worden, maar in
 *	ieder geval voor BGdraw. Start waarde ('u').
 * BGdraw()
 *	Tekent gedefinieerde buttons binnen rechthoek BGposition(), met
 *	spatiering BGspacing() in de richting BGdirection(). Hierna wordt
 *	de lijst geflushed.
 */
 
#include "blender.h"
#include "graphics.h"
#include "screen.h"

/* ************ GLOBALS ************* */

struct ButBlock *BGfirstbutblock= 0;

struct But *BGfirst;
short BGaantal;
short BGteller;
short BGwin;
void *BGfont=0, *BGfonts=0, *BGfontss=0;
short BGcol;
short BGdrawtype;
void (*ButDrawFunc)();
void (*SliderDrawFunc)();
static void do_textbut();
void SetButsWin(short win, short min, short max);

float Bxasp=1.0;
int Bfrontbuf= 0, BGaddicon= 0, BGlock= 0, Btextleft= 0, Bnotext= 0;
static char *BGlockstr= NULL;

struct ButCol BGbutcol[20]= {
	{ NORMALDRAW,0,
	100,
	101,102,103,104,
	104,105,106,103},
	{ OVERDRAW,0,
	0,
	3,1,1,3,
	1,3,1,3 }
};

struct ButIcon BGicon[10];

/* ************ BUTGRP GLOBALS ************* */

struct ListBase _butbase = {
	0,0};
struct ListBase *butbase = &_butbase;
ushort BG_xsp = 2 , BG_ysp = 2,BG_w=100, BG_h = 100;
short BG_x = 0, BG_y = 0, BG_dir = 'u';

/* ******************************** */

void drawcolorband(ColorBand *coba, float x1, float y1, float sizex, float sizey)
{
	CBData *cbd;
	float v3[2], v1[2], v2[2];
	int a;
	
	if(coba==0) return;
	
	/* outline */
	v1[0]= x1; v1[1]= y1;
	glLineWidth((GLfloat)(3));
	cpack(0x0);
	glBegin(GL_LINE_LOOP);
		glVertex2fv(v1);
		v1[0]+= sizex;
		glVertex2fv(v1);
		v1[1]+= sizey;
		glVertex2fv(v1);
		v1[0]-= sizex;
		glVertex2fv(v1);
	glEnd();
	glLineWidth((GLfloat)(1));


	glShadeModel(GL_SMOOTH);
	cbd= coba->data;
	
	v1[0]= v2[0]= x1;
	v1[1]= y1;
	v2[1]= y1+sizey;
	
	glBegin(GL_QUAD_STRIP);
	
	glColor3fv( &cbd->r );
	glVertex2fv(v1); glVertex2fv(v2);
	
	for(a=0; a<coba->tot; a++, cbd++) {
		
		v1[0]=v2[0]= x1+ cbd->pos*sizex;

		glColor3fv( &cbd->r );
		glVertex2fv(v1); glVertex2fv(v2);
	}
	
	v1[0]=v2[0]= x1+ sizex;
	glVertex2fv(v1); glVertex2fv(v2);
	
	glEnd();
	glShadeModel(GL_FLAT);
	
	/* hulplijntjes */
	
	v1[0]= v2[0]=v3[0]= x1;
	v1[1]= y1;
	v2[1]= y1+0.5*sizey;
	v3[1]= y1+sizey;
	
	cbd= coba->data;
	for(a=0; a<coba->tot; a++, cbd++) {
		v1[0]=v2[0]=v3[0]= x1+ cbd->pos*sizex;
		
		if(a==coba->cur) glLineWidth((GLfloat)(3));
		cpack(0x0);
		LINE2F(v1, v2);

		cpack(0xFFFFFF);
		LINE2F(v2, v3);
		
		if(a==coba->cur) {
			glLineWidth((GLfloat)(1));
		}
	}
}



/* ************ FUNC ************* */

void AutoButFontSize(void *size8, void *size6)
{
	BGfonts= size8;
	BGfontss= size6;
}

void myfmsetfont(void *font)
{

	if(BGfonts==0 || Bxasp <1.15) BGfont= font;
	else if(BGfontss==0 || Bxasp <1.59) BGfont= BGfonts;
	else BGfont= BGfontss;

	fmsetfont(BGfont);
	
}

void disable_capslock(int val)
{
	
}

void FrontbufferButs(int val)
{
	Bfrontbuf= val;
}

float Bwinmat[4][4];
int getsizex, getsizey;

void graphics_to_window(x, y)	/* voor rectwrite b.v. */
float *x, *y;
{
	float gx, gy;
	int sx, sy;
	
	gx= *x;
	gy= *y;
	*x= getsizex*(0.5+ 0.5*(gx*Bwinmat[0][0]+ gy*Bwinmat[1][0]+ Bwinmat[3][0]));
	*y= getsizey*(0.5+ 0.5*(gx*Bwinmat[0][1]+ gy*Bwinmat[1][1]+ Bwinmat[3][1]));
	
	mygetsuborigin(&sx, &sy);
	*x+= sx;
	*y+= sy;
}



void window_to_graphics(x, y)	/* voor muiscursor b.v. */
float *x, *y;
{
	float a, b, c, d, e, f, px, py;
	
	a= .5*getsizex*Bwinmat[0][0];
	b= .5*getsizex*Bwinmat[1][0];
	c= .5*getsizex*(1.0+Bwinmat[3][0]);

	d= .5*getsizey*Bwinmat[0][1];
	e= .5*getsizey*Bwinmat[1][1];
	f= .5*getsizey*(1.0+Bwinmat[3][1]);
	
	px= *x;
	py= *y;
	
	*y=  (a*(py-f) + d*(c-px))/(a*e-d*b);
	*x= (px- b*(*y)- c)/a;
	
}

void ButtonsGetmouse(short *adr)
{
	/* map muiscoordinaat invers naar geprojecteerde coordinaat */
	static int xmax, x, y;
	static short addx=0, mvalo[2]={1, 1};
	float xwin, ywin, xt, yt;
	int automove;

	if(adr==0) {
		xmax = displaysizex-1;
		
		mygetsingmatrix(Bwinmat);
		getsize(&getsizex, &getsizey);
		Bxasp= 2.0/(getsizex*Bwinmat[0][0]);
		return;
	}
	
	getmouseco_sc(adr);
	mygetsuborigin(&x, &y);

	adr[0]-= x;
	adr[1]-= y;
	
	/* testen: als aan rand scherm, automatisch mouseco's maken */
	automove= 0;
	if(mvalo[0]+x<=0 && adr[0]+x<=0) automove= 1;
	else if(mvalo[0]+x>=xmax && adr[0]+x>=xmax) automove= 2;

	if(automove) {		
		if(automove==1) addx-= 4;
		else addx+= 4;
		
		if(addx>2*xmax) addx= 2*xmax;
		else if(addx<-xmax) addx= -xmax;
		
		usleep(30);	/* at BeOS doesn't work when there's an event! */
		adr[0]+= addx;
	}
	else {
		addx= 0;
		mvalo[0]= adr[0];
		mvalo[1]= adr[1];
	}

	xwin= adr[0];
	ywin= adr[1];

	window_to_graphics(&xwin, &ywin);

	adr[0]= (short)(xwin+0.5);
	adr[1]= (short)(ywin+0.5);
}


void DefButIcon(nr, rect, xim, yim, xofs, yofs)
short nr;
uint *rect;
short xim, yim, xofs, yofs;
{
	BGicon[nr].rect= rect;
	BGicon[nr].xim= xim;
	BGicon[nr].yim= yim;
	BGicon[nr].xofs= xofs;
	BGicon[nr].yofs= yofs;
}

uint temprect[40*40*4];


void bDrawIcon(nr, paper, xnr, ynr, x, y, high)
short nr;
uint paper;
short xnr, ynr, x, y, high;		/* xnr ynr is iconnr; x,y is tekencentrum */
{
	uint *rii;		/* recticon */
	float xs, ys;
	int a, b, sizea, sizeb, rfac=256, gfac=256, bfac=256, fac;
	char *rd, *ri, *col;	/* rectdraw, recticon */
	
	rd= (char *)temprect;
	rii= BGicon[nr].rect;
	if(rii==0) {
		printf("Non existing iconrect: %d\n", nr);
		return;
	}
	/* eerste pixels zijn zwart: grid, en daarbij rand: 3 pixels totaal offset*/
	rii+= (3+ynr*BGicon[nr].yofs)*BGicon[nr].xim+ xnr*BGicon[nr].xofs+3;
	/* en natuurlijk de andere rand eraf: */
	sizea= BGicon[nr].xofs-5;
	sizeb= BGicon[nr].yofs-5;
	
	if(*rii) {
		col= (char *)&paper;	/* ABGR */
		ri= (char *)rii;	/* eerste kleur icon==paperkleur */
		
		if(ri[0] && ri[1] && ri[2]) {
			rfac= (col[RCOMP]<<8)/ri[0];
			gfac= (col[GCOMP]<<8)/ri[1];
			bfac= (col[BCOMP]<<8)/ri[2];
		}
	}
	
	for(b=sizeb; b>0; b--) {
		ri= (char *)rii;
		for(a=sizea; a>0; a--, ri+=4, rd+=4) {
			fac= (rfac*ri[0])>>8;
			if(fac>255) rd[0]= 255; else rd[0]= fac;
			fac= (gfac*ri[1])>>8;
			if(fac>255) rd[1]= 255; else rd[1]= fac;
			fac= (bfac*ri[2])>>8;
			if(fac>255) rd[2]= 255; else rd[2]= fac;
		}
		rii+= BGicon[nr].xim;
	}
	
	rii= temprect;
	
	high/= Bxasp;
	if(sizeb> high-5) {
		b= (sizeb- (high-5))/2;
		rii+= b*sizea;
		sizeb-= b;
	}
	if(sizeb<2) return;
	
	x-= sizea/2;
	y-= (sizeb+1)/2;

	xs= x; ys= y;
	x= ffloor(xs);	
	y= ffloor(ys+0.51);

#ifdef IRISGL
	glClearFlag();
#endif
	glRasterPos2i(x, y);
	if(Bxasp>1.1) glPixelZoom(1.0/Bxasp, 1.0/Bxasp);
	glDrawPixels(sizea, sizeb, GL_RGBA, GL_UNSIGNED_BYTE,  rii);
	glPixelZoom(1.0, 1.0);
	
}

void fdrawline(float x1, float y1, float x2, float y2)
{
	float v[2];

	glBegin(GL_LINE_STRIP);
	v[0] = x1; v[1] = y1;
	glVertex2fv(v);
	v[0] = x2; v[1] = y2;
	glVertex2fv(v);
	glEnd();
}

void fdrawbox(float x1, float y1, float x2, float y2)
{
	float v[2];

	glBegin(GL_LINE_STRIP);

	v[0] = x1; v[1] = y1;
	glVertex2fv(v);
	v[0] = x1; v[1] = y2;
	glVertex2fv(v);
	v[0] = x2; v[1] = y2;
	glVertex2fv(v);
	v[0] = x2; v[1] = y1;
	glVertex2fv(v);
	v[0] = x1; v[1] = y1;
	glVertex2fv(v);

	glEnd();
}

void sdrawline(short x1, short y1, short x2, short y2)
{
	short v[2];

	glBegin(GL_LINE_STRIP);
	v[0] = x1; v[1] = y1;
	glVertex2sv(v);
	v[0] = x2; v[1] = y2;
	glVertex2sv(v);
	glEnd();
}

void sdrawbox(short x1, short y1, short x2, short y2)
{
	short v[2];

	glBegin(GL_LINE_STRIP);

	v[0] = x1; v[1] = y1;
	glVertex2sv(v);
	v[0] = x1; v[1] = y2;
	glVertex2sv(v);
	v[0] = x2; v[1] = y2;
	glVertex2sv(v);
	v[0] = x2; v[1] = y1;
	glVertex2sv(v);
	v[0] = x1; v[1] = y1;
	glVertex2sv(v);

	glEnd();
}

void FreeButBlockData(struct ButBlock *bb)
{
	struct But *but;
	int a;
	
	but= bb->first;
	for(a=0; a<bb->aantal; a++, but++) {
		if(but->str && but->str != but->strdata) 
			freeN(but->str);
	}
	
	freeN(bb->first);
}

void FreeButs(char mode)
{
	struct ButBlock *bb,*bn;
	short a;

	bb= BGfirstbutblock;
	while(bb) {
		bn= bb->next;
		FreeButBlockData(bb);
		freeN(bb);
		bb= bn;
	}
	
	BGfirstbutblock= 0;
	
	if(mode!= 'i') {
		for(a=0; a<10; a++) {
			if(BGicon[a].rect) freeN(BGicon[a].rect);
		}
	}
}



short GetButVal(struct But *but, float *fvalue, int *lvalue)
{
	short type, format= 0;	/* geeft 0 als geen val, 1 is int, 2 is float */
	void *poin;

	type= but->type & BUTPOIN;
	poin= but->poin;

	if((but->type & BUTTYPE) == HSVSLI) {
		float h, s, v, *fp= (float *) poin;
		
		rgb_to_hsv(fp[0], fp[1], fp[2], &h, &s, &v);

		switch(but->str[0]) {
		case 'H': *fvalue= h; break;
		case 'S': *fvalue= s; break;
		case 'V': *fvalue= v; break;
		}
		
		format= 2;
		
	} else if( type == CHA ) {
		*lvalue= *(char *)poin;
		format= 1;
	} else if( type == SHO ) {
		*lvalue= *(short *)poin;
		format= 1;
	} else if( type == INT ) {
		*lvalue= *(int *)poin;		
		format= 1;
	} else if( type == FLO ) {
		*fvalue= *(float *)poin;
		format= 2;
	}

	if (format==1) *fvalue= (float)*lvalue;
	else if (format==2) *lvalue= (int) *fvalue;

	return format;
}

void SetButVal(struct But *but, float fvalue, int lvalue)
{
	short type;
	void *poin;

	type= but->type & BUTPOIN;
	poin= but->poin;

	/* value is een hsvwaarde: omzetten naar de rgb */
	if( (but->type & BUTTYPE)==HSVSLI ) {
		float h, s, v, *fp= (float *)but->poin;
		
		rgb_to_hsv(fp[0], fp[1], fp[2], &h, &s, &v);
		
		switch(but->str[0]) {
		case 'H': h= fvalue; break;
		case 'S': s= fvalue; break;
		case 'V': v= fvalue; break;
		}
		
		hsv_to_rgb(h, s, v, fp, fp+1, fp+2);
		
	}
	else if( type==CHA )
		*((char *)poin)= (char)lvalue;
	else if( type==SHO )
		*((short *)poin)= (short)lvalue;
	else if( type==INT )
		*((int *)poin)= lvalue;
	else if( type==FLO )
		*((float *)poin)= fvalue;
}

void SetButFont(void *font) 
{
	BGfont= font;
}

void SetButCol(short nr)
{
	BGcol= nr;
}

void SetButLock(int val, char *lockstr)
{
	BGlock |= val;
	if (val) BGlockstr= lockstr;
}

void ClearButLock()
{
	BGlock= 0;
	BGlockstr= NULL;
}

void DefButCol(nr, draw,back, pap_sel, pap_dsel, pen_sel, pen_dsel, b1, b2, b3, b4)
short nr,draw;
unsigned int back,pap_sel,pap_dsel,pen_sel,pen_dsel,b1,b2,b3,b4;
{
	struct ButCol *bc;
	char *cp1, *cp2;
	
	bc= &BGbutcol[nr];
	bc->back= back;
	bc->drmode= draw;
	if(draw==RGBDRAW) {
		bc->drmode= NORMALDRAW;
		bc->rgb= 1;
	}
	else bc->rgb= 0;

	bc->paper_sel= pap_sel;
	bc->paper_desel= pap_dsel;
	bc->pen_sel= pen_sel;
	bc->pen_desel= pen_dsel;
	bc->border1= b1;
	bc->border2= b2;
	bc->border3= b3;
	bc->border4= b4;

	if(draw==RGBDRAW) {
		
		cp1= (char *)&pap_sel;
		
		/* white */
		cp2= (char *)&(bc->border1);
		if(cp1[BCOMP]+60>255) cp2[BCOMP]= 255; else cp2[BCOMP]= cp1[BCOMP]+60;
		if(cp1[GCOMP]+60>255) cp2[GCOMP]= 255; else cp2[GCOMP]= cp1[GCOMP]+60;
		if(cp1[RCOMP]+60>255) cp2[RCOMP]= 255; else cp2[RCOMP]= cp1[RCOMP]+60;

		/* light */
		cp2= (char *)&(bc->border2);
		if(cp1[BCOMP]+35>255) cp2[BCOMP]= 255; else cp2[BCOMP]= cp1[BCOMP]+35;
		if(cp1[GCOMP]+35>255) cp2[GCOMP]= 255; else cp2[GCOMP]= cp1[GCOMP]+35;
		if(cp1[RCOMP]+35>255) cp2[RCOMP]= 255; else cp2[RCOMP]= cp1[RCOMP]+35;

		/* grey */
		cp2= (char *)&(bc->border3);
		if(cp1[BCOMP]-35<0) cp2[BCOMP]= 0; else cp2[BCOMP]= cp1[BCOMP]-35;
		if(cp1[GCOMP]-35<0) cp2[GCOMP]= 0; else cp2[GCOMP]= cp1[GCOMP]-35;
		if(cp1[RCOMP]-35<0) cp2[RCOMP]= 0; else cp2[RCOMP]= cp1[RCOMP]-35;

		/* dark */
		cp2= (char *)&(bc->border4);
		if(cp1[BCOMP]-60<0) cp2[BCOMP]= 0; else cp2[BCOMP]= cp1[BCOMP]-60;
		if(cp1[GCOMP]-60<0) cp2[GCOMP]= 0; else cp2[GCOMP]= cp1[GCOMP]-60;
		if(cp1[RCOMP]-60<0) cp2[RCOMP]= 0; else cp2[RCOMP]= cp1[RCOMP]-60;
	
	}
}

short IsButSel(struct But *b)
{
	float value;
	int lvalue;
	short a, push=0, true=1;

	a= GetButVal(b, &value, &lvalue);
	if(a==0) return 0;

	if( (b->type & BUTTYPE)==TOGN ) true= 0;

	if( (b->type & BIT) ) {
		if( BTST(lvalue,(b->type & 31)) ) return true;
		return !true;
	}

	switch(b->type & BUTTYPE) {
	case BUT:
		push= 0;
		break;
	case TOG:
	case ICONTOG:
		if(value!=0.0) push=1;
		break;
	case TOGN:
		if(value==0.0) push=1;
		break;
	case ROW:
		if(value == b->max) push=1;
		break;
	}

	return push;
}

/* ************** DRAW ******************* */


void EmbossBoxN(float x1, float y1, float x2, float y2, int sel)
{
	struct ButCol *bc= &BGbutcol[BGcol];
	float asp1, asp2;
	
	asp1= Bxasp;
	asp2= asp1+Bxasp;
		
	if(sel) {
		cpack(bc->border3);
		fdrawline(x1+asp1, y2+asp1, x2-asp1, y2+asp1);
		cpack(bc->border4);
		fdrawbox(x1, y1, x2, y2);
		x1+= asp1;
		x2-= asp1;
		y1+= asp1;
		y2-= asp1;
		
		cpack(0x0);
		fdrawbox(x1, y1, x2, y2);
		cpack(bc->border1);
		fdrawline(x1, y1-asp1, x2, y1-asp1);
		cpack(bc->border2);
		fdrawline(x1-asp1, y1-asp2, x2+asp1, y1-asp2);
		
	}
	else {
		cpack(0);
		fdrawbox(x1, y1, x2, y2);
		x1+= asp1;
		x2-= asp1;
		y1+= asp1;
		y2-= asp1;
		
		cpack(bc->border1);
		fdrawline(x1, y2, x2, y2);
		fdrawline(x1, y1+asp1, x1, y2-asp1);
		cpack(bc->border2);
		fdrawline(x1, y2-asp1, x2, y2-asp1);
		fdrawline(x1+asp1, y1+asp2, x1+asp1, y2-asp1);
		fdrawline(x2, y1+asp1, x2, y2-asp1);
		
		cpack(bc->border3);
		fdrawline(x1+asp1, y1+asp1, x2-asp1, y1+asp1);
		cpack(bc->border4);
		fdrawline(x1, y1, x2, y1);

		fdrawline(x1, y1-asp2, x2, y1-asp2);
	}
}

void EmbossBox2(short x1, short y1, short x2, short y2, short sel, uint dark, uint light)
{

	if(sel) cpack(dark); 
	else cpack(light);	
	
	glRects(x1,  y2,  x2,  y2-3);	/* boven */
	glRects(x1,  y1,  x1+3,  y2);	/* links */
	
	if(sel) cpack(light); 
	else cpack(dark);	
	
	glRects(x1,  y1,  x2,  y1+2);	/* onder */
	glRects(x2,  y1,  x2-2,  y2);	/* rechts */
	
}


void EmbossSlider1(str1,str2,f,x1,y1,x2,y2,sel,col,oud)
char *str1,*str2;
float f;
short x1,y1,x2,y2,sel,col,*oud;
{
	/* s1: getal, s2: naam, f: van 0.0 tot 1.0.
	   '*oud' zit in butstruct, is oude lengte getalstring */
	struct ButCol *bc;
	short s,h;

	h=(y2-y1);
	bc= &BGbutcol[col];

	cpack(0x505050);
	glRectf(x1, y1, x2, y2);
	EmbossBoxN((float)x1-1, (float)y1-1, (float)x2+1, (float)y2+1, 1);
	
	/* het blokje */
	sel? cpack(bc->border1): cpack(bc->paper_sel);
	glRects(x1+f, y1+1, x1+h+f, y2-1);
	cpack(bc->border2);
	
	fdrawline(x1+f, y2-1, x1+f+h, y2-1);
	cpack(0x0);
	fdrawline(x1+f, y1+1, x1+f+h, y1+1);

	if(Bnotext) return;

	/* tekst rechts */
	h= 12;
	s=Bxasp*fmgetstrwidth(BGfont,str2);
	cpack(bc->back);
	glRectf(x2+8, y1-3, x2+8+s, y1+h-5);
	cpack(bc->pen_sel);
	glRasterPos2i(x2+8, y1-2);
	fmprstr(str2);

	/* tekst links */
	s=Bxasp*fmgetstrwidth(BGfont,str1);
	cpack(bc->back);
	if( *oud==0) *oud=s;
	glRectf(x1- *oud-8, y1-3, x1-8, y1+h-4);
	*oud= s;
	cpack(bc->pen_sel);
	glRasterPos2i(x1-s-8, y1-2);
	fmprstr(str1);
}

void EmbossBut1(str, x1, y1, x2, y2, sel, col)	/* CMAP met ditherlijntjes en zwart kader */
char *str;
short x1,y1,x2,y2,sel,col;
{
	struct ButCol *bc;
	short s;

	tbox_embossbox(x1, y1, x2, y2, sel);

	if(str[0]==0) return;
	s= Bxasp*fmgetstrwidth(BGfont,str);
	if(Btextleft) x1= x1+4;
	else x1= (x1+x2-s+1)/2;

	y1= (y1+y2- 12/2-1)/2;
	glRasterPos2i(x1, y1);
	fmprstr(str);
}

void EmbossBut2(str, x1, y1, x2, y2, sel, col)	/* alleen RGBmode! */
char *str;
short x1,y1,x2,y2,sel,col;
{
	struct ButCol *bc;
	int iconr, icox, icoy;
	short s, x, y;
	char *cp1, *cp2;
	
	SetButCol(col);
	bc= &BGbutcol[col];
	
	/* background */
	if(sel!=2) {
		if(sel) cpack(bc->border3);
		else cpack(bc->paper_sel);
		
		glRects(x1+1, y1+1, x2-1, y2-1);
	}
	
	EmbossBoxN((float)x1, (float)y1, (float)x2, (float)y2, sel);
	
	/* after this function, textbutton needs color */
	if(sel) cpack(bc->pen_sel);
	else cpack(0x0);

	if( strncmp(str, "ICON", 4)==0 ) {
		sscanf(str+4, "%d %d %d\n", &iconr, &icox, &icoy);
		bDrawIcon(iconr, bc->paper_sel, icox+BGaddicon, icoy, (x1+x2)/2, (y1+y2)/2, (y2-y1));
	}
	else if(str[0]!=0) {
		
		while( (s= Bxasp*fmgetstrwidth(BGfont,str)) > x2-x1) {
			
			if(s < 10) break;
			if(str[1]==0) break;
			
			str++;
		}
		
		if(Btextleft) x= x1+4;
		else x= (x1+x2-s+1)/2;
		
		glRasterPos2f( (float)x, (float)(y1+y2- 12/2 - 3.0)/2.0);
		
		fmprstr(str);
	}
}

void EmbossBut2a(str, x1, y1, x2, y2, sel, col)	/* textbuttons alleen RGBmode! */
char *str;
short x1,y1,x2,y2,sel,col;
{
	struct ButCol *bc;
	int iconr, icox, icoy;
	short s, x, y;
	char *cp1, *cp2;
	
	SetButCol(col);
	bc= &BGbutcol[col];
	
	/* background */
	if(sel!=2) {
		cpack(bc->paper_sel);
		
		glRects(x1+1, y1+1, x2-1, y2-1);
	}
	
	EmbossBoxN((float)x1, (float)y1, (float)x2, (float)y2, 1);
	
	/* after this function, textbutton needs color */
	if(sel) cpack(bc->pen_sel);
	else cpack(0x0);

	if( strncmp(str, "ICON", 4)==0 ) {
		sscanf(str+4, "%d %d %d\n", &iconr, &icox, &icoy);
		bDrawIcon(iconr, bc->paper_sel, icox+BGaddicon, icoy, (x1+x2)/2, (y1+y2)/2, (y2-y1));
	}
	else if(str[0]!=0) {
		
		while( (s= Bxasp*fmgetstrwidth(BGfont,str)) > x2-x1) {
			
			if(s < 10) break;
			if(str[1]==0) break;
			
			str++;
		}
		
		if(Btextleft) x= x1+4;
		else x= (x1+x2-s+1)/2;
		
		glRasterPos2f( (float)x, (float)(y1+y2- 12/2 - 3.0)/2.0);
		
		fmprstr(str);
	}
}

void TextLabelBut(str,x1,y1,x2,y2,sel, col)
char *str;
short x1,y1,x2,y2,sel,col;
{
	int y;
	

	if(str[0]!=0) {
		if(sel) cpack(0xFFFFFF);
		else cpack(0x0);
		
		y= (y1+y2- 12/2-2)/2;
		glRasterPos2i(x1,  y);
		fmprstr(str);
	}
}

short tekentekstbut(struct But *b, char *s, short *pos)
{
	short temp,h,t,texcol,ofs;
	char s1[150],ch;


	h=( b->y1 + b->y2 -  12/2 -2)/2;
	s1[0]=0;

	if(*pos== -1) {		/* niet aktieve tekstbut */
		ButDrawFunc(s1,b->x1,b->y1,b->x2,b->y2,0,b->col);
		strcpy(s1,b->str);
		strcat(s1,s);
		ofs=0;
		while(Bxasp*fmgetstrwidth(BGfont,s1+ofs)>(b->x2 - b->x1 -10)) ofs++;
		glRasterPos2i(b->x1+4, h);
		cpack(BGbutcol[b->col].pen_desel);
		fmprstr(s1+ofs);
	}
	else {			/*  aktieve tekstbut */
		ButDrawFunc(s1,b->x1,b->y1,b->x2,b->y2,1,b->col);
		strcpy(s1,b->str);
		strcat(s1,s);
		if(*pos < strlen(b->str)) *pos = strlen(b->str);
		if(*pos > strlen(s1)) *pos = strlen(s1);
		ch=s1[*pos];
		s1[*pos]= 0;
		ofs=0;
		while(Bxasp*fmgetstrwidth(BGfont,s1+ofs)>(b->x2 - b->x1 -10)) ofs++;

		/* de cursor */
		t=Bxasp*fmgetstrwidth(BGfont,s1+ofs)+3;
		cpack(0xFF);
	
		glRects(b->x1+t, b->y1+2, b->x1+t+3, b->y2-2);

		texcol= BGbutcol[b->col].pen_sel;
		if(ofs) {
			cpack(texcol);
			glRasterPos2i(b->x1+4, h);
			fmprstr(s1+ofs);
		}
		if(ofs==0) {
			s1[*pos]=ch;
			temp = 0;
			while(Bxasp*fmgetstrwidth(BGfont,s1+temp)>(b->x2 - b->x1 -10)) temp++;
			cpack(texcol);
			glRasterPos2i(b->x1+4, h);
			s1[strlen(s1)-temp]=0;
			fmprstr(s1);
		}

	}
	
	return (ofs);
}

/* *********** PC PATCH ************* */

#ifdef glIndexi
#undef glIndexi
#endif
#define glIndexi(i)	ColorFunc(i)

/* ****** MENUBUT ****** */

extern float tbwinmat[4][4], tbprojmat[4][4];
extern short oldmap[4][3], tboldwin;
extern ushort tbpat[16];
extern int tbfontyofs;

void butmenu_draw(startx, starty, width, height, items, title, spoin, rows, columns)
int startx, starty, width, height, items;
char *title, **spoin;
int rows, columns;
{
	int a;
	int x1, y1;
	
	glIndexi(TBOXBLACK);

	if(title) {
		x1= startx;
		y1= starty+4 + rows*height;
		
		tbox_embossbox(x1, y1, x1+width*columns, y1+height, 0);
		glRasterPos2i(x1+5,  y1+tbfontyofs);
		fmprstr(title);
	}
	
	for(a=0; a<items; a++) {	
		x1= startx + width*((int)a/rows);
		y1= starty - height*(a%rows) + (rows-1)*height; 
		
		tbox_embossbox(x1, y1, x1+width, y1+height, 2);
		glRasterPos2i(x1+5, y1+tbfontyofs);
		fmprstr(spoin[a]);
	}
}

void getname_menu_but(char *butname, char *str, int item)
{
	int itemcount=0, perc= 0;
	char *bstr;
	
	bstr= butname;
	
	while( *str) {
		
		switch( *str ) {
		case '%':
			if( str[1]=='t' ) {
				itemcount--;
			}
			else if(str[1]=='x') {
				perc= 1;
				if( atoi(str+2)==item ) {
					bstr[0]= 0;
					return;
				}
			}
			break;
		case '|':
			if(perc==0 && itemcount==item) {
				bstr[0]= 0;
				return;
			}
			itemcount++;
			bstr= butname;
			break;
		default:
			*bstr= *str;
			bstr++;
		}
		
		str++;
	}
	
	if(itemcount!=item) butname[0]= 0;
}

int domenubut(struct But *but)
{
	float fvalue;
	int width, height, a, retval[100], *retp, items, xmax, ymax, startx, starty, endx, endy;
	int act, acto= -1, value=0, event=0;
	short val, winakt=0, mval[2];
	short redrawq[10], redrawcount= 0, mousemove[2];
	char *instr, *str, *astr, *title=0, *spoin[100], **spp;
	int columns=1, rows=0, x1, y1;

	/* if(TRUE) { */
	/* 	return pupmenu(but->str); */
	/* } */

	instr= but->str;
	
	/* kopie string maken */
	str= mallocN(strlen(instr)+1, "pupmenu");
	memcpy(str, instr, strlen(instr)+1);
	
	/* eerst string uitelkaar pulken, tellen hoeveel elementen, return values */
	astr= str;
	spoin[0]= astr;
	items= 0;
	retval[0]= 0;
	while( *astr) {
		switch( *astr ) {
			case '%':
				if(astr[1]=='x') {
					retval[items]= atoi(astr+2);
					*astr= 0;
				}
				else if(astr[1]=='t') {
					title= spoin[0];
					*astr= 0;
				}
				break;
			case '|':
				if(astr[1]) {
					items++;
					spoin[items]= astr+1;

					if(title) retval[items]= items-1;
					else retval[items]= items;
				}
				*astr= 0;
				break;
		}
		astr++;
	}
	
	items++;	/* het begon bij 0 te tellen */

	if (title) {
		items--;
		spp= &spoin[1];
		retp= &retval[1];
	} else {
		spp= &spoin[0];
		retp= &retval[0];
	}
	
#if 0
	if (items>15) {
		rows= items/4;
		if (items%((int)items/3) <= items%rows) {
			rows= items/3;
			if (items%((int)items/2) <= items%rows) {
				rows= items/2;
			}
		}
	} else {
		rows= items;
	}
	if (rows<1) rows= 1;
	
	if(rows && items) {
		columns= items/rows;
		columns+= items%rows?1:0;
	}
#else
	columns= (items+20)/20;
	if (columns<1) columns= 1;
	
	rows= (int) items/columns;
	if (rows<1) rows= 1;
	
	while (rows*columns<items) rows++;
#endif
		
	/* afmetingen en plaats */
	width= 0;
	for(a=0; a<items; a++) {
		xmax= fmgetstrwidth(G.font, spp[a]);
		if(xmax>width) width= xmax;
	}
	if (width<50) width=50;
	width+= 10;
	
	height= rows*TBOXH;
	if (title) height+= TBOXH;
	
	xmax = G.curscreen->sizex;
	ymax = G.curscreen->sizey;

	getmouseco_sc(mval);
	
	GetButVal(but, &fvalue, &value);
	
	for(a=0; a<items; a++) {
		if( retp[a]==value ) break;
	}
	if(a==items) a= 0;

	startx = mval[0]-width/2 - ((int)a/rows)*width;
	starty = mval[1]-height + TBOXH/2 + (a%rows)*TBOXH;
	
	if (title) starty+= TBOXH;
	
	mousemove[0]= mousemove[1]= 0;
	
	if(startx<10) {
		mousemove[0]= 10-startx;
		startx= 10;
	}
	if(starty<10) {
		mousemove[1]= 10-starty;
		starty= 10;
	}
	
	endx= startx+width*columns;
	endy= starty+height;
	
	if(endx>xmax) {
		mousemove[0]= xmax-endx-10;
		endx= xmax-10;
		startx= endx-width*columns;
	}
	if(endy>ymax) {
		mousemove[1]= ymax-endy-10;
		endy= ymax-10;
		starty= endy-height;
	}

	warp_pointer(mval[0]+mousemove[0], mval[1]+mousemove[1]);

	mousemove[0]= mval[0];
	mousemove[1]= mval[1];

	bgnpupdraw(startx, starty, endx, endy);
	butmenu_draw(startx, starty, width, TBOXH, items, title, spp, rows, columns);

	value= 0;
	while(value==0) {
		
		if(acto== -1);
		else event= extern_qread(&val);
		
		if(event) {
			
			switch(event) {
			case LEFTMOUSE: case RIGHTMOUSE: case RETKEY: case PADENTER:
				if(val==0) {
					value= 2;
				}
				break;
			/* case MIDDLEMOUSE: */
			/* 	if(val) value= 3; */
			/* 	break; */
			case INPUTCHANGE:
				winakt= val;
				break;
			case ESCKEY:
				value= 1;
				break;
			case REDRAW:
				if(val && redrawcount<10) {
					redrawq[redrawcount]= val;
					redrawcount++;
				}
			}
		}
		else usleep(2);
		
		getmouseco_sc(mval);

		if(mval[0]<startx-20 || mval[0]>endx+20 || mval[1]<starty-30 || mval[1]>endy+40) value= 1;
		if(mval[0]<startx-20 || mval[0]>endx+20 || mval[1]<starty-30 || mval[1]>endy+40) value= 1;

		a= (mval[0]-startx)/width;
		CLAMP(a, 0, columns-1);
		act= rows * a;

		a= (mval[1]-starty)/TBOXH;
		CLAMP(a, 0, rows-1);
		act+= (rows-1) - a;
		
		if(act!=acto) {
			if(acto>=0 && acto<items) {	/* vergelijken, acto is init op -1 */
				x1= startx + width*((int)acto/rows);
				y1= starty - TBOXH*(acto%rows) + (rows-1)*TBOXH; 
				
				tbox_embossbox(x1, y1, x1+width, y1+TBOXH, 2);
				glIndexi(TBOXBLACK);
				glRasterPos2i(x1+5, y1+tbfontyofs);
				fmprstr(spp[acto]);
			}

			if(act>=0 && act<items) {
				x1= startx + width*((int)act/rows);
				y1= starty - TBOXH*(act%rows) + (rows-1)*TBOXH;
				
				tbox_embossbox(x1, y1, x1+width, y1+TBOXH, 3);
				glIndexi(TBOXBLACK);
				glRasterPos2i(x1+5, y1+tbfontyofs);
				fmprstr(spp[act]);
			}
			
			acto= act;
		}
	}
	
	endpupdraw();
	freeN(str);

	if(winakt) qenter(INPUTCHANGE, winakt);

	for(val=0; val<redrawcount; val++) qenter(REDRAW, redrawq[val]);

	warp_pointer(mousemove[0], mousemove[1]);
	
	if(value==2 && act>=0 && act<items) {
		value= retp[act];
		fvalue= (float)value;
		SetButVal(but, fvalue, value);
	}
	else if(value==3) {				/* maak er een tekstbut van */
		char butname[120], *tstr;
		float min, max;
		int temp;
		void *oldpoin, *oldstr;
		
		oldpoin= but->poin;
		oldstr= but->str;
		
		GetButVal(but, &fvalue, &value);
		getname_menu_but(butname, but->str, value);
		but->poin= butname;
		but->str= "";
		min= but->min;
		max= but->max;
		but->min= 0.0;
		but->max= 79.0;
		temp= but->type;
		but->type= TEX;
		
		do_textbut(but);
		
		but->poin= oldpoin;
		but->str= oldstr;
		but->min= min;
		but->max= max;
		but->type= temp;
		
		/* test butname en verander zonodig poin */
	}
	else return 0;
	
	return 1;
}



/* *********** END MENUBUT ********************** */

void SetButShape(short nr)
{
	BGdrawtype= nr;

	if(nr==1) {			/* overdraw cmap */
		ButDrawFunc= EmbossBut1;
		SliderDrawFunc= EmbossSlider1;
	}
	else if(nr==2) {	/* alleen RGB */
		ButDrawFunc= EmbossBut2;
		SliderDrawFunc= EmbossSlider1;
	}
	else if(nr==3) {	/* alleen RGB, voor ipo */
		ButDrawFunc= TextLabelBut;
		SliderDrawFunc= EmbossSlider1;
	}
	else if(nr==4) {	/* alleen RGB, speciale tekstbut */
		ButDrawFunc= EmbossBut2a;
		SliderDrawFunc= EmbossSlider1;
	}
}


void DrawBut2(struct But *but, short sel)
{
	ID *id;
	float f, value, *fp;
	uint tempcol;
	int x1, y1, x2, y2, s, lvalue;
	int olddraw;
	int iconr, icox, icoy;
	short *sp, ok, a,h,w,b3,b4, colr, colg, colb;
	char r,g,b, s1[120], butname[120], *cp;

	if(but==0) return;

	myfmsetfont(but->font);

	if(but->drawtype!=BGdrawtype) SetButShape(but->drawtype);

	switch (but->type & BUTTYPE) {

	case BUT: 
	case ROW: 
	case TOG: 
	case TOGN:
	case BUTRET:
		ButDrawFunc(but->str,but->x1,but->y1,but->x2,but->y2,sel,but->col);
		break;
	case ICONTOG: 
		/* tijdelijk icoon ophogen als sel */
		if(sel) BGaddicon= 1;
		ButDrawFunc(but->str,but->x1,but->y1,but->x2,but->y2,sel,but->col);
		break;
	case ICONROW:
		/* tijdelijk veranderen van icoon, daarna weer terug! */
		GetButVal(but, &value, &lvalue);
		BGaddicon= lvalue- (int)(but->min);
			
		ButDrawFunc(but->str,but->x1,but->y1,but->x2,but->y2,sel,but->col);
		
		/* teken pijltjes, icon is standaard RGB */
		a= (but->y1+but->y2)/2;
		
		cpack(0);
		sdrawline(but->x1-1, a-2, but->x1-1, a+2);
		sdrawline(but->x1-2, a-1, but->x1-2, a+1);
		sdrawline(but->x1-3, a, but->x1-3, a);
		cpack(0xFFFFFF);
		sdrawline(but->x1-3, a-1, but->x1-1, a-3);
		
		cpack(0);
		sdrawline(but->x2+1, a-2, but->x2+1, a+2);
		sdrawline(but->x2+2, a-1, but->x2+2, a+1);
		sdrawline(but->x2+3, a, but->x2+3, a);
		cpack(0xFFFFFF);
		sdrawline(but->x2+3, a-1, but->x2+1, a-3);

		break;
	case MENU:
		if(but->x2-but->x1 > 30) {
			GetButVal(but, &value, &lvalue);
			getname_menu_but(butname, but->str, lvalue);
		}
		else {
			butname[0]= 0;
			but->rt[0]= 0;
		}
		
		ButDrawFunc(butname,but->x1,but->y1,but->x2,but->y2,sel,but->col);

		/* als er ruimte is: teken symbooltje */
		if(but->rt[0]+10 < but->x2-but->x1) {

			h= but->y2- but->y1;
			x1= but->x2-0.66*h; x2= x1+.33*h;
			y1= but->y1+.42*h; y2= y1+.16*h;
		
			cpack(BGbutcol[but->col].pen_desel);
			glRecti(x1,  y1,  x2,  y2);
			cpack(BGbutcol[but->col].pen_sel);
			glRecti(x1-1,  y1+1,  x2-1,  y2+1);
		}
		
		break;
	case NUM:
		a= GetButVal(but, &value, &lvalue);
		s1[0]=0;
		if(a==1) {
			sprintf(s1,"%s%d",but->str,lvalue);
		}
		else if(a==2) {
			if(but->max<10.001) sprintf(s1, "%s%.3f", but->str, value);
			else sprintf(s1, "%s%.2f", but->str, value);
		}
		
		ButDrawFunc(s1, but->x1, but->y1, but->x2, but->y2, sel, but->col);
		
		break;
	case NUMSLI:
	case HSVSLI:
		Btextleft= 1;

		a= GetButVal(but, &value, &lvalue);
		s1[0]=0;
		if(a==1) {
			sprintf(s1,"%s%d",but->str,lvalue);
		}
		else if(a==2) {
			if(but->max<10.001) sprintf(s1, "%s%.3f", but->str, value);
			else sprintf(s1, "%s%.2f", but->str, value);
		}
		ButDrawFunc(s1,but->x1,but->y1,but->x2,but->y2, sel==1, but->col);
		Btextleft= 0;
		
		/* de slider */
		Bnotext= 1;
		x1= but->x1; x2= but->x2;
		y1= but->y1; y2= but->y2;
		
		but->x1= (but->x1+but->x2)/2;
		but->x2-= 9;
		but->y1= -4+(but->y1+but->y2)/2;
		but->y2= but->y1+6;
		
		s1[0]= 0;
		f= (value-but->min)*(but->x2-but->x1-but->y2+but->y1)/(but->max - but->min);
		SliderDrawFunc(s1, but->str, f, but->x1, but->y1, but->x2, but->y2, sel==2, but->col, but->rt);
		Bnotext= 0;
		
		but->x1= x1; but->x2= x2;
		but->y1= y1; but->y2= y2;
		
		/* kleurschuif */
		if(but->a1 && but->a1!=but->nr) {	/* colornummer, pas op oneindige loop */
			if(BGbutcol[BGcol].rgb) {
				SetButsWin(but->win, but->a1, but->a1);
			}
		}
		break;
	case TOG3:
	
		tempcol= BGbutcol[but->col].pen_sel;
		if(sel) {
			ok= 0;
			if( (but->type & BUTPOIN)==CHA ) {
				if( BTST( *(but->poin+2),(but->type & 31) ) ) ok= 1;
			}
			else if( (but->type & BUTPOIN)==SHO ) {
				sp= (short *)but->poin;
				if( BTST( sp[1], (but->type & 31) ) ) ok= 1;
			}
			if(ok) BGbutcol[but->col].pen_sel= 0xFFFF;
		}
		ButDrawFunc(but->str,but->x1,but->y1,but->x2,but->y2,sel,but->col);

		BGbutcol[but->col].pen_sel= tempcol;

		break;
	case LABEL:
		/* eerst clear */
		
		if(BGbutcol[but->col].drmode==OVERDRAW) {
			if(but->max==1.0) {
				glIndexi(BGbutcol[but->col].back);
				glRects(but->x1-1, but->y1-1, but->x2+1, but->y2+1);
			}
			if(but->min==1.0) glIndexi(BGbutcol[but->col].pen_sel);
			else glIndexi(BGbutcol[but->col].pen_desel);
		}
		else {
			if(but->max==1.0) {
				cpack(BGbutcol[but->col].back);
				glRects(but->x1-1, but->y1-1, but->x2+1, but->y2+1);
			}
			if(but->min==1.0) cpack(BGbutcol[but->col].pen_sel);
			else cpack(BGbutcol[but->col].pen_desel);
		}
		s= Bxasp*fmgetstrwidth(BGfont,but->str);
		x1= (but->x1+but->x2-s)/2;
		y1= (but->y1+but->y2- 12/2-1)/2;
		glRasterPos2i(x1, y1);
		fmprstr(but->str);

		break;
	case SLI:
		a= GetButVal(but, &value, &lvalue);
		f= (value-but->min)*(but->x2-but->x1-but->y2+but->y1)/(but->max - but->min);
		s1[0]=0;
		if(a==1) {
			sprintf(s1,"%d",lvalue);
		}
		else if(a==2) {
			sprintf(s1,"%.2f",value);
		}
		SliderDrawFunc(s1,but->str,f,but->x1,but->y1,but->x2,but->y2,sel,but->col,but->rt);

		break;
	case TEX:
		a= -1;
		tekentekstbut(but, but->poin, &a);
		break;
	case IDPOIN:
		id= *( (ID **)but->poin );
		strcpy(butname, but->str);
		if(id) strcat(butname, id->name+2);
		ButDrawFunc(butname, but->x1, but->y1, but->x2, but->y2, sel, but->col);
		break;
	case COL:
		ButDrawFunc(but->str, but->x1,but->y1,but->x2,but->y2, 2, but->col);
		if( (but->type & BUTPOIN)==FLO ) {
			fp= (float *)but->poin;
			colr= ffloor(255.0*fp[0]+0.5);
			colg= ffloor(255.0*fp[1]+0.5);
			colb= ffloor(255.0*fp[2]+0.5);
		}
		else {
			cp= (char *)but->poin;
			colr= cp[0];
			colg= cp[1];
			colb= cp[2];
		}
		glColor3ub(colr,  colg,  colb);
		glRects(but->x1+2,  but->y1+2,  but->x2-2,  but->y2-2);
		break;
	}

	BGaddicon= 0;
}

void DrawBut(struct But *but, short sel)
{
	
	if(Bfrontbuf) {
		glDrawBuffer(GL_FRONT);
		DrawBut2(but, sel);
		glDrawBuffer(GL_BACK);
	}
	DrawBut2(but, sel);
}




void SetButs(short min, short max)
{
	struct ButBlock *bb;
	struct But *but;
	int oldwin, actwin;
	short push,nr;

	bb= BGfirstbutblock;
	but=0;
	oldwin = actwin = winget();
	while(bb) {

		but= bb->first;
		for(nr=0; nr<bb->aantal; nr++) {
			if(but->nr>=min && but->nr<=max) {
				
				if(bb->window!=actwin) {
					if(mywinexist(bb->window)) {
						actwin = bb->window;
						ButtonsGetmouse(0);
						winset(actwin);
					}
					else {
						nr= bb->aantal;
						continue;
					}
				}

				push= IsButSel(but);
				DrawBut(but, push);
			}
			but++;
		}
		bb=bb->next;
	}

	if(oldwin != actwin) winset(oldwin);
}

void SetButsWin(short win, short min, short max)
{
	struct ButBlock *bb;
	struct But *but;
	int oldwin, actwin;
	short push,nr;

	bb= BGfirstbutblock;
	but=0;
	oldwin= winget();
	
	actwin= win;
	if(oldwin!=actwin) winset(actwin);
	
	while(bb) {
		
		if(bb->window==win) {
			but= bb->first;
			for(nr=0; nr<bb->aantal; nr++, but++) {
				if(but->nr>=min && but->nr<=max) {
				
					push= IsButSel(but);
					DrawBut(but, push);
				}
			}
		}
		bb=bb->next;
	}

	if(oldwin != actwin) winset(oldwin);
}


void FreeButBlock(char *str)
{
	struct ButBlock *lastb=0,*nextb,*b1,*bb;

	bb= BGfirstbutblock;
	while(bb) {
		if(strcmp(str,bb->naam)==0) break;
		bb=bb->next;
	}

	if(bb==0) return;

	nextb= bb->next;

	b1= BGfirstbutblock;

	if(bb==b1) BGfirstbutblock=nextb;
	else {
		while(b1) {
			if(b1->next==bb) {
				b1->next=nextb;
				break;
			}
			b1=b1->next;
		}
	}
	FreeButBlockData(bb);
	freeN(bb);
}


void DefButBlock(char *str, int win, void *font, short aantal, short col, short drawtype)
{
	struct ButBlock *bb;
	static short firsttime=1;

	if(firsttime) {
		firsttime= 0;
		BGfirstbutblock=0;
	}

	winset(win);
	myfmsetfont(font);
	SetButShape( drawtype );

	FreeButBlock(str);
	
	bb= callocN(sizeof(struct ButBlock),"DefButBlock");
	if(bb==0) {
		printf("Calloc error in DefButBlock\n");
		BGfirst= 0;
		return;
	}
	
	bb->next= BGfirstbutblock;
	BGfirstbutblock=bb;
	bb->first= callocN( aantal*sizeof(struct But), "DefButBlock2");
	bb->aantal=aantal;
	bb->window=win;
	strncpy(bb->naam,str,19);
	bb->naam[19]=0;

	/* globals */
	BGfirst= bb->first;
	BGaantal= bb->aantal;
	BGteller=0;
	BGwin= win;
	BGfont= font;
	BGcol= col;
	BGdrawtype= drawtype;
	BGlock= 0;
	BGlockstr= NULL;
	
	/* voor zekerheid: als winmat veranderd is, tekenen ikonen goed */
	ButtonsGetmouse(0);
}

void SetButFunc(void (*func)())
{
	struct But *b;
	
	b= BGfirst+BGteller;
	b->func= func;

}

struct But *DefBut(short type, short nr, char *str, short x1, short y1, short x2, short y2, void *poin, float min, float max, short a1, short a2)
{
	struct But *b;
	float value;
	int maxl, lvalue;
	short push,a;
	char s[120], butname[120];
	
	if( type & BUTPOIN ) {		/* er is pointer nodig */
		if(poin==0) {
			/* als pointer nul is wordt button gewist en niet gedefinieerd */

			cpack(BGbutcol[BGcol].back);
			glRects(x1,  y1,  x1+x2,  y1+y2);
			return 0;
		}
	}

	if(BGteller>=BGaantal-1) {
		struct But *obut, *nbut;
		struct But *tmp;
		int a;
		
		tmp= BGfirst;
		BGfirst= callocN( sizeof(struct But)*2*BGaantal, "new butblock");
		memcpy(BGfirst, tmp, sizeof(struct But)*BGaantal);

		obut= tmp;
		nbut= BGfirst;
		for(a=0; a<BGaantal; a++, obut++, nbut++) {
			if(obut->str == obut->strdata)
				nbut->str= nbut->strdata;
		}
				
		BGaantal*= 2;
		BGfirstbutblock->aantal= BGaantal;
		BGfirstbutblock->first= BGfirst;

		freeN(tmp);
	}
	b= BGfirst+BGteller;
	BGteller++;

	/* ga er van uit dat de font en window goed staan */

	b->type= type;
	b->nr=nr;
	if( strlen(str)>=MAXBUTSTR-1 ) {
		b->str= callocN( strlen(str)+2, "DefBut");
		strcpy(b->str, str);
	}
	else {
		b->str= b->strdata;
		strcpy(b->str, str);
	}
	b->x1= x1; 
	b->y1= y1; 
	b->x2= (x1+x2); 
	b->y2= (y1+y2);
	b->poin= poin;
	b->min= min; 
	b->max= max;
	b->a1= a1; 
	b->a2= a2;
	b->font= BGfont;
	b->lock= BGlock;
	b->lockstr= BGlockstr;
	b->col= BGcol;
	b->win= BGwin;
	b->drawtype= BGdrawtype;

	/* pixel-breedte naam: */
	if((type & BUTTYPE)==MENU) {
		
		GetButVal(b, &value, &lvalue);
		getname_menu_but(butname, b->str, lvalue);
		
		if(butname[0]) b->rt[0]= Bxasp*fmgetstrwidth(BGfont, butname);
		else b->rt[0]= 0;
	}
	else {
		if(str[0]) b->rt[0]= Bxasp*fmgetstrwidth(BGfont, str);
		else b->rt[0]= 0;
	}
	
	/* automatische breedte */
	if(x2==0) {
		x2= b->rt[0]+6;
		b->x2= (x1+x2); 
	}

	if((type & BUTTYPE)==NUM) {	/* spatie toevoegen achter naam */
		a= strlen(b->str);
		if(a>0 && a<MAXBUTSTR-2) {
			if(b->str[a-1]!=' ') {
				b->str[a]= ' ';
				b->str[a+1]= 0;
			}
		}
	}
	
	/* testen op min en max */
	switch( type & BUTTYPE ) {
	case NUM:
	case SLI:
	case NUMSLI:
	case HSVSLI:
		GetButVal(b, &value, &lvalue);
		if(value < min) value= min;
		if(value > max) value= max;
		SetButVal(b, value, lvalue);
		
		break;
	}

	push= IsButSel(b);
	DrawBut(b,push);
	return b;
}

struct But *DefButt(short type, short nr, char *str, short x1, short y1, short x2, short y2, void *poin, float min, float max, short a1, short a2, char *tip)
{
	struct But *but;
	
	but= DefBut(type, nr, str, x1, y1, x2, y2, poin, min, max, a1, a2);
	if(but) {
		but->tip = tip;
	}
	return but;
}


static void SetupFile(struct ButBlock *bb)
{
	struct But *b;
	short totbut,nr;
	FILE *fp;

	fp=fopen("butsetup","w");
	if(fp==NULL);
	else {
		b= bb->first;
		totbut= bb->aantal;
		for(nr=0;nr<totbut;nr++) {
			fprintf(fp,"%d,%d,%d,%d   %s\n",b->x1,b->y1,b->x2-b->x1,b->y2-b->y1,b->str);
			b++;
		}
		fclose(fp);
	}
}

static void EditBut(struct But *but)
{
	short mval[2],mx,my,dx,dy,w,h,s,push;
	int x,y,maxx,maxy;
	

	getmouseco_sc(mval);
	
	getorigin(&x, &y);
	getsize(&maxx, &maxy);
	mval[0]-=x; 
	mval[1]-=y;
	mx=mval[0]; 
	my=mval[1];
	
	while(get_mbut()&L_MOUSE) {
	
		getmouseco_sc(mval);
		
		mval[0]-=x; 
		mval[1]-=y;
		dx=mval[0]-mx; 
		dy=mval[1]-my;
		if(get_qual()&LR_SHIFTKEY) {
			dx/=4; 
			dy/=4;
		}
		if(dx!=0 || dy!=0) {
			if(mval[0]<maxx && mval[1]<maxy) {
				mx=mval[0]; 
				my=mval[1];
				
				cpack(BGbutcol[but->col].back);
				if(Bfrontbuf) {
					glDrawBuffer(GL_FRONT);
				}
				
				if( (but->type & BUTTYPE)==SLI) {

					h= 12;
					s=Bxasp*fmgetstrwidth(BGfont,but->str);
					
					glRectf(but->x2+6, but->y1-4, but->x2+9+s, but->y1+h);

					s= but->rt[0];
					glRectf(but->x1-12-s,  but->y1-4,  but->x1-7,  but->y1+h);
				}
	
				glRects(but->x1-2,  but->y1-2,  but->x2+2,  but->y2+2);
				
				if(!(get_qual()&LR_ALTKEY)) {
					but->x1+=dx;
					but->y1+=dy;
				}
				but->x2+=dx;
				but->y2+=dy;
				push= IsButSel(but);
				DrawBut(but, push);

				if(Bfrontbuf) {
					glDrawBuffer(GL_BACK);
				}
			}
		}
	}
}

static void do_textbut(struct But *b)
{
	ushort dev;
	short val, temp, x, mval[2], c, len=0,pos=0,ofs,qual;
	char s[150];
			
	ButtonsGetmouse(mval);

	/* doen: eerst pos berekenen ahv muisco */
	DrawBut(b,1);	/* ivm met globals zoals tekenmode */
	strcpy(s,b->poin);
	pos= 150;
	ofs = tekentekstbut(b,s,&pos);

	while((Bxasp*fmgetstrwidth(BGfont,s+ofs) + b->x1 + 4) > mval[0]) {
		if (pos <= ofs) break;
		pos--;
		s[pos] = 0;
	}


	if(Bfrontbuf) glDrawBuffer(GL_FRONT);

	strcpy(s,b->poin);
	tekentekstbut(b,s,&pos);

	while (get_mbut() & L_MOUSE) usleep(1);
	len=strlen(s);
	b->min= 0.0;
	
#ifdef IRISGL
	qdevice(KEYBD);
#endif
	while(TRUE) {
			
		dev = extern_qread(&val);

		if(dev==INPUTCHANGE) break;
		else if(get_mbut() & L_MOUSE) break;
		else if(get_mbut() & R_MOUSE) break;
		else if(dev==ESCKEY) break;
		else if(dev==MOUSEX) val= 0;
		else if(dev==MOUSEY) val= 0;
		else if(dev==0) usleep(1);

		if(dev==KEYBD && val) {
			c= val;

			switch(c) {
			case 0:
				break;
			case '\b':	case 'b'+100: /* backspace */
				if(len!=0) {
					if(get_qual()&LR_SHIFTKEY) {
						s[0]= 0;
						pos= 0;
						len= 0;
					}
					else {
						temp=pos-strlen(b->str);
						if(temp>0) {
							for(x=temp;x<=strlen(s);x++)
								s[x-1]=s[x];
							pos--;
							s[--len]='\0';
						}
					}
				} 
				break;
			case '\n':
			case '\r':
				/* niet doen: rawkey afhandelen */
				break;
			default:
				if( c>31 && c<127) {
					if(len < b->max) {
						temp= pos-strlen(b->str);
						for(x= b->max; x>temp; x--)
							s[x]= s[x-1];
						s[temp]= c;
						pos++; 
						len++;
						s[len]= '\0';
					}
				}
			}
		}
		else if(val) {
		
			if(dev==RIGHTARROWKEY) {
				if(G.qual & LR_SHIFTKEY) pos= 150;
				else pos++;
			}
			else if(dev==LEFTARROWKEY) {
				if(G.qual & LR_SHIFTKEY) pos= 0;
				else if(pos>0) pos--;
			}
			else if(dev==PADENTER || dev==RETKEY) {
				b->min= 1.0;
				break;
			}
		}
		if(val && myqtest()==0) {
			tekentekstbut(b, s, &pos);
		}
	}
#ifdef IRISGL
	unqdevice(KEYBD);
#endif
	
	if(dev!=ESCKEY) strcpy(b->poin,s);
	pos= -1;
	
	if(b->func) b->func(b->poin);


	if(Bfrontbuf) {
		tekentekstbut(b, b->poin, &pos);
	}
	glDrawBuffer(GL_BACK);
	tekentekstbut(b, b->poin, &pos);
}

static void act_as_textbut(struct But *b)
{
	float min, max, value;
	int a, lvalue, temp;
	char s[120], *point;
	
	GetButVal(b, &value, &lvalue);
	if( (b->type & BUTPOIN)==FLO ) {
		sprintf(s, "%.4f", value);
	}
	else {
		sprintf(s, "%d", lvalue);
	}
	point= b->poin;
	b->poin= s;
	min= b->min;
	max= b->max;
	b->min= 0.0;
	b->max= 15.0;
	temp= b->type;
	b->type= TEX;
	
	do_textbut(b);
	
	b->type= temp;
	b->poin= point;
	if( (b->type & BUTPOIN)==FLO ) {
		value= atof(s);
		if(value<min) value= min;
		if(value>max) value= max;
		lvalue= value;
	}
	else {
		lvalue= atoi(s);;
		if(lvalue<min) lvalue= min;
		if(lvalue>max) lvalue= max;
		value= lvalue;
	}
	SetButVal(b, value, lvalue);
	b->min= min;
	b->max= max;
}

static void do_sliderbut(struct But *but)
{
	float f, fstart, tempf, deler, value;
	int a, sx, h, temp, pos=0, lvalue;
	short mval[2], qual;

	a= GetButVal(but, &value, &lvalue);
	ButtonsGetmouse(mval);
	sx= mval[0];
	h= but->y2-but->y1;
	fstart= but->max-but->min;
	fstart= (value - but->min)/fstart;
	temp= 32767;
	DrawBut(but,2);
	if( (but->type & BUTTYPE)==NUMSLI) deler= ( (but->x2-but->x1)/2 - h);
	else if( (but->type & BUTTYPE)==HSVSLI) deler= ( (but->x2-but->x1)/2 - h);
	else deler= (but->x2-but->x1-h);
	
	while (get_mbut() & L_MOUSE) {
		qual= get_qual();
		ButtonsGetmouse(mval);
		f= (float)(mval[0]-sx)/deler +fstart;
		if(qual & LR_CTRLKEY) {
			if(qual & LR_SHIFTKEY) f= ffloor(f*100.0)/100.0;
			else f= ffloor(f*10.0)/10.0;
		} else if (qual & LR_SHIFTKEY) {
			f= (f-fstart)/10.0 + fstart;
		}
		CLAMP(f, 0.0, 1.0);
		tempf= but->min+f*(but->max-but->min);
		
		temp= ffloor(tempf+.5);
		GetButVal(but, &value, &lvalue);
		
		if((a==1 && temp != lvalue) || (tempf != value )) {
			pos=1;
			SetButVal(but,tempf,temp);
			DrawBut(but,2);
			if(but->func) but->func(but);
		} else usleep(1);
	}
	
	if(temp!=32767 && pos==0) {  /* plus 1 of min 1 */
		
		if( (but->type & BUTTYPE)==SLI) f= (float)(mval[0]-but->x1)/(but->x2-but->x1-h);
		else f= (float)(mval[0]- (but->x1+but->x2)/2)/( (but->x2-but->x1)/2 - h);
		
		f= but->min+f*(but->max-but->min);
		
		if(a==1) {

			if(f<temp) temp--;
			else temp++;
			if( temp>=but->min && temp<=but->max)
				SetButVal(but, tempf, temp);
		
		} else {

			if(f<tempf) tempf-=.01;
			else tempf+=.01;
			if( tempf>=but->min && tempf<=but->max)
				SetButVal(but, tempf, temp);

		}
	}
}

void donumslibut(struct But *but)
{
	float value;
	int lvalue;
	short mval[2];
	
	/* eerste bepalen of het slider is of textbut */
	ButtonsGetmouse(mval);
	
	if(mval[0]>= -6+(but->x1+but->x2)/2 ) {	/* slider */
		DrawBut(but, 2);
		do_sliderbut(but);
	}
	else {
		DrawBut(but, 1);
		act_as_textbut(but);
	}

	while(get_mbut()&L_MOUSE) usleep(1);
	
	DrawBut(but, 0);	
	
	/* hsv patch */
	if((but->type & BUTTYPE)==HSVSLI) {
		if(but->str[0]=='H') {
			DrawBut(but+1, 0);
			DrawBut(but+2, 0);
		} else if(but->str[0]=='S') {
			DrawBut(but+1, 0);
			DrawBut(but-1, 0);
		} else if(but->str[0]=='V') {
			DrawBut(but-1, 0);
			DrawBut(but-2, 0);
		}
	}
}

char *GetButTip()
{
	struct But *b;
	struct ButBlock *bb;
	float value;
	int nr, temp, winakt, totbut, lvalue;
	short mval[2];
	static char butname[128];
	
	/* aktieve window */
	winakt=winget();
	if(winakt==0) return NULL;

	bb= BGfirstbutblock;
	while(bb) {
	
		b=0;
		while(bb) {
			if(bb->window==winakt) {
				b= bb->first;
				totbut= bb->aantal;
				break;
			}
			bb= bb->next;
		}
	
		if(b==0) return 0;  /*  geen actieve window */
	
		ButtonsGetmouse(0);
		ButtonsGetmouse(mval);
	
		for(nr=0; nr<totbut; nr++, b++) {
			
			if(mval[0]>=b->x1 && mval[0]<b->x2) {
				
				if( (b->type & BUTTYPE)==SLI ) temp=4;
				else temp= 0;
				
				if(mval[1]>=b->y1-temp && mval[1]<=b->y2+temp) {
					if(b->tip) return b->tip;
					else return "";
				}
			}
		}
		bb= bb->next;
	}
	
	return 0;
}



short DoButtons()
{
	struct But *b,*bt;
	void (*func)();
	ID **idpp, *id;
	struct ButBlock *bb;
	float f,fstart,deler,value,tempf, min, max;
	int x, y, lvalue, winakt, temp;
	short *sp, a, nr=0, mval[2], push=0, w, totbut, sx, sy, h, c;
	short len=0,pos=0,ofs,qual, retval= 1, getshift;
	char s[150];

	/* aktieve window */
	winakt=winget();
	if(winakt==0) return 0;

	bb= BGfirstbutblock;
	while(bb) {
	
		b=0;
		while(bb) {
			if(bb->window==winakt) {
				b= bb->first;
				totbut= bb->aantal;
				break;
			}
			bb= bb->next;
		}
	
		if(b==0) return 0;  /*  geen actieve window */
	
		ButtonsGetmouse(0);
		ButtonsGetmouse(mval);
	
		for(nr=0; nr<totbut; nr++) {
			if(mval[0]>=b->x1 && mval[0]<b->x2) {
			
				if( (b->type & BUTTYPE)==SLI ) temp=4;
				else temp= 0;

				if(mval[1]>=b->y1-temp && mval[1]<=b->y2+temp) {
					if(b->lock) {
						if (b->lockstr) {
							error(b->lockstr);
						}
					} else {
						if( b->type & BUTPOIN ) {		/* er is pointer nodig */
							if(b->poin==0 ) {
								printf("DoButton pointer error: %s\n",b->str);
								return 0;
							}
						}

#ifndef FREE
						if(get_qual()&LR_CTRLKEY) {
							EditBut(b);
							SetupFile(bb);
							return 0;
						}
#endif

						switch(b->type & BUTTYPE) {
						case BUT:
							DrawBut(b,1);
							push= 1;
							if(b->func==0) {
								while (get_mbut()&L_MOUSE) {
									ButtonsGetmouse(mval);
									a=0;
									if(mval[0]>b->x1)
										if(mval[0]<b->x2)
											if(mval[1]>=b->y1-1)
												if(mval[1]<=b->y2+1) a=1;
	
									if(a!=push) {
										push=a;
										DrawBut(b,push);
									
									}
									usleep(1);
								}
							}
							if(push) {
								if(b->func) {
									if(b->poin) {
										GetButVal(b, &value, &lvalue);
										b->func(lvalue);
									}
									else b->func();
									winset(winakt);
									
									while (get_mbut()&L_MOUSE) usleep(1);
								}
							}
							DrawBut(b,0);
							if(push==0) return 0;
							
							break;
						case TOG: 
						case ICONTOG: 
						case TOGN:
							GetButVal(b, &value, &lvalue);
							if(b->type & BIT) {
								w= BTST(lvalue, b->type & 31);
								if(w) lvalue = BCLR(lvalue,b->type & 31);
								else lvalue = BSET(lvalue,b->type & 31);
								SetButVal(b,value,lvalue);
								if(w) push=0; 
								else push=1;
								if((b->type & BUTTYPE)==TOGN) push= !push;
								DrawBut(b,push);
							}
							else {
								lvalue= value;
								if(value==0.0) push=1; 
								else push=0;
								if((b->type & BUTTYPE)==TOGN) push= !push;
								SetButVal(b,(float)push,push);
								DrawBut(b,push);
							}
							while (get_mbut()&L_MOUSE) usleep(1);
							
							if(b->func) {
								b->func(lvalue);
								winset(winakt);
							}
							break;
						case ROW:
							bt= bb->first;
							for(temp=0;temp<totbut;temp++) {
								if( nr!=temp && (bt->type & BUTTYPE)==ROW ) {
									if(bt->min==b->min) {
										DrawBut(bt, 0);
									}

								}
								bt++;
							}
							SetButVal(b,b->max,(int)b->max);
							DrawBut(b,1);
							break;
						case SLI:
							DrawBut(b, 1);
							do_sliderbut(b);
							DrawBut(b,0);
							break;
						case NUM:		/* trekbut */
							DrawBut(b, 1);
							ButtonsGetmouse(mval);
							a= GetButVal(b, &value, &lvalue);
							sx= mval[0];
							fstart= (float)(b->max-b->min);
							fstart= (value - b->min)/fstart;
							f= fstart;
							
							temp= lvalue;
							tempf= value;
							
							getshift= get_qual()&LR_SHIFTKEY;
							
							while (get_mbut()&L_MOUSE) {
								qual= get_qual();
								
								if(get_mbut()&(M_MOUSE|R_MOUSE) || getshift) {	/* maak er textbut van */
									act_as_textbut(b);

									DrawBut(b,0);
									
									return b->nr;
								}
								deler= 500;
								if( (b->type & BUTPOIN)!=FLO ) {

									if( (b->max-b->min)<100 ) deler= 200.0;
									if( (b->max-b->min)<25 ) deler= 50.0;

								}
								if(qual & LR_SHIFTKEY) deler*= 10.0;
								if(qual & LR_ALTKEY) deler*= 20.0;

								ButtonsGetmouse(mval);
								if(mval[0] != sx) {
									f+= ((float)(mval[0]-sx))/deler;
									if(f>1.0) f= 1.0;
									if(f<0.0) f= 0.0;
									sx= mval[0];
									tempf= ( b->min + f*(b->max-b->min));
									if(a==1) {
										
										temp= ffloor(tempf+.5);
										
										if(tempf==b->min || tempf==b->max);
										else if(qual & LR_CTRLKEY) temp=10*(temp/10);
										
										if( temp>=b->min && temp<=b->max) {
											a= GetButVal(b, &value, &lvalue);
											if(temp != lvalue ) {
												pos=1;
												SetButVal(b, tempf, temp);
												DrawBut(b,1);
												if(b->func) b->func();
												winset(winakt);
											}
										}
	
									}
									else {
										temp= 0;
										if(qual & LR_CTRLKEY) {
											if(tempf==b->min || tempf==b->max);
											else if(b->max-b->min < 2.10) tempf= 0.1*ffloor(10*tempf);
											else if(b->max-b->min < 21.0) tempf= ffloor(tempf);
											else tempf= 10.0*ffloor(tempf/10.0);
										}
										
										if( tempf>=b->min && tempf<=b->max) {
											a= GetButVal(b, &value, &lvalue);
											if(tempf != value ) {
												pos=1;
												SetButVal(b,tempf, temp);
												DrawBut(b,1);
											}
										}
	
									}
								}
								usleep(1);
							}
							if(pos==0) {  /* plus 1 of min 1 */
								if(a==1) {

									if(sx<(b->x1+b->x2)/2) temp--;
									else temp++;
									if( temp>=b->min && temp<=b->max)
										SetButVal(b, tempf, temp);

								}
								else {
									if(qual) tempf= (int)(tempf+0.5);
									else {
										if(sx<(b->x1+b->x2)/2) tempf-= 0.01*b->a1;
										else tempf+= 0.01*b->a1;
									}
									
									if( tempf>=b->min && tempf<=b->max)
										SetButVal(b, tempf, temp);

								}
							}
							DrawBut(b, 0);
							
							break;
						case NUMSLI:
						case HSVSLI:
							donumslibut(b);
							break;
						case BUTRET:	/* keert terug, daarna SetBut aanroepen */
							DrawBut(b,1);
						case LABEL:		/* keert terug, hoeft niet te tekenen  */
							break;
						case TOG3:	/* driestand met bitjes */
							if( (b->type & BUTPOIN)==SHO ) {
								sp= (short *)b->poin;
								if( BTST(sp[1], b->type & 31)) {
									sp[1]= BCLR(sp[1], b->type & 31);
									sp[0]= BCLR(sp[0], b->type & 31);
									DrawBut(b, 0);
								}
								else if( BTST(sp[0], b->type & 31)) {
									sp[1]= BSET(sp[1], b->type & 31);
									DrawBut(b, 1);
								} else {
									sp[0]= BSET(sp[0], b->type & 31);
									DrawBut(b,1);
								}
							}
							else {
								if( BTST(*(b->poin+2), b->type & 31)) {
									*(b->poin+2)= BCLR(*(b->poin+2), b->type & 31);
									*(b->poin)= BCLR(*(b->poin), b->type & 31);
									DrawBut(b, 0);
								}
								else if( BTST(*(b->poin), b->type & 31)) {
									*(b->poin+2)= BSET(*(b->poin+2), b->type & 31);
									DrawBut(b, 1);
								} else {
									*(b->poin)= BSET(*(b->poin), b->type & 31);
									DrawBut(b, 1);
								}
							}
							break;
						case TEX:	/* tekstbut */
							do_textbut(b);
							break;
						case MENU:
							DrawBut(b, 1);
							retval= domenubut(b);
							DrawBut(b, 0);
							break;
						case ICONROW:
							
							DrawBut(b, 1);
							ButtonsGetmouse(mval);
							sx= mval[0];
							sy= mval[1];
							GetButVal(b, &value, &lvalue);
							a= 0;
							pos= 0;
							while(get_mbut()&L_MOUSE) {
								ButtonsGetmouse(mval);
								if(abs(sx-mval[0])+abs(sy-mval[1]) >2) pos=1;
								w= (mval[0]+mval[1]+10-sx-sy)/20;
								if(w!=a) {
									temp= (int)value +w;
									if(temp< (int)b->min) temp= b->min;
									if(temp> (int)b->max) temp= b->max;
								
									SetButVal(b, tempf, temp);
									DrawBut(b, 1);
								}
								a= w;
								usleep(2);
							}
							if(pos==0) {
								if(sx>(b->x1+b->x2)/2) temp= value+1;
								else temp= value-1;
							
								if(temp< (int)b->min) temp= b->min;
								if(temp> (int)b->max) temp= b->max;
							
								SetButVal(b, tempf, temp);
							}
							
							DrawBut(b, 0);
							break;
						case IDPOIN:
							idpp= (ID **)b->poin;
							id= *idpp;
							if(id) strcpy(s, id->name+2);
							else s[0]= 0;
							
							b->type= TEX;
							func= b->func;
							b->func= 0;
							b->poin= s;
							b->min= 0.0;
							b->max= 22.0;
							do_textbut(b);
							b->poin= (char *)idpp;
							b->func= func;
							b->type= IDPOIN;
							b->func(s, idpp);

							DrawBut(b, 0);
							break;
						}
						
						if(retval) return b->nr; else return 9;
					}
				}
			}
			b++;
		}
	
		bb= bb->next;
	}
}

/* ******************START VAN BUTGROUP FUNCTIES****************** */



void BGflush()
{
	struct Link * link;

	while (butbase->first){
		link = butbase->first;
		remlink(butbase,link);
		free(link);
	}
}


void BGadd(type,nr,str,x1,y1,x2,y2,poin,min,max,a1,a2)
short type,nr;
char *str;
short x1,y1,x2,y2;
char *poin;
float min,max;
short a1,a2;
{
	struct Bgrp * bgrp;

	bgrp = mallocstruct(struct Bgrp,1);
	bgrp->type = type;
	bgrp->nr = nr;
	bgrp->str = str;
	bgrp->x1 = x1;
	bgrp->y1 = y1;
	bgrp->x2 = x2;
	bgrp->y2 = y2;
	bgrp->poin = poin;
	bgrp->min = min;
	bgrp->max = max;
	bgrp->a1 = a1;
	bgrp->a2 = a2;

	bgrp->font= BGfont;
	bgrp->col= BGcol;
	bgrp->drawtype= BGdrawtype;
	bgrp->func= 0;
	
	addtail(butbase,bgrp);
}


void BGaddq(type,nr,str,x2,y2,poin,min,max,a1,a2)
short type,nr;
char *str;
short x2,y2;
char *poin;
float min,max;
short a1,a2;
{
	short x1 = 0, y1 = 0;
	BGadd(type,nr,str,x1,y1,x2,y2,poin,min,max,a1,a2);
}


void BGnewline()
{
	struct Bgrp * bgrp;

	/* definitie van newline: poin = x2 = y2 = 0 */

	bgrp = callocstruct(struct Bgrp,1);
	addtail(butbase,bgrp);
}

void BGposition(x,y,w,h)
short x,y;
ushort w,h;
{
	BG_x = x;
	BG_y = y;
	BG_w = w;
	BG_h = h;
}

void BGspacing(x,y)
ushort x,y;
{
	BG_xsp = x;
	BG_ysp = y;
}

void BGdirection(dir)
char dir;
{
	switch(dir){
	case 'U':
	case 'u':
		BG_dir = 'u';
		break;
	case 'D':
	case 'd':
		BG_dir = 'd';
		break;
	default:
		printf("ButGroup: direction '%c' ignored\n",dir);
	}
}

void BGdraw()
{
	struct Bgrp * bgrp , *fstbut;
	float xfac,yfac,xpos,ypos;
	short x,y = 0,buts, lines = 0, maxy, butsfound = 0;

	bgrp = (struct Bgrp *) butbase->first;
	if (bgrp == 0) return;

	while (bgrp){
		fstbut = bgrp;
		x = buts = maxy = 0;

		while (bgrp->x2 != 0 && bgrp->y2 != 0){
			buts ++;
			x += bgrp->x2;
			if (bgrp->y2 > maxy) maxy = bgrp->y2;
			bgrp = bgrp->next;
			if (bgrp == 0) break;
		}

		lines ++;

		if (buts){
			butsfound = 1;
			y += maxy;
			xfac = (BG_w - (BG_xsp * (buts - 1.0)))/(float) x;
			xpos = BG_x;

			while (fstbut->x2 != 0 && fstbut->y2 != 0){
				fstbut->x1 = xpos + 0.5;
				xpos += fstbut->x2 * xfac;
				fstbut->x2 = xpos + 0.5 - fstbut->x1;
				xpos += BG_xsp;
				fstbut = fstbut->next;
				if (fstbut == 0) break;
			}
		}
		if (bgrp) bgrp = bgrp->next; /* over newline heenspringen */
	}

	if (butsfound){
		yfac = (BG_h - (BG_ysp * (lines - 1.0)))/(float) y;
		ypos = BG_y;
		if (BG_dir == 'd'){
			yfac = -yfac;
			ypos += BG_h;
		}

		bgrp = (struct Bgrp *) butbase->first;
		if (bgrp == 0) return;

		while (bgrp){
			fstbut = bgrp;
			buts = maxy = 0;

			while (bgrp->x2 != 0 && bgrp->y2 != 0){
				buts = 1;
				if (bgrp->y2 > maxy) maxy = bgrp->y2;
				bgrp = bgrp->next;
				if (bgrp == 0) break;
			}

			if (buts){
				while (fstbut->x2 != 0 && fstbut->y2 != 0){
					fstbut->y1 = ypos + 0.5;
					fstbut->y2 = (ypos + 0.5 + yfac * fstbut->y2) - fstbut->y1;

					if (BG_dir == 'd'){
						fstbut->y2 = - fstbut->y2;
						fstbut->y1 -= fstbut->y2;
					}
					fstbut = fstbut->next;
					if (fstbut == 0) break;
				}
				ypos += yfac * maxy;
			}
			if (BG_dir == 'd') ypos -= BG_ysp;
			else ypos += BG_ysp;

			if (bgrp) bgrp = bgrp->next; /* over newline heenspringen */
		}
	}

	bgrp = (struct Bgrp *) butbase->first;
	while (bgrp){
		if (bgrp->x2 != 0 && bgrp->y2 != 0) {

			SetButFont(bgrp->font);
			SetButCol(bgrp->col);
			SetButShape(bgrp->drawtype);
			SetButFunc(bgrp->func);
			
			DefBut(bgrp->type,bgrp->nr,bgrp->str,bgrp->x1,bgrp->y1,bgrp->x2,bgrp->y2,bgrp->poin,bgrp->min,bgrp->max,bgrp->a1,bgrp->a2);
		}
		bgrp = bgrp->next;
	}

	BGflush();
}

