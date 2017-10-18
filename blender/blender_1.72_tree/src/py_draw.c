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
#include "graphics.h"
#include "screen.h"

/* Button Object */

PyTypeObject Button_Type;

typedef struct _Button {
	PyObject_VAR_HEAD

	int type; /* 1 == int, 2 == float, 3 == string */
	int slen; /* length of string (if type == 3) */
	union {
		int asint;
		float asfloat;
		char *asstr;
	} val;
} Button;

static void Button_dealloc(PyObject *self) {
	Button *but= (Button*) self;

	if(but->type==3) freeN(but->val.asstr);
		
	PyMem_DEL(self);	
}

static PyObject *Button_getattr(PyObject *self, char *name) {
	Button *but= (Button*) self;
	
	if(STREQ(name, "val")) {
		if (but->type==1)
			return Py_BuildValue("i", but->val.asint);			
		else if (but->type==2) 
			return Py_BuildValue("f", but->val.asfloat);
		else if (but->type==3) 
			return Py_BuildValue("s", but->val.asstr);
	}
	PyErr_SetString(PyExc_AttributeError, name);
	return NULL;
}

static int Button_setattr(PyObject *self,  char *name, PyObject *v) {
	Button *but= (Button*) self;
	
	if(STREQ(name, "val")) {
		if  (but->type==1)
			PyArg_Parse(v, "i", &but->val.asint);
		else if (but->type==2)
			PyArg_Parse(v, "f", &but->val.asfloat);			
		else if (but->type==3) {
			char *newstr;
			
			PyArg_Parse(v, "s", &newstr);
			strncpy(but->val.asstr, newstr, but->slen); 
		}
	} else {
		PyErr_SetString(PyExc_AttributeError, name);
		return -1;
	}
	
	return 0;
}

static Button *newbutton (void) {
	Button *but= (Button *) PyObject_NEW(Button, &Button_Type);
	
	return but;	
}

PyTypeObject Button_Type = {
	PyObject_HEAD_INIT(NULL)
	0,								/*ob_size*/
	"Button",						/*tp_name*/
	sizeof(Button),				/*tp_basicsize*/
	0,								/*tp_itemsize*/
	(destructor) Button_dealloc,	/*tp_dealloc*/
	(printfunc)  0,					/*tp_print*/
	(getattrfunc) Button_getattr,	/*tp_getattr*/
	(setattrfunc) Button_setattr,	/*tp_setattr*/
};

/* GUI interface routinges */

extern void drawtextspace();
extern void winqreadtextspace(ushort, short);

void exit_pydraw(void) {
	extern void defheaddraw(), defheadqread();
	SpaceText *st= curarea->spacedata.first;

	Py_XDECREF((PyObject *) st->py_draw);
	Py_XDECREF((PyObject *) st->py_event);
	Py_XDECREF((PyObject *) st->py_button);
	
	curarea->windraw= (void(*)()) drawtextspace;
	curarea->winqread= (void(*)()) winqreadtextspace;

	curarea->headdraw= (void(*)()) defheaddraw;
	curarea->headqread= NULL;
	
	st->py_draw= NULL;
	st->py_event= NULL;
	st->py_button= NULL;
	st->py_head_draw= NULL;
	st->py_head_event= NULL;
	st->py_head_button= NULL;

	addqueue(curarea->win, REDRAW, 1);
	addqueue(curarea->headwin, REDRAW, 1);
}


void do_py_draw(void) {
	SpaceText *st= curarea->spacedata.first;
	PyObject *py_draw_callback= st->py_draw;
	PyObject *args, *result;
	char butblock[16];
	
	if (!py_draw_callback) {
		cpack(0x585858);
		glClear(GL_COLOR_BUFFER_BIT);
		
		return;
	}

	sprintf(butblock, "%p", st);	
	DefButBlock(butblock, curarea->win, G.font, 4, 2, 2); 

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	args= Py_BuildValue("()");
	result= PyEval_CallObject(py_draw_callback, args);
	
	glPopAttrib();
	
	if (result==NULL) {
		st->text->compiled= NULL;
		PyErr_Print();
		exit_pydraw();
		allqueue(REDRAWTEXT, 0);
	}

	Py_DECREF(args);
	
	curarea->win_swap= WIN_BACK_OK;

	if (!py_draw_callback) exit_pydraw();
}

void do_py_buttons(ushort event) {
	SpaceText *st= curarea->spacedata.first;
	PyObject *callback= st->py_button;
	PyObject *args, *result;
	
	if (!callback) return;
	
	args= Py_BuildValue("(i)", event);
	result= PyEval_CallObject(callback, args);
	if (result==NULL) {
		PyErr_Print();
		exit_pydraw();
		allqueue(REDRAWTEXT, 0);
	}

	Py_DECREF(args);

	if (!callback) exit_pydraw();
}


void do_py_event(ushort event, short val) {
	SpaceText *st= curarea->spacedata.first;
	PyObject *callback= st->py_event;
	PyObject *args, *result;
	short butevent;
	static int mdown= 0;
	
	if (!callback) return;

	if (event==QKEY && G.qual & (LR_ALTKEY|LR_CTRLKEY|LR_SHIFTKEY)) {
		exit_pydraw();
		return;
	}

	if (event==LEFTMOUSE && val) {
		FrontbufferButs(TRUE);
		butevent= DoButtons();
		FrontbufferButs(FALSE);

		if(butevent) do_py_buttons(butevent);
	}
	
	args= Py_BuildValue("(ii)", event, val);
	result= PyEval_CallObject(callback, args);
	if (result==NULL) {
		PyErr_Print();
		exit_pydraw();
		allqueue(REDRAWTEXT, 0);
	}
	
	Py_DECREF(args);

	if (!callback) exit_pydraw();
}

void do_py_head_draw(void) {
	SpaceText *st= curarea->spacedata.first;
	PyObject *callback= st->py_head_draw;
	PyObject *args, *result;
	char butblock[16];
	int width= curarea->winx+100;
	int selcol= 0;

	printf ("Drawing header\n");

	if(G.curscreen->winakt) {
		if(curarea->headwin == G.curscreen->winakt) selcol= 1;
		else if(curarea->win == G.curscreen->winakt) selcol= 1;
	}

	headerbox(selcol, curarea->winx+100);	

	sprintf(butblock, "header %d", curarea->headwin);
	DefButBlock(butblock, curarea->headwin, G.font, 35, 2, 2);

	DefButt(ICONROW|CHA,B_NEWSPACE, "ICON 0 0 0", 6,0,20,20, &(curarea->butspacetype), 1.0, 10.0, 0, 0, "Current window type (LeftMouse+drag to change)");

	if(curarea->full) DefButt(BUT,B_FULL, "ICON 0 1 8",	40,0,20,20, 0, 0, 0, 0, 0, "Restore smaller windows (CTRL+Up arrow)");
	else DefButt(BUT,B_FULL, "ICON 0 0 8",	40,0,20,20, 0, 0, 0, 0, 0, "Make fullscreen window (CTRL+Down arrow)");

	if (!callback) return;

	args= Py_BuildValue("()");
	result= PyEval_CallObject(callback, args);
	if (result==NULL) {
		st->text->compiled= NULL;
		PyErr_Print();
		exit_pydraw();
		allqueue(REDRAWTEXT, 0);
	}

	Py_DECREF(args);
	
	curarea->head_swap= WIN_BACK_OK;

	if (!callback) exit_pydraw();
}

void do_py_head_buttons(ushort event) {
	SpaceText *st= curarea->spacedata.first;
	PyObject *callback= st->py_head_button;
	PyObject *args, *result;

	if (!callback) return;
	
	args= Py_BuildValue("(i)", event);
	result= PyEval_CallObject(callback, args);
	if (result==NULL) {
		PyErr_Print();
		exit_pydraw();
		allqueue(REDRAWTEXT, 0);
	}

	Py_DECREF(args);

	if (!callback) exit_pydraw();
}


void do_py_head_event(ushort event, short val) {
	SpaceText *st= curarea->spacedata.first;
	PyObject *callback= st->py_head_event;
	PyObject *args, *result;
	short butevent;
	static int mdown= 0;
	
	if (!callback) return;

	if (event==QKEY && G.qual & (LR_ALTKEY|LR_CTRLKEY|LR_SHIFTKEY)) {
		exit_pydraw();
		return;
	}

	if (event==LEFTMOUSE && val) {
		FrontbufferButs(TRUE);
		butevent= DoButtons();
		FrontbufferButs(FALSE);

		if(butevent) do_py_head_buttons(butevent);
	}
	
	args= Py_BuildValue("(ii)", event, val);
	result= PyEval_CallObject(callback, args);
	if (result==NULL) {
		PyErr_Print();
		exit_pydraw();
		allqueue(REDRAWTEXT, 0);
	}
	
	Py_DECREF(args);

	if (!callback) exit_pydraw();
}

int py_is_regged(SpaceText *st) {
	if (st->py_draw || st->py_event || st->py_button)
		return 1;
	
	return 0;
}

static char Method_Exit_doc[]= 
"() - Exit the windowing interface";
static PyObject *Method_Exit (PyObject *self, PyObject *args)
{	
	SpaceText *st= curarea->spacedata.first;
	Py_Try(PyArg_ParseTuple(args, ""));
	
	exit_pydraw();
	
	return py_incr_ret(Py_None);
}

static char Method_Register_doc[]= 
"(draw, event, button) - Register callbacks for windowing\n\
	(draw) A function to draw the screen, taking no arguments\n\
	(event) A function to handle events, taking 2 arguments (evt, val)\n\
		(evt) The event number\n\
		(val) The value modifier (for key and mouse press/release)\n\
	(button) A function to handle buttons, taking 1 argument (evt)\n\
		(evt) The button number";
static PyObject *Method_Register (PyObject *self, PyObject *args)
{
	PyObject *newdrawc= NULL, *neweventc= NULL, *newbuttonc= NULL;
	PyObject *newheaddrawc= NULL, *newheadeventc= NULL, *newheadbuttonc= NULL;
	SpaceText *st= curarea->spacedata.first;
	
	Py_Try(PyArg_ParseTuple(args, "|OOOOOO", &newdrawc, &neweventc, &newbuttonc, 
											&newheaddrawc, &newheadeventc, &newheadbuttonc));

	Py_XINCREF(newdrawc);
	Py_XINCREF(neweventc);
	Py_XINCREF(newbuttonc);
/* 	Py_XINCREF(newheaddrawc); */
/* 	Py_XINCREF(newheadeventc); */
/* 	Py_XINCREF(newheadbuttonc); */
	
	Py_XDECREF((PyObject *) st->py_draw);
	Py_XDECREF((PyObject *) st->py_event);
	Py_XDECREF((PyObject *) st->py_button);
/* 	Py_XDECREF((PyObject *) st->py_head_draw); */
/* 	Py_XDECREF((PyObject *) st->py_head_event); */
/* 	Py_XDECREF((PyObject *) st->py_head_button); */

	if (newdrawc) curarea->windraw= (void(*)()) do_py_draw;
	if (neweventc) curarea->winqread= (void(*)()) do_py_event;

/* 	if (newheaddrawc) curarea->headdraw= (void(*)()) do_py_head_draw; */
/* 	if (newheadeventc) curarea->headqread= (void(*)()) do_py_head_event; */
	
	st->py_draw= newdrawc;
	st->py_event= neweventc;
	st->py_button= newbuttonc;
/* 	st->py_head_draw= newheaddrawc; */
/* 	st->py_head_event= newheadeventc; */
/* 	st->py_head_button= newheadbuttonc; */

	addqueue(curarea->win, REDRAW, 1);
/* 	addqueue(curarea->headwin, REDRAW, 1); */
	
	return py_incr_ret(Py_None);
}

static char Method_Create_doc[]= "";
static PyObject *Method_Create (PyObject *self,  PyObject *args)
{
	Button *but;
	PyObject *in;

	Py_Try(PyArg_ParseTuple(args, "O", &in));
	
	but= newbutton();
	if(PyFloat_Check(in)) {
		but->type= 2;
		but->val.asfloat= PyFloat_AsDouble(in);
	} else if (PyInt_Check(in)) {		
		but->type= 1;
		but->val.asint= PyInt_AsLong(in);
	} else if (PyString_Check(in)) {
		char *newstr= PyString_AsString(in);
		
		but->type= 3;
		but->slen= strlen(newstr);
		but->val.asstr= mallocN(but->slen+1, "button string");
		
		strcpy(but->val.asstr, newstr);
	}
		
	return (PyObject *) but;
}

static char Method_Button_doc[]= "";
static PyObject *Method_Button (PyObject *self,  PyObject *args)
{
	char *name, *tip= NULL;
	int event, type;
	int x, y, w, h;
	
	Py_Try(PyArg_ParseTuple(args, "siiiii|s", &name, &event, &x, &y, &w, &h, &tip));
	
	DefButt(BUT, event, name, x, y, w, h, 0, 0, 0, 0, 0, tip);
	
	return py_incr_ret(Py_None);
}

static char Method_Menu_doc[]= "";
static PyObject *Method_Menu (PyObject *self,  PyObject *args)
{
	char *name, *tip= NULL;
	int event, type, def;
	int x, y, w, h;
	Button *but;
	
	Py_Try(PyArg_ParseTuple(args, "siiiiii|s", &name, &event, &x, &y, &w, &h, &def, &tip));
	
	but= newbutton();
	but->type= 1;
	but->val.asint= def;
	
	DefButt(MENU|INT, event, name, x, y, w, h, &but->val.asint, 0, 0, 0, 0, tip);
	
	return (PyObject *) but;
}

static char Method_Toggle_doc[]= "";
static PyObject *Method_Toggle (PyObject *self,  PyObject *args)
{
	char *name, *tip= NULL;
	int event, type;
	int x, y, w, h;
	Button *but;
	
	Py_Try(PyArg_ParseTuple(args, "siiiii|s", &name, &event, &x, &y, &w, &h, &tip));
	
	but= newbutton();
	but->type= 1;
	but->val.asint= 0;
	
	DefButt(TOG|INT, event, name, x, y, w, h, &but->val.asint, 0, 0, 0, 0, tip);
	
	return (PyObject *) but;
}

static void py_slider_update(struct But *but) {
	SpaceText *st= curarea->spacedata.first;

	curarea->win_swap= WIN_FRONT_OK;
	
	do_py_buttons(but->nr);
}

static char Method_Slider_doc[]= "";
static PyObject *Method_Slider (PyObject *self,  PyObject *args)
{
	char *name, *tip;
	int event, type;
	int x, y, w, h;
	Button *but;
	PyObject *mino, *maxo, *inio;
	
	Py_Try(PyArg_ParseTuple(args, "siiiiiOOO|s", &name, &event, &x, &y, &w, &h, &inio, &mino, &maxo, &tip));
	
	but= newbutton();
	
	if (PyFloat_Check(inio)) {
		float ini, min, max;

		ini= PyFloat_AsDouble(inio);
		min= PyFloat_AsDouble(mino);
		max= PyFloat_AsDouble(maxo);
				
		but->type= 2;
		but->val.asfloat= ini;

		SetButFunc(py_slider_update);	
		DefButt(NUMSLI|FLO, event, name, x, y, w, h, &but->val.asfloat, min, max, 0, 0, tip);
	} else {
		int ini, min, max;

		ini= PyInt_AsLong(inio);
		min= PyInt_AsLong(mino);
		max= PyInt_AsLong(maxo);
		
		but->type= 1;
		but->val.asint= ini;
	
		DefButt(NUMSLI|INT, event, name, x, y, w, h, &but->val.asint, min, max, 0, 0, tip);
	}
	
	return (PyObject *) but;
}

static char Method_Number_doc[]= "";
static PyObject *Method_Number (PyObject *self,  PyObject *args)
{
	char *name, *tip;
	int event, type;
	int x, y, w, h;
	Button *but;
	PyObject *mino, *maxo, *inio;
	
	Py_Try(PyArg_ParseTuple(args, "siiiiiOOO|s", &name, &event, &x, &y, &w, &h, &inio, &mino, &maxo, &tip));
	
	but= newbutton();
	
	if (PyFloat_Check(inio)) {
		float ini, min, max;

		ini= PyFloat_AsDouble(inio);
		min= PyFloat_AsDouble(mino);
		max= PyFloat_AsDouble(maxo);
				
		but->type= 2;
		but->val.asfloat= ini;
	
		DefButt(NUM|FLO, event, name, x, y, w, h, &but->val.asfloat, min, max, 0, 0, tip);
	} else {
		int ini, min, max;

		ini= PyInt_AsLong(inio);
		min= PyInt_AsLong(mino);
		max= PyInt_AsLong(maxo);
		
		but->type= 1;
		but->val.asint= ini;
	
		DefButt(NUM|INT, event, name, x, y, w, h, &but->val.asint, min, max, 0, 0, tip);
	}
	
	return (PyObject *) but;
}

static char Method_String_doc[]= "";
static PyObject *Method_String (PyObject *self,  PyObject *args)
{
	char *name, *tip, *newstr;
	int event, type;
	int x, y, w, h, len;
	Button *but;
	
	Py_Try(PyArg_ParseTuple(args, "siiiiisi|s", &name, &event, &x, &y, &w, &h, &newstr, &len, &tip));
	
	but= newbutton();
	but->type= 3;
	but->slen= len;
	but->val.asstr= mallocN(len+1, "button string");
	
	strncpy(but->val.asstr, newstr, len);
	but->val.asstr[len]= 0;
	
	DefButt(TEX, event, name, x, y, w, h, but->val.asstr, 0, len, 0, 0, tip);

	return (PyObject *) but;
}

static char Method_Redraw_doc[]= 
"() - Queue a redraw event";
static PyObject *Method_Redraw (PyObject *self,  PyObject *args)
{
	Py_Try(PyArg_ParseTuple(args, ""));

	addqueue(curarea->win, REDRAW, 1);
	
	return py_incr_ret(Py_None);
}


static char Method_Text_doc[]= 
"(text) - Draw text onscreen\n\
	(text) The text to draw\n";
static PyObject *Method_Text (PyObject *self, PyObject *args)
{
	char *text;
	
	Py_Try(PyArg_ParseTuple(args, "s", &text));
	
	fmprstr(text);
	
	return py_incr_ret(Py_None);
}

static struct PyMethodDef DrawM_methods[] = {
	MethodDef(Create),
	
	MethodDef(Button),
	MethodDef(Toggle),
	MethodDef(Menu),
	MethodDef(Slider),
	MethodDef(Number),
	MethodDef(String),

	MethodDef(Text),

	MethodDef(Exit),
	MethodDef(Redraw),
	MethodDef(Register),
	{NULL, NULL}
};

PyObject *init_py_draw(void) 
{
	PyObject *mod= Py_InitModule("Blender.Draw", DrawM_methods);
	PyObject *dict= PyModule_GetDict(mod);

	Button_Type.ob_type= &PyType_Type;

	Py_AddConsti(dict, LEFTMOUSE);
	Py_AddConsti(dict, MIDDLEMOUSE);
	Py_AddConsti(dict, RIGHTMOUSE);
	Py_AddConsti(dict, MOUSEX);
	Py_AddConsti(dict, MOUSEY);
	Py_AddConsti(dict, TIMER0);
	Py_AddConsti(dict, TIMER1);
	Py_AddConsti(dict, TIMER2);
	Py_AddConsti(dict, TIMER3);
	Py_AddConsti(dict, KEYBD);
	Py_AddConsti(dict, RAWKEYBD);
	Py_AddConsti(dict, REDRAW);
	Py_AddConsti(dict, INPUTCHANGE);
	Py_AddConsti(dict, QFULL);
	Py_AddConsti(dict, WINFREEZE);
	Py_AddConsti(dict, WINTHAW);
	Py_AddConsti(dict, WINCLOSE);
	Py_AddConsti(dict, WINQUIT);
#ifndef IRISGL
	Py_AddConsti(dict, Q_FIRSTTIME);
#endif
	Py_AddConsti(dict, AKEY);
	Py_AddConsti(dict, BKEY);
	Py_AddConsti(dict, CKEY);
	Py_AddConsti(dict, DKEY);
	Py_AddConsti(dict, EKEY);
	Py_AddConsti(dict, FKEY);
	Py_AddConsti(dict, GKEY);
	Py_AddConsti(dict, HKEY);
	Py_AddConsti(dict, IKEY);
	Py_AddConsti(dict, JKEY);
	Py_AddConsti(dict, KKEY);
	Py_AddConsti(dict, LKEY);
	Py_AddConsti(dict, MKEY);
	Py_AddConsti(dict, NKEY);
	Py_AddConsti(dict, OKEY);
	Py_AddConsti(dict, PKEY);
	Py_AddConsti(dict, QKEY);
	Py_AddConsti(dict, RKEY);
	Py_AddConsti(dict, SKEY);
	Py_AddConsti(dict, TKEY);
	Py_AddConsti(dict, UKEY);
	Py_AddConsti(dict, VKEY);
	Py_AddConsti(dict, WKEY);
	Py_AddConsti(dict, XKEY);
	Py_AddConsti(dict, YKEY);
	Py_AddConsti(dict, ZKEY);
	Py_AddConsti(dict, ZEROKEY);
	Py_AddConsti(dict, ONEKEY);
	Py_AddConsti(dict, TWOKEY);
	Py_AddConsti(dict, THREEKEY);
	Py_AddConsti(dict, FOURKEY);
	Py_AddConsti(dict, FIVEKEY);
	Py_AddConsti(dict, SIXKEY);
	Py_AddConsti(dict, SEVENKEY);
	Py_AddConsti(dict, EIGHTKEY);
	Py_AddConsti(dict, NINEKEY);
	Py_AddConsti(dict, CAPSLOCKKEY);
	Py_AddConsti(dict, LEFTCTRLKEY);
	Py_AddConsti(dict, LEFTALTKEY);
	Py_AddConsti(dict, RIGHTALTKEY);
	Py_AddConsti(dict, RIGHTCTRLKEY);
	Py_AddConsti(dict, RIGHTSHIFTKEY);
	Py_AddConsti(dict, LEFTSHIFTKEY);
	Py_AddConsti(dict, ESCKEY);
	Py_AddConsti(dict, TABKEY);
	Py_AddConsti(dict, RETKEY);
	Py_AddConsti(dict, SPACEKEY);
	Py_AddConsti(dict, LINEFEEDKEY);
	Py_AddConsti(dict, BACKSPACEKEY);
	Py_AddConsti(dict, DELKEY);
	Py_AddConsti(dict, SEMICOLONKEY);
	Py_AddConsti(dict, PERIODKEY);
	Py_AddConsti(dict, COMMAKEY);
	Py_AddConsti(dict, QUOTEKEY);
	Py_AddConsti(dict, ACCENTGRAVEKEY);
	Py_AddConsti(dict, MINUSKEY);
	Py_AddConsti(dict, VIRGULEKEY);
	Py_AddConsti(dict, SLASHKEY);
	Py_AddConsti(dict, BACKSLASHKEY);
	Py_AddConsti(dict, EQUALKEY);
	Py_AddConsti(dict, LEFTBRACKETKEY);
	Py_AddConsti(dict, RIGHTBRACKETKEY);
	Py_AddConsti(dict, LEFTARROWKEY);
	Py_AddConsti(dict, DOWNARROWKEY);
	Py_AddConsti(dict, RIGHTARROWKEY);
	Py_AddConsti(dict, UPARROWKEY);
	Py_AddConsti(dict, PAD2);
	Py_AddConsti(dict, PAD4);
	Py_AddConsti(dict, PAD6);
	Py_AddConsti(dict, PAD8);
	Py_AddConsti(dict, PAD1);
	Py_AddConsti(dict, PAD3);
	Py_AddConsti(dict, PAD5);
	Py_AddConsti(dict, PAD7);
	Py_AddConsti(dict, PAD9);
	Py_AddConsti(dict, PADPERIOD);
	Py_AddConsti(dict, PADVIRGULEKEY);
	Py_AddConsti(dict, PADASTERKEY);
	Py_AddConsti(dict, PAD0);
	Py_AddConsti(dict, PADMINUS);
	Py_AddConsti(dict, PADENTER);
	Py_AddConsti(dict, PADPLUSKEY);
	Py_AddConsti(dict, F1KEY);
	Py_AddConsti(dict, F2KEY);
	Py_AddConsti(dict, F3KEY);
	Py_AddConsti(dict, F4KEY);
	Py_AddConsti(dict, F5KEY);
	Py_AddConsti(dict, F6KEY);
	Py_AddConsti(dict, F7KEY);
	Py_AddConsti(dict, F8KEY);
	Py_AddConsti(dict, F9KEY);
	Py_AddConsti(dict, F10KEY);
	Py_AddConsti(dict, F11KEY);
	Py_AddConsti(dict, F12KEY);
	Py_AddConsti(dict, PAUSEKEY);
	Py_AddConsti(dict, INSERTKEY);
	Py_AddConsti(dict, HOMEKEY);
	Py_AddConsti(dict, PAGEUPKEY);
	Py_AddConsti(dict, PAGEDOWNKEY);
	Py_AddConsti(dict, ENDKEY);
	
	return mod;
}

#ifndef IRISGL

/* By golly George! It looks like fancy pants macro time!!! */

#define int_str				"i"
#define float_str			"f"
#define buffer_str			"O!"

#define int_var(number)		bgl_int##number
#define float_var(number)	bgl_float##number
#define buffer_var(number)	(bgl_buffer##number)->buf.asvoid

#define int_ref(number)		&bgl_int##number
#define float_ref(number)	&bgl_float##number
#define buffer_ref(number)	&Buffer_Type, &bgl_buffer##number

#define int_def(number)		int int_var(number)
#define float_def(number)	float float_var(number)
#define buffer_def(number)	Buffer *bgl_buffer##number

#define arg_def0()						int unused
#define arg_def1(a1)					a1##_def(1)
#define arg_def2(a1, a2)				arg_def1(a1); a2##_def(2)
#define arg_def3(a1, a2, a3)			arg_def2(a1, a2); a3##_def(3)
#define arg_def4(a1, a2, a3, a4)		arg_def3(a1, a2, a3); a4##_def(4)
#define arg_def5(a1, a2, a3, a4, a5)	arg_def4(a1, a2, a3, a4); a5##_def(5)
#define arg_def6(a1, a2, a3, a4, a5, a6)arg_def5(a1, a2, a3, a4, a5); a6##_def(6)
#define arg_def7(a1, a2, a3, a4, a5, a6, a7)arg_def6(a1, a2, a3, a4, a5, a6); a7##_def(7)
#define arg_def8(a1, a2, a3, a4, a5, a6, a7, a8)arg_def7(a1, a2, a3, a4, a5, a6, a7); a8##_def(8)
#define arg_def9(a1, a2, a3, a4, a5, a6, a7, a8, a9)arg_def8(a1, a2, a3, a4, a5, a6, a7, a8); a9##_def(9)
#define arg_def10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)arg_def9(a1, a2, a3, a4, a5, a6, a7, a8, a9); a10##_def(10)

#define arg_var0()		
#define arg_var1(a1)					a1##_var(1)
#define arg_var2(a1, a2)				arg_var1(a1), a2##_var(2)
#define arg_var3(a1, a2, a3)			arg_var2(a1, a2), a3##_var(3)
#define arg_var4(a1, a2, a3, a4)		arg_var3(a1, a2, a3), a4##_var(4)
#define arg_var5(a1, a2, a3, a4, a5)	arg_var4(a1, a2, a3, a4), a5##_var(5)
#define arg_var6(a1, a2, a3, a4, a5, a6)arg_var5(a1, a2, a3, a4, a5), a6##_var(6)
#define arg_var7(a1, a2, a3, a4, a5, a6, a7)arg_var6(a1, a2, a3, a4, a5, a6), a7##_var(7)
#define arg_var8(a1, a2, a3, a4, a5, a6, a7, a8)arg_var7(a1, a2, a3, a4, a5, a6, a7), a8##_var(8)
#define arg_var9(a1, a2, a3, a4, a5, a6, a7, a8, a9)arg_var8(a1, a2, a3, a4, a5, a6, a7, a8), a9##_var(9)
#define arg_var10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)arg_var9(a1, a2, a3, a4, a5, a6, a7, a8, a9), a10##_var(10)

#define arg_ref0()						NULL
#define arg_ref1(a1)					a1##_ref(1)
#define arg_ref2(a1, a2)				arg_ref1(a1), a2##_ref(2)
#define arg_ref3(a1, a2, a3)			arg_ref2(a1, a2), a3##_ref(3)
#define arg_ref4(a1, a2, a3, a4)		arg_ref3(a1, a2, a3), a4##_ref(4)
#define arg_ref5(a1, a2, a3, a4, a5)	arg_ref4(a1, a2, a3, a4), a5##_ref(5)
#define arg_ref6(a1, a2, a3, a4, a5, a6)arg_ref5(a1, a2, a3, a4, a5), a6##_ref(6)
#define arg_ref7(a1, a2, a3, a4, a5, a6, a7)arg_ref6(a1, a2, a3, a4, a5, a6), a7##_ref(7)
#define arg_ref8(a1, a2, a3, a4, a5, a6, a7, a8)arg_ref7(a1, a2, a3, a4, a5, a6, a7), a8##_ref(8)
#define arg_ref9(a1, a2, a3, a4, a5, a6, a7, a8, a9)arg_ref8(a1, a2, a3, a4, a5, a6, a7, a8), a9##_ref(9)
#define arg_ref10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)arg_ref9(a1, a2, a3, a4, a5, a6, a7, a8, a9), a10##_ref(10)

#define arg_str0()						""
#define arg_str1(a1)					a1##_str
#define arg_str2(a1, a2)				arg_str1(a1) a2##_str
#define arg_str3(a1, a2, a3)			arg_str2(a1, a2) a3##_str
#define arg_str4(a1, a2, a3, a4)		arg_str3(a1, a2, a3) a4##_str
#define arg_str5(a1, a2, a3, a4, a5)	arg_str4(a1, a2, a3, a4) a5##_str
#define arg_str6(a1, a2, a3, a4, a5, a6)arg_str5(a1, a2, a3, a4, a5) a6##_str
#define arg_str7(a1, a2, a3, a4, a5, a6, a7)arg_str6(a1, a2, a3, a4, a5, a6) a7##_str
#define arg_str8(a1, a2, a3, a4, a5, a6, a7, a8)arg_str7(a1, a2, a3, a4, a5, a6, a7) a8##_str
#define arg_str9(a1, a2, a3, a4, a5, a6, a7, a8, a9)arg_str8(a1, a2, a3, a4, a5, a6, a7, a8) a9##_str
#define arg_str10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)arg_str9(a1, a2, a3, a4, a5, a6, a7, a8, a9) a10##_str

#define ret_def_void	
#define ret_set_void	
#define ret_ret_void	return py_incr_ret(Py_None)

#define ret_def_int		int ret_int
#define ret_set_int		ret_int= 
#define ret_ret_int		return PyInt_FromLong(ret_int);

#define ret_def_string	const char *ret_str;
#define ret_set_string	ret_str= 
#define ret_ret_string	return PyString_FromString(ret_str);

#define BGL_Wrapa(nargs, funcname, add, ret, arg_list) \
static PyObject *Method_##funcname (PyObject *self, PyObject *args) {\
	arg_def##nargs##arg_list; \
	ret_def_##ret; \
	if(!PyArg_ParseTuple(args, arg_str##nargs##arg_list, arg_ref##nargs##arg_list)) return NULL;\
	ret_set_##ret gl##funcname##add (arg_var##nargs##arg_list);\
	ret_ret_##ret; \
}

#define BGL_Wrap(nargs, funcname, ret, arg_list) \
static PyObject *Method_##funcname (PyObject *self, PyObject *args) {\
	arg_def##nargs##arg_list; \
	ret_def_##ret; \
	if(!PyArg_ParseTuple(args, arg_str##nargs##arg_list, arg_ref##nargs##arg_list)) return NULL;\
	ret_set_##ret gl##funcname (arg_var##nargs##arg_list);\
	ret_ret_##ret; \
}

/* void glAccum (GLenum op, GLfloat value); */
BGL_Wrap(2, Accum, void, (int, float))

/* void glAlphaFunc (GLenum func, GLclampf ref); */
BGL_Wrap(2, AlphaFunc, void, (int, float))

/* void glBegin (GLenum mode); */
BGL_Wrap(1, Begin, void, (int))

/* void glBlendFunc (GLenum sfactor, GLenum dfactor); */
BGL_Wrap(2, BlendFunc, void, (int, int))

/* void glCallList (GLuint list); */
BGL_Wrap(1, CallList, void, (int))

/* void glClear (GLbitfield mask); */
BGL_Wrap(1, Clear, void, (int))

/* void glClearAccum (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha); */
BGL_Wrap(4, ClearAccum, void, (float, float, float, float))

/* void glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha); */
BGL_Wrap(4, ClearColor, void, (float, float, float, float))

/* void glClearDepth (GLclampd depth); */
BGL_Wrap(1, ClearDepth, void, (float))

/* void glClearIndex (GLfloat c); */
BGL_Wrap(1, ClearIndex, void, (float))

/* void glClearStencil (GLint s); */
BGL_Wrap(1, ClearStencil, void, (int))

/* void glColor3f (GLfloat red, GLfloat green, GLfloat blue); */
BGL_Wrapa(3, Color3, f, void, (float, float, float))

/* void glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha); */
BGL_Wrapa(4, Color4, f, void, (float, float, float, float))

/* void glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha); */
BGL_Wrap(4, ColorMask, void, (int, int, int, int))

/* void glColorMaterial (GLenum face, GLenum mode); */
BGL_Wrap(2, ColorMaterial, void, (int, int))

/* void glCopyPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type); */
BGL_Wrap(5, CopyPixels, void, (int, int, int, int, int))

/* void glCullFace (GLenum mode); */
BGL_Wrap(1, CullFace, void, (int))

/* void glDeleteLists (GLuint list, GLsizei range); */
BGL_Wrap(2, DeleteLists, void, (int, int))

/* void glDepthFunc (GLenum func); */
BGL_Wrap(1, DepthFunc, void, (int))

/* void glDepthMask (GLboolean flag); */
BGL_Wrap(1, DepthMask, void, (int))

/* void glDepthRange (GLclampd near, GLclampd far); */
BGL_Wrap(2, DepthRange, void, (float, float))

/* void glDisable (GLenum cap); */
BGL_Wrap(1, Disable, void, (int))

/* void glDrawBuffer (GLenum mode); */
BGL_Wrap(1, DrawBuffer, void, (int))

/* void glEdgeFlag (GLboolean flag); */
BGL_Wrap(1, EdgeFlag, void, (int))

/* void glEnable (GLenum cap); */
BGL_Wrap(1, Enable, void, (int))

/* void glEnd (void); */
BGL_Wrap(0, End, void, ())

/* void glEndList (void); */
BGL_Wrap(0, EndList, void, ())

/* void glEvalCoord1f (GLfloat u); */
BGL_Wrapa(1, EvalCoord1, f, void, (float))

/* void glEvalCoord2f (GLfloat u, GLfloat v); */
BGL_Wrapa(2, EvalCoord2, f, void, (float, float))

/* void glEvalMesh1 (GLenum mode, GLint i1, GLint i2); */
BGL_Wrap(3, EvalMesh1, void, (int, int, int))

/* void glEvalMesh2 (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2); */
BGL_Wrap(5, EvalMesh2, void, (int, int, int, int, int))

/* void glEvalPoint1 (GLint i); */
BGL_Wrap(1, EvalPoint1, void, (int))

/* void glEvalPoint2 (GLint i, GLint j); */
BGL_Wrap(2, EvalPoint2, void, (int, int))

/* void glFinish (void); */
BGL_Wrap(0, Finish, void, ())

/* void glFlush (void); */
BGL_Wrap(0, Flush, void, ())

/* void glFogf (GLenum pname, GLfloat param); */
BGL_Wrapa(2, Fog, f, void, (int, float))

/* void glFrontFace (GLenum mode); */
BGL_Wrap(1, FrontFace, void, (int))

/* void glFrustum (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far); */
BGL_Wrap(6, Frustum, void, (float, float, float, float, float, float))

/* GLuint glGenLists (GLsizei range); */
BGL_Wrap(1, GenLists, int, (int))

/* GLenum glGetError (void); */
BGL_Wrap(0, GetError, int, ())

/* const GLubyte* glGetString (GLenum name); */
BGL_Wrap(1, GetString, string, (int))

/* void glHint (GLenum target, GLenum mode); */
BGL_Wrap(2, Hint, void, (int, int))

/* void glIndex (GLfloat c); */
BGL_Wrapa(1, Index, f, void, (float))

/* void glInitNames (void); */
BGL_Wrap(0, InitNames, void, ())

/* GLboolean glIsEnabled (GLenum cap); */
BGL_Wrap(1, IsEnabled, int, (int))

/* GLboolean glIsList (GLuint list); */
BGL_Wrap(1, IsList, int, (int))

/* void glLightModelf (GLenum pname, GLfloat param); */
BGL_Wrapa(2, LightModel, f, void, (int, float))

/* void glLightf (GLenum light, GLenum pname, GLfloat param); */
BGL_Wrapa(3, Light, f, void, (int, int, float))

/* void glLineStipple (GLint factor, GLushort pattern); */
BGL_Wrap(2, LineStipple, void, (int, int))

/* void glLineWidth (GLfloat width); */
BGL_Wrap(1, LineWidth, void, (float))

/* void glListBase (GLuint base); */
BGL_Wrap(1, ListBase, void, (int))

/* void glLoadIdentity (void); */
BGL_Wrap(0, LoadIdentity, void, ())

/* void glLoadName (GLuint name); */
BGL_Wrap(1, LoadName, void, (int))

/* void glLogicOp (GLenum opcode); */
BGL_Wrap(1, LogicOp, void, (int))

/* void glMapGrid1f (GLint un, GLfloat u1, GLfloat u2); */
BGL_Wrapa(3, MapGrid1, f, void, (int, float, float))

/* void glMapGrid2f (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2); */
BGL_Wrapa(6, MapGrid2, f, void, (int, float, float, int, float, float))

/* void glMaterialf (GLenum face, GLenum pname, GLfloat param); */
BGL_Wrapa(3, Material, f, void, (int, int, float))

/* void glMatrixMode (GLenum mode); */
BGL_Wrap(1, MatrixMode, void, (int))

/* void glNewList (GLuint list, GLenum mode); */
BGL_Wrap(2, NewList, void, (int, int))

/* void glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz); */
BGL_Wrapa(3, Normal3, f, void, (float, float, float))

/* void glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far); */
BGL_Wrap(6, Ortho, void, (float, float, float, float, float, float))

/* void glPassThrough (GLfloat token); */
BGL_Wrap(1, PassThrough, void, (float))

/* void glPixelStoref (GLenum pname, GLfloat param); */
BGL_Wrapa(2, PixelStore, f, void, (int, float))

/* void glPixelTransferf (GLenum pname, GLfloat param); */
BGL_Wrapa(2, PixelTransfer, f, void, (int, float))

/* void glPixelZoom (GLfloat xfactor, GLfloat yfactor); */
BGL_Wrap(2, PixelZoom, void, (float, float))

/* void glPointSize (GLfloat size); */
BGL_Wrap(1, PointSize, void, (float))

/* void glPolygonMode (GLenum face, GLenum mode); */
BGL_Wrap(2, PolygonMode, void, (int, int))

/* void glPopAttrib (void); */
BGL_Wrap(0, PopAttrib, void, ())

/* void glPopMatrix (void); */
BGL_Wrap(0, PopMatrix, void, ())

/* void glPopName (void); */
BGL_Wrap(0, PopName, void, ())

/* void glPushAttrib (GLbitfield mask); */
BGL_Wrap(1, PushAttrib, void, (int))

/* void glPushMatrix (void); */
BGL_Wrap(0, PushMatrix, void, ())

/* void glPushName (GLuint name); */
BGL_Wrap(1, PushName, void, (int))

/* void glRasterPos2f (GLfloat x, GLfloat y); */
BGL_Wrapa(2, RasterPos2, f, void, (float, float))

/* void glRasterPos3f (GLfloat x, GLfloat y, GLfloat z); */
BGL_Wrapa(3, RasterPos3, f, void, (float, float, float))

/* void glRasterPos4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w); */
BGL_Wrapa(4, RasterPos4, f, void, (float, float, float, float))

/* void glReadBuffer (GLenum mode); */
BGL_Wrap(1, ReadBuffer, void, (int))

/* void glRectf (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2); */
BGL_Wrapa(4, Rect, f, void, (float, float, float, float))

/* GLint glRenderMode (GLenum mode); */
BGL_Wrap(1, RenderMode, int, (int))

/* void glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z); */
BGL_Wrapa(4, Rotate, f, void, (float, float, float, float))

/* void glScalef (GLfloat x, GLfloat y, GLfloat z); */
BGL_Wrapa(3, Scale, f, void, (float, float, float))

/* void glScissor (GLint x, GLint y, GLsizei width, GLsizei height); */
BGL_Wrap(4, Scissor, void, (int, int, int, int))

/* void glShadeModel (GLenum mode); */
BGL_Wrap(1, ShadeModel, void, (int))

/* void glStencilFunc (GLenum func, GLint ref, GLuint mask); */
BGL_Wrap(3, StencilFunc, void, (int, int, int))

/* void glStencilMask (GLuint mask); */
BGL_Wrap(1, StencilMask, void, (int))

/* void glStencilOp (GLenum fail, GLenum zfail, GLenum zpass); */
BGL_Wrap(3, StencilOp, void, (int, int, int))

/* void glTexCoord1f (GLfloat s); */
BGL_Wrapa(1, TexCoord1, f, void, (float))

/* void glTexCoord2f (GLfloat s, GLfloat t); */
BGL_Wrapa(2, TexCoord2, f, void, (float, float))

/* void glTexCoord3f (GLfloat s, GLfloat t, GLfloat r); */
BGL_Wrapa(3, TexCoord3, f, void, (float, float, float))

/* void glTexCoord4f (GLfloat s, GLfloat t, GLfloat r, GLfloat q); */
BGL_Wrapa(4, TexCoord4, f, void, (float, float, float, float))

/* void glTexEnvf (GLenum target, GLenum pname, GLfloat param); */
BGL_Wrapa(3, TexEnv, f, void, (int, int, float))

/* void glTexGenf (GLenum coord, GLenum pname, GLfloat param); */
BGL_Wrapa(3, TexGen, f, void, (int, int, float))

/* void glTexParameterf (GLenum target, GLenum pname, GLfloat param); */
BGL_Wrapa(3, TexParameter, f, void, (int, int, float))

/* void glTranslatef (GLfloat x, GLfloat y, GLfloat z); */
BGL_Wrapa(3, Translate, f, void, (float, float, float))

/* void glVertex2f (GLfloat x, GLfloat y); */
BGL_Wrapa(2, Vertex2, f, void, (float, float))

/* void glVertex3f (GLfloat x, GLfloat y, GLfloat z); */
BGL_Wrapa(3, Vertex3, f, void, (float, float, float))

/* void glVertex4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w); */
BGL_Wrapa(4, Vertex4, f, void, (float, float, float, float))

/* void glViewport (GLint x, GLint y, GLsizei width, GLsizei height); */
BGL_Wrap(4, Viewport, void, (int, int, int, int))

/* SPECIAL FUNCTIONS */

/* Buffer Object */

PyTypeObject Buffer_Type;

typedef struct _Buffer {
	PyObject_VAR_HEAD

	int type; /* GL_BYTE, GL_SHORT, GL_INT, GL_FLOAT */
	int rows, cols, items;
	
	union {
		char *aschar;
		short *asshort;
		int *asint;
		float *asfloat;
		void *asvoid;
	} buf;
} Buffer;

static void Buffer_dealloc(PyObject *self) {
	Buffer *buf= (Buffer *) self;

	freeN(buf->buf.asvoid);
		
	PyMem_DEL(self);	
}

static PyObject *Method_BufList (PyObject *self, PyObject *args)
{
	Buffer *buf=NULL;
	int x, y, b, set;
	PyObject *list, *row, *col, *item;
	char *cbuf;
	short *sbuf;
	int *ibuf;
	float *fbuf;
	
	Py_Try(PyArg_ParseTuple(args, "O!", &Buffer_Type, &buf));	
	
	cbuf= buf->buf.aschar;
	sbuf= buf->buf.asshort;
	ibuf= buf->buf.asint;
	fbuf= buf->buf.asfloat;

	list= PyList_New(buf->rows);
	
	for(y= 0; y<buf->rows; y++) {
		if (buf->items==1) {
			if(buf->type==GL_BYTE) row= PyInt_FromLong(*(cbuf++));
			else if (buf->type==GL_SHORT) row= PyInt_FromLong(*(sbuf++));
			else if (buf->type==GL_INT) row= PyInt_FromLong(*(ibuf++));
			else if (buf->type==GL_FLOAT) row= PyFloat_FromDouble(*(fbuf++));
		} else {
			row= PyList_New(buf->cols);
	
			for(x= 0; x<buf->cols; x++) {
				if (buf->items==1) {
					if(buf->type==GL_BYTE) col= PyInt_FromLong(*(cbuf++));
					else if (buf->type==GL_SHORT) col= PyInt_FromLong(*(sbuf++));
					else if (buf->type==GL_INT) col= PyInt_FromLong(*(ibuf++));
					else if (buf->type==GL_FLOAT) col= PyFloat_FromDouble(*(fbuf++));
				} else {
					col= PyList_New(buf->items);
	
					for(b= 0; b<buf->items; b++) {
						if(buf->type==GL_BYTE) item= PyInt_FromLong(*(cbuf++));
						else if (buf->type==GL_SHORT) item= PyInt_FromLong(*(sbuf++));
						else if (buf->type==GL_INT) item= PyInt_FromLong(*(ibuf++));
						else if (buf->type==GL_FLOAT) item= PyFloat_FromDouble(*(fbuf++));
					
						Py_INCREF(item);
						PyList_SetItem(col, b, item);
					}
				}
				
				Py_INCREF(col);
				PyList_SetItem(row, x, col);
			}
		}
		
		Py_INCREF(row);
		PyList_SetItem(list, y, row);
	}
	
	return list;	
}

static PyObject *Method_BufSet (PyObject *self, PyObject *args)
{
	Buffer *buf=NULL;
	int x, y, i, inc;
	PyObject *set;
	
	Py_Try(PyArg_ParseTuple(args, "O!iiiO", &Buffer_Type, &buf, &x, &y, &i, &set));	
	
	if (x>=buf->cols || y>=buf->rows || i>=buf->items) {
		PyErr_SetString(PyExc_IndexError, "buffer bounds exceeded");
		return NULL;
	}
	
	inc= (y*(buf->cols)+x)*(buf->items) + i;
	if(buf->type==GL_BYTE)
		buf->buf.aschar[inc]= PyInt_AsLong(set);
		
	else if (buf->type==GL_SHORT)
		buf->buf.asshort[inc]= PyInt_AsLong(set);
		
	else if (buf->type==GL_INT)
		buf->buf.asint[inc]= PyInt_AsLong(set);
		
	else if (buf->type==GL_FLOAT)
		buf->buf.asfloat[inc]= PyFloat_AsDouble(set);
	
	return py_incr_ret(Py_None);	
}
	
static PyObject *Method_Buffer (PyObject *self, PyObject *args)
{
	int rows=1, cols=1, items=1, type;
	PyObject *buf=NULL, *row=NULL, *col=NULL, *item;
	Buffer *newbuf;
	void *buffer= NULL;
	int x, y, b, numc, numr, numb, isize;
	char *cbuf;
	short *sbuf;
	int *ibuf;
	float *fbuf;
	
	Py_Try(PyArg_ParseTuple(args, "i|Oiii", &type, &buf, &rows, &cols, &items));

	switch(type) {
	case GL_BYTE: isize= sizeof(char); break;
	case GL_SHORT: isize= sizeof(short); break;
	case GL_INT: isize= sizeof(int); break;
	case GL_FLOAT: isize= sizeof(float); break;
	default:
		PyErr_SetString(PyExc_AttributeError, "type");
		return NULL;
	}
	
	buffer= mallocN(rows*cols*items*isize, "PyBuffer buffer");
	
	cbuf= (char*)	buffer;
	sbuf= (short*)	buffer;
	ibuf= (int*)	buffer;
	fbuf= (float*)	buffer;

	if (rows==1 && cols==1 && items==1 && !PyList_Check(buf)) {
		if (type==GL_BYTE) *(cbuf++)= PyInt_AsLong(buf);
		else if (type==GL_SHORT) *(sbuf++)= PyInt_AsLong(buf);
		else if (type==GL_INT) *(ibuf++)= PyInt_AsLong(buf);
		else if (type==GL_FLOAT) *(fbuf++)= PyFloat_AsDouble(buf);
	} else {
		if(buf && !PyList_Check(buf)) buf= NULL;
	
		if(buf) numr= PyList_Size(buf);
		else numr= 0;
		
		for (y=0; y<numr; y++) {
			row= PyList_GetItem(buf, y);
	
			if (cols==1 && items==1 && !PyList_Check(row)) {
				if (type==GL_BYTE) *(cbuf++)= PyInt_AsLong(row);
				else if (type==GL_SHORT) *(sbuf++)= PyInt_AsLong(row);
				else if (type==GL_INT) *(ibuf++)= PyInt_AsLong(row);
				else if (type==GL_FLOAT) *(fbuf++)= PyFloat_AsDouble(row);
			} else {
				if(!PyList_Check(row)) row= NULL;
	
				if(row) numc= PyList_Size(row);
				else numc= 0;
			
				for(x=0; x<numc; x++) {
					col= PyList_GetItem(row, x);
		
					if (items==1 && !PyList_Check(col)) {
						if (type==GL_BYTE) *(cbuf++)= PyInt_AsLong(col);
						else if (type==GL_SHORT) *(sbuf++)= PyInt_AsLong(col);
						else if (type==GL_INT) *(ibuf++)= PyInt_AsLong(col);
						else if (type==GL_FLOAT) *(fbuf++)= PyFloat_AsDouble(col);
					} else {			
						if(!PyList_Check(col)) col= NULL;
		
						if(col) numb= PyList_Size(col);
						else numb= 0;
					
						for (b=0; b<numb; b++) {
							item= PyList_GetItem(col, b);
						
							if (type==GL_BYTE) *(cbuf++)= PyInt_AsLong(item);
							else if (type==GL_SHORT) *(sbuf++)= PyInt_AsLong(item);
							else if (type==GL_INT) *(ibuf++)= PyInt_AsLong(item);
							else if (type==GL_FLOAT) *(fbuf++)= PyFloat_AsDouble(item);
						}
						while (b<items) {
							if (type==GL_BYTE) *(cbuf++)= 0;
							else if (type==GL_SHORT) *(sbuf++)= 0;
							else if (type==GL_INT) *(ibuf++)= 0;
							else if (type==GL_FLOAT) *(fbuf++)= 0.0;
							b++;
						}
					}
				}
				while(x<cols) {
					for (b=0; b<items; b++) {
						if (type==GL_BYTE) *(cbuf++)= 0;
						else if (type==GL_SHORT) *(sbuf++)= 0;
						else if (type==GL_INT) *(ibuf++)= 0;
						else if (type==GL_FLOAT) *(fbuf++)= 0.0;
					}
					x++;
				}
			}
		}
		while (y<rows) {
			for (x=0; x<cols; x++) {
				for (b=0; b<items; b++) {
					if (type==GL_BYTE) *(cbuf++)= 0;
					else if (type==GL_SHORT) *(sbuf++)= 0;
					else if (type==GL_INT) *(ibuf++)= 0;
					else if (type==GL_FLOAT) *(fbuf++)= 0.0;
				}
			}
			y++;
		}
	}
	
	newbuf= (Buffer *) PyObject_NEW(Buffer, &Buffer_Type);
	newbuf->rows= rows;
	newbuf->cols= cols;
	newbuf->items= items;
	newbuf->type= type;
	newbuf->buf.asvoid= buffer;
	
	return (PyObject *) newbuf;
}

PyTypeObject Buffer_Type = {
	PyObject_HEAD_INIT(NULL)
	0,								/*ob_size*/
	"Buffer",						/*tp_name*/
	sizeof(Buffer),					/*tp_basicsize*/
	0,								/*tp_itemsize*/
	(destructor) Buffer_dealloc,	/*tp_dealloc*/
	(printfunc)  0,					/*tp_print*/
	(getattrfunc) 0,				/*tp_getattr*/
	(setattrfunc) 0,				/*tp_setattr*/
};

/* void glGetFloatv (GLenum pname, GLfloat *params); */
BGL_Wrapa(2, GetFloat, v, void, (int, buffer))

/* void glGetIntegerv (GLenum pname, GLint *params); */
BGL_Wrapa(2, GetInteger, v, void, (int, buffer))

/* void glGetClipPlane (GLenum plane, GLdouble *equation); */
BGL_Wrap(2, GetClipPlane, void, (int, buffer))

/* void glGetLightfv (GLenum light, GLenum pname, GLfloat *params); */
BGL_Wrapa(3, GetLight, fv, void, (int, int, buffer))

/* void glGetMapfv (GLenum target, GLenum query, GLfloat *v); */
BGL_Wrapa(3, GetMap, fv, void, (int, int, buffer))

/* void glGetMaterialfv (GLenum face, GLenum pname, GLfloat *params); */
BGL_Wrapa(3, GetMaterial, fv, void, (int, int, buffer))

/* void glGetPixelMapfv (GLenum map, GLfloat *values); */
BGL_Wrapa(2, GetPixelMap, fv, void, (int, buffer))

/* void glGetPolygonStipple (GLubyte *mask); */
BGL_Wrap(1, GetPolygonStipple, void, (buffer))

/* void glGetTexEnvfv (GLenum target, GLenum pname, GLfloat *params); */
BGL_Wrapa(3, GetTexEnv, fv, void, (int, int, buffer))

/* void glGetTexGenfv (GLenum coord, GLenum pname, GLfloat *params); */
BGL_Wrapa(3, GetTexGen, fv, void, (int, int, buffer))

/* void glGetTexImage (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels); */
BGL_Wrap(5, GetTexImage, void, (int, int, int, int, buffer))

/* void glGetTexLevelParameterfv (GLenum target, GLint level, GLenum pname, GLfloat *params); */
BGL_Wrapa(4, GetTexLevelParameter, fv, void, (int, int, int, buffer))

/* void glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params); */
BGL_Wrapa(3, GetTexParameter, fv, void, (int, int, buffer))



/* void glBitmap (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap); */
BGL_Wrap(7, Bitmap, void, (int, int, float, float, float, float, buffer))

/* void glClipPlane (GLenum plane, const GLdouble *equation); */
BGL_Wrap(2, ClipPlane, void, (int, buffer))

/* void glDrawPixels (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels); */
BGL_Wrap(5, DrawPixels, void, (int, int, int, int, buffer))

/* void glFeedbackBuffer (GLsizei size, GLenum type, GLfloat *buffer); */
BGL_Wrap(3, FeedbackBuffer, void, (int, int, buffer))

/* void glLoadMatrix (const GLfloat *m); */
BGL_Wrapa(1, LoadMatrix, f, void, (buffer))

/* void glMultMatrix (const GLfloat *m); */
BGL_Wrapa(1, MultMatrix, f, void, (buffer))

/* void glPolygonStipple (const GLubyte *mask); */
BGL_Wrap(1, PolygonStipple, void, (buffer))

/* void glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels); */
BGL_Wrap(7, ReadPixels, void, (int, int, int, int, int, int, buffer))

/* void glSelectBuffer (GLsizei size, GLuint *buffer); */
BGL_Wrap(2, SelectBuffer, void, (int, buffer))

/* void glMap1f (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points); */
BGL_Wrapa(6, Map1, f, void, (int, float, float, int, int, buffer))

/* void glMap2f (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points); */
BGL_Wrapa(10, Map2, f, void, (int, float, float, int, int, float, float, int, int, buffer))

/* void glTexImage1D (GLenum target, GLint level, GLint components, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels); */
BGL_Wrap(8, TexImage1D, void, (int, int, int, int, int, int, int, buffer))

/* void glTexImage2D (GLenum target, GLint level, GLint components, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels); */
BGL_Wrap(9, TexImage2D, void, (int, int, int, int, int, int, int, int, buffer))

/* void glPixelMapfv (GLenum map, GLint mapsize, const GLfloat *values); */
BGL_Wrapa(3, PixelMap, fv, void, (int, int, buffer))

#undef MethodDef	/* py_blender.h */
#define MethodDef(func) {"gl"#func, Method_##func, METH_VARARGS}

static struct PyMethodDef BGL_methods[] = {
	{"Buffer", Method_Buffer, METH_VARARGS}, 
	{"BufSet", Method_BufSet, METH_VARARGS}, 
	{"BufList", Method_BufList, METH_VARARGS}, 

	MethodDef( Accum ),
	MethodDef( AlphaFunc ),
	MethodDef( Begin ),
	MethodDef( BlendFunc ),
	MethodDef( CallList ),
	MethodDef( Clear ),
	MethodDef( ClearAccum ),
	MethodDef( ClearColor ),
	MethodDef( ClearDepth ),
	MethodDef( ClearIndex ),
	MethodDef( ClearStencil ),
	MethodDef( Color3 ),
	MethodDef( Color4 ),
	MethodDef( ColorMask ),
	MethodDef( ColorMaterial ),
	MethodDef( CopyPixels ),
	MethodDef( CullFace ),
	MethodDef( DeleteLists ),
	MethodDef( DepthFunc ),
	MethodDef( DepthMask ),
	MethodDef( DepthRange ),
	MethodDef( Disable ),
	MethodDef( DrawBuffer ),
	MethodDef( EdgeFlag ),
	MethodDef( Enable ),
	MethodDef( End ),
	MethodDef( EndList ),
	MethodDef( EvalCoord1 ),
	MethodDef( EvalCoord2 ),
	MethodDef( EvalMesh1 ),
	MethodDef( EvalMesh2 ),
	MethodDef( EvalPoint1 ),
	MethodDef( EvalPoint2 ),
	MethodDef( Finish ),
	MethodDef( Flush ),
	MethodDef( Fog ),
	MethodDef( FrontFace ),
	MethodDef( Frustum ),
	MethodDef( GenLists ),
	MethodDef( GetError ),
	MethodDef( GetString ),
	MethodDef( Hint ),
	MethodDef( Index ),
	MethodDef( InitNames ),
	MethodDef( IsEnabled ),
	MethodDef( IsList ),
	MethodDef( LightModel ),
	MethodDef( Light ),
	MethodDef( LineStipple ),
	MethodDef( LineWidth ),
	MethodDef( ListBase ),
	MethodDef( LoadIdentity ),
	MethodDef( LoadName ),
	MethodDef( LogicOp ),
	MethodDef( MapGrid1 ),
	MethodDef( MapGrid2 ),
	MethodDef( Material ),
	MethodDef( MatrixMode ),
	MethodDef( NewList ),
	MethodDef( Normal3 ),
	MethodDef( Ortho ),
	MethodDef( PassThrough ),
	MethodDef( PixelStore ),
	MethodDef( PixelTransfer ),
	MethodDef( PixelZoom ),
	MethodDef( PointSize ),
	MethodDef( PolygonMode ),
	MethodDef( PopAttrib ),
	MethodDef( PopMatrix ),
	MethodDef( PopName ),
	MethodDef( PushAttrib ),
	MethodDef( PushMatrix ),
	MethodDef( PushName ),
	MethodDef( RasterPos2 ),
	MethodDef( RasterPos3 ),
	MethodDef( RasterPos4 ),
	MethodDef( ReadBuffer ),
	MethodDef( Rect ),
	MethodDef( RenderMode ),
	MethodDef( Rotate ),
	MethodDef( Scale ),
	MethodDef( Scissor ),
	MethodDef( ShadeModel ),
	MethodDef( StencilFunc ),
	MethodDef( StencilMask ),
	MethodDef( StencilOp ),
	MethodDef( TexCoord1 ),
	MethodDef( TexCoord2 ),
	MethodDef( TexCoord3 ),
	MethodDef( TexCoord4 ),
	MethodDef( TexEnv ),
	MethodDef( TexGen ),
	MethodDef( TexParameter ),
	MethodDef( Translate ),
	MethodDef( Vertex2 ),
	MethodDef( Vertex3 ),
	MethodDef( Vertex4 ),
	MethodDef( Viewport ),
	MethodDef( GetFloat ),
	MethodDef( GetInteger ),
	MethodDef( GetClipPlane ),
	MethodDef( GetLight ),
	MethodDef( GetMap ),
	MethodDef( GetMaterial ),
	MethodDef( GetPixelMap ),
	MethodDef( GetPolygonStipple ),
	MethodDef( GetTexEnv ),
	MethodDef( GetTexGen ),
	MethodDef( GetTexImage ),
	MethodDef( GetTexLevelParameter ),
	MethodDef( GetTexParameter ),
	MethodDef( Bitmap ),
	MethodDef( ClipPlane ),
	MethodDef( DrawPixels ),
	MethodDef( FeedbackBuffer ),
	MethodDef( LoadMatrix ),
	MethodDef( MultMatrix ),
	MethodDef( PolygonStipple ),
	MethodDef( ReadPixels ),
	MethodDef( SelectBuffer ),
	MethodDef( Map1 ),
	MethodDef( Map2 ),
	MethodDef( TexImage1D ),
	MethodDef( TexImage2D ),
	MethodDef( PixelMap ),	{NULL, NULL}
};

PyObject *init_py_bgl(void) 
{
	PyObject *mod= Py_InitModule("Blender.BGL", BGL_methods);
	PyObject *dict= PyModule_GetDict(mod);

	Buffer_Type.ob_type= &PyType_Type;

	Py_AddConsti(dict, GL_CURRENT_BIT);
	Py_AddConsti(dict, GL_POINT_BIT);
	Py_AddConsti(dict, GL_LINE_BIT);
	Py_AddConsti(dict, GL_POLYGON_BIT);
	Py_AddConsti(dict, GL_POLYGON_STIPPLE_BIT);
	Py_AddConsti(dict, GL_PIXEL_MODE_BIT);
	Py_AddConsti(dict, GL_LIGHTING_BIT);
	Py_AddConsti(dict, GL_FOG_BIT);
	Py_AddConsti(dict, GL_DEPTH_BUFFER_BIT);
	Py_AddConsti(dict, GL_ACCUM_BUFFER_BIT);
	Py_AddConsti(dict, GL_STENCIL_BUFFER_BIT);
	Py_AddConsti(dict, GL_VIEWPORT_BIT);
	Py_AddConsti(dict, GL_TRANSFORM_BIT);
	Py_AddConsti(dict, GL_ENABLE_BIT);
	Py_AddConsti(dict, GL_COLOR_BUFFER_BIT);
	Py_AddConsti(dict, GL_HINT_BIT);
	Py_AddConsti(dict, GL_EVAL_BIT);
	Py_AddConsti(dict, GL_LIST_BIT);
	Py_AddConsti(dict, GL_TEXTURE_BIT);
	Py_AddConsti(dict, GL_SCISSOR_BIT);
	Py_AddConsti(dict, GL_ALL_ATTRIB_BITS);

	Py_AddConsti(dict, GL_FALSE);
	Py_AddConsti(dict, GL_TRUE);

	Py_AddConsti(dict, GL_POINTS);
	Py_AddConsti(dict, GL_LINES);
	Py_AddConsti(dict, GL_LINE_LOOP);
	Py_AddConsti(dict, GL_LINE_STRIP);
	Py_AddConsti(dict, GL_TRIANGLES);
	Py_AddConsti(dict, GL_TRIANGLE_STRIP);
	Py_AddConsti(dict, GL_TRIANGLE_FAN);
	Py_AddConsti(dict, GL_QUADS);
	Py_AddConsti(dict, GL_QUAD_STRIP);
	Py_AddConsti(dict, GL_POLYGON);

	Py_AddConsti(dict, GL_ACCUM);
	Py_AddConsti(dict, GL_LOAD);
	Py_AddConsti(dict, GL_RETURN);
	Py_AddConsti(dict, GL_MULT);
	Py_AddConsti(dict, GL_ADD);

	Py_AddConsti(dict, GL_NEVER);
	Py_AddConsti(dict, GL_LESS);
	Py_AddConsti(dict, GL_EQUAL);
	Py_AddConsti(dict, GL_LEQUAL);
	Py_AddConsti(dict, GL_GREATER);
	Py_AddConsti(dict, GL_NOTEQUAL);
	Py_AddConsti(dict, GL_GEQUAL);
	Py_AddConsti(dict, GL_ALWAYS);

	Py_AddConsti(dict, GL_ZERO);
	Py_AddConsti(dict, GL_ONE);
	Py_AddConsti(dict, GL_SRC_COLOR);
	Py_AddConsti(dict, GL_ONE_MINUS_SRC_COLOR);
	Py_AddConsti(dict, GL_SRC_ALPHA);
	Py_AddConsti(dict, GL_ONE_MINUS_SRC_ALPHA);
	Py_AddConsti(dict, GL_DST_ALPHA);
	Py_AddConsti(dict, GL_ONE_MINUS_DST_ALPHA);

	Py_AddConsti(dict, GL_DST_COLOR);
	Py_AddConsti(dict, GL_ONE_MINUS_DST_COLOR);
	Py_AddConsti(dict, GL_SRC_ALPHA_SATURATE);

	Py_AddConsti(dict, GL_NONE);
	Py_AddConsti(dict, GL_FRONT_LEFT);
	Py_AddConsti(dict, GL_FRONT_RIGHT);
	Py_AddConsti(dict, GL_BACK_LEFT);
	Py_AddConsti(dict, GL_BACK_RIGHT);
	Py_AddConsti(dict, GL_FRONT);
	Py_AddConsti(dict, GL_BACK);
	Py_AddConsti(dict, GL_LEFT);
	Py_AddConsti(dict, GL_RIGHT);
	Py_AddConsti(dict, GL_FRONT_AND_BACK);
	Py_AddConsti(dict, GL_AUX0);
	Py_AddConsti(dict, GL_AUX1);
	Py_AddConsti(dict, GL_AUX2);
	Py_AddConsti(dict, GL_AUX3);

	Py_AddConsti(dict, GL_NO_ERROR);
	Py_AddConsti(dict, GL_INVALID_ENUM);
	Py_AddConsti(dict, GL_INVALID_VALUE);
	Py_AddConsti(dict, GL_INVALID_OPERATION);
	Py_AddConsti(dict, GL_STACK_OVERFLOW);
	Py_AddConsti(dict, GL_STACK_UNDERFLOW);
	Py_AddConsti(dict, GL_OUT_OF_MEMORY);

	Py_AddConsti(dict, GL_2D);
	Py_AddConsti(dict, GL_3D);
	Py_AddConsti(dict, GL_3D_COLOR);
	Py_AddConsti(dict, GL_3D_COLOR_TEXTURE);
	Py_AddConsti(dict, GL_4D_COLOR_TEXTURE);

	Py_AddConsti(dict, GL_PASS_THROUGH_TOKEN);
	Py_AddConsti(dict, GL_POINT_TOKEN);
	Py_AddConsti(dict, GL_LINE_TOKEN);
	Py_AddConsti(dict, GL_POLYGON_TOKEN);
	Py_AddConsti(dict, GL_BITMAP_TOKEN);
	Py_AddConsti(dict, GL_DRAW_PIXEL_TOKEN);
	Py_AddConsti(dict, GL_COPY_PIXEL_TOKEN);
	Py_AddConsti(dict, GL_LINE_RESET_TOKEN);

	Py_AddConsti(dict, GL_EXP);
	Py_AddConsti(dict, GL_EXP2);

	Py_AddConsti(dict, GL_CW);
	Py_AddConsti(dict, GL_CCW);

	Py_AddConsti(dict, GL_COEFF);
	Py_AddConsti(dict, GL_ORDER);
	Py_AddConsti(dict, GL_DOMAIN);

	Py_AddConsti(dict, GL_PIXEL_MAP_I_TO_I);
	Py_AddConsti(dict, GL_PIXEL_MAP_S_TO_S);
	Py_AddConsti(dict, GL_PIXEL_MAP_I_TO_R);
	Py_AddConsti(dict, GL_PIXEL_MAP_I_TO_G);
	Py_AddConsti(dict, GL_PIXEL_MAP_I_TO_B);
	Py_AddConsti(dict, GL_PIXEL_MAP_I_TO_A);
	Py_AddConsti(dict, GL_PIXEL_MAP_R_TO_R);
	Py_AddConsti(dict, GL_PIXEL_MAP_G_TO_G);
	Py_AddConsti(dict, GL_PIXEL_MAP_B_TO_B);
	Py_AddConsti(dict, GL_PIXEL_MAP_A_TO_A);

	Py_AddConsti(dict, GL_CURRENT_COLOR);
	Py_AddConsti(dict, GL_CURRENT_INDEX);
	Py_AddConsti(dict, GL_CURRENT_NORMAL);
	Py_AddConsti(dict, GL_CURRENT_TEXTURE_COORDS);
	Py_AddConsti(dict, GL_CURRENT_RASTER_COLOR);
	Py_AddConsti(dict, GL_CURRENT_RASTER_INDEX);
	Py_AddConsti(dict, GL_CURRENT_RASTER_TEXTURE_COORDS);
	Py_AddConsti(dict, GL_CURRENT_RASTER_POSITION);
	Py_AddConsti(dict, GL_CURRENT_RASTER_POSITION_VALID);
	Py_AddConsti(dict, GL_CURRENT_RASTER_DISTANCE);
	Py_AddConsti(dict, GL_POINT_SMOOTH);
	Py_AddConsti(dict, GL_POINT_SIZE);
	Py_AddConsti(dict, GL_POINT_SIZE_RANGE);
	Py_AddConsti(dict, GL_POINT_SIZE_GRANULARITY);
	Py_AddConsti(dict, GL_LINE_SMOOTH);
	Py_AddConsti(dict, GL_LINE_WIDTH);
	Py_AddConsti(dict, GL_LINE_WIDTH_RANGE);
	Py_AddConsti(dict, GL_LINE_WIDTH_GRANULARITY);
	Py_AddConsti(dict, GL_LINE_STIPPLE);
	Py_AddConsti(dict, GL_LINE_STIPPLE_PATTERN);
	Py_AddConsti(dict, GL_LINE_STIPPLE_REPEAT);
	Py_AddConsti(dict, GL_LIST_MODE);
	Py_AddConsti(dict, GL_MAX_LIST_NESTING);
	Py_AddConsti(dict, GL_LIST_BASE);
	Py_AddConsti(dict, GL_LIST_INDEX);
	Py_AddConsti(dict, GL_POLYGON_MODE);
	Py_AddConsti(dict, GL_POLYGON_SMOOTH);
	Py_AddConsti(dict, GL_POLYGON_STIPPLE);
	Py_AddConsti(dict, GL_EDGE_FLAG);
	Py_AddConsti(dict, GL_CULL_FACE);
	Py_AddConsti(dict, GL_CULL_FACE_MODE);
	Py_AddConsti(dict, GL_FRONT_FACE);
	Py_AddConsti(dict, GL_LIGHTING);
	Py_AddConsti(dict, GL_LIGHT_MODEL_LOCAL_VIEWER);
	Py_AddConsti(dict, GL_LIGHT_MODEL_TWO_SIDE);
	Py_AddConsti(dict, GL_LIGHT_MODEL_AMBIENT);
	Py_AddConsti(dict, GL_SHADE_MODEL);
	Py_AddConsti(dict, GL_COLOR_MATERIAL_FACE);
	Py_AddConsti(dict, GL_COLOR_MATERIAL_PARAMETER);
	Py_AddConsti(dict, GL_COLOR_MATERIAL);
	Py_AddConsti(dict, GL_FOG);
	Py_AddConsti(dict, GL_FOG_INDEX);
	Py_AddConsti(dict, GL_FOG_DENSITY);
	Py_AddConsti(dict, GL_FOG_START);
	Py_AddConsti(dict, GL_FOG_END);
	Py_AddConsti(dict, GL_FOG_MODE);
	Py_AddConsti(dict, GL_FOG_COLOR);
	Py_AddConsti(dict, GL_DEPTH_RANGE);
	Py_AddConsti(dict, GL_DEPTH_TEST);
	Py_AddConsti(dict, GL_DEPTH_WRITEMASK);
	Py_AddConsti(dict, GL_DEPTH_CLEAR_VALUE);
	Py_AddConsti(dict, GL_DEPTH_FUNC);
	Py_AddConsti(dict, GL_ACCUM_CLEAR_VALUE);
	Py_AddConsti(dict, GL_STENCIL_TEST);
	Py_AddConsti(dict, GL_STENCIL_CLEAR_VALUE);
	Py_AddConsti(dict, GL_STENCIL_FUNC);
	Py_AddConsti(dict, GL_STENCIL_VALUE_MASK);
	Py_AddConsti(dict, GL_STENCIL_FAIL);
	Py_AddConsti(dict, GL_STENCIL_PASS_DEPTH_FAIL);
	Py_AddConsti(dict, GL_STENCIL_PASS_DEPTH_PASS);
	Py_AddConsti(dict, GL_STENCIL_REF);
	Py_AddConsti(dict, GL_STENCIL_WRITEMASK);
	Py_AddConsti(dict, GL_MATRIX_MODE);
	Py_AddConsti(dict, GL_NORMALIZE);
	Py_AddConsti(dict, GL_VIEWPORT);
	Py_AddConsti(dict, GL_MODELVIEW_STACK_DEPTH);
	Py_AddConsti(dict, GL_PROJECTION_STACK_DEPTH);
	Py_AddConsti(dict, GL_TEXTURE_STACK_DEPTH);
	Py_AddConsti(dict, GL_MODELVIEW_MATRIX);
	Py_AddConsti(dict, GL_PROJECTION_MATRIX);
	Py_AddConsti(dict, GL_TEXTURE_MATRIX);
	Py_AddConsti(dict, GL_ATTRIB_STACK_DEPTH);
	Py_AddConsti(dict, GL_ALPHA_TEST);
	Py_AddConsti(dict, GL_ALPHA_TEST_FUNC);
	Py_AddConsti(dict, GL_ALPHA_TEST_REF);
	Py_AddConsti(dict, GL_DITHER);
	Py_AddConsti(dict, GL_BLEND_DST);
	Py_AddConsti(dict, GL_BLEND_SRC);
	Py_AddConsti(dict, GL_BLEND);
	Py_AddConsti(dict, GL_LOGIC_OP_MODE);
	Py_AddConsti(dict, GL_LOGIC_OP);
	Py_AddConsti(dict, GL_AUX_BUFFERS);
	Py_AddConsti(dict, GL_DRAW_BUFFER);
	Py_AddConsti(dict, GL_READ_BUFFER);
	Py_AddConsti(dict, GL_SCISSOR_BOX);
	Py_AddConsti(dict, GL_SCISSOR_TEST);
	Py_AddConsti(dict, GL_INDEX_CLEAR_VALUE);
	Py_AddConsti(dict, GL_INDEX_WRITEMASK);
	Py_AddConsti(dict, GL_COLOR_CLEAR_VALUE);
	Py_AddConsti(dict, GL_COLOR_WRITEMASK);
	Py_AddConsti(dict, GL_INDEX_MODE);
	Py_AddConsti(dict, GL_RGBA_MODE);
	Py_AddConsti(dict, GL_DOUBLEBUFFER);
	Py_AddConsti(dict, GL_STEREO);
	Py_AddConsti(dict, GL_RENDER_MODE);
	Py_AddConsti(dict, GL_PERSPECTIVE_CORRECTION_HINT);
	Py_AddConsti(dict, GL_POINT_SMOOTH_HINT);
	Py_AddConsti(dict, GL_LINE_SMOOTH_HINT);
	Py_AddConsti(dict, GL_POLYGON_SMOOTH_HINT);
	Py_AddConsti(dict, GL_FOG_HINT);
	Py_AddConsti(dict, GL_TEXTURE_GEN_S);
	Py_AddConsti(dict, GL_TEXTURE_GEN_T);
	Py_AddConsti(dict, GL_TEXTURE_GEN_R);
	Py_AddConsti(dict, GL_TEXTURE_GEN_Q);
	Py_AddConsti(dict, GL_PIXEL_MAP_I_TO_I_SIZE);
	Py_AddConsti(dict, GL_PIXEL_MAP_S_TO_S_SIZE);
	Py_AddConsti(dict, GL_PIXEL_MAP_I_TO_R_SIZE);
	Py_AddConsti(dict, GL_PIXEL_MAP_I_TO_G_SIZE);
	Py_AddConsti(dict, GL_PIXEL_MAP_I_TO_B_SIZE);
	Py_AddConsti(dict, GL_PIXEL_MAP_I_TO_A_SIZE);
	Py_AddConsti(dict, GL_PIXEL_MAP_R_TO_R_SIZE);
	Py_AddConsti(dict, GL_PIXEL_MAP_G_TO_G_SIZE);
	Py_AddConsti(dict, GL_PIXEL_MAP_B_TO_B_SIZE);
	Py_AddConsti(dict, GL_PIXEL_MAP_A_TO_A_SIZE);
	Py_AddConsti(dict, GL_UNPACK_SWAP_BYTES);
	Py_AddConsti(dict, GL_UNPACK_LSB_FIRST);
	Py_AddConsti(dict, GL_UNPACK_ROW_LENGTH);
	Py_AddConsti(dict, GL_UNPACK_SKIP_ROWS);
	Py_AddConsti(dict, GL_UNPACK_SKIP_PIXELS);
	Py_AddConsti(dict, GL_UNPACK_ALIGNMENT);
	Py_AddConsti(dict, GL_PACK_SWAP_BYTES);
	Py_AddConsti(dict, GL_PACK_LSB_FIRST);
	Py_AddConsti(dict, GL_PACK_ROW_LENGTH);
	Py_AddConsti(dict, GL_PACK_SKIP_ROWS);
	Py_AddConsti(dict, GL_PACK_SKIP_PIXELS);
	Py_AddConsti(dict, GL_PACK_ALIGNMENT);
	Py_AddConsti(dict, GL_MAP_COLOR);
	Py_AddConsti(dict, GL_MAP_STENCIL);
	Py_AddConsti(dict, GL_INDEX_SHIFT);
	Py_AddConsti(dict, GL_INDEX_OFFSET);
	Py_AddConsti(dict, GL_RED_SCALE);
	Py_AddConsti(dict, GL_RED_BIAS);
	Py_AddConsti(dict, GL_ZOOM_X);
	Py_AddConsti(dict, GL_ZOOM_Y);
	Py_AddConsti(dict, GL_GREEN_SCALE);
	Py_AddConsti(dict, GL_GREEN_BIAS);
	Py_AddConsti(dict, GL_BLUE_SCALE);
	Py_AddConsti(dict, GL_BLUE_BIAS);
	Py_AddConsti(dict, GL_ALPHA_SCALE);
	Py_AddConsti(dict, GL_ALPHA_BIAS);
	Py_AddConsti(dict, GL_DEPTH_SCALE);
	Py_AddConsti(dict, GL_DEPTH_BIAS);
	Py_AddConsti(dict, GL_MAX_EVAL_ORDER);
	Py_AddConsti(dict, GL_MAX_LIGHTS);
	Py_AddConsti(dict, GL_MAX_CLIP_PLANES);
	Py_AddConsti(dict, GL_MAX_TEXTURE_SIZE);
	Py_AddConsti(dict, GL_MAX_PIXEL_MAP_TABLE);
	Py_AddConsti(dict, GL_MAX_ATTRIB_STACK_DEPTH);
	Py_AddConsti(dict, GL_MAX_MODELVIEW_STACK_DEPTH);
	Py_AddConsti(dict, GL_MAX_NAME_STACK_DEPTH);
	Py_AddConsti(dict, GL_MAX_PROJECTION_STACK_DEPTH);
	Py_AddConsti(dict, GL_MAX_TEXTURE_STACK_DEPTH);
	Py_AddConsti(dict, GL_MAX_VIEWPORT_DIMS);
	Py_AddConsti(dict, GL_SUBPIXEL_BITS);
	Py_AddConsti(dict, GL_INDEX_BITS);
	Py_AddConsti(dict, GL_RED_BITS);
	Py_AddConsti(dict, GL_GREEN_BITS);
	Py_AddConsti(dict, GL_BLUE_BITS);
	Py_AddConsti(dict, GL_ALPHA_BITS);
	Py_AddConsti(dict, GL_DEPTH_BITS);
	Py_AddConsti(dict, GL_STENCIL_BITS);
	Py_AddConsti(dict, GL_ACCUM_RED_BITS);
	Py_AddConsti(dict, GL_ACCUM_GREEN_BITS);
	Py_AddConsti(dict, GL_ACCUM_BLUE_BITS);
	Py_AddConsti(dict, GL_ACCUM_ALPHA_BITS);
	Py_AddConsti(dict, GL_NAME_STACK_DEPTH);
	Py_AddConsti(dict, GL_AUTO_NORMAL);
	Py_AddConsti(dict, GL_MAP1_COLOR_4);
	Py_AddConsti(dict, GL_MAP1_INDEX);
	Py_AddConsti(dict, GL_MAP1_NORMAL);
	Py_AddConsti(dict, GL_MAP1_TEXTURE_COORD_1);
	Py_AddConsti(dict, GL_MAP1_TEXTURE_COORD_2);
	Py_AddConsti(dict, GL_MAP1_TEXTURE_COORD_3);
	Py_AddConsti(dict, GL_MAP1_TEXTURE_COORD_4);
	Py_AddConsti(dict, GL_MAP1_VERTEX_3);
	Py_AddConsti(dict, GL_MAP1_VERTEX_4);
	Py_AddConsti(dict, GL_MAP2_COLOR_4);
	Py_AddConsti(dict, GL_MAP2_INDEX);
	Py_AddConsti(dict, GL_MAP2_NORMAL);
	Py_AddConsti(dict, GL_MAP2_TEXTURE_COORD_1);
	Py_AddConsti(dict, GL_MAP2_TEXTURE_COORD_2);
	Py_AddConsti(dict, GL_MAP2_TEXTURE_COORD_3);
	Py_AddConsti(dict, GL_MAP2_TEXTURE_COORD_4);
	Py_AddConsti(dict, GL_MAP2_VERTEX_3);
	Py_AddConsti(dict, GL_MAP2_VERTEX_4);
	Py_AddConsti(dict, GL_MAP1_GRID_DOMAIN);
	Py_AddConsti(dict, GL_MAP1_GRID_SEGMENTS);
	Py_AddConsti(dict, GL_MAP2_GRID_DOMAIN);
	Py_AddConsti(dict, GL_MAP2_GRID_SEGMENTS);
	Py_AddConsti(dict, GL_TEXTURE_1D);
	Py_AddConsti(dict, GL_TEXTURE_2D);

	Py_AddConsti(dict, GL_TEXTURE_WIDTH);
	Py_AddConsti(dict, GL_TEXTURE_HEIGHT);
	Py_AddConsti(dict, GL_TEXTURE_COMPONENTS);
	Py_AddConsti(dict, GL_TEXTURE_BORDER_COLOR);
	Py_AddConsti(dict, GL_TEXTURE_BORDER);

	Py_AddConsti(dict, GL_DONT_CARE);
	Py_AddConsti(dict, GL_FASTEST);
	Py_AddConsti(dict, GL_NICEST);

	Py_AddConsti(dict, GL_AMBIENT);
	Py_AddConsti(dict, GL_DIFFUSE);
	Py_AddConsti(dict, GL_SPECULAR);
	Py_AddConsti(dict, GL_POSITION);
	Py_AddConsti(dict, GL_SPOT_DIRECTION);
	Py_AddConsti(dict, GL_SPOT_EXPONENT);
	Py_AddConsti(dict, GL_SPOT_CUTOFF);
	Py_AddConsti(dict, GL_CONSTANT_ATTENUATION);
	Py_AddConsti(dict, GL_LINEAR_ATTENUATION);
	Py_AddConsti(dict, GL_QUADRATIC_ATTENUATION);

	Py_AddConsti(dict, GL_COMPILE);
	Py_AddConsti(dict, GL_COMPILE_AND_EXECUTE);

	Py_AddConsti(dict, GL_BYTE);
	Py_AddConsti(dict, GL_UNSIGNED_BYTE);
	Py_AddConsti(dict, GL_SHORT);
	Py_AddConsti(dict, GL_UNSIGNED_SHORT);
	Py_AddConsti(dict, GL_INT);
	Py_AddConsti(dict, GL_UNSIGNED_INT);
	Py_AddConsti(dict, GL_FLOAT);
	Py_AddConsti(dict, GL_2_BYTES);
	Py_AddConsti(dict, GL_3_BYTES);
	Py_AddConsti(dict, GL_4_BYTES);

	Py_AddConsti(dict, GL_CLEAR);
	Py_AddConsti(dict, GL_AND);
	Py_AddConsti(dict, GL_AND_REVERSE);
	Py_AddConsti(dict, GL_COPY);
	Py_AddConsti(dict, GL_AND_INVERTED);
	Py_AddConsti(dict, GL_NOOP);
	Py_AddConsti(dict, GL_XOR);
	Py_AddConsti(dict, GL_OR);
	Py_AddConsti(dict, GL_NOR);
	Py_AddConsti(dict, GL_EQUIV);
	Py_AddConsti(dict, GL_INVERT);
	Py_AddConsti(dict, GL_OR_REVERSE);
	Py_AddConsti(dict, GL_COPY_INVERTED);
	Py_AddConsti(dict, GL_OR_INVERTED);
	Py_AddConsti(dict, GL_NAND);
	Py_AddConsti(dict, GL_SET);

	Py_AddConsti(dict, GL_EMISSION);
	Py_AddConsti(dict, GL_SHININESS);
	Py_AddConsti(dict, GL_AMBIENT_AND_DIFFUSE);
	Py_AddConsti(dict, GL_COLOR_INDEXES);

	Py_AddConsti(dict, GL_MODELVIEW);
	Py_AddConsti(dict, GL_PROJECTION);
	Py_AddConsti(dict, GL_TEXTURE);

	Py_AddConsti(dict, GL_COLOR);
	Py_AddConsti(dict, GL_DEPTH);
	Py_AddConsti(dict, GL_STENCIL);

	Py_AddConsti(dict, GL_COLOR_INDEX);
	Py_AddConsti(dict, GL_STENCIL_INDEX);
	Py_AddConsti(dict, GL_DEPTH_COMPONENT);
	Py_AddConsti(dict, GL_RED);
	Py_AddConsti(dict, GL_GREEN);
	Py_AddConsti(dict, GL_BLUE);
	Py_AddConsti(dict, GL_ALPHA);
	Py_AddConsti(dict, GL_RGB);
	Py_AddConsti(dict, GL_RGBA);
	Py_AddConsti(dict, GL_LUMINANCE);
	Py_AddConsti(dict, GL_LUMINANCE_ALPHA);

	Py_AddConsti(dict, GL_BITMAP);

	Py_AddConsti(dict, GL_POINT);
	Py_AddConsti(dict, GL_LINE);
	Py_AddConsti(dict, GL_FILL);

	Py_AddConsti(dict, GL_RENDER);
	Py_AddConsti(dict, GL_FEEDBACK);
	Py_AddConsti(dict, GL_SELECT);

	Py_AddConsti(dict, GL_FLAT);
	Py_AddConsti(dict, GL_SMOOTH);

	Py_AddConsti(dict, GL_KEEP);
	Py_AddConsti(dict, GL_REPLACE);
	Py_AddConsti(dict, GL_INCR);
	Py_AddConsti(dict, GL_DECR);

	Py_AddConsti(dict, GL_VENDOR);
	Py_AddConsti(dict, GL_RENDERER);
	Py_AddConsti(dict, GL_VERSION);
	Py_AddConsti(dict, GL_EXTENSIONS);

	Py_AddConsti(dict, GL_S);
	Py_AddConsti(dict, GL_T);
	Py_AddConsti(dict, GL_R);
	Py_AddConsti(dict, GL_Q);

	Py_AddConsti(dict, GL_MODULATE);
	Py_AddConsti(dict, GL_DECAL);

	Py_AddConsti(dict, GL_TEXTURE_ENV_MODE);
	Py_AddConsti(dict, GL_TEXTURE_ENV_COLOR);

	Py_AddConsti(dict, GL_TEXTURE_ENV);

	Py_AddConsti(dict, GL_EYE_LINEAR);
	Py_AddConsti(dict, GL_OBJECT_LINEAR);
	Py_AddConsti(dict, GL_SPHERE_MAP);

	Py_AddConsti(dict, GL_TEXTURE_GEN_MODE);
	Py_AddConsti(dict, GL_OBJECT_PLANE);
	Py_AddConsti(dict, GL_EYE_PLANE);

	Py_AddConsti(dict, GL_NEAREST);
	Py_AddConsti(dict, GL_LINEAR);

	Py_AddConsti(dict, GL_NEAREST_MIPMAP_NEAREST);
	Py_AddConsti(dict, GL_LINEAR_MIPMAP_NEAREST);
	Py_AddConsti(dict, GL_NEAREST_MIPMAP_LINEAR);
	Py_AddConsti(dict, GL_LINEAR_MIPMAP_LINEAR);

	Py_AddConsti(dict, GL_TEXTURE_MAG_FILTER);
	Py_AddConsti(dict, GL_TEXTURE_MIN_FILTER);
	Py_AddConsti(dict, GL_TEXTURE_WRAP_S);
	Py_AddConsti(dict, GL_TEXTURE_WRAP_T);

	Py_AddConsti(dict, GL_CLAMP);
	Py_AddConsti(dict, GL_REPEAT);

	Py_AddConsti(dict, GL_CLIP_PLANE0);
	Py_AddConsti(dict, GL_CLIP_PLANE1);
	Py_AddConsti(dict, GL_CLIP_PLANE2);
	Py_AddConsti(dict, GL_CLIP_PLANE3);
	Py_AddConsti(dict, GL_CLIP_PLANE4);
	Py_AddConsti(dict, GL_CLIP_PLANE5);

	Py_AddConsti(dict, GL_LIGHT0);
	Py_AddConsti(dict, GL_LIGHT1);
	Py_AddConsti(dict, GL_LIGHT2);
	Py_AddConsti(dict, GL_LIGHT3);
	Py_AddConsti(dict, GL_LIGHT4);
	Py_AddConsti(dict, GL_LIGHT5);
	Py_AddConsti(dict, GL_LIGHT6);
	Py_AddConsti(dict, GL_LIGHT7);
	
	PyModule_GetDict(mod);
		
	return mod;
}

#endif /* IRISGL */

#endif /* !(WITHOUT_PYTHON) */

