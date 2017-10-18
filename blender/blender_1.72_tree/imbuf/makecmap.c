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

#include "iff.h"

#include "util.h"

#define OBJECTBLOK "makecmap"


#define float double

void _percentdone(float rt)
{
	
}

void (*wu_percentdone)(float) = _percentdone;

struct WU_box
{
	struct WU_box *next,*prev;
	short num[4];
	short min[4];
	short max[4];
	float error;
	float value[6];
};

#define POW_I(x,y,z) {int i ; (x)=1 ; for(i=0 ; i<(z); i++) (x) *= (y);}
#define calcerr4(a,b,g,r) ((a) + (b) + 4.0 * (g) + 2.0 * (r))
#define calcerr3(b,g,r) ((b) + 4.0 * (g) + 2.0 * (r))
#define add_r 1
extern short floydmax;

void sigma_ccalc(arr,numa,numb,numg,numr)
float *arr;
short numa,numb,numg,numr;
{
	int a,b,g,r,add_a,add_b,add_g,index,a_index,b_index,g_index,t;
	float value;

	/*
		doelstelling: kan voor zowel rgb als voor rgba colormap berekenen
		methode volgens Wu in Graphic Gems II.
		volgorde is abgr.
		Kleurberekening kan beter in een kleurruimte gebeuren waar
		vectoren iets te betekenen hebben.
		a,b,g,r = aantal componenten waarop die factor gequantiseerd is (minimaal 1)
		arr1 = bronarray;
		arr2 = doelarray;
	*/

	/* volgorde van berekenen:

	add_a	add_b	add_g	add_r

	0		0		0		0		+
	0		0		0		1		+
	0		0		1		1		-
	0		0		1		0		+
	0		1		1		0		-
	0		1		1		1		+
	0		1		0		1		-
	0		1		0		0		+

	1		1		0		0		-
	1		1		0		1		+
	1		1		1		1		-
	1		1		1		0		+
	1		0		1		0		-
	1		0		1		1		+
	1		0		0		1		-
	1		0		0		0		+	

	*/

	if (arr == 0) return;

	add_g = (numr + 1) * add_r;
	add_b = (numg + 1) * add_g;
	add_a = (numb + 1) * add_b;
	if (numa == 1) add_a = 0;

	a_index = 0;
	for(a=numa ; a >0; a--){
		b_index = 0;
		for(b=numb;b>0;b--){
			g_index = 0;
			for(g=numg;g>0;g--){
				index = a_index + b_index + g_index;
				for(r=numr;r>0;r--){
					t = index;
					value = arr[index];

					t  -= add_r;
					value += arr[t];
					t  -= add_g;
					value -= arr[t];
					t  += add_r;
					value += arr[t];
					t  -= add_b;
					value -= arr[t];
					t  -= add_r;
					value += arr[t];
					t  += add_g;
					value -= arr[t];
					t  += add_r;
					value += arr[t];

					if (add_a){			/* doet alpha mee ? */
						t  -= add_a;
						value -= arr[t];
						t  -= add_r;
						value += arr[t];
						t  -= add_g;
						value -= arr[t];
						t  += add_r;
						value += arr[t];
						t  += add_b;
						value -= arr[t];
						t  -= add_r;
						value += arr[t];
						t  += add_g;
						value -= arr[t];
						t  += add_r;
						value += arr[t];
					}
					arr[index] = value;
					index += add_r;
				}
				g_index += add_g;
			}
			b_index += add_b;
		}
		a_index += add_a;
	}
}


makearrays(arrays,numa,numb,numg,numr)
float **arrays;
short numa,numb,numg,numr;
{
	int a, b, g, r, i, add_a, add_b, add_g, index, a_index, b_index, g_index;
	float *a_array, *b_array, *g_array, *r_array, *cnt_array, *err_array, value;
	float fa, fb, fg, fr;

	add_g = (numr + 1) * add_r;
	add_b = (numg + 1) * add_g;
	add_a = (numb + 1) * add_b;
	if (numa == 1) add_a = 0;

	cnt_array = arrays[0];
	a_array = arrays[1];
	b_array = arrays[2];
	g_array = arrays[3];
	r_array = arrays[4];

	if (add_a){
		a_index = 0;
		for(a = 0 ; a < numa ; a++){
			fa = a ;
			b_index = 0;
			
			wu_percentdone((100.0 * a)/ numa);
			
			for(b = 0 ; b < numb ; b++){
				fb = b ;
				g_index = 0;

				for(g = 0 ; g < numg ; g++){
					fg = g;
					index = a_index + b_index + g_index;
					for(r = 0 ; r < numr ; r++){
						value = cnt_array[index];
						a_array[index] = fa * value;
						b_array[index] = fb * value;
						g_array[index] = fg * value;
						r_array[index] = r * value;

						index += add_r;
					}
					g_index += add_g;
				}
				b_index += add_b;
			}
			a_index += add_a;
		}
	} else{
		b_index = 0;
		for(b = 0 ; b < numb ; b++){
			fb = b ;
			g_index = 0;
			wu_percentdone((100.0 * b)/ numb);

			for(g = 0 ; g < numg ; g++){
				fg = g;
				index = b_index + g_index;
				for(r = 0 ; r < numr ; r++){
					value = cnt_array[index];
					b_array[index] = fb * value;
					g_array[index] = fg * value;
					r_array[index] = r * value;

					index += add_r;
				}
				g_index += add_g;
			}
			b_index += add_b;
		}
	}

	err_array = arrays[5];

	if (add_a){
		a_index = 0;
		for(a = 0 ; a < numa ; a++){
			fa = a;
			fa *= fa ;
			b_index = 0;

			wu_percentdone((100.0 * a)/ numa);

			for(b = 0 ; b < numb ; b++){
				fb = b ;
				fb *= fb ;
				g_index = 0;

				for(g = 0 ; g < numg ; g++){
					fg = g;
					fg *= fg ;
					index = a_index + b_index + g_index;

					for(r = 0 ; r < numr ; r++){
						err_array[index] = calcerr4(fa,fb,fg,r * r) * cnt_array[index];
						index += add_r;
					}
					g_index += add_g;
				}
				b_index += add_b;
			}
			a_index += add_a;
		}
	} else{
		b_index = 0;
		for(b = 0 ; b < numb ; b++){
			fb = b ;
			fb *= fb ;
			g_index = 0;
			wu_percentdone((100.0 * b)/ numb);

			for(g = 0 ; g < numg ; g++){
				fg = g;
				fg *= fg ;
				index =b_index + g_index;

				for(r = 0 ; r < numr ; r++){
					err_array[index] = calcerr3(fb,fg,r * r) * cnt_array[index];
					index += add_r;
				}
				g_index += add_g;
			}
			b_index += add_b;
		}
	}
	for (i = 5 ; i >= 0 ; i--){
		sigma_ccalc(arrays[i],numa,numb,numg,numr);
	}
}

#undef add_r

float eval_box(box,arrays)
struct WU_box * box;
float **arrays;
{
	int mula,mulb,mulg;
	int adda,addb,addg,addr;
	int index,i,t;
	float *arr,value;
	double dvalue;

	mulg = (box->num[3] + 1);
	mulb = mulg * (box->num[2] + 1);
	mula = mulb * (box->num[1] + 1);
	if (box->num[0] == 1) mula = 0;

	adda = mula * (box->max[0] - box->min[0]);
	addb = mulb * (box->max[1] - box->min[1]);
	addg = mulg * (box->max[2] - box->min[2]);
	addr = (box->max[3] - box->min[3]);

	index = box->max[3] + mulg * box->max[2] + mulb * box->max[1] + mula * box->max[0];
	for(i = 5; i >= 0 ; i--){
		arr = arrays[i];
		if (arr){
			value = arr[index];
			t = index;

			t  -= addr;
			value -= arr[t];
			t  -= addg;
			value += arr[t];
			t  += addr;
			value -= arr[t];
			t  -= addb;
			value += arr[t];
			t  -= addr;
			value -= arr[t];
			t  += addg;
			value += arr[t];
			t  += addr;
			value -= arr[t];

			if (adda){			/* doet alpha mee ? */
				t  -= adda;
				value += arr[t];
				t  -= addr;
				value -= arr[t];
				t  -= addg;
				value += arr[t];
				t  += addr;
				value -= arr[t];
				t  += addb;
				value += arr[t];
				t  -= addr;
				value -= arr[t];
				t  += addg;
				value += arr[t];
				t  += addr;
				value -= arr[t];
			}
			box->value[i] = value;
		}
	}

	/*
		we gebruiken maar even de aangepaste berekening voor y (van yuv) voor 
		de bepaling van de error. (was .114 / .587 / .299 );
		Dit kan misschien nog wel vereenvoudigd worden tot 1 / 4 / 2;
		Als dit een keer goed gaat, kan dat meteen bij het maken van
		de arrays gebruikt worden, zodat dat er nog maar zes hoeven te zijn.
	*/

	if (adda){
		dvalue = calcerr4(box->value[1] * box->value[1] ,box->value[2] * box->value[2] , box->value[3] * box->value[3]
		    , box->value[4] * box->value[4]);
		dvalue /= box->value[0];
		dvalue -= box->value[5];
		box->error = -dvalue;
	} else{
		dvalue = calcerr3(box->value[2] * box->value[2] , box->value[3] * box->value[3] , box->value[4] * box->value[4]);
		dvalue /= box->value[0];
		dvalue -= box->value[5];
		box->error = -dvalue;
	}

	return(box->value[0]);		/* geeft aantal pixels in box terug */
}


float cut_wu_box(mbox, box1, box2, bbox1, bbox2, arrays, merror, index)
struct WU_box *mbox, *box1, *box2, *bbox1, *bbox2;
float **arrays, merror;
short index;
{
	short i;
	float nerror;

	for (i = mbox->min[index] + 1 ; i < mbox->max[index] ; i++){
		box1->max[index] = i;
		box2->min[index] = i;
		if (eval_box(box1,arrays) == 0.0) mbox->min[index] = i;
		else{
			if (eval_box(box2,arrays) == 0.0) {
				mbox->max[index] = i ;
				break;
			}
			nerror = box1->error + box2->error;
			if (nerror < merror){
				*bbox1 = *box1;
				*bbox2 = *box2;
				merror = nerror;
			}
		}
	}

	return (merror);
}


void make_wu_map(wu_base,arrays,ncols)
struct ListBase *wu_base;
float ** arrays;
short ncols;
{
	struct WU_box *pbox,*mbox,box1,box2,bbox1,bbox2;
	short i, totcols;
	float merror;

	if (wu_base == 0) return;
	totcols = ncols;
	
	for(; ncols > 1 ; ncols --){
		pbox = (struct WU_box *) wu_base->first;
		if (pbox == 0) return;
		merror = 0.0;

		while(pbox){
			if (pbox->error > merror){
				merror = pbox->error;
				mbox = pbox;
			}
			pbox = pbox->next;
		}

		if (merror < 0.001) break; 						/* < 0.01 is te klein bij floats*/

		/*printf("%f\n", merror);*/
		bbox1 = *mbox;								/* voor alle zekerheid */
		bbox2 = *mbox;

		box1 = *mbox ;
		box2 = *mbox;
		merror = cut_wu_box(mbox,&box1,&box2,&bbox1,&bbox2,arrays,merror,2);
		box1 = *mbox ;
		box2 = *mbox;
		merror = cut_wu_box(mbox,&box1,&box2,&bbox1,&bbox2,arrays,merror,3);
		box1 = *mbox ;
		box2 = *mbox;
		merror = cut_wu_box(mbox,&box1,&box2,&bbox1,&bbox2,arrays,merror,1);
		box1 = *mbox ;
		box2 = *mbox;
		merror = cut_wu_box(mbox,&box1,&box2,&bbox1,&bbox2,arrays,merror,0);

		*mbox = bbox1;
		mbox = mallocstruct(struct WU_box,1);
		*mbox = bbox2;
		addtail(wu_base,mbox);
		
		wu_percentdone(100.0 - ((100.0 * ncols) / totcols));
	}
}


void makecmap(ibuf, abits, cbits, cols, dit)
struct ImBuf * ibuf;
short abits, cbits, cols;
void (*dit)();
{
	float *arrays[6],*_arrays[6],*arr;
	int i,r,g,b;
	uchar *rect;
	uint *cmap;
	ushort mula, mulb, mulg;
	uint ofs;
	short numa, numb, numg, numr;
	struct WU_box *box;
	struct ListBase _wu_base;
	struct ListBase * wu_base = &_wu_base;
	ImBuf * tbuf;
	extern void rectcpy();
	extern short alpha_col0;

	if (ibuf == 0) {
		printf("makecmap: nopic\n");
		return;
	}
	if (ibuf->rect == 0) {
		printf("makecmap: norect\n");
		return;
	}

	if (cbits == 0) cbits = ibuf->cbits;
	else ibuf->cbits = cbits;
	
	tbuf = dupImBuf(ibuf);
	
	numa = 1 << abits ;
	numb = 1 << cbits;
	numg = 1 << cbits ;
	numr = 1 << cbits;
	
	if (dit){
		if (abits) dit(ibuf, 0, abits);
		dit(ibuf, 1, cbits);
		dit(ibuf, 2, cbits);
		dit(ibuf, 3, cbits);
	} else{
		if (abits) dit0(ibuf, 0, abits);
		dit0(ibuf, 1, cbits);
		dit0(ibuf, 2, cbits);
		dit0(ibuf, 3, cbits);
	}

	if (abits == 0){
		_arrays[0] = callocstruct(float, (numb + 1) * (numg + 1) * (numr + 1));
		_arrays[1] = 0;
		for (i = 2 ; i < 6 ;i++) _arrays[i] = callocstruct(float, (numb + 1) * (numg + 1) * (numr + 1));
	} else for (i = 0 ; i < 6 ;i++) _arrays[i] = callocstruct(float, (numa + 1) * (numb + 1) * (numg + 1) * (numr + 1));

	mulg = (numr + 1);
	mulb = mulg * (numg + 1);
	mula = mulb * (numb + 1);
	if (numa == 1) mula = 0;

	ofs = mula + mulb + mulg + 1;
	for (i = 0 ; i < 6 ;i++){
		if (_arrays[i]){
			arrays[i] = _arrays[i] + ofs;
		} else{
			arrays[i] = 0;
		}
	}

	/* pixelcount array genereren */

	abits = 8 - abits;
	cbits = 8 - cbits;

	arr = arrays[0];
	rect = (uchar *) ibuf->rect;
	if (mula){
		for (i = ibuf->x * ibuf->y ; i>0 ; i--){
			arr[(mula * rect[0] >> abits) + (mulb * rect[1] >> cbits) + (mulg * rect[2] >> cbits) + (rect[3] >> cbits)]
			    += 1.0;
			rect += 4;
		}
	} else{
		for (i = ibuf->x * ibuf->y ; i>0 ; i--){
			arr[(mulb * rect[1] >> cbits) + (mulg * rect[2] >> cbits) + (rect[3] >> cbits)] += 1.0;
			rect += 4;
		}
	}

	makearrays(arrays, numa, numb, numg, numr);

	wu_base->first = 0 ;
	wu_base->last = 0;
	box = mallocstruct(struct WU_box,1);
	addtail(wu_base,box);

	box->num[0] = numa ;
	box->num[1] = numb ;
	box->num[2] = numg ;
	box->num[3] = numr;
	for (i = 3 ; i>=0 ; i--){
		box->min[i] = -1;
		box->max[i] = box->num[i] - 1;
	}
	eval_box(box,arrays);

	ibuf->mincol = 0;
	ibuf->maxcol = cols;
	ibuf->depth = 0;

	if (alpha_col0) cols--;
	
	make_wu_map(wu_base, arrays, cols);

	addcmapImBuf(ibuf);

	cmap = ibuf->cmap;
	i = 0;

	if (alpha_col0) {
		cmap[i] = 0;
		i++;
	}
	
	box = (struct WU_box *) wu_base->first;

	while (box){
		short r,g,b,a;
		a = (box->value[1] / box->value[0]) +.5;
		b = (box->value[2] / box->value[0]) +.5;
		g = (box->value[3] / box->value[0]) +.5;
		r = (box->value[4] / box->value[0]) +.5;
		a <<= abits ;
		b <<= cbits ;
		g <<= cbits ;
		r <<= cbits ;
		if (a & 0xff00){
			if (a < 0) a = 0;
			else a = 255;
		}
		if (b & 0xff00){
			if (b < 0) b = 0;
			else b = 255;
		}
		if (g & 0xff00){
			if (g < 0) g = 0;
			else g = 255;
		}
		if (r & 0xff00){
			if (r < 0) r = 0;
			else r = 255;
		}
		cmap[i] = (a << 24) + (b << 16) + (g << 8) + r;
		i++;
		box = box->next;
	}
	
	if (i < cols) printf("found %d instead of %d\n", i, cols);

	if (abits == 8) {
		/* alpha bitjes zetten */
		for (i--; i >= 0; i--) {
			ibuf->cmap[i] |= 0xff000000;
		}
		if (alpha_col0) ibuf->cmap[0] &= 0xffffff;
	}
	
	freelist(wu_base);
	for (i = 0 ; i < 6 ;i++) if ( _arrays[i] ) free(_arrays[i]);
	
	rectoptot(ibuf, tbuf, rectcpy);

	freeImBuf(tbuf);
	converttocmap(ibuf);
}

