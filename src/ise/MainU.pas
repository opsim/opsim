{ $Id$ }
{
 /***************************************************************************

                  Abstract: The main form of the Integrated Simulation
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
    MenuItem1: TMenuItem;
    MenuItem2: TMenuItem;
    miTPropertyProviderTest: TMenuItem;
    miTest: TMenuItem;
    miAbout: TMenuItem;
    miClose: TMenuItem;
    miDatabaseConnect: TMenuItem;
    miDatabaseConnection: TMenuItem;
    miDatabaseDisconnect: TMenuItem;
    miExit: TMenuItem;
    miFile: TMenuItem;
    miHelp: TMenuItem;
    miNew: TMenuItem;
    miOpen: TMenuItem;
    miPhysicalPropertyExplorer: TMenuItem;
    miView: TMenuItem;
    miViewPFDDesigner: TMenuItem;
    miViewUnitopPallet: TMenuItem;
    mmStandardMenu: TMainMenu;
    pnlToolbar: TPanel;
    sbAbout: TSpeedButton;
    sbExit: TSpeedButton;
    sbNew: TSpeedButton;
    sbOpen: TSpeedButton;
    sbSave: TSpeedButton;
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure miAboutClick(Sender: TObject);
    procedure miCloseClick(Sender: TObject);
    procedure miDatabaseConnectClick(Sender: TObject);
    procedure miDatabaseDisconnectClick(Sender: TObject);
    procedure miExitClick(Sender: TObject);
    procedure miNewClick(Sender: TObject);
    procedure miOpenClick(Sender: TObject);
    procedure miPhysicalPropertyExplorerClick(Sender: TObject);
    procedure miTPropertyProviderTestClick(Sender: TObject);
    procedure miViewPfdWorkspaceClick(Sender: TObject);
    procedure miViewUnitopPalletClick(Sender: TObject);
    procedure sbExitClick(Sender: TObject);
  public
    procedure NewSimulationCase;
    procedure UpdateInterface;
  end;
  
var
  Main: TMain;

implementation

uses
  AboutU, UnitopPalletU, PFDDesignerU, DMBaseU, PhysicalPropertyExplorerU;

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
  UpdateInterface;
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

procedure TMain.miDatabaseConnectClick(Sender: TObject);
begin
  DMBase.Connected := True;
  UpdateInterface;
end;

procedure TMain.miDatabaseDisconnectClick(Sender: TObject);
begin
  DMBase.Connected := False;
  UpdateInterface;
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

procedure TMain.miPhysicalPropertyExplorerClick(Sender: TObject);
begin
  if PhysicalPropertyExplorer = nil then
    PhysicalPropertyExplorer := TPhysicalPropertyExplorer.Create(Self);
  PhysicalPropertyExplorer.Show;
  UpdateInterface;
end;

procedure TMain.miTPropertyProviderTestClick(Sender: TObject);
begin
  //
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

procedure TMain.UpdateInterface;
begin
  miDatabaseConnect.Enabled := not DMBase.Connected;
  miDatabaseDisconnect.Enabled := DMBase.Connected;
end;

initialization
  {$I MainU.lrs}

end.

