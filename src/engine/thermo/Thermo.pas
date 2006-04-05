{ $Id$ }
{
 /***************************************************************************

                  Abstract: Definition of fundamental thermo classes.
                  Initial Revision : 05/04/2006
                  Authors:
                    - Samuel Jorge Marques Cartaxo
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
unit Thermo;

interface

uses
  SysUtils, Classes, Entities;

type

  TAggregationState = (asUndefined, asSolid, asLiquid, asVapor);

  TCompound = class (TCollectionItem)
  protected
    function GetDisplayName: string; override;
    procedure SetIndex(Value: Integer); override;
  public
    CompID: Integer;
    CompName: string;
    CPA: TValue;
    CPB: TValue;
    CPC: TValue;
    CPD: TValue;
    DELGF: TValue;
    DELHG: TValue;
    DIPM: TValue;
    Formula: string;
    LDEN: TValue;
    MW: TValue;
    NEQ: TValue;
    PC: TValue;
    TB: TValue;
    TC: TValue;
    TCPHIL: TValue;
    TDEN: TValue;
    TFP: TValue;
    TMAX: TValue;
    TMIN: TValue;
    VC: TValue;
    VPA: TValue;
    VPB: TValue;
    VPC: TValue;
    VPD: TValue;
    VSTAR: TValue;
    W: TValue;
    WSRK: TValue;
    ZC: TValue;
    Zra: TValue;
    constructor Create(ACollection: TCollection); override;
    destructor Destroy; override;
  end;
  
  TCompounds = class (TCollection)
  private
    function GetItem(Index: Integer): TCompound;
    procedure SetItem(Index: Integer; Value: TCompound);
  protected
    procedure Update(Item: TCollectionItem); override;
  public
    constructor Create;
    function Add: TCompound;
    property Items[Index: Integer]: TCompound read GetItem write SetItem; 
            default;
  end;
  
  TPhase = class (TCollectionItem)
  private
    FAggregationState: TAggregationState;
    FOverallFraction: TValue;
    function GetMolarFractions(Index: Integer): TValue;
    procedure SetMolarFractions(Index: Integer; Value: TValue);
  public
    property AggregationState: TAggregationState read FAggregationState write 
            FAggregationState;
    property MolarFractions[Index: Integer]: TValue read GetMolarFractions 
            write SetMolarFractions; default;
    property OverallFraction: TValue read FOverallFraction write 
            FOverallFraction;
  end;
  
  TPhases = class (TCollection)
  private
    function GetItem(Index: Integer): TPhase;
    procedure SetItem(Index: Integer; Value: TPhase);
  public
    constructor Create;
    function Add: TPhase;
    property Items[Index: Integer]: TPhase read GetItem write SetItem; default;
  end;
  
  TMaterial = class (TObject)
  private
    FCompounds: TCompounds;
    FPressure: TValue;
    FTemperature: TValue;
    function GetPhases(Index: Integer): TPhase;
    procedure SetPhases(Index: Integer; const Value: TPhase);
  public
    property Compounds: TCompounds read FCompounds write FCompounds;
    property Phases[Index: Integer]: TPhase read GetPhases write SetPhases;
    property Pressure: TValue read FPressure write FPressure;
    property Temperature: TValue read FTemperature write FTemperature;
  end;
  
implementation

{
********************************** TCompound ***********************************
}
constructor TCompound.Create(ACollection: TCollection);
begin
  inherited Create(ACollection);
end;

destructor TCompound.Destroy;
begin
  inherited Destroy;
end;

function TCompound.GetDisplayName: string;
begin
  if Result = '' then Result := inherited GetDisplayName;
end;

procedure TCompound.SetIndex(Value: Integer);
begin
  inherited SetIndex(Value);
end;

{
********************************** TCompounds **********************************
}
constructor TCompounds.Create;
begin
  inherited Create(TCompound);
end;

function TCompounds.Add: TCompound;
begin
  Result := TCompound(inherited Add);
end;

function TCompounds.GetItem(Index: Integer): TCompound;
begin
  Result := TCompound(inherited GetItem(Index));
end;

procedure TCompounds.SetItem(Index: Integer; Value: TCompound);
begin
  inherited SetItem(Index, Value);
end;

procedure TCompounds.Update(Item: TCollectionItem);
begin
  //Refresh data from the database.
end;

{
************************************ TPhase ************************************
}
function TPhase.GetMolarFractions(Index: Integer): TValue;
begin
end;

procedure TPhase.SetMolarFractions(Index: Integer; Value: TValue);
begin
end;

{
*********************************** TPhases ************************************
}
constructor TPhases.Create;
begin
  inherited Create(TPhase);
end;

function TPhases.Add: TPhase;
begin
  Result := TPhase(inherited Add);
end;

function TPhases.GetItem(Index: Integer): TPhase;
begin
  Result := TPhase(inherited GetItem(Index));
end;

procedure TPhases.SetItem(Index: Integer; Value: TPhase);
begin
  inherited SetItem(Index, Value);
end;

{
********************************** TMaterial ***********************************
}
function TMaterial.GetPhases(Index: Integer): TPhase;
begin
end;

procedure TMaterial.SetPhases(Index: Integer; const Value: TPhase);
begin
end;



end.
