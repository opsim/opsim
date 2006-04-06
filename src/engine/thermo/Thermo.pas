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

  { Forward declarations }
  TMaterial = class;
  TPhases = class;

  TCompound = class (TCollectionItem)
  protected
    function GetDisplayName: string; override;
    procedure SetIndex(Value: Integer); override;
  public
    CompID: Integer;
    CompName: string;
    CPA: TValueRec;
    CPB: TValueRec;
    CPC: TValueRec;
    CPD: TValueRec;
    DELGF: TValueRec;
    DELHG: TValueRec;
    DIPM: TValueRec;
    Formula: string;
    LDEN: TValueRec;
    MW: TValueRec;
    NEQ: TValueRec;
    PC: TValueRec;
    TB: TValueRec;
    TC: TValueRec;
    TCPHIL: TValueRec;
    TDEN: TValueRec;
    TFP: TValueRec;
    TMAX: TValueRec;
    TMIN: TValueRec;
    VC: TValueRec;
    VPA: TValueRec;
    VPB: TValueRec;
    VPC: TValueRec;
    VPD: TValueRec;
    VSTAR: TValueRec;
    W: TValueRec;
    WSRK: TValueRec;
    ZC: TValueRec;
    Zra: TValueRec;
    constructor Create(ACollection: TCollection); override;
    destructor Destroy; override;
  end;
  
  TCompounds = class (TCollection)
  private
    function GetItem(Index: Integer): TCompound;
    procedure SetItem(Index: Integer; Value: TCompound);
  public
    constructor Create;
    function AddCompound: TCompound;
    procedure DeleteCompound(ID: Variant);
    function FindCompound(ID: Variant): TCompound;
    property Compounds[Index: Integer]: TCompound read GetItem write SetItem; 
            default;
  end;
  
  {{
  - A phase is a stable or metastable collection of compounds with a defined 
  amount of substance and a homogeneous composition. It has an associated state 
  of aggregation, e.g. liquid. A given phase can be distinguished from others 
  through the presence of physical interfaces that separate states of matter 
  with different characteristics, such as density (according to CAPE-OPEN).
  - In a material, each phase is given a unique identifier. In order to 
  distinguish multiple instances of similar phases, such as liquids, the phase 
  can be associated with a number of attributes such as a ‘key compound’.
  }
  TPhase = class (TCollectionItem)
  private
    FAggregationState: TAggregationState;
    FCompressFactor: TValueRec;
    FEnthalpy: TValueRec;
    FMassFlow: TValueRec;
    FMolarFractions: TValues;
    FMoleFlow: TValueRec;
    FOverallFraction: TValueRec;
    FStdLiqVolumeFlow: TValueRec;
    FVolumeFlow: TValueRec;
    function GetMaterial: TMaterial;
  public
    constructor Create(Collection: TCollection); override;
    destructor Destroy; override;
    property AggregationState: TAggregationState read FAggregationState write 
            FAggregationState;
    property CompressFactor: TValueRec read FCompressFactor write 
            FCompressFactor;
    property Enthalpy: TValueRec read FEnthalpy write FEnthalpy;
    property MassFlow: TValueRec read FMassFlow write FMassFlow;
    property Material: TMaterial read GetMaterial;
    property MolarFractions: TValues read FMolarFractions;
    property MoleFlow: TValueRec read FMoleFlow write FMoleFlow;
    property OverallFraction: TValueRec read FOverallFraction write 
            FOverallFraction;
    property StdLiqVolumeFlow: TValueRec read FStdLiqVolumeFlow write 
            FStdLiqVolumeFlow;
    property VolumeFlow: TValueRec read FVolumeFlow write FVolumeFlow;
  end;
  
  TPhases = class (TCollection)
  private
    FOwner: TMaterial;
    function GetItem(Index: Integer): TPhase;
    procedure SetItem(Index: Integer; Value: TPhase);
  public
    constructor Create(AMaterial: TMaterial);
    function AddPhase: TPhase;
    property Owner: TMaterial read FOwner write FOwner;
    property Phases[Index: Integer]: TPhase read GetItem write SetItem; default;
  end;
  
  {{
  - A material is a mixture of one or more compounds occurring in one or more 
  phases. A material is characterised by the values of physical properties, 
  which can describe the overall material or the compounds within particular 
  phases. A material often corresponds to a stream in a conventional process 
  simulation package  (according to CAPE-OPEN).
  - It is assumed that all phases in a TMaterial have the same temperature and 
  pressure.
  - It is expected to be most common that a TMaterial has one or two phases.
  }
  TMaterial = class (TObject)
  private
    FCompounds: TCompounds;
    FPhases: TPhases;
    FPressure: TValueRec;
    FTemperature: TValueRec;
    function GetCompounds(Index: Integer): TCompound;
    function GetMassFlow: TValueRec;
    function GetMoleFlow: TValueRec;
    function GetStdLiqVolumeFlow: TValueRec;
    function GetVolumeFlow: TValueRec;
    procedure SetCompounds(Index: Integer; Value: TCompound);
  public
    constructor Create;
    destructor Destroy; override;
    function AddCompound: TCompound;
    procedure DeleteCompound(ID: Variant);
    function FindCompound(ID: Variant): TCompound;
    property Compounds[Index: Integer]: TCompound read GetCompounds write 
            SetCompounds;
    property MassFlow: TValueRec read GetMassFlow;
    property MoleFlow: TValueRec read GetMoleFlow;
    {{
    The list of phases present in the material object. Most commonly, there 
    will be only one phase.
    }
    property Phases: TPhases read FPhases write FPhases;
    property Pressure: TValueRec read FPressure write FPressure;
    {{
    This property will combine the standard volume flow rate of all liquid 
    phases in the material object.
    }
    property StdLiqVolumeFlow: TValueRec read GetStdLiqVolumeFlow;
    property Temperature: TValueRec read FTemperature write FTemperature;
    property VolumeFlow: TValueRec read GetVolumeFlow;
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

function TCompounds.AddCompound: TCompound;
begin
  Result := TCompound(inherited Add);
end;

procedure TCompounds.DeleteCompound(ID: Variant);
var
  C: TCompound;
begin
  C := FindCompound(ID);
  if C <> nil then
    Delete(C.Index);
end;

function TCompounds.FindCompound(ID: Variant): TCompound;
var
  I: Integer;
begin
  Result := nil;
  for I := 0 to Count - 1 do
    if Compounds[I].CompID = ID then begin
      Result := Compounds[I];
      Break;
    end;//if
end;

function TCompounds.GetItem(Index: Integer): TCompound;
begin
  Result := TCompound(inherited GetItem(Index));
end;

procedure TCompounds.SetItem(Index: Integer; Value: TCompound);
begin
  inherited SetItem(Index, Value);
end;

{
************************************ TPhase ************************************
}
constructor TPhase.Create(Collection: TCollection);
begin
  inherited Create(Collection);
  FMolarFractions := TValues.Create;
end;

destructor TPhase.Destroy;
begin
  FMolarFractions.Free;
  inherited Destroy;
end;

function TPhase.GetMaterial: TMaterial;
begin
  Result := (Collection as TPhases).Owner;
end;

{
*********************************** TPhases ************************************
}
constructor TPhases.Create(AMaterial: TMaterial);
begin
  inherited Create(TPhase);
  FOwner := AMaterial;
end;

function TPhases.AddPhase: TPhase;
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
constructor TMaterial.Create;
begin
  inherited Create;
  FCompounds := TCompounds.Create;
  FPhases := TPhases.Create(Self);
end;

destructor TMaterial.Destroy;
begin
  FCompounds.Free;
  FPhases.Free;
  inherited Destroy;
end;

function TMaterial.AddCompound: TCompound;
begin
  Result := FCompounds.AddCompound;
  //
end;

procedure TMaterial.DeleteCompound(ID: Variant);
begin
  FCompounds.DeleteCompound(ID);
  //
end;

function TMaterial.FindCompound(ID: Variant): TCompound;
begin
  Result := FCompounds.FindCompound(ID);
end;

function TMaterial.GetCompounds(Index: Integer): TCompound;
begin
  Result := FCompounds.Compounds[Index];
end;

function TMaterial.GetMassFlow: TValueRec;
begin
end;

function TMaterial.GetMoleFlow: TValueRec;
begin
end;

function TMaterial.GetStdLiqVolumeFlow: TValueRec;
begin
end;

function TMaterial.GetVolumeFlow: TValueRec;
begin
end;

procedure TMaterial.SetCompounds(Index: Integer; Value: TCompound);
begin
  FCompounds.Compounds[Index] := Value;
end;

end.
