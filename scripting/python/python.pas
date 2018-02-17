(* FPC interface to the Python language (see python.txt
 * for more infos on Python itself). All credits go to
 * the P4D project)
 *
 * P4D <https://github.com/pyscripter/python4delphi>)
 *)

unit Python;

{$mode Delphi}

{ TODO -oMMM : implement tp_as_buffer slot }
{ TODO -oMMM : implement Attribute descriptor and subclassing stuff }

interface

uses
  SysUtils;

type
  TPythonVersionProp = packed record
    DllName: String;
    RegVersion: String;
    APIVersion: Integer;
    CanUseLatest: Boolean;
  end;

const
  PYT_METHOD_BUFFER_INCREASE = 10;
  PYT_MEMBER_BUFFER_INCREASE = 10;
  PYT_GETSET_BUFFER_INCREASE = 10;

  METH_VARARGS = $0001;
  METH_KEYWORDS = $0002;

  // Masks for the co_flags field of PyCodeObject
  CO_OPTIMIZED = $0001;
  CO_NEWLOCALS = $0002;
  CO_VARARGS = $0004;
  CO_VARKEYWORDS = $0008;

  // Rich comparison opcodes introduced in version 2.1
  Py_LT = 0;
  Py_LE = 1;
  Py_EQ = 2;
  Py_NE = 3;
  Py_GT = 4;
  Py_GE = 5;

type
  // Delphi equivalent used by TPyObject
  TRichComparisonOpcode = (pyLT, pyLE, pyEQ, pyNE, pyGT, pyGE);

const
{Type flags (tp_flags) introduced in version 2.0

These flags are used to extend the type structure in a backwards-compatible
fashion. Extensions can use the flags to indicate (and test) when a given
type structure contains a new feature. The Python core will use these when
introducing new functionality between major revisions (to avoid mid-version
changes in the PYTHON_API_VERSION).

Arbitration of the flag bit positions will need to be coordinated among
all extension writers who publically release their extensions (this will
be fewer than you might expect!)..

Python 1.5.2 introduced the bf_getcharbuffer slot into PyBufferProcs.

Type definitions should use Py_TPFLAGS_DEFAULT for their tp_flags value.

Code can use PyType_HasFeature(type_ob, flag_value) to test whether the
given type object has a specified feature.
}

  // PyBufferProcs contains bf_getcharbuffer
  Py_TPFLAGS_HAVE_GETCHARBUFFER = (1 shl 0);

  // PySequenceMethods contains sq_contains
  Py_TPFLAGS_HAVE_SEQUENCE_IN = (1 shl 1);

  // Objects which participate in garbage collection (see objimp.h)
  Py_TPFLAGS_GC = (1 shl 2);

  // PySequenceMethods and PyNumberMethods contain in-place operators
  Py_TPFLAGS_HAVE_INPLACEOPS = (1 shl 3);

  // PyNumberMethods do their own coercion */
  Py_TPFLAGS_CHECKTYPES = (1 shl 4);

  Py_TPFLAGS_HAVE_RICHCOMPARE = (1 shl 5);

  // Objects which are weakly referencable if their tp_weaklistoffset is >0
  // XXX Should this have the same value as Py_TPFLAGS_HAVE_RICHCOMPARE?
  // These both indicate a feature that appeared in the same alpha release.

  Py_TPFLAGS_HAVE_WEAKREFS = (1 shl 6);

  // tp_iter is defined
  Py_TPFLAGS_HAVE_ITER = (1 shl 7);

  // New members introduced by Python 2.2 exist
  Py_TPFLAGS_HAVE_CLASS = (1 shl 8);

  // Set if the type object is dynamically allocated
  Py_TPFLAGS_HEAPTYPE = (1 shl 9);

  // Set if the type allows subclassing
  Py_TPFLAGS_BASETYPE = (1 shl 10);

  // Set if the type is 'ready' -- fully initialized
  Py_TPFLAGS_READY = (1 shl 12);

  // Set while the type is being 'readied', to prevent recursive ready calls
  Py_TPFLAGS_READYING = (1 shl 13);

  // Objects support garbage collection (see objimp.h)
  Py_TPFLAGS_HAVE_GC = (1 shl 14);

  Py_TPFLAGS_DEFAULT = Py_TPFLAGS_HAVE_GETCHARBUFFER or Py_TPFLAGS_HAVE_SEQUENCE_IN or Py_TPFLAGS_HAVE_INPLACEOPS or Py_TPFLAGS_HAVE_RICHCOMPARE or Py_TPFLAGS_HAVE_WEAKREFS or Py_TPFLAGS_HAVE_ITER or Py_TPFLAGS_HAVE_CLASS or Py_TPFLAGS_BASETYPE;

// See function PyType_HasFeature below for testing the flags.

// Delphi equivalent used by TPythonType
type
  TPFlag = (tpfHaveGetCharBuffer, tpfHaveSequenceIn, tpfGC, tpfHaveInplaceOps,
    tpfCheckTypes, tpfHaveRichCompare, tpfHaveWeakRefs
    , tpfHaveIter, tpfHaveClass, tpfHeapType, tpfBaseType, tpfReady, tpfReadying, tpfHaveGC
    );
  TPFlags = set of TPFlag;

const
  TPFLAGS_DEFAULT = [tpfHaveGetCharBuffer, tpfHaveSequenceIn, tpfHaveInplaceOps,
    tpfHaveRichCompare, tpfHaveWeakRefs, tpfHaveIter,
    tpfHaveClass, tpfBaseType
    ];
//-------  Python opcodes  ----------//
Const
  single_input = 256;
  file_input = 257;
  eval_input = 258;
  p4d_funcdef = 259;
  p4d_parameters = 260;
  p4d_varargslist = 261;
  p4d_fpdef = 262;
  p4d_fplist = 263;
  p4d_stmt = 264;
  p4d_simple_stmt = 265;
  p4d_small_stmt = 266;
  p4d_expr_stmt = 267;
  p4d_augassign = 268;
  p4d_print_stmt = 269;
  p4d_del_stmt = 270;
  p4d_pass_stmt = 271;
  p4d_flow_stmt = 272;
  p4d_break_stmt = 273;
  p4d_continue_stmt = 274;
  p4d_return_stmt = 275;
  p4d_raise_stmt = 276;
  p4d_import_stmt = 277;
  p4d_import_as_name = 278;
  p4d_dotted_as_name = 279;
  p4d_dotted_name = 280;
  p4d_global_stmt = 281;
  p4d_exec_stmt = 282;
  p4d_assert_stmt = 283;
  p4d_compound_stmt = 284;
  p4d_if_stmt = 285;
  p4d_while_stmt = 286;
  p4d_for_stmt = 287;
  p4d_try_stmt = 288;
  p4d_except_clause = 289;
  p4d_suite = 290;
  p4d_test = 291;
  p4d_and_test = 291;
  p4d_not_test = 293;
  p4d_comparison = 294;
  p4d_comp_op = 295;
  p4d_expr = 296;
  p4d_xor_expr = 297;
  p4d_and_expr = 298;
  p4d_shift_expr = 299;
  p4d_arith_expr = 300;
  p4d_term = 301;
  p4d_factor = 302;
  p4d_power = 303;
  p4d_atom = 304;
  p4d_listmaker = 305;
  p4d_lambdef = 306;
  p4d_trailer = 307;
  p4d_subscriptlist = 308;
  p4d_subscript = 309;
  p4d_sliceop = 310;
  p4d_exprlist = 311;
  p4d_testlist = 312;
  p4d_dictmaker = 313;
  p4d_classdef = 314;
  p4d_arglist = 315;
  p4d_argument = 316;
  p4d_list_iter = 317;
  p4d_list_for = 318;
  p4d_list_if = 319;

// structmember.h
const
  //* Types */
  T_SHORT = 0;
  T_INT = 1;
  T_LONG = 2;
  T_FLOAT = 3;
  T_DOUBLE = 4;
  T_STRING = 5;
  T_OBJECT = 6;
  //* XXX the ordering here is weird for binary compatibility */
  T_CHAR = 7;  //* 1-character string */
  T_BYTE = 8;  //* 8-bit signed int */
  //* unsigned variants: */
  T_UBYTE = 9;
  T_USHORT = 10;
  T_UINT = 11;
  T_ULONG = 12;

  //* Added by Jack: strings contained in the structure */
  T_STRING_INPLACE = 13;

  T_OBJECT_EX = 16;{* Like T_OBJECT, but raises AttributeError
                                        when the value is NULL, instead of
                                        converting to None. *}

  //* Flags */
  READONLY = 1;
  RO = READONLY;    //* Shorthand */
  READ_RESTRICTED = 2;
  WRITE_RESTRICTED = 4;
  RESTRICTED = (READ_RESTRICTED or WRITE_RESTRICTED);

type
  TPyMemberType = (mtShort, mtInt, mtLong, mtFloat, mtDouble, mtString, mtObject,
    mtChar, mtByte, mtUByte, mtUShort, mtUInt, mtULong,
    mtStringInplace, mtObjectEx);
  TPyMemberFlag = (mfDefault, mfReadOnly, mfReadRestricted, mfWriteRestricted, mfRestricted);

//#######################################################
//##                                                   ##
//##           Non-Python specific constants           ##
//##                                                   ##
//#######################################################

const
  ErrInit = -300;
  CR = #13;
  LF = #10;
  TAB = #09;
  CRLF = CR + LF;



//#######################################################
//##                                                   ##
//##    Global declarations, nothing Python specific   ##
//##                                                   ##
//#######################################################

type
  TPAnsiChar = array[0..16000] of PAnsiChar;
  TPWideChar = array[0..16000] of PWideChar;
  PPAnsiChar = ^TPAnsiChar;
  PPWideChar = ^TPWideChar;
  PInt = ^Integer;
  PDouble = ^Double;
  PFloat = ^Real;
  PLong = ^LongInt;
  PShort = ^ShortInt;


//#######################################################
//##                                                   ##
//##            Python specific interface              ##
//##                                                   ##
//#######################################################

type
  PP_frozen = ^P_frozen;
  P_frozen = ^_frozen;
  PPyObject = ^PyObject;
  PPPyObject = ^PPyObject;
  PPPPyObject = ^PPPyObject;
  PPyIntObject = ^PyIntObject;
  PPyTypeObject = ^PyTypeObject;
  PPySliceObject = ^PySliceObject;

  AtExitProc = procedure;
  init_func = function(): pPyObject; cdecl;
  PyCFunction = function(self, args: PPyObject): PPyObject; cdecl;
  PyCFunctionWithKW = function(self, args, keywords: PPyObject): PPyObject; cdecl;

  unaryfunc = function(ob1: PPyObject): PPyObject; cdecl;
  binaryfunc = function(ob1, ob2: PPyObject): PPyObject; cdecl;
  ternaryfunc = function(ob1, ob2, ob3: PPyObject): PPyObject; cdecl;
  inquiry = function(ob1: PPyObject): integer; cdecl;
  lenfunc = function(ob1: PPyObject): NativeInt; cdecl;
  coercion = function(ob1, ob2: PPPyObject): integer; cdecl;
  ssizeargfunc = function(ob1: PPyObject; i: NativeInt): PPyObject; cdecl;
  ssizessizeargfunc = function(ob1: PPyObject; i1, i2: NativeInt): PPyObject; cdecl;
  ssizeobjargproc = function(ob1: PPyObject; i: NativeInt; ob2: PPyObject): integer; cdecl;
  ssizessizeobjargproc = function(ob1: PPyObject; i1, i2: NativeInt; ob2: PPyObject): integer; cdecl;
  objobjargproc = function(ob1, ob2, ob3: PPyObject): integer; cdecl;

  pydestructor = procedure(ob: PPyObject); cdecl;
  printfunc = function(ob: PPyObject; var f: file; i: integer): integer; cdecl;
  getattrfunc = function(ob1: PPyObject; name: PAnsiChar): PPyObject; cdecl;
  setattrfunc = function(ob1: PPyObject; name: PAnsiChar; ob2: PPyObject): integer; cdecl;
  cmpfunc = function(ob1, ob2: PPyObject): integer; cdecl;
  reprfunc = function(ob: PPyObject): PPyObject; cdecl;
  hashfunc = function(ob: PPyObject): NativeInt; cdecl; // !! in 2.x it is still a LongInt
  getattrofunc = function(ob1, ob2: PPyObject): PPyObject; cdecl;
  setattrofunc = function(ob1, ob2, ob3: PPyObject): integer; cdecl;

  /// jah 29-sep-2000 : updated for python 2.0
  ///                   added from object.h
  getreadbufferproc = function(ob1: PPyObject; i: NativeInt; ptr: Pointer): NativeInt; cdecl;
  getwritebufferproc = function(ob1: PPyObject; i: NativeInt; ptr: Pointer): NativeInt; cdecl;
  getsegcountproc = function(ob1: PPyObject; i: NativeInt): NativeInt; cdecl;
  getcharbufferproc = function(ob1: PPyObject; i: NativeInt; const pstr: PAnsiChar): NativeInt; cdecl;
  objobjproc = function(ob1, ob2: PPyObject): integer; cdecl;
  visitproc = function(ob1: PPyObject; ptr: Pointer): integer; cdecl;
  traverseproc = function(ob1: PPyObject; proc: visitproc; ptr: Pointer): integer; cdecl;

  richcmpfunc = function(ob1, ob2: PPyObject; i: Integer): PPyObject; cdecl;
  getiterfunc = function(ob1: PPyObject): PPyObject; cdecl;
  iternextfunc = function(ob1: PPyObject): PPyObject; cdecl;
  descrgetfunc = function(ob1, ob2, ob3: PPyObject): PPyObject; cdecl;
  descrsetfunc = function(ob1, ob2, ob3: PPyObject): Integer; cdecl;
  initproc = function(self, args, kwds: PPyObject): Integer; cdecl;
  newfunc = function(subtype: PPyTypeObject; args, kwds: PPyObject): PPyObject; cdecl;
  allocfunc = function(self: PPyTypeObject; nitems: NativeInt): PPyObject; cdecl;

  PyNumberMethods = {$IFNDEF CPUX64}packed{$ENDIF} record
    nb_add: binaryfunc;
    nb_substract: binaryfunc;
    nb_multiply: binaryfunc;
    nb_divide: binaryfunc;
    nb_remainder: binaryfunc;
    nb_divmod: binaryfunc;
    nb_power: ternaryfunc;
    nb_negative: unaryfunc;
    nb_positive: unaryfunc;
    nb_absolute: unaryfunc;
    nb_nonzero: inquiry;
    nb_invert: unaryfunc;
    nb_lshift: binaryfunc;
    nb_rshift: binaryfunc;
    nb_and: binaryfunc;
    nb_xor: binaryfunc;
    nb_or: binaryfunc;
    nb_coerce: coercion;
    nb_int: unaryfunc;
    nb_long: unaryfunc;
    nb_float: unaryfunc;
    nb_oct: unaryfunc;
    nb_hex: unaryfunc;

    /// jah 29-sep-2000 : updated for python 2.0
    ///                   added from .h
    nb_inplace_add: binaryfunc;
    nb_inplace_subtract: binaryfunc;
    nb_inplace_multiply: binaryfunc;
    nb_inplace_divide: binaryfunc;
    nb_inplace_remainder: binaryfunc;
    nb_inplace_power: ternaryfunc;
    nb_inplace_lshift: binaryfunc;
    nb_inplace_rshift: binaryfunc;
    nb_inplace_and: binaryfunc;
    nb_inplace_xor: binaryfunc;
    nb_inplace_or: binaryfunc;

    // Added in release 2.2
    // The following require the Py_TPFLAGS_HAVE_CLASS flag
    nb_floor_divide: binaryfunc;
    nb_true_divide: binaryfunc;
    nb_inplace_floor_divide: binaryfunc;
    nb_inplace_true_divide: binaryfunc;
  end;
  PPyNumberMethods = ^PyNumberMethods;

  PySequenceMethods = {$IFNDEF CPUX64}packed{$ENDIF} record
    sq_length: lenfunc;
    sq_concat: binaryfunc;
    sq_repeat: ssizeargfunc;
    sq_item: ssizeargfunc;
    sq_slice: ssizessizeargfunc;
    sq_ass_item: ssizeobjargproc;
    sq_ass_slice: ssizessizeobjargproc;

    /// jah 29-sep-2000 : updated for python 2.0
    ///                   added from .h
    sq_contains: objobjproc;
    sq_inplace_concat: binaryfunc;
    sq_inplace_repeat: ssizeargfunc;
  end;
  PPySequenceMethods = ^PySequenceMethods;

  PyMappingMethods = {$IFNDEF CPUX64}packed{$ENDIF} record
    mp_length: lenfunc;
    mp_subscript: binaryfunc;
    mp_ass_subscript: objobjargproc;
  end;
  PPyMappingMethods = ^PyMappingMethods;

  /// jah 29-sep-2000 : updated for python 2.0
  ///                   added from .h
  PyBufferProcs = {$IFNDEF CPUX64}packed{$ENDIF} record
    bf_getreadbuffer: getreadbufferproc;
    bf_getwritebuffer: getwritebufferproc;
    bf_getsegcount: getsegcountproc;
    bf_getcharbuffer: getcharbufferproc;
  end;
  PPyBufferProcs = ^PyBufferProcs;

  Py_complex =  {$IFNDEF CPUX64}packed{$ENDIF} record
    real: double;
    imag: double;
  end;

  PyObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
  end;

  PyIntObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    ob_ival: LongInt;
  end;

  _frozen = {$IFNDEF CPUX64}packed{$ENDIF} record
    name: PAnsiChar;
    code: PByte;
    size: Integer;
  end;

  PySliceObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    start, stop, step: PPyObject;
  end;

  PPyMethodDef = ^PyMethodDef;

  PyMethodDef = {$IFNDEF CPUX64}packed{$ENDIF} record
    ml_name: PAnsiChar;
    ml_meth: PyCFunction;
    ml_flags: Integer;
    ml_doc: PAnsiChar;
  end;

  // structmember.h
  PPyMemberDef = ^PyMemberDef;

  PyMemberDef = {$IFNDEF CPUX64}packed{$ENDIF} record
    name: PAnsiChar;
    _type: integer;
    offset: NativeInt;
    flags: integer;
    doc: PAnsiChar;
  end;

  // descrobject.h

  // Descriptors

  getter = function(obj: PPyObject; context: Pointer): PPyObject; cdecl;
  setter = function(obj, value: PPyObject; context: Pointer): integer; cdecl;

  PPyGetSetDef = ^PyGetSetDef;

  PyGetSetDef = {$IFNDEF CPUX64}packed{$ENDIF} record
    name: PAnsiChar;
    get: getter;
    _set: setter;
    doc: PAnsiChar;
    closure: Pointer;
  end;

  wrapperfunc = function(self, args: PPyObject; wrapped: Pointer): PPyObject; cdecl;

  pwrapperbase = ^wrapperbase;

  wrapperbase = {$IFNDEF CPUX64}packed{$ENDIF} record
    name: PAnsiChar;
    wrapper: wrapperfunc;
    doc: PAnsiChar;
  end;

  // Various kinds of descriptor objects

  {#define PyDescr_COMMON \
          PyObject_HEAD \
          PyTypeObject *d_type; \
          PyObject *d_name
  }

  PPyDescrObject = ^PyDescrObject;

  PyDescrObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    d_type: PPyTypeObject;
    d_name: PPyObject;
  end;

  PPyMethodDescrObject = ^PyMethodDescrObject;

  PyMethodDescrObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of PyDescr_COMMON
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    d_type: PPyTypeObject;
    d_name: PPyObject;
    // End of PyDescr_COMMON
    d_method: PPyMethodDef;
  end;

  PPyMemberDescrObject = ^PyMemberDescrObject;

  PyMemberDescrObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of PyDescr_COMMON
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    d_type: PPyTypeObject;
    d_name: PPyObject;
    // End of PyDescr_COMMON
    d_member: PPyMemberDef;
  end;

  PPyGetSetDescrObject = ^PyGetSetDescrObject;

  PyGetSetDescrObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of PyDescr_COMMON
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    d_type: PPyTypeObject;
    d_name: PPyObject;
    // End of PyDescr_COMMON
    d_getset: PPyGetSetDef;
  end;

  PPyWrapperDescrObject = ^PyWrapperDescrObject;

  PyWrapperDescrObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of PyDescr_COMMON
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    d_type: PPyTypeObject;
    d_name: PPyObject;
    // End of PyDescr_COMMON
    d_base: pwrapperbase;
    d_wrapped: Pointer; // This can be any function pointer
  end;

  PPyModuleDef_Base = ^PyModuleDef_Base;

  PyModuleDef_Base = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    m_init: function(): PPyObject; cdecl;
    m_index: NativeInt;
    m_copy: PPyObject;
  end;

  PPyModuleDef = ^PyModuleDef;

  PyModuleDef = {$IFNDEF CPUX64}packed{$ENDIF} record
    m_base: PyModuleDef_Base;
    m_name: PAnsiChar;
    m_doc: PAnsiChar;
    m_size: NativeInt;
    m_methods: PPyMethodDef;
    m_reload: inquiry;
    m_traverse: traverseproc;
    m_clear: inquiry;
    m_free: inquiry;
  end;


  // object.h
  PyTypeObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    ob_size: NativeInt; // Number of items in variable part
    tp_name: PAnsiChar;   // For printing
    tp_basicsize, tp_itemsize: NativeInt; // For allocation

    // Methods to implement standard operations

    tp_dealloc: pydestructor;
    tp_print: printfunc;
    tp_getattr: getattrfunc;
    tp_setattr: setattrfunc;
    tp_compare: cmpfunc;
    tp_repr: reprfunc;

    // Method suites for standard classes

    tp_as_number: PPyNumberMethods;
    tp_as_sequence: PPySequenceMethods;
    tp_as_mapping: PPyMappingMethods;

    // More standard operations (here for binary compatibility)

    tp_hash: hashfunc;
    tp_call: ternaryfunc;
    tp_str: reprfunc;
    tp_getattro: getattrofunc;
    tp_setattro: setattrofunc;

    /// jah 29-sep-2000 : updated for python 2.0

    // Functions to access object as input/output buffer
    tp_as_buffer: PPyBufferProcs;
    // Flags to define presence of optional/expanded features
    tp_flags: LongInt;

    tp_doc: PAnsiChar; // Documentation string

    // call function for all accessible objects
    tp_traverse: traverseproc;

    // delete references to contained objects
    tp_clear: inquiry;

    // rich comparisons
    tp_richcompare: richcmpfunc;

    // weak reference enabler
    tp_weaklistoffset: NativeInt;
    // Iterators
    tp_iter: getiterfunc;
    tp_iternext: iternextfunc;

    // Attribute descriptor and subclassing stuff
    tp_methods: PPyMethodDef;
    tp_members: PPyMemberDef;
    tp_getset: PPyGetSetDef;
    tp_base: PPyTypeObject;
    tp_dict: PPyObject;
    tp_descr_get: descrgetfunc;
    tp_descr_set: descrsetfunc;
    tp_dictoffset: NativeInt;
    tp_init: initproc;
    tp_alloc: allocfunc;
    tp_new: newfunc;
    tp_free: pydestructor; // Low-level free-memory routine
    tp_is_gc: inquiry; // For PyObject_IS_GC
    tp_bases: PPyObject;
    tp_mro: PPyObject; // method resolution order
    tp_cache: PPyObject;
    tp_subclasses: PPyObject;
    tp_weaklist: PPyObject;
    tp_del: PyDestructor;
    tp_version_tag: NativeUInt;  // Type attribute cache version tag. Added in version 2.6
    tp_finalize: PyDestructor;
    //More spares
    tp_xxx1: NativeInt;
    tp_xxx2: NativeInt;
    tp_xxx3: NativeInt;
    tp_xxx4: NativeInt;
    tp_xxx5: NativeInt;
    tp_xxx6: NativeInt;
    tp_xxx7: NativeInt;
    tp_xxx8: NativeInt;
    tp_xxx9: NativeInt;
    tp_xxx10: NativeInt;
  end;

  PPyMethodChain = ^PyMethodChain;

  PyMethodChain = {$IFNDEF CPUX64}packed{$ENDIF} record
    methods: PPyMethodDef;
    link: PPyMethodChain;
  end;

  PPyClassObject = ^PyClassObject;

  PyClassObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    cl_bases: PPyObject;       // A tuple of class objects
    cl_dict: PPyObject;       // A dictionary
    cl_name: PPyObject;       // A string
    // The following three are functions or NULL
    cl_getattr: PPyObject;
    cl_setattr: PPyObject;
    cl_delattr: PPyObject;
  end;

  PPyInstanceObject = ^PyInstanceObject;

  PyInstanceObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    in_class: PPyClassObject;      // The class object
    in_dict: PPyObject;           // A dictionary
  end;

{ Instance method objects are used for two purposes:
   (a) as bound instance methods (returned by instancename.methodname)
   (b) as unbound methods (returned by ClassName.methodname)
   In case (b), im_self is NULL
}

  PPyMethodObject = ^PyMethodObject;

  PyMethodObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    im_func: PPyObject;      // The function implementing the method
    im_self: PPyObject;      // The instance it is bound to, or NULL
    im_class: PPyObject;      // The class that defined the method
  end;


  // Bytecode object, compile.h
  PPyCodeObject = ^PyCodeObject;

  PyCodeObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    co_argcount: Integer;         // #arguments, except *args
    co_nlocals: Integer;         // #local variables
    co_stacksize: Integer;          // #entries needed for evaluation stack
    co_flags: Integer;         // CO_..., see below
    co_code: PPyObject;       // instruction opcodes (it hides a PyStringObject)
    co_consts: PPyObject;       // list (constants used)
    co_names: PPyObject;       // list of strings (names used)
    co_varnames: PPyObject;       // tuple of strings (local variable names)
    co_freevars: PPyObject;        // tuple of strings (free variable names)
    co_cellvars: PPyObject;       // tuple of strings (cell variable names)
    // The rest doesn't count for hash/cmp
    co_filename: PPyObject;       // string (where it was loaded from)
    co_name: PPyObject;       // string (name, for reference)
    co_firstlineno: Integer;         // first source line number
    co_lnotab: PPyObject;       // string (encoding addr<->lineno mapping)
  end;


  // from pystate.h
  PPyInterpreterState = ^PyInterpreterState;
  PPyThreadState = ^PyThreadState;
  PPyFrameObject = ^PyFrameObject;

  // Interpreter environments
  PyInterpreterState = {$IFNDEF CPUX64}packed{$ENDIF} record
    next: PPyInterpreterState;
    tstate_head: PPyThreadState;

    modules: PPyObject;
    sysdict: PPyObject;
    builtins: PPyObject;

    checkinterval: integer;
  end;

  // Thread specific information
  PyThreadState = {$IFNDEF CPUX64}packed{$ENDIF} record
    next: PPyThreadState;
    interp: PPyInterpreterState;

    frame: PPyFrameObject;
    recursion_depth: integer;
    ticker: integer;
    tracing: integer;

    sys_profilefn: Pointer;           // c-functions for profile/trace
    sys_tracefn: Pointer;
    sys_profilefunc: PPyObject;
    sys_tracefunc: PPyObject;

    curexc_type: PPyObject;
    curexc_value: PPyObject;
    curexc_traceback: PPyObject;

    exc_type: PPyObject;
    exc_value: PPyObject;
    exc_traceback: PPyObject;

    dict: PPyObject;
    tick_counter: Integer;
    gilstate_counter: Integer;

    async_exc: PPyObject; { Asynchronous exception to raise }
    thread_id: LongInt;   { Thread id where this tstate was created }

    { XXX signal handlers should also be here }
  end;

  // from frameobject.h

  PPyTryBlock = ^PyTryBlock;

  PyTryBlock = {$IFNDEF CPUX64}packed{$ENDIF} record
    b_type: Integer;       // what kind of block this is
    b_handler: Integer;       // where to jump to find handler
    b_level: Integer;       // value stack level to pop to
  end;

  CO_MAXBLOCKS = 0..19;

  PyFrameObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of the VAR_HEAD of an object.
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    ob_size: NativeInt;           // Number of items in variable part
    // End of the Head of an object
    f_back: PPyFrameObject;    // previous frame, or NULL
    f_code: PPyCodeObject;     // code segment
    f_builtins: PPyObject;         // builtin symbol table (PyDictObject)
    f_globals: PPyObject;         // global symbol table (PyDictObject)
    f_locals: PPyObject;         // local symbol table (PyDictObject)
    f_valuestack: PPPyObject;        // points after the last local
    (* Next free slot in f_valuestack.  Frame creation sets to f_valuestack.
       Frame evaluation usually NULLs it, but a frame that yields sets it
       to the current stack top. *)
    f_stacktop: PPPyObject;
    f_trace: PPyObject;         // Trace function
    f_exc_type, f_exc_value, f_exc_traceback: PPyObject;
    f_tstate: PPyThreadState;
    f_lasti: Integer;           // Last instruction if called
    f_lineno: Integer;           // Current line number
    f_iblock: Integer;           // index in f_blockstack
    f_blockstack: array[CO_MAXBLOCKS] of PyTryBlock; // for try and loop blocks
    f_localsplus: array[0..0] of PPyObject; // locals+stack, dynamically sized
  end;

  // From traceback.c
  PPyTraceBackObject = ^PyTraceBackObject;

  PyTraceBackObject = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    tb_next: PPyTraceBackObject;
    tb_frame: PPyFrameObject;
    tb_lasti: Integer;
    tb_lineno: Integer;
  end;

  // Parse tree node interface

  PNode = ^node;

  node = {$IFNDEF CPUX64}packed{$ENDIF} record
    n_type: smallint;
    n_str: PAnsiChar;
    n_lineno: integer;
    n_col_offset: integer;
    n_nchildren: integer;
    n_child: PNode;
  end;

  // From weakrefobject.h

  PPyWeakReference = ^PyWeakReference;

  PyWeakReference = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    wr_object: PPyObject;
    wr_callback: PPyObject;
    hash: NativeInt;
    wr_prev: PPyWeakReference;
    wr_next: PPyWeakReference;
  end;

// from datetime.h


{* Fields are packed into successive bytes, each viewed as unsigned and
 * big-endian, unless otherwise noted:
 *
 * byte offset
 *  0     year     2 bytes, 1-9999
 *  2      month    1 byte,  1-12
 *  3     day      1 byte,  1-31
 *  4     hour     1 byte,  0-23
 *  5     minute   1 byte,  0-59
 *  6     second   1 byte,  0-59
 *  7     usecond  3 bytes, 0-999999
 * 10
 *}

const
  { # of bytes for year, month, and day. }
  _PyDateTime_DATE_DATASIZE = 4;

  { # of bytes for hour, minute, second, and usecond. }
  _PyDateTime_TIME_DATASIZE = 6;

  { # of bytes for year, month, day, hour, minute, second, and usecond. }
  _PyDateTime_DATETIME_DATASIZE = 10;

type
  PyDateTime_Delta = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    hashcode: Integer;  // -1 when unknown
    days: Integer;  // -MAX_DELTA_DAYS <= days <= MAX_DELTA_DAYS
    seconds: Integer;  // 0 <= seconds < 24*3600 is invariant
    microseconds: Integer;  // 0 <= microseconds < 1000000 is invariant
  end;
  PPyDateTime_Delta = ^PyDateTime_Delta;

  PyDateTime_TZInfo = {$IFNDEF CPUX64}packed{$ENDIF} record // a pure abstract base clase
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
  end;
  PPyDateTime_TZInfo = ^PyDateTime_TZInfo;

{
/* The datetime and time types have hashcodes, and an optional tzinfo member,
 * present if and only if hastzinfo is true.
 */
#define _PyTZINFO_HEAD    \
  PyObject_HEAD    \
  long hashcode;    \
  char hastzinfo;    /* boolean flag */
}

{* No _PyDateTime_BaseTZInfo is allocated; it's just to have something
 * convenient to cast to, when getting at the hastzinfo member of objects
 * starting with _PyTZINFO_HEAD.
 *}
  _PyDateTime_BaseTZInfo = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of _PyTZINFO_HEAD
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    hashcode: Integer;
    hastzinfo: Char;  // boolean flag
    // End of _PyTZINFO_HEAD
  end;
  _PPyDateTime_BaseTZInfo = ^_PyDateTime_BaseTZInfo;

{* All time objects are of PyDateTime_TimeType, but that can be allocated
 * in two ways, with or without a tzinfo member.  Without is the same as
 * tzinfo == None, but consumes less memory.  _PyDateTime_BaseTime is an
 * internal struct used to allocate the right amount of space for the
 * "without" case.
 *}
{#define _PyDateTime_TIMEHEAD  \
  _PyTZINFO_HEAD    \
  unsigned char data[_PyDateTime_TIME_DATASIZE];
}

  _PyDateTime_BaseTime = {$IFNDEF CPUX64}packed{$ENDIF} record // hastzinfo false
    // Start of _PyDateTime_TIMEHEAD
    // Start of _PyTZINFO_HEAD
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    hashcode: Integer;
    hastzinfo: Char;  // boolean flag
    // End of _PyTZINFO_HEAD
    data: array[0..Pred(_PyDateTime_TIME_DATASIZE)] of Byte;
    // End of _PyDateTime_TIMEHEAD
  end;
  _PPyDateTime_BaseTime = ^_PyDateTime_BaseTime;

  PyDateTime_Time = {$IFNDEF CPUX64}packed{$ENDIF} record // hastzinfo true
    // Start of _PyDateTime_TIMEHEAD
    // Start of _PyTZINFO_HEAD
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    hashcode: Integer;
    hastzinfo: Char;  // boolean flag
    // End of _PyTZINFO_HEAD
    data: array[0..Pred(_PyDateTime_TIME_DATASIZE)] of Byte;
    // End of _PyDateTime_TIMEHEAD
    tzinfo: PPyObject;
  end;
  PPyDateTime_Time = ^PyDateTime_Time;



{* All datetime objects are of PyDateTime_DateTimeType, but that can be
 * allocated in two ways too, just like for time objects above.  In addition,
 * the plain date type is a base class for datetime, so it must also have
 * a hastzinfo member (although it's unused there).
 *}
  PyDateTime_Date = {$IFNDEF CPUX64}packed{$ENDIF} record
    // Start of _PyTZINFO_HEAD
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    hashcode: Integer;
    hastzinfo: Char;  // boolean flag
    // End of _PyTZINFO_HEAD
    data: array[0..Pred(_PyDateTime_DATE_DATASIZE)] of Byte;
  end;
  PPyDateTime_Date = ^PyDateTime_Date;

 {
#define _PyDateTime_DATETIMEHEAD  \
  _PyTZINFO_HEAD      \
  unsigned char data[_PyDateTime_DATETIME_DATASIZE];
}

  _PyDateTime_BaseDateTime = {$IFNDEF CPUX64}packed{$ENDIF} record // hastzinfo false
    // Start of _PyTZINFO_HEAD
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    hashcode: Integer;
    hastzinfo: Char;  // boolean flag
    // End of _PyTZINFO_HEAD
    data: array[0..Pred(_PyDateTime_DATETIME_DATASIZE)] of Byte;
  end;
  _PPyDateTime_BaseDateTime = ^_PyDateTime_BaseDateTime;

  PyDateTime_DateTime = {$IFNDEF CPUX64}packed{$ENDIF} record // hastzinfo true
    // Start of _PyDateTime_DATETIMEHEAD
    // Start of _PyTZINFO_HEAD
    // Start of the Head of an object
    ob_refcnt: NativeInt;
    ob_type: PPyTypeObject;
    // End of the Head of an object
    hashcode: Integer;
    hastzinfo: Char;  // boolean flag
    // End of _PyTZINFO_HEAD
    data: array[0..Pred(_PyDateTime_DATETIME_DATASIZE)] of Byte;
    // End of _PyDateTime_DATETIMEHEAD
    tzinfo: PPyObject;
  end;
  PPyDateTime_DateTime = ^PyDateTime_DateTime;

  //#######################################################
  //##                                                   ##
  //##         GIL state                                 ##
  //##                                                   ##
  //#######################################################
  PyGILState_STATE = (PyGILState_LOCKED, PyGILState_UNLOCKED);

  //#######################################################
  //##                                                   ##
  //##         New exception classes                     ##
  //##                                                   ##
  //#######################################################

  // Components' exceptions
  EDLLLoadError = class(Exception);

  EDLLImportError = class(Exception)
  public
    WrongFunc: AnsiString;
    ErrorCode: Integer;
  end;

  // Python's exceptions
  EPythonError = class(Exception)
  public
    EName: String;
    EValue: String;
  end;

  EPyExecError = class(EPythonError);


  // Standard exception classes of Python

  /// jah 29-sep-2000 : updated for python 2.0
  ///                   base classes updated according python documentation

{ Hierarchy of Python exceptions, Python 2.3, copied from <INSTALL>\Python\exceptions.c

Exception\n\
 |\n\
 +-- SystemExit\n\
 +-- StopIteration\n\
 +-- StandardError\n\
 |    |\n\
 |    +-- KeyboardInterrupt\n\
 |    +-- ImportError\n\
 |    +-- EnvironmentError\n\
 |    |    |\n\
 |    |    +-- IOError\n\
 |    |    +-- OSError\n\
 |    |         |\n\
 |    |         +-- WindowsError\n\
 |    |         +-- VMSError\n\
 |    |\n\
 |    +-- EOFError\n\
 |    +-- RuntimeError\n\
 |    |    |\n\
 |    |    +-- NotImplementedError\n\
 |    |\n\
 |    +-- NameError\n\
 |    |    |\n\
 |    |    +-- UnboundLocalError\n\
 |    |\n\
 |    +-- AttributeError\n\
 |    +-- SyntaxError\n\
 |    |    |\n\
 |    |    +-- IndentationError\n\
 |    |         |\n\
 |    |         +-- TabError\n\
 |    |\n\
 |    +-- TypeError\n\
 |    +-- AssertionError\n\
 |    +-- LookupError\n\
 |    |    |\n\
 |    |    +-- IndexError\n\
 |    |    +-- KeyError\n\
 |    |\n\
 |    +-- ArithmeticError\n\
 |    |    |\n\
 |    |    +-- OverflowError\n\
 |    |    +-- ZeroDivisionError\n\
 |    |    +-- FloatingPointError\n\
 |    |\n\
 |    +-- ValueError\n\
 |    |    |\n\
 |    |    +-- UnicodeError\n\
 |    |        |\n\
 |    |        +-- UnicodeEncodeError\n\
 |    |        +-- UnicodeDecodeError\n\
 |    |        +-- UnicodeTranslateError\n\
 |    |\n\
 |    +-- ReferenceError\n\
 |    +-- SystemError\n\
 |    +-- MemoryError\n\
 |\n\
 +---Warning\n\
      |\n\
      +-- UserWarning\n\
      +-- DeprecationWarning\n\
      +-- PendingDeprecationWarning\n\
      +-- SyntaxWarning\n\
      +-- RuntimeWarning\n\
      +-- FutureWarning"
}
  EPyException = class(EPythonError);
  EPyStandardError = class(EPyException);
  EPyArithmeticError = class(EPyStandardError);
  EPyLookupError = class(EPyStandardError);
  EPyAssertionError = class(EPyStandardError);
  EPyAttributeError = class(EPyStandardError);
  EPyEOFError = class(EPyStandardError);
  EPyFloatingPointError = class(EPyArithmeticError);
  EPyEnvironmentError = class(EPyStandardError);
  EPyIOError = class(EPyEnvironmentError);
  EPyOSError = class(EPyEnvironmentError);
  EPyImportError = class(EPyStandardError);
  EPyIndexError = class(EPyLookupError);
  EPyKeyError = class(EPyLookupError);
  EPyKeyboardInterrupt = class(EPyStandardError);
  EPyMemoryError = class(EPyStandardError);
  EPyNameError = class(EPyStandardError);
  EPyOverflowError = class(EPyArithmeticError);
  EPyRuntimeError = class(EPyStandardError);
  EPyNotImplementedError = class(EPyRuntimeError);

  EPySyntaxError = class(EPyStandardError)
  public
    EFileName: string;
    ELineStr: string;
    ELineNumber: Integer;
    EOffset: Integer;
  end;

  EPyIndentationError = class(EPySyntaxError);
  EPyTabError = class(EPyIndentationError);
  EPySystemError = class(EPyStandardError);
  EPySystemExit = class(EPyException);
  EPyTypeError = class(EPyStandardError);
  EPyUnboundLocalError = class(EPyNameError);
  EPyValueError = class(EPyStandardError);
  EPyUnicodeError = class(EPyValueError);
  UnicodeEncodeError = class(EPyUnicodeError);
  UnicodeDecodeError = class(EPyUnicodeError);
  UnicodeTranslateError = class(EPyUnicodeError);
  EPyZeroDivisionError = class(EPyArithmeticError);
  EPyStopIteration = class(EPyException);
  EPyWarning = class(EPyException);
  EPyUserWarning = class(EPyWarning);
  EPyDeprecationWarning = class(EPyWarning);
  PendingDeprecationWarning = class(EPyWarning);
  FutureWarning = class(EPyWarning);
  EPySyntaxWarning = class(EPyWarning);
  EPyRuntimeWarning = class(EPyWarning);
  EPyReferenceError = class(EPyStandardError);
 {$IFDEF MSWINDOWS}
  EPyWindowsError = class(EPyOSError);
 {$ENDIF}

const
  kMaxLines = 1000;
  kMaxLineLength = 256;

var
  // define Python flags. See file pyDebug.h
  Py_DebugFlag: PInt;
  Py_VerboseFlag: PInt;
  Py_InteractiveFlag: PInt;
  Py_OptimizeFlag: PInt;
  Py_NoSiteFlag: PInt;
  Py_UseClassExceptionsFlag: PInt;
  Py_FrozenFlag: PInt;
  Py_TabcheckFlag: PInt;
  Py_UnicodeFlag: PInt;
  Py_IgnoreEnvironmentFlag: PInt;
  Py_DivisionWarningFlag: PInt;
  //_PySys_TraceFunc:    PPPyObject;
  //_PySys_ProfileFunc: PPPPyObject;

  PyImport_FrozenModules: PP_frozen;
  PyImport_AppendInittab: function(const name: pchar; initfunc: init_func): integer; cdecl;

  Py_None: PPyObject;
  Py_Ellipsis: PPyObject;
  Py_False: PPyIntObject;
  Py_True: PPyIntObject;
  Py_NotImplemented: PPyObject;

  PyExc_AttributeError: PPPyObject;
  PyExc_EOFError: PPPyObject;
  PyExc_IOError: PPPyObject;
  PyExc_ImportError: PPPyObject;
  PyExc_IndexError: PPPyObject;
  PyExc_KeyError: PPPyObject;
  PyExc_KeyboardInterrupt: PPPyObject;
  PyExc_MemoryError: PPPyObject;
  PyExc_NameError: PPPyObject;
  PyExc_OverflowError: PPPyObject;
  PyExc_RuntimeError: PPPyObject;
  PyExc_SyntaxError: PPPyObject;
  PyExc_SystemError: PPPyObject;
  PyExc_SystemExit: PPPyObject;
  PyExc_TypeError: PPPyObject;
  PyExc_ValueError: PPPyObject;
  PyExc_ZeroDivisionError: PPPyObject;
  PyExc_ArithmeticError: PPPyObject;
  PyExc_Exception: PPPyObject;
  PyExc_FloatingPointError: PPPyObject;
  PyExc_LookupError: PPPyObject;
  PyExc_StandardError: PPPyObject;
  PyExc_AssertionError: PPPyObject;
  PyExc_EnvironmentError: PPPyObject;
  PyExc_IndentationError: PPPyObject;
  PyExc_MemoryErrorInst: PPPyObject;
  PyExc_NotImplementedError: PPPyObject;
  PyExc_OSError: PPPyObject;
  PyExc_TabError: PPPyObject;
  PyExc_UnboundLocalError: PPPyObject;
  PyExc_UnicodeError: PPPyObject;
 {$IFDEF MSWINDOWS}
  PyExc_WindowsError: PPPyObject;
 {$ENDIF}
  PyExc_Warning: PPPyObject;
  PyExc_DeprecationWarning: PPPyObject;
  PyExc_RuntimeWarning: PPPyObject;
  PyExc_SyntaxWarning: PPPyObject;
  PyExc_UserWarning: PPPyObject;
  PyExc_ReferenceError: PPPyObject;
  PyExc_StopIteration: PPPyObject;
  PyExc_FutureWarning: PPPyObject;
  PyExc_PendingDeprecationWarning: PPPyObject;
  PyExc_UnicodeDecodeError: PPPyObject;
  PyExc_UnicodeEncodeError: PPPyObject;
  PyExc_UnicodeTranslateError: PPPyObject;

  PyType_Type: PPyTypeObject;
  PyCFunction_Type: PPyTypeObject;
  PyCObject_Type: PPyTypeObject;
  PyClass_Type: PPyTypeObject;
  PyCode_Type: PPyTypeObject;
  PyComplex_Type: PPyTypeObject;
  PyDict_Type: PPyTypeObject;
  PyFile_Type: PPyTypeObject;
  PyFloat_Type: PPyTypeObject;
  PyFrame_Type: PPyTypeObject;
  PyFunction_Type: PPyTypeObject;
  PyInstance_Type: PPyTypeObject;
  PyInt_Type: PPyTypeObject;
  PyList_Type: PPyTypeObject;
  PyLong_Type: PPyTypeObject;
  PyMethod_Type: PPyTypeObject;
  PyModule_Type: PPyTypeObject;
  PyObject_Type: PPyTypeObject;
  PyRange_Type: PPyTypeObject;
  PySlice_Type: PPyTypeObject;
  PyString_Type: PPyTypeObject;
  PyTuple_Type: PPyTypeObject;
  PyBaseObject_Type: PPyTypeObject;
  PyBuffer_Type: PPyTypeObject;
  PyCallIter_Type: PPyTypeObject;
  PyCell_Type: PPyTypeObject;
  PyClassMethod_Type: PPyTypeObject;
  PyProperty_Type: PPyTypeObject;
  PySeqIter_Type: PPyTypeObject;
  PyStaticMethod_Type: PPyTypeObject;
  PySuper_Type: PPyTypeObject;
  PyTraceBack_Type: PPyTypeObject;
  PyUnicode_Type: PPyTypeObject;
  PyWrapperDescr_Type: PPyTypeObject;
  _PyWeakref_RefType: PPyTypeObject;
  _PyWeakref_ProxyType: PPyTypeObject;
  _PyWeakref_CallableProxyType: PPyTypeObject;
  PyBaseString_Type: PPyTypeObject;
  PyBool_Type: PPyTypeObject;
  PyEnum_Type: PPyTypeObject;

  //PyArg_GetObject: function(args : PPyObject; nargs, i: integer; p_a: PPPyObject): integer; cdecl;
  //PyArg_GetLong:   function(args : PPyObject; nargs, i: integer; p_a: PLong): integer; cdecl;
  //PyArg_GetShort:  function(args : PPyObject; nargs, i: integer; p_a: PShort): integer; cdecl;
  //PyArg_GetFloat:  function(args : PPyObject; nargs, i: integer; p_a: PFloat): integer; cdecl;
  //PyArg_GetString: function(args : PPyObject; nargs, i: integer; p_a: PAnsiString): integer; cdecl;
  //PyArgs_VaParse:  function (args : PPyObject; format: PAnsiChar; va_list: array of const): integer; cdecl;
  // Does not work!
  // Py_VaBuildValue: function (format: PAnsiChar; va_list: array of const): PPyObject; cdecl;
  //PyBuiltin_Init:     procedure; cdecl;

  PyComplex_FromCComplex: function(c: Py_complex): PPyObject; cdecl;
  PyComplex_FromDoubles: function(realv, imag: double): PPyObject; cdecl;
  PyComplex_RealAsDouble: function(op: PPyObject): double; cdecl;
  PyComplex_ImagAsDouble: function(op: PPyObject): double; cdecl;
  PyComplex_AsCComplex: function(op: PPyObject): Py_complex; cdecl;
  PyCFunction_GetFunction: function(ob: PPyObject): Pointer; cdecl;
  PyCFunction_GetSelf: function(ob: PPyObject): PPyObject; cdecl;
  PyCallable_Check: function(ob: PPyObject): integer; cdecl;
  PyCObject_FromVoidPtr: function(cobj, destruct: Pointer): PPyObject; cdecl;
  PyCObject_AsVoidPtr: function(ob: PPyObject): Pointer; cdecl;
  PyClass_New: function(ob1, ob2, ob3: PPyObject): PPyObject; cdecl;
  PyClass_IsSubclass: function(ob1, ob2: PPyObject): integer cdecl;

  Py_InitModule4: function(name: PAnsiChar; methods: PPyMethodDef; doc: PAnsiChar; passthrough: PPyObject; Api_Version: Integer): PPyObject; cdecl;
  PyModule_Create2: function(moduledef: PPyModuleDef; Api_Version: Integer): PPyObject; cdecl;
  PyErr_BadArgument: function: integer; cdecl;
  PyErr_BadInternalCall: procedure; cdecl;
  PyErr_CheckSignals: function: integer; cdecl;
  PyErr_Clear: procedure; cdecl;
  PyErr_Fetch: procedure(errtype, errvalue, errtraceback: PPPyObject); cdecl;
  PyErr_NoMemory: function: PPyObject; cdecl;
  PyErr_Occurred: function: PPyObject; cdecl;
  PyErr_Print: procedure; cdecl;
  PyErr_Restore: procedure(errtype, errvalue, errtraceback: PPyObject); cdecl;
  PyErr_SetFromErrno: function(ob: PPyObject): PPyObject; cdecl;
  PyErr_SetNone: procedure(value: PPyObject); cdecl;
  PyErr_SetObject: procedure(ob1, ob2: PPyObject); cdecl;
  PyErr_SetString: procedure(ErrorObject: PPyObject; text: PAnsiChar); cdecl;
  PyImport_GetModuleDict: function: PPyObject; cdecl;
  PyInt_FromLong: function(x: LongInt): PPyObject; cdecl;
  PyArg_Parse: function(args: PPyObject; format: PAnsiChar {;....}): Integer; cdecl varargs;
  PyArg_ParseTuple: function(args: PPyObject; format: PAnsiChar {;...}): Integer; cdecl varargs;
  Py_BuildValue: function(format: PAnsiChar {;...}): PPyObject; cdecl varargs;
  Py_Initialize: procedure; cdecl;
  Py_Exit: procedure(RetVal: Integer); cdecl;
  PyEval_GetBuiltins: function: PPyObject; cdecl;
  PyDict_Copy: function(mp: PPyObject): PPyObject; cdecl;
  PyDict_GetItem: function(mp, key: PPyObject): PPyObject; cdecl;
  PyDict_SetItem: function(mp, key, item: PPyObject): integer; cdecl;
  PyDict_DelItem: function(mp, key: PPyObject): integer; cdecl;
  PyDict_Clear: procedure(mp: PPyObject); cdecl;
  PyDict_Next: function(mp: PPyObject; pos: PNativeInt; key, value: PPPyObject): integer; cdecl;
  PyDict_Keys: function(mp: PPyObject): PPyObject; cdecl;
  PyDict_Values: function(mp: PPyObject): PPyObject; cdecl;
  PyDict_Items: function(mp: PPyObject): PPyObject; cdecl;
  PyDict_Size: function(mp: PPyObject): NativeInt; cdecl;
  PyDict_DelItemString: function(dp: PPyObject; key: PAnsiChar): integer; cdecl;
  PyDict_New: function: PPyObject; cdecl;
  PyDict_GetItemString: function(dp: PPyObject; key: PAnsiChar): PPyObject; cdecl;
  PyDict_SetItemString: function(dp: PPyObject; key: PAnsiChar; item: PPyObject): Integer; cdecl;
  PyDictProxy_New: function(obj: PPyObject): PPyObject; cdecl;
  PyModule_GetDict: function(module: PPyObject): PPyObject; cdecl;
  PyObject_Str: function(v: PPyObject): PPyObject; cdecl;
  PyRun_String: function(str: PAnsiChar; start: Integer; globals: PPyObject; locals: PPyObject): PPyObject; cdecl;
  PyRun_SimpleString: function(str: PAnsiChar): Integer; cdecl;
  PyString_AsString: function(ob: PPyObject): PAnsiChar; cdecl;
  PySys_SetArgv: procedure(argc: Integer; argv: PPAnsiChar); cdecl;
  PySys_SetArgv3000: procedure(argc: Integer; argv: PPWideChar); cdecl;

  PyCFunction_New: function(md: PPyMethodDef; ob: PPyObject): PPyObject; cdecl;
  PyCFunction_NewEx: function(md: PPyMethodDef; self, ob: PPyObject): PPyObject; cdecl;
  // Removed.  Use PyEval_CallObjectWithKeywords with third argument nil
  //    PyEval_CallObject: function(callable_obj, args:PPyObject):PPyObject; cdecl;
  PyEval_CallObjectWithKeywords: function(callable_obj, args, kw: PPyObject): PPyObject; cdecl;
  PyEval_GetFrame: function: PPyObject; cdecl;
  PyEval_GetGlobals: function: PPyObject; cdecl;
  PyEval_GetLocals: function: PPyObject; cdecl;
  //PyEval_GetOwner:function :PPyObject; cdecl;
  PyEval_GetRestricted: function: integer; cdecl;

  PyEval_InitThreads: procedure; cdecl;
  PyEval_RestoreThread: procedure(tstate: PPyThreadState); cdecl;
  PyEval_SaveThread: function: PPyThreadState; cdecl;

  PyFile_FromString: function(pc1, pc2: PAnsiChar): PPyObject; cdecl;
  PyFile_GetLine: function(ob: PPyObject; i: integer): PPyObject; cdecl;
  PyFile_Name: function(ob: PPyObject): PPyObject; cdecl;
  PyFile_SetBufSize: procedure(ob: PPyObject; i: integer); cdecl;
  PyFile_SoftSpace: function(ob: PPyObject; i: integer): integer; cdecl;
  PyFile_WriteObject: function(ob1, ob2: PPyObject; i: integer): integer; cdecl;
  PyFile_WriteString: procedure(s: PAnsiChar; ob: PPyObject); cdecl;
  PyFloat_AsDouble: function(ob: PPyObject): DOUBLE; cdecl;
  PyFloat_FromDouble: function(db: double): PPyObject; cdecl;
  PyFunction_GetCode: function(ob: PPyObject): PPyObject; cdecl;
  PyFunction_GetGlobals: function(ob: PPyObject): PPyObject; cdecl;
  PyFunction_New: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyImport_AddModule: function(name: PAnsiChar): PPyObject; cdecl;
  PyImport_Cleanup: procedure; cdecl;
  PyImport_GetMagicNumber: function: LONGINT; cdecl;
  PyImport_ImportFrozenModule: function(key: PAnsiChar): integer; cdecl;
  PyImport_ImportModule: function(name: PAnsiChar): PPyObject; cdecl;
  PyImport_Import: function(name: PPyObject): PPyObject; cdecl;
  //PyImport_Init:procedure; cdecl;
  PyImport_ReloadModule: function(ob: PPyObject): PPyObject; cdecl;
  PyInstance_New: function(obClass, obArg, obKW: PPyObject): PPyObject; cdecl;
  PyInt_AsLong: function(ob: PPyObject): LONGINT; cdecl;
  PyList_Append: function(ob1, ob2: PPyObject): integer; cdecl;
  PyList_AsTuple: function(ob: PPyObject): PPyObject; cdecl;
  PyList_GetItem: function(ob: PPyObject; i: NativeInt): PPyObject; cdecl;
  PyList_GetSlice: function(ob: PPyObject; i1, i2: NativeInt): PPyObject; cdecl;
  PyList_Insert: function(dp: PPyObject; idx: NativeInt; item: PPyObject): integer; cdecl;
  PyList_New: function(size: NativeInt): PPyObject; cdecl;
  PyList_Reverse: function(ob: PPyObject): integer; cdecl;
  PyList_SetItem: function(dp: PPyObject; idx: NativeInt; item: PPyObject): integer; cdecl;
  PyList_SetSlice: function(ob: PPyObject; i1, i2: NativeInt; ob2: PPyObject): integer; cdecl;
  PyList_Size: function(ob: PPyObject): NativeInt; cdecl;
  PyList_Sort: function(ob: PPyObject): integer; cdecl;
  PyLong_AsDouble: function(ob: PPyObject): DOUBLE; cdecl;
  PyLong_AsLong: function(ob: PPyObject): LONGINT; cdecl;
  PyLong_FromDouble: function(db: double): PPyObject; cdecl;
  PyLong_FromLong: function(l: longint): PPyObject; cdecl;
  PyLong_FromString: function(pc: PAnsiChar; var ppc: PAnsiChar; i: integer): PPyObject; cdecl;
  PyLong_FromUnsignedLong: function(val: cardinal): PPyObject; cdecl;
  PyLong_AsUnsignedLong: function(ob: PPyObject): Cardinal; cdecl;
  PyLong_FromUnicode: function(ob: PPyObject; a, b: integer): PPyObject; cdecl;
  PyLong_FromLongLong: function(val: Int64): PPyObject; cdecl;
  PyLong_AsLongLong: function(ob: PPyObject): Int64; cdecl;
  PyMapping_Check: function(ob: PPyObject): integer; cdecl;
  PyMapping_GetItemString: function(ob: PPyObject; key: PAnsiChar): PPyObject; cdecl;
  PyMapping_HasKey: function(ob, key: PPyObject): integer; cdecl;
  PyMapping_HasKeyString: function(ob: PPyObject; key: PAnsiChar): integer; cdecl;
  PyMapping_Length: function(ob: PPyObject): NativeInt; cdecl;
  PyMapping_SetItemString: function(ob: PPyObject; key: PAnsiChar; value: PPyObject): integer; cdecl;
  PyMethod_Class: function(ob: PPyObject): PPyObject; cdecl;
  PyMethod_Function: function(ob: PPyObject): PPyObject; cdecl;
  PyMethod_New: function(ob1, ob2, ob3: PPyObject): PPyObject; cdecl;
  PyMethod_Self: function(ob: PPyObject): PPyObject; cdecl;
  PyModule_GetName: function(ob: PPyObject): PAnsiChar; cdecl;
  PyModule_New: function(key: PAnsiChar): PPyObject; cdecl;
  PyNumber_Absolute: function(ob: PPyObject): PPyObject; cdecl;
  PyNumber_Add: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyNumber_And: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyNumber_Check: function(ob: PPyObject): integer; cdecl;
  PyNumber_Coerce: function(var ob1, ob2: PPyObject): integer; cdecl;
  PyNumber_Divide: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyNumber_FloorDivide: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyNumber_TrueDivide: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyNumber_Divmod: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyNumber_Float: function(ob: PPyObject): PPyObject; cdecl;
  PyNumber_Int: function(ob: PPyObject): PPyObject; cdecl;
  PyNumber_Invert: function(ob: PPyObject): PPyObject; cdecl;
  PyNumber_Long: function(ob: PPyObject): PPyObject; cdecl;
  PyNumber_Lshift: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyNumber_Multiply: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyNumber_Negative: function(ob: PPyObject): PPyObject; cdecl;
  PyNumber_Or: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyNumber_Positive: function(ob: PPyObject): PPyObject; cdecl;
  PyNumber_Power: function(ob1, ob2, ob3: PPyObject): PPyObject; cdecl;
  PyNumber_Remainder: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyNumber_Rshift: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyNumber_Subtract: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyNumber_Xor: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyOS_InitInterrupts: procedure; cdecl;
  PyOS_InterruptOccurred: function: integer; cdecl;
  PyObject_CallObject: function(ob, args: PPyObject): PPyObject; cdecl;
  PyObject_CallMethod: function(obj: PPyObject; method, format: PAnsiChar {...}): PPyObject; cdecl varargs;
  PyObject_CallMethodStr: function(obj: PPyObject; method, format, value: PAnsiChar): PPyObject; cdecl;
  PyObject_Compare: function(ob1, ob2: PPyObject): integer; cdecl;
  PyObject_RichCompare: function(ob1, ob2: PPyObject; opid: integer): PPyObject; cdecl;
  PyObject_RichCompareBool: function(ob1, ob2: PPyObject; opid: integer): Integer; cdecl;
  PyObject_GetAttr: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyObject_GetAttrString: function(ob: PPyObject; c: PAnsiChar): PPyObject; cdecl;
  PyObject_GetItem: function(ob, key: PPyObject): PPyObject; cdecl;
  PyObject_DelItem: function(ob, key: PPyObject): PPyObject; cdecl;
  PyObject_HasAttrString: function(ob: PPyObject; key: PAnsiChar): integer; cdecl;
  PyObject_Hash: function(ob: PPyObject): NativeInt; cdecl;
  PyObject_IsTrue: function(ob: PPyObject): integer; cdecl;
  PyObject_Length: function(ob: PPyObject): NativeInt; cdecl;
  PyObject_Repr: function(ob: PPyObject): PPyObject; cdecl;
  PyObject_SetAttr: function(ob1, ob2, ob3: PPyObject): integer; cdecl;
  PyObject_SetAttrString: function(ob: PPyObject; key: PAnsiChar; value: PPyObject): integer; cdecl;
  PyObject_SetItem: function(ob1, ob2, ob3: PPyObject): integer; cdecl;
  PyObject_Init: function(ob: PPyObject; t: PPyTypeObject): PPyObject; cdecl;
  PyObject_InitVar: function(ob: PPyObject; t: PPyTypeObject; size: NativeInt): PPyObject; cdecl;
  PyObject_New: function(t: PPyTypeObject): PPyObject; cdecl;
  PyObject_NewVar: function(t: PPyTypeObject; size: NativeInt): PPyObject; cdecl;
  PyObject_Free: procedure(ob: PPyObject); cdecl;
  PyObject_GetIter: function(obj: PPyObject): PPyObject; cdecl;
  PyIter_Next: function(obj: PPyObject): PPyObject; cdecl;
  PyObject_IsInstance: function(inst, cls: PPyObject): integer; cdecl;
  PyObject_IsSubclass: function(derived, cls: PPyObject): integer; cdecl;
  PyObject_Call: function(ob, args, kw: PPyObject): PPyObject; cdecl;
  PyObject_GenericGetAttr: function(obj, name: PPyObject): PPyObject; cdecl;
  PyObject_GenericSetAttr: function(obj, name, value: PPyObject): Integer; cdecl;
  PyObject_GC_Malloc: function(size: NativeUInt): PPyObject; cdecl;
  PyObject_GC_New: function(t: PPyTypeObject): PPyObject; cdecl;
  PyObject_GC_NewVar: function(t: PPyTypeObject; size: NativeInt): PPyObject; cdecl;
  PyObject_GC_Resize: function(t: PPyObject; newsize: NativeInt): PPyObject; cdecl;
  PyObject_GC_Del: procedure(ob: PPyObject); cdecl;
  PyObject_GC_Track: procedure(ob: PPyObject); cdecl;
  PyObject_GC_UnTrack: procedure(ob: PPyObject); cdecl;
  PySequence_Check: function(ob: PPyObject): integer; cdecl;
  PySequence_Concat: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PySequence_Count: function(ob1, ob2: PPyObject): integer; cdecl;
  PySequence_GetItem: function(ob: PPyObject; i: NativeInt): PPyObject; cdecl;
  PySequence_GetSlice: function(ob: PPyObject; i1, i2: NativeInt): PPyObject; cdecl;
  PySequence_In: function(ob1, ob2: PPyObject): integer; cdecl;
  PySequence_Index: function(ob1, ob2: PPyObject): NativeInt; cdecl;
  PySequence_Length: function(ob: PPyObject): NativeInt; cdecl;
  PySequence_Repeat: function(ob: PPyObject; count: NativeInt): PPyObject; cdecl;
  PySequence_SetItem: function(ob: PPyObject; i: NativeInt; value: PPyObject): integer; cdecl;
  PySequence_SetSlice: function(ob: PPyObject; i1, i2: NativeInt; value: PPyObject): integer; cdecl;
  PySequence_DelSlice: function(ob: PPyObject; i1, i2: NativeInt): integer; cdecl;
  PySequence_Tuple: function(ob: PPyObject): PPyObject; cdecl;
  PySequence_Contains: function(ob, value: PPyObject): integer; cdecl;
  PySeqIter_New: function(obj: PPyObject): PPyObject; cdecl;
  PySlice_GetIndices: function(ob: PPySliceObject; length: NativeInt; var start, stop, step: NativeInt): integer; cdecl;
  PySlice_GetIndicesEx: function(ob: PPySliceObject; length: NativeInt; var start, stop, step, slicelength: NativeInt): integer; cdecl;
  PySlice_New: function(start, stop, step: PPyObject): PPyObject; cdecl;
  PyString_Concat: procedure(var ob1: PPyObject; ob2: PPyObject); cdecl;
  PyString_ConcatAndDel: procedure(var ob1: PPyObject; ob2: PPyObject); cdecl;
  PyString_Format: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyString_FromStringAndSize: function(s: PAnsiChar; i: NativeInt): PPyObject; cdecl;
  PyString_Size: function(ob: PPyObject): NativeInt; cdecl;
  PyString_DecodeEscape: function(s: PAnsiChar; len: NativeInt; errors: PAnsiChar; unicode: NativeInt; recode_encoding: PAnsiChar): PPyObject; cdecl;
  PyString_Repr: function(ob: PPyObject; smartquotes: integer): PPyObject; cdecl;
  PySys_GetObject: function(s: PAnsiChar): PPyObject; cdecl;
  //PySys_Init:procedure; cdecl;
  PySys_SetObject: function(s: PAnsiChar; ob: PPyObject): integer; cdecl;
  PySys_SetPath: procedure(path: PAnsiChar); cdecl;
  //PyTraceBack_Fetch:function :PPyObject; cdecl;
  PyTraceBack_Here: function(p: pointer): integer; cdecl;
  PyTraceBack_Print: function(ob1, ob2: PPyObject): integer; cdecl;
  //PyTraceBack_Store:function (ob:PPyObject):integer; cdecl;
  PyTuple_GetItem: function(ob: PPyObject; i: NativeInt): PPyObject; cdecl;
  PyTuple_GetSlice: function(ob: PPyObject; i1, i2: NativeInt): PPyObject; cdecl;
  PyTuple_New: function(size: NativeInt): PPyObject; cdecl;
  PyTuple_SetItem: function(ob: PPyObject; key: NativeInt; value: PPyObject): integer; cdecl;
  PyTuple_Size: function(ob: PPyObject): NativeInt; cdecl;
  PyType_IsSubtype: function(a, b: PPyTypeObject): integer; cdecl;
  PyType_GenericAlloc: function(atype: PPyTypeObject; nitems: NativeInt): PPyObject; cdecl;
  PyType_GenericNew: function(atype: PPyTypeObject; args, kwds: PPyObject): PPyObject; cdecl;
  PyType_Ready: function(atype: PPyTypeObject): integer; cdecl;
  PyUnicode_FromWideChar: function(const w: PWideChar; size: NativeInt): PPyObject; cdecl;
  PyUnicode_AsWideChar: function(unicode: PPyObject; w: PWideChar; size: NativeInt): integer; cdecl;
  PyUnicode_Decode: function(const s: PAnsiChar; size: NativeInt; const encoding: PAnsiChar; const errors: PAnsiChar): PPyObject; cdecl;
  PyUnicode_AsEncodedString: function(unicode: PPyObject; const encoding: PAnsiChar; const errors: PAnsiChar): PPyObject; cdecl;
  PyUnicode_FromOrdinal: function(ordinal: integer): PPyObject; cdecl;
  PyUnicode_FromString: function(const u: pchar): pPyObject; cdecl;
  PyUnicode_GetSize: function(unicode: PPyObject): NativeInt; cdecl;
  PyWeakref_GetObject: function(ref: PPyObject): PPyObject; cdecl;
  PyWeakref_NewProxy: function(ob, callback: PPyObject): PPyObject; cdecl;
  PyWeakref_NewRef: function(ob, callback: PPyObject): PPyObject; cdecl;
  PyWrapper_New: function(ob1, ob2: PPyObject): PPyObject; cdecl;
  PyBool_FromLong: function(ok: Integer): PPyObject; cdecl;
  PyThreadState_SetAsyncExc: function(t_id: LongInt; exc: PPyObject): Integer; cdecl;
  Py_AtExit: function(proc: AtExitProc): integer; cdecl;
  //Py_Cleanup:procedure; cdecl;
  Py_CompileString: function(s1, s2: PAnsiChar; i: integer): PPyObject; cdecl;
  Py_FatalError: procedure(s: PAnsiChar); cdecl;
  Py_FindMethod: function(md: PPyMethodDef; ob: PPyObject; key: PAnsiChar): PPyObject; cdecl;
  Py_FindMethodInChain: function(mc: PPyMethodChain; ob: PPyObject; key: PAnsiChar): PPyObject; cdecl;
  _PyObject_New: function(obt: PPyTypeObject; ob: PPyObject): PPyObject; cdecl;
  _PyString_Resize: function(var ob: PPyObject; i: NativeInt): integer; cdecl;

  DLL_Py_GetBuildInfo: function: PAnsiChar; cdecl;
  DLL_PyCode_Addr2Line: function(co: PPyCodeObject; addrq: Integer): Integer; cdecl;
  DLL_PyImport_ExecCodeModule: function(const name: AnsiString; codeobject: PPyObject): PPyObject; cdecl;

  DLL_PyString_FromString: function(str: PAnsiChar): PPyObject; cdecl;
  DLL_Py_FlushLine: procedure; cdecl;

  Py_Finalize: procedure; cdecl;
  PyErr_ExceptionMatches: function(exc: PPyObject): Integer; cdecl;
  PyErr_GivenExceptionMatches: function(raised_exc, exc: PPyObject): Integer; cdecl;
  PyEval_EvalCode: function(co: PPyCodeObject; globals, locals: PPyObject): PPyObject; cdecl;
  Py_GetVersion: function: PAnsiChar; cdecl;
  Py_GetCopyright: function: PAnsiChar; cdecl;
  Py_GetExecPrefix: function: PAnsiChar; cdecl;
  Py_GetPath: function: PAnsiChar; cdecl;
  Py_GetPrefix: function: PAnsiChar; cdecl;
  Py_GetProgramName: function: PAnsiChar; cdecl;

  PyParser_SimpleParseString: function(str: PAnsiChar; start: Integer): PNode; cdecl;
  PyNode_Free: procedure(n: PNode); cdecl;
  PyErr_NewException: function(name: PAnsiChar; base, dict: PPyObject): PPyObject; cdecl;
  Py_Malloc: function(size: NativeInt): Pointer;
  PyMem_Malloc: function(size: NativeInt): Pointer;

  {New exported Objects in Python 1.5}
  Py_SetProgramName: procedure(name: PAnsiChar); cdecl;
  Py_SetProgramName3000: procedure(name: PWideChar); cdecl;
  Py_IsInitialized: function: integer; cdecl;
  Py_GetProgramFullPath: function: PAnsiChar; cdecl;
  Py_NewInterpreter: function: PPyThreadState; cdecl;
  Py_EndInterpreter: procedure(tstate: PPyThreadState); cdecl;
  PyEval_AcquireLock: procedure; cdecl;
  PyEval_ReleaseLock: procedure; cdecl;
  PyEval_AcquireThread: procedure(tstate: PPyThreadState); cdecl;
  PyEval_ReleaseThread: procedure(tstate: PPyThreadState); cdecl;
  PyInterpreterState_New: function: PPyInterpreterState; cdecl;
  PyInterpreterState_Clear: procedure(interp: PPyInterpreterState); cdecl;
  PyInterpreterState_Delete: procedure(interp: PPyInterpreterState); cdecl;
  PyThreadState_New: function(interp: PPyInterpreterState): PPyThreadState; cdecl;
  PyThreadState_Clear: procedure(tstate: PPyThreadState); cdecl;
  PyThreadState_Delete: procedure(tstate: PPyThreadState); cdecl;
  PyThreadState_Get: function: PPyThreadState; cdecl;
  PyThreadState_Swap: function(tstate: PPyThreadState): PPyThreadState; cdecl;
  PyErr_SetInterrupt: procedure; cdecl;
  PyGILState_Ensure: function(): PyGILstate_STATE; cdecl;
  PyGILState_Release: procedure(gilstate: PyGILState_STATE); cdecl;

{Further exported Objects, may be implemented later}
{
    PyCode_New: Pointer;
    PyFile_AsFile: Pointer;
    PyFile_FromFile: Pointer;
    PyFloat_AsString: Pointer;
    PyFrame_BlockPop: Pointer;
    PyFrame_BlockSetup: Pointer;
    PyFrame_ExtendStack: Pointer;
    PyFrame_FastToLocals: Pointer;
    PyFrame_LocalsToFast: Pointer;
    PyFrame_New: Pointer;
    PyGrammar_AddAccelerators: Pointer;
    PyGrammar_FindDFA: Pointer;
    PyGrammar_LabelRepr: Pointer;
    PyInstance_DoBinOp: Pointer;
    PyInt_GetMax: Pointer;
    PyMarshal_Init: Pointer;
    PyMarshal_ReadLongFromFile: Pointer;
    PyMarshal_ReadObjectFromFile: Pointer;
    PyMarshal_ReadObjectFromString: Pointer;
    PyMarshal_WriteLongToFile: Pointer;
    PyMarshal_WriteObjectToFile: Pointer;
    PyMember_Get: Pointer;
    PyMember_Set: Pointer;
    PyNode_AddChild: Pointer;
    PyNode_Compile: Pointer;
    PyNode_New: Pointer;
    PyOS_GetLastModificationTime: Pointer;
    PyOS_Readline: Pointer;
    PyOS_strtol: Pointer;
    PyOS_strtoul: Pointer;
    PyObject_CallFunction: Pointer;
    PyObject_Print: Pointer;
    PyParser_AddToken: Pointer;
    PyParser_Delete: Pointer;
    PyParser_New: Pointer;
    PyParser_ParseFile: Pointer;
    PyParser_ParseString: Pointer;
    PyParser_SimpleParseFile: Pointer;
    PyRun_AnyFile: Pointer;
    PyRun_File: Pointer;
    PyRun_InteractiveLoop: Pointer;
    PyRun_InteractiveOne: Pointer;
    PyRun_SimpleFile: Pointer;
    PySys_GetFile: Pointer;
    PyToken_OneChar: Pointer;
    PyToken_TwoChars: Pointer;
    PyTokenizer_Free: Pointer;
    PyTokenizer_FromFile: Pointer;
    PyTokenizer_FromString: Pointer;
    PyTokenizer_Get: Pointer;
    Py_Main: Pointer;
    _PyParser_Grammar: Pointer;
    _PyParser_TokenNames: Pointer;
    _PyThread_Started: Pointer;
    _Py_c_diff: Pointer;
    _Py_c_neg: Pointer;
    _Py_c_pow: Pointer;
    _Py_c_prod: Pointer;
    _Py_c_quot: Pointer;
    _Py_c_sum: Pointer;
}

// functions redefined in Delphi
procedure Py_INCREF(op: PPyObject);
procedure Py_DECREF(op: PPyObject);
procedure Py_XINCREF(op: PPyObject);
procedure Py_XDECREF(op: PPyObject);

function Py_GetPlatform: PAnsiChar; cdecl;
function PyCode_Addr2Line(co: PPyCodeObject; addrq: Integer): Integer; cdecl;
//function Py_GetBuildInfo: PAnsiChar; cdecl;
//function PyImport_ExecCodeModule( const AName : AnsiString; codeobject : PPyObject) : PPyObject;
//function PyString_Check( obj : PPyObject ) : Boolean;
//function PyString_CheckExact( obj : PPyObject ) : Boolean;
//function PyFloat_Check( obj : PPyObject ) : Boolean;
//function PyFloat_CheckExact( obj : PPyObject ) : Boolean;
//function PyInt_Check( obj : PPyObject ) : Boolean;
//function PyInt_CheckExact( obj : PPyObject ) : Boolean;
//function PyLong_Check( obj : PPyObject ) : Boolean;
//function PyLong_CheckExact( obj : PPyObject ) : Boolean;
//function PyTuple_Check( obj : PPyObject ) : Boolean;
//function PyTuple_CheckExact( obj : PPyObject ) : Boolean;
//function PyInstance_Check( obj : PPyObject ) : Boolean;
//function PyClass_Check( obj : PPyObject ) : Boolean;
//function PyType_CheckExact( obj : PPyObject ) : Boolean;
//function PyMethod_Check( obj : PPyObject ) : Boolean;
//function PyList_Check( obj : PPyObject ) : Boolean;
//function PyList_CheckExact( obj : PPyObject ) : Boolean;
//function PyDict_Check( obj : PPyObject ) : Boolean;
//function PyDict_CheckExact( obj : PPyObject ) : Boolean;
//function PyModule_Check( obj : PPyObject ) : Boolean;
//function PyModule_CheckExact( obj : PPyObject ) : Boolean;
//function PySlice_Check( obj : PPyObject ) : Boolean;
//function PyFunction_Check( obj : PPyObject ) : Boolean;
//function PyIter_Check( obj : PPyObject ) : Boolean;
//function PyUnicode_Check( obj : PPyObject ) : Boolean;
//function PyUnicode_CheckExact( obj : PPyObject ) : Boolean;
//function PyType_IS_GC(t : PPyTypeObject ) : Boolean;
//function PyObject_IS_GC( obj : PPyObject ) : Boolean;
//function PyWeakref_Check( obj : PPyObject ) : Boolean;
//function PyWeakref_CheckRef( obj : PPyObject ) : Boolean;
//function PyWeakref_CheckProxy( obj : PPyObject ) : Boolean;
//function PyBool_Check( obj : PPyObject ) : Boolean;
//function PyBaseString_Check( obj : PPyObject ) : Boolean;
//function PyEnum_Check( obj : PPyObject ) : Boolean;
//function PyObject_TypeCheck(obj:PPyObject; t:PPyTypeObject) : Boolean;
//function Py_InitModule( const AName : PAnsiChar; md : PPyMethodDef) : PPyObject;
//function Py_InitModule3000( const md : PyModuleDef) : PPyObject;
//function PyString_FromString( str: PAnsiChar): PPyObject;
//function PyString_AsDelphiString( ob: PPyObject): string;
//procedure Py_FlushLine; cdecl;

type
  TDatetimeConversionMode = (dcmToTuple, dcmToDatetime);

const
  DEFAULT_DATETIME_CONVERSION_MODE = dcmToTuple;

type
  TPathInitializationEvent = procedure(Sender: TObject; var Path: String) of Object;
  TSysPathInitEvent = procedure(Sender: TObject; PathList: PPyObject) of Object;
  TPythonFlag = (pfDebug, pfInteractive, pfNoSite, pfOptimize, pfTabcheck, pfUnicode, pfVerbose,
    pfUseClassExceptionsFlag, pfFrozenFlag, pfIgnoreEnvironmentFlag, pfDivisionWarningFlag);
  TPythonFlags = set of TPythonFlag;

  TMethodArray = array[0 .. 16000] of PyMethodDef;
  PMethodArray = ^TMethodArray;
  TDelphiMethod = function(self, args: PPyObject): PPyObject of object; cdecl;
  TDelphiMethodWithKW = function(self, args, keywords: PPyObject): PPyObject of object; cdecl;
  TPythonEvent = procedure(Sender: TObject; PSelf, Args: PPyObject; var Result: PPyObject) of object;


  //------------------------------------------------------------
  //--                                                        --
  //--class: TMembersContainer derived from TMethodsContainer --
  //--                                                        --
  //------------------------------------------------------------

  TMemberArray = array[0 .. 16000] of PyMemberDef;
  PMemberArray = ^TMemberArray;


  TGetSetArray = array[0 .. 16000] of PyGetSetDef;
  PGetSetArray = ^TGetSetArray;


  TBasicServices = set of (bsPrint, bsGetAttr, bsSetAttr,
    bsRepr, bsCompare, bsHash,
    bsStr, bsGetAttrO, bsSetAttrO,
    bsCall,
    // since version 2.0
    bsTraverse, bsClear,
    // since version 2.1
    bsRichCompare,
    // since version 2.2
    bsIter, bsIterNext);
  TNumberServices = set of (nsAdd, nsSubstract, nsMultiply,
    nsDivide, nsRemainder, nsDivmod,
    nsPower, nsNegative, nsPositive,
    nsAbsolute, nsNonZero, nsInvert,
    nsLShift, nsRShift, nsAnd,
    nsXor, nsOr, nsCoerce,
    nsInt, nsLong, nsFloat,
    nsOct, nsHex,
    // since version 2.2
    nsFloorDivide, nsTrueDivide);

  // TInplaceNumberServices exists since version 2.0
  TInplaceNumberServices = set of (nsInplaceAdd, nsInplaceSubtract,
    nsInplaceMultiply, nsInplaceDivide,
    nsInplaceRemainder, nsInplacePower,
    nsInplaceLShift, nsInplaceRShift,
    nsInplaceAnd, nsInplaceXor, nsInplaceOr,
    // since version 2.2
    nsInplaceFloorDivide, nsInplaceTrueDivide);

  TSequenceServices = set of (ssLength, ssConcat, ssRepeat,
    ssItem, ssSlice, ssAssItem,
    ssAssSlice,
    // since version 2.0
    ssContains,
    ssInplaceConcat,
    ssInplaceRepeat
    );

  TMappingServices = set of (msLength, msSubscript, msAssSubscript);


  //-------------------------------------------------------
  //--                                                   --
  //--  class: TPythonVar derived from TEngineClient     --
  //--                                                   --
  //-------------------------------------------------------

  TGetDataEvent = procedure(Sender: TObject; var Data: Variant) of Object;
  TSetDataEvent = procedure(Sender: TObject; Data: Variant) of Object;
  TExtGetDataEvent = procedure(Sender: TObject; var Data: PPyObject) of Object;
  TExtSetDataEvent = procedure(Sender: TObject; Data: PPyObject) of Object;


  //#######################################################
  //##                                                   ##
  //##  Thread Object with Python interpreter lock       ##
  //##                                                   ##
  //#######################################################
  TThreadExecMode = (emNewState, emNewInterpreter);


//#######################################################
//##                                                   ##
//##        New Python objects                         ##
//##                                                   ##
//#######################################################

//#######################################################
//##                                                   ##
//##    Methods for new Python objects or modules      ##
//##                                                   ##
//#######################################################

// Module pyio for Python Input/Outputs
//function  pyio_write(self, args : PPyObject) : PPyObject; cdecl;
//function  pyio_read(self, args : PPyObject) : PPyObject; cdecl;
//function  pyio_SetDelayWrites(self, args : PPyObject) : PPyObject; cdecl;
//function  pyio_SetMaxLines(self, args : PPyObject) : PPyObject; cdecl;
//function  pyio_GetTypesStats(self, args : PPyObject) : PPyObject; cdecl;


//#######################################################
//##                                                   ##
//##        Global procedures                          ##
//##                                                   ##
//#######################################################

//function  PythonOK : Boolean;
//function  IsDelphiObject( obj : PPyObject ) : Boolean;
//procedure PyObjectDestructor( pSelf : PPyObject); cdecl;
//procedure FreeSubtypeInst(ob:PPyObject); cdecl;
//procedure Register;
//function  PyType_HasFeature(AType : PPyTypeObject; AFlag : Integer) : Boolean;

{ Helper functions}
(*
    Checks whether the PythonVersion x.x is Registered
*)
{$IFDEF MSWINDOWS}
//function IsPythonVersionRegistered(PythonVersion : string;
//  out InstallPath: string; out AllUserInstall: Boolean) : Boolean;
{$ENDIF}
(*
  Mask FPU Excptions - Useful for importing SciPy and other Python libs
  See http://bugs.python.org/issue9980 and
  http://stackoverflow.com/questions/3933851/
*)
//procedure MaskFPUExceptions(ExceptionsMasked : boolean;
//  MatchPythonPrecision : Boolean = True);

//#######################################################
//##                                                   ##
//##        Global variables                           ##
//##                                                   ##
//#######################################################

implementation

uses
  DynLibs;

var
  DLL_handle: THandle;

procedure Py_INCREF(op: PPyObject);
begin
  Inc(op^.ob_refcnt);
end;

procedure Py_DECREF(op: PPyObject);
begin
  with op^ do
  begin
    Dec(ob_refcnt);
    if ob_refcnt = 0 then
    begin
      ob_type^.tp_dealloc(op);
    end;
  end;
end;

procedure Py_XINCREF(op: PPyObject);
begin
  if op <> nil then
    Py_INCREF(op);
end;

procedure Py_XDECREF(op: PPyObject);
begin
  if op <> nil then
    Py_DECREF(op);
end;

function Py_GetPlatform: PAnsiChar; cdecl;
begin
  Py_GetPlatform := 'win32';
end;

function PyCode_Addr2Line(co: PPyCodeObject; addrq: Integer): Integer; cdecl;
var
  size: Integer;
  p: PAnsiChar;
  line: Integer;
  addr: Integer;
  cpt: Integer;
begin
  if Assigned(DLL_PyCode_Addr2Line) then
  begin
    Result := DLL_PyCode_Addr2Line(co, addrq);
    Exit;
  end;
  size := PyString_Size(co^.co_lnotab) div 2;
  p := PyString_AsString(co^.co_lnotab);
  line := co^.co_firstlineno;
  addr := 0;
  cpt := 0;
  while (size - 1) >= 0 do
  begin
    Dec(size);
    Inc(addr, Ord(p[cpt]));
    Inc(cpt);
    if addr > addrq then
      Break;
    Inc(line, Ord(p[cpt]));
    Inc(cpt);
  end;
  Result := line;
end;

const
  GetUnicodeTypeSuffix = '';

function ImportPyFunc(const funcname: ansistring; canFail: boolean = True): Pointer;
begin
  Result := GetProcAddress(DLL_handle, PAnsiChar(funcname));
  if (Result = nil) and canFail then
  begin
    writeln('Error: could not find symbol "', funcname, '"');
    halt(1);
  end;
end;

initialization

  DLL_handle := LoadLibrary('python36.dll');
  if DLL_handle = NilHandle then
  begin
    writeln('Error: could not load python');
    halt(1);
  end;

  Py_DebugFlag := ImportPyFunc('Py_DebugFlag');
  Py_VerboseFlag := ImportPyFunc('Py_VerboseFlag');
  Py_InteractiveFlag := ImportPyFunc('Py_InteractiveFlag');
  Py_OptimizeFlag := ImportPyFunc('Py_OptimizeFlag');
  Py_NoSiteFlag := ImportPyFunc('Py_NoSiteFlag');
  Py_FrozenFlag := ImportPyFunc('Py_FrozenFlag');

  Py_IgnoreEnvironmentFlag := ImportPyFunc('Py_IgnoreEnvironmentFlag');

  //_PySys_TraceFunc           := ImportPyFunc('_PySys_TraceFunc');
  //_PySys_ProfileFunc         := ImportPyFunc('_PySys_ProfileFunc');

  Py_None := ImportPyFunc('_Py_NoneStruct');
  Py_Ellipsis := ImportPyFunc('_Py_EllipsisObject');
  Py_False := ImportPyFunc('_Py_FalseStruct');
  Py_True := ImportPyFunc('_Py_TrueStruct');
  Py_NotImplemented := ImportPyFunc('_Py_NotImplementedStruct');

  PyImport_FrozenModules := ImportPyFunc('PyImport_FrozenModules');
  PyImport_AppendInittab := ImportPyFunc('PyImport_AppendInittab');

  PyExc_AttributeError := ImportPyFunc('PyExc_AttributeError');
  PyExc_EOFError := ImportPyFunc('PyExc_EOFError');
  PyExc_IOError := ImportPyFunc('PyExc_IOError');
  PyExc_ImportError := ImportPyFunc('PyExc_ImportError');
  PyExc_IndexError := ImportPyFunc('PyExc_IndexError');
  PyExc_KeyError := ImportPyFunc('PyExc_KeyError');
  PyExc_KeyboardInterrupt := ImportPyFunc('PyExc_KeyboardInterrupt');
  PyExc_MemoryError := ImportPyFunc('PyExc_MemoryError');
  PyExc_NameError := ImportPyFunc('PyExc_NameError');
  PyExc_OverflowError := ImportPyFunc('PyExc_OverflowError');
  PyExc_RuntimeError := ImportPyFunc('PyExc_RuntimeError');
  PyExc_SyntaxError := ImportPyFunc('PyExc_SyntaxError');
  PyExc_SystemError := ImportPyFunc('PyExc_SystemError');
  PyExc_SystemExit := ImportPyFunc('PyExc_SystemExit');
  PyExc_TypeError := ImportPyFunc('PyExc_TypeError');
  PyExc_ValueError := ImportPyFunc('PyExc_ValueError');
  PyExc_ZeroDivisionError := ImportPyFunc('PyExc_ZeroDivisionError');
  PyExc_ArithmeticError := ImportPyFunc('PyExc_ArithmeticError');
  PyExc_Exception := ImportPyFunc('PyExc_Exception');
  PyExc_FloatingPointError := ImportPyFunc('PyExc_FloatingPointError');
  PyExc_LookupError := ImportPyFunc('PyExc_LookupError');
  PyExc_AssertionError := ImportPyFunc('PyExc_AssertionError');
  PyExc_EnvironmentError := ImportPyFunc('PyExc_EnvironmentError');
  PyExc_IndentationError := ImportPyFunc('PyExc_IndentationError');
  PyExc_NotImplementedError := ImportPyFunc('PyExc_NotImplementedError');
  PyExc_OSError := ImportPyFunc('PyExc_OSError');
  PyExc_TabError := ImportPyFunc('PyExc_TabError');
  PyExc_UnboundLocalError := ImportPyFunc('PyExc_UnboundLocalError');
  PyExc_UnicodeError := ImportPyFunc('PyExc_UnicodeError');
{$IFDEF MSWINDOWS}
  PyExc_WindowsError := ImportPyFunc('PyExc_WindowsError');
{$ENDIF}
  PyExc_Warning := ImportPyFunc('PyExc_Warning');
  PyExc_DeprecationWarning := ImportPyFunc('PyExc_DeprecationWarning');
  PyExc_RuntimeWarning := ImportPyFunc('PyExc_RuntimeWarning');
  PyExc_SyntaxWarning := ImportPyFunc('PyExc_SyntaxWarning');
  PyExc_UserWarning := ImportPyFunc('PyExc_UserWarning');
  PyExc_ReferenceError := ImportPyFunc('PyExc_ReferenceError');
  PyExc_StopIteration := ImportPyFunc('PyExc_StopIteration');
  PyExc_FutureWarning := ImportPyFunc('PyExc_FutureWarning');
  PyExc_PendingDeprecationWarning := ImportPyFunc('PyExc_PendingDeprecationWarning');
  PyExc_UnicodeDecodeError := ImportPyFunc('PyExc_UnicodeDecodeError');
  PyExc_UnicodeEncodeError := ImportPyFunc('PyExc_UnicodeEncodeError');
  PyExc_UnicodeTranslateError := ImportPyFunc('PyExc_UnicodeTranslateError');
  PyType_Type := ImportPyFunc('PyType_Type');
  PyCFunction_Type := ImportPyFunc('PyCFunction_Type');
  PyCode_Type := ImportPyFunc('PyCode_Type');
  PyComplex_Type := ImportPyFunc('PyComplex_Type');
  PyDict_Type := ImportPyFunc('PyDict_Type');
  PyFloat_Type := ImportPyFunc('PyFloat_Type');
  PyFrame_Type := ImportPyFunc('PyFrame_Type');
  PyFunction_Type := ImportPyFunc('PyFunction_Type');
  PyList_Type := ImportPyFunc('PyList_Type');
  PyLong_Type := ImportPyFunc('PyLong_Type');
  PyMethod_Type := ImportPyFunc('PyMethod_Type');
  PyModule_Type := ImportPyFunc('PyModule_Type');
  PyObject_Type := ImportPyFunc('PyObject_Type');
  PyRange_Type := ImportPyFunc('PyRange_Type');
  PySlice_Type := ImportPyFunc('PySlice_Type');
  PyString_Type := ImportPyFunc('PyBytes_Type');
  PyTuple_Type := ImportPyFunc('PyTuple_Type');
  PyUnicode_Type := ImportPyFunc('PyUnicode_Type');
  PyBaseObject_Type := ImportPyFunc('PyBaseObject_Type');
  PyCallIter_Type := ImportPyFunc('PyCallIter_Type');
  PyCell_Type := ImportPyFunc('PyCell_Type');
  PyClassMethod_Type := ImportPyFunc('PyClassMethod_Type');
  PyProperty_Type := ImportPyFunc('PyProperty_Type');
  PySeqIter_Type := ImportPyFunc('PySeqIter_Type');
  PyStaticMethod_Type := ImportPyFunc('PyStaticMethod_Type');
  PySuper_Type := ImportPyFunc('PySuper_Type');
  PyTraceBack_Type := ImportPyFunc('PyTraceBack_Type');
  PyWrapperDescr_Type := ImportPyFunc('PyWrapperDescr_Type');
  _PyWeakref_RefType := ImportPyFunc('_PyWeakref_RefType');
  _PyWeakref_ProxyType := ImportPyFunc('_PyWeakref_ProxyType');
  _PyWeakref_CallableProxyType := ImportPyFunc('_PyWeakref_CallableProxyType');
  PyBool_Type := ImportPyFunc('PyBool_Type');
  PyEnum_Type := ImportPyFunc('PyEnum_Type');

  //@PyArg_GetObject           := ImportPyFunc('PyArg_GetObject');
  //@PyArg_GetLong             := ImportPyFunc('PyArg_GetLong');
  //@PyArg_GetShort            := ImportPyFunc('PyArg_GetShort');
  //@PyArg_GetFloat            := ImportPyFunc('PyArg_GetFloat');
  //@PyArg_GetString           := ImportPyFunc('PyArg_GetString');
  //@PyArgs_VaParse            := ImportPyFunc('PyArgs_VaParse');
  //@Py_VaBuildValue           := ImportPyFunc('Py_VaBuildValue');
  //@PyBuiltin_Init            := ImportPyFunc('PyBuiltin_Init');
  PyComplex_FromCComplex := ImportPyFunc('PyComplex_FromCComplex');
  PyComplex_FromDoubles := ImportPyFunc('PyComplex_FromDoubles');
  PyComplex_RealAsDouble := ImportPyFunc('PyComplex_RealAsDouble');
  PyComplex_ImagAsDouble := ImportPyFunc('PyComplex_ImagAsDouble');
  PyComplex_AsCComplex := ImportPyFunc('PyComplex_AsCComplex');
  PyCFunction_GetFunction := ImportPyFunc('PyCFunction_GetFunction');
  PyCFunction_GetSelf := ImportPyFunc('PyCFunction_GetSelf');
  PyCallable_Check := ImportPyFunc('PyCallable_Check');
  PyDict_GetItem := ImportPyFunc('PyDict_GetItem');
  PyDict_SetItem := ImportPyFunc('PyDict_SetItem');
  PyDict_DelItem := ImportPyFunc('PyDict_DelItem');
  PyDict_Clear := ImportPyFunc('PyDict_Clear');
  PyDict_Next := ImportPyFunc('PyDict_Next');
  PyDict_Keys := ImportPyFunc('PyDict_Keys');
  PyDict_Values := ImportPyFunc('PyDict_Values');
  PyDict_Items := ImportPyFunc('PyDict_Items');
  PyDict_Size := ImportPyFunc('PyDict_Size');
  PyDict_DelItemString := ImportPyFunc('PyDict_DelItemString');
  PyDict_Copy := ImportPyFunc('PyDict_Copy');
  PyDictProxy_New := ImportPyFunc('PyDictProxy_New');
  PyModule_Create2 := ImportPyFunc('PyModule_Create2');
  PyErr_Print := ImportPyFunc('PyErr_Print');
  PyErr_SetNone := ImportPyFunc('PyErr_SetNone');
  PyErr_SetObject := ImportPyFunc('PyErr_SetObject');
  PyErr_Restore := ImportPyFunc('PyErr_Restore');
  PyErr_BadArgument := ImportPyFunc('PyErr_BadArgument');
  PyErr_NoMemory := ImportPyFunc('PyErr_NoMemory');
  PyErr_SetFromErrno := ImportPyFunc('PyErr_SetFromErrno');
  PyErr_BadInternalCall := ImportPyFunc('PyErr_BadInternalCall');
  PyErr_CheckSignals := ImportPyFunc('PyErr_CheckSignals');
  PyErr_Occurred := ImportPyFunc('PyErr_Occurred');
  PyErr_Clear := ImportPyFunc('PyErr_Clear');
  PyErr_Fetch := ImportPyFunc('PyErr_Fetch');
  PyErr_SetString := ImportPyFunc('PyErr_SetString');
  PyEval_GetBuiltins := ImportPyFunc('PyEval_GetBuiltins');
  PyImport_GetModuleDict := ImportPyFunc('PyImport_GetModuleDict');
  PyInt_FromLong := ImportPyFunc('PyLong_FromLong');
  PyArg_Parse := ImportPyFunc('PyArg_Parse');
  PyArg_ParseTuple := ImportPyFunc('PyArg_ParseTuple');
  Py_BuildValue := ImportPyFunc('Py_BuildValue');
  Py_Initialize := ImportPyFunc('Py_Initialize');
  PyDict_New := ImportPyFunc('PyDict_New');
  PyDict_SetItemString := ImportPyFunc('PyDict_SetItemString');
  PyModule_GetDict := ImportPyFunc('PyModule_GetDict');
  PyObject_Str := ImportPyFunc('PyObject_Str');
  PyRun_String := ImportPyFunc('PyRun_String');
  PyRun_SimpleString := ImportPyFunc('PyRun_SimpleString');
  PyDict_GetItemString := ImportPyFunc('PyDict_GetItemString');
  PyString_AsString := ImportPyFunc('PyBytes_AsString');
  PySys_SetArgv3000 := ImportPyFunc('PySys_SetArgv');
  Py_Exit := ImportPyFunc('Py_Exit');

  PyCFunction_NewEx := ImportPyFunc('PyCFunction_NewEx');

  PyEval_CallObjectWithKeywords := ImportPyFunc('PyEval_CallObjectWithKeywords');
  PyEval_GetFrame := ImportPyFunc('PyEval_GetFrame');
  PyEval_GetGlobals := ImportPyFunc('PyEval_GetGlobals');
  PyEval_GetLocals := ImportPyFunc('PyEval_GetLocals');
  //@PyEval_GetOwner           :=ImportPyFunc('PyEval_GetOwner');
  PyEval_InitThreads := ImportPyFunc('PyEval_InitThreads');
  PyEval_RestoreThread := ImportPyFunc('PyEval_RestoreThread');
  PyEval_SaveThread := ImportPyFunc('PyEval_SaveThread');
  PyFile_GetLine := ImportPyFunc('PyFile_GetLine');
  PyFile_WriteObject := ImportPyFunc('PyFile_WriteObject');
  PyFile_WriteString := ImportPyFunc('PyFile_WriteString');
  PyFloat_AsDouble := ImportPyFunc('PyFloat_AsDouble');
  PyFloat_FromDouble := ImportPyFunc('PyFloat_FromDouble');
  PyFunction_GetCode := ImportPyFunc('PyFunction_GetCode');
  PyFunction_GetGlobals := ImportPyFunc('PyFunction_GetGlobals');
  PyFunction_New := ImportPyFunc('PyFunction_New');
  PyImport_AddModule := ImportPyFunc('PyImport_AddModule');
  PyImport_Cleanup := ImportPyFunc('PyImport_Cleanup');
  PyImport_GetMagicNumber := ImportPyFunc('PyImport_GetMagicNumber');
  PyImport_ImportFrozenModule := ImportPyFunc('PyImport_ImportFrozenModule');
  PyImport_ImportModule := ImportPyFunc('PyImport_ImportModule');
  PyImport_Import := ImportPyFunc('PyImport_Import');
  //PyImport_Init             :=ImportPyFunc('PyImport_Init');
  PyImport_ReloadModule := ImportPyFunc('PyImport_ReloadModule');
  PyInt_AsLong := ImportPyFunc('PyLong_AsLong');
  PyList_Append := ImportPyFunc('PyList_Append');
  PyList_AsTuple := ImportPyFunc('PyList_AsTuple');
  PyList_GetItem := ImportPyFunc('PyList_GetItem');
  PyList_GetSlice := ImportPyFunc('PyList_GetSlice');
  PyList_Insert := ImportPyFunc('PyList_Insert');
  PyList_New := ImportPyFunc('PyList_New');
  PyList_Reverse := ImportPyFunc('PyList_Reverse');
  PyList_SetItem := ImportPyFunc('PyList_SetItem');
  PyList_SetSlice := ImportPyFunc('PyList_SetSlice');
  PyList_Size := ImportPyFunc('PyList_Size');
  PyList_Sort := ImportPyFunc('PyList_Sort');
  PyLong_AsDouble := ImportPyFunc('PyLong_AsDouble');
  PyLong_AsLong := ImportPyFunc('PyLong_AsLong');
  PyLong_FromDouble := ImportPyFunc('PyLong_FromDouble');
  PyLong_FromLong := ImportPyFunc('PyLong_FromLong');
  PyLong_FromString := ImportPyFunc('PyLong_FromString');
  PyLong_FromString := ImportPyFunc('PyLong_FromString');
  PyLong_FromUnsignedLong := ImportPyFunc('PyLong_FromUnsignedLong');
  PyLong_AsUnsignedLong := ImportPyFunc('PyLong_AsUnsignedLong');
  PyLong_FromUnicode := ImportPyFunc('PyLong_FromUnicode');
  PyLong_FromLongLong := ImportPyFunc('PyLong_FromLongLong');
  PyLong_AsLongLong := ImportPyFunc('PyLong_AsLongLong');
  PyMapping_Check := ImportPyFunc('PyMapping_Check');
  PyMapping_GetItemString := ImportPyFunc('PyMapping_GetItemString');
  PyMapping_HasKey := ImportPyFunc('PyMapping_HasKey');
  PyMapping_HasKeyString := ImportPyFunc('PyMapping_HasKeyString');
  PyMapping_Length := ImportPyFunc('PyMapping_Length');
  PyMapping_SetItemString := ImportPyFunc('PyMapping_SetItemString');
  PyMethod_Function := ImportPyFunc('PyMethod_Function');
  PyMethod_New := ImportPyFunc('PyMethod_New');
  PyMethod_Self := ImportPyFunc('PyMethod_Self');
  PyModule_GetName := ImportPyFunc('PyModule_GetName');
  PyModule_New := ImportPyFunc('PyModule_New');
  PyNumber_Absolute := ImportPyFunc('PyNumber_Absolute');
  PyNumber_Add := ImportPyFunc('PyNumber_Add');
  PyNumber_And := ImportPyFunc('PyNumber_And');
  PyNumber_Check := ImportPyFunc('PyNumber_Check');
  PyNumber_Divide := ImportPyFunc('PyNumber_TrueDivide');
  PyNumber_FloorDivide := ImportPyFunc('PyNumber_FloorDivide');
  PyNumber_TrueDivide := ImportPyFunc('PyNumber_TrueDivide');
  PyNumber_Divmod := ImportPyFunc('PyNumber_Divmod');
  PyNumber_Float := ImportPyFunc('PyNumber_Float');
  PyNumber_Invert := ImportPyFunc('PyNumber_Invert');
  PyNumber_Long := ImportPyFunc('PyNumber_Long');
  PyNumber_Lshift := ImportPyFunc('PyNumber_Lshift');
  PyNumber_Multiply := ImportPyFunc('PyNumber_Multiply');
  PyNumber_Negative := ImportPyFunc('PyNumber_Negative');
  PyNumber_Or := ImportPyFunc('PyNumber_Or');
  PyNumber_Positive := ImportPyFunc('PyNumber_Positive');
  PyNumber_Power := ImportPyFunc('PyNumber_Power');
  PyNumber_Remainder := ImportPyFunc('PyNumber_Remainder');
  PyNumber_Rshift := ImportPyFunc('PyNumber_Rshift');
  PyNumber_Subtract := ImportPyFunc('PyNumber_Subtract');
  PyNumber_Xor := ImportPyFunc('PyNumber_Xor');
  PyOS_InitInterrupts := ImportPyFunc('PyOS_InitInterrupts');
  PyOS_InterruptOccurred := ImportPyFunc('PyOS_InterruptOccurred');
  PyObject_CallObject := ImportPyFunc('PyObject_CallObject');
  PyObject_CallMethod := ImportPyFunc('PyObject_CallMethod');
  PyObject_CallMethodStr := ImportPyFunc('PyObject_CallMethod');
  PyObject_RichCompare := ImportPyFunc('PyObject_RichCompare');
  PyObject_RichCompareBool := ImportPyFunc('PyObject_RichCompareBool');
  PyObject_GetAttr := ImportPyFunc('PyObject_GetAttr');
  PyObject_GetAttrString := ImportPyFunc('PyObject_GetAttrString');
  PyObject_GetItem := ImportPyFunc('PyObject_GetItem');
  PyObject_DelItem := ImportPyFunc('PyObject_DelItem');
  PyObject_HasAttrString := ImportPyFunc('PyObject_HasAttrString');
  PyObject_Hash := ImportPyFunc('PyObject_Hash');
  PyObject_IsTrue := ImportPyFunc('PyObject_IsTrue');
  PyObject_Length := ImportPyFunc('PyObject_Length');
  PyObject_Repr := ImportPyFunc('PyObject_Repr');
  PyObject_SetAttr := ImportPyFunc('PyObject_SetAttr');
  PyObject_SetAttrString := ImportPyFunc('PyObject_SetAttrString');
  PyObject_SetItem := ImportPyFunc('PyObject_SetItem');
  PyObject_Init := ImportPyFunc('PyObject_Init');
  PyObject_InitVar := ImportPyFunc('PyObject_InitVar');
  PyObject_New := ImportPyFunc('_PyObject_New');
  PyObject_NewVar := ImportPyFunc('_PyObject_NewVar');
  PyObject_Free := ImportPyFunc('PyObject_Free');
  PyObject_GetIter := ImportPyFunc('PyObject_GetIter');
  PyIter_Next := ImportPyFunc('PyIter_Next');
  PyObject_IsInstance := ImportPyFunc('PyObject_IsInstance');
  PyObject_IsSubclass := ImportPyFunc('PyObject_IsSubclass');
  PyObject_Call := ImportPyFunc('PyObject_Call');
  PyObject_GenericGetAttr := ImportPyFunc('PyObject_GenericGetAttr');
  PyObject_GenericSetAttr := ImportPyFunc('PyObject_GenericSetAttr');
  PyObject_GC_Malloc := ImportPyFunc('_PyObject_GC_Malloc');
  PyObject_GC_New := ImportPyFunc('_PyObject_GC_New');
  PyObject_GC_NewVar := ImportPyFunc('_PyObject_GC_NewVar');
  PyObject_GC_Resize := ImportPyFunc('_PyObject_GC_Resize');
  PyObject_GC_Del := ImportPyFunc('PyObject_GC_Del');
  PyObject_GC_Track := ImportPyFunc('PyObject_GC_Track');
  PyObject_GC_UnTrack := ImportPyFunc('PyObject_GC_UnTrack');
  PySequence_Check := ImportPyFunc('PySequence_Check');
  PySequence_Concat := ImportPyFunc('PySequence_Concat');
  PySequence_Count := ImportPyFunc('PySequence_Count');
  PySequence_GetItem := ImportPyFunc('PySequence_GetItem');
  PySequence_GetSlice := ImportPyFunc('PySequence_GetSlice');
  PySequence_In := ImportPyFunc('PySequence_In');
  PySequence_Index := ImportPyFunc('PySequence_Index');
  PySequence_Length := ImportPyFunc('PySequence_Length');
  PySequence_Repeat := ImportPyFunc('PySequence_Repeat');
  PySequence_SetItem := ImportPyFunc('PySequence_SetItem');
  PySequence_SetSlice := ImportPyFunc('PySequence_SetSlice');
  PySequence_DelSlice := ImportPyFunc('PySequence_DelSlice');
  PySequence_Tuple := ImportPyFunc('PySequence_Tuple');
  PySequence_Contains := ImportPyFunc('PySequence_Contains');
  PySlice_GetIndices := ImportPyFunc('PySlice_GetIndices');
  PySeqIter_New := ImportPyFunc('PySeqIter_New');
  PySlice_GetIndicesEx := ImportPyFunc('PySlice_GetIndicesEx');
  PySlice_New := ImportPyFunc('PySlice_New');
  PyString_Concat := ImportPyFunc('PyBytes_Concat');
  PyString_ConcatAndDel := ImportPyFunc('PyBytes_ConcatAndDel');
  PyString_FromStringAndSize := ImportPyFunc('PyBytes_FromStringAndSize');
  PyString_Size := ImportPyFunc('PyBytes_Size');
  PyString_DecodeEscape := ImportPyFunc('PyBytes_DecodeEscape');
  PyString_Repr := ImportPyFunc('PyBytes_Repr');
  PySys_GetObject := ImportPyFunc('PySys_GetObject');
  //PySys_Init                :=ImportPyFunc('PySys_Init');
  PySys_SetObject := ImportPyFunc('PySys_SetObject');
  PySys_SetPath := ImportPyFunc('PySys_SetPath');
  //PyTraceBack_Fetch         :=ImportPyFunc('PyTraceBack_Fetch');
  PyTraceBack_Here := ImportPyFunc('PyTraceBack_Here');
  PyTraceBack_Print := ImportPyFunc('PyTraceBack_Print');
  //@PyTraceBack_Store         :=ImportPyFunc('PyTraceBack_Store');
  PyTuple_GetItem := ImportPyFunc('PyTuple_GetItem');
  PyTuple_GetSlice := ImportPyFunc('PyTuple_GetSlice');
  PyTuple_New := ImportPyFunc('PyTuple_New');
  PyTuple_SetItem := ImportPyFunc('PyTuple_SetItem');
  PyTuple_Size := ImportPyFunc('PyTuple_Size');
  PyType_IsSubtype := ImportPyFunc('PyType_IsSubtype');
  PyType_GenericAlloc := ImportPyFunc('PyType_GenericAlloc');
  PyType_GenericNew := ImportPyFunc('PyType_GenericNew');
  PyType_Ready := ImportPyFunc('PyType_Ready');
  PyUnicode_FromWideChar := ImportPyFunc(AnsiString(Format('PyUnicode%s_FromWideChar', [GetUnicodeTypeSuffix])));
  PyUnicode_AsWideChar := ImportPyFunc(AnsiString(Format('PyUnicode%s_AsWideChar', [GetUnicodeTypeSuffix])));
  PyUnicode_Decode := ImportPyFunc(AnsiString(Format('PyUnicode%s_Decode', [GetUnicodeTypeSuffix])));
  PyUnicode_AsEncodedString := ImportPyFunc(AnsiString(Format('PyUnicode%s_AsEncodedString', [GetUnicodeTypeSuffix])));
  PyUnicode_FromOrdinal := ImportPyFunc(AnsiString(Format('PyUnicode%s_FromOrdinal', [GetUnicodeTypeSuffix])));
  PyUnicode_FromString := ImportPyFunc(AnsiString(Format('PyUnicode%s_FromString', [GetUnicodeTypeSuffix])));
  PyUnicode_GetSize := ImportPyFunc(AnsiString(Format('PyUnicode%s_GetSize', [GetUnicodeTypeSuffix])));
  PyWeakref_GetObject := ImportPyFunc('PyWeakref_GetObject');
  PyWeakref_NewProxy := ImportPyFunc('PyWeakref_NewProxy');
  PyWeakref_NewRef := ImportPyFunc('PyWeakref_NewRef');
  PyWrapper_New := ImportPyFunc('PyWrapper_New');
  PyBool_FromLong := ImportPyFunc('PyBool_FromLong');
  PyThreadState_SetAsyncExc := ImportPyFunc('PyThreadState_SetAsyncExc');
  Py_AtExit := ImportPyFunc('Py_AtExit');
  //Py_Cleanup                :=ImportPyFunc('Py_Cleanup');
  Py_CompileString := ImportPyFunc('Py_CompileString');
  Py_FatalError := ImportPyFunc('Py_FatalError');
  _PyObject_New := ImportPyFunc('_PyObject_New');
  _PyString_Resize := ImportPyFunc('_PyBytes_Resize');
  Py_Finalize := ImportPyFunc('Py_Finalize');
  if getProcAddress(DLL_handle, 'PyCode_Addr2Line') <> nil then
    DLL_PyCode_Addr2Line := ImportPyFunc('PyCode_Addr2Line');
  if getProcAddress(DLL_handle, 'PyImportPyFunc_ExecCodeModule') <> nil then
    DLL_PyImport_ExecCodeModule := ImportPyFunc('DLL_PyImport_ExecCodeModule');
  //@PyClass_IsSubclass         :=ImportPyFunc('PyClass_IsSubclass');
  PyErr_ExceptionMatches := ImportPyFunc('PyErr_ExceptionMatches');
  PyErr_GivenExceptionMatches := ImportPyFunc('PyErr_GivenExceptionMatches');
  PyEval_EvalCode := ImportPyFunc('PyEval_EvalCode');
  Py_GetVersion := ImportPyFunc('Py_GetVersion');
  Py_GetCopyright := ImportPyFunc('Py_GetCopyright');
  Py_GetExecPrefix := ImportPyFunc('Py_GetExecPrefix');
  Py_GetPath := ImportPyFunc('Py_GetPath');
  Py_GetPrefix := ImportPyFunc('Py_GetPrefix');
  Py_GetProgramName := ImportPyFunc('Py_GetProgramName');
  PyParser_SimpleParseString := ImportPyFunc('PyParser_SimpleParseString');
  PyNode_Free := ImportPyFunc('PyNode_Free');
  PyErr_NewException := ImportPyFunc('PyErr_NewException');
  /// jah 29-sep-2000 : updated for python 2.0
  ///                   replaced Py_Malloc with PyMem_Malloc
  ///---   @Py_Malloc := ImportPyFunc ('Py_Malloc');
  ///+++   @Py_Malloc := ImportPyFunc ('PyMem_Malloc');
  try
    Py_Malloc := ImportPyFunc('PyMem_Malloc');
    PyMem_Malloc := ImportPyFunc('PyMem_Malloc');
  except
  end;
  Py_SetProgramName := ImportPyFunc('Py_SetProgramName');
  Py_SetProgramName3000 := ImportPyFunc('Py_SetProgramName');
  Py_IsInitialized := ImportPyFunc('Py_IsInitialized');
  Py_GetProgramFullPath := ImportPyFunc('Py_GetProgramFullPath');
  if getProcAddress(DLL_handle, 'Py_GetBuildInfo') <> nil then
    DLL_Py_GetBuildInfo := ImportPyFunc('Py_GetBuildInfo');
  Py_NewInterpreter := ImportPyFunc('Py_NewInterpreter');
  Py_EndInterpreter := ImportPyFunc('Py_EndInterpreter');
  PyEval_AcquireLock := ImportPyFunc('PyEval_AcquireLock');
  PyEval_ReleaseLock := ImportPyFunc('PyEval_ReleaseLock');
  PyEval_AcquireThread := ImportPyFunc('PyEval_AcquireThread');
  PyEval_ReleaseThread := ImportPyFunc('PyEval_ReleaseThread');
  PyInterpreterState_New := ImportPyFunc('PyInterpreterState_New');
  PyInterpreterState_Clear := ImportPyFunc('PyInterpreterState_Clear');
  PyInterpreterState_Delete := ImportPyFunc('PyInterpreterState_Delete');
  PyThreadState_New := ImportPyFunc('PyThreadState_New');
  PyThreadState_Clear := ImportPyFunc('PyThreadState_Clear');
  PyThreadState_Delete := ImportPyFunc('PyThreadState_Delete');
  PyThreadState_Get := ImportPyFunc('PyThreadState_Get');
  PyThreadState_Swap := ImportPyFunc('PyThreadState_Swap');
  PyErr_SetInterrupt := ImportPyFunc('PyErr_SetInterrupt');
  PyGILState_Ensure := ImportPyFunc('PyGILState_Ensure');
  PyGILState_Release := ImportPyFunc('PyGILState_Release');

end.
