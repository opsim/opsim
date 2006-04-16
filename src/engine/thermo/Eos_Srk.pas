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
  public
    function FindRoots(APhase: TPhase): Variant; override;
  end;
  
implementation

{
*********************************** TSrkEos ************************************
}
function TSrkEos.FindRoots(APhase: TPhase): Variant;
  
  const
    R = 0.08206; {atm*m^3/(kgmol*K)} {temporary}
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
  
  //Return the three roots.
  Result := FindCubicRoots(-1.0, MainA - MainB - Sqr(MainB), -1.0 * MainA * MainB);
end;

end.
