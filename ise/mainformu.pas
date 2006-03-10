unit MainFormU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ActnList,
  StdActns, Menus, ExtCtrls, Buttons, StdCtrls;

type

  { TMainForm }

  TMainForm = class(TForm)
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
  MainForm: TMainForm;

implementation

uses
  AboutFormU, UnitopPalletFormU, PfdWorkSpaceFormU;

{ TMainForm }

procedure TMainForm.miAboutClick(Sender: TObject);
begin
  with TAboutForm.Create(Self) do
  try
    ShowModal;
  finally
    Free;
  end;//try
end;

procedure TMainForm.FormCreate(Sender: TObject);
begin
  Left := 0;
  Top := 0;
  Width := Screen.Width;
end;

procedure TMainForm.FormShow(Sender: TObject);
begin
  miViewUnitopPalletClick(Self);
  miViewPfdWorkspaceClick(Self);
end;

procedure TMainForm.miExitClick(Sender: TObject);
begin
  Close;
end;

procedure TMainForm.miOpenClick(Sender: TObject);
begin
  ShowMessage('Nothing yet!');
end;

procedure TMainForm.miViewPfdWorkspaceClick(Sender: TObject);
begin
  PfdWorkSpaceForm.Show;
end;

procedure TMainForm.miViewUnitopPalletClick(Sender: TObject);
begin
  UnitopPalletForm.Show;
end;

initialization
  {$I mainformu.lrs}

end.

