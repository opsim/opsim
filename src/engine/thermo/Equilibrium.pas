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
  SysUtils, Classes, Variants, Entities, Thermo, Eos;

type
  TThreePhaseFlash = class (TObject)
{*****************************************************************************
 * This Class implements a 3 Phase Flash Alogorithym.  This routine is based *
 * on alogorithms presented in "Rapid Phase Determination in Multiple-Phase  *
 * Flash Calculations" by P. A. Nelson.  Comput. chem. Engng, Vol 11, No 6,  *
 * pp. 581-591,1987.  Some notes for this routine reference equations out of *
 * that paper directly and it is recommended to have a copy available if when*
 * trying to understand it.                                                  *
 *****************************************************************************
}
    function EstK1Values(ACompounds: TCompounds; P, T: Real): Variant;
    function EstK2Values(ACompounds: TCompounds; P, T: Real): Variant;
    procedure TP(AMaterial: TMaterial; T, P: Real);
  private
    FEos: TEos;
    function GetTwoPhaseLiquidFraction (const K: array of Real; const AComposition: TCompositions): Real;
    function TwoPhaseSum(const K: array of Real; const AComposition: TCompositions; LiquidFraction : Real): Real;
    function dTwoPhaseSum(const K: array of Real; const AComposition: TCompositions;LiquidFraction : Real): Real;

  public
    property Eos: TEos read FEos write FEos;
  end;
  
  TEquilibriumServer = class (TObject)
  public
    {
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

procedure TThreePhaseFlash.TP(AMaterial: TMaterial; T, P : Real);
{*****************************************************************************
 * This routine implements an isothermal, isobaric flash referred to as a    *
 * Boston and Britt Type VI Flash.  The overall composition, Temperature and *
 * Pressure are specified returns a amount and composition of each phase     *
 *****************************************************************************
 }
var
{ Some of these variables will be moved directly to the AMaterial but I am leaving them
  separate for the moment so they are scratch variables
  // ZVap, ZLiq1, ZLiq2: Real -> Should be part of TPhase
 }
  K1, PreviousK1 : array of Real;  // Vapor/Liquid equilibrium for component i in Liquid Phase 1 (Hydrocarbon)
  K2, PreviousK2 : array of Real;  // Vapor/Liquid equilibrium for component i in Liquid Phase 2 (Immiscible Phase)
  Phi1,                            // Fraction of Overall AMaterial in Liquid Phase 1
  Phi2:  Real;                     // Fraction of Overall AMaterial in Liquid Phase 2
  TotalLiquidFraction: Real;       // Fraction of the overall AMaterial present as any liquid
  
  iter,i,j;                        // temporary counters.. may be used as needed
  
  {...Scratch Variables... }

  VaporPhase,                      // Vapor Composition (If it exists)
  Liquid1Phase,                    // Liquid1 (Hydrocarbon Liquid) Composition
  Liquid2Phase,                    // Liquid2 (Second Liquid, Usually Water) if it exists
  PreviousTotalPhase,              // Same as above, but from the previous iteration
  PreviousVaporPhase,
  PreviousLiquid1Phase,
  PreviousLiquid2Phase : TPhase;
  Err: Real;                       // Cumulative error
  Converged : Boolean;
  PhaseExist:  array of Boolean;   // Element 1-Vapor, 2-Liq1, 3-Liq2
  
begin

  SetLength(PhaseExist,3);         //This flash routine currently handles a max of three phases
  SetLength(K1, AMaterial.Compounds.Count);
  SetLength(PreviousK1,AMaterial.Compounds.Count);
  SetLength(K2, AMaterial.Compounds.Count);
  SetLength(PreviousK2,AMaterial.Compounds.Count);


  {Create Potential Phases.. remember AMaterial tehnically may not have ANY Phases at this point(first flash)}
  VaporPhase:=TPhase.Create(Self);
  PreviousVaporPhase:=TPhase.Create(Self);
  
  Liquid1Phase:=TPhase.Create(Self);
  PreviousLiquid1Phase:=TPhase.Create(Self);
  
  Liquid2Phase:=TPhase.Create(Self);
  PreviousLiquid2Phase:=TPhase.Create(Self);
  
  VaporPhase.Compositions.Assign(AMaterial.Compositions); // Dont care so much about the actual composition as the structure
  VaporPhase.AggregationState := asVapor;

  Liquid1Phase.Compositions.Assign(AMaterial.Compositions);
  Liquid1Phase.AggregationState := asLiquid;

  Liquid2Phase.Compositions.Assign(AMaterial.Compositions);
  Liquid2Phase.AggregationState := asLiquid;


  Converged := False;  // Otherwise why would we be running this :-) .. makes sure things run at least once

  K1 := EstK1Values(AMaterial.Compositions, T, P);  // Come up with an initial Estimate
  
  TotalLiquidFraction := GetTwoPhaseL(K1, AMaterial.Compositions);  // Generate an inital Liquid Fraction
  
  // From Liquid Fraction and K values, generate Mole Fractions for the two phase situation
  
  for i := 0 to AMaterial.Compounds.Count - 1  do
      Liquid1Phase.Compositions[i].MoleFraction.Value := AMaterial.Compositions[i].MoleFraction.Value /
                                                         (LiquidFraction + (1 - LiquidFraction) * K1[i]);
  for i := 0 to AMaterial.Compounds.Count - 1  do
      VaporPhase.Compositions[i].MoleFraction.Value := K1[i] * AMaterial.Compositions[i].MoleFraction.Value /
                                                         (LiquidFraction + (1 - LiquidFraction) * K1[i]);

  OldK1 := K1;
  
{Okay.. made it to here}
  
  K1 := EOS.GetK(OldK1, Y, X1, ZVap, ZLiq1, t, p);
  
 {Need to determine if this little section will be robust enough to use}
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

function TThreePhaseFlash.EstK1Values(ACompounds: TCompounds; P, T: Real): Variant;
{ Estimates K values for a vapor phase and a Hydrocarbon Liquid Phase.
  According to the paper I found it in, is based on the Wilson Equation but I
  have not been able to verify.  This should be used if previous set of K values
  is not available.
}
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

function TThreePhaseFlash.EstK2Values(ACompounds: TCompounds; P, T: Real): Variant;
{ Estimates K values for a vapor phase and a Second Liquid Phase.
  According to the paper I found it in, is based on the Wilson Equation but I
  have not been able to verify.  This should be used if previous set of K values
  is not available. Currently not used in this Flash routine
}
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

function TThreePhaseFlash.GetTwoPhaseLiquidFraction (const K: array of Real; const AComposition: TCompositions): Real;
var
  Err: Real;
  LiquidFraction, OldLiquidFraction: Real;
  iter: Integer;
  F, dFdL: Double;  // Function Value and Derivative  (could be eliminated but may be necessary for debug)

  {Assuming two phases (Vapor/Liquid) and given feed composition
   and K's, solve for the Liquid Fraction using using Newton-Raphson }
  {Function and Derivative return values}

begin
  LiquidFraction:=0.5;
  Err:=10000.0;
  iter:=0;
  While ((Abs(Err) > 0.00005) and (iter<100)) do
   begin
    OldLiquidFraction := LiquidFraction;
    F:= TwoPhaseSum(K, AComposition, OldLiquidFraction);
    dFdL := dTwoPhaseSum(K, AComposition, OldLiquidFraction);
    LiquidFraction := OldLiquidFraction - F/dFdL;
    Err := L/OldL -1;
    iter:=iter+1;
   end;
  Result := LiquidFraction;
end;

function TThreePhaseFlash.TwoPhaseSum(const K: array of Real; const AComposition: TCompositions;
                                      LiquidFraction : Real): Real;

{Still thinking about the exact way to phrase this}
var
  i : Integer;
  Total : Double;


begin
  Total := 0.0;
  for i := 0 to AComposition.Count - 1  do
    Total:=Total + (1.0-K[i])*AComposition[I].MoleFraction.Value /(LiquidFraction+(1-LiquidFraction)*K[i]);
  Result := Total;

end;

function TThreePhaseFlash.dTwoPhaseSum(const K: array of Real; const AComposition: TCompositions;
                                       LiquidFraction : Real): Real;
var
  i : Integer;
  Total : Double;

  {Still thinking about these two.. two phase flash equations for solution with Newton-Raphson}

begin
  Total := 0.0;
  for i := 0 to AComposition.Count - 1  do
    Total := Total - (Sqr(1.0-K[i])*AComposition[I].MoleFraction.Value/Sqr((LiquidFraction +(1-LiquidFraction)*K[i])));
  Result := Total;
end;

{
****************************** TEquilibriumServer ******************************
}
procedure TEquilibriumServer.CalcEquilibrium(AMaterial: TMaterial);
begin
end;

end.
