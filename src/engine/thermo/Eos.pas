{ $Id$ }
{
 /***************************************************************************

                  Abstract: Base classes for equation of state.
                  Initial Revision : 15/04/2006
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
unit Eos;

interface

uses
  SysUtils, Classes, Entities, Thermo;

type
  {{
  Base class for all equations of state.
  }
  TEos = class (TObject)
  public
    {{
    Calculates the enthalpy an entropy residues at once, since the algorithm is 
    almost the same.
    }
    function CalcDepartures(APhase: TPhase; var Enthalpy, Entropy: TValueRec): 
            TValueRec; virtual;
    function CompressibilityFactor(APhase: TPhase): TValueRec; virtual;
    function EnthalpyDeparture(APhase: TPhase): TValueRec; virtual;
    function EntropyDeparture(APhase: TPhase): TValueRec; virtual;
    {{
    Returns the EOS roots as a variant array.
    }
    function FindRoots: Variant; virtual;
    function FugacityCoefficient(ACompounds: TCompounds): TValueRec; virtual;
  end;
  

implementation

{
************************************* TEos *************************************
}
function TEos.CalcDepartures(APhase: TPhase; var Enthalpy, Entropy: TValueRec): 
        TValueRec;
begin
end;

function TEos.CompressibilityFactor(APhase: TPhase): TValueRec;
begin
end;

function TEos.EnthalpyDeparture(APhase: TPhase): TValueRec;
begin
end;

function TEos.EntropyDeparture(APhase: TPhase): TValueRec;
begin
end;

function TEos.FindRoots: Variant;
begin
end;

function TEos.FugacityCoefficient(ACompounds: TCompounds): TValueRec;
begin
end;

end.
