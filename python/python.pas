unit Python;

interface

{$mode Delphi}

uses
  ctypes;

type
  psize_t = ^size_t;
  wchar_t = cint32;
  pwchar_t = ^wchar_t;

var
  Py_Initialize : procedure; cdecl;
  Py_DecodeLocale : function (const arg: pChar; size: psize_t): pwchar_t; cdecl;
  Py_SetProgramName : procedure (name: pwchar_t);
  PyRun_SimpleString : function (command : PChar) : integer;
  Py_Finalize : procedure; cdecl;
  PyMem_RawFree : procedure (p : pointer);

implementation

uses
  DynLibs;

var
  DLL_handle: THandle;

function ImportPyFunc(const funcname: ansistring; canFail: boolean = True): Pointer;
begin
  Result := GetProcAddress(DLL_handle, PAnsiChar(funcname));
  if (Result = nil) and canFail then
  begin
    writeln('Error: could not find symbol "', funcname, '"');
    halt(1);
    //E := EDllImportError.CreateFmt('Error: could not find symbol "%s"', [funcname]); //AT
    ////E.ErrorCode := GetLastError;
    //E.WrongFunc := funcname;
    //raise E;
  end;
end;

initialization
  DLL_handle := LoadLibrary('python35.dll');

  Py_Initialize := ImportPyFunc('Py_Initialize');
  Py_DecodeLocale := ImportPyFunc('Py_DecodeLocale');
  Py_SetProgramName := ImportPyFunc('Py_SetProgramName');
  PyRun_SimpleString := ImportPyFunc('PyRun_SimpleString');
  Py_Finalize := ImportPyFunc('Py_Finalize');
  PyMem_RawFree := ImportPyFunc('PyMem_RawFree');

end.
