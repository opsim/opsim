program extended_embedding;

uses
  SysUtils,
  Python;

var
  pName, pModule, pDict, pFunc: pPyObject;
  pArgs, pValue: pPyObject;
  i: integer;
  numargs: integer = 0;

  (* Return the number of arguments of the application command line *)
  function emb_numargs(self, args: pPyObject): pPyObject; cdecl;
  begin
    if PyArg_ParseTuple(args, ':numargs') = 0 then
      exit(nil);
    exit(PyLong_FromLong(numargs));
  end;

var
  EmbMethods: array [0..1] of PyMethodDef = (
    (ml_name: 'numargs';
     ml_meth: @emb_numargs;
     ml_flags: METH_VARARGS;
     ml_doc: 'Return the number of arguments received by the process.'),
    (ml_name: nil;
     ml_meth: nil;
     ml_flags: 0;
     ml_doc: nil)
    );

  EmbModule: array [0..0] of PyModuleDef = (
    (m_base: (ob_refcnt: 0;
              ob_type: nil;
              m_init: nil;
              m_index: 0;
              m_copy: nil);
     m_name: 'emb';
     m_doc: nil;
     m_size: -1;
     m_methods: @EmbMethods;
     m_reload: nil;
     m_traverse: nil;
     m_clear: nil;
     m_free: nil)
    );

  function PyInit_emb(): pPyObject; cdecl;
  begin
    exit(PyModule_Create2(@EmbModule, 1013));
  end;

begin
  if (argc < 3) then
  begin
    writeln(stderr, 'Usage: call pythonfile funcname [args]');
    halt(1);
  end;

  numargs := argc;
  PyImport_AppendInittab('emb', @PyInit_emb);

  Py_Initialize();
  pName := PyUnicode_FromString(argv[1]);
  (* Error checking of pName left out *)

  pModule := PyImport_Import(pName);
  Py_DECREF(pName);

  if pModule <> nil then
  begin
    pFunc := PyObject_GetAttrString(pModule, argv[2]);
    (* pFunc is a new reference *)

    if (pFunc <> nil) and (PyCallable_Check(pFunc) <> 0) then
    begin
      pArgs := PyTuple_New(argc - 3);
      for i := 0 to argc - 4 do
      begin
        pValue := PyLong_FromLong(StrToInt(argv[i + 3]));
        if pValue = nil then
        begin
          Py_DECREF(pArgs);
          Py_DECREF(pModule);
          writeln(stderr, 'Cannot convert argument');
          halt(1);
        end;
        (* pValue reference stolen here: *)
        PyTuple_SetItem(pArgs, i, pValue);
      end;
      pValue := PyObject_CallObject(pFunc, pArgs);
      Py_DECREF(pArgs);
      if pValue <> nil then
      begin
        writeln('Result of call: ', PyLong_AsLong(pValue));
        Py_DECREF(pValue);
      end
      else
      begin
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        PyErr_Print();
        writeln(stderr, 'Call failed');
        halt(1);
      end;
    end
    else
    begin
      if PyErr_Occurred() <> nil then
        PyErr_Print();
      writeln(stderr, 'Cannot find function ', argv[2]);
    end;
    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
  end
  else
  begin
    PyErr_Print();
    writeln(stderr, 'Failed to load ', argv[1]);
    halt(1);
  end;

  Py_Finalize();
end.
