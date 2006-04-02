{ $Id$ }
{
 /***************************************************************************

                  Abstract: Module for testing TPropertyProvider.
                  Initial Revision : 02/04/2006
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
unit TPropertyProviderTestU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs;

type
  TfmTPropertyProviderTest = class (TForm)
  end;
  
var
  fmTPropertyProviderTest: TfmTPropertyProviderTest;

implementation

initialization

  {$I TPropertyProviderTestU.lrs}

end.

