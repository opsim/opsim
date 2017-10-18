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

#include <stdio.h>

#include "avi.h"

#define AVI_RAW      0
#define AVI_CHUNK    1
#define AVI_LIST     2
#define AVI_MAINH    3
#define AVI_STREAMH  4
#define AVI_BITMAPH  5
#define AVI_INDEXE   6
#define AVI_MJPEGU   7

void awrite (AviMovie *movie, void *datain, int block, int size, FILE *fp, int type);

