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

#include "Python.h"
#include "py_blender.h"
#include "blender.h"
#include "screen.h"
#include "ipo.h"

static PyObject *mdict, *gdict;

static PyObject *init_blender(void);

void copy_scriptlink(ScriptLink *scriptlink)
{
	void *tmp;
	
	if (scriptlink->totscript) {
		tmp= scriptlink->scripts;
		scriptlink->scripts= mallocN(sizeof(ID*)*scriptlink->totscript, "scriptlistL");
		memcpy(scriptlink->scripts, tmp, sizeof(ID*)*scriptlink->totscript);

		tmp= scriptlink->flag;
		scriptlink->flag= mallocN(sizeof(short)*scriptlink->totscript, "scriptlistF");
		memcpy(scriptlink->flag, tmp, sizeof(short)*scriptlink->totscript);
	}
}

void free_scriptlink(ScriptLink *slink)
{
	if (slink->totscript) {
		freeN(slink->flag);
		freeN(slink->scripts);
	}
}

void clear_bad_scriptlink(ID *id, Text *byebye)
{
	ScriptLink *scriptlink;
	int offset=-1;
	char *structname=NULL;
	int i;

	if (GS(id->name)==ID_OB) structname= "Object";
	else if (GS(id->name)==ID_LA) structname= "Lamp";
	else if (GS(id->name)==ID_CA) structname= "Camera";
	else if (GS(id->name)==ID_MA) structname= "Material";
	else if (GS(id->name)==ID_WO) structname= "World";
	else if (GS(id->name)==ID_SCE) structname= "Scene";
	
	if (!structname) return;
	
	offset= findstruct_offset(structname, "scriptlink");
	
	if (offset<0) return;
	
	scriptlink= (ScriptLink *) (((char *)id) + offset);

	for(i=0; i<scriptlink->totscript; i++)
		if ((Text*)scriptlink->scripts[i] == byebye)
			scriptlink->scripts[i] = NULL;
}

void clear_bad_scriptlist(ListBase *list, Text *byebye)
{
	ID *id;

	id= list->first;
	while (id) {
		clear_bad_scriptlink(id, byebye);
		
		id= id->next;
	}
}

void clear_bad_scriptlinks(Text *byebye)
{
	clear_bad_scriptlist(&G.main->object,	byebye);
	clear_bad_scriptlist(&G.main->lamp,		byebye);
	clear_bad_scriptlist(&G.main->camera,	byebye);
	clear_bad_scriptlist(&G.main->mat,		byebye);
	clear_bad_scriptlist(&G.main->world,	byebye);
	
	clear_bad_scriptlink(&G.scene->id, byebye);
	
	allqueue(REDRAWBUTSSCRIPT, 0);	
}


#ifndef WITHOUT_PYTHON
 
void start_python (void) 
{
	static int started=0;
	PyObject *mod;
	extern int Py_FrozenFlag;
	extern int Py_NoSiteFlag;
	extern int Py_UseClassExceptionsFlag;
	extern int Py_VerboseFlag;
	extern int Py_DebugFlag;
	
	if (TEST_C_KEY==0) return;

	if (started) return;
	started= 1;
	
	/* This gave too annoying warnings and
	 * no one seemed to use it.	*/
	if (0 && G.f & G_DEBUG) {
		Py_VerboseFlag= 1;
		Py_DebugFlag= 1;		
	} else {
		Py_FrozenFlag= 1;
		Py_NoSiteFlag= 1;
		Py_UseClassExceptionsFlag= 0;
	}
		
	Py_SetProgramName("blender");
	Py_Initialize();
	
	mdict= init_blender();
	
	gdict= PyDict_New();
	PyDict_SetItemString(gdict, "__builtins__", PyEval_GetBuiltins());

	return;
}

void end_python(void) {
	int i, len;
	PyObject *dictlist;

	if (TEST_C_KEY==0) return;

	dictlist= PyDict_Items(gdict);
	len= PyList_Size(dictlist);
	
	for (i=0; i<len; i++) {
		PyObject *ob= PyList_GetItem(dictlist, i);
		PyDict_DelItem(gdict, PyTuple_GetItem(ob, 0));
	}
	Py_DECREF(dictlist);
	
	Py_Finalize();
}

static int compile_script (Text *text)
{
	int ret=1;
	char *buf;

	if (TEST_C_KEY==0) return 0;
	
	if (!text->compiled) {
		buf= txt_to_buf(text);
		strcat(buf, "\n");
		
		text->compiled= Py_CompileString(buf, text->id.name, Py_file_input);
		
		if (PyErr_Occurred()) {
			PyErr_Print();
			text->compiled= NULL;
			
			ret= 0;
		}

		freeN(buf);
	}
	
	return ret;
}

void txt_do_python (Text *text)
{
	if (TEST_C_KEY==0) return;

	if (compile_script(text)) {
		Py_INCREF(Py_False);
		PyDict_SetItemString(mdict, "bylink", Py_False);

		PyEval_EvalCode(text->compiled, gdict, NULL);
		
		if(PyErr_Occurred()) {
			PyErr_Print();
			text->compiled= NULL;
		}
	}
}

extern PyTypeObject Object_Type;
extern PyTypeObject Lamp_Type;
extern PyTypeObject Camera_Type;
extern PyTypeObject Material_Type;
extern PyTypeObject World_Type;
/* extern PyTypeObject Mesh_Type; */

PyBlock *new_pyblock(int id) {
	PyBlock *new= NULL;
	
	if (id==ID_OB) new= PyObject_NEW(PyBlock, &Object_Type);
	else if (id==ID_LA) new= PyObject_NEW(PyBlock, &Lamp_Type);
	else if (id==ID_CA) new= PyObject_NEW(PyBlock, &Camera_Type);
	else if (id==ID_MA) new= PyObject_NEW(PyBlock, &Material_Type);
	else if (id==ID_WO) new= PyObject_NEW(PyBlock, &World_Type);
/* 	else if (id==ID_ME) new= PyObject_NEW(PyBlock, &Mesh_Type); */
	else return NULL;
	
	if (new) new->data= NULL;
	
	return new;
}

PyObject *add_pyblock(int id, void *data) {
	PyBlock *new= NULL;
	
	if (id==ID_OB) new= PyObject_NEW(PyBlock, &Object_Type);
	else if (id==ID_LA) new= PyObject_NEW(PyBlock, &Lamp_Type);
	else if (id==ID_CA) new= PyObject_NEW(PyBlock, &Camera_Type);
	else if (id==ID_MA) new= PyObject_NEW(PyBlock, &Material_Type);
	else if (id==ID_WO) new= PyObject_NEW(PyBlock, &World_Type);
/* 	else if (id==ID_ME) new= PyObject_NEW(PyBlock, &Mesh_Type); */
	else return NULL;
	
	if (new) new->data= data;
	
	return (PyObject *) new;
}

PyObject *py_find_from_list(ID *list, int idcode, PyObject *args) {
	char *name= NULL;
	
	Py_Try(PyArg_ParseTuple(args, "|s", &name));
	
	if (name) {
		while (list) {
			if (strcmp(name, list->name+2)==0) 
				return add_pyblock(idcode, list);

			list= list->next;
		}
		return py_incr_ret(Py_None);
		
	} else {
		PyObject *pylist= PyList_New(countlist((ListBase*)&list));
		int i=0;
	
		while (list) {
			PyList_SetItem(pylist, i, add_pyblock(idcode, list));	
		
			list= list->next; i++;
		}
		return pylist;
	}
}
	
void do_pyscript(ID *id, short event) 
{
	ScriptLink *scriptlink;
	int offset;
	PyObject *arg;
	char *structname=NULL;
	int i;

	if (TEST_C_KEY==0) return;

	PyDict_SetItemString(gdict, "bylink", py_incr_ret(Py_True));
	
	if (GS(id->name)==ID_OB) structname= "Object";
	else if (GS(id->name)==ID_LA) structname= "Lamp";
	else if (GS(id->name)==ID_CA) structname= "Camera";
	else if (GS(id->name)==ID_MA) structname= "Material";
	else if (GS(id->name)==ID_WO) structname= "World";
	else if (GS(id->name)==ID_SCE) structname= "Scene";
	else return;
	
	if (!structname) return;
	
	offset= findstruct_offset(structname, "scriptlink");
	
	if (offset<0) return;
	
	scriptlink= (ScriptLink *) (((char *)id) + offset);

	disable_where_script(1);
		
	if (scriptlink->totscript) {
		if (GS(id->name)==ID_SCE) {
			Py_INCREF(Py_None);
			arg= Py_None;
			
		} else arg= add_pyblock(GS(id->name), id);

		for(i=0; i<scriptlink->totscript; i++)	{
			if (scriptlink->flag[i]==event && scriptlink->scripts[i]) {
				PyDict_SetItemString(mdict, "link", arg);
				Py_INCREF(Py_True);
				PyDict_SetItemString(mdict, "bylink", Py_True);
					
				compile_script((Text*) scriptlink->scripts[i]);
				
				PyEval_EvalCode(((Text*) scriptlink->scripts[i])->compiled, gdict, NULL);

				if(PyErr_Occurred()) {
					PyErr_Print();
					((Text*) scriptlink->scripts[i])->compiled= NULL;
				}				
			}
		}
	
		Py_DECREF(arg);
	}

	disable_where_script(0);	
}

void do_all_scriptlist(ListBase *list, short event)
{
	ID *id;
	
	if (TEST_C_KEY==0) return;

	id= list->first;
	while (id) {
		do_pyscript (id, event);
		
		id= id->next;
	}
	
}

void do_all_scripts(short event) 
{
	if (TEST_C_KEY==0) return;

 	do_all_scriptlist(&G.main->object, event);
	do_all_scriptlist(&G.main->lamp, event);
	do_all_scriptlist(&G.main->camera, event);
	do_all_scriptlist(&G.main->mat, event);
	do_all_scriptlist(&G.main->world, event);
}

/*****************************/
/*  Main interface routines  */
/*****************************/

PyObject *py_incr_ret(PyObject *ob) {
	Py_INCREF(ob);
	
	return ob;
}

static PyObject *Method_Redraw(PyObject *self, PyObject *args) 
{
	ScrArea *tempsa, *sa;
	
	Py_Try(PyArg_ParseTuple(args, ""));
	
	if (during_script()) return py_incr_ret(Py_None);

	tempsa= curarea;
	sa= G.curscreen->areabase.first;
	while(sa) {
		if (sa->spacetype== SPACE_VIEW3D) {
			areawinset(sa->win);
			sa->windraw();
		}

		sa= sa->next;
	}
	if(curarea!=tempsa) areawinset(tempsa->win);

	screen_swapbuffers();

	return py_incr_ret(Py_None);
}

#define BP_CURFRAME 1
#define BP_CURTIME  2

#ifndef FREE

static PyObject *Method_Set (PyObject *self, PyObject *args)
{
	int item, val, err=0;
	
	Py_Try(PyArg_ParseTuple(args, "ii", &item, &val));

	switch (item) {
	case BP_CURFRAME:
		CFRA= val;
		do_global_buttons(B_NEWFRAME);
		break;
		
	default:
		err= 1;
		break;
	}

	if (err) {
		PyErr_SetString(PyExc_AttributeError, "bad request identifier");
		return NULL;
	} else {
		Py_INCREF(Py_None);
		return Py_None;		
	}
}
#endif

static char Method_Get_doc[]=
"([request]) - Retrieve settings from Blender\n\
[request] Identifies the desired settings. Availble settings...\n\n\
BP_CURFRAME - Returns the current animation frame as an Int\n\
BP_CURTIME - Returns the current animation time as a Float\n\n\
Constants should be imported from Blender.Const";

static PyObject *Method_Get (PyObject *self, PyObject *args)
{
	int item;
	
	Py_Try(PyArg_ParseTuple(args, "i", &item));

	switch (item) {
	case BP_CURFRAME:
		return PyInt_FromLong(CFRA);

	case BP_CURTIME:
		return PyFloat_FromDouble(frame_to_float(CFRA));

	default:
		break;
	}

	PyErr_SetString(PyExc_AttributeError, "bad request identifier");
	return NULL;
}

static struct PyMethodDef Blender_methods[] = {
	{"Redraw", Method_Redraw, 1},
#ifndef FREE
	{"Set", Method_Set, 1}, 
#endif
	{"Get", Method_Get, 1, Method_Get_doc}, 
	{NULL, NULL}
};

static struct PyMethodDef Null_methods[] = {
	{NULL, NULL}
};

int badzr252= 0;

static PyObject *init_blender(void)
{
	PyObject *mod, *dict, *cmod, *cdict;
	
	mod= Py_InitModule("Blender", Blender_methods);

	dict= PyModule_GetDict(mod);

		/* Blender object modules */
	PyDict_SetItemString(dict, "Camera",	init_py_camera());	
	PyDict_SetItemString(dict, "Lamp",		init_py_lamp());
	PyDict_SetItemString(dict, "Material",	init_py_material());
	PyDict_SetItemString(dict, "Object",	init_py_object());
	PyDict_SetItemString(dict, "World",	init_py_world());

	PyDict_SetItemString(dict, "NMesh",		init_py_nmesh());

	PyDict_SetItemString(dict, "Draw",		init_py_draw());
#ifndef IRISGL
	PyDict_SetItemString(dict, "BGL",		init_py_bgl());
#endif

	if (badzr252) {
		PyDict_SetItemString(dict, "Demo",		init_py_demo());

		PyDict_SetItemString(dict, "Interface", init_py_interface());
	}

	cmod= Py_InitModule("Blender.Const", Null_methods);
	PyDict_SetItemString(dict, "Const", cmod);
	
	cdict= PyModule_GetDict(cmod);
	
	Py_AddConsti(cdict, BP_CURFRAME);
	Py_AddConsti(cdict, BP_CURTIME);

	return dict;
}

#else /* WITHOUT_PYTHON */

void do_pyscript(void) {}

void start_python(void) {}

void end_python(void) {}

void do_all_scripts(short event) {}

void txt_do_python(Text *text) {
	static int error_no_python=0;
	
	if (!error_no_python) {
		printf ("ERROR: Python is not available in this binary!\n");
		error_no_python=1;
	}
}

#endif /* !(WITHOUT_PYTHON) */

