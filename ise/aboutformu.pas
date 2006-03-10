unit AboutFormU;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ExtCtrls,
  StdCtrls, ComCtrls, Buttons;

type

  { TAboutForm }

  TAboutForm = class(TForm)
    Bevel1: TBevel;
    btnOk: TButton;
    Image1: TImage;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Memo1: TMemo;
    Memo2: TMemo;
    PageControl1: TPageControl;
    Panel1: TPanel;
    tbshAbout: TTabSheet;
    tbshContributors: TTabSheet;
  private
    { private declarations }
  public
    { public declarations }
  end; 

var
  AboutForm: TAboutForm;

implementation

initialization
  {$I aboutformu.lrs}

end.

