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

PyTypeObject Camera_Type;

static void Camera_dealloc(PyBlock *self) {
	PyMem_DEL(self);
}

static PyObject *Camera_getattr(PyBlock *self, char *name) {
	Camera *ca= (Camera*) self->data;
	
	if (STREQ(name, "Lens")) return PyFloat_FromDouble(ca->lens);
	else if (STREQ(name, "ClSta")) return PyFloat_FromDouble(ca->clipsta);
	else if (STREQ(name, "ClEnd")) return PyFloat_FromDouble(ca->clipend);

	else if (STREQ(name, "name")) return PyString_FromString(ca->id.name+2);
	
	PyErr_SetString(PyExc_AttributeError, name);
	return NULL;
}

static int Camera_setattr(PyBlock *self, char *name, PyObject *v) {
	float val;
	Camera *ca= (Camera*) self->data;
	
	if (!v) return 0;

	if (STREQ(name, "name")) {
		char *name;
		
		if (!PyArg_Parse(v, "s", &name)) return -1;
		
		new_id(&G.main->camera, &ca->id, name);
		
		return 0;
	}	
	else {
		if (!PyArg_Parse(v, "f", &val)) return -1;
		
		if (STREQ(name, "Lens")) ca->lens= CLAMPIS(val, 1.0, 250.0);
		else if (STREQ(name, "ClSta")) ca->clipsta= CLAMPIS(val, 0.0, 100.0);
		else if (STREQ(name, "ClEnd")) ca->clipend= CLAMPIS(val, 1.0, 5000.0);
		else {
			PyErr_SetString(PyExc_AttributeError, name);
			return -1;
		}
	}
	
	return 0;
}

static int Camera_print (PyBlock *self, FILE *fp, int flags) {
	Camera *ca= (Camera*) self->data;
	
	fprintf (fp, "[Camera %s]", ca->id.name+2);
	
	return 0;
}

PyTypeObject Camera_Type = {
	PyObject_HEAD_INIT(NULL)
	0,								/*ob_size*/
	"Camera",						/*tp_name*/
	sizeof(PyBlock),				/*tp_basicsize*/
	0,								/*tp_itemsize*/
	(destructor) Camera_dealloc,	/*tp_dealloc*/
	(printfunc) Camera_print,		/*tp_print*/
	(getattrfunc) Camera_getattr,	/*tp_getattr*/
	(setattrfunc) Camera_setattr,	/*tp_setattr*/
};


static char Method_Get_doc[]=
"([name]) - Get cameras from Blender\n\
[name] Return the camera with the given name\n\
Returns a list of all cameras if name is not specified";

static PyObject *Method_Get (PyObject *self, PyObject *args)
{
	py_find_from_list(G.main->camera.first, ID_CA, args);
}

static struct PyMethodDef CameraM_methods[] = {
	MethodDef(Get),
	{NULL, NULL}
};

PyObject *init_py_camera(void) 
{
	Camera_Type.ob_type = &PyType_Type;		
	
	return Py_InitModule("Blender.Camera", CameraM_methods);
}

#endif /* !(WITHOUT_PYTHON) */

