
/* Copyright (c) Mark J. Kilgard, 1994. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include <stdlib.h>
#include <glut.h>
#include "glutint.h"
#include "glutbitmap.h"

/* ZR - wanted to get rotate pages working... didnt succeed (yet) */

static int GLUTpagerotation= 0;

void glutRotatePage(int degrees)
{
	GLUTpagerotation+= degrees;
}

void APIENTRY 
glutBitmapCharacter(GLUTbitmapFont font, int c)
{
	const BitmapCharRec *ch;
	BitmapCharRecNC *newc;
	BitmapFontPtr fontinfo;
	GLint swapbytes, lsbfirst, rowlength;
	GLint skiprows, skippixels, alignment;
	int yadv=0;
	
#if defined(WIN32)
	fontinfo = (BitmapFontPtr) __glutFont(font);
#else
	fontinfo = (BitmapFontPtr) font;
#endif


	if (c < fontinfo->first ||
	    c >= fontinfo->first + fontinfo->num_chars)
		return;

	ch= fontinfo->ch[c - fontinfo->first];

/*
	if (GLUTpagerotation) {
		int x, y;
		
		newc = malloc(sizeof(BitmapCharRec));
		memcpy(newc, fontinfo->ch[c - fontinfo->first], sizeof(BitmapCharRec));
		
		newc->width= ch->height;
		newc->height= ch->width;

		newc->xorig= ch->yorig;
		newc->yorig= ch->xorig;
		
		yadv= ch->advance;
		newc->advance= 0;
		
		newc->bitmap= malloc(newc->height);
		memset(newc->bitmap, 0, newc->height);

		for (x=0; x<ch->width; x++) {
			for (y=0; y<ch->height; y++) {
				newc->bitmap[x] |= ((ch->bitmap[y]>>x)&1)<<y;
			}			
		}

		ch= newc;
	}
*/

	if (ch) {
		/* Save current modes. */
		glGetIntegerv(GL_UNPACK_SWAP_BYTES, &swapbytes);
		glGetIntegerv(GL_UNPACK_LSB_FIRST, &lsbfirst);
		glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowlength);
		glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skiprows);
		glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skippixels);
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
		/* Little endian machines (DEC Alpha for example) could
       benefit from setting GL_UNPACK_LSB_FIRST to GL_TRUE
       instead of GL_FALSE, but this would require changing the
       generated bitmaps too. */
		glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
		glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glBitmap(ch->width, ch->height, ch->xorig, ch->yorig,
		    ch->advance, 0, ch->bitmap);
/*			
		glBitmap(ch->width, ch->height, ch->xorig, ch->yorig,
		    ch->advance, yadv, ch->bitmap);
*/			
		/* Restore saved modes. */
		glPixelStorei(GL_UNPACK_SWAP_BYTES, swapbytes);
		glPixelStorei(GL_UNPACK_LSB_FIRST, lsbfirst);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, rowlength);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, skiprows);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, skippixels);
		glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	}

/* 	if (GLUTpagerotation) free(newc); */
}


/* *************** TON **************** */

void
glutBitmapStr(GLUTbitmapFont font, char *str)
{
	const BitmapCharRec *ch;
	BitmapFontPtr fontinfo = (BitmapFontPtr) font;
	GLint swapbytes, lsbfirst, rowlength;
	GLint skiprows, skippixels, alignment;
	int a, len, c, val;
	float rpos[4];
	
	if(str==0 || str[0]==0) return;

	/* Save current modes. */
	glGetIntegerv(GL_UNPACK_SWAP_BYTES, &swapbytes);
	glGetIntegerv(GL_UNPACK_LSB_FIRST, &lsbfirst);
	glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowlength);
	glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skiprows);
	glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skippixels);
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	/* Little endian machines (DEC Alpha for example) could
	   benefit from setting GL_UNPACK_LSB_FIRST to GL_TRUE
	   instead of GL_FALSE, but this would require changing the
	   generated bitmaps too. */
	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	len= strlen(str);
	for(a=0; a<len; a++) {
		c= str[a];

		if (c < fontinfo->first || c >= fontinfo->first + fontinfo->num_chars) continue;

		ch = fontinfo->ch[c - fontinfo->first];
		if (ch) {
			glBitmap(ch->width, ch->height, ch->xorig, ch->yorig, ch->advance, 0, ch->bitmap);
		}
	}

	/* Restore saved modes. */
	glPixelStorei(GL_UNPACK_SWAP_BYTES, swapbytes);
	glPixelStorei(GL_UNPACK_LSB_FIRST, lsbfirst);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, rowlength);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, skiprows);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, skippixels);
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
}

/* ZR */
/* Works like the other but without clipping text */

void
glutBitmapStrNC(GLUTbitmapFont font, char *str, float x, float y)
{
	const BitmapCharRec *ch;
	BitmapFontPtr fontinfo = (BitmapFontPtr) font;
	GLint swapbytes, lsbfirst, rowlength;
	GLint skiprows, skippixels, alignment;
	int a, len, c, val;
	float rpos[4];
	
	glRasterPos2f(x, y);
	if(str==0 || str[0]==0) return;

	/* Save current modes. */
	glGetIntegerv(GL_UNPACK_SWAP_BYTES, &swapbytes);
	glGetIntegerv(GL_UNPACK_LSB_FIRST, &lsbfirst);
	glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowlength);
	glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skiprows);
	glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skippixels);
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	/* Little endian machines (DEC Alpha for example) could
	   benefit from setting GL_UNPACK_LSB_FIRST to GL_TRUE
	   instead of GL_FALSE, but this would require changing the
	   generated bitmaps too. */
	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	len= strlen(str);
	for(a=0; a<len; a++) {
		c= str[a];

		if (c < fontinfo->first || c >= fontinfo->first + fontinfo->num_chars) continue;

		ch = fontinfo->ch[c - fontinfo->first];
		if (ch) {
			glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID, &val);
			if (!val) {
				x+= ch->advance;
				glRasterPos2f(x, y);
				continue;
			} else glBitmap(ch->width, ch->height, ch->xorig, ch->yorig, ch->advance, 0, ch->bitmap);
		}
	}

	/* Restore saved modes. */
	glPixelStorei(GL_UNPACK_SWAP_BYTES, swapbytes);
	glPixelStorei(GL_UNPACK_LSB_FIRST, lsbfirst);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, rowlength);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, skiprows);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, skippixels);
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
}
