{ $Id$ }
{
 /***************************************************************************

                  Abstract: Classes and routines related to equilibrium
                            calculations.
                  Initial Revision : 21/04/2006
                  Authors:
                    - Samuel Jorge Marques Cartaxo
                    - Matt Henley
                    - Hazem Haddad
                    - Additional contributors...

 ***************************************************************************/

 *****************************************************************************
 *                                                                           *
 *  This file is part of the OpSim - OPEN SOURCE PROCESS SIMULATOR           *
 *                                                                           *
 *  See the file COPYING.GPL, included in this distribution,                 *
 *  for details about the copyright.                                         *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                     *
 *                                                                           *
 *****************************************************************************
}
unit Equilibrium;

interface

uses
  SysUtils, Classes, Variants, Entities, Thermo;

type
  TFlash = class (TObject)
    function EstK1Values(ACompounds: TCompounds; P, T: Real): Variant;
    function EstK2Values(ACompounds: TCompounds; P, T: Real): Variant;
    procedure TP(AMaterial: TMaterial);
  end;
  
  TEquilibriumServer = class (TObject)
  public
    {{
    - This routine will calculate the equilibrium properties for the given 
    material object by performing a flash calculation.
    - It may delegate the operation to specialized service flash classes.
    }
    procedure CalcEquilibrium(AMaterial: TMaterial);
  end;
  
implementation

{
************************************ TFlash ************************************
}
function TFlash.EstK1Values(ACompounds: TCompounds; P, T: Real): Variant;
var
  i: Integer;
begin
  with ACompounds do begin
    //Makes room for the result according to the number of compounds
    Result := VarArrayCreate([0, Count - 1], varDouble);
    for i := 0 to Count - 1 do
      with Compounds[I] do
        Result[i] := exp(5.3727 * (1 + W.Value) * (1 - 1 / (t / Tc.Value))) / (p / Pc.Value);
  end;//with
end;

function TFlash.EstK2Values(ACompounds: TCompounds; P, T: Real): Variant;
var
  i: Integer;
begin
  with ACompounds do begin
    //Makes room for the result according to the number of compounds
    Result := VarArrayCreate([0, Count - 1], varDouble);
    for i := 0 to Count - 1 do
      with Compounds[I] do
        Result[i] := 1000000.0 * (t / Tc.Value) / (p / Pc.Value);
  end;//with
end;

procedure TFlash.TP(AMaterial: TMaterial);
  
  (*var
    i, j, numphase: Integer;
    Z, Y, X1, X2: CompArray;
    temp, L: Double;
    phi1, phi2: Double;
    FlowV, FlowL1, FlowL2: Double;
    K1, K2: CompArray;
    ZVap, ZLiq1, ZLiq2: Double;
    OldK1, OldK2: CompArray;
    OldY, OldX1, OldX2: CompArray;
    PhaseExist: ExistArray;
    Converged: Boolean;
    iter: Integer;
    Err: Double;*)
  
begin
  (*Converged := False;
  Z := Feed.Comp;
  K1 := EOS.EstK1Values(t, p);
  L := GetTwoPhaseL(K1, Z);
  for i := 1 to NumComp do X1[i] := Z[i] / (L + (1 - L) * K1[i]);
  for i := 1 to NumComp do Y[i] := K1[i] * Z[i] / (L + (1 - L) * K1[i]);
  OldK1 := K1;
  K1 := EOS.GetK(OldK1, Y, X1, ZVap, ZLiq1, t, p);
  temp := 0.0;
  for i := 1 to NumComp do begin
    if (i <> WhichImmisc) then begin
      X2[i] := Z[i] * 0.001;
      temp := temp + X2[i];
    end;
  end;
  X2[WhichImmisc] := 100.0 - temp;
  for i := 1 to NumComp do OldK2[i] := Y[i] / X2[i];
  K2 := EOS.GetK(OldK2, Y, X2, ZVap, Zliq2, t, p);
  iter := 1;
  Err := 10000;
  {temporary debug
  K1[1]:=68.7;
  K1[2]:=0.341;
  K1[3]:=163.0;
  K2[1]:=94200;
  K2[2]:=100000000000.0;
  K2[3]:=0.962;
  {End Debug}
  
  while ((abs(Err) > 0.01) and (Iter < 100)) do begin
    OldY := Y;
    OldX1 := X1;
    OldX2 := X2;
    PhaseExist := DoesPhaseExist(K1, K2, Z, phi1, phi2);
    numphase := 0;
    for i := 1 to 3 do if PhaseExist[i] then numphase := numphase + 1;
    if (numphase = 1) then begin
      if PhaseExist[1] then Feed.Vap.Comp := Z
      else if PhaseExist[2] then Feed.Liq1.Comp := Z
      else Feed.Liq2.Comp := Z;
    end
    else if (numphase = 2) then begin
      FlowL1 := phi1 * Feed.Flow;
      FlowL2 := phi2 * Feed.Flow;
      FlowV := Feed.Flow - FlowL1 - FlowL2;
      for i := 1 to numcomp do begin
        Y[i] := Z[i] * Feed.Flow * K1[i] * K2[i] / (FlowV * K1[i] * K2[i] + FlowL1 * K2[i] + FlowL2 * K1[i]);
        X1[i] := Z[i] * Feed.Flow * K2[i] / (FlowV * K1[i] * K2[i] + FlowL1 * K2[i] + FlowL2 * K1[i]);
        X2[i] := Z[i] * Feed.Flow * K1[i] / (FlowV * K1[i] * K2[i] + FlowL1 * K2[i] + FlowL2 * K1[i]);
      end;
      normalize(Y);
      normalize(X1);
      normalize(X2);
    end
    else {Three Phases present} begin
      Solvephi1phi2(phi1, phi2, K1, K2, Z);
      FlowL1 := phi1 * Feed.Flow;
      FlowL2 := phi2 * Feed.Flow;
      FlowV := Feed.Flow - FlowL1 - FlowL2;
      for i := 1 to numcomp do begin
        Y[i] := Z[i] * Feed.Flow * K1[i] * K2[i] / (FlowV * K1[i] * K2[i] + FlowL1 * K2[i] + FlowL2 * K1[i]);
        X1[i] := Z[i] * Feed.Flow * K2[i] / (FlowV * K1[i] * K2[i] + FlowL1 * K2[i] + FlowL2 * K1[i]);
        X2[i] := Z[i] * Feed.Flow * K1[i] / (FlowV * K1[i] * K2[i] + FlowL1 * K2[i] + FlowL2 * K1[i]);
      end;
      normalize(Y);
      normalize(X1);
      normalize(X2);
    end;
    Err := 0.0;
    OldK1 := K1;
    OldK2 := K2;
    K1 := EOS.GetK(OldK1, Y, X1, ZVap, ZLiq1, t, p);
    K2 := EOS.GetK(OldK2, Y, X2, ZVap, ZLiq2, t, p);
  // for i:=1 to NumComp  do Err:=Err + K1[i]/OldK1[i]-1.0;
  // for i:=1 to NumComp  do Err:=Err + K2[i]/OldK2[i]-1.0;
    for i := 1 to NumComp do Err := Err + abs(Y[i] / OldY[i] - 1.0);
    for i := 1 to NumComp do Err := Err + abs(X1[i] / OldX1[i] - 1.0);
    for i := 1 to NumComp do Err := Err + abs(X2[i] / OldX2[i] - 1.0);
    iter := iter + 1;
  end;
  Feed.t := t;
  Feed.p := p;
  if PhaseExist[1] then {Vapor Phase         } begin
    Feed.Vap.Comp := Y;
    Feed.Vap.Flow := FlowV;
    Feed.Vap.Exist := True;
    Feed.Vap.Z := ZVap;
  end
  else Feed.Vap.clearphase;
  if PhaseExist[2] then {Liquid 1 (HC) Phase } begin
    Feed.Liq1.Comp := X1;
    Feed.Liq1.Flow := FlowL1;
    Feed.Liq1.Exist := True;
    Feed.Liq1.Z := ZLiq1;
  end
  else Feed.Liq1.clearphase;
  if PhaseExist[3] then {Liquid 2 (AQ) Phase } begin
    Feed.Liq2.Comp := X2;
    Feed.Liq2.Flow := FlowL2;
    Feed.Liq2.Exist := True;
    Feed.Liq2.Z := ZLiq2;
  end
  else Feed.Liq2.clearphase;*)
  
end;

{
****************************** TEquilibriumServer ******************************
}
procedure TEquilibriumServer.CalcEquilibrium(AMaterial: TMaterial);
begin
end;

end.
