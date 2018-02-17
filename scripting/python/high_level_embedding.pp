program high_level_embedding;

uses
  Python;

begin
  Py_SetProgramName(argv[0]);  (* optional but recommended *)
  Py_Initialize();
  PyRun_SimpleString('from time import time,ctime' + #13#10 +
                     'print(''Today is'', ctime(time()))');
  Py_Finalize();
end.
