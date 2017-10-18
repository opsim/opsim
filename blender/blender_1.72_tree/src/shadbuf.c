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


/*  shadbuf.c        RENDER
 * 
 *  april 95
 * 
 */

#include "blender.h"
#include "render.h"

float jitshad[32][2];
short filt3[9]= { 1,2,1,2,4,2,1,2,1 };
static int bias= 0x00500000;

void lrectreadRectz(x1,y1,x2,y2,r1)	/* leest deel uit rectz in r1 */
int x1,y1,x2,y2;
char *r1;
{
	uint len4, *rz;	

	if(x1>=R.rectx || x2>=R.rectx || y1>=R.recty || y2>=R.recty) return;
	if(x1>x2 || y1>y2) return;

	len4= 4*(x2- x1+1);
	rz= R.rectz+R.rectx*y1+x1;
	for(;y1<=y2;y1++) {
		memcpy(r1,rz,len4);
		rz+= R.rectx;
		r1+= len4;
	}
}

void initshadowbuf(LampRen *lar, Lamp *la, float mat[][4])
{
	struct ShadBuf *shb;
	float hoek, dist, temp, viewinv[4][4];
	short v;

	/* if(la->spsi<16) return; */

	/* geheugen reserveren */
	shb= (struct ShadBuf *)callocN( sizeof(struct ShadBuf),"initshadbuf");
	lar->shb= shb;

	if(shb==0) return;

	VECCOPY(shb->co, lar->co);
	
	/* percentage: min en max in de gaten houden */
	shb->size= (la->bufsize*R.r.size)/100;
	if(shb->size<512) shb->size= 512;
	else if(shb->size > la->bufsize) shb->size= la->bufsize;
	
	shb->samp= la->samp;
	shb->soft= la->soft;
	shb->shadhalostep= la->shadhalostep;
	
	shb->zbuf= (ulong *)mallocN( sizeof(ulong)*(shb->size*shb->size)/256, "initshadbuf2");
	shb->cbuf= (char *)callocN( (shb->size*shb->size)/256, "initshadbuf3");

	if(shb->zbuf==0 || shb->cbuf==0) {
		if(shb->zbuf) freeN(shb->zbuf);
		freeN(lar->shb);
		lar->shb= 0;
		return;
	}

	Mat4Ortho(mat);
	Mat4Invert(shb->winmat, mat);	/* winmat is hier temp */

	/* matrix: combinatie van inverse view en lampmat */
	/* opnieuw berekenen: de ortho-render heeft geen correcte viewinv */
	Mat4Invert(viewinv, R.viewmat);
	Mat4MulMat4(shb->viewmat, viewinv, shb->winmat);

	/* projektie */
	hoek= safacos(lar->spotsi);
	temp= 0.5*shb->size*fcos(hoek)/fsin(hoek);
	shb->d= la->clipsta;

	shb->pixsize= (shb->d)/temp;

	shb->far= la->clipend;
	/* bias is percentage, 2x groter gemaakt ivm invalshoek correctie */
	shb->bias= (0.02*la->bias)*0x7FFFFFFF;
	shb->bias= shb->bias*(100/R.r.size);
	
}

int sizeoflampbuf(shb)
struct ShadBuf *shb;
{
	int num,count=0;
	char *cp;
	
	cp= shb->cbuf;
	num= (shb->size*shb->size)/256;

	while(num--) count+= *(cp++);
	
	return 256*count;
}

void makeshadowbuf(LampRen *lar)
{
	struct ShadBuf *shb= lar->shb;
	extern float panophi;
	void projectvert();
	struct VlakRen *vlr;
	float temp, wsize, wd, dist;
	float persmat[4][4], vec[3];
	int *rz, *rz1, verg, verg1;
	ulong *ztile;
	int a, x, y, xof, yof, win, v, minx, miny, byt1, byt2;
	short temprx,tempry, square;
	char *rc, *rcline, *ctile, *zt;

	/* viewvars onthouden */
	temprx= R.rectx; tempry= R.recty;
	R.rectx= R.recty= shb->size;

	initjit(shb->jit[0], shb->samp*shb->samp);

	/* matrices en window: in R.winmat komt transformatie
		van obsview naar lampview,  inclusief lampwinmat */
	
	wsize= shb->pixsize*(shb->size/2.0);

	i_window(-wsize, wsize, -wsize, wsize, shb->d, shb->far, shb->winmat);

	Mat4MulMat4(shb->persmat, shb->viewmat, shb->winmat);
	
	/* temp, will be restored */
	Mat4SwapMat4(shb->persmat, R.winmat);

	/* zbufferen */
	if(R.rectz) freeN(R.rectz);
 	R.rectz= (uint *)mallocN(sizeof(int)*shb->size*shb->size,"makeshadbuf");
	rcline= mallocN(256*4+sizeof(int),"makeshadbuf2");

	/* onthouden: panorama rot */
	temp= panophi;
	panophi= 0.0;

	setzbufvlaggen(projectvert);

	panophi= temp;
	
	zbuffershad(lar);

	/* alle pixels 1 x ingevuld verwijderen (oneven) */
	/* probleem hierbij kan geven dat er abrupte overgangen van zacht gebied
	 * naar geen zacht gebied is: bijv als eronder een klein vlakje zit
	 * DAAROM ER WEER UIT
	 * ook vanwege shadowhalo!
	 * 
		a= shb->size*shb->size;
		rz= R.rectz;
		while(a--) {
		    if(*rz & 1) *rz= 0x7FFFFFFF;
		    rz++;
		}
	 */
	
	square= lar->mode & LA_SQUARE;
	
	/* Z tiles aanmaken: dit systeem is 24 bits!!! */
	
	ztile= shb->zbuf;
	ctile= shb->cbuf;
	for(y=0; y<shb->size; y+=16) {
		if(y< shb->size/2) miny= y+15-shb->size/2;
		else miny= y-shb->size/2;	
				
		for(x=0; x<shb->size; x+=16) {

			/* ligt rechthoek binnen spotbundel? */
			a= shb->size/2;
			if(x< a) minx= x+15-a;
			else minx= x-a;	
			
			dist= fsqrt( (float)(minx*minx+miny*miny) );

			if(square==0 && dist>(float)(a+12)) {	/* 12, tested with a onlyshadow lamp */
				a= 256; verg= 0; /* 0x80000000; */ /* 0x7FFFFFFF; */
				rz1= (&verg)+1;
			} 
			else {
				lrectreadRectz(x, y, MIN2(shb->size-1,x+15), MIN2(shb->size-1,y+15), rcline);
				rz1= (int *)rcline;
				
				verg= (*rz1 & 0xFFFFFF00);

				for(a=0;a<256;a++,rz1++) {
					if( (*rz1 & 0xFFFFFF00) !=verg) break;
				}
			}
			if(a==256) { /* compleet leeg vakje */
				*ctile= 0;
				*ztile= *(rz1-1);
			}
			else {
				
				/* ACOMP enz. zijn defined L/B endian */
				
				rc= rcline;
				rz1= (int *)rcline;
				verg=  rc[ACOMP];
				verg1= rc[BCOMP];
				rc+= 4;
				byt1= 1; byt2= 1;
				for(a=1;a<256;a++,rc+=4) {
					byt1 &= (verg==rc[ACOMP]);
					byt2 &= (verg1==rc[BCOMP]);

					if(byt1==0) break;
				}
				if(byt1 && byt2) {	/* alleen byte opslaan */
					*ctile= 1;
					*ztile= (ulong)mallocN(256+4, "tile1");
					rz= (int *)*ztile;
					*rz= *rz1;

					zt= (char *)(rz+1);
					rc= rcline;
					for(a=0; a<256; a++, zt++, rc+=4) *zt= rc[GCOMP];	
				}
				else if(byt1) {		/* short opslaan */
					*ctile= 2;
					*ztile= (ulong)mallocN(2*256+4,"Tile2");
					rz= (int *)*ztile;
					*rz= *rz1;

					zt= (char *)(rz+1);
					rc= rcline;
					for(a=0; a<256; a++, zt+=2, rc+=4) {
						zt[0]= rc[BCOMP];
						zt[1]= rc[GCOMP];
					}
				}
				else {			/* triple opslaan */
					*ctile= 3;
					*ztile= (ulong)mallocN(3*256,"Tile3");

					zt= (char *)*ztile;
					rc= rcline;
					for(a=0; a<256; a++, zt+=3, rc+=4) {
						zt[0]= rc[ACOMP];
						zt[1]= rc[BCOMP];
						zt[2]= rc[GCOMP];
					}
				}
			}
			ztile++;
			ctile++;
		}
	}

	freeN(rcline);
	freeN(R.rectz); R.rectz= 0;

	R.rectx= temprx; R.recty= tempry;
	Mat4SwapMat4(shb->persmat, R.winmat);

	/* printf("lampbuf %d\n", sizeoflampbuf(shb)); */
}

int firstreadshadbuf(struct ShadBuf *shb, int xs, int ys, int nr)
{
	/* return 1 als volledig gecomprimeerde shadbuftile && z==const */
	static int *rz;
	int ofs;
	char *ct;

	/* always test borders of shadowbuffer */
	if(xs<0) xs= 0; else if(xs>=shb->size) xs= shb->size-1;
	if(ys<0) ys= 0; else if(ys>=shb->size) ys= shb->size-1;
   
	/* z berekenen */
	ofs= (ys>>4)*(shb->size>>4) + (xs>>4);
	ct= shb->cbuf+ofs;
	if(*ct==0) {
	    if(nr==0) {
			rz= *( (int **)(shb->zbuf+ofs) );
			return 1;
	    }
		else if(rz!= *( (int **)(shb->zbuf+ofs) )) return 0;
		
	    return 1;
	}
	
	return 0;
}

float readshadowbuf(struct ShadBuf *shb, int xs, int ys, int zs)	/* return 1.0 : volledig licht */
{
	float temp;
	int *rz, ofs;
	int zsamp;
	char *ct, *cz;

	/* simpleclip */
	/* if(xs<0 || ys<0) return 1.0; */
	/* if(xs>=shb->size || ys>=shb->size) return 1.0; */
	
	/* always test borders of shadowbuffer */
	if(xs<0) xs= 0; else if(xs>=shb->size) xs= shb->size-1;
	if(ys<0) ys= 0; else if(ys>=shb->size) ys= shb->size-1;

	/* z berekenen */
	ofs= (ys>>4)*(shb->size>>4) + (xs>>4);
	ct= shb->cbuf+ofs;
	rz= *( (int **)(shb->zbuf+ofs) );

	if(*ct==3) {
		ct= ((char *)rz)+3*16*(ys & 15)+3*(xs & 15);
		cz= (char *)&zsamp;
		cz[ACOMP]= ct[0];
		cz[BCOMP]= ct[1];
		cz[GCOMP]= ct[2];
	}
	else if(*ct==2) {
		ct= ((char *)rz);
		ct+= 4+2*16*(ys & 15)+2*(xs & 15);
		zsamp= *rz;
	
		cz= (char *)&zsamp;
		cz[BCOMP]= ct[0];
		cz[GCOMP]= ct[1];
	}
	else if(*ct==1) {
		ct= ((char *)rz);
		ct+= 4+16*(ys & 15)+(xs & 15);
		zsamp= *rz;

		cz= (char *)&zsamp;
		cz[GCOMP]= ct[0];

	}
	else {
		zsamp= (int) rz;
	}

	/* if(zsamp >= 0x7FFFFE00) return 1.0; */	/* geen schaduw als op oneindig wordt gesampeld*/

	if(zsamp > zs) return 1.0; 		/* absoluut geen schaduw */
	else if( zsamp < zs-bias) return 0.0 ;	/* absoluut wel schaduw */
	else {					/* zacht gebied */

		temp=  ( (float)(zs- zsamp) )/(float)bias;
		return 1.0 - temp*temp;
			
	}
}


float testshadowbuf(struct ShadBuf *shb, float inp)  	/* return 1.0: geen schaduw */
{
	float fac, co[4], dx[3], dy[3], vec[3], aantal=0;
	float xs1,ys1, siz, *j, xres, yres;
	int xs,ys, zs;
	short a,num;

	/* if(inp <= 0.0) return 1.0; */

	/* renderco en osaco roteren */
	siz= 0.5*(float)shb->size;
	VECCOPY(co, R.co);
	co[3]= 1.0;

	Mat4MulVec4fl(shb->persmat, co);	/* rationele hom co */

	xs1= siz*(1.0+co[0]/co[3]);
	ys1= siz*(1.0+co[1]/co[3]);

	/* clip op z */
	fac= (co[2]/co[3]);

	if(fac>=1.0) return 0.0;
	else if(fac<= -1.0) return 1.0;

	zs= ((float)0x7FFFFFFF)*fac;

	/* num*num samples nemen, gebied met fac vergroten */
	num= shb->samp*shb->samp;
	fac= shb->soft;
	
	
	bias= (1.1-inp*inp)*shb->bias;

	if(num==1) return readshadowbuf(shb,(int)xs1, (int)ys1, zs);

	co[0]= R.co[0]+O.dxco[0];
	co[1]= R.co[1]+O.dxco[1];
	co[2]= R.co[2]+O.dxco[2];
	co[3]= 1.0;
	Mat4MulVec4fl(shb->persmat,co);	/* rationele hom co */
	dx[0]= xs1- siz*(1.0+co[0]/co[3]);
	dx[1]= ys1- siz*(1.0+co[1]/co[3]);

	co[0]= R.co[0]+O.dyco[0];
	co[1]= R.co[1]+O.dyco[1];
	co[2]= R.co[2]+O.dyco[2];
	co[3]= 1.0;
	Mat4MulVec4fl(shb->persmat,co);	/* rationele hom co */
	dy[0]= xs1- siz*(1.0+co[0]/co[3]);
	dy[1]= ys1- siz*(1.0+co[1]/co[3]);

	xres= fac*( fabs(dx[0])+fabs(dy[0]) );
	yres= fac*( fabs(dx[1])+fabs(dy[1]) );

	if(xres<fac) xres= fac;
	if(yres<fac) yres= fac;
	
	xs1-= (xres)/2;
	ys1-= (yres)/2;

	j= shb->jit[0];

	if(xres<16.0 && yres<16.0) {
	    if(firstreadshadbuf(shb, (int)xs1, (int)ys1, 0)) {
		if(firstreadshadbuf(shb, (int)(xs1+xres), (int)ys1, 1)) {
		    if(firstreadshadbuf(shb, (int)xs1, (int)(ys1+yres), 1)) {
			if(firstreadshadbuf(shb, (int)(xs1+xres), (int)(ys1+yres), 1)) {
			    return readshadowbuf(shb,(int)xs1, (int)ys1, zs);
			}
		    }
		}
	    }
	}

	for(a=num;a>0;a--) {	
		    /* i.p.v. jit ook met random geprobeerd: lelijk! */
		xs= xs1 + xres*j[0];
		ys= ys1 + yres*j[1];
		j+=2;
		
		aantal+= readshadowbuf(shb, xs, ys, zs);
	}

	return aantal/( (float)(num) );
}

/* different function... sampling behind clipend can be LIGHT, bias is negative! */
/* return: light */
float readshadowbuf_halo(struct ShadBuf *shb, int xs, int ys, int zs)
{
	float temp;
	int *rz, ofs;
	int zbias, zsamp;
	char *ct, *cz;

	/* simpleclip */
	if(xs<0 || ys<0) return 0.0;
	if(xs>=shb->size || ys>=shb->size) return 0.0;

	/* z berekenen */
	ofs= (ys>>4)*(shb->size>>4) + (xs>>4);
	ct= shb->cbuf+ofs;
	rz= *( (int **)(shb->zbuf+ofs) );

	if(*ct==3) {
		ct= ((char *)rz)+3*16*(ys & 15)+3*(xs & 15);
		cz= (char *)&zsamp;
		zsamp= 0;
		cz[ACOMP]= ct[0];
		cz[BCOMP]= ct[1];
		cz[GCOMP]= ct[2];
	}
	else if(*ct==2) {
		ct= ((char *)rz);
		ct+= 4+2*16*(ys & 15)+2*(xs & 15);
		zsamp= *rz;
	
		cz= (char *)&zsamp;
		cz[BCOMP]= ct[0];
		cz[GCOMP]= ct[1];
	}
	else if(*ct==1) {
		ct= ((char *)rz);
		ct+= 4+16*(ys & 15)+(xs & 15);
		zsamp= *rz;

		cz= (char *)&zsamp;
		cz[GCOMP]= ct[0];

	}
	else {
		zsamp= (int) rz;
	}

	/* geen schaduw als op oneindig wordt gesampeld*/

	if(zsamp >= 0x7FFFFE00) return 1.0; 

	if(zsamp > zs) return 1.0; 		/* absoluut geen schaduw */
	else {
		/* bias is negative, so the (zs-bias) can be beyond 0x7fffffff */
		zbias= 0x7fffffff - zs;
		if(zbias > -bias) {
			if( zsamp < zs-bias) return 0.0 ;	/* absoluut wel schaduw */
		}
		else return 0.0 ;	/* absoluut wel schaduw */
	}
	
	/* zacht gebied */

	temp=  ( (float)(zs- zsamp) )/(float)bias;
	return 1.0 - temp*temp;
}


float shadow_halo(LampRen *lar, float *p1, float *p2)
{
	/* p1 p2 already are rotated in spot-space */
	ShadBuf *shb= lar->shb;
	float co[4], siz;
	float labda, labdao, labdax, labday, ldx, ldy;
	float zf, xf1, yf1, zf1, xf2, yf2, zf2;
	float count, lightcount;
	int x, y, z, xs1, ys1, xs2, ys2;
	int dx, dy;
	
	siz= 0.5*(float)shb->size;
	/* negative! The other side is more important */
	bias= -shb->bias;
	
	co[0]= p1[0];
	co[1]= p1[1];
	co[2]= p1[2]/lar->sh_zfac;
	co[3]= 1.0;
	Mat4MulVec4fl(shb->winmat, co);	/* rationele hom co */
	xf1= siz*(1.0+co[0]/co[3]);
	yf1= siz*(1.0+co[1]/co[3]);
	zf1= (co[2]/co[3]);


	co[0]= p2[0];
	co[1]= p2[1];
	co[2]= p2[2]/lar->sh_zfac;
	co[3]= 1.0;
	Mat4MulVec4fl(shb->winmat, co);	/* rationele hom co */
	xf2= siz*(1.0+co[0]/co[3]);
	yf2= siz*(1.0+co[1]/co[3]);
	zf2= (co[2]/co[3]);

	/* de 2dda */

	xs1= (int)xf1;
	ys1= (int)yf1;

	if(xf1 != xf2) {
		if(xf2-xf1 > 0.0) {
			labdax= (xf1-xs1-1.0)/(xf1-xf2);
			ldx= -shb->shadhalostep/(xf1-xf2);
			dx= shb->shadhalostep;
		}
		else {
			labdax= (xf1-xs1)/(xf1-xf2);
			ldx= shb->shadhalostep/(xf1-xf2);
			dx= -shb->shadhalostep;
		}
	}
	else {
		labdax= 1.0;
		ldx= 0.0;
	}

	if(yf1 != yf2) {
		if(yf2-yf1 > 0.0) {
			labday= (yf1-ys1-1.0)/(yf1-yf2);
			ldy= -shb->shadhalostep/(yf1-yf2);
			dy= shb->shadhalostep;
		}
		else {
			labday= (yf1-ys1)/(yf1-yf2);
			ldy= shb->shadhalostep/(yf1-yf2);
			dy= -shb->shadhalostep;
		}
	}
	else {
		labday= 1.0;
		ldy= 0.0;
	}
	
	x= xs1;
	y= ys1;
	labda= count= lightcount= 0.0;

/* printf("start %x %x	\n", (int)(0x7FFFFFFF*zf1), (int)(0x7FFFFFFF*zf2)); */

	while(TRUE) {
		labdao= labda;
		
		if(labdax==labday) {
			labdax+= ldx;
			x+= dx;
			labday+= ldy;
			y+= dy;
		}
		else {
			if(labdax<labday) {
				labdax+= ldx;
				x+= dx;
			} else {
				labday+= ldy;
				y+= dy;
			}
		}
		
		labda= MIN2(labdax, labday);
		if(labda==labdao || labda>=1.0) break;
		
		zf= zf1 + labda*(zf2-zf1);
		count+= 1.0;

		if(zf<= 0.0) lightcount += 1.0;	/* close to the spot */
		else {
		
			/* make sure, behind the clipend we extend halolines. */
			if(zf>=1.0) z= 0x7FFFF000;
			else z= (int)(0x7FFFF000*zf);
			
			lightcount+= readshadowbuf_halo(shb, x, y, z);
			
		}
	}
	
	if(count!=0.0) return (lightcount/count);
	return 0.0;
	
}




/* sampelen met filter
	xstart= xs-1;
	ystart= ys-1;
	if(xstart<0) xstart= 0;
	if(ystart<0) ystart= 0;
	xend= xstart+2;
	yend= ystart+2;
	if(xend>=shb->size) { xstart= shb->size-3; xend= shb->size-1;}
	if(yend>=shb->size) { ystart= shb->size-3; yend= shb->size-1;}

	fid= filt3;
	for(ys=ystart;ys<=yend;ys++) {
		rz= shb->buf+ ys*shb->size+ xstart;
		for(xs= xstart;xs<=xend;xs++,rz++) {
			if( *rz+0x100000<zs) aantal+= *fid;
			fid++;
		}
	}
	

	return 1.0-((float)aantal)/16.0;
*/











