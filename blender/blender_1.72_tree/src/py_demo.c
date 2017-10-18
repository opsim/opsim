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

/*  python.c      MIXED MODEL

 * 
 *  june 99
 */

#ifndef WITHOUT_PYTHON
 
#include "Python.h"
#include "py_blender.h"
#include "blender.h"
#include "radio.h"
#include "screen.h"

static PyObject *PyD_RunRadio (PyObject *self, PyObject *args)
{
	int ret;
	
	rad_collect_meshes();
	
	waitcursor(1);
	ret= rad_go();
	waitcursor(0);
	
	return Py_BuildValue("i", ret);
}

static PyObject *PyD_ReadFile (PyObject *self, PyObject *args)
{
	char *file, *lslash;
	
	Py_Try(PyArg_ParseTuple(args, "s", &file));
	
	if (fop_exists(file)) {
		add_readfile_event(file);
	} else {
		char relfile[FILE_MAXDIR+FILE_MAXFILE]="";
		
		strcpy(relfile, G.sce);
		
		lslash= (strrchr(relfile, '/')>strrchr(relfile, '\\'))?strrchr(relfile, '/'):strrchr(relfile, '\\');
		if (lslash) {
			strcpy(lslash+1, file);
		}
		
		if (fop_exists(relfile)) {
			add_readfile_event(relfile);
		} else {
			printf ("Unable to load file <%s> <%s>\n", file, relfile);
		}
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *PyD_PlayAnim (PyObject *self, PyObject *args)
{
	int ret;
	
	ret= play_anim(2);

	return Py_BuildValue("i", ret);
}

static PyObject *PyD_Render (PyObject *self, PyObject *args)
{
	int ret;
	
	ret= do_renderfg(0);

	return Py_BuildValue("i", ret);
}

static PyObject *PyB_SceneScripts (PyObject *self, PyObject *args)
{
	int on;
	
	Py_Try(PyArg_ParseTuple(args, "i", &on));

	if (on) G.f |= G_SCENESCRIPT;
	else G.f &= ~G_SCENESCRIPT;
	
	Py_INCREF(Py_None);
	return Py_None;	
}

static struct PyMethodDef DemoM_methods[] = {
	{"PlayAnim", PyD_PlayAnim, 1, NULL},
	{"Render", PyD_Render, 1, NULL},
	{"RunRadio", PyD_RunRadio, 1, NULL},
	{"ReadFile", PyD_ReadFile, 1, NULL},
	{"SceneScripts", PyB_SceneScripts, 1}, 
	{NULL, NULL}
};

PyObject *init_py_demo(void) 
{
	return Py_InitModule("Blender.Demo", DemoM_methods);
}

#endif /* !(WITHOUT_PYTHON) */

