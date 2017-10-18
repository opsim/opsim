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


/*  font.c      MIXED MODEL
 * 
 *  maart 95
 *  
 * 
 */

#include "blender.h"
#include "objfnt.h"
#include "datatoc.h"
#include "file.h"

#define MAX_VF_CHARS 256

/* Nieuwe opzet voor vectorfont:
 *
 * geen PDrawfont meer, alles meteen naar Nurb en BezTriple
 *
 */

struct chartrans {
	float xof, yof;
	float rot;
	short linenr,charnr;
};

void free_vfont(struct VFont *vf)
{
	Nurb *nu;
	BezTriple *bezt;
	int i;

	if (vf == 0) return;

	for (i = 0; i < MAX_VF_CHARS; i++){
		while (nu = vf->data->nurbsbase[i].first){
			bezt = nu->bezt;
			if (bezt) freeN(bezt);
			remlink(&vf->data->nurbsbase[i], nu);
			freeN(nu);
		}
	}

	if (vf->data->kern) freeN(vf->data->kern);
	freeN(vf->data);

}


int objfnt_to_vfont(struct objfnt *fnt, VFont *vfont)
{
	VFontData *vfd;
	chardesc *cd;
	short *_data, *data;
	int a, i, count, stop, ready, meet;
	short first[2], last[2];
	struct Nurb *nu;
	struct BezTriple *bezt, *bez2;
	float scale, dx, dy;


	if (fnt == 0) return (0);

	if (fnt->type != SP_TYPE){
		printf("objfnt is wrong type: need splines\n");
		return (0);
	}
	scale = 10.0/(float)fnt->scale;	/* na IRIX 6.2, schaal klopte niet meer */
	vfd= vfont->data;
	
	for (i = 0; i < MAX_VF_CHARS; i++) {
		cd = getchardesc(fnt, i);
		if (cd && cd->data && cd->datalen) {
			vfd->width[i] = scale * cd->movex;

			_data = data = cd->data;

			do{
				/* eerst even tellen */
				_data = data;
				count = 0;
				ready = stop = FALSE;

				do{
					switch(*data++){
					case SP_MOVETO:
						first[0] = data[0];
						first[1] = data[1];
					case SP_LINETO:
						count++;
						last[0] = data[0];
						last[1] = data[1];
						data += 2;
						break;
					case SP_CURVETO:
						count++;
						last[0] = data[4];
						last[1] = data[5];
						data += 6;
						break;
					case SP_RET:
					case SP_RETCLOSEPATH:
						stop = TRUE;
						ready = TRUE;
						break;
					case SP_CLOSEPATH:
						stop = TRUE;
						break;
					}
				} while (stop == FALSE);

				if (last[0] == first[0] && last[1] == first[1]) meet = 1;
				else meet = FALSE;

				/* is er meer dan 1 uniek punt ?*/

				if (count - meet > 0) {
					data = _data;

					nu = callocstructN(struct Nurb,1, "objfnt_nurb");
					bezt = callocstructN(struct BezTriple, count, "objfnt_bezt");
					if (nu != 0 && bezt != 0) {
						addtail(&vfd->nurbsbase[i], nu);
						nu->type= CU_BEZIER+CU_2D;
						nu->pntsu = count;
						nu->resolu= 8;
						nu->flagu= 1;
						nu->bezt = bezt;
						stop = FALSE;

						/* punten inlezen */
						do {
							switch(*data++){
							case SP_MOVETO:
								bezt->vec[1][0] = scale * *data++;
								bezt->vec[1][1] = scale * *data++;
								
								break;
							case SP_LINETO:
								bez2 = bezt++;
								bezt->vec[1][0] = scale * *data++;
								bezt->vec[1][1] = scale * *data++;
								/* vector handles */
								bezt->h1= HD_VECT;
								bez2->h2= HD_VECT;
								dx = (bezt->vec[1][0] - bez2->vec[1][0]) / 3.0;
								dy = (bezt->vec[1][1] - bez2->vec[1][1]) / 3.0;
								bezt->vec[0][0] = bezt->vec[1][0] - dx;
								bezt->vec[0][1] = bezt->vec[1][1] - dy;
								bez2->vec[2][0] = bez2->vec[1][0] + dx;
								bez2->vec[2][1] = bez2->vec[1][1] + dy;
								break;
								
							case SP_CURVETO:
								bezt->vec[2][0] = scale * *data++;
								bezt->vec[2][1] = scale * *data++;
								bezt->h2= HD_ALIGN;
								bezt++;
								bezt->vec[0][0] = scale * *data++;
								bezt->vec[0][1] = scale * *data++;
								bezt->vec[1][0] = scale * *data++;
								bezt->vec[1][1] = scale * *data++;
								bezt->h1= HD_ALIGN;
								break;
								
							case SP_RET:
							case SP_RETCLOSEPATH:
								stop = TRUE;
								ready = TRUE;
								break;
							case SP_CLOSEPATH:
								stop = TRUE;
								break;
							}
						} while (stop == FALSE);

						if (meet) {
							/* kopieer handles */
							nu->bezt->vec[0][0] = bezt->vec[0][0];
							nu->bezt->vec[0][1] = bezt->vec[0][1];
							/* en vergeet laatste punt */
							nu->pntsu--;
						}
						else {
							/* vector handles */
							bez2 = nu->bezt;
							dx = (bezt->vec[1][0] - bez2->vec[1][0]) / 3.0;
							dy = (bezt->vec[1][1] - bez2->vec[1][1]) / 3.0;
							bezt->vec[2][0] = bezt->vec[1][0] - dx;
							bezt->vec[2][1] = bezt->vec[1][1] - dy;
							bez2->vec[0][0] = bez2->vec[1][0] + dx;
							bez2->vec[0][1] = bez2->vec[1][1] + dy;
							bezt->h2= bez2->h1= HD_VECT;
						}
						
						/* verboden handle combinaties */
						a= nu->pntsu;
						bezt= nu->bezt;
						while(a--) {
							if(bezt->h1!=HD_ALIGN && bezt->h2==HD_ALIGN) bezt->h2= 0;
							else if(bezt->h2!=HD_ALIGN && bezt->h1==HD_ALIGN) bezt->h1= 0;
							bezt++;
						}
						
					}
					else {
						if (nu) freeN(nu);
						if (bezt) freeN(bezt);
					}
				}
				_data = data;
			} while (ready == FALSE);
		}
	}

	return(1);
}

VFont *load_vfont(char *name)
{
	struct objfnt *fnt;
	struct VFont *vfont=0;
	extern struct objfnt * loadpostcriptfont(char *);
	int ok;
	char di[FILE_MAXDIR], fi[FILE_MAXFILE];
		
	waitcursor(1);

	fnt = loadpostcriptfont(name);
	if (fnt) {
	
		strcpy(di, name);
		splitdirstring(di, fi);

		vfont = alloc_libblock(&G.main->vfont, ID_VF, fi);
		vfont->data= callocN(sizeof(VFontData), "Vfontdata");
		
		ok= objfnt_to_vfont(fnt, vfont);
		freeobjfnt(fnt);

		if(ok) {
			strncpy(vfont->name, name, sizeof(vfont->name));
			vfont->flag= 0;		/* loaded */
		}
		else {
			free_libblock_us(&G.main->vfont, vfont);
		}
	}
	waitcursor(0);
	
	return(vfont);
}

void load_firstfont()
{
	VFont *vf;
	char *dir, str[256];
	
	dir= gethome();
	strcpy(str, dir);
	strcat(str, "/.Bfont");
	
	vf= load_vfont(str);
	if(vf==0) {
		inst_file(".Bfont", datatoc_Bfont, datatoc_Bfont_size);
		vf= load_vfont(str);
		if(vf==0) {
			printf("Unable to load $HOME/.Bfont\n");
			exit(1);
		}
	}
	
	vf->id.us= 0;
}

void reload_vfont(VFont *vfont)		/* komt uit readile */
{
	struct objfnt *fnt;
	extern struct objfnt * loadpostcriptfont(char *);

	fnt = loadpostcriptfont(vfont->name);
	if (fnt) {
		objfnt_to_vfont(fnt, vfont);
		freeobjfnt(fnt);
	}
	
	vfont->flag= 0;		/* loaded */
	
}


void buildchar(Curve *cu, uchar ascii, float ofsx, float ofsy, float rot)
{
	BezTriple *bezt1, *bezt2;
	Nurb *nu1, *nu2;
	float *fp, fsize, shear, x, si, co;
	int i;

	/* maak een kopie op afstand ofsx, ofsy met shear*/
	fsize= cu->fsize;
	shear= cu->shear;
	si= fsin(rot);
	co= fcos(rot);
	
	nu1 = cu->vfont->data->nurbsbase[ascii].first;
	while(nu1)
	{
		bezt1 = nu1->bezt;
		if (bezt1){
			nu2 = mallocstructN(struct Nurb,1, "duplichar_nurb");
			if (nu2 == 0) break;
			memcpy(nu2, nu1, sizeof(struct Nurb));
			nu2->resolu= cu->resolu;
			nu2->bp = 0;
			nu2->knotsu = nu2->knotsv = 0;
			nu2->flag= ME_SMOOTH;
			/* nu2->trim.first = 0; */
			/* nu2->trim.last = 0; */
			i = nu2->pntsu;

			bezt2= mallocstructN(struct BezTriple, i, "duplichar_bezt2");
			if (bezt2 == 0){
				freeN(nu2);
				break;
			}
			memcpy(bezt2, bezt1, i * sizeof(struct BezTriple));
			nu2->bezt = bezt2;
			
			if (shear != 0.0) {
				bezt2 = nu2->bezt;
				
				for (i= nu2->pntsu; i > 0; i--) {
					bezt2->vec[0][0] += shear * bezt2->vec[0][1];
					bezt2->vec[1][0] += shear * bezt2->vec[1][1];
					bezt2->vec[2][0] += shear * bezt2->vec[2][1];
					bezt2++;
				}
			}
			if(rot!=0.0) {
				bezt2= nu2->bezt;
				for (i=nu2->pntsu; i > 0; i--) {
					fp= bezt2->vec[0];
					
					x= fp[0];
					fp[0]= co*x + si*fp[1];
					fp[1]= -si*x + co*fp[1];
					x= fp[3];
					fp[3]= co*x + si*fp[4];
					fp[4]= -si*x + co*fp[4];
					x= fp[6];
					fp[6]= co*x + si*fp[7];
					fp[7]= -si*x + co*fp[7];

					bezt2++;
				}
			}
			bezt2 = nu2->bezt;
			
			for (i= nu2->pntsu; i > 0; i--) {
				fp= bezt2->vec[0];

				fp[0]= (fp[0]+ofsx)*fsize;
				fp[1]= (fp[1]+ofsy)*fsize;
				fp[3]= (fp[3]+ofsx)*fsize;
				fp[4]= (fp[4]+ofsy)*fsize;
				fp[6]= (fp[6]+ofsx)*fsize;
				fp[7]= (fp[7]+ofsy)*fsize;
				bezt2++;
			}
			
			addtail(&(cu->nurb), nu2);
		}
		nu1 = nu1->next;
	}
}


struct chartrans *text_to_curve(Object *ob, int mode) 
{
	VFont *vfont;
	Curve *cu, *cucu;
	struct chartrans *chartransdata, *ct;
	float distfac, tabfac, ctime, dtime, tvec[4], vec[4], rotvec[3], minx, maxx, miny, maxy;
	float cmat[3][3], timeofs, si, co, sizefac;
	float *f, maxlen=0, xof, yof, xtrax, linedist, *ld, *linedata, *linedata2;
	int len, i, slen, oldflag;
	short *kern, kcount, str, cnr=0, lnr=0;
	uchar ascii,*mem;

	/* opmerking: berekeningen altijd tot en met de '\0' van de string omdat
	   de cursor op die plek moet kunnen staan */

	if(ob->type!=OB_FONT) return 0;

	cu= ob->data;

	vfont= cu->vfont;
	if (vfont==0) return 0;
	if (cu->str==0) return 0;

	/* aantal regels tellen */
	mem= cu->str;
	slen = strlen(mem);
	cu->lines= 1;
	for (i= 0; i<=slen; i++, mem++) {
		ascii = *mem;
		if(ascii== '\n' || ascii== '\r') cu->lines++;
	}

	/* bereken ofset en rotatie van iedere letter */
	ct=chartransdata= callocstructN(struct chartrans, slen+1, "buildtext");
	linedata= mallocN(sizeof(float)*cu->lines,"buildtext2");
	linedata2= mallocN(sizeof(float)*cu->lines,"buildtext2");
	xof= cu->xof;
	yof= cu->yof;

	xtrax= 0.5*cu->spacing-0.5;
	linedist= cu->linedist;

	for (i = 0 ; i<=slen ; i++) {
		ascii = cu->str[i];
		if(ascii== '\n' || ascii== '\r' || ascii==0) {
			ct->xof= xof;
			ct->yof= yof;
			ct->linenr= lnr;
			ct->charnr= cnr;
			
			/* alleen lege regels mogen kleiner dan 1 zijn */
			if( linedist<1.0) {
				if(i<slen && (cu->str[i+1]=='\r' || cu->str[i+1]=='\n')) yof-= linedist;
				else yof-= 1.0;
			}
			else yof-= linedist;
			
			maxlen= MAX2(maxlen, xof);
			linedata[lnr]= xof;
			linedata2[lnr]= cnr;
			xof= cu->xof;
			lnr++;
			cnr= 0;
		}
		else if(ascii==9) {	/* TAB */
			ct->xof= xof;
			ct->yof= yof;
			ct->linenr= lnr;
			ct->charnr= cnr++;

			tabfac= (xof-cu->xof+0.01);
			tabfac= 2.0*fceil(tabfac/2.0);
			xof= cu->xof+tabfac;
		}
		else {
			ct->xof= xof;
			ct->yof= yof;
			ct->linenr= lnr;
			ct->charnr= cnr++;

			xof += vfont->data->width[ascii] + xtrax;

			if(vfont->data->kern) {
				kern = vfont->data->kern;
				str = (ascii << 8) + cu->str[i+1];
				for (kcount = vfont->data->kcount ; kcount >0 ; kcount--) {
					if (*kern++ == str) {
						xof += *kern;
						break;
					}
					kern++;
				}
			}
		}
		ct++;
	}

	/* met alle fontsettings plekken letters berekenen */
	if(cu->spacemode!=CU_LEFT && lnr>1) {
		ct= chartransdata;

		if(cu->spacemode==CU_RIGHT) {
			for(i=0;i<lnr;i++) linedata[i]= maxlen-linedata[i];
			for (i=0; i<=slen; i++) {
				ct->xof+= linedata[ct->linenr];
				ct++;
			}
		} else if(cu->spacemode==CU_MIDDLE) {
			for(i=0;i<lnr;i++) linedata[i]= (maxlen-linedata[i])/2;
			for (i=0; i<=slen; i++) {
				ct->xof+= linedata[ct->linenr];
				ct++;
			}
		} else if(cu->spacemode==CU_FLUSH) {
			for(i=0;i<lnr;i++)
				if(linedata2[i]>1)
					linedata[i]= (maxlen-linedata[i])/(linedata2[i]-1);
			for (i=0; i<=slen; i++) {
				ct->xof+= ct->charnr*linedata[ct->linenr];
				ct++;
			}
		}
	}
	
	/* TEXT ON CURVE */
	if(cu->textoncurve) {
		cucu= cu->textoncurve->data;
		
		oldflag= cucu->flag;
		cucu->flag |= (CU_PATH+CU_FOLLOW);
		
		if(cucu->path==0) calc_curvepath(cu->textoncurve);
		if(cucu->path) {
			

			Mat3CpyMat4(cmat, cu->textoncurve->obmat);
			sizefac= Normalise(cmat[0])/cu->fsize;
			
			minx=miny= 1.0e20;
			maxx=maxy= -1.0e20;
			ct= chartransdata;
			for (i=0; i<=slen; i++, ct++) {
				if(minx>ct->xof) minx= ct->xof;
				if(maxx<ct->xof) maxx= ct->xof;
				if(miny>ct->yof) miny= ct->yof;
				if(maxy<ct->yof) maxy= ct->yof;
			}
			
			/* we zetten de x-coordinaat exact op de curve, de y wordt geroteerd */
			
			/* de lengte correctie */
			distfac= sizefac*cucu->path->totdist/(maxx-minx);
			timeofs= 0.0;
			
			if(distfac > 1.0) {
				/* pad langer dan tekst: spacemode doet mee */
				distfac= 1.0/distfac;
				
				if(cu->spacemode==CU_RIGHT) {
					timeofs= 1.0-distfac;
				}
				else if(cu->spacemode==CU_MIDDLE) {
					timeofs= (1.0-distfac)/2.0;
				}
				else if(cu->spacemode==CU_FLUSH) distfac= 1.0;
				
			}
			else distfac= 1.0;
			
			distfac/= (maxx-minx);
			
			timeofs+= distfac*cu->xof;	/* niet cyclic */
			
			ct= chartransdata;
			for (i=0; i<=slen; i++, ct++) {
				
				/* roteren rond centrum letter */
				ascii = cu->str[i];
				dtime= distfac*0.35*vfont->data->width[ascii];	/* Waarom is 0.5 te groot? */
				dtime= distfac*0.0*vfont->data->width[ascii];	/* Waarom is 0.5 te groot? */
				
				ctime= timeofs + distfac*( ct->xof - minx);
				CLAMP(ctime, 0.0, 1.0);
				
				/* de goede plek EN de goede rotatie apart berekenen */
				where_on_path(cu->textoncurve, ctime, vec, tvec);
				where_on_path(cu->textoncurve, ctime+dtime, tvec, rotvec);
				
				VecMulf(vec, sizefac);
				
				ct->rot= M_PI-fatan2(rotvec[1], rotvec[0]);

				si= fsin(ct->rot);
				co= fcos(ct->rot);

				yof= ct->yof;
				
				ct->xof= vec[0] + si*yof;
				ct->yof= vec[1] + co*yof;
				
			}
			cucu->flag= oldflag;
		}
	}


	if(mode==FO_CURSUP || mode==FO_CURSDOWN) {
		/* 2: curs omhoog
		   3: curs omlaag */
		ct= chartransdata+cu->pos;
		
		if(mode==FO_CURSUP && ct->linenr==0);
		else if(mode==FO_CURSDOWN && ct->linenr==lnr);
		else {
			if(mode==FO_CURSUP) lnr= ct->linenr-1;
			else lnr= ct->linenr+1;
			cnr= ct->charnr;
			/* zoek karakter met lnr en cnr */
			cu->pos= 0;
			ct= chartransdata;
			for (i= 0; i<slen; i++) {
				if(ct->linenr==lnr) {
					if(ct->charnr==cnr) break;
					if( (ct+1)->charnr==0) break;
				}
				else if(ct->linenr>lnr) break;
				cu->pos++;
				ct++;
			}
		}
	}
	
	/* eerst cursor */
	if(ob==G.obedit) {
		ct= chartransdata+cu->pos;
		si= fsin(ct->rot);
		co= fcos(ct->rot);
				
		f= G.textcurs[0];
		
		f[0]= cu->fsize*(-0.1*co + ct->xof);
		f[1]= cu->fsize*(0.1*si + ct->yof);
		
		f[2]= cu->fsize*(0.1*co + ct->xof);
		f[3]= cu->fsize*(-0.1*si + ct->yof);
		
		f[4]= cu->fsize*( 0.1*co + 0.8*si + ct->xof);
		f[5]= cu->fsize*(-0.1*si + 0.8*co + ct->yof);
		
		f[6]= cu->fsize*(-0.1*co + 0.8*si + ct->xof);
		f[7]= cu->fsize*( 0.1*si + 0.8*co + ct->yof);
		
	}

	if(mode==0) {
		/* nurbdata maken */
		
		freeNurblist(&cu->nurb);
		
		ct= chartransdata;
		for (i= 0; i<slen; i++) {
			ascii = cu->str[i];
			buildchar(cu, ascii, ct->xof, ct->yof, ct->rot);
			ct++;
		}
	}

	freeN(linedata);
	freeN(linedata2);

	if(mode==FO_DUPLI) {
		return chartransdata;
	}

	freeN(chartransdata);
	return 0;
}


/* ***************** DUPLI  ***************** */

Object *find_family_object(Object **obar, char *family, char ch)
{
	Object *ob;
	int flen;
	
	if( obar[ch] ) return obar[ch];
	
	flen= strlen(family);
	
	ob= G.main->object.first;
	while(ob) {
		if( ob->id.name[flen+2]==ch ) {
			if( strncmp(ob->id.name+2, family, flen)==0 ) break;
		}
		ob= ob->id.next;
	}
	
	obar[ch]= ob;
	
	return ob;
}


void font_duplilist(Object *par)
{
	extern ListBase duplilist;
	Object *ob, *new, *obar[256];
	Curve *cu;
	struct chartrans *ct, *chartransdata;
	float vec[3], pmat[4][4], fsize, xof, yof;
	float *q2;
	int lay, slen, a;
	
	Mat4CpyMat4(pmat, par->obmat);

	lay= G.scene->lay;
	if(G.vd) lay= G.vd->lay;
	
	/* in par staat een familienaam, deze gebruiken om objecten te vinden */

	chartransdata= text_to_curve(par, FO_DUPLI);
	if(chartransdata==0) return;
	
	bzero(obar, 256*4);
	
	cu= par->data;
	slen= strlen(cu->str);
	fsize= cu->fsize;
	xof= cu->xof;
	yof= cu->yof;
	
	ct= chartransdata;
	set_displist_onlyzero(1);

	for(a=0; a<slen; a++, ct++) {
	
		ob= find_family_object(obar, cu->family, cu->str[a]);
		if(ob) {
			
			makeDispList(ob);
			
			vec[0]= fsize*(ct->xof - xof);
			vec[1]= fsize*(ct->yof - yof);
			vec[2]= 0.0;
	
			Mat4MulVecfl(pmat, vec);
			
			new= mallocN(sizeof(Object), "newobj dupli");
			memcpy(new, ob, sizeof(Object));
			new->flag |= OB_FROMDUPLI;
			new->id.new= (ID *)par;		/* duplicator bewaren */
			new->totcol= par->totcol;	/* voor give_current_material */
			
			Mat4CpyMat4(new->obmat, par->obmat);
			VECCOPY(new->obmat[3], vec);
			
			new->parent= 0;
			new->track= 0;
			
			addtail(&duplilist, new);
		}
		
	}
	set_displist_onlyzero(0);
	freeN(chartransdata);
}

