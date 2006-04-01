{ $Id$ }
{
 /***************************************************************************

                  Abstract: Set of classes for gathering properties
                        from the database and serve the simulator.
                  Initial Revision : 01/04/2006
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
unit ThermoProvider;

interface

uses
  SysUtils, Classes;

type

  //Describes a dimensional value. UnitID is the ID code defined by the
  //standardized units table.
  TDimensionalValue = record
    Value: Real;
    UnitID: Integer;
  end;
  
  TPropertyProvider= class;

  TCompound = class (TCollectionItem)
  protected
    function GetDisplayName: string; override;
    procedure SetIndex(Value: Integer); override;
  public
    CNUM: TDimensionalValue;
    CompName: TDimensionalValue;
    CPA: TDimensionalValue;
    CPB: TDimensionalValue;
    CPC: TDimensionalValue;
    CPD: TDimensionalValue;
    DELGF: TDimensionalValue;
    DELHF: TDimensionalValue;
    DIPM: TDimensionalValue;
    Formula: TDimensionalValue;
    LDEN: TDimensionalValue;
    MW: TDimensionalValue;
    NEQ: TDimensionalValue;
    PC: TDimensionalValue;
    TB: TDimensionalValue;
    TC: TDimensionalValue;
    TCPHIL: TDimensionalValue;
    TDEN: TDimensionalValue;
    TFP: TDimensionalValue;
    TMAX: TDimensionalValue;
    TMIN: TDimensionalValue;
    VC: TDimensionalValue;
    VPA: TDimensionalValue;
    VPB: TDimensionalValue;
    VPC: TDimensionalValue;
    VPD: TDimensionalValue;
    VSTAR: TDimensionalValue;
    W: TDimensionalValue;
    WSRK: TDimensionalValue;
    ZC: TDimensionalValue;
    Zra: TDimensionalValue;
    constructor Create(ACollection: TCollection); override;
    destructor Destroy; override;
  end;
  
  TCompounds = class (TCollection)
  private
    FPropertyProvider: TPropertyProvider;
    function GetItem(Index: Integer): TCompound;
    procedure SetItem(Index: Integer; Value: TCompound);
  protected
    procedure Update(Item: TCollectionItem); override;
  public
    constructor Create(APropertyProvider: TPropertyProvider);
    function Add: TCompound;
    property Items[Index: Integer]: TCompound read GetItem write SetItem; 
            default;
    property PropertyProvider: TPropertyProvider read FPropertyProvider write 
            FPropertyProvider;
  end;
  
  TPropertyProvider = class (TObject)
  private
    FCompounds: TCompounds;
    procedure Connect;
    procedure Disconnect;
  public
    constructor Create;
    destructor Destroy; override;
    procedure AddCompound(ID: Variant);
    procedure ListAvailableCompounds(AList: TStrings);
    property Compounds: TCompounds read FCompounds write FCompounds;
  end;
  
implementation

{
********************************** TCompounds **********************************
}
constructor TCompounds.Create(APropertyProvider: TPropertyProvider);
begin
  inherited Create(TCompound);
  FPropertyProvider := APropertyProvider;
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
****************************** TPropertyProvider *******************************
}
constructor TPropertyProvider.Create;
begin
  inherited Create;
  FCompounds := TCompounds.Create(Self);
end;

destructor TPropertyProvider.Destroy;
begin
  FCompounds.Free;
  inherited Destroy;
end;

procedure TPropertyProvider.AddCompound(ID: Variant);
begin
end;

procedure TPropertyProvider.Connect;
begin
end;

procedure TPropertyProvider.Disconnect;
begin
end;

procedure TPropertyProvider.ListAvailableCompounds(AList: TStrings);
begin
end;

end.
