{
 /***************************************************************************

                  Abstract: Classe related to L-V equilibrium
                            calculations base on Rachford and Rice Algo.
                  Initial Revision : 16/11/2006
                  Authors:
                    - Itinerant
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
unit FlashRachRice;

interface

uses
  SysUtils, Classes, Variants, Entities, Thermo, Flash, CubicEos, SRKEos;

type
  TypeClassFunc = function(X:Real;var DFX:Real) : real of Object;
  
  TFlashRachRice = class (TFlash)
  private
    //FEos: TEos;
    NComp:integer;
  public
    constructor Create(var AMaterial: TMaterial);
    destructor Destroy;override;
    procedure FlashTP(T, P: Real);
    function residus(loVapFrac:Real;var Derivative:Real):Real;

    TheMaterial:TMaterial;
    LPhase,VPhase:TPhase;
    EosVap,EosLiq:TCubicEos;
    LiqComp, VapComp, GlobalComp, KValues, dKValuesdT : Array[0..10] of real;
    VapFrac:Real;
  end;
  
procedure simplestNewRaph(TheFunc:TypeClassFunc; var Xin:Real; MaxIter:integer; Eps:Real;var converged:Boolean);

implementation

constructor TFlashRachRice.Create(var AMaterial: TMaterial);
begin
  inherited Create;
  TheMaterial := AMaterial;
//  TheMaterial := TMaterial.Create;
//  TheMaterial.Assign(AMaterial);
  LPhase:=TheMaterial.Phases.Items[1]; // temporary   Liquid[1]
  VPhase:=TheMaterial.Phases.Items[0];

//  EosVap:=TCubicEos.Create(VPhase);
//  EosLiq:=TCubicEos.Create(LPhase);
  EosVap:=TSRKEos.Create;
  EosVap.UpdateCompounds(VPhase);
  EosLiq:=TSRKEos.Create;
  EosLiq.UpdateCompounds(LPhase);

// For now we suppose that we are in the two phase region
// todo test with the overall composition if it is true

//1. Initialization
  NComp := byte(TheMaterial.Compounds.Count);
{  SetLength(VapComp, NComp);
  SetLength(LiqComp, NComp);
  SetLength(GlobalComp, NComp);
  SetLength(KValues, NComp);
  SetLength(dKValuesdT, NComp);}
end;

destructor TFlashRachRice.Destroy;
begin
//  TheMaterial.free;
  EosVap.free;
  EosLiq.free;
{  VapComp:=nil;
  LiqComp:=nil;
  GlobalComp:=nil;
  KValues:=nil;
  dKValuesdT:=nil;}
  inherited Destroy;
end;

procedure TFlashRachRice.FlashTP(T, P: Real);
var
  J,MaxIter,Iter:integer;
  
  FeedFlow,LiqFlow,VapFlow,Eps,SomError : double;
  somX, somY : double;
  NewK:array of double;
  Converged,NewRaphConv:Boolean;
begin
  VapFrac := 0.5;
  MaxIter := 50;
  Iter := 0;
  Eps := 1.0e-7;
  SetLength(NewK, NComp);
  Converged:=false;

  for J:=0 to NComp-1 do
    GlobalComp[J]:=TheMaterial.Compositions[J].MoleFraction.Value;

//2.Initial K values
  IdealKValue(T,P,TheMaterial.Compounds,KValues,dKValuesdT);
  
//3.Begin loop
  while (not(Converged) and (Iter<MaxIter))do
  begin
    Inc(Iter);
    NewRaphConv := false;
    simplestNewRaph(residus,VapFrac,MaxIter,Eps,NewRaphConv);
    if (NewRaphConv)then
    begin
      SomX := 0;
      SomY := 0;
    
      for J:=0 to NComp-1 do
      begin
        LiqComp[J] := GlobalComp[J]/(1+VapFrac*(KValues[J]-1));
        VapComp[J] := KValues[J]*LiqComp[J];
        SomX := SomX + LiqComp[J];
        SomY := SomY + VapComp[J];
      end;
      for J:=0 to NComp-1 do
      begin
        LiqComp[J] := LiqComp[J]/SomX;
        VapComp[J] := VapComp[J]/SomY;
        LPhase.Compositions[J].MoleFraction.Value:=LiqComp[J];
        VPhase.Compositions[J].MoleFraction.Value:=VapComp[J];
      end;
//  TheMaterial.Phases[0].MoleFlow.Value := TheMaterial.MoleFlow.Value * (1-VapFrac);
//  LPhase.MoleFlow.Value := TheMaterial.MoleFlow.Value * (1-VapFrac);
//  VPhase.MoleFlow.Value := TheMaterial.MoleFlow.Value-LPhase.MoleFlow.Value;

      EosVap.CalcFugacityCoefficients(VPhase);
      EosLiq.CalcFugacityCoefficients(LPhase);
      
      somError := 0;
      for J:=0 to NComp-1 do
      begin
        NewK[J] := LPhase.Compositions[J].FugacityCoefficient.Value / VPhase.Compositions[J].FugacityCoefficient.Value;
        SomError := SomError + Abs(NewK[J]-KValues[J]);
        KValues[J]:=NewK[J];
      end;
    
      Converged := (SomError<Eps);
    end
    else
      Converged := true; // todo correct this

  end;
//end loop
  EosVap.CalcDepartures(VPhase);
  EosLiq.CalcDepartures(LPhase);
  NewK := nil;
end;


function TFlashRachRice.residus(loVapFrac:Real;var Derivative:Real):Real;
var
  J:byte;
  som,Term,OneMinusK, Numerator,Denominator:Real;
begin
  som := 0.0;
  Derivative := 0.0;
  for J:=0 to NComp-1 do
  begin
    OneMinusK := 1.0-KValues[J];
    Numerator :=  GlobalComp[J]*OneMinusK;
    Denominator := (1-loVapFrac*OneMinusK);
    Term := Numerator/Denominator;
    som := som +  Term;
    Derivative := Derivative + Term * OneMinusK / Denominator;
  end;
  Result := som;
end;
procedure simplestNewRaph(TheFunc:TypeClassFunc; var Xin:Real; MaxIter:integer; Eps:Real;var converged:Boolean);
var
  iter:integer;
  FX,DFX,NRError,X,XNew,DX:Real;
begin
  Iter := 0;
  X:=Xin;
  FX:=TheFunc(X,DFX);
  NRError := SQRT(FX*FX);
  Converged := (NRError<Eps);

  while not (Converged) do
  begin
    Inc(Iter);
    if (MaxIter<Iter)then
    begin
      Xin := X;
      exit;
    end;
    DX := FX/DFX;
    XNew := X - DX;
    if (XNew<0)then XNew:=0;
    if (1<XNew)then XNew:=1;
    // todo bound X
    FX:=TheFunc(XNew,DFX);
    NRError := SQRT(FX*FX);
// Tests todo...X-Xnew tests, DFX tests...
    Converged := (NRError<Eps);
    X:=XNew;
  end;
  Xin:=X;
end;
end.
