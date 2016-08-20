unit About;

{$mode objfpc}{$H+}

interface

uses
  Classes, Forms, ExtCtrls, LCLIntf, Controls, StdCtrls, License;

type

  { TAboutForm }

  TAboutForm = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Image1: TImage;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    SFLabel: TLabel;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure SFLabelClick(Sender: TObject);
    procedure SFLabelMouseEnter(Sender: TObject);
    procedure SFLabelMouseLeave(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end;

var
  AboutForm: TAboutForm;

implementation

{$R *.lfm}

{ TAboutForm }

procedure TAboutForm.Button3Click(Sender: TObject);
begin
  Close;
end;

procedure TAboutForm.SFLabelClick(Sender: TObject);
begin
  OpenURL('http://www.opsim.cc/');
end;

procedure TAboutForm.SFLabelMouseEnter(Sender: TObject);
begin
  SFLabel.Cursor := crHandPoint;
  SFLabel.Font.Underline := True;
end;

procedure TAboutForm.SFLabelMouseLeave(Sender: TObject);
begin
  SFLabel.Cursor := crDefault;
  SFLabel.Font.Underline := False;
end;

procedure TAboutForm.Button1Click(Sender: TObject);
begin
end;

procedure TAboutForm.Button2Click(Sender: TObject);
begin
  if LicenseForm = nil then
    LicenseForm := TLicenseForm.Create(nil);

  LicenseForm.ShowModal;

  LicenseForm.Free;
end;

end.

