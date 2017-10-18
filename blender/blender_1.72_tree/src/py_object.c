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
#include "ika.h"

PyTypeObject Object_Type;

static void Object_dealloc(PyBlock *self) {
	PyMem_DEL(self);
}

static PyObject *Object_getattr(PyBlock *self, char *name) {
	Object *ob= (Object *)self->data;
	
	if (STREQ(name, "LocX")) return PyFloat_FromDouble(ob->loc[0]);
	else if (STREQ(name, "LocY")) return PyFloat_FromDouble(ob->loc[1]);
	else if (STREQ(name, "LocZ")) return PyFloat_FromDouble(ob->loc[2]);
	else if (STREQ(name, "dLocX")) return PyFloat_FromDouble(ob->dloc[0]);
	else if (STREQ(name, "dLocY")) return PyFloat_FromDouble(ob->dloc[1]);
	else if (STREQ(name, "dLocZ")) return PyFloat_FromDouble(ob->dloc[2]);
	else if (STREQ(name, "loc")) return newVectorObject(ob->loc);
	else if (STREQ(name, "dloc")) return newVectorObject(ob->dloc);

	else if (STREQ(name, "RotX")) return PyFloat_FromDouble(ob->rot[0]);
	else if (STREQ(name, "RotY")) return PyFloat_FromDouble(ob->rot[1]);
	else if (STREQ(name, "RotZ")) return PyFloat_FromDouble(ob->rot[2]);
	else if (STREQ(name, "dRotX")) return PyFloat_FromDouble(ob->drot[0]);
	else if (STREQ(name, "dRotY")) return PyFloat_FromDouble(ob->drot[1]);
	else if (STREQ(name, "dRotZ")) return PyFloat_FromDouble(ob->drot[2]);
	else if (STREQ(name, "rot")) return newVectorObject(ob->rot);
	else if (STREQ(name, "drot")) return newVectorObject(ob->drot);

	else if (STREQ(name, "SizeX")) return PyFloat_FromDouble(ob->size[0]);
	else if (STREQ(name, "SizeY")) return PyFloat_FromDouble(ob->size[1]);
	else if (STREQ(name, "SizeZ")) return PyFloat_FromDouble(ob->size[2]);
	else if (STREQ(name, "dSizeX")) return PyFloat_FromDouble(ob->dsize[0]);
	else if (STREQ(name, "dSizeY")) return PyFloat_FromDouble(ob->dsize[1]);
	else if (STREQ(name, "dSizeZ")) return PyFloat_FromDouble(ob->dsize[2]);
	else if (STREQ(name, "size")) return newVectorObject(ob->size);
	else if (STREQ(name, "dsize")) return newVectorObject(ob->dsize);

	else if (STREQ(name, "Layer"))	return PyInt_FromLong(ob->lay);

	else if (STREQ(name, "name")) return PyString_FromString(ob->id.name+2);

	else if (strncmp(name, "Eff", 3)==0) {
		Ika *ika= ob->data;
		
		if (ob->type==OB_IKA && ika) {
			if (STREQ(name, "EffX")) return PyFloat_FromDouble(ika->effg[0]);
			else if (STREQ(name, "EffY")) return PyFloat_FromDouble(ika->effg[1]);
			else if (STREQ(name, "EffZ")) return PyFloat_FromDouble(ika->effg[2]);
		} else {
			return PyFloat_FromDouble(0.0);
		}
	} else if (STREQ(name, "mat")) {
		disable_where_script(1);
		where_is_object(ob);
		disable_where_script(0);

		return newMatrixObject((float *) ob->obmat);
		
	} else if (STREQ(name, "parent")) {
		if (ob->parent) return add_pyblock(ID_OB, ob->parent);
		else return py_incr_ret(Py_None);
		
	} else if (STREQ(name, "data")) {
		if(ob->data) {
			if (ob->type==OB_MESH)
				return newNMeshObject(ob->data);			
			else if (ob->type!=OB_EMPTY)
				return add_pyblock(GS(((ID *)ob->data)->name), ob->data);
		}
		
		Py_INCREF(Py_None);
		return Py_None;
	}
	
	PyErr_SetString(PyExc_AttributeError, name);
	return NULL;
}

static int Object_setattr(PyBlock *self, char *name, PyObject *v) {
	Object *ob= (Object *)self->data;
	Base *base;
	float fval;
	
	if (!v) return 0;
	
	if (STREQ2(name, "Layer", "layer")) {
		int ival;

		if (!PyArg_Parse(v, "i", &ival)) return -1;
		
		ob->lay= ival;
			
		base= FIRSTBASE;
		while (base) {
			if (base->object == ob) base->lay= ob->lay;
			base= base->next;
		}
		
	} else if (STREQ(name, "name")) {
		char *name;
		
		if (!PyArg_Parse(v, "s", &name)) return -1;
		
		new_id(&G.main->object, &ob->id, name);
		
	} else {
		if (!PyArg_Parse(v, "f", &fval)) return -1;
		
		if (STREQ(name, "LocX")) ob->loc[0]= fval;
		else if (STREQ(name, "LocY")) ob->loc[1]= fval;
		else if (STREQ(name, "LocZ")) ob->loc[2]= fval;
		else if (STREQ(name, "dLocX")) ob->dloc[0]= fval;
		else if (STREQ(name, "dLocY")) ob->dloc[1]= fval;
		else if (STREQ(name, "dLocZ")) ob->dloc[2]= fval;
	
		else if (STREQ(name, "RotX")) ob->rot[0]= fval;
		else if (STREQ(name, "RotY")) ob->rot[1]= fval;
		else if (STREQ(name, "RotZ")) ob->rot[2]= fval;
		else if (STREQ(name, "dRotX")) ob->drot[0]= fval;
		else if (STREQ(name, "dRotY")) ob->drot[1]= fval;
		else if (STREQ(name, "dRotZ")) ob->drot[2]= fval;
	
		else if (STREQ(name, "SizeX")) ob->size[0]= fval;
		else if (STREQ(name, "SizeY")) ob->size[1]= fval;
		else if (STREQ(name, "SizeZ")) ob->size[2]= fval;
		else if (STREQ(name, "dSizeX")) ob->dsize[0]= fval;
		else if (STREQ(name, "dSizeY")) ob->dsize[1]= fval;
		else if (STREQ(name, "dSizeZ")) ob->dsize[2]= fval;

		else if (strncmp(name, "Eff", 3)==0) {
			Ika *ika= ob->data;
		
			if ((name[3]!='X' && name[3]!='Y' && name[3]!='Z') || name[4]!=0) {
				PyErr_SetString(PyExc_AttributeError, name);
				return -1;
			} else if (ob->type==OB_IKA && ika) {
				if (name[3]=='X') ika->effg[0]= fval;
				else if (name[3]=='Y') ika->effg[1]= fval;
				else if (name[3]=='Z') ika->effg[2]= fval;
				
				itterate_ika(ob);
			}
		} else {
			PyErr_SetString(PyExc_AttributeError, name);
			return -1;	
		}
	}
	
	return 0;
}

static int Object_print (PyBlock *self, FILE *fp, int flags) {
	Object *ob= (Object *)self->data;

	fprintf (fp, "[Object %s at: <%f, %f, %f>]", ob->id.name+2, ob->loc[0], ob->loc[1], ob->loc[2], ob->obmat[3][0], ob->obmat[3][1], ob->obmat[3][2]);
	
	return 0;
}

PyTypeObject Object_Type = {
	PyObject_HEAD_INIT(NULL)
	0,								/*ob_size*/
	"Object",						/*tp_name*/
	sizeof(PyBlock),				/*tp_basicsize*/
	0,								/*tp_itemsize*/
	(destructor)Object_dealloc,		/*tp_dealloc*/
	(printfunc)Object_print,		/*tp_print*/
	(getattrfunc)Object_getattr,	/*tp_getattr*/
	(setattrfunc)Object_setattr,	/*tp_setattr*/
};

static char Member_Get_doc[]=
"([name]) - Get objects from Blender\n\
[name] Return the object with the given name\n\
Returns a list of all objects if name is not specified";

static PyObject *Member_Get (PyObject *self, PyObject *args)
{
	py_find_from_list(G.main->object.first, ID_OB, args);
}

static char Member_GetSelected_doc[]="() - Returns a list of all selected objects";
static PyObject *Member_GetSelected (PyObject *self, PyObject *args)
{
	PyObject *list;
	Object *tmp;
	
	list= PyList_New(0);
	
	tmp= G.main->object.first;
	while (tmp) {
		if (tmp->flag & SELECT) 
			PyList_Append(list, add_pyblock(ID_OB, tmp));	
			
		tmp= tmp->id.next;
	}
	
	return list;
}

static char Member_Update_doc[]="(name) - Updates the specified object during user-transformation";
static PyObject *Member_Update (PyObject *self, PyObject *args)
{
	Base *base;
	char *name= NULL;
		
	Py_Try(PyArg_ParseTuple(args, "s", &name));
	
	base= FIRSTBASE;			
	while (base) {
		if (strcmp(name, base->object->id.name+2)==0) 
			base->flag|= BA_WHERE_UPDATE|BA_PARSEL|BA_DISP_UPDATE;
					
		base= base->next;
	}

	Py_INCREF(Py_None);
	return Py_None;	
}

static struct PyMethodDef ObjectM_methods[] = {
	{"Get", Member_Get, 1, Member_Get_doc},
	{"GetSelected", Member_GetSelected, 1, Member_GetSelected_doc},
	{"Update", Member_Update, 1, Member_Update_doc},
	{NULL, NULL}
};

PyObject *init_py_object(void) 
{
	Object_Type.ob_type = &PyType_Type;		
	
	return Py_InitModule("Blender.Object", ObjectM_methods);
}

#endif /* !(WITHOUT_PYTHON) */

