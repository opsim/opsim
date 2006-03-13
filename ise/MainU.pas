unit MainU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ActnList,
  StdActns, Menus, ExtCtrls, Buttons, StdCtrls;

type

  { TMain }

  TMain = class(TForm)
    actnlStandardActions: TActionList;
    actnFileExit: TFileExit;
    actnFileOpen: TFileOpen;
    Label1: TLabel;
    miViewPfdWorkspace: TMenuItem;
    miViewUnitopPallet: TMenuItem;
    miView: TMenuItem;
    miAbout: TMenuItem;
    miHelp: TMenuItem;
    mmStandardMenu: TMainMenu;
    miOpen: TMenuItem;
    MenuItem2: TMenuItem;
    miExit: TMenuItem;
    miFile: TMenuItem;
    pnlToolbar: TPanel;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    SpeedButton3: TSpeedButton;
    SpeedButton4: TSpeedButton;
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure miAboutClick(Sender: TObject);
    procedure miExitClick(Sender: TObject);
    procedure miOpenClick(Sender: TObject);
    procedure miViewPfdWorkspaceClick(Sender: TObject);
    procedure miViewUnitopPalletClick(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end; 

var
  Main: TMain;

implementation

uses
  AboutU, UnitopPalletU, PfdDesignerU;

{ TMain }

procedure TMain.miAboutClick(Sender: TObject);
begin
  with TAboutForm.Create(Self) do
  try
    ShowModal;
  finally
    Free;
  end;//try
end;

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

procedure TMain.miExitClick(Sender: TObject);
begin
  Close;
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

initialization
  {$I MainU.lrs}

end.

