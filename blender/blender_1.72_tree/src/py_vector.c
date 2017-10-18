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
#include "screen.h"

/*****************************/
/*    Vector Python Object   */
/*****************************/

staticforward PyTypeObject Vector_Type;

#define VectorObject_Check(v)	((v)->ob_type == &Vector_Type)

PyObject *newVectorObject(float *vec) {
	VectorObject *self;
	
	self= PyObject_NEW(VectorObject, &Vector_Type);
	
	if(self!=NULL) {
		self->flags= 0;
		self->vec= vec;
	}
	
	return (PyObject*) self;
}

static VectorObject *newVectorObjectFree(void) {
	VectorObject *self;
	
	self= PyObject_NEW(VectorObject, &Vector_Type);
	
	if(self!=NULL) {
		self->flags= PYVECTOR_FREE;
		self->vec= mallocN(sizeof(float)*3, "vector object");
	}
	
	return self;
}

static void Vector_dealloc(VectorObject *self) {
	if (self->flags&PYVECTOR_FREE) freeN(self->vec);
	PyMem_DEL(self);
}

static PyObject *Vector_getattr(VectorObject *self, char *name) {
	PyObject *list;
	
	if (strcmp(name, "x")==0){
		return Py_BuildValue("f", self->vec[0]);
	} else if (strcmp(name, "y")==0){
		return Py_BuildValue("f", self->vec[1]);
	} else if (strcmp(name, "z")==0){
		return Py_BuildValue("f", self->vec[2]);
	}
}

static int Vector_setattr(VectorObject *self, char *name, PyObject *v) {
	float val;
	
	Py_Try(PyArg_Parse(v, "f", &val));

	if (strcmp(name, "x")==0)
		self->vec[0]= val;
	else if (strcmp(name, "y")==0)
		self->vec[1]= val;
	else if (strcmp(name, "z")==0)
		self->vec[2]= val;
	else
		return -1;
		
	return 0;
}

static int Vector_print (VectorObject *self, FILE *fp, int flags) {
	fprintf (fp, "[%f, %f, %f]", self->vec[0], self->vec[1], self->vec[2]);
	
	return 0;
}

/* Vectors Sequence methods */

static PyObject *Vector_item(VectorObject *a, int i)
{
	if (i < 0 || i >= 3) {
		PyErr_SetString(PyExc_IndexError, "array index out of range");
		return NULL;
	}
	return Py_BuildValue("f", a->vec[i]);
}

static int Vector_ass_item(VectorObject *self, int i, PyObject *v)
{
	float val;
	
	if (i < 0 || i >= 3) {
		PyErr_SetString(PyExc_IndexError, "array assignment index out of range");
		return -1;
	}
	
	if (!PyArg_Parse(v, "f;Coordinates must be floats", &val)) return -1;
	
	self->vec[i]= val;
	
	return 0;
}

static PySequenceMethods Vector_SeqMethods = {
	(inquiry) 0,				/*sq_length*/
	(binaryfunc) 0,						/*sq_concat*/
	(intargfunc) 0,						/*sq_repeat*/
	(intargfunc) Vector_item,			/*sq_item*/
	(intintargfunc) 0,					/*sq_slice*/
	(intobjargproc) Vector_ass_item,	/*sq_ass_item*/
	(intintobjargproc) 0,				/*sq_ass_slice*/
};

/* Vector Number methods */

static PyObject *Vector_add(VectorObject *a, VectorObject *b)
{
	VectorObject *newvec= newVectorObjectFree();
	
	if (!newvec) return NULL;

	VecAddf(newvec->vec, a->vec, b->vec);
	
	return (PyObject *) newvec;
}

static PyObject *Vector_sub(VectorObject *a, VectorObject *b)
{
	VectorObject *newvec= newVectorObjectFree();
	
	if (!newvec) return NULL;

	VecSubf(newvec->vec, a->vec, b->vec);
	
	return (PyObject *) newvec;
}

static PyObject *Vector_mul(VectorObject *a, VectorObject *b)
{
	VectorObject *newvec= newVectorObjectFree();

	if (!newvec) return NULL;

	newvec->vec[0]= a->vec[0]*b->vec[0];
	newvec->vec[1]= a->vec[1]*b->vec[1];
	newvec->vec[2]= a->vec[2]*b->vec[2];
	
	return (PyObject *) newvec;
}

static PyObject *Vector_div(VectorObject *a, VectorObject *b)
{
	VectorObject *newvec= newVectorObjectFree();

	if (!newvec) return NULL;

	newvec->vec[0]= a->vec[0]/b->vec[0];
	newvec->vec[1]= a->vec[1]/b->vec[1];
	newvec->vec[2]= a->vec[2]/b->vec[2];
	
	return (PyObject *) newvec;
}

static PyObject *Vector_neg(VectorObject *a)
{
	VectorObject *newvec= newVectorObjectFree();

	if (!newvec) return NULL;

	newvec->vec[0]= -a->vec[0];
	newvec->vec[1]= -a->vec[1];
	newvec->vec[2]= -a->vec[2];
	
	return (PyObject *) newvec;
}

static PyObject *Vector_pos(VectorObject *a)
{
	Py_INCREF(a);
	return (PyObject *) a;
}

static PyObject *Vector_xor(VectorObject *a, VectorObject *b)
{
	VectorObject *newvec= newVectorObjectFree();

	if (!newvec) return NULL;
	
	Crossf(newvec->vec, a->vec, b->vec);
		
	return (PyObject *) newvec;
}

static int Vector_coerce(PyObject **pv, PyObject **pw)
{
	VectorObject *newvec;
	float val;

	if (PyFloat_Check(*pw)) {
		val= PyFloat_AsDouble(*pw);
	} else if (PyLong_Check(*pw)) {
		val= (float) PyLong_AsLong(*pw);
	} else if (PyInt_Check(*pw)) {
		val= (float) PyInt_AsLong(*pw);
	} else {
		return 1;
	}
	
	newvec= newVectorObjectFree();

	newvec->vec[0]= newvec->vec[1]= newvec->vec[2]= val;
		
	*pw= (PyObject*) newvec;

	return 0;
}

static PyNumberMethods Vector_NumMethods = {
	(binaryfunc)	Vector_add,		/* nb_add */
	(binaryfunc)	Vector_sub,		/* nb_subtract */
	(binaryfunc)	Vector_mul,		/* nb_multiply */
	(binaryfunc)	Vector_div,		/* nb_divide */
	(binaryfunc)	0,		/* nb_remainder */
	(binaryfunc)	0,		/* nb_divmod */
	(ternaryfunc)	0,		/* nb_power */
	(unaryfunc)		Vector_neg,		/* nb_negative */
	(unaryfunc)		Vector_pos,		/* nb_positive */
	(unaryfunc)		0,		/* nb_absolute */
	(inquiry)		0,		/* nb_nonzero */
	(unaryfunc)		0,		/* nb_invert */
	(binaryfunc)	0,		/* nb_lshift */
	(binaryfunc)	0,		/* nb_rshift */
	(binaryfunc)	0,		/* nb_and */
	(binaryfunc)	Vector_xor,		/* nb_xor */
	(binaryfunc)	0,		/* nb_or */
	(coercion)		Vector_coerce,		/* nb_coerce */
	(unaryfunc)		0,		/* nb_int */
	(unaryfunc)		0,		/* nb_long */
	(unaryfunc)		0,		/* nb_float */
	(unaryfunc)		0,		/* nb_oct */
	(unaryfunc)		0,		/* nb_hex */
};

statichere PyTypeObject Vector_Type = {
	PyObject_HEAD_INIT(NULL)
	0,								/*ob_size*/
	"Vector",						/*tp_name*/
	sizeof(VectorObject),			/*tp_basicsize*/
	0,								/*tp_itemsize*/
	/* methods */
	(destructor)Vector_dealloc,		/*tp_dealloc*/
	(printfunc)Vector_print,		/*tp_print*/
	(getattrfunc)Vector_getattr,	/*tp_getattr*/
	(setattrfunc)Vector_setattr,	/*tp_setattr*/
	0,								/*tp_compare*/
	0,								/*tp_repr*/
	&Vector_NumMethods,				/*tp_as_number*/
	&Vector_SeqMethods,				/*tp_as_sequence*/
	0,								/*tp_as_mapping*/
	0,								/*tp_hash*/
	0,								/*tp_call*/
	0,								/*tp_str*/
	
	0L,0L,0L,0L,					/* Space for future expansion */
	"Type for handling vector operations"
};

/* Vector Object handling routines */

static char PyV_New_doc[]=
"([x, y, z]) - returns a new vector \n\
[x=0.0, y=0.0, z=0.0] set the initial values of the vector";
PyObject *PyV_New(PyObject *self, PyObject *args) {
	VectorObject *newvec;
	float v[3]={0.0, 0.0, 0.0};
	
	Py_Try(PyArg_ParseTuple(args, "|fff", &v[0], &v[1], &v[2]));
	
	newvec= PyObject_NEW(VectorObject, &Vector_Type);
	
	if(newvec!=NULL) {
		newvec->flags= PYVECTOR_FREE;
		newvec->vec= mallocN(sizeof(float)*3, "vector object");
		
		VECCOPY(newvec->vec, v);
	}
	
	return (PyObject*) newvec;
}

static struct PyMethodDef VectorM_methods[] = {
	{"New", PyV_New, METH_VARARGS, PyV_New_doc},
	{NULL, NULL}
};

PyObject *init_py_vector(void) 
{
	Vector_Type.ob_type = &PyType_Type;
	
	return Py_InitModule3("Blender.Vector", VectorM_methods, 
		"Fast vector manipulation and handling library");
}

#endif /* !(WITHOUT_PYTHON) */

