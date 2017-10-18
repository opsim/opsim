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


/*  sector.c      MIXED MODEL
 * 
 *  aug 97		  SILICON VERSIE
 *  
 * 
 */

#ifndef FREE

#include "blender.h"
#include "graphics.h"
#include "sector.h"
#include "psx.h"
#include "blendpsx.h"

extern Object *main_actor;

float visifac[2]= {0.7, 0.3};
float distfac[2]= {10.0, 5.0};

void evaluate_camera_network(Object *set, short mode);

	/* regel voor FUZZY getallen: hoe hoger hoe beter */
	
	/* capo->visi */
#define	CA_INVALID		0
#define	CA_ALWAYS_TEST	1
#define CA_TEST_OUTWARD	2
#define	CA_OK			3
	
	/* capo->direction */
#define CA_OUTWARD		0
#define CA_MIDDLE		1
#define CA_INWARD		2

	/* capo->dist */
#define CA_TOTAL		0
#define CA_MEDIUM		1
#define CA_CLOSE		2

	/* capo->d_axis */
/* #define CA_INVALID		0 */
#define CA_SMALL		1
/* #define CA_OK			3 */

	/* capo->view */
#define CA_SIDE		0
#define CA_FRONT	1
#define CA_BACK		2

	/* capo->flag */
#define CA_SETPOS		1


/* ********************************** */


Sector *find_sector_lay(float *vec, float *local, uint lay)			/* algemene find */
{
	Base *base;
		
	base= FIRSTBASE;
	while(base) {
		if(base->lay & lay) {
			if(base->object->type==OB_SECTOR) {
				
				if(sector_inside(base->object->data, vec, local)) {
					return base->object->data;
				}
			}
		}
		base= base->next;
	}
	return 0;
}



/* ************* CAMERA NETWORK ************** */


void add_camera_to_sector(Sector *se, Object *ob)
{
	/* in stappen van 8 malloccen */
	CamPos *capo;
	Camera *cam;
	
	if(se==0) return;
	
	if((se->totcam & 7)==0) {
		capo= callocN( (se->totcam+8)*sizeof(CamPos), "campos");
		if(se->totcam) {
			memcpy(capo, se->campos, sizeof(CamPos)*se->totcam);
			freeN(se->campos);
		}
		se->campos= capo;
	}
	else capo= se->campos;
	
	capo+= se->totcam;
	se->totcam++;

	capo->ob= ob;
	cam= ob->data;
	
	Mat4Ortho(ob->obmat);
	Mat4Invert(ob->imat, ob->obmat);	/* viewmat */
	Mat4CpyMat4(capo->piramat, ob->imat);
	view_to_piramat(capo->piramat, cam->lens, cam->clipend);
		
	VECCOPY(capo->loc, ob->loc);
	VECCOPY(capo->rot, ob->rot);
	
	capo->sta= cam->netsta;
	capo->end= cam->netend;
	capo->clipsta= cam->clipsta;
	capo->clipend= cam->clipend;
	capo->lens= cam->lens;
	capo->hold= cam->hold;
}

short netlink_flag(Object *to, Object *from, Sector *se)
{
	CamPos *capo;
	short a, fl=0;
	
	/* zit object to ook in deze sector? */
	capo= se->campos;
	for(a=0; a<se->totcam; a++, capo++) {
		if(capo->ob==to) fl |= NL_BOTH;
	}

	if(fl & NL_BOTH) {
		if( strcmp(to->id.name, from->id.name) < 0 ) fl |= NL_BACKWARD;
	}

	return fl;
}

short netlink_sfra(Object *to, Object *from, Sector *se)
{
	CamPos *capo;
	NetLink *nl;
	short a;
	
	/* van to naar from is backwards: vind de sfra */
	
	capo= se->campos;
	for(a=0; a<se->totcam; a++, capo++) {
		if(capo->ob == to) {
			nl= capo->ob->network.first;
			while(nl) {
				if((nl->flag & NL_BACKWARD)==0) {
					if(nl->ob == from) return nl->sfra - 1 + nl->len/2;
				}
				nl= nl->next;
			}
		}
	}
	
	return 0;
}

void next_camera(int prev)
{
	Base *base;
	Sector *se, *cursec;
	Camera *cam;
	CamPos *capo;
	float tempf[3];
	int a;
	
	if(G.scene->camera==0) return;
	if(G.vd->persp!=2) return;
	
	sector_calc_imat();	/* anders na het renderen: cursec niet vinden */
	
	/* in de sector van de huidige camera: andere vinden */
	cursec= find_sector_lay(G.scene->camera->obmat[3], tempf, G.scene->camera->lay);
	if(cursec==0) {
		error("Camera not in a sector (layer?)");
		return;
	}
	
	base= FIRSTBASE;
	while(base) {
		if(base->lay & G.scene->lay) {
			if(base->object->type==OB_SECTOR) {
				Mat4Invert(base->object->imat, base->object->obmat);
			}
		}
		base= base->next;
	}
				
	/* alle camera's aflopen, sectortest, aan sector-array toevoegen */
	base= FIRSTBASE;
	while(base) {
	
		if(base->lay & G.scene->lay) {
			if(base->object->type == OB_CAMERA) {
				cam= base->object->data;
				if(cam->flag & CAM_NETWORK) {
					se= find_sector_lay(base->object->obmat[3], tempf, base->lay);
					if(se && se==cursec) add_camera_to_sector(se, base->object);	
				}
			}
		}
		base= base->next;
	}

	/* gaat ie */
	if(cursec->totcam) {
		
		capo= cursec->campos;
		for(a=0; a<cursec->totcam; a++, capo++) {
			capo->ob->flag &= ~SELECT;
			if(capo->ob==G.scene->camera) break;
		}

		if(a==cursec->totcam) a= 0;
		
		if(prev==0) {	/* next camera */
			if(a==cursec->totcam-1) a= 0;
			else a++;
		}	
		else {			/* prev camera */
			if(a==0) a= cursec->totcam-1;
			else a--;
		}

		capo= cursec->campos+a;
		
		G.scene->camera= G.vd->camera= capo->ob;
		capo->ob->flag |= SELECT;
		copy_objectflags();
		
		if((G.f & (G_FACESELECT|G_VERTEXPAINT))==0) {
			set_active_object(capo->ob);
			handle_view3d_lock();
		}
		
		freeN(cursec->campos);
		cursec->campos= 0;
		cursec->totcam= 0;

		allqueue(REDRAWVIEW3D, 0);
		
	}
	
}

void init_camera_network()
{
	Life *lf;
	Base *base;
	Sector *se;
	CamPos *capo;
	CamFrame *cf;
	NetLink *nl;
	Camera *cam, *camnl;
	float t, fac, mfac, tempf[3];
	int a, cfie;
	
	
	/* alle camera's aflopen, sectortest, aan sector-array toevoegen */
	base= FIRSTBASE;
	while(base) {
	
		if(base->lay & G.scene->lay) {
			if(base->object->type == OB_CAMERA) {
				cam= base->object->data;
				if(cam->flag & CAM_NETWORK) {
					G.simulf |= G_NETWORK;
					
					se= find_sector_lay(base->object->obmat[3], tempf, base->lay);
					add_camera_to_sector(se, base->object);
					
				}
			}
		}
		base= base->next;
	}

	/* sectors aflopen en camera's tellen */
	if(G.simulf & G_NETWORK) {
		printf("Camera network on\n");
		
		base= FIRSTBASE;
		while(base) {
			if(base->lay & G.scene->lay) {
				if(base->object->type == OB_SECTOR) {
					
					se= base->object->data;
					se->totfra= 0;
					capo= se->campos;
					for(a=0; a<se->totcam; a++, capo++) {

						capo->sfra= se->totfra;	/* frame in 'movie' */
						se->totfra++;
						
						nl= capo->ob->network.first;
						while(nl) {
							nl->flag = netlink_flag(nl->ob, capo->ob, se);
							if((nl->flag & NL_BACKWARD)==0) {
								nl->sfra= se->totfra;
								se->totfra+= nl->len/2;
							}
							nl= nl->next;
						}
					}
					
					/* backwardse startframes vinden */
					capo= se->campos;
					for(a=0; a<se->totcam; a++, capo++) {
						nl= capo->ob->network.first;
						while(nl) {
							if(nl->flag & NL_BACKWARD) {
								nl->sfra= netlink_sfra(nl->ob, capo->ob, se);
							}
							nl= nl->next;
						}
					}
					
					/* CamFrame array aanleggen */

					if(se->totfra) {
						cf=se->camframe= callocN( se->totfra*sizeof(CamFrame), "camframe");
					}
					
					capo= se->campos;
					for(a=0; a<se->totcam; a++, capo++) {
						
						
						VECCOPY(cf->loc, capo->ob->loc);
						VECCOPY(cf->rot, capo->ob->rot);
						cam= capo->ob->data;
						cf->lens= cam->lens;
						cf->clipsta= cam->clipsta;
						cf->clipend= cam->clipend;
						
						cf++;
						
						nl= capo->ob->network.first;
						while(nl) {
							if((nl->flag & NL_BACKWARD)==0) {
								for(cfie=0; cfie<nl->len; cfie+=2, cf++) {
									t= cfie+2;
									t/= (float)(nl->len+2);
									
									fac= t*t;
									fac= 3.0*fac - 2.0*t*fac;
									mfac= 1.0-fac;
									
									cf->loc[0]= mfac*capo->loc[0] + fac*nl->ob->loc[0];
									cf->loc[1]= mfac*capo->loc[1] + fac*nl->ob->loc[1];
									cf->loc[2]= mfac*capo->loc[2] + fac*nl->ob->loc[2];
							
									cf->rot[0]= mfac*capo->rot[0] + fac*nl->ob->rot[0];
									cf->rot[1]= mfac*capo->rot[1] + fac*nl->ob->rot[1];
									cf->rot[2]= mfac*capo->rot[2] + fac*nl->ob->rot[2];
									
									camnl= nl->ob->data;
									cf->lens= mfac*capo->lens+ fac*camnl->lens;
									cf->clipsta= mfac*cam->clipsta+ fac*camnl->clipsta;
									cf->clipend= mfac*cam->clipend+ fac*camnl->clipend;
								}
							}
							nl= nl->next;
						}
					}
					
				}
			}
			base= base->next;
		}

		evaluate_camera_network(0, -1); /*init */
	}
	
	

}

void end_camera_network()
{
	CamPos *capo;
	Sector *se;
	Base *base;
	Camera *cam;
	int a;

	base= FIRSTBASE;
	while(base) {
		if(base->lay & G.scene->lay) {
			if(base->object->type == OB_SECTOR) {
				se= base->object->data;
				capo= se->campos;
				for(a=0; a<se->totcam; a++, capo++) {

					/* restore */
					
					VECCOPY(capo->ob->loc, capo->loc);
					VECCOPY(capo->ob->rot, capo->rot);
					if(capo->ob->type==OB_CAMERA) {
						cam= capo->ob->data;
						
						cam->lens= capo->lens;
						
						cam->netsta= capo->sta;
						cam->netend= capo->end;
						cam->clipsta= capo->clipsta;
						cam->clipend= capo->clipend;
					}
					
				}
				if(se->campos) freeN(se->campos);
				if(se->camframe) freeN(se->camframe);
				se->campos= 0;
				se->camframe= 0;
				se->totcam= 0;
			}
		}
		base= base->next;
	}

	G.simulf &= ~G_NETWORK;
}

void set_netclipsta(CamPos *capo)
{
	Camera *cam= G.scene->camera->data;
	
	if(capo==0 || capo->ob==0) return;
	cam->netsta= capo->sta;
	cam->netend= capo->end;
			
}

void sector_frame_to_camera(Sector *se)
{
	CamFrame *cf;
	Camera *cam;
	Object *ob= G.scene->camera;
	
	if(ob==0 || se==0 || se->camframe==0) return;
	
	cf= se->camframe + se->cfra;

	
	VECCOPY(ob->loc, cf->loc);
	VECCOPY(ob->rot, cf->rot);
	
	cam= ob->data;
	cam->lens= cf->lens;
	cam->clipsta= cf->clipsta;
	cam->clipend= cf->clipend;
	
}

void init_render_camera_network()
{
	static int isdone= 0;
	Base *base;
	Sector *se;
	short sfra, efra;

	/* backgroundrender afhandelen */
	if(G.background) {
		if(isdone) return;
		isdone= 1;
	}

	sfra= 1;
	efra= 0;
	
	G.simulf= 0;
	init_sectors();	/* doet ook mesh_isdone flag */
	init_camera_network();
	
	/* EFRA berekenen */
	G.cursector= 0;
	base= FIRSTBASE;
	while(base) {
		if(base->lay & G.scene->lay) {
			if(base->object->type == OB_SECTOR) {
				se= base->object->data;
				if(se->flag & SE_RENDER_NETWORK) {
					if(se->campos) {
						se->sfra= efra + 1;
						efra+= se->totfra;
					}
				}
			}
		}
		base= base->next;
	}

	if(efra == 0) efra= 1;
	
	if(G.background==0) {
		EFRA= efra;
		SFRA= sfra;
	}
}

void end_render_camera_network()
{

	if(G.background) return;
	
	end_camera_network();	/* staat ook in exit_blender */
	end_sectors();
	G.cursector= 0;
}


void render_frame_to_camera()
{
	Base *base;
	Sector *se= 0;
	int sfra=1;
	
	/* is init_camera_net gedaan? */
	if((G.simulf & G_NETWORK)==0) return;
	
	/* welke sector */
	G.cursector= 0;
	base= FIRSTBASE;
	while(base) {
		if(base->lay & G.scene->lay) {
			if(base->object->type == OB_SECTOR) {
				se= base->object->data;
				if(se->flag & SE_RENDER_NETWORK) {
					if(se->totfra) {
						if(CFRA < se->sfra+se->totfra) break;
						sfra += se->totfra;
					}
				}
				se= 0;
			}
		}
		base= base->next;
	}

	if(se) {
		G.cursector= se;
		G.cursector->cfra= CFRA-sfra;
		sector_frame_to_camera(G.cursector);
	}
}

void render_frame_to_info(float *loc, float *rot, float *lens)
{
	CamFrame *cf;
	
	render_frame_to_camera();
	
	if(G.cursector) {
		cf= G.cursector->camframe + G.cursector->cfra;
	
		*lens= cf->lens;
		VECCOPY(loc, cf->loc);
		VECCOPY(rot, cf->rot);
	}
}

void evaluate_camera(CamPos *capo)
{
	float hoco[3], sx, sy, inp;
	float min, max;
	short fl;
	
	VECCOPY(hoco, main_actor->obmat[3]);

	Mat4MulVecfl(capo->piramat, hoco);
	max= (hoco[2]);
	min= -max;
	
	fl= 0;
	if(hoco[0] < min) fl+= 1; else if(hoco[0] > max) fl+= 2;
	if(hoco[1] < min) fl+= 4; else if(hoco[1] > max) fl+= 8;
	if(hoco[2] < capo->sta) fl+= 16; else if(hoco[2] > capo->end) fl+= 32;

	if(fl) {
		capo->visi= CA_INVALID;
		capo->dura= 0;
		return;
	}
	
	/* visibility */
	hoco[0]= (hoco[0]/min);
	hoco[1]= (hoco[1]/min);
	max= MAX2( fabs(hoco[0]), fabs(hoco[1]));
	if(max < visifac[1]) capo->visi= CA_OK;
	else if(max < visifac[0]) capo->visi= CA_TEST_OUTWARD;
	else capo->visi= CA_ALWAYS_TEST;
	
	/* dist: 'echte' coords */
	
	min= capo->sta + 0.2*(capo->end - capo->sta);
	max= capo->sta + 0.8*(capo->end - capo->sta);
	
	if(hoco[2] > max) capo->dist= CA_TOTAL;
	else if(hoco[2] > min) capo->dist= CA_MEDIUM;
	else capo->dist= CA_CLOSE;
	
	/* internal direction (Y-as) */
	sx= hoco[0];
	sy= hoco[1];
	VECCOPY(hoco, main_actor->obmat[1]);
	Mat4Mul3Vecfl(capo->piramat, hoco);
		/* eenheden van 256: ivm inprodukt test */
	capo->axis[0]= (256.0*hoco[0]);
	capo->axis[1]= (256.0*hoco[1]);
	
	if(capo->visi==CA_OK) capo->direction= CA_MIDDLE;
	else {
		inp= sx*hoco[0]+sy*hoco[1];
		if(inp<0.1) capo->direction= CA_OUTWARD;
		else if(inp>0.1) capo->direction= CA_INWARD;
		else capo->direction= CA_MIDDLE;
	}
	
	if(hoco[2] < -0.1) capo->view= CA_FRONT;
	else if(hoco[2] > 0.1) capo->view= CA_BACK;
	else capo->view= CA_SIDE;
}

CamPos *find_best_campos1(CamPos *curcapo)		/* leuk voor stills */
{
	CamPos *capo, *bestcapo= 0;
	int a, round= 0;

	while(bestcapo==0) {

		capo= G.cursector->campos;
		for(a=0; a<G.cursector->totcam; a++, capo++) {
			
			if(capo->visi==CA_INVALID) continue;
			if(capo->d_axis==CA_INVALID) continue;	/* alleen 1e ronde */
			
			if(curcapo->visi==CA_OK) {			/* alleen closere met juiste as */
			
				if(bestcapo==0) bestcapo= curcapo;
				
				if(capo->visi==CA_OK) {
	
					if(capo->dist>curcapo->dist) {
						if(bestcapo==0 || bestcapo->dist<capo->dist) bestcapo= capo;
					}
				}
			}
			else if(curcapo->visi==CA_TEST_OUTWARD) {	/* alleen inward accepteren */
				
				if(bestcapo==0) bestcapo= curcapo;
				
				if(capo->visi==CA_OK) {
		
					if(capo->dist>curcapo->dist) {
						if(bestcapo==0 || bestcapo->dist<capo->dist) bestcapo= capo;
					}
	
				}
				else if(curcapo->direction==CA_OUTWARD) {
					if(capo->visi>=CA_TEST_OUTWARD) {
						if(capo->direction==CA_INWARD) {
							/* beste dist */
							if(bestcapo==0 || bestcapo->dist<capo->dist) bestcapo= capo;
						}
					}
				}
			}
			else if(curcapo->visi==CA_ALWAYS_TEST) {	/* betere visi accepteren */
				
				if(capo->visi>CA_ALWAYS_TEST) {
					/* dist vergelijken met bestcapo */
					if(bestcapo==0 || bestcapo->dist<capo->dist) bestcapo= capo;
				}
			}
			else {		/* cur visi==0: alles accepteren */
			
				if(bestcapo==0 || capo->visi>bestcapo->visi) bestcapo= capo;
				
			}
			
			if(capo->visi && (capo->ob->flag & 1)) {
				if(get_qual()&LR_SHIFTKEY) PRINT4(d, d, d, d, round, capo->visi, capo->dist, capo->d_axis);
			}
		}
		
		if(bestcapo==0) {
			if(round==0) {
				capo= G.cursector->campos;
				for(a=0; a<G.cursector->totcam; a++, capo++) capo->d_axis= CA_SMALL;
			}
			else break;
		}

		round++;
	}

}

CamPos *find_best_campos2(CamPos *curcapo)		/* aktie */
{
	CamPos *capo, *bestcapo= 0;
	int a, round= 0;

	/* 1e ronde: alleen VISI / D_AXIS camera's
	 * 2e ronde: alle VISI
	 */

	while(bestcapo==0) {

		if(curcapo->visi==CA_OK) bestcapo= curcapo;		/* stabiliseert */
		if(round==1 && curcapo->visi)  bestcapo= curcapo;	/* wanhopig */

		capo= G.cursector->campos;
		for(a=0; a<G.cursector->totcam; a++, capo++) {
			
			if(capo->visi==CA_INVALID) continue;
			if(capo->d_axis==CA_INVALID) continue;		/* alleen 1e ronde */
				
			if(curcapo->visi==0 || curcapo->direction==CA_OUTWARD) {	/* altijd betere vinden */
				if(capo->direction!=CA_OUTWARD) {

					/* beste visi */
					if(bestcapo==0 || bestcapo->visi<capo->visi) bestcapo= capo;
					/* beste dist */
					if(bestcapo==0 || bestcapo->dist<capo->dist) bestcapo= capo;
				}
			}
			
		}
		
		if(bestcapo==0) {
			if(round==0) {
				capo= G.cursector->campos;
				for(a=0; a<G.cursector->totcam; a++, capo++) capo->d_axis= CA_SMALL;
			}
			else if(round==1) {
			
				/* noodgreep! */
				capo= G.cursector->campos;
				for(a=0; a<G.cursector->totcam; a++, capo++)
					if(capo->visi) return capo;
			}
			else break;
		}

		round++;
	}

	return bestcapo;
}

CamPos *find_best_campos3(CamPos *curcapo)		/* aktie + back */
{
	CamPos *capo, *bestcapo= 0;
	short a, round= 0;

	if(curcapo->visi && curcapo->view==CA_BACK) bestcapo= curcapo;

	capo= G.cursector->campos;
	for(a=0; a<G.cursector->totcam; a++, capo++) {
		
		if(capo->visi==CA_INVALID) continue;
		if(capo->view!=CA_BACK) continue;
		
		/* als we hier zijn: capo is back+visi */
		if(bestcapo==0) bestcapo= capo;
		
		/* beste axis */
		if(bestcapo->direction==0 && bestcapo->direction < capo->direction) bestcapo= capo;
		
		/* beste dist */
		if(capo->direction==CA_INWARD) {
			if(bestcapo->dist < capo->dist) bestcapo= capo;
			
			/* beste visi */
			if(capo->dist==CA_CLOSE) {
				if(bestcapo->visi < capo->visi) bestcapo= capo;
			}
		}
	}
	return bestcapo;
}


Action *playmovie= 0;

void evaluate_camera_network(Object *set, short mode)
{
	static CamPos *curcapo, *bestcapo;
	CamPos *capo;
	Life *lf;
	int inp;
	short a, newsec=0;
	
	if(main_actor==0) return;
	
	if(mode== -1) {				/*  init */
		curcapo= bestcapo= 0;
		playmovie= 0;
		return;
	}

	lf= main_actor->data;
	if(lf->sector != G.cursector) {
		curcapo= bestcapo= 0;
		G.cursector= lf->sector;
	}


	if(G.cursector==0 || G.cursector->totcam==0) return;

	if(set) {
		capo= G.cursector->campos;
		for(a=0; a<G.cursector->totcam; a++, capo++) {
			if(capo->ob==set) {
				bestcapo= capo;
				bestcapo->flag |= CA_SETPOS;
				if(curcapo) curcapo->dura= 0;
				bestcapo->dura= bestcapo->hold;
				break;
			}
		}
	}

	if(G.fields==0) return;

	if(playmovie) {
		Object *ob;
		float ctime;
		
		if(playmovie->poin==0 || playmovie->cur>= playmovie->end) {
			playmovie= 0;
		}
		else {
			ob= playmovie->poin;
			playmovie->cur+= G.fields;
			ctime= (float)(0.5*playmovie->cur);
			
			where_is_object_time(ob, ctime);
			
			VECCOPY(G.scene->camera->loc, ob->loc);
			VECCOPY(G.scene->camera->rot, ob->rot);
			
			return;
		}
	}

	if(curcapo && curcapo->nl) {	/* link aan't afspelen */
		
		curcapo->cfie+= G.fields;   /* rekenen in fields */

		if( curcapo->cfie >= curcapo->nl->len) {

			curcapo->nl= 0;
			curcapo= bestcapo;
			G.cursector->cfra= curcapo->sfra;
		}
		else {
			if(curcapo->nl->flag & NL_BACKWARD) 
				G.cursector->cfra= curcapo->nl->sfra - (curcapo->cfie/2);
			else G.cursector->cfra= curcapo->nl->sfra + (curcapo->cfie/2);
			
			sector_frame_to_camera(G.cursector);
			
			return;
		}
	}

	if(curcapo==0) {		/* Of de eerste Of de huidige camera, NIET bestcapo */
		capo=curcapo= G.cursector->campos;
		if(curcapo==0) return;
		
		for(a=0; a<G.cursector->totcam; a++, capo++) {
			if(bestcapo==capo);
			else if(capo->ob==G.scene->camera) curcapo= capo;
		}
		curcapo->dura= 0;
		newsec= 1;
	}

	/* alle camera's in huidige sector aflopen PLUS huidige camera */
	capo= G.cursector->campos;
	for(a=0; a<G.cursector->totcam; a++, capo++) {
		if(capo!=curcapo) evaluate_camera(capo);
	}
	evaluate_camera(curcapo);

	/* de as regel */
	capo= G.cursector->campos;
	for(a=0; a<G.cursector->totcam; a++, capo++) {
		
		inp= curcapo->axis[0]*capo->axis[0] + curcapo->axis[1]*capo->axis[1];
		if(inp<-100) capo->d_axis= CA_INVALID;
		else if(inp<10000) capo->d_axis= CA_SMALL;
		else capo->d_axis= CA_OK;
		
		if(capo->ob->flag & 1) {
			if(get_qual()&LR_SHIFTKEY) {
				PRINT4(d, d, d, d, capo->visi, capo->dist, capo->view, capo->direction);
			}
		}

	}
	
	/* de conservatieve regel */
	if(curcapo->dura>0) curcapo->dura-= G.fields;

	if(curcapo->dura<=0) {
		
		/* de beste */	
		if(bestcapo && (bestcapo->flag & CA_SETPOS)) bestcapo->flag &= ~CA_SETPOS;
		else {
			bestcapo= find_best_campos3(curcapo);
			if(bestcapo==0) bestcapo= find_best_campos2(curcapo);	/* tzt vervangen */
		}

		if(bestcapo) {
			
			if(bestcapo!=curcapo) {
				NetLink *nl= curcapo->ob->network.first;
	
				if(newsec) nl= 0;
				
				/* is er een netlink? */
				while(nl) {
					if(nl->ob==bestcapo->ob) break;
					nl= nl->next;
				}
			
				if(nl) {
					curcapo->nl= nl;
					curcapo->cfie= 0;
					G.cursector->cfra= nl->sfra;	/*  frames  */
				}
				else {
					G.cursector->cfra= bestcapo->sfra;	/*  frames */
					curcapo= bestcapo;
				}
				
				bestcapo->dura= bestcapo->hold;
			}
			else G.cursector->cfra= bestcapo->sfra;	/*  frames */
		}
	}
	
	/* if(get_qual()&LR_SHIFTKEY) PRINT2(x, d, G.cursector, G.cursector->cfra); */

	sector_frame_to_camera(G.cursector);
	set_netclipsta(curcapo);
	
}


void add_networklink()
{
	Object *obact;
	Base *base;
	static short len= 25;
	short mode;
	NetLink *nl;

	obact= OBACT;
	if(obact==0) return;
	mode= pupmenu("NetWork %t|add link %x1| remove link %x2");
	
	if(mode<1) return;
	
	if(mode==1) button(&len, 2, 250, "Frames");
	
	/* leg links van de ACTIEVE naar de GESELECTEERDE en terug */
	
	base= FIRSTBASE;
	while(base) {
		if TESTBASE(base) {
			if(base->object!=obact) {
				
				/* 1: van obact naar ob */
				nl= obact->network.first;
				while(nl) {
					if(nl->ob==base->object) break;
					nl= nl->next;
				}
				
				if(mode==1) {				/* add */
					if(nl==0) {
						nl= callocN(sizeof(NetLink), "Netlink");
						addtail(&obact->network, nl);
					}
					nl->ob= base->object;
					nl->len= 2*len;		/* fields */
					
				}
				else if(mode==2 && nl) {	/* del */
					remlink(&obact->network, nl);
					freeN(nl);
				}
				
				/* 2: van ob naar obact */
				nl= base->object->network.first;
				while(nl) {
					if(nl->ob==obact) break;
					nl= nl->next;
				}
				
				if(mode==1) {				/* add */
					if(nl==0) {
						nl= callocN(sizeof(NetLink), "Netlink");
						addtail(&base->object->network, nl);
					}
					nl->ob= obact;
					nl->len= 2*len;	/* fields */
					
				}
				else if(mode==2 && nl) {	/* del */
					remlink(&base->object->network, nl);
					freeN(nl);
				}
			}
		}
		base= base->next;
	}
	allqueue(REDRAWVIEW3D, 0);
}

#endif

