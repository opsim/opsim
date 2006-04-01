unit PhysicalPropertyExplorerU;
{ $Id$ }
{
 /***************************************************************************

                  Abstract: Iterface for inspecting the physical properties
                            data on database.
                  Initial Revision : 01/04/2006
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
{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ZDataset;

type

  { TPhysicalPropertyExplorer }

  TPhysicalPropertyExplorer = class(TForm)
    ZTable1: TZTable;
  private
    { private declarations }
  public
    { public declarations }
  end; 

var
  PhysicalPropertyExplorer: TPhysicalPropertyExplorer;

implementation

initialization
  {$I PhysicalPropertyExplorerU.lrs}

end.

