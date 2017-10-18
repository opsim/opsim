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

#ifndef OBJFNTDEF

#define OBJFNTDEF

typedef struct chardesc {
    short movex, movey;		/* advance */
    short llx, lly;		/* bounding box */
    short urx, ury;
    short *data;		/* char data */
    long datalen;		
} chardesc;

typedef struct objfnt {
    struct objfnt *freeaddr;	/* if freeaddr != 0, objfnt is one chunck */
    short type;
    short charmin, charmax;
    short my_nchars;
    short scale;
    chardesc *my_chars;
} objfnt;

#define OFMAGIC		0x93339333

#define TM_TYPE		1
#define PO_TYPE		2
#define SP_TYPE		3

/* ops for tmesh characters */

#define	TM_BGNTMESH	(1)
#define	TM_SWAPTMESH	(2)
#define	TM_ENDBGNTMESH	(3)
#define	TM_RETENDTMESH	(4)
#define	TM_RET		(5)

/* ops for poly characters */

#define	PO_BGNLOOP	(1)
#define	PO_ENDBGNLOOP	(2)
#define	PO_RETENDLOOP	(3)
#define	PO_RET		(4)

/* ops for spline  characters */

#define	SP_MOVETO	(1)
#define	SP_LINETO	(2)
#define	SP_CURVETO	(3)
#define	SP_CLOSEPATH	(4)
#define	SP_RETCLOSEPATH	(5)
#define	SP_RET		(6)


#define MIN_ASCII 	' '
#define MAX_ASCII 	'~'
#define NASCII		(256 - 32)

#define NOBBOX		(30000)

typedef struct pschar {
    char *name;
    int code;
    int prog;
} pschar;

extern pschar charlist[NASCII];

objfnt *fontname();
objfnt *readobjfnt();
objfnt *newobjfnt();
float fontstringwidth();
short *getcharprog();
chardesc *getchardesc();
char *asciiname();

#endif

