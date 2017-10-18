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


/*  envmap.c        RENDER
 * 
 *  may 1999
 * 
 */

#include "blender.h"
#include "render.h"


EnvMap *add_envmap()
{
	EnvMap *env;
	
	env= callocN(sizeof(EnvMap), "envmap");
	env->type= ENV_CUBE;
	env->stype= ENV_STATIC;
	env->clipsta= 0.1;
	env->clipend= 100.0;
	env->cuberes= 100;
	
	return env;
}

EnvMap *copy_envmap(EnvMap *env)
{
	EnvMap *envn;
	int a;
	
	envn= dupallocN(env);
	envn->ok= 0;
	for(a=0; a<6; a++) envn->cube[a]= 0;
	if(envn->ima) id_us_plus((ID *)envn->ima);
	
	return envn;
}

void free_envmapdata(EnvMap *env)
{
	Image *ima;
	int a, part;
	
	for(part=0; part<6; part++) {
		ima= env->cube[part];
		if(ima) {
			if(ima->ibuf) freeImBuf(ima->ibuf);

			for(a=0; a<MAXMIPMAP; a++) {
				if(ima->mipmap[a]) freeImBuf(ima->mipmap[a]);
			}
			freeN(ima);
			env->cube[part]= 0;
		}
	}
	env->ok= 0;
}

void free_envmap(EnvMap *env)
{
	
	free_envmapdata(env);
	freeN(env);
	
}

void save_envmap(EnvMap *env, char *str)
{
	ImBuf *ibuf;
	extern rectcpy();
	int dx;
	
	/* all interactive stuff is handled in buttons.c */
	
	dx= env->cuberes;
	ibuf= allocImBuf(3*dx, 2*dx, 24, IB_rect, 0);
	
	rectop(ibuf, env->cube[0]->ibuf, 
			0, 0, 0, 0, dx, dx, rectcpy, 0);
	rectop(ibuf, env->cube[1]->ibuf, 
			dx, 0, 0, 0, dx, dx, rectcpy, 0);
	rectop(ibuf, env->cube[2]->ibuf, 
			2*dx, 0, 0, 0, dx, dx, rectcpy, 0);
	rectop(ibuf, env->cube[3]->ibuf, 
			0, dx, 0, 0, dx, dx, rectcpy, 0);
	rectop(ibuf, env->cube[4]->ibuf, 
			dx, dx, 0, 0, dx, dx, rectcpy, 0);
	rectop(ibuf, env->cube[5]->ibuf, 
			2*dx, dx, 0, 0, dx, dx, rectcpy, 0);
	
	write_ibuf(ibuf, str);
	freeImBuf(ibuf);
}

void envmap_split_ima(EnvMap *env)
{
	ImBuf *ibuf;
	Image *ima;
	extern rectcpy();
	int dx, part;
	
	free_envmapdata(env);	
	
	dx= env->ima->ibuf->y;
	dx/= 2;
	if(3*dx != env->ima->ibuf->x) {
		error("Incorrect envmap size");
		env->ok= 0;
		env->ima->ok= 0;
	}
	else {
		for(part=0; part<6; part++) {
			ibuf= allocImBuf(dx, dx, 24, IB_rect, 0);
			ima= callocN(sizeof(Image), "image");
			ima->ibuf= ibuf;
			ima->ok= 1;
			env->cube[part]= ima;
		}
		rectop(env->cube[0]->ibuf, env->ima->ibuf, 
			0, 0, 0, 0, dx, dx, rectcpy, 0);
		rectop(env->cube[1]->ibuf, env->ima->ibuf, 
			0, 0, dx, 0, dx, dx, rectcpy, 0);
		rectop(env->cube[2]->ibuf, env->ima->ibuf, 
			0, 0, 2*dx, 0, dx, dx, rectcpy, 0);
		rectop(env->cube[3]->ibuf, env->ima->ibuf, 
			0, 0, 0, dx, dx, dx, rectcpy, 0);
		rectop(env->cube[4]->ibuf, env->ima->ibuf, 
			0, 0, dx, dx, dx, dx, rectcpy, 0);
		rectop(env->cube[5]->ibuf, env->ima->ibuf, 
			0, 0, 2*dx, dx, dx, dx, rectcpy, 0);
		env->ok= 2;
	}
}

/* ****************** RENDER ********************** */

void envmap_renderdata(EnvMap *env)
{
	static Render envR;
	static Object *camera;
	
	if(env) {
		envR= R;
		camera= G.scene->camera;
		
		env->cuberes &= 0xFFFC;
		R.rectx= R.r.xsch= R.recty= R.r.ysch= env->cuberes;
		R.afmx= R.afmy= R.r.xsch/2;
		R.xstart= R.ystart= -R.afmx;
		R.xend= R.yend= R.xstart+R.rectx-1;

		R.r.mode &= ~(R_BORDER | R_PANORAMA | R_ORTHO | R_MBLUR);
		R.r.xparts= R.r.yparts= 1;
		R.r.bufflag= 0;
		R.r.size= 100;
		R.ycor= 1.0; 
		R.r.yasp= R.r.xasp= 1;
		
		R.near= env->clipsta;
		R.far= env->clipend;
		
		G.scene->camera= env->object;
		
	}
	else {
		/* this to make sure init_renderdisplay works */
		envR.winx= R.winx;
		envR.winy= R.winy;
		envR.winxof= R.winxof;
		envR.winyof= R.winyof;
		
		R= envR;
		G.scene->camera= camera;
	}
	
}

void envmap_transmatrix(float *mat, int part)
{
	float tmat[4][4], eul[3], rotmat[4][4];
	
	eul[0]= eul[1]= eul[2]= 0.0;
	
	if(part==0) {			/* neg z */
		;
	} else if(part==1) {	/* pos z */
		eul[0]= M_PI;
	} else if(part==2) {	/* pos y */
		eul[0]= M_PI/2.0;
	} else if(part==3) {	/* neg x */
		eul[0]= M_PI/2.0;
		eul[2]= M_PI/2.0;
	} else if(part==4) {	/* neg y */
		eul[0]= M_PI/2.0;
		eul[2]= M_PI;
	} else {				/* pos x */
		eul[0]= M_PI/2.0;
		eul[2]= -M_PI/2.0;
	}
	Mat4CpyMat4(tmat, mat);
	EulToMat4(eul, rotmat);
	Mat4MulSerie(mat, tmat, rotmat, 0);
}

void env_rotate_scene(float *mat, int mode)
{
	VlakRen *vlr;
	VertRen *ver;
	LampRen *lar;
	HaloRen *har;
	float xn, yn, zn, imat[3][3], pmat[4][4], smat[4][4], tmat[4][4];
	int a;
	
	if(mode==0) {
		Mat4Invert(tmat, mat);
		Mat3CpyMat4(imat, tmat);
	}
	else {
		Mat4CpyMat4(tmat, mat);
		Mat3CpyMat4(imat, mat);
	}
	
	for(a=0; a<R.totvert; a++) {
		if((a & 255)==0) ver= R.blove[a>>8];
		else ver++;
		
		Mat4MulVecfl(tmat, ver->co);
		
		xn= ver->n[0];
		yn= ver->n[1];
		zn= ver->n[2];
		/* geen transpose ! */
		ver->n[0]= imat[0][0]*xn+imat[1][0]*yn+imat[2][0]*zn;
		ver->n[1]= imat[0][1]*xn+imat[1][1]*yn+imat[2][1]*zn;
		ver->n[2]= imat[0][2]*xn+imat[1][2]*yn+imat[2][2]*zn;
		Normalise(ver->n);
	}
	
	for(a=0; a<R.tothalo; a++) {
		if((a & 255)==0) har= R.bloha[a>>8];
		else har++;
	
		Mat4MulVecfl(tmat, har->co);
	}
		
	for(a=0; a<R.totvlak; a++) {
		if((a & 255)==0) vlr= R.blovl[a>>8];
		else vlr++;
		
		xn= vlr->n[0];
		yn= vlr->n[1];
		zn= vlr->n[2];
		/* geen transpose ! */
		vlr->n[0]= imat[0][0]*xn+imat[1][0]*yn+imat[2][0]*zn;
		vlr->n[1]= imat[0][1]*xn+imat[1][1]*yn+imat[2][1]*zn;
		vlr->n[2]= imat[0][2]*xn+imat[1][2]*yn+imat[2][2]*zn;
		Normalise(vlr->n);
	}
	
	set_normalflags();
	
	for(a=0; a<R.totlamp; a++) {
		lar= R.la[a];
		Mat3CpyMat3(smat, lar->imat);
		Mat3MulMat3(lar->imat, smat, imat);
		Mat3MulVecfl(imat, lar->vec);
		Mat4MulVecfl(tmat, lar->co);

		lar->sh_invcampos[0]= -lar->co[0];
		lar->sh_invcampos[1]= -lar->co[1];
		lar->sh_invcampos[2]= -lar->co[2];
		Mat3MulVecfl(lar->imat, lar->sh_invcampos);
		lar->sh_invcampos[2]*= lar->sh_zfac;
		
		if(lar->shb) {
			if(mode==1) {
				Mat4Invert(pmat, mat);
				Mat4MulMat4(smat, pmat, lar->shb->viewmat);
				Mat4MulMat4(lar->shb->persmat, smat, lar->shb->winmat);
			}
			else Mat4MulMat4(lar->shb->persmat, lar->shb->viewmat, lar->shb->winmat);
		}
	}
	
}

void env_layerflags(uint notlay)
{
	VlakRen *vlr;
	int a;
	
	for(a=0; a<R.totvlak; a++) {
		if((a & 255)==0) vlr= R.blovl[a>>8];
		else vlr++;
		if(vlr->lay & notlay) vlr->flag &= ~R_VISIBLE;
	}
}

void env_set_imats()
{
	Base *base;
	float mat[4][4];
	
	base= FIRSTBASE;
	while(base) {
		Mat4MulMat4(mat, base->object->obmat, R.viewmat);
		Mat4Invert(base->object->imat, mat);
		
		base= base->next;
	}

}	

void render_envmap(EnvMap *env)
{
	/* only the cubemap is implemented */
	ImBuf *ibuf;
	Image *ima;
	float oldwinmat[4][4], oldviewinv[4][4], mat[4][4], tmat[4][4];
	short part;
	
	/* need a recalc: ortho-render has no correct viewinv */
	Mat4Invert(oldviewinv, R.viewmat);

	/* setup necessary globals */
	envmap_renderdata(env);
	
	init_render_display();

	R.rectot= mallocN(sizeof(int)*R.rectx*R.recty, "rectot");
	R.rectz=  mallocN(sizeof(int)*R.rectx*R.recty, "rectz");

	for(part=0; part<6; part++) {

		clear_render_display();
		fillrect(R.rectot, R.rectx, R.recty, 0);
		
		setwindowclip(1,-1); /*  geen jit:(-1) */
		
		Mat4CpyMat4(tmat, G.scene->camera->obmat);
		Mat4Ortho(tmat);
		envmap_transmatrix(tmat[0], part);
		Mat4Invert(mat, tmat);
		/* mat now is the camera 'viewmat' */

		Mat4CpyMat4(R.viewmat, mat);
		Mat4CpyMat4(R.viewinv, tmat);
		
		/* we have to correct for the already rotated vertexcoords */
		Mat4MulMat4(tmat, oldviewinv, R.viewmat);
		Mat4Invert(env->imat, tmat);
		
		env_rotate_scene(tmat[0], 1);
		init_render_world();
		setzbufvlaggen(projectverto);
		env_layerflags(env->notlay);
		env_set_imats();
				
		if(test_break()==0) {

			printrenderinfo(part);

			if(R.r.mode & R_OSA) zbufshadeDA();
			else zbufshade();

		}
		
		/* rotate back */
		env_rotate_scene(tmat[0], 0);

		if(test_break()==0) {
			ibuf= allocImBuf(R.rectx, R.recty, 24, IB_rect, 0);
			ima= callocN(sizeof(Image), "image");
			memcpy(ibuf->rect, R.rectot, 4*ibuf->x*ibuf->y);
			ima->ibuf= ibuf;
			ima->ok= 1;
			env->cube[part]= ima;
		}
		
		if(test_break()) break;

	}
	
	if(R.rectz) freeN(R.rectz); R.rectz= 0;
	if(R.rectot) freeN(R.rectot); R.rectot= 0;
	
	if(test_break()) free_envmapdata(env);
	else {
		if(R.r.mode & R_OSA) env->ok= ENV_OSA;
		else env->ok= ENV_NORMAL;
		env->lastframe= CFRA;
	}
	
	/* restore */
	envmap_renderdata(0);
	env_set_imats();
	init_render_world();

}

void make_envmaps()
{
	Tex *tex;
	int do_init= 0;
	
	tex= G.main->tex.first;
	while(tex) {
		if(tex->id.us && tex->type==TEX_ENVMAP) {
			if(tex->env && tex->env->object) {
				if(tex->env->object->lay & G.scene->lay) {
					if(tex->env->stype!=ENV_LOAD) {
						
						if(tex->env->ok==0) {
							do_init= 1;
							render_envmap(tex->env);
						}
						else if((R.r.mode & R_OSA) && tex->env->ok==ENV_NORMAL) {
							do_init= 1;
							free_envmapdata(tex->env);
							render_envmap(tex->env);
						}
					}
				}
			}
		}
		tex= tex->id.next;
	}

	if(do_init) {
		init_render_display();
		clear_render_display();
		allqueue(REDRAWBUTSTEX, 0);
	}
}

int envcube_isect(float *vec, float *answ)
{
	float labda;
	int face;
	
	/* which face */
	if( vec[2]<=-fabs(vec[0]) && vec[2]<=-fabs(vec[1]) ) {
		face= 0;
		labda= -1.0/vec[2];
		answ[0]= labda*vec[0];
		answ[1]= labda*vec[1];
	}
	else if( vec[2]>=fabs(vec[0]) && vec[2]>=fabs(vec[1]) ) {
		face= 1;
		labda= 1.0/vec[2];
		answ[0]= labda*vec[0];
		answ[1]= -labda*vec[1];
	}
	else if( vec[1]>=fabs(vec[0]) ) {
		face= 2;
		labda= 1.0/vec[1];
		answ[0]= labda*vec[0];
		answ[1]= labda*vec[2];
	}
	else if( vec[0]<=-fabs(vec[1]) ) {
		face= 3;
		labda= -1.0/vec[0];
		answ[0]= labda*vec[1];
		answ[1]= labda*vec[2];
	}
	else if( vec[1]<=-fabs(vec[0]) ) {
		face= 4;
		labda= -1.0/vec[1];
		answ[0]= -labda*vec[0];
		answ[1]= labda*vec[2];
	}
	else {
		face= 5;
		labda= 1.0/vec[0];
		answ[0]= -labda*vec[1];
		answ[1]= labda*vec[2];
	}
	answ[0]= 0.5+0.5*answ[0];
	answ[1]= 0.5+0.5*answ[1];
	return face;
}

static void set_dxtdyt(float *dxts, float *dyts, float *dxt, float *dyt, int face)
{
	if(face==2 || face==4) {
		dxts[0]= dxt[0];
		dyts[0]= dyt[0];
		dxts[1]= dxt[2];
		dyts[1]= dyt[2];
	}
	else if(face==3 || face==5) {
		dxts[0]= dxt[1];
		dxts[1]= dxt[2];
		dyts[0]= dyt[1];
		dyts[1]= dyt[2];
	}
	else {
		dxts[0]= dxt[0];
		dyts[0]= dyt[0];
		dxts[1]= dxt[1];
		dyts[1]= dyt[1];
	}
}

int envmaptex(Tex *tex, float *texvec, float *dxt, float *dyt)
{
	/* texvec should be the already reflected normal */
	extern float Tin, Ta, Tr, Tg, Tb;
	EnvMap *env;
	float fac, vec[3], sco[3], col[20], dxts[3], dyts[3];
	int face, face1;
	
	env= tex->env;
	if(env==0 || env->object==0) return 0;
	
	if(env->stype==ENV_LOAD) {
		env->ima= tex->ima;
		if(env->ima && env->ima->ok) {
			if(env->ima->ibuf==0) ima_ibuf_is_nul(tex);
			if(env->ima->ok && env->ok==0) envmap_split_ima(env);
		}
	}

	if(env->ok==0) {
		
		Tin= 0.0;
		return 0;
	}
	
	/* rotate to envmap space */
	VECCOPY(vec, texvec);
	Mat4Mul3Vecfl(env->object->imat, vec);
	
	face= envcube_isect(vec, sco);
	tex->ima= env->cube[face];
	
	if(R.osatex) {
		Mat4Mul3Vecfl(env->object->imat, dxt);
		Mat4Mul3Vecfl(env->object->imat, dyt);
		
		set_dxtdyt(dxts, dyts, dxt, dyt, face);
		imagewraposa(tex, sco, dxts, dyts);
		
		/* edges? */
		
		if(Ta<1.0) {
			col[0]= Ta; col[1]= Tr; col[2]= Tg; col[3]= Tb;
	
			VecAddf(vec, vec, dxt);
			face1= envcube_isect(vec, sco);
			VecSubf(vec, vec, dxt);
			
			if(face!=face1) {
				tex->ima= env->cube[face1];
				set_dxtdyt(dxts, dyts, dxt, dyt, face1);
				imagewraposa(tex, sco, dxts, dyts);
				col[4]= Ta; col[5]= Tr; col[6]= Tg; col[7]= Tb;
			}
			else col[4]= col[5]= col[6]= col[7]= 0.0;
			
			/* here was the nasty bug! col[5,6,7] were not zero-ed. FPE! */
			
			VecAddf(vec, vec, dyt);
			face1= envcube_isect(vec, sco);
			VecSubf(vec, vec, dyt);
			
			if(face!=face1) {
				tex->ima= env->cube[face1];
				set_dxtdyt(dxts, dyts, dxt, dyt, face1);
				imagewraposa(tex, sco, dxts, dyts);
				col[8]= Ta; col[9]= Tr; col[10]= Tg; col[11]= Tb;
			}
			else col[8]= col[9]= col[10]= col[11]= 0.0;
			
			fac= (col[0]+col[4]+col[8]);
			fac= 1.0/fac;
			
			Tr= fac*(col[0]*col[1] + col[4]*col[5] + col[8]*col[9] );
			Tg= fac*(col[0]*col[2] + col[4]*col[6] + col[8]*col[10] );
			Tb= fac*(col[0]*col[3] + col[4]*col[7] + col[8]*col[11] );
			Ta= 1.0;

		}
	}
	else {
		imagewrap(tex, sco);
	}
	
	tex->ima= env->ima;
	
	return 1;
}


