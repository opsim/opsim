{ $Id$ }
{
 /***************************************************************************

                  Abstract: The main for of the Integrated Simulation
                            Environment (ISE).
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
unit MainU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ActnList,
  StdActns, Menus, ExtCtrls, Buttons, StdCtrls;

type

  { TMain }

  TMain = class (TForm)
    actnFileExit: TFileExit;
    actnFileOpen: TFileOpen;
    actnlStandardActions: TActionList;
    Label1: TLabel;
    MenuItem2: TMenuItem;
    miAbout: TMenuItem;
    miClose: TMenuItem;
    miExit: TMenuItem;
    miFile: TMenuItem;
    miHelp: TMenuItem;
    miNew: TMenuItem;
    miOpen: TMenuItem;
    miView: TMenuItem;
    miViewPfdWorkspace: TMenuItem;
    miViewUnitopPallet: TMenuItem;
    mmStandardMenu: TMainMenu;
    pnlToolbar: TPanel;
    sbExit: TSpeedButton;
    sbNew: TSpeedButton;
    sbOpen: TSpeedButton;
    sbSave: TSpeedButton;
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure miAboutClick(Sender: TObject);
    procedure miCloseClick(Sender: TObject);
    procedure miExitClick(Sender: TObject);
    procedure miNewClick(Sender: TObject);
    procedure miOpenClick(Sender: TObject);
    procedure miViewPfdWorkspaceClick(Sender: TObject);
    procedure miViewUnitopPalletClick(Sender: TObject);
    procedure sbExitClick(Sender: TObject);
  public
    procedure NewSimulationCase;
  end;
  
var
  Main: TMain;

implementation

uses
  AboutU, UnitopPalletU, PFDDesignerU;

{ TMain }

{
************************************ TMain *************************************
}
procedure TMain.FormCreate(Sender: TObject);
begin
  Left := 0;
  Top := 0;
  Width := Screen.Width;
end;

procedure TMain.FormShow(Sender: TObject);
begin
  miViewUnitopPalletClick(Self);
  miViewPfdWorkspaceClick(Self);
end;

procedure TMain.miAboutClick(Sender: TObject);
begin
  with TAboutForm.Create(Self) do
  try
    ShowModal;
  finally
    Free;
  end;//try
end;

procedure TMain.miCloseClick(Sender: TObject);
begin
  //Close opened case.
end;

procedure TMain.miExitClick(Sender: TObject);
begin
  Close;
end;

procedure TMain.miNewClick(Sender: TObject);
begin
  NewSimulationCase;
end;

procedure TMain.miOpenClick(Sender: TObject);
begin
  ShowMessage('Nothing yet!');
end;

procedure TMain.miViewPfdWorkspaceClick(Sender: TObject);
begin
  PfdDesigner.Show;
end;

procedure TMain.miViewUnitopPalletClick(Sender: TObject);
begin
  UnitopPallet.Show;
end;

procedure TMain.NewSimulationCase;
begin
end;

procedure TMain.sbExitClick(Sender: TObject);
begin
  Close;
end;

initialization
  {$I MainU.lrs}

end.

