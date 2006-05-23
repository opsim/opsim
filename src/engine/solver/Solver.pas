{ $Id$ }
{
 /***************************************************************************

                  Abstract: Implementation of the global solver.
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
unit Solver;

interface

uses
  SysUtils, Classes;

type
  {{
  - Model equations should be in the form:
  
  RESIDUE = F(X1,...,Xn)
  
  - A model equation in the form LEFT = RIGHT
  may be rewritten like:
  
  RESIDUE = RIGHT - LEFT
  
  - A model can contain several equations.
  }
  TEquation = class (TCollectionItem)
  private
    FVariables: TList;
    function GetResidue: Real; virtual;
  public
    property Residue: Real read GetResidue;
    property Variables: TList read FVariables write FVariables;
  end;
  
  TEquations = class (TCollection)
  end;
  
  TSolver = class (TObject)
  private
    FEquations: TEquations;
  public
    procedure Solve; virtual;
    property Equations: TEquations read FEquations write FEquations;
  end;
  
  TLASolver = class (TSolver)
  end;
  
  TNLASolver = class (TSolver)
  end;
  
  TDAESolver = class (TSolver)
  end;
  
  TPDAESolver = class (TSolver)
  end;
  

implementation

{
********************************** TEquation ***********************************
}
function TEquation.GetResidue: Real;
begin
end;

{
*********************************** TSolver ************************************
}
procedure TSolver.Solve;
begin
end;


end.
