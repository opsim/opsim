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
#include "graphics.h"
#include "screen.h"

/* GUI interface routinges */

static PyObject *PyI_Error (PyObject *self, PyObject *args)
{
	char *name;

	Py_Try(PyArg_ParseTuple(args, "s", &name));

	error(name);
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *PyI_Okay (PyObject *self, PyObject *args)
{
	char *name;

	Py_Try(PyArg_ParseTuple(args, "s", &name));

	return Py_BuildValue("i", okee(name));
}

static PyObject *PyI_Confirm (PyObject *self, PyObject *args)
{
	char *name, *title;

	Py_Try(PyArg_ParseTuple(args, "ss", &title, &name));

	return Py_BuildValue("i", confirm(title, name));
}

static PyObject *PyI_Dialog (PyObject *self, PyObject *args)
{	
	float f1, f2, f3;
	PyObject *ret;
	PyObject *buttons;
	PyObject *butrec;
	char *name, *title, *tip;
	void **data;
	int i, len;
	short type, but;
	float min, max;
	
	PyFloatObject *fl_tmp;
	
	Py_Try(PyArg_ParseTuple(args, "sO", &title, &buttons));
	
	len= PyList_Size(buttons);
	
	data= mallocN(sizeof(void*)*len, "numbut data");
	
	for (i=0; i<len; i++) {
		butrec= PyList_GetItem(buttons, i);
		
		type= PyInt_AsLong(PyList_GetItem(butrec, 0));
		but= type & BUTTYPE;
		type= type & ~BUTTYPE;
		
		name= PyString_AsString(PyList_GetItem(butrec, 1));
		
		min= PyFloat_AsDouble(PyList_GetItem(butrec, 3));
		max= PyFloat_AsDouble(PyList_GetItem(butrec, 4));
		
		if (type == FLO) {
			data[i]= mallocN(sizeof(float), "float data");
			*(float*)data[i]= (float) PyFloat_AsDouble(PyList_GetItem(butrec, 2));
		} else if (type == INT) {
			data[i]= mallocN(sizeof(int), "integer data");
			*(int*)data[i]= (int) PyInt_AsLong(PyList_GetItem(butrec, 2));
		} else if (but & TEX) {
			data[i]= mallocN(sizeof(char)*((int)max+1), "char data");
			strncpy((char*)data[i], PyString_AsString(PyList_GetItem(butrec, 2)), (int)max);
			((char*)data[i])[(int)max]= 0;				
		} else {
			data[i]= NULL;
		}

		tip= PyString_AsString(PyList_GetItem(butrec, 5));
		
		add_numbut(i, but|type, name, min, max, data[i], tip);
	}
	
	if (!do_clever_numbuts(title, i, REDRAW)) {
		for (i=0; i<len; i++) 
			if(data[i]) freeN(data[i]);
		freeN(data);
		
		return py_incr_ret(Py_None);
	}

	ret= PyList_New(0);
	for (i=0; i<len; i++) {
		butrec= PyList_GetItem(buttons, i);
		
		type= PyInt_AsLong(PyList_GetItem(butrec, 0));
		but= type & BUTTYPE;
		type= type & ~BUTTYPE;

		if (type == FLO) {
			PyList_Append(ret, Py_BuildValue("f", *(float*)data[i]));
		} else if (type == INT) {
			PyList_Append(ret, Py_BuildValue("i", *(int*)data[i]));
		} else if (but & TEX) {
			PyList_Append(ret, Py_BuildValue("s", (char*)data[i]));
		} else {
			PyList_Append(ret, py_incr_ret(Py_None));
		}
		
		if(data[i]) freeN(data[i]);
	}
	
	freeN(data);
		
	return ret;
}

static struct PyMethodDef InterfaceM_methods[] = {
	{"Error", PyI_Error, 1},
	{"Okay", PyI_Okay, 1},
	{"Confirm", PyI_Confirm, 1},
	{"Dialog", PyI_Dialog, 1},
	{NULL, NULL}
};

PyObject *init_py_interface(void) 
{
	return Py_InitModule("Blender.Interface", InterfaceM_methods);
}

#endif /* !(WITHOUT_PYTHON) */

