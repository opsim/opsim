unit CS2_VAR; {Cajscript 2.0 Variable management, Procedure management}
{$I CS2_DEF.INC}
interface
uses
  CS2_UTL;
type
  TCS2Error = Word;

const
  ENoError = 0;
  ECanNotReadProperty = 1;
  ECanNotWriteProperty = 2;
  EUnknownIdentifier = 3;
  EIdentifierExpected = 4;
  ESemicolonExpected = 5;
  EBeginExpected = 6;
  EDuplicateIdentifier = 7;
  EUnexpectedEndOfFile = 8;
  EColonExpected = 9;
  ESyntaxError = 10;
  EStringError = 11;
  EErrorInStatement = 12;
  EAssignmentExpected = 13;
  ETypeMismatch = 14;
  EErrorInExpression = 15;
  ERoundOpenExpected = 16;
  ERoundCloseExpected = 17;
  EVariableExpected = 18;
  ECommaExpected = 19;
  EThenExpected = 20;
  EPeriodExpected = 21;
  EParameterError = 22;
  EToExpected = 23;
  EDoExpected = 24;
  ERangeError = 25;
  EOfExpected = 26;
  EEndExpected = 27;
  EOutOfRange = 28;
  EOpenBlockExpected = 29;
  ECloseBlockExpected = 30;

const
  CSV_NONE = 0; { Void/ERROR }
  CSV_UByte = 1; { Byte }
  CSV_SByte = 2; { ShortInt }
  CSV_UInt16 = 3; { Word }
  CSV_SInt16 = 4; { Integer (Delphi : SmallInt) }
  CSV_UInt32 = 5; { Longint (Delphi : Cardinal) }
  CSV_SInt32 = 6; { Longint }
  CSV_Char = 7; { Char }
  CSV_String = 8; { String }
  CSV_Real = 9; { Real }
  CSV_Single = 10; { Single }
  CSV_Double = 11; { Double }
  CSV_Extended = 12; { Extended }
  CSV_Comp = 13; { Comp }
  CSV_Bool = 14; { Boolean }
  CSV_Var = 15; { variable }
  CSV_Array = 16; { array }

type
  PCajVariant = ^TCajVariant;
  TCajVariant = packed record
    VType: Word;
    Flags: Byte; {Readonly(Const) = 1}
{$IFNDEF SS}
    CV_Str: string;
{$ENDIF}
    case Word of
      CSV_UByte: (CV_UByte: Byte);
      CSV_SByte: (CV_SByte: ShortInt);
      CSV_Char: (CV_Char: Char);
      CSV_UInt16: (CV_UInt16: Word);

      CSV_SInt16: (CV_SInt16: {$IFDEF I32}SmallInt{$ELSE}Integer{$ENDIF});
      CSV_UInt32: (CV_UInt32: {$IFDEF I32}Cardinal{$ELSE}LongInt{$ENDIF});
      CSV_SInt32: (CV_SInt32: LongInt);
      CSV_String: ({$IFDEF SS}CV_Str: string{$ENDIF});
      CSV_Real: (CV_Real: Real);
      CSV_Single: (CV_Single: Single);
      CSV_Double: (CV_Double: Double);
      CSV_Extended: (CV_Extended: Extended);
      CSV_Comp: (CV_Comp: Comp);
      CSV_Bool: (CV_Bool: Boolean);
      CSV_Var: (CV_Var: Pointer); {Pointer to a CajVariant}
      CSV_Array: (CV_ArrVType: Word; CV_ArrItems: TifList);
  end;
{ Array:
  SubType(s): IntToStr(TypeNo);
}

function CreateCajVariant(VType, ArrVType: Word): PCajVariant;
procedure SetType(p: PCajVariant; FType: Word);

function CreateReal(const e: Extended): PCajVariant;
function CreateString(const s: string): PCajVariant;
function CreateInteger(i: LongInt): PCajVariant;
function CreateBool(b: Boolean): PCajVariant;

procedure DestroyCajVariant(p: PCajVariant);

type
  PVariableManager = ^TVariableManager;
  TVariableManager = packed record
    Names: TifStringList;
    Ptr: TifList;
  end;

function VM_Create(InheritFrom: PVariableManager): PVariableManager;
procedure VM_Destroy(p: PVariableManager);
function VM_Add(P: PVariableManager; D: PCajVariant; const Name: string):
PcajVariant;
procedure VM_Delete(p: PVariableManager; Idx: LongInt);
function VM_Get(p: PVariableManager; Idx: LongInt): PCajVariant;
procedure VM_SetName(p: PVariableManager; Idx: LongInt; S: string);
function VM_Count(p: PVariableManager): LongInt;
function VM_Find(p: PVariableManager; const Name: string): LongInt;
procedure VM_Clear(p: PVariableManager);


type
  TRegisteredProc = function(ID: Pointer;
    const ProcName: string; Params: PVariableManager;
    res: PCajVariant): TCS2Error;

  PProcedureManager = ^TProcedureManager;
  TProcedureManager = packed record
    Names: TifStringList;
    Ptr: TifList;
  end;
  {Spec: RESTYPE NAME PARAM1NAME PARAM1TYPE PARAM2NAME PARAM2TYPE
  an ! before the paramname means is VARIABLE
  }

function PM_Create: PProcedureManager;
procedure PM_Destroy(p: PProcedureManager);
procedure PM_Clear(p: PProcedureManager);
procedure PM_Add(p: PProcedureManager; const Spec: string; Addr: Pointer);
procedure PM_Delete(p: PProcedureManager; I: LongInt);
function PM_Find(p: PProcedureManager; const Name: string): Integer;
function PM_Get(p: PProcedureManager; i: LongInt): Pointer;
function PM_GetSpec(p: PProcedureManager; i: LongInt): string;

function DoMinus(p: PCajVariant): Boolean;
function DoNot(p: PCajVariant): Boolean;
type
  TPerformType = (PtSet, ptMinus, PtPlus, PtMul, ptDiv, PtIntDiv, PtIntMod,
    PtAnd,
    ptOr, ptXor, PtShl, PtShr, PtGreater, PtLess, PtEqual, PtNotEqual,
    PtGreaterEqual, PtLessEqual);
function Perform(V1: pCajVariant; v2: pCajVariant; T: TPerformType): Boolean;

procedure SetInteger(p: PCajVariant; I: LongInt);
procedure SetReal(p: PCajVariant; i: Extended);
procedure SetString(p: PCajVariant; const I: string);

function IsStringType(v: PCajVariant): Boolean;
function IsIntRealType(v: PCajVariant): Boolean;
function IsIntegerType(v: PCajVariant): Boolean;
function IsBooleanType(v: PCajVariant): Boolean;
function IsRealType(v: PCajVariant): Boolean;

function GetStr(v: PCajVariant): string;
function GetReal(v: PCajVariant): Extended;
function GetInt(v: PCajVariant): LongInt;
function GetBool(v: PCajVariant): Boolean;

function GetVarLink(V: PCajVariant): PCajVariant;
{Always use this function when using VM_Get}
function GetArrayItem(Arr: PCajVariant; FieldNo: Longint): PCajVariant;

function ErrorToString(e: TCS2Error): string;

implementation

function ErrorToString(e: TCS2Error): string;
begin
  case e of
    ENoError: ErrorToString := 'no error';
    ECanNotReadProperty: ErrorToString := 'can not read property';
    ECanNotWriteProperty: ErrorToString := 'can not write property';
    EUnknownIdentifier: ErrorToString := 'unknown identifier';
    EIdentifierExpected: ErrorToString := 'identifier expected';
    ESemicolonExpected: ErrorToString := 'semicolon expected';
    EBeginExpected: ErrorToString := 'begin expected';
    EDuplicateIdentifier: ErrorToString := 'duplicate identifier';
    EUnexpectedEndOfFile: ErrorToString := 'unexpected end of file';
    EColonExpected: ErrorToString := 'colon expected';
    ESyntaxError: ErrorToString := 'syntax error';
    EStringError: ErrorToString := 'string error';
    EErrorInStatement: ErrorToString := 'error in statement';
    EAssignmentExpected: ErrorToString := 'assignment expected';
    ETypeMismatch: ErrorToString := 'type mismatch';
    EErrorInExpression: ErrorToString := 'error in expression';
    ERoundOpenExpected: ErrorToString := 'round open expected';
    ERoundCloseExpected: ErrorToString := 'round close expected';
    EVariableExpected: ErrorToString := 'variable expected';
    ECommaExpected: ErrorToString := 'comma expected';
    EThenExpected: ErrorToString := 'then expected';
    EPeriodExpected: ErrorToString := 'period expected';
    EParameterError: ErrorToString := 'parameter error';
    EToExpected: ErrorToString := 'to expected';
    EDoExpected: ErrorToString := 'do expected';
    ERangeError: ErrorToString := 'range error';
    EOfExpected: ErrorToString := 'of expected';
    EEndExpected: ErrorToString := 'end expected';
    EOutOfRange: ErrorToString := 'out of range';
    EOpenBlockExpected: ErrorToString := 'open block expected';
    ECloseBlockExpected: ErrorToString := 'close block expected';
  else
    ErrorToString := 'unknown error';
  end;
end;

function GetArrayItem(Arr: PCajVariant; FieldNo: Longint): PCajVariant;
begin
  Arr := GetVarLink(Arr);
  if Arr^.VType <> CSV_Array then
  begin
    GetArrayItem := nil;
    Exit;
  end;
  if (FieldNo < 0) or (FieldNo > Arr^.CV_ArrItems.Count - 1) then
  begin
    GetArrayItem := nil;
    Exit;
  end;
  GetArrayItem := Arr^.CV_ArrItems.GetItem(FieldNo);
end;

function GetVarLink(V: PCajVariant): PCajVariant;
begin
  if Assigned(v) then
    while v^.VType = CSV_Var do
    begin
      if Assigned(V^.CV_Var) then
        v := V^.Cv_Var
      else
        Break;
    end;
  GetVarLink := v;
end;

procedure SetType(p: PCajVariant; FType: Word);
var
  i: Longint;
begin
  if p^.Vtype = CSV_String then
    p^.cv_Str := ''
  else
    if p^.Vtype = CSV_Array then
    begin
      for i := 0 to p^.CV_ArrItems.count - 1 do
      begin
        DestroyCajVariant(p^.CV_ArrItems.GetItem(i));
      end;
      p^.CV_ArrItems.Destroy;
    end;
  p^.VType := FType;
  if FType = CSV_Array then begin
    p^.CV_ArrVType := CSV_UByte;
    p^.CV_ArrItems.Create;
  end;
end;


function CreateCajVariant(VType, ArrVType: Word): PCajVariant;
{
  Creates an instance of a CajVariant.
}
var
  p: PCajVariant;
begin
  New(p);
  p^.VType := VType;
  p^.Flags := 0;
  if VType = CSV_Var then
    p^.CV_Var := nil
  else if VType = CSV_Array then
  begin
    p^.CV_ArrVType := ArrVType;
    p^.CV_ArrItems.Create;
  end;
  CreateCajVariant := p;
end;

function CreateReal(const e: Extended): PCajVariant;
var
  p: PCajVariant;
begin
  p := CreateCajVariant(CSV_Extended, 0);
  p^.Cv_Extended := e;
  CreateReal := p;
end;

function CreateString(const s: string): PCajVariant;
var
  p: PCajVariant;
begin
  p := CreateCajVariant(CSV_String, 0);
  p^.Cv_Str := s;
  CreateString := p;
end;

function CreateInteger(i: LongInt): PCajVariant;
var
  p: PCajVariant;
begin
  p := CreateCajVariant(CSV_SInt32, 0);
  p^.Cv_sInt32 := i;
  CreateInteger := p;
end;

function CreateBool(b: Boolean): PCajVariant;
var
  p: PCajVariant;
begin
  p := CreateCajVariant(CSV_Bool, 0);
  p^.Cv_Bool := b;
  Createbool := p;
end;

procedure DestroyCajVariant(p: PCajVariant);
{ Destroys an instance of a CajVariant.}
var
  i: Longint;
begin
  if Assigned(p) then
  begin
    if P^.Vtype = CSV_Array then
    begin
      for i := 0 to p^.CV_ArrItems.count - 1 do
      begin
        DestroyCajVariant(p^.CV_ArrItems.GetItem(i));
      end;
      p^.CV_ArrItems.Destroy;
    end;
    Dispose(p);
  end;
end;

function VM_Create(InheritFrom: PVariableManager): PVariableManager;
{Creates an instance of a VariableManger}
var
  p: PVariableManager;
  i: Integer;
begin
  New(p);
  p^.names.Create;
  p^.Ptr.Create;
  if Assigned(InheritFrom) then
  begin
    for i := 0 to InheritFrom^.names.count - 1 do
    begin
      p^.names.Add(InheritFrom^.names.GetItem(i));
      p^.Ptr.Add(InheritFrom^.Ptr.GetItem(i));
    end;
  end;
  VM_Create := p;
end;

procedure VM_Destroy(p: PVariableManager);
{Destroys an instance of a VariableManager}
var
  i: Integer;
begin
  for i := 0 to p^.Ptr.count - 1 do
  begin
    DestroyCajVariant(p^.Ptr.GetItem(i));
  end;
  p^.names.Destroy;
  p^.Ptr.Destroy;
  Dispose(p);
end;

function VM_Add(P: PVariableManager; D: PCajVariant; const Name: string):
PCajVariant;
var
  i: Integer;
begin
  for i := 0 to p^.Names.Count - 1 do
  begin
    if p^.names.GetItem(i) = Name then
    begin
      VM_Add := nil;
      Exit;
    end;
  end;
  p^.Names.Add(Name);
  p^.Ptr.Add(D);
  VM_Add := D;
end;

procedure VM_Clear(p: PVariableManager);
var
  i: Integer;
begin
  for i := 0 to p^.Ptr.count - 1 do
  begin
    DestroyCajVariant(p^.Ptr.GetItem(i));
  end;
  p^.names.Clear;
  p^.Ptr.Clear;
end;

procedure VM_Delete(p: PVariableManager; Idx: LongInt);
begin
  p^.Names.Delete(idx);
  DestroyCajVariant(p^.Ptr.GetItem(idx));
  p^.Ptr.Delete(Idx);
end;

function VM_Find(p: PVariableManager; const Name: string): LongInt;
var
  i: Integer;
begin
  for i := 0 to p^.Names.Count - 1 do
  begin
    if p^.names.GetItem(i) = Name then
    begin
      VM_Find := I;
      Exit;
    end;
  end;
  VM_Find := -1;
end;

function VM_Count(p: PVariableManager): LongInt;
begin
  VM_Count := P^.Ptr.Count;
end;

function VM_Get(p: PVariableManager; Idx: LongInt): PCajVariant;
begin
  VM_Get := P^.Ptr.GetItem(idx);
end;

procedure VM_SetName(p: PVariableManager; Idx: LongInt; S: string);
begin
  P^.Names.SetItem(idx, s);
end;


function PM_Create: PProcedureManager;
{Creates an instance of a Procedure Manager}
var
  p: PProcedureManager;
begin
  New(p);
  p^.names.Create;
  p^.Ptr.Create;
  PM_Create := p;
end;

procedure PM_Clear(p: PProcedureManager);
begin
  p^.names.Clear;
  p^.Ptr.Clear;
end;

procedure PM_Destroy(p: PProcedureManager);
{Destroys an instance of a Procedure Manager}
begin
  p^.names.Destroy;
  p^.Ptr.Destroy;
  Dispose(p);
end;

procedure PM_Add(p: PProcedureManager; const Spec: string; Addr: Pointer);
var
  w: string;
begin
  w := spec;
  Delete(w, 1, Pos(' ', w));
  Rs(w);
  if Pos(' ', w) > 0 then
    w := Copy(w, 1, Pos(' ', w) - 1);
  if Pm_Find(p, w) = -1 then
  begin
    p^.Names.Add(Spec);
    p^.Ptr.Add(Addr);
  end;
end;

procedure PM_Delete(p: PProcedureManager; I: LongInt);
begin
  p^.Names.Delete(i);
  p^.Ptr.Delete(I);
end;

function PM_Find(p: PProcedureManager; const Name: string): Integer;
var
  i: Integer;
  s: string;
begin
  for i := 0 to p^.names.count - 1 do
  begin
    s := p^.names.GetItem(i);
    Delete(s, 1, Pos(' ', s));
    Rs(s);
    if Pos(' ', s) > 0 then
      s := Copy(s, 1, Pos(' ', s) - 1);
    if s = Name then
    begin
      PM_Find := i;
      Exit;
    end;
  end;
  PM_Find := -1;
end;

function PM_Get(p: PProcedureManager; i: LongInt): Pointer;
begin
  PM_Get := p^.Ptr.GetItem(i);
end;

function PM_GetSpec(p: PProcedureManager; i: LongInt): string;
begin
  PM_GetSpec := p^.Names.GetItem(i);
end;

function DoMinus(p: PCajVariant): Boolean;
begin
  p := GetVarLink(p);
  DoMinus := True;
  case P^.VType of
    CSV_UByte: p^.Cv_UByte := -p^.Cv_UByte;
    CSV_SByte: p^.Cv_SByte := -p^.Cv_SByte;
    CSV_UInt16: p^.Cv_UInt16 := -p^.Cv_UInt16;
    CSV_SInt16: p^.Cv_SInt16 := -p^.Cv_SInt16;
    CSV_UInt32: p^.Cv_UInt32 := -p^.Cv_UInt32;
    CSV_SInt32: p^.Cv_SInt32 := -p^.Cv_SInt32;
    CSV_Real: p^.Cv_Real := -p^.Cv_Real;
    CSV_Single: p^.Cv_Single := -p^.cv_Single;
    CSV_Double: p^.Cv_Double := -p^.Cv_Double;
    CSV_Extended: p^.Cv_Extended := -p^.Cv_Extended;
    CSV_Comp: p^.Cv_Comp := -p^.Cv_Comp;
  else
    DoMinus := False;
  end;
end;

function DoNot(p: PCajVariant): Boolean;
begin
  p := GetVarLink(p);
  DoNot := True;
  case P^.VType of
    CSV_UByte: p^.Cv_UByte := not p^.Cv_UByte;
    CSV_SByte: p^.Cv_SByte := not p^.Cv_SByte;
    CSV_UInt16: p^.Cv_UInt16 := not p^.Cv_UInt16;
    CSV_SInt16: p^.Cv_SInt16 := not p^.Cv_SInt16;
    CSV_UInt32: p^.Cv_UInt32 := not p^.Cv_UInt32;
    CSV_SInt32: p^.Cv_SInt32 := not p^.Cv_SInt32;
    CSV_Bool: p^.CV_Bool := not p^.CV_Bool;
  else
    DoNot := False;
  end;
end;

procedure SetInteger(p: PCajVariant; I: LongInt);
begin
  p := GetVarLink(p);
  case P^.VType of
    CSV_UByte: p^.Cv_UByte := i;
    CSV_SByte: p^.Cv_SByte := i;
    CSV_UInt16: p^.Cv_UInt16 := i;
    CSV_SInt16: p^.Cv_SInt16 := i;
    CSV_UInt32: p^.Cv_UInt32 := i;
    CSV_SInt32: p^.Cv_SInt32 := i;
  end;
end;

procedure SetReal(p: PCajVariant; i: Extended);
begin
  p := GetVarLink(p);
  case P^.VType of
    CSV_Real: P^.CV_Real := i;
    CSV_Single: P^.CV_Single := i;
    CSV_Double: P^.CV_Double := i;
    CSV_Extended: P^.CV_Extended := i;
    CSV_Comp: P^.CV_Comp := round(i);
  end;
end;

procedure SetString(p: PCajVariant; const I: string);
begin
  p := GetVarLink(p);
  case P^.VType of
    CSV_String: P^.Cv_Str := i;
  end;
end;

function IsRealType(v: PCajVariant): Boolean;
begin
  v := GetVarLink(v);
  IsRealType := (V^.VType = CSV_Real) or
    (v^.Vtype = CSV_Single) or
    (v^.Vtype = CSV_Double) or
    (v^.Vtype = CSV_Extended) or
    (v^.Vtype = CSV_Comp);
end;

function IsIntegerType(v: PCajVariant): Boolean;
begin
  v := GetVarLink(v);
  IsIntegerType := (v^.Vtype = CSV_UByte) or
    (v^.Vtype = CSV_SByte) or
    (v^.Vtype = CSV_UInt16) or
    (v^.Vtype = CSV_SInt16) or
    (v^.Vtype = CSV_UInt32) or
    (v^.Vtype = CSV_SInt32);
end;

function IsBooleanType(v: PCajVariant): Boolean;
begin
  v := GetVarLink(v);
  IsBooleanType := (v^.Vtype = CSV_Bool);
end;

function IsIntRealType(v: PCajVariant): Boolean;
begin
  v := GetVarLink(v);
  IsIntRealType := (v^.Vtype = CSV_UByte) or
    (v^.Vtype = CSV_SByte) or
    (v^.Vtype = CSV_UInt16) or
    (v^.Vtype = CSV_SInt16) or
    (v^.Vtype = CSV_UInt32) or
    (v^.Vtype = CSV_SInt32) or
    (V^.VType = CSV_Real) or
    (v^.Vtype = CSV_Single) or
    (v^.Vtype = CSV_Double) or
    (v^.Vtype = CSV_Extended) or
    (v^.Vtype = CSV_Comp);
end;

function IsStringType(v: PCajVariant): Boolean;
begin
  v := GetVarLink(v);
  IsStringType := (v^.Vtype = CSV_Char) or
    (v^.Vtype = CSV_String);
end;

function GetInt(v: PCajVariant): LongInt;
begin
  v := GetVarLink(v);
  case v^.Vtype of
    CSV_UByte: GetInt := V^.CV_UByte;
    CSV_SByte: GetInt := V^.CV_SByte;
    CSV_UInt16: GetInt := V^.CV_UInt16;
    CSV_SInt16: GetInt := V^.CV_SInt16;
    CSV_UInt32: GetInt := V^.CV_UInt32;
    CSV_SInt32: GetInt := V^.CV_SInt32;
  else GetInt := 0;
  end;
end;

function GetReal(v: PCajVariant): Extended;
begin
  v := GetVarLink(v);
  case v^.Vtype of
    CSV_Real: GetReal := V^.CV_Real;
    CSV_Single: GetReal := V^.CV_single;
    CSV_Double: GetReal := V^.CV_double;
    CSV_Extended: GetReal := V^.CV_Extended;
    CSV_Comp: GetReal := V^.CV_Comp;
    CSV_UByte: GetReal := V^.CV_UByte;
    CSV_SByte: GetReal := V^.CV_SByte;
    CSV_UInt16: GetReal := V^.CV_UInt16;
    CSV_SInt16: GetReal := V^.CV_SInt16;
    CSV_UInt32: GetReal := V^.CV_UInt32;
    CSV_SInt32: GetReal := V^.CV_SInt32;
  else GetReal := 0;
  end;
end;

function GetStr(v: PCajVariant): string;
begin
  v := GetVarLink(v);
  case v^.Vtype of
    CSV_String: GetStr := V^.CV_Str;
    CSV_Char: GetStr := V^.CV_Char;
  end;
end;

function GetBool(v: PCajVariant): Boolean;
begin
  v := GetVarLink(v);
  case v^.Vtype of
    CSV_Bool: GetBool := V^.CV_Bool;
  else
    GetBool := False;
  end;
end;

{$IFDEF DELPHI}{$WARNINGS OFF}{$ENDIF}

function Perform(V1: pCajVariant; v2: pCajVariant; T: TPerformType): Boolean;

var
  err: Boolean;
  i: Longint;

  procedure MakeItReal(v: Extended);
  begin
    V1^.VType := CSV_Extended;
    v1^.Cv_Extended := v;
  end;

  procedure MakeItBool(v: Boolean);
  begin
    v1^.VType := CSV_Bool;
    v1^.Cv_Bool := v;
  end;

  procedure AddArrayVar(var v: TIfList; i: PcajVariant);
  var
    n: PCajVariant;
  begin
    new(n);
    n^.VType := CSV_Var;
    n^.CV_Var := nil;
    PerForm(n, i, ptSet);
  end;

begin
  v1 := GetVarLink(v1);
  v2 := GetVarLink(v2);
  if ((v1^.Vtype <> v2^.VType) and
    not (IsIntRealType(v1) and IsIntRealType(v2)) and
    not (IsStringType(v1) and IsStringType(v2)) and
    not (v1^.VType = CSV_Var)) or (v1^.VType = CSV_Array)
    then
    begin
    Perform := False;
    Exit;
  end;
  Err := False;
  case T of
    PtSet:
      begin
        if IsIntegerType(v1) and not IsIntegerType(v2) then
        begin
          Err := True;
        end else
          case V1^.VType of
            CSV_UByte: v1^.Cv_UByte := GetInt(v2);
            CSV_SByte: v1^.Cv_SByte := GetInt(v2);
            CSV_Char:
              begin
                v1^.Cv_Str := GetStr(v2);
                if Length(v1^.Cv_Str) > 1 then Err := True else
                  v1^.Cv_Char := v1^.Cv_Str[1];
              end;
            CSV_UInt16: v1^.Cv_UInt16 := GetInt(v2);
            CSV_SInt16: v1^.Cv_SInt16 := GetInt(v2);
            CSV_UInt32: v1^.Cv_UInt32 := GetInt(v2);
            CSV_SInt32: v1^.Cv_SInt32 := GetInt(v2);
            CSV_String: v1^.Cv_Str := GetStr(v2);
            CSV_Real: v1^.CV_Real := GetReal(v2);
            CSV_Single: v1^.CV_Single := GetReal(v2);
            CSV_Double: v1^.CV_Double := GetReal(v2);
            CSV_Extended: v1^.CV_Extended := GetReal(v2);
            CSV_Comp: v1^.CV_comp := round(GetReal(v2));
            CSV_Bool:
              begin
                if v2^.VType = CSV_Bool then
                  v1^.Cv_Bool := v2^.Cv_Bool
                else
                  err := True;
              end;
            CSV_Var: begin
                if v1^.Vtype = CSV_Array then
                begin
                  for i := 0 to v1^.CV_ArrItems.count - 1 do
                  begin
                    DestroyCajVariant(v1^.CV_ArrItems.GetItem(i));
                  end;
                  v1^.CV_ArrItems.Destroy;
                end;
                v1^.VType := v2^.VType;
                case v2^.vtype of
                  CSV_UByte: v1^.CV_UByte := v2^.CV_UByte;
                  CSV_SByte: v1^.CV_SByte := v2^.CV_SByte;
                  CSV_UInt16: v1^.CV_UInt16 := v2^.CV_UInt16;
                  CSV_SInt16: v1^.CV_SInt16 := v2^.CV_SInt16;
                  CSV_UInt32: v1^.CV_UInt32 := v2^.CV_UInt32;
                  CSV_SInt32: v1^.CV_SInt32 := v2^.CV_Sint32;
                  CSV_Char: v1^.CV_Char := v2^.CV_Char;
                  CSV_String: v1^.CV_Str := v2^.CV_Str;
                  CSV_Real: v1^.CV_Real := v2^.CV_Real;
                  CSV_Single: v1^.CV_Single := v2^.CV_Single;
                  CSV_Double: v1^.CV_Double := v2^.CV_Double;
                  CSV_Extended: v1^.CV_Extended := v2^.CV_Extended;
                  CSV_Comp: v1^.CV_Comp := v2^.CV_Comp;
                  CSV_Bool: v1^.CV_Bool := v2^.CV_Bool;
                  CSV_Array: begin
                      v1^.cv_ArrItems.Create;
                      for i := 1 to v2^.CV_ArrItems.Count - 1 do
                      begin
                        AddArrayVar(v1^.CV_ArrItems, v2^.CV_ArrItems.GetItem(i));
                      end;
                    end;
                end;
              end;
          end;
      end;
    ptMinus:
      case v1^.VType of
        CSV_UByte:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.CV_UByte - GetReal(v2))
            else
              v1^.CV_UByte := v1^.CV_UByte - GetInt(V2);
          end;
        CSV_SByte:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.CV_SByte - GetReal(v2))
            else
              v1^.CV_SByte := v1^.CV_SByte - GetInt(V2);
          end;
        CSV_UInt16:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.Cv_UInt16 - GetReal(v2))
            else
              v1^.CV_UInt16 := v1^.CV_UInt16 - GetInt(V2);
          end;
        CSV_SInt16:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.CV_SInt16 - GetReal(v2))
            else
              v1^.CV_SInt16 := v1^.CV_SInt16 - GetInt(V2);
          end;
        CSV_UInt32:
          begin
            if IsRealType(v2) then
              MakeItReal(V1^.Cv_Uint32 - GetReal(v2))
            else
              v1^.CV_UInt32 := v1^.CV_UInt32 - GetInt(V2);
          end;
        CSV_SInt32:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.Cv_Sint32 - GetReal(v2))
            else
              v1^.CV_SInt32 := v1^.CV_SInt32 - GetInt(V2);
          end;
        CSV_Real:
          begin
            v1^.CV_Real := v1^.CV_Real - GetReal(V2);
          end;
        CSV_Single:
          begin
            v1^.CV_Single := v1^.CV_Single - GetReal(V2);
          end;
        CSV_Double:
          begin
            v1^.CV_Double := v1^.CV_Double - GetReal(V2);
          end;
        CSV_Extended:
          begin
            v1^.CV_Extended := v1^.CV_Extended - GetReal(V2);
          end;
        CSV_Comp:
          begin
            v1^.cv_Comp := v1^.cv_Comp - round(GetReal(V2));
          end;
      else
        Err := True;
      end { CASE };
    ptPlus:
      case v1^.VType of
        CSV_UByte:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.CV_UByte + GetReal(v2))
            else
              v1^.CV_UByte := v1^.CV_UByte + GetInt(V2);
          end;
        CSV_SByte:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.CV_SByte + GetReal(v2))
            else
              v1^.CV_SByte := v1^.CV_SByte + GetInt(V2);
          end;
        CSV_UInt16:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.Cv_UInt16 + GetReal(v2))
            else
              v1^.CV_UInt16 := v1^.CV_UInt16 + GetInt(V2);
          end;
        CSV_SInt16:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.CV_SInt16 + GetReal(v2))
            else
              v1^.CV_SInt16 := v1^.CV_SInt16 + GetInt(V2);
          end;
        CSV_UInt32:
          begin
            if IsRealType(v2) then
              MakeItReal(V1^.Cv_Uint32 + GetReal(v2))
            else
              v1^.CV_UInt32 := v1^.CV_UInt32 + GetInt(V2);
          end;
        CSV_SInt32:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.Cv_Sint32 + GetReal(v2))
            else
              v1^.CV_SInt32 := v1^.CV_SInt32 + GetInt(V2);
          end;
        CSV_Real:
          begin
            v1^.CV_Real := v1^.CV_Real + GetReal(V2);
          end;
        CSV_Single:
          begin
            v1^.CV_Single := v1^.CV_Single + GetReal(V2);
          end;
        CSV_Double:
          begin
            v1^.CV_Double := v1^.CV_Double + GetReal(V2);
          end;
        CSV_Extended:
          begin
            v1^.CV_Extended := v1^.CV_Extended + GetReal(V2);
          end;
        CSV_Comp:
          begin
            v1^.cv_Comp := v1^.cv_Comp + round(GetReal(V2));
          end;
        CSV_String:
          begin
            v1^.cv_Str := v1^.cv_str + GetStr(v2);
          end;
      else
        Err := True;
      end { CASE };
    ptMul:
      case v1^.VType of
        CSV_UByte:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.CV_UByte * GetReal(v2))
            else
              v1^.CV_UByte := v1^.CV_UByte * GetInt(V2);
          end;
        CSV_SByte:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.CV_SByte * GetReal(v2))
            else
              v1^.CV_SByte := v1^.CV_SByte * GetInt(V2);
          end;
        CSV_UInt16:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.Cv_UInt16 * GetReal(v2))
            else
              v1^.CV_UInt16 := v1^.CV_UInt16 * GetInt(V2);
          end;
        CSV_SInt16:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.CV_SInt16 * GetReal(v2))
            else
              v1^.CV_SInt16 := v1^.CV_SInt16 * GetInt(V2);
          end;
        CSV_UInt32:
          begin
            if IsRealType(v2) then
              MakeItReal(V1^.Cv_Uint32 * GetReal(v2))
            else
              v1^.CV_UInt32 := v1^.CV_UInt32 * GetInt(V2);
          end;
        CSV_SInt32:
          begin
            if IsRealType(v2) then
              MakeItReal(v1^.Cv_Sint32 * GetReal(v2))
            else
              v1^.CV_SInt32 := v1^.CV_SInt32 * GetInt(V2);
          end;
        CSV_Real:
          begin
            v1^.CV_Real := v1^.CV_Real * GetReal(V2);
          end;
        CSV_Single:
          begin
            v1^.CV_Single := v1^.CV_Single * GetReal(V2);
          end;
        CSV_Double:
          begin
            v1^.CV_Double := v1^.CV_Double * GetReal(V2);
          end;
        CSV_Extended:
          begin
            v1^.CV_Extended := v1^.CV_Extended * GetReal(V2);
          end;
        CSV_Comp:
          begin
            v1^.cv_Comp := round(v1^.cv_Comp * GetReal(V2));
          end;
      else
        Err := True;
      end { CASE };
    ptDiv:
      begin
        case v1^.VType of
          CSV_UByte:
            begin
              MakeItReal(v1^.CV_UByte / GetReal(v2));
            end;
          CSV_SByte:
            begin
              MakeItReal(v1^.CV_SByte / GetReal(v2));
            end;
          CSV_UInt16:
            begin
              MakeItReal(v1^.CV_UInt16 / GetReal(v2));
            end;
          CSV_SInt16:
            begin
              MakeItReal(v1^.CV_SInt16 / GetReal(v2));
            end;
          CSV_UInt32:
            begin
              MakeItReal(v1^.CV_UInt32 / GetReal(v2));
            end;
          CSV_SInt32:
            begin
              MakeItReal(v1^.Cv_Sint32 / GetReal(v2));
            end;
          CSV_Real:
            begin
              v1^.CV_Real := v1^.CV_Real / GetReal(V2);
            end;
          CSV_Single:
            begin
              v1^.CV_Single := v1^.CV_Single / GetReal(V2);
            end;
          CSV_Double:
            begin
              v1^.CV_Double := v1^.CV_Double / GetReal(V2);
            end;
          CSV_Extended:
            begin
              v1^.CV_Extended := v1^.CV_Extended / GetReal(V2);
            end;
          CSV_Comp:
            begin
              v1^.cv_Comp := round(v1^.cv_Comp / GetReal(V2));
            end;
        else
          Err := True;
        end { CASE };
      end; { begin }
    ptIntDiv:
      begin
        if not isIntegerType(V2) then
        begin
          Perform := False;
          Exit;
        end;
        case v1^.VType of
          CSV_UByte:
            begin
              v1^.CV_UByte := v1^.CV_UByte div GetInt(V2);
            end;
          CSV_SByte:
            begin
              v1^.CV_SByte := v1^.CV_SByte div GetInt(V2);
            end;
          CSV_UInt16:
            begin
              v1^.CV_UInt16 := v1^.CV_UInt16 div GetInt(V2);
            end;
          CSV_SInt16:
            begin
              v1^.CV_SInt16 := v1^.CV_SInt16 div GetInt(V2);
            end;
          CSV_UInt32:
            begin
              v1^.CV_UInt32 := v1^.CV_UInt32 div GetInt(V2);
            end;
          CSV_SInt32:
            begin
              v1^.CV_SInt32 := v1^.CV_SInt32 div GetInt(V2);
            end;
        else
          Err := True;
        end;
      end;
    ptIntMod:
      begin
        if not isIntegerType(V2) then
        begin
          Perform := false;
          Exit;
        end;
        case v1^.VType of
          CSV_UByte:
            begin
              v1^.CV_UByte := v1^.CV_UByte mod GetInt(V2);
            end;
          CSV_SByte:
            begin
              v1^.CV_SByte := v1^.CV_SByte mod GetInt(V2);
            end;
          CSV_UInt16:
            begin
              v1^.CV_UInt16 := v1^.CV_UInt16 mod GetInt(V2);
            end;
          CSV_SInt16:
            begin
              v1^.CV_SInt16 := v1^.CV_SInt16 mod GetInt(V2);
            end;
          CSV_UInt32:
            begin
              v1^.CV_UInt32 := v1^.CV_UInt32 mod GetInt(V2);
            end;
          CSV_SInt32:
            begin
              v1^.CV_SInt32 := v1^.CV_SInt32 mod GetInt(V2);
            end;
        else
          Err := True;
        end;
      end;
    ptAnd:
      begin
        if (not isIntegerType(V2)) and (not ISBooleanType(v2)) then
        begin
          Perform := False;
          Exit;
        end;
        case v1^.VType of
          CSV_UByte:
            begin
              v1^.CV_UByte := v1^.CV_UByte and GetInt(V2);
            end;
          CSV_SByte:
            begin
              v1^.CV_SByte := v1^.CV_SByte and GetInt(V2);
            end;
          CSV_UInt16:
            begin
              v1^.CV_UInt16 := v1^.CV_UInt16 and GetInt(V2);
            end;
          CSV_SInt16:
            begin
              v1^.CV_SInt16 := v1^.CV_SInt16 and GetInt(V2);
            end;
          CSV_UInt32:
            begin
              v1^.CV_UInt32 := v1^.CV_UInt32 and GetInt(V2);
            end;
          CSV_SInt32:
            begin
              v1^.CV_SInt32 := v1^.CV_SInt32 and GetInt(V2);
            end;
          CSV_Bool:
            begin
              v1^.CV_Bool := v1^.CV_Bool and GetBool(V2);
            end;
        else
          Err := True;
        end;
      end;
    ptOr:
      begin
        if (not isIntegerType(V2)) and (not ISBooleanType(v2)) then
        begin
          Perform := False;
          Exit;
        end;
        case v1^.VType of
          CSV_UByte:
            begin
              v1^.CV_UByte := v1^.CV_UByte or GetInt(V2);
            end;
          CSV_SByte:
            begin
              v1^.CV_SByte := v1^.CV_SByte or GetInt(V2);
            end;
          CSV_UInt16:
            begin
              v1^.CV_UInt16 := v1^.CV_UInt16 or GetInt(V2);
            end;
          CSV_SInt16:
            begin
              v1^.CV_SInt16 := v1^.CV_SInt16 or GetInt(V2);
            end;
          CSV_UInt32:
            begin
              v1^.CV_UInt32 := v1^.CV_UInt32 or GetInt(V2);
            end;
          CSV_SInt32:
            begin
              v1^.CV_SInt32 := v1^.CV_SInt32 or GetInt(V2);
            end;
          CSV_Bool:
            begin
              v1^.CV_Bool := v1^.CV_Bool or GetBool(V2);
            end;
        else
          Err := True;
        end;
      end;
    ptXor:
      begin
        if (not isIntegerType(V2)) and (not ISBooleanType(v2)) then
        begin
          Perform := False;
          Exit;
        end;
        case v1^.VType of
          CSV_UByte:
            begin
              v1^.CV_UByte := v1^.CV_UByte xor GetInt(V2);
            end;
          CSV_SByte:
            begin
              v1^.CV_SByte := v1^.CV_SByte xor GetInt(V2);
            end;
          CSV_UInt16:
            begin
              v1^.CV_UInt16 := v1^.CV_UInt16 xor GetInt(V2);
            end;
          CSV_SInt16:
            begin
              v1^.CV_SInt16 := v1^.CV_SInt16 xor GetInt(V2);
            end;
          CSV_UInt32:
            begin
              v1^.CV_UInt32 := v1^.CV_UInt32 xor GetInt(V2);
            end;
          CSV_SInt32:
            begin
              v1^.CV_SInt32 := v1^.CV_SInt32 xor GetInt(V2);
            end;
          CSV_Bool:
            begin
              v1^.CV_Bool := v1^.CV_Bool xor GetBool(V2);
            end;
        else
          Err := True;
        end;
      end;
    ptShr:
      begin
        if not isIntegerType(V2) then
        begin
          Perform := True;
          Exit;
        end;
        case v1^.VType of
          CSV_UByte:
            begin
              v1^.CV_UByte := v1^.CV_UByte shr GetInt(V2);
            end;
          CSV_SByte:
            begin
              v1^.CV_SByte := v1^.CV_SByte shr GetInt(V2);
            end;
          CSV_UInt16:
            begin
              v1^.CV_UInt16 := v1^.CV_UInt16 shr GetInt(V2);
            end;
          CSV_SInt16:
            begin
              v1^.CV_SInt16 := v1^.CV_SInt16 shr GetInt(V2);
            end;
          CSV_UInt32:
            begin
              v1^.CV_UInt32 := v1^.CV_UInt32 shr GetInt(V2);
            end;
          CSV_SInt32:
            begin
              v1^.CV_SInt32 := v1^.CV_SInt32 shr GetInt(V2);
            end;
        else
          Err := True;
        end;
      end;
    ptShl:
      begin
        if not isIntegerType(V2) then
        begin
          Perform := True;
          Exit;
        end;
        case v1^.VType of
          CSV_UByte:
            begin
              v1^.CV_UByte := v1^.CV_UByte shl GetInt(V2);
            end;
          CSV_SByte:
            begin
              v1^.CV_SByte := v1^.CV_SByte shl GetInt(V2);
            end;
          CSV_UInt16:
            begin
              v1^.CV_UInt16 := v1^.CV_UInt16 shl GetInt(V2);
            end;
          CSV_SInt16:
            begin
              v1^.CV_SInt16 := v1^.CV_SInt16 shl GetInt(V2);
            end;
          CSV_UInt32:
            begin
              v1^.CV_UInt32 := v1^.CV_UInt32 shl GetInt(V2);
            end;
          CSV_SInt32:
            begin
              v1^.CV_SInt32 := v1^.CV_SInt32 shl GetInt(V2);
            end;
        else
          Err := True;
        end;
      end;
    PtGreater: case V1^.VType of
        CSV_UByte: if IsRealType(v2) then
            MakeItBool(V1^.Cv_UByte > GetReal(v2))
          else
            MakeItBool(v1^.Cv_UByte > GetInt(V2));
        CSV_SByte: if IsRealType(v2) then
            MakeItBool(V1^.Cv_SByte > GetReal(v2))
          else
            MakeItBool(v1^.Cv_SByte > GetInt(V2));
        CSV_Char: if v2^.VType = CSV_Char then
            MakeItBool(V1^.Cv_Char > v2^.CV_Char)
          else
            Err := True;
        CSV_UInt16: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Uint16 > GetReal(v2))
          else
            MakeItBool(v1^.CV_UInt16 > GetInt(V2));
        CSV_SInt16: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Sint16 > GetReal(v2))
          else
            MakeItBool(v1^.CV_SInt16 > GetInt(V2));
        CSV_UInt32: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Uint32 > GetReal(v2))
          else
            MakeItBool(v1^.CV_UInt32 > GetInt(V2));
        CSV_SInt32: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Sint32 > GetReal(v2))
          else
            MakeItBool(v1^.CV_SInt32 > GetInt(V2));
        CSV_Real: MakeItBool(V1^.Cv_Real > GetReal(v2));
        CSV_Single: MakeItBool(V1^.Cv_Single > GetReal(v2));
        CSV_Double: MakeItBool(V1^.Cv_Double > GetReal(v2));
        CSV_Extended: MakeItBool(V1^.Cv_Extended > GetReal(v2));
        CSV_Comp: MakeItBool(V1^.Cv_Comp > GetReal(v2));
        CSV_Bool: MakeItBool(V1^.Cv_Bool > V2^.Cv_Bool);
        CSV_String: MakeItBool(V1^.Cv_Str > V2^.Cv_Str);
      else Err := True;
      end; {case item}
    PtLess: case V1^.VType of
        CSV_UByte: if IsRealType(v2) then
            MakeItBool(V1^.Cv_UByte < GetReal(v2))
          else
            MakeItBool(v1^.Cv_UByte < GetInt(V2));
        CSV_SByte: if IsRealType(v2) then
            MakeItBool(V1^.Cv_SByte < GetReal(v2))
          else
            MakeItBool(v1^.Cv_SByte < GetInt(V2));
        CSV_Char: if v2^.VType = CSV_Char then
            MakeItBool(V1^.Cv_Char < v2^.CV_Char)
          else
            Err := True;
        CSV_UInt16: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Uint16 < GetReal(v2))
          else
            MakeItBool(v1^.CV_UInt16 < GetInt(V2));
        CSV_SInt16: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Sint16 < GetReal(v2))
          else
            MakeItBool(v1^.CV_SInt16 < GetInt(V2));
        CSV_UInt32: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Uint32 < GetReal(v2))
          else
            MakeItBool(v1^.CV_UInt32 < GetInt(V2));
        CSV_SInt32: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Sint32 < GetReal(v2))
          else
            MakeItBool(v1^.CV_SInt32 < GetInt(V2));
        CSV_Real: MakeItBool(V1^.Cv_Real < GetReal(v2));
        CSV_Single: MakeItBool(V1^.Cv_Single < GetReal(v2));
        CSV_Double: MakeItBool(V1^.Cv_Double < GetReal(v2));
        CSV_Extended: MakeItBool(V1^.Cv_Extended < GetReal(v2));
        CSV_Comp: MakeItBool(V1^.Cv_Comp < GetReal(v2));
        CSV_Bool: MakeItBool(V1^.Cv_Bool < V2^.Cv_Bool);
        CSV_String: MakeItBool(V1^.Cv_Str < V2^.Cv_Str);
      else Err := True;
      end; {case item}
    PtGreaterEqual: case V1^.VType of
        CSV_UByte: if IsRealType(v2) then
            MakeItBool(V1^.Cv_UByte >= GetReal(v2))
          else
            MakeItBool(v1^.Cv_UByte >= GetInt(V2));
        CSV_SByte: if IsRealType(v2) then
            MakeItBool(V1^.Cv_SByte >= GetReal(v2))
          else
            MakeItBool(v1^.Cv_SByte >= GetInt(V2));
        CSV_Char: if v2^.VType = CSV_Char then
            MakeItBool(V1^.Cv_Char >= v2^.CV_Char)
          else
            Err := True;
        CSV_UInt16: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Uint16 >= GetReal(v2))
          else
            MakeItBool(v1^.CV_UInt16 >= GetInt(V2));
        CSV_SInt16: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Sint16 >= GetReal(v2))
          else
            MakeItBool(v1^.CV_SInt16 >= GetInt(V2));
        CSV_UInt32: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Uint32 >= GetReal(v2))
          else
            MakeItBool(v1^.CV_UInt32 >= GetInt(V2));
        CSV_SInt32: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Sint32 >= GetReal(v2))
          else
            MakeItBool(v1^.CV_SInt32 >= GetInt(V2));
        CSV_Real: MakeItBool(V1^.Cv_Real >= GetReal(v2));
        CSV_Single: MakeItBool(V1^.Cv_Single >= GetReal(v2));
        CSV_Double: MakeItBool(V1^.Cv_Double >= GetReal(v2));
        CSV_Extended: MakeItBool(V1^.Cv_Extended >= GetReal(v2));
        CSV_Comp: MakeItBool(V1^.Cv_Comp >= GetReal(v2));
        CSV_Bool: MakeItBool(V1^.Cv_Bool >= V2^.Cv_Bool);
        CSV_String: MakeItBool(V1^.Cv_Str >= V2^.Cv_Str);
      else Err := True;
      end; {case item}
    PtLessEqual: case V1^.VType of
        CSV_UByte: if IsRealType(v2) then
            MakeItBool(V1^.Cv_UByte <= GetReal(v2))
          else
            MakeItBool(v1^.Cv_UByte <= GetInt(V2));
        CSV_SByte: if IsRealType(v2) then
            MakeItBool(V1^.Cv_SByte <= GetReal(v2))
          else
            MakeItBool(v1^.Cv_SByte <= GetInt(V2));
        CSV_Char: if v2^.VType = CSV_Char then
            MakeItBool(V1^.Cv_Char <= v2^.CV_Char)
          else
            Err := True;
        CSV_UInt16: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Uint16 <= GetReal(v2))
          else
            MakeItBool(v1^.CV_UInt16 <= GetInt(V2));
        CSV_SInt16: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Sint16 <= GetReal(v2))
          else
            MakeItBool(v1^.CV_SInt16 <= GetInt(V2));
        CSV_UInt32: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Uint32 <= GetReal(v2))
          else
            MakeItBool(v1^.CV_UInt32 <= GetInt(V2));
        CSV_SInt32: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Sint32 <= GetReal(v2))
          else
            MakeItBool(v1^.CV_SInt32 <= GetInt(V2));
        CSV_Real: MakeItBool(V1^.Cv_Real <= GetReal(v2));
        CSV_Single: MakeItBool(V1^.Cv_Single <= GetReal(v2));
        CSV_Double: MakeItBool(V1^.Cv_Double <= GetReal(v2));
        CSV_Extended: MakeItBool(V1^.Cv_Extended <= GetReal(v2));
        CSV_Comp: MakeItBool(V1^.Cv_Comp <= GetReal(v2));
        CSV_Bool: MakeItBool(V1^.Cv_Bool <= V2^.Cv_Bool);
        CSV_String: MakeItBool(V1^.Cv_Str <= V2^.Cv_Str);
      else Err := True;
      end; {case item}
    PtEqual: case V1^.VType of
        CSV_UByte: if IsRealType(v2) then
            MakeItBool(V1^.Cv_UByte = GetReal(v2))
          else
            MakeItBool(v1^.Cv_UByte = GetInt(V2));
        CSV_SByte: if IsRealType(v2) then
            MakeItBool(V1^.Cv_SByte = GetReal(v2))
          else
            MakeItBool(v1^.Cv_SByte = GetInt(V2));
        CSV_Char: if v2^.VType = CSV_Char then
            MakeItBool(V1^.Cv_Char = v2^.CV_Char)
          else
            Err := True;
        CSV_UInt16: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Uint16 = GetReal(v2))
          else
            MakeItBool(v1^.CV_UInt16 = GetInt(V2));
        CSV_SInt16: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Sint16 = GetReal(v2))
          else
            MakeItBool(v1^.CV_SInt16 = GetInt(V2));
        CSV_UInt32: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Uint32 = GetReal(v2))
          else
            MakeItBool(v1^.CV_UInt32 = GetInt(V2));
        CSV_SInt32: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Sint32 = GetReal(v2))
          else
            MakeItBool(v1^.CV_SInt32 = GetInt(V2));
        CSV_Real: MakeItBool(V1^.Cv_Real = GetReal(v2));
        CSV_Single: MakeItBool(V1^.Cv_Single = GetReal(v2));
        CSV_Double: MakeItBool(V1^.Cv_Double = GetReal(v2));
        CSV_Extended: MakeItBool(V1^.Cv_Extended = GetReal(v2));
        CSV_Comp: MakeItBool(V1^.Cv_Comp = GetReal(v2));
        CSV_Bool: MakeItBool(V1^.Cv_Bool = V2^.Cv_Bool);
        CSV_String: MakeItBool(v1^.Cv_Str = v2^.Cv_Str)
      else
        Err := True;
      end; {case item}
    PtNotEqual: case V1^.VType of
        CSV_UByte: if IsRealType(v2) then
            MakeItBool(V1^.Cv_UByte <> GetReal(v2))
          else
            MakeItBool(v1^.Cv_UByte <> GetInt(V2));
        CSV_SByte: if IsRealType(v2) then
            MakeItBool(V1^.Cv_SByte <> GetReal(v2))
          else
            MakeItBool(v1^.Cv_SByte <> GetInt(V2));
        CSV_Char: if v2^.VType = CSV_Char then
            MakeItBool(V1^.Cv_Char <> v2^.CV_Char)
          else
            Err := True;
        CSV_UInt16: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Uint16 <> GetReal(v2))
          else
            MakeItBool(v1^.CV_UInt16 <> GetInt(V2));
        CSV_SInt16: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Sint16 <> GetReal(v2))
          else
            MakeItBool(v1^.CV_SInt16 <> GetInt(V2));
        CSV_UInt32: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Uint32 <> GetReal(v2))
          else
            MakeItBool(v1^.CV_UInt32 <> GetInt(V2));
        CSV_SInt32: if IsRealType(v2) then
            MakeItBool(V1^.Cv_Sint32 <> GetReal(v2))
          else
            MakeItBool(v1^.CV_SInt32 <> GetInt(V2));
        CSV_Real: MakeItBool(V1^.Cv_Real <> GetReal(v2));
        CSV_Single: MakeItBool(V1^.Cv_Single <> GetReal(v2));
        CSV_Double: MakeItBool(V1^.Cv_Double <> GetReal(v2));
        CSV_Extended: MakeItBool(V1^.Cv_Extended <> GetReal(v2));
        CSV_Comp: MakeItBool(V1^.Cv_Comp <> GetReal(v2));
        CSV_Bool: MakeItBool(V1^.Cv_Bool <> V2^.Cv_Bool);
        CSV_String: MakeItBool(v1^.Cv_Str <> v2^.Cv_Str)
      else
        Err := True;
      end; {case item}
  end;
  PerForm := not Err;
end;
{$IFDEF DELPHI}{$WARNINGS ON}{$ENDIF}
end.

