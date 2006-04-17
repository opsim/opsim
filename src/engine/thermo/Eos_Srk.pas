{ $Id$ }
{
 /***************************************************************************

                  Abstract: Implementation of SRK equation of state.
                  Initial Revision : 15/04/2006
                  Authors:
                    - Samuel Jorge Marques Cartaxo
                    - Matt Henley
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
unit Eos_Srk;

interface

uses
  SysUtils, Classes, Entities, Thermo, Eos;

type
  TSrkEos = class (TCubicEos)
  protected
    procedure CalcDepartures(APhase: TPhase); override;
    function CalcFugacityCoefficients(APhase: TPhase): TValueRec; override;
    function FindRoots(APhase: TPhase): Variant; override;
  end;
  
implementation

const
  R = 0.08206; {atm*m^3/(kgmol*K)} {temporary}

{
*********************************** TSrkEos ************************************
}
procedure TSrkEos.CalcDepartures(APhase: TPhase);
  
  const
    P0 = 1; {Reference Pressure... i assume to be 1 atms}
  var
    MainA: double;
    MainB: double;
    a: double;
    b: double;
    i, j: integer;
    ai, bi, mi: array of Real;
    dadt: double;
  
begin
  inherited CalcDepartures(APhase);
  with APhase do begin
  
    //Initializes auxiliar arrays.
    SetLength(ai, Compounds.Count);
    SetLength(bi, Compounds.Count);
    SetLength(mi, Compounds.Count);
  
    b := 0.0;
    for i := 0 to Compounds.Count - 1 do begin
      with Compounds[i] do begin
        mi[i] := 0.48 + 1.574 * w.Value - 0.176 * (w.Value)**2;
        bi[i] := 0.08664 * R * Tc.Value / Pc.Value;
        ai[i] := 0.42748 * (R * Tc.Value)**2 / Pc.Value * Sqr(1 + mi[i] *
                 (1 - Sqrt(Temperature.Value / Tc.Value)));
        b := b + Compositions[i].MoleFraction.Value * bi[i];
      end;//with
    end;//for
    a := 0;
    for i := 0 to Compounds.Count - 1 do
      for j := 0 to Compounds.Count - 1 do
        a := a + Compositions[i].MoleFraction.Value *
             Compositions[j].MoleFraction.Value * Sqrt(ai[i] * ai[j]);
    MainA := a * Pressure.Value / (R * Temperature.Value)**2;
    MainB := b * Pressure.Value / (R * Temperature.Value);
    EnthalpyDeparture.Value := (CompressibilityFactor.Value - 1.0 - 1.0 / (b * R * Temperature.Value) *
                (a - Temperature.Value * dadt) * ln(1 + b / MoleVolume.Value)) *
                R * Temperature.Value;
    EntropyDeparture.Value := (ln(CompressibilityFactor.Value - b) - ln(Pressure.Value / P0) +
                     MainA / MainB * (Temperature.Value / a * dadt) *
                     ln(1 + MainB / CompressibilityFactor.Value)) * R * Temperature.Value;
  
  end;//with
  
  //Deallocate dynamic arrays.
  ai := nil;
  bi := nil;
  mi := nil;
end;

function TSrkEos.CalcFugacityCoefficients(APhase: TPhase): TValueRec;
var
  V, test: Double;
  a, bigA: Double;
  b, bigB: Double;
  tempsum: Double;
  i, j: Integer;
  ai, bi, mi: array of Real;
begin
  Result := inherited CalcFugacityCoefficients(APhase);
  with APhase do begin
  
    //Initializes auxiliar arrays.
    SetLength(ai, Compounds.Count);
    SetLength(bi, Compounds.Count);
    SetLength(mi, Compounds.Count);
  
    b := 0.0;
    for i := 0 to Compounds.Count - 1 do begin
      with Compounds[i] do begin
        mi[i] := 0.48 + 1.574 * w.Value - 0.176 * w.Value**2;
        bi[i] := 0.08664 * R * Tc.Value / Pc.Value;
        ai[i] := 0.42748 * (R * Tc.Value)**2 / Pc.Value *
                 Sqr(1 + mi[i] * (1 - Sqrt(Temperature.Value / Tc.Value)));
        b := b + Compositions[i].MoleFraction.Value * bi[i];
      end;//with
    end;//for
    bigB := b * Pressure.Value / (R * Temperature.Value);
  
    a := 0;
    for i := 0 to Compositions.Count - 1 do
      for j := 0 to Compositions.Count - 1 do
        a := a + Compositions[i].MoleFraction.Value *
             Compositions[j].MoleFraction.Value * Sqrt(ai[i] * ai[j]);
    bigA := a * Pressure.Value / (R * Temperature.Value)**2;
  
    for i := 0 to Compositions.Count - 1 do begin
      test := bi[i] / b * (CompressibilityFactor.Value - 1.0) -
              ln(CompressibilityFactor.Value - bigB) - bigA / bigB *
              (2.0 * Sqrt(ai[i] / a) - bi[i] / b) *
              ln(1 + bigB / CompressibilityFactor.Value);
      //Define the fugacity coefficient for each compound in the phase.
      Compositions[i].FugacityCoefficient.Value := exp(test) * Pressure.Value *
                                                   Compositions[i].MoleFraction.Value;
    end;//for
  
  end;//with
  
  //Deallocate dynamic arrays.
  ai := nil;
  bi := nil;
  mi := nil;
end;

function TSrkEos.FindRoots(APhase: TPhase): Variant;
var
  MainA: Double;
  MainB: Double;
  a: Double;
  b: Double;
  i, j: Integer;
  ai, bi, mi: array of Real;
begin
  Result := inherited FindRoots(APhase);
  with APhase do begin
  
    //Initializes auxiliar arrays.
    SetLength(ai, Compounds.Count);
    SetLength(bi, Compounds.Count);
    SetLength(mi, Compounds.Count);
  
    b := 0.0;
    for i := 0 to Compounds.Count - 1 do begin
      with Compounds[i] do begin
        mi[i] := 0.48 + 1.574 * w.Value - 0.176 * w.Value**2;
        bi[i] := 0.08664 * R * Tc.Value / Pc.Value;
        ai[i] := 0.42748 * (R * Tc.Value)**2 / Pc.Value * (1 + mi[i] *
                 (1 - Sqrt(Temperature.Value / Tc.Value)))**2;
        b := b + Compositions[i].MoleFraction.Value * bi[i];
      end;//with
    end;//for
  
    a := 0;
    for i := 0 to Compounds.Count - 1 do
      for j := 0 to Compounds.Count - 1 do
        a := a + Compositions[i].MoleFraction.Value *
             Compositions[j].MoleFraction.Value * Sqrt(ai[i] * ai[j]);
  
    MainA := a * Pressure.Value / (R * Temperature.Value)**2;
    MainB := b * Pressure.Value / (R * Temperature.Value);
  
  end;//with
  
  //Deallocate dynamic arrays.
  ai := nil;
  bi := nil;
  mi := nil;
  
  //Return the significant roots.
  Result := FindCubicRoots(-1.0, MainA - MainB - Sqr(MainB), -1.0 * MainA * MainB);
end;

end.
