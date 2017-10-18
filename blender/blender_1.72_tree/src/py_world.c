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

PyTypeObject World_Type;

static void World_dealloc(PyBlock *self) {
	PyMem_DEL(self);
}

static PyObject *World_getattr(PyBlock *self, char *name) {
	World *wo= (World *) self->data;
	
	if (STREQ(name, "HorR")) return PyFloat_FromDouble(wo->horr);
	else if (STREQ(name, "HorG")) return PyFloat_FromDouble(wo->horg);
	else if (STREQ(name, "HorB")) return PyFloat_FromDouble(wo->horb);
	else if (STREQ(name, "ZenR")) return PyFloat_FromDouble(wo->zenr);
	else if (STREQ(name, "ZenG")) return PyFloat_FromDouble(wo->zeng);
	else if (STREQ(name, "ZenB")) return PyFloat_FromDouble(wo->zenb);
	else if (STREQ(name, "Expos")) return PyFloat_FromDouble(wo->exposure);
	else if (STREQ(name, "MisSta")) return PyFloat_FromDouble(wo->miststa);
	else if (STREQ(name, "MisDi")) return PyFloat_FromDouble(wo->mistdist);
	else if (STREQ(name, "MisHi")) return PyFloat_FromDouble(wo->misthi);
	else if (STREQ(name, "StarDi")) return PyFloat_FromDouble(wo->stardist);
	else if (STREQ(name, "StarSi")) return PyFloat_FromDouble(wo->starsize);

	else if (STREQ(name, "name")) return PyString_FromString(wo->id.name+2);

	PyErr_SetString(PyExc_AttributeError, name);
	return NULL;
}

static int World_setattr(PyBlock *self, char *name, PyObject *v) {
	World *wo= (World *) self->data;
	float val;
	
	if (STREQ(name, "name")) {
		Py_Try(PyArg_Parse(v, "s", &name));
		
		new_id(&G.main->world, &wo->id, name);

	} else {
		Py_Try(PyArg_Parse(v, "f", &val));		

		if (STREQ(name, "HorR")) wo->horr= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "HorG")) wo->horg= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "HorB")) wo->horb= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "ZenR")) wo->zenr= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "ZenG")) wo->zeng= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "ZenB")) wo->zenb= CLAMPIS(val, 0.0, 1.0);
		else if (STREQ(name, "Expos")) wo->exposure= CLAMPIS(val, 0.0, 5.0);
		else if (STREQ(name, "MisSta")) wo->miststa= CLAMPIS(val, 0.0, 1000.0);
		else if (STREQ(name, "MisDi")) wo->mistdist= CLAMPIS(val, 0.0, 1000.0);
		else if (STREQ(name, "MisHi")) wo->misthi= CLAMPIS(val, 0.0, 100.0);
		else if (STREQ(name, "StarDi")) wo->stardist= CLAMPIS(val, 2.0, 1000.0);
		else if (STREQ(name, "StarSi")) wo->starsize= CLAMPIS(val, 0.0, 10.0);
		else {
			PyErr_SetString(PyExc_AttributeError, name);
			return -1;
		}
	}
	
	return 0;
}

static int World_print (PyBlock *self, FILE *fp, int flags) {
	World *wo= (World *) self->data;
	
	fprintf (fp, "[World %s]", wo->id.name+2);
	
	return 0;
}

PyTypeObject World_Type = {
	PyObject_HEAD_INIT(NULL)
	0,								/*ob_size*/
	"World",						/*tp_name*/
	sizeof(PyBlock),			/*tp_basicsize*/
	0,								/*tp_itemsize*/
	(destructor) World_dealloc,		/*tp_dealloc*/
	(printfunc) World_print,		/*tp_print*/
	(getattrfunc) World_getattr,	/*tp_getattr*/
	(setattrfunc) World_setattr,	/*tp_setattr*/
};

static char Method_Get_doc[]=
"([name]) - Get worlds from Blender\n\
[name] Return the world with the given name\n\
Returns a list of all worlds if no name is specified";

static PyObject *Method_Get (PyObject *self, PyObject *args)
{
	return py_find_from_list(G.main->world.first, ID_WO, args);
}

static char Method_GetActive_doc[]="() - Returns the active world";
static PyObject *Method_GetActive (PyObject *self, PyObject *args)
{
	if (G.scene->world) 
		return add_pyblock(ID_WO, G.scene->world);
	
	return py_incr_ret(Py_None);	
}

static struct PyMethodDef WorldM_methods[] = {
	{"Get", Method_Get, 1, Method_Get_doc},
	{"GetActive", Method_GetActive, 1, Method_GetActive_doc},
	{NULL, NULL}
};

PyObject *init_py_world(void) 
{
	World_Type.ob_type = &PyType_Type;		
	
	return Py_InitModule("Blender.World", WorldM_methods);
}

#endif /* !(WITHOUT_PYTHON) */

