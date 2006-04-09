{ $Id$ }
{
 /***************************************************************************

                  Abstract: Ports definition classes.
                  Initial Revision : 09/04/2006
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
unit Ports;

interface

uses
  SysUtils, Classes, Entities, Thermo;

type
  TPort = class (TPersistent)
  private
    FDirection: Integer;
    FName: Integer;
  public
    property Direction: Integer read FDirection write FDirection;
    property Name: Integer read FName write FName;
  end;
  
  TMaterialPort = class (TPort)
  private
    FMaterial: TMaterial;
  public
    property Material: TMaterial read FMaterial write FMaterial;
  end;
  
  TEnergyPort = class (TPort)
  private
    FFlux: Integer;
  public
    property Flux: Integer read FFlux write FFlux;
  end;
  
  TInformationPort = class (TPort)
  end;
  

implementation


end.
