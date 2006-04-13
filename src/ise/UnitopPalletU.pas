{ $Id$ }
{
 /***************************************************************************

                  Abstract: Implements the component pallet for drag/drop
                            unit operations.
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
unit UnitopPalletU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ExtCtrls,
  Buttons, PFDCtrls;

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
    procedure UnitOperationClick(Sender: TObject);
  private
    FNewPFDControl: TPFDControl;
    procedure SetNewPFDControl(Value: TPFDControl);
  public
    property NewPFDControl: TPFDControl read FNewPFDControl write 
            SetNewPFDControl;
  end;
  
var
  UnitopPallet: TUnitopPallet;

implementation

uses
  PFDDesignerU;

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

procedure TUnitopPallet.SetNewPFDControl(Value: TPFDControl);
var
  I: Integer;
begin
  FNewPFDControl := Value;
  //Reset all buttons to normal state (not down).
  if FNewPFDControl = nil then
    for I := 0 to ComponentCount - 1 do
      if Components[I] is TSpeedButton then
        TSpeedButton(Components[I]).Down := False;
end;

procedure TUnitopPallet.UnitOperationClick(Sender: TObject);
var
  P: TPoint;
begin
  if FNewPFDControl <> nil then
    FreeAndNil(FNewPFDControl);
  if (Sender as TSpeedButton).Down then begin
    if Sender = sbMixer then
      FNewPFDControl := TPFDMixer.Create(PFDDesigner.PFDWorkplace)
    else
    if Sender = sbValve then
      FNewPFDControl := TPFDValve.Create(PFDDesigner.PFDWorkplace)
    else
      Exit;
    //Prepares the new PFD control.
    with FNewPFDControl do begin
      P := PFDDesigner.PFDWorkplace.ScreenToClient(Mouse.CursorPos);
      Visible := False;
      Selected := True;
      SetBounds(P.X - Width, P.Y - Height, Width, Height);
      DrawWireFrame := True;
    end;//with
  end;//if
end;

initialization
  {$I UnitopPalletU.lrs}

end.

