{
Cajscript 2 PascalScript
version: 2.16

Parts ready:
 - Calculation
 - Assignments (a:=b;)
 - External Procedure/Function calls
 - Sub Begins
 - If Then Else
 - Internal Procedure/Functions
 - Variable parameters for internal and extenal functions.
 - Internal Procedure calls from outside the script.
 - Documentation and examples
 - For/To/Downto/Do
 - Cajsoft STDLib
 - While/Begin/End
 - Case x Of/End
 - Array (Dynamic, only one dimensional)

To do:
 - PCode
 - Object orientation
 - Repeat/Until
}
unit CS2; {CajScript 2.0}
{$I CS2_DEF.INC}
interface
uses
  CS2_VAR, CS2_UTL;

type
{$IFDEF CLASS}
  TCs2PascalScript = class;
  PCs2PascalScript = TCs2PascalScript;
{$ELSE}
  PCs2PascalScript = ^TCs2PascalScript;
{$ENDIF}
  TOnUses = function(Id: Pointer; Sender: PCs2PascalScript; Name: string):
  TCs2Error;
  TOnRunLine = function(Id: Pointer; Sender: PCs2PascalScript; Position:
    Longint): TCs2Error;
  TCs2PascalScript = {$IFDEF CLASS}class{$ELSE}object{$ENDIF}
  Private
    FUses: TifStringList;
    InternalProcedures: PProcedureManager;

    Text: PChar;
    MainOffset: LongInt;
    FId: Pointer;
    Parser: PCs2PascalParser;
    FErrorPos: LongInt;
    FErrorCode: TCs2Error;
{$IFDEF CLASS}
    FOnUses: TOnUses;
    FOnRunLine: TOnRunLine;
{$ENDIF}
    function CalcArrayInt(Vars: PVariableManager; var W: PCajVariant): Boolean;
    function IdentifierExists(SubVars: PVariableManager; const S: string):
    Boolean;
    function ProcessVars(Vars: PVariableManager): Boolean;
    procedure RunError(C: TCs2Error);
    function RunBegin(Vars: PVariableManager; Skip: Boolean): Boolean;
    function Calc(Vars: PVariableManager; res: PCajVariant;
      StopOn: TCs2TokenId): Boolean;
    function DoProc(Vars: PVariableManager; Internal: Boolean): PCajVariant;
  Public
    Variables: PVariableManager;
    Procedures: PProcedureManager;
{$IFDEF CLASS}
    property GetText: Pchar read Text;
    property OnRunLine: TOnRunLine Read FOnRunLine Write FOnRunLine;
    property OnUses: TOnUses Read FOnUses Write FOnUses;
    property ErrorCode: TCs2Error Read FErrorCode;
    property ErrorPos: LongInt Read FErrorPos;
{$ELSE}
    OnUses: TOnUses;
    OnRunLine: TOnRunLine;
    function ErrorCode: TCs2Error;
    function ErrorPos: LongInt;
{$ENDIF}
    procedure RunScript;
    function RunScriptProc(const Name: string;
      Parameters: PVariableManager): PCajVariant;

    procedure SetText(p: Pchar);
    constructor Create(Id: Pointer);
    destructor Destroy; {$IFDEF CLASS}Override; {$ENDIF}
  end;

procedure RegisterStdLib(P: PCs2PascalScript);
{Register all standard functions:}
{
Install:
  Function StrGet(S : String; I : Integer) : Char;
  Function StrSet(c : Char; I : Integer; var s : String) : Char;
  Function Ord(C : Char) : Byte;
  Function Chr(B : Byte) : Char;
  Function StrToInt(s : string;def : Longint) : Longint;
  Function IntToStr(i : Longint) : String;
  Function Uppercase(s : string) : string;
  Function Copy(S : String; Indx, Count : Integer) : String;
  Procedure Delete(var S : String; Indx, Count : Integer);
  Function Pos(SubStr, S : String) : Integer;
  Procedure Insert(Source : String; var Dest : String; Indx : Integer);
  Procedure SetArrayLength(var Arr : Array; I : Longint);
  Function GetArrayLength(var Arr : Array) : Longint;
  Function Length(s : String) : Longint;
  Function Sin(e : Extended) : Extended;
  Function Cos(e : Extended) : Extended;
  Function Round(e : Extended) : Longint;
  Function Trunc(e : Extended) : Longint;
  Function Pi : Extended;
}

implementation
type
  TGTyperec = record
    ident: string[20];
    typeid: Word;
  end;
const
  GTypes: array[1..16] of TGTyperec = (
    (Ident: 'BYTE'; typeid: CSV_UByte),
    (Ident: 'SHORTINT'; typeid: CSV_SByte),
    (Ident: 'CHAR'; typeid: CSV_Char),
    (Ident: 'WORD'; typeid: CSV_UInt16),
    (Ident: 'SMALLINT'; typeid: CSV_SInt16),
    (Ident: 'CARDINAL'; typeid: CSV_UInt32),
    (Ident: 'LONGINT'; typeid: CSV_SInt32),
    (Ident: 'INTEGER'; typeid: CSV_SInt32),
    (Ident: 'STRING'; typeid: CSV_String),
    (Ident: 'REAL'; typeid: CSV_Real),
    (Ident: 'SINGLE'; typeid: CSV_Single),
    (Ident: 'DOUBLE'; typeid: CSV_Double),
    (Ident: 'EXTENDED'; typeid: CSV_Extended),
    (Ident: 'COMP'; typeid: CSV_Comp),
    (Ident: 'BOOLEAN'; typeid: CSV_Bool),
    (Ident: 'ARRAY'; typeid: CSV_Array));

function GetType(const s: string): Word;
var
  i: Integer;
begin
  for i := 1 to 16 do begin
    if s = GTypes[i].Ident then
    begin
      GetType := GTypes[i].typeid;
      Exit;
    end;
  end;
  GetType := 0;
end;

function IntToStr(I: LongInt): string;
var
  s: string;
begin
  Str(i, s);
  IntToStr := s;
end;

function StrToInt(const S: string): LongInt;
var
  e: Integer;
  Res: LongInt;
begin
  Val(S, Res, e);
  if e <> 0 then
    StrToInt := -1
  else
    StrToInt := Res;
end;

function StrToIntDef(const S: string; Def: LongInt): LongInt;
var
  e: Integer;
  Res: LongInt;
begin
  Val(S, Res, e);
  if e <> 0 then
    StrToIntDef := Def
  else
    StrToIntDef := Res;
end;

function StrToReal(const S: string): Extended;
var
  e: Integer;
  Res: Extended;
begin
  Val(S, Res, e);
  if e <> 0 then
    StrToReal := -1
  else
    StrToReal := Res;
end;


function IntProcDefParam(S: string; I: Integer): Integer;
{
Parse the incode-script procedure definition from a string.
When I=0 this function will return the result type.
When I=-1 this function will return the number of parameters.
When I=1 this function will return the first parameter type.
When I=2 this function will return the second parameter type.
etc.
}
var
  Res: Integer;
begin
  if I = 0 then
  {Return result-type} IntProcDefParam := StrToInt(Fw(s)) else
    if I = -1 then
    {Return param count} begin
      res := 0;
      Delete(S, 1, Length(Fw(s))); {result}
      Rs(S);
      Delete(S, 1, Length(Fw(s))); {name}
      Rs(S);
      while Length(s) > 0 do begin
        Inc(Res);
        Delete(S, 1, Length(Fw(s))); {Delete parameter name}
        Rs(S);
        Delete(S, 1, Length(Fw(s))); {Delete parameter type}
        Rs(S);
      end; {while}
      IntProcDefParam := Res;
    end {else if} else begin
      res := 0;
      if I < 1 then
      begin IntProcDefParam := -1; Exit; end;
      Delete(S, 1, Length(Fw(s))); {result}
      Rs(S);
      Delete(S, 1, Length(Fw(s))); {name}
      Rs(S);
      while Length(s) > 0 do begin
        Inc(Res);
        Delete(S, 1, Length(Fw(s))); {delete parameter name}
        Rs(S);
        if Res = I then
        begin IntProcDefParam := StrToInt(Fw(s)); Exit; end;
        Delete(S, 1, Length(Fw(s))); {delete type}
        Rs(S);
      end; {while}
      IntProcDefParam := 0;
    end {Else Else if}
end; {IntProcDefParam}

function IntProcDefName(S: string; I: Integer): string;
{
Parse the incode-script procedure definition from a string.
i=0 will return the procedure name
I=1 will return the first one
}
var
  Res: Integer;
begin
  res := 0;
  if i = 0 then
  begin
    Delete(S, 1, Length(Fw(s))); {result}
    Rs(S);
    IntProcDefName := fw(s);
    Exit;
  end;
  if I < 1 then
  begin IntProcDefName := ''; Exit; end;
  Delete(S, 1, Length(Fw(s))); {result}
  Rs(S);
  Delete(S, 1, Length(Fw(s))); {name}
  Rs(S);
  while Length(s) > 0 do begin
    Inc(Res);
    if Res = I then
    begin IntProcDefName := Fw(s); Exit; end;
    Delete(S, 1, Length(Fw(s))); {delete parameter name}
    Rs(S);
    Delete(S, 1, Length(Fw(s))); {delete type}
    Rs(S);
  end; {while}
  IntProcDefName := '';
end; {IntProcDefParam}

function TCs2PascalScript.CalcArrayInt(Vars: PVariableManager; var W:
  PCajVariant): Boolean;
{Calculate array [integer]}
var
  r: PCajVariant;
begin
  CalcArrayInt := False;
  while w^.VType = CSV_Array do begin
    NextNoJunk(Parser);
    if Parser^.CurrTokenId <> CSTI_OpenBlock then
    begin
      RunError(EOpenBlockExpected);
      Exit;
    end; {if}
    NextNoJunk(Parser);
    r := CreateCajVariant(CSV_SInt32, 0);
    if not Calc(Vars, r, CSTI_CloseBlock) then
    begin
      DestroyCajVariant(r);
      Exit;
    end; {if}
    if Parser^.CurrTokenID <> CSTI_CloseBlock then
    begin
      RunError(ECloseBlockExpected);
      DestroyCajVariant(r);
      Exit;
    end; {if}
    w := GetArrayItem(w, r^.CV_SInt32);
    DestroyCajVariant(r);
    if not Assigned(w) then
    begin
      RunError(EOutOfRange);
      Exit;
    end; {if}
  end;
  CalcArrayInt := True;
end; {CalcArrayInt}

function TCs2PascalScript.IdentifierExists(SubVars: PVariableManager; const S:
  string): Boolean;
{ Check if an identifier exists }
  function UsesExists(s: string): Boolean;
  var
    i: Integer;
  begin
    UsesExists := False;
    for i := 0 to FUses.Count - 1 do
      if FUses.GetItem(i) = s then
      begin
        UsesExists := True;
        Break;
      end;
  end; { UsesExists }

begin
  IdentifierExists := False;
  if UsesExists(FastUppercase(s)) then
    IdentifierExists := True
  else if PM_Find(Procedures, FastUppercase(s)) <> -1 then
    IdentifierExists := True
  else if PM_Find(InternalProcedures, FastUppercase(s)) <> -1 then
    IdentifierExists := True
  else if VM_Find(Variables, FastUppercase(s)) <> -1 then
    IdentifierExists := True
  else if GetType(FastUppercase(s)) <> 0 then
    IdentifierExists := True
  else if Assigned(SubVars) and (VM_Find(subVars, FastUppercase(s)) <> -1) then
  begin
    IdentifierExists := True
  end;
end; {IdentifierExists}


procedure TCs2PascalScript.SetText(p: PChar);
{ Assign a text to the script engine, this also checks for uses and variables. }
var
  HaveHadProgram,
    HaveHadUses: Boolean;

  function ProcessUses: Boolean;
  {Process Uses block}
  var
    i: Integer;
  begin
    ProcessUses := False;
    while Parser^.CurrTokenId <> CSTI_EOF do begin
      if Parser^.CurrTokenId <> CSTI_Identifier then
      begin
        RunError(EIdentifierExpected);
        Exit;
      end; {If}
      if IdentifierExists(nil, GetToken(Parser)) then
      begin
        RunError(EDuplicateIdentifier);
        Exit;
      end; {If}
      FUses.Add(FastUpperCase(GetToken(Parser)));
      if Assigned(OnUses) then
      begin
        i := OnUses(FId, {$IFNDEF CLASS}@{$ENDIF}Self, GetToken(Parser));
        if I <> ENoError then
        begin
          RunError(i);
          Exit;
        end; {If}
      end {If}
      else
      begin
        RunError(EUnknownIdentifier);
        Exit;
      end; {Else if}
      NextNoJunk(Parser);
      if (Parser^.CurrTokenId = CSTI_SemiColon) then
      begin
        NextNoJunk(Parser);
        Break;
      end {if}
      else if (Parser^.CurrTokenId <> CSTI_Comma) then
      begin
        RunError(EDuplicateIdentifier);
        Exit;
      end; {Else if}
    end;
    if Parser^.CurrTokenId = CSTI_EOF then
    begin
      RunError(EUnexpectedEndOfFile);
    end {If}
    else
    begin
      ProcessUses := True;
    end; {Else If}
  end; {ProcessUses}

  function DoFuncHeader: Boolean;
  var
    FuncParam: string;
    FuncName: string;
    CurrVar: string;
    CurrType: Word;
    FuncRes: Word;
    function Duplic(S: string): Boolean;
    var
      s2, s3: string;
      i: Integer;
    begin
      if s = FuncName then
      begin
        Duplic := True;
        Exit;
      end; {if}
      if (funcRes <> 0) and (s = 'RESULT') then
      begin
        duplic := True;
        Exit;
      end;
      s2 := CurrVar;
      while Pos('|', s2) > 0 do begin
        if Pos('!', s2) = 1 then
          Delete(s2, 1, 1);
        if Copy(s2, 1, Pos('|', s2) - 1) = s then
        begin
          Duplic := True;
          Exit;
        end; {if}
        Delete(s2, 1, Pos('|', s2));
      end; {while}
      s2 := '0 ' + FuncParam;
      for i := 1 to IntProcDefParam(s2, -1) do begin
        s3 := IntProcDefName(s2, 0);
        if Pos('!', s2) = 1 then
          Delete(s2, 1, 1);
        if s3 = s then
        begin
          Duplic := True;
          Exit;
        end; {if}
      end; {for}
      Duplic := False;
    end; {duplic}
  begin
    DoFuncHeader := False;
    if Parser^.CurrTokenId = CSTII_Procedure then
      FuncRes := 0
    else
      FuncRes := 1;
    NextNoJunk(Parser);
    if Parser^.CurrTokenId <> CSTI_Identifier then
    begin
      RunError(EIdentifierExpected);
      Exit;
    end; {if}
    if IdentifierExists(nil, GetToken(Parser)) then
    begin
      RunError(EDuplicateIdentifier);
      Exit;
    end; {if}
    FuncName := FastUppercase(GetToken(Parser));
    FuncParam := FuncName;
    CurrVar := '';
    NextNoJunk(Parser);
    if parser^.CurrTokenId = CSTI_OpenRound then
    begin
      while True do begin
        NextNoJunk(Parser);
        if Parser^.CurrTokenId = CSTII_Var then
        begin
          CurrVar := '!';
          NextNoJunk(Parser);
        end; {if}
        while True do begin
          if Parser^.CurrTokenId <> CSTI_Identifier then
          begin
            RunError(EIdentifierExpected);
            Exit;
          end; {if}
          if IdentifierExists(nil, GetToken(Parser)) or Duplic(GetToken(Parser))
            then
          begin
            RunError(EDuplicateIdentifier);
            Exit;
          end; {if}
          CurrVar := CurrVar + fastuppercase(GetToken(Parser)) + '|';
          NextNoJunk(parser);
          if Parser^.CurrTokenId = CSTI_Colon then
            Break;
          if Parser^.CurrTokenId <> CSTI_Comma then
          begin
            RunError(ECommaExpected);
            Exit;
          end; {if}
          NextNoJunk(Parser);
        end; {while}
        NextNoJunk(Parser);
        CurrType := GetType(FastUppercase(GetToken(Parser)));
        if CurrType = 0 then
        begin
          RunError(EUnknownIdentifier);
          Exit;
        end; {if}
        if Pos('!', CurrVar) = 1 then
        begin
          Delete(currVar, 1, 1);
          while Pos('|', CurrVar) > 0 do begin
            FuncParam := FuncParam + ' !' + Copy(CurrVar, 1, Pos('|', CurrVar) -
              1) + ' ' + IntToStr(CurrType);
            Delete(CurrVar, 1, Pos('|', CurrVar));
          end; {while}
        end else
        begin
          while Pos('|', CurrVar) > 0 do begin
            FuncParam := FuncParam + ' ' + Copy(CurrVar, 1, Pos('|', CurrVar) -
              1) + ' ' + IntToStr(CurrType);
            Delete(CurrVar, 1, Pos('|', CurrVar));
          end; {while}
        end; {if}
        NextNoJunk(Parser);
        if Parser^.CurrTokenId = CSTI_CloseRound then
        begin
          NextNoJunk(Parser);
          Break;
        end; {if}
        if Parser^.CurrTokenId <> CSTI_SemiColon then
        begin
          RunError(ESemiColonExpected);
          Exit;
        end; {if}
        NextNoJunk(Parser);
      end; {while}
    end; {if}
    if FuncRes = 1 then
    begin
      if Parser^.CurrTokenId <> CSTI_Colon then
      begin
        RunError(EColonExpected);
        Exit;
      end;
      NextNoJunk(Parser);
      if Parser^.CurrTokenId <> CSTI_Identifier then
      begin
        RunError(EIdentifierExpected);
        Exit;
      end;
      FuncRes := GetType(FastUppercase(GetToken(Parser)));
      if FuncRes = 0 then
      begin
        RunError(EUnknownIdentifier);
        Exit;
      end;
      NextNoJunk(parser);
    end;
    FuncParam := InttoStr(FuncRes) + ' ' + FuncParam;
    if Parser^.CurrTokenId <> CSTI_Semicolon then
    begin
      RunError(ESemiColonExpected);
      Exit;
    end;
    NextNoJunk(Parser);
    PM_Add(InternalProcedures, FuncParam, Pointer(Parser^.CurrTokenPos));
    DoFuncHeader := True;
    if Parser^.CurrTokenId = CSTII_Var then
    begin
      while (Parser^.CurrTokenID <> CSTII_Begin) and (Parser^.CurrTokenID <>
        CSTI_EOF) do
        NextNoJunk(Parser);
    end;
    RunBegin(nil, True);
    if Parser^.CurrTokenId <> CSTI_Semicolon then
    begin
      RunError(ESemiColonExpected);
      Exit;
    end;
    NextNoJunk(Parser);
  end; {DoFuncHeader}

begin
  FUses.Clear;
  VM_Clear(Variables);
  Vm_Add(Variables, CreateBool(True), 'TRUE')^.Flags := 1;
  Vm_Add(Variables, CreateBool(False), 'FALSE')^.Flags := 1;
  PM_Clear(Procedures);
  PM_Clear(InternalProcedures);
  FUses.Add('SYSTEM');
  if Assigned(OnUses) then
    OnUses(fId, {$IFNDEF CLASS}@{$ENDIF}Self, 'SYSTEM');

  RunError(ENoError);
  MainOffset := -1;
  Text := p;
  if Text = nil then
  begin
    Exit;
  end; {If}
  Parser^.Text := Text;
  Parser^.CurrTokenPos := 0;
  HaveHadProgram := False;
  HaveHadUses := False;
  ParseToken(Parser);
  if (Parser^.CurrTokenId = CSTI_WhiteSpace) or (Parser^.CurrTokenId =
    CSTI_Comment) then
    NextNoJunk(Parser);
  while Parser^.CurrTokenId <> CSTI_EOF do begin
    case Parser^.CurrTokenId of
      CSTI_CommentEOFError,
        CSTI_CharError,
        CSTI_SyntaxError: begin RunError(ESyntaxError); Exit; end;
      CSTI_StringError: begin RunError(EStringError); Exit; end;
    end;
    if (Parser^.CurrTokenId = CSTII_Program) and (HaveHadProgram = False) and
      (HaveHadUses = False) then
    begin
      NextNoJunk(Parser);
      if Parser^.CurrTokenId <> CSTI_Identifier then
      begin
        RunError(EIdentifierExpected);
        Exit;
      end; {if}
      NextNoJunk(Parser);
      if Parser^.CurrTokenId <> CSTI_Semicolon then
      begin
        RunError(ESemicolonExpected);
        Exit;
      end; {if}
      NextNoJunk(Parser);
      HaveHadProgram := True;
    end {if}
    else if (Parser^.CurrTokenId = CSTII_Uses) and (HaveHadUses = False) then
    begin
      NextNoJunk(Parser);
      if not ProcessUses then
        Exit;
      HaveHadUses := True;
    end {else if}
    else if (Parser^.CurrTokenId = CSTII_Var) then
    begin
      if not ProcessVars(Variables) then
        Exit;
    end {Else if}
    else if (Parser^.CurrTokenId = CSTII_Procedure) or
      (Parser^.CurrTokenId = CSTII_Function)
      then
    begin
      if not DoFuncHeader then
        Exit;
    end {else if}
    else if (Parser^.CurrTokenId = CSTII_Begin) then
    begin
      MainOffset := Parser^.CurrTokenPos;
      Exit;
    end {Else if}
    else if (Parser^.CurrTokenId = CSTI_EOF) then
    begin
      RunError(EUnexpectedEndOfFile);
    end {Else if}
    else
    begin
      RunError(EBeginExpected);
      Exit;
    end; {Else If}
  end; {While}
end; {SetText}


function TCs2PascalScript.ProcessVars(Vars: PVariableManager): Boolean;
        { Process Vars block }
var
  Names: string;
  AType: Word;
  ArrVType: Word;
begin
  NextNojunk(Parser);
  Names := '';
  ProcessVars := False;
  while True do begin
    case Parser^.CurrTokenId of
      CSTI_CommentEOFError,
        CSTI_CharError,
        CSTI_SyntaxError: begin RunError(ESyntaxError); Exit; end;
      CSTI_StringError: begin RunError(EStringError); Exit; end;
      CSTI_EOF: begin RunError(EUnexpectedEndOfFile); Exit; end;
    end;
    if Parser^.CurrTokenId <> CSTI_Identifier then
    begin
      RunError(EIdentifierExpected);
      Exit;
    end;
    if IdentifierExists(Vars, GetToken(Parser)) then
    begin
      RunError(EDuplicateIdentifier);
      Exit;
    end; {if}

    Names := Names + FastUpperCase(GetToken(Parser)) + '|';
    NextNoJunk(Parser);
    while Parser^.CurrTokenId = CSTI_Comma do begin
      NextNoJunk(Parser);
      if Parser^.CurrTokenId <> CSTI_Identifier then
      begin
        RunError(EIdentifierExpected);
        Exit;
      end; {if}
      if IdentifierExists(nil, GetToken(Parser)) then
      begin
        RunError(EDuplicateIdentifier);
        Exit;
      end; {if}
      Names := Names + FastUpperCase(GetToken(Parser)) + '|';
      NextNoJunk(Parser);
    end; {while}
    if Parser^.CurrTokenId <> CSTI_Colon then
    begin
      RunError(EColonExpected);
      Exit;
    end; {if}
    NextNoJunk(Parser);
    if Parser^.CurrTokenId = CSTI_Identifier then
    begin
      AType := GetType(FastUpperCase(GetToken(Parser)));
      if AType = 0 then
      begin
        RunError(EUnknownIdentifier);
        Exit;
      end; {if}
      if AType = CSV_Array then
      begin
        NextNoJunk(Parser);
        if Parser^.CurrTokenId <> CSTII_Of then
        begin
          RunError(EOfExpected);
          Exit;
        end;
        NextNoJunk(Parser);
        ArrVType := GetType(FastUpperCase(GetToken(Parser)));
        if ArrVType = 0 then
        begin
          RunError(EUnknownIdentifier);
          Exit;
        end; {if}
      end else ArrVType := 0;
      while Pos('|', names) > 0 do begin
        VM_Add(Vars, CreateCajVariant(AType, ArrVType), Copy(names, 1, Pos('|',
          names) - 1));
        Delete(Names, 1, Pos('|', Names));
      end; {if}
    end {else if}
    else
    begin
      RunError(EIdentifierExpected);
      Exit;
    end; {if}
    NextNoJunk(Parser);
    if Parser^.CurrTokenId <> CSTI_Semicolon then
    begin
      RunError(ESemicolonExpected);
      Exit;
    end; {if}
    NextNoJunk(Parser);
    if Parser^.CurrTokenId <> CSTI_Identifier then
      Break;
  end; {while}
  ProcessVars := True;
end; {ProcessVars}

constructor TCs2PascalScript.Create(Id: Pointer);
begin
{$IFDEF CLASS}
  inherited Create;
{$ENDIF}
  FUses.Create;
  New(Parser);
  FId := Id;
  RunError(ENoError);
  Text := nil;
  MainOffset := -1;
  Procedures := PM_Create;
  InternalProcedures := PM_Create;
  Variables := VM_Create(nil);
  OnUses := nil;
  OnRunLine := nil;
end; {Create}

destructor TCs2PascalScript.Destroy;
begin
  Dispose(Parser);
  VM_Destroy(Variables);
  PM_Destroy(InternalProcedures);
  PM_Destroy(Procedures);
  FUses.Destroy;
{$IFDEF CLASS}
  inherited Destroy;
{$ENDIF}
end; {Create}

{$IFNDEF CLASS}

function TCs2PascalScript.ErrorCode: TCs2Error;
{ Return the error code }
begin
  ErrorCode := FErrorCode;
end; {Errorcode}

function TCs2PascalScript.ErrorPos: LongInt;
{ Return the error position }
begin
  ErrorPos := FErrorPos;
end; {ErrorPos}

{$ENDIF}

procedure TCs2PascalScript.RunError(C: TCs2Error);
{ Run an error }
begin
  if c = ENoError then
  begin
    FErrorCode := C;
    FErrorPos := -1;
  end {if}
  else
  begin
    FErrorCode := C;
    FErrorPos := Parser^.CurrTokenPos;
  end; {else if}
end; {RunError}

procedure TCs2PascalScript.RunScript;
{ Run the script! }
begin
  if MainOffset = -1 then
  begin
    Exit;
  end; {if}
  RunError(ENoError);
  Parser^.CurrTokenPos := MainOffset;
  ParseToken(Parser);
  if RunBegin(nil, False) then
  begin
    if Parser^.CurrTokenId <> CSTI_Period then
    begin
      RunError(EPeriodExpected);
    end;
  end;
end; {RunScript}

type
  PCajSmallCalculation = ^TCajSmallCalculation;
  TCajSmallCalculation = packed record
    TType: Byte;
                                  {
                                  0 = Variant

                                  2 = *
                                  3 = /
                                  4 = DIV
                                  5 = MOD
                                  6 = AND
                                  7 = SHR
                                  8 = SHL

                                  9 = +
                                  10 = -
                                  11 = OR
                                  12 = XOR

                                  13 = =
                                  14 = >
                                  15 = <
                                  16 = <>
                                  17 = <=
                                  18 = >=
                                  }
    CajVariant: PCajVariant;
  end;

function TCs2PascalScript.Calc(Vars: PVariableManager; res: PCajVariant; StopOn:
  TCs2TokenId): Boolean;
{ Calculate an expression }
var
  Items: TifList;
  PreCalc: string;
  temp4: PCajVariant;
  Work: PCajSmallCalculation;
  function ChrToStr(s: string): Char;
    {Turn a char intto a string}
  begin
    Delete(s, 1, 1); {First char : #}
    ChrToStr := Chr(StrToInt(s));
  end;
  function PString(s: string): string;
    { remove the ' from the strings}
  begin
    s := Copy(s, 2, Length(s) - 2);
    PString := s;
  end;
  function DoPrecalc: Boolean;
    {Pre calculate (- not +)}
  begin
    DoPrecalc := True;
    while Length(Precalc) > 0 do begin
      if precalc[1] = '-' then
      begin
        if not DoMinus(Work^.CajVariant) then
        begin
          RunError(ETypeMismatch);
          Exit;
        end;
      end else if precalc[1] = '|' then
      begin
        if not DoNot(Work^.CajVariant) then
        begin
          RunError(ETypeMismatch);
          Exit;
        end;
      end else if precalc[1] = '+' then
      begin
        {plus has no effect}
      end else
      begin
        DoPreCalc := False;
        Exit;
      end;
      Delete(PreCalc, 1, 1);
    end;
  end;

  procedure DisposeList;
    { Dispose the items }
  var
    i: Integer;
    p: PCajSmallCalculation;
  begin
    for i := 0 to Items.Count - 1 do begin
      p := items.GetItem(i);
      if p^.TType = 0 then
        DestroyCajVariant(p^.CajVariant);
      Dispose(p);
    end;
    Items.Destroy;
  end;
  function ParseString: string;
    { Parse a string }
  var
    temp3: string;
  begin
    temp3 := '';
    while (Parser^.CurrTokenId = CSTI_String) or (Parser^.CurrTokenId =
      CSTI_Char) do begin
      if Parser^.CurrTokenId = CSTI_String then
      begin
        temp3 := temp3 + PString(GetToken(Parser));
        NextNoJunk(Parser);
        if Parser^.CurrTokenId = CSTI_String then
          temp3 := temp3 + #39;
      end {if}
      else
      begin
        temp3 := temp3 + ChrToStr(GetToken(Parser));
        NextnoJunk(parser);
      end; {else if}
    end; {while}
    ParseString := temp3;
  end;
  procedure Calculate;
    { Calculate the full expression }
  var
    l: PCajSmallCalculation;
    i: LongInt;
  begin
    i := 0;
    while i < (items.count - 1) div 2 do begin
      l := PCajSmallCalculation(items.GetItem(i * 2 + 1));
      if (l^.TType >= 2) and (l^.TType <= 8) then
      begin
        case l^.TType of
          2: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtMul)
            then
              RunError(ETypeMismatch);
          3: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtDiv)
            then
              RunError(ETypeMismatch);
          4: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtIntDiv)
            then
              RunError(ETypeMismatch);
          5: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtIntMod)
            then
              RunError(ETypeMismatch);
          6: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtAnd)
            then
              RunError(ETypeMismatch);
          7: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtShr)
            then
              RunError(ETypeMismatch);
          8: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtShl)
            then
              RunError(ETypeMismatch);
        end;
        if ErrorCode <> 0 then
          Exit;
        l := PCajSmallCalculation(items.GetItem(i * 2 + 2));
        DestroycajVariant(l^.CajVariant);
        Dispose(l);
        Items.Remove(l);
        l := PCajSmallCalculation(items.GetItem(i * 2 + 1));
        Dispose(l);
        Items.Remove(l);
      end else Inc(i);
    end;

    i := 0;
    while i < (items.count - 1) div 2 do begin
      l := PCajSmallCalculation(items
        .GetItem(i * 2 + 1));
      if (l^.TType >= 9) and (l^.TType <= 12) then
      begin
        case l^.TType of
          9: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtPlus)
            then
              RunError(ETypeMismatch);
          10: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtMinus)
            then
              RunError(ETypeMismatch);
          11: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtOr)
            then
              RunError(ETypeMismatch);
          12: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtXor)
            then
              RunError(ETypeMismatch);
        end;
        if ErrorCode <> 0 then
          Exit;
        l := PCajSmallCalculation(items.GetItem(i * 2 + 2));
        DestroycajVariant(l^.CajVariant);
        Dispose(l);
        Items.Remove(l);
        l := PCajSmallCalculation(items
          .GetItem(i * 2 + 1));
        Dispose(l);
        Items.Remove(l);
      end else Inc(i);
    end;
    i := 0;
    while i < (items.count - 1) div 2 do begin
      l := PCajSmallCalculation(items
        .GetItem(i * 2 + 1));
      if (l^.TType >= 13) and (l^.TType <= 18) then
      begin
        case l^.TType of
          13: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtEqual)
            then
              RunError(ETypeMismatch);
          14: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtGreater)
            then
              RunError(ETypeMismatch);
          15: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtLess)
            then
              RunError(ETypeMismatch);
          16: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtNotEqual)
            then
              RunError(ETypeMismatch);
          17: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtLessEqual)
            then
              RunError(ETypeMismatch);
          18: if not Perform(PCajSmallCalculation(items.GetItem(i * 2))^.
              CajVariant, PCajSmallCalculation(items.GetItem(i * 2 + 2))
              ^.CajVariant, PtGreaterEqual)
            then
              RunError(ETypeMismatch);
        end;
        if ErrorCode <> 0 then
          Exit;
        l := PCajSmallCalculation(items.GetItem(i * 2 + 2));
        DestroycajVariant(l^.CajVariant);
        Dispose(l);
        Items.Remove(l);
        l := PCajSmallCalculation(items.GetItem(i * 2 + 1));
        Dispose(l);
        Items.Remove(l);
      end else Inc(i);
    end;
  end;
begin
  Items.Create;
  Calc := False;
  Res := GetVarLink(res);
  while True do begin
    if Parser^.CurrTokenId = StopOn then
      Break;
    case Parser^.CurrTokenId of
      CSTII_Else,
        CSTII_To,
        CSTII_DownTo,
        CSTII_do,
        CSTI_Semicolon,
        CSTII_End,
        CSTI_Comma,
        CSTI_CloseRound:
        begin
          Break;
        end; {Csti_Else...}
      CSTI_EOF:
        begin
          RunError(EUnexpectedEndOfFile);
          DisposeList;
          Exit;
        end; {CSTI_Eof}
      CSTI_SyntaxError,
        CSTI_CommentEOFError,
        CSTI_CharError:
        begin
          RunError(ESyntaxError);
          DisposeList;
          Exit;
        end; {Csti_SyntaxError...}
      CSTI_StringError:
        begin
          RunError(EStringError);
          DisposeList;
          Exit;
        end; {csti_Stringerror}
    end; {case}
    if (Items.Count and $1) = 0 then
    begin
      PreCalc := '';
      while (Parser^.CurrTokenId = CSTI_Minus) or
        (Parser^.CurrTokenId = CSTII_Not) or
        (Parser^.CurrTokenId = CSTI_Plus)
        do begin
        if (Parser^.CurrTokenId = CSTI_Minus) then
          PreCalc := PreCalc + '-';
        if (Parser^.CurrTokenId = CSTII_Not) then
          PreCalc := PreCalc + '|';
        if (Parser^.CurrTokenId = CSTI_Plus) then
          PreCalc := PreCalc + '+';
        NextNoJunk(Parser);
      end; {While}

      New(Work);
      case Parser^.CurrTokenId of
        CSTI_OpenRound:
          begin
            NextNoJunk(Parser);
            Work^.CajVariant := CreateCajVariant(CSV_Var, 0);
            Work^.CajVariant^.Cv_Var := nil;
            Work^.TType := 0;
            if not Calc(vars, Work^.CajVariant, CSTI_CloseRound) then
            begin
              DestroyCajVariant(Work^.CajVariant);
              Dispose(Work);
              DisposeList;
              Exit;
            end; {if}
            if not DoPreCalc then
            begin
              DestroyCajVariant(Work^.CajVariant);
              Dispose(Work);
              DisposeList;
              Exit;
            end; {if}
            NextNoJunk(Parser);
            Items.Add(Work);
          end; {CSTI_OpenRound}
        CSTI_Identifier:
          begin
            if Assigned(vars) and (Vm_Find(Vars, FastUppercase(GetToken(Parser)))
              <> -1) then
            begin
              Temp4 := GetVarLink(Vm_Get(Vars, Vm_Find(Vars,
                FastUppercase(GetToken(Parser)))));
              if not CalcArrayInt(Vars, Temp4) then
                Exit;
              NextNoJunk(Parser);
              Work^.CajVariant := CreateCajVariant(Temp4^.VType, 0);
              Work^.TType := 0;
              if not PerForm(Work^.CajVariant, Temp4, ptSet) then
              begin
                DestroyCajVariant(Work^.CajVariant);
                Dispose(Work);
                DisposeList;
                Exit;
              end; {if}
            end {if}
            else if Vm_Find(Variables, FastUppercase(GetToken(Parser))) <> -1
              then
            begin
              Temp4 := GetVarLink(Vm_Get(Variables,
                Vm_Find(Variables, FastUppercase(GetToken(Parser)))));
              if not CalcArrayInt(Vars, Temp4) then
                Exit;
              NextNoJunk(Parser);
              Work^.CajVariant := CreateCajVariant(Temp4^.VType, 0);
              Work^.TType := 0;
              if not PerForm(Work^.CajVariant, Temp4, ptSet) then
              begin
                DestroyCajVariant(Work^.CajVariant);
                Dispose(Work);
                DisposeList;
                Exit;
              end; {if}
            end {if}
            else if PM_Find(Procedures, FastUpperCase(GetToken(Parser))) <> -1
              then
            begin
              Temp4 := DoProc(vars, False);
              if Temp4 = nil then
              begin
                Dispose(Work);
                DisposeList;
                Exit;
              end; {if}
              Work^.CajVariant := CreateCajVariant(Temp4^.VType, 0);
              Work^.TType := 0;
              PerForm(Work^.CajVariant, Temp4, ptSet);
            end {else if}
            else if PM_Find(InternalProcedures, FastUpperCase(GetToken(Parser)))
              <> -1 then
            begin
              Temp4 := DoProc(vars, True);
              if ErrorCode <> ENoError then
              begin
                Dispose(Work);
                DisposeList;
                Exit;
              end; {if}
              Work^.CajVariant := CreateCajVariant(Temp4^.VType, 0);
              Work^.TType := 0;
              PerForm(Work^.CajVariant, Temp4, ptSet);
            end {else if}
            else
            begin
              RunError(EUnknownIdentifier);
              Dispose(Work);
              DisposeList;
              Exit;
            end; {else else if}
            Items.Add(Work);
          end; {CSTI_Identifier}
        CSTI_Integer:
          begin
            if ((Res^.VType >= csv_SByte) and (Res^.VType <= Csv_SInt32)) or
              ((res^.VType >= csv_Real) and (res^.vtype <= csv_comp)) then
              Work^.CajVariant := CreateCajVariant(res^.VType, 0)
            else
              Work^.CajVariant := CreateCajVariant(csv_SInt32, 0);
            Work^.TType := 0;
            if IsRealType(work^.CajVariant) then
              SetReal(Work^.CajVariant, StrToInt(GetToken(Parser)))
            else
              SetInteger(Work^.CajVariant, StrToInt(GetToken(Parser)));
            if not DoPreCalc then
            begin
              DestroyCajVariant(Work^.CajVariant);
              Dispose(Work);
              DisposeList;
              Exit;
            end; {if}
            NextNoJunk(Parser);
            Items.Add(Work);
          end; {CSTI_Integer}
        CSTI_Real:
          begin
            if (Res^.VType >= CSV_Real) and (Res^.VType <= CSV_Comp) then
              Work^.CajVariant := CreateCajVariant(res^.VType, 0)
            else
              Work^.CajVariant := CreateCajVariant(CSV_Extended, 0);
            Work^.TType := 0;
            SetReal(Work^.CajVariant, StrToReal(GetToken(Parser)));
            if not DoPreCalc then
            begin
              DestroyCajVariant(Work^.CajVariant);
              Dispose(Work);
              DisposeList;
              Exit;
            end;
            NextNoJunk(Parser);
            Items.Add(Work);
          end; {CSTI_Real}
        CSTI_String, CSTI_Char:
          begin
            Work^.CajVariant := CreateCajVariant(CSV_String, 0);
            Work^.TType := 0;
            Work^.CajVariant^.CV_Str := ParseString;
            if not DoPreCalc then
            begin
              DestroyCajVariant(Work^.CajVariant);
              Dispose(Work);
              DisposeList;
              Exit;
            end; {if}
            Items.Add(Work);
          end; {CSTI_String}
        CSTI_HexInt:
          begin
            Work^.TType := 0;
            if (Res^.VType >= csv_SByte) and (Res^.VType <= Csv_SInt32) then
              Work^.CajVariant := CreateCajVariant(res^.VType, 0)
            else
              Work^.CajVariant := CreateCajVariant(csv_SInt32, 0);
            SetInteger(Work^.CajVariant, StrToInt(GetToken(Parser)));
            if not DoPreCalc then
            begin
              DestroyCajVariant(Work^.CajVariant);
              Dispose(Work);
              DisposeList;
              Exit;
            end; {if}
            NextNoJunk(Parser);
            Items.Add(Work);
          end; {CSTI_HexInt}
      else
        begin
          RunError(EErrorInExpression);
          Dispose(Work);
          DisposeList;
          Exit;
        end;
      end; {case}
    end {if}
    else
    begin
      New(Work);
      case Parser^.CurrTokenId of
        CSTI_Equal: Work^.TType := 13;
        CSTI_NotEqual: Work^.TType := 16;
        CSTI_Greater: Work^.TType := 14;
        CSTI_GreaterEqual: Work^.TType := 18;
        CSTI_Less: Work^.TType := 15;
        CSTI_LessEqual: Work^.TType := 17;
        CSTI_Plus: Work^.TType := 9;
        CSTI_Minus: Work^.TType := 10;
        CSTI_Divide:
          begin
            Work^.TType := 3;
            if Res^.VType = CSV_VAR then
              SetType(Res, CSV_Extended);
          end;
        CSTI_Multiply: Work^.TType := 2;
        CSTII_and: Work^.TType := 6;
        CSTII_div: Work^.TType := 4;
        CSTII_mod: Work^.TType := 5;
        CSTII_or: Work^.TType := 11;
        CSTII_shl: Work^.TType := 8;
        CSTII_shr: Work^.TType := 7;
        CSTII_xor: Work^.TType := 12;
      else
        begin
          RunError(EErrorInExpression);
          Dispose(Work);
          DisposeList;
          Exit;
        end; {else case}
      end; {case}
      Items.Add(Work);
      NextnoJunk(parser);
    end; {else if}
  end; {while}
  Calculate;
  if ErrorCode = 0 then
  begin
    if Items.Count <> 1 then
    begin
      RunError(EErrorInExpression);
      Calc := False;
    end else
    begin
      Work := Items.GetItem(0);
      if Perform(Res, Work^.CajVariant, PtSet) then
        Calc := True
      else RunError(ETypeMismatch);
    end; {if}
  end; {if}
  DisposeList;
end; {Calc}

function TCs2PascalScript.RunScriptProc(const Name: string; Parameters:
  PVariableManager): PCajVariant;
var
  ProcCall: LongInt;
  ProcDef: string;
  w: PCajVariant;
  i: LongInt;
  function IRem(S: string): string;
  {Remove the !}
  begin
    Delete(s, 1, 1);
    IRem := s;
  end; {irem}
begin
  RunScriptProc := nil;
  if MainOffset = -1 then
  begin
    Parser^.CurrTokenPos := -1;
    RunError(EBeginExpected);
    Exit;
  end; {if}
  if PM_Find(InternalProcedures, FastUpperCase(Name)) = -1 then
  begin
    RunError(EUnknownIdentifier);
    Exit;
  end; {if}
  ProcCall := LongInt(PM_Get(InternalProcedures, PM_Find(InternalProcedures,
    FastUpperCase(Name))));
  ProcDef := PM_GetSpec(InternalProcedures, PM_Find(InternalProcedures,
    FastUpperCase(Name)));
  if IntProcDefParam(ProcDef, -1) <> VM_Count(Parameters) then
  begin
    Parser^.CurrTokenPos := -1;
    RunError(EParameterError);
    Exit;
  end;
  for i := 1 to IntProcDefParam(ProcDef, -1) do begin
    if Pos('!', IntProcDefName(ProcDef, I)) = 1 then
    begin
      w := GetVarLink(VM_Get(Parameters, i - 1));
      if (w^.VType <> IntProcDefParam(ProcDef, I)) or ((W^.Flags and $1) <> 0)
        then
      begin
        Parser^.CurrTokenPos := I - 1;
        RunError(EParameterError);
        Exit;
      end; {if}
      VM_SetName(Parameters, I - 1, IRem(IntProcDefName(ProcDef, I)));
    end {if} else
    begin
      w := GetVarLink(VM_Get(Parameters, i - 1));
      if IntProcDefParam(ProcDef, i) <> w^.VType then
      begin
        Parser^.CurrTokenPos := I - 1;
        RunError(EParameterError);
        Exit;
      end; {if}
      VM_SetName(Parameters, I - 1, IntProcDefName(ProcDef, I));
    end; {else if}
  end; {for}
  if IntProcDefParam(ProcDef, 0) <> 0 then
  begin
    w := CreateCajVariant(IntProcDefParam(ProcDef, 0), 0);
    VM_Add(Parameters, CreateCajVariant(CSV_Var, 0), 'RESULT')^.Cv_Var := w;
  end {if}
  else w := nil;
  Parser^.CurrTokenPos := ProcCall;
  ParseToken(Parser);
  if Parser^.CurrTokenId = CSTII_Var then
  begin
    if not ProcessVars(Parameters) then
    begin
      DestroyCajVariant(w);
      Exit;
    end; {if}
  end; {if}
  if not RunBegin(Parameters, False) then
  begin
    DestroycajVariant(w);
    Exit;
  end; {if}
  ParseToken(Parser);
  RunScriptProc := w;
end;

function TCs2PascalScript.DoProc(Vars: PVariableManager; Internal: Boolean):
PCajVariant;
{Call an internal/external Procedure}
var
  ProcCall: TRegisteredProc;
  ProcCall2: LongInt;
  ProcDef: string;
  w: PCajVariant;
  i: LongInt;
  Params: PVariableManager;
  function IRem(S: string): string;
  {Remove the !}
  begin
    Delete(s, 1, 1);
    IRem := s;
  end; {irem}
begin
  DoProc := nil;
  if Internal then
  begin
    ProcCall2 := LongInt(PM_Get(InternalProcedures, PM_Find(InternalProcedures,
      FastUpperCase(GetToken(Parser)))));
    ProcDef := PM_GetSpec(InternalProcedures, PM_Find(InternalProcedures,
      FastUpperCase(GetToken(Parser))));
{$IFDEF FPC}
    ProcCall := TRegisteredProc(nil);
{$ELSE}
    ProcCall := nil;
{$ENDIF}
  end else
  begin
{$IFDEF FPC}
    ProcCall := TRegisteredProc(PM_Get(Procedures, PM_Find(Procedures,
      FastUpperCase(GetToken(Parser)))));
{$ELSE}
    @ProcCall := PM_Get(Procedures, PM_Find(Procedures,
      FastUpperCase(GetToken(Parser))));
{$ENDIF}
    ProcCall2 := 0;
    ProcDef := PM_GetSpec(Procedures, PM_Find(Procedures,
      FastUpperCase(GetToken(Parser))));
  end;
  Params := VM_Create(nil);
  NextnoJunk(Parser);
  if (IntProcDefParam(ProcDef, -1) <> 0) and (Parser^.CurrTokenId <>
    CSTI_OpenRound) then
  begin
    RunError(ERoundOpenExpected);
    VM_Destroy(params);
    Exit;
  end; {if}
  if (IntProcDefParam(ProcDef, -1) = 0) and (Parser^.CurrTokenId =
    CSTI_OpenRound) then
  begin
    RunError(ESemiColonExpected);
    VM_Destroy(params);
    Exit;
  end; {if}
  if Parser^.CurrTokenId = CSTI_OpenRound then
  begin
    for i := 1 to IntProcDefParam(ProcDef, -1) do begin
      NextNoJunk(Parser);
      if Pos('!', intProcDefName(ProcDef, i)) = 1 then
      begin
        {Expect a variable}
        if Assigned(Vars) and (VM_Find(Vars, FastUppercase(GetToken(Parser))) <>
          -1) then
          w := GetVarLink(VM_Get(Vars, VM_Find(Vars,
            FastUppercase(GetToken(Parser)))))
        else if VM_Find(Variables, FastUppercase(GetToken(Parser))) <> -1 then
          w := GetVarLink(VM_Get(Variables, VM_Find(Variables,
            FastUppercase(GetToken(Parser)))))
        else
        begin
          RunError(EVariableExpected);
          VM_Destroy(params);
          Exit;
        end; {else else if}
        if (w^.Flags and $1) <> 0 then
        begin
          RunError(EVariableExpected);
          VM_Destroy(params);
          Exit;
        end; {if}
        if IntProcDefParam(ProcDef, I) = CSV_Array then
        begin
          if w^.VType <> CSV_Array then
          begin
            RunError(ETypeMismatch);
            VM_Destroy(params);
            Exit;
          end;
        end else
        begin
          if not CalcArrayInt(Vars, w) then
          begin
            VM_Destroy(params);
            Exit;
          end;
          if w^.VType <> IntProcDefParam(ProcDef, I) then
          begin
            RunError(ETypeMismatch);
            VM_Destroy(params);
            Exit;
          end;
        end;
        VM_Add(Params, CreateCajVariant(CSV_Var, 0),
          FastUppercase(IRem(IntProcDefName(ProcDef, i))))^.Cv_var := w;
        NextNoJunk(Parser);
      end {if}
      else
      begin
        w := VM_Add(Params, CreateCajVariant(IntProcDefParam(ProcDef, i), 0),
          IntProcDefName(ProcDef, i));
        if not Calc(vars, w, CSTI_CloseRound) then
        begin
          VM_Destroy(params);
          Exit;
        end; {if}
      end; {else if}
      if i = IntProcDefParam(ProcDef, -1) then
      begin
        if parser^.CurrTokenId <> CSTI_CloseRound then
        begin
          RunError(ERoundCloseExpected);
          VM_Destroy(params);
          Exit;
        end; {if}
      end {if}
      else
      begin
        if parser^.CurrTokenId <> CSTI_Comma then
        begin
          RunError(ECommaExpected);
          VM_Destroy(params);
          Exit;
        end; {if}
      end; {else if}
    end; {for}
    NextNoJunk(Parser);
  end; {if}
  {Now we have all the parameters}
  if Internal then
  begin
    if IntProcDefParam(ProcDef, 0) <> 0 then
    begin
      w := CreateCajVariant(IntProcDefParam(ProcDef, 0), 0);
      VM_Add(Params, CreateCajVariant(CSV_Var, 0), 'RESULT')^.Cv_Var := w;
    end {if}
    else w := nil;
    i := Parser^.CurrTokenPos;
    Parser^.CurrTokenPos := ProcCall2;
    ParseToken(Parser);
    if Parser^.CurrTokenId = CSTII_Var then
    begin
      if not ProcessVars(Params) then
      begin
        DestroyCajVariant(w);
        Exit;
      end; {if}
    end; {if}
    if not RunBegin(Params, False) then
    begin
      DestroycajVariant(w);
      Exit;
    end; {if}
    Parser^.CurrTokenPos := I;
    ParseToken(Parser);
    DoProc := w;
    VM_Destroy(params);
  end {if}
  else
  begin
    if IntProcDefParam(ProcDef, 0) <> 0 then
      w := CreateCajVariant(IntProcDefParam(ProcDef, 0), 0)
    else
      w := nil;
    RunError(ProcCall(fId, IntProcDefName(ProcDef, 0), Params, w));
    if ErrorCode <> ENoError then
    begin
      VM_Destroy(params);
      DestroyCajVariant(w);
      Exit;
    end; {if}
    VM_Destroy(params);
    DoProc := w;
  end; {if}
end; {DoExternalProc}

function TCs2PascalScript.RunBegin(Vars: PVariableManager; Skip: Boolean):
Boolean;
      { Run the Script, this is the main part of the script engine }
var
  StopOnSemicolon: Boolean;
  c, c2: PCajVariant;
  IPos, IStart, II, IEnd: LongInt;
  B: Boolean;

begin
  RunBegin := False;
  if Skip then
  begin
    if Parser^.CurrTokenId = CSTII_Begin then
    begin
      NextNoJunk(Parser);
      IPos := 1;
      while True do begin
        if Parser^.CurrTokenId = CSTI_EOF then
        begin RunError(EUnexpectedEndOfFile); Exit; end;
        if Parser^.CurrTokenId = CSTI_SyntaxError then
        begin RunError(ESyntaxError); Exit; end;
        if Parser^.CurrTokenId = CSTI_CommentEOFError then
        begin RunError(ESyntaxError); Exit; end;
        if Parser^.CurrTokenId = CSTI_CharError then
        begin RunError(ESyntaxError); Exit; end;
        if Parser^.CurrTokenId = CSTI_StringError then
        begin RunError(EStringError); Exit; end;
        if Parser^.CurrTokenId = CSTII_Case then
          Inc(IPos);
        if Parser^.CurrTokenId = CSTII_Begin then
          Inc(IPos);
        if Parser^.CurrTokenId = CSTII_End then
        begin
          Dec(IPos);
          if IPos = 0 then
            Break;
        end;
        NextNoJunk(Parser);
      end; {While}
      NextNoJunk(Parser); {Skip end}
    end else
    begin
      IPos := 1;
      while True do begin
        if Parser^.CurrTokenId = CSTI_EOF then
        begin RunError(EUnexpectedEndOfFile); Exit; end;
        if Parser^.CurrTokenId = CSTI_SyntaxError then
        begin RunError(ESyntaxError); Exit; end;
        if Parser^.CurrTokenId = CSTI_CommentEOFError then
        begin RunError(ESyntaxError); Exit; end;
        if Parser^.CurrTokenId = CSTI_CharError then
        begin RunError(ESyntaxError); Exit; end;
        if Parser^.CurrTokenId = CSTI_StringError then
        begin RunError(EStringError); Exit; end;
        if Parser^.CurrTokenId = CSTI_SemiColon then
          Break;
        if Parser^.CurrTokenId = CSTII_Begin then
        begin
          if not RunBegin(Vars, True) then
            Exit;
        end;
        if Parser^.CurrTokenId = CSTII_Else then
        begin
          Dec(IPos);
          if Ipos <= 0 then
            Break;
        end;
        if Parser^.CurrTokenId = CSTII_If then
          Inc(IPos);
        if Parser^.CurrTokenId = CSTII_End then
          Break;
        NextNoJunk(Parser);
      end; {While}
    end; {If}
    RunBegin := True;
    Exit;
  end; {If}
  if Parser^.CurrTokenId = CSTII_Begin then
  begin
    StopOnSemicolon := False;
    NextNoJunk(Parser); {skip begin}
  end else
    StopOnSemicolon := True;
  while True do begin
    case Parser^.CurrTokenId of
      CSTI_EOF: begin RunError(EUnexpectedEndOfFile); Exit; end;
      CSTI_SyntaxError: begin RunError(ESyntaxError); Exit; end;
      CSTI_CommentEOFError: begin RunError(ESyntaxError); Exit; end;
      CSTI_CharError: begin RunError(ESyntaxError); Exit; end;
      CSTI_StringError: begin RunError(EStringError); Exit; end;
      CSTII_Else: begin
          if StopOnSemicolon then
          begin
            RunBegin := True;
            Exit;
          end;
          RunError(EErrorInStatement);
          Exit;
        end;
      CSTII_End:
        begin
          RunBegin := True;
          NextNoJunk(Parser);
          Exit;
        end; {CSTII_End}
      CSTI_Semicolon:
        begin
          if StopOnSemicolon then
          begin
            RunBegin := True;
            Exit;
          end;
          NextNojunk(Parser);
        end; {CSTI_SemiColon}
      CSTII_If:
        begin
          if Assigned(OnRunLine) then
          begin
            RunError(OnRunLine(fId, {$IFNDEF CLASS}@{$ENDIF}Self,
              Parser^.CurrTokenPos));
            if ErrorCode <> ENoError then
              Exit;
          end;
          NextNoJunk(Parser);
          c := CreateCajVariant(CSV_Bool, 0);
          if not Calc(vars, c, CSTII_Then) then
          begin
            DestroyCajVariant(c);
            Exit;
          end; {if}
          if Parser^.CurrTokenId <> CSTII_then
            then
          begin
            RunError(EThenExpected);
            DestroyCajVariant(c);
            Exit;
          end;
          NextNoJunk(Parser); {skip THEN}
          if c^.Cv_Bool then
          begin
            DestroyCajVariant(c);
            if not RunBegin(Vars, False) then
            begin
              Exit;
            end; {if}
            if Parser^.CurrTokenId = CSTII_Else then
            begin
              if Assigned(OnRunLine) then
              begin
                RunError(OnRunLine(fId, {$IFNDEF CLASS}@{$ENDIF}Self,
                  Parser^.CurrTokenPos));
                if ErrorCode <> ENoError then
                  Exit;
              end;
              NextnoJunk(Parser);
              if not RunBegin(Vars, True) then
              begin
                Exit;
              end; {if}
            end; {if}
          end {if}
          else
          begin
            DestroyCajVariant(c);
            if not RunBegin(Vars, True) then
            begin
              Exit;
            end; {if}
            if Parser^.CurrTokenId = CSTII_Else then
            begin
              NextnoJunk(Parser);
              if Assigned(OnRunLine) then
              begin
                RunError(OnRunLine(fId, {$IFNDEF CLASS}@{$ENDIF}Self,
                  Parser^.CurrTokenPos));
                if ErrorCode <> ENoError then
                  Exit;
              end;
              if not RunBegin(Vars, False) then
              begin
                Exit;
              end; {if}
            end; {if}
          end; {if}
        end; {CSTII_If}
      CSTII_While:
        begin
          NextNoJunk(Parser);
          C := CreateCajVariant(CSV_Bool, 0);
          Ipos := Parser^.CurrTokenPos;
          if not Calc(Vars, c, CSTII_Do) then
          begin
            DestroyCajVariant(c);
            Exit;
          end; {if}
          if Parser^.CurrTokenID <> CSTII_Do then
          begin
            RunError(EDoExpected);
            DestroyCajVariant(c);
            Exit;
          end; {if}
          NextNoJunk(Parser);
          IStart := Parser^.CurrTokenPos;
          while C^.Cv_Bool do begin
            if Assigned(OnRunLine) then
            begin
              RunError(OnRunLine(fId, {$IFNDEF CLASS}@{$ENDIF}Self,
                Parser^.CurrTokenPos));
              if ErrorCode <> ENoError then
                Exit;
            end;
            if not RunBegin(Vars, False) then
            begin
              DestroyCajVariant(c);
              Exit;
            end;
            Parser^.CurrTokenPos := IPos;
            ParseToken(Parser);
            if not Calc(Vars, c, CSTII_Do) then
            begin
              DestroyCajVariant(c);
              Exit;
            end; {if}
            Parser^.CurrTokenPos := IStart;
            ParseToken(Parser);
          end; {While}
          DestroyCajVariant(c);
          if not RunBegin(Vars, True) then
            Exit;
        end; {CSTII_While}
      CSTII_For:
        begin
          NextNoJunk(Parser);
          if Parser^.CurrTokenId <> CSTI_Identifier then
          begin
            RunError(EIdentifierExpected);
            Exit;
          end; {if}
          if Assigned(Vars) and (VM_Find(Vars, FastUppercase(GetToken(Parser)))
            <> -1) then
            C := GetVarLink(VM_Get(Vars, VM_Find(Vars,
              FastUppercase(GetToken(Parser)))))
          else if VM_Find(Variables, FastUppercase(GetToken(Parser))) <> -1 then
            c := GetVarLink(VM_Get(Variables, VM_Find(Variables,
              FastUppercase(GetToken(Parser)))))
          else
          begin
            RunError(EUnknownIdentifier);
            Exit;
          end; {if}
          if (c^.Flags and $1) <> 0 then
          begin
            RunError(EVariableExpected);
            Exit;
          end; {if}
          if not IsIntegerType(c) then
          begin
            RunError(ETypeMismatch);
          end; {if}
          NextNoJunk(Parser);
          if Parser^.CurrTokenId <> CSTI_Assignment then
          begin
            RunError(EAssignmentExpected);
            Exit;
          end; {if}
          NextNoJunk(Parser);
          if not Calc(Vars, c, CSTII_To) then
            Exit;
          IStart := GetInt(c);
          if (Parser^.CurrTokenId <> CSTII_To) and
            (Parser^.CurrTokenId = CSTII_DownTo) then
          begin
            RunError(EToExpected);
            Exit;
          end; {if}
          B := Parser^.CurrTokenId = CSTII_DownTo;
          NextNoJunk(Parser);
          if not Calc(Vars, c, CSTII_Do) then
            Exit;
          IEnd := GetInt(c);
          if Parser^.CurrTokenId <> CSTII_Do then
          begin
            RunError(EDoExpected);
            Exit;
          end; {if}
          NextNoJunk(Parser);
          IPos := Parser^.CurrTokenPos;
          if B then
          begin
            c^.Flags := c^.Flags or $1;
            for II := IStart downto IEnd do begin
              if Assigned(OnRunLine) then
              begin
                RunError(OnRunLine(fId, {$IFNDEF CLASS}@{$ENDIF}Self,
                  Parser^.CurrTokenPos));
                if ErrorCode <> ENoError then
                  Exit;
              end;
              SetInteger(C, II);
              if not RunBegin(Vars, False) then
              begin
                c^.Flags := c^.Flags and not $1;
                Exit;
              end;
              Parser^.CurrTokenPos := IPos;
              ParseToken(Parser);
            end;
            c^.Flags := c^.Flags and not $1;
            if not RunBegin(Vars, True) then
              Exit;
          end {if}
          else
          begin
            c^.Flags := c^.Flags or $1;
            for II := IStart to IEnd do begin
              if Assigned(OnRunLine) then
              begin
                RunError(OnRunLine(fId, {$IFNDEF CLASS}@{$ENDIF}Self,
                  Parser^.CurrTokenPos));
                if ErrorCode <> ENoError then
                  Exit;
              end;
              SetInteger(C, II);
              if not RunBegin(Vars, False) then
              begin
                c^.Flags := c^.Flags and not $1;
                Exit;
              end;
              Parser^.CurrTokenPos := IPos;
              ParseToken(Parser);
            end;
            c^.Flags := c^.Flags and not $1;
            if not RunBegin(Vars, True) then
              Exit;
          end {if}
        end;
      CSTII_Repeat:
        begin
          RunError(EErrorInStatement);
          Exit;
        end; {CSTII_Repeat}
      CSTII_Begin:
        begin
          if not RunBegin(Vars, False) then
            Exit;
        end; {CSTII_Begin}
      CSTII_Case:
        begin
          NextNoJunk(Parser);
          c := CreateCajVariant(CSV_Var, 0);
          c^.CV_Var := nil; {Say that calc can assign any type}
          if not Calc(vars, c, CSTII_Of) then
          begin
            DestroyCajVariant(c);
            Exit;
          end; {If}
          if Parser^.CurrTokenId <> CSTII_OF then
          begin
            RunError(EOfExpected);
            exit;
          end; {If}
          NextNoJunk(Parser);
          B := False;
          while Parser^.CurrTokenId <> CSTII_End do begin
            if Parser^.CurrTokenId = CSTII_Else then
            begin
              NextNoJunk(Parser);
              DestroyCajVariant(c);
              if not RunBegin(Vars, B) then
                Exit;
              if Parser^.CurrTokenId = CSTI_SemiColon then
              begin
                NextNoJunk(Parser);
              end;
              if Parser^.CurrTokenId <> CSTII_End then
              begin
                RunError(EEndExpected);
                Exit;
              end;
              Exit;
            end;
            if Assigned(OnRunLine) then
            begin
              RunError(OnRunLine(fId, {$IFNDEF CLASS}@{$ENDIF}Self,
                Parser^.CurrTokenPos));
              if ErrorCode <> ENoError then
                Exit;
            end;
            c2 := CreateCajVariant(C^.VType, 0);
            if not Calc(vars, c2, CSTI_Colon) then
            begin
              DestroyCajVariant(c);
              DestroyCajVariant(c2);
              Exit;
            end; {If}
            if Parser^.CurrTokenId <> CSTI_Colon then
            begin
              RunError(EColonExpected);
              DestroyCajVariant(c);
              DestroyCajVariant(c2);
              Exit;
            end; {If}
            NextNoJunk(Parser);
            if not Perform(c2, c, ptEqual) then
            begin
              RunError(ETypeMismatch);
              DestroyCajVariant(c);
              DestroyCajVariant(c2);
              Exit;
            end; {If}
            if not RunBegin(Vars, (not c2^.CV_Bool or B)) then
            begin
              DestroyCajVariant(c);
              DestroyCajVariant(c2);
              Exit;
            end;
            if c2^.CV_Bool then
              B := True;
            if Parser^.CurrTokenid = CSTI_SemiColon then
            begin
              NextNoJunk(Parser);
            end;
            DestroyCajVariant(c2);
          end; {While}
          DestroyCajVariant(c);
          NextNoJunk(Parser); {Skip end}
        end; {CSTII_Case}
      CSTI_Identifier:
        begin
          if Assigned(OnRunLine) then
          begin
            RunError(OnRunLine(fId, {$IFNDEF CLASS}@{$ENDIF}Self,
              Parser^.CurrTokenPos));
            if ErrorCode <> ENoError then
              Exit;
          end;
          if PM_Find(InternalProcedures, FastUppercase(GetToken(Parser))) <> -1
            then
          begin
            DestroyCajVariant(DoProc(Vars, True));
            if ErrorCode <> ENoError then
              Exit;
          end {if}
          else if Assigned(vars) and (Vm_Find(Vars,
            FastUppercase(GetToken(Parser))) <> -1) then
          begin
            c := GetVarLink(VM_Get(Vars, Vm_Find(Vars,
              FastUppercase(GetToken(Parser)))));
            if (1 and c^.Flags) <> 0 then
            begin
              RunError(EErrorInStatement);
              Exit;
            end; {else if}
            if not CalcArrayInt(Vars, c) then
              Exit;
            NextNoJunk(Parser);
            if Parser^.CurrTokenId <> CSTI_Assignment then
            begin
              RunError(EAssignmentExpected);
              Exit;
            end; {if}
            NextNoJunk(Parser);
            if not Calc(vars, c, CSTI_Semicolon) then
              Exit;
          end {if}
          else if Vm_Find(Variables, FastUppercase(GetToken(Parser))) <> -1 then
          begin
            c := GetVarLink(VM_Get(Variables,
              Vm_Find(Variables, FastUppercase(GetToken(Parser)))));
            if (1 and c^.Flags) <> 0 then
            begin
              RunError(EErrorInStatement);
              Exit;
            end; {else if}
            if not CalcArrayInt(Vars, c) then
              Exit;
            NextNoJunk(Parser);
            if Parser^.CurrTokenId <> CSTI_Assignment then
            begin
              RunError(EAssignmentExpected);
              Exit;
            end; {if}
            NextNoJunk(Parser);
            if not Calc(vars, c, CSTI_Semicolon) then
              Exit;
          end {if}
          else if PM_Find(Procedures, FastUppercase(GetToken(Parser))) <> -1
            then
          begin
            DestroyCajVariant(DoProc(Vars, False));
            if ErrorCode <> ENoError then
              Exit;
          end {else if}
          else
          begin
            RunError(EUnknownIdentifier);
            Exit;
          end; {if}
        end; {CSTI_Identifier}
    else
      begin
        RunError(EErrorInStatement);
        Exit;
      end; {Else case}
    end; {Case}
  end; {While}
  RunBegin := True;
end; {RunBegin}

function StdProc(ID: Pointer; const ProcName: string; Params: PVariableManager;
  res: PCajVariant): TCS2Error;
{$IFDEF P16}Far; {$ENDIF}
var
  c: PCajVariant;
  i1, i2: LongInt;
  function mkchr(c: PCajVariant): Integer;
  begin
    if c^.vtype = CSV_String then
    begin
      if Length(c^.cv_str) = 1 then
      begin
        mkChr := Ord(c^.CV_str[1]);
      end else
        mkchr := -1;
    end else
    begin
      mkChr := Ord(c^.CV_Char);
    end;
  end;
begin
  StdProc := ENoError;
  if ProcName = 'STRGET' then
  begin
    c := GetVarLink(VM_Get(params, 0));
    i1 := GetInt(GetVarLink(VM_Get(params, 1)));
    if (i1 < 1) or (i1 > Length(c^.cv_Str)) then
    begin
      StdProc := ERangeError;
      Exit;
    end;
    Res^.CV_Char := c^.cv_Str[i1];
  end else if ProcName = 'STRSET' then
  begin
    c := GetVarLink(VM_Get(params, 2));
    i1 := GetInt(GetVarLink(VM_Get(params, 1)));
    if (i1 < 1) or (i1 > Length(c^.cv_Str)) then
    begin
      StdProc := ERangeError;
      Exit;
    end;
    i2 := MkChr(GetVarLink(VM_Get(params, 2)));
    if i2 = -1 then
    begin
      StdProc := ERangeError;
      Exit;
    end;
    C^.CV_Str[i1] := Chr(i2);
  end else if ProcName = 'ORD' then
  begin
    i1 := MkChr(GetVarLink(VM_Get(params, 0)));
    if i1 = -1 then
    begin
      StdProc := ERangeError;
      Exit;
    end;
    res^.cv_UByte := i1;
  end else if ProcName = 'CHR' then
  begin
    res^.Cv_Char := Chr(GetInt(GetVarLink(VM_Get(Params, 0))));
  end else if ProcName = 'UPPERCASE' then
  begin
    SetString(Res, FastUppercase(GetStr(GetVarLink(VM_Get(Params, 0)))));
  end else if ProcName = 'POS' then
  begin
    SetInteger(Res, Pos(GetStr(GetVarLink(VM_Get(Params, 0))),
      GetStr(GetVarLink(VM_Get(Params, 1)))));
  end else if ProcName = 'INTTOSTR' then
  begin
    SetString(Res, IntToStr(GetInt(GetVarLink(VM_Get(Params, 0)))));
  end else if ProcName = 'STRTOINT' then
  begin
    SetInteger(Res, StrToIntDef(GetStr(GetVarLink(VM_Get(Params, 0))),
      GetInt(GetVarLink(VM_Get(Params, 1)))));
  end else if ProcName = 'COPY' then
  begin
    SetString(Res, Copy(GetStr(GetVarLink(VM_Get(Params, 0))),
      GetInt(GetVarLink(VM_Get(Params, 1))),
      GetInt(GetVarLink(VM_Get(Params, 2)))));
  end else if ProcName = 'DELETE' then
  begin
    c := GetVarLink(VM_Get(params, 0));
    Delete(c^.cv_Str, GetInt(GetVarLink(VM_Get(Params, 1))),
      GetInt(GetVarLink(VM_Get(Params, 2))));
  end else if ProcName = 'INSERT' then
  begin
    c := GetVarLink(VM_Get(params, 1));
    Insert(GetStr(GetVarLink(VM_Get(Params, 0))), c^.cv_Str,
      GetInt(GetVarLink(VM_Get(Params, 2))));
  end else if ProcName = 'SETARRAYLENGTH' then
  begin
    c := GetVarLink(VM_Get(params, 0));
    i1 := GetInt(GetVarLink(VM_Get(params, 1)));
    if i1 > c^.CV_ArrItems.Count then
    begin
      for i2 := 1 to i1 - C^.CV_ArrItems.Count do begin
        c^.CV_ArrItems.Add(CreateCajVariant(c^.CV_ArrVType, 0));
      end;
    end else if i1 < c^.CV_ArrItems.Count then
    begin
      for i2 := 1 to c^.CV_ArrItems.Count - i1 do begin
        DestroyCajVariant(c^.CV_ArrItems.GetItem(c^.Cv_ArrItems.Count - 1));
        c^.Cv_ArrItems.Delete(c^.Cv_ArrItems.Count - 1);
      end;
    end;
  end else if ProcName = 'GETARRAYLENGTH' then
  begin
    SetInteger(Res, GetVarLink(VM_Get(params, 0))^.CV_ArrItems.Count);
  end else if ProcName = 'LENGTH' then
  begin
    SetInteger(Res, Length(GetStr(GetVarLink(VM_Get(params, 0)))));
  end else if ProcName = 'SIN' then
  begin
    SetReal(Res, Sin(GetReal(GetVarLink(VM_Get(params, 0)))));
  end else if ProcName = 'COS' then
  begin
    SetReal(Res, Cos(GetReal(GetVarLink(VM_Get(params, 0)))));
  end else if ProcName = 'PI' then
  begin
    SetReal(Res, PI);
  end else if ProcName = 'ROUND' then
  begin
    SetInteger(Res, Round(GetReal(GetVarLink(VM_Get(params, 0)))));
  end else if ProcName = 'TRUNC' then
  begin
    SetInteger(Res, Trunc(GetReal(GetVarLink(VM_Get(params, 0)))));
  end;
end;

procedure RegisterStdLib(P: PCs2PascalScript);
{Register standard library}
begin
{$IFDEF CLASS}
  PM_Add(p.Procedures, '7 STRGET S 8 I 6', @StdProc);
  PM_Add(p.Procedures, '0 STRSET C 7 I 6 !S 8', @StdProc);
  PM_Add(p.Procedures, '1 ORD C 7', @StdProc);
  PM_Add(p.Procedures, '7 CHR B 1', @StdProc);
  PM_Add(p.Procedures, '6 STRTOINT S 8 I 6', @StdProc);
  PM_Add(p.Procedures, '8 INTTOSTR I 6', @StdProc);
  PM_Add(p.Procedures, '8 UPPERCASE S 8', @StdProc);
  PM_Add(p.Procedures, '8 COPY S 8 I1 6 I2 6', @StdProc);
  PM_Add(p.Procedures, '0 DELETE !S 8 I1 6 I2 6', @StdProc);
  PM_Add(p.Procedures, '0 INSERT S1 8 !S 8 I1 6', @StdProc);
  PM_Add(p.Procedures, '6 POS S1 8 S2 8', @StdProc);
  PM_Add(p.Procedures, '0 SETARRAYLENGTH !ARR 16 I 6', @StdProc);
  PM_Add(p.Procedures, '6 GETARRAYLENGTH !ARR 16', @StdProc);
  PM_Add(p.Procedures, '6 LENGTH S 8', @StdProc);
  PM_Add(p.Procedures, '12 SIN S 12', @StdProc);
  PM_Add(p.Procedures, '12 COS S 12', @StdProc);
  PM_Add(p.Procedures, '6 ROUND S 12', @StdProc);
  PM_Add(p.Procedures, '6 TRUNC S 12', @StdProc);
  PM_Add(p.Procedures, '12 PI', @StdProc);
{$ELSE}
  PM_Add(p^.Procedures, '7 STRGET S 8 I 6', @StdProc);
  PM_Add(p^.Procedures, '0 STRSET C 7 I 6 !S 8', @StdProc);
  PM_Add(p^.Procedures, '1 ORD C 7', @StdProc);
  PM_Add(p^.Procedures, '7 CHR B 1', @StdProc);
  PM_Add(p^.Procedures, '6 STRTOINT S 8 I 6', @StdProc);
  PM_Add(p^.Procedures, '8 INTTOSTR I 6', @StdProc);
  PM_Add(p^.Procedures, '8 UPPERCASE S 8', @StdProc);
  PM_Add(p^.Procedures, '8 COPY S 8 I1 6 I2 6', @StdProc);
  PM_Add(p^.Procedures, '0 DELETE !S 8 I1 6 I2 6', @StdProc);
  PM_Add(p^.Procedures, '0 INSERT S1 8 !S 8 I1 6', @StdProc);
  PM_Add(p^.Procedures, '6 POS S1 8 S2 8', @StdProc);
  PM_Add(p^.Procedures, '0 SETARRAYLENGTH !ARR 16 I 6', @StdProc);
  PM_Add(p^.Procedures, '6 GETARRAYLENGTH !ARR 16', @StdProc);
  PM_Add(p^.Procedures, '6 LENGTH S 8', @StdProc);
  PM_Add(p^.Procedures, '12 SIN S 12', @StdProc);
  PM_Add(p^.Procedures, '12 COS S 12', @StdProc);
  PM_Add(p^.Procedures, '6 ROUND S 12', @StdProc);
  PM_Add(p^.Procedures, '6 TRUNC S 12', @StdProc);
  PM_Add(p^.Procedures, '12 PI', @StdProc);
{$ENDIF}
end;


end.

