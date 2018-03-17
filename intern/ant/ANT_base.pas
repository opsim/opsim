unit ANT_base;

{$mode objfpc}

interface

uses
  DNA_listbase,
  ANT_types,
  MEM_guardedalloc;

const
  ANT_PLATFORM_ERROR = 1;

var
  errfunc: ANT_ErrorCallback = nil;
  windowlist: ListBase;
  gWindowWidth: integer = 640;
  gWindowHeight: integer = 480;

{
   This procedure is used to message errors in the ANT code. It can be redirected in the user application.
   @param error: error constant
   @param fmt: error string
   @param args: arguments passed to the error message
}
procedure antError(error: integer; const fmt: string; const args: array of const);

implementation

uses
  SysUtils;

procedure antError(error: integer; const fmt: string; const args: array of const);
begin
  if assigned(errfunc) then
    errfunc(error, format(fmt, args));
end;

end.

