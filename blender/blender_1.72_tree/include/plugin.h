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

/* plugin.h    dec 95

 * 
 * 
 */

#ifndef PLUGIN_H
#define PLUGIN_H

#include "Button.h"

#if !defined(__BeOS) && !defined(WIN32)
#include <dlfcn.h>
#endif
#include "iff.h"

typedef	int (*TexDoit)(int, void*, float*, float*, float*);
typedef void (*SeqDoit)(void*, float, float, int, int, ImBuf*, ImBuf*, ImBuf*, ImBuf*);

typedef struct VarStruct {
	int type;
	char name[16];
	float def, min, max;
	char tip[80];
} VarStruct;

typedef struct _PluginInfo {
	char *name;
	char *snames;

	int stypes;
	int nvars;
	VarStruct *varstr;
	float *result;
	float *cfra;

	void (*init)(void);
	void (*callback)(int);
	TexDoit tex_doit;
	SeqDoit seq_doit;
} PluginInfo;

int plugin_tex_getversion(void);
int plugin_seq_getversion(void);
void plugin_getinfo(PluginInfo *);

extern float hnoise(float noisesize, float x, float y, float z);
extern float hnoisep(float noisesize, float x, float y, float z);
extern float turbulence(float noisesize, float x, float y, float z, int nr);
extern float turbulence1(float noisesize, float x, float y, float z, int nr);

#endif /* PLUGIN_H */

