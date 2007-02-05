{ $Id$ }
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
unit Eos;

interface

uses
  SysUtils, Classes, Variants, Entities, Thermo, Math;

type
  {:
  - Base class for all equations of state.
  - Can solve completely a phase through its static Solve method. It is 
  designed in order that this is the only method a TFlash class needs to call 
  to define a phase.
  - Specialized behavior is delegated to descendant classes through the other 
  virtual methods.
  }
  TEos = class (TObject)
  protected
    fNComp : integer;
    function CalcCompressibilityFactor(APhase: TPhase):TValueRec; virtual;
    function FindRoots(APhase: TPhase): Variant; virtual;
  public
    property NComp : integer read fNComp write fNComp;
    {:
    - Completly solves the phase object with the equation of state. After 
    excuting 
    this method, the compressibility factor and fugacity coefficients for the 
    phase should are defined.
    - Defines completely the APhase parameter using the equation of state, 
    state 
    of agrgregation and compositions.  
    }
    constructor Create; virtual;
    destructor Destroy; override;
    procedure UpdateCompounds(APhase: TPhase);virtual;
    procedure Solve(APhase: TPhase);virtual;
    procedure CalcDepartures(APhase: TPhase); virtual;
    procedure CalcFugacityCoefficients(APhase: TPhase); virtual;
    function FindPhaseRoot(APhase: TPhase):double;
    procedure SetKij(i,j:integer;aKij:double);virtual;
  end;
  
const

  RGAS = 0.08206; {atm*m^3/(kgmol*K)} {temporary}
implementation

{
************************************* TEos *************************************
}
constructor TEos.Create;
begin
  inherited Create;
end;

destructor TEos.Destroy;
begin
  inherited Destroy;
end;

procedure TEos.UpdateCompounds(APhase: TPhase);
begin

end;

function TEos.CalcCompressibilityFactor(APhase: TPhase): TValueRec;
begin
end;

procedure TEos.CalcDepartures(APhase: TPhase);
begin
end;

procedure TEos.CalcFugacityCoefficients(APhase: TPhase);
begin
end;

function TEos.FindRoots(APhase: TPhase): Variant;
begin
  Result := VarArrayOf([]);
end;

procedure TEos.Solve(APhase: TPhase);
var
  Roots: array of Variant;
  I: Integer;
  r,mv,z,t,p:real;
begin
  //Defines completly the APhase parameter using the equation of state,
  //state of agrgregation and compositions.
  
  //Calculate the roots of the equation.
  Roots := FindRoots(APhase);
  with APhase do begin
    //Calculate the compressibility factor for the phase acording to the physical
    //state.
    case AggregationState of
  
      asLiquid: begin
  
        //Find the smallest root for liquid phase.
        CompressibilityFactor.Value := MaxDouble;
        for I := 0 to 2 do
          if (Roots[I] > 0) and (Roots[I] < CompressibilityFactor.Value) then
            CompressibilityFactor.Value := Roots[I];
  
      end;//asLiquid
  
      asVapor: begin
  
        //Find the greatest root for the a vapor phase.
        CompressibilityFactor.Value := MinDouble;
        for I := 0 to 2 do
          if (Roots[I] > 0) and (Roots[I] > CompressibilityFactor.Value) then
            CompressibilityFactor.Value := Roots[I];
  
      end;//asVapor
  
      //Raise an exception if phase state has no meaning.
      else
        raise Exception.Create('Cannot calculate compressibility factor because phase''s state is not defined.');
  
    end;//case
  end;//with
  
  //Once the compressibility factor is defined, calculates the fugacities
  //coefficients for all compounds.
  CalcFugacityCoefficients(APhase);
  
  //Now calculates the enthalpy and entropy departures.
  CalcDepartures(APhase);
  //Update MoleVolume
  t:=APhase.Temperature.Value;
  p:=APhase.Pressure.Value;
  z:=APhase.CompressibilityFactor.Value;
  r:=rgas;
  mv:=z*r*t/p;
  APhase.MoleVolume.Value:=APhase.CompressibilityFactor.Value*RGAS*APhase.Temperature.Value/APhase.Pressure.Value;
  //Update MassVolume
//  rho := 0.0;
//  for i:=0 to NComp-1 do
//    rho := rho + APhase.Compounds[i].MW.Value*APhase.Compositions[i].MoleFraction.Value;
//  rho := rho / APhase.MoleVolume.Value;
end;

function TEos.FindPhaseRoot(APhase: TPhase):double;
var
  zRoots: Variant; // Strange. array of Variant do not work ???
  I: Integer;
  ZVGuess, ZLGuess : Double;
begin
  //Calculate the roots of the equation.
  zRoots := FindRoots(APhase);
  with APhase do begin
    //Calculate the compressibility factor for the phase acording to the physical
    //state.
    case AggregationState of

      asLiquid: begin

        //Find the smallest root for liquid phase.
        CompressibilityFactor.Value := MaxDouble;
        for I := 0 to 2 do
          if (zRoots[I] > 0) and (zRoots[I] < CompressibilityFactor.Value) then
          begin
            zVGuess :=zRoots[I];
            CompressibilityFactor.Value := zRoots[I];
          end;

      end;//asLiquid

      asVapor: begin

        //Find the greatest root for the a vapor phase.
        CompressibilityFactor.Value := MinDouble;
        for I := 0 to 2 do
          if (zRoots[I] > 0) and (zRoots[I] > CompressibilityFactor.Value) then
          begin
            zVGuess :=zRoots[I];
            CompressibilityFactor.Value := zRoots[I];
          end

      end;//asVapor

      asUndefined: begin
//Itinerant. todo return the number of double solution and a way to give them back
// to the user. Actually (Phase) has no room for that.
//
// We return the vapor solution by default
// Suppose Vapor
        ZVGuess:= MinDouble;
        for I := 0 to 2 do   // Low(Roots) does not work ???
          if (zRoots[I] > 0) and (zRoots[I] > ZVGuess) then
            ZVGuess := zRoots[I];
            
        ZLGuess:= MaxDouble;
        for I := 0 to 2 do
          if (zRoots[I] > 0) and (zRoots[I] < ZLGuess) then
            ZLGuess := zRoots[I];


        CompressibilityFactor.Value:=ZVGuess;
        AggregationState:=asVapor;
        
        if (ZVGuess > 1) then
        begin
          CompressibilityFactor.Value:=ZLGuess;
          AggregationState:=asLiquid;

      //Raise an exception if the two solutions has no physical meaning.
          if (ZLGuess >1) then
            raise Exception.Create('Calculated compressibility factor has no physical meaning (>1).');

        end;
      end;//asUndefined

      //Raise an exception if phase state has no meaning.
      else
        raise Exception.Create('Cannot calculate compressibility factor because phase''s state is not defined.');

    end;//case
  Result := CompressibilityFactor.Value;
  end;//with
  
end;
procedure TEos.SetKij(i,j:integer;aKij:double);
begin
end;


end.
