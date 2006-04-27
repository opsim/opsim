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
    function CalcQ1(const K1,K2: array of Real; const AComposition:TCompositions; Phi1,Phi2:Real): Real;
    function CalcQ2(const K1,K2: array of Real; const ACompositon: TCompositions; phi1,phi2:Real): Real;
    function CalcdQ1dphi1(const K1,K2: array of Real; const ACompositon: TCompositions; phi1,phi2:Real): Real;
    function CalcdQ1dphi2(const K1,K2: array of Real; const ACompositon: TCompositions; phi1,phi2:Real): Real;
    function CalcdQ2dphi2(const K1,K2: array of Real; const ACompositon: TCompositions; phi1,phi2:Real): Real;
    function SolveQ1(const K1,K2: array of Real; const ACompositon: TCompositions; Phi2: Real): Real;
    function SolveQ2(const K1,K2: array of Real; const ACompositon: TCompositions; Phi1: Real): Real;
    function SolveQ1minusQ2(const K1,K2: array of Real; const ACompositon: TCompositions): Real;
    

    procedure Normalize(var AComposition: TCompositions);
    procedure UpdateKValues(var VaporPhase, LiquidPhase: TPhase; var K:array of Real);
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
const
  MaxPhases = 3;                  // This Flash solver is currently designed for a Maximum of 3 possible number of Phases
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
  LiquidFraction: Real;
  
  iter,i,j: Integer;                        // temporary counters.. may be used as needed
  
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
  OldK1: array of Real;
  
begin
  //This flash routine currently handles a max of three phases
  SetLength(PhaseExist,MaxPhases);
  SetLength(K1, AMaterial.Compounds.Count);
  SetLength(PreviousK1,AMaterial.Compounds.Count);
  SetLength(K2, AMaterial.Compounds.Count);
  SetLength(PreviousK2,AMaterial.Compounds.Count);


  {Create Potential Phases.. remember AMaterial tehnically may not have ANY Phases at this point(first flash)}
  VaporPhase:=TPhase.Create(nil);
  PreviousVaporPhase:=TPhase.Create(nil);
  
  Liquid1Phase:=TPhase.Create(nil);
  PreviousLiquid1Phase:=TPhase.Create(nil);
  
  Liquid2Phase:=TPhase.Create(nil);
  PreviousLiquid2Phase:=TPhase.Create(nil);
  
  {Dont care so much about the actual composition as the structure}
  VaporPhase.Compositions.Assign(AMaterial.Compositions);
  VaporPhase.AggregationState := asVapor;
  VaporPhase.Temperature.Value := T;
  VaporPhase.Temperature.Value := P;

  Liquid1Phase.Compositions.Assign(AMaterial.Compositions);
  Liquid1Phase.AggregationState := asLiquid;
  Liquid1Phase.Temperature.Value := T;
  Liquid1Phase.Temperature.Value := P;

  Liquid2Phase.Compositions.Assign(AMaterial.Compositions);
  Liquid2Phase.AggregationState := asLiquid;
  Liquid2Phase.Temperature.Value := T;
  Liquid2Phase.Temperature.Value := P;


  Converged := False;
  
  {Come up with an initial Estimate}
  K1 := EstK1Values(AMaterial.Compounds, T, P);
  
  // Generate an inital Liquid Fraction
  TotalLiquidFraction := GetTwoPhaseLiquidFraction(K1, AMaterial.Compositions);
  
  // From Liquid Fraction and K values, generate Mole Fractions for the two phase situation
  for i := 0 to AMaterial.Compounds.Count - 1  do
      Liquid1Phase.Compositions[i].MoleFraction.Value := AMaterial.Compositions[i].MoleFraction.Value /
                                                         (LiquidFraction + (1 - LiquidFraction) * K1[i]);
  for i := 0 to AMaterial.Compounds.Count - 1  do
      VaporPhase.Compositions[i].MoleFraction.Value := K1[i] * AMaterial.Compositions[i].MoleFraction.Value /
                                                         (LiquidFraction + (1 - LiquidFraction) * K1[i]);

  OldK1 := K1;
  
   {Calculate New K Values}
  
  UpdateKValues(VaporPhase, Liquid1Phase, K1);

 { Create a preliminary guess for liquid phase 2 by taking the predominate Liquid 2 Component (usually water)
   and putting it in a phase.  Add a tiny bit of the other components }
 {Need to determine if this little section will be robust enough to use}
  With aMaterial do
  begin
   temp := 0.0;
   for i := 0 to Compounds.Count - 1  do begin
     if (Compounds[i].CompID <> Compounds.ImmiscibleComponent) then
        begin
          Liquid2Phase.Compositions[i].MoleFraction.Value := Compositions[i].MoleFraction.Value * 0.001;
          temp := temp + Liquid2Phase.Compositions[i].MoleFraction.Value;
        end;
      else ImmiscibleIndex := i;
    
   end;
   Liquid2Phase.Compositions[ImmiscibleIndex].MoleFraction.Value := 100.0 - temp;
  end;
  
  for i := 0 to AMaterial.Compounds.Count - 1  do
  K2[i] := VaporPhase.Compositions[i].MoleFraction.Value / Liquid2Phase.Compositions[i].MoleFraction.Value;

  UpdateKValues(VaporPhase, Liquid2Phase, K2);
  
  iter := 1;
  Err := 10000;

  while ((abs(Err) > 0.01) and (Iter < 100)) do begin
    OldVaporPhase := VaporPhase;
    OldLiquid1Phase := Liquid1Phase;
    OldLiquid2Phase := Liquid2Phase;

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
    Total := Total - (Sqr(1.0-K[i])*AComposition[i].MoleFraction.Value/Sqr((LiquidFraction +(1-LiquidFraction)*K[i])));
  Result := Total;
end;

procedure ThreePhaseFlash.UpdateKValues(var VaporPhase, LiquidPhase: TPhase; var K:array of Real);

        
{Updates K Values for a pair of Phases.  Will Fail if K's are 0.0.  Also updates the Compressibiliteis
and Fugasity Coefficients}

var
  i: Integer;

begin
  FEOS.Solve(VaporPhase);
  FEOS.Solve(LiquidPhase);
  for i := 0 to VaporPhase.Compounds.Count -1 do
    K[i] := LiquidPhase.Compositions[i].FugacityCoefficient.Value /
            VaporPhase.Compositions[i].FugacityCoefficient.Value * K[i];
end;

procedure ThreePhaseFlash.normalize(var AComposition: TCompositions);
// Make sure a set of mole fractions add up to 1
var
i : integer;
total: real;
begin
total:=0.0;
for i:= 0 to AComposition.Count - 1  do
    total := total + AComposition.Items[i].MoleFraction.Value;
total := 1.0 / total;
for i:= 0 to AComposition.Count - 1 do
    AComposition.Items[i].MoleFraction.Value := Total * AComposition.Items[i].MoleFraction.Value;
end;

function TThreePhaseFlash.CalcQ1(const K1,K2: array of Real; const AComposition:TCompositions; Phi1,Phi2:Real): Real;
var
  i: Integer;
  Q1: Real;


  {An implemenation of EQ 7a.  Partial derivative functions are below.

  Essentially this is a mass balance inserted into the component balance and then
  played around with to produce a form that solves better under Newton-Raphson
  No variables in the call list are changed.. only a number is returned}

  {EQ 7a}

begin

  Q1:=0.0;
  for i := 0 to AComposition.Count - 1  do
    begin
      Q1:= Q1  + AComposition[i].MoleFraction.Value*K2[i]*(1.0-K1[i])/
                 (K1[i]*K2[i]+Phi1*K2[i]*(1-K1[i])+Phi2*K1[i]*(1-K2[i]));
    end;
  Result:= Q1;
end;

function TThreePhaseFlash.CalcQ2(const K1,K2: array of Real; const ACompositon: TCompositions; phi1,phi2:Real): Real;
var
  i: Integer;
  Q2: Real;

  {An implemenation of EQ 7b.  Partial derivative functions are below.

  Essentially this is a mass balance inserted into the component balance and then
  played around with to produce a form that solves better under Newton-Raphson
  No variables in the call list are changed.. only a number is returned}
  {EQ 7b}

begin
  Q2:=0.0;
  for i := 0 to AComposition.Count - 1  do
    begin
      Q2:= Q2  + AComposition[i].MoleFraction.Value*K1[i]*(1.0-K2[i])/
                 (K1[i]*K2[i]+phi1*K2[i]*(1-K1[i])+phi2*K1[i]*(1-K2[i]));
    end;
  Result:= Q2;
end;

function TThreePhaseFlash.CalcdQ1dphi1(const K1,K2: array of Real; const ACompositon: TCompositions; phi1,phi2:Real): Real;
var
  i: Integer;
  dQ: Double;

  {Partial derivative function of Q1 with respect to phi1 are below. Defined in EQ 10a

  Needed for the Newton-Raphson solutions for both 2 and 3 phase cases}

begin
  dQ:=0.0;
  for i := 0 to AComposition.Count - 1  do
    begin
      dQ:= dQ  - AComposition[i].MoleFraction.Value*Sqr(K2[i])*Sqr((1.0-K1[i]))/
           Sqr(K1[i]*K2[i]+phi1*K2[i]*(1-K1[i])+phi2*K1[i]*(1-K2[i]));
    end;
  Result:= dQ;
end;

function TThreePhaseFlash.CalcdQ1dphi2(const K1,K2: array of Real; const ACompositon: TCompositions; phi1,phi2:Real): Real;
var
  i: Integer;
  dQ: Double;

  {Partial derivative function of Q1 with respect to phi2 are below. Defined in EQ 10b
  Same as dQ2/dphi1
  Needed for the Newton-Raphson solutions for both 2 and 3 phase cases}

begin
  dQ:=0.0;
  for i := 0 to AComposition.Count - 1  do
    begin
      dQ:= dQ  - AComposition[i].MoleFraction.Value*K1[i]*K2[i]*(1.0-K1[i])*(1.0-K2[i])/
           Sqr(K1[i]*K2[i]+phi1*K2[i]*(1-K1[i])+phi2*K1[i]*(1-K2[i]));
    end;
  GetdQ1dphi2:=dQ;
end;

function TThreePhaseFlash.CalcdQ2dphi2(const K1,K2: array of Real; const ACompositon: TCompositions; phi1,phi2:Real): Real;
var
  i: Integer;
  dQ: Double;

  {Partial derivative function of Q2 with respect to phi2 are below. Defined in EQ 10c

  Needed for the Newton-Raphson solutions for both 2 and 3 phase cases}

begin
  dQ:=0.0;
  for i := 0 to AComposition.Count - 1  do
    begin
      dQ:= dQ  - AComposition[i].MoleFraction.Value*Sqr(K1[i])*Sqr((1.0-K2[i]))/
           Sqr(K1[i]*K2[i]+phi1*K2[i]*(1-K1[i])+phi2*K1[i]*(1-K2[i]));
    end;
  GetdQ2dphi2:=dQ;
end;

function TThreePhaseFlash.SolveQ1(const K1,K2: array of Real; const ACompositon: TCompositions; Phi2:Real): Real;
var
  Err: Real;
  L, OldL: Real;
  iter: Integer;
  F, dFdL: Real;

  {Given a set of K values for both Liquid Phases and the Feed mol Fractions of the Feed
  Solve Eq 7a for phi1 at fixed phi2 using Newton-Raphson
  -- so despite its name, it returns a Liquid fraction of Liquid phase 1 at a given
  liquid phase 2 fraction

  }
  {Function and derivative of Function, in this case Q1()}

begin
  L:=0.5;
  Err:=10000.0;
  iter:=0;           {Limit the number of iterations}
  While ((Abs(Err) > 0.00005) and (iter<100)) do
   begin
    OldL:= L;
    F:= CalcQ1(K1,K2,aComposition,L,Phi2);
    dFdL := CalcdQ1dphi1(K1,K2,aComposition,L,Phi2);
    L := OldL - F/dFdL;
    Err := L/OldL -1;
    iter:=iter+1;
   end;
  Result:=L;
end;

function TThreePhaseFlash.SolveQ1minusQ2(const K1,K2: array of Real; const ACompositon: TCompositions): Real;
var
  Err: Double;
  L, OldL: Double;
  iter: Integer;
  F, dFdL: Double;

  {This is the third boundary condition.  Shown graphically as the diagonal line
  on Fig 2 of Nelson.  Here phi2=1-phi1 .  We are solving for the root of Q1(phi1,1-phi1)-
  Q2(phi1,1-phi1)=0 which returns phi1 along the diagonal edge}

begin
  L:=0.5;
  Err:=10000.0;
  iter:=0;
  While ((Abs(Err) > 0.00005) and (iter<100)) do
   begin
    OldL:= L;
    F:= CalcQ1(K1,K2,aComposition,L,1.0-L)-CalcQ2(K1,K2,aComposition,L,1.0-L);
    dFdL := CalcdQ1dphi1(K1,K2,aComposition,L,1.0-L)-CalcdQ1dphi2(K1,K2,aComposition,L,1.0-L);
    L := OldL - F/dFdL;
    Err := L/OldL -1;
    iter:=iter+1;
   end;
  Result:=L;
end;

function TThreePhaseFlash.SolveQ2(const K1,K2: array of Real; const ACompositon: TCompositions; Phi1: Real): Real;
var
  Err: Double;
  L, OldL: Double;
  iter: Integer;
  F, dFdL: Double;

  {Given a set of K values for both Liquid Phases and the Feed mol Fractions of the Feed
  Solve Eq 7b for phi1 at fixed phi1 using Newton-Raphson
  -- so despite its name, it returns a Liquid fraction of Liquid phase 2 at a given
  liquid phase 1 fraction

  }

begin
  L:=0.5;
  Err:=10000.0;
  iter:=0;
  While ((Abs(Err) > 0.00005) and (iter<100)) do
   begin
    OldL:= L;
    F:= CalcQ2(K1,K2,aComposition,Phi1,L);
    dFdL := CalcdQ2dphi2(K1,K2,aComposition,Phi1,L);
    L := OldL - F/dFdL;
    Err := L/OldL -1;
    iter:=iter+1;
   end;
  Result:=L;
end;

{
****************************** TEquilibriumServer ******************************
}
procedure TEquilibriumServer.CalcEquilibrium(AMaterial: TMaterial);
begin
end;

end.
