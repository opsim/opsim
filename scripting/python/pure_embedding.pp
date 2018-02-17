program pure_embedding;

uses
  SysUtils,
  Python;

var
  pName, pModule, pDict, pFunc: pPyObject;
  pArgs, pValue: pPyObject;
  i: integer;

begin
  if (argc < 3) then
  begin
    writeln(stderr, 'Usage: call pythonfile funcname [args]');
    halt(1);
  end;

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
