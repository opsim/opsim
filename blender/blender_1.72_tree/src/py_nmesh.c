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

staticforward PyTypeObject NMesh_Type;
staticforward PyTypeObject NMFace_Type;
staticforward PyTypeObject NMVert_Type;
staticforward PyTypeObject NMCol_Type;

#define NMesh_Check(v)       ((v)->ob_type == &NMesh_Type)
#define NMFace_Check(v)      ((v)->ob_type == &NMFace_Type)
#define NMVert_Check(v)      ((v)->ob_type == &NMVert_Type)
#define NMCol_Check(v)       ((v)->ob_type == &NMCol_Type)

/*****************************/
/*	    Mesh Color Object    */
/*****************************/

typedef struct _NMCol {
	PyObject_VAR_HEAD

	unsigned char r, g, b, a;
} NMCol;

static void NMCol_dealloc(PyObject *self) {
	PyMem_DEL(self);
}

static NMCol *newcol (char r, char g, char b, char a) {
	NMCol *mc= (NMCol *) PyObject_NEW(NMCol, &NMCol_Type);
	
	mc->r= r;
	mc->g= g;
	mc->b= b;
	mc->a= a;

	return mc;	
}

static char Method_Col_doc[]=
"([r, g, b, a]) - Get a new mesh color\n\
[r=255, g=255, b=255, a=255] Specify the color components";

static PyObject *Method_Col(PyObject *self, PyObject *args) {
	NMCol *mc;
	int r=255, g=255, b=255, a=255;
	
	Py_Try(PyArg_ParseTuple(args, "|iiii", &r, &g, &b, &a));
	
	return (PyObject *) newcol(r, g, b, a);
}

static PyObject *NMCol_getattr(PyObject *self, char *name) {
	NMCol *mc= (NMCol *) self;
		
	if (strcmp(name, "r")==0) return Py_BuildValue("i", mc->r);
	else if (strcmp(name, "g")==0) return Py_BuildValue("i", mc->g);
	else if (strcmp(name, "b")==0) return Py_BuildValue("i", mc->b);
	else if (strcmp(name, "a")==0) return Py_BuildValue("i", mc->a);

	PyErr_SetString(PyExc_AttributeError, name);
	return NULL;
}

static int NMCol_setattr(PyObject *self, char *name, PyObject *v) {
	NMCol *mc= (NMCol *) self;
	int ival;
	
	PyArg_Parse(v, "i", &ival);
	CLAMP(ival, 0, 255);
	
	if (strcmp(name, "r")==0) mc->r= ival;
	else if (strcmp(name, "g")==0) mc->g= ival;
	else if (strcmp(name, "b")==0) mc->b= ival;
	else if (strcmp(name, "a")==0) mc->a= ival;
	else return -1;
	
	return 0;
}

static int NMCol_print (PyObject *self, FILE *fp, int flags) {
	NMCol *mc= (NMCol *) self;
		
	fprintf (fp, "[Color - <%d, %d, %d, %d>]", mc->r, mc->g, mc->b, mc->a);
	
	return 0;
}

statichere PyTypeObject NMCol_Type = {
	PyObject_HEAD_INIT(NULL)
	0,							/*ob_size*/
	"NMCol",						/*tp_name*/
	sizeof(NMCol),			/*tp_basicsize*/
	0,							/*tp_itemsize*/
	/* methods */
	(destructor) NMCol_dealloc,	/*tp_dealloc*/
	(printfunc) NMCol_print,		/*tp_print*/
	(getattrfunc) NMCol_getattr,	/*tp_getattr*/
	(setattrfunc) NMCol_setattr,/*tp_setattr*/
	0,							/*tp_compare*/
	0,							/*tp_repr*/
	0,							/*tp_as_number*/
	0,							/*tp_as_sequence*/
	0,							/*tp_as_mapping*/
	0,							/*tp_hash*/
};

/*****************************/
/*    NMesh Python Object    */
/*****************************/

typedef struct _NMFace {
	PyObject_VAR_HEAD
	
	PyObject *v;
	
	char mat_nr;
		
	PyObject *col;
} NMFace;

static void NMFace_dealloc(PyObject *self) {
	NMFace *mf= (NMFace *) self;
	
	Py_DECREF(mf->v);
	Py_DECREF(mf->col);
	
	PyMem_DEL(self);

}

static NMFace *newface(void) {
	NMFace *mf= PyObject_NEW(NMFace, &NMFace_Type);

	mf->v= PyList_New(0);
	mf->col= PyList_New(0);

	mf->mat_nr= 0;
	
	return mf;
}

static char Method_Face_doc[]=
"() - Get a new face";
static PyObject *Method_Face(PyObject *self, PyObject *args) {
	Py_Try(PyArg_ParseTuple(args, ""));
	
	return (PyObject *) newface();
}

static PyObject *NMFace_getattr(PyObject *self, char *name) {
	PyObject *list;
	NMFace *mf= (NMFace *) self;
		
	if(strcmp(name, "v")==0) 
		return Py_BuildValue("O", mf->v);
	else if (strcmp(name, "col")==0) 
		return Py_BuildValue("O", mf->col);
	else if (strcmp(name, "mat")==0) 
		return Py_BuildValue("i", mf->mat_nr);
	
	PyErr_SetString(PyExc_AttributeError, name);
	return NULL;
}

static int NMFace_setattr(PyObject *self, char *name, PyObject *v) {
	NMFace *mf= (NMFace *) self;
	PyObject *ob;
	int ival;
	
	if (STREQ(name, "v")) {
		PyArg_Parse(v, "O", &ob);
	
		if(PyList_Check(ob)) {
			Py_DECREF(mf->v);
			mf->v= py_incr_ret(ob);

			return 0;
		} else PyErr_SetString(PyExc_AttributeError, name);			
		
	} else if (STREQ(name, "col")) {
		PyArg_Parse(v, "O", &ob);
	
		if(PyList_Check(ob)) {
			Py_DECREF(mf->col);
			mf->col= py_incr_ret(ob);

			return 0;
		} else PyErr_SetString(PyExc_AttributeError, name);			
	
	} else if (STREQ(name, "mat")) {
		PyArg_Parse(v, "i", &ival);

		mf->mat_nr= ival;
		
		return 0;
	}
	
	return -1;
}

static int NMFace_print (PyObject *self, FILE *fp, int flags) {
	NMFace *mf= (NMFace *) self;
		
	fprintf (fp, "[Face - Mat: %d]", mf->mat_nr);
	
	return 0;
}

statichere PyTypeObject NMFace_Type = {
	PyObject_HEAD_INIT(NULL)
	0,							/*ob_size*/
	"NMFace",						/*tp_name*/
	sizeof(NMFace),			/*tp_basicsize*/
	0,							/*tp_itemsize*/
	/* methods */
	(destructor) NMFace_dealloc,	/*tp_dealloc*/
	(printfunc) NMFace_print,		/*tp_print*/
	(getattrfunc) NMFace_getattr,	/*tp_getattr*/
	(setattrfunc) NMFace_setattr,/*tp_setattr*/
	0,							/*tp_compare*/
	0,							/*tp_repr*/
	0,							/*tp_as_number*/
	0,							/*tp_as_sequence*/
	0,							/*tp_as_mapping*/
	0,							/*tp_hash*/
};

typedef struct _NMVert {
	PyObject_VAR_HEAD

	float co[3];
	float no[3];
	float uvco[3];
	
	int index;
} NMVert;

static void NMVert_dealloc(PyObject *self) {
	PyMem_DEL(self);
}

static NMVert *newvert(float *co) {
	NMVert *mv= PyObject_NEW(NMVert, &NMVert_Type);

	VECCOPY(mv->co, co);
	mv->no[0]= mv->no[1]= mv->no[2]= 0.0;
	mv->uvco[0]= mv->uvco[1]= mv->uvco[2]= 0.0;
	
	return mv;
}

static char Method_Vert_doc[]=
"([x, y, z]) - Get a new vertice\n\
[x, y, z] Specify new coordinates";

static PyObject *Method_Vert(PyObject *self, PyObject *args) {
	float co[3]= {0.0, 0.0, 0.0};
	
	Py_Try(PyArg_ParseTuple(args, "|fff", &co[0], &co[1], &co[2]));
	
	return (PyObject *) newvert(co);
}

static PyObject *NMVert_getattr(PyObject *self, char *name) {
	PyObject *list;
	NMVert *mv= (NMVert *) self;

	if (STREQ(name, "co")) return newVectorObject(mv->co);
	else if (STREQ(name, "no")) return newVectorObject(mv->no);		
	else if (STREQ(name, "uvco")) return newVectorObject(mv->uvco);		
	else if (STREQ(name, "index")) return PyInt_FromLong(mv->index);		
	
	PyErr_SetString(PyExc_AttributeError, name);
	return NULL;
}

static int NMVert_setattr(PyObject *self, char *name, PyObject *v) {
	NMVert *mv= (NMVert *) self;
	int i;
	

	if (STREQ(name,"index")) {
		PyArg_Parse(v, "i", &i);
		mv->index= i;
		return 0;
	}
	
	return -1;
}

static int NMVert_print (PyObject *self, FILE *fp, int flags) {
	NMVert *mv= (NMVert *) self;

	fprintf (fp, "[Vert - <%f, %f, %f>]", mv->co[0], mv->co[1], mv->co[2]);
	
	return 0;
}

statichere PyTypeObject NMVert_Type = {
	PyObject_HEAD_INIT(NULL)
	0,							/*ob_size*/
	"NMVert",						/*tp_name*/
	sizeof(NMVert),			/*tp_basicsize*/
	0,							/*tp_itemsize*/
	/* methods */
	(destructor) NMVert_dealloc,	/*tp_dealloc*/
	(printfunc) NMVert_print,		/*tp_print*/
	(getattrfunc) NMVert_getattr,	/*tp_getattr*/
	(setattrfunc) NMVert_setattr,/*tp_setattr*/
	0,							/*tp_compare*/
	0,							/*tp_repr*/
	0,							/*tp_as_number*/
	0,							/*tp_as_sequence*/
	0,							/*tp_as_mapping*/
	0,							/*tp_hash*/
};

#define NMESH_HASMCOL	1<<0
#define NMESH_HASUVCO	1<<1

typedef struct _NMesh {
	PyObject_VAR_HEAD

	PyObject *verts;
	PyObject *faces;
	
	char flags;
} NMesh;

static void NMesh_dealloc(PyObject *self) {
	NMesh *me= (NMesh *) self;

	Py_DECREF(me->verts);
	Py_DECREF(me->faces);
	
	PyMem_DEL(self);
}

static PyObject *NMesh_getattr(PyObject *self, char *name) {
	NMesh *me= (NMesh *) self;

	if (STREQ(name, "verts"))
		return Py_BuildValue("O", me->verts);
		
	else if (STREQ(name, "faces"))
		return Py_BuildValue("O", me->faces);
		
	else if (STREQ(name, "has_col")) {
		if (me->flags & NMESH_HASMCOL) {
			Py_INCREF (Py_True);
			return Py_True;
		} else {			
			Py_INCREF (Py_False);
			return Py_False;
		}
		
	} else if (STREQ(name, "has_uvco")) {
		if (me->flags & NMESH_HASUVCO) {
			Py_INCREF (Py_True);
			return Py_True;
		} else {			
			Py_INCREF (Py_False);
			return Py_False;
		}
	}

	PyErr_SetString(PyExc_AttributeError, name);
	return NULL;
}

static int NMesh_setattr(PyObject *self, char *name, PyObject *v) {
	NMesh *me= (NMesh *) self;
	PyObject *ob;
	
	if (STREQ2(name, "has_col", "has_uvco")) {
		int flag, ival;
		
		if(STREQ(name, "has_col")) flag= NMESH_HASMCOL;
		else flag= NMESH_HASUVCO;
		
		PyArg_Parse(v, "i", &ival);

		if(ival) me->flags |= flag;
		else me->flags &= ~flag;
		
	} else if (STREQ2(name, "verts", "faces")) {
		PyArg_Parse(v, "O", &ob);
	
		if(PyList_Check(ob)) {
			if(STREQ(name, "verts")) {
				Py_DECREF(me->verts);
				me->verts= py_incr_ret(ob);
			} else {
				Py_DECREF(me->faces);
				me->faces= py_incr_ret(ob);				
			}
		} else {
			PyErr_SetString(PyExc_AttributeError, name);
			return -1;
		}
	} else return -1;

	return 0;
}

static int NMesh_print (PyObject *self, FILE *fp, int flags) {
	NMesh *me= (NMesh *) self;
	int nfaces=0, nverts=0;
	
	if (PyList_Check(me->verts)) nverts= PyList_Size(me->verts);
	if (PyList_Check(me->faces)) nfaces= PyList_Size(me->faces);
	
	fprintf (fp, "[NMesh - Verts:%d Faces:%d]", nverts, nfaces);
	
	return 0;
}

statichere PyTypeObject NMesh_Type = {
	PyObject_HEAD_INIT(NULL)
	0,							/*ob_size*/
	"NMesh",						/*tp_name*/
	sizeof(NMesh),			/*tp_basicsize*/
	0,							/*tp_itemsize*/
	/* methods */
	(destructor) NMesh_dealloc,	/*tp_dealloc*/
	(printfunc) NMesh_print,		/*tp_print*/
	(getattrfunc) NMesh_getattr,	/*tp_getattr*/
	(setattrfunc) NMesh_setattr,/*tp_setattr*/
	0,							/*tp_compare*/
	0,							/*tp_repr*/
	0,							/*tp_as_number*/
	0,							/*tp_as_sequence*/
	0,							/*tp_as_mapping*/
	0,							/*tp_hash*/
};

PyObject *newNMeshObject(Mesh *oldmesh) 
{
	NMesh *me;
	NMVert *mv;
	NMFace *mf;
	NMCol *mc;
	MFace *oldmf=NULL;
	MVert *oldmv=NULL;
	MSticky *oldst=NULL;
	MCol *oldmc;
	int i, j, len;
	PyObject *vert;
	
	me= PyObject_NEW(NMesh, &NMesh_Type);
	me->flags= 0;
	
	if (oldmesh==NULL) {
		me->verts= PyList_New(0);
		me->faces= PyList_New(0);
	} else {
		oldmv= oldmesh->mvert;
		oldst= oldmesh->msticky;
		oldmc= oldmesh->mcol;
		
		if (oldst) me->flags |= NMESH_HASUVCO;
		if (oldmc) me->flags |= NMESH_HASMCOL;
		
		me->verts= PyList_New(oldmesh->totvert);
		for (i=0; i<oldmesh->totvert; i++) {
			mv= PyObject_NEW(NMVert, &NMVert_Type);
			
			VECCOPY (mv->co, oldmv->co);
		
			mv->no[0]= oldmv->no[0]/32767.0;
			mv->no[1]= oldmv->no[1]/32767.0;
			mv->no[2]= oldmv->no[2]/32767.0;
	
			if (oldst) {
				mv->uvco[0]= oldst->co[0];
				mv->uvco[1]= oldst->co[1];
				mv->uvco[2]= 0.0;
	
				oldst++;
			} else mv->uvco[0]= mv->uvco[1]= mv->uvco[2]= 0.0;
			
			mv->index= i;
			
			PyList_SetItem(me->verts, i, (PyObject *) mv);
			oldmv++;
		}
		
		oldmf= oldmesh->mface;
		oldmc= oldmesh->mcol;
		me->faces= PyList_New(oldmesh->totface);
		for (i=0; i<oldmesh->totface; i++) {
			mf= PyObject_NEW(NMFace, &NMFace_Type);
			
			if(oldmf->v4) mf->v= PyList_New(4);
			else if(oldmf->v3) mf->v= PyList_New(3);
			else mf->v= PyList_New(2);
	
			vert= PyList_GetItem(me->verts, oldmf->v1);	Py_INCREF(vert);
			PyList_SetItem(mf->v, 0, vert);
			vert= PyList_GetItem(me->verts, oldmf->v2);	Py_INCREF(vert);
			PyList_SetItem(mf->v, 1, vert);
			
			if(oldmf->v3) {
				vert= PyList_GetItem(me->verts, oldmf->v3);	Py_INCREF(vert);
				PyList_SetItem(mf->v, 2, vert);
			}
			
			if(oldmf->v4) {
				vert= PyList_GetItem(me->verts, oldmf->v4);	Py_INCREF(vert);
				PyList_SetItem(mf->v, 3, vert);
			}

			mf->mat_nr= oldmf->mat_nr;
			
			if (oldmc==NULL) {
				mf->col= PyList_New(0);
			} else {
				mf->col= PyList_New(4);
				
				for(j=0; j<4; j++, oldmc++)
					PyList_SetItem(mf->col, j, (PyObject *)newcol(oldmc->b, oldmc->g, oldmc->r, oldmc->a));
			}
			
			PyList_SetItem(me->faces, i, (PyObject *) mf);
			oldmf++;
		}
	}
	
	return (PyObject *) me;	
}

static char Method_GetRaw_doc[]=
"([name]) - Get a raw mesh from Blender\n\n\
If name is not specified a new empty mesh is\n\
returned, otherwise Blender returns an existing\n\
mesh.";

static PyObject *Method_GetRaw(PyObject *self, PyObject *args) 
{
	char *name=NULL;
	Mesh *oldmesh=NULL;
	
	Py_Try(PyArg_ParseTuple(args, "|s", &name));	

	if(name) {
		oldmesh= G.main->mesh.first;
	
		while (oldmesh) {
			if (strcmp(name, oldmesh->id.name+2)==0) break;
			
			oldmesh= oldmesh->id.next;
		}
		
		if (oldmesh==NULL) {
			Py_INCREF(Py_None);
			return Py_None;			
		}
	}
	
	return newNMeshObject(oldmesh);
}

/* This is a very nasty function, for several reasons, it
needs to be reworked and rewritten (but i have no time now).
one thing - make it fail instead of trying to always create
a reasonable mesh! - Zr */

static char Method_PutRaw_doc[]=
"(mesh, [name, renormal]) - Return a raw mesh to Blender\n\
(mesh) The NMesh object to store\n\
[name] The mesh to replace\n\
[renormal] Flag to control vertex normal recalculation\n\n\
If the name of a mesh to replace is not given a new\n\
object is created and returned.";

static PyObject *Method_PutRaw(PyObject *self, PyObject *args) 
{
	char *name= NULL;
	Mesh *nmesh= NULL;
	Object *ob= NULL;
	NMesh *me;
	NMVert *mv;
	NMFace *mf;
	NMCol *mc;
	MFace *oldmf= NULL;
	MVert *newmv= NULL;
	MSticky *newst= NULL;
	MCol *newmc= NULL;
	int i, j, len;
	int recalc_normals= 1;
	
	Py_Try(PyArg_ParseTuple(args, "O|si", &me, &name, &recalc_normals));

	if (!NMesh_Check(me)) {
		PyErr_SetString(PyExc_AttributeError, "object is not a mesh");
		return NULL;
	}
	
	if (name) {
		nmesh= G.main->mesh.first;
	
		while (nmesh) {
			if(strcmp(name, nmesh->id.name+2)==0) break;

			nmesh= nmesh->id.next;
		}
	}
	if(!nmesh || nmesh->id.us==0) {
		ob= add_object(OB_MESH);
		if (nmesh) {
			set_mesh(ob, nmesh);
	
		} else nmesh= (Mesh *) ob->data;
		
		ob->loc[0]= ob->loc[1]= ob->loc[2]= 0.0;
		ob->rot[0]= ob->rot[1]= ob->rot[2]= 0.0;
		ob->size[0]= ob->size[1]= ob->size[2]= 1.0;
	}
	if(name) new_id(&G.main->mesh, &nmesh->id, name);
	
	freedisplist(&nmesh->disp);
	if(nmesh->mvert) {
		freeN(nmesh->mvert);
		nmesh->mvert= NULL;
	}
	if(nmesh->mface) {
		freeN(nmesh->mface);
		nmesh->mface= NULL;
	}
			
	if(nmesh->msticky) {
		freeN(nmesh->msticky);
		nmesh->msticky= NULL;
	}
			
	if(nmesh->mcol) {
		freeN(nmesh->mcol);
		nmesh->mcol= NULL;
	}

	nmesh->totvert= nmesh->totface= 0;
	
	if(PyList_Check(me->verts)) nmesh->totvert= PyList_Size(me->verts);
	if(PyList_Check(me->faces)) nmesh->totface= PyList_Size(me->faces);

	if (nmesh->totvert) {
		if (me->flags&NMESH_HASUVCO)
			nmesh->msticky= callocN(sizeof(MSticky)*(nmesh->totvert+1), "msticky");

		nmesh->mvert= callocN(sizeof(MVert)*(nmesh->totvert+1), "mverts");

		if (nmesh->totface) {
			if (me->flags&NMESH_HASMCOL)
				nmesh->mcol= callocN(4*sizeof(MCol)*(nmesh->totface+1), "mcol");
			
			nmesh->mface= callocN(sizeof(MFace)*(nmesh->totface+1), "mfaces");
		}
	}

	/* Set all indexes to -1 so we can make sure verts really exist in mesh */
	for (i=0; i<nmesh->totface; i++) {
		mf= (NMFace *) PyList_GetItem(me->faces, i);
		if (!NMFace_Check(mf)) continue;
			
		j= PyList_Size(mf->v);
		while (j--) {
			mv= (NMVert *) PyList_GetItem(mf->v, j);
			if (mv && NMVert_Check(mv)) mv->index= -1;
		}
	}
	
	newmv= nmesh->mvert;
	newst= nmesh->msticky;
	for (i=0; i<nmesh->totvert; i++, newmv++, newst++) {
		mv= (NMVert *) PyList_GetItem(me->verts, i);
		if (!NMVert_Check(mv)) continue;
		
		mv->index= i;
		
		VECCOPY (newmv->co, mv->co);
		newmv->no[0]= mv->no[0]*32767.0;
		newmv->no[1]= mv->no[1]*32767.0;
		newmv->no[2]= mv->no[2]*32767.0;
		
		newmv->flag= 0;
		newmv->mat_nr= 0;

		if (me->flags&NMESH_HASUVCO) {
			newst->co[0]= mv->uvco[0];
			newst->co[1]= mv->uvco[1];	
		}
	}
	if (nmesh->totvert) {
		newmc= nmesh->mcol;
		oldmf= nmesh->mface;
		for (i=0; i<nmesh->totface; i++, oldmf++) {
			mf= (NMFace *) PyList_GetItem(me->faces, i);
			if (!NMFace_Check(mf)) continue;
			
			j= PyList_Size(mf->v);
			if(j>0) {
				mv= (NMVert *) PyList_GetItem(mf->v, 0);
				if (mv && NMVert_Check(mv) && mv->index!=-1) oldmf->v1= mv->index;
				else oldmf->v1= 0;

				if(j>1) {
					mv= (NMVert *) PyList_GetItem(mf->v, 1);
					if (mv && NMVert_Check(mv) && mv->index!=-1) oldmf->v2= mv->index;
					else oldmf->v2= 0;
			
					if(j>2) {
						mv= (NMVert *) PyList_GetItem(mf->v, 2);
						if (mv && NMVert_Check(mv) && mv->index!=-1) oldmf->v3= mv->index;
						else oldmf->v3= 0;
				
						if(j>3) {
							mv= (NMVert *) PyList_GetItem(mf->v, 3);
							if (mv && NMVert_Check(mv) && mv->index!=-1) oldmf->v4= mv->index;
							else oldmf->v4= 0;
						}
					}
				}
			}
			test_index_mface(oldmf, j);
					
			oldmf->puno= 0;
			oldmf->mat_nr= mf->mat_nr;
			oldmf->edcode= 0;
			oldmf->flag= 0;
	
			if (me->flags&NMESH_HASMCOL) {
				len= PyList_Size(mf->col);
				
				if(len) {
					mc= (NMCol *) PyList_GetItem(mf->col, 0);
					if(!NMCol_Check(mc)) {
						newmc+=4;
						continue;
					}
					
					for(j=0; j<4; j++, newmc++) {
						if (j<len) mc= (NMCol *) PyList_GetItem(mf->col, j);
						if(!NMCol_Check(mc)) continue;
					
						newmc->b= mc->r;
						newmc->g= mc->g;
						newmc->r= mc->b;
						newmc->a= mc->a;
					}
				}
			}
		}
	} else {
		nmesh->totface= 0;
	}
	
	if(recalc_normals) vertexnormals_mesh(nmesh, 0);
	edge_drawflags_mesh(nmesh);
	tex_space_mesh(nmesh);
	
	if (!during_script())
		allqueue(REDRAWVIEW3D, 0);
	
	if (ob) return add_pyblock(ID_OB, ob);
	else return py_incr_ret(Py_None);
}

static struct PyMethodDef NMeshM_methods[] = {
	MethodDef(Col),
	MethodDef(Vert),
	MethodDef(Face),
	MethodDef(GetRaw),
	MethodDef(PutRaw),
	{NULL, NULL}
};

PyObject *init_py_nmesh(void) 
{
	NMesh_Type.ob_type = &PyType_Type;	
	NMVert_Type.ob_type = &PyType_Type;	
	NMFace_Type.ob_type = &PyType_Type;	
	NMCol_Type.ob_type = &PyType_Type;	
	
	return Py_InitModule("Blender.NMesh", NMeshM_methods);
}

#endif /* !(WITHOUT_PYTHON) */

