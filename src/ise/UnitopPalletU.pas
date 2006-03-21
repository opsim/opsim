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
    sb3PhaseSeparator: TSpeedButton;
    sbCompressor: TSpeedButton;
    sbCooler: TSpeedButton;
    sbCSTR: TSpeedButton;
    sbExpander: TSpeedButton;
    sbHeater: TSpeedButton;
    sbHeatExchanger: TSpeedButton;
    sbMixer: TSpeedButton;
    sbPFR: TSpeedButton;
    sbPipeSegment: TSpeedButton;
    sbPump: TSpeedButton;
    sbSeparator: TSpeedButton;
    sbTank: TSpeedButton;
    sbTee: TSpeedButton;
    sbValve: TSpeedButton;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    SpeedButton3: TSpeedButton;
    SpeedButton4: TSpeedButton;
    SpeedButton5: TSpeedButton;
    SpeedButton6: TSpeedButton;
    SpeedButton7: TSpeedButton;
    SpeedButton8: TSpeedButton;
    SpeedButton9: TSpeedButton;
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

