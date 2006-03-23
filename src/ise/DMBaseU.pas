{ $Id$ }
{
 /***************************************************************************

                  Abstract: The is the main data module for accessing the
                            database. Should centralize the data manipulation
                            whenever appropriate.
                  Initial Revision : 09/03/2006
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
unit DMBaseU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Dialogs;

type

  { TDMBase }

  TDMBase = class (TDataModule)
  end;
  
var
  DMBase: TDMBase;

implementation

initialization

  {$I DMBaseU.lrs}

end.

