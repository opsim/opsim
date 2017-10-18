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


/*  write_blendpsx.c   juli 96    MIXED MODEL!!!
 *
 *
 *			   totpacket:
 *
 *			   short type, nr_elems;
 *			   ... nr_elems*sizeof(type)
 *				
 *			   short type, nr_elems;
 *			   ... nr_elems*sizeof(type)
 *
 *			   ...
 */
 
/* in de PSX structs zit de data al LITTLE endian */
 

#include "blender.h"
#include "file.h"
#include "edit.h"
#include "sector.h"
#include "psx.h"
#include "blendpsx.h"

#define DYNAFIX		16384.0		
#define SECTORFIX	4096.0
#define COORDFIX	4096.0

/* ivm variabele sectorsize: max 8! */

/* 4096 is UNIT 1 */

short le_floatshort(float ftemp)
{
	int temp;
	short new;
	char *rt=(char *)&temp, *rtn=(char *)&new;
	
	temp= ffloor(4096.0*ftemp+0.5);
	CLAMP(temp, -32768, 32767);
	
	rtn[0]= rt[3];
	rtn[1]= rt[2];

	return new;
}

ushort le_floatushort(float ftemp)
{
	int temp;
	short new;
	char *rt=(char *)&temp, *rtn=(char *)&new;
	
	temp= ffloor(4096.0*ftemp+0.5);
	CLAMP(temp, 0, 0xFFFF);
	
	rtn[0]= rt[3];
	rtn[1]= rt[2];

	return new;
}

short le_dyna_short(float ftemp)
{
	int temp;
	short new;
	char *rt=(char *)&temp, *rtn=(char *)&new;
	
	temp= ffloor(DYNAFIX*ftemp+0.5);
	CLAMP(temp, -32768, 32767);
	
	rtn[0]= rt[3];
	rtn[1]= rt[2];

	return new;
}

int le_dyna_int(float ftemp)
{
	int temp;
	int new;
	char *rt=(char *)&temp, *rtn=(char *)&new;
	
	temp= ffloor(DYNAFIX*ftemp)+0.5;
	
	rtn[0]= rt[3];
	rtn[1]= rt[2];
	rtn[2]= rt[1];
	rtn[3]= rt[0];

	return new;
}

short le_floatangshort(float ftemp)
{
	int temp;
	short new;
	char *rt=(char *)&temp, *rtn=(char *)&new;
	
	temp= ffloor(4096.0*ftemp/(2.0*M_PI));
	if(temp > 0) temp= temp % 4096;
	else temp= - ( (abs(temp) % 4096));
	
	rtn[0]= rt[3];
	rtn[1]= rt[2];

	return new;
}

short le_float_dangshort(float ftemp)		/* voor delta's */
{
	int temp;
	short new;
	char *rt=(char *)&temp, *rtn=(char *)&new;
	
	temp= ffloor(4096.0*ftemp/(2.0*M_PI));
	
	rtn[0]= rt[3];
	rtn[1]= rt[2];

	return new;
}


/* van floatco naar integer coord */
short le_coordshort(float ftemp)
{
	int temp;
	short new;
	char *rt=(char *)&temp, *rtn=(char *)&new;
	
	temp= ffloor(COORDFIX*ftemp + 0.5);
	CLAMP(temp, -32768, 32767);
	
	rtn[0]= rt[3];
	rtn[1]= rt[2];

	return new;
}

short le_short(short temp)
{
	short new;
	char *rt=(char *)&temp, *rtn=(char *)&new;

	rtn[0]= rt[1];
	rtn[1]= rt[0];

	return new;
}


int le_int(int temp)
{
	int new;
	char *rt=(char *)&temp, *rtn=(char *)&new;

	rtn[0]= rt[3];
	rtn[1]= rt[2];
	rtn[2]= rt[1];
	rtn[3]= rt[0];

	return new;
}

/* van floatco naar integer coord */
int le_coordint(float ftemp)
{
	int temp;

	ftemp*= COORDFIX;
	temp= ffloor(ftemp+.5);

	return le_int(temp);
}


void write_p_data(int filecode, int len, void *adr, void *old)
{
	BHead bh;
	short *sp;
	
	if(adr==0) return;
	if(len==0) return;

	len+= 3;
	len-= ( len % 4);

	/* BHead vullen met data */
	bh.code= le_int(filecode);
	bh.old= old;
	bh.nr= le_int(1);
	bh.SDNAnr= 0;
	bh.len= le_int(len);
	
	mywrite(&bh, sizeof(BHead));
	if(len) mywrite(adr, len);
	
}

/* ******************* */

void write_p_ipos(ListBase *ipobase)
{
	pIpo *pipo, *ipo;
	float *fpoin, *firstloc;
	uint col;
	int a, b, len;
	short *sp, *psp, scol;
	
	ipo= ipobase->first;
	while(ipo) {
		
		if(ipo->type <= IP_FROMOB) {
			len= sizeof(pIpo) + ipo->nr_elems*ipo->elemsize*2 + 2*ipo->nr_keys;
			
			pipo= callocN(len, "data");
			
			pipo->type= le_short(ipo->type);
			pipo->nr_elems= le_short(ipo->nr_elems);
			pipo->elemsize= le_short(ipo->elemsize);
			pipo->nr_keys= le_short(ipo->nr_keys);
			pipo->sta= le_short(ipo->sta);
			
			firstloc= 0;
			
			a= ipo->nr_elems;
			fpoin= (float *)(ipo+1);
			sp= (short *)(pipo+1);
			while(a--) {
				
				if(ipo->type & IP_OBCOL) {
					for(b=0; b<3; b++) {
						scol= 255.0*fpoin[b];
						CLAMP(scol, 0, 255);
						sp[b]= le_short(scol);
					}
					fpoin+= 3; sp+= 3;
				}
				if(ipo->type & IP_LOC) {
				
					if(firstloc==0) {
						firstloc= fpoin;
						pipo->offs[0]= le_coordint(firstloc[0]);
						pipo->offs[1]= le_coordint(firstloc[1]);
						pipo->offs[2]= le_coordint(firstloc[2]);
					}
				
					for(b=0; b<3; b++) sp[b]= le_coordshort(fpoin[b] - firstloc[b] );
					fpoin+= 3; sp+= 3;
				}
				if(ipo->type & IP_ROT) {
					for(b=0; b<3; b++) sp[b]= le_floatangshort(fpoin[b]);
					fpoin+= 3; sp+= 3;
				}
				if(ipo->type & IP_SIZE) {
					for(b=0; b<3; b++) sp[b]= le_floatshort(fpoin[b]);
					fpoin+= 3; sp+= 3;
				}
			}
			
			a= ipo->nr_keys;
			psp= sp;
			sp= (short *)fpoin;
			while(a--) {
				*psp= le_short(*sp);
				psp++; sp++;
			}
			
			/* schrijf Data */
			write_p_data(DATA, len, pipo, ipo);
			
			freeN(pipo);
		}
		
		ipo= ipo->next;
	}
}

 

void number_images()
{
	Image *ima;
	TFace *tface;
	Mesh *me;
	int a;
	
	ima= G.main->image.first;
	while(ima) {
		ima->flag &= ~IMA_USED;
		ima= ima->id.next;
	}
	
	me= G.main->mesh.first;
	while(me) {
		tface= me->tface;
		if(tface) {
			for(a=me->totface; a>0; a--, tface++) {
				if(ima= tface->tpage) {
					ima->flag |= IMA_USED;
				}
			}
		}
		me= me->id.next;
	}
	
	a= 0;
	ima= G.main->image.first;
	while(ima) {
		if(ima->flag & IMA_USED) {
			/* ima->tpagenr= a++; */
		}
		ima= ima->id.next;
	}
}

void calc_texwin(TFace *tface, char *tw)
{
	Image *ima;
	ImBuf *ibuf;

	/* tw pointers zijn little endian shorts */

	ima= tface->tpage;
	if(ima) ibuf= ima->ibuf;
	
	tw[4]= tw[6]= 255;
	tw[0]= tw[2]= 0;
	
	if(ima==0 || (tface->mode & TF_TILES)==0) return;
	
	/* alleen 256 x 256 images!!! */
	
	tw[4]= 256/ima->xrep;
	tw[6]= 256/ima->yrep;
	
	tw[2]= (tface->tile /ima->xrep);
	tw[0]= tface->tile - tw[2]*ima->xrep;

	tw[0]*= tw[4];
	tw[2]*= tw[6];
	tw[2]= 255-tw[2];
	
}

void conv_uv_ar(short *uv_ar, short *uv)
{
	int midx, midy;
	
	midx= (uv[0]+ uv[2]+ uv[4]+ uv[6])/4;
	midy= (uv[1]+ uv[3]+ uv[5]+ uv[7])/4;
	
	if(uv[0] >= midx) uv_ar[0]= (uv[0]-64)>>7; else uv_ar[0]= (uv[0]+63)>>7;
	if(uv[2] >= midx) uv_ar[2]= (uv[2]-64)>>7; else uv_ar[2]= (uv[2]+63)>>7;
	if(uv[4] >= midx) uv_ar[4]= (uv[4]-64)>>7; else uv_ar[4]= (uv[4]+63)>>7;
	if(uv[6] >= midx) uv_ar[6]= (uv[6]-64)>>7; else uv_ar[6]= (uv[6]+63)>>7;
	
	if(uv[1] >= midy) uv_ar[1]= (uv[1]-64)>>7; else uv_ar[1]= (uv[1]+63)>>7;
	if(uv[3] >= midy) uv_ar[3]= (uv[3]-64)>>7; else uv_ar[3]= (uv[3]+63)>>7;
	if(uv[5] >= midy) uv_ar[5]= (uv[5]-64)>>7; else uv_ar[5]= (uv[5]+63)>>7;
	if(uv[7] >= midy) uv_ar[7]= (uv[7]-64)>>7; else uv_ar[7]= (uv[7]+63)>>7;
	
	uv_ar[1]= 255-uv_ar[1];
	uv_ar[3]= 255-uv_ar[3];
	uv_ar[5]= 255-uv_ar[5];
	uv_ar[7]= 255-uv_ar[7];
}

int packet_code(TFace *tface)
{
	int mode;
	
	if(tface->mode & TF_MASK) {
		if(tface->mode & TF_QUAD) mode= P_MASK4;
		else mode= P_MASK3;
	}
	else if(tface->mode & TF_BILLBOARD) {
		if(tface->mode & TF_OBCOL) mode= P_OBBILLB;
		else mode= P_BILLB;
	}
	else if(tface->mode & TF_SHADOW) {
		mode= P_SHADOW;
	}
	else if(tface->mode & TF_TWOSIDE) {
		if(tface->mode & TF_QUAD) mode= P_2GT4;
		else mode= P_2GT3;
	}
	else {
		mode= tface->mode & 7;
		if(tface->mode & TF_LIGHT) {
			mode |= 8;
			
			if(tface->mode & TF_SHAREDCOL) {
				if(		mode==P_LG3) mode= P_LPG3;
				else if(mode==P_LG4) mode= P_LPG4;
				else if(mode==P_LGT3) mode= P_LPGT3;
				else if(mode==P_LGT4) mode= P_LPGT4;
			}
		}
		else if(tface->mode & TF_OBCOL) {
			if(		mode==P_F3) mode= P_OBF3;
			else if(mode==P_F4) mode= P_OBF4;
			else if(mode==P_G3) mode= P_OBG3;
			else if(mode==P_G4) mode= P_OBG4;
			else if(mode==P_FT3) mode= P_OBFT3;
			else if(mode==P_FT4) mode= P_OBFT4;
			else if(mode==P_GT3) mode= P_OBGT3;
			else if(mode==P_GT4) mode= P_OBGT4;
		}
	}
	
	return mode;
}

/* ************* LEUKE DEFINES ****************** */

#define PRIMVERT3		pr->v1= (SVECTOR *)le_int( mface->v2); \
						pr->v2= (SVECTOR *)le_int( mface->v1); \
						pr->v3= (SVECTOR *)le_int( mface->v3);
#define PRIMVERT4		pr->v1= (SVECTOR *)le_int( mface->v4); \
						pr->v2= (SVECTOR *)le_int( mface->v3); \
						pr->v3= (SVECTOR *)le_int( mface->v1); \
						pr->v4= (SVECTOR *)le_int( mface->v2);
#define PRIMCOL			cp= (char *)tface->col; \
						pr->po.r0= cp[3]; \
						pr->po.g0= cp[2]; \
						pr->po.b0= cp[1]; \
						*( (int *)&pr->c0)= *( (int *)&pr->po.r0);

#define PRIMCOL3		cp= (char *)&tface->col[1]; \
						pr->po.r0= cp[3];	\
						pr->po.g0= cp[2];	\
						pr->po.b0= cp[1];	\
						*( (int *)&pr->c0)= *( (int *)&pr->po.r0); \
						cp-= 4;				\
						pr->po.r1= cp[3];	\
						pr->po.g1= cp[2];	\
						pr->po.b1= cp[1];	\
						*( (int *)&pr->c1)= *( (int *)&pr->po.r1); \
						cp+= 8;				\
						pr->po.r2= cp[3];	\
						pr->po.g2= cp[2];	\
						pr->po.b2= cp[1];	\
						*( (int *)&pr->c2)= *( (int *)&pr->po.r2);
						
#define PRIMCOL4		cp= (char *)&tface->col[3]; \
						pr->po.r0= cp[3];	\
						pr->po.g0= cp[2];	\
						pr->po.b0= cp[1];	\
						*( (int *)&pr->c0)= *( (int *)&pr->po.r0); \
						cp-= 4;				\
						pr->po.r1= cp[3];	\
						pr->po.g1= cp[2];	\
						pr->po.b1= cp[1];	\
						*( (int *)&pr->c1)= *( (int *)&pr->po.r1); \
						cp-= 8;				\
						pr->po.r2= cp[3];	\
						pr->po.g2= cp[2];	\
						pr->po.b2= cp[1];	\
						*( (int *)&pr->c2)= *( (int *)&pr->po.r2); \
						cp+= 4;				\
						pr->po.r3= cp[3];	\
						pr->po.g3= cp[2];	\
						pr->po.b3= cp[1];	\
						*( (int *)&pr->c3)= *( (int *)&pr->po.r3);
						
#define PRIMUV3			conv_uv_ar(uv_ar, tface->uv[0]);			\
						pr->po.u0= uv_ar[2]; pr->po.v0= uv_ar[3];	\
						pr->po.u1= uv_ar[0]; pr->po.v1= uv_ar[1];	\
						pr->po.u2= uv_ar[4]; pr->po.v2= uv_ar[5];

#define PRIMUV4			conv_uv_ar(uv_ar, tface->uv[0]);	\
						pr->po.u0= uv_ar[6]; pr->po.v0= uv_ar[7];	\
						pr->po.u1= uv_ar[4]; pr->po.v1= uv_ar[5];	\
						pr->po.u2= uv_ar[0]; pr->po.v2= uv_ar[1];	\
						pr->po.u3= uv_ar[2]; pr->po.v3= uv_ar[3];

						
#define PRIMNOR			pr->no.vx= le_short(tface->no[0]>>3); \
						pr->no.vy= le_short(tface->no[1]>>3); \
						pr->no.vz= le_short(tface->no[2]>>3);

#define PRIMTEX			pr->po.clut= 0;		\
						pr->po.tpage= 0;	\
						/* if(tface->tpage) pr->po.tpage= le_short( ((Image *)tface->tpage)->tpagenr );\ */ \
						calc_texwin(tface, (char *)pr->dr.code);

int tot_dyna=0, tot_packet=0, tot_prim= 0, tot_ob=0, tot_life=0;

void make_packetdata(pMesh *pme, Mesh *me)
{
	TFace *tface;
	MFace *mface;
	MVert *mvert;
	float vec[3];
	int a, p, datalen=0, totpacket= 0, mode;
	short type_ar[64], uv_ar[8];
	short *sp, *data, *packetdata;
	char *cp;
	
	/* totaal aantal mogelijke packets: aantal poly types */
	bzero(type_ar, 2*64);
	
	tface= me->tface;
	mface= me->mface;
	mvert= me->mvert;
	if(tface==0) return;
	
	/* welke packets */
	for(a=me->totface; a>0; a--, tface++, mface++) {
		if(mface->v3) {
			
			if(mface->v4) tface->mode |= TF_QUAD; else  tface->mode &= ~TF_QUAD;
			type_ar[ packet_code(tface) ]++;
		}
	}
	
	pme->polysize= 0;	/* voor PSX voor doublebuf test */
	
	for(p=0; p<63; p++) {
		if(type_ar[p]==0) continue;
		
		totpacket++;
		datalen+= 4;	/* short type, short nr_elems */

		switch(p) {
		case P_F3:
		case P_OBF3:
			pme->polysize+= type_ar[p]*sizeof(POLY_F3);
			datalen+= type_ar[p] * sizeof(PrimF3); break;
		case P_F4:
		case P_OBF4:
			pme->polysize+= type_ar[p]*sizeof(POLY_F4);
			datalen+= type_ar[p] * sizeof(PrimF4); break;
			
		case P_LF3:
			pme->polysize+= type_ar[p]*sizeof(POLY_F3);
			datalen+= type_ar[p] * sizeof(PrimLF3); break;
		case P_LF4:
			pme->polysize+= type_ar[p]*sizeof(POLY_F4);
			datalen+= type_ar[p] * sizeof(PrimLF4); break;
		
		case P_G3:
		case P_OBG3:
			pme->polysize+= type_ar[p]*sizeof(POLY_G3);
			datalen+= type_ar[p] * sizeof(PrimG3); break;
		case P_G4:
		case P_OBG4:
			pme->polysize+= type_ar[p]*sizeof(POLY_G4);
			datalen+= type_ar[p] * sizeof(PrimG4); break;
			
		case P_LPG3:
			pme->flag |= ME_PUNO;
		case P_LG3:
			pme->polysize+= type_ar[p]*sizeof(POLY_G3);
			datalen+= type_ar[p] * sizeof(PrimLG3); break;
		case P_LPG4:
			pme->flag |= ME_PUNO;
		case P_LG4:
			pme->polysize+= type_ar[p]*sizeof(POLY_G4);
			datalen+= type_ar[p] * sizeof(PrimLG4); break;
		
		case P_MASK3:
		case P_FT3:
		case P_OBFT3:
			pme->polysize+= type_ar[p]*sizeof(POLY_FT3);
			datalen+= type_ar[p] * sizeof(PrimFT3); break;
		case P_SHADOW:
		case P_MASK4:
		case P_FT4:
		case P_OBFT4:
			pme->polysize+= type_ar[p]*sizeof(POLY_FT4);
			datalen+= type_ar[p] * sizeof(PrimFT4); break;
			
		case P_LFT3:
			pme->polysize+= type_ar[p]*sizeof(POLY_FT3);
			datalen+= type_ar[p] * sizeof(PrimLFT3); break;
		case P_LFT4:
			pme->polysize+= type_ar[p]*sizeof(POLY_FT4);
			datalen+= type_ar[p] * sizeof(PrimLFT4); break;
		
		case P_2GT3:
		case P_OBGT3:
		case P_GT3:
			pme->polysize+= type_ar[p]*sizeof(POLY_GT3);
			datalen+= type_ar[p] * sizeof(PrimGT3); break;
		case P_2GT4:
		case P_OBGT4:
		case P_GT4:
			pme->polysize+= type_ar[p]*sizeof(POLY_GT4);
			datalen+= type_ar[p] * sizeof(PrimGT4); break;
			
		case P_LPGT3:
			pme->flag |= ME_PUNO;
		case P_LGT3:
			pme->polysize+= type_ar[p]*sizeof(POLY_GT3);
			datalen+= type_ar[p] * sizeof(PrimLGT3); break;
		case P_LPGT4:
			pme->flag |= ME_PUNO;
		case P_LGT4:
			pme->polysize+= type_ar[p]*sizeof(POLY_GT4);
			datalen+= type_ar[p] * sizeof(PrimLGT4); break;
			
		
		case P_OBBILLB:
		case P_BILLB:
			pme->polysize+= type_ar[p]*sizeof(POLY_FT4);
			datalen+= type_ar[p] * sizeof(PrimBillb); break;
		}
	}
	
	if(datalen==0) return;
tot_packet+= datalen;

	data=packetdata= callocN(datalen, "packet");
	
	for(p=0; p<63; p++) {
		if(type_ar[p]==0) continue;

tot_prim+= type_ar[p];
		
		data[0]= le_short(  p );
		data[1]= le_short(type_ar[p]);
		
		data+= 2;
		
		tface= me->tface;
		mface= me->mface;
		for(a=me->totface; a>0; a--, tface++, mface++) {
			
			if( mface->v3 && p==packet_code(tface) ) {

				switch(p) {
		/* GL_FLAT */				
				case P_F3:
				case P_OBF3:
					{
						PrimF3 *pr= (PrimF3 *)data;
						data= (short *)(pr+1);

						setPolyF3(&pr->po);	/* geen le_! */
						
						PRIMCOL
						PRIMVERT3
					}
					break;	
				case P_F4:
				case P_OBF4:
					{
						PrimF4 *pr= (PrimF4 *)data;
						data= (short *)(pr+1);
						
						setPolyF4(&pr->po);	/* geen le_! */
						
						PRIMCOL
						PRIMVERT4
					}
					break;	

				case P_LF3:
					{
						PrimLF3 *pr= (PrimLF3 *)data;
						data= (short *)(pr+1);

						setPolyF3(&pr->po);	/* geen le_! */
						
						PRIMNOR
						PRIMCOL
						PRIMVERT3
					}
					break;	
				case P_LF4:
					{
						PrimLF4 *pr= (PrimLF4 *)data;
						data= (short *)(pr+1);
						
						setPolyF4(&pr->po);	/* geen le_! */
						
						PRIMNOR
						PRIMCOL
						PRIMVERT4
					}
					break;
		/* GOUR */				
				case P_G3:
				case P_OBG3:
					{
						PrimG3 *pr= (PrimG3 *)data;
						data= (short *)(pr+1);

						setPolyG3(&pr->po);	/* geen le_! */
						
						PRIMCOL3
						PRIMVERT3
					}
					break;	
				case P_G4:
				case P_OBG4:
					{
						PrimG4 *pr= (PrimG4 *)data;
						data= (short *)(pr+1);
						
						setPolyG4(&pr->po);	/* geen le_! */
						
						PRIMCOL4
						PRIMVERT4
					}
					break;	

				case P_LPG3:
				case P_LG3:
					{
						PrimLG3 *pr= (PrimLG3 *)data;
						data= (short *)(pr+1);

						setPolyG3(&pr->po);	/* geen le_! */
						
						PRIMNOR
						PRIMCOL3
						PRIMVERT3
					}
					break;
				case P_LPG4:
				case P_LG4:
					{
						PrimLG4 *pr= (PrimLG4 *)data;
						data= (short *)(pr+1);
						
						setPolyG4(&pr->po);	/* geen le_! */
						
						PRIMNOR
						PRIMCOL4
						PRIMVERT4
					}
					break;	
	/* GL_FLAT TEX */
				case P_FT3:
				case P_MASK3:
				case P_OBFT3:
					{
						PrimFT3 *pr= (PrimFT3 *)data;
						data= (short *)(pr+1);
						
						setPolyFT3(&pr->po);	/* geen le_! */
						
						PRIMCOL
						PRIMVERT3
						PRIMUV3
						PRIMTEX
						
						pr->po.pad1= le_short( getTPage(0, (tface->flag>>8), 0, 0) );
						/* if(tface->mode & TF_TRANSP) setSemiTrans( &pr->po, 1); */
						pr->c0.cd= pr->po.code;		/* voor colorcol en dpq */
					}
					break;
				case P_FT4:
				case P_SHADOW:
				case P_MASK4:
				case P_OBFT4:
					{
						PrimFT4 *pr= (PrimFT4 *)data;
						data= (short *)(pr+1);
						
						setPolyFT4(&pr->po);	/* geen le_! */
						
						PRIMCOL
						PRIMVERT4
						PRIMUV4
						PRIMTEX
						
						pr->po.pad2= le_short( getTPage(0, (tface->flag>>8), 0, 0) );
						/* if(tface->mode & TF_TRANSP) setSemiTrans( &pr->po, 1); */
						pr->c0.cd= pr->po.code;		/* voor colorcol en dpq */
					}
					break;

				case P_LFT3:
					{
						PrimLFT3 *pr= (PrimLFT3 *)data;
						data= (short *)(pr+1);
						
						setPolyFT3(&pr->po);	/* geen le_! */
						
						PRIMNOR
						PRIMCOL
						PRIMVERT3
						PRIMUV3
						PRIMTEX
						
						pr->po.pad1= le_short( getTPage(0, (tface->flag>>8), 0, 0) );
						/* if(tface->mode & TF_TRANSP) setSemiTrans( &pr->po, 1); */
						pr->c0.cd= pr->po.code;		/* voor colorcol en dpq */
					}
					break;
				case P_LFT4:
					{
						PrimLFT4 *pr= (PrimLFT4 *)data;
						data= (short *)(pr+1);
						
						setPolyFT4(&pr->po);	/* geen le_! */
						
						PRIMNOR
						PRIMCOL
						PRIMVERT4
						PRIMUV4
						PRIMTEX
						
						pr->po.pad2= le_short( getTPage(0, (tface->flag>>8), 0, 0) );
						/* if(tface->mode & TF_TRANSP) setSemiTrans( &pr->po, 1); */
						pr->c0.cd= pr->po.code;		/* voor colorcol en dpq */
					}
					break;
	/* GOUR TEX */
				case P_2GT3:
				case P_OBGT3:
				case P_GT3:
					{
						PrimGT3 *pr= (PrimGT3 *)data;
						data= (short *)(pr+1);
						
						setPolyGT3(&pr->po);	/* geen le_! */
						
						PRIMCOL3
						PRIMVERT3
						PRIMUV3
						PRIMTEX
						
						pr->po.pad2= le_short( getTPage(0, (tface->flag>>8), 0, 0) );
						/* if(tface->mode & TF_TRANSP) setSemiTrans( &pr->po, 1); */
						pr->c0.cd= pr->po.code;		/* voor colorcol en dpq */
					}
					break;
				case P_2GT4:
				case P_OBGT4:
				case P_GT4:
					{
						PrimGT4 *pr= (PrimGT4 *)data;
						data= (short *)(pr+1);
						
						setPolyGT4(&pr->po);	/* geen le_! */
						
						PRIMCOL4
						PRIMVERT4
						PRIMUV4
						PRIMTEX
						
						pr->po.pad2= le_short( getTPage(0, (tface->flag>>8), 0, 0) );
						/* if(tface->mode & TF_TRANSP) setSemiTrans( &pr->po, 1); */
						pr->c0.cd= pr->po.code;		/* voor colorcol en dpq */
					}
					break;

				case P_LPGT3:
				case P_LGT3:
					{
						PrimLGT3 *pr= (PrimLGT3 *)data;
						data= (short *)(pr+1);
						
						setPolyGT3(&pr->po);	/* geen le_! */
						
						PRIMNOR
						PRIMCOL3
						PRIMVERT3
						PRIMUV3
						PRIMTEX
						
						pr->po.pad2= le_short( getTPage(0, (tface->flag>>8), 0, 0) );
						if(tface->mode & TF_TRANSP) setSemiTrans( &pr->po, 1);
						pr->c0.cd= pr->po.code;		/* voor colorcol en dpq */
					}
					break;
				case P_LPGT4:
				case P_LGT4:
					{
						PrimLGT4 *pr= (PrimLGT4 *)data;
						data= (short *)(pr+1);
						
						setPolyGT4(&pr->po);	/* geen le_! */
						
						PRIMNOR
						PRIMCOL4
						PRIMVERT4
						PRIMUV4
						PRIMTEX
						
						pr->po.pad2= le_short( getTPage(0, (tface->flag>>8), 0, 0) );
						if(tface->mode & TF_TRANSP) setSemiTrans( &pr->po, 1);
						pr->c0.cd= pr->po.code;		/* voor colorcol en dpq */
					}
					break;
	/* SPECIALS */
				case P_OBBILLB:
				case P_BILLB:
					{
						PrimBillb *pr= (PrimBillb *)data;
						data= (short *)(pr+1);
						
						setPolyFT4(&pr->po);	/* geen le_! */
						
						PRIMCOL
						PRIMUV4
						PRIMTEX
						
						pr->po.pad2= le_short( getTPage(0, (tface->flag>>8), 0, 0) );
						if(tface->mode & TF_TRANSP) setSemiTrans( &pr->po, 1);

						CalcCent4f(vec, (mvert+mface->v1)->co, (mvert+mface->v2)->co, 
							(mvert+mface->v3)->co, (mvert+mface->v4)->co );
						
						pr->cent.vx= le_coordshort( vec[0] );
						pr->cent.vy= le_coordshort( vec[1] );
						pr->cent.vz= le_coordshort( vec[2] );
						/* pad==size : 0.5/wortel2 */
						pr->cent.pad= le_coordshort( 0.36*VecLenf(vec, (mvert+mface->v1)->co) );
						
						
					}
					break;
		
				}
			}
		}
	}

	pme->packetdata= packetdata;
	pme->totpacket= le_short(totpacket);
	pme->totface= le_short(me->totface);
	pme->polysize= le_int(pme->polysize);
	/* pme->flag mee wachten! */
}



void make_d_mvertdata(pMesh *pme, Mesh *me)
{
	MVert *mvert;
	int a;
	short *sp;
	
	a= me->totvert;
	if(a==0) return;
	sp=pme->mvert= mallocN(8*a, "pmvert");
	mvert= me->mvert;

tot_dyna+= 8*a;
	
	for(; a>0; a--, sp+=4, mvert++) {
		sp[0]= le_floatshort(mvert->co[0]);
		sp[1]= le_floatshort(mvert->co[1]);
		sp[2]= le_floatshort(mvert->co[2]);
	}
}

void make_mvertdata(pMesh *pme, Mesh *me)
{
	MVert *mvert;
	int a;
	short *sp;
	
	a= me->totvert;
	if(a==0) return;
	sp=pme->mvert= mallocN(8*a, "pmvert");
	mvert= me->mvert;
	
	for(; a>0; a--, sp+=4, mvert++) {
		sp[0]= le_coordshort(mvert->co[0]);
		sp[1]= le_coordshort(mvert->co[1]);
		sp[2]= le_coordshort(mvert->co[2]);
	}
}

void make_ocdata(pMesh *pme, Mesh *me)
{
	OcInfo *oc;
	pOcInfo *poc;

	if(me->oc==0) return;
	poc= pme->oc= mallocN(sizeof(pOcInfo), "ocpsx");

	poc->dvec[0]= le_floatshort(me->oc->dvec[0]);
	poc->dvec[1]= le_floatshort(me->oc->dvec[1]);
	poc->dvec[2]= le_floatshort(me->oc->dvec[2]);
	
	/* size is 'absolute' size: USHORT */
	poc->size[0]= le_floatushort(me->oc->size[0]);
	poc->size[1]= le_floatushort(me->oc->size[1]);
	poc->size[2]= le_floatushort(me->oc->size[2]);

}

void make_mvert_punodata(pMesh *pme, Mesh *me)
{
	MVert *mvert;
	int a;
	short *sp;
	
	a= me->totvert;
	if(a==0) return;
	sp=pme->mvert= mallocN(16*a, "pmvert");
	mvert= me->mvert;
	
	for(; a>0; a--, sp+=8, mvert++) {
		sp[0]= le_coordshort(mvert->co[0]);
		sp[1]= le_coordshort(mvert->co[1]);
		sp[2]= le_coordshort(mvert->co[2]);
		sp[4]= le_short(mvert->no[0]>>3);
		sp[5]= le_short(mvert->no[1]>>3);
		sp[6]= le_short(mvert->no[2]>>3);
	}
}

short *mvertco_to_index(MVert *first, float *v1)
{
	int adr;
	
	if(v1==0) return (short *)-1;
	
	adr= (int)v1 - (int)first;
	adr/= sizeof(MVert);

	return (short *)le_int(adr);
}

void make_dfaces(pMesh *pme, Mesh *me)
{
	DFace *dface;
	pDFace *pdface;
	int a;
	
	a= me->totface;
	if(a==0 || me->dface==0) return;
	dface= me->dface;
	pme->dface=pdface= callocN(a*sizeof(pDFace), "pdface");

tot_dyna+= a*sizeof(pDFace);

	for(; a>0; a--, dface++, pdface++) {
		
		/* LET OP: ivm switch_dir_dface: index==0 kan ook op plek 3 voorkomen, 
			daarom index -1 : geen vertex */
		
		pdface->v1= mvertco_to_index(me->mvert, dface->v1);
		pdface->v2= mvertco_to_index(me->mvert, dface->v2);
		pdface->v3= mvertco_to_index(me->mvert, dface->v3);
		pdface->v4= mvertco_to_index(me->mvert, dface->v4);

		pdface->dist= le_dyna_int(DYNAFIX*dface->dist);
		pdface->ma= (void *)dface->ma;
		if(dface->ma) dface->ma->id.flag |= LIB_DOIT;	/* schrijfflag */
		
		pdface->no[0]= le_dyna_short(dface->no[0]);
		pdface->no[1]= le_dyna_short(dface->no[1]);
		pdface->no[2]= le_dyna_short(dface->no[2]);
		
		pdface->proj= le_short(dface->proj);

		pdface->ocx= le_short(dface->ocx);
		pdface->ocy= le_short(dface->ocy);
		pdface->ocz= le_short(dface->ocz);

		pdface->edcode= (dface->edcode);	/* chars */
		pdface->flag= (dface->flag);
	}
}

void convertID(pID *new,  ID *old)
{

	new->next= old->next;
	new->prev= old->prev;
	new->lib= (void *)old->lib;
	*((long *)new->name)= *((long *)old->name);	
	new->us= le_short(old->us);
	new->flag= le_short(old->flag);
}

short sensor_decode(event)
{
	
	if(event<3) event= 0;

	return le_short(event);
}

void write_p_lifes(ListBase *idbase)
{
	Life *lf;
	pLife plf;
	Sensor *sn;
	pSensor *psn, *psnmain;
	Event *ev;
	pEvent *pev, *pevmain;
	Action *ac;
	pAction *pac, *pacmain;
	int a, b, val;
	
	lf= idbase->first;
	while(lf) {
		if(lf->id.us>0) {
		
tot_life++;
			/* converteren */
			bzero(&plf, sizeof(pLife));
			convertID(&plf.id, &lf->id);
			
			plf.type= lf->type;
			plf.lay= lf->lay;
			plf.flag= le_short(lf->flag);
			plf.totsens= le_short(lf->totsens);
			plf.mass= le_floatshort(lf->mass);
			plf.frict= le_floatshort( lf->frict );
			plf.frictfac= le_floatshort(lf->frictfac);
			plf.rotfrict= le_floatshort( lf->rotfrict );
			plf.sensors= (void *)(lf->sensors);
			plf.sector= (void *)(lf->sector);
			plf.sfra= le_short( 2*lf->sfra);
			plf.cfra= le_short(lf->cfra);
			if(lf->axsize<=0.01) plf.axsize= le_floatshort(0.01);
			else plf.axsize= le_floatshort(lf->axsize);
			
			plf.links.max= le_short(lf->links.max);
			plf.links.tot= le_short(lf->links.tot);
			plf.links.ob= (void *)lf->links.ob;
			
			plf.dynamesh= (void *)lf->dynamesh;
			plf.texmesh= (void *)lf->texmesh;
			plf.ipo= lf->ipo;	/* listbase */
			
			/* schrijf LibData */
			write_p_data(ID_LF, sizeof(pLife), &plf, lf);
			
			write_p_ipos(&lf->ipo);
			
			if(lf->links.tot) {
				write_p_data(DATA, lf->links.tot*4, lf->links.ob, lf->links.ob);
			}

			if(lf->totsens) {
				/* converteren */
				psn=psnmain= callocN(lf->totsens*sizeof(pSensor), "sensor");
				sn= lf->sensors;
				
				for(a= 0; a<lf->totsens; a++, psn++, sn++) {
					/* converteren */
					
					psn->totaction= le_short(sn->totaction);
					psn->totevent= le_short(sn->totevent);
					psn->events= (void *)sn->events;		/* dit moet: anders werkt newadr niet. Dus niet malloccen en vrijgeven en dat adres gebruiken! */
					psn->actions= (void *)sn->actions;
					psn->flag= le_short(sn->flag);
					
					b= sn->totevent;
					ev= sn->events;
					pev= pevmain= callocN(sn->totevent*sizeof(pEvent), "event");
					while(b--) {
						
						pev->event= sensor_decode(ev->event);
						pev->var= le_short(ev->var);
						pev->shiftval= le_short(ev->shiftval);
						pev->flag= ev->flag;
						pev->pulse= ev->pulse;
						pev->sfac= le_short(ev->sfac);
						pev->sfac1= le_short(ev->sfac1);
						
						if(ev->event==SN_NEAR) {
							pev->sfac= le_coordshort(ev->fac);
							pev->sfac1= le_coordshort(ev->fac1);
						}
						if(ev->poin) {
							if ELEM3(ev->event, SN_VAR_EQUAL, SN_VAR_INTERVAL, SN_VAR_CHANGED) {
								pev->poin= (void *)le_int( ev->varnr );
							}
							else pev->poin= ev->poin;
						}
						pev++;
						ev++;
					}
					
					write_p_data(DATA, sn->totevent*sizeof(pEvent), pevmain, psn->events);
					freeN(pevmain);
					
					b= sn->totaction;
					ac= sn->actions;
					pac= pacmain= callocN(sn->totaction*sizeof(pAction), "action");
					while(b--) {

						pac->action= le_short(ac->action);
						pac->go= le_short(ac->go);
						pac->cur= le_short(ac->cur);
						pac->var= le_short(ac->var);
						pac->sta= le_short(ac->sta);
						pac->end= le_short(ac->end);
						pac->flag= le_short(ac->flag);
						pac->fac= le_floatshort(ac->fac);
						
						/* proefondervindelijk: 2.6 */
						if ELEM3(ac->action, SN_X_ROT, SN_Y_ROT, SN_Z_ROT) {
							pac->force= le_dyna_short( 2.6*DTIME*ac->force);
							
							pac->min= le_float_dangshort(ac->min);
							pac->max= le_float_dangshort(ac->max);
						}
						else 
							pac->force= le_dyna_short( DTIME*ac->force);

						if(ac->action==SN_GOTO) {
							pac->force= le_coordshort( ac->force);
							pac->min= le_coordshort( ac->min);
							pac->max= le_coordshort( ac->max);
							pac->poin= ac->poin;
						}
						else if ELEM(ac->action, SN_LOADFILE, SN_PLAYMOVIE) {
							pac->poin= ac->name;
							write_p_data(DATA, 16, ac->name, ac->name);
						}
						else if(ac->action>=100 && ac->action<200) {	/* ipos */
							if(ac->poin && (ac->flag & SN_IPO_SETVAR)) {
								pac->poin= (void *)le_int( ac->varnr );
							}
						}
						else if(ac->action>=200 && ac->action<300) {	/* divers */
							if(ac->action==SN_ROBBIE_M) {
								pac->poin= ac->poin;
								pac->force= le_floatshort(ac->force);	/* h */
								pac->min= le_floatshort(ac->min);	/* dist */
								pac->max= le_floatshort(ac->max);	/* dist */
							}
							else pac->poin= ac->poin;
						}
						else if(ac->action>=300 && ac->action<400) {
							if(ac->poin) {
								/* le_int !!! */
								/* negatief: lokaal, positief: actvarar */
								pac->poin= (void *)le_int( ac->varnr );
							}
							
						}
						else pac->poin= ac->poin;
						
						pac++;
						ac++;
					}
					
					write_p_data(DATA, sn->totaction*sizeof(pAction), pacmain, psn->actions);
					freeN(pacmain);	
				}
				write_p_data(DATA, lf->totsens*sizeof(pSensor), psnmain, lf->sensors);
				freeN(psnmain);
			}
			
		}
		lf= lf->id.next;
	}
	
}


void write_p_sectors(ListBase *idbase)
{
	Sector *se;
	pSector pse;
	Portal *po;
	pPortal *ppo, *ppmain;
	CamPos *capo;
	pCamPos *pcapo, *pcapomain;
	int a, b;
	char str[24];
	
	se= idbase->first;
	while(se) {
		if(se->id.us>0) {
			
			/* converteren */
			bzero(&pse, sizeof(pSector));
			convertID(&pse.id, &se->id);
			
			pse.type= le_short(se->type);
			pse.flag= le_short(se->flag);
			pse.totport= le_short(se->totport);
			pse.totcam= le_short(se->totcam);
			pse.actport= le_short(se->actport);
			pse.portals= (void *)se->portals;
			pse.campos= (void *)se->campos;
			if(se->campos) pse.name= se->id.name+2;
			pse.dynamesh= (void *)se->dynamesh;
			pse.texmesh= (void *)se->texmesh;
			
			pse.lbuf.max= le_short(se->lbuf.max);
			pse.lbuf.tot= le_short(se->lbuf.tot);
			pse.lbuf.ob= (void *)se->lbuf.ob;
			
			pse.ipo= se->ipo;	/* listbase */
			
			for(a=0; a<3; a++) {
				pse.size[a]= le_floatshort(se->size[a]);
				pse.bbsize[a]= le_coordshort(se->bbsize[a]);
			}
			
			/* schrijf LibData */

			write_p_data(ID_SE, sizeof(pSector), &pse, se);
			
			write_p_ipos(&se->ipo);
			
			if(se->totport) {
				/* converteren */
				ppo=ppmain= callocN(se->totport*sizeof(pPortal), "port");
				po= se->portals;

				for(a= 0; a<se->totport; a++, ppo++, po++) {
					ppo->sector= (void *)po->sector;

					ppo->type= le_short(po->type);
					ppo->flag= le_short(po->flag);

				}
				write_p_data(DATA, se->totport*sizeof(pPortal), ppmain, se->portals);
				freeN(ppmain);
			}
			
			if(se->lbuf.tot) {
				write_p_data(DATA, se->lbuf.tot*4, se->lbuf.ob, se->lbuf.ob);
			}
			
			if(se->campos) {
				/* converteren */
				pcapo= pcapomain= callocN(se->totcam*sizeof(pCamPos), "capo");
				capo= se->campos;
				
				for(a=0; a<se->totcam; a++, capo++, pcapo++) {
					pcapo->ob= (void *)capo->ob;
					pcapo->flag= le_short(capo->flag);
					pcapo->hold= le_short(capo->hold);
					pcapo->sfra= le_short(capo->sfra);
					pcapo->sta= le_coordint(capo->sta/4.0);	/* correctie voor aangepaste piramat */
					pcapo->end= le_coordint(capo->end/4.0);
					
					pcapo->lens= le_short( (int)(2.5*capo->lens) );	/* tijdelijk totdat hier piramat voorberekend wordt */
				}
				write_p_data(DATA, se->totcam*sizeof(pCamPos), pcapomain, se->campos);
				freeN(pcapomain);
				
				/* moviename  */
				if(G.scene->flag & SCE_ADDSCENAME) {
					strncpy(str, G.scene->id.name+2, 8);
					strcat(str, "/");
					strcat(str, se->id.name+2);
				}
				else strcpy(str, se->id.name+2);
				
				write_p_data(DATA, 20, str, se->id.name+2);
				
			}
		}
		se= se->id.next;
	}
}


void write_p_scenes(ListBase *scebase)
{
	Scene *sce;
	Camera *cam;
	pScene psce;
	Base *base;
	pBase pbase;

	bzero(&pbase, sizeof(pBase));

	sce= scebase->first;
	while(sce) {

		bzero(&psce, sizeof(pScene));
		
		/* converteren */
		convertID(&psce.id, &sce->id);
	
		psce.camera= (void *)sce->camera;
		psce.set= (void *)sce->set;

		psce.lay= le_int(sce->lay);
		psce.base= sce->base;	/* listbase */
		psce.grav= le_dyna_short( DTIME*sce->grav);
		psce.maxdrawsector= le_short(sce->maxdrawsector);
		if(sce->camera) {
			cam= sce->camera->data;
			psce.lens= le_short( (int)(2.5*cam->lens) );
		}
		else psce.lens= 92;
		
		if(G.scene->world) {
			psce.mist= (G.scene->world->mode & WO_MIST);
			psce.mir= 255.0*G.scene->world->horr;
			psce.mig= 255.0*G.scene->world->horg;
			psce.mib= 255.0*G.scene->world->horb;
		}
		
		/* schrijf LibData */
		write_p_data(ID_SCE, sizeof(pScene), &psce, sce);
		
		/* alle direkte data: baselijst */
		base= sce->base.first;
		while(base) {
			
			/* voor zekerheid: straks set_scene doen op PSX */
			base->object->lay= base->lay;

			/* converteren */
			pbase.prev= (void *)base->prev;
			pbase.next= (void *)base->next;
			pbase.lay= le_int(base->lay);
			pbase.flag= le_int(base->flag);
			
			if ELEM4(base->object->type, OB_MESH, OB_SECTOR, OB_LIFE, OB_CAMERA) 
				pbase.object= (void *)base->object;
			else pbase.object= 0;
			
			write_p_data(DATA, sizeof(pBase), &pbase, base);

			base= base->next;
		}
		
		sce= sce->id.next;
	}
}

#define CODEVEC(a)	(a[0]==4096?0 : (a[1]==4096?1 : (a[2]==4096?2 : (a[0]==-4096?3 : (a[1]==-4096?4 : (a[2]==-4096?5 :6))))))

void convert_matrix(m1, m2)
MATRIX *m1;
float m2[][4];
{
	short cox, coy, coz;
	
	m1->flag= 0;

	m1->m[0][0]= ffloor( (4096.0*m2[0][0]) + 0.5 );
	m1->m[0][1]= ffloor( (4096.0*m2[0][1]) + 0.5 );
	m1->m[0][2]= ffloor( (4096.0*m2[0][2]) + 0.5 );

	m1->m[1][0]= ffloor( (4096.0*m2[1][0]) + 0.5 );
	m1->m[1][1]= ffloor( (4096.0*m2[1][1]) + 0.5 );
	m1->m[1][2]= ffloor( (4096.0*m2[1][2]) + 0.5 );

	m1->m[2][0]= ffloor( (4096.0*m2[2][0]) + 0.5 );
	m1->m[2][1]= ffloor( (4096.0*m2[2][1]) + 0.5 );
	m1->m[2][2]= ffloor( (4096.0*m2[2][2]) + 0.5 );
		
	cox= CODEVEC(m1->m[0]);
	if(cox!=6) {
		coy= CODEVEC(m1->m[1]);
		if(coy!=6) {
			coz= CODEVEC(m1->m[2]);
			if(coz!=6) {
				if(cox==0 && coy==1 && coz==2) m1->flag= MAT_ONE;
				else if(cox==4 && coy==0 && coz==2) m1->flag= MAT_90;
				else if(cox==3 && coy==4 && coz==2) m1->flag= MAT_180;
				else if(cox==1 && coy==3 && coz==2) m1->flag= MAT_270;
				
			}
		}
	}
}

void write_p_objects(ListBase *idbase)
{
	extern float rvecmat[3][3], rcolmat[3][3];
	MATRIX obmat, imat;
	Object *ob;
	pObject pob;
	NetLink *nl;
	pNetLink pnl;
	int a, b;
	
	bzero(&pob, sizeof(pObject));

	ob= idbase->first;
	while(ob) {
		if(ob->id.us>0) {
			
			/* alleen: */
			if ELEM4(ob->type, OB_MESH, OB_SECTOR, OB_LIFE, OB_CAMERA) {
				
tot_ob++;
				/* init matrices */
				bzero(rvecmat, 3*3*4);
				do_realtimelight(ob, 0, 0);
				
				/* converteren */
				convertID(&pob.id, &ob->id);
				
				pob.type= le_short(ob->type);
				pob.partype= le_short(ob->partype);
				pob.sf= le_short( (short)ob->sf );
				pob.lay= le_int(ob->lay);
				
				pob.parent= (void *)ob->parent;
				pob.track= (void *)ob->track;
				pob.data= ob->data;
				pob.network.first= ob->network.first;
				pob.network.last= ob->network.last;

				/* je weet nooit */
				Mat4Invert(ob->imat, ob->obmat);
				
				/* recalclight optie? */
				if(ob->type==OB_LIFE) {
					Life *lf= ob->data;
					if(lf->flag & LF_RECALCLIGHT) {
						Mat4Mul3Vecfl(ob->obmat, rvecmat[0]); Normalise(rvecmat[0]);
						Mat4Mul3Vecfl(ob->obmat, rvecmat[1]); Normalise(rvecmat[1]);
						Mat4Mul3Vecfl(ob->obmat, rvecmat[2]); Normalise(rvecmat[2]);
						Mat3Transp(rvecmat);	/* JAWEL!!! zie ook psx code */
					}
				}
				
				convert_matrix(&obmat, ob->obmat);
				pob.obmat.flag= le_short(obmat.flag);
				convert_matrix(&imat, ob->imat);
				pob.imat.flag= le_short(imat.flag);

				for(a=0; a<3; a++) {
					pob.loc[a]= le_coordint(ob->loc[a]);
					pob.rot[a]= le_floatangshort(ob->rot[a]);
					pob.size[a]= le_floatshort(ob->size[a]);

					/* *********************** MAT SWITCH ********************* */				
					for(b=0; b<3; b++) {
						pob.obmat.m[b][a]= le_short(obmat.m[a][b]);
						pob.imat.m[b][a]= le_short(imat.m[a][b]);
						
						pob.loclight[b][a]= le_floatshort(rvecmat[b][a]);
						pob.lcolor[b][a]= le_floatshort(32767.0*rcolmat[a][b]);
					}
					/* *********************** MAT SWITCH ********************* */
					pob.obmat.t[a]= le_coordint(ob->obmat[3][a]);
					pob.imat.t[a]= le_coordint(ob->imat[3][a]);
				}

				/* schrijf LibData */
				write_p_data(ID_OB, sizeof(pObject), &pob, ob);
				
				/* alle direkte data */

				/* netlinks */
				nl= ob->network.first;
				while(nl) {
					pnl.prev= (void *)nl->prev;
					pnl.next= (void *)nl->next;
					pnl.ob= (void *)nl->ob;
					pnl.type= le_short(nl->type);
					pnl.flag= le_short(nl->flag);
					pnl.sfra= le_short(nl->sfra);
					pnl.len= le_short(nl->len);
					write_p_data(DATA, sizeof(pNetLink), &pnl, nl);
					nl= nl->next;
				}
			}
			
		}
		ob= ob->id.next;
	}
}

void write_p_meshs(ListBase *idbase)
{
	Material *ma;
	Object *ob;
	Sector *se;
	Life *lf;
	Mesh *mesh;
	pMesh pm;

	/* op nul: */
	ma= idbase->first;
	while(ma) {
		ma->id.flag &= ~LIB_DOIT;
		ma= ma->id.next;
	}


	mesh= idbase->first;
	while(mesh) {
		if(mesh->id.us>0 && (mesh->tface || mesh->dface)) {
			
			/* converteren */
			bzero(&pm, sizeof(pMesh));
			convertID(&pm.id, &mesh->id);

			pm.totvert= le_short(mesh->totvert);
			pm.totface= le_short(mesh->totface);
			pm.flag= mesh->flag & ~7;	/* eerste drie bitjes voor psx */
			
			make_packetdata(&pm, mesh);			
			
			if(mesh->dface) {
				make_d_mvertdata(&pm, mesh);
				make_dfaces(&pm, mesh);
			}
			else {
				if(pm.flag & ME_PUNO) make_mvert_punodata(&pm, mesh);
				else make_mvertdata(&pm, mesh);
			}
			
			if(mesh->oc) make_ocdata(&pm, mesh);
			
			pm.flag= le_short(pm.flag);
			
			/* schrijf LibData */
			write_p_data(ID_ME, sizeof(pMesh), &pm, mesh);
			
			/* schrijf alle direkte data */
			write_p_data(DATA, alloc_len(pm.packetdata), pm.packetdata, pm.packetdata);
			write_p_data(DATA, alloc_len(pm.mvert), pm.mvert, pm.mvert);		
			write_p_data(DATA, alloc_len(pm.dface), pm.dface, pm.dface);		
			write_p_data(DATA, alloc_len(pm.oc), pm.oc, pm.oc);
			
			/* vrijgeven */
			if(pm.packetdata) freeN(pm.packetdata);
			if(pm.mvert) freeN(pm.mvert);
			if(pm.dface) freeN(pm.dface);
			if(pm.oc) freeN(pm.oc);
		}
		mesh= mesh->id.next;
	}
		
}

void to_dos_name(char *strdos, char *strunix)
{
	int a;
	
	if( strncmp("/usr/people/", strunix, 12)==0) {
		strcpy(strdos, "P:\\");
		strcat(strdos, strunix+12);
	}
	else if( strncmp("/render/", strunix, 8)==0) {
		strcpy(strdos, "R:\\");
		strcat(strdos, strunix+8);
	}
	else if( strncmp("/pics/", strunix, 6)==0) {
		strcpy(strdos, "P:\\pics\\");
		strcat(strdos, strunix+6);
	}
	else if( strncmp("/data/", strunix, 6)==0) {
		strcpy(strdos, "P:\\data\\");
		strcat(strdos, strunix+6);
	}
	else {
		error("image stringconvert to dos");
		printf("name error: %s\n", strunix);
		return;
	}
	
	a= strlen(strdos);
	while(a--) {
		if(strdos[a]=='/') strdos[a]= '\\';
	}
}

void write_p_images(ListBase *idbase)
{
	Image *ima;
	pImage pm;
	
	ima= idbase->first;
	while(ima) {
		if(ima->id.us>0 && (ima->flag & IMA_USED)) {
			
			/* converteren */
			bzero(&pm, sizeof(pImage));
			
			convertID(&pm.id, &ima->id);
			
			to_dos_name(pm.name, ima->name);
			pm.tpageflag= le_short(ima->tpageflag);
			/* pm.tpagenr= le_short(ima->tpagenr); */
			pm.xrep= le_short(ima->xrep);
			pm.yrep= le_short(ima->yrep);
			
			pm.twsta= ima->twsta;
			pm.twend= ima->twend;
			
			/* schrijf LibData */
			write_p_data(ID_IM, sizeof(pImage), &pm, ima);
			
			/* schrijf alle direkte data */

			
			/* vrijgeven */
		}
		ima= ima->id.next;
	}
}

void write_p_materials(ListBase *idbase)
{
	Material *ma;
	pMaterial pma;
	int a;
	
	ma= idbase->first;
	while(ma) {
		if(ma->id.us>0 && (ma->id.flag & LIB_DOIT)) {
			
			/* converteren */
			bzero(&pma, sizeof(pMaterial));
			
			convertID(&pma.id, &ma->id);
			
			pma.ref= le_floatshort( DYNA_REF(ma));
			pma.fh_dist= le_floatshort( DYNA_FH_DIST(ma));
			pma.fh_int= le_floatshort( 0.1*DYNA_FH_INT(ma));
			pma.fh_frict= le_floatshort( DYNA_FH_FRICT(ma));
			pma.fh_xyfrict= le_floatshort( DYNA_FH_XYFRICT(ma));
			pma.lay= le_short(ma->lay);
			
			/* schrijf LibData */
			write_p_data(ID_MA, sizeof(pMaterial), &pma, ma);
			
		}
		ma= ma->id.next;
	}
}

void write_p_globals()
{
	short a, vars[MAXACTVAR];

	actvars_to_vars(vars);	/* array vullen */
	
	a= MAXACTVAR;
	while(a--) vars[a]= le_short(vars[a]);
	
	write_p_data(GLOB, 2*MAXACTVAR, vars, vars);
	
	
}

void write_append_data()
{
	Image *ima;
	int a, file, len;
	char *filedata;


	ima= G.main->image.first;
	while(ima) {
		if(ima->id.us>0 && (ima->flag & IMA_USED)) {

			file= open(ima->name, O_BINARY|O_RDONLY);
			if(file==-1) {
				error("image doesn't exsist\n");
				printf("image %s doesn't exsist\n", ima->name);
				len= 0;
				filedata= 0;
			}
			else {
				len= filesize(file);
				
				filedata= mallocN(len, "imagefile");
				read(file, filedata, len);
				
				close(file);
				
				
			}
			write_p_data(IMAG, len, filedata, filedata);
			
			if(filedata) freeN(filedata);
		}
		ima= ima->id.next;
	}
}

void write_blendpsx(char *dir)
{
	BHead bh;
	extern int mywtot;	/* writefile.c */
	char versionfstr[16];
	int file, len, fout=0;
	char astr[200], tempname[100], *home;

	len= strlen(dir);
	if(len==0) return;
	
	/* test op libraries: eruit */
	
	file= open(dir, O_BINARY|O_RDONLY);
	close(file);
	if(file>-1) if(saveover(dir)==0) return;

	/* BEVEILIGING eruit */
	
	file= open(dir, O_BINARY|O_WRONLY+O_CREAT+O_TRUNC, 0666);
	if(file== -1) {
		error("Can't write file");
		return;
	}
	strcpy(G.psx, dir);

	waitcursor(1);
	
	init_sectors();	/* doet ook mesh_isdone flag */
	init_camera_network();
	init_lifes();
	init_devs();

	number_images();
	/* deze is niet scene onafhankelijk */
	do_realtimelight(0, 0, 0);
	
	bgnwrite(file);

	strcpy(versionfstr, "BLENDPSXV105");
	mywrite(versionfstr, 16);	/* plus 4 voor filelen TOT appenddata */

	/* split_main(); */
/*----------------*/						
	
	write_p_scenes(&G.main->scene);
	write_p_objects(&G.main->object);
	write_p_meshs(&G.main->mesh);


	write_p_materials(&G.main->mat);	/* moet NA mesh */

	write_p_images(&G.main->image);
	write_p_sectors(&G.main->sector);
	write_p_lifes(&G.main->life);

	write_p_globals();

	/* write_p_cameras(&G.main->camera); */
	/* write_p_lamps(&G.main->lamp); */

	/* write_p_ikas(&G.main->ika); */

	
	/* write_p_keys(&G.main->key); */
	/* write_p_worlds(&G.main->world); */
	/* write_p_libraries(G.main->next); */
	

	/* lengte van eerste stuk wegschrijven */
	flushwrite();
	lseek(file, 12, SEEK_SET);
	len= le_int(mywtot-16);
	write(file, &len, 4);
	lseek(file, 0, SEEK_END);
	printf("saved %s, data size: %d ", G.psx, mywtot);
	len= mywtot;
	
	write_append_data();

	printf("append: %d\n", mywtot-len);
PRINT3(d, d, d, tot_prim, tot_ob, tot_life);
PRINT2(d, d, tot_packet, tot_dyna);

PRINT2(d, d, sizeof(pObject), sizeof(pLife));

	endwrite();
/*----------------*/						
	/* join_main(); */

	/* testen of alles goed is gelopen */
	
	bh.code= le_int(ENDB);
	bh.len= 0;
	if(write(file, &bh, sizeof(BHead)) != sizeof(BHead)) {
		error("Not enough diskspace");
		fout= 1;
	}

	close(file);

	end_camera_network();
	end_sectors();
	end_lifes(0);

	waitcursor(0);
}
	


 

