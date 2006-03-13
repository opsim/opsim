unit PfdDesignerU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ExtCtrls,
  Buttons, PFDCtrls, LCLProc;

type

  { TPfdDesigner }

  TPfdDesigner = class(TForm)
    pnlWorkplace: TPanel;
    pnlStatusBar: TPanel;
    pnlToolbar: TPanel;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    procedure FormCreate(Sender: TObject);
  private
    { private declarations }
    PFDWorkplace: TPFDWorkplace;
  public
    { public declarations }
  end; 

var
  PfdDesigner: TPfdDesigner;

implementation

{ TPfdDesigner }

procedure TPfdDesigner.FormCreate(Sender: TObject);
var
  I: Integer;
begin
  PFDWorkplace := TPFDWorkplace.Create(Self);
  with PFDWorkplace do begin
  	Parent := pnlWorkplace;
  end;//with

  //Mixer example.
  for I := 1 to 10 do begin
  	with TPFDMixer.Create(PFDWorkplace) do begin
      Top := Random(300);
      Left := Random(300);
      Scale := 1;
      //Debugln(Format('Width: %s', [IntToStr(Width)]));
      //Debugln(Format('Height: %s', [IntToStr(Height)]));
    end;//with
  end;//for
	with TPFDMixer.Create(PFDWorkplace) do begin
    Top := 30;
    Left := 100;
    Scale := 1;
  end;//with
end;

initialization
  {$I PfdDesignerU.lrs}

end.

