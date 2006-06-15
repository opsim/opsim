unit FlashTestMainForm;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, StdCtrls,
  Buttons,SelectComponents, ZConnection, ZDataset, Thermo, ThermoProvider,ActualFlash;

type

  { TMainForm }

  TMainForm = class(TForm)
    SelectComponentsButton: TButton;
    FlashTestButton: TButton;
    Button3: TButton;
    DatabaseConnectToggle: TToggleBox;
    DBConnection: TZConnection;
    DBTable: TZTable;
    procedure FormCreate(Sender:TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure ToggleBox1Click(Sender: TObject);
  private
    { private declarations }
    PropertyProvider:TPropertyProvider;
  public
    { public declarations }
    FlashStrm:TMaterial;
  end; 

var
  MainForm: TMainForm;

implementation


{ TMainForm }

procedure TMainForm.FormCreate(Sender:TObject);
begin
  PropertyProvider:=TPropertyProvider.Create;
  FlashStrm:=TMaterial.Create;
end;

procedure TMainForm.ToggleBox1Click(Sender: TObject);
var
  TempString: String;
begin
  If DatabaseConnectToggle.State = cbChecked then
      begin
       TempString := ExtractFilePath(Application.ExeName) + 'SYSDATA.ODB';
       DBConnection.Database:= TempString;
       DBConnection.Connected := True;
       DBTable.Active:=True;
       SelectComponentsForm.AvailableComponents.Clear;
       SelectComponentsForm.AvailableComponents.Sorted:=True;
       DBTable.First;
       while not DBTable.EOF do
        begin
         TempString:=DBTable['COMPONENT'];
         SelectComponentsForm.AvailableComponents.Items.Add(TempString);
         DBTable.Next;
        end;
      end
    else
      begin
       DBTable.Active:=False;
       DBConnection.Connected := False;
      end;
end;

procedure TMainForm.Button3Click(Sender: TObject);
begin
  Close;
end;

procedure TMainForm.Button1Click(Sender: TObject);
var
  i : integer;
  CompID: String;
  TempString: String;
begin
  CompID:='';
  PropertyProvider.Compounds.Clear;
  PropertyProvider.DBConnection:=DBConnection;
  SelectComponentsForm.ShowModal;
  with SelectComponentsForm do
  begin
    for i := 0 to SelectedComponents.Items.Count - 1 do
    begin
      TempString:=SelectedComponents.Items[i];
      CompID:=DBTable.Lookup('COMPONENT',TempString,'NUMBER');
      PropertyProvider.AddCompound(CompID);
    end;
  end;
  FlashStrm.Compounds.Assign(PropertyProvider.Compounds);
end;

procedure TMainForm.Button2Click(Sender: TObject);
var
  i : integer;
begin
  FlashForm.FlashGrid.RowCount:=FlashStrm.Compounds.Count + 2;
  for i:= 1 to FlashStrm.Compounds.Count do
  begin
    FlashForm.FlashGrid.Cells[0,i]:=FlashStrm.Compounds.Items[i-1].CompName;
  end;
  FlashForm.FlashGrid.Cells[0,FlashStrm.Compounds.Count+1]:='Total';
  FlashForm.ShowModal;
end;

initialization
  {$I flashtestmainform.lrs}

end.

