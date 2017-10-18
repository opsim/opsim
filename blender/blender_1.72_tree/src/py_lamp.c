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

PyTypeObject Lamp_Type;

static void Lamp_dealloc(PyBlock *self) {
	PyMem_DEL(self);
}

static PyObject *Lamp_getattr(PyBlock *self, char *name) {
	Lamp *la= (Lamp*) self->data;
	
	if (STREQ(name, "R")) return PyFloat_FromDouble(la->r);
	else if (STREQ(name, "G")) return PyFloat_FromDouble(la->g);
	else if (STREQ(name, "B")) return PyFloat_FromDouble(la->b);
	else if (STREQ(name, "Energ")) return PyFloat_FromDouble(la->energy);
	else if (STREQ(name, "Dist")) return PyFloat_FromDouble(la->dist);
	else if (STREQ(name, "SpoSi")) return PyFloat_FromDouble(la->spotsize);
	else if (STREQ(name, "SpoBl")) return PyFloat_FromDouble(la->spotblend);
	else if (STREQ(name, "HaInt")) return PyFloat_FromDouble(la->haint);
	else if (STREQ(name, "Quad1")) return PyFloat_FromDouble(la->att1);
	else if (STREQ(name, "Quad2")) return PyFloat_FromDouble(la->att2);

	else if (STREQ(name, "name")) return PyString_FromString(la->id.name+2);

	PyErr_SetString(PyExc_AttributeError, name);
	return NULL;
}

static int Lamp_setattr(PyBlock *self, char *name, PyObject *v) {
	float val;
	Lamp *la= (Lamp*) self->data;
	
	if (!v) return 0;

	if (STREQ(name, "name")) {
		char *name;
		
		if (!PyArg_Parse(v, "s", &name)) return -1;
		
		new_id(&G.main->lamp, &la->id, name);
		
		return 0;
	}	
	else {
		if (!PyArg_Parse(v, "f", &val)) return -1;

		if (STREQ(name, "R")) la->r= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "G")) la->g= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "B")) la->b= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "Energ")) la->energy= CLAMPIS(val, 0.0, 10.0);
		else if (STREQ(name, "Dist")) la->dist= CLAMPIS(val, 0.01, 5000.0);
		else if (STREQ(name, "SpoSi")) la->spotsize= CLAMPIS(val, 1.0, 180.0);
		else if (STREQ(name, "SpoBl")) la->spotblend= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "HaInt")) la->haint= CLAMPIS(val, 1.0, 5.0);
		else if (STREQ(name, "Quad1")) la->att1= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "Quad2")) la->att2= CLAMPIS(val, 0.0, 1.0);
		else {
			PyErr_SetString(PyExc_AttributeError, name);
			return -1;
		}
	}
	
	return 0;
}

static int Lamp_print (PyBlock *self, FILE *fp, int flags) {
	Lamp *la= (Lamp*) self->data;
	
	fprintf (fp, "[Lamp %s]", la->id.name+2);
	
	return 0;
}

PyTypeObject Lamp_Type = {
	PyObject_HEAD_INIT(NULL)
	0,								/*ob_size*/
	"Lamp",							/*tp_name*/
	sizeof(PyBlock),				/*tp_basicsize*/
	0,								/*tp_itemsize*/
	(destructor) Lamp_dealloc,		/*tp_dealloc*/
	(printfunc) Lamp_print,			/*tp_print*/
	(getattrfunc) Lamp_getattr,		/*tp_getattr*/
	(setattrfunc) Lamp_setattr,		/*tp_setattr*/
};

static char Method_Get_doc[]=
"([name]) - Get lamps from Blender\n\
[name] Return the lamp with the given name\n\
Returns a list of all lamps if name is not specified";

static PyObject *Method_Get (PyObject *self, PyObject *args)
{
	py_find_from_list(G.main->lamp.first, ID_LA, args);
}

static struct PyMethodDef LampM_methods[] = {
	{"Get", Method_Get, 1, Method_Get_doc},
	{NULL, NULL}
};

PyObject *init_py_lamp(void) 
{
	Lamp_Type.ob_type = &PyType_Type;		
	
	return Py_InitModule("Blender.Lamp", LampM_methods);
}

#endif /* !(WITHOUT_PYTHON) */

