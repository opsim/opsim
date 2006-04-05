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
