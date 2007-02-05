{ $Id: Equilibrium.pas 310 2006-09-17 20:54:26Z mhenley $ }
{
 /***************************************************************************

                  Abstract: Classes and routines related to equilibrium
                            calculations.
                  Initial Revision : 21/04/2006
                  Authors:
                    - Samuel Jorge Marques Cartaxo
                    - Matt Henley
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
unit Flash;

interface

uses
  SysUtils, Classes, Variants, Entities, Thermo, Eos;

type
  {:
  **************************************************************************** 
  * This Class implements a 3 Phase Flash Alogorithym.  This routine is based * 
  * on alogorithms presented in "Rapid Phase Determination in Multiple-Phase  * 
  * Flash Calculations" by P. A. Nelson.  Comput. chem. Engng, Vol 11, No 6,  * 
  * pp. 581-591,1987.  Some notes for this routine reference equations out of * 
  * that paper directly and it is recommended to have a copy available if when* 
  * trying to understand it.                                                  * 
  *****************************************************************************
  }
  TFlash = class (TObject)
  private
    FEos: TEos;
  public
    property Eos: TEos read FEos write FEos;
  end;
  

implementation



end.
