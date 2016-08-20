unit Settings;

{$mode objfpc}{$H+}

interface

uses
  Classes, TreeFilterEdit, Forms, ButtonPanel, ComCtrls, ExtCtrls;

type

  { TSettingsForm }

  TSettingsForm = class(TForm)
    ButtonPanel1: TButtonPanel;
    PageControl1: TPageControl;
    Panel1: TPanel;
    Splitter1: TSplitter;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    TreeFilterEdit1: TTreeFilterEdit;
    TreeView1: TTreeView;
    procedure CancelButtonClick(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure OKButtonClick(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end;

var
  SettingsForm: TSettingsForm;

implementation

{$R *.lfm}

{ TSettingsForm }

procedure TSettingsForm.CancelButtonClick(Sender: TObject);
begin
  Close;
end;

procedure TSettingsForm.FormActivate(Sender: TObject);
begin
  //read the settings
  //MovieDatabaseEdit.Caption := PropertiesINIReadString('Settings', 'MovieDB', '');

end;

procedure TSettingsForm.OKButtonClick(Sender: TObject);
begin
  //save the settings and exit
  //AppProps.WriteString('Settings', 'MovieSearchPath', MovieSearchPathsListBox.Items.DelimitedText);

  Close;
end;

end.

