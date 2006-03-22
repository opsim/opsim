{ $Id$ }
{
 /***************************************************************************
                               Entities.pas
                               ------------
                  Abstract: Defines the basic exposed elements for 
                            constructiong the solver. Such as variables that
                            build equations that build unit operations.
                  Initial Revision : 22/03/2006
                  Author: Samuel Jorge Marques Cartaxo

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
unit UnitOperation;

interface

uses
  SysUtils, Classes;

type
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
******************************** TUnitOperation ********************************
}
procedure TUnitOperation.Solve;
begin
end;

function TUnitOperation.SolveVariable(AVariable: TVariable): Variant;
begin
end;


end.
