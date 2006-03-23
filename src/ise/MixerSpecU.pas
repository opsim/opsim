{ $Id$ }
{
 /***************************************************************************

                  Abstract: This is the specs form for the Mixer
                            unit operation.
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
unit MixerSpecU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, DB,
  DBGrids;

type

  { TMixerSpec }

  TMixerSpec = class (TForm)
    dbGrid1: TdbGrid;
  end;
  
var
  MixerSpec: TMixerSpec;

implementation

uses
  DMBaseU;

initialization

  {$I MixerSpecU.lrs}

end.

