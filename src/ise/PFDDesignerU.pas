{ $Id$ }
{
 /***************************************************************************

                  Abstract: Module for visually building the PFD diagram.
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
unit PFDDesignerU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ExtCtrls,
  Buttons, PFDCtrls, LCLProc;

type

  { TPFDDesigner }

  TPFDDesigner = class (TForm)
    pnlStatusBar: TPanel;
    pnlToolbar: TPanel;
    pnlWorkplace: TPanel;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    procedure FormCreate(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure pnlWorkplaceMouseMove(Sender: TObject; Shift: TShiftState; X, Y: 
            Integer);
  private
    PFDWorkplace: TPFDWorkplace;
  end;
  
var
  PFDDesigner: TPFDDesigner;

implementation

{ TPFDDesigner }

{
********************************* TPFDDesigner *********************************
}
procedure TPFDDesigner.FormCreate(Sender: TObject);
var
  I: Integer;
begin
  PFDWorkplace := TPFDWorkplace.Create(Self);
  with PFDWorkplace do begin
    Parent := pnlWorkplace;
  end;//with
  
  //Mixer example.
  (*for I := 1 to 10 do begin
    with TPFDMixer.Create(PFDWorkplace) do begin
      Top := Random(300);
      Left := Random(300);
      Scale := 1;
      //Debugln(Format('Width: %s', [IntToStr(Width)]));
      //Debugln(Format('Height: %s', [IntToStr(Height)]));
    end;//with
  end;//for
  with TPFDValve.Create(PFDWorkplace) do begin
    Top := 200;
    Left := 300;
    Scale := 1;
  end;//with*)
end;

procedure TPFDDesigner.FormResize(Sender: TObject);
begin
  
end;

procedure TPFDDesigner.pnlWorkplaceMouseMove(Sender: TObject; Shift: 
        TShiftState; X, Y: Integer);
begin
  
end;

initialization
  {$I PFDDesignerU.lrs}

end.

