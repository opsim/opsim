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
{*****************************************************************************
 * This Class implements a 3 Phase Flash Alogorithym.  This routine is based *
 * on alogorithms presented in "Rapid Phase Determination in Multiple-Phase  *
 * Flash Calculations" by P. A. Nelson.  Comput. chem. Engng, Vol 11, No 6,  *
 * pp. 581-591,1987.  Some notes for this routine reference equations out of *
 * that paper directly and it is recommended to have a copy available if when*
 * trying to understand it.                                                  *
 *****************************************************************************}
  TThreePhaseFlash = class (TObject)
    function EstK1Values(ACompounds: TCompounds; P, T: Real): Variant;
    function EstK2Values(ACompounds: TCompounds; P, T: Real): Variant;
    procedure TP(AMaterial: TMaterial; T, P: Real);
  private
    FEos: TEos;
    function CalcdQ1dphi1(const K1,K2: array of Real; const AComposition: 
            TCompositions; phi1,phi2:Real): Real;
    function CalcdQ1dphi2(const K1,K2: array of Real; const AComposition: 
            TCompositions; phi1,phi2:Real): Real;
    function CalcdQ2dphi2(const K1,K2: array of Real; const AComposition: 
            TCompositions; phi1,phi2:Real): Real;
    function CalcQ1(const K1,K2: array of Real; const 
            AComposition:TCompositions; Phi1,Phi2:Real): Real;
    function CalcQ2(const K1,K2: array of Real; const AComposition: 
            TCompositions; phi1,phi2:Real): Real;
    procedure DoesPhaseExist(const K1,K2: array of Real; const AComposition: 
            TCompositions; var PhaseExist: array of Boolean; var phi1,phi2: 
            Real);
    function dTwoPhaseSum(const K: array of Real; const AComposition: 
            TCompositions;LiquidFraction : Real): Real;
    function GetTwoPhaseLiquidFraction(const K: array of Real; const 
            AComposition: TCompositions): Real;
    procedure Normalize(var AComposition: TCompositions);
    procedure Solvephi1phi2(const K1,K2: array of Real; const AComposition: 
            TCompositions; var Phi1,Phi2: Real);
    function SolveQ1(const K1,K2: array of Real; const AComposition: 
            TCompositions; Phi2: Real): Real;
    function SolveQ1minusQ2(const K1,K2: array of Real; const AComposition: 
            TCompositions): Real;
    function SolveQ2(const K1,K2: array of Real; const AComposition: 
            TCompositions; Phi1: Real): Real;
    function TwoPhaseSum(const K: array of Real; const AComposition: 
            TCompositions; LiquidFraction : Real): Real;
    procedure UpdateKValues(var VaporPhase, LiquidPhase: TPhase; var K:array of 
            Real);
  public
    property Eos: TEos read FEos write FEos;
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
******************************* TThreePhaseFlash *******************************
}
function TThreePhaseFlash.CalcdQ1dphi1(const K1,K2: array of Real; const 
        AComposition: TCompositions; phi1,phi2:Real): Real;
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

function TThreePhaseFlash.CalcdQ1dphi2(const K1,K2: array of Real; const 
        AComposition: TCompositions; phi1,phi2:Real): Real;
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
  Result:=dQ;
end;

function TThreePhaseFlash.CalcdQ2dphi2(const K1,K2: array of Real; const 
        AComposition: TCompositions; phi1,phi2:Real): Real;
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
  Result:=dQ;
end;

function TThreePhaseFlash.CalcQ1(const K1,K2: array of Real; const 
        AComposition:TCompositions; Phi1,Phi2:Real): Real;
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

function TThreePhaseFlash.CalcQ2(const K1,K2: array of Real; const 
        AComposition: TCompositions; phi1,phi2:Real): Real;
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

procedure TThreePhaseFlash.DoesPhaseExist(const K1,K2: array of Real; const 
        AComposition: TCompositions; var PhaseExist: array of Boolean; var phi1,
        phi2: Real);
var
  i: Integer;
  T1, T2, T3, T4, T5, T6, T7, T8, T9: Real;
  Otherphi1: Real;
  
  {Given K1,K2 and Z, this function detemines the existence of the different phases
   based on rules on page 585 of nelson paper.  It returns an array showing
   the existing phases, it also returns phi values for phases that exists, are subsequently used}
  
begin
  // Initialize Phase Existence array to no phases
  PhaseExist[1]:=False;
  PhaseExist[2]:=False;
  PhaseExist[3]:=False;
  // Test Variables
  T1:=0.0;
  T2:=0.0;
  T3:=0.0;
  T4:=0.0;
  T5:=0.0;
  T6:=0.0;
  
  for i := 0 to AComposition.Count - 1  do
  begin
    // Equation 9a from Nelson
    T1:=T1+AComposition[i].MoleFraction.Value/K1[i];
    // Equation 9b from Nelson
    T2:=T2+AComposition[i].MoleFraction.Value*K1[i];
    // Equation 9c from Nelson
    T3:=T3+AComposition[i].MoleFraction.Value/K2[i];
    // Equation 9d from Nelson
    T4:=T4+AComposition[i].MoleFraction.Value*K2[i];
    // Equation 9e from Nelson
    T5:=T5+AComposition[i].MoleFraction.Value*K1[i]/K2[i];
    // Equation 9f from Nelson
    T6:=T6+AComposition[i].MoleFraction.Value*K2[i]/K1[i];
  end;
  { First Check for only Single Phases, Do the easy one first         }
  if  ( (T1<1.0) and (T3<1.0) ) then
      PhaseExist[1] := True
  else if ( (T2<1.0) and (T5<1.0) ) then
      PhaseExist[2] := True
  else if ( (T4<1.0) and (T6<1.0)) then
      PhaseExist[3] := True
  else
  {Check Two Phase Scenarios, See Fig 3 in paper for graphical explanation }
      begin
        {Check boundary conditions}
        {Find Q2(phi1,0) where phi1 is the root of Q1(phi1,0)=0}
        phi1 := SolveQ1(K1,K2,AComposition,0.0);
        T7 := CalcQ2(K1,K2,AComposition,phi1,0.0);
        {Find Q1(0,phi2) where phi2 is the root of Q2(0,phi2)=0)}
        phi2 := SolveQ2(K1,K2,AComposition,0.0);
        T8 := CalcQ1(K1,K2,AComposition,0.0,phi2);
        {Find Q1(phi1,1-phi1) at the root of Q1(phi1,1-phi1)-Q2(phi1,1-phi1)=0}
        OtherPhi1:=SolveQ1minusQ2(K1,K2,AComposition);
        T9 := CalcQ1(K1,K2,AComposition,Otherphi1,1.0-Otherphi1);
        if ( (T7 < 0.0) and (T1>1.0) and (T2>1.0) ) then
          begin                            { Vap and Liq1 Only        }
            PhaseExist[1]:=True;
            PhaseExist[2]:=True;
            phi2:=0.0
          end
        else if ( (T8 < 0.0) and (T1>1.0) and (T2>1.0) ) then
          begin                            { Vap and Liq2 Only        }
            PhaseExist[1]:=True;
            PhaseExist[3]:=True;
            phi1:=0.0
          end
        else if ( (T9 > 0.0) and (T5>1.0) and (T6>1.0) ) then
          begin                            { Liq1 and Liq2 Only       }
            PhaseExist[2]:=True;
            PhaseExist[3]:=True;
          end
        else
          begin
            PhaseExist[1]:=True;                { Three Phase System       }
            PhaseExist[2]:=True;
            PhaseExist[3]:=True;
          end;
      end;
  
end;

function TThreePhaseFlash.dTwoPhaseSum(const K: array of Real; const 
        AComposition: TCompositions;LiquidFraction : Real): Real;
var
  i: Integer;
  Total: Double;
  
  {Still thinking about these two.. two phase flash equations for solution with Newton-Raphson}
  
begin
  Total := 0.0;
  for i := 0 to AComposition.Count - 1  do
    Total := Total - (Sqr(1.0-K[i])*AComposition[i].MoleFraction.Value/Sqr((LiquidFraction +(1-LiquidFraction)*K[i])));
  Result := Total;
end;

function TThreePhaseFlash.EstK1Values(ACompounds: TCompounds; P, T: Real): 
        Variant;
var
  i: Integer;
  
  { Estimates K values for a vapor phase and a Hydrocarbon Liquid Phase.
    According to the paper I found it in, is based on the Wilson Equation but I
    have not been able to verify.  This should be used if previous set of K values
    is not available.
  }
  
begin
  with ACompounds do begin
    //Makes room for the result according to the number of compounds
    Result := VarArrayCreate([0, Count - 1], varDouble);
    for i := 0 to Count - 1 do
      with Compounds[I] do
        Result[i] := exp(5.3727 * (1 + W.Value) * (1 - 1 / (t / Tc.Value))) / (p / Pc.Value);
  end;//with
end;

function TThreePhaseFlash.EstK2Values(ACompounds: TCompounds; P, T: Real): 
        Variant;
var
  i: Integer;
  
  { Estimates K values for a vapor phase and a Second Liquid Phase.
    According to the paper I found it in, is based on the Wilson Equation but I
    have not been able to verify.  This should be used if previous set of K values
    is not available. Currently not used in this Flash routine
  }
  
begin
  with ACompounds do begin
    //Makes room for the result according to the number of compounds
    Result := VarArrayCreate([0, Count - 1], varDouble);
    for i := 0 to Count - 1 do
      with Compounds[I] do
        Result[i] := 1000000.0 * (t / Tc.Value) / (p / Pc.Value);
  end;//with
end;

function TThreePhaseFlash.GetTwoPhaseLiquidFraction(const K: array of Real; 
        const AComposition: TCompositions): Real;
var
  Err: Real;
  LiquidFraction, OldLiquidFraction: Real;
  iter: Integer;
  F, dFdL: Double;
  
  // Function Value and Derivative  (could be eliminated but may be necessary for debug)
  
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
    Err := LiquidFraction/OldLiquidFraction -1;
    iter:=iter+1;
   end;
  Result := LiquidFraction;
end;

procedure TThreePhaseFlash.Normalize(var AComposition: TCompositions);
var
  i: Integer;
  total: Real;
  
  // Make sure a set of mole fractions add up to 1
  
begin
  total:=0.0;
  for i:= 0 to AComposition.Count - 1  do
      total := total + AComposition[i].MoleFraction.Value;
  total := 1.0 / total;
  for i:= 0 to AComposition.Count - 1 do
      AComposition.Items[i].MoleFraction.Value := Total * AComposition.Items[i].MoleFraction.Value;
end;

procedure TThreePhaseFlash.Solvephi1phi2(const K1,K2: array of Real; const 
        AComposition: TCompositions; var Phi1,Phi2: Real);
var
  i: Integer;
  Q1, Q2: Real;
  Oldphi1, Oldphi2: Real;
  D, M11, M12, M21, M22: Real;
  Converged: Boolean;
  
  { Solve for phi1 and phi2 using Newton-Raphson method.  Eq 11 in Nelson Paper
    This is the solution method when we have determined that three phases exist.
    It returns the liquid fractions of each of the two liquid phases}
  
begin
  Converged:=False;
  {Dont start with 0.0}
  If (Phi1=0.0) then Phi1:=0.5;
  If (Phi2=0.0) then Phi2:=0.5;
  Q1:= CalcQ1(K1,K2,AComposition,phi1,phi2);
  Q2:= CalcQ2(K1,K2,AComposition,phi1,phi2);
  While (not Converged) do
  begin
    {Need previous run for derivative}
    Oldphi1:=phi1;
    Oldphi2:=phi2;
    {Get partial derivative of Q2 with repect to phi2}
    M11:= CalcdQ2dphi2(K1,K2,AComposition,Oldphi1,Oldphi2);
    {Take the negative of the partial derivative of Q1 with respect to phi2 }
    M12:= -1.0 * CalcdQ1dphi2(K1,K2,AComposition,Oldphi1,Oldphi2);
    {As per EQ 10b is equivalent to M12}
    M21:= M12;
    { Get partial derivative of Q1 with respect to phi1}
    M22:= CalcdQ1dphi1(K1,K2,AComposition,Oldphi1,Oldphi2);
    {Calculate the Determinent.  Negatives Cancel on M12*M21}
    D := M22*M11 - M12*M21;
    {Calculate the next iternation of phi1}
    phi1:=Oldphi1-(1.0/D)*(M11*Q1+M12*Q2);
    {Calculate the next iteration of phi2}
    phi2:=Oldphi2-(1.0/D)*(M21*Q1+M22*Q2);
    Q1:= CalcQ1(K1,K2,AComposition,phi1,phi2);
    Q2:= CalcQ2(K1,K2,AComposition,phi1,phi2);
    // Repeat until Q1 and Q2 are 0.0
    If ((Abs(Q1)<0.001) and (Abs(Q2)<0.001)) then Converged := True;
  end;
  
  
end;

function TThreePhaseFlash.SolveQ1(const K1,K2: array of Real; const 
        AComposition: TCompositions; Phi2: Real): Real;
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

function TThreePhaseFlash.SolveQ1minusQ2(const K1,K2: array of Real; const 
        AComposition: TCompositions): Real;
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

function TThreePhaseFlash.SolveQ2(const K1,K2: array of Real; const 
        AComposition: TCompositions; Phi1: Real): Real;
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

procedure TThreePhaseFlash.TP(AMaterial: TMaterial; T, P: Real);
  
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
  
    iter,i,j,ImmiscibleIndex: Integer;                        // temporary counters.. may be used as needed
    temp : Real;
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
    NumPhase: Integer;               // Current Number of Phases Predicted
    OldK1: array of Real;
  
begin
   //This flash routine currently handles a max of three phases
  SetLength(PhaseExist,MaxPhases);
  SetLength(K1, AMaterial.Compounds.Count);
  SetLength(PreviousK1,AMaterial.Compounds.Count);
  SetLength(K2, AMaterial.Compounds.Count);
  SetLength(PreviousK2,AMaterial.Compounds.Count);
  
  AMaterial.Temperature.value :=T;
  AMaterial.Pressure.Value:= P;
  
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
  VaporPhase.Pressure.Value := P;
  
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
                                                         (TotalLiquidFraction + (1 - TotalLiquidFraction) * K1[i]);
  for i := 0 to AMaterial.Compounds.Count - 1  do
      VaporPhase.Compositions[i].MoleFraction.Value := K1[i] * AMaterial.Compositions[i].MoleFraction.Value /
                                                         (TotalLiquidFraction + (1 - TotalLiquidFraction) * K1[i]);
  
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
        end
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
    PreviousVaporPhase := VaporPhase;
    PreviousLiquid1Phase := Liquid1Phase;
    PreviousLiquid2Phase := Liquid2Phase;
  
    DoesPhaseExist(K1,K2,AMaterial.Compositions,PhaseExist,phi1,phi2);
  
    NumPhase := 0;
    for i := 1 to MaxPhases do if PhaseExist[i] then numphase := numphase + 1;
    if (NumPhase = 1) then begin
      if PhaseExist[1] then VaporPhase.Compositions:= AMaterial.Compositions
      else if PhaseExist[2] then Liquid1Phase.Compositions:= AMaterial.Compositions
      else Liquid2Phase.Compositions := AMaterial.Compositions;
    end
    else if (numphase = 2) then begin
      Liquid1Phase.MoleFlow.Value:= Phi1 * AMaterial.MoleFlow.Value;
      Liquid2Phase.MoleFlow.Value:= Phi2 * AMaterial.MoleFlow.Value;
      VaporPhase.MoleFlow.Value:= AMaterial.MoleFlow.Value-Liquid1Phase.MoleFlow.Value-Liquid2Phase.MoleFlow.Value;
      for i := 0 to AMaterial.Compounds.Count - 1  do
       begin
        VaporPhase.Compositions[i].MoleFraction.Value:=AMaterial.Compositions[i].MoleFraction.Value*AMaterial.MoleFlow.Value*
                     K1[i]*K2[i] / (VaporPhase.MoleFlow.Value*K1[i]*K2[i]+Liquid1Phase.MoleFlow.Value*K2[i]+
                     Liquid2Phase.MoleFlow.Value*K1[i]);
  
        Liquid1Phase.Compositions[i].MoleFraction.Value:=AMaterial.Compositions[i].MoleFraction.Value*AMaterial.MoleFlow.Value*
                     K2[i] / (VaporPhase.MoleFlow.Value*K1[i]*K2[i]+Liquid1Phase.MoleFlow.Value*K2[i]+
                     Liquid2Phase.MoleFlow.Value*K1[i]);
  
        Liquid2Phase.Compositions[i].MoleFraction.Value:=AMaterial.Compositions[i].MoleFraction.Value*AMaterial.MoleFlow.Value*
                     K1[i] / (VaporPhase.MoleFlow.Value*K1[i]*K2[i]+Liquid1Phase.MoleFlow.Value*K2[i]+
                     Liquid2Phase.MoleFlow.Value*K1[i]);
  
       end;
      Normalize(VaporPhase.Compositions);
      Normalize(Liquid1Phase.Compositions);
      Normalize(Liquid2Phase.Compositions);
    end
  
    else begin
      Solvephi1phi2(K1,K2,AMaterial.Compositions,Phi1,Phi2);
      Liquid1Phase.MoleFlow.Value:= Phi1 * AMaterial.MoleFlow.Value;
      Liquid2Phase.MoleFlow.Value:= Phi2 * AMaterial.MoleFlow.Value;
      VaporPhase.MoleFlow.Value := AMaterial.MoleFlow.Value - Liquid1Phase.MoleFlow.Value - Liquid2Phase.MoleFlow.Value;
      for i := 0 to AMaterial.Compounds.Count - 1  do
       begin
        VaporPhase.Compositions[i].MoleFraction.Value:=AMaterial.Compositions[i].MoleFraction.Value*AMaterial.MoleFlow.Value*
                     K1[i]*K2[i] / (VaporPhase.MoleFlow.Value*K1[i]*K2[i]+Liquid1Phase.MoleFlow.Value*K2[i]+
                     Liquid2Phase.MoleFlow.Value*K1[i]);
  
        Liquid1Phase.Compositions[i].MoleFraction.Value:=AMaterial.Compositions[i].MoleFraction.Value*AMaterial.MoleFlow.Value*
                     K2[i] / (VaporPhase.MoleFlow.Value*K1[i]*K2[i]+Liquid1Phase.MoleFlow.Value*K2[i]+
                     Liquid2Phase.MoleFlow.Value*K1[i]);
  
        Liquid2Phase.Compositions[i].MoleFraction.Value:=AMaterial.Compositions[i].MoleFraction.Value*AMaterial.MoleFlow.Value*
                     K1[i] / (VaporPhase.MoleFlow.Value*K1[i]*K2[i]+Liquid1Phase.MoleFlow.Value*K2[i]+
                     Liquid2Phase.MoleFlow.Value*K1[i]);
  
       end;
      Normalize(VaporPhase.Compositions);
      Normalize(Liquid1Phase.Compositions);
      Normalize(Liquid2Phase.Compositions);
    end;
    Err := 0.0;
    UpdateKValues(VaporPhase, Liquid1Phase, K1);
    UpdateKValues(VaporPhase, Liquid2Phase, K2);
    for i := 0 to AMaterial.Compounds.Count - 1  do
    begin
      Err := Err + abs(Real(VaporPhase.Compositions[i].MoleFraction.Value / PreviousVaporPhase.Compositions[i].MoleFraction.Value - 1.0));
      Err := Err + abs(Real(Liquid1Phase.Compositions[i].MoleFraction.Value / PreviousLiquid1Phase.Compositions[i].MoleFraction.Value - 1.0));
      Err := Err + abs(Real(Liquid2Phase.Compositions[i].MoleFraction.Value / PreviousLiquid2Phase.Compositions[i].MoleFraction.Value - 1.0));
    end;
    iter := iter + 1;
  
  end;
   {Vapor Phase}
  AMaterial.Phases.Clear;
  
  if PhaseExist[1] then AMaterial.Phases.Add.Assign(VaporPhase);
  if PhaseExist[2] then AMaterial.Phases.Add.Assign(Liquid1Phase);
  if PhaseExist[3] then AMaterial.Phases.Add.Assign(Liquid2Phase);
  
  VaporPhase.Free;
  Liquid1Phase.Free;
  Liquid2Phase.Free;
  
end;

function TThreePhaseFlash.TwoPhaseSum(const K: array of Real; const 
        AComposition: TCompositions; LiquidFraction : Real): Real;
var
  i: Integer;
  Total: Double;
  
  {Still thinking about the exact way to phrase this}
  
begin
  Total := 0.0;
  for i := 0 to AComposition.Count - 1  do
    Total:=Total + (1.0-K[i])*AComposition[I].MoleFraction.Value /(LiquidFraction+(1-LiquidFraction)*K[i]);
  Result := Total;
  
end;

procedure TThreePhaseFlash.UpdateKValues(var VaporPhase, LiquidPhase: TPhase; 
        var K:array of Real);
var
  i: Integer;
  
  {Updates K Values for a pair of Phases.  Will Fail if K's are 0.0.  Also updates the Compressibiliteis
  and Fugasity Coefficients}
  
begin
  FEOS.Solve(VaporPhase);
  FEOS.Solve(LiquidPhase);
  for i := 0 to VaporPhase.Compounds.Count -1 do
    K[i] := LiquidPhase.Compositions[i].FugacityCoefficient.Value /
            VaporPhase.Compositions[i].FugacityCoefficient.Value * K[i];
end;

{
****************************** TEquilibriumServer ******************************
}
procedure TEquilibriumServer.CalcEquilibrium(AMaterial: TMaterial);
begin
end;

end.
