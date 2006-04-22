unit SimBaseTypes;
{Copyright 2000 by Matt Henley Released under the GNU General License (GPL) V2 }
interface
uses math;
const
  MaxComp = 32;
  R: Double = 0.08206; {atm*m^3/(kgmol*K)} {temporary}
type
  ExistArray = array[1..3] of Boolean; {1-Vapor 2-Liq1 3-Liq2                 }
  cuberootreturn = array[1..3] of Double;
  compArray = array[1..MaxComp] of Double;
  compIntArray = array[1..MaxComp] of Integer;
  TPhase = class (TObject)
    Comp: compArray;
    Enthalpy: Double;
    EnthRes: Double;
    Entropy: Double;
    EntrRes: Double;
    Exist: Boolean;
    Flow: Double;
    mw: Double;
    p: Double;
    t: Double;
    V: Double;
    Z: Double;
    procedure clearphase;
  end;
  
  TStrm = class (TObject)
    comp: compArray;
    Enthalpy: Double;
    Entropy: Double;
    Flow: Double;
    Liq1: TPhase;
    Liq2: TPhase;
    mw: Double;
    p: Double;
    t: Double;
    Vap: TPhase;
    constructor Create;
    procedure clearstrm;
  end;
  
  TEOS = class (TObject)
    NumComp: Integer;
    Pc: CompArray;
    Tc: CompArray;
    W: CompArray;
    function EstK1Values(t, p: double): CompArray; virtual;
    function EstK2Values(t, p: double): CompArray; virtual;
    function GetFugasityCoef(var Comp: CompArray;Z, t, p: double): CompArray; 
            virtual; abstract;
    function GetK(var OldK, VComp, LComp: CompArray;out ZVap, Zliq: Double;t, 
            p: double): CompArray; virtual;
    procedure GetResiduals(var Phase: TPhase;var EnthalpyRes, EntropyRes: 
            double); virtual; abstract;
    function GetZLiq(var EOSReturn: cuberootreturn): Double; virtual;
    function GetZVap(var EOSReturn: cuberootreturn): Double; virtual;
    procedure InitComponents(var Components: compintarray;num: integer); 
            virtual;
    function SolveEOS(var Comp: CompArray;t, p: double): cuberootreturn; 
            virtual; abstract;
  end;
  
function CubeRoot(A, B, C: Double): cuberootreturn;

implementation

{
************************************ TPhase ************************************
}
procedure TPhase.clearphase;
var
  i: Integer;
begin
  Exist := False;
  for i := 1 to MaxComp do comp[i] := 0.0;
  mw := 0.0;
  Flow := 0.0;
  Enthalpy := 0.0;
  EnthRes := 0.0;
  Entropy := 0.0;
  EntrRes := 0.0;
  Z := 0.0;
  V := 0.0;
end;

{
************************************ TStrm *************************************
}
constructor TStrm.Create;
begin
  inherited Create;
  Vap := TPhase.Create;
  Liq1 := TPhase.Create;
  Liq2 := TPhase.Create;
  clearstrm;
end;

procedure TStrm.clearstrm;
var
  i: Integer;
begin
  for i := 1 to MaxComp do comp[i] := 0.0;
  mw := 0.0;
  Flow := 0.0;
  Enthalpy := 0.0;
  Entropy := 0.0;
  t := 0.0;
  p := 0.0;
  Vap.clearphase;
  Liq1.clearphase;
  Liq2.clearphase;
end;

{
************************************* TEOS *************************************
}
function TEOS.EstK1Values(t, p: double): CompArray;
var
  i: Integer;
begin
  for i := 1 to NumComp do begin
    EstK1Values[i] := exp(5.3727 * (1 + W[i]) * (1 - 1 / (t / Tc[i]))) / (p / Pc[i]);
  end;
end;

function TEOS.EstK2Values(t, p: double): CompArray;
var
  i: Integer;
begin
  for i := 1 to NumComp do begin
    EstK2Values[i] := 1000000.0 * (t / Tc[i]) / (p / Pc[i]);
  end;
end;

function TEOS.GetK(var OldK, VComp, LComp: CompArray;out ZVap, Zliq: Double;t, 
        p: double): CompArray;
var
  i: Integer;
  FugCoefVap, FugCoefLiq: CompArray;
  EOSReturnValue: cuberootreturn;
begin
  EOSReturnValue := SolveEOS(VComp, t, p);
  ZVap := GetZVap(EOSReturnValue);
  EOSReturnValue := SolveEOS(LComp, t, p);
  ZLiq := GetZLiq(EOSReturnValue);
  FugCoefVap := GetFugasityCoef(VComp, ZVap, t, p);
  FugCoefLiq := GetFugasityCoef(LComp, ZLiq, t, p);
  for i := 1 to NumComp do
    GetK[i] := FugCoefLiq[i] / FugCoefVap[i] * OldK[i];
end;

function TEOS.GetZLiq(var EOSReturn: cuberootreturn): Double;
var
  MinimumTemp: Double;
  i: Integer;
  
  {Find the minimum positive real root}
  
begin
  if EOSReturn[2] = 0.0 {If [2] is 0, there was only one result} then GetZLiq := EOSReturn[1]
  else begin
    MinimumTemp := MaxDouble;
    for i := 1 to 3 do
      if (EOSReturn[i] > 0.0) and (EOSReturn[i] < MinimumTemp) then MinimumTemp := EOSReturn[i];
    GetZLiq := MinimumTemp
  end;
end;

function TEOS.GetZVap(var EOSReturn: cuberootreturn): Double;
begin
  GetZVap := MaxValue(EOSReturn);
end;

procedure TEOS.InitComponents(var Components: compintarray;num: integer);
begin
  NumComp := 3;
  Tc[1] := 305.4;
  Tc[2] := 568.8;
  Tc[3] := 647.3;
  Pc[1] := 48.20;
  Pc[2] := 24.62;
  Pc[3] := 214.75;
  W[1] := 0.0980;
  W[2] := 0.3940;
  W[3] := 0.3440;
end;

function CubeRoot(A, B, C: Double): cuberootreturn;
var
  Q: Double;
  R: Double;
  S: Double;
  T: Double;
  theta: Double;
begin
  Q := (Sqr(A) - 3.0 * B) / 9.0;
  R := (2.0 * IntPower(A, 3) - 9.0 * A * B + 27 * C) / 54.0;
  if (Sqr(R) < IntPower(Q, 3)) then begin
    theta := ArcCos(R / (Sqrt(IntPower(Q, 3))));
    CubeRoot[1] := -2.0 * Sqrt(Q) * cos(theta / 3.0) - A / 3.0;
    CubeRoot[2] := -2.0 * Sqrt(Q) * cos((theta + 2.0 * pi) / 3.0) - A / 3.0;
    CubeRoot[3] := -2.0 * Sqrt(Q) * cos((theta - 2.0 * pi) / 3.0) - A / 3.0;
  end
  else begin
    S := abs(R) + Sqrt(Sqr(R) - IntPower(Q, 3));
    S := power(S, 0.3333333);
    S := -1.0 * sign(R) * S;
    if S = 0 then
      T := 0
    else
      T := Q / S;
    CubeRoot[1] := (S + T) - A / 3.0;
    CubeRoot[2] := 0.0;
    CubeRoot[3] := 0.0;
  end;
end;

end.
.
