{ $Id$ }
{
 /***************************************************************************

                  Abstract: Defines the basic exposed elements for
                            constructiong the solver. Such as variables that
                            build equations that build unit operations.
                  Initial Revision : 22/03/2006
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
unit Entities;

interface

uses
  SysUtils, Classes;

type

  TCollectionNotifyEvent = procedure (Item: TCollectionItem;  Action: 
          TCollectionNotification) of object;
  //Describes a dimensional value. UnitID is the ID code defined by the
  //standardized units table.
  TValueRec = record
    Value: Variant;
    UnitID: Integer;
    Calculated: Boolean;
    UserDefined: Boolean;
  end;
  
  {{
  Implements a dynamic array of TValues.
  }
  TValueArray = class (TObject)
  private
    FValues: array of TValueRec;
    function GetCount: Integer;
    function GetItems(Index: Integer): TValueRec;
    procedure SetCount(Value: Integer);
    procedure SetItems(Index: Integer; Value: TValueRec);
  public
    constructor Create;
    destructor Destroy; override;
    function Add(const Item: TValueRec): Integer;
    procedure Clear;
    procedure Delete(Index: Integer);
    property Count: Integer read GetCount write SetCount;
    property Items[Index: Integer]: TValueRec read GetItems write SetItems; 
            default;
  end;
  
  TValue = class (TCollectionItem)
  private
    FData: TValueRec;
  public
    property Data: TValueRec read FData write FData;
  end;
  
  TValues = class (TCollection)
  private
    function GetItem(Index: Integer): TValue;
    procedure SetItem(Index: Integer; Value: TValue);
  public
    constructor Create;
    function Add: TValue;
    property Items[Index: Integer]: TValue read GetItem write SetItem; default;
  end;
  
  TVariable = class (TPersistent)
  private
    FName: string;
    FValue: Variant;
  published
    property Name: string read FName write FName;
    property Value: Variant read FValue write FValue;
  end;
  
  {{
  - Model equations should be in the form:
  
  RESIDUE = F(X1,...,Xn)
  
  - A model equation in the form LEFT = RIGHT
  may be rewritten like:
  
  RESIDUE = RIGHT - LEFT
  
  - A model can contain several equations.
  }
  TEquation = class (TPersistent)
  private
    FVariables: TList;
    function GetResidue: Real; virtual;
  public
    property Residue: Real read GetResidue;
    property Variables: TList read FVariables write FVariables;
  end;
  
  TUnitOperation = class (TPersistent)
  private
    FEquations: TList;
    FName: string;
  public
    procedure Solve; virtual;
    function SolveVariable(AVariable: TVariable): Variant; virtual;
    property Equations: TList read FEquations write FEquations;
  published
    property Name: string read FName write FName;
  end;
  

implementation

{
********************************* TValueArray **********************************
}
constructor TValueArray.Create;
begin
  inherited Create;
end;

destructor TValueArray.Destroy;
begin
  //Frees the allocated memory.
  Clear;
  inherited Destroy;
end;

function TValueArray.Add(const Item: TValueRec): Integer;
begin
  //Create room for new element.
  SetLength(FValues, Length(FValues) + 1);
  //Copy the new item and return the index.
  FValues[High(FValues)] := Item;
  Result := High(FValues);
end;

procedure TValueArray.Clear;
begin
  FValues := nil;
end;

procedure TValueArray.Delete(Index: Integer);
var
  I: Integer;
  NewValues: array of TValueRec;
begin
  //** This is a time consuming routine, so use it with care! To delete all
  //elements, call Clear method.
  
  //Copy all elements, except the deleted one.
  for I := Low(FValues) to High(FValues) do
    if I <> Index then begin
      //Creates room for the new value.
      SetLength(NewValues, Length(NewValues) + 1);
      //Copy the element.
      NewValues[High(NewValues)] := FValues[I];
    end;//if
  //Clear the old array.
  FValues := nil;
  //Reference the new one.
  FValues := NewValues;
end;

function TValueArray.GetCount: Integer;
begin
  Result := Length(FValues);
end;

function TValueArray.GetItems(Index: Integer): TValueRec;
begin
  Result := FValues[Index];
end;

procedure TValueArray.SetCount(Value: Integer);
begin
  SetLength(FValues, Value);
end;

procedure TValueArray.SetItems(Index: Integer; Value: TValueRec);
begin
  FValues[Index] := Value;
end;

{
*********************************** TValues ************************************
}
constructor TValues.Create;
begin
  inherited Create(TValue);
end;

function TValues.Add: TValue;
begin
  Result := TValue(inherited Add);
end;

function TValues.GetItem(Index: Integer): TValue;
begin
  Result := TValue(inherited GetItem(Index));
end;

procedure TValues.SetItem(Index: Integer; Value: TValue);
begin
  inherited SetItem(Index, Value);
end;

{
********************************** TEquation ***********************************
}
function TEquation.GetResidue: Real;
begin
end;

{
******************************** TUnitOperation ********************************
}
procedure TUnitOperation.Solve;
begin
end;

function TUnitOperation.SolveVariable(AVariable: TVariable): Variant;
begin
end;

end.
