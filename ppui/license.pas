unit License;

{$mode objfpc}{$H+}

interface

uses
  Forms, StdCtrls, ButtonPanel;

type

  { TLicenseForm }

  TLicenseForm = class(TForm)
    ButtonPanel1: TButtonPanel;
    Memo1: TMemo;
  private
    { private declarations }
  public
    { public declarations }
  end;

var
  LicenseForm: TLicenseForm;

implementation

{$R *.lfm}

end.

