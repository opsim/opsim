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

#include "xa_mpg.h"

typedef struct Square
{
	void	*next, *prev;
	int		qscale, rlsize;
	
	float	* qtab;

	float	in[8][8];
	float	dct[8][8];
	float	rdct[8][8];
	float	out[8][8];
	
	short	quant[64];
	ushort	runlevel[65];
	char	is_last;
	char	is_c;
}Square;

print_square(Square * square)
{
	int x, y, i;
	
	i = 0;
	
	for (x = 0; x < 8 ; x++) {
		for (y = 0; y < 8 ; y++) {
			printf("%6d ", square->quant[i]);
			i++;
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
}


float trans_coef[8][8];

float mdec_ytab[64] =
{
	 2, 16, 19, 22, 26, 27, 29, 34, 
	16, 16, 22, 24, 27, 29, 34, 37, 
	19, 22, 26, 27, 29, 34, 34, 38, 
	22, 22, 26, 27, 29, 34, 37, 40, 
	22, 26, 27, 29, 32, 35, 40, 48, 
	26, 27, 29, 32, 35, 40, 48, 58, 
	26, 27, 29, 34, 38, 46, 56, 69, 
	27, 29, 35, 38, 46, 56, 69, 83
};

float mdec_ctab[64] =
{
	 2, 16, 16, 16, 16, 16, 16, 16, 
	16, 16, 16, 16, 16, 16, 16, 16, 
	16, 16, 16, 16, 16, 16, 16, 16, 
	16, 16, 16, 16, 16, 16, 16, 16, 
	16, 16, 16, 16, 16, 16, 16, 16, 
	16, 16, 16, 16, 16, 16, 16, 16, 
	16, 16, 16, 16, 16, 16, 16, 16, 
	16, 16, 16, 16, 16, 16, 16, 16, 
};


int mdec_zscan[64] = 
{
	 0,  1,  8, 16,  9,  2,  3, 10, 
	17, 24, 32, 25, 18, 11,  4,  5, 
	12, 19, 26, 33, 40, 48, 41, 34, 
	27, 20, 13,  6,  7, 14, 21, 28, 
	35, 42, 49, 56, 57, 50, 43, 36, 
	29, 22, 15, 23, 30, 37, 44, 51, 
	58, 59, 52, 45, 38, 31, 39, 46, 
	53, 60, 61, 54, 47, 55, 62, 63
};


#define RUN_SHIFT 10
#define LEVEL_MASK ((1 << RUN_SHIFT) -1)
#define RL_NOP 0xfe00
#define HUFF_MAXRUN	32
#define HUFF_MAXLEVEL	41
#define DCTSIZE2 64

uchar  *mpg_buff = 0;
int   mpg_bsize = 0;

/* BUFFER reading global vars */
int   mpg_b_bnum;
#ifdef WIN32
DWORD  mpg_b_bbuf;
#else
unsigned long long  mpg_b_bbuf;
#endif

/*******
 *
 */
#define MPG_INIT_BBUF(buff, bsize) \
{ mpg_buff = buff; mpg_bsize = bsize; \
  mpg_b_bnum = 0; mpg_b_bbuf = 0; \
}

/*******
 *
 */
#ifdef WIN32
#define MPG_BIT_MASK(s) ((1L << (s)) - 1)
#else
#define MPG_BIT_MASK(s) ((1ll << (s)) - 1)
#endif

/*******
 *
 */
#define MPG_GET_BBITS(num, result) \
{ while(mpg_b_bnum < (num)) { mpg_b_bnum += 8; mpg_bsize--; \
		mpg_b_bbuf = (*mpg_buff++) | (mpg_b_bbuf << 8); } \
  mpg_b_bnum -= (num); \
  (result) = ((mpg_b_bbuf >> mpg_b_bnum) & MPG_BIT_MASK(num)); \
}

#define MPG_NXT_BBITS(num, result) \
{ while(mpg_b_bnum < (num)) { mpg_b_bnum += 8; mpg_bsize--; \
		mpg_b_bbuf = (*mpg_buff++) | (mpg_b_bbuf << 8); } \
  result = (mpg_b_bbuf >> (mpg_b_bnum - num)) & MPG_BIT_MASK(num); \
}

#define MPG_FLUSH_BBITS(num) \
{ while(mpg_b_bnum < (num)) { mpg_b_bnum += 8; mpg_bsize--; \
		mpg_b_bbuf = (*mpg_buff++) | (mpg_b_bbuf << 8); } \
  mpg_b_bnum -= (num); \
}


/**************** HUFFMAN TABLES AND INIT ******************************/

#define MB_ESCAPE 35
#define MB_STUFFING 34


/* Code for unbound values in decoding tables */

#define RUN_MASK_XA		0xfc00
#define LEVEL_MASK_XA	0x03f0
#define NUM_MASK_XA		0x000f
#define RUN_SHIFT_XA	10
#define LEVEL_SHIFT_XA	4
#define END_OF_BLOCK	0x3e
#define DCT_ERROR		0x3f
#define ESCAPE			0x3d


/*
 *--------------------------------------------------------------
 *
 * decodedctcoeff --
 *
 *	Huffman Decoder for dct_coeff_first and dct_coeff_next;
 *      locations where the results of decoding: run and level, are to
 *      be placed and also the type of DCT coefficients, either
 *      dct_coeff_first or dct_coeff_next, are being passed as argument.
 *      
 *      The decoder first examines the next 8 bits in the input stream,
 *      and perform according to the following cases:
 *      
 *      '0000 0000' - examine 8 more bits (i.e. 16 bits total) and
 *                    perform a table lookup on dct_coeff_tbl_0.
 *                    One more bit is then examined to determine the sign
 *                    of level.
 *
 *      '0000 0001' - examine 4 more bits (i.e. 12 bits total) and 
 *                    perform a table lookup on dct_coeff_tbl_1.
 *                    One more bit is then examined to determine the sign
 *                    of level.
 *      
 *      '0000 0010' - examine 2 more bits (i.e. 10 bits total) and
 *                    perform a table lookup on dct_coeff_tbl_2.
 *                    One more bit is then examined to determine the sign
 *                    of level.
 *
 *      '0000 0011' - examine 2 more bits (i.e. 10 bits total) and 
 *                    perform a table lookup on dct_coeff_tbl_3.
 *                    One more bit is then examined to determine the sign
 *                    of level.
 *
 *      otherwise   - perform a table lookup on dct_coeff_tbl. If the
 *                    value of run is not ESCAPE, extract one more bit
 *                    to determine the sign of level; otherwise 6 more
 *                    bits will be extracted to obtain the actual value 
 *                    of run , and then 10 bits to get the value of level.
 *                    
 *      
 *
 * Results:
 *	The decoded values of run and level or ERROR for unbound values
 *      are placed in the locations specified.
 *
 * Side effects:
 *	Bit stream is irreversibly parsed.
 *
 *--------------------------------------------------------------
 */
static void decodedctcoeff(dct_coeff_tbl, run, level)
ushort *dct_coeff_tbl;
uint *run;
int *level;
{
	uint idx, value, f;
	int temp;
	
	MPG_NXT_BBITS(8,idx);

	if (idx > 3)
	{
		value = dct_coeff_tbl[idx];
		*run = (value & RUN_MASK_XA) >> RUN_SHIFT_XA;
		if (*run == END_OF_BLOCK) {
			*level = END_OF_BLOCK;
		} else {
			f = (value & NUM_MASK_XA) + 1;
			MPG_FLUSH_BBITS(f);

			if (*run != ESCAPE)
			{
				uint t;
				*level = (value & LEVEL_MASK_XA) >> LEVEL_SHIFT_XA;
				MPG_GET_BBITS(1,t);
				if (t) *level = - *level;
			}
			else	/* *run == ESCAPE */
			{
				/* non-mpeg-standard */

				MPG_GET_BBITS(16,temp);  /* 6 are run, 10 are part of level */
				*run = temp >> 10;		 /*  1st 6 bits are run */
				*level = (temp << 22) >> 22;
			}
		}
	} else {
		uint t;
		if (idx == 2)
		{
			MPG_NXT_BBITS(10,idx);
			value = dct_coeff_tbl_2[idx & 3];
		}
		else if (idx == 3)
		{
			MPG_NXT_BBITS(10,idx);
			value = dct_coeff_tbl_3[idx & 3];
		}
		else if (idx)	/* idx == 1 */
		{
			MPG_NXT_BBITS(12,idx);
			value = dct_coeff_tbl_1[idx & 15];
		}
		else /* idx == 0 */
		{
			MPG_NXT_BBITS(16,idx);
			value = dct_coeff_tbl_0[idx & 255];
		}
		*run = (value & RUN_MASK_XA) >> RUN_SHIFT_XA;
		*level = (value & LEVEL_MASK_XA) >> LEVEL_SHIFT_XA;

		f = (value & NUM_MASK_XA) + 1;
		MPG_FLUSH_BBITS(f);
		MPG_GET_BBITS(1,t);
		if (t) *level = -*level;
	}
}

void huffparse(Square * square)
{
	short dcval;
	int index, tmp;
	int run, level;

	/* read DCvalue */

	for (index = 1; index < DCTSIZE2; index++) square->quant[index] = 0;

	MPG_GET_BBITS(10, dcval);
	dcval <<= 6;
	dcval >>= 6;
	
	square->quant[0] = dcval;
	
	index = 0;

	while( index < DCTSIZE2 )
	{
		decodedctcoeff(dct_coeff_next, &run, &level);
		if (run == END_OF_BLOCK) break;

		index += (run + 1);
		
		square->quant[index] = level;
	}
	
	MPG_GET_BBITS(2, tmp); /* End of block bits */
	if (tmp != 2) printf("Parse Error: %d index=%d run=%d level=%d\n", tmp, index, run, level);
	
}


void unquant(Square * square)
{
	float * rdct, qscale, *qtab;
	short * quant;
	int i, j;
	
	quant = &square->quant[0];
	rdct = &square->rdct[0][0];
	qtab = square->qtab;
	
	qscale = square->qscale / 8.0;
	
	rdct[0] = quant[0] * qtab[0];

	for (i = 1; i < 64; i++) {
		j = mdec_zscan[i];
		rdct[j] = quant[i] * qtab[j] * qscale;
	}
}


void init_dct()
{
	int i, j;
	float s;

	for (i=0; i<8; i++)
	{
		s = (i==0) ? sqrt(0.125) : 0.5;

		for (j=0; j<8; j++)
			trans_coef[i][j] = s * cos((M_PI/8.0)*i*(j+0.5));
	}
}


void rev_dct(Square * square)
{
	int i, j, k;
	float s;
	float tmp[64];
	static init_done = FALSE;
	
	if (init_done == FALSE) init_dct();
	init_done = TRUE;
	
	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
		{
			s = 0.0;

			for (k=0; k<8; k++)
				s += trans_coef[k][j] * square->rdct[i][k];

			tmp[8*i+j] = s;
		}

	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
		{
			s = 0.0;

			for (k=0; k<8; k++)
				s += trans_coef[k][i] * tmp[8*k+j];

			square->out[i][j] = s;
		}
}

void square2ibuf(Square * square, ImBuf * ibuf, int x, int y, int c)
{
	int i, j;
	uchar * rect;
	float * out, val;
	int overfl = 0;
	
	if (square == 0) return;
	
	rect = (uchar *) (ibuf->rect + x + y * ibuf->x);
	rect += c;
	
	out = &square->out[0][0];
	
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 32; j += 4) {
			val = *out++ + 128.5;
			if (val < 0.0) {
				/*overfl++;*/
				val = 0.0;
			} else if (val > 255.0) {
				val = 255.0;
				/*overfl++;*/
			}
			
			rect[j] = val;
		}
		rect += 4 * ibuf->x;
	}
	
/*
	if (overfl > 32) {
		printf("mdec overfl %d\n", overfl);
		print_square(square);
	}
*/
}

void Rev_Square(Square * square, ImBuf * ibuf, int x, int y, int c)
{
	float tmp[64], * rdct;
	int i, t;
	
	huffparse(square);
	unquant(square);
	rev_dct(square);
	square2ibuf(square, ibuf, x, y, c);
}

void huffdecompress(ImBuf * ibuf, char * mdec, int size, int qscale)
{
	char * buf, *_buf;
	int i, data, x, y;
	Square * square;
	ImBuf * hbuf;
	uchar * src, * dst;
	
	buf = _buf = callocN(size + 4, "huff_compress");
	
	for(i = size; i > 0; i -= 4) {
		*buf++ = mdec[1];
		*buf++ = mdec[0];
		*buf++ = mdec[3];
		*buf++ = mdec[2];
		mdec += 4;	
	}
		
	square = NEW(Square);
	square->qscale = qscale;
	
	MPG_INIT_BBUF((uchar *) _buf, size);
	
	hbuf = allocImBuf(ibuf->x / 2, ibuf->y / 2, 24,  IB_rect, 0);
	
	for (x = 0; x < hbuf->x; x += 8) {
		for (y = 0; y < hbuf->y; y += 8) {
			square->qtab = mdec_ctab;
			Rev_Square(square, hbuf, x, y, 3);
			Rev_Square(square, hbuf, x, y, 1);
			
			square->qtab = mdec_ytab;
			Rev_Square(square, ibuf, 2 * x,     2 * y    , 2);
			Rev_Square(square, ibuf, 2 * x + 8, 2 * y    , 2);
			Rev_Square(square, ibuf, 2 * x,     2 * y + 8, 2);
			Rev_Square(square, ibuf, 2 * x + 8, 2 * y + 8, 2);
		}
	}
	
	/* wat staat er nu nog in de file ???? */
	
	i = (mpg_bsize * 8) + mpg_b_bnum;
	MPG_NXT_BBITS(32, data);
	
	if (data != 0x7fc00000) printf("MDEC PARSE ERROR: still %d bits available: %08x\n", i, data);
	
	dst = (uchar *) ibuf->rect;

	for (y = 0; y < ibuf->y; y++) {
		src = (uchar *) hbuf->rect;
		src += (y >> 1) * hbuf->x * 4;

		for (x = hbuf->x; x > 0; x--) {
			dst[5] = dst[1] = src[1];
			dst[7] = dst[3] = src[3];
			src += 4;
			dst += 8;
		}
	}

	freeImBuf(hbuf);

	freeN(_buf);
	freeN(square);
}


