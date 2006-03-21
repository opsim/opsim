{ $Id$ }
{
 /***************************************************************************
                               UnitopPalletU.pas
                               -----------------
                  Abstract: Implements the component pallet for drag/drop
                            unit operations.
                  Initial Revision : 09/03/2006
                  Author: Samuel Jorge Marques Cartaxo


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
unit UnitopPalletU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ExtCtrls,
  Buttons, XMLPropStorage, IniPropStorage;

type

  { TUnitopPallet }

  TUnitopPallet = class (TForm)
    pnlBase: TPanel;
    pnlToolbar1: TPanel;
    pnlToolbar2: TPanel;
    pnlToolbar3: TPanel;
    sbCSTR: TSpeedButton;
    sbTee: TSpeedButton;
    sbHeatExchanger: TSpeedButton;
    sbTank: TSpeedButton;
    SpeedButton13: TSpeedButton;
    SpeedButton14: TSpeedButton;
    SpeedButton15: TSpeedButton;
    sbSeparator: TSpeedButton;
    sbValve: TSpeedButton;
    sbCooler: TSpeedButton;
    sbPipeSegment: TSpeedButton;
    sbCompressor: TSpeedButton;
    SpeedButton20: TSpeedButton;
    SpeedButton21: TSpeedButton;
    sbExpander: TSpeedButton;
    sb3PhaseSeparator: TSpeedButton;
    SpeedButton24: TSpeedButton;
    sbHeater: TSpeedButton;
    sbPump: TSpeedButton;
    SpeedButton5: TSpeedButton;
    SpeedButton6: TSpeedButton;
    SpeedButton7: TSpeedButton;
    sbMixer: TSpeedButton;
    sbPFR: TSpeedButton;
    procedure FormCreate(Sender: TObject);
    procedure XMLPropStorage1SavingProperties(Sender: TObject);
  end;
  
var
  UnitopPallet: TUnitopPallet;

implementation

{ TUnitopPallet }

{
******************************** TUnitopPallet *********************************
}
procedure TUnitopPallet.FormCreate(Sender: TObject);
begin
  //Positon on the left of the screen.
  Left := Screen.Width - Width;
  //Define size constraints based on design time dimensions.
  with Constraints do begin
    MinHeight := Height;
    MaxHeight := Height;
    MinWidth := Width;
    MaxWidth := Width;
  end;//with
end;

procedure TUnitopPallet.XMLPropStorage1SavingProperties(Sender: TObject);
begin

end;

initialization
  {$I UnitopPalletU.lrs}

end.

