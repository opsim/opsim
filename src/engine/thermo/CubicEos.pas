{ $Id: Eos.pas 287 2006-05-25 13:51:23Z samuelcartaxo $ }
{
 /***************************************************************************

                  Abstract: Base classes for equation of state.
                  Initial Revision : 15/04/2006
                  Authors:
                    - Samuel Jorge Marques Cartaxo
                    - Khaled Hammadi (Itinerant)
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
unit CubicEos;

interface

uses
  SysUtils, Classes, Variants, Entities, Thermo, Math, Eos;

type
  {:
  - Specialized behavior of the Eos Class.
  - Base Class for the Cubic Eos type.
  - At the same time This Class is the VanDerWaals Eos.
  }

  TCubicEos = class (TEos)
  protected
// Variables needed for Compressibility factor calculation and its derivatives
    eosA, eosB, eosBB : double;
    
    AC, BC, MC, ALFA, AALF,ALFA1,ALFA2,X1,X2,XSTAR,Z1,Z2,ZSTAR : array of double;
    ZX11, ZX12, ZX22, AALFA, Q8 : double;
    Q1,Q3,Q4,N,DZA,DZB,DAT,DBT,DZT,DQ1T,DQ2,DQ6,DAP,DBP,DQ1P,DZP : double;
    Q2, Q5, Q6, Q7,DAX, DBX, DZX : array of double;

    PHI, DPHIT, DPHIP, DHX : array of double;
    DPHIX : array of array of double;
    DHP : double;
    KIJ :  array of array of double;

    calcdT, calcdP,calcDX : boolean;
    needPostProcess:boolean;
  public
    constructor Create; override;
    destructor Destroy; override;
    procedure UpdateCompounds(APhase:TPhase); override;
    procedure MixRules(APhase:TPhase); virtual;
    function FindRoots(APhase:TPhase) : Variant; override;
    function FindCubicRoots(A, B, C: Double): Variant;
    
    procedure PostProcessCompressibility(T,P,Z:double);
    procedure CalcFugacityCoefficients(APhase:TPhase);override;
    procedure CalcDepartures(APhase:TPhase); override;
    function GetEnthalpy(APhase:TPhase):double;
    procedure SetKij(i,j:integer;aKij:double);override;
    function residus(Z,A,B,C:double):double;
  end;
  
implementation

const
//  RGAS = 0.08206; {atm*m^3/(kgmol*K)} {temporary}
  ZERO = 0.0;
  ONE = 1.0;
  TWO = 2.0;
  THREE = 3.0;

{
********************************** TCubicEos ***********************************
}
constructor TCubicEos.Create;
begin
  inherited Create;
  needPostProcess:=false;
end;
destructor TCubicEos.Destroy;
begin
//Deallocate dynamic arrays.

  ac := nil;
  bc := nil;
  mc := nil;
  alfa := nil;
  aalf := nil;
  alfa1 := nil;
  alfa2 := nil;
  X1 := nil;
  X2 := nil;
  XSTAR := nil;
  Z1 := nil;
  Z2 := nil;
  ZSTAR := nil;
  KIJ := nil;
// todo test if they have been allocated...
  PHI := nil;
  DPHIT := nil;
  DPHIP := nil;
  DPHIX := nil;
  DHX := nil;
  Q2 := nil;
  Q5 := nil;
  Q6 := nil;
  Q7 := nil;
  DAX := nil;
  DBX := nil;
  DZX := nil;

  inherited Destroy;
end;

function TCubicEos.residus(Z,A,B,C:double):double;
begin
  Result := C+Z*(B+Z*(A+Z));
end;

function TCubicEos.FindCubicRoots(A, B, C: Double): Variant;
var
  Q: Double;
  R: Double;
  S: Double;
  T: Double;
  theta: Double;
  Zroots,check:double;
begin
  Q := (Sqr(A) - 3.0e0 * B) / 9.0e0;
  R := (2.0 * IntPower(A, 3) - 9.0 * A * B + 27.0 * C) / 54.0;
  if (Sqr(R) < IntPower(Q, 3)) then begin
    theta := ArcCos(R / (Sqrt(IntPower(Q, 3))));
    //Make room for the three roots.
    Result := VarArrayOf([0,0,0]);
    ZRoots :=-2.0 * Sqrt(Q) * cos(theta / 3.0) - A / 3.0;
    check:=residus(ZRoots,A,B,C);
    Result[0] := ZRoots;
    ZRoots :=-2.0 * Sqrt(Q) * cos((theta + 2.0 * pi) / 3.0) - A / 3.0;
    check:=residus(ZRoots,A,B,C);
    Result[1] := ZRoots;
    ZRoots :=-2.0 * Sqrt(Q) * cos((theta - 2.0 * pi) / 3.0) - A / 3.0;
    check:=residus(ZRoots,A,B,C);
    Result[2] := ZRoots;
  end
  else begin
    S := abs(R) + Sqrt(Sqr(R) - IntPower(Q, 3));
    S := Power(S, 0.3333333);
    S := -1.0 * sign(R) * S;
    if S = 0 then
      T := 0
    else
      T := Q / S;
    //If the root is unique, return an array with only one element.
    //Itinerant. Low and High does not seem to work with array of variant  ???
    //for instance we'll retrun always an array of 3 solutions
    Result := VarArrayOf([0,0,0]);
    Result[0] := (S + T) - A / 3.0;;
    Result[1] := ZeroValue;
    Result[2] := ZeroValue;
  end;//if
end;


procedure TCubicEos.UpdateCompounds(APhase:TPhase);
var
  I, J : integer;
  
  R2764, R18 : double;
  LocalPc, LocalTC, RGASTC:double;
begin
  CalcdT := false;
  CalcdP := false;
  CalcDX := false;
  
  NComp := APhase.Compounds.Count;

//Allocate and Initializes auxiliar arrays.
  SetLength(ac, NComp);
  SetLength(bc, NComp);
  SetLength(mc, NComp);
  SetLength(alfa, NComp);
  SetLength(aalf, NComp);
  SetLength(alfa1, NComp);
  SetLength(alfa2, NComp);
  SetLength(X1, NComp);
  SetLength(X2, NComp);
  SetLength(XSTAR, NComp);
  SetLength(Z1, NComp);
  SetLength(Z2, NComp);
  SetLength(ZSTAR, NComp);
  SetLength(KIJ, NComp,NComp);

// some test if we really need to...
  SetLength(PHI, NComp);
  SetLength(DPHIT, NComp);
  SetLength(DPHIP, NComp);
  SetLength(DPHIX, NComp, NComp);
  SetLength(Q2, NComp);
  SetLength(Q5, NComp);
  SetLength(Q6, NComp);
  SetLength(Q7, NComp);
  SetLength(DAX, NComp);
  SetLength(DBX, NComp);
  SetLength(DZX, NComp);
  SetLength(DHX, NComp);

//Itinerant:13:11:2006
// We calculate theses Array once and for all to speedUp calculation
// as they do not depend on the Phase conditions (T, P or Composition)
// By default the Cubic EOS is That of VW
// Be sure to re-call Initialize method if any of component properties
// change (Pc, TC, ...etc) or the number/ident of components.
//

   R2764 := 27.0 / 64.0;
   R18 := 1.0 / 8.0;
   with APhase do
   begin
     for J:=0 to NComp-1 do
     begin
       with Compounds[J] do begin
         LocalPc := PC.Value; // speedup
         LocalTc := TC.Value;
         RgasTC := RGAS * LocalTC;

         AC[J] := SQRT(R2764 * RGASTC * RGASTC / LocalPC);
         BC[J] := R18 * RGASTC / LocalPC;
         MC[J] := ZERO;
         ALFA[J] := ONE;
         AALF[J] := AC[J]*ALFA[J];
         ALFA1[J] := AC[J] * (ONE+MC[J]);
         ALFA2[J] := - AC[J] * MC[J] / SQRT(LocalTC);
       end;
     end;
   end;
end;

procedure TCubicEos.MixRules(APhase:TPhase);
{
OutPut
       : X1,X2,Z1,Z2,X*,Z*,AALFA,ZX11,ZX12,ZX22,
       : eosA,eosB,eosBB
}
var
  I,J : Integer;
  SQRTT : double;
  // todo Add these variables as Class.Attribute
  eosTemperature, eosPressure, MolFracJ : double;
begin
   eosBB := ZERO;
   with APhase do
   begin
     eosTemperature := Temperature.Value;
     eosPressure := Pressure.Value;
     
     SQRTT := SQRT(eosTemperature);
// todo : find a way to Update this part only when needed (P changed from last call)
// Begin code Pressure independante
     for J:=0 to NComp-1 do
     begin
       with Compounds[J] do begin
         MolFracJ := Compositions[J].MoleFraction.Value;
         
         X1[J] := ALFA1[J] * MolFracJ;
         X2[J] := ALFA2[J] * MolFracJ;
         XSTAR[J] := X1[J]+X2[J]* SQRTT;
         eosBB := eosBB + MolFracJ * BC[J];
       end;
     end;

     ZX11 := ZERO;
     ZX12 := ZERO;
     ZX22 := ZERO;
     for J:=0 to NComp-1 do
     begin
       Z1[J] := ZERO;
       Z2[J] := ZERO;
       for I:=0 to NComp-1 do
       begin
         Z1[J] := Z1[J] + X1[I] * (1.0 - KIJ[J,I]);
         Z2[J] := Z2[J] + X2[I] * (1.0 - KIJ[J,I]);
       end;
       ZSTAR[J] := Z1[J] + SQRTT * Z2[J];
       ZX11 := ZX11 + X1[J] * Z1[J];
       ZX12 := ZX12 + X1[J] * Z2[J];
       ZX22 := ZX22 + X2[J] * Z2[J];
     end;
// End code Pressure independante

     AALFA := ZX11 + TWO*ZX12*SQRTT + ZX22*eosTemperature;
     eosA := AALFA * eosPressure / (RGAS * eosTemperature)**2;
     eosB := eosBB * eosPressure / (RGAS * eosTemperature);

// Needed only in Enthalpydepartue...
     Q8 := ZERO;
     for I:=1 to NCOMP-1 do
       Q8 := Q8 + XSTAR[I]*ZSTAR[I]*(ONE+MC[I])/ALFA[I];
       
     Q8 := Q8 / eosBB
   end;
end;
function TCubicEos.FindRoots(APhase:TPhase): Variant;
var
  CubB, CubC, CubD : Double;
begin
// Calculate the mnixing rules depending on the EOS
// todo: fixe the calling sequence to avoid calling MixtureRule more than once
  MixRules(APhase);

//Itinerant:13:11:2006
// This formula holds true for VW, RK, RKS and PR. Check validity when
// you add another CubicDEos

   CubB := -ONE;
   CubC := eosA-eosB-(eosB*eosB);
   CubD := -eosA*eosB;

  Result := FindCubicRoots(CubB, CubC, CubD);
// todo: (in case needed by user preferences) add some solution refinement
// throught a newton-raphson-like algo iterations
end;

procedure TCubicEos.CalcFugacityCoefficients(APhase:TPhase);
{******************************************************************
     Get fugacity coefficients for a mixture and their derivatives

     d[Ln(Phi)]/dT , d[Ln(Phi)]/dx, d[Ln(Phi)]/dP.

     Original file (in fortran) by Itinerant(K.Hammadi) 18:02:1995
     First convertion to Pascal by Itinerant(K.Hammadi) 13:11:2006
*******************************************************************}
var
  T : double;
  P : double;
  
  Z,toto : double;

  I, J : byte;

  TERME,TERM00,TERM01,TERM02 : double;
begin
//Itinerant:13:11:2006
// todo store copies of the results and check if we need to UpDate them

      Z := FindPhaseRoot(APhase);
      T := APhase.Temperature.Value;
      P := APhase.Pressure.Value;

      PostProcessCompressibility(T,P,Z);

      for I := 0 to NCOMP-1 do
      begin
        toto := BC[I];
        toto := BC[I]*(Z-ONE)/eosBB;
        toto := (Z-eosB);
        toto := q2[i];
        toto :=-Q1*Q2[I];
        toto := exp(toto);
        PHI[I]  :=  EXP( BC[I]*(Z-ONE)/eosBB ) / (Z-eosB) * EXP(-Q1*Q2[I]);
      end;

// Compute Temperature derivative ? By default no

      if (calcdT)then
      begin
        for I := 0 to NCOMP-1 do
        begin
          Q3 := ALFA1[I]*(TWO*Z1[I]+Z2[I]*SQRT(T))+ALFA2[I]*Z1[I]*SQRT(T);
          DQ2 := -(Q3+BC[I]*Q4/eosBB)/eosBB/RGAS/SQR(T);
          DPHIT[I] := BC[I]*DZT/eosBB - (DZT-DBT)/(Z-eosB) - DQ1T*Q2[I] - DQ2*Q1;
        end;
      end;
//      else
//        for I := 0 to NCOMP-1 do
//          DPHIT[I] := ZERO; // todo speedup

// compute composition derivative?
     if (CalcdX)then
     begin
       for J := 0 to NCOMP-1 do
       begin
         TERME := (DZX[J]-BC[J]*(Z-1)/eosBB)/eosBB;
         for I := 0 to NCOMP-1 do
         begin
//todo KIJ           Term00 := AALF[J]*AALF[I]*(ONE-KIJ[I,J]) /AALFA;
           Term00 := AALF[J]*AALF[I]*(ONE) /AALFA;
           Term01 := AALF[I]*ZSTAR[I]*BC[J];
           Term02 := AALF[J]*ZSTAR[J]*BC[I];
           
           DQ6 :=  TWO*eosA*( (Term00-(Term01+Term02)/eosBB)/AALFA + BC[I]*BC[J]/eosBB/eosBB ) / eosB;
           DPHIX[I,J] := BC[I]*TERME-Q5[J]-Q6[I]*Q7[J]-Q1*DQ6;
         end;
       end;
     end;
//     else
//         for I := 0 to NCOMP-1 do // todo:speedup
//           for J := 0 to NCOMP-1 do
//             DPHIX[I,J]  :=  ZERO;

     if (CalcdP)then
     begin
       for I := 0 to NCOMP-1 do
         DPHIP[I] := BC[I]*DZP/eosBB-(DZP-DBP)/(Z-eosB)-DQ1P*Q2[I];
     end;
//     else
//       for I := 0 to NCOMP-1 do // todo:speedup
//         DPHIP[I] := ZERO;
         
  for I := 0 to NCOMP-1 do
    APhase.Compositions[I].FugacityCoefficient.Value := Phi[I];
end;

procedure TCubicEos.CalcDepartures(APhase:TPhase);
{******************************************************************
     Get Enthalpy Departure for a mixture and its derivatives

     d[DH]/dT , d[DH]/dx, d[DH]/dP.

     Original file (in fortran) by Itinerant(K.Hammadi) 18:02:1995
     First convertion to Pascal by Itinerant(K.Hammadi) 15:11:2006
*******************************************************************}
var
  T : double;
  P : double;
  Z : double;
  I, J : byte;
  DELTAH,DHDT,DQ8 : double;
  PerfectGasEnthalpy:TValueRec;
begin
// todo store copies of the results and check if we need to UpDate them
//  Mixrules;
  Z := FindPhaseRoot(APhase);
  T := APhase.Temperature.Value;
  P := APhase.Pressure.Value;

  PostProcessCompressibility(T,P,Z);
  
  DELTAH := RGAS*T*(Z-ONE)+Q1*Q4/eosBB;

  if (calcdT)then
    DHDT:=RGAS*(Z-ONE+T*DZT)-(Q1*(ZX12/SQRT(T))/TWO - Q4*DQ1T)/eosBB;

// Composition derivatives
  if (CalcdX)then
   begin
     for I:=0 to NCOMP-1 do
     begin
      DQ8 := -BC[I]*Q8/eosBB+(ALFA1[I]*ZSTAR[I]+AALF[I]*Z1[I])/eosBB;
      DHX[I] := RGAS*T*DZX[I] - Q1*DQ8 - Q8*Q7[I];
     end;
   end;

// Pressure derivative
  if (CalcdP)then
     DHP := RGAS*T*DZP+DQ1P*Q4/eosBB;

   APhase.EnthalpyDeparture.Value:=DeltaH;
   PerfectGasEnthalpy := HVMXID(APhase);
   APhase.Enthalpy.Value := APhase.EnthalpyDeparture.Value + PerfectGasEnthalpy.Value;
end;

procedure TCubicEos.PostProcessCompressibility(T,P,Z:double);
//Itinerant:15:11:2006
// I think the most efficient way is to have two distinct route and
// as many methods : One route for simple calculation of Z, Phi, DH and DS
// and another route for these values and their derivatives.
// (add 'WithDerivatives' in the methodes names for example)
//
// for now we still use the if's tests method.
var
  I,J : Byte;
begin
// todo required test to procede only
// if CompressilityFactor(Z) is different from the
// last call. : Else Return.
if (needPostProcess) then
begin
  Q1 := LN(1+eosB/Z);

// Needed for Phi.
  for I := 0 to NCOMP-1 do
    Q2[I]   :=  (TWO*AALF[I]*ZSTAR[I] - BC[I]*AALFA/eosBB)/(eosBB*RGAS*T);

  if (calcdT)then
  begin
    Q4 := -(ZX11+ZX12*SQRT(T));
    N := Z*(THREE*Z-TWO)+eosA-eosB*(ONE+eosB);
    DZA := (eosB-Z)/N;
    DZB := (Z*(ONE+TWO*eosB)+eosA)/N;
    DAT := eosA*(Q4/AALFA-1)/T;
    DBT := -eosB/T;
    DZT := DZA*DAT+DZB*DBT;
    DQ1T := (DBT-eosB*DZT/Z)/(Z+eosB);
  end;
// compute composition derivative?
  if (CalcdX)then
  begin
    for I := 0 to NCOMP-1 do
    begin
      DAX[I]  :=  TWO*eosA*AALF[I]*ZSTAR[I]/AALFA;
      DBX[I]  :=  eosB*BC[I]/eosBB;
      DZX[I] := DZA*DAX[I]+DZB*DBX[I];
      Q5[I]  := (DZX[I]-DBX[I])/(Z-eosB);
      Q6[I]   :=  DAX[I]/eosB - BC[I]*eosA/eosB/eosBB;
      Q7[I]  := (DBX[I]-eosB*DZX[I]/Z)/(Z+eosB);
    end;
  end;

  if (CalcdP)then
  begin
    DAP := AALFA/SQR(RGAS*T);
    DBP := eosBB/RGAS/T;
    DZP := DZA*DAP+DZB*DBP;
    DQ1P := eosB*(DBP/eosB-DZP/Z)/(Z+eosB);
  end;
end;
end;

function TCubicEos.GetEnthalpy(APhase:TPhase):double;
var
  temp : TValueRec;
begin
  CalcDepartures(APhase);
  temp := HVMXID(APhase);
  APhase.Enthalpy.Value := APhase.EnthalpyDeparture.Value + temp.Value;
  Result := APhase.Enthalpy.Value;
end;


procedure TCubicEos.SetKij(i,j:integer;aKij:double);
var
  ip1,jp1:integer;
begin
  ip1:=i+1;
  jp1:=j+1;
// todo : Tests i, j bound values (Ncomp/0)
  if( (ip1<NComp) and (jp1<NComp) and (0<i) and (j<0))then
  begin
    KIJ[ip1,jp1]:=aKij;
    KIJ[jp1,ip1]:=aKij;
  end;
end;

end.
