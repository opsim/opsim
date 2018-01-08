//see: https://docs.python.org/3/extending/embedding.html
program high_level_embedding;

uses
  Python;

var
  _program: pwchar_t;

begin
  _program := Py_DecodeLocale(argv[0], nil);
  if _program = nil then
  begin
    writeln(stderr, 'Fatal error: cannot decode argv[0]');
    halt(1);
  end;
  Py_SetProgramName(_program);  (* optional but recommended *)
  Py_Initialize();
  PyRun_SimpleString('from time import time,ctime' + #13#10 +
                     'print(''Today is'', ctime(time()))');
  Py_Finalize();
  PyMem_RawFree(_program);

  readln;
end.
