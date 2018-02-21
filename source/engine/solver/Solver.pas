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
  {:
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
  
  {:
  - Defines a linear system of equations.
  }
  TAlgebraicEquation = class (TEquation)
  end;
  
  {:
  - Inherits from class TAlgebraicEquation and refines it to define a mixed set 
  of differential and algebraic equations.
  }
  TDifferentialAlgebraicEquation = class (TAlgebraicEquation)
  end;
  
  {:
  - The definition of large sets of nonlinear equations of any kind generally 
  requires a large amount of relatively complex data. This has led us to 
  introduce the concept of an Equation Set Object (ESO) as a means of defining 
  this information in a way that can be accessed and used by instances of 
  NLASolvers and DAESolvers. The structure of the ESO is, therefore, central to 
  the interface definitions which are the ultimate goal of this work. The 
  Equation Set Object is an abstraction representing a square or rectangular 
  set of equations. These are the equations that define the physical behaviour 
  of the process1 under consideration, and which must be solved within a 
  flowsheeting problem. The interface to this object is intended to serve the 
  needs of the various solver objects by allowing them to obtain information 
  about the size and structure of the system, to adjust the values of variables 
  occurring in it, and to compute the resulting equation residuals and, 
  potentially, other related information (e.g. partial der
  ivatives). Hence, this interface requires standardisation as part of 
  CAPE-OPEN. However, the construction of such an object will be a proprietary 
  matter for individual vendors of flowsheeting packages and will be 
  standardised as part of CAPE-OPEN.
  }
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
