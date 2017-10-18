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

#include "imbuf.h"


#define OBJECTBLOK "izoom"

#ifndef IZOOMDEF
#define IZOOMDEF

#define IMPULSE		1
#define BOX			2
#define TRIANGLE	3
#define QUADRATIC	4
#define MITCHELL	5
#define GAUSSIAN	6
#define LANCZOS3	7
#define BSPLINE		8
#define BELL		9

typedef struct FILTER {
	int n, totw, halftotw;
	short *dat;
	short *w;
} FILTER;

typedef struct zoom {
	int (*getfunc)();
	short *abuf;
	short *bbuf;
	int anx, any;
	int bnx, bny;
	short **xmap;
	int type;
	int curay;
	int y;
	FILTER *xfilt, *yfilt;	/* stuff for fitered zoom */
	short *tbuf;
	int nrows, clamp, ay;
	short **filtrows;
	int *accrow;
} zoom;

zoom *newzoom();
float filterinteg();

#endif


typedef struct filtinteg {
	float rad, min, max;
	float *tab;
} filtinteg;

#define flerp(f0,f1,p) (f0 + p * (f1 - f0))

#define GRIDTOFLOAT(pos,n)	(((pos)+0.5)/(n))
#define FLOATTOGRID(pos,n)	((pos)*(n))
#define SHIFT 				12
#define ONE 				(1<<SHIFT)
#define EPSILON				0.0001
#define FILTERRAD			(blurfactor*shape->rad)
#define FILTTABSIZE			250

static makexmap();
static setintrow();
static xscalebuf();
static addrow();
static divrow();
static FILTER *makefilt();
static freefilt();
static applyxfilt();
float filterinteg();
static mitchellinit();

static int (*xfiltfunc)();
static float blurfactor;
int izoomdebug;

static filtinteg *shapeBOX;
static filtinteg *shapeTRIANGLE;
static filtinteg *shapeQUADRATIC;
static filtinteg *shapeMITCHELL;
static filtinteg *shapeGAUSSIAN;
static filtinteg *shapeLANCZOS3;
static filtinteg *shapeBSPLINE;
static filtinteg *shapeBELL;
static filtinteg *shape;


float filt_box(x)
float x;
{
	if (x<-0.5) return 0.0;
	if (x< 0.5) return 1.0;
	return 0.0;
}

float filt_triangle(x)
float x;
{
	if (x < 0.0) x = -x;
	if (x < 1.0) return 1.0 - x;
	return 0.0;
}

float filt_quadratic(x)
float x;
{
	if (x<-1.5) return 0.0;
	if (x<-0.5) return 0.5*(x+1.5)*(x+1.5);
	if (x< 0.5) return 0.75-(x*x);
	if (x< 1.5) return 0.5*(x-1.5)*(x-1.5);
	return 0.0;
}

static float p0, p2, p3, q0, q1, q2, q3;

/*
 * see Mitchell&Netravali, "Reconstruction Filters in Computer Graphics",
 * SIGGRAPH 88.  Mitchell code provided by Paul Heckbert.
 *
 */

float filt_mitchell(x)	/* Mitchell & Netravali's two-param cubic */
float x;
{
	static int mitfirsted;

	if(!mitfirsted) {
		mitchellinit(1.0/3.0,1.0/3.0);
		mitfirsted = 1;
	}
	if (x<-2.0) return 0.0;
	if (x<-1.0) return (q0-x*(q1-x*(q2-x*q3)));
	if (x< 0.0) return (p0+x*x*(p2-x*p3));
	if (x< 1.0) return (p0+x*x*(p2+x*p3));
	if (x< 2.0) return (q0+x*(q1+x*(q2+x*q3)));
	return 0.0;
}

static mitchellinit(b,c)
float b, c;
{
	p0 = (  6.0 -  2.0*b         ) / 6.0;
	p2 = (-18.0 + 12.0*b +  6.0*c) / 6.0;
	p3 = ( 12.0 -  9.0*b -  6.0*c) / 6.0;
	q0 = (	   8.0*b + 24.0*c) / 6.0;
	q1 = (      - 12.0*b - 48.0*c) / 6.0;
	q2 = (         6.0*b + 30.0*c) / 6.0;
	q3 = (       -     b -  6.0*c) / 6.0;
}

#define NARROWNESS	1.5

float filt_gaussian(x)
float x;
{
	x = x*NARROWNESS;
	return (1.0/exp(x*x) - 1.0/exp(1.5*NARROWNESS*1.5*NARROWNESS));
}



float filt_bell(t)		/* box (*) box (*) box */
float t;
{
	if(t < 0) t = -t;
	if(t < .5) return(.75 - (t * t));
	if(t < 1.5) {
		t = (t - 1.5);
		return(.5 * (t * t));
	}
	return(0.0);
}

float filt_bspline(t)	/* box (*) box (*) box (*) box */
float t;
{
	double tt;

	if(t < 0) t = -t;
	if(t < 1) {
		tt = t * t;
		return((.5 * tt * t) - tt + (2.0 / 3.0));
	} else if(t < 2) {
		t = 2 - t;
		return((1.0 / 6.0) * (t * t * t));
	}
	return(0.0);
}

float sinc(x)
float x;
{
	x *= M_PI;
	if(x != 0) return(sin(x) / x);
	return(1.0);
}


float filt_lanczos3(t)
float t;
{
	if(t < 0) t = -t;
	if(t < 3.0) return(sinc(t) * sinc(t/3.0));
	return(0.0);
}



/* We gaan dit snel testen dus we doen niet al te netjes */

struct ImBuf * zoomin, * zoomuit;
int channel;

getimgrow(short * buf, int y)
{
    uchar * rect;
	int x;
	
	rect = (uchar *) zoomin->rect;
	rect += 4 * y * zoomin->x;
	rect += channel;
	
	for (x = zoomin->x; x > 0; x--) {
		*buf++ = rect[0];
		rect += 4;
	}
}


putimgrow(short * buf, int y)
{
    uchar * rect;
	int x;
	
	rect = (uchar *) zoomuit->rect;
	rect += 4 * y * zoomuit->x;
	rect += channel;
	
	for (x = zoomuit->x; x > 0; x--) {
		rect[0] = *buf++;
		rect += 4;
	}
}


#define DOCLAMP(iptr)	(((*(iptr) & 0xff00) == 0) ? *(iptr): ((*(iptr)>0) ? 255 : 0))

clamprow(iptr,optr,n)
short *iptr, *optr;
int n;
{
	short  val;

	while(n>=8) {
		optr[0] = DOCLAMP(iptr+0);
		optr[1] = DOCLAMP(iptr+1);
		optr[2] = DOCLAMP(iptr+2);
		optr[3] = DOCLAMP(iptr+3);
		optr[4] = DOCLAMP(iptr+4);
		optr[5] = DOCLAMP(iptr+5);
		optr[6] = DOCLAMP(iptr+6);
		optr[7] = DOCLAMP(iptr+7);
		iptr += 8;
		optr += 8;
		n -= 8;
	}
	while(n--) {
		optr[0] = DOCLAMP(iptr+0);
		iptr++;
		optr++;
	}
}


static filtinteg *integrate(filtfunc,rad)
float (*filtfunc)();
float rad;
{
	int i;
	float del, x, min, max;
	double tot;
	filtinteg *filt;

	min = -rad;
	max =  rad;
	del = 2*rad;
	tot = 0.0;
	filt = (filtinteg *)malloc(sizeof(filtinteg));
	filt->rad = rad;
	filt->min = min;
	filt->max = max;
	filt->tab = (float *)malloc(FILTTABSIZE*sizeof(float));
	for(i=0; i<FILTTABSIZE; i++) {
		x = min+(del*i/(FILTTABSIZE-1.0));
		tot = tot+filtfunc(x);
		filt->tab[i] = tot;
	}
	for(i=0; i<FILTTABSIZE; i++)
		filt->tab[i] /= tot;
	return filt;
}

float filterinteg(bmin,bmax,blurf)
float bmin, bmax, blurf;
{
	int i1, i2;
	float f1, f2;
	float *tab;
	float mult;

	bmin /= blurf;
	bmax /= blurf;
	tab = shape->tab;
	mult = (FILTTABSIZE-1.0)/(2.0*shape->rad);

	f1 = ((bmin-shape->min)*mult);
	i1 = ffloor(f1);
	f1 = f1-i1;
	if(i1<0)
		f1 = 0.0;
	else if(i1>=(FILTTABSIZE-1))
		f1 = 1.0;
	else
		f1 = flerp(tab[i1],tab[i1+1],f1);

	f2 = ((bmax-shape->min)*mult);
	i2 = ffloor(f2);
	f2 = f2-i2;
	if(i2<0)
		f2 = 0.0;
	else if(i2>=(FILTTABSIZE-1))
		f2 = 1.0;
	else
		f2 = flerp(tab[i2],tab[i2+1],f2);
	return f2-f1;
}

setfiltertype(filttype)
int filttype;
{
	switch(filttype) {
	case IMPULSE:
		shape = 0;
		break;
	case BOX:
		if(!shapeBOX)
			shapeBOX = integrate(filt_box,0.5);
		shape = shapeBOX;
		break;
	case TRIANGLE:
		if(!shapeTRIANGLE)
			shapeTRIANGLE = integrate(filt_triangle,1.0);
		shape = shapeTRIANGLE;
		break;
	case QUADRATIC:
		if(!shapeQUADRATIC)
			shapeQUADRATIC = integrate(filt_quadratic,1.5);
		shape = shapeQUADRATIC;
		break;
	case MITCHELL:
		if(!shapeMITCHELL)
			shapeMITCHELL = integrate(filt_mitchell,2.0);
		shape = shapeMITCHELL;
		break;
	case GAUSSIAN:
		if(!shapeGAUSSIAN)
			shapeGAUSSIAN = integrate(filt_gaussian,1.5);
		shape = shapeGAUSSIAN;
		break;
	case LANCZOS3:
		if (!shapeLANCZOS3)
			shapeLANCZOS3 = integrate(filt_lanczos3, 3.0);
		break;
	case BSPLINE:
		if (!shapeBSPLINE)
			shapeBSPLINE = integrate(filt_bspline, 2.0);
		break;
	case BELL:
		if (!shapeBELL)
			shapeBELL = integrate(filt_bell, 1.5);
		break;
	}
}


/*
 *	general zoom follows
 *
 */
 
zoom *newzoom(getfunc,anx,any,bnx,bny,filttype,blur)
int (*getfunc)();
int anx,any,bnx,bny;
int filttype;
float blur;
{
	zoom *z;
	int i;

	setfiltertype(filttype);
	z = (zoom *)malloc(sizeof(zoom));
	z->getfunc = getfunc;
	z->abuf = (short *)malloc(anx*sizeof(short));
	z->bbuf = (short *)malloc(bnx*sizeof(short));
	z->anx = anx;
	z->any = any;
	z->bnx = bnx;
	z->bny = bny;
	z->curay = -1;
	z->y = 0;
	z->type = filttype;
	if(filttype == IMPULSE) {
		if(z->anx != z->bnx) {
			z->xmap = (short **)malloc(z->bnx*sizeof(short *));
			makexmap(z->abuf,z->xmap,z->anx,z->bnx);
		}
	} else {
		blurfactor = blur;
		if(filttype == MITCHELL)
			z->clamp = 1;
		else
			z->clamp = 0;
		z->tbuf = (short *)malloc(bnx*sizeof(short));
		z->xfilt = makefilt(z->abuf,anx,bnx,&z->nrows);
		z->yfilt = makefilt(0,any,bny,&z->nrows);
		z->filtrows = (short **)malloc(z->nrows * sizeof(short *));
		for(i=0; i<z->nrows; i++)
			z->filtrows[i] = (short *)malloc(z->bnx*sizeof(short));
		z->accrow = (int *)malloc(z->bnx*sizeof(int));
		z->ay = 0;
	}
	return z;
}

getzoomrow(z,buf,y)
zoom *z;
short *buf;
int y;
{
	float fy;
	int ay;
	FILTER *f;
	int i, max;
	short *row;

	if(y==0) {
		z->curay = -1;
		z->y = 0;
		z->ay = 0;
	}
	if(z->type == IMPULSE) {
		fy = GRIDTOFLOAT(z->y,z->bny);
		ay = FLOATTOGRID(fy,z->any);
		if(z->anx == z->bnx) {
			if(z->curay != ay) {
				z->getfunc(z->abuf,ay);
				z->curay = ay;
				if(xfiltfunc)
					xfiltfunc(z->abuf,z->bnx);
			}
			bcopy(z->abuf,buf,z->bnx*sizeof(short));
		} else {
			if(z->curay != ay) {
				z->getfunc(z->abuf,ay);
				xscalebuf(z->xmap,z->bbuf,z->bnx);
				z->curay = ay;
				if(xfiltfunc)
					xfiltfunc(z->bbuf,z->bnx);
			}
			bcopy(z->bbuf,buf,z->bnx*sizeof(short));
		}
	} else if(z->any == 1 && z->bny == 1) {
		z->getfunc(z->abuf,z->ay++);
		applyxfilt(z->filtrows[0],z->xfilt,z->bnx);
		if(xfiltfunc)
			xfiltfunc(z->filtrows[0],z->bnx);
		if(z->clamp) {
			clamprow(z->filtrows[0],z->tbuf,z->bnx);
			bcopy(z->tbuf,buf,z->bnx*sizeof(short));
		} else {
			bcopy(z->filtrows[0],buf,z->bnx*sizeof(short));
		}
	} else {
		f = z->yfilt+z->y;
		max = ((long)f->dat)/sizeof(short)+(f->n-1);
		while(z->ay<=max) {
			z->getfunc(z->abuf,z->ay++);
			row = z->filtrows[0];
			for(i=0; i<(z->nrows-1); i++)
				z->filtrows[i] = z->filtrows[i+1];
			z->filtrows[z->nrows-1] = row;
			applyxfilt(z->filtrows[z->nrows-1],z->xfilt,z->bnx);
			if(xfiltfunc)
				xfiltfunc(z->filtrows[z->nrows-1],z->bnx);
		}
		if(f->n == 1) {
			if(z->clamp) {
				clamprow(z->filtrows[z->nrows-1],z->tbuf,z->bnx);
				bcopy(z->tbuf,buf,z->bnx*sizeof(short));
			} else {
				bcopy(z->filtrows[z->nrows-1],buf,z->bnx*sizeof(short));
			}
		} else {
			setintrow(z->accrow,f->halftotw,z->bnx);
			for(i=0; i<f->n; i++)
				addrow(z->accrow, z->filtrows[i+(z->nrows-1)-(f->n-1)],
				    f->w[i],z->bnx);
			divrow(z->accrow,z->bbuf,f->totw,z->bnx);
			if(z->clamp) {
				clamprow(z->bbuf,z->tbuf,z->bnx);
				bcopy(z->tbuf,buf,z->bnx*sizeof(short));
			} else {
				bcopy(z->bbuf,buf,z->bnx*sizeof(short));
			}
		}
	}
	z->y++;
}

static setintrow(buf,val,n)
int *buf;
int val, n;
{
	while(n>=8) {
		buf[0] = val;
		buf[1] = val;
		buf[2] = val;
		buf[3] = val;
		buf[4] = val;
		buf[5] = val;
		buf[6] = val;
		buf[7] = val;
		buf += 8;
		n -= 8;
	}
	while(n--)
		*buf++ = val;
}

freezoom(z)
zoom *z;
{
	int i;

	if(z->type == IMPULSE) {
		if(z->anx != z->bnx)
			free(z->xmap);
	} else {
		freefilt(z->xfilt,z->bnx);
		freefilt(z->yfilt,z->bny);
		free(z->tbuf);
		for(i=0; i<z->nrows; i++)
			free(z->filtrows[i]);
		free(z->filtrows);
		free(z->accrow);
	}
	free(z->abuf);
	free(z->bbuf);
	free(z);

}

filterzoom(getfunc,putfunc,anx,any,bnx,bny,filttype,blur)
int (*getfunc)(), (*putfunc)();
int anx, any;
int bnx, bny;
int filttype;
float blur;
{
	zoom *z;
	int y;
	short *buf;

	buf = (short *)malloc(bnx*sizeof(short));
	z = newzoom(getfunc,anx,any,bnx,bny,filttype,blur);
	for(y=0; y<bny; y++) {
		getzoomrow(z,buf,y);
		putfunc(buf,y);
	}
	freezoom(z);
	free(buf);
}

/*
 *	impulse zoom utilities
 *
 */
static makexmap(abuf,xmap,anx,bnx)
short *abuf;
short *xmap[];
int anx, bnx;
{
	int x, ax;
	float fx;

	for(x=0; x<bnx; x++) {
		fx = GRIDTOFLOAT(x,bnx);
		ax = FLOATTOGRID(fx,anx);
		xmap[x] = abuf+ax;
	}
}

static xscalebuf(xmap,bbuf,bnx)
short *xmap[];
short *bbuf;
int bnx;
{
	while(bnx>=8) {
		bbuf[0] = *(xmap[0]);
		bbuf[1] = *(xmap[1]);
		bbuf[2] = *(xmap[2]);
		bbuf[3] = *(xmap[3]);
		bbuf[4] = *(xmap[4]);
		bbuf[5] = *(xmap[5]);
		bbuf[6] = *(xmap[6]);
		bbuf[7] = *(xmap[7]);
		bbuf += 8;
		xmap += 8;
		bnx -= 8;
	}
	while(bnx--)
		*bbuf++ = *(*xmap++);
}

zoomxfilt(filtfunc)
int (*filtfunc)();
{
	xfiltfunc = filtfunc;
}

/*
 *	filter zoom utilities
 *
 */

static addrow(iptr,sptr,w,n)
int *iptr;
short *sptr;
int w, n;
{
	while(n>=8) {
		iptr[0] += (w*sptr[0]);
		iptr[1] += (w*sptr[1]);
		iptr[2] += (w*sptr[2]);
		iptr[3] += (w*sptr[3]);
		iptr[4] += (w*sptr[4]);
		iptr[5] += (w*sptr[5]);
		iptr[6] += (w*sptr[6]);
		iptr[7] += (w*sptr[7]);
		iptr += 8;
		sptr += 8;
		n -= 8;
	}
	while(n--)
		*iptr++ += (w * *sptr++);
}

static divrow(iptr,sptr,tot,n)
int *iptr;
short *sptr;
int tot, n;
{
	while(n>=8) {
		sptr[0] = iptr[0]/tot;
		sptr[1] = iptr[1]/tot;
		sptr[2] = iptr[2]/tot;
		sptr[3] = iptr[3]/tot;
		sptr[4] = iptr[4]/tot;
		sptr[5] = iptr[5]/tot;
		sptr[6] = iptr[6]/tot;
		sptr[7] = iptr[7]/tot;
		sptr += 8;
		iptr += 8;
		n -= 8;
	}
	while(n--)
		*sptr++ = (*iptr++)/tot;
}

static FILTER *makefilt(abuf,anx,bnx,maxn)
short *abuf;
int anx, bnx;
int *maxn;
{
	FILTER *f, *filter;
	int x, min, max, n, pos;
	float bmin, bmax, bcent, brad;
	float fmin, fmax, acent, arad;
	int amin, amax;
	float coverscale;

	if(izoomdebug)
		fprintf(stderr,"makefilt\n");
	f = filter = (FILTER *)malloc(bnx*sizeof(FILTER));
	*maxn = 0;
	if(bnx<anx) {
		coverscale = ((float)anx/bnx*ONE)/2.0;
		brad = FILTERRAD/bnx;
		for(x=0; x<bnx; x++) {
			bcent = ((float)x+0.5)/bnx;
			amin = ffloor((bcent-brad)*anx+EPSILON);
			amax = ffloor((bcent+brad)*anx-EPSILON);
			if(amin<0)
				amin = 0;
			if(amax>=anx)
				amax = anx-1;
			f->n = 1+amax-amin;
			f->dat = abuf+amin;
			f->w = (short *)malloc(f->n*sizeof(short));
			f->totw = 0;
			if(izoomdebug)
				fprintf(stderr,"| ");
			for(n=0; n<f->n; n++) {
				bmin = bnx*((((float)amin+n)/anx)-bcent);
				bmax = bnx*((((float)amin+n+1)/anx)-bcent);
				f->w[n] = ffloor((coverscale*filterinteg(bmin,bmax,blurfactor))+0.5);
				if(izoomdebug)
					fprintf(stderr,"%d ",f->w[n]);
				f->totw += f->w[n];
			}
			f->halftotw = f->totw/2;
			if(f->n>*maxn)
				*maxn = f->n;
			f++;
		}
	} else {
		coverscale = ((float)bnx/anx*ONE)/2.0;
		arad = FILTERRAD/anx;
		for(x=0; x<bnx; x++) {
			bmin = ((float)x)/bnx;
			bmax = ((float)x+1.0)/bnx;
			amin = ffloor((bmin-arad)*anx+(0.5+EPSILON));
			amax = ffloor((bmax+arad)*anx-(0.5+EPSILON));
			if(amin<0)
				amin = 0;
			if(amax>=anx)
				amax = anx-1;
			f->n = 1+amax-amin;
			f->dat = abuf+amin;
			f->w = (short *)malloc(f->n*sizeof(short));
			f->totw = 0;
			if(izoomdebug)
				fprintf(stderr,"| ");
			for(n=0; n<f->n; n++) {
				acent = (amin+n+0.5)/anx;
				fmin = anx*(bmin-acent);
				fmax = anx*(bmax-acent);
				f->w[n] = ffloor((coverscale*filterinteg(fmin,fmax,blurfactor))+0.5);
				if(izoomdebug)
					fprintf(stderr,"%d ",f->w[n]);
				f->totw += f->w[n];
			}
			f->halftotw = f->totw/2;
			if(f->n>*maxn)
				*maxn = f->n;
			f++;
		}
	}
	if(izoomdebug)
		fprintf(stderr,"|\n");
	return filter;
}

static freefilt(filt,n)
FILTER *filt;
int n;
{
	FILTER *f;

	f = filt;
	while(n--) {
		free(f->w);
		f++;
	}
	free(filt);
}

static applyxfilt(bbuf,xfilt,bnx)
short *bbuf;
FILTER *xfilt;
int bnx;
{
	short *w;
	short *dptr;
	int n, val;

	while(bnx--) {
		if((n=xfilt->n) == 1) {
			*bbuf++ = *xfilt->dat;
		} else {
			w = xfilt->w;
			dptr = xfilt->dat;
			val = xfilt->halftotw;
			n = xfilt->n;
			while(n--)
				val += *w++ * *dptr++;
			*bbuf++ = val / xfilt->totw;
		}
		xfilt++;
	}
}


struct ImBuf * zoomImBuf(struct ImBuf * ibuf, int newx, int newy, int filttype, double blur)
{
	int i;
	
	if (ibuf == 0) return (0);
	if (ibuf->rect == 0) return (0);
	
	zoomin = ibuf;
	zoomuit = allocImBuf(newx, newy, ibuf->depth, IB_rect, 0);
	
	for (i = 3; i >= 0; i--) {
		channel = i;
		filterzoom(getimgrow, putimgrow, ibuf->x, ibuf->y, newx, newy, filttype, blur);
	} 
	
	return (zoomuit);
}

