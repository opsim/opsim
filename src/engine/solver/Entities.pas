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

  //Describes a dimensional value. UnitID is the ID code defined by the
  //standardized units table.
  TValue = record
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
    FValues: array of TValue;
    function GetCount: Integer;
    function GetItems(Index: Integer): TValue;
    procedure SetCount(Value: Integer);
    procedure SetItems(Index: Integer; Value: TValue);
  public
    constructor Create;
    destructor Destroy; override;
    procedure Clear;
    procedure Delete(Index: Integer);
    property Count: Integer read GetCount write SetCount;
    property Items[Index: Integer]: TValue read GetItems write SetItems; 
            default;
  end;
  
  TVariable = class (TObject)
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
  TEquation = class (TObject)
  private
    FVariables: TList;
    function GetResidue: Real; virtual;
  public
    property Residue: Real read GetResidue;
    property Variables: TList read FVariables write FVariables;
  end;
  
  TUnitOperation = class (TObject)
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

procedure TValueArray.Clear;
begin
  FValues := nil;
end;

procedure TValueArray.Delete(Index: Integer);
var
  I: Integer;
  NewValues: array of TValue;
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
  Result := High(FValues);
end;

function TValueArray.GetItems(Index: Integer): TValue;
begin
  Result := FValues[Index];
end;

procedure TValueArray.SetCount(Value: Integer);
begin
  SetLength(FValues, Value);
end;

procedure TValueArray.SetItems(Index: Integer; Value: TValue);
begin
  FValues[Index] := Value;
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
