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
/*    Matrix Python Object   */
/*****************************/

staticforward PyTypeObject Matrix_Type;
staticforward PyTypeObject SubMatrix_Type;

SubMatrixObject *newSubMatrixObject(float *mat) {
	SubMatrixObject *self;
	
	self= PyObject_NEW(SubMatrixObject, &SubMatrix_Type);	
	if(!self) return (SubMatrixObject *) PyErr_NoMemory();

	self->mat= mat;
	
	return self;
}

static void SubMatrix_dealloc(SubMatrixObject *self) {
	PyMem_DEL(self);
}

static PyObject *SubMatrix_getattr(SubMatrixObject *self, char *name) {
	PyErr_SetString(PyExc_AttributeError, name);
	
	return NULL;
}

static int SubMatrix_setattr(SubMatrixObject *self, char *name, PyObject *v) {
	PyErr_SetString(PyExc_AttributeError, name);

	return -1;
}

static int SubMatrix_print (SubMatrixObject *self, FILE *fp, int flags) {
	fprintf (fp, "[%f, %f, %f, %f]", self->mat[0], self->mat[1], self->mat[2], self->mat[3]);

	return 0;
}

static PyObject *SubMatrix_item(SubMatrixObject *a, int i)
{
	if (i < 0 || i >= 4) {
		PyErr_SetString(PyExc_IndexError, "array index out of range");
		return NULL;
	}
	return Py_BuildValue("f", a->mat[i]);
}

static int SubMatrix_ass_item(SubMatrixObject *self, int i, PyObject *v)
{
	float val;
	
	if (i < 0 || i >= 4) {
		PyErr_SetString(PyExc_IndexError, "array index out of range");
		return -1;
	}
	
	if (!PyArg_Parse(v, "f;Coordinates must be floats", &val)) return -1;
	
	self->mat[i]= val;
	
	return 0;
}

static PySequenceMethods SubMatrix_SeqMethods = {
	(inquiry) 0,				/*sq_length*/
	(binaryfunc) 0,						/*sq_concat*/
	(intargfunc) 0,						/*sq_repeat*/
	(intargfunc) SubMatrix_item,			/*sq_item*/
	(intintargfunc) 0,					/*sq_slice*/
	(intobjargproc) SubMatrix_ass_item,	/*sq_ass_item*/
	(intintobjargproc) 0,				/*sq_ass_slice*/
};

statichere PyTypeObject SubMatrix_Type = {
	PyObject_HEAD_INIT(NULL)
	0,								/*ob_size*/
	"SubMatrix",						/*tp_name*/
	sizeof(SubMatrixObject),			/*tp_basicsize*/
	0,								/*tp_itemsize*/
	/* methods */
	(destructor)SubMatrix_dealloc,		/*tp_dealloc*/
	(printfunc)SubMatrix_print,		/*tp_print*/
	(getattrfunc)SubMatrix_getattr,	/*tp_getattr*/
	(setattrfunc)SubMatrix_setattr,	/*tp_setattr*/
	0,								/*tp_compare*/
	0,								/*tp_repr*/
	0,								/*tp_as_number*/
	&SubMatrix_SeqMethods,			/*tp_as_sequence*/
	0,								/*tp_as_mapping*/
	0,								/*tp_hash*/
	0,								/*tp_call*/
	0,								/*tp_str*/
	
	0L,0L,0L,0L,					/* Space for future expansion */
	"Type for handling SubMatrix operations"
};

PyObject *newMatrixObject(float *mat) {
	MatrixObject *self;
	
	self= PyObject_NEW(MatrixObject, &Matrix_Type);
	if(!self) return PyErr_NoMemory();

	self->flags= 0;
	self->mat= mat;
		
	self->smats= mallocN(sizeof(SubMatrixObject*)*4, "submatrices");
	if(!self->smats) return PyErr_NoMemory();

	Py_Try(self->smats[0]= newSubMatrixObject(self->mat));
	Py_Try(self->smats[1]= newSubMatrixObject(self->mat+4));
	Py_Try(self->smats[2]= newSubMatrixObject(self->mat+8));
	Py_Try(self->smats[3]= newSubMatrixObject(self->mat+12));
	
	return (PyObject*) self;
}

MatrixObject *newMatrixObjectFree(void) {
	MatrixObject *self;
	int i;
	
	self= PyObject_NEW(MatrixObject, &Matrix_Type);
	if(!self) return (MatrixObject *) PyErr_NoMemory();
	
	self->flags= PYMATRIX_FREE;
	self->mat= mallocN(sizeof(float)*16, "freematrix");
	if(!self->mat) return (MatrixObject *) PyErr_NoMemory();
		
	for (i=0; i<16; i++) self->mat[i]= 0.0;
		
	self->smats= mallocN(sizeof(SubMatrixObject*)*4, "submatrices");
	if(!self->smats) return (MatrixObject *) PyErr_NoMemory();

	Py_Try(self->smats[0]= newSubMatrixObject(self->mat));
	Py_Try(self->smats[1]= newSubMatrixObject(self->mat+4));
	Py_Try(self->smats[2]= newSubMatrixObject(self->mat+8));
	Py_Try(self->smats[3]= newSubMatrixObject(self->mat+12));
	
	return self;
}

static PyObject *Matrix_scale(PyObject *selfob, PyObject *args) 
{
	MatrixObject *self= (MatrixObject *) selfob;
	float val[3];	
	float mat[3][3];
	float mat2[4][4];
	float mat3[4][4];
	
	Py_Try(PyArg_ParseTuple(args, "fff", &val[0], &val[1], &val[2]));
	
	Mat3One(mat);
	Mat4One(mat2);

	SizeToMat3(val, mat);
	Mat4CpyMat3(mat2, mat);
	
	Mat4CpyMat4(mat3, self->mat);
	Mat4MulMat4(self->mat, mat2, mat3);
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *Matrix_rotate(PyObject *selfob, PyObject *args) 
{
	MatrixObject *self= (MatrixObject *) selfob;
	float val[3];	
	float mat[4][4];
	float mat2[4][4];
	
	Py_Try(PyArg_ParseTuple(args, "fff", &val[0], &val[1], &val[2]));
	
	Mat4One(mat);
	EulToMat4(val, mat);
	
	Mat4CpyMat4(mat2, self->mat);
	Mat4MulMat4(self->mat, mat, mat2);
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *Matrix_translate(PyObject *selfob, PyObject *args) 
{
	MatrixObject *self= (MatrixObject *) selfob;
	float val[3];	
	float mat[4][4];
	
	Py_Try(PyArg_ParseTuple(args, "fff", &val[0], &val[1], &val[2]));
	
	VecAddf(self->mat+12, self->mat+12, val);
		
	Py_INCREF(Py_None);
	return Py_None;
}

static struct PyMethodDef Matrix_methods[] = {
	{"translate", Matrix_translate, 1}, 
	{"scale", Matrix_scale, 1}, 
	{"rotate", Matrix_rotate, 1}, 
	{NULL, NULL}
};

static void Matrix_dealloc(MatrixObject *self) {
	Py_DECREF(self->smats[0]);
	Py_DECREF(self->smats[1]);
	Py_DECREF(self->smats[2]);
	Py_DECREF(self->smats[3]);

	freeN(self->smats);
	
	if (self->flags & PYMATRIX_FREE) freeN(self->mat);
	
	PyMem_DEL(self);
}

static PyObject *Matrix_getattr(MatrixObject *self, char *name) {
	PyObject *list=NULL;
	PyObject *item;
	float val[3];
	float mat3[3][3];
	
	if (strcmp(name, "rot")==0) {
		Py_Try(list= PyList_New(3));
		
		Mat3CpyMat4(mat3, self->mat);
		Mat3ToEul(mat3, val);

		Py_Try(item= Py_BuildValue("f", val[0]));
		PyList_SetItem(list, 0, item);

		Py_Try(item= Py_BuildValue("f", val[1]));
		PyList_SetItem(list, 1, item);

		Py_Try(item= Py_BuildValue("f", val[2]));
		PyList_SetItem(list, 2, item);
		
		return list;
	} else if (strcmp(name, "size")==0) {
		Py_Try(list= PyList_New(3));
		
		Mat4ToSize(self->mat, val);

		Py_Try(item= Py_BuildValue("f", val[0]));
		PyList_SetItem(list, 0, item);

		Py_Try(item= Py_BuildValue("f", val[1]));
		PyList_SetItem(list, 1, item);

		Py_Try(item= Py_BuildValue("f", val[2]));
		PyList_SetItem(list, 2, item);
		
		return list;
	} else if (strcmp(name, "loc")==0) {
		Py_Try(list= PyList_New(3));

		Py_Try(item= Py_BuildValue("f", self->mat[12]));
		PyList_SetItem(list, 0, item);

		Py_Try(item= Py_BuildValue("f", self->mat[13]));
		PyList_SetItem(list, 1, item);

		Py_Try(item= Py_BuildValue("f", self->mat[14]));
		PyList_SetItem(list, 2, item);
		
		return list;
	}
	
	return Py_FindMethod(Matrix_methods, (PyObject*)self, name);
}

static int Matrix_setattr(MatrixObject *self, char *name, PyObject *v) {
	return -1;
}

static int Matrix_print (MatrixObject *self, FILE *fp, int flags) {
	float *smat;
	
	smat= self->smats[0]->mat;
	fprintf (fp, "[%f, %f, %f, %f], ", smat[0], smat[1], smat[2], smat[3]);
	smat= self->smats[1]->mat;
	fprintf (fp, "[%f, %f, %f, %f], ", smat[0], smat[1], smat[2], smat[3]);
	smat= self->smats[2]->mat;
	fprintf (fp, "[%f, %f, %f, %f], ", smat[0], smat[1], smat[2], smat[3]);
	smat= self->smats[3]->mat;
	fprintf (fp, "[%f, %f, %f, %f]", smat[0], smat[1], smat[2], smat[3]);

	return 0;
}

static PyObject *Matrix_item(MatrixObject *a, int i)
{
	if (i < 0 || i >= 4) {
		PyErr_SetString(PyExc_IndexError, "array index out of range");
		return NULL;
	}
	return Py_BuildValue("O", a->smats[i]);
}

static PySequenceMethods Matrix_SeqMethods = {
	(inquiry) 0,				/*sq_length*/
	(binaryfunc) 0,				/*sq_concat*/
	(intargfunc) 0,				/*sq_repeat*/
	(intargfunc) Matrix_item,	/*sq_item*/
	(intintargfunc) 0,			/*sq_slice*/
	(intobjargproc) 0,			/*sq_ass_item*/
	(intintobjargproc) 0,		/*sq_ass_slice*/
};

static PyObject *Matrix_mul(MatrixObject *a, MatrixObject *b)
{
	MatrixObject *newmat= newMatrixObjectFree();

	if (!newmat) return NULL;
	
	Mat4MulMat4(newmat->mat, a->mat, b->mat);

	return (PyObject *) newmat;
}

static PyNumberMethods Matrix_NumMethods = {
	(binaryfunc)	0,		/* nb_add */
	(binaryfunc)	0,		/* nb_subtract */
	(binaryfunc)	Matrix_mul,		/* nb_multiply */
	(binaryfunc)	0,		/* nb_divide */
	(binaryfunc)	0,		/* nb_remainder */
	(binaryfunc)	0,		/* nb_divmod */
	(ternaryfunc)	0,		/* nb_power */
	(unaryfunc)		0, 		/* nb_negative */
	(unaryfunc)		0, 		/* nb_positive */
	(unaryfunc)		0,		/* nb_absolute */
	(inquiry)		0,		/* nb_nonzero */
	(unaryfunc)		0,		/* nb_invert */
	(binaryfunc)	0,		/* nb_lshift */
	(binaryfunc)	0,		/* nb_rshift */
	(binaryfunc)	0,		/* nb_and */
	(binaryfunc)	0,		/* nb_xor */
	(binaryfunc)	0,		/* nb_or */
	(coercion)		0,		/* nb_coerce */
	(unaryfunc)		0,		/* nb_int */
	(unaryfunc)		0,		/* nb_long */
	(unaryfunc)		0,		/* nb_float */
	(unaryfunc)		0,		/* nb_oct */
	(unaryfunc)		0,		/* nb_hex */
};

statichere PyTypeObject Matrix_Type = {
	PyObject_HEAD_INIT(NULL)
	0,								/*ob_size*/
	"Matrix",						/*tp_name*/
	sizeof(MatrixObject),			/*tp_basicsize*/
	0,								/*tp_itemsize*/
	/* methods */
	(destructor)Matrix_dealloc,		/*tp_dealloc*/
	(printfunc)Matrix_print,		/*tp_print*/
	(getattrfunc)Matrix_getattr,	/*tp_getattr*/
	(setattrfunc)Matrix_setattr,	/*tp_setattr*/
	0,								/*tp_compare*/
	0,								/*tp_repr*/
	&Matrix_NumMethods,				/*tp_as_number*/
	&Matrix_SeqMethods,				/*tp_as_sequence*/
	0,								/*tp_as_mapping*/
	0,								/*tp_hash*/
	0,								/*tp_call*/
	0,								/*tp_str*/
	
	0L,0L,0L,0L,					/* Space for future expansion */
	"Type for handling Matrix operations"
};

static struct PyMethodDef MatrixM_methods[] = {
	{NULL, NULL}
};

PyObject *init_py_matrix(void) 
{
	Matrix_Type.ob_type = &PyType_Type;
	SubMatrix_Type.ob_type = &PyType_Type;
	
	return Py_InitModule3("Blender.Matrix", MatrixM_methods, 
		"Fast matrix manipulation and handling library");
}

#endif /* !(WITHOUT_PYTHON) */

