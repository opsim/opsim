library PyMinMod;
 
{
 
  Minimal Python module (library) that includes simple functions.
 
  Author: Phil (MacPgmr at fastermac.net).
 
  To compile this module:
    - With Delphi: Open this .dpr file and compile.
    - With Lazarus: Open .lpi file and compile.
 
  To deploy module:
    - With Delphi: Rename compiled .dll to .pyd.
    - With Lazarus on Windows: Rename compiled .so to .pyd.
    - With Lazarus on OS X and Linux: .so extension is okay.
 
}
 
uses
  SysUtils,
  python;
 
 
function SumTwoIntegers(Self : PyObject;
                        Args : PyObject) : PyObject; cdecl;
var
  Arg1 : Integer;
  Arg2 : Integer;
begin
  PyArg_ParseTuple(Args, 'ii', @Arg1, @Arg2);  //Get the two int arguments
  Result := PyInt_FromLong(Arg1 + Arg2);  //Add them together and return sum
//  Result := PyLong_FromLong(Arg1 + Arg2);
//  Result := PyLong_FromUnsignedLong(Arg1 + Arg2);
end;
 
 
function ConcatTwoStrings(Self : PyObject;
                          Args : PyObject) : PyObject; cdecl;
 {From Python documentation for "s" format: "You must not provide storage for 
   the string itself; a pointer to an existing string is stored into the 
   character pointer variable whose address you pass."
  From Python documentation for PyString_FromString: "Return a new string 
   object with a copy of the string v as value on success".}
var
  Arg1 : PAnsiChar;
  Arg2 : PAnsiChar;
begin
  PyArg_ParseTuple(Args, 'ss', @Arg1, @Arg2);  //Get the two string arguments
  Result := PyString_FromString(PAnsiChar(AnsiString(Arg1) + AnsiString(Arg2)));  
             //Concatenate and return string
end;
 
 
var
  Methods : packed array [0..2] of PyMethodDef;
 
procedure initPyMinMod; cdecl;
begin
  Methods[0].name := 'SumTwoIntegers';
  Methods[0].meth := @SumTwoIntegers;
  Methods[0].flags := METH_VARARGS;
  Methods[0].doc := 'Tests passing ints to and from module function';
 
  Methods[1].name := 'ConcatTwoStrings';
  Methods[1].meth := @ConcatTwoStrings;
  Methods[1].flags := METH_VARARGS;
  Methods[1].doc := 'Tests passing strings to and from module function';
 
  Methods[2].name := nil;
  Methods[2].meth := nil;
  Methods[2].flags := 0;
  Methods[2].doc := nil;
 
  Py_InitModule('PyMinMod', Methods[0]);
end;
 
 
exports
  initPyMinMod;
 
end.
