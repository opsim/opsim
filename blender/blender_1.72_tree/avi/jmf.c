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

#include "avi.h"

#include <stdlib.h>

static int readbyte=0;
static int rbufferlevel=0;

static void readinit (void) {
  readbyte=0;
  rbufferlevel=0;
}

static int bitread (unsigned char **buf) {
  int out=0;

  if (rbufferlevel == 0) {
    readbyte = *(*buf);
    *buf = *buf + 1;
    rbufferlevel = 8;
  }

  rbufferlevel--;
  out = (readbyte >> rbufferlevel) & 1;

  return out;
}

static int byteread (unsigned char **buf) {
  int i;
  int out=0;

  for (i=7; i >= 0; i--) {
    out |= bitread (buf) << i;
  }

  return out;
}

static int nbitread (unsigned char **buf, int n) {
  int i;
  int out=0;

  for (i=n-1; i >= 0; i--) {
    out |= bitread (buf) << i;
  }

  return out;
}

static int writebyte=0;
static int wbufferlevel=7;

static void writeinit (void) {
  writebyte=0;
  wbufferlevel=7;
}

static void bitwrite (unsigned char **buf, char bit) {
  if (wbufferlevel != 0) { 
    writebyte = writebyte | (bit << wbufferlevel);
    wbufferlevel--;
  } else {
    writebyte = writebyte | (bit << wbufferlevel);
    *(*buf) = writebyte;
    *buf = *buf + 1;

    wbufferlevel=7;
    writebyte=0;
  }
}

static void bytewrite (unsigned char **buf, int byte) {
  int i;

  for (i=7; i >= 0; i--) {
    bitwrite (buf, (char) ((byte >> i) & 1));
  }
}

static void nbitwrite (unsigned char **buf, int somebits, int n) {
  int i;

  for (i=n-1; i >= 0; i--) {
    bitwrite (buf,(char) ((somebits >> i) & 1));
  }
}

static void writeflush (unsigned char **buf) {

  while (wbufferlevel != 7)
    bitwrite (buf, 0);
}

void *avi_converter_from_jmf (AviMovie *movie, int stream, unsigned char *buffer, int *size) {
  unsigned char *buf, *tbuffer, *temp2;
  int x=0, y=0, r=0, g=0, rc=0, gc=0, bc=0, b=0;
  int sign=0;

  readinit();

  buf = mallocN (movie->header->Width * movie->header->Height * 3, "fromjmfbuf");
  *size = movie->header->Width * movie->header->Height * 3;
  for (x=0; x < *size;x++)
    buf[x]=0;

  temp2 = buffer;

  tbuffer = buf;

  for (y=0; y < movie->header->Height; y++) {
    for (x=0; x < movie->header->Width; x++) {

      if (bitread (&temp2)) {
	r = rc;
	g = gc;
	b = bc;
      } else {
        if (bitread (&temp2)) {
          sign = (bitread(&temp2)) ? -1 : 1;
          
          r = sign * nbitread(&temp2,4) + rc;
          g = sign * nbitread(&temp2,4) + gc;
          b = sign * nbitread(&temp2,4) + bc;
        } else {
	  r = byteread(&temp2);
          g = byteread(&temp2);
          b = byteread(&temp2);
	}
      }

      rc = r;
      gc = g;
      bc = b;

      *(tbuffer++) = r;
      *(tbuffer++) = g;
      *(tbuffer++) = b;
    }
  }

  freeN (buffer);

  return buf;
}

void *avi_converter_to_jmf (AviMovie *movie, int stream, unsigned char *buffer, int *size) {
  int x, y;
  int rf=0, gf=0, bf=0;
  int rc=0, gc=0, bc=0;
  int rd=0, gd=0, bd=0;
#define SAME_PIXEL      1 << 0
#define KNOWN_DIFF      1 << 1
  int compression = SAME_PIXEL | KNOWN_DIFF;
  unsigned char *buf, *temp, *temp2;

  writeinit();

  buf = mallocN (*size, "tojmfbuf");
  memcpy (buf, buffer, *size);

  temp2 = buffer;
  temp = buf;

  for (y=0; y < movie->header->Height; y++) {
    for (x=0; x < movie->header->Width; x++) {
      rf = *(temp++);
      gf = *(temp++);
      bf = *(temp++);
      
      if (rf<0 || rf > 255)
	printf ("Red %d is OOR @ %d,%d\n",rf,x,y);
      if (gf<0 || gf > 255)
	printf ("Green %d is OOR @ %d,%d\n",gf,x,y);
      if (bf<0 || bf > 255)
	printf ("Blue %d is OOR @ %d,%d\n",bf,x,y);

      rd = rf - rc;
      gd = gf - gc;
      bd = bf - bc;
      
      if (rd == 0 &&
	  gd == 0  &&
	  bd == 0 &&
	  (compression & SAME_PIXEL)) {
	bitwrite (&temp2, 1);
      } else {
	bitwrite (&temp2, 0);

	if (rd <= 15 && gd <= 15 && bd <= 15 &&
	    rd >= 0 && gd >= 0 && bd >= 0 &&
	    (compression & KNOWN_DIFF)) {
	  bitwrite (&temp2, 1);
	  bitwrite (&temp2, 0);
	  
	  nbitwrite (&temp2, rd, 4);
	  nbitwrite (&temp2, gd, 4);
	  nbitwrite (&temp2, bd, 4);
	} else if (rd >= -15 && gd >= -15 && bd >= -15 &&
		   rd <= 0 && gd <= 0 && bd <= 0 &&
		   (compression & KNOWN_DIFF)) {
	  bitwrite (&temp2, 1);
	  bitwrite (&temp2, 1);
	  
	  nbitwrite (&temp2, abs(rd), 4);
	  nbitwrite (&temp2, abs(gd), 4);
	  nbitwrite (&temp2, abs(bd), 4);
	} else {
	  bitwrite (&temp2, 0);
	  
	  bytewrite (&temp2, rf);
	  bytewrite (&temp2, gf);
	  bytewrite (&temp2, bf);
	}
      }

      rc = rf;
      gc = gf;
      bc = bf;
    }
  }
  writeflush (&temp2);
  
  *size = temp2 - buffer;

  freeN (buf);

  return (buffer);
}

