uses
  Cs2, Cs2_Var;
var
  p : Pchar;
  f : File;
  cs : TCs2PascalScript;

function UtilProc(ID: Pointer; const ProcName: string; Params: PVariableManager;res: PCajVariant): TCS2Error;
begin
  if procname='WRITELN' then begin
    writeln(VM_Get(Params, 0)^.cv_Str);
  end;
  utilproc:=ENoError;
end;

function MyOnUses(Id: Pointer; Sender: PCs2PascalScript; Name: string): TCs2Error;
begin
  if name='SYSTEM' then begin
    MyOnUses:=ENoError;
    RegisterStdLib(Sender);
    PM_Add(Sender^.Procedures, '0 WRITELN S 8', @utilProc);

  end else
    MyOnUses:=EUnknownIdentifier;
end;


begin
  assign(f, 'fpctest.ps');
  reset(f, 1);
  getmem(p, filesize(f)+1);
  p[filesize(F)]:=#0;
  blockread(f, p^, filesize(f));
  close(f);
  cs.Create(nil);
  cs.OnUses:=@MyOnUses;
  cs.SetText(p);
  cs.RunScript;
  if cs.ErrorCode<>ENoError then begin
    writeln('Error:'+ErrorToString(cs.ErrorCode),' at:',cs.ErrorPos);
  end;
  cs.Destroy;
  FreeMem(p);
end.