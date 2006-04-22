unit SRK;
{Copyright 2000 by Matt Henley Released under the GNU General License (GPL) V2 }
interface
uses SimBaseTypes;
type
  TSRK = class (TEOS)
    function GetFugasityCoef(var Comp: CompArray;Z, t, p: double): CompArray; 
            override;
    procedure GetResiduals(var Phase: TPhase;var EnthalpyRes, EntropyRes: 
            double); override;
    function SolveEOS(var Comp: CompArray;t, p: double): cuberootreturn; 
            override;
  end;
  
implementation

{
************************************* TSRK *************************************
}
function TSRK.GetFugasityCoef(var Comp: CompArray;Z, t, p: double): CompArray;
var
  V, test: Double;
  a, bigA: Double;
  b, bigB: Double;
  tempsum: Double;
  i, j: Integer;
  ai, bi, mi: CompArray;
begin
  b := 0.0;
  for i := 1 to NumComp do begin
    mi[i] := 0.48 + 1.574 * w[i] - 0.176 * Sqr(w[i]);
    bi[i] := 0.08664 * R * Tc[i] / Pc[i];
    ai[i] := 0.42748 * Sqr(R * Tc[i]) / Pc[i] * Sqr(1 + mi[i] * (1 - Sqrt(t / Tc[i])));
    b := b + Comp[i] * bi[i];
  end;
  bigB := b * p / (R * t);
  a := 0;
  for i := 1 to NumComp do
    for j := 1 to NumComp do
      a := a + Comp[i] * Comp[j] * Sqrt(ai[i] * ai[j]);
  bigA := a * p / (Sqr(R * t));
  for i := 1 to NumComp do begin
    test := bi[i] / b * (Z - 1.0) - ln(Z - bigB) - bigA / bigB * (2.0 * Sqrt(ai[i] / a) - bi[i] / b) *
      ln(1 + bigB / Z);
    GetFugasityCoef[i] := exp(test) * p * Comp[i];
  end;
end;

procedure TSRK.GetResiduals(var Phase: TPhase;var EnthalpyRes, EntropyRes: 
        double);
var
  MainA: Double;
  MainB: Double;
  a: Double;
  b: Double;
  i, j: Integer;
  ai, bi, mi: CompArray;
  dadt: Double;
  P0: Double;
  
  {Reference Pressure... i assume to be 1 atms}
  
begin
  P0 := 1.0;
  b := 0.0;
  for i := 1 to NumComp do begin
    mi[i] := 0.48 + 1.574 * w[i] - 0.176 * Sqr(w[i]);
    bi[i] := 0.08664 * R * Tc[i] / Pc[i];
    ai[i] := 0.42748 * Sqr(R * Tc[i]) / Pc[i] * Sqr(1 + mi[i] * (1 - Sqrt(Phase.t / Tc[i])));
    b := b + Phase.Comp[i] * bi[i];
  end;
  a := 0;
  for i := 1 to NumComp do
    for j := 1 to NumComp do
      a := a + Phase.Comp[i] * Phase.Comp[j] * Sqrt(ai[i] * ai[j]);
  MainA := a * Phase.p / (Sqr(R * Phase.t));
  MainB := b * Phase.p / (R * Phase.t);
  EnthalpyRes := (Phase.Z - 1.0 - 1.0 / (b * R * Phase.t) * (a - Phase.t * dadt) * ln(1 + b / Phase.V)) * R * Phase.t;
  EntropyRes := (ln(Phase.Z - b) - ln(Phase.p / P0) + MainA / MainB * (Phase.t / a * dadt) * ln(1 + MainB / Phase.Z)) * R * Phase.t;
  
end;

function TSRK.SolveEOS(var Comp: CompArray;t, p: double): cuberootreturn;
var
  MainA: Double;
  MainB: Double;
  a: Double;
  b: Double;
  i, j: Integer;
  ai, bi, mi: CompArray;
begin
  b := 0.0;
  for i := 1 to NumComp do begin
    mi[i] := 0.48 + 1.574 * w[i] - 0.176 * Sqr(w[i]);
    bi[i] := 0.08664 * R * Tc[i] / Pc[i];
    ai[i] := 0.42748 * Sqr(R * Tc[i]) / Pc[i] * Sqr(1 + mi[i] * (1 - Sqrt(t / Tc[i])));
    b := b + Comp[i] * bi[i];
  end;
  a := 0;
  for i := 1 to NumComp do
    for j := 1 to NumComp do
      a := a + Comp[i] * Comp[j] * Sqrt(ai[i] * ai[j]);
  MainA := a * p / (Sqr(R * t));
  MainB := b * p / (R * T);
  SolveEOS := cuberoot(-1.0, MainA - MainB - Sqr(MainB), -1.0 * MainA * MainB);
end;

end.


