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

PyTypeObject Material_Type;

static void Material_dealloc(PyBlock *self) {
	PyMem_DEL(self);
}

static PyObject *Material_getattr(PyBlock *self, char *name) {
	Material *ma= (Material*) self->data;
	
	if (STREQ(name, "R")) return PyFloat_FromDouble(ma->r);
	else if (STREQ(name, "G")) return PyFloat_FromDouble(ma->g);
	else if (STREQ(name, "B")) return PyFloat_FromDouble(ma->b);
	else if (STREQ(name, "SpecR")) return PyFloat_FromDouble(ma->specr);
	else if (STREQ(name, "SpecG")) return PyFloat_FromDouble(ma->specg);
	else if (STREQ(name, "SpecB")) return PyFloat_FromDouble(ma->specb);
	else if (STREQ(name, "MirR")) return PyFloat_FromDouble(ma->mirr);
	else if (STREQ(name, "MirG")) return PyFloat_FromDouble(ma->mirg);
	else if (STREQ(name, "MirB")) return PyFloat_FromDouble(ma->mirb);
	else if (STREQ(name, "Ref")) return PyFloat_FromDouble(ma->ref);
	else if (STREQ(name, "Alpha")) return PyFloat_FromDouble(ma->alpha);
	else if (STREQ(name, "Emit")) return PyFloat_FromDouble(ma->emit);
	else if (STREQ(name, "Amb")) return PyFloat_FromDouble(ma->amb);
	else if (STREQ(name, "Spec")) return PyFloat_FromDouble(ma->spec);
	else if (STREQ(name, "SpTra")) return PyFloat_FromDouble(ma->spectra);
	else if (STREQ(name, "HaSize")) return PyFloat_FromDouble(ma->hasize);
	
	else if (STREQ(name, "Hard")) return PyInt_FromLong(ma->har);
	else if (STREQ(name, "Mode")) return PyInt_FromLong(ma->b);

	else if (STREQ(name, "name")) return PyString_FromString(ma->id.name+2);

	PyErr_SetString(PyExc_AttributeError, name);
	return NULL;
}

static int Material_setattr(PyBlock *self, char *name, PyObject *v) {
	Material *ma= (Material*) self->data;
	float val;
	int ival;

	if (!v) return 0;
	
	if (STREQ(name, "name")) {
		char *name;
		
		if (!PyArg_Parse(v, "s", &name)) return -1;
		
		new_id(&G.main->mat, &ma->id, name);
		
		return 0;
	}	
	else if (STREQ2(name, "Mode", "Hard")) {
		if (!PyArg_Parse(v, "i", &ival)) return -1;

		if (STREQ(name, "Mode")) ma->mode= val;
		else ma->har= CLAMPIS(val, 1, 128);
		
		return 0;
	} else {
		if (!PyArg_Parse(v, "f", &val)) return -1;

		if (STREQ(name, "R")) ma->r= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "G")) ma->g= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "B")) ma->b= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "SpecR")) ma->specr= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "SpecG")) ma->specg= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "SpecB")) ma->specb= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "MirR")) ma->mirr= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "MirG")) ma->mirg= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "MirB")) ma->mirb= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "Ref")) ma->ref= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "Alpha")) ma->alpha= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "Emit")) ma->emit= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "Amb")) ma->amb= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "Spec")) ma->spec= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "SpTra")) ma->spectra= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "HaSize")) ma->hasize= CLAMPIS(val, 0.0, 10000.0);	
		else {
			PyErr_SetString(PyExc_AttributeError, name);
			return -1;
		}
	}
	
	return 0;
}

static int Material_print (PyBlock *self, FILE *fp, int flags) {
	Material *ma= (Material*) self->data;

	fprintf (fp, "[Material %s]", ma->id.name);
	
	return 0;
}

PyTypeObject Material_Type = {
	PyObject_HEAD_INIT(NULL)
	0,								/*ob_size*/
	"Material",						/*tp_name*/
	sizeof(PyBlock),			/*tp_basicsize*/
	0,								/*tp_itemsize*/
	(destructor)Material_dealloc,		/*tp_dealloc*/
	(printfunc)Material_print,		/*tp_print*/
	(getattrfunc)Material_getattr,	/*tp_getattr*/
	(setattrfunc)Material_setattr,	/*tp_setattr*/
};

static char Method_Get_doc[]=
"([name]) - Get cameras from Blender\n\
[name] Return the camera with the given name\n\
Returns a list of all cameras if no name is specified";

static PyObject *Method_Get (PyObject *self, PyObject *args)
{
	py_find_from_list(G.main->mat.first, ID_MA, args);
}

static struct PyMethodDef MaterialM_methods[] = {
	{"Get", Method_Get, 1, Method_Get_doc},
	{NULL, NULL}
};

PyObject *init_py_material(void) 
{
	Material_Type.ob_type = &PyType_Type;		
	
	return Py_InitModule("Blender.Material", MaterialM_methods);
}

#endif /* !(WITHOUT_PYTHON) */

