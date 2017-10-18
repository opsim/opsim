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

/* ***************************************




    radio.c	nov/dec 1992
			may 1999

    - mainlus
    - toetsafhandeling

	
	- PREPROCES
		- collect meshes 
		- spitconnected	(alle vlakken met verschillende kleur en normaal)
		- setedgepointers (nodes wijzen naar buren)

	- EDITING
		- min-max patch en min-max elementsize
		- ahv bovenstaande evt patches subdividen
		- lampsubdivide
	
		- als er teveel lampen zijn voor de subdivide shooting:
			- tijdelijk patches samenvoegen
			- met de hand aangeven?
	
	- SUBDIVIDE SHOOTING
		- behalve laatste shooting bepaalt dit patch-subdivide
		- als gesubdivide patches nog > 2*minsize : doorgaan
		- op eind zoveel mogelijk elements maken
		- ook onthouden of lamp (nog) subdivide veroorzaakt.
		
	- REFINEMENT SHOOTING
		- testen op overflows (shootpatch subdividen)
		- testen op extreme kleur verlopen:
			- als nog kan: shootpatch subdividen
			- elements subdividen = overnieuw beginnen ?
		- ittereren stoppen nadat ?
		
	- DEFINITIVE SHOOTING
		- user geeft aan hoeveel vlakken maximaal en itteratie lengte.
		- nodes omzetten naar minder geheugen-intensief formaat
		- element- nodes weer samenvoegen waarin niets gebeurt (faces)
		- elements subdividen waarin veel gebeurt

 *************************************** */

#include "radio.h"

RadGlobal RG= {0, 0};

void freeAllRad()
{
	Base *base;
	extern int Ntotvert, Ntotnode, Ntotpatch;
	RPatch *rp, *next;
	
	/* clear flag that disables drawing the meshes */
	if(G.scene) {
		base= FIRSTBASE;
		while(base) {		
			if(base->object->type==OB_MESH) {
				base->flag &= ~OB_RADIO;
			}
			base= base->next;
		}
	}
	
	free_fastAll();	/* verts, nodes, patches */
	RG.patchbase.first= RG.patchbase.last= 0;
	Ntotvert= Ntotnode= Ntotpatch= 0;
	
	closehemiwindows();		/* not real windows anymore... */
	if(RG.elem) freeN(RG.elem);
	RG.elem= 0;
	if(RG.verts) freeN(RG.verts);
	RG.verts= 0;
	if(RG.topfactors) freeN(RG.topfactors);
	RG.topfactors= 0;
	if(RG.sidefactors) freeN(RG.sidefactors);
	RG.sidefactors= 0;
	if(RG.formfactors) freeN(RG.formfactors);
	RG.formfactors= 0;
	if(RG.index) freeN(RG.index);
	RG.index= 0;
	if(RG.facebase) {
		init_face_tab();	/* frees all tables */
		freeN(RG.facebase);
		RG.facebase= 0;
	}
	RG.totelem= RG.totpatch= RG.totvert= RG.totface= RG.totlamp= 0;	
}

int rad_phase()
{
	int flag= 0;
	
	if(RG.totpatch) flag |= RAD_PHASE_PATCHES;
	if(RG.totface) flag |= RAD_PHASE_FACES;
	
	return flag;
}

void rad_status_str(char *str)
{
	extern int totfastmem;
	int tot;
	char *phase;
	
	tot= (RG.totface*sizeof(Face))/1024;
	tot+= totfastmem/1024;
	
	if(RG.phase==RAD_SHOOTE) phase= "Phase: ELEMENT SUBD,  ";
	else if(RG.phase==RAD_SHOOTP) phase= "Phase: PATCH SUBD,  ";
	else if(RG.phase==RAD_SOLVE) phase= "Phase: SOLVE,  ";
	else if(RG.totpatch==0) phase= "Phase: COLLECT MESHES ";
	else if(RG.totface) phase= "Phase: FINISHED,  ";
	else phase= "Phase: INIT, ";
	
	if(RG.totpatch==0) strcpy(str, phase);
	else sprintf(str, "%s TotPatch: %d TotElem: %d Emit: %d Faces %d Mem: %d k ", phase, RG.totpatch, RG.totelem, RG.totlamp, RG.totface, tot);

	if(RG.phase==RAD_SOLVE) strcat(str, "(press ESC to stop)");
}

void rad_printstatus()
{
	/* actions always are started from a buttonswindow */
	curarea->windraw();
	screen_swapbuffers();
}

void rad_setlimits()
{
	Radio *rad= G.scene->radio;
	float fac;
	
	fac= 0.0005*rad->pama;
	RG.patchmax= RG.maxsize*fac;
	RG.patchmax*= RG.patchmax;
	fac= 0.0005*rad->pami;
	RG.patchmin= RG.maxsize*fac;
	RG.patchmin*= RG.patchmin;

	fac= 0.0005*rad->elma;
	RG.elemmax= RG.maxsize*fac;
	RG.elemmax*= RG.elemmax;
	fac= 0.0005*rad->elmi;
	RG.elemmin= RG.maxsize*fac;
	RG.elemmin*= RG.elemmin;
}

void set_radglobal()
{
	/* always call before any action is performed */
	Radio *rad= G.scene->radio;

	if(RG.radio==0) {
		/* firsttime and to be sure */
		bzero(&RG, sizeof(RadGlobal));
	}
	
	if(rad==0) return;

	if(rad != RG.radio) {
		if(RG.radio) freeAllRad();
		bzero(&RG, sizeof(RadGlobal));
		RG.radio= rad;
	}
	
	RG.hemires= rad->hemires & 0xFFF0;
	RG.drawtype= rad->drawtype;
	RG.flag= rad->flag;
	RG.subshootp= rad->subshootp;
	RG.subshoote= rad->subshoote;
	RG.nodelim= rad->nodelim;
	RG.maxsublamp= rad->maxsublamp;
	RG.maxnode= 2*rad->maxnode;		/* in button:max elem, subdividing! */
	RG.convergence= rad->convergence/1000.0;
	RG.radfac= rad->radfac;
	RG.gamma= rad->gamma;
	RG.maxiter= rad->maxiter;
	
	rad_setlimits();
}

/* called from buttons.c */
void add_radio()
{
	Radio *rad;
	
	if(G.scene->radio) freeN(G.scene->radio);
	rad= G.scene->radio= callocN(sizeof(Radio), "radio");

	rad->hemires= 300;
	rad->convergence= 0.1;
	rad->radfac= 30.0;
	rad->gamma= 2.0;
	rad->drawtype= DTSOLID;
	rad->subshootp= 1;
	rad->subshoote= 2;
	rad->maxsublamp= 0;
	
	rad->pama= 500;
	rad->pami= 200;
	rad->elma= 100;
	rad->elmi= 20;
	rad->nodelim= 0;
	rad->maxnode= 10000;
	rad->flag= 2;
	set_radglobal();
}

void delete_radio()
{
	freeAllRad();
	if(G.scene->radio) freeN(G.scene->radio);
	G.scene->radio= 0;

	RG.radio= 0;
}

int rad_go(void)	/* return 0 when user escapes */
{
	int time, cputime, retval;
	/* firsttime moet vervangen worden door radphase? */
	
	if(RG.totface) return 0;

	start_timer();	/* calc duration */
	bgntimer();		/* for esc */
	G.afbreek= 0;
	
	set_radglobal();
	initradiosity();	/* LUT's */
	inithemiwindows();	/* views */
	
	maxsizePatches();

	setnodelimit(RG.patchmin);
	RG.phase= RAD_SHOOTP;
	subdivideshootPatches(RG.subshootp);

	setnodelimit(RG.elemmin);
	RG.phase= RAD_SHOOTE;
	subdivideshootElements(RG.subshoote);

	setnodelimit(RG.patchmin);
	subdividelamps();

	setnodelimit(RG.elemmin);

	RG.phase= RAD_SOLVE;
	subdiv_elements();

	progressiverad();

	removeEqualNodes(RG.nodelim);

	make_face_tab();	/* nu geankerd */

	closehemiwindows();
	RG.phase= 0;

	endtimer();
	end_timer(&time, &cputime);
	printf("Radiosity solving time: %d\n", cputime);

	if(G.afbreek==1) retval= 1;
	else retval= 0;
	
	G.afbreek= 0;
	
	return retval;
}

void rad_subdivshootpatch()
{
	
	if(RG.totface) return;

	bgntimer();
	G.afbreek= 0;

	set_radglobal();
	initradiosity();	/* LUT's */
	inithemiwindows();	/* views */
	
	subdivideshootPatches(1);

	removeEqualNodes(RG.nodelim);
	closehemiwindows();
	endtimer();
	
	allqueue(REDRAWVIEW3D, 1);
}

void rad_subdivshootelem()
{
	
	if(RG.totface) return;

	bgntimer();
	G.afbreek= 0;

	set_radglobal();
	initradiosity();	/* LUT's */
	inithemiwindows();	/* views */
	
	subdivideshootElements(1);

	removeEqualNodes(RG.nodelim);
	closehemiwindows();
	endtimer();
	
	allqueue(REDRAWVIEW3D, 1);
}

void rad_limit_subdivide()
{

	if(G.scene->radio==0) return;

	set_radglobal();

	if(RG.totpatch==0) {
		/* printf("exit: no relevant data\n"); */
		return;
	}
	
	maxsizePatches();

	init_face_tab();	/* free faces */
}

