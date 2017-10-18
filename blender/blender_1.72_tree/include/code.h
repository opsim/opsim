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

/* code.h    april 99

 * 
 * 
 */

#ifndef CODE_H
#define CODE_H

extern char bdata[], cdata[];
extern int (*cfunc[])(void);
	
#define TEST_C_KEY	(cfunc[ 0 ])()	/* without error message */
#define TEST1_C_KEY	(cfunc[ 1 ])()
#define ADD_RADIO				(cfunc[ cdata[57] ])
#define AUTOSMOOTH				(cfunc[ cdata[56] ])
#define WRITE_VRML_FS			(cfunc[ cdata[55] ])
#define WRITE_DXF_FS			(cfunc[ cdata[54] ])
#define DRAW_PROP_CIRCLE		(cfunc[ cdata[53] ])
#define SCRIPTBUTS				(cfunc[ cdata[52] ])


#endif

