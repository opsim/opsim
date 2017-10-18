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


/*  image.c        MIX MODEL
 * 
 *  maart 95
 * 
 */

#include "blender.h"
#include "render.h"
#include "sector.h"

extern float Tin, Tr, Tg, Tb, Ta;

int Talpha;
int imaprepeat, imapextend;


/*
 * 
 *  Talpha==TRUE betekent: lees alpha uit plaatje. Dit betekent niet dat Ta
 *  niet gebruikt moet worden,  hier kan info over rand van image in staan!
 * 
 */

void free_image(Image *ima)
{
	int a;

	if(ima->ibuf) freeImBuf(ima->ibuf);
	ima->ibuf= 0;
	if(ima->anim) free_anim(ima->anim);
	ima->anim= 0;
	
	for(a=0; a<MAXMIPMAP; a++) {
		if(ima->mipmap[a]) freeImBuf(ima->mipmap[a]);
		ima->mipmap[a]= 0;
	}
	
	free_realtime_image(ima);
}


Image *add_image(char *name)
{
	Image *ima;
	int file, len;
	char *libname, str[256], strtest[256];
	
	strcpy(str, name);
	convertstringcode(str);
	
	file= open(str, O_BINARY|O_RDONLY);
	if(file== -1) return 0;
	close(file);
	
	/* eerst zoeken naar eenzelfde ima */
	ima= G.main->image.first;
	while(ima) {
		strcpy(strtest, ima->name);
		convertstringcode(strtest);
		if( strcmp(strtest, str)==0 ) {
			if(ima->anim==0 || ima->id.us==0) {
				strcpy(ima->name, name);	/* for stringcode */
				ima->id.us++;
				ima->ok= 1;
				return ima;
			}
		}
		ima= ima->id.next;
	}

	len= strlen(name);
	
	while(len>0 && name[len-1]!='/') len--;
	libname= name+len;
	
	ima= alloc_libblock(&G.main->image, ID_IM, libname);
	strcpy(ima->name, name);
	ima->ok= 1;
	
	ima->xrep= ima->yrep= 1;
	
	return ima;
}

void free_unused_animimages()
{
	Image *ima, *nima;

	ima= G.main->image.first;
	while(ima) {
		nima= ima->id.next;
		if(ima->id.us==0) {
			if(ima->flag & IMA_FROMANIM) free_libblock(&G.main->image, ima);
		}
		ima= nima;
	}
}


/* *********** LEZEN EN SCHRIJVEN ************** */

void makepicstring(char *string, int frame)
{
	short i,len;
	char num[10], *extension;

	if (string==0) return;

	extension= "";

	strcpy(string, G.scene->r.pic);
	convertstringcode(string);

	if(G.scene->r.scemode & R_NETWORK) {
		if(G.cursector) {
			strcat(string, G.cursector->id.name+2);
			len= strlen(string);
			string[len]= '/';
			string[len+1]= 0;
			
			frame-= G.cursector->sfra-1;
		}
	}


	len=strlen(string);

	/* kan ook: sprintf(num, "%04d", frame); */

	i=4-sprintf(num,"%d",frame);
	for(;i>0;i--){
		string[len]='0';
		len++;
	}
	string[len]=0;
	strcat(string,num);

	if(G.scene->r.imtype== R_IRIS) {
		extension= ".rgb";
	}
	else if(G.scene->r.imtype==R_IRIZ) {
		extension= ".rgb";
	}
	else if(G.scene->r.imtype==R_MDEC) {
		extension= ".mdc";
	}
	else if(G.scene->r.imtype==R_TARGA) {
		extension= ".tga";
	}
	else if(G.scene->r.imtype==R_RAWTGA) {
		extension= ".tga";
	}
	else if(G.scene->r.imtype==R_JPEG90) {
		extension= ".jpg";
	}
	
	if(G.scene->r.scemode & R_EXTENSION) strcat(string, extension);
		
}

void write_ibuf(ImBuf *ibuf, char *name)
{
	/* to be used for e.g. envmap, not rendered images */
	
	if(R.r.imtype== R_IRIS) ibuf->ftype= IMAGIC;
	else if ELEM(R.r.imtype, R_TARGA, R_MDEC) {
		ibuf->ftype= TGA;
	}
	else if(R.r.imtype==R_RAWTGA) {
		ibuf->ftype= RAWTGA;
	}
	else if(R.r.imtype==R_HAMX) {
		ibuf->ftype= AN_hamx;
	}
	else if ELEM(R.r.imtype, R_JPEG90, R_MOVIE) {
		if(R.r.quality < 10) R.r.quality= 90;

		ibuf->ftype= JPG|R.r.quality;
	}
	else ibuf->ftype= TGA;
	
	make_existing_file(name);
	
	if(saveiff(ibuf, name, IB_rect)==0) {
		perror(name);
	}
}

/* ******** IMAGWRAPPING INIT ************* */

void converttopremul(ibuf)
struct ImBuf *ibuf;
{
	int x, y, val;
	char *cp;
	
	if(ibuf==0) return;
	if(ibuf->depth==24) {	/* alpha op 255 zetten */

		cp= (char *)(ibuf->rect);
		for(y=0; y<ibuf->y; y++) {
			for(x=0; x<ibuf->x; x++, cp+=4) {
				cp[3]= 255;
			}
		}
		return;
	}
	
	cp= (char *)(ibuf->rect);
	for(y=0; y<ibuf->y; y++) {
		for(x=0; x<ibuf->x; x++, cp+=4) {
			if(cp[3]==0) {
				cp[0]= cp[1]= cp[2]= 0;
			}
			else if(cp[3]!=255) {
				val= cp[3];
				cp[0]= (cp[0]*val)>>8;
				cp[1]= (cp[1]*val)>>8;
				cp[2]= (cp[2]*val)>>8;
			}
		}
	}
}



void makemipmap(ima)
Image *ima;
{
	struct ImBuf *ibuf;
	struct MipMap *mm, *mmn;
	int minsize, curmap=0;

	ibuf= ima->ibuf;
	minsize= MIN2(ibuf->x, ibuf->y);

	while(minsize>3 && curmap<MAXMIPMAP) {

		ibuf= dupImBuf(ibuf);
		filter(ibuf);
		ima->mipmap[curmap]= (struct ImBuf *)onehalf(ibuf);
		freeImBuf(ibuf);
		ibuf= ima->mipmap[curmap];
		
		curmap++;
		minsize= MIN2(ibuf->x, ibuf->y);
	}
}

struct anim *openanim(char * name, int flags)
{
	struct anim * anim;
	struct ImBuf * ibuf;
	
	anim = open_anim(name, ANIM_DFLT, flags);
	if (anim == 0) return(0);

	
	ibuf = anim_absolute(anim, 0);
	if (ibuf == 0) {
		printf("anim_absolute 0 failed\n");
		free_anim(anim);
		return(0);
	}
	freeImBuf(ibuf);
	
	return(anim);
}

int calcimanr(int cfra, Tex *tex)
{
	int imanr, len, a, fra, dur;

	/* hier (+fie_ima/2-1) zorgt ervoor dat correct wordt gedeeld */
	
	if(tex->frames==0) return 1;
	
	cfra= cfra-tex->sfra+1;
	
	/* cyclic */
	if(tex->len==0) len= (tex->fie_ima*tex->frames)/2;
	else len= tex->len;
	
	if(tex->imaflag & TEX_ANIMCYCLIC) {
		cfra= ( (cfra) % len );
		if(cfra < 0) cfra+= len;
		if(cfra==0) cfra= len;
	}
	
	if(cfra<1) cfra= 1;
	else if(cfra>len) cfra= len;
	
	/* omzetten current frame naar current field */
	cfra= 2*(cfra);
	if(R.flag & R_SEC_FIELD) cfra++;
	

	/* transformeren naar images space */
	imanr= (cfra+tex->fie_ima-2)/tex->fie_ima;
	
	if(imanr>tex->frames) imanr= tex->frames;
	imanr+= tex->offset;
	
	/* zijn er plaatjes die langer duren? */
	for(a=0; a<4; a++) {
		if(tex->fradur[a][0]) {
			
			fra= tex->fradur[a][0];
			dur= tex->fradur[a][1]-1;
			
			while(dur>0 && imanr>fra) {
				imanr--;
				dur--;
			}
		}
	}
	
	
	return imanr;
}

void do_laseroptics_patch(ImBuf *ibuf)
{
	char *rt;
	float fac;
	int a, val;
	
	rt= (char *)ibuf->rect;
	a= ibuf->x*ibuf->y;

	if(ibuf->flags & IB_fields) a+= a;

	while(a--) {
		
		fac= (rt[1]+rt[2]+rt[3])/765.0;
		val= ((255.0/0.8)*(fac-0.1));
		
		if(val<0) val= 0; else if(val>255) val= 255;
		
		rt[0]= rt[1]= rt[2]= rt[3]= val;
		
		rt+= 4;
	}
}

void de_interlace_ng(struct ImBuf *ibuf)	/* neogeo fields */
{
	struct ImBuf * tbuf1, * tbuf2;
	extern rectcpy();
	
	if (ibuf == 0) return;
	if (ibuf->flags & IB_fields) return;
	ibuf->flags |= IB_fields;
	
	if (ibuf->rect) {
		/* kopieen aanmaken */
		tbuf1 = allocImBuf(ibuf->x, ibuf->y / 2, 32, IB_rect, 0);
		tbuf2 = allocImBuf(ibuf->x, ibuf->y / 2, 32, IB_rect, 0);
		
		ibuf->x *= 2;	
		rectop(tbuf1, ibuf, 0, 0, 0, 0, 32767, 32767, rectcpy);
		rectop(tbuf2, ibuf, 0, 0, tbuf2->x, 0, 32767, 32767, rectcpy);
	
		ibuf->x /= 2;
		rectop(ibuf, tbuf1, 0, 0, 0, 0, 32767, 32767, rectcpy);
		rectop(ibuf, tbuf2, 0, tbuf2->y, 0, 0, 32767, 32767, rectcpy);
		
		freeImBuf(tbuf1);
		freeImBuf(tbuf2);
	}
	ibuf->y /= 2;
}

void de_interlace_st(struct ImBuf *ibuf)	/* standard fields */
{
	struct ImBuf * tbuf1, * tbuf2;
	extern rectcpy();
	
	if (ibuf == 0) return;
	if (ibuf->flags & IB_fields) return;
	ibuf->flags |= IB_fields;
	
	if (ibuf->rect) {
		/* kopieen aanmaken */
		tbuf1 = allocImBuf(ibuf->x, ibuf->y / 2, 32, IB_rect, 0);
		tbuf2 = allocImBuf(ibuf->x, ibuf->y / 2, 32, IB_rect, 0);
		
		ibuf->x *= 2;	
		rectop(tbuf1, ibuf, 0, 0, 0, 0, 32767, 32767, rectcpy);
		rectop(tbuf2, ibuf, 0, 0, tbuf2->x, 0, 32767, 32767, rectcpy);
	
		ibuf->x /= 2;
		rectop(ibuf, tbuf2, 0, 0, 0, 0, 32767, 32767, rectcpy);
		rectop(ibuf, tbuf1, 0, tbuf2->y, 0, 0, 32767, 32767, rectcpy);
		
		freeImBuf(tbuf1);
		freeImBuf(tbuf2);
	}
	ibuf->y /= 2;
}


void ima_ibuf_is_nul(Tex *tex)
{
	void (*de_interlacefunc)();
	Image *ima;
	int a, fra;
	short curs;
	char str[FILE_MAXDIR+FILE_MAXFILE], *cp;
	
	ima= tex->ima;
	if(ima==0) return;

	waitcursor(1);
		
	strcpy(str, ima->name);
	convertstringcode(str);
	
	if(tex->imaflag & TEX_STD_FIELD) de_interlacefunc= de_interlace_st;
	else de_interlacefunc= de_interlace_ng;

	if(tex->imaflag & TEX_ANIM5) {
	
		if(ima->anim==0) ima->anim = openanim(str, IB_cmap | IB_rect);
		if(ima->anim) {
			
			ima->lastquality= R.osa;
			fra= ima->lastframe-1;
			if(fra<0) fra= 0;
			ima->ibuf = anim_absolute(ima->anim, fra);
			
			/* patch ivm textbutton met naam ima (B_NAMEIMA) */
			if(ima->ibuf) {
				strcpy(ima->ibuf->name, ima->name);
				if (tex->imaflag & TEX_FIELDS) de_interlacefunc(ima->ibuf);
			}
		}
		else error("Not an anim");

	}
	else {
		ima->ibuf = loadiffname(str , LI_rect);
		if(tex->imaflag & TEX_FIELDS) de_interlacefunc(ima->ibuf);
		
		ima->lastquality= R.osa;
	}
	
	if(ima->ibuf) {
		
		/* stringcodes ook in ibuf. ibuf->name wordt als 'undo' gebruikt (buttons.c) */
		strcpy(ima->ibuf->name, ima->name);
		
		if(ima->ibuf->cmap) {
			
			if(tex->imaflag & TEX_ANIM5) {
			
				if(tex->imaflag & TEX_MORKPATCH) {
						/**** PATCH OM KLEUR 2 GOED TE KUNNEN ZETTEN MORKRAMIA */
					if(ima->ibuf->maxcol > 4) {
						cp= (char *)(ima->ibuf->cmap+2);
						cp[0]= 0x80;
					}
				}
			
				applycmap(ima->ibuf);
				convert_rgba_to_abgr(ima->ibuf->x*ima->ibuf->y, ima->ibuf->rect);
				
			}
			
			converttopremul(ima->ibuf);
		}
		
		if(R.osa) {
			
			if(tex->imaflag & TEX_ANTISCALE) {
				clever_double(ima->ibuf);
				antialias(ima->ibuf);
			}
			else if(tex->imaflag & TEX_ANTIALI) antialias(ima->ibuf);
		}
		
		if(tex->imaflag & TEX_LASOPPATCH) {
			do_laseroptics_patch(ima->ibuf);
		}
		
	}
	
	if(ima->ibuf==0) ima->ok= 0;
	
	for(a=0; a<MAXMIPMAP; a++) {
		if(ima->mipmap[a]) freeImBuf(ima->mipmap[a]);
		ima->mipmap[a]= 0;
	}

	if((R.flag & R_RENDERING)==0) waitcursor(0);
	
}



/* *********** IMAGEWRAPPING ****************** */


int imagewrap(Tex *tex, float *texvec)
{
	Image *ima;
	struct ImBuf *ibuf;
	float fx, fy, fac1, fac2, val1, val2, val3;
	int ofs, x, y;
	char *rect;

	ima= tex->ima;

	if(ima==0 || ima->ok== 0) {
		Tin= Ta= Tr= Tg= Tb= 0.0;
		return 0;
	}
	
	if(ima->ibuf==0) ima_ibuf_is_nul(tex);

	if (ima->ok) {

		ibuf = ima->ibuf;

		if( (R.flag & R_SEC_FIELD) && (ibuf->flags & IB_fields) ) {
			ibuf->rect+= (ibuf->x*ibuf->y);
		}
		
		if(tex->imaflag & TEX_IMAROT) {
			fy= texvec[0];
			fx= texvec[1];
		}
		else {
			fx= texvec[0];
			fy= texvec[1];
		}
		
		x = (int)(fx*ibuf->x);
		y = (int)(fy*ibuf->y);

		if(tex->extend == TEX_CLIPCUBE) {
			if(x<0 || y<0 || x>=ibuf->x || y>=ibuf->y || texvec[2]<-1.0 || texvec[2]>1.0) {
				Tin= 0;
				return 0;
			}
		}
		else if(tex->extend == TEX_CLIP) {
			if(x<0 || y<0 || x>=ibuf->x || y>=ibuf->y) {
				Tin= 0;
				return 0;
			}
		}
		else {
			if(tex->extend==TEX_EXTEND) {
				if(x>=ibuf->x) x = ibuf->x-1;
				else if(x<0) x= 0;
			}
			else {
				x= x % ibuf->x;
				if(x<0) x+= ibuf->x;
			}
			if(tex->extend==TEX_EXTEND) {
				if(y>=ibuf->y) y = ibuf->y-1;
				else if(y<0) y= 0;
			}
			else {
				y= y % ibuf->y;
				if(y<0) y+= ibuf->y;
			}
		}
		
		ofs = y * ibuf->x + x;
		rect = (char *)( ibuf->rect+ ofs);

		Talpha= 0;
		if(tex->imaflag & TEX_USEALPHA) {
			if(tex->imaflag & TEX_CALCALPHA);
			else Talpha= 1;
		}

		Tr = ((float)rect[0])/255.0;
		Tg = ((float)rect[1])/255.0;
		Tb = ((float)rect[2])/255.0;
		
		if(tex->nor) {
			/* bump: drie samples nemen */
			val1= Tr+Tg+Tb;

			if(x<ibuf->x-1) {
				rect+=4;
				val2= ((float)(rect[0]+rect[1]+rect[2]))/255.0;
				rect-=4;
			}
			else val2= val1;

			if(y<ibuf->y-1) {
				rect+= 4*ibuf->x;
				val3= ((float)(rect[0]+rect[1]+rect[2]))/255.0;
			}
			else val3= val1;

			/* niet x en y verwisselen! */
			tex->nor[0]= (val1-val2);
			tex->nor[1]= (val1-val3);
		}

		BRICONRGB;

		if(Talpha) Ta= Tin= ((float)rect[3])/255.0;
		else if(tex->imaflag & TEX_CALCALPHA) {
			Ta= Tin= MAX3(Tr, Tg, Tb);
		}
		else Ta= Tin= 1.0;
		
		if(tex->flag & TEX_NEGALPHA) Ta= 1.0-Ta;

		if( (R.flag & R_SEC_FIELD) && (ibuf->flags & IB_fields) ) {
			ibuf->rect-= (ibuf->x*ibuf->y);
		}
	}

	if(tex->flag & TEX_COLORBAND) do_colorband(tex->coba);

	if(tex->nor) return 3;
	else return 1;
}

void clipx_rctf_swap(stack, count, x1, x2)
rctf *stack;
short *count;
float x1, x2;
{
	rctf *rf, *new;
	short a;

	a= *count;
	rf= stack;
	for(;a>0;a--) {
		if(rf->xmin<x1) {
			if(rf->xmax<x1) {
				rf->xmin+= (x2-x1);
				rf->xmax+= (x2-x1);
			}
			else {
				if(rf->xmax>x2) rf->xmax= x2;
				new= stack+ *count;
				(*count)++;

				new->xmax= x2;
				new->xmin= rf->xmin+(x2-x1);
				new->ymin= rf->ymin;
				new->ymax= rf->ymax;
				
				if(new->xmin==new->xmax) (*count)--;
				
				rf->xmin= x1;
			}
		}
		else if(rf->xmax>x2) {
			if(rf->xmin>x2) {
				rf->xmin-= (x2-x1);
				rf->xmax-= (x2-x1);
			}
			else {
				if(rf->xmin<x1) rf->xmin= x1;
				new= stack+ *count;
				(*count)++;

				new->xmin= x1;
				new->xmax= rf->xmax-(x2-x1);
				new->ymin= rf->ymin;
				new->ymax= rf->ymax;

				if(new->xmin==new->xmax) (*count)--;

				rf->xmax= x2;
			}
		}
		rf++;
	}

}

void clipy_rctf_swap(stack, count, y1, y2)
rctf *stack;
short *count;
float y1, y2;
{
	rctf *rf, *new;
	short a;

	a= *count;
	rf= stack;
	for(;a>0;a--) {
		if(rf->ymin<y1) {
			if(rf->ymax<y1) {
				rf->ymin+= (y2-y1);
				rf->ymax+= (y2-y1);
			}
			else {
				if(rf->ymax>y2) rf->ymax= y2;
				new= stack+ *count;
				(*count)++;

				new->ymax= y2;
				new->ymin= rf->ymin+(y2-y1);
				new->xmin= rf->xmin;
				new->xmax= rf->xmax;

				if(new->ymin==new->ymax) (*count)--;

				rf->ymin= y1;
			}
		}
		else if(rf->ymax>y2) {
			if(rf->ymin>y2) {
				rf->ymin-= (y2-y1);
				rf->ymax-= (y2-y1);
			}
			else {
				if(rf->ymin<y1) rf->ymin= y1;
				new= stack+ *count;
				(*count)++;

				new->ymin= y1;
				new->ymax= rf->ymax-(y2-y1);
				new->xmin= rf->xmin;
				new->xmax= rf->xmax;

				if(new->ymin==new->ymax) (*count)--;

				rf->ymax= y2;
			}
		}
		rf++;
	}

}



float square_rctf(rf)
rctf *rf;
{
	float x, y;

	x= rf->xmax- rf->xmin;
	y= rf->ymax- rf->ymin;
	return (x*y);
}

float clipx_rctf(rf, x1, x2)
rctf *rf;
float x1, x2;
{
	float size;

	size= rf->xmax - rf->xmin;

	if(rf->xmin<x1) {
		rf->xmin= x1;
	}
	if(rf->xmax>x2) {
		rf->xmax= x2;
	}
	if(rf->xmin > rf->xmax) {
		rf->xmin = rf->xmax;
		return 0.0;
	}
	else if(size!=0.0) {
		return (rf->xmax - rf->xmin)/size;
	}
	return 1.0;
}

float clipy_rctf(rf, y1, y2)
rctf *rf;
float y1, y2;
{
	float size;

	size= rf->ymax - rf->ymin;
/* PRINT(f, size); */
	if(rf->ymin<y1) {
		rf->ymin= y1;
	}
	if(rf->ymax>y2) {
		rf->ymax= y2;
	}
/* PRINT(f, size); */
	if(rf->ymin > rf->ymax) {
		rf->ymin = rf->ymax;
		return 0.0;
	}
	else if(size!=0.0) {
		return (rf->ymax - rf->ymin)/size;
	}
	return 1.0;

}

void boxsampleclip(ibuf, rf, rcol, gcol, bcol, acol)	/* return kleur 0.0-1.0 */
struct ImBuf *ibuf;
rctf *rf;
float *rcol, *gcol, *bcol, *acol;
{
	/* sample box, is reeds geclipt en minx enz zijn op ibuf size gezet.
     * Vergroot uit met antialiased edges van de pixels */

	float muly,mulx,div;
	int ofs;
	int x, y, startx, endx, starty, endy;
	char *rect;

	startx= ffloor(rf->xmin);
	endx= ffloor(rf->xmax);
	starty= ffloor(rf->ymin);
	endy= ffloor(rf->ymax);

	if(startx < 0) startx= 0;
	if(starty < 0) starty= 0;
	if(endx>=ibuf->x) endx= ibuf->x-1;
	if(endy>=ibuf->y) endy= ibuf->y-1;

	if(starty==endy && startx==endx) {

		ofs = starty*ibuf->x + startx;
		rect = (char *)(ibuf->rect +ofs);
		*rcol= ((float)rect[0])/255.0;
		*gcol= ((float)rect[1])/255.0;
		*bcol= ((float)rect[2])/255.0;
			/* alpha is globaal, reeds gezet in functie imagewraposa() */
		if(Talpha) {
			*acol= ((float)rect[3])/255.0;
		}
	}
	else {
		div= *rcol= *gcol= *bcol= *acol= 0.0;
		for(y=starty;y<=endy;y++) {
			ofs = y*ibuf->x +startx;
			rect = (char *)(ibuf->rect+ofs);

			muly= 1.0;

			if(starty==endy);
			else {
				if(y==starty) muly= 1.0-(rf->ymin - y);
				if(y==endy) muly= (rf->ymax - y);
			}
			if(startx==endx) {
				mulx= muly;
				if(Talpha) *acol+= mulx*rect[3];
				*rcol+= mulx*rect[0];
				*gcol+= mulx*rect[1];
				*bcol+= mulx*rect[2];
				div+= mulx;
			}
			else {
				for(x=startx;x<=endx;x++) {
					mulx= muly;
					if(x==startx) mulx*= 1.0-(rf->xmin - x);
					if(x==endx) mulx*= (rf->xmax - x);

					if(mulx==1.0) {
						if(Talpha) *acol+= rect[3];
						*rcol+= rect[0];
						*gcol+= rect[1];
						*bcol+= rect[2];
						div+= 1.0;
					}
					else {
						if(Talpha) *acol+= mulx*rect[3];
						*rcol+= mulx*rect[0];
						*gcol+= mulx*rect[1];
						*bcol+= mulx*rect[2];
						div+= mulx;
					}
					rect+=4;
				}
			}
		}
		if(div!=0.0) {
			div*= 255.0;
	
			*bcol/= div;
			*gcol/= div;
			*rcol/= div;
			
			if(Talpha) *acol/= div;
		}
		else {
			*rcol= *gcol= *bcol= *acol= 0.0;
		}
	}
}

void boxsample(ibuf, minx, miny, maxx, maxy, rcol, gcol, bcol, acol)	/* return kleur 0.0-1.0 */
struct ImBuf *ibuf;
float minx, miny, maxx, maxy;
float *rcol, *gcol, *bcol, *acol;
{
	/* Sample box, doet clip. minx enz lopen van 0.0 - 1.0 .
     * Vergroot uit met antialiased edges van de pixels.
     * Als global imaprepeat is gezet, worden
     *  de weggeclipte stukken ook gesampled.
     */
	rctf *rf, stack[8];
	float opp, tot, r, g, b, a, alphaclip= 1.0;
	short count=1;

	rf= stack;
	rf->xmin= minx*(ibuf->x);
	rf->xmax= maxx*(ibuf->x);
	rf->ymin= miny*(ibuf->y);
	rf->ymax= maxy*(ibuf->y);

	if(imapextend);
	else if(imaprepeat) clipx_rctf_swap(stack, &count, 0.0, (float)(ibuf->x));
	else {
		alphaclip= clipx_rctf(rf, 0.0, (float)(ibuf->x));

		if(alphaclip<=0.0) {
			*rcol= *bcol= *gcol= *acol= 0.0;
			return;
		}
	}

	if(imapextend);
	else if(imaprepeat) clipy_rctf_swap(stack, &count, 0.0, (float)(ibuf->y));
	else {
		alphaclip*= clipy_rctf(rf, 0.0, (float)(ibuf->y));

		if(alphaclip<=0.0) {
			*rcol= *bcol= *gcol= *acol= 0.0;
			return;
		}
	}

	if(count>1) {
		tot= *rcol= *bcol= *gcol= *acol= 0.0;
		while(count--) {
			boxsampleclip(ibuf, rf, &r, &g, &b, &a);
			
			opp= square_rctf(rf);
			tot+= opp;

			*rcol+= opp*r;
			*gcol+= opp*g;
			*bcol+= opp*b;
			if(Talpha) *acol+= opp*a;
			rf++;
		}
		if(tot!= 0.0) {
			*rcol/= tot;
			*gcol/= tot;
			*bcol/= tot;
			if(Talpha) *acol/= tot;
		}
	}
	else {
		boxsampleclip(ibuf, rf, rcol, gcol, bcol, acol);
	}

	if(Talpha==0) *acol= 1.0;
	
	if(alphaclip!=1.0) {
		/* this is for laer investigation, premul or not? */
		/* *rcol*= alphaclip; */
		/* *gcol*= alphaclip; */
		/* *bcol*= alphaclip; */
		*acol*= alphaclip;
	}
}	

void filtersample(ibuf, fx, fy, rcol, gcol, bcol, acol)   /* return kleur 0.0-1.0 */
struct ImBuf *ibuf;										/* fx en fy tussen 0.0 en 1.0 */
float fx, fy;
float *rcol, *gcol, *bcol, *acol;
{
	/* met weighted filter 3x3
     * de linker of rechter kolom is altijd 0
     * en de bovenste of onderste rij is altijd 0
     */

	int fac, fac1, fac2, fracx, fracy, filt[4], *poin= 0;
	int ix, iy, x4;
	uint r=0, g=0, b=0, a=0;
	char *rowcol, *rfilt[4];

	ix= (int)( 256.0*fx );
	fracx= (ix & 255);
	ix= (ix>>8);
	iy= (int)( 256.0*fy );
	fracy= (iy & 255);
	iy= (iy>>8);
	
	if(ix>=ibuf->x) ix= ibuf->x-1;
	if(iy>=ibuf->y) iy= ibuf->y-1;
	
	rowcol= (char *)(ibuf->rect+ iy*ibuf->x +ix);

	rfilt[0]= rfilt[1]= rfilt[2]= rfilt[3]= rowcol;
	x4= 4*ibuf->x;

	if(fracx<128) {
		if(ix>0) { 
			rfilt[0]-= 4; 
			rfilt[2]-=4; 
		}
		else if(imaprepeat) { 
			rfilt[0]+= x4-4; 
			rfilt[2]+= x4-4; 
		}

		if(fracy<128) {
			/* geval linksonder */
			fac1= 128+fracy;
			fac2= 128-fracy;

			if(iy>0) { 
				rfilt[3]-= x4; 
				rfilt[2]-= x4; 
			}
			else if(imaprepeat) {
				fac= x4*(ibuf->y-1) ;
				rfilt[3]+= fac; 
				rfilt[2]+= fac;
			}
		}
		else {
			/* geval linksboven */
			fac2= 384-fracy;
			fac1= fracy-128;

			if(iy<ibuf->y-1) { 
				rfilt[1]+= x4; 
				rfilt[0]+= x4; 
			}
			else if(imaprepeat) {
				fac= x4*(ibuf->y-1) ;
				rfilt[1]-= fac; 
				rfilt[0]-= fac;
			}
		}

		filt[1]=filt[3]= 128+ fracx;
		filt[0]=filt[2]= 128- fracx;
		filt[0]*= fac1; 
		filt[1]*= fac1;
		filt[2]*= fac2; 
		filt[3]*= fac2;
	}
	else {
		if(fracy<128) {
			/* geval rechtsonder */
			fac1= 128+fracy;
			fac2= 128-fracy;

			if(iy>0) { 
				rfilt[3]-= x4; 
				rfilt[2]-= x4; 
			}
			else if(imaprepeat) {
				fac= x4*(ibuf->y-1) ;
				rfilt[3]+= fac; 
				rfilt[2]+= fac;
			}
		}
		else {
			/* geval rechtsboven */
			fac2= 384-fracy;
			fac1= fracy-128;

			if(iy<ibuf->y-1) { 
				rfilt[1]+= x4; 
				rfilt[0]+= x4; 
			}
			else if(imaprepeat) {
				fac= x4*(ibuf->y-1) ;
				rfilt[1]-= fac; 
				rfilt[0]-= fac;
			}
		}
		filt[0]=filt[2]= 384-fracx;
		filt[1]=filt[3]= fracx-128;
		filt[0]*= fac1; 
		filt[1]*= fac1;
		filt[2]*= fac2; 
		filt[3]*= fac2;

		if(ix<ibuf->x-1) { 
			rfilt[1]+= 4; 
			rfilt[3]+=4; 
		}
		else if(imaprepeat) { 
			rfilt[1]-= x4-4; 
			rfilt[3]-= x4-4; 
		}
	}

	for(fac=3; fac>=0; fac--) {
		rowcol= rfilt[fac];
		r+= filt[fac]*rowcol[0];
		g+= filt[fac]*rowcol[1];
		b+= filt[fac]*rowcol[2];
		if(Talpha) a+= filt[fac]*rowcol[3];		/* alpha is globaal */
	}
	*rcol= ((float)r)/16777216.0;
	*gcol= ((float)g)/16777216.0;
	*bcol= ((float)b)/16777216.0;
	if(Talpha) *acol= ((float)a)/16777216.0;
	
}


int imagewraposa(Tex *tex, float *texvec, float *dxt, float *dyt)
{
	struct Image *ima;
	struct ImBuf *ibuf, *previbuf;
	float fx, fy, minx, maxx, miny, maxy, dx, dy, fac1, fac2, fac3, fac4;
	float maxd, pixsize, val1, val2, val3;
	int proj, a;
	int x, ok, curmap, areaflag= 0;
	
	ima= tex->ima;

	Tin= Ta= Tr= Tg= Tb= 0.0;

	if(ima==0 || ima->ok== 0) {
		return 0;
	}
	
	if(ima->ibuf==0) ima_ibuf_is_nul(tex);

	if (ima->ok) {
	
		if(tex->imaflag & TEX_MIPMAP) {
			if(ima->mipmap[0]==0) makemipmap(ima);
		}
	
		ibuf = ima->ibuf;
		
		if( (R.flag & R_SEC_FIELD) && (ibuf->flags & IB_fields) ) {
			ibuf->rect+= (ibuf->x*ibuf->y);
		}

		Talpha= 0;
		if(tex->imaflag & TEX_USEALPHA) {
			if(tex->imaflag & TEX_CALCALPHA);
			else Talpha= 1;
		}
		
		if(tex->imaflag & TEX_IMAROT) {
			fy= texvec[0];
			fx= texvec[1];
		}
		else {
			fx= texvec[0];
			fy= texvec[1];
		}
		
		
		if(ibuf->flags & IB_fields) {
			if(R.r.mode & R_FIELDS) {			/* field render */
				if(R.flag & R_SEC_FIELD) {		/* correctie voor tweede field */
					/* fac1= 0.5/( (float)ibuf->y ); */
					/* fy-= fac1; */
				}
				else {				/* eerste field */
					fac1= 0.5/( (float)ibuf->y );
					fy+= fac1;
				}
			}
		}
		
		/* pixel coordinaten */

		minx= MIN3(dxt[0],dyt[0],dxt[0]+dyt[0] );
		maxx= MAX3(dxt[0],dyt[0],dxt[0]+dyt[0] );
		miny= MIN3(dxt[1],dyt[1],dxt[1]+dyt[1] );
		maxy= MAX3(dxt[1],dyt[1],dxt[1]+dyt[1] );
		
		/* tex_sharper afgeschaft */
		#ifndef FREE		
		if(tex->imaflag & TEX_FILTX) {
			extern float Txtra;
			minx= Txtra*tex->filtersize*(maxx-minx)/2.0;
			miny= Txtra*tex->filtersize*(maxy-miny)/2.0;
		}
		else {
			minx= tex->filtersize*(maxx-minx)/2.0;
			miny= tex->filtersize*(maxy-miny)/2.0;
		}
		#else
		minx= tex->filtersize*(maxx-minx)/2.0;
		miny= tex->filtersize*(maxy-miny)/2.0;
		#endif
		
		if(tex->imaflag & TEX_IMAROT) SWAP(float, minx, miny);
		
		if(minx>0.25) minx= 0.25;
		else if(minx<0.00001) minx= 0.00001;	/* zijvlakken van eenheidskubus */
		if(miny>0.25) miny= 0.25;
		else if(miny<0.00001) miny= 0.00001;

		
		/* repeat en clip */
		
		/* let op: imaprepeat is globale waarde (zie boxsample) */
		imaprepeat= (tex->extend==TEX_REPEAT);
		imapextend= (tex->extend==TEX_EXTEND);


		if(tex->extend == TEX_CLIPCUBE) {
			if(fx+minx<0.0 || fy+miny<0.0 || fx-minx>1.0 || fy-miny>1.0 || texvec[2]<-1.0 || texvec[2]>1.0) {
				Tin= 0;
				return 0;
			}
		}
		else if(tex->extend == TEX_CLIP) {
			if(fx+minx<0.0 || fy+miny<0.0 || fx-minx>1.0 || fy-miny>1.0) {
				Tin= 0.0;
				return 0;
			}
		}
		else {
			if(tex->extend==TEX_EXTEND) {
				if(fx>1.0) fx = 1.0;
				else if(fx<0.0) fx= 0.0;
			}
			else {
				if(fx>1.0) fx -= (int)(fx);
				else if(fx<0.0) fx+= 1-(int)(fx);
			}
			
			if(tex->extend==TEX_EXTEND) {
				if(fy>1.0) fy = 1.0;
				else if(fy<0.0) fy= 0.0;
			}
			else {
				if(fy>1.0) fy -= (int)(fy);
				else if(fy<0.0) fy+= 1-(int)(fy);
			}
		}

		/* keuze:  */
		if(tex->imaflag & TEX_MIPMAP) {
			
			dx= minx;
			dy= miny;
			maxd= MAX2(dx, dy);
			if(maxd>0.5) maxd= 0.5;

			pixsize = 1.0 / (float) MIN2(ibuf->x, ibuf->y);
			
			curmap= 0;
			previbuf= ibuf;
			while(curmap<MAXMIPMAP && ima->mipmap[curmap]) {
				if(maxd < pixsize) break;
				previbuf= ibuf;
				ibuf= ima->mipmap[curmap];
				pixsize= 1.0 / (float)MIN2(ibuf->x, ibuf->y); /* hier stond 1.0 */		
				curmap++;
			}
			
			if(previbuf!=ibuf || (tex->imaflag & TEX_INTERPOL)) {
				/* minmaal 1 pixel sampelen */
				if (minx < 0.5 / ima->ibuf->x) minx = 0.5 / ima->ibuf->x;
				if (miny < 0.5 / ima->ibuf->y) miny = 0.5 / ima->ibuf->y;
			}
			
			if(tex->nor) {
				/* beetje extra filter */
				minx*= 1.35;
				miny*= 1.35;
				
				boxsample(ibuf, fx-2.0*minx, fy-2.0*miny, fx+minx, fy+miny, &Tr, &Tg, &Tb, &Ta);
				val1= Tr+Tg+Tb;
				boxsample(ibuf, fx-minx, fy-2.0*miny, fx+2.0*minx, fy+miny, &fac1, &fac2, &fac3, &fac4);
				val2= fac1+fac2+fac3;
				boxsample(ibuf, fx-2.0*minx, fy-miny, fx+minx, fy+2.0*miny, &fac1, &fac2, &fac3, &fac4);
				val3= fac1+fac2+fac3;
	
				if(previbuf!=ibuf) {  /* interpoleren */
					
					boxsample(previbuf, fx-2.0*minx, fy-2.0*miny, fx+minx, fy+miny, &fac1, &fac2, &fac3, &fac4);
					
					/* rgb berekenen */
					dx= 2.0*(pixsize-maxd)/pixsize;
					if(dx>=1.0) {
						Ta= fac4; Tb= fac3;
						Tg= fac2; Tr= fac1;
					}
					else {
						dy= 1.0-dx;
						Tb= dy*Tb+ dx*fac3;
						Tg= dy*Tg+ dx*fac2;
						Tr= dy*Tr+ dx*fac1;
						if(Talpha) Ta= dy*Ta+ dx*fac4;
					}
					
					val1= dy*val1+ dx*(fac1+fac2+fac3);
					boxsample(previbuf, fx-minx, fy-2.0*miny, fx+2.0*minx, fy+miny, &fac1, &fac2, &fac3, &fac4);
					val2= dy*val2+ dx*(fac1+fac2+fac3);
					boxsample(previbuf, fx-2.0*minx, fy-miny, fx+minx, fy+2.0*miny, &fac1, &fac2, &fac3, &fac4);
					val3= dy*val3+ dx*(fac1+fac2+fac3);
				}

				/* niet x en y verwisselen! */
				tex->nor[0]= (val1-val2);
				tex->nor[1]= (val1-val3);

			}
			else {
				maxx= fx+minx;
				minx= fx-minx;
				maxy= fy+miny;
				miny= fy-miny;
	
				boxsample(ibuf, minx, miny, maxx, maxy, &Tr, &Tg, &Tb, &Ta);
	
				if(previbuf!=ibuf) {  /* interpoleren */
					boxsample(previbuf, minx, miny, maxx, maxy, &fac1, &fac2, &fac3, &fac4);
					
					fx= 2.0*(pixsize-maxd)/pixsize;
					
					if(fx>=1.0) {
						Ta= fac4; Tb= fac3;
						Tg= fac2; Tr= fac1;
					} else {
						fy= 1.0-fx;
						Tb= fy*Tb+ fx*fac3;
						Tg= fy*Tg+ fx*fac2;
						Tr= fy*Tr+ fx*fac1;
						if(Talpha) Ta= fy*Ta+ fx*fac4;
					}
				}
			}
		}
		else {
			if((tex->imaflag & TEX_INTERPOL)) {
				/* minmaal 1 pixel sampelen */
				if (minx < 0.5 / ima->ibuf->x) minx = 0.5 / ima->ibuf->x;
				if (miny < 0.5 / ima->ibuf->y) miny = 0.5 / ima->ibuf->y;
			}

			if(tex->nor) {
				
				/* beetje extra filter */
				minx*= 1.35;
				miny*= 1.35;
				
				boxsample(ibuf, fx-2.0*minx, fy-2.0*miny, fx+minx, fy+miny, &Tr, &Tg, &Tb, &Ta);
				val1= Tr+Tg+Tb;

				boxsample(ibuf, fx-minx, fy-2.0*miny, fx+2.0*minx, fy+miny, &fac1, &fac2, &fac3, &fac4);
				val2= fac1+fac2+fac3;
				
				boxsample(ibuf, fx-2.0*minx, fy-miny, fx+miny, fy+2.0*miny, &fac1, &fac2, &fac3, &fac4);
				val3= fac1+fac2+fac3;

				/* niet x en y verwisselen! */
				tex->nor[0]= (val1-val2);
				tex->nor[1]= (val1-val3);
				
			}
			else {
				boxsample(ibuf, fx-minx, fy-miny, fx+minx, fy+miny, &Tr, &Tg, &Tb, &Ta);
			}

		}
		
		BRICONRGB;
		
		if(tex->imaflag & TEX_CALCALPHA) {
			Ta= Tin= MAX3(Tr, Tg, Tb);
		}
		else Tin= Ta;
		if(tex->flag & TEX_NEGALPHA) Ta= 1.0-Ta;
		
		if( (R.flag & R_SEC_FIELD) && (ibuf->flags & IB_fields) ) {
			ibuf->rect-= (ibuf->x*ibuf->y);
		}

	}
	else {
		Tin= 0.0;
		return 0;
	}
	
	if(tex->flag & TEX_COLORBAND) do_colorband(tex->coba);
	
	if(tex->nor) return 3;
	else return 1;
}

