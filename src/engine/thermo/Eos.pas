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
    function CalcCompressibilityFactor(APhase: TPhase): TValueRec;
    function CalcFugacityCoefficient(ACompounds: TCompounds): TValueRec;
  end;
  

implementation

{
************************************* TEos *************************************
}
function TEos.CalcCompressibilityFactor(APhase: TPhase): TValueRec;
begin
end;

function TEos.CalcFugacityCoefficient(ACompounds: TCompounds): TValueRec;
begin
end;


end.
