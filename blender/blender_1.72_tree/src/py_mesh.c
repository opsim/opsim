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
/*    Mesh Python Object   */
/*****************************/

PyTypeObject Mesh_Type;

static void Mesh_dealloc(PyBlock *self) {
	PyMem_DEL(self);
}

static PyObject *Mesh_add(PyBlock *self, PyObject *args) {
	Mesh *me= (Mesh *) self->data;
	MVert *tmp;
	float vec[3];
		
	if (!PyArg_ParseTuple(args, "fff", &vec[0], &vec[1], &vec[2]))
		return NULL;

	tmp= mallocN(sizeof(MVert)*(me->totvert+1), "mverts");
	if (me->totvert) {
		memcpy(tmp, me->mvert, sizeof(MVert)*me->totvert);
		freeN(me->mvert);
	}
	me->mvert= tmp;
	
	me->mvert[me->totvert].co[0]= vec[0];
	me->mvert[me->totvert].co[1]= vec[1];
	me->mvert[me->totvert].co[2]= vec[2];
	
	me->totvert++;
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef Mesh_methods[] = {
	{"add",	(PyCFunction)Mesh_add,	1},
	{NULL,		NULL}		/* sentinel */
};


static PyObject *Mesh_getattr(PyBlock *self, char *name) {
	PyObject *list;
	Mesh *me= (Mesh *) self->data;
	
	if (strcmp(name, "name")==0) {
		return Py_BuildValue("s", me->id.name);
	} else if (strcmp(name, "verts")==0) {
		PyObject *list;
		PyObject *vert;
		MVert *mvert;
		
		int i;
		
		list= PyList_New(me->totvert);
		
		mvert= me->mvert; i=0;
		while (i<me->totvert) {
			vert= newVectorObject(mvert->co);

			PyList_SetItem(list, i, vert);
			
			mvert++; i++;			
		}
		
		return list;
	} else if (strcmp(name, "faces")==0) {
		PyObject *faces;
		PyObject *face;
		PyObject *vert;
		MVert *mverts;
		MFace *mface;
		int i;
		
		faces= PyList_New(me->totface);
		
		mverts= me->mvert;
		
		mface= me->mface; i=0;
		while (i<me->totface) {
			if (mface->v4) {
				face= PyList_New(4);
			
				vert= newVectorObject(mverts[mface->v1].co);
				PyList_SetItem(face, 0, vert);

				vert= newVectorObject(mverts[mface->v2].co);
				PyList_SetItem(face, 1, vert);					

				vert= newVectorObject(mverts[mface->v3].co);
				PyList_SetItem(face, 2, vert);

				vert= newVectorObject(mverts[mface->v4].co);
				PyList_SetItem(face, 3, vert);
			} else if (mface->v3) {
				face= PyList_New(3);
			
				vert= newVectorObject(mverts[mface->v1].co);
				PyList_SetItem(face, 0, vert);

				vert= newVectorObject(mverts[mface->v2].co);
				PyList_SetItem(face, 1, vert);					

				vert= newVectorObject(mverts[mface->v3].co);
				PyList_SetItem(face, 2, vert);					
			} else {
				face= PyList_New(2);
			
				vert= newVectorObject(mverts[mface->v1].co);
				PyList_SetItem(face, 0, vert);

				vert= newVectorObject(mverts[mface->v2].co);
				PyList_SetItem(face, 1, vert);				
			}
			
			PyList_SetItem(faces, i, face);
			
			mface++; i++;
		}
		return faces;
	}

	return Py_FindMethod(Mesh_methods, (PyObject *)self, name);
}

static int Mesh_setattr(PyBlock *self, char *name, PyObject *v) {
	return -1;
}

static int Mesh_print (PyBlock *self, FILE *fp, int flags) {
	Mesh *me= (Mesh *) self->data;

	fprintf (fp, "[Mesh %s]", me->id.name);
	
	return 0;
}

PyTypeObject Mesh_Type = {
	PyObject_HEAD_INIT(NULL)
	0,							/*ob_size*/
	"Mesh",						/*tp_name*/
	sizeof(PyBlock),			/*tp_basicsize*/
	0,							/*tp_itemsize*/
	/* methods */
	(destructor) Mesh_dealloc,	/*tp_dealloc*/
	(printfunc) Mesh_print,		/*tp_print*/
	(getattrfunc) Mesh_getattr,	/*tp_getattr*/
	(setattrfunc) Mesh_setattr,	/*tp_setattr*/
	0,							/*tp_compare*/
	0,							/*tp_repr*/
	0,							/*tp_as_number*/
	0,							/*tp_as_sequence*/
	0,							/*tp_as_mapping*/
	0,							/*tp_hash*/
};

/* Mesh Object handling routines */

/* ZR - WRITE THIS ! */

static char PyM_Get_doc[]=
"([name]) - Get meshes from Blender\n\
[name] Returns the mesh with the given name\n\
Returns a list of all meshes if no name is specified";
static PyObject *PyM_Get(PyObject *self, PyObject *args) 
{
	return py_find_from_list(G.main->mesh.first, ID_ME, args);	
}

static char PyM_GetNew_doc[]=
"(name) - Create a new object\n\
Returns a new mesh with the specified name";
static PyObject *PyM_GetNew(PyObject *self, PyObject *args)
{
	char *name;
	Mesh *tmp;
	PyBlock *new;
	
	Py_Try(PyArg_ParseTuple(args, "s", &name));

	new= new_pyblock(ID_ME);
	new->data= add_mesh();
	
	G.totmesh++;
	rename_id((ID *)new->data, name);
	((ID *)new->data)->us--;
	
	return (PyObject *)new;
}

static struct PyMethodDef MeshM_methods[] = {
	{"Get", PyM_Get, 1, PyM_Get_doc}, 
	{"GetNew", PyM_GetNew, 1, PyM_GetNew_doc}, 
	{NULL, NULL}
};

PyObject *init_py_mesh(void) 
{
	Mesh_Type.ob_type = &PyType_Type;	
	
	return Py_InitModule("Blender.Mesh", MeshM_methods);
}

#endif /* !(WITHOUT_PYTHON) */

