{ $Id$ }
{
 /***************************************************************************

                  Abstract: Base classes for equation of state.
                  Initial Revision : 15/04/2006
                  Authors:
                    - Samuel Jorge Marques Cartaxo
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
unit Eos;

interface

uses
  SysUtils, Classes, Variants, Entities, Thermo, Math;

type
  {{
  Base class for all equations of state.
  }
  TEos = class (TObject)
  protected
    procedure CalcCompressibilityFactors(APhase: TPhase; var ZVapor, ZLiquid: 
            Real); virtual;
    procedure CalcDepartures(APhase: TPhase; var Enthalpy, Entropy: TValueRec); 
            virtual;
    function CompressibilityFactor(APhase: TPhase): TValueRec; virtual;
    function EnthalpyDeparture(APhase: TPhase): TValueRec; virtual;
    function EntropyDeparture(APhase: TPhase): TValueRec; virtual;
    function FindRoots(APhase: TPhase): Variant; virtual;
    function FugacityCoefficients(APhase: TPhase): TValueRec; virtual;
  public
    {{
    Completly solve the phase object with the equation of state. After excuting 
    this method, the compressibility factor and fugacity coefficients for the 
    phase should are defined.
    }
    procedure Solve(APhase: TPhase); virtual;
  end;
  
  TCubicEos = class (TEos)
  protected
    procedure CalcCompressibilityFactors(APhase: TPhase; var ZVapor, ZLiquid: 
            Real); override;
    function CompressibilityFactor(APhase: TPhase): TValueRec; override;
    function FindCubicRoots(A, B, C: Double): Variant;
    function FindRoots(APhase: TPhase): Variant; override;
  end;
  

implementation

{
************************************* TEos *************************************
}
procedure TEos.CalcCompressibilityFactors(APhase: TPhase; var ZVapor, ZLiquid: 
        Real);
begin
end;

procedure TEos.CalcDepartures(APhase: TPhase; var Enthalpy, Entropy: TValueRec);
begin
end;

function TEos.CompressibilityFactor(APhase: TPhase): TValueRec;
begin
end;

function TEos.EnthalpyDeparture(APhase: TPhase): TValueRec;
begin
end;

function TEos.EntropyDeparture(APhase: TPhase): TValueRec;
begin
end;

function TEos.FindRoots(APhase: TPhase): Variant;
begin
  Result := VarArrayOf([]);
end;

function TEos.FugacityCoefficients(APhase: TPhase): TValueRec;
begin
end;

procedure TEos.Solve(APhase: TPhase);
begin
end;

{
********************************** TCubicEos ***********************************
}
procedure TCubicEos.CalcCompressibilityFactors(APhase: TPhase; var ZVapor, 
        ZLiquid: Real);
var
  Roots: Variant;
  MinimumTemp: Double;
  i: Integer;
begin
  inherited CalcCompressibilityFactors(APhase, ZVapor, ZLiquid);
  Roots := FindRoots(APhase);
end;

function TCubicEos.CompressibilityFactor(APhase: TPhase): TValueRec;
begin
  Result := inherited CompressibilityFactor(APhase);
  //Find the minimum root.
  //If second root is 0, there was only one result.
  //if Roots[1] = 0 then
    //ZLiquid := Roots[0]
  //else begin
    //MinimumTemp := MaxDouble;
    //for i := 0 to 2 do
      //if (Roots[i] > 0) and (Roots[i] < MinimumTemp) then
        //MinimumTemp := Roots[i];
    //ZLiquid := MinimumTemp
  //end;
  
  //Get the maximum root.
  //ZVapor := MaxValue([Real(Roots[0]), Real(Roots[1]), Real(Roots[2])]);
end;

function TCubicEos.FindCubicRoots(A, B, C: Double): Variant;
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
    //Make room for the three roots.
    Result := VarArrayOf([0,0,0]);
    Result[0] := -2.0 * Sqrt(Q) * cos(theta / 3.0) - A / 3.0;
    Result[1] := -2.0 * Sqrt(Q) * cos((theta + 2.0 * pi) / 3.0) - A / 3.0;
    Result[2] := -2.0 * Sqrt(Q) * cos((theta - 2.0 * pi) / 3.0) - A / 3.0;
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
    Result := VarArrayOf([0]);
    Result[0] := (S + T) - A / 3.0;
    //Result[1] := 0.0;
    //Result[2] := 0.0;
  end;//if
end;

function TCubicEos.FindRoots(APhase: TPhase): Variant;
begin
  Result := inherited FindRoots(APhase);
  //Initializaes a variant array with three elements.
  Result := VarArrayOf([0, 0, 0]);
end;

end.
